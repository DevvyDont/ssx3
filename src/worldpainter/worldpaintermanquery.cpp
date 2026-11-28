#include "common.h"
#include "worldpainter/worldpainterman.h"
#include "vtable.h"
#include "worldpainter/quadtree.h"
#include "worldpainter/worldpainterquery.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldPainterQuery_cWorldPainterQuery(void* self, void* arg);
extern const char str_WorldPainterQuery[];

struct cWorldPainterManArg {
    void* field_0x0;
};

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", cWorldPainterMan_getQuery__FPvP19cWorldPainterManArg);
#ifdef SKIP_ASM
/**
 * Creates a new world painter query for one painted resource.
 *
 * Allocates 0x18 bytes tagged "WorldPainterQuery" and constructs a cWorldPainterQuery from the first word
 * of @p x. The manager itself is not used.
 *
 * @param x  the painted resource record (such as kWPR_Lighting; the lens effects pass nine of them)
 * @return the new query
 */
void* cWorldPainterMan_getQuery(void* vself, cWorldPainterManArg* x)
{
    cWorldPainterMan* self = (cWorldPainterMan*)vself;
    void* mem = cMemMan_alloc(0x18, str_WorldPainterQuery, 0, 0);
    void* queryArg = x->field_0x0;
    return cWorldPainterQuery_cWorldPainterQuery(mem, queryArg);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_blend);
#ifdef SKIP_ASM
/**
 * Blends an int value toward a target: *dst = *src when @p blend is exactly 1.0, otherwise *dst = (int)(*dst * (1 -
 * blend) + *src * blend). Nothing in retail calls it out of line.
 *
 * @param dst  the value to blend (updated in place)
 * @param src  the target value
 * @param blend  the blend factor (0 keeps dst, 1 takes src)
 */
