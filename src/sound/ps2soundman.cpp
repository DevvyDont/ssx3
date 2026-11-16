#include "common.h"

//100%
INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts_cSndPlayOpts);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_9350(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void func_003BA020(void* desc);
extern "C" void* func_00416210(void* dst, int v, int n);
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
// PORT: the unit declares D_00483BE0 later with its own struct type; view bound by asm label
extern const sSmVtbl9350 D_00483BE0_9350 __asm__("D_00483BE0");

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
    *(const sSmVtbl9350**)(self->vbase + 4) = &D_00483BE0_9350;
    if (inchrg == 0) {
        sSmVtbl9350 vtCopy = D_00483BE0_9350;
        *(sSmVtbl9350**)(self->vbase + 4) = &vtCopy;
        char* base = self->vbase - 0x88;
        int thisAdjust = (char*)self - base;
        vtCopy.e[1].delta = D_00483BE0_9350.e[1].delta + thisAdjust;
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
    func_00416210((char*)self + self->depth * 8 + 0x40, 0, 8);
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
    func_003BA020(&self->descs[self->depth]);
    self->cur = self->descs[self->depth];
    self->depth--;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/ps2soundman", func_002A97C0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);

struct sSmVEntry97C0 {
    short delta;
    short index;
    void* fn;
};
struct sSmVtbl97C0 {
    sSmVEntry97C0 e[4];
} __attribute__((aligned(8)));
extern const sSmVtbl97C0 D_00483BE0;

// PORT: g++ 2.95 virtual-base destruction with a stack vtable this-adjust fix-up, written out by hand.
extern "C" void func_002A97C0(char* self, int flags)
{
    *(const sSmVtbl97C0**)(*(char**)self + 4) = &D_00483BE0;
    if (flags == 0) {
        sSmVtbl97C0 t1 = D_00483BE0;
        *(sSmVtbl97C0**)(*(char**)self + 4) = &t1;
        char* base = *(char**)self - 0x88;
        int d = self - base;
        t1.e[1].delta = D_00483BE0.e[1].delta + d;
    }
    if (*(void**)(self + 0x20) != 0) {
        cMemMan_free(*(void**)(self + 0x20));
    }
    if (*(void**)(self + 0x34) != 0) {
        cMemMan_free(*(void**)(self + 0x34));
    }
    if (*(void**)(self + 0x38) != 0) {
        cMemMan_free(*(void**)(self + 0x38));
    }
    if (*(void**)(self + 0x3C) != 0) {
        cMemMan_free(*(void**)(self + 0x3C));
    }
    if (*(void**)(self + 0x60) != 0) cMemMan_free(*(void**)(self + 0x60));
    if (*(void**)(self + 0x64) != 0) cMemMan_free(*(void**)(self + 0x64));
    if (*(void**)(self + 0x68) != 0) cMemMan_free(*(void**)(self + 0x68));
    if (*(void**)(self + 0x6C) != 0) cMemMan_free(*(void**)(self + 0x6C));
    if (*(void**)(self + 0x70) != 0) cMemMan_free(*(void**)(self + 0x70));
    if (*(void**)(self + 0x74) != 0) cMemMan_free(*(void**)(self + 0x74));
    if (*(void**)(self + 0x78) != 0) cMemMan_free(*(void**)(self + 0x78));
    if (*(void**)(self + 0x7C) != 0) cMemMan_free(*(void**)(self + 0x7C));
    if (*(void**)(self + 0x80) != 0) cMemMan_free(*(void**)(self + 0x80));
    if (*(void**)(self + 0x84) != 0) cMemMan_free(*(void**)(self + 0x84));
    if (flags & 2) {
        cSoundSys__cSoundSys(*(void**)self, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ps2soundman", func_002A9988);
#ifdef SKIP_ASM
extern "C" void func_002A9988(void* self, float a, float b)
{
    (*(float**)((char*)self + 0x6C))[*(int*)((char*)self + 0x1C)] = a;
    if (b < 0.0f) {
        (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = a;
        return;
    }
    (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = b;
}
#endif

