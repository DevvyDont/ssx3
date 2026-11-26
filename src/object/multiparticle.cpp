#include "vtable.h"
#include "common.h"
#include "world/worldrecords.h"
#include "ai/rider.h"
#include "intersect/bxnash.h"
#include "world/worldview.h"
#include "world/instancerecord.h"
#include "object/animframe.h"
#include "bx/appman.h"
#include "bx/math3d.h"
#include "object/animdeltanode.h"
#include "object/railmannode.h"
#include "object/animcombonode.h"
#include "object/animnode.h"
#include "object/multiparticle.h"
#include "object/splinemodifier.h"
#include "object/physicsmodifier.h"
#include "replay/bxreplayframe.h"
#include "world/worldcache.h"
#include "world/world.h"

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_setupMultiParticle);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern "C" void* cStaticEmitter_cStaticEmitter(void* p);
extern "C" void cStaticEmitter_setupEmitter(void* sys, void* data, int n);
extern char str_InstIds[];
extern char str_Emitter[];
extern t4Matrix t4mat_Identity;

// PORT: array new bound to the tagged allocator
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sMPRef {
    unsigned int id;
    sMPRef() : id(0xFFFFFFFF) {}
};


/**
 * Allocates a multi-particle effect's instance-id table and its static emitter, and sets the emitter up.
 *
 * InstIds (+0xC) becomes MaxInst (+0x0) ids ("InstIds"), all -1; Emitter (+0x10) a 0x190-byte cStaticEmitter
 * ("Emitter", constructed by cStaticEmitter_cStaticEmitter), set up by cStaticEmitter_setupEmitter (On Tour setupEmitter) with an identity
 * transform and the particle data (+0x8).
 */
void cMultiParticle::setupMultiParticle()
{
    sMPRef** slot = &(*(sMPRef**)&InstIds);
    *slot = new (str_InstIds, 0x20000000, 0) sMPRef[MaxInst];
    (*(void**)&Emitter) = cStaticEmitter_cStaticEmitter(cMemMan_alloc(400, str_Emitter, 0x20000000, 0));
    for (int i = 0; i < MaxInst; i++) {
        InstIds[i] = tResourceID::Invalid();
    }
    cStaticEmitter_setupEmitter(((void*)Emitter), &t4mat_Identity, ((int)Particle));
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_update);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_update(void* self, float dt);

/**
 * Advances the multi-particle's static emitter by one 1/60-second frame.
 *
 * Calls cStaticEmitter_update() on the emitter that offset 0x10 points to.
 */
void cMultiParticle::update()
{
    cStaticEmitter_update(Emitter, 0.01666666753590107f);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_unloadSection);
#ifdef SKIP_ASM

/**
 * Drops a world section's instances from a multi-particle system.
 *
 * Removes every id in InstIds (+0xC, NumInst +0x4) whose low byte is the section, moving the last id into the hole and
 * rechecking it; a removed last entry becomes -1.
 *
 * @param key  world section id
 */
