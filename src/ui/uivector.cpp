#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "ui/uianimation.h"
#include "ui/uitextscroll.h"
#include "ui/uistate.h"
#include "ui/uislider.h"
#include "ui/uiscrollbar.h"
#include "ui/uiscreen.h"
#include "ui/uiprogress.h"
#include "ui/uipair.h"
#include "ui/uilistbox.h"
#include "ui/uiicon.h"
#include "ui/uigroup.h"
#include "util/list.h"
#include "ui/uivector.h"
#include "ui/uimenu.h"
#include "render/ps2graphicsman.h"
#include "ui/uitext.h"
#include "ui/uithing.h"

// Flag words at cUIVector + 0x14 and + 0x74. The setters take an int and store it into a
// bool bitfield: that int->bool conversion is the `sltu` in their asm.
struct sUIVectorFlags {
    bool bit0 : 1;
    bool bit1 : 1;
    bool bit2 : 1;
    bool bit3 : 1;
    bool bit4 : 1;
    bool bit5 : 1;
    bool bit6 : 1;
};

//100%
INCLUDE_ASM("ui/uivector", cUIVector_setUIData);
#ifdef SKIP_ASM
extern "C" void cUIThing_setUIData(void* self, void* data);
extern char str_UIVectorClr[];
extern char str_UIVectorVtx[];
extern char str_UIVectorVtxRdr[];
extern char str_UIVectorLine[];

// On Tour: tAttrVector.
struct sUIVecFlags_34C0 {
    unsigned int mFill : 1;
    unsigned int mLoop : 1;
    unsigned int mNumTriangles : 15;
    unsigned int mNumVertices : 15;
};

// PORT: tagged array new (operator_new__FUi) as member operators; element ctors are empty.
struct sUIVec16_34C0 {
    float x, y, z, w;
    sUIVec16_34C0() {}
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
};

struct sUIVec3_34C0 {
    float x, y, z;
    sUIVec3_34C0() {}
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
};

struct sUIVec3b_34C0 {
    float x, y, z;
    sUIVec3b_34C0() {}
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
};

struct sUIVec48_34C0 {
    char data[0x30];
    sUIVec48_34C0() {}
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
};

// The unit declares t3vect_Zero later as a plain vec3; bind this view by asm label.
extern sUIVec3_34C0 D_004FF0D8_34C0 __asm__("t3vect_Zero");

// On Tour: tUIVector (a tUIBaseObject header, then the counts; triangle data follows).
struct sUIVecData_34C0 {
    char pad0[0x20];
    char mNumVertices;          // 0x20
    char mNumTriangles;         // 0x21
    char mFlags;                // 0x22
    char mPad;                  // 0x23
};


/**
 * Sets up a UI vector shape from its record in the screen data.
 *
 * After the base cUIThing setup it copies the fill and loop flags and the triangle and vertex counts, points
 * the triangle list just past the record, and allocates the vertex colour, position (zeroed) and
 * render-position arrays. An unfilled (line) shape also gets an array of 0x30-byte line vertices.
 *
 * @param uiVector        the vector object
 * @param pUIBase  its record (On Tour tUIBaseObject*)
 */
extern "C" void cUIVector_setUIData(cUIVector* uiVector, void* pUIBase)
{
    cUIVector* self = uiVector;
    cUIThing_setUIData(self, pUIBase);
    sUIVecData_34C0* pIYFVector = (sUIVecData_34C0*)pUIBase;
    (*(sUIVecFlags_34C0*)&self->mAttrVector).mFill = pIYFVector->mFlags & 1;
    (*(sUIVecFlags_34C0*)&self->mAttrVector).mLoop = pIYFVector->mFlags & 2;
    (*(sUIVecFlags_34C0*)&self->mAttrVector).mNumTriangles = pIYFVector->mNumTriangles;
    if ((*(sUIVecFlags_34C0*)&self->mAttrVector).mNumTriangles)
        *(void**)&self->mpTriangles = (unsigned char*)(pIYFVector + 1);
    (*(sUIVecFlags_34C0*)&self->mAttrVector).mNumVertices = pIYFVector->mNumVertices;
    if (pIYFVector->mNumVertices)
    {
        sUIVec16_34C0** pa = (sUIVec16_34C0**)&self->mpVertexColour;
        *pa = new (str_UIVectorClr, 0x100, 0) sUIVec16_34C0[pIYFVector->mNumVertices];
        sUIVec3_34C0** pb = (sUIVec3_34C0**)&self->mpVertexPosition;
        *pb = new (str_UIVectorVtx, 0x100, 0) sUIVec3_34C0[pIYFVector->mNumVertices];
        sUIVec3b_34C0** pc = (sUIVec3b_34C0**)&self->mpVertexPositionRender;
        *pc = new (str_UIVectorVtxRdr, 0x100, 0) sUIVec3b_34C0[pIYFVector->mNumVertices];
        for (int i = 0; i < pIYFVector->mNumVertices; i++)
            ((sUIVec3_34C0*)self->mpVertexPosition)[i] = D_004FF0D8_34C0;
    }
    if (!(*(sUIVecFlags_34C0*)&self->mAttrVector).mFill)
    {
        sUIVec48_34C0** pd = (sUIVec48_34C0**)&self->mpVertices;
        *pd = new (str_UIVectorLine, 0x100, 0) sUIVec48_34C0[(*(sUIVecFlags_34C0*)&self->mAttrVector).mNumVertices];
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_setChannelData);
#ifdef SKIP_ASM
struct sCount_36D0 {
    unsigned int flag : 1;
    unsigned int pad1 : 1;
    unsigned int mode : 15;
    unsigned int count : 15;
};
extern "C" void cUIThing_setChannelData(void* self, unsigned short key, float v);

/**
 * Sets one animated channel of a UI vector shape.
 *
 * Channels below 21 go to cUIThing_setChannelData(). Higher channels address vertex data, nine per vertex: x,
 * y, z, u, v and the four colour components in tARGB order. Does nothing at all, even for base channels,
 * while the shape has no vertex positions; vertex channels past the last vertex are ignored.
 *
 * @param key  channel number (On Tour channel)
 * @param data    the channel's new value
 */
extern "C" void cUIVector_setChannelData(cUIVector* self, unsigned short key, float data)
{
    if ((self->mpVertexPosition) == 0) {
        return;
    }
    if (key >= 21) {
        key -= 21;
        unsigned short idx = key / 9;
        if (idx >= (*(sCount_36D0*)&self->mAttrVector).count) {
            return;
        }
        switch ((unsigned short)(key % 9)) {
        case 0: (self->mpVertexPosition)[idx].x = data; break;
        case 1: (self->mpVertexPosition)[idx].y = data; break;
        case 2: (self->mpVertexPosition)[idx].z = data; break;
        case 3: (self->mpUV)[idx].x = data; break;
        case 4: (self->mpUV)[idx].y = data; break;
        case 5: (self->mpVertexColour)[idx].A = data; break;
        case 6: (self->mpVertexColour)[idx].R = data; break;
        case 7: (self->mpVertexColour)[idx].G = data; break;
        case 8: (self->mpVertexColour)[idx].B = data; break;
        }
    } else {
        cUIThing_setChannelData(self, key, data);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_setChannelDataKCP);
#ifdef SKIP_ASM
struct sCount_3808 {
    unsigned int flag : 1;
    unsigned int pad1 : 1;
    unsigned int mode : 15;
    unsigned int count : 15;
};
struct sBits_3808 {
    unsigned int lo : 13;
    unsigned int layer : 6;
    unsigned int hi : 13;
};
struct sKey_3808 {
    short key;
    short value;
};

/**
 * Applies a list of keyframed (channel, value) pairs to a UI vector shape.
 *
 * Vertex channels (21 and up) set position or colour like cUIVector_setChannelData() (no UVs). Base channels
 * are handled here: 0/1 position, 6/7 size, 9/10 scale in percent, 12 the 6-bit attribute field at bits 13-18
 * (mPivot in On Tour's tAttrThing), 13-16 the colour components divided by 255. Nothing happens while the
 * shape has no vertex positions.
 *
 * @param n     number of pairs (On Tour NumChannels)
 * @param keys  the pairs, two shorts each (channel, value)
 */
extern "C" void cUIVector_setChannelDataKCP(cUIVector* self, unsigned int n, sKey_3808* keys)
{
    if ((self->mpVertexPosition) == 0) {
        return;
    }
    for (unsigned int i = 0; i < n; i++, keys++) {
        float data = (float)keys->value;
        int key = keys->key;
        if (key >= 21) {
            key -= 21;
            unsigned short idx = key / 9;
            if (idx >= (*(sCount_3808*)&self->mAttrVector).count) {
                continue;
            }
            switch (key % 9) {
            case 0: (self->mpVertexPosition)[idx].x = data; break;
            case 1: (self->mpVertexPosition)[idx].y = data; break;
            case 2: (self->mpVertexPosition)[idx].z = data; break;
            case 5: (self->mpVertexColour)[idx].A = data; break;
            case 6: (self->mpVertexColour)[idx].R = data; break;
            case 7: (self->mpVertexColour)[idx].G = data; break;
            case 8: (self->mpVertexColour)[idx].B = data; break;
            }
        } else {
            switch (key) {
            case 0: self->mPosition.x = data; break;
            case 1: self->mPosition.y = data; break;
            case 9: self->mScale.x = data * 0.01f; break;
            case 10: self->mScale.y = data * 0.01f; break;
            case 12: (*(sBits_3808*)&self->mAttr).layer = (int)data; break;
            case 13: self->mColour.A = data * (1.0f / 255.0f); break;
            case 14: self->mColour.R = data * (1.0f / 255.0f); break;
            case 15: self->mColour.G = data * (1.0f / 255.0f); break;
            case 16: self->mColour.B = data * (1.0f / 255.0f); break;
            case 6: self->mSize.x = data; break;
            case 7: self->mSize.y = data; break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_render2D);
#ifdef SKIP_ASM
// PORT: fptoui is libgcc's __fixunssfsi (float -> unsigned); called by its address name.
extern "C" unsigned int fptoui(float f);


struct sV4_3A39F8 {
    float v[4];
} __attribute__((aligned(16)));


struct sVtx_3A39F8 {
    float s;
    float t;
    float q;
    int padC;
    int col[4];         // 0x10
    sV4_3A39F8 pos;     // 0x20
};

struct sCount_3A39F8 {
    unsigned int flag : 1;
    unsigned int pad1 : 1;
    unsigned int mode : 15;
    unsigned int count : 15;
};

struct sFlags14_3A39F8 {
    unsigned char b14;
    unsigned char layer : 5;
};


struct sVE_3A39F8 { short delta; short index; void* fn; };

typedef void (*tDraw_3A39F8)(void*, int, int, void*, void*, int, void*, void*);
typedef void (*tDraw2_3A39F8)(void*, int, void*, int);

static inline void SetCol_3A39F8(sVtx_3A39F8& vtx, int k, float v)
{
    vtx.col[k] = fptoui(v * 0.5f);
}

static inline void AddPos_3A39F8(sV4_3A39F8& p, int k, float d)
{
    p.v[k] += d;
}

/**
 * Draws a UI vector shape at its position plus an offset.
 *
 * Scales the vertex positions by mScale, sets the draw layer from the thing's attributes, then either draws
 * the filled triangles through graphics manager slot 72 or, for an outline, builds the line vertices (offset
 * positions, colours halved into the GS 0-128 range) and draws them through slot 73.
 *
 * @param x  horizontal offset added to the position
 * @param y  vertical offset added to the position
 */
extern "C" void cUIVector_render2D(cUIVector* self, float x, float y)
{
    if (self->mpVertexPosition == 0)
        return;
    for (unsigned int i = 0; i < ((sCount_3A39F8*)(((char*)self) + 0x74))->count; i++)
    {
        (self->mpVertexPositionRender)[i] = (self->mpVertexPosition)[i];
        (self->mpVertexPositionRender)[i].x *= self->mScale.x;
        (self->mpVertexPositionRender)[i].y *= self->mScale.y;
    }
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | (((((sFlags14_3A39F8*)(((char*)self) + 0x14))->layer) + 9) << 5 & 0x3E0);
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = -1;
    if (((sCount_3A39F8*)(((char*)self) + 0x74))->flag)
    {
        float pos[3];
        pos[0] = self->mPosition.x + x;
        pos[1] = self->mPosition.y + y;
        pos[2] = 0.0f;
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVE_3A39F8* vt = (sVE_3A39F8*)ps2GraphicsMan->mVtbl;
        ((tDraw_3A39F8)vt[72].fn)((char*)ps2GraphicsMan + vt[72].delta, ((sCount_3A39F8*)(((char*)self) + 0x74))->mode, ((sCount_3A39F8*)(((char*)self) + 0x74))->count,
                                  self->mpTriangles, self->mpVertexPositionRender, 0, self->mpVertexColour, pos);
        return;
    }
    if ((sVtx_3A39F8*)self->mpVertices == 0)
        return;
    for (unsigned int i = 0; i < ((sCount_3A39F8*)(((char*)self) + 0x74))->count; i++)
    {
        sV4_3A39F8 tt;
        tt.v[0] = (self->mpVertexPositionRender)[i].x;
        tt.v[1] = (self->mpVertexPositionRender)[i].y;
        tt.v[2] = (self->mpVertexPositionRender)[i].z;
        tt.v[3] = 1.0f;
        ((sVtx_3A39F8*)self->mpVertices)[i].pos = tt;
        AddPos_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i].pos, 0, self->mPosition.x + x);
        AddPos_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i].pos, 1, self->mPosition.y + y);
        ((sVtx_3A39F8*)self->mpVertices)[i].q = 1.0f;
        ((sVtx_3A39F8*)self->mpVertices)[i].s = 0.0f;
        ((sVtx_3A39F8*)self->mpVertices)[i].t = 0.0f;
        SetCol_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i], 3, (self->mpVertexColour)[i].A);
        SetCol_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i], 0, (self->mpVertexColour)[i].R);
        SetCol_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i], 1, (self->mpVertexColour)[i].G);
        SetCol_3A39F8(((sVtx_3A39F8*)self->mpVertices)[i], 2, (self->mpVertexColour)[i].B);
    }
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVE_3A39F8* vt = (sVE_3A39F8*)ps2GraphicsMan->mVtbl;
    ((tDraw2_3A39F8)vt[73].fn)((char*)ps2GraphicsMan + vt[73].delta, ((sCount_3A39F8*)(((char*)self) + 0x74))->count, self->mpVertices, 0);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_update);
#ifdef SKIP_ASM
extern "C" void cUIAnimation_updateThing(void*, void*, unsigned char, unsigned short);
extern "C" void cUIAnimation_updateFrame(void*, unsigned char);

/**
 * Advances a UI vector shape's animation, if it has one, to a frame.
 *
 * @param ev  the frame number (On Tour FrameNum)
 */
