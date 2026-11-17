#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

struct cOVStateHUDElem {
    int field_0x0;
    int field_0x4;
    float rangeMin; // 0x8
    float rangeMax; // 0xc
    float x; // 0x10
    float y; // 0x14
    float z; // 0x18
    float w; // 0x1c
};

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHiScoreList_onCreateScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);
extern signed char D_00535C10[];
extern char str_hiscorelist[];
extern char str_helptext02[];
extern char str_helptext01[];
extern char str_helptxt01[];
extern char str_helptxt02[];

static inline int HasScores1E8FF0(char* hs)
{
    return *(int*)(hs + 0x18) >= 0 || *(int*)(hs + 0x1C) >= 0;
}

/**
 * Builds the high-score list ("hiscorelist") and shows its help texts for the current event.
 *
 * For cGameMode 0 / 5 it shows "helptext02", otherwise it hides "helptext01"; without a new record for either
 * player (score interface +0x18 / +0x1C both negative) "helptxt01" and "helptxt02" are hidden. Plays UI event
 * 0xE.
 */
// Builds the hi-score list screen and shows/hides its panels for the current game mode.
extern "C" void cOVStateHiScoreList_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_hiscorelist), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    switch (D_00535C10[0]) {
    case 0: case 5:
        cUIState_showObjSafe(self, str_helptext02);
        break;
    case 1: case 2: case 3: case 4: case 6:
    default:
        cUIState_hideObjSafe(self, str_helptext01);
        break;
    }
    char* hs = (char*)cBE_getInterface_Fv(cBE_getBE(), 8);
    if (!HasScores1E8FF0(hs)) {
        cUIState_hideObjSafe(self, str_helptxt01);
        cUIState_hideObjSafe(self, str_helptxt02);
    }
    cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xE);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHiScoreList_onWidgetEvent);
#ifdef SKIP_ASM
struct sVEntry001E9130 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

struct sObj001E9130 {
    int field_0x0;
    sVEntry001E9130* vt;
};

extern "C" void cUIStateStack_push(void* list, void* item);

/**
 * On Cross, pushes the front end's next state for the item with choice 1.
 *
 * Other items push a null state, which cUIStateStack_push() does not check.
 *
 * @param item  the chosen item; ignored when null
 * @param Event  the widget event; only 5 (Cross) acts
 */
