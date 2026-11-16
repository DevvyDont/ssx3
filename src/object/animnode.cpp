#include "common.h"
#include "object/floatingnode.h"

//100%
INCLUDE_ASM("object/animnode", cAnimNode_setAnimMeshCache);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_003513D0(void* self);
extern "C" void func_003612C0(void* self);
extern "C" void func_00351398(void* self, void* src);
extern char str_IntersectInfo[];
extern char str_MeshVelocity[];
extern void* cMeshAnimFrame_vtbl[];
extern void* cAnimFrame_vtbl[];

struct sMeshInst_E448 {
    char pad_0x0[0x84];
    void** vtable;          // 0x84
    char pad_0x88[0x48];
    sMeshInst_E448()
    {
        vtable = cAnimFrame_vtbl;
        func_003513D0(this);
        vtable = cMeshAnimFrame_vtbl;
        func_003612C0(this);
    }
    void operator delete[](void* p, unsigned int size);
};

struct sMeshRef_E448 {
    char data[0x20];
    sMeshRef_E448() {}
};

struct sAnimEntry_E448 {
    int pad_0x0[2];
    void* src;              // 0x8
    int pad_0xC;
};

struct sAnimDef_E448 {
    int pad_0x0;
    int count;                  // 0x4
    sAnimEntry_E448* entries;   // 0x8
};

struct sAnimNode_E448 {
    char pad_0x0[0x2C];
    char* res;                  // 0x2C
    char pad_0x30[0x10];
    int numMeshes;              // 0x40
    int pad_0x44;
    sMeshInst_E448* meshes;     // 0x48
    sMeshRef_E448* refs;        // 0x4C
};

/**
 * Allocates an animation node's per-mesh caches: one cMeshAnimFrame per animated mesh and one tMeshVelocity
 * per mesh.
 *
 * Counts the meshes of the instance's model (record +0x2C, model +0x80) that have an animation record (+0x8
 * of each 0x10-byte entry) into NumAnimMeshes (+0x40). AnimFrames (+0x48) gets that many 0xD0-byte frames
 * (tag "IntersectInfo"; 0 when none) and MeshVelocity (+0x4C) one 0x20-byte entry per mesh ("MeshVelocity");
 * each animated mesh's frame is then pointed at its record (func_00351398).
 */