extern "C" void cUIVector_update(cUIVector* self, unsigned short ev)
{
    void* anim = self->mpAnim;
    if (anim != 0) {
        cUIAnimation_updateThing(anim, self, self->mAnimPlaybackStatus, ev);
        cUIAnimation_updateFrame(self->mpAnim, self->mAnimPlaybackStatus);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_getSize);
#ifdef SKIP_ASM

extern t3Vector t3vect_Zero;

/**
 * Returns a UI vector shape's size: the width and height of its vertices' bounding box.
 *
 * z is 0. The box starts from +/-1000, so vertices further out are not measured correctly. @p out is left
 * untouched when there are no vertex positions.
 *
 * @param out  receives the size
 */
extern "C" void cUIVector_getSize(cUIVector* self, t3Vector* out)
{
    if (self->mpVertexPosition == 0) {
        return;
    }
    *out = t3vect_Zero;
    float minX = 1000.0f;
    float maxX = -1000.0f;
    float minY = 1000.0f;
    float maxY = -1000.0f;
    unsigned int i;
    for (i = 0; i < *(unsigned int*)&self->mAttrVector >> 17; i++) {
        t3Vector* vertexPos = &(self->mpVertexPosition)[i];
        if (vertexPos->x < minX) {
            minX = vertexPos->x;
        }
        if (maxX < vertexPos->x) {
            maxX = vertexPos->x;
        }
        if (vertexPos->y < minY) {
            minY = vertexPos->y;
        }
        if (maxY < vertexPos->y) {
            maxY = vertexPos->y;
        }
    }
    out->x = maxX - minX;
    out->y = maxY - minY;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_setSize);
#ifdef SKIP_ASM

struct cUIVector_setSize_sCount74 {
    unsigned int lo : 17;
    unsigned int count : 15;
};

/**
 * Sets a UI vector's size, rebuilding its corners when it is a four-vertex quad.
 *
 * With exactly four vertices, vertex 0 stays put and the others become v0 + (x, 0), v0 + (x, y) and v0 + (0, y),
 * in order 1, 2, 3; the z offset is ignored. mSize is set to @p off whatever the vertex count.
 *
 * @param off  new width and height (x, y); z is only copied into mSize
 */
extern "C" void cUIVector_setSize(cUIVector* self, t3Vector* off)
{
    if (((cUIVector_setSize_sCount74*)&self->mAttrVector)->count == 4) {
        signed char i;
        for (i = 1; i < 4; i++) {
            (self->mpVertexPosition)[i] = (self->mpVertexPosition)[0];
        }
        (self->mpVertexPosition)[1].x += off->x;
        (self->mpVertexPosition)[3].y += off->y;
        (self->mpVertexPosition)[2].x += off->x;
        (self->mpVertexPosition)[2].y += off->y;
    }
    *(&self->mSize) = *off;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", func_003A3F08);
#ifdef SKIP_ASM
/**
 * Returns its first argument unchanged.
 *
 * A variadic function: it spills the remaining integer and float argument registers to its stack frame and uses none of
 * them. Nothing in retail calls or references it.
 *
 * @return the first argument
 */
extern "C" void* func_003A3F08(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", func_003A3F48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_3A3F48 {
    float r, g, b, a;
    sColor_3A3F48(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

void* cUISoundInterface_cUISoundInterface(void* self);
extern void* cUISoundInterface_vtbl[];
extern void* gUISoundNull;

struct sV2_3A3F48 {
    float x, y;
    sV2_3A3F48(float ax, float ay) : x(ax), y(ay) {}
};
extern sV2_3A3F48 D_004A5A60;

extern sColor_3A3F48 D_005083D0;
extern sColor_3A3F48 D_005083E0;
extern sColor_3A3F48 D_005083F0;
extern sColor_3A3F48 D_00508400;
extern sColor_3A3F48 D_00508410;
extern sColor_3A3F48 D_00508420;
extern sColor_3A3F48 D_00508430;
extern sColor_3A3F48 D_00508440;
extern sColor_3A3F48 D_00508450;
extern sColor_3A3F48 D_00508460;
extern sColor_3A3F48 D_00508470;
extern sColor_3A3F48 D_00508480;
extern sColor_3A3F48 D_00508490;
extern sColor_3A3F48 D_005084A0;
extern sColor_3A3F48 D_005084B0;
extern sColor_3A3F48 D_005084C0;
extern sColor_3A3F48 D_005084D0;
extern sColor_3A3F48 D_005084E0;
extern sColor_3A3F48 D_005084F0;
extern sColor_3A3F48 D_00508500;
extern sColor_3A3F48 D_00508510;
extern sColor_3A3F48 D_00508520;
extern sColor_3A3F48 D_00508530;
extern sColor_3A3F48 D_00508540;
extern sColor_3A3F48 D_00508550;
extern sColor_3A3F48 D_00508560;
extern sColor_3A3F48 D_00508570;
extern sColor_3A3F48 D_00508580;
extern sColor_3A3F48 D_00508590;
extern sColor_3A3F48 D_005085A0;
extern sColor_3A3F48 D_005085B0;
extern sColor_3A3F48 D_005085C0;
extern sColor_3A3F48 D_005085D0;
extern sColor_3A3F48 D_005085E0;
extern sColor_3A3F48 D_005085F0;
extern sColor_3A3F48 D_00508600;
extern sColor_3A3F48 D_00508610;
extern sColor_3A3F48 D_00508620;
extern sColor_3A3F48 D_00508630;
extern sColor_3A3F48 D_00508640;
extern sColor_3A3F48 D_00508650;
extern sColor_3A3F48 D_00508660;
extern sColor_3A3F48 D_00508670;
extern sColor_3A3F48 D_00508680;
extern sColor_3A3F48 D_00508690;
extern sColor_3A3F48 D_005086A0;
extern sColor_3A3F48 D_005086B0;
extern sColor_3A3F48 D_005086C0;
extern sColor_3A3F48 D_005086D0;
extern sColor_3A3F48 D_005086E0;
extern sColor_3A3F48 D_005086F0;
extern sColor_3A3F48 D_00508700;
extern sColor_3A3F48 D_00508710;
extern sColor_3A3F48 D_00508720;
extern sColor_3A3F48 D_00508730;
extern sColor_3A3F48 D_00508740;

// The unit declares func_003A3F48 later as returning void*; bind this body by asm label.
extern "C" void func_003A3F48_impl(int initialize, int priority) __asm__("func_003A3F48");

/**
 * Constructs or tears down the UI module's static objects (g++'s static initialisation and destruction routine).
 *
 * Only when @p priority is 0xFFFF: with @p initialize set it constructs 59 colour constants (D_005083D0 .. D_00508740,
 * each four floats, many 1/1/1/1 or 1/0/0/0), constructs the null UI sound interface gUISoundNull with cUISoundInterface_cUISoundInterface
 * and sets D_004A5A60, the default text shadow offset that uitext copies into each font, to (2, 2). With @p initialize
 * clear it only puts the base sound-interface vtable cUISoundInterface_vtbl back into gUISoundNull (its destructor). The static
 * initialiser func_003A6648 calls it with (1, 0xFFFF), the static destructor func_003A6668 with (0, 0xFFFF).
 *
 * @param initialize  non-zero to construct, zero to destroy
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_003A3F48_impl(int initialize, int priority)
{
    if (priority == 0xFFFF) {
        if (initialize != 0) {
        new (&D_005083D0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_005083E0) sColor_3A3F48(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_005083F0) sColor_3A3F48(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_00508400) sColor_3A3F48(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_00508410) sColor_3A3F48(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_00508420) sColor_3A3F48(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_00508430) sColor_3A3F48(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_00508440) sColor_3A3F48(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_00508450) sColor_3A3F48(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_00508460) sColor_3A3F48(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_00508470) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_00508480) sColor_3A3F48(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_00508490) sColor_3A3F48(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_005084A0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005084B0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005084C0) sColor_3A3F48(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_005084D0) sColor_3A3F48(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_005084E0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005084F0) sColor_3A3F48(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_00508500) sColor_3A3F48(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_00508510) sColor_3A3F48(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_00508520) sColor_3A3F48(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_00508530) sColor_3A3F48(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_00508540) sColor_3A3F48(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_00508550) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_00508560) sColor_3A3F48(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_00508570) sColor_3A3F48(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_00508580) sColor_3A3F48(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_00508590) sColor_3A3F48(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_005085A0) sColor_3A3F48(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_005085B0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005085C0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005085D0) sColor_3A3F48(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_005085E0) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_005085F0) sColor_3A3F48(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_00508600) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_00508610) sColor_3A3F48(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_00508620) sColor_3A3F48(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_00508630) sColor_3A3F48(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_00508640) sColor_3A3F48(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_00508650) sColor_3A3F48(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_00508660) sColor_3A3F48(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_00508670) sColor_3A3F48(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_00508680) sColor_3A3F48(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_00508690) sColor_3A3F48(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_005086A0) sColor_3A3F48(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_005086B0) sColor_3A3F48(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_005086C0) sColor_3A3F48(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_005086D0) sColor_3A3F48(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_005086E0) sColor_3A3F48(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_005086F0) sColor_3A3F48(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_00508700) sColor_3A3F48(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_00508710) sColor_3A3F48(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_00508720) sColor_3A3F48(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_00508730) sColor_3A3F48(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_00508740) sColor_3A3F48(1.0f, 1.0f, 0.0f, 0.0f);
            cUISoundInterface_cUISoundInterface(&gUISoundNull);
            new (&D_004A5A60) sV2_3A3F48(2.0f, 2.0f);
        } else {
            gUISoundNull = cUISoundInterface_vtbl;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIAnimation_cUIAnimation1);
#ifdef SKIP_ASM
extern void* cUIAnimation_vtbl[];
/**
 * Constructs a cUIAnimation with no data: stores cUIAnimation_vtbl in its vptr (+0x10) and returns it.
 *
 * Unlike cUIAnimation_cUIAnimation it sets no animation header or channels. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIAnimation_cUIAnimation1(cUIAnimation* self) {
    self->mVtbl = cUIAnimation_vtbl;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIAnimation_getHashName);
#ifdef SKIP_ASM
/**
 * Returns the animation's hash name: the word at +0x8 of the animation header its first member points to.
 *
 * @param anim  the cUIAnimation
 * @return the header's hash id
 */
extern "C" int cUIAnimation_getHashName(void **anim) {
    return (*(int *)((char*)(*anim) + (8)));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIAnimation_setFrameNumber);
#ifdef SKIP_ASM
/**
 * Sets the animation's current frame number (the short at +0xA) to @p frame.
 *
 * @param anim  the cUIAnimation
 * @param frame  the frame number
 */
extern "C" void cUIAnimation_setFrameNumber(cUIAnimation* anim, short frame) {
    (anim->mFrameNum) = frame;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListOperator__cListOperator);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cListOperator_vtbl[];

/**
 * Destroys a list operator: restores cListOperator's vtable pointer, then frees the object when asked to.
 *
 * List operators are the functor objects the UI applies to each node of a list (cUIGroup_setFocus uses a
 * cListOpSetFocus).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cListOperator__cListOperator(int* self, int flags)
{
    *(void***)self = cListOperator_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListNode_cListNode);
#ifdef SKIP_ASM
extern void* cListNode_vtbl[];

/**
 * Constructs a list node linked to itself (mpNext = mpPrev = the node) with cListNode_vtbl.
 *
 * @param self  the node being constructed (the this pointer)
 * @return @p self
 */
extern "C" void *cListNode_cListNode(cListNode* self) {
    ((void*)self->mpNext) = ((void*)self);
    ((int*)self->mVtbl) = (int*)&cListNode_vtbl;
    ((void*)self->mpPrev) = ((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListNode__cListNode);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cListNode_destroyListNode(cListNode* self);

extern void* cListNode_vtbl[];

/**
 * Destroys a list node: restores cListNode_vtbl, unlinks it (cListNode_destroyListNode, cListNode::~cListNode) and frees it when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cListNode__cListNode(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

extern "C" void cListNode_removeFromList(void*);

//100%
INCLUDE_ASM("ui/uivector", cListNode_prev);
#ifdef SKIP_ASM
/**
 * Returns the list node's previous node (mpPrev).
 *
 * @param node  the cListNode
 * @return mpPrev
 */
extern "C" int cListNode_prev(int *node) {
    return *node;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cListNode_next);
#ifdef SKIP_ASM
/**
 * Returns the list node's next node (mpNext).
 *
 * @param listNode  the cListNode
 * @return mpNext
 */
extern "C" int cListNode_next(cListNode* listNode) {
    return ((int)listNode->mpNext);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cListNode_setPrev);
#ifdef SKIP_ASM
/**
 * Sets the list node's previous node (mpPrev) to @p prev.
 *
 * @param node  the cListNode
 * @param prev  the new previous node
 */
extern "C" void cListNode_setPrev(int *node, int prev) {
    *node = prev;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cListNode_setNext);
#ifdef SKIP_ASM
/**
 * Sets the list node's next node (mpNext) to @p next.
 *
 * @param node  the cListNode
 * @param next  the new next node
 */
extern "C" void cListNode_setNext(void *node, int next) {
    (*(int *)((char*)(node) + (4))) = next;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListNode_destroyListNode);
#ifdef SKIP_ASM
/**
 * Unlinks the node from its list (cListNode_removeFromList).
 */
extern "C" void cListNode_destroyListNode(cListNode* self)
{
    cListNode_removeFromList(self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cList__cList);
#ifdef SKIP_ASM
extern "C" void* cList_destroyList(void*);
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Destroys a list: restores cList's vtable, destroys its nodes, then unlinks its end and start sentinel
 * nodes.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cList__cList(cList* self, int flags)
{
    self->mVtbl = cList_vtbl;
    cList_destroyList(self);
    self->mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)&self->mEnd);
    self->mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_cUIThing1);
#ifdef SKIP_ASM
struct sFlags_6548;
extern "C" sFlags_6548* tAttrThing_tAttrThing(sFlags_6548* f);
extern void* cUIThing_vtbl[];

/**
 * Constructs a bare cUIThing: links the node to itself (mpPrev = mpNext = this), stores cUIThing_vtbl and constructs
 * the tAttrThing word mAttr with tAttrThing_tAttrThing; no other member is set.
 *
 * Unlike cUIThing_cUIThing it takes no screen or parent. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIThing_cUIThing1(cUIThing* self)
{
    self->mpNext = (cListNode*)((char*)self);
    self->mpPrev = (cListNode*)((char*)self);
    self->mVtbl = cUIThing_vtbl;
    tAttrThing_tAttrThing((sFlags_6548*)(&self->mAttr));
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isPlayingAnimation);
#ifdef SKIP_ASM
/**
 * Returns whether the UI thing has an animation attached (mpAnim is not null).
 *
 * @param uiThing  the cUIThing
 * @return 1 when mpAnim is set, else 0
 */
extern "C" int cUIThing_isPlayingAnimation(cUIThing* uiThing) {
    return ((int)uiThing->mpAnim) != 0;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getAnimation);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's current animation (mpAnim).
 *
 * @param uiThing  the cUIThing
 * @return mpAnim
 */
extern "C" int cUIThing_getAnimation(cUIThing* uiThing) {
    return ((int)uiThing->mpAnim);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isHorizontal);
#ifdef SKIP_ASM
/**
 * Returns whether the UI thing is horizontal: 1 when the orientation bit (mAttr bit 0) is clear.
 *
 * @param uiThing  the cUIThing
 * @return 1 for horizontal, 0 for vertical
 */
extern "C" int cUIThing_isHorizontal(cUIThing* uiThing) {
    return ((uiThing->mAttr) ^ 1) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isVertical);
#ifdef SKIP_ASM
/**
 * Returns whether the UI thing is vertical: the orientation bit (mAttr bit 0).
 *
 * @param uiThing  the cUIThing
 * @return 1 for vertical, 0 for horizontal
 */
extern "C" int cUIThing_isVertical(cUIThing* uiThing) {
    return (uiThing->mAttr) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isActive);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's active flag (mAttr bit 1).
 *
 * @param uiThing  the cUIThing
 * @return 1 when active, else 0
 */
extern "C" int cUIThing_isActive(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 1) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isCached);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's cached flag (mAttr bit 2).
 *
 * @param uiThing  the cUIThing
 * @return 1 when cached, else 0
 */
extern "C" int cUIThing_isCached(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 2) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isEnabled);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's enabled flag (mAttr bit 3).
 *
 * @param uiThing  the cUIThing
 * @return 1 when enabled, else 0
 */
extern "C" int cUIThing_isEnabled(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 3) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isFocused);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's focused flag (mAttr bit 4).
 *
 * @param uiThing  the cUIThing
 * @return 1 when focused, else 0
 */
extern "C" int cUIThing_isFocused(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 4) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isStatic);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's static flag (mAttr bit 5).
 *
 * @param uiThing  the cUIThing
 * @return 1 when static, else 0
 */
extern "C" int cUIThing_isStatic(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 5) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isVisible);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's visible flag (mAttr bit 6).
 *
 * @param uiThing  the cUIThing
 * @return 1 when visible, else 0
 */
extern "C" int cUIThing_isVisible(cUIThing* uiThing) {
    return ((int) (uiThing->mAttr) >> 6) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_isWrap);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's wrap flag (mAttr bit 7).
 *
 * @param uiThing  the cUIThing
 * @return 1 when wrapping is on, else 0
 */
extern "C" int cUIThing_isWrap(cUIThing* uiThing) {
    int wrapBits = uiThing->mAttr >> 7;
    return wrapBits & 1;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setActive);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as active or not.
 *
 * Sets mActive, bit 1 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setActive(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit1 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setCached);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as cached or not.
 *
 * Sets mCached, bit 2 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setCached(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit2 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setEnabled);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as enabled or not.
 *
 * Sets mEnabled, bit 3 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setEnabled(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit3 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setFocus);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as focused or not.
 *
 * Sets mFocused, bit 4 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setFocus(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit4 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setStatic);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as static or not.
 *
 * Sets mStatic, bit 5 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setStatic(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit5 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setVisible);
#ifdef SKIP_ASM
/**
 * Marks a UI thing as visible or not.
 *
 * Sets mVisible, bit 6 of the attribute word mAttr, to @p enable != 0.
 *
 * @param enable  nonzero to set the flag, 0 to clear it
 */
extern "C" void cUIThing_setVisible(cUIThing* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttr)->bit6 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setWrap);
#ifdef SKIP_ASM
struct sUIVectorFlags8 {
    bool bit0 : 1;
    bool bit1 : 1;
    bool bit2 : 1;
    bool bit3 : 1;
    bool bit4 : 1;
    bool bit5 : 1;
    bool bit6 : 1;
    bool bit7 : 1;
};
/**
 * Sets the wrap flag (mWrap, bit 7 of mAttr).
 *
 * @param enable  non-zero to wrap
 */
extern "C" void cUIThing_setWrap(cUIThing* self, int enable)
{
    ((sUIVectorFlags8*)&self->mAttr)->bit7 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getMiscData);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's caller-defined data word (mMiscData).
 *
 * @param uiThing  the cUIThing
 * @return mMiscData
 */
extern "C" int cUIThing_getMiscData(cUIThing* uiThing) {
    return (uiThing->mMiscData);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setMiscData);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's caller-defined data word (mMiscData) to @p miscData.
 *
 * @param uiThing  the cUIThing
 * @param miscData  the value to store
 */
extern "C" void cUIThing_setMiscData(cUIThing* uiThing, int miscData) {
    (uiThing->mMiscData) = miscData;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setOrientation);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's orientation bit (mAttr bit 0) to the low bit of @p orientation (0 horizontal, 1 vertical).
 *
 * @param uiThing  the cUIThing
 * @param orientation  0 for horizontal, 1 for vertical
 */
