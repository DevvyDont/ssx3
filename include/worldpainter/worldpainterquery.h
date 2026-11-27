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
//   0x8 (m_data + vptr; vtable tWorldPainterQuery_vtbl {dtor, pure queryPosition}), cWorldPainterQuery 0x18 (derives from
//   tWorldPainterQuery; vtable cWorldPainterQuery_vtbl = D_00483E00 {dtor cWorldPainterQuery__cWorldPainterQuery, queryPosition cWorldPainterQuery_queryPosition}).
// tWPInGameData's vtable (base D_00485D50, 69 slots): 1-63 the getters in On Tour's order (Type, Value, MixPreset,
// BankID, SpeechID, DrawDistance, FogDensity ... PointRate; each subclass overrides its own and they return
// this + offset), 64 writeToReplayFrame, 65 readFromReplayFrame (64/65 order from On Tour's declaration order,
// not verified), 66 blend(const tWPData*, float), 67 debugBlend, 68 isEqual(const tWPData*), 69 setDefaults; the
// base's 64-66/68/69 are pure (__pure_virtual), its getters return 0. No virtual destructor.
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
//   D_004A5358 worldpainterman, kNullNameField worldpainterquery; [O] On Tour globals; [R] the static initializers zero
//   it, the Lighting ctor/setDefaults copy it into the names);
//   kWPR_MusicTrigger..kWPR_Danger (static const tWorldPainterRegionType(n, n), n = 0..13, a copy per translation
//   unit, 8 bytes apart: D_004A52E8.. worldpainterlayer, D_004A5360.. worldpainterman, D_004A53D8.. worldpainterquery,
//   and the visualfx copy named kWPR_MusicTrigger..kWPR_Danger at 0x4A5540 (worldlightman's static initializer
//   func_002F6B88 fills it; lensfx's cRenderStateMan_init reads it); [O] On Tour globals kWPR_*).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructors (cWorldPainterQuery_cWorldPainterQuery, tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger ... tWPInGameData_Danger_tWPInGameData_Danger for the
//       tWPInGameData_<X>), destructors (cWorldPainterQuery__cWorldPainterQuery, tWorldPainterQuery__tWorldPainterQuery) and allocations;
//   [U] how retail code uses the field (getters, blend/isEqual, queryPosition cWorldPainterQuery_queryPosition);
//   [O] SSX On Tour's DWARF (all of these types): member names, order and, here, the same offsets (no bools or
//       vectors in these classes).

#include "layout.h"

struct cWorldPainterLayer;
struct cReplayFramePtr;
struct tWPRegion;

// Which painted resource a query reads: m_layer picks the section's layer slot, m_id must equal the region's
// m_iRegionType and picks the tWPInGameData_<X> (0 MusicTrigger, 1 Mix, 2 Ambience, 3 Speech, 4 Camera, 5 Fog,
// 6 LightGlow, 7 ScreenTint, 8 SkyBox, 9 Sun, 10 Surface, 11 Lighting, 12 Weather, 13 Danger). Bit fields: no
// CHECK_OFFSET; the word is at offset 0.
// The region type ids (tWorldPainterRegionType::m_id; names ours, after On Tour's constants kWPR_<X>). [O] On Tour's
// kWPR_MusicTrigger .. kWPR_Danger are tWorldPainterRegionType(n, n), n = 0..13 in this order; [U] the
// cWorldPainterQuery ctor creates the tWPInGameData_<X> of the same name for each id.
enum eWPRegionId {
    kWPRID_MusicTrigger = 0, kWPRID_Mix = 1, kWPRID_Ambience = 2, kWPRID_Speech = 3, kWPRID_Camera = 4,
    kWPRID_Fog = 5, kWPRID_LightGlow = 6, kWPRID_ScreenTint = 7, kWPRID_SkyBox = 8, kWPRID_Sun = 9,
    kWPRID_Surface = 10, kWPRID_Lighting = 11, kWPRID_Weather = 12, kWPRID_Danger = 13
};

struct tWorldPainterRegionType {
    unsigned int m_layer : 8;       // bits 0-7 [O]; [R] the ctor stores the low byte (sb); [U] queryPosition indexes
                                    //       the header's layer slots with it (lbu)
    unsigned int m_id : 24;         // bits 8-31 [O]; [R] the ctor switches on it (>> 8); [U] compared with m_iRegionType
#ifdef __cplusplus
    // [O] tWorldPainterRegionType(unsigned int layer, unsigned int id); [R] every unit's static init builds the 14
    // constants kWPR_MusicTrigger .. kWPR_Danger as (n, n), n = 0..13 (views sFlags_2C0B70, sSlot_18E8, sSlot_3E60).
    tWorldPainterRegionType(unsigned int layer, unsigned int id) { m_layer = layer; m_id = id; }
#endif
};

CHECK_SIZE(tWorldPainterRegionType, 0x4);

