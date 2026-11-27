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
#include "world/world.h"
#include "world/worldcache.h" /* tResourceID */
#include "world/worldrecords.h" /* tModelRecord */

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
void cFlexRailNode::addSpaceHash()
{
    cFlexRailSpaceNode* n = (cFlexRailSpaceNode*)cMemMan_alloc(0x10, str_FlexRailSpaceNode, 0x20000000, 0);
    n->FlexNode = this;
    n->type = 3;
    Node = (cFlexRailSpaceNode*)n;
    nash_Octant* cell = (nash_Octant*)cObjectInterface_getNash();
    cFlexRailSpaceNode* node = Node;
    nash_CellKey key;
    nash_Nash_findkey(&key, (char*)&BoundBox);
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
 * Takes the space hash (cObjectInterface_getNash), bumps its deletecount, finds the hash key of BoundBox
 * (nash_Nash_findkey), picks the octant from the key's sign bits and removes the rail's hash node (Node) from that
 * cell's list 2 (nash_Cell_remnode). An octant left with no children and no lists is deleted. Finally deletes the hash
 * node and nulls Node.
 */
void cFlexRailNode::remSpaceHash()
{
    cBXNash* nash = (cBXNash*)cObjectInterface_getNash();
    cFlexRailSpaceNode* node = Node;
    nash->deletecount++;
    nash_Octant* cell = nash->octant[0][0];
    nash_CellKey key;
    nash_Nash_findkey(&key, (char*)&BoundBox);
    nash_Octant* cellX = key.x < 0 ? cell + 4 : cell;
    nash_Octant* cellXY = key.y < 0 ? cellX + 2 : cellX;
    nash_Octant* cellXYZ = key.z < 0 ? cellXY + 1 : cellXY;
    nash_Octant* octant = cellXYZ;
    nash_Cell_remnode(octant->rootcell, 2, node, &key, octant);
    if (octEmpty_8C48(octant->rootcell)) {
        operator_delete((int*)(octant->rootcell));
        octant->rootcell = 0;
    }
    operator_delete((int*)Node);
    Node = 0;
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

// tResourceID with EA's inline constructor to the invalid id (retail fills the local id array with -1 first).
struct tRailInstId : public tResourceID {
    tRailInstId() : tResourceID(0xFFFFFFFF) {}
};

// tFloatRailNode with EA's inline constructor: a new node's InstId starts invalid (retail's new[] runs a constructor
// loop storing -1 at +0x40).
struct tFloatRailNodeInit : public tFloatRailNode {
    tFloatRailNodeInit() { InstId = 0xFFFFFFFF; }
};

/**
 * Constructs a float rail (node type 20) for a placed rail instance.
 *
 * Runs the flex-rail constructor cFlexRailNode_cFlexRailNode, installs cFloatRailNode_vtbl and copies LSpring, LDamp
 * and ForceScale from the make data. Allocates NumNodes 0x50-byte nodes (Nodes, "FloatRailNodes"); each gets the next
 * valid instance id (InstId1..8) and mesh (Mesh1..8), zero Vel and Force, and its Pos and OPos from the mesh position
 * (cFloatRailNode_getPosition). Then builds the segment matrices (cFloatRailNode_updateMats), BoundBox
 * (cFloatRailNode_updateBoundBox) and the space hash entry (cFlexRailNode_addSpaceHash).
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
    tRailInstId refs[8];
    int bones[8];
    int n = 0;
    unsigned int* ids = &desc->InstId1;
    int* bn = &desc->Mesh1;
    for (int i = 0; i < 8; i++, ids++, bn++) {
        unsigned int id = *ids;
        if (id != tResourceID::kInvalidRID) {
            refs[n].m_uiRID = id;
            bones[n] = *bn;
            n++;
        }
    }
    tFloatRailNode*& nodes = self->Nodes;
    nodes = new (str_FloatRailNodes, 0x20000000, 0) tFloatRailNodeInit[self->NumNodes];
    for (int j = 0; j < self->NumNodes; j++) {
        self->Nodes[j].InstId = refs[j].m_uiRID;
        self->Nodes[j].Mesh = bones[j];
        self->Nodes[j].Vel = t4vect_Zero;
        self->Nodes[j].Force = t4vect_Zero;
        self->getPosition(&self->Nodes[j]);
        self->Nodes[j].OPos = self->Nodes[j].Pos;
    }
    self->cFloatRailNode::updateMats();
    self->cFloatRailNode::updateBoundBox();
    self->addSpaceHash();
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



/**
 * Constructs a float rail from a replay frame.
 *
 * Runs cFlexRailNode_cFlexRailNode1, installs cFloatRailNode_vtbl, reads LSpring/LDamp/ForceScale (0xC bytes) and
 * the NumNodes * 0x50 node array (Nodes, allocated as "FloatRailNodes"), then rebuilds the matrices
 * (cFloatRailNode_updateMats), BoundBox (cFloatRailNode_updateBoundBox) and space hash entry
 * (cFlexRailNode_addSpaceHash).
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
void* cFloatRailNode::cFloatRailNode1(void* bucket, cBxReplayFrame* stream)
{
    cFlexRailNode_cFlexRailNode1(((char*)this), bucket, stream);
    VPTR_OF(cObjNode, this) = (void**)cFloatRailNode_vtbl;
    stream->read((char*)&LSpring, 0xC);
    tFloatRailNode** nodes = &Nodes;
    *nodes = new (str_FloatRailNodes, 0x20000000, 0) tFloatRailNodeInit[NumNodes];
    stream->read((char*)Nodes, NumNodes * 80);
    this->cFloatRailNode::updateMats();
    this->cFloatRailNode::updateBoundBox();
    this->addSpaceHash();
    return ((char*)this);
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
    self->remSpaceHash();
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
void cFloatRailNode::updateBoundBox()
{
    sFrBoxElem* points = (sFrBoxElem*)Nodes;
    int n = NumNodes;
    BoundBox.MinPoint = BoundBox.MaxPoint = points[0].pos;
    for (int i = 1; i < n; i++) {
        t4Vector* point = &points[i].pos;
        if (point->x < BoundBox.MinPoint.x) {
            BoundBox.MinPoint.x = point->x;
        }
        if (point->y < BoundBox.MinPoint.y) {
            BoundBox.MinPoint.y = point->y;
        }
        if (point->z < BoundBox.MinPoint.z) {
            BoundBox.MinPoint.z = point->z;
        }
        if (point->x > BoundBox.MaxPoint.x) {
            BoundBox.MaxPoint.x = point->x;
        }
        if (point->y > BoundBox.MaxPoint.y) {
            BoundBox.MaxPoint.y = point->y;
        }
        if (point->z > BoundBox.MaxPoint.z) {
            BoundBox.MaxPoint.z = point->z;
        }
    }
    t4Vector margin;
    margin.x = 200.0f;
    margin.y = 200.0f;
    margin.z = 200.0f;
    margin.w = 0.0f;
    vu0SubFRBox(BoundBox.MinPoint, margin);
    vu0AddFRBox(BoundBox.MaxPoint, margin);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_getPosition);
#ifdef SKIP_ASM

struct sFrNodeMtx {
    t4Vector r[4];
};

extern "C" cWorld* cObjectInterface_getWorld();
extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, sFrNodeMtx* out);

static inline tInstanceRecord* refToInstance(unsigned int p)
{
    return (tInstanceRecord*)(p << 2);
}

// The instance record that resource id @p id names, or 0 when its section is not loaded or its slot is empty: the
// section's cache table keeps each instance (type 3) as its address >> 2 (EA's inline world-cache lookup; the same
// one as src/object/multiparticle.cpp's lookup58260).
static inline tInstanceRecord* getRailInstance(const unsigned int& id)
{
    cWorldCacheTable* table = cObjectInterface_getWorld()->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    if (table == 0) {
        return 0;
    }
    unsigned int p = table->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
    if (p == 0) {
        return 0;
    }
    return refToInstance(p);
}


/**
 * Sets a float rail node's Pos and OPos to the current position of its mesh.
 *
 * Builds the mesh matrix of the node's instance and mesh (cInstanceNode_buildMeshMat) and copies its translation.
 *
 * @param node  rail node to update
 */
void cFloatRailNode::getPosition(tFloatRailNode* node)
{
    sFrNodeMtx boneMat;
    cInstanceNode_buildMeshMat(getRailInstance(node->InstId), node->Mesh, &boneMat);
    t4Vector bonePos = boneMat.r[3];
    node->OPos = bonePos;
    node->Pos = bonePos;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateMats);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).




extern float CatmullRom[16];

// PORT: PS2-only VU0 inline asm (vector * 4x4 basis matrix, row-vector convention).
static inline void vu0BasisMul_92A8(t4Vector& dst, const t4Vector& g, const float* b)
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

static inline void set4_92A8(t4Vector& r, float x, float y, float z, float w)
{
    r.x = x; r.y = y; r.z = z; r.w = w;
}

// Component @p i (0 x, 1 y, 2 z, 3 w) of @p v, read through its address as EA's vector index operator does.
static inline const float& component_92A8(const t4Vector& v, int i)
{
    return (&v.x)[i];
}

// Spline basis matrix for control points a, b, c, d.
static inline void updateMat_92A8(t4Matrix& out, const t4Vector& a, const t4Vector& b, const t4Vector& c,
                                  const t4Vector& d)
{
    t4Matrix L;
    t4Vector v;
    set4_92A8(L.row[1], component_92A8(a, 0), component_92A8(b, 0), component_92A8(c, 0), component_92A8(d, 0));
    vu0BasisMul_92A8(L.row[2], L.row[1], CatmullRom);
    L.row[0] = L.row[2];
    set4_92A8(L.row[2], component_92A8(a, 1), component_92A8(b, 1), component_92A8(c, 1), component_92A8(d, 1));
    vu0BasisMul_92A8(L.row[3], L.row[2], CatmullRom);
    L.row[1] = L.row[3];
    set4_92A8(L.row[3], component_92A8(a, 2), component_92A8(b, 2), component_92A8(c, 2), component_92A8(d, 2));
    vu0BasisMul_92A8(v, L.row[3], CatmullRom);
    L.row[2] = v;
    out.row[0] = L.row[0];
    out.row[1] = L.row[1];
    out.row[2] = L.row[2];
    L.row[3].x = 0.0f;
    L.row[3].y = 0.0f;
    L.row[3].z = 0.0f;
    L.row[3].w = 1.0f;
    out.row[3] = L.row[3];
    MMI_TRANSPOSE4(&out);
}

/**
 * Rebuilds a float rail's segment matrices from its node positions.
 *
 * Each segment matrix (Mats) is the cubic for that segment built from the four surrounding node positions with the
 * basis CatmullRom; the first and last segments repeat the end node as the missing control point.
 *
 * @param selfv  the float rail
 */
// PORT: hand-written MMI 4x4 transpose (MMI_TRANSPOSE4: lq/pextlw/pextuw/pcpyld/pcpyud/sq, no gcc 2.95 pattern;
// retail's registers differ per call site, so an operand asm) plus VU0 macro asm; a port replaces them with a
// plain C transpose and a C vector-matrix product.
void cFloatRailNode::updateMats()
{
    cFloatRailNode* self = this;
    int i = 1;
    updateMat_92A8(self->Mats[0], self->Nodes[0].Pos, self->Nodes[0].Pos, self->Nodes[1].Pos, self->Nodes[2].Pos);
    for (; i < self->NumNodes - 2; i++) {
        updateMat_92A8(self->Mats[i], self->Nodes[i - 1].Pos, self->Nodes[i].Pos, self->Nodes[i + 1].Pos,
                       self->Nodes[i + 2].Pos);
    }
    updateMat_92A8(self->Mats[i], self->Nodes[i - 1].Pos, self->Nodes[i].Pos, self->Nodes[i + 1].Pos,
                   self->Nodes[i + 1].Pos);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_applyForce);
#ifdef SKIP_ASM

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
 * Does nothing when the hit segment (Seg) is negative. Otherwise node Seg gets offset * (1 - T) and node Seg + 1 gets
 * offset * T added to its Force.
 *
 * @param node  intersect info of the hit (its Seg and T)
 * @param offset  force to apply
 */
extern "C" void cFloatRailNode_applyForce(cFloatRailNode* self, tIntersectInfo* node, t4Vector* offset)
{
    if (node->Seg >= 0) {
        vu0AddFR(self->Nodes[node->Seg].Force, vu0ScaleFR(*offset, 1.0f - node->T));
        vu0AddFR(self->Nodes[node->Seg + 1].Force, vu0ScaleFR(*offset, node->T));
    }
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFloatRailNode_updateIntersectInfo);
#ifdef SKIP_ASM
/**
 * Adds the float rail's velocity at the hit point to an intersect info.
 *
 * Does nothing when the hit segment (Seg) is negative. Otherwise adds Vel[Seg] * T + Vel[Seg + 1] * (1 - T) to the
 * info's Ivel.
 *
 * Likely bug: the weights are the reverse of cFloatRailNode_applyForce, which gives node seg the (1 - t) share.
 *
 * @param node  intersect info of the hit
 */
// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (vector add-assign).
extern "C" void cFloatRailNode_updateIntersectInfo(cFloatRailNode* self, tIntersectInfo* node)
{
    if (node->Seg >= 0) {
        vu0AddFR(node->Ivel, vu0ScaleFR(self->Nodes[node->Seg].Vel, node->T));
        vu0AddFR(node->Ivel, vu0ScaleFR(self->Nodes[node->Seg + 1].Vel, 1.0f - node->T));
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
 * For each node: snaps Pos and OPos back to the mesh (cFloatRailNode_getPosition) while the instance has an active node
 * (m_pActiveNode); accel = -(Pos - OPos) * LSpring + Force * ForceScale - Vel * LDamp; Pos += Vel * dt; Vel += accel *
 * dt (dt from cObjectInterface_getUpdateTime); Force is cleared.
 */
void cFloatRailNode::updateNodes()
{
    float dt = cObjectInterface_getUpdateTime();
    tFloatRailNode* n = Nodes;
    for (int i = 0; i < NumNodes; i++, n++) {
        if (getRailInstance(n->InstId)->m_pActiveNode)
            getPosition((tFloatRailNode*)n);
        // The three cFloatRailNodeState words (LSpring, LDamp, ForceScale): retail keeps their address in a register
        // and reads ForceScale as its third word.
        float* state = &LSpring;
        float ks = -LSpring;
        t4Vector acc = vu0Add_98E8(vu0Scale_98E8(vu0Sub_98E8((*(&n->Pos)), (*(&n->OPos))), ks),
                                    vu0Scale_98E8((*(&n->Force)), state[2]));
        vu0AddEq_98E8(acc, vu0Scale_98E8((*(&n->Vel)), -LDamp));
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

// tSpringRailNode with EA's inline constructor: a new node's InstId starts invalid (retail's new[] runs a constructor
// loop storing -1 at +0x50).
struct tSpringRailNodeInit : public tSpringRailNode {
    tSpringRailNodeInit() { InstId = 0xFFFFFFFF; }
};

/**
 * Constructs a spring rail (node type 21) for a placed rail instance.
 *
 * Runs the flex-rail constructor cFlexRailNode_cFlexRailNode, installs cSpringRailNode_vtbl and copies ASpring, ADamp
 * and TorqueScale from the make data. Allocates NumNodes 0x60-byte nodes (Nodes, "SpringRailNodes"); each gets the
 * next valid instance id and mesh, zero V/O/W/T and its P from the mesh position (cSpringRailNode_getPosition). V of
 * each node becomes the vector to the next node's P. Then builds the segment matrices (cSpringRailNode_updateMats),
 * BoundBox (cSpringRailNode_updateBoundBox) and the space hash entry (cFlexRailNode_addSpaceHash).
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
    tRailInstId refs[8];
    int bones[8];
    int n = 0;
    unsigned int* ids = &desc->InstId1;
    int* bn = &desc->Mesh1;
    for (int i = 0; i < 8; i++, ids++, bn++) {
        unsigned int id = *ids;
        if (id != tResourceID::kInvalidRID) {
            refs[n].m_uiRID = id;
            bones[n] = *bn;
            n++;
        }
    }
    tSpringRailNode*& nodes = self->Nodes;
    nodes = new (str_SpringRailNodes, 0x20000000, 0) tSpringRailNodeInit[self->NumNodes];
    for (int j = 0; j < self->NumNodes; j++) {
        self->Nodes[j].InstId = refs[j].m_uiRID;
        self->Nodes[j].Mesh = bones[j];
        self->Nodes[j].W = t4vect_Zero_9B48;
        self->Nodes[j].T = t4vect_Zero_9B48;
        self->Nodes[j].O = t4vect_Zero_9B48;
        self->Nodes[j].V = t4vect_Zero_9B48;
        self->getPosition(&self->Nodes[j]);
    }
    for (int k = 0; k < self->NumNodes - 1; k++) {
        self->Nodes[k].V = Sub_9B48(self->Nodes[k + 1].P, self->Nodes[k].P);
    }
    self->cSpringRailNode::updateMats();
    self->cSpringRailNode::updateBoundBox();
    self->addSpaceHash();
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



/**
 * Constructs a spring rail from a replay frame.
 *
 * Runs cFlexRailNode_cFlexRailNode1, installs cSpringRailNode_vtbl, reads ASpring/ADamp/TorqueScale (0xC bytes) and
 * the NumNodes * 0x60 node array (Nodes, "SpringRailNodes"), then rebuilds the matrices (cSpringRailNode_updateMats)
 * and the space hash entry. Unlike the float rail's replay ctor it does not recompute BoundBox (it comes back from the
 * replay state).
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
void* cSpringRailNode::cSpringRailNode1(void* bucket, cBxReplayFrame* stream)
{
    cFlexRailNode_cFlexRailNode1(((char*)this), bucket, stream);
    VPTR_OF(cObjNode, this) = (void**)cSpringRailNode_vtbl;
    stream->read((char*)&ASpring, 0xC);
    tSpringRailNode** nodes = &Nodes;
    *nodes = new (str_SpringRailNodes, 0x20000000, 0) tSpringRailNodeInit[NumNodes];
    stream->read((char*)Nodes, NumNodes * 0x60);
    this->cSpringRailNode::updateMats();
    this->addSpaceHash();
    return ((char*)this);
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
    self->remSpaceHash();
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
void cSpringRailNode::updateBoundBox()
{
    sFrBoxElem60* points = (sFrBoxElem60*)Nodes;
    int n = NumNodes;
    BoundBox.MinPoint = BoundBox.MaxPoint = points[0].pos;
    for (int i = 1; i < n; i++) {
        t4Vector* point = &points[i].pos;
        if (point->x < BoundBox.MinPoint.x) {
            BoundBox.MinPoint.x = point->x;
        }
        if (point->y < BoundBox.MinPoint.y) {
            BoundBox.MinPoint.y = point->y;
        }
        if (point->z < BoundBox.MinPoint.z) {
            BoundBox.MinPoint.z = point->z;
        }
        if (point->x > BoundBox.MaxPoint.x) {
            BoundBox.MaxPoint.x = point->x;
        }
        if (point->y > BoundBox.MaxPoint.y) {
            BoundBox.MaxPoint.y = point->y;
        }
        if (point->z > BoundBox.MaxPoint.z) {
            BoundBox.MaxPoint.z = point->z;
        }
    }
    t4Vector margin;
    margin.x = 200.0f;
    margin.y = 200.0f;
    margin.z = 200.0f;
    margin.w = 0.0f;
    vu0SubFRBox(BoundBox.MinPoint, margin);
    vu0AddFRBox(BoundBox.MaxPoint, margin);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cSpringRailNode_getPosition);
#ifdef SKIP_ASM
/**
 * Sets a spring rail node's P to the current position of its mesh (cInstanceNode_buildMeshMat translation).
 *
 * @param node  rail node to update
 */
void cSpringRailNode::getPosition(tSpringRailNode* node)
{
    sFrNodeMtx boneMat;
    cInstanceNode_buildMeshMat(getRailInstance(node->InstId), node->Mesh, &boneMat);
    t4Vector bonePos = boneMat.r[3];
    node->P = bonePos;
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
void cSpringRailNode::updateMats()
{
    cSpringRailNode* self = this;
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
void cSpringRailNode::updateNodes()
{
    float dt = cObjectInterface_getUpdateTime();
    sV4Slot_A8C8 segTorque[8];
    for (int i = 0; i < NumNodes - 1; i++) {
        segTorque[i].v = vu0Scale_A8C8(Nodes[i].O, -ASpring);
        vu0AddEq_A8C8(segTorque[i].v, vu0Scale_A8C8(Nodes[i].W, -ADamp));
    }
    for (int i = 0; i < NumNodes - 1; i++) {
        vu0AddEq_A8C8(segTorque[i].v, Nodes[i].T);
    }
    for (int i = 0; i < NumNodes - 1; i++) {
        t4Vector angVelStep;
        angVelStep = vu0Scale_A8C8(segTorque[i].v, dt);
        vu0AddEq_A8C8(Nodes[i].O, vu0Scale_A8C8(Nodes[i].W, dt));
        vu0AddEq_A8C8(Nodes[i].W, angVelStep);
    }
    t4Vector acc = t4vect_Zero_A8C8;
    for (int i = 0; i < NumNodes - 1; i++) {
        vu0AddEq_A8C8(acc, Nodes[i].O);
        float len = vu0Len_A8C8(Nodes[i].V);
        t4Vector segVec = vu0Scale_A8C8(vu0Norm_A8C8(vu0Add_A8C8(Nodes[i].V, vu0Cross_A8C8(Nodes[i].V, acc))), len);
        Nodes[i + 1].P = vu0Add_A8C8(Nodes[i].P, segVec);
    }
    for (int i = 0; i < NumNodes; i++) {
        Nodes[i].T = t4vect_Zero_A8C8;
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
extern "C" tFlagState* tFlagState_tFlagState(tFlagState* out, void* src)
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

extern "C" tFlagState* tFlagState_tFlagState(tFlagState* out, void* src);
extern "C" void cFlagManager_AddFlag(void* mgr, tInstanceRecord* node, tFlagState* desc);

/**
 * Constructs a flag node (node type 10) and registers the flag with the flag manager.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs cFlagNode_vtbl, sets the record's m_iFlags to
 * (m_iFlags & ~0x2) | 0x4,
 * converts src (tFlagState_tFlagState) and adds the instance to the flag manager (cFlagManager_AddFlag on cObjectInterface_getFlagManager()).
 *
 * @param bucket  update bucket
 * @param node  flag instance record
 * @param src  flag make-data
 * @return self
 */
void* cFlagNode::cFlagNode1(void* bucket, tInstanceRecord* node, void* src)
{
    tFlagState desc;
    cInstanceNode_cInstanceNode1(this, bucket, 10, node);
    VPTR_OF(cObjNode, this) = (void**)cFlagNode_vtbl;
    node->m_iFlags = (node->m_iFlags & 0xFFFFFFFD) | 4;
    tFlagState_tFlagState(&desc, src);
    cFlagManager_AddFlag(cObjectInterface_getFlagManager(), node, &desc);
    return ((void*)this);
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
void* cFlagNode_writeToReplayFrame(void* vself)
{
    cFlagNode* self = (cFlagNode*)vself;
    return cInstanceNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagNode_render__FPv);
#ifdef SKIP_ASM
/**
 * Draws nothing: the flag node's render has an empty body.
 */
void cFlagNode::render()
{
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", tFlagSetState_tFlagSetState);
#ifdef SKIP_ASM
/**
 * Constructs a flag-set state: no instances, model id -1.
 *
 * Sets mCurrentNumInstances to 0 and mModelID to -1. The state is the first 0x60 bytes of a cFlagSet.
 *
 * @return self
 */
extern "C" void* tFlagSetState_tFlagSetState(void* self)
{
    cFlagSet* flagSet = (cFlagSet*)self;
    flagSet->mCurrentNumInstances = 0;
    flagSet->mModelID = 0xFFFFFFFFU;
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
    flagSet->mGridMesh = 0;
    flagSet->mVerts = 0;
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
 * @return true if the flag can join this set
 */
bool cFlagSet::SameKind(void* data, tInstanceRecord* ctx)
{
    if (mCurrentNumInstances == 0) {
        return false;
    }
    if (*(int*)ctx->m_Model != mModelID) {
        return false;
    }
    return memcmp(data, this, 88) == 0;
}
#endif

//100%
INCLUDE_ASM("object/flexrailnode", cFlagSet_AddFlag);
#ifdef SKIP_ASM
extern "C" void cFlagSet_CreateMesh(void* self, void* mesh);


/**
 * Adds a flag instance to a flag set, creating the set's cloth mesh for the first one.
 *
 * For an empty set it copies the flag data (mFlagData), stores the instance's model id (mModelID) and builds the mesh
 * (cFlagSet_CreateMesh). The instance is appended to mInstances and mCurrentNumInstances incremented; nothing checks
 * the 60-entry limit.
 *
 * @param def  converted flag data
 * @param mesh  instance record of the flag
 */
void cFlagSet::AddFlag(tFlagState* def, tInstanceRecord* mesh)
{
    if (mCurrentNumInstances == 0) {
        mFlagData = *def;
        mModelID = mesh->m_Model->m_RID;
        cFlagSet_CreateMesh(this, mesh);
    }
    mInstances[mCurrentNumInstances] = mesh;
    mCurrentNumInstances++;
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
 * @return true if the instance was removed
 */
bool cFlagSet::RemoveFlag(tInstanceRecord* rider)
{
    if (mCurrentNumInstances == 0) {
        return false;
    }
    if (*((int*)rider->m_Model) != mModelID) {
        return false;
    }
    for (int i = 0; i < mCurrentNumInstances; i++) {
        if (((int)mInstances[i]->m_RID) == ((int)rider->m_RID)) {
            mInstances[i] = mInstances[mCurrentNumInstances - 1];
            mInstances[mCurrentNumInstances - 1] = 0;
            if (--mCurrentNumInstances == 0) {
                cFlagSet_PurgeMesh(this);
            }
            return true;
        }
    }
    return false;
}
#endif

