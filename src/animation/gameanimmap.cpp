#include "common.h"
#include "animation/rideranimbase.h"

//100%
INCLUDE_ASM("animation/gameanimmap", cGameAnimMap_testResolve__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing: the animation-map resolve test has an empty body.
 */
void cGameAnimMap_testResolve()
{
}
#endif

//100%
INCLUDE_ASM("animation/gameanimmap", func_003150B8);
#ifdef SKIP_ASM
/**
 * Returns @p self; any further arguments (it is variadic) are spilled to the stack and ignored.
 *
 * Only the register save area of a varargs function remains, so this is probably a compiled-out debug helper. No
 * callers or data references to it were found.
 *
 * @return @p self
 */
extern "C" void* func_003150B8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/gameanimmap", func_003150F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_004FE420[4];
extern float D_004FE430[4];
extern float D_004FE440[4];
extern float D_004FE450[4];
extern float D_004FE460[4];
extern float D_004FE470[4];
extern float D_004FE480[4];
extern float D_004FE490[4];
extern float D_004FE4A0[4];
extern float D_004FE4B0[4];
extern float D_004FE4C0[4];
extern float D_004FE4D0[4];
extern float D_004FE4E0[4];
extern float D_004FE4F0[4];
extern float D_004FE500[4];
extern float D_004FE510[4];
extern float D_004FE520[4];
extern float D_004FE530[4];
extern float D_004FE540[4];
extern float D_004FE550[4];
extern float D_004FE560[4];
extern float D_004FE570[4];
extern float D_004FE580[4];
extern float D_004FE590[4];
extern float D_004FE5A0[4];
extern float D_004FE5B0[4];
extern float D_004FE5C0[4];
extern float D_004FE5D0[4];
extern float D_004FE5E0[4];
extern float D_004FE5F0[4];
extern float D_004FE600[4];
extern float D_004FE610[4];
extern float D_004FE620[4];
extern float D_004FE630[4];
extern float D_004FE640[4];
extern float D_004FE650[4];
extern float D_004FE660[4];
extern float D_004FE670[4];
extern float D_004FE680[4];
extern float D_004FE690[4];
extern float D_004FE6A0[4];
extern float D_004FE6B0[4];
extern float D_004FE6C0[4];
extern float D_004FE6D0[4];
extern float D_004FE6E0[4];
extern float D_004FE6F0[4];
extern float D_004FE700[4];
extern float D_004FE710[4];
extern float D_004FE720[4];
extern float D_004FE730[4];
extern float D_004FE740[4];
extern float D_004FE750[4];
extern float D_004FE760[4];
extern float D_004FE770[4];
extern float D_004FE780[4];
extern float D_004FE790[4];
extern int kAnimIDInvalid;
void sti_003150F8(int init, int prio) __asm__("func_003150F8");
/**
 * Static initializer of this unit's first group of globals: sets kAnimIDInvalid to -1 and fills 56 float tables.
 *
 * Runs only for @p init nonzero and @p prio 0xFFFF (g++'s static initialization and destruction routine; called by
 * func_00315A00 with 1, 0xFFFF). kAnimIDInvalid gets all bits set (its low byte first, then the upper 24 bits); the
 * tables D_004FE420 .. D_004FE790 (four floats each) get constant values.
 *
 * @param init  nonzero to initialize (0 would be destruction; nothing is done then)
 * @param prio  the initialization priority; only 0xFFFF (the default) does anything
 */
void sti_003150F8(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        *(char*)&kAnimIDInvalid = -1;
        kAnimIDInvalid = (kAnimIDInvalid & 0xFF) | 0xFFFFFF00u;
        D_004FE420[0] = 1.0f;
        D_004FE420[1] = 1.0f;
        D_004FE420[2] = 1.0f;
        D_004FE420[3] = 0.5f;
        D_004FE430[0] = 0.4998680055141449f;
        D_004FE430[1] = 1.0f;
        D_004FE430[2] = 1.0f;
        D_004FE430[3] = 0.0f;
        D_004FE440[0] = 0.12951700389385223f;
        D_004FE440[1] = 1.0f;
        D_004FE440[2] = 1.0f;
        D_004FE440[3] = 0.0f;
        D_004FE450[0] = 0.22426900267601013f;
        D_004FE450[1] = 0.9402310252189636f;
        D_004FE450[2] = 0.5425059795379639f;
        D_004FE450[3] = 0.0004030000127386302f;
        D_004FE460[0] = 0.4652079939842224f;
        D_004FE460[1] = 0.32170501351356506f;
        D_004FE460[2] = 0.9943439960479736f;
        D_004FE460[3] = 0.0f;
        D_004FE470[0] = 0.17792600393295288f;
        D_004FE470[1] = 1.0f;
        D_004FE470[2] = 1.0f;
        D_004FE470[3] = 0.0f;
        D_004FE480[0] = 0.4411740005016327f;
        D_004FE480[1] = 0.41388601064682007f;
        D_004FE480[2] = 0.3211260139942169f;
        D_004FE480[3] = 0.4398239850997925f;
        D_004FE490[0] = 0.500997006893158f;
        D_004FE490[1] = 0.24849599599838257f;
        D_004FE490[2] = 1.0f;
        D_004FE490[3] = 1.0f;
        D_004FE4A0[0] = 0.27687299251556396f;
        D_004FE4A0[1] = 0.6395620107650757f;
        D_004FE4A0[2] = 0.13423900306224823f;
        D_004FE4A0[3] = 0.0f;
        D_004FE4B0[0] = 0.8936439752578735f;
        D_004FE4B0[1] = 0.5263469815254211f;
        D_004FE4B0[2] = 0.5051450133323669f;
        D_004FE4B0[3] = 0.8687300086021423f;
        D_004FE4C0[0] = 1.0f;
        D_004FE4C0[1] = 1.0f;
        D_004FE4C0[2] = 1.0f;
        D_004FE4C0[3] = 1.0f;
        D_004FE4D0[0] = 1.0f;
        D_004FE4D0[1] = 0.0f;
        D_004FE4D0[2] = 0.0f;
        D_004FE4D0[3] = 0.0f;
        D_004FE4E0[0] = 1.0f;
        D_004FE4E0[1] = 0.550000011920929f;
        D_004FE4E0[2] = 0.5350000262260437f;
        D_004FE4E0[3] = 0.550000011920929f;
        D_004FE4F0[0] = 1.0f;
        D_004FE4F0[1] = 1.0f;
        D_004FE4F0[2] = 1.0f;
        D_004FE4F0[3] = 1.0f;
        D_004FE500[0] = 1.0f;
        D_004FE500[1] = 1.0f;
        D_004FE500[2] = 1.0f;
        D_004FE500[3] = 1.0f;
        D_004FE510[0] = 1.0f;
        D_004FE510[1] = 0.0f;
        D_004FE510[2] = 0.0f;
        D_004FE510[3] = 0.0f;
        D_004FE520[0] = 1.0f;
        D_004FE520[1] = 0.0f;
        D_004FE520[2] = 0.0f;
        D_004FE520[3] = 0.0f;
        D_004FE530[0] = 1.0f;
        D_004FE530[1] = 1.0f;
        D_004FE530[2] = 1.0f;
        D_004FE530[3] = 1.0f;
        D_004FE540[0] = 1.0f;
        D_004FE540[1] = 0.5f;
        D_004FE540[2] = 1.0f;
        D_004FE540[3] = 0.5f;
        D_004FE550[0] = 1.0f;
        D_004FE550[1] = 0.5f;
        D_004FE550[2] = 1.0f;
        D_004FE550[3] = 0.5f;
        D_004FE560[0] = 1.0f;
        D_004FE560[1] = 0.5f;
        D_004FE560[2] = 1.0f;
        D_004FE560[3] = 0.5f;
        D_004FE570[0] = 1.0f;
        D_004FE570[1] = 0.7828119993209839f;
        D_004FE570[2] = 0.06171900033950806f;
        D_004FE570[3] = 0.03984399884939194f;
        D_004FE580[0] = 1.0f;
        D_004FE580[1] = 1.0f;
        D_004FE580[2] = 0.7988259792327881f;
        D_004FE580[3] = 0.0f;
        D_004FE590[0] = 0.9886710047721863f;
        D_004FE590[1] = 0.9230499863624573f;
        D_004FE590[2] = 0.34311699867248535f;
        D_004FE590[3] = 0.0f;
        D_004FE5A0[0] = 1.0f;
        D_004FE5A0[1] = 1.0f;
        D_004FE5A0[2] = 1.0f;
        D_004FE5A0[3] = 1.0f;
        D_004FE5B0[0] = 1.0f;
        D_004FE5B0[1] = 1.0f;
        D_004FE5B0[2] = 0.7976430058479309f;
        D_004FE5B0[3] = 0.0f;
        D_004FE5C0[0] = 1.0f;
        D_004FE5C0[1] = 0.9222670197486877f;
        D_004FE5C0[2] = 0.34179699420928955f;
        D_004FE5C0[3] = 0.0f;
        D_004FE5D0[0] = 1.0f;
        D_004FE5D0[1] = 0.7847639918327332f;
        D_004FE5D0[2] = 0.06054700165987015f;
        D_004FE5D0[3] = 0.04062499850988388f;
        D_004FE5E0[0] = 1.0f;
        D_004FE5E0[1] = 0.800000011920929f;
        D_004FE5E0[2] = 0.44999998807907104f;
        D_004FE5E0[3] = 1.0f;
        D_004FE5F0[0] = 1.0f;
        D_004FE5F0[1] = 0.3281260132789612f;
        D_004FE5F0[2] = 1.0f;
        D_004FE5F0[3] = 0.3539060056209564f;
        D_004FE600[0] = 1.0f;
        D_004FE600[1] = 1.0f;
        D_004FE600[2] = 1.0f;
        D_004FE600[3] = 1.0f;
        D_004FE610[0] = 1.0f;
        D_004FE610[1] = 1.0f;
        D_004FE610[2] = 1.0f;
        D_004FE610[3] = 1.0f;
        D_004FE620[0] = 1.0f;
        D_004FE620[1] = 0.8613290190696716f;
        D_004FE620[2] = 0.38124799728393555f;
        D_004FE620[3] = 0.0f;
        D_004FE630[0] = 1.0f;
        D_004FE630[1] = 1.0f;
        D_004FE630[2] = 1.0f;
        D_004FE630[3] = 1.0f;
        D_004FE640[0] = 1.0f;
        D_004FE640[1] = 0.3244380056858063f;
        D_004FE640[2] = 1.0f;
        D_004FE640[3] = 0.0f;
        D_004FE650[0] = 1.0f;
        D_004FE650[1] = 1.0f;
        D_004FE650[2] = 1.0f;
        D_004FE650[3] = 1.0f;
        D_004FE660[0] = 1.0f;
        D_004FE660[1] = 1.0f;
        D_004FE660[2] = 0.0f;
        D_004FE660[3] = 0.0f;
        D_004FE670[0] = 0.7007060050964355f;
        D_004FE670[1] = 0.0f;
        D_004FE670[2] = 0.0f;
        D_004FE670[3] = 0.0f;
        D_004FE680[0] = 1.0f;
        D_004FE680[1] = 0.7828530073165894f;
        D_004FE680[2] = 0.06140600144863129f;
        D_004FE680[3] = 0.04146299883723259f;
        D_004FE690[0] = 1.0f;
        D_004FE690[1] = 0.32487601041793823f;
        D_004FE690[2] = 1.0f;
        D_004FE690[3] = 0.35280099511146545f;
        D_004FE6A0[0] = 1.0f;
        D_004FE6A0[1] = 1.0f;
        D_004FE6A0[2] = 0.7977359890937805f;
        D_004FE6A0[3] = 0.0f;
        D_004FE6B0[0] = 1.0f;
        D_004FE6B0[1] = 0.4937489926815033f;
        D_004FE6B0[2] = 1.0f;
        D_004FE6B0[3] = 0.49179700016975403f;
        D_004FE6C0[0] = 1.0f;
        D_004FE6C0[1] = 1.0f;
        D_004FE6C0[2] = 1.0f;
        D_004FE6C0[3] = 1.0f;
        D_004FE6D0[0] = 1.0f;
        D_004FE6D0[1] = 0.800000011920929f;
        D_004FE6D0[2] = 1.0f;
        D_004FE6D0[3] = 0.800000011920929f;
        D_004FE6E0[0] = 1.0f;
        D_004FE6E0[1] = 1.0f;
        D_004FE6E0[2] = 0.0f;
        D_004FE6E0[3] = 0.0f;
        D_004FE6F0[0] = 1.0f;
        D_004FE6F0[1] = 1.0f;
        D_004FE6F0[2] = 0.0f;
        D_004FE6F0[3] = 0.0f;
        D_004FE700[0] = 1.0f;
        D_004FE700[1] = 1.0f;
        D_004FE700[2] = 0.0f;
        D_004FE700[3] = 0.0f;
        D_004FE710[0] = 1.0f;
        D_004FE710[1] = 0.3273420035839081f;
        D_004FE710[2] = 1.0f;
        D_004FE710[3] = 0.35390499234199524f;
        D_004FE720[0] = 1.0f;
        D_004FE720[1] = 1.0f;
        D_004FE720[2] = 0.061000000685453415f;
        D_004FE720[3] = 0.039000000804662704f;
        D_004FE730[0] = 1.0f;
        D_004FE730[1] = 1.0f;
        D_004FE730[2] = 0.7999979853630066f;
        D_004FE730[3] = 0.0f;
        D_004FE740[0] = 1.0f;
        D_004FE740[1] = 0.9589840173721313f;
        D_004FE740[2] = 0.6143789887428284f;
        D_004FE740[3] = 0.0f;
        D_004FE750[0] = 1.0f;
        D_004FE750[1] = 0.9285150170326233f;
        D_004FE750[2] = 0.3399839997291565f;
        D_004FE750[3] = 0.0f;
        D_004FE760[0] = 1.0f;
        D_004FE760[1] = 0.8738250136375427f;
        D_004FE760[2] = 0.24931700527668f;
        D_004FE760[3] = 0.017578000202775f;
        D_004FE770[0] = 1.0f;
        D_004FE770[1] = 0.7843739986419678f;
        D_004FE770[2] = 0.06132800132036209f;
        D_004FE770[3] = 0.03945299983024597f;
        D_004FE780[0] = 1.0f;
        D_004FE780[1] = 0.0f;
        D_004FE780[2] = 1.0f;
        D_004FE780[3] = 0.0f;
        D_004FE790[0] = 1.0f;
        D_004FE790[1] = 1.0f;
        D_004FE790[2] = 0.0f;
        D_004FE790[3] = 0.0f;

    }
}
#endif