// A painted name (irradiance map names of tWPInGameData_Lighting). On Tour: one member m_nameId of an 8-byte union
// type (DWARF name lost); retail copies it with 8-byte moves and compares it as two words (tWPNameField_operatorNotEqual).
struct tWPNameField {
    // m_nameId[1] of a resolved field (name ours). [U] cRenderStateMan_resolveIrradianceName replaces the name in
    // m_nameId[0] with its id and stores this; cRenderStateMan_Irradiance resolves a field without it first.
    enum { kResolved = 0x123400 };
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
    // 0x4: the vptr (after the one data member, as g++ 2.95 places it). [R] the subclass ctors store their vtable;
    // base vtable D_00485D50. Slots: [R] the vtables (tools/.harness/classes/tWPInGameData/methods.txt maps each
    // entry), names and types [O] On Tour's tWPInGameData (the getters in its declaration order; it lost the pure
    // virtuals' parameters, which come from the subclasses' retail code). 64 / 65: the version in slot 64 calls the
    // frame's slot 1 (cReplayFramePtr_write), the one in slot 65 its slot 2 (read).
    virtual const int& Type() const;                                // 1
    virtual const int& Value() const;                               // 2
    virtual const int& MixPreset() const;                           // 3
    virtual const int& BankID() const;                              // 4
    virtual const int& SpeechID() const;                            // 5
    virtual const float& DrawDistance() const;                      // 6
    virtual const float& FogDensity() const;                        // 7
    virtual const float& FogNear() const;                           // 8
    virtual const float& FogFar() const;                            // 9
    virtual const float& FogColourR() const;                        // 10
    virtual const float& FogColourG() const;                        // 11
    virtual const float& FogColourB() const;                        // 12
    virtual const float& LightGlowCutoff() const;                   // 13
    virtual const float& LightGlowCutoffScale() const;              // 14
    virtual const float& LightGlowCopyIntensity() const;            // 15
    virtual const float& LightGlowSourceIntensity() const;          // 16
    virtual const float& LightGlowFrameIntensity() const;           // 17
    virtual const float& LightGlowBlend2() const;                   // 18
    virtual const float& LightGlowBlend3() const;                   // 19
    virtual const float& ScreenTintScaleR() const;                  // 20
    virtual const float& ScreenTintScaleG() const;                  // 21
    virtual const float& ScreenTintScaleB() const;                  // 22
    virtual const float& ScreenTintOffsetR() const;                 // 23
    virtual const float& ScreenTintOffsetG() const;                 // 24
    virtual const float& ScreenTintOffsetB() const;                 // 25
    virtual const float& SkyBoxRotation() const;                    // 26
    virtual const float& SunPitchOffset() const;                    // 27
    virtual const float& SunYawOffset() const;                      // 28
    virtual const float& SunColorR() const;                         // 29
    virtual const float& SunColorG() const;                         // 30
    virtual const float& SunColorB() const;                         // 31
    virtual const float& SunIntensity() const;                      // 32
    virtual const int& SunType() const;                             // 33
    virtual const float& LensIntensity() const;                     // 34
    virtual const float& SunRadius() const;                         // 35
    virtual const float& SnowSparkleDensity() const;                // 36
    virtual const tWPNameField & LightIrradianceName() const;       // 37
    virtual const tWPNameField & ShadowIrradianceName() const;      // 38
    virtual const tWPNameField & TunnelIrradianceName() const;      // 39
    virtual const tWPNameField & ObjectIrradianceName() const;      // 40
    virtual const float& AmbientLightIntensityOffset() const;       // 41
    virtual const float& RimLightIntensityOffset() const;           // 42
    virtual const float& SnowIntensity() const;                     // 43
    virtual const float& FlakeSize() const;                         // 44
    virtual const float& WindIntensity() const;                     // 45
    virtual const float& WindRotation() const;                      // 46
    virtual const float& PlayerWindIntensity() const;               // 47
    virtual const float& Temperature() const;                       // 48
    virtual const float& SnowFlurries() const;                      // 49
    virtual const float& SnowGravityMultiplier() const;             // 50
    virtual const float& SnowFluffItensity() const;                 // 51
    virtual const float& SnowFluffWindIntensity() const;            // 52
    virtual const float& LightningChance() const;                   // 53
    virtual const float& SnowFlakeColourA() const;                  // 54
    virtual const float& SnowFlakeColourR() const;                  // 55
    virtual const float& SnowFlakeColourG() const;                  // 56
    virtual const float& SnowFlakeColourB() const;                  // 57
    virtual const float& SnowFluffColourA() const;                  // 58
    virtual const float& SnowFluffColourR() const;                  // 59
    virtual const float& SnowFluffColourG() const;                  // 60
    virtual const float& SnowFluffColourB() const;                  // 61
    virtual const int& Level() const;                               // 62
    virtual const float& PointRate() const;                         // 63
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const = 0; // 64
    virtual void readFromReplayFrame(cReplayFramePtr* frame) = 0;   // 65
    virtual void blend(const tWPData* data, float blend) = 0;       // 66
    virtual void debugBlend() __asm__("tWPInGameData_debugBlend__FPv");                                      // 67
    virtual bool isEqual(const tWPData* data) = 0;                  // 68
    virtual void setDefaults() = 0;                                 // 69