extern "C" void cUIThing_setOrientation(cUIThing* uiThing, int orientation) {
    (uiThing->mAttr) = (int) (((uiThing->mAttr) & ~1) | (orientation & 1));
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setColour);
#ifdef SKIP_ASM

/**
 * Sets the UI thing's colour by copying a 16-byte tARGB value into mColour.
 *
 * @param in  colour to copy
 */
extern "C" void cUIThing_setColour(cUIThing* self, tARGB* in)
{
    *(&self->mColour) = *in;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_getColour);
#ifdef SKIP_ASM
/**
 * Returns the address of the UI thing's colour (mColour).
 *
 * @param uiThing  the cUIThing (as an address)
 * @return a pointer to mColour
 */
extern "C" int cUIThing_getColour(int uiThing) {
    return uiThing + 0x1C;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getColour1);
#ifdef SKIP_ASM
/**
 * Copies the UI thing's colour (mColour) into @p out.
 *
 * @param out  receives the colour (a, r, g, b)
 */
extern "C" void cUIThing_getColour1(cUIThing* self, tARGB* out)
{
    *out = *(&self->mColour);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setRadAngle);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's rotation in radians (mAngle.z) to @p fparg0.
 *
 * @param uiThing  the cUIThing
 * @param fparg0  the angle in radians
 */
extern "C" void cUIThing_setRadAngle(cUIThing* uiThing, float fparg0) {
    (uiThing->mAngle.z) = fparg0;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getRadAngle);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's rotation in radians (mAngle.z).
 *
 * @param uiThing  the cUIThing
 * @return mAngle.z
 */
extern "C" float cUIThing_getRadAngle(cUIThing* uiThing) {
    return (uiThing->mAngle.z);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getDrawLayer);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's draw layer (mAttr bits 8-12, 0..31).
 *
 * @param uiThing  the cUIThing
 * @return the draw layer
 */
extern "C" int cUIThing_getDrawLayer(cUIThing* uiThing) {
    return ((unsigned int) ((unsigned int)uiThing->mAttr) >> 8) & 0x1F;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setDrawLayer);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's draw layer (mAttr bits 8-12) to the low 5 bits of @p layer.
 *
 * @param uiThing  the cUIThing
 * @param layer  the draw layer (0..31)
 */
extern "C" void cUIThing_setDrawLayer(cUIThing* uiThing, int layer) {
    (uiThing->mAttr) = (int) (((uiThing->mAttr) & ~0x1F00) | ((layer & 0x1F) << 8));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setPivot);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's pivot (mAttr bits 13-18) to the low 6 bits of @p pivot.
 *
 * @param uiThing  the cUIThing
 * @param pivot  the pivot code (0..63; tAttrThing_tAttrThing starts it at 9)
 */
extern "C" void cUIThing_setPivot(cUIThing* uiThing, int pivot) {
    (uiThing->mAttr) = (int) (((uiThing->mAttr) & 0xFFF81FFF) | ((pivot & 0x3F) << 13));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getPivot);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's pivot (mAttr bits 13-18, 0..63).
 *
 * @param uiThing  the cUIThing
 * @return the pivot code
 */
extern "C" int cUIThing_getPivot(cUIThing* uiThing) {
    return ((unsigned int) ((unsigned int)uiThing->mAttr) >> 0xD) & 0x3F;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setHashName);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's hash name (mHashName) to @p hashName.
 *
 * @param uiThing  the cUIThing
 * @param hashName  the hashed name
 */
extern "C" void cUIThing_setHashName(cUIThing* uiThing, int hashName) {
    (uiThing->mHashName) = hashName;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getHashName);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's hash name (mHashName).
 *
 * @param uiThing  the cUIThing
 * @return mHashName
 */
extern "C" int cUIThing_getHashName(cUIThing* uiThing) {
    return (uiThing->mHashName);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_onInput__FPv);
#ifdef SKIP_ASM
/**
 * Does no input handling for a UI thing and reports the input as not handled.
 *
 * @return always 0
 */
int cUIThing_onInput(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_updateLocale__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cUIThing's updateLocale has an empty body.
 */
void cUIThing_updateLocale(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_render2D__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cUIThing's render2D has an empty body.
 */
void cUIThing_render2D(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_render3D__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: cUIThing's render3D has an empty body.
 */
void cUIThing_render3D(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setParent);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's parent (mpParent, at +0x40) to @p parent.
 *
 * @param uiThing  the cUIThing
 * @param parent  the new parent thing
 */
extern "C" void cUIThing_setParent(void *uiThing, int parent) {
    (*(int *)((char*)(uiThing) + (0x40))) = parent;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getParent);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's parent (mpParent).
 *
 * @param uiThing  the cUIThing
 * @return mpParent
 */
extern "C" int cUIThing_getParent(cUIThing* uiThing) {
    return ((int)uiThing->mpParent);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getPosition);
#ifdef SKIP_ASM
/**
 * Copies mPosition into @p out.
 *
 * @param out  receives the position
 */
extern "C" void cUIThing_getPosition(cUIThing* self, t3Vector* out)
{
    *out = *(&self->mPosition);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setPosition);
#ifdef SKIP_ASM
/**
 * Sets mPosition from @p out.
 *
 * @param out  the new position (read, despite its name; On Tour vec)
 */
extern "C" void cUIThing_setPosition(cUIThing* self, t3Vector* out)
{
    *(&self->mPosition) = *out;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getScale);
#ifdef SKIP_ASM
/**
 * Copies mScale into @p out.
 *
 * @param out  receives the scale
 */
extern "C" void cUIThing_getScale(cUIThing* self, t3Vector* out)
{
    *out = *(&self->mScale);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_setScale);
#ifdef SKIP_ASM
/**
 * Sets mScale from @p out.
 *
 * @param out  the new scale (read, despite its name; On Tour vec)
 */
extern "C" void cUIThing_setScale(cUIThing* self, t3Vector* out)
{
    *(&self->mScale) = *out;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIThing_getScreen);
#ifdef SKIP_ASM
/**
 * Returns the screen the UI thing belongs to (mpScreen).
 *
 * @param uiThing  the cUIThing
 * @return mpScreen
 */
extern "C" int cUIThing_getScreen(cUIThing* uiThing) {
    return ((int)uiThing->mpScreen);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_getSize);
#ifdef SKIP_ASM

/**
 * Copies the UI thing's size (mSize) into a caller-supplied vector.
 *
 * @param out  receives the size
 */
extern "C" void cUIThing_getSize(cUIThing* self, t3Vector* out)
{
    *out = *(&self->mSize);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThing_setSize);
#ifdef SKIP_ASM
/**
 * Sets the UI thing's size by copying a vector into mSize.
 *
 * @param in  new size
 */
extern "C" void cUIThing_setSize(cUIThing* self, t3Vector* in)
{
    *(&self->mSize) = *in;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListOpOnInput__cListOpOnInput);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cListOperator_vtbl[];

/**
 * Destroys an on-input list operator: restores the base cListOperator vtable, then frees the object when asked
 * to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cListOpOnInput__cListOpOnInput(int* self, int flags)
{
    *(void***)self = cListOperator_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListOpOnInput_onListNode);
#ifdef SKIP_ASM

/**
 * Offers the operator's controller input to one list node.
 *
 * Calls the node's onInput (vtable slot 15) with the input kept at offset 4 of the operator.
 *
 * @param obj  list node (a UI thing) to try
 * @return @p obj when it handled the input, 0 otherwise
 */
extern "C" void* cListOpOnInput_onListNode(cList* self, cListNode* obj)
{
    sVEntry<int(void*, void*)>* vt = (sVEntry<int(void*, void*)>*)obj->mVtbl;
    if (vt[0xF].fn((char*)obj + vt[0xF].delta, self->mStart.mpNext) == 0) {
        return 0;
    }
    return ((void*)obj);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListOpSetFocus__cListOpSetFocus);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cListOperator_vtbl[];

/**
 * Destroys a set-focus list operator: restores the base cListOperator vtable, then frees the object when asked
 * to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cListOpSetFocus__cListOpSetFocus(int* self, int flags)
{
    *(void***)self = cListOperator_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cListOpSetFocus_onListNode);
#ifdef SKIP_ASM
class cListOpSetFocus_onListNode_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void setFocus(int);
};

/**
 * List operator callback that sets the focus state of one UI node.
 *
 * Calls the node's setFocus (slot 7) with the operator's value (+0x4).
 *
 * @param obj  the node visited
 * @return 0, so the traversal continues to every node
 */
extern "C" int cListOpSetFocus_onListNode(cListOpSetFocus* self, cListOpSetFocus_onListNode_cVirt* obj)
{
    obj->setFocus(self->mbState);
    return 0;
}
#endif

extern void* cUIIcon_vtbl[];
extern "C" void* cUIThing__cUIThing(void*);

//100%
INCLUDE_ASM("ui/uivector", cUISoundInterface__cUISoundInterface1);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cUISoundInterface_vtbl[];

/**
 * Destroys a UI sound interface: puts back the base sound-interface vtable cUISoundInterface_vtbl and frees the object with
 * operator_delete when bit 0 of @p flags is set.
 *
 * The same code as cUISoundInterface__cUISoundInterface (fe/fepopupmisc), which cUISoundInterface_vtbl's slot points to; nothing in
 * retail references this copy.
 *
 * @param flags  bit 0 set to free the memory
 */
extern "C" void cUISoundInterface__cUISoundInterface1(int* self, int flags)
{
    *(void***)self = cUISoundInterface_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISoundInterface_playSound1);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function: the base UI sound interface's play-sound method, a copy of
 * cUISoundInterface_playSound__FPv). Nothing in retail references this copy.
 */
extern "C" void cUISoundInterface_playSound1(void) {

}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISoundInterface_soundInitialize1);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function: the base UI sound interface's initialise method, a copy of
 * cUISoundInterface_soundInitialize__FPv). Nothing in retail references this copy.
 */
extern "C" void cUISoundInterface_soundInitialize1(void) {

}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISoundInterface_soundShutdown1);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function: the base UI sound interface's shutdown method, a copy of
 * cUISoundInterface_soundShutdown__FPv). Nothing in retail references this copy.
 */
extern "C" void cUISoundInterface_soundShutdown1(void) {

}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_cUIIcon1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" void* func_0021CCC8(void* p);
extern void* cUIIcon_vtbl[];

/**
 * Constructs a cUIIcon with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIIcon_vtbl and constructs
 * the tAttrIcon word mAttrIcon (+0x74) with func_0021CCC8.
 *
 * Unlike cUIIcon_cUIIcon it leaves mTextureID and mpTextureElem unset. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIIcon_cUIIcon1(cUIIcon* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIIcon_vtbl;
    func_0021CCC8(&self->mAttrIcon);
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_cUIIcon);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern "C" void* func_0021CCC8(void* p);
extern void* cUIIcon_vtbl[];

/**
 * Constructs a UI icon: the cUIThing constructor (cUIThing_cUIThing, declared with only self, so the screen and parent
 * pass through in $a1 / $a2), cUIIcon_vtbl, the icon attributes at +0x74 (func_0021CCC8), mTextureID -1 and
 * mpTextureElem 0.
 *
 * @return @p self
 */
