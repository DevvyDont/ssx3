#include "common.h"

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_CreateFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
/* RCMP::RCMP_SYSTEM, the allocator the RCMP library calls (ealib/rcmpbase.h, restated for this C unit; [O] On Tour's
 * DWARF). The one instance is the global RCMP_rcmp_sys. */
struct RCMP_RCMP_SYSTEM {
    void* (*AllocMemFunc)(const char* tag, int size, int align, int d, int memDir); /* 0x0 */
    void* (*FreeMemFunc)(void* p);                                                 /* 0x4 */
    int m_DefaultMemDir;                                                           /* 0x8 */
    void** mVtbl;                                                                  /* 0xC */
};
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
extern char D_004953E8[];
extern int *func_003B07F8_x(void* a0, int a1, int a2) __asm__("func_003B07F8");
/**
 * Creates a video frame for the Sony codec and links it at the front of the codec's frame list.
 *
 * Allocates 20 bytes from the movie allocator (RCMP_rcmp_sys with an empty name and the memory type at D_00509438), builds
 * the frame with func_003B07F8 (a cleared 32-bit SHAPE of the given size) and links the frame's list node (+8) after
 * the list head at @p sonyCodec + 0x20 (word 8). Likely bug: when the allocation fails the null node is still linked.
 *
 * @param sonyCodec  the PS2_SONY_CODEC_INTERNAL object
 * @param frameWidth  frame width in pixels
 * @param frameHeight  frame height in pixels
 */
void PS2_SONY_CODEC_INTERNAL_CreateFrame(int *sonyCodec, int frameWidth, int frameHeight) {
    int *n = func_003B07F8_x(RCMP_rcmp_sys.AllocMemFunc(D_004953E8, 20, 0, 0, RCMP_rcmp_sys.m_DefaultMemDir),
                             frameWidth, frameHeight);
    int *node = n ? n + 2 : 0;
    int *head = (int *)sonyCodec[8];
    sonyCodec[8] = (int)node;
    head[1] = (int)node;
    node[1] = (int)(sonyCodec + 8);
    node[0] = (int)head;
}
#endif

/**
 * Does nothing (an empty placement array delete of the Sony codec class).
 */
void PS2_SONY_CODEC_INTERNAL_operator_delete_array(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new_array);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged (a placement array new of the Sony codec class: returns the given address).
 *
 * @param allocSize  bytes requested (unused)
 * @param value  the address to construct at
 * @return @p value
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
long PS2_SONY_CODEC_INTERNAL_operator_new_array(int allocSize, long value) {
    return value;
}
#endif

/**
 * Does nothing (an empty placement delete of the Sony codec class).
 */