    // Methods bound to their retail symbols by asm labels.
    void blend1(float* dst, float* src, float blend) __asm__("tWPInGameData_blend1");
    void* MusicTrigger_Type() __asm__("tWPInGameData_MusicTrigger_Type__FPv");
    void* MusicTrigger_Value() __asm__("tWPInGameData_MusicTrigger_Value__FPv");
    void* Mix_MixPreset() __asm__("tWPInGameData_Mix_MixPreset__FPv");
    void* Ambience_BankID() __asm__("tWPInGameData_Ambience_BankID__FPv");
    void* Speech_SpeechID() __asm__("tWPInGameData_Speech_SpeechID__FPv");
    void* Camera_DrawDistance() __asm__("tWPInGameData_Camera_DrawDistance__FPv");
    void* Fog_FogDensity() __asm__("tWPInGameData_Fog_FogDensity__FPv");
    void* Fog_FogNear() __asm__("tWPInGameData_Fog_FogNear__FPv");
    void* Fog_FogFar() __asm__("tWPInGameData_Fog_FogFar__FPv");
    void* Fog_FogColourR() __asm__("tWPInGameData_Fog_FogColourR__FPv");
    void* Fog_FogColourG() __asm__("tWPInGameData_Fog_FogColourG__FPv");
    void* Fog_FogColourB() __asm__("tWPInGameData_Fog_FogColourB__FPv");
    void* LightGlow_LightGlowCutoff() __asm__("tWPInGameData_LightGlow_LightGlowCutoff__FPv");
    void* LightGlow_LightGlowCutoffScale() __asm__("tWPInGameData_LightGlow_LightGlowCutoffScale__FPv");
    void* LightGlow_LightGlowCopyIntensity() __asm__("tWPInGameData_LightGlow_LightGlowCopyIntensity__FPv");
    void* LightGlow_LightGlowSourceIntensity() __asm__("tWPInGameData_LightGlow_LightGlowSourceIntensity__FPv");
    void* LightGlow_LightGlowFrameIntensity() __asm__("tWPInGameData_LightGlow_LightGlowFrameIntensity__FPv");
    void* LightGlow_LightGlowBlend2() __asm__("tWPInGameData_LightGlow_LightGlowBlend2__FPv");
    void* LightGlow_LightGlowBlend3() __asm__("tWPInGameData_LightGlow_LightGlowBlend3__FPv");
    void* ScreenTint_ScreenTintScaleR() __asm__("tWPInGameData_ScreenTint_ScreenTintScaleR__FPv");
    void* ScreenTint_ScreenTintScaleG() __asm__("tWPInGameData_ScreenTint_ScreenTintScaleG__FPv");
    void* ScreenTint_ScreenTintScaleB() __asm__("tWPInGameData_ScreenTint_ScreenTintScaleB__FPv");
    void* ScreenTint_ScreenTintOffsetR() __asm__("tWPInGameData_ScreenTint_ScreenTintOffsetR__FPv");
    void* ScreenTint_ScreenTintOffsetG() __asm__("tWPInGameData_ScreenTint_ScreenTintOffsetG__FPv");
    void* ScreenTint_ScreenTintOffsetB() __asm__("tWPInGameData_ScreenTint_ScreenTintOffsetB__FPv");
    void* SkyBox_SkyBoxRotation() __asm__("tWPInGameData_SkyBox_SkyBoxRotation__FPv");
    void* Sun_SunPitchOffset() __asm__("tWPInGameData_Sun_SunPitchOffset__FPv");
    void* Sun_SunYawOffset() __asm__("tWPInGameData_Sun_SunYawOffset__FPv");
    void* Sun_SunColorR() __asm__("tWPInGameData_Sun_SunColorR__FPv");
    void* Sun_SunColorG() __asm__("tWPInGameData_Sun_SunColorG__FPv");
    void* Sun_SunColorB() __asm__("tWPInGameData_Sun_SunColorB__FPv");
    void* Sun_SunIntensity() __asm__("tWPInGameData_Sun_SunIntensity__FPv");
    void* Sun_SunType() __asm__("tWPInGameData_Sun_SunType__FPv");
    void* Sun_LensIntensity() __asm__("tWPInGameData_Sun_LensIntensity__FPv");
    void* Sun_SunRadius() __asm__("tWPInGameData_Sun_SunRadius__FPv");
    void* Surface_SnowSparkleDensity() __asm__("tWPInGameData_Surface_SnowSparkleDensity__FPv");
    void* Lighting_LightIrradianceName() __asm__("tWPInGameData_Lighting_LightIrradianceName__FPv");
    void* Lighting_ShadowIrradianceName() __asm__("tWPInGameData_Lighting_ShadowIrradianceName__FPv");
    void* Lighting_TunnelIrradianceName() __asm__("tWPInGameData_Lighting_TunnelIrradianceName__FPv");
    void* Lighting_ObjectIrradianceName() __asm__("tWPInGameData_Lighting_ObjectIrradianceName__FPv");
    void* Lighting_AmbientLightIntensityOffset() __asm__("tWPInGameData_Lighting_AmbientLightIntensityOffset__FPv");
    void* Lighting_RimLightIntensityOffset() __asm__("tWPInGameData_Lighting_RimLightIntensityOffset__FPv");
    void* Weather_SnowIntensity() __asm__("tWPInGameData_Weather_SnowIntensity__FPv");
    void* Weather_FlakeSize() __asm__("tWPInGameData_Weather_FlakeSize__FPv");
    void* Weather_WindIntensity() __asm__("tWPInGameData_Weather_WindIntensity__FPv");
    void* Weather_WindRotation() __asm__("tWPInGameData_Weather_WindRotation__FPv");
    void* Weather_PlayerWindIntensity() __asm__("tWPInGameData_Weather_PlayerWindIntensity__FPv");
    void* Weather_Temperature() __asm__("tWPInGameData_Weather_Temperature__FPv");
    void* Weather_SnowFlurries() __asm__("tWPInGameData_Weather_SnowFlurries__FPv");
    void* Weather_SnowGravityMultiplier() __asm__("tWPInGameData_Weather_SnowGravityMultiplier__FPv");
    void* Weather_SnowFluffItensity() __asm__("tWPInGameData_Weather_SnowFluffItensity__FPv");
    void* Weather_SnowFluffWindIntensity() __asm__("tWPInGameData_Weather_SnowFluffWindIntensity__FPv");
    void* Weather_LightningChance() __asm__("tWPInGameData_Weather_LightningChance__FPv");
    void* Weather_SnowFlakeColourA() __asm__("tWPInGameData_Weather_SnowFlakeColourA__FPv");
    void* Weather_SnowFlakeColourR() __asm__("tWPInGameData_Weather_SnowFlakeColourR__FPv");
    void* Weather_SnowFlakeColourG() __asm__("tWPInGameData_Weather_SnowFlakeColourG__FPv");
    void* Weather_SnowFlakeColourB() __asm__("tWPInGameData_Weather_SnowFlakeColourB__FPv");
    void* Weather_SnowFluffColourA() __asm__("tWPInGameData_Weather_SnowFluffColourA__FPv");
    void* Weather_SnowFluffColourR() __asm__("tWPInGameData_Weather_SnowFluffColourR__FPv");
    void* Weather_SnowFluffColourG() __asm__("tWPInGameData_Weather_SnowFluffColourG__FPv");
    void* Weather_SnowFluffColourB() __asm__("tWPInGameData_Weather_SnowFluffColourB__FPv");
    void* Danger_Level() __asm__("tWPInGameData_Danger_Level__FPv");
    void* Danger_PointRate() __asm__("tWPInGameData_Danger_PointRate__FPv");
};

CHECK_OFFSET(tWPInGameData, m_wpTravelDistance, 0x0);
CHECK_SIZE(tWPInGameData, 0x8);

// ---- The painted data of one region (the file side; tWPRegion::m_pData points at one). On Tour: tWPData_<X>, a
// tWPData (m_blendData) followed by the values; [U] each subclass's blend (vtable slot 66) and isEqual (slot 68)
// read exactly these offsets (ints through lw, or lwc1 for the int->float blend).
struct tWPData_MusicTrigger {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_Type;                      // 0x04 [O]; [U] tWPInGameData_MusicTrigger_blend / tWPInGameData_MusicTrigger_isEqual
    int m_Value;                     // 0x08 [O]; [U] tWPInGameData_MusicTrigger_blend / tWPInGameData_MusicTrigger_isEqual
};

CHECK_OFFSET(tWPData_MusicTrigger, m_blendData, 0x00);
CHECK_OFFSET(tWPData_MusicTrigger, m_Type, 0x04);
CHECK_OFFSET(tWPData_MusicTrigger, m_Value, 0x08);
CHECK_SIZE(tWPData_MusicTrigger, 0x0C);

struct tWPData_Mix {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_MixPreset;                 // 0x04 [O]; [U] tWPInGameData_Mix_blend / tWPInGameData_Mix_isEqual
};

CHECK_OFFSET(tWPData_Mix, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Mix, m_MixPreset, 0x04);
CHECK_SIZE(tWPData_Mix, 0x08);

struct tWPData_Ambience {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_BankID;                    // 0x04 [O]; [U] tWPInGameData_Ambience_blend / tWPInGameData_Ambience_isEqual
};

CHECK_OFFSET(tWPData_Ambience, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Ambience, m_BankID, 0x04);
CHECK_SIZE(tWPData_Ambience, 0x08);

struct tWPData_Speech {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    int m_SpeechID;                  // 0x04 [O]; [U] tWPInGameData_Speech_blend / tWPInGameData_Speech_isEqual
};