extern "C" void cAnimNode_setAnimMeshCache(sAnimNode_E448* self)
{
    sAnimDef_E448* def = *(sAnimDef_E448**)(self->res + 0x80);
    sAnimEntry_E448* entry = def->entries;
    int i;
    self->numMeshes = 0;
    for (i = 0; i < def->count; i++, entry++) {
        if (entry->src != 0)
            self->numMeshes++;
    }
    if (self->numMeshes != 0) {
        sMeshInst_E448*& meshes = self->meshes;
        meshes = new (str_IntersectInfo, 0x20000000, 0) sMeshInst_E448[self->numMeshes];
    } else {
        self->meshes = 0;
    }
    sMeshRef_E448*& refs = self->refs;
    refs = new (str_MeshVelocity, 0x20000000, 0) sMeshRef_E448[def->count];
    entry = def->entries;
    int k = 0;
    for (i = 0; i < def->count; i++, entry++) {
        if (entry->src != 0) {
            func_00351398(&self->meshes[k++], entry->src);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_calcPosition);
#ifdef SKIP_ASM
struct sVec4_E600 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_E600 {
    sVec4_E600 r[4];
    sMat_E600() {}
};

struct sVEntry_E600a {
    short delta;
    short index;
    sMat_E600* (*fn)(void*, int);
};

struct sVEntry_E600b {
    short delta;
    short index;
    void (*fn)(void*, int, sMat_E600*);
};

/**
 * Finds the world position of one animated mesh of an instance: the translation row of its animation matrix.
 *
 * The node's cMoveNode part starts at +0x14 (cAnimNodeState comes first), so its Instance is +0x2C and its vtable
 * +0x20. When @p id is the node's own instance, the position is row 3 of the matrix returned by virtual slot 29
 * (getAnimMat(@p idx)); otherwise slot 33 (makeAnimMats) builds 24 matrices on the stack from the record's root
 * transform (record +0x10) and row 3 of matrix @p idx is used.
 *
 * @param id   the instance record (On Tour tInstanceRecord* inst) whose mesh is wanted
 * @param idx  the mesh index (On Tour mesh)
 * @param out  receives the position (x, y, z, w)
 */
extern "C" void cAnimNode_calcPosition(void* self, int id, int idx, sVec4_E600* out)
{
    void* obj = (char*)self + 0x14;
    if (id == *(int*)((char*)self + 0x2C)) {
        sVEntry_E600a* vt = *(sVEntry_E600a**)((char*)self + 0x20);
        sVec4_E600 t = vt[29].fn((char*)obj + vt[29].delta, idx)->r[3];
        *out = t;
        return;
    }
    sMat_E600 buf[24];
    sVEntry_E600b* vt = *(sVEntry_E600b**)((char*)self + 0x20);
    vt[33].fn((char*)obj + vt[33].delta, id + 0x10, buf);
    sVec4_E600 t = buf[idx].r[3];
    *out = t;
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_updateIntersectInfo);
#ifdef SKIP_ASM
struct sVec4_E698 {
    float x, y, z, w;
    sVec4_E698() {}
} __attribute__((aligned(16)));

struct sDelta_E698 {
    sVec4_E698 pos;
    sVec4_E698 rot;
};

struct sBody_E698 {
    sVec4_E698 pos;     // 0x00
    char pad10[0x10];
    sVec4_E698 vel;     // 0x20
    sVec4_E698 angVel;  // 0x30
    char pad40[0x10];
    int anim;           // 0x50
    char pad54[0x8];
    int bone;           // 0x5C
};

// Takes the body as a second (out) argument, as in multiparticle.cpp.
extern "C" void cMoveNode_updateIntersectInfo(void* self, sBody_E698* body);
extern "C" void cAnimNode_updateMeshVelocity(char* self);
extern "C" void cAnimNode_calcPosition_E698(void* self, int id, int idx, sVec4_E698* out) __asm__("cAnimNode_calcPosition");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_E698 vu0Sub_E698(const sVec4_E698& a, const sVec4_E698& b)
{
    sVec4_E698 r;
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
static inline sVec4_E698 vu0Add_E698(const sVec4_E698& a, const sVec4_E698& b)
{
    sVec4_E698 r;
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
static inline sVec4_E698 vu0Cross_E698(const sVec4_E698& a, const sVec4_E698& b)
{
    sVec4_E698 r;
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
static inline void vu0AddEq_E698(sVec4_E698& a, const sVec4_E698& b)
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
 * Calls cMoveNode_updateIntersectInfo() on the cMoveNode part (+0x14) first. If the hit names a mesh (@p body +0x5C
 * >= 0), it refreshes the per-mesh velocities with cAnimNode_updateMeshVelocity when MeshVelocityDirty (+0x10) is set, gets the
 * mesh's position with cAnimNode_calcPosition (instance at @p body +0x50), and with that mesh's MeshVelocity entry (+0x4C,
 * 0x20 bytes: linear, angular) does vel += linear + (hit point - mesh position) x angular and angVel += angular.
 *
 * @param body  the intersection info (On Tour tIntersectInfo& intInfo): hit point +0x00, velocity +0x20, angular
 *              velocity +0x30, instance +0x50, mesh index +0x5C
 */
// Applies the attached bone's motion delta to the body: vel += delta.pos + (body.pos - bonePos) x delta.rot; angVel += delta.rot.
extern "C" void cAnimNode_updateIntersectInfo(char* self, sBody_E698* body)
{
    cMoveNode_updateIntersectInfo(self + 0x14, body);
    int bone = body->bone;
    if (bone < 0)
        return;
    if (*(int*)(self + 0x10) != 0)
        cAnimNode_updateMeshVelocity(self);
    sVec4_E698 p;
    cAnimNode_calcPosition_E698(self, body->anim, bone, &p);
    sVec4_E698 d = vu0Sub_E698(body->pos, p);
    sVec4_E698 rot = ((sDelta_E698*)*(char**)(self + 0x4C))[bone].rot;
    vu0AddEq_E698(body->vel, vu0Add_E698(((sDelta_E698*)*(char**)(self + 0x4C))[bone].pos,
                                          vu0Cross_E698(d, ((sDelta_E698*)*(char**)(self + 0x4C))[bone].rot)));
    vu0AddEq_E698(body->angVel, rot);
}
#endif

//100%
INCLUDE_ASM("object/animnode", cAnimNode_updateMeshVelocity);
#ifdef SKIP_ASM
struct sVec4_E798 {
    float x, y, z, w;
    sVec4_E798() {}
    sVec4_E798(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sVec3_E798 {
    float x, y, z;
};

struct sRow_E798 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_E798 {
    sRow_E798 r[4];
};

struct sDelta_E798 {
    sVec4_E798 pos;
    sVec4_E798 rot;
};

struct sBone_E798 {
    int parent;
    int f4;
    int active;
    int fC;
};

struct sVE_E798 {
    short delta;
    short index;
    void* fn;
};

extern sVec4_E798 t4vect_Zero;
extern char* cAppMan_TheAppMan;
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void cAnimFrame_getDeriv(void* node, sVec4_E798* pos, sVec4_E798* rot, float t);

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEq_E798(sVec4_E798& v, float s)
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
static inline sVec4_E798 vu0MulMat_E798(const sMat_E798* m, const sVec4_E798& v)
{
    sVec4_E798 r;
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
static inline sVec4_E798 vu0Sub_E798(const sVec4_E798& a, const sVec4_E798& b)
{
    sVec4_E798 r;
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
static inline sVec4_E798 vu0Add_E798(const sVec4_E798& a, const sVec4_E798& b)
{
    sVec4_E798 r;
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
static inline sVec4_E798 vu0Cross_E798(const sVec4_E798& a, const sVec4_E798& b)
{
    sVec4_E798 r;
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
static inline void vu0AddEq_E798(sVec4_E798& a, const sVec4_E798& b)
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

static inline sVec4_E798 boneE798Pos(char* self, int i)
{
    return *(sVec4_E798*)&(*(sMat_E798**)(self + 0x44))[i].r[3];
}

/**
 * Recomputes the per-mesh linear and angular velocities (MeshVelocity) of an animated instance.
 *
 * If the cMoveNode part's RootTransformDirty flag (+0x26 bit 0) is set, virtual slot 51 refreshes the root first.
 * For each mesh of the model's hierarchy (Instance's model +0x80: count +0x4, 0x10-byte entries +0x8) the
 * MeshVelocity entry (+0x4C) is zeroed; meshes with an animation (entry +0x8) take the next AnimFrames entry (+0x48,
 * 0xD0 bytes each) and cAnimFrame_getDeriv gives its position/rotation derivatives at CacheTime (+0x4). Both are scaled by
 * the animation speed (virtual slot 53) times the frame rate (cAppMan_TheAppMan +0x10), the rotation also by
 * degrees-to-radians and an Euler matrix built from the frame's negated angles (+0x50..+0x58), then transformed by the
 * mesh's AnimMats (+0x44) matrix. A mesh with a parent adds the parent's velocity plus (offset x parent angular
 * velocity) and the parent's angular velocity. Clears MeshVelocityDirty (+0x10) at the end.
 */
extern "C" void cAnimNode_updateMeshVelocity(char* self)
{
    char* obj = self + 0x14;
    if (*(unsigned short*)(self + 0x26) & 1) {
        sVE_E798* vt = *(sVE_E798**)(self + 0x20);
        ((void (*)(void*))vt[51].fn)(obj + vt[51].delta);
    }
    char* info = *(char**)(*(char**)(obj + 0x18) + 0x80);
    int k = 0;
    sBone_E798* ent = *(sBone_E798**)(info + 8);
    for (int i = 0; i < *(int*)(info + 4); i++, ent++) {
        ((sDelta_E798*)*(char**)(self + 0x4C))[i].pos = t4vect_Zero;
        ((sDelta_E798*)*(char**)(self + 0x4C))[i].rot = t4vect_Zero;
        float d2r = 0.01745329424738884f;
        if (ent->active == 0) continue;
        char* node = *(char**)(self + 0x48) + k * 208;
        k++;
        sVE_E798* vt = *(sVE_E798**)(self + 0x20);
        float t = ((float (*)(void*))vt[53].fn)(self + vt[53].delta);
        sVec4_E798 pos;
        sVec4_E798 rotv;
        cAnimFrame_getDeriv(node, &pos, &rotv, *(float*)(self + 4));
        sVec4_E798 rot;
        rot = sVec4_E798(-*(float*)(node + 0x50), -*(float*)(node + 0x54), -*(float*)(node + 0x58), 0.0f);
        vu0ScaleEq_E798(pos, t * (float)*(int*)(cAppMan_TheAppMan + 0x10));
        vu0ScaleEq_E798(rotv, t * (float)*(int*)(cAppMan_TheAppMan + 0x10) * d2r);
        sMat_E798 m;
        {
            sVec3_E798 e;
            e.x = rot.x;
            e.y = rot.y;
            e.z = rot.z;
            float s1, c1, s2, c2;
            func_0031BE50(&s1, &c1, e.y);
            func_0031BE50(&s2, &c2, e.z);
            m.r[0].x = c1 * c2;
            m.r[0].y = -c1 * s2;
            m.r[0].z = s1;
            m.r[0].w = 0.0f;
            m.r[1].x = s2;
            m.r[1].y = c2;
            m.r[1].z = 0.0f;
            m.r[1].w = 0.0f;
            m.r[2].x = 0.0f;
            m.r[2].y = 0.0f;
            m.r[2].z = 1.0f;
            m.r[2].w = 0.0f;
            m.r[3].x = 0.0f;
            m.r[3].y = 0.0f;
            m.r[3].z = 0.0f;
            m.r[3].w = 1.0f;
        }
        rotv = vu0MulMat_E798(&m, rotv);
        pos = vu0MulMat_E798(&(*(sMat_E798**)(self + 0x44))[i], pos);
        rotv = vu0MulMat_E798(&(*(sMat_E798**)(self + 0x44))[i], rotv);
        if (ent->parent >= 0) {
            sVec4_E798 d = vu0Sub_E798(boneE798Pos(self, i), boneE798Pos(self, ent->parent));
            vu0AddEq_E798(rotv, ((sDelta_E798*)*(char**)(self + 0x4C))[ent->parent].rot);
            vu0AddEq_E798(pos, vu0Add_E798(((sDelta_E798*)*(char**)(self + 0x4C))[ent->parent].pos,
                                           vu0Cross_E798(d, ((sDelta_E798*)*(char**)(self + 0x4C))[ent->parent].rot)));
        }
        ((sDelta_E798*)*(char**)(self + 0x4C))[i].pos = pos;
        vu0AddEq_E798(((sDelta_E798*)*(char**)(self + 0x4C))[i].rot, rotv);
    }
    *(int*)(self + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("object/animnode", func_0034EBA0);
#ifdef SKIP_ASM
struct sAnimLinkList_EBA0;

struct sAnimLink_EBA0 {
    char pad_0x00[0x18];
    sAnimLinkList_EBA0* list; // 0x18
};

struct sAnimLinkList_EBA0 {
    char pad_0x00[0xC];
    sAnimLink_EBA0* head; // 0x0C
};

extern "C" void func_002D1AC8(sAnimLinkList_EBA0* list);

struct sAnimNode_EBA0 {
    char pad_0x00[0x14];
    sAnimLink_EBA0 link; // 0x14
};

// is this node's link at the head of its list, after func_002D1AC8?
extern "C" int func_0034EBA0(sAnimNode_EBA0* self)
{
    sAnimLink_EBA0* link = &self->link;
    func_002D1AC8(self->link.list);
    return link->list->head == link;
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
extern "C" int cAnimNode_frameUpdateEvent(sRange_EBE0* self, float frame)
{
    int inRange = 0;
    if (self->start != 1e10f) {
        if (self->start < self->end) {
            if (self->start <= frame && frame < self->end) inRange = 1;
        } else {
            if (!(self->end <= frame)) return 0;
            inRange = frame < self->start;
        }
    }
    return inRange;
}
#endif

//100%
INCLUDE_ASM("object/animnode", func_0034EC58);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* func_00355280(void* self, void* a1, int type, void* a2);
struct cFloatingNode;
void cFloatingNode_initInfo(cFloatingNode* self);
extern char str_FloatingMats[];
extern void* cFloatingNode_vtbl[];

struct sAnimFrame_EC58 {
    char data[0x40];
    sAnimFrame_EC58() {}
};

struct sQ_EC58 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sAnimInfo_EC58 {
    int f0;
    float f4, f8, fC, f10, f14, f18;
};

extern "C" void* func_0034EC58(cFloatingNode* self, void* a1, void* a2, sAnimInfo_EC58* info)
{
    func_00355280(self, a1, 0xE, a2);
    self->mVtbl = cFloatingNode_vtbl;
    self->ZSpring = info->f4;
    self->ZDamp = info->f8;
    self->ForceScale = info->f14;
    self->ASpring = info->fC;
    self->ADamp = info->f10;
    self->TorqueScale = info->f18 * 1e-6f;
    sQ_EC58 p = *(sQ_EC58*)((char*)self->Instance + 0x40);
    *(int*)&self->OX = 0;
    *(int*)&self->WX = 0;
    *(int*)&self->TX = 0;
    *(int*)&self->OY = 0;
    *(int*)&self->WY = 0;
    *(int*)&self->TY = 0;
    *(int*)&self->Vel = 0;
    *(int*)&self->Force = 0;
    self->Info = 0;
    self->Zo = p.z;
    self->Z = p.z;
    cFloatingNode_initInfo((cFloatingNode*)self);
    sAnimFrame_EC58** pf = (sAnimFrame_EC58**)(((char*)self) + 0x74);
    *pf = new (str_FloatingMats, 0x20000000, 0) sAnimFrame_EC58[*(int*)(*(char**)((char*)self->Instance + 0x80) + 0x4)];
    self->Flags |= 1;
    char* m = (char*)self->Instance;
    *(unsigned int*)(m + 8) = (*(unsigned int*)(m + 8) & 0xFFFFFFFD) | 4;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/animnode", func_0034ED88);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self, void* a1, void* stream);
struct cFloatingNode;
void cFloatingNode_initInfo(cFloatingNode* self);
extern void* cFloatingNode_vtbl[];
extern char str_FloatingMats[];

struct sStreamVE_ED88 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// Element type with an empty constructor; the array new leaves only its loop counters.
struct sAnimFrame_ED88 {
    char data[0x40];
    sAnimFrame_ED88() {}
};

extern "C" void* func_0034ED88(cFloatingNode* self, void* bucket, void* stream)
{
    int hasInfo;
    cMoveNode_cMoveNode(self, bucket, stream);
    self->Info = 0;
    self->mVtbl = cFloatingNode_vtbl;
    sStreamVE_ED88* vt = *(sStreamVE_ED88**)stream;
    vt[2].fn((char*)stream + vt[2].delta, &hasInfo, 4);
    if (hasInfo != 0) {
        cFloatingNode_initInfo((cFloatingNode*)self);
    }
    sStreamVE_ED88* vt2 = *(sStreamVE_ED88**)stream;
    vt2[2].fn((char*)stream + vt2[2].delta, ((char*)self) + 0x2C, 72);
    sAnimFrame_ED88** pf = (sAnimFrame_ED88**)(((char*)self) + 0x74);
    *pf = new (str_FloatingMats, 0x20000000, 0) sAnimFrame_ED88[*(int*)(*(char**)((char*)self->Instance + 0x80) + 0x4)];
    self->Flags |= 1;
    return ((void*)self);
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
    self->mVtbl = cFloatingNode_vtbl;
    cFloatingNode_restoreOverlapSystem(self);
    operator_delete((int*)self->AnimMats);
    cMoveNode__cMoveNode(self, flags);
}
#endif

