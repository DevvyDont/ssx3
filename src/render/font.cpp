#include "common.h"
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
    virtual void v50(int);
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
        ((sFontGfx_19E8*)ps2GraphicsMan)->v50(self->TextureID);
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
extern "C" void func_004186C8(sMat00391C48* m);
extern "C" void cFont_text(cFont* self, float x, float y, sMat00391C48* m);

/**
 * Formats text into a shared static buffer (D_00500CA0) and draws it at a position.
 *
 * The C declaration drops the format and va_list arguments (On Tour fmt, va); they reach the vsprintf
 * (func_004186C8) in the untouched argument registers.
 *
 * @param x  screen x
 * @param y  screen y
 */
extern "C" void cFont_vtextf(cFont* self, float x, float y)
{
    func_004186C8(&D_00500CA0);
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
INCLUDE_ASM("render/font", func_00391E30);
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

extern "C" void func_00391E30(cFont* self, float x, float y, void* m)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    (ps2GraphicsMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = fontGetTex1E30(self);
    sFontPos1E30 pos(x + self->Offset[0] * self->Scale[0],
                     y + self->Offset[1] * self->Scale[1], 0.0f, 0.0f);
    if (self->ShadowOffset[0] != 0.0f || self->ShadowOffset[1] != 0.0f) {
        sDrawState* top = ps2GraphicsMan->m_pCurrentDrawState;
        int saved = (unsigned)(((int)top->unk_00[2]) & 0x3E0) >> 5;
        int a = saved - 1;
        if (a < 0) a = 0;
        top->unk_00[2] = (((int)top->unk_00[2]) & ~0x3E0) | ((a << 5) & 0x3E0);
        sFontVEntry1E30* vt = (sFontVEntry1E30*)ps2GraphicsMan->mVtbl;
        vt[81].fn((char*)ps2GraphicsMan + vt[81].delta, self, m, ((char*)self) + 0x50,
                  pos.x + self->ShadowOffset[0], pos.y + self->ShadowOffset[1],
                  self->Scale[0], self->Scale[1], self->UVScale[0], self->UVScale[1], pos.z);
        sDrawState* top2 = ps2GraphicsMan->m_pCurrentDrawState;
        top2->unk_00[2] = (((int)top2->unk_00[2]) & ~0x3E0) | (saved << 5);
    }
    sFontVEntry1E30* vt = (sFontVEntry1E30*)ps2GraphicsMan->mVtbl;
    vt[81].fn((char*)ps2GraphicsMan + vt[81].delta, self, m, ((char*)self) + 0x40, pos.x, pos.y,
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
INCLUDE_ASM("render/font", func_003921F0);
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

// PORT: g++ `<?`/`>?` (min/max) operators, removed in GCC 4.3.
extern "C" float func_003921F0(cFont* f_, const unsigned short* s, void* r_, int mono, float sx, float sy)
{
    cFont* font = f_;
    sRect21F0* r = (sRect21F0*)r_;
    sRect21F0 tmp;
    sRect21F0* rc = r ? r : &tmp;
    rc->x0 = 0.0f;
    rc->y0 = 0.0f;
    rc->x1 = 0.0f;
    rc->y1 = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    for (; *s; s++) {
        tGlyph* glyph = sFontRes21F0_find(font, *s);
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
INCLUDE_ASM("render/font", func_00392430);
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

// Copy as much of str into dst as fits in width; returns where the next line starts (0 when done).
extern "C" char* func_00392430(cFont* f_, const char* str, char* dst, int max, float sx, float width)
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
INCLUDE_ASM("render/font", func_00392680);
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

// Unicode version of func_00392430: copies as much of str into dst as fits in width; returns the next line start (0 when done).
extern "C" unsigned short* func_00392680(cFont* f_, float sx, float width, const unsigned short* str, unsigned short* dst, int max)
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
INCLUDE_ASM("render/font", func_00392908);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(cFont* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_00392908(cFont* self, float x, float y, const char* str)
{
    float w = cFont_textExtent(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text(self, x - w * 0.5f, y, (sMat00391C48*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392980);
#ifdef SKIP_ASM
extern "C" void func_00392980(cFont* self, const unsigned short* str, float x, float y)
{
    float w = func_003921F0(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    func_00391E30(self, x - w * 0.5f, y, (void*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_vcentertextf);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void func_003191C0(void* str, const char* fmt, void* ap);
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
    func_003191C0(&txt, fmt, ap);
    func_00392908(self, x, y, txt.p);
    cBXString__cBXString(&txt, 2);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/font", func_00392A60);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(cFont* self, const char* str, void* out, int n, float sx, float sy);

extern "C" void func_00392A60(cFont* self, const char* str, float x, float y)
{
    float w = cFont_textExtent(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    cFont_text(self, x - w, y, (sMat00391C48*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392AD0);
#ifdef SKIP_ASM
extern "C" void func_00392AD0(cFont* self, const unsigned short* str, float x, float y)
{
    float w = func_003921F0(self, str, 0, 0, self->Scale[0], self->Scale[1]);
    func_00391E30(self, x - w, y, (void*)str);
}
#endif

//100%
INCLUDE_ASM("render/font", cFont_vrighttextf);
#ifdef SKIP_ASM
extern void* cBXString_EmptyString;
extern "C" void func_003191C0(void* str, const char* fmt, void* ap);
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
    func_003191C0(&txt, fmt, ap);
    func_00392A60(self, txt.p, x, y);
    cBXString__cBXString(&txt, 2);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392BA8);
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

extern "C" float func_00392BA8(cFont* font, char c)
{
    tGlyph* glyph = findGlyph((font), c);
    if (glyph)
        return glyph->advance;
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392C60);
#ifdef SKIP_ASM
extern "C" float func_00392C60(cFont* font, unsigned short c)
{
    tGlyph* glyph = findGlyph((font), c);
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
INCLUDE_ASM("render/font", func_00392D58);
#ifdef SKIP_ASM
extern "C" void func_00392D58(cPlayerLights* playerLights, void *colour) {
    (playerLights->mExtraAmbientLight.R) = (float) ((playerLights->mExtraAmbientLight.R) + (*(float *)((char*)(colour) + (0))));
    (playerLights->mExtraAmbientLight.G) = (float) ((playerLights->mExtraAmbientLight.G) + (*(float *)((char*)(colour) + (4))));
    (playerLights->mExtraAmbientLight.B) = (float) ((playerLights->mExtraAmbientLight.B) + (*(float *)((char*)(colour) + (8))));
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
INCLUDE_ASM("render/font", func_00392DE0);
#ifdef SKIP_ASM
extern "C" void func_00392DE0(cPlayerLights* playerLights, int value) {
    (playerLights->mPlayerIsAdditive) = value;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392DE8__FPvi);
#ifdef SKIP_ASM
void func_00392DE8(void* self, int val)
{
    *(int*)((char*)self + 0xA4) = val;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392DF0);
#ifdef SKIP_ASM
extern "C" float func_0031BF60(float a);

extern "C" void* func_00392DF0(void* self)
{
    int i;
    for (i = 0; i < 640; i++) {
        ((float*)self)[i] = func_0031BF60((float)i * 0.01227184571325779f);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00392E60);
#ifdef SKIP_ASM
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
extern "C" void cGraphicsMan_SetScreenOffset(cPS2GraphicsMan* self, int xoffset, int yoffset)
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
extern "C" int cGraphicsMan_GetScreenXOffset(cPS2GraphicsMan* self)
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
extern "C" int cGraphicsMan_GetScreenYOffset(cPS2GraphicsMan* self)
{
    return (int)self->m_screenOffset[1];
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394008__FPv);
#ifdef SKIP_ASM
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
INCLUDE_ASM("render/font", func_00394020);
#ifdef SKIP_ASM
extern "C" void func_00394020(cPS2GraphicsMan* ps2GraphicsMan, int numViews) {
    (ps2GraphicsMan->mNumViews) = numViews;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394028);
#ifdef SKIP_ASM
extern "C" int func_00394028(cPS2GraphicsMan* ps2GraphicsMan) {
    return (ps2GraphicsMan->mNumViews);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394030);
#ifdef SKIP_ASM
extern "C" void func_00394030(cPS2GraphicsMan* ps2GraphicsMan, int viewIndex) {
    (ps2GraphicsMan->mViewIndex) = viewIndex;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394038);
#ifdef SKIP_ASM
extern "C" int func_00394038(cPS2GraphicsMan* ps2GraphicsMan) {
    return (ps2GraphicsMan->mViewIndex);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394040);
#ifdef SKIP_ASM
extern "C" void func_00394040(cPS2GraphicsMan* base, int value, int idx) {
    *(int*)(((char*)base) + (idx << 2) + 0xC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394050);
#ifdef SKIP_ASM
extern "C" int func_00394050(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0xC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394060);
#ifdef SKIP_ASM
extern "C" void func_00394060(cPS2GraphicsMan* base, int value, int idx) {
    *(int*)(((char*)base) + (idx << 2) + 0x2C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394070);
#ifdef SKIP_ASM
extern "C" int func_00394070(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x2C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394080);
#ifdef SKIP_ASM
extern "C" void func_00394080(cPS2GraphicsMan* base, int value, int idx) {
    *(int*)(((char*)base) + (idx << 2) + 0x4C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394090);
#ifdef SKIP_ASM
extern "C" int func_00394090(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x4C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940A0);
#ifdef SKIP_ASM
extern "C" void func_003940A0(cPS2GraphicsMan* base, int value, int idx) {
    *(int*)(((char*)base) + (idx << 2) + 0x6C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940B0);
#ifdef SKIP_ASM
extern "C" int func_003940B0(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x6C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940C0);
#ifdef SKIP_ASM
extern "C" void func_003940C0(cPS2GraphicsMan* base, int value, int idx) {
    *(int*)(((char*)base) + (idx << 2) + 0x8C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940D0);
#ifdef SKIP_ASM
extern "C" int func_003940D0(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x8C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940E0);
#ifdef SKIP_ASM
extern "C" void func_003940E0(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0xAC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003940F0);
#ifdef SKIP_ASM
extern "C" float func_003940F0(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0xAC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394100);
#ifdef SKIP_ASM
extern "C" void func_00394100(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0xCC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394110);
#ifdef SKIP_ASM
extern "C" float func_00394110(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0xCC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394120);
#ifdef SKIP_ASM
extern "C" void func_00394120(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0xEC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394130);
#ifdef SKIP_ASM
extern "C" float func_00394130(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0xEC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394140);
#ifdef SKIP_ASM
extern "C" void func_00394140(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x10C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394150);
#ifdef SKIP_ASM
extern "C" float func_00394150(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x10C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394160);
#ifdef SKIP_ASM
extern "C" void func_00394160(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x12C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394170);
#ifdef SKIP_ASM
extern "C" float func_00394170(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x12C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394180);
#ifdef SKIP_ASM
extern "C" void func_00394180(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x14C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394190);
#ifdef SKIP_ASM
extern "C" float func_00394190(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x14C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941A0);
#ifdef SKIP_ASM
extern "C" void func_003941A0(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x16C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941B0);
#ifdef SKIP_ASM
extern "C" float func_003941B0(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x16C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941C0);
#ifdef SKIP_ASM
extern "C" void func_003941C0(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x18C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941D0);
#ifdef SKIP_ASM
extern "C" float func_003941D0(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x18C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941E0);
#ifdef SKIP_ASM
extern "C" void func_003941E0(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x1AC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003941F0);
#ifdef SKIP_ASM
extern "C" float func_003941F0(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1AC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394200);
#ifdef SKIP_ASM
extern "C" void func_00394200(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x1CC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394210);
#ifdef SKIP_ASM
extern "C" float func_00394210(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1CC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394220);
#ifdef SKIP_ASM
extern "C" void func_00394220(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x1EC) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394230);
#ifdef SKIP_ASM
extern "C" float func_00394230(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x1EC);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394240);
#ifdef SKIP_ASM
extern "C" void func_00394240(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x20C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394250);
#ifdef SKIP_ASM
extern "C" float func_00394250(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x20C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394260);
#ifdef SKIP_ASM
extern "C" void func_00394260(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x22C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394270);
#ifdef SKIP_ASM
extern "C" float func_00394270(cPS2GraphicsMan* base, int idx) {
    return *(float*)(((char*)base) + (idx << 2) + 0x22C);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394280);
#ifdef SKIP_ASM
extern "C" void func_00394280(cPS2GraphicsMan* base, int idx, float value) {
    *(float*)(((char*)base) + (idx << 2) + 0x24C) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394290);
#ifdef SKIP_ASM
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
extern "C" void* cGraphicsMan_GetTexel(void* self)
{
    *(float*)self = 1.0f;
    *(float*)((char*)self + 0x4) = 1.0f;
    *(int*)((char*)self + 0x8) = 0;
    *(float*)((char*)self + 0xc) = 1.0f;
    return self;
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
INCLUDE_ASM("render/font", func_003942D0__FPv);
#ifdef SKIP_ASM
void func_003942D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_003942D8__FPv);
#ifdef SKIP_ASM
void* func_003942D8(void* self)
{
    return (char*)self + 0xED8;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003942E0__FPv);
#ifdef SKIP_ASM
void func_003942E0(void* self)
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
extern "C" void cGraphicsMan_SetDefaultDrawState(cPS2GraphicsMan* self)
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
extern "C" void cGraphicsMan_SetDrawState(cPS2GraphicsMan* self, sDrawState* src)
{
    *(self->m_pCurrentDrawState) = *src;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394358);
#ifdef SKIP_ASM
extern "C" int func_00394358(cPS2GraphicsMan* ps2GraphicsMan) {
    return ((int)ps2GraphicsMan->m_pCurrentDrawState);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394360);
#ifdef SKIP_ASM
extern "C" int func_00394360(cPS2GraphicsMan* ps2GraphicsMan) {
    return ((int)ps2GraphicsMan->m_pCurrentDrawState);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394368);
#ifdef SKIP_ASM
extern "C" void func_00394368(cPS2GraphicsMan* ps2GraphicsMan, int enable) {
    int *drawState = (int*)ps2GraphicsMan->m_pCurrentDrawState;
    int bit = (enable << 22) & 0x400000;
    drawState[1] = (drawState[1] & ~0x400000) | bit;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394398);
#ifdef SKIP_ASM
extern "C" void func_00394398(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    void *temp_6;

    temp_6 = ((void*)ps2GraphicsMan->m_pCurrentDrawState);
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
extern "C" void cGraphicsMan_SetConstantColour(cPS2GraphicsMan* self, sFontV4 colour, int idx)
{
    // PORT: pointer arithmetic via int (index-first addu)
    *(sFontV4*)((char*)((idx << 4) + (int)self) + 0xF04) = colour;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394418);
#ifdef SKIP_ASM
extern "C" sFontV4 func_00394418(cPS2GraphicsMan* self, sFontV4 v, int idx)
{
    // PORT: pointer arithmetic via int (index-first addu)
    return *(sFontV4*)((char*)((idx << 4) + (int)self) + 0xF04);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394470);
#ifdef SKIP_ASM
extern "C" void func_00394470(cPS2GraphicsMan* ps2GraphicsMan, int mode, int context) {
    int temp_2;
    int temp_4;
    void *temp_7;

    temp_7 = ((void*)ps2GraphicsMan->m_pCurrentDrawState);
    temp_2 = (context * 5) + 2;
    temp_4 = 0x1F << temp_2;
    (*(int *)((char*)(temp_7) + (4))) = (int) (((*(int *)((char*)(temp_7) + (4))) & ~temp_4) | ((mode << temp_2) & temp_4));
}
#endif
//100%
INCLUDE_ASM("render/font", func_003944A8);
#ifdef SKIP_ASM
extern "C" void func_003944A8(cPS2GraphicsMan* ps2GraphicsMan, int arg1, int arg2) {
    void *temp_3;
    void *temp_8;

    temp_8 = ((void*)ps2GraphicsMan->m_pCurrentDrawState);
    (*(int *)((char*)(temp_8) + (4))) = (int) (((*(int *)((char*)(temp_8) + (4))) & ~3) | (arg1 & 3));
    temp_3 = ((void*)ps2GraphicsMan->m_pCurrentDrawState);
    (*(int *)((char*)(temp_3) + (8))) = (int) (((*(int *)((char*)(temp_3) + (8))) & 0xE00003FF) | ((arg2 << 0xA) & 0x1FFFFC00));
}
#endif
//100%
INCLUDE_ASM("render/font", func_003944F8);
#ifdef SKIP_ASM
extern "C" void func_003944F8(cPS2GraphicsMan* ps2GraphicsMan, int mode, int context) {
    int *temp_7;
    int temp_4;
    int temp_6;

    temp_7 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    temp_6 = (context * 2) + 2;
    temp_4 = 3 << temp_6;
    *temp_7 = (*temp_7 & ~temp_4) | ((mode << temp_6) & temp_4);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394530);
#ifdef SKIP_ASM
extern "C" void func_00394530(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    int *drawState = (int*)ps2GraphicsMan->m_pCurrentDrawState;
    int masked = value & 0x3FFFFFFF;
    drawState[3] = (drawState[3] & 0xC0000000) | masked;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394558);
#ifdef SKIP_ASM
extern "C" unsigned int func_00394558(cPS2GraphicsMan* ps2GraphicsMan) {
    return (unsigned int) ((*(int *)((char*)((ps2GraphicsMan->m_pCurrentDrawState)) + (8))) & 0x3E0) >> 5;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394570);
#ifdef SKIP_ASM
extern "C" unsigned int func_00394570(cPS2GraphicsMan* ps2GraphicsMan) {
    return (unsigned int) ((*(int *)((char*)((ps2GraphicsMan->m_pCurrentDrawState)) + (4))) & 0x7C) >> 2;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394588);
#ifdef SKIP_ASM
extern "C" int func_00394588(cPS2GraphicsMan* ps2GraphicsMan) {
    return (*(int *)((char*)((ps2GraphicsMan->m_pCurrentDrawState)) + (4))) & 3;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394598);
#ifdef SKIP_ASM
extern "C" unsigned int func_00394598(cPS2GraphicsMan* ps2GraphicsMan) {
    return (unsigned int) ((*(int *)((char*)((ps2GraphicsMan->m_pCurrentDrawState)) + (8))) & 0x1FFFFC00) >> 10;
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
INCLUDE_ASM("render/font", func_003945C8__FPv);
#ifdef SKIP_ASM
void func_003945C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_003945D0__FPv);
#ifdef SKIP_ASM
void func_003945D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_003945D8__FPv);
#ifdef SKIP_ASM
void func_003945D8(void* self)
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
INCLUDE_ASM("render/font", func_003945F0);
#ifdef SKIP_ASM
extern "C" int func_003945F0(cPS2GraphicsMan* ps2GraphicsMan) {
    return (ps2GraphicsMan->mNumTexRefs);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003945F8);
#ifdef SKIP_ASM
extern "C" char* func_003945F8(cPS2GraphicsMan* base, int idx)
{
    return ((char*)base) + 0x280 + idx * 0xC;
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
INCLUDE_ASM("render/font", func_00394618__FPv);
#ifdef SKIP_ASM
void func_00394618(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394620__FPv);
#ifdef SKIP_ASM
void func_00394620(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394628__FPv);
#ifdef SKIP_ASM
void func_00394628(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394630__FPv);
#ifdef SKIP_ASM
void func_00394630(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394638__FPv);
#ifdef SKIP_ASM
void func_00394638(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394640__FPv);
#ifdef SKIP_ASM
void func_00394640(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394648__FPv);
#ifdef SKIP_ASM
void func_00394648(void* self)
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
INCLUDE_ASM("render/font", func_00394668__FPv);
#ifdef SKIP_ASM
void func_00394668(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394670__FPv);
#ifdef SKIP_ASM
void func_00394670(void* self)
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
INCLUDE_ASM("render/font", func_00394680__FPv);
#ifdef SKIP_ASM
void func_00394680(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394688__FPv);
#ifdef SKIP_ASM
void func_00394688(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394690);
#ifdef SKIP_ASM
extern "C" int func_00394690(void) {
    return (*(int*)&cGraphicsMan_TheGraphicsMan);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394698);
#ifdef SKIP_ASM
extern "C" void func_00394698(cPS2GraphicsMan* ps2GraphicsMan, int texId) {
    (ps2GraphicsMan->mSpecTexID) = texId;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946A0);
#ifdef SKIP_ASM
extern "C" int func_003946A0(cPS2GraphicsMan* ps2GraphicsMan) {
    return (ps2GraphicsMan->mSpecTexID);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946A8);
#ifdef SKIP_ASM
extern "C" void func_003946A8(cPS2GraphicsMan* base, int idx, int value) {
    *(int*)(((char*)base) + (idx << 2) + 0x10B0) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946B8);
#ifdef SKIP_ASM
extern "C" int func_003946B8(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x10B0);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946C8);
#ifdef SKIP_ASM
extern "C" void func_003946C8(cPS2GraphicsMan* ps2GraphicsMan, int texId) {
    (ps2GraphicsMan->mPatchSpecTexID) = texId;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946D0);
#ifdef SKIP_ASM
extern "C" int func_003946D0(cPS2GraphicsMan* ps2GraphicsMan) {
    return (ps2GraphicsMan->mPatchSpecTexID);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003946D8);
#ifdef SKIP_ASM
extern "C" void func_003946D8(cPS2GraphicsMan* ps2GraphicsMan) {
    (ps2GraphicsMan->mSkipNextPageFlip) = 1;
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
extern "C" void cGraphicsMan_SetPixelTest(cPS2GraphicsMan* self, int depthtestmode, int atestmode, int aref)
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
extern "C" void cGraphicsMan_SetTexture(cPS2GraphicsMan* self, short value, int idx) {
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
extern "C" void cGraphicsMan_PushDrawState(cPS2GraphicsMan* self)
{
    sDrawState* top = self->m_pCurrentDrawState;
    top[1] = top[0];
    self->m_pCurrentDrawState = (sDrawState*)(self->m_pCurrentDrawState + 1);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003947C8);
#ifdef SKIP_ASM
extern "C" void func_003947C8(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0xE84))) = (int) ((*(int *)((char*)(graphicsMan) + (0xE84))) - 20);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003947D8);
#ifdef SKIP_ASM
typedef char* (*sFontGetFn_47D8)(char*);

static inline short* sFontRS_shorts_47D8(char* st)
{
    return (short*)(st + 0x10);
}

extern "C" int func_003947D8(cPS2GraphicsMan* self)
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
INCLUDE_ASM("render/font", func_003948B0);
#ifdef SKIP_ASM
extern "C" void func_003948B0(cPS2GraphicsMan* base, int idx, int value) {
    *(int*)(((char*)base) + (idx << 2) + 0xF50) = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003948C0);
#ifdef SKIP_ASM
extern "C" int func_003948C0(cPS2GraphicsMan* base, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0xF50);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003948D0);
#ifdef SKIP_ASM
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
extern "C" float func_00394AD8(float a, float b)
{
    float factor;
    if (a < 5000.0f) return 1.0f;
    if (!(b <= 3750.0f)) factor = 1.0f - (b - 3750.0f) * 0.0008f;
    else factor = 1.0f;
    return factor;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B38);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00493938[];

extern "C" void func_00394B38(int* self, int flags)
{
    *(void**)((char*)self + 0x4) = D_00493938;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B68__FPv);
#ifdef SKIP_ASM
void func_00394B68(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B70);
#ifdef SKIP_ASM
extern int cBezierMan_TheBezierMan;

extern "C" int func_00394B70(void) {
    return cBezierMan_TheBezierMan;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B78__FPv);
#ifdef SKIP_ASM
void func_00394B78(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B80__FPv);
#ifdef SKIP_ASM
void func_00394B80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B88__FPvf);
#ifdef SKIP_ASM
void func_00394B88(void* self, float val)
{
    *(float*)((char*)self + 0x0) = val;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B90__FPv);
#ifdef SKIP_ASM
float func_00394B90(void* self)
{
    return *(float*)((char*)self + 0x0);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394B98);
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

extern "C" void func_00394B98(cPS2BezierMan* self, int a, int b, int c, int d, int e)
{
    ((cFontVirt*)self)->v22(a, b, c, d, e);
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
INCLUDE_ASM("render/font", func_00394C18);
#ifdef SKIP_ASM
extern "C" int func_00394C18(cPS2BezierMan* base, int shift, int idx) {
    return *(int*)(((char*)base) + (idx << 2) + 0x44C) + (shift << 8);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394C30);
#ifdef SKIP_ASM
extern "C" int func_00394C30(cPS2BezierMan* bezierMan) {
    return (*(int *)((char*)(bezierMan) + (0x1A0)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394C38);
#ifdef SKIP_ASM
extern "C" int func_00394C38(cPS2BezierMan* bezierMan) {
    return (*(int *)((char*)(bezierMan) + (0x320)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00394C40);
#ifdef SKIP_ASM
extern int cLightMan_TheLightMan;

extern "C" int func_00394C40(void) {
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
INCLUDE_ASM("render/font", func_00394C88__FPvi);
#ifdef SKIP_ASM
void* func_00394C88(void* self, int index)
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
INCLUDE_ASM("render/font", func_00394D38__FPv);
#ifdef SKIP_ASM
void func_00394D38(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394D40__FPv);
#ifdef SKIP_ASM
void func_00394D40(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394D48__FPv);
#ifdef SKIP_ASM
void func_00394D48(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394D50);
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

extern "C" void* func_00394D50(void* self)
{
    new (self) sFontCache;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00394ED0);
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

extern "C" sGlyphEntry* func_00394ED0(sGlyphCache* self, sGlyphKey* key, unsigned char hash)
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
INCLUDE_ASM("render/font", func_00395000);
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

extern "C" sNode5000* func_00395000(sCache5000* self, int a, int b, unsigned char hash, int c)
{
    sNode5000** pp = &self->buckets[hash];
    while (*pp != 0) {
        sNode5000* e;
        int hit = 0;
        if ((*pp)->busy == 0) {
            int t = (*pp)->k4 == b && (*pp)->k8 == c && (*pp)->k0 == a;
            if (t) hit = 1;
        }
        e = *pp;
        if (hit) {
            if ((*pp) == self->buckets[hash]) {
                return e;
            }
            *pp = e->next;
            e->next = self->buckets[hash];
            self->buckets[hash] = e;
            return e;
        }
        pp = &e->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003950C0);
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

// Compiler-generated member-wise assignment of the font render state.
extern "C" sFontState50C0* func_003950C0(sFontState50C0* self, const sFontState50C0* other)
{
    *self = *other;
    return self;
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
    new ((char*)self + 0x10E0) sRenderSlot5288[12];
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
int cPS2GraphicsMan_GetDTV(void* self)
{
    return *(int*)((char*)self + 0x5A4C);
}
#endif

//100%
INCLUDE_ASM("render/font", cPS2GraphicsMan_GetDisplayMode__FPv);
#ifdef SKIP_ASM
int cPS2GraphicsMan_GetDisplayMode(void* self)
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
    return (char*)self + 0x6B94;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003953B8);
#ifdef SKIP_ASM
extern "C" void func_003956B0(cPS2GraphicsMan* self);

// Fetch the current (combined) matrix into out and return out.
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
extern "C" void* func_003953B8(void* out, void* self)
{
    func_003956B0((cPS2GraphicsMan*)self);
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
INCLUDE_ASM("render/font", func_00395418);
#ifdef SKIP_ASM
extern "C" int func_00395418(int graphicsMan) {
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
INCLUDE_ASM("render/font", func_00395428);
#ifdef SKIP_ASM
extern "C" int func_00395428(cPS2GraphicsMan* pS2GraphicsMan) {
    return (pS2GraphicsMan->mNumProjTextures);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395430);
#ifdef SKIP_ASM
extern "C" char* func_00395430(cPS2GraphicsMan* base, int idx)
{
    return ((char*)base) + 0x13F0 + idx * 160;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395448);
#ifdef SKIP_ASM
extern "C" void func_00395448(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    int *temp_6;

    temp_6 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    *temp_6 = (*temp_6 & ~0x3C0) | ((value << 6) & 0x3C0);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395470);
#ifdef SKIP_ASM
extern "C" void func_00395470(cPS2GraphicsMan* ps2GraphicsMan, int value) {
    int *temp_6;

    temp_6 = ((int*)ps2GraphicsMan->m_pCurrentDrawState);
    *temp_6 = (*temp_6 & ~0xC00) | ((value << 0xA) & 0xC00);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395498);
#ifdef SKIP_ASM
extern "C" void func_00395498(cPS2GraphicsMan* ps2GraphicsMan, int alpha, int context) {
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
    return (char*)self + 0x6BB0;
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
INCLUDE_ASM("render/font", func_00395518);
#ifdef SKIP_ASM
extern "C" int func_00395518(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A84)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395520);
#ifdef SKIP_ASM
extern "C" int func_00395520(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A80)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395528);
#ifdef SKIP_ASM
extern "C" int func_00395528(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A3C)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395530);
#ifdef SKIP_ASM
extern "C" int func_00395530(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A40)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395538);
#ifdef SKIP_ASM
extern "C" int func_00395538(cPS2GraphicsMan* pS2GraphicsMan) {
    return ((int)pS2GraphicsMan->mpTextureMan);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395540);
#ifdef SKIP_ASM
extern "C" void func_00395540(void *graphicsMan, int minWait) {
    (*(int *)((char*)(graphicsMan) + (0x5AB8))) = minWait;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395548);
#ifdef SKIP_ASM
extern "C" int func_00395548(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A88)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395550);
#ifdef SKIP_ASM
extern "C" int func_00395550(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A5C)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395558);
#ifdef SKIP_ASM
extern "C" int func_00395558(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A50)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395560);
#ifdef SKIP_ASM
extern "C" int func_00395560(void *graphicsMan) {
    return (*(int *)((char*)(graphicsMan) + (0x5A3C)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395568);
#ifdef SKIP_ASM
extern "C" void func_00395568(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0x59E8))) = 1;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395578);
#ifdef SKIP_ASM
extern "C" void func_00395578(void *graphicsMan) {
    (*(int *)((char*)(graphicsMan) + (0x59E8))) = 0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395580);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator.
static inline float clamp_5580(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

extern "C" void func_00395580(void* self, float x)
{
    *(float*)((char*)self + 0x59FC) = clamp_5580(x, 2.0f);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003955A8);
#ifdef SKIP_ASM
extern "C" float func_003955A8(void *graphicsMan) {
    return (*(float *)((char*)(graphicsMan) + (0x59FC)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_003955B0);
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

// Queues a draw entry with the current render state in the uncached ring.
extern "C" void func_003955B0(cPS2GraphicsMan* gfx, unsigned int buf, unsigned int next, int flag)
{
    sDrawState* rs = gfx->m_pCurrentDrawState;
    sRing_55B0* ring = (sRing_55B0*)gfx->mpDeferMan;
    if (ring->count < 2600) {
        rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x1F) | (*(int*)((char*)ring + 0x69CC4) & 0x1F);
        int key = *(int*)((char*)ring + 0x69CC0);
        sEnt_55B0* e = (sEnt_55B0*)((ring->count++ << 7) + ((unsigned int)ring->ents | 0x30000000));
        e->rs = *rs;
        e->buf = buf;
        e->next = next;
        e->key = key;
        e->flag = flag;
    }
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395658);
#ifdef SKIP_ASM
extern "C" int func_00395658(cPS2GraphicsMan* pS2GraphicsMan) {
    return ((int)pS2GraphicsMan->mpDeferMan);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395660);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int func_00395660(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 16) & 0xFF);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00395688);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int func_00395688(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 24) & 0xFF);
}
#endif

//100%
INCLUDE_ASM("render/font", func_003956B0);
#ifdef SKIP_ASM
extern "C" void func_0037D968(void* self);

extern "C" void func_003956B0(cPS2GraphicsMan* self)
{
    if (self->mMatricesValid == 0) {
        func_0037D968(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_003956D8);
#ifdef SKIP_ASM
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
// Matrix stack push: duplicate the top 4x4 matrix into the next slot, then advance.
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
extern "C" void func_0031BE50(float* s, float* c, float angle);
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
    func_0031BE50(&s, &c, angle);
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

// Builds the view matrix on top of the stack: identity with y/z swapped, rotations about x, z, y,
// then a translation by -pos.
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
// Load matrix m into the top of the matrix stack and clear the cached flag at 0x6b90.
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
// Multiply the top of the matrix stack by m in place (top = top * m) and clear
// the cached flag at 0x6b90.
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
// Load the matrix t4mat_Identity (likely identity) into the top of the matrix stack
// and clear the cached flag at 0x6b90.
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
// Translate the top matrix of the stack: row 3 = top * v (v is a 4-vector),
// then clear the cached flag at 0x6b90.
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

extern "C" void func_0031BE50(float* s, float* c, float angle);

/**
 * Rotates the top view matrix by @p angle about @p axis: builds the axis-angle rotation matrix (sin and cos from
 * func_0031BE50) and concatenates it onto the top matrix with VU0, then clears mMatricesValid.
 *
 * On Tour's RotateView(float deg, const t4Vector& axis) takes degrees and the reverse order; here the axis comes
 * first and the angle goes straight to the sincos helper.
 *
 * @param axis  the rotation axis (x, y, z; assumed unit length)
 * @param angle  the rotation angle
 */
// Multiply the top of the matrix stack by a rotation of `angle` about `axis`.
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply); the PC port needs a plain matrix multiply.
extern "C" void cPS2GraphicsMan_RotateView(cPS2GraphicsMan* self, float* axis, float angle)
{
    sFontRotMat rot;
    float sinA;
    float cosA;
    char* top = (char*)self->mCurrentView;
    func_0031BE50(&sinA, &cosA, angle);
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
INCLUDE_ASM("render/font", func_00395ED8);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (matrix * vector, clamp, clip flags read back from vi1).
extern "C" int func_00395ED8(cPS2GraphicsMan* self, void* vec)
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
        : "r"(((void*)self)), "r"(vec), "r"((char*)self + 0x58A0)
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
INCLUDE_ASM("render/font", func_00395FA8);
#ifdef SKIP_ASM
extern "C" int func_00395FA8(cPS2GraphicsMan* self, char* vec) {
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
INCLUDE_ASM("render/font", func_00396058);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE); GS CSR hardware register.
extern "C" int func_00396058(void)
{
    return (int)((*(volatile unsigned long*)0x12001000 >> 13) & 1);
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396080);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
#define GS_CSR (*(volatile unsigned long*)0x12001000)
extern "C" int func_00396080(void)
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
INCLUDE_ASM("render/font", func_003960E8);
#ifdef SKIP_ASM
extern "C" void func_00382AF0(void* self, int arg1, int arg2);

extern "C" void func_003960E8(void* self, int arg1) {
    func_00382AF0(self, arg1, 1);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396108__FPv);
#ifdef SKIP_ASM
int func_00396108(void* self)
{
    return *(int*)((char*)self + 0x1C8);
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396110);
#ifdef SKIP_ASM
extern "C" void func_00396110(void* self, float range)
{
    *(float*)((char*)self + 0x194) = range;
    *(float*)((char*)self + 0x198) = 32767.0f / range;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396128);
#ifdef SKIP_ASM
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
extern "C" void func_00396190(void) {

}
#endif
//100%
INCLUDE_ASM("render/font", func_00396198);
#ifdef SKIP_ASM
extern "C" void func_00396198(void) {

}
#endif
//100%
INCLUDE_ASM("render/font", func_003961A0);
#ifdef SKIP_ASM
extern "C" void func_003961A0(void) {

}
#endif

//100%
INCLUDE_ASM("render/font", func_003961A8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_003961A8(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_003961D8);
#ifdef SKIP_ASM
extern int D_004A44C0;

extern "C" int *func_003961D8(void) {
    return &D_004A44C0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003961E8);
#ifdef SKIP_ASM
extern int D_004A44C4;

extern "C" int *func_003961E8(void) {
    return &D_004A44C4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003961F8);
#ifdef SKIP_ASM
extern int D_004A44C8;

extern "C" int *func_003961F8(void) {
    return &D_004A44C8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396208);
#ifdef SKIP_ASM
extern int D_004A44CC;

extern "C" int *func_00396208(void) {
    return &D_004A44CC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396218);
#ifdef SKIP_ASM
extern int D_004A44D0;

extern "C" int *func_00396218(void) {
    return &D_004A44D0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396228);
#ifdef SKIP_ASM
extern int D_004A44D4;

extern "C" int *func_00396228(void) {
    return &D_004A44D4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396238);
#ifdef SKIP_ASM
extern int D_004A44D8;

extern "C" int *func_00396238(void) {
    return &D_004A44D8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396248);
#ifdef SKIP_ASM
extern int D_004A44DC;

extern "C" int *func_00396248(void) {
    return &D_004A44DC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396258);
#ifdef SKIP_ASM
extern int D_004A44E0;

extern "C" int *func_00396258(void) {
    return &D_004A44E0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396268);
#ifdef SKIP_ASM
extern int D_004A44E4;

extern "C" int *func_00396268(void) {
    return &D_004A44E4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396278);
#ifdef SKIP_ASM
extern int D_004A44E8;

extern "C" int *func_00396278(void) {
    return &D_004A44E8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396288);
#ifdef SKIP_ASM
extern int D_004A44EC;

extern "C" int *func_00396288(void) {
    return &D_004A44EC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396298);
#ifdef SKIP_ASM
extern int D_004A44F0;

extern "C" int *func_00396298(void) {
    return &D_004A44F0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962A8);
#ifdef SKIP_ASM
extern int D_004A44F4;

extern "C" int *func_003962A8(void) {
    return &D_004A44F4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962B8);
#ifdef SKIP_ASM
extern int D_004A44F8;

extern "C" int *func_003962B8(void) {
    return &D_004A44F8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962C8);
#ifdef SKIP_ASM
extern int D_004A44FC;

extern "C" int *func_003962C8(void) {
    return &D_004A44FC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962D8);
#ifdef SKIP_ASM
extern int D_004A4500;

extern "C" int *func_003962D8(void) {
    return &D_004A4500;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962E8);
#ifdef SKIP_ASM
extern int D_004A4504;

extern "C" int *func_003962E8(void) {
    return &D_004A4504;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003962F8);
#ifdef SKIP_ASM
extern int D_004A4508;

extern "C" int *func_003962F8(void) {
    return &D_004A4508;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396308);
#ifdef SKIP_ASM
extern int D_004A450C;

extern "C" int *func_00396308(void) {
    return &D_004A450C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396318);
#ifdef SKIP_ASM
extern int D_004A4510;

extern "C" int *func_00396318(void) {
    return &D_004A4510;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396328);
#ifdef SKIP_ASM
extern int D_004A4514;

extern "C" int *func_00396328(void) {
    return &D_004A4514;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396338);
#ifdef SKIP_ASM
extern int D_004A4518;

extern "C" int *func_00396338(void) {
    return &D_004A4518;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396348);
#ifdef SKIP_ASM
extern int D_004A451C;

extern "C" int *func_00396348(void) {
    return &D_004A451C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396358);
#ifdef SKIP_ASM
extern int D_004A4520;

extern "C" int *func_00396358(void) {
    return &D_004A4520;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396368);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableSnowFall;

extern "C" int *func_00396368(void) {
    return &cRenderToggles_m_DisableSnowFall;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396378);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableBoardWake;

extern "C" int *func_00396378(void) {
    return &cRenderToggles_m_DisableBoardWake;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396388);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableSkyBox;

extern "C" int *func_00396388(void) {
    return &cRenderToggles_m_DisableSkyBox;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396398);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableCharacterSpecular;

extern "C" int *func_00396398(void) {
    return &cRenderToggles_m_DisableCharacterSpecular;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963A8);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableAIVisualFX;

extern "C" int *func_003963A8(void) {
    return &cRenderToggles_m_DisableAIVisualFX;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963B8);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableBoundingBoxes;

extern "C" int *func_003963B8(void) {
    return &cRenderToggles_m_DisableBoundingBoxes;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963C8);
#ifdef SKIP_ASM
extern int cRenderToggles_m_UseReflectionVector;

extern "C" int *func_003963C8(void) {
    return &cRenderToggles_m_UseReflectionVector;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963D8);
#ifdef SKIP_ASM
extern int cRenderToggles_m_DisableHUD;

extern "C" int *func_003963D8(void) {
    return &cRenderToggles_m_DisableHUD;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963E8);
#ifdef SKIP_ASM
extern int D_004A4544;

extern "C" int *func_003963E8(void) {
    return &D_004A4544;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003963F8);
#ifdef SKIP_ASM
extern int D_004A4548;

extern "C" int *func_003963F8(void) {
    return &D_004A4548;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396408);
#ifdef SKIP_ASM
extern int D_004A454C;

extern "C" int *func_00396408(void) {
    return &D_004A454C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396418);
#ifdef SKIP_ASM
extern int D_004A4550;

extern "C" int *func_00396418(void) {
    return &D_004A4550;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396428);
#ifdef SKIP_ASM
extern int D_004A4554;

extern "C" int *func_00396428(void) {
    return &D_004A4554;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396438);
#ifdef SKIP_ASM
extern int D_004A4558;

extern "C" int *func_00396438(void) {
    return &D_004A4558;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396448);
#ifdef SKIP_ASM
extern int D_004A455C;

extern "C" int *func_00396448(void) {
    return &D_004A455C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396458);
#ifdef SKIP_ASM
extern int D_004A4560;

extern "C" int *func_00396458(void) {
    return &D_004A4560;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396468);
#ifdef SKIP_ASM
extern int D_004A4564;

extern "C" int *func_00396468(void) {
    return &D_004A4564;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396478);
#ifdef SKIP_ASM
extern int D_004A4568;

extern "C" int *func_00396478(void) {
    return &D_004A4568;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396488);
#ifdef SKIP_ASM
extern int D_004A456C;

extern "C" int *func_00396488(void) {
    return &D_004A456C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396498);
#ifdef SKIP_ASM
extern int D_004A4570;

extern "C" int *func_00396498(void) {
    return &D_004A4570;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003964A8);
#ifdef SKIP_ASM
extern int D_004A4574;

extern "C" int *func_003964A8(void) {
    return &D_004A4574;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003964B8);
#ifdef SKIP_ASM
extern int D_004A4578;

extern "C" int *func_003964B8(void) {
    return &D_004A4578;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003964C8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_003964C8(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_003964F8);
#ifdef SKIP_ASM
extern int cPlants_m_Enable;

extern "C" int *func_003964F8(void) {
    return &cPlants_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396508);
#ifdef SKIP_ASM
extern int D_004A4448;

extern "C" int *func_00396508(void) {
    return &D_004A4448;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396518);
#ifdef SKIP_ASM
extern int D_004A444C;

extern "C" int *func_00396518(void) {
    return &D_004A444C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396528);
#ifdef SKIP_ASM
extern int D_004A4450;

extern "C" int *func_00396528(void) {
    return &D_004A4450;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396538);
#ifdef SKIP_ASM
extern int D_004A4454;

extern "C" int *func_00396538(void) {
    return &D_004A4454;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396548);
#ifdef SKIP_ASM
extern int D_004A4458;

extern "C" int *func_00396548(void) {
    return &D_004A4458;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396558);
#ifdef SKIP_ASM
extern int D_004A445C;

extern "C" int *func_00396558(void) {
    return &D_004A445C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396568);
#ifdef SKIP_ASM
extern int D_004A4460;

extern "C" int *func_00396568(void) {
    return &D_004A4460;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396578);
#ifdef SKIP_ASM
extern int D_004A4464;

extern "C" int *func_00396578(void) {
    return &D_004A4464;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396588);
#ifdef SKIP_ASM
extern int D_004A4468;

extern "C" int *func_00396588(void) {
    return &D_004A4468;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396598);
#ifdef SKIP_ASM
extern int D_004A446C;

extern "C" int *func_00396598(void) {
    return &D_004A446C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965A8);
#ifdef SKIP_ASM
extern int D_004A4470;

extern "C" int *func_003965A8(void) {
    return &D_004A4470;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965B8);
#ifdef SKIP_ASM
extern "C" int func_003965B8(int arg0) {
    return arg0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965C0);
#ifdef SKIP_ASM
extern "C" int func_003965C0(int arg0) {
    return arg0 + 352;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965C8);
#ifdef SKIP_ASM
extern "C" int func_003965C8(int arg0) {
    return arg0 + 0xC0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965D0);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_003965D0(void *arg0, long long *arg1) {
    (*(long long *)((char*)(arg0) + (0xC0))) = (long long) *arg1;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965E0);
#ifdef SKIP_ASM
extern "C" int func_003965E0(int arg0) {
    return arg0 + 0x20;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965E8);
#ifdef SKIP_ASM
extern "C" void func_003965E8(void *arg0, float fparg0) {
    (*(float *)((char*)(arg0) + (0x58))) = fparg0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965F0);
#ifdef SKIP_ASM
extern "C" int func_003965F0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x178)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_003965F8);
#ifdef SKIP_ASM
extern "C" int func_003965F8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x17C)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396600);
#ifdef SKIP_ASM
extern "C" int func_00396600(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x174)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396608);
#ifdef SKIP_ASM
extern "C" void func_00396608(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x174))) = arg1;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396610);
#ifdef SKIP_ASM
extern "C" int func_00396610(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x1E0)));
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396618);
#ifdef SKIP_ASM
struct sFontQuad_6618 {
    int v[4];
} __attribute__((aligned(16)));

extern "C" sFontQuad_6618 func_00396618(void* self, int idx)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    return (*(sFontQuad_6618**)((char*)self + 0x1A0))[(idx + next) % *(int*)((char*)self + 0x178)];
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396658);
#ifdef SKIP_ASM
struct sFontQuad_6658 {
    int v[4];
} __attribute__((aligned(16)));

extern "C" sFontQuad_6658 func_00396658(void* self, int idx)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    return (*(sFontQuad_6658**)((char*)self + 0x1A4))[(idx + next) % *(int*)((char*)self + 0x178)];
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396698);
#ifdef SKIP_ASM
extern "C" void func_00396698(void* self, int idx, int** outA, int** outB)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    int off = ((idx + next) % *(int*)((char*)self + 0x178)) * 0x10;
    *outA = (int*)(*(char**)((char*)self + 0x1A0) + off);
    *outB = (int*)(*(char**)((char*)self + 0x1A4) + off);
}
#endif
//100%
INCLUDE_ASM("render/font", func_003966D8);
#ifdef SKIP_ASM
struct sFontQuad_66D8 {
    int v[4];
} __attribute__((aligned(16)));

extern "C" void func_003966D8(void* self, int idx, sFontQuad_66D8* outA, sFontQuad_66D8* outB)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    int i = (idx + next) % *(int*)((char*)self + 0x178);
    *outA = (*(sFontQuad_66D8**)((char*)self + 0x1A0))[i];
    *outB = (*(sFontQuad_66D8**)((char*)self + 0x1A4))[i];
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396720);
#ifdef SKIP_ASM
extern "C" void func_00396720(void* self, int idx, int** outA, int** outB, int** outC)
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
INCLUDE_ASM("render/font", func_00396770);
#ifdef SKIP_ASM
struct sFontQ4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFontRGBA {
    char r, g, b, a;
};

extern "C" void func_00396770(char* self, int idx, sFontQ4* out0, sFontQ4* out1, sFontRGBA* out2)
{
    int i = (idx + 1 + *(int*)(self + 0x17C)) % *(int*)(self + 0x178);
    *out0 = (*(sFontQ4**)(self + 0x1A0))[i];
    *out1 = (*(sFontQ4**)(self + 0x1A4))[i];
    *out2 = (*(sFontRGBA**)(self + 0x204))[i];
}
#endif
//100%
INCLUDE_ASM("render/font", func_003967D8);
#ifdef SKIP_ASM
struct sFontP_67D8 {
    int v;
} __attribute__((packed));

struct sFontWord_67D8 {
    sFontP_67D8 p;
    sFontWord_67D8() {}
    sFontWord_67D8(const sFontWord_67D8& o) : p(o.p) {}
};

extern "C" sFontWord_67D8 func_003967D8(void* self, int idx)
{
    int next = *(int*)((char*)self + 0x17C) + 1;
    return (*(sFontWord_67D8**)((char*)self + 0x204))[(idx + next) % *(int*)((char*)self + 0x178)];
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396820);
#ifdef SKIP_ASM
extern char D_004930A0[];

extern "C" void* func_00396820(void** self)
{
    *self = D_004930A0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396838);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_00396838(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396868);
#ifdef SKIP_ASM
extern int D_004A41D4;

extern "C" int *func_00396868(void) {
    return &D_004A41D4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396878);
#ifdef SKIP_ASM
extern int D_004A41D8;

extern "C" int *func_00396878(void) {
    return &D_004A41D8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396888);
#ifdef SKIP_ASM
extern int D_004A41DC;

extern "C" int *func_00396888(void) {
    return &D_004A41DC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396898);
#ifdef SKIP_ASM
extern int D_004A41E0;

extern "C" int *func_00396898(void) {
    return &D_004A41E0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003968A8);
#ifdef SKIP_ASM
extern int D_004A41E4;

extern "C" int *func_003968A8(void) {
    return &D_004A41E4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003968B8);
#ifdef SKIP_ASM
extern int D_004A41E8;

extern "C" int *func_003968B8(void) {
    return &D_004A41E8;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003968C8__FPv);
#ifdef SKIP_ASM
void func_003968C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_003968D0);
#ifdef SKIP_ASM
extern "C" void func_003968D0(void* self, int index, void* src)
{
    cQuad128* quads = *(cQuad128**)((char*)self + 0x8);
    quads[index] = *(cQuad128*)src;
}
#endif

//100%
INCLUDE_ASM("render/font", func_003968E8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_003968E8(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396918__FPv);
#ifdef SKIP_ASM
void func_00396918(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396920__FPv);
#ifdef SKIP_ASM
void func_00396920(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396928);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_00396928(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396958__FPv);
#ifdef SKIP_ASM
void func_00396958(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396960);
#ifdef SKIP_ASM
extern int D_004A4340;

extern "C" int *func_00396960(void) {
    return &D_004A4340;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396970);
#ifdef SKIP_ASM
extern int D_004A4344;

extern "C" int *func_00396970(void) {
    return &D_004A4344;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396980);
#ifdef SKIP_ASM
extern int D_004A4348;

extern "C" int *func_00396980(void) {
    return &D_004A4348;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396990);
#ifdef SKIP_ASM
extern int D_004A434C;

extern "C" int *func_00396990(void) {
    return &D_004A434C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969A0);
#ifdef SKIP_ASM
extern int D_004A4350;

extern "C" int *func_003969A0(void) {
    return &D_004A4350;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969B0);
#ifdef SKIP_ASM
extern int D_004A4354;

extern "C" int *func_003969B0(void) {
    return &D_004A4354;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969C0);
#ifdef SKIP_ASM
extern int D_004A4358;

extern "C" int *func_003969C0(void) {
    return &D_004A4358;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969D0);
#ifdef SKIP_ASM
extern int D_004A435C;

extern "C" int *func_003969D0(void) {
    return &D_004A435C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969E0);
#ifdef SKIP_ASM
extern int D_004A4360;

extern "C" int *func_003969E0(void) {
    return &D_004A4360;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003969F0);
#ifdef SKIP_ASM
extern int D_004A4364;

extern "C" int *func_003969F0(void) {
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
INCLUDE_ASM("render/font", func_00396A30);
#ifdef SKIP_ASM
extern int D_004A4478;

extern "C" int *func_00396A30(void) {
    return &D_004A4478;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A40);
#ifdef SKIP_ASM
extern int D_004A447C;

extern "C" int *func_00396A40(void) {
    return &D_004A447C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A50);
#ifdef SKIP_ASM
extern int D_004A4480;

extern "C" int *func_00396A50(void) {
    return &D_004A4480;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A60);
#ifdef SKIP_ASM
extern int D_004A4484;

extern "C" int *func_00396A60(void) {
    return &D_004A4484;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A70);
#ifdef SKIP_ASM
extern int D_004A4488;

extern "C" int *func_00396A70(void) {
    return &D_004A4488;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A80);
#ifdef SKIP_ASM
extern int D_004A448C;

extern "C" int *func_00396A80(void) {
    return &D_004A448C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396A90);
#ifdef SKIP_ASM
extern int D_004A4490;

extern "C" int *func_00396A90(void) {
    return &D_004A4490;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AA0);
#ifdef SKIP_ASM
extern int D_004A4494;

extern "C" int *func_00396AA0(void) {
    return &D_004A4494;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AB0);
#ifdef SKIP_ASM
extern int D_004A4498;

extern "C" int *func_00396AB0(void) {
    return &D_004A4498;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AC0);
#ifdef SKIP_ASM
extern int D_004A449C;

extern "C" int *func_00396AC0(void) {
    return &D_004A449C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AD0);
#ifdef SKIP_ASM
extern int D_004A44A0;

extern "C" int *func_00396AD0(void) {
    return &D_004A44A0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AE0);
#ifdef SKIP_ASM
extern int D_004A44A4;

extern "C" int *func_00396AE0(void) {
    return &D_004A44A4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396AF0);
#ifdef SKIP_ASM
extern int D_004A44A8;

extern "C" int *func_00396AF0(void) {
    return &D_004A44A8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396B00);
#ifdef SKIP_ASM
extern int D_004A44AC;

extern "C" int *func_00396B00(void) {
    return &D_004A44AC;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396B10);
#ifdef SKIP_ASM
extern int D_004A44B0;

extern "C" int *func_00396B10(void) {
    return &D_004A44B0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396B20);
#ifdef SKIP_ASM
extern int cVisualEffectsTest_m_Enable;

extern "C" int *func_00396B20(void) {
    return &cVisualEffectsTest_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396B30);
#ifdef SKIP_ASM
extern int cVisualEffectsTest_m_LayerId;

extern "C" int *func_00396B30(void) {
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
 * microprogram at D_1150. cPS2GraphicsMan_DrawCharModel (DrawCharModel) calls it through slot 124 with a scale of 1.0, which this code
 * ignores.
 */
// Emits the font's GS setup packet (register set + the 10 cached qwords) and a VU call for the glyph program.
extern "C" void cPS2GraphicsMan_InitCharMatrices(char* self)
{
    sRS_6B40* rs = *(sRS_6B40**)(self + 0xE84);
    unsigned int addrMask = 0x3FFFFFFF;
    rs->f0 = (rs->f0 & ~0x3C0) | 0x180;
    sRS_6B40* rs2 = *(sRS_6B40**)(self + 0xE84);
    rs2->fC = (rs2->fC & 0xC0000000) | (*(unsigned int*)(self + 0x5A00) & addrMask);
    unsigned long* packet = (unsigned long*)(*(unsigned int*)(self + 0x5A00) | 0x30000000);
    packet[0] = 0x6000000C;
    packet[2] = (unsigned long)0x8000 << 45;
    packet[3] = 0x6C0A000701000101;
    packet[1] = 0;
    packet += 4;
    cQuad128_6B40* src = (cQuad128_6B40*)(self + 0x6BB0);
    for (int i = 0; i < 10; i++) {
        ((cQuad128_6B40*)packet)[i] = src[i];
    }
    packet[20] = 0;
    packet[21] = (unsigned long)(((unsigned int)D_1150 >> 3) | 0x14000000) << 32;
    packet += 22;
    *(unsigned int*)(self + 0x5A00) = (unsigned int)packet & 0x0FFFFFFF;
}
#endif

//100%
INCLUDE_ASM("render/font", func_00396C30);
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

// Emits a DMA chain that uploads count 4x4 matrices (selected by index) and then a closing tag.
extern "C" void func_00396C30(void* self, sMtxList_6C30* list, sMtx_6C30* mats, unsigned long** cur)
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
INCLUDE_ASM("render/font", func_00396D30);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00488680[];

extern "C" void func_00396D30(int* self, int flags)
{
    *(void**)self = D_00488680;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

extern "C" void* __sti__all_in_one_render_cpp(int, int);

//100%
INCLUDE_ASM("render/font", func_00396D60);
#ifdef SKIP_ASM
extern int cScreenTint_m_Enable;

extern "C" int *func_00396D60(void) {
    return &cScreenTint_m_Enable;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396D70);
#ifdef SKIP_ASM
extern int cScreenTint_m_OverrideWP;

extern "C" int *func_00396D70(void) {
    return &cScreenTint_m_OverrideWP;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396D80);
#ifdef SKIP_ASM
extern int cScreenTint_m_DisplayValues;

extern "C" int *func_00396D80(void) {
    return &cScreenTint_m_DisplayValues;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396D90);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourR;

extern "C" int *func_00396D90(void) {
    return &cScreenTint_m_ColourR;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DA0);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourG;

extern "C" int *func_00396DA0(void) {
    return &cScreenTint_m_ColourG;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DB0);
#ifdef SKIP_ASM
extern int cScreenTint_m_ColourB;

extern "C" int *func_00396DB0(void) {
    return &cScreenTint_m_ColourB;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DC0);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillR;

extern "C" int *func_00396DC0(void) {
    return &cScreenTint_m_FillR;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DD0);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillG;

extern "C" int *func_00396DD0(void) {
    return &cScreenTint_m_FillG;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DE0);
#ifdef SKIP_ASM
extern int cScreenTint_m_FillB;

extern "C" int *func_00396DE0(void) {
    return &cScreenTint_m_FillB;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396DF0);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourR;

extern "C" int *func_00396DF0(void) {
    return &cScreenTint_m_XNColourR;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E00);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourG;

extern "C" int *func_00396E00(void) {
    return &cScreenTint_m_XNColourG;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E10);
#ifdef SKIP_ASM
extern int cScreenTint_m_XNColourB;

extern "C" int *func_00396E10(void) {
    return &cScreenTint_m_XNColourB;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E20);
#ifdef SKIP_ASM
extern int D_004A4608;

extern "C" int *func_00396E20(void) {
    return &D_004A4608;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E30);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningEnable;

extern "C" int *func_00396E30(void) {
    return &cScreenTint_m_LightningEnable;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E40);
#ifdef SKIP_ASM
extern int D_004A4610;

extern "C" int *func_00396E40(void) {
    return &D_004A4610;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E50);
#ifdef SKIP_ASM
extern int cScreenTint_m_LightningChance;

extern "C" int *func_00396E50(void) {
    return &cScreenTint_m_LightningChance;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E60);
#ifdef SKIP_ASM
extern int D_004A4618;

extern "C" int *func_00396E60(void) {
    return &D_004A4618;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E70);
#ifdef SKIP_ASM
extern int D_004A461C;

extern "C" int *func_00396E70(void) {
    return &D_004A461C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E80);
#ifdef SKIP_ASM
extern int D_004A4620;

extern "C" int *func_00396E80(void) {
    return &D_004A4620;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396E90);
#ifdef SKIP_ASM
extern int D_004A4624;

extern "C" int *func_00396E90(void) {
    return &D_004A4624;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396EA0);
#ifdef SKIP_ASM
extern int D_004A4628;

extern "C" int *func_00396EA0(void) {
    return &D_004A4628;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396EB0);
#ifdef SKIP_ASM
extern int D_004A462C;

extern "C" int *func_00396EB0(void) {
    return &D_004A462C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396EC0);
#ifdef SKIP_ASM
extern int D_004A4630;

extern "C" int *func_00396EC0(void) {
    return &D_004A4630;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396ED0);
#ifdef SKIP_ASM
extern int D_004A4634;

extern "C" int *func_00396ED0(void) {
    return &D_004A4634;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396EE0);
#ifdef SKIP_ASM
extern int D_004A4638;

extern "C" int *func_00396EE0(void) {
    return &D_004A4638;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396EF0);
#ifdef SKIP_ASM
extern int D_004A463C;

extern "C" int *func_00396EF0(void) {
    return &D_004A463C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F00);
#ifdef SKIP_ASM
extern int D_004A4640;

extern "C" int *func_00396F00(void) {
    return &D_004A4640;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F10);
#ifdef SKIP_ASM
extern int D_004A4644;

extern "C" int *func_00396F10(void) {
    return &D_004A4644;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F20);
#ifdef SKIP_ASM
extern int D_004A4648;

extern "C" int *func_00396F20(void) {
    return &D_004A4648;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F30);
#ifdef SKIP_ASM
extern int D_004A464C;

extern "C" int *func_00396F30(void) {
    return &D_004A464C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F40);
#ifdef SKIP_ASM
extern int D_004A4650;

extern "C" int *func_00396F40(void) {
    return &D_004A4650;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F50);
#ifdef SKIP_ASM
extern int D_004A4654;

extern "C" int *func_00396F50(void) {
    return &D_004A4654;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F60);
#ifdef SKIP_ASM
extern int D_004A4658;

extern "C" int *func_00396F60(void) {
    return &D_004A4658;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F70);
#ifdef SKIP_ASM
extern int D_004A465C;

extern "C" int *func_00396F70(void) {
    return &D_004A465C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F80);
#ifdef SKIP_ASM
extern int D_004A4660;

extern "C" int *func_00396F80(void) {
    return &D_004A4660;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396F90);
#ifdef SKIP_ASM
extern int D_004A4664;

extern "C" int *func_00396F90(void) {
    return &D_004A4664;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396FA0);
#ifdef SKIP_ASM
extern int D_004A4668;

extern "C" int *func_00396FA0(void) {
    return &D_004A4668;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396FB0);
#ifdef SKIP_ASM
extern int D_004A466C;

extern "C" int *func_00396FB0(void) {
    return &D_004A466C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396FC0);
#ifdef SKIP_ASM
extern int D_004A4670;

extern "C" int *func_00396FC0(void) {
    return &D_004A4670;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396FD0);
#ifdef SKIP_ASM
extern int D_004A4674;

extern "C" int *func_00396FD0(void) {
    return &D_004A4674;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00396FE0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern float colR __asm__("cScreenTint_m_ColourR");
extern float colG __asm__("cScreenTint_m_ColourG");
extern float colB __asm__("cScreenTint_m_ColourB");

extern "C" int func_00396FE0(void)
{
    int red = (int)(colR * 255.0f);
    int green = (int)(colG * 255.0f);
    int blue = (int)(colB * 255.0f);
    return (0xFF << 24) | (red << 16) | (green << 8) | blue;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397038);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A0;

extern "C" float func_00397038(void) {
    return D_004A469C * D_004A46A0;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397048);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A4;

extern "C" float func_00397048(void) {
    return D_004A469C * D_004A46A4;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397058);
#ifdef SKIP_ASM
extern float D_004A469C;
extern float D_004A46A8;

extern "C" float func_00397058(void) {
    return D_004A469C * D_004A46A8;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397068);
#ifdef SKIP_ASM
extern "C" void func_00397068(float value) {
    *(float*)&cScreenTint_m_FillB = value;
    *(float*)&cScreenTint_m_FillG = value;
    *(float*)&cScreenTint_m_FillR = value;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397078);
#ifdef SKIP_ASM
extern int D_004A4678;

extern "C" int *func_00397078(void) {
    return &D_004A4678;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397088);
#ifdef SKIP_ASM
extern int D_004A467C;

extern "C" int *func_00397088(void) {
    return &D_004A467C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_00397098);
#ifdef SKIP_ASM
extern int D_004A4680;

extern "C" int *func_00397098(void) {
    return &D_004A4680;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003970A8);
#ifdef SKIP_ASM
extern int D_004A4684;

extern "C" int *func_003970A8(void) {
    return &D_004A4684;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003970B8);
#ifdef SKIP_ASM
extern int D_004A4688;

extern "C" int *func_003970B8(void) {
    return &D_004A4688;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003970C8);
#ifdef SKIP_ASM
extern int D_004A468C;

extern "C" int *func_003970C8(void) {
    return &D_004A468C;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003970D8);
#ifdef SKIP_ASM
extern int D_004A4690;

extern "C" int *func_003970D8(void) {
    return &D_004A4690;
}
#endif
//100%
INCLUDE_ASM("render/font", func_003970E8);
#ifdef SKIP_ASM
extern int D_004A4694;

extern "C" int *func_003970E8(void) {
    return &D_004A4694;
}
#endif

//99.38%
INCLUDE_ASM("render/font", func_003970F8__FPv);
#ifdef SKIP_ASM
void* func_003970F8(void* self)
{
    return __sti__all_in_one_render_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__all_in_one_render_cpp(int, int);

//99.38%
INCLUDE_ASM("render/font", func_00397118__FPv);
#ifdef SKIP_ASM
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

