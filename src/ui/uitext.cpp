#include "common.h"
#include "bx/math3d.h"
#include "render/2drender.h"
#include "fe/ovstatehud.h"
#include "ui/uiengine.h"
#include "ui/uifontinterface.h"
#include "ui/uiscreen.h"
#include "ui/uistate.h"
#include "ui/uidata.h"
#include "render/font.h"
#include "ui/uiinput.h"
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
extern "C" float cFont_charAdvance(void* font, int ch);
// The unit declares these with the string as int; bind pointer-typed views.
extern "C" float cFont_textExtent1_s(void* font, unsigned short* str, void* out, int flags, float sx, float sy) __asm__("cFont_textExtent1");
extern "C" void cFont_text1_s(void* font, float x, float y, unsigned short* str) __asm__("cFont_text1");
extern "C" void cUIThing_applyPivot(void* self, void* pos, void* size);
extern "C" void usprintf(unsigned short* dst, unsigned short* fmt, unsigned short* str);
extern "C" int USTR_length(unsigned short* s);
void cMemMan_free(void*);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UITEXTTEMP_b[];
extern unsigned short D_004A47B0[];
extern unsigned short D_00508750[];

struct sV3_05E8 {
    float x, y, z;
    sV3_05E8() {}
    sV3_05E8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sRect_05E8 {
    float x0, y0, x1, y1;
};

// Default drop-shadow offset ($gp-relative, 8 bytes).
extern t2Vector D_004A5A60;


struct sLayerBits_05E8 {
    unsigned char lo;
    unsigned char layer : 5;
};



/**
 * Draws a text object's lines at its position plus an offset.
 *
 * Refreshes the string through updateLocale (slot 16) when there is none or the cached bit is clear. Picks
 * the static colour (static thing with use-static-colour set), the disabled colour (disabled thing with
 * use-disabled-colour set) or the normal colour, and adds a drop shadow when the shadow bit is set. Each line
 * is measured, placed by cUIThing_applyPivot and drawn 3 pixels below the last; with ellipses on and several lines,
 * only the first line is drawn, followed by "...". When a flag word reached through the screen's engine has
 * bit 27 set (probably a debug option), every line is replaced by a row of 'W's as wide as the object.
 *
 * @param x  horizontal offset added to the object's position
 * @param y  vertical offset added to the object's position
 */
void cUIText::render2D(float x, float y)
{
    if (mpString == 0 || ((mAttr >> 2) & 1) == 0) {
        updateLocale();
    }
    int* ov = (int*)mpScreen->mpState->mpEngine->mpLocaleInterface;
    if (ov != 0 && ((*ov >> 27) & 1)) {
        cFont* font = (cFont*)cUIText_getFont(this);
        if (font == 0) {
            return;
        }
        int n = (int)(mSize.x / (cFont_charAdvance(font, 'W') * mScale.x));
        int i = 0;
        if (n < 0x400) {
            for (i = 0; i < n; i++) {
                D_00508750[i] = 'W';
            }
        }
        D_00508750[i] = 0;
    }
    if (mpString == 0) {
        return;
    }
    cFont* font = (cFont*)cUIText_getFont(this);
    if (font == 0) {
        return;
    }
    t2Vector textScale(mScale.x, mScale.y);
    (*(t2Vector*)&font->Scale[0]).x = (*(t2Vector*)&font->BaseScale[0]).x * textScale.x;
    (*(t2Vector*)&font->Scale[0]).y = (*(t2Vector*)&font->BaseScale[0]).y * textScale.y;
    if (((mAttr >> 5) & 1) && ((mAttrText >> 4) & 1)) {
        (*(tARGB*)&font->Color[0]) = *(&mStaticColour);
    } else if (((mAttr >> 3) & 1) == 0 && ((mAttrText >> 5) & 1)) {
        (*(tARGB*)&font->Color[0]) = *(&mDisabledColour);
    } else {
        (*(tARGB*)&font->Color[0]) = *(&mColour);
    }
    if ((mAttrText >> 2) & 1) {
        tARGB shadowCol = *(&mShadowColour);
        shadowCol.A = (*(&mColour)).A;
        (*(tARGB*)&font->ShadowColor[0]) = shadowCol;
        (*(t2Vector*)&font->ShadowOffset[0]) = D_004A5A60;
    } else {
        (*(t2Vector*)&font->ShadowOffset[0]) = t2Vector(0.0f, 0.0f);
    }
    int* drawFlags = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *drawFlags = (*drawFlags & ~0x3E0) | (((((sLayerBits_05E8*)&mAttr)->layer + 9) << 5) & 0x3E0);
    sV3_05E8 pos(mPosition.x + x, mPosition.y + y, 0.0f);
    int i = 0;
    if (mNumLines > 0) do {
        unsigned short* str;
        if (i != 0) {
            str = mpLineOffset[i];
        } else {
            str = mpString;
        }
        if (ov != 0 && ((*ov >> 27) & 1)) {
            str = D_00508750;
        }
        if (((mAttrText >> 6) & 1) && mNumLines >= 2) {
            unsigned short* buf = (unsigned short*)operator_new_tag((USTR_length(str) + 5) * 2, str_UITEXTTEMP_b, 0x100, 0);
            usprintf(buf, D_004A47B0, str);
            sRect_05E8 textBounds;
            cFont_textExtent1_s(font, buf, &textBounds, 0, textScale.x, textScale.y);
            sV3_05E8 size(textBounds.x1, mScale.y * ((float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y),
                    0.0f);
            sV3_05E8 drawPos = pos;
            cUIThing_applyPivot(this, &drawPos, &size);
            pos.y += (float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y * textScale.y + 3.0f;
            cFont_text1_s(font, drawPos.x, drawPos.y, buf);
            if (buf != 0) {
                cMemMan_free(buf);
            }
            return;
        }
        sRect_05E8 textBounds;
        cFont_textExtent1_s(font, str, &textBounds, 0, textScale.x, textScale.y);
        i++;
        sV3_05E8 size(textBounds.x1, mScale.y * ((float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y),
                0.0f);
        sV3_05E8 drawPos = pos;
        cUIThing_applyPivot(this, &drawPos, &size);
        pos.y += (float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y * textScale.y + 3.0f;
        cFont_text1_s(font, drawPos.x, drawPos.y, str);
    } while (i < mNumLines);
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
void cUIText::deleteText()
{
    mNumLines = 0;
    if ((void*)mpLineOffset != 0) {
        cMemMan_free(mpLineOffset);
        mpLineOffset = (unsigned short**)0;
    }
    if ((void*)mpString != 0) {
        cMemMan_free(mpString);
        mpString = (unsigned short*)0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getNumTextLines);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern unsigned short gUSTRBackSlash;
/**
 * Counts the lines of a text object's string.
 *
 * A carriage return (13) or a doubled line-break character (gUSTRBackSlash twice) starts a new line.
 *
 * @return the number of lines, or 0 when there is no string
 */
char cUIText::getNumTextLines()
{
    unsigned short* text = mpString;
    if (text == 0) return 0;
    unsigned short* cursor = text;
    int n = 1;
    while (*cursor != 0) {
        unsigned short curChar = *cursor;
        unsigned short next = cursor[1];
        if (curChar == gUSTRBackSlash && next == curChar) {
            cursor += 2;
            n++;
        } else if (curChar == '\r') {
            cursor++;
            n++;
        } else {
            cursor++;
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
int cUIText::getAsciiString()
{
    if (((mAttrText >> 3) & 1) == 0) {
        return (int)mpString;
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
int cUIText::getUnicodeString()
{
    if (((mAttrText >> 3) & 1) != 0) {
        return (int)mpString;
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
void cUIText::setAsciiString(const char* str)
{
    mStringID = 0;
    setAsciiStringPrivate(str);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setAsciiStringPrivate);
#ifdef SKIP_ASM
extern "C" int strlen(const char* s);
void UIAsciiToUnicode(unsigned short* dst, const char* src);
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self, const unsigned short* str);

/**
 * Sets a text object's string from an ASCII string.
 *
 * Marks the text cached (setCached(1), slot 5) and not Unicode (setUnicode(0), slot 24), then converts the
 * string to UCS-2 and stores it with cUIText_setUnicodeStringPrivate(). A string of 0x200 characters or more
 * is ignored.
 *
 * @param str  the ASCII string
 */
void cUIText::setAsciiStringPrivate(const char* str)
{
    unsigned short buf[0x200];
    setCached(1);
    setUnicode(0);
    if (strlen(str) + 1 < 0x200) {
        UIAsciiToUnicode(buf, str);
        setUnicodeStringPrivate(buf);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_setUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_charAdvance1(void* font, unsigned short ch);
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern "C" void cUIText_setUnicodeStringPrivate(cUIText* self, const unsigned short* str);
void cMemMan_free(void*);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UITEXTTEMP_b[];
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
void cUIText::setUnicodeStringWrap(unsigned short* str)
{
    setCached(1);
    void* font = cUIText_getFont(this);
    if (font == 0) {
        return;
    }
    int last = -1;
    float w = 0.0f;
    int len = USTR_length(str);
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, str_UITEXTTEMP_b, 0x100, 0);
    int i = 0;
    USTR_copy(buf, str);
    while (i < len) {
        if (buf[i] == gUSTRBackSlash && buf[i + 1] == buf[i]) {
            last = -1;
            w = 0.0f;
            i += 2;
        } else {
            w += cFont_charAdvance1(font, buf[i]) * mScale.x;
            if (buf[i] == ' ') {
                last = i;
            }
            if (w > mSize.x && last >= 0) {
                buf[last] = 13;
                i = last;
                w = 0.0f;
                last = -1;
            }
            i++;
        }
    }
    setUnicodeStringPrivate(buf);
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
void cUIText::setUnicodeString(const unsigned short* str)
{
    mStringID = 0;
    setUnicodeStringPrivate(str);
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
extern char str_UITextUStr[];
extern char str_UITextLine[];
extern unsigned short gUSTRBackSlash;

/**
 * Stores a copy of a UCS-2 string in a text object and splits it into lines.
 *
 * Marks the text cached and Unicode (slots 5 and 24), deletes the old text, copies the string (ignored if
 * longer than 800 characters) and counts its lines. With two or more lines it builds the line-start table,
 * cutting the copy at each carriage return and doubled line-break marker.
 *
 * @param str    the string to store
 */
void cUIText::setUnicodeStringPrivate(const unsigned short* str)
{
    setCached(1);
    setUnicode(1);
    deleteText();
    int len = USTR_length((unsigned short*)str);
    if (len < 801) {
        mStringSize = len + 1;
        mpString = (unsigned short*)operator_new_tag((len + 1) * 2, str_UITextUStr, 0x100, 0);
        USTR_copy(mpString, (unsigned short*)str);
        int n = cUIText_getNumTextLines(((char*)this));
        mNumLines = n;
        if (n >= 2) {
            unsigned short** lines = (unsigned short**)operator_new_tag(n * 4, str_UITextLine, 0x100, 0);
            unsigned short* text = mpString;
            mpLineOffset = lines;
            mLineSize = *(unsigned short*)&mNumLines;
            lines[0] = text;
            int k = 1;
            unsigned short* cursor = text;
            while (*cursor != 0) {
                unsigned short curChar = *cursor;
                unsigned short next = cursor[1];
                if (curChar == gUSTRBackSlash && next == curChar) {
                    *cursor = 0;
                    cursor += 2;
                    lines[k++] = cursor;
                } else if (curChar == '\r') {
                    *cursor = 0;
                    cursor++;
                    lines[k++] = cursor;
                } else {
                    cursor++;
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
void cUIText::setUnicodeStringByID(int id)
{
    mStringID = id;
    setCached(0);  // virtual cUIThing::setCached(false)
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getTextExtent);
#ifdef SKIP_ASM
extern "C" void* cUIText_getFont(void* self);
extern "C" float cFont_textExtent1(void* font, int str, void* out, int flags, float sx, float sy);

struct cUIText_getTextExtent_sVec3 {
    float x;
    float y;
    float z;
};
extern cUIText_getTextExtent_sVec3 t3vect_Zero;

struct cUIText_getTextExtent_sRect {
    float x0;
    float y0;
    float x1;
    float y1;
};

/**
 * Returns the size of a UI text's string at its font and scale.
 *
 * Refreshes the text first (virtual slot 16) when it has no string (+0xB4) or bit 2 of +0x14 is clear; then measures
 * the string with cFont_textExtent1 using cUIText_getFont and mScale and returns the measured rectangle's far corner (+0x8,
 * +0xC) as x and y. Without a string or font it returns t3vect_Zero. cFEStateUberTrick_updateTrickMenu uses it to size
 * the trick menu.
 *
 * @param out  receives the size (x, y)
 */
extern "C" void cUIText_getTextExtent(cUIText* self, cUIText_getTextExtent_sVec3* out)
{
    if ((int)self->mpString == 0 || ((self->mAttr >> 2) & 1) == 0) {
        self->updateLocale();
    }
    void* font = cUIText_getFont(self);
    int str = (int)self->mpString;
    if (str != 0 && font != 0) {
        cUIText_getTextExtent_sRect textRect;
        cFont_textExtent1(font, str, &textRect, 0, self->mScale.x, self->mScale.y);
        out->x = textRect.x1;
        out->y = textRect.y1;
    } else {
        *out = t3vect_Zero;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIText_getNumLinesOfString);
#ifdef SKIP_ASM
/**
 * Returns a UI text's line count (+0xA8), refreshing the text first (virtual slot 16) when needed.
 *
 * The refresh condition is the one cUIText_getTextExtent uses. Nothing in the retail code references it.
 *
 * @return the number of lines
 */
int cUIText::getNumLinesOfString()
{
    if ((int)mpString == 0 || ((mAttr >> 2) & 1) == 0) {
        updateLocale();
    }
    return mNumLines;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_cUITextScroll);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a1, int a2);
extern "C" void* cList_cList(void* self);
extern "C" void cUITextScroll_setInputMap(cUITextScroll* self, char a1);
extern void* cUITextScroll_vtbl[];

/**
 * Constructs an empty text scroll.
 *
 * Runs the cUIThing constructor (cUIThing_cUIThing) with the screen and parent, stores cUITextScroll_vtbl, clears mMode,
 * mShadow, mUnicode and mStaticColour, constructs mList and clears mpScrollBar, mpLineOffset, mStringID,
 * mSelectedIndex, mLinesOffset, mLineSize and mPixelOffset; then cUITextScroll_setInputMap(self, 0).
 *
 * @param pState  the screen (On Tour cUIScreen* pState)
 * @param pParent  the parent object (On Tour cUIThing* pParent)
 * @return @p self
 */
extern "C" void* cUITextScroll_cUITextScroll(cUITextScroll* self, int pState, int pParent)
{
    cUIThing_cUIThing(self, pState, pParent);
    VPTR_OF(cListNode, self) = cUITextScroll_vtbl;
    *(unsigned char*)&self->mAttrTextScroll &= 0xC6;
    cList_cList(&self->mList);
    self->mpScrollBar = 0;
    self->mpLineOffset = 0;
    self->mStringID = (unsigned int)0;
    self->mSelectedIndex = 0;
    self->mLinesOffset = 0;
    self->mLineSize = 0;
    self->mPixelOffset = 0.0f;
    self->setInputMap(0);
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

// A tARGB with EA's constructors (ours has none): mShadowColour is a tARGB ([O]).
struct tARGBInit : tARGB {
    tARGBInit() {}
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};

/**
 * Sets a text scroll up from its screen record.
 *
 * Runs cUIThing's setUIData, takes mFont from the record's +0x20 byte, calls slot 23 (cUITextScroll_setShadow) with bit 0 of
 * its +0x21 byte, and loads the record's text id (+0x24) when nonzero (clear, cUITextScroll_clearBuffer; add, cUITextScroll_addUnicodeStringByID).
 * When mShadow is then set, takes mShadowColour from its RGBA bytes (+0x28, scaled by 1/255).
 *
 * @param pObj  the screen record (On Tour tUIBaseObject* pUIBase)
 */
void cUITextScroll::setUIData(tUIBaseObject* pObj)
{
    tUITextScroll* data = (tUITextScroll*)pObj;
    cUIThing_setUIData_2(this, data);
    mAttrTextScroll.mFont = data->mFontID & 3;
    setShadow(data->mTextFlags & 1);
    if (((int)data->mStringID) != 0) {
        cUITextScroll_clearBuffer_1(this);
        addUnicodeStringByID(((int)data->mStringID));
    }
    int bits = *(int*)&mAttrTextScroll >> 3;
    if (bits & 1) {
        mShadowColour = tARGBInit(data->mSColour.a * 0.003921568859368563f,
                                        data->mSColour.r * 0.003921568859368563f,
                                        data->mSColour.g * 0.003921568859368563f,
                                        data->mSColour.b * 0.003921568859368563f);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_getFont);
#ifdef SKIP_ASM
/**
 * Returns the text box's font: the mpFont of entry mFont (bits 1-2 of mAttrTextScroll) in the UI engine's font table
 * (mpFontInterface->mFontEntry), the engine being mpScreen->mpState->mpEngine.
 *
 * @return the cFont (as an int), or 0 without an engine
 */
int cUITextScroll::getFont()
{
    cUIEngine* engine = mpScreen->mpState->mpEngine;
    if (((void*)engine) != 0) {
        return (int)(&engine->mpFontInterface->mFontEntry[0])[(*(unsigned int*)&mAttrTextScroll >> 1) & 3].mpFont;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setInputMap);
#ifdef SKIP_ASM
/**
 * Fills mInputMap with input ids 0, 1, 2, 3 when @p value is 0, else 4, 5, 6, 7.
 *
 * @param value  which input set to use
 */
void cUITextScroll::setInputMap(char value)
{
    if (value == 0) {
        mInputMap[0] = 0;
        mInputMap[1] = 1;
        mInputMap[2] = 2;
        mInputMap[3] = 3;
    } else {
        mInputMap[0] = 4;
        mInputMap[1] = 5;
        mInputMap[2] = 6;
        mInputMap[3] = 7;
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
    virtual void* getStringByHash(int);
};

class cUITextScroll_updateLocale_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void setCached(int);
};

/**
 * Reloads the text scroll's string (mStringID) in the engine's current language and sets it cached (slot 5).
 *
 * Does nothing without a string id or a locale; the text is replaced (cUITextScroll_setUnicodeStringWrap) only when the locale has it.
 */
void cUITextScroll::updateLocale()
{
    int id = (int)mStringID;
    if (id != 0) {
        cUITextScroll_updateLocale_cVirtA* strings = (cUITextScroll_updateLocale_cVirtA*)mpScreen->mpState->mpEngine->mpLocaleInterface;
        if (strings != 0) {
            void* wideStr = strings->getStringByHash(id);
            if (wideStr != 0) {
                setUnicodeStringWrap(wideStr);
            }
            ((cUIThing*)(this))->setCached(1);
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



static inline int pressed13E0(char* input, int key)
{
    sUIInputVE13E0* e = &(*(sUIInputVE13E0**)(input + 8))[2];
    return e->fn(vthis(input, *e), key);
}

/**
 * Scrolls the text, or moves the selection in a selectable list (mMode), on the keys in mInputMap.
 *
 * Rows that fit = mSize.y / (line height + 3). Selection mode: up / down (mInputMap[0] / [1], getState) move
 * mSelectedIndex within the lines and return true; with neither key, mLinesOffset is moved to keep the selection in
 * view (false is returned). Plain mode: up / down move mLinesOffset within 0 .. lines - rows and return true. Nothing
 * happens without a font.
 *
 * @param input  the input (On Tour const cUIInput* pInput)
 * @return true when a key was used, else false (On Tour returns bool)
 */
bool cUITextScroll::onInput(cUIInput* input)
{
    cFont* font = (cFont*)this->getFont();
    if (font == 0) {
        return false;
    }
    signed char rows = (int)(mSize.y / (mScale.y * ((float)font->MaxCharHeight * font->BaseScale[1]) + 3.0f));
    if ((*(unsigned int*)&mAttrTextScroll) & 1) {
        if (pressed13E0(((char*)input), (mInputMap[0])) != 0) {
            if (mSelectedIndex != 0) {
                mSelectedIndex--;
            }
            return true;
        }
        if (pressed13E0(((char*)input), (mInputMap[1])) != 0) {
            if (mSelectedIndex < mLineSize - 1) {
                mSelectedIndex++;
            }
            return true;
        }
        short rowOffset = mSelectedIndex - mLinesOffset;
        if (rowOffset < 0) {
            mLinesOffset = mSelectedIndex;
        } else if (rowOffset >= rows) {
            mLinesOffset = mSelectedIndex - rows + 1;
        }
    } else {
        if (pressed13E0(((char*)input), (mInputMap[0])) != 0) {
            if (mLinesOffset != 0) {
                mLinesOffset--;
            }
            return true;
        }
        if (pressed13E0(((char*)input), (mInputMap[1])) == 0) {
            return false;
        }
        if (mLinesOffset < mLineSize - rows) {
            mLinesOffset++;
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_render2D);
#ifdef SKIP_ASM
extern "C" int cUITextScroll_getFont(cUITextScroll* self);
extern "C" float cFont_textExtent1(void* font, int str, void* out, int flags, float sx, float sy);
extern "C" void cUIThing_applyPivot(void* self, void* pos, void* size);
extern "C" void cFont_text1(void* font, float x, float y, int str);
extern "C" void cUIScrollBar_setValues(void* sb, int top, int rows, int count);

struct sV3_1588 {
    float x, y, z;
    sV3_1588() {}
    sV3_1588(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sRect_1588 {
    float x0, y0, x1, y1;
};


struct sLayerBits_1588 {
    unsigned char lo;
    unsigned char layer : 5;
};
struct sDrawBits_1588 {
    unsigned int lo : 5;
    unsigned int layer : 5;
};


/**
 * Draws the visible lines of the text scroll from mLinesOffset, then updates its scroll bar.
 *
 * Calls slot 16 (cUITextScroll_updateLocale) first unless mCached is set; needs a font. Uses mScale, mShadow (mShadowColour with
 * its first component from mColour, at (2, 2)) and draw layer mLayer + 9. Each line that fits in mSize.y is drawn at
 * the position plus (@p x, @p y), mPixelOffset lower, in mColour (mSelectedColour for the selected line in mMode);
 * then mpScrollBar gets the offset, the rows and the line count (cUIScrollBar::setValues).
 *
 * @param x  the parent's x offset
 * @param y  the parent's y offset
 */
void cUITextScroll::render2D(float x, float y)
{
    if (((mAttr >> 2) & 1) == 0) {
        updateLocale();
    }
    cFont* font = (cFont*)this->getFont();
    if (font == 0) {
        return;
    }
    t2Vector textScale(mScale.x, mScale.y);
    (*(t2Vector*)&font->Scale[0]).x = (*(t2Vector*)&font->BaseScale[0]).x * textScale.x;
    (*(t2Vector*)&font->Scale[0]).y = (*(t2Vector*)&font->BaseScale[0]).y * textScale.y;
    if (((*(int*)&mAttrTextScroll) >> 3) & 1) {
        tARGB shadowCol = mShadowColour;
        shadowCol.A = mColour.A;
        (*(tARGB*)&font->ShadowColor[0]) = shadowCol;
        (*(t2Vector*)&font->ShadowOffset[0]) = t2Vector(2.0f, 2.0f);
    } else {
        (*(t2Vector*)&font->ShadowOffset[0]) = t2Vector(0.0f, 0.0f);
    }
    int* drawFlags = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *drawFlags = (*drawFlags & ~0x3E0) | (((((sLayerBits_1588*)&mAttr)->layer + 9) << 5) & 0x3E0);
    sV3_1588 pos(mPosition.x + x, mPosition.y + y, 0.0f);
    float lineH = mScale.y * ((float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y) + 3.0f;
    signed char rows = (int)(mSize.y / lineH);
    int i = 0;
    if (rows > 0) do {
        unsigned short idx = mLinesOffset + i;
        if (idx >= mLineSize) {
            break;
        }
        if (((*(int*)&mAttrTextScroll) & 1) && idx == mSelectedIndex) {
            (*(tARGB*)&font->Color[0]) = mSelectedColour;
        } else {
            (*(tARGB*)&font->Color[0]) = mColour;
        }
        int str = ((int*)mpLineOffset)[i + mLinesOffset];
        i++;
        sRect_1588 textBounds;
        cFont_textExtent1(font, str, &textBounds, 0, mScale.x, mScale.y);
        sV3_1588 size(textBounds.x1, mScale.y * ((float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y),
                0.0f);
        sV3_1588 drawPos = pos;
        cUIThing_applyPivot(this, &drawPos, &size);
        pos.y += lineH;
        cFont_text1(font, drawPos.x, drawPos.y + mPixelOffset, str);
    } while (i < rows);
    if (((void*)mpScrollBar) != 0) {
        cUIScrollBar_setValues(((void*)mpScrollBar), mLinesOffset, rows, mLineSize);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_clearBuffer);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees every text line (mpLineOffset[0 .. mLineSize - 1]) and the line array, then zeroes mpLineOffset, mLinesOffset
 * and mLineSize.
 */
void cUITextScroll::clearBuffer()
{
    if (mpLineOffset != 0) {
        unsigned short i;
        for (i = 0; i < mLineSize; i++) {
            if (mpLineOffset[i] != 0) {
                cMemMan_free(mpLineOffset[i]);
            }
        }
        if (mpLineOffset != 0) {
            cMemMan_free(mpLineOffset);
        }
        mpLineOffset = 0;
    }
    mLinesOffset = 0;
    mLineSize = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_allocLine);
#ifdef SKIP_ASM
extern "C" void* func_0041605C(void* dst, const void* src, int n);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UITEXTSCBUF[];

/**
 * Adds one slot to a scrolling text's line table and returns its index.
 *
 * Grows the line pointer array (+0xBC) by one entry, copies the old pointers and frees the old array. The new
 * slot is cleared only when an old array existed.
 *
 * @return index of the new line slot (the old line count)
 */
int cUITextScroll::allocLine()
{
    unsigned short old = mLineSize;
    mLineSize = old + 1;
    void** arr = (void**)operator_new_tag(mLineSize * 4, str_UITEXTSCBUF, 0x100, 0);
    if ((void**)mpLineOffset != 0) {
        func_0041605C(arr, (void**)mpLineOffset, old * 4);
        void** prev = (void**)mpLineOffset;
        arr[old] = 0;
        if (prev != 0) {
            cMemMan_free(prev);
        }
    }
    mpLineOffset = (unsigned short**)arr;
    return old;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_addAsciiString);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_addUnicodeString(cUITextScroll* self, unsigned short* str);
extern "C" void strcpy3(void* dst, void* src);

/**
 * Appends ASCII text, word-wrapped to the box width (On Tour addAsciiString).
 *
 * Calls virtual slot 5 (cUIThing_setCached) with 1 and needs a font. Widens the text into a temporary UTF-16 buffer
 * (tag str_UITEXTTEMP_b "UITEXTTEMP") and adds lines with addUnicodeString: a doubled backslash or a carriage return ends a
 * line; otherwise character advances (cFont::charAdvance times mScale.x) add up and the line breaks at the last space
 * once the width passes mSize.x. Without a space it breaks before the overflowing character and the character before
 * it is overwritten with the terminator, so that character is lost.
 *
 * @param str  the ASCII text (On Tour pString)
 */
void cUITextScroll::addAsciiString(char* str)
{
    setCached(1);
    void* font = (void*)this->getFont();
    if (font == 0) {
        return;
    }
    int last = -1;
    int len = strlen(str);
    int i = 0;
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, str_UITEXTTEMP_b, 0x100, 0);
    float w = 0.0f;
    int start = 0;
    strcpy3(buf, str);
    while (i < len) {
        if (buf[i] == '\\' && buf[i + 1] == buf[i]) {
            buf[i] = 0;
            i += 2;
            addUnicodeString(&buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else if (buf[i] == '\r') {
            buf[i] = 0;
            i += 1;
            addUnicodeString(&buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else {
            w += cFont_charAdvance1(font, buf[i]) * mScale.x;
            if (buf[i] == ' ') {
                last = i;
            }
            if (w > mSize.x) {
                if (last < 0) {
                    last = i - 1;
                }
                if (last > 0) {
                    buf[last] = 0;
                    i = last;
                    w = 0.0f;
                    addUnicodeString(&buf[start]);
                    last = -1;
                    start = i + 1;
                }
            }
            i++;
        }
    }
    if (start != i) {
        addUnicodeString(&buf[start]);
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
extern char str_UITXTSCRSTR[];

/**
 * Appends a copy of a UCS-2 string to a scrolling text as a new line.
 *
 * @param str  the string to add
 */
void cUITextScroll::addUnicodeString(unsigned short* str)
{
    int idx = allocLine();
    unsigned short* copy = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UITXTSCRSTR, 0x100, 0);
    USTR_copy(copy, str);
    mpLineOffset[idx] = copy;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setAsciiString);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
typedef void (*fnUT1C80)(char*, int);


/**
 * Replaces the text with ASCII text: clears the lines (cUITextScroll_clearBuffer_1), clears mStringID, calls virtual
 * slot 5 (cUIThing_setCached) with 1 and slot 24 (cUITextScroll_setUnicode) with 0, then addAsciiString.
 *
 * @param str  the ASCII text (On Tour pAscii)
 */
void cUITextScroll::setAsciiString(char* str)
{
    cUITextScroll_clearBuffer_1(this);
    mStringID = (unsigned int)0;
    setCached(1);
    setUnicode(0);
    addAsciiString(str);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_setUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_clearBuffer(cUITextScroll*, void*);
extern "C" void cUITextScroll_addUnicodeStringWrap(cUITextScroll*, void*);

/**
 * Replaces the text with word-wrapped UTF-16 text: clearBuffer, then addUnicodeStringWrap. mStringID is left as it
 * was.
 *
 * @param pString  the UTF-16 text
 */
void cUITextScroll::setUnicodeStringWrap(void* pString)
{
    clearBuffer();
    addUnicodeStringWrap(pString);
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_addUnicodeStringWrap);
#ifdef SKIP_ASM
extern "C" void cUITextScroll_addUnicodeString(cUITextScroll* self, unsigned short* str);

/**
 * Appends UTF-16 text, word-wrapped to the box width (On Tour addUnicodeStringWrap).
 *
 * Same as addAsciiString on a heap copy of the text (tag str_UITEXTTEMP_b "UITEXTTEMP"), except that only a doubled
 * backslash forces a line break (a carriage return does not), and with the same lost character on a forced break
 * without a space.
 *
 * @param vstr  the UTF-16 text (On Tour pString)
 */
void cUITextScroll::addUnicodeStringWrap(void* vstr)
{
    unsigned short* str = (unsigned short*)vstr;
    setCached(1);
    void* font = (void*)this->getFont();
    if (font == 0) {
        return;
    }
    int last = -1;
    int len = USTR_length(str);
    int i = 0;
    unsigned short* buf = (unsigned short*)operator_new_tag((len + 1) * 2, str_UITEXTTEMP_b, 0x100, 0);
    float w = 0.0f;
    int start = 0;
    USTR_copy(buf, str);
    while (i < len) {
        if (buf[i] == '\\' && buf[i + 1] == buf[i]) {
            buf[i] = 0;
            i += 2;
            addUnicodeString(&buf[start]);
            last = -1;
            w = 0.0f;
            start = i;
        } else {
            w += cFont_charAdvance1(font, buf[i]) * mScale.x;
            if (buf[i] == ' ') {
                last = i;
            }
            if (w > mSize.x) {
                if (last < 0) {
                    last = i - 1;
                }
                if (last > 0) {
                    buf[last] = 0;
                    i = last;
                    w = 0.0f;
                    addUnicodeString(&buf[start]);
                    last = -1;
                    start = i + 1;
                }
            }
            i++;
        }
    }
    if (start != i) {
        addUnicodeString(&buf[start]);
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
    virtual void* getStringByHash(int);
};

class cUITextScroll_addUnicodeStringByID_cVirtB {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void setCached(int);
};

/**
 * Appends the localised string @p id: stores mStringID, looks the string up through the UI engine's string table
 * (engine + 0x10, its virtual slot 4), adds it with addUnicodeStringWrap when found and calls virtual slot 5
 * (cUIThing_setCached) with 1. Does nothing without a string table.
 *
 * @param id  the string's hash id (On Tour hashname)
 */
void cUITextScroll::addUnicodeStringByID(int id)
{
    mStringID = (unsigned int)id;
    cUITextScroll_addUnicodeStringByID_cVirtA* localeInterface = (cUITextScroll_addUnicodeStringByID_cVirtA*)mpScreen->mpState->mpEngine->mpLocaleInterface;
    if (localeInterface != 0) {
        void* unicodeString = localeInterface->getStringByHash(id);
        if (unicodeString != 0) {
            addUnicodeStringWrap(unicodeString);
        }
        ((cUIThing*)(this))->setCached(1);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_deleteLineByIndex);
#ifdef SKIP_ASM
/**
 * Removes line @p idx: allocates a line array one shorter (tag str_UITEXTSCBUF "UITEXTSCBUF"), copies the lines before and
 * after it, frees the old array and decrements mLineSize.
 *
 * The removed line's own string is not freed, and a negative @p idx is not rejected.
 *
 * @param idx  the line index (On Tour index)
 */
void cUITextScroll::deleteLineByIndex(int idx)
{
    if ((void**)mpLineOffset != 0) {
        unsigned short n = mLineSize;
        if (idx < n) {
            void** arr = (void**)operator_new_tag((n - 1) * 4, str_UITEXTSCBUF, 0x100, 0);
            if (idx > 0) {
                func_0041605C(arr, (void**)mpLineOffset, idx * 4);
            }
            if (idx < mLineSize - 1) {
                // PORT: pointer held in int (only spelling found that gives idx-first addu)
                func_0041605C((void*)(idx * 4 + (int)arr), (void*)(idx * 4 + (int)mpLineOffset + 4),
                        (mLineSize - idx - 1) * 4);
            }
            if ((void**)mpLineOffset != 0) {
                cMemMan_free((void**)mpLineOffset);
            }
            mpLineOffset = (unsigned short**)arr;
            (mLineSize)--;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUITextScroll_getStringByIndex);
#ifdef SKIP_ASM
/**
 * Returns entry @p i of a text scroller's line table (+0xBC), or 0 when there is no table or @p i is out of range.
 *
 * The count is the unsigned short at +0xC8. cFEStateCredits_onRender2D uses it.
 *
 * @param i  the line index
 * @return the line, or 0
 */
int cUITextScroll::getStringByIndex(int i)
{
    int* arr = (int*)mpLineOffset;
    if (arr != 0 && i >= 0 && i < mLineSize) {
        return arr[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cTextureElement_render);
#ifdef SKIP_ASM
struct sGfx_20A0 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// The unit declares GrMan as char*; typed view by asm label.
extern int G2D_20A0 __asm__("G2D");
extern "C" void c2DRender_drawRect(int ctx, tBxVertex* v, tRect* r);
extern "C" void c2DRender_drawRotRect(int ctx, tBxVertex* v, tRect* r);

/**
 * Draws a texture element as a screen quad in a rectangle with a colour.
 *
 * Does nothing for a negative texture (+0x0). Sets the current draw state's texture, builds 4 vertices with the
 * element's UVs (+0xC..+0x18; flag 1 swaps u, flag 2 swaps v) and the colour scaled by 128, and submits them with
 * c2DRender_drawRotRect when flag 4 is set, else c2DRender_drawRect. Nothing in the retail code references it.
 *
 * @param rect  the screen rectangle (x, y, w, h)
 * @param colour  the colour (a, r, g, b), 0..1
 * @param flags  1 flip u, 2 flip v, 4 use c2DRender_drawRotRect
 */
void cTextureElement::render(tRect rect, tARGB colour, int flags)
{
    int tex = mSourceShape;
    if (tex < 0) {
        return;
    }
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = tex;
    float topV = mTop;
    float bottomV = mBottom;
    float leftU = mLeft;
    float rightU = mRight;
    if (flags & 1) {
        leftU = rightU;
        rightU = mLeft;
    }
    if (flags & 2) {
        topV = bottomV;
        bottomV = mTop;
    }
    tBxVertex quadVerts[4];
    for (int i = 0; i < 4; i++) {
        tBxVertex* vertPtr = &quadVerts[i];
        vertPtr->a = (int)(colour.A * 128.0f);
        vertPtr->r = (int)(colour.R * 128.0f);
        vertPtr->g = (int)(colour.G * 128.0f);
        vertPtr->b = (int)(colour.B * 128.0f);
        vertPtr->one = 1.0f;
    }
    quadVerts[0].u = leftU;
    quadVerts[0].v = topV;
    quadVerts[1].u = rightU;
    quadVerts[1].v = topV;
    quadVerts[2].u = leftU;
    quadVerts[2].v = bottomV;
    quadVerts[3].u = rightU;
    quadVerts[3].v = bottomV;
    if (flags & 4) {
        c2DRender_drawRotRect(G2D_20A0, quadVerts, &rect);
    } else {
        c2DRender_drawRect(G2D_20A0, quadVerts, &rect);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", cTextureElement_render2);
#ifdef SKIP_ASM
struct sGfx_2220 {
    char pad[0xE84];
    sDrawState* top;          // 0xE84
};
// The unit declares GrMan as char*; typed views by asm label.
extern int G2D_2220 __asm__("G2D");
extern void c2DRender_drawRect_2220(int ctx, tBxVertex* v, tRect* r) __asm__("c2DRender_drawRect");
extern void c2DRender_drawRotRect_2220(int ctx, tBxVertex* v, tRect* r) __asm__("c2DRender_drawRotRect");

/**
 * Draws the bottom @p fillFraction part of a texture element as a screen quad, like a fill gauge.
 *
 * The rectangle and the top v are moved down by (1 - @p fillFraction) of their height, so only the bottom part shows. Unlike
 * cTextureElement_render it draws even a negative texture, ignores the flip flags and scales r, g, b by 255 (alpha by 128). Flag
 * 4 selects c2DRender_drawRotRect over c2DRender_drawRect. Nothing in the retail code references it.
 *
 * @param rect  the screen rectangle (x, y, w, h)
 * @param colour  the colour (a, r, g, b), 0..1
 * @param flags  4 uses c2DRender_drawRotRect
 * @param fillFraction  the shown fraction, 0..1
 */
void cTextureElement::render2(tRect rect, tARGB colour, int flags, float fillFraction)
{
    float k = 1.0f - fillFraction;
    int tex = mSourceShape;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = tex;
    float topV = mTop;
    float bottomV = mBottom;
    float leftU = mLeft;
    float rightU = mRight;
    rect.Y += k * rect.H;
    rect.H *= 1.0f - k;
    topV += k * (bottomV - topV);
    tBxVertex quadVerts[4];
    for (int i = 0; i < 4; i++) {
        tBxVertex* vertPtr = &quadVerts[i];
        vertPtr->a = (int)(colour.A * 128.0f);
        vertPtr->r = (int)(colour.R * 255.0f);
        vertPtr->g = (int)(colour.G * 255.0f);
        vertPtr->b = (int)(colour.B * 255.0f);
        vertPtr->one = 1.0f;
    }
    quadVerts[0].u = leftU;
    quadVerts[0].v = topV;
    quadVerts[1].u = rightU;
    quadVerts[1].v = topV;
    quadVerts[2].u = leftU;
    quadVerts[2].v = bottomV;
    quadVerts[3].u = rightU;
    quadVerts[3].v = bottomV;
    if (flags & 4) {
        c2DRender_drawRotRect_2220(G2D_2220, quadVerts, &rect);
    } else {
        c2DRender_drawRect_2220(G2D_2220, quadVerts, &rect);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", cTextureElement_renderTiled);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// A tRect with EA's constructors (ours has none).
struct tRectInit : tRect {
    tRectInit() {}
    tRectInit(float x, float y, float w, float h) { X = x; Y = y; W = w; H = h; }
};
extern int G2D_23B8 __asm__("G2D");
extern "C" void c2DRender_drawRect2(int ctx, tBxVertex* v, tRect* dst, tRect* uv);
extern "C" void c2DRender_drawRotRect2(int ctx, tBxVertex* v, tRect* dst, tRect* uv);

/**
 * Draws the texture element @p element tiled @p tx times across and @p ty times down the rectangle @p destRect, cropping the last
 * partial row and column.
 *
 * Selects the element's texture (tex) as texture 0 of GrMan's current draw state, then returns at once if @p tx or
 * @p ty is not positive. Each tile is r.w / tx by r.h / ty. For floor(ty) rows it draws floor(tx) full tiles plus, when
 * @p tx has a fraction, a last tile cut to the rectangle's right edge with its texture range cut to the same fraction;
 * when @p ty has a fraction a last row cut to the bottom edge follows. Every quad goes to the 2D renderer G2D
 * through c2DRender_drawRect2, or c2DRender_drawRotRect2 (texture turned 90 degrees, so the fractions crop the other axis) when flag
 * bit 2 is set. Flag bit 0 swaps the left and right texture coordinates (u0/u1), bit 1 the top and bottom ones (v0/v1).
 *
 * The four vertices get alpha = c.a * 128 and red/green/blue = c * 255; cTextureElement_render scales all four by 128, so the
 * colour here is twice as strong. No caller in retail references it.
 *
 * @param destRect  the screen rectangle to fill (x, y, w, h)
 * @param colour  the colour (a, r, g, b in 0..1)
 * @param flags  bit 0 mirror horizontally, bit 1 mirror vertically, bit 2 draw the texture turned 90 degrees
 * @param tx  how many times the texture repeats across (may be fractional)
 * @param ty  how many times the texture repeats down (may be fractional)
 */
void cTextureElement::renderTiled(tRect destRect, tARGB colour, int flags, float tx, float ty)
{
    int tex = mSourceShape;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = tex;
    float x0 = destRect.X;
    float y0 = destRect.Y;
    float topV = mTop;
    float bottomV = mBottom;
    float leftU = mLeft;
    float rightU = mRight;
    if (tx <= 0.0f) {
        return;
    }
    if (ty <= 0.0f) {
        return;
    }
    float w = destRect.W / tx;
    float tileHeight = destRect.H / ty;
    if (flags & 1) {
        leftU = rightU;
        rightU = mLeft;
    }
    if (flags & 2) {
        topV = bottomV;
        bottomV = mTop;
    }
    int nx = (int)tx;
    float y = y0;
    int ny = (int)ty;
    tBxVertex quadVerts[4];
    float fny = (float)ny;
    for (int i = 0; i < 4; i++) {
        tBxVertex* vert = &quadVerts[i];
        vert->a = (int)(colour.A * 128.0f);
        vert->r = (int)(colour.R * 255.0f);
        vert->g = (int)(colour.G * 255.0f);
        vert->b = (int)(colour.B * 255.0f);
        vert->one = 1.0f;
    }
    int j;
    float x;
    if (ny != 0) {
        for (y = y0, j = 0; j < ny; j++) {
            x = x0;
            if (flags & 4) {
                for (int i = 0; i < nx; i++) {
                    tRectInit dst(x, y, w, tileHeight);
                    tRectInit uv(leftU, topV, rightU - leftU, bottomV - topV);
                    c2DRender_drawRotRect2(G2D_23B8, quadVerts, &dst, &uv);
                    x += w;
                }
                if ((float)nx != tx) {
                    tRectInit dst(x, y, (x0 + destRect.W) - x, tileHeight);
                    tRectInit uv(leftU, topV, rightU - leftU, (topV + (bottomV - topV) * (tx - (float)nx)) - topV);
                    c2DRender_drawRotRect2(G2D_23B8, quadVerts, &dst, &uv);
                }
            } else {
                for (int i = 0; i < nx; i++) {
                    tRectInit dst(x, y, w, tileHeight);
                    tRectInit uv(leftU, topV, rightU - leftU, bottomV - topV);
                    c2DRender_drawRect2(G2D_23B8, quadVerts, &dst, &uv);
                    x += w;
                }
                if ((float)nx != tx) {
                    tRectInit dst(x, y, (x0 + destRect.W) - x, tileHeight);
                    tRectInit uv(leftU, topV, (leftU + (rightU - leftU) * (tx - (float)nx)) - leftU, bottomV - topV);
                    c2DRender_drawRect2(G2D_23B8, quadVerts, &dst, &uv);
                }
            }
            y += tileHeight;
        }
    }
    if (fny != ty) {
        x = x0;
        if (flags & 4) {
            float u0b = leftU + (rightU - leftU) * (1.0f - (ty - fny));
            float hh = (y0 + destRect.H) - y;
            for (int i = 0; i < nx; i++) {
                tRectInit dst(x, y, w, hh);
                tRectInit uv(u0b, topV, rightU - u0b, bottomV - topV);
                c2DRender_drawRotRect2(G2D_23B8, quadVerts, &dst, &uv);
                x += w;
            }
            if ((float)nx != tx) {
                tRectInit dst(x, y, (x0 + destRect.W) - x, hh);
                tRectInit uv(u0b, topV, rightU - u0b, (topV + (bottomV - topV) * (tx - (float)nx)) - topV);
                c2DRender_drawRotRect2(G2D_23B8, quadVerts, &dst, &uv);
            }
        } else {
            float v1b = topV + (bottomV - topV) * (ty - fny);
            float hh = (y0 + destRect.H) - y;
            for (int i = 0; i < nx; i++) {
                tRectInit dst(x, y, w, hh);
                tRectInit uv(leftU, topV, rightU - leftU, v1b - topV);
                c2DRender_drawRect2(G2D_23B8, quadVerts, &dst, &uv);
                x += w;
            }
            if ((float)nx != tx) {
                tRectInit dst(x, y, (x0 + destRect.W) - x, hh);
                tRectInit uv(leftU, topV, (leftU + (rightU - leftU) * (tx - (float)nx)) - leftU, v1b - topV);
                c2DRender_drawRect2(G2D_23B8, quadVerts, &dst, &uv);
            }
        }
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", cTextureElement_renderRot);
#ifdef SKIP_ASM
extern int G2D_2A00 __asm__("G2D");
extern "C" void c2DRender_drawRect1(int ctx, tBxVertex* v, tRect* r, t3Matrix* m);
extern "C" void c2DRender_drawRotRect1(int ctx, tBxVertex* v, tRect* r, t3Matrix* m);

/**
 * Draws the texture element @p element once over the rectangle @p destRect, with the corners transformed by the 3x3 matrix @p transform.
 *
 * Selects the element's texture (tex) as texture 0 of GrMan's current draw state, sets the four vertices' texture
 * coordinates to the element's rectangle (u0/u1 swapped when flag bit 0 is set, v0/v1 when bit 1 is set) and their
 * colour to alpha = c.a * 128, red/green/blue = c * 255 (cTextureElement_render scales all four by 128), then hands the quad to
 * the 2D renderer G2D through c2DRender_drawRect1, or c2DRender_drawRotRect1 (corners in the order that turns the texture 90
 * degrees) when bit 2 is set; both transform each corner by @p transform (transformPoint2D) before submitting. No caller in retail
 * references it.
 *
 * @param destRect  the screen rectangle (x, y, w, h) before the transform
 * @param transform  the 3x3 transform applied to each corner (rows of three floats)
 * @param colour  the colour (a, r, g, b in 0..1)
 * @param flags  bit 0 mirror horizontally, bit 1 mirror vertically, bit 2 draw the texture turned 90 degrees
 */
void cTextureElement::renderRot(tRect destRect, t3Matrix transform, tARGB colour, int flags)
{
    int tex = mSourceShape;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = tex;
    float topV = mTop;
    float bottomV = mBottom;
    float leftU = mLeft;
    float rightU = mRight;
    if (flags & 1) {
        leftU = rightU;
        rightU = mLeft;
    }
    if (flags & 2) {
        topV = bottomV;
        bottomV = mTop;
    }
    tBxVertex quadVerts[4];
    for (int i = 0; i < 4; i++) {
        tBxVertex* vert = &quadVerts[i];
        vert->a = (int)(colour.A * 128.0f);
        vert->r = (int)(colour.R * 255.0f);
        vert->g = (int)(colour.G * 255.0f);
        vert->b = (int)(colour.B * 255.0f);
        vert->one = 1.0f;
    }
    quadVerts[0].u = leftU;
    quadVerts[0].v = topV;
    quadVerts[1].u = rightU;
    quadVerts[1].v = topV;
    quadVerts[2].u = leftU;
    quadVerts[2].v = bottomV;
    quadVerts[3].u = rightU;
    quadVerts[3].v = bottomV;
    if (flags & 4) {
        c2DRender_drawRotRect1(G2D_2A00, quadVerts, &destRect, &transform);
    } else {
        c2DRender_drawRect1(G2D_2A00, quadVerts, &destRect, &transform);
    }
}
#endif
//100%
INCLUDE_ASM("ui/uitext", cTextureElement_renderRotTiled);
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
extern int G2D_2BC8 __asm__("G2D");
extern "C" void c2DRender_drawRect3(int ctx, sQuadVtx_2BC8* v, sRect_2BC8* dst, sRect_2BC8* uv, t3Matrix* m);
extern "C" void c2DRender_drawRotRect3(int ctx, sQuadVtx_2BC8* v, sRect_2BC8* dst, sRect_2BC8* uv, t3Matrix* m);

/**
 * Draws the texture element @p element tiled @p tx times across and @p ty times down the rectangle @p destRect, like cTextureElement_renderTiled,
 * with every tile's corners transformed by the 3x3 matrix @p transform.
 *
 * Selects the element's texture (tex) as texture 0 of GrMan's current draw state and returns if @p tx or @p ty is not
 * positive. Tiles are r.w / tx by r.h / ty: floor(ty) rows of floor(tx) full tiles, a cropped last tile per row when @p
 * tx has a fraction and a cropped last row when @p ty has one (texture ranges cut by the same fractions). Each quad
 * goes to the 2D renderer G2D through c2DRender_drawRect3, or c2DRender_drawRotRect3 (texture turned 90 degrees) when flag bit
 * 2 is set; both transform the corners by @p transform. Flag bit 0 swaps the left and right texture coordinates, bit 1 the top
 * and bottom ones. Vertex colour: alpha = c.a * 128, red/green/blue = c * 255 (cTextureElement_render uses 128 for all four). No
 * caller in retail references it.
 *
 * @param element  the texture element: texture id (tex) and its texture rectangle (v0 top, u0 left, u1 right, v1 bottom)
 * @param destRect  the screen rectangle to fill (x, y, w, h) before the transform
 * @param transform  the 3x3 transform applied to every tile's corners
 * @param colour  the colour (a, r, g, b in 0..1)
 * @param flags  bit 0 mirror horizontally, bit 1 mirror vertically, bit 2 draw the texture turned 90 degrees
 * @param tx  how many times the texture repeats across (may be fractional)
 * @param ty  how many times the texture repeats down (may be fractional)
 */
extern "C" void cTextureElement_renderRotTiled(sSprite_2BC8* element, sRect_2BC8 destRect, t3Matrix transform, sColor_2BC8 colour, int flags, float tx, float ty)
{
    int j;
    float x;
    int tex = element->tex;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = tex;
    float x0 = destRect.x;
    float y0 = destRect.y;
    float topV = element->v0;
    float bottomV = element->v1;
    float leftU = element->u0;
    float rightU = element->u1;
    if (tx <= 0.0f) {
        return;
    }
    if (ty <= 0.0f) {
        return;
    }
    float w = destRect.w / tx;
    float tileHeight = destRect.h / ty;
    if (flags & 1) {
        leftU = rightU;
        rightU = element->u0;
    }
    if (flags & 2) {
        topV = bottomV;
        bottomV = element->v0;
    }
    int nx = (int)tx;
    float y = y0;
    int ny = (int)ty;
    sQuadVtx_2BC8 quadVerts[4];
    for (int i = 0; i < 4; i++) {
        sQuadVtx_2BC8* vert = &quadVerts[i];
        vert->a = (int)(colour.a * 128.0f);
        vert->r = (int)(colour.r * 255.0f);
        vert->g = (int)(colour.g * 255.0f);
        vert->b = (int)(colour.b * 255.0f);
        vert->q = 1.0f;
    }
    if (ny != 0) {
        for (y = y0, j = 0; j < ny; j++) {
            x = x0;
            if (flags & 4) {
                for (int i = 0; i < nx; i++) {
                    sRect_2BC8 dst(x, y, w, tileHeight);
                    sRect_2BC8 uv(leftU, topV, rightU - leftU, bottomV - topV);
                    c2DRender_drawRotRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_2BC8 dst(x, y, (x0 + destRect.w) - x, tileHeight);
                    sRect_2BC8 uv(leftU, topV, rightU - leftU, (topV + (bottomV - topV) * (tx - (float)nx)) - topV);
                    c2DRender_drawRotRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                }
            } else {
                for (int i = 0; i < nx; i++) {
                    sRect_2BC8 dst(x, y, w, tileHeight);
                    sRect_2BC8 uv(leftU, topV, rightU - leftU, bottomV - topV);
                    c2DRender_drawRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                    x += w;
                }
                if ((float)nx != tx) {
                    sRect_2BC8 dst(x, y, (x0 + destRect.w) - x, tileHeight);
                    sRect_2BC8 uv(leftU, topV, (leftU + (rightU - leftU) * (tx - (float)nx)) - leftU, bottomV - topV);
                    c2DRender_drawRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                }
            }
            y += tileHeight;
        }
    }
    if ((float)ny != ty) {
        x = x0;
        if (flags & 4) {
            float u0b = leftU + (rightU - leftU) * (1.0f - (ty - (float)ny));
            float hh = (y0 + destRect.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_2BC8 dst(x, y, w, hh);
                sRect_2BC8 uv(u0b, topV, rightU - u0b, bottomV - topV);
                c2DRender_drawRotRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_2BC8 dst(x, y, (x0 + destRect.w) - x, hh);
                sRect_2BC8 uv(u0b, topV, rightU - u0b, (topV + (bottomV - topV) * (tx - (float)nx)) - topV);
                c2DRender_drawRotRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
            }
        } else {
            float v1b = topV + (bottomV - topV) * (ty - (float)ny);
            float hh = (y0 + destRect.h) - y;
            for (int i = 0; i < nx; i++) {
                sRect_2BC8 dst(x, y, w, hh);
                sRect_2BC8 uv(leftU, topV, rightU - leftU, v1b - topV);
                c2DRender_drawRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
                x += w;
            }
            if ((float)nx != tx) {
                sRect_2BC8 dst(x, y, (x0 + destRect.w) - x, hh);
                sRect_2BC8 uv(leftU, topV, (leftU + (rightU - leftU) * (tx - (float)nx)) - leftU, v1b - topV);
                c2DRender_drawRect3(G2D_2BC8, quadVerts, &dst, &uv, &transform);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uitext", cUIVector_cUIVector);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self, int a1, int a2);
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

/**
 * Constructs an empty vector shape: the cUIThing constructor with the screen and parent, cUIVector_vtbl, mFill, mLoop
 * and mNumVertices clear and no triangle, colour, position, render-position, UV or vertex arrays.
 *
 * @param pScreen  the screen (On Tour cUIScreen* pScreen)
 * @param pParent  the parent object (On Tour cUIThing* pParent)
 * @return @p self
 */
extern "C" cUIVector_cUIVector_s* cUIVector_cUIVector(cUIVector* self, int pScreen, int pParent)
{
    cUIThing_cUIThing(((cUIVector_cUIVector_s*)self), pScreen, pParent);
    VPTR_OF(cListNode, self) = cUIVector_vtbl;
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
INCLUDE_ASM("ui/uitext", _$_9cUIVector);
#ifdef SKIP_ASM
/**
 * Destroys a vector widget: frees its vertex colour, position, render-position, UV and vertex arrays when it has them;
 * the cUIThing part does the rest and frees it when asked.
 */
cUIVector::~cUIVector()
{
    if (mpVertexColour != 0) {
        cMemMan_free(mpVertexColour);
    }
    if (mpVertexPosition != 0) {
        cMemMan_free(mpVertexPosition);
    }
    if (mpVertexPositionRender != 0) {
        cMemMan_free(mpVertexPositionRender);
    }
    if (mpUV != 0) {
        cMemMan_free(mpUV);
    }
    if (mpVertices != 0) {
        cMemMan_free(mpVertices);
    }
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
void cUIVector::setColour(tARGB* colour)
{
    tARGB scaled = *colour;
    unsigned int i;
    *(&mColour) = *colour;
    if (mpVertexColour == 0) {
        return;
    }
    scaled.A = (int)(colour->A * 255.0f);
    scaled.R = (int)(colour->R * 255.0f);
    scaled.G = (int)(colour->G * 255.0f);
    scaled.B = (int)(colour->B * 255.0f);
    for (i = 0; i < *(unsigned int*)&mAttrVector >> 17; i++) {
        mpVertexColour[i] = scaled;
    }
}
#endif

