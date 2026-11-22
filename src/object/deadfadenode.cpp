#include "common.h"
#include "world/instancerecord.h"
#include "vtable.h"
#include "object/deadfadenode.h"
#include "object/restorenode.h"
#include "object/hidenode.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_cDeadFadeNode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* a3);
extern "C" float cObjectInterface_getUpdateTime();
extern "C" void MEM_copy(void* dst, void* src, int size);
extern char cDeadFadeNode_vtbl[];
extern char str_DeadFadeAnimMats[];

// PORT: array new bound to the tagged allocator
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sDfV4_0750 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDfMat_0750 {
    sDfV4_0750 r[4];
};

struct sDfMatElem_0750 {
    sDfV4_0750 r[4];
    sDfMatElem_0750() {}
};

struct sDfPart_0750 {
    int parent;
    int pad4;
    int pad8;
    sDfMat_0750* mat;   // 0xC
};

struct sDfModel_0750 {
    int pad0;
    int count;              // 0x4
    sDfPart_0750* parts;    // 0x8
};


struct sDfDesc_0750 {
    int w[2];
    float f8;
    float fC;
};


struct sDfVE_0750 {
    short delta;
    short index;
    void* fn;
};

struct sDfOther_0750 {
    char pad_0x00[0xC];
    sDfVE_0750* vt;         // 0xC
};


// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_0750(sDfMat_0750* d, const void* a, const sDfMat_0750* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
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
        ".set reorder\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_0750(void* d, const void* s)
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

static inline sDfV4_0750 row_0750(const sDfMat_0750* m, int i)
{
    return m->r[i];
}

static inline void mulv_0750(sDfV4_0750& r, const sDfV4_0750& a, const sDfV4_0750& b)
{
    r.x = a.x * b.x;
    r.y = a.y * b.y;
    r.z = a.z * b.z;
    r.w = a.w * b.w;
}

// Builds a part matrix: dst = parent * (local with its translation row scaled by s).
static inline void partMat_0750(sDfMat_0750* dst, const void* parentMat, const sDfMat_0750* m, const sDfV4_0750& s)
{
    sDfMat_0750 l;
    l.r[0] = row_0750(m, 0);
    l.r[1] = row_0750(m, 1);
    l.r[2] = row_0750(m, 2);
    sDfV4_0750 t;
    sDfV4_0750 r3 = m->r[3];
    mulv_0750(t, r3, s);
    l.r[3] = t;
    sDfMat_0750 tt;
    vu0MulMat_0750(&tt, parentMat, &l);
    vu0CopyMat_0750(dst, &tt);
}

typedef int (*DfBoolFn_0750)(void*);
typedef sDfMat_0750* (*DfMatFn_0750)(void*);
typedef void (*DfSetFn_0750)(void*, int);

/**
 * Constructs a dead-fade node: the object fades out and sinks while keeping the pose it had when it died.
 *
 * Copies the first 12 bytes of the fade data to +0x1C, sets the fade step to frame time / fade duration (desc
 * +0x8) and the sink speed to desc +0xC * frame time, starting fully opaque at depth 0. Allocates one matrix
 * per model part ("DeadFadeAnimMats"): copied from the old node's animation matrices when it has them (slots
 * 26/27), otherwise built from the part hierarchy under the old node's root transform (slot 24) or the
 * instance's world matrix. Then deletes the old node and updates the instance's flag bits.
 *
 * @param bucket  update bucket passed to the instance-node constructor
 * @param obj     the world instance (On Tour tInstanceRecord*)
 * @param desc    the fade data (On Tour tMakeDeadFadeData*)
 * @param other   the instance's previous node, or 0 (On Tour pOldNode); it is deleted here
 * @return self
 */