extern "C" void* func_003150F8(int, int);

//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_getFrameRateFactor);
#ifdef SKIP_ASM
/**
 * Returns the rider animation's FrameRateFactor (the playback-rate factor copied into each sequence it starts).
 *
 * @param riderAnim  the cRiderAnimBase
 * @return FrameRateFactor
 */
extern "C" float cRiderAnimBase_getFrameRateFactor(cRiderAnimBase* riderAnim) {
    return (riderAnim->FrameRateFactor);
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_getGraftingMask);
#ifdef SKIP_ASM
/**
 * Returns the rider animation's 64-bit GraftMask (the bone mask copied into each sequence it starts).
 *
 * @param riderAnim  the cRiderAnimBase
 * @return GraftMask
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long cRiderAnimBase_getGraftingMask(cRiderAnimBase* riderAnim) {
    return ((long)riderAnim->GraftMask);
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_getOrientationOffset);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider animation's OrientationOffset (the tPose at +0x30).
 *
 * @param riderAnim  the cRiderAnimBase (passed as an int)
 * @return @p riderAnim + 0x30
 */
extern "C" int cRiderAnimBase_getOrientationOffset(int riderAnim) {
    return riderAnim + 0x30;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_isFlipped);
#ifdef SKIP_ASM
/**
 * Returns the rider animation's IsFlipped flag (mirrored animations).
 *
 * @param riderAnim  the cRiderAnimBase
 * @return IsFlipped
 */
