#ifndef WORLDPAINTER_WORLDPAINTERQUERY_H
#define WORLDPAINTER_WORLDPAINTERQUERY_H

// The world painter's query side. A system that wants painted values at a position (lens flares, audio ambience
// and mix, the rider's lighting, ...) asks cWorldPainterMan::getQuery(kWPR_<X>) for a cWorldPainterQuery (0x18 bytes,
// tag "WorldPainterQuery"); its constructor allocates the matching tWPInGameData_<X> (tag "tWPIGD_<X>") into
// m_data and links the query into the global list cWorldPainterQuery::m_queryListStart. queryPosition(fx, fy, blend)
// adds the distance moved to m_data->m_wpTravelDistance, looks the position up in the current section's layer
// m_regionType.m_layer (cWorldPainterLayer, worldpainter/worldpainterlayer.h), and when the region's m_iRegionType
// is m_regionType.m_id, calls the data's virtual blend(region data, blend) / isEqual / setDefaults; it returns m_data,
// whose virtual getters (FogNear(), SunColorR(), ...) the caller reads. cWorldPainterQuery::reset (via
// cWorldPainterMan::reset) sets every query's m_data->m_wpTravelDistance back to -99999.0f ("no position yet").
//
// Classes here (sizes [R] from the allocations; On Tour: the same sizes and members):
//   tWorldPainterRegionType 0x4, tWPNameField 0x8, tWPData 0x4 + tWPData_<X> (the file's region data),
//   tWPInGameData 0x8 + 14 tWPInGameData_<X> (vptr at 0x4, after tWPInGameData's one data member), tWorldPainterQuery
//   0x8 (m_data + vptr; vtable D_00485D30 {dtor, pure queryPosition}), cWorldPainterQuery 0x18 (derives from
//   tWorldPainterQuery; vtable cWorldPainterQuery_vtbl = D_00483E00 {dtor cWorldPainterQuery__cWorldPainterQuery, queryPosition cWorldPainterQuery_queryPosition}).
// tWPInGameData's vtable (base D_00485D50, 69 slots): 1-63 the getters in On Tour's order (Type, Value, MixPreset,
// BankID, SpeechID, DrawDistance, FogDensity ... PointRate; each subclass overrides its own and they return
// this + offset), 64 writeToReplayFrame, 65 readFromReplayFrame (64/65 order from On Tour's declaration order,
// not verified), 66 blend(const tWPData*, float), 67 debugBlend, 68 isEqual(const tWPData*), 69 setDefaults; the
// base's 64-66/68/69 are pure (func_00411530), its getters return 0. No virtual destructor.
// EA's source: d:/ssx/code/source/worldpainter/worldpainterquery.cpp (preview: the "tWPIGD_<X>" and
// "WorldPainterQuery" tags, "iContents < pWPLayer->getNumRegions()"); it is one translation unit in retail
// (0x002BC500-0x002C16A8: tWPInGameData's code from 0x002BC500 in bx/seg_1BA100, then cWorldPainterQuery from
// 0x002C03A8, its static initializer func_002C0B70 and the inline getters at 0x002C1490). The header EA declared
// these in is not known; worldpainterquery.h is our choice.
// Methods: cWorldPainterQuery/methods.txt (queries), tWPInGameData/methods.txt (the data classes).
//
// Statics (not declared here: the .cpp files declare these D_ symbols with differing types):
//   cWorldPainterQuery::m_queryListStart = cWorldPainterQuery_m_queryListStart ([O] static member; [U] ctor, dtor, reset);
//   kNullNameField (static const tWPNameField, a copy per translation unit: D_004A52E0 worldpainterlayer,
//   D_004A5358 worldpainterman, D_004A53D0 worldpainterquery; [O] On Tour globals; [R] the static initializers zero
//   it, the Lighting ctor/setDefaults copy it into the names);
//   kWPR_MusicTrigger..kWPR_Danger (static const tWorldPainterRegionType(n, n), n = 0..13, a copy per translation
//   unit, 8 bytes apart: D_004A52E8.. worldpainterlayer, D_004A5360.. worldpainterman, D_004A53D8.. worldpainterquery,
//   kWPR_Lighting = D_004A5598 lensfx's copy; [O] On Tour globals kWPR_*).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructors (cWorldPainterQuery_cWorldPainterQuery, func_002BC6F8 ... func_002BCBD0 for the
//       tWPInGameData_<X>), destructors (cWorldPainterQuery__cWorldPainterQuery, tWorldPainterQuery__tWorldPainterQuery) and allocations;
//   [U] how retail code uses the field (getters, blend/isEqual, queryPosition cWorldPainterQuery_queryPosition);
//   [O] SSX On Tour's DWARF (all of these types): member names, order and, here, the same offsets (no bools or
//       vectors in these classes).

#include "layout.h"

struct cWorldPainterLayer;
struct tWPRegion;

// Which painted resource a query reads: m_layer picks the section's layer slot, m_id must equal the region's
// m_iRegionType and picks the tWPInGameData_<X> (0 MusicTrigger, 1 Mix, 2 Ambience, 3 Speech, 4 Camera, 5 Fog,
// 6 LightGlow, 7 ScreenTint, 8 SkyBox, 9 Sun, 10 Surface, 11 Lighting, 12 Weather, 13 Danger). Bit fields: no
// CHECK_OFFSET; the word is at offset 0.
struct tWorldPainterRegionType {
    unsigned int m_layer : 8;       // bits 0-7 [O]; [R] the ctor stores the low byte (sb); [U] queryPosition indexes
                                    //       the header's layer slots with it (lbu)
    unsigned int m_id : 24;         // bits 8-31 [O]; [R] the ctor switches on it (>> 8); [U] compared with m_iRegionType
};

CHECK_SIZE(tWorldPainterRegionType, 0x4);

