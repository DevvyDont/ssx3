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
extern "C" void* cFERiderManager_cFERiderManager(void* self);
extern "C" void* func_0019CDF0(void* self);
// PORT: called with an extra int in $a1 here; cSimpleCamera_init's body is (self, x, y, z). Bound by asm label.
void cSimpleCamera_init_i(void* self, int a1, float x, float y, float z) __asm__("cSimpleCamera_init");
extern void* cFE_vtbl[];
extern void* D_0046D8D8[];
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
 * loader ("FXLoader FE", +0xB5AA0).
 *
 * @return @p self
 */
extern "C" void* cFE_cFE(cFE* self)
{
    cFadeScreen_cFadeScreen(self);
    self->mVtbl = cFE_vtbl;
    cSimpleCamera_cSimpleCamera(&self->mCamera);
    self->mCamera.mVtbl = D_0046D8D8;
    cFERiderManager_cFERiderManager(&self->mRiderManager);
    func_0019CDF0(self->mBoltFileManager);
    self->mpUI = (cUIEngine*)(new (str_UIEngine, 0, 0) cFEObjK1956F8_70);
    cSimpleCamera_init_i(&self->mCamera, 0, 0.7853981852531433f, 5.0f, 5000.0f);
    self->mFXLoader = (cFXLoader*)(new (str_FXLoader_FE, 0, 0) cFEObjK1956F8_1(1));
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", cFE__cFE);
#ifdef SKIP_ASM
void operator_delete(int* p);
extern "C" void cUIEngine__cUIEngine(void* p, int flags);
extern "C" void cGraphicsMan_PurgeTextures(void* p);
extern "C" void cFXLoader__cFXLoader(void* p, int flags);
extern "C" void func_0019CE20(void* p, int flags);
extern "C" void cFERiderManager__cFERiderManager(void* p, int flags);
extern void* cFE_vtbl[];
extern void* cAppModule_vtbl[];
extern void* cSimpleCamera_vtbl[];

/**
 * Destroys the front-end module's base part, freeing it when asked.
 *
 * Stores the vtable cFE_vtbl, destroys the UI engine (+0xC, cUIEngine__cUIEngine with 3), purges the graphics manager's
 * textures, destroys the FX loader (+0xB5AA0) when present, then the file manager (+0x1A70, func_0019CE20) and the
 * rider manager (+0xB0, cFERiderManager__cFERiderManager), resets the camera's (+0x10) vtable word (+0xA0) to cSimpleCamera_vtbl and
 * stores the base vtable cAppModule_vtbl. Calls operator_delete when bit 0 of @p flags is set. cRealFE__cRealFE calls it as
 * its base destructor.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFE__cFE(cFE* self, int flags)
{
    self->mVtbl = cFE_vtbl;
    if (self->mpUI != 0) {
        cUIEngine__cUIEngine(self->mpUI, 3);
    }
    cGraphicsMan_PurgeTextures(GrMan);
    if (self->mFXLoader != 0) {
        cFXLoader__cFXLoader(self->mFXLoader, 3);
    }
    func_0019CE20(self->mBoltFileManager, 2);
    cFERiderManager__cFERiderManager(&self->mRiderManager, 2);
    self->mCamera.mVtbl = cSimpleCamera_vtbl;
    self->mVtbl = cAppModule_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
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
    *(char*)&self->mDisplayMode = 0;
    *(char*)&self->mLanguage = 0;
    *(char*)&self->mNumLanguages = 0;
    vptr_at(self, 0x4) = cUILocaleInterface_vtbl;
    tAttrLocale_tAttrLocale((char*)&self->mAttrLocale);
    *(char*)&self->mModule = 0;
    vptr_at(self, 0x4) = cFELocale_vtbl;
    *(char*)&self->mNumLanguages = 4;
    memset((char*)&self->mpLocaleFile, 0, 80);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", cFELocale__cFELocale);
#ifdef SKIP_ASM
extern void* cFELocale_vtbl[];
extern void* cUILocaleInterface_vtbl[];
extern "C" void cFELocale_deleteAllFiles(cFELocale* self);
void operator_delete(int* p);

/**
 * Destroys the front-end locale, freeing it when asked.
 *
 * Stores the cFELocale vtable cFELocale_vtbl at +0x4, deletes all its string files (cFELocale_deleteAllFiles), stores the base vtable
 * cUILocaleInterface_vtbl and calls operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cFELocale__cFELocale(cFELocale* self, int flags)
{
    vptr_at(self, 0x4) = cFELocale_vtbl;
    cFELocale_deleteAllFiles(self);
    vptr_at(self, 0x4) = cUILocaleInterface_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatecharequip", cFELocale_deleteAllFiles);
#ifdef SKIP_ASM
extern "C" void cFELocale_deleteFile(void* self, int i);

/**
 * Deletes all 20 string files of the front-end locale (cFELocale_deleteFile for each index).
 */
extern "C" void cFELocale_deleteAllFiles(cFELocale* self)
{
    signed char i;
    for (i = 0; i < 20; i++) {
        cFELocale_deleteFile(self, i);
    }
}
#endif

