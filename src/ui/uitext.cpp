#include "common.h"
#include "ui/uitextscroll.h"
#include "ui/uithing.h"
#include "ui/uivector.h"
#include "render/ps2graphicsman.h"
#include "vtable.h"
#include "ui/uitext.h"

//100%
INCLUDE_ASM("ui/uitext", cUIText_render2D);
#ifdef SKIP_ASM
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_00392BA8(void* font, int ch);
// The unit declares these with the string as int; bind pointer-typed views.
extern "C" float func_003921F0_s(void* font, unsigned short* str, void* out, int flags, float sx, float sy) __asm__("func_003921F0");
extern "C" void func_00391E30_s(void* font, float x, float y, unsigned short* str) __asm__("func_00391E30");
extern "C" void func_0039FEB8(void* self, void* pos, void* size);
extern "C" void func_002C2718(unsigned short* dst, unsigned short* fmt, unsigned short* str);
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void*);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00494078[];
extern unsigned short D_004A47B0[];
extern unsigned short D_00508750[];

struct sV2_05E8 {
    float x, y;
    sV2_05E8() {}
    sV2_05E8(float ax, float ay) : x(ax), y(ay) {}
};
struct sV3_05E8 {
    float x, y, z;
    sV3_05E8() {}
    sV3_05E8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sCol_05E8 {
    float r, g, b, a;
};
struct sRect_05E8 {
    float x0, y0, x1, y1;
};

// Default drop-shadow offset ($gp-relative, 8 bytes).
extern sV2_05E8 D_004A5A60;

struct sFont_05E8 {
    char pad0[0x14];
    int height;             // 0x14
    char pad18[0x10];
    sV2_05E8 shadowOfs;     // 0x28
    sV2_05E8 scale;         // 0x30
    sV2_05E8 cur;           // 0x38
    sCol_05E8 color;        // 0x40
    sCol_05E8 shadowColor;  // 0x50
};

struct sLayerBits_05E8 {
    unsigned char lo;
    unsigned char layer : 5;
};

struct sVE_05E8 { short delta; short index; void (*fn)(void*); };

struct sText_05E8 {
    char pad0[0x8];
    sVE_05E8* vt;           // 0x08
    char padC[0x8];
    int flags14;            // 0x14
    char pad18[0x4];
    sCol_05E8 color;        // 0x1C
    char pad2C[0x18];
    float x;                // 0x44
    float y;                // 0x48
    char pad4C[0x4];
    float sx;               // 0x50
    float sy;               // 0x54
    char pad58[0x4];
    char* owner;            // 0x5C
    float width;            // 0x60
    char pad64[0x10];
    int flags;              // 0x74
    sCol_05E8 shadowColor;  // 0x78
    sCol_05E8 hiColor;      // 0x88
    sCol_05E8 offColor;     // 0x98
    int numLines;           // 0xA8
    unsigned short** lines; // 0xAC
    char padB0[0x4];
    unsigned short* str;    // 0xB4
};

/**
 * Draws a text object's lines at its position plus an offset.
 *
 * Refreshes the string through updateLocale (slot 16) when there is none or the cached bit is clear. Picks
 * the static colour (static thing with use-static-colour set), the disabled colour (disabled thing with
 * use-disabled-colour set) or the normal colour, and adds a drop shadow when the shadow bit is set. Each line
 * is measured, placed by func_0039FEB8 and drawn 3 pixels below the last; with ellipses on and several lines,
 * only the first line is drawn, followed by "...". When a flag word reached through the screen's engine has
 * bit 27 set (probably a debug option), every line is replaced by a row of 'W's as wide as the object.
 *
 * @param x  horizontal offset added to the object's position
 * @param y  vertical offset added to the object's position
 */
extern "C" void cUIText_render2D(sText_05E8* self, float x, float y)
{
    if (self->str == 0 || ((self->flags14 >> 2) & 1) == 0) {
        self->vt[16].fn((char*)self + self->vt[16].delta);
    }
    int* ov = *(int**)(*(char**)(*(char**)(self->owner + 0xD0) + 0x10) + 0x10);
    if (ov != 0 && ((*ov >> 27) & 1)) {
        sFont_05E8* font = (sFont_05E8*)cUIText_getFont(self);
        if (font == 0) {
            return;
        }
        int n = (int)(self->width / (func_00392BA8(font, 'W') * self->sx));
        int i = 0;
        if (n < 0x400) {
            for (i = 0; i < n; i++) {
                D_00508750[i] = 'W';
            }
        }
        D_00508750[i] = 0;
    }
    if (self->str == 0) {
        return;
    }
    sFont_05E8* font = (sFont_05E8*)cUIText_getFont(self);
    if (font == 0) {
        return;
    }
    sV2_05E8 s(self->sx, self->sy);
    font->cur.x = font->scale.x * s.x;
    font->cur.y = font->scale.y * s.y;
    if (((self->flags14 >> 5) & 1) && ((self->flags >> 4) & 1)) {
        font->color = self->hiColor;
    } else if (((self->flags14 >> 3) & 1) == 0 && ((self->flags >> 5) & 1)) {
        font->color = self->offColor;
    } else {
        font->color = self->color;
    }
    if ((self->flags >> 2) & 1) {
        sCol_05E8 c = self->shadowColor;
        c.r = self->color.r;
        font->shadowColor = c;
        font->shadowOfs = D_004A5A60;
    } else {
        font->shadowOfs = sV2_05E8(0.0f, 0.0f);
    }
    int* d = (int*)((char*)GrMan->m_pCurrentDrawState + 8);
    *d = (*d & ~0x3E0) | (((((sLayerBits_05E8*)&self->flags14)->layer + 9) << 5) & 0x3E0);
    sV3_05E8 pos(self->x + x, self->y + y, 0.0f);
    int i = 0;
    if (self->numLines > 0) do {
        unsigned short* str;
        if (i != 0) {
            str = self->lines[i];
        } else {
            str = self->str;
        }
        if (ov != 0 && ((*ov >> 27) & 1)) {
            str = D_00508750;
        }
        if (((self->flags >> 6) & 1) && self->numLines >= 2) {
            unsigned short* buf = (unsigned short*)operator_new_tag((USTR_length(str) + 5) * 2, D_00494078, 0x100, 0);
            func_002C2718(buf, D_004A47B0, str);
            sRect_05E8 r;
            func_003921F0_s(font, buf, &r, 0, s.x, s.y);
            sV3_05E8 size(r.x1, self->sy * ((float)font->height * font->scale.y), 0.0f);
            sV3_05E8 p = pos;
            func_0039FEB8(self, &p, &size);
            pos.y += (float)font->height * font->scale.y * s.y + 3.0f;
            func_00391E30_s(font, p.x, p.y, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
            return;
        }
        sRect_05E8 r;
        func_003921F0_s(font, str, &r, 0, s.x, s.y);
        i++;
        sV3_05E8 size(r.x1, self->sy * ((float)font->height * font->scale.y), 0.0f);
        sV3_05E8 p = pos;
        func_0039FEB8(self, &p, &size);
        pos.y += (float)font->height * font->scale.y * s.y + 3.0f;
        func_00391E30_s(font, p.x, p.y, str);
    } while (i < self->numLines);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_deleteText);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees the text object's string and line table and resets its line count.
 *
 * Sets mNumLines to 0, then frees mpLineOffset and mpString when they are set and clears both pointers.
 */
extern "C" void cUIText_deleteText(cUIText* self)
{
    self->mNumLines = 0;
    if ((void*)self->mpLineOffset != 0) {
        cMemMan_free(self->mpLineOffset);
        self->mpLineOffset = (unsigned short**)0;
    }
    if ((void*)self->mpString != 0) {
        cMemMan_free(self->mpString);
        self->mpString = (unsigned short*)0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getNumTextLines);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned short gUSTRBackSlash;
extern "C" char cUIText_getNumTextLines_(void* self) __asm__("cUIText_getNumTextLines");
/**
 * Counts the lines of a text object's string.
 *
 * A carriage return (13) or a doubled line-break character (gUSTRBackSlash twice) starts a new line.
 *
 * @return the number of lines, or 0 when there is no string
 */
extern "C" char cUIText_getNumTextLines_(void* self)
{
    unsigned short* text = *(unsigned short**)((char*)self + 0xB4);
    if (text == 0) return 0;
    unsigned short* p = text;
    int n = 1;
    while (*p != 0) {
        unsigned short c = *p;
        unsigned short next = p[1];
        if (c == gUSTRBackSlash && next == c) {
            p += 2;
            n++;
        } else if (c == 13) {
            p++;
            n++;
        } else {
            p++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getAsciiString);
#ifdef SKIP_ASM
/**
 * Returns the text's string when the text is not flagged as Unicode.
 *
 * Tests the mUnicode bit (bit 3) of mAttrText.
 *
 * @return mpString as an int when the bit is clear, otherwise 0
 */
extern "C" int cUIText_getAsciiString(cUIText* self)
{
    if (((self->mAttrText >> 3) & 1) == 0) {
        return (int)self->mpString;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getUnicodeString);
#ifdef SKIP_ASM
/**
 * Returns the text's string when the text is flagged as Unicode.
 *
 * Tests the mUnicode bit (bit 3) of mAttrText.
 *
 * @return mpString as an int when the bit is set, otherwise 0
 */
extern "C" int cUIText_getUnicodeString(cUIText* self)
{
    if (((self->mAttrText >> 3) & 1) != 0) {
        return (int)self->mpString;
    }
    return 0;
}
#endif

extern "C" void cUIText_setAsciiStringPrivate(cUIText* self, const char* str);

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiString__FP7cUITextPCc);
#ifdef SKIP_ASM
/**
 * Replaces the text with an ASCII string, detaching it from any localised string ID.
 *
 * Clears mStringID, then calls cUIText_setAsciiStringPrivate(), which converts @p str to UCS-2 and stores it
 * (strings of 511 or more characters are ignored).
 *
 * @param str  NUL-terminated ASCII text
 */
void cUIText_setAsciiString(cUIText* self, const char* str)
{
    self->mStringID = 0;
    cUIText_setAsciiStringPrivate(self, str);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiStringPrivate);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self, const unsigned short* str);

class cUIText_3A0C70 {
public:
    char pad[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
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
    virtual void v22();
    virtual void v23();
    virtual void v24(int);
};

/**
 * Sets a text object's string from an ASCII string.
 *
 * Marks the text cached (setCached(1), slot 5) and not Unicode (setUnicode(0), slot 24), then converts the
 * string to UCS-2 and stores it with cUIText_setUnicodeStringPrivate(). A string of 0x200 characters or more
 * is ignored.
 *
 * @param str  the ASCII string
 */
extern "C" void cUIText_setAsciiStringPrivate(cUIText* self, const char* str)
{
    unsigned short buf[0x200];
    ((cUIText_3A0C70*)self)->v05(1);
    ((cUIText_3A0C70*)self)->v24(0);
    if (strlen(str) + 1 < 0x200) {
        UIAsciiToUnicode(buf, str);
        cUIText_setUnicodeStringPrivate(self, buf);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_00392C60(void* font, unsigned short ch);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self, const unsigned short* str);
void cMemMan_free(void*);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00494078[];
extern unsigned short gUSTRBackSlash;

/**
 * Sets a text object's string, word-wrapped to the object's width.
 *
 * Copies the string, then walks it adding up character widths (scaled by mScale.x); when a line gets wider
 * than mSize.x, the last space is replaced by a carriage return. Doubled line-break markers reset the line
 * width. The result goes to cUIText_setUnicodeStringPrivate(). Nothing happens when the text has no font.
 *
 * @param str  the UCS-2 string
 */
extern "C" void cUIText_setUnicodeStringWrap(cUIText* self, unsigned short* str)
{
    ((cUIText_3A0C70*)self)->v05(1);
    void* font = cUIText_getFont(self);
    if (font == 0) {
        return;
    }
    int last = -1;
    float w = 0.0f;
    int len = USTR_length(str);
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, D_00494078, 0x100, 0);
    int i = 0;
    USTR_copy(buf, str);
    while (i < len) {
        if (buf[i] == gUSTRBackSlash && buf[i + 1] == buf[i]) {
            last = -1;
            w = 0.0f;
            i += 2;
        } else {
            w += func_00392C60(font, buf[i]) * self->mScale.x;
            if (buf[i] == 0x20) {
                last = i;
            }
            if (w > self->mSize.x && last >= 0) {
                buf[last] = 0xD;
                i = last;
                w = 0.0f;
                last = -1;
            }
            i++;
        }
    }
    cUIText_setUnicodeStringPrivate(self, buf);
    if (buf) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeString);
#ifdef SKIP_ASM
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self, const unsigned short* str);

/**
 * Replaces the text with a UCS-2 string, detaching it from any localised string ID.
 *
 * Clears mStringID, then calls cUIText_setUnicodeStringPrivate() to copy @p str and split it into lines.
 *
 * @param str  NUL-terminated UCS-2 text
 */
extern "C" void cUIText_setUnicodeString(cUIText* self, const unsigned short* str)
{
    self->mStringID = 0;
    cUIText_setUnicodeStringPrivate(self, str);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringPrivate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_deleteText(cUIText* self);
extern "C" int cUIText_getNumTextLines(void* self);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00494088[];
extern char D_00494098[];
extern unsigned short gUSTRBackSlash;

/**
 * Stores a copy of a UCS-2 string in a text object and splits it into lines.
 *
 * Marks the text cached and Unicode (slots 5 and 24), deletes the old text, copies the string (ignored if
 * longer than 800 characters) and counts its lines. With two or more lines it builds the line-start table,
 * cutting the copy at each carriage return and doubled line-break marker.
 *
 * @param self_  the text object
 * @param str    the string to store
 */
// Stores a copy of the string and splits it into lines at CR and at doubled line-break markers.
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self_, const unsigned short* str)
{
    ((cUIText_3A0C70*)self_)->v05(1);
    ((cUIText_3A0C70*)self_)->v24(1);
    cUIText_deleteText(self_);
    int len = USTR_length((unsigned short*)str);
    if (len < 0x321) {
        *(short*)&self_->mStringSize = len + 1;
        self_->mpString = (unsigned short*)operator_new_tag((len + 1) * 2, D_00494088, 0x100, 0);
        USTR_copy(self_->mpString, (unsigned short*)str);
        int n = cUIText_getNumTextLines(((char*)self_));
        self_->mNumLines = n;
        if (n >= 2) {
            unsigned short** lines = (unsigned short**)operator_new_tag(n * 4, D_00494098, 0x100, 0);
            unsigned short* text = self_->mpString;
            self_->mpLineOffset = lines;
            *(short*)&self_->mLineSize = *(unsigned short*)&self_->mNumLines;
            lines[0] = text;
            int k = 1;
            unsigned short* p = text;
            while (*p != 0) {
                unsigned short c = *p;
                unsigned short next = p[1];
                if (c == gUSTRBackSlash && next == c) {
                    *p = 0;
                    p += 2;
                    lines[k++] = p;
                } else if (c == 0xD) {
                    *p = 0;
                    p++;
                    lines[k++] = p;
                } else {
                    p++;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringByID__FP7cUITexti);
#ifdef SKIP_ASM
/**
 * Makes a text widget show a localised string, given the string's id.
 *
 * Stores @p id in mStringID and calls setCached(0) (vtable slot 5) so the text is no longer treated as cached.
 *
 * @param id  string id (hash of the text's key)
 */
void cUIText_setUnicodeStringByID(cUIText* self, int id)
{
    self->mStringID = id;
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
    vt[5].fn((char*)self + vt[5].delta, 0);  // virtual cUIThing::setCached(false)
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A1030);
#ifdef SKIP_ASM
extern "C" void* cUIText_getFont(void* self);
extern "C" float func_003921F0(void* font, int str, void* out, int flags, float sx, float sy);

struct func_003A1030_sVec3 {
    float x;
    float y;
    float z;
};
extern func_003A1030_sVec3 t3vect_Zero;

struct func_003A1030_sRect {
    float x0;
    float y0;
    float x1;
    float y1;
};

struct func_003A1030_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_003A1030(void* self, func_003A1030_sVec3* out)
{
    if (*(int*)((char*)self + 0xB4) == 0 || ((*(int*)((char*)self + 0x14) >> 2) & 1) == 0) {
        func_003A1030_sVEntry* vt = *(func_003A1030_sVEntry**)((char*)self + 8);
        vt[16].fn((char*)self + vt[16].delta);
    }
    void* font = cUIText_getFont(self);
    int str = *(int*)((char*)self + 0xB4);
    if (str != 0 && font != 0) {
        func_003A1030_sRect r;
        func_003921F0(font, str, &r, 0, *(float*)((char*)self + 0x50), *(float*)((char*)self + 0x54));
        out->x = r.x1;
        out->y = r.y1;
    } else {
        *out = t3vect_Zero;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A10F0);
#ifdef SKIP_ASM
struct func_003A10F0_sVEntry {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" int func_003A10F0(void* self)
{
    if (*(int*)((char*)self + 0xB4) == 0 || ((*(int*)((char*)self + 0x14) >> 2) & 1) == 0) {
        func_003A10F0_sVEntry* vt = *(func_003A10F0_sVEntry**)((char*)self + 8);
        vt[16].fn((char*)self + vt[16].delta);
    }
    return *(int*)((char*)self + 0xA8);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_cUITextScroll);
#ifdef SKIP_ASM
extern "C" void* func_0039FB30(void* self, int a1, int a2);
extern "C" void* func_003977E8(void* self);
extern "C" void cUITextScroll_setInputMap(cUITextScroll* self, char a1);
extern void* cUITextScroll_vtbl[];

extern "C" void* cUITextScroll_cUITextScroll(cUITextScroll* self, int a1, int a2)
{
    func_0039FB30(self, a1, a2);
    self->mVtbl = cUITextScroll_vtbl;
    *(unsigned char*)&self->mAttrTextScroll &= 0xC6;
    func_003977E8(((char*)self) + 0x8C);
    self->mpScrollBar = 0;
    self->mpLineOffset = 0;
    self->mStringID = (unsigned int)0;
    *(short*)&self->mSelectedIndex = 0;
    *(short*)&self->mLinesOffset = 0;
    *(short*)&self->mLineSize = 0;
    *(int*)&self->mPixelOffset = 0;
    cUITextScroll_setInputMap(self, 0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setUIData);
#ifdef SKIP_ASM
// PORT: cUIThing_setUIData really takes (self, data); other units declare it with one argument.
void* cUIThing_setUIData_2(void* self, void* data) __asm__("cUIThing_setUIData");
// PORT: called with only self here (the unit declares two arguments).
void cUITextScroll_clearBuffer_1(void* self) __asm__("cUITextScroll_clearBuffer");
extern "C" void cUITextScroll_addUnicodeStringByID(cUITextScroll* self, int id);

struct sUIColor11B8 {
    float r, g, b, a;
    sUIColor11B8() {}
    sUIColor11B8(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};
struct sUIVt11B8 { short delta; short index; void (*fn)(void*, int); };
struct sUIText11B8 {
    char pad0[0x8];
    sUIVt11B8* vt;              // 0x8
    char padC[0x74 - 0xC];
    unsigned int b0 : 1;        // 0x74
    unsigned int mode : 2;
    unsigned int rest : 29;
    sUIColor11B8 color;         // 0x78
};
struct sUIData11B8 {
    char pad0[0x20];
    unsigned char mode;         // 0x20
    unsigned char vis;          // 0x21
    char pad22[2];
    int textId;                 // 0x24
    unsigned char r, g, b, a;   // 0x28
};

extern "C" void cUITextScroll_setUIData(sUIText11B8* self, sUIData11B8* data)
{
    cUIThing_setUIData_2(self, data);
    self->mode = data->mode & 3;
    self->vt[23].fn((char*)self + self->vt[23].delta, data->vis & 1);
    if (data->textId != 0) {
        cUITextScroll_clearBuffer_1(self);
        cUITextScroll_addUnicodeStringByID((cUITextScroll*)self, data->textId);
    }
    int f = *(int*)((char*)self + 0x74) >> 3;
    if (f & 1) {
        self->color = sUIColor11B8(data->r * 0.003921568859368563f, data->g * 0.003921568859368563f,
                                   data->b * 0.003921568859368563f, data->a * 0.003921568859368563f);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_getFont);
#ifdef SKIP_ASM
extern "C" int cUITextScroll_getFont(cUITextScroll* self)
{
    void* p = *(void**)((char*)*(void**)((char*)self->mpScreen + 0xd0) + 0x10);
    if (p != 0) {
        return *(int*)((char*)*(void**)((char*)p + 0x8) + ((*(unsigned int*)&self->mAttrTextScroll >> 1) & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setInputMap);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_setInputMap(cUITextScroll* self, char a1)
{
    if (a1 == 0) {
        *(char*)((char*)self + 0x88) = 0;
        *(char*)((char*)self + 0x89) = 1;
        *(char*)((char*)self + 0x8A) = 2;
        *(char*)((char*)self + 0x8B) = 3;
    } else {
        *(char*)((char*)self + 0x88) = 4;
        *(char*)((char*)self + 0x89) = 5;
        *(char*)((char*)self + 0x8A) = 6;
        *(char*)((char*)self + 0x8B) = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_updateLocale);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_setUnicodeStringWrap(cUITextScroll* self, void* a1);

class cUITextScroll_updateLocale_cVirtA {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* v04(int);
};

class cUITextScroll_updateLocale_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};

extern "C" void cUITextScroll_updateLocale(cUITextScroll* self)
{
    int id = (int)self->mStringID;
    if (id != 0) {
        cUITextScroll_updateLocale_cVirtA* a = *(cUITextScroll_updateLocale_cVirtA**)((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x10);
        if (a != 0) {
            void* r = a->v04(id);
            if (r != 0) {
                cUITextScroll_setUnicodeStringWrap(self, r);
            }
            ((cUITextScroll_updateLocale_cVirtB*)self)->v05(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_onInput);
#ifdef SKIP_ASM
struct sUIInputVE13E0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sUIFont13E0 {
    char pad_0x0[0x14];
    int height;             // 0x14
    char pad_0x18[0x1C];
    float scale;            // 0x34
};

struct sUIText13E0 {
    char pad_0x0[0x54];
    float scale;            // 0x54
    char pad_0x58[0xC];
    float height;           // 0x64
    char pad_0x68[0xC];
    unsigned int flags;     // 0x74
    char pad_0x78[0x10];
    signed char keyUp;      // 0x88
    signed char keyDown;    // 0x89
    char pad_0x8A[0x3A];
    unsigned short cur;     // 0xC4
    unsigned short top;     // 0xC6
    unsigned short count;   // 0xC8
};

static inline int pressed13E0(char* input, int key)
{
    sUIInputVE13E0* e = &(*(sUIInputVE13E0**)(input + 8))[2];
    return e->fn(input + e->delta, key);
}

extern "C" int cUITextScroll_onInput(sUIText13E0* self, char* input)
{
    sUIFont13E0* font = (sUIFont13E0*)cUITextScroll_getFont((cUITextScroll*)self);
    if (font == 0) {
        return 0;
    }
    signed char rows = (int)(self->height / (self->scale * ((float)font->height * font->scale) + 3.0f));
    if (self->flags & 1) {
        if (pressed13E0(input, self->keyUp) != 0) {
            if (self->cur != 0) {
                self->cur--;
            }
            return 1;
        }
        if (pressed13E0(input, self->keyDown) != 0) {
            if (self->cur < self->count - 1) {
                self->cur++;
            }
            return 1;
        }
        short d = self->cur - self->top;
        if (d < 0) {
            self->top = self->cur;
        } else if (d >= rows) {
            self->top = self->cur - rows + 1;
        }
    } else {
        if (pressed13E0(input, self->keyUp) != 0) {
            if (self->top != 0) {
                self->top--;
            }
            return 1;
        }
        if (pressed13E0(input, self->keyDown) == 0) {
            return 0;
        }
        if (self->top < self->count - rows) {
            self->top++;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_render2D);
#ifdef SKIP_ASM
extern "C" int cUITextScroll_getFont(cUITextScroll* self);
extern "C" float func_003921F0(void* font, int str, void* out, int flags, float sx, float sy);
extern "C" void func_0039FEB8(void* self, void* pos, void* size);
extern "C" void func_00391E30(void* font, float x, float y, int str);
extern "C" void func_0039DA20(void* sb, int top, int rows, int count);

struct sV2_1588 {
    float x, y;
    sV2_1588() {}
    sV2_1588(float ax, float ay) : x(ax), y(ay) {}
};
struct sV3_1588 {
    float x, y, z;
    sV3_1588() {}
    sV3_1588(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sCol_1588 {
    float r, g, b, a;
};
struct sRect_1588 {
    float x0, y0, x1, y1;
};

struct sFont_1588 {
    char pad0[0x14];
    int height;             // 0x14
    char pad18[0x10];
    sV2_1588 shadowOfs;     // 0x28
    sV2_1588 scale;         // 0x30
    sV2_1588 cur;           // 0x38
    sCol_1588 color;        // 0x40
    sCol_1588 shadowColor;  // 0x50
};

struct sLayerBits_1588 {
    unsigned char lo;
    unsigned char layer : 5;
};
struct sDrawBits_1588 {
    unsigned int lo : 5;
    unsigned int layer : 5;
};

struct sVE_1588 { short delta; short index; void (*fn)(void*); };

struct sText_1588 {
    char pad0[0x8];
    sVE_1588* vt;           // 0x08
    char padC[0x8];
    int flags14;            // 0x14
    char pad18[0x4];
    sCol_1588 color;        // 0x1C
    char pad2C[0x18];
    float x;                // 0x44
    float y;                // 0x48
    char pad4C[0x4];
    float sx;               // 0x50
    float sy;               // 0x54
    char pad58[0xC];
    float height;           // 0x64
    char pad68[0xC];
    int flags;              // 0x74
    sCol_1588 shadowColor;  // 0x78
    char pad88[0x20];
    sCol_1588 selColor;     // 0xA8
    void* scroll;           // 0xB8
    int* strs;              // 0xBC
    char padC0[0x4];
    unsigned short cur;     // 0xC4
    unsigned short top;     // 0xC6
    unsigned short count;   // 0xC8
    char padCA[0x2];
    float yofs;             // 0xCC
};

extern "C" void cUITextScroll_render2D(sText_1588* self, float x, float y)
{
    if (((self->flags14 >> 2) & 1) == 0) {
        self->vt[16].fn((char*)self + self->vt[16].delta);
    }
    sFont_1588* font = (sFont_1588*)cUITextScroll_getFont((cUITextScroll*)self);
    if (font == 0) {
        return;
    }
    sV2_1588 s(self->sx, self->sy);
    font->cur.x = font->scale.x * s.x;
    font->cur.y = font->scale.y * s.y;
    if ((self->flags >> 3) & 1) {
        sCol_1588 c = self->shadowColor;
        c.r = self->color.r;
        font->shadowColor = c;
        font->shadowOfs = sV2_1588(2.0f, 2.0f);
    } else {
        font->shadowOfs = sV2_1588(0.0f, 0.0f);
    }
    int* d = (int*)((char*)GrMan->m_pCurrentDrawState + 8);
    *d = (*d & ~0x3E0) | (((((sLayerBits_1588*)&self->flags14)->layer + 9) << 5) & 0x3E0);
    sV3_1588 pos(self->x + x, self->y + y, 0.0f);
    float lineH = self->sy * ((float)font->height * font->scale.y) + 3.0f;
    signed char rows = (int)(self->height / lineH);
    int i = 0;
    if (rows > 0) do {
        unsigned short idx = self->top + i;
        if (idx >= self->count) {
            break;
        }
        if ((self->flags & 1) && idx == self->cur) {
            font->color = self->selColor;
        } else {
            font->color = self->color;
        }
        int str = self->strs[i + self->top];
        i++;
        sRect_1588 r;
        func_003921F0(font, str, &r, 0, self->sx, self->sy);
        sV3_1588 size(r.x1, self->sy * ((float)font->height * font->scale.y), 0.0f);
        sV3_1588 p = pos;
        func_0039FEB8(self, &p, &size);
        pos.y += lineH;
        func_00391E30(font, p.x, p.y + self->yofs, str);
    } while (i < rows);
    if (self->scroll != 0) {
        func_0039DA20(self->scroll, self->top, rows, self->count);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_clearBuffer);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void cUITextScroll_clearBuffer(cUITextScroll* self, void* a1)
{
    if ((void**)self->mpLineOffset != 0) {
        unsigned short i;
        for (i = 0; i < self->mLineSize; i++) {
            if (((void**)self->mpLineOffset)[i] != 0) {
                cMemMan_free(((void**)self->mpLineOffset)[i]);
            }
        }
        if ((void**)self->mpLineOffset != 0) {
            cMemMan_free((void**)self->mpLineOffset);
        }
        self->mpLineOffset = 0;
    }
    self->mLinesOffset = 0;
    self->mLineSize = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_allocLine);
#ifdef SKIP_ASM
extern "C" void* func_0041605C(void* dst, const void* src, int n);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004940A8[];

/**
 * Adds one slot to a scrolling text's line table and returns its index.
 *
 * Grows the line pointer array (+0xBC) by one entry, copies the old pointers and frees the old array. The new
 * slot is cleared only when an old array existed.
 *
 * @return index of the new line slot (the old line count)
 */
extern "C" int cUITextScroll_allocLine(cUITextScroll* self)
{
    unsigned short old = self->mLineSize;
    self->mLineSize = old + 1;
    void** arr = (void**)operator_new_tag(self->mLineSize * 4, D_004940A8, 0x100, 0);
    if ((void**)self->mpLineOffset != 0) {
        func_0041605C(arr, (void**)self->mpLineOffset, old * 4);
        void** prev = (void**)self->mpLineOffset;
        arr[old] = 0;
        if (prev != 0) {
            cMemMan_free(prev);
        }
    }
    self->mpLineOffset = (unsigned short*)arr;
    return old;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_addAsciiString);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_addUnicodeString(cUITextScroll* self, unsigned short* str);
extern "C" void func_002C2540(void* dst, void* src);

extern "C" void cUITextScroll_addAsciiString(cUITextScroll* self, char* str)
{
    ((cUIText_3A0C70*)self)->v05(1);
    void* font = (void*)cUITextScroll_getFont(self);
    if (font == 0) {
        return;
    }
    int last = -1;
    int len = strlen(str);
    int i = 0;
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, D_00494078, 0x100, 0);
    float w = 0.0f;
    int start = 0;
    func_002C2540(buf, str);
    while (i < len) {
        if (buf[i] == 0x5C && buf[i + 1] == buf[i]) {
            buf[i] = 0;
            i += 2;
            cUITextScroll_addUnicodeString(self, &buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else if (buf[i] == 0xD) {
            buf[i] = 0;
            i += 1;
            cUITextScroll_addUnicodeString(self, &buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else {
            w += func_00392C60(font, buf[i]) * self->mScale.x;
            if (buf[i] == 0x20) {
                last = i;
            }
            if (w > self->mSize.x) {
                if (last < 0) {
                    last = i - 1;
                }
                if (last > 0) {
                    buf[last] = 0;
                    i = last;
                    w = 0.0f;
                    cUITextScroll_addUnicodeString(self, &buf[start]);
                    last = -1;
                    start = i + 1;
                }
            }
            i++;
        }
    }
    if (start != i) {
        cUITextScroll_addUnicodeString(self, &buf[start]);
    }
    if (buf) {
        cMemMan_free(buf);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeString);
#ifdef SKIP_ASM
extern "C" int cUITextScroll_allocLine(cUITextScroll* self);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_004940B8[];

/**
 * Appends a copy of a UCS-2 string to a scrolling text as a new line.
 *
 * @param str  the string to add
 */
extern "C" void cUITextScroll_addUnicodeString(cUITextScroll* self, unsigned short* str)
{
    int idx = cUITextScroll_allocLine(self);
    unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, D_004940B8, 0x100, 0);
    USTR_copy(copy, str);
    ((unsigned short**)self->mpLineOffset)[idx] = copy;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setAsciiString);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef void (*fnUT1C80)(char*, int);

struct sUTVEntry1C80 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cUITextScroll_setAsciiString(cUITextScroll* self, char* str)
{
    cUITextScroll_clearBuffer_1(self);
    self->mStringID = (unsigned int)0;
    ((sUTVEntry1C80*)self->mVtbl)[5].fn((char*)self + ((sUTVEntry1C80*)self->mVtbl)[5].delta, 1);
    ((sUTVEntry1C80*)self->mVtbl)[24].fn((char*)self + ((sUTVEntry1C80*)self->mVtbl)[24].delta, 0);
    cUITextScroll_addAsciiString(self, str);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_clearBuffer(cUITextScroll*, void*);
extern "C" void cUITextScroll_addUnicodeStringWrap(cUITextScroll*, void*);

extern "C" void cUITextScroll_setUnicodeStringWrap(cUITextScroll* self, void* a1)
{
    cUITextScroll_clearBuffer(self, a1);
    cUITextScroll_addUnicodeStringWrap(self, a1);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_addUnicodeString(cUITextScroll* self, unsigned short* str);

extern "C" void cUITextScroll_addUnicodeStringWrap(cUITextScroll* self, void* vstr)
{
    unsigned short* str = (unsigned short*)vstr;
    ((cUIText_3A0C70*)self)->v05(1);
    void* font = (void*)cUITextScroll_getFont(self);
    if (font == 0) {
        return;
    }
    int last = -1;
    int len = USTR_length(str);
    int i = 0;
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, D_00494078, 0x100, 0);
    float w = 0.0f;
    int start = 0;
    USTR_copy(buf, str);
    while (i < len) {
        if (buf[i] == 0x5C && buf[i + 1] == buf[i]) {
            buf[i] = 0;
            i += 2;
            cUITextScroll_addUnicodeString(self, &buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else {
            w += func_00392C60(font, buf[i]) * self->mScale.x;
            if (buf[i] == 0x20) {
                last = i;
            }
            if (w > self->mSize.x) {
                if (last < 0) {
                    last = i - 1;
                }
                if (last > 0) {
                    buf[last] = 0;
                    i = last;
                    w = 0.0f;
                    cUITextScroll_addUnicodeString(self, &buf[start]);
                    last = -1;
                    start = i + 1;
                }
            }
            i++;
        }
    }
    if (start != i) {
        cUITextScroll_addUnicodeString(self, &buf[start]);
    }
    if (buf) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeStringByID);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_addUnicodeStringWrap(cUITextScroll*, void*);

class cUITextScroll_addUnicodeStringByID_cVirtA {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void* v04(int);
};

class cUITextScroll_addUnicodeStringByID_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};

extern "C" void cUITextScroll_addUnicodeStringByID(cUITextScroll* self, int id)
{
    self->mStringID = (unsigned int)id;
    cUITextScroll_addUnicodeStringByID_cVirtA* a = *(cUITextScroll_addUnicodeStringByID_cVirtA**)((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x10);
    if (a != 0) {
        void* r = a->v04(id);
        if (r != 0) {
            cUITextScroll_addUnicodeStringWrap(self, r);
        }
        ((cUITextScroll_addUnicodeStringByID_cVirtB*)self)->v05(1);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_deleteLineByIndex);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_deleteLineByIndex(cUITextScroll* self, int idx)
{
    if ((void**)self->mpLineOffset != 0) {
        unsigned short n = self->mLineSize;
        if (idx < n) {
            void** arr = (void**)operator_new_tag((n - 1) * 4, D_004940A8, 0x100, 0);
            if (idx > 0) {
                func_0041605C(arr, (void**)self->mpLineOffset, idx * 4);
            }
            if (idx < self->mLineSize - 1) {
                // PORT: pointer held in int (only spelling found that gives idx-first addu)
                func_0041605C((void*)(idx * 4 + (int)arr), (void*)(idx * 4 + (int)self->mpLineOffset + 4), (self->mLineSize - idx - 1) * 4);
            }
            if ((void**)self->mpLineOffset != 0) {
                cMemMan_free((void**)self->mpLineOffset);
            }
            self->mpLineOffset = (unsigned short*)arr;
            (self->mLineSize)--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A2068);
#ifdef SKIP_ASM
extern "C" int func_003A2068(void* self, int i)
{
    int* arr = *(int**)((char*)self + 0xBC);
    if (arr != 0 && i >= 0 && i < *(unsigned short*)((char*)self + 0xC8)) {
        return arr[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", func_003A20A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_20A0 {
    float x, y, w, h;
};
struct sColor_20A0 {
    float a, r, g, b;
};
struct sGfx_20A0 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// One vertex of a submitted quad (stride 0x30).
struct sQuadVtx_20A0 {
    float s, t, q;
    int pad;
    int r, g, b, a;
    float pos[4];
    sQuadVtx_20A0() {}
};
struct sSprite_20A0 {
    int tex;                // 0x0
    char pad[8];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};
// The unit declares GrMan as char*; typed view by asm label.
extern int D_004A28AC_20A0 __asm__("D_004A28AC");
extern "C" void func_0038E118(int ctx, sQuadVtx_20A0* v, sRect_20A0* r);
extern "C" void func_0038E200(int ctx, sQuadVtx_20A0* v, sRect_20A0* r);

extern "C" void func_003A20A0(sSprite_20A0* o, sRect_20A0 r, sColor_20A0 c, int flags)
{
    int tex = o->tex;
    if (tex < 0) {
        return;
    }
    (GrMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    float v0 = o->v0;
    float v1 = o->v1;
    float u0 = o->u0;
    float u1 = o->u1;
    if (flags & 1) {
        u0 = u1;
        u1 = o->u0;
    }
    if (flags & 2) {
        v0 = v1;
        v1 = o->v0;
    }
    sQuadVtx_20A0 v[4];
    for (int i = 0; i < 4; i++) {
        sQuadVtx_20A0* p = &v[i];
        p->a = (int)(c.a * 128.0f);
        p->r = (int)(c.r * 128.0f);
        p->g = (int)(c.g * 128.0f);
        p->b = (int)(c.b * 128.0f);
        p->q = 1.0f;
    }
    v[0].s = u0;
    v[0].t = v0;
    v[1].s = u1;
    v[1].t = v0;
    v[2].s = u0;
    v[2].t = v1;
    v[3].s = u1;
    v[3].t = v1;
    if (flags & 4) {
        func_0038E200(D_004A28AC_20A0, v, &r);
    } else {
        func_0038E118(D_004A28AC_20A0, v, &r);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", func_003A2220);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_2220 {
    float x, y, w, h;
};
struct sColor_2220 {
    float a, r, g, b;
};
struct sGfx_2220 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// One vertex of a submitted quad (stride 0x30).
struct sQuadVtx_2220 {
    float s, t, q;
    int pad;
    int r, g, b, a;
    float pos[4];
    sQuadVtx_2220() {}
};
struct sSprite_2220 {
    int tex;                // 0x0
    char pad[8];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};
// The unit declares GrMan as char*; typed views by asm label.
extern int D_004A28AC_2220 __asm__("D_004A28AC");
extern void func_0038E118_2220(int ctx, sQuadVtx_2220* v, sRect_2220* r) __asm__("func_0038E118");
extern void func_0038E200_2220(int ctx, sQuadVtx_2220* v, sRect_2220* r) __asm__("func_0038E200");

extern "C" void func_003A2220(sSprite_2220* o, sRect_2220 r, sColor_2220 c, int flags, float t)
{
    float k = 1.0f - t;
    int tex = o->tex;
    (GrMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    float v0 = o->v0;
    float v1 = o->v1;
    float u0 = o->u0;
    float u1 = o->u1;
    r.y += k * r.h;
    r.h *= 1.0f - k;
    v0 += k * (v1 - v0);
    sQuadVtx_2220 v[4];
    for (int i = 0; i < 4; i++) {
        sQuadVtx_2220* p = &v[i];
        p->a = (int)(c.a * 128.0f);
        p->r = (int)(c.r * 255.0f);
        p->g = (int)(c.g * 255.0f);
        p->b = (int)(c.b * 255.0f);
        p->q = 1.0f;
    }
    v[0].s = u0;
    v[0].t = v0;
    v[1].s = u1;
    v[1].t = v0;
    v[2].s = u0;
    v[2].t = v1;
    v[3].s = u1;
    v[3].t = v1;
    if (flags & 4) {
        func_0038E200_2220(D_004A28AC_2220, v, &r);
    } else {
        func_0038E118_2220(D_004A28AC_2220, v, &r);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", func_003A23B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_23B8 {
    float x, y, w, h;
    sRect_23B8() {}
    sRect_23B8(float x_, float y_, float w_, float h_) : x(x_), y(y_), w(w_), h(h_) {}
};
struct sColor_23B8 {
    float a, r, g, b;
};
struct sGfx_23B8 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// One vertex of a submitted quad (stride 0x30).
struct sQuadVtx_23B8 {
    float s, t, q;
    int pad;
    int r, g, b, a;
    float pos[4];
    sQuadVtx_23B8() {}
};
struct sSprite_23B8 {
    int tex;                // 0x0
    char pad[8];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};
extern int D_004A28AC_23B8 __asm__("D_004A28AC");
extern "C" void func_0038E5E8(int ctx, sQuadVtx_23B8* v, sRect_23B8* dst, sRect_23B8* uv);
extern "C" void func_0038E738(int ctx, sQuadVtx_23B8* v, sRect_23B8* dst, sRect_23B8* uv);

// Draws the sprite tiled tx by ty times over r (fractional tiles are cropped).
extern "C" void func_003A23B8(sSprite_23B8* o, sRect_23B8 r, sColor_23B8 c, int flags, float tx, float ty)
{
    int tex = o->tex;
    (GrMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    float x0 = r.x;
    float y0 = r.y;
    float v0 = o->v0;
    float v1 = o->v1;
    float u0 = o->u0;
    float u1 = o->u1;
    if (tx <= 0.0f) {
        return;
    }
    if (ty <= 0.0f) {
        return;
    }
    float w = r.w / tx;
    float h = r.h / ty;
    if (flags & 1) {
        u0 = u1;
        u1 = o->u0;
    }
    if (flags & 2) {
        v0 = v1;
        v1 = o->v0;
    }
    int nx = (int)tx;
    float y = y0;
    int ny = (int)ty;
    sQuadVtx_23B8 v[4];
    float fny = (float)ny;
    for (int i = 0; i < 4; i++) {
        sQuadVtx_23B8* p = &v[i];
        p->a = (int)(c.a * 128.0f);
        p->r = (int)(c.r * 255.0f);
        p->g = (int)(c.g * 255.0f);
        p->b = (int)(c.b * 255.0f);
        p->q = 1.0f;
    }
    int j;
    float x;
    if (ny != 0) {
        for (y = y0, j = 0; j < ny; j++) {
            x = x0;
            if (flags & 4) {
                for (int i = 0; i < nx; i++) {
                    sRect_23B8 dst(x, y, w, h);
                    sRect_23B8 uv(u0, v0, u1 - u0, v1 - v0);
                    func_0038E738(D_004A28AC_23B8, v, &dst, &uv);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_23B8 dst(x, y, (x0 + r.w) - x, h);
                    sRect_23B8 uv(u0, v0, u1 - u0, (v0 + (v1 - v0) * (tx - (float)nx)) - v0);
                    func_0038E738(D_004A28AC_23B8, v, &dst, &uv);
                }
            } else {
                for (int i = 0; i < nx; i++) {
                    sRect_23B8 dst(x, y, w, h);
                    sRect_23B8 uv(u0, v0, u1 - u0, v1 - v0);
                    func_0038E5E8(D_004A28AC_23B8, v, &dst, &uv);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_23B8 dst(x, y, (x0 + r.w) - x, h);
                    sRect_23B8 uv(u0, v0, (u0 + (u1 - u0) * (tx - (float)nx)) - u0, v1 - v0);
                    func_0038E5E8(D_004A28AC_23B8, v, &dst, &uv);
                }
            }
            y += h;
        }
    }
    if (fny != ty) {
        x = x0;
        if (flags & 4) {
            float u0b = u0 + (u1 - u0) * (1.0f - (ty - fny));
            float hh = (y0 + r.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_23B8 dst(x, y, w, hh);
                sRect_23B8 uv(u0b, v0, u1 - u0b, v1 - v0);
                func_0038E738(D_004A28AC_23B8, v, &dst, &uv);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_23B8 dst(x, y, (x0 + r.w) - x, hh);
                sRect_23B8 uv(u0b, v0, u1 - u0b, (v0 + (v1 - v0) * (tx - (float)nx)) - v0);
                func_0038E738(D_004A28AC_23B8, v, &dst, &uv);
            }
        } else {
            float v1b = v0 + (v1 - v0) * (ty - fny);
            float hh = (y0 + r.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_23B8 dst(x, y, w, hh);
                sRect_23B8 uv(u0, v0, u1 - u0, v1b - v0);
                func_0038E5E8(D_004A28AC_23B8, v, &dst, &uv);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_23B8 dst(x, y, (x0 + r.w) - x, hh);
                sRect_23B8 uv(u0, v0, (u0 + (u1 - u0) * (tx - (float)nx)) - u0, v1b - v0);
                func_0038E5E8(D_004A28AC_23B8, v, &dst, &uv);
            }
        }
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", func_003A2A00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_2A00 {
    float x, y, w, h;
};
struct sColor_2A00 {
    float a, r, g, b;
};
struct sMat3_2A00 {
    float m[9];
};
struct sGfx_2A00 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// One vertex of a submitted quad (stride 0x30).
struct sQuadVtx_2A00 {
    float s, t, q;
    int pad;
    int r, g, b, a;
    float pos[4];
    sQuadVtx_2A00() {}
};
struct sSprite_2A00 {
    int tex;                // 0x0
    char pad[8];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};
// The unit declares GrMan as char*; typed views by asm label.
extern int D_004A28AC_2A00 __asm__("D_004A28AC");
extern "C" void func_0038E2E8(int ctx, sQuadVtx_2A00* v, sRect_2A00* r, sMat3_2A00* m);
extern "C" void func_0038E468(int ctx, sQuadVtx_2A00* v, sRect_2A00* r, sMat3_2A00* m);

extern "C" void func_003A2A00(sSprite_2A00* o, sRect_2A00 r, sMat3_2A00 m, sColor_2A00 c, int flags)
{
    int tex = o->tex;
    (GrMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    float v0 = o->v0;
    float v1 = o->v1;
    float u0 = o->u0;
    float u1 = o->u1;
    if (flags & 1) {
        u0 = u1;
        u1 = o->u0;
    }
    if (flags & 2) {
        v0 = v1;
        v1 = o->v0;
    }
    sQuadVtx_2A00 v[4];
    for (int i = 0; i < 4; i++) {
        sQuadVtx_2A00* p = &v[i];
        p->a = (int)(c.a * 128.0f);
        p->r = (int)(c.r * 255.0f);
        p->g = (int)(c.g * 255.0f);
        p->b = (int)(c.b * 255.0f);
        p->q = 1.0f;
    }
    v[0].s = u0;
    v[0].t = v0;
    v[1].s = u1;
    v[1].t = v0;
    v[2].s = u0;
    v[2].t = v1;
    v[3].s = u1;
    v[3].t = v1;
    if (flags & 4) {
        func_0038E468(D_004A28AC_2A00, v, &r, &m);
    } else {
        func_0038E2E8(D_004A28AC_2A00, v, &r, &m);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", func_003A2BC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sRect_2BC8 {
    float x, y, w, h;
    sRect_2BC8() {}
    sRect_2BC8(float x_, float y_, float w_, float h_) : x(x_), y(y_), w(w_), h(h_) {}
};
struct sColor_2BC8 {
    float a, r, g, b;
};
struct sMat3_2BC8 {
    float m[9];
};
struct sGfx_2BC8 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// One vertex of a submitted quad (stride 0x30).
struct sQuadVtx_2BC8 {
    float s, t, q;
    int pad;
    int r, g, b, a;
    float pos[4];
    sQuadVtx_2BC8() {}
};
struct sSprite_2BC8 {
    int tex;                // 0x0
    char pad[8];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};
extern int D_004A28AC_2BC8 __asm__("D_004A28AC");
extern "C" void func_0038E880(int ctx, sQuadVtx_2BC8* v, sRect_2BC8* dst, sRect_2BC8* uv, sMat3_2BC8* m);
extern "C" void func_0038EA60(int ctx, sQuadVtx_2BC8* v, sRect_2BC8* dst, sRect_2BC8* uv, sMat3_2BC8* m);

// Tiled draw like func_003A23B8, with an extra 3x3 transform m.
extern "C" void func_003A2BC8(sSprite_2BC8* o, sRect_2BC8 r, sMat3_2BC8 m, sColor_2BC8 c, int flags, float tx, float ty)
{
    int j;
    float x;
    int tex = o->tex;
    (GrMan->m_pCurrentDrawState)->m_textureState.m_texid[0] = tex;
    float x0 = r.x;
    float y0 = r.y;
    float v0 = o->v0;
    float v1 = o->v1;
    float u0 = o->u0;
    float u1 = o->u1;
    if (tx <= 0.0f) {
        return;
    }
    if (ty <= 0.0f) {
        return;
    }
    float w = r.w / tx;
    float h = r.h / ty;
    if (flags & 1) {
        u0 = u1;
        u1 = o->u0;
    }
    if (flags & 2) {
        v0 = v1;
        v1 = o->v0;
    }
    int nx = (int)tx;
    float y = y0;
    int ny = (int)ty;
    sQuadVtx_2BC8 v[4];
    for (int i = 0; i < 4; i++) {
        sQuadVtx_2BC8* p = &v[i];
        p->a = (int)(c.a * 128.0f);
        p->r = (int)(c.r * 255.0f);
        p->g = (int)(c.g * 255.0f);
        p->b = (int)(c.b * 255.0f);
        p->q = 1.0f;
    }
    if (ny != 0) {
        for (y = y0, j = 0; j < ny; j++) {
            x = x0;
            if (flags & 4) {
                for (int i = 0; i < nx; i++) {
                    sRect_2BC8 dst(x, y, w, h);
                    sRect_2BC8 uv(u0, v0, u1 - u0, v1 - v0);
                    func_0038EA60(D_004A28AC_2BC8, v, &dst, &uv, &m);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_2BC8 dst(x, y, (x0 + r.w) - x, h);
                    sRect_2BC8 uv(u0, v0, u1 - u0, (v0 + (v1 - v0) * (tx - (float)nx)) - v0);
                    func_0038EA60(D_004A28AC_2BC8, v, &dst, &uv, &m);
                }
            } else {
                for (int i = 0; i < nx; i++) {
                    sRect_2BC8 dst(x, y, w, h);
                    sRect_2BC8 uv(u0, v0, u1 - u0, v1 - v0);
                    func_0038E880(D_004A28AC_2BC8, v, &dst, &uv, &m);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_2BC8 dst(x, y, (x0 + r.w) - x, h);
                    sRect_2BC8 uv(u0, v0, (u0 + (u1 - u0) * (tx - (float)nx)) - u0, v1 - v0);
                    func_0038E880(D_004A28AC_2BC8, v, &dst, &uv, &m);
                }
            }
            y += h;
        }
    }
    if ((float)ny != ty) {
        x = x0;
        if (flags & 4) {
            float u0b = u0 + (u1 - u0) * (1.0f - (ty - (float)ny));
            float hh = (y0 + r.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_2BC8 dst(x, y, w, hh);
                sRect_2BC8 uv(u0b, v0, u1 - u0b, v1 - v0);
                func_0038EA60(D_004A28AC_2BC8, v, &dst, &uv, &m);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_2BC8 dst(x, y, (x0 + r.w) - x, hh);
                sRect_2BC8 uv(u0b, v0, u1 - u0b, (v0 + (v1 - v0) * (tx - (float)nx)) - v0);
                func_0038EA60(D_004A28AC_2BC8, v, &dst, &uv, &m);
            }
        } else {
            float v1b = v0 + (v1 - v0) * (ty - (float)ny);
            float hh = (y0 + r.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_2BC8 dst(x, y, w, hh);
                sRect_2BC8 uv(u0, v0, u1 - u0, v1b - v0);
                func_0038E880(D_004A28AC_2BC8, v, &dst, &uv, &m);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_2BC8 dst(x, y, (x0 + r.w) - x, hh);
                sRect_2BC8 uv(u0, v0, (u0 + (u1 - u0) * (tx - (float)nx)) - u0, v1b - v0);
                func_0038E880(D_004A28AC_2BC8, v, &dst, &uv, &m);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIVector_cUIVector);
#ifdef SKIP_ASM
extern "C" void* func_0039FB30(void* self, int a1, int a2);
extern void* cUIVector_vtbl[];

struct cUIVector_cUIVector_s {
    char pad0[0x8];
    void** vtbl; // 0x8
    char padC[0x74 - 0xC];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int mid : 15;
    unsigned int hi : 15;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
};

extern "C" cUIVector_cUIVector_s* cUIVector_cUIVector(cUIVector* self, int a1, int a2)
{
    func_0039FB30(((cUIVector_cUIVector_s*)self), a1, a2);
    self->mVtbl = cUIVector_vtbl;
    ((cUIVector_cUIVector_s*)self)->b0 = 0;
    ((cUIVector_cUIVector_s*)self)->b1 = 0;
    ((cUIVector_cUIVector_s*)self)->hi = 0;
    self->mpTriangles = 0;
    self->mpVertexColour = 0;
    self->mpVertexPosition = 0;
    self->mpVertexPositionRender = 0;
    self->mpUV = 0;
    self->mpVertices = 0;
    return ((cUIVector_cUIVector_s*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIVector__cUIVector);
#ifdef SKIP_ASM
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern void* cUIVector_vtbl[];

/**
 * Destroys a UI vector: frees its vertex arrays, then runs the cUIThing destructor.
 *
 * Restores cUIVector_vtbl, frees mpVertexColour, mpVertexPosition, mpVertexPositionRender, mpUV and mpVertices
 * where set, then calls cUIThing__cUIThing(self, flags).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIVector__cUIVector(cUIVector* self, int flags)
{
    self->mVtbl = cUIVector_vtbl;
    if (self->mpVertexColour != 0) {
        cMemMan_free(self->mpVertexColour);
    }
    if (self->mpVertexPosition != 0) {
        cMemMan_free(self->mpVertexPosition);
    }
    if (self->mpVertexPositionRender != 0) {
        cMemMan_free(self->mpVertexPositionRender);
    }
    if (self->mpUV != 0) {
        cMemMan_free(self->mpUV);
    }
    if (self->mpVertices != 0) {
        cMemMan_free(self->mpVertices);
    }
    cUIThing__cUIThing(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIVector_setColour);
#ifdef SKIP_ASM

/**
 * Sets a UI vector shape's colour, also recolouring every vertex.
 *
 * Stores the colour in mColour; when vertex colours exist, each gets the colour's components times 255,
 * truncated to whole numbers.
 *
 * @param colour  the new colour (components 0 to 1)
 */
extern "C" void cUIVector_setColour(cUIVector* self, tARGB* colour)
{
    tARGB tmp = *colour;
    unsigned int i;
    *(&self->mColour) = *colour;
    if (self->mpVertexColour == 0) {
        return;
    }
    tmp.A = (int)(colour->A * 255.0f);
    tmp.R = (int)(colour->R * 255.0f);
    tmp.G = (int)(colour->G * 255.0f);
    tmp.B = (int)(colour->B * 255.0f);
    for (i = 0; i < *(unsigned int*)&self->mAttrVector >> 17; i++) {
        (self->mpVertexColour)[i] = tmp;
    }
}
#endif