// A painted name (irradiance map names of tWPInGameData_Lighting). On Tour: one member m_nameId of an 8-byte union
// type (DWARF name lost); retail copies it with 8-byte moves and compares it as two words (func_002BB100).
struct tWPNameField {
    int m_nameId[2];                // 0x0 [O] m_nameId (written as two words); [R] kNullNameField's ctor zeroes both
};

CHECK_OFFSET(tWPNameField, m_nameId, 0x0);
CHECK_SIZE(tWPNameField, 0x8);

// The file side's base (On Tour: tWPData, 4 bytes).
struct tWPData {
    float m_blendData;              // 0x0 [O]
};

CHECK_OFFSET(tWPData, m_blendData, 0x0);
CHECK_SIZE(tWPData, 0x4);

// The in-game side's base (On Tour: tWPInGameData, 8 bytes: its one data member, then the vptr).
struct tWPInGameData {
    float m_wpTravelDistance;       // 0x0 [O]; [R] every subclass ctor stores -99999.0f; [U] cWorldPainterQuery_reset
                                    //       stores -99999.0f, queryPosition adds the distance moved
    void** mVtbl;                   // 0x4 [R] the subclass ctors store their vtable; base vtable D_00485D50
};

CHECK_OFFSET(tWPInGameData, m_wpTravelDistance, 0x0);
CHECK_OFFSET(tWPInGameData, mVtbl, 0x4);
CHECK_SIZE(tWPInGameData, 0x8);

// ---- The painted data of one region (the file side; tWPRegion::m_pData points at one). On Tour: tWPData_<X>, a
// tWPData (m_blendData) followed by the values; [U] each subclass's blend (vtable slot 66) and isEqual (slot 68)
// read exactly these offsets (ints through lw, or lwc1 for the int->float blend).
struct tWPData_MusicTrigger {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_Type;                      // 0x04 [O]; [U] tWPInGameData_MusicTrigger_blend (blend) / func_002BDA98 (isEqual)
    int m_Value;                     // 0x08 [O]; [U] tWPInGameData_MusicTrigger_blend (blend) / func_002BDA98 (isEqual)
};

CHECK_OFFSET(tWPData_MusicTrigger, m_blendData, 0x00);
CHECK_OFFSET(tWPData_MusicTrigger, m_Type, 0x04);
CHECK_OFFSET(tWPData_MusicTrigger, m_Value, 0x08);
CHECK_SIZE(tWPData_MusicTrigger, 0x0C);

struct tWPData_Mix {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_MixPreset;                 // 0x04 [O]; [U] tWPInGameData_Mix_blend (blend) / func_002BDAC8 (isEqual)
};

CHECK_OFFSET(tWPData_Mix, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Mix, m_MixPreset, 0x04);
CHECK_SIZE(tWPData_Mix, 0x08);

struct tWPData_Ambience {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_BankID;                    // 0x04 [O]; [U] tWPInGameData_Ambience_blend (blend) / func_002BDAE0 (isEqual)
};

CHECK_OFFSET(tWPData_Ambience, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Ambience, m_BankID, 0x04);
CHECK_SIZE(tWPData_Ambience, 0x08);

struct tWPData_Speech {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_SpeechID;                  // 0x04 [O]; [U] tWPInGameData_Speech_blend (blend) / func_002BDAF8 (isEqual)
};

CHECK_OFFSET(tWPData_Speech, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Speech, m_SpeechID, 0x04);
CHECK_SIZE(tWPData_Speech, 0x08);

struct tWPData_Camera {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_DrawDistance;            // 0x04 [O]; [U] tWPInGameData_Camera_blend (blend) / func_002BDB10 (isEqual)
};

CHECK_OFFSET(tWPData_Camera, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Camera, m_DrawDistance, 0x04);
CHECK_SIZE(tWPData_Camera, 0x08);

struct tWPData_Fog {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_FogDensity;              // 0x04 [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
    float m_FogNear;                 // 0x08 [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
    float m_FogFar;                  // 0x0C [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
    float m_FogColourR;              // 0x10 [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
    float m_FogColourG;              // 0x14 [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
    float m_FogColourB;              // 0x18 [O]; [U] tWPInGameData_Fog_blend (blend) / func_002BDB38 (isEqual)
};

CHECK_OFFSET(tWPData_Fog, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Fog, m_FogDensity, 0x04);
CHECK_OFFSET(tWPData_Fog, m_FogNear, 0x08);
CHECK_OFFSET(tWPData_Fog, m_FogFar, 0x0C);
CHECK_OFFSET(tWPData_Fog, m_FogColourR, 0x10);
CHECK_OFFSET(tWPData_Fog, m_FogColourG, 0x14);
CHECK_OFFSET(tWPData_Fog, m_FogColourB, 0x18);
CHECK_SIZE(tWPData_Fog, 0x1C);

struct tWPData_LightGlow {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_LightGlowCutoff;         // 0x04 [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowCutoffScale;    // 0x08 [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowCopyIntensity;  // 0x0C [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowSourceIntensity; // 0x10 [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowFrameIntensity; // 0x14 [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowBlend2;         // 0x18 [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
    float m_LightGlowBlend3;         // 0x1C [O]; [U] tWPInGameData_LightGlow_blend (blend) / func_002BDBD0 (isEqual)
};

CHECK_OFFSET(tWPData_LightGlow, m_blendData, 0x00);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowCutoff, 0x04);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowCutoffScale, 0x08);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowCopyIntensity, 0x0C);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowSourceIntensity, 0x10);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowFrameIntensity, 0x14);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowBlend2, 0x18);
CHECK_OFFSET(tWPData_LightGlow, m_LightGlowBlend3, 0x1C);
CHECK_SIZE(tWPData_LightGlow, 0x20);

struct tWPData_ScreenTint {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_ScreenTintScaleR;        // 0x04 [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
    float m_ScreenTintScaleG;        // 0x08 [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
    float m_ScreenTintScaleB;        // 0x0C [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
    float m_ScreenTintOffsetR;       // 0x10 [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
    float m_ScreenTintOffsetG;       // 0x14 [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
    float m_ScreenTintOffsetB;       // 0x18 [O]; [U] tWPInGameData_ScreenTint_blend (blend) / func_002BDC78 (isEqual)
};