extern "C" void tWPInGameData_blend(tWPInGameData* self, int* dst, int* src, float blend)
{
    if (blend == 1.0f) {
        *dst = *src;
    } else {
        *dst = (int)((float)*dst * (1.0f - blend) + (float)*src * blend);
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_blend1);
#ifdef SKIP_ASM
/**
 * Blends a float value toward a target: *dst = *src when @p blend is exactly 1.0, otherwise *dst = *dst * (1 - blend) +
 * *src * blend. Nothing in retail calls it out of line.
 *
 * @param dst  the value to blend (updated in place)
 * @param src  the target value
 * @param blend  the blend factor (0 keeps dst, 1 takes src)
 */
void tWPInGameData::blend1(float* dst, float* src, float blend)
{
    if (blend == 1.0f) {
        *dst = *src;
    } else {
        *dst = *dst * (1.0f - blend) + *src * blend;
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0B30);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest: a variadic function whose body only spills the argument registers
 * (a1-a3, t0-t3 and f12-f19) to its stack frame. Nothing in retail references it.
 *
 * @return the first argument
 */
extern "C" void* func_002C0B30(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0B70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_2C0B70 {
    float r, g, b, a;
    sColor_2C0B70(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};



extern int kNullNameField_w[2] __asm__("kNullNameField");
extern tWorldPainterRegionType D_004A53D8;
extern tWorldPainterRegionType D_004A53E0;
extern tWorldPainterRegionType D_004A53E8;
extern tWorldPainterRegionType D_004A53F0;
extern tWorldPainterRegionType D_004A53F8;
extern tWorldPainterRegionType D_004A5400;
extern tWorldPainterRegionType D_004A5408;
extern tWorldPainterRegionType D_004A5410;
extern tWorldPainterRegionType D_004A5418;
extern tWorldPainterRegionType D_004A5420;
extern tWorldPainterRegionType D_004A5428;
extern tWorldPainterRegionType D_004A5430;
extern tWorldPainterRegionType D_004A5438;
extern tWorldPainterRegionType D_004A5440;

extern sColor_2C0B70 D_004D4638;
extern sColor_2C0B70 D_004D4648;
extern sColor_2C0B70 D_004D4658;
extern sColor_2C0B70 D_004D4668;
extern sColor_2C0B70 D_004D4678;
extern sColor_2C0B70 D_004D4688;
extern sColor_2C0B70 D_004D4698;
extern sColor_2C0B70 D_004D46A8;
extern sColor_2C0B70 D_004D46B8;
extern sColor_2C0B70 D_004D46C8;
extern sColor_2C0B70 D_004D46D8;
extern sColor_2C0B70 D_004D46E8;
extern sColor_2C0B70 D_004D46F8;
extern sColor_2C0B70 D_004D4708;
extern sColor_2C0B70 D_004D4718;
extern sColor_2C0B70 D_004D4728;
extern sColor_2C0B70 D_004D4738;
extern sColor_2C0B70 D_004D4748;
extern sColor_2C0B70 D_004D4758;
extern sColor_2C0B70 D_004D4768;
extern sColor_2C0B70 D_004D4778;
extern sColor_2C0B70 D_004D4788;
extern sColor_2C0B70 D_004D4798;
extern sColor_2C0B70 D_004D47A8;
extern sColor_2C0B70 D_004D47B8;
extern sColor_2C0B70 D_004D47C8;
extern sColor_2C0B70 D_004D47D8;
extern sColor_2C0B70 D_004D47E8;
extern sColor_2C0B70 D_004D47F8;
extern sColor_2C0B70 D_004D4808;
extern sColor_2C0B70 D_004D4818;
extern sColor_2C0B70 D_004D4828;
extern sColor_2C0B70 D_004D4838;
extern sColor_2C0B70 D_004D4848;
extern sColor_2C0B70 D_004D4858;
extern sColor_2C0B70 D_004D4868;
extern sColor_2C0B70 D_004D4878;
extern sColor_2C0B70 D_004D4888;
extern sColor_2C0B70 D_004D4898;
extern sColor_2C0B70 D_004D48A8;
extern sColor_2C0B70 D_004D48B8;
extern sColor_2C0B70 D_004D48C8;
extern sColor_2C0B70 D_004D48D8;
extern sColor_2C0B70 D_004D48E8;
extern sColor_2C0B70 D_004D48F8;
extern sColor_2C0B70 D_004D4908;
extern sColor_2C0B70 D_004D4918;
extern sColor_2C0B70 D_004D4928;
extern sColor_2C0B70 D_004D4938;
extern sColor_2C0B70 D_004D4948;
extern sColor_2C0B70 D_004D4958;
extern sColor_2C0B70 D_004D4968;
extern sColor_2C0B70 D_004D4978;
extern sColor_2C0B70 D_004D4988;
extern sColor_2C0B70 D_004D4998;
extern sColor_2C0B70 D_004D49A8;

// The unit declares func_002C0B70 later as returning void*; bind this body by asm label.
extern "C" void func_002C0B70_impl(int initialize, int priority) __asm__("func_002C0B70");

/**
 * Runs the static constructors of the world-painter query unit (g++'s static initialisation and destruction function).
 *
 * Only when @p initialize is nonzero and @p priority is DEFAULT_INIT_PRIORITY: constructs the 56 four-float colour
 * constants D_004D4638 .. D_004D49A8, zeroes this unit's kNullNameField copy (kNullNameField) and builds its 14
 * region-type constants kWPR_MusicTrigger .. kWPR_Danger (D_004A53D8 .. D_004A5440, each tWorldPainterRegionType(n, n),
 * n = 0 .. 13). The destruction pass does nothing. func_002C1688__FPv calls it with (1, 0xFFFF).
 *
 * @param initialize  nonzero for the construction pass
 * @param priority  the init priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void func_002C0B70_impl(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        new (&D_004D4638) sColor_2C0B70(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D4648) sColor_2C0B70(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4658) sColor_2C0B70(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4668) sColor_2C0B70(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D4678) sColor_2C0B70(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D4688) sColor_2C0B70(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4698) sColor_2C0B70(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D46A8) sColor_2C0B70(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D46B8) sColor_2C0B70(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D46C8) sColor_2C0B70(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D46D8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D46E8) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D46F8) sColor_2C0B70(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D4708) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4718) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4728) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4738) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4748) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4758) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4768) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4778) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4788) sColor_2C0B70(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D4798) sColor_2C0B70(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D47A8) sColor_2C0B70(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D47B8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D47C8) sColor_2C0B70(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D47D8) sColor_2C0B70(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D47E8) sColor_2C0B70(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D47F8) sColor_2C0B70(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D4808) sColor_2C0B70(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D4818) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4828) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4838) sColor_2C0B70(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D4848) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4858) sColor_2C0B70(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D4868) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4878) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4888) sColor_2C0B70(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4898) sColor_2C0B70(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D48A8) sColor_2C0B70(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D48B8) sColor_2C0B70(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D48C8) sColor_2C0B70(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D48D8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D48E8) sColor_2C0B70(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D48F8) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4908) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4918) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4928) sColor_2C0B70(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D4938) sColor_2C0B70(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D4948) sColor_2C0B70(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D4958) sColor_2C0B70(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D4968) sColor_2C0B70(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D4978) sColor_2C0B70(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D4988) sColor_2C0B70(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D4998) sColor_2C0B70(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D49A8) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        kNullNameField_w[1] = 0;
        kNullNameField_w[0] = 0;
        new (&D_004A53D8) tWorldPainterRegionType(0, 0);
        new (&D_004A53E0) tWorldPainterRegionType(1, 1);
        new (&D_004A53E8) tWorldPainterRegionType(2, 2);
        new (&D_004A53F0) tWorldPainterRegionType(3, 3);
        new (&D_004A53F8) tWorldPainterRegionType(4, 4);
        new (&D_004A5400) tWorldPainterRegionType(5, 5);
        new (&D_004A5408) tWorldPainterRegionType(6, 6);
        new (&D_004A5410) tWorldPainterRegionType(7, 7);
        new (&D_004A5418) tWorldPainterRegionType(8, 8);
        new (&D_004A5420) tWorldPainterRegionType(9, 9);
        new (&D_004A5428) tWorldPainterRegionType(10, 10);
        new (&D_004A5430) tWorldPainterRegionType(11, 11);
        new (&D_004A5438) tWorldPainterRegionType(12, 12);
        new (&D_004A5440) tWorldPainterRegionType(13, 13);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_debugBlend__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: tWPInGameData's own entry in vtable slot 67 (between the pure blend and isEqual slots).
 */
void tWPInGameData::debugBlend()
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWorldPainterQuery__tWorldPainterQuery);
#ifdef SKIP_ASM
extern void* tWorldPainterQuery_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a world painter query: restores its vtable (+0x4), then frees it when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void tWorldPainterQuery__tWorldPainterQuery(tWorldPainterQuery* self, int flags)
{
    VPTR_OF(tWorldPainterQuery, self) = tWorldPainterQuery_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_MusicTrigger_Type__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_Type (this + 0x8) of a tWPInGameData_MusicTrigger (its override of vtable slot 1).
 *
 * @return &m_Type
 */
void* tWPInGameData::MusicTrigger_Type()
{
    return &((tWPInGameData_MusicTrigger*)this)->m_Type;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_MusicTrigger_Value__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_Value (this + 0x10) of a tWPInGameData_MusicTrigger (its override of vtable slot 2).
 *
 * @return &m_Value
 */
void* tWPInGameData::MusicTrigger_Value()
{
    return &((tWPInGameData_MusicTrigger*)this)->m_Value;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Mix_MixPreset__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_MixPreset (this + 0x8) of a tWPInGameData_Mix (its override of vtable slot 3).
 *
 * @return &m_MixPreset
 */
void* tWPInGameData::Mix_MixPreset()
{
    return &((tWPInGameData_Mix*)this)->m_MixPreset;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Ambience_BankID__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_BankID (this + 0x8) of a tWPInGameData_Ambience (its override of vtable slot 4).
 *
 * @return &m_BankID
 */
void* tWPInGameData::Ambience_BankID()
{
    return &((tWPInGameData_Ambience*)this)->m_BankID;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Speech_SpeechID__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SpeechID (this + 0x8) of a tWPInGameData_Speech (its override of vtable slot 5).
 *
 * @return &m_SpeechID
 */
void* tWPInGameData::Speech_SpeechID()
{
    return &((tWPInGameData_Speech*)this)->m_SpeechID;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Camera_DrawDistance__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_DrawDistance (this + 0x8) of a tWPInGameData_Camera (its override of vtable slot 6).
 *
 * @return &m_DrawDistance
 */
void* tWPInGameData::Camera_DrawDistance()
{
    return &((tWPInGameData_Camera*)this)->m_DrawDistance;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogDensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogDensity (this + 0x8) of a tWPInGameData_Fog (its override of vtable slot 7).
 *
 * @return &m_FogDensity
 */
void* tWPInGameData::Fog_FogDensity()
{
    return &((tWPInGameData_Fog*)this)->m_FogDensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogNear__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogNear (this + 0x10) of a tWPInGameData_Fog (its override of vtable slot 8).
 *
 * @return &m_FogNear
 */
void* tWPInGameData::Fog_FogNear()
{
    return &((tWPInGameData_Fog*)this)->m_FogNear;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogFar__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogFar (this + 0x18) of a tWPInGameData_Fog (its override of vtable slot 9).
 *
 * @return &m_FogFar
 */
void* tWPInGameData::Fog_FogFar()
{
    return &((tWPInGameData_Fog*)this)->m_FogFar;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogColourR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogColourR (this + 0x20) of a tWPInGameData_Fog (its override of vtable slot 10).
 *
 * @return &m_FogColourR
 */
void* tWPInGameData::Fog_FogColourR()
{
    return &((tWPInGameData_Fog*)this)->m_FogColourR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogColourG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogColourG (this + 0x28) of a tWPInGameData_Fog (its override of vtable slot 11).
 *
 * @return &m_FogColourG
 */
void* tWPInGameData::Fog_FogColourG()
{
    return &((tWPInGameData_Fog*)this)->m_FogColourG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Fog_FogColourB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FogColourB (this + 0x30) of a tWPInGameData_Fog (its override of vtable slot 12).
 *
 * @return &m_FogColourB
 */
void* tWPInGameData::Fog_FogColourB()
{
    return &((tWPInGameData_Fog*)this)->m_FogColourB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowCutoff__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowCutoff (this + 0x8) of a tWPInGameData_LightGlow (its override of vtable slot 13).
 *
 * @return &m_LightGlowCutoff
 */
void* tWPInGameData::LightGlow_LightGlowCutoff()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowCutoff;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowCutoffScale__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowCutoffScale (this + 0x10) of a tWPInGameData_LightGlow (its override of vtable slot 14).
 *
 * @return &m_LightGlowCutoffScale
 */
void* tWPInGameData::LightGlow_LightGlowCutoffScale()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowCutoffScale;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowCopyIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowCopyIntensity (this + 0x18) of a tWPInGameData_LightGlow (its override of vtable slot 15).
 *
 * @return &m_LightGlowCopyIntensity
 */
void* tWPInGameData::LightGlow_LightGlowCopyIntensity()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowCopyIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowSourceIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowSourceIntensity (this + 0x20) of a tWPInGameData_LightGlow (its override of vtable slot 16).
 *
 * @return &m_LightGlowSourceIntensity
 */
void* tWPInGameData::LightGlow_LightGlowSourceIntensity()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowSourceIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowFrameIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowFrameIntensity (this + 0x28) of a tWPInGameData_LightGlow (its override of vtable slot 17).
 *
 * @return &m_LightGlowFrameIntensity
 */
void* tWPInGameData::LightGlow_LightGlowFrameIntensity()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowFrameIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowBlend2__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowBlend2 (this + 0x30) of a tWPInGameData_LightGlow (its override of vtable slot 18).
 *
 * @return &m_LightGlowBlend2
 */
void* tWPInGameData::LightGlow_LightGlowBlend2()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowBlend2;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_LightGlow_LightGlowBlend3__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightGlowBlend3 (this + 0x38) of a tWPInGameData_LightGlow (its override of vtable slot 19).
 *
 * @return &m_LightGlowBlend3
 */
void* tWPInGameData::LightGlow_LightGlowBlend3()
{
    return &((tWPInGameData_LightGlow*)this)->m_LightGlowBlend3;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintScaleR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintScaleR (this + 0x8) of a tWPInGameData_ScreenTint (its override of vtable slot 20).
 *
 * @return &m_ScreenTintScaleR
 */
void* tWPInGameData::ScreenTint_ScreenTintScaleR()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintScaleR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintScaleG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintScaleG (this + 0x10) of a tWPInGameData_ScreenTint (its override of vtable slot 21).
 *
 * @return &m_ScreenTintScaleG
 */
void* tWPInGameData::ScreenTint_ScreenTintScaleG()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintScaleG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintScaleB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintScaleB (this + 0x18) of a tWPInGameData_ScreenTint (its override of vtable slot 22).
 *
 * @return &m_ScreenTintScaleB
 */
void* tWPInGameData::ScreenTint_ScreenTintScaleB()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintScaleB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintOffsetR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintOffsetR (this + 0x20) of a tWPInGameData_ScreenTint (its override of vtable slot 23).
 *
 * @return &m_ScreenTintOffsetR
 */
void* tWPInGameData::ScreenTint_ScreenTintOffsetR()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintOffsetR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintOffsetG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintOffsetG (this + 0x28) of a tWPInGameData_ScreenTint (its override of vtable slot 24).
 *
 * @return &m_ScreenTintOffsetG
 */
void* tWPInGameData::ScreenTint_ScreenTintOffsetG()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintOffsetG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_ScreenTint_ScreenTintOffsetB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ScreenTintOffsetB (this + 0x30) of a tWPInGameData_ScreenTint (its override of vtable slot 25).
 *
 * @return &m_ScreenTintOffsetB
 */
void* tWPInGameData::ScreenTint_ScreenTintOffsetB()
{
    return &((tWPInGameData_ScreenTint*)this)->m_ScreenTintOffsetB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_SkyBox_SkyBoxRotation__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SkyBoxRotation (this + 0x8) of a tWPInGameData_SkyBox (its override of vtable slot 26).
 *
 * @return &m_SkyBoxRotation
 */
void* tWPInGameData::SkyBox_SkyBoxRotation()
{
    return &((tWPInGameData_SkyBox*)this)->m_SkyBoxRotation;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunPitchOffset__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunPitchOffset (this + 0x8) of a tWPInGameData_Sun (its override of vtable slot 27).
 *
 * @return &m_SunPitchOffset
 */
void* tWPInGameData::Sun_SunPitchOffset()
{
    return &((tWPInGameData_Sun*)this)->m_SunPitchOffset;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunYawOffset__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunYawOffset (this + 0x10) of a tWPInGameData_Sun (its override of vtable slot 28).
 *
 * @return &m_SunYawOffset
 */
void* tWPInGameData::Sun_SunYawOffset()
{
    return &((tWPInGameData_Sun*)this)->m_SunYawOffset;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunColorR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunColorR (this + 0x18) of a tWPInGameData_Sun (its override of vtable slot 29).
 *
 * @return &m_SunColorR
 */
void* tWPInGameData::Sun_SunColorR()
{
    return &((tWPInGameData_Sun*)this)->m_SunColorR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunColorG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunColorG (this + 0x20) of a tWPInGameData_Sun (its override of vtable slot 30).
 *
 * @return &m_SunColorG
 */
void* tWPInGameData::Sun_SunColorG()
{
    return &((tWPInGameData_Sun*)this)->m_SunColorG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunColorB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunColorB (this + 0x28) of a tWPInGameData_Sun (its override of vtable slot 31).
 *
 * @return &m_SunColorB
 */
void* tWPInGameData::Sun_SunColorB()
{
    return &((tWPInGameData_Sun*)this)->m_SunColorB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunIntensity (this + 0x30) of a tWPInGameData_Sun (its override of vtable slot 32).
 *
 * @return &m_SunIntensity
 */
void* tWPInGameData::Sun_SunIntensity()
{
    return &((tWPInGameData_Sun*)this)->m_SunIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunType__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunType (this + 0x38) of a tWPInGameData_Sun (its override of vtable slot 33).
 *
 * @return &m_SunType
 */
void* tWPInGameData::Sun_SunType()
{
    return &((tWPInGameData_Sun*)this)->m_SunType;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_LensIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LensIntensity (this + 0x40) of a tWPInGameData_Sun (its override of vtable slot 34).
 *
 * @return &m_LensIntensity
 */
void* tWPInGameData::Sun_LensIntensity()
{
    return &((tWPInGameData_Sun*)this)->m_LensIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Sun_SunRadius__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SunRadius (this + 0x48) of a tWPInGameData_Sun (its override of vtable slot 35).
 *
 * @return &m_SunRadius
 */
void* tWPInGameData::Sun_SunRadius()
{
    return &((tWPInGameData_Sun*)this)->m_SunRadius;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Surface_SnowSparkleDensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowSparkleDensity (this + 0x8) of a tWPInGameData_Surface (its override of vtable slot 36).
 *
 * @return &m_SnowSparkleDensity
 */
void* tWPInGameData::Surface_SnowSparkleDensity()
{
    return &((tWPInGameData_Surface*)this)->m_SnowSparkleDensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_LightIrradianceName__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightIrradianceName (this + 0x8) of a tWPInGameData_Lighting (its override of vtable slot 37).
 *
 * @return &m_LightIrradianceName
 */
void* tWPInGameData::Lighting_LightIrradianceName()
{
    return &((tWPInGameData_Lighting*)this)->m_LightIrradianceName;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_ShadowIrradianceName__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ShadowIrradianceName (this + 0x18) of a tWPInGameData_Lighting (its override of vtable slot 38).
 *
 * @return &m_ShadowIrradianceName
 */
void* tWPInGameData::Lighting_ShadowIrradianceName()
{
    return &((tWPInGameData_Lighting*)this)->m_ShadowIrradianceName;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_TunnelIrradianceName__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_TunnelIrradianceName (this + 0x28) of a tWPInGameData_Lighting (its override of vtable slot 39).
 *
 * @return &m_TunnelIrradianceName
 */
void* tWPInGameData::Lighting_TunnelIrradianceName()
{
    return &((tWPInGameData_Lighting*)this)->m_TunnelIrradianceName;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_ObjectIrradianceName__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_ObjectIrradianceName (this + 0x38) of a tWPInGameData_Lighting (its override of vtable slot 40).
 *
 * @return &m_ObjectIrradianceName
 */
void* tWPInGameData::Lighting_ObjectIrradianceName()
{
    return &((tWPInGameData_Lighting*)this)->m_ObjectIrradianceName;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_AmbientLightIntensityOffset__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_AmbientLightIntensityOffset (this + 0x48) of a tWPInGameData_Lighting (its override of vtable slot 41).
 *
 * @return &m_AmbientLightIntensityOffset
 */
void* tWPInGameData::Lighting_AmbientLightIntensityOffset()
{
    return &((tWPInGameData_Lighting*)this)->m_AmbientLightIntensityOffset;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Lighting_RimLightIntensityOffset__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_RimLightIntensityOffset (this + 0x50) of a tWPInGameData_Lighting (its override of vtable slot 42).
 *
 * @return &m_RimLightIntensityOffset
 */
void* tWPInGameData::Lighting_RimLightIntensityOffset()
{
    return &((tWPInGameData_Lighting*)this)->m_RimLightIntensityOffset;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowIntensity (this + 0x8) of a tWPInGameData_Weather (its override of vtable slot 43).
 *
 * @return &m_SnowIntensity
 */
void* tWPInGameData::Weather_SnowIntensity()
{
    return &((tWPInGameData_Weather*)this)->m_SnowIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_FlakeSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_FlakeSize (this + 0x10) of a tWPInGameData_Weather (its override of vtable slot 44).
 *
 * @return &m_FlakeSize
 */
void* tWPInGameData::Weather_FlakeSize()
{
    return &((tWPInGameData_Weather*)this)->m_FlakeSize;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_WindIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_WindIntensity (this + 0x18) of a tWPInGameData_Weather (its override of vtable slot 45).
 *
 * @return &m_WindIntensity
 */
void* tWPInGameData::Weather_WindIntensity()
{
    return &((tWPInGameData_Weather*)this)->m_WindIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_WindRotation__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_WindRotation (this + 0x20) of a tWPInGameData_Weather (its override of vtable slot 46).
 *
 * @return &m_WindRotation
 */
void* tWPInGameData::Weather_WindRotation()
{
    return &((tWPInGameData_Weather*)this)->m_WindRotation;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_PlayerWindIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_PlayerWindIntensity (this + 0x28) of a tWPInGameData_Weather (its override of vtable slot 47).
 *
 * @return &m_PlayerWindIntensity
 */
void* tWPInGameData::Weather_PlayerWindIntensity()
{
    return &((tWPInGameData_Weather*)this)->m_PlayerWindIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_Temperature__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_Temperature (this + 0x30) of a tWPInGameData_Weather (its override of vtable slot 48).
 *
 * @return &m_Temperature
 */
void* tWPInGameData::Weather_Temperature()
{
    return &((tWPInGameData_Weather*)this)->m_Temperature;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFlurries__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFlurries (this + 0x38) of a tWPInGameData_Weather (its override of vtable slot 49).
 *
 * @return &m_SnowFlurries
 */
void* tWPInGameData::Weather_SnowFlurries()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFlurries;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowGravityMultiplier__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowGravityMultiplier (this + 0x40) of a tWPInGameData_Weather (its override of vtable slot 50).
 *
 * @return &m_SnowGravityMultiplier
 */
void* tWPInGameData::Weather_SnowGravityMultiplier()
{
    return &((tWPInGameData_Weather*)this)->m_SnowGravityMultiplier;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffItensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffItensity (this + 0x48) of a tWPInGameData_Weather (its override of vtable slot 51).
 *
 * @return &m_SnowFluffItensity
 */
void* tWPInGameData::Weather_SnowFluffItensity()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffItensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffWindIntensity__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffWindIntensity (this + 0x50) of a tWPInGameData_Weather (its override of vtable slot 52).
 *
 * @return &m_SnowFluffWindIntensity
 */
void* tWPInGameData::Weather_SnowFluffWindIntensity()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffWindIntensity;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_LightningChance__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_LightningChance (this + 0x58) of a tWPInGameData_Weather (its override of vtable slot 53).
 *
 * @return &m_LightningChance
 */
void* tWPInGameData::Weather_LightningChance()
{
    return &((tWPInGameData_Weather*)this)->m_LightningChance;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFlakeColourA__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFlakeColourA (this + 0x60) of a tWPInGameData_Weather (its override of vtable slot 54).
 *
 * @return &m_SnowFlakeColourA
 */
void* tWPInGameData::Weather_SnowFlakeColourA()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFlakeColourA;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFlakeColourR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFlakeColourR (this + 0x68) of a tWPInGameData_Weather (its override of vtable slot 55).
 *
 * @return &m_SnowFlakeColourR
 */
void* tWPInGameData::Weather_SnowFlakeColourR()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFlakeColourR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFlakeColourG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFlakeColourG (this + 0x70) of a tWPInGameData_Weather (its override of vtable slot 56).
 *
 * @return &m_SnowFlakeColourG
 */
void* tWPInGameData::Weather_SnowFlakeColourG()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFlakeColourG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFlakeColourB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFlakeColourB (this + 0x78) of a tWPInGameData_Weather (its override of vtable slot 57).
 *
 * @return &m_SnowFlakeColourB
 */
void* tWPInGameData::Weather_SnowFlakeColourB()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFlakeColourB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffColourA__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffColourA (this + 0x80) of a tWPInGameData_Weather (its override of vtable slot 58).
 *
 * @return &m_SnowFluffColourA
 */
void* tWPInGameData::Weather_SnowFluffColourA()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffColourA;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffColourR__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffColourR (this + 0x88) of a tWPInGameData_Weather (its override of vtable slot 59).
 *
 * @return &m_SnowFluffColourR
 */
void* tWPInGameData::Weather_SnowFluffColourR()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffColourR;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffColourG__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffColourG (this + 0x90) of a tWPInGameData_Weather (its override of vtable slot 60).
 *
 * @return &m_SnowFluffColourG
 */
void* tWPInGameData::Weather_SnowFluffColourG()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffColourG;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Weather_SnowFluffColourB__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_SnowFluffColourB (this + 0x98) of a tWPInGameData_Weather (its override of vtable slot 61).
 *
 * @return &m_SnowFluffColourB
 */
void* tWPInGameData::Weather_SnowFluffColourB()
{
    return &((tWPInGameData_Weather*)this)->m_SnowFluffColourB;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Danger_Level__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_Level (this + 0x8) of a tWPInGameData_Danger (its override of vtable slot 62).
 *
 * @return &m_Level
 */
void* tWPInGameData::Danger_Level()
{
    return &((tWPInGameData_Danger*)this)->m_Level;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tWPInGameData_Danger_PointRate__FPv);
#ifdef SKIP_ASM
/**
 * Returns &m_PointRate (this + 0x10) of a tWPInGameData_Danger (its override of vtable slot 63).
 *
 * @return &m_PointRate
 */
void* tWPInGameData::Danger_PointRate()
{
    return &((tWPInGameData_Danger*)this)->m_PointRate;
}
#endif

extern "C" void* func_002C0B70(int, int);

//99.38%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1688__FPv);
#ifdef SKIP_ASM
/**
 * Runs the world-painter query unit's static constructors: func_002C0B70(1, 0xFFFF). It is entry 23 of the static
 * constructor table __CTOR_LIST__.
 *
 * @return func_002C0B70's return register (it returns nothing)
 */
void* func_002C1688(void* self)
{
    return func_002C0B70(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tQuadTreeNode_setLeafNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// LANDING: later declaration (line ~656) is `extern "C" void tQuadTreeNode_setLeafNode(int, int, int);` used by its caller
// with an int expression; retype it to (void*, int, int) and drop the alias if the lander prefers.
extern "C" void tQuadTreeNode_setLeafNode_impl(void* self, int a, int b) __asm__("tQuadTreeNode_setLeafNode");

/**
 * Makes a node a leaf: stores @p key in m_key (+0x4) and @p data in m_data (+0x0). cQuadTree::cQuadTree uses it for the
 * root.
 *
 * @param key  the leaf key (the region index)
 * @param data  the leaf data pointer
 */
void tQuadTreeNode::setLeafNode(int key, int data)
{
    m_leaf.m_key = key;
    *(int*)&m_leaf = data;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tQuadTreeNode_setSubNodes);
#ifdef SKIP_ASM
/**
 * Makes a node an inner node with four children: stores each child index shifted left by one, the first with bit 0 set
 * (the sub-node flag) and the other three keeping their old bit 0.
 *
 * cQuadTree_exportTree uses it to renumber the children of exported nodes.
 *
 * @param node  the node (as its four 16-bit words)
 * @param subA  the first child's node index
 * @param subB  the second child's node index
 * @param subC  the third child's node index
 * @param subD  the fourth child's node index
 */
extern "C" void tQuadTreeNode_setSubNodes(unsigned short* node, unsigned short subA, unsigned short subB, unsigned short subC, unsigned short subD)
{
    node[0] = (subA << 1) | 1;
    node[1] = (subB << 1) | (node[1] & 1);
    node[2] = (subC << 1) | (node[2] & 1);
    node[3] = (subD << 1) | (node[3] & 1);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", tQuadTreeNode_init);
#ifdef SKIP_ASM
/**
 * Clears a quad-tree node: zeroes both of its words (a leaf with key 0 and no data).
 *
 */
void tQuadTreeNode::init()
{
    (*(int*)&m_leaf) = 0;
    ((int)m_leaf.m_key) = 0;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/worldpaintermanquery", __9cQuadTreeiifff);
#ifdef SKIP_ASM
extern "C" unsigned short cQuadTree_getFreeNode();
extern "C" void tQuadTreeNode_setLeafNode(int, int, int);

/**
 * Constructs an empty quad tree over a square area whose whole area is one leaf.
 *
 * Clears m_numUsedNodes, m_numNodes, m_nodes and m_freeNodes, stores the area's minimum x / y in m_offsetX / m_offsetY
 * and 32768 / width in m_scale, takes the root node from cQuadTree_getFreeNode (which allocates the first 1024 nodes),
 * clears m_pad and makes the root a leaf with the default key and data (tQuadTreeNode_setLeafNode), which m_defaultNode
 * also keeps (getLeaf returns it for positions outside the grid).
 *
 * @param defaultKey  the key of the whole area (and of positions outside it)
 * @param defaultData  the data pointer of the whole area
 * @param fparg0  the area's minimum x in world units
 * @param fparg1  the area's minimum y in world units
 * @param fparg2  the area's width (and height) in world units
 */
cQuadTree::cQuadTree(int defaultKey, int defaultData, float fparg0, float fparg1, float fparg2)
{
    m_freeNodes = 0;
    m_nodes = 0;
    m_numNodes = 0;
    m_numUsedNodes = 0;
    m_offsetX = fparg0;
    m_offsetY = fparg1;
    m_scale = (float) (32768.0f / fparg2);
    m_root = cQuadTree_getFreeNode();
    (*(short*)&m_pad) = 0;
    tQuadTreeNode_setLeafNode(((int)m_nodes) + ((m_root) * 8), defaultKey, defaultData);
    m_defaultNode.m_key = defaultKey;
    (*(int*)&m_defaultNode) = defaultData;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", cQuadTree_resolve__FPv);
#ifdef SKIP_ASM
/**
 * Points m_nodes and m_freeNodes at the node array stored right after the 0x28-byte tree in streamed world-painter data
 * (this + 0x28). cWorldPainterLayer_resolve calls it.
 */
void cQuadTree::resolve()
{
    void* nodes = (char*)this + 0x28;
    m_nodes = (tQuadTreeNode*)nodes;
    m_freeNodes = (tQuadTreeNode*)nodes;
}
#endif

