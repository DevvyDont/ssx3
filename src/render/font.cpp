#include "common.h"
#include "render/ps2gridmesh.h"
#include "render/particle.h"
#include "render/graphicsman.h"
#include "render/ps2bezierman.h"
#include "render/ps2lightman.h"
#include "render/playerlights.h"
#include "render/font.h"
#include "render/ps2graphicsman.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("render/font", cFont_linkFont);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
struct sFontTexHdr;
extern "C" void cFont_downloadTexture(cFont* self, sFontTexHdr* img);
extern char str_Glyphs[];

struct sGlyph_1788 {
    unsigned short code;    // 0x0
    unsigned char w;        // 0x2
    unsigned char h;        // 0x3
    unsigned short u;       // 0x4
    unsigned short v;       // 0x6
    unsigned char adv;      // 0x8
    unsigned char xoff;     // 0x9
    signed char yoff;       // 0xA
    char pad;
};

struct sPad_1788 {
    float x, y;
};


/**
 * Builds a font's glyph table from loaded font data and uploads its texture.
 *
 * Records the run of consecutive character codes from the first (LastLinearCode), uses glyph 0xAC as the
 * missing
 * glyph, gives 0xA0 (no-break space) the space's metrics, and tracks the tallest glyph.
 *
 * @param vself  the font
 * @param vdata  the font file's data (On Tour const char* data)
 * @param flags  memory class for the glyph table (On Tour memtype)
 */
