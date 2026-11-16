#include "common.h"
#include "render/ps2bezierman.h"

extern "C" void func_00374C90(void* self);
extern "C" void func_00372B30(void* self);
extern "C" void cPS2BezierMan_Init(void* self);
extern void* cPS2BezierMan_vtbl[16];

//100%
INCLUDE_ASM("render/ps2bezierman", cPS2BezierMan_cPS2BezierMan__FPv);
#ifdef SKIP_ASM
// PORT: func_00374C90 (the base constructor) returns this; the unit declares it void.
extern "C" void* func_00374C90_r(void* self) __asm__("func_00374C90");
inline void* operator new[](unsigned int, void* p) { return p; }

// Element type of an inline-constructed array; its empty constructor leaves only the loop counters.
struct sBezSlot_8AE0 {
    void* p;
    sBezSlot_8AE0() {}
};

/**
 * Constructs the Bezier patch manager and allocates its VIF caches.
 *
 * The base constructor func_00374C90() registers the object as the current manager (cBezierMan_TheBezierMan); then it stores
 * cPS2BezierMan's vtable, builds the vertex tables at offset 0x10, and cPS2BezierMan_Init() allocates the "4x4VifCache",
 * "6x6VifCache" and "8x8VifCache" buffers, sets the distance settings and fills the 6x6 and 8x8 UV grids.
 *
 * @return @p self
 */
void* cPS2BezierMan_cPS2BezierMan(void* self)
{
    func_00374C90_r(self);
    *(void**)((char*)self + 0x4) = cPS2BezierMan_vtbl;
    func_00372B30((char*)self + 0x10);
    new ((char*)self + 0x8) sBezSlot_8AE0[2];
    cPS2BezierMan_Init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("render/ps2bezierman", cPS2BezierMan__cPS2BezierMan);
#ifdef SKIP_ASM
extern "C" void cPS2BezierMan_Purge(void* self);
extern "C" void func_00374CA8(void* self, int flags);

/**
 * Destroys the Bezier patch manager: stores cPS2BezierMan_vtbl, runs Purge and chains to the base destructor
 * func_00374CA8 with @p flags.
 *
 * @param flags  destructor flags passed to the base destructor
 */
extern "C" void cPS2BezierMan__cPS2BezierMan(cPS2BezierMan* self, int flags)
{
    self->mVtbl = cPS2BezierMan_vtbl;
    cPS2BezierMan_Purge(self);
    func_00374CA8(self, flags);
}
#endif

