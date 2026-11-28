#include "common.h"
#include "render/lightman.h"
#include "render/ps2lightman.h"

extern void* cLightMan_vtbl[16];

// On Tour's intermediate class between cLightMan and cPS2LightMan (render/ps2lightman.h); retail keeps its name only
// in the constructor's symbol. No members of its own here.
struct cPSPLightMan : cLightMan {
};

//99.83%
INCLUDE_ASM("render/ps2lightman", cLightMan_cLightMan__FP12cPSPLightMan);
#ifdef SKIP_ASM
/**
 * The cLightMan base constructor: stores the abstract base vtable (cLightMan_vtbl) and registers the object as the
 * global light manager (cLightMan_TheLightMan, returned by cLightMan_getTheLightMan()). cLightMan_construct__Fv then installs
 * the PS2 vtable. (It was once named after On Tour's cPSPLightMan constructor; the parameter type keeps that name.)
 *
 * @return @p self
 */
cPSPLightMan* cLightMan_cLightMan(cPSPLightMan* self)
{
    self->mVtbl = cLightMan_vtbl;
    cLightMan_TheLightMan = self;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/ps2lightman", cLightMan__cLightMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys the light manager base: unregisters the global light manager and frees it if asked.
 *
 * Stores the abstract cLightMan vtable cLightMan_vtbl, clears cLightMan_TheLightMan (On Tour first asserts TheLightMan ==
 * this) and calls operator_delete on @p flags bit 0. cPS2LightMan__cPS2LightMan chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cLightMan__cLightMan(cLightMan* self, int flags)
{
    self->mVtbl = cLightMan_vtbl;
    cLightMan_TheLightMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

