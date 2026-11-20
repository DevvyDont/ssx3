#include "common.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_cDebounceNode);
#ifdef SKIP_ASM
struct sDebounceVEntry2D10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cMoveNode_cMoveNode(void* self, void*, void*);
extern char cDebounceNode_vtbl[];

/**
 * Constructs a debounce node from a replay frame.
 *
 * Runs the cMoveNode replay constructor, then reads the countdown (+0x2C) and the expire mode (+0x30) through
 * the frame's read method (slot 2).
 *
 * @param bucket  update bucket
 * @param stream  the replay frame to read (On Tour cBxReplayFrame*)
 * @return self
 */
extern "C" void* cDebounceNode_cDebounceNode(void* self, void* bucket, void* stream)
{
    cMoveNode_cMoveNode(self, bucket, stream);
    *(void**)((char*)self + 0xC) = cDebounceNode_vtbl;
    sDebounceVEntry2D10* readEntry = &(*(sDebounceVEntry2D10**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x2C, 4);
    readEntry = &(*(sDebounceVEntry2D10**)stream)[2];
    readEntry->fn((char*)stream + readEntry->delta, (char*)self + 0x30, 4);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_updateNode);
#ifdef SKIP_ASM
struct sDebounceVEntry2D88 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Counts down the debounce node's timer and expires the node when it runs out.
 *
 * When the counter at offset 0x2C is positive it is decremented; on reaching 0 the node's expire virtual
 * (slot 34, cDebounceNode_expire) is called with 1.
 *
 * @return 0 on the tick the node expires, otherwise 1
 */
extern "C" int cDebounceNode_updateNode(void* self)
{
    int n = *(int*)((char*)self + 0x2C);
    if (n > 0) {
        n--;
        *(int*)((char*)self + 0x2C) = n;
        if (n == 0) {
            sDebounceVEntry2D88* vt = *(sDebounceVEntry2D88**)((char*)self + 0xC);
            vt[34].fn((char*)self + vt[34].delta, 1);
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_loseFocus);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cDeadNode_cDeadNode(void* self, void* parent);
extern const char str_DeadNode_b[];

struct sDebounceVEntry2DD8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Removes a debounce node when its object loses focus, unless its countdown is negative.
 *
 * The node deletes itself; with expire mode 0 (+0x30) the instance then gets a new dead node ("DeadNode").
 */
extern "C" void cDebounceNode_loseFocus(void* self)
{
    if (*(int*)((char*)self + 0x2C) >= 0) {
        if (*(int*)((char*)self + 0x30) == 0) {
            void* parent = *(void**)((char*)self + 0x18);
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
            cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode_b, 0x20000000, 0), parent);
        } else {
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
        }
    }
}
#endif

extern "C" void* cMoveNode_signal(void* self);

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_signal__FPv);
#ifdef SKIP_ASM
/**
 * Passes a signal to the base cMoveNode_signal(), which forwards it to the node's modifier block if it has one.
 *
 * Only @p self is declared; the signal's further arguments reach cMoveNode_signal() unchanged in their
 * registers.
 *
 * @return no meaningful value (cMoveNode_signal() returns nothing)
 */