CHECK_OFFSET(tWPData_ScreenTint, m_blendData, 0x00);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintScaleR, 0x04);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintScaleG, 0x08);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintScaleB, 0x0C);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintOffsetR, 0x10);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintOffsetG, 0x14);
CHECK_OFFSET(tWPData_ScreenTint, m_ScreenTintOffsetB, 0x18);
CHECK_SIZE(tWPData_ScreenTint, 0x1C);

struct tWPData_SkyBox {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_SkyBoxRotation;          // 0x04 [O]; [U] tWPInGameData_SkyBox_blend (blend) / func_002BDD10 (isEqual)
};

CHECK_OFFSET(tWPData_SkyBox, m_blendData, 0x00);
CHECK_OFFSET(tWPData_SkyBox, m_SkyBoxRotation, 0x04);
CHECK_SIZE(tWPData_SkyBox, 0x08);

struct tWPData_Sun {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_SunPitchOffset;          // 0x04 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunYawOffset;            // 0x08 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunColorR;               // 0x0C [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunColorG;               // 0x10 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunColorB;               // 0x14 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunIntensity;            // 0x18 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    int m_SunType;                   // 0x1C [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_LensIntensity;           // 0x20 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
    float m_SunRadius;               // 0x24 [O]; [U] tWPInGameData_Sun_blend (blend) / func_002BDD38 (isEqual)
};

CHECK_OFFSET(tWPData_Sun, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Sun, m_SunPitchOffset, 0x04);
CHECK_OFFSET(tWPData_Sun, m_SunYawOffset, 0x08);
CHECK_OFFSET(tWPData_Sun, m_SunColorR, 0x0C);
CHECK_OFFSET(tWPData_Sun, m_SunColorG, 0x10);
CHECK_OFFSET(tWPData_Sun, m_SunColorB, 0x14);
CHECK_OFFSET(tWPData_Sun, m_SunIntensity, 0x18);
CHECK_OFFSET(tWPData_Sun, m_SunType, 0x1C);
CHECK_OFFSET(tWPData_Sun, m_LensIntensity, 0x20);
CHECK_OFFSET(tWPData_Sun, m_SunRadius, 0x24);
CHECK_SIZE(tWPData_Sun, 0x28);

struct tWPData_Surface {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_SnowSparkleDensity;      // 0x04 [O]; [U] tWPInGameData_Surface_blend (blend) / func_002BDE08 (isEqual)
};

CHECK_OFFSET(tWPData_Surface, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Surface, m_SnowSparkleDensity, 0x04);
CHECK_SIZE(tWPData_Surface, 0x08);

struct tWPData_Lighting {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    tWPNameField m_LightIrradianceName; // 0x04 [O]; [U] tWPInGameData_Lighting_blend (blend) copies it (ldl/ldr)
    tWPNameField m_ShadowIrradianceName; // 0x0C [O]; [U] tWPInGameData_Lighting_blend (blend) copies it (ldl/ldr)
    tWPNameField m_TunnelIrradianceName; // 0x14 [O]; [U] tWPInGameData_Lighting_blend (blend) copies it (ldl/ldr)
    tWPNameField m_ObjectIrradianceName; // 0x1C [O]; [U] tWPInGameData_Lighting_blend (blend) copies it (ldl/ldr)
    float m_AmbientLightIntensityOffset; // 0x24 [O]; [U] tWPInGameData_Lighting_blend (blend) / func_002BDE30 (isEqual)
    float m_RimLightIntensityOffset; // 0x28 [O]; [U] tWPInGameData_Lighting_blend (blend) / func_002BDE30 (isEqual)
};

CHECK_OFFSET(tWPData_Lighting, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Lighting, m_LightIrradianceName, 0x04);
CHECK_OFFSET(tWPData_Lighting, m_ShadowIrradianceName, 0x0C);
CHECK_OFFSET(tWPData_Lighting, m_TunnelIrradianceName, 0x14);
CHECK_OFFSET(tWPData_Lighting, m_ObjectIrradianceName, 0x1C);
CHECK_OFFSET(tWPData_Lighting, m_AmbientLightIntensityOffset, 0x24);
CHECK_OFFSET(tWPData_Lighting, m_RimLightIntensityOffset, 0x28);
CHECK_SIZE(tWPData_Lighting, 0x2C);

struct tWPData_Weather {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_SnowIntensity;           // 0x04 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_FlakeSize;               // 0x08 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_WindIntensity;           // 0x0C [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_WindRotation;            // 0x10 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_PlayerWindIntensity;     // 0x14 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_Temperature;             // 0x18 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFlurries;            // 0x1C [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowGravityMultiplier;   // 0x20 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffItensity;       // 0x24 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffWindIntensity;  // 0x28 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_LightningChance;         // 0x2C [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFlakeColourA;        // 0x30 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFlakeColourR;        // 0x34 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFlakeColourG;        // 0x38 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFlakeColourB;        // 0x3C [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffColourA;        // 0x40 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffColourR;        // 0x44 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffColourG;        // 0x48 [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
    float m_SnowFluffColourB;        // 0x4C [O]; [U] tWPInGameData_Weather_blend (blend) / func_002BDEE0 (isEqual)
};

