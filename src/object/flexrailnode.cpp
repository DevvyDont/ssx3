#include "vtable.h"
#include "common.h"
#include "ai/rider.h"
#include "world/instancerecord.h"
#include "intersect/bxnash.h"
#include "object/railspacenode.h"
#include "bx/appman.h"
#include "bx/math3d.h"
#include "object/flagset.h"
#include "object/flagnode.h"
#include "object/springrailnode.h"
#include "object/floatrailnode.h"
#include "object/flexrailnode.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("object/flexrailnode", cFlexRailNode_addSpaceHash);
#ifdef SKIP_ASM



extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cObjectInterface_getNash();
extern "C" void nash_Nash_findkey(nash_CellKey* out, void* box);
extern "C" void nash_Octant_addnode(nash_Octant* cell, int type, cFlexRailSpaceNode* node, nash_CellKey* key);
extern const char str_FlexRailSpaceNode[];

/**
 * Registers a flex rail in the Nash rail-space octree through a new space node.
 *
 * Allocates a 0x10-byte cFlexRailSpaceNode ("FlexRailSpaceNode") with type 3 (+0x8) and this node as its
 * owner (+0xC), keeps it in Node (+0x54) and inserts it into octree list 2 under the bound box at +0x30.
 */
extern "C" void cFlexRailNode_addSpaceHash(cFlexRailNode* self)
{
    cFlexRailSpaceNode* n = (cFlexRailSpaceNode*)cMemMan_alloc(0x10, str_FlexRailSpaceNode, 0x20000000, 0);
    (*(void**)&n->FlexNode) = ((void*)self);
    n->type = 3;
    self->Node = (cFlexRailSpaceNode*)n;
    nash_Octant* cell = (nash_Octant*)cObjectInterface_getNash();
    cFlexRailSpaceNode* node = self->Node;
    nash_CellKey key;
    nash_Nash_findkey(&key, (char*)&self->BoundBox);
    nash_Octant* cellX = key.x < 0 ? cell + 4 : cell;
    nash_Octant* cellXY = key.y < 0 ? cellX + 2 : cellX;
    nash_Octant* cellXYZ = key.z < 0 ? cellXY + 1 : cellXY;
    nash_Octant_addnode(cellXYZ, 2, node, &key);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlexRailNode_remSpaceHash);
#ifdef SKIP_ASM
void operator_delete(int*);


extern "C" void nash_Cell_remnode(nash_Cell* oct, int type, cFlexRailSpaceNode* node, nash_CellKey* key, nash_Octant* cell);