void cMultiParticle::unloadSection(unsigned int key)
{
    int i;
    for (i = 0; i < NumInst; i++) {
        if (InstIds[i].m_bfRID.m_uiSectionID == key) {
            NumInst--;
            if (i < NumInst) {
                InstIds[i] = InstIds[NumInst];
                i--;
            } else {
                InstIds[i] = tResourceID::Invalid();
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_addInst);
#ifdef SKIP_ASM
struct sParticleList {
    int capacity; // 0x0
    int count;    // 0x4
    int field_0x8;
    int* items;   // 0xc
};

/**
 * Adds an instance id to a multi-particle system when it has room (NumInst below MaxInst).
 *
 * @param list  the multi-particle system
 * @param item  instance id to add
 */
extern "C" void cMultiParticle_addInst(sParticleList* list, int item)
{
    int n = list->count;
    if (n < list->capacity) {
        list->items[n] = item;
        list->count = n + 1;
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_remInst);
#ifdef SKIP_ASM

/**
 * Removes an instance id from a multi-particle system.
 *
 * Finds the first matching id in InstIds, decrements NumInst and moves the last id into its place (or sets the slot to
 * -1 when it was last).
 *
 * @param key  instance id to remove
 */
// Removes the first entry equal to key, filling the hole with the last entry.
void cMultiParticle::remInst(unsigned int key)
{
    int i;
    for (i = 0; i < NumInst; i++) {
        if (InstIds[i].m_uiRID == key) {
            NumInst--;
            // the do/while(0) wrapper (a compiled-out macro block) is needed to match register allocation
            do {
                if (i < NumInst) {
                    InstIds[i] = InstIds[NumInst];
                } else {
                    InstIds[i] = tResourceID::Invalid();
                }
                return;
            } while (0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_render);
#ifdef SKIP_ASM

extern "C" cWorld* cObjectInterface_getWorld();
extern "C" void cStaticEmitter_render(void* desc, tBoundBox* box, const t4Vector& center);

static inline void* refToPtr58260(unsigned int p)
{
    return (void*)(p << 2);
}

// The record in slot uid of a section table's type list: the tag keeps its address >> 2 (0 = not loaded).
static inline void* lookup58260(cWorldCacheTable* table, int type, unsigned int uid)
{
    unsigned int p = table->m_pCacheTable[type].m_pDataTag[uid].m_bfuiMemLoc;
    if (p == 0) {
        return 0;
    }
    return refToPtr58260(p);
}

// The instance record @p rid names (cache type 3), or 0 when its section or record is not loaded.
static inline tInstanceRecord* instanceRecord58260(const tResourceID& rid, cWorld* world)
{
    unsigned int id = rid.m_uiRID;
    cWorldCacheTable* table = world->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    if (table == 0) {
        return 0;
    }
    return (tInstanceRecord*)lookup58260(table, 3, id >> 8);
}

// The instance's position: the last row of its world transform (16-byte aligned in the record).
static inline t4Vector instancePos58260(const tInstanceRecord* inst)
{
    return *(const t4Vector*)inst->m_fmTransform.m[3];
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddTo58260(t4Vector& dst, t4Vector b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector vu0Add58260(const t4Vector& a, const t4Vector& b)
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


/**
 * Draws the multi-particle effect once at each loaded instance it is attached to.
 *
 * For each of the NumInst (+0x4) ids in InstIds (+0xC) whose instance record is loaded in the world, calls
 * cStaticEmitter_render() with BoundBox (+0x20) moved by the record's position (record +0x40), drawing at
 * that position plus Offset (+0x40).
 */
void cMultiParticle::render()
{
    for (int i = 0; i < NumInst; i++) {
        tInstanceRecord* inst = instanceRecord58260(InstIds[i], cObjectInterface_getWorld());
        if (inst != 0) {
            tBoundBox bounds = BoundBox;
            vu0AddTo58260(bounds.MinPoint, instancePos58260(inst));
            vu0AddTo58260(bounds.MaxPoint, instancePos58260(inst));
            cStaticEmitter_render(((void*)Emitter), &bounds, vu0Add58260(instancePos58260(inst), Offset));
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_writeToReplayFrame);
#ifdef SKIP_ASM


extern "C" void cStaticEmitter_writeToReplayFrame(void* desc, void* stream);

/**
 * Writes a multi-particle effect to a replay frame: its counts, particle data, emitter, offset, bound box and
 * instance ids.
 *
 * In order: MaxInst and NumInst (4 bytes each), the 0xD8-byte particle data (+0x8), the emitter
 * (cStaticEmitter_writeToReplayFrame), Offset (+0x40), BoundBox (+0x20) and, when NumInst > 0, the first
 * NumInst entries of InstIds.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
void cMultiParticle::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((const char*)this, 4);
    stream->write((const char*)&NumInst, 4);
    stream->write((const char*)Particle, 216);
    cStaticEmitter_writeToReplayFrame(Emitter, stream);
    stream->write((const char*)&Offset, 0x10);
    stream->write((const char*)&BoundBox, 0x20);
    if (NumInst > 0) {
        stream->write((const char*)InstIds, NumInst << 2);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cPhysicsModifier_cPhysicsModifier);
#ifdef SKIP_ASM
// Same stream-like class as cPhysicsModifier_writeToReplayFrame_cObj; v01 sits at vtable offset 8, v02 at 0x10.
extern char cPhysicsModifier_vtbl[];

/**
 * Constructs a physics modifier from a replay frame: installs cPhysicsModifier_vtbl and reads its first 4 bytes.
 *
 * @param obj  replay stream (its read slot, vtable entry 2)
 * @return self
 */
extern "C" void* cPhysicsModifier_cPhysicsModifier(cPhysicsModifier* self, cBxReplayFrame* obj)
{
    VPTR_OF(cPhysicsModifier, self) = (void**)cPhysicsModifier_vtbl;
    obj->read((char*)self, 4);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cPhysicsModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cPhysicsModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void write(void* buf, int size);
};

/**
 * Writes the physics modifier's Active word (4 bytes at +0x0) through the replay frame's write virtual (slot 1).
 *
 * @param obj  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cPhysicsModifier_writeToReplayFrame(cPhysicsModifier* self, cBxReplayFrame* obj)
{
    obj->write((const char*)self, 4);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_cRailManNode1);
#ifdef SKIP_ASM
extern void* cRailManNode_ActiveNode;
extern void* cRailManNode_vtbl[];
extern "C" void* cSortObjNode_cSortObjNode1(void* self, void* a1, int type);

/**
 * Constructs the rail manager node (node type 18) and makes it the active one.
 *
 * Runs the sort-node constructor cSortObjNode_cSortObjNode1, installs cRailManNode_vtbl, sets all 128 rail slots (+0x18) to -1,
 * stores itself in cRailManNode_ActiveNode (the active rail manager) and clears the slots again.
 *
 * @param bucket  update bucket
 * @return self
 */
void* cRailManNode::cRailManNode1(void* bucket)
{
    int i;
    cSortObjNode_cSortObjNode1(this, bucket, 18);
    VPTR_OF(cObjNode, this) = cRailManNode_vtbl;
    unsigned int* railSlot = &RailInfo[0].RailRID.m_uiRID;
    for (i = 0x7F; i != -1; i--, railSlot++) {
        *railSlot = 0xFFFFFFFF;
    }
    cRailManNode_ActiveNode = ((void*)this);
    for (i = 0; i < 0x80; i++) {
        RailInfo[i].RailRID = tResourceID::Invalid();
    }
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_cRailManNode);
#ifdef SKIP_ASM
extern void* cRailManNode_ActiveNode;
extern void* cRailManNode_vtbl[];
extern "C" void* cSortObjNode_cSortObjNode(void* self, void* a1, void* stream);
extern "C" void cRailManNode_disableRail(cRailManNode* self, int idx);

/**
 * Rebuilds the rail manager node from a replay frame and disables again every rail it had disabled.
 *
 * After the cSortObjNode replay constructor and its vtable, the 128 rail slots (+0x18, On Tour RailInfo) are
 * set to -1, the node becomes the active one (cRailManNode_ActiveNode, On Tour ActiveNode) and the 0x200 bytes of slots
 * are read from the frame. cRailManNode_disableRail (On Tour disableRail) takes each used slot's spline segments out of
 * the Nash rail-space octree.
 *
 * @param bucket  bucket index, passed on to the cSortObjNode constructor
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cRailManNode_cRailManNode(cRailManNode* self, void* bucket, cBxReplayFrame* stream)
{
    int i;
    int j;
    cSortObjNode_cSortObjNode(self, bucket, stream);
    VPTR_OF(cObjNode, self) = cRailManNode_vtbl;
    unsigned int* slot = &self->RailInfo[0].RailRID.m_uiRID;
    for (j = 0x7F; j != -1; j--, slot++) {
        *slot = 0xFFFFFFFF;
    }
    cRailManNode_ActiveNode = ((void*)self);
    stream->read((char*)&self->RailInfo[0], 0x200);
    for (i = 0; i < 0x80; i++) {
        if (self->RailInfo[i].RailRID != tResourceID::Invalid()) {
            self->disableRail(i);
        }
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode__cRailManNode);
#ifdef SKIP_ASM
extern void* cRailManNode_ActiveNode;
extern void* cRailManNode_vtbl[];
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx);
// PORT: cSortObjNode__cSortObjNode__FPv is the ObjNode deleting dtor; it takes (self, flags).
extern void* cSortObjNode__cSortObjNode_dtor(void* self, int flags) __asm__("cSortObjNode__cSortObjNode__FPv");

/**
 * Destroys the rail manager node: re-enables every rail it disabled and clears the active-node pointer.
 *
 * cRailManNode_enableRail (On Tour enableRail) puts each used slot's spline segments back into the Nash rail-space
 * octree and frees the slot; then cRailManNode_ActiveNode (On Tour ActiveNode) is cleared and the cSortObjNode destructor
 * runs.
 *
 * @param flags  g++ destructor flags, passed on to the cSortObjNode destructor (bit 0 frees the memory)
 */
extern "C" void cRailManNode__cRailManNode(cRailManNode* self, int flags)
{
    int i;
    VPTR_OF(cObjNode, self) = cRailManNode_vtbl;
    for (i = 0; i < 0x80; i++) {
        if (self->RailInfo[i].RailRID != tResourceID::Invalid()) {
            self->enableRail(i);
        }
    }
    cRailManNode_ActiveNode = 0;
    cSortObjNode__cSortObjNode_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_unloadSection);
#ifdef SKIP_ASM
extern void* cRailManNode_ActiveNode;
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx);

struct sMpSlot_8700 {
    unsigned char kind;
    unsigned char pad[3];
};

struct sMpMgr_8700 {
    char pad[0x18];
    sMpSlot_8700 slots[128];
};

/**
 * Re-enables every disabled rail of a world section and frees its slot.
 *
 * With an active rail manager (cRailManNode_ActiveNode), each used slot whose id has this section calls cRailManNode_enableRail.
 * cGame_sectionUnloading calls it with the unloading section.
 *
 * @param kind  world section id
 */
extern "C" void cRailManNode_unloadSection(int kind)
{
    int i;
    if (cRailManNode_ActiveNode != 0) {
        for (i = 0; i < 0x80; i++) {
            sMpMgr_8700* railMan = (sMpMgr_8700*)cRailManNode_ActiveNode;
            if (((unsigned int*)((char*)railMan + 0x18))[i] != tResourceID::kInvalidRID
                && railMan->slots[i].kind == kind) {
                ((cRailManNode*)railMan)->enableRail(i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_disableRail);
#ifdef SKIP_ASM
struct sMpCell_8780 {
    int level;
    int x, y, z;
};
struct sMpNode_8780 {
    sMpNode_8780* child[2][2][2];   // 0x00
    void* lists[3];                 // 0x20
    int empty()
    {
        if (child[0][0][0] || child[0][0][1] || child[0][1][0] || child[0][1][1] || child[1][0][0] ||
            child[1][0][1] || child[1][1][0] || child[1][1][1]) {
            return 0;
        }
        for (int i = 0; i < 3; i++) {
            if (lists[i]) return 0;
        }
        return 1;
    }
};
struct sMpRoot_8780 {
    sMpCell_8780 cell;
    sMpNode_8780* node;
};
extern "C" cWorld* cObjectInterface_getWorld();
extern "C" char* cObjectInterface_getNash(void);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Cell_remnode(void* node, int idx, void* item, const void* target, const void* cur);
void operator_delete(int*);

static inline void* refToPtr_8780(unsigned int p)
{
    return (void*)(p << 2);
}

// The record in slot uid of a section table's type list: the tag keeps its address >> 2 (0 = not loaded).
static inline void* lookup_8780(cWorldCacheTable* table, int type, unsigned int uid)
{
    unsigned int p = table->m_pCacheTable[type].m_pDataTag[uid].m_bfuiMemLoc;
    if (p == 0) {
        return 0;
    }
    return refToPtr_8780(p);
}

// The spline record @p rid names (cache type 8), or 0 when its section or record is not loaded.
static inline tSplineRecord* splineRecord_8780(const tResourceID& rid, cWorld* world)
{
    unsigned int id = rid.m_uiRID;
    cWorldCacheTable* table = world->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    if (table == 0) {
        return 0;
    }
    return (tSplineRecord*)lookup_8780(table, 8, id >> 8);
}

static inline sMpRoot_8780* findRoot_8780(char* w, int x, int y, int z)
{
    sMpRoot_8780* r = (sMpRoot_8780*)w;
    sMpRoot_8780* rx = x >= 0 ? r : r + 4;
    sMpRoot_8780* ry = y >= 0 ? rx : rx + 2;
    sMpRoot_8780* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_8780(char* vw, void* item, const tBoundBox* box)
{
    cBXNash* w = (cBXNash*)vw;
    (*(int*)&w->deletecount)++;
    sMpCell_8780 c;
    nash_Nash_findkey(&c, box);
    sMpRoot_8780* r = findRoot_8780(((char*)w), c.x, c.y, c.z);
    nash_Cell_remnode(r->node, 2, item, &c, &r->cell);
    if (r->node->empty()) {
        operator_delete((int*)r->node);
        r->node = 0;
    }
}

/**
 * Takes a rail out of collision by removing its segments from the world space hash.
 *
 * Looks up the spline of rail slot idx (+0x18) and, for each of its segments (next +0x64), removes the segment from the
 * space hash using its box (+0x6C..+0x80): nash_Nash_findkey, nash_Cell_remnode on list 2, and deletes an octant that
 * becomes empty.
 *
 * @param idx  rail slot index
 */
// Removes every particle node of slot 'idx' from the world's spatial tree.
void cRailManNode::disableRail(int idx)
{
    tResourceID ref = RailInfo[idx].RailRID;
    tSplineRecord* spline = splineRecord_8780(ref, cObjectInterface_getWorld());
    tSplineSegmentRecord* node = spline->m_HeaderSeg;
    for (int i = 0; i < spline->m_iNumSegments; i++) {
        tBoundBox box;
        (*(&box.MinPoint)).x = node->m_fvMin.m[0];
        (*(&box.MinPoint)).y = node->m_fvMin.m[1];
        (*(&box.MinPoint)).z = node->m_fvMin.m[2];
        (*(&box.MinPoint)).w = 1.0f;
        (*(&box.MaxPoint)).x = node->m_fvMax.m[0];
        (*(&box.MaxPoint)).y = node->m_fvMax.m[1];
        (*(&box.MaxPoint)).z = node->m_fvMax.m[2];
        (*(&box.MaxPoint)).w = 1.0f;
        remove_8780(cObjectInterface_getNash(), node, &box);
        node = (tSplineSegmentRecord*)node->m_NextSeg;
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_enableRail);
#ifdef SKIP_ASM
struct sMpCell_8998 {
    int level;
    int x, y, z;
};
struct sMpRoot_8998 {
    sMpCell_8998 cell;
    void* node;
};
extern "C" cWorld* cObjectInterface_getWorld();
extern "C" char* cObjectInterface_getNash(void);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Octant_addnode(void* root, int idx, void* item, const void* target);

static inline void* refToPtr_8998(unsigned int p)
{
    return (void*)(p << 2);
}

// The record in slot uid of a section table's type list: the tag keeps its address >> 2 (0 = not loaded).
static inline void* lookup_8998(cWorldCacheTable* table, int type, unsigned int uid)
{
    unsigned int p = table->m_pCacheTable[type].m_pDataTag[uid].m_bfuiMemLoc;
    if (p == 0) {
        return 0;
    }
    return refToPtr_8998(p);
}

// The spline record @p rid names (cache type 8), or 0 when its section or record is not loaded.
static inline tSplineRecord* splineRecord_8998(const tResourceID& rid, cWorld* world)
{
    unsigned int id = rid.m_uiRID;
    cWorldCacheTable* table = world->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    if (table == 0) {
        return 0;
    }
    return (tSplineRecord*)lookup_8998(table, 8, id >> 8);
}

static inline sMpRoot_8998* findRoot_8998(char* w, int x, int y, int z)
{
    sMpRoot_8998* r = (sMpRoot_8998*)w;
    sMpRoot_8998* rx = x >= 0 ? r : r + 4;
    sMpRoot_8998* ry = y >= 0 ? rx : rx + 2;
    sMpRoot_8998* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void insert_8998(char* w, void* item, const tBoundBox* box)
{
    sMpCell_8998 c;
    nash_Nash_findkey(&c, box);
    nash_Octant_addnode(findRoot_8998(w, c.x, c.y, c.z), 2, item, &c);
}

/**
 * Puts a rail back into collision by reinserting its segments into the world space hash, then frees its slot.
 *
 * For each segment of the spline in slot idx it adds the segment to the space hash by its box (nash_Octant_addnode,
 * list 2), then sets the slot to -1.
 *
 * @param idx  rail slot index
 */
// Re-inserts every particle node of slot 'idx' into the world's spatial tree, then frees the slot.
void cRailManNode::enableRail(int idx)
{
    tResourceID ref = RailInfo[idx].RailRID;
    tSplineRecord* spline = splineRecord_8998(ref, cObjectInterface_getWorld());
    tSplineSegmentRecord* node = spline->m_HeaderSeg;
    for (int i = 0; i < spline->m_iNumSegments; i++) {
        tBoundBox box;
        (*(&box.MinPoint)).x = node->m_fvMin.m[0];
        (*(&box.MinPoint)).y = node->m_fvMin.m[1];
        (*(&box.MinPoint)).z = node->m_fvMin.m[2];
        (*(&box.MinPoint)).w = 1.0f;
        (*(&box.MaxPoint)).x = node->m_fvMax.m[0];
        (*(&box.MaxPoint)).y = node->m_fvMax.m[1];
        (*(&box.MaxPoint)).z = node->m_fvMax.m[2];
        (*(&box.MaxPoint)).w = 1.0f;
        insert_8998(cObjectInterface_getNash(), node, &box);
        node = (tSplineSegmentRecord*)node->m_NextSeg;
    }
    RailInfo[idx].RailRID = tResourceID::Invalid();
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_addRail);
#ifdef SKIP_ASM

extern void* cRailManNode_ActiveNode;
extern cRailManNode* cRailManNode_ActiveNode_mgr8B28 __asm__("cRailManNode_ActiveNode");
extern char str_RailMan_b[];
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern "C" void* cRailManNode_cRailManNode1(cRailManNode* self, void* a1);
extern "C" void cRailManNode_disableRail(cRailManNode* self, int idx);
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx);

/**
 * Switches a rail (spline) off or back on, creating the rail manager on first use.
 *
 * Creates the manager if cRailManNode_ActiveNode is 0 (536 bytes "RailMan", bucket 1). Finds the slot holding key, or else the first
 * free one. Switching off a rail that is already off, or on a rail that is not off, does nothing; otherwise the slot
 * takes key and cRailManNode_enableRail (on) or cRailManNode_disableRail (off) runs. Nothing happens when all 128 slots are used.
 *
 * @param key  spline id of the rail
 * @param add  nonzero to switch the rail back on, 0 to switch it off
 */
extern "C" void cRailManNode_addRail(tResourceID key, int add)
{
    if (cRailManNode_ActiveNode == 0) {
        ((cRailManNode*)cMemMan_alloc(536, str_RailMan_b, 0x20000000, 0))->cRailManNode1((void*)1);
    }
    tRailInfo kFree;
    int idx = cRailManNode::kNoSlot;
    for (int i = 0; i < 128; i++) {
        if (idx == cRailManNode::kNoSlot && cRailManNode_ActiveNode_mgr8B28->RailInfo[i].RailRID == kFree.RailRID) {
            idx = i;
        }
        if (cRailManNode_ActiveNode_mgr8B28->RailInfo[i].RailRID == key) {
            idx = i;
            goto found;
        }
    }
found:
    cRailManNode* mgr = cRailManNode_ActiveNode_mgr8B28;
    if (idx != cRailManNode::kNoSlot) {
        tRailInfo& railSlot = mgr->RailInfo[idx];
        if (railSlot.RailRID == key) {
            if (add == 0) {
                return;
            }
        } else if (add != 0) {
            return;
        }
        mgr->RailInfo[idx].RailRID = key;
        if (add != 0) {
            mgr->enableRail(idx);
        } else {
            mgr->disableRail(idx);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cSortObjNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a rail-manager node's state to a replay frame.
 *
 * Writes the sort-object part with cSortObjNode_writeToReplayFrame(), then the 0x200 bytes at offset 0x18
 * through the stream's write virtual (slot 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cRailManNode_writeToReplayFrame(cRailManNode* self, cBxReplayFrame* stream)
{
    cSortObjNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->RailInfo, 0x200);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cAnimComboNode1);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: same symbol as the unit's cAnimObjectNode_cAnimObjectNode1(void*, int, int, int, int); pointer-typed view.
extern "C" void cAnimObjectNode_cAnimObjectNode1_8C80(void* self, int a, int type, void* b, const void* c) __asm__("cAnimObjectNode_cAnimObjectNode1");
extern char cAnimComboNode_vtbl[];
extern char str_ComboXform[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sMPElem8C80 {
    char pad_0x0[0x40];
    sMPElem8C80() {}
};

// One-member union view of a float field (see the FAKEMATCH note on cAnimComboNode_cAnimComboNode1).
union uFloat8C80 { float v; };




/**
 * Constructs an anim combo node: an animated object that can switch to a second time range on a signal.
 *
 * Builds the cAnimObjectNode part at +0x18 (cAnimObjectNode_cAnimObjectNode1, node type 3) and installs cAnimComboNode_vtbl. From adata: the second
 * range's start (+0x8) = FromTime2, or ToTime when FromTime2 is negative, in seconds/30; its end (+0xC) = ToTime2 / 30,
 * or the animation length when negative; step (+0x0) = Speed2 / 30 / frame rate; Stop flags (+0x16 Stop != 0, +0x17
 * Stop < 0). Allocates the "ComboXform" matrices (+0x84, one per animated mesh) and clears the active/stopped bytes and
 * the current time.
 *
 * @param bucket  update bucket
 * @param pInstance  instance record
 * @param adata  anim combo make-data (tMakeAnimComboData)
 * @return self
 */
// FAKEMATCH: the fC (life) store goes through a one-member union view (uFloat8C80), which makes it MEM_IN_STRUCT so sched1
// no longer keeps the TheAppMan (cAppMan_TheAppMan) load after it; plain form 91.43%. Byte-identical and behaves like the plain form;
// landed marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest
// spelling matches.
// cAnimComboNode::cAnimComboNode(int bucket, tInstanceRecord* pInstance, const tMakeAnimComboData* adata)
// (SSX On Tour symbols; frame counts are scaled by 1/30, and f0 is then divided by the cAppMan tick rate like ticks2secs).
extern "C" void* cAnimComboNode_cAnimComboNode1(cAnimComboNode* self, int bucket, tInstanceRecord* pInstance, const tMakeAnimComboData* adata)
{
    cAnimObjectNode_cAnimObjectNode1_8C80((char*)&self->AnimType, bucket, 3, pInstance, adata);
    self->mVtbl = (void**)cAnimComboNode_vtbl;
    float* rate = &self->FromTime2;
    tModelRecord* pModel = (tModelRecord*)pInstance->m_Model;
    *rate = (adata->FromTime2 < 0.0f ? adata->ToTime : adata->FromTime2) * 0.03333333507180214f;
    uFloat8C80* life = (uFloat8C80*)&self->ToTime2;
    life->v = adata->ToTime2 < 0.0f ? pModel->m_fAnimTime : adata->ToTime2 * 0.03333333507180214f;
    int fps = cAppMan_TheAppMan->TimerHz;
    self->AnimSpeed2 = adata->Speed2 * 0.03333333507180214f / (float)fps;
    self->Stop = adata->Stop != 0;
    self->StopInPlace = adata->Stop < 0;
    int n = self->NumAnimMeshes;
    sMPElem8C80** slot = (sMPElem8C80**)&self->ComboXform;
    *slot = new (operator_new_tag(n << 6, str_ComboXform, 0x20000000, 0)) sMPElem8C80[n];
    self->AnimActive2 = 0;
    self->AnimComplete2 = 0;
    *(int*)&self->CurrTime2 = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cAnimComboNode);
#ifdef SKIP_ASM
extern "C" void* cAnimObjectNode_cAnimObjectNode(void* self, void* a, void* stream);
extern "C" void cAnimComboNode_setComboXform(cAnimComboNode* self);
extern char cAnimComboNode_vtbl[];
extern char str_ComboXform[];

// Element type with an empty constructor; the array new leaves only its loop counters.
struct sParticle_8DB0 {
    char data[0x40];
    sParticle_8DB0() {}
};

/**
 * Rebuilds an animation combo node from a replay frame: its embedded animated-object node, combo state and
 * combo matrices.
 *
 * Runs the cAnimObjectNode replay constructor on +0x18, stores the cAnimComboNode vtable (cAnimComboNode_vtbl) at
 * +0x54 and reads the 0x18-byte cAnimComboNodeState into +0x0. ComboXform (+0x84) gets one 0x40-byte matrix
 * per animated mesh (+0x74) ("ComboXform"), filled by cAnimComboNode_setComboXform (probably On Tour setComboXform).
 *
 * @param bucket  bucket index, passed on to the cAnimObjectNode constructor
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cAnimComboNode_cAnimComboNode(cAnimComboNode* self, void* bucket, cBxReplayFrame* stream)
{
    sParticle_8DB0** pp = (sParticle_8DB0**)(&self->ComboXform);
    cAnimObjectNode_cAnimObjectNode(&self->AnimType, bucket, stream);
    self->mVtbl = (void**)cAnimComboNode_vtbl;
    stream->read((char*)self, 0x18);
    *pp = new (str_ComboXform, 0x20000000, 0) sParticle_8DB0[self->NumAnimMeshes];
    self->setComboXform();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode__cAnimComboNode);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void*);
extern "C" void cAnimObjectNode__cAnimObjectNode(void* p, int flags);
extern char cAnimComboNode_vtbl[];

/**
 * Destroys an anim combo node.
 *
 * Reinstalls cAnimComboNode_vtbl, frees ComboXform (+0x84), destroys the cAnimObjectNode part (cAnimObjectNode__cAnimObjectNode on +0x18) and
 * deletes the node when bit 0 of flags is set.
 *
 * @param flags  destructor flags
 */
extern "C" void cAnimComboNode__cAnimComboNode(cAnimComboNode* self, int flags)
{
    self->mVtbl = (void**)cAnimComboNode_vtbl;
    void* buf = self->ComboXform;
    if (buf != 0) {
        cMemMan_free(buf);
    }
    cAnimObjectNode__cAnimObjectNode((char*)&self->AnimType, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_updateNode);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_updateNode(void* p);
extern "C" int cAnimNode_frameUpdate(void* p);

/**
 * Updates an anim combo node for one frame.
 *
 * Resets the frame-event times (+0x3C, +0x40) to 1e10. When stopped (+0x15) it returns true at once. When the second
 * range is not active (+0x14) it runs the normal animated-object update (cAnimObjectNode_updateNode). When active it
 * counts down the start delay (+0x24); once that is 0 it advances the time (+0x4) by the step, sets the frame events to
 * the current frame, and on reaching the end (+0xC) clamps there, ends the range and marks the node stopped if Stop is
 * set; it returns 0 when the frame-update script (cAnimNode_frameUpdate) says so. Otherwise sets RootTransformDirty
 * (+0x5A) and returns true.
 *
 * @return false when the frame-update script asks to stop updating, else true
 */
bool cAnimComboNode::updateNode()
{
    FrameStart = 10000000000.0f;
    FrameEnd = 10000000000.0f;
    if (*(signed char*)&AnimComplete2 == 0) {
        if (*(signed char*)&AnimActive2 != 0) {
            int countdown = Delay;
            if (countdown >= 0) {
                if (countdown > 0) {
                    Delay = countdown - 1;
                } else {
                    float pos = CurrTime2 + AnimSpeed2;
                    float max = ToTime2;
                    float frameTime = CurrTime;
                    FrameStart = frameTime;
                    FrameEnd = frameTime;
                    CurrTime2 = pos;
                    if (max <= pos) {
                        CurrTime2 = max;
                        if (*(signed char*)&Stop != 0) {
                            *(signed char*)&AnimComplete2 = 1;
                        }
                        *(signed char*)&AnimActive2 = 0;
                    }
                    if (cAnimNode_frameUpdate(&CurrTime) == 0) {
                        return false;
                    }
                }
            }
        } else {
            cAnimObjectNode_updateNode(&AnimType);
            return true;
        }
        Flags |= 1;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_updateIntersectInfo);
#ifdef SKIP_ASM
struct sMpQuad {
    float v[4];
} __attribute__((aligned(16)));

extern sMpQuad t4vect_Zero;
extern "C" void cMoveNode_updateIntersectInfo(void* self, void* out);

/**
 * Fills an intersection with a combo node: the plain cMoveNode result with no velocity.
 *
 * Calls cMoveNode_updateIntersectInfo() on the cMoveNode part (+0x48), then sets the info's velocity (+0x20) and
 * angular velocity (+0x30) to t4vect_Zero: unlike cAnimNode's version, the animation's motion is not passed on.
 *
 * @param out  the intersection info (On Tour tIntersectInfo& intInfo)
 */
void cAnimComboNode::updateIntersectInfo(tIntersectInfo* out)
{
    cMoveNode_updateIntersectInfo(&MoveNodeBase, out);
    *(sMpQuad*)&out->Ivel = t4vect_Zero;
    *(sMpQuad*)&out->Iangvel = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_signal);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_signal(void*);
extern "C" void cAnimComboNode_setComboXform(cAnimComboNode*);

/**
 * Handles a signal on an anim combo node: kAnimComboSignal_Start2 starts the second time range.
 *
 * For kAnimComboSignal_Start2, when neither active nor stopped (+0x14, +0x15), it copies +0x38 to +0x10, snapshots the
 * current mesh matrices (cAnimComboNode_setComboXform), marks the range active and sets the time to its start (+0x8).
 * Any other signal goes to the animated-object handler cAnimObjectNode_signal.
 *
 * @param msg  signal id
 */
void cAnimComboNode::signal(int msg)
{
    if (msg == kAnimComboSignal_Start2) {
        if (*(unsigned short*)&AnimActive2 == 0) {
            ComboTime = CacheTime;
            setComboXform();
            AnimActive2 = 1;
            CurrTime2 = FromTime2;
        }
    } else {
        cAnimObjectNode_signal((char*)&AnimType);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_setComboXform);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrix_9030(void* dst, void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}



/**
 * Copies the current matrices of the instance's animated meshes into the ComboXform buffer (+0x84).
 *
 * Does nothing when the node has no animated meshes. For each model part with anim info it copies the next frame's
 * matrix in order.
 *
 * @param vself  the anim combo node
 */
// Gathers the world matrices of the model's active parts into the matrix buffer at 0x84.
void cAnimComboNode::setComboXform()
{
    cAnimComboNode* self = this;
    if (self->NumAnimMeshes <= 0) {
        return;
    }
    tModelRecord* model = ((self->Instance)->m_Model);
    int n = 0;
    tMeshHeirarchyRecord* part = model->m_MeshHeirarchy;
    for (int i = 0; i < model->m_iNumMeshes; i++, part++) {
        if (part->m_MeshAnim != 0) {
            cAnimNode* animNode = (cAnimNode*)&self->CurrTime; // the cAnimNode part (+0x34)
            vu0CopyMatrix_9030(&(self->ComboXform)[n], &animNode->AnimFrames[n].FrameMatrix);
            n++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_loseFocus);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_loseFocus(void*);

/**
 * Passes loseFocus to the anim-combo node's embedded instance node unless the combo has finished.
 *
 * Calls cInstanceNode_loseFocus() on the node at offset 0x48 only while byte 0x15 is 0; the update sets that
 * byte when the animation reaches its end with byte 0x16 set.
 */
void cAnimComboNode::loseFocus()
{
    if (*(signed char*)&AnimComplete2 == 0) {
        cInstanceNode_loseFocus(&MoveNodeBase);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cAnimObjectNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes an anim combo node to a replay frame: the animated-object part (cAnimObjectNode_writeToReplayFrame on +0x18), then its 0x18-byte
 * combo block.
 *
 * @param obj  replay stream to write to
 */
void cAnimComboNode::writeToReplayFrame(cBxReplayFrame* obj)
{
    cAnimObjectNode_writeToReplayFrame((char*)&AnimType, obj);
    obj->write((const char*)this, 0x18);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cacheFrame);
#ifdef SKIP_ASM


extern "C" void cAnimNode_cacheFrame(void* self);
extern "C" void cMeshAnimFrame_transform(cMeshAnimFrame* item, t4Matrix* mtx);

/**
 * Caches a combo node's animation frame and applies its combo transforms to every animated mesh.
 *
 * Runs cAnimNode_cacheFrame on the cAnimNode part (+0x34) first. If there are frames (+0x7C) and the node is in combo mode
 * (+0x14 set, or both +0x17 and +0x15 set), each mesh of the instance's model (+0x60, model +0x80) that has an
 * animation (part info +0x1C) gets its frame transformed by the next ComboXform matrix (+0x84) with cMeshAnimFrame_transform.
 */
void cAnimComboNode::cacheFrame()
{
    cAnimNode_cacheFrame(&CurrTime);
    if ((AnimFrames) == 0) {
        return;
    }
    if (*(signed char*)&AnimActive2 == 0) {
        if (*(signed char*)&StopInPlace == 0) {
            return;
        }
        if (*(signed char*)&AnimComplete2 == 0) {
            return;
        }
    }
    tModelRecord* model = (Instance->m_Model);
    int n = 0;
    tMeshHeirarchyRecord* meshPart = model->m_MeshHeirarchy;
    for (int i = 0; i < model->m_iNumMeshes; i++, meshPart++) {
        if (meshPart->m_MeshAnim->m_NumChannels != 0) {
            cMeshAnimFrame_transform(&AnimFrames[n], &ComboXform[n]);
            n++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_cAnimDeltaNode1);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_cAnimObjectNode1(void* self, int a, int type, int b, int c);
extern void* cAnimDeltaNode_vtbl[];

/**
 * Constructs an anim delta node (node type 2): an animated object that plays while a delta budget lasts.
 *
 * Builds the cAnimObjectNode base (cAnimObjectNode_cAnimObjectNode1), installs cAnimDeltaNode_vtbl at +0x3C and zeroes Delta (+0x6C) and +0x4.
 *
 * @param bucket  update bucket
 * @param instRecord  instance record
 * @param animData  anim delta make-data
 * @return self
 */
void* cAnimDeltaNode::cAnimDeltaNode1(int bucket, int instRecord, int animData)
{
    cAnimObjectNode_cAnimObjectNode1(this, bucket, 2, instRecord, animData);
    mVtbl = cAnimDeltaNode_vtbl;
    *(int*)&Delta = 0;
    AnimActive = 0;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_cAnimDeltaNode);
#ifdef SKIP_ASM

extern "C" void* cAnimObjectNode_cAnimObjectNode(void* self, void* a, void* stream);
extern void* cAnimDeltaNode_vtbl[];

/**
 * Rebuilds an animation delta node from a replay frame: the cAnimObjectNode part, its vtable and its Delta.
 *
 * Stores the cAnimDeltaNode vtable (cAnimDeltaNode_vtbl) at +0x3C and reads the 4-byte Delta (On Tour +0x68) into
 * +0x6C.
 *
 * @param bucket  bucket index, passed on to the cAnimObjectNode constructor
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cAnimDeltaNode_cAnimDeltaNode(cAnimDeltaNode* self, void* bucket, cBxReplayFrame* stream)
{
    cAnimObjectNode_cAnimObjectNode(self, bucket, stream);
    self->mVtbl = cAnimDeltaNode_vtbl;
    stream->read((char*)&self->Delta, 4);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_updateNode);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_updateNode(void* p);


/**
 * Updates an anim delta node: the animation runs while Delta is positive and Delta drains by the speed.
 *
 * Resets the frame-event times (+0x24, +0x28). Unless the node is complete (+0x8): with Delta (+0x6C) above 0 and the
 * delay (+0xC) at 0, Delta drops by |speed| (+0x10) down to 0; the base update (cAnimObjectNode_updateNode) runs and +0x4 records
 * whether Delta is still positive. With Delta at 0 the delay counts down. RootTransformDirty (+0x42) is set unless the
 * animated branch returned early.
 *
 * Quirk: the branch that expires the node (vtable entry 34) re-tests +0x8, which is already known to be 0, so it never
 * runs.
 *
 * @return false only from that dead expire branch, else true
 */
bool cAnimDeltaNode::updateNode()
{
    FrameStart = 10000000000.0f;
    FrameEnd = 10000000000.0f;
    if (AnimComplete == 0) {
        float curDelta = Delta;
        int countdown = Delay;
        if (curDelta > 0.0f) {
            if (countdown >= 0) {
                if (countdown == 0 && curDelta > 0.0f) {
                    float newDelta = curDelta - __builtin_fabsf(AnimSpeed);
                    Delta = newDelta;
                    if (newDelta < 0.0f) {
                        Delta = 0.0f;
                    }
                }
                cAnimObjectNode_updateNode(((char*)this));
                if (Delta > 0.0f) {
                    AnimActive = 1;
                } else {
                    AnimActive = 0;
                }
                return true;
            }
        } else {
            if (countdown > 0) {
                Delay = countdown - 1;
            }
            if (AnimComplete != 0) {
                char* obj = (char*)&MoveNodeBase;
                sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
                sVEntry<void(void*, int)>* vtEntry = &vt[34];
                vtEntry->fn(vthis(obj, *vtEntry), 1);
                return false;
            }
        }
        Flags |= 1;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_signal);
#ifdef SKIP_ASM
// PORT: cAnimObjectNode_signal really takes (self, id, value); the unit declares it (void*).
extern "C" void cAnimObjectNode_signal_set(void* self, int id, float v) __asm__("cAnimObjectNode_signal");

/**
 * Handles a signal on an anim delta node: kAnimDeltaSignal_AddDelta adds arg / 30 to Delta (+0x6C); others go to
 * cAnimObjectNode_signal.
 *
 * @param id  signal id
 * @param signalArg  signal argument
 */
void cAnimDeltaNode::signal(int id, float signalArg)
{
    if (id == kAnimDeltaSignal_AddDelta) {
        Delta += signalArg * 0.03333333507180214f;
    } else {
        cAnimObjectNode_signal_set(this, id, signalArg);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cAnimObjectNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes an anim delta node to a replay frame: the animated-object part (cAnimObjectNode_writeToReplayFrame), then Delta (+0x6C).
 *
 * @param stream  replay stream to write to
 */
void cAnimDeltaNode::writeToReplayFrame(cBxReplayFrame* stream)
{
    cAnimObjectNode_writeToReplayFrame(this, stream);
    stream->write((const char*)&Delta, 0x4);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cSplineModifier_cSplineModifier1);
#ifdef SKIP_ASM
extern char cSplineModifier_vtbl[];
extern "C" void cSpline_init(void* ref, int id);
extern "C" void cSplineModifier_setSpeed(void* self, float v);
float AIrandf(float lo, float hi);

/**
 * Builds a spline modifier from script data: sets up its spline, speed, start distance and orientation.
 *
 * Sequence, Orient, Heading (degrees, stored in radians), Force (* 100) and ForceTime go to +0x30 to +0x44;
 * the spline is set up from SplineId and the speed through cSplineModifier_setSpeed(). The start distance is
 * 0 (Length for a negative Speed) or a nonzero StartDistance, plus AIrandf(-1, 1) * RandomDistance, reset to
 * 0 when outside [0, Length].
 *
 * Start (+0x54) = Start != 0; Complete, Radius, Dir, DDir, X2 and Y2 are cleared and RootTransformDirty
 * (+0x58) is set.
 *
 * @param desc  the spline data (On Tour const tAddSplineData&)
 * @return @p self
 */
extern "C" void* cSplineModifier_cSplineModifier1(cSplineModifier* self, tAddSplineData* desc)
{
    *(void**)self = cSplineModifier_vtbl;
    self->Spline.SplineId = 0xFFFFFFFF;
    self->Spline.Seg = 0;
    self->Spline.SegIndex = 0;
    *(int*)&self->Spline.Length = 0;
    *(int*)&self->Radius = 0;
    self->SequenceType = desc->Sequence;
    self->OrientType = desc->Orient;
    self->HeadTweak = desc->Heading * 0.01745329424738884f;
    self->Force = desc->Force * 100.0f;
    self->ForceTime = desc->ForceTime;
    cSpline_init(&self->Spline, (int)desc->SplineId);
    cSplineModifier_setSpeed(((char*)self), desc->Speed);
    if (desc->Speed >= 0.0f) {
        self->OffsetDistance = 0.0f;
    } else {
        self->OffsetDistance = self->Spline.Length;
    }
    float zeroF = 0.0f;
    self->Start = desc->Start != 0;
    self->Complete = 0;
    if (desc->StartDistance != zeroF) {
        self->OffsetDistance = desc->StartDistance;
    }
    self->OffsetDistance += AIrandf(-1.0f, 1.0f) * desc->RandomDistance;
    if (self->OffsetDistance < zeroF || self->Spline.Length < self->OffsetDistance) {
        self->OffsetDistance = zeroF;
    }
    *(sMpQuad*)&self->Dir = t4vect_Zero;
    *(sMpQuad*)&self->DDir = t4vect_Zero;
    self->RootTransformDirty = 1;
    *(int*)&self->X2 = 0;
    *(int*)&self->Y2 = 0;
    return ((char*)self);
}
#endif