extern "C" int cRiderAnimBase_isFlipped(cRiderAnimBase* riderAnim) {
    return (riderAnim->IsFlipped);
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_setIsFlipped);
#ifdef SKIP_ASM
/**
 * Sets the rider animation's IsFlipped flag; only sequences started afterwards pick it up.
 *
 * cRiderAnimBase_changeIsFlipped also changes the flag of the sequences already playing.
 *
 * @param riderAnim  the cRiderAnimBase
 * @param isFlipped  nonzero to mirror the animations
 */
extern "C" void cRiderAnimBase_setIsFlipped(cRiderAnimBase* riderAnim, int isFlipped) {
    riderAnim->IsFlipped = isFlipped;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_setFrameRateFactor);
#ifdef SKIP_ASM
/**
 * Sets the rider animation's FrameRateFactor (the playback-rate factor copied into each sequence it starts).
 *
 * @param riderAnim  the cRiderAnimBase
 * @param fparg0  the new factor
 */
extern "C" void cRiderAnimBase_setFrameRateFactor(cRiderAnimBase* riderAnim, float fparg0) {
    riderAnim->FrameRateFactor = fparg0;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_setHeadingOffset);
#ifdef SKIP_ASM
#include "bx/math3d.h"
extern "C" void fSinCos(float *, float *, float);
extern t4Vector t4vect_Origin;
extern t4Vector t4vect_ZAxis;

/**
 * Sets the rider animation's OrientationOffset to a pure turn of @p angle about the heading axis.
 *
 * Position (+0x30) becomes t4vect_Origin; rotation (+0x40) the quaternion of -@p angle about t4vect_ZAxis (half-angle
 * sine and cosine from fSinCos). cRiderAnimBase_changeHeadingOffset applies the same transform on top of the
 * playing sequences instead.
 *
 * @param angle  the heading offset (applied with the opposite sign)
 */
