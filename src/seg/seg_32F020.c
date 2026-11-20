#include "common.h"

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E020);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
extern char D_004953E8[];
extern int *func_003B07F8_x(int a0, int a1, int a2) __asm__("func_003B07F8");
/**
 * Creates a video frame for the Sony codec and links it at the front of the codec's frame list.
 *
 * Allocates 20 bytes from the movie allocator (D_00509430 with an empty name and the memory type at D_00509438), builds
 * the frame with func_003B07F8 (a cleared 32-bit SHAPE of the given size) and links the frame's list node (+8) after
 * the list head at @p sonyCodec + 0x20 (word 8). Likely bug: when the allocation fails the null node is still linked.
 *
 * @param sonyCodec  the PS2_SONY_CODEC_INTERNAL object
 * @param frameWidth  frame width in pixels
 * @param frameHeight  frame height in pixels
 */
void func_0042E020(int *sonyCodec, int frameWidth, int frameHeight) {
    int *n = func_003B07F8_x(D_00509430((int)D_004953E8, 20, 0, 0, ((int *)&D_00509430)[2]), frameWidth, frameHeight);
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
void func_0042E0B0(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E0B8);
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
long func_0042E0B8(int allocSize, long value) {
    return value;
}
#endif

/**
 * Does nothing (an empty placement delete of the Sony codec class).
 */
void func_0042E0C0(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E0C8);
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
long func_0042E0C8(int allocSize, long value) {
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E0D0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees a Sony codec array through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int func_0042E0D0(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E0F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees a Sony codec object through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int func_0042E0F8(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E120);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
/**
 * Allocates a Sony codec array with the movie allocator, naming the block.
 *
 * Calls the allocation function D_00509430 with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second
 * (cMoviePlayer_SSXAllocMemFunc in this game).
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void func_0042E120(int allocSize, int blockName, int alignment, int hdrSize, int memType) {
    D_00509430(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E150);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
/**
 * Allocates a Sony codec object with the movie allocator, naming the block.
 *
 * Calls the allocation function D_00509430 with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void func_0042E150(int allocSize, int blockName, int alignment, int hdrSize, int memType) {
    D_00509430(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E180);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
extern char D_004953E8[];
/**
 * Allocates a Sony codec array with the movie allocator: D_00509430 with an empty name (D_004953E8), @p allocSize, alignment
 * 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void func_0042E180(int allocSize) {
    D_00509430((int)D_004953E8, allocSize, 0, 0, ((int *)&D_00509430)[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E1C0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
extern char D_004953E8[];
/**
 * Allocates a Sony codec object with the movie allocator: D_00509430 with an empty name (D_004953E8), @p allocSize, alignment
 * 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void func_0042E1C0(int allocSize) {
    D_00509430((int)D_004953E8, allocSize, 0, 0, ((int *)&D_00509430)[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E200);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_0053EA00;
extern int D_004A5BA0;
extern char str_Q24RCMP5CODEC[];
extern char str_23PS2_SONY_CODEC_INTERNAL[];
extern void func_00414C20(void *a0, void *a1);
extern void func_00414C00(void *a0, void *a1, void *a2);
/**
 * Returns the run-time type descriptor of PS2_SONY_CODEC_INTERNAL, building it on first use.
 *
 * When D_0053EA00 is still empty it first builds the base class descriptor D_004A5BA0 ("Q24RCMP5CODEC", func_00414C20)
 * if needed, then D_0053EA00 itself as a single-base descriptor named "23PS2_SONY_CODEC_INTERNAL" (func_00414C00).
 *
 * @return &D_0053EA00
 */
int *func_0042E200(void) {
    if (D_0053EA00 == 0) {
        if (D_004A5BA0 == 0) {
            func_00414C20(&D_004A5BA0, str_Q24RCMP5CODEC);
        }
        func_00414C00(&D_0053EA00, str_23PS2_SONY_CODEC_INTERNAL, &D_004A5BA0);
    }
    return &D_0053EA00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E278);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00456810[];
extern int (*D_00509434[])(void);
/**
 * Destroys an RCMP::CODEC: restores its vtable (D_00456810, _vt$Q24RCMP5CODEC) and, when bit 0 of @p dtorFlags is set, frees
 * it through the movie allocator's free function (D_00509434).
 *
 * @param codecObj  the codec object
 * @param dtorFlags  destructor flags (bit 0: free the object)
 */
void func_0042E278(int *codecObj, int dtorFlags) {
    *codecObj = (int)D_00456810;
    if (dtorFlags & 1) {
        D_00509434[0]();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E2B0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A5BA0;
extern char str_Q24RCMP5CODEC[];
extern void func_00414C20(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::CODEC (D_004A5BA0, named "Q24RCMP5CODEC"), building it on first use
 * with func_00414C20.
 *
 * @return &D_004A5BA0
 */
int *func_0042E2B0(void) {
    if (D_004A5BA0 == 0) {
        func_00414C20(&D_004A5BA0, str_Q24RCMP5CODEC);
    }
    return &D_004A5BA0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E2F0);
#ifdef SKIP_ASM
/**
 * Returns the address of a decoder's codec instance data (@p decoderObj + 4).
 *
 * @param decoderObj  the RCMP::DECODER
 * @return @p decoderObj + 4
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_0042E2F0(int decoderObj) {
    return decoderObj + 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E2F8);
#ifdef SKIP_ASM
/**
 * Returns whether a decoder has a codec chosen (word 5, +0x14, non-zero).
 *
 * @param decoderObj  the RCMP::DECODER
 * @return 1 when a codec is set, else 0
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int func_0042E2F8(int *decoderObj) {
    return decoderObj[5] != 0;
}
#endif

/**
 * Does nothing (an empty placement array delete of RCMP::DECODER).
 */
void func_0042E308(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E310);
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
long func_0042E310(int allocSize, long value) {
    return value;
}
#endif

/**
 * Does nothing (an empty placement delete of RCMP::DECODER).
 */
void func_0042E318(void) {
}

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E320);
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
long func_0042E320(int allocSize, long value) {
    return value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E328);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees an RCMP::DECODER array through the movie allocator's free function (D_00509434), passing the pointer argument
 * on unchanged.
 *
 * @return the free function's result
 */
int func_0042E328(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E350);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509434[])(void);
/**
 * Frees an RCMP::DECODER through the movie allocator's free function (D_00509434), passing the pointer argument on
 * unchanged.
 *
 * @return the free function's result
 */
int func_0042E350(void) {
    return D_00509434[0]();
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E378);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
/**
 * Allocates an RCMP::DECODER array with the movie allocator, naming the block.
 *
 * Calls the allocation function D_00509430 with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void func_0042E378(int allocSize, int blockName, int alignment, int hdrSize, int memType) {
    D_00509430(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E3A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
/**
 * Allocates an RCMP::DECODER with the movie allocator, naming the block.
 *
 * Calls the allocation function D_00509430 with (@p blockName, @p allocSize, @p alignment, @p hdrSize, @p memType), i.e. name first and size second.
 *
 * @param allocSize  bytes requested
 * @param blockName  block name
 * @param alignment  alignment
 * @param hdrSize  header bytes
 * @param memType  memory type and flags
 */
void func_0042E3A8(int allocSize, int blockName, int alignment, int hdrSize, int memType) {
    D_00509430(blockName, allocSize, alignment, hdrSize, memType);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E3D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
extern char D_004953B8[];
/**
 * Allocates an RCMP::DECODER array with the movie allocator: D_00509430 with an empty name (D_004953B8), @p allocSize,
 * alignment 0, header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void func_0042E3D8(int allocSize) {
    D_00509430((int)D_004953B8, allocSize, 0, 0, ((int *)&D_00509430)[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E418);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int (*D_00509430)(int a0, int a1, int a2, int a3, int a4);
extern char D_004953B8[];
/**
 * Allocates an RCMP::DECODER with the movie allocator: D_00509430 with an empty name (D_004953B8), @p allocSize, alignment 0,
 * header 0 and the memory type at D_00509438.
 *
 * @param allocSize  bytes requested
 */
void func_0042E418(int allocSize) {
    D_00509430((int)D_004953B8, allocSize, 0, 0, ((int *)&D_00509430)[2]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E458);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A5B90;
extern char str_Q24RCMP7DECODER[];
extern void func_00414C20(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::DECODER (D_004A5B90, named "Q24RCMP7DECODER"), building it on first use
 * with func_00414C20.
 *
 * @return &D_004A5B90
 */
int *func_0042E458(void) {
    if (D_004A5B90 == 0) {
        func_00414C20(&D_004A5B90, str_Q24RCMP7DECODER);
    }
    return &D_004A5B90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E498);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_00456890[];
extern void operator_delete__FPi(int *a0);
/**
 * Destroys an RCMP::RCMP_SYSTEM: restores its vtable pointer (word 3, D_00456890, _vt$Q24RCMP11RCMP_SYSTEM) and, when
 * bit 0 of @p dtorFlags is set, frees it with the global operator delete (operator_delete__FPi).
 *
 * @param rcmpSystem  the RCMP_SYSTEM object
 * @param dtorFlags  destructor flags (bit 0: free the object)
 */
void func_0042E498(int *rcmpSystem, int dtorFlags) {
    rcmpSystem[3] = (int)D_00456890;
    if (dtorFlags & 1) {
        operator_delete__FPi(rcmpSystem);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E4C8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A5B98;
extern char str_Q24RCMP11RCMP_SYSTEM[];
extern void func_00414C20(void *a0, void *a1);
/**
 * Returns the run-time type descriptor of RCMP::RCMP_SYSTEM (D_004A5B98, named "Q24RCMP11RCMP_SYSTEM"), building it on
 * first use with func_00414C20.
 *
 * @return &D_004A5B98
 */
int *func_0042E4C8(void) {
    if (D_004A5B98 == 0) {
        func_00414C20(&D_004A5B98, str_Q24RCMP11RCMP_SYSTEM);
    }
    return &D_004A5B98;
}
#endif

//100%
INCLUDE_ASM("seg/seg_32F020", func_0042E508);
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
void func_0042E508(sClk *clock) {
    unsigned now = TIMER_gettick();
    if (clock->rate == 0x1000) {
        clock->acc += now - clock->last;
    } else {
        clock->acc += ((unsigned long)clock->rate * (unsigned long)(now - clock->last)) >> 12;
    }
    clock->last = now;
}
#endif