CHECK_OFFSET(tWPData_Speech, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Speech, m_SpeechID, 0x04);
CHECK_SIZE(tWPData_Speech, 0x08);

struct tWPData_Camera {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_DrawDistance;            // 0x04 [O]; [U] tWPInGameData_Camera_blend / tWPInGameData_Camera_isEqual
};

CHECK_OFFSET(tWPData_Camera, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Camera, m_DrawDistance, 0x04);
CHECK_SIZE(tWPData_Camera, 0x08);

struct tWPData_Fog {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_FogDensity;              // 0x04 [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
    float m_FogNear;                 // 0x08 [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
    float m_FogFar;                  // 0x0C [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
    float m_FogColourR;              // 0x10 [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
    float m_FogColourG;              // 0x14 [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
    float m_FogColourB;              // 0x18 [O]; [U] tWPInGameData_Fog_blend / tWPInGameData_Fog_isEqual
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
    float m_LightGlowCutoff;         // 0x04 [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowCutoffScale;    // 0x08 [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowCopyIntensity;  // 0x0C [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowSourceIntensity; // 0x10 [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowFrameIntensity; // 0x14 [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowBlend2;         // 0x18 [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
    float m_LightGlowBlend3;         // 0x1C [O]; [U] tWPInGameData_LightGlow_blend / tWPInGameData_LightGlow_isEqual
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
    float m_ScreenTintScaleR;        // 0x04 [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
    float m_ScreenTintScaleG;        // 0x08 [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
    float m_ScreenTintScaleB;        // 0x0C [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
    float m_ScreenTintOffsetR;       // 0x10 [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
    float m_ScreenTintOffsetG;       // 0x14 [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
    float m_ScreenTintOffsetB;       // 0x18 [O]; [U] tWPInGameData_ScreenTint_blend / tWPInGameData_ScreenTint_isEqual
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
    float m_SkyBoxRotation;          // 0x04 [O]; [U] tWPInGameData_SkyBox_blend / tWPInGameData_SkyBox_isEqual
};

CHECK_OFFSET(tWPData_SkyBox, m_blendData, 0x00);
CHECK_OFFSET(tWPData_SkyBox, m_SkyBoxRotation, 0x04);
CHECK_SIZE(tWPData_SkyBox, 0x08);

struct tWPData_Sun {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    float m_SunPitchOffset;          // 0x04 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunYawOffset;            // 0x08 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunColorR;               // 0x0C [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunColorG;               // 0x10 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunColorB;               // 0x14 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunIntensity;            // 0x18 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    int m_SunType;                   // 0x1C [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_LensIntensity;           // 0x20 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
    float m_SunRadius;               // 0x24 [O]; [U] tWPInGameData_Sun_blend / tWPInGameData_Sun_isEqual
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
    float m_SnowSparkleDensity;      // 0x04 [O]; [U] tWPInGameData_Surface_blend / tWPInGameData_Surface_isEqual
};

CHECK_OFFSET(tWPData_Surface, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Surface, m_SnowSparkleDensity, 0x04);
CHECK_SIZE(tWPData_Surface, 0x08);

struct tWPData_Lighting {
    float m_blendData;               // 0x00 [O] tWPData::m_blendData (no retail reader seen)
    tWPNameField m_LightIrradianceName; // 0x04 [O]; [U] tWPInGameData_Lighting_blend copies it (ldl/ldr)
    tWPNameField m_ShadowIrradianceName; // 0x0C [O]; [U] tWPInGameData_Lighting_blend copies it (ldl/ldr)
    tWPNameField m_TunnelIrradianceName; // 0x14 [O]; [U] tWPInGameData_Lighting_blend copies it (ldl/ldr)
    tWPNameField m_ObjectIrradianceName; // 0x1C [O]; [U] tWPInGameData_Lighting_blend copies it (ldl/ldr)
    float m_AmbientLightIntensityOffset; // 0x24 [O]; [U] tWPInGameData_Lighting_blend / tWPInGameData_Lighting_isEqual
    float m_RimLightIntensityOffset; // 0x28 [O]; [U] tWPInGameData_Lighting_blend / tWPInGameData_Lighting_isEqual
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
    float m_SnowIntensity;           // 0x04 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_FlakeSize;               // 0x08 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_WindIntensity;           // 0x0C [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_WindRotation;            // 0x10 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_PlayerWindIntensity;     // 0x14 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_Temperature;             // 0x18 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFlurries;            // 0x1C [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowGravityMultiplier;   // 0x20 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffItensity;       // 0x24 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffWindIntensity;  // 0x28 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_LightningChance;         // 0x2C [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFlakeColourA;        // 0x30 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFlakeColourR;        // 0x34 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFlakeColourG;        // 0x38 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFlakeColourB;        // 0x3C [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffColourA;        // 0x40 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffColourR;        // 0x44 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffColourG;        // 0x48 [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
    float m_SnowFluffColourB;        // 0x4C [O]; [U] tWPInGameData_Weather_blend / tWPInGameData_Weather_isEqual
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
    int m_Level;                     // 0x04 [O]; [U] tWPInGameData_Danger_blend / tWPInGameData_Danger_isEqual
    float m_PointRate;               // 0x08 [O]; [U] tWPInGameData_Danger_blend / tWPInGameData_Danger_isEqual
};

CHECK_OFFSET(tWPData_Danger, m_blendData, 0x00);
CHECK_OFFSET(tWPData_Danger, m_Level, 0x04);
CHECK_OFFSET(tWPData_Danger, m_PointRate, 0x08);
CHECK_SIZE(tWPData_Danger, 0x0C);

// ---- The blended in-game values, one subclass per region type (tWorldPainterQuery::m_data points at one).
// Each value has a pair: m_<X> (the blended value the getter returns) and m_last<X> (the last target blended
// towards; blend stores the rhs value there). Sizes [R] from cWorldPainterQuery's cMemMan_alloc(size, "tWPIGD_<X>")
// (= On Tour sizes); the constructor stores the vtable at 0x4, -99999.0f into m_wpTravelDistance and the defaults.
struct tWPInGameData_MusicTrigger : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger stores the vtable tWPInGameData_MusicTrigger_vtbl
    // --- tWPInGameData_MusicTrigger (alloc tag "tWPIGD_MusicTrigger", 0x18 bytes)
    int m_Type;                      // 0x08 [O]; [R] ctor default; [U] getter vtable slot 1 (tWPInGameData_MusicTrigger_Type) returns this + 0x8
    int m_lastType;                  // 0x0C [O]; [R] ctor default; [U] blend stores the target
    int m_Value;                     // 0x10 [O]; [R] ctor default; [U] getter vtable slot 2 (tWPInGameData_MusicTrigger_Value) returns this + 0x10
    int m_lastValue;                 // 0x14 [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_MusicTrigger_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_MusicTrigger_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_MusicTrigger_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const
        __asm__("tWPInGameData_MusicTrigger_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_MusicTrigger_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_MusicTrigger_isEqual");
};