CHECK_OFFSET(tWPData_Weather, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Weather, m_SnowIntensity, 0x04);
CHECK_OFFSET(tWPData_Weather, m_FlakeSize, 0x08);
CHECK_OFFSET(tWPData_Weather, m_WindIntensity, 0x0C);
CHECK_OFFSET(tWPData_Weather, m_WindRotation, 0x10);
CHECK_OFFSET(tWPData_Weather, m_PlayerWindIntensity, 0x14);
CHECK_OFFSET(tWPData_Weather, m_Temperature, 0x18);
CHECK_OFFSET(tWPData_Weather, m_SnowFlurries, 0x1C);
CHECK_OFFSET(tWPData_Weather, m_SnowGravityMultiplier, 0x20);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffItensity, 0x24);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffWindIntensity, 0x28);
CHECK_OFFSET(tWPData_Weather, m_LightningChance, 0x2C);
CHECK_OFFSET(tWPData_Weather, m_SnowFlakeColourA, 0x30);
CHECK_OFFSET(tWPData_Weather, m_SnowFlakeColourR, 0x34);
CHECK_OFFSET(tWPData_Weather, m_SnowFlakeColourG, 0x38);
CHECK_OFFSET(tWPData_Weather, m_SnowFlakeColourB, 0x3C);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffColourA, 0x40);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffColourR, 0x44);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffColourG, 0x48);
CHECK_OFFSET(tWPData_Weather, m_SnowFluffColourB, 0x4C);
CHECK_SIZE(tWPData_Weather, 0x50);

struct tWPData_Danger {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_Level;                     // 0x04 [O]; [U] tWPInGameData_Danger_blend (blend) / func_002BE078 (isEqual)
    float m_PointRate;               // 0x08 [O]; [U] tWPInGameData_Danger_blend (blend) / func_002BE078 (isEqual)
};

CHECK_OFFSET(tWPData_Danger, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Danger, m_Level, 0x04);
CHECK_OFFSET(tWPData_Danger, m_PointRate, 0x08);
CHECK_SIZE(tWPData_Danger, 0x0C);

