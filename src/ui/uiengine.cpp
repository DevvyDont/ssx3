#include "vtable.h"
#include "common.h"
#include "ealib/shape.h"
#include "render/font.h"
#include "render/2drender.h"
#include "ui/uidata.h"
#include "bx/math3d.h"
#include "fe/ovstatehud.h"
#include "ui/uifontinterface.h"
#include "ui/uiscreen.h"
#include "ui/uiinput.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "ui/uitexturebank.h"
#include "ui/uithing.h"
#include "ui/uilistbox.h"
#include "ui/uiicon.h"
#include "ui/uigroup.h"
#include "render/ps2graphicsman.h"
#include "util/locale.h"

//100%
INCLUDE_ASM("ui/uiengine", UIAsciiToUnicode__FPUsPCc);
#ifdef SKIP_ASM
/**
 * Widens a NUL-terminated byte string to 16-bit characters.
 *
 * Each byte goes through a (char) cast, so bytes 0x80-0xFF are sign-extended to 0xFF80-0xFFFF rather than
 * mapped to U+0080-U+00FF.
 *
 * @param dst  destination buffer of 16-bit characters, including room for the terminator
 * @param src  NUL-terminated byte string
 */
void UIAsciiToUnicode(unsigned short* dst, const char* src)
{
    for (; *src != 0; src++, dst++) {
        *dst = (char)*(unsigned char*)src;
    }
    *dst = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_cUIEngine);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* self);

/**
 * Constructs a UI engine: clears its interface pointers and builds its state stack, animation bank and texture bank.
 *
 * Zeroes the six interface pointers (+0x0..+0x14), constructs the state stack's two lists (+0x18, +0x34), clears
 * +0x58..+0x68 and points the animation bank (+0x54) and texture bank (+0x6C) back at the engine. cFE_cFE and the
 * load-screen loaders build their UI with it.
 *
 * @return @p self
 */
extern "C" void* cUIEngine_cUIEngine(cUIEngine* self)
{
    self->mpCustom = 0;
    self->mpData = 0;
    self->mpFontInterface = 0;
    self->mpInput = 0;
    self->mpLocaleInterface = 0;
    self->mpSound = 0;
    cList_cList((char*)&self->mStateStack);
    cList_cList(&self->mStateStack.mExplicitStack);
    *(int*)&self->mTextureBank = 0;
    self->mTextureBank.mpTexturesIDs = 0;
    self->mTextureBank.mNumTextureIDs = 0;
    self->mTextureBank.mpElements = 0;
    self->mTextureBank.mNumElements = 0;
    self->mAnimBank.mpEngine = (cUIEngine*)((void*)self);
    self->mTextureBank.mpEngine = (cUIEngine*)((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine__cUIEngine);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cUITextureBank__cUITextureBank(cUITextureBank* self, int flags);
extern "C" void cUIStateStack__cUIStateStack(void* self, int flags);

class cUIEngOwnA {
public:
    int f0;
    virtual ~cUIEngOwnA();
};

class cUIEngOwnB {
public:
    int f0;
    int f4;
    virtual ~cUIEngOwnB();
};

class cUIEngOwnC {
public:
    virtual ~cUIEngOwnC();
};


/**
 * Destroys a UI engine: deletes its custom, input and sound interfaces, frees its data and destroys its texture bank
 * and state stack.
 *
 * Deletes +0x0, +0xC and +0x14 through their virtual destructors and frees the loaded data (+0x4); the font and locale
 * interfaces (+0x8, +0x10) are left alone. Then cUITextureBank__cUITextureBank destroys the texture bank (+0x58) and cUIStateStack__cUIStateStack the
 * state stack (+0x18); bit 0 of @p flags frees the engine.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUIEngine__cUIEngine(cUIEngine* self, int flags)
{
    cUIEngine* fields = self;
    delete ((cUIEngOwnA*)fields->mpCustom);
    if (((void*)fields->mpData)) {
        cMemMan_free(((void*)fields->mpData));
    }
    delete ((cUIEngOwnB*)fields->mpInput);
    delete ((cUIEngOwnC*)fields->mpSound);
    cUITextureBank__cUITextureBank((cUITextureBank*)&self->mTextureBank, 2);
    cUIStateStack__cUIStateStack((char*)&self->mStateStack, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_loadFile);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* FILE_load(const char* name, int flags);
void cUIAnimationBank_setData(void* self, int val);
extern "C" void cUITextureBank_setData(cUITextureBank* bank, int id, void* data);
extern const char str_pct_s_lui[];

/**
 * Loads a UI file ("<name>.lui") and sets up its fonts and texture bank from the blocks it contains.
 *
 * Despite the int type, @p id is the file's base name (it is formatted with %s; On Tour const char*
 * pszFilename).
 *
 * @param id  base name of the UI file (char*, passed as int)
 * @return 1 (the load result is not checked)
 */
int cUIEngine::loadFile(int id)
{
    char name[100];
    sprintf(name, str_pct_s_lui, id);
    char* file = (char*)FILE_load(name, 0);
    mpData = file;
    tUIHeaderBlock* header = (tUIHeaderBlock*)file;
    // PORT: the animation bank's setter takes the data pointer as an int.
    cUIAnimationBank_setData(&mAnimBank, (int)(file + header->mAnimBlockOffset));
    file = mpData;
    header = (tUIHeaderBlock*)file;
    mTextureBank.setData(id, file + header->mTextureBlockOffset);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_addScreenByHashName);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cList_cList(void* self);
extern "C" void tAttrScreen_tAttrScreen(void* self);
extern "C" void cUIScreen_setData(void* self, char* data);
extern "C" void cUIState_addScreen(void* self, void* node);
extern "C" void cUIScreen_createAllObjects(void* self);
// PORT: cUIThing_cUIThing really takes (self, a, b); the unit's other caller uses a 1-arg declaration.
void* cUIThing_cUIThing_3(void* self, int a, int b) __asm__("cUIThing_cUIThing");
extern char str_UIScreen[];
extern void* cUIScreen_vtbl[];
extern void* cUIGroup_vtbl[];

struct sUIScreenD4 {
    sUIScreenD4* next;          // 0x0
    sUIScreenD4* prev;          // 0x4
    void** vt;                  // 0x8
    int fC;
    int f10;
    char o14[4];                // 0x14
    char o18[0x1C];             // 0x18
    int f34;
    int f38;
    int f3C;
    char o40[8];                // 0x40
    void** vt48;                // 0x48
    char pad4C[0xB4 - 0x4C];
    char oB4[0x1C];             // 0xB4
    void* owner;                // 0xD0
};
struct sUIScrEnt { unsigned int hash; int offset; };
struct sUIScrTbl { unsigned int count; sUIScrEnt ents[1]; };

/**
 * Creates a screen from the loaded UI file's screen with the given name hash and adds it to a state.
 *
 * A new cUIScreen is allocated unless @p screen supplies one; the state gets the screen and then its widgets
 * are
 * created. A hash missing from the file is not checked for.
 *
 * @param owner  the state that owns the screen (On Tour cUIState* pState)
 * @param hash  the screen's name hash (On Tour hashname)
 * @param screen  an existing screen object to fill, or 0 to allocate one
 * @return the screen
 */
void* cUIEngine::addScreenByHashName(cUIState* owner, unsigned int hash, void* screen)
{
    char* bank = mpData;
    sUIScrTbl* tbl = (sUIScrTbl*)(bank + *(int*)(bank + 0xC));
    sUIScrEnt* found = 0;
    sUIScrEnt* screenOffset = tbl->ents;
    for (unsigned int i = 0; i < tbl->count; i++, screenOffset++) {
        if (screenOffset->hash == hash) {
            found = screenOffset;
            break;
        }
    }
    char* data = (char*)tbl + found->offset;
    if (screen == 0) {
        sUIScreenD4* newScreen = new (str_UIScreen, 0x100, 0) sUIScreenD4;
        newScreen->prev = newScreen;
        newScreen->next = newScreen;
        newScreen->vt = cUIScreen_vtbl;
        newScreen->fC = 0;
        newScreen->f10 = 0;
        tAttrScreen_tAttrScreen(newScreen->o14);
        cList_cList(newScreen->o18);
        newScreen->f34 = 0;
        newScreen->f38 = 0;
        newScreen->f3C = 0;
        cUIThing_cUIThing_3(newScreen->o40, 0, 0);
        newScreen->vt48 = cUIGroup_vtbl;
        cList_cList(newScreen->oB4);
        newScreen->owner = ((void*)owner);
        screen = newScreen;
    }
    cUIScreen_setData(screen, data);
    cUIState_addScreen(owner, screen);
    cUIScreen_createAllObjects(screen);
    return screen;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_render2D);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sUIRS_7DF8 {
    unsigned int f0_0 : 2;
    unsigned int f0_2 : 2;
    unsigned int f0_4 : 28;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    int f8;
    int fC;
    int f10;
};

class cUICtx_7DF8 {
public:
    char pad0[0xE84];
    sUIRS_7DF8* top;
    char pad1[0x10D8 - 0xE88];
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
    virtual void PushViewport();
    virtual void PopViewport();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void SetViewportOrthographic(int a, int b, float x, float y, float w, float h, float zn, float zf);
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void PushView();
    virtual void PopView();
    virtual void v33();
    virtual void SetView1(void* p);
};

// Typed view of the render context (the unit declares GrMan as void* later).
extern cUICtx_7DF8* GrMan_7df8 __asm__("GrMan");
extern t4Matrix t4mat_Identity;
extern sUIRS_7DF8 sDrawState_s_defaultDrawState_7df8 __asm__("sDrawState_s_defaultDrawState");
extern "C" void cUIStateStack_render2D(void* p);

static inline void setF4_22_7DF8(sUIRS_7DF8* rs, int v) { rs->f4_22 = v; }
static inline void setF4_23_7DF8(sUIRS_7DF8* rs, int v) { rs->f4_23 = v; }
static inline void setF4_20_7DF8(sUIRS_7DF8* rs, int v) { rs->f4_20 = v; }
static inline void setF4_12_7DF8(sUIRS_7DF8* rs, int v) { rs->f4_12 = v; }
static inline void setF4_2_7DF8(sUIRS_7DF8* rs, int v) { rs->f4_2 = v; }

/**
 * Draws a UI engine's 2D layer: sets up a 640x480 screen-space view and renders the state stack.
 *
 * Pushes a copy of the draw state and resets it to sDrawState_s_defaultDrawState, sets a 0..640 x 0..480 view (z -1..1)
 * and the identity matrix t4mat_Identity through GrMan virtuals, sets the 2D draw-state bits, then renders the state
 * stack (+0x18) with cUIStateStack_render2D and pops the draw state. cRealFE_render, the load screens and the game and app
 * render code call it each frame.
 */
void cUIEngine::render2D()
{
    cUICtx_7DF8* ctx = GrMan_7df8;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *GrMan_7df8->top = sDrawState_s_defaultDrawState_7df8;
    GrMan->PushView();
    GrMan->PushViewport();
    GrMan->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f, 0, 1);
    GrMan->SetView1(t4mat_Identity);
    setF4_22_7DF8(GrMan_7df8->top, 1);
    setF4_23_7DF8(GrMan_7df8->top, 2);
    setF4_20_7DF8(GrMan_7df8->top, 0);
    setF4_12_7DF8(GrMan_7df8->top, 20);
    setF4_2_7DF8(GrMan_7df8->top, 5);
    GrMan_7df8->top->f0_2 = 3;
    cUIStateStack_render2D((char*)&mStateStack);
    GrMan->PopViewport();
    GrMan->PopView();
    GrMan_7df8->top--;
}
#endif

void cUIStateStack_render3D(void*);

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_render3D);
#ifdef SKIP_ASM
/**
 * Draws a UI engine's 3D layer: calls cUIStateStack_render3D__FPv (empty) on the state stack (+0x18).
 *
 * cRealFE_render calls it just before cUIEngine_render2D.
 */