void* cDebounceNode_signal(void* self)
{
    return cMoveNode_signal(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_expire);
#ifdef SKIP_ASM
int cInstanceNode_runExpireScript(void* self);
extern "C" void* cRestoreNode_cRestoreNode(void* self, void* parent);
extern const char str_RestoreNode_b[];

/**
 * Ends a debounce node and replaces it according to its expire mode.
 *
 * Does nothing when @p check is set and the instance's expire script runs instead (cInstanceNode_runExpireScript), or when
 * the countdown is negative. Otherwise the node deletes itself and the instance gets a dead node (mode 0), a
 * restore node (mode 3, "RestoreNode") or no new node (other modes).
 *
 * @param check  nonzero to run the instance's script first (On Tour runscript)
 */
extern "C" void cDebounceNode_expire(void* self, int check)
{
    if (check != 0 && cInstanceNode_runExpireScript(self) != 0) {
        return;
    }
    if (*(int*)((char*)self + 0x2C) >= 0) {
        int mode = *(int*)((char*)self + 0x30);
        if (mode == 0) {
            void* parent = *(void**)((char*)self + 0x18);
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
            cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode_b, 0x20000000, 0), parent);
        } else if (mode == 3) {
            void* parent = *(void**)((char*)self + 0x18);
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
            cRestoreNode_cRestoreNode(cMemMan_alloc(0x1C, str_RestoreNode_b, 0x20000000, 0), parent);
        } else {
            if (self != 0) {
                sDebounceVEntry2DD8* vt = *(sDebounceVEntry2DD8**)((char*)self + 0xC);
                vt[1].fn((char*)self + vt[1].delta, 3);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cDebounceNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sDebounceVEntry2FA8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cMoveNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a debounce node's state to a replay frame.
 *
 * Writes the cMoveNode part, then the countdown (+0x2C) and the expire mode (+0x30) through the frame's write
 * method (slot 1).
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
extern "C" void cDebounceNode_writeToReplayFrame(void* self, void* stream)
{
    cMoveNode_writeToReplayFrame(self, stream);
    sDebounceVEntry2FA8* writeEntry = &(*(sDebounceVEntry2FA8**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x2C, 4);
    writeEntry = &(*(sDebounceVEntry2FA8**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)self + 0x30, 4);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_cConveyorNode);
#ifdef SKIP_ASM
struct sVec2_3010 {
    float x;
    float y;
};

struct sDesc_3010 {
    int f0;
    float f4;
    float f8;
    float fC;
    int f10[8];
};

struct sFlags_3010 {
    char pad_0x00[0x8];
    unsigned int w;
};

struct sDebounce_3010 {
    char pad_0x00[0xC];
    char* vt;               // 0xC
    char pad_0x10[0x8];
    sFlags_3010* flags;     // 0x18
    float f1C;              // 0x1C
    unsigned char c20[8];   // 0x20
    char pad_0x28[0x10];
    sVec2_3010 v38;         // 0x38
    int f40;
    int f44;
};

extern char cConveyorNode_vtbl[];
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);

/**
 * Constructs a conveyor node (node type 0xF) for a placed instance from its make-data.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs the conveyor vtable cConveyorNode_vtbl, then fills the node state at
 * +0x1C: Vel (+0x1C) from desc +0x4, UVScroll (+0x38) from desc +0x8/+0xC, UV (+0x40/+0x44) zeroed, and the first 8
 * Rotate flags (+0x20) from desc +0x10..+0x17 (entries 8 and up are left as they are). Finally clears bit 0x2 and sets
 * bit 0x4 in the instance record's flags.
 *
 * @param bucket  update bucket the node joins
 * @param instRecord  instance record the node drives
 * @param desc  conveyor make-data (Vel, UScroll, VScroll, Rotate0..7)
 * @return self
 */
extern "C" void* cConveyorNode_cConveyorNode(sDebounce_3010* self, void* bucket, void* instRecord, sDesc_3010* desc)
{
    cInstanceNode_cInstanceNode1(self, bucket, 0xF, instRecord);
    self->vt = cConveyorNode_vtbl;
    self->f1C = desc->f4;
    sVec2_3010 uvScroll;
    uvScroll.x = desc->f8;
    uvScroll.y = desc->fC;
    self->v38 = uvScroll;
    self->f40 = 0;
    self->f44 = 0;
    for (int i = 0; i < 8; i++) {
        self->c20[i] = desc->f10[i];
    }
    self->flags->w = (self->flags->w & ~2) | 4;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_cConveyorNode1);
#ifdef SKIP_ASM
struct sDebounceVEntry30D0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cInstanceNode_cInstanceNode(void* self, void*, void*);
extern char cConveyorNode_vtbl[];

/**
 * Constructs a conveyor node from a replay frame.
 *
 * Runs the replay constructor cInstanceNode_cInstanceNode, installs the conveyor vtable cConveyorNode_vtbl and reads the
 * 44-byte node state (+0x1C..+0x47: Vel, Rotate flags, UVScroll, UV) through the stream's read slot (vtable entry 2).
 *
 * @param bucket  update bucket, passed on to cInstanceNode_cInstanceNode
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cConveyorNode_cConveyorNode1(void* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    *(void**)((char*)self + 0xC) = cConveyorNode_vtbl;
    sDebounceVEntry30D0* vt = *(sDebounceVEntry30D0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x1C, 44);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_updateIntersectInfo);
#ifdef SKIP_ASM
// PORT: 128-bit TImode quadword (the PC port needs a 16-byte struct).
typedef int cQuad3130 __attribute__((mode(TI)));
extern cQuad3130 t4vect_YAxis;

struct sVec3130 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat3130 {
    sVec3130 r[4];
    sMat3130() {}
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat3130(const sMat3130& o)
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
            : "r"(this), "r"(&o)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat3130& operator=(const sMat3130& o)
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
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix * matrix).
static inline sMat3130 mulMat3130(const sMat3130& a, const sMat3130& b)
{
    sMat3130 r;
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
        : "r"(&r), "r"(&a), "r"(&b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (load a vector into vf8).
static inline void loadVf8_3130(const sVec3130& v)
{
    __asm__ __volatile__("lqc2      $vf8, 0x0(%0)\n" : : "r"(&v));
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vf8).
static inline sVec3130 mulVec3130(const sMat3130& m)
{
    sVec3130 r;
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(&m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec3130 sub3130(const sVec3130& a, const sVec3130& b)
{
    sVec3130 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place a += b).
static inline void addEq3130(sVec3130& a, const sVec3130& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVec3130 scale3130(const sVec3130& v, float s)
{
    sVec3130 r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec3130 cross3130(const sVec3130& a, const sVec3130& b)
{
    sVec3130 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float len3130(const sVec3130& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec3130 norm3130(const sVec3130& v)
{
    sVec3130 r;
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

static inline sVec3130 pos3130(const sMat3130& m)
{
    return m.r[3];
}

struct sBone3130 {
    int parent;                 // 0x0
    int pad4;                   // 0x4
    int pad8;                   // 0x8
    sMat3130* mat;              // 0xC
};

struct sVE3130 {
    short delta;
    short index;
    sMat3130* (*fn)(void*);
};

struct sHit3130 {
    sVec3130 pos;               // 0x00
    sVec3130 dir;               // 0x10
    sVec3130 angVel;            // 0x20
    sVec3130 vel;               // 0x30
    char pad40[0x1C];           // 0x40
    int bone;                   // 0x5C
};

struct sNode3130 {
    char pad0[0xC];
    sVE3130* vt;                // 0x0C
    char pad10[0x8];            // 0x10
    char* model;                // 0x18
    float strength;             // 0x1C
    signed char flags[32];      // 0x20
};

/**
 * Applies the conveyor's push to a rider's intersect info for the bone it hit.
 *
 * Does nothing when the hit bone (+0x5C) is negative. Otherwise builds the bone's world matrix: its local matrix times
 * each parent's (model def +0x80, bone table +0x8, 16-byte bones with parent and matrix), then the node's root
 * transform (vtable entry 24). If the bone's Rotate flag (+0x20 + bone) is set, the push is linear: impulse = hit
 * direction (+0x10) scaled by Vel (+0x1C) over the distance from the bone origin to the hit point; impulse is added to
 * the velocity (+0x30) and offset cross impulse to the angular term (+0x20). If the flag is clear, only (bone Y axis
 * cross hit direction) scaled by Vel is added to +0x20.
 *
 * @param hit  intersect info of the collision (position, direction, accumulators, bone index)
 */
extern "C" void cConveyorNode_updateIntersectInfo(sNode3130* self, sHit3130* hit)
{
    if (hit->bone < 0) {
        return;
    }
    sMat3130* w = self->vt[24].fn((char*)self + self->vt[24].delta);
    sBone3130* bones = *(sBone3130**)(*(char**)(self->model + 0x80) + 0x8);
    sMat3130 boneMat = *bones[hit->bone].mat;
    for (int parentIndex = bones[hit->bone].parent; parentIndex != -1; parentIndex = bones[parentIndex].parent) {
        boneMat = mulMat3130(*bones[parentIndex].mat, boneMat);
    }
    boneMat = mulMat3130(*w, boneMat);
    if (self->flags[hit->bone] != 0) {
        sVec3130 offset = sub3130(hit->pos, pos3130(boneMat));
        float k = self->strength / len3130(offset);
        sVec3130 impulse = scale3130(hit->dir, k);
        sVec3130 torque = cross3130(offset, impulse);
        addEq3130(hit->angVel, torque);
        addEq3130(hit->vel, impulse);
    } else {
        loadVf8_3130(*(sVec3130*)&t4vect_YAxis);
        sVec3130 n = norm3130(mulVec3130(boneMat));
        sVec3130 torque = scale3130(cross3130(n, hit->dir), self->strength);
        addEq3130(hit->angVel, torque);
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_update);
#ifdef SKIP_ASM
struct sDebounceScroll {
    char pad_0x0[0x38];
    float du; // 0x38
    float dv; // 0x3c
    float u;  // 0x40
    float v;  // 0x44
};

/**
 * Advances the conveyor's texture scroll by one frame.
 *
 * Adds UVScroll (+0x38/+0x3C) to UV (+0x40/+0x44) and wraps each component back into -1..1 by one step.
 */
extern "C" void cConveyorNode_update(sDebounceScroll* self)
{
    self->u += self->du;
    self->v += self->dv;
    if (self->u > 1.0f) {
        self->u -= 1.0f;
    } else if (self->u < -1.0f) {
        self->u += 1.0f;
    }
    if (self->v > 1.0f) {
        self->v -= 1.0f;
    } else if (self->v < -1.0f) {
        self->v += 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sDfVec_343588 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDfMat_343588 {
    sDfVec_343588 r[4];
};

struct sDfSlot_343588 {
    int f0;
    int type;
};

struct sDfCam_343588 {
    char pad_0x0[0x20];
    sDfSlot_343588 slots[(0x3F0 - 0x20) / 8];  // 0x20
    struct {
        int kind;
        char pad[0x14];
    } paths[1];                     // 0x3F0
};

struct sDfObj_343588 {
    char pad_0x0[0x60];
    float pos[3];                   // 0x60
    float scl[3];                   // 0x6C
    char pad_0x78[0x7D - 0x78];
    unsigned char slot;             // 0x7D
    short path;                     // 0x7E
    char pad_0x80[0x94 - 0x80];
    void* light;                    // 0x94
};

struct sDfGfxVt_343588 {
    short delta;
    short index;
    void* fn;
};

struct cDfGfx_343588 {
    char pad_0x0[0x10D8];
    sDfGfxVt_343588* vt;            // 0x10D8
};

typedef sDfMat_343588 (*DfViewFn_343588)(void*);
typedef int (*DfCullFn_343588)(void*, sDfVec_343588*, sDfVec_343588*, const sDfMat_343588&);
typedef void (*DfDrawFn_343588)(void*, void*, void*, void*, int);

extern sDfMat_343588 D_004FF1A0_343588 __asm__("t4mat_Identity");

#define OBJ_343588(s) (*(sDfObj_343588**)((char*)(s) + 0x18))

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void MatCopy_343588(sDfMat_343588* d, const sDfMat_343588* s)
{
    __asm__(
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

static inline bool IsT6_343588(sDfCam_343588* c, int i)
{
    return c->slots[i].type == 6;
}

static inline int Visible_343588(sDfCam_343588* cam, sDfObj_343588* obj)
{
    if (obj->slot == 0xFF) {
        return 1;
    }
    if (IsT6_343588(cam, obj->slot)) {
        return cam->paths[obj->path].kind == 3;
    }
    return 0;
}

/**
 * Draws the conveyor's instance with its texture scrolled by the current UV.
 *
 * Returns when the instance's world-cache page is not loaded, or when graphics-manager slot 93 culls the instance's
 * bounding box (+0x60..+0x74) against the view matrix from slot 43. Otherwise copies the identity matrix t4mat_Identity,
 * puts UV (+0x40/+0x44) into its translation x/y and draws the instance through graphics-manager slot 96 with that
 * texture matrix and mode 2.
 */
extern "C" void cConveyorNode_render(char* self)
{
    sDfCam_343588* cam = (sDfCam_343588*)cWorld_m_pTheWorld->m_pWorldCache;
    if (!Visible_343588(cam, OBJ_343588(self))) {
        return;
    }
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    {
        sDfGfxVt_343588* vt = (sDfGfxVt_343588*)gfx->mVtbl;
        char* thisp = (char*)gfx + vt[93].delta;
        sDfVec_343588 cullBox[2];
        cullBox[0].x = OBJ_343588(self)->pos[0];
        cullBox[0].y = OBJ_343588(self)->pos[1];
        cullBox[0].z = OBJ_343588(self)->pos[2];
        cullBox[0].w = 1.0f;
        cullBox[1].x = OBJ_343588(self)->scl[0];
        cullBox[1].y = OBJ_343588(self)->scl[1];
        cullBox[1].z = OBJ_343588(self)->scl[2];
        cullBox[1].w = 1.0f;
        int res = ((DfCullFn_343588)vt[93].fn)(thisp, &cullBox[0], &cullBox[1], ((DfViewFn_343588)((sDfGfxVt_343588*)gfx->mVtbl)[43].fn)((char*)gfx + ((sDfGfxVt_343588*)gfx->mVtbl)[43].delta));
        if (res == 1) {
            return;
        }
    }
    sDfMat_343588 xf;
    MatCopy_343588(&xf, &D_004FF1A0_343588);
    xf.r[3].x = *(float*)(self + 0x40);
    xf.r[3].y = *(float*)(self + 0x44);
    ((DfDrawFn_343588)((sDfGfxVt_343588*)gfx->mVtbl)[96].fn)((char*)gfx + ((sDfGfxVt_343588*)gfx->mVtbl)[96].delta, OBJ_343588(self), 0, &xf, 2);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cConveyorNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sDebounceVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes the conveyor node to a replay frame.
 *
 * Writes the instance-node part (cInstanceNode_writeToReplayFrame), then the 44-byte node state at +0x1C (Vel, Rotate
 * flags, UVScroll, UV) through the stream's write slot (vtable entry 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cConveyorNode_writeToReplayFrame(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sDebounceVEntry* vt = *(sDebounceVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 44);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cCounterNode_cCounterNode1);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);
extern char cCounterNode_vtbl[];

/**
 * Constructs a counter node (node type 7) for a placed instance.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs cCounterNode_vtbl, sets the count (+0x1C) to the short StartCount
 * at counterData +0x4 and the counted value (+0x20) to 0.
 *
 * @param bucket  update bucket the node joins
 * @param inst  instance record the node belongs to
 * @param counterData  counter make-data (StartCount at +0x4)
 * @return self
 */
extern "C" void* cCounterNode_cCounterNode1(void* self, void* bucket, void* inst, void* counterData)
{
    cInstanceNode_cInstanceNode1(self, bucket, 7, inst);
    *(void**)((char*)self + 0xC) = cCounterNode_vtbl;
    *(int*)((char*)self + 0x1C) = *(short*)((char*)counterData + 0x4);
    *(int*)((char*)self + 0x20) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cCounterNode_cCounterNode);
#ifdef SKIP_ASM
struct sDebounceVEntry37C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cInstanceNode_cInstanceNode(void* self, void*, void*);
extern char cCounterNode_vtbl[];

/**
 * Constructs a counter node from a replay frame.
 *
 * Runs the cInstanceNode replay constructor, then reads 8 bytes at +0x1C (the count and the received-level
 * bits) through the frame's read method (slot 2).
 *
 * @param bucket  update bucket
 * @param stream  the replay frame to read (On Tour cBxReplayFrame*)
 * @return self
 */
extern "C" void* cCounterNode_cCounterNode(void* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    *(void**)((char*)self + 0xC) = cCounterNode_vtbl;
    sDebounceVEntry37C0* vt = *(sDebounceVEntry37C0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x1C, 0x8);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cCounterNode_update);
#ifdef SKIP_ASM
struct sDebounceVEntry1 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Expires the counter node once its count has reached zero.
 *
 * When the count (offset 0x1C, decremented by cCounterNode_signal) is 0, sets it to -1 so this happens once, and
 * calls the node's expire virtual (slot 34, cInstanceNode_expire) with 1.
 */
extern "C" void cCounterNode_update(void* self)
{
    if (*(int*)((char*)self + 0x1C) == 0) {
        *(int*)((char*)self + 0x1C) = -1;
        sDebounceVEntry1* vt = *(sDebounceVEntry1**)((char*)self + 0xC);
        vt[34].fn((char*)self + vt[34].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cCounterNode_signal);
#ifdef SKIP_ASM
/**
 * Counts a signal towards a counter node's target; each counted signal decrements the count (+0x1C) while
 * positive.
 *
 * Levels 1-10 map to bits 0-9 of the received mask (+0x20). Signal 500 counts each level once, in any order;
 * signal 501 counts each level once and only in sequence (level 1 first, then a level whose predecessor was
 * received); signal 502 always counts. Other signals do nothing.
 *
 * @param msg    signal number (On Tour signal)
 * @param level  the signal's argument: the level 1-10 (On Tour arg)
 */
extern "C" void cCounterNode_signal(void* self, int msg, float level)
{
    int bit = 0;
    int fire = 0;
    if (level == 1.0f)
        bit = 1;
    else if (level == 2.0f)
        bit = 2;
    else if (level == 3.0f)
        bit = 4;
    else if (level == 4.0f)
        bit = 8;
    else if (level == 5.0f)
        bit = 0x10;
    else if (level == 6.0f)
        bit = 0x20;
    else if (level == 7.0f)
        bit = 0x40;
    else if (level == 8.0f)
        bit = 0x80;
    else if (level == 9.0f)
        bit = 0x100;
    else if (level == 10.0f)
        bit = 0x200;
    if (msg == 500)
    {
        int seenLevels = *(int*)((char*)self + 0x20);
        if ((seenLevels & bit) == 0)
        {
            fire = 1;
            *(int*)((char*)self + 0x20) = seenLevels | bit;
        }
    }
    else if (msg == 501)
    {
        int seenLevels = *(int*)((char*)self + 0x20);
        if ((seenLevels & bit) == 0)
        {
            if (level == 1.0f)
            {
                fire = 1;
                *(int*)((char*)self + 0x20) = seenLevels | 1;
            }
            else if (seenLevels & (bit >> 1))
            {
                fire = 1;
                *(int*)((char*)self + 0x20) = seenLevels | bit;
            }
        }
    }
    else
    {
        fire = msg == 502;
    }
    if (fire)
    {
        int remaining = *(int*)((char*)self + 0x1C);
        if (remaining > 0)
            *(int*)((char*)self + 0x1C) = remaining - 1;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cCounterNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sDebounceVEntryA18 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a counter node's state to a replay frame.
 *
 * Writes the instance-node part with cInstanceNode_writeToReplayFrame(), then the 8 bytes at offset 0x1C (the
 * count and the signal-level bits) through the stream's write virtual (slot 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cCounterNode_writeToReplayFrame(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sDebounceVEntryA18* vt = *(sDebounceVEntryA18**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0x8);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343A68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sDebounceState;
extern "C" void func_003442E0(sDebounceState* self);
extern void* D_004A3FF0_p __asm__("D_004A3FF0");

struct sSlot00343A68 {
    unsigned int id;
    int value;
};
struct sState00343A68 {
    int f0;
    unsigned int f4;
    unsigned int f8;
    sSlot00343A68 slots[32];
    char rest[0x1F4 - 0x10C];
};

/**
 * Constructs the 3D crowd manager: five crowd blocks of 0x1F4 bytes, all empty.
 *
 * For each block it sets the crowd model id (+0x4) and all 32 member ids (+0xC, 8-byte entries) to -1 and resets the
 * block with func_003442E0, stores the manager in D_004A3FF0 (the global the world scripts and replay code use), then
 * resets all five blocks again. cGame's loader allocates it (2500 bytes, "CrowdMan3d").
 *
 * @return self
 */
extern "C" sState00343A68* func_00343A68(sState00343A68* self)
{
    sState00343A68* crowdBlock = self;
    int i;
    for (i = 4; i != -1; i--) {
        sState00343A68* next = crowdBlock + 1;
        crowdBlock->f4 = 0xFFFFFFFF;
        sSlot00343A68* memberSlot = crowdBlock->slots;
        int j;
        for (j = 31; j != -1; j--) {
            memberSlot->id = 0xFFFFFFFF;
            memberSlot++;
        }
        func_003442E0((sDebounceState*)crowdBlock);
        crowdBlock = next;
    }
    D_004A3FF0_p = self;
    for (i = 0; i < 5; i++) {
        func_003442E0((sDebounceState*)&self[i]);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343B28);
#ifdef SKIP_ASM
extern int D_004A3FF0;
void operator_delete(int*);
void* func_00344348(void* self);

struct sDebounceElem_343B28 {
    char data[0x1F4];
};

struct sDebounce_343B28 {
    sDebounceElem_343B28 elems[5];
};

/**
 * Destroys the 3D crowd manager.
 *
 * Frees each block's animation data (func_00344348), clears the global D_004A3FF0, then runs the block destructor
 * (func_00344348 again) over the five blocks in reverse and deletes the manager when bit 0 of flags is set. The second
 * pass is harmless because func_00344800 nulls the pointers it frees.
 *
 * @param flags  destructor flags; bit 0 deletes the storage
 */
extern "C" void func_00343B28(void* self, int flags)
{
    sDebounce_343B28* crowdMan = (sDebounce_343B28*)self;
    int i;
    for (i = 0; i < 5; i++) {
        func_00344348(&crowdMan->elems[i]);
    }
    D_004A3FF0 = 0;
    if (crowdMan->elems != 0) {
        sDebounceElem_343B28* blockIter = crowdMan->elems + 5;
        while (crowdMan->elems != blockIter) {
            blockIter--;
            func_00344348(blockIter);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343BC0);
#ifdef SKIP_ASM
struct sDebounceState;
extern "C" void func_003442E0(sDebounceState* self);
void* func_00344348(void* self);

struct sDebounceBlock {
    char data[0x1F4];
};

/**
 * Empties every crowd block of the 3D crowd manager.
 *
 * For each of the five blocks: frees its animation data (func_00344348) and resets its state (func_003442E0). Used
 * before a replay frame is read (func_00344240).
 */
extern "C" void func_00343BC0(sDebounceBlock* self)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_00344348(&self[i]);
        func_003442E0((sDebounceState*)&self[i]);
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00343C08);
#ifdef SKIP_ASM
struct sDebounceElem3C08 {
    char data[0x1F4];
};

extern "C" void func_00344898(void* elem, void* arg);

/**
 * Removes every crowd member that belongs to a world section from all five crowd blocks.
 *
 * Calls func_00344898 on each block; that drops the members whose instance id has the section in its low byte and frees
 * a block that becomes empty. cGame_sectionUnloading calls it with the unloading section.
 *
 * @param sectionId  world section id being unloaded
 */
extern "C" void func_00343C08(sDebounceElem3C08* self, void* sectionId)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_00344898(&self[i], sectionId);
    }
}
#endif

INCLUDE_ASM("object/debouncenode", func_00343C60);

//100%
INCLUDE_ASM("object/debouncenode", func_00343F38);
#ifdef SKIP_ASM
// FAKEMATCH: a dead two-variable cycle `dA = dB + id; dB = dA;` (dB = 0 before the loop) on release()'s not-found path, so
// `id` is still referenced after the found arm when cse2 picks the canonical register for `ents[j].ref.id == id` (flow
// deletes the cycle); plain form 92.09% (the earlier `id == ents[j].ref.id` spelling scores 96.46%). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** cObjectInterface_getWorld();
struct sDebounceState;
extern "C" void func_003442E0(sDebounceState* self);
extern "C" void* func_00344800(void* self);

struct sModel3F38 {
    char pad_0x0[0x8];
    unsigned int flags;     // 0x8
    char pad_0xc[0x74];
    unsigned int* key;      // 0x80
};

static inline sModel3F38* refToPtr3F38(unsigned int p)
{
    return (sModel3F38*)(p << 2);
}

struct sModelSet3F38 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C

    sModel3F38* lookup(unsigned int idx)
    {
        unsigned int p = refs[idx] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr3F38(p);
    }
};

struct sWorld3F38 {
    char pad_0x0[0x8];
    sModelSet3F38** sets;   // 0x8
};

static inline sModel3F38* getModel3F38(unsigned int id)
{
    sModelSet3F38* set = (*(sWorld3F38**)cObjectInterface_getWorld())->sets[id & 0xFF];
    if (set == 0) {
        return 0;
    }
    return set->lookup(id >> 8);
}

struct sModelRef3F38 {
    unsigned int id;

    sModel3F38* get()
    {
        return getModel3F38(id);
    }
};

struct sEnt3F38 {
    sModelRef3F38 ref;
    int f4;
};

struct sBlock3F38 {
    int count;              // 0x0
    int f4;                 // 0x4
    unsigned int key;       // 0x8
    sEnt3F38 ents[32];      // 0xC
    char pad[0x1F4 - 0x10C];

    void release(unsigned int id)
    {
        unsigned int dA;
        unsigned int dB = 0;
        for (int j = 0; j < 32; j++) {
            if (ents[j].ref.id == id) {
                sEnt3F38* e = &ents[j];
                sModel3F38* m2 = e->ref.get();
                m2->flags = (m2->flags & 0xFFFFFFFB) | 2;
                ents[j].ref.id = 0xFFFFFFFF;
                if (--count <= 0) {
                    func_00344800(this);
                    func_003442E0((sDebounceState*)this);
                }
                return;
            }
            dA = dB + id;
            dB = dA;
        }
    }
};

struct sDebounceMgr3F38 {
    sBlock3F38 blocks[5];
};

/**
 * Removes one instance from the 3D crowd it belongs to.
 *
 * Looks up the instance record for id, takes the crowd key from its model (+0x80, value >> 8) and finds the block whose
 * key (+0x8) matches. In that block it clears the member entry with this id, sets the record's flags to (flags & ~0x4)
 * | 0x2, decrements the member count (+0x0) and, when the block is empty, frees its animation data (func_00344800) and
 * resets it (func_003442E0). The world script handler calls it for crowd command 1.
 *
 * @param id  instance id (tResourceID) of the crowd member
 */
extern "C" void func_00343F38(sDebounceMgr3F38* self, unsigned int id)
{
    sModel3F38* model = getModel3F38(id);
    unsigned int* key = model->key;
    for (int i = 0; i < 5; i++) {
        if (*key >> 8 == self->blocks[i].key) {
            self->blocks[i].release(id);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003440C8);
#ifdef SKIP_ASM
struct sDebounceElem40C8 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

extern "C" void func_003449F0(void* elem);

/**
 * Updates the 3D crowds: advances the animation of every block that is in use.
 *
 * A block is in use while its key (+0x8) is not -1; each such block goes through func_003449F0. Called once per game
 * update for cGame's CrowdMan3d.
 */
extern "C" void func_003440C8(sDebounceElem40C8* self)
{
    int i;
    sDebounceElem40C8* crowdBlock = self;
    for (i = 0; i < 5; i++) {
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_003449F0(crowdBlock);
        }
        crowdBlock++;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344138);
#ifdef SKIP_ASM
struct sDebounceElem4138 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

extern "C" void func_00344E18(void* elem);

/**
 * Renders the 3D crowds: draws every block that is in use.
 *
 * A block is in use while its key (+0x8) is not -1; each such block is drawn by func_00344E18. Called from the game
 * render pass.
 */
extern "C" void func_00344138(sDebounceElem4138* self)
{
    int i;
    sDebounceElem4138* crowdBlock = self;
    for (i = 0; i < 5; i++) {
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344E18(crowdBlock);
        }
        crowdBlock++;
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003441A8);
#ifdef SKIP_ASM
class cDebounceStream;
extern "C" void func_00344FC0(void* self, cDebounceStream* s);

struct sDebounceElem41A8 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

struct sDebounceVEntry41A8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the 3D crowd manager to a replay frame.
 *
 * For each of the five blocks it writes the 4-byte key (+0x8) through the stream's write slot (vtable entry 1) and, for
 * a block in use, the block's member data (func_00344FC0).
 *
 * @param replayStream  replay stream to write to
 */
extern "C" void func_003441A8(sDebounceElem41A8* self, cDebounceStream* replayStream)
{
    int i;
    for (i = 0; i < 5; i++) {
        sDebounceVEntry41A8* writeSlot = &(*(sDebounceVEntry41A8**)replayStream)[1];
        writeSlot->fn((char*)replayStream + writeSlot->delta, &self[i].field_0x8, 4);
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344FC0(&self[i], replayStream);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/debouncenode", func_00344240);
#ifdef SKIP_ASM
class cDebounceStream;
extern "C" void func_00344FF8(void* self, cDebounceStream* s);

struct sDebounceElem4240 {
    int field_0x0;
    unsigned int field_0x4;
    unsigned int field_0x8;
    char pad_0xc[0x1E8];
};

struct sDebounceVEntry4240 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the 3D crowd manager back from a replay frame.
 *
 * Empties all blocks first (func_00343BC0), then for each of the five reads the 4-byte key (+0x8) through the stream's
 * read slot (vtable entry 2) and, for a block in use, its member data and animation (func_00344FF8).
 *
 * @param replayStream  replay stream to read from
 */
extern "C" void func_00344240(sDebounceElem4240* self, cDebounceStream* replayStream)
{
    int i;
    func_00343BC0((sDebounceBlock*)self);
    for (i = 0; i < 5; i++) {
        sDebounceVEntry4240* readSlot = &(*(sDebounceVEntry4240**)replayStream)[2];
        readSlot->fn((char*)replayStream + readSlot->delta, &self[i].field_0x8, 4);
        if (self[i].field_0x8 != 0xFFFFFFFF) {
            func_00344FF8(&self[i], replayStream);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003442E0);
#ifdef SKIP_ASM
struct sDebounceSlot {
    unsigned int id; // 0x0
    int value;       // 0x4
};

struct sDebounceState {
    int field_0x0;            // 0x000
    unsigned int field_0x4;   // 0x004
    unsigned int field_0x8;   // 0x008
    sDebounceSlot slots[32];  // 0x00c
    int field_0x10c;          // 0x10c
    int field_0x110;          // 0x110
    int field_0x114;          // 0x114
    char pad_0x118[0xc4];     // 0x118
    int field_0x1dc;          // 0x1dc
    int field_0x1e0;          // 0x1e0
    int field_0x1e4;          // 0x1e4
    int field_0x1e8;          // 0x1e8
    int field_0x1ec;          // 0x1ec
    int field_0x1f0;          // 0x1f0
};

/**
 * Resets one crowd block to empty.
 *
 * Zeroes the member count (+0x0), the animation time (+0x1DC), the animation matrices, mesh array and anim def pointers
 * (+0x1E8, +0x1EC, +0x1F0) and the item counts (+0x110, +0x114); sets the model id (+0x4) and key (+0x8) to -1 and
 * every member entry (+0xC, 32 of them) to id -1, value 0. It does not free anything.
 */
extern "C" void func_003442E0(sDebounceState* self)
{
    int i;
    self->field_0x0 = 0;
    self->field_0x1dc = 0;
    self->field_0x8 = 0xFFFFFFFF;
    self->field_0x4 = 0xFFFFFFFF;
    self->field_0x1e8 = 0;
    self->field_0x1ec = 0;
    self->field_0x1f0 = 0;
    self->field_0x114 = 0;
    self->field_0x110 = 0;
    for (i = 0; i < 32; i++) {
        self->slots[i].id = 0xFFFFFFFF;
        self->slots[i].value = 0;
    }
}
#endif

extern "C" void* func_00344800(void* self);

//100%
INCLUDE_ASM("object/debouncenode", func_00344348__FPv);
#ifdef SKIP_ASM
/**
 * Destroys one crowd block by freeing its animation data (func_00344800).
 *
 * @return nothing meaningful: it forwards func_00344800, which returns no value
 */
void* func_00344348(void* self)
{
    return func_00344800(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344368);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;
extern "C" void func_003443A8(void* self);

/**
 * Sets the crowd model of a block and builds its animation data.
 *
 * Stores modelId at +0x4, zeroes the animation time (+0x1DC), sets the time step (+0x1E4) to 1 / the frame rate at
 * cAppMan_TheAppMan +0x10 and calls func_003443A8.
 *
 * @param modelId  instance id (tResourceID) of the crowd model
 */
extern "C" void func_00344368(void* self, int modelId)
{
    char* owner = (char*)self;
    char* appMan = cAppMan_TheAppMan;
    *(int*)(owner + 0x4) = modelId;
    *(int*)(owner + 0x1DC) = 0;
    *(float*)(owner + 0x1E4) = 1.0f / (float)*(int*)(appMan + 0x10);
    func_003443A8(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_003443A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** cObjectInterface_getWorld();
extern "C" void cAnimFrame_init(void* self);
extern "C" void cMeshAnimFrame_init(void* self);
extern "C" void cAnimFrame_setAnimRecord(void* self, void* src);
extern "C" void MEM_fill(void* p, int v, int n);
extern char str_AnimMats[];
extern char str_CrowdAnimMesh[];
extern void* cMeshAnimFrame_vtbl[];
extern void* cAnimFrame_vtbl[];

struct sDbVEntry_43A8 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sDbMesh_43A8 {
    char pad_0x0[0x84];
    void** vtable;                  // 0x84
    char pad_0x88[0x48];
    sDbMesh_43A8()
    {
        vtable = cAnimFrame_vtbl;
        cAnimFrame_init(this);
        vtable = cMeshAnimFrame_vtbl;
        cMeshAnimFrame_init(this);
    }
    void operator delete[](void* p, unsigned int size);
};

struct sDbPad_43A8 {
    char data[0x40];
    sDbPad_43A8() {}
};

struct sDbEntry_43A8 {
    int pad_0x0[2];
    void* src;                      // 0x8
    int pad_0xC;
};

struct sDbDef_43A8 {
    int pad_0x0;
    int count;                      // 0x4
    sDbEntry_43A8* entries;         // 0x8
    int pad_0xC[2];
    float period;                   // 0x14
};

struct sDbItem_43A8 {
    char pad_0x0[0xC];
    int fC;                         // 0xC
    char* data;                     // 0x10
};

struct sDbList_43A8 {
    int count;                      // 0x0
    sDbItem_43A8* items[1];         // 0x4
};

struct sDbModel_43A8 {
    char pad_0x0[0x80];
    sDbDef_43A8* def;               // 0x80
    char pad_0x84[0x10];
    sDbList_43A8* list;             // 0x94
};

struct sDbSlot_43A8 {
    unsigned short value;           // 0x0
    short ids[3];                   // 0x2
    int f8;
    int fC;
    int f10;
};

static inline sDbModel_43A8* refToPtr_43A8(unsigned int p)
{
    return (sDbModel_43A8*)(p << 2);
}

struct sDbModelSet_43A8 {
    char pad_0x0[0x1C];
    unsigned int* refs;             // 0x1C

    sDbModel_43A8* lookup(unsigned int idx)
    {
        unsigned int p = refs[idx] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr_43A8(p);
    }
};

struct sDbWorld_43A8 {
    char pad_0x0[0x8];
    sDbModelSet_43A8** sets;        // 0x8
};

struct sDbModelRef_43A8 {
    unsigned int id;

    sDbModel_43A8* get()
    {
        sDbModelSet_43A8* set = (*(sDbWorld_43A8**)cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return set->lookup(id >> 8);
    }
};

struct sDbOwner_43A8 {
    int field_0x0;
    sDbModelRef_43A8 model;         // 0x4
    char pad_0x8[0x104];
    int nActive;                    // 0x10C
    int count;                      // 0x110
    int first;                      // 0x114
    int total;                      // 0x118
    sDbSlot_43A8* ptrs[8];          // 0x11C
    sDbSlot_43A8 slots[8];          // 0x13C
    float time;                     // 0x1DC
    float period;                   // 0x1E0
    float step;                     // 0x1E4
    sDbPad_43A8* buf;               // 0x1E8
    sDbMesh_43A8* meshes;           // 0x1EC
    sDbDef_43A8* def;               // 0x1F0
};

extern "C" void func_00344AA0_43A8(void* self) __asm__("func_00344AA0");



extern "C" void func_003443A8_impl(sDbOwner_43A8* self) __asm__("func_003443A8");
/**
 * Builds a crowd block's animation data from its crowd model.
 *
 * Looks up the model record (id at +0x4) and takes its anim def (+0x80) and the period (def +0x14). Allocates one
 * 64-byte matrix per def entry ("AnimMats", +0x1E8), counts the entries with an animation source (+0x10C) and allocates
 * that many cMeshAnimFrame objects ("CrowdAnimMesh", 0xD0 bytes each, +0x1EC), initialising each from its source
 * (cAnimFrame_setAnimRecord) and updating it to the current time (vtable entry 2). Builds the part matrices (func_00344AA0), then
 * reads the model's item list (+0x94): +0x114 takes the first word of the first item with data, +0x110 the item count,
 * and for each item the slot pointer table (+0x11C) points at its own 20-byte slot (+0x13C, with the item's +0xC and
 * the u16 at data +0x4) or at the item itself when it has no data.
 *
 * Likely bug: the loop that sets the slot's three ids to -1 always writes slot 0, not the current slot. The tables hold
 * 8 entries but the item count is not checked against that.
 */
extern "C" void func_003443A8_impl(sDbOwner_43A8* self)
{
    sDbModel_43A8* animModel = self->model.get();
    int i;
    self->def = animModel->def;
    self->period = self->def->period;
    sDbPad_43A8*& buf = self->buf;
    buf = new (str_AnimMats, 0x20000000, 0) sDbPad_43A8[self->def->count];
    sDbEntry_43A8* entry = self->def->entries;
    self->nActive = 0;
    for (i = 0; i < self->def->count; i++, entry++) {
        if (entry->src != 0)
            self->nActive++;
    }
    if (self->nActive != 0) {
        sDbMesh_43A8*& meshes = self->meshes;
        meshes = new (str_CrowdAnimMesh, 0x20000000, 0) sDbMesh_43A8[self->nActive];
    } else {
        self->meshes = 0;
    }
    entry = self->def->entries;
    int k = 0;
    for (i = 0; i < self->def->count; i++, entry++) {
        if (entry->src != 0) {
            cAnimFrame_setAnimRecord(&self->meshes[k++], entry->src);
        }
    }
    for (i = 0; i < self->nActive; i++) {
        sDbMesh_43A8* slotIds = (sDbMesh_43A8*)(i * 0xD0 + (int)self->meshes);
        ((sDbVEntry_43A8*)slotIds->vtable)[2].fn((char*)slotIds + ((sDbVEntry_43A8*)slotIds->vtable)[2].delta, self->time);
    }
    func_00344AA0_43A8(self);
    self->first = 0;
    self->count = animModel->list->count;
    for (i = 0; i < self->count; i++) {
        char* itemData = animModel->list->items[i]->data;
        if (itemData != 0) {
            self->first = *(int*)itemData;
            break;
        }
    }
    MEM_fill(&self->total, 0, 196);
    self->total = self->count;
    for (i = 0; i < self->count; i++) {
        sDbItem_43A8* item = animModel->list->items[i];
        if (item->data != 0) {
            self->ptrs[i] = &self->slots[i];
            self->ptrs[i]->fC = item->fC;
            short* slotIds = (short*)self->slots;
            for (int j = 1; j < 4; j++) {
                slotIds[j] = -1;
            }
            self->slots[i].value = *(unsigned short*)(item->data + 4);
        } else {
            self->ptrs[i] = (sDbSlot_43A8*)item;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344730);
#ifdef SKIP_ASM
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** cObjectInterface_getWorld();

struct sItem4730 {
    char pad_0x0[0x10];
    char* data;             // 0x10
};

struct sItemList4730 {
    int count;              // 0x0
    sItem4730* items[1];    // 0x4
};

struct sModel4730 {
    char pad_0x0[0x94];
    sItemList4730* list;    // 0x94
};

static inline sModel4730* refToPtr4730(unsigned int p)
{
    return (sModel4730*)(p << 2);
}

struct sModelSet4730 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C

    sModel4730* lookup(unsigned int idx)
    {
        unsigned int p = refs[idx] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr4730(p);
    }
};

struct sWorld4730 {
    char pad_0x0[0x8];
    sModelSet4730** sets;   // 0x8
};


struct sModelRef4730 {
    unsigned int id;

    sModel4730* get()
    {
        sModelSet4730* set = (*(sWorld4730**)cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return set->lookup(id >> 8);
    }
};

struct sSlot4730 {
    unsigned short value;   // 0x0
    char pad_0x2[0x12];
};

struct sDebounce4730 {
    int field_0x0;
    sModelRef4730 model;    // 0x4
    char pad_0x8[0x108];
    int count;              // 0x110
    char pad_0x114[0x28];
    sSlot4730 slots[1];     // 0x13C
};

/**
 * Selects the per-item value for one crowd member variant.
 *
 * For every item of the block's crowd model (+0x94 list) that has data, copies the u16 at data + 4 + index * 4 into
 * that item's slot value (+0x13C + 20 * item).
 *
 * @param index  variant index (the member entry's value)
 */
extern "C" void func_00344730(sDebounce4730* self, int index)
{
    sModel4730* animModel = self->model.get();
    for (int i = 0; i < self->count; i++) {
        char* itemData = animModel->list->items[i]->data;
        if (itemData != 0) {
            self->slots[i].value = *(unsigned short*)(itemData + (index << 2) + 4);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344800);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMemMan_free(void*);

struct sDebounceVEntry4800 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sDebounceItem4800 {
    char pad_0x0[0x84];
    sDebounceVEntry4800* vt;    // 0x84
    char pad_0x88[0x48];
};

struct sDebounceOwner4800 {
    char pad_0x0[0x1E8];
    void* buf;                  // 0x1E8
    sDebounceItem4800* items;   // 0x1EC, array-new block (count at -0x10)
};

// The unit declares func_00344800 as `void* (void*)`, but the body returns nothing
// (its wrapper func_00344348 only forwards). Bound by asm label.
void func_00344800_v(sDebounceOwner4800* self) __asm__("func_00344800");

/**
 * Frees a crowd block's animation data.
 *
 * Destroys the cMeshAnimFrame array (+0x1EC; each element's destructor, vtable entry 1, in reverse, then frees the
 * array block) and frees the matrix buffer (+0x1E8); both pointers end up null.
 */
void func_00344800_v(sDebounceOwner4800* self)
{
    sDebounceItem4800* items = self->items;
    if (items != 0) {
        sDebounceItem4800* item = items + ((int*)items)[-4];
        while (self->items != item) {
            item--;
            item->vt[1].fn((char*)item + item->vt[1].delta, 0);
        }
        cMemMan_free((char*)self->items - 0x10);
    }
    if (self->buf != 0) {
        cMemMan_free(self->buf);
    }
    self->items = 0;
    self->buf = 0;
}
#endif

INCLUDE_ASM("object/debouncenode", func_00344898);

//100%
INCLUDE_ASM("object/debouncenode", func_003449F0);
#ifdef SKIP_ASM
struct sDebounceVEntry49F0 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sDebounceItem49F0 {
    char pad_0x0[0x84];
    sDebounceVEntry49F0* vt;    // 0x84
    char pad_0x88[0x48];
};

struct sDebounceOwner49F0 {
    char pad_0x0[0x10C];
    int count;                  // 0x10C
    char pad_0x110[0xCC];
    float time;                 // 0x1DC
    float period;               // 0x1E0
    float step;                 // 0x1E4
    void* buf;                  // 0x1E8
    sDebounceItem49F0* items;   // 0x1EC
};

extern "C" void func_00344AA0(sDebounceOwner49F0* self);

/**
 * Advances a crowd block's animation by one frame.
 *
 * Adds the time step (+0x1E4) to the time (+0x1DC), wrapping it by the period (+0x1E0), updates each animated mesh
 * (+0x1EC, count +0x10C) to the new time through its vtable entry 2 and rebuilds the part matrices (func_00344AA0).
 *
 * @param elem  the crowd block (0x1F4 bytes)
 */
// PORT: pointer held in int for the element address (index-first addu); not 64-bit safe.
extern "C" void func_003449F0(void* elem)
{
    sDebounceOwner49F0* self = (sDebounceOwner49F0*)elem;
    float newTime = self->time + self->step;
    self->time = newTime;
    if (newTime > self->period) {
        self->time = newTime - self->period;
    }
    for (int i = 0; i < self->count; i++) {
        sDebounceItem49F0* item = (sDebounceItem49F0*)(i * 0xD0 + (int)self->items);
        item->vt[2].fn((char*)item + item->vt[2].delta, self->time);
    }
    func_00344AA0(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344AA0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** cObjectInterface_getWorld();

struct sDbV4_4AA0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDbMat_4AA0 {
    sDbV4_4AA0 r[4];
};

struct sDbPart_4AA0 {
    int parent;             // 0x0
    int pad4;
    int animated;           // 0x8
    sDbMat_4AA0* mat;       // 0xC
};

struct sDbModelInfo_4AA0 {
    int pad0;
    int count;              // 0x4
    sDbPart_4AA0* parts;    // 0x8
};

struct sDbInst_4AA0 {
    char pad_0x00[0x84];
    float scale;            // 0x84
};

static inline sDbInst_4AA0* refToPtr_4AA0(unsigned int p)
{
    return (sDbInst_4AA0*)(p << 2);
}

struct sDbModelSet_4AA0 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C

    sDbInst_4AA0* lookup(unsigned int idx)
    {
        unsigned int p = refs[idx] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr_4AA0(p);
    }
};

struct sDbWorld_4AA0 {
    char pad_0x0[0x8];
    sDbModelSet_4AA0** sets;    // 0x8
};

struct sDbModelRef_4AA0 {
    unsigned int id;

    sDbInst_4AA0* get()
    {
        sDbModelSet_4AA0* set = (*(sDbWorld_4AA0**)cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return set->lookup(id >> 8);
    }
};

struct sDbItem_4AA0 {
    char pad_0x00[0x90];
    sDbMat_4AA0 mat;        // 0x90
};

struct sDbAnim_4AA0 {
    int pad0;
    sDbModelRef_4AA0 model;     // 0x4
    char pad_0x8[0x1E0];
    sDbMat_4AA0* mats;          // 0x1E8
    sDbItem_4AA0* items;        // 0x1EC
    sDbModelInfo_4AA0* info;    // 0x1F0
};

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_4AA0(sDbMat_4AA0* d, const sDbMat_4AA0* a, const sDbMat_4AA0* b)
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
static inline void vu0CopyMat_4AA0(sDbMat_4AA0* d, const sDbMat_4AA0* s)
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

static inline sDbV4_4AA0 row_4AA0(const sDbMat_4AA0* m, int i)
{
    return m->r[i];
}

static inline void mulv_4AA0(sDbV4_4AA0& r, const sDbV4_4AA0& a, const sDbV4_4AA0& b)
{
    r.x = a.x * b.x;
    r.y = a.y * b.y;
    r.z = a.z * b.z;
    r.w = a.w * b.w;
}

// Builds a part matrix: dst = parent * (local with its translation row scaled by s).
static inline void partMat_4AA0(sDbMat_4AA0* dst, const sDbMat_4AA0* parentMat, const sDbMat_4AA0* m, const sDbV4_4AA0& s)
{
    sDbMat_4AA0 l;
    l.r[0] = row_4AA0(m, 0);
    l.r[1] = row_4AA0(m, 1);
    l.r[2] = row_4AA0(m, 2);
    sDbV4_4AA0 t;
    sDbV4_4AA0 r3 = m->r[3];
    mulv_4AA0(t, r3, s);
    l.r[3] = t;
    sDbMat_4AA0 tt;
    vu0MulMat_4AA0(&tt, parentMat, &l);
    vu0CopyMat_4AA0(dst, &tt);
}

// Scales the translation row of m by s.
static inline void scaleTrans_4AA0(sDbMat_4AA0* m, const sDbV4_4AA0& s)
{
    sDbV4_4AA0 t;
    sDbV4_4AA0 r3 = m->r[3];
    mulv_4AA0(t, r3, s);
    m->r[3] = t;
}

// Rebuilds the world matrix of every part; animated parts take their local matrix from the next animation item.
extern "C" void func_00344AA0_4AA0(sDbAnim_4AA0* self) __asm__("func_00344AA0");
/**
 * Rebuilds the world matrix of every part of a crowd block's model.
 *
 * Walks the anim def's parts (+0x1F0: count +0x4, 16-byte parts at +0x8 with parent, animated flag and matrix). A
 * part's local matrix is the next animated mesh's frame matrix (+0x90) when the part is animated, otherwise its own
 * matrix; its translation row is scaled by the model's scale (+0x84). Child parts are multiplied by their parent's
 * result; the results go to the matrix buffer (+0x1E8).
 */
extern "C" void func_00344AA0_4AA0(sDbAnim_4AA0* self)
{
    sDbPart_4AA0* part = self->info->parts;
    float k = self->model.get()->scale;
    sDbV4_4AA0 scaleVec;
    scaleVec.x = k;
    scaleVec.y = k;
    scaleVec.z = k;
    scaleVec.w = 1.0f;
    int n = 0;
    for (int i = 0; i < self->info->count; i++, part++) {
        int parent = part->parent;
        if (part->animated != 0) {
            if (parent != -1) {
                // PORT: pointer arithmetic done in int (gives the target's offset-first addu).
                sDbMat_4AA0* local = &((sDbItem_4AA0*)(n++ * (int)sizeof(sDbItem_4AA0) + (int)self->items))->mat;
                sDbMat_4AA0 combined;
                partMat_4AA0(&combined, &self->mats[parent], local, scaleVec);
                vu0CopyMat_4AA0(&self->mats[i], &combined);
            } else {
                vu0CopyMat_4AA0(&self->mats[i], &self->items[n++].mat);
                scaleTrans_4AA0(&self->mats[i], scaleVec);
            }
        } else {
            if (parent != -1) {
                sDbMat_4AA0 combined;
                partMat_4AA0(&combined, &self->mats[parent], part->mat, scaleVec);
                vu0CopyMat_4AA0(&self->mats[i], &combined);
            } else {
                vu0CopyMat_4AA0(&self->mats[i], part->mat);
                scaleTrans_4AA0(&self->mats[i], scaleVec);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344E18);
#ifdef SKIP_ASM
struct sTrackWorld51C0;
extern "C" sTrackWorld51C0** cObjectInterface_getWorld();
extern "C" void func_00344730(sDebounce4730* self, int index);

struct sModel_44E18 {
    char pad_0x0[0x7D];
    unsigned char page;     // 0x7D
    short slot;             // 0x7E
};

struct sModelSet_44E18 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sWorld_44E18 {
    char pad_0x0[0x8];
    sModelSet_44E18** sets; // 0x8
};

static inline sModel_44E18* GetModel_44E18(unsigned int* ref)
{
    return (sModel_44E18*)((sModelRef4730*)ref)->get();
}

static inline int IsLoaded_44E18(sModel_44E18* m)
{
    char* wc = (char*)cWorld_m_pTheWorld->m_pWorldCache;
    if (m->page == 0xFF)
        return 1;
    int ok = *(int*)(wc + (m->page << 3) + 0x24) == 6;
    return ok ? *(int*)(wc + m->slot * 0x18 + 0x3F0) == 3 : 0;
}

struct sVE7_44E18 { short delta; short index; void (*fn)(void*, void*, float, void*, void*, void*, int, int); };
struct sVE5_44E18 { short delta; short index; void (*fn)(void*, void*, void*, int, int); };

/**
 * Draws every member of a crowd block.
 *
 * Skips free entries (id -1) and members whose world-cache page is not loaded. When +0x114 is 2 or more it first
 * selects the member's variant (func_00344730 with the entry's value) and draws the member through graphics-manager
 * slot 95 with the slot table (+0x118) and matrix buffer (+0x1E8); otherwise it draws through slot 96 with the matrix
 * buffer only.
 */
extern "C" void func_00344E18(void* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    unsigned int* ref = (unsigned int*)((char*)self + 0xC);
    int off = 0;
    for (int i = 0; i < 32; ref += 2, i++, off += 8)
    {
        if (~*ref == 0)
            continue;
        sModel_44E18* model = GetModel_44E18(ref);
        if (!IsLoaded_44E18(model))
            continue;
        if (*(int*)((char*)self + 0x114) >= 2)
        {
            func_00344730((sDebounce4730*)self, *(int*)((char*)self + off + 0x10));
            sVE7_44E18* vt = (sVE7_44E18*)ps2GraphicsMan->mVtbl;
            vt[95].fn((char*)ps2GraphicsMan + vt[95].delta, model, 1.0f, (char*)self + 0x118, (char*)model + 0x10, *(void**)((char*)self + 0x1E8), 0, 0);
        }
        else
        {
            sVE5_44E18* vt = (sVE5_44E18*)ps2GraphicsMan->mVtbl;
            vt[96].fn((char*)ps2GraphicsMan + vt[96].delta, model, *(void**)((char*)self + 0x1E8), 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344FC0);
#ifdef SKIP_ASM
// Replay stream: vtable entry 1 writes, entry 2 reads (as the other replay writers and readers use it).
class cDebounceStream {
public:
    virtual void write(void* buf, int size);
    virtual void read(void* buf, int size);
};

/**
 * Writes a crowd block's member data (its first 268 bytes: count, model id, key, 32 entries) to a replay frame.
 *
 * @param replayStream  replay stream to write to (vtable entry 1)
 */
extern "C" void func_00344FC0(void* self, cDebounceStream* replayStream)
{
    replayStream->write(self, 268);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", func_00344FF8);
#ifdef SKIP_ASM
extern "C" void func_003443A8(void* self);

/**
 * Reads a crowd block's member data back from a replay frame and rebuilds its animation.
 *
 * Reads the first 268 bytes (count, model id, key, 32 entries) through the stream's vtable entry 2, then frees
 * (func_00344800) and rebuilds (func_003443A8) the animation data.
 *
 * @param replayStream  replay stream to read from
 */
extern "C" void func_00344FF8(void* self, cDebounceStream* replayStream)
{
    replayStream->read(self, 268);
    func_00344800(self);
    func_003443A8(self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cSplineSegment_getPos);
#ifdef SKIP_ASM
struct sDebVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDebMtx {
    sDebVec4 r[4];
};

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sDebVec4 debMtxApply(sDebMtx* m, const sDebVec4& v)
{
    sDebVec4 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline sDebVec4 debVecScale(const sDebVec4& v, float s)
{
    sDebVec4 out;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(out), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return out;
}

struct sDebCurve {
    char pad_0x00[0x10];
    sDebMtx basis;   // 0x10
    float coef[4];   // 0x50: time-warp cubic
    char pad_0x60[0x24];
    float t0;        // 0x84
};

/**
 * Evaluates one spline segment at a distance along the spline: position, direction and the direction's change.
 *
 * The distance is made relative to the segment's start (+0x84) and mapped to the curve parameter u by the cubic at
 * +0x50 (u = ((c0 d + c1) d + c2) d + c3; du is its derivative). With the 4x4 basis matrix at +0x10 (B):
 * @p pos = B (u^3, u^2, u, 1), @p vel = B (3u^2, 2u, 1, 0) and @p acc = B (6u, 2, 0, 0) * du. On Tour asserts the
 * distance lies within the segment and the position within +-5e6; retail does not.
 *
 * @param dist    the distance along the spline (On Tour dist)
 * @param pos  receives the position
 * @param vel  receives the direction (the derivative with respect to u, not normalised)
 * @param acc  receives the direction's derivative with respect to the distance (On Tour dDir)
 */
// Evaluate the curve at time t: position, velocity and acceleration.
extern "C" void cSplineSegment_getPos(sDebCurve* self, float dist, sDebVec4* pos, sDebVec4* vel, sDebVec4* acc)
{
    dist -= self->t0;
    float splineParam = ((self->coef[0] * dist + self->coef[1]) * dist + self->coef[2]) * dist + self->coef[3];
    float du = (self->coef[0] * (dist * 3.0f) + self->coef[1] * 2.0f) * dist + self->coef[2];
    sDebVec4 secondDeriv;
    secondDeriv.x = splineParam * 6.0f;
    secondDeriv.y = 2.0f;
    secondDeriv.z = 0.0f;
    secondDeriv.w = 0.0f;
    *acc = debVecScale(debMtxApply(&self->basis, secondDeriv), du);
    sDebVec4 powerBasis;
    powerBasis.x = splineParam * splineParam * splineParam;
    powerBasis.y = splineParam * splineParam;
    powerBasis.z = splineParam;
    powerBasis.w = 1.0f;
    *pos = debMtxApply(&self->basis, powerBasis);
    sDebVec4 firstDeriv;
    firstDeriv.x = splineParam * splineParam * 3.0f;
    firstDeriv.y = splineParam * 2.0f;
    firstDeriv.z = 1.0f;
    firstDeriv.w = 0.0f;
    *vel = debMtxApply(&self->basis, firstDeriv);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cSpline_init);
#ifdef SKIP_ASM
struct cSpline;
float cSpline_calcLength(cSpline* self);

struct sTrackNode51C0 {
    char pad_0x0[0x24];
    int value;              // 0x24
};

struct sTrackSet51C0 {
    char pad_0x0[0x44];
    unsigned int* refs;     // 0x44, (node >> 2) << 8 | low byte
};

struct sTrackWorld51C0 {
    char pad_0x0[0x8];
    sTrackSet51C0** sets;   // 0x8
};

extern "C" sTrackWorld51C0** cObjectInterface_getWorld();

static inline sTrackNode51C0* refToNode51C0(unsigned int p)
{
    return (sTrackNode51C0*)(p << 2);
}

struct sTrackRef51C0 {
    unsigned int id;

    sTrackNode51C0* get()
    {
        sTrackSet51C0* set = (*cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set != 0) {
            unsigned int p = set->refs[id >> 8] >> 8;
            if (p != 0) {
                return refToNode51C0(p);
            }
        }
        return 0;
    }
};

struct sSplineFollow51C0 {
    sTrackRef51C0 ref;      // 0x0
    int field_0x4;          // 0x4
    int field_0x8;          // 0x8
    float length;           // 0xC
};

/**
 * Points a spline at a spline record and measures it.
 *
 * Sets SplineId to id, Seg to the record's first segment (+0x24), SegIndex to 0 and Length to cSpline_calcLength().
 *
 * @param id  resource id of the spline (low 8 bits: resource set, >> 8: index)
 */
extern "C" void cSpline_init(sSplineFollow51C0* self, unsigned int id)
{
    self->ref.id = id;
    sTrackNode51C0* node = self->ref.get();
    self->field_0x8 = node->value;
    self->field_0x4 = 0;
    self->length = cSpline_calcLength((cSpline*)self);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cSpline_getPos);
#ifdef SKIP_ASM
struct sDebKey5248 {
    char pad_0x00[0x60];
    sDebKey5248* prev;  // 0x60
    sDebKey5248* next;  // 0x64
    char pad_0x68[0x1C];
    float t0;           // 0x84
};

struct sDebTrack5248 {
    int field_0x0;
    int index;          // 0x4
    sDebKey5248* key;   // 0x8
    float length;       // 0xC

    void back()
    {
        sDebKey5248* p = key->prev;
        index--;
        key = p;
    }
    void forward()
    {
        sDebKey5248* n = key->next;
        index++;
        key = n;
    }
};

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ftrunc_00345248(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

/**
 * Evaluates the spline at a distance: position, direction and the direction's change, wrapping around its length.
 *
 * The distance is wrapped into [0, Length) (Length +0xC; the floor uses an FPU truncate, then one step down for
 * negatives). The current segment (Seg +0x8, its index +0x4) is stepped back through prev (+0x60) while the distance
 * is before its start (+0x84), then forward through next (+0x64) while the next one starts at or before it; the
 * found segment stays current for the next call. The result comes from cSplineSegment_getPos on that segment.
 *
 * @param distance    the distance along the spline (On Tour distance)
 * @param pos  receives the position
 * @param vel  receives the direction
 * @param acc  receives the direction's derivative (On Tour dDir)
 */
extern "C" void cSpline_getPos(sDebTrack5248* self, float distance, sDebVec4* pos, sDebVec4* vel, sDebVec4* acc)
{
    float len = self->length;
    float laps = distance / len;
    float wholeLaps = ftrunc_00345248(laps);
    if (laps < wholeLaps) {
        wholeLaps -= 1.0f;
    }
    distance -= wholeLaps * len;
    while (distance < self->key->t0 && self->key->prev != 0) {
        self->back();
    }
    sDebKey5248* next = self->key->next;
    while (next != 0 && next->t0 <= distance) {
        self->forward();
        next = self->key->next;
    }
    cSplineSegment_getPos((sDebCurve*)self->key, distance, pos, vel, acc);
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cSpline_isVisible);
#ifdef SKIP_ASM
/**
 * Returns whether a spline's bounding box survives view culling.
 *
 * Builds the min/max corners from the spline record's box (+0x4..+0x18, w = 1) and asks graphics-manager slot 93 to
 * cull them against the view matrix from slot 43.
 *
 * @return 1 unless the cull test returns 1 (fully outside)
 */
extern "C" int cSpline_isVisible(sTrackRef51C0* self)
{
    float* bounds = (float*)self->get();
    sDfVec_343588 box[2];
    box[0].x = bounds[1];
    box[0].y = bounds[2];
    box[0].z = bounds[3];
    box[0].w = 1.0f;
    box[1].x = bounds[4];
    box[1].y = bounds[5];
    box[1].z = bounds[6];
    box[1].w = 1.0f;
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    sDfGfxVt_343588* vt = (sDfGfxVt_343588*)gfx->mVtbl;
    char* thisp = (char*)gfx + vt[93].delta;
    int res = ((DfCullFn_343588)vt[93].fn)(thisp, &box[0], &box[1], ((DfViewFn_343588)((sDfGfxVt_343588*)gfx->mVtbl)[43].fn)((char*)gfx + ((sDfGfxVt_343588*)gfx->mVtbl)[43].delta));
    return res != 1;
}
#endif

//100%
INCLUDE_ASM("object/debouncenode", cSpline_draw);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00345638(void* node);

class cWorld00345430 {
public:
    char pad[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void RenderSpline(void* a, int b, int c);
};

struct sCtl00345430 {
    int f0;
    unsigned int lo : 2;
    unsigned int mode : 5;
    unsigned int hi : 25;
    int f8;
    int fC;
    short f10;
};

/**
 * Draws a spline's visible segments as debug lines in one colour.
 *
 * Sets the current draw state's +0x10 short to -1 and mode bits to 1, then walks the segment list of Seg's spline
 * (+0x68: count +0x20, first +0x24, next +0x64) and, for each segment that func_00345638 reports visible, calls
 * graphics-manager slot 75 with the segment data (+0x10), the colour and 10.
 *
 * @param lineColour  address of the colour to draw with (cSplineModifier passes its +0x20)
 */
extern "C" void cSpline_draw(void* self, int lineColour)
{
    cPS2GraphicsMan* w = cGraphicsMan_TheGraphicsMan;
    char* list = *(char**)(*(char**)((char*)self + 8) + 0x68);
    int n = *(int*)(list + 0x20);
    char* node = *(char**)(list + 0x24);
    (((sCtl00345430*)w->m_pCurrentDrawState))->f10 = -1;
    (((sCtl00345430*)w->m_pCurrentDrawState))->mode = 1;
    int i;
    for (i = 0; i < n; i++) {
        if (func_00345638(node) != 0) {
            ((cWorld00345430*)w)->RenderSpline(node + 0x10, lineColour, 10);
        }
        node = *(char**)(node + 0x64);
    }
}
#endif

