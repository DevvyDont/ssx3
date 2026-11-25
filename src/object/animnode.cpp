#include "common.h"
#include "ai/rider.h"
#include "world/instancerecord.h"
#include "object/movenode.h"
#include "object/animframe.h"
#include "world/worldrecords.h"
#include "vtable.h"
#include "bx/appman.h"
#include "bx/math3d.h"
#include "object/animnode.h"
#include "object/floatingnode.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("object/animnode", cAnimNode_setAnimMeshCache);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern "C" void cAnimFrame_init(void* self);
extern "C" void cMeshAnimFrame_init(void* self);
extern "C" void cAnimFrame_setAnimRecord(void* self, void* src);
extern char str_IntersectInfo[];
extern char str_MeshVelocity[];
extern void* cMeshAnimFrame_vtbl[];
extern void* cAnimFrame_vtbl[];

// cMeshAnimFrame's constructor as new[] inlines it per element: the cAnimFrame part first, then its own. Ctor only;
// the sized operator delete[] makes new[] store the element count.
struct cMeshAnimFrameCtor : cMeshAnimFrame {
    cMeshAnimFrameCtor()
    {
        mVtbl = cAnimFrame_vtbl;
        cAnimFrame_init(this);
        mVtbl = cMeshAnimFrame_vtbl;
        cMeshAnimFrame_init(this);
    }
    void operator delete[](void* p, unsigned int size);
};





/**
 * Allocates an animation node's per-mesh caches: one cMeshAnimFrame per animated mesh and one tMeshVelocity
 * per mesh.
 *
 * Counts the meshes of the instance's model (Instance->m_Model) whose m_MeshHeirarchy entry has an animation
 * record (m_MeshAnim) into NumAnimMeshes. AnimFrames gets that many 0xD0-byte frames (tag "IntersectInfo"; 0 when
 * none) and MeshVelocity one 0x20-byte entry per mesh ("MeshVelocity"); each animated mesh's frame is then pointed
 * at its record (cAnimFrame_setAnimRecord).
 */