extern "C" void* cUIIcon_cUIIcon(cUIIcon* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUIIcon_vtbl;
    func_0021CCC8(&self->mAttrIcon);
    self->mTextureID = -1;
    self->mpTextureElem = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIIcon__cUIIcon__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a UI icon: restores cUIIcon_vtbl, then runs the cUIThing destructor.
 *
 * Only @p self is declared: the destructor flags the caller passed in the second argument register reach
 * cUIThing__cUIThing() unchanged.
 *
 * @return no meaningful value (the cUIThing destructor returns nothing)
 */
void* cUIIcon__cUIIcon(void* vself)
{
    cUIIcon* self = (cUIIcon*)vself;
    self->mVtbl = (void**)((int)(void*)cUIIcon_vtbl);
    return cUIThing__cUIThing(self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIIcon_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of an icon.
 *
 * @return always 0x11 (groups return 0x10, menus 0x13)
 */
int cUIIcon_getType(void* self)
{
    return 17;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", tAttrIcon_tAttrIcon);
#ifdef SKIP_ASM
/**
 * Clears the low four bits (mIconTiled, mIconFlip, mIconRotate) of a cUIIcon's tAttrIcon byte and returns it: the
 * tAttrIcon constructor.
 *
 * cUIIcon's constructors use func_0021CCC8 for the same job; nothing in retail calls this copy.
 *
 * @param attr  the tAttrIcon (cUIIcon + 0x74)
 * @return @p attr
 */
extern "C" unsigned char *tAttrIcon_tAttrIcon(unsigned char *attr) {
    *attr &= 0xF0;
    return attr;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_isTiled);
#ifdef SKIP_ASM
/**
 * Returns whether the icon is drawn tiled (mAttrIcon bit 0, mIconTiled).
 *
 * @param uiIcon  the cUIIcon
 * @return 1 when tiled, else 0
 */
extern "C" int cUIIcon_isTiled(cUIIcon* uiIcon) {
    return (*(int*)&uiIcon->mAttrIcon) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setTiled);
#ifdef SKIP_ASM
/**
 * Sets the icon's tiled flag (mAttrIcon bit 0, mIconTiled) to @p tiled != 0.
 *
 * @param uiIcon  the cUIIcon
 * @param tiled  non-zero to draw the texture tiled
 */
extern "C" void cUIIcon_setTiled(cUIIcon* uiIcon, int tiled) {
    *(int*)&uiIcon->mAttrIcon = (int) (((*(int*)&uiIcon->mAttrIcon) & ~1) | (tiled != 0));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_isIconFlip);
#ifdef SKIP_ASM
/**
 * Returns the icon's flip flag (mAttrIcon bit 1, mIconFlip).
 *
 * @param uiIcon  the cUIIcon
 * @return 1 when flipped, else 0
 */
extern "C" int cUIIcon_isIconFlip(cUIIcon* uiIcon) {
    return ((int) (*(int*)&uiIcon->mAttrIcon) >> 1) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setIconFlip);
#ifdef SKIP_ASM
/**
 * Sets the icon's flip flag (mAttrIcon bit 1, mIconFlip) to the low bit of @p enable.
 *
 * @param enable  1 to flip the icon, 0 not to
 */
extern "C" void cUIIcon_setIconFlip(cUIIcon* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrIcon)->bit1 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_isIconRotate);
#ifdef SKIP_ASM
/**
 * Returns whether the icon is drawn rotated: whether its 2-bit rotate field (mAttrIcon bits 2-3, mIconRotate) is
 * non-zero.
 *
 * @param uiIcon  the cUIIcon
 * @return 1 when rotated, else 0
 */
extern "C" int cUIIcon_isIconRotate(cUIIcon* uiIcon) {
    return ((*(int*)&uiIcon->mAttrIcon) & 0xC) != 0;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setIconRotate);
#ifdef SKIP_ASM
struct sUIVectorFlagsB {
    bool bit0 : 1;
    bool bit1 : 1;
    bool bits23 : 2;
};
/**
 * Sets the icon's rotate field (mAttrIcon bits 2-3, mIconRotate) to 1 when @p enable is non-zero, else to 0 (the field
 * is written through a bool bitfield, so the value 2 is never stored).
 *
 * @param enable  non-zero to draw the icon rotated
 */
extern "C" void cUIIcon_setIconRotate(cUIIcon* self, int enable)
{
    ((sUIVectorFlagsB*)&self->mAttrIcon)->bits23 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_getElement);
#ifdef SKIP_ASM
/**
 * Returns the icon's texture element (mpTextureElem; null when the icon draws by texture id).
 *
 * @param uiIcon  the cUIIcon
 * @return mpTextureElem
 */
extern "C" int cUIIcon_getElement(cUIIcon* uiIcon) {
    return ((int)uiIcon->mpTextureElem);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setElement);
#ifdef SKIP_ASM
/**
 * Makes the icon draw the texture element @p textureElem: stores it in mpTextureElem and sets mTextureID to -1.
 *
 * @param icon  the cUIIcon
 * @param textureElem  the cTextureElement to draw
 */
extern "C" void cUIIcon_setElement(void *icon, int textureElem) {
    (*(int *)((char*)(icon) + (0x7C))) = textureElem;
    (*(int *)((char*)(icon) + (0x78))) = -1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_getTexture);
#ifdef SKIP_ASM
/**
 * Returns the icon's texture id (mTextureID; -1 when it draws a texture element instead).
 *
 * @param uiIcon  the cUIIcon
 * @return mTextureID
 */
extern "C" int cUIIcon_getTexture(cUIIcon* uiIcon) {
    return (uiIcon->mTextureID);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setTexture);
#ifdef SKIP_ASM
/**
 * Makes the icon draw texture @p textureId: stores it in mTextureID and clears mpTextureElem.
 *
 * @param icon  the cUIIcon
 * @param textureId  the texture id
 */
extern "C" void cUIIcon_setTexture(void *icon, int textureId) {
    (*(int *)((char*)(icon) + (0x78))) = textureId;
    (*(int *)((char*)(icon) + (0x7C))) = 0;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_getTileUV);
#ifdef SKIP_ASM
struct sVec2 { float x; float y; };
/**
 * Copies the icon's tile counts (mTileUV, two floats: repeats across and down) into @p out and returns @p out.
 *
 * @param out  the result buffer (g++'s hidden return pointer for a t2Vector returned by value)
 * @return @p out
 */
extern "C" sVec2* cUIIcon_getTileUV(sVec2* out, cUIIcon* self)
{
    *out = *(sVec2*)&self->mTileUV[0];
    return out;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setTileUV);
#ifdef SKIP_ASM
/**
 * Sets the icon's tile counts (mTileUV) from the two floats at @p in (repeats across, repeats down).
 *
 * @param in  the new tile counts (u, v)
 */
extern "C" void cUIIcon_setTileUV(cUIIcon* self, sVec2* in)
{
    *(sVec2*)&self->mTileUV[0] = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIIcon_setTileUV1);
#ifdef SKIP_ASM
/**
 * Sets the icon's tile counts (mTileUV) to @p x across and @p y down.
 *
 * @param x  how many times the texture repeats across
 * @param y  how many times the texture repeats down
 */
extern "C" void cUIIcon_setTileUV1(cUIIcon* self, float x, float y)
{
    sVec2 tileUv;
    tileUv.x = x;
    tileUv.y = y;
    *(sVec2*)&self->mTileUV[0] = tileUv;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_cUIGroup1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" void* cList_cList(void* p);
extern void* cUIGroup_vtbl[];

/**
 * Constructs a cUIGroup with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIGroup_vtbl and
 * constructs the child list mList (+0x74) with cList_cList.
 *
 * Nothing in retail calls it; cUIScreen_createObjectByStruct uses cUIGroup_cUIGroup.
 *
 * @return the object
 */
extern "C" void* cUIGroup_cUIGroup1(cUIGroup* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIGroup_vtbl;
    cList_cList(&self->mList);
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_cUIGroup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern "C" void* cList_cList(void* p);
extern void* cUIGroup_vtbl[];

/**
 * Constructs a UI group: the cUIThing constructor (cUIThing_cUIThing; the screen and parent pass through in $a1 / $a2),
 * cUIGroup_vtbl and its child list at +0x74 (cList::cList).
 *
 * @return @p self
 */
extern "C" void* cUIGroup_cUIGroup(cUIGroup* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUIGroup_vtbl;
    cList_cList(&self->mList);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIGroup__cUIGroup);
#ifdef SKIP_ASM
extern void* cUIGroup_vtbl[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];
// PORT: called here for its side effect only; declared void so the scheduling matches.
extern "C" void cList_destroyList_v(void* p) __asm__("cList_destroyList");
extern "C" void cListNode_destroyListNode(cListNode* p);
// PORT: the unit declares cUIThing__cUIThing with one argument; the dtor chain passes the delete flag too.
extern "C" void cUIThing__cUIThing_2(void* self, int flag) __asm__("cUIThing__cUIThing");

/**
 * Destroys a UI group: destroys its child list (+0x74) and the list's sentinel nodes, then the cUIThing part.
 *
 * @param flag  g++ destructor flags, passed to cUIThing's destructor (bit 0 frees the object)
 */
extern "C" void cUIGroup__cUIGroup(cUIGroup* self, int flag)
{
    self->mVtbl = cUIGroup_vtbl;
    self->mList.mVtbl = cList_vtbl;
    cList_destroyList_v(&self->mList);
    self->mList.mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)(&self->mList.mEnd));
    self->mList.mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)(&self->mList));
    cUIThing__cUIThing_2(((char*)self), flag);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIGroup_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a group.
 *
 * @return always 0x10 (vectors return 0x18)
 */
int cUIGroup_getType(void* self)
{
    return 0x10;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIGroup_first);
#ifdef SKIP_ASM
extern "C" void cList_first__FP5cList(int);

/**
 * Returns the group's first child: cList_first__FP5cList on its child list mList (+0x74), whose result is passed back.
 *
 * @param group  the cUIGroup (as an address)
 */
extern "C" void cUIGroup_first(int group) {
    cList_first__FP5cList(group + 116);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_last);
#ifdef SKIP_ASM
extern "C" void cList_last(int);

/**
 * Returns the group's last child: cList_last (the list's last node) on its child list mList (+0x74), whose result is
 * passed back.
 *
 * @param group  the cUIGroup (as an address)
 */
extern "C" void cUIGroup_last(int group) {
    cList_last(group + 116);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_getByIndex);
#ifdef SKIP_ASM
extern "C" void cList_getByIndex(int);

/**
 * Returns the group's child at an index: cList_getByIndex on its child list mList (+0x74). The index the caller passes
 * in the second argument register goes through untouched, and the list's result is passed back.
 *
 * @param group  the cUIGroup (as an address)
 */
extern "C" void cUIGroup_getByIndex(int group) {
    cList_getByIndex(group + 116);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_addToFront);
#ifdef SKIP_ASM
extern "C" void cList_addToFront__FP5cListP9cListNode(int);

/**
 * Links a node at the front of the group's child list: cList_addToFront__FP5cListP9cListNode on mList (+0x74), with the
 * node the caller passes in the second argument register going through untouched.
 *
 * @param group  the cUIGroup (as an address)
 */
extern "C" void cUIGroup_addToFront(int group) {
    cList_addToFront__FP5cListP9cListNode(group + 116);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIGroup_addToEnd);
#ifdef SKIP_ASM
extern "C" void cList_addToEnd__FP5cListP9cListNode(int);

/**
 * Links a node at the end of the group's child list: cList_addToEnd__FP5cListP9cListNode on mList (+0x74), with the
 * node the caller passes in the second argument register going through untouched.
 *
 * @param group  the cUIGroup (as an address)
 */
extern "C" void cUIGroup_addToEnd(int group) {
    cList_addToEnd__FP5cListP9cListNode(group + 116);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIGroup_traverseFlat);
#ifdef SKIP_ASM
class cUIGroup_traverseFlat_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int onListNode(void*);
};

/**
 * Applies a list operator to the group itself, without visiting its children.
 *
 * Returns the operator's onListNode (vtable slot 2 of the operator, whose vtable pointer is its first word) for
 * the group, the same as cListNode's own traverseFlat.
 *
 * @param self    the group
 * @param obj  the list operator (On Tour: pOperator)
 * @return the operator's onListNode result
 */
extern "C" int cUIGroup_traverseFlat(cUIGroup* self, cUIGroup_traverseFlat_cVirt* obj)
{
    return obj->onListNode(self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIGroup_traverseRecurse);
#ifdef SKIP_ASM
class cUIGroup_traverseRecurse_cFunctor {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int onListNode(void*);
};

extern "C" void* cList_traverseRecurse(void* list, cUIGroup_traverseRecurse_cFunctor* fn);

/**
 * Runs a list operator on a UI group and then, recursively, on its children.
 *
 * Stops at the group itself when the operator's callback (slot 2) returns nonzero; otherwise traverses the
 * child list (+0x74) with cList_traverseRecurse().
 *
 * @param fn  the list operator (On Tour cListOperator*)
 * @return the node the operator stopped at, or 0 if none
 */
extern "C" void* cUIGroup_traverseRecurse(cUIGroup* self, cUIGroup_traverseRecurse_cFunctor* fn)
{
    if (fn->onListNode(self) != 0) {
        return ((void*)self);
    }
    return cList_traverseRecurse((char*)&self->mList, fn);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIMenu_cUIMenu);
#ifdef SKIP_ASM
extern void* cUIMenu_vtbl[];
// PORT: the other ctor in the unit calls this base ctor with (self, 0, 0)
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern "C" void* cList_cList(void* p);
extern "C" void* tAttrMenu_tAttrMenu(void* self);

/**
 * Constructs an empty menu.
 *
 * Runs the cUIThing constructor (cUIThing_cUIThing; the screen and parent, On Tour pScreen / parent, are not declared
 * here and stay in $a1/$a2), stores cUIGroup_vtbl and constructs the item list (+0x74), then stores cUIMenu_vtbl,
 * constructs mAttrMenu (tAttrMenu_tAttrMenu), sets mStoredIndex 0xFF and clears mScrollItemCurrent, mScrollItemNum,
 * mScrollItemNumVisible, mScrollItemOffset, mpScrollBar and mpSelected.
 *
 * @return @p self
 */
extern "C" void* cUIMenu_cUIMenu(cUIMenu* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUIGroup_vtbl;
    cList_cList(&self->mList);
    self->mVtbl = cUIMenu_vtbl;
    tAttrMenu_tAttrMenu(&self->mAttrMenu);
    self->mStoredIndex = 0xFF;
    self->mScrollItemCurrent = 0;
    self->mScrollItemNum = 0;
    self->mScrollItemNumVisible = 0;
    self->mScrollItemOffset = 0;
    self->mpScrollBar = 0;
    self->mpSelected = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIMenu__cUIMenu);
#ifdef SKIP_ASM
extern void* cUIGroup_vtbl[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];
// PORT: called here for its side effect only; declared void so the scheduling matches.
extern "C" void cList_destroyList_v(void* p) __asm__("cList_destroyList");
extern "C" void cListNode_destroyListNode(cListNode* p);
// PORT: the unit declares cUIThing__cUIThing with one argument; the dtor chain passes the delete flag too.
extern "C" void cUIThing__cUIThing_2(void* self, int flag) __asm__("cUIThing__cUIThing");

/**
 * Destroys a UI menu: the cUIGroup destructor's work, with cUIGroup's vtable stored.
 *
 * cUIMenu has nothing of its own to destroy, so this is cUIGroup__cUIGroup inlined.
 *
 * @param flag  g++ destructor flags, passed to cUIThing's destructor (bit 0 frees the object)
 */
extern "C" void cUIMenu__cUIMenu(cUIMenu* self, int flag)
{
    self->mVtbl = cUIGroup_vtbl;
    self->mList.mVtbl = cList_vtbl;
    cList_destroyList_v(&self->mList);
    self->mList.mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)(&self->mList.mEnd));
    self->mList.mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)(&self->mList));
    cUIThing__cUIThing_2(((char*)self), flag);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a menu.
 *
 * @return always 0x13 (icons return 0x11, pairs 0x15)
 */
int cUIMenu_getType(void* self)
{
    return 19;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", tAttrMenu_tAttrMenu);
#ifdef SKIP_ASM
/**
 * Clears the low four bits of a cUIMenu's tAttrMenu byte (mCreateThread, mFlipInput, mScrollMenu, mDisableDirSound) and
 * returns it: the tAttrMenu constructor, run on cUIMenu + 0x90.
 *
 * @return the tAttrMenu
 */
extern "C" void* tAttrMenu_tAttrMenu(void* self)
{
    *(unsigned char*)self &= 0xf0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIMenu_isFlipInput);
#ifdef SKIP_ASM
/**
 * Returns the menu's flip-input flag (mAttrMenu bit 1, mFlipInput).
 *
 * @param uiMenu  the cUIMenu
 * @return 1 when input is flipped, else 0
 */
extern "C" int cUIMenu_isFlipInput(cUIMenu* uiMenu) {
    return ((int) (*(int*)&uiMenu->mAttrMenu) >> 1) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setFlipInput);
#ifdef SKIP_ASM
/**
 * Sets the menu's flip-input flag (mAttrMenu bit 1, mFlipInput) to the low bit of @p enable.
 *
 * @param enable  1 to flip the menu's input directions, 0 not to
 */
extern "C" void cUIMenu_setFlipInput(cUIMenu* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrMenu)->bit1 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_isDisableDirSound);
#ifdef SKIP_ASM
/**
 * Returns the menu's disable-direction-sound flag (mAttrMenu bit 3, mDisableDirSound): when set the menu stays silent
 * as the selection moves.
 *
 * @param uiMenu  the cUIMenu
 * @return 1 when the direction sound is off, else 0
 */
extern "C" int cUIMenu_isDisableDirSound(cUIMenu* uiMenu) {
    return ((int) (*(int*)&uiMenu->mAttrMenu) >> 3) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setDisableDirSound);
#ifdef SKIP_ASM
/**
 * Stores bit 0 of @p enable in mAttrMenu.mDisableDirSound (no sound when the selection moves).
 *
 * @param enable  nonzero (odd) to silence direction changes (On Tour bool a)
 */
extern "C" void cUIMenu_setDisableDirSound(cUIMenu* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrMenu)->bit3 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_isCreateThead);
#ifdef SKIP_ASM
/**
 * Returns the menu's create-thread flag (mAttrMenu bit 0, mCreateThread), which cUIMenu_onInputChangeItem passes on to
 * cUIMenu_setSelected.
 *
 * @param uiMenu  the cUIMenu
 * @return 1 when set, else 0
 */
extern "C" int cUIMenu_isCreateThead(cUIMenu* uiMenu) {
    return (*(int*)&uiMenu->mAttrMenu) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setCreateThread);
#ifdef SKIP_ASM
/**
 * Sets the menu's create-thread flag (mAttrMenu bit 0, mCreateThread) to @p createThread != 0.
 *
 * @param uiMenu  the cUIMenu
 * @param createThread  non-zero to set the flag
 */
extern "C" void cUIMenu_setCreateThread(cUIMenu* uiMenu, int createThread) {
    *(int*)&uiMenu->mAttrMenu = (int) (((*(int*)&uiMenu->mAttrMenu) & ~1) | (createThread != 0));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getScrollIndex);
#ifdef SKIP_ASM
/**
 * Returns the menu's current item index (mScrollItemCurrent).
 *
 * @param uiMenu  the cUIMenu
 * @return mScrollItemCurrent
 */
extern "C" unsigned char cUIMenu_getScrollIndex(cUIMenu* uiMenu) {
    return (uiMenu->mScrollItemCurrent);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getScrollOffset);
#ifdef SKIP_ASM
/**
 * Returns the menu's scroll offset (mScrollItemOffset), the first item shown.
 *
 * @param uiMenu  the cUIMenu
 * @return mScrollItemOffset
 */
extern "C" unsigned char cUIMenu_getScrollOffset(cUIMenu* uiMenu) {
    return (uiMenu->mScrollItemOffset);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getScrollNum);
#ifdef SKIP_ASM
/**
 * Returns the menu's item count (mScrollItemNum).
 *
 * @param uiMenu  the cUIMenu
 * @return mScrollItemNum
 */
extern "C" unsigned char cUIMenu_getScrollNum(cUIMenu* uiMenu) {
    return (uiMenu->mScrollItemNum);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setScrollIndex);
#ifdef SKIP_ASM
/**
 * Sets the menu's current item index (mScrollItemCurrent) to @p scrollItemCurrent, without touching the selection.
 *
 * @param uiMenu  the cUIMenu
 * @param scrollItemCurrent  the new index
 */
extern "C" void cUIMenu_setScrollIndex(cUIMenu* uiMenu, signed char scrollItemCurrent) {
    *(signed char*)&uiMenu->mScrollItemCurrent = scrollItemCurrent;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setScrollNum);
#ifdef SKIP_ASM
/**
 * Sets the menu's item count (mScrollItemNum) to @p scrollItemNum.
 *
 * @param uiMenu  the cUIMenu
 * @param scrollItemNum  the new count
 */
extern "C" void cUIMenu_setScrollNum(cUIMenu* uiMenu, signed char scrollItemNum) {
    *(signed char*)&uiMenu->mScrollItemNum = scrollItemNum;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_setScrollOffset);
#ifdef SKIP_ASM
/**
 * Sets the menu's scroll offset (mScrollItemOffset, the first item shown) to @p scrollItemOffset.
 *
 * @param uiMenu  the cUIMenu
 * @param scrollItemOffset  the new offset
 */
extern "C" void cUIMenu_setScrollOffset(cUIMenu* uiMenu, signed char scrollItemOffset) {
    *(signed char*)&uiMenu->mScrollItemOffset = scrollItemOffset;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getSelected);
#ifdef SKIP_ASM
/**
 * Returns the menu's selected item (mpSelected).
 *
 * @param uiMenu  the cUIMenu
 * @return mpSelected
 */
extern "C" int cUIMenu_getSelected(cUIMenu* uiMenu) {
    return ((int)uiMenu->mpSelected);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIMenu_getSelectedIndex);
#ifdef SKIP_ASM
/**
 * Returns the menu's selected item index (mScrollItemCurrent, the same byte cUIMenu_getScrollIndex returns).
 *
 * @param uiMenu  the cUIMenu
 * @return mScrollItemCurrent
 */
