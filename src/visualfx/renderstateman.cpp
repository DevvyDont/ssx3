#include "common.h"
#include "worldpainter/worldpainterquery.h"
#include "visualfx/renderstateman.h"
#include "render/2drender.h"
#include "main/game.h"
#include "ai/rider.h"
#include "bx/math3d.h"
#include "vtable.h"
#include "visualfx/visualeffectsman.h"
#include "visualfx/aivisualeffectsman.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourR);
#ifdef SKIP_ASM
struct sRSMVEntry {
    short delta;
    short index;
    float* (*fn)(void*);
};

struct sRSMObj {
    int field_0x0;
    sRSMVEntry* vt;
};


extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the red component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of cRenderStateMan_m_state (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 55
 * returns a pointer to the value.
 *
 * @param i  row of cRenderStateMan_m_state (the avalanche effect passes 0)
 * @return the red component
 */
extern "C" float cRenderStateMan_SnowFlakeColourR(int i)
{
    tRenderState* entry = &cRenderStateMan_m_state[i];
    tWPInGameData* stateObj = entry->m_query[8]->m_data;
    return stateObj->SnowFlakeColourR();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourG);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the green component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of cRenderStateMan_m_state (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 56
 * returns a pointer to the value.
 *
 * @param i  row of cRenderStateMan_m_state (the avalanche effect passes 0)
 * @return the green component
 */
extern "C" float cRenderStateMan_SnowFlakeColourG(int i)
{
    tRenderState* entry = &cRenderStateMan_m_state[i];
    tWPInGameData* stateObj = entry->m_query[8]->m_data;
    return stateObj->SnowFlakeColourG();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourB);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the blue component of the snowflake colour for one row of the render-state table.
 *
 * Row @p i of cRenderStateMan_m_state (0xF0 bytes per row) holds at +0x20 a pointer to an object whose virtual slot 57
 * returns a pointer to the value.
 *
 * @param i  row of cRenderStateMan_m_state (the avalanche effect passes 0)
 * @return the blue component
 */
extern "C" float cRenderStateMan_SnowFlakeColourB(int i)
{
    tRenderState* entry = &cRenderStateMan_m_state[i];
    tWPInGameData* stateObj = entry->m_query[8]->m_data;
    return stateObj->SnowFlakeColourB();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourA);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow fluff colour's alpha for render state @p i.
 *
 * Calls virtual slot 58 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as .x of the second snow colour set.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffColourA(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[8]->m_data;
    return stateQuery->SnowFluffColourA();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourR);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow fluff colour's red for render state @p i.
 *
 * Calls virtual slot 59 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as .y of the second snow colour set.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffColourR(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[8]->m_data;
    return stateQuery->SnowFluffColourR();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourG);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow fluff colour's green for render state @p i.
 *
 * Calls virtual slot 60 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as .z of the second snow colour set.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffColourG(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[8]->m_data;
    return stateQuery->SnowFluffColourG();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourB);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow fluff colour's blue for render state @p i.
 *
 * Calls virtual slot 61 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cSnowFall_update uses it as .w of the second snow colour set.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowFluffColourB(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[8]->m_data;
    return stateQuery->SnowFluffColourB();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogDensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog density for render state @p i.
 *
 * Calls virtual slot 7 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in cDepthFog_m_Density
 * (for the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogDensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogDensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogNear);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog near distance for render state @p i.
 *
 * Calls virtual slot 8 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in cDepthFog_m_NearPlane
 * (for the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogNear(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogNear();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogFar);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog far distance for render state @p i.
 *
 * Calls virtual slot 9 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in cDepthFog_m_FarPlane
 * (for the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogFar(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogFar();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourR);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog colour's red for render state @p i.
 *
 * Calls virtual slot 10 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in the fog colour (for
 * the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogColourR(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogColourR();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourG);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog colour's green for render state @p i.
 *
 * Calls virtual slot 11 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in the fog colour (for
 * the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogColourG(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogColourG();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourB);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the fog colour's blue for render state @p i.
 *
 * Calls virtual slot 12 of the state's world-painter query object m_query[1] (+0x4) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in the fog colour (for
 * the camera's state) and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_FogColourB(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[1]->m_data;
    return stateQuery->FogColourB();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleR);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint scale's red for render state @p i (1 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 20 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourR and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleR(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintScaleR();
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleG);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint scale's green for render state @p i (1 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 21 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourG and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleG(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintScaleG();
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleB);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint scale's blue for render state @p i (1 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 22 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourB and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleB(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintScaleB();
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetR);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint offset's red for render state @p i (0 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 23 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillR and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetR(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintOffsetR();
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetG);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint offset's green for render state @p i (0 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 24 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillG and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetG(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintOffsetG();
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetB);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_indoor;

/**
 * Returns the screen tint offset's blue for render state @p i (0 while cRenderStateMan_m_indoor is set).
 *
 * Calls virtual slot 25 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag cRenderStateMan_m_indoor is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillB and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetB(int i)
{
    if (cRenderStateMan_m_indoor == 0) {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[3]->m_data;
        return stateQuery->ScreenTintOffsetB();
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCutoff);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow cutoff for render state @p i.
 *
 * Calls virtual slot 13 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2MinIntensity.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowCutoff(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowCutoff();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCutoffScale);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow cutoff scale for render state @p i.
 *
 * Calls virtual slot 14 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2ModIntensity.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowCutoffScale(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowCutoffScale();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCopyIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow copy intensity for render state @p i.
 *
 * Calls virtual slot 15 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2CopyIntensity.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowCopyIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowCopyIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowSourceIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow source intensity for render state @p i.
 *
 * Calls virtual slot 16 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2FrameSrcScale.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowSourceIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowSourceIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowFrameIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow frame intensity for render state @p i.
 *
 * Calls virtual slot 17 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2FrameDstScale.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowFrameIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowFrameIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowBlend2);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow's second blend intensity for render state @p i.
 *
 * Calls virtual slot 18 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2IntensityTex2.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowBlend2(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowBlend2();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowBlend3);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the light glow's third blend intensity for render state @p i.
 *
 * Calls virtual slot 19 of the state's world-painter query object m_query[2] (+0x8) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cVisualEffectsMan_commonUpdate stores it in
 * cLightGlow_m_PS2IntensityTex3.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_LightGlowBlend3(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[2]->m_data;
    return stateQuery->LightGlowBlend3();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_RimLightIntensityOffset);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the rim light intensity offset for render state @p i (accessor slot 42).
 *
 * Calls virtual slot 42 of the state's world-painter query object m_query[7] (+0x1C) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cRider_SetRiderLights passes it with the rider's
 * kRiderVt_getAIIndex to func_00389CB8.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_RimLightIntensityOffset(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
    return stateQuery->RimLightIntensityOffset();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_AmbientLightIntensityOffset);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the ambient light intensity offset for render state @p i.
 *
 * Calls virtual slot 41 of the state's world-painter query object m_query[7] (+0x1C) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cRenderStateMan_update uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_AmbientLightIntensityOffset(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
    return stateQuery->AmbientLightIntensityOffset();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowSparkleDensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the snow sparkle density for render state @p i, reduced inside tunnels.
 *
 * Calls virtual slot 36 of the state's world-painter query object m_query[6] (+0x18) of cRenderStateMan_m_state[@p i]
 * (0xF0-byte rows) and scales the value it points to by (1 - the row's +0x24, On Tour m_inTunnel).
 * cVisualEffectsMan_commonUpdate stores it in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_SnowSparkleDensity(int i)
{
    tRenderState* entry = &cRenderStateMan_m_state[i];
    tWPInGameData* object = entry->m_query[6]->m_data;
    return object->SnowSparkleDensity() * (1.0f - entry->m_inTunnel);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_PlayerWindIntensity);
#ifdef SKIP_ASM
extern tRenderState cRenderStateMan_m_state[];

/**
 * Returns the wind intensity felt by players for render state @p i.
 *
 * Calls virtual slot 47 of the state's world-painter query object m_query[8] (+0x20) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. cRider_updateWind uses it.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_PlayerWindIntensity(int i)
{
    tRenderState* renderState = &cRenderStateMan_m_state[i];
    tWPInGameData* stateQuery = renderState->m_query[8]->m_data;
    return stateQuery->PlayerWindIntensity();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightIrradianceName);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_hasCustomIrradiance;
extern int cRenderStateMan_m_customIrradiance;

/**
 * Returns the light irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While cRenderStateMan_m_hasCustomIrradiance is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field cRenderStateMan_m_customIrradiance; otherwise
 * it calls virtual slot 37 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_LightIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_LightIrradianceName(int i)
{
    if (cRenderStateMan_m_hasCustomIrradiance != 0) {
        return &cRenderStateMan_m_customIrradiance;
    }
    {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
        sRSMVEntry* vt = (sRSMVEntry*)VPTR_OF(tWPInGameData, stateQuery);
        return vt[37].fn(vthis(stateQuery, vt[37]));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ShadowIrradianceName);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_hasCustomIrradiance;
extern int cRenderStateMan_m_customIrradiance;

/**
 * Returns the shadow irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While cRenderStateMan_m_hasCustomIrradiance is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field cRenderStateMan_m_customIrradiance; otherwise
 * it calls virtual slot 38 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_ShadowIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_ShadowIrradianceName(int i)
{
    if (cRenderStateMan_m_hasCustomIrradiance != 0) {
        return &cRenderStateMan_m_customIrradiance;
    }
    {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
        sRSMVEntry* vt = (sRSMVEntry*)VPTR_OF(tWPInGameData, stateQuery);
        return vt[38].fn(vthis(stateQuery, vt[38]));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_TunnelIrradianceName);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_hasCustomIrradiance;
extern int cRenderStateMan_m_customIrradiance;

/**
 * Returns the tunnel irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While cRenderStateMan_m_hasCustomIrradiance is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field cRenderStateMan_m_customIrradiance; otherwise
 * it calls virtual slot 39 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_TunnelIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_TunnelIrradianceName(int i)
{
    if (cRenderStateMan_m_hasCustomIrradiance != 0) {
        return &cRenderStateMan_m_customIrradiance;
    }
    {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
        sRSMVEntry* vt = (sRSMVEntry*)VPTR_OF(tWPInGameData, stateQuery);
        return vt[39].fn(vthis(stateQuery, vt[39]));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ObjectIrradianceName);
#ifdef SKIP_ASM
extern int cRenderStateMan_m_hasCustomIrradiance;
extern int cRenderStateMan_m_customIrradiance;

/**
 * Returns the object irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While cRenderStateMan_m_hasCustomIrradiance is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field cRenderStateMan_m_customIrradiance; otherwise
 * it calls virtual slot 40 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_ObjectIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_ObjectIrradianceName(int i)
{
    if (cRenderStateMan_m_hasCustomIrradiance != 0) {
        return &cRenderStateMan_m_customIrradiance;
    }
    {
        tRenderState* renderState = &cRenderStateMan_m_state[i];
        tWPInGameData* stateQuery = renderState->m_query[7]->m_data;
        sRSMVEntry* vt = (sRSMVEntry*)VPTR_OF(tWPInGameData, stateQuery);
        return vt[40].fn(vthis(stateQuery, vt[40]));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_writeToReplayFrame);
#ifdef SKIP_ASM
class cRSMObjWF {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64(void* frame);
};

/**
 * Records the render-state manager's per-viewer state into a replay frame.
 *
 * For rows 0 to 5 of cRenderStateMan_m_state it calls virtual slot 64 of the object behind the row's +0x20 pointer with
 * @p frame, twice for the same object (On Tour does the same).
 *
 * @param frame  the replay frame being written (On Tour data)
 */
extern "C" void cRenderStateMan_writeToReplayFrame(void* frame)
{
    int i;
    for (i = 0; i < 6; i++) {
        tRenderState* stateEntry = &cRenderStateMan_m_state[i];
        ((cRSMObjWF*)stateEntry->m_query[8]->m_data)->v64(frame);
        ((cRSMObjWF*)stateEntry->m_query[8]->m_data)->v64(frame);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_readFromReplayFrame);
#ifdef SKIP_ASM
class cRSMObjRF {
public:
    int field_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65(void* frame);
};

/**
 * Restores the render states from a replay frame.
 *
 * For each of the 6 render state entries (cRenderStateMan_m_state, 0xF0 bytes each), calls virtual slot 65 of the object at
 * *p20 (+0x20) twice with the frame.
 *
 * @param frame  the replay frame stream to read from
 */
extern "C" void cRenderStateMan_readFromReplayFrame(void* frame)
{
    int i;
    for (i = 0; i < 6; i++) {
        tRenderState* entry = &cRenderStateMan_m_state[i];
        ((cRSMObjRF*)entry->m_query[8]->m_data)->v65(frame);
        ((cRSMObjRF*)entry->m_query[8]->m_data)->v65(frame);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_setInNIS);
#ifdef SKIP_ASM
extern void* cRenderStateMan_m_inNIS;
extern int cPlants_m_Enable;

/**
 * Tells the render-state manager whether an in-game cut scene (NIS) is playing.
 *
 * Stores @p innis in cRenderStateMan_m_inNIS and sets cPlants_m_Enable when it is non-zero.
 *
 * @param innis  nonzero while a cut scene plays (On Tour innis)
 */
extern "C" void cRenderStateMan_setInNIS(void* innis)
{
    cRenderStateMan_m_inNIS = innis;
    cPlants_m_Enable = innis != 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_setCustomIrradiance);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern int cRenderStateMan_m_hasCustomIrradiance;
extern int cRenderStateMan_m_customIrradiance;

/**
 * Selects a named custom irradiance for cut scenes, or turns it off.
 *
 * A null @p name clears the flag cRenderStateMan_m_hasCustomIrradiance; otherwise the name is copied to cRenderStateMan_m_customIrradiance and the flag is set.
 *
 * @param name  irradiance name (under 8 characters), or 0 for none (On Tour pName)
 */
extern "C" void cRenderStateMan_setCustomIrradiance(const char* name)
{
    if (name == 0) {
        cRenderStateMan_m_hasCustomIrradiance = 0;
    } else {
        strcpy((char*)&cRenderStateMan_m_customIrradiance, name);
        cRenderStateMan_m_hasCustomIrradiance = 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_cAirStreamerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/renderstateman_classes.h"

/**
 * Constructs an air streamer, the ribbon trail of an airborne rider; cRider is its virtual base.
 *
 * Builds the cRider base only when it is the most-derived object (a hidden second argument), sets the cRider
 * part's vtables, allocates the 25-entry ring of 0x20-byte trail segments (tag "cAirStreamerFX") at +0x4 and
 * resets the trail with readFromReplayFrame().
 */
cRsm_2EF3B0::cRsm_2EF3B0()
{
    cRsmItem_2EF3B0** slot = &items;
    *slot = new (str_cAirStreamerFX, 0, 0) cRsmItem_2EF3B0[25];
    cAirStreamerFX_readFromReplayFrame_F3B0(this);
}

// PORT: the compiler names this ctor __11cRsm_2EF3B0i; expose it under the address-derived name.
__asm__(".globl cAirStreamerFX_cAirStreamerFX\n.type cAirStreamerFX_cAirStreamerFX,@function\ncAirStreamerFX_cAirStreamerFX = __11cRsm_2EF3B0i\n.size cAirStreamerFX_cAirStreamerFX,0x180\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("__12cRiderVb_rsm = cRider_cRider\n"
        "_vt$11cRsm_2EF3B0$13cRiderVbC_rsm = D_004888D0\n"
        "_vt$11cRsm_2EF3B0$14cRiderVbBC_rsm = D_00459B90\n"
        "_vt$11cRsm_2EF3B0$12cRiderVb_rsm = D_00488918\n");
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX__cAirStreamerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/renderstateman_classes.h"

/**
 * Destroys an air streamer: frees its segment ring, then tears down per the g++ destructor flags.
 *
 * Restores this class's vtables in the virtual cRider base and frees the ring (+0x4) if set; bit 1 of the
 * hidden flags then destroys the cRider base (cRider__cRider) and bit 0 frees the object.
 */
cVbDtor002EF530::~cVbDtor002EF530()
{
    if (m04 != 0) cMemMan_free(m04);
}

// PORT: the compiler names this dtor _$_15cVbDtor002EF530; expose it under the address-derived name.
__asm__(".globl cAirStreamerFX__cAirStreamerFX\n.type cAirStreamerFX__cAirStreamerFX,@function\ncAirStreamerFX__cAirStreamerFX = _$_15cVbDtor002EF530\n.size cAirStreamerFX__cAirStreamerFX,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002EF530$13cRiderVbC_rsm = D_004888D0\n"
        "_vt$15cVbDtor002EF530$14cRiderVbBC_rsm = D_00459B90\n"
        "_vt$15cVbDtor002EF530$12cRiderVb_rsm = D_00488918\n"
        "_$_12cRiderVb_rsm = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_reset);
#ifdef SKIP_ASM
/**
 * Resets the air streamer: empties the trail and picks its maximum length.
 *
 * Clears the head, count and texture scroll (+0x8, +0xC, +0x10) and sets the length (+0x14) to 25 segments
 * for a rider with a view (mViewIndex >= 0), otherwise 18. Despite the name it reads nothing from a replay
 * frame.
 */
extern "C" void cAirStreamerFX_reset(cAirStreamerFX* self)
{
    int mode = (*(int*)(*(char**)self + 0x870) >= 0) ? 25 : 18;
    *(int*)&self->mVOffset = 0;
    self->StreamBufHead = 0;
    self->StreamBufLen = 0;
    self->mMaxStreamerSegments = mode;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_update);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);
extern "C" int AITick();




// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_EF6D0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_EF6D0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_EF6D0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_EF6D0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

static inline int Approach_EF6D0(int n, int step)
{
    return n > step ? n - step : (n > -step - 1 ? 0 : n + step);
}

/**
 * Grows the air streamer's trail while the rider is airborne, shrinks it otherwise, and scrolls its texture.
 *
 * In the air (MS_Air) the head segment gets two points 90 units either side of the board bone
 * (mBoneBoardChild) along its axis, minus 0.7 x the hips position; on even AI ticks the head moves on around
 * the 25-entry ring and the count grows up to the length. Otherwise the count drops by 2 per frame. The
 * texture offset moves by the rider's speed / 6000 and wraps into 0..1.
 */
extern "C" void cAirStreamerFX_update(cAirStreamerFX* self)
{
    if (cRider_getMotionState(((char*)self->mRider)) == 1) {
        char* riderPtr = ((char*)self->mRider);
        t4Vector bodyOff = *(t4Vector*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x89C) << 6) + 0x30);
        t4Vector boardOff = *(t4Vector*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x8A4) << 6) + 0x30);
        t4Vector boardDir = *(t4Vector*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x34) + (*(int*)(riderPtr + 0x8A4) << 6));
        t4Vector streamBase = Sub_EF6D0(boardOff, Scale_EF6D0(bodyOff, 0.699999988079071f));
        (*(t4Vector*)&self->StreamBuf[self->StreamBufHead].l) = Add_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        (*(t4Vector*)&self->StreamBuf[self->StreamBufHead].r) = Sub_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        if ((AITick() & 1) == 0) {
            int old = self->StreamBufHead;
            self->StreamBufHead = old + 1;
            self->StreamBufHead %= 25;
            if (self->StreamBufLen < self->mMaxStreamerSegments)
                self->StreamBufLen++;
            self->StreamBuf[self->StreamBufHead] = self->StreamBuf[old];
        }
    } else {
        int n = self->StreamBufLen;
        self->StreamBufLen = Approach_EF6D0(n, 2);
    }
    self->mVOffset -= Length_EF6D0(self->mRider->mVelocity) * 0.00016666666488163173f;
    if (self->mVOffset < 0.0f)
        self->mVOffset += 1.0f;
    if (self->mVOffset > 1.0f)
        self->mVOffset -= 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_render);
