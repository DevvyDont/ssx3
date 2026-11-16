#include "common.h"
#include "render/ps2bezierman.h"

extern "C" void func_00374C90(void* self);
extern "C" void func_00372B30(void* self);
extern "C" void func_0038AF30(void* self);
extern void* cPSPBezierMan_vtbl[16];

//100%
INCLUDE_ASM("render/ps2bezierman", cPSPBezierMan_cPSPBezierMan__FPv);
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
 * The base constructor func_00374C90() registers the object as the current manager (D_004A4248); then it stores
 * cPSPBezierMan's vtable, builds the vertex tables at offset 0x10, and func_0038AF30() allocates the "4x4VifCache",
 * "6x6VifCache" and "8x8VifCache" buffers, sets the distance settings and fills the 6x6 and 8x8 UV grids.
 *
 * @return @p self
 */
void* cPSPBezierMan_cPSPBezierMan(void* self)
{
    func_00374C90_r(self);
    *(void**)((char*)self + 0x4) = cPSPBezierMan_vtbl;
    func_00372B30((char*)self + 0x10);
    new ((char*)self + 0x8) sBezSlot_8AE0[2];
    func_0038AF30(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("render/ps2bezierman", func_0038AEB0);
#ifdef SKIP_ASM
extern "C" void func_0038B0F8(void* self);
extern "C" void func_00374CA8(void* self, int flags);

extern "C" void func_0038AEB0(cPSPBezierMan* self, int flags)
{
    self->mVtbl = cPSPBezierMan_vtbl;
    func_0038B0F8(self);
    func_00374CA8(self, flags);
}
#endif