extern "C" unsigned char cUIMenu_getSelectedIndex(cUIMenu* uiMenu) {
    return (uiMenu->mScrollItemCurrent);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIPair_cUIPair1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" void* tAttrPair_tAttrPair(tAttrPair* p);
extern void* cUIPair_vtbl[];

/**
 * Constructs a cUIPair with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIPair_vtbl and constructs
 * mAttrPair (+0x74) with tAttrPair_tAttrPair.
 *
 * Unlike cUIPair_cUIPair it leaves mpEntry / mpWidget unset. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIPair_cUIPair1(cUIPair* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIPair_vtbl;
    tAttrPair_tAttrPair(&self->mAttrPair);
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIPair_cUIPair);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern "C" void* tAttrPair_tAttrPair(tAttrPair* self);
extern void* cUIPair_vtbl[];

/**
 * Constructs a UI pair: the cUIThing constructor (cUIThing_cUIThing; the screen and parent pass through in $a1 / $a2),
 * cUIPair_vtbl, its tAttrPair (tAttrPair_tAttrPair), no entry or widget, and sets mActive (bit 1 of mAttr).
 *
 * @return @p self
 */
extern "C" void* cUIPair_cUIPair(cUIPair* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUIPair_vtbl;
    tAttrPair_tAttrPair(&self->mAttrPair);
    self->mpEntry = 0;
    self->mpWidget = 0;
    ((sUIVectorFlags*)(&self->mAttr))->bit1 = 1;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIPair__cUIPair);
#ifdef SKIP_ASM
extern void* cUIPair_vtbl[];
extern "C" void cUIPair_destroyPair(void* self);
// PORT: the unit declares cUIThing__cUIThing with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* cUIThing__cUIThing_dtor(void* self, int flags) __asm__("cUIThing__cUIThing");

/**
 * Destroys a UI pair: deletes its two child objects, then runs the cUIThing destructor.
 *
 * Restores cUIPair_vtbl, calls cUIPair_destroyPair() and then cUIThing__cUIThing(self, flags).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIPair__cUIPair(cUIPair* self, int flags)
{
    self->mVtbl = cUIPair_vtbl;
    cUIPair_destroyPair(self);
    cUIThing__cUIThing_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIPair_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a pair.
 *
 * @return always 0x15 (menus return 0x13, vectors 0x18)
 */
int cUIPair_getType(void* self)
{
    return 21;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", tAttrPair_tAttrPair);
#ifdef SKIP_ASM
/**
 * Clears the two 6-bit pivot fields of a cUIPair's tAttrPair (mEntryPivot in byte 0, mWidgetPivot in byte 1) and
 * returns it: the tAttrPair constructor, run on cUIPair + 0x74.
 *
 * @return the tAttrPair
 */
extern "C" void* tAttrPair_tAttrPair(tAttrPair* self)
{
    *(unsigned char*)((char*)self + 0x1) &= 0xc0;
    *(unsigned char*)((char*)self + 0x0) &= 0xc0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIPair_getEntry);
#ifdef SKIP_ASM
/**
 * Returns the pair's entry (label) object (mpEntry).
 *
 * @param uiPair  the cUIPair
 * @return mpEntry
 */
extern "C" int cUIPair_getEntry(cUIPair* uiPair) {
    return ((int)uiPair->mpEntry);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIPair_setEntry);
#ifdef SKIP_ASM

/**
 * Replaces the pair's entry object: destroys the old one through its virtual destructor (slot 1, flags 3) and stores
 * @p obj.
 *
 * @param obj  the new entry (On Tour pWidget)
 */
extern "C" void cUIPair_setEntry(cUIPair* self, cUIThing* obj)
{
    cUIThing* old = self->mpEntry;
    if (((char*)old) != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)old->mVtbl;
        vt[1].fn(((char*)old) + vt[1].delta, 3);
    }
    self->mpEntry = (cUIThing*)obj;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIPair_getWidget);
#ifdef SKIP_ASM
/**
 * Returns the pair's widget (value) object (mpWidget).
 *
 * @param uiPair  the cUIPair
 * @return mpWidget
 */
extern "C" int cUIPair_getWidget(cUIPair* uiPair) {
    return ((int)uiPair->mpWidget);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIPair_setWidget);
#ifdef SKIP_ASM

/**
 * Replaces the pair's widget object: destroys the old one through its virtual destructor (slot 1, flags 3) and stores
 * @p obj.
 *
 * @param obj  the new widget (On Tour pWidget)
 */
extern "C" void cUIPair_setWidget(cUIPair* self, cUIThing* obj)
{
    cUIThing* old = self->mpWidget;
    if (((char*)old) != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)old->mVtbl;
        vt[1].fn(((char*)old) + vt[1].delta, 3);
    }
    self->mpWidget = (cUIThing*)obj;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_cUIListBox1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" unsigned char *tAttrListBox_tAttrListBox(unsigned char *arg0);
extern "C" void *tListBoxEntry_tListBoxEntry(void *arg0);
extern void* cUIListBox_vtbl[];

/**
 * Constructs a cUIListBox with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIListBox_vtbl,
 * constructs mAttrListBox (+0x74) with tAttrListBox_tAttrListBox and each of the 32 mpEntries (0x14 bytes from +0x98) with
 * tListBoxEntry_tListBoxEntry.
 *
 * Unlike cUIListBox_cUIListBox it leaves mNumEntries, mCurrentEntry and the colours unset. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIListBox_cUIListBox1(cUIListBox* self)
{
    int i;
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIListBox_vtbl;
    tAttrListBox_tAttrListBox((unsigned char*)(&self->mAttrListBox));
    char* entry = (char*)&self->mpEntries[0];
    for (i = 0x1F; i != -1; i--) {
        tListBoxEntry_tListBoxEntry(entry);
        entry += 0x14;
    }
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox__cUIListBox);
#ifdef SKIP_ASM
extern void* cUIListBox_vtbl[];
extern "C" void cUIListBox_deleteAllEntries(void* self);
// PORT: the unit declares cUIThing__cUIThing with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* cUIThing__cUIThing_dtor(void* self, int flags) __asm__("cUIThing__cUIThing");

/**
 * Destroys a list box: restores cUIListBox_vtbl, clears its entries (cUIListBox_deleteAllEntries), then runs the cUIThing
 * destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the cUIThing destructor)
 */
extern "C" void cUIListBox__cUIListBox(cUIListBox* self, int flags)
{
    self->mVtbl = cUIListBox_vtbl;
    cUIListBox_deleteAllEntries(self);
    cUIThing__cUIThing_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a list box object.
 *
 * Virtual slot 22, as for every cUIThing type (cUIIcon_getType returns 0x11).
 *
 * @return always 0x12 (18)
 */
int cUIListBox_getType(void* self)
{
    return 18;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setFontIndex);
#ifdef SKIP_ASM
/**
 * Sets the list box's font index (mAttrListBox bits 0-1, mFont) to the low two bits of @p font.
 *
 * @param uiListBox  the cUIListBox
 * @param font  the font index (0..3)
 */
extern "C" void cUIListBox_setFontIndex(cUIListBox* uiListBox, int font) {
    *(int*)&uiListBox->mAttrListBox = (int) (((*(int*)&uiListBox->mAttrListBox) & ~3) | (font & 3));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_hasShadow);
#ifdef SKIP_ASM
/**
 * Returns whether the list box draws its text with a shadow (mAttrListBox bit 2, mShadow).
 *
 * @param uiListBox  the cUIListBox
 * @return 1 when shadowed, else 0
 */
extern "C" int cUIListBox_hasShadow(cUIListBox* uiListBox) {
    return ((int) (*(int*)&uiListBox->mAttrListBox) >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setShadow);
#ifdef SKIP_ASM
/**
 * Sets the shadow flag (mShadow, bit 2 of mAttrListBox).
 *
 * @param enable  non-zero to draw a shadow
 */
extern "C" void cUIListBox_setShadow(cUIListBox* self, int enable) {
    ((sUIVectorFlags*)&self->mAttrListBox)->bit2 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_getUseStaticColour);
#ifdef SKIP_ASM
/**
 * Returns whether the list box uses its static colour (mAttrListBox bit 5, mStaticColour).
 *
 * @param uiListBox  the cUIListBox
 * @return 1 when set, else 0
 */
extern "C" int cUIListBox_getUseStaticColour(cUIListBox* uiListBox) {
    return ((int) (*(int*)&uiListBox->mAttrListBox) >> 5) & 1;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setUseStaticColour);
#ifdef SKIP_ASM
/**
 * Sets the static-colour flag (mStaticColour, bit 5 of mAttrListBox).
 *
 * @param enable  non-zero to draw in mStaticColour
 */
extern "C" void cUIListBox_setUseStaticColour(cUIListBox* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrListBox)->bit5 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIListBox_isWrapping);
#ifdef SKIP_ASM
/**
 * Returns whether the list box wraps around at its ends: the cUIThing wrap flag (mAttr bit 7).
 *
 * @param uiListBox  the cUIListBox
 * @return 1 when wrapping, else 0
 */
extern "C" int cUIListBox_isWrapping(cUIListBox* uiListBox) {
    int wrapBits = uiListBox->mAttr >> 7;
    return wrapBits & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setWrapping);
#ifdef SKIP_ASM
/**
 * Sets the list box's wrap flag (cUIThing mAttr bit 7) to the low bit of @p enable.
 *
 * @param enable  1 to wrap around at the ends, 0 not to
 */
extern "C" void cUIListBox_setWrapping(cUIListBox* self, int enable)
{
    ((sUIVectorFlags8*)&self->mAttr)->bit7 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setShadowColour);
#ifdef SKIP_ASM
/**
 * Copies @p in into mShadowColour.
 *
 * @param in  the shadow colour (On Tour pSColour)
 */
extern "C" void cUIListBox_setShadowColour(cUIListBox* self, tARGB* in)
{
    *(&self->mShadowColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_setStaticColour);
#ifdef SKIP_ASM
/**
 * Copies @p in into mStaticColour.
 *
 * @param in  the static colour (On Tour pSColour)
 */
extern "C" void cUIListBox_setStaticColour(cUIListBox* self, tARGB* in)
{
    *(&self->mStaticColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_getCurrentEntryIndex);
#ifdef SKIP_ASM
/**
 * Returns the index of the list box's current entry (mCurrentEntry).
 *
 * @param uiListBox  the cUIListBox
 * @return mCurrentEntry
 */
extern "C" unsigned char cUIListBox_getCurrentEntryIndex(cUIListBox* uiListBox) {
    return (uiListBox->mCurrentEntry);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIListBox_getNumEntries);
#ifdef SKIP_ASM
/**
 * Returns the number of entries in the list box (mNumEntries).
 *
 * @param uiListBox  the cUIListBox
 * @return mNumEntries
 */
extern "C" unsigned char cUIListBox_getNumEntries(cUIListBox* uiListBox) {
    return (uiListBox->mNumEntries);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_cUIState2);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* p);
extern "C" void* tAttrState_tAttrState(void* p);
extern void* cUIState_vtbl[];

/**
 * Constructs a bare cUIState: links the node to itself (mpPrev = mpNext = this), stores the cUIState vtable cUIState_vtbl,
 * constructs the tAttrState word mAttrState with tAttrState_tAttrState (visible, first time, mode 2) and the screen list
 * mScreens with cList_cList.
 *
 * Unlike cUIState_cUIState / cUIState_cUIState1 it sets no engine, parent, controller or user fields. Nothing in retail calls
 * it.
 *
 * @return the object
 */
extern "C" void* cUIState_cUIState2(cUIState* self)
{
    self->mpNext = (cListNode*)((char*)self);
    self->mpPrev = (cListNode*)((char*)self);
    self->mVtbl = cUIState_vtbl;
    tAttrState_tAttrState(&self->mAttrState);
    cList_cList(&self->mScreens);
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setHashName);
#ifdef SKIP_ASM
/**
 * Sets the state's hash name (mHashName, the state id) to @p hashName.
 *
 * @param uiState  the cUIState
 * @param hashName  the hashed name
 */
extern "C" void cUIState_setHashName(cUIState* uiState, int hashName) {
    (uiState->mHashName) = hashName;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getHashName);
#ifdef SKIP_ASM
/**
 * Returns the state's hash name (mHashName, the state id).
 *
 * @param uiState  the cUIState
 * @return mHashName
 */
extern "C" int cUIState_getHashName(cUIState* uiState) {
    return (uiState->mHashName);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getEngine);
#ifdef SKIP_ASM
/**
 * Returns the UI engine that owns the state (mpEngine).
 *
 * @param uiState  the cUIState
 * @return mpEngine
 */
extern "C" int cUIState_getEngine(cUIState* uiState) {
    return ((int)uiState->mpEngine);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setCurrentCtrl);
#ifdef SKIP_ASM
/**
 * Sets the state's current controller (mCurrentCtrl) to @p currentCtrl.
 *
 * @param uiState  the cUIState
 * @param currentCtrl  the controller index (-1 for none)
 */
extern "C" void cUIState_setCurrentCtrl(cUIState* uiState, signed char currentCtrl) {
    (uiState->mCurrentCtrl) = currentCtrl;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getCurrentCtrl);
#ifdef SKIP_ASM
/**
 * Returns the state's current controller (mCurrentCtrl, sign-extended; -1 for none).
 *
 * @param uiState  the cUIState
 * @return mCurrentCtrl
 */
extern "C" signed char cUIState_getCurrentCtrl(cUIState* uiState) {
    return (uiState->mCurrentCtrl);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setAllCtrlBits);
#ifdef SKIP_ASM
/**
 * Sets the state's whole controller bit mask (mCtrlBits) to @p ctrlBits.
 *
 * @param uiState  the cUIState
 * @param ctrlBits  one bit per controller that may drive the state
 */
extern "C" void cUIState_setAllCtrlBits(cUIState* uiState, signed char ctrlBits) {
    *(signed char*)&uiState->mCtrlBits = ctrlBits;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getCtrlBitMask);
#ifdef SKIP_ASM
/**
 * Returns the state's controller bit mask (mCtrlBits).
 *
 * @param uiState  the cUIState
 * @return mCtrlBits
 */
extern "C" unsigned char cUIState_getCtrlBitMask(cUIState* uiState) {
    return (uiState->mCtrlBits);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getMiscData);
#ifdef SKIP_ASM
/**
 * Returns the state's caller-defined data word (miMiscData).
 *
 * @param uiState  the cUIState
 * @return miMiscData
 */
extern "C" int cUIState_getMiscData(cUIState* uiState) {
    return (uiState->miMiscData);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setMiscData);
#ifdef SKIP_ASM
/**
 * Sets the state's caller-defined data word (miMiscData) to @p miscData.
 *
 * @param uiState  the cUIState
 * @param miscData  the value to store
 */
extern "C" void cUIState_setMiscData(cUIState* uiState, int miscData) {
    (uiState->miMiscData) = miscData;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getMode);
#ifdef SKIP_ASM
/**
 * Returns the state's mode (mAttrState bits 8-13, read from byte +0x1D; the constructors start it at 2).
 *
 * @param uiState  the cUIState
 * @return the mode (0..63)
 */
extern "C" int cUIState_getMode(cUIState* uiState) {
    return (*(unsigned char *)((char*)(uiState) + (0x1D))) & 0x3F;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setMode);
#ifdef SKIP_ASM
/**
 * Sets the state's mode (mAttrState bits 8-13) to the low six bits of @p mode.
 *
 * @param uiState  the cUIState
 * @param mode  the mode (0..63)
 */
extern "C" void cUIState_setMode(cUIState* uiState, int mode) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x3F00) | ((mode & 0x3F) << 8));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isFirstTime);
#ifdef SKIP_ASM
/**
 * Returns the state's first-time flag (mAttrState bit 4).
 *
 * @param uiState  the cUIState
 * @return 1 when set, else 0
 */
extern "C" int cUIState_isFirstTime(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 4) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setFirstTime);
#ifdef SKIP_ASM
/**
 * Sets the state's first-time flag (mAttrState bit 4) to the low bit of @p firstTime.
 *
 * @param uiState  the cUIState
 * @param firstTime  1 to set the flag, 0 to clear it
 */
extern "C" void cUIState_setFirstTime(cUIState* uiState, int firstTime) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x10) | ((firstTime & 1) * 0x10));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isEnabled);
#ifdef SKIP_ASM
/**
 * Returns the state's enabled flag (mAttrState bit 5).
 *
 * @param uiState  the cUIState
 * @return 1 when enabled, else 0
 */
extern "C" int cUIState_isEnabled(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 5) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setEnabled);
#ifdef SKIP_ASM
/**
 * Sets the state's enabled flag (mAttrState bit 5) to the low bit of @p enabled.
 *
 * @param uiState  the cUIState
 * @param enabled  1 to enable the state, 0 to disable it
 */
extern "C" void cUIState_setEnabled(cUIState* uiState, int enabled) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x20) | ((enabled & 1) << 5));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isPushCheck);
#ifdef SKIP_ASM
/**
 * Returns the state's push-check flag (mAttrState bit 14).
 *
 * @param uiState  the cUIState
 * @return 1 when set, else 0
 */
extern "C" int cUIState_isPushCheck(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 0xE) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setPushCheck);
#ifdef SKIP_ASM
/**
 * Sets the state's push-check flag (mAttrState bit 14) to the low bit of @p pushCheck.
 *
 * @param uiState  the cUIState
 * @param pushCheck  1 to set the flag, 0 to clear it
 */
extern "C" void cUIState_setPushCheck(cUIState* uiState, int pushCheck) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x4000) | ((pushCheck & 1) << 0xE));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isPopFloor);
#ifdef SKIP_ASM
/**
 * Returns the state's pop-floor flag (mAttrState bit 15).
 *
 * @param uiState  the cUIState
 * @return 1 when set, else 0
 */
extern "C" int cUIState_isPopFloor(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 0xF) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setPopFloor);
#ifdef SKIP_ASM
/**
 * Sets the state's pop-floor flag (mAttrState bit 15) to the low bit of @p popFloor.
 *
 * @param uiState  the cUIState
 * @param popFloor  1 to set the flag, 0 to clear it
 */
extern "C" void cUIState_setPopFloor(cUIState* uiState, int popFloor) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & 0xFFFF7FFF) | ((popFloor & 1) << 0xF));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isExplicit);
#ifdef SKIP_ASM
/**
 * Returns the state's explicit flag (mAttrState bit 6).
 *
 * @param uiState  the cUIState
 * @return 1 when set, else 0
 */
