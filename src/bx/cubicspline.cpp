#include "common.h"
#include "bx/memblockman.h"
#include "bx/cubicspline.h"
#include "bx/dynamicmemman.h"

//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_initCommon);
#ifdef SKIP_ASM
void cCubicSplineInterpolant_purge(void* self);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_cCSICubicSpline[];

struct sCubicSplineFlags {
    unsigned int Type : 1;
    unsigned int Initialized : 1;
    unsigned int Loaded : 1;
    unsigned int LoadingIndex : 8;
    unsigned int NumSplines : 8;
};

struct sCubicSpline {
    sCubicSplineFlags flags;
    float MaxX; // cCubicSplineInterpolant::MaxX
    void* data;
};

// PORT: binds the real 2-arg body; the unit declares a 1-arg prototype used by callers
extern "C" void cCubicSplineInterpolant_initCommon_impl(sCubicSpline* self, int n) __asm__("cCubicSplineInterpolant_initCommon");
/**
 * Prepares the spline for @p n points: frees any old data and allocates n - 1 segment records.
 *
 * Purges the old data (cCubicSplineInterpolant_purge), stores n - 1 in the 8-bit count field, allocates (n - 1) * 0x14 bytes
 * ("cCSICubicSpline"), sets the allocated flag (bit 1) and resets the added-point counter (bits 3-10).
 *
 * @param n  number of points (On Tour numPoints); at most 256 fit the count field
 */
extern "C" void cCubicSplineInterpolant_initCommon_impl(sCubicSpline* self, int n)
{
    if (self->data != 0) {
        cCubicSplineInterpolant_purge(self);
    }
    self->flags.NumSplines = n - 1;
    self->data = operator_new_tag(self->flags.NumSplines * 20, str_cCSICubicSpline, 0, 0);
    self->flags.Initialized = 1;
    self->flags.LoadingIndex = 0;
}
#endif

extern "C" void cCubicSplineInterpolant_initCommon(void* self);

//99.62%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_initAsNatural__FPv);
#ifdef SKIP_ASM
/**
 * Prepares a spline interpolant as a natural spline: cCubicSplineInterpolant_initCommon, then clears the type bit (bit
 * 0).
 *
 * The point count is the second argument, which the C does not declare; it reaches cCubicSplineInterpolant_initCommon
 * untouched in its register. The spline camera algorithm builds its interpolants with it.
 */
void cCubicSplineInterpolant::initAsNatural()
{
    cCubicSplineInterpolant_initCommon(this);
    *(int*)this &= -2;
}
#endif

//99.76%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_initAsClamped__FPvff);
#ifdef SKIP_ASM
/**
 * Prepares a spline interpolant as a clamped spline with given end slopes.
 *
 * Runs cCubicSplineInterpolant_initCommon (the point count is the undeclared second argument, passed through in its
 * register), sets the type bit (bit 0), and parks @p start in the first segment's fifth float (+0x10 of the data, +0x8)
 * and @p end in +0x4 until cCubicSplineInterpolant_calculateSplines reads them.
 *
 * @param start  the slope at the first point
 * @param end  the slope at the last point
 */
void cCubicSplineInterpolant_initAsClamped(void* vself, float start, float end)
{
    cCubicSplineInterpolant* self = (cCubicSplineInterpolant*)vself;
    cCubicSplineInterpolant_initCommon(self);
    unsigned int flags = *(unsigned int*)self;
    cCSICubicSpline* ptr = self->CubicSplines;
    flags |= 1;
    *(unsigned int*)self = flags;
    ptr->X0 = start;
    self->MaxX = end;
}
#endif

void* cMemMan_free(void* ptr);

//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_purge__FPv);
#ifdef SKIP_ASM
/**
 * Frees a spline interpolant's segment data (+0x8, cMemMan_free) and clears its allocated and loaded flags (bits 1 and
 * 2).
 *
 * cCubicSplineInterpolant_initCommon calls it before allocating new data.
 */
void cCubicSplineInterpolant_purge(void* vself)
{
    cCubicSplineInterpolant* self = (cCubicSplineInterpolant*)vself;
    if (self->CubicSplines != 0) {
        cMemMan_free(self->CubicSplines);
        self->CubicSplines = 0;
    }
    int flags = *(int*)self;
    flags &= -3;
    flags &= -5;
    *(int*)self = flags;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_addPoint);
#ifdef SKIP_ASM
// calculateSplines still reads the interpolant through the sCubicSpline view above.
extern "C" void cCubicSplineInterpolant_calculateSplines(sCubicSpline* self);

/**
 * Adds the next point (@p x, @p y) to a spline interpolant; the last point completes it.
 *
 * While the added-point counter (bits 3-10) is below the segment count (bits 11-18), the point goes into that segment's
 * start (+0x0, +0x4 of its 0x14 bytes) and the counter goes up. The point after that becomes the last segment's end
 * (+0x8, +0xC); the coefficients are then computed (cCubicSplineInterpolant_calculateSplines) and the loaded flag (bit 2) is set. The camera code
 * feeds its key times and values through it.
 *
 * @param x  the point's input value (time)
 * @param y  the point's output value
 */