extern "C" void cOVStateHiScoreList_onWidgetEvent(void* self, void* item, int Event)
{
    if (item != 0 && Event == 5) {
        void* nextState = 0;
        if (*(int*)((char*)item + 0x18) == 1) {
            sObj001E9130* factory = **(sObj001E9130***)((char*)self + 0x10);
            nextState = factory->vt[4].fn((char*)factory + factory->vt[4].delta, self, 1);
        }
        cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", tHUDDef_getColour__FPvT0);
#ifdef SKIP_ASM
/**
 * Copies a HUD element definition's colour into @p outColor in alpha, red, green, blue order.
 *
 * The four floats at +0x10..+0x1C (x, y, z, w in this unit's cOVStateHUDElem view; On Tour tHUDDef::a, r, g, b) are
 * copied unchanged. The text renderers (cOVStateHUD1P_renderTime1, cOVStateHUD1P_renderText, cOVStateHUD1P_renderMultiText, ...) use it for an element's
 * default colour.
 *
 * @param outColor  receives the colour as four floats (a tARGB: alpha, red, green, blue)
 */
void tHUDDef_getColour(void* self, void* outColor)
{
    cOVStateHUDElem* elem = (cOVStateHUDElem*)self;
    float* out = (float*)outColor;
    out[0] = elem->x;
    out[1] = elem->y;
    out[2] = elem->z;
    out[3] = elem->w;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", tHUDDef_getColour1__FPvT0);
#ifdef SKIP_ASM
/**
 * Copies a HUD element definition's colour into @p outColor as an (r, g, b, a) vector.
 *
 * The alpha at +0x10 goes to the fourth float and red, green, blue (+0x14..+0x1C) to the first three. The blending text
 * renderers cOVStateHUD1P_renderText1 and func_001F25E0 use it to interpolate two elements' colours with VU0 vector maths.
 *
 * @param outColor  receives the colour as an (r, g, b, a) 4-vector
 */
void tHUDDef_getColour1(void* self, void* outColor)
{
    cOVStateHUDElem* elem = (cOVStateHUDElem*)self;
    float* out = (float*)outColor;
    out[3] = elem->x;
    out[0] = elem->y;
    out[1] = elem->z;
    out[2] = elem->w;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", tHUDDef_getPos);
#ifdef SKIP_ASM
/**
 * Writes a HUD element definition's screen position into @p outPos.
 *
 * The position is the two shorts at +0x0 and +0x2 (On Tour tHUDDef::px, py), converted to float.
 *
 * @param outPos  receives (x, y)
 */
extern "C" void tHUDDef_getPos(void* self, void* outPos)
{
    short* in = (short*)self;
    float* out = (float*)outPos;
    out[0] = in[0];
    out[1] = in[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", tHUDDef_getSize);
#ifdef SKIP_ASM
/**
 * Writes a HUD element definition's size into @p outVec, taking unset dimensions from a texture element.
 *
 * Each dimension comes from the shorts at +0x4/+0x6 (On Tour tHUDDef::sx, sy). A negative value means "not set": the
 * dimension becomes 0, or the texture element's width or height (+0x4/+0x8, On Tour cTextureElement::mWidth/mHeight)
 * when @p fallback is given. cOVStateHUD1P_renderMultiText and cOVStateHUD1P_renderMultiText1 pass no texture and use the width as their wrap width.
 *
 * @param outVec  receives (width, height)
 * @param fallback  a texture element whose mWidth/mHeight replace unset dimensions, or 0
 */
extern "C" void tHUDDef_getSize(void* self, void* outVec, void* fallback)
{
    short* in = (short*)self;
    float* out = (float*)outVec;
    float* def = (float*)fallback;
    float x = in[2];
    if (x >= 0.0f) {
        out[0] = x;
    } else {
        out[0] = 0.0f;
        if (def != 0) {
            out[0] = def[1];
        }
    }
    float y = in[3];
    if (y >= 0.0f) {
        out[1] = y;
    } else {
        out[1] = 0.0f;
        if (def != 0) {
            out[1] = def[2];
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", tHUDDef_getScale__FPvT0);
#ifdef SKIP_ASM
/**
 * Writes a HUD element definition's x and y scale into @p outRange.
 *
 * Copies the floats at +0x8/+0xC (rangeMin/rangeMax in this unit's cOVStateHUDElem view; On Tour tHUDDef::xx, xy).
 * Every renderer multiplies the element's size and font scale by them. The declaration also returns the x scale; the
 * callers ignore it.
 *
 * @param outRange  receives (scale x, scale y)
 * @return the x scale it copied
 */
float tHUDDef_getScale(void* self, void* outRange)
{
    cOVStateHUDElem* elem = (cOVStateHUDElem*)self;
    float minValue = elem->rangeMin;
    *(float*)outRange = minValue;
    *(float*)((char*)outRange + 0x4) = elem->rangeMax;
    return minValue;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E92A8);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" float func_003921F0(void* self, const unsigned short* str, void* out, int n, float sx, float sy);
struct sFont_1E92A8 {
    char pad0[0x14];
    int lineH;          // 0x14
    char pad18[0x18];
    float w30;          // 0x30
    float h34;          // 0x34
    float sx;           // 0x38
    float sy;           // 0x3C
};
struct sIcon_1E92A8 {
    int hash;           // 0x0
    char pad4[8];
    float w;            // 0xC
    float h;            // 0x10
    char pad14[0x10];
};
struct sSeg_1E92A8 {
    float w;
    unsigned short* str;
};
struct sLine_1E92A8 {
    float total;        // 0x0
    float height;       // 0x4
    sFont_1E92A8* font; // 0x8
    float yoff;         // 0xC
    float sx;           // 0x10
    float sy;           // 0x14
    int count;          // 0x18
    sSeg_1E92A8 segs[6];// 0x1C
    unsigned short text[1]; // 0x4C
};
extern sIcon_1E92A8 D_004C8980[];

/**
 * Lays out a HUD text line with inline pictures: splits its 16-bit text into text and picture segments and measures
 * them.
 *
 * The line object (filled by func_001E94E0 / func_001E9540) holds the total width (+0x0), height (+0x4), font (+0x8),
 * text y offset (+0xC), scale (+0x10, +0x14), segment count (+0x18), six (width, pointer) segments (+0x1C) and the text
 * (+0x4C). First the font's Scale is set to BaseScale times the line scale. Each '@' ends the current text segment (it
 * is overwritten with 0) and starts a picture name that runs to the next space or the end; the name's GetHashValue32 is
 * looked up in the picture table D_004C8980 (0x24-byte records: hash, texture, width +0xC, height +0x10, uvs; On Tour
 * tHUDPictureDB) and the record becomes the next segment, so even segments are text and odd ones pictures. Text
 * segments are measured with func_003921F0, with the end advance before a picture and without it for the last one. It
 * stores the total width, the height max(font MaxCharHeight * BaseScale.y * scale y, tallest picture) and the y offset
 * that centres the text in that height.
 *
 * Likely bugs: a name missing from the table leaves a null record that is read anyway, and nothing limits the segments
 * to the six slots (a third picture writes past them into the text). The picture name is copied from 16-bit characters
 * into an 8-bit buffer, keeping the low byte of each.
 *
 * @param textLine  the text line to lay out
 */
extern "C" void func_001E92A8(void* textLine)
{
    sLine_1E92A8* self = (sLine_1E92A8*)textLine;
    unsigned short* cursor;
    int n = 0;
    sFont_1E92A8* font = self->font;
    sSeg_1E92A8* segs = self->segs;
    char buf[16];
    char name[32];
    font->sx = font->w30 * self->sx;
    font->sy = font->h34 * self->sy;
    self->segs[0].w = 0;
    float maxh = self->segs[0].w;
    self->count = 0;
    self->segs[0].str = self->text;
    cursor = self->segs[0].str;
    float total = maxh;
    while (*cursor != 0) {
        if (*cursor == 0x40) {
            *cursor = 0;
            cursor++;
            if (*segs[n].str != 0) {
                float w = func_003921F0(font, segs[n].str, buf, 1, font->sx, font->sy);
                total += w;
                segs[n].w = w;
            }
            n++;
            char* namePos = name;
            do {
                if (*cursor == 0x20 || *cursor == 0) break;
                *namePos = *cursor;
                cursor++;
                namePos++;
            } while (1);
            *namePos = 0;
            int h = GetHashValue32(name);
            sIcon_1E92A8* picture = 0;
            for (sIcon_1E92A8* candidate = D_004C8980; candidate->hash != 0; candidate++) {
                if (candidate->hash == h) { picture = candidate; break; }
            }
            // The icon entry is kept in the segment's string slot.
            segs[n].str = (unsigned short*)picture;
            float ew = picture->w;
            segs[n].w = ew;
            total += ew;
            n++;
            if (maxh < picture->h) maxh = picture->h;
            segs[n].str = cursor;
            segs[n].w = 0;
            if (*cursor != 0) cursor++;
        } else {
            cursor++;
        }
    }
    if (*segs[n].str != 0) {
        float w = func_003921F0(font, segs[n].str, buf, 0, font->sx, font->sy);
        total += w;
        segs[n].w = w;
        n++;
    }
    self->count = n;
    float h = (float)font->lineH * font->h34 * self->sy;
    self->total = total;
    // PORT: `>?` is the g++ maximum operator (EE max.s).
    float lineHeight = h >? maxh;
    self->height = lineHeight;
    self->yoff = (lineHeight - h) * 0.5f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovstatehud", func_001E94E0);
#ifdef SKIP_ASM
extern "C" unsigned short* func_002C2540(unsigned short* dst, char* src);
extern "C" void func_001E92A8(void* self);
struct sVec2f { float x, y; };

/**
 * Sets a HUD text line from an 8-bit string and lays it out.
 *
 * Widens @p str into the line's 16-bit text (+0x4C) with func_002C2540, stores @p font (+0x8) and @p scale (+0x10),
 * then runs func_001E92A8. cOVStateHUD1P_onCreateScreen fills three of its message lines this way with the HUD's +0x428
 * font.
 *
 * @param str  the text; "@name" embeds a picture from D_004C8980
 * @param font  the font (a cFont*) to measure and draw with
 * @param scale  the line's x and y scale
 */
extern "C" void func_001E94E0(void* self, char* str, int font, sVec2f* scale)
{
    func_002C2540((unsigned short*)((char*)self + 0x4C), str);
    *(int*)((char*)self + 0x8) = font;
    *(sVec2f*)((char*)self + 0x10) = *scale;
    func_001E92A8(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovstatehud", func_001E9540);
#ifdef SKIP_ASM
extern "C" unsigned short* strcpy1(unsigned short* dst, unsigned short* src);
extern "C" void func_001E92A8(void* self);
/**
 * Sets a HUD text line from a 16-bit string and lays it out.
 *
 * Copies @p str into the line's text (+0x4C) with strcpy1, stores @p font (+0x8) and @p scale (+0x10), then runs
 * func_001E92A8. cOVStateHUD1P_onCreateScreen uses it for a line built with USTR_sprintf.
 *
 * @param str  the 16-bit text; "@name" embeds a picture from D_004C8980
 * @param font  the font (a cFont*) to measure and draw with
 * @param scale  the line's x and y scale
 */
extern "C" void func_001E9540(void* self, unsigned short* str, int font, sVec2f* scale)
{
    strcpy1((unsigned short*)((char*)self + 0x4C), str);
    *(int*)((char*)self + 0x8) = font;
    *(sVec2f*)((char*)self + 0x10) = *scale;
    func_001E92A8(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001E95A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void func_00391E30_95A0(void* self, float x, float y, void* m) __asm__("func_00391E30");

struct sCol_E95A0 {
    float a, r, g, b;
};

struct sSize_E95A0 {
    float x, y;
};

struct sFont_E95A0 {
    char pad[0x30];
    float baseW;            // 0x30
    float baseH;            // 0x34
    float scaleW;           // 0x38
    float scaleH;           // 0x3C
    sCol_E95A0 color;       // 0x40
    sCol_E95A0 shadow;      // 0x50
};

struct sHudEntry_E95A0 {
    float w;
    void* p;
};

struct sHudImg_E95A0 {
    int f0;
    int f4;
    int tex;                // 0x08
    float w;                // 0x0C
    float h;                // 0x10
    float u0;               // 0x14
    float v0;               // 0x18
    float u1;               // 0x1C
    float v1;               // 0x20
};

struct sHudLine_E95A0 {
    float w;                // 0x00
    float h;                // 0x04
    sFont_E95A0* font;      // 0x08
    float lineH;            // 0x0C
    float sx;               // 0x10
    float sy;               // 0x14
    int count;              // 0x18
    sHudEntry_E95A0 entries[1]; // 0x1C
};

struct sPos_E95A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHudVert_E95A0 {
    float u, v, q, f0C;     // 0x00
    int r, g, b, a;         // 0x10
    sPos_E95A0 pos;         // 0x20
    sHudVert_E95A0() {}
};

static inline int texOf_E95A0(sHudImg_E95A0* i) { return i->tex; }

struct sVEnt_E95A0 { short delta; short index; void (*fn)(void*, int, sHudVert_E95A0*, int); };

static inline void setScale_E95A0(sFont_E95A0* f, const sSize_E95A0& s)
{
    f->scaleW = f->baseW * s.x;
    f->scaleH = f->baseH * s.y;
}

/**
 * Draws a laid-out HUD text line, pictures included, at a screen position.
 *
 * Does nothing for a line without segments. Sets the line font's Scale to BaseScale * line scale * @p scale, its Color
 * to @p color and its ShadowColor to the saved shadow colour with the alpha multiplied by @p color's alpha. @p alignX
 * / @p alignY place the line around @p pos: 0 centres it, a positive value puts its right (bottom) edge there, a
 * negative one its left (top) edge. It then walks the segments left to right, advancing pos[0] (the caller's array is
 * modified): text segments are drawn with func_00391E30 at y plus the line's text offset; picture segments are drawn as
 * a quad (the record's texture id into the current draw state's texture slot, its uvs, its size times @p scale, centred
 * vertically on the line, colour channels times 128) through GrMan's virtual slot 71 (cPS2GraphicsMan_RenderTriStrip).
 * Finally the shadow colour is restored and the font Scale reset to BaseScale.
 *
 * @param pos  screen position (x, y); updated in place, x ends after the last segment
 * @param scale  extra x and y scale
 * @param color  the colour (tARGB: alpha, red, green, blue)
 * @param alignX  horizontal pivot: 0 centre, > 0 right edge at pos, < 0 left edge
 * @param alignY  vertical pivot: 0 centre, > 0 bottom edge at pos, < 0 top edge
 */
extern "C" void func_001E95A0(sHudLine_E95A0* self, float* pos, float* scale, sCol_E95A0* color, int alignX, int alignY)
{
    if (self->count == 0) {
        return;
    }
    float alpha = color->a;
    sCol_E95A0 saved = self->font->shadow;
    sCol_E95A0 shadow = saved;
    sSize_E95A0 sz;
    float sx0 = scale[0];
    sz.x = self->sx * sx0;
    sz.y = self->sy * scale[1];
    shadow.a *= alpha;
    setScale_E95A0(self->font, sz);
    self->font->color = *color;
    self->font->shadow = shadow;
    if (alignX == 0) {
        float axisScale = scale[0];
        pos[0] -= self->w * axisScale * 0.5f;
    } else if (alignX > 0) {
        float axisScale = scale[0];
        pos[0] -= self->w * axisScale;
    }
    if (alignY == 0) {
        float axisScale = scale[1];
        pos[1] -= self->h * axisScale * 0.5f;
    } else if (alignY > 0) {
        float axisScale = scale[1];
        pos[1] -= self->h * axisScale;
    }
    float lineOff = self->lineH * scale[1];
    float yc = pos[1] + self->h * scale[1] * 0.5f;
    sHudVert_E95A0 verts[4];
    for (int i = 0; i < 4; i++) {
        verts[i].a = (int)(color->a * 128.0f);
        verts[i].r = (int)(color->r * 128.0f);
        verts[i].g = (int)(color->g * 128.0f);
        verts[i].b = (int)(color->b * 128.0f);
        verts[i].q = 1.0f;
    }
    sHudEntry_E95A0* entry = self->entries;
    for (int i = 0; i < self->count; i++, entry++) {
        if (!(i & 1)) {
            unsigned short* str = (unsigned short*)entry->p;
            if (*str != 0) {
                func_00391E30_95A0(self->font, pos[0], pos[1] + lineOff, str);
                float sx = scale[0];
                pos[0] += entry->w * sx;
            }
        } else {
            sHudImg_E95A0* img = (sHudImg_E95A0*)entry->p;
            sSize_E95A0 wh;
            float w = img->w * scale[0];
            float scaledHeight = img->h * scale[1];
            wh.x = w;
            wh.y = scaledHeight;
            float x0 = pos[0];
            float x1 = x0 + wh.x;
            float y0 = yc - wh.y * 0.5f;
            float y1 = y0 + wh.y;
            verts[0].u = img->u0;
            verts[0].v = img->v0;
            verts[1].u = img->u1;
            verts[1].v = img->v0;
            verts[2].u = img->u0;
            verts[2].v = img->v1;
            verts[3].u = img->u1;
            verts[3].v = img->v1;
            sPos_E95A0 corner;
            corner.x = x0;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[0].pos = corner;
            corner.x = x1;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[1].pos = corner;
            corner.x = x0;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[2].pos = corner;
            corner.x = x1;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[3].pos = corner;
            *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = texOf_E95A0(img);
            sVEnt_E95A0* vt = (sVEnt_E95A0*)GrMan->mVtbl;
            vt[71].fn((char*)GrMan + vt[71].delta, 4, verts, 0);
            pos[0] += wh.x;
        }
    }
    self->font->shadow = saved;
    sSize_E95A0 one;
    one.x = 1.0f;
    one.y = 1.0f;
    setScale_E95A0(self->font, one);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_cOVStateHUD1P);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_0039E2A0_9A30(void* self, int a1) __asm__("func_0039E2A0");
extern "C" void func_0020EAA0(void*);
extern char cOVStateHUD1P_vtbl[];

struct sElem_9A30 {
    void* operator new[](unsigned int, void* p) { return p; }
    char pad[8];
    char sub[0x84];
    sElem_9A30() { func_0020EAA0(sub); }
};

/**
 * Constructs the one-player HUD overlay state.
 *
 * Runs the cUIState constructor (func_0039E2A0) with @p pEngine, installs cOVStateHUD1P_vtbl at +0x8, constructs the
 * two 0x8C-byte HUD elements at +0x48 (func_0020EAA0 on each one's +0x8), sets +0x474 to -1, clears +0x3D4, +0x3D8,
 * +0x480 and +0x484 and sets the four scales +0x478, +0x47C, +0x488 and +0x48C to 1.0. The caller (gameload) passes a
 * third argument, 0, which is not used.
 *
 * @param hud     the memory for the state (this; the caller allocates 3224 bytes)
 * @param pEngine  the UI engine
 * @return @p hud
 */
// HUD overlay state constructor: base state, vtable, the two sub-panels and default scales.
extern "C" void* cOVStateHUD1P_cOVStateHUD1P(void* hud, int pEngine)
{
    char* self = (char*)hud;
    func_0039E2A0_9A30(self, pEngine);
    *(char**)(self + 8) = cOVStateHUD1P_vtbl;
    new (self + 0x48) sElem_9A30[2];
    *(int*)(self + 0x474) = -1;
    *(int*)(self + 0x3D4) = 0;
    *(int*)(self + 0x3D8) = 0;
    *(float*)(self + 0x478) = 1.0f;
    *(float*)(self + 0x47C) = 1.0f;
    *(int*)(self + 0x480) = 0;
    *(int*)(self + 0x484) = 0;
    *(float*)(self + 0x488) = 1.0f;
    *(float*)(self + 0x48C) = 1.0f;
    return hud;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_onCreateScreen);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "vtable.h"

int GetHashValue32(char* str);

extern "C" void* cBE_getBE();

// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");


extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

// (prototype from their definition at line 316)
extern "C" void func_001E94E0_reen(void* self, char* str, int a2, sVec2f* pos) __asm__("func_001E94E0");

// (prototype from their definition at line 328)
extern "C" void func_001E9540_reen(void* self, unsigned short* str, int a2, sVec2f* pos) __asm__("func_001E9540");

extern "C" void* cUITextureBank_getElementByHashname(void* self, int id);
extern "C" int func_003983F0(void* self, int key);
extern "C" void cOVStateHUD1P_calcElemPos_reen(void* self, float* out, float* pos, float* size, float* scale, int alignX, int alignY, int flags) __asm__("cOVStateHUD1P_calcElemPos");
extern "C" void func_0020EB50(void* self, void* screen, void* rect);
extern "C" void func_003DCA40(unsigned short* dst, const char* src);
extern "C" void USTR_sprintf(unsigned short* dst, const unsigned short* fmt, ...);
unsigned int BXrand();
extern char D_004A2108[];
extern char str_fefont[];
extern char str_hud_b[];
extern char str_hudfont[];
extern char str_nm3w[];
extern char str_nm2w[];
extern char str_nm1w[];
extern char str_go[];
extern char str_switch[];
extern char str_nm4w[];
extern char str_nm5w[];
extern char str_nm6w[];
extern char str_st_w[];
extern char str_nd_w[];
extern char str_rd_w[];
extern char str_th_w[];
extern char str_slash_b[];
extern char str_Sphere[];
extern char str_00[];
extern char str_11_b[];
extern char str_pct_s_at_skip_pct_s[];
extern char str_kT_OVRCMNPress[];
extern char str_kT_OVRCMNToSkip[];
extern int D_004C88D8[26];
extern int D_004C8940[5][3];
extern char* D_004A21E4;
extern char* D_004A21EC;
extern char* D_004A21D0[2];
extern char* D_004A21D8;
extern sVec2f D_004A4F50;
extern sVec2f D_004A4F58;

// On Tour's t2Vector derives from t2vect, whose float& x() / y() accessors the ctor assigns through; X()/Y()
// stand in for them here (the fields keep their names). onRender2D's codegen needs the accessor form.
struct t2Vector {
    float x, y;
    float& X() { return x; }
    float& Y() { return y; }
    t2Vector() {}
    t2Vector(float x_, float y_) { X() = x_; Y() = y_; }
};

// HUD element layout record (0x24 bytes, const tables in .rodata).
struct sHudElem_9AD0 {
    short x, y;         // 0x00
    short w, h;         // 0x04
    float sx, sy;       // 0x08
    float col[4];       // 0x10
    char alignX;        // 0x20
    char alignY;        // 0x21
    char blend;         // 0x22
};
extern const sHudElem_9AD0 D_00477000;
extern const sHudElem_9AD0 D_00477B40;
extern const sHudElem_9AD0 D_00476940;
extern const sHudElem_9AD0 D_00477480;
extern const sHudElem_9AD0 D_00477048;
extern const sHudElem_9AD0 D_00477B88;
void getElemPos_9AD0(const sHudElem_9AD0* e, t2Vector* out) __asm__("tHUDDef_getPos");
void getElemSize_9AD0(const sHudElem_9AD0* e, t2Vector* out, void* def) __asm__("tHUDDef_getSize");
void getElemScale_9AD0(const sHudElem_9AD0* e, t2Vector* out) __asm__("tHUDDef_getScale__FPvT0");

struct tARGB_9AD0 {
    float r, g, b, a;
    tARGB_9AD0() {}
    tARGB_9AD0(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

struct tRect_9AD0 {
    float x, y, w, h;
};

struct sUIItem_9AD0 {
    int handle;     // 0x00
    int f04;
    int f08;
    float y0;           // 0x0C
    float x0;           // 0x10
    float x1;           // 0x14
    float y1;           // 0x18
    float getX0() const { return x0; }
    float getY0() const { return y0; }
    float getX1() const { return x1; }
    float getY1() const { return y1; }
};

struct sTok_9AD0 {
    int hash;       // 0x00
    int name;       // 0x04
    int tex;        // 0x08
    float width;    // 0x0C
    float height;   // 0x10
    t2Vector uv0;   // 0x14
    t2Vector uv1;   // 0x1C
};
extern sTok_9AD0 D_004C8980_9AD0[] __asm__("D_004C8980");

class cBeing_9AD0 {
public:
    virtual ~cBeing_9AD0();
    virtual const void* getVelocity() const;
    virtual int isIntersecting(void* rec) const;
    virtual void applyBoost(const void* impulse);
    virtual const void* getPosition() const;
    virtual const void* getTransform() const;
    virtual int getAIIndex() const;
};

struct sRiderPanel {
    int aiIndex;        // 0x00
    int rider;          // 0x04
    char trophy[0x3C];  // 0x08
    int f44;            // 0x44
    float f48;          // 0x48
    float f4C;          // 0x4C
    int f50;            // 0x50
    tARGB_9AD0 col;     // 0x54
    float f64;          // 0x64
    float f68;          // 0x68
    int f6C;            // 0x6C
    float f70;          // 0x70
    int f74;            // 0x74
    float f78;          // 0x78
    int f7C;            // 0x7C
    int f80;            // 0x80
    int f84;            // 0x84
    int f88;            // 0x88
};

struct sSlot_9AD0 {
    int pad0[3];
    int id;
    int pad10[2];
};

struct tBarConfig {
    float scale;
    int count;
};

struct sGameOpts_9AD0 {
    unsigned int flags;         // 0x000: bits 22-24 = language
    char pad[0x284];
};
extern sGameOpts_9AD0 cBEOptionInterface_mOption;

// One screen dimension in pixels, set from the unsigned value the renderer reports.
// (The stores are aggregate-member stores: the target schedules the GrMan reload above them.)
struct sScreenDim_9AD0 {
    float pixels;
    void set(unsigned int v) { pixels = v; }
};
struct sHUD1P {
    char pad0[0xC];
    int hash;                   // 0x00C
    char* engine;               // 0x010
    char pad14[0x48 - 0x14];
    sRiderPanel panels[2];      // 0x048
    int f160, f164, f168;       // 0x160
    float w16C, w170, w174, w178; // 0x16C
    int f17C, f180, f184, f188; // 0x17C
    unsigned f18C;              // 0x18C
    float f190;                 // 0x190
    int f194, f198;             // 0x194
    char pad19C[0x3A4 - 0x19C];
    int f3A4, f3A8;             // 0x3A4
    char pad3AC[0x3B4 - 0x3AC];
    unsigned f3B4;              // 0x3B4
    char pad3B8[0x3C0 - 0x3B8];
    float f3C0;                 // 0x3C0
    int f3C4, f3C8, f3CC;       // 0x3C4
    float f3D0;                 // 0x3D0
    char pad3D4[0x3E0 - 0x3D4];
    sSlot_9AD0 slots[3];        // 0x3E0
    int bank428;                // 0x428
    void* font;                 // 0x42C
    char* fontName;             // 0x430
    void* w434, *w438, *w43C, *w440; // 0x434
    void* w444[11];             // 0x444
    void* w470;                 // 0x470
    int mode;                   // 0x474
    sScreenDim_9AD0 scrW, scrH; // 0x478
    char pad480[0x490 - 0x480];
    unsigned f490;              // 0x490
    void* icons[26];            // 0x494
    void* boost[5][3];          // 0x4FC
    void* w538;                 // 0x538
    tBarConfig bars[4];         // 0x53C
    int rnd;                    // 0x55C
    char box560[0x14C];         // 0x560
    char box6AC[0x14C];         // 0x6AC
    char box7F8[0x14C];         // 0x7F8
    char pad944[0xA44 - 0x944];
    int fA44;                   // 0xA44
    char padA48[0xB48 - 0xA48];
    char boxB48[0x14C];         // 0xB48
    float fC94;                 // 0xC94
};

class cGfx_9AD0 {
public:
    char pad[0xF50];
    int mode;           // 0xF50
};


static inline void* findElem_9AD0(char* engine, char* name)
{
    int hash = GetHashValue32(name);
    return cUITextureBank_getElementByHashname(engine + 0x58, hash);
}
static inline int findFont_9AD0(void* bank, char* name) { return func_003983F0(bank, GetHashValue32(name)); }

struct sRace_9AD0 {
    char pad0[0x40];
    char* riders[15];       // 0x40
    int numRiders;          // 0x7C
};
static inline sRace_9AD0* getRace_9AD0() { return (sRace_9AD0*)SSXApp->Game->AI; }

// Segment count of a vertical bar: one 23-pixel segment per 23 pixels of height d above the base (rounded).
static inline void calcBarSegments_9AD0(int& n, float d)
{
    if (d > 23.0f) {
        n = (int)(d * (1.0f / 23.0f));
        if (((float)n + 0.5f) * 23.0f < d) {
            n++;
        }
    } else {
        n = 1;
    }
}

// A vertical bar of 23-pixel segments over a 54-pixel base: segment count and scale for an element height h.
static inline void setupVertBar_9AD0(tBarConfig* b, const float& h)
{
    calcBarSegments_9AD0(b->count, h - 54.0f);
    b->scale = h / ((float)b->count * 23.0f + 54.0f);
}

// Localised string by name from the game's text table (SSXApp + 0x8C; vptr at 0x4, slot 4 = lookup by hash).
static inline unsigned short* getLocString_9AD0(char* id)
{
    char* o = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = *(sVEntry<unsigned short*(void*, int)>**)(o + 4);
    o += vt[4].delta;
    int h = GetHashValue32(id);
    return vt[4].fn(o, h);
}

/**
 * Sets up the single-player HUD: resolves its textures, fonts and elements, resets its timers, and gives each
 * local rider a panel.
 *
 * Names the state "hud" and fills the token table D_004C8980 with texture handles and UVs. Each local rider
 * (cRider::mIsLocal) gets the next of two panels with its rider and AI index; the panels' display objects are
 * placed from layout D_00477000 (one rider) or D_00477B40 (two, the second mirrored).
 * Looks up "fefont" / "hudfont", the countdown (nm3w, nm2w, nm1w, go) and "switch" elements, the place digits
 * nm1w-nm6w with st_w / nd_w / rd_w / th_w and "slash", "Sphere", 26 icons and 5x3 boost textures; sizes the
 * bars; picks ":" or "'" as the time separator by the language bits 22-24 of cBEOptionInterface_mOption and measures "00",
 * "-" and "11". Builds the hint boxes: a random Uber trick (L1 or R1 + Square, BXrand() & 1 kept at +0x55C),
 * RECOVER and "%s @skip %s" (kT_OVRCMNPress, kT_OVRCMNToSkip).
 *
 * @param vself  the HUD state (this)
 */
extern "C" void cOVStateHUD1P_onCreateScreen(void* vself)
{
    sHUD1P* self = (sHUD1P*)vself;
    self->hash = GetHashValue32(str_hud_b);
    sTok_9AD0* entry = D_004C8980_9AD0;
    while (entry->hash != 0) {
        sUIItem_9AD0* it = (sUIItem_9AD0*)cUITextureBank_getElementByHashname(self->engine + 0x58, entry->name);
        entry->tex = it->handle;
        entry->uv0 = t2Vector(it->getX0(), it->getY0());
        entry->uv1 = t2Vector(it->getX1(), it->getY1());
        entry++;
    }
    self->f3C4 = 80;
    self->f3B4 = 0xF0000000;
    self->f3D0 = -1.0f;
    self->f3C0 = -1.0f;
    self->f3C8 = 0;
    self->f190 = -1.0f;
    self->f194 = 0;
    self->f198 = 0;
    self->f3A4 = 0;
    self->f3A8 = 0;
    self->fA44 = 0;
    self->fC94 = -1.0f;
    self->f3CC = 0;
    for (int i = 0; i < 2; i++) {
        self->panels[i].f44 = -1;
        self->panels[i].aiIndex = -1;
        self->panels[i].rider = -1;
        self->panels[i].f48 = 1.0f;
        self->panels[i].f4C = -1.0f;
        tARGB_9AD0 white;
        white.r = 1.0f;
        white.g = 1.0f;
        white.b = 1.0f;
        white.a = 1.0f;
        self->panels[i].col = white;
        self->panels[i].f64 = -1.0f;
        self->panels[i].f68 = 0.5f;
        self->panels[i].f6C = -1;
        self->panels[i].f70 = -1.0f;
        self->panels[i].f74 = 0;
        self->panels[i].f78 = -1.0f;
        self->panels[i].f80 = 0;
        self->panels[i].f84 = 0;
        self->panels[i].f88 = 0;
        self->panels[i].f7C = 0;
    }
    self->f160 = 0;
    self->f164 = 0;
    self->f168 = 0;
    for (int i = 0; i < 3; i++) {
        self->slots[i].id = -1;
    }
    signed char n = 0;
    for (int i = 0; i < getRace_9AD0()->numRiders; i++) {
        char* r = getRace_9AD0()->riders[i];
        if (*(int*)(*(char**)(r + 0x18) + 0x87C) != 0) {
            self->panels[n].rider = i;
            self->panels[n].aiIndex = ((cBeing_9AD0*)(*(char**)(r + 0x18) + 0x6C0))->getAIIndex();
            n++;
        }
    }
    tRect_9AD0 scr;
    scr.x = 0.0f;
    scr.y = 0.0f;
    scr.w = 640.0f;
    scr.h = 480.0f;
    if (n == 1) {
        t2Vector a;
        t2Vector size;
        t2Vector sc;
        getElemPos_9AD0(&D_00477000, &a);
        getElemSize_9AD0(&D_00477000, &size, 0);
        getElemScale_9AD0(&D_00477000, &sc);
        size.x *= sc.x;
        size.y *= sc.y;
        t2Vector outPos;
        outPos.x = 1.0f;
        outPos.y = 1.0f;
        sc = outPos;
        cOVStateHUD1P_calcElemPos_reen(self, &outPos.x, &a.x, &size.x, &sc.x, D_00477000.alignX, D_00477000.alignY, 0);
        tRect_9AD0 r;
        r.x = outPos.x;
        r.y = outPos.y;
        r.w = size.x;
        r.h = size.y;
        func_0020EB50(self->panels[0].trophy, &scr, &r);
        func_0020EB50(self->panels[1].trophy, &scr, &r);
    } else {
        t2Vector a;
        t2Vector size;
        t2Vector sc;
        getElemPos_9AD0(&D_00477B40, &a);
        getElemSize_9AD0(&D_00477B40, &size, 0);
        getElemScale_9AD0(&D_00477B40, &sc);
        size.x *= sc.x;
        size.y *= sc.y;
        t2Vector outPos;
        outPos.x = 1.0f;
        outPos.y = 1.0f;
        sc = outPos;
        cOVStateHUD1P_calcElemPos_reen(self, &outPos.x, &a.x, &size.x, &sc.x, D_00477B40.alignX, D_00477B40.alignY, 0);
        tRect_9AD0 trophyRect0;
        trophyRect0.x = outPos.x;
        trophyRect0.y = outPos.y;
        trophyRect0.w = size.x;
        trophyRect0.h = size.y;
        func_0020EB50(self->panels[0].trophy, &scr, &trophyRect0);
        cOVStateHUD1P_calcElemPos_reen(self, &outPos.x, &a.x, &size.x, &sc.x, D_00477B40.alignX, D_00477B40.alignY, 1);
        tRect_9AD0 trophyRect1;
        trophyRect1.x = outPos.x;
        trophyRect1.y = outPos.y;
        trophyRect1.w = size.x;
        trophyRect1.h = size.y;
        func_0020EB50(self->panels[1].trophy, &scr, &trophyRect1);
    }
    cBE_getInterface_Fv(cBE_getBE(), 4);
    self->bank428 = findFont_9AD0(*(void**)(self->engine + 8), str_fefont);
    self->font = (void*)findFont_9AD0(*(void**)(self->engine + 8), str_hudfont);
    self->w440 = findElem_9AD0(self->engine, str_nm3w);
    self->w43C = findElem_9AD0(self->engine, str_nm2w);
    self->w438 = findElem_9AD0(self->engine, str_nm1w);
    self->w434 = findElem_9AD0(self->engine, str_go);
    self->w470 = findElem_9AD0(self->engine, str_switch);
    for (int i = 0; i < 26; i++) {
        self->icons[i] = cUITextureBank_getElementByHashname(self->engine + 0x58, D_004C88D8[i]);
    }
    cPS2GraphicsMan* gfx = GrMan;
    self->mode = gfx->_EffectTexMap[0];
    self->f490 = 0xDEEDDEED;
    {
        sVEntry<unsigned int(void*, int)>* vt = (sVEntry<unsigned int (void*, int)>*)GrMan->mVtbl;
        self->scrW.set(vt[56].fn((char*)GrMan + vt[56].delta, self->mode));
    }
    {
        sVEntry<unsigned int(void*, int)>* vt = (sVEntry<unsigned int (void*, int)>*)GrMan->mVtbl;
        self->scrH.set(vt[57].fn((char*)GrMan + vt[57].delta, self->mode));
    }
    self->w444[0] = findElem_9AD0(self->engine, str_nm1w);
    self->w444[1] = findElem_9AD0(self->engine, str_nm2w);
    self->w444[2] = findElem_9AD0(self->engine, str_nm3w);
    self->w444[3] = findElem_9AD0(self->engine, str_nm4w);
    self->w444[4] = findElem_9AD0(self->engine, str_nm5w);
    self->w444[5] = findElem_9AD0(self->engine, str_nm6w);
    self->w444[6] = findElem_9AD0(self->engine, str_st_w);
    self->w444[7] = findElem_9AD0(self->engine, str_nd_w);
    self->w444[8] = findElem_9AD0(self->engine, str_rd_w);
    self->w444[9] = findElem_9AD0(self->engine, str_th_w);
    self->w444[10] = findElem_9AD0(self->engine, str_slash_b);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            self->boost[i][j] = cUITextureBank_getElementByHashname(self->engine + 0x58, D_004C8940[i][j]);
        }
    }
    self->w538 = findElem_9AD0(self->engine, str_Sphere);
    {
    t2Vector a;
    getElemSize_9AD0(&D_00476940, &a, 0);
    setupVertBar_9AD0(&self->bars[0], a.y);
    getElemSize_9AD0(&D_00477480, &a, 0);
    setupVertBar_9AD0(&self->bars[1], a.y);
    getElemSize_9AD0(&D_00477048, &a, 0);
    self->bars[2].count = 1;
    self->bars[2].scale = 1.0f;
    a.x -= 23.0f;
    if (a.x - 18.0f < 0.0f) {
        self->bars[2].count = 0;
        self->bars[2].scale = a.x * (1.0f / 18.0f);
    }
    getElemSize_9AD0(&D_00477B88, &a, 0);
    self->bars[3].count = 1;
    self->bars[3].scale = 1.0f;
    a.x -= 23.0f;
    if (a.x - 18.0f < 0.0f) {
        self->bars[3].count = 0;
        self->bars[3].scale = a.x * (1.0f / 18.0f);
    }
    }
    {
    sGameOpts_9AD0 opts = cBEOptionInterface_mOption;
    switch ((opts.flags >> 22) & 7) {
    case 0:
    case 2:
        self->fontName = D_004A21E4;
        break;
    case 1:
    case 3:
        self->fontName = D_004A21EC;
        break;
    }
    }
    self->w16C = cFont_textExtent(self->font, self->fontName, 0, 0, 1.0f, 1.0f);
    self->w170 = cFont_textExtent(self->font, str_00, 0, 0, 1.0f, 1.0f);
    self->w174 = cFont_textExtent(self->font, D_004A2108, 0, 0, 1.0f, 1.0f);
    self->w178 = (self->w170 - cFont_textExtent(self->font, str_11_b, 0, 0, 1.0f, 1.0f)) * 0.25f;
    self->f17C = 0;
    self->f180 = 0;
    self->f184 = 0;
    self->f188 = 0;
    self->f18C = 0xFFFFFFFF;
    self->rnd = BXrand() & 1;
    func_001E94E0_reen(self->box560, D_004A21D0[self->rnd], self->bank428, &D_004A4F50);
    func_001E94E0_reen(self->box6AC, D_004A21D8, self->bank428, &D_004A4F50);
    func_001E94E0_reen(self->box7F8, D_004A21D8, self->bank428, &D_004A4F50);
    unsigned short fmt[32];
    func_003DCA40(fmt, str_pct_s_at_skip_pct_s);
    unsigned short* name = getLocString_9AD0(str_kT_OVRCMNPress);
    unsigned short* skip = getLocString_9AD0(str_kT_OVRCMNToSkip);
    unsigned short buf[32];
    USTR_sprintf(buf, fmt, name, skip);
    func_001E9540_reen(self->boxB48, buf, self->bank428, &D_004A4F58);
}
#endif

INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_onUpdate);

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_handleEvent);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned short* strcpy1(unsigned short* dst, unsigned short* src);
extern "C" void* func_002B40F0(void* self, int i);

/**
 * Handles a notification sent to the HUD (virtual slot 24): sets its flags, counters and message texts.
 *
 * 0 / 1 set and clear +0x3D4 (sent by the script data manager); 2 / 3 / 4 reset, raise and lower the
 * open-overlay count +0x3D8 (cOVState_onCreateScreen / onDestroyScreen send 3 / 4); 5 rechecks the current
 * song and restarts the Now Playing popup on a new title; 6 start gate opened; 7 restart reset; 8 / 9 start
 * and stop the mail icon (+0x160); 12 / 13 and 14 / 15 set and clear the message texts at +0x944 and +0xA48
 * (flags 0x20000 / 0x40000).
 *
 * @param msg  the notification code
 * @param str  the text for codes 12 and 14 (On Tour pParam)
 * @return always 0
 */
// HUD overlay message handler: flags, counters, the two message strings and the track-name hash.
extern "C" int cOVStateHUD1P_handleEvent(char* self, int msg, unsigned short* str)
{
    switch (msg) {
    case 0:
        *(int*)(self + 0x3D4) = 1;
        break;
    case 1:
        *(int*)(self + 0x3D4) = 0;
        break;
    case 2:
        *(int*)(self + 0x3D8) = 0;
        break;
    case 3:
        *(int*)(self + 0x3D8) += 1;
        break;
    case 4:
        if (*(int*)(self + 0x3D8) > 0) {
            *(int*)(self + 0x3D8) -= 1;
        }
        break;
    case 12:
        *(unsigned int*)(self + 0x3CC) |= 0x20000;
        *(int*)(self + 0xA44) = 0;
        strcpy1((unsigned short*)(self + 0x944), str);
        break;
    case 13:
        *(unsigned short*)(self + 0x944) = 0;
        *(int*)(self + 0xA44) = 0;
        *(unsigned int*)(self + 0x3CC) &= ~0x20000;
        break;
    case 14:
        *(unsigned int*)(self + 0x3CC) |= 0x40000;
        strcpy1((unsigned short*)(self + 0xA48), str);
        break;
    case 15:
        *(unsigned short*)(self + 0xA48) = 0;
        *(unsigned int*)(self + 0x3CC) &= ~0x40000;
        break;
    case 5: {
        *(unsigned int*)(self + 0x3CC) &= ~0x10000;
        char* name = (char*)func_002B40F0((char*)cAudioInterface_bxAudio() + 0x118, -1);
        unsigned int hash = 0xF0000000;
        if (name != 0 && *name != 0) {
            *(unsigned int*)(self + 0x3CC) |= 0x10000;
            hash = GetHashValue32(name);
        }
        if (!(*(unsigned int*)(self + 0x3CC) & 0x10000) || hash != *(unsigned int*)(self + 0x3B4)) {
            *(int*)(self + 0x3B4) = hash;
            *(float*)(self + 0x190) = -1.0f;
            *(int*)(self + 0x198) = 0;
            *(int*)(self + 0x3A4) = 0;
            *(int*)(self + 0x3A8) = 0;
            *(int*)(self + 0x194) = 0;
        }
        break;
    }
    case 6:
        *(int*)(self + 0x3C8) = 0;
        *(int*)(self + 0x3C0) = 0;
        *(int*)(self + 0x3C4) = 8;
        break;
    case 7:
        *(unsigned int*)(self + 0x3CC) &= 0xFFBDC3FF;
        *(int*)(self + 0x3C4) = 80;
        *(int*)(self + 0x3C0) = 0;
        *(int*)(self + 0xA44) = 0;
        *(unsigned short*)(self + 0x944) = 0;
        *(unsigned int*)(self + 0x18C) = 0xFFFFFFFF;
        break;
    case 8:
        *(int*)(self + 0x168) = 0;
        *(int*)(self + 0x160) = 1;
        break;
    case 9:
        *(int*)(self + 0x160) = 0;
        *(int*)(self + 0x168) = 0;
        *(int*)(self + 0x164) = 0;
        break;
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_onRender2D);

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_calcElemPos);
#ifdef SKIP_ASM
extern "C" float cOVStateHUD1P_calcElemPosX(void* self, int align, int flags, float pos, float size, float scale);
extern "C" float cOVStateHUD1P_calcElemPosY(void* self, int align, float pos, float size, float scale);

/**
 * Computes the screen position of a HUD element's top-left corner from its anchor, size, scale and pivots.
 *
 * out.x = cOVStateHUD1P_calcElemPosX: with bit 0 of @p flags it mirrors x about 640 and, unless bit 1 is also set, flips the
 * horizontal pivot; pivot 0 then subtracts half the scaled width, a positive pivot the whole scaled width. out.y =
 * cOVStateHUD1P_calcElemPosY: the same for the vertical pivot, without mirroring.
 *
 * @param out  receives the corner (x, y)
 * @param pos  the element's anchor position
 * @param size  its unscaled size
 * @param scale  its scale
 * @param alignX  horizontal pivot (On Tour pivotX): 0 centre, > 0 right edge at the anchor, < 0 left edge
 * @param alignY  vertical pivot (On Tour pivotY), the same way
 * @param flags  reflect bits: 1 mirrors x about 640, 2 then keeps the horizontal pivot unflipped
 */
extern "C" void cOVStateHUD1P_calcElemPos(void* self, float* out, float* pos, float* size, float* scale, int alignX, int alignY, int flags)
{
    out[0] = cOVStateHUD1P_calcElemPosX(self, alignX, flags, pos[0], size[0], scale[0]);
    out[1] = cOVStateHUD1P_calcElemPosY(self, alignY, pos[1], size[1], scale[1]);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderBitmap);
#ifdef SKIP_ASM
struct sPos_001F1190 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHudVert_001F1190 {
    float u, v, q, f0C;     // 0x00
    int r, g, b, a;         // 0x10
    sPos_001F1190 pos;      // 0x20
    sHudVert_001F1190() {}
};

static inline int texOf_001F1190(cOVStateHUDElem* e) { return e->field_0x0; }

struct sVEnt_001F1190 { short delta; short index; void (*fn)(void*, int, sHudVert_001F1190*, int); };

/**
 * Draws a texture element as a coloured quad.
 *
 * Does nothing when @p elem is 0. Puts the element's texture (word +0x0) into the current draw state's texture slot,
 * colours four vertices with @p color times 128 (alpha, red, green, blue), takes the uvs from the element's left, top,
 * right and bottom (+0x10, +0xC, +0x14, +0x18: On Tour cTextureElement::mLeft, mTop, mRight, mBottom; this unit's
 * cOVStateHUDElem view calls them x, rangeMax, y, z), spans @p pos to @p pos + @p size * @p scale and draws the strip
 * through GrMan's virtual slot 71 (cPS2GraphicsMan_RenderTriStrip).
 *
 * @param elem  the texture element (typed cOVStateHUDElem* here), or 0
 * @param pos  the top-left corner
 * @param size  the unscaled size
 * @param scale  the scale
 * @param color  the colour (tARGB, 1.0 = full)
 */
extern "C" void cOVStateHUD1P_renderBitmap(void* self, cOVStateHUDElem* elem, float* pos, float* size, float* scale, float* color)
{
    if (elem != 0) {
        *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = texOf_001F1190(elem);
        sHudVert_001F1190 verts[4];
        int alpha = (int)(color[0] * 128.0f);
        int red = (int)(color[1] * 128.0f);
        int green = (int)(color[2] * 128.0f);
        int blue = (int)(color[3] * 128.0f);
        for (int i = 0; i < 4; i++) {
            verts[i].a = alpha;
            verts[i].r = red;
            verts[i].g = green;
            verts[i].b = blue;
            verts[i].q = 1.0f;
        }
        float x0 = pos[0];
        float y0 = pos[1];
        float x1 = x0 + size[0] * scale[0];
        float y1 = y0 + size[1] * scale[1];
        verts[0].u = elem->x;
        verts[0].v = elem->rangeMax;
        verts[1].u = elem->y;
        verts[1].v = elem->rangeMax;
        verts[2].u = elem->x;
        verts[2].v = elem->z;
        verts[3].u = elem->y;
        verts[3].v = elem->z;
        sPos_001F1190 corner;
        corner.x = x0;
        corner.y = y0;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[0].pos = corner;
        corner.x = x1;
        corner.y = y0;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[1].pos = corner;
        corner.x = x0;
        corner.y = y1;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[2].pos = corner;
        corner.x = x1;
        corner.y = y1;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[3].pos = corner;
        sVEnt_001F1190* vt = (sVEnt_001F1190*)GrMan->mVtbl;
        vt[71].fn((char*)GrMan + vt[71].delta, 4, verts, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderQuad);
#ifdef SKIP_ASM
struct sPos_001F1338 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHudVert_001F1338 {
    float u, v, q, f0C;     // 0x00
    int r, g, b, a;         // 0x10
    sPos_001F1338 pos;      // 0x20
    sHudVert_001F1338() {}
};

struct sVEnt_001F1338 { short delta; short index; void (*fn)(void*, int, sHudVert_001F1338*, int); };

/**
 * Draws an untextured coloured quad.
 *
 * Spans @p pos to @p pos + @p size * @p scale with texture -1 and draws it through GrMan's virtual slot 71
 * (cPS2GraphicsMan_RenderTriStrip). The alpha is scaled by 128 but red, green and blue by 255, unlike cOVStateHUD1P_renderBitmap
 * (all by 128); on the PS2's 0x80 = 1.0 colour scale that doubles the colour (likely a bug, or a deliberate
 * brightening).
 *
 * @param pos  the top-left corner
 * @param size  the unscaled size
 * @param scale  the scale
 * @param color  the colour (tARGB)
 */
extern "C" void cOVStateHUD1P_renderQuad(void* self, float* pos, float* size, float* scale, float* color)
{
    sHudVert_001F1338 verts[4];
    int alpha = (int)(color[0] * 128.0f);
    int red = (int)(color[1] * 255.0f);
    int green = (int)(color[2] * 255.0f);
    int blue = (int)(color[3] * 255.0f);
    for (int i = 0; i < 4; i++) {
        verts[i].a = alpha;
        verts[i].r = red;
        verts[i].g = green;
        verts[i].b = blue;
        verts[i].q = 1.0f;
    }
    float x0 = pos[0];
    float y0 = pos[1];
    float x1 = x0 + size[0] * scale[0];
    float y1 = y0 + size[1] * scale[1];
    sPos_001F1338 corner;
    corner.x = x0;
    corner.y = y0;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[0].pos = corner;
    corner.x = x1;
    corner.y = y0;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[1].pos = corner;
    corner.x = x0;
    corner.y = y1;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[2].pos = corner;
    corner.x = x1;
    corner.y = y1;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[3].pos = corner;
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = -1;
    sVEnt_001F1338* vt = (sVEnt_001F1338*)GrMan->mVtbl;
    vt[71].fn((char*)GrMan + vt[71].delta, 4, verts, 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderExplodeText);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);
struct sGlyphTable_1F14B0;
extern "C" void cFont_text(void* self, float x, float y, const char* str);

struct sGlyph_1F14B0 {
    unsigned short code;
    unsigned char lead;
    char pad_0x3[6];
    signed char width;
    char pad_0xA[2];
};

struct sGlyphTable_1F14B0 {
    int count;
    sGlyph_1F14B0* glyphs;
    sGlyph_1F14B0* fallback;
    int first;
    int last;
    char pad14[0x1C];
    float f30;
    float f34;
    float f38;
    float f3C;
};

static inline sGlyph_1F14B0* findGlyph_1F14B0(sGlyphTable_1F14B0* t, int c)
{
    if (c < t->first)
        return 0;
    if (c <= t->last)
        return t->glyphs + c - t->first;
    int lo = t->last - t->first;
    int hi = t->count;
    do {
        int mid = (lo + hi) >> 1;
        sGlyph_1F14B0* g = &t->glyphs[mid];
        if (c == g->code)
            return g;
        if (c < g->code)
            hi = mid;
        else
            lo = mid + 1;
    } while (lo < hi);
    return t->fallback;
}

struct sVec2_1F14B0 {
    float x, y;
};

/**
 * Draws a string one character at a time, each flung away from its place by an amount ("exploding" text).
 *
 * Character i is drawn with cFont_text at (@p x, @p y) plus @p amp times a per-character offset: up by (2 * d * ((c &
 * 0x7F) >> 1) * 5) & 0x7F and sideways by (d * (c & 0xF) * 4) & 0x7F, where c is the character code and d its distance
 * from the middle character (the middle counts as 1); characters left of the middle move left. x then advances by the
 * glyph's width plus x offset (tGlyph width and xoffset) times the font's Scale / BaseScale. cOVStateHUD1P_onRender2D
 * is the only caller.
 *
 * @param str  the text
 * @param font  the font (a cFont)
 * @param amp  how far the characters fly out (0 draws the plain text)
 * @param x  the left edge
 * @param y  the top
 */
extern "C" void cOVStateHUD1P_renderExplodeText(void* self, const char* str, sGlyphTable_1F14B0* font, float amp, float x, float y)
{
    sVec2_1F14B0 sc;
    sc.x = font->f38 / font->f30;
    sc.y = font->f3C / font->f34;
    int len = strlen(str);
    char buf[2];
    buf[1] = 0;
    for (int i = 0; i < len; i++) {
        buf[0] = str[i];
        int centreDistance = i - (len >> 1);
        int neg = 0;
        if (centreDistance == 0) {
            centreDistance = 1;
        } else if (centreDistance < 0) {
            neg = 1;
            centreDistance = -centreDistance;
        }
        unsigned char ch = buf[0];
        int doubledDistance = centreDistance * 2;
        int half = (ch & 0x7F) >> 1;
        int riseOffset = (doubledDistance * half * 5) & 0x7F;
        int sideOffset = (centreDistance * (ch & 0xF) * 4) & 0x7F;
        float oy = (float)riseOffset;
        float ox = (float)sideOffset;
        oy = -oy;
        if (neg) {
            ox = -ox;
        }
        cFont_text(font, x + amp * ox, y + amp * oy, buf);
        sGlyph_1F14B0* glyph = findGlyph_1F14B0(font, buf[0]);
        x += (float)(glyph->width + glyph->lead) * sc.x;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderTime1);
#ifdef SKIP_ASM
struct sColor_001F16C0 {
    float r, g, b, a;
};
struct sHudElemDef_001F16C0 {
    char pad[0x20];
    signed char align;      // 0x20
    signed char flags;      // 0x21
    char pad22[2];
};
extern "C" void cOVStateHUD1P_renderTime(void* self, int a1, int a2, int a3, int a4, int a5, float* pos, float* scale, sColor_001F16C0* color, int align, int flags, int a11);

/**
 * Draws a clock time placed, scaled and coloured by a HUD element definition.
 *
 * Takes element @p idx of @p defs: its position (tHUDDef_getPos) plus @p posOff when given, its scale (tHUDDef_getScale)
 * times @p scaleMul when given, and its colour (tHUDDef_getColour) unless @p color overrides it, then passes these, the
 * element's pivots (+0x20, +0x21) and @p reflect to cOVStateHUD1P_renderTime.
 *
 * @param hour  the first field (hours)
 * @param minute  the second field (minutes)
 * @param sec  the third field (seconds)
 * @param secFrac  hundredths, drawn with flag bit 0
 * @param timeRenderFlags  bit 0 hundredths, bit 1 a "-" sign, bit 2 a "+" sign
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx  the element index
 * @param reflect  reflect bits for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param scaleMul  extra scale, or 0
 * @param posOff  offset added to the position, or 0
 * @param color  colour override (tARGB), or 0 for the element's
 */
extern "C" void cOVStateHUD1P_renderTime1(void* self, int hour, int minute, int sec, int secFrac, int timeRenderFlags, sHudElemDef_001F16C0* defs, int idx, int reflect, float* scaleMul, float* posOff, sColor_001F16C0* color)
{
    float pos[4];
    float scale[4];
    sColor_001F16C0 col;
    sHudElemDef_001F16C0* elemDef = &defs[idx];
    tHUDDef_getPos(elemDef, pos);
    tHUDDef_getScale(elemDef, scale);
    if (scaleMul != 0) {
        scale[0] *= scaleMul[0];
        scale[1] *= scaleMul[1];
    }
    if (posOff != 0) {
        pos[0] += posOff[0];
        pos[1] += posOff[1];
    }
    if (color == 0) {
        tHUDDef_getColour(elemDef, &col);
    } else {
        col = *color;
    }
    cOVStateHUD1P_renderTime(self, hour, minute, sec, secFrac, timeRenderFlags, pos, scale, &col, defs[idx].align, defs[idx].flags, reflect);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderTime);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFont_textf(void* font, float x, float y, const char* fmt, ...);
extern char D_004A2100[];
extern char D_004A2108[];
extern char str_pct_02d[];
extern char str_pct_02d_b[];

struct sFont_001F1840 {
    char pad0[0x14];
    int count;          // 0x14
    char pad18[0x18];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColor_001F16C0 color; // 0x40
};

struct sHUD_001F1840 {
    char pad0[0x16C];
    float digitW;       // 0x16C
    float f170;         // 0x170
    float f174;         // 0x174
    float oneW;         // 0x178
    char pad17C[0x42C - 0x17C];
    sFont_001F1840* font; // 0x42C
    const char* sep;    // 0x430
};

static inline float oneAdj_001F1840(const char* s, float w)
{
    float adj = 0.0f;
    if (s[0] == '1') adj = w;
    if (s[1] == '1') adj += w;
    return adj;
}

/**
 * Draws a clock as two-digit fields joined by the time separator, placed by the element's alignment.
 *
 * The box is three fields wide (four with hundredths); cOVStateHUD1P_calcElemPos places it from @p pos, @p scale, the two
 * alignments and @p reflect. Flag bit 0 appends ".hh" from @p secFrac, bits 1 / 2 prefix "-" / "+". Each '1' digit is
 * nudged right by +0x178 (probably to centre the narrow glyph). The struct field called digitW (+0x16C) holds
 * the separator's width and f170 the width of "00", so the locals sepW / digW are swapped; the sign's advance
 * (f174) is not scaled.
 *
 * @param self_  the HUD state (this)
 * @param hour  the first field, hours (On Tour hour)
 * @param minute  the second field, minutes
 * @param sec  the third field, seconds
 * @param secFrac  hundredths, drawn only with flag bit 0 (On Tour secFrac)
 * @param timeRenderFlags  the time flags: bit 0 hundredths, bit 1 a "-" sign, bit 2 a "+" sign
 * @param pos  the element position (x, y)
 * @param scale  the x / y scale, also applied to the font
 * @param color  the text colour
 * @param align  the horizontal alignment (the element's alignX; On Tour pivotX)
 * @param flags  the vertical alignment, despite the name (alignY; On Tour pivotY)
 * @param reflect  the reflect flags for cOVStateHUD1P_calcElemPos: bit 0 mirrors x about 640, bit 1 then keeps the horizontal
 *             alignment unflipped
 */
extern "C" void cOVStateHUD1P_renderTime(void* self_, int hour, int minute, int sec, int secFrac, int timeRenderFlags, float* pos, float* scale, sColor_001F16C0* color, int align, int flags, int reflect)
{
    sHUD_001F1840* self = (sHUD_001F1840*)self_;
    float size[2];
    float out[2];
    char bufs[3][4];
    float n = 3.0f;
    if (timeRenderFlags & 1) n = 4.0f;
    size[1] = (float)self->font->count * self->font->f34;
    size[0] = self->f170 * n + (self->digitW + 2.0f + 2.0f) * (n - 1.0f);
    cOVStateHUD1P_calcElemPos(self, out, pos, size, scale, align, flags, reflect);
    sFont_001F1840* font = self->font;
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
    self->font->color = *color;
    float sepW = (self->f170 + 2.0f) * scale[0];
    float digW = (self->digitW + 2.0f) * scale[0];
    float oneW = self->oneW * scale[0];
    if (timeRenderFlags & 6) {
        if (timeRenderFlags & 2)
            cFont_text(self->font, out[0], out[1], D_004A2108);
        else
            cFont_text(self->font, out[0], out[1], D_004A2100);
        out[0] += self->f174;
    }
    sprintf(bufs[0], str_pct_02d, hour);
    sprintf(bufs[1], str_pct_02d, minute);
    sprintf(bufs[2], str_pct_02d, sec);
    cFont_text(self->font, out[0] + oneAdj_001F1840(bufs[0], oneW), out[1], bufs[0]);
    out[0] += sepW;
    for (int i = 1; i < 3; i++) {
        cFont_text(self->font, out[0], out[1], self->sep);
        out[0] += digW;
        char* fieldText = bufs[i];
        cFont_text(self->font, out[0] + oneAdj_001F1840(fieldText, oneW), out[1], fieldText);
        out[0] += sepW;
    }
    if (timeRenderFlags & 1)
        cFont_textf(self->font, out[0], out[1], str_pct_02d_b, secFrac);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderText);
#ifdef SKIP_ASM
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sVec4_001F1B30 {
    float x, y, z, w;
    sVec4_001F1B30(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
};

struct sFont_001F1B30 {
    char pad0[0x30];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColor_001F16C0 color; // 0x40
    sVec4_001F1B30 tint;   // 0x50
};


/**
 * Draws an 8-bit string placed, scaled and coloured by a HUD element definition.
 *
 * From element @p idx of @p defs it takes the position (tHUDDef_getPos) and the scale (tHUDDef_getScale, times @p scaleMul
 * when given). The font's Scale becomes BaseScale times that scale and the text is measured with cFont_textExtent. The
 * pivots are the element's (+0x20, +0x21) unless @p align / @p flags give one (0x7F means "use the element's");
 * cOVStateHUD1P_calcElemPos places the measured box with reflect bits @p reflect, @p posOff is added and @p rect receives the box. The
 * colour is the element's (tHUDDef_getColour) or @p color. The element's layer byte (+0x22, On Tour tHUDDef::layer) goes
 * into bits 5-9 of the current draw state's word 2, the font's Color is set and its ShadowColor becomes (colour alpha,
 * 0, 0, 0); cFont_text draws the text. Afterwards ShadowColor is reset to (1, 0, 0, 0) and Scale to BaseScale.
 *
 * @param ctx  the HUD state, passed on to cOVStateHUD1P_calcElemPos
 * @param font  the font
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx  the element index
 * @param str  the text
 * @param reflect  reflect bits for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param scaleMul  extra scale, or 0
 * @param posOff  offset added after placing, or 0
 * @param color  colour override (tARGB), or 0 for the element's
 * @param rect  receives the drawn box (x, y, width, height), or 0
 * @param align  horizontal pivot override, 0x7F for the element's
 * @param flags  vertical pivot override, 0x7F for the element's
 */
extern "C" void cOVStateHUD1P_renderText(void* ctx, sFont_001F1B30* font, sHudElemDef_001F16C0* defs, int idx, const char* str, int reflect, float* scaleMul, float* posOff, sColor_001F16C0* color, float* rect, int align, int flags)
{
    float scale[2];
    float pos[4];
    float escale[2];
    float extent[4];
    float out[2];
    float size[2];
    sColor_001F16C0 col;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    sHudElemDef_001F16C0* hudDef = &defs[idx];
    tHUDDef_getPos(hudDef, pos);
    tHUDDef_getScale(hudDef, escale);
    if (scaleMul != 0) {
        escale[0] *= scaleMul[0];
        escale[1] *= scaleMul[1];
    }
    font->f38 = font->f30 * escale[0];
    font->f3C = font->f34 * escale[1];
    cFont_textExtent(font, str, extent, 0, font->f38, font->f3C);
    int ax = (align == 0x7F) ? hudDef->align : align;
    int ay = (flags == 0x7F) ? defs[idx].flags : flags;
    size[0] = extent[2];
    size[1] = extent[3];
    cOVStateHUD1P_calcElemPos(ctx, out, pos, size, scale, ax, ay, reflect);
    if (posOff != 0) {
        out[0] += posOff[0];
        out[1] += posOff[1];
    }
    if (rect != 0) {
        rect[0] = out[0];
        rect[1] = out[1];
        rect[2] = extent[2];
        rect[3] = extent[3];
    }
    if (color == 0) {
        tHUDDef_getColour(&defs[idx], &col);
    } else {
        col = *color;
    }
    sVec4_001F1B30 shadowColour(col.r, 0.0f, 0.0f, 0.0f);
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[idx].pad22[0] << 5) & 0x3E0);
    font->color = col;
    font->tint = shadowColour;
    cFont_text(font, out[0], out[1], str);
    font->tint = sVec4_001F1B30(1.0f, 0.0f, 0.0f, 0.0f);
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderText1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// NOTE w1 2026-10-21: cOVStateHUD1P_renderText1 best 99.94 (not committed). Only residual: 4 insns at the start of the
// last block before the jal cFont_text: $f0..$f3 numbering of the font->f38/f3C = f30/f34 * resEsc[] loads
// is swapped (local alloc; sx regs reports nothing). Tried operand order / statement order / locals (99.78-99.94).
// Sibling func_001F25E0 is identical except: func_003921F0 instead of cFont_textExtent (str is const unsigned short*),
// func_00391E30 instead of cFont_text. Needs sColor_001F16C0, sHudElemDef_001F16C0 from cOVStateHUD1P_renderTime1's block.
// Lessons: dead stores of dOutT/dOut land in the frame because arrays are written; col shares the slot of dOutT when
// dOutT/dOut live in an inner block; nested by-value VU0 helper calls in ONE expression share one temp slot;
// a 4-float ctor temp + struct copy gives the 'swc1 x4; ldl/ldr/sdl/sdr' colour build.

struct sV4_001F1E28 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCol_001F1E28 {
    float r, g, b, a;
    sCol_001F1E28() {}
    sCol_001F1E28(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sFont_001F1E28 {
    char pad0[0x30];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sCol_001F1E28 color; // 0x40
};


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_001F1E28 vu0Sub_1E28(const sV4_001F1E28& a, const sV4_001F1E28& b)
{
    sV4_001F1E28 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_001F1E28 vu0Add_1E28(const sV4_001F1E28& a, const sV4_001F1E28& b)
{
    sV4_001F1E28 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_001F1E28 vu0Scale_1E28(const sV4_001F1E28& v, float s)
{
    sV4_001F1E28 r;
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

/**
 * Draws an 8-bit string blended between two HUD element definitions, interpolating position, scale and colour by @p blend.
 *
 * Measures the text at the font's BaseScale (cFont_textExtent), reads the position and scale of elements @p idx1
 * and @p idx2 (both times @p scaleMul when given), places each scaled box with cOVStateHUD1P_calcElemPos (pivots as in
 * cOVStateHUD1P_renderText, 0x7F = the element's; reflect bits @p reflect) and interpolates position and scale linearly by @p blend;
 * then @p posOff is added and @p rect filled. Without @p color the two element colours (tHUDDef_getColour1, as r, g, b, a
 * vectors) are interpolated with VU0 vector maths, the end colour's alpha forced to 0 so the text fades out toward @p blend
 * = 1; with @p color its alpha is multiplied by 1 - @p blend. The draw state's layer bits (5-9 of word 2) come from element
 * 26's layer byte whatever the indices. Draws with cFont_text at the blended scale and resets Scale to BaseScale;
 * unlike cOVStateHUD1P_renderText it leaves ShadowColor alone.
 *
 * @param ctx  the HUD state, passed on to cOVStateHUD1P_calcElemPos
 * @param blend  the blend: 0 = element @p idx1, 1 = element @p idx2
 * @param font  the font
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx1  the element at @p blend = 0
 * @param idx2  the element at @p blend = 1
 * @param str  the text
 * @param reflect  reflect bits for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param scaleMul  extra scale, or 0
 * @param posOff  offset added after placing, or 0
 * @param color  colour override (tARGB), or 0 to blend the elements' colours
 * @param rect  receives the drawn box (x, y, width, height), or 0
 * @param align  horizontal pivot override, 0x7F for the elements'
 * @param flags  vertical pivot override, 0x7F for the elements'
 */
// Draws text blended between two HUD element layouts (t = 0 is element idx1, t = 1 is element idx2).
extern "C" void cOVStateHUD1P_renderText1(void* ctx, float blend, sFont_001F1E28* font, sHudElemDef_001F16C0* defs, int idx1, int idx2, const char* str, int reflect, float* scaleMul, float* posOff, sColor_001F16C0* color, float* rect, int align, int flags)
{
    float scale[2];
    float extent[4];
    float pos1[4];
    float esc1[2];
    float pos2[4];
    float esc2[2];
    float out1[2];
    float out2[2];
    float size1[2];
    float size2[2];
    float resEsc[2];
    float resPos[2];
    float dEsc[2];
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    font->f38 = font->f30;
    font->f3C = font->f34;
    cFont_textExtent(font, str, extent, 0, font->f38, font->f3C);
    sHudElemDef_001F16C0* startDef = &defs[idx1];
    tHUDDef_getPos(startDef, pos1);
    tHUDDef_getScale(startDef, esc1);
    if (scaleMul != 0) {
        esc1[0] *= scaleMul[0];
        esc1[1] *= scaleMul[1];
    }
    sHudElemDef_001F16C0* endDef = &defs[idx2];
    tHUDDef_getPos(endDef, pos2);
    tHUDDef_getScale(endDef, esc2);
    if (scaleMul != 0) {
        esc2[0] *= scaleMul[0];
        esc2[1] *= scaleMul[1];
    }
    size1[0] = extent[2] * esc1[0];
    size1[1] = extent[3] * esc1[1];
    size2[0] = extent[2] * esc2[0];
    size2[1] = extent[3] * esc2[1];
    int ax1 = (align == 0x7F) ? startDef->align : (signed char)align;
    int ay1 = (flags == 0x7F) ? defs[idx1].flags : (signed char)flags;
    int ax2 = (align == 0x7F) ? defs[idx2].align : (signed char)align;
    int ay2 = (flags == 0x7F) ? defs[idx2].flags : (signed char)flags;
    cOVStateHUD1P_calcElemPos(ctx, out1, pos1, size1, scale, ax1, ay1, reflect);
    cOVStateHUD1P_calcElemPos(ctx, out2, pos2, size2, scale, ax2, ay2, reflect);
    dEsc[0] = esc2[0] - esc1[0];
    dEsc[1] = esc2[1] - esc1[1];
    {
        float dOutT[2];
        float dOut[2];
        dOut[0] = out2[0] - out1[0];
        dOut[1] = out2[1] - out1[1];
        dOutT[0] = dOut[0] * blend;
        dOutT[1] = dOut[1] * blend;
        resPos[0] = dOutT[0] + out1[0];
        resPos[1] = dOutT[1] + out1[1];
        resEsc[0] = dEsc[0] * blend + esc1[0];
        resEsc[1] = dEsc[1] * blend + esc1[1];
    }
    if (posOff != 0) {
        resPos[0] = resPos[0] + posOff[0];
        resPos[1] = resPos[1] + posOff[1];
    }
    if (rect != 0) {
        rect[0] = resPos[0];
        rect[1] = resPos[1];
        rect[2] = extent[2] * resEsc[0];
        rect[3] = extent[3] * resEsc[1];
    }
    sCol_001F1E28 col;
    if (color == 0) {
        sV4_001F1E28 startColour;
        sV4_001F1E28 endColour;
        tHUDDef_getColour1(&defs[idx1], &startColour);
        tHUDDef_getColour1(&defs[idx2], &endColour);
        endColour.w = 0.0f;
        sV4_001F1E28 blendedColour = vu0Add_1E28(vu0Scale_1E28(vu0Sub_1E28(endColour, startColour), blend), startColour);
        col = sCol_001F1E28(blendedColour.w, blendedColour.x, blendedColour.y, blendedColour.z);
    } else {
        col = *(sCol_001F1E28*)color;
        col.r = col.r * (1.0f - blend);
    }
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[26].pad22[0] << 5) & 0x3E0);
    font->color = col;
    float baseScaleX = font->f30;
    float blendScaleX = resEsc[0];
    float blendScaleY = resEsc[1];
    float baseScaleY = font->f34;
    font->f3C = baseScaleY * blendScaleY;
    font->f38 = baseScaleX * blendScaleX;
    cFont_text(font, resPos[0], resPos[1], str);
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderText2);
#ifdef SKIP_ASM
extern "C" float func_003921F0(void* self, const unsigned short* str, void* out, int n, float sx, float sy);
extern "C" void func_00391E30(void* self, float x, float y, const unsigned short* str);

struct sVec4_001F22E8 {
    float x, y, z, w;
    sVec4_001F22E8(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
};

struct sFont_001F22E8 {
    char pad0[0x30];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColor_001F16C0 color; // 0x40
    sVec4_001F22E8 tint;   // 0x50
};


/**
 * Draws a 16-bit string placed, scaled and coloured by a HUD element definition.
 *
 * The same as cOVStateHUD1P_renderText except that the text is measured with func_003921F0 and drawn with func_00391E30: element
 * position and scale (times @p scaleMul), pivots from the element unless overridden (0x7F), placement by cOVStateHUD1P_calcElemPos
 * with reflect bits @p reflect, @p posOff, the box into @p rect, the element's or @p color's colour, the layer byte into
 * bits 5-9 of the draw state's word 2, ShadowColor (colour alpha, 0, 0, 0) while drawing, then ShadowColor (1, 0, 0, 0)
 * and Scale = BaseScale again.
 *
 * @param ctx  the HUD state, passed on to cOVStateHUD1P_calcElemPos
 * @param font  the font
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx  the element index
 * @param str  the 16-bit text
 * @param reflect  reflect bits for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param scaleMul  extra scale, or 0
 * @param posOff  offset added after placing, or 0
 * @param color  colour override (tARGB), or 0 for the element's
 * @param rect  receives the drawn box (x, y, width, height), or 0
 * @param align  horizontal pivot override, 0x7F for the element's
 * @param flags  vertical pivot override, 0x7F for the element's
 */
extern "C" void cOVStateHUD1P_renderText2(void* ctx, sFont_001F22E8* font, sHudElemDef_001F16C0* defs, int idx, const unsigned short* str, int reflect, float* scaleMul, float* posOff, sColor_001F16C0* color, float* rect, int align, int flags)
{
    float scale[2];
    float pos[4];
    float escale[2];
    float extent[4];
    float out[2];
    float size[2];
    sColor_001F16C0 col;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    sHudElemDef_001F16C0* hudDef = &defs[idx];
    tHUDDef_getPos(hudDef, pos);
    tHUDDef_getScale(hudDef, escale);
    if (scaleMul != 0) {
        escale[0] *= scaleMul[0];
        escale[1] *= scaleMul[1];
    }
    font->f38 = font->f30 * escale[0];
    font->f3C = font->f34 * escale[1];
    func_003921F0(font, str, extent, 0, font->f38, font->f3C);
    int ax = (align == 0x7F) ? hudDef->align : align;
    int ay = (flags == 0x7F) ? defs[idx].flags : flags;
    size[0] = extent[2];
    size[1] = extent[3];
    cOVStateHUD1P_calcElemPos(ctx, out, pos, size, scale, ax, ay, reflect);
    if (posOff != 0) {
        out[0] += posOff[0];
        out[1] += posOff[1];
    }
    if (rect != 0) {
        rect[0] = out[0];
        rect[1] = out[1];
        rect[2] = extent[2];
        rect[3] = extent[3];
    }
    if (color == 0) {
        tHUDDef_getColour(&defs[idx], &col);
    } else {
        col = *color;
    }
    sVec4_001F22E8 shadowColour(col.r, 0.0f, 0.0f, 0.0f);
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[idx].pad22[0] << 5) & 0x3E0);
    font->color = col;
    font->tint = shadowColour;
    func_00391E30(font, out[0], out[1], str);
    font->tint = sVec4_001F22E8(1.0f, 0.0f, 0.0f, 0.0f);
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", func_001F25E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_001F25E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sCol_001F25E0 {
    float r, g, b, a;
    sCol_001F25E0() {}
    sCol_001F25E0(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sFont_001F25E0 {
    char pad0[0x30];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sCol_001F25E0 color; // 0x40
};


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sV4_001F25E0 vu0Sub_25E0(const sV4_001F25E0& a, const sV4_001F25E0& b)
{
    sV4_001F25E0 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_001F25E0 vu0Add_25E0(const sV4_001F25E0& a, const sV4_001F25E0& b)
{
    sV4_001F25E0 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_001F25E0 vu0Scale_25E0(const sV4_001F25E0& v, float s)
{
    sV4_001F25E0 r;
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

/**
 * Draws a 16-bit string blended between two HUD element definitions, interpolating position, scale and colour by @p blend.
 *
 * The same as cOVStateHUD1P_renderText1 except that the text is measured with func_003921F0 and drawn with func_00391E30: both
 * elements' positions and scales (times @p scaleMul), boxes placed by cOVStateHUD1P_calcElemPos with reflect bits @p reflect, linear
 * interpolation by @p blend, @p posOff and @p rect, colours blended with VU0 maths toward an end alpha of 0 (or @p color
 * with its alpha times 1 - @p blend), and the layer bits taken from element 26 whatever the indices. Nothing in retail
 * calls it.
 *
 * @param ctx  the HUD state, passed on to cOVStateHUD1P_calcElemPos
 * @param blend  the blend: 0 = element @p idx1, 1 = element @p idx2
 * @param font  the font
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx1  the element at @p blend = 0
 * @param idx2  the element at @p blend = 1
 * @param str  the 16-bit text
 * @param reflect  reflect bits for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param scaleMul  extra scale, or 0
 * @param posOff  offset added after placing, or 0
 * @param color  colour override (tARGB), or 0 to blend the elements' colours
 * @param rect  receives the drawn box (x, y, width, height), or 0
 * @param align  horizontal pivot override, 0x7F for the elements'
 * @param flags  vertical pivot override, 0x7F for the elements'
 */
// Draws text blended between two HUD element layouts (t = 0 is element idx1, t = 1 is element idx2).
extern "C" void func_001F25E0(void* ctx, float blend, sFont_001F25E0* font, sHudElemDef_001F16C0* defs, int idx1, int idx2, const unsigned short* str, int reflect, float* scaleMul, float* posOff, sColor_001F16C0* color, float* rect, int align, int flags)
{
    float scale[2];
    float extent[4];
    float pos1[4];
    float esc1[2];
    float pos2[4];
    float esc2[2];
    float out1[2];
    float out2[2];
    float size1[2];
    float size2[2];
    float resEsc[2];
    float resPos[2];
    float dEsc[2];
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    font->f38 = font->f30;
    font->f3C = font->f34;
    func_003921F0(font, str, extent, 0, font->f38, font->f3C);
    sHudElemDef_001F16C0* startDef = &defs[idx1];
    tHUDDef_getPos(startDef, pos1);
    tHUDDef_getScale(startDef, esc1);
    if (scaleMul != 0) {
        esc1[0] *= scaleMul[0];
        esc1[1] *= scaleMul[1];
    }
    sHudElemDef_001F16C0* endDef = &defs[idx2];
    tHUDDef_getPos(endDef, pos2);
    tHUDDef_getScale(endDef, esc2);
    if (scaleMul != 0) {
        esc2[0] *= scaleMul[0];
        esc2[1] *= scaleMul[1];
    }
    size1[0] = extent[2] * esc1[0];
    size1[1] = extent[3] * esc1[1];
    size2[0] = extent[2] * esc2[0];
    size2[1] = extent[3] * esc2[1];
    int ax1 = (align == 0x7F) ? startDef->align : (signed char)align;
    int ay1 = (flags == 0x7F) ? defs[idx1].flags : (signed char)flags;
    int ax2 = (align == 0x7F) ? defs[idx2].align : (signed char)align;
    int ay2 = (flags == 0x7F) ? defs[idx2].flags : (signed char)flags;
    cOVStateHUD1P_calcElemPos(ctx, out1, pos1, size1, scale, ax1, ay1, reflect);
    cOVStateHUD1P_calcElemPos(ctx, out2, pos2, size2, scale, ax2, ay2, reflect);
    dEsc[0] = esc2[0] - esc1[0];
    dEsc[1] = esc2[1] - esc1[1];
    {
        float dOutT[2];
        float dOut[2];
        dOut[0] = out2[0] - out1[0];
        dOut[1] = out2[1] - out1[1];
        dOutT[0] = dOut[0] * blend;
        dOutT[1] = dOut[1] * blend;
        resPos[0] = dOutT[0] + out1[0];
        resPos[1] = dOutT[1] + out1[1];
        resEsc[0] = dEsc[0] * blend + esc1[0];
        resEsc[1] = dEsc[1] * blend + esc1[1];
    }
    if (posOff != 0) {
        resPos[0] = resPos[0] + posOff[0];
        resPos[1] = resPos[1] + posOff[1];
    }
    if (rect != 0) {
        rect[0] = resPos[0];
        rect[1] = resPos[1];
        rect[2] = extent[2] * resEsc[0];
        rect[3] = extent[3] * resEsc[1];
    }
    sCol_001F25E0 col;
    if (color == 0) {
        sV4_001F25E0 startColour;
        sV4_001F25E0 endColour;
        tHUDDef_getColour1(&defs[idx1], &startColour);
        tHUDDef_getColour1(&defs[idx2], &endColour);
        endColour.w = 0.0f;
        sV4_001F25E0 blendedColour = vu0Add_25E0(vu0Scale_25E0(vu0Sub_25E0(endColour, startColour), blend), startColour);
        col = sCol_001F25E0(blendedColour.w, blendedColour.x, blendedColour.y, blendedColour.z);
    } else {
        col = *(sCol_001F25E0*)color;
        col.r = col.r * (1.0f - blend);
    }
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[26].pad22[0] << 5) & 0x3E0);
    font->color = col;
    float baseScaleX = font->f30;
    float blendScaleX = resEsc[0];
    float blendScaleY = resEsc[1];
    float baseScaleY = font->f34;
    font->f3C = baseScaleY * blendScaleY;
    font->f38 = baseScaleX * blendScaleX;
    func_00391E30(font, resPos[0], resPos[1], str);
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderMultiText);
#ifdef SKIP_ASM
extern "C" char* func_00392430(void* f, const char* str, char* dst, int max, float sx, float width);
extern "C" float cFont_textExtent(void* self, const char* str, void* out, int n, float sx, float sy);

struct sFont_001F2AA0 {
    char pad0[0x14];
    int count;          // 0x14
    char pad18[0x18];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColor_001F16C0 color; // 0x40
};


static inline float alignPos_001F2AA0(float v, int a, float size)
{
    if (a == 0)
        v -= size * 0.5f;
    else if (a > 0)
        v -= size;
    return v;
}

/**
 * Draws an 8-bit string word-wrapped to a HUD element's width, as lines stacked from the element's position.
 *
 * The wrapping uses the HUD state's font at +0x428 (cOVStateHUD1P's "fefont"; every caller also passes it as @p font),
 * scaled to the element's scale: func_00392430 cuts lines of at most 50 characters to fit the element's width
 * (tHUDDef_getSize). The colour is the element's (no override) and its layer byte goes into the draw state as in
 * cOVStateHUD1P_renderText. The block's top is the element's y moved up by half (pivot 0) or all (pivot > 0) of the total height,
 * lines * MaxCharHeight * BaseScale.y * scale y. Each line is measured with cFont_textExtent at @p font's current
 * Scale, placed by the horizontal pivot (bit 0 of @p flags mirrors x about 640 and, without bit 1, flips the pivot) and
 * drawn with cFont_text. @p rect, when given, receives the leftmost x, the top, the widest line and the total height.
 * Finally @p font's Scale is reset to BaseScale.
 *
 * Likely bug: nothing stops the wrap loop at five lines, so a longer text overruns the 5 x 50 line buffer on the stack.
 *
 * @param font  the font to draw with
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx  the element index
 * @param str  the text
 * @param flags  reflect bits: 1 mirrors x about 640, 2 then keeps the pivot unflipped
 * @param rect  receives the drawn area (x, y, width, height), or 0
 */
extern "C" void cOVStateHUD1P_renderMultiText(void* self, sFont_001F2AA0* font, sHudElemDef_001F16C0* defs, int idx, const char* str, int flags, float* rect)
{
    float wrap[2];
    float pos[2];
    float escale[2];
    char lines[5][50];
    sColor_001F16C0 col;
    float scale[2];
    int n = 0;
    sHudElemDef_001F16C0* hudDef = &defs[idx];
    tHUDDef_getSize(hudDef, wrap, 0);
    tHUDDef_getPos(hudDef, pos);
    tHUDDef_getScale(hudDef, escale);
    sFont_001F2AA0* sf = *(sFont_001F2AA0**)((char*)self + 0x428);
    sf->f38 = sf->f30 * escale[0];
    sf->f3C = sf->f34 * escale[1];
    const char* remaining = str;
    while (remaining != 0) {
        remaining = func_00392430(*(sFont_001F2AA0**)((char*)self + 0x428), remaining, lines[n], 50, (*(sFont_001F2AA0**)((char*)self + 0x428))->f38, wrap[0]);
        n++;
    }
    tHUDDef_getColour(&defs[idx], &col);
    font->color = col;
    float fn = (float)n;
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[idx].pad22[0] << 5) & 0x3E0);
    float lineH = (float)font->count * font->f34 * escale[1];
    float totalH = lineH * fn;
    float y = alignPos_001F2AA0(pos[1], defs[idx].flags, totalH);
    if (rect != 0) {
        rect[0] = 65536.0f;
        rect[1] = y;
        rect[2] = 0.0f;
        rect[3] = totalH;
    }
    int i = 0;
    if (n > 0) do {
        char* line = lines[i];
        float w = cFont_textExtent(font, line, 0, 0, font->f38, font->f3C);
        float x = pos[0];
        int al = defs[idx].align;
        if (flags & 1) {
            x = 640.0f - x;
            if (!(flags & 2)) al = -al;
        }
        if (al == 0)
            x -= w * 0.5f;
        else if (al > 0)
            x -= w;
        cFont_text(font, x, y, line);
        if (rect != 0) {
            if (x < rect[0]) rect[0] = x;
            if (rect[2] < w) rect[2] = w;
        }
        y += lineH;
    } while (++i < n);
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cOVStateHUD1P_renderMultiText1);
#ifdef SKIP_ASM
extern "C" unsigned short* func_00392680(void* f, float sx, float width, const unsigned short* str, unsigned short* dst, int max);
extern "C" float func_003921F0(void* self, const unsigned short* str, void* out, int n, float sx, float sy);
extern "C" void func_00391E30(void* self, float x, float y, const unsigned short* str);

struct sFont_001F2DB0 {
    char pad0[0x14];
    int count;          // 0x14
    char pad18[0x18];
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColor_001F16C0 color; // 0x40
};


static inline float alignPos_001F2DB0(float v, int a, float size)
{
    if (a == 0)
        v -= size * 0.5f;
    else if (a > 0)
        v -= size;
    return v;
}

/**
 * Draws a 16-bit string word-wrapped to a HUD element's width, as lines stacked from the element's position.
 *
 * The same as cOVStateHUD1P_renderMultiText with 16-bit text: func_00392680 wraps it (HUD state font at +0x428, scaled to the element),
 * func_003921F0 measures and func_00391E30 draws each line, with the element's colour, layer, pivots, @p flags reflect
 * bits and the @p rect out. Up to five lines of 50 characters fit the stack buffer and nothing enforces it.
 *
 * Likely bug: the wrap width always comes from element 29 (tHUDDef_getSize on defs[29]), not from element @p idx.
 *
 * @param font  the font to draw with
 * @param defs  the HUD definition table (0x24-byte records)
 * @param idx  the element index
 * @param str  the 16-bit text
 * @param flags  reflect bits: 1 mirrors x about 640, 2 then keeps the pivot unflipped
 * @param rect  receives the drawn area (x, y, width, height), or 0
 */
extern "C" void cOVStateHUD1P_renderMultiText1(void* self, sFont_001F2DB0* font, sHudElemDef_001F16C0* defs, int idx, const unsigned short* str, int flags, float* rect)
{
    float wrap[2];
    float pos[2];
    float escale[2];
    unsigned short lines[5][50];
    sColor_001F16C0 col;
    float scale[2];
    int n = 0;
    tHUDDef_getSize(&defs[29], wrap, 0);
    sHudElemDef_001F16C0* hudDef = &defs[idx];
    tHUDDef_getPos(hudDef, pos);
    tHUDDef_getScale(hudDef, escale);
    sFont_001F2DB0* sf = *(sFont_001F2DB0**)((char*)self + 0x428);
    sf->f38 = sf->f30 * escale[0];
    sf->f3C = sf->f34 * escale[1];
    const unsigned short* remaining = str;
    while (remaining != 0) {
        remaining = func_00392680(*(sFont_001F2DB0**)((char*)self + 0x428), (*(sFont_001F2DB0**)((char*)self + 0x428))->f38, wrap[0], remaining, lines[n], 50);
        n++;
    }
    tHUDDef_getColour(&defs[idx], &col);
    font->color = col;
    float fn = (float)n;
    sDrawState* rs = GrMan->m_pCurrentDrawState;
    rs->unk_00[2] = (((int)rs->unk_00[2]) & ~0x3E0) | ((defs[idx].pad22[0] << 5) & 0x3E0);
    float lineH = (float)font->count * font->f34 * escale[1];
    float totalH = lineH * fn;
    float y = alignPos_001F2DB0(pos[1], defs[idx].flags, totalH);
    if (rect != 0) {
        rect[0] = 65536.0f;
        rect[1] = y;
        rect[2] = 0.0f;
        rect[3] = totalH;
    }
    int i = 0;
    if (n > 0) do {
        unsigned short* line = lines[i];
        float w = func_003921F0(font, line, 0, 0, font->f38, font->f3C);
        float x = pos[0];
        int al = defs[idx].align;
        if (flags & 1) {
            x = 640.0f - x;
            if (!(flags & 2)) al = -al;
        }
        if (al == 0)
            x -= w * 0.5f;
        else if (al > 0)
            x -= w;
        func_00391E30(font, x, y, line);
        if (rect != 0) {
            if (x < rect[0]) rect[0] = x;
            if (rect[2] < w) rect[2] = w;
        }
        y += lineH;
    } while (++i < n);
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    font->f38 = font->f30 * scale[0];
    font->f3C = font->f34 * scale[1];
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_cBXLoadIndicator__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the loading indicator with everything cleared.
 *
 * Zeroes its four words, On Tour mpLoadText, mLoadIndicators, mAnimFrame and mpFont.
 *
 * @return @p self
 */
void* cBXLoadIndicator_cBXLoadIndicator(void* self)
{
    cOVStateHUDElem* elem = (cOVStateHUDElem*)self;
    int initValue = 0;
    elem->field_0x0 = initValue;
    *(int*)&elem->rangeMax = initValue;
    elem->field_0x4 = initValue;
    *(int*)&elem->rangeMin = initValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_Init);
#ifdef SKIP_ASM
struct sVEntry001F30D8 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sHudIds001F30D8 {
    int a;
    int b;
    int c;
    int d;
};

int GetHashValue32(char* str);
extern "C" int func_003983F0(void* self, int key);
extern char str_kT_ovrcmnloading_b[];
extern char str_fefont[];

/**
 * Loads the loading indicator's text and font and clears its state.
 *
 * mpLoadText (+0x0) becomes the localized "kT_ovrcmnloading_b" string (its GetHashValue32 looked up through
 * SSXApp->FELocale's virtual slot 4) and mpFont (+0xC) the "fefont" font from the UI's font bank (func_003983F0 on
 * SSXApp->Game->UI + 0x8); mLoadIndicators (+0x4) and mAnimFrame (+0x8) are cleared. cGame_gainFocus calls it.
 *
 * @param vself  the loading indicator (this)
 */
extern "C" void cBXLoadIndicator_Init(void* vself)
{
    sHudIds001F30D8* self = (sHudIds001F30D8*)vself;
    char* locale = (char*)SSXApp->FELocale;
    sVEntry001F30D8* vt = *(sVEntry001F30D8**)(locale + 4);
    char* adj = locale + vt[4].delta;
    int loadingHash = GetHashValue32(str_kT_ovrcmnloading_b);
    self->a = vt[4].fn(adj, loadingHash);
    void* tbl = *(void**)((char*)SSXApp->Game->UI + 8);
    self->d = func_003983F0(tbl, GetHashValue32(str_fefont));
    self->b = 0;
    self->c = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_DeInit__FPv);
#ifdef SKIP_ASM
/**
 * Releases the loading indicator's resources: forgets its text and font and clears its state.
 *
 * Zeroes the text (+0x0), the indicator mask (+0x4), the animation frame (+0x8) and the font (+0xC).
 */
void cBXLoadIndicator_DeInit(void* self)
{
    cOVStateHUDElem* indicator = (cOVStateHUDElem*)self;
    int zero = 0;
    indicator->field_0x0 = zero;
    *(int*)&indicator->rangeMax = zero;
    indicator->field_0x4 = zero;
    *(int*)&indicator->rangeMin = zero;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_Update);
#ifdef SKIP_ASM
/**
 * Advances the loading indicator's animation while it is shown.
 *
 * When the indicator mask (+0x4) is nonzero, adds 1/60 to the animation frame (+0x8) and subtracts 2.0 until it
 * is at most 2.0.
 */
extern "C" void cBXLoadIndicator_Update(void* self)
{
    if (*(int*)((char*)self + 4) != 0) {
        float* animFrame = (float*)((char*)self + 8);
        float two = 2.0f;
        *animFrame += 0.01666666753590107f;
        while (*animFrame > two) {
            *animFrame -= two;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_Render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern "C" void func_00391E30(void* self, float x, float y, const unsigned short* str);

struct sVec4_31E0 {
    float x, y, z, w;
    sVec4_31E0() {}
    sVec4_31E0(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};
struct sV2_31E0 {
    float x, y;
    sV2_31E0() {}
    sV2_31E0(const float& ax, const float& ay) : x(ax), y(ay) {}
};
struct sCol_31E0 {
    float r, g, b, a;
    sCol_31E0(const float& ar, const float& ag, const float& ab, const float& aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sGs_31E0 {
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
};

extern sGs_31E0 D_00501420_31E0 __asm__("sDrawState_s_defaultDrawState");
extern char t4mat_Identity[];
extern int D_004A4EB0;
extern sV2_31E0 D_004A4EA8;
extern "C" float func_003921F0(void* obj, const unsigned short* txt, void* a, int b, float x, float y);

class cGfx_31E0 {
public:
    char pad[0xE84];
    sGs_31E0* stateTop;
    char padE88[0x1068 - 0xE88];
    int f1068;
    int GetF1068() { return f1068; }
    char pad106C[0x10D8 - 0x106C];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual void BeginScene();
    virtual void EndScene();
    virtual void v22(float, float, float, float, int);
    virtual void v23(); virtual void v24();
    virtual void SetViewport(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void Begin2D();
    virtual void End2D();
    virtual void v32();
    virtual void SetTexture(void*);
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76();
    virtual void DrawSprite(const sVec4_31E0&, const sV2_31E0&, const sV2_31E0&, const sV2_31E0&, const sCol_31E0&);
};


struct sFont_001F31E0 {
    char pad0[0x14];
    int count;          // 0x14
    char pad18[0x10];
    sV2_31E0 scale;     // 0x28
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sVec4_31E0 color;   // 0x40
    sVec4_31E0 color2;  // 0x50
    void SetScale(float sx, float sy) { f38 = f30 * sx; f3C = f34 * sy; }
};

struct sHudTimer_001F31E0 {
    const unsigned short* text; // 0x0
    int active;                 // 0x4
    float t;                    // 0x8
    sFont_001F31E0* font;       // 0xC
};

/**
 * Draws the loading indicator, its "loading" text and a flipping icon in the bottom-right corner, while any reason to
 * show it is set.
 *
 * Does nothing when mLoadIndicators (+0x4) is 0. Saves the render state (pushes a copy of the current draw state,
 * cPS2GraphicsMan_PushViewport, cPS2GraphicsMan_PushView), resets the draw state to sDrawState_s_defaultDrawState with
 * four bit fields of its word 1 set (2, 0, 20, 5), sets an identity view (func_00395C38 with t4mat_Identity), a
 * full-screen viewport window (func_00376A70 with the screen width and height) and func_00377458 (0, 0, 640, 480, ...).
 * mpLoadText is drawn with mpFont at 0.7 scale in colour (1, 1, 1, 0) (tARGB: opaque yellow) with a black shadow offset
 * by (3, 3), right-aligned at x 620 and vertically centred on y 445, with layer 31 in the draw state. Then an icon
 * sprite (texture: GrMan's word +0x1068) is drawn with cPS2GraphicsMan_DrawSprite 17 px left of the text at y 445, 18
 * high and |mAnimFrame - 1| * 18 wide, so it flips as mAnimFrame runs from 0 to 2, and everything is popped again. The
 * font keeps the 0.7 Scale and a (2, 2) shadow offset afterwards. cGame_render and func_0022C830 call it.
 */
extern "C" void cBXLoadIndicator_Render(sHudTimer_001F31E0* self)
{
    if (self->active == 0) {
        return;
    }
    if (D_004A4EB0 == 0) {
        D_004A4EB0 = 1;
        D_004A4EA8.y = D_004A4EA8.x = 3.0f;
    }
    float flipScale = self->t - 1.0f;
    if (flipScale < 0.0f) {
        flipScale = -flipScale;
    }
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    ((cGfx_31E0*)ps2GraphicsMan)->stateTop[1] = ((cGfx_31E0*)ps2GraphicsMan)->stateTop[0];
    ((cGfx_31E0*)ps2GraphicsMan)->stateTop++;
    ((cGfx_31E0*)GrMan)->BeginScene();
    ((cGfx_31E0*)GrMan)->Begin2D();
    *((cGfx_31E0*)GrMan)->stateTop = D_00501420_31E0;
    ((cGfx_31E0*)GrMan)->stateTop->f4_23 = 2;
    ((cGfx_31E0*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_31E0*)GrMan)->stateTop->f4_12 = 20;
    ((cGfx_31E0*)GrMan)->stateTop->f4_2 = 5;
    ((cGfx_31E0*)GrMan)->SetTexture(t4mat_Identity);
    {
        sVEntry<void(void*, float, float, float, float, int)>* vt = (sVEntry<void (void*, float, float, float, float, int)>*)GrMan->mVtbl;
        char* thisp = (char*)GrMan + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_31E0*)GrMan)->GetWidth(), (float)((cGfx_31E0*)GrMan)->GetHeight(), 1);
    }
    ((cGfx_31E0*)GrMan)->SetViewport(0.0f, 0.0f, 640.0f, 480.0f, 1, 0, 0.0f, 1.0f);
    self->font->color = sVec4_31E0(1.0f, 1.0f, 1.0f, 0.0f);
    self->font->SetScale(0.7f, 0.7f);
    self->font->color2 = sVec4_31E0(1.0f, 0.0f, 0.0f, 0.0f);
    self->font->scale = D_004A4EA8;
    float w = func_003921F0(self->font, self->text, 0, 0, self->font->f38, self->font->f3C);
    sVec4_31E0 pos(620.0f - w, 445.0f - (float)self->font->count * self->font->f34 * 0.35f, 0.0f, 1.0f);
    ((cGfx_31E0*)GrMan)->stateTop->f8_5 = 0x1F;
    func_00391E30(self->font, pos.x, pos.y, self->text);
    self->font->scale = sV2_31E0(2.0f, 2.0f);
    pos.x = pos.x - 17.0f;
    pos.y = 445.0f;
    sV2_31E0 size;
    sV2_31E0 uv0(0.0f, 0.0f);
    sV2_31E0 uv1(1.0f, 1.0f);
    size.x = flipScale * 18.0f;
    size.y = 18.0f;
    *(short*)((char*)((cGfx_31E0*)GrMan)->stateTop + 0x10) = ((cGfx_31E0*)GrMan)->GetF1068();
    ((cGfx_31E0*)GrMan)->DrawSprite(pos, size, uv0, uv1, sCol_31E0(1.0f, 1.0f, 1.0f, 1.0f));
    ((cGfx_31E0*)GrMan)->End2D();
    ((cGfx_31E0*)GrMan)->EndScene();
    ((cGfx_31E0*)GrMan)->stateTop--;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_Show);
#ifdef SKIP_ASM
/**
 * Shows the loading indicator for one more reason.
 *
 * Sets bit @p bit in the indicator mask (+0x4, On Tour mLoadIndicators); cBXLoadIndicator_Hide clears it.
 *
 * @param bit  the reason (On Tour eLoadIndicator)
 */
extern "C" void cBXLoadIndicator_Show(void* self, int bit)
{
    *(int*)((char*)self + 0x4) |= (1 << bit);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cBXLoadIndicator_Hide);
#ifdef SKIP_ASM
/**
 * Hides one reason for showing the loading indicator, and restarts its animation once none are left.
 *
 * Clears bit @p bit in the indicator mask (+0x4, On Tour mLoadIndicators); when the mask becomes 0, the
 * animation frame (+0x8) is reset to 0.
 *
 * @param bit  the indicator to hide (On Tour eLoadIndicator); callers use 0 to 4
 */
extern "C" void cBXLoadIndicator_Hide(void* self, int bit)
{
    *(int*)((char*)self + 0x4) &= ~(1 << bit);
    if (*(int*)((char*)self + 0x4) == 0) {
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatehud", cFEStateLodge_cFEStateLodge);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getPlayerCtrlID(void* self, int a1);
extern "C" void func_001A1CB8(void* self, int a1, int val);
extern void* cFEStateLodge_vtbl[];

/**
 * Constructs the lodge front-end state.
 *
 * Runs the cUIState constructor func_0039E2A0 (the engine the callers pass as a second argument reaches it untouched in
 * $a1; this declaration takes only the memory), sets mHashName to 39 and mVtbl to cFEStateLodge_vtbl and sets mUserNum
 * to 0. When the engine's first word (an object) is set, the state is limited to player 0's controller: mCtrlBits = 1
 * << cBENewPlayerInterface_getPlayerCtrlID(player 0), and the same mask is stored in that object's byte at +0x8
 * (func_001A1CB8, index 0). The callers allocate 88 bytes tagged "cFEStateLodge".
 *
 * @return @p self
 */
extern "C" void* cFEStateLodge_cFEStateLodge(void* self)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 39;
    *(void***)((char*)self + 0x8) = cFEStateLodge_vtbl;
    int idx = cBENewPlayerInterface_getPlayerCtrlID(cBE_getInterface_Fv(cBE_getBE(), 1), 0);
    *(signed char*)((char*)self + 0x44) = 0;
    void* factory = **(void***)((char*)self + 0x10);
    if (factory != 0) {
        unsigned char mask = 1 << idx;
        func_001A1CB8(factory, 0, mask);
        *(unsigned char*)((char*)self + 0x15) = mask;
    }
    return self;
}
#endif