extern "C" int cUIState_isExplicit(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 6) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setExplicit);
#ifdef SKIP_ASM
/**
 * Sets the state's explicit flag (mAttrState bit 6) to the low bit of @p isExplicit.
 *
 * @param uiState  the cUIState
 * @param isExplicit  1 to set the flag, 0 to clear it
 */
extern "C" void cUIState_setExplicit(cUIState* uiState, int isExplicit) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x40) | ((isExplicit & 1) << 6));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isPendingDelete);
#ifdef SKIP_ASM
/**
 * Returns the state's pending-delete flag (mAttrState bit 7).
 *
 * @param uiState  the cUIState
 * @return 1 when set, else 0
 */
extern "C" int cUIState_isPendingDelete(cUIState* uiState) {
    int pendingBits = uiState->mAttrState >> 7;
    return pendingBits & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setPendingDelete);
#ifdef SKIP_ASM
/**
 * Sets the state's pending-delete flag (mAttrState bit 7) to the low bit of @p pendingDelete.
 *
 * @param uiState  the cUIState
 * @param pendingDelete  1 to set the flag, 0 to clear it
 */
extern "C" void cUIState_setPendingDelete(cUIState* uiState, int pendingDelete) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~0x80) | ((pendingDelete & 1) << 7));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_hasPopup);
#ifdef SKIP_ASM
/**
 * Returns whether the state has a popup open: whether its 2-bit popup count (mAttrState bits 1-2) is non-zero.
 *
 * @param uiState  the cUIState
 * @return 1 when the count is non-zero, else 0
 */
extern "C" int cUIState_hasPopup(cUIState* uiState) {
    return ((uiState->mAttrState) & 6) != 0;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_numPopup);
#ifdef SKIP_ASM
/**
 * Returns the state's popup count (mAttrState bits 1-2, 0..3).
 *
 * @param uiState  the cUIState
 * @return the popup count
 */
extern "C" int cUIState_numPopup(cUIState* uiState) {
    return ((unsigned int) ((unsigned int)uiState->mAttrState) >> 1) & 3;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setHasPopup);
#ifdef SKIP_ASM
/**
 * Sets the state's popup count (mAttrState bits 1-2) to the low two bits of @p popupCount.
 *
 * @param uiState  the cUIState
 * @param popupCount  the count (0..3)
 */
extern "C" void cUIState_setHasPopup(cUIState* uiState, int popupCount) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~6) | ((popupCount & 3) * 2));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_incrementPopup);
#ifdef SKIP_ASM
struct sUIVecMode1C {
    unsigned int bit0 : 1;
    unsigned int mode : 2;
};
/**
 * Increments the state's popup count (mHasPopup, the 2-bit field at bits 1-2 of mAttrState). It wraps from 3 to 0.
 */
extern "C" void cUIState_incrementPopup(cUIState* self)
{
    sUIVecMode1C* attr = (sUIVecMode1C*)(&self->mAttrState);
    attr->mode = attr->mode + 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_decrementPopup);
#ifdef SKIP_ASM
/**
 * Decrements the state's popup count (mHasPopup, bits 1-2 of mAttrState) when it is non-zero.
 */
extern "C" void cUIState_decrementPopup(cUIState* self)
{
    sUIVecMode1C* attr = (sUIVecMode1C*)(&self->mAttrState);
    if (attr->mode != 0)
        attr->mode = attr->mode - 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isPopup);
#ifdef SKIP_ASM
/**
 * Returns whether the state is a popup (mAttrState bit 0).
 *
 * @param uiState  the cUIState
 * @return 1 for a popup, else 0
 */
extern "C" int cUIState_isPopup(cUIState* uiState) {
    return (uiState->mAttrState) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setPopup);
#ifdef SKIP_ASM
/**
 * Sets the state's popup flag (mAttrState bit 0) to the low bit of @p isPopup.
 *
 * @param uiState  the cUIState
 * @param isPopup  1 to mark the state as a popup, 0 not to
 */
extern "C" void cUIState_setPopup(cUIState* uiState, int isPopup) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~1) | (isPopup & 1));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_isVisible);
#ifdef SKIP_ASM
/**
 * Returns the state's visible flag (mAttrState bit 3).
 *
 * @param uiState  the cUIState
 * @return 1 when visible, else 0
 */
extern "C" int cUIState_isVisible(cUIState* uiState) {
    return ((int) (uiState->mAttrState) >> 3) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setVisible);
#ifdef SKIP_ASM
/**
 * Sets the state's visible flag (mAttrState bit 3) to the low bit of @p visible.
 *
 * @param uiState  the cUIState
 * @param visible  1 to show the state, 0 to hide it
 */