#ifdef SKIP_ASM

struct sRsColor_EF950 {
    float x, y, z, w;
    sRsColor_EF950() {}
    sRsColor_EF950(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
};

struct sRsVertUV_EF950 {
    float x, y, z, w;
    sRsVertUV_EF950() {}
} __attribute__((aligned(16)));


struct sRsState_EF950 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sRsVEnt_EF950 {
    short delta;
    short index;
    void (*fn)(void*, int, tBxVertex*, int);
};

struct sRsCtx_EF950 {
    char pad0[0xE84];
    sRsState_EF950* top;        // 0xE84
    char pad1[0x1044 - 0xE88];
    int tex1044;                // 0x1044
    int pad1048;
    int tex104C;                // 0x104C
    char pad2[0x10D8 - 0x1050];
    sRsVEnt_EF950* vtable;      // 0x10D8
};



extern sRsCtx_EF950* GrMan_renderstateman __asm__("GrMan");
extern sRsState_EF950 sDrawState_s_defaultDrawState[];
extern int cPowerUpFXToggles_m_TestTrickBoost;
extern float D_004A3B74;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector rsScale_EF950(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector rsAdd_EF950(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector rsSub_EF950(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline int rsTex1044_EF950(sRsCtx_EF950* ctx)
{
    return ctx->tex1044;
}

static inline int rsTex104C_EF950(sRsCtx_EF950* ctx)
{
    return ctx->tex104C;
}

static inline void rsPush_EF950(sRsCtx_EF950* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void rsPop_EF950(sRsCtx_EF950* ctx)
{
    ctx->top--;
}

static inline void rsSetA_EF950(sRsCtx_EF950* ctx, int mask, int shift, int v)
{
    ctx->top->flagsA = (ctx->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_EF950(sRsCtx_EF950* ctx, int mask, int shift, int v)
{
    ctx->top->flagsB = (ctx->top->flagsB & ~mask) | ((v << shift) & mask);
}

static inline t4Vector rsRow_EF950(char* base, int idx, int off)
{
    return *(t4Vector*)(base + (idx << 6) + off);
}

static inline void rsVert_EF950(tBxVertex* v, float u, float w, const sRsColor_EF950& c, const t4Vector& p)
{
    v->r = (int)(c.y * 255.0f);
    v->g = (int)(c.z * 255.0f);
    v->b = (int)(c.w * 255.0f);
    v->a = (int)(c.x * 128.0f);
    (*(sRsVertUV_EF950*)&v->u).z = 1.0f;
    (*(&v->p)) = p;
    (*(sRsVertUV_EF950*)&v->u).x = u;
    (*(sRsVertUV_EF950*)&v->u).y = w;
}

/**
 * Draws the air streamer as two textured triangle strips that fade toward the tail.
 *
 * Needs at least two segments. Normally 15 units wide at alpha 0.3 with GrMan _EffectTexMap[63]; while a
 * trick boost runs (mPowerUpTrickTimer > 0 or the TestTrickBoost toggle) it glows: 37.5 wide, alpha 1, tinted
 * D_004A3B74, _EffectTexMap[61] scrolled by the trail offset. Segment alpha falls off as 1 - t^2/(len-1)^2,
 * and the segments are placed back at 0.7 x the current hips position.
 */
extern "C" void cAirStreamerFX_render(cAirStreamerFX* self)
{
    if (self->StreamBufLen < 2) {
        return;
    }
    rsPush_EF950(GrMan_renderstateman);
    *GrMan_renderstateman->top = sDrawState_s_defaultDrawState[0];
    int glow = 0;
    float alpha = 0.3f;
    float start = 0.0f;
    if (cPowerUpFXToggles_m_TestTrickBoost != 0 || 0.0f < self->mRider->mPowerUpTrickTimer) {
        glow = 1;
        alpha = 1.0f;
        *(short*)((char*)GrMan_renderstateman->top + 0x10) = rsTex1044_EF950(GrMan_renderstateman);
        GrMan_renderstateman->top->f0 &= ~0xC;
        start = self->mVOffset;
    } else {
        sRsCtx_EF950* renderCtx = GrMan_renderstateman;
        renderCtx->top->f0 |= 0xC;
        *(short*)((char*)renderCtx->top + 0x10) = rsTex104C_EF950(renderCtx);
    }
    sRsCtx_EF950* ctx = GrMan_renderstateman;
    rsSetA_EF950(ctx, 0x400000, 22, 1);
    rsSetA_EF950(ctx, 0x1800000, 23, 0);
    rsSetA_EF950(ctx, 0x300000, 20, 0);
    rsSetA_EF950(ctx, 0xFF000, 12, 20);
    rsSetA_EF950(ctx, 0x3, 0, 0);
    rsSetB_EF950(ctx, 0x1FFFFC00, 10, 0);
    rsSetB_EF950(ctx, 0x3E0, 5, 8);
    rsSetA_EF950(ctx, 0x7C, 2, 7);
    tBxVertex vertsL[52];
    tBxVertex vertsR[52];
    cRider* rider = self->mRider;
    float uvy = start;
    float invSq = 1.0f / (float)((self->mMaxStreamerSegments - 1) * (self->mMaxStreamerSegments - 1));
    t4Vector off = rsScale_EF950(rsRow_EF950((char*)rider->mAnimModel->SkelMatrices, rider->mBoneHips, 0x30), 0.7f);
    float dv = 1.0f / (float)self->StreamBufLen;
    float falloffT = 0.0f;
    int nverts = 0;
    sRsColor_EF950 color;
    color.x = 1.0f;
    color.y = 1.0f;
    color.z = 1.0f;
    color.w = 1.0f;
    t4Vector leftPos;
    t4Vector rightPos;
    rider = (cRider*)self->mRider;
    t4Vector col = rsRow_EF950((char*)rider->mAnimModel->SkelMatricesScaled, rider->mBoneBoardChild, 0x20);
    float size = 15.0f;
    if (glow) {
        float k = D_004A3B74;
        color = sRsColor_EF950(1.0f, k, k, k);
        size = 37.5f;
    }
    t4Vector half = rsScale_EF950(col, size * 0.5f);
    int j = self->StreamBufHead;
    int k = 0;
    for (int i = 0; i < self->StreamBufLen; i++) {
        if (j < 0) {
            j = 24;
        }
        color.x = alpha * (1.0f - falloffT * falloffT * invSq);
        if (color.x < 0.0f) {
            color.x = 0.0f;
        }
        leftPos = rsAdd_EF950((*(&(self->StreamBuf)[j].l)), off);
        rightPos = rsAdd_EF950((*(&(self->StreamBuf)[j].r)), off);
        rsVert_EF950(&vertsL[k], 0.0f, uvy, color, rsSub_EF950(leftPos, half));
        rsVert_EF950(&vertsR[k], 0.0f, uvy, color, rsSub_EF950(rightPos, half));
        k++;
        rsVert_EF950(&vertsL[k], 1.0f, uvy, color, rsAdd_EF950(leftPos, half));
        rsVert_EF950(&vertsR[k], 1.0f, uvy, color, rsAdd_EF950(rightPos, half));
        k++;
        uvy += dv;
        nverts += 2;
        j--;
        falloffT += 1.0f;
    }
    sRsCtx_EF950* grMan = GrMan_renderstateman;
    grMan->vtable[71].fn(vthis(grMan, grMan->vtable[71]), nverts, vertsL, 0);
    grMan = GrMan_renderstateman;
    grMan->vtable[71].fn(vthis(grMan, grMan->vtable[71]), nverts, vertsR, 0);
    rsPop_EF950(GrMan_renderstateman);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", visualEffectsMan);
#ifdef SKIP_ASM
extern void* gVisualEffectsMan;

/**
 * Returns the visual effects manager (gVisualEffectsMan), which cVisualEffectsMan_cVisualEffectsMan records.
 *
 * Nothing in the retail code references it.
 *
 * @return the manager
 */
extern "C" int visualEffectsMan(void) {
    return (*(int*)&gVisualEffectsMan);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cVisualEffectsMan_cVisualEffectsMan);
#ifdef SKIP_ASM
struct sRSEntry_002EFF98 {
    int f0;
    int f4;
    int f8;
    int pad[5];
};
struct sRSMan_002EFF98 {
    int f[17];
    void* vt;
};
extern char cVisualEffectsMan_vtbl[];
extern sRSEntry_002EFF98* cGraphicsMan_TheGraphicsMan_renderstateman __asm__("cGraphicsMan_TheGraphicsMan");
struct sShadowCfg_002EFF98 {
    int texIndex;
};
extern sShadowCfg_002EFF98 D_004A45D4;
extern int D_004A443C;
extern void* gVisualEffectsMan;

/**
 * Constructs the visual-effects manager base: empty effect slots, default effect textures, global pointer.
 *
 * Sets the vtable (+0x44), clears the 17 effect-object slots, stores 0x37 in D_004A45D4 and 0x30 in
 * D_004A443C and makes itself the manager gVisualEffectsMan. On the graphics manager it sets view 6's specular,
 * patch specular, snow sparkle and rim texture ids (0x2E, 0x31, 0x44, 0x2F) and zeroes views 6 and 7 of the
 * per-view arrays m_isInNIS through m_ScreenTintOffsetB.
 *
 * @return self
 */
extern "C" void* cVisualEffectsMan_cVisualEffectsMan(sRSMan_002EFF98* self)
{
    self->vt = cVisualEffectsMan_vtbl;
    self->f[0] = 0;
    self->f[1] = 0;
    self->f[2] = 0;
    self->f[3] = 0;
    self->f[4] = 0;
    self->f[5] = 0;
    self->f[6] = 0;
    self->f[7] = 0;
    self->f[8] = 0;
    self->f[9] = 0;
    self->f[10] = 0;
    self->f[11] = 0;
    self->f[12] = 0;
    self->f[13] = 0;
    self->f[14] = 0;
    self->f[15] = 0;
    self->f[16] = 0;
    D_004A45D4.texIndex = 55;
    D_004A443C = 0x30;
    gVisualEffectsMan = self;
    sRSEntry_002EFF98* rsEntries = cGraphicsMan_TheGraphicsMan_renderstateman;
    rsEntries[1].f4 = 46;
    rsEntries[2].f4 = 49;
    rsEntries[4].f4 = 47;
    rsEntries[3].f4 = 68;
    rsEntries[5].f4 = 0; rsEntries[5].f8 = 0;
    rsEntries[6].f4 = 0; rsEntries[6].f8 = 0;
    rsEntries[7].f4 = 0; rsEntries[7].f8 = 0;
    rsEntries[8].f4 = 0; rsEntries[8].f8 = 0;
    rsEntries[9].f4 = 0; rsEntries[9].f8 = 0;
    rsEntries[10].f4 = 0; rsEntries[10].f8 = 0;
    rsEntries[11].f4 = 0; rsEntries[11].f8 = 0;
    rsEntries[12].f4 = 0; rsEntries[12].f8 = 0;
    rsEntries[13].f4 = 0; rsEntries[13].f8 = 0;
    rsEntries[14].f4 = 0; rsEntries[14].f8 = 0;
    rsEntries[15].f4 = 0; rsEntries[15].f8 = 0;
    rsEntries[16].f4 = 0; rsEntries[16].f8 = 0;
    rsEntries[17].f4 = 0; rsEntries[17].f8 = 0;
    rsEntries[18].f4 = 0; rsEntries[18].f8 = 0;
    rsEntries[19].f4 = 0; rsEntries[19].f8 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cVisualEffectsMan_commonUpdate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" float cRenderStateMan_DrawDistance(int i);
extern "C" float cRenderStateMan_LightningChance(int i);
extern "C" float cRenderStateMan_FogDensity(int i);
extern "C" float cRenderStateMan_FogNear(int i);
extern "C" float cRenderStateMan_FogFar(int i);
extern "C" float cRenderStateMan_FogColourR(int i);
extern "C" float cRenderStateMan_FogColourG(int i);
extern "C" float cRenderStateMan_FogColourB(int i);
extern "C" float cRenderStateMan_ScreenTintScaleR(int i);
extern "C" float cRenderStateMan_ScreenTintScaleG(int i);
extern "C" float cRenderStateMan_ScreenTintScaleB(int i);
extern "C" float cRenderStateMan_ScreenTintOffsetR(int i);
extern "C" float cRenderStateMan_ScreenTintOffsetG(int i);
extern "C" float cRenderStateMan_ScreenTintOffsetB(int i);
extern "C" float cRenderStateMan_LightGlowCutoff(int i);
extern "C" float cRenderStateMan_LightGlowCutoffScale(int i);
extern "C" float cRenderStateMan_LightGlowCopyIntensity(int i);
extern "C" float cRenderStateMan_LightGlowSourceIntensity(int i);
extern "C" float cRenderStateMan_LightGlowFrameIntensity(int i);
extern "C" float cRenderStateMan_LightGlowBlend2(int i);
extern "C" float cRenderStateMan_LightGlowBlend3(int i);
extern "C" float cRenderStateMan_SnowSparkleDensity(int i);
extern "C" int func_002F7BE0(void* p);
extern void* cRenderStateMan_m_inNIS;
extern int cDepthFog_m_OverrideWP;
extern int cDepthFog_m_Exponent;
extern float cDepthFog_m_Density;
extern float cDepthFog_m_NearPlane;
extern float cDepthFog_m_FarPlane;
extern int cScreenTint_m_OverrideWP;
extern float cScreenTint_m_ColourR;
extern float cScreenTint_m_ColourG;
extern float cScreenTint_m_ColourB;
extern float cScreenTint_m_FillR;
extern float cScreenTint_m_FillG;
extern float cScreenTint_m_FillB;
extern float cScreenTint_m_LightningChance;
extern int cScreenTint_m_LightningEnable;
extern int cLightGlow_m_OverrideWP;
extern float cLightGlow_m_PS2MinIntensity;
extern float cLightGlow_m_PS2ModIntensity;
extern float cLightGlow_m_PS2CopyIntensity;
extern float cLightGlow_m_PS2FrameSrcScale;
extern float cLightGlow_m_PS2FrameDstScale;
extern float cLightGlow_m_PS2IntensityTex2;
extern float cLightGlow_m_PS2IntensityTex3;

// A colour in On Tour's tARGB order: alpha first, then red, green, blue.
struct sRsCol_F00A0 {
    float A, R, G, B;
    sRsCol_F00A0(const float& ia, const float& ir, const float& ig, const float& ib) : A(ia), R(ir), G(ig), B(ib) {}
};
extern sRsCol_F00A0 cDepthFog_m_Colour;

struct sRsCtx_F00A0 {
    char pad0[0x8C];
    void* m_isInNIS[8];       // 0x8C
    float m_SnowSparkleDensity[8];       // 0xAC
    float m_DrawDistance[8];       // 0xCC
    float m_FogDensity[8];       // 0xEC
    float m_FogColourR[8];      // 0x10C
    float m_FogColourG[8];      // 0x12C
    float m_FogColourB[8];      // 0x14C
    float m_FogNear[8];      // 0x16C
    float m_FogFar[8];      // 0x18C
    float m_ScreenTintScaleR[8];      // 0x1AC
    float m_ScreenTintScaleG[8];      // 0x1CC
    float m_ScreenTintScaleB[8];      // 0x1EC
    float m_ScreenTintOffsetR[8];      // 0x20C
    float m_ScreenTintOffsetG[8];      // 0x22C
    float m_ScreenTintOffsetB[8];      // 0x24C
};
extern sRsCtx_F00A0* D_ctx_F00A0 __asm__("cGraphicsMan_TheGraphicsMan");

/**
 * Pulls one camera's visual-effect settings from the render-state manager into the effect systems.
 *
 * Unless cDepthFog_m_OverrideWP is set, the fog density, near and far planes and colour come from the matching queries;
 * unless cScreenTint_m_OverrideWP is set, the tint scale and fill colours and the lightning chance do too (lightning is
 * enabled when the app's +0x84 state is 1 or func_002F7BE0 says so); unless cLightGlow_m_OverrideWP is set, the
 * light-glow intensities and scales do. The in-NIS flag and the remaining per-camera values are stored per @p i in the
 * graphics context.
 *
 * @param i  which camera's render state to use (On Tour cameraId)
 */
extern "C" void cVisualEffectsMan_commonUpdate(cVisualEffectsMan* self, int i)
{
    sRsCtx_F00A0* ctx = D_ctx_F00A0;
    if (cDepthFog_m_OverrideWP == 0) {
        sRsCol_F00A0* dst = &cDepthFog_m_Colour;
        cDepthFog_m_Density = cRenderStateMan_FogDensity(i);
        cDepthFog_m_Exponent = 1;
        cDepthFog_m_NearPlane = cRenderStateMan_FogNear(i);
        cDepthFog_m_FarPlane = cRenderStateMan_FogFar(i);
        float fogRed = cRenderStateMan_FogColourR(i);
        float fogGreen = cRenderStateMan_FogColourG(i);
        float fogBlue = cRenderStateMan_FogColourB(i);
        *dst = sRsCol_F00A0(1.0f, fogRed, fogGreen, fogBlue);
    }
    if (cScreenTint_m_OverrideWP == 0) {
        cScreenTint_m_ColourR = cRenderStateMan_ScreenTintScaleR(i);
        cScreenTint_m_ColourG = cRenderStateMan_ScreenTintScaleG(i);
        cScreenTint_m_ColourB = cRenderStateMan_ScreenTintScaleB(i);
        cScreenTint_m_FillR = cRenderStateMan_ScreenTintOffsetR(i);
        cScreenTint_m_FillG = cRenderStateMan_ScreenTintOffsetG(i);
        cScreenTint_m_FillB = cRenderStateMan_ScreenTintOffsetB(i);
        cScreenTint_m_LightningChance = cRenderStateMan_LightningChance(i);
        int on = 0;
        int* st = *(int**)(*(char**)((char*)SSXApp + 0x84) + 0x28);
        if (*st == 1 || func_002F7BE0(st))
            on = 1;
        cScreenTint_m_LightningEnable = on;
    }
    if (cLightGlow_m_OverrideWP == 0) {
        cLightGlow_m_PS2MinIntensity = cRenderStateMan_LightGlowCutoff(i);
        cLightGlow_m_PS2ModIntensity = cRenderStateMan_LightGlowCutoffScale(i);
        cLightGlow_m_PS2CopyIntensity = cRenderStateMan_LightGlowCopyIntensity(i);
        cLightGlow_m_PS2FrameSrcScale = cRenderStateMan_LightGlowSourceIntensity(i);
        cLightGlow_m_PS2FrameDstScale = cRenderStateMan_LightGlowFrameIntensity(i);
        cLightGlow_m_PS2IntensityTex2 = cRenderStateMan_LightGlowBlend2(i);
        cLightGlow_m_PS2IntensityTex3 = cRenderStateMan_LightGlowBlend3(i);
    }
    ctx->m_isInNIS[i] = cRenderStateMan_m_inNIS;
    ctx->m_SnowSparkleDensity[i] = cRenderStateMan_SnowSparkleDensity(i);
    ctx->m_DrawDistance[i] = cRenderStateMan_DrawDistance(i);
    ctx->m_FogDensity[i] = cRenderStateMan_FogDensity(i);
    ctx->m_FogColourR[i] = cRenderStateMan_FogColourR(i);
    ctx->m_FogColourG[i] = cRenderStateMan_FogColourG(i);
    ctx->m_FogColourB[i] = cRenderStateMan_FogColourB(i);
    ctx->m_FogNear[i] = cRenderStateMan_FogNear(i);
    ctx->m_FogFar[i] = cRenderStateMan_FogFar(i);
    ctx->m_ScreenTintScaleR[i] = cRenderStateMan_ScreenTintScaleR(i);
    ctx->m_ScreenTintScaleG[i] = cRenderStateMan_ScreenTintScaleG(i);
    ctx->m_ScreenTintScaleB[i] = cRenderStateMan_ScreenTintScaleB(i);
    ctx->m_ScreenTintOffsetR[i] = cRenderStateMan_ScreenTintOffsetR(i);
    ctx->m_ScreenTintOffsetG[i] = cRenderStateMan_ScreenTintOffsetG(i);
    ctx->m_ScreenTintOffsetB[i] = cRenderStateMan_ScreenTintOffsetB(i);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", aivisualEffectsMan);
#ifdef SKIP_ASM
extern void* gAIVisualEffectsMan;

/**
 * Returns the AI visual effects manager (gAIVisualEffectsMan), which cAIVisualEffectsMan_cAIVisualEffectsMan records.
 *
 * Nothing in the retail code references it.
 *
 * @return the manager
 */
extern "C" int aivisualEffectsMan(void) {
    return (*(int*)&gAIVisualEffectsMan);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAIVisualEffectsMan_cAIVisualEffectsMan);
#ifdef SKIP_ASM
extern void* cAIVisualEffectsMan_vtbl[];
extern void* gAIVisualEffectsMan;

/**
 * Constructs the base AI visual effects manager and makes it the current one.
 *
 * Clears +0x0, sets the vtable cAIVisualEffectsMan_vtbl at +0x4 and stores this in gAIVisualEffectsMan.
 *
 * @return this
 */
extern "C" void* cAIVisualEffectsMan_cAIVisualEffectsMan(cAIVisualEffectsMan* self)
{
    *(int*)self = 0;
    self->mVtbl = cAIVisualEffectsMan_vtbl;
    gAIVisualEffectsMan = ((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", func_002F0388);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * It sits between cAIVisualEffectsMan_cAIVisualEffectsMan and cAIVisualEffectsMan_construct; nothing in the retail code
 * references it.
 */
extern "C" void func_002F0388(void) {

}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAIVisualEffectsMan_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan(cPS2AIVisualEffectsMan* self);
extern char str_PS2AIVisualEffectsMan[];

/**
 * Creates the platform's AI visual effects manager.
 *
 * Allocates 8 bytes named "PS2AIVisualEffectsMan" and runs cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan on them.
 *
 * @return the new manager
 */
extern "C" void* cAIVisualEffectsMan_construct(void)
{
    return cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan((cPS2AIVisualEffectsMan*)cMemMan_alloc(8, str_PS2AIVisualEffectsMan, 0, 0));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/renderstateman", cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cAIVisualEffectsMan_cAIVisualEffectsMan(cAIVisualEffectsMan* self);
extern "C" void cAvalanche_init(void* self);
extern char str_Avalanche[];
extern void* cPS2AIVisualEffectsMan_vtbl[];
extern void* cAvalanche_vtbl[];

/**
 * Constructs the PS2 AI visual effects manager with its avalanche effect.
 *
 * Runs cAIVisualEffectsMan_cAIVisualEffectsMan, sets the vtable cPS2AIVisualEffectsMan_vtbl at +0x4, allocates the avalanche object ("Avalanche", 4 bytes), gives
 * it cAvalanche_vtbl, runs cAvalanche_init on it and keeps it at +0x0.
 *
 * @return this
 */
extern "C" void* cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan(cPS2AIVisualEffectsMan* self)
{
    cAIVisualEffectsMan_cAIVisualEffectsMan((cAIVisualEffectsMan*)self);
    self->mVtbl = cPS2AIVisualEffectsMan_vtbl;
    void** avalanche = (void**)cMemMan_alloc(4, str_Avalanche, 0, 0);
    *avalanche = cAvalanche_vtbl;
    cAvalanche_init(avalanche);
    *(void***)self = avalanche;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cPS2AIVisualEffectsMan_vtbl[];
extern void* cAIVisualEffectsMan_vtbl[];

class cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(int);
};

/**
 * Destroys the PS2 AI visual effects manager and its avalanche effect.
 *
 * Restores cPS2AIVisualEffectsMan_vtbl, destroys the object at +0x0 (if any) through its virtual slot 1 with flag 3, switches to the
 * base vtable cAIVisualEffectsMan_vtbl and calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan(cPS2AIVisualEffectsMan* self, int flags)
{
    self->mVtbl = cPS2AIVisualEffectsMan_vtbl;
    cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan_cVirt* avalanche = *(cPS2AIVisualEffectsMan__cPS2AIVisualEffectsMan_cVirt**)self;
    if (avalanche != 0) {
        avalanche->v01(3);
    }
    self->mVtbl = cAIVisualEffectsMan_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cPS2AIVisualEffectsMan_update);
#ifdef SKIP_ASM
class cPS2AIVisualEffectsMan_update_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
};

/**
 * Updates the AI-driven visual effects (the avalanche) for this frame.
 *
 * Calls virtual slot 2 of the object at +0x0.
 */
extern "C" void cPS2AIVisualEffectsMan_update(cPS2AIVisualEffectsMan_update_cObj** self)
{
    (*self)->v02();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cPS2AIVisualEffectsMan_render);
#ifdef SKIP_ASM
/**
 * Draws the AI-driven visual effects (the avalanche).
 *
 * Calls virtual slot 3 of the object at +0x0.
 */
extern "C" void cPS2AIVisualEffectsMan_render(cPS2AIVisualEffectsMan_update_cObj** self)
{
    (*self)->v03();
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cVisualEffectsMan_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2VisualEffectsMan_cPS2VisualEffectsMan(void* self);
extern char str_PS2VisualEffectsMan[];

/**
 * Creates the visual-effects manager: allocates 0x48 bytes tagged "PS2VisualEffectsMan" and constructs a
 * cPS2VisualEffectsMan there.
 *
 * The base constructor records it in gVisualEffectsMan; this function stores it nowhere itself.
 *
 * @return the new manager
 */
extern "C" void* cVisualEffectsMan_construct(void)
{
    return cPS2VisualEffectsMan_cPS2VisualEffectsMan(cMemMan_alloc(72, str_PS2VisualEffectsMan, 0, 0));
}
#endif