void cAnimNode::setAnimMeshCache()
{
    tModelRecord* def = Instance->m_Model;
    tMeshHeirarchyRecord* entry = (def->m_MeshHeirarchy);
    int i;
    NumAnimMeshes = 0;
    for (i = 0; i < def->m_iNumMeshes; i++, entry++) {
        if (((void*)entry->m_MeshAnim) != 0)
            NumAnimMeshes++;
    }
    if (NumAnimMeshes != 0) {
        cMeshAnimFrame*& frames = AnimFrames;
        frames = new (str_IntersectInfo, 0x20000000, 0) cMeshAnimFrameCtor[NumAnimMeshes];
    } else {
        AnimFrames = 0;
    }
    tMeshVelocity*& refs = (MeshVelocity);
    refs = new (str_MeshVelocity, 0x20000000, 0) tMeshVelocity[def->m_iNumMeshes];
    entry = (def->m_MeshHeirarchy);
    int k = 0;
    for (i = 0; i < def->m_iNumMeshes; i++, entry++) {
        if (((void*)entry->m_MeshAnim) != 0) {
            cAnimFrame_setAnimRecord(&AnimFrames[k++], ((void*)entry->m_MeshAnim));
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_calcPosition);
#ifdef SKIP_ASM

// t4Matrix with an empty constructor, as the matrix array here was declared (ctor only).
struct t4MatrixCtor : t4Matrix {
    t4MatrixCtor() {}
};

/**
 * Finds the world position of one animated mesh of an instance: the translation row of its animation matrix.
 *
 * The node's cMoveNode part starts at MoveNodeBase (+0x14; cAnimNodeState comes first); the virtual calls go through
 * mVtbl on that part. When @p id is the node's own Instance, the position is row 3 of the matrix returned by virtual
 * slot 29 (getAnimMat(@p idx)); otherwise slot 33 (makeAnimMats) builds 24 matrices on the stack from the record's root
 * transform (record +0x10) and row 3 of matrix @p idx is used.
 *
 * @param id   the instance record (On Tour tInstanceRecord* inst) whose mesh is wanted
 * @param idx  the mesh index (On Tour mesh)
 * @param out  receives the position (x, y, z, w)
 */
void cAnimNode::calcPosition(int id, int idx, t4Vector* out)
{
    void* obj = (char*)&MoveNodeBase;
    if (id == (int)Instance) {
        sVEntry<t4Matrix*(void*, int)>* vt = (sVEntry<t4Matrix*(void*, int)>*)mVtbl;
        t4Vector position = vt[29].fn(vthis(obj, vt[29]), idx)->row[3];
        *out = position;
        return;
    }
    t4MatrixCtor buf[24];
    sVEntry<void(void*, int, t4Matrix*)>* vt = (sVEntry<void(void*, int, t4Matrix*)>*)mVtbl;
    vt[33].fn(vthis(obj, vt[33]), id + 0x10, buf);
    t4Vector position = buf[idx].row[3];
    *out = position;
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_updateIntersectInfo);
#ifdef SKIP_ASM



// Takes the body as a second (out) argument, as in multiparticle.cpp.
extern "C" void cMoveNode_updateIntersectInfo(void* self, tIntersectInfo* body);
extern "C" void cAnimNode_updateMeshVelocity(cAnimNode* self);
extern "C" void cAnimNode_calcPosition_E698(void* self, int id, int idx, t4Vector* out) __asm__("cAnimNode_calcPosition");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_E698(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector vu0Add_E698(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_E698(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_E698(t4Vector& a, const t4Vector& b)
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

/**
 * Adds the motion of the animated mesh that was hit to an intersection's velocity and angular velocity.
 *
 * Calls cMoveNode_updateIntersectInfo() on the cMoveNode part (MoveNodeBase) first. If the hit names a mesh
 * (@p body Mesh >= 0), it refreshes the per-mesh velocities with cAnimNode_updateMeshVelocity when MeshVelocityDirty
 * is set, gets the mesh's position with cAnimNode_calcPosition (instance @p body Inst), and with that mesh's
 * MeshVelocity entry (Vel, AngVel) does vel += Vel + (hit point - mesh position) x AngVel and angVel += AngVel.
 *
 * @param body  the intersection info (On Tour tIntersectInfo& intInfo): hit point +0x00, velocity +0x20, angular
 *              velocity +0x30, instance +0x50, mesh index +0x5C
 */
// Applies the attached bone's motion delta to the body: vel += delta.pos + (body.pos - bonePos) x delta.rot; angVel += delta.rot.
void cAnimNode::updateIntersectInfo(tIntersectInfo* body)
{
    cMoveNode_updateIntersectInfo(&MoveNodeBase, body);
    int bone = body->Mesh;
    if (bone < 0)
        return;
    if (MeshVelocityDirty != 0)
        updateMeshVelocity();
    t4Vector meshPos;
    cAnimNode_calcPosition_E698(((char*)this), ((int)body->Inst), bone, &meshPos);
    t4Vector offset = vu0Sub_E698((*(&body->Ipos)), meshPos);
    t4Vector rot = MeshVelocity[bone].AngVel;
    vu0AddEq_E698((*(&body->Ivel)), vu0Add_E698(MeshVelocity[bone].Vel,
                                                vu0Cross_E698(offset, MeshVelocity[bone].AngVel)));
    vu0AddEq_E698((*(&body->Iangvel)), rot);
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_updateMeshVelocity);
#ifdef SKIP_ASM
// A t4Vector with EA's constructors (ours has none): returned by value (hidden result pointer) and built from
// four floats.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};







extern t4Vector t4vect_Zero;
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cAnimFrame_getDeriv(void* node, t4Vector* pos, t4Vector* rot, float t);

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEq_E798(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline t4VectorXYZW vu0MulMat_E798(const t4Matrix* m, const t4Vector& v)
{
    t4VectorXYZW r;
    __asm__(
        "lqc2      $vf8, %2\n"
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
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4VectorXYZW vu0Sub_E798(const t4Vector& a, const t4Vector& b)
{
    t4VectorXYZW r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4VectorXYZW vu0Add_E798(const t4Vector& a, const t4Vector& b)
{
    t4VectorXYZW r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4VectorXYZW vu0Cross_E798(const t4Vector& a, const t4Vector& b)
{
    t4VectorXYZW r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_E798(t4Vector& a, const t4Vector& b)
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

// The translation row of mesh @p i's animation matrix. Same-layout cast: t4VectorXYZW only adds constructors to
// t4Vector.
static inline t4VectorXYZW boneE798Pos(cAnimNode* self, int i)
{
    return *(t4VectorXYZW*)&self->AnimMats[i].row[3];
}

/**
 * Recomputes the per-mesh linear and angular velocities (MeshVelocity) of an animated instance.
 *
 * If the cMoveNode part's RootTransformDirty flag (Flags bit 0) is set, virtual slot 51 refreshes the root first.
 * For each mesh of the model's hierarchy (Instance->m_Model: m_iNumMeshes entries of m_MeshHeirarchy) the
 * MeshVelocity entry is zeroed; meshes with an animation (m_MeshAnim) take the next AnimFrames entry and
 * cAnimFrame_getDeriv gives its position/rotation derivatives at CacheTime. Both are scaled by the animation speed
 * (virtual slot 53) times the frame rate (cAppMan_TheAppMan->TimerHz), the rotation also by degrees-to-radians and an
 * Euler matrix built from the frame's negated angles (Vals[3..5]), then transformed by the mesh's AnimMats matrix. A
 * mesh with a parent adds the parent's velocity plus (offset x parent angular velocity) and the parent's angular
 * velocity. Clears MeshVelocityDirty at the end.
 */
void cAnimNode::updateMeshVelocity()
{
    cMoveNode* obj = (cMoveNode*)&MoveNodeBase;
    if (Flags & 1) {
        sVEntry<void>* vt = (sVEntry<void>*)mVtbl;
        ((void (*)(void*))vt[51].fn)(vthis(obj, vt[51]));
    }
    tModelRecord* info = (tModelRecord*)obj->Instance->m_Model;
    int k = 0;
    tMeshHeirarchyRecord* ent = info->m_MeshHeirarchy;
    for (int i = 0; i < info->m_iNumMeshes; i++, ent++) {
        MeshVelocity[i].Vel = t4vect_Zero;
        MeshVelocity[i].AngVel = t4vect_Zero;
        float d2r = 0.01745329424738884f;
        if (((int)ent->m_MeshAnim) == 0) continue;
        cMeshAnimFrame* node = &AnimFrames[k];
        k++;
        sVEntry<void>* vt = (sVEntry<void>*)mVtbl;
        float animSpeed = ((float (*)(void*))vt[53].fn)(vthis(this, vt[53]));
        t4VectorXYZW pos;
        t4VectorXYZW rotv;
        cAnimFrame_getDeriv(node, &pos, &rotv, CacheTime);
        t4VectorXYZW rot;
        rot = t4VectorXYZW(-node->Vals[3], -node->Vals[4], -node->Vals[5], 0.0f);
        vu0ScaleEq_E798(pos, animSpeed * (float)cAppMan_TheAppMan->TimerHz);
        vu0ScaleEq_E798(rotv, animSpeed * (float)cAppMan_TheAppMan->TimerHz * d2r);
        t4Matrix eulerMat;
        {
            t3Vector eulerAngles;
            eulerAngles.x = rot.x;
            eulerAngles.y = rot.y;
            eulerAngles.z = rot.z;
            float sinY, cosY, sinZ, cosZ;
            fSinCos(&sinY, &cosY, eulerAngles.y);
            fSinCos(&sinZ, &cosZ, eulerAngles.z);
            ((&eulerMat.row[0]))[0].x = cosY * cosZ;
            ((&eulerMat.row[0]))[0].y = -cosY * sinZ;
            ((&eulerMat.row[0]))[0].z = sinY;
            ((&eulerMat.row[0]))[0].w = 0.0f;
            ((&eulerMat.row[0]))[1].x = sinZ;
            ((&eulerMat.row[0]))[1].y = cosZ;
            ((&eulerMat.row[0]))[1].z = 0.0f;
            ((&eulerMat.row[0]))[1].w = 0.0f;
            ((&eulerMat.row[0]))[2].x = 0.0f;
            ((&eulerMat.row[0]))[2].y = 0.0f;
            ((&eulerMat.row[0]))[2].z = 1.0f;
            ((&eulerMat.row[0]))[2].w = 0.0f;
            ((&eulerMat.row[0]))[3].x = 0.0f;
            ((&eulerMat.row[0]))[3].y = 0.0f;
            ((&eulerMat.row[0]))[3].z = 0.0f;
            ((&eulerMat.row[0]))[3].w = 1.0f;
        }
        rotv = vu0MulMat_E798(&eulerMat, rotv);
        pos = vu0MulMat_E798(&AnimMats[i], pos);
        rotv = vu0MulMat_E798(&AnimMats[i], rotv);
        if (ent->m_iParentID >= 0) {
            t4VectorXYZW parentOffset = vu0Sub_E798(boneE798Pos(this, i), boneE798Pos(this, ent->m_iParentID));
            vu0AddEq_E798(rotv, MeshVelocity[ent->m_iParentID].AngVel);
            vu0AddEq_E798(pos, vu0Add_E798(MeshVelocity[ent->m_iParentID].Vel,
                                           vu0Cross_E798(parentOffset, MeshVelocity[ent->m_iParentID].AngVel)));
        }
        MeshVelocity[i].Vel = pos;
        vu0AddEq_E798(MeshVelocity[i].AngVel, rotv);
    }
    MeshVelocityDirty = 0;
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_frameUpdate);
#ifdef SKIP_ASM
struct sAnimLinkList_EBA0;

struct sAnimLink_EBA0 {
    char pad_0x00[0x18];
    tInstanceRecord* list; // 0x18
};


extern "C" void cObjectInterface_runFrameUpdateScript(tInstanceRecord* list);

struct sAnimNode_EBA0 {
    char pad_0x00[0x14];
    sAnimLink_EBA0 link; // 0x14
};

/**
 * Runs the frame-update world script of the instance whose node sits 0x14 bytes into @p self, and returns whether that
 * node is still the instance's active node.
 *
 * Passes the instance record at @p self + 0x2C (the node's Instance) to cObjectInterface_runFrameUpdateScript, which runs the record's type-5
 * script through cWScriptMan_doOnFrameUpdateScript (the type cWScriptMan uses for frame-update scripts), then compares the record's
 * active-node pointer (+0xC) with @p self + 0x14. Its callers (cAnimObjectNode_updateNode, cAnimTeeterNode_updateNode, cAnimComboNode_updateNode) return 0
 * from their node update when it returns 0.
 *
 * @return 1 if the node at @p self + 0x14 is still the record's active node, otherwise 0
 */
// is this node's link at the head of its list, after cObjectInterface_runFrameUpdateScript?
int cAnimNode::frameUpdate()
{
    sAnimLink_EBA0* link = (sAnimLink_EBA0*)&MoveNodeBase;
    cObjectInterface_runFrameUpdateScript(Instance);
    return ((sAnimLink_EBA0*)link->list->m_pActiveNode) == link;
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_frameUpdateEvent);
#ifdef SKIP_ASM
struct sRange_EBE0 {
    char pad[8];
    float start;  // 0x8; 1e10 = unset
    float end;    // 0xC
};

/**
 * Tells whether a time lies inside the range stored at offsets 8 and 0xC.
 *
 * With start < end the range is [start, end); otherwise it is [end, start). A start of 1e10 means the range is
 * unset, and the answer is then 0.
 *
 * @param frame  the time to test
 * @return 1 if @p frame is inside the range, otherwise 0
 */
// Is t between the two bounds: [start, end) when start < end, else [end, start).
int cAnimNode::frameUpdateEvent(float frame)
{
    int inRange = 0;
    if (FrameStart != 1e10f) {
        if (FrameStart < FrameEnd) {
            if (FrameStart <= frame && frame < FrameEnd) inRange = 1;
        } else {
            if (!(FrameEnd <= frame)) return 0;
            inRange = frame < FrameStart;
        }
    }
    return inRange;
}
#endif

//100%
INCLUDE_ASM("object/animnode", cFloatingNode_cFloatingNode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a2);
struct cFloatingNode;
void cFloatingNode_initInfo(cFloatingNode* self);
extern char str_FloatingMats[];
extern void* cFloatingNode_vtbl[];

struct sAnimFrame_EC58 {
    char data[0x40];
    sAnimFrame_EC58() {}
};


struct sAnimInfo_EC58 {
    int f0;
    float f4, f8, fC, f10, f14, f18;
};

/**
 * Constructs a floating node from a placed instance and its floating parameters.
 *
 * Runs the cMoveNode constructor cMoveNode_cMoveNode1 with node type 0xE, installs cFloatingNode_vtbl and copies the spring
 * constants from @p info: ZSpring (+0x4), ZDamp (+0x8), ASpring (+0xC), ADamp (+0x10), ForceScale (+0x14) and
 * TorqueScale (+0x18 scaled by 1e-6). Zeroes the tilt state (OX, WX, TX, OY, WY, TY), Vel, Force and Info, and sets Zo
 * and Z to the z of the instance's root translation (record + 0x40). Then calls cFloatingNode_initInfo, allocates
 * AnimMats ("FloatingMats", one 0x40-byte matrix per mesh of the instance's model), sets the RootTransformDirty bit in
 * Flags and, in the instance record's flags (+0x8), clears bit 1 and sets bit 2.
 *
 * The world script loader (src/seg/seg_1FBE38.cpp) calls it on a cMemMan_alloc(0x7C) block.
 *
 * @param bucket  the bucket the node is added to
 * @param instRecord  the instance record (tInstanceRecord) the node animates
 * @param info  the floating parameters: spring, damping and scale values at +0x4..+0x18
 * @return @p self
 */
extern "C" void* cFloatingNode_cFloatingNode(cFloatingNode* self, void* bucket, void* instRecord, sAnimInfo_EC58* info)
{
    cMoveNode_cMoveNode1(self, bucket, 0xE, instRecord);
    VPTR_OF(cObjNode, self) = cFloatingNode_vtbl;
    self->ZSpring = info->f4;
    self->ZDamp = info->f8;
    self->ForceScale = info->f14;
    self->ASpring = info->fC;
    self->ADamp = info->f10;
    self->TorqueScale = info->f18 * 1e-6f;
    t4Vector rootPos = *(t4Vector*)&self->Instance->m_fmTransform.m[3][0];
    *(int*)&self->OX = 0;
    *(int*)&self->WX = 0;
    *(int*)&self->TX = 0;
    *(int*)&self->OY = 0;
    *(int*)&self->WY = 0;
    *(int*)&self->TY = 0;
    *(int*)&self->Vel = 0;
    *(int*)&self->Force = 0;
    self->Info = 0;
    self->Zo = rootPos.z;
    self->Z = rootPos.z;
    cFloatingNode_initInfo((cFloatingNode*)self);
    sAnimFrame_EC58** pf = (sAnimFrame_EC58**)(&self->AnimMats);
    *pf = new (str_FloatingMats, 0x20000000, 0) sAnimFrame_EC58[self->Instance->m_Model->m_iNumMeshes];
    self->Flags |= 1;
    tInstanceRecord* record = self->Instance;
    record->m_iFlags = ((unsigned int)record->m_iFlags & 0xFFFFFFFD) | 4;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/animnode", cFloatingNode_cFloatingNode1);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self, void* a1, void* stream);
struct cFloatingNode;
void cFloatingNode_initInfo(cFloatingNode* self);
extern void* cFloatingNode_vtbl[];
extern char str_FloatingMats[];

// Element type with an empty constructor; the array new leaves only its loop counters.
struct sAnimFrame_ED88 {
    char data[0x40];
    sAnimFrame_ED88() {}
};

/**
 * Reconstructs a floating node from a replay frame.
 *
 * Runs the replay cMoveNode constructor (cMoveNode_cMoveNode), clears Info and installs cFloatingNode_vtbl. Reads a
 * 4-byte flag from @p stream (virtual slot 2, read) and calls cFloatingNode_initInfo when it is set, then reads the
 * 0x48-byte cFloatingNodeState into +0x2C. Allocates AnimMats ("FloatingMats", one 0x40-byte matrix per mesh of the
 * instance's model) and sets the RootTransformDirty bit in Flags.
 *
 * The replay loader calls it for node type 14 on a cMemMan_alloc(0x7C, "Floating") block.
 *
 * @param bucket  the bucket the node is added to
 * @param stream  the replay frame to read from
 * @return @p self
 */
void* cFloatingNode::cFloatingNode1(void* bucket, cBxReplayFrame* stream)
{
    int hasInfo;
    cMoveNode_cMoveNode(this, bucket, stream);
    Info = 0;
    VPTR_OF(cObjNode, this) = cFloatingNode_vtbl;
    stream->read((char*)&hasInfo, 4);
    if (hasInfo != 0) {
        cFloatingNode_initInfo((cFloatingNode*)this);
    }
    stream->read((char*)&ZSpring, 72);
    sAnimFrame_ED88** pf = (sAnimFrame_ED88**)(&AnimMats);
    *pf = new (str_FloatingMats, 0x20000000, 0) sAnimFrame_ED88[Instance->m_Model->m_iNumMeshes];
    Flags |= 1;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/animnode", cFloatingNode__cFloatingNode);
#ifdef SKIP_ASM
extern void* cFloatingNode_vtbl[];
extern "C" void cFloatingNode_restoreOverlapSystem(void* self);
extern "C" void cMoveNode__cMoveNode(void* self, int flags);
void operator_delete(int*);

/**
 * Destroys a floating node: restores its vtable, undoes its overlap-system changes and frees its buffer.
 *
 * Calls cFloatingNode_restoreOverlapSystem(), frees the block at offset 0x74, then runs cMoveNode__cMoveNode().
 *
 * @param flags  g++ destructor flags, passed to cMoveNode__cMoveNode(); bit 0 set frees the object's memory
 */
extern "C" void cFloatingNode__cFloatingNode(cFloatingNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = cFloatingNode_vtbl;
    cFloatingNode_restoreOverlapSystem(self);
    operator_delete((int*)self->AnimMats);
    cMoveNode__cMoveNode(self, flags);
}
#endif