extern "C" void cRiderAnimBase_setHeadingOffset(cRiderAnimBase* self, float angle) {
    t4Vector rotation;
    float sinHalf;
    float cosHalf;
    const t4Vector* axis = &t4vect_ZAxis;

    self->OrientationOffset.P = t4vect_Origin;
    fSinCos(&sinHalf, &cosHalf, -angle * 0.5f);
    rotation.w = cosHalf;
    rotation.x = sinHalf * axis->x;
    rotation.y = sinHalf * axis->y;
    rotation.z = sinHalf * axis->z;
    self->OrientationOffset.Q = rotation;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_setOrientationOffset);
#ifdef SKIP_ASM
/**
 * Copies the 32-byte pose @p offset (position, rotation) into the rider animation's OrientationOffset (+0x30).
 *
 * @param riderAnim  the cRiderAnimBase
 * @param offset  the new orientation offset (tPose)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cRiderAnimBase_setOrientationOffset(cRiderAnimBase* riderAnim, tPose* offset) {
    (*(long long*)&riderAnim->OrientationOffset) = (long long) (*(long long*)&offset->P);
    (*(long long*)&riderAnim->OrientationOffset.Q) = (long long) (*(long long*)&offset->Q);
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_setGraftingMask);
#ifdef SKIP_ASM
/**
 * Sets the rider animation's 64-bit GraftMask (the bones the sequences it starts may drive).
 *
 * @param riderAnim  the cRiderAnimBase
 * @param graftMask  the new mask
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cRiderAnimBase_setGraftingMask(cRiderAnimBase* riderAnim, long graftMask) {
    ((long)riderAnim->GraftMask) = graftMask;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", cRiderAnimBase_clearGraftingMask);
#ifdef SKIP_ASM
/**
 * Resets the rider animation's GraftMask to -1 (all bits set: no bone is excluded).
 *
 * @param riderAnim  the cRiderAnimBase
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cRiderAnimBase_clearGraftingMask(cRiderAnimBase* riderAnim) {
    ((long)riderAnim->GraftMask) = -1;
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", tEventFlags_clear);
#ifdef SKIP_ASM
/**
 * Clears bit @p bit of the first 64-bit word of @p bits and, if it was set, sets it in the second word.
 *
 * The pair is an event flag set: word 0 says which events are set, word 1 which changed since last cleared (the
 * layout of cAnimSequence's event flags at +0xB0).
 *
 * @param bits  the two 64-bit words
 * @param bit  the bit number, 0-63
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void tEventFlags_clear(long* bits, int bit)
{
    long old = bits[0];
    long mask = (long)1 << bit;
    bits[0] = old & ~mask;
    bits[1] = bits[1] | (mask & old);
}
#endif
//100%
INCLUDE_ASM("animation/gameanimmap", tEventFlags_isSet);
#ifdef SKIP_ASM
/**
 * Returns whether bit @p bit of the first 64-bit word of @p bits is set (see tEventFlags_clear).
 *
 * @param bits  the event flag words
 * @param bit  the bit number, 0-63
 * @return true when the bit is set
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" bool tEventFlags_isSet(long* bits, int bit)
{
    long mask = (long)1 << bit;
    return (*bits & mask) != 0;
}
#endif

//99.38%
INCLUDE_ASM("animation/gameanimmap", func_00315A00__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's first static initializer: func_003150F8(1, 0xFFFF) (a g++ global-constructor entry).
 *
 * @return what func_003150F8 leaves in the return register (unused)
 */