extern "C" void cFont_linkFont(cFont* vself, void* vdata, int flags)
{
    cFont* self = vself;
    char* data = (char*)vdata;
    sGlyph_1788* src = (sGlyph_1788*)(data + *(int*)(data + 0x14));
    sPad_1788 offset;
    offset.x = (float)data[0x10];
    offset.y = (float)data[0x11];
    (*(sPad_1788*)&self->Offset[0]) = offset;
    self->NumGlyphs = *(short*)(data + 0xA);
    (*(sGlyph_1788**)&self->Glyphs) = (sGlyph_1788*)operator_new_tag(self->NumGlyphs * 12, str_Glyphs, flags, 0);
    int space = 0;
    self->MaxCharHeight = 0;
    self->LastLinearCode = self->FirstCode = src->code;
    for (int i = 0; i < self->NumGlyphs; i++) {
        ((sGlyph_1788*)self->Glyphs)[i].code = src[i].code;
        ((sGlyph_1788*)self->Glyphs)[i].w = src[i].w;
        ((sGlyph_1788*)self->Glyphs)[i].h = src[i].h;
        ((sGlyph_1788*)self->Glyphs)[i].u = src[i].u;
        ((sGlyph_1788*)self->Glyphs)[i].v = src[i].v;
        ((sGlyph_1788*)self->Glyphs)[i].adv = src[i].adv;
        ((sGlyph_1788*)self->Glyphs)[i].xoff = src[i].xoff;
        ((sGlyph_1788*)self->Glyphs)[i].yoff = src[i].yoff;
        if (i != 0 && ((sGlyph_1788*)self->Glyphs)[i].code == self->LastLinearCode + 1) {
            self->LastLinearCode = ((sGlyph_1788*)self->Glyphs)[i].code;
        }
        if (((sGlyph_1788*)self->Glyphs)[i].code == ' ') {
            space = i;
        }
        if (((sGlyph_1788*)self->Glyphs)[i].code == 172) {
            (*(sGlyph_1788**)&self->MissingGlyph) = &((sGlyph_1788*)self->Glyphs)[i];
        }
        if (((sGlyph_1788*)self->Glyphs)[i].code == 160) {
            ((sGlyph_1788*)self->Glyphs)[i].w = src[space].w;
            ((sGlyph_1788*)self->Glyphs)[i].h = src[space].h;
            ((sGlyph_1788*)self->Glyphs)[i].u = src[space].u;
            ((sGlyph_1788*)self->Glyphs)[i].v = src[space].v;
            ((sGlyph_1788*)self->Glyphs)[i].adv = src[space].adv;
            ((sGlyph_1788*)self->Glyphs)[i].xoff = src[space].xoff;
            ((sGlyph_1788*)self->Glyphs)[i].yoff = src[space].yoff;
        }
        if (self->MaxCharHeight < ((sGlyph_1788*)self->Glyphs)[i].h + ((sGlyph_1788*)self->Glyphs)[i].yoff) {
            self->MaxCharHeight = ((sGlyph_1788*)self->Glyphs)[i].h + ((sGlyph_1788*)self->Glyphs)[i].yoff;
        }
    }
    cFont_downloadTexture((cFont*)((char*)self), (sFontTexHdr*)(data + *(int*)(data + 0x1C)));
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_purge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
void cMemMan_free(void*);

class sFontGfx_19E8 {
public:
    char pad_0x0[0x10D8];
    virtual void v1(int);
    virtual void v2(int);
    virtual void v3(int);
    virtual void v4(int);
    virtual void v5(int);
    virtual void v6(int);
    virtual void v7(int);
    virtual void v8(int);
    virtual void v9(int);
    virtual void v10(int);
    virtual void v11(int);
    virtual void v12(int);
    virtual void v13(int);
    virtual void v14(int);
    virtual void v15(int);
    virtual void v16(int);
    virtual void v17(int);
    virtual void v18(int);
    virtual void v19(int);
    virtual void v20(int);
    virtual void v21(int);
    virtual void v22(int);
    virtual void v23(int);
    virtual void v24(int);
    virtual void v25(int);
    virtual void v26(int);
    virtual void v27(int);
    virtual void v28(int);
    virtual void v29(int);
    virtual void v30(int);
    virtual void v31(int);
    virtual void v32(int);
    virtual void v33(int);
    virtual void v34(int);
    virtual void v35(int);
    virtual void v36(int);
    virtual void v37(int);
    virtual void v38(int);
    virtual void v39(int);
    virtual void v40(int);
    virtual void v41(int);
    virtual void v42(int);
    virtual void v43(int);
    virtual void v44(int);
    virtual void v45(int);
    virtual void v46(int);
    virtual void v47(int);
    virtual void v48(int);
    virtual void v49(int);
    virtual void RemoveTexture(int);
};


struct sFont_19E8 {
    int f0;
    void* pix;          // 0x4
    char pad_0x8[0x58];
    int handle;         // 0x60
};

/**
 * Frees a font's glyph table and releases its texture.
 */
extern "C" void cFont_purge(cFont* self)
{
    if (self->Glyphs != 0) {
        cMemMan_free(self->Glyphs);
    }
    self->Glyphs = 0;
    self->NumGlyphs = 0;
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    if (self->TextureID >= 0) {
        ((sFontGfx_19E8*)ps2GraphicsMan)->RemoveTexture(self->TextureID);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_downloadTexture);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" void MEM_copy(void* dst, const void* src, int n);
extern char str_FontTemp[];
extern char str_FontPalTemp[];
void cMemMan_free(void*);

struct sFontVEntryDL {
    short delta;
    short index;
    int (*fn)(void*, void*, void*, int, int, int, int, int, void*, int, int, int);
};

struct sFontCtxDL {
    char pad_0x0[0x10D8];
    sFontVEntryDL* vtable;      // 0x10D8
};


struct sFontTexHdr {
    int f0;
    short w;        // 0x4
    short h;        // 0x6
    int f8;
    int flags;      // 0xC
    int offset;     // 0x10
};

struct sFontUV {
    float u, v;
};

/**
 * Uploads a font's 4-bit glyph sheet as a texture with a white palette whose alpha ramps over the 16 indices.
 *
 * The texture height is the next power of two above the sheet's height (double it when it already is one);
 * the
 * UV scale is set to match.
 *
 * @param img  the glyph sheet (On Tour const SHAPE* shape)
 */
extern "C" void cFont_downloadTexture(cFont* self, sFontTexHdr* img)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    int newHeight = 1;
    do {
        newHeight <<= 1;
    } while (newHeight <= img->h);
    sFontUV uv;
    uv.v = 1.0f / newHeight;
    uv.u = 1.0f / img->w;
    *(sFontUV*)&self->UVScale[0] = uv;
    unsigned char* pix = (unsigned char*)operator_new_tag((img->w * newHeight) >> 1, str_FontTemp, 0x100, 0);
    unsigned char* pal = (unsigned char*)operator_new_tag(0x40, str_FontPalTemp, 0x100, 0);
    MEM_fill(pix, 0, (img->w * newHeight) >> 1);
    MEM_fill(pal, 0, 0x40);
    char* src;
    if (img->flags & 0x1000)
        src = (char*)img + img->offset;
    else
        src = (char*)img + 0x10;
    MEM_copy(pix, src, (img->w * img->h) >> 1);
    int i;
    signed char* palEntry;
    for (i = 0, palEntry = (signed char*)pal; i < 16; i++) {
        palEntry[0] = -0x80;
        palEntry[1] = -0x80;
        palEntry[2] = -0x80;
        palEntry[3] = (i << 7) / 15;
        palEntry += 4;
    }
    sFontVEntryDL* vt = (sFontVEntryDL*)ps2GraphicsMan->mVtbl;
    self->TextureID = vt[47].fn((char*)ps2GraphicsMan + vt[47].delta, pix, ((char*)self) + 0x64, img->w, newHeight, 0, 11, 1, pal, 0, 1, -1);
    if (pix != 0)
        cMemMan_free(pix);
    if (pal != 0)
        cMemMan_free(pal);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_vtextf);
#ifdef SKIP_ASM
struct sMat00391C48 {
    float m[4][4];
} __attribute__((aligned(16)));

extern sMat00391C48 D_00500CA0;
extern "C" void vsprintf(sMat00391C48* m);
extern "C" void cFont_text(cFont* self, float x, float y, sMat00391C48* m);

/**
 * Formats text into a shared static buffer (D_00500CA0) and draws it at a position.
 *
 * The C declaration drops the format and va_list arguments (On Tour fmt, va); they reach the vsprintf
 * (vsprintf) in the untouched argument registers.
 *
 * @param x  screen x
 * @param y  screen y
 */
extern "C" void cFont_vtextf(cFont* self, float x, float y)
{
    vsprintf(&D_00500CA0);
    cFont_text(self, x, y, &D_00500CA0);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_text);
#ifdef SKIP_ASM

struct sFontVEntry1CB0 {
    short delta;
    short index;
    void (*fn)(void*, void*, sMat00391C48*, void*, float, float, float, float, float, float, float);
};

struct sFontCtx1CB0 {
    char pad_0x0[0xE84];
    sDrawState* top;           // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sFontVEntry1CB0* vtable;    // 0x10D8
};


struct sFontPos1CB0 {
    float x, y, z, w;
    sFontPos1CB0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline int fontGetTex(void* s)
{
    return *(int*)((char*)s + 0x60);
}

/**
 * Draws a string with the font at a screen position, with a drop shadow when the font has a shadow offset.
 *
 * Binds the font texture (fontGetTex) and offsets the position by Offset * Scale. With a nonzero ShadowOffset it
 * first draws the text at the shadow offset in ShadowColor (+0x50), with the draw state's 5-bit field at bits 5-9
 * lowered by one, then restores it; the text itself is drawn in Color (+0x40). Both draws go through GrMan slot 80
 * with Scale and UVScale. The unit's view types the string as a matrix (sMat00391C48*); callers pass a char string
 * (cFont_vtextf's buffer, the centred / right-aligned text helpers, cDebugFE).
 *
 * @param x  the screen x
 * @param y  the screen y
 * @param str  the text to draw (On Tour const char* str)
 */
extern "C" void cFont_text(cFont* self, float x, float y, sMat00391C48* str)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    (ps2GraphicsMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = fontGetTex(self);
    sFontPos1CB0 pos(x + self->Offset[0] * self->Scale[0],
                     y + self->Offset[1] * self->Scale[1], 0.0f, 0.0f);
    if (self->ShadowOffset[0] != 0.0f || self->ShadowOffset[1] != 0.0f) {
        sDrawState* top = ps2GraphicsMan->m_pCurrentDrawState;
        int saved = (unsigned)(((int)top->unk_00[2]) & 0x3E0) >> 5;
        int shadowLayer = saved - 1;
        if (shadowLayer < 0) shadowLayer = 0;
        top->unk_00[2] = (((int)top->unk_00[2]) & ~0x3E0) | ((shadowLayer << 5) & 0x3E0);
        sFontVEntry1CB0* vt = (sFontVEntry1CB0*)ps2GraphicsMan->mVtbl;
        vt[80].fn((char*)ps2GraphicsMan + vt[80].delta, self, str, ((char*)self) + 0x50,
                  pos.x + self->ShadowOffset[0], pos.y + self->ShadowOffset[1],
                  self->Scale[0], self->Scale[1], self->UVScale[0], self->UVScale[1], pos.z);
        sDrawState* top2 = ps2GraphicsMan->m_pCurrentDrawState;
        top2->unk_00[2] = (((int)top2->unk_00[2]) & ~0x3E0) | (saved << 5);
    }
    sFontVEntry1CB0* vt = (sFontVEntry1CB0*)ps2GraphicsMan->mVtbl;
    vt[80].fn((char*)ps2GraphicsMan + vt[80].delta, self, str, ((char*)self) + 0x40, pos.x, pos.y,
              self->Scale[0], self->Scale[1], self->UVScale[0], self->UVScale[1], pos.z);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_text1);
#ifdef SKIP_ASM

struct sFontVEntry1E30 {
    short delta;
    short index;
    void (*fn)(void*, void*, void*, void*, float, float, float, float, float, float, float);
};

struct sFontCtx1E30 {
    char pad_0x0[0xE84];
    sDrawState* top;           // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sFontVEntry1E30* vtable;    // 0x10D8
};


struct sFontPos1E30 {
    float x, y, z, w;
    sFontPos1E30(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

static inline int fontGetTex1E30(void* s)
{
    return *(int*)((char*)s + 0x60);
}

/**
 * Draws a 16-bit (Unicode) string with the font at a screen position, with a drop shadow when the font has a shadow
 * offset.
 *
 * The Unicode counterpart of cFont_text: binds the font's TextureID as texture 0 of the current draw state and offsets
 * the position by Offset * Scale. With a nonzero ShadowOffset it first draws the text at the shadow offset in
 * ShadowColor, with the draw state's 5-bit field at bits 5-9 of word 2 lowered by one (not below 0), then restores that
 * field; the text itself is drawn in Color. Both draws go through graphics manager virtual slot 81 (cPS2GraphicsMan_DrawString3D1) with
 * Scale and UVScale.
 *
 * @param x  the screen x
 * @param y  the screen y
 * @param unicodeStr  the 0-terminated 16-bit string
 */
extern "C" void cFont_text1(cFont* self, float x, float y, void* unicodeStr)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    (ps2GraphicsMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = fontGetTex1E30(self);
    sFontPos1E30 pos(x + self->Offset[0] * self->Scale[0],
                     y + self->Offset[1] * self->Scale[1], 0.0f, 0.0f);
    if (self->ShadowOffset[0] != 0.0f || self->ShadowOffset[1] != 0.0f) {
        sDrawState* top = ps2GraphicsMan->m_pCurrentDrawState;
        int saved = (unsigned)(((int)top->unk_00[2]) & 0x3E0) >> 5;
        int shadowLayer = saved - 1;
        if (shadowLayer < 0) shadowLayer = 0;
        top->unk_00[2] = (((int)top->unk_00[2]) & ~0x3E0) | ((shadowLayer << 5) & 0x3E0);
        sFontVEntry1E30* vt = (sFontVEntry1E30*)ps2GraphicsMan->mVtbl;
        vt[81].fn((char*)ps2GraphicsMan + vt[81].delta, self, unicodeStr, ((char*)self) + 0x50,
                  pos.x + self->ShadowOffset[0], pos.y + self->ShadowOffset[1],
                  self->Scale[0], self->Scale[1], self->UVScale[0], self->UVScale[1], pos.z);
        sDrawState* top2 = ps2GraphicsMan->m_pCurrentDrawState;
        top2->unk_00[2] = (((int)top2->unk_00[2]) & ~0x3E0) | (saved << 5);
    }
    sFontVEntry1E30* vt = (sFontVEntry1E30*)ps2GraphicsMan->mVtbl;
    vt[81].fn((char*)ps2GraphicsMan + vt[81].delta, self, unicodeStr, ((char*)self) + 0x40, pos.x, pos.y,
              self->Scale[0], self->Scale[1], self->UVScale[0], self->UVScale[1], pos.z);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_textExtent);
#ifdef SKIP_ASM


struct sRect1FB0 {
    float x0, y0, x1, y1;
};

static inline tGlyph* sFontRes1FB0_find(cFont* font, int c)
{
    if (c < font->FirstCode)
        return 0;
    if (c <= font->LastLinearCode)
        return (font->Glyphs) + c - font->FirstCode;
    int lo = font->LastLinearCode - font->FirstCode;
    int hi = font->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* glyph = (font->Glyphs) + mid;
        if (c == glyph->code)
            return glyph;
        if (c < glyph->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (font->MissingGlyph);
}

/**
 * Measures a string's bounding box in a font at the given scale, including the font's shadow offset.
 *
 * @param f_  the font
 * @param str  the string
 * @param r_  receives the bounding box (On Tour tRect* rect), or 0
 * @param mono  nonzero to measure each character by its advance rather than its glyph width (On Tour
 * addCharAdvanceOnEnd)
 * @param sx  horizontal scale
 * @param sy  vertical scale
 * @return the box's right edge
 */
// PORT: g++ `<?`/`>?` (min/max) operators, removed in GCC 4.3.
extern "C" float cFont_textExtent(cFont* f_, const char* str, void* r_, int mono, float sx, float sy)
{
    cFont* font = f_;
    sRect1FB0* rect = (sRect1FB0*)r_;
    sRect1FB0 localRect;
    sRect1FB0* rc = rect ? rect : &localRect;
    rc->x0 = 0.0f;
    rc->y0 = 0.0f;
    rc->x1 = 0.0f;
    rc->y1 = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    for (; *str; str++) {
        tGlyph* glyph = sFontRes1FB0_find(font, *(unsigned char*)str);
        if (glyph) {
            float adv = glyph->advance * sx;
            rc->x0 = (x + glyph->xoffset * sx) <? rc->x0;
            rc->y0 = (y + glyph->yoffset * sy) <? rc->y0;
            rc->y1 = (y + (glyph->yoffset + glyph->height) * sy) >? rc->y1;
            float nx;
            if (!mono) {
                rc->x1 = (x + (glyph->xoffset + glyph->width) * sx) >? rc->x1;
                nx = x + adv;
            } else {
                rc->x1 = (x + adv) >? rc->x1;
                nx = x + adv;
            }
            x = nx;
        }
    }
    rc->x1 = rc->x1 + rc->x0;
    rc->y1 = rc->y1 + rc->y0;
    if (font->ShadowOffset[0] < 0.0f) {
        rc->x0 = rc->x0 - font->ShadowOffset[0];
        rc->x1 = rc->x1 - font->ShadowOffset[0];
    } else {
        rc->x1 = rc->x1 + font->ShadowOffset[0];
    }
    if (font->ShadowOffset[1] < 0.0f) {
        rc->y0 = rc->y0 - font->ShadowOffset[1];
        rc->y1 = rc->y1 - font->ShadowOffset[1];
    } else {
        rc->y1 = rc->y1 + font->ShadowOffset[1];
    }
    return rc->x1;
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_textExtent1);
#ifdef SKIP_ASM


struct sRect21F0 {
    float x0, y0, x1, y1;
};

static inline tGlyph* sFontRes21F0_find(cFont* font, int c)
{
    if (c < font->FirstCode)
        return 0;
    if (c <= font->LastLinearCode)
        return (font->Glyphs) + c - font->FirstCode;
    int lo = font->LastLinearCode - font->FirstCode;
    int hi = font->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* glyph = (font->Glyphs) + mid;
        if (c == glyph->code)
            return glyph;
        if (c < glyph->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (font->MissingGlyph);
}

/**
 * Measures a 16-bit (Unicode) string: fills a bounding rectangle at the given scale and returns its right edge.
 *
 * The Unicode counterpart of cFont_textExtent. Looks up each character's glyph (linear range, binary search above
 * LastLinearCode, MissingGlyph when not found; codes below FirstCode are skipped) and grows the rectangle from the
 * glyph offsets and sizes times @p sx / @p sy; the right edge uses the glyph's width, or its advance when @p mono is
 * set. Then x0 is added to x1 and y0 to y1, and the ShadowOffset widens the rectangle on its side. Without @p r_ a
 * local rectangle is used.
 *
 * @param f_  the font (this)
 * @param unicodeText  the 0-terminated 16-bit string
 * @param r_  the rectangle (x0, y0, x1, y1) to fill, or null
 * @param mono  nonzero to measure each character by its advance instead of its glyph width
 * @param sx  the horizontal scale
 * @param sy  the vertical scale
 * @return the rectangle's x1 (the text width including the shadow)
 */
// PORT: g++ `<?`/`>?` (min/max) operators, removed in GCC 4.3.
extern "C" float cFont_textExtent1(cFont* f_, const unsigned short* unicodeText, void* r_, int mono, float sx, float sy)
{
    cFont* font = f_;
    sRect21F0* outRect = (sRect21F0*)r_;
    sRect21F0 localRect;
    sRect21F0* rc = outRect ? outRect : &localRect;
    rc->x0 = 0.0f;
    rc->y0 = 0.0f;
    rc->x1 = 0.0f;
    rc->y1 = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    for (; *unicodeText; unicodeText++) {
        tGlyph* glyph = sFontRes21F0_find(font, *unicodeText);
        if (glyph) {
            float adv = glyph->advance * sx;
            rc->x0 = (x + glyph->xoffset * sx) <? rc->x0;
            rc->y0 = (y + glyph->yoffset * sy) <? rc->y0;
            rc->y1 = (y + (glyph->yoffset + glyph->height) * sy) >? rc->y1;
            float nx;
            if (!mono) {
                rc->x1 = (x + (glyph->xoffset + glyph->width) * sx) >? rc->x1;
                nx = x + adv;
            } else {
                rc->x1 = (x + adv) >? rc->x1;
                nx = x + adv;
            }
            x = nx;
        }
    }
    rc->x1 = rc->x1 + rc->x0;
    rc->y1 = rc->y1 + rc->y0;
    if (font->ShadowOffset[0] < 0.0f) {
        rc->x0 = rc->x0 - font->ShadowOffset[0];
        rc->x1 = rc->x1 - font->ShadowOffset[0];
    } else {
        rc->x1 = rc->x1 + font->ShadowOffset[0];
    }
    if (font->ShadowOffset[1] < 0.0f) {
        rc->y0 = rc->y0 - font->ShadowOffset[1];
        rc->y1 = rc->y1 - font->ShadowOffset[1];
    } else {
        rc->y1 = rc->y1 + font->ShadowOffset[1];
    }
    return rc->x1;
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_textSmartLimit);
#ifdef SKIP_ASM


static inline tGlyph* sFontRes2430_find(cFont* font, int c)
{
    if (c < font->FirstCode)
        return 0;
    if (c <= font->LastLinearCode)
        return (font->Glyphs) + c - font->FirstCode;
    int lo = font->LastLinearCode - font->FirstCode;
    int hi = font->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* glyph = (font->Glyphs) + mid;
        if (c == glyph->code)
            return glyph;
        if (c < glyph->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (font->MissingGlyph);
}

// PORT: PS2-only abs.s asm helper (fabsf off-PS2).
static inline float fabs_2430(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Copies as much of a string as fits in a width, breaking at a word boundary, and returns where the rest starts.
 *
 * The width is reduced by |ShadowOffset x| (not below 0). Characters are copied into @p dst while their measured width
 * (glyph extents times @p sx, with the previous glyph's trailing advance added) stays below @p width. When a space is
 * reached with the text already too wide, @p dst is cut after the last whole word and the spaces after it are skipped.
 * When the width runs out inside the first word, @p dst is cut before the current character. Copying also stops at
 * @p max - 1 characters; if the text still fits at that point the function returns 0 as if the whole string had been
 * copied (the rest is dropped).
 *
 * @param f_  the font (this)
 * @param str  the 0-terminated source string
 * @param dst  the buffer for the part that fits (0-terminated)
 * @param max  the size of @p dst in characters
 * @param sx  the horizontal scale
 * @param width  the available width
 * @return the start of the remaining text, or 0 when everything copied fits
 */
extern "C" char* cFont_textSmartLimit(cFont* f_, const char* str, char* dst, int max, float sx, float width)
{
    cFont* font = f_;
    width -= fabs_2430(font->ShadowOffset[0]);
    if (width < 0.0f)
        width = 0.0f;
    const char* start = str;
    float w = 0.0f;
    float pend = w;
    int lastSpace = -1;
    int n = 0;
    int k = 0;
    while (*str) {
        int sp = *str == ' ';
        if (sp) {
            if (width <= w) {
                dst[lastSpace + 1] = 0;
                const char* rest = start + lastSpace;
                do
                    rest++;
                while (*rest == ' ');
                return (char*)rest;
            }
            if (k > 0 && start[k - 1] != ' ')
                lastSpace = k - 1;
        }
        tGlyph* glyph = sFontRes2430_find(font, *(unsigned char*)str);
        if (glyph) {
            float right = (glyph->xoffset + glyph->width) * sx;
            w += pend + right;
            pend = glyph->advance * sx - right;
        }
        if (width <= w) {
            if (lastSpace == -1) {
                dst[n] = 0;
                return (char*)str;
            }
        }
        dst[n] = *str;
        n++;
        k++;
        str++;
        if (n == max - 1)
            break;
    }
    if (width <= w) {
        dst[lastSpace + 1] = 0;
        const char* rest = start + lastSpace;
        do
            rest++;
        while (*rest == ' ');
        return (char*)rest;
    }
    dst[n] = 0;
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_textSmartLimit1);
#ifdef SKIP_ASM


static inline tGlyph* sFontRes2680_find(cFont* font, int c)
{
    if (c < font->FirstCode)
        return 0;
    if (c <= font->LastLinearCode)
        return (font->Glyphs) + c - font->FirstCode;
    int lo = font->LastLinearCode - font->FirstCode;
    int hi = font->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* glyph = (font->Glyphs) + mid;
        if (c == glyph->code)
            return glyph;
        if (c < glyph->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (font->MissingGlyph);
}

// PORT: PS2-only abs.s asm helper (fabsf off-PS2).
static inline float fabs_2680(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Copies as much of a 16-bit (Unicode) string as fits in a width, breaking at a word boundary, and returns where the
 * rest starts.
 *
 * The Unicode counterpart of cFont_textSmartLimit, with the same rules: the width is reduced by |ShadowOffset x|, the text is
 * cut after the last whole word (skipping the spaces that follow) or inside the first word when no word fits, and
 * copying stops at @p max - 1 characters, in which case a text that still fits returns 0 and the rest is dropped.
 *
 * @param f_  the font (this)
 * @param sx  the horizontal scale
 * @param width  the available width
 * @param str  the 0-terminated source string
 * @param dst  the buffer for the part that fits (0-terminated)
 * @param max  the size of @p dst in characters
 * @return the start of the remaining text, or 0 when everything copied fits
 */
extern "C" unsigned short* cFont_textSmartLimit1(cFont* f_, float sx, float width, const unsigned short* str, unsigned short* dst, int max)
{
    cFont* font = f_;
    width -= fabs_2680(font->ShadowOffset[0]);
    if (width < 0.0f)
        width = 0.0f;
    const unsigned short* start = str;
    float w = 0.0f;
    float pend = w;
    int lastSpace = -1;
    int n = 0;
    int k = 0;
    while (*str) {
        int sp = *str == ' ';
        if (sp) {
            if (width <= w) {
                dst[lastSpace + 1] = 0;
                const unsigned short* rest = start + lastSpace;
                do
                    rest++;
                while (*rest == ' ');
                return (unsigned short*)rest;
            }
            if (k > 0 && start[k - 1] != ' ')
                lastSpace = k - 1;
        }
        tGlyph* glyph = sFontRes2680_find(font, *str);
        if (glyph) {
            float right = (glyph->xoffset + glyph->width) * sx;
            w += pend + right;
            pend = glyph->advance * sx - right;
        }
        if (width <= w) {
            if (lastSpace == -1) {
                dst[n] = 0;
                return (unsigned short*)str;
            }
        }
        dst[n] = *str;
        n++;
        k++;
        str++;
        if (n == max - 1)
            break;
    }
    if (width <= w) {
        dst[lastSpace + 1] = 0;
        const unsigned short* rest = start + lastSpace;
        do
            rest++;
        while (*rest == ' ');
        return (unsigned short*)rest;
    }
    dst[n] = 0;
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/font", cFont_centertext);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(cFont* self, const char* str, void* out, int n, float sx, float sy);

/**
 * Draws a string centred on x: measures it with cFont_textExtent at the font's Scale and draws it with cFont_text at x
 * minus half the width.
 *
 * @param x  the centre x
 * @param y  the screen y
 * @param str  the 0-terminated string
 */
extern "C" void cFont_centertext(cFont* self, float x, float y, const char* str)
{
    float w = cFont_textExtent(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text(self, x - w * 0.5f, y, (sMat00391C48*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_centertext1);
#ifdef SKIP_ASM
/**
 * Draws a 16-bit (Unicode) string centred on x: measures it with cFont_textExtent1 at the font's Scale and draws it with
 * cFont_text1 at x minus half the width.
 *
 * @param str  the 0-terminated 16-bit string
 * @param x  the centre x
 * @param y  the screen y
 */
extern "C" void cFont_centertext1(cFont* self, const unsigned short* str, float x, float y)
{
    float w = cFont_textExtent1(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text1(self, x - w * 0.5f, y, (void*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_vcentertextf);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void cBXString_vprintf(void* str, const char* fmt, void* ap);
extern "C" void cBXString__cBXString(void* self, int flags);

struct sBXStr_29F8 {
    char* p;
};

/**
 * Formats text and draws it centred on a position.
 *
 * @param fmt  printf-style format
 * @param ap  the format's arguments (On Tour va)
 * @param x  screen x of the centre
 * @param y  screen y
 */
extern "C" void cFont_vcentertextf(cFont* self, const char* fmt, void* ap, float x, float y)
{
    sBXStr_29F8 txt;
    txt.p = (char*)cBXString_EmptyString;
    cBXString_vprintf(&txt, fmt, ap);
    cFont_centertext(self, x, y, txt.p);
    cBXString__cBXString(&txt, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/font", cFont_righttext);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(cFont* self, const char* str, void* out, int n, float sx, float sy);

/**
 * Draws a string right-aligned at x: measures it with cFont_textExtent at the font's Scale and draws it with cFont_text
 * at x minus the width.
 *
 * @param str  the 0-terminated string
 * @param x  the right edge x
 * @param y  the screen y
 */
extern "C" void cFont_righttext(cFont* self, const char* str, float x, float y)
{
    float w = cFont_textExtent(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text(self, x - w, y, (sMat00391C48*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_righttext1);
#ifdef SKIP_ASM
/**
 * Draws a 16-bit (Unicode) string right-aligned at x: measures it with cFont_textExtent1 at the font's Scale and draws it
 * with cFont_text1 at x minus the width.
 *
 * @param str  the 0-terminated 16-bit string
 * @param x  the right edge x
 * @param y  the screen y
 */
extern "C" void cFont_righttext1(cFont* self, const unsigned short* str, float x, float y)
{
    float w = cFont_textExtent1(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text1(self, x - w, y, (void*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_vrighttextf);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void cBXString_vprintf(void* str, const char* fmt, void* ap);
extern "C" void cBXString__cBXString(void* self, int flags);

struct sBXStr_2B40 {
    char* p;
};

/**
 * Formats text and draws it right-aligned to a position.
 *
 * @param fmt  printf-style format
 * @param ap  the format's arguments (On Tour va)
 * @param x  screen x of the right edge
 * @param y  screen y
 */
extern "C" void cFont_vrighttextf(cFont* self, const char* fmt, void* ap, float x, float y)
{
    sBXStr_2B40 txt;
    txt.p = (char*)cBXString_EmptyString;
    cBXString_vprintf(&txt, fmt, ap);
    cFont_righttext(self, txt.p, x, y);
    cBXString__cBXString(&txt, 2);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_charAdvance);
#ifdef SKIP_ASM


static inline tGlyph* findGlyph(cFont* font, int c)
{
    if (c < font->FirstCode)
        return 0;
    if (c <= font->LastLinearCode)
        return (font->Glyphs) + c - font->FirstCode;
    int lo = font->LastLinearCode - font->FirstCode;
    int hi = font->NumGlyphs;
    do {
        int mid = (lo + hi) >> 1;
        tGlyph* glyph = &(font->Glyphs)[mid];
        if (c == glyph->code)
            return glyph;
        if (c < glyph->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return (font->MissingGlyph);
}

/**
 * Returns the unscaled advance of a character's glyph, or 0 when the font has no glyph for it.
 *
 * Finds the glyph (codes below FirstCode have none; up to LastLinearCode it is indexed directly, above that a binary
 * search by code, falling back to MissingGlyph).
 *
 * @param font  the font (this)
 * @param charCode  the character
 * @return the glyph's advance, or 0.0
 */
extern "C" float cFont_charAdvance(cFont* font, char charCode)
{
    tGlyph* glyph = findGlyph((font), charCode);
    if (glyph)
        return glyph->advance;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_charAdvance1);
#ifdef SKIP_ASM
/**
 * Returns the unscaled advance of a 16-bit (Unicode) character's glyph, or 0 when the font has no glyph for it.
 *
 * The same glyph lookup as cFont_charAdvance (direct index up to LastLinearCode, binary search above, MissingGlyph as the
 * fallback, nothing below FirstCode).
 *
 * @param font  the font (this)
 * @param charCode  the character code
 * @return the glyph's advance, or 0.0
 */
extern "C" float cFont_charAdvance1(cFont* font, unsigned short charCode)
{
    tGlyph* glyph = findGlyph((font), charCode);
    if (glyph)
        return glyph->advance;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("render/font", cPlayerLights_Clear);
#ifdef SKIP_ASM

struct sFontQuad {
    int v[4];
} __attribute__((aligned(16)));


/**
 * Clears the player lights: no directional lights, no extra ambient light, neither rider nor board additive.
 */
extern "C" void cPlayerLights_Clear(cPlayerLights* self)
{
    tRGB ambient;
    ambient.R = 0.0f;
    ambient.G = 0.0f;
    ambient.B = 0.0f;
    self->mNumDirectionalLights = 0;
    (*(&self->mExtraAmbientLight)) = ambient;
    self->mPlayerIsAdditive = 0;
    self->mBoardIsAdditive = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPlayerLights_AddAmbient);
#ifdef SKIP_ASM
/**
 * Adds a colour's R, G and B to mExtraAmbientLight.
 *
 * @param playerLights  the player lights (this)
 * @param colour  the colour to add (three floats R, G, B)
 */
extern "C" void cPlayerLights_AddAmbient(cPlayerLights* playerLights, tRGB* colour) {
    (playerLights->mExtraAmbientLight.R) = (float) ((playerLights->mExtraAmbientLight.R) + (colour->R));
    (playerLights->mExtraAmbientLight.G) = (float) ((playerLights->mExtraAmbientLight.G) + (colour->G));
    (playerLights->mExtraAmbientLight.B) = (float) ((playerLights->mExtraAmbientLight.B) + (colour->B));
}
#endif

//100%
INCLUDE_ASM("render/font", cPlayerLights_AddDirectional);
#ifdef SKIP_ASM
/**
 * Appends a directional light: stores @p colour and @p direction at index mNumDirectionalLights and increments it.
 *
 * There is no bound check against the size of mDirectionalLightColours / mDirectionalLightDirections.
 *
 * @param direction  the light's direction (a 16-byte vector)
 * @param colour  the light's colour
 */
extern "C" void cPlayerLights_AddDirectional(cPlayerLights* self, sFontQuad* direction, tRGB* colour)
{
    ((&self->mDirectionalLightColours[0]))[self->mNumDirectionalLights] = *colour;
    ((sFontQuad*)&self->mDirectionalLightDirections[0])[self->mNumDirectionalLights] = *direction;
    self->mNumDirectionalLights++;
}
#endif

//100%
INCLUDE_ASM("render/font", cPlayerLights_PlayerIsAdditive);
#ifdef SKIP_ASM
/**
 * Sets mPlayerIsAdditive to @p value.
 *
 * @param playerLights  the player lights (this)
 * @param value  the new flag
 */
extern "C" void cPlayerLights_PlayerIsAdditive(cPlayerLights* playerLights, int value) {
    (playerLights->mPlayerIsAdditive) = value;
}
#endif

//100%
INCLUDE_ASM("render/font", cPlayerLights_BoardIsAdditive__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the cPlayerLights flag mBoardIsAdditive (the word at +0xA4) to @p val.
 *
 * cTrickBoostFX_update sets it to 1 on the rider's player lights before adding a directional light.
 *
 * @param val  the new flag
 */
void cPlayerLights_BoardIsAdditive(void* self, int val)
{
    *(int*)((char*)self + 0xA4) = val;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392DF0);
#ifdef SKIP_ASM
extern "C" float fSin(float a);

/**
 * Fills a 640-entry float table with sines: entry i = fSin(i * 2 * pi / 512), so it covers one and a quarter turns.
 *
 * Called from the render unit's static initialiser (__sti__all_in_one_render_cpp).
 *
 * @return @p self (the table)
 */
extern "C" void* func_00392DF0(void* self)
{
    int i;
    for (i = 0; i < 640; i++) {
        ((float*)self)[i] = fSin((float)i * 0.01227184571325779f);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392E60);
#ifdef SKIP_ASM
/**
 * Opens a VIF unpack block in a packet builder: records the unpack settings and reserves one quadword for the STCYCL
 * and UNPACK codes that func_00392E98 writes when the block is closed.
 *
 * Stores the current write position as the block start (+0x8), @p cycleLength at +0x18, @p writeLength at +0x1C,
 * @p unpackMode at +0x20, @p vuAddr at +0x24 and whether @p vuAddr is nonzero at +0x28 (the UNPACK's FLG bit), sets the
 * open flag at +0x2C and advances the write position (+0x0) by 0x10.
 *
 * @param packet  the packet builder
 * @param writeLength  the STCYCL write length (WL)
 * @param cycleLength  the STCYCL cycle length (CL)
 * @param unpackMode  the UNPACK format (vn/vl bits, 0-15)
 * @param vuAddr  the VU memory address to unpack to
 */
extern "C" void func_00392E60(void *packet, int writeLength, int cycleLength, int unpackMode, int vuAddr) {
    int cur = *(int *)packet;
    *(int *)((char*)packet + 0x8) = cur;
    *(int *)((char*)packet + 0x2C) = 1;
    *(int *)((char*)packet + 0x18) = cycleLength;
    *(int *)((char*)packet + 0x24) = vuAddr;
    *(int *)((char*)packet + 0x1C) = writeLength;
    *(int *)((char*)packet + 0x20) = unpackMode;
    *(int *)((char*)packet + 0x28) = vuAddr != 0;
    *(int *)packet = cur + 0x10;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00392E98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sFontPkt_2E98 {
    char* cur;       // 0x00
    int pad04;
    char* start;     // 0x08
    int pad0C[3];
    int w;           // 0x18
    int h;           // 0x1C
    int mode;        // 0x20
    int flagY;       // 0x24
    int flagX;       // 0x28
    int done;        // 0x2C
};

static inline long packWord_2E98(int y, int n, int x, int mode, int mask)
{
    return (long)(y | ((n << 16) | (x ? mask : 0)) | ((mode | 0x60) << 24)) << 32;
}

/**
 * Closes the VIF unpack block opened by func_00392E60: writes its STCYCL and UNPACK codes at the block start.
 *
 * With a negative @p count the element count comes from the bytes written since the block start (minus the 16-byte
 * header) divided by the element size of the unpack mode (12, 6, 3, 16, 8, 4 or 2 bytes; mode 2 counts bytes), then
 * scaled by max(CL, WL) / min(CL, WL). The STCYCL word is CL | WL << 8 and the UNPACK word carries the VU address, the
 * count (low 8 bits), the FLG bit when the address was nonzero and the mode. Clears the open flag.
 *
 * @param count  the number of elements, or negative to compute it
 */
extern "C" void func_00392E98(sFontPkt_2E98* self, int count)
{
    int n;
    int requested = count;
    if (requested < 0) {
        n = self->cur - self->start - 16;
        switch (self->mode) {
        case 8:
            n /= 12;
            break;
        case 9:
            n /= 6;
            break;
        case 10:
            n /= 3;
            break;
        case 12:
            n /= 16;
            break;
        case 4: case 13:
            n /= 8;
            break;
        case 0: case 5: case 14:
            n /= 4;
            break;
        case 1: case 6: case 15:
            n /= 2;
            break;
        }
        // PORT: g++ `<?` / `>?` (min/max) operators, removed in GCC 4.3.
        n = n / (self->w <? self->h) * (self->w >? self->h);
    } else {
        n = requested;
    }
    n &= 0xFF;
    self->done = 0;
    *(long*)self->start = (long)(self->w | (self->h << 8) | 0x1000000) << 32;
    long* unpackWord = (long*)(self->start + 8);
    *unpackWord = packWord_2E98(self->flagY, n, self->flagX, self->mode, 0x8000);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00393008);
#ifdef SKIP_ASM
/**
 * Returns @p self unchanged, ignoring any further arguments (a variadic no-op).
 *
 * @return @p self
 */
extern "C" void* func_00393008(void* self, ...)
{
    return self;
}
#endif

INCLUDE_ASM("render/font", __sti__all_in_one_render_cpp);

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDTV__FPv);
#ifdef SKIP_ASM
/**
 * The base cGraphicsMan::SetDTV: does nothing (cPS2GraphicsMan_SetDTV is the PS2 override).
 *
 * Declared with only the this pointer; On Tour's SetDTV(bool Enable) takes the flag, which stays unused in $a1.
 */
void cGraphicsMan_SetDTV(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetDTV__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default: reports that no DTV (digital TV) output is in use.
 *
 * Returns 0. cPS2GraphicsMan replaces this slot (9) with its own cPS2GraphicsMan_GetDTV__FPv.
 *
 * @return 0 (false)
 */
int cGraphicsMan_GetDTV(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetScreenOffset);
#ifdef SKIP_ASM
/**
 * Sets the screen offset: m_screenOffset[0] / [1] = @p xoffset / @p yoffset converted to float.
 *
 * @param xoffset  the horizontal offset in pixels
 * @param yoffset  the vertical offset in pixels
 */
extern "C" void cGraphicsMan_SetScreenOffset(cGraphicsMan* self, int xoffset, int yoffset)
{
    self->m_screenOffset[0] = (float)xoffset;
    self->m_screenOffset[1] = (float)yoffset;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetScreenXOffset);
#ifdef SKIP_ASM
/**
 * Returns the horizontal screen offset, m_screenOffset[0] truncated to int.
 *
 * @return the X offset in pixels
 */
extern "C" int cGraphicsMan_GetScreenXOffset(cGraphicsMan* self)
{
    return (int)self->m_screenOffset[0];
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetScreenYOffset);
#ifdef SKIP_ASM
/**
 * Returns the vertical screen offset, m_screenOffset[1] truncated to int.
 *
 * @return the Y offset in pixels
 */
extern "C" int cGraphicsMan_GetScreenYOffset(cGraphicsMan* self)
{
    return (int)self->m_screenOffset[1];
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394008__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty stub). It fills slot 16 of the cGraphicsMan vtable (cGraphicsMan_vtbl), which cPS2GraphicsMan
 * (cPS2GraphicsMan_vtbl) inherits, between the clear-colour slot 15 (cPS2GraphicsMan_SetScreenClearColour in the PS2 vtable) and
 * IsRenderReady (slot 17).
 */
void func_00394008(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_IsRenderReady__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default: reports that the renderer is ready for a new frame.
 *
 * Returns 1. The module render functions call slot 17 and skip the frame while it returns 0; cPS2GraphicsMan overrides
 * it with cPS2GraphicsMan_IsRenderReady__FPv.
 *
 * @return 1 (true)
 */
int cGraphicsMan_IsRenderReady(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_IsRenderIdle__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default: reports that the renderer has no work in flight.
 *
 * Returns 1. cGraphicsMan_FlushRenderer() polls slot 18; cPS2GraphicsMan overrides it with cPS2GraphicsMan_IsRenderIdle__FPv.
 *
 * @return 1 (true)
 */
int cGraphicsMan_IsRenderIdle(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetNumViews);
#ifdef SKIP_ASM
/**
 * Sets mNumViews to @p numViews.
 *
 * @param graphicsMan  the graphics manager (this)
 * @param numViews  the number of views
 */
extern "C" void cGraphicsMan_SetNumViews(cGraphicsMan* graphicsMan, int numViews) {
    (graphicsMan->mNumViews) = numViews;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetNumViews);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's mNumViews (the number of views rendered; the constructor sets 1).
 *
 * @param graphicsMan  the graphics manager
 * @return mNumViews
 */
extern "C" int cGraphicsMan_GetNumViews(cGraphicsMan* graphicsMan) {
    return (graphicsMan->mNumViews);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetViewIndex);
#ifdef SKIP_ASM
/**
 * Sets mViewIndex to @p viewIndex.
 *
 * @param graphicsMan  the graphics manager (this)
 * @param viewIndex  the current view
 */
extern "C" void cGraphicsMan_SetViewIndex(cGraphicsMan* graphicsMan, int viewIndex) {
    (graphicsMan->mViewIndex) = viewIndex;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetViewIndex);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's mViewIndex (the view being worked on; the constructor sets 0).
 *
 * @param graphicsMan  the graphics manager
 * @return mViewIndex
 */
extern "C" int cGraphicsMan_GetViewIndex(cGraphicsMan* graphicsMan) {
    return (graphicsMan->mViewIndex);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setSpecularTextureId);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_SpecularTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param value  the new texture id
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 */
extern "C" void cGraphicsMan_setSpecularTextureId(cGraphicsMan* graphicsMan, int value, int idx) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0xC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getSpecularTextureId);
#ifdef SKIP_ASM
/**
 * Returns m_SpecularTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored texture id
 */
extern "C" int cGraphicsMan_getSpecularTextureId(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0xC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setPatchSpecularTextureId);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_PatchSpecularTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param value  the new texture id
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 */
extern "C" void cGraphicsMan_setPatchSpecularTextureId(cGraphicsMan* graphicsMan, int value, int idx) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0x2C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getPatchSpecularTextureId);
#ifdef SKIP_ASM
/**
 * Returns m_PatchSpecularTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored texture id
 */
extern "C" int cGraphicsMan_getPatchSpecularTextureId(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0x2C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setSnowSparkleTextureId);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_SnowSparkleTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param value  the new texture id
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 */
extern "C" void cGraphicsMan_setSnowSparkleTextureId(cGraphicsMan* graphicsMan, int value, int idx) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0x4C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getSnowSparkleTextureId);
#ifdef SKIP_ASM
/**
 * Returns m_SnowSparkleTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored texture id
 */
extern "C" int cGraphicsMan_getSnowSparkleTextureId(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0x4C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setRimTextureId);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_RimTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param value  the new texture id
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 */
extern "C" void cGraphicsMan_setRimTextureId(cGraphicsMan* graphicsMan, int value, int idx) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0x6C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getRimTextureId);
#ifdef SKIP_ASM
/**
 * Returns m_RimTextureId[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored texture id
 */
extern "C" int cGraphicsMan_getRimTextureId(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0x6C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setisInNIS);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_isInNIS[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param value  the new flag
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 */
extern "C" void cGraphicsMan_setisInNIS(cGraphicsMan* graphicsMan, int value, int idx) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0x8C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getisInNIS);
#ifdef SKIP_ASM
/**
 * Returns m_isInNIS[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored flag
 */
extern "C" int cGraphicsMan_getisInNIS(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0x8C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setSnowSparkleDensity);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_SnowSparkleDensity[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new density
 */
extern "C" void cGraphicsMan_setSnowSparkleDensity(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0xAC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getSnowSparkleDensity);
#ifdef SKIP_ASM
/**
 * Returns m_SnowSparkleDensity[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored density
 */
extern "C" float cGraphicsMan_getSnowSparkleDensity(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0xAC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setDrawDistance);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_DrawDistance[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new distance
 */
extern "C" void cGraphicsMan_setDrawDistance(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0xCC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getDrawDistance);
#ifdef SKIP_ASM
/**
 * Returns m_DrawDistance[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored distance
 */
extern "C" float cGraphicsMan_getDrawDistance(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0xCC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogDensity);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogDensity[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new density
 */
extern "C" void cGraphicsMan_setFogDensity(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0xEC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogDensity);
#ifdef SKIP_ASM
/**
 * Returns m_FogDensity[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored density
 */
extern "C" float cGraphicsMan_getFogDensity(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0xEC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogColourR);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogColourR[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new red component
 */
extern "C" void cGraphicsMan_setFogColourR(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x10C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogColourR);
#ifdef SKIP_ASM
/**
 * Returns m_FogColourR[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored red component
 */
extern "C" float cGraphicsMan_getFogColourR(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0x10C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogColourG);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogColourG[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new green component
 */
extern "C" void cGraphicsMan_setFogColourG(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x12C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogColourG);
#ifdef SKIP_ASM
/**
 * Returns m_FogColourG[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored green component
 */
extern "C" float cGraphicsMan_getFogColourG(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0x12C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogColourB);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogColourB[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new blue component
 */
extern "C" void cGraphicsMan_setFogColourB(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x14C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogColourB);
#ifdef SKIP_ASM
/**
 * Returns m_FogColourB[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored blue component
 */
extern "C" float cGraphicsMan_getFogColourB(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0x14C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogNear);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogNear[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new distance
 */
extern "C" void cGraphicsMan_setFogNear(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x16C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogNear);
#ifdef SKIP_ASM
/**
 * Returns m_FogNear[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored distance
 */
extern "C" float cGraphicsMan_getFogNear(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0x16C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setFogFar);
#ifdef SKIP_ASM
/**
 * Stores @p value in m_FogFar[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @param value  the new distance
 */
extern "C" void cGraphicsMan_setFogFar(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x18C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getFogFar);
#ifdef SKIP_ASM
/**
 * Returns m_FogFar[@p idx].
 *
 * @param graphicsMan  the graphics manager (this)
 * @param idx  the render-state query id (0-7) that indexes the per-view array
 * @return the stored distance
 */
extern "C" float cGraphicsMan_getFogFar(cGraphicsMan* graphicsMan, int idx) {
    return *(float*)(((char*)graphicsMan) + (idx << 2) + 0x18C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintScaleR);
#ifdef SKIP_ASM
/**
 * Stores the red tint scale for one view: m_ScreenTintScaleR[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new red tint scale
 */
extern "C" void cGraphicsMan_setScreenTintScaleR(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x1AC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941F0);
#ifdef SKIP_ASM
/**
 * Returns the red tint scale for one view: m_ScreenTintScaleR[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the red tint scale
 */
extern "C" float func_003941F0(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1AC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintScaleG);
#ifdef SKIP_ASM
/**
 * Stores the green tint scale for one view: m_ScreenTintScaleG[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new green tint scale
 */
extern "C" void cGraphicsMan_setScreenTintScaleG(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x1CC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394210);
#ifdef SKIP_ASM
/**
 * Returns the green tint scale for one view: m_ScreenTintScaleG[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the green tint scale
 */
extern "C" float func_00394210(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1CC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintScaleB);
#ifdef SKIP_ASM
/**
 * Stores the blue tint scale for one view: m_ScreenTintScaleB[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new blue tint scale
 */
extern "C" void cGraphicsMan_setScreenTintScaleB(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x1EC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394230);
#ifdef SKIP_ASM
/**
 * Returns the blue tint scale for one view: m_ScreenTintScaleB[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the blue tint scale
 */
extern "C" float func_00394230(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1EC);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintOffsetR);
#ifdef SKIP_ASM
/**
 * Stores the red tint offset for one view: m_ScreenTintOffsetR[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new red tint offset
 */
extern "C" void cGraphicsMan_setScreenTintOffsetR(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x20C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394250);
#ifdef SKIP_ASM
/**
 * Returns the red tint offset for one view: m_ScreenTintOffsetR[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the red tint offset
 */
extern "C" float func_00394250(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x20C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintOffsetG);
#ifdef SKIP_ASM
/**
 * Stores the green tint offset for one view: m_ScreenTintOffsetG[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new green tint offset
 */
extern "C" void cGraphicsMan_setScreenTintOffsetG(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x22C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394270);
#ifdef SKIP_ASM
/**
 * Returns the green tint offset for one view: m_ScreenTintOffsetG[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the green tint offset
 */
extern "C" float func_00394270(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x22C);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setScreenTintOffsetB);
#ifdef SKIP_ASM
/**
 * Stores the blue tint offset for one view: m_ScreenTintOffsetB[@p idx] = @p value.
 *
 * One of the per-view render-state set/get pairs (the eight-entry arrays cVisualEffectsMan_commonUpdate fills each
 * frame). No range check on @p idx. No caller in C; the out-of-line copy of an inline accessor.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @param value  the new blue tint offset
 */
extern "C" void cGraphicsMan_setScreenTintOffsetB(cGraphicsMan* graphicsMan, int idx, float value) {
    *(float*)(((char*)graphicsMan) + (idx << 2) + 0x24C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394290);
#ifdef SKIP_ASM
/**
 * Returns the blue tint offset for one view: m_ScreenTintOffsetB[@p idx].
 *
 * The getter paired with the setter just before it. No range check on @p idx.
 *
 * @param base  the graphics manager
 * @param idx  the view / render-state query index (0-7)
 * @return the blue tint offset
 */
extern "C" float func_00394290(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x24C);
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetTexel);
#ifdef SKIP_ASM
/**
 * Base-class default for reading one texel: returns a fixed colour without looking at any texture.
 *
 * The C parameter called self is really the hidden pointer to the returned tARGB (the object pointer and the texture
 * id, u, v and filter arguments follow it in registers and are ignored). It stores 1.0 in words 0, 1 and 3 and 0 in
 * word 2 and returns the same pointer. cPS2GraphicsMan overrides the slot with cPS2GraphicsMan_GetTexel.
 *
 * @return the result pointer
 */
extern "C" void* cGraphicsMan_GetTexel(cGraphicsMan* self)
{
    *(float*)self = 1.0f;
    self->m_screenOffset[1] = 1.0f;
    self->m_AIUpdatesPerRender = 0;
    *(float*)&self->m_SpecularTextureId[0] = 1.0f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RemoveScreenGrabTexture__FPv);
#ifdef SKIP_ASM
/**
 * The base cGraphicsMan::RemoveScreenGrabTexture: does nothing (cPS2GraphicsMan_RemoveScreenGrabTexture is the PS2
 * version). Declared with only the this pointer.
 */
void cGraphicsMan_RemoveScreenGrabTexture(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_TakeScreenGrab__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for capturing the screen into a screen-grab texture: does nothing.
 *
 * cPS2GraphicsMan overrides slot 61 with cPS2GraphicsMan_TakeScreenGrab.
 */
void cGraphicsMan_TakeScreenGrab(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetActiveDrawState__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 62.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_SetActiveDrawState(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_ActiveDrawState__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the graphics manager's active draw state (&m_activeDrawState, +0xED8).
 *
 * cGraphicsMan vtable slot 64 (cPS2GraphicsMan keeps it). cGraphicsMan_DrawStateChanged calls it through the vtable to compare the
 * current draw state with this one.
 *
 * @return a pointer to m_activeDrawState
 */
void* cGraphicsMan_ActiveDrawState(void* self)
{
    return &((cGraphicsMan*)self)->m_activeDrawState;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetActiveTexture__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 65.
 *
 * cPS2GraphicsMan does not override the slot.
 */
void cGraphicsMan_SetActiveTexture(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDefaultDrawState);
#ifdef SKIP_ASM

extern sDrawState sDrawState_s_defaultDrawState;

/**
 * Resets the current draw state (*m_pCurrentDrawState) to the default draw state sDrawState_s_defaultDrawState built by the cGraphicsMan
 * constructor.
 */
extern "C" void cGraphicsMan_SetDefaultDrawState(cGraphicsMan* self)
{
    *(self->m_pCurrentDrawState) = sDrawState_s_defaultDrawState;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDrawState);
#ifdef SKIP_ASM

/**
 * Copies @p src into the current draw state (*m_pCurrentDrawState).
 *
 * @param src  the draw state to apply
 */
extern "C" void cGraphicsMan_SetDrawState(cGraphicsMan* self, sDrawState* src)
{
    *(self->m_pCurrentDrawState) = *src;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_CurrentDrawState);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's current draw state pointer (m_pCurrentDrawState).
 *
 * Identical to cGraphicsMan_CurrentDrawState1.
 *
 * @param graphicsMan  the graphics manager
 * @return m_pCurrentDrawState (as an int)
 */
extern "C" int cGraphicsMan_CurrentDrawState(cGraphicsMan* graphicsMan) {
    return ((int)graphicsMan->m_pCurrentDrawState);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_CurrentDrawState1);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's current draw state pointer (m_pCurrentDrawState).
 *
 * Identical to cGraphicsMan_CurrentDrawState.
 *
 * @param graphicsMan  the graphics manager
 * @return m_pCurrentDrawState (as an int)
 */
extern "C" int cGraphicsMan_CurrentDrawState1(cGraphicsMan* graphicsMan) {
    return ((int)graphicsMan->m_pCurrentDrawState);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDepthMask);
#ifdef SKIP_ASM
/**
 * Sets the depth-mask bit (bit 22 of word 1) of the current draw state (*m_pCurrentDrawState).
 *
 * Only bit 0 of @p enable is used.
 *
 * @param graphicsMan  the graphics manager
 * @param enable  the depth mask mode (On Tour eGMDepthMaskMode); bit 0 is stored
 */
extern "C" void cGraphicsMan_SetDepthMask(cGraphicsMan* graphicsMan, int enable) {
    int *drawState = (int*)graphicsMan->m_pCurrentDrawState;
    int bit = (enable << 22) & 0x400000;
    drawState[1] = (drawState[1] & ~0x400000) | bit;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDrawLayer);
#ifdef SKIP_ASM
/**
 * Sets the draw layer field (bits 5-9 of word 2) of the current draw state (*m_pCurrentDrawState).
 *
 * cGraphicsMan_GetDrawLayer reads the field back.
 *
 * @param graphicsMan  the graphics manager
 * @param value  the draw layer (On Tour eGMDrawLayer; 5 bits)
 */
extern "C" void cGraphicsMan_SetDrawLayer(cGraphicsMan* graphicsMan, int value) {
    void *temp_6;

    temp_6 = ((void*)graphicsMan->m_pCurrentDrawState);
    (*(int *)((char*)(temp_6) + (8))) = (int) (((*(int *)((char*)(temp_6) + (8))) & ~0x3E0) | ((value << 5) & 0x3E0));
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetConstantColour);
#ifdef SKIP_ASM
struct sFontV4 {
    float x, y, z, w;
};

/**
 * Stores @p colour in constant-colour slot @p idx (16-byte entries at +0xF04, m_constantColours). @p idx is not
 * range-checked.
 *
 * @param colour  the colour, passed by value as a 16-byte vector
 * @param idx  the constant-colour slot
 */
extern "C" void cGraphicsMan_SetConstantColour(cGraphicsMan* self, sFontV4 colour, int idx)
{
    // PORT: pointer arithmetic via int (index-first addu)
    *(sFontV4*)((char*)((idx << 4) + (int)self) + 0xF04) = colour;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394418);
#ifdef SKIP_ASM
/**
 * Returns constant colour @p idx of the graphics manager (m_constantColours[idx], 16 bytes) by value.
 *
 * The result goes through the hidden return buffer. The second argument, a 16-byte colour passed by value, is copied to
 * the stack and never used: the getter has the same parameter list as cGraphicsMan_SetConstantColour, which stores the
 * colour there. No range check on @p idx (two entries).
 *
 * @param v  a colour argument the body ignores
 * @param idx  the constant colour slot (On Tour eGMConstantColourId; 0 or 1)
 * @return the colour in m_constantColours[@p idx]
 */
extern "C" sFontV4 func_00394418(cPS2GraphicsMan* self, sFontV4 v, int idx)
{
    // PORT: pointer arithmetic via int (index-first addu)
    return *(sFontV4*)((char*)((idx << 4) + (int)self) + 0xF04);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetBlendMode);
#ifdef SKIP_ASM
/**
 * Sets the blend mode of one GS context in the current draw state (*m_pCurrentDrawState): 5 bits of word 1 at bit 2 + 5
 * * @p context.
 *
 * @param graphicsMan  the graphics manager
 * @param mode  the blend mode (On Tour eGMBlendMode)
 * @param context  the GS context (0 or 1)
 */
extern "C" void cGraphicsMan_SetBlendMode(cGraphicsMan* graphicsMan, int mode, int context) {
    int temp_2;
    int temp_4;
    void *temp_7;

    temp_7 = ((void*)graphicsMan->m_pCurrentDrawState);
    temp_2 = (context * 5) + 2;
    temp_4 = 0x1F << temp_2;
    (*(int *)((char*)(temp_7) + (4))) = (int) (((*(int *)((char*)(temp_7) + (4))) & ~temp_4) | ((mode << temp_2) & temp_4));
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetAlphaSort);
#ifdef SKIP_ASM
/**
 * Sets the alpha-sort mode and sort key of the current draw state (*m_pCurrentDrawState).
 *
 * The mode goes in bits 0-1 of word 1, the key in bits 10-28 of word 2 (19 bits); cGraphicsMan_GetAlphaSort and cGraphicsMan_GetAlphaSortKey read
 * them back.
 *
 * @param graphicsMan  the graphics manager
 * @param alphaSort  the alpha sort mode (On Tour eGMAlphaSort; 2 bits)
 * @param sortKey  the sort key (19 bits)
 */
extern "C" void cGraphicsMan_SetAlphaSort(cGraphicsMan* graphicsMan, int alphaSort, int sortKey) {
    void *temp_3;
    void *temp_8;

    temp_8 = ((void*)graphicsMan->m_pCurrentDrawState);
    (*(int *)((char*)(temp_8) + (4))) = (int) (((*(int *)((char*)(temp_8) + (4))) & ~3) | (alphaSort & 3));
    temp_3 = ((void*)graphicsMan->m_pCurrentDrawState);
    (*(int *)((char*)(temp_3) + (8))) = (int) (((*(int *)((char*)(temp_3) + (8))) & 0xE00003FF) | ((sortKey << 0xA) & 0x1FFFFC00));
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetTextureClamp);
#ifdef SKIP_ASM
/**
 * Sets the texture clamp mode of one GS context in the current draw state (*m_pCurrentDrawState): 2 bits of word 0 at
 * bit 2 + 2 * @p context.
 *
 * @param graphicsMan  the graphics manager
 * @param mode  the clamp mode (On Tour eGMClampMode)
 * @param context  the GS context (0 or 1)
 */
extern "C" void cGraphicsMan_SetTextureClamp(cGraphicsMan* graphicsMan, int mode, int context) {
    int *temp_7;
    int temp_4;
    int temp_6;

    temp_7 = ((int*)graphicsMan->m_pCurrentDrawState);
    temp_6 = (context * 2) + 2;
    temp_4 = 3 << temp_6;
    *temp_7 = (*temp_7 & ~temp_4) | ((mode << temp_6) & temp_4);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetDmaDataPtr);
#ifdef SKIP_ASM
/**
 * Stores the low 30 bits of @p value in word 3 of the current draw state (*m_pCurrentDrawState), keeping its top two
 * bits.
 *
 * @param graphicsMan  the graphics manager
 * @param value  the DMA data pointer / offset to record (30 bits)
 */
extern "C" void cGraphicsMan_SetDmaDataPtr(cGraphicsMan* graphicsMan, int value) {
    int *drawState = (int*)graphicsMan->m_pCurrentDrawState;
    int masked = value & 0x3FFFFFFF;
    drawState[3] = (drawState[3] & 0xC0000000) | masked;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetDrawLayer);
#ifdef SKIP_ASM
/**
 * Returns the draw layer field (bits 5-9 of word 2) of the current draw state (*m_pCurrentDrawState).
 *
 * cGraphicsMan_SetDrawLayer sets it.
 *
 * @param graphicsMan  the graphics manager
 * @return the draw layer (0-31)
 */
extern "C" unsigned int cGraphicsMan_GetDrawLayer(cGraphicsMan* graphicsMan) {
    return (unsigned int) ((*(int *)((char*)((graphicsMan->m_pCurrentDrawState)) + (8))) & 0x3E0) >> 5;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetBlendMode);
#ifdef SKIP_ASM
/**
 * Returns the context-0 blend mode (bits 2-6 of word 1) of the current draw state (*m_pCurrentDrawState).
 *
 * cGraphicsMan_SetBlendMode sets it.
 *
 * @param graphicsMan  the graphics manager
 * @return the blend mode (0-31)
 */
extern "C" unsigned int cGraphicsMan_GetBlendMode(cGraphicsMan* graphicsMan) {
    return (unsigned int) ((*(int *)((char*)((graphicsMan->m_pCurrentDrawState)) + (4))) & 0x7C) >> 2;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetAlphaSort);
#ifdef SKIP_ASM
/**
 * Returns the alpha-sort mode (bits 0-1 of word 1) of the current draw state (*m_pCurrentDrawState).
 *
 * cGraphicsMan_SetAlphaSort sets it.
 *
 * @param graphicsMan  the graphics manager
 * @return the alpha sort mode (0-3)
 */
extern "C" int cGraphicsMan_GetAlphaSort(cGraphicsMan* graphicsMan) {
    return (*(int *)((char*)((graphicsMan->m_pCurrentDrawState)) + (4))) & 3;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetAlphaSortKey);
#ifdef SKIP_ASM
/**
 * Returns the alpha-sort key (bits 10-28 of word 2) of the current draw state (*m_pCurrentDrawState).
 *
 * cGraphicsMan_SetAlphaSort sets it.
 *
 * @param graphicsMan  the graphics manager
 * @return the sort key
 */
extern "C" unsigned int cGraphicsMan_GetAlphaSortKey(cGraphicsMan* graphicsMan) {
    return (unsigned int) ((*(int *)((char*)((graphicsMan->m_pCurrentDrawState)) + (8))) & 0x1FFFFC00) >> 10;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_ClearProjTextures__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for forgetting the projected textures of the frame: does nothing.
 *
 * cPS2GraphicsMan overrides slot 66 with cPS2GraphicsMan_ClearProjTextures__FPv, which zeroes mNumProjTextures.
 */
void cGraphicsMan_ClearProjTextures(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_AddProjectiveTexture__FPv);
#ifdef SKIP_ASM
/**
 * The base cGraphicsMan::AddProjectiveTexture: does nothing (cPS2GraphicsMan_AddProjectiveTexture is the PS2
 * version). Declared with only the this pointer.
 */
void cGraphicsMan_AddProjectiveTexture(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RenderLineStrip__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 74.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_RenderLineStrip(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RenderBoardTrail__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 88.
 *
 * cPS2GraphicsMan does not override the slot (it overrides the next one, 89, with cPS2GraphicsMan_RenderMultipleBoardTrails).
 */
void cGraphicsMan_RenderBoardTrail(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RenderBoardTrail1__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 89.
 *
 * cPS2GraphicsMan overrides the slot with cPS2GraphicsMan_RenderMultipleBoardTrails.
 */
void cGraphicsMan_RenderBoardTrail1(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RenderBoardWake__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for drawing a board wake: does nothing.
 *
 * cPS2GraphicsMan overrides slot 90 with cPS2GraphicsMan_RenderBoardWake, which cBoardWakeFX_render() calls.
 */
void cGraphicsMan_RenderBoardWake(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetShinyBoarder__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for the shiny-boarder (chrome rider) flag: does nothing.
 *
 * cPS2GraphicsMan overrides slot 98 with cPS2GraphicsMan_SetShinyBoarder__FPvi, which stores the flag.
 */
void cGraphicsMan_SetShinyBoarder(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetNumTexRefs);
#ifdef SKIP_ASM
/**
 * Returns the number of texture references (mNumTexRefs).
 *
 * @param graphicsMan  the graphics manager
 * @return mNumTexRefs
 */
extern "C" int cGraphicsMan_GetNumTexRefs(cGraphicsMan* graphicsMan) {
    return (graphicsMan->mNumTexRefs);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetTexRef);
#ifdef SKIP_ASM
/**
 * Returns the address of texture reference @p idx (&mTexRefs[idx], 12 bytes each).
 *
 * No range check (the preview's version asserted the index).
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the texture reference index
 * @return a pointer to mTexRefs[@p idx]
 */
extern "C" char* cGraphicsMan_GetTexRef(cGraphicsMan* graphicsMan, int idx)
{
    return ((char*)graphicsMan) + 0x280 + idx * 0xC;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_DrawCharShadowModel__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for drawing a character's shadow model: does nothing.
 *
 * cPS2GraphicsMan overrides slot 100 with cPS2GraphicsMan_DrawCharShadowModel, which cAnimModel_rendershadow() calls.
 */
void cGraphicsMan_DrawCharShadowModel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SectionLoaded__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 102.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_SectionLoaded(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_InitGlobalBuffer__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 103.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_InitGlobalBuffer(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_PurgeGlobalBuffer__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 104.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_PurgeGlobalBuffer(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_InitGlobalRGBBuffer__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 105.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_InitGlobalRGBBuffer(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_PurgeGlobalRGBBuffer__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 106.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_PurgeGlobalRGBBuffer(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_PurgeMeshData__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 107.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_PurgeMeshData(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RemapModelMaterialId__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 108.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_RemapModelMaterialId(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_ResolveCharModel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the retail graphics manager's ResolveCharModel is an empty stub.
 *
 * On Tour's version takes the model header and data, part bone offsets, blended matrices and morph weights; this
 * declaration names none of them.
 */
void cGraphicsMan_ResolveCharModel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_InitCharModelPart__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for preparing one part of a character model: does nothing.
 *
 * cAnimModel_compile() calls slot 110; cPS2GraphicsMan's override cPS2GraphicsMan_InitCharModelPart__FPv is empty too.
 */
void cGraphicsMan_InitCharModelPart(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_GenerateBlendedMatrices__FPv);
#ifdef SKIP_ASM
/**
 * The base cGraphicsMan::GenerateBlendedMatrices: does nothing (cPS2GraphicsMan_GenerateBlendedMatrices is the PS2
 * version). Declared with only the this pointer.
 */
void cGraphicsMan_GenerateBlendedMatrices(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_PurgeCharModel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 112.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_PurgeCharModel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_PurgeCharModels__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 113.
 *
 * cPS2GraphicsMan does not override the slot, so the call is a no-op on PS2.
 */
void cGraphicsMan_PurgeCharModels(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_getCurrentFrameCount__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for the rendered-frame counter: always 0.
 *
 * cPS2GraphicsMan overrides slot 114 with cPS2GraphicsMan_getCurrentFrameCount__FPv, which returns its frame counter; the AI uses the value to
 * tell when a rider was last drawn.
 *
 * @return 0
 */
int cGraphicsMan_getCurrentFrameCount(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_UpdateFullScreenEffects__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 115.
 *
 * cPS2GraphicsMan overrides the slot with cPS2GraphicsMan_UpdateFullScreenEffects.
 */
void cGraphicsMan_UpdateFullScreenEffects(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_RenderFullScreenEffects__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of cGraphicsMan vtable slot 116.
 *
 * cPS2GraphicsMan overrides the slot with cPS2GraphicsMan_RenderFullScreenEffects.
 */
void cGraphicsMan_RenderFullScreenEffects(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_getGraphicsMan);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager singleton (cGraphicsMan_TheGraphicsMan).
 *
 * @return cGraphicsMan_TheGraphicsMan (as an int)
 */
extern "C" int cGraphicsMan_getGraphicsMan(void) {
    return (*(int*)&cGraphicsMan_TheGraphicsMan);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetSpecID);
#ifdef SKIP_ASM
/**
 * Sets the specular texture id (mSpecTexID).
 *
 * @param graphicsMan  the graphics manager
 * @param texId  the texture id
 */
extern "C" void cGraphicsMan_SetSpecID(cGraphicsMan* graphicsMan, int texId) {
    (graphicsMan->mSpecTexID) = texId;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetSpecID);
#ifdef SKIP_ASM
/**
 * Returns the specular texture id (mSpecTexID).
 *
 * @param graphicsMan  the graphics manager
 * @return mSpecTexID
 */
extern "C" int cGraphicsMan_GetSpecID(cGraphicsMan* graphicsMan) {
    return (graphicsMan->mSpecTexID);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946A8);
#ifdef SKIP_ASM
/**
 * Stores character specular texture id @p idx: mCharSpecTexID[idx] = @p value.
 *
 * No range check (ten entries). Retail-only; the preview has no such accessor.
 *
 * @param base  the graphics manager
 * @param idx  the character slot (0-9)
 * @param value  the texture id
 */
extern "C" void func_003946A8(cPS2GraphicsMan* base, int idx, int value) {
    *(int*)(((char*)base) + (idx << 2) + 0x10B0) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946B8);
#ifdef SKIP_ASM
/**
 * Returns character specular texture id @p idx (mCharSpecTexID[idx]).
 *
 * No range check (ten entries).
 *
 * @param base  the graphics manager
 * @param idx  the character slot (0-9)
 * @return the texture id
 */
extern "C" int func_003946B8(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x10B0);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetPatchSpecID);
#ifdef SKIP_ASM
/**
 * Sets the patch specular texture id (mPatchSpecTexID).
 *
 * @param graphicsMan  the graphics manager
 * @param texId  the texture id
 */
extern "C" void cGraphicsMan_SetPatchSpecID(cGraphicsMan* graphicsMan, int texId) {
    (graphicsMan->mPatchSpecTexID) = texId;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_GetPatchSpecID);
#ifdef SKIP_ASM
/**
 * Returns the patch specular texture id (mPatchSpecTexID).
 *
 * @param graphicsMan  the graphics manager
 * @return mPatchSpecTexID
 */
extern "C" int cGraphicsMan_GetPatchSpecID(cGraphicsMan* graphicsMan) {
    return (graphicsMan->mPatchSpecTexID);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SkipNextPageFlip);
#ifdef SKIP_ASM
/**
 * Requests that the next page flip be skipped: sets mSkipNextPageFlip to 1.
 *
 * @param graphicsMan  the graphics manager
 */
extern "C" void cGraphicsMan_SkipNextPageFlip(cGraphicsMan* graphicsMan) {
    (graphicsMan->mSkipNextPageFlip) = 1;
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_loadexecpurge__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for the shutdown done before loading another executable: does nothing.
 *
 * cSSXApp_loadexecpurge__FPv() calls slot 119; cPS2GraphicsMan overrides it with cPS2GraphicsMan_loadexecpurge, which removes its
 * interrupt handlers.
 */
void cGraphicsMan_loadexecpurge(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetPixelTest);
#ifdef SKIP_ASM
/**
 * Sets the current draw state's pixel tests in its word at +4: depth test bits 23-24, alpha test bits 20-21 and the
 * alpha reference bits 12-19.
 *
 * @param depthtestmode  the depth test mode (2 bits)
 * @param atestmode  the alpha test mode (2 bits)
 * @param aref  the alpha reference value (8 bits)
 */
extern "C" void cGraphicsMan_SetPixelTest(cGraphicsMan* self, int depthtestmode, int atestmode, int aref)
{
    int sa = (depthtestmode << 23) & 0x01800000;
    *(int*)((char*)self->m_pCurrentDrawState + 4) = (*(int*)((char*)self->m_pCurrentDrawState + 4) & ~0x01800000) | sa;
    int sb = (atestmode << 20) & 0x00300000;
    *(int*)((char*)self->m_pCurrentDrawState + 4) = (*(int*)((char*)self->m_pCurrentDrawState + 4) & ~0x00300000) | sb;
    int sc = (aref << 12) & 0x000FF000;
    *(int*)((char*)self->m_pCurrentDrawState + 4) = (*(int*)((char*)self->m_pCurrentDrawState + 4) & ~0x000FF000) | sc;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_SetTexture);
#ifdef SKIP_ASM
/**
 * Sets texture @p idx of the current draw state: the short at draw state + 0x10 + idx * 2 (On Tour SetTexture(texid,
 * contextID)).
 *
 * @param value  the texture id to store (On Tour texid)
 * @param idx  the texture context (On Tour contextID); not range-checked
 */
extern "C" void cGraphicsMan_SetTexture(cGraphicsMan* self, short value, int idx) {
    *(short*)((char*)self->m_pCurrentDrawState + (idx << 1) + 0x10) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_PushDrawState);
#ifdef SKIP_ASM

/**
 * Pushes the draw-state stack: copies the current draw state into the next entry and makes that entry current. No
 * overflow check.
 */
extern "C" void cGraphicsMan_PushDrawState(cGraphicsMan* self)
{
    sDrawState* top = self->m_pCurrentDrawState;
    top[1] = top[0];
    self->m_pCurrentDrawState = (sDrawState*)(self->m_pCurrentDrawState + 1);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_PopDrawState);
#ifdef SKIP_ASM
/**
 * Pops the draw-state stack: moves m_pCurrentDrawState back one sDrawState (20 bytes).
 *
 * No underflow check; the preview's version asserted that the pointer was above m_drawStateStack.
 *
 * @param graphicsMan  the graphics manager
 */
extern "C" void cGraphicsMan_PopDrawState(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0xE84))) = (int) ((*(int *)((char*)(graphicsMan) + (0xE84))) - 20);
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_DrawStateChanged);
#ifdef SKIP_ASM
typedef char* (*sFontGetFn_47D8)(char*);

static inline short* sFontRS_shorts_47D8(char* st)
{
    return (short*)(st + 0x10);
}

/**
 * Reports whether the current draw state differs from the active one.
 *
 * Compares the four state words of *m_pCurrentDrawState, then its two texture ids, with the draw state that vtable slot
 * 64 (cGraphicsMan_ActiveDrawState, &m_activeDrawState) returns; the virtual is called again for every word.
 *
 * @return 1 at the first difference, else 0
 */
extern "C" int cGraphicsMan_DrawStateChanged(cGraphicsMan* self)
{
    int i;
    unsigned int j;
    for (i = 0; i < 4; i++) {
        char* obj = (char*)self->mVtbl;
        int* current = (int*)((char*)self->m_pCurrentDrawState + (i << 2));
        char* cmp = ((sFontGetFn_47D8) * (void**)(obj + 0x204))(((char*)self) + *(short*)(obj + 0x200));
        if (*current != *(int*)(cmp + (i << 2))) return 1;
    }
    for (j = 0; j < 2; j++) {
        char* obj = (char*)self->mVtbl;
        short* current = sFontRS_shorts_47D8((char*)self->m_pCurrentDrawState) + j;
        char* cmp = ((sFontGetFn_47D8) * (void**)(obj + 0x204))(((char*)self) + *(short*)(obj + 0x200));
        if (*current != *(short*)(cmp + (j << 1) + 0x10)) return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_setEffectTexMap);
#ifdef SKIP_ASM
/**
 * Stores effect texture map entry @p idx: _EffectTexMap[idx] = @p value.
 *
 * No range check (86 entries); the preview's version asserted the index.
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the effect id
 * @param value  the texture id
 */
extern "C" void cGraphicsMan_setEffectTexMap(cGraphicsMan* graphicsMan, int idx, int value) {
    *(int*)(((char*)graphicsMan) + (idx << 2) + 0xF50) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cGraphicsMan_getEffectTexMap);
#ifdef SKIP_ASM
/**
 * Returns effect texture map entry @p idx (_EffectTexMap[idx]).
 *
 * No range check (86 entries).
 *
 * @param graphicsMan  the graphics manager
 * @param idx  the effect id
 * @return the texture id (-1 when unset)
 */
extern "C" int cGraphicsMan_getEffectTexMap(cGraphicsMan* graphicsMan, int idx) {
    return *(int*)(((char*)graphicsMan) + (idx << 2) + 0xF50);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003948D0);
#ifdef SKIP_ASM
/**
 * Returns a distance falloff factor that is 1 within 100 units and falls off as 1/d, 1/d^2 or 1/d^3.
 *
 * @p mode 1: 100 / d; 2: 10000 / d^2; 3: 1000000 / d^3, each only once @p distance reaches 100; any other mode returns
 * 1. No caller in C or asm (an out-of-line copy of an inline helper).
 *
 * @param mode  the falloff order (1-3)
 * @param distance  the distance
 * @return the factor, 1 at most
 */
extern "C" float func_003948D0(int mode, float distance)
{
    switch (mode) {
    case 1:
        if (distance < 100.0f) return 1.0f;
        return 100.0f / distance;
    case 2:
        if (distance < 100.0f) return 1.0f;
        return 10000.0f / (distance * distance);
    case 3:
        if (distance < 100.0f) return 1.0f;
        return 1000000.0f / (distance * distance * distance);
    }
    return 1.0f;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003949A0);
#ifdef SKIP_ASM
/**
 * Returns a near-range ramp: (100x)^n while 100x is below 1, else 1.
 *
 * @p mode 1, 2 or 3 picks the power n; any other mode returns 1. No caller in C or asm.
 *
 * @param mode  the ramp order (1-3)
 * @param x  the value, scaled by 100 first
 * @return the ramp value, 1 at most
 */
extern "C" float func_003949A0(int mode, float x)
{
    float scaled = x * 100.0f;
    if (scaled < 1.0f) {
        switch (mode) {
        case 1: return scaled;
        case 2: return scaled * scaled;
        case 3: return scaled * scaled * scaled;
        }
    }
    return 1.0f;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394A20);
#ifdef SKIP_ASM
/**
 * Raises @p x to the integer power @p n.
 *
 * Powers 0-4 are multiplied out; any other power uses square-and-multiply, with a negative power inverting @p x first;
 * 0 raised to such a power returns 0. No caller in C or asm.
 *
 * @param n  the exponent
 * @param x  the base
 * @return x^n
 */
extern "C" float func_00394A20(int n, float x) {
    float result;

    switch (n) {
    case 0:
        return 1.0f;
    case 1:
        return x;
    case 2:
        return x * x;
    case 3:
        return x * x * x;
    case 4:
        return x * x * x * x;
    default:
        result = 0.0f;
        if (x != 0.0f) {
            if (n < 0) {
                n = -n;
                x = 1.0f / x;
            }
            result = 1.0f;
            while (n != 0) {
                if (n & 1) {
                    result *= x;
                }
                n >>= 1;
                x *= x;
            }
        }
        return result;
    }
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394AD8);
#ifdef SKIP_ASM
/**
 * Returns a fade factor: 1, unless @p fadeGate is at least 5000 and @p fadeDistance is above 3750, then 1 - (b - 3750) * 0.0008.
 *
 * The factor reaches 0 at @p fadeDistance = 5000 and goes negative beyond it (no clamp). No caller in C or asm.
 *
 * @param fadeGate  a distance compared with 5000
 * @param fadeDistance  the distance that drives the fade
 * @return the fade factor
 */
extern "C" float func_00394AD8(float fadeGate, float fadeDistance)
{
    float factor;
    if (fadeGate < 5000.0f) return 1.0f;
    if (!(fadeDistance <= 3750.0f)) factor = 1.0f - (fadeDistance - 3750.0f) * 0.0008f;
    else factor = 1.0f;
    return factor;
}
#endif

//100%
INCLUDE_ASM("render/font", cGarbageMan__cGarbageMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00493938[];

/**
 * Destructor body: stores the vtable D_00493938 at +4 and frees the object when bit 0 of @p flags is set.
 *
 * It is the only slot (slot 1) of that one-entry vtable.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cGarbageMan__cGarbageMan(int* self, int flags)
{
    *(void**)((char*)self + 0x4) = D_00493938;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cGridMesh_SetScale__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of slot 9 of the grid-mesh vtable D_004938E0.
 *
 * The derived vtable cPS2GridMesh_vtbl overrides the slot with cPS2GridMesh_SetScale.
 */
void cGridMesh_SetScale(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_getBezierMan);
#ifdef SKIP_ASM
extern int cBezierMan_TheBezierMan;

/**
 * Returns the Bezier patch manager singleton (cBezierMan_TheBezierMan).
 *
 * @return cBezierMan_TheBezierMan
 */
extern "C" int cBezierMan_getBezierMan(void) {
    return cBezierMan_TheBezierMan;
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_Reset__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of slot 4 of the cBezierMan vtable cBezierMan_vtbl.
 *
 * cPS2BezierMan overrides the slot with cPS2BezierMan_Reset__FPv.
 */
void cBezierMan_Reset(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_UpdatePatchesCache__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base-class body of slot 5 of the cBezierMan vtable cBezierMan_vtbl.
 *
 * cPS2BezierMan keeps it.
 */
void cBezierMan_UpdatePatchesCache(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_SetPatchDist__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the current patch distance (m_currPatchDist, +0x0).
 *
 * Slot 19 of the cBezierMan vtable (cPS2BezierMan keeps it).
 *
 * @param val  the patch distance
 */
void cBezierMan_SetPatchDist(void* self, float val)
{
    *(float*)((char*)self + 0x0) = val;
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_GetPatchDist__FPv);
#ifdef SKIP_ASM
/**
 * Returns the current patch distance (m_currPatchDist, +0x0).
 *
 * Slot 20 of the cBezierMan vtable (cPS2BezierMan keeps it).
 *
 * @return m_currPatchDist
 */
float cBezierMan_GetPatchDist(void* self)
{
    return *(float*)((char*)self + 0x0);
}
#endif

//100%
INCLUDE_ASM("render/font", cBezierMan_AddPatch);
#ifdef SKIP_ASM
class cFontVirt {
public:
    int field_0x0;
    // vptr lands at 0x4 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
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
    virtual void v22(int, int, int, int, int);
};

/**
 * Forwards all five arguments to the virtual in vtable entry 22 (offset 0xB0) of the Bezier manager.
 *
 * That entry is this function's own slot in the cBezierMan vtable cBezierMan_vtbl; cPS2BezierMan overrides it with
 * func_0038B190, so the forward only reaches a derived implementation (on a plain cBezierMan it would call itself).
 *
 * @param patchRecord  the patch record (On Tour tPatchRecord* pPatch)
 * @param edgeLods  the edge LOD bytes (On Tour edges)
 * @param textureIds  the texture ids (On Tour texid)
 * @param clip  the clip flag (On Tour clip)
 * @param lightMats  the light matrices (preview tLightMats*)
 */
extern "C" void cBezierMan_AddPatch(cPS2BezierMan* self, int patchRecord, int edgeLods, int textureIds, int clip, int lightMats)
{
    ((cFontVirt*)self)->v22(patchRecord, edgeLods, textureIds, clip, lightMats);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_SetNearLOD__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the near level-of-detail distance for the Bezier patches.
 *
 * Stores @p nearLOD at +0x33C (On Tour mNearLODcm).
 *
 * @param nearLOD  the near LOD distance
 */
void cPS2BezierMan_SetNearLOD(void* self, float nearLOD)
{
    *(float*)((char*)self + 0x33C) = nearLOD;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_SetFarLOD__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the far level-of-detail distance for the Bezier patches.
 *
 * Stores @p farLOD at +0x340 (On Tour mFarLODcm).
 *
 * @param farLOD  the far LOD distance
 */
void cPS2BezierMan_SetFarLOD(void* self, float farLOD)
{
    *(float*)((char*)self + 0x340) = farLOD;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_GetNearLOD__FPv);
#ifdef SKIP_ASM
/**
 * Returns the near level-of-detail distance.
 *
 * Reads +0x33C (On Tour mNearLODcm).
 *
 * @return the near LOD distance
 */
float cPS2BezierMan_GetNearLOD(void* self)
{
    return *(float*)((char*)self + 0x33C);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_GetFarLOD__FPv);
#ifdef SKIP_ASM
/**
 * Returns the far level-of-detail distance.
 *
 * Reads +0x340 (On Tour mFarLODcm).
 *
 * @return the far LOD distance
 */
float cPS2BezierMan_GetFarLOD(void* self)
{
    return *(float*)((char*)self + 0x340);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_SetLmapStart__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the first light-map index used by the patches.
 *
 * Stores @p lmapStart at +0x354.
 *
 * @param lmapStart  the first light-map index
 */
void cPS2BezierMan_SetLmapStart(void* self, int lmapStart)
{
    *(int*)((char*)self + 0x354) = lmapStart;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_EnableWireMode__FPvi);
#ifdef SKIP_ASM
/**
 * Turns wireframe drawing of the patches on or off.
 *
 * Stores @p enable at +0x344 (On Tour mWireModeEnabled).
 *
 * @param enable  non-zero to draw wireframe
 */
void cPS2BezierMan_EnableWireMode(void* self, int enable)
{
    *(int*)((char*)self + 0x344) = enable;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_EnableLightMaps__FPvi);
#ifdef SKIP_ASM
/**
 * Turns the patches' light maps on or off.
 *
 * Stores @p enable at +0x34C (On Tour mLightMapsEnabled).
 *
 * @param enable  non-zero to use light maps
 */
void cPS2BezierMan_EnableLightMaps(void* self, int enable)
{
    *(int*)((char*)self + 0x34C) = enable;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_EnableShadows__FPvi);
#ifdef SKIP_ASM
/**
 * Turns shadows on the patches on or off.
 *
 * Stores @p enable at +0x350 (On Tour mShadowsEnabled).
 *
 * @param enable  non-zero to draw shadows
 */
void cPS2BezierMan_EnableShadows(void* self, int enable)
{
    *(int*)((char*)self + 0x350) = enable;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_IsWireModeEnabled__FPv);
#ifdef SKIP_ASM
/**
 * Reports whether the patches are drawn as wireframe.
 *
 * Reads +0x344.
 *
 * @return the wireframe flag
 */
int cPS2BezierMan_IsWireModeEnabled(void* self)
{
    return *(int*)((char*)self + 0x344);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_IsLightMapsEnabled__FPv);
#ifdef SKIP_ASM
/**
 * Reports whether the patches use light maps.
 *
 * Reads +0x34C.
 *
 * @return the light-map flag
 */
int cPS2BezierMan_IsLightMapsEnabled(void* self)
{
    return *(int*)((char*)self + 0x34C);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_IsShadowsEnabled__FPv);
#ifdef SKIP_ASM
/**
 * Reports whether shadows are drawn on the patches.
 *
 * Reads +0x350.
 *
 * @return the shadow flag
 */
int cPS2BezierMan_IsShadowsEnabled(void* self)
{
    return *(int*)((char*)self + 0x350);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2BezierMan_GetVifCache);
#ifdef SKIP_ASM
/**
 * Returns VIF cache cursor @p idx advanced by @p shift quadword blocks: mVifCachePos[idx] + (shift << 8).
 *
 * @param base  the Bezier patch manager
 * @param shift  the number of 256-byte blocks to skip
 * @param idx  the VIF cache (0: 4x4, 1: 6x6, 2: 8x8)
 * @return the cache address
 */
extern "C" int cPS2BezierMan_GetVifCache(cPS2BezierMan* base, int shift, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x44C) + (shift << 8);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2BezierMan_GetBezST);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x1A0 of the Bezier manager (inside the patch cache at +0x10).
 *
 * @param bezierMan  the Bezier patch manager
 * @return the word at +0x1A0
 */
extern "C" int cPS2BezierMan_GetBezST(cPS2BezierMan* bezierMan) {
    return (*(int *)((char*)(bezierMan) + (0x1A0)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2BezierMan_GetBezdST);
#ifdef SKIP_ASM
/**
 * Returns the word at +0x320 of the Bezier manager (inside the patch cache at +0x10).
 *
 * @param bezierMan  the Bezier patch manager
 * @return the word at +0x320
 */
extern "C" int cPS2BezierMan_GetBezdST(cPS2BezierMan* bezierMan) {
    return (*(int *)((char*)(bezierMan) + (0x320)));
}
#endif
//100%
INCLUDE_ASM("render/font", cLightMan_getTheLightMan);
#ifdef SKIP_ASM
extern int cLightMan_TheLightMan;

/**
 * Returns the light manager singleton (cLightMan_TheLightMan).
 *
 * @return cLightMan_TheLightMan
 */
extern "C" int cLightMan_getTheLightMan(void) {
    return cLightMan_TheLightMan;
}
#endif

//100%
INCLUDE_ASM("render/font", cLightMan_CreateAmbient__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cLightMan's CreateAmbient has an empty body.
 */
void cLightMan_CreateAmbient(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cLightMan_LoadLight__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for loading a light into a hardware slot: does nothing.
 *
 * cLightMan vtable slot 13; cPS2LightMan overrides it.
 */
void cLightMan_LoadLight(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cLightMan_SetChannel__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for setting a light channel's mask: does nothing.
 *
 * cLightMan vtable slot 14; cPS2LightMan overrides it.
 */
void cLightMan_SetChannel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cLightMan_ClearChannel__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for clearing a light channel: does nothing.
 *
 * cLightMan vtable slot 15; cPS2LightMan overrides it.
 */
void cLightMan_ClearChannel(void* self)
{
}
#endif

extern "C" void* cLightMan__cLightMan(void* self);

//100%
INCLUDE_ASM("render/font", cPS2LightMan__cPS2LightMan__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the PS2 light manager through the light-manager destructor cLightMan__cLightMan().
 *
 * That destructor clears the global cLightMan_TheLightMan and frees the object when bit 0 of the destructor flags is set.
 * Only @p self is declared here: the flags reach cLightMan__cLightMan() unchanged in their register.
 *
 * @return no meaningful value (cLightMan__cLightMan() returns nothing)
 */
void* cPS2LightMan__cPS2LightMan(void* self)
{
    return cLightMan__cLightMan(self);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_GetLightObj__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the address of light slot @p index in the light table (&m_pLights[index]).
 *
 * Slot 2 of the cPS2LightMan vtable D_00493650.
 *
 * @param index  the light index
 * @return a pointer to m_pLights[@p index]
 */
void* cPS2LightMan_GetLightObj(void* self, int index)
{
    return ((char*)*(void**)((char*)self + 0x4) + index * 4);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_Init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_LightInst[];

/**
 * Sets up the light manager for a number of static lights plus 50 dynamic ones.
 *
 * Stores @p numLights as m_iStartDynLight (the first dynamic light index), clears m_iDynLight, sets m_iMaxLights to @p numLights + 50
 * and allocates that many cLightInst pointers (operator_new_tag, tag str_LightInst) into m_pLights.
 *
 * @param numLights  the number of static lights
 */
extern "C" void cPS2LightMan_Init(cPS2LightMan* self, int numLights)
{
    self->m_iStartDynLight = numLights;
    self->m_iDynLight = 0;
    self->m_iMaxLights = numLights + 50;
    self->m_pLights = (cLightInst**)operator_new_tag((numLights + 50) * 4, str_LightInst, 0, 0);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_Purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees the light manager's light buffer.
 */
extern "C" void cPS2LightMan_Purge(cPS2LightMan* self)
{
    void* lights = self->m_pLights;
    if (lights != 0) {
        cMemMan_free(lights);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_ResetDynamicLights__FPv);
#ifdef SKIP_ASM
/**
 * Removes all dynamic lights by zeroing the light manager's dynamic light count (+0xC).
 */
void cPS2LightMan_ResetDynamicLights(void* self)
{
    *(int*)((char*)self + 0xC) = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_GetDynamicLight);
#ifdef SKIP_ASM
/**
 * Hands out the next dynamic light index.
 *
 * Returns m_iStartDynLight + m_iDynLight and increments m_iDynLight. No limit check: the preview asserted m_iDynLight <
 * 50 (the 50 spare slots cPS2LightMan_Init allocates). The C declares the result as char*; it is an int index.
 * cPS2LightMan_ResetDynamicLights__FPv sets the counter back to 0.
 *
 * @return the light index
 */
extern "C" char* cPS2LightMan_GetDynamicLight(cPS2LightMan* self)
{
    return (char*)self->m_iStartDynLight + (self->m_iDynLight)++;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_SetAmbient__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cPS2LightMan's SetAmbient has an empty body.
 */
void cPS2LightMan_SetAmbient(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_LoadLight__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cPS2LightMan's body for vtable slot 13.
 *
 * Slot 13 of the cPS2LightMan vtable D_00493650; it overrides cLightMan_LoadLight__FPv, which is empty too.
 */
void cPS2LightMan_LoadLight(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_SetChannel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cPS2LightMan's body for vtable slot 14.
 *
 * Slot 14 of the cPS2LightMan vtable D_00493650; it overrides cLightMan_SetChannel__FPv, which is empty too.
 */
void cPS2LightMan_SetChannel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2LightMan_ClearChannel__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cPS2LightMan's body for vtable slot 15.
 *
 * Slot 15 of the cPS2LightMan vtable D_00493650; it overrides cLightMan_ClearChannel__FPv, which is empty too.
 */
void cPS2LightMan_ClearChannel(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2DeferManager_cPS2DeferManager);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

struct sFontTexKey {
    char block[0x10];       // 0x0
    short ids[2];           // 0x10
    sFontTexKey()
    {
        for (unsigned int i = 0; i < 2; i++) {
            ids[i] = -1;
        }
        func_00416210(block, 0, 0x10);
    }
};

struct sFontGlyph80 {
    sFontTexKey key;        // 0x0
    char pad_0x14[0x6C];
};

struct sFontGlyph18 {
    sFontTexKey key;        // 0x0
    int field_0x14;
};

struct sFontEmpty24 {
    int a;
    sFontEmpty24() {}
};

struct sFontEmptyC {
    int a;
    sFontEmptyC() {}
};

struct sFontCache {
    char pad_0x0[0x80];
    sFontGlyph80 glyphs[0xA28];     // 0x80
    int field_0x51480;
    sFontGlyph18 small[0x400];      // 0x51484
    sFontEmpty24 e1[0x800];         // 0x57484
    char pad_0x59484[0x69CA8 - 0x59484];
    sFontTexKey key;                // 0x69CA8
    sFontEmptyC e2[0xC];            // 0x69CBC
    sFontCache() {}
};

inline void* operator new(unsigned int, void* p) { return p; }

/**
 * Constructs the deferred-draw manager in place and returns it.
 *
 * Runs the element constructors of its tables: 2600 (0xA28) 0x80-byte draw records from +0x80 and 1024 0x18-byte state
 * entries from +0x51484, each with a zeroed 16-byte state and both texture ids -1; the state at +0x69CA8 likewise; 2048
 * words from +0x57484 and twelve from +0x69CBC with empty constructors. Neither count (+0x0 for the draw records,
 * +0x51480 for the state entries) is set here. Called from cPS2GraphicsMan_Init. The C's struct names (sFontCache,
 * sFontGlyph80, ...) are placeholders: the object is the one mpDeferMan points to.
 *
 * @return @p self
 */
extern "C" void* cPS2DeferManager_cPS2DeferManager(void* self)
{
    new (self) sFontCache;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2DeferManager_Find);
#ifdef SKIP_ASM
struct sGlyphKey {
    int k[4];
    int v;
};

struct sGlyphEntry {
    sGlyphKey key;          // 0x00
    sGlyphEntry* next;      // 0x14
};

struct sGlyphCache {
    char pad_0x00[0x51480];
    int count;                  // 0x51480
    sGlyphEntry entries[3755];  // 0x51484
    char pad_0x6748C[0x14];
    sGlyphEntry* buckets[256];  // 0x674A0
};

static inline int glyphKeyEq(const int* a, const int* b)
{
    unsigned int i;
    for (i = 0; i < 4; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Finds or adds the defer manager's shared state entry for a draw state, by hash bucket.
 *
 * Walks the chain of bucket @p hash (heads at +0x674A0) comparing the four state words (not the texture-id word @p key
 * carries); a hit is moved to the front of its chain and returned. On a miss it copies @p key into the next free entry
 * (+0x51484, count at +0x51480), pushes it on the chain and returns it. There is no capacity check on the 1024-entry
 * table. Callers: cPS2BezierMan_DrawPatches and the bucket builder func_00362DE8.
 *
 * @param key  the draw state to look up (four state words + the texture-id word)
 * @param hash  the bucket index (an 8-bit hash of the state words)
 * @return the matching or new state entry
 */
extern "C" sGlyphEntry* cPS2DeferManager_Find(sGlyphCache* self, sGlyphKey* key, unsigned char hash)
{
    sGlyphEntry** pp = &self->buckets[hash];
    while (*pp != 0) {
        sGlyphEntry* entry = *pp;
        if (glyphKeyEq(entry->key.k, key->k)) {
            if (entry == self->buckets[hash]) {
                return entry;
            }
            *pp = entry->next;
            entry->next = self->buckets[hash];
            self->buckets[hash] = entry;
            return entry;
        }
        pp = &entry->next;
    }
    sGlyphEntry* entry = &self->entries[self->count++];
    entry->key = *key;
    entry->next = self->buckets[hash];
    self->buckets[hash] = entry;
    return entry;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2DeferManager_Find1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sNode5000 {
    int k0;
    int k4;
    int k8;
    int padC[2];
    int busy;
    int pad18;
    sNode5000* next;
};

struct sCache5000 {
    char pad0[0x678A0];
    sNode5000* buckets[256];
};

/**
 * Looks up the defer manager's draw bucket for a (state, texture key, matrix) triple.
 *
 * Walks the chain of bucket @p hash (heads at +0x678A0), skipping nodes whose busy word (+0x14) is set, and compares
 * state, texture key and matrix index. A hit is moved to the front of its chain and returned; a miss returns 0 (the
 * callers then add a bucket). Callers: cPS2BezierMan_DrawPatches, cPS2GraphicsMan_RenderModel and func_00362DE8.
 *
 * @param stateEntry  the state entry (from cPS2DeferManager_Find)
 * @param texKey  the texture key (both texture ids)
 * @param hash  the bucket index
 * @param matrixIndex  the matrix index
 * @return the bucket, or 0 if none matches
 */
extern "C" sNode5000* cPS2DeferManager_Find1(sCache5000* self, int stateEntry, int texKey, unsigned char hash, int matrixIndex)
{
    sNode5000** pp = &self->buckets[hash];
    while (*pp != 0) {
        sNode5000* node;
        int hit = 0;
        if ((*pp)->busy == 0) {
            int keysMatch = (*pp)->k4 == texKey && (*pp)->k8 == matrixIndex && (*pp)->k0 == stateEntry;
            if (keysMatch) hit = 1;
        }
        node = *pp;
        if (hit) {
            if ((*pp) == self->buckets[hash]) {
                return node;
            }
            *pp = node->next;
            node->next = self->buckets[hash];
            self->buckets[hash] = node;
            return node;
        }
        pp = &node->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_sViewportStack_operatorE);
#ifdef SKIP_ASM
struct sVec50C0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat50C0 {
    float m[16];
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat50C0& operator=(const sMat50C0& o)
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
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

struct sPair50C0 {
    int a, b;
};

struct sFontState50C0 {
    float f00, f04, f08, f0C, f10, f14, f18, f1C;
    int i20;
    float f24, f28, f2C;
    sVec50C0 v30, v40, v50;
    sMat50C0 m60, mA0, mE0, m120;
    sVec50C0 v160, v170;
    sMat50C0 m180, m1C0;
    sVec50C0 v200, v210;
    sPair50C0 p220;
    int i228;
};

/**
 * Copies one pushed viewport state (sViewportStack, 0x230 bytes) member by member and returns @p self.
 *
 * The compiler-generated assignment: floats and ints word by word, vectors and matrices with VU0 quadword copies.
 * cPS2GraphicsMan_PushViewport uses it. (The line comment above it calls it the font render state; it is the viewport
 * stack entry.)
 *
 * @param other  the entry to copy
 * @return @p self
 */
extern "C" sFontState50C0* cPS2GraphicsMan_sViewportStack_operatorE(cPS2GraphicsMan* self, const sFontState50C0* other)
{
    *((sFontState50C0*)self) = *other;
    return ((sFontState50C0*)self);
}
#endif

extern "C" void* cGraphicsMan__cGraphicsMan(void* self);

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan__cPS2GraphicsMan__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the PS2 graphics manager by chaining to cGraphicsMan__cGraphicsMan.
 *
 * Declared with only the this pointer: the destructor flags stay in $a1 and reach the base destructor, which frees
 * the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cPS2GraphicsMan__cPS2GraphicsMan(void* self)
{
    return cGraphicsMan__cGraphicsMan(self);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_cPS2GraphicsMan);
#ifdef SKIP_ASM
extern "C" void* cGraphicsMan_cGraphicsMan(void* self);
extern void* cPS2GraphicsMan_vtbl[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sRenderSlot5288 {
    float v[4];
    sRenderSlot5288() {}
};

/**
 * Constructs the PS2 graphics manager: runs the cGraphicsMan constructor and stores cPS2GraphicsMan_vtbl.
 *
 * Then placement-constructs five sRenderSlot5288 arrays (12 at +0x10E0, 8 at +0x11A0, 2 at +0x1220, 2 at +0x1240, 4
 * at +0x1260); the element type has no constructor, so these loops do no work.
 *
 * @return @p self
 */
extern "C" void* cPS2GraphicsMan_cPS2GraphicsMan(cPS2GraphicsMan* self)
{
    cGraphicsMan_cGraphicsMan(self);
    self->mVtbl = cPS2GraphicsMan_vtbl;
    new ((char*)&self->mViewStack) sRenderSlot5288[12];
    new ((char*)self + 0x11A0) sRenderSlot5288[8];
    new ((char*)self + 0x1220) sRenderSlot5288[2];
    new ((char*)self + 0x1240) sRenderSlot5288[2];
    new ((char*)self + 0x1260) sRenderSlot5288[4];
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetNumDisplayModes__FPv);
#ifdef SKIP_ASM
/**
 * Returns how many display modes the renderer offers.
 *
 * Reads the count at +0x59C4.
 *
 * @return the number of display modes
 */
int cPS2GraphicsMan_GetNumDisplayModes(void* self)
{
    return *(int*)((char*)self + 0x59C4);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetDTV__FPv);
#ifdef SKIP_ASM
/**
 * Returns the DTV setting held in unk_5A4C.
 *
 * @return the word at unk_5A4C
 */
int cPS2GraphicsMan_GetDTV(void* self)
{
    return *(int*)((char*)self + 0x5A4C);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetDisplayMode1__FPv);
#ifdef SKIP_ASM
/**
 * Returns CurrentDisplayMode (-1 until a mode has been set).
 *
 * @return the current display mode index
 */
int cPS2GraphicsMan_GetDisplayMode1(void* self)
{
    return *(int*)((char*)self + 0x59C8);
}
#endif

// 0xa4-byte elements reached through a pointer at self+0x59cc, indexed by
// the counter at self+0x59c8
struct sFontEntry {
    char pad_0x00[0x88];
    int field_0x88;
    int field_0x8c;
    char pad_0x90[0x14];
};

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetScreenWidth);
#ifdef SKIP_ASM
/**
 * Returns the width of the current display mode: DisplayModes[CurrentDisplayMode] field +0x88.
 *
 * @return the screen width in pixels
 */
extern "C" int cPS2GraphicsMan_GetScreenWidth(cPS2GraphicsMan* self)
{
    int i = self->CurrentDisplayMode;
    sFontEntry* modes = (sFontEntry*)self->DisplayModes;
    return modes[i].field_0x88;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetScreenHeight);
#ifdef SKIP_ASM
/**
 * Returns the height of the current display mode: DisplayModes[CurrentDisplayMode] field +0x8C.
 *
 * @return the screen height in pixels
 */
extern "C" int cPS2GraphicsMan_GetScreenHeight(cPS2GraphicsMan* self)
{
    int i = self->CurrentDisplayMode;
    sFontEntry* modes = (sFontEntry*)self->DisplayModes;
    return modes[i].field_0x8c;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetViewportWindow1);
#ifdef SKIP_ASM
struct sFontVEntry23 {
    short delta;
    short index;
    void (*fn)(void*, int, float, float, float, float);
};

/**
 * Sets the viewport window from a rectangle: calls virtual slot 23 (cPS2GraphicsMan_SetViewportWindow) with
 * @p applyScreenOffset and the four floats of @p rect.
 *
 * @param rect  x, y, width and height (four floats)
 * @param applyScreenOffset  passed through as the slot's first argument
 */
extern "C" void cPS2GraphicsMan_SetViewportWindow1(cPS2GraphicsMan* self, float* rect, int applyScreenOffset)
{
    sFontVEntry23* vt = (sFontVEntry23*)self->mVtbl;
    vt[23].fn((char*)self + vt[23].delta, applyScreenOffset, rect[0], rect[1], rect[2], rect[3]);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetTVDisplayFormat__FPv);
#ifdef SKIP_ASM
/**
 * Returns the TV display format record.
 *
 * Returns the address of the record at +0x6B94.
 *
 * @return this + 0x6B94
 */
void* cPS2GraphicsMan_GetTVDisplayFormat(void* self)
{
    return &((cPS2GraphicsMan*)self)->mTVDisplayFormat;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetNetMatrix);
#ifdef SKIP_ASM
extern "C" void cPS2GraphicsMan_updateNetMat(cPS2GraphicsMan* self);

/**
 * Copies the graphics manager's combined matrix (mNetMatrix) into @p out, recomputing it first if stale.
 *
 * Calls cPS2GraphicsMan_updateNetMat (recompute when mMatricesValid is 0), then copies the 64-byte matrix with VU0. Returns through
 * the hidden result pointer, hence @p out comes first. cPS2GraphicsMan vtable slot 43.
 *
 * @param out  receives the matrix
 * @return @p out
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void* cPS2GraphicsMan_GetNetMatrix(void* out, void* self)
{
    cPS2GraphicsMan_updateNetMat((cPS2GraphicsMan*)self);
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
        : "r"(out), "r"((char*)self + 0x58A0)
        : "memory");
    return out;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetGuardBandMatrix);
#ifdef SKIP_ASM
/**
 * Returns the address of the guard-band matrix (&mGuardBandMatrix, +0x5800).
 *
 * @param graphicsMan  the graphics manager
 * @return a pointer to mGuardBandMatrix (as an int)
 */
extern "C" int cPS2GraphicsMan_GetGuardBandMatrix(int graphicsMan) {
    return graphicsMan + 22528;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_ClearProjTextures__FPv);
#ifdef SKIP_ASM
/**
 * Forgets the projected textures queued for the frame.
 *
 * Sets mNumProjTextures (+0x13EC) to 0; cPS2GraphicsMan_AddProjectiveTexture() adds them again.
 */
void cPS2GraphicsMan_ClearProjTextures(void* self)
{
    *(int*)((char*)self + 0x13EC) = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetNumProjTextures);
#ifdef SKIP_ASM
/**
 * Returns the number of projective textures (mNumProjTextures).
 *
 * @param pS2GraphicsMan  the graphics manager
 * @return mNumProjTextures
 */
extern "C" int cPS2GraphicsMan_GetNumProjTextures(cPS2GraphicsMan* pS2GraphicsMan) {
    return (pS2GraphicsMan->mNumProjTextures);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetProjTexInfo);
#ifdef SKIP_ASM
/**
 * Returns the address of projective texture @p idx (&mProjTextures[idx], 0xA0 bytes each).
 *
 * No range check (eight entries); the preview's version asserted the index.
 *
 * @param base  the graphics manager
 * @param idx  the projective texture index
 * @return a pointer to the entry
 */
extern "C" char* cPS2GraphicsMan_GetProjTexInfo(cPS2GraphicsMan* base, int idx)
{
    return ((char*)base) + 0x13F0 + idx * 160;
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetVifCodeState);
#ifdef SKIP_ASM
/**
 * Sets bits 6-9 of word 0 of the current draw state (*m_pCurrentDrawState) (the VIF code state).
 *
 * @param ps2GraphicsMan  the graphics manager
 * @param value  the VIF code state (preview VifCodeState; 4 bits)
 */
extern "C" void cPS2GraphicsMan_SetVifCodeState(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    int *temp_6;

    temp_6 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    *temp_6 = (*temp_6 & ~0x3C0) | ((value << 6) & 0x3C0);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetVUCodeInitType);
#ifdef SKIP_ASM
/**
 * Sets bits 10-11 of word 0 of the current draw state (*m_pCurrentDrawState) (the VU code init type).
 *
 * @param ps2GraphicsMan  the graphics manager
 * @param value  the VU code init type (preview VUCodeInitType; 2 bits)
 */
extern "C" void cPS2GraphicsMan_SetVUCodeInitType(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    int *temp_6;

    temp_6 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    *temp_6 = (*temp_6 & ~0xC00) | ((value << 0xA) & 0xC00);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetFixAlpha);
#ifdef SKIP_ASM
/**
 * Sets the fixed alpha of one GS context in the current draw state (*m_pCurrentDrawState): 8 bits of word 0 at bit 12 +
 * 8 * @p context.
 *
 * @param ps2GraphicsMan  the graphics manager
 * @param alpha  the fixed alpha (0-255)
 * @param context  the GS context (0 or 1)
 */
extern "C" void cPS2GraphicsMan_SetFixAlpha(cPS2GraphicsMan* ps2GraphicsMan, int alpha, int context) {
    int *temp_7;
    int temp_3;
    int temp_6;

    temp_7 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    temp_6 = (context * 8) + 0xC;
    temp_3 = 0xFF << temp_6;
    *temp_7 = (*temp_7 & ~temp_3) | (((alpha & 0xFF) << temp_6) & temp_3);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetIrradiance);
#ifdef SKIP_ASM
struct sFontBlockA0 {
    float f[40];
} __attribute__((aligned(16)));

/**
 * Copies the 0xA0-byte irradiance block @p src into mIrradianceInfo (+0x6BB0). On Tour's SetIrradiance also takes a
 * bool that it ignores; retail has no such parameter.
 *
 * @param src  the irradiance info to use
 */
extern "C" void cPS2GraphicsMan_SetIrradiance(cPS2GraphicsMan* self, sFontBlockA0* src)
{
    *(sFontBlockA0*)((char*)self + 0x6BB0) = *src;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetIrradiance__FPv);
#ifdef SKIP_ASM
/**
 * Returns the irradiance (ambient lighting) record.
 *
 * Returns the address of mIrradianceInfo (+0x6BB0), which cPS2GraphicsMan_SetIrradiance() fills.
 *
 * @return &mIrradianceInfo
 */
void* cPS2GraphicsMan_GetIrradiance(void* self)
{
    return &((cPS2GraphicsMan*)self)->mIrradianceInfo;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_InitCharModelPart__FPv);
#ifdef SKIP_ASM
/**
 * Prepares one part of a character model: empty in the PS2 renderer.
 *
 * cAnimModel_compile() calls slot 110.
 */
void cPS2GraphicsMan_InitCharModelPart(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_getCurrentFrameCount__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of frames the renderer has drawn.
 *
 * Reads the counter at +0x5A74. The AI compares it with each rider's last-drawn frame before deleting riders.
 *
 * @return the frame counter
 */
int cPS2GraphicsMan_getCurrentFrameCount(void* self)
{
    return *(int*)((char*)self + 0x5A74);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetCurrentFrameBufferPage);
#ifdef SKIP_ASM
/**
 * Returns the current frame-buffer page (mCurrentFrameBufferPage).
 *
 * @param graphicsMan  the graphics manager
 * @return mCurrentFrameBufferPage
 */
extern "C" int cPS2GraphicsMan_GetCurrentFrameBufferPage(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A84)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetCurrentZBufferPage);
#ifdef SKIP_ASM
/**
 * Returns the current Z-buffer page (mCurrentZBufferPage).
 *
 * @param graphicsMan  the graphics manager
 * @return mCurrentZBufferPage
 */
extern "C" int cPS2GraphicsMan_GetCurrentZBufferPage(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A80)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetCurrentFBFormat);
#ifdef SKIP_ASM
/**
 * Returns the current frame-buffer pixel format (mCurrentFBFormat).
 *
 * @param graphicsMan  the graphics manager
 * @return mCurrentFBFormat
 */
extern "C" int cPS2GraphicsMan_GetCurrentFBFormat(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A3C)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetCurrentZBFormat);
#ifdef SKIP_ASM
/**
 * Returns the current Z-buffer format (mCurrentZBFormat).
 *
 * @param graphicsMan  the graphics manager
 * @return mCurrentZBFormat
 */
extern "C" int cPS2GraphicsMan_GetCurrentZBFormat(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A40)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetTextureMan);
#ifdef SKIP_ASM
/**
 * Returns the texture manager (mpTextureMan).
 *
 * @param pS2GraphicsMan  the graphics manager
 * @return mpTextureMan
 */
extern "C" int cPS2GraphicsMan_GetTextureMan(cPS2GraphicsMan* pS2GraphicsMan) {
    return ((int)pS2GraphicsMan->mpTextureMan);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetMinFlipWait);
#ifdef SKIP_ASM
/**
 * Sets the minimum number of vblanks between page flips (mMinFlipWait).
 *
 * @param graphicsMan  the graphics manager
 * @param minWait  the vblank count
 */
extern "C" void cPS2GraphicsMan_SetMinFlipWait(void *graphicsMan, int minWait) {
    (*(int *)((char*)(graphicsMan) + (0x5AB8))) = minWait;
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetDisplayBufferAddress);
#ifdef SKIP_ASM
/**
 * Returns the display buffer address (mDisplayBufferAddress).
 *
 * @param graphicsMan  the graphics manager
 * @return mDisplayBufferAddress
 */
extern "C" int cPS2GraphicsMan_GetDisplayBufferAddress(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A88)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395550);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's word at +0x5A5C (unk_5A5C).
 *
 * A retail-only accessor: the preview's run of out-of-line accessors has nothing between the display-buffer and
 * frame-buffer-width ones.
 *
 * @param graphicsMan  the graphics manager
 * @return unk_5A5C
 */
extern "C" int func_00395550(cPS2GraphicsMan* graphicsMan) {
    return (graphicsMan->unk_5A5C);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetFrameBufferWidth);
#ifdef SKIP_ASM
/**
 * Returns the frame-buffer width (mFrameBufferWidth).
 *
 * @param graphicsMan  the graphics manager
 * @return mFrameBufferWidth
 */
extern "C" int cPS2GraphicsMan_GetFrameBufferWidth(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A50)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetPSMFormat);
#ifdef SKIP_ASM
/**
 * Returns the current frame-buffer pixel format (mCurrentFBFormat), like cPS2GraphicsMan_GetCurrentFBFormat.
 *
 * @param graphicsMan  the graphics manager
 * @return mCurrentFBFormat
 */
extern "C" int cPS2GraphicsMan_GetPSMFormat(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A3C)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_EnableFilter);
#ifdef SKIP_ASM
/**
 * Enables the display filter: sets mFilterEnabled to 1.
 *
 * @param graphicsMan  the graphics manager
 */
extern "C" void cPS2GraphicsMan_EnableFilter(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0x59E8))) = 1;
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_DisableFilter);
#ifdef SKIP_ASM
/**
 * Disables the display filter: sets mFilterEnabled to 0.
 *
 * @param graphicsMan  the graphics manager
 */
extern "C" void cPS2GraphicsMan_DisableFilter(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0x59E8))) = 0;
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetScreenBrightness);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator.
static inline float clamp_5580(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

/**
 * Sets the screen brightness (mScreenBrightness), clamped to 0..2.
 *
 * Values below 0 (and NaN) store 0; values above 2 store 2.
 *
 * @param x  the requested brightness
 */
extern "C" void cPS2GraphicsMan_SetScreenBrightness(cPS2GraphicsMan* self, float x)
{
    self->mScreenBrightness = clamp_5580(x, 2.0f);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetScreenBrightness);
#ifdef SKIP_ASM
/**
 * Returns the screen brightness (mScreenBrightness).
 *
 * @param graphicsMan  the graphics manager
 * @return mScreenBrightness
 */
extern "C" float cPS2GraphicsMan_GetScreenBrightness(void *graphicsMan) {
    return (*(float *)((char*)(graphicsMan) + (0x59FC)));
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_AddDMAToWorld);
#ifdef SKIP_ASM
struct sEnt_55B0 {
    sDrawState rs;                    // 0x00
    unsigned int buf;               // 0x14
    unsigned int next;              // 0x18
    short key;                      // 0x1C
    short flag;                     // 0x1E
    char pad_0x20[0x60];
};
struct sRing_55B0 {
    int count;                      // 0x0
    char pad_0x4[0x7C];
    sEnt_55B0 ents[1];              // 0x80
};
struct sGfx_55B0 {
    char pad_0x0[0xE84];
    sDrawState* top;                  // 0xE84
    char pad_0xE88[0x18F0 - 0xE88];
    sRing_55B0* ring;               // 0x18F0
};

/**
 * Queues a DMA chain as a deferred draw record carrying the current draw state.
 *
 * Does nothing once the defer manager (mpDeferMan) holds 2600 records. Otherwise it copies the low 5 bits of the defer
 * manager's word at +0x69CC4 into the low 5 bits of word 2 of the current draw state, then appends a 0x80-byte record
 * through the uncached alias (| 0x30000000): the draw state, @p buf, @p next, the defer manager's word at +0x69CC0 as a
 * 16-bit key, and @p flag.
 *
 * @param gfx  the graphics manager
 * @param buf  the start of the DMA chain
 * @param next  the chain's end / next pointer
 * @param flag  a flag stored with the record (preview: bool)
 */
extern "C" void cPS2GraphicsMan_AddDMAToWorld(cPS2GraphicsMan* gfx, unsigned int buf, unsigned int next, int flag)
{
    sDrawState* rs = gfx->m_pCurrentDrawState;
    sRing_55B0* ring = (sRing_55B0*)gfx->mpDeferMan;
    if (ring->count < 2600) {
        rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_55B0* record = (sEnt_55B0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        record->rs = *rs;
        record->buf = buf;
        record->next = next;
        record->key = key;
        record->flag = flag;
    }
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetDeferMan);
#ifdef SKIP_ASM
/**
 * Returns the deferred-draw manager (mpDeferMan).
 *
 * @param pS2GraphicsMan  the graphics manager
 * @return mpDeferMan (as an int)
 */
extern "C" int cPS2GraphicsMan_GetDeferMan(cPS2GraphicsMan* pS2GraphicsMan) {
    return ((int)pS2GraphicsMan->mpDeferMan);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GS_Revision);
#ifdef SKIP_ASM
/**
 * Returns the GS revision: bits 16-23 of the GS CSR register (0x12001000).
 *
 * @return the revision byte
 */
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int cPS2GraphicsMan_GS_Revision(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 16) & 0xFF);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GS_ID);
#ifdef SKIP_ASM
/**
 * Returns the GS id: bits 24-31 of the GS CSR register (0x12001000).
 *
 * @return the id byte
 */
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int cPS2GraphicsMan_GS_ID(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 24) & 0xFF);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_updateNetMat);
#ifdef SKIP_ASM
extern "C" void func_0037D968(void* self);

/**
 * Recomputes the combined matrices with func_0037D968 when mMatricesValid is 0.
 *
 * Called by cPS2GraphicsMan_GetNetMatrix before it copies mNetMatrix.
 */
extern "C" void cPS2GraphicsMan_updateNetMat(cPS2GraphicsMan* self)
{
    if (self->mMatricesValid == 0) {
        func_0037D968(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_003956D8);
#ifdef SKIP_ASM
/**
 * Returns the graphics manager's word at +0x59C0 (unk_59C0).
 *
 * func_00363490 uses the field as a loop count. A retail-only accessor (the preview's run has no counterpart).
 *
 * @param pS2GraphicsMan  the graphics manager
 * @return unk_59C0
 */
extern "C" int func_003956D8(cPS2GraphicsMan* pS2GraphicsMan) {
    return (pS2GraphicsMan->unk_59C0);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetShinyBoarder__FPvi);
#ifdef SKIP_ASM
/**
 * Turns the shiny-boarder (chrome rider) rendering flag on or off.
 *
 * Stores @p state in the word at +0x6D14.
 *
 * @param state  non-zero to draw riders shiny (On Tour State)
 */
void cPS2GraphicsMan_SetShinyBoarder(void* self, int state)
{
    *(int*)((char*)self + 0x6D14) = state;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_PushView);
#ifdef SKIP_ASM
/**
 * Pushes the view matrix stack: copies the top matrix (mCurrentView) into the next 0x40-byte slot with VU0, advances
 * mCurrentView to it and increments mViewDepth. No overflow check.
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void cPS2GraphicsMan_PushView(cPS2GraphicsMan* self)
{
    char* top = (char*)self->mCurrentView;
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
        : "r"(top + 0x40), "r"(top)
        : "memory");
    *(char**)&self->mCurrentView += 0x40;
    self->mViewDepth += 1;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_PopView__FPv);
#ifdef SKIP_ASM
/**
 * Pops the view-matrix stack.
 *
 * Moves mCurrentView (+0x13E4) back by one 0x40-byte matrix, decrements mViewDepth (+0x13E0) and clears mMatricesValid
 * (+0x6B90) so the combined matrices are rebuilt. No underflow check (the preview asserted).
 */
void cPS2GraphicsMan_PopView(void* self)
{
    *(int*)((char*)self + 0x6b90) = 0;
    *(char**)((char*)self + 0x13e4) -= 0x40;
    *(int*)((char*)self + 0x13e0) -= 1;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetView);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern char t4mat_Identity[];
extern float t4vect_XAxis[4];
extern float t4vect_YAxis[4];
extern float t4vect_ZAxis[4];

struct sMat_5750 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVec_5750 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_5750 Scale_5750(const sVec_5750& v, float s)
{
    sVec_5750 r;
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

// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy).
static inline void LoadIdentity_5750(void* self)
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
        : "r"(*(char**)((char*)self + 0x13e4)), "r"(t4mat_Identity)
        : "memory");
}

// Multiply the top of the matrix stack by a rotation of `angle` about `axis`.
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply).
static inline void Rotate_5750(void* self, float* axis, float angle)
{
    sMat_5750 r;
    float s;
    float c;
    char* top = *(char**)((char*)self + 0x13e4);
    fSinCos(&s, &c, angle);
    float t = 1.0f - c;
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r.m[0][0] = tx * x + c;
    r.m[1][0] = tx * y + sz;
    r.m[2][0] = tx * z - sy;
    r.m[0][1] = ty * x - sz;
    r.m[1][1] = ty * y + c;
    r.m[2][1] = ty * z + sx;
    r.m[0][2] = tz * x + sy;
    r.m[1][2] = tz * y - sx;
    r.m[2][2] = tz * z + c;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[0][3] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][3] = 1.0f;
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
        : "r"(top), "r"(&r)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (matrix-vector product into row 3).
static inline void Translate_5750(void* self, void* v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, 0x0(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(*(char**)((char*)self + 0x13e4)), "r"(v)
        : "memory");
}

/**
 * Builds the view matrix in *mCurrentView from a camera position and three rotation angles.
 *
 * Loads t4mat_Identity, swaps the Y and Z axes (elements 5 and 10 become 0, 6 and 9 become 1), then multiplies in
 * rotations by -@p rx about t4vect_XAxis, -@p ry about t4vect_ZAxis and -@p rz about t4vect_YAxis (VU0 code), and
 * finally translates by -@p pos (its w is kept). Clears mMatricesValid so the combined matrices are rebuilt.
 *
 * @param pos  the camera position
 * @param rx  the rotation about the X axis, in radians
 * @param ry  the rotation applied about the Z axis after the axis swap, in radians
 * @param rz  the rotation applied about the Y axis after the axis swap, in radians
 */
extern "C" void cPS2GraphicsMan_SetView(cPS2GraphicsMan* self, sVec_5750* pos, float rx, float ry, float rz)
{
    LoadIdentity_5750(self);
    ((float*)self->mCurrentView)[5] = 0.0f;
    ((float*)self->mCurrentView)[9] = 1.0f;
    ((float*)self->mCurrentView)[6] = 1.0f;
    ((float*)self->mCurrentView)[10] = 0.0f;
    Rotate_5750(self, t4vect_XAxis, -rx);
    Rotate_5750(self, t4vect_ZAxis, -ry);
    Rotate_5750(self, t4vect_YAxis, -rz);
    sVec_5750 negPos = Scale_5750(*pos, -1.0f);
    negPos.w = -negPos.w;
    Translate_5750(self, &negPos);
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetView1);
#ifdef SKIP_ASM
/**
 * Copies the 4x4 matrix @p viewMat into *mCurrentView (VU0 quadword copy) and clears mMatricesValid.
 *
 * @param viewMat  the view matrix to load
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void cPS2GraphicsMan_SetView1(cPS2GraphicsMan* self, void* viewMat)
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
        : "r"((char*)self->mCurrentView), "r"(viewMat)
        : "memory");
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetView__FPv);
#ifdef SKIP_ASM
/**
 * Returns the current view matrix.
 *
 * Returns mCurrentView (+0x13E4), the top of the view stack.
 *
 * @return the current view matrix
 */
int cPS2GraphicsMan_GetView(void* self)
{
    return *(int*)((char*)self + 0x13E4);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_AppendView);
#ifdef SKIP_ASM
/**
 * Concatenates @p mat onto the top view matrix in place with VU0 (each of @p mat's four vectors is transformed by the
 * current top matrix) and clears mMatricesValid.
 *
 * @param mat  the 4x4 matrix to append (On Tour m)
 */
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply); the PC port needs a plain matrix multiply.
extern "C" void cPS2GraphicsMan_AppendView(cPS2GraphicsMan* self, void* mat)
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
        : "r"((char*)self->mCurrentView), "r"(mat)
        : "memory");
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_IdentityView);
#ifdef SKIP_ASM
extern char t4mat_Identity[];

/**
 * Sets the top view matrix (mCurrentView) to t4mat_Identity.
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void cPS2GraphicsMan_IdentityView(cPS2GraphicsMan* self)
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
        : "r"((char*)self->mCurrentView), "r"(t4mat_Identity)
        : "memory");
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_TranslateView);
#ifdef SKIP_ASM
/**
 * Translates the top view matrix: its fourth row becomes the top matrix applied to @p pos (a 4-vector, VU0), then
 * clears mMatricesValid.
 *
 * @param pos  the translation as a 4-vector (On Tour pos)
 */
// PORT: PS2-only VU0 inline asm (vmula/vmadd matrix-vector product); the PC port
// needs a C fallback.
extern "C" void cPS2GraphicsMan_TranslateView(cPS2GraphicsMan* self, void* pos)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, 0x0(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self->mCurrentView), "r"(pos)
        : "memory");
    self->mMatricesValid = 0;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_RotateView);
#ifdef SKIP_ASM
struct sFontRotMat {
    float m[4][4];
} __attribute__((aligned(16)));

extern "C" void fSinCos(float* s, float* c, float angle);

/**
 * Rotates the top view matrix by @p angle about @p axis: builds the axis-angle rotation matrix (sin and cos from
 * fSinCos) and concatenates it onto the top matrix with VU0, then clears mMatricesValid.
 *
 * On Tour's RotateView(float deg, const t4Vector& axis) takes degrees and the reverse order; here the axis comes
 * first and the angle goes straight to the sincos helper.
 *
 * @param axis  the rotation axis (x, y, z; assumed unit length)
 * @param angle  the rotation angle
 */
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply); the PC port needs a plain matrix multiply.
extern "C" void cPS2GraphicsMan_RotateView(cPS2GraphicsMan* self, float* axis, float angle)
{
    sFontRotMat rot;
    float sinA;
    float cosA;
    char* top = (char*)self->mCurrentView;
    fSinCos(&sinA, &cosA, angle);
    float oneMinusCos = 1.0f - cosA;
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    float tx = oneMinusCos * x;
    float ty = oneMinusCos * y;
    float tz = oneMinusCos * z;
    float sx = sinA * x;
    float sy = sinA * y;
    float sz = sinA * z;
    rot.m[0][0] = tx * x + cosA;
    rot.m[1][0] = tx * y + sz;
    rot.m[2][0] = tx * z - sy;
    rot.m[0][1] = ty * x - sz;
    rot.m[1][1] = ty * y + cosA;
    rot.m[2][1] = ty * z + sx;
    rot.m[0][2] = tz * x + sy;
    rot.m[1][2] = tz * y - sx;
    rot.m[2][2] = tz * z + cosA;
    rot.m[3][0] = 0.0f;
    rot.m[3][1] = 0.0f;
    rot.m[3][2] = 0.0f;
    rot.m[3][3] = 1.0f;
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
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
        : "r"(top), "r"(&rot)
        : "memory");
    self->mMatricesValid = 0;
}
#endif

extern "C" void* func_003826E0(void* self);

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_CalcSortZ);
#ifdef SKIP_ASM
/**
 * Returns the integer depth-sort key of a point.
 *
 * Recomputes the matrices first when mMatricesValid is 0 (func_0037D968). With VU0 it transforms @p vec by mNetMatrix,
 * multiplies the result's w by unk_5920.w, clamps it to 0..unk_5920.x and converts it to an integer.
 *
 * @param vec  the point (t4Vector)
 * @return the clamped, scaled w as an integer
 */
// PORT: PS2-only VU0 inline asm (matrix * vector, clamp, clip flags read back from vi1).
extern "C" int cPS2GraphicsMan_CalcSortZ(cPS2GraphicsMan* self, void* vec)
{
    long sortZ;
    if (self->mMatricesValid == 0) {
        func_0037D968(self);
    }
    __asm__ __volatile__(
        "lqc2      $vf5, 0x5920(%1)\n"
        "lqc2      $vf10, 0x0(%2)\n"
        "lqc2      $vf1, 0x0(%3)\n"
        "lqc2      $vf2, 0x10(%3)\n"
        "lqc2      $vf3, 0x20(%3)\n"
        "lqc2      $vf4, 0x30(%3)\n"
        "vmulax.xyzw ACC, $vf1, $vf10x\n"
        "vmadday.xyzw ACC, $vf2, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf10z\n"
        "vmaddw.xyzw $vf11, $vf4, $vf10w\n"
        "vmulw.w    $vf11, $vf11, $vf5w\n"
        "vmaxx.w    $vf11, $vf11, $vf0x\n"
        "vminix.w   $vf11, $vf11, $vf5x\n"
        "vftoi0.xyzw $vf11, $vf11\n"
        "vmtir      $vi1, $vf11w\n"
        "cfc2.ni    %0, $vi1\n"
        : "=&r"(sortZ)
        : "r"(((void*)self)), "r"(vec), "r"((char*)&self->mNetMatrix)
        : "memory");
    return (int)sortZ;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_IsRenderReady__FPv);
#ifdef SKIP_ASM
/**
 * Reports whether the renderer can accept a new frame.
 *
 * Returns func_003826E0().
 *
 * @return non-zero when a frame can be drawn
 */
void* cPS2GraphicsMan_IsRenderReady(void* self)
{
    return func_003826E0(self);
}
#endif

extern "C" void* func_00382730(void* self);

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_IsRenderIdle__FPv);
#ifdef SKIP_ASM
/**
 * Reports whether the renderer has finished all queued work.
 *
 * Returns func_00382730(); cGraphicsMan_FlushRenderer() polls it.
 *
 * @return non-zero when idle
 */
void* cPS2GraphicsMan_IsRenderIdle(void* self)
{
    return func_00382730(self);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetLightMats__FPv);
#ifdef SKIP_ASM
/**
 * Sets the lighting matrices: empty in the PS2 renderer.
 */
void cPS2GraphicsMan_SetLightMats(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_projOutCode);
#ifdef SKIP_ASM
/**
 * Returns the clip out-code of a projected point against the bounds unk_6B70 (min) and unk_6B80 (max).
 *
 * Bits: 2 x below min.x, 1 x above max.x, 8 y below min.y, 4 y above max.y. If w is negative it returns those bits XOR
 * 0x2F at once; otherwise it adds 0x20 when z is above unk_6B70.z and 0x10 when z is below unk_6B80.z.
 *
 * @param vec  the point (x, y, z, w)
 * @return the out-code bits
 */
extern "C" int cPS2GraphicsMan_projOutCode(cPS2GraphicsMan* self, char* vec) {
    float x = *(float*)(vec + 0);
    int flags = 0;
    if (x < self->unk_6B70.x) {
        flags = 2;
    }
    if (self->unk_6B80.x < x) {
        flags |= 1;
    }
    float y = *(float*)(vec + 4);
    if (y < self->unk_6B70.y) {
        flags |= 8;
    }
    if (self->unk_6B80.y < y) {
        flags |= 4;
    }
    if (*(float*)(vec + 0xC) < 0.0f) {
        return flags ^ 0x2F;
    }
    float z = *(float*)(vec + 8);
    if (self->unk_6B70.z < z) {
        flags |= 0x20;
    }
    if (z < self->unk_6B80.z) {
        flags |= 0x10;
    }
    return flags;
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetCurrentField);
#ifdef SKIP_ASM
/**
 * Returns the current video field: bit 13 (FIELD) of the GS CSR register.
 *
 * @return 0 or 1
 */
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int cPS2GraphicsMan_GetCurrentField(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 13) & 1);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_WaitVSync);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
#define GS_CSR (*(volatile unsigned long*)0x12001000)
/**
 * Waits for the next vertical sync and returns the current field.
 *
 * Writes 8 to the GS CSR (clears the VSINT flag), spins until bit 3 is set again, then returns CSR bit 13.
 *
 * @return the field bit (0 or 1)
 */
extern "C" int cPS2GraphicsMan_WaitVSync(void)
{
    GS_CSR = 8;
    if (!(GS_CSR & 8)) {
        do {
        } while (!(GS_CSR & 8));
    }
    return (int)((GS_CSR >> 13) & 1);
}
#endif
//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_PageFlip);
#ifdef SKIP_ASM
extern "C" void func_00382AF0(void* self, int arg1, int arg2);

/**
 * Flips the frame: calls func_00382AF0(@p self, @p arg1, 1).
 *
 * @param arg1  passed through as func_00382AF0's second argument (preview: a bool)
 */
extern "C" void cPS2GraphicsMan_PageFlip(cPS2GraphicsMan* self, int arg1) {
    func_00382AF0(self, arg1, 1);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GridMesh_IsOk__FPv);
#ifdef SKIP_ASM
/**
 * Returns the grid mesh's word at +0x1C8.
 *
 * Slot 2 of the grid-mesh vtable cPS2GridMesh_vtbl.
 *
 * @return the word at +0x1C8
 */
int cPS2GridMesh_IsOk(void* self)
{
    return *(int*)((char*)self + 0x1C8);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GridMesh_SetScale);
#ifdef SKIP_ASM
/**
 * Sets the grid mesh's range: +0x194 = @p range and +0x198 = 32767 / @p range.
 *
 * Slot 9 of the grid-mesh vtable cPS2GridMesh_vtbl, overriding the empty cGridMesh_SetScale.
 *
 * @param range  the scale / range value
 */
extern "C" void cPS2GridMesh_SetScale(cPS2GridMesh* self, float range)
{
    self->unk_194 = range;
    self->unk_198 = 32767.0f / range;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396128);
#ifdef SKIP_ASM
/**
 * Resets three cached ids of the texture manager to -1: the words at +0x1F4C, +0x1F50 and +0x1F54.
 *
 * The first two in a two-pass loop. func_00363490 calls it on mpTextureMan before building the frame's chains.
 */
extern "C" void func_00396128(void* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        ((int*)((char*)self + 0x1f4c))[i] = -1;
    }
    *(int*)((char*)self + 0x1f54) = -1;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396160);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body: stores the cVisualEffect base vtable D_00488680 and frees the object when bit 0 of @p flags is set.
 *
 * Same body as the visual-effect destructors after it; nothing references this copy (no vtable, no call).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void func_00396160(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396190);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing references it (no call, no vtable entry).
 */
extern "C" void func_00396190(void) {

}
#endif
//100%
INCLUDE_ASM("render/font", func_00396198);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing references it (no call, no vtable entry).
 */
extern "C" void func_00396198(void) {

}
#endif
//100%
INCLUDE_ASM("render/font", func_003961A0);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Nothing references it (no call, no vtable entry).
 */
extern "C" void func_003961A0(void) {

}
#endif

//100%
INCLUDE_ASM("render/font", cRenderToggles__cRenderToggles);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "RenderToggles": stores the cVisualEffect base vtable D_00488680
 * and frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cRenderToggles_vtbl (slot 2 is cRenderToggles_update), which cPS2VisualEffectsMan_cPS2VisualEffectsMan installs.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cRenderToggles__cRenderToggles(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cRenderToggles_AIUpdatesPerRender);
#ifdef SKIP_ASM
extern int D_004A44C0;

/**
 * Returns &D_004A44C0, the int that item 0 of cRenderTogglesMenu_cRenderTogglesMenu edits as "AI Frames per Render
 * Frame" (0 to 60).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44C0
 */
extern "C" int *cRenderToggles_AIUpdatesPerRender(void) {
    return &D_004A44C0;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisplayRenderStats);
#ifdef SKIP_ASM
extern int D_004A44C4;

/**
 * Returns &D_004A44C4, the int that item 1 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Display Render Stats" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44C4
 */
extern "C" int *cRenderToggles_DisplayRenderStats(void) {
    return &D_004A44C4;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_ResetStats);
#ifdef SKIP_ASM
extern int D_004A44C8;

/**
 * Returns &D_004A44C8, the int that item 2 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Reset Stats" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44C8
 */
extern "C" int *cRenderToggles_ResetStats(void) {
    return &D_004A44C8;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_UseSmallTexture0);
#ifdef SKIP_ASM
extern int D_004A44CC;

/**
 * Returns &D_004A44CC, the int that item 3 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Use Small Texture 0" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44CC
 */
extern "C" int *cRenderToggles_UseSmallTexture0(void) {
    return &D_004A44CC;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_UseSmallTexture1);
#ifdef SKIP_ASM
extern int D_004A44D0;

/**
 * Returns &D_004A44D0, the int that item 4 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Use Small Texture 1" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44D0
 */
extern "C" int *cRenderToggles_UseSmallTexture1(void) {
    return &D_004A44D0;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisablePatches);
#ifdef SKIP_ASM
extern int D_004A44D4;

/**
 * Returns &D_004A44D4, the int that item 5 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Patches" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44D4
 */
extern "C" int *cRenderToggles_DisablePatches(void) {
    return &D_004A44D4;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisablePatchReflection);
#ifdef SKIP_ASM
extern int D_004A44D8;

/**
 * Returns &D_004A44D8, the int that item 6 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Patch Reflection"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44D8
 */
extern "C" int *cRenderToggles_DisablePatchReflection(void) {
    return &D_004A44D8;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisablePatchShadow);
#ifdef SKIP_ASM
extern int D_004A44DC;

/**
 * Returns &D_004A44DC, the int that item 7 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Patch Shadows" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44DC
 */
extern "C" int *cRenderToggles_DisablePatchShadow(void) {
    return &D_004A44DC;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableSparkles);
#ifdef SKIP_ASM
extern int D_004A44E0;

/**
 * Returns &D_004A44E0, the int that item 8 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Sparkles" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44E0
 */
extern "C" int *cRenderToggles_DisableSparkles(void) {
    return &D_004A44E0;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableLineStrips);
#ifdef SKIP_ASM
extern int D_004A44E4;

/**
 * Returns &D_004A44E4, the int that item 9 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Line Strips" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44E4
 */
extern "C" int *cRenderToggles_DisableLineStrips(void) {
    return &D_004A44E4;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableTriStrips);
#ifdef SKIP_ASM
extern int D_004A44E8;

/**
 * Returns &D_004A44E8, the int that item 10 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Tri Strips" (0
 * to 3).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44E8
 */
extern "C" int *cRenderToggles_DisableTriStrips(void) {
    return &D_004A44E8;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableTransformStrips);
#ifdef SKIP_ASM
extern int D_004A44EC;

/**
 * Returns &D_004A44EC, the int that item 11 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Transform
 * Strips" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44EC
 */
extern "C" int *cRenderToggles_DisableTransformStrips(void) {
    return &D_004A44EC;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableMeshes);
#ifdef SKIP_ASM
extern int D_004A44F0;

/**
 * Returns &D_004A44F0, the int that item 12 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Meshes" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44F0
 */
extern "C" int *cRenderToggles_DisableMeshes(void) {
    return &D_004A44F0;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableMeshReflection);
#ifdef SKIP_ASM
extern int D_004A44F4;

/**
 * Returns &D_004A44F4, the int that item 13 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Mesh Reflection"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44F4
 */
extern "C" int *cRenderToggles_DisableMeshReflection(void) {
    return &D_004A44F4;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableStaticEmitters);
#ifdef SKIP_ASM
extern int D_004A44F8;

/**
 * Returns &D_004A44F8, the int that item 14 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Static Emitters"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44F8
 */
extern "C" int *cRenderToggles_DisableStaticEmitters(void) {
    return &D_004A44F8;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableDynamicEmitters);
#ifdef SKIP_ASM
extern int D_004A44FC;

/**
 * Returns &D_004A44FC, the int that item 15 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Dynamic
 * Emitters" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44FC
 */
extern "C" int *cRenderToggles_DisableDynamicEmitters(void) {
    return &D_004A44FC;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableDynamicColourEmitters);
#ifdef SKIP_ASM
extern int D_004A4500;

/**
 * Returns &D_004A4500, the int that item 16 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Dynamic Colour
 * Emitters" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4500
 */
extern "C" int *cRenderToggles_DisableDynamicColourEmitters(void) {
    return &D_004A4500;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableSprites);
#ifdef SKIP_ASM
extern int D_004A4504;

/**
 * Returns &D_004A4504, the int that item 17 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Sprites" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4504
 */
extern "C" int *cRenderToggles_DisableSprites(void) {
    return &D_004A4504;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_Disable3DSprites);
#ifdef SKIP_ASM
extern int D_004A4508;

/**
 * Returns &D_004A4508, the int that item 18 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable 3D Sprites" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4508
 */
extern "C" int *cRenderToggles_Disable3DSprites(void) {
    return &D_004A4508;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableTextureRects);
#ifdef SKIP_ASM
extern int D_004A450C;

/**
 * Returns &D_004A450C, the int that item 19 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Texture Rects"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A450C
 */
extern "C" int *cRenderToggles_DisableTextureRects(void) {
    return &D_004A450C;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisablePlayerStrips);
#ifdef SKIP_ASM
extern int D_004A4510;

/**
 * Returns &D_004A4510, the int that item 20 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Player Strips"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4510
 */
extern "C" int *cRenderToggles_DisablePlayerStrips(void) {
    return &D_004A4510;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableShadows);
#ifdef SKIP_ASM
extern int D_004A4514;

/**
 * Returns &D_004A4514, the int that item 21 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Shadows" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4514
 */
extern "C" int *cRenderToggles_DisableShadows(void) {
    return &D_004A4514;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableBoardTrail);
#ifdef SKIP_ASM
extern int D_004A4518;

/**
 * Returns &D_004A4518, the int that item 22 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Board Trail" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4518
 */
extern "C" int *cRenderToggles_DisableBoardTrail(void) {
    return &D_004A4518;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableAIBoardTrail);
#ifdef SKIP_ASM
extern int D_004A451C;

/**
 * Returns &D_004A451C, the int that item 23 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable AI Board Trail"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A451C
 */
extern "C" int *cRenderToggles_DisableAIBoardTrail(void) {
    return &D_004A451C;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableVisibilityRects);
#ifdef SKIP_ASM
extern int D_004A4520;

/**
 * Returns &D_004A4520, the int that item 24 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable Visibility
 * Rects" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4520
 */
extern "C" int *cRenderToggles_DisableVisibilityRects(void) {
    return &D_004A4520;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableSnowFall);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableSnowFall;

/**
 * Returns &cRenderToggles_m_DisableSnowFall, the int that item 25 of cRenderTogglesMenu_cRenderTogglesMenu edits as
 * "Disable Snowfall" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableSnowFall
 */
extern "C" int *cRenderToggles_DisableSnowFall(void) {
    return &cRenderToggles_m_DisableSnowFall;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableBoardWake);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableBoardWake;

/**
 * Returns &cRenderToggles_m_DisableBoardWake, the int that item 26 of cRenderTogglesMenu_cRenderTogglesMenu edits as
 * "Disable Board Wake" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableBoardWake
 */
extern "C" int *cRenderToggles_DisableBoardWake(void) {
    return &cRenderToggles_m_DisableBoardWake;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableSkyBox);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableSkyBox;

/**
 * Returns &cRenderToggles_m_DisableSkyBox, the int that item 27 of cRenderTogglesMenu_cRenderTogglesMenu edits as
 * "Disable Sky Box" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableSkyBox
 */
extern "C" int *cRenderToggles_DisableSkyBox(void) {
    return &cRenderToggles_m_DisableSkyBox;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableCharacterSpecular);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableCharacterSpecular;

/**
 * Returns &cRenderToggles_m_DisableCharacterSpecular, the int that item 28 of cRenderTogglesMenu_cRenderTogglesMenu
 * edits as "Disable Character Specular" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableCharacterSpecular
 */
extern "C" int *cRenderToggles_DisableCharacterSpecular(void) {
    return &cRenderToggles_m_DisableCharacterSpecular;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableAIVisualFX);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableAIVisualFX;

/**
 * Returns &cRenderToggles_m_DisableAIVisualFX, the int that item 29 of cRenderTogglesMenu_cRenderTogglesMenu edits as
 * "Disable AI Visual FX" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableAIVisualFX
 */
extern "C" int *cRenderToggles_DisableAIVisualFX(void) {
    return &cRenderToggles_m_DisableAIVisualFX;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableBoundingBoxes);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableBoundingBoxes;

/**
 * Returns &cRenderToggles_m_DisableBoundingBoxes, the int that item 30 of cRenderTogglesMenu_cRenderTogglesMenu edits
 * as "Disable Bounding Boxes" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableBoundingBoxes
 */
extern "C" int *cRenderToggles_DisableBoundingBoxes(void) {
    return &cRenderToggles_m_DisableBoundingBoxes;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_UseReflectionVector);
#ifdef SKIP_ASM
extern int cRenderToggles_m_UseReflectionVector;

/**
 * Returns &cRenderToggles_m_UseReflectionVector, the int that item 31 of cRenderTogglesMenu_cRenderTogglesMenu edits as
 * "Use Reflection Vector for Sphere Map" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_UseReflectionVector
 */
extern "C" int *cRenderToggles_UseReflectionVector(void) {
    return &cRenderToggles_m_UseReflectionVector;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DisableHUD);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableHUD;

/**
 * Returns &cRenderToggles_m_DisableHUD, the int that item 32 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Disable
 * HUD elements" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cRenderToggles_m_DisableHUD
 */
extern "C" int *cRenderToggles_DisableHUD(void) {
    return &cRenderToggles_m_DisableHUD;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_TestTunnel);
#ifdef SKIP_ASM
extern int D_004A4544;

/**
 * Returns &D_004A4544, the int that item 33 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Test Tunnel" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4544
 */
extern "C" int *cRenderToggles_TestTunnel(void) {
    return &D_004A4544;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_ParticleSizeScale);
#ifdef SKIP_ASM
extern int D_004A4548;

/**
 * Returns &D_004A4548, the float that item 34 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Particle Size Scale"
 * (1 to 2000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4548
 */
extern "C" int *cRenderToggles_ParticleSizeScale(void) {
    return &D_004A4548;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_TestSurfaceTypes);
#ifdef SKIP_ASM
extern int D_004A454C;

/**
 * Returns &D_004A454C, the int that item 35 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Test Surface Types" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A454C
 */
extern "C" int *cRenderToggles_TestSurfaceTypes(void) {
    return &D_004A454C;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_SoftwareDynamicParticleSystems);
#ifdef SKIP_ASM
extern int D_004A4550;

/**
 * Returns &D_004A4550, the int that item 36 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Software Based Dynamic
 * Systems" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4550
 */
extern "C" int *cRenderToggles_SoftwareDynamicParticleSystems(void) {
    return &D_004A4550;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_TempInt);
#ifdef SKIP_ASM
extern int D_004A4554;

/**
 * Returns &D_004A4554, the int that item 37 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Temp Int" (0 to 20).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4554
 */
extern "C" int *cRenderToggles_TempInt(void) {
    return &D_004A4554;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_Prog2x);
#ifdef SKIP_ASM
extern int D_004A4558;

/**
 * Returns &D_004A4558, the float that item 38 of cRenderTogglesMenu_cRenderTogglesMenu edits as "PROG2 x" (-10 to 10).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4558
 */
extern "C" int *cRenderToggles_Prog2x(void) {
    return &D_004A4558;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_Prog2y);
#ifdef SKIP_ASM
extern int D_004A455C;

/**
 * Returns &D_004A455C, the float that item 39 of cRenderTogglesMenu_cRenderTogglesMenu edits as "PROG2 y" (-10 to 10).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A455C
 */
extern "C" int *cRenderToggles_Prog2y(void) {
    return &D_004A455C;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_Prog2z);
#ifdef SKIP_ASM
extern int D_004A4560;

/**
 * Returns &D_004A4560, the float that item 40 of cRenderTogglesMenu_cRenderTogglesMenu edits as "PROG2 z" (-10 to 10).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4560
 */
extern "C" int *cRenderToggles_Prog2z(void) {
    return &D_004A4560;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_Prog2w);
#ifdef SKIP_ASM
extern int D_004A4564;

/**
 * Returns &D_004A4564, the float that item 41 of cRenderTogglesMenu_cRenderTogglesMenu edits as "PROG2 w" (-10 to 10).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4564
 */
extern "C" int *cRenderToggles_Prog2w(void) {
    return &D_004A4564;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DebugInfo);
#ifdef SKIP_ASM
extern int D_004A4568;

/**
 * Returns &D_004A4568, the int that item 42 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Enable Build Info" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4568
 */
extern "C" int *cRenderToggles_DebugInfo(void) {
    return &D_004A4568;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DebugNISMemInfo);
#ifdef SKIP_ASM
extern int D_004A456C;

/**
 * Returns &D_004A456C, the int that item 43 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Enable NIS Mem Usage
 * Info" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A456C
 */
extern "C" int *cRenderToggles_DebugNISMemInfo(void) {
    return &D_004A456C;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_DebugGameModeInfo);
#ifdef SKIP_ASM
extern int D_004A4570;

/**
 * Returns &D_004A4570, the int that item 44 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Enable Gameflow Info" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4570
 */
extern "C" int *cRenderToggles_DebugGameModeInfo(void) {
    return &D_004A4570;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_ProfileIndividualFX);
#ifdef SKIP_ASM
extern int D_004A4574;

/**
 * Returns &D_004A4574, the int that item 45 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Profile Individual
 * Effects" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4574
 */
extern "C" int *cRenderToggles_ProfileIndividualFX(void) {
    return &D_004A4574;
}
#endif
//100%
INCLUDE_ASM("render/font", cRenderToggles_ShowSurfaceTypes);
#ifdef SKIP_ASM
extern int D_004A4578;

/**
 * Returns &D_004A4578, the int that item 46 of cRenderTogglesMenu_cRenderTogglesMenu edits as "Show Surface Types" (0
 * to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4578
 */
extern "C" int *cRenderToggles_ShowSurfaceTypes(void) {
    return &D_004A4578;
}
#endif

//100%
INCLUDE_ASM("render/font", cPlants__cPlants);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "Plants": stores the cVisualEffect base vtable D_00488680 and
 * frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cPlants_vtbl (slot 2 is cPlants_update), which cPS2VisualEffectsMan_cPS2VisualEffectsMan installs in
 * the object it allocates as "Plants".
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cPlants__cPlants(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cPlants_Enable);
#ifdef SKIP_ASM
extern int cPlants_m_Enable;

/**
 * Returns &cPlants_m_Enable, the int that item 0 of cPlantsMenu_cPlantsMenu edits as "Enable" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cPlants_m_Enable
 */
extern "C" int *cPlants_Enable(void) {
    return &cPlants_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_SwayAmplitude);
#ifdef SKIP_ASM
extern int D_004A4448;

/**
 * Returns &D_004A4448, the float that item 1 of cPlantsMenu_cPlantsMenu edits as "Sway Amplitude" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4448
 */
extern "C" int *cPlants_SwayAmplitude(void) {
    return &D_004A4448;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_SwayOffset);
#ifdef SKIP_ASM
extern int D_004A444C;

/**
 * Returns &D_004A444C, the float that item 2 of cPlantsMenu_cPlantsMenu edits as "Sway Offset" (-5 to 5).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A444C
 */
extern "C" int *cPlants_SwayOffset(void) {
    return &D_004A444C;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_BranchAmplitude);
#ifdef SKIP_ASM
extern int D_004A4450;

/**
 * Returns &D_004A4450, the float that item 3 of cPlantsMenu_cPlantsMenu edits as "Branch Amplitude" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4450
 */
extern "C" int *cPlants_BranchAmplitude(void) {
    return &D_004A4450;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_RotationAmplitude);
#ifdef SKIP_ASM
extern int D_004A4454;

/**
 * Returns &D_004A4454, the float that item 4 of cPlantsMenu_cPlantsMenu edits as "Rotation Amplitude" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4454
 */
extern "C" int *cPlants_RotationAmplitude(void) {
    return &D_004A4454;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_PositionScale);
#ifdef SKIP_ASM
extern int D_004A4458;

/**
 * Returns &D_004A4458, the float that item 5 of cPlantsMenu_cPlantsMenu edits as "Position Scale" (0 to 1000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4458
 */
extern "C" int *cPlants_PositionScale(void) {
    return &D_004A4458;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_SwayFrequency);
#ifdef SKIP_ASM
extern int D_004A445C;

/**
 * Returns &D_004A445C, the int that item 6 of cPlantsMenu_cPlantsMenu edits as "Sway Frequency (x/600)Hz" (0 to 6000).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A445C
 */
extern "C" int *cPlants_SwayFrequency(void) {
    return &D_004A445C;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_RotationFrequency);
#ifdef SKIP_ASM
extern int D_004A4460;

/**
 * Returns &D_004A4460, the float that item 7 of cPlantsMenu_cPlantsMenu edits as "Rotation Frequency" (0 to 6000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4460
 */
extern "C" int *cPlants_RotationFrequency(void) {
    return &D_004A4460;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_BranchFrequency);
#ifdef SKIP_ASM
extern int D_004A4464;

/**
 * Returns &D_004A4464, the float that item 8 of cPlantsMenu_cPlantsMenu edits as "Branch Frequency (x/600)Hz" (0 to
 * 6000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4464
 */
extern "C" int *cPlants_BranchFrequency(void) {
    return &D_004A4464;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_BranchFrequency2);
#ifdef SKIP_ASM
extern int D_004A4468;

/**
 * Returns &D_004A4468, the float that item 9 of cPlantsMenu_cPlantsMenu edits as "Branch Frequency 2 (x/600)Hz" (0 to
 * 6000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4468
 */
extern "C" int *cPlants_BranchFrequency2(void) {
    return &D_004A4468;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_SwayDirection);
#ifdef SKIP_ASM
extern int D_004A446C;

/**
 * Returns &D_004A446C, the float that item 10 of cPlantsMenu_cPlantsMenu edits as "Sway Direction" (0 to 360).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A446C
 */
extern "C" int *cPlants_SwayDirection(void) {
    return &D_004A446C;
}
#endif
//100%
INCLUDE_ASM("render/font", cPlants_BranchDirection);
#ifdef SKIP_ASM
extern int D_004A4470;

/**
 * Returns &D_004A4470, the float that item 11 of cPlantsMenu_cPlantsMenu edits as "Branch Direction" (0 to 360).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4470
 */
extern "C" int *cPlants_BranchDirection(void) {
    return &D_004A4470;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965B8);
#ifdef SKIP_ASM
/**
 * Returns @p object unchanged.
 *
 * An 8-byte accessor between the cPlants and cBaseClass_DynamicEmitter accessor runs, with no preview counterpart and
 * no caller.
 *
 * @param object  an object pointer
 * @return @p object
 */
extern "C" int func_003965B8(int object) {
    return object;
}
#endif
//100%
INCLUDE_ASM("render/font", cStaticEmitter_getBoundBox);
#ifdef SKIP_ASM
/**
 * Returns @p emitter + 0x160: in cStaticEmitter's layout, the address of BoundBox.
 *
 * No caller.
 *
 * @param emitter  the emitter (cStaticEmitter)
 * @return the address of the bound box
 */
extern "C" int cStaticEmitter_getBoundBox(int emitter) {
    return emitter + 352;
}
#endif
//100%
INCLUDE_ASM("render/font", cStaticEmitter_getPosition);
#ifdef SKIP_ASM
/**
 * Returns @p emitter + 0xC0: in cStaticEmitter's layout, the address of Emitter.X0 (the emitter position).
 *
 * No caller. cStaticEmitter's render(bbox, pos) overload stores its position there.
 *
 * @param emitter  the emitter (cStaticEmitter)
 * @return the address of the position
 */
extern "C" int cStaticEmitter_getPosition(int emitter) {
    return emitter + 0xC0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965D0);
#ifdef SKIP_ASM
/**
 * Copies the 16-byte vector at @p position to @p emitter + 0xC0 (in cStaticEmitter's layout, Emitter.X0).
 *
 * One lq/sq pair: long long is 128-bit on ee-gcc, so the copy is 16 bytes. No caller.
 *
 * @param emitter  the emitter (cStaticEmitter)
 * @param position  the new position
 */
// PORT: `long long` is 128-bit on the EE (one lq/sq copy); off-PS2 the same C copies only 8 bytes, so use a
// 16-byte type.
extern "C" void func_003965D0(void *emitter, long long *position) {
    (*(long long *)((char*)(emitter) + (0xC0))) = (long long) *position;
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getBaseEmitter);
#ifdef SKIP_ASM
/**
 * Returns the address of the emitter's particle system (m_emitter, +0x20).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @return &m_emitter
 */
extern "C" int cBaseClass_DynamicEmitter_getBaseEmitter(int emitter) {
    return emitter + 0x20;
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_setSeed);
#ifdef SKIP_ASM
/**
 * Stores @p fparg0 as the particle system's position seed (m_emitter.SeedX0, +0x58).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @param fparg0  the seed
 */
extern "C" void cBaseClass_DynamicEmitter_setSeed(cBaseClass_DynamicEmitter* emitter, float fparg0) {
    (emitter->m_emitter.SeedX0) = fparg0;
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_numSubEmitters);
#ifdef SKIP_ASM
/**
 * Returns the number of sub-emitters (m_numSubEmitters, +0x178).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @return m_numSubEmitters
 */
extern "C" int cBaseClass_DynamicEmitter_numSubEmitters(cBaseClass_DynamicEmitter* emitter) {
    return (emitter->m_numSubEmitters);
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_subEmitterId);
#ifdef SKIP_ASM
/**
 * Returns the current sub-emitter ring index (m_subEmitterId, +0x17C).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @return m_subEmitterId
 */
extern "C" int cBaseClass_DynamicEmitter_subEmitterId(cBaseClass_DynamicEmitter* emitter) {
    return (emitter->m_subEmitterId);
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_enabled);
#ifdef SKIP_ASM
/**
 * Returns the enabled flag (m_enabled, +0x174).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @return m_enabled
 */
extern "C" int cBaseClass_DynamicEmitter_enabled(cBaseClass_DynamicEmitter* emitter) {
    return (emitter->m_enabled);
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_setEnabled);
#ifdef SKIP_ASM
/**
 * Sets the enabled flag (m_enabled, +0x174).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @param enable  nonzero to enable
 */
extern "C" void cBaseClass_DynamicEmitter_setEnabled(cBaseClass_DynamicEmitter* emitter, int enable) {
    (emitter->m_enabled) = enable;
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getEnabledCount);
#ifdef SKIP_ASM
/**
 * Returns the number of enabled sub-emitters (m_enabledCount, +0x1E0).
 *
 * No caller.
 *
 * @param emitter  the dynamic emitter
 * @return m_enabledCount
 */
extern "C" int cBaseClass_DynamicEmitter_getEnabledCount(cBaseClass_DynamicEmitter* emitter) {
    return (emitter->m_enabledCount);
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getSubEmitterPosition);
#ifdef SKIP_ASM
struct sFontQuad_6618 {
    int v[4];
} __attribute__((aligned(16)));

/**
 * Returns a copy of sub-emitter @p idx's position (m_positions entry, 16 bytes).
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller.
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @return the position
 */
extern "C" sFontQuad_6618 cBaseClass_DynamicEmitter_getSubEmitterPosition(cBaseClass_DynamicEmitter* self, int idx)
{
    int next = self->m_subEmitterId + 1;
    return ((sFontQuad_6618*)self->m_positions)[(idx + next) % self->m_numSubEmitters];
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getSubEmitterVelocity);
#ifdef SKIP_ASM
struct sFontQuad_6658 {
    int v[4];
} __attribute__((aligned(16)));

/**
 * Returns a copy of sub-emitter @p idx's velocity (m_velocities entry, 16 bytes).
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller.
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @return the velocity
 */
extern "C" sFontQuad_6658 cBaseClass_DynamicEmitter_getSubEmitterVelocity(cBaseClass_DynamicEmitter* self, int idx)
{
    int next = self->m_subEmitterId + 1;
    return ((sFontQuad_6658*)self->m_velocities)[(idx + next) % self->m_numSubEmitters];
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getSubEmitterPositionVelocity);
#ifdef SKIP_ASM
/**
 * Points @p outA and @p outB at sub-emitter @p idx's position and velocity entries.
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller.
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @param outA  receives &m_positions[slot]
 * @param outB  receives &m_velocities[slot]
 */
extern "C" void cBaseClass_DynamicEmitter_getSubEmitterPositionVelocity(cBaseClass_DynamicEmitter* self, int idx, int** outA, int** outB)
{
    int next = self->m_subEmitterId + 1;
    int off = ((idx + next) % self->m_numSubEmitters) * 0x10;
    *outA = (int*)((char*)self->m_positions + off);
    *outB = (int*)((char*)self->m_velocities + off);
}
#endif
//100%
INCLUDE_ASM("render/font", cBaseClass_DynamicEmitter_getSubEmitterPositionVelocity1);
#ifdef SKIP_ASM
struct sFontQuad_66D8 {
    int v[4];
} __attribute__((aligned(16)));

/**
 * Copies sub-emitter @p idx's position and velocity into @p outA and @p outB.
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller.
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @param outA  receives the position
 * @param outB  receives the velocity
 */
extern "C" void cBaseClass_DynamicEmitter_getSubEmitterPositionVelocity1(cBaseClass_DynamicEmitter* self, int idx, sFontQuad_66D8* outA, sFontQuad_66D8* outB)
{
    int next = self->m_subEmitterId + 1;
    int i = (idx + next) % self->m_numSubEmitters;
    *outA = ((sFontQuad_66D8*)self->m_positions)[i];
    *outB = ((sFontQuad_66D8*)self->m_velocities)[i];
}
#endif
//100%
INCLUDE_ASM("render/font", cDynamicColourEmitter_getSubEmitterPositionVelocityColour);
#ifdef SKIP_ASM
/**
 * Points @p outA, @p outB and @p outC at sub-emitter @p idx's position, velocity and colour entries.
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller. The colours are the
 * 4-byte m_colours entries (+0x204).
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @param outA  receives &m_positions[slot]
 * @param outB  receives &m_velocities[slot]
 * @param outC  receives &m_colours[slot]
 */
extern "C" void cDynamicColourEmitter_getSubEmitterPositionVelocityColour(void* self, int idx, int** outA, int** outB, int** outC)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    int i = (idx + next) % *(int*)((char*)self + 0x178);
    int off = i * 0x10;
    *outA = (int*)(*(char**)((char*)self + 0x1A0) + off);
    *outB = (int*)(*(char**)((char*)self + 0x1A4) + off);
    *outC = (int*)(*(char**)((char*)self + 0x204) + i * 4);
}
#endif
//100%
INCLUDE_ASM("render/font", cDynamicColourEmitter_getSubEmitterPositionVelocityColour1);
#ifdef SKIP_ASM
struct sFontQ4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFontRGBA {
    char r, g, b, a;
};

/**
 * Copies sub-emitter @p idx's position, velocity and colour into @p out0, @p out1 and @p out2.
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller. The colour is the
 * 4-byte m_colours entry (+0x204).
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @param out0  receives the position
 * @param out1  receives the velocity
 * @param out2  receives the packed colour
 */
extern "C" void cDynamicColourEmitter_getSubEmitterPositionVelocityColour1(char* self, int idx, sFontQ4* out0, sFontQ4* out1, sFontRGBA* out2)
{
    int i = (idx + 1 + *(int*)(self + 0x17C)) % *(int*)(self + 0x178);
    *out0 = (*(sFontQ4**)(self + 0x1A0))[i];
    *out1 = (*(sFontQ4**)(self + 0x1A4))[i];
    *out2 = (*(sFontRGBA**)(self + 0x204))[i];
}
#endif
//100%
INCLUDE_ASM("render/font", cDynamicColourEmitter_getSubEmitterColour);
#ifdef SKIP_ASM
struct sFontP_67D8 {
    int v;
} __attribute__((packed));

struct sFontWord_67D8 {
    sFontP_67D8 p;
    sFontWord_67D8() {}
    sFontWord_67D8(const sFontWord_67D8& o) : p(o.p) {}
};

/**
 * Returns sub-emitter @p idx's packed colour (the 4-byte m_colours entry, +0x204).
 *
 * The ring slot is (@p idx + m_subEmitterId + 1) % m_numSubEmitters, so @p idx 0 is the oldest entry. The preview's
 * version asserted (unsigned)id < (unsigned)m_numSubEmitters; this one does not check. No caller.
 *
 * @param idx  the sub-emitter, counted from the oldest
 * @return the colour
 */
extern "C" sFontWord_67D8 cDynamicColourEmitter_getSubEmitterColour(void* self, int idx)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    return (*(sFontWord_67D8**)((char*)self + 0x204))[(idx + next) % *(int*)((char*)self + 0x178)];
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_cTube);
#ifdef SKIP_ASM
extern char cTube_vtbl[];

/**
 * Stores vtable cTube_vtbl in @p self and returns it.
 *
 * No caller: cPS2VisualEffectsMan_cPS2VisualEffectsMan writes the same vtable inline into the object it allocates as
 * "Tube".
 *
 * @return @p self
 */
extern "C" void* cTube_cTube(void** self)
{
    *self = cTube_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", cTube__cTube);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "Tube": stores the cVisualEffect base vtable D_00488680 and frees
 * the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cTube_vtbl (slot 2 cTube_update, slot 3 the empty cTube_render).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cTube__cTube(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cTube_Enable);
#ifdef SKIP_ASM
extern int D_004A41D4;

/**
 * Returns &D_004A41D4, the int that item 0 of cTubeMenu_cTubeMenu edits as "Enable" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A41D4
 */
extern "C" int *cTube_Enable(void) {
    return &D_004A41D4;
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_ScaleWidth);
#ifdef SKIP_ASM
extern int D_004A41D8;

/**
 * Returns &D_004A41D8, the float that item 1 of cTubeMenu_cTubeMenu edits as "Scale Width" (0 to 1000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A41D8
 */
extern "C" int *cTube_ScaleWidth(void) {
    return &D_004A41D8;
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_ScaleHeight);
#ifdef SKIP_ASM
extern int D_004A41DC;

/**
 * Returns &D_004A41DC, the float that item 2 of cTubeMenu_cTubeMenu edits as "Scale Height" (0 to 1000).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A41DC
 */
extern "C" int *cTube_ScaleHeight(void) {
    return &D_004A41DC;
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_R);
#ifdef SKIP_ASM
extern int D_004A41E0;

/**
 * Returns &D_004A41E0, the int that item 3 of cTubeMenu_cTubeMenu edits as "R" (0 to 255).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A41E0
 */
extern "C" int *cTube_R(void) {
    return &D_004A41E0;
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_G);
#ifdef SKIP_ASM
extern int D_004A41E4;

/**
 * Returns &D_004A41E4, the int that item 4 of cTubeMenu_cTubeMenu edits as "G" (0 to 255).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A41E4
 */
extern "C" int *cTube_G(void) {
    return &D_004A41E4;
}
#endif
//100%
INCLUDE_ASM("render/font", cTube_B);
#ifdef SKIP_ASM
extern int D_004A41E8;

/**
 * Returns &D_004A41E8, the int that item 5 of cTubeMenu_cTubeMenu edits as "B" (0 to 255).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A41E8
 */
extern "C" int *cTube_B(void) {
    return &D_004A41E8;
}
#endif

//100%
INCLUDE_ASM("render/font", cTube_render__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Slot 3 of vtable cTube_vtbl (the "Tube" effect); cSimpleTube_vtbl overrides that slot with cSimpleTube_render, which draws the
 * tube.
 */
void cTube_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_003968D0);
#ifdef SKIP_ASM
/**
 * Copies the 16-byte vector at @p src into entry @p index of the array at +0x8.
 *
 * Slot 5 of vtable cSimpleTube_vtbl (the object cSimpleTube_cSimpleTube builds; its destructor cSimpleTube__cSimpleTube frees the same +0x8
 * array). No bounds check.
 *
 * @param index  the entry
 * @param src  the vector to store
 */
extern "C" void func_003968D0(void* self, int index, void* src)
{
    cQuad128* quads = *(cQuad128**)((char*)self + 0x8);
    quads[index] = *(cQuad128*)src;
}
#endif

//100%
INCLUDE_ASM("render/font", cPathArrow__cPathArrow);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "PathArrow": stores the cVisualEffect base vtable D_00488680 and
 * frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cPathArrow_vtbl, which cPathArrow_cPathArrow installs (slots 2 and 3 are the empty cPathArrow_update and
 * cPathArrow_render).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cPathArrow__cPathArrow(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cPathArrow_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Slot 2 of vtable cPathArrow_vtbl (the "PathArrow" effect).
 */
void cPathArrow_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cPathArrow_render__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Slot 3 of vtable cPathArrow_vtbl (the "PathArrow" effect).
 */
void cPathArrow_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cDetailSystem__cDetailSystem);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "DetailSystem": stores the cVisualEffect base vtable D_00488680
 * and frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cDetailSystem_vtbl, which cDetailSystem_cDetailSystem installs (slot 2 cDetailSystem_update, slot 5 the empty cDetailSystem_addPatch).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cDetailSystem__cDetailSystem(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cDetailSystem_addPatch__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Slot 5 of vtable cDetailSystem_vtbl (the "DetailSystem" effect).
 */
void cDetailSystem_addPatch(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", cDetailSystem_Enable);
#ifdef SKIP_ASM
extern int D_004A4340;

/**
 * Returns &D_004A4340, the int that item 0 of cDetailSystemMenu_cDetailSystemMenu edits as "Enable" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4340
 */
extern "C" int *cDetailSystem_Enable(void) {
    return &D_004A4340;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_SpinScale);
#ifdef SKIP_ASM
extern int D_004A4344;

/**
 * Returns &D_004A4344, the float that item 1 of cDetailSystemMenu_cDetailSystemMenu edits as "Sparkle Spin Scale" (-1
 * to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4344
 */
extern "C" int *cDetailSystem_SpinScale(void) {
    return &D_004A4344;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_PS2SparkleCount);
#ifdef SKIP_ASM
extern int D_004A4348;

/**
 * Returns &D_004A4348, the float that item 2 of cDetailSystemMenu_cDetailSystemMenu edits as "PS2 Sparkle Count" (0 to
 * 20).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4348
 */
extern "C" int *cDetailSystem_PS2SparkleCount(void) {
    return &D_004A4348;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_PS2SparkleSize);
#ifdef SKIP_ASM
extern int D_004A434C;

/**
 * Returns &D_004A434C, the float that item 3 of cDetailSystemMenu_cDetailSystemMenu edits as "PS2 Sparkle World Size"
 * (0 to 500).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A434C
 */
extern "C" int *cDetailSystem_PS2SparkleSize(void) {
    return &D_004A434C;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_PS2MinSparkleSize);
#ifdef SKIP_ASM
extern int D_004A4350;

/**
 * Returns &D_004A4350, the float that item 4 of cDetailSystemMenu_cDetailSystemMenu edits as "PS2 Min Sparkle Screen
 * Size" (0 to 100).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4350
 */
extern "C" int *cDetailSystem_PS2MinSparkleSize(void) {
    return &D_004A4350;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_PS2MaxSparkleSize);
#ifdef SKIP_ASM
extern int D_004A4354;

/**
 * Returns &D_004A4354, the float that item 5 of cDetailSystemMenu_cDetailSystemMenu edits as "PS2 Max Sparkle Screen
 * Size" (0 to 100).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4354
 */
extern "C" int *cDetailSystem_PS2MaxSparkleSize(void) {
    return &D_004A4354;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_XBoxNumSparkles);
#ifdef SKIP_ASM
extern int D_004A4358;

/**
 * Returns &D_004A4358, the float that item 6 of cDetailSystemMenu_cDetailSystemMenu edits as "XBox Sparkle Count" (1 to
 * 300).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4358
 */
extern "C" int *cDetailSystem_XBoxNumSparkles(void) {
    return &D_004A4358;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_XBoxSize);
#ifdef SKIP_ASM
extern int D_004A435C;

/**
 * Returns &D_004A435C, the float that item 7 of cDetailSystemMenu_cDetailSystemMenu edits as "XBox - World Size" (0 to
 * 500).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A435C
 */
extern "C" int *cDetailSystem_XBoxSize(void) {
    return &D_004A435C;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_XBoxMinSize);
#ifdef SKIP_ASM
extern int D_004A4360;

/**
 * Returns &D_004A4360, the float that item 8 of cDetailSystemMenu_cDetailSystemMenu edits as "XBox - Min Screen Size"
 * (0 to 100).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4360
 */
extern "C" int *cDetailSystem_XBoxMinSize(void) {
    return &D_004A4360;
}
#endif
//100%
INCLUDE_ASM("render/font", cDetailSystem_XBoxMaxSize);
#ifdef SKIP_ASM
extern int D_004A4364;

/**
 * Returns &D_004A4364, the float that item 9 of cDetailSystemMenu_cDetailSystemMenu edits as "XBox - Max Screen Size"
 * (0 to 100).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4364
 */
extern "C" int *cDetailSystem_XBoxMaxSize(void) {
    return &D_004A4364;
}
#endif

//100%
INCLUDE_ASM("render/font", cVisualEffectsTest__cVisualEffectsTest);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destroys the visual-effects test object: restores the base vtable and frees it if asked.
 *
 * Stores the cVisualEffect base vtable (D_00488680; the derived vtable store is dead and dropped) and calls
 * operator_delete on @p flags bit 0. Slot 1 of vt_00492FD0, the cVisualEffectsTest vtable whose slots 2/3 are
 * cVisualEffectsTest_update and cVisualEffectsTest_render.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVisualEffectsTest__cVisualEffectsTest(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_HUDEnable);
#ifdef SKIP_ASM
extern int D_004A4478;

/**
 * Returns &D_004A4478, the int that item 0 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Show All HUD
 * Graphics" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4478
 */
extern "C" int *cVisualEffectsTest_HUDEnable(void) {
    return &D_004A4478;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_ShowCurrentElement);
#ifdef SKIP_ASM
extern int D_004A447C;

/**
 * Returns &D_004A447C, the int that item 1 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Show Current
 * Element" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A447C
 */
extern "C" int *cVisualEffectsTest_ShowCurrentElement(void) {
    return &D_004A447C;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_HUDPrint);
#ifdef SKIP_ASM
extern int D_004A4480;

/**
 * Returns &D_004A4480, the int that item 2 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Output HUD Data"
 * (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4480
 */
extern "C" int *cVisualEffectsTest_HUDPrint(void) {
    return &D_004A4480;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_CurrElement);
#ifdef SKIP_ASM
extern int D_004A4484;

/**
 * Returns &D_004A4484, the int that item 3 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Current Element
 * Id" (0 to 1000).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4484
 */
extern "C" int *cVisualEffectsTest_CurrElement(void) {
    return &D_004A4484;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_PositionX);
#ifdef SKIP_ASM
extern int D_004A4488;

/**
 * Returns &D_004A4488, the int that item 4 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Position X" (-640
 * to 640).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4488
 */
extern "C" int *cVisualEffectsTest_PositionX(void) {
    return &D_004A4488;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_PositionY);
#ifdef SKIP_ASM
extern int D_004A448C;

/**
 * Returns &D_004A448C, the int that item 5 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Position Y" (-480
 * to 480).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A448C
 */
extern "C" int *cVisualEffectsTest_PositionY(void) {
    return &D_004A448C;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_SizeX);
#ifdef SKIP_ASM
extern int D_004A4490;

/**
 * Returns &D_004A4490, the int that item 6 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Size X" (-640 to
 * 640).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4490
 */
extern "C" int *cVisualEffectsTest_SizeX(void) {
    return &D_004A4490;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_SizeY);
#ifdef SKIP_ASM
extern int D_004A4494;

/**
 * Returns &D_004A4494, the int that item 7 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Size Y" (-640 to
 * 640).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4494
 */
extern "C" int *cVisualEffectsTest_SizeY(void) {
    return &D_004A4494;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_ScaleX);
#ifdef SKIP_ASM
extern int D_004A4498;

/**
 * Returns &D_004A4498, the int that item 8 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Scale X" (-1 to
 * 1000).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4498
 */
extern "C" int *cVisualEffectsTest_ScaleX(void) {
    return &D_004A4498;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_ScaleY);
#ifdef SKIP_ASM
extern int D_004A449C;

/**
 * Returns &D_004A449C, the int that item 9 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Scale Y" (-1 to
 * 1000).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A449C
 */
extern "C" int *cVisualEffectsTest_ScaleY(void) {
    return &D_004A449C;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_Alpha);
#ifdef SKIP_ASM
extern int D_004A44A0;

/**
 * Returns &D_004A44A0, the int that item 10 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Alpha" (-255 to
 * 0x).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44A0
 */
extern "C" int *cVisualEffectsTest_Alpha(void) {
    return &D_004A44A0;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_Red);
#ifdef SKIP_ASM
extern int D_004A44A4;

/**
 * Returns &D_004A44A4, the int that item 11 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Red" (-255 to
 * 0x).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44A4
 */
extern "C" int *cVisualEffectsTest_Red(void) {
    return &D_004A44A4;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_Green);
#ifdef SKIP_ASM
extern int D_004A44A8;

/**
 * Returns &D_004A44A8, the int that item 12 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Green" (-255 to
 * 0x).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44A8
 */
extern "C" int *cVisualEffectsTest_Green(void) {
    return &D_004A44A8;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_Blue);
#ifdef SKIP_ASM
extern int D_004A44AC;

/**
 * Returns &D_004A44AC, the int that item 13 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Blue" (-255 to
 * 0x).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44AC
 */
extern "C" int *cVisualEffectsTest_Blue(void) {
    return &D_004A44AC;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_LayerOffset);
#ifdef SKIP_ASM
extern int D_004A44B0;

/**
 * Returns &D_004A44B0, the int that item 14 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as "Layer Offset"
 * (-16 to 16).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A44B0
 */
extern "C" int *cVisualEffectsTest_LayerOffset(void) {
    return &D_004A44B0;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_Enable);
#ifdef SKIP_ASM
extern int cVisualEffectsTest_m_Enable;

/**
 * Returns &cVisualEffectsTest_m_Enable, the int that item 15 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as
 * "World Painter Display Enable" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cVisualEffectsTest_m_Enable
 */
extern "C" int *cVisualEffectsTest_Enable(void) {
    return &cVisualEffectsTest_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", cVisualEffectsTest_LayerId);
#ifdef SKIP_ASM
extern int cVisualEffectsTest_m_LayerId;

/**
 * Returns &cVisualEffectsTest_m_LayerId, the int that item 16 of cVisualEffectsTestMenu_cVisualEffectsTestMenu edits as
 * "World Painter LayerId" (0 to 100).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cVisualEffectsTest_m_LayerId
 */
extern "C" int *cVisualEffectsTest_LayerId(void) {
    return &cVisualEffectsTest_m_LayerId;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_InitCharMatrices);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only GS packet build; uncached (0x30000000) pointers held in int and 128-bit qword copies.
typedef int cQuad128_6B40 __attribute__((mode(TI)));
extern char D_1150[];

struct sRS_6B40 {
    int f0;
    int f4;
    int f8;
    int fC;
};

/**
 * Starts a character-model batch: sends the lighting setup to VU1.
 *
 * Sets the top render state's mode bits to 6 (0x180) and points it at the current packet address (+0x5A00), then writes
 * a 12-qword DMA packet: a GIF/VIF header, the 10 qwords of mIrradianceInfo (+0x6BB0) and a VIF call of the
 * microprogram at D_1150. cPS2GraphicsMan_DrawCharModel calls it through slot 124 with a scale of 1.0, which this code
 * ignores.
 */
extern "C" void cPS2GraphicsMan_InitCharMatrices(cPS2GraphicsMan* self)
{
    sRS_6B40* rs = (sRS_6B40*)self->m_pCurrentDrawState;
    unsigned int addrMask = 0x3FFFFFFF;
    rs->f0 = (rs->f0 & ~0x3C0) | 0x180;
    sRS_6B40* rs2 = (sRS_6B40*)self->m_pCurrentDrawState;
    rs2->fC = (rs2->fC & 0xC0000000) | (self->mVifCursor & addrMask);
    unsigned long* packet = (unsigned long*)(self->mVifCursor | 0x30000000);
    packet[0] = 0x6000000C;
    packet[2] = (unsigned long)0x8000 << 45;
    packet[3] = 0x6C0A000701000101;
    packet[1] = 0;
    packet += 4;
    cQuad128_6B40* src = (cQuad128_6B40*)(((char*)self) + 0x6BB0);
    for (int i = 0; i < 10; i++) {
        ((cQuad128_6B40*)packet)[i] = src[i];
    }
    packet[20] = 0;
    packet[21] = (unsigned long)(((unsigned int)D_1150 >> 3) | 0x14000000) << 32;
    packet += 22;
    self->mVifCursor = (unsigned int)packet & 0x0FFFFFFF;
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_SetBlendMatrices);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char D_80[];
extern char D_70[];

struct sMtx_6C30 {
    float m[16];
};

struct sMtxList_6C30 {
    int count;
    int* index;
};

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMtx_6C30(void* dst, const void* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

/**
 * Appends a VIF packet that uploads selected 4x4 matrices to the packet at *@p cur, and advances *@p cur.
 *
 * Header (4 doublewords): DMA cnt tag 0x10000002 | count << 2 (4 * count + 2 qwords), then the VIF codes NOP, FLUSHE,
 * STCYCL 0x01000101 and UNPACK V4-32 0x6C000011 | count << 18 (4 * count qwords to VU address 0x11). Then matrix @p
 * mats[@p list->index[i]] for each i, copied with VU0 (lqc2/sqc2). Tail: UNPACK S-32 0x600103FF with the count as its
 * data (to VU address 0x3FF), then two MSCAL codes (0x14000000 | addr >> 3) for the microprogram addresses D_80 and
 * D_70. @p self is unused. No caller (particle.cpp carries an inline copy).
 *
 * @param list  the matrix count and the index array
 * @param mats  the matrix array
 * @param cur  the packet write pointer, advanced past the packet
 */
extern "C" void cPS2GraphicsMan_SetBlendMatrices(cPS2GraphicsMan* self, sMtxList_6C30* list, sMtx_6C30* mats, unsigned long** cur)
{
    int n = list->count;
    int* idx = list->index;
    unsigned long* packet = *cur;
    packet[0] = (n << 2) | 0x10000002;
    packet[2] = (unsigned long)0x8000 << 45;
    packet[3] = ((unsigned long)(unsigned int)((n << 18) | 0x6C000011) << 32) | 0x1000101;
    packet[1] = 0;
    packet += 4;
    for (int i = 0; i < n; i++) {
        vu0CopyMtx_6C30(packet, &mats[idx[i]]);
        packet += 8;
    }
    packet[0] = ((unsigned long)n << 32) | 0x600103FF;
    packet[1] = (unsigned long)(((unsigned int)D_80 >> 3) | 0x14000000) | ((unsigned long)(((unsigned int)D_70 >> 3) | 0x14000000) << 32);
    packet += 2;
    *cur = packet;
}
#endif

//100%
INCLUDE_ASM("render/font", cScreenTint__cScreenTint);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

/**
 * Destructor body for the visual effect allocated as "ScreenTint": stores the cVisualEffect base vtable D_00488680 and
 * frees the object when bit 0 of @p flags is set.
 *
 * Slot 1 of vtable cScreenTint_vtbl (slot 2 cScreenTint_update, slot 3 cScreenTint_render), which
 * cPS2VisualEffectsMan_cPS2VisualEffectsMan installs.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cScreenTint__cScreenTint(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

extern "C" void* __sti__all_in_one_render_cpp(int, int);

//100%
INCLUDE_ASM("render/font", cScreenTint_Enable);
#ifdef SKIP_ASM
extern int cScreenTint_m_Enable;

/**
 * Returns &cScreenTint_m_Enable, the int that item 0 of cScreenTintMenu_cScreenTintMenu edits as "Enable" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cScreenTint_m_Enable
 */
extern "C" int *cScreenTint_Enable(void) {
    return &cScreenTint_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OverrideWP);
#ifdef SKIP_ASM
extern int cScreenTint_m_OverrideWP;

/**
 * Returns &cScreenTint_m_OverrideWP, the int that item 1 of cScreenTintMenu_cScreenTintMenu edits as "Override World
 * Painter" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cScreenTint_m_OverrideWP
 */
extern "C" int *cScreenTint_OverrideWP(void) {
    return &cScreenTint_m_OverrideWP;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_DisplayValues);
#ifdef SKIP_ASM
extern int cScreenTint_m_DisplayValues;

/**
 * Returns &cScreenTint_m_DisplayValues, the int that item 2 of cScreenTintMenu_cScreenTintMenu edits as "Display
 * Values" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cScreenTint_m_DisplayValues
 */
extern "C" int *cScreenTint_DisplayValues(void) {
    return &cScreenTint_m_DisplayValues;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_ColourR);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourR;

/**
 * Returns &cScreenTint_m_ColourR, the float that item 3 of cScreenTintMenu_cScreenTintMenu edits as "Tint Red" (0 to
 * 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_ColourR
 */
extern "C" int *cScreenTint_ColourR(void) {
    return &cScreenTint_m_ColourR;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_ColourG);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourG;

/**
 * Returns &cScreenTint_m_ColourG, the float that item 4 of cScreenTintMenu_cScreenTintMenu edits as "Tint Green" (0 to
 * 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_ColourG
 */
extern "C" int *cScreenTint_ColourG(void) {
    return &cScreenTint_m_ColourG;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_ColourB);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourB;

/**
 * Returns &cScreenTint_m_ColourB, the float that item 5 of cScreenTintMenu_cScreenTintMenu edits as "Tint Blue" (0 to
 * 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_ColourB
 */
extern "C" int *cScreenTint_ColourB(void) {
    return &cScreenTint_m_ColourB;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_FillR);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillR;

/**
 * Returns &cScreenTint_m_FillR, the float that item 6 of cScreenTintMenu_cScreenTintMenu edits as "Fill Red" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_FillR
 */
extern "C" int *cScreenTint_FillR(void) {
    return &cScreenTint_m_FillR;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_FillG);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillG;

/**
 * Returns &cScreenTint_m_FillG, the float that item 7 of cScreenTintMenu_cScreenTintMenu edits as "Fill Green" (0 to
 * 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_FillG
 */
extern "C" int *cScreenTint_FillG(void) {
    return &cScreenTint_m_FillG;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_FillB);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillB;

/**
 * Returns &cScreenTint_m_FillB, the float that item 8 of cScreenTintMenu_cScreenTintMenu edits as "Fill Blue" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_FillB
 */
extern "C" int *cScreenTint_FillB(void) {
    return &cScreenTint_m_FillB;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_XNColourR);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourR;

/**
 * Returns &cScreenTint_m_XNColourR, the float that item 9 of cScreenTintMenu_cScreenTintMenu edits as "XN Tint Red" (0
 * to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_XNColourR
 */
extern "C" int *cScreenTint_XNColourR(void) {
    return &cScreenTint_m_XNColourR;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_XNColourG);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourG;

/**
 * Returns &cScreenTint_m_XNColourG, the float that item 10 of cScreenTintMenu_cScreenTintMenu edits as "XN Tint Green"
 * (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_XNColourG
 */
extern "C" int *cScreenTint_XNColourG(void) {
    return &cScreenTint_m_XNColourG;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_XNColourB);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourB;

/**
 * Returns &cScreenTint_m_XNColourB, the float that item 11 of cScreenTintMenu_cScreenTintMenu edits as "XN Tint Blue"
 * (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_XNColourB
 */
extern "C" int *cScreenTint_XNColourB(void) {
    return &cScreenTint_m_XNColourB;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_XNLerp);
#ifdef SKIP_ASM
extern int D_004A4608;

/**
 * Returns &D_004A4608, the float that item 12 of cScreenTintMenu_cScreenTintMenu edits as "XN Lerp" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4608
 */
extern "C" int *cScreenTint_XNLerp(void) {
    return &D_004A4608;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningEnable);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningEnable;

/**
 * Returns &cScreenTint_m_LightningEnable, the int that item 13 of cScreenTintMenu_cScreenTintMenu edits as "Enable
 * Lightning" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &cScreenTint_m_LightningEnable
 */
extern "C" int *cScreenTint_LightningEnable(void) {
    return &cScreenTint_m_LightningEnable;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_FrameId);
#ifdef SKIP_ASM
extern int D_004A4610;

/**
 * Returns &D_004A4610, the int that item 14 of cScreenTintMenu_cScreenTintMenu edits as "Frame Id" (-1 to 1000).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4610
 */
extern "C" int *cScreenTint_FrameId(void) {
    return &D_004A4610;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningChance);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningChance;

/**
 * Returns &cScreenTint_m_LightningChance, the float that item 15 of cScreenTintMenu_cScreenTintMenu edits as "Lightning
 * Chance" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &cScreenTint_m_LightningChance
 */
extern "C" int *cScreenTint_LightningChance(void) {
    return &cScreenTint_m_LightningChance;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningFadeIn);
#ifdef SKIP_ASM
extern int D_004A4618;

/**
 * Returns &D_004A4618, the int that item 16 of cScreenTintMenu_cScreenTintMenu edits as "Flash Fade In" (0 to 600).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4618
 */
extern "C" int *cScreenTint_LightningFadeIn(void) {
    return &D_004A4618;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningFadeOut);
#ifdef SKIP_ASM
extern int D_004A461C;

/**
 * Returns &D_004A461C, the int that item 17 of cScreenTintMenu_cScreenTintMenu edits as "Flash Sustain" (0 to 600).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A461C
 */
extern "C" int *cScreenTint_LightningFadeOut(void) {
    return &D_004A461C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningFadeIn2);
#ifdef SKIP_ASM
extern int D_004A4620;

/**
 * Returns &D_004A4620, the int that item 18 of cScreenTintMenu_cScreenTintMenu edits as "Inverse Duration" (0 to 600).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4620
 */
extern "C" int *cScreenTint_LightningFadeIn2(void) {
    return &D_004A4620;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningFadeIn3);
#ifdef SKIP_ASM
extern int D_004A4624;

/**
 * Returns &D_004A4624, the int that item 19 of cScreenTintMenu_cScreenTintMenu edits as "Recover Fade In" (0 to 600).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4624
 */
extern "C" int *cScreenTint_LightningFadeIn3(void) {
    return &D_004A4624;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningScale);
#ifdef SKIP_ASM
extern int D_004A4628;

/**
 * Returns &D_004A4628, the float that item 20 of cScreenTintMenu_cScreenTintMenu edits as "Flash Scale" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4628
 */
extern "C" int *cScreenTint_LightningScale(void) {
    return &D_004A4628;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningScale2);
#ifdef SKIP_ASM
extern int D_004A462C;

/**
 * Returns &D_004A462C, the float that item 21 of cScreenTintMenu_cScreenTintMenu edits as "Inverse Scale" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A462C
 */
extern "C" int *cScreenTint_LightningScale2(void) {
    return &D_004A462C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningScale3);
#ifdef SKIP_ASM
extern int D_004A4630;

/**
 * Returns &D_004A4630, the float that item 22 of cScreenTintMenu_cScreenTintMenu edits as "Recover Scale" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4630
 */
extern "C" int *cScreenTint_LightningScale3(void) {
    return &D_004A4630;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColourR);
#ifdef SKIP_ASM
extern int D_004A4634;

/**
 * Returns &D_004A4634, the float that item 23 of cScreenTintMenu_cScreenTintMenu edits as "Flash Tint Red" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4634
 */
extern "C" int *cScreenTint_LightningColourR(void) {
    return &D_004A4634;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColourG);
#ifdef SKIP_ASM
extern int D_004A4638;

/**
 * Returns &D_004A4638, the float that item 24 of cScreenTintMenu_cScreenTintMenu edits as "Flash Tint Green" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4638
 */
extern "C" int *cScreenTint_LightningColourG(void) {
    return &D_004A4638;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColourB);
#ifdef SKIP_ASM
extern int D_004A463C;

/**
 * Returns &D_004A463C, the float that item 25 of cScreenTintMenu_cScreenTintMenu edits as "Flash Tint Blue" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A463C
 */
extern "C" int *cScreenTint_LightningColourB(void) {
    return &D_004A463C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour2R);
#ifdef SKIP_ASM
extern int D_004A4640;

/**
 * Returns &D_004A4640, the float that item 26 of cScreenTintMenu_cScreenTintMenu edits as "Inverse Tint Red" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4640
 */
extern "C" int *cScreenTint_LightningColour2R(void) {
    return &D_004A4640;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour2G);
#ifdef SKIP_ASM
extern int D_004A4644;

/**
 * Returns &D_004A4644, the float that item 27 of cScreenTintMenu_cScreenTintMenu edits as "Inverse Tint Green" (0 to
 * 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4644
 */
extern "C" int *cScreenTint_LightningColour2G(void) {
    return &D_004A4644;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour2B);
#ifdef SKIP_ASM
extern int D_004A4648;

/**
 * Returns &D_004A4648, the float that item 28 of cScreenTintMenu_cScreenTintMenu edits as "Inverse Tint Blue" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4648
 */
extern "C" int *cScreenTint_LightningColour2B(void) {
    return &D_004A4648;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour3R);
#ifdef SKIP_ASM
extern int D_004A464C;

/**
 * Returns &D_004A464C, the float that item 29 of cScreenTintMenu_cScreenTintMenu edits as "Recover Tint Red" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A464C
 */
extern "C" int *cScreenTint_LightningColour3R(void) {
    return &D_004A464C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour3G);
#ifdef SKIP_ASM
extern int D_004A4650;

/**
 * Returns &D_004A4650, the float that item 30 of cScreenTintMenu_cScreenTintMenu edits as "Recover Tint Green" (0 to
 * 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4650
 */
extern "C" int *cScreenTint_LightningColour3G(void) {
    return &D_004A4650;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningColour3B);
#ifdef SKIP_ASM
extern int D_004A4654;

/**
 * Returns &D_004A4654, the float that item 31 of cScreenTintMenu_cScreenTintMenu edits as "Recover Tint Blue" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4654
 */
extern "C" int *cScreenTint_LightningColour3B(void) {
    return &D_004A4654;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaEnable);
#ifdef SKIP_ASM
extern int D_004A4658;

/**
 * Returns &D_004A4658, the int that item 32 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Enable" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself.
 *
 * @return &D_004A4658
 */
extern "C" int *cScreenTint_GammaEnable(void) {
    return &D_004A4658;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaPower);
#ifdef SKIP_ASM
extern int D_004A465C;

/**
 * Returns &D_004A465C, the float that item 33 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Power" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A465C
 */
extern "C" int *cScreenTint_GammaPower(void) {
    return &D_004A465C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaColourR);
#ifdef SKIP_ASM
extern int D_004A4660;

/**
 * Returns &D_004A4660, the float that item 34 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Tint Red" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4660
 */
extern "C" int *cScreenTint_GammaColourR(void) {
    return &D_004A4660;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaColourG);
#ifdef SKIP_ASM
extern int D_004A4664;

/**
 * Returns &D_004A4664, the float that item 35 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Tint Green" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4664
 */
extern "C" int *cScreenTint_GammaColourG(void) {
    return &D_004A4664;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaColourB);
#ifdef SKIP_ASM
extern int D_004A4668;

/**
 * Returns &D_004A4668, the float that item 36 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Tint Blue" (0 to 2).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4668
 */
extern "C" int *cScreenTint_GammaColourB(void) {
    return &D_004A4668;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaFillR);
#ifdef SKIP_ASM
extern int D_004A466C;

/**
 * Returns &D_004A466C, the float that item 37 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Fill Red" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A466C
 */
extern "C" int *cScreenTint_GammaFillR(void) {
    return &D_004A466C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaFillG);
#ifdef SKIP_ASM
extern int D_004A4670;

/**
 * Returns &D_004A4670, the float that item 38 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Fill Green" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4670
 */
extern "C" int *cScreenTint_GammaFillG(void) {
    return &D_004A4670;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_GammaFillB);
#ifdef SKIP_ASM
extern int D_004A4674;

/**
 * Returns &D_004A4674, the float that item 39 of cScreenTintMenu_cScreenTintMenu edits as "Gamma Fill Blue" (0 to 1).
 *
 * No caller: the menu takes the variable's address itself. The C declares the result int* although the menu edits the
 * value as a float.
 *
 * @return &D_004A4674
 */
extern "C" int *cScreenTint_GammaFillB(void) {
    return &D_004A4674;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_Colour);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern float colR __asm__("cScreenTint_m_ColourR");
extern float colG __asm__("cScreenTint_m_ColourG");
extern float colB __asm__("cScreenTint_m_ColourB");

/**
 * Returns the screen tint colour as a packed 0xAARRGGBB word: alpha 0xFF, each channel (int)(cScreenTint_m_ColourR/G/B
 * * 255).
 *
 * No clamp: the menu lets the colours reach 2.0, which gives channel values up to 510 that spill into the neighbouring
 * byte. No caller.
 *
 * @return the packed colour
 */
extern "C" int cScreenTint_Colour(void)
{
    int red = (int)(colR * 255.0f);
    int green = (int)(colG * 255.0f);
    int blue = (int)(colB * 255.0f);
    return (0xFF << 24) | (red << 16) | (green << 8) | blue;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningRedColour);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A0;

/**
 * Returns D_004A469C * D_004A46A0.
 *
 * cScreenTint_update and cScreenTint_render use these globals; On Tour's cScreenTint statics m_lightningBrightness, m_redColour,
 * m_greenColour, m_blueColour fit this layout. No caller.
 *
 * @return the product (a float)
 */
extern "C" float cScreenTint_LightningRedColour(void) {
    return D_004A469C * D_004A46A0;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningGreenColour);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A4;

/**
 * Returns D_004A469C * D_004A46A4.
 *
 * cScreenTint_update and cScreenTint_render use these globals; On Tour's cScreenTint statics m_lightningBrightness, m_redColour,
 * m_greenColour, m_blueColour fit this layout. No caller.
 *
 * @return the product (a float)
 */
extern "C" float cScreenTint_LightningGreenColour(void) {
    return D_004A469C * D_004A46A4;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_LightningBlueColour);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A8;

/**
 * Returns D_004A469C * D_004A46A8.
 *
 * cScreenTint_update and cScreenTint_render use these globals; On Tour's cScreenTint statics m_lightningBrightness, m_redColour,
 * m_greenColour, m_blueColour fit this layout. No caller.
 *
 * @return the product (a float)
 */
extern "C" float cScreenTint_LightningBlueColour(void) {
    return D_004A469C * D_004A46A8;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_Fill);
#ifdef SKIP_ASM
/**
 * Sets all three fill components (cScreenTint_m_FillR, FillG, FillB) to @p value.
 *
 * No caller.
 *
 * @param value  the fill level
 */
extern "C" void cScreenTint_Fill(float value) {
    *(float*)&cScreenTint_m_FillB = value;
    *(float*)&cScreenTint_m_FillG = value;
    *(float*)&cScreenTint_m_FillR = value;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaEnable);
#ifdef SKIP_ASM
extern int D_004A4678;

/**
 * Returns &D_004A4678.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4678
 */
extern "C" int *cScreenTint_OldGammaEnable(void) {
    return &D_004A4678;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaPower);
#ifdef SKIP_ASM
extern int D_004A467C;

/**
 * Returns &D_004A467C.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A467C
 */
extern "C" int *cScreenTint_OldGammaPower(void) {
    return &D_004A467C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaColourR);
#ifdef SKIP_ASM
extern int D_004A4680;

/**
 * Returns &D_004A4680.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4680
 */
extern "C" int *cScreenTint_OldGammaColourR(void) {
    return &D_004A4680;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaColourG);
#ifdef SKIP_ASM
extern int D_004A4684;

/**
 * Returns &D_004A4684.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4684
 */
extern "C" int *cScreenTint_OldGammaColourG(void) {
    return &D_004A4684;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaColourB);
#ifdef SKIP_ASM
extern int D_004A4688;

/**
 * Returns &D_004A4688.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4688
 */
extern "C" int *cScreenTint_OldGammaColourB(void) {
    return &D_004A4688;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaFillR);
#ifdef SKIP_ASM
extern int D_004A468C;

/**
 * Returns &D_004A468C.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A468C
 */
extern "C" int *cScreenTint_OldGammaFillR(void) {
    return &D_004A468C;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaFillG);
#ifdef SKIP_ASM
extern int D_004A4690;

/**
 * Returns &D_004A4690.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4690
 */
extern "C" int *cScreenTint_OldGammaFillG(void) {
    return &D_004A4690;
}
#endif
//100%
INCLUDE_ASM("render/font", cScreenTint_OldGammaFillB);
#ifdef SKIP_ASM
extern int D_004A4694;

/**
 * Returns &D_004A4694.
 *
 * No caller, and no debug-menu item edits the variable.
 *
 * @return &D_004A4694
 */
extern "C" int *cScreenTint_OldGammaFillB(void) {
    return &D_004A4694;
}
#endif

//99.38%
INCLUDE_ASM("render/font", func_003970F8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the render unit's static initialisers: __sti__all_in_one_render_cpp(1, 0xffff).
 *
 * Listed in the global constructor table D_0043CE38 (count 0x27, then one function per unit); @p self is ignored.
 *
 * @return whatever __sti__all_in_one_render_cpp returns
 */
void* func_003970F8(void* self)
{
    return __sti__all_in_one_render_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__all_in_one_render_cpp(int, int);

//99.38%
INCLUDE_ASM("render/font", func_00397118__FPv);
#ifdef SKIP_ASM
/**
 * Runs the render unit's static destructors: __sti__all_in_one_render_cpp(0, 0xffff).
 *
 * Listed in the global destructor table that starts at D_0043CEE0 (count 0xD); @p self is ignored.
 *
 * @return whatever __sti__all_in_one_render_cpp returns
 */
void* func_00397118(void* self)
{
    return __sti__all_in_one_render_cpp(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("render/font", cUIAnimationBank_setData__FPvi);
#ifdef SKIP_ASM
/**
 * Points an animation bank at its loaded animation block.
 *
 * Stores @p val in the bank's first word (On Tour mpAnimationBlock). cUIEngine_loadFile calls it for the engine's bank
 * at +0x50 with the file's animation block (file + *(file + 8)).
 *
 * @param val  the animation block's address (On Tour: char* pData)
 */
void cUIAnimationBank_setData(void* self, int val)
{
    *(int*)((char*)self + 0x0) = val;
}
#endif