CHECK_OFFSET(tWPInGameData_MusicTrigger, m_Type, 0x08);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_lastType, 0x0C);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_Value, 0x10);
CHECK_OFFSET(tWPInGameData_MusicTrigger, m_lastValue, 0x14);
CHECK_SIZE(tWPInGameData_MusicTrigger, 0x18);

struct tWPInGameData_Mix : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Mix_tWPInGameData_Mix stores the vtable tWPInGameData_Mix_vtbl
    // --- tWPInGameData_Mix (alloc tag "tWPIGD_Mix", 0x10 bytes)
    int m_MixPreset;                 // 0x08 [O]; [R] ctor default; [U] getter vtable slot 3 (tWPInGameData_Mix_MixPreset) returns this + 0x8
    int m_lastMixPreset;             // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Mix_setDefaults): declared first, it is the vtable's key function,
    // which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Mix_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Mix_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Mix_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Mix_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Mix_isEqual");
};

CHECK_OFFSET(tWPInGameData_Mix, m_MixPreset, 0x08);
CHECK_OFFSET(tWPInGameData_Mix, m_lastMixPreset, 0x0C);
CHECK_SIZE(tWPInGameData_Mix, 0x10);

struct tWPInGameData_Ambience : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Ambience_tWPInGameData_Ambience stores the vtable tWPInGameData_Ambience_vtbl
    // --- tWPInGameData_Ambience (alloc tag "tWPIGD_Ambience", 0x10 bytes)
    int m_BankID;                    // 0x08 [O]; [R] ctor default; [U] getter vtable slot 4 (tWPInGameData_Ambience_BankID) returns this + 0x8
    int m_lastBankID;                // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Ambience_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Ambience_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Ambience_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Ambience_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Ambience_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Ambience_isEqual");
};

CHECK_OFFSET(tWPInGameData_Ambience, m_BankID, 0x08);
CHECK_OFFSET(tWPInGameData_Ambience, m_lastBankID, 0x0C);
CHECK_SIZE(tWPInGameData_Ambience, 0x10);

struct tWPInGameData_Speech : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Speech_tWPInGameData_Speech stores the vtable tWPInGameData_Speech_vtbl
    // --- tWPInGameData_Speech (alloc tag "tWPIGD_Speech", 0x10 bytes)
    int m_SpeechID;                  // 0x08 [O]; [R] ctor default; [U] getter vtable slot 5 (tWPInGameData_Speech_SpeechID) returns this + 0x8
    int m_lastSpeechID;              // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Speech_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Speech_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Speech_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Speech_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Speech_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Speech_isEqual");
};

CHECK_OFFSET(tWPInGameData_Speech, m_SpeechID, 0x08);
CHECK_OFFSET(tWPInGameData_Speech, m_lastSpeechID, 0x0C);
CHECK_SIZE(tWPInGameData_Speech, 0x10);

struct tWPInGameData_Camera : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Camera_tWPInGameData_Camera stores the vtable tWPInGameData_Camera_vtbl
    // --- tWPInGameData_Camera (alloc tag "tWPIGD_Camera", 0x10 bytes)
    float m_DrawDistance;            // 0x08 [O]; [R] ctor default; [U] getter vtable slot 6 (tWPInGameData_Camera_DrawDistance) returns this + 0x8
    float m_lastDrawDistance;        // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Camera_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Camera_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Camera_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Camera_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Camera_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Camera_isEqual");
};

CHECK_OFFSET(tWPInGameData_Camera, m_DrawDistance, 0x08);
CHECK_OFFSET(tWPInGameData_Camera, m_lastDrawDistance, 0x0C);
CHECK_SIZE(tWPInGameData_Camera, 0x10);

struct tWPInGameData_Fog : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Fog_tWPInGameData_Fog stores the vtable tWPInGameData_Fog_vtbl
    // --- tWPInGameData_Fog (alloc tag "tWPIGD_Fog", 0x38 bytes)
    float m_FogDensity;              // 0x08 [O]; [R] ctor default; [U] getter vtable slot 7 (tWPInGameData_Fog_FogDensity) returns this + 0x8
    float m_lastFogDensity;          // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_FogNear;                 // 0x10 [O]; [R] ctor default; [U] getter vtable slot 8 (tWPInGameData_Fog_FogNear) returns this + 0x10
    float m_lastFogNear;             // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_FogFar;                  // 0x18 [O]; [R] ctor default; [U] getter vtable slot 9 (tWPInGameData_Fog_FogFar) returns this + 0x18
    float m_lastFogFar;              // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourR;              // 0x20 [O]; [R] ctor default; [U] getter vtable slot 10 (tWPInGameData_Fog_FogColourR) returns this + 0x20
    float m_lastFogColourR;          // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourG;              // 0x28 [O]; [R] ctor default; [U] getter vtable slot 11 (tWPInGameData_Fog_FogColourG) returns this + 0x28
    float m_lastFogColourG;          // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_FogColourB;              // 0x30 [O]; [R] ctor default; [U] getter vtable slot 12 (tWPInGameData_Fog_FogColourB) returns this + 0x30
    float m_lastFogColourB;          // 0x34 [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Fog_setDefaults): declared first, it is the vtable's key function,
    // which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Fog_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Fog_readFromReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Fog_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Fog_isEqual");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Fog_writeToReplayFrame");
};

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