// ---- The blended in-game values, one subclass per region type (tWorldPainterQuery::m_data points at one).
// Each value has a pair: m_<X> (the blended value the getter returns) and m_last<X> (the last target blended
// towards; blend stores the rhs value there). Sizes [R] from cWorldPainterQuery's cMemMan_alloc(size, "tWPIGD_<X>")
// (= On Tour sizes); the constructor stores the vtable at 0x4, -99999.0f into m_wpTravelDistance and the defaults.
struct tWPInGameData_MusicTrigger {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC6F8 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC6F8 stores the vtable D_00485AF8
    // --- tWPInGameData_MusicTrigger (alloc tag "tWPIGD_MusicTrigger", 0x18 bytes)
    int m_Type;                      // 0x08 [O]; [R] ctor default; [U] getter vtable slot 1 (func_002C1490) returns this + 0x8
    int m_lastType;                  // 0x0C [O]; [R] ctor default; [U] blend stores the target
    int m_Value;                     // 0x10 [O]; [R] ctor default; [U] getter vtable slot 2 (func_002C1498) returns this + 0x10
    int m_lastValue;                 // 0x14 [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_MusicTrigger, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_MusicTrigger, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_Type, 0x08);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_lastType, 0x0C);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_Value, 0x10);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_lastValue, 0x14);
CHECK_SIZE(tWPInGameData_MusicTrigger, 0x18);

struct tWPInGameData_Mix {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC728 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC728 stores the vtable D_004858C0
    // --- tWPInGameData_Mix (alloc tag "tWPIGD_Mix", 0x10 bytes)
    int m_MixPreset;                 // 0x08 [O]; [R] ctor default; [U] getter vtable slot 3 (func_002C14A0) returns this + 0x8
    int m_lastMixPreset;             // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Mix, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Mix, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Mix, m_MixPreset, 0x08);
CHECK_OFFSET(tWPInGameData_Mix, m_lastMixPreset, 0x0C);
CHECK_SIZE(tWPInGameData_Mix, 0x10);

struct tWPInGameData_Ambience {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC750 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC750 stores the vtable D_00485688
    // --- tWPInGameData_Ambience (alloc tag "tWPIGD_Ambience", 0x10 bytes)
    int m_BankID;                    // 0x08 [O]; [R] ctor default; [U] getter vtable slot 4 (func_002C14A8) returns this + 0x8
    int m_lastBankID;                // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Ambience, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Ambience, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Ambience, m_BankID, 0x08);
CHECK_OFFSET(tWPInGameData_Ambience, m_lastBankID, 0x0C);
CHECK_SIZE(tWPInGameData_Ambience, 0x10);

struct tWPInGameData_Speech {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC778 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC778 stores the vtable D_00485450
    // --- tWPInGameData_Speech (alloc tag "tWPIGD_Speech", 0x10 bytes)
    int m_SpeechID;                  // 0x08 [O]; [R] ctor default; [U] getter vtable slot 5 (func_002C14B0) returns this + 0x8
    int m_lastSpeechID;              // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Speech, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Speech, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Speech, m_SpeechID, 0x08);
CHECK_OFFSET(tWPInGameData_Speech, m_lastSpeechID, 0x0C);
CHECK_SIZE(tWPInGameData_Speech, 0x10);

struct tWPInGameData_Camera {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC7A0 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC7A0 stores the vtable D_00485218
    // --- tWPInGameData_Camera (alloc tag "tWPIGD_Camera", 0x10 bytes)
    float m_DrawDistance;            // 0x08 [O]; [R] ctor default; [U] getter vtable slot 6 (func_002C14B8) returns this + 0x8
    float m_lastDrawDistance;        // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Camera, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Camera, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Camera, m_DrawDistance, 0x08);
CHECK_OFFSET(tWPInGameData_Camera, m_lastDrawDistance, 0x0C);
CHECK_SIZE(tWPInGameData_Camera, 0x10);

struct tWPInGameData_Fog {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC7C8 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC7C8 stores the vtable D_00484FE0
    // --- tWPInGameData_Fog (alloc tag "tWPIGD_Fog", 0x38 bytes)
    float m_FogDensity;              // 0x08 [O]; [R] ctor default; [U] getter vtable slot 7 (func_002C14C0) returns this + 0x8
    float m_lastFogDensity;          // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_FogNear;                 // 0x10 [O]; [R] ctor default; [U] getter vtable slot 8 (func_002C14C8) returns this + 0x10
    float m_lastFogNear;             // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_FogFar;                  // 0x18 [O]; [R] ctor default; [U] getter vtable slot 9 (func_002C14D0) returns this + 0x18
    float m_lastFogFar;              // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourR;              // 0x20 [O]; [R] ctor default; [U] getter vtable slot 10 (func_002C14D8) returns this + 0x20
    float m_lastFogColourR;          // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourG;              // 0x28 [O]; [R] ctor default; [U] getter vtable slot 11 (func_002C14E0) returns this + 0x28
    float m_lastFogColourG;          // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourB;              // 0x30 [O]; [R] ctor default; [U] getter vtable slot 12 (func_002C14E8) returns this + 0x30
    float m_lastFogColourB;          // 0x34 [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Fog, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Fog, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Fog, m_FogDensity, 0x08);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogDensity, 0x0C);
CHECK_OFFSET(tWPInGameData_Fog, m_FogNear, 0x10);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogNear, 0x14);
CHECK_OFFSET(tWPInGameData_Fog, m_FogFar, 0x18);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogFar, 0x1C);
CHECK_OFFSET(tWPInGameData_Fog, m_FogColourR, 0x20);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogColourR, 0x24);
CHECK_OFFSET(tWPInGameData_Fog, m_FogColourG, 0x28);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogColourG, 0x2C);
CHECK_OFFSET(tWPInGameData_Fog, m_FogColourB, 0x30);
CHECK_OFFSET(tWPInGameData_Fog, m_lastFogColourB, 0x34);
CHECK_SIZE(tWPInGameData_Fog, 0x38);

struct tWPInGameData_LightGlow {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] tWPInGameData_LightGlow_tWPInGameData_LightGlow stores -99999.0f
    void** mVtbl;                    // 0x04 [R] tWPInGameData_LightGlow_tWPInGameData_LightGlow stores the vtable D_00484DA8
    // --- tWPInGameData_LightGlow (alloc tag "tWPIGD_LightGlow", 0x40 bytes)
    float m_LightGlowCutoff;         // 0x08 [O]; [R] ctor default; [U] getter vtable slot 13 (func_002C14F0) returns this + 0x8
    float m_lastLightGlowCutoff;     // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowCutoffScale;    // 0x10 [O]; [R] ctor default; [U] getter vtable slot 14 (func_002C14F8) returns this + 0x10
    float m_lastLightGlowCutoffScale; // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowCopyIntensity;  // 0x18 [O]; [R] ctor default; [U] getter vtable slot 15 (func_002C1500) returns this + 0x18
    float m_lastLightGlowCopyIntensity; // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowSourceIntensity; // 0x20 [O]; [R] ctor default; [U] getter vtable slot 16 (func_002C1508) returns this + 0x20
    float m_lastLightGlowSourceIntensity; // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowFrameIntensity; // 0x28 [O]; [R] ctor default; [U] getter vtable slot 17 (func_002C1510) returns this + 0x28
    float m_lastLightGlowFrameIntensity; // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowBlend2;         // 0x30 [O]; [R] ctor default; [U] getter vtable slot 18 (func_002C1518) returns this + 0x30
    float m_lastLightGlowBlend2;     // 0x34 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowBlend3;         // 0x38 [O]; [R] ctor default; [U] getter vtable slot 19 (func_002C1520) returns this + 0x38
    float m_lastLightGlowBlend3;     // 0x3C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_LightGlow, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_LightGlow, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowCutoff, 0x08);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowCutoff, 0x0C);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowCutoffScale, 0x10);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowCutoffScale, 0x14);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowCopyIntensity, 0x18);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowCopyIntensity, 0x1C);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowSourceIntensity, 0x20);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowSourceIntensity, 0x24);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowFrameIntensity, 0x28);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowFrameIntensity, 0x2C);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowBlend2, 0x30);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowBlend2, 0x34);
CHECK_OFFSET(tWPInGameData_LightGlow, m_LightGlowBlend3, 0x38);
CHECK_OFFSET(tWPInGameData_LightGlow, m_lastLightGlowBlend3, 0x3C);
CHECK_SIZE(tWPInGameData_LightGlow, 0x40);

struct tWPInGameData_ScreenTint {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] tWPInGameData_ScreenTint_tWPInGameData_ScreenTint stores -99999.0f
    void** mVtbl;                    // 0x04 [R] tWPInGameData_ScreenTint_tWPInGameData_ScreenTint stores the vtable D_00484B70
    // --- tWPInGameData_ScreenTint (alloc tag "tWPIGD_ScreenTint", 0x38 bytes)
    float m_ScreenTintScaleR;        // 0x08 [O]; [R] ctor default; [U] getter vtable slot 20 (func_002C1528) returns this + 0x8
    float m_lastScreenTintScaleR;    // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintScaleG;        // 0x10 [O]; [R] ctor default; [U] getter vtable slot 21 (func_002C1530) returns this + 0x10
    float m_lastScreenTintScaleG;    // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintScaleB;        // 0x18 [O]; [R] ctor default; [U] getter vtable slot 22 (func_002C1538) returns this + 0x18
    float m_lastScreenTintScaleB;    // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetR;       // 0x20 [O]; [R] ctor default; [U] getter vtable slot 23 (func_002C1540) returns this + 0x20
    float m_lastScreenTintOffsetR;   // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetG;       // 0x28 [O]; [R] ctor default; [U] getter vtable slot 24 (func_002C1548) returns this + 0x28
    float m_lastScreenTintOffsetG;   // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetB;       // 0x30 [O]; [R] ctor default; [U] getter vtable slot 25 (func_002C1550) returns this + 0x30
    float m_lastScreenTintOffsetB;   // 0x34 [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_ScreenTint, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_ScreenTint, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintScaleR, 0x08);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintScaleR, 0x0C);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintScaleG, 0x10);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintScaleG, 0x14);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintScaleB, 0x18);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintScaleB, 0x1C);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintOffsetR, 0x20);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintOffsetR, 0x24);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintOffsetG, 0x28);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintOffsetG, 0x2C);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_ScreenTintOffsetB, 0x30);
CHECK_OFFSET(tWPInGameData_ScreenTint, m_lastScreenTintOffsetB, 0x34);
CHECK_SIZE(tWPInGameData_ScreenTint, 0x38);

struct tWPInGameData_SkyBox {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC8E8 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC8E8 stores the vtable D_00484938
    // --- tWPInGameData_SkyBox (alloc tag "tWPIGD_SkyBox", 0x10 bytes)
    float m_SkyBoxRotation;          // 0x08 [O]; [R] ctor default; [U] getter vtable slot 26 (func_002C1558) returns this + 0x8
    float m_lastSkyBoxRotation;      // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_SkyBox, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_SkyBox, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_SkyBox, m_SkyBoxRotation, 0x08);
CHECK_OFFSET(tWPInGameData_SkyBox, m_lastSkyBoxRotation, 0x0C);
CHECK_SIZE(tWPInGameData_SkyBox, 0x10);

struct tWPInGameData_Sun {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] tWPInGameData_Sun_tWPInGameData_Sun stores -99999.0f
    void** mVtbl;                    // 0x04 [R] tWPInGameData_Sun_tWPInGameData_Sun stores the vtable D_00484700
    // --- tWPInGameData_Sun (alloc tag "tWPIGD_Sun", 0x50 bytes)
    float m_SunPitchOffset;          // 0x08 [O]; [R] ctor default; [U] getter vtable slot 27 (func_002C1560) returns this + 0x8
    float m_lastSunPitchOffset;      // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_SunYawOffset;            // 0x10 [O]; [R] ctor default; [U] getter vtable slot 28 (func_002C1568) returns this + 0x10
    float m_lastSunYawOffset;        // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorR;               // 0x18 [O]; [R] ctor default; [U] getter vtable slot 29 (func_002C1570) returns this + 0x18
    float m_lastSunColorR;           // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorG;               // 0x20 [O]; [R] ctor default; [U] getter vtable slot 30 (func_002C1578) returns this + 0x20
    float m_lastSunColorG;           // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorB;               // 0x28 [O]; [R] ctor default; [U] getter vtable slot 31 (func_002C1580) returns this + 0x28
    float m_lastSunColorB;           // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_SunIntensity;            // 0x30 [O]; [R] ctor default; [U] getter vtable slot 32 (func_002C1588) returns this + 0x30
    float m_lastSunIntensity;        // 0x34 [O]; [R] ctor default; [U] blend stores the target
    int m_SunType;                   // 0x38 [O]; [R] ctor default; [U] getter vtable slot 33 (func_002C1590) returns this + 0x38
    int m_lastSunType;               // 0x3C [O]; [R] ctor default; [U] blend stores the target
    float m_LensIntensity;           // 0x40 [O]; [R] ctor default; [U] getter vtable slot 34 (func_002C1598) returns this + 0x40
    float m_lastLensIntensity;       // 0x44 [O]; [R] ctor default; [U] blend stores the target
    float m_SunRadius;               // 0x48 [O]; [R] ctor default; [U] getter vtable slot 35 (func_002C15A0) returns this + 0x48
    float m_lastSunRadius;           // 0x4C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Sun, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Sun, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Sun, m_SunPitchOffset, 0x08);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunPitchOffset, 0x0C);
CHECK_OFFSET(tWPInGameData_Sun, m_SunYawOffset, 0x10);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunYawOffset, 0x14);
CHECK_OFFSET(tWPInGameData_Sun, m_SunColorR, 0x18);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunColorR, 0x1C);
CHECK_OFFSET(tWPInGameData_Sun, m_SunColorG, 0x20);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunColorG, 0x24);
CHECK_OFFSET(tWPInGameData_Sun, m_SunColorB, 0x28);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunColorB, 0x2C);
CHECK_OFFSET(tWPInGameData_Sun, m_SunIntensity, 0x30);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunIntensity, 0x34);
CHECK_OFFSET(tWPInGameData_Sun, m_SunType, 0x38);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunType, 0x3C);
CHECK_OFFSET(tWPInGameData_Sun, m_LensIntensity, 0x40);
CHECK_OFFSET(tWPInGameData_Sun, m_lastLensIntensity, 0x44);
CHECK_OFFSET(tWPInGameData_Sun, m_SunRadius, 0x48);
CHECK_OFFSET(tWPInGameData_Sun, m_lastSunRadius, 0x4C);
CHECK_SIZE(tWPInGameData_Sun, 0x50);