void cUIEngine::render3D()
{
    cUIStateStack_render3D((char*)&mStateStack);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIEngine_update);
#ifdef SKIP_ASM

extern "C" void cUIStateStack_update(void*);

/**
 * Updates a UI engine: polls its input object (+0xC, virtual slot 46), then updates the state stack (cUIStateStack_update on
 * +0x18).
 *
 * cRealFE_update, cFELoadScreen_update, the game loader and the app's UI loops call it.
 */
void cUIEngine::update()
{
    cUIInput* obj = mpInput;
    obj->update();
    cUIStateStack_update((char*)&mStateStack);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUITextureBank__cUITextureBank);
#ifdef SKIP_ASM
extern void* GrMan_uiengine __asm__("GrMan");

/**
 * Destroys a UI texture bank: releases its textures through the graphics manager and frees its arrays.
 *
 * Each of the +0x8 texture ids in the array at +0x4 goes to GrMan virtual slot 50; the id array and the element array
 * (+0xC) are freed; bit 0 of @p flags frees the bank. cUIEngine__cUIEngine runs it on the engine's +0x58.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUITextureBank__cUITextureBank(cUITextureBank* self, int flags)
{
    int i;
    for (i = 0; i < self->mNumTextureIDs; i++) {
        cPS2GraphicsMan* obj = (cPS2GraphicsMan*)((char*)GrMan_uiengine);
        obj->RemoveTexture((self->mpTexturesIDs)[i]);
    }
    if (self->mpTexturesIDs != 0) {
        cMemMan_free(self->mpTexturesIDs);
    }
    if (self->mpElements != 0) {
        cMemMan_free(self->mpElements);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUITextureBank_setData);
#ifdef SKIP_ASM
struct sUITexElem8138 {
    int mSourceShape;                // 0x00
    float mWidth;               // 0x04
    float mHeight;               // 0x08
    float mTop;               // 0x0C
    float mLeft;              // 0x10
    float mRight;              // 0x14
    float mBottom;              // 0x18
    int mHashID;                 // 0x1C
    sUITexElem8138()
    {
        mSourceShape = -1;
        mWidth = 1.0f;
        mHeight = 1.0f;
        mTop = 0.0f;
        mLeft = 0.0f;
        mRight = 1.0f;
        mBottom = 1.0f;
    }
};

struct sUITexSrc8138 {
    int id;                 // 0x00
    char b4;                // 0x04
    unsigned char tex;      // 0x05
    char pad6[2];
    float f8;               // 0x08
    float fC;               // 0x0C
    float f10;              // 0x10
    float f14;              // 0x14
};

struct sUITexData8138 {
    int count;              // 0x00
    sUITexSrc8138 e[1];     // 0x04
};


struct sUIFshDir8138 {
    int offset;
    int pad;
};



extern char str_pct_s_1_ssh[];
extern char str_UITextureIDs[];
extern char str_UITextureELs[];
extern char str_UITPage[];
extern char* GrMan_8138 __asm__("GrMan");
extern "C" SHAPEFILE* FILE_loadpack(char* name, int flags);
extern "C" void MEM_free(void* p);
extern "C" void* memset(void* dst, int c, int n);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

/**
 * Loads the UI texture sheet "<name>_1.ssh" into the renderer and builds the bank's texture-element table.
 *
 * Every shape becomes a texture in the "UITPage" group (renderer slot 46); each element of @p vdata keeps its
 * texture coordinates and the texture ID of its shape.
 *
 * @param id  base name of the texture sheet (char*, passed as int; On Tour pShapeName)
 * @param vdata  the UI file's texture-element block (On Tour pData)
 */