void* func_00315A00(void* self)
{
    return func_003150F8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("animation/gameanimmap", func_00315A20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[8];
extern float cfm_sav_Physics_BoardPress_kBPHardBalanceRate[8];
extern float cfm_sav_Physics_Cruise_kPowderTiltBwd[8];
extern float cfm_sav_Physics_Turn_kDigMult[8];
extern float cfm_sav_Physics_Turn_kMaxTurnKickAngle[8];
extern float cfm_sav_Physics_Turn_kKickAngleSpeed[8];
extern float cfm_sav_Physics_Turn_kLowSpeedRotation[8];
extern float cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[8];
extern float cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[8];
extern float D_004FEA40[4];
extern float D_004FEA50[4];
extern float D_004FEA60[4];
extern float D_004FEA70[4];
extern float D_004FEA80[4];
extern float D_004FEA90[4];
extern float D_004FEAA0[4];
extern float D_004FEAB0[4];
extern float D_004FEAC0[4];
extern float D_004FEAD0[4];
extern float D_004FEAE0[4];
extern float D_004FEAF0[4];
extern float D_004FEB00[4];
extern float D_004FEB10[4];
extern float D_004FEB20[4];
extern float D_004FEB30[8];
extern float D_004FEB50[8];
extern float D_004FEB70[8];
extern float D_004FEB90[8];
extern float D_004FEBB0[8];
extern float D_004FEBD0[8];
extern float D_004FEBF0[8];
extern float D_004FEC10[8];
extern float D_004FEC30[8];
extern float D_004FEC50[8];
extern float D_004FEC70[8];
extern float D_004FEC90[8];
extern float D_004FECB0[8];
extern float D_004FECD0[8];
extern float D_004FECF0[8];
extern float D_004FED10[8];
extern float D_004FED30[8];
extern float D_004FED50[8];
extern float D_004FED70[4];
extern float D_004FED80[4];
extern float D_004FED90[4];
extern float D_004FEDA0[4];
extern float D_004FEDB0[4];
extern float D_004FEDC0[4];
extern float D_004FEDD0[4];
extern float D_004FEDE0[4];
extern float D_004FEDF0[4];
extern float D_004FEE00[4];
extern float D_004FEE10[4];
extern float D_004FEE20[4];
extern float D_004FEE30[4];
extern float D_004FEE40[4];
extern float D_004FEE50[4];
extern float D_004FEE60[4];
extern float D_004FEE70[4];
extern float D_004FEE80[4];
extern float D_004FEE90[4];
extern float D_004FEEA0[4];
extern float D_004FEEB0[4];
extern float D_004FEEC0[4];
extern float D_004FEED0[4];
extern float D_004FEEE0[4];
extern float D_004FEEF0[4];
extern float D_004FEF00[4];
extern float D_004FEF10[4];
extern float D_004FEF20[4];
extern float D_004FEF30[4];
extern float D_004FEF40[4];
extern float D_004FEF50[4];
extern float D_004FEF60[4];
extern float D_004FEF70[4];
extern float D_004FEF80[4];
extern float D_004FEF90[4];
extern float D_004FEFA0[4];
extern float D_004FEFB0[4];
extern float D_004FEFC0[4];
extern float D_004FEFD0[4];
extern float D_004FEFE0[4];
extern float D_004FEFF0[4];
void sti_00315A20(int init, int prio) __asm__("func_00315A20");
/**
 * Static initializer of this unit's second group of globals: fills 83 float tables (cfm_sav_Physics_BoardPress_kBPEasyBalanceRate .. ) with constants.
 *
 * Runs only for @p init nonzero and @p prio 0xFFFF (called by func_00316878 with 1, 0xFFFF). The tables are four or
 * eight floats; the eight-float ones read as four (x, y) points of a piecewise curve (e.g. cfm_sav_Physics_BoardPress_kBPEasyBalanceRate = (0, 0.2),
 * (0.4, 0.4), (0.6, 0.4), (1, 0.2)), and the AI code reaches several through pointers in .sdata (kBPEasyBalanceRate ->
 * cfm_sav_Physics_BoardPress_kBPEasyBalanceRate, kBPHardBalanceRate -> cfm_sav_Physics_BoardPress_kBPHardBalanceRate).
 *
 * @param init  nonzero to initialize (0 would be destruction; nothing is done then)
 * @param prio  the initialization priority; only 0xFFFF (the default) does anything
 */
void sti_00315A20(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[0] = 0.0f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[1] = 0.20000000298023224f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[2] = 0.4000000059604645f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[3] = 0.4000000059604645f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[4] = 0.6000000238418579f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[5] = 0.4000000059604645f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[6] = 1.0f;
        cfm_sav_Physics_BoardPress_kBPEasyBalanceRate[7] = 0.20000000298023224f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[0] = 0.0f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[1] = 0.30000001192092896f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[2] = 0.4000000059604645f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[3] = 1.2000000476837158f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[4] = 0.6000000238418579f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[5] = 1.2000000476837158f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[6] = 1.0f;
        cfm_sav_Physics_BoardPress_kBPHardBalanceRate[7] = 0.30000001192092896f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[0] = 0.0f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[1] = 0.20000000298023224f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[2] = 40.517601013183594f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[3] = 0.8055530190467834f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[4] = 50.945098876953125f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[5] = 0.35607799887657166f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[6] = 120.0f;
        cfm_sav_Physics_Cruise_kPowderTiltBwd[7] = 0.2501629889011383f;
        cfm_sav_Physics_Turn_kDigMult[0] = 0.0f;
        cfm_sav_Physics_Turn_kDigMult[1] = 1.750789999961853f;
        cfm_sav_Physics_Turn_kDigMult[2] = 49.37540054321289f;
        cfm_sav_Physics_Turn_kDigMult[3] = 1.7348599433898926f;
        cfm_sav_Physics_Turn_kDigMult[4] = 79.97119903564453f;
        cfm_sav_Physics_Turn_kDigMult[5] = 1.7510600090026855f;
        cfm_sav_Physics_Turn_kDigMult[6] = 120.0f;
        cfm_sav_Physics_Turn_kDigMult[7] = 0.885142982006073f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[0] = 0.0f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[1] = 40.959598541259766f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[2] = 20.827800750732422f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[3] = 10.0f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[4] = 50.08369827270508f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[5] = 10.0f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[6] = 120.0f;
        cfm_sav_Physics_Turn_kMaxTurnKickAngle[7] = 10.0f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[0] = 0.0f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[1] = 1.0614099502563477f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[2] = 20.654800415039062f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[3] = 1.1025999784469604f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[4] = 68.0f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[5] = 1.022879958152771f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[6] = 120.0f;
        cfm_sav_Physics_Turn_kKickAngleSpeed[7] = 1.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[0] = 0.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[1] = 0.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[2] = 22.6156005859375f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[3] = 0.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[4] = 41.7135009765625f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[5] = 0.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[6] = 120.0f;
        cfm_sav_Physics_Turn_kLowSpeedRotation[7] = 0.0f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[0] = 0.0f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[1] = 0.9511110186576843f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[2] = 45.61130142211914f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[3] = 0.952904999256134f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[4] = 100.22699737548828f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[5] = 0.9505529999732971f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[6] = 120.0f;
        cfm_sav_Physics_Turn_kSteerTuneMaxAttribEffect[7] = 0.9033470153808594f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[0] = 0.0f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[1] = 0.8039699792861938f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[2] = 40.09749984741211f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[3] = 0.7641159892082214f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[4] = 90.90560150146484f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[5] = 0.7656030058860779f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[6] = 120.0f;
        cfm_sav_Physics_Turn_kSteerTuneMinAttribEffect[7] = 0.7000259757041931f;
        D_004FEA40[0] = 1.0f;
        D_004FEA40[1] = 1.0f;
        D_004FEA40[2] = 1.0f;
        D_004FEA40[3] = 0.5f;
        D_004FEA50[0] = 0.4998680055141449f;
        D_004FEA50[1] = 1.0f;
        D_004FEA50[2] = 1.0f;
        D_004FEA50[3] = 0.0f;
        D_004FEA60[0] = 0.12951700389385223f;
        D_004FEA60[1] = 1.0f;
        D_004FEA60[2] = 1.0f;
        D_004FEA60[3] = 0.0f;
        D_004FEA70[0] = 0.22426900267601013f;
        D_004FEA70[1] = 0.9402310252189636f;
        D_004FEA70[2] = 0.5425059795379639f;
        D_004FEA70[3] = 0.0004030000127386302f;
        D_004FEA80[0] = 0.4652079939842224f;
        D_004FEA80[1] = 0.32170501351356506f;
        D_004FEA80[2] = 0.9943439960479736f;
        D_004FEA80[3] = 0.0f;
        D_004FEA90[0] = 0.17792600393295288f;
        D_004FEA90[1] = 1.0f;
        D_004FEA90[2] = 1.0f;
        D_004FEA90[3] = 0.0f;
        D_004FEAA0[0] = 0.4411740005016327f;
        D_004FEAA0[1] = 0.41388601064682007f;
        D_004FEAA0[2] = 0.3211260139942169f;
        D_004FEAA0[3] = 0.4398239850997925f;
        D_004FEAB0[0] = 0.500997006893158f;
        D_004FEAB0[1] = 0.24849599599838257f;
        D_004FEAB0[2] = 1.0f;
        D_004FEAB0[3] = 1.0f;
        D_004FEAC0[0] = 0.27687299251556396f;
        D_004FEAC0[1] = 0.6395620107650757f;
        D_004FEAC0[2] = 0.13423900306224823f;
        D_004FEAC0[3] = 0.0f;
        D_004FEAD0[0] = 0.8936439752578735f;
        D_004FEAD0[1] = 0.5263469815254211f;
        D_004FEAD0[2] = 0.5051450133323669f;
        D_004FEAD0[3] = 0.8687300086021423f;
        D_004FEAE0[0] = 1.0f;
        D_004FEAE0[1] = 1.0f;
        D_004FEAE0[2] = 1.0f;
        D_004FEAE0[3] = 1.0f;
        D_004FEAF0[0] = 1.0f;
        D_004FEAF0[1] = 0.0f;
        D_004FEAF0[2] = 0.0f;
        D_004FEAF0[3] = 0.0f;
        D_004FEB00[0] = 1.0f;
        D_004FEB00[1] = 0.550000011920929f;
        D_004FEB00[2] = 0.5350000262260437f;
        D_004FEB00[3] = 0.550000011920929f;
        D_004FEB10[0] = 1.0f;
        D_004FEB10[1] = 1.0f;
        D_004FEB10[2] = 1.0f;
        D_004FEB10[3] = 1.0f;
        D_004FEB20[0] = 1.0f;
        D_004FEB20[1] = 1.0f;
        D_004FEB20[2] = 1.0f;
        D_004FEB20[3] = 1.0f;
        D_004FEB30[0] = 0.0f;
        D_004FEB30[1] = 0.004000000189989805f;
        D_004FEB30[2] = 40.0f;
        D_004FEB30[3] = 0.017000000923871994f;
        D_004FEB30[4] = 80.0f;
        D_004FEB30[5] = 0.03999999910593033f;
        D_004FEB30[6] = 120.0f;
        D_004FEB30[7] = 0.07000000029802322f;
        D_004FEB50[0] = 0.0f;
        D_004FEB50[1] = 0.004000000189989805f;
        D_004FEB50[2] = 40.0f;
        D_004FEB50[3] = 0.017000000923871994f;
        D_004FEB50[4] = 80.0f;
        D_004FEB50[5] = 0.03999999910593033f;
        D_004FEB50[6] = 120.0f;
        D_004FEB50[7] = 0.07000000029802322f;
        D_004FEB70[0] = 0.0f;
        D_004FEB70[1] = 0.011993000283837318f;
        D_004FEB70[2] = 25.0f;
        D_004FEB70[3] = 0.13333700597286224f;
        D_004FEB70[4] = 81.0f;
        D_004FEB70[5] = 0.26667100191116333f;
        D_004FEB70[6] = 120.0f;
        D_004FEB70[7] = 0.3500039875507355f;
        D_004FEB90[0] = 0.0f;
        D_004FEB90[1] = 0.004000000189989805f;
        D_004FEB90[2] = 40.0f;
        D_004FEB90[3] = 0.017000000923871994f;
        D_004FEB90[4] = 80.0f;
        D_004FEB90[5] = 0.03999999910593033f;
        D_004FEB90[6] = 120.0f;
        D_004FEB90[7] = 0.07000000029802322f;
        D_004FEBB0[0] = 0.0f;
        D_004FEBB0[1] = 0.0f;
        D_004FEBB0[2] = 67.0f;
        D_004FEBB0[3] = 0.1464100033044815f;
        D_004FEBB0[4] = 101.0f;
        D_004FEBB0[5] = 0.4130750000476837f;
        D_004FEBB0[6] = 120.0f;
        D_004FEBB0[7] = 0.9000040292739868f;
        D_004FEBD0[0] = 0.0f;
        D_004FEBD0[1] = 0.0038499999791383743f;
        D_004FEBD0[2] = 18.0f;
        D_004FEBD0[3] = 0.07051700353622437f;
        D_004FEBD0[4] = 82.0f;
        D_004FEBD0[5] = 0.07051700353622437f;
        D_004FEBD0[6] = 120.0f;
        D_004FEBD0[7] = 0.07051700353622437f;
        D_004FEBF0[0] = 0.0f;
        D_004FEBF0[1] = 0.017000000923871994f;
        D_004FEBF0[2] = 67.0f;
        D_004FEBF0[3] = 0.25666698813438416f;
        D_004FEBF0[4] = 97.0f;
        D_004FEBF0[5] = 0.7566670179367065f;
        D_004FEBF0[6] = 120.0f;
        D_004FEBF0[7] = 1.6699999570846558f;
        D_004FEC10[0] = 0.0f;
        D_004FEC10[1] = 0.0f;
        D_004FEC10[2] = 67.0f;
        D_004FEC10[3] = 0.1464100033044815f;
        D_004FEC10[4] = 101.0f;
        D_004FEC10[5] = 0.4130750000476837f;
        D_004FEC10[6] = 120.0f;
        D_004FEC10[7] = 0.9000040292739868f;
        D_004FEC30[0] = 0.0f;
        D_004FEC30[1] = 0.0f;
        D_004FEC30[2] = 67.0f;
        D_004FEC30[3] = 0.1464100033044815f;
        D_004FEC30[4] = 101.0f;
        D_004FEC30[5] = 0.4130750000476837f;
        D_004FEC30[6] = 120.0f;
        D_004FEC30[7] = 0.9000040292739868f;
        D_004FEC50[0] = 0.0f;
        D_004FEC50[1] = 0.0f;
        D_004FEC50[2] = 30.0f;
        D_004FEC50[3] = 0.05000000074505806f;
        D_004FEC50[4] = 80.0f;
        D_004FEC50[5] = 0.11666599661111832f;
        D_004FEC50[6] = 120.0f;
        D_004FEC50[7] = 0.1333329975605011f;
        D_004FEC70[0] = 0.0f;
        D_004FEC70[1] = 0.004000000189989805f;
        D_004FEC70[2] = 40.0f;
        D_004FEC70[3] = 0.017000000923871994f;
        D_004FEC70[4] = 80.0f;
        D_004FEC70[5] = 0.03999999910593033f;
        D_004FEC70[6] = 120.0f;
        D_004FEC70[7] = 0.07000000029802322f;
        D_004FEC90[0] = 0.0f;
        D_004FEC90[1] = 0.012039000168442726f;
        D_004FEC90[2] = 28.0f;
        D_004FEC90[3] = 0.1333339959383011f;
        D_004FEC90[4] = 83.0f;
        D_004FEC90[5] = 0.2666670083999634f;
        D_004FEC90[6] = 120.0f;
        D_004FEC90[7] = 0.31666699051856995f;
        D_004FECB0[0] = 0.0f;
        D_004FECB0[1] = 0.056666601449251175f;
        D_004FECB0[2] = 50.0f;
        D_004FECB0[3] = 0.056666601449251175f;
        D_004FECB0[4] = 80.5999984741211f;
        D_004FECB0[5] = 0.12333299964666367f;
        D_004FECB0[6] = 120.0f;
        D_004FECB0[7] = 0.12333299964666367f;
        D_004FECD0[0] = 0.0f;
        D_004FECD0[1] = 0.0f;
        D_004FECD0[2] = 67.0f;
        D_004FECD0[3] = 0.1464100033044815f;
        D_004FECD0[4] = 101.0f;
        D_004FECD0[5] = 0.4130750000476837f;
        D_004FECD0[6] = 120.0f;
        D_004FECD0[7] = 0.9000040292739868f;
        D_004FECF0[0] = 0.0f;
        D_004FECF0[1] = 0.0f;
        D_004FECF0[2] = 67.0f;
        D_004FECF0[3] = 0.1464100033044815f;
        D_004FECF0[4] = 101.0f;
        D_004FECF0[5] = 0.4130750000476837f;
        D_004FECF0[6] = 120.0f;
        D_004FECF0[7] = 0.9000040292739868f;
        D_004FED10[0] = 0.0f;
        D_004FED10[1] = 0.0f;
        D_004FED10[2] = 67.0f;
        D_004FED10[3] = 0.1464100033044815f;
        D_004FED10[4] = 101.0f;
        D_004FED10[5] = 0.4130750000476837f;
        D_004FED10[6] = 120.0f;
        D_004FED10[7] = 0.9000040292739868f;
        D_004FED30[0] = 0.0f;
        D_004FED30[1] = 0.004000000189989805f;
        D_004FED30[2] = 40.0f;
        D_004FED30[3] = 0.017000000923871994f;
        D_004FED30[4] = 80.0f;
        D_004FED30[5] = 0.03999999910593033f;
        D_004FED30[6] = 120.0f;
        D_004FED30[7] = 0.07000000029802322f;
        D_004FED50[0] = 0.0f;
        D_004FED50[1] = 0.0f;
        D_004FED50[2] = 67.0f;
        D_004FED50[3] = 0.1464100033044815f;
        D_004FED50[4] = 101.0f;
        D_004FED50[5] = 0.4130750000476837f;
        D_004FED50[6] = 120.0f;
        D_004FED50[7] = 0.9000040292739868f;
        D_004FED70[0] = 1.0f;
        D_004FED70[1] = 0.0f;
        D_004FED70[2] = 0.0f;
        D_004FED70[3] = 0.0f;
        D_004FED80[0] = 1.0f;
        D_004FED80[1] = 0.0f;
        D_004FED80[2] = 0.0f;
        D_004FED80[3] = 0.0f;
        D_004FED90[0] = 1.0f;
        D_004FED90[1] = 1.0f;
        D_004FED90[2] = 1.0f;
        D_004FED90[3] = 1.0f;
        D_004FEDA0[0] = 1.0f;
        D_004FEDA0[1] = 0.5f;
        D_004FEDA0[2] = 1.0f;
        D_004FEDA0[3] = 0.5f;
        D_004FEDB0[0] = 1.0f;
        D_004FEDB0[1] = 0.5f;
        D_004FEDB0[2] = 1.0f;
        D_004FEDB0[3] = 0.5f;
        D_004FEDC0[0] = 1.0f;
        D_004FEDC0[1] = 0.5f;
        D_004FEDC0[2] = 1.0f;
        D_004FEDC0[3] = 0.5f;
        D_004FEDD0[0] = 1.0f;
        D_004FEDD0[1] = 0.7828119993209839f;
        D_004FEDD0[2] = 0.06171900033950806f;
        D_004FEDD0[3] = 0.03984399884939194f;
        D_004FEDE0[0] = 1.0f;
        D_004FEDE0[1] = 1.0f;
        D_004FEDE0[2] = 0.7988259792327881f;
        D_004FEDE0[3] = 0.0f;
        D_004FEDF0[0] = 0.9886710047721863f;
        D_004FEDF0[1] = 0.9230499863624573f;
        D_004FEDF0[2] = 0.34311699867248535f;
        D_004FEDF0[3] = 0.0f;
        D_004FEE00[0] = 1.0f;
        D_004FEE00[1] = 1.0f;
        D_004FEE00[2] = 1.0f;
        D_004FEE00[3] = 1.0f;
        D_004FEE10[0] = 1.0f;
        D_004FEE10[1] = 1.0f;
        D_004FEE10[2] = 0.7976430058479309f;
        D_004FEE10[3] = 0.0f;
        D_004FEE20[0] = 1.0f;
        D_004FEE20[1] = 0.9222670197486877f;
        D_004FEE20[2] = 0.34179699420928955f;
        D_004FEE20[3] = 0.0f;
        D_004FEE30[0] = 1.0f;
        D_004FEE30[1] = 0.7847639918327332f;
        D_004FEE30[2] = 0.06054700165987015f;
        D_004FEE30[3] = 0.04062499850988388f;
        D_004FEE40[0] = 1.0f;
        D_004FEE40[1] = 0.800000011920929f;
        D_004FEE40[2] = 0.44999998807907104f;
        D_004FEE40[3] = 1.0f;
        D_004FEE50[0] = 1.0f;
        D_004FEE50[1] = 0.3281260132789612f;
        D_004FEE50[2] = 1.0f;
        D_004FEE50[3] = 0.3539060056209564f;
        D_004FEE60[0] = 1.0f;
        D_004FEE60[1] = 1.0f;
        D_004FEE60[2] = 1.0f;
        D_004FEE60[3] = 1.0f;
        D_004FEE70[0] = 1.0f;
        D_004FEE70[1] = 1.0f;
        D_004FEE70[2] = 1.0f;
        D_004FEE70[3] = 1.0f;
        D_004FEE80[0] = 1.0f;
        D_004FEE80[1] = 0.8613290190696716f;
        D_004FEE80[2] = 0.38124799728393555f;
        D_004FEE80[3] = 0.0f;
        D_004FEE90[0] = 1.0f;
        D_004FEE90[1] = 1.0f;
        D_004FEE90[2] = 1.0f;
        D_004FEE90[3] = 1.0f;
        D_004FEEA0[0] = 1.0f;
        D_004FEEA0[1] = 0.3244380056858063f;
        D_004FEEA0[2] = 1.0f;
        D_004FEEA0[3] = 0.0f;
        D_004FEEB0[0] = 1.0f;
        D_004FEEB0[1] = 1.0f;
        D_004FEEB0[2] = 1.0f;
        D_004FEEB0[3] = 1.0f;
        D_004FEEC0[0] = 1.0f;
        D_004FEEC0[1] = 1.0f;
        D_004FEEC0[2] = 0.0f;
        D_004FEEC0[3] = 0.0f;
        D_004FEED0[0] = 0.7007060050964355f;
        D_004FEED0[1] = 0.0f;
        D_004FEED0[2] = 0.0f;
        D_004FEED0[3] = 0.0f;
        D_004FEEE0[0] = 1.0f;
        D_004FEEE0[1] = 0.7828530073165894f;
        D_004FEEE0[2] = 0.06140600144863129f;
        D_004FEEE0[3] = 0.04146299883723259f;
        D_004FEEF0[0] = 1.0f;
        D_004FEEF0[1] = 0.32487601041793823f;
        D_004FEEF0[2] = 1.0f;
        D_004FEEF0[3] = 0.35280099511146545f;
        D_004FEF00[0] = 1.0f;
        D_004FEF00[1] = 1.0f;
        D_004FEF00[2] = 0.7977359890937805f;
        D_004FEF00[3] = 0.0f;
        D_004FEF10[0] = 1.0f;
        D_004FEF10[1] = 0.4937489926815033f;
        D_004FEF10[2] = 1.0f;
        D_004FEF10[3] = 0.49179700016975403f;
        D_004FEF20[0] = 1.0f;
        D_004FEF20[1] = 1.0f;
        D_004FEF20[2] = 1.0f;
        D_004FEF20[3] = 1.0f;
        D_004FEF30[0] = 1.0f;
        D_004FEF30[1] = 0.800000011920929f;
        D_004FEF30[2] = 1.0f;
        D_004FEF30[3] = 0.800000011920929f;
        D_004FEF40[0] = 1.0f;
        D_004FEF40[1] = 1.0f;
        D_004FEF40[2] = 0.0f;
        D_004FEF40[3] = 0.0f;
        D_004FEF50[0] = 1.0f;
        D_004FEF50[1] = 1.0f;
        D_004FEF50[2] = 0.0f;
        D_004FEF50[3] = 0.0f;
        D_004FEF60[0] = 1.0f;
        D_004FEF60[1] = 1.0f;
        D_004FEF60[2] = 0.0f;
        D_004FEF60[3] = 0.0f;
        D_004FEF70[0] = 1.0f;
        D_004FEF70[1] = 0.3273420035839081f;
        D_004FEF70[2] = 1.0f;
        D_004FEF70[3] = 0.35390499234199524f;
        D_004FEF80[0] = 1.0f;
        D_004FEF80[1] = 1.0f;
        D_004FEF80[2] = 0.061000000685453415f;
        D_004FEF80[3] = 0.039000000804662704f;
        D_004FEF90[0] = 1.0f;
        D_004FEF90[1] = 1.0f;
        D_004FEF90[2] = 0.7999979853630066f;
        D_004FEF90[3] = 0.0f;
        D_004FEFA0[0] = 1.0f;
        D_004FEFA0[1] = 0.9589840173721313f;
        D_004FEFA0[2] = 0.6143789887428284f;
        D_004FEFA0[3] = 0.0f;
        D_004FEFB0[0] = 1.0f;
        D_004FEFB0[1] = 0.9285150170326233f;
        D_004FEFB0[2] = 0.3399839997291565f;
        D_004FEFB0[3] = 0.0f;
        D_004FEFC0[0] = 1.0f;
        D_004FEFC0[1] = 0.8738250136375427f;
        D_004FEFC0[2] = 0.24931700527668f;
        D_004FEFC0[3] = 0.017578000202775f;
        D_004FEFD0[0] = 1.0f;
        D_004FEFD0[1] = 0.7843739986419678f;
        D_004FEFD0[2] = 0.06132800132036209f;
        D_004FEFD0[3] = 0.03945299983024597f;
        D_004FEFE0[0] = 1.0f;
        D_004FEFE0[1] = 0.0f;
        D_004FEFE0[2] = 1.0f;
        D_004FEFE0[3] = 0.0f;
        D_004FEFF0[0] = 1.0f;
        D_004FEFF0[1] = 1.0f;
        D_004FEFF0[2] = 0.0f;
        D_004FEFF0[3] = 0.0f;
    }
}
#endif

extern "C" void* func_00315A20(int, int);

//99.38%
INCLUDE_ASM("animation/gameanimmap", func_00316878__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's second static initializer: func_00315A20(1, 0xFFFF) (a g++ global-constructor entry).
 *
 * @return what func_00315A20 leaves in the return register (unused)
 */
void* func_00316878(void* self)
{
    return func_00315A20(1, 0xffff);
}
#endif