struct tWPInGameData_LightGlow : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_LightGlow_tWPInGameData_LightGlow stores the vtable tWPInGameData_LightGlow_vtbl
    // --- tWPInGameData_LightGlow (alloc tag "tWPIGD_LightGlow", 0x40 bytes)
    float m_LightGlowCutoff;         // 0x08 [O]; [R] ctor default; [U] getter vtable slot 13 (tWPInGameData_LightGlow_LightGlowCutoff) returns this + 0x8
    float m_lastLightGlowCutoff;     // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowCutoffScale;    // 0x10 [O]; [R] ctor default; [U] getter vtable slot 14 (tWPInGameData_LightGlow_LightGlowCutoffScale) returns this + 0x10
    float m_lastLightGlowCutoffScale; // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowCopyIntensity;  // 0x18 [O]; [R] ctor default; [U] getter vtable slot 15 (tWPInGameData_LightGlow_LightGlowCopyIntensity) returns this + 0x18
    float m_lastLightGlowCopyIntensity; // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowSourceIntensity; // 0x20 [O]; [R] ctor default; [U] getter vtable slot 16 (tWPInGameData_LightGlow_LightGlowSourceIntensity) returns this + 0x20
    float m_lastLightGlowSourceIntensity; // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowFrameIntensity; // 0x28 [O]; [R] ctor default; [U] getter vtable slot 17 (tWPInGameData_LightGlow_LightGlowFrameIntensity) returns this + 0x28
    float m_lastLightGlowFrameIntensity; // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowBlend2;         // 0x30 [O]; [R] ctor default; [U] getter vtable slot 18 (tWPInGameData_LightGlow_LightGlowBlend2) returns this + 0x30
    float m_lastLightGlowBlend2;     // 0x34 [O]; [R] ctor default; [U] blend stores the target
    float m_LightGlowBlend3;         // 0x38 [O]; [R] ctor default; [U] getter vtable slot 19 (tWPInGameData_LightGlow_LightGlowBlend3) returns this + 0x38
    float m_lastLightGlowBlend3;     // 0x3C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_LightGlow_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_LightGlow_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_LightGlow_readFromReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_LightGlow_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_LightGlow_isEqual");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_LightGlow_writeToReplayFrame");
};

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

struct tWPInGameData_ScreenTint : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_ScreenTint_tWPInGameData_ScreenTint stores the vtable tWPInGameData_ScreenTint_vtbl
    // --- tWPInGameData_ScreenTint (alloc tag "tWPIGD_ScreenTint", 0x38 bytes)
    float m_ScreenTintScaleR;        // 0x08 [O]; [R] ctor default; [U] getter vtable slot 20 (tWPInGameData_ScreenTint_ScreenTintScaleR) returns this + 0x8
    float m_lastScreenTintScaleR;    // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintScaleG;        // 0x10 [O]; [R] ctor default; [U] getter vtable slot 21 (tWPInGameData_ScreenTint_ScreenTintScaleG) returns this + 0x10
    float m_lastScreenTintScaleG;    // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintScaleB;        // 0x18 [O]; [R] ctor default; [U] getter vtable slot 22 (tWPInGameData_ScreenTint_ScreenTintScaleB) returns this + 0x18
    float m_lastScreenTintScaleB;    // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetR;       // 0x20 [O]; [R] ctor default; [U] getter vtable slot 23 (tWPInGameData_ScreenTint_ScreenTintOffsetR) returns this + 0x20
    float m_lastScreenTintOffsetR;   // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetG;       // 0x28 [O]; [R] ctor default; [U] getter vtable slot 24 (tWPInGameData_ScreenTint_ScreenTintOffsetG) returns this + 0x28
    float m_lastScreenTintOffsetG;   // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_ScreenTintOffsetB;       // 0x30 [O]; [R] ctor default; [U] getter vtable slot 25 (tWPInGameData_ScreenTint_ScreenTintOffsetB) returns this + 0x30
    float m_lastScreenTintOffsetB;   // 0x34 [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_ScreenTint_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_ScreenTint_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_ScreenTint_readFromReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_ScreenTint_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_ScreenTint_isEqual");
};

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

struct tWPInGameData_SkyBox : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_SkyBox_tWPInGameData_SkyBox stores the vtable tWPInGameData_SkyBox_vtbl
    // --- tWPInGameData_SkyBox (alloc tag "tWPIGD_SkyBox", 0x10 bytes)
    float m_SkyBoxRotation;          // 0x08 [O]; [R] ctor default; [U] getter vtable slot 26 (tWPInGameData_SkyBox_SkyBoxRotation) returns this + 0x8
    float m_lastSkyBoxRotation;      // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_SkyBox_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_SkyBox_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_SkyBox_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_SkyBox_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_SkyBox_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_SkyBox_isEqual");
};

CHECK_OFFSET(tWPInGameData_SkyBox, m_SkyBoxRotation, 0x08);
CHECK_OFFSET(tWPInGameData_SkyBox, m_lastSkyBoxRotation, 0x0C);
CHECK_SIZE(tWPInGameData_SkyBox, 0x10);

struct tWPInGameData_Sun : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Sun_tWPInGameData_Sun stores the vtable tWPInGameData_Sun_vtbl
    // --- tWPInGameData_Sun (alloc tag "tWPIGD_Sun", 0x50 bytes)
    float m_SunPitchOffset;          // 0x08 [O]; [R] ctor default; [U] getter vtable slot 27 (tWPInGameData_Sun_SunPitchOffset) returns this + 0x8
    float m_lastSunPitchOffset;      // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_SunYawOffset;            // 0x10 [O]; [R] ctor default; [U] getter vtable slot 28 (tWPInGameData_Sun_SunYawOffset) returns this + 0x10
    float m_lastSunYawOffset;        // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorR;               // 0x18 [O]; [R] ctor default; [U] getter vtable slot 29 (tWPInGameData_Sun_SunColorR) returns this + 0x18
    float m_lastSunColorR;           // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorG;               // 0x20 [O]; [R] ctor default; [U] getter vtable slot 30 (tWPInGameData_Sun_SunColorG) returns this + 0x20
    float m_lastSunColorG;           // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_SunColorB;               // 0x28 [O]; [R] ctor default; [U] getter vtable slot 31 (tWPInGameData_Sun_SunColorB) returns this + 0x28
    float m_lastSunColorB;           // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_SunIntensity;            // 0x30 [O]; [R] ctor default; [U] getter vtable slot 32 (tWPInGameData_Sun_SunIntensity) returns this + 0x30
    float m_lastSunIntensity;        // 0x34 [O]; [R] ctor default; [U] blend stores the target
    int m_SunType;                   // 0x38 [O]; [R] ctor default; [U] getter vtable slot 33 (tWPInGameData_Sun_SunType) returns this + 0x38
    int m_lastSunType;               // 0x3C [O]; [R] ctor default; [U] blend stores the target
    float m_LensIntensity;           // 0x40 [O]; [R] ctor default; [U] getter vtable slot 34 (tWPInGameData_Sun_LensIntensity) returns this + 0x40
    float m_lastLensIntensity;       // 0x44 [O]; [R] ctor default; [U] blend stores the target
    float m_SunRadius;               // 0x48 [O]; [R] ctor default; [U] getter vtable slot 35 (tWPInGameData_Sun_SunRadius) returns this + 0x48
    float m_lastSunRadius;           // 0x4C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Sun_setDefaults): declared first, it is the vtable's key function,
    // which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Sun_setDefaults");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Sun_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Sun_isEqual");
    virtual void readFromReplayFrame(cReplayFramePtr* baseFrame) __asm__("tWPInGameData_Sun_readFromReplayFrame");
};

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