extern "C" void cUIState_setVisible(cUIState* uiState, int visible) {
    (uiState->mAttrState) = (int) (((uiState->mAttrState) & ~8) | ((visible & 1) * 8));
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onCreateScreen__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base cUIState's create-screen handler (vtable slot 4), which states override to set up their
 * screens.
 */
void cUIState_onCreateScreen(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onGainTransition__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing for the base UI state's gain transition.
 *
 * @return always 0
 */
int cUIState_onGainTransition(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Default hook called before input is routed to a state's screens; does not consume it.
 *
 * @return 0 (input not handled)
 */
int cUIState_onInputBegin(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onLoseTransition__FPv);
#ifdef SKIP_ASM
/**
 * Default hook for a state's losing-focus transition; reports nothing to play.
 *
 * @return 0
 */
int cUIState_onLoseTransition(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for a state losing focus; does nothing.
 */
void cUIState_onLoseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onPreRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Default hook called before a state's screens are drawn; does nothing.
 */
void cUIState_onPreRender2D(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onScreenEvent__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for an event raised by one of a state's screens; does not handle it.
 *
 * @return 0 (event not handled)
 */
int cUIState_onScreenEvent(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_setParentState);
#ifdef SKIP_ASM
/**
 * Sets the state's parent state (mpParentState) to @p parentState.
 *
 * @param uiState  the cUIState
 * @param parentState  the new parent state
 */
extern "C" void cUIState_setParentState(void *uiState, int parentState) {
    (*(int *)((char*)(uiState) + (0x20))) = parentState;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getParentState);
#ifdef SKIP_ASM
/**
 * Returns the state's parent state (mpParentState).
 *
 * @param uiState  the cUIState
 * @return mpParentState
 */
extern "C" int cUIState_getParentState(cUIState* uiState) {
    return ((int)uiState->mpParentState);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getScreen);
#ifdef SKIP_ASM
/**
 * Returns the state's screen (mpScreen).
 *
 * @param uiState  the cUIState
 * @return mpScreen
 */
extern "C" int cUIState_getScreen(cUIState* uiState) {
    return ((int)uiState->mpScreen);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_setUserNum);
#ifdef SKIP_ASM
/**
 * Sets the user (player) number the state belongs to (mUserNum) to @p userNum.
 *
 * @param uiState  the cUIState
 * @param userNum  the user number (-1 for none)
 */
extern "C" void cUIState_setUserNum(cUIState* uiState, signed char userNum) {
    (uiState->mUserNum) = userNum;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIState_getUserNum);
#ifdef SKIP_ASM
/**
 * Returns the user (player) number the state belongs to (mUserNum, sign-extended; -1 for none).
 *
 * @param uiState  the cUIState
 * @return mUserNum
 */
extern "C" signed char cUIState_getUserNum(cUIState* uiState) {
    return (uiState->mUserNum);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onPopupEvent__FPv);
#ifdef SKIP_ASM
/**
 * Ignores popup events: the base UI state's onPopupEvent has an empty body.
 */
void cUIState_onPopupEvent(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI state's onWidgetCreate has an empty body.
 */
void cUIState_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onWidgetEvent__FPv);
#ifdef SKIP_ASM
/**
 * Ignores widget events: the base UI state's onWidgetEvent has an empty body.
 */
void cUIState_onWidgetEvent(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onWidgetMenu__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for a menu widget event; returns the default result.
 *
 * @return 0x101, the default result
 */
int cUIState_onWidgetMenu(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onWidgetPair__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for a pair widget event; returns the default result.
 *
 * @return 0x101, the default result
 */
int cUIState_onWidgetPair(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_onWidgetListBox__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for a list box widget event; returns the default result.
 *
 * @return 0x101, the default result
 */
int cUIState_onWidgetListBox(void* self)
{
    return 257;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_getData__FPv);
#ifdef SKIP_ASM
/**
 * Returns no data for the base UI state.
 *
 * @return always 0
 */
int cUIState_getData(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIState_handleEvent__FPv);
#ifdef SKIP_ASM
/**
 * Handles no event for the base UI state and reports it as not handled.
 *
 * @return always 0
 */
int cUIState_handleEvent(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIThread__cUIThread);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cListNode_destroyListNode(cListNode* self);

extern void* cListNode_vtbl[];

/**
 * Destroys a cUIThread: puts back cListNode_vtbl, unlinks the node from its list with cListNode_destroyListNode and
 * frees it with operator_delete when bit 0 of @p flags is set.
 *
 * It is slot 1 (the destructor) of D_004946A0, the vtable cUIScreen_playFrame gives each "UIThread" it creates.
 *
 * @param flags  bit 0 set to free the memory
 */
extern "C" void cUIThread__cUIThread(int* self, int flags)
{
    *(void***)((char*)self + 0x8) = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIScreen_cUIScreen1);
#ifdef SKIP_ASM
extern "C" void* tAttrScreen_tAttrScreen(void* self);
extern "C" void* cList_cList(void* p);
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern void* cUIScreen_vtbl[];

/**
 * Constructs a bare cUIScreen: links the node to itself, stores cUIScreen_vtbl, constructs mAttrScreen
 * (tAttrScreen_tAttrScreen), the thread list mThreads (cList_cList at +0x18) and the root group mRootGroup
 * (cUIThing_cUIThing on +0x40 with no screen or parent, cUIGroup_vtbl at +0x48, its child list at +0xB4).
 *
 * Unlike cUIScreen_cUIScreen it sets no state, buffer or record pointers. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIScreen_cUIScreen1(cUIScreen* self)
{
    self->mpNext = (cListNode*)((char*)self);
    self->mpPrev = (cListNode*)((char*)self);
    self->mVtbl = cUIScreen_vtbl;
    tAttrScreen_tAttrScreen(&self->mAttrScreen);
    cList_cList(&self->mThreads);
    cUIThing_cUIThing(&self->mRootGroup, 0, 0);
    self->mRootGroup.mVtbl = cUIGroup_vtbl;
    cList_cList(&self->mRootGroup.mList);
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_cUIScreen);
#ifdef SKIP_ASM
extern "C" void* tAttrScreen_tAttrScreen(void* self);
extern "C" void* cList_cList(void* p);
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern void* cUIScreen_vtbl[];

/**
 * Constructs a UI screen for a state (On Tour cUIScreen(cUIState* pState)).
 *
 * Self-links its list node, clears mpBuffer and mpScreenHeader, stores cUIScreen_vtbl, constructs its attributes at
 * +0x14 (tAttrScreen_tAttrScreen) and the thread list (+0x18), clears the keyframe and object blocks and the object list, and
 * builds the root group in place (cUIThing constructor at +0x40 with no screen or parent, cUIGroup_vtbl, its cList at
 * +0xB4). Then mpState = @p pState.
 *
 * @param pState  the owning cUIState (declared int)
 * @return @p self
 */
extern "C" void* cUIScreen_cUIScreen(cUIScreen* self, int pState)
{
    self->mpNext = (cListNode*)((char*)self);
    self->mpPrev = (cListNode*)((char*)self);
    self->mpBuffer = 0;
    self->mpScreenHeader = 0;
    self->mVtbl = cUIScreen_vtbl;
    tAttrScreen_tAttrScreen(&self->mAttrScreen);
    cList_cList(&self->mThreads);
    self->mpKeyframeBlock = 0;
    self->mpObjectBlock = 0;
    self->mppObjectList = 0;
    cUIThing_cUIThing(&self->mRootGroup, 0, 0);
    self->mRootGroup.mVtbl = cUIGroup_vtbl;
    cList_cList(&self->mRootGroup.mList);
    self->mpState = (cUIState*)pState;
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_getHashName);
#ifdef SKIP_ASM
/**
 * Returns the screen's hash name: the first word of its screen record (mpScreenHeader).
 *
 * @param uiScreen  the cUIScreen
 * @return the record's hash name
 */
extern "C" int cUIScreen_getHashName(cUIScreen* uiScreen) {
    return *((int*)uiScreen->mpScreenHeader);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrScreen_tAttrScreen1);
#ifdef SKIP_ASM
/**
 * Sets bit 0 (mFocused) and clears bit 1 (mBufferAlloc) of a cUIScreen's 16-bit tAttrScreen word and returns it: the
 * tAttrScreen constructor, a copy of tAttrScreen_tAttrScreen. Nothing in retail calls this copy.
 *
 * @param attr  the tAttrScreen (cUIScreen + 0x14)
 * @return @p attr
 */
extern "C" unsigned short *tAttrScreen_tAttrScreen1(unsigned short *attr) {
    *attr = (*attr | 1) & 0xFFFD;
    return attr;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_hasFocus);
#ifdef SKIP_ASM
/**
 * Returns whether the screen has focus (mAttrScreen bit 0).
 *
 * @param uiScreen  the cUIScreen
 * @return 1 when focused, else 0
 */
extern "C" int cUIScreen_hasFocus(cUIScreen* uiScreen) {
    return (uiScreen->mAttrScreen) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_setFocus);
#ifdef SKIP_ASM
/**
 * Sets the screen's focus flag (mAttrScreen bit 0) to @p focused != 0.
 *
 * @param uiScreen  the cUIScreen
 * @param focused  non-zero to give the screen focus
 */
extern "C" void cUIScreen_setFocus(cUIScreen* uiScreen, int focused) {
    (uiScreen->mAttrScreen) = (int) (((uiScreen->mAttrScreen) & ~1) | (focused != 0));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_rootGroupPushBack);
#ifdef SKIP_ASM
extern "C" void cList_addToEnd__FP5cListP9cListNode(int);

/**
 * Appends a UI object to the screen's root group: cList_addToEnd__FP5cListP9cListNode on mRootGroup's child list
 * (+0xB4), with the object the caller passes in the second argument register going through untouched.
 *
 * @param screen  the cUIScreen (as an address)
 */
extern "C" void cUIScreen_rootGroupPushBack(int screen) {
    cList_addToEnd__FP5cListP9cListNode(screen + 180);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_getRootGroup);
#ifdef SKIP_ASM
/**
 * Returns the address of the screen's root group (mRootGroup, +0x40).
 *
 * @param screen  the cUIScreen (as an address)
 * @return a pointer to mRootGroup
 */
extern "C" int cUIScreen_getRootGroup(int screen) {
    return screen + 0x40;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_setVisible);
#ifdef SKIP_ASM
/**
 * Shows or hides the screen through its root group's visible flag (bit 6 of +0x54).
 *
 * @param enable  non-zero to show (On Tour b)
 */
extern "C" void cUIScreen_setVisible(cUIScreen* self, int enable)
{
    ((sUIVectorFlags*)(&self->mRootGroup.mAttr))->bit6 = enable;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_isVisible);
#ifdef SKIP_ASM
/**
 * Returns whether the screen is visible: the visible bit (bit 6) of its root group's mAttr (+0x54).
 *
 * @param uiScreen  the cUIScreen
 * @return 1 when visible, else 0
 */
extern "C" int cUIScreen_isVisible(cUIScreen* uiScreen) {
    return ((int) (uiScreen->mRootGroup.mAttr) >> 6) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_getEngine);
#ifdef SKIP_ASM
/**
 * Returns the UI engine of the screen's state (mpState's mpEngine).
 *
 * @param uiScreen  the cUIScreen
 * @return the state's engine
 */
extern "C" int cUIScreen_getEngine(cUIScreen* uiScreen) {
    return ((int)uiScreen->mpState->mpEngine);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScreen_getParentState);
#ifdef SKIP_ASM
/**
 * Returns the state that owns the screen (mpState).
 *
 * @param uiScreen  the cUIScreen
 * @return mpState
 */
extern "C" int cUIScreen_getParentState(cUIScreen* uiScreen) {
    return ((int)uiScreen->mpState);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_cUIText1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" unsigned char* tAttrText_tAttrText(unsigned char* p);
extern void* cUIText_vtbl[];

/**
 * Constructs a cUIText with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIText_vtbl and constructs
 * the tAttrText word mAttrText (+0x74) with tAttrText_tAttrText.
 *
 * Unlike cUIText_cUIText it leaves the colours and the string members unset. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIText_cUIText1(cUIText* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIText_vtbl;
    tAttrText_tAttrText((unsigned char*)(&self->mAttrText));
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText__cUIText);
#ifdef SKIP_ASM
extern void* cUIText_vtbl[];
extern "C" void cUIText_deleteText(void* self);
// PORT: the unit declares cUIThing__cUIThing with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* cUIThing__cUIThing_dtor(void* self, int flags) __asm__("cUIThing__cUIThing");

/**
 * Destroys a UI text object: frees its string and line table, then runs the cUIThing destructor.
 *
 * Restores cUIText's vtable, calls cUIText_deleteText(), then cUIThing__cUIThing(), which frees the object when
 * asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIText__cUIText(cUIText* self, int flags)
{
    self->mVtbl = cUIText_vtbl;
    cUIText_deleteText(self);
    cUIThing__cUIThing_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a text object.
 *
 * Virtual slot 22, as for every cUIThing type (cUIIcon_getType returns 0x11).
 *
 * @return always 0x17 (23)
 */
int cUIText_getType(void* self)
{
    return 23;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setFontIndex);
#ifdef SKIP_ASM
/**
 * Sets the text's font index (mAttrText bits 0-1, mFont) to the low two bits of @p fontIndex.
 *
 * @param uiText  the cUIText
 * @param fontIndex  the font index (0..3)
 */
extern "C" void cUIText_setFontIndex(cUIText* uiText, int fontIndex) {
    (uiText->mAttrText) = (int) (((uiText->mAttrText) & ~3) | (fontIndex & 3));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_hasShadow);
#ifdef SKIP_ASM
/**
 * Returns whether the text is drawn with a shadow (mAttrText bit 2, mShadow).
 *
 * @param uiText  the cUIText
 * @return 1 when shadowed, else 0
 */
extern "C" int cUIText_hasShadow(cUIText* uiText) {
    return ((int) (uiText->mAttrText) >> 2) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_isUnicode);
#ifdef SKIP_ASM
/**
 * Returns whether the text holds a Unicode (UCS-2) string (mAttrText bit 3, mUnicode).
 *
 * @param uiText  the cUIText
 * @return 1 for Unicode, else 0
 */
extern "C" int cUIText_isUnicode(cUIText* uiText) {
    return ((int) (uiText->mAttrText) >> 3) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_getUseStaticColour);
#ifdef SKIP_ASM
/**
 * Returns whether the text uses its static colour (mAttrText bit 4), which cUIText_render2D applies when the thing is
 * static.
 *
 * @param uiText  the cUIText
 * @return 1 when set, else 0
 */
extern "C" int cUIText_getUseStaticColour(cUIText* uiText) {
    return ((int) (uiText->mAttrText) >> 4) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_getUseDisabledColour);
#ifdef SKIP_ASM
/**
 * Returns whether the text uses its disabled colour (mAttrText bit 5), which cUIText_render2D applies when the thing is
 * not enabled.
 *
 * @param uiText  the cUIText
 * @return 1 when set, else 0
 */
extern "C" int cUIText_getUseDisabledColour(cUIText* uiText) {
    return ((int) (uiText->mAttrText) >> 5) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_isEllipses);
#ifdef SKIP_ASM
/**
 * Returns the text's ellipses flag (mAttrText bit 6).
 *
 * @param uiText  the cUIText
 * @return 1 when set, else 0
 */
extern "C" int cUIText_isEllipses(cUIText* uiText) {
    return ((int) (uiText->mAttrText) >> 6) & 1;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setShadow);
#ifdef SKIP_ASM
/**
 * Sets or clears bit 2 of a text object's attributes (+0x74): whether the text draws a drop shadow.
 *
 * @param enable  nonzero sets the bit, 0 clears it
 */
extern "C" void cUIText_setShadow(cUIText* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrText)->bit2 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setUnicode);
#ifdef SKIP_ASM
/**
 * Sets or clears bit 3 of a text object's attributes (+0x74): whether the text holds a Unicode string.
 *
 * @param enable  nonzero sets the bit, 0 clears it
 */
extern "C" void cUIText_setUnicode(cUIText* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrText)->bit3 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setUseStaticColour);
#ifdef SKIP_ASM
/**
 * Sets or clears bit 4 of a text object's attributes (+0x74): whether the text uses its static colour while
 * the thing is static.
 *
 * @param enable  nonzero sets the bit, 0 clears it
 */
extern "C" void cUIText_setUseStaticColour(cUIText* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrText)->bit4 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setUseDisabledColour);
#ifdef SKIP_ASM
/**
 * Sets or clears bit 5 of a text object's attributes (+0x74): whether the text uses its disabled colour while
 * the thing is disabled.
 *
 * @param enable  nonzero sets the bit, 0 clears it
 */
extern "C" void cUIText_setUseDisabledColour(cUIText* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrText)->bit5 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setEllipses);
#ifdef SKIP_ASM
/**
 * Sets or clears bit 6 of a text object's attributes (+0x74): whether the text draws only its first line
 * followed by "...".
 *
 * @param enable  nonzero sets the bit, 0 clears it
 */
extern "C" void cUIText_setEllipses(cUIText* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrText)->bit6 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIText_setShadowColour);
#ifdef SKIP_ASM
/**
 * Copies @p in into mShadowColour.
 *
 * @param in  the shadow colour (On Tour pSColour)
 */
extern "C" void cUIText_setShadowColour(cUIText* self, tARGB* in)
{
    *(&self->mShadowColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_setStaticColour);
#ifdef SKIP_ASM
/**
 * Copies @p in into mStaticColour.
 *
 * @param in  the static colour (On Tour pSColour)
 */
extern "C" void cUIText_setStaticColour(cUIText* self, tARGB* in)
{
    *(&self->mStaticColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIText_getStringID);
#ifdef SKIP_ASM
/**
 * Returns the text's string-table id (mStringID; 0 when the text was set directly).
 *
 * @param uiText  the cUIText
 * @return mStringID
 */
extern "C" int cUIText_getStringID(cUIText* uiText) {
    return (uiText->mStringID);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_cUIScrollBar1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern void* cUIScrollBar_vtbl[];

/**
 * Constructs a cUIScrollBar with no screen or parent: runs cUIThing_cUIThing(this, 0, 0) and stores cUIScrollBar_vtbl;
 * no scroll-bar member is set. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIScrollBar_cUIScrollBar1(cUIScrollBar* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIScrollBar_vtbl;
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_cUIScrollBar);
#ifdef SKIP_ASM
extern void* cUIScrollBar_vtbl[];
// PORT: the other ctors in the unit call this base ctor with (self, 0, 0)
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");

/**
 * Constructs a UI scroll bar: the cUIThing constructor (cUIThing_cUIThing; the screen and parent pass through in $a1 /
 * $a2), cUIScrollBar_vtbl, no background, knob, arrows or hide-on-empty object, meHideState, mValue, mNumValues and
 * mPage 0, and empty up / down animation names.
 *
 * @return @p self
 */
extern "C" void* cUIScrollBar_cUIScrollBar(cUIScrollBar* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mpObjBackground = 0;
    self->mVtbl = cUIScrollBar_vtbl;
    self->mpObjKnob = 0;
    self->mpUpArrow = 0;
    self->mpDownArrow = 0;
    self->mpHideOnEmpty = 0;
    self->meHideState = 0;
    self->mValue = 0;
    self->mNumValues = 0;
    self->mPage = 0;
    self->mszUpAnimation[0] = 0;
    self->mszDownAnimation[0] = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a scroll bar object.
 *
 * Virtual slot 22, as for every cUIThing type (cUIIcon_getType returns 0x11).
 *
 * @return always 0x21 (33)
 */
int cUIScrollBar_getType(void* self)
{
    return 33;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setBackgroundObject);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar's background object (mpObjBackground) to @p background.
 *
 * @param scrollBar  the cUIScrollBar
 * @param background  the background UI object
 */
extern "C" void cUIScrollBar_setBackgroundObject(void *scrollBar, int background) {
    (*(int *)((char*)(scrollBar) + (0x74))) = background;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setKnobObject);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar's knob object (mpObjKnob) to @p knob.
 *
 * @param scrollBar  the cUIScrollBar
 * @param knob  the knob UI object
 */
extern "C" void cUIScrollBar_setKnobObject(void *scrollBar, int knob) {
    (*(int *)((char*)(scrollBar) + (0x78))) = knob;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setUpArrowObject);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar's up-arrow object (mpUpArrow) to @p upArrow.
 *
 * @param scrollBar  the cUIScrollBar
 * @param upArrow  the up-arrow UI object
 */
extern "C" void cUIScrollBar_setUpArrowObject(void *scrollBar, int upArrow) {
    (*(int *)((char*)(scrollBar) + (0x7C))) = upArrow;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setDownArrowObject);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar's down-arrow object (mpDownArrow) to @p downArrow.
 *
 * @param scrollBar  the cUIScrollBar
 * @param downArrow  the down-arrow UI object
 */
extern "C" void cUIScrollBar_setDownArrowObject(void *scrollBar, int downArrow) {
    (*(int *)((char*)(scrollBar) + (0x80))) = downArrow;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setHideOnEmpty);
#ifdef SKIP_ASM
/**
 * Sets the object the scroll bar hides when there is nothing to scroll (mpHideOnEmpty) to @p hideOnEmpty.
 *
 * @param scrollBar  the cUIScrollBar
 * @param hideOnEmpty  the UI object to hide
 */
extern "C" void cUIScrollBar_setHideOnEmpty(void *scrollBar, int hideOnEmpty) {
    (*(int *)((char*)(scrollBar) + (0x84))) = hideOnEmpty;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setHideState);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar's hide mode (meHideState) to @p hideState.
 *
 * @param uiScrollBar  the cUIScrollBar
 * @param hideState  the eScrollBarHide value
 */
extern "C" void cUIScrollBar_setHideState(cUIScrollBar* uiScrollBar, int hideState) {
    (uiScrollBar->meHideState) = hideState;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setUpAnimationName);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char*);
extern "C" char* strcpy(char*, const char*);

/**
 * Copies @p str into the scroll bar's up-animation name (mszUpAnimation, 25 bytes); a name of 25 or more characters is
 * not copied and the name is set to empty instead.
 *
 * @param str  the animation name
 */
extern "C" void cUIScrollBar_setUpAnimationName(cUIScrollBar* self, char* str)
{
    if (strlen(str) >= 25U) {
        *(self->mszUpAnimation) = 0;
        return;
    }
    strcpy(self->mszUpAnimation, str);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_setDownAnimationName);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char*);
extern "C" char* strcpy(char*, const char*);

/**
 * Copies @p str into the scroll bar's down-animation name (mszDownAnimation, 25 bytes); a name of 25 or more characters
 * is not copied and the name is set to empty instead.
 *
 * @param str  the animation name
 */
extern "C" void cUIScrollBar_setDownAnimationName(cUIScrollBar* self, char* str)
{
    if (strlen(str) >= 25U) {
        ((char*)self)[0xA5] = 0;
        return;
    }
    strcpy(self->mszDownAnimation, str);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIScrollBar_getValue);
#ifdef SKIP_ASM
/**
 * Returns the scroll bar's current position (mValue).
 *
 * @param uiScrollBar  the cUIScrollBar
 * @return mValue
 */
extern "C" int cUIScrollBar_getValue(cUIScrollBar* uiScrollBar) {
    return (uiScrollBar->mValue);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIProgress_cUIProgress1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern void* cUIProgress_vtbl[];

/**
 * Constructs a cUIProgress with no screen or parent: runs cUIThing_cUIThing(this, 0, 0) and stores cUIProgress_vtbl; no
 * progress-bar member is set. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIProgress_cUIProgress1(cUIProgress* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIProgress_vtbl;
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIProgress_cUIProgress);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern void* cUIProgress_vtbl[];

/**
 * Constructs a UI progress bar: the cUIThing constructor (cUIThing_cUIThing; the screen and parent pass through in $a1 /
 * $a2), cUIProgress_vtbl, no background or knob, mValue, mNumValues and mBackWidth 0.
 *
 * @return @p self
 */
extern "C" void* cUIProgress_cUIProgress(cUIProgress* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUIProgress_vtbl;
    self->mpObjBackground = 0;
    self->mpObjKnob = 0;
    self->mValue = 0;
    self->mNumValues = 0;
    self->mBackWidth = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIProgress_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a progress bar.
 *
 * @return always 0x19 (vectors return 0x18)
 */
int cUIProgress_getType(void* self)
{
    return 25;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIProgress_setBackgroundObject);
#ifdef SKIP_ASM
/**
 * Sets the progress bar's background object (mpObjBackground) to @p background.
 *
 * @param progress  the cUIProgress
 * @param background  the background UI object
 */
extern "C" void cUIProgress_setBackgroundObject(void *progress, int background) {
    (*(int *)((char*)(progress) + (0x74))) = background;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIProgress_setKnobObject);
#ifdef SKIP_ASM
/**
 * Sets the progress bar's knob object (mpObjKnob) to @p knob.
 *
 * @param progress  the cUIProgress
 * @param knob  the knob UI object
 */
extern "C" void cUIProgress_setKnobObject(void *progress, int knob) {
    (*(int *)((char*)(progress) + (0x78))) = knob;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIProgress_getValue);
#ifdef SKIP_ASM
/**
 * Returns the progress bar's value (mValue).
 *
 * @param uiProgress  the cUIProgress
 * @return mValue
 */
extern "C" int cUIProgress_getValue(cUIProgress* uiProgress) {
    return (uiProgress->mValue);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIProgress_setNumValues);
#ifdef SKIP_ASM
/**
 * Sets the progress bar's number of steps (mNumValues) to @p numValues.
 *
 * @param uiProgress  the cUIProgress
 * @param numValues  the step count
 */
extern "C" void cUIProgress_setNumValues(cUIProgress* uiProgress, int numValues) {
    (uiProgress->mNumValues) = numValues;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_cUISlider1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern void* cUISlider_vtbl[];

/**
 * Constructs a cUISlider with no screen or parent: runs cUIThing_cUIThing(this, 0, 0) and stores cUISlider_vtbl; no
 * slider member is set. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUISlider_cUISlider1(cUISlider* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUISlider_vtbl;
    return ((char*)self);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_cUISlider);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIThing_cUIThing_self(void* self) __asm__("cUIThing_cUIThing");
extern void* cUISlider_vtbl[];

/**
 * Constructs a UI slider: the cUIThing constructor (cUIThing_cUIThing; the screen and parent pass through in $a1 / $a2),
 * cUISlider_vtbl, mValue, mNumValues, mStartPixel and mBackWidth 0, no background or knob.
 *
 * @return @p self
 */
extern "C" void* cUISlider_cUISlider(cUISlider* self)
{
    cUIThing_cUIThing_self(((char*)self));
    self->mVtbl = cUISlider_vtbl;
    self->mValue = 0;
    self->mNumValues = 0;
    self->mStartPixel = 0;
    self->mBackWidth = 0;
    self->mpObjBackground = 0;
    self->mpObjKnob = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUISlider_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns 22, the UI object type of a slider (cUISlider vtable slot 21).
 *
 * @return 22
 */
int cUISlider_getType(void* self)
{
    return 22;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUISlider_getValue);
#ifdef SKIP_ASM
/**
 * Returns the slider's value (mValue).
 *
 * @param uiSlider  the cUISlider
 * @return mValue
 */
extern "C" int cUISlider_getValue(cUISlider* uiSlider) {
    return (uiSlider->mValue);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_setValue);
#ifdef SKIP_ASM
/**
 * Sets the slider's value (mValue) to @p value, with no range check.
 *
 * @param uiSlider  the cUISlider
 * @param value  the new value
 */
extern "C" void cUISlider_setValue(cUISlider* uiSlider, int value) {
    (uiSlider->mValue) = value;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_setNumValues);
#ifdef SKIP_ASM
/**
 * Sets the slider's number of steps (mNumValues) to @p numValues.
 *
 * @param uiSlider  the cUISlider
 * @param numValues  the step count
 */
extern "C" void cUISlider_setNumValues(cUISlider* uiSlider, int numValues) {
    (uiSlider->mNumValues) = numValues;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_setBackgroundObject);
#ifdef SKIP_ASM
/**
 * Sets the slider's background object (mpObjBackground) to @p background.
 *
 * @param slider  the cUISlider
 * @param background  the background UI object
 */
extern "C" void cUISlider_setBackgroundObject(void *slider, int background) {
    (*(int *)((char*)(slider) + (0x88))) = background;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUISlider_setKnobObject);
#ifdef SKIP_ASM
/**
 * Sets the slider's knob object (mpObjKnob) to @p knob.
 *
 * @param slider  the cUISlider
 * @param knob  the knob UI object
 */
extern "C" void cUISlider_setKnobObject(void *slider, int knob) {
    (*(int *)((char*)(slider) + (0x8C))) = knob;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_cUITextScroll1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" void* cList_cList(void* p);
extern "C" unsigned char* tAttrTextScroll_tAttrTextScroll(unsigned char* p);
extern void* cUITextScroll_vtbl[];

/**
 * Constructs a cUITextScroll with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUITextScroll_vtbl,
 * constructs the tAttrTextScroll byte mAttrTextScroll (+0x74) with tAttrTextScroll_tAttrTextScroll and the line list mList (+0x8C) with
 * cList_cList.
 *
 * Unlike cUITextScroll_cUITextScroll it sets no colours, input map or line state. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUITextScroll_cUITextScroll1(cUITextScroll* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUITextScroll_vtbl;
    tAttrTextScroll_tAttrTextScroll((unsigned char*)(&self->mAttrTextScroll));
    cList_cList(&self->mList);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll__cUITextScroll);
#ifdef SKIP_ASM
extern void* cUITextScroll_vtbl[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];
extern "C" void cListNode_destroyListNode(cListNode* self);
extern "C" void cUITextScroll_clearBuffer(void* self);
// PORT: the unit declares cList_destroyList as returning void*; its body (util/list) is void.
extern "C" void cList_destroyList_v(void* self) __asm__("cList_destroyList");
// PORT: the unit declares cUIThing__cUIThing with one parameter; the real body is a
// destructor taking (self, flags). Bound to the same symbol with an asm label.
extern "C" void* cUIThing__cUIThing_dtor(void* self, int flags) __asm__("cUIThing__cUIThing");

/**
 * Destroys a text scroll: frees its text buffer (cUITextScroll_clearBuffer), destroys mList with its end and start nodes, then
 * runs the cUIThing destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the cUIThing destructor)
 */
extern "C" void cUITextScroll__cUITextScroll(cUITextScroll* self, int flags)
{
    self->mVtbl = cUITextScroll_vtbl;
    cUITextScroll_clearBuffer(self);
    self->mList.mVtbl = cList_vtbl;
    cList_destroyList_v((char*)&self->mList);
    self->mList.mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)(&self->mList.mEnd));
    self->mList.mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode((cListNode*)&self->mList);
    cUIThing__cUIThing_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a scrolling text object.
 *
 * Virtual slot 22, as for every cUIThing type (cUIIcon_getType returns 0x11).
 *
 * @return always 0x20 (32)
 */
int cUITextScroll_getType(void* self)
{
    return 0x20;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setFontIndex);
#ifdef SKIP_ASM
/**
 * Sets the text scroll's font index (mAttrTextScroll bits 1-2, mFont) to the low two bits of @p font.
 *
 * @param uiTextScroll  the cUITextScroll
 * @param font  the font index (0..3)
 */
extern "C" void cUITextScroll_setFontIndex(cUITextScroll* uiTextScroll, int font) {
    *(int*)&uiTextScroll->mAttrTextScroll = (int) (((*(int*)&uiTextScroll->mAttrTextScroll) & ~6) | ((font & 3) * 2));
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_hasShadow);
#ifdef SKIP_ASM
/**
 * Returns whether the text scroll draws its text with a shadow (mAttrTextScroll bit 3, mShadow).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return 1 when shadowed, else 0
 */
extern "C" int cUITextScroll_hasShadow(cUITextScroll* uiTextScroll) {
    return ((int) (*(int*)&uiTextScroll->mAttrTextScroll) >> 3) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_isUnicode);
#ifdef SKIP_ASM
/**
 * Returns whether the text scroll's lines are Unicode (mAttrTextScroll bit 4, mUnicode).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return 1 for Unicode, else 0
 */
extern "C" int cUITextScroll_isUnicode(cUITextScroll* uiTextScroll) {
    return ((int) (*(int*)&uiTextScroll->mAttrTextScroll) >> 4) & 1;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setShadow);
#ifdef SKIP_ASM
/**
 * Sets the shadow flag (mShadow, bit 3 of mAttrTextScroll).
 *
 * @param enable  non-zero to draw a shadow
 */
extern "C" void cUITextScroll_setShadow(cUITextScroll* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrTextScroll)->bit3 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setUnicode);
#ifdef SKIP_ASM
/**
 * Sets the Unicode flag (mUnicode, bit 4 of mAttrTextScroll).
 *
 * @param enable  non-zero when the text is UTF-16
 */
extern "C" void cUITextScroll_setUnicode(cUITextScroll* self, int enable)
{
    ((sUIVectorFlags*)&self->mAttrTextScroll)->bit4 = enable;
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setShadowColour);
#ifdef SKIP_ASM
/**
 * Copies @p in into mShadowColour.
 *
 * @param in  the shadow colour (On Tour pSColour)
 */
extern "C" void cUITextScroll_setShadowColour(cUITextScroll* self, tARGB* in)
{
    *(&self->mShadowColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_first);
#ifdef SKIP_ASM
extern "C" void cList_first__FP5cList(int);

/**
 * Returns the text scroll's first line object: cList_first__FP5cList on its list mList (+0x8C), whose result is passed
 * back.
 *
 * @param textScroll  the cUITextScroll (as an address)
 */
extern "C" void cUITextScroll_first(int textScroll) {
    cList_first__FP5cList(textScroll + 140);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_last);
#ifdef SKIP_ASM
extern "C" void cList_last(int);

/**
 * Returns the text scroll's last line object: cList_last (the list's last node) on its list mList (+0x8C), whose
 * result is passed back.
 *
 * @param textScroll  the cUITextScroll (as an address)
 */
extern "C" void cUITextScroll_last(int textScroll) {
    cList_last(textScroll + 140);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_getByIndex);
#ifdef SKIP_ASM
extern "C" void cList_getByIndex(int);

/**
 * Returns the text scroll's line object at an index: cList_getByIndex on its list mList (+0x8C). The index the caller
 * passes in the second argument register goes through untouched, and the list's result is passed back.
 *
 * @param textScroll  the cUITextScroll (as an address)
 */
extern "C" void cUITextScroll_getByIndex(int textScroll) {
    cList_getByIndex(textScroll + 140);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_addToFront);
#ifdef SKIP_ASM
extern "C" void cList_addToFront__FP5cListP9cListNode(int);

/**
 * Links a node at the front of the text scroll's list: cList_addToFront__FP5cListP9cListNode on mList (+0x8C), with the
 * node the caller passes in the second argument register going through untouched.
 *
 * @param textScroll  the cUITextScroll (as an address)
 */
extern "C" void cUITextScroll_addToFront(int textScroll) {
    cList_addToFront__FP5cListP9cListNode(textScroll + 140);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_addToEnd);
#ifdef SKIP_ASM
extern "C" void cList_addToEnd__FP5cListP9cListNode(int);

/**
 * Links a node at the end of the text scroll's list: cList_addToEnd__FP5cListP9cListNode on mList (+0x8C), with the
 * node the caller passes in the second argument register going through untouched.
 *
 * @param textScroll  the cUITextScroll (as an address)
 */
extern "C" void cUITextScroll_addToEnd(int textScroll) {
    cList_addToEnd__FP5cListP9cListNode(textScroll + 140);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setChatWindowMode);
#ifdef SKIP_ASM
/**
 * Puts the text scroll into chat-window mode: sets mAttrTextScroll bit 0 (mMode). There is no way here to clear it.
 *
 * @param uiTextScroll  the cUITextScroll
 */
extern "C" void cUITextScroll_setChatWindowMode(cUITextScroll* uiTextScroll) {
    *(int*)&uiTextScroll->mAttrTextScroll = (int) ((*(int*)&uiTextScroll->mAttrTextScroll) | 1);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_isChatWindowMode);
#ifdef SKIP_ASM
/**
 * Returns whether the text scroll is in chat-window mode (mAttrTextScroll bit 0, mMode).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return 1 in chat-window mode, else 0
 */
extern "C" int cUITextScroll_isChatWindowMode(cUITextScroll* uiTextScroll) {
    return (*(int*)&uiTextScroll->mAttrTextScroll) & 1;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setSelectedColour);
#ifdef SKIP_ASM
/**
 * Copies the colour @p in into the text scroll's selected-line colour (mSelectedColour), which cUITextScroll_render2D
 * uses for the selected line.
 *
 * @param in  the colour (a, r, g, b)
 */
extern "C" void cUITextScroll_setSelectedColour(cUITextScroll* self, tARGB* in)
{
    *(&self->mSelectedColour) = *in;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setScrollBar);
#ifdef SKIP_ASM
/**
 * Sets the scroll bar the text scroll drives (mpScrollBar) to @p scrollBar.
 *
 * @param textScroll  the cUITextScroll
 * @param scrollBar  the cUIScrollBar
 */
extern "C" void cUITextScroll_setScrollBar(void *textScroll, int scrollBar) {
    (*(int *)((char*)(textScroll) + (0xB8))) = scrollBar;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setLineOffset);
#ifdef SKIP_ASM
/**
 * Sets the text scroll's first visible line (mLinesOffset) to @p linesOffset.
 *
 * @param uiTextScroll  the cUITextScroll
 * @param linesOffset  the line index
 */
extern "C" void cUITextScroll_setLineOffset(cUITextScroll* uiTextScroll, short linesOffset) {
    *(short*)&uiTextScroll->mLinesOffset = linesOffset;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_getLineOffset);
#ifdef SKIP_ASM
/**
 * Returns the text scroll's first visible line (mLinesOffset).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return mLinesOffset
 */
extern "C" unsigned short cUITextScroll_getLineOffset(cUITextScroll* uiTextScroll) {
    return (uiTextScroll->mLinesOffset);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_getLineSize);
#ifdef SKIP_ASM
/**
 * Returns the number of lines in the text scroll (mLineSize).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return mLineSize
 */
extern "C" unsigned short cUITextScroll_getLineSize(cUITextScroll* uiTextScroll) {
    return (uiTextScroll->mLineSize);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_setSelectedIndex);
#ifdef SKIP_ASM
/**
 * Sets the text scroll's selected line (mSelectedIndex) to @p selectedIndex.
 *
 * @param uiTextScroll  the cUITextScroll
 * @param selectedIndex  the line index
 */
extern "C" void cUITextScroll_setSelectedIndex(cUITextScroll* uiTextScroll, short selectedIndex) {
    *(short*)&uiTextScroll->mSelectedIndex = selectedIndex;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUITextScroll_getSelectedIndex);
#ifdef SKIP_ASM
/**
 * Returns the text scroll's selected line (mSelectedIndex).
 *
 * @param uiTextScroll  the cUITextScroll
 * @return mSelectedIndex
 */
extern "C" unsigned short cUITextScroll_getSelectedIndex(cUITextScroll* uiTextScroll) {
    return (uiTextScroll->mSelectedIndex);
}
#endif
//100%
INCLUDE_ASM("ui/uivector", cUIVector_cUIVector1);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a, int b);
extern "C" void* tAttrVector_tAttrVector(void* p);
extern void* cUIVector_vtbl[];

/**
 * Constructs a cUIVector with no screen or parent: runs cUIThing_cUIThing(this, 0, 0), stores cUIVector_vtbl and
 * constructs the tAttrVector word mAttrVector (+0x74) with tAttrVector_tAttrVector (no fill, no loop, no vertices).
 *
 * Unlike cUIVector_cUIVector it leaves the colour and vertex arrays unset. Nothing in retail calls it.
 *
 * @return the object
 */
extern "C" void* cUIVector_cUIVector1(cUIVector* self)
{
    cUIThing_cUIThing(((char*)self), 0, 0);
    self->mVtbl = cUIVector_vtbl;
    tAttrVector_tAttrVector(&self->mAttrVector);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uivector", cUIVector_getType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the UI type id of a vector.
 *
 * @return always 0x18 (groups return 0x10)
 */
int cUIVector_getType(void* self)
{
    return 0x18;
}
#endif

extern "C" void* func_003A3F48(int, int);

//100%
INCLUDE_ASM("ui/uivector", tAttrThing_tAttrThing);
#ifdef SKIP_ASM
struct sFlags_6548 {
    unsigned b0 : 1;
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    unsigned f8 : 5;
    unsigned f13 : 6;
};

/**
 * Constructs a cUIThing's tAttrThing word and returns it: sets bit 0 (orientation: vertical), bit 1 (active), bit 3
 * (enabled) and bit 6 (visible), clears bit 2 (cached), bit 4 (focused), bit 5 (static), bit 7 (wrap) and the 5-bit
 * draw layer (bits 8-12), and sets the 6-bit pivot (bits 13-18) to 9.
 *
 * cUIThing_cUIThing1 runs it on mAttr (+0x14).
 *
 * @param attrThing  the tAttrThing (cUIThing + 0x14)
 * @return @p attrThing
 */
extern "C" sFlags_6548* tAttrThing_tAttrThing(sFlags_6548* attrThing)
{
    attrThing->b0 = 1;
    attrThing->b7 = 0;
    attrThing->f8 = 0;
    attrThing->b5 = 0;
    attrThing->b4 = 0;
    attrThing->b2 = 0;
    attrThing->b1 = 1;
    attrThing->b3 = 1;
    attrThing->b6 = 1;
    attrThing->f13 = 9;
    return attrThing;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrListBox_tAttrListBox);
#ifdef SKIP_ASM
/**
 * Constructs a cUIListBox's tAttrListBox byte and returns it: sets mShadow (bit 2) and clears mFont (bits 0-1),
 * mUnicode (bit 3), mCallback (bit 4) and mStaticColour (bit 5); bits 6-7 are kept.
 *
 * cUIListBox_cUIListBox1 runs it on mAttrListBox (+0x74).
 *
 * @param attr  the tAttrListBox (cUIListBox + 0x74)
 * @return @p attr
 */
extern "C" unsigned char *tAttrListBox_tAttrListBox(unsigned char *attr) {
    *attr = (*attr | 4) & 0xC4;
    return attr;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tListBoxEntry_tListBoxEntry);
#ifdef SKIP_ASM
/**
 * Constructs one list-box entry (tListBoxEntry, 0x14 bytes) and returns it: zeroes mStringID, mpString, mpAsciiString,
 * mpData and the byte mUnicodeBufferSize.
 *
 * cUIListBox_cUIListBox1 runs it on each of a cUIListBox's 32 mpEntries.
 *
 * @param entry  the tListBoxEntry
 * @return @p entry
 */
extern "C" void *tListBoxEntry_tListBoxEntry(void *entry) {
    (*(int *)((char*)(entry) + (0))) = 0;
    (*(int *)((char*)(entry) + (4))) = 0;
    (*(int *)((char*)(entry) + (8))) = 0;
    (*(int *)((char*)(entry) + (0xC))) = 0;
    (*(signed char *)((char*)(entry) + (0x10))) = 0;
    return entry;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrState_tAttrState);
#ifdef SKIP_ASM
/**
 * Constructs a cUIState's tAttrState and returns it: stores 0x218 in the 16-bit word, i.e. visible (bit 3) and first
 * time (bit 4) set, mode (bits 8-13) 2 and every other flag and the popup count clear.
 *
 * cUIState_cUIState2 runs it on mAttrState (+0x1C).
 *
 * @return the object
 */
extern "C" void* tAttrState_tAttrState(void* self)
{
    *(short*)self = 536;
    return self;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrText_tAttrText);
#ifdef SKIP_ASM
/**
 * Constructs a cUIText's tAttrText byte and returns it: sets mShadow (bit 2) and clears mUnicode (bit 3), mStaticColour
 * (bit 4), mDisabledColour (bit 5) and mEllipses (bit 6); the font index (bits 0-1) and bit 7 are kept.
 *
 * cUIText_cUIText1 runs it on mAttrText (+0x74).
 *
 * @param attr  the tAttrText (cUIText + 0x74)
 * @return @p attr
 */
extern "C" unsigned char *tAttrText_tAttrText(unsigned char *attr) {
    *attr = (*attr | 4) & 0x87;
    return attr;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrTextScroll_tAttrTextScroll);
#ifdef SKIP_ASM
/**
 * Constructs a cUITextScroll's tAttrTextScroll byte and returns it: clears mMode (bit 0), mShadow (bit 3), mUnicode
 * (bit 4) and mStaticColour (bit 5); the font index (bits 1-2) and bits 6-7 are kept.
 *
 * cUITextScroll_cUITextScroll1 runs it on mAttrTextScroll (+0x74).
 *
 * @param attr  the tAttrTextScroll (cUITextScroll + 0x74)
 * @return @p attr
 */
extern "C" unsigned char *tAttrTextScroll_tAttrTextScroll(unsigned char *attr) {
    *attr &= 0xC6;
    return attr;
}
#endif
//100%
INCLUDE_ASM("ui/uivector", tAttrVector_tAttrVector);
#ifdef SKIP_ASM
struct sCount_6618 {
    unsigned int flag : 1;
    unsigned int pad1 : 1;
    unsigned int mode : 15;
    unsigned int count : 15;
};
/**
 * Constructs a cUIVector's tAttrVector word and returns it: clears mFill (bit 0), mLoop (bit 1) and the 15-bit vertex
 * count mNumVertices (bits 17-31); the triangle count (bits 2-16) is left as it was.
 *
 * cUIVector_cUIVector1 runs it on mAttrVector (+0x74).
 *
 * @param attrVector  the tAttrVector (cUIVector + 0x74)
 * @return the same address
 */
extern "C" void* tAttrVector_tAttrVector(void* attrVector)
{
    sCount_6618* self = (sCount_6618*)attrVector;
    self->flag = 0;
    self->pad1 = 0;
    self->count = 0;
    return self;
}
#endif

//99.38%
INCLUDE_ASM("ui/uivector", func_003A6648__FPv);
#ifdef SKIP_ASM
/**
 * Runs the UI module's static constructors: func_003A3F48(1, 0xFFFF) (g++'s static initialiser for this unit).
 *
 * @return whatever func_003A3F48 leaves in the return register (it returns nothing)
 */
void* func_003A6648(void* self)
{
    return func_003A3F48(1, 0xffff);
}
#endif

extern "C" void* func_003A3F48(int, int);

//99.38%
INCLUDE_ASM("ui/uivector", func_003A6668__FPv);
#ifdef SKIP_ASM
/**
 * Runs the UI module's static destructors: func_003A3F48(0, 0xFFFF) (g++'s static destructor for this unit).
 *
 * @return whatever func_003A3F48 leaves in the return register (it returns nothing)
 */
void* func_003A6668(void* self)
{
    return func_003A3F48(0, 0xffff);
}
#endif

