#include "common.h"
#include "vtable.h"
#include "ui/uidata.h"
#include "ui/uiscreen.h"
#include "ui/uitext.h"
#include "ui/uithing.h"


//100%
INCLUDE_ASM("ui/uithing", cUIThing_getKeyframerEvent__FP8cUIThingi);
#ifdef SKIP_ASM
/**
 * Returns the keyframer event id stored at an index.
 *
 * @param x  event index; only its low 8 bits are used, as a signed value
 * @return mEvents[x], or 0xFFFF when the thing has no event table
 */
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x)
{
    signed char idx = (signed char)x;
    if (self->mpEvents != 0) {
        return self->mpEvents[idx];
    }
    return 0xFFFF;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_playKeyframerEvent);
#ifdef SKIP_ASM
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Plays the object's keyframer event @p idx on its screen (cUIScreen_playFrame(.., event, 1)).
 *
 * Nothing happens when the object has no event table, the event is 0xFFFF (none) or there is no screen.
 *
 * @param idx  the event index (On Tour char e)
 * @return 1 when the event was played, else 0 (On Tour returns bool)
 */
extern "C" int cUIThing_playKeyframerEvent(cUIThing* self, signed char idx)
{
    if (self->mpEvents != 0 && self->mpEvents[idx] != 0xFFFF) {
        void* scr = self->mpScreen;
        if (scr != 0) {
            cUIScreen_playFrame(scr, self->mpEvents[idx], 1);
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_update);
#ifdef SKIP_ASM
extern "C" void cUIAnimation_updateThing(void*, void*, unsigned char, unsigned short);
extern "C" void cUIAnimation_updateFrame(void*, unsigned char);

/**
 * Advances the UI thing's current animation, if any, through cUIAnimation_updateThing() and _updateFrame().
 *
 * @param ev  the animation frame number, passed to cUIAnimation_updateThing() (On Tour FrameNum)
 */
extern "C" void cUIThing_update(cUIThing* self, unsigned short ev)
{
    void* anim = self->mpAnim;
    if (anim != 0) {
        cUIAnimation_updateThing(anim, self, self->mAnimPlaybackStatus, ev);
        cUIAnimation_updateFrame(self->mpAnim, self->mAnimPlaybackStatus);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_setChannelData);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sUIThing0048 {
    char pad0[0x14];
    unsigned int b0 : 13;
    unsigned int frame : 6;   // 0x14 bits 13..18
    unsigned int b19 : 13;
    char pad18[0x4];
    float r;                  // 0x1C
    float g;                  // 0x20
    float b;                  // 0x24
    float a;                  // 0x28
    float rotX;               // 0x2C
    float rotY;               // 0x30
    float rotZ;               // 0x34
    char pad38[0xC];
    float x;                  // 0x44
    float y;                  // 0x48
    char pad4C[0x4];
    float scaleX;             // 0x50
    float scaleY;             // 0x54
    char pad58[0x8];
    float f60;                // 0x60
    float f64;                // 0x64
};

/**
 * Sets one animatable property of a widget from a keyframe value.
 *
 * Channels: 0-1 position, 3-5 rotation in degrees, 6-7 size, 9-10 scale in percent, 12 frame, 13-16 alpha and
 * RGB in 0-255. Other channels are ignored.
 *
 * @param key  the channel (On Tour channel)
 * @param data  the value in the channel's units
 */
extern "C" void cUIThing_setChannelData(cUIThing* self, unsigned short key, float data)
{
    switch (key) {
    case 0:
        self->mPosition.x = data;
        break;
    case 1:
        self->mPosition.y = data;
        break;
    case 9:
        self->mScale.x = data * 0.01f;
        break;
    case 10:
        self->mScale.y = data * 0.01f;
        break;
    case 3:
        self->mAngle.x = data * 0.017444444820284843f;
        break;
    case 4:
        self->mAngle.y = data * 0.017444444820284843f;
        break;
    case 5:
        self->mAngle.z = data * 0.017444444820284843f;
        break;
    case 12:
        ((sUIThing0048*)self)->frame = (int)data;
        break;
    case 13:
        self->mColour.A = data * 0.003921568859368563f;
        break;
    case 14:
        self->mColour.R = data * 0.003921568859368563f;
        break;
    case 15:
        self->mColour.G = data * 0.003921568859368563f;
        break;
    case 16:
        self->mColour.B = data * 0.003921568859368563f;
        break;
    case 6:
        self->mSize.x = data;
        break;
    case 7:
        self->mSize.y = data;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_setChannelDataKCP);
#ifdef SKIP_ASM
/**
 * Applies a list of animation key values to the UI thing, one channel per pair.
 *
 * Each pair is a channel ID and a value: 0 and 1 position x/y, 3 to 5 angle x/y/z (degrees), 6 and 7 size x/y,
 * 9 and 10 scale x/y (percent), 12 attribute bits 13-18, 13 to 16 colour alpha, red, green and blue (0-255).
 * Other IDs are ignored.
 *
 * @param vself  the UI thing (this)
 * @param count  the number of pairs
 * @param pPtr      the pairs, as shorts: channel ID, then value
 */
extern "C" void cUIThing_setChannelDataKCP(cUIThing* vself, unsigned int count, short *pPtr)
{
    unsigned int i = 0;

    if (count != 0) {
        do {
            float data = (float)pPtr[1];
            short id = pPtr[0];
            switch (id) {
            case 0:
                vself->mPosition.x = data;
                break;
            case 1:
                vself->mPosition.y = data;
                break;
            case 9:
                vself->mScale.x = data * 0.01f;
                break;
            case 10:
                vself->mScale.y = data * 0.01f;
                break;
            case 3:
                vself->mAngle.x = data * 0.017444445f;
                break;
            case 4:
                vself->mAngle.y = data * 0.017444445f;
                break;
            case 5:
                vself->mAngle.z = data * 0.017444445f;
                break;
            case 12:
                vself->mAttr = (vself->mAttr & 0xFFF81FFF) | (((int)data & 0x3F) << 13);
                break;
            case 13:
                vself->mColour.A = data * 0.003921569f;
                break;
            case 14:
                vself->mColour.R = data * 0.003921569f;
                break;
            case 15:
                vself->mColour.G = data * 0.003921569f;
                break;
            case 16:
                vself->mColour.B = data * 0.003921569f;
                break;
            case 6:
                vself->mSize.x = data;
                break;
            case 7:
                vself->mSize.y = data;
                break;
            }
            i++;
            pPtr += 2;
        } while (i < count);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", func_003A0290);
#ifdef SKIP_ASM
struct sUIVec3 {
    float x, y, z;
};

struct sUIThingPos {
    char pad0[0x40];
    sUIThingPos* parent;    // 0x40
    sUIVec3 pos;            // 0x44
};

/**
 * Writes the UI thing's absolute position, its mPosition plus the mPosition of every ancestor up the mpParent chain, to
 * @p out.
 *
 * The reward galleries (cFEStateRewardGalleryBase_onGainFocus, cFEStateRewardGalleryBase_updateHighlither), the
 * uber-trick menu and the async-file screens use it to place a highlight or another widget over a nested widget.
 *
 * @param out  receives the summed position (x, y, z)
 */
extern "C" void func_003A0290(sUIThingPos* self, sUIVec3* out)
{
    *out = self->pos;
    for (sUIThingPos* ancestor = self->parent; ancestor != 0; ancestor = ancestor->parent) {
        sUIVec3 ancestorPos = ancestor->pos;
        out->x += ancestorPos.x;
        out->y += ancestorPos.y;
        out->z += ancestorPos.z;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_getSound);
#ifdef SKIP_ASM
/**
 * Returns the UI thing's sound id number @p soundIndex (mSound[soundIndex]; the index is taken as a signed char).
 *
 * cUIMenu_onInput and cUIListBox_onInput fetch sound 0 or 1 of the selected widget with it when the selection moves or
 * is confirmed.
 *
 * @param soundIndex  which of the two sounds (0 or 1)
 * @return the sound id stored there (the cUIThing constructor stores 4 and 6)
 */
extern "C" int cUIThing_getSound(void* self, int soundIndex)
{
    self = (char*)self + ((signed char)soundIndex << 2);
    return *(int*)((char*)self + 0x6c);
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIThing_setSound);
#ifdef SKIP_ASM
/**
 * Stores @p sound as the UI thing's sound number @p soundIndex (mSound[soundIndex]; the index is taken as a signed
 * char).
 *
 * cFEStateTitle_onWidgetCreate uses it to set a menu item's first sound to 0.
 *
 * @param soundIndex  which of the two sounds (0 or 1)
 * @param sound  the sound id to store
 */
extern "C" void cUIThing_setSound(void* self, int soundIndex, int sound)
{
    self = (char*)self + ((signed char)soundIndex << 2);
    *(int*)((char*)self + 0x6c) = sound;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIText_cUIText);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self);
extern void* cUIText_vtbl[];

struct cUIText_cUIText_sQuad {
    int a, b, c, d;
};
extern cUIText_cUIText_sQuad gStaticColour;

struct cUIText_cUIText_sObj {
    char base[0x74];
    unsigned char flags;
    char pad75[0x88 - 0x75];
    cUIText_cUIText_sQuad q88;
    cUIText_cUIText_sQuad q98;
    int fA8;
    int fAC;
    int fB0;
    int fB4;
    short fB8;
    short fBA;
};

/**
 * Constructs a cUIText in place and returns it.
 *
 * Runs the cUIThing constructor (cUIThing_cUIThing; the screen and parent the caller passes in a1/a2 go straight
 * through), stores cUIText_vtbl, sets tAttrText's shadow bit (bit 2) and clears bits 3-6 (unicode, static colour,
 * disabled colour, ellipses), zeroes mNumLines, mpLineOffset, mStringID, mpString, mStringSize and mLineSize, and
 * copies the default colour gStaticColour into mStaticColour and mDisabledColour. cUIScreen_createObjectByStruct calls it
 * on a 0xBC-byte "UIText" allocation; cUIText_cUIText1 is the default constructor.
 *
 * @return the object
 */
extern "C" cUIText_cUIText_sObj* cUIText_cUIText(cUIText_cUIText_sObj* self)
{
    cUIThing_cUIThing(self);
    *(void***)((char*)self + 8) = cUIText_vtbl;
    self->flags = (self->flags | 4) & 0x87;
    self->fA8 = 0;
    self->fAC = 0;
    self->fB0 = 0;
    self->fB4 = 0;
    self->fB8 = 0;
    self->fBA = 0;
    self->q88 = gStaticColour;
    self->q98 = gStaticColour;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIText_setUIData);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText*, int);
// PORT: cUIThing_setUIData really takes (self, data); other units declare it with one argument.
void* cUIThing_setUIData_2(void* self, void* data) __asm__("cUIThing_setUIData");

struct sUIColor03F0 {
    float r, g, b, a;
    sUIColor03F0() {}
    sUIColor03F0(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};
struct sUIVt03F0 { short delta; short index; void (*fn)(void*, int); };
struct sUIThing03F0 {
    char pad0[0x8];
    sUIVt03F0* vt;              // 0x8
    char padC[0x74 - 0xC];
    unsigned int mode : 2;      // 0x74
    unsigned int rest : 30;
    sUIColor03F0 color;         // 0x78
};

/**
 * Sets a text widget up from its screen-data record: font, visibility, localized string and, when enabled,
 * colour.
 *
 * @param data  the widget's record (On Tour tUIBaseObject* pUIBase)
 */
extern "C" void cUIText_setUIData(sUIThing03F0* self, tUIText* data)
{
    cUIThing_setUIData_2(self, data);
    self->mode = data->mFontID & 3;
    self->vt[23].fn(vthis(self, self->vt[23]), data->mBools1 & 1);
    cUIText_setUnicodeStringByID((cUIText*)self, ((int)data->mStringID));
    int textFlags = *(int*)((char*)self + 0x74) >> 2;
    if (textFlags & 1) {
        self->color = sUIColor03F0(data->mSColour.a * 0.003921568859368563f, data->mSColour.r * 0.003921568859368563f,
                                   data->mSColour.g * 0.003921568859368563f, data->mSColour.b * 0.003921568859368563f);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIText_getFont);
#ifdef SKIP_ASM
/**
 * Returns the font of the text widget's font slot from the UI engine's font interface.
 *
 * @return the cFont (as int), or 0 without a font interface
 */
extern "C" int cUIText_getFont(cUIText* self)
{
    void* engine = *(void**)((char*)self->mpScreen->mpState + 0x10);
    if (engine != 0) {
        return *(int*)((char*)*(void**)((char*)engine + 0x8) + (*(unsigned char*)&self->mAttrText & 3) * 12 + 0xC);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uithing", cUIText_updateLocale);
#ifdef SKIP_ASM
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern "C" void cUIText_setUnicodeStringPrivate(void* self, unsigned short* str);

struct cUIText_updateLocale_sVEntry {
    short delta;
    short index;
    void* fn;
};

/**
 * Reloads a text widget's localized string for the current language, wrapping it when the widget wraps.
 */
extern "C" void cUIText_updateLocale(cUIText* self)
{
    int id = self->mStringID;
    if (id != 0) {
        cUIScreen* screen = self->mpScreen;
        void* state = screen->mpState;
        void* engine = *(void**)((char*)state + 0x10);
        char* loc = *(char**)((char*)engine + 0x10);
        if (loc != 0) {
            cUIText_updateLocale_sVEntry* vt = *(cUIText_updateLocale_sVEntry**)(loc + 4);
            unsigned short* str = ((unsigned short* (*)(void*, int))vt[4].fn)(vthis(loc, vt[4]), id);
            if (str != 0) {
                int attrBits = self->mAttr >> 7;
                if ((attrBits & 1) || ((self->mAttrText >> 6) & 1)) {
                    cUIText_setUnicodeStringWrap(self, str);
                } else {
                    cUIText_setUnicodeStringPrivate(self, str);
                }
            }
            cUIText_updateLocale_sVEntry* vt2 = (cUIText_updateLocale_sVEntry*)self->mVtbl;
            ((void (*)(void*, int))vt2[5].fn)(vthis(self, vt2[5]), 1);
        }
    }
}
#endif