struct tWPInGameData_Surface {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC980 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC980 stores the vtable D_004844C8
    // --- tWPInGameData_Surface (alloc tag "tWPIGD_Surface", 0x10 bytes)
    float m_SnowSparkleDensity;      // 0x08 [O]; [R] ctor default; [U] getter vtable slot 36 (func_002C15A8) returns this + 0x8
    float m_lastSnowSparkleDensity;  // 0x0C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Surface, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Surface, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Surface, m_SnowSparkleDensity, 0x08);
CHECK_OFFSET(tWPInGameData_Surface, m_lastSnowSparkleDensity, 0x0C);
CHECK_SIZE(tWPInGameData_Surface, 0x10);

struct tWPInGameData_Lighting {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BC9B0 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BC9B0 stores the vtable D_00484290
    // --- tWPInGameData_Lighting (alloc tag "tWPIGD_Lighting", 0x58 bytes)
    tWPNameField m_LightIrradianceName; // 0x08 [O]; [R] ctor default; [U] getter vtable slot 37 (func_002C15B0) returns this + 0x8
    tWPNameField m_lastLightIrradianceName; // 0x10 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_ShadowIrradianceName; // 0x18 [O]; [R] ctor default; [U] getter vtable slot 38 (func_002C15B8) returns this + 0x18
    tWPNameField m_lastShadowIrradianceName; // 0x20 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_TunnelIrradianceName; // 0x28 [O]; [R] ctor default; [U] getter vtable slot 39 (func_002C15C0) returns this + 0x28
    tWPNameField m_lastTunnelIrradianceName; // 0x30 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_ObjectIrradianceName; // 0x38 [O]; [R] ctor default; [U] getter vtable slot 40 (func_002C15C8) returns this + 0x38
    tWPNameField m_lastObjectIrradianceName; // 0x40 [O]; [R] ctor default; [U] blend stores the target
    float m_AmbientLightIntensityOffset; // 0x48 [O]; [R] ctor default; [U] getter vtable slot 41 (func_002C15D0) returns this + 0x48
    float m_lastAmbientLightIntensityOffset; // 0x4C [O]; [R] ctor default; [U] blend stores the target
    float m_RimLightIntensityOffset; // 0x50 [O]; [R] ctor default; [U] getter vtable slot 42 (func_002C15D8) returns this + 0x50
    float m_lastRimLightIntensityOffset; // 0x54 [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Lighting, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Lighting, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Lighting, m_LightIrradianceName, 0x08);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastLightIrradianceName, 0x10);
CHECK_OFFSET(tWPInGameData_Lighting, m_ShadowIrradianceName, 0x18);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastShadowIrradianceName, 0x20);
CHECK_OFFSET(tWPInGameData_Lighting, m_TunnelIrradianceName, 0x28);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastTunnelIrradianceName, 0x30);
CHECK_OFFSET(tWPInGameData_Lighting, m_ObjectIrradianceName, 0x38);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastObjectIrradianceName, 0x40);
CHECK_OFFSET(tWPInGameData_Lighting, m_AmbientLightIntensityOffset, 0x48);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastAmbientLightIntensityOffset, 0x4C);
CHECK_OFFSET(tWPInGameData_Lighting, m_RimLightIntensityOffset, 0x50);
CHECK_OFFSET(tWPInGameData_Lighting, m_lastRimLightIntensityOffset, 0x54);
CHECK_SIZE(tWPInGameData_Lighting, 0x58);