void cUITextureBank::setData(int id, void* vdata)
{
    cUITextureBank* self = this;
    sUITexData8138* data = (sUITexData8138*)vdata;
    char name[0x70];
    int i = 0;
    sprintf(name, str_pct_s_1_ssh, id);
    SHAPEFILE* fsh = FILE_loadpack(name, 0x3000100);
    self->mNumTextureIDs = fsh->num;
    self->mpTexturesIDs = (int*)operator_new_tag(self->mNumTextureIDs * 4, str_UITextureIDs, 0, 0);
    memset(self->mpTexturesIDs, 0xFF, self->mNumTextureIDs * 4);
    for (; i < self->mNumTextureIDs; i++) {
        char* shape = (char*)fsh + ((sUIFshDir8138*)&fsh->tbl[0].offset)[i].offset;
        if (shape != 0) {
            sVEntry<int(void*, void*, void*, int, int, int)>* vt = *(sVEntry<int(void*, void*, void*, int, int, int)>**)(GrMan_8138 + 0x10D8);
            self->mpTexturesIDs[i] = vt[46].fn(vthis(GrMan_8138, vt[46]), shape, str_UITPage, 0, 1, -1);
        }
    }
    MEM_free(fsh);
    sUITexSrc8138* src = data->e;
    self->mpTextureTableHeader = (tUITextureTableHeader*)data;
    int n = data->count;
    self->mNumElements = n;
    sUITexElem8138*& slot = ((sUITexElem8138*)self->mpElements);
    slot = new (str_UITextureELs, 0, 0) sUITexElem8138[n];
    memset(((sUITexElem8138*)self->mpElements), 0xFF, self->mNumElements * 4);
    int j;
    for (j = 0; j < self->mNumElements; j++, src++) {
        ((sUITexElem8138*)self->mpElements)[j].mTop = src->fC;
        ((sUITexElem8138*)self->mpElements)[j].mLeft = src->f8;
        ((sUITexElem8138*)self->mpElements)[j].mBottom = src->f14;
        ((sUITexElem8138*)self->mpElements)[j].mRight = src->f10;
        ((sUITexElem8138*)self->mpElements)[j].mHashID = src->id;
        ((sUITexElem8138*)self->mpElements)[j].mSourceShape = self->mpTexturesIDs[src->tex];
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUITextureBank_getElementByHashname);
#ifdef SKIP_ASM

/**
 * Looks up a texture element by its hashed name.
 *
 * Scans the elements (mpElements, +0xC; mNumElements at +0x10; 0x20 bytes each) and returns the first whose mHashID
 * (+0x1C) equals @p id.
 *
 * @param id  hashed element name (On Tour hashname)
 * @return the element (On Tour cTextureElement), or 0
 */
cTextureElement* cUITextureBank::getElementByHashname(int id)
{
    int i;
    for (i = 0; i < mNumElements; i++) {
        cTextureElement* item = &mpElements[i];
        if (((int)item->mHashID) == id) {
            return item;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUITextureBank_getTextureByHashname);
#ifdef SKIP_ASM

/**
 * Returns the texture of the UI texture bank element with hash name @p key, or 0.
 *
 * Searches the bank's +0x10 elements (0x20 bytes each, from +0xC) for one whose +0x1C equals @p key and returns its
 * first word. Nothing in the retail code references it.
 *
 * @param key  the element hash name
 * @return the texture, or 0 when not found
 */
int cUITextureBank::getTextureByHashname(int key)
{
    int count = mNumElements;
    for (int i = 0; i < count; i++) {
        cTextureElement* entry = &mpElements[i];
        if (((int)entry->mHashID) == key) {
            return entry->mSourceShape;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIFontInterface_getFontByHashName);
#ifdef SKIP_ASM


/**
 * Returns the font of the font-interface entry with hash name @p key, or 0.
 *
 * Searches the up to 4 entries (0xC bytes from +0x4; count at +0x34) for hash @p key and returns its font (+0x8). The
 * HUD (cOVStateHUD1P_onCreateScreen) and trophy screens fetch the "fefont" font with it.
 *
 * @param key  the font hash name
 * @return the font, or 0 when not found
 */
int cUIFontInterface::getFontByHashName(int key)
{
    signed char i;
    for (i = 0; i < mFontCount; i++) {
        cUIFontEntry* entry = &((&mFontEntry[0]))[i];
        if (((int)entry->mHashID) == key) {
            return ((int)entry->mpFont);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIFontInterface_purgeFonts);
#ifdef SKIP_ASM
extern "C" void _$_5cFont(cFont* h, int a);

/**
 * Releases all fonts of a font interface and empties it.
 *
 * Each entry with a font (+0x8) has it destroyed with cFont::~cFont(font, 3); every entry's name and hash are cleared
 * and the count (+0x34) goes to 0. cSSXApp_purge calls it.
 */
void cUIFontInterface::purgeFonts()
{
    for (int i = 0; i < mFontCount; i++) {
        cUIFontEntry* fontEntry = &((&mFontEntry[0]))[i];
        delete (fontEntry->mpFont);
        fontEntry->mpFileName = 0;
        fontEntry->mHashID = 0;
    }
    mFontCount = 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIFontInterface_addFontEntry);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cUIFontInterface::addFontEntry (On Tour): fill font slot `index` (cUIFontEntry::setFileName then
// setHashName, both inline here) and bump the font count.

static inline void uiFontSetFileName(cUIFontEntry* e, const char* name)
{
    (*(int*)&e->mpFileName) = (int)name;
}

static inline void uiFontSetHashName(cUIFontEntry* e, unsigned int hash)
{
    e->mHashID = hash;
}

/**
 * Fills font-interface entry @p index with a font file name and hash name and counts it, unless 4 entries are already
 * used.
 *
 * cSSXApp_initload adds "fefont" and "hudfont" with it; cUIFontInterface_setData adds the entries of a font table.
 *
 * @param index  the entry to fill, 0..3
 * @param pszFontName  the font file name
 * @param hashID  the font hash name
 */
void cUIFontInterface::addFontEntry(int index, char* pszFontName, unsigned int hashID)
{
    if (mFontCount < 4) {
        uiFontSetFileName(&((&mFontEntry[0]))[index], pszFontName);
        uiFontSetHashName(&((&mFontEntry[0]))[index], hashID);
        mFontCount++;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiengine", cUIFontInterface_setData);
#ifdef SKIP_ASM
extern "C" void cUIFontInterface_addFontEntry(void* self, unsigned int index, void* data, int type);


/**
 * Points a font interface at a font table and adds each of its records as a font entry.
 *
 * Stores @p hdr in mpFontTableHeader; each record (after the 4-byte count) has its hash name at +0x0, its size at
 * +0x4 and the file name from +0x8, and goes to cUIFontInterface_addFontEntry with its index. Nothing in the C code
 * calls it directly.
 *
 * @param hdr  the font table: a count followed by the records
 */
void cUIFontInterface::setData(tUIFontTableHeader* hdr)
{
    // ORIGINAL-PTR: the records are variable-length ({hash, name size, name}) in the loaded UI file's font chunk, so
    // the walk steps over each by its stored size
    char* rec = (char*)hdr + 4;
    mpFontTableHeader = hdr;
    unsigned int i = 0;
    if (hdr->mNumEntries != 0) {
        do {
            cUIFontInterface_addFontEntry(this, i, rec + 8, *(int*)rec);
            i++;
            rec += *(int*)(rec + 4) + 8;
        } while (i < mpFontTableHeader->mNumEntries);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIFontInterface_loadFonts);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int __5cFontPCci(void* mem, int name, int a2);
extern char str_UI_Font[];

static inline int uiFontNameOf(cUIFontEntry* e)
{
    return ((int)e->mpFileName);
}

/**
 * Creates a cFont for each of the UI's font entries.
 */
void cUIFontInterface::loadFonts()
{
    for (int i = 0; i < mFontCount; i++) {
        signed char j = i;
        *(int*)&mFontEntry[j].mpFont = __5cFontPCci(cMemMan_alloc(132, str_UI_Font, 0, 0),
                uiFontNameOf(&((&mFontEntry[0]))[i]), 0);
    }
}
#endif

extern "C" void* cUIThing_setUIData(void* self);

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_setUIData__FPv);
#ifdef SKIP_ASM
/**
 * Sets the group's UI data using the base cUIThing_setUIData().
 */
void cUIGroup::setUIData(tUIBaseObject*)
{
    cUIThing_setUIData(this);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_setFocus);
#ifdef SKIP_ASM
struct cUIGroup_setFocus_sFlags {
    unsigned int pad0 : 4;
    unsigned int on : 1;
};

/**
 * Gives or removes focus on a widget group and all of its children.
 *
 * @param on  nonzero to focus (On Tour focus)
 */
void cUIGroup::setFocus(int on)
{
    {
        cListOpSetFocus focusOp(on);
        mList.traverseFlat(&focusOp);
    }
    ((cUIGroup_setFocus_sFlags*)&mAttr)->on = (on != 0);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_getObjectByHashName);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

/**
 * Finds a widget by name hash in a group: the group itself, or the first child (searched recursively) that
 * matches.
 *
 * @param id  the name's hash (On Tour hashname)
 * @return the widget, or 0
 */
cUIThing* cUIGroup::getObjectByHashName(unsigned int id)
{
    if (cUIThing::getObjectByHashName(id) != 0) {
        return this;
    }
    cUIThing* n = (cUIThing*)cList_first((cList*)&mList);
    if (((void*)n) != 0) {
        do {
            cUIThing* found = n->getObjectByHashName(id);
            if (found != 0) {
                return found;
            }
            n = (cUIThing*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_updateLocale);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

/**
 * Refreshes the localised text of every object in a UI group.
 *
 * Calls updateLocale (vtable slot 16) on each child in the group's list.
 */
void cUIGroup::updateLocale()
{
    cUIThing* n = (cUIThing*)cList_first((cList*)&mList);
    if (((void*)n) != 0) {
        do {
            n->updateLocale();
            n = (cUIThing*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_onInput);
#ifdef SKIP_ASM
/**
 * Offers an input to a group's children until one of them uses it; a disabled group (bit 5 of its flags)
 * ignores input.
 *
 * @param pInput  the input
 * @return true when a child used the input, otherwise false
 */
bool cUIGroup::onInput(const cUIInput* pInput)
{
    if ((mAttr >> 5) & 1) {
        return false;
    }
    cListOpOnInput inputOp(pInput);
    return mList.traverseFlat(&inputOp) != 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_render3D);
#ifdef SKIP_ASM
/**
 * Renders a group's children in 3D.
 *
 * @param pRenCon  the render context (On Tour cUIRenderContext*)
 */
void cUIGroup::render3D(cUIRenderContext* pRenCon)
{
    cUIThing* n = (cUIThing*)cList_first((cList*)&mList);
    if (((void*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            n->render3D(pRenCon);
            n = (cUIThing*)((void*)n->mpNext);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_render2D);
#ifdef SKIP_ASM
/**
 * Draws a group's visible children at the group's position plus the given offset.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
void cUIGroup::render2D(float x, float y)
{
    cUIThing* n = (cUIThing*)cList_first((cList*)&mList);
    if (((void*)n) != 0) {
        do {
            if ((n->mAttr >> 6) & 1) {
                n->render2D(mPosition.x + x, mPosition.y + y);
            }
            n = (cUIThing*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIGroup_update);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
extern "C" void cUIThing_update(void* self, unsigned short ev);

/**
 * Updates every object in a UI group for a frame, then the group itself.
 *
 * Calls update (vtable slot 19) on each child in the group's list, then cUIThing_update() on the group.
 *
 * @param ev  frame number (On Tour: FrameNum), passed to every update
 */
void cUIGroup::update(unsigned short ev)
{
    cUIThing* n = (cUIThing*)cList_first((cList*)&mList);
    if (((void*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            n->update(ev);
            n = (cUIThing*)((void*)n->mpNext);
        }
    }
    cUIThing_update(this, ev);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIIcon_setUIData);
#ifdef SKIP_ASM
// PORT: cUIThing_setUIData really takes (self, data) and its result is unused here; the unit declares it with one argument.
void cUIThing_setUIData_8998(void* self, void* data) __asm__("cUIThing_setUIData");

/**
 * Sets an icon up from its screen-data record: clip and alignment bits, its texture element and its size.
 *
 * The record's mIconFlags bit 2 goes to mAttrIcon.mIconFlip and bits 0-1 to mIconRotate; the element is the texture
 * bank's mTextureID entry. The size (mTiledU, mTiledV) is stored in hundredths in the record.
 *
 * @param pObj  the icon's record (On Tour tUIBaseObject* pUIBase)
 */
void cUIIcon::setUIData(tUIBaseObject* pObj)
{
    tUIIcon* data = (tUIIcon*)pObj;
    cUIThing_setUIData_8998(this, data);
    mAttrIcon.mIconFlip = data->mIconFlags >> 2;
    mAttrIcon.mIconRotate = (unsigned char)data->mIconFlags;
    mpTextureElem = (const cTextureElement*)mpScreen->mpState->mpEngine->mTextureBank.getElementByHashname(
        data->mTextureID);
    *(t2Vector*)&mTileUV[0] = t2Vector(data->mTiledU * 0.01f, data->mTiledV * 0.01f);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", SwapFloats__FPvT0);
#ifdef SKIP_ASM
/**
 * Swaps two floats.
 *
 * cUIIcon_render2D uses it to flip a quad's u or v coordinates.
 *
 * @param first  the first float
 * @param second  the second float
 */
void SwapFloats(void* first, void* second)
{
    float saved = *(float*)first;
    *(float*)first = *(float*)second;
    *(float*)second = saved;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIIcon_render2D);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cUIThing_applyPivot(void* self, void* a, void* b);
extern "C" void fSinCos(float* s, float* c, float angle);

struct sV3_8A78 {
    float x, y, z;
    sV3_8A78() {}
    sV3_8A78(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sV3_8A78 operator+(const sV3_8A78& o) const { return sV3_8A78(x + o.x, y + o.y, z + o.z); }
};
struct sV4_8A78 {
    float x, y, z, w;
    sV4_8A78() {}
    sV4_8A78(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
    void Set(float ax, float ay, float az)
    {
        x = ax;
        y = ay;
        z = az;
    }
} __attribute__((aligned(16)));
struct sM33_8A78 {
    float m[3][3];
};
// Texture page entry: id and the uv rectangle.
struct sUITex_8A78 {
    int id;
    char pad4[0xC - 0x4];
    float v0;   // 0xC
    float u0;   // 0x10
    float u1;   // 0x14
    float v1;   // 0x18
};
struct sRS_8A78 {
    int f0;
    int f4;
    int f8;
    int fC;
    short f10;
};
struct sVE_8A78 { short delta; short index; void (*fn)(void*, ...); };
struct cUICtx_8A78 {
    char pad0[0xE84];
    sRS_8A78* top;           // 0xE84
    char padE88[0x10D8 - 0xE88];
    sVE_8A78* vt;            // 0x10D8
};
struct cUIIcon_8A78 {
    char pad0[0x8];
    sVE_8A78* vt;            // 0x8
    char padC[0x15 - 0xC];
    unsigned char f15;       // 0x15
    char pad16[0x1C - 0x16];
    float col[4];            // 0x1C (a, r, g, b)
    sV3_8A78 rot;            // 0x2C
    char pad38[0x44 - 0x38];
    sV3_8A78 pos;            // 0x44
    sV3_8A78 scale;          // 0x50
    char pad5C[0x74 - 0x5C];
    unsigned int flags;      // 0x74
    int texId;               // 0x78
    sUITex_8A78* tex;        // 0x7C
};
extern cUICtx_8A78* GrMan_8a78 __asm__("GrMan");
extern sM33_8A78 t3mat_Identity_8a78 __asm__("t3mat_Identity");

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_8A78(sV4_8A78& a, const sV4_8A78& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// Texture of the current render state (-1 = untextured).
static inline void SetTexture_8A78(cUICtx_8A78* ctx, int id)
{
    ctx->top->f10 = id;
}

// Number of quarter turns applied to the texture coordinates.
static inline int GetTurns_8A78(cUIIcon_8A78* self)
{
    return (self->flags >> 2) & 3;
}

// Pushes a copy of the current render state.
static inline void PushRS_8A78()
{
    cUICtx_8A78* ctx = GrMan_8a78;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void PopRS_8A78()
{
    GrMan_8a78->top--;
}

// m = r * m for a rotation of `angle` about `axis`.
static inline void Rotate_8A78(sM33_8A78& m, const sV3_8A78& axis, float angle)
{
    sM33_8A78 r;
    float s;
    float c;
    fSinCos(&s, &c, angle);
    float t = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
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
    sM33_8A78 a = m;
    m.m[0][0] = a.m[0][0] * r.m[0][0] + a.m[1][0] * r.m[0][1] + a.m[2][0] * r.m[0][2];
    m.m[1][0] = a.m[0][0] * r.m[1][0] + a.m[1][0] * r.m[1][1] + a.m[2][0] * r.m[1][2];
    m.m[2][0] = a.m[0][0] * r.m[2][0] + a.m[1][0] * r.m[2][1] + a.m[2][0] * r.m[2][2];
    m.m[0][1] = a.m[0][1] * r.m[0][0] + a.m[1][1] * r.m[0][1] + a.m[2][1] * r.m[0][2];
    m.m[1][1] = a.m[0][1] * r.m[1][0] + a.m[1][1] * r.m[1][1] + a.m[2][1] * r.m[1][2];
    m.m[2][1] = a.m[0][1] * r.m[2][0] + a.m[1][1] * r.m[2][1] + a.m[2][1] * r.m[2][2];
    m.m[0][2] = a.m[0][2] * r.m[0][0] + a.m[1][2] * r.m[0][1] + a.m[2][2] * r.m[0][2];
    m.m[1][2] = a.m[0][2] * r.m[1][0] + a.m[1][2] * r.m[1][1] + a.m[2][2] * r.m[1][2];
    m.m[2][2] = a.m[0][2] * r.m[2][0] + a.m[1][2] * r.m[2][1] + a.m[2][2] * r.m[2][2];
}

// Transforms the point (v.x, v.y) by the upper two rows of m.
static inline sV4_8A78 Xform_8A78(const sM33_8A78& m, const sV4_8A78& v)
{
    sV4_8A78 r;
    r.x = m.m[0][0] * v.x + m.m[1][0] * v.y;
    r.y = m.m[0][1] * v.x + m.m[1][1] * v.y;
    r.z = m.m[0][2] * v.x + m.m[1][2] * v.y;
    r.w = v.w;
    return r;
}

/**
 * Draws an icon as a textured, tinted quad, flipped, turned and rotated as set, at its position plus the
 * given offset.
 *
 * Uses its texture element, else a whole texture by ID, else no texture. Rotation is about the icon's
 * position.
 *
 * @param x  parent x offset
 * @param y  parent y offset
 */
extern "C" void cUIIcon_render2D(cUIIcon_8A78* self, float x, float y)
{
    tBxVertex verts[4];
    cUICtx_8A78* ctx = GrMan_8a78;
    if (self->tex != 0) {
        SetTexture_8A78(ctx, self->tex->id);
        verts[0].u = self->tex->u0;
        verts[0].v = self->tex->v0;
        verts[1].u = self->tex->u1;
        verts[1].v = self->tex->v0;
        verts[2].u = self->tex->u0;
        verts[2].v = self->tex->v1;
        verts[3].u = self->tex->u1;
        verts[3].v = self->tex->v1;
    } else if (self->texId > 0) {
        SetTexture_8A78(ctx, self->texId);
        verts[0].u = 0.0f;
        verts[0].v = 0.0f;
        verts[1].u = 1.0f;
        verts[1].v = 0.0f;
        verts[2].u = 0.0f;
        verts[2].v = 1.0f;
        verts[3].u = 1.0f;
        verts[3].v = 1.0f;
    } else {
        SetTexture_8A78(ctx, -1);
    }
    if (self->flags & 2) {
        SwapFloats(&verts[0].u, &verts[1].u);
        SwapFloats(&verts[0].v, &verts[1].v);
        SwapFloats(&verts[2].u, &verts[3].u);
        SwapFloats(&verts[2].v, &verts[3].v);
    }
    for (char i = 0; i < GetTurns_8A78(self); i++) {
        float swapUV = verts[0].u;
        verts[0].u = verts[2].u;
        verts[2].u = verts[3].u;
        verts[3].u = verts[1].u;
        verts[1].u = swapUV;
        swapUV = verts[0].v;
        verts[0].v = verts[2].v;
        verts[2].v = verts[3].v;
        verts[3].v = verts[1].v;
        verts[1].v = swapUV;
    }
    sV3_8A78 pos = self->pos;
    sV3_8A78 size;
    sV3_8A78 scale = self->scale;
    self->vt[20].fn(vthis(self, self->vt[20]), &size);
    size.x *= scale.x;
    size.y *= scale.y;
    cUIThing_applyPivot(self, &pos, &size);
    pos.x += x;
    pos.y += y;
    sM33_8A78 rotMat = t3mat_Identity_8a78;
    if (self->rot.x != 0.0f) {
        sV3_8A78 axis(1.0f, 0.0f, 0.0f);
        Rotate_8A78(rotMat, axis, self->rot.x);
    }
    if (self->rot.y != 0.0f) {
        sV3_8A78 axis(0.0f, 1.0f, 0.0f);
        Rotate_8A78(rotMat, axis, self->rot.y);
    }
    if (self->rot.z != 0.0f) {
        sV3_8A78 axis(0.0f, 0.0f, 1.0f);
        Rotate_8A78(rotMat, axis, self->rot.z);
    }
    sV3_8A78 minCorner(pos.x - self->pos.x, pos.y - self->pos.y, pos.z - self->pos.z);
    sV3_8A78 maxCorner;
    {
        sV3_8A78 end = pos + size;
        maxCorner.x = end.x - self->pos.x;
        maxCorner.y = end.y - self->pos.y;
        maxCorner.z = end.z - self->pos.z;
    }
    (*(sV4_8A78*)&verts[0].p) = Xform_8A78(rotMat, sV4_8A78(minCorner.x, minCorner.y, 0.0f, 1.0f));
    (*(sV4_8A78*)&verts[1].p) = Xform_8A78(rotMat, sV4_8A78(maxCorner.x, minCorner.y, 0.0f, 1.0f));
    (*(sV4_8A78*)&verts[2].p) = Xform_8A78(rotMat, sV4_8A78(minCorner.x, maxCorner.y, 0.0f, 1.0f));
    (*(sV4_8A78*)&verts[3].p) = Xform_8A78(rotMat, sV4_8A78(maxCorner.x, maxCorner.y, 0.0f, 1.0f));
    vu0AddEq_8A78((*(sV4_8A78*)&verts[0].p), sV4_8A78(self->pos.x, self->pos.y, self->pos.z, 0.0f));
    vu0AddEq_8A78((*(sV4_8A78*)&verts[1].p), sV4_8A78(self->pos.x, self->pos.y, self->pos.z, 0.0f));
    vu0AddEq_8A78((*(sV4_8A78*)&verts[2].p), sV4_8A78(self->pos.x, self->pos.y, self->pos.z, 0.0f));
    vu0AddEq_8A78((*(sV4_8A78*)&verts[3].p), sV4_8A78(self->pos.x, self->pos.y, self->pos.z, 0.0f));
    for (int i = 0; i < 4; i++) {
        (*(sV4_8A78*)&verts[i].p).Set((*(sV4_8A78*)&verts[i].p).x, (*(sV4_8A78*)&verts[i].p).y, 0.0f);
        verts[i].a = (int)(self->col[0] * 128.0f);
        verts[i].r = (int)(self->col[1] * 128.0f);
        verts[i].g = (int)(self->col[2] * 128.0f);
        verts[i].b = (int)(self->col[3] * 128.0f);
        verts[i].one = 1.0f;
    }
    PushRS_8A78();
    cUICtx_8A78* rc = GrMan_8a78;
    rc->top->f0 |= 0xC;
    int* w = &rc->top->f8;
    *w = (*w & ~0x3E0) | ((((self->f15 & 0x1F) + 9) << 5) & 0x3E0);
    sVE_8A78* vt = rc->vt;
    vt[71].fn(vthis(rc, vt[71]), 4, verts, 0);
    PopRS_8A78();
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiengine", cUIIcon_setTextureByHashValue);
#ifdef SKIP_ASM
/**
 * Points the icon at a texture by name hash: mpTextureElem from the engine's texture bank (engine + 0x58,
 * cUITextureBank::getElementByHashname).
 *
 * The engine is the one of the screen's state. mTextureID is left as it was.
 *
 * @param id  the texture's name hash (On Tour int hashVal)
 */
void cUIIcon::setTextureByHashValue(int id)
{
    cUIScreen* screen = mpScreen;
    cUIState* state = screen->mpState;
    cUIEngine* engine = state->mpEngine;
    mpTextureElem = (const cTextureElement*)engine->mTextureBank.getElementByHashname(id);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_cUIListBox);
#ifdef SKIP_ASM
extern "C" void* cUIThing_cUIThing(void* self);
extern "C" void* memset(void* dst, int c, int n);
extern void* cUIListBox_vtbl[];

extern tARGB gStaticColour;



/**
 * Constructs an empty list box.
 *
 * Runs the cUIThing constructor (cUIThing_cUIThing; the screen and parent, On Tour pState / pParent, are not declared
 * here and stay in $a1/$a2), stores cUIListBox_vtbl, sets mShadow and clears mUnicode and mCallback, clears the 32
 * entries (and memsets them), mNumEntries and mCurrentEntry, and sets mStaticColour from gStaticColour.
 *
 * @return @p self
 */
extern "C" cUIListBox* cUIListBox_cUIListBox(cUIListBox* self)
{
    cUIThing_cUIThing((self));
    VPTR_OF(cListNode, self) = cUIListBox_vtbl;
    *(unsigned char*)&self->mAttrListBox = (*(unsigned char*)&self->mAttrListBox | 4) & 0xC4;
    tListBoxEntry* listBoxEntry = ((&self->mpEntries[0]));
    for (int i = 31; i != -1; i--, listBoxEntry++) {
        listBoxEntry->mStringID = 0;
        listBoxEntry->mpString = 0;
        listBoxEntry->mpAsciiString = 0;
        listBoxEntry->mpData = 0;
        listBoxEntry->mUnicodeBufferSize = 0;
    }
    self->mNumEntries = 0;
    self->mCurrentEntry = 0;
    memset(((&self->mpEntries[0])), 0, 640);
    (*(&self->mStaticColour)) = gStaticColour;
    return (self);
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_setUIData);
#ifdef SKIP_ASM
// PORT: cUIThing_setUIData really takes (self, data); the unit declares it with one argument.
void* cUIThing_setUIData_2(void* self, void* data) __asm__("cUIThing_setUIData");

// A tARGB built from its four components (A, R, G, B).
struct tARGBInit : tARGB {
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};

/**
 * Sets a list box up from its screen record.
 *
 * Runs cUIThing's setUIData, takes the font (mFont) from the record's mFontID, calls slot 23 (cUIListBox_setShadow)
 * with bit 0 of its mFlags and, when mShadow is then set, takes mShadowColour from its mSColour bytes (scaled by
 * 1/255). Always sets mWrap (bit 7 of mAttr).
 *
 * @param pObj  the screen record (On Tour tUIBaseObject* pUIBase)
 */
void cUIListBox::setUIData(tUIBaseObject* pObj)
{
    tUIListBox* data = (tUIListBox*)pObj;
    cUIThing_setUIData_2(this, data);
    mAttrListBox.mFont = data->mFontID & 3;
    // slot 23 (cUIListBox_setShadow) by hand: cUIListBox declares no virtual for it
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)VPTR_OF(cListNode, this);
    vt[23].fn(vthis(this, vt[23]), data->mFlags & 1);
    // hasShadow, read as cUIListBox_hasShadow does: mShadow from the attribute word
    if ((*(int*)&mAttrListBox >> 2) & 1) {
        mShadowColour = tARGBInit(data->mSColour.a * 0.003921568859368563f,
                                        data->mSColour.r * 0.003921568859368563f,
                                        data->mSColour.g * 0.003921568859368563f,
                                        data->mSColour.b * 0.003921568859368563f);
    }
    mAttr |= 0x80;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_getFont);
#ifdef SKIP_ASM
void* cUIThing_getEngine(void*);

struct cUIListBox_getFont_sEntry {
    int a;
    int b;
    int c;
};

/**
 * Returns the list box's font: entry mFont + 1 of the UI engine's font table (engine + 8, 12-byte entries), found
 * through getEngine (cUIThing_getEngine).
 *
 * @return the cFont (as an int), or 0 without an engine
 */
int cUIListBox::getFont()
{
    cUIEngine* engine = (cUIEngine*)cUIThing_getEngine(this);
    if (((void*)engine) != 0) {
        cUIListBox_getFont_sEntry* fonts = (cUIListBox_getFont_sEntry*)engine->mpFontInterface;
        return fonts[(*(unsigned char*)&mAttrListBox & 3) + 1].a;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_onInput);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void* cUIThing_getEngine(void* self);
extern "C" int cUIThing_getSound(void* self, int a);
extern char gUISoundNull[4];

struct sVE_9970 { short delta; short index; int (*fn)(void*, ...); };

static inline int inputIs_9970(const void* in, int slot)
{
    sVE_9970* vt = *(sVE_9970**)((const char*)in + 8);
    return vt[slot].fn(vthis((void*)in, vt[slot]));
}

static inline void focusNotify_9970(char* self, int what)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(o + 8);
    vt[19].fn(vthis(o, vt[19]), self, what);
}

static inline int focusMove_9970(char* self, int dir)
{
    char* o = *(char**)(*(char**)(self + 0x5C) + 0xD0);
    sVE_9970* vt = *(sVE_9970**)(o + 8);
    return vt[22].fn(vthis(o, vt[22]), self, dir);
}

static inline int isClear_9970(int flags)
{
    return (flags & 1) == 0;
}

/**
 * Moves the list box's current entry on left / right (or down / up when vertical) and reports it to the state.
 *
 * Does nothing without entries. Left (horizontal) or down (vertical, mOrientation set) selects the previous entry,
 * right or up the next, wrapping when mWrap is set; a change is reported to the screen's state as widget event 9
 * (onWidgetEvent). The state's onWidgetListBox then gets the direction (0 / 3 back, 1 / 2 forward): result bit 0x100
 * plays keyframer event 8 or 9, bit 1 gives 1 and bit 0x10 the widget's mSound[0]. Previous (back) reports event 6
 * and direction 4 (keyframer event 3; bit 1 gives mSound[1]). The result goes to slot 2 of the engine's +0x14
 * listener (gUISoundNull when none).
 *
 * @param in  the input (On Tour const cUIInput* pInput)
 * @return true when the input was used, else false (On Tour returns bool)
 */
bool cUIListBox::onInput(const cUIInput* in)
{
    if (mNumEntries == 0)
        return false;
    int result = 0;
    char* listener = gUISoundNull;
    cUIEngine* owner = (cUIEngine*)((char*)cUIThing_getEngine(((char*)this)));
    if ((char*)owner->mpSound != 0) {
        listener = (char*)owner->mpSound;
    }
    int moveResult;
    if ((inputIs_9970(in, 19) && isClear_9970(mAttr))
        || (inputIs_9970(in, 18) && (mAttr & 1))) {
        int cur = mCurrentEntry;
        if (cur != 0) {
            mCurrentEntry = cur - 1;
            focusNotify_9970(((char*)this), 9);
        } else {
            int wrapBits = mAttr >> 7;
            if (wrapBits & 1) {
                mCurrentEntry = mNumEntries - 1;
                focusNotify_9970(((char*)this), 9);
            }
        }
        if (isClear_9970(mAttr))
            moveResult = focusMove_9970(((char*)this), 0);
        else
            moveResult = focusMove_9970(((char*)this), 3);
        if (moveResult & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)this, 8);
            if (ev != cUIScreen::kNoFrame && mpScreen != 0) {
                cUIScreen_playFrame(mpScreen, ev, 1);
            }
        }
        if (moveResult & 1) {
            result = 1;
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(((char*)this), 0);
        }
        if (listener != 0) {
            sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)listener;
            vt[2].fn(vthis(listener, vt[2]), result);
        }
        return true;
    } else if ((inputIs_9970(in, 20) && isClear_9970(mAttr))
               || (inputIs_9970(in, 17) && (mAttr & 1))) {
        int next = mCurrentEntry + 1;
        if (next < mNumEntries) {
            mCurrentEntry = next;
            focusNotify_9970(((char*)this), 9);
        } else {
            int wrapBits = mAttr >> 7;
            if (wrapBits & 1) {
                mCurrentEntry = 0;
                focusNotify_9970(((char*)this), 9);
            }
        }
        if (isClear_9970(mAttr))
            moveResult = focusMove_9970(((char*)this), 1);
        else
            moveResult = focusMove_9970(((char*)this), 2);
        if (moveResult & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)this, 9);
            if (ev != cUIScreen::kNoFrame && mpScreen != 0) {
                cUIScreen_playFrame(mpScreen, ev, 1);
            }
        }
        if (moveResult & 1) {
            result = 1;
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(((char*)this), 0);
        }
        if (listener != 0) {
            sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)listener;
            vt[2].fn(vthis(listener, vt[2]), result);
        }
        return true;
    } else if (inputIs_9970(in, 6)) {
        focusNotify_9970(((char*)this), 6);
        moveResult = focusMove_9970(((char*)this), 4);
        if (moveResult & 0x100) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)this, 3);
            if (ev != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, ev, 1);
            }
        }
        if (moveResult & 1) {
            result = cUIThing_getSound(((char*)this), 1);
        } else if (moveResult & 0x10) {
            result = cUIThing_getSound(((char*)this), 0);
        }
        if (listener != 0) {
            sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)listener;
            vt[2].fn(vthis(listener, vt[2]), result);
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_setUnicodePrivate);
#ifdef SKIP_ASM
void cMemMan_free(void*);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int USTR_length(unsigned short* s);
extern "C" void USTR_copy(unsigned short* dst, unsigned short* src);
extern char str_UILISTBOXSTR[];

struct cUIListBox_setUnicodePrivate_sEntry {
    unsigned short* str;
    int pad[4];
};

/**
 * Replaces the Unicode text of entry @p idx with a heap copy of @p str (tag str_UILISTBOXSTR "UILISTBOXSTR"), freeing the
 * old copy.
 *
 * Does nothing unless the UI engine (mpScreen->mpState->mpEngine) has its string table at +0x10.
 *
 * @param idx  the entry index (On Tour Index); not range-checked
 * @param str  the UTF-16 text (On Tour pUnicode)
 */
void cUIListBox::setUnicodePrivate(unsigned char idx, unsigned short* str)
{
    cUIScreen* screen = mpScreen;
    cUIState* state = screen->mpState;
    cUIEngine* engine = state->mpEngine;
    if (engine->mpLocaleInterface != 0) {
        cUIListBox_setUnicodePrivate_sEntry* tbl = (cUIListBox_setUnicodePrivate_sEntry*)(&mpEntries[0].mpString);
        cUIListBox_setUnicodePrivate_sEntry* entry = &tbl[idx];
        if (entry->str) {
            cMemMan_free(entry->str);
        }
        unsigned short* tempString = (unsigned short*)operator_new_tag((USTR_length(str) + 1) * 2, str_UILISTBOXSTR, 0x100, 0);
        USTR_copy(tempString, str);
        entry->str = tempString;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiengine", cUIListBox_updateLocale);
#ifdef SKIP_ASM
extern "C" void cUIListBox_setUnicodePrivate(cUIListBox* self, unsigned char idx, unsigned short* str);

/**
 * Re-reads every entry's localised text after a language change.
 *
 * For each entry with a string id (mStringID), looks the text up in the engine's locale (getStringByHash, slot 4) and
 * stores it (cUIListBox_setUnicodePrivate); then sets the box cached (slot 5, setCached(1)). Does nothing without a
 * locale.
 */
void cUIListBox::updateLocale()
{
    cUIScreen* screen = mpScreen;
    cUIState* state = screen->mpState;
    cUIEngine* engine = state->mpEngine;
    cUILocaleInterface* loc = engine->mpLocaleInterface;
    if (loc != 0) {
        for (unsigned char i = 0; i < mNumEntries; i++) {
            int id = mpEntries[i].mStringID;
            if (id != 0) {
                unsigned short* unicodeStr = loc->getStringByHash(id);
                if (unicodeStr != 0) {
                    setUnicodePrivate(i, unicodeStr);
                }
            }
        }
        setCached(1);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiengine", cUIListBox_render2D);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" float cFont_textExtent1(void* font, int str, void* out, int flags, float sx, float sy);
extern "C" void cUIThing_applyPivot(void* self, void* pos, void* size);
extern "C" void cFont_text1(void* font, float x, float y, int str);
extern char* GrMan_9F00 __asm__("GrMan");

struct sV3_9F00 {
    float x, y, z;
    sV3_9F00() {}
    sV3_9F00(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};
struct sV4_9F00 {
    float x, y, z, w;
    sV4_9F00() {}
    sV4_9F00(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sRect_9F00 {
    float x0, y0, x1, y1;
};


struct sLayerBits_9F00 {
    unsigned char lo;
    unsigned char layer : 5;
};

struct sStrEntry_9F00 {
    int str;
    int pad[4];
};


struct sVert_9F00 {
    float u, v, q;
    int pad;
    int r, g, b, a;
    sV4_9F00 pos;
};

struct sVE_9F00 { short delta; short index; void (*fn)(void*, int, void*, int); };

static inline void setLayer_9F00(cUIListBox* self)
{
    int* d = (int*)(*(char**)(GrMan_9F00 + 0xE84) + 8);
    *d = (*d & ~0x3E0) | (((((sLayerBits_9F00*)&self->mAttr)->layer + 9) << 5) & 0x3E0);
}

/**
 * Draws the list box's current entry (entry 0 when mCallback is set) at its position plus (@p x, @p y).
 *
 * Returns when the box has no font or no entries. Uses the font at mScale, mStaticColour when both mStatic and
 * mUseStaticColour are set (else mColour) and, with mShadow, mShadowColour (its first component from mColour.A) at
 * offset (2, 2). When the locale's bit 26 is set (a debug outline), also draws mSize as a red outline, or as a
 * translucent red box when the text is wider than mSize.x.
 *
 * @param x  the parent's x offset
 * @param y  the parent's y offset
 */
void cUIListBox::render2D(float x, float y)
{
    cFont* font = (cFont*)getFont();
    if (font == 0 || mNumEntries == 0) {
        return;
    }
    t2Vector listScale(mScale.x, mScale.y);
    (*(t2Vector*)&font->Scale[0]).x = (*(t2Vector*)&font->BaseScale[0]).x * listScale.x;
    (*(t2Vector*)&font->Scale[0]).y = (*(t2Vector*)&font->BaseScale[0]).y * listScale.y;
    if (((mAttr >> 5) & 1) && ((*(int*)&mAttrListBox >> 5) & 1)) {
        (*(tARGB*)&font->Color[0]) = (*(&mStaticColour));
    } else {
        (*(tARGB*)&font->Color[0]) = (*(&mColour));
    }
    if ((*(int*)&mAttrListBox >> 2) & 1) {
        tARGB shadow = (*(&mShadowColour));
        shadow.A = mColour.A;
        (*(tARGB*)&font->ShadowColor[0]) = shadow;
        (*(t2Vector*)&font->ShadowOffset[0]) = t2Vector(2.0f, 2.0f);
    } else {
        (*(t2Vector*)&font->ShadowOffset[0]) = t2Vector(0.0f, 0.0f);
    }
    int str;
    if (*(int*)&mAttrListBox & 0x10) {
        str = ((sStrEntry_9F00*)&mpEntries[0].mpString)[0].str;
    } else {
        str = ((sStrEntry_9F00*)&mpEntries[0].mpString)[mCurrentEntry].str;
    }
    setLayer_9F00((this));
    int over = 0;
    float* box = &mSize.x;
    {
        sRect_9F00 textRect;
        cFont_textExtent1(font, str, &textRect, 0, listScale.x, listScale.y);
        if (mSize.x < textRect.x1) {
            over = 1;
        }
        sV3_9F00 size(textRect.x1, mScale.y * ((float)font->MaxCharHeight * (*(t2Vector*)&font->BaseScale[0]).y), 0.0f);
        sV3_9F00 position(mPosition.x + x, mPosition.y + y, 0.0f);
        cUIThing_applyPivot((this), &position, &size);
        cFont_text1(font, position.x, position.y, str);
    }
    char* loc = *(char**)(*(char**)(*(char**)((char*)mpScreen + 0xD0) + 0x10) + 0x10);
    if (loc == 0) {
        return;
    }
    if ((*(int*)loc >> 26) & 1) {
        sV3_9F00 position = *(sV3_9F00*)&mPosition;
        position.x += x;
        position.y += y;
        cUIThing_applyPivot((this), &position, box);
        setLayer_9F00((this));
        sVert_9F00 verts[5];
        unsigned int i;
        if (!over) {
            for (i = 0; i < 5; i++) {
                verts[i].q = 1.0f;
                verts[i].u = 0.0f;
                verts[i].v = 0.0f;
                verts[i].a = 0xFF;
                verts[i].r = 0xFF;
                verts[i].g = 0;
                verts[i].b = 0;
                sV4_9F00 vertexPos;
                vertexPos.x = position.x;
                vertexPos.y = position.y;
                vertexPos.z = position.z;
                vertexPos.w = 1.0f;
                verts[i].pos = vertexPos;
            }
            verts[1].pos.x += mSize.x;
            verts[2].pos.x += mSize.x;
            verts[2].pos.y += mSize.y;
            verts[3].pos.y += mSize.y;
            sVE_9F00* vt = *(sVE_9F00**)(GrMan_9F00 + 0x10D8);
            vt[73].fn(vthis(GrMan_9F00, vt[73]), 5, verts, 0);
        } else {
            *(short*)(*(char**)(GrMan_9F00 + 0xE84) + 0x10) = -1;
            for (i = 0; i < 4; i++) {
                verts[i].q = 1.0f;
                verts[i].u = 0.0f;
                verts[i].v = 0.0f;
                verts[i].a = 0x40;
                verts[i].r = 0x80;
                verts[i].g = 0;
                verts[i].b = 0;
                sV4_9F00 vertexPos;
                vertexPos.x = position.x;
                vertexPos.y = position.y;
                vertexPos.z = position.z;
                vertexPos.w = 1.0f;
                verts[i].pos = vertexPos;
            }
            verts[1].pos.x += mSize.x;
            verts[2].pos.y += mSize.y;
            verts[3].pos.x += mSize.x;
            verts[3].pos.y += mSize.y;
            sVE_9F00* vt = *(sVE_9F00**)(GrMan_9F00 + 0x10D8);
            vt[71].fn(vthis(GrMan_9F00, vt[71]), 4, verts, 0);
        }
    }
}
#endif

