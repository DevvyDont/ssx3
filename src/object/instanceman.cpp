#include "vtable.h"
#include "common.h"
#include "world/worldview.h"
#include "world/worldcache.h"
#include "world/worldrecords.h"
#include "bx/math3d.h"
#include "bx/appman.h"
#include "object/instanceman.h"
#include "object/animframe.h"
#include "object/meshanimnode.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"

extern "C" void MEM_copy(void* dst, void* src, int size);

struct sInstanceStruct {
    char pad_0x00[0x8];
    int field_0x8;
    char pad_0xC[0x78 - 0xC];
    void* field_0x78;
};

//100%
INCLUDE_ASM("object/instanceman", cInstanceMan_copyInstance__FPvP15sInstanceStructT0);
#ifdef SKIP_ASM
/**
 * Copies one instance record over another, keeping the destination's word at +0x78 and setting its flag
 * 0x2000.
 *
 * MEM_copy of 0xA0 bytes, then the saved +0x78 word (probably the record's own id; multi-spline replays save
 * it per copy) is put back and bit 0x2000 of the flags (+0x8) is set. The manager itself is not used.
 *
 * @param dst  record to overwrite
 * @param src  record to copy from
 */
void cInstanceMan_copyInstance(void* vself, sInstanceStruct* dst, void* src)
{
    cInstanceMan* self = (cInstanceMan*)vself;
    void* saved = dst->field_0x78;
    MEM_copy(dst, src, 160);
    dst->field_0x78 = saved;
    dst->field_0x8 |= 0x2000;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", func_003512C0);
#ifdef SKIP_ASM
struct sInstHandle {
    unsigned int gen : 8;
    unsigned int index : 24;
};

struct sInstSlot {
    char pad_0x00[0x8];
    int flags;              // 0x08
    char pad_0x0C[0x6C];    // 0x0C
    sInstHandle handle;     // 0x78
    char pad_0x7C[0x34];    // 0x7C
};

struct sInstPool {
    int gen;                // 0x0
    char pad_0x4[0x8];      // 0x4
    sInstSlot* slots;       // 0xC
};

/**
 * Claims the dynamic instance record with a given id.
 *
 * When the id's low byte matches the manager's section id (+0x0), takes record id >> 8 from the record array (+0xC,
 * 0xB0 bytes each), stamps its id (+0x78: section byte, index) and sets flag 0x2000 (in use).
 *
 * @param handle  instance id (tResourceID)
 * @return the record, or 0 when the id belongs to another section
 */
extern "C" sInstSlot* func_003512C0(sInstPool* self, unsigned int handle)
{
    if ((handle & 0xFF) == self->gen) {
        sInstSlot* slot = &self->slots[handle >> 8];
        slot->handle.gen = self->gen;
        slot->handle.index = slot - self->slots;
        slot->flags |= 0x2000;
        return slot;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_cAnimFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cAnimFrame_init(cAnimFrame* self);
extern "C" void cAnimFrame_setAnimRecord(cAnimFrame* self, float* src);
void cAnimFrame_update_impl(char* self, float t) __asm__("cAnimFrame_update");
extern char cAnimFrame_vtbl[];

extern "C" void* cAnimFrame_cAnimFrame_impl(char* self, float* src, float f) __asm__("cAnimFrame_cAnimFrame");
/**
 * Constructs an animation frame for an anim record at a start time.
 *
 * Installs cAnimFrame_vtbl (+0x84), clears the frame (cAnimFrame_init), attaches the record (cAnimFrame_setAnimRecord) and evaluates
 * it at time (cAnimFrame_update).
 *
 * @param src  anim record (tAnimRecord)
 * @param time  time to evaluate at
 * @return self
 */
extern "C" void* cAnimFrame_cAnimFrame_impl(char* vself, float* src, float time)
{
    cAnimFrame* self = (cAnimFrame*)vself;
    self->mVtbl = (void**)cAnimFrame_vtbl;
    cAnimFrame_init((cAnimFrame*)self);
    cAnimFrame_setAnimRecord((cAnimFrame*)self, src);
    cAnimFrame_update_impl(((char*)self), time);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_setAnimRecord);
#ifdef SKIP_ASM
/**
 * Attaches an anim record to an animation frame and copies its first six values.
 *
 * Stores src at +0x40 and src[0..5] at +0x44..+0x58.
 *
 * @param src  anim record (tAnimRecord)
 */
extern "C" void cAnimFrame_setAnimRecord(cAnimFrame* self, float* src)
{
    self->AnimRecord = (const tAnimRecord*)src;
    self->Vals[0] = src[0];
    self->Vals[1] = src[1];
    self->Vals[2] = src[2];
    self->Vals[3] = src[3];
    self->Vals[4] = src[4];
    self->Vals[5] = src[5];
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_init);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int value, int size);

/**
 * Clears an animation frame: zeroes its two 0x40-byte value blocks (+0x0, +0x44) and the record pointer (+0x40).
 */
extern "C" void cAnimFrame_init(cAnimFrame* self)
{
    MEM_fill(self, 0, 0x40);
    MEM_fill(&self->Vals[0], 0, 0x40);
    self->AnimRecord = 0;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_writeToReplayFrame);
#ifdef SKIP_ASM
class cInstSink {
public:
    virtual void write(void *p, int n);
    virtual void read(void *p, int n);
};

/**
 * Writes an animation frame's 0x40-byte value block to a replay stream (stream vtable entry 1).
 *
 * @param src  the animation frame
 * @param sink  replay stream to write to
 */
extern "C" void cAnimFrame_writeToReplayFrame(cAnimFrame* src, cInstSink *sink) {
    sink->write(src, 0x40);
}
#endif
//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads an animation frame's 0x40-byte value block back from a replay stream (stream vtable entry 2).
 *
 * @param src  the animation frame
 * @param sink  replay stream to read from
 */
extern "C" void cAnimFrame_readFromReplayFrame(cAnimFrame* src, cInstSink *sink) {
    sink->read(src, 0x40);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/instanceman", cMeshAnimFrame_cMeshAnimFrame);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_351480(t4Matrix* d, const t4Matrix* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

extern void* cMeshAnimFrame_vtbl[];
extern t4Matrix t4mat_Identity;
extern "C" void* cAnimFrame_cAnimFrame(void* self, float* src, float f);
// PORT: cMeshAnimFrame_update is defined later in the unit with a typed self; bound by asm label.
void cMeshAnimFrame_update_vf(void* self, float f) __asm__("cMeshAnimFrame_update");

/**
 * Constructs a mesh animation frame for an anim record at a start time.
 *
 * Runs the cAnimFrame constructor cAnimFrame_cAnimFrame, installs cMeshAnimFrame_vtbl, sets the frame matrix (+0x90) to
 * identity, stores the record at +0x40 again and evaluates it (cMeshAnimFrame_update).
 *
 * @param src  anim record
 * @param time  time to evaluate at
 * @return self
 */
extern "C" void* cMeshAnimFrame_cMeshAnimFrame(cMeshAnimFrame* self, float* src, float time)
{
    cAnimFrame_cAnimFrame(self, src, time);
    self->mVtbl = cMeshAnimFrame_vtbl;
    vu0CopyMat_351480(((&self->FrameMatrix)), &t4mat_Identity);
    self->AnimRecord = (const tAnimRecord*)src;
    cMeshAnimFrame_update_vf(self, time);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_getStartTime);
#ifdef SKIP_ASM
/**
 * Returns the time an animation frame's animation starts: the start of its first channel's first segment.
 *
 * Follows AnimRecord (+0x40) to its channel list (+0x20); the first channel's segment array (+0x4) gives the start
 * (+0x10) of segment 0. Returns 0.0 when the channel or its segments are missing. Likely bug (kept for matching): the
 * first channel pointer is read before the list pointer is tested for null.
 *
 * @return the start time in frames, or 0.0
 */
// PORT: the original reads *a before checking a for null.
extern "C" float cAnimFrame_getStartTime(cAnimFrame* self)
{
    float startTime = 0.0f;
    char** channelList = (char**)self->AnimRecord->m_pChannels;
    tChannelRecord* channel = (tChannelRecord*)(*channelList);
    if (channelList != 0 && ((char*)channel) != 0) {
        tSegmentRecord* segments = channel->m_Segments;
        if (((char*)segments) != 0) {
            startTime = segments->m_fStart;
        }
    }
    return startTime;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_update);
#ifdef SKIP_ASM
struct sImRange;
struct sImRangeTable;
extern "C" sImRange* cAnimChannel_nextSegment(sImRangeTable* t, int* cur, float x);


struct sImCurve_351538 {
    int count;
    tSegmentRecord* segs;
};

static inline bool inRange_351538(tSegmentRecord* e, float x)
{
    return e->m_fStart <= x && x < e->m_fEnd;
}

// PORT: the unit declares cAnimFrame_update(void*) for its caller; the body also reads t from $f12
void cAnimFrame_update_impl(char* self, float t) __asm__("cAnimFrame_update");

/**
 * Evaluates the animation's channels at a time into the frame's 16 values.
 *
 * For each of the 16 channels whose bit is set in the anim record's mask (+0x18) it takes the channel's current
 * cubic segment (the record's curve table at +0x20, the frame's segment index per channel at +0x0), moves to the
 * segment holding @p time when needed (cAnimChannel_nextSegment, On Tour cAnimChannel::nextSegment) and stores ((a t + b) t + c)
 * t + d in Vals[i] (+0x44). Channels whose bit is clear keep their value.
 *
 * @param time  the time to evaluate at (On Tour time)
 */
void cAnimFrame_update_impl(char* vself, float time)
{
    cAnimFrame* self = (cAnimFrame*)vself;
    sImCurve_351538** curves = (sImCurve_351538**)self->AnimRecord->m_pChannels;
    int* cur = (int*)self;
    unsigned int mask = 1;
    float* out = (float*)(&self->Vals[0]);
    for (int i = 0; i < 16; i++) {
        sImCurve_351538* curve = *curves;
        if ((unsigned int)self->AnimRecord->m_ChannelMask & mask) {
            tSegmentRecord* segment = &curve->segs[*cur];
            if (!inRange_351538(segment, time)) {
                segment = (tSegmentRecord*)cAnimChannel_nextSegment((sImRangeTable*)curve, cur, time);
            }
            *out = ((segment->m_fvInv.m[0] * time + segment->m_fvInv.m[1]) * time + segment->m_fvInv.m[2]) * time + segment->m_fvInv.m[3];
            curves++;
            cur++;
        }
        mask <<= 1;
        out++;
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_getDeriv);
#ifdef SKIP_ASM
extern "C" sImRange* cAnimChannel_nextSegment(sImRangeTable* t, int* cur, float x);


struct sImCurve_351660 {
    int count;
    tSegmentRecord* segs;
};

struct sImV4_351660 {
    float x, y, z, w;
    sImV4_351660() {}
    sImV4_351660(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

static inline bool inRange_351660(tSegmentRecord* e, float x)
{
    return e->m_fStart <= x && x < e->m_fEnd;
}

/**
 * Evaluates an animation frame's rate of change at a time: translation and rotation per frame.
 *
 * For each of the 16 channels (bit i of the record's mask at AnimRecord +0x18) it uses the channel's current segment
 * (index array at +0x0), stepping to the right one with cAnimChannel_nextSegment when the time is outside it, and evaluates the
 * cubic's derivative 3a t^2 + 2b t + c; channels not in the mask give 0. Channels 0-2 are the translation rates,
 * 3-5 the angle rates (negated).
 *
 * @param vel     receives the translation rate (w = 0) (On Tour dTrans)
 * @param rotVel  receives the negated angle rates (w = 0) (On Tour dAng)
 * @param time       the animation time
 */
extern "C" void cAnimFrame_getDeriv(cAnimFrame* self, sImV4_351660* vel, sImV4_351660* rotVel, float time)
{
    float out[16];
    sImCurve_351660** curves = (sImCurve_351660**)self->AnimRecord->m_pChannels;
    int* cur = (int*)self;
    int mask = 1;
    float* outRate = out;
    for (int i = 0; i < 16; i++, mask <<= 1, outRate++) {
        sImCurve_351660* channel = *curves;
        if (self->AnimRecord->m_ChannelMask & mask) {
            tSegmentRecord* segment = &channel->segs[*cur];
            if (!inRange_351660(segment, time)) {
                segment = (tSegmentRecord*)cAnimChannel_nextSegment((sImRangeTable*)channel, cur, time);
            }
            curves++;
            cur++;
            *outRate = segment->m_fvInv.m[0] * (time * 3.0f) * time + 2.0f * segment->m_fvInv.m[1] * time + segment->m_fvInv.m[2];
        } else {
            *outRate = 0.0f;
        }
    }
    *vel = sImV4_351660(out[0], out[1], out[2], 0.0f);
    *rotVel = sImV4_351660(-out[3], -out[4], -out[5], 0.0f);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimFrame_update);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void* cAnimFrame_update(void* self);


/**
 * Evaluates a mesh animation frame and rebuilds its transform from the rotation (degrees) and translation channels.
 *
 * Calls cAnimFrame_update(self) without a time: On Tour's update(float time) passes it on, and here it reaches
 * cAnimFrame_update untouched in $f12 (a port must pass it). The matrix is the X-Y-Z Euler rotation of minus the
 * rotation values (converted to radians, fSinCos sine / cosine) with the translation in row 3.
 */
extern "C" void cMeshAnimFrame_update(cMeshAnimFrame* self)
{
    cAnimFrame_update(self);
    float rotRad[3];
    float sc[3][2];
    rotRad[0] = -((float*)&self->Vals[3])[0] * 0.01745329424738884f;
    rotRad[1] = -((float*)&self->Vals[3])[1] * 0.01745329424738884f;
    rotRad[2] = -((float*)&self->Vals[3])[2] * 0.01745329424738884f;
    fSinCos(&sc[0][0], &sc[0][1], rotRad[0]);
    fSinCos(&sc[1][0], &sc[1][1], rotRad[1]);
    fSinCos(&sc[2][0], &sc[2][1], rotRad[2]);
    float sx = sc[0][0];
    float sy = sc[1][0];
    float cx = sc[0][1];
    float sz = sc[2][0];
    float cz = sc[2][1];
    float cy = sc[1][1];
    float sxsy = sx * sy;
    float cxsy = cx * sy;
    float cxsz = cx * sz;
    ((float (*)[4])&self->FrameMatrix)[0][0] = cy * cz;
    ((float (*)[4])&self->FrameMatrix)[0][1] = -cy * sz;
    ((float (*)[4])&self->FrameMatrix)[0][2] = sy;
    ((float (*)[4])&self->FrameMatrix)[0][3] = 0.0f;
    ((float (*)[4])&self->FrameMatrix)[1][0] = sxsy * cz + cxsz;
    ((float (*)[4])&self->FrameMatrix)[1][1] = cx * cz - sxsy * sz;
    ((float (*)[4])&self->FrameMatrix)[1][2] = -sx * cy;
    ((float (*)[4])&self->FrameMatrix)[1][3] = 0.0f;
    ((float (*)[4])&self->FrameMatrix)[2][0] = sx * sz - cxsy * cz;
    ((float (*)[4])&self->FrameMatrix)[2][1] = cxsz * sy + sx * cz;
    ((float (*)[4])&self->FrameMatrix)[2][2] = cx * cy;
    ((float (*)[4])&self->FrameMatrix)[2][3] = 0.0f;
    ((float (*)[4])&self->FrameMatrix)[3][0] = self->Vals[0];
    ((float (*)[4])&self->FrameMatrix)[3][1] = self->Vals[1];
    ((float (*)[4])&self->FrameMatrix)[3][2] = self->Vals[2];
    ((float (*)[4])&self->FrameMatrix)[3][3] = 1.0f;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimFrame_transform);
#ifdef SKIP_ASM
struct sImMat44 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMatIM(sImMat44* d, const sImMat44* a, const sImMat44* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMatIM(sImMat44* d, const sImMat44* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

/**
 * Transforms a mesh animation frame's matrix by another matrix.
 *
 * Multiplies FrameMatrix (+0x90) by @p xform on VU0 and copies the product back into FrameMatrix.
 *
 * @param xform  the transform to apply (On Tour xform)
 */
extern "C" void cMeshAnimFrame_transform(cMeshAnimFrame* self, sImMat44* xform)
{
    sImMat44* dst = (sImMat44*)(&self->FrameMatrix);
    sImMat44 productCopy;
    sImMat44 product;
    vu0MulMatIM(&product, xform, dst);
    vu0CopyMatIM(&productCopy, &product);
    vu0CopyMatIM(dst, &productCopy);
}
#endif

extern "C" void* cAnimFrame_update(void* self);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/instanceman", cCamAnimFrame_cCamAnimFrame);
#ifdef SKIP_ASM
extern void* cCamAnimFrame_vtbl[];
// PORT: cCamAnimFrame_update is defined later in the unit as (void*); bound by asm label with its real float argument.
void cCamAnimFrame_update_vf(void* self, float f) __asm__("cCamAnimFrame_update__FPv");

/**
 * Constructs a camera animation frame for an anim record at a start time.
 *
 * Runs the cAnimFrame constructor cAnimFrame_cAnimFrame, installs the camera-frame vtable cCamAnimFrame_vtbl and evaluates the frame
 * (cCamAnimFrame_update).
 *
 * @param src  anim record
 * @param animTime  time to evaluate at
 * @return self
 */
extern "C" void* cCamAnimFrame_cCamAnimFrame(cCamAnimFrame* self, float* src, float animTime)
{
    cAnimFrame_cAnimFrame(self, src, animTime);
    self->mVtbl = cCamAnimFrame_vtbl;
    cCamAnimFrame_update_vf(self, animTime);
    return ((void*)self);
}
#endif

//99.29%
INCLUDE_ASM("object/instanceman", cCamAnimFrame_update__FPv);
#ifdef SKIP_ASM
/**
 * Updates a camera animation frame by forwarding to cAnimFrame_update.
 *
 * @return whatever cAnimFrame_update leaves (callers ignore it)
 */
void* cCamAnimFrame_update(void* self)
{
    return cAnimFrame_update(self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimChannel_nextSegment);
#ifdef SKIP_ASM
struct sImRange {
    char pad_0x00[0x10];
    float lo;   // 0x10
    float hi;   // 0x14
};

struct sImRangeTable {
    int count;          // 0x0
    sImRange* entries;  // 0x4
};

static inline bool sImRange_contains(sImRange* self, float x)
{
    return self->lo <= x && x < self->hi;
}

static inline bool sImRange_below(sImRange* self, float x)
{
    return x < self->hi;
}

/**
 * Finds the segment of an animation channel that holds a time, starting with the one after the current one.
 *
 * Increments @p cur; if that segment exists and lo <= @p x < hi (+0x10, +0x14) it is returned. Otherwise the search
 * restarts at segment 0 and returns the first segment whose end is past @p x, or the last one.
 *
 * @param channel    the channel: segment count +0x0, segment array +0x4 (0x18 bytes each)
 * @param cur  the current segment index; updated to the returned segment
 * @param x    the time
 * @return the segment that holds the time (the last one when none does)
 */
extern "C" sImRange* cAnimChannel_nextSegment(sImRangeTable* channel, int* cur, float x)
{
    sImRange* segment;
    int i = ++*cur;
    if (i < channel->count) {
        segment = &channel->entries[i];
        if (sImRange_contains(segment, x)) {
            return segment;
        }
    }
    segment = channel->entries;
    for (*cur = 0; *cur < channel->count - 1; ++*cur, segment++) {
        if (sImRange_below(segment, x)) {
            return segment;
        }
    }
    return segment;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_cMeshAnimNode1);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* def);
extern "C" float cObjectInterface_getUpdateTime();
extern "C" void cMeshAnimNode_init(cMeshAnimNode* self);

extern void* cMeshAnimNode_vtbl[];

struct sImV4 {
    float v[4];
    sImV4() {}
    sImV4(const float& x, const float& y, const float& z, const float& w)
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }
    sImV4(float x, float y, float z)
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = 0.0f;
    }
    // placement form for the member arrays (the unit defines the global one further down)
    static void* operator new[](unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

static inline sImV4 operator*(const sImV4& a, const sImV4& b)
{
    return sImV4(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2], a.v[3] * b.v[3]);
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sImV4 vu0ScaleIM(const sImV4& v, float s)
{
    sImV4 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddIM(sImV4& d, const sImV4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d)
        : "m"(d), "m"(b)
        : "memory");
}

// PORT: abs.s helper
static inline float absIM(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

struct sImMtx {
    float m[4][4];
    sImMtx() {}
    sImMtx(const sImMtx& o) { vu0CopyMatIM((sImMat44*)this, (const sImMat44*)&o); }
    sImV4 getRow(int i) const { return *(const sImV4*)m[i]; }
    sImMtx& operator=(const sImMtx& o)
    {
        vu0CopyMatIM((sImMat44*)this, (const sImMat44*)&o);
        return *this;
    }
} __attribute__((aligned(16)));

// parent * (local bone matrix with its translation scaled)
static inline sImMtx mulScaledIM(const sImMtx& parent, const sImMtx& src, const sImV4& s)
{
    sImMtx m;
    *(sImV4*)m.m[0] = src.getRow(0);
    *(sImV4*)m.m[1] = src.getRow(1);
    *(sImV4*)m.m[2] = src.getRow(2);
    *(sImV4*)m.m[3] = src.getRow(3) * s;
    sImMtx r;
    vu0MulMatIM((sImMat44*)&r, (const sImMat44*)&parent, (const sImMat44*)&m);
    return r;
}

struct sImVEntry {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sImVEntryI {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sImObj {
    char pad_0x00[0xC];
    sImVEntry* vt;
};

struct sImModelInst {
    char pad_0x0[0xC];
    sImObj* owner;        // 0xC
    sImObj* getOwner() { return owner; }
};

struct sImBone {
    int parent;           // 0x0
    char pad_0x4[0x8];
    sImMtx* mtx;          // 0xC
};

struct sImSkel {
    char pad_0x0[0x4];
    int count;            // 0x4
    sImBone* bones;       // 0x8
};

struct sImModel {
    char pad_0x0[0x8];
    unsigned int flags;   // 0x8
    char pad_0xC[0x74];   // 0x10: root matrix
    sImSkel* skel;        // 0x80
    float scale;          // 0x84
};

struct sImWorld3 {
    char pad_0x0[0x8];
    unsigned int** sets;  // 0x8
};

extern "C" sImWorld3** cObjectInterface_getWorld();

static inline void* refToPtrIM(unsigned int p)
{
    return (void*)(p << 2);
}

struct sImRef {
    unsigned int id;
    void* get()
    {
        unsigned int* set = (*cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        unsigned int p = (*(unsigned int**)((char*)set + 0x1C))[id >> 8] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtrIM(p);
    }
};

struct sImDef {
    char pad_0x0[0x4];
    int f4;               // 0x4
    float f8;             // 0x8
    float fC;             // 0xC
    float f10;            // 0x10
    float a[3];           // 0x14
    float b[3];           // 0x20
    float angle;          // 0x2C
    float f30;            // 0x30
    sImRef model;         // 0x34
    int bone;             // 0x38
};

struct sImPair {
    sImV4 pos;
    sImV4 rot;
};

extern "C" sImPair tPose_tPose(sImMtx* m);

struct sImInst {
    char pad_0x00[0xC];
    void* vt;             // 0xC
    char pad_0x10[0x8];
    sImModel* model;      // 0x18
    float f1C;            // 0x1C
    float f20;            // 0x20
    float f24;            // 0x24
    char pad_0x28[0x8];
    sImV4 rot[24];        // 0x30
    sImV4 arr1[24];       // 0x1B0
    char pad_0x330[0x60];
    sImV4 f390;           // 0x390
    sImV4 f3A0;           // 0x3A0
    sImV4 pos[24];        // 0x3B0
    sImV4 arr3[24];       // 0x530
    int f6B0;             // 0x6B0
    float f6B4;           // 0x6B4
};

struct sImSrc {
    char pad_0x00[0x10];
    sImV4 v;              // 0x10
    char pad_0x20[0x10];
    float s;              // 0x30
};

/**
 * Constructs a mesh-anim (break-apart) node: the instance's meshes fly off from where they are.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1 (node type 11), installs cMeshAnimNode_vtbl and constructs its 24-entry
 * Qrot, Axis, Pos and Vel arrays. From def: Type (+0x4) to +0x6B0, Drag (+0x8) to +0x6B4, Life (+0x10) to +0x1C,
 * FadeTime (+0x30) to +0x24, and AngVel (+0x2C, degrees) as radians per update to +0x20. The mesh matrices come from
 * the root instance (def RootInstId/RootMesh) or the old node owner's anim mats when it has them, else are built from
 * the model's bones scaled by the model scale; each is split into Qrot and Pos (tPose_tPose). The old node is deleted
 * (vtable entry 1, flags 3). Disperse (+0x390) and InitVel (+0x3A0) come from def, InitVel plus the hit direction
 * scaled by |speed| * VelScale when a collision is given. The record flags get bit 0x4, lose 0x2 and 0x60, then
 * cMeshAnimNode_init randomises the motion.
 *
 * @param bucket  update bucket
 * @param instRecord  instance record to break apart
 * @param def  mesh-anim make-data (tMakeMeshAnimData layout)
 * @param src  collision that caused it (direction and speed), or 0
 * @param owner  node the instance had before, deleted here, or 0
 * @return self
 */
extern "C" sImInst* cMeshAnimNode_cMeshAnimNode1(sImInst* self, void* bucket, void* instRecord, sImDef* def, sImSrc* src, sImObj* owner)
{
    cInstanceNode_cInstanceNode1(self, bucket, 11, instRecord);
    self->vt = cMeshAnimNode_vtbl;
    new (&self->rot) sImV4[24];
    new (&self->arr1) sImV4[24];
    new (&self->pos) sImV4[24];
    new (&self->arr3) sImV4[24];
    self->f6B0 = def->f4;
    self->f1C = def->f10;
    self->f24 = def->f30;
    self->f6B4 = def->f8;
    float rot = def->angle * 0.01745329424738884f;
    self->f20 = rot * cObjectInterface_getUpdateTime();
    sImSkel* skel = self->model->skel;
    sImMtx local[24];
    sImMtx* mats = local;
    sImObj* obj = owner;
    sImMtx* root = (sImMtx*)((char*)self->model + 0x10);
    if (~def->model.id != 0) {
        sImModelInst* rootInst = (sImModelInst*)def->model.get();
        if (rootInst != 0 && rootInst->getOwner() != 0) {
            obj = rootInst->getOwner();
        }
    }
    if (obj != 0) {
        sImVEntry* ownerSlot = &obj->vt[24];
        root = (sImMtx*)ownerSlot->fn(vthis(obj, *ownerSlot));
        ownerSlot = &obj->vt[26];
        if (ownerSlot->fn(vthis(obj, *ownerSlot)) != 0) {
            ownerSlot = &obj->vt[27];
            mats = (sImMtx*)ownerSlot->fn(vthis(obj, *ownerSlot));
            if (def->bone != -1) {
                root = &mats[def->bone];
                mats = local;
            }
        }
    }
    if (mats == local) {
        sImBone* boneIter = skel->bones;
        float modelScale = self->model->scale;
        sImV4 scale(modelScale, modelScale, modelScale, 1.0f);
        for (int i = 0; i < skel->count; i++, boneIter++) {
            if (boneIter->parent != -1) {
                local[i] = mulScaledIM(local[boneIter->parent], *boneIter->mtx, scale);
            } else {
                local[i] = mulScaledIM(*root, *boneIter->mtx, scale);
            }
        }
    }
    for (int i = 0; i < skel->count; i++) {
        sImPair bonePose = tPose_tPose(&mats[i]);
        self->rot[i] = bonePose.rot;
        self->pos[i] = bonePose.pos;
    }
    if (owner != 0) {
        sImVEntryI* vt = (sImVEntryI*)owner->vt;
        vt[1].fn(vthis(owner, vt[1]), 3);
    }
    self->f390 = sImV4(def->a[0], def->a[1], def->a[2]);
    self->f3A0 = sImV4(def->b[0], def->b[1], def->b[2]);
    if (src != 0) {
        vu0AddIM(self->f3A0, vu0ScaleIM(vu0ScaleIM(src->v, absIM(src->s)), def->fC));
    }
    self->model->flags = (self->model->flags & ~2u) | 4u;
    self->model->flags &= ~0x60u;
    cMeshAnimNode_init((cMeshAnimNode*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_cMeshAnimNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern void* cMeshAnimNode_vtbl[];

inline void* operator new[](unsigned int, void* p) { return p; }


struct sInstElem2168 {
    float v[4];
    sInstElem2168() {}
};

/**
 * Rebuilds a mesh-animation node (an instance whose meshes fly apart) from a replay frame.
 *
 * Runs the cInstanceNode replay constructor and stores cMeshAnimNode's vtable; the four 24-element vector
 * arrays (On Tour Qrot, Axis, Pos, Vel) have empty constructors. Reads the 0xC-byte cMeshAnimNodeState (Life,
 * AngVel, FadeTime) into +0x1C, then sets Life to -1, so update and render treat the node as finished.
 *
 * @param bucket  bucket index, passed on to cInstanceNode_cInstanceNode()
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cMeshAnimNode_cMeshAnimNode(cMeshAnimNode* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    self->mVtbl = cMeshAnimNode_vtbl;
    new ((char*)&self->Qrot[0]) sInstElem2168[24];
    new ((char*)&self->Axis[0]) sInstElem2168[24];
    new ((char*)&self->Angle[0]) sInstElem2168[24];
    new ((char*)&self->Pos[16]) sInstElem2168[24];
    sVEntry<void(void*, void*, int)>* vtEntry = &(*(sVEntry<void(void*, void*, int)>**)stream)[2];
    vtEntry->fn(vthis(stream, *vtEntry), (char*)&self->Life, 0xC);
    self->Life = -1.0f;
    return ((void*)self);
}
#endif

extern void* cMeshAnimNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode__cMeshAnimNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a mesh animation node: stores cMeshAnimNode's vtable at +0xC, then runs the cInstanceNode
 * destructor.
 *
 * The g++ delete flags in the second argument register pass through untouched to
 * cInstanceNode__cInstanceNode().
 *
 * @return no meaningful value (cInstanceNode__cInstanceNode returns nothing)
 */
void* cMeshAnimNode__cMeshAnimNode(void* vself)
{
    cMeshAnimNode* self = (cMeshAnimNode*)vself;
    self->mVtbl = (void**)((int)(void*)cMeshAnimNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_init);
#ifdef SKIP_ASM
unsigned int BXrand();

struct sV4_352230 {
    float x, y, z, w;
    sV4_352230() {}
    sV4_352230(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));


static inline float randf_352230()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float randRange_352230(float lo, float hi)
{
    return randf_352230() * (hi - lo) + lo;
}

static inline float randScale_352230(float s)
{
    return s * randf_352230();
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sV4_352230 Normalize_352230(const sV4_352230& v)
{
    sV4_352230 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_352230 Scale_352230(const sV4_352230& v, float s)
{
    sV4_352230 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void AddEq_352230(sV4_352230& d, const sV4_352230& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d)
        : "m"(d), "m"(b)
        : "memory");
}

/**
 * Randomises a mesh-anim node's per-mesh spin and velocity.
 *
 * For every mesh of the model (def +0x80, count +0x4): Angle = AngVel (+0x20) * random 0.5..1 and Axis = a random unit
 * vector. Then Vel = InitVel (+0x3A0) * cAppMan_TheAppMan +0x14 plus a random per-axis term up to Disperse (+0x390)
 * with random sign, divided by the frame rate.
 *
 * @param debris  the mesh-anim node
 */
extern "C" void cMeshAnimNode_init(cMeshAnimNode* debris)
{
    cMeshAnimNode* self = debris;
    tModelRecord* modelDef = (tModelRecord*)self->Instance->m_Model;
    for (int i = 0; i < modelDef->m_iNumMeshes; i++) {
        self->Angle[i] = self->AngVel * randRange_352230(0.5f, 1.0f);
        ((sV4_352230*)&self->Axis[0])[i] = sV4_352230(randRange_352230(-1.0f, 1.0f), randRange_352230(-1.0f, 1.0f), randRange_352230(-1.0f, 1.0f), 0.0f);
        ((sV4_352230*)&self->Axis[0])[i] = Normalize_352230(((sV4_352230*)&self->Axis[0])[i]);
    }
    for (int j = 0; j < modelDef->m_iNumMeshes; j++) {
        float vx = randScale_352230(self->Disperse.x) * (1.0f / (float)cAppMan_TheAppMan->TimerHz);
        if (BXrand() & 1) {
            vx = -vx;
        }
        float vy = randScale_352230(self->Disperse.y) * (1.0f / (float)cAppMan_TheAppMan->TimerHz);
        if (BXrand() & 1) {
            vy = -vy;
        }
        float vz = randScale_352230(self->Disperse.z) * (1.0f / (float)cAppMan_TheAppMan->TimerHz);
        ((sV4_352230*)&self->Vel[0])[j] = Scale_352230(*(sV4_352230*)&self->InitVel, cAppMan_TheAppMan->TimerPeriod);
        AddEq_352230(((sV4_352230*)&self->Vel[0])[j], sV4_352230(vx, vy, vz, 0.0f));
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_update);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

struct sV4_352500 {
    float x, y, z, w;
    sV4_352500() {}
    sV4_352500(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sQ_352500 : sV4_352500 {
    sQ_352500(float aw, float ax, float ay, float az) { w = aw; x = ax; y = ay; z = az; }
};

struct sVE_352500 { short delta; short index; void (*fn)(void*, int); };

struct sDebris_352500 {
    char pad0[0xC];
    sVE_352500* vt;         // 0xC
    char pad10[0x8];
    char* model;            // 0x18
    float timer;            // 0x1C
    char pad20[0x10];
    sV4_352500 rot[24];     // 0x30
    sV4_352500 axis[24];    // 0x1B0
    float spin[24];         // 0x330
    char pad390[0x20];
    sV4_352500 pos[24];     // 0x3B0
    sV4_352500 vel[24];     // 0x530
    int state;              // 0x6B0
    float gravity;          // 0x6B4
};

// PORT: PS2-only VU0 inline asm (quaternion multiply).
static inline sV4_352500 QMul_352500(const sV4_352500& a, const sV4_352500& b)
{
    sV4_352500 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vmul.xyzw $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w  ACC, $vf7, $vf7x\n"
        "vmsubay.w ACC, $vf0, $vf7y\n"
        "vmsubz.w  $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place normalize via rsqrt).
static inline void NormalizeEq_352500(sV4_352500& v)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        :
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void AddEq_352500(sV4_352500& d, const sV4_352500& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d)
        : "m"(d), "m"(b)
        : "memory");
}

/**
 * Moves the flying meshes of a mesh-animation node and expires the node when its life runs out.
 *
 * While Life (+0x1C) is positive it drops by the tick period; on reaching 0 the node calls its virtual expire
 * (slot 34) with runscript 1 and returns. Otherwise each mesh's Qrot (+0x30) turns by Angle[i] (+0x330) about
 * Axis[i] (+0x1B0) and is renormalised, Pos (+0x3B0) advances by Vel (+0x530) with no time step, and Vel.z
 * drops by Drag (+0x6B4) * 980 * the tick period.
 */
extern "C" void cMeshAnimNode_update(sDebris_352500* self)
{
    if (self->timer > 0.0f) {
        self->timer -= cAppMan_TheAppMan->TimerPeriod;
        if (self->timer <= 0.0f) {
            self->vt[34].fn(vthis(self, self->vt[34]), 1);
            return;
        }
    }
    if (self->timer > 0.0f) {
        tModelRecord* skel = (tModelRecord*)(*(char**)(self->model + 0x80));
        for (int i = 0; i < skel->m_iNumMeshes; i++) {
            float sinHalf, cosHalf;
            fSinCos(&sinHalf, &cosHalf, self->spin[i] * 0.5f);
            sV4_352500* quat = &self->rot[i];
            *quat = QMul_352500(*quat, sQ_352500(cosHalf, sinHalf * self->axis[i].x, sinHalf * self->axis[i].y, sinHalf * self->axis[i].z));
            NormalizeEq_352500(*quat);
        }
        for (int i = 0; i < skel->m_iNumMeshes; i++) {
            AddEq_352500(self->pos[i], self->vel[i]);
            AddEq_352500(self->vel[i], sV4_352500(0.0f, 0.0f, self->gravity * -980.0f * cAppMan_TheAppMan->TimerPeriod, 0.0f));
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_expire);
#ifdef SKIP_ASM

int cInstanceNode_runExpireScript(void* self);
extern "C" void cInstanceNode_makeThisDead(void* self);

/**
 * Expires a mesh animation node according to its end mode (+0x6B0).
 *
 * When @p check is set and cInstanceNode_runExpireScript() runs a script for the instance (probably its on-expire script),
 * nothing else happens. Mode 0 replaces the node with a new cDeadNode for the same instance (cInstanceNode_makeThisDead);
 * mode 2 deletes the node through its virtual destructor; other modes leave it.
 *
 * @param check  nonzero to give the instance's script the first chance (On Tour expire's runscript)
 */
extern "C" void cMeshAnimNode_expire(cMeshAnimNode* self, int check)
{
    if (check != 0 && cInstanceNode_runExpireScript(self) != 0) {
        return;
    }
    if (self->Type == 0) {
        cInstanceNode_makeThisDead(self);
    }
    if (self->Type == 2 && ((void*)self) != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
        vt[1].fn(vthis(self, vt[1]), 3);
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_loseFocus);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_makeThisDead(void* self);


/**
 * Handles a mesh animation node losing focus: deletes it or swaps it for a dead node, by its end mode (+0x6B0).
 *
 * A nonzero mode resets the instance record's flag word (+0x8) the way cInstanceNode_expire() does (keeps bits
 * 8-9 and the upper half, copies the upper half down, sets bit 1) and deletes the node. Mode 0 replaces the
 * node with a new cDeadNode for the same instance (cInstanceNode_makeThisDead).
 */
extern "C" void cMeshAnimNode_loseFocus(cMeshAnimNode* self)
{
    if (self->Type != 0) {
        self->Instance->m_iFlags &= 0xFFFF0300;
        self->Instance->m_iFlags |= self->Instance->m_iFlags >> 16;
        self->Instance->m_iFlags |= 2;
        if (((void*)self) != 0) {
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
            vt[1].fn(vthis(self, vt[1]), 3);
        }
    } else {
        cInstanceNode_makeThisDead(self);
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_render);
#ifdef SKIP_ASM

struct sMat_352810 {
    t4Vector r[4];
    sMat_352810() {}
} __attribute__((aligned(16)));

// tResourceID: section (8 bits) + unique id (24 bits).
struct tResourceID_352810 {
    unsigned int sid : 8;
    unsigned int uid : 24;
};
static inline void initRID_352810(tResourceID_352810* r, unsigned int sid, unsigned int uid)
{
    r->sid = sid;
    r->uid = uid;
}

// cWorldCacheTable: per stream-data-type resource tables.
struct sCacheType_352810 {
    unsigned int* entries;  // +0x4 + type * 8
    unsigned int count;     // +0x8 + type * 8
};
struct cWorld_352810 {
    cWorldCache* cache;          // 0x0
};

static inline bool tableValid_352810(cWorldCacheTable* t, int type, unsigned int uid)
{
    if (((sCacheType_352810*)&t->m_pCacheTable[0])[type].count < uid) {
        return false;
    }
    return (((sCacheType_352810*)&t->m_pCacheTable[0])[type].entries[uid] & 0xFFFFFF00) != 0;
}

static inline bool cacheValid_352810(cWorldCache* c, int type, tResourceID_352810 rid)
{
    unsigned int uiSection = rid.sid;
    unsigned int uiUID = rid.uid;
    if (uiSection == 0xFF) {
        return tableValid_352810((c->m_pSharedCacheTable), type, uiUID);
    }
    if (uiSection < (c->mp_DataHeader)->m_uiNumSections && ((cWorldCacheTable**)c->m_pSectionCacheTable)[uiSection] != 0) {
        return tableValid_352810(((cWorldCacheTable**)c->m_pSectionCacheTable)[uiSection], type, uiUID);
    }
    return false;
}

struct sVE_352810 { short delta; short index; void (*fn)(void*, void*, void*, void*, void*, int, int, float); };
struct sRCtx_352810 {
    char pad0[0x10D8];
    sVE_352810* vt;         // 0x10D8
};
extern char t4mat_Identity_2810[] __asm__("t4mat_Identity");

// cMeshAnimNode (On Tour: cMeshAnimNode::render).
struct sDebris_352810 {
    char pad0[0x18];
    char* model;            // 0x18
    float timer;            // 0x1C
    char pad20[4];
    float fade;             // 0x24
    char pad28[8];
    t4Vector rot[24];     // 0x30
    char pad1B0[0x200];
    t4Vector pos[24];     // 0x3B0
};

// PORT: PS2-only VU0 inline asm (quaternion to rotation matrix, row 3 = (0,0,0,1)).
static inline void QuatToMat_352810(sMat_352810* d, const t4Vector& q)
{
    __asm__ __volatile__(
        "lqc2       $vf4, %1\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        :
        : "r"(d), "m"(q)
        : "memory");
}

static inline void setOffset_352810(sMat_352810& m, const t4Vector& v)
{
    m.r[3] = v;
}

/**
 * Draws a mesh-animation node's meshes at their current rotations and positions, fading near the end of its
 * life.
 *
 * Draws nothing once Life (+0x1C) is 0 or less, or while any of the model's materials is not loaded in the
 * world cache (global section, type 9). Builds one matrix per mesh from Qrot and Pos and draws through the
 * graphics manager's slot 95 with an identity root and flags 0x420. The fade is 1, or Life itself (not Life /
 * FadeTime) when FadeTime (+0x24) is not negative and Life is at most FadeTime.
 */
extern "C" void cMeshAnimNode_render(cMeshAnimNode* self)
{
    if (self->Life <= 0.0f) {
        return;
    }
    int numMaterials = *(int*)(char*)self->Instance->m_Model->m_MaterialBlock;
    for (int i = 0; i < numMaterials; i++) {
        short* pMaterialRecord = ((short**)((char*)self->Instance->m_MaterialBlock + 4))[i];
        tResourceID_352810 resourceId;
        initRID_352810(&resourceId, 0xFF, *pMaterialRecord);
        if (!cacheValid_352810(cWorld_m_pTheWorld->m_pWorldCache, 9, resourceId)) {
            return;
        }
    }
    sMat_352810 animMats[24];
    tModelRecord* pModel = (tModelRecord*)self->Instance->m_Model;
    for (int i = 0; i < pModel->m_iNumMeshes; i++) {
        QuatToMat_352810(&animMats[i], ((&self->Qrot[0]))[i]);
        setOffset_352810(animMats[i], ((&self->Pos[0]))[i]);
    }
    float fade = 1.0f;
    if (self->FadeTime >= 0.0f && self->Life <= self->FadeTime) {
        fade = self->Life;
    }
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    ((sVE_352810*)ps2GraphicsMan->mVtbl)[95].fn(vthis(ps2GraphicsMan, ((sVE_352810*)ps2GraphicsMan->mVtbl)[95]), (char*)self->Instance, self->Instance->m_MaterialBlock, t4mat_Identity_2810, animMats, 0, 1056, fade);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes the mesh animation node to a replay frame: the cInstanceNode data, then 12 bytes from +0x1C.
 *
 * The extra bytes go through the stream's virtual write (slot 1).
 *
 * @param stream  the replay frame stream to write to
 */
extern "C" void cMeshAnimNode_writeToReplayFrame(cMeshAnimNode* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sVEntry<void(void*, void*, int)>* vt = *(sVEntry<void(void*, void*, int)>**)stream;
    vt[1].fn(vthis(stream, vt[1]), (char*)&self->Life, 0xC);
}
#endif

