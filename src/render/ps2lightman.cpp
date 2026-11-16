#include "common.h"
#include "render/ps2lightman.h"

extern void* cLightMan_vtbl[16];
extern void* cLightMan_TheLightMan;

struct cPSPLightMan {
    void* vtable;
};

//99.83%
INCLUDE_ASM("render/ps2lightman", cPSPLightMan_cPSPLightMan__FP12cPSPLightMan);
#ifdef SKIP_ASM
/**
 * Constructs the light manager and registers it as the global one (cLightMan_TheLightMan, returned by func_00394C40()).
 *
 * Only the vtable is stored; the class keeps On Tour's PSP name.
 *
 * @return @p self
 */
cPSPLightMan* cPSPLightMan_cPSPLightMan(cPSPLightMan* self)
{
    self->vtable = cLightMan_vtbl;
    cLightMan_TheLightMan = self;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/ps2lightman", cLightMan__cLightMan);
#ifdef SKIP_ASM
extern void* cLightMan_TheLightMan;
extern void* cLightMan_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys the light manager base: unregisters the global light manager and frees it if asked.
 *
 * Stores the abstract cLightMan vtable cLightMan_vtbl, clears cLightMan_TheLightMan (On Tour first asserts TheLightMan ==
 * this) and calls operator_delete on @p flags bit 0. cPS2LightMan__cPS2LightMan chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cLightMan__cLightMan(cPS2LightMan* self, int flags)
{
    *(void***)self = cLightMan_vtbl;
    cLightMan_TheLightMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

