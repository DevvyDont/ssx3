#include "common.h"
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

struct sRSMEntry {
    char pad0[0x4];
    sRSMObj** p04;
    sRSMObj** p08;
    char pad0C[0x1C - 0xC];
    sRSMObj** p1C;
    sRSMObj** p20;
    char pad24[0xF0 - 0x24];
};

extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* entry = &cRenderStateMan_m_state[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[55].fn((char*)stateObj + vt[55].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourG);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* entry = &cRenderStateMan_m_state[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[56].fn((char*)stateObj + vt[56].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFlakeColourB);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* entry = &cRenderStateMan_m_state[i];
    sRSMObj* stateObj = *entry->p20;
    sRSMVEntry* vt = stateObj->vt;
    return *vt[57].fn((char*)stateObj + vt[57].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourA);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p20;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[58].fn((char*)stateQuery + vt[58].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourR);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p20;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[59].fn((char*)stateQuery + vt[59].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourG);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p20;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[60].fn((char*)stateQuery + vt[60].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowFluffColourB);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p20;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[61].fn((char*)stateQuery + vt[61].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogDensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[7].fn((char*)stateQuery + vt[7].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogNear);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[8].fn((char*)stateQuery + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogFar);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[9].fn((char*)stateQuery + vt[9].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourR);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[10].fn((char*)stateQuery + vt[10].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourG);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[11].fn((char*)stateQuery + vt[11].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_FogColourB);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p04;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[12].fn((char*)stateQuery + vt[12].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleR);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint scale's red for render state @p i (1 while D_004A3B60 is set).
 *
 * Calls virtual slot 20 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourR and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleR(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[20].fn((char*)stateQuery + vt[20].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleG);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint scale's green for render state @p i (1 while D_004A3B60 is set).
 *
 * Calls virtual slot 21 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourG and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleG(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[21].fn((char*)stateQuery + vt[21].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintScaleB);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint scale's blue for render state @p i (1 while D_004A3B60 is set).
 *
 * Calls virtual slot 22 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 1 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_ColourB and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintScaleB(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[22].fn((char*)stateQuery + vt[22].delta);
    }
    return 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetR);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint offset's red for render state @p i (0 while D_004A3B60 is set).
 *
 * Calls virtual slot 23 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillR and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetR(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[23].fn((char*)stateQuery + vt[23].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetG);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint offset's green for render state @p i (0 while D_004A3B60 is set).
 *
 * Calls virtual slot 24 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillG and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetG(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[24].fn((char*)stateQuery + vt[24].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ScreenTintOffsetB);
#ifdef SKIP_ASM
extern int D_004A3B60;

/**
 * Returns the screen tint offset's blue for render state @p i (0 while D_004A3B60 is set).
 *
 * Calls virtual slot 25 of the state's world-painter query object m_query[3] (+0xC) (cRenderStateMan_m_state[@p i],
 * 0xF0-byte rows) and returns the value it points to. It returns 0 instead while the flag D_004A3B60 is set
 * (world-script events set it with bit 4 and clear it with bit 8). cVisualEffectsMan_commonUpdate stores it in
 * cScreenTint_m_FillB and in its per-state table.
 *
 * @param i  the render state (view) index
 * @return the value
 */
extern "C" float cRenderStateMan_ScreenTintOffsetB(int i)
{
    if (D_004A3B60 == 0) {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = **(sRSMObj***)((char*)renderState + 0xC);
        sRSMVEntry* vt = stateQuery->vt;
        return *vt[25].fn((char*)stateQuery + vt[25].delta);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCutoff);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[13].fn((char*)stateQuery + vt[13].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCutoffScale);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[14].fn((char*)stateQuery + vt[14].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowCopyIntensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[15].fn((char*)stateQuery + vt[15].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowSourceIntensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[16].fn((char*)stateQuery + vt[16].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowFrameIntensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[17].fn((char*)stateQuery + vt[17].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowBlend2);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[18].fn((char*)stateQuery + vt[18].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightGlowBlend3);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p08;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[19].fn((char*)stateQuery + vt[19].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_RimLightIntensityOffset);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p1C;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[42].fn((char*)stateQuery + vt[42].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_AmbientLightIntensityOffset);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p1C;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[41].fn((char*)stateQuery + vt[41].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_SnowSparkleDensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* entry = &cRenderStateMan_m_state[i];
    sRSMObj* object = **(sRSMObj***)((char*)entry + 0x18);
    sRSMVEntry* vt = object->vt;
    return *vt[36].fn((char*)object + vt[36].delta) * (1.0f - *(float*)((char*)entry + 0x24));
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_PlayerWindIntensity);
#ifdef SKIP_ASM
extern sRSMEntry cRenderStateMan_m_state[];

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
    sRSMEntry* renderState = &cRenderStateMan_m_state[i];
    sRSMObj* stateQuery = *renderState->p20;
    sRSMVEntry* vt = stateQuery->vt;
    return *vt[47].fn((char*)stateQuery + vt[47].delta);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_LightIrradianceName);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

/**
 * Returns the light irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While D_004A3B64 is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field D_004A55B0; otherwise
 * it calls virtual slot 37 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_LightIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_LightIrradianceName(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = *renderState->p1C;
        sRSMVEntry* vt = stateQuery->vt;
        return vt[37].fn((char*)stateQuery + vt[37].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ShadowIrradianceName);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

/**
 * Returns the shadow irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While D_004A3B64 is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field D_004A55B0; otherwise
 * it calls virtual slot 38 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_ShadowIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_ShadowIrradianceName(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = *renderState->p1C;
        sRSMVEntry* vt = stateQuery->vt;
        return vt[38].fn((char*)stateQuery + vt[38].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_TunnelIrradianceName);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

/**
 * Returns the tunnel irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While D_004A3B64 is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field D_004A55B0; otherwise
 * it calls virtual slot 39 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_TunnelIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_TunnelIrradianceName(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = *renderState->p1C;
        sRSMVEntry* vt = stateQuery->vt;
        return vt[39].fn((char*)stateQuery + vt[39].delta);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cRenderStateMan_ObjectIrradianceName);
#ifdef SKIP_ASM
extern int D_004A3B64;
extern int D_004A55B0;

/**
 * Returns the object irradiance name field for render state @p i, or the custom one while a custom irradiance is set.
 *
 * While D_004A3B64 is set (cRenderStateMan_setCustomIrradiance) it returns the custom name field D_004A55B0; otherwise
 * it calls virtual slot 40 of the state's world-painter query object m_query[7] (+0x1C) of cRenderStateMan_m_state[@p
 * i] and returns the pointer it gives. cRenderStateMan_ObjectIrradiance calls it.
 *
 * @param i  the render state (view) index
 * @return the name field
 */
extern "C" void* cRenderStateMan_ObjectIrradianceName(int i)
{
    if (D_004A3B64 != 0) {
        return &D_004A55B0;
    }
    {
        sRSMEntry* renderState = &cRenderStateMan_m_state[i];
        sRSMObj* stateQuery = *renderState->p1C;
        sRSMVEntry* vt = stateQuery->vt;
        return vt[40].fn((char*)stateQuery + vt[40].delta);
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
        sRSMEntry* stateEntry = &cRenderStateMan_m_state[i];
        ((cRSMObjWF*)*stateEntry->p20)->v64(frame);
        ((cRSMObjWF*)*stateEntry->p20)->v64(frame);
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
        sRSMEntry* entry = &cRenderStateMan_m_state[i];
        ((cRSMObjRF*)*entry->p20)->v65(frame);
        ((cRSMObjRF*)*entry->p20)->v65(frame);
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
extern int D_004A3B64;
extern int D_004A55B0;

/**
 * Selects a named custom irradiance for cut scenes, or turns it off.
 *
 * A null @p name clears the flag D_004A3B64; otherwise the name is copied to D_004A55B0 and the flag is set.
 *
 * @param name  irradiance name (under 8 characters), or 0 for none (On Tour pName)
 */
extern "C" void cRenderStateMan_setCustomIrradiance(const char* name)
{
    if (name == 0) {
        D_004A3B64 = 0;
    } else {
        strcpy((char*)&D_004A55B0, name);
        D_004A3B64 = 1;
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
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x14) = mode;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_update);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);
extern "C" int AITick();

struct sRsVec4_EF6D0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRsSeg_EF6D0 {
    sRsVec4_EF6D0 p0;
    sRsVec4_EF6D0 p1;
};

struct sTrail_EF6D0 {
    char* rider;                // 0x0
    sRsSeg_EF6D0* ring;         // 0x4
    int head;                   // 0x8
    int count;                  // 0xC
    float start;                // 0x10
    int len;                    // 0x14
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRsVec4_EF6D0 Scale_EF6D0(const sRsVec4_EF6D0& v, float s)
{
    sRsVec4_EF6D0 r;
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
static inline sRsVec4_EF6D0 Add_EF6D0(const sRsVec4_EF6D0& a, const sRsVec4_EF6D0& b)
{
    sRsVec4_EF6D0 r;
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
static inline sRsVec4_EF6D0 Sub_EF6D0(const sRsVec4_EF6D0& a, const sRsVec4_EF6D0& b)
{
    sRsVec4_EF6D0 r;
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
static inline float Length_EF6D0(const sRsVec4_EF6D0& v)
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
extern "C" void cAirStreamerFX_update(sTrail_EF6D0* self)
{
    if (cRider_getMotionState(self->rider) == 1) {
        char* riderPtr = self->rider;
        sRsVec4_EF6D0 bodyOff = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x89C) << 6) + 0x30);
        sRsVec4_EF6D0 boardOff = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x30) + (*(int*)(riderPtr + 0x8A4) << 6) + 0x30);
        sRsVec4_EF6D0 boardDir = *(sRsVec4_EF6D0*)(*(char**)(*(char**)(riderPtr + 0x780) + 0x34) + (*(int*)(riderPtr + 0x8A4) << 6));
        sRsVec4_EF6D0 streamBase = Sub_EF6D0(boardOff, Scale_EF6D0(bodyOff, 0.699999988079071f));
        self->ring[self->head].p0 = Add_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        self->ring[self->head].p1 = Sub_EF6D0(streamBase, Scale_EF6D0(boardDir, 90.0f));
        if ((AITick() & 1) == 0) {
            int old = self->head;
            self->head = old + 1;
            self->head %= 25;
            if (self->count < self->len)
                self->count++;
            self->ring[self->head] = self->ring[old];
        }
    } else {
        int n = self->count;
        self->count = Approach_EF6D0(n, 2);
    }
    self->start -= Length_EF6D0(*(sRsVec4_EF6D0*)(self->rider + 0x1E0)) * 0.00016666666488163173f;
    if (self->start < 0.0f)
        self->start += 1.0f;
    if (self->start > 1.0f)
        self->start -= 1.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAirStreamerFX_render);
#ifdef SKIP_ASM
struct sRsVec4_EF950 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRsColor_EF950 {
    float x, y, z, w;
    sRsColor_EF950() {}
    sRsColor_EF950(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
};

struct sRsVertUV_EF950 {
    float x, y, z, w;
    sRsVertUV_EF950() {}
} __attribute__((aligned(16)));

struct sRsVert_EF950 {
    sRsVertUV_EF950 uv;     // 0x00
    int r, g, b, a;         // 0x10
    sRsVec4_EF950 pos;      // 0x20
};

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
    void (*fn)(void*, int, sRsVert_EF950*, int);
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

struct sRsSeg_EF950 {
    sRsVec4_EF950 p0;
    sRsVec4_EF950 p1;
};

struct sTrail_EF950 {
    char* rider;                // 0x0
    sRsSeg_EF950* ring;         // 0x4
    int head;                   // 0x8
    int count;                  // 0xC
    float start;                // 0x10
    int len;                    // 0x14
};

extern sRsCtx_EF950* GrMan_renderstateman __asm__("GrMan");
extern sRsState_EF950 sDrawState_s_defaultDrawState[];
extern int cPowerUpFXToggles_m_TestTrickBoost;
extern float D_004A3B74;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sRsVec4_EF950 rsScale_EF950(const sRsVec4_EF950& v, float s)
{
    sRsVec4_EF950 r;
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
static inline sRsVec4_EF950 rsAdd_EF950(const sRsVec4_EF950& a, const sRsVec4_EF950& b)
{
    sRsVec4_EF950 r;
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
static inline sRsVec4_EF950 rsSub_EF950(const sRsVec4_EF950& a, const sRsVec4_EF950& b)
{
    sRsVec4_EF950 r;
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

static inline sRsVec4_EF950 rsRow_EF950(char* base, int idx, int off)
{
    return *(sRsVec4_EF950*)(base + (idx << 6) + off);
}

static inline void rsVert_EF950(sRsVert_EF950* v, float u, float w, const sRsColor_EF950& c, const sRsVec4_EF950& p)
{
    v->r = (int)(c.y * 255.0f);
    v->g = (int)(c.z * 255.0f);
    v->b = (int)(c.w * 255.0f);
    v->a = (int)(c.x * 128.0f);
    v->uv.z = 1.0f;
    v->pos = p;
    v->uv.x = u;
    v->uv.y = w;
}

/**
 * Draws the air streamer as two textured triangle strips that fade toward the tail.
 *
 * Needs at least two segments. Normally 15 units wide at alpha 0.3 with GrMan _EffectTexMap[63]; while a
 * trick boost runs (mPowerUpTrickTimer > 0 or the TestTrickBoost toggle) it glows: 37.5 wide, alpha 1, tinted
 * D_004A3B74, _EffectTexMap[61] scrolled by the trail offset. Segment alpha falls off as 1 - t^2/(len-1)^2,
 * and the segments are placed back at 0.7 x the current hips position.
 */
extern "C" void cAirStreamerFX_render(sTrail_EF950* self)
{
    if (self->count < 2) {
        return;
    }
    rsPush_EF950(GrMan_renderstateman);
    *GrMan_renderstateman->top = sDrawState_s_defaultDrawState[0];
    int glow = 0;
    float alpha = 0.3f;
    float start = 0.0f;
    if (cPowerUpFXToggles_m_TestTrickBoost != 0 || 0.0f < *(float*)(self->rider + 0x2EC)) {
        glow = 1;
        alpha = 1.0f;
        *(short*)((char*)GrMan_renderstateman->top + 0x10) = rsTex1044_EF950(GrMan_renderstateman);
        GrMan_renderstateman->top->f0 &= ~0xC;
        start = self->start;
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
    sRsVert_EF950 vertsL[52];
    sRsVert_EF950 vertsR[52];
    char* rider = self->rider;
    float uvy = start;
    float invSq = 1.0f / (float)((self->len - 1) * (self->len - 1));
    sRsVec4_EF950 off = rsScale_EF950(rsRow_EF950(*(char**)(*(char**)(rider + 0x780) + 0x30), *(int*)(rider + 0x89C), 0x30), 0.7f);
    float dv = 1.0f / (float)self->count;
    float falloffT = 0.0f;
    int nverts = 0;
    sRsColor_EF950 color;
    color.x = 1.0f;
    color.y = 1.0f;
    color.z = 1.0f;
    color.w = 1.0f;
    sRsVec4_EF950 leftPos;
    sRsVec4_EF950 rightPos;
    rider = self->rider;
    sRsVec4_EF950 col = rsRow_EF950(*(char**)(*(char**)(rider + 0x780) + 0x34), *(int*)(rider + 0x8A4), 0x20);
    float size = 15.0f;
    if (glow) {
        float k = D_004A3B74;
        color = sRsColor_EF950(1.0f, k, k, k);
        size = 37.5f;
    }
    sRsVec4_EF950 half = rsScale_EF950(col, size * 0.5f);
    int j = self->head;
    int k = 0;
    for (int i = 0; i < self->count; i++) {
        if (j < 0) {
            j = 24;
        }
        color.x = alpha * (1.0f - falloffT * falloffT * invSq);
        if (color.x < 0.0f) {
            color.x = 0.0f;
        }
        leftPos = rsAdd_EF950(self->ring[j].p0, off);
        rightPos = rsAdd_EF950(self->ring[j].p1, off);
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
    grMan->vtable[71].fn((char*)grMan + grMan->vtable[71].delta, nverts, vertsL, 0);
    grMan = GrMan_renderstateman;
    grMan->vtable[71].fn((char*)grMan + grMan->vtable[71].delta, nverts, vertsR, 0);
    rsPop_EF950(GrMan_renderstateman);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", visualEffectsMan);
#ifdef SKIP_ASM
extern void* D_004A3B78;

/**
 * Returns the visual effects manager (D_004A3B78), which cVisualEffectsMan_cVisualEffectsMan records.
 *
 * Nothing in the retail code references it.
 *
 * @return the manager
 */
extern "C" int visualEffectsMan(void) {
    return (*(int*)&D_004A3B78);
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
extern void* D_004A3B78;

/**
 * Constructs the visual-effects manager base: empty effect slots, default effect textures, global pointer.
 *
 * Sets the vtable (+0x44), clears the 17 effect-object slots, stores 0x37 in D_004A45D4 and 0x30 in
 * D_004A443C and makes itself the manager D_004A3B78. On the graphics manager it sets view 6's specular,
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
    D_004A3B78 = self;
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
    void* a8C[8];       // 0x8C
    float aAC[8];       // 0xAC
    float aCC[8];       // 0xCC
    float aEC[8];       // 0xEC
    float a10C[8];      // 0x10C
    float a12C[8];      // 0x12C
    float a14C[8];      // 0x14C
    float a16C[8];      // 0x16C
    float a18C[8];      // 0x18C
    float a1AC[8];      // 0x1AC
    float a1CC[8];      // 0x1CC
    float a1EC[8];      // 0x1EC
    float a20C[8];      // 0x20C
    float a22C[8];      // 0x22C
    float a24C[8];      // 0x24C
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
    ctx->a8C[i] = cRenderStateMan_m_inNIS;
    ctx->aAC[i] = cRenderStateMan_SnowSparkleDensity(i);
    ctx->aCC[i] = cRenderStateMan_DrawDistance(i);
    ctx->aEC[i] = cRenderStateMan_FogDensity(i);
    ctx->a10C[i] = cRenderStateMan_FogColourR(i);
    ctx->a12C[i] = cRenderStateMan_FogColourG(i);
    ctx->a14C[i] = cRenderStateMan_FogColourB(i);
    ctx->a16C[i] = cRenderStateMan_FogNear(i);
    ctx->a18C[i] = cRenderStateMan_FogFar(i);
    ctx->a1AC[i] = cRenderStateMan_ScreenTintScaleR(i);
    ctx->a1CC[i] = cRenderStateMan_ScreenTintScaleG(i);
    ctx->a1EC[i] = cRenderStateMan_ScreenTintScaleB(i);
    ctx->a20C[i] = cRenderStateMan_ScreenTintOffsetR(i);
    ctx->a22C[i] = cRenderStateMan_ScreenTintOffsetG(i);
    ctx->a24C[i] = cRenderStateMan_ScreenTintOffsetB(i);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", aivisualEffectsMan);
#ifdef SKIP_ASM
extern void* D_004A3B80;

/**
 * Returns the AI visual effects manager (D_004A3B80), which cAIVisualEffectsMan_cAIVisualEffectsMan records.
 *
 * Nothing in the retail code references it.
 *
 * @return the manager
 */
extern "C" int aivisualEffectsMan(void) {
    return (*(int*)&D_004A3B80);
}
#endif

//100%
INCLUDE_ASM("visualfx/renderstateman", cAIVisualEffectsMan_cAIVisualEffectsMan);
#ifdef SKIP_ASM
extern void* cAIVisualEffectsMan_vtbl[];
extern void* D_004A3B80;

/**
 * Constructs the base AI visual effects manager and makes it the current one.
 *
 * Clears +0x0, sets the vtable cAIVisualEffectsMan_vtbl at +0x4 and stores this in D_004A3B80.
 *
 * @return this
 */
extern "C" void* cAIVisualEffectsMan_cAIVisualEffectsMan(cAIVisualEffectsMan* self)
{
    *(int*)self = 0;
    self->mVtbl = cAIVisualEffectsMan_vtbl;
    D_004A3B80 = ((void*)self);
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
 * The base constructor records it in D_004A3B78; this function stores it nowhere itself.
 *
 * @return the new manager
 */
extern "C" void* cVisualEffectsMan_construct(void)
{
    return cPS2VisualEffectsMan_cPS2VisualEffectsMan(cMemMan_alloc(72, str_PS2VisualEffectsMan, 0, 0));
}
#endif