void PS2_SONY_CODEC_INTERNAL_operator_delete(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged (a placement new of the Sony codec class: returns the given address).
 *
 * @param allocSize  bytes requested (unused)
 * @param value  the address to construct at
 * @return @p value
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
long PS2_SONY_CODEC_INTERNAL_operator_new(int allocSize, long value) {
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_delete_array1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees a Sony codec array through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int PS2_SONY_CODEC_INTERNAL_operator_delete_array1(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_delete1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees a Sony codec object through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int PS2_SONY_CODEC_INTERNAL_operator_delete1(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new_array1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
/**
 * Allocates a Sony codec array with the movie allocator, naming the block.
 *
 * Calls the allocation function RCMP_rcmp_sys with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second
 * (cMoviePlayer_SSXAllocMemFunc in this game).
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void PS2_SONY_CODEC_INTERNAL_operator_new_array1(int allocSize, const char* blockName, int alignment, int hdrSize,
                                                 int memType) {
    RCMP_rcmp_sys.AllocMemFunc(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
/**
 * Allocates a Sony codec object with the movie allocator, naming the block.
 *
 * Calls the allocation function RCMP_rcmp_sys with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void PS2_SONY_CODEC_INTERNAL_operator_new1(int allocSize, const char* blockName, int alignment, int hdrSize,
                                           int memType) {
    RCMP_rcmp_sys.AllocMemFunc(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new_array2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
extern char D_004953E8[];
/**
 * Allocates a Sony codec array with the movie allocator: RCMP_rcmp_sys with an empty name (D_004953E8), @p allocSize, alignment
 * 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void PS2_SONY_CODEC_INTERNAL_operator_new_array2(int allocSize) {
    RCMP_rcmp_sys.AllocMemFunc(D_004953E8, allocSize, 0, 0, RCMP_rcmp_sys.m_DefaultMemDir);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", PS2_SONY_CODEC_INTERNAL_operator_new2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
extern char D_004953E8[];
/**
 * Allocates a Sony codec object with the movie allocator: RCMP_rcmp_sys with an empty name (D_004953E8), @p allocSize, alignment
 * 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void PS2_SONY_CODEC_INTERNAL_operator_new2(int allocSize) {
    RCMP_rcmp_sys.AllocMemFunc(D_004953E8, allocSize, 0, 0, RCMP_rcmp_sys.m_DefaultMemDir);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", __tf23PS2_SONY_CODEC_INTERNAL);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int __ti23PS2_SONY_CODEC_INTERNAL;
extern int __tiQ24RCMP5CODEC;
extern char str_Q24RCMP5CODEC[];
extern char str_23PS2_SONY_CODEC_INTERNAL[];
extern void __rtti_user(void *a0, void *a1);
extern void __rtti_si(void *a0, void *a1, void *a2);
/**
 * Returns the run-time type descriptor of PS2_SONY_CODEC_INTERNAL, building it on first use.
 *
 * When __ti23PS2_SONY_CODEC_INTERNAL is still empty it first builds the base class descriptor __tiQ24RCMP5CODEC ("Q24RCMP5CODEC", __rtti_user)
 * if needed, then __ti23PS2_SONY_CODEC_INTERNAL itself as a single-base descriptor named "23PS2_SONY_CODEC_INTERNAL" (__rtti_si).
 *
 * @return &__ti23PS2_SONY_CODEC_INTERNAL
 */
int *__tf23PS2_SONY_CODEC_INTERNAL(void) {
    if (__ti23PS2_SONY_CODEC_INTERNAL == 0) {
        if (__tiQ24RCMP5CODEC == 0) {
            __rtti_user(&__tiQ24RCMP5CODEC, str_Q24RCMP5CODEC);
        }
        __rtti_si(&__ti23PS2_SONY_CODEC_INTERNAL, str_23PS2_SONY_CODEC_INTERNAL, &__tiQ24RCMP5CODEC);
    }
    return &__ti23PS2_SONY_CODEC_INTERNAL;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_CODEC__CODEC);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void* RCMP_CODEC_vtbl[];
/* RCMP::CODEC, the abstract codec base: only its vptr (ealib/rcmpbase.h, restated for this C unit). */
struct RCMP_CODEC {
    void** mVtbl;
};
extern int (*D_00509434[])(void);
/**
 * Destroys an RCMP::CODEC: restores its vtable (RCMP_CODEC_vtbl, _vt$Q24RCMP5CODEC) and, when bit 0 of @p dtorFlags is set, frees
 * it through the movie allocator's free function (D_00509434).
 *
 * @param codecObj  the codec object
 * @param dtorFlags  destructor flags (bit 0: free the object)
 */
void RCMP_CODEC__CODEC(struct RCMP_CODEC *codecObj, int dtorFlags) {
    codecObj->mVtbl = RCMP_CODEC_vtbl;
    if (dtorFlags & 1) {
        D_00509434[0]();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", __tfQ24RCMP5CODEC);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int __tiQ24RCMP5CODEC;
extern char str_Q24RCMP5CODEC[];
extern void __rtti_user(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::CODEC (__tiQ24RCMP5CODEC, named "Q24RCMP5CODEC"), building it on first use
 * with __rtti_user.
 *
 * @return &__tiQ24RCMP5CODEC
 */
int *__tfQ24RCMP5CODEC(void) {
    if (__tiQ24RCMP5CODEC == 0) {
        __rtti_user(&__tiQ24RCMP5CODEC, str_Q24RCMP5CODEC);
    }
    return &__tiQ24RCMP5CODEC;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_GetCodecIData);
#ifdef SKIP_ASM
/**
 * Returns the address of a decoder's codec instance data (@p decoderObj + 4).
 *
 * @param decoderObj  the RCMP::DECODER
 * @return @p decoderObj + 4
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int RCMP_DECODER_GetCodecIData(int decoderObj) {
    return decoderObj + 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_HasCodec);
#ifdef SKIP_ASM
/**
 * Returns whether a decoder has a codec chosen (word 5, +0x14, non-zero).
 *
 * @param decoderObj  the RCMP::DECODER
 * @return 1 when a codec is set, else 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int RCMP_DECODER_HasCodec(int *decoderObj) {
    return decoderObj[5] != 0;
}
#endif

/**
 * Does nothing (an empty placement array delete of RCMP::DECODER).
 */
void RCMP_DECODER_operator_delete_array(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new_array);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged (a placement array new of RCMP::DECODER: returns the given address).
 *
 * @param allocSize  bytes requested (unused)
 * @param value  the address to construct at
 * @return @p value
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
long RCMP_DECODER_operator_new_array(int allocSize, long value) {
    return value;
}
#endif

/**
 * Does nothing (an empty placement delete of RCMP::DECODER).
 */
void RCMP_DECODER_operator_delete(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new);
#ifdef SKIP_ASM
/**
 * Returns @p value unchanged (a placement new of RCMP::DECODER: returns the given address).
 *
 * @param allocSize  bytes requested (unused)
 * @param value  the address to construct at
 * @return @p value
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
long RCMP_DECODER_operator_new(int allocSize, long value) {
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_delete_array1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees an RCMP::DECODER array through the movie allocator's free function (D_00509434), passing the pointer argument
 * on unchanged.
 *
 * @return the free function's result
 */
int RCMP_DECODER_operator_delete_array1(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_delete1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees an RCMP::DECODER through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int RCMP_DECODER_operator_delete1(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new_array1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
/**
 * Allocates an RCMP::DECODER array with the movie allocator, naming the block.
 *
 * Calls the allocation function RCMP_rcmp_sys with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void RCMP_DECODER_operator_new_array1(int allocSize, const char* blockName, int alignment, int hdrSize, int memType) {
    RCMP_rcmp_sys.AllocMemFunc(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
/**
 * Allocates an RCMP::DECODER with the movie allocator, naming the block.
 *
 * Calls the allocation function RCMP_rcmp_sys with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void RCMP_DECODER_operator_new1(int allocSize, const char* blockName, int alignment, int hdrSize, int memType) {
    RCMP_rcmp_sys.AllocMemFunc(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new_array2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
extern char D_004953B8[];
/**
 * Allocates an RCMP::DECODER array with the movie allocator: RCMP_rcmp_sys with an empty name (D_004953B8), @p allocSize,
 * alignment 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void RCMP_DECODER_operator_new_array2(int allocSize) {
    RCMP_rcmp_sys.AllocMemFunc(D_004953B8, allocSize, 0, 0, RCMP_rcmp_sys.m_DefaultMemDir);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_DECODER_operator_new2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern struct RCMP_RCMP_SYSTEM RCMP_rcmp_sys;
extern char D_004953B8[];
/**
 * Allocates an RCMP::DECODER with the movie allocator: RCMP_rcmp_sys with an empty name (D_004953B8), @p allocSize, alignment 0,
 * header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void RCMP_DECODER_operator_new2(int allocSize) {
    RCMP_rcmp_sys.AllocMemFunc(D_004953B8, allocSize, 0, 0, RCMP_rcmp_sys.m_DefaultMemDir);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", __tfQ24RCMP7DECODER);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int __tiQ24RCMP7DECODER;
extern char str_Q24RCMP7DECODER[];
extern void __rtti_user(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::DECODER (__tiQ24RCMP7DECODER, named "Q24RCMP7DECODER"), building it on first use
 * with __rtti_user.
 *
 * @return &__tiQ24RCMP7DECODER
 */
int *__tfQ24RCMP7DECODER(void) {
    if (__tiQ24RCMP7DECODER == 0) {
        __rtti_user(&__tiQ24RCMP7DECODER, str_Q24RCMP7DECODER);
    }
    return &__tiQ24RCMP7DECODER;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_RCMP_SYSTEM__RCMP_SYSTEM);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern void* RCMP_RCMP_SYSTEM_vtbl[];
extern void operator_delete__FPi(void *a0);
/**
 * Destroys an RCMP::RCMP_SYSTEM: restores its vtable pointer (word 3, RCMP_RCMP_SYSTEM_vtbl, _vt$Q24RCMP11RCMP_SYSTEM) and, when
 * bit 0 of @p dtorFlags is set, frees it with the global operator delete (operator_delete__FPi).
 *
 * @param rcmpSystem  the RCMP_SYSTEM object
 * @param dtorFlags  destructor flags (bit 0: free the object)
 */
void RCMP_RCMP_SYSTEM__RCMP_SYSTEM(struct RCMP_RCMP_SYSTEM *rcmpSystem, int dtorFlags) {
    rcmpSystem->mVtbl = RCMP_RCMP_SYSTEM_vtbl;
    if (dtorFlags & 1) {
        operator_delete__FPi(rcmpSystem);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", __tfQ24RCMP11RCMP_SYSTEM);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int __tiQ24RCMP11RCMP_SYSTEM;
extern char str_Q24RCMP11RCMP_SYSTEM[];
extern void __rtti_user(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::RCMP_SYSTEM (__tiQ24RCMP11RCMP_SYSTEM, named "Q24RCMP11RCMP_SYSTEM"), building it on
 * first use with __rtti_user.
 *
 * @return &__tiQ24RCMP11RCMP_SYSTEM
 */
int *__tfQ24RCMP11RCMP_SYSTEM(void) {
    if (__tiQ24RCMP11RCMP_SYSTEM == 0) {
        __rtti_user(&__tiQ24RCMP11RCMP_SYSTEM, str_Q24RCMP11RCMP_SYSTEM);
    }
    return &__tiQ24RCMP11RCMP_SYSTEM;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", RCMP_AV_MS_TIMER_Update);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
typedef struct { unsigned last; unsigned pad; unsigned long acc; unsigned rate; } sClk;
extern unsigned TIMER_gettick(void);
/**
 * Advances a timer by the ticks elapsed since its last update, scaled by its rate.
 *
 * Reads TIMER_gettick. When rate is 0x1000 (1.0 in 4.12 fixed point) it adds the elapsed ticks (now - last) to the
 * 64-bit acc; otherwise it adds rate * elapsed >> 12, computed in 64 bits. Then stores the new tick in last. Called by
 * func_003AE450 and func_003AE9A0.
 *
 * @param clock  the timer (last tick, accumulated time, rate)
 */
void RCMP_AV_MS_TIMER_Update(sClk *clock) {
    unsigned now = TIMER_gettick();
    if (clock->rate == 0x1000) {
        clock->acc += now - clock->last;
    } else {
        clock->acc += ((unsigned long)clock->rate * (unsigned long)(now - clock->last)) >> 12;
    }
    clock->last = now;
}
#endif
