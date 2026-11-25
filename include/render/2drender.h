#ifndef RENDER_2DRENDER_H
#define RENDER_2DRENDER_H

// c2DRender: EA's stateless 2D quad/line helper. Its methods fill the positions (and UVs) of four tBxVertex from a
// tRect, optionally transform them by a 3x3 matrix (func_0038E0A8), and submit them to the graphics manager
// (cGraphicsMan_TheGraphicsMan virtual slot 71 RenderTriStrip, slot 73 RenderLineStrip for drawLine). Callers: the UI
// text
// elements (cTextureElement_render, cTextureElement_render2, cTextureElement_render*), cHeartBeat_draw and cFadeScreen_drawFade.
// No EA source path in the preview's asserts (On Tour: render/all_in_one_render.cpp); retail methods are in the
// render/lightman splat unit (0x38E118..0x38EE78).
// Size 0x1 (an empty class): [R] cSSXApp_init allocates the only instance with cMemMan_alloc(1, "2DRender") and stores
// it
// in G2D (On Tour's global G2D, c2DRender*); [O] On Tour's c2DRender is 0x1 with no members. Every caller loads
// G2D into a0, and no method reads or writes through it (retail asm: 0 accesses through a0).
//
// Its methods' arguments are what carry the raw offsets, so the two argument types are laid out here too:
//   tBxVertex (0x30): stride 0x30 in every method (u/v stores at +0x0/+0x4, sqc2 of p at +0x20 per vertex);
//   tRect (0x10): X, Y, W, H read at +0x0..+0xC.
// Evidence tags: [R] retail allocation, [U] retail use (src/render/lightman.cpp and its asm), [O] SSX On Tour's DWARF
// (class c2DRender, struct tBxVertex, struct tRect). tBxVertex is a bx renderer type and may belong in a bx header.

#include "layout.h"
#include "bx/math3d.h"

// One renderer vertex (On Tour tBxVertex, 0x30). Colours are 0..255 (alpha 0..128) ints, as the GS takes them.
// The other users of the type: cPS2GraphicsMan::RenderTriStrip(int, const tBxVertex*, bool) (vtable slot 71; [O] the
// PSP twin's signature), cSimpleTube::m_vertices ([O] tBxVertex[262], retail cSimpleTube_m_vertices: u 0, v 0/1,
// one 1.0, colour, p set per frame), cGraphicsMan::RenderIndexedTriList / RenderSpline (local tBxVertex arrays) and
// the transition objects' InitVertex(tBxVertex&, ...) ([O]).
// The empty default constructor is EA's ([O] On Tour lists tBxVertex::tBxVertex()): [R] retail code that declares a
// local array of them keeps the empty construction loop (cGraphicsMan_RenderIndexedTriList verts[3],
// cCircle_TObj_Draw top[80] / bot[80], cWorldPainterMan_render1 quadVerts[5]).
struct tBxVertex {
    float u;                        // 0x00 [O]; [U] drawRect2/3 and drawRotRect2/3 store the UVs; drawRect4 0/1
    float v;                        // 0x04 [O]
    float one;                      // 0x08 [O]; [U] drawRect4 stores 1.0 (the Q coordinate)
    float pad;                      // 0x0C [O]
    unsigned int r;                 // 0x10 [O]; [U] drawRect4/drawLine store (int)(colour.R * 255)
    unsigned int g;                 // 0x14 [O]; [U] (int)(colour.G * 255)
    unsigned int b;                 // 0x18 [O]; [U] (int)(colour.B * 255)
    unsigned int a;                 // 0x1C [O]; [U] (int)(colour.A * 128)
    t4Vector p;                     // 0x20 [O]; [U] every method stores the position (sqc2/sq), w = 1

    tBxVertex() {}                  // [O] tBxVertex::tBxVertex(); [R] the empty construction loops above
};

CHECK_OFFSET(tBxVertex, u, 0x00);
CHECK_OFFSET(tBxVertex, v, 0x04);
CHECK_OFFSET(tBxVertex, one, 0x08);
CHECK_OFFSET(tBxVertex, pad, 0x0C);
CHECK_OFFSET(tBxVertex, r, 0x10);
CHECK_OFFSET(tBxVertex, g, 0x14);
CHECK_OFFSET(tBxVertex, b, 0x18);
CHECK_OFFSET(tBxVertex, a, 0x1C);
CHECK_OFFSET(tBxVertex, p, 0x20);
CHECK_SIZE(tBxVertex, 0x30);

// A screen rectangle (On Tour tRect, 0x10).
struct tRect {
    float X;                        // 0x0 [O]; [U] left edge (lwc1 0x0)
    float Y;                        // 0x4 [O]; [U] top edge
    float W;                        // 0x8 [O]; [U] right = X + W
    float H;                        // 0xC [O]; [U] bottom = Y + H
};

CHECK_OFFSET(tRect, X, 0x0);
CHECK_OFFSET(tRect, Y, 0x4);
CHECK_OFFSET(tRect, W, 0x8);
CHECK_OFFSET(tRect, H, 0xC);
CHECK_SIZE(tRect, 0x10);

struct t4Vector;

struct c2DRender {
    // no members ([O] On Tour size 0x1; [R] cMemMan_alloc(1, "2DRender"))

    // Methods bound to their retail symbols by asm labels.
    void drawRect(tBxVertex* verts, float* rect) __asm__("c2DRender_drawRect");
    void drawRotRect(tBxVertex* verts, float* rect) __asm__("c2DRender_drawRotRect");
    void drawRect1(tBxVertex* verts, float* rect, float* mat) __asm__("c2DRender_drawRect1");
    void drawRotRect1(tBxVertex* verts, float* rect, float* mat) __asm__("c2DRender_drawRotRect1");
    void drawRect2(tBxVertex* vtx, float* rect, float* uv) __asm__("c2DRender_drawRect2");
    void drawRotRect2(tBxVertex* vtx, float* rect, float* uv) __asm__("c2DRender_drawRotRect2");
    void drawRect3(tBxVertex* vtx, float* rect, float* uv, float* mat) __asm__("c2DRender_drawRect3");
    void drawRotRect3(tBxVertex* vtx, float* rect, float* uv, float* mat) __asm__("c2DRender_drawRotRect3");
    void drawRect4(float* topLeft, float* bottomRight, float* col, float z) __asm__("c2DRender_drawRect4");
    void drawLine(t4Vector* start, t4Vector* end, float* col) __asm__("c2DRender_drawLine");
};

CHECK_SIZE(c2DRender, 0x1);

#endif /* RENDER_2DRENDER_H */