struct tWPInGameData_Weather {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BCAF8 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BCAF8 stores the vtable D_00484058
    // --- tWPInGameData_Weather (alloc tag "tWPIGD_Weather", 0xA0 bytes)
    float m_SnowIntensity;           // 0x08 [O]; [R] ctor default; [U] getter vtable slot 43 (func_002C15E0) returns this + 0x8
    float m_lastSnowIntensity;       // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_FlakeSize;               // 0x10 [O]; [R] ctor default; [U] getter vtable slot 44 (func_002C15E8) returns this + 0x10
    float m_lastFlakeSize;           // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_WindIntensity;           // 0x18 [O]; [R] ctor default; [U] getter vtable slot 45 (func_002C15F0) returns this + 0x18
    float m_lastWindIntensity;       // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_WindRotation;            // 0x20 [O]; [R] ctor default; [U] getter vtable slot 46 (func_002C15F8) returns this + 0x20
    float m_lastWindRotation;        // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_PlayerWindIntensity;     // 0x28 [O]; [R] ctor default; [U] getter vtable slot 47 (func_002C1600) returns this + 0x28
    float m_lastPlayerWindIntensity; // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_Temperature;             // 0x30 [O]; [R] ctor default; [U] getter vtable slot 48 (func_002C1608) returns this + 0x30
    float m_lastTemperature;         // 0x34 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlurries;            // 0x38 [O]; [R] ctor default; [U] getter vtable slot 49 (func_002C1610) returns this + 0x38
    float m_lastSnowFlurries;        // 0x3C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowGravityMultiplier;   // 0x40 [O]; [R] ctor default; [U] getter vtable slot 50 (func_002C1618) returns this + 0x40
    float m_lastSnowGravityMultiplier; // 0x44 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffItensity;       // 0x48 [O]; [R] ctor default; [U] getter vtable slot 51 (func_002C1620) returns this + 0x48
    float m_lastSnowFluffItensity;   // 0x4C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffWindIntensity;  // 0x50 [O]; [R] ctor default; [U] getter vtable slot 52 (func_002C1628) returns this + 0x50
    float m_lastSnowFluffWindIntensity; // 0x54 [O]; [R] ctor default; [U] blend stores the target
    float m_LightningChance;         // 0x58 [O]; [R] ctor default; [U] getter vtable slot 53 (func_002C1630) returns this + 0x58
    float m_lastLightningChance;     // 0x5C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourA;        // 0x60 [O]; [R] ctor default; [U] getter vtable slot 54 (func_002C1638) returns this + 0x60
    float m_lastSnowFlakeColourA;    // 0x64 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourR;        // 0x68 [O]; [R] ctor default; [U] getter vtable slot 55 (func_002C1640) returns this + 0x68
    float m_lastSnowFlakeColourR;    // 0x6C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourG;        // 0x70 [O]; [R] ctor default; [U] getter vtable slot 56 (func_002C1648) returns this + 0x70
    float m_lastSnowFlakeColourG;    // 0x74 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourB;        // 0x78 [O]; [R] ctor default; [U] getter vtable slot 57 (func_002C1650) returns this + 0x78
    float m_lastSnowFlakeColourB;    // 0x7C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourA;        // 0x80 [O]; [R] ctor default; [U] getter vtable slot 58 (func_002C1658) returns this + 0x80
    float m_lastSnowFluffColourA;    // 0x84 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourR;        // 0x88 [O]; [R] ctor default; [U] getter vtable slot 59 (func_002C1660) returns this + 0x88
    float m_lastSnowFluffColourR;    // 0x8C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourG;        // 0x90 [O]; [R] ctor default; [U] getter vtable slot 60 (func_002C1668) returns this + 0x90
    float m_lastSnowFluffColourG;    // 0x94 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourB;        // 0x98 [O]; [R] ctor default; [U] getter vtable slot 61 (func_002C1670) returns this + 0x98
    float m_lastSnowFluffColourB;    // 0x9C [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Weather, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Weather, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowIntensity, 0x08);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowIntensity, 0x0C);
CHECK_OFFSET(tWPInGameData_Weather, m_FlakeSize, 0x10);
CHECK_OFFSET(tWPInGameData_Weather, m_lastFlakeSize, 0x14);
CHECK_OFFSET(tWPInGameData_Weather, m_WindIntensity, 0x18);
CHECK_OFFSET(tWPInGameData_Weather, m_lastWindIntensity, 0x1C);
CHECK_OFFSET(tWPInGameData_Weather, m_WindRotation, 0x20);
CHECK_OFFSET(tWPInGameData_Weather, m_lastWindRotation, 0x24);
CHECK_OFFSET(tWPInGameData_Weather, m_PlayerWindIntensity, 0x28);
CHECK_OFFSET(tWPInGameData_Weather, m_lastPlayerWindIntensity, 0x2C);
CHECK_OFFSET(tWPInGameData_Weather, m_Temperature, 0x30);
CHECK_OFFSET(tWPInGameData_Weather, m_lastTemperature, 0x34);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFlurries, 0x38);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFlurries, 0x3C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowGravityMultiplier, 0x40);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowGravityMultiplier, 0x44);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffItensity, 0x48);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffItensity, 0x4C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffWindIntensity, 0x50);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffWindIntensity, 0x54);
CHECK_OFFSET(tWPInGameData_Weather, m_LightningChance, 0x58);
CHECK_OFFSET(tWPInGameData_Weather, m_lastLightningChance, 0x5C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFlakeColourA, 0x60);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFlakeColourA, 0x64);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFlakeColourR, 0x68);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFlakeColourR, 0x6C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFlakeColourG, 0x70);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFlakeColourG, 0x74);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFlakeColourB, 0x78);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFlakeColourB, 0x7C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffColourA, 0x80);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffColourA, 0x84);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffColourR, 0x88);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffColourR, 0x8C);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffColourG, 0x90);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffColourG, 0x94);
CHECK_OFFSET(tWPInGameData_Weather, m_SnowFluffColourB, 0x98);
CHECK_OFFSET(tWPInGameData_Weather, m_lastSnowFluffColourB, 0x9C);
CHECK_SIZE(tWPInGameData_Weather, 0xA0);

