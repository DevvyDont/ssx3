#include "common.h"
#include "object/animdeltanode.h"
#include "object/railmannode.h"
#include "object/animcombonode.h"
#include "object/multiparticle.h"
#include "object/splinemodifier.h"
#include "object/physicsmodifier.h"

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
extern char t4mat_Identity[];

// PORT: array new bound to the tagged allocator
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sMPRef {
    unsigned int id;
    sMPRef() : id(0xFFFFFFFF) {}
};

struct sMultiParticle {
    int count;     // 0x0
    int field_0x4; // 0x4
    int field_0x8; // 0x8
    sMPRef* refs;  // 0xC
    void* sys;     // 0x10
};

/**
 * Allocates a multi-particle effect's instance-id table and its static emitter, and sets the emitter up.
 *
 * InstIds (+0xC) becomes MaxInst (+0x0) ids ("InstIds"), all -1; Emitter (+0x10) a 0x190-byte cStaticEmitter
 * ("Emitter", constructed by cStaticEmitter_cStaticEmitter), set up by cStaticEmitter_setupEmitter (On Tour setupEmitter) with an identity
 * transform and the particle data (+0x8).
 */
extern "C" void cMultiParticle_setupMultiParticle(sMultiParticle* self)
{
    sMPRef** slot = &self->refs;
    *slot = new (str_InstIds, 0x20000000, 0) sMPRef[self->count];
    self->sys = cStaticEmitter_cStaticEmitter(cMemMan_alloc(400, str_Emitter, 0x20000000, 0));
    for (int i = 0; i < self->count; i++) {
        self->refs[i].id = 0xFFFFFFFF;
    }
    cStaticEmitter_setupEmitter(self->sys, t4mat_Identity, self->field_0x8);
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
extern "C" void cMultiParticle_update(cMultiParticle* self)
{
    cStaticEmitter_update(self->Emitter, 0.01666666753590107f);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_unloadSection);
#ifdef SKIP_ASM
struct sMoveList {
    int field_0x0;          // 0x0
    int count;              // 0x4
    int field_0x8;          // 0x8
    unsigned int* entries;  // 0xC
};

/**
 * Drops a world section's instances from a multi-particle system.
 *
 * Removes every id in InstIds (+0xC, NumInst +0x4) whose low byte is the section, moving the last id into the hole and
 * rechecking it; a removed last entry becomes -1.
 *
 * @param key  world section id
 */
extern "C" void cMultiParticle_unloadSection(sMoveList* self, unsigned int key)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if ((self->entries[i] & 0xFF) == key) {
            self->count--;
            if (i < self->count) {
                self->entries[i] = self->entries[self->count];
                i--;
            } else {
                self->entries[i] = 0xFFFFFFFF;
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
struct sMoveList_81F0 {
    int field_0x0;          // 0x0
    int count;              // 0x4
    int field_0x8;          // 0x8
    unsigned int* entries;  // 0xC
};

/**
 * Removes an instance id from a multi-particle system.
 *
 * Finds the first matching id in InstIds, decrements NumInst and moves the last id into its place (or sets the slot to
 * -1 when it was last).
 *
 * @param key  instance id to remove
 */
// Removes the first entry equal to key, filling the hole with the last entry.
extern "C" void cMultiParticle_remInst(sMoveList_81F0* self, unsigned int key)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (self->entries[i] == key) {
            self->count--;
            // the do/while(0) wrapper (a compiled-out macro block) is needed to match register allocation
            do {
                if (i < self->count) {
                    self->entries[i] = self->entries[self->count];
                } else {
                    self->entries[i] = 0xFFFFFFFF;
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
struct sMPVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMPBox {
    sMPVec4 min;
    sMPVec4 max;
};

struct sMPModelSet {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sMPWorld {
    char pad_0x0[0x8];
    sMPModelSet** sets;     // 0x8
};

extern "C" sMPWorld** cObjectInterface_getWorld();
extern "C" void cStaticEmitter_render(void* desc, sMPBox* box, const sMPVec4& center);

static inline void* refToPtr58260(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup58260(sMPModelSet* set, unsigned int idx)
{
    unsigned int p = set->refs[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr58260(p);
}

struct sMPModelRef {
    unsigned int id;

    void* get(sMPWorld* w)
    {
        sMPModelSet* set = w->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        return lookup58260(set, id >> 8);
    }
};

struct sMPModel {
    char pad_0x0[0x40];
    sMPVec4 pos;            // 0x40
    sMPVec4 getPos() { return pos; }
};

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void vu0AddTo58260(sMPVec4& dst, sMPVec4 b)
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
static inline sMPVec4 vu0Add58260(const sMPVec4& a, const sMPVec4& b)
{
    sMPVec4 r;
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

struct sMultiPart58260 {
    void* owner;            // 0x0
    int count;              // 0x4
    void* data;             // 0x8
    sMPModelRef* refs;      // 0xC
    void* desc;             // 0x10
    char pad_0x14[0xC];
    sMPBox box;             // 0x20
    sMPVec4 center;         // 0x40
};

/**
 * Draws the multi-particle effect once at each loaded instance it is attached to.
 *
 * For each of the NumInst (+0x4) ids in InstIds (+0xC) whose instance record is loaded in the world, calls
 * cStaticEmitter_render() with BoundBox (+0x20) moved by the record's position (record +0x40), drawing at
 * that position plus Offset (+0x40).
 */
extern "C" void cMultiParticle_render(sMultiPart58260* self)
{
    for (int i = 0; i < self->count; i++) {
        sMPModel* model = (sMPModel*)self->refs[i].get(*cObjectInterface_getWorld());
        if (model != 0) {
            sMPBox bounds = self->box;
            vu0AddTo58260(bounds.min, model->pos);
            vu0AddTo58260(bounds.max, model->pos);
            cStaticEmitter_render(self->desc, &bounds, vu0Add58260(model->getPos(), self->center));
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cMultiParticle_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_58380 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};


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
extern "C" void cMultiParticle_writeToReplayFrame(cMultiParticle* self, void* stream)
{
    sSerVEntry_58380* writeEntry = &(*(sSerVEntry_58380**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (self), 4);
    writeEntry = &(*(sSerVEntry_58380**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, &self->NumInst, 4);
    writeEntry = &(*(sSerVEntry_58380**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, self->Particle, 216);
    cStaticEmitter_writeToReplayFrame(self->Emitter, stream);
    writeEntry = &(*(sSerVEntry_58380**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)&self->Offset, 0x10);
    writeEntry = &(*(sSerVEntry_58380**)stream)[1];
    writeEntry->fn((char*)stream + writeEntry->delta, (char*)&self->BoundBox, 0x20);
    if (self->NumInst > 0) {
        writeEntry = &(*(sSerVEntry_58380**)stream)[1];
        writeEntry->fn((char*)stream + writeEntry->delta, (int*)self->InstIds, self->NumInst << 2);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cPhysicsModifier_cPhysicsModifier);
#ifdef SKIP_ASM
// Same stream-like class as cPhysicsModifier_writeToReplayFrame_cObj; v01 sits at vtable offset 8, v02 at 0x10.
class cPhysicsModifier_cPhysicsModifier_cObj {
public:
    virtual void v01(void* buf, int size);
    virtual void read(void* buf, int size);
};

extern char cPhysicsModifier_vtbl[];

/**
 * Constructs a physics modifier from a replay frame: installs cPhysicsModifier_vtbl and reads its first 4 bytes.
 *
 * @param obj  replay stream (its read slot, vtable entry 2)
 * @return self
 */
extern "C" void* cPhysicsModifier_cPhysicsModifier(cPhysicsModifier* self, cPhysicsModifier_cPhysicsModifier_cObj* obj)
{
    self->mVtbl = (void**)cPhysicsModifier_vtbl;
    obj->read(self, 4);
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
extern "C" void cPhysicsModifier_writeToReplayFrame(cPhysicsModifier* self, cPhysicsModifier_writeToReplayFrame_cObj* obj)
{
    obj->write(self, 4);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_cRailManNode1);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern void* cRailManNode_vtbl[];
extern "C" void* cSortObjNode_cSortObjNode1(void* self, void* a1, int type);

/**
 * Constructs the rail manager node (node type 18) and makes it the active one.
 *
 * Runs the sort-node constructor cSortObjNode_cSortObjNode1, installs cRailManNode_vtbl, sets all 128 rail slots (+0x18) to -1,
 * stores itself in D_004A4040 (the active rail manager) and clears the slots again.
 *
 * @param bucket  update bucket
 * @return self
 */
extern "C" void* cRailManNode_cRailManNode1(cRailManNode* self, void* bucket)
{
    int i;
    cSortObjNode_cSortObjNode1(self, bucket, 18);
    self->mVtbl = cRailManNode_vtbl;
    unsigned int* railSlot = (unsigned int*)(&self->RailInfo[0]);
    for (i = 0x7F; i != -1; i--, railSlot++) {
        *railSlot = 0xFFFFFFFF;
    }
    D_004A4040 = ((void*)self);
    for (i = 0; i < 0x80; i++) {
        ((unsigned int*)(&self->RailInfo[0]))[i] = 0xFFFFFFFF;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_cRailManNode);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern void* cRailManNode_vtbl[];
extern "C" void* cSortObjNode_cSortObjNode(void* self, void* a1, void* stream);
extern "C" void cRailManNode_disableRail(cRailManNode* self, int idx);

class cRailManNode_cRailManNode_cStream {
public:
    virtual void v01();
    virtual void read(void* buf, int size);
};

/**
 * Rebuilds the rail manager node from a replay frame and disables again every rail it had disabled.
 *
 * After the cSortObjNode replay constructor and its vtable, the 128 rail slots (+0x18, On Tour RailInfo) are
 * set to -1, the node becomes the active one (D_004A4040, On Tour ActiveNode) and the 0x200 bytes of slots
 * are read from the frame. cRailManNode_disableRail (On Tour disableRail) takes each used slot's spline segments out of
 * the Nash rail-space octree.
 *
 * @param bucket  bucket index, passed on to the cSortObjNode constructor
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cRailManNode_cRailManNode(cRailManNode* self, void* bucket, cRailManNode_cRailManNode_cStream* stream)
{
    int i;
    int j;
    cSortObjNode_cSortObjNode(self, bucket, stream);
    self->mVtbl = cRailManNode_vtbl;
    unsigned int* slot = (unsigned int*)(&self->RailInfo[0]);
    for (j = 0x7F; j != -1; j--, slot++) {
        *slot = 0xFFFFFFFF;
    }
    D_004A4040 = ((void*)self);
    stream->read(&self->RailInfo[0], 0x200);
    for (i = 0; i < 0x80; i++) {
        if (((unsigned int*)(&self->RailInfo[0]))[i] != 0xFFFFFFFF) {
            cRailManNode_disableRail(self, i);
        }
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode__cRailManNode);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern void* cRailManNode_vtbl[];
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx);
// PORT: cSortObjNode__cSortObjNode__FPv is the ObjNode deleting dtor; it takes (self, flags).
extern void* cSortObjNode__cSortObjNode_dtor(void* self, int flags) __asm__("cSortObjNode__cSortObjNode__FPv");

/**
 * Destroys the rail manager node: re-enables every rail it disabled and clears the active-node pointer.
 *
 * cRailManNode_enableRail (On Tour enableRail) puts each used slot's spline segments back into the Nash rail-space
 * octree and frees the slot; then D_004A4040 (On Tour ActiveNode) is cleared and the cSortObjNode destructor
 * runs.
 *
 * @param flags  g++ destructor flags, passed on to the cSortObjNode destructor (bit 0 frees the memory)
 */
extern "C" void cRailManNode__cRailManNode(cRailManNode* self, int flags)
{
    int i;
    self->mVtbl = cRailManNode_vtbl;
    for (i = 0; i < 0x80; i++) {
        if (((unsigned int*)(&self->RailInfo[0]))[i] != 0xFFFFFFFF) {
            cRailManNode_enableRail(self, i);
        }
    }
    D_004A4040 = 0;
    cSortObjNode__cSortObjNode_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_00358700);
#ifdef SKIP_ASM
extern void* D_004A4040;
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
 * With an active rail manager (D_004A4040), each used slot whose id has this section calls cRailManNode_enableRail.
 * cGame_sectionUnloading calls it with the unloading section.
 *
 * @param kind  world section id
 */
extern "C" void func_00358700(int kind)
{
    int i;
    if (D_004A4040 != 0) {
        for (i = 0; i < 0x80; i++) {
            sMpMgr_8700* railMan = (sMpMgr_8700*)D_004A4040;
            if (((unsigned int*)((char*)railMan + 0x18))[i] != 0xFFFFFFFF && railMan->slots[i].kind == kind) {
                cRailManNode_enableRail((cRailManNode*)railMan, i);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_disableRail);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMpVec4_8780 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sMpBox_8780 {
    sMpVec4_8780 min;
    sMpVec4_8780 max;
};
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
struct sMpPart_8780 {
    char pad[0x20];
    int count;             // 0x20
    char* first;           // 0x24
};
struct sMpTable_8780 {
    char pad[0x44];
    unsigned int* entries; // 0x44
};
struct sMpMgr_8780 {
    char pad[0x8];
    sMpTable_8780** tables; // 0x8
};

extern "C" sMpMgr_8780** cObjectInterface_getWorld_8780(void) __asm__("cObjectInterface_getWorld");
extern "C" char* cObjectInterface_getNash(void);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Cell_remnode(void* node, int idx, void* item, const void* target, const void* cur);
void operator_delete(int*);

static inline void* refToPtr_8780(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup_8780(sMpTable_8780* t, unsigned int idx)
{
    unsigned int p = t->entries[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_8780(p);
}

struct sMpRef_8780 {
    unsigned int id;

    void* get(sMpMgr_8780* w)
    {
        sMpTable_8780* t = w->tables[id & 0xFF];
        if (t == 0) {
            return 0;
        }
        return lookup_8780(t, id >> 8);
    }
};

static inline sMpRoot_8780* findRoot_8780(char* w, int x, int y, int z)
{
    sMpRoot_8780* r = (sMpRoot_8780*)w;
    sMpRoot_8780* rx = x >= 0 ? r : r + 4;
    sMpRoot_8780* ry = y >= 0 ? rx : rx + 2;
    sMpRoot_8780* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void remove_8780(char* w, void* item, const sMpBox_8780* box)
{
    (*(int*)(w + 0xA0))++;
    sMpCell_8780 c;
    nash_Nash_findkey(&c, box);
    sMpRoot_8780* r = findRoot_8780(w, c.x, c.y, c.z);
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
extern "C" void cRailManNode_disableRail(cRailManNode* self, int idx)
{
    sMpRef_8780 ref = ((sMpRef_8780*)&self->RailInfo)[idx];
    sMpPart_8780* part = (sMpPart_8780*)ref.get(*cObjectInterface_getWorld_8780());
    char* node = part->first;
    for (int i = 0; i < part->count; i++) {
        sMpBox_8780 box;
        box.min.x = *(float*)(node + 0x6C);
        box.min.y = *(float*)(node + 0x70);
        box.min.z = *(float*)(node + 0x74);
        box.min.w = 1.0f;
        box.max.x = *(float*)(node + 0x78);
        box.max.y = *(float*)(node + 0x7C);
        box.max.z = *(float*)(node + 0x80);
        box.max.w = 1.0f;
        remove_8780(cObjectInterface_getNash(), node, &box);
        node = *(char**)(node + 0x64);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_enableRail);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMpVec4_8998 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sMpBox_8998 {
    sMpVec4_8998 min;
    sMpVec4_8998 max;
};
struct sMpCell_8998 {
    int level;
    int x, y, z;
};
struct sMpRoot_8998 {
    sMpCell_8998 cell;
    void* node;
};
struct sMpPart_8998 {
    char pad[0x20];
    int count;             // 0x20
    char* first;           // 0x24
};
struct sMpTable_8998 {
    char pad[0x44];
    unsigned int* entries; // 0x44
};
struct sMpMgr_8998 {
    char pad[0x8];
    sMpTable_8998** tables; // 0x8
};

extern "C" sMpMgr_8998** cObjectInterface_getWorld_8998(void) __asm__("cObjectInterface_getWorld");
extern "C" char* cObjectInterface_getNash(void);
extern "C" void nash_Nash_findkey(void* out, const void* box);
extern "C" void nash_Octant_addnode(void* root, int idx, void* item, const void* target);

static inline void* refToPtr_8998(unsigned int p)
{
    return (void*)(p << 2);
}

static inline void* lookup_8998(sMpTable_8998* t, unsigned int idx)
{
    unsigned int p = t->entries[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_8998(p);
}

struct sMpRef_8998 {
    unsigned int id;

    void* get(sMpMgr_8998* w)
    {
        sMpTable_8998* t = w->tables[id & 0xFF];
        if (t == 0) {
            return 0;
        }
        return lookup_8998(t, id >> 8);
    }
};

static inline sMpRoot_8998* findRoot_8998(char* w, int x, int y, int z)
{
    sMpRoot_8998* r = (sMpRoot_8998*)w;
    sMpRoot_8998* rx = x >= 0 ? r : r + 4;
    sMpRoot_8998* ry = y >= 0 ? rx : rx + 2;
    sMpRoot_8998* rz = z >= 0 ? ry : ry + 1;
    return rz;
}

static inline void insert_8998(char* w, void* item, const sMpBox_8998* box)
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
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx)
{
    sMpRef_8998 ref = ((sMpRef_8998*)&self->RailInfo)[idx];
    sMpPart_8998* part = (sMpPart_8998*)ref.get(*cObjectInterface_getWorld_8998());
    char* node = part->first;
    for (int i = 0; i < part->count; i++) {
        sMpBox_8998 box;
        box.min.x = *(float*)(node + 0x6C);
        box.min.y = *(float*)(node + 0x70);
        box.min.z = *(float*)(node + 0x74);
        box.min.w = 1.0f;
        box.max.x = *(float*)(node + 0x78);
        box.max.y = *(float*)(node + 0x7C);
        box.max.z = *(float*)(node + 0x80);
        box.max.w = 1.0f;
        insert_8998(cObjectInterface_getNash(), node, &box);
        node = *(char**)(node + 0x64);
    }
    ((unsigned int*)((char*)&self->RailInfo))[idx] = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_00358B28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMPRef8B28 {
    unsigned int id;
    sMPRef8B28() : id(0xFFFFFFFF) {}
};

struct sMPMgr8B28 {
    char pad_0x0[0x18];
    sMPRef8B28 slots[128];          // 0x18
};

extern void* D_004A4040;
extern sMPMgr8B28* D_004A4040_mgr8B28 __asm__("D_004A4040");
extern char str_RailMan_b[];
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern "C" void* cRailManNode_cRailManNode1(cRailManNode* self, void* a1);
extern "C" void cRailManNode_disableRail(cRailManNode* self, int idx);
extern "C" void cRailManNode_enableRail(cRailManNode* self, int idx);

/**
 * Switches a rail (spline) off or back on, creating the rail manager on first use.
 *
 * Creates the manager if D_004A4040 is 0 (536 bytes "RailMan", bucket 1). Finds the slot holding key, or else the first
 * free one. Switching off a rail that is already off, or on a rail that is not off, does nothing; otherwise the slot
 * takes key and cRailManNode_enableRail (on) or cRailManNode_disableRail (off) runs. Nothing happens when all 128 slots are used.
 *
 * @param key  spline id of the rail
 * @param add  nonzero to switch the rail back on, 0 to switch it off
 */
extern "C" void func_00358B28(sMPRef8B28 key, int add)
{
    if (D_004A4040 == 0) {
        cRailManNode_cRailManNode1((cRailManNode*)cMemMan_alloc(536, str_RailMan_b, 0x20000000, 0), (void*)1);
    }
    sMPRef8B28 kFree;
    int idx = -1;
    for (int i = 0; i < 128; i++) {
        if (idx == -1 && D_004A4040_mgr8B28->slots[i].id == kFree.id) {
            idx = i;
        }
        if (D_004A4040_mgr8B28->slots[i].id == key.id) {
            idx = i;
            goto found;
        }
    }
found:
    sMPMgr8B28* mgr = D_004A4040_mgr8B28;
    if (idx != -1) {
        sMPRef8B28& railSlot = mgr->slots[idx];
        if (railSlot.id == key.id) {
            if (add == 0) {
                return;
            }
        } else if (add != 0) {
            return;
        }
        mgr->slots[idx] = key;
        if (add != 0) {
            cRailManNode_enableRail((cRailManNode*)mgr, idx);
        } else {
            cRailManNode_disableRail((cRailManNode*)mgr, idx);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cRailManNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_00358C30 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cSortObjNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a rail-manager node's state to a replay frame.
 *
 * Writes the sort-object part with cSortObjNode_writeToReplayFrame(), then the 0x200 bytes at offset 0x18
 * through the stream's write virtual (slot 1).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cRailManNode_writeToReplayFrame(cRailManNode* self, void* stream)
{
    cSortObjNode_writeToReplayFrame(self, stream);
    sSerVEntry_00358C30* vt = *(sSerVEntry_00358C30**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)&self->RailInfo, 0x200);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cAnimComboNode1);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: same symbol as the unit's cAnimObjectNode_cAnimObjectNode1(void*, int, int, int, int); pointer-typed view.
extern "C" void cAnimObjectNode_cAnimObjectNode1_8C80(void* self, int a, int type, void* b, const void* c) __asm__("cAnimObjectNode_cAnimObjectNode1");
extern char D_0048EC28[];
extern char str_ComboXform[];
extern char* cAppMan_TheAppMan;

inline void* operator new[](unsigned int, void* p) { return p; }

struct sMPElem8C80 {
    char pad_0x0[0x40];
    sMPElem8C80() {}
};

// One-member union view of a float field (see the FAKEMATCH note on cAnimComboNode_cAnimComboNode1).
union uFloat8C80 { float v; };

struct sMPDesc8C80 {
    char pad_0x0[0x10];
    float f10;              // 0x10
    char pad_0x14[0x14];
    float f28;              // 0x28
    float f2C;              // 0x2C
    float f30;              // 0x30
    int f34;                // 0x34
};

struct sMPOwner8C80 {
    char pad_0x0[0x80];
    char* p80;              // 0x80
};

struct sMP8C80 {
    float f0;               // 0x0
    int f4;                 // 0x4
    float f8;               // 0x8
    float fC;               // 0xC
    char pad_0x10[0x4];
    char b14;               // 0x14
    char b15;               // 0x15
    char b16;               // 0x16
    char b17;               // 0x17
    char pad_0x18[0x3C];
    void* vt54;             // 0x54
    char pad_0x58[0x1C];
    int n74;                // 0x74
    char pad_0x78[0xC];
    sMPElem8C80* elems84;   // 0x84
};

/**
 * Constructs an anim combo node: an animated object that can switch to a second time range on a signal.
 *
 * Builds the cAnimObjectNode part at +0x18 (cAnimObjectNode_cAnimObjectNode1, node type 3) and installs D_0048EC28. From adata: the second
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
extern "C" void* cAnimComboNode_cAnimComboNode1(cAnimComboNode* self, int bucket, sMPOwner8C80* pInstance, const sMPDesc8C80* adata)
{
    cAnimObjectNode_cAnimObjectNode1_8C80((char*)&self->AnimType, bucket, 3, pInstance, adata);
    self->mVtbl = (void**)D_0048EC28;
    float* rate = &self->FromTime2;
    char* pModel = pInstance->p80;
    *rate = (adata->f28 < 0.0f ? adata->f10 : adata->f28) * 0.03333333507180214f;
    uFloat8C80* life = (uFloat8C80*)&self->ToTime2;
    life->v = adata->f2C < 0.0f ? *(float*)(pModel + 0x14) : adata->f2C * 0.03333333507180214f;
    int fps = *(int*)(cAppMan_TheAppMan + 0x10);
    self->AnimSpeed2 = adata->f30 * 0.03333333507180214f / (float)fps;
    self->Stop = adata->f34 != 0;
    self->StopInPlace = adata->f34 < 0;
    int n = self->NumAnimMeshes;
    sMPElem8C80** slot = (sMPElem8C80**)&self->ComboXform;
    *slot = new (operator_new_tag(n << 6, str_ComboXform, 0x20000000, 0)) sMPElem8C80[n];
    self->AnimActive2 = 0;
    self->AnimComplete2 = 0;
    *(int*)&self->CurrTime2 = 0;
    return ((sMP8C80*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cAnimComboNode);
#ifdef SKIP_ASM
extern "C" void* cAnimObjectNode_cAnimObjectNode(void* self, void* a, void* stream);
extern "C" void cAnimComboNode_setComboXform(cAnimComboNode* self);
extern char D_0048EC28[];
extern char str_ComboXform[];

struct sStreamVE_8DB0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// Element type with an empty constructor; the array new leaves only its loop counters.
struct sParticle_8DB0 {
    char data[0x40];
    sParticle_8DB0() {}
};

/**
 * Rebuilds an animation combo node from a replay frame: its embedded animated-object node, combo state and
 * combo matrices.
 *
 * Runs the cAnimObjectNode replay constructor on +0x18, stores the cAnimComboNode vtable (D_0048EC28) at
 * +0x54 and reads the 0x18-byte cAnimComboNodeState into +0x0. ComboXform (+0x84) gets one 0x40-byte matrix
 * per animated mesh (+0x74) ("ComboXform"), filled by cAnimComboNode_setComboXform (probably On Tour setComboXform).
 *
 * @param bucket  bucket index, passed on to the cAnimObjectNode constructor
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 * @return @p self
 */
extern "C" void* cAnimComboNode_cAnimComboNode(cAnimComboNode* self, void* bucket, void* stream)
{
    sParticle_8DB0** pp = (sParticle_8DB0**)(&self->ComboXform);
    cAnimObjectNode_cAnimObjectNode(&self->AnimType, bucket, stream);
    self->mVtbl = (void**)D_0048EC28;
    sStreamVE_8DB0* vt = *(sStreamVE_8DB0**)stream;
    vt[2].fn((char*)stream + vt[2].delta, self, 0x18);
    *pp = new (str_ComboXform, 0x20000000, 0) sParticle_8DB0[self->NumAnimMeshes];
    cAnimComboNode_setComboXform(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode__cAnimComboNode);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void*);
extern "C" void cAnimObjectNode__cAnimObjectNode(void* p, int flags);
extern char D_0048EC28[];

/**
 * Destroys an anim combo node.
 *
 * Reinstalls D_0048EC28, frees ComboXform (+0x84), destroys the cAnimObjectNode part (cAnimObjectNode__cAnimObjectNode on +0x18) and
 * deletes the node when bit 0 of flags is set.
 *
 * @param flags  destructor flags
 */
extern "C" void cAnimComboNode__cAnimComboNode(cAnimComboNode* self, int flags)
{
    self->mVtbl = (void**)D_0048EC28;
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
extern "C" int func_0034EBA0(void* p);

/**
 * Updates an anim combo node for one frame.
 *
 * Resets the frame-event times (+0x3C, +0x40) to 1e10. When stopped (+0x15) it returns 1 at once. When the second range
 * is not active (+0x14) it runs the normal animated-object update (cAnimObjectNode_updateNode). When active it counts down the start
 * delay (+0x24); once that is 0 it advances the time (+0x4) by the step, sets the frame events to the current frame,
 * and on reaching the end (+0xC) clamps there, ends the range and marks the node stopped if Stop is set; it returns 0
 * when the frame-update script (func_0034EBA0) says so. Otherwise sets RootTransformDirty (+0x5A) and returns 1.
 *
 * @return 0 when the frame-update script asks to stop updating, else 1
 */
extern "C" int cAnimComboNode_updateNode(cAnimComboNode* self)
{
    self->FrameStart = 10000000000.0f;
    self->FrameEnd = 10000000000.0f;
    if (*(signed char*)&self->AnimComplete2 == 0) {
        if (*(signed char*)&self->AnimActive2 != 0) {
            int countdown = self->Delay;
            if (countdown >= 0) {
                if (countdown > 0) {
                    self->Delay = countdown - 1;
                } else {
                    float pos = self->CurrTime2 + self->AnimSpeed2;
                    float max = self->ToTime2;
                    float frameTime = self->CurrTime;
                    self->FrameStart = frameTime;
                    self->FrameEnd = frameTime;
                    self->CurrTime2 = pos;
                    if (max <= pos) {
                        self->CurrTime2 = max;
                        if (*(signed char*)&self->Stop != 0) {
                            *(signed char*)&self->AnimComplete2 = 1;
                        }
                        *(signed char*)&self->AnimActive2 = 0;
                    }
                    if (func_0034EBA0(&self->CurrTime) == 0) {
                        return 0;
                    }
                }
            }
        } else {
            cAnimObjectNode_updateNode(&self->AnimType);
            return 1;
        }
        self->Flags |= 1;
    }
    return 1;
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
extern "C" void cAnimComboNode_updateIntersectInfo(cAnimComboNode* self, void* out)
{
    cMoveNode_updateIntersectInfo(&self->unk_048, out);
    *(sMpQuad*)((char*)out + 0x20) = t4vect_Zero;
    *(sMpQuad*)((char*)out + 0x30) = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_signal);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_signal(void*);
extern "C" void cAnimComboNode_setComboXform(cAnimComboNode*);

/**
 * Handles a signal on an anim combo node: signal 130 starts the second time range.
 *
 * For 130, when neither active nor stopped (+0x14, +0x15), it copies +0x38 to +0x10, snapshots the current mesh
 * matrices (cAnimComboNode_setComboXform), marks the range active and sets the time to its start (+0x8). Any other signal goes to the
 * animated-object handler cAnimObjectNode_signal.
 *
 * @param msg  signal id
 */
extern "C" void cAnimComboNode_signal(cAnimComboNode* self, int msg)
{
    if (msg == 130) {
        if (*(unsigned short*)&self->AnimActive2 == 0) {
            self->ComboTime = self->CacheTime;
            cAnimComboNode_setComboXform(self);
            self->AnimActive2 = 1;
            self->CurrTime2 = self->FromTime2;
        }
    } else {
        cAnimObjectNode_signal((char*)&self->AnimType);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_setComboXform);
#ifdef SKIP_ASM
struct sMpItem_9030 {
    char pad_0x0[0x90];
    float mtx[16];          // 0x90
} __attribute__((aligned(16)));

struct sMpPart_9030 {
    int field_0x0;
    int field_0x4;
    void* info;             // 0x8
    int field_0xc;
};

struct sMpModel_9030 {
    int field_0x0;
    int count;              // 0x4
    sMpPart_9030* parts;    // 0x8
};

struct sMpInst_9030 {
    char pad_0x0[0x80];
    sMpModel_9030* model;   // 0x80
};

struct sMpNode_9030 {
    char pad_0x0[0x48];
    sMpItem_9030* items;    // 0x48 (self + 0x7C)
};

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

struct sMtx_9030 {
    float m[16];
};


/**
 * Copies the current matrices of the instance's animated meshes into the ComboXform buffer (+0x84).
 *
 * Does nothing when the node has no animated meshes. For each model part with anim info it copies the next frame's
 * matrix in order.
 *
 * @param vself  the anim combo node
 */
// Gathers the world matrices of the model's active parts into the matrix buffer at 0x84.
extern "C" void cAnimComboNode_setComboXform(cAnimComboNode* vself)
{
    cAnimComboNode* self = vself;
    if (self->NumAnimMeshes <= 0) {
        return;
    }
    sMpModel_9030* model = ((sMpInst_9030*)self->Instance)->model;
    int n = 0;
    sMpPart_9030* part = model->parts;
    for (int i = 0; i < model->count; i++, part++) {
        if (part->info != 0) {
            sMpNode_9030* node = (sMpNode_9030*)((char*)self + 0x34);
            vu0CopyMatrix_9030(&((sMtx_9030*)self->ComboXform)[n], node->items[n].mtx);
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
extern "C" void cAnimComboNode_loseFocus(cAnimComboNode* self)
{
    if (*(signed char*)&self->AnimComplete2 == 0) {
        cInstanceNode_loseFocus(&self->unk_048);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry003590F0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cAnimObjectNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes an anim combo node to a replay frame: the animated-object part (cAnimObjectNode_writeToReplayFrame on +0x18), then its 0x18-byte
 * combo block.
 *
 * @param obj  replay stream to write to
 */
extern "C" void cAnimComboNode_writeToReplayFrame(cAnimComboNode* self, void* obj)
{
    cAnimObjectNode_writeToReplayFrame((char*)&self->AnimType, obj);
    sVEntry003590F0* vt = *(sVEntry003590F0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x18);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimComboNode_cacheFrame);
#ifdef SKIP_ASM
struct sMpItem_9140 {
    char data[0xD0];
};

struct sMpMtx_9140 {
    float m[16];
};

struct sMpPartInfo_9140 {
    char pad_0x0[0x1C];
    int active;             // 0x1C
};

struct sMpPart_9140 {
    int field_0x0;
    int field_0x4;
    sMpPartInfo_9140* info; // 0x8
    int field_0xc;
};

struct sMpModel_9140 {
    int field_0x0;
    int count;              // 0x4
    sMpPart_9140* parts;    // 0x8
};

struct sMpInst_9140 {
    char pad_0x0[0x80];
    sMpModel_9140* model;   // 0x80
};

struct sMp_9140 {
    char pad_0x0[0x14];
    signed char f14;        // 0x14
    signed char f15;        // 0x15
    signed char f16;        // 0x16
    signed char f17;        // 0x17
    char pad_0x18[0x48];
    sMpInst_9140* inst;     // 0x60
    char pad_0x64[0x18];
    sMpItem_9140* items;    // 0x7C
    int field_0x80;
    sMpMtx_9140* mtx;       // 0x84
};

extern "C" void cAnimNode_cacheFrame(void* self);
extern "C" void cMeshAnimFrame_transform(sMpItem_9140* item, sMpMtx_9140* mtx);

/**
 * Caches a combo node's animation frame and applies its combo transforms to every animated mesh.
 *
 * Runs cAnimNode_cacheFrame on the cAnimNode part (+0x34) first. If there are frames (+0x7C) and the node is in combo mode
 * (+0x14 set, or both +0x17 and +0x15 set), each mesh of the instance's model (+0x60, model +0x80) that has an
 * animation (part info +0x1C) gets its frame transformed by the next ComboXform matrix (+0x84) with cMeshAnimFrame_transform.
 */
extern "C" void cAnimComboNode_cacheFrame(sMp_9140* self)
{
    cAnimNode_cacheFrame((char*)self + 0x34);
    if (self->items == 0) {
        return;
    }
    if (self->f14 == 0) {
        if (self->f17 == 0) {
            return;
        }
        if (self->f15 == 0) {
            return;
        }
    }
    sMpModel_9140* model = self->inst->model;
    int n = 0;
    sMpPart_9140* meshPart = model->parts;
    for (int i = 0; i < model->count; i++, meshPart++) {
        if (meshPart->info->active != 0) {
            cMeshAnimFrame_transform(&self->items[n], (sMpMtx_9140*)((char*)self->mtx + (n << 6)));
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
extern "C" void* cAnimDeltaNode_cAnimDeltaNode1(cAnimDeltaNode* self, int bucket, int instRecord, int animData)
{
    cAnimObjectNode_cAnimObjectNode1(self, bucket, 2, instRecord, animData);
    self->mVtbl = cAnimDeltaNode_vtbl;
    *(int*)&self->Delta = 0;
    self->AnimActive = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_cAnimDeltaNode);
#ifdef SKIP_ASM
struct sVEntry00359270 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

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
extern "C" void* cAnimDeltaNode_cAnimDeltaNode(cAnimDeltaNode* self, void* bucket, void* stream)
{
    cAnimObjectNode_cAnimObjectNode(self, bucket, stream);
    self->mVtbl = cAnimDeltaNode_vtbl;
    sVEntry00359270* vt = *(sVEntry00359270**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)&self->Delta, 4);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_updateNode);
#ifdef SKIP_ASM
extern "C" void cAnimObjectNode_updateNode(void* p);

struct sVEntry_3592D0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

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
 * @return 0 only from that dead expire branch, else 1
 */
extern "C" int cAnimDeltaNode_updateNode(cAnimDeltaNode* self)
{
    self->FrameStart = 10000000000.0f;
    self->FrameEnd = 10000000000.0f;
    if (self->AnimComplete == 0) {
        float curDelta = self->Delta;
        int countdown = self->Delay;
        if (curDelta > 0.0f) {
            if (countdown >= 0) {
                if (countdown == 0 && curDelta > 0.0f) {
                    float newDelta = curDelta - __builtin_fabsf(self->AnimSpeed);
                    self->Delta = newDelta;
                    if (newDelta < 0.0f) {
                        self->Delta = 0.0f;
                    }
                }
                cAnimObjectNode_updateNode(((char*)self));
                if (self->Delta > 0.0f) {
                    self->AnimActive = 1;
                } else {
                    self->AnimActive = 0;
                }
                return 1;
            }
        } else {
            if (countdown > 0) {
                self->Delay = countdown - 1;
            }
            if (self->AnimComplete != 0) {
                char* obj = (char*)&self->unk_030;
                sVEntry_3592D0* vtEntry = &((sVEntry_3592D0*)self->mVtbl)[34];
                vtEntry->fn(obj + vtEntry->delta, 1);
                return 0;
            }
        }
        self->Flags |= 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_signal);
#ifdef SKIP_ASM
// PORT: cAnimObjectNode_signal really takes (self, id, value); the unit declares it (void*).
extern "C" void cAnimObjectNode_signal_set(void* self, int id, float v) __asm__("cAnimObjectNode_signal");

/**
 * Handles a signal on an anim delta node: signal 140 adds arg / 30 to Delta (+0x6C); others go to cAnimObjectNode_signal.
 *
 * @param id  signal id
 * @param signalArg  signal argument
 */
extern "C" void cAnimDeltaNode_signal(cAnimDeltaNode* self, int id, float signalArg)
{
    if (id == 140) {
        self->Delta += signalArg * 0.03333333507180214f;
    } else {
        cAnimObjectNode_signal_set(self, id, signalArg);
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", cAnimDeltaNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_00359410 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cAnimObjectNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes an anim delta node to a replay frame: the animated-object part (cAnimObjectNode_writeToReplayFrame), then Delta (+0x6C).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cAnimDeltaNode_writeToReplayFrame(cAnimDeltaNode* self, void* stream)
{
    cAnimObjectNode_writeToReplayFrame(self, stream);
    sSerVEntry_00359410* vt = *(sSerVEntry_00359410**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)&self->Delta, 0x4);
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
extern "C" void* cSplineModifier_cSplineModifier1(cSplineModifier* self, char* desc)
{
    *(void**)self = cSplineModifier_vtbl;
    self->Spline.SplineId = 0xFFFFFFFF;
    self->Spline.Seg = 0;
    self->Spline.SegIndex = 0;
    *(int*)&self->Spline.Length = 0;
    *(int*)&self->Radius = 0;
    self->SequenceType = *(int*)(desc + 0x8);
    self->OrientType = *(int*)(desc + 0xC);
    self->HeadTweak = *(float*)(desc + 0x14) * 0.01745329424738884f;
    self->Force = *(float*)(desc + 0x24) * 100.0f;
    self->ForceTime = *(float*)(desc + 0x28);
    cSpline_init(&self->Spline, *(int*)(desc + 0x4));
    cSplineModifier_setSpeed(((char*)self), *(float*)(desc + 0x10));
    if (*(float*)(desc + 0x10) >= 0.0f) {
        self->OffsetDistance = 0.0f;
    } else {
        self->OffsetDistance = self->Spline.Length;
    }
    float zeroF = 0.0f;
    self->Start = *(int*)(desc + 0x20) != 0;
    self->Complete = 0;
    if (*(float*)(desc + 0x18) != zeroF) {
        self->OffsetDistance = *(float*)(desc + 0x18);
    }
    self->OffsetDistance += AIrandf(-1.0f, 1.0f) * *(float*)(desc + 0x1C);
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

