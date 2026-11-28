#include "common.h"
#include "vtable.h"
#include "fe/fereal.h"
#include "util/locale.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("fe/festatecharequip", cFE_cFE);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cFadeScreen_cFadeScreen(void* self);
extern "C" void* cSimpleCamera_cSimpleCamera(void* self);
extern "C" void* __15cFERiderManager(void* self);
extern "C" void* __18cFEBoltFileManager(void* self);
// PORT: called with an extra int in $a1 here; cSimpleCamera_init's body is (self, x, y, z). Bound by asm label.
void cSimpleCamera_init_i(void* self, int a1, float x, float y, float z) __asm__("cSimpleCamera_init");
extern void* cFE_vtbl[];
extern void* cFECamera_vtbl[];
extern char str_UIEngine[];
extern char str_FXLoader_FE[];

// Constructors bound to their unmangled symbols (g++ 2.95 ctors return this).
class cFEObjK1956F8_70 {
public:
    char pad_0x000[0x70];
    cFEObjK1956F8_70() __asm__("cUIEngine_cUIEngine");
};
class cFEObjK1956F8_1 {
public:
    cFEObjK1956F8_1(int a) __asm__("cFXLoader_cFXLoader");
};

/**
 * Constructs the front end: its camera, rider manager, asynchronous files, UI engine and effects loader.
 *
 * Runs the base constructor cFadeScreen_cFadeScreen, stores the vtable cFE_vtbl, builds the camera at +0x10 (vtable
 * at +0xA0) and sets it up with cSimpleCamera_init (pi/4, 5, 5000), the rider manager at +0xB0 (On Tour
 * mRiderManager) and the object at +0x1A70, and allocates the UI engine ("UIEngine", +0xC) and an effects
 * loader ("FXLoader FE", +0xB5AA0). A C function: as cFE::cFE(), g++ would build the rider manager (a member with a
 * constructor) before the camera.
 *
 * @return @p self
 */
extern "C" void* cFE_cFE(cFE* self)
{
    cFadeScreen_cFadeScreen(self);
    VPTR_OF(cAppModule, self) = cFE_vtbl;
    cSimpleCamera_cSimpleCamera(&self->mCamera);
    self->mCamera.mVtbl = cFECamera_vtbl;
    __15cFERiderManager(&self->mRiderManager);
    __18cFEBoltFileManager(self->mBoltFileManager);
    self->mpUI = (cUIEngine*)(new (str_UIEngine, 0, 0) cFEObjK1956F8_70);
    cSimpleCamera_init_i(&self->mCamera, 0, 0.7853981852531433f, 5.0f, 5000.0f);
    self->mFXLoader = (cFXLoader*)(new (str_FXLoader_FE, 0, 0) cFEObjK1956F8_1(1));
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", _$_3cFE);
#ifdef SKIP_ASM
extern "C" void cUIEngine__cUIEngine(void* p, int flags);
extern "C" void cGraphicsMan_PurgeTextures(void* p);
extern "C" void cFXLoader__cFXLoader(void* p, int flags);
extern "C" void _$_18cFEBoltFileManager(void* p, int flags);
extern "C" void cFERiderManager__cFERiderManager(void* p, int flags);
extern void* cSimpleCamera_vtbl[];

/**
 * Destroys the front-end module's base part.
 *
 * Deletes the UI engine (cUIEngine__cUIEngine with 3) when present, purges the graphics manager's textures, deletes
 * the FX loader when present, then destroys the bolt file manager (cFEBoltFileManager::~cFEBoltFileManager) and the
 * rider manager (cFERiderManager__cFERiderManager) and puts cSimpleCamera's vtable back in the camera; the base
 * destructors (cFadeScreen, cAppModule) follow inline. cRealFE's destructor calls it as its base destructor.
 */
cFE::~cFE()
{
    if (mpUI != 0) {
        cUIEngine__cUIEngine(mpUI, 3);
    }
    cGraphicsMan_PurgeTextures(GrMan);
    if (mFXLoader != 0) {
        cFXLoader__cFXLoader(mFXLoader, 3);
    }
    _$_18cFEBoltFileManager(mBoltFileManager, 2);
    cFERiderManager__cFERiderManager(&mRiderManager, 2);
    mCamera.mVtbl = cSimpleCamera_vtbl;
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", cFELocale_cFELocale);
#ifdef SKIP_ASM
extern void* cFELocale_vtbl[];
extern void* cUILocaleInterface_vtbl[];
extern "C" void* tAttrLocale_tAttrLocale(void* p);
extern "C" void memset(void* dst, int value, int size);

/**
 * Constructs the front-end locale with no string files loaded.
 *
 * Zeroes the base's first three bytes, stores the base vtable, clears the low 5 bits of byte 3
 * (tAttrLocale_tAttrLocale) and byte 0x58, then stores the cFELocale vtable, sets byte 2 to 4 and zeroes the 20 file
 * slots at +0x8.
 *
 * @return @p self
 */
extern "C" void* cFELocale_cFELocale(cFELocale* self)
{
    self->mDisplayMode = 0;
    self->mLanguage = 0;
    self->mNumLanguages = 0;
    VPTR_OF(cUILocaleInterface, self) = cUILocaleInterface_vtbl;
    tAttrLocale_tAttrLocale((char*)&self->mAttrLocale);
    self->mModule = 0;
    VPTR_OF(cUILocaleInterface, self) = cFELocale_vtbl;
    self->mNumLanguages = 4;
    memset((char*)&self->mpLocaleFile, 0, 80);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", _$_9cFELocale);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteAllFiles(cFELocale* self);

/**
 * Destroys the front-end locale: deletes all its string files (deleteAllFiles); the cUILocaleInterface part does the
 * rest and frees it when asked.
 */
cFELocale::~cFELocale()
{
    deleteAllFiles();
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", cFELocale_deleteAllFiles);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteFile(void* self, int i);

/**
 * Deletes all 20 string files of the front-end locale (cFELocale_deleteFile for each index).
 */
void cFELocale::deleteAllFiles()
{
    signed char i;
    for (i = 0; i < 20; i++) {
        cFELocale_deleteFile(this, i);
    }
}
#endif

