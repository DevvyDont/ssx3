#include "common.h"
#include "sound/bankmonitor.h"

//100%
INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts_cSndPlayOpts);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_9350(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void SNDplaysetdef(void* desc);
extern "C" void* memset(void* dst, int v, int n);
extern char str_mCurrent[];
extern char str_mPosStatic[];
extern char str_mBaseVol[];
extern char str_mVolControl[];
extern char str_mPtr[];
extern char str_mViewport[];
extern char str_mVanish[];
extern char str_mRadius[];
extern char str_mDelay[];
extern char str_mExternalVol[];
extern char str_mMapToMaxVol[];
extern char str_mCloseOnEnd[];
extern char str_mTwoPlayerPan[];
extern char str_mTwoPlayerSide[];

struct sSmVEntry9350 {
    short delta;
    short index;
    void* fn;
};
struct sSmVtbl9350 {
    sSmVEntry9350 e[4];
} __attribute__((aligned(8)));
// PORT: the unit declares cSndPlayOpts_cSoundSys_vtbl later with its own struct type; view bound by asm label
extern const sSmVtbl9350 cSndPlayOpts_cSoundSys_vtbl_9350 __asm__("cSndPlayOpts_cSoundSys_vtbl");

struct sSndDesc9350 {
    char c[0x18];
};

struct sSndEnv9350 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sSndPlayOpts9350 {
    char* vbase;                // 0x00
    sSndDesc9350 cur;           // 0x04
    volatile int depth;         // 0x1C
    sSndDesc9350* descs;        // 0x20
    int f24[4];                 // 0x24
    int* f34;                   // 0x34
    int* f38;                   // 0x38
    int* f3C;                   // 0x3C
    char f40[4][8];             // 0x40
    int* f60;                   // 0x60
    int* f64;                   // 0x64
    int* f68;                   // 0x68
    int* f6C;                   // 0x6C
    sSndEnv9350* env;           // 0x70
    int* f74;                   // 0x74
    int* f78;                   // 0x78
    int* f7C;                   // 0x7C
    int* f80;                   // 0x80
    int* f84;                   // 0x84
};

/**
 * Constructs the sound play options: a stack of option sets for the next sound to play.
 *
 * When @p inchrg is set it also constructs the cSoundSys virtual base at +0x88. It allocates one array per
 * option ("mCurrent", "mPosStatic", "mBaseVol", "mVolControl", "mPtr", "mViewport", "mVanish", "mRadius",
 * "mDelay", "mExternalVol", "mMapToMaxVol", "mCloseOnEnd", "mTwoPlayerPan", "mTwoPlayerSide"). Level 0 gets
 * the defaults (max volume 127, close on end), which are copied to the current set; the depth is back at -1.
 *
 * @param inchrg  nonzero when this object is complete and must construct its virtual base
 * @param heap    the audio mode passed to the cSoundSys base (On Tour audioMode)
 * @return self
 */
// PORT: g++ 2.95 virtual-base construction with a stack vtable this-adjust fix-up, written out by hand.
extern "C" sSndPlayOpts9350* cSndPlayOpts_cSndPlayOpts(sSndPlayOpts9350* self, int inchrg, int heap)
{
    if (inchrg != 0) {
        self->vbase = (char*)self + 0x88;
        cSoundSys_cSoundSys((char*)self + 0x88, heap);
    }
    *(const sSmVtbl9350**)(self->vbase + 4) = &cSndPlayOpts_cSoundSys_vtbl_9350;
    if (inchrg == 0) {
        sSmVtbl9350 vtCopy = cSndPlayOpts_cSoundSys_vtbl_9350;
        *(sSmVtbl9350**)(self->vbase + 4) = &vtCopy;
        char* base = self->vbase - 0x88;
        int thisAdjust = (char*)self - base;
        vtCopy.e[1].delta = cSndPlayOpts_cSoundSys_vtbl_9350.e[1].delta + thisAdjust;
    }
    self->depth = -1;
    self->descs = (sSndDesc9350*)operator_new_9350(0x60, str_mCurrent, 0, 0);
    self->f34 = (int*)operator_new_9350(0x10, str_mPosStatic, 0, 0);
    self->f38 = (int*)operator_new_9350(0x10, str_mBaseVol, 0, 0);
    self->f3C = (int*)operator_new_9350(0x10, str_mVolControl, 0, 0);
    self->f60 = (int*)operator_new_9350(0x10, str_mPtr, 0, 0);
    self->f64 = (int*)operator_new_9350(0x10, str_mViewport, 0, 0);
    self->f68 = (int*)operator_new_9350(0x10, str_mVanish, 0, 0);
    self->f6C = (int*)operator_new_9350(0x10, str_mRadius, 0, 0);
    self->env = (sSndEnv9350*)operator_new_9350(0x40, str_mDelay, 0, 0);
    self->f74 = (int*)operator_new_9350(0x10, str_mExternalVol, 0, 0);
    self->f78 = (int*)operator_new_9350(0x10, str_mMapToMaxVol, 0, 0);
    self->f7C = (int*)operator_new_9350(0x10, str_mCloseOnEnd, 0, 0);
    self->f80 = (int*)operator_new_9350(0x10, str_mTwoPlayerPan, 0, 0);
    self->f84 = (int*)operator_new_9350(0x10, str_mTwoPlayerSide, 0, 0);
    self->depth++;
    self->descs[self->depth] = self->cur;
    self->f24[self->depth] = 0;
    self->f34[self->depth] = 0;
    self->f38[self->depth] = self->descs[self->depth].c[0];
    self->f3C[self->depth] = 0;
    memset((char*)self + self->depth * 8 + 0x40, 0, 8);
    self->f60[self->depth] = 0;
    self->f64[self->depth] = 0;
    self->f68[self->depth] = 0;
    self->f6C[self->depth] = 0;
    self->env[self->depth].f4 = 0;
    self->env[self->depth].f0 = 100;
    self->env[self->depth].f8 = 90;
    self->env[self->depth].fC = 50;
    self->f74[self->depth] = 0;
    self->f80[self->depth] = 0;
    self->f84[self->depth] = 0;
    self->f78[self->depth] = 127;
    self->f7C[self->depth] = 1;
    SNDplaysetdef(&self->descs[self->depth]);
    self->cur = self->descs[self->depth];
    self->depth--;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts__cSndPlayOpts);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern "C" void cSoundSys__cSoundSys(void* self, int flags) __asm__("_$_9cSoundSys");

struct sSmVEntry97C0 {
    short delta;
    short index;
    void* fn;
};
struct sSmVtbl97C0 {
    sSmVEntry97C0 e[4];
} __attribute__((aligned(8)));
extern const sSmVtbl97C0 cSndPlayOpts_cSoundSys_vtbl;

/**
 * Destroys a sound play-option set: frees its per-level option arrays, then its virtual base and memory if asked.
 *
 * Points the virtual base's vtable (+4 of the base at *this) at cSndPlayOpts_cSoundSys_vtbl. When @p flags is 0 (a not-in-charge call)
 * it installs a stack copy instead, whose entry 1 has its this-delta adjusted by this object's distance from where a
 * complete object would start (the virtual base minus 0x88). Frees (cMemMan_free) each non-null array at +0x20, +0x34,
 * +0x38, +0x3C and +0x60 to +0x84. Bit 1 of @p flags runs cSoundSys__cSoundSys(0) on the virtual base and bit 0 frees
 * the object (operator_delete). The cMonitor destructor calls it with 0.
 *
 * @param flags  bit 0 frees the memory, bit 1 destroys the virtual base; 0 for a not-in-charge call
 */
// PORT: g++ 2.95 virtual-base destruction with a stack vtable this-adjust fix-up, written out by hand.
extern "C" void cSndPlayOpts__cSndPlayOpts(cSndPlayOpts* self, int flags)
{
    *(const sSmVtbl97C0**)(*(char**)self + 4) = &cSndPlayOpts_cSoundSys_vtbl;
    if (flags == 0) {
        sSmVtbl97C0 vtableCopy = cSndPlayOpts_cSoundSys_vtbl;
        *(sSmVtbl97C0**)(*(char**)self + 4) = &vtableCopy;
        char* base = *(char**)self - 0x88;
        int thisDelta = ((char*)self) - base;
        vtableCopy.e[1].delta = cSndPlayOpts_cSoundSys_vtbl.e[1].delta + thisDelta;
    }
    if (self->mCurrent != 0) {
        cMemMan_free(self->mCurrent);
    }
    if (self->mPosStatic != 0) {
        cMemMan_free(self->mPosStatic);
    }
    if (self->mBaseVol != 0) {
        cMemMan_free(self->mBaseVol);
    }
    if (self->mVolControl != 0) {
        cMemMan_free(self->mVolControl);
    }
    if (self->mPtr != 0) cMemMan_free(self->mPtr);
    if (self->mViewport != 0) cMemMan_free(self->mViewport);
    if (self->mVanish != 0) cMemMan_free(self->mVanish);
    if (self->mRadius != 0) cMemMan_free(self->mRadius);
    if (self->mDelay != 0) cMemMan_free(self->mDelay);
    if (self->mExternalVol != 0) cMemMan_free(self->mExternalVol);
    if (self->mMapToMaxVol != 0) cMemMan_free(self->mMapToMaxVol);
    if (self->mCloseOnEnd != 0) cMemMan_free(self->mCloseOnEnd);
    if (self->mTwoPlayerPan != 0) cMemMan_free(self->mTwoPlayerPan);
    if (self->mTwoPlayerSide != 0) cMemMan_free(self->mTwoPlayerSide);
    if (flags & 2) {
        cSoundSys__cSoundSys(*(void**)self, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts_OPTS_Radius);
#ifdef SKIP_ASM
/**
 * Sets the current play-option level's two 3D distances; a negative second one copies the first.
 *
 * Stores @p dist at +0x6C[level] and @p vanish, or @p dist when @p vanish is negative, at +0x68[level]; the level is
 * the current one (+0x1C). The audio code passes 100 for the rider loops, 30 for landings, 300 and about 30 elsewhere,
 * always with -1 for @p vanish.
 *
 * @param dist  the first distance (+0x6C)
 * @param vanish  the second distance (+0x68); negative to use @p dist
 */
void cSndPlayOpts::OPTS_Radius(float dist, float vanish)
{
    mRadius[(int)mInst] = dist;
    if (vanish < 0.0f) {
        mVanish[(int)mInst] = dist;
        return;
    }
    mVanish[(int)mInst] = vanish;
}
#endif

