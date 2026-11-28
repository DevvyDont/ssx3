#include "vtable.h"
#include "common.h"
#include "render/bxps2tex.h"
#include "visualfx/worldlightman.h"
#include "render/ps2scratchpad.h"
#include "bx/math3d.h"
#include "render/2drender.h"
#include "render/ps2lightman.h"
#include "render/font.h"
#include "ui/uithing.h"
#include "render/ps2graphicsman.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_LightMan[];
extern void* cPS2LightMan_vtbl[16];

struct cPSPLightMan;
cPSPLightMan* cLightMan_cLightMan(cPSPLightMan* self);

//100%
INCLUDE_ASM("render/lightman", cLightMan_construct__Fv);
#ifdef SKIP_ASM
/**
 * Creates the light manager: allocates 0x14 bytes ("LightMan"), constructs it and installs the PS2 vtable.
 *
 * Runs cLightMan_cLightMan(), which registers the object as the global light manager (cLightMan_TheLightMan), then
 * overwrites the vtable with cPS2LightMan_vtbl (cPS2LightMan's: its slot 1 is cPS2LightMan__cPS2LightMan).
 *
 * @return the new manager
 */
void* cLightMan_construct()
{
    void* mem = cMemMan_alloc(20, str_LightMan, 0, 0);
    cLightMan_cLightMan((cPSPLightMan*)mem);
    *(void**)mem = cPS2LightMan_vtbl;
    return mem;
}
#endif

//100%
INCLUDE_ASM("render/lightman", func_0038DC20);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
extern "C" void func_0038DC20(void) {

}
#endif
//100%
INCLUDE_ASM("render/lightman", func_0038DC28);
#ifdef SKIP_ASM
/**
 * Sets the four words at @p self to -1.
 *
 * No caller.
 */
extern "C" void func_0038DC28(unsigned int* self)
{
    self[0] = 0xFFFFFFFF;
    self[1] = 0xFFFFFFFF;
    self[2] = 0xFFFFFFFF;
    self[3] = 0xFFFFFFFF;
}
#endif
//100%
INCLUDE_ASM("render/lightman", func_0038DC48);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
extern "C" void func_0038DC48(void) {

}
#endif

