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
#include "replay/bxreplayframe.h"
#include "object/instancenode.h"

extern "C" void MEM_copy(void* dst, void* src, int size);

// t4Vector with the constructors the original's vector class has: an empty one (the nodes' vector arrays run it per
// element) and one from four floats.
struct t4VectorCtor : public t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

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
INCLUDE_ASM("object/instanceman", cInstanceMan_getInstanceRecord);
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
extern "C" sInstSlot* cInstanceMan_getInstanceRecord(sInstPool* self, unsigned int handle)
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
    self->init();
    self->setAnimRecord(src);
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
void cAnimFrame::setAnimRecord(float* src)
{
    AnimRecord = (const tAnimRecord*)src;
    Vals[0] = src[0];
    Vals[1] = src[1];
    Vals[2] = src[2];
    Vals[3] = src[3];
    Vals[4] = src[4];
    Vals[5] = src[5];
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_init);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int value, int size);

/**
 * Clears an animation frame: zeroes its two 0x40-byte value blocks (+0x0, +0x44) and the record pointer (+0x40).
 */
void cAnimFrame::init()
{
    MEM_fill(this, 0, 0x40);
    MEM_fill(&Vals[0], 0, 0x40);
    AnimRecord = 0;
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
 * @param sink  replay stream to write to
 */
void cAnimFrame::writeToReplayFrame(cBxReplayFrame* sink)
{
    sink->write((const char*)this, 0x40);
}
#endif
//100%
INCLUDE_ASM("object/instanceman", cAnimFrame_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads an animation frame's 0x40-byte value block back from a replay stream (stream vtable entry 2).
 *
 * @param sink  replay stream to read from
 */
void cAnimFrame::readFromReplayFrame(cBxReplayFrame* sink)
{
    sink->read((char*)this, 0x40);
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
float cAnimFrame::getStartTime()
{
    float startTime = 0.0f;
    char** channelList = (char**)AnimRecord->m_pChannels;
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
void cAnimFrame::update(float time)
{
    tChannelRecord** curves = AnimRecord->m_pChannels;
    int* cur = (int*)this;
    unsigned int mask = 1;
    float* out = (float*)(&Vals[0]);
    for (int i = 0; i < 16; i++) {
        tChannelRecord* curve = *curves;
        if ((unsigned int)AnimRecord->m_ChannelMask & mask) {
            tSegmentRecord* segment = &curve->m_Segments[*cur];
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
void cAnimFrame::getDeriv(t4Vector* vel, t4Vector* rotVel, float time)
{
    float out[16];
    tChannelRecord** curves = AnimRecord->m_pChannels;
    int* cur = (int*)this;
    int mask = 1;
    float* outRate = out;
    for (int i = 0; i < 16; i++, mask <<= 1, outRate++) {
        tChannelRecord* channel = *curves;
        if (AnimRecord->m_ChannelMask & mask) {
            tSegmentRecord* segment = &channel->m_Segments[*cur];
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
    *vel = t4VectorCtor(out[0], out[1], out[2], 0.0f);
    *rotVel = t4VectorCtor(-out[3], -out[4], -out[5], 0.0f);
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
void cMeshAnimFrame::update()
{
    cAnimFrame_update(this);
    float rotRad[3];
    float sc[3][2];
    rotRad[0] = -Vals[3] * 0.01745329424738884f;
    rotRad[1] = -Vals[4] * 0.01745329424738884f;
    rotRad[2] = -Vals[5] * 0.01745329424738884f;
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
    FrameMatrix.row[0].x = cy * cz;
    FrameMatrix.row[0].y = -cy * sz;
    FrameMatrix.row[0].z = sy;
    FrameMatrix.row[0].w = 0.0f;
    FrameMatrix.row[1].x = sxsy * cz + cxsz;
    FrameMatrix.row[1].y = cx * cz - sxsy * sz;
    FrameMatrix.row[1].z = -sx * cy;
    FrameMatrix.row[1].w = 0.0f;
    FrameMatrix.row[2].x = sx * sz - cxsy * cz;
    FrameMatrix.row[2].y = cxsz * sy + sx * cz;
    FrameMatrix.row[2].z = cx * cy;
    FrameMatrix.row[2].w = 0.0f;
    FrameMatrix.row[3].x = Vals[0];
    FrameMatrix.row[3].y = Vals[1];
    FrameMatrix.row[3].z = Vals[2];
    FrameMatrix.row[3].w = 1.0f;
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
void* cCamAnimFrame::update()
{
    return cAnimFrame_update(this);
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
            if (def->bone != cMeshAnimNode::kNoRootMesh) {
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
            if (boneIter->parent != tMeshHeirarchyRecord::kNoParent) {
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
    ((cMeshAnimNode*)self)->init();
    return self;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_cMeshAnimNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern void* cMeshAnimNode_vtbl[];

inline void* operator new[](unsigned int, void* p) { return p; }



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
extern "C" void* cMeshAnimNode_cMeshAnimNode(cMeshAnimNode* self, void* bucket, cBxReplayFrame* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    VPTR_OF(cObjNode, self) = cMeshAnimNode_vtbl;
    new ((char*)&self->Qrot[0]) t4VectorCtor[24];
    new ((char*)&self->Axis[0]) t4VectorCtor[24];
    new ((char*)&self->Angle[0]) t4VectorCtor[24];
    new ((char*)&self->Pos[16]) t4VectorCtor[24];
    stream->read((char*)&self->Life, 0xC);
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
    VPTR_OF(cObjNode, self) = (void**)((int)(void*)cMeshAnimNode_vtbl);
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_init);
#ifdef SKIP_ASM
unsigned int BXrand();



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
static inline t4VectorCtor Normalize_352230(const t4Vector& v)
{
    t4VectorCtor r;
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
static inline t4VectorCtor Scale_352230(const t4Vector& v, float s)
{
    t4VectorCtor r;
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
static inline void AddEq_352230(t4Vector& d, const t4Vector& b)
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
 */
void cMeshAnimNode::init()
{
    cMeshAnimNode* self = this;
    tModelRecord* modelDef = (tModelRecord*)self->Instance->m_Model;
    for (int i = 0; i < modelDef->m_iNumMeshes; i++) {
        self->Angle[i] = self->AngVel * randRange_352230(0.5f, 1.0f);
        self->Axis[i] = t4VectorCtor(randRange_352230(-1.0f, 1.0f), randRange_352230(-1.0f, 1.0f),
                                     randRange_352230(-1.0f, 1.0f), 0.0f);
        self->Axis[i] = Normalize_352230(self->Axis[i]);
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
        self->Vel[j] = Scale_352230(self->InitVel, cAppMan_TheAppMan->TimerPeriod);
        AddEq_352230(self->Vel[j], t4VectorCtor(vx, vy, vz, 0.0f));
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_update);
#ifdef SKIP_ASM
extern "C" void fSinCos(float* s, float* c, float angle);

// t4Vector built from a quaternion's (w, x, y, z), as the rotation step is.
struct t4VectorWXYZ : public t4Vector {
    t4VectorWXYZ(float aw, float ax, float ay, float az)
    {
        w = aw;
        x = ax;
        y = ay;
        z = az;
    }
};

// PORT: PS2-only VU0 inline asm (quaternion multiply).
static inline t4VectorCtor QMul_352500(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
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
static inline void NormalizeEq_352500(t4Vector& v)
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
static inline void AddEq_352500(t4Vector& d, const t4Vector& b)
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
extern "C" void cMeshAnimNode_update(cMeshAnimNode* self)
{
    if (self->Life > 0.0f) {
        self->Life -= cAppMan_TheAppMan->TimerPeriod;
        if (self->Life <= 0.0f) {
            self->expire(1);
            return;
        }
    }
    if (self->Life > 0.0f) {
        tModelRecord* skel = self->Instance->m_Model;
        for (int i = 0; i < skel->m_iNumMeshes; i++) {
            float sinHalf, cosHalf;
            fSinCos(&sinHalf, &cosHalf, self->Angle[i] * 0.5f);
            t4Vector* quat = &self->Qrot[i];
            *quat = QMul_352500(*quat, t4VectorWXYZ(cosHalf, sinHalf * self->Axis[i].x, sinHalf * self->Axis[i].y,
                                                    sinHalf * self->Axis[i].z));
            NormalizeEq_352500(*quat);
        }
        for (int i = 0; i < skel->m_iNumMeshes; i++) {
            AddEq_352500(self->Pos[i], self->Vel[i]);
            AddEq_352500(self->Vel[i],
                         t4VectorCtor(0.0f, 0.0f, self->Drag * -980.0f * cAppMan_TheAppMan->TimerPeriod, 0.0f));
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
 * nothing else happens. kMeshAnimExpire_Dead replaces the node with a new cDeadNode for the same instance
 * (cInstanceNode_makeThisDead); kMeshAnimExpire_Delete deletes the node through its virtual destructor; other modes
 * leave it.
 *
 * @param check  nonzero to give the instance's script the first chance (On Tour expire's runscript)
 */
void cMeshAnimNode::expire(int check)
{
    if (check != 0 && cInstanceNode_runExpireScript(this) != 0) {
        return;
    }
    if (Type == cMeshAnimNode::kMeshAnimExpire_Dead) {
        cInstanceNode_makeThisDead(this);
    }
    if (Type == cMeshAnimNode::kMeshAnimExpire_Delete && ((void*)this) != 0) {
        delete this;
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
void cMeshAnimNode::loseFocus()
{
    if (Type != 0) {
        Instance->m_iFlags &= 0xFFFF0300;
        Instance->m_iFlags |= Instance->m_iFlags >> 16;
        Instance->m_iFlags |= 2;
        delete this;
    } else {
        cInstanceNode_makeThisDead(this);
    }
}
#endif

//100%
INCLUDE_ASM("object/instanceman", cMeshAnimNode_render);
#ifdef SKIP_ASM

// t4Matrix with an empty constructor: the per-mesh matrix array runs it per element, as retail does.
struct t4MatrixCtor : public t4Matrix {
    t4MatrixCtor() {}
};

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

static inline bool cacheValid_352810(cWorldCache* c, int type, tResourceID rid)
{
    unsigned int uiSection = rid.m_bfRID.m_uiSectionID;
    unsigned int uiUID = rid.m_bfRID.m_uiUID;
    if (uiSection == 0xFF) {
        return tableValid_352810((c->m_pSharedCacheTable), type, uiUID);
    }
    if (uiSection < (c->mp_DataHeader)->m_uiNumSections && ((cWorldCacheTable**)c->m_pSectionCacheTable)[uiSection] != 0) {
        return tableValid_352810(((cWorldCacheTable**)c->m_pSectionCacheTable)[uiSection], type, uiUID);
    }
    return false;
}

// The graphics manager's RenderModel (slot 95 of render/graphicsman.h) as this caller makes it: the opacity, the one
// float argument, travels in $f12 wherever it is listed.
typedef sVEntry<void(void*, const tInstanceRecord*, const tMaterialBlockRecord*, const t4Matrix*, const t4Matrix*,
                     const t4Matrix*, int, float)> tRenderModelSlot;
extern t4Matrix t4mat_Identity;

// PORT: PS2-only VU0 inline asm (quaternion to rotation matrix, row 3 = (0,0,0,1)).
static inline void QuatToMat_352810(t4Matrix* d, const t4Vector& q)
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

static inline void setOffset_352810(t4Matrix& m, const t4Vector& v)
{
    m.row[3] = v;
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
void cMeshAnimNode::render()
{
    if (Life <= 0.0f) {
        return;
    }
    int numMaterials = Instance->m_Model->m_MaterialBlock->m_iNumMaterials;
    for (int i = 0; i < numMaterials; i++) {
        tMaterialRecord* pMaterialRecord = Instance->m_MaterialBlock->GetMaterialRecordList()[i];
        tResourceID resourceId(0xFF, pMaterialRecord->m_sTexture);
        if (!cacheValid_352810(cWorld_m_pTheWorld->m_pWorldCache, 9, resourceId)) {
            return;
        }
    }
    t4MatrixCtor animMats[24];
    tModelRecord* pModel = (tModelRecord*)Instance->m_Model;
    for (int i = 0; i < pModel->m_iNumMeshes; i++) {
        QuatToMat_352810(&animMats[i], ((&Qrot[0]))[i]);
        setOffset_352810(animMats[i], ((&Pos[0]))[i]);
    }
    float fade = 1.0f;
    if (FadeTime >= 0.0f && Life <= FadeTime) {
        fade = Life;
    }
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    tRenderModelSlot* vt = (tRenderModelSlot*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    vt[95].fn(vthis(ps2GraphicsMan, vt[95]), Instance, Instance->m_MaterialBlock, &t4mat_Identity, animMats,
              0, 1056, fade);
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
void cMeshAnimNode::writeToReplayFrame(cBxReplayFrame* stream)
{
    cInstanceNode_writeToReplayFrame(this, stream);
    stream->write((const char*)&Life, 0xC);
}
#endif