extern "C" cDeadFadeNode* cDeadFadeNode_cDeadFadeNode(cDeadFadeNode* self, int bucket, void* obj, sDfDesc_0750* desc, sDfOther_0750* other)
{
    sDfMatElem_0750** pm = (sDfMatElem_0750**)&self->AnimMats;
    cInstanceNode_cInstanceNode1(self, bucket, 22, obj);
    *(void**)&self->mVtbl = cDeadFadeNode_vtbl;
    ((unsigned int*)&(*(&self->Data)).InstId)[0] = 0xFFFFFFFF;
    *(&self->Data) = *(tMakeDeadData*)desc;
    self->FadeRate = cObjectInterface_getUpdateTime() / desc->f8;
    self->DistRate = desc->fC * cObjectInterface_getUpdateTime();
    self->Dist = 0.0f;
    self->Fade = 1.0f;
    sDfModel_0750* model = ((sDfModel_0750*)(self->Instance)->m_Model);
    *pm = new (str_DeadFadeAnimMats, 0x20000000, 0) sDfMatElem_0750[model->count];
    if (other != 0 && ((DfBoolFn_0750)other->vt[26].fn)(vthis(other, other->vt[26]))) {
        sDfMat_0750* src = ((DfMatFn_0750)other->vt[27].fn)(vthis(other, other->vt[27]));
        MEM_copy(((sDfMatElem_0750*)self->AnimMats), src, model->count << 6);
    } else {
        sDfMat_0750* base = &(*(sDfMat_0750*)&(self->Instance)->m_fmTransform);
        if (other != 0) {
            base = ((DfMatFn_0750)other->vt[24].fn)(vthis(other, other->vt[24]));
        }
        sDfPart_0750* part = model->parts;
        float k = (self->Instance)->m_fScale;
        sDfV4_0750 scaleVec;
        scaleVec.x = k;
        scaleVec.y = k;
        scaleVec.z = k;
        scaleVec.w = 1.0f;
        sDfMat_0750 partMatrix;
        for (int i = 0; i < model->count; i++, part++) {
            int parent = part->parent;
            if (parent != -1) {
                partMat_0750(&partMatrix, &((sDfMatElem_0750*)self->AnimMats)[parent], part->mat, scaleVec);
                vu0CopyMat_0750(&((sDfMatElem_0750*)self->AnimMats)[i], &partMatrix);
            } else {
                partMat_0750(&partMatrix, base, part->mat, scaleVec);
                vu0CopyMat_0750(&((sDfMatElem_0750*)self->AnimMats)[i], &partMatrix);
            }
        }
    }
    if (other != 0) {
        ((DfSetFn_0750)other->vt[1].fn)(vthis(other, other->vt[1]), 3);
    }
    (*(unsigned int*)&(self->Instance)->m_iFlags) &= ~0x60;
    (self->Instance)->m_iFlags = (((unsigned int)(self->Instance)->m_iFlags) & ~2) | 4;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_cDeadFadeNode1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cInstanceNode_ctor_v(void* self) __asm__("cInstanceNode_cInstanceNode");
extern char cDeadFadeNode_vtbl[];

/**
 * Reconstructs a dead-fade node from a replay frame.
 *
 * Runs the replay cInstanceNode constructor (cInstanceNode_cInstanceNode), installs cDeadFadeNode_vtbl, sets the word
 * at +0x1C to -1 and clears the word at +0x38. The replay loader calls it for node type 22 on a cMemMan_alloc(0x3C,
 * "DeadFade") block and deletes the node again at once.
 *
 * @return @p self
 */
extern "C" void* cDeadFadeNode_cDeadFadeNode1(cDeadFadeNode* self)
{
    cInstanceNode_ctor_v(self);
    self->mVtbl = (void**)cDeadFadeNode_vtbl;
    self->Data.InstId = 0xFFFFFFFFU;
    self->AnimMats = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode__cDeadFadeNode);
#ifdef SKIP_ASM
extern char cDeadFadeNode_vtbl[];
void cMemMan_free(void*);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

/**
 * Destroys a dead-fade node: frees its part matrices (+0x38), then runs the cInstanceNode destructor.
 *
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cDeadFadeNode__cDeadFadeNode(cDeadFadeNode* self, int flags)
{
    self->mVtbl = (void**)cDeadFadeNode_vtbl;
    void* partMats = self->AnimMats;
    if (partMats != 0) {
        cMemMan_free(partMats);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_update);
#ifdef SKIP_ASM
// PORT: the project names this cObjectInterface_makeDead__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* cObjectInterface_makeDead_2(void*, void*) __asm__("cObjectInterface_makeDead__FPv");

/**
 * Advances a dead-fade node by one frame: sinks it further and fades it out.
 *
 * Adds the sink speed to the depth (+0x30) and subtracts the fade step from the opacity (+0x28). Once the
 * opacity has reached 0, cObjectInterface_makeDead() applies the fade data's follow-up event to the instance (event 0
 * turns it into a dead node, 3 into a restore node).
 */
extern "C" void cDeadFadeNode_update(cDeadFadeNode* self)
{
    float fade;
    self->Dist += self->DistRate;
    fade = self->Fade;
    if (fade > 0.0f) {
        self->Fade = fade - self->FadeRate;
    } else {
        cObjectInterface_makeDead_2(self->Instance, &self->Data);
    }
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_loseFocus);
#ifdef SKIP_ASM
// PORT: the project names this cObjectInterface_makeDead__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* cObjectInterface_makeDead_2(void*, void*) __asm__("cObjectInterface_makeDead__FPv");

/**
 * Applies the dead fade node's stored instance event (+0x1C) to its instance record (+0x18) when it loses
 * focus.
 *
 * cObjectInterface_makeDead() passes both to makeDead(), which by event kind deletes the record's node (1), replaces
 * it with a new "DeadNode" (0) or with a "RestoreNode" (3); the update does the same once the fade timer runs
 * out.
 */
extern "C" void cDeadFadeNode_loseFocus(cDeadFadeNode* self)
{
    cObjectInterface_makeDead_2(self->Instance, &self->Data);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sDfVec_350CB8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sDfMat_350CB8 {
    sDfVec_350CB8 r[4];
};

struct sDfSlot_350CB8 {
    int f0;
    int type;
};

struct sDfCam_350CB8 {
    char pad_0x0[0x20];
    sDfSlot_350CB8 slots[(0x3F0 - 0x20) / 8];  // 0x20
    struct {
        int kind;
        char pad[0x14];
    } paths[1];                     // 0x3F0
};

struct sDfObj_350CB8 {
    char pad_0x0[0x60];
    float pos[3];                   // 0x60
    float scl[3];                   // 0x6C
    char pad_0x78[0x7D - 0x78];
    unsigned char slot;             // 0x7D
    short path;                     // 0x7E
    char pad_0x80[0x94 - 0x80];
    void* light;                    // 0x94
};


struct cDfGfx_350CB8 {
    char pad_0x0[0x10D8];
    sVEntry<void>* vt;            // 0x10D8
};

typedef sDfMat_350CB8 (*DfViewFn_350CB8)(void*);
typedef int (*DfCullFn_350CB8)(void*, sDfVec_350CB8*, sDfVec_350CB8*, const sDfMat_350CB8&);
typedef void (*DfDrawFn_350CB8)(void*, void*, void*, void*, float, void*, void*, int);

extern sDfMat_350CB8 D_004FF1A0_350CB8 __asm__("t4mat_Identity");

#define OBJ_350CB8(s) (*(sDfObj_350CB8**)((char*)(s) + 0x18))

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void MatCopy_350CB8(sDfMat_350CB8* d, const sDfMat_350CB8* s)
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

static inline bool IsT6_350CB8(sDfCam_350CB8* c, int i)
{
    return c->slots[i].type == 6;
}

static inline int Visible_350CB8(sDfCam_350CB8* cam, sDfObj_350CB8* obj)
{
    if (obj->slot == 0xFF) {
        return 1;
    }
    if (IsT6_350CB8(cam, obj->slot)) {
        return cam->paths[obj->path].kind == 3;
    }
    return 0;
}

/**
 * Draws a fading dead object, lowered by its sink depth and with its current opacity.
 *
 * Skips the object unless the world cache reports its section as drawable, or when its bounding box the
 * graphics manager culls (slot 93), then draws the instance (slot 95) with an identity matrix translated down
 * by the depth (+0x30), the opacity (+0x28) and the saved part matrices (+0x38).
 */
// Draws the fading corpse: culls its box, then draws it lowered by the fade depth at 0x30.
extern "C" void cDeadFadeNode_render(cDeadFadeNode* self)
{
    sDfCam_350CB8* cam = (sDfCam_350CB8*)cWorld_m_pTheWorld->m_pWorldCache;
    if (!Visible_350CB8(cam, OBJ_350CB8(self))) {
        return;
    }
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    {
        sVEntry<void>* vt = (sVEntry<void>*)gfx->mVtbl;
        char* thisp = (char*)gfx + vt[93].delta;
        sDfVec_350CB8 box[2];
        box[0].x = OBJ_350CB8(self)->pos[0];
        box[0].y = OBJ_350CB8(self)->pos[1];
        box[0].z = OBJ_350CB8(self)->pos[2];
        box[0].w = 1.0f;
        box[1].x = OBJ_350CB8(self)->scl[0];
        box[1].y = OBJ_350CB8(self)->scl[1];
        box[1].z = OBJ_350CB8(self)->scl[2];
        box[1].w = 1.0f;
        int res = ((DfCullFn_350CB8)vt[93].fn)(thisp, &box[0], &box[1], ((DfViewFn_350CB8)((sVEntry<void>*)gfx->mVtbl)[43].fn)((char*)gfx + ((sVEntry<void>*)gfx->mVtbl)[43].delta));
        if (res == 1) {
            return;
        }
    }
    sDfMat_350CB8 xf;
    MatCopy_350CB8(&xf, &D_004FF1A0_350CB8);
    sDfVec_350CB8 offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = -self->Dist;
    offset.w = 1.0f;
    xf.r[3] = offset;
    ((DfDrawFn_350CB8)((sVEntry<void>*)gfx->mVtbl)[95].fn)((char*)gfx + ((sVEntry<void>*)gfx->mVtbl)[95].delta, OBJ_350CB8(self), OBJ_350CB8(self)->light, &xf,
                                      self->Fade, self->AnimMats, 0, 0);
}
#endif

extern "C" void* cInstanceNode_writeToReplayFrame(void* self);

//100%
INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the dead-fade node's replay data using the base cInstanceNode_writeToReplayFrame().
 *
 * Only @p self is declared; the replay stream argument reaches the base function unchanged in its register.
 *
 * @return no meaningful value (cInstanceNode_writeToReplayFrame() returns nothing)
 */
void* cDeadFadeNode_writeToReplayFrame(void* self)
{
    return cInstanceNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cHideNode_cHideNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* a3);
extern char cHideNode_vtbl[];

/**
 * Constructs a hide node for an instance record (node type 16, bucket 1).
 *
 * Runs the cInstanceNode constructor cInstanceNode_cInstanceNode1 with bucket 1, type 0x10 and the instance record, installs the
 * vtable cHideNode_vtbl, then in the record's flags (+0x8) clears bits 5 and 6 (0x60) and bit 1 and sets bit 2. The world
 * script code (src/seg/seg_1FBE38.cpp) calls it on a cMemMan_alloc(0x1C) block.
 *
 * @param instRecord  the instance record to hide
 * @return @p self
 */
extern "C" void* cHideNode_cHideNode(cHideNode* self, void* instRecord)
{
    cInstanceNode_cInstanceNode1(self, 1, 0x10, instRecord);
    self->mVtbl = (void**)cHideNode_vtbl;
    unsigned int* recordFlags = (unsigned int*)(&self->Instance->m_iFlags);
    *recordFlags &= 0xFFFFFF9F;
    recordFlags = (unsigned int*)(&self->Instance->m_iFlags);
    *recordFlags = (*recordFlags & 0xFFFFFFFD) | 4;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cHideNode_cHideNode1);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char cHideNode_vtbl[];

/**
 * Reconstructs a hide node from a replay frame.
 *
 * Runs the replay cInstanceNode constructor (cInstanceNode_cInstanceNode) and installs the hide node vtable cHideNode_vtbl.
 * The replay loader calls it for node type 16 on a cMemMan_alloc(0x1C, "Hide") block.
 *
 * @return @p self
 */
extern "C" void* cHideNode_cHideNode1(cHideNode* self)
{
    cInstanceNode_cInstanceNode(self);
    self->mVtbl = (void**)cHideNode_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cHideNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the node to a replay frame by forwarding to cInstanceNode_writeToReplayFrame.
 *
 * It fills the writeToReplayFrame slot (word 25) of the hide node vtable cHideNode_vtbl.
 *
 * @return cInstanceNode_writeToReplayFrame()'s result
 */
void* cHideNode_writeToReplayFrame(void* self)
{
    return cInstanceNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cRestoreNode_cRestoreNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* a3);
extern char cRestoreNode_vtbl[];

/**
 * Constructs a restore node for an instance record (node type 19, bucket 1).
 *
 * Runs the cInstanceNode constructor cInstanceNode_cInstanceNode1 with bucket 1, type 19 and the instance record, installs the vtable
 * cRestoreNode_vtbl, then in the record's flags (+0x8) clears bits 5 and 6 (0x60) and bit 1 and sets bit 2. World script code
 * and the debounce node code call it on a cMemMan_alloc(0x1C, "RestoreNode") block.
 *
 * @param instRecord  the instance record to restore
 * @return @p self
 */
extern "C" void* cRestoreNode_cRestoreNode(cRestoreNode* self, void* instRecord)
{
    cInstanceNode_cInstanceNode1(self, 1, 19, instRecord);
    self->mVtbl = (void**)cRestoreNode_vtbl;
    unsigned int* recordFlags = (unsigned int*)(&self->Instance->m_iFlags);
    *recordFlags &= 0xFFFFFF9F;
    recordFlags = (unsigned int*)(&self->Instance->m_iFlags);
    *recordFlags = (*recordFlags & 0xFFFFFFFD) | 4;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cRestoreNode_cRestoreNode1);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char cRestoreNode_vtbl[];

/**
 * Reconstructs a restore node from a replay frame.
 *
 * Runs the replay cInstanceNode constructor (cInstanceNode_cInstanceNode) and installs the restore node vtable
 * cRestoreNode_vtbl. The replay loader calls it for node type 19 on a cMemMan_alloc(0x1C, "Restore") block.
 *
 * @return @p self
 */
extern "C" void* cRestoreNode_cRestoreNode1(cRestoreNode* self)
{
    cInstanceNode_cInstanceNode(self);
    self->mVtbl = (void**)cRestoreNode_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", cRestoreNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the node to a replay frame by forwarding to cInstanceNode_writeToReplayFrame.
 *
 * It fills the writeToReplayFrame slot (word 25) of the restore node vtable cRestoreNode_vtbl.
 *
 * @return cInstanceNode_writeToReplayFrame()'s result
 */
void* cRestoreNode_writeToReplayFrame(void* self)
{
    return cInstanceNode_writeToReplayFrame(self);
}
#endif