struct tWPInGameData_Danger {
    // --- tWPInGameData
    float m_wpTravelDistance;        // 0x00 [O]; [R] func_002BCBD0 stores -99999.0f
    void** mVtbl;                    // 0x04 [R] func_002BCBD0 stores the vtable D_00483E20
    // --- tWPInGameData_Danger (alloc tag "tWPIGD_Danger", 0x18 bytes)
    int m_Level;                     // 0x08 [O]; [R] ctor default; [U] getter vtable slot 62 (func_002C1678) returns this + 0x8
    int m_lastLevel;                 // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_PointRate;               // 0x10 [O]; [R] ctor default; [U] getter vtable slot 63 (func_002C1680) returns this + 0x10
    float m_lastPointRate;           // 0x14 [O]; [R] ctor default; [U] blend stores the target
};

CHECK_OFFSET(tWPInGameData_Danger, m_wpTravelDistance, 0x00);
CHECK_OFFSET(tWPInGameData_Danger, mVtbl, 0x04);
CHECK_OFFSET(tWPInGameData_Danger, m_Level, 0x08);
CHECK_OFFSET(tWPInGameData_Danger, m_lastLevel, 0x0C);
CHECK_OFFSET(tWPInGameData_Danger, m_PointRate, 0x10);
CHECK_OFFSET(tWPInGameData_Danger, m_lastPointRate, 0x14);
CHECK_SIZE(tWPInGameData_Danger, 0x18);

// ---- The queries.
// On Tour: tWorldPainterQuery, 8 bytes (m_data, then the vptr of its virtual dtor / queryPosition).
struct tWorldPainterQuery {
    tWPInGameData* m_data;          // 0x0 [O]
    void** mVtbl;                   // 0x4 [R] tWorldPainterQuery__tWorldPainterQuery (dtor) stores D_00485D30
};

CHECK_OFFSET(tWorldPainterQuery, m_data, 0x0);
CHECK_OFFSET(tWorldPainterQuery, mVtbl, 0x4);
CHECK_SIZE(tWorldPainterQuery, 0x8);

struct cWorldPainterQuery {
    // --- tWorldPainterQuery
    tWPInGameData* m_data;          // 0x00 [O]; [R] ctor: the tWPInGameData_<X> for m_regionType.m_id; dtor deletes it;
                                    //       [U] queryPosition blends into it and returns it; reset writes its
                                    //       m_wpTravelDistance
    void** mVtbl;                   // 0x04 [R] ctor and dtor store cWorldPainterQuery_vtbl (D_00483E00); the dtor then
                                    //       the base's D_00485D30
    // --- cWorldPainterQuery
    float m_x;                      // 0x08 [O]; [R] ctor stores a sentinel; [U] queryPosition: the last fx (distance
                                    //       moved), then stores fx
    float m_y;                      // 0x0C [O]; [R] ctor like m_x; [U] queryPosition: the last fy
    tWorldPainterRegionType m_regionType; // 0x10 [O]; [R] ctor's argument (sb low byte, then the word with the id)
    cWorldPainterQuery* m_next;     // 0x14 [O]; [R] ctor links this in front of m_queryListStart; dtor unlinks;
                                    //       [U] reset walks the list
};

CHECK_OFFSET(cWorldPainterQuery, m_data, 0x00);
CHECK_OFFSET(cWorldPainterQuery, mVtbl, 0x04);
CHECK_OFFSET(cWorldPainterQuery, m_x, 0x08);
CHECK_OFFSET(cWorldPainterQuery, m_y, 0x0C);
CHECK_OFFSET(cWorldPainterQuery, m_regionType, 0x10);
CHECK_OFFSET(cWorldPainterQuery, m_next, 0x14);
CHECK_SIZE(cWorldPainterQuery, 0x18);

#endif /* WORLDPAINTER_WORLDPAINTERQUERY_H */