//100%
INCLUDE_ASM("render/lightman", cLightInst_CreateAmbient__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
void cLightInst::CreateAmbient()
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", cLightInst_CreateDirectional__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
void cLightInst::CreateDirectional()
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", cLightInst_CreatePoint__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
void cLightInst::CreatePoint()
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", cLightInst_CreateSpot__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * No caller.
 */
void cLightInst::CreateSpot()
{
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2LightMan_CreateLights);
#ifdef SKIP_ASM
struct sV3_DC70 {
    float x, y, z;
    sV3_DC70() {}
    sV3_DC70(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sV3_DC70(const sV3_DC70& o) : x(o.x), y(o.y), z(o.z) {}
};

struct sCol_DC70 {
    float a, r, g, b;
};

extern "C" float func_0031C128(float x);
// PORT: the unit's empty stubs take (void*); these callers pass more, bound by asm label.
void cLightInst_CreateAmbient_DC70(void* p, sCol_DC70* c, float f) __asm__("cLightInst_CreateAmbient__FPv");
void cLightInst_CreateDirectional_DC70(void* p, sCol_DC70* c, sV3_DC70* d, float f) __asm__("cLightInst_CreateDirectional__FPv");
void cLightInst_CreatePoint_DC70(void* p, sCol_DC70* c, void* pos, float f, int e) __asm__("cLightInst_CreatePoint__FPv");
void cLightInst_CreateSpot_DC70(void* p, sCol_DC70* c, void* pos, sV3_DC70* d, float x, float y, int e, float z) __asm__("cLightInst_CreateSpot__FPv");

static inline sV3_DC70 Normalize_DC70(float x, float y, float z)
{
    float d2 = x * x + y * y + z * z;
    float d;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    if (d != 0.0f) {
        float k = 1.0f / d;
        return sV3_DC70(x * k, y * k, z * k);
    }
    return sV3_DC70(x, y, z);
}

/**
 * Creates the light instances for a block of light records (directional, spot, point or ambient).
 *
 * For each record i, fetches light @p base + i through vtable slot 2 (cPS2LightMan_GetLightObj), stores the record
 * pointer in its first word and builds an ARGB colour (alpha 1.0, rgb from +0x20). The record type (+0x10) picks the
 * cLightInst creator: eNEWLight_Directional (cLightInst_CreateDirectional, normalised direction +0x2C), eNEWLight_Point
 * (cLightInst_CreatePoint, position +0x38, decay byte +0x64), eNEWLight_Spot (cLightInst_CreateSpot, cone angle pi/2 -
 * func_0031C128(+0x5C), decay and dropoff bytes +0x64/+0x65), eNEWLight_Ambient (cLightInst_CreateAmbient); intensity
 * is +0x14. On the PS2 the four cLightInst creators are empty stubs, so only the record pointer sticks. Vtable slot 5
 * of vt_00493650.
 *
 * @param lights  the light records (0x70 bytes each)
 * @param n       how many records to create
 * @param base    index of the first light to fill (On Tour: index)
 */
void cPS2LightMan::CreateLights(tLightRecord* lights, int n, int base)
{
    int i;
    for (i = 0; i < n; i++) {
        sVEntry<cLightInst*(void*, int)>* vt = (sVEntry<cLightInst*(void*, int)>*)mVtbl;
        cLightInst* obj = vt[kLightManVt_GetLightObj].fn(vthis(this, vt[kLightManVt_GetLightObj]), base + i);
        obj->mpRecord = &lights[i];
        sCol_DC70 col;
        col.a = 1.0f;
        col.r = lights[i].m_fvColor[0];
        col.g = lights[i].m_fvColor[1];
        col.b = lights[i].m_fvColor[2];
        switch (lights[i].m_eLightType) {
        case eNEWLight_Directional: {
            sV3_DC70 dir = Normalize_DC70(lights[i].m_fvDirection[0], lights[i].m_fvDirection[1],
                                          lights[i].m_fvDirection[2]);
            cLightInst_CreateDirectional_DC70(obj, &col, &dir, lights[i].m_fIntensity);
            break;
        }
        case eNEWLight_Point:
            cLightInst_CreatePoint_DC70(obj, &col, ((char*)&lights[i].m_fvWorldPos[0]), lights[i].m_fIntensity, lights[i].m_cDecayRate);
            break;
        case eNEWLight_Spot: {
            char* pos = ((char*)&lights[i].m_fvWorldPos[0]);
            sV3_DC70 dir = Normalize_DC70(lights[i].m_fvDirection[0], lights[i].m_fvDirection[1],
                                          lights[i].m_fvDirection[2]);
            cLightInst_CreateSpot_DC70(obj, &col, pos, &dir,
                    1.5707963705062866f - func_0031C128(lights[i].mSpotInnerCos), lights[i].m_fIntensity,
                    lights[i].m_cDecayRate, (float)lights[i].m_cDropOff);
            break;
        }
        case eNEWLight_Ambient:
            cLightInst_CreateAmbient_DC70(obj, &col, lights[i].m_fIntensity);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2LightMan_CreateAmbient);
#ifdef SKIP_ASM
// PORT: the unit defines cLightInst_CreateAmbient(void*) (an empty stub), but this caller
// passes (void*, int, float); bind the 3-arg call by asm label.
void cLightInst_CreateAmbient_3(void* p, int b, float f) __asm__("cLightInst_CreateAmbient__FPv");

/**
 * Makes one light an ambient light of the given colour and intensity.
 *
 * Fetches light @p light through vtable slot 2 (cPS2LightMan_GetLightObj: entry @p light of the light array at +0x4) and passes the rest
 * to the cLightInst creator cLightInst_CreateAmbient, an empty stub on the PS2. Vtable slot 9 of vt_00493650.
 *
 * @param light    index of the light to set up
 * @param colour    the colour (const tARGB&, passed by address)
 * @param intensity    the intensity
 */
void cPS2LightMan::CreateAmbient(int light, int colour, float intensity)
{
    sVEntry<cLightInst*(void*, int)>* vt = (sVEntry<cLightInst*(void*, int)>*)mVtbl;
    cLightInst* obj = vt[kLightManVt_GetLightObj].fn(vthis(this, vt[kLightManVt_GetLightObj]), light);
    cLightInst_CreateAmbient_3(obj, colour, intensity);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2LightMan_CreateDirectional);
#ifdef SKIP_ASM
// PORT: the unit defines cLightInst_CreateDirectional(void*) (an empty stub), but this caller
// passes (void*, int, int, float); bind the 4-arg call by asm label.
void cLightInst_CreateDirectional_4(void* p, int b, int c, float f) __asm__("cLightInst_CreateDirectional__FPv");

/**
 * Makes one light a directional light of the given colour, direction and intensity.
 *
 * Fetches light @p light through vtable slot 2 (cPS2LightMan_GetLightObj: entry @p light of the light array at +0x4) and passes the rest
 * to the cLightInst creator cLightInst_CreateDirectional, an empty stub on the PS2. Vtable slot 10 of vt_00493650.
 *
 * @param light    index of the light to set up
 * @param colour    the colour (const tARGB&, passed by address)
 * @param dir    the direction (const t3vect&)
 * @param intensity    the intensity
 */
void cPS2LightMan::CreateDirectional(int light, int colour, int dir, float intensity)
{
    sVEntry<cLightInst*(void*, int)>* vt = (sVEntry<cLightInst*(void*, int)>*)mVtbl;
    cLightInst* obj = vt[kLightManVt_GetLightObj].fn(vthis(this, vt[kLightManVt_GetLightObj]), light);
    cLightInst_CreateDirectional_4(obj, colour, dir, intensity);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2LightMan_CreatePoint);
#ifdef SKIP_ASM
// PORT: the unit defines cLightInst_CreatePoint(void*) (an empty stub), but this caller
// passes (void*, int, int, float, int); bind the 5-arg call by asm label.
void cLightInst_CreatePoint_5(void* p, int b, int c, float f, int d) __asm__("cLightInst_CreatePoint__FPv");

/**
 * Makes one light a point light of the given colour, position, intensity and decay.
 *
 * Fetches light @p light through vtable slot 2 (cPS2LightMan_GetLightObj: entry @p light of the light array at +0x4) and passes the rest
 * to the cLightInst creator cLightInst_CreatePoint, an empty stub on the PS2. Vtable slot 11 of vt_00493650.
 *
 * @param light    index of the light to set up
 * @param colour    the colour (const tARGB&, passed by address)
 * @param pos    the position (const t3vect&)
 * @param intensity    the intensity
 * @param decay    the decay type
 */
void cPS2LightMan::CreatePoint(int light, int colour, int pos, float intensity, int decay)
{
    sVEntry<cLightInst*(void*, int)>* vt = (sVEntry<cLightInst*(void*, int)>*)mVtbl;
    cLightInst* obj = vt[kLightManVt_GetLightObj].fn(vthis(this, vt[kLightManVt_GetLightObj]), light);
    cLightInst_CreatePoint_5(obj, colour, pos, intensity, decay);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2LightMan_CreateSpot);
#ifdef SKIP_ASM
// PORT: the unit defines cLightInst_CreateSpot(void*) (an empty stub), but this caller
// passes (void*, int, int, int, float, float, int, float); bind the call by asm label.
void cLightInst_CreateSpot_8(void* p, int b, int c, int d, float x, float y, int e, float z) __asm__("cLightInst_CreateSpot__FPv");

/**
 * Makes one light a spot light of the given colour, position, direction, angle, intensity, decay and dropoff.
 *
 * Fetches light @p light through vtable slot 2 (cPS2LightMan_GetLightObj: entry @p light of the light array at +0x4) and passes the rest
 * to the cLightInst creator cLightInst_CreateSpot, an empty stub on the PS2. Vtable slot 12 of vt_00493650.
 *
 * @param light    index of the light to set up
 * @param colour    the colour (const tARGB&, passed by address)
 * @param pos    the position (const t3vect&)
 * @param dir    the direction (const t3vect&)
 * @param x    the cone angle (On Tour Ang)
 * @param y    the intensity
 * @param decay    the decay type
 * @param z    the dropoff
 */
void cPS2LightMan::CreateSpot(int light, int colour, int pos, int dir, float x, float y, int decay, float z)
{
    sVEntry<cLightInst*(void*, int)>* vt = (sVEntry<cLightInst*(void*, int)>*)mVtbl;
    cLightInst* obj = vt[kLightManVt_GetLightObj].fn(vthis(this, vt[kLightManVt_GetLightObj]), light);
    cLightInst_CreateSpot_8(obj, colour, pos, dir, x, y, decay, z);
}
#endif

//100%
INCLUDE_ASM("render/lightman", transformPoint2D);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).
// A t4Vector with EA's constructors (ours has none); the quad helpers below build the corners with it.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

/**
 * Transforms a 2D point by a 3x3 matrix and returns it as a 4-vector with z = 0 and w = 1.
 *
 * x' = m[0] x + m[3] y + m[6]; y' = m[1] x + m[4] y + m[7] (rows at +0, +0xC, +0x18). Callers: the matrix variants
 * c2DRender_drawRect1, c2DRender_drawRotRect1, c2DRender_drawRect3, c2DRender_drawRotRect3.
 *
 * @param point  the point {x, y}
 * @param mat  the 3x3 matrix (nine floats)
 * @return {x', y', 0, 1}, through the hidden result pointer
 */
extern "C" t4VectorXYZW transformPoint2D(float* point, float* mat)
{
    t4VectorXYZW result;
    float x = point[0];
    float y = point[1];
    result.z = 0.0f;
    result.w = 1.0f;
    result.x = mat[0] * x + mat[3] * y + mat[6];
    result.y = mat[1] * x + mat[4] * y + mat[7];
    return result;
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRect);
#ifdef SKIP_ASM

struct sLmQuadVEntry_E118 {
    short delta;
    short index;
    void (*fn)(void*, int, tBxVertex*, int);
};

struct sLmQuadCtx_E118 {
    char pad_0x0[0x10D8];
    sLmQuadVEntry_E118* vtable;      // 0x10D8
};


/**
 * Fills the four vertex positions of a quad from @p rect {x, y, w, h} and draws it.
 *
 * Corners in strip order: top-left, top-right, bottom-left, bottom-right, at z 0, w 1; the caller sets colours and UVs.
 * @p self is unused. Callers: cTextureElement_render and cTextureElement_render2.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param verts  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 */
void c2DRender::drawRect(tBxVertex* verts, float* rect)
{
    verts[0].p = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    verts[1].p = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    verts[2].p = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    verts[3].p = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, verts, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRotRect);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

/**
 * Fills the four vertex positions of a quad from @p rect with the corners turned a quarter turn, and draws it.
 *
 * Vertex 0 gets the bottom-left corner, 1 the top-left, 2 the bottom-right and 3 the top-right, so colours and UVs the
 * caller set per vertex appear rotated by 90 degrees. Callers: cTextureElement_render and cTextureElement_render2.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param verts  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 */
void c2DRender::drawRotRect(tBxVertex* verts, float* rect)
{
    verts[1].p = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    verts[3].p = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    verts[0].p = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    verts[2].p = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, verts, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRect1);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

extern "C" t4VectorXYZW transformPoint2D(float* p, float* m);

/**
 * Fills the four vertex positions of a quad from @p rect, transforms them by @p mat, and draws it.
 *
 * Same corner order as c2DRender_drawRect; each corner goes through transformPoint2D. Caller: cTextureElement_renderRot.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param verts  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param mat  the 3x3 transform
 */
void c2DRender::drawRect1(tBxVertex* verts, float* rect, float* mat)
{
    t4Vector* corner0 = &verts[0].p;
    t4Vector* corner1 = &verts[1].p;
    t4Vector* corner2 = &verts[2].p;
    t4Vector* corner3 = &verts[3].p;
    *corner0 = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    *corner1 = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    *corner2 = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    *corner3 = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    *corner0 = transformPoint2D(&corner0->x, mat);
    *corner1 = transformPoint2D(&corner1->x, mat);
    *corner2 = transformPoint2D(&corner2->x, mat);
    *corner3 = transformPoint2D(&corner3->x, mat);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, verts, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRotRect1);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

extern "C" t4VectorXYZW transformPoint2D(float* p, float* m);

/**
 * Fills the four vertex positions of a quad from @p rect with the corners turned a quarter turn, transforms them by @p
 * mat, and draws it.
 *
 * Same corner order as c2DRender_drawRotRect; each corner goes through transformPoint2D. Caller: cTextureElement_renderRot.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param verts  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param mat  the 3x3 transform
 */
void c2DRender::drawRotRect1(tBxVertex* verts, float* rect, float* mat)
{
    t4Vector* corner0 = &verts[0].p;
    t4Vector* corner1 = &verts[1].p;
    t4Vector* corner2 = &verts[2].p;
    t4Vector* corner3 = &verts[3].p;
    *corner1 = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    *corner3 = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    *corner0 = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    *corner2 = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    *corner0 = transformPoint2D(&corner0->x, mat);
    *corner1 = transformPoint2D(&corner1->x, mat);
    *corner2 = transformPoint2D(&corner2->x, mat);
    *corner3 = transformPoint2D(&corner3->x, mat);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, verts, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRect2);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

/**
 * Fills the four vertex positions from @p rect and their UVs from @p uv, and draws the quad.
 *
 * Corners top-left, top-right, bottom-left, bottom-right, with the matching corners of the UV rectangle. Caller:
 * cTextureElement_renderTiled.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param vtx  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param uv  the UV rectangle {u, v, w, h}
 */
void c2DRender::drawRect2(tBxVertex* vtx, float* rect, float* uv)
{
    vtx[0].p = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    vtx[0].u = uv[0];
    vtx[0].v = uv[1];
    vtx[1].p = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    vtx[1].u = uv[0] + uv[2];
    vtx[1].v = uv[1];
    vtx[2].p = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[2].u = uv[0];
    vtx[2].v = uv[1] + uv[3];
    vtx[3].p = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[3].u = uv[0] + uv[2];
    vtx[3].v = uv[1] + uv[3];
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, vtx, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRotRect2);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

/**
 * Fills the four vertex positions from @p rect and their UVs from @p uv turned a quarter turn, and draws the quad.
 *
 * Positions as in c2DRender_drawRect2; the UVs go top-left (u + w, v), top-right (u + w, v + h), bottom-left (u, v),
 * bottom-right (u, v + h), so the texture appears rotated by 90 degrees. Caller: cTextureElement_renderTiled.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param vtx  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param uv  the UV rectangle {u, v, w, h}
 */
void c2DRender::drawRotRect2(tBxVertex* vtx, float* rect, float* uv)
{
    vtx[0].p = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    vtx[0].u = uv[0] + uv[2];
    vtx[0].v = uv[1];
    vtx[1].p = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    vtx[1].u = uv[0] + uv[2];
    vtx[1].v = uv[1] + uv[3];
    vtx[2].p = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[2].u = uv[0];
    vtx[2].v = uv[1];
    vtx[3].p = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[3].u = uv[0];
    vtx[3].v = uv[1] + uv[3];
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, vtx, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRect3);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

extern "C" t4VectorXYZW transformPoint2D(float* p, float* m);

/**
 * Fills the four vertex positions from @p rect and their UVs from @p uv, transforms the positions by @p mat, and draws
 * the quad.
 *
 * Layout as in c2DRender_drawRect2; each position goes through transformPoint2D. Caller: cTextureElement_renderRotTiled.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param vtx  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param uv  the UV rectangle {u, v, w, h}
 * @param mat  the 3x3 transform
 */
void c2DRender::drawRect3(tBxVertex* vtx, float* rect, float* uv, float* mat)
{
    t4Vector* corner0 = &vtx[0].p;
    t4Vector* corner1 = &vtx[1].p;
    t4Vector* corner2 = &vtx[2].p;
    t4Vector* corner3 = &vtx[3].p;
    *corner0 = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    vtx[0].u = uv[0];
    vtx[0].v = uv[1];
    *corner1 = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    vtx[1].u = uv[0] + uv[2];
    vtx[1].v = uv[1];
    *corner2 = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[2].u = uv[0];
    vtx[2].v = uv[1] + uv[3];
    *corner3 = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[3].u = uv[0] + uv[2];
    vtx[3].v = uv[1] + uv[3];
    *corner0 = transformPoint2D(&corner0->x, mat);
    *corner1 = transformPoint2D(&corner1->x, mat);
    *corner2 = transformPoint2D(&corner2->x, mat);
    *corner3 = transformPoint2D(&corner3->x, mat);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, vtx, 0);
}
#endif
//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRotRect3);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

extern "C" t4VectorXYZW transformPoint2D(float* p, float* m);

/**
 * Fills the four vertex positions from @p rect and their UVs from @p uv turned a quarter turn, transforms the positions
 * by @p mat, and draws the quad.
 *
 * Layout as in c2DRender_drawRotRect2; each position goes through transformPoint2D. Caller: cTextureElement_renderRotTiled.
 *
 * Submits the four vertices as a triangle strip through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param vtx  the four vertices
 * @param rect  the rectangle {x, y, w, h}
 * @param uv  the UV rectangle {u, v, w, h}
 * @param mat  the 3x3 transform
 */
void c2DRender::drawRotRect3(tBxVertex* vtx, float* rect, float* uv, float* mat)
{
    t4Vector* corner0 = &vtx[0].p;
    t4Vector* corner1 = &vtx[1].p;
    t4Vector* corner2 = &vtx[2].p;
    t4Vector* corner3 = &vtx[3].p;
    *corner0 = t4VectorXYZW(rect[0], rect[1], 0.0f, 1.0f);
    vtx[0].u = uv[0] + uv[2];
    vtx[0].v = uv[1];
    *corner1 = t4VectorXYZW(rect[0] + rect[2], rect[1], 0.0f, 1.0f);
    vtx[1].u = uv[0] + uv[2];
    vtx[1].v = uv[1] + uv[3];
    *corner2 = t4VectorXYZW(rect[0], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[2].u = uv[0];
    vtx[2].v = uv[1];
    *corner3 = t4VectorXYZW(rect[0] + rect[2], rect[1] + rect[3], 0.0f, 1.0f);
    vtx[3].u = uv[0];
    vtx[3].v = uv[1] + uv[3];
    *corner0 = transformPoint2D(&corner0->x, mat);
    *corner1 = transformPoint2D(&corner1->x, mat);
    *corner2 = transformPoint2D(&corner2->x, mat);
    *corner3 = transformPoint2D(&corner3->x, mat);
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, vtx, 0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", c2DRender_drawRect4);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sLmQuadVec4 {
    float x, y, z, w;
    sLmQuadVec4() {}
    sLmQuadVec4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sLmQuadVtx {
    float s;                // 0x0
    float t;                // 0x4
    float q;                // 0x8
    int pad_0xC;
    int c[4];               // 0x10
    sLmQuadVec4 pos;        // 0x20
};

struct sLmQuadVEntry {
    short delta;
    short index;
    void (*fn)(void*, int, sLmQuadVtx*, int);
};

struct sLmQuadCtx {
    char pad_0x0[0x10D8];
    sLmQuadVEntry* vtable;      // 0x10D8
};


/**
 * Draws a solid-colour rectangle from @p topLeft to @p bottomRight at depth @p z.
 *
 * Builds four vertices with colour R, G, B = col[1..3] * 255 and A = col[0] * 128, UVs (0, 0) to (1, 1) and Q = 1, and
 * submits them through vtable slot 71 (cPS2GraphicsMan_RenderTriStrip). Callers: cHeartBeat_draw and
 * cFadeScreen_drawFade.
 *
 * @param topLeft  the top-left corner {x, y}
 * @param bottomRight  the bottom-right corner {x, y}
 * @param col  the colour {a, r, g, b} in 0..1
 * @param z  the depth
 */
void c2DRender::drawRect4(float* topLeft, float* bottomRight, float* col, float z)
{
    sLmQuadVtx verts[4];
    float zero = 0.0f;
    float one = 1.0f;
    verts[0].q = one;
    verts[0].c[0] = (int)(col[1] * 255.0f);
    verts[0].c[1] = (int)(col[2] * 255.0f);
    verts[0].c[2] = (int)(col[3] * 255.0f);
    verts[0].c[3] = (int)(col[0] * 128.0f);
    verts[0].pos = sLmQuadVec4(topLeft[0], topLeft[1], z, one);
    verts[3] = verts[0];
    verts[2] = verts[0];
    verts[1] = verts[0];
    verts[3].pos.x = bottomRight[0];
    verts[1].pos.x = bottomRight[0];
    verts[3].pos.y = bottomRight[1];
    verts[2].pos.y = bottomRight[1];
    verts[0].t = zero;
    verts[0].s = zero;
    verts[1].s = one;
    verts[1].t = zero;
    verts[2].s = zero;
    verts[2].t = one;
    verts[3].s = one;
    verts[3].t = one;
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ps2GraphicsMan->RenderTriStrip(4, (tBxVertex*)verts, 0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", c2DRender_drawLine);
#ifdef SKIP_ASM
// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).

/**
 * Draws a line from @p start to @p end in colour @p col.
 *
 * Both vertices get R, G, B = col[1..3] * 255 and A = col[0] * 128 and go to vtable slot 73 (cPS2GraphicsMan_RenderLineStrip) with a
 * count of 2. No caller.
 *
 * @param start  the first end point
 * @param end  the second end point
 * @param col  the colour {a, r, g, b} in 0..1
 */
void c2DRender::drawLine(t4Vector* start, t4Vector* end, float* col)
{
    tBxVertex verts[2];
    int red = (int)(col[1] * 255.0f);
    int green = (int)(col[2] * 255.0f);
    int blue = (int)(col[3] * 255.0f);
    int alpha = (int)(col[0] * 128.0f);
    verts[0].p = *start;
    verts[1].p = *end;
    verts[1].r = red;
    verts[1].g = green;
    verts[1].b = blue;
    verts[1].a = alpha;
    verts[0].r = red;
    verts[0].g = green;
    verts[0].b = blue;
    verts[0].a = alpha;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    ctx->RenderLineStrip(2, verts, 0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPlants_update);
#ifdef SKIP_ASM
extern "C" float fSin(float a);
extern "C" float fCos(float a);
extern "C" void fSinCos(float* s, float* c, float angle);


struct sLmVEntry_EE78 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sLmCtx_EE78 {
    char pad_0x0[0x10D8];
    sLmVEntry_EE78* vtable;      // 0x10D8
};

extern t4Matrix cPlants_m_meshDeformationMatrixList[];
extern t4Matrix t4mat_Identity;
extern float t4vect_ZAxis[3];
extern int cPlants_m_SwayFrequency;
extern float cPlants_m_SwayAmplitude;
extern float cPlants_m_SwayOffset;
extern float cPlants_m_BranchAmplitude;
extern float cPlants_m_RotationAmplitude;
extern float cPlants_m_RotationFrequency;
extern float cPlants_m_BranchFrequency;
extern float cPlants_m_BranchFrequency2;
extern float cPlants_m_SwayDirection;
extern float cPlants_m_BranchDirection;

// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
static inline void lmCopy_EE78(void* dst, const void* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply); the PC port needs a plain matrix multiply.
static inline void lmMul_EE78(void* out, const void* a, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(out), "r"(a), "r"(b)
        : "memory");
}


// PORT: PS2-only VU0 inline asm (in-place 4x4 matrix multiply m = m * b); the PC port needs a plain matrix multiply.
static inline void lmMulIn_EE78(void* m, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(b)
        : "memory");
}

/**
 * Rebuilds the eight plant sway matrices cPlants_m_meshDeformationMatrixList[0..7] from the cPlants tweak values and the frame count.
 *
 * With t = vtable slot 114 (getCurrentFrameCount) and f = i / 8 for matrix i: sway = cPlants_m_SwayAmplitude ("Sway Amplitude") *
 * sin(f + t * cPlants_m_SwayFrequency / 600) + cPlants_m_SwayOffset; branch = cPlants_m_BranchAmplitude * sin(f * cPlants_m_BranchFrequency2 / 600 + sin(f + t * cPlants_m_BranchFrequency /
 * 600)); angle = cPlants_m_RotationAmplitude * sin(f + t * cPlants_m_RotationFrequency / 600).
 *
 * Each matrix is S * R (row vectors): S is an identity whose z row adds sway * (sin, cos) of cPlants_m_SwayDirection ("Sway
 * Direction", degrees) to x and y, so a point moves in proportion to its height; R is a rotation by angle about
 * t4vect_ZAxis whose z column adds branch * (x sin + y cos) of cPlants_m_BranchDirection ("Branch Direction"). Note: cPlants_m_BranchFrequency2
 * multiplies f rather than the time, so the outer branch sine moves only through its inner sine. Slot 2 of vtable
 * cPlants_vtbl (the object allocated as "Plants").
 */
extern "C" void cPlants_update(void)
{
    t4Matrix branchMat;
    t4Matrix rotMat;
    t4Matrix product;
    float sinAngle;
    float cosAngle;
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    float frameClock = (float)ps2GraphicsMan->getCurrentFrameCount();
    int i;
    for (i = 0; i < 8; i++) {
        t4Matrix* pm = &branchMat;
        float fi = (float)i * 0.125f;
        float amp = cPlants_m_SwayAmplitude * fSin(fi + frameClock * (float)cPlants_m_SwayFrequency * 0.0016666667f) + cPlants_m_SwayOffset;
        float branchPhase = fSin(fi + frameClock * cPlants_m_BranchFrequency * 0.0016666667f);
        float amp2 = cPlants_m_BranchAmplitude * fSin(fi * cPlants_m_BranchFrequency2 * 0.0016666667f + branchPhase);
        float angle = cPlants_m_RotationAmplitude * fSin(fi + frameClock * cPlants_m_RotationFrequency * 0.0016666667f);
        lmCopy_EE78(&branchMat, &t4mat_Identity);
        fSinCos(&sinAngle, &cosAngle, angle);
        float tc = 1.0f - cosAngle;
        float x = t4vect_ZAxis[0];
        float y = t4vect_ZAxis[1];
        float z = t4vect_ZAxis[2];
        float tx = tc * x;
        float ty = tc * y;
        float tz = tc * z;
        float sx = sinAngle * x;
        float sy = sinAngle * y;
        float sz = sinAngle * z;
        rotMat.row[0].x = tx * x + cosAngle;
        rotMat.row[1].x = tx * y + sz;
        rotMat.row[2].x = tx * z - sy;
        rotMat.row[0].y = ty * x - sz;
        rotMat.row[1].y = ty * y + cosAngle;
        rotMat.row[2].y = ty * z + sx;
        rotMat.row[0].z = tz * x + sy;
        rotMat.row[1].z = tz * y - sx;
        rotMat.row[2].z = tz * z + cosAngle;
        rotMat.row[3].x = 0.0f;
        rotMat.row[3].y = 0.0f;
        rotMat.row[3].z = 0.0f;
        rotMat.row[0].w = 0.0f;
        rotMat.row[1].w = 0.0f;
        rotMat.row[2].w = 0.0f;
        rotMat.row[3].w = 1.0f;
        lmMulIn_EE78(pm, &rotMat);
        t4Matrix* cell = &cPlants_m_meshDeformationMatrixList[i];
        lmCopy_EE78(cell, &t4mat_Identity);
        cell->row[2].x += fSin(cPlants_m_SwayDirection * 0.017453294f) * amp;
        cell->row[2].y += fCos(cPlants_m_SwayDirection * 0.017453294f) * amp;
        branchMat.row[0].z += amp2 * fSin(cPlants_m_BranchDirection * 0.017453294f);
        branchMat.row[1].z += amp2 * fCos(cPlants_m_BranchDirection * 0.017453294f);
        lmMul_EE78(&product, cell, &branchMat);
        lmCopy_EE78(&rotMat, &product);
        lmCopy_EE78(cell, &rotMat);
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPlants_getDeformationMatrix);
#ifdef SKIP_ASM
extern float cPlants_m_PositionScale;

/**
 * Picks the plant sway matrix for a position: cPlants_m_meshDeformationMatrixList[(int)(cPlants_m_PositionScale * sqrt(x * x + y * y)) % 8].
 *
 * cPlants_m_PositionScale is the "Position Scale" tweak. Caller: cGraphicsMan_DeformMeshMatrix.
 *
 * @param pos  the position (x and y are used)
 * @return the address of one of the eight matrices cPlants_update builds
 */
extern "C" t4Matrix* cPlants_getDeformationMatrix(float* pos)
{
    float distSq = pos[0] * pos[0] + pos[1] * pos[1];
    float dist;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(dist) : "f"(distSq));
    int i = (int)(cPlants_m_PositionScale * dist);
    return &cPlants_m_meshDeformationMatrixList[i % 8];
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_setLayout);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void dma_waitFromScratchpadIdle(void);
extern "C" void dma_waitToScratchpadIdle(void);
// PORT: the unit declares cPS2ScratchPad_freeSpace as int further down; bind a pointer view by asm label
extern char* cPS2ScratchPad_freeSpace_F300 __asm__("cPS2ScratchPad_freeSpace");


/**
 * Splits the scratchpad into the buffers of the DMA ring for @p mode.
 *
 * Does nothing when the mode is unchanged. Modes kLayout_1x1024 to kLayout_3x256 give 1, 2, 2, 2, 2, 3, 3 buffers of
 * 0x400, 0x200, 0x1C0, 0x180, 0x100, 320, 0x100 qwords at 0x70000000 + i * size * 16; both indices and all addresses
 * and sizes are reset. cPS2ScratchPad_freeSpace gets the scratchpad space after the buffers, or 0 when none is left. Then it waits
 * for both scratchpad DMA channels (dma_waitFromScratchpadIdle, dma_waitToScratchpadIdle). The ring object (SPRMan): mode +0, buffer count
 * +4, next buffer +8, current buffer +0xC, default size in qwords +0x10, scratchpad buffers +0x14[3], main-memory
 * addresses +0x20[3], sizes +0x2C[3].
 *
 * Callers: __sti__all_in_one_render_cpp and cPS2DeferManager_buildDmaChains.
 *
 * @param mode  the layout, cPS2ScratchPad::eLayout
 */
void cPS2ScratchPad::setLayout(int mode)
{
    // ORIGINAL-PTR: the EE scratchpad RAM (SPR) sits at the fixed address 0x70000000 (EE User's Manual); the buffers
    // are carved out of it from that base.
    int i;
    if (unk_00 == mode) {
        return;
    }
    unk_00 = mode;
    switch (mode) {
    case cPS2ScratchPad::kLayout_1x1024: mNumBlocks = 1; break;
    case cPS2ScratchPad::kLayout_2x512: mNumBlocks = 2; break;
    case cPS2ScratchPad::kLayout_2x448: mNumBlocks = 2; break;
    case cPS2ScratchPad::kLayout_2x384: mNumBlocks = 2; break;
    case cPS2ScratchPad::kLayout_2x256: mNumBlocks = 2; break;
    case cPS2ScratchPad::kLayout_3x320:
    case cPS2ScratchPad::kLayout_3x256:
        mNumBlocks = 3;
        break;
    }
    unk_08 = 0;
    mDMABlock = 0;
    switch (mode) {
    case cPS2ScratchPad::kLayout_1x1024: mBlockSize = 0x400; break;
    case cPS2ScratchPad::kLayout_2x512: mBlockSize = 0x200; break;
    case cPS2ScratchPad::kLayout_2x448: mBlockSize = 0x1C0; break;
    case cPS2ScratchPad::kLayout_2x384: mBlockSize = 0x180; break;
    case cPS2ScratchPad::kLayout_3x320: mBlockSize = 320; break;
    case cPS2ScratchPad::kLayout_2x256:
    case cPS2ScratchPad::kLayout_3x256:
        mBlockSize = 0x100;
        break;
    }
    for (i = 0; i < mNumBlocks; i++) {
        mBlockAddr[i] = (char*)0x70000000 + mBlockSize * i * 16;
        ((int*)&mBlockMainAddr[0])[i] = 0;
        unk_2C[i] = 0;
    }
    int total = mBlockSize * mNumBlocks;
    if (total < 0x400) {
        cPS2ScratchPad_freeSpace_F300 = (char*)0x70000000 + total * 16;
    } else {
        cPS2ScratchPad_freeSpace_F300 = 0;
    }
    dma_waitFromScratchpadIdle();
    dma_waitToScratchpadIdle();
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_claimBuffer);
#ifdef SKIP_ASM
struct sLightRing {
    int field_0x0;
    int count;        // 0x4
    int next;         // 0x8
    int field_0xC;
    int defSize;      // 0x10
    int result[3];    // 0x14
    unsigned int addr[3]; // 0x20
    int size[3];      // 0x2C
};

extern "C" void dma_waitFromScratchpadIdle(void);

/**
 * Claims the ring's next scratchpad buffer for data that will go to main-memory address @p addr.
 *
 * Records @p addr and the size (in qwords; the default size when @p size is negative) for the buffer and advances the
 * next index. With @p flags & 4 it first waits for the fromSPR channel (dma_waitFromScratchpadIdle).
 *
 * @param addr  the main-memory address the buffer will be copied to
 * @param size  the size in bytes, or -1
 * @param flags  4: wait for the fromSPR channel first
 * @return the scratchpad address of the claimed buffer
 */
int cPS2ScratchPad::claimBuffer(unsigned int addr, int size, int flags)
{
    sLightRing* ring = (sLightRing*)this;
    if (flags & 4) {
        dma_waitFromScratchpadIdle();
    }
    if (size < 0) {
        size = ring->defSize;
    } else {
        size >>= 4;
    }
    int i = ring->next++;
    ring->next %= ring->count;
    ring->addr[i] = addr;
    ring->size[i] = size;
    return ring->result[i];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", cPS2ScratchPad_claimAndLoadBuffer);
#ifdef SKIP_ASM
extern "C" int cPS2ScratchPad_claimBuffer(void* self, unsigned int addr, int size, int flags);
extern "C" void dma_copyToScratchpad(unsigned int madr, int sadr, int qwc, int flags);

/**
 * Claims the ring's next scratchpad buffer (cPS2ScratchPad_claimBuffer) and fills it from @p addr by toSPR DMA.
 *
 * The copy is dma_copyToScratchpad(@p addr, buffer, @p size >> 4, @p flags).
 *
 * @param addr  the main-memory source
 * @param size  the size in bytes
 * @param flags  dma_copyToScratchpad's wait flags
 * @return the scratchpad address of the claimed buffer
 */
int cPS2ScratchPad::claimAndLoadBuffer(unsigned int addr, int size, int flags)
{
    int slot = cPS2ScratchPad_claimBuffer(this, addr, size, 0);
    dma_copyToScratchpad(addr, slot, size >> 4, flags);
    return slot;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_flushCurrentBuffer);
#ifdef SKIP_ASM

// PORT: cPS2ScratchPad_flushBuffer takes (self, size, flags) and returns the buffer address.
unsigned int cPS2ScratchPad_flushBuffer_f560(cPS2ScratchPad* self, int size, int flags) __asm__("cPS2ScratchPad_flushBuffer");

/**
 * Flushes the ring's current buffer with the size recorded for it (cPS2ScratchPad_flushBuffer).
 *
 * @param flags  cPS2ScratchPad_flushBuffer's wait flags
 * @return cPS2ScratchPad_flushBuffer's result: the main-memory address after the copied data
 */
unsigned int cPS2ScratchPad::flushCurrentBuffer(int flags)
{
    return cPS2ScratchPad_flushBuffer_f560(this, unk_2C[mDMABlock] << 4, flags);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_flushBuffer);
#ifdef SKIP_ASM
extern "C" void dma_waitFromScratchpadIdle(void);
extern "C" void cPS2ScratchPad_advanceBuffer(cPS2ScratchPad* self);
extern "C" void dma_copyFromScratchpad(int sadr, unsigned int madr, int qwc, int flags);

// PORT: the unit declares cPS2ScratchPad_flushBuffer as `void (sLmStack*, int, int, int)`; the body takes
// (self, size, flags) and returns the buffer address, so it is bound by asm label.
unsigned int cPS2ScratchPad_flushBuffer_impl(cPS2ScratchPad* self, int size, int flags) __asm__("cPS2ScratchPad_flushBuffer");

/**
 * Copies @p size bytes of the ring's current scratchpad buffer to its main-memory address by fromSPR DMA and moves to
 * the next buffer.
 *
 * With @p size 0 it only waits for the fromSPR channel (when @p flags & 6 is non-zero), returns the recorded address
 * and advances (cPS2ScratchPad_advanceBuffer). Otherwise it runs sync, calls dma_copyFromScratchpad(buffer, address, @p size >> 4, @p flags)
 * and advances the current index.
 *
 * @param size  the byte count (a multiple of 16), or 0
 * @param flags  dma_copyFromScratchpad's wait flags
 * @return the main-memory address just past the copied data
 */
unsigned int cPS2ScratchPad::flushBuffer(int size, int flags)
{
    if (size == 0) {
        if (flags & 6) {
            dma_waitFromScratchpadIdle();
        }
        unsigned int retAddr = mBlockMainAddr[mDMABlock];
        advanceBuffer();
        return retAddr;
    }
    size >>= 4;
    // PORT: EE sync (memory barrier before kicking the DMA)
    __asm__ __volatile__("sync");
    dma_copyFromScratchpad((int)mBlockAddr[mDMABlock], mBlockMainAddr[mDMABlock], size, flags);
    int cur = mDMABlock;
    unsigned int retAddr = mBlockMainAddr[cur] + (size << 4);
    mDMABlock = (cur + 1) % mNumBlocks;
    return retAddr;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", cPS2ScratchPad_flushBufferTo);
#ifdef SKIP_ASM
struct sLmStack {
    char pad_0x00[0xC];
    int depth;          // 0x0C
    char pad_0x10[4];
    int marks[1];       // 0x14
};

extern "C" void cPS2ScratchPad_flushBuffer(sLmStack* self, int size, int arg, int base);

/**
 * Flushes the ring's current buffer up to @p end, rounded up to 16 bytes, through cPS2ScratchPad_flushBuffer.
 *
 * Declared void, but it tail-calls cPS2ScratchPad_flushBuffer and leaves that result (the main-memory address after the data) in v0;
 * its callers bind an int-returning view to read it. Passes the buffer start as a fourth, unused argument.
 *
 * @param end  the scratchpad write pointer at the end of the data
 * @param flags  cPS2ScratchPad_flushBuffer's wait flags
 */
extern "C" void cPS2ScratchPad_flushBufferTo(sLmStack* self, int end, int flags)
{
    int base = self->marks[self->depth];
    int size = end - base;
    cPS2ScratchPad_flushBuffer(self, size + (-size & 0xF), flags, base);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_advanceBuffer);
#ifdef SKIP_ASM
/**
 * Advances the ring's current-buffer index: mDMABlock = (mDMABlock + 1) % mNumBlocks.
 */
void cPS2ScratchPad::advanceBuffer()
{
    mDMABlock = (mDMABlock + 1) % mNumBlocks;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2ScratchPad_mapToMainAddress);
#ifdef SKIP_ASM
struct sLightDma_F6D0 {
    int field_0x0;
    int count;              // 0x4
    int next;               // 0x8
    int cur;                // 0xC
    int defSize;            // 0x10
    int result[3];          // 0x14
    unsigned int addr[3];   // 0x20
};

/**
 * Maps a scratchpad write pointer in the current buffer to the main-memory address it will be copied to.
 *
 * @param end  a scratchpad address inside the current buffer
 * @return the buffer's main-memory address plus the offset of @p end rounded up to 16 bytes
 */
extern "C" unsigned int cPS2ScratchPad_mapToMainAddress(sLightDma_F6D0* self, int end)
{
    int cur = self->cur;
    int size = end - self->result[cur];
    return self->addr[cur] + (((size + (-size & 0xF)) >> 4) << 4);
}
#endif

//100%
INCLUDE_ASM("render/lightman", scratchpad_loadSpareArea);
#ifdef SKIP_ASM
extern int cPS2ScratchPad_freeSpace;
extern "C" void dma_copyToScratchpad(unsigned int madr, int sadr, int qwc, int flags);

/**
 * Copies @p size bytes from main memory into the spare scratchpad area cPS2ScratchPad_freeSpace by toSPR DMA.
 *
 * dma_copyToScratchpad(@p madr, cPS2ScratchPad_freeSpace, @p size >> 4, @p flags).
 *
 * @param madr  the main-memory source
 * @param size  the byte count
 * @param flags  dma_copyToScratchpad's wait flags
 * @return cPS2ScratchPad_freeSpace
 */
extern "C" int scratchpad_loadSpareArea(unsigned int madr, int size, int flags)
{
    dma_copyToScratchpad(madr, cPS2ScratchPad_freeSpace, size >> 4, flags);
    return cPS2ScratchPad_freeSpace;
}
#endif

//100%
INCLUDE_ASM("render/lightman", scratchpad_storeSpareArea);
#ifdef SKIP_ASM
extern int cPS2ScratchPad_freeSpace;
extern "C" void dma_copyFromScratchpad(int sadr, unsigned int madr, int qwc, int flags);

/**
 * Copies @p size bytes from the spare scratchpad area cPS2ScratchPad_freeSpace to main memory by fromSPR DMA.
 *
 * dma_copyFromScratchpad(cPS2ScratchPad_freeSpace, @p madr, @p size >> 4, @p flags).
 *
 * @param madr  the main-memory destination
 * @param size  the byte count
 * @param flags  dma_copyFromScratchpad's wait flags
 */
extern "C" void scratchpad_storeSpareArea(unsigned int madr, int size, int flags)
{
    dma_copyFromScratchpad(cPS2ScratchPad_freeSpace, madr, size >> 4, flags);
}
#endif

//100%
INCLUDE_ASM("render/lightman", dma_waitFromScratchpadIdle);
#ifdef SKIP_ASM
/**
 * Waits until the fromSPR DMA channel (D8, CHCR 0x1000D000) is idle.
 */
// PORT: polls PS2 hardware register 0x1000D000 directly; needs a platform shim.
extern "C" void dma_waitFromScratchpadIdle(void)
{
    while (*(volatile int*)0x1000D000 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", dma_waitToScratchpadIdle);
#ifdef SKIP_ASM
/**
 * Waits until the toSPR DMA channel (D9, CHCR 0x1000D400) is idle.
 */
// PORT: polls PS2 hardware register 0x1000D400 directly; needs a platform shim.
extern "C" void dma_waitToScratchpadIdle(void)
{
    while (*(volatile int*)0x1000D400 & 0x100) {
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", cVisualEffectsTest_update__FPv);
#ifdef SKIP_ASM
/**
 * Per-frame update of the debug visual-effects test object: does nothing.
 *
 * Slot 2 of vt_00492FD0 (cVisualEffect's update slot); On Tour's update is empty too.
 */
void cVisualEffectsTest_update(void* self)
{
}
#endif

extern "C" void* cWorldPainterManRender(void* self);

//100%
INCLUDE_ASM("render/lightman", cVisualEffectsTest_render__FPv);
#ifdef SKIP_ASM
/**
 * Render pass of the debug visual-effects test object: renders the world painter.
 *
 * Only calls cWorldPainterManRender (On Tour cWorldPainterManRender). Slot 3 of vt_00492FD0 (cVisualEffect's render slot).
 *
 * @return cWorldPainterManRender's result as this unit declares it (On Tour's render returns nothing)
 */
void* cVisualEffectsTest_render(void* self)
{
    return cWorldPainterManRender(self);
}
#endif

//100%
INCLUDE_ASM("render/lightman", tex_reorder4BitBlock);
#ifdef SKIP_ASM
#include "fakematch.h"
#include "render/graphicsman.h"
extern unsigned int tex_reorder4BitTable[];

/**
 * Reorders one 256-byte block of 4-bit texels by the nibble table tex_reorder4BitTable.
 *
 * Four passes of 64 source bytes; each output byte packs two nibbles picked by table entries (bit 0 = which nibble, the
 * rest = byte index), and odd passes use the table's second half. @p self is unused. Caller: tex_rearrange4BitTile.
 *
 * @param src  the source block
 * @param dst  the 256-byte output
 * @return 0
 */
// FAKEMATCH: variable reuse (`a` holds both table entries, `x` both byte indices, so each second value shares its
// first value's register and sched1 keeps the second load after the first value's last use) plus a register pin
// `register unsigned char* d FM_REG("$16") = dst;` (fakematch.h; global-alloc tie between dst and the outer i+1);
// plain form (separate a/b and index locals, no pin) 77.33%, reuse without the pin 96.45%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int tex_reorder4BitBlock(void* self, unsigned char* src, unsigned char* dst)
{
    register unsigned char* outBytes FM_REG("$16") = dst;
    unsigned int n = 0;
    unsigned int i;
    for (i = 0; i < 4; i++) {
        unsigned int k = (i & 1) << 7;
        unsigned int j;
        for (j = 0; j < 16; j++) {
            unsigned int* tablePtr = &tex_reorder4BitTable[k];
            unsigned int column;
            for (column = 0; column < 4; column++) {
                unsigned int entry = *tablePtr++;
                int sa = (entry & 1) * 4;
                unsigned int x = entry >> 1;
                unsigned char* pa = &src[x];
                entry = *tablePtr++;
                k += 2;
                int sb = (entry & 1) * 4;
                x = entry >> 1;
                int lo = (*pa & (0xF << sa)) >> sa;
                unsigned char hi = (src[x] & (0xF << sb)) >> sb;
                outBytes[n] = lo | ((hi * 0x10) & ~0xF);
                n++;
            }
        }
        src += 0x40;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/lightman", tex_reorder8BitBlock);
#ifdef SKIP_ASM
extern int tex_reorder8BitTable[];

/**
 * Reorders one 256-byte block of 8-bit texels by the index table tex_reorder8BitTable.
 *
 * Four passes of 64 source bytes, odd passes using the table's second half. @p self is unused. Caller: tex_rearrange8BitTile.
 *
 * @param src  the source block
 * @param dst  the 256-byte output
 * @return 0
 */
extern "C" int tex_reorder8BitBlock(void* self, unsigned char* src, unsigned char* dst)
{
    unsigned int n = 0;
    unsigned int i;
    for (i = 0; i < 4; i++) {
        unsigned int k = (i & 1) * 64;
        unsigned int j;
        for (j = 0; j < 16; j++) {
            unsigned int column;
            for (column = 0; column < 4; column++) {
                dst[n] = src[tex_reorder8BitTable[k]];
                k++;
                n++;
            }
        }
        src += 0x40;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/lightman", tex_rearrange4BitTile);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern int D_0044C060[];
extern int D_0044BFE0[];
extern "C" int tex_reorder4BitBlock(void* self, unsigned char* src, unsigned char* dst);
extern "C" void* memset(void* dst, int c, int n);

struct sRow16_F9C0 {
    unsigned char b[16];
};

struct sRow32_F9C0 {
    unsigned char b[32];
};

/**
 * Rearranges a 4-bit texture block by block: each 32x16 block is reordered with tex_reorder4BitBlock and placed where the
 * tables D_0044BFE0 and D_0044C060 say.
 *
 * Reads @p w / 32 by @p height / 16 blocks from @p src (row pitch 0x40 bytes) and writes 8 rows of 32 bytes per block to @p
 * dst (row pitch 0x100). The pitches are fixed, so it only fits 128-texel-wide images. Caller: tex_rearrange4BitTexture (128x128
 * tiles).
 *
 * @param w  the width in texels
 * @param height  the height in texels
 * @param src  the source texels
 * @param dst  the output
 * @return 0
 */
extern "C" int tex_rearrange4BitTile(void* self, int w, int height, unsigned char* src, unsigned char* dst)
{
    int xmap[32];
    int ymap[32];
    unsigned char block[0x100];
    unsigned char out[0x100];
    int n = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            int slot = D_0044C060[n++];
            xmap[slot] = j;
            ymap[slot] = i;
        }
    }
    int bw = w / 32;
    int bh = height / 16;
    int spitch = 0x40;
    int dpitch = 0x100;
    memset(block, 0, 0x100);
    memset(out, 0, 0x100);
    for (i = 0; i < bh; i++) {
        for (j = 0; j < bw; j++) {
            int id = D_0044BFE0[i * bw + j];
            sRow16_F9C0* srcRow = (sRow16_F9C0*)(src + (i * 16) * spitch + j * 16);
            sRow16_F9C0* blockRow = (sRow16_F9C0*)block;
            for (k = 0; k < 16; k++) {
                *blockRow = *srcRow;
                blockRow++;
                srcRow += 4;
            }
            tex_reorder4BitBlock(self, block, out);
            sRow32_F9C0* outRow = (sRow32_F9C0*)out;
            sRow32_F9C0* dstRow = (sRow32_F9C0*)(dst + (ymap[id] * 8) * dpitch + xmap[id] * 32);
            for (k = 0; k < 8; k++) {
                *dstRow = *outRow;
                outRow++;
                dstRow += 8;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/lightman", tex_rearrange8BitTile);
#ifdef SKIP_ASM
struct sRow16_FC48 {
    unsigned char b[16];
};

struct sRow32_FC48 {
    unsigned char b[32];
};

extern int D_0044C160[];
extern int D_0044C0E0[];
extern "C" void* memset(void* dst, int c, int n);

/**
 * Rearranges an 8-bit texture block by block: each 16x16 block is reordered with tex_reorder8BitBlock and placed where the
 * tables D_0044C0E0 and D_0044C160 say.
 *
 * Reads @p w / 16 by @p height / 16 blocks from @p src (row pitch 0x80 bytes) and writes 8 rows of 32 bytes per block to @p
 * dst (row pitch 0x100). Caller: tex_rearrange8BitTexture (128x64 tiles).
 *
 * @param w  the width in texels
 * @param height  the height in texels
 * @param src  the source texels
 * @param dst  the output
 * @return 0
 */
extern "C" int tex_rearrange8BitTile(void* self, int w, int height, unsigned char* src, unsigned char* dst)
{
    int xmap[32];
    int ymap[32];
    unsigned char block[0x100];
    unsigned char out[0x100];
    int n = 0;
    int i;
    int j;
    int k;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            int slot = D_0044C160[n++];
            xmap[slot] = j;
            ymap[slot] = i;
        }
    }
    int bw = w / 16;
    int bh = height / 16;
    int spitch = 0x80;
    int dpitch = 0x100;
    memset(block, 0, 0x100);
    memset(out, 0, 0x100);
    for (i = 0; i < bh; i++) {
        for (j = 0; j < bw; j++) {
            int id = D_0044C0E0[i * bw + j];
            sRow16_FC48* srcRow = (sRow16_FC48*)(src + (i * 16) * spitch + j * 16);
            sRow16_FC48* blockRow = (sRow16_FC48*)block;
            for (k = 0; k < 16; k++) {
                *blockRow = *srcRow;
                blockRow++;
                srcRow += 8;
            }
            tex_reorder8BitBlock(self, block, out);
            sRow32_FC48* outRow = (sRow32_FC48*)out;
            sRow32_FC48* dstRow = (sRow32_FC48*)(dst + (ymap[id] * 8) * dpitch + xmap[id] * 32);
            for (k = 0; k < 8; k++) {
                *dstRow = *outRow;
                outRow++;
                dstRow += 8;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/lightman", tex_rearrange4BitTexture);
#ifdef SKIP_ASM
extern "C" void* func_0041605C(void* dst, const void* src, int n);

/**
 * Rearranges a 4-bit texture whose sides are powers of two from 1 to 1024, in 128x128 tiles through tex_rearrange4BitTile.
 *
 * Each tile is gathered into a buffer at pitch 0x40, rearranged, and copied out at half the rows and twice the pitch.
 * No caller.
 *
 * @param w  the width in texels
 * @param height  the height in texels
 * @param src  the source texels
 * @param dst  the output
 * @return 0, or -1 when a side is not a power of two up to 1024
 */
extern "C" int tex_rearrange4BitTexture(void* self, int w, int height, unsigned char* src, unsigned char* dst)
{
    unsigned char tile[0x2000];
    unsigned char out[0x2000];
    int i;
    int j;
    int k;

    for (i = 0; i < cTextureMan::kNumTexSides; i++) {
        if (w == (0x400 >> i))
            break;
    }
    if (i == cTextureMan::kNumTexSides)
        return -1;
    memset(tile, 0, 0x2000);
    memset(out, 0, 0x2000);
    for (i = 0; i < cTextureMan::kNumTexSides; i++) {
        if (height == (0x400 >> i))
            break;
    }
    if (i == cTextureMan::kNumTexSides)
        return -1;

    int nx = (w - 1) / 128 + 1;
    int ny = (height - 1) / 128 + 1;
    int srow;
    int nrows;
    int drows;
    int drow;
    if (nx == 1) {
        srow = w / 2;
        drows = w / 4;
    } else {
        srow = 0x40;
        drows = 0x20;
    }
    if (ny == 1) {
        nrows = height;
        drow = height * 2;
    } else {
        drow = 0x100;
        nrows = 0x80;
    }

    for (i = 0; i < ny; i++) {
        for (j = 0; j < nx; j++) {
            unsigned char* tileDst = tile;
            unsigned char* srcRow = src + srow * (nx << 7) * i + srow * j;
            for (k = 0; k < nrows; k++) {
                func_0041605C(tileDst, srcRow, srow);
                srcRow += srow * nx;
                tileDst += 0x40;
            }
            tex_rearrange4BitTile(self, 0x80, 0x80, tile, out);
            unsigned char* outRow = out;
            unsigned char* dstRow = dst + drow * (nx << 5) * i + drow * j;
            for (k = 0; k < drows; k++) {
                func_0041605C(dstRow, outRow, drow);
                dstRow += drow * nx;
                outRow += 0x100;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/lightman", tex_rearrange8BitTexture);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// (prototype from their definition at line 1223)
extern "C" int tex_rearrange8BitTile_0198(void* self, int w, int h, unsigned char* src, unsigned char* dst) __asm__("tex_rearrange8BitTile");

extern "C" void* func_0041605C_0198(void* dst, void* src, int n) __asm__("func_0041605C");
extern "C" void* memset(void* dst, int c, int n);

/**
 * Rearranges an 8-bit texture whose sides are powers of two from 1 to 1024, in 128x64 tiles through tex_rearrange8BitTile.
 *
 * Each tile is gathered into a buffer at pitch 0x80, rearranged, and copied out at half the rows and twice the pitch.
 * Caller: cTextureMan_rearrangeMipLevels.
 *
 * @param w  the width in texels
 * @param height  the height in texels
 * @param src  the source texels
 * @param dst  the output
 * @return 0, or -1 when a side is not a power of two up to 1024
 */
extern "C" int tex_rearrange8BitTexture(cTextureMan* self, int w, int height, unsigned char* src, unsigned char* dst)
{
    unsigned char bufA[0x2000];
    unsigned char bufB[0x2000];
    int i;
    int j;
    int k;
    for (i = 0; i < cTextureMan::kNumTexSides; i++) {
        if (w == (0x400 >> i))
            break;
    }
    if (i == cTextureMan::kNumTexSides)
        return -1;
    for (i = 0; i < cTextureMan::kNumTexSides; i++) {
        if (height == (0x400 >> i))
            break;
    }
    if (i == cTextureMan::kNumTexSides)
        return -1;
    memset(bufA, 0, 0x2000);
    memset(bufB, 0, 0x2000);
    int qw = (w - 1) / 128;
    int rows = (height - 1) / 64 + 1;
    int cols = qw + 1;
    int bw;
    int bs;
    int th;
    int th2;
    if (qw == 0) {
        bw = w;
        bs = bw * 2;
    } else {
        bw = 0x80;
        bs = 0x100;
    }
    if (rows == 1) {
        th = height;
        th2 = height / 2;
    } else {
        th = 0x40;
        th2 = 0x20;
    }
    for (i = 0; i < rows; i++) {
        int colsTimes64 = cols << 6;
        int sstep = bw * colsTimes64;
        for (j = 0; j < cols; j++) {
            unsigned char* srcRow = src + sstep * i + bw * j;
            unsigned char* bufARow = bufA;
            for (k = 0; k < th; k++) {
                func_0041605C_0198(bufARow, srcRow, bw);
                srcRow += bw * cols;
                bufARow += 0x80;
            }
            tex_rearrange8BitTile_0198(self, 0x80, 0x40, bufA, bufB);
            int dstep = bs * th2 * cols;
            unsigned char* dstRow = dst + dstep * i + bs * j;
            unsigned char* bufBRow = bufB;
            for (k = 0; k < th2; k++) {
                func_0041605C_0198(dstRow, bufBRow, bs);
                dstRow += bs * cols;
                bufBRow += 0x100;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2GraphicsMan_copyScreenTint);
#ifdef SKIP_ASM
extern float cScreenTint_m_ColourR;
extern float cScreenTint_m_ColourG;
extern float cScreenTint_m_ColourB;
extern float cScreenTint_m_FillR;
extern float cScreenTint_m_FillG;
extern float cScreenTint_m_FillB;


/**
 * Copies the screen-tint colour and fill tweak values into the graphics manager's tint entry for view @p idx.
 *
 * Writes cScreenTint_m_ColourR/G/B and cScreenTint_m_FillR/G/B to the 0x18-byte entry at +0x6CA4 + @p idx * 0x18.
 * Callers: cPS2GraphicsMan_queueScreenTintPass (when cScreenTint_m_OverrideWP is set) and cPS2GraphicsMan_UpdateFullScreenEffects.
 *
 * @param idx  the view index
 */
void cPS2GraphicsMan::copyScreenTint(int idx)
{
    float* entry = unk_6CA4[idx];
    entry[0] = cScreenTint_m_ColourR;
    entry[1] = cScreenTint_m_ColourG;
    entry[2] = cScreenTint_m_ColourB;
    entry[3] = cScreenTint_m_FillR;
    entry[4] = cScreenTint_m_FillG;
    entry[5] = cScreenTint_m_FillB;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cPS2GraphicsMan_queueScreenTintPass);
#ifdef SKIP_ASM
extern int cScreenTint_m_Enable;
extern int cScreenTint_m_OverrideWP;
extern void* SPRMan;

extern "C" void func_003905E8(void* self, int idx, int* mark);
// PORT: cPS2ScratchPad_flushBufferTo is declared void in this unit, but it returns its tail call's
// value and this caller uses it; bind an int-returning view to the same symbol.
extern "C" int cPS2ScratchPad_flushBufferTo_r(void* self, int end, int arg) __asm__("cPS2ScratchPad_flushBufferTo");
extern "C" void cTextureMan_appendFixedPacketRef(void* self, void* pkt);

/**
 * Queues the screen-tint full-screen pass for view @p idx on the scratchpad DMA ring.
 *
 * Returns at once unless cScreenTint_m_Enable; refreshes the entry first (cPS2GraphicsMan_copyScreenTint) when cScreenTint_m_OverrideWP
 * is set, and skips the neutral setting colour (1, 1, 1) with fill (0, 0, 0). Otherwise it claims a buffer
 * (cPS2ScratchPad_claimBuffer), lets func_003905E8 write the pass, flushes it (cPS2ScratchPad_flushBufferTo, result into *@p addr) and calls
 * cTextureMan_appendFixedPacketRef(mTexMan, @p pkt). Caller: cPS2DeferManager_buildDmaChains.
 *
 * @param idx  the view index
 * @param pkt  passed to cTextureMan_appendFixedPacketRef
 * @param addr  in/out: the DMA ring's current main-memory address
 */
void cPS2GraphicsMan::queueScreenTintPass(int idx, void* pkt, int* addr)
{
    if (cScreenTint_m_Enable == 0) {
        return;
    }
    if (cScreenTint_m_OverrideWP != 0) {
        copyScreenTint(idx);
    }
    float* tint = unk_6CA4[idx];
    if (tint[0] == 1.0f && tint[1] == 1.0f && tint[2] == 1.0f && tint[3] == 0.0f && tint[4] == 0.0f && tint[5] == 0.0f) {
        return;
    }
    void* ring = SPRMan;
    int mark = cPS2ScratchPad_claimBuffer(ring, *addr, -1, 0);
    func_003905E8(this, idx, &mark);
    *addr = cPS2ScratchPad_flushBufferTo_r(ring, mark, 4);
    cTextureMan_appendFixedPacketRef(mTexMan, pkt);
}
#endif

INCLUDE_ASM("render/lightman", func_003905E8);

//100%
INCLUDE_ASM("render/lightman", cRenderToggles_update);
#ifdef SKIP_ASM
extern int cRenderToggles_m_AIUpdatesPerRender;
extern int cRenderToggles_m_ResetStats;


/**
 * Passes cRenderToggles_m_AIUpdatesPerRender (menu "AI Frames per Render Frame") to vtable slot 16 (cGraphicsMan_SetAIUpdatesPerRender) and clears cRenderToggles_m_ResetStats (menu
 * "Reset Stats") when it is set.
 *
 * Slot 2 of vtable cRenderToggles_vtbl (the object allocated as "RenderToggles").
 */
extern "C" void cRenderToggles_update()
{
    cPS2GraphicsMan* mgr = cGraphicsMan_TheGraphicsMan;
    mgr->SetAIUpdatesPerRender(cRenderToggles_m_AIUpdatesPerRender);
    if (cRenderToggles_m_ResetStats != 0) {
        cRenderToggles_m_ResetStats = 0;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", cScreenTint_update);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningEnable;
extern int cScreenTint_m_timeTillThunder;
extern int cScreenTint_m_lightningFrame;
extern float cScreenTint_m_lightningBrightness;
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
extern float cScreenTint_m_ColourR;
extern float cScreenTint_m_ColourG;
extern float cScreenTint_m_ColourB;
extern float cScreenTint_m_redColour;
extern float cScreenTint_m_greenColour;
extern float cScreenTint_m_blueColour;
extern int cScreenTint_m_blendMode;
int BXrand();
extern "C" void DoLightningFlash(float t);

static inline float RandF_0C60()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float RandRange_0C60(float lo, float hi)
{
    return RandF_0C60() * (hi - lo) + lo;
}

/**
 * Runs the lightning flash: counts down to thunder, may start a new flash, and sets the flash brightness and colour for
 * the current frame.
 *
 * Does nothing unless cScreenTint_m_LightningEnable. cScreenTint_m_timeTillThunder (frames until thunder) counts down to 0. While no flash
 * runs (cScreenTint_m_lightningFrame < 0) a random draw below cScreenTint_m_LightningChance squared starts one through DoLightningFlash with
 * a random distance of 2000 to 200000. Otherwise the frame advances (or holds at cScreenTint_m_FrameId when that is a valid frame)
 * and picks the phase: fade in over cScreenTint_m_LightningFadeIn frames to brightness cScreenTint_m_LightningScale (colour cScreenTint_m_LightningColourR..3C), hold for
 * cScreenTint_m_LightningFadeOut, an inverse phase of cScreenTint_m_LightningFadeIn2 at cScreenTint_m_LightningScale2 (colour cScreenTint_m_LightningColour2R..48), and fade out over cScreenTint_m_LightningFadeIn3 from
 * cScreenTint_m_LightningScale3 (colour cScreenTint_m_LightningColour3R..54); after that the frame returns to -1 with the tint colour cScreenTint_m_ColourR/G/B
 * and brightness 0. Each step sets blend mode cScreenTint_m_blendMode = 5.
 *
 * Slot 2 of vtable cScreenTint_vtbl (the object allocated as "ScreenTint").
 */
extern "C" void cScreenTint_update()
{
    if (cScreenTint_m_LightningEnable == 0) {
        return;
    }
    if (cScreenTint_m_timeTillThunder > 0) {
        cScreenTint_m_timeTillThunder--;
    }
    int lightFrame = cScreenTint_m_lightningFrame;
    int trig = cScreenTint_m_FrameId;
    if (lightFrame < 0) {
        if (RandF_0C60() < cScreenTint_m_LightningChance * cScreenTint_m_LightningChance) {
            DoLightningFlash(RandRange_0C60(0.019999999552965164f, 2.0f) * 100000.0f);
        }
        return;
    }
    if (trig < 0 || cScreenTint_m_LightningFadeIn + cScreenTint_m_LightningFadeOut + cScreenTint_m_LightningFadeIn2 + cScreenTint_m_LightningFadeIn3 < trig) {
        cScreenTint_m_lightningFrame = lightFrame + 1;
    } else {
        cScreenTint_m_lightningFrame = trig;
    }
    int cur = cScreenTint_m_lightningFrame;
    int fadeInFrames = cScreenTint_m_LightningFadeIn;
    if (cur <= fadeInFrames) {
        cScreenTint_m_lightningBrightness = ((float)cur / (float)fadeInFrames) * cScreenTint_m_LightningScale;
        cScreenTint_m_redColour = cScreenTint_m_LightningColourR;
        cScreenTint_m_greenColour = cScreenTint_m_LightningColourG;
        cScreenTint_m_blueColour = cScreenTint_m_LightningColourB;
        cScreenTint_m_blendMode = 5;
    } else if (cur <= fadeInFrames + cScreenTint_m_LightningFadeOut) {
        cScreenTint_m_lightningBrightness = cScreenTint_m_LightningScale;
        cScreenTint_m_redColour = cScreenTint_m_LightningColourR;
        cScreenTint_m_greenColour = cScreenTint_m_LightningColourG;
        cScreenTint_m_blueColour = cScreenTint_m_LightningColourB;
        cScreenTint_m_blendMode = 5;
    } else if (cur <= fadeInFrames + cScreenTint_m_LightningFadeOut + cScreenTint_m_LightningFadeIn2) {
        cScreenTint_m_lightningBrightness = cScreenTint_m_LightningScale2;
        cScreenTint_m_redColour = cScreenTint_m_LightningColour2R;
        cScreenTint_m_greenColour = cScreenTint_m_LightningColour2G;
        cScreenTint_m_blueColour = cScreenTint_m_LightningColour2B;
        cScreenTint_m_blendMode = 5;
    } else if (cur <= fadeInFrames + cScreenTint_m_LightningFadeOut + cScreenTint_m_LightningFadeIn2 + cScreenTint_m_LightningFadeIn3) {
        cScreenTint_m_lightningBrightness = (1.0f - ((float)cur - (float)fadeInFrames
                                                     - (float)cScreenTint_m_LightningFadeOut
                                                     - (float)cScreenTint_m_LightningFadeIn2)
                                                    / (float)cScreenTint_m_LightningFadeIn3)
                                            * cScreenTint_m_LightningScale3;
        cScreenTint_m_redColour = cScreenTint_m_LightningColour3R;
        cScreenTint_m_greenColour = cScreenTint_m_LightningColour3G;
        cScreenTint_m_blueColour = cScreenTint_m_LightningColour3B;
        cScreenTint_m_blendMode = 5;
    } else {
        cScreenTint_m_lightningFrame = -1;
        cScreenTint_m_redColour = cScreenTint_m_ColourR;
        cScreenTint_m_greenColour = cScreenTint_m_ColourG;
        cScreenTint_m_blueColour = cScreenTint_m_ColourB;
        cScreenTint_m_blendMode = 5;
        cScreenTint_m_lightningBrightness = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", DoLightningFlash);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningEnable;
extern int cScreenTint_m_lightningFrame;
extern float cScreenTint_m_lightningBrightness;
extern float cScreenTint_m_flashDistance;
extern int cScreenTint_m_timeTillThunder;

/**
 * Starts a lightning flash at distance @p flashDistance when lightning is enabled.
 *
 * Sets the flash frame cScreenTint_m_lightningFrame and brightness cScreenTint_m_lightningBrightness to 0, stores @p flashDistance in cScreenTint_m_flashDistance and sets the thunder
 * countdown cScreenTint_m_timeTillThunder to (int)(@p flashDistance * 0.0018072289). Callers: cScreenTint_update and LightningFunc.
 *
 * @param flashDistance  the flash distance; larger values delay the thunder
 */
extern "C" void DoLightningFlash(float flashDistance)
{
    if (cScreenTint_m_LightningEnable != 0) {
        cScreenTint_m_lightningFrame = 0;
        cScreenTint_m_lightningBrightness = 0.0f;
        cScreenTint_m_flashDistance = flashDistance;
        cScreenTint_m_timeTillThunder = (int)(flashDistance * 0.0018072288949042559f);
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", PlayThunder);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningEnable;
extern int cScreenTint_m_timeTillThunder;

/**
 * Reports once that the thunder of the last lightning flash is due.
 *
 * When cScreenTint_m_LightningEnable is set and the countdown cScreenTint_m_timeTillThunder has reached 0, sets it to
 * -1 and returns true. Caller: cSSXAudio_Update (audio).
 *
 * @return true on the frame the thunder is due, else false
 */
extern "C" bool PlayThunder()
{
    if (cScreenTint_m_LightningEnable != 0 && cScreenTint_m_timeTillThunder == 0) {
        cScreenTint_m_timeTillThunder = -1;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cScreenTint_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sGs_0F20 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 19;
    unsigned int f8_29 : 3;
    unsigned int wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF22(int v) { f4_22 = v; }
    void SetF2(int v) { int* w = (int*)((char*)this + 4); *w = (*w & ~0x7C) | ((v << 2) & 0x7C); }
};



extern sGs_0F20 sDrawState_s_defaultDrawState_0F20 __asm__("sDrawState_s_defaultDrawState");
extern int cScreenTint_m_LightningEnable_i __asm__("cScreenTint_m_LightningEnable");
extern float cScreenTint_m_lightningBrightness_f __asm__("cScreenTint_m_lightningBrightness");
extern float cScreenTint_m_redColour_f __asm__("cScreenTint_m_redColour");
extern float cScreenTint_m_greenColour_f __asm__("cScreenTint_m_greenColour");
extern float cScreenTint_m_blueColour_f __asm__("cScreenTint_m_blueColour");
extern int cScreenTint_m_blendMode_i __asm__("cScreenTint_m_blendMode");

class cGfx_0F20 {
public:
    char pad[0x270];
    int f270;
    int f274;
    char pad278[0xE84 - 0x278];
    sGs_0F20* stateTop;
    char padE88[0x10D8 - 0xE88];
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual unsigned int v9(); virtual unsigned int v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void PushViewport(); virtual void PopViewport(); virtual void v22(float, float, float, float, int);
    virtual void v23(); virtual void v24(); virtual void SetViewportOrthographic(float, float, float, float, float, float, int, int);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void PushView(); virtual void PopView(); virtual void v32(); virtual void v33(char*);
    virtual void v34(); virtual void v35(); virtual void IdentityView(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void RenderTriStrip(int, tBxVertex*, int);
};

/**
 * Draws the lightning flash as a full-screen quad when lightning is enabled and the flash brightness is non-zero.
 *
 * Pushes a draw state copied from sDrawState_s_defaultDrawState with blend mode cScreenTint_m_blendMode and no texture, then
 * PushViewport, PushView and IdentityView, and an orthographic viewport through slot 26
 * (cPS2GraphicsMan_SetViewportOrthographic). The quad is 640 x 480, or the left or right 320-pixel half when mNumViews
 * is not 1 (mViewIndex picks the half). Its colour is brightness * cScreenTint_m_redColour/A4/A8 * 255 with alpha brightness * 255,
 * drawn with slot 71 (cPS2GraphicsMan_RenderTriStrip); then PopView, PopViewport and the draw state is popped. Slot 3
 * of vtable cScreenTint_vtbl (the object allocated as "ScreenTint").
 */
extern "C" void cScreenTint_render()
{
    int flag = 0;
    if (cScreenTint_m_LightningEnable_i != 0) {
        if (cScreenTint_m_lightningBrightness_f != 0.0f) {
            flag = 1;
        }
    }
    if (flag) {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        ((cGfx_0F20*)ps2GraphicsMan)->PushState();
        ps2GraphicsMan->PushViewport();
        ps2GraphicsMan->PushView();
        ps2GraphicsMan->IdentityView();
        float quadWidth;
        float quadLeft;
        if (((cGfx_0F20*)ps2GraphicsMan)->f270 == 1) {
            quadWidth = 640.0f;
            quadLeft = 0.0f;
        } else {
            quadWidth = 320.0f;
            quadLeft = 0.0f;
            if (((cGfx_0F20*)ps2GraphicsMan)->f274 != 0) {
                quadLeft = quadWidth;
            }
        }
        ps2GraphicsMan->SetViewportOrthographic(quadLeft, 0.0f, quadWidth, 480.0f, 0.0f, 1.0f, 0, 0);
        *((cGfx_0F20*)ps2GraphicsMan)->stateTop = sDrawState_s_defaultDrawState_0F20;
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->f8_5 = 5;
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->SetF22(1);
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->f4_23 = 2;
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->f4_20 = 0;
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->f4_12 = 20;
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop->SetF2(cScreenTint_m_blendMode_i);
        *(short*)((char*)((cGfx_0F20*)ps2GraphicsMan)->stateTop + 0x10) = -1;
        tBxVertex verts[4];
        t4Vector corner;
        corner.x = quadLeft;
        corner.z = 0.0f;
        corner.y = 0.0f;
        corner.w = 1.0f;
        (*(&verts[0].p)) = corner;
        corner.x = quadLeft + quadWidth;
        corner.z = 0.0f;
        corner.y = 0.0f;
        (*(&verts[1].p)) = corner;
        corner.x = quadLeft;
        corner.z = 0.0f;
        corner.y = 480.0f;
        (*(&verts[2].p)) = corner;
        corner.x = quadLeft + quadWidth;
        corner.z = 0.0f;
        corner.y = 480.0f;
        (*(&verts[3].p)) = corner;
        if (flag) {
            float al = cScreenTint_m_lightningBrightness_f;
            int red = (int)(al * cScreenTint_m_redColour_f * 255.0f);
            int gg = (int)(al * cScreenTint_m_greenColour_f * 255.0f);
            int bb = (int)(al * cScreenTint_m_blueColour_f * 255.0f);
            int aa = (int)(al * 255.0f);
            for (int i = 0; i < 4; i++) {
                verts[i].r = red;
                verts[i].g = gg;
                verts[i].b = bb;
                verts[i].a = aa;
                verts[i].one = 1.0f;
            }
            ps2GraphicsMan->RenderTriStrip(4, verts, 0);
        }
        ps2GraphicsMan->PopView();
        ps2GraphicsMan->PopViewport();
        ((cGfx_0F20*)ps2GraphicsMan)->stateTop--;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", vu0_uploadProgramChainA);
#ifdef SKIP_ASM
extern char vu0_programChainA[];
extern "C" void FlushCache(int);

/**
 * Uploads the VU0 microprogram chain at vu0_programChainA through the VIF0 DMA channel and waits for it.
 *
 * Waits for VIF0 DMA (0x10008000) to be idle, starts it in chain mode from vu0_programChainA (CHCR 260), waits again and calls
 * FlushCache(0). Caller: cPS2GraphicsMan_Init.
 */
// PORT: kicks a DMA chain on channel 0 (VIF0) through the PS2 hardware registers at
// 0x10008000 (CHCR), 0x10008020 (QWC) and 0x10008030 (TADR); needs a platform shim.
extern "C" void vu0_uploadProgramChainA(void)
{
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    // ORIGINAL-PTR: the DMA tag address register (D0_TADR) takes the chain's address as a 32-bit word (Sony SDK
    // eeregs.h, DPUT_D0_TADR(u_int)).
    *(volatile int*)0x10008030 = (int)vu0_programChainA;
    *(volatile int*)0x10008020 = 0;
    *(volatile int*)0x10008000 = 260;
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    FlushCache(0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", vu0_uploadProgramChainB);
#ifdef SKIP_ASM
extern char vu0_programChainB[];
extern "C" void FlushCache(int);

/**
 * Uploads the VU0 microprogram chain at vu0_programChainB through the VIF0 DMA channel and waits for it.
 *
 * Same sequence as vu0_uploadProgramChainA with the chain at vu0_programChainB. Caller: cPS2GraphicsMan_Init.
 */
// PORT: kicks a DMA chain on channel 0 (VIF0) through the PS2 hardware registers at
// 0x10008000 (CHCR), 0x10008020 (QWC) and 0x10008030 (TADR); needs a platform shim.
extern "C" void vu0_uploadProgramChainB(void)
{
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    // ORIGINAL-PTR: the DMA tag address register (D0_TADR) takes the chain's address as a 32-bit word (Sony SDK
    // eeregs.h, DPUT_D0_TADR(u_int)).
    *(volatile int*)0x10008030 = (int)vu0_programChainB;
    *(volatile int*)0x10008020 = 0;
    *(volatile int*)0x10008000 = 260;
    while (*(volatile int*)0x10008000 & 0x100) {
    }
    FlushCache(0);
}
#endif

//100%
INCLUDE_ASM("render/lightman", vu0_runProgramWithMatrices);
#ifdef SKIP_ASM
/**
 * Loads 16 quadwords from @p mats into VU0 registers vf1-vf16 and starts the VU0 microprogram at address 0.
 *
 * @p self is unused. Caller: cAIFwdDiffCache_TesselatePatch.
 *
 * @param mats  16 quadwords (four 4x4 matrices)
 */
// PORT: PS2-only VU0 inline asm (lqc2 x16 into vf1-vf16, then vcallmsr of the
// microprogram at VU0 micro-memory address 0); the PC port needs a C version.
extern "C" void vu0_runProgramWithMatrices(void* self, void* mats)
{
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "lqc2      $vf5, 0x40(%0)\n"
        "lqc2      $vf6, 0x50(%0)\n"
        "lqc2      $vf7, 0x60(%0)\n"
        "lqc2      $vf8, 0x70(%0)\n"
        "lqc2      $vf9, 0x80(%0)\n"
        "lqc2      $vf10, 0x90(%0)\n"
        "lqc2      $vf11, 0xA0(%0)\n"
        "lqc2      $vf12, 0xB0(%0)\n"
        "lqc2      $vf13, 0xC0(%0)\n"
        "lqc2      $vf14, 0xD0(%0)\n"
        "lqc2      $vf15, 0xE0(%0)\n"
        "lqc2      $vf16, 0xF0(%0)\n"
        "lui       $2, 0\n"
        "addiu     $2, $2, 0\n"
        "srl       $2, $2, 3\n"
        "ctc2.ni   $2, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        ".set pop\n"
        :
        : "r"(mats)
        : "$2", "memory");
}
#endif

//100%
INCLUDE_ASM("render/lightman", vu0_readDataMemory);
#ifdef SKIP_ASM
struct sQuad16 {
    int w[4];
} __attribute__((aligned(16)));

/**
 * Copies 100 quadwords from VU0 data memory (0x11004200) to @p dst.
 *
 * @p self is unused. Caller: cAIFwdDiffCache_TesselatePatch.
 *
 * @param dst  the 1600-byte destination
 */
extern "C" void vu0_readDataMemory(void* self, sQuad16* dst)
{
    sQuad16* src = (sQuad16*)0x11004200;
    int i;
    for (i = 0; i < 10; i++) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        dst[8] = src[8];
        dst[9] = src[9];
        src += 10;
        dst += 10;
    }
}
#endif

//100%
INCLUDE_ASM("render/lightman", vu0_waitReadVi1);
#ifdef SKIP_ASM
/**
 * Reads VU0 integer register vi1, waiting for the running microprogram to finish (cfc2.i).
 *
 * Caller: cAIFwdDiffCache_TesselatePatch.
 *
 * @return the value of vi1
 */
// PORT: PS2-only VU0 integer register read (vi1); the PC port needs a C fallback.
extern "C" int vu0_waitReadVi1(void)
{
    int result;
    __asm__ __volatile__("cfc2.i %0, $vi1" : "=r"(result));
    return result;
}
#endif

//100%
INCLUDE_ASM("render/lightman", cFont_cFont1);
#ifdef SKIP_ASM
// A tARGB with EA's constructors (ours has none): cFont's Color and ShadowColor are tARGBs ([O]).
struct tARGBInit : tARGB {
    tARGBInit() {}
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};

/**
 * Initialises a font object to its defaults without loading anything.
 *
 * Zeroes NumGlyphs, Glyphs, MissingGlyph, FirstCode, LastLinearCode and MaxCharHeight; sets ShadowOffset (0, 0),
 * BaseScale (1, 1), Scale = BaseScale, Color (1, 1, 1, 1), ShadowColor (1, 0, 0, 0), TextureID -1 and FontName to an
 * empty string. Offset and UVScale are left as they are. No caller.
 *
 * @return @p self
 */
void* cFont::cFont1()
{
    *(t2Vector*)&ShadowOffset[0] = t2Vector(0.0f, 0.0f);
    *(t2Vector*)&BaseScale[0] = t2Vector(1.0f, 1.0f);
    *(t2Vector*)&Scale[0] = *(t2Vector*)&BaseScale[0];
    {
        tARGBInit allOnes;
        allOnes.A = 1.0f; allOnes.R = 1.0f; allOnes.G = 1.0f; allOnes.B = 1.0f;
        *(tARGB*)&Color[0] = allOnes;
    }
    *(tARGB*)&ShadowColor[0] = tARGBInit(1.0f, 0.0f, 0.0f, 0.0f);
    TextureID = -1;
    NumGlyphs = 0;
    Glyphs = 0;
    MissingGlyph = 0;
    MaxCharHeight = 0;
    FirstCode = 0;
    LastLinearCode = 0;
    FontName[0] = 0;
    return ((char*)this);
}
#endif

//100%
INCLUDE_ASM("render/lightman", __5cFontPCci);
#ifdef SKIP_ASM


extern "C" void cFont_loadFont(cFont* self, const char* name, int flags);

/**
 * Constructs a font: sets the default draw state, then loads the named font file.
 *
 * MissingGlyph = 0, ShadowOffset (0, 0), BaseScale and Scale (1, 1), Color (1, 1, 1, 1), ShadowColor (1, 0,
 * 0, 0) and an empty FontName, then cFont_loadFont().
 *
 * @param name  font file name (e.g. "data/fonts/debugfnt.sfn")
 * @param flags  memory flags for the font's allocations (On Tour: memtype)
 */
cFont::cFont(const char* name, int flags)
{
    MissingGlyph = 0;
    (*(t2Vector*)&ShadowOffset[0]) = t2Vector(0.0f, 0.0f);
    (*(t2Vector*)&BaseScale[0]) = t2Vector(1.0f, 1.0f);
    (*(t2Vector*)&Scale[0]) = (*(t2Vector*)&BaseScale[0]);
    {
        tARGBInit white;
        white.A = 1.0f;
        white.R = 1.0f;
        white.G = 1.0f;
        white.B = 1.0f;
        (*(tARGB*)&Color[0]) = white;
    }
    (*(tARGB*)&ShadowColor[0]) = tARGBInit(1.0f, 0.0f, 0.0f, 0.0f);
    FontName[0] = 0;
    loadFont(name, flags);
}
#endif

//100%
INCLUDE_ASM("render/lightman", _$_5cFont);
#ifdef SKIP_ASM
extern "C" void cFont_purge(void* self);
void operator_delete(int* ptr);

/**
 * Destroys a font: calls cFont_purge and frees the object when bit 0 of g++'s delete flag is set.
 *
 * Callers: cDebugFE_purge, cGame_purge and cUIFontInterface_purgeFonts.
 */
cFont::~cFont()
{
    cFont_purge(this);
}
#endif

//100%
INCLUDE_ASM("render/lightman", cFont_loadFont);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void cFont_linkFont(void* self, void* data, int flags);
void cMemMan_free(void*);
extern const char str_font_pct_24s[];

/**
 * Loads a font file and builds the font's glyph table and texture from it.
 *
 * Writes "font:%24s" with @p name into FontName, loads the whole file with FILE_load() (flags with bit 0x100
 * flipped; memstd's allocator _MEM_allocaligna treats that bit as a "back" flag), builds the font with
 * cFont_linkFont() and frees the file buffer. %24s is a minimum width, not a limit, so a name longer than 26
 * characters would overrun the 32-byte FontName.
 *
 * @param name  font file name
 * @param flags  memory flags for the glyph table (On Tour: memtype)
 */
void cFont::loadFont(const char* name, int flags)
{
    void* data;

    sprintf((char*)&FontName, str_font_pct_24s, name);
    data = FILE_load(name, flags ^ 0x100);
    cFont_linkFont(this, data, flags);
    if (data != 0) {
        cMemMan_free(data);
    }
}
#endif

