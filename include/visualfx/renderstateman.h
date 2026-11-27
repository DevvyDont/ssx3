#ifndef VISUALFX_RENDERSTATEMAN_H
#define VISUALFX_RENDERSTATEMAN_H

// cRenderStateMan: the per-view render state - what the world painter says about the place a view (rider / camera)
// is at: fog, light glow, screen tint, sky box, sun, surface, lighting and weather values, the patch colour under the
// rider and the rider's irradiance. A static-only class (On Tour: size 1, every member static; retail has no
// allocation and no vtable): its state is the array cRenderStateMan::m_state of 8 tRenderState rows, one per view
// index (the riders' kRiderVt_getAIIndex); the getters (FogNear(i), SunColorR(i), SnowIntensity(i), ...) read row
// i's world-painter query, update(i, pos) re-queries row i and blends its colours and irradiance.
// EA's source: d:/ssx/code/source/visualfx/renderstateman.cpp (preview assert path); retail keeps purge .. the
// irradiance getters in the splat unit visualfx/lensfx (0x002ECFA0 .. 0x002EE390) and the rest of the getters,
// writeToReplayFrame, readFromReplayFrame, setInNIS and setCustomIrradiance in visualfx/renderstateman
// (.. 0x002EF3A8).
//
// This header lays out the row, tRenderState (EA's name: tRenderState_blendToColour / _blendToColour1 are its two
// methods; On Tour struct tRenderState).
// Size 0xF0: [U] every getter indexes m_state with `mult i * 0xF0`; init / purge / writeToReplayFrame step a row
// pointer by 0xF0 (8 rows; the replay loop stops at 0x5A0 = 6 rows); config symbol cRenderStateMan_m_state is
// 0x780 = 8 * 0xF0. [O] On Tour's row is 0xA0 (its cIrradiance is 0x44; retail's is 0xA0: cIrradianceDataBase_Get
// steps mIrradianceArray by 0xA0). No base classes, no vtable.
//
// Statics (not declared here: the .cpp files declare these symbols with differing types - sLensFxSlot[],
// sRSMEntry[], sLensRow_*[], char[] for m_state; int / void* for the flags):
//   cRenderStateMan::m_state[8]          = cRenderStateMan_m_state (0x004FA370, tRenderState[8]) [O][U]
//   cRenderStateMan::m_initialized       = cRenderStateMan_m_initialized (0x004A3B54, int) [O]; [U] init sets, purge tests
//   cRenderStateMan::m_doColourReset     = cRenderStateMan_m_doColourReset (int) [O] order; [U] update passes it as blendToColour's resetcolour
//   cRenderStateMan::m_inNIS             = cRenderStateMan_m_inNIS (0x004A3B5C, int) [O]; [U] setInNIS
//   cRenderStateMan::m_indoor            = cRenderStateMan_m_indoor (int) [O] order; [U] the snow / screen-tint getters test it
//   cRenderStateMan::m_hasCustomIrradiance = cRenderStateMan_m_hasCustomIrradiance (int) [O]; [U] setCustomIrradiance, the *IrradianceName getters
//   cRenderStateMan::m_customIrradiance  = cRenderStateMan_m_customIrradiance (tWPNameField, 8 bytes) [O]; [U] setCustomIrradiance strcpy's into it
//   cRenderStateMan::m_sectionId         = cRenderStateMan_m_sectionId (unsigned int) [O]; [U] update stores the section id (also into
//                                          cWorldPainterMan_m_iCurrSection)
//   cRenderStateMan::m_farPlane          = D_004A3B70 (float) [O]; [U] update reads it (lwc1)
//
// A FLAT layout, like main/game.h. Tags: [R] retail's cRenderStateMan_init (fills each row); [U] how retail code
// uses the field; [O] SSX On Tour's DWARF (struct tRenderState: m_query[9] 0x0, m_inTunnel 0x24, m_colour 0x30,
// m_lightMapColour 0x40, m_irradiance 0x50 - On Tour's tARGB is 16-aligned; retail's is 4-aligned (blendToColour
// stores the colours with sdl/sdr), so the colours sit at 0x28 / 0x38 here). No preview evidence (no accessors, no
// paired methods for layout_xlate.py).

#include "layout.h"
#include "ui/uithing.h"

struct tWorldPainterQuery;   // worldpainter/worldpainterquery.h (cWorldPainterQuery, 0x18; m_data at 0x0)

struct tRenderState {
    tWorldPainterQuery* m_query[9]; // 0x000 [O] m_query[9]; [R] cRenderStateMan_init stores cWorldPainterMan_getQuery for
                                    //       kWPR_Camera, Fog, LightGlow, ScreenTint, SkyBox, Sun, Surface, Lighting, Weather
                                    //       (q[0]..q[8]); [U] purge deletes each (slot 1, flags 3); update calls each one's
                                    //       queryPosition (slot 1); the getters read q[0] DrawDistance, q[1] Fog*,
                                    //       q[2] LightGlow*, q[3] ScreenTint*, q[4] SkyBoxRotation, q[5] Sun* / LensIntensity,
                                    //       q[6] SnowSparkleDensity, q[7] the irradiance names / Rim/AmbientLightIntensityOffset,
                                    //       q[8] the weather getters (Snow*, Wind*, FlakeSize, Temperature, LightningChance);
                                    //       write/readFromReplayFrame call q[8]'s data slots 64 / 65
    float m_inTunnel;               // 0x024 [O]; [R] init zeroes it; [U] update blends it (x * 0.9 [+ 0.1 in a tunnel]);
                                    //       SnowIntensity / WindIntensity / SnowSparkleDensity scale by it (lwc1)
    tARGB m_colour;                 // 0x028 [O] tARGB m_colour {A, R, G, B}; [R] init sets 1.0 x4; [U] blendToColour /
                                    //       blendToColour1 (self + 0x28, sdl/sdr); boardsparkfx / boardwakefx read
                                    //       m_state[ai].m_colour
    tARGB m_lightMapColour;         // 0x038 [O]; [R] init sets 1.0 x4; [U] blendToColour (+0x38);
                                    //       update passes &m_state[i].m_lightMapColour to calculateIrradianceBlend
    char unk_048[0x8];              // 0x048 alignment gap: m_irradiance is 16-byte aligned (lq/sq)
    char m_irradiance[0xA0];        // 0x050 [O] cIrradiance m_irradiance (retail 0xA0: cIrradianceDataBase_Get's stride);
                                    //       [U] getIrradiance(i) returns &m_state[i] + 0x50; update copies irradiance
                                    //       records into it (lq/sq 0x20 at a time)
};

CHECK_OFFSET(tRenderState, m_query, 0x000);
CHECK_OFFSET(tRenderState, m_inTunnel, 0x024);
CHECK_OFFSET(tRenderState, m_colour, 0x028);
CHECK_OFFSET(tRenderState, m_lightMapColour, 0x038);
CHECK_OFFSET(tRenderState, unk_048, 0x048);
CHECK_OFFSET(tRenderState, m_irradiance, 0x050);
CHECK_SIZE(tRenderState, 0x0F0);

#endif /* VISUALFX_RENDERSTATEMAN_H */