struct tWPInGameData_Surface : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Surface_tWPInGameData_Surface stores the vtable tWPInGameData_Surface_vtbl
    // --- tWPInGameData_Surface (alloc tag "tWPIGD_Surface", 0x10 bytes)
    float m_SnowSparkleDensity;      // 0x08 [O]; [R] ctor default; [U] getter vtable slot 36 (tWPInGameData_Surface_SnowSparkleDensity) returns this + 0x8
    float m_lastSnowSparkleDensity;  // 0x0C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Surface_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Surface_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Surface_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Surface_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Surface_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Surface_isEqual");
};

CHECK_OFFSET(tWPInGameData_Surface, m_SnowSparkleDensity, 0x08);
CHECK_OFFSET(tWPInGameData_Surface, m_lastSnowSparkleDensity, 0x0C);
CHECK_SIZE(tWPInGameData_Surface, 0x10);

struct tWPInGameData_Lighting : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Lighting_tWPInGameData_Lighting stores the vtable tWPInGameData_Lighting_vtbl
    // --- tWPInGameData_Lighting (alloc tag "tWPIGD_Lighting", 0x58 bytes)
    tWPNameField m_LightIrradianceName; // 0x08 [O]; [R] ctor default; [U] getter vtable slot 37 (tWPInGameData_Lighting_LightIrradianceName) returns this + 0x8
    tWPNameField m_lastLightIrradianceName; // 0x10 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_ShadowIrradianceName; // 0x18 [O]; [R] ctor default; [U] getter vtable slot 38 (tWPInGameData_Lighting_ShadowIrradianceName) returns this + 0x18
    tWPNameField m_lastShadowIrradianceName; // 0x20 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_TunnelIrradianceName; // 0x28 [O]; [R] ctor default; [U] getter vtable slot 39 (tWPInGameData_Lighting_TunnelIrradianceName) returns this + 0x28
    tWPNameField m_lastTunnelIrradianceName; // 0x30 [O]; [R] ctor default; [U] blend stores the target
    tWPNameField m_ObjectIrradianceName; // 0x38 [O]; [R] ctor default; [U] getter vtable slot 40 (tWPInGameData_Lighting_ObjectIrradianceName) returns this + 0x38
    tWPNameField m_lastObjectIrradianceName; // 0x40 [O]; [R] ctor default; [U] blend stores the target
    float m_AmbientLightIntensityOffset; // 0x48 [O]; [R] ctor default; [U] getter vtable slot 41 (tWPInGameData_Lighting_AmbientLightIntensityOffset) returns this + 0x48
    float m_lastAmbientLightIntensityOffset; // 0x4C [O]; [R] ctor default; [U] blend stores the target
    float m_RimLightIntensityOffset; // 0x50 [O]; [R] ctor default; [U] getter vtable slot 42 (tWPInGameData_Lighting_RimLightIntensityOffset) returns this + 0x50
    float m_lastRimLightIntensityOffset; // 0x54 [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Lighting_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Lighting_setDefaults");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Lighting_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Lighting_isEqual");
    virtual void readFromReplayFrame(cReplayFramePtr* baseFrame) __asm__("tWPInGameData_Lighting_readFromReplayFrame");
};

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

struct tWPInGameData_Weather : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Weather_tWPInGameData_Weather stores the vtable tWPInGameData_Weather_vtbl
    // --- tWPInGameData_Weather (alloc tag "tWPIGD_Weather", 0xA0 bytes)
    float m_SnowIntensity;           // 0x08 [O]; [R] ctor default; [U] getter vtable slot 43 (tWPInGameData_Weather_SnowIntensity) returns this + 0x8
    float m_lastSnowIntensity;       // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_FlakeSize;               // 0x10 [O]; [R] ctor default; [U] getter vtable slot 44 (tWPInGameData_Weather_FlakeSize) returns this + 0x10
    float m_lastFlakeSize;           // 0x14 [O]; [R] ctor default; [U] blend stores the target
    float m_WindIntensity;           // 0x18 [O]; [R] ctor default; [U] getter vtable slot 45 (tWPInGameData_Weather_WindIntensity) returns this + 0x18
    float m_lastWindIntensity;       // 0x1C [O]; [R] ctor default; [U] blend stores the target
    float m_WindRotation;            // 0x20 [O]; [R] ctor default; [U] getter vtable slot 46 (tWPInGameData_Weather_WindRotation) returns this + 0x20
    float m_lastWindRotation;        // 0x24 [O]; [R] ctor default; [U] blend stores the target
    float m_PlayerWindIntensity;     // 0x28 [O]; [R] ctor default; [U] getter vtable slot 47 (tWPInGameData_Weather_PlayerWindIntensity) returns this + 0x28
    float m_lastPlayerWindIntensity; // 0x2C [O]; [R] ctor default; [U] blend stores the target
    float m_Temperature;             // 0x30 [O]; [R] ctor default; [U] getter vtable slot 48 (tWPInGameData_Weather_Temperature) returns this + 0x30
    float m_lastTemperature;         // 0x34 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlurries;            // 0x38 [O]; [R] ctor default; [U] getter vtable slot 49 (tWPInGameData_Weather_SnowFlurries) returns this + 0x38
    float m_lastSnowFlurries;        // 0x3C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowGravityMultiplier;   // 0x40 [O]; [R] ctor default; [U] getter vtable slot 50 (tWPInGameData_Weather_SnowGravityMultiplier) returns this + 0x40
    float m_lastSnowGravityMultiplier; // 0x44 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffItensity;       // 0x48 [O]; [R] ctor default; [U] getter vtable slot 51 (tWPInGameData_Weather_SnowFluffItensity) returns this + 0x48
    float m_lastSnowFluffItensity;   // 0x4C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffWindIntensity;  // 0x50 [O]; [R] ctor default; [U] getter vtable slot 52 (tWPInGameData_Weather_SnowFluffWindIntensity) returns this + 0x50
    float m_lastSnowFluffWindIntensity; // 0x54 [O]; [R] ctor default; [U] blend stores the target
    float m_LightningChance;         // 0x58 [O]; [R] ctor default; [U] getter vtable slot 53 (tWPInGameData_Weather_LightningChance) returns this + 0x58
    float m_lastLightningChance;     // 0x5C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourA;        // 0x60 [O]; [R] ctor default; [U] getter vtable slot 54 (tWPInGameData_Weather_SnowFlakeColourA) returns this + 0x60
    float m_lastSnowFlakeColourA;    // 0x64 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourR;        // 0x68 [O]; [R] ctor default; [U] getter vtable slot 55 (tWPInGameData_Weather_SnowFlakeColourR) returns this + 0x68
    float m_lastSnowFlakeColourR;    // 0x6C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourG;        // 0x70 [O]; [R] ctor default; [U] getter vtable slot 56 (tWPInGameData_Weather_SnowFlakeColourG) returns this + 0x70
    float m_lastSnowFlakeColourG;    // 0x74 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFlakeColourB;        // 0x78 [O]; [R] ctor default; [U] getter vtable slot 57 (tWPInGameData_Weather_SnowFlakeColourB) returns this + 0x78
    float m_lastSnowFlakeColourB;    // 0x7C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourA;        // 0x80 [O]; [R] ctor default; [U] getter vtable slot 58 (tWPInGameData_Weather_SnowFluffColourA) returns this + 0x80
    float m_lastSnowFluffColourA;    // 0x84 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourR;        // 0x88 [O]; [R] ctor default; [U] getter vtable slot 59 (tWPInGameData_Weather_SnowFluffColourR) returns this + 0x88
    float m_lastSnowFluffColourR;    // 0x8C [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourG;        // 0x90 [O]; [R] ctor default; [U] getter vtable slot 60 (tWPInGameData_Weather_SnowFluffColourG) returns this + 0x90
    float m_lastSnowFluffColourG;    // 0x94 [O]; [R] ctor default; [U] blend stores the target
    float m_SnowFluffColourB;        // 0x98 [O]; [R] ctor default; [U] getter vtable slot 61 (tWPInGameData_Weather_SnowFluffColourB) returns this + 0x98
    float m_lastSnowFluffColourB;    // 0x9C [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Weather_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Weather_setDefaults");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Weather_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Weather_isEqual");
    virtual void readFromReplayFrame(cReplayFramePtr* baseFrame) __asm__("tWPInGameData_Weather_readFromReplayFrame");
};

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