void cCubicSplineInterpolant::addPoint(float x, float y)
{
    // Until calculateSplines runs, segment i holds key i in A, B and the last segment holds the end key in C, D.
    if (LoadingIndex == NumSplines) {
        CubicSplines[LoadingIndex - 1].C = x;
        CubicSplines[LoadingIndex - 1].D = y;
        cCubicSplineInterpolant_calculateSplines((sCubicSpline*)this);
        Loaded = 1;
    } else {
        CubicSplines[LoadingIndex].A = x;
        CubicSplines[LoadingIndex].B = y;
        LoadingIndex++;
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_calculateSplines);
#ifdef SKIP_ASM
/**
 * Computes a spline interpolant's cubic coefficients from its points, natural or clamped by the type bit.
 *
 * Copies the points into local arrays and solves the tridiagonal system for the second-derivative terms: with natural
 * end conditions (type bit 0 clear) or with the end slopes that cCubicSplineInterpolant_initAsClamped parked at +0x10 of the data and at +0x4.
 * Each 0x14-byte segment then holds d, c, b, a and its start x, for y = a + b*u + c*u^2 + d*u^3 with u = t - x.
 *
 * +0x4 becomes the last point's x and the cached segment index (bits 19-26) is cleared. The local arrays hold 256
 * entries. cCubicSplineInterpolant_addPoint calls it after the last point.
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// self->data holds 5 floats per key: x, y, end x, end y, start slope on input;
// d, c, b, a, x of each segment (y = a + b*u + c*u^2 + d*u^3, u = t - x) on output.
// Builds the natural (flags.b0 == 0) or clamped (flags.b0 == 1) cubic spline through the added keys.
extern "C" void cCubicSplineInterpolant_calculateSplines(sCubicSpline* self)
{
    float x[256];
    float yValues[256];
    float spans[256];
    float alpha[256];
    float diagonal[256];
    float mu[256];
    float z[256];
    float linCoefs[256];
    float quadCoefs[256];
    float cubicCoefs[256];
    int n = self->flags.NumSplines;
    int i;
    for (i = 0; i < n; i++) {
        x[i] = ((float*)self->data)[i * 5 + 0];
        yValues[i] = ((float*)self->data)[i * 5 + 1];
    }
    x[n] = ((float*)self->data)[(n - 1) * 5 + 2];
    yValues[n] = ((float*)self->data)[(n - 1) * 5 + 3];
    for (i = 0; i < n; i++) {
        spans[i] = x[i + 1] - x[i];
    }
    switch (self->flags.Type) {
    case cCubicSplineInterpolant::kNatural:
        for (i = 1; i < n; i++) {
            alpha[i] = 3.0f / spans[i] * (yValues[i + 1] - yValues[i]) - 3.0f / spans[i - 1] * (yValues[i] - yValues[i - 1]);
        }
        diagonal[0] = 1.0f;
        mu[0] = 0.0f;
        z[0] = 0.0f;
        for (i = 1; i < n; i++) {
            diagonal[i] = 2.0f * (x[i + 1] - x[i - 1]) - spans[i - 1] * mu[i - 1];
            mu[i] = spans[i] / diagonal[i];
            z[i] = (alpha[i] - spans[i - 1] * z[i - 1]) / diagonal[i];
        }
        diagonal[n] = 1.0f;
        z[n] = 0.0f;
        quadCoefs[n] = 0.0f;
        for (i = n - 1; i >= 0; i--) {
            quadCoefs[i] = z[i] - mu[i] * quadCoefs[i + 1];
            linCoefs[i] = (yValues[i + 1] - yValues[i]) / spans[i] - spans[i] * (quadCoefs[i + 1] + 2.0f * quadCoefs[i]) * 0.33333334f;
            cubicCoefs[i] = (quadCoefs[i + 1] - quadCoefs[i]) / (3.0f * spans[i]);
        }
        break;
    case cCubicSplineInterpolant::kClamped:
        alpha[0] = 3.0f * (yValues[1] - yValues[0]) / spans[0] - 3.0f * ((float*)self->data)[4];
        alpha[n] = 3.0f * self->MaxX - 3.0f * (yValues[n] - yValues[n - 1]) / spans[n - 1];
        for (i = 1; i < n; i++) {
            alpha[i] = 3.0f / spans[i] * (yValues[i + 1] - yValues[i]) - 3.0f / spans[i - 1] * (yValues[i] - yValues[i - 1]);
        }
        diagonal[0] = 2.0f * spans[0];
        mu[0] = 0.5f;
        z[0] = alpha[0] / diagonal[0];
        for (i = 1; i < n; i++) {
            diagonal[i] = 2.0f * (x[i + 1] - x[i - 1]) - spans[i - 1] * mu[i - 1];
            mu[i] = spans[i] / diagonal[i];
            z[i] = (alpha[i] - spans[i - 1] * z[i - 1]) / diagonal[i];
        }
        diagonal[n] = spans[n - 1] * (2.0f - mu[n - 1]);
        z[n] = (alpha[n] - spans[n - 1] * z[n - 1]) / diagonal[n];
        quadCoefs[n] = z[n];
        for (i = n - 1; i >= 0; i--) {
            quadCoefs[i] = z[i] - mu[i] * quadCoefs[i + 1];
            linCoefs[i] = (yValues[i + 1] - yValues[i]) / spans[i] - spans[i] * (quadCoefs[i + 1] + 2.0f * quadCoefs[i]) * 0.33333334f;
            cubicCoefs[i] = (quadCoefs[i + 1] - quadCoefs[i]) / (3.0f * spans[i]);
        }
        break;
    }
    for (i = 0; i < n; i++) {
        ((float*)self->data)[i * 5 + 0] = cubicCoefs[i];
        ((float*)self->data)[i * 5 + 1] = quadCoefs[i];
        ((float*)self->data)[i * 5 + 2] = linCoefs[i];
        ((float*)self->data)[i * 5 + 3] = yValues[i];
        ((float*)self->data)[i * 5 + 4] = x[i];
    }
    self->MaxX = x[n];
    *(unsigned int*)&self->flags &= 0xF807FFFF;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_evaluate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" float cCSICubicSpline_evaluate(cCSICubicSpline* seg, float t);

extern "C" float cCubicSplineInterpolant_evaluate_impl(cCubicSplineInterpolant* s, float t)
    __asm__("cCubicSplineInterpolant_evaluate");
/**
 * Evaluates a spline interpolant at @p time.
 *
 * Uses the cached segment (bits 19-26) when @p time lies in it, else its neighbour, else a bisection over the segments,
 * caches the one found and evaluates its cubic with cCSICubicSpline_evaluate. Times before the first segment use segment 0; when
 * the cached segment is the second-to-last, times past the end use the last.
 *
 * Likely bug: a time past the end (+0x4) while the cached segment is any other one reads past the segment array
 * (seg[cur + 2]) or never leaves the bisection, which keeps landing on the last segment.
 *
 * @param spline  the interpolant
 * @param time  the input value
 * @return the interpolated value
 */
extern "C" float cCubicSplineInterpolant_evaluate_impl(cCubicSplineInterpolant* spline, float time)
{
    int last = spline->NumSplines - 1;
    float* pend;
    if (last == (int)spline->LastEvaluatedIndex) {
        pend = &spline->MaxX;
    } else {
        pend = &spline->CubicSplines[spline->LastEvaluatedIndex + 1].X0;
    }
    cCSICubicSpline* seg = &spline->CubicSplines[spline->LastEvaluatedIndex];
    if (seg->X0 <= time && time <= *pend) {
        return cCSICubicSpline_evaluate(seg, time);
    }
    if (time < seg->X0) {
        if (time < spline->CubicSplines[1].X0) {
            spline->LastEvaluatedIndex = 0;
            return cCSICubicSpline_evaluate(spline->CubicSplines, time);
        }
    } else {
        if ((int)spline->LastEvaluatedIndex == last - 1) {
            spline->LastEvaluatedIndex = last;
            return cCSICubicSpline_evaluate(&spline->CubicSplines[last], time);
        }
        if (time < spline->CubicSplines[spline->LastEvaluatedIndex + 2].X0) {
            spline->LastEvaluatedIndex = spline->LastEvaluatedIndex + 1;
            return cCSICubicSpline_evaluate(&spline->CubicSplines[spline->LastEvaluatedIndex], time);
        }
    }
    int hi = last;
    int lo = 0;
    for (;;) {
        int mid = (hi + lo) / 2;
        cCSICubicSpline* midSeg = &spline->CubicSplines[mid];
        float segStart = midSeg->X0;
        float segEnd;
        if (mid < last) {
            segEnd = midSeg[1].X0;
        } else {
            segEnd = spline->MaxX;
        }
        if (segStart <= time) {
            if (time <= segEnd) {
                spline->LastEvaluatedIndex = mid;
                return cCSICubicSpline_evaluate(midSeg, time);
            } else {
                lo = mid + 1;
            }
        } else {
            hi = mid;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_SpewGraphPoints);
#ifdef SKIP_ASM
extern "C" void cCubicSplineInterpolant_evaluate(void* self, float t);

/**
 * Steps through a spline interpolant's range in 99 steps, evaluating it at each one and discarding the values.
 *
 * The range runs from the first segment's start x (+0x10 of the data) to the end (+0x4). A first loop over the same
 * steps is empty; this is what remains of a debug dump. Nothing calls it.
 */
void cCubicSplineInterpolant::SpewGraphPoints()
{
    float end = MaxX;
    float start = CubicSplines->X0;
    float step = (end - start) * 0.010101010091602802f;
    for (float firstPassT = start; firstPassT <= end; firstPassT += step) {
    }
    for (float sampleT = start; sampleT <= end; sampleT += step) {
        cCubicSplineInterpolant_evaluate(this, sampleT);
    }
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cCubicSplineInterpolant_SpewSplineNumbers);
#ifdef SKIP_ASM
// Number of spline segments (flags.count, bits 11..18); the empty loop is a stripped debug walk.
static inline int cCubicSpline_getCount(sCubicSpline* s) { return s->flags.NumSplines; }

/**
 * Loops over a spline interpolant's segments (count in bits 11-18) doing nothing.
 *
 * An empty loop, what remains of a debug dump. Nothing calls it.
 */
void cCubicSplineInterpolant::SpewSplineNumbers()
{
    for (int i = 0; i < cCubicSpline_getCount(((sCubicSpline*)this)); i++) {
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", largebin_index);
#ifdef SKIP_ASM
/**
 * Returns the large-bin index (32 to 95) for a chunk size, from its highest set bit and the next two bits.
 *
 * Sizes of 0x1000000 or more give 95. cDynamicMemMan_dlmalloc uses it to find the bin to search and to file chunks.
 *
 * @param sz  the chunk size in bytes
 * @return the bin index
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// dlmalloc 2.7 largebin_index() (portable highest-bit version).
extern "C" int largebin_index(unsigned int sz)
{
    unsigned int x = sz >> 8;
    unsigned int shift;
    if (x >= 0x10000)
        return 95;
    {
        unsigned int n = ((x - 0x100) >> 16) & 8;
        x <<= n;
        shift = ((x - 0x1000) >> 16) & 4;
        n += shift;
        x <<= shift;
        shift = ((x - 0x4000) >> 16) & 2;
        n += shift;
        x = (x << shift) >> 14;
        shift = 13 - n + (x & ~(x >> 1));
    }
    return 32 + (shift << 2) + ((sz >> (shift + 6)) & 3);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", malloc_init_state__FPv);
#ifdef SKIP_ASM
/**
 * Initialises a malloc state: empty circular bins, default limits, and the top chunk at the first bin.
 *
 * Points both links of the 95 bins (from +0x38) at themselves, sets the fastbin limit (+0x0) to 0x50 keeping its low
 * two flag bits, trim threshold 0x8000 (+0x344), mmap threshold 0x40000 (+0x34C), top pad and mmap count 0 (+0x348,
 * +0x354), page size 0x1000 (+0x35C), the contiguous flag (bit 0 of +0x360) and the top (+0x30) to the first bin.
 * cDynamicMemMan_malloc_consolidate calls it on a state whose fastbin limit is still 0.
 */
void malloc_init_state(void* vself)
{
    // ORIGINAL-PTR: dlmalloc 2.7's bin_at: each bin is a fake chunk 8 bytes before its bins[] pair, so its fd / bk
    // land on the pair; the bins are walked in 8-byte steps (a byte heap)
    malloc_state* self = (malloc_state*)vself;
    char* bin = (char*)&self->bins[0];
    for (int i = 94; i >= 0; i--) {
        *(void**)(bin + 0xC) = bin;
        *(void**)(bin + 0x8) = bin;
        bin += 8;
    }
    self->mmap_threshold = 0x40000;
    self->trim_threshold = 0x8000;
    *(int*)&self->morecore_properties |= 1;
    *(int*)self = (*(int*)self & 3) | 0x50;
    self->top = (malloc_chunk*)(&self->bins[0]);
    self->top_pad = 0;
    self->n_mmaps_max = 0;
    self->pagesize = 0x1000;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_sYSMALLOc);
#ifdef SKIP_ASM
// dlmalloc 2.7 sYSMALLOc()
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" unsigned int cDynamicMemMan_dlmorecore(cDynamicMemMan* self, long incr); // PORT: 64-bit long param
extern "C" void* cDynamicMemMan_dlmalloc(cDynamicMemMan* ms, unsigned int bytes);
struct sMState_31EEE8;
extern "C" void cDynamicMemMan_malloc_consolidate(cDynamicMemMan* ms, malloc_state* av);
// PORT: the unit declares cDynamicMemMan_dlfree(void*); the body really takes (ms, mem).
extern "C" void cDynamicMemMan_dlfree_E2D8(void* ms, void* mem) __asm__("cDynamicMemMan_dlfree");
extern "C" int* __errno();



/**
 * Gets more memory for the heap from its core (cDynamicMemMan_dlmorecore) and allocates a chunk of @p nb bytes from the grown top.
 *
 * When fast chunks exist (bit 1 of +0x0) it consolidates them (cDynamicMemMan_malloc_consolidate) and retries cDynamicMemMan_dlmalloc instead.
 * Otherwise it asks cDynamicMemMan_dlmorecore for nb + top pad + 16 bytes rounded to the page (less the old top when the core is
 * contiguous), fixes up a foreign or misaligned break with a second request, frees the old top when the new memory is
 * not adjacent, and updates sbrked_mem and its maxima.
 *
 * Returns the chunk carved from the new top, or sets *__errno() to 12 (ENOMEM) and returns null. cDynamicMemMan_dlmalloc
 * ends with it.
 *
 * @param nb  the padded chunk size needed
 * @param av  the malloc state
 * @return the new memory, or null
 */
void* cDynamicMemMan::sYSMALLOc(unsigned int nb, malloc_state* av)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    malloc_chunk* old_top;
    unsigned int old_size;
    char* old_end;
    long size;
    char* brk;
    long correction;
    char* snd_brk;
    unsigned int front_misalign;
    unsigned int end_misalign;
    char* aligned_brk;
    malloc_chunk* chunk;
    malloc_chunk* remainder;
    unsigned int remainder_size;
    unsigned int sum;
    unsigned int pagemask = av->pagesize - 1;

    if (av->max_fast & 2) {
        malloc_consolidate((malloc_state*)av);
        return dlmalloc(nb - 15);
    }

    old_top = av->top;
    old_size = old_top->size & ~3U;
    old_end = (char*)old_top + old_size;

    brk = snd_brk = (char*)0xFFFFFFFF;

    size = nb + av->top_pad + 16;
    if (av->morecore_properties & 1)
        size -= old_size;
    size = (size + pagemask) & ~pagemask;
    if (size > 0)
        brk = (char*)this->dlmorecore(size);

    if (brk != (char*)0xFFFFFFFF) {
        av->sbrked_mem += size;

        if (brk == old_end && snd_brk == (char*)0xFFFFFFFF) {
            old_top->size = (size + old_size) | 1;
        } else {
            front_misalign = 0;
            end_misalign = 0;
            correction = 0;
            aligned_brk = brk;

            if ((av->morecore_properties & 1) && old_size != 0 && brk < old_end) {
                av->morecore_properties &= ~1U;
            }

            if (av->morecore_properties & 1) {
                if (old_size != 0)
                    av->sbrked_mem += brk - old_end;

                front_misalign = (unsigned int)(brk + 8) & 15;
                if (front_misalign > 0) {
                    correction = 16 - front_misalign;
                    aligned_brk += correction;
                }

                correction += old_size;

                end_misalign = (unsigned int)(brk + size + correction);
                correction += ((end_misalign + pagemask) & ~pagemask) - end_misalign;

                snd_brk = (char*)this->dlmorecore(correction);

                if (snd_brk == (char*)0xFFFFFFFF) {
                    correction = 0;
                    snd_brk = (char*)this->dlmorecore(0);
                } else if (snd_brk < brk) {
                    snd_brk = brk + size;
                    correction = 0;
                    av->morecore_properties &= ~1U;
                }
            } else {
                if (snd_brk == (char*)0xFFFFFFFF) {
                    snd_brk = (char*)this->dlmorecore(0);
                    av->sbrked_mem += snd_brk - brk - size;
                }
            }

            if (snd_brk != (char*)0xFFFFFFFF) {
                (*(malloc_chunk**)&av->top) = (malloc_chunk*)aligned_brk;
                av->top->size = (snd_brk - aligned_brk + correction) | 1;
                av->sbrked_mem += correction;

                if (old_size != 0) {
                    old_size = (old_size - 12) & ~15U;
                    old_top->size = old_size | 1;
                    ((malloc_chunk*)((char*)old_top + old_size))->size = 5;
                    ((malloc_chunk*)((char*)old_top + old_size + 4))->size = 5;
                    if (old_size >= 16) {
                        unsigned int tt = av->trim_threshold;
                        av->trim_threshold = 0xFFFFFFFF;
                        cDynamicMemMan_dlfree_E2D8(this, &old_top->fd);
                        av->trim_threshold = tt;
                    }
                }
            }
        }

        sum = av->sbrked_mem;
        if (sum > av->max_sbrked_mem)
            av->max_sbrked_mem = sum;
        sum += av->mmapped_mem;
        if (sum > av->max_total_mem)
            av->max_total_mem = sum;

        chunk = av->top;
        size = chunk->size & ~3U;
        if ((unsigned int)size >= (unsigned int)(nb + 16)) {
            remainder_size = size - nb;
            remainder = (malloc_chunk*)((char*)chunk + nb);
            (*(malloc_chunk**)&av->top) = remainder;
            chunk->size = nb | 1;
            remainder->size = remainder_size | 1;
            return (char*)&chunk->fd;
        }
    }

    *__errno() = 12;
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_sYSTRIm);
#ifdef SKIP_ASM
extern "C" unsigned int cDynamicMemMan_dlmorecore(cDynamicMemMan* self, long incr); // PORT: 64-bit long param
// PORT: libgcc 64-bit helpers called by name so the relocations match
// (__divdi3 / __muldi3 in the retail ELF's unnamed libgcc copy).
extern "C" long __divdi3(long a, long b);
extern "C" long __muldi3(long a, long b);



/**
 * Gives unused memory at the top of the heap back to its core (cDynamicMemMan_dlmorecore with a negative size), keeping @p topPad
 * bytes.
 *
 * Releases whole pages only, and only when the core's current end is still the top chunk's end; updates the top size
 * and sbrked_mem.
 *
 * @param topPad  bytes to keep above what is in use
 * @param av  the malloc state
 * @return true when memory was released
 */
bool cDynamicMemMan::sYSTRIm(unsigned int topPad, malloc_state* av)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    long top_size;
    long extra;
    long released;
    char* current_brk;
    char* new_brk;
    unsigned int pagesz;

    pagesz = av->pagesize;
    top_size = av->top->size & ~3;
    extra = __muldi3(__divdi3(top_size - topPad - 16 + (pagesz - 1), pagesz) - 1, pagesz);
    if (extra > 0) {
        current_brk = (char*)this->dlmorecore(0);
        if (current_brk == (char*)(av->top) + top_size) {
            this->dlmorecore(-extra);
            new_brk = (char*)this->dlmorecore(0);
            if (new_brk != (char*)0xFFFFFFFF) {
                released = current_brk - new_brk;
                if (released != 0) {
                    av->sbrked_mem -= released;
                    av->top->size = (top_size - released) | 1;
                    return true;
                }
            }
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmalloc);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// dlmalloc 2.7.0 malloc()
extern "C" int largebin_index(unsigned int sz);
extern "C" void cDynamicMemMan_malloc_consolidate(cDynamicMemMan* ms, malloc_state* av);
extern "C" void* cDynamicMemMan_sYSMALLOc(cDynamicMemMan* ms, unsigned int nb, malloc_state* av);

#define bin_at_E818(av, i) ((malloc_chunk*)((char*)&(av)->bins[(i) << 1] - 8))

/**
 * Allocates @p bytes from the heap (dlmalloc 2.7's malloc on 16-byte aligned chunks).
 *
 * Sizes of 0xFFFFFFE0 or more set *__errno() to 12 (ENOMEM) and return null. Otherwise it tries the fast bins,
 * the small bins, the unsorted chunks (filing them into their bins, largebin_index for large ones), the best-fitting
 * larger bin through the bin map, then the top chunk, and finally grows the heap with cDynamicMemMan_sYSMALLOc. Every other
 * allocator here (cDynamicMemMan_alloc, calloc, realloc, memalign, cDynamicMemMan_iALLOc) goes through it.
 *
 * @param bytes  the size requested
 * @return the memory, or null
 */
void* cDynamicMemMan::dlmalloc(unsigned int bytes)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, chunk2mem, and bin_at's
    // fake chunk 8 bytes before each bins[] pair): a byte heap
    malloc_state* av = MallocState;
    unsigned int nb;
    unsigned int idx;
    malloc_chunk* bin;
    malloc_chunk** fb;
    malloc_chunk* victim;
    unsigned int size;
    int victim_index;
    malloc_chunk* remainder;
    unsigned int remainder_size;
    unsigned int block;
    unsigned int bit;
    unsigned int map;
    malloc_chunk* fwd;
    malloc_chunk* bck;

    if (bytes >= (unsigned int)-32) {
        *__errno() = 12;
        return 0;
    }
    nb = (bytes + 4 + 15 < 16) ? 16 : (bytes + 4 + 15) & ~15U;

    if (!(av->max_fast & 1)) {
        if (av->max_fast == 0)
            malloc_consolidate(av);
        goto use_top;
    }

    if (nb <= av->max_fast) {
        fb = &av->fastbins[(nb >> 3) - 2];
        if ((victim = *fb) != 0) {
            *fb = victim->fd;
            return (char*)victim + 8;
        }
    }

    if (nb < 256) {
        idx = nb >> 3;
        malloc_chunk** base = &av->bins[idx << 1];
        bin = (malloc_chunk*)((char*)base - 8);
        if ((victim = base[1]) != bin) {
            bck = victim->bk;
            ((malloc_chunk*)((char*)victim + nb))->size |= 1;
            base[1] = bck;
            bck->fd = bin;
            return (char*)victim + 8;
        }
    } else {
        idx = largebin_index(nb);
        if (av->max_fast & 2)
            malloc_consolidate(av);
    }

    while ((victim = bin_at_E818(av, 1)->bk) != bin_at_E818(av, 1)) {
        bck = victim->bk;
        size = victim->size & ~3U;

        if (nb < 256 && bck == bin_at_E818(av, 1) && victim == av->last_remainder && size > nb + 16) {
            remainder_size = size - nb;
            remainder = (malloc_chunk*)((char*)victim + nb);
            av->bins[3] = av->bins[2] = remainder;
            av->last_remainder = remainder;
            remainder->bk = remainder->fd = bin_at_E818(av, 1);
            victim->size = nb | 1;
            remainder->size = remainder_size | 1;
            ((malloc_chunk*)((char*)remainder + remainder_size))->prev_size = remainder_size;
            return (char*)victim + 8;
        }

        bin_at_E818(av, 1)->bk = bck;
        bck->fd = bin_at_E818(av, 1);

        if (size == nb) {
            ((malloc_chunk*)((char*)victim + size))->size |= 1;
            return (char*)victim + 8;
        }

        if (size < 256) {
            victim_index = size >> 3;
            malloc_chunk** base = &av->bins[victim_index << 1];
            bck = (malloc_chunk*)((char*)base - 8);
            fwd = base[0];
        } else {
            victim_index = largebin_index(size);
            malloc_chunk** base = &av->bins[victim_index << 1];
            bck = (malloc_chunk*)((char*)base - 8);
            fwd = base[0];
            if (fwd != bck) {
                if (size < bck->bk->size) {
                    fwd = bck;
                    bck = base[1];
                } else if (size >= 256) {
                    size |= 1;
                    while (size < fwd->size)
                        fwd = fwd->fd;
                    bck = fwd->bk;
                }
            }
        }

        av->binmap[victim_index >> 5] |= 1U << (victim_index & 31);
        victim->bk = bck;
        victim->fd = fwd;
        fwd->bk = victim;
        bck->fd = victim;
    }

    if (!(nb < 256)) {
        malloc_chunk** base = &av->bins[idx << 1];
        bin = (malloc_chunk*)((char*)base - 8);
        for (victim = base[1]; victim != bin; victim = victim->bk) {
            size = victim->size & ~3U;
            if (size >= nb) {
                remainder_size = size - nb;
                fwd = victim->fd;
                bck = victim->bk;
                fwd->bk = bck;
                bck->fd = fwd;
                void* mem = (char*)victim + 8;
                if (remainder_size < 16) {
                    ((malloc_chunk*)((char*)victim + size))->size |= 1;
                    return mem;
                } else {
                    remainder = (malloc_chunk*)((char*)victim + nb);
                    av->bins[3] = av->bins[2] = remainder;
                    remainder->bk = remainder->fd = bin_at_E818(av, 1);
                    victim->size = nb | 1;
                    remainder->size = remainder_size | 1;
                    ((malloc_chunk*)((char*)remainder + remainder_size))->prev_size = remainder_size;
                    return mem;
                }
            }
        }
    }

    ++idx;
    bin = bin_at_E818(av, idx);
    block = idx >> 5;
    map = av->binmap[block];
    bit = 1U << (idx & 31);

    for (;;) {
        if (bit > map || bit == 0) {
            do {
                if (++block >= 3)
                    goto use_top;
            } while ((map = av->binmap[block]) == 0);
            bin = bin_at_E818(av, block << 5);
            bit = 1;
        }

        while ((bit & map) == 0) {
            bin = (malloc_chunk*)((char*)bin + 8);
            bit <<= 1;
        }

        victim = bin->bk;
        if (victim == bin) {
            av->binmap[block] = map &= ~bit;
            bin = (malloc_chunk*)((char*)bin + 8);
            bit <<= 1;
        } else {
            size = victim->size & ~3U;
            remainder_size = size - nb;
            bck = victim->bk;
            bin->bk = bck;
            bck->fd = bin;
            if (remainder_size < 16) {
                ((malloc_chunk*)((char*)victim + size))->size |= 1;
                return (char*)victim + 8;
            } else {
                remainder = (malloc_chunk*)((char*)victim + nb);
                av->bins[3] = av->bins[2] = remainder;
                remainder->bk = remainder->fd = bin_at_E818(av, 1);
                if (nb < 256)
                    av->last_remainder = remainder;
                victim->size = nb | 1;
                remainder->size = remainder_size | 1;
                ((malloc_chunk*)((char*)remainder + remainder_size))->prev_size = remainder_size;
                return (char*)victim + 8;
            }
        }
    }

use_top:
    victim = av->top;
    size = victim->size & ~3U;
    if (size >= nb + 16) {
        remainder_size = size - nb;
        remainder = (malloc_chunk*)((char*)victim + nb);
        av->top = remainder;
        victim->size = nb | 1;
        remainder->size = remainder_size | 1;
        return (char*)victim + 8;
    }
    return sYSMALLOc(nb, av);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlfree);
#ifdef SKIP_ASM



struct sMState_31EEE8;
struct sMState_31E6D8;
extern "C" void cDynamicMemMan_malloc_consolidate(cDynamicMemMan* ms, malloc_state* av);
extern "C" bool cDynamicMemMan_sYSTRIm(cDynamicMemMan* ms, unsigned int pad, malloc_state* av);

// PORT: the unit declares cDynamicMemMan_dlfree(void*); the body really takes (ms, mem).
extern "C" void cDynamicMemMan_dlfree_free(cDynamicMemMan* ms, void* mem) __asm__("cDynamicMemMan_dlfree");

/**
 * Releases a heap block back to the dynamic memory manager (dlmalloc's free).
 *
 * Small chunks go to their fast bin; others are merged with free neighbours and put in the unsorted bin, or merged into
 * the top chunk, which may then be trimmed (cDynamicMemMan_sYSTRIm) after consolidating the fast bins (cDynamicMemMan_malloc_consolidate). The C
 * definition is cDynamicMemMan_dlfree, bound by asm label because the unit declares cDynamicMemMan_dlfree with one argument.
 *
 * @param ms  the memory manager (this); holds the malloc state pointer
 * @param mem  the block to free, or null
 */
extern "C" void cDynamicMemMan_dlfree_free(cDynamicMemMan* ms, void* mem)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    malloc_state* av = ms->MallocState;
    malloc_chunk* freedChunk;
    malloc_chunk* nextchunk;
    unsigned int size;
    unsigned int nextsize;
    unsigned int prevsize;
    int nextinuse;
    malloc_chunk* bck;
    malloc_chunk* fwd;
    malloc_chunk** fb;

    // PORT: chunk header reads go through (int)mem - 8 (pointer in int) to match gcc 2.95 offset folding
    if (mem != 0)
    {
        size = ((malloc_chunk*)((int)mem - 8))->size & ~3U;
        freedChunk = (malloc_chunk*)((char*)mem - 8);
        if (size <= av->max_fast)
        {
            av->max_fast |= 3;
            fb = &av->fastbins[(size >> 3) - 2];
            *(malloc_chunk**)mem = *fb;
            *fb = freedChunk;
        }
        else if (!(((malloc_chunk*)((int)mem - 8))->size & 2))
        {
            av->max_fast |= 1;
            nextchunk = (malloc_chunk*)((char*)freedChunk + size);
            nextsize = nextchunk->size & ~3U;
            if (!(((malloc_chunk*)((int)mem - 8))->size & 1))
            {
                prevsize = freedChunk->prev_size;
                size += prevsize;
                freedChunk = (malloc_chunk*)((char*)freedChunk - (long)prevsize);
                fwd = freedChunk->fd;
                bck = freedChunk->bk;
                fwd->bk = bck;
                bck->fd = fwd;
            }
            if (nextchunk != av->top)
            {
                nextinuse = ((malloc_chunk*)((char*)nextchunk + nextsize))->size & 1;
                nextchunk->size = nextsize;
                if (!nextinuse)
                {
                    fwd = nextchunk->fd;
                    bck = nextchunk->bk;
                    fwd->bk = bck;
                    bck->fd = fwd;
                    size += nextsize;
                }
                fwd = ((malloc_chunk*)((char*)&((malloc_chunk**)&av->bins[0])[1 << 1] - 8))->fd;
                freedChunk->bk = (malloc_chunk*)((char*)&((malloc_chunk**)&av->bins[0])[1 << 1] - 8);
                freedChunk->fd = fwd;
                ((malloc_chunk*)((char*)&((malloc_chunk**)&av->bins[0])[1 << 1] - 8))->fd = freedChunk;
                fwd->bk = freedChunk;
                freedChunk->size = size | 1;
                ((malloc_chunk*)((char*)freedChunk + size))->prev_size = size;
            }
            else
            {
                size += nextsize;
                freedChunk->size = size | 1;
                (*(malloc_chunk**)&av->top) = freedChunk;
            }
            if (size >= 0x4000)
            {
                if (av->max_fast & 2)
                    ms->malloc_consolidate((malloc_state*)av);
                if ((av->top->size & ~3U) >= av->trim_threshold)
                    ms->sYSTRIm(av->top_pad, (malloc_state*)av);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_malloc_consolidate);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void malloc_init_state(void* av);



/**
 * Merges every chunk in the fast bins with its free neighbours and moves the results to the unsorted bin or the top.
 *
 * Clears the fast-chunks flag (bit 1 of +0x0) first. A state whose fastbin limit (+0x0) is 0 has never been set up and
 * gets malloc_init_state instead.
 *
 * @param av  the malloc state
 */
void cDynamicMemMan::malloc_consolidate(malloc_state* av)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    malloc_chunk** fb;
    malloc_chunk** maxfb;
    malloc_chunk* chunk;
    malloc_chunk* nextp;
    malloc_chunk* unsorted_bin;
    malloc_chunk* first_unsorted;
    malloc_chunk* nextchunk;
    unsigned int size;
    unsigned int nextsize;
    unsigned int prevsize;
    int nextinuse;
    malloc_chunk* bck;
    malloc_chunk* fwd;

    if (av->max_fast != 0) {
        av->max_fast &= ~2U;
        unsorted_bin = (malloc_chunk*)((char*)&((malloc_chunk**)&av->bins[0])[1 << 1] - 8);
        maxfb = &av->fastbins[(av->max_fast >> 3) - 2];
        fb = &av->fastbins[0];
        do {
            if ((chunk = *fb) != 0) {
                *fb = 0;
                do {
                    nextp = chunk->fd;
                    size = chunk->size & ~1U;
                    nextchunk = (malloc_chunk*)((char*)chunk + size);
                    nextsize = nextchunk->size & ~3U;
                    if (!(chunk->size & 1)) {
                        prevsize = chunk->prev_size;
                        size += prevsize;
                        chunk = (malloc_chunk*)((char*)chunk - (long)prevsize);
                        fwd = chunk->fd;
                        bck = chunk->bk;
                        fwd->bk = bck;
                        bck->fd = fwd;
                    }
                    if (nextchunk != av->top) {
                        nextinuse = ((malloc_chunk*)((char*)nextchunk + nextsize))->size & 1;
                        nextchunk->size = nextsize;
                        if (!nextinuse) {
                            size += nextsize;
                            fwd = nextchunk->fd;
                            bck = nextchunk->bk;
                            fwd->bk = bck;
                            bck->fd = fwd;
                        }
                        first_unsorted = unsorted_bin->fd;
                        unsorted_bin->fd = chunk;
                        first_unsorted->bk = chunk;
                        chunk->size = size | 1;
                        chunk->bk = unsorted_bin;
                        chunk->fd = first_unsorted;
                        ((malloc_chunk*)((char*)chunk + size))->prev_size = size;
                    } else {
                        size += nextsize;
                        chunk->size = size | 1;
                        (*(malloc_chunk**)&av->top) = chunk;
                    }
                } while ((chunk = nextp) != 0);
            }
        } while (fb++ != maxfb);
    } else {
        malloc_init_state(av);
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlrealloc);
#ifdef SKIP_ASM

extern "C" void MEM_copy(void* d, void* s, int n);

/**
 * Resizes an allocation (dlmalloc 2.7's realloc), in place when it can.
 *
 * A null @p oldmem allocates with cDynamicMemMan_dlmalloc. Otherwise it grows into the top or a free next chunk, or allocates a
 * new block, copies the contents (MEM_copy) and frees the old one with cDynamicMemMan_dlfree; any surplus is split off
 * and freed. Sizes of 0xFFFFFFE0 or more set *__errno() to 12 (ENOMEM) and return null.
 *
 * @param oldmem  the allocation to resize, or null
 * @param bytes  the new size
 * @return the resized allocation, or null
 */
void* cDynamicMemMan::dlrealloc(void* oldmem, unsigned int bytes)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    char* av = *(char**)this;
    unsigned int nb;
    malloc_chunk* oldp;
    unsigned int oldsize;
    malloc_chunk* newp;
    unsigned int newsize;
    char* newmem;
    malloc_chunk* next;
    malloc_chunk* remainder;
    unsigned int remainder_size;
    unsigned int head;

    if (oldmem == 0)
        return dlmalloc(bytes);

    if (bytes >= (unsigned int)-32)
        goto nomem;
    nb = (bytes + 4 + 15 < 16) ? 16 : (bytes + 4 + 15) & ~15;

    head = *(unsigned int*)((char*)oldmem - 4);
    oldp = (malloc_chunk*)((char*)oldmem - 8);
    oldsize = head & ~3;
    if (head & 2)
        goto nomem;

    if (oldsize >= nb) {
        newp = oldp;
        newsize = oldsize;
    } else {
        next = (malloc_chunk*)((char*)oldp + oldsize);
        if (next == *(malloc_chunk**)(av + 0x30)) {
            if ((newsize = oldsize + (next->size & ~3)) >= nb + 16) {
                *(unsigned int*)((int)oldmem - 4) = (head & 1) | nb;
                *(malloc_chunk**)(av + 0x30) = (malloc_chunk*)((char*)oldp + nb);
                ((malloc_chunk*)((char*)oldp + nb))->size = (newsize - nb) | 1;
                return oldmem;
            }
        } else if (!(((malloc_chunk*)((char*)next + (next->size & ~1)))->size & 1) &&
                   (newsize = oldsize + (next->size & ~3)) >= nb) {
            malloc_chunk* bck;
            malloc_chunk* fwd;
            newp = oldp;
            fwd = next->fd;
            bck = next->bk;
            fwd->bk = bck;
            bck->fd = fwd;
            goto split;
        }

        newmem = (char*)dlmalloc(nb - 15);
        if (newmem == 0)
            return 0;
        newp = (malloc_chunk*)(newmem - 8);
        newsize = *(unsigned int*)(newmem - 4) & ~3;
        if (newp == next) {
            newsize += oldsize;
            newp = oldp;
        } else {
            unsigned int copysize = oldsize - 4;
            unsigned int* sp = (unsigned int*)oldmem;
            unsigned int* dp = (unsigned int*)newmem;
            unsigned int ncopies = copysize / 4;
            if (ncopies > 9) {
                MEM_copy(dp, sp, copysize);
            } else {
                dp[0] = sp[0];
                dp[1] = sp[1];
                dp[2] = sp[2];
                if (ncopies > 4) {
                    dp[3] = sp[3];
                    dp[4] = sp[4];
                    if (ncopies > 6) {
                        dp[5] = sp[5];
                        dp[6] = sp[6];
                        if (ncopies > 8) {
                            dp[7] = sp[7];
                            dp[8] = sp[8];
                        }
                    }
                }
            }
            cDynamicMemMan_dlfree_E2D8(this, oldmem);
            return (char*)&newp->fd;
        }
    }

split:
    remainder_size = newsize - nb;
    if (remainder_size < 16) {
        newp->size = (newp->size & 1) | newsize;
        ((malloc_chunk*)((char*)newp + newsize))->size |= 1;
    } else {
        remainder = (malloc_chunk*)((char*)newp + nb);
        newp->size = (newp->size & 1) | nb;
        remainder->size = remainder_size | 1;
        ((malloc_chunk*)((char*)remainder + remainder_size))->size |= 1;
        cDynamicMemMan_dlfree_E2D8(this, &remainder->fd);
    }
    return (char*)&newp->fd;

nomem:
    *__errno() = 12;
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmemalign);
#ifdef SKIP_ASM
// dlmalloc 2.7 mEMALIGn()
extern "C" void* cDynamicMemMan_dlmalloc(cDynamicMemMan* ms, unsigned int bytes);
// PORT: the unit declares cDynamicMemMan_dlfree(void*); the body really takes (ms, mem).
extern "C" void cDynamicMemMan_dlfree_F2C8(void* ms, void* mem) __asm__("cDynamicMemMan_dlfree");
extern "C" int* __errno();


// PORT: the unit declares cDynamicMemMan_dlmemalign(void*); the body really takes (ms, alignment, bytes).
extern "C" void* cDynamicMemMan_dlmemalign_memalign(void* ms, unsigned int alignment, unsigned int bytes) __asm__("cDynamicMemMan_dlmemalign");

/**
 * Allocates @p bytes aligned to a power of two of at least @p alignment (dlmalloc 2.7's memalign).
 *
 * An alignment of 16 or less is a plain cDynamicMemMan_dlmalloc. Otherwise it over-allocates by the alignment, frees the leading
 * part and any surplus tail (cDynamicMemMan_dlfree) and returns the aligned chunk. cDynamicMemMan_dlvalloc and cDynamicMemMan_dlpvalloc call
 * it with the page size.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param alignment  the alignment wanted
 * @param bytes  the size requested
 * @return the aligned memory, or null
 */
extern "C" void* cDynamicMemMan_dlmemalign_memalign(void* ms, unsigned int alignment, unsigned int bytes)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    unsigned int nb;
    char* rawMem;
    malloc_chunk* memChunk;
    char* brk;
    malloc_chunk* newp;
    unsigned int newsize;
    unsigned int leadsize;
    malloc_chunk* remainder;
    unsigned int remainder_size;
    unsigned int size;

    if (alignment <= 16)
        return ((cDynamicMemMan*)ms)->dlmalloc(bytes);
    if (alignment < 16)
        alignment = 16;
    if ((alignment & (alignment - 1)) != 0) {
        unsigned int alignPow2 = 16 * 2;
        while (alignPow2 < alignment)
            alignPow2 <<= 1;
        alignment = alignPow2;
    }
    if (bytes >= (unsigned int)-32) {
        *__errno() = 12;
        return 0;
    }
    nb = (bytes + 4 + 15 < 16) ? 16 : (bytes + 4 + 15) & ~15;
    rawMem = (char*)((cDynamicMemMan*)ms)->dlmalloc(nb + alignment + 16);
    if (rawMem == 0)
        return 0;
    memChunk = (malloc_chunk*)(rawMem - 8);
    // PORT: chunk header reads go through (int)m - 8 (pointer in int) to match gcc 2.95 offset folding
    if ((unsigned int)rawMem % alignment != 0) {
        brk = (char*)(((unsigned int)(rawMem + alignment - 1) & -(int)alignment) - 8);
        if ((unsigned int)(brk - (char*)memChunk) < 16)
            brk += alignment;
        newp = (malloc_chunk*)brk;
        leadsize = brk - (char*)memChunk;
        newsize = (((malloc_chunk*)((int)rawMem - 8))->size & ~3) - leadsize;
        if (((malloc_chunk*)((int)rawMem - 8))->size & 2) {
            newp->prev_size = ((malloc_chunk*)((int)rawMem - 8))->prev_size + leadsize;
            newp->size = newsize | 2;
            return (char*)&newp->fd;
        }
        newp->size = newsize | 1;
        ((malloc_chunk*)((char*)newp + newsize))->size |= 1;
        ((malloc_chunk*)((int)rawMem - 8))->size = (((malloc_chunk*)((int)rawMem - 8))->size & 1) | leadsize;
        cDynamicMemMan_dlfree_F2C8(ms, &memChunk->fd);
        memChunk = newp;
    }
    if (!(memChunk->size & 2)) {
        size = memChunk->size & ~3;
        if (size > nb + 16) {
            remainder_size = size - nb;
            remainder = (malloc_chunk*)((char*)memChunk + nb);
            remainder->size = remainder_size | 1;
            memChunk->size = (memChunk->size & 1) | nb;
            cDynamicMemMan_dlfree_F2C8(ms, &remainder->fd);
        }
    }
    return (char*)&memChunk->fd;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmemalignhdr);
#ifdef SKIP_ASM
// dlmalloc 2.7 mEMALIGn() variant: aligns the address mem + offset instead of mem
extern "C" void* cDynamicMemMan_dlmalloc(cDynamicMemMan* ms, unsigned int bytes);
// PORT: the unit declares cDynamicMemMan_dlfree(void*); the body really takes (ms, mem).
extern "C" void cDynamicMemMan_dlfree_F4E8(void* ms, void* mem) __asm__("cDynamicMemMan_dlfree");
extern "C" int* __errno();


// PORT: the unit declares cDynamicMemMan_dlmemalignhdr(void*); the body really takes (ms, alignment, bytes, offset).
extern "C" void* cDynamicMemMan_dlmemalignhdr_memalign(void* ms, unsigned int alignment, unsigned int bytes, unsigned int offset) __asm__("cDynamicMemMan_dlmemalignhdr");

/**
 * Allocates memory so that the address @p offset bytes into it is aligned to @p alignment (memalign with a header).
 *
 * Works like cDynamicMemMan_dlmemalign on bytes + offset. Likely bug: for an alignment of 16 or less it returns
 * cDynamicMemMan_dlmalloc(bytes) without adding the offset, so the block lacks the header's room.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param alignment  the alignment wanted
 * @param bytes  the size requested
 * @param offset  the header size: the address this far into the block is aligned
 * @return the memory, or null
 */
extern "C" void* cDynamicMemMan_dlmemalignhdr_memalign(void* ms, unsigned int alignment, unsigned int bytes, unsigned int offset)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    unsigned int nb;
    char* rawMem;
    malloc_chunk* memChunk;
    char* brk;
    malloc_chunk* newp;
    unsigned int newsize;
    unsigned int leadsize;
    malloc_chunk* remainder;
    unsigned int remainder_size;
    unsigned int size;

    if (alignment <= 16)
        return ((cDynamicMemMan*)ms)->dlmalloc(bytes);
    if (alignment < 16)
        alignment = 16;
    if ((alignment & (alignment - 1)) != 0) {
        unsigned int alignPow2 = 16 * 2;
        while (alignPow2 < alignment)
            alignPow2 <<= 1;
        alignment = alignPow2;
    }
    bytes += offset;
    if (bytes >= (unsigned int)-32) {
        *__errno() = 12;
        return 0;
    }
    nb = (bytes + 4 + 15 < 16) ? 16 : (bytes + 4 + 15) & ~15;
    rawMem = (char*)((cDynamicMemMan*)ms)->dlmalloc(nb + alignment + 16);
    if (rawMem == 0)
        return 0;
    memChunk = (malloc_chunk*)(rawMem - 8);
    // PORT: chunk header reads go through (int)m - 8 (pointer in int) to match gcc 2.95 offset folding
    if ((unsigned int)(rawMem + offset) % alignment != 0) {
        brk = (char*)(((unsigned int)(rawMem + offset + alignment - 1) & -(int)alignment) - offset - 8);
        if ((unsigned int)(brk - (char*)memChunk) < 16)
            brk += alignment;
        newp = (malloc_chunk*)brk;
        leadsize = brk - (char*)memChunk;
        newsize = (((malloc_chunk*)((int)rawMem - 8))->size & ~3) - leadsize;
        if (((malloc_chunk*)((int)rawMem - 8))->size & 2) {
            newp->prev_size = ((malloc_chunk*)((int)rawMem - 8))->prev_size + leadsize;
            newp->size = newsize | 2;
            return (char*)&newp->fd;
        }
        newp->size = newsize | 1;
        ((malloc_chunk*)((char*)newp + newsize))->size |= 1;
        ((malloc_chunk*)((int)rawMem - 8))->size = (((malloc_chunk*)((int)rawMem - 8))->size & 1) | leadsize;
        cDynamicMemMan_dlfree_F4E8(ms, &memChunk->fd);
        memChunk = newp;
    }
    if (!(memChunk->size & 2)) {
        size = memChunk->size & ~3;
        if (size > nb + 16) {
            remainder_size = size - nb;
            remainder = (malloc_chunk*)((char*)memChunk + nb);
            remainder->size = remainder_size | 1;
            memChunk->size = (memChunk->size & 1) | nb;
            cDynamicMemMan_dlfree_F4E8(ms, &remainder->fd);
        }
    }
    return (char*)&memChunk->fd;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlcalloc);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Allocates @p n * @p elem_size bytes with cDynamicMemMan_dlmalloc and zeroes them (dlmalloc 2.7's calloc).
 *
 * Small chunks are cleared word by word, larger ones with MEM_fill. The product is not checked for overflow.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param n  the number of elements
 * @param elem_size  the size of one element
 * @return the zeroed memory, or null
 */
// dlmalloc 2.7 cALLOc()
void* cDynamicMemMan::dlcalloc(unsigned int n, unsigned int elem_size)
{
    // ORIGINAL-PTR: dlmalloc 2.7's mem2chunk: the chunk header sits 8 bytes before the user memory and its size
    // word 4 bytes before it (a byte heap)
    unsigned int bytes = n * elem_size;
    void* mem = dlmalloc(bytes);
    if (mem != 0) {
        unsigned int csz = *(unsigned int*)((char*)mem - 4);
        if (!(csz & 2)) {
            unsigned int clearsize = (csz & ~3U) - 4;
            unsigned int nclears = clearsize >> 2;
            unsigned int* words = (unsigned int*)mem;
            if (nclears > 9) {
                MEM_fill(words, 0, clearsize);
            } else {
                words[0] = 0;
                words[1] = 0;
                words[2] = 0;
                if (nclears > 4) {
                    words[3] = 0;
                    words[4] = 0;
                    if (nclears > 6) {
                        words[5] = 0;
                        words[6] = 0;
                        if (nclears > 8) {
                            words[7] = 0;
                            words[8] = 0;
                        }
                    }
                }
            }
        } else {
            MEM_fill(mem, 0, (csz & ~3U) - 8);
        }
    }
    return mem;
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlcfree);
#ifdef SKIP_ASM
// PORT: the unit declares cDynamicMemMan_dlfree(void*); the body really takes (ms, mem).
extern "C" void cDynamicMemMan_dlfree_F7F0(void* ms, void* mem) __asm__("cDynamicMemMan_dlfree");

/**
 * Frees an allocation through cDynamicMemMan_dlfree.
 *
 * @param mem  the allocation
 */
void cDynamicMemMan::dlcfree(void* mem)
{
    cDynamicMemMan_dlfree_F7F0(this, mem);
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlindependent_calloc);
#ifdef SKIP_ASM
extern "C" void* cDynamicMemMan_iALLOc(cDynamicMemMan* ms, unsigned int n, unsigned int* sizes, int opts, void** chunks);

/**
 * Allocates @p n zeroed elements of @p elem_size bytes as separate chunks in one block (cDynamicMemMan_iALLOc with options 3).
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param n  the number of elements
 * @param elem_size  the size of each element
 * @param chunks  receives the element pointers, or null to allocate the array too
 * @return the pointer array, or null
 */
// dlmalloc 2.7 independent_calloc()
void* cDynamicMemMan::dlindependent_calloc(unsigned int n, unsigned int elem_size, void** chunks)
{
    unsigned int sz = elem_size;
    return iALLOc(n, &sz, 3, chunks);
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlindependent_comalloc);
#ifdef SKIP_ASM
extern "C" void* cDynamicMemMan_iALLOc(cDynamicMemMan* ms, unsigned int n, unsigned int* sizes, int opts, void** chunks);

/**
 * Allocates @p n chunks of the sizes in @p sizes in one block (cDynamicMemMan_iALLOc with options 0).
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param n  the number of chunks
 * @param sizes  the size of each chunk
 * @param chunks  receives the chunk pointers, or null to allocate the array too
 * @return the pointer array, or null
 */
// dlmalloc 2.7 independent_comalloc()
void* cDynamicMemMan::dlindependent_comalloc(unsigned int n, unsigned int* sizes, void** chunks)
{
    return iALLOc(n, sizes, 0, chunks);
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_iALLOc);
#ifdef SKIP_ASM

extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Allocates @p n chunks from one block and returns an array of pointers to them (the worker behind dlmalloc 2.7's
 * independent_calloc and independent_comalloc).
 *
 * Bit 0 of @p opts means all elements have the size *@p sizes, bit 1 zeroes the contents. Without @p chunks the pointer
 * array is carved from the same block. The mmap count limit (+0x354) is set to 0 around the cDynamicMemMan_dlmalloc call. @p n of
 * 0 returns @p chunks, or cDynamicMemMan_dlmalloc(0) when that is null.
 *
 * @param n  the number of chunks
 * @param sizes  the chunk sizes (one size when bit 0 of @p opts is set)
 * @param opts  bit 0: same size for all; bit 1: zero the memory
 * @param chunks  receives the chunk pointers, or null
 * @return the pointer array, or null
 */
void* cDynamicMemMan::iALLOc(unsigned int n, unsigned int* sizes, int opts, void** chunks)
{
    // ORIGINAL-PTR: dlmalloc 2.7 addresses chunks by byte offsets in its heap (chunk_at_offset, mem2chunk /
    // chunk2mem, bin_at's fake chunk 8 bytes before each bins[] pair): a byte heap
    malloc_state* av = *(malloc_state**)this;
    unsigned int element_size;
    unsigned int contents_size;
    unsigned int array_size;
    void* mem;
    malloc_chunk* chunk;
    unsigned int remainder_size;
    void** marray;
    malloc_chunk* array_chunk;
    int mmx;
    unsigned int size;
    unsigned int i;

    if (av->max_fast == 0)
        malloc_consolidate(av);

    if (chunks != 0) {
        if (n == 0)
            return chunks;
        marray = chunks;
        array_size = 0;
    } else {
        if (n == 0)
            return dlmalloc(0);
        marray = 0;
        array_size = (n * sizeof(void*) + 4 + 15 < 16) ? 16 : (n * sizeof(void*) + 4 + 15) & ~15;
    }

    if (opts & 1) {
        element_size = (*sizes + 4 + 15 < 16) ? 16 : (*sizes + 4 + 15) & ~15;
        contents_size = n * element_size;
    } else {
        element_size = 0;
        contents_size = 0;
        for (i = 0; i != n; ++i)
            contents_size += (sizes[i] + 4 + 15 < 16) ? 16 : (sizes[i] + 4 + 15) & ~15;
    }

    size = contents_size + array_size - 15;

    mmx = av->n_mmaps_max;
    av->n_mmaps_max = 0;
    mem = dlmalloc(size);
    av->n_mmaps_max = mmx;
    if (mem == 0)
        return 0;

    chunk = (malloc_chunk*)((char*)mem - 8);
    remainder_size = *(unsigned int*)((char*)mem - 4) & ~3;

    if (opts & 2)
        MEM_fill(mem, 0, remainder_size - 4 - array_size);

    if (marray == 0) {
        array_chunk = (malloc_chunk*)((char*)chunk + contents_size);
        marray = (void**)(&array_chunk->fd);
        array_chunk->size = (remainder_size - contents_size) | 1;
        remainder_size = contents_size;
    }

    for (i = 0; ; ++i) {
        marray[i] = (char*)&chunk->fd;
        if (i != n - 1) {
            if (element_size != 0)
                size = element_size;
            else
                size = (sizes[i] + 4 + 15 < 16) ? 16 : (sizes[i] + 4 + 15) & ~15;
            remainder_size -= size;
            chunk->size = size | 1;
            chunk = (malloc_chunk*)((char*)chunk + size);
        } else {
            chunk->size = remainder_size | 1;
            break;
        }
    }
    return marray;
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlvalloc);
#ifdef SKIP_ASM

/**
 * Allocates @p bytes aligned to the page size (+0x35C of the malloc state) with cDynamicMemMan_dlmemalign.
 *
 * Sets the state up first (cDynamicMemMan_malloc_consolidate) when its fastbin limit is still 0.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param bytes  the size requested
 * @return the memory, or null
 */
extern "C" void* cDynamicMemMan_dlvalloc(void** ms, unsigned int bytes)
{
    malloc_state* av = *(malloc_state**)ms;
    if (av->max_fast == 0) {
        ((cDynamicMemMan*)ms)->malloc_consolidate((malloc_state*)av);
    }
    return cDynamicMemMan_dlmemalign_memalign(ms, av->pagesize, bytes);
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlpvalloc);
#ifdef SKIP_ASM

/**
 * Allocates @p bytes rounded up to whole pages, page-aligned, with cDynamicMemMan_dlmemalign.
 *
 * Sets the state up first (cDynamicMemMan_malloc_consolidate) when its fastbin limit is still 0. The page size is +0x35C of the malloc
 * state.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param bytes  the size requested
 * @return the memory, or null
 */
extern "C" void* cDynamicMemMan_dlpvalloc(void** ms, unsigned int bytes)
{
    malloc_state* av = *(malloc_state**)ms;
    if (av->max_fast == 0) {
        ((cDynamicMemMan*)ms)->malloc_consolidate((malloc_state*)av);
    }
    unsigned int pagesize = av->pagesize;
    return cDynamicMemMan_dlmemalign_memalign(ms, pagesize, (bytes + pagesize - 1) & ~(pagesize - 1));
}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmalloc_trim);
#ifdef SKIP_ASM
struct sMState_31EEE8;
struct sMState_31E6D8;
extern "C" void cDynamicMemMan_malloc_consolidate(cDynamicMemMan* ms, malloc_state* av);
extern "C" bool cDynamicMemMan_sYSTRIm(cDynamicMemMan* ms, unsigned int pad, malloc_state* av);

/**
 * Consolidates the fast bins (cDynamicMemMan_malloc_consolidate) and returns unused top memory beyond @p topPad to the core
 * (cDynamicMemMan_sYSTRIm).
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @param topPad  bytes to keep above what is in use
 * @return 1 when memory was released, else 0
 */
// dlmalloc 2.7 malloc_trim(): consolidate fastbins, then sYSTRIm.
extern "C" int cDynamicMemMan_dlmalloc_trim(void** ms, unsigned int topPad)
{
    void* av = *ms;
    ((cDynamicMemMan*)ms)->malloc_consolidate((malloc_state*)av);
    return ((cDynamicMemMan*)ms)->sYSTRIm(topPad, (malloc_state*)av);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmalloc_usable_size__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns how many bytes of an allocation can be used: its chunk size minus the header overhead.
 *
 * 0 for null or for a chunk whose next chunk does not mark it in use.
 *
 * @param mem  the allocation
 * @return the usable size, or 0
 */
unsigned int cDynamicMemMan::dlmalloc_usable_size(void* mem)
{
    // ORIGINAL-PTR: dlmalloc 2.7's mem2chunk: the chunk header sits 8 bytes before the user memory and its size
    // word 4 bytes before it (a byte heap)
    if (mem != 0) {
        unsigned int size = *(unsigned int*)((char*)mem - 4);
        char* chunk = (char*)mem - 8;
        if (size & 2) {
            return (size & ~3U) - 8;
        } else if (*(unsigned int*)(chunk + (size & ~1U) + 4) & 1) {
            return (size & ~3U) - 4;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmallinfo);
#ifdef SKIP_ASM



struct sMState_31EEE8;
extern "C" void cDynamicMemMan_malloc_consolidate(cDynamicMemMan* ms, malloc_state* av);

/**
 * Returns heap statistics: free and used totals, block counts and the top size (dlmalloc 2.7's mallinfo).
 *
 * Counts the fast bins and walks every regular bin; unlike dlmalloc it stops a bin walk early when the links loop back
 * on themselves. A state with no top yet is set up first (cDynamicMemMan_malloc_consolidate). The structure is returned by value.
 *
 * @param ms  the heap manager; its first word points to the malloc state
 * @return the statistics
 */
extern "C" mallinfo cDynamicMemMan_dlmallinfo(cDynamicMemMan* ms)
{
    // ORIGINAL-PTR: dlmalloc 2.7's bin_at: each bin is a fake chunk 8 bytes before its bins[] pair, so its fd / bk
    // land on the pair; the bins are walked in 8-byte steps (a byte heap)
    malloc_state* av = *(malloc_state**)ms;
    mallinfo mi;
    int i;
    malloc_chunk* binHead;
    malloc_chunk* chunk;
    unsigned int avail;
    unsigned int fastavail;
    int nblocks;
    int nfastblocks;

    if (av->top == 0) {
        ms->malloc_consolidate((malloc_state*)av);
    }

    avail = av->top->size & ~3U;
    nblocks = 1;

    nfastblocks = 0;
    fastavail = 0;

    for (i = 0; (unsigned int)i < 11U; ++i) {
        for (chunk = av->fastbins[i]; chunk != 0; chunk = chunk->fd) {
            ++nfastblocks;
            fastavail += chunk->size & ~3U;
        }
    }

    avail += fastavail;

    for (i = 1; i < 96; ++i) {
        malloc_chunk** bp = &((malloc_chunk**)&av->bins[0])[i << 1];
        binHead = (malloc_chunk*)((char*)bp - 8);
        for (chunk = bp[1]; chunk != binHead; chunk = chunk->bk) {
            ++nblocks;
            avail += chunk->size & ~3U;
            if (chunk == chunk->bk->bk) break;
            if (chunk == chunk->bk && chunk != binHead) break;
        }
    }

    mi.smblks = nfastblocks;
    mi.ordblks = nblocks;
    mi.fordblks = avail;
    mi.uordblks = av->sbrked_mem - avail;
    mi.arena = av->sbrked_mem;
    mi.hblks = av->n_mmaps;
    mi.hblkhd = av->mmapped_mem;
    mi.fsmblks = fastavail;
    mi.keepcost = av->top->size & ~3U;
    mi.usmblks = av->max_total_mem;
    return mi;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", func_0031FE28);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body).
 *
 * Sits between cDynamicMemMan_dlmallinfo and cDynamicMemMan_dlmallopt, where dlmalloc 2.7 defines its statistics printer. Nothing calls it.
 */
extern "C" void func_0031FE28(void) {

}
#endif
//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmallopt);
#ifdef SKIP_ASM
/**
 * Sets a heap parameter and returns whether the value was accepted (dlmalloc 2.7's mallopt).
 *
 * Consolidates the fast bins first (cDynamicMemMan_malloc_consolidate). @p kind M_MXFAST sets the fastbin limit (+0x0)
 * for values up to 80; M_TRIM_THRESHOLD the trim threshold (+0x344), M_TOP_PAD the top pad (+0x348),
 * M_MMAP_THRESHOLD the mmap threshold (+0x34C), and M_MMAP_MAX the mmap count (+0x354), which only accepts 0.
 *
 * @param kind  the parameter (eMallocOption)
 * @param val  the new value
 * @return true when accepted
 */
extern "C" bool cDynamicMemMan_dlmallopt(void** self, int kind, unsigned int val) {
    malloc_state* ms = (malloc_state*)((char*)*self);
    ((cDynamicMemMan*)self)->malloc_consolidate((malloc_state*)ms);
    switch (kind) {
    case M_MXFAST:
        if (val < 81U) {
            int low = (*(int*)ms) & 3;
            int res;
            if (val != 0) {
                unsigned int rounded = val + 19;
                if (rounded >= 0x10U) {
                    res = (rounded & 0xFFFFFFF0) | low;
                } else {
                    res = low | 0x10;
                }
            } else {
                res = low | 8;
            }
            *(int*)ms = res;
            return true;
        }
        return false;
    case M_TRIM_THRESHOLD:
        ms->trim_threshold = val;
        return true;
    case M_TOP_PAD:
        ms->top_pad = val;
        return true;
    case M_MMAP_THRESHOLD:
        ms->mmap_threshold = val;
        return true;
    case M_MMAP_MAX:
        if (val == 0) {
            ms->n_mmaps_max = 0;
            return true;
        }
        return false;
    default:
        return false;
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_cDynamicMemMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty dynamic heap manager: zeroes its four words (malloc state, heap, core top, heap size).
 *
 * @return @p self
 */
void* cDynamicMemMan_cDynamicMemMan(void* vself)
{
    cDynamicMemMan* self = (cDynamicMemMan*)vself;
    self->Heap = 0;
    self->CoreTop = 0;
    self->HeapSize = 0;
    self->MallocState = 0;
    return ((void*)self);
}
#endif

void operator_delete(int* ptr);

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan__cDynamicMemMan__FPvi);
#ifdef SKIP_ASM
/**
 * Destroys a dynamic heap manager: frees the object when bit 0 of @p flags is set; nothing else to release.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
void cDynamicMemMan__cDynamicMemMan(void* vself, int flags)
{
    cDynamicMemMan* self = (cDynamicMemMan*)vself;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_init);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Sets up a dynamic heap in a block of memory.
 *
 * The start is @p base rounded up to 16 (stored at +0x0); the first 0x380 bytes hold the zeroed malloc state, so the
 * free area starts at +0x4 = start + 0x380 (also +0x8) with size +0xC = the aligned remaining size minus 0x380.
 *
 * @param base  the memory to manage (On Tour heap)
 * @param size  its size in bytes
 */
void cDynamicMemMan::init(unsigned int base, unsigned int size)
{
    unsigned int start = (base + 0xF) & 0xFFFFFFF0;
    Heap = (char*)start;
    MallocState = (malloc_state*)start;
    HeapSize = (size - (start - base)) & 0xFFFFFFF0;
    MEM_fill((void*)start, 0, 888);
    *(unsigned int*)&Heap += 0x380;
    HeapSize -= 0x380;
    CoreTop = (char*)((unsigned int)Heap);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_dlmorecore);
#ifdef SKIP_ASM
/**
 * Moves the heap's core top (+0x8) by @p incr bytes, like sbrk, within the heap (+0x4, size +0xC).
 *
 * A non-negative @p incr that still fits returns the old top; one that does not fit returns -1. A negative @p incr
 * moves the top down without a lower-bound check and returns the new top. cDynamicMemMan_sYSMALLOc and cDynamicMemMan_sYSTRIm use it as
 * their core.
 *
 * @param incr  the number of bytes to add (negative to give back)
 * @return the old top (new top when shrinking), or -1 (0xFFFFFFFF)
 */
unsigned int cDynamicMemMan::dlmorecore(long incr)
{
    if (incr >= 0) {
        unsigned int old = (unsigned int)CoreTop;
        unsigned int top = old + incr;
        if (top <= (unsigned int)Heap + HeapSize) {
            CoreTop = (char*)top;
            return old;
        }
    } else {
        return *(unsigned int*)&CoreTop += incr;
    }
    return 0xFFFFFFFF;
}
#endif

extern "C" int cDynamicMemMan_dlmemalign(void* self);

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_alloc);
#ifdef SKIP_ASM
extern "C" void* cDynamicMemMan_dlmalloc(cDynamicMemMan* ms, unsigned int bytes);

/**
 * Allocates @p bytes from the dynamic heap (cDynamicMemMan_dlmalloc).
 *
 * @param bytes  the size requested
 * @return the memory, or null
 */
void* cDynamicMemMan::alloc(unsigned int bytes)
{
    return dlmalloc(bytes);
}
#endif

//99.29%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_allocAlign__FPv);
#ifdef SKIP_ASM
/**
 * Allocates aligned memory from the dynamic heap: forwards to cDynamicMemMan_dlmemalign with the alignment and size passed through.
 *
 * The C declares only the heap manager; the alignment and size reach cDynamicMemMan_dlmemalign untouched in the second and third
 * argument registers.
 *
 * @return the memory (declared int)
 */
int cDynamicMemMan::allocAlign()
{
    return cDynamicMemMan_dlmemalign(this);
}
#endif

extern "C" int cDynamicMemMan_dlmemalignhdr(void* self);

//99.29%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_allocAlignHdr__FPv);
#ifdef SKIP_ASM
/**
 * Allocates header-offset aligned memory from the dynamic heap: forwards to cDynamicMemMan_dlmemalignhdr with its arguments passed
 * through.
 *
 * The C declares only the heap manager; the alignment, size and header size reach cDynamicMemMan_dlmemalignhdr untouched in their
 * registers.
 *
 * @return the memory (declared int)
 */
int cDynamicMemMan::allocAlignHdr()
{
    return cDynamicMemMan_dlmemalignhdr(this);
}
#endif

extern "C" int cDynamicMemMan_dlfree(void* self);

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_isBlock);
#ifdef SKIP_ASM
/**
 * Returns whether an address lies strictly inside the dynamic heap (between +0x4 and +0x4 plus the size at +0xC).
 *
 * @param addr  the address to test
 * @return true when inside
 */
bool cDynamicMemMan::isBlock(unsigned int addr)
{
    unsigned int base = (unsigned int)Heap;
    unsigned int end = base + HeapSize;
    return base < addr && addr < end;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", cDynamicMemMan_free__FPv);
#ifdef SKIP_ASM
/**
 * Frees a block of the dynamic heap.
 *
 * Tail-calls cDynamicMemMan_dlfree. The block pointer passes through in $a1, which the one-argument C prototype does
 * not show.
 *
 * @return nothing useful (the C declares int)
 */
int cDynamicMemMan::free()
{
    return cDynamicMemMan_dlfree(this);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", func_003200E0);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body).
 *
 * Sits right after cDynamicMemMan_free. Nothing calls it.
 */
extern "C" void func_003200E0(void) {

}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer8_TMemContainer8__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 8-byte block pool's free list: 256 blocks of 8 bytes, each linked to the next.
 *
 * The list head at +0x800 points to the first block and the last block's link (+0x7F8) is 0. cMemBlockMan_initialize
 * runs it on the "m_container8" allocation (cMemBlockMan_m_container8).
 *
 * @return @p self
 */
void* TMemContainer8_TMemContainer8(void* vself)
{
    TMemContainer8* self = (TMemContainer8*)vself;
    self->m_freeList = (TMemBlock8*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (unsigned int i = 0; i < 0xFF; i++) {
        *(void**)cursor = next;
        cursor += 8;
        next += 8;
    }
    *(int*)&self->m_blockList[255] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer16_TMemContainer16__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 16-byte block pool's free list: 512 blocks of 16 bytes, each linked to the next.
 *
 * The list head at +0x2000 points to the first block and the last block's link (+0x1FF0) is 0. cMemBlockMan_initialize
 * runs it on the "m_container16" allocation (cMemBlockMan_m_container16).
 *
 * @return @p self
 */
void* TMemContainer16_TMemContainer16(void* vself)
{
    TMemContainer16* self = (TMemContainer16*)vself;
    self->m_freeList = (TMemBlock16*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (unsigned int i = 0; i < 0x1FF; i++) {
        *(void**)cursor = next;
        cursor += 0x10;
        next += 0x10;
    }
    *(int*)&self->m_blockList[511] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer32_TMemContainer32__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 32-byte block pool's free list: 768 blocks of 32 bytes, each linked to the next.
 *
 * The list head at +0x6000 points to the first block and the last block's link (+0x5FE0) is 0. cMemBlockMan_initialize
 * runs it on the "m_container32" allocation (cMemBlockMan_m_container32).
 *
 * @return @p self
 */
void* TMemContainer32_TMemContainer32(void* vself)
{
    TMemContainer32* self = (TMemContainer32*)vself;
    self->m_freeList = (TMemBlock32*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (unsigned int i = 0; i < 767; i++) {
        *(void**)cursor = next;
        cursor += 0x20;
        next += 0x20;
    }
    *(int*)&self->m_blockList[767] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer64_TMemContainer64__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 64-byte block pool's free list: 3200 blocks of 64 bytes, each linked to the next.
 *
 * The list head at +0x32000 points to the first block and the last block's link (+0x31FC0) is 0.
 * cMemBlockMan_initialize runs it on the "m_container64" allocation (cMemBlockMan_m_container64).
 *
 * @return @p self
 */
void* TMemContainer64_TMemContainer64(void* vself)
{
    TMemContainer64* self = (TMemContainer64*)vself;
    unsigned int i = 0;
    self->m_freeList = (TMemBlock64*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (; i < 3199; i++) {
        *(void**)cursor = next;
        cursor += 0x40;
        next += 0x40;
    }
    *(int*)&self->m_blockList[3199] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer128_TMemContainer128__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 128-byte block pool's free list: 1200 blocks of 128 bytes, each linked to the next.
 *
 * The list head at +0x25800 points to the first block and the last block's link (+0x25780) is 0.
 * cMemBlockMan_initialize runs it on the "m_container128" allocation (cMemBlockMan_m_container128).
 *
 * @return @p self
 */
void* TMemContainer128_TMemContainer128(void* vself)
{
    TMemContainer128* self = (TMemContainer128*)vself;
    unsigned int i = 0;
    self->m_freeList = (TMemBlock128*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (; i < 1199; i++) {
        *(void**)cursor = next;
        cursor += 0x80;
        next += 0x80;
    }
    *(int*)&self->m_blockList[1199] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer256_TMemContainer256__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 256-byte block pool's free list: 512 blocks of 256 bytes, each linked to the next.
 *
 * The list head at +0x20000 points to the first block and the last block's link (+0x1FF00) is 0.
 * cMemBlockMan_initialize runs it on the "m_container256" allocation (cMemBlockMan_m_container256).
 *
 * @return @p self
 */
void* TMemContainer256_TMemContainer256(void* vself)
{
    TMemContainer256* self = (TMemContainer256*)vself;
    unsigned int i = 0;
    self->m_freeList = (TMemBlock256*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (; i < 0x1FF; i++) {
        *(void**)cursor = next;
        cursor += 0x100;
        next += 0x100;
    }
    *(int*)&self->m_blockList[511] = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer512_TMemContainer512__FPv);
#ifdef SKIP_ASM
/**
 * Builds the 512-byte block pool's free list: 256 blocks of 512 bytes, each linked to the next.
 *
 * The list head at +0x20000 points to the first block and the last block's link (+0x1FE00) is 0.
 * cMemBlockMan_initialize runs it on the "m_container512" allocation (cMemBlockMan_m_container512).
 *
 * @return @p self
 */
void* TMemContainer512_TMemContainer512(void* vself)
{
    TMemContainer512* self = (TMemContainer512*)vself;
    unsigned int i = 0;
    self->m_freeList = (TMemBlock512*)((void*)self);
    char* cursor = (char*)self;
    char* next = (char*)&self->m_blockList[1];
    for (; i < 0xFF; i++) {
        *(void**)cursor = next;
        cursor += 0x200;
        next += 0x200;
    }
    *(int*)&self->m_blockList[255] = 0;
    return ((void*)self);
}
#endif

extern "C" void* cMemBlockMan_alloc(int size);

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer8_alloc__FPv);
#ifdef SKIP_ASM
/**
 * Takes a block from the 8-byte pool.
 *
 * Pops the head of the free list at +0x800; when the pool is empty it falls back to cMemBlockMan_alloc(12). Nothing in
 * the C calls it.
 *
 * @return the block
 */
void* TMemContainer8_alloc(void* vself)
{
    TMemContainer8* self = (TMemContainer8*)vself;
    void* node = self->m_freeList;
    if (node != 0) {
        void* next = *(void**)node;
        self->m_freeList = (TMemBlock8*)next;
        return node;
    }
    return cMemBlockMan_alloc(0xC);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer16_alloc__FPv);
#ifdef SKIP_ASM
/**
 * Takes a block from the 16-byte pool.
 *
 * Pops the head of the free list at +0x2000; when the pool is empty it falls back to cMemBlockMan_alloc(20). Nothing in
 * the C calls it.
 *
 * @return the block
 */
void* TMemContainer16_alloc(void* vself)
{
    TMemContainer16* self = (TMemContainer16*)vself;
    void* node = self->m_freeList;
    if (node != 0) {
        void* next = *(void**)node;
        self->m_freeList = (TMemBlock16*)next;
        return node;
    }
    return cMemBlockMan_alloc(20);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer32_alloc__FPv);
#ifdef SKIP_ASM
/**
 * Takes a block from the 32-byte pool.
 *
 * Pops the head of the free list at +0x6000; when the pool is empty it falls back to cMemBlockMan_alloc(36). Nothing in
 * the C calls it.
 *
 * @return the block
 */
void* TMemContainer32_alloc(void* vself)
{
    TMemContainer32* self = (TMemContainer32*)vself;
    void* node = self->m_freeList;
    if (node != 0) {
        void* next = *(void**)node;
        self->m_freeList = (TMemBlock32*)next;
        return node;
    }
    return cMemBlockMan_alloc(36);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer64_alloc__FPv);
#ifdef SKIP_ASM
static inline void* popFreeNode(void** head)
{
    void* node = *head;
    *head = *(void**)node;
    return node;
}

/**
 * Takes a block from the 64-byte pool.
 *
 * Pops the head of the free list at +0x32000; when the pool is empty it falls back to cMemBlockMan_alloc(68). Nothing
 * in the C calls it.
 *
 * @return the block
 */
void* TMemContainer64_alloc(void* vself)
{
    TMemContainer64* self = (TMemContainer64*)vself;
    if (self->m_freeList != 0) {
        return popFreeNode((void**)(&self->m_freeList));
    }
    return cMemBlockMan_alloc(68);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer128_alloc__FPv);
#ifdef SKIP_ASM
/**
 * Takes a block from the 128-byte pool.
 *
 * Pops the head of the free list at +0x25800; when the pool is empty it falls back to cMemBlockMan_alloc(132). Nothing
 * in the C calls it.
 *
 * @return the block
 */
void* TMemContainer128_alloc(void* vself)
{
    TMemContainer128* self = (TMemContainer128*)vself;
    if (self->m_freeList != 0) {
        return popFreeNode((void**)(&self->m_freeList));
    }
    return cMemBlockMan_alloc(132);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer256_alloc__FPv);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemBlockMan_alloc_0408(unsigned int size, unsigned int align) __asm__("cMemBlockMan_alloc");



static inline void* reserveBlock_0408(TMemBlock256** blockptr)
{
    TMemBlock256* block = *blockptr;
    *blockptr = block->unk_00;
    return block;
}

void* TMemContainer256_alloc(TMemContainer256* self, unsigned int align) __asm__("TMemContainer256_alloc__FPv");
/**
 * Takes a block from the 256-byte pool.
 *
 * Pops the head of the free list at +0x20000; when the pool is empty it falls back to cMemBlockMan_alloc(260), passing
 * @p align on. Nothing in the C calls it.
 *
 * @param align  the alignment, passed to the fallback
 * @return the block
 */
void* TMemContainer256::alloc(unsigned int align)
{
    if (m_freeList != 0) {
        return reserveBlock_0408(&(*(TMemBlock256**)&m_freeList));
    }
    return cMemBlockMan_alloc_0408(260, align);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer512_alloc__FPv);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemBlockMan_alloc_0448(unsigned int size, unsigned int align) __asm__("cMemBlockMan_alloc");



static inline void* reserveBlock_0448(TMemBlock512** blockptr)
{
    TMemBlock512* block = *blockptr;
    *blockptr = block->unk_00;
    return block;
}

void* TMemContainer512_alloc(TMemContainer512* self, unsigned int align) __asm__("TMemContainer512_alloc__FPv");
/**
 * Takes a block from the 512-byte pool.
 *
 * Pops the head of the free list at +0x20000; when the pool is empty it falls back to cMemBlockMan_alloc(516), passing
 * @p align on. Nothing in the C calls it.
 *
 * @param align  the alignment, passed to the fallback
 * @return the block
 */
void* TMemContainer512::alloc(unsigned int align)
{
    if (m_freeList != 0) {
        return reserveBlock_0448(&(*(TMemBlock512**)&m_freeList));
    }
    return cMemBlockMan_alloc_0448(516, align);
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer8_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 8-byte pool by pushing it onto the free list at +0x800.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container8.
 *
 * @param node  the block to free
 */
void TMemContainer8_free(void* vself, void** node)
{
    TMemContainer8* self = (TMemContainer8*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock8*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer16_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 16-byte pool by pushing it onto the free list at +0x2000.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container16.
 *
 * @param node  the block to free
 */
void TMemContainer16_free(void* vself, void** node)
{
    TMemContainer16* self = (TMemContainer16*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock16*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer32_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 32-byte pool by pushing it onto the free list at +0x6000.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container32.
 *
 * @param node  the block to free
 */
void TMemContainer32_free(void* vself, void** node)
{
    TMemContainer32* self = (TMemContainer32*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock32*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer64_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 64-byte pool by pushing it onto the free list at +0x32000.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container64.
 *
 * @param node  the block to free
 */
void TMemContainer64_free(void* vself, void** node)
{
    TMemContainer64* self = (TMemContainer64*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock64*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer128_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 128-byte pool by pushing it onto the free list at +0x25800.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container128.
 *
 * @param node  the block to free
 */
void TMemContainer128_free(void* vself, void** node)
{
    TMemContainer128* self = (TMemContainer128*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock128*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer256_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 256-byte pool by pushing it onto the free list at +0x20000.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container256.
 *
 * @param node  the block to free
 */
void TMemContainer256_free(void* vself, void** node)
{
    TMemContainer256* self = (TMemContainer256*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock256*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer512_free__FPvPPv);
#ifdef SKIP_ASM
/**
 * Returns a block to the 512-byte pool by pushing it onto the free list at +0x20000.
 *
 * The block's first word becomes the link. cMemBlockMan_free calls it for addresses inside cMemBlockMan_m_container512.
 *
 * @param node  the block to free
 */
void TMemContainer512_free(void* vself, void** node)
{
    TMemContainer512* self = (TMemContainer512*)vself;
    *node = (void*)self->m_freeList;
    self->m_freeList = (TMemBlock512*)node;
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer8_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 8-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container8.
 */
void TMemContainer8_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer16_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 16-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container16.
 */
void TMemContainer16_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer32_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 32-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container32.
 */
void TMemContainer32_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer64_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 64-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container64.
 */
void TMemContainer64_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer128_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 128-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container128.
 */
void TMemContainer128_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer256_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 256-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container256.
 */
void TMemContainer256_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", TMemContainer512_validate__Fv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body): the 512-byte pool's check.
 *
 * cMemBlockMan_validate calls it on cMemBlockMan_m_container512.
 */
void TMemContainer512_validate(void)
{
}
#endif

//100%
INCLUDE_ASM("bx/cubicspline", __sti__all_in_one_bx_cpp);
#ifdef SKIP_ASM
void* cBlockMan_cBlockMan(void* self);
void* cRandom_init(void* self);
void* cDynamicMemMan_cDynamicMemMan(void* self);
void cDynamicMemMan__cDynamicMemMan(void* self, int flags);

inline void* operator new(unsigned int, void* p) { return p; }

struct sV2_0550 {
    float x, y;
    sV2_0550(const float& ax, const float& ay) : x(ax), y(ay) {}
};
struct sV3_0550 {
    float x, y, z;
    sV3_0550(const float& ax, const float& ay, const float& az) : x(ax), y(ay), z(az) {}
};
struct sV4_0550 {
    float x, y, z, w;
    sV4_0550(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));
struct sQ_0550 {
    float x, y, z, w;
    sQ_0550(const float& ax, const float& ay, const float& az, const float& aw)
    {
        w = aw;
        x = ax;
        y = ay;
        z = az;
    }
} __attribute__((aligned(16)));
struct sM33_0550 {
    float m[3][3];
    sM33_0550()
    {
        m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f;
        m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f;
        m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f;
    }
};
struct sM44_0550 {
    float m[4][4];
    sM44_0550(float d)
    {
        m[0][0] = d;    m[1][0] = 0.0f; m[2][0] = 0.0f; m[3][0] = 0.0f;
        m[0][1] = 0.0f; m[1][1] = d;    m[2][1] = 0.0f; m[3][1] = 0.0f;
        m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = d;    m[3][2] = 0.0f;
        m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = d;
    }
};
struct sXf_0550 {
    sV4_0550 a;
    sQ_0550 b;
    sXf_0550(const sV4_0550& aa, const sQ_0550& ab) : a(aa), b(ab) {}
};

extern char gBlockMan[];
extern char BXrandom[];
extern char AIrandom[];
extern char tRGB_kRed[], tRGB_kBlue[], tRGB_kYellow[], tRGB_kPurple[], tRGB_kGreen[], tRGB_kOrange[], tRGB_kBlack[], tRGB_kWhite[], tRGB_kGray[];
extern char D_004A5958;
extern void* D_004A3E94;
extern char t2vect_Zero[], t2vect_XAxis[], t2vect_YAxis[];
extern char t3vect_Zero[], t3vect_XAxis[], t3vect_YAxis[], t3vect_ZAxis[];
extern char t4vect_Zero[], t4vect_Origin[], t4vect_XAxis[], t4vect_YAxis[], t4vect_ZAxis[];
extern char t3mat_Identity[], t4mat_Identity[], t4mat_Zero[], tquat_Identity[], tPose_Identity[], TheDynamicMemMan[];

/**
 * Static initialisation and destruction of the bx library's globals.
 *
 * Only for priority DEFAULT_INIT_PRIORITY. With @p init: cBlockMan_cBlockMan on gBlockMan, cRandom_init (cRandom
 * constructor) on BXrandom and AIrandom, then the colour/vector/matrix constants and tPose_Identity, and
 * cDynamicMemMan_cDynamicMemMan constructs TheDynamicMemMan. Without: cDynamicMemMan__cDynamicMemMan destroys
 * TheDynamicMemMan.
 *
 * @param init  nonzero to construct, zero to destroy
 * @param prio  the init priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void __sti__all_in_one_bx_cpp(int init, int prio)
{
    if (prio == DEFAULT_INIT_PRIORITY) {
        if (init != 0) {
            cBlockMan_cBlockMan(gBlockMan);
            cRandom_init(BXrandom);
            cRandom_init(AIrandom);
            new (tRGB_kRed) sV3_0550(1.0f, 0.0f, 0.0f);
            new (tRGB_kBlue) sV3_0550(0.0f, 0.0f, 1.0f);
            new (tRGB_kYellow) sV3_0550(1.0f, 1.0f, 0.0f);
            new (tRGB_kPurple) sV3_0550(1.0f, 0.0f, 1.0f);
            new (tRGB_kGreen) sV3_0550(0.0f, 1.0f, 0.0f);
            new (tRGB_kOrange) sV3_0550(1.0f, 0.5f, 0.0f);
            new (tRGB_kBlack) sV3_0550(0.0f, 0.0f, 0.0f);
            new (tRGB_kWhite) sV3_0550(1.0f, 1.0f, 1.0f);
            new (tRGB_kGray) sV3_0550(0.25f, 0.25f, 0.25f);
            D_004A3E94 = &D_004A5958;
            new (t2vect_Zero) sV2_0550(0.0f, 0.0f);
            new (t2vect_XAxis) sV2_0550(1.0f, 0.0f);
            new (t2vect_YAxis) sV2_0550(0.0f, 1.0f);
            new (t3vect_Zero) sV3_0550(0.0f, 0.0f, 0.0f);
            new (t3vect_XAxis) sV3_0550(1.0f, 0.0f, 0.0f);
            new (t3vect_YAxis) sV3_0550(0.0f, 1.0f, 0.0f);
            new (t3vect_ZAxis) sV3_0550(0.0f, 0.0f, 1.0f);
            new (t4vect_Zero) sV4_0550(0.0f, 0.0f, 0.0f, 0.0f);
            new (t4vect_Origin) sV4_0550(0.0f, 0.0f, 0.0f, 1.0f);
            new (t4vect_XAxis) sV4_0550(1.0f, 0.0f, 0.0f, 0.0f);
            new (t4vect_YAxis) sV4_0550(0.0f, 1.0f, 0.0f, 0.0f);
            new (t4vect_ZAxis) sV4_0550(0.0f, 0.0f, 1.0f, 0.0f);
            new (t3mat_Identity) sM33_0550();
            new (t4mat_Identity) sM44_0550(1.0f);
            new (t4mat_Zero) sM44_0550(0.0f);
            new (tquat_Identity) sQ_0550(0.0f, 0.0f, 0.0f, 1.0f);
            new (tPose_Identity) sXf_0550(*(sV4_0550*)t4vect_Origin, *(sQ_0550*)tquat_Identity);
            cDynamicMemMan_cDynamicMemMan(TheDynamicMemMan);
        } else {
            cDynamicMemMan__cDynamicMemMan(TheDynamicMemMan, 2);
        }
    }
}
#endif