static inline int octEmpty_8C48(nash_Cell* o)
{
    if (o->children[0][0][0] != 0 || o->children[0][0][1] != 0 || o->children[0][1][0] != 0 || o->children[0][1][1] != 0
        || o->children[1][0][0] != 0 || o->children[1][0][1] != 0 || o->children[1][1][0] != 0 || o->children[1][1][1] != 0) {
        return 0;
    }
    for (int i = 0; i < 3; i++) {
        if (((void**)&o->nodesinside[0])[i] != 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * Removes a flex rail from the world space hash.
 *
 * Takes the space hash (cObjectInterface_getNash), bumps its +0xA0 counter, finds the hash key of BoundBox (+0x30,
 * nash_Nash_findkey), picks the octant from the key's sign bits and removes the rail's hash node (+0x54) from that
 * cell's list 2 (nash_Cell_remnode). An octant left with no children and no lists is deleted. Finally deletes the hash
 * node and nulls +0x54.
 */
extern "C" void cFlexRailNode_remSpaceHash(cFlexRailNode* self)
{
    nash_Octant* cell = (nash_Octant*)cObjectInterface_getNash();
    cFlexRailSpaceNode* node = self->Node;
    (*(int*)((char*)cell + 0xA0))++;
    nash_CellKey key;
    nash_Nash_findkey(&key, (char*)&self->BoundBox);
    nash_Octant* cellX = key.x < 0 ? cell + 4 : cell;
    nash_Octant* cellXY = key.y < 0 ? cellX + 2 : cellX;
    nash_Octant* cellXYZ = key.z < 0 ? cellXY + 1 : cellXY;
    nash_Octant* octant = cellXYZ;
    nash_Cell_remnode(octant->rootcell, 2, node, &key, octant);
    if (octEmpty_8C48(octant->rootcell)) {
        operator_delete((int*)(octant->rootcell));
        (*(nash_Cell**)&octant->rootcell) = 0;
    }
    operator_delete((int*)self->Node);
    self->Node = 0;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_cFloatRailNode);
#ifdef SKIP_ASM
extern "C" void cFlexRailNode_cFlexRailNode(void* self, int type, void* a1, void* desc);
extern "C" void cFloatRailNode_updateMats(cFloatRailNode* self);
struct sFrBoxRail;
extern "C" void cFloatRailNode_updateBoundBox(cFloatRailNode* self);
extern "C" void cFlexRailNode_addSpaceHash(cFlexRailNode* self);
struct sFrNode9220;
extern "C" void cFloatRailNode_getPosition(cFloatRailNode* self, tFloatRailNode* node);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char cFloatRailNode_vtbl[];
extern char str_FloatRailNodes[];

extern t4Vector t4vect_Zero;

struct sFlexRailRef_348D98 {
    unsigned int id;
    sFlexRailRef_348D98() : id(0xFFFFFFFF) {}
};

struct sFlexRailElem_348D98 {
    t4Vector force;            // 0x00
    t4Vector vel;              // 0x10
    t4Vector posA;             // 0x20
    t4Vector posB;             // 0x30
    sFlexRailRef_348D98 link;   // 0x40
    int bone;                   // 0x44
    char pad48[8];
};

/**
 * Constructs a float rail (node type 20) for a placed rail instance.
 *
 * Runs the flex-rail constructor cFlexRailNode_cFlexRailNode, installs cFloatRailNode_vtbl and copies LSpring, LDamp and ForceScale (desc
 * +0x48..+0x50) to +0x60..+0x68. Allocates NumNodes 0x50-byte nodes ("FloatRailNodes", +0x70); each gets the next valid
 * instance id (desc +0x4) and mesh (desc +0x24), zero Vel and Force, and its Pos and OPos from the mesh position
 * (cFloatRailNode_getPosition). Then builds the segment matrices (cFloatRailNode_updateMats), BoundBox (cFloatRailNode_updateBoundBox) and the space hash entry
 * (cFlexRailNode_addSpaceHash).
 *
 * @param instRecord  rail instance record
 * @param desc  float rail make-data
 * @return self
 */
extern "C" void* cFloatRailNode_cFloatRailNode(cFloatRailNode* self, void* instRecord, tMakeFloatRailData* desc)
{
    cFlexRailNode_cFlexRailNode(((char*)self), 20, instRecord, desc);
    VPTR_OF(cObjNode, self) = (void**)cFloatRailNode_vtbl;
    self->LSpring = desc->LSpring;
    self->LDamp = desc->LDamp;
    self->ForceScale = desc->ForceScale;
    sFlexRailRef_348D98 refs[8];
    int bones[8];
    int n = 0;
    unsigned int* ids = (unsigned int*)(&desc->InstId1);
    int* bn = (int*)(&desc->Mesh1);
    for (int i = 0; i < 8; i++, ids++, bn++) {
        unsigned int id = *ids;
        if (id != 0xFFFFFFFF) {
            refs[n].id = id;
            bones[n] = *bn;
            n++;
        }
    }
    sFlexRailElem_348D98*& elems = (sFlexRailElem_348D98*)self->Nodes;
    elems = new (str_FloatRailNodes, 0x20000000, 0) sFlexRailElem_348D98[self->NumNodes];
    for (int j = 0; j < self->NumNodes; j++) {
        ((sFlexRailElem_348D98*)self->Nodes)[j].link.id = refs[j].id;
        ((sFlexRailElem_348D98*)self->Nodes)[j].bone = bones[j];
        ((sFlexRailElem_348D98*)self->Nodes)[j].vel = t4vect_Zero;
        ((sFlexRailElem_348D98*)self->Nodes)[j].force = t4vect_Zero;
        cFloatRailNode_getPosition(self, (tFloatRailNode*)&((sFlexRailElem_348D98*)self->Nodes)[j]);
        ((sFlexRailElem_348D98*)self->Nodes)[j].posB = ((sFlexRailElem_348D98*)self->Nodes)[j].posA;
    }
    cFloatRailNode_updateMats(self);
    cFloatRailNode_updateBoundBox(self);
    cFlexRailNode_addSpaceHash((cFlexRailNode*)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_cFloatRailNode1);
#ifdef SKIP_ASM
extern "C" void* cFlexRailNode_cFlexRailNode1(void* self, void* a1, void* stream);
extern "C" void cFloatRailNode_updateMats(cFloatRailNode* self);
struct sFrBoxRail;
extern "C" void cFloatRailNode_updateBoundBox(cFloatRailNode* self);
extern "C" void cFlexRailNode_addSpaceHash(cFlexRailNode* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char cFloatRailNode_vtbl[];
extern char str_FloatRailNodes[];

struct sFlexRailRef_348FA0 {
    unsigned int id;
    sFlexRailRef_348FA0() : id(0xFFFFFFFF) {}
};

struct sFlexRailElem_348FA0 {
    char pad[0x40];
    sFlexRailRef_348FA0 link;
    char pad44[0xC];
};


/**
 * Constructs a float rail from a replay frame.
 *
 * Runs cFlexRailNode_cFlexRailNode1, installs cFloatRailNode_vtbl, reads LSpring/LDamp/ForceScale (+0x60, 0xC bytes) and the NumNodes * 0x50
 * node array (allocated as "FloatRailNodes" at +0x70), then rebuilds the matrices (cFloatRailNode_updateMats), BoundBox
 * (cFloatRailNode_updateBoundBox) and space hash entry (cFlexRailNode_addSpaceHash).
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cFloatRailNode_cFloatRailNode1(cFloatRailNode* self, void* bucket, cBxReplayFrame* stream)
{
    cFlexRailNode_cFlexRailNode1(((char*)self), bucket, stream);
    VPTR_OF(cObjNode, self) = (void**)cFloatRailNode_vtbl;
    stream->read((char*)&self->LSpring, 0xC);
    sFlexRailElem_348FA0** elems = (sFlexRailElem_348FA0**)(&self->Nodes);
    *elems = new (str_FloatRailNodes, 0x20000000, 0) sFlexRailElem_348FA0[self->NumNodes];
    stream->read((char*)self->Nodes, self->NumNodes * 80);
    cFloatRailNode_updateMats(self);
    cFloatRailNode_updateBoundBox(self);
    cFlexRailNode_addSpaceHash((cFlexRailNode*)self);
    return ((char*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", cFloatRailNode__cFloatRailNode);
#ifdef SKIP_ASM
extern char cFloatRailNode_vtbl[];
void cMemMan_free(void*);
extern "C" void cFlexRailNode_remSpaceHash(cFlexRailNode* self);
extern "C" void cFlexRailNode__cFlexRailNode(void* self, int flags);

/**
 * Destroys a float rail.
 *
 * Reinstalls cFloatRailNode_vtbl, removes the rail from the space hash (cFlexRailNode_remSpaceHash), frees the node array (+0x70) and runs
 * the flex-rail destructor cFlexRailNode__cFlexRailNode with flags.
 *
 * @param flags  destructor flags
 */
extern "C" void cFloatRailNode__cFloatRailNode(cFloatRailNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = (void**)cFloatRailNode_vtbl;
    cFlexRailNode_remSpaceHash((cFlexRailNode*)self);
    void* railNodes = self->Nodes;
    if (railNodes != 0) {
        cMemMan_free(railNodes);
    }
    cFlexRailNode__cFlexRailNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateBoundBox);
#ifdef SKIP_ASM

struct sFrBoxElem {
    char pad_0x00[0x20];
    t4Vector pos; // 0x20
    char pad_0x30[0x20];
};

struct sFrBoxRail {
    char pad_0x00[0x20];
    int count;          // 0x20
    char pad_0x24[0xC];
    t4Vector min;     // 0x30
    t4Vector max;     // 0x40
    char pad_0x50[0x20];
    sFrBoxElem* elems;  // 0x70
};

// PORT: PS2-only VU0 inline asm (vector sub-assign).
static inline void vu0SubFRBox(t4Vector& dst, t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddFRBox(t4Vector& dst, t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Recomputes a float rail's BoundBox from its node positions, padded by 200 on each axis.
 */
// bounding box of the rail elements, padded by 200 on x/y/z
extern "C" void cFloatRailNode_updateBoundBox(cFloatRailNode* self)
{
    sFrBoxElem* points = (sFrBoxElem*)self->Nodes;
    int n = self->NumNodes;
    *(t4Vector*)&self->BoundBox = *(t4Vector*)&self->BoundBox.MaxPoint = points[0].pos;
    for (int i = 1; i < n; i++) {
        t4Vector* point = &points[i].pos;
        if (point->x < self->BoundBox.MinPoint.x) {
            self->BoundBox.MinPoint.x = point->x;
        }
        if (point->y < self->BoundBox.MinPoint.y) {
            self->BoundBox.MinPoint.y = point->y;
        }
        if (point->z < self->BoundBox.MinPoint.z) {
            self->BoundBox.MinPoint.z = point->z;
        }
        if (point->x > self->BoundBox.MaxPoint.x) {
            self->BoundBox.MaxPoint.x = point->x;
        }
        if (point->y > self->BoundBox.MaxPoint.y) {
            self->BoundBox.MaxPoint.y = point->y;
        }
        if (point->z > self->BoundBox.MaxPoint.z) {
            self->BoundBox.MaxPoint.z = point->z;
        }
    }
    t4Vector margin;
    margin.x = 200.0f;
    margin.y = 200.0f;
    margin.z = 200.0f;
    margin.w = 0.0f;
    vu0SubFRBox(*(t4Vector*)&self->BoundBox, margin);
    vu0AddFRBox(*(t4Vector*)&self->BoundBox.MaxPoint, margin);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_getPosition);
#ifdef SKIP_ASM

struct sFrNodeMtx {
    t4Vector r[4];
};

struct sFrModelSet9220 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C, (model >> 2) << 8 | low byte
};

struct sFrWorld9220 {
    char pad_0x0[0x8];
    sFrModelSet9220** sets; // 0x8
};

extern "C" sFrWorld9220** cObjectInterface_getWorld();
extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, sFrNodeMtx* out);

static inline void* refToPtr9220(unsigned int p)
{
    return (void*)(p << 2);
}

struct sFrModelRef9220 {
    unsigned int id;

    void* get()
    {
        sFrModelSet9220* set = (*cObjectInterface_getWorld())->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        unsigned int p = set->refs[id >> 8] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr9220(p);
    }
};


/**
 * Sets a float rail node's Pos and OPos to the current position of its mesh.
 *
 * Builds the mesh matrix of the node's instance and mesh (cInstanceNode_buildMeshMat) and copies its translation.
 *
 * @param node  rail node to update
 */
extern "C" void cFloatRailNode_getPosition(cFloatRailNode* self, tFloatRailNode* node)
{
    sFrNodeMtx boneMat;
    cInstanceNode_buildMeshMat((*(sFrModelRef9220*)&node->InstId).get(), node->Mesh, &boneMat);
    t4Vector bonePos = boneMat.r[3];
    (*(t4Vector*)&node->OPos) = bonePos;
    (*(t4Vector*)&node->Pos) = bonePos;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateMats);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sFrV4_92A8 {
    float x, y, z, w;
    const float& operator[](int i) const { return (&x)[i]; }
} __attribute__((aligned(16)));




extern float CatmullRom[16];

// PORT: PS2-only VU0 inline asm (vector * 4x4 basis matrix, row-vector convention).
static inline void vu0BasisMul_92A8(sFrV4_92A8& dst, const sFrV4_92A8& g, const float* b)
{
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(dst)
        : "m"(g), "r"(b)
        : "memory");
}

typedef int tMmiQuad __attribute__((mode(TI)));

// PORT: hand-written MMI 4x4 transpose in place (lq/pextlw/pextuw/pcpyld/pcpyud/sq: no gcc 2.95 pattern); a port
// swaps m[i][j] and m[j][i] in plain C.
#define MMI_TRANSPOSE4(m)                                                                                              \
    {                                                                                                                  \
        tMmiQuad r0_, r1_, r2_, r3_, t0_, t1_, t2_, t3_;                                                               \
        __asm__ __volatile__(                                                                                          \
            "lq      %0, 0x0(%8)\n"                                                                                    \
            "lq      %1, 0x10(%8)\n"                                                                                   \
            "lq      %2, 0x20(%8)\n"                                                                                   \
            "lq      %3, 0x30(%8)\n"                                                                                   \
            "pextlw  %4, %1, %0\n"                                                                                     \
            "pextuw  %5, %1, %0\n"                                                                                     \
            "pextlw  %6, %3, %2\n"                                                                                     \
            "pextuw  %7, %3, %2\n"                                                                                     \
            "pcpyld  %0, %6, %4\n"                                                                                     \
            "pcpyud  %1, %4, %6\n"                                                                                     \
            "pcpyld  %2, %7, %5\n"                                                                                     \
            "pcpyud  %3, %5, %7\n"                                                                                     \
            "sq      %0, 0x0(%8)\n"                                                                                    \
            "sq      %1, 0x10(%8)\n"                                                                                   \
            "sq      %2, 0x20(%8)\n"                                                                                   \
            "sq      %3, 0x30(%8)\n"                                                                                   \
            : "=&r"(r0_), "=&r"(r1_), "=&r"(r2_), "=&r"(r3_), "=&r"(t0_), "=&r"(t1_), "=&r"(t2_), "=&r"(t3_)          \
            : "r"(m)                                                                                                   \
            : "memory");                                                                                               \
    }

static inline void set4_92A8(sFrV4_92A8& r, float x, float y, float z, float w)
{
    r.x = x; r.y = y; r.z = z; r.w = w;
}

// Spline basis matrix for control points a, b, c, d.
static inline void updateMat_92A8(t4Matrix& out, const sFrV4_92A8& a, const sFrV4_92A8& b, const sFrV4_92A8& c, const sFrV4_92A8& d)
{
    t4Matrix L;
    sFrV4_92A8 v;
    set4_92A8(((sFrV4_92A8*)&L.row[0])[1], a[0], b[0], c[0], d[0]);
    vu0BasisMul_92A8(((sFrV4_92A8*)&L.row[0])[2], ((sFrV4_92A8*)&L.row[0])[1], CatmullRom);
    *(sFrV4_92A8*)&L.row[0].x = ((sFrV4_92A8*)&L.row[0])[2];
    set4_92A8(((sFrV4_92A8*)&L.row[0])[2], a[1], b[1], c[1], d[1]);
    vu0BasisMul_92A8(((sFrV4_92A8*)&L.row[0])[3], ((sFrV4_92A8*)&L.row[0])[2], CatmullRom);
    ((sFrV4_92A8*)&L.row[0])[1] = ((sFrV4_92A8*)&L.row[0])[3];
    set4_92A8(((sFrV4_92A8*)&L.row[0])[3], a[2], b[2], c[2], d[2]);
    vu0BasisMul_92A8(v, ((sFrV4_92A8*)&L.row[0])[3], CatmullRom);
    ((sFrV4_92A8*)&L.row[0])[2] = v;
    *(sFrV4_92A8*)&out.row[0].x = *(sFrV4_92A8*)&L.row[0].x; ((sFrV4_92A8*)&out.row[0])[1] = ((sFrV4_92A8*)&L.row[0])[1]; ((sFrV4_92A8*)&out.row[0])[2] = ((sFrV4_92A8*)&L.row[0])[2];
    ((sFrV4_92A8*)&L.row[0])[3].x = 0.0f; ((sFrV4_92A8*)&L.row[0])[3].y = 0.0f; ((sFrV4_92A8*)&L.row[0])[3].z = 0.0f; ((sFrV4_92A8*)&L.row[0])[3].w = 1.0f;
    ((sFrV4_92A8*)&out.row[0])[3] = ((sFrV4_92A8*)&L.row[0])[3];
    MMI_TRANSPOSE4(&out);
}

/**
 * Rebuilds a float rail's segment matrices from its node positions.
 *
 * Each segment matrix (+0x50) is the cubic for that segment built from the four surrounding node positions with the
 * basis CatmullRom; the first and last segments repeat the end node as the missing control point.
 *
 * @param selfv  the float rail
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm; a port replaces them with a
// plain C transpose and a C vector-matrix product.
extern "C" void cFloatRailNode_updateMats(cFloatRailNode* selfv)
{
    cFloatRailNode* self = selfv;
    int i = 1;
    updateMat_92A8((self->Mats)[0], (*(sFrV4_92A8*)&(self->Nodes)[0].Pos), (*(sFrV4_92A8*)&(self->Nodes)[0].Pos), (*(sFrV4_92A8*)&(self->Nodes)[1].Pos), (*(sFrV4_92A8*)&(self->Nodes)[2].Pos));
    for (; i < self->NumNodes - 2; i++) {
        updateMat_92A8((self->Mats)[i], (*(sFrV4_92A8*)&(self->Nodes)[i - 1].Pos), (*(sFrV4_92A8*)&self->Nodes[i].Pos), (*(sFrV4_92A8*)&(self->Nodes)[i + 1].Pos), (*(sFrV4_92A8*)&(self->Nodes)[i + 2].Pos));
    }
    updateMat_92A8((self->Mats)[i], (*(sFrV4_92A8*)&(self->Nodes)[i - 1].Pos), (*(sFrV4_92A8*)&self->Nodes[i].Pos), (*(sFrV4_92A8*)&(self->Nodes)[i + 1].Pos), (*(sFrV4_92A8*)&(self->Nodes)[i + 1].Pos));
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_applyForce);
#ifdef SKIP_ASM

struct sFrElem {
    t4Vector pos;   // 0x00
    t4Vector vel;   // 0x10
    char pad[0x30];
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0ScaleFR(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline void vu0AddFR(t4Vector& dst, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Adds a force at a hit point to the two float rail nodes around it.
 *
 * Does nothing when the hit segment (intInfo +0x64) is negative. Otherwise node seg gets offset * (1 - t) and node seg
 * + 1 gets offset * t added to its Force (t at +0x68).
 *
 * @param node  intersect info of the hit (segment +0x64, t +0x68)
 * @param offset  force to apply
 */
extern "C" void cFloatRailNode_applyForce(cFloatRailNode* self, cFloatRailNode* node, t4Vector* offset)
{
    if (*(int*)&node->LDamp >= 0) {
        vu0AddFR(((sFrElem*)self->Nodes)[*(int*)&node->LDamp].pos,
                 vu0ScaleFR(*offset, 1.0f - node->ForceScale));
        vu0AddFR(((sFrElem*)self->Nodes)[*(int*)&node->LDamp + 1].pos,
                 vu0ScaleFR(*offset, node->ForceScale));
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateIntersectInfo);
#ifdef SKIP_ASM
/**
 * Adds the float rail's velocity at the hit point to an intersect info.
 *
 * Does nothing when the hit segment (+0x64) is negative. Otherwise adds Vel[seg] * t + Vel[seg + 1] * (1 - t) to the
 * info's +0x20 vector (t at +0x68).
 *
 * Likely bug: the weights are the reverse of cFloatRailNode_applyForce, which gives node seg the (1 - t) share.
 *
 * @param node  intersect info of the hit
 */
// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (vector add-assign).
extern "C" void cFloatRailNode_updateIntersectInfo(cFloatRailNode* self, cFloatRailNode* node)
{
    if (*(int*)&node->LDamp >= 0) {
        vu0AddFR(*(t4Vector*)&node->NumNodes,
                 vu0ScaleFR(((sFrElem*)self->Nodes)[*(int*)&node->LDamp].vel,
                            node->ForceScale));
        vu0AddFR(*(t4Vector*)&node->NumNodes,
                 vu0ScaleFR(((sFrElem*)self->Nodes)[*(int*)&node->LDamp + 1].vel,
                            1.0f - node->ForceScale));
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateNodes);
#ifdef SKIP_ASM


extern "C" float cObjectInterface_getUpdateTime();
extern "C" void cFloatRailNode_getPosition(cFloatRailNode* self, tFloatRailNode* node);
// View (cFloatRailNode_cFloatRailNode earlier in this unit declares it with its own type).
extern t4Vector t4vect_Zero_98E8 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_98E8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_98E8(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_98E8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_98E8(t4Vector& dst, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Steps the float rail's spring simulation by one update.
 *
 * For each node: snaps Pos and OPos back to the mesh (cFloatRailNode_getPosition) while the instance has an active node (+0xC);
 * accel = -(Pos - OPos) * LSpring (+0x60) + Force * ForceScale (+0x68) - Vel * LDamp (+0x64); Pos += Vel * dt; Vel +=
 * accel * dt (dt from cObjectInterface_getUpdateTime); Force is cleared.
 */
extern "C" void cFloatRailNode_updateNodes(cFloatRailNode* self)
{
    float dt = cObjectInterface_getUpdateTime();
    tFloatRailNode* n = self->Nodes;
    for (int i = 0; i < self->NumNodes; i++, n++) {
        if (*(int*)((char*)(*(sFrModelRef9220*)&n->InstId).get() + 0xC))
            cFloatRailNode_getPosition(self, (tFloatRailNode*)n);
        float* k = (float*)(&self->LSpring);
        float ks = -self->LSpring;
        t4Vector acc = vu0Add_98E8(vu0Scale_98E8(vu0Sub_98E8((*(&n->Pos)), (*(&n->OPos))), ks),
                                    vu0Scale_98E8((*(&n->Force)), k[2]));
        vu0AddEq_98E8(acc, vu0Scale_98E8((*(&n->Vel)), -self->LDamp));
        vu0AddEq_98E8((*(&n->Pos)), vu0Scale_98E8((*(&n->Vel)), dt));
        vu0AddEq_98E8((*(&n->Vel)), vu0Scale_98E8(acc, dt));
        (*(&n->Force)) = t4vect_Zero_98E8;
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFlexRailVEntry9AD0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cFlexRailNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a float rail to a replay frame.
 *
 * Writes the flex-rail base (cFlexRailNode_writeToReplayFrame), LSpring/LDamp/ForceScale (+0x60, 0xC bytes) and the NumNodes * 0x50 node
 * array (+0x70).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cFloatRailNode_writeToReplayFrame(cFloatRailNode* self, cBxReplayFrame* stream)
{
    cFlexRailNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->LSpring, 0xC);
    stream->write((const char*)self->Nodes, self->NumNodes * 80);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_cSpringRailNode);
#ifdef SKIP_ASM
extern "C" void cFlexRailNode_cFlexRailNode(void* self, int type, void* a1, void* desc);
extern "C" void cSpringRailNode_updateMats(cSpringRailNode* self);
struct sFrBoxRail60;
extern "C" void cSpringRailNode_updateBoundBox(cSpringRailNode* self);
struct sFrNodeA028;
extern "C" void cSpringRailNode_getPosition(cSpringRailNode* self, tSpringRailNode* node);
extern char cSpringRailNode_vtbl[];
extern char str_SpringRailNodes[];

extern t4Vector t4vect_Zero_9B48 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_9B48(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

struct sFlexRef_9B48 {
    unsigned int id;
    sFlexRef_9B48() : id(0xFFFFFFFF) {}
};

struct sFlexElem_9B48 {
    t4Vector pos;              // 0x00
    t4Vector dir;              // 0x10
    t4Vector v20;              // 0x20
    t4Vector v30;              // 0x30
    t4Vector v40;              // 0x40
    sFlexRef_9B48 link;         // 0x50
    int bone;                   // 0x54
    char pad58[8];
};

/**
 * Constructs a spring rail (node type 21) for a placed rail instance.
 *
 * Runs the flex-rail constructor cFlexRailNode_cFlexRailNode, installs cSpringRailNode_vtbl and copies ASpring, ADamp and TorqueScale (desc
 * +0x48..+0x50) to +0x60..+0x68. Allocates NumNodes 0x60-byte nodes ("SpringRailNodes", +0x70); each gets the next
 * valid instance id and mesh, zero V/O/W/T and its P from the mesh position (cSpringRailNode_getPosition). V of each node becomes the
 * vector to the next node's P. Then builds the segment matrices (cSpringRailNode_updateMats), BoundBox (cSpringRailNode_updateBoundBox) and the space
 * hash entry (cFlexRailNode_addSpaceHash).
 *
 * @param instRecord  rail instance record
 * @param desc  spring rail make-data
 * @return self
 */
extern "C" void* cSpringRailNode_cSpringRailNode(cSpringRailNode* self, void* instRecord, tMakeSpringRailData* desc)
{
    cFlexRailNode_cFlexRailNode(((char*)self), 21, instRecord, desc);
    VPTR_OF(cObjNode, self) = (void**)cSpringRailNode_vtbl;
    self->ASpring = desc->ASpring;
    self->ADamp = desc->ADamp;
    self->TorqueScale = desc->TorqueScale;
    sFlexRef_9B48 refs[8];
    int bones[8];
    int n = 0;
    unsigned int* ids = (unsigned int*)(&desc->InstId1);
    int* bn = (int*)(&desc->Mesh1);
    for (int i = 0; i < 8; i++, ids++, bn++) {
        unsigned int id = *ids;
        if (id != 0xFFFFFFFF) {
            refs[n].id = id;
            bones[n] = *bn;
            n++;
        }
    }
    sFlexElem_9B48*& elems = (sFlexElem_9B48*)self->Nodes;
    elems = new (str_SpringRailNodes, 0x20000000, 0) sFlexElem_9B48[self->NumNodes];
    for (int j = 0; j < self->NumNodes; j++) {
        ((sFlexElem_9B48*)self->Nodes)[j].link.id = refs[j].id;
        ((sFlexElem_9B48*)self->Nodes)[j].bone = bones[j];
        ((sFlexElem_9B48*)self->Nodes)[j].v30 = t4vect_Zero_9B48;
        ((sFlexElem_9B48*)self->Nodes)[j].v40 = t4vect_Zero_9B48;
        ((sFlexElem_9B48*)self->Nodes)[j].v20 = t4vect_Zero_9B48;
        ((sFlexElem_9B48*)self->Nodes)[j].dir = t4vect_Zero_9B48;
        cSpringRailNode_getPosition(self, (tSpringRailNode*)&((sFlexElem_9B48*)self->Nodes)[j]);
    }
    for (int k = 0; k < self->NumNodes - 1; k++) {
        ((sFlexElem_9B48*)self->Nodes)[k].dir =
            Sub_9B48(((sFlexElem_9B48*)self->Nodes)[k + 1].pos, ((sFlexElem_9B48*)self->Nodes)[k].pos);
    }
    cSpringRailNode_updateMats(self);
    cSpringRailNode_updateBoundBox(self);
    cFlexRailNode_addSpaceHash((cFlexRailNode*)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_cSpringRailNode1);
#ifdef SKIP_ASM
extern "C" void* cFlexRailNode_cFlexRailNode1(void* self, void* a1, void* stream);
extern "C" void cSpringRailNode_updateMats(cSpringRailNode* self);
extern "C" void cFlexRailNode_addSpaceHash(cFlexRailNode* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char cSpringRailNode_vtbl[];
extern char str_SpringRailNodes[];

struct sFlexRailElem_349DB0 {
    char pad[0x50];
    unsigned int link;
    char pad54[0xC];
    sFlexRailElem_349DB0() : link(~0u) {}
};


/**
 * Constructs a spring rail from a replay frame.
 *
 * Runs cFlexRailNode_cFlexRailNode1, installs cSpringRailNode_vtbl, reads ASpring/ADamp/TorqueScale (+0x60, 0xC bytes) and the NumNodes * 0x60
 * node array ("SpringRailNodes", +0x70), then rebuilds the matrices (cSpringRailNode_updateMats) and the space hash entry. Unlike
 * the float rail's replay ctor it does not recompute BoundBox (it comes back from the replay state).
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cSpringRailNode_cSpringRailNode1(cSpringRailNode* self, void* bucket, cBxReplayFrame* stream)
{
    cFlexRailNode_cFlexRailNode1(((char*)self), bucket, stream);
    VPTR_OF(cObjNode, self) = (void**)cSpringRailNode_vtbl;
    stream->read((char*)&self->ASpring, 0xC);
    sFlexRailElem_349DB0** elems = (sFlexRailElem_349DB0**)(&self->Nodes);
    *elems = new (str_SpringRailNodes, 0x20000000, 0) sFlexRailElem_349DB0[self->NumNodes];
    stream->read((char*)self->Nodes, self->NumNodes * 0x60);
    cSpringRailNode_updateMats(self);
    cFlexRailNode_addSpaceHash((cFlexRailNode*)self);
    return ((char*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", cSpringRailNode__cSpringRailNode);
#ifdef SKIP_ASM
extern char cSpringRailNode_vtbl[];
void cMemMan_free(void*);
extern "C" void cFlexRailNode_remSpaceHash(cFlexRailNode* self);
extern "C" void cFlexRailNode__cFlexRailNode(void* self, int flags);

/**
 * Destroys a spring rail.
 *
 * Reinstalls cSpringRailNode_vtbl, removes the rail from the space hash (cFlexRailNode_remSpaceHash), frees the node array (+0x70) and runs
 * cFlexRailNode__cFlexRailNode with flags.
 *
 * @param flags  destructor flags
 */
extern "C" void cSpringRailNode__cSpringRailNode(cSpringRailNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = (void**)cSpringRailNode_vtbl;
    cFlexRailNode_remSpaceHash((cFlexRailNode*)self);
    void* railNodes = self->Nodes;
    if (railNodes != 0) {
        cMemMan_free(railNodes);
    }
    cFlexRailNode__cFlexRailNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_updateBoundBox);
#ifdef SKIP_ASM
struct sFrBoxElem60 {
    t4Vector pos; // 0x00
    char pad_0x10[0x50];
};

struct sFrBoxRail60 {
    char pad_0x00[0x20];
    int count;           // 0x20
    char pad_0x24[0xC];
    t4Vector min;      // 0x30
    t4Vector max;      // 0x40
    char pad_0x50[0x20];
    sFrBoxElem60* elems; // 0x70
};

/**
 * Recomputes a spring rail's BoundBox from its node positions, padded by 200 on each axis.
 */
// PORT: PS2-only VU0 inline asm (vector sub-assign).
// PORT: PS2-only VU0 inline asm (vector add-assign).
// bounding box of the rail elements, padded by 200 on x/y/z
extern "C" void cSpringRailNode_updateBoundBox(cSpringRailNode* self)
{
    sFrBoxElem60* points = (sFrBoxElem60*)self->Nodes;
    int n = self->NumNodes;
    *(t4Vector*)&self->BoundBox = *(t4Vector*)&self->BoundBox.MaxPoint = points[0].pos;
    for (int i = 1; i < n; i++) {
        t4Vector* point = &points[i].pos;
        if (point->x < self->BoundBox.MinPoint.x) {
            self->BoundBox.MinPoint.x = point->x;
        }
        if (point->y < self->BoundBox.MinPoint.y) {
            self->BoundBox.MinPoint.y = point->y;
        }
        if (point->z < self->BoundBox.MinPoint.z) {
            self->BoundBox.MinPoint.z = point->z;
        }
        if (point->x > self->BoundBox.MaxPoint.x) {
            self->BoundBox.MaxPoint.x = point->x;
        }
        if (point->y > self->BoundBox.MaxPoint.y) {
            self->BoundBox.MaxPoint.y = point->y;
        }
        if (point->z > self->BoundBox.MaxPoint.z) {
            self->BoundBox.MaxPoint.z = point->z;
        }
    }
    t4Vector margin;
    margin.x = 200.0f;
    margin.y = 200.0f;
    margin.z = 200.0f;
    margin.w = 0.0f;
    vu0SubFRBox(*(t4Vector*)&self->BoundBox, margin);
    vu0AddFRBox(*(t4Vector*)&self->BoundBox.MaxPoint, margin);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_getPosition);
#ifdef SKIP_ASM
extern "C" sFrWorld9220** cObjectInterface_getWorld();
extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, sFrNodeMtx* out);


/**
 * Sets a spring rail node's P to the current position of its mesh (cInstanceNode_buildMeshMat translation).
 *
 * @param node  rail node to update
 */
extern "C" void cSpringRailNode_getPosition(cSpringRailNode* self, tSpringRailNode* node)
{
    sFrNodeMtx boneMat;
    cInstanceNode_buildMeshMat((*(sFrModelRef9220*)&node->InstId).get(), node->Mesh, &boneMat);
    t4Vector bonePos = boneMat.r[3];
    (*(t4Vector*)&node->P) = bonePos;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_updateMats);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).


extern float CatmullRom[16];

// PORT: PS2-only VU0 inline asm (vector * 4x4 basis matrix, row-vector convention).
static inline void vu0BasisMul_A0B0(t4Vector& dst, const t4Vector& g, const float* b)
{
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(dst)
        : "m"(g), "r"(b)
        : "memory");
}

typedef int tMmiQuad __attribute__((mode(TI)));

// PORT: hand-written MMI 4x4 transpose in place (lq/pextlw/pextuw/pcpyld/pcpyud/sq: no gcc 2.95 pattern); a port
// swaps m[i][j] and m[j][i] in plain C.
#define MMI_TRANSPOSE4(m)                                                                                              \
    {                                                                                                                  \
        tMmiQuad r0_, r1_, r2_, r3_, t0_, t1_, t2_, t3_;                                                               \
        __asm__ __volatile__(                                                                                          \
            "lq      %0, 0x0(%8)\n"                                                                                    \
            "lq      %1, 0x10(%8)\n"                                                                                   \
            "lq      %2, 0x20(%8)\n"                                                                                   \
            "lq      %3, 0x30(%8)\n"                                                                                   \
            "pextlw  %4, %1, %0\n"                                                                                     \
            "pextuw  %5, %1, %0\n"                                                                                     \
            "pextlw  %6, %3, %2\n"                                                                                     \
            "pextuw  %7, %3, %2\n"                                                                                     \
            "pcpyld  %0, %6, %4\n"                                                                                     \
            "pcpyud  %1, %4, %6\n"                                                                                     \
            "pcpyld  %2, %7, %5\n"                                                                                     \
            "pcpyud  %3, %5, %7\n"                                                                                     \
            "sq      %0, 0x0(%8)\n"                                                                                    \
            "sq      %1, 0x10(%8)\n"                                                                                   \
            "sq      %2, 0x20(%8)\n"                                                                                   \
            "sq      %3, 0x30(%8)\n"                                                                                   \
            : "=&r"(r0_), "=&r"(r1_), "=&r"(r2_), "=&r"(r3_), "=&r"(t0_), "=&r"(t1_), "=&r"(t2_), "=&r"(t3_)          \
            : "r"(m)                                                                                                   \
            : "memory");                                                                                               \
    }

static inline void set4_A0B0(t4Vector& r, float x, float y, float z, float w)
{
    r.x = x; r.y = y; r.z = z; r.w = w;
}

// Spline basis matrix for the segment from e[0] to e[1]: control points a, e[0], e[1], e[2].
static inline void updateMatSeg_A0B0(t4Matrix& out, const t4Vector& a, const tSpringRailNode* e)
{
    t4Matrix L;
    t4Vector v;
    set4_A0B0(L.row[1], a.x, e[0].P.x, e[1].P.x, e[2].P.x);
    vu0BasisMul_A0B0(L.row[2], L.row[1], CatmullRom);
    L.row[0] = L.row[2];
    set4_A0B0(L.row[2], a.y, e[0].P.y, e[1].P.y, e[2].P.y);
    vu0BasisMul_A0B0(L.row[3], L.row[2], CatmullRom);
    L.row[1] = L.row[3];
    set4_A0B0(L.row[3], a.z, e[0].P.z, e[1].P.z, e[2].P.z);
    vu0BasisMul_A0B0(v, L.row[3], CatmullRom);
    L.row[2] = v;
    out.row[0] = L.row[0]; out.row[1] = L.row[1]; out.row[2] = L.row[2];
    L.row[3].x = 0.0f; L.row[3].y = 0.0f; L.row[3].z = 0.0f; L.row[3].w = 1.0f;
    out.row[3] = L.row[3];
    MMI_TRANSPOSE4(&out);
}

// Last segment: the end point e[1] is repeated as the fourth control point.
static inline void updateMatEnd_A0B0(t4Matrix& out, const t4Vector& a, const tSpringRailNode* e)
{
    t4Matrix L;
    t4Vector v;
    set4_A0B0(L.row[1], a.x, e[0].P.x, e[1].P.x, e[1].P.x);
    vu0BasisMul_A0B0(L.row[2], L.row[1], CatmullRom);
    L.row[0] = L.row[2];
    set4_A0B0(L.row[2], a.y, e[0].P.y, e[1].P.y, e[1].P.y);
    vu0BasisMul_A0B0(L.row[3], L.row[2], CatmullRom);
    L.row[1] = L.row[3];
    set4_A0B0(L.row[3], a.z, e[0].P.z, e[1].P.z, e[1].P.z);
    vu0BasisMul_A0B0(v, L.row[3], CatmullRom);
    L.row[2] = v;
    out.row[0] = L.row[0]; out.row[1] = L.row[1]; out.row[2] = L.row[2];
    L.row[3].x = 0.0f; L.row[3].y = 0.0f; L.row[3].z = 0.0f; L.row[3].w = 1.0f;
    out.row[3] = L.row[3];
    MMI_TRANSPOSE4(&out);
}

/**
 * Rebuilds a spring rail's segment matrices from its node positions.
 *
 * Same scheme as cFloatRailNode_updateMats: each segment cubic comes from the surrounding four P values with basis CatmullRom,
 * repeating the end node for the first and last segments.
 *
 * @param selfv  the spring rail
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm; a port replaces them with a
// plain C transpose and a C vector-matrix product.
extern "C" void cSpringRailNode_updateMats(cSpringRailNode* selfv)
{
    cSpringRailNode* self = selfv;
    int i = 1;
    updateMatSeg_A0B0(self->Mats[0], self->Nodes[0].P, &self->Nodes[0]);
    for (; i < self->NumNodes - 2; i++) {
        updateMatSeg_A0B0(self->Mats[i], self->Nodes[i - 1].P, &self->Nodes[i]);
    }
    updateMatEnd_A0B0(self->Mats[i], self->Nodes[i - 1].P, &self->Nodes[i]);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_applyForce);
#ifdef SKIP_ASM




// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Len_A568(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_A568(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar, in place).
static inline void vu0ScaleEq_A568(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_A568(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product).
static inline t4Vector vu0Cross_A568(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_A568(t4Vector& dst, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

/**
 * Turns a force at a hit point into torque on the spring rail's segments up to the hit.
 *
 * Clamps dir to length 200, then does nothing when the hit segment (+0x64) is negative. torque = clamped dir cross (hit
 * point - P[0]); each segment before the hit gets torque * |V[i]| / |arm| * TorqueScale (+0x68) added to T, the hit
 * segment gets torque * |hit - P[seg]| / |arm| * TorqueScale.
 *
 * Quirk: each loop pass also rescales a copy of the arm to |V[i]| and never uses it.
 *
 * @param node  intersect info of the hit (position, segment +0x64)
 * @param dir  force to apply
 */
extern "C" void cSpringRailNode_applyForce(cSpringRailNode* self, tIntersectInfo* node, t4Vector* dir)
{
    float len = vu0Len_A568(*dir);
    t4Vector clamped;
    if (200.0f < len) {
        clamped = vu0Scale_A568(*dir, 200.0f / len);
    } else {
        clamped = *dir;
    }
    if (node->Seg < 0) {
        return;
    }
    t4Vector arm = vu0Sub_A568(node->Ipos, (self->Nodes)[0].P);
    t4Vector torque = vu0Cross_A568(clamped, arm);
    float alen = vu0Len_A568(arm);
    int i;
    for (i = 0; i < node->Seg; i++) {
        float* k = ((float*)&self->ASpring);
        t4Vector scaled = arm;
        float lv = vu0Len_A568(self->Nodes[i].V);
        vu0ScaleEq_A568(scaled, lv / vu0Len_A568(scaled));
        vu0AddEq_A568(self->Nodes[i].T, vu0Scale_A568(vu0Scale_A568(torque, vu0Len_A568(self->Nodes[i].V) / alen), k[2]));
    }
    float* k = ((float*)&self->ASpring);
    vu0AddEq_A568((self->Nodes)[node->Seg].T, vu0Scale_A568(vu0Scale_A568(torque, vu0Len_A568(vu0Sub_A568(node->Ipos, (self->Nodes)[node->Seg].P)) / alen), k[2]));
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_updateIntersectInfo);
#ifdef SKIP_ASM




// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_A838(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product into dst, w = 0).
static inline void vu0CrossTo_A838(t4Vector& dst, const t4Vector& a, const t4Vector& b)
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
    dst = r;
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_A838(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

/**
 * Adds the spring rail's rotational motion at the hit point to an intersect info.
 *
 * For each node from 0 to the hit segment (+0x64; none when negative) adds (hit point - P[i]) cross W[i] to the info's
 * +0x20 vector.
 *
 * @param node  intersect info of the hit
 */
extern "C" void cSpringRailNode_updateIntersectInfo(cSpringRailNode* self, tIntersectInfo* node)
{
    if (node->Seg < 0)
        return;
    for (int i = 0; i <= node->Seg; i++) {
        t4Vector cross;
        t4Vector arm = vu0Sub_A838(node->Ipos, self->Nodes[i].P);
        vu0CrossTo_A838(cross, arm, self->Nodes[i].W);
        vu0AddEq_A838(node->Ivel, cross);
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_updateNodes);
#ifdef SKIP_ASM
extern "C" float cObjectInterface_getUpdateTime();


struct sV4Slot_A8C8 {
    t4Vector v;
    sV4Slot_A8C8() {}
};

// PORT: the unit declares t4vect_Zero with another vector type; alias it here.
extern t4Vector t4vect_Zero_A8C8 __asm__("t4vect_Zero");



// PORT: PS2-only VU0 inline asm (vector length).
static inline float vu0Len_A8C8(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (normalize).
static inline t4Vector vu0Norm_A8C8(const t4Vector& v)
{
    t4Vector r;
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
static inline t4Vector vu0Scale_A8C8(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_A8C8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product).
static inline t4Vector vu0Cross_A8C8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_A8C8(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b)
        : "memory");
}

/**
 * Steps the spring rail's angular springs by one update and repositions its nodes.
 *
 * For each segment: torque = -O * ASpring (+0x60) - W * ADamp (+0x64) + T; O += W * dt; W += torque * dt. Then walks
 * the chain accumulating O and places P[i + 1] at P[i] + V[i] bent by the accumulated angle (normalised V + V cross
 * angle, scaled back to |V|). T of every node is cleared. dt comes from cObjectInterface_getUpdateTime.
 */
extern "C" void cSpringRailNode_updateNodes(cSpringRailNode* self)
{
    float dt = cObjectInterface_getUpdateTime();
    sV4Slot_A8C8 segTorque[8];
    for (int i = 0; i < self->NumNodes - 1; i++) {
        segTorque[i].v = vu0Scale_A8C8(self->Nodes[i].O, -self->ASpring);
        vu0AddEq_A8C8(segTorque[i].v, vu0Scale_A8C8(self->Nodes[i].W, -self->ADamp));
    }
    for (int i = 0; i < self->NumNodes - 1; i++) {
        vu0AddEq_A8C8(segTorque[i].v, self->Nodes[i].T);
    }
    for (int i = 0; i < self->NumNodes - 1; i++) {
        t4Vector angVelStep;
        angVelStep = vu0Scale_A8C8(segTorque[i].v, dt);
        vu0AddEq_A8C8(self->Nodes[i].O, vu0Scale_A8C8(self->Nodes[i].W, dt));
        vu0AddEq_A8C8(self->Nodes[i].W, angVelStep);
    }
    t4Vector acc = t4vect_Zero_A8C8;
    for (int i = 0; i < self->NumNodes - 1; i++) {
        vu0AddEq_A8C8(acc, self->Nodes[i].O);
        float len = vu0Len_A8C8(self->Nodes[i].V);
        t4Vector segVec = vu0Scale_A8C8(vu0Norm_A8C8(vu0Add_A8C8(self->Nodes[i].V, vu0Cross_A8C8(self->Nodes[i].V, acc))), len);
        (self->Nodes)[i + 1].P = vu0Add_A8C8(self->Nodes[i].P, segVec);
    }
    for (int i = 0; i < self->NumNodes; i++) {
        self->Nodes[i].T = t4vect_Zero_A8C8;
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFlexRailVEntryAC10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cFlexRailNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a spring rail to a replay frame.
 *
 * Writes the flex-rail base (cFlexRailNode_writeToReplayFrame), ASpring/ADamp/TorqueScale (+0x60, 0xC bytes) and the NumNodes * 0x60 node
 * array (+0x70).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cSpringRailNode_writeToReplayFrame(cSpringRailNode* self, cBxReplayFrame* stream)
{
    cFlexRailNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->ASpring, 0xC);
    stream->write((const char*)self->Nodes, self->NumNodes * 0x60);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", tFlagState_tFlagState);
#ifdef SKIP_ASM

struct sFrRailDesc;

struct sFrRailOut_AC88 {
    char on0, on1, on2, on3;  // 0x0
    float a[4];               // 0x4
    float b[4];               // 0x14
    float c[4];               // 0x24
    float p[3];               // 0x34
    float q[4];               // 0x40
    float r0;                 // 0x50
    float r1;                 // 0x54
};

struct sFrRailTriple_AC88 {
    float a, b, c;
};

struct sFrRailSrc_AC88 {
    int f0;
    int on0, on1, on2;        // 0x4
    sFrRailTriple_AC88 t[4];  // 0x10
    float p[3];               // 0x40
    float q[4];               // 0x4C
    float r0;                 // 0x5C
    float r1;                 // 0x60
    int on3;                  // 0x64
};

static inline float perSec_34AC88(float x)
{
    return x * (1.0f / (float)cAppMan_TheAppMan->TimerHz);
}

/**
 * Converts flag make-data into the runtime flag state, turning per-second rates into per-frame ones.
 *
 * The held-side and over-saturate ints become bytes (mBaseSideHeld, mOppositeSideHeld, mPerpSideHeld, mOverSaturate);
 * the 4 wave Speeds and the U/V scroll rates are divided by the frame rate (cAppMan_TheAppMan +0x10); Magnitudes,
 * Frequencies, low/high wind movement and MinWindFactor are copied.
 *
 * @param out  tFlagState to fill (0x58 bytes)
 * @param src  tMakeFlagData to read
 * @return out
 */
extern "C" sFrRailDesc* tFlagState_tFlagState(sFrRailDesc* out, void* src)
{
    sFrRailOut_AC88* flagState = (sFrRailOut_AC88*)out;
    sFrRailSrc_AC88* makeData = (sFrRailSrc_AC88*)src;
    flagState->on0 = makeData->on0 != 0;
    flagState->on1 = makeData->on1 != 0;
    flagState->on2 = makeData->on2 != 0;
    flagState->on3 = makeData->on3 != 0;
    {
        float x = makeData->p[0];
        float y = makeData->p[1];
        float z = makeData->p[2];
        flagState->p[0] = x;
        flagState->p[1] = y;
        flagState->p[2] = z;
    }
    {
        float x = makeData->q[0];
        float y = makeData->q[1];
        float z = makeData->q[2];
        flagState->q[0] = x;
        flagState->q[1] = y;
        flagState->q[2] = z;
    }
    flagState->q[3] = makeData->q[3];
    flagState->r0 = perSec_34AC88(makeData->r0);
    flagState->r1 = perSec_34AC88(makeData->r1);
    flagState->a[0] = perSec_34AC88(makeData->t[0].a);
    flagState->b[0] = makeData->t[0].b;
    flagState->c[0] = makeData->t[0].c;
    flagState->a[1] = perSec_34AC88(makeData->t[1].a);
    flagState->b[1] = makeData->t[1].b;
    flagState->c[1] = makeData->t[1].c;
    flagState->a[2] = perSec_34AC88(makeData->t[2].a);
    flagState->b[2] = makeData->t[2].b;
    flagState->c[2] = makeData->t[2].c;
    flagState->a[3] = perSec_34AC88(makeData->t[3].a);
    flagState->b[3] = makeData->t[3].b;
    flagState->c[3] = makeData->t[3].c;
    return out;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flexrailnode", cFlagNode_cFlagNode1);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);
extern "C" void* cObjectInterface_getFlagManager(void);
extern char cFlagNode_vtbl[];

struct sFrRailDesc {
    int data[0x58 / 4];
};

extern "C" sFrRailDesc* tFlagState_tFlagState(sFrRailDesc* out, void* src);
extern "C" void cFlagManager_AddFlag(void* mgr, void* node, sFrRailDesc* desc);

/**
 * Constructs a flag node (node type 10) and registers the flag with the flag manager.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs cFlagNode_vtbl, sets the record flags to (flags & ~0x2) | 0x4,
 * converts src (tFlagState_tFlagState) and adds the instance to the flag manager (cFlagManager_AddFlag on cObjectInterface_getFlagManager()).
 *
 * @param bucket  update bucket
 * @param node  flag instance record
 * @param src  flag make-data
 * @return self
 */
extern "C" void* cFlagNode_cFlagNode1(cFlagNode* self, void* bucket, cFlagNode* node, void* src)
{
    sFrRailDesc desc;
    cInstanceNode_cInstanceNode1(self, bucket, 10, node);
    VPTR_OF(cObjNode, self) = (void**)cFlagNode_vtbl;
    node->Prev = (cObjNode*)(((unsigned int)node->Prev & 0xFFFFFFFD) | 4);
    tFlagState_tFlagState(&desc, src);
    cFlagManager_AddFlag(cObjectInterface_getFlagManager(), node, &desc);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagNode_cFlagNode);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char cFlagNode_vtbl[];

/**
 * Constructs a flag node on top of the instance-node constructor.
 *
 * Runs cInstanceNode_cInstanceNode(), then stores cFlagNode's vtable. Only @p self is declared; the constructor
 * arguments the caller passed reach cInstanceNode_cInstanceNode() unchanged in their registers.
 *
 * @return @p self
 */
extern "C" void* cFlagNode_cFlagNode(cFlagNode* self)
{
    cInstanceNode_cInstanceNode(self);
    VPTR_OF(cObjNode, self) = (void**)cFlagNode_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagNode__cFlagNode);
#ifdef SKIP_ASM
extern char cFlagNode_vtbl[];
extern "C" void* cObjectInterface_getFlagManager(void);
extern "C" void cFlagManager_RemoveFlag(void* set, void* arg);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

/**
 * Destroys a flag node and removes its object from the game's flag manager.
 *
 * Restores cFlagNode's vtable, asks the flag manager (cFlagManager_RemoveFlag) to remove the object at offset 0x18 from
 * the first active flag set holding it, then runs cInstanceNode__cInstanceNode().
 *
 * @param flags  g++ destructor flags, passed to cInstanceNode__cInstanceNode(); bit 0 set frees the object's memory
 */
extern "C" void cFlagNode__cFlagNode(cFlagNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = (void**)cFlagNode_vtbl;
    cFlagManager_RemoveFlag(cObjectInterface_getFlagManager(), self->Instance);
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

extern "C" void* cInstanceNode_writeToReplayFrame(void* self);

//100%
INCLUDE_ASM("object/flexrailnode", cFlagNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the flag node's replay data using the base cInstanceNode_writeToReplayFrame().
 *
 * Only @p self is declared; the replay stream argument reaches the base function unchanged in its register.
 *
 * @return no meaningful value (cInstanceNode_writeToReplayFrame() returns nothing)
 */
void* cFlagNode_writeToReplayFrame(void* self)
{
    return cInstanceNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: the flag node's render has an empty body.
 */
void cFlagNode_render(void* self)
{
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", tFlagSetState_tFlagSetState);
#ifdef SKIP_ASM
/**
 * Constructs a flag-set state: no instances, model id -1.
 *
 * Sets mCurrentNumInstances (+0x5C) to 0 and mModelID (+0x58) to -1.
 *
 * @return self
 */
extern "C" void* tFlagSetState_tFlagSetState(void* self)
{
    *(int*)((char*)self + 0x5c) = 0;
    *(unsigned int*)((char*)self + 0x58) = 0xFFFFFFFFU;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet_cFlagSet);
#ifdef SKIP_ASM
extern int cFlagSet_mgFrameToUpdateGridCounter;
extern void* cFlagSet_vtbl[];


/**
 * Constructs an empty flag set.
 *
 * Runs tFlagSetState_tFlagSetState, nulls mGridMesh (+0x60) and mVerts (+0x74), installs cFlagSet_vtbl (vtable at +0x184), clears the 60
 * mInstances (+0x94) and gives the set the current grid-update parity cFlagSet_mgFrameToUpdateGridCounter (mFrameToUpdateGrid, +0x90), which
 * then alternates 0/1 for the next set.
 *
 * @return self
 */
extern "C" void* cFlagSet_cFlagSet(cFlagSet* self)
{
    cFlagSet* flagSet = self;
    int i;
    tFlagSetState_tFlagSetState(self);
    (*(int*)&flagSet->mGridMesh) = 0;
    (*(int*)&flagSet->mVerts) = 0;
    flagSet->mVtbl = cFlagSet_vtbl;
    for (i = 0; i < 60; i++) {
        flagSet->mInstances[i] = 0;
    }
    int id = cFlagSet_mgFrameToUpdateGridCounter;
    flagSet->mFrameToUpdateGrid = id;
    cFlagSet_mgFrameToUpdateGridCounter = id + 1;
    if (cFlagSet_mgFrameToUpdateGridCounter >= 2) {
        cFlagSet_mgFrameToUpdateGridCounter = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet__cFlagSet);
#ifdef SKIP_ASM
extern void* cFlagSet_vtbl[];
void operator_delete(int*);

/**
 * Destroys a flag set: reinstalls its vtable cFlagSet_vtbl and deletes it when bit 0 of flags is set.
 *
 * @param flags  destructor flags
 */
extern "C" void cFlagSet__cFlagSet(cFlagSet* self, int flags)
{
    self->mVtbl = cFlagSet_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet_SameKind);
#ifdef SKIP_ASM
extern "C" int memcmp(const void* a, const void* b, int n);

/**
 * Returns whether a flag set in use already holds flags of the same kind as a new one.
 *
 * True when the set has instances (+0x5C), its model id (+0x58) equals the instance's model id (first word of ctx
 * +0x80) and the 0x58-byte flag data compares equal (memcmp).
 *
 * @param data  converted flag data of the new flag
 * @param ctx  instance record of the new flag
 * @return 1 if the flag can join this set, else 0
 */
extern "C" int cFlagSet_SameKind(cFlagSet* self, void* data, tInstanceRecord* ctx)
{
    if (self->mCurrentNumInstances == 0) {
        return 0;
    }
    if (*(int*)ctx->m_Model != self->mModelID) {
        return 0;
    }
    return memcmp(data, self, 88) == 0;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet_AddFlag);
#ifdef SKIP_ASM
extern "C" void cFlagSet_CreateMesh(void* self, void* mesh);


struct sFlagSet_34B038 {
    tFlagState def;
    int mModelID;
    int count;
    char pad60[0x94 - 0x60];
    void* meshes[1];
};

/**
 * Adds a flag instance to a flag set, creating the set's cloth mesh for the first one.
 *
 * For an empty set it copies the flag data, stores the instance's model id (+0x58) and builds the mesh
 * (cFlagSet_CreateMesh). The instance is appended to mInstances (+0x94) and the count (+0x5C) incremented; nothing
 * checks the 60-entry limit.
 *
 * @param def  converted flag data
 * @param mesh  instance record of the flag
 */
extern "C" void cFlagSet_AddFlag(sFlagSet_34B038* self, tFlagState* def, tInstanceRecord* mesh)
{
    if (self->count == 0) {
        self->def = *def;
        self->mModelID = *(int*)mesh->m_Model;
        cFlagSet_CreateMesh(self, mesh);
    }
    self->meshes[self->count] = ((char*)mesh);
    self->count++;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet_RemoveFlag);
#ifdef SKIP_ASM


extern "C" void cFlagSet_PurgeMesh(void* self);

/**
 * Removes a flag instance from a flag set, purging the mesh when the set empties.
 *
 * Fails when the set is empty or its model id (+0x58) differs from the instance's. Otherwise finds the instance by id
 * (+0x78), moves the last instance into its place, clears the last slot and decrements the count; an empty set frees
 * its mesh (cFlagSet_PurgeMesh).
 *
 * @param rider  instance record of the flag to remove
 * @return 1 if the instance was removed, else 0
 */
extern "C" int cFlagSet_RemoveFlag(cFlagSet* self, tInstanceRecord* rider)
{
    if (self->mCurrentNumInstances == 0) {
        return 0;
    }
    if (*((int*)rider->m_Model) != self->mModelID) {
        return 0;
    }
    for (int i = 0; i < self->mCurrentNumInstances; i++) {
        if (((int)self->mInstances[i]->m_RID) == ((int)rider->m_RID)) {
            self->mInstances[i] = self->mInstances[self->mCurrentNumInstances - 1];
            self->mInstances[self->mCurrentNumInstances - 1] = 0;
            if (--self->mCurrentNumInstances == 0) {
                cFlagSet_PurgeMesh(self);
            }
            return 1;
        }
    }
    return 0;
}
#endif