struct tWPInGameData_Danger : public tWPInGameData {
    // tWPInGameData (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] tWPInGameData_Danger_tWPInGameData_Danger stores the vtable tWPInGameData_Danger_vtbl
    // --- tWPInGameData_Danger (alloc tag "tWPIGD_Danger", 0x18 bytes)
    int m_Level;                     // 0x08 [O]; [R] ctor default; [U] getter vtable slot 62 (tWPInGameData_Danger_Level) returns this + 0x8
    int m_lastLevel;                 // 0x0C [O]; [R] ctor default; [U] blend stores the target
    float m_PointRate;               // 0x10 [O]; [R] ctor default; [U] getter vtable slot 63 (tWPInGameData_Danger_PointRate) returns this + 0x10
    float m_lastPointRate;           // 0x14 [O]; [R] ctor default; [U] blend stores the target

    // Methods bound to their retail symbols by asm labels.
    // setDefaults stays a C function (tWPInGameData_Danger_setDefaults): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void setDefaults() __asm__("tWPInGameData_Danger_setDefaults");
    virtual void readFromReplayFrame(cReplayFramePtr* frame) __asm__("tWPInGameData_Danger_readFromReplayFrame");
    virtual void writeToReplayFrame(cReplayFramePtr* frame) const __asm__("tWPInGameData_Danger_writeToReplayFrame");
    virtual void blend(const tWPData* data, float blendFactor) __asm__("tWPInGameData_Danger_blend");
    virtual bool isEqual(const tWPData* data) __asm__("tWPInGameData_Danger_isEqual");
};

CHECK_OFFSET(tWPInGameData_Danger, m_Level, 0x08);
CHECK_OFFSET(tWPInGameData_Danger, m_lastLevel, 0x0C);
CHECK_OFFSET(tWPInGameData_Danger, m_PointRate, 0x10);
CHECK_OFFSET(tWPInGameData_Danger, m_lastPointRate, 0x14);
CHECK_SIZE(tWPInGameData_Danger, 0x18);

// ---- The queries.
// On Tour: tWorldPainterQuery, 8 bytes (m_data, then the vptr of its virtual dtor / queryPosition).
struct tWorldPainterQuery {
    tWPInGameData* m_data;          // 0x0 [O]
    // 0x4: the vptr. [R] tWorldPainterQuery__tWorldPainterQuery (dtor) stores tWorldPainterQuery_vtbl. Slots:
    virtual ~tWorldPainterQuery();                              // 1  ([R] tWorldPainterQuery_vtbl {dtor, pure queryPosition})
    virtual const tWPInGameData* queryPosition(float fx, float fy, float blend) = 0; // 2 (types [O])
};

CHECK_OFFSET(tWorldPainterQuery, m_data, 0x0);
CHECK_SIZE(tWorldPainterQuery, 0x8);

struct cWorldPainterQuery : public tWorldPainterQuery {
    // tWorldPainterQuery (worldpainter/worldpainterquery.h, 0x8): the vptr at 0x4:
    //   [R] ctor and dtor store cWorldPainterQuery_vtbl (D_00483E00); the dtor then the base's tWorldPainterQuery_vtbl
    // --- cWorldPainterQuery
    float m_x;                      // 0x08 [O]; [R] ctor stores a sentinel; [U] queryPosition: the last fx (distance
                                    //       moved), then stores fx
    float m_y;                      // 0x0C [O]; [R] ctor like m_x; [U] queryPosition: the last fy
    tWorldPainterRegionType m_regionType; // 0x10 [O]; [R] ctor's argument (sb low byte, then the word with the id)
    cWorldPainterQuery* m_next;     // 0x14 [O]; [R] ctor links this in front of m_queryListStart; dtor unlinks;
                                    //       [U] reset walks the list

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cWorldPainterQuery__cWorldPainterQuery stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cWorldPainterQuery();
    virtual const tWPInGameData* queryPosition(float x, float z, float blend)
        __asm__("cWorldPainterQuery_queryPosition");
};

CHECK_OFFSET(cWorldPainterQuery, m_x, 0x08);
CHECK_OFFSET(cWorldPainterQuery, m_y, 0x0C);
CHECK_OFFSET(cWorldPainterQuery, m_regionType, 0x10);
CHECK_OFFSET(cWorldPainterQuery, m_next, 0x14);
CHECK_SIZE(cWorldPainterQuery, 0x18);

#endif /* WORLDPAINTER_WORLDPAINTERQUERY_H */
