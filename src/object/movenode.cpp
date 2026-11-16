#include "common.h"
#include "object/positionmodifier.h"
#include "object/parentmodifier.h"
#include "object/magnetmodifier.h"
#include "object/avasplinemodifier.h"
#include "object/movenode.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"

//100%
INCLUDE_ASM("object/movenode", cMoveNode_cMoveNode);
#ifdef SKIP_ASM
struct sSerVEntry_55298 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct cMoveNode;
void cMoveNode_addModifierBlock(cMoveNode* self);
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a, void* stream);
extern "C" void tModifierBlock_readFromReplayFrame(void* block, void* stream);
extern "C" void cMoveNode_setupOverlapSystem(cMoveNode* self);
extern char cMoveNode_vtbl[];

// PORT: the unit declares cMoveNode_cMoveNode(void*) (1 arg) for its callers; the body reads a1/a2
// (forwarded to cInstanceNode_cInstanceNode), so the 3-arg body is bound by asm label.
void* cMoveNode_cMoveNode_3(void* self, void* a, void* stream) __asm__("cMoveNode_cMoveNode");

/**
 * Recreates a moving world object from a replay frame: the instance-node part, debounce, fade and transfer
 * flag, and its modifiers.
 *
 * A modifier block is created and read back only when the frame says the node had one. Joins the overlap
 * system
 * and marks the node for update.
 *
 * @param a  the bucket the node belongs to (On Tour const int bucket)
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 * @return @p self
 */
void* cMoveNode_cMoveNode_3(void* self, void* a, void* stream)
{
    int flag;
    cInstanceNode_cInstanceNode(self, a, stream);
    *(int*)((char*)self + 0x1C) = 0;
    *(void**)((char*)self + 0xC) = cMoveNode_vtbl;
    sSerVEntry_55298* e = &(*(sSerVEntry_55298**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x20, 4);
    e = &(*(sSerVEntry_55298**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x24, 4);
    e = &(*(sSerVEntry_55298**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x28, 4);
    e = &(*(sSerVEntry_55298**)stream)[2];
    e->fn((char*)stream + e->delta, &flag, 4);
    if (flag) {
        cMoveNode_addModifierBlock((cMoveNode*)self);
        tModifierBlock_readFromReplayFrame(*(void**)((char*)self + 0x1C), stream);
    }
    cMoveNode_setupOverlapSystem((cMoveNode*)self);
    *(unsigned short*)((char*)self + 0x12) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode__cMoveNode);
#ifdef SKIP_ASM
extern char cMoveNode_vtbl[];
extern "C" void cMoveNode_restoreOverlapSystem(cMoveNode* self);
extern "C" void tModifierBlock__tModifierBlock(void* block, int flags);
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

/**
 * Destroys a move node: restores the overlap system, deletes its modifier block, then runs the base destructor.
 *
 * Stores cMoveNode's vtable at +0xC, calls cMoveNode_restoreOverlapSystem(), destroys and frees the
 * ModifierBlock (+0x1C) when there is one, then calls cInstanceNode__cInstanceNode() with the same flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMoveNode__cMoveNode(cMoveNode* self, int flags)
{
    self->mVtbl = (void**)cMoveNode_vtbl;
    cMoveNode_restoreOverlapSystem(self);
    void* block = self->ModifierBlock;
    if (block != 0) {
        tModifierBlock__tModifierBlock(block, 3);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_shouldRebound);
#ifdef SKIP_ASM
struct sMoveNodeVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Tells whether a rider should rebound off this node, asking its transform modifier when it has one.
 *
 * @return 1 when the node has no modifier block or no transform modifier in it, otherwise the modifier's
 *         shouldRebound() (vtable slot 8)
 */
extern "C" int cMoveNode_shouldRebound(cMoveNode* self)
{
    void* block = self->ModifierBlock;
    if (block == 0) {
        return 1;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 1;
    }
    sMoveNodeVEntryI* vt = *(sMoveNodeVEntryI**)node;
    return vt[8].fn((char*)node + vt[8].delta);
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
struct tModifierBlock;
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self);
extern const char D_0048E9D8[];


//100%
INCLUDE_ASM("object/movenode", cMoveNode_addModifierBlock__FP9cMoveNode);
#ifdef SKIP_ASM
/**
 * Gives the node a new, empty modifier block ("ModifierBlock").
 */
void cMoveNode_addModifierBlock(cMoveNode* self)
{
    void* mem = cMemMan_alloc(0x28, D_0048E9D8, 0x20000000, 0);
    (*(void**)&self->ModifierBlock) = tModifierBlock_tModifierBlock((tModifierBlock*)mem);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addTransformModifier);
#ifdef SKIP_ASM
struct sMoveNodeVEntryV54B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void tModifierBlock_remTransformModifier(void* block);

/**
 * Makes a transform modifier the node's root transform, replacing (deleting) any earlier one.
 *
 * Leaves the overlap system first (restoreOverlapSystem, slot 49) and rejoins it after (setupOverlapSystem,
 * slot
 * 48), then switches the object's flags from static (0x20) to moving (0x40).
 *
 * @param mod  the transform modifier (On Tour cTransformModifier* modifier)
 */
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod)
{
    sMoveNodeVEntryV54B0* e = &((sMoveNodeVEntryV54B0*)self->mVtbl)[49];
    e->fn((char*)self + e->delta);
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remTransformModifier(((void*)self->ModifierBlock));
    }
    *(void**)((void*)self->ModifierBlock) = mod;
    e = &((sMoveNodeVEntryV54B0*)self->mVtbl)[48];
    e->fn((char*)self + e->delta);
    unsigned int* f = (unsigned int*)((char*)self->Instance + 8);
    *f = (*f & ~0x20u) | 0x40;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addUVScrollModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remUVScrollModifier(void* block);

/**
 * Gives the node a UV-scroll modifier, replacing any it had.
 *
 * Creates the modifier block when the node has none (cMoveNode_addModifierBlock), else removes the old UV-scroll
 * modifier (tModifierBlock_remUVScrollModifier), then stores @p modifier at block + 0x4.
 *
 * @param modifier  the new modifier (On Tour cUVScrollModifier* modifier)
 */
extern "C" void cMoveNode_addUVScrollModifier(cMoveNode* self, void* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remUVScrollModifier(((void*)self->ModifierBlock));
    }
    *(void**)((char*)((void*)self->ModifierBlock) + 0x4) = modifier;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addTexFlipModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remTexFlipModifier(void* block);

/**
 * Gives the node a texture-flip modifier, replacing any it had.
 *
 * Creates the modifier block when the node has none, else removes the old one (tModifierBlock_remTexFlipModifier),
 * then stores @p modifier at block + 0x8.
 *
 * @param modifier  the new modifier (On Tour cTexFlipModifier* modifier)
 */
extern "C" void cMoveNode_addTexFlipModifier(cMoveNode* self, void* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remTexFlipModifier(((void*)self->ModifierBlock));
    }
    *(void**)((char*)((void*)self->ModifierBlock) + 0x8) = modifier;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addPhysicsModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remPhysicsModifier(void* block);

/**
 * Gives the node a physics modifier, replacing any it had.
 *
 * Creates the modifier block when the node has none, else removes the old one (tModifierBlock_remPhysicsModifier),
 * then stores @p modifier at block + 0xC.
 *
 * @param modifier  the new modifier (On Tour cPhysicsModifier* modifier)
 */
extern "C" void cMoveNode_addPhysicsModifier(cMoveNode* self, void* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remPhysicsModifier(((void*)self->ModifierBlock));
    }
    *(void**)((char*)((void*)self->ModifierBlock) + 0xC) = modifier;
}
#endif

struct cEffectLink;
void cEffectLink_add(cEffectLink* link, cEffectLink* other);

//99.74%
INCLUDE_ASM("object/movenode", cMoveNode_addEffectModifier__FP9cMoveNodePv);
#ifdef SKIP_ASM
/**
 * Adds an effect modifier (particles, halo...) to the node's effect list, creating the modifier block if
 * needed.
 *
 * @param effect  the effect modifier (On Tour cEffectModifier* modifier)
 */
void cMoveNode_addEffectModifier(cMoveNode* self, void* effect)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    }
    cEffectLink_add((cEffectLink*)((char*)((void*)self->ModifierBlock) + 0x10), (cEffectLink*)effect);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addRailModifier);
#ifdef SKIP_ASM
extern "C" void cRailLink_add(void* list, void* item);

/**
 * Adds a rail modifier to the node's rail list, creating the modifier block if needed.
 *
 * @param item  the rail modifier (On Tour cRailModifier* modifier)
 */
extern "C" void cMoveNode_addRailModifier(cMoveNode* self, void* item)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    }
    cRailLink_add((char*)((void*)self->ModifierBlock) + 0x1C, item);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_preUpdateModifierBlock);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_preUpdate(void* block);
extern "C" int tModifierBlock_isRootTransformDirty(void* block);

/**
 * Runs the modifier block's pre-update and marks the node for update when its root transform asks for it.
 */
extern "C" void cMoveNode_preUpdateModifierBlock(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        tModifierBlock_preUpdate(((void*)self->ModifierBlock));
        if (tModifierBlock_isRootTransformDirty(((void*)self->ModifierBlock)) != 0) {
            self->Flags |= 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_postUpdateModifierBlock);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_postUpdate(void*);

/**
 * Runs tModifierBlock_postUpdate() on the node's modifier block (+0x1C), when it has one.
 */
extern "C" void cMoveNode_postUpdateModifierBlock(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        tModifierBlock_postUpdate(((void*)self->ModifierBlock));
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_onCollide);
#ifdef SKIP_ASM
extern char* D_004A5B64;
extern "C" int func_002D1B30(int);
void func_0034FE00(void*, int, int, int);

struct cMN_355770_Obj {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual int v09(int);
    virtual int v10(int a, int b);
};

struct cMN_355770_VEnt {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Handles a collision with the node: the root transform reacts first, then the collision is passed on at most
 * every half second.
 *
 * When the root transform's slot-9 test rejects the colliding being, nothing more happens. Otherwise, once
 * the
 * debounce has run out it is re-armed to half a second and func_0034FE00 forwards the collision (probably to
 * the
 * script system, D_004A3DD8).
 *
 * @param tc  collision info (On Tour const tCollideInfo*, passed as int)
 * @param ti  intersection info (On Tour const tIntersectInfo*, passed as int)
 * @param pBeing  the colliding being (cBeing*, passed as int)
 */
extern "C" void cMoveNode_onCollide(cMoveNode* self, int tc, int ti, int pBeing)
{
    cMN_355770_Obj** p = (cMN_355770_Obj**)((void*)self->ModifierBlock);
    if (p != 0) {
        cMN_355770_Obj* obj = *p;
        if (obj != 0) {
            obj->v10(tc, ti);
            char* o = *(char**)((void*)self->ModifierBlock);
            cMN_355770_VEnt* vt = *(cMN_355770_VEnt**)o;
            if (vt[9].fn(o + vt[9].delta, func_002D1B30(pBeing)) == 0) {
                return;
            }
        }
    }
    if (self->Debounce <= 0) {
        self->Debounce = *(int*)(D_004A5B64 + 0x10) / 2;
        func_0034FE00(self, tc, ti, pBeing);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_setDebounce);
#ifdef SKIP_ASM
extern char* D_004A5B64;

/**
 * Sets the node's collision debounce, in seconds, as a count of app timer ticks.
 *
 * @param secs  debounce time in seconds
 */
extern "C" void cMoveNode_setDebounce(cMoveNode* self, float secs)
{
    self->Debounce = (int)((float)*(int*)(D_004A5B64 + 0x10) * secs);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00355878__FPvT0);
#ifdef SKIP_ASM
float func_00355878(void* self, void* a1)
{
    float t0 = *(float*)((char*)a1 + 0x4);
    *(float*)((char*)self + 0x24) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addDebounce);
#ifdef SKIP_ASM
struct sMnVEntryF {
    short delta;
    short index;
    void (*fn)(void*, float);
};

/**
 * Sets the node's debounce time from a placement record: setDebounce (slot 41) with the record's float at +0x4.
 *
 * @param data  the debounce data (On Tour const tAddDebounceData& data); +0x4 holds the time in seconds
 */
extern "C" void cMoveNode_addDebounce(cMoveNode* self, void* data)
{
    sMnVEntryF* vt = (sMnVEntryF*)self->mVtbl;
    vt[41].fn((char*)self + vt[41].delta, *(float*)((char*)data + 0x4));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_003558B8);
#ifdef SKIP_ASM
struct sMoveQuad;
extern char D_0048E8F0[];
extern "C" void* func_00356EB8(cPositionModifier* self, sMoveQuad* pos);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

extern "C" void func_003558B8(cMoveNode* self, sMoveQuad* pos)
{
    cMoveNode_addTransformModifier(self, func_00356EB8((cPositionModifier*)cMemMan_alloc(0x90, D_0048E8F0, 0x20000000, 0), pos));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00355918);
#ifdef SKIP_ASM
extern char D_0048E8F0[];
extern "C" void* func_00356F10(cPositionModifier* mem, void* desc);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

extern "C" void func_00355918(cMoveNode* self, void* desc)
{
    cMoveNode_addTransformModifier(self, func_00356F10((cPositionModifier*)cMemMan_alloc(0x90, D_0048E8F0, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addParent);
#ifdef SKIP_ASM
struct sMoveQuad;
extern char D_0048E8E0[];
extern "C" void* func_00357038(cParentModifier* self, void* a1, void* a2, sMoveQuad* pos);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

/**
 * Attaches the node to a parent instance: a new cParentModifier ("ParentModifier", func_00357038) as its transform
 * modifier.
 *
 * @param parentinst  the parent instance (On Tour tInstanceRecord* parentinst)
 * @param mesh  the parent's mesh index (On Tour int mesh)
 * @param pos  the offset from the parent (On Tour const t4Vector& offset)
 */
extern "C" void cMoveNode_addParent(cMoveNode* self, void* parentinst, void* mesh, sMoveQuad* pos)
{
    cMoveNode_addTransformModifier(self, func_00357038((cParentModifier*)cMemMan_alloc(0xB0, D_0048E8E0, 0x20000000, 0), parentinst, mesh, pos));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addMagnet);
#ifdef SKIP_ASM
extern char D_0048E918[];
extern "C" void* func_003572E0(cMagnetModifier* self, void* owner, void* a2, float f0, float f1);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

/**
 * Makes the node a magnet: a new cMagnetModifier ("MagnetModifier", func_003572E0) on its instance as its transform
 * modifier.
 *
 * @param mat  the magnet's matrix (On Tour const t4Matrix& mat)
 * @param radius  the radius (On Tour radius)
 * @param magnetSpeed  the speed (On Tour speed)
 */
extern "C" void cMoveNode_addMagnet(cMoveNode* self, void* mat, float radius, float magnetSpeed)
{
    cMoveNode_addTransformModifier(self, func_003572E0((cMagnetModifier*)cMemMan_alloc(0xB0, D_0048E918, 0x20000000, 0),
                                      self->Instance, mat, radius, magnetSpeed));
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00355A78);
#ifdef SKIP_ASM
extern int D_004A3AC4;
extern char D_0048E928[];
extern "C" void* func_00357750(cAvaSplineModifier* self, void* src);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

extern "C" void func_00355A78(cMoveNode* self)
{
    if (D_004A3AC4 == 0) {
        cMoveNode_addTransformModifier(self, func_00357750((cAvaSplineModifier*)cMemMan_alloc(0x90, D_0048E928, 0x20000000, 0), self->Instance));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addSpline1);
#ifdef SKIP_ASM
extern char D_0048E8D0[];
extern "C" void* cSplineModifier_cSplineModifier1(void* mem, void* desc);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

/**
 * Gives the node a spline path: builds a cSplineModifier from the spline data and attaches it.
 *
 * Allocates 0xF0 bytes tagged "SplineModifier", runs cSplineModifier_cSplineModifier1() on them, and passes the
 * result to cMoveNode_addTransformModifier().
 *
 * @param desc  the spline setup data (On Tour const tAddSplineData&)
 */
extern "C" void cMoveNode_addSpline1(cMoveNode* self, void* desc)
{
    cMoveNode_addTransformModifier(self, cSplineModifier_cSplineModifier1(cMemMan_alloc(0xF0, D_0048E8D0, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addSpline);
#ifdef SKIP_ASM
extern char D_0048E9E8[];
extern "C" void* cMultiSplineModifier_cMultiSplineModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

/**
 * Makes a new spline-path modifier ("MultiSplineModifier") the node's root transform.
 *
 * @param desc  the spline description (On Tour const tAddSplineData& sdata)
 */
extern "C" void cMoveNode_addSpline(cMoveNode* self, void* desc)
{
    cMoveNode_addTransformModifier(self, cMultiSplineModifier_cMultiSplineModifier(cMemMan_alloc(0x58, D_0048E9E8, 0x20000000, 0), desc,
                                      self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addUVScroll);
#ifdef SKIP_ASM
extern char D_0048E940[];
extern "C" void* func_0035F6E8(void* mem, void* desc);
extern "C" void cMoveNode_addUVScrollModifier(cMoveNode* self, void* mod);

/**
 * Adds a UV-scroll modifier built from a placement record: a new "UVScrollModifier" (func_0035F6E8) on the node.
 *
 * @param desc  the UV-scroll data (On Tour const tAddUVScrollData& udata)
 */
extern "C" void cMoveNode_addUVScroll(cMoveNode* self, void* desc)
{
    cMoveNode_addUVScrollModifier(self, func_0035F6E8(cMemMan_alloc(0x50, D_0048E940, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addTexFlip);
#ifdef SKIP_ASM
extern char D_0048E958[];
extern "C" void* func_0035F0B8(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addTexFlipModifier(cMoveNode* self, void* mod);

/**
 * Adds a texture-flip modifier built from a placement record: a new "TexFlipModifier" (func_0035F0B8, with the node's
 * instance) on the node.
 *
 * @param desc  the texture-flip data (On Tour const tAddTexFlipData& tdata)
 */
extern "C" void cMoveNode_addTexFlip(cMoveNode* self, void* desc)
{
    cMoveNode_addTexFlipModifier(self, func_0035F0B8(cMemMan_alloc(0xE8, D_0048E958, 0x20000000, 0), desc, self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addBoost);
#ifdef SKIP_ASM
extern char D_0048E968[];
extern "C" void* func_00342768(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addPhysicsModifier(cMoveNode* self, void* mod);

/**
 * Adds a boost modifier built from a placement record: a new "BoostModifier" (func_00342768, with the node's
 * instance) as the node's physics modifier.
 *
 * @param desc  the boost data (On Tour const tAddBoostData& adata)
 */
extern "C" void cMoveNode_addBoost(cMoveNode* self, void* desc)
{
    cMoveNode_addPhysicsModifier(self, func_00342768(cMemMan_alloc(0x50, D_0048E968, 0x20000000, 0), desc, self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addParticle);
#ifdef SKIP_ASM
extern char D_0048E978[];
extern "C" void* func_003458C0(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

/**
 * Adds a particle-emitter effect ("ParticleModifier") to the node.
 *
 * @param desc  the particle description (On Tour tAddParticleData* pdata)
 */
extern "C" void cMoveNode_addParticle(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, func_003458C0(cMemMan_alloc(0x1F0, D_0048E978, 0x20000000, 0), desc,
                                                    self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addDynamicParticle);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void* func_00345C90(void* mem, void* desc, void* inst, int index, void* a4);
extern const char D_0048E990[];

/**
 * Adds a particle effect tied to one of the node's meshes ("DynamicParticleModifier"); a bad mesh index frees
 * the description instead.
 *
 * @param desc  the particle description (On Tour tAddDynamicParticleData* pdata); owned by the call
 * @param index  the mesh index (On Tour mesh)
 * @param offset  emitter offset from the mesh (On Tour const t4Vector&)
 */
extern "C" void cMoveNode_addDynamicParticle(cMoveNode* self, void* desc, int index, void* offset)
{
    if (index < 0 || index >= *(int*)(*(char**)((char*)self->Instance + 0x80) + 4)) {
        operator_delete((int*)desc);
        return;
    }
    cMoveNode_addEffectModifier(self, func_00345C90(cMemMan_alloc(0x270, D_0048E990, 0x20000000, 0), desc, self->Instance, index, offset));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addRoller);
#ifdef SKIP_ASM
extern char D_0048E908[];
extern "C" void* cRollerModifier_cRollerModifier(void* mem, void* a1, void* a2, void* a3);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, void* mod);

/**
 * Makes a new roller modifier ("RollerModifier") the node's root transform.
 *
 * @param rdata  the roller description (On Tour const tAddRollerData&)
 * @param ii  intersection info of the trigger (On Tour const tIntersectInfo*)
 * @param ci  collision info of the trigger (On Tour const tCollideInfo*)
 */
extern "C" void cMoveNode_addRoller(cMoveNode* self, void* rdata, void* ii, void* ci)
{
    cMoveNode_addTransformModifier(self, cRollerModifier_cRollerModifier(cMemMan_alloc(0x2D0, D_0048E908, 0x20000000, 0), rdata, ii, ci));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addRail);
#ifdef SKIP_ASM
extern char D_0048E9B8[];
extern "C" int tModifierBlock_findRail(void* self, int id);
extern "C" void* cRailModifier_cRailModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addRailModifier(cMoveNode* self, void* item);

/**
 * Adds a rail modifier ("RailModifier") for a rail description, unless the node already has a rail with that
 * ID.
 *
 * @param desc  the rail description (On Tour const tAddRailData& rdata); its ID is at +4
 */
extern "C" void cMoveNode_addRail(cMoveNode* self, void* desc)
{
    void* block = ((void*)self->ModifierBlock);
    if (block != 0 && tModifierBlock_findRail(block, *(int*)((char*)desc + 0x4)) != 0) {
        return;
    }
    cMoveNode_addRailModifier(self, cRailModifier_cRailModifier(cMemMan_alloc(0x90, D_0048E9B8, 0x20000000, 0), desc,
                                      self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addHalo);
#ifdef SKIP_ASM
extern char D_0048E9A8[];
extern "C" void* func_00346120(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

/**
 * Adds a halo effect ("HaloModifier") to the node.
 *
 * @param desc  the halo description (On Tour const tAddHaloData& hdata)
 */
extern "C" void cMoveNode_addHalo(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, func_00346120(cMemMan_alloc(0x40, D_0048E9A8, 0x20000000, 0), desc,
                                                    self->Instance));
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_transferFromOld);
#ifdef SKIP_ASM
extern "C" void cMoveNode_transferEffectModifier(cMoveNode* self, cMoveNode* other);

struct sMat44_355F10 {
    float m[16];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrix_355F10(void* dst, void* src)
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

struct sNodeVEntryI_355F10 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sNodeVEntryP_355F10 {
    short delta;
    short index;
    char* (*fn)(void*);
};
struct sNodeVEntryD_355F10 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sNodeVEntryV_355F10 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Takes over an old instance node's effects and transform, then deletes the old node.
 *
 * When the old node has effects (slot 16) they move over (cMoveNode_transferEffectModifier). If its transform (slot
 * 24) is this node's own instance matrix (Instance + 0x10) the old node is just deleted; otherwise the matrix is
 * copied, the old node deleted (keeping Instance's word +0x8), the copy applied with func_00355918, TransferHack
 * set and virtual slot 50 called.
 *
 * @param srcv  the node being replaced (On Tour cInstanceNode* pOldNode); null is ignored
 */
extern "C" void cMoveNode_transferFromOld(cMoveNode* self, void* srcv)
{
    char* src = (char*)srcv;
    if (src == 0) {
        return;
    }
    sNodeVEntryI_355F10* vi = *(sNodeVEntryI_355F10**)(src + 0xC);
    if (vi[16].fn(src + vi[16].delta) != 0) {
        cMoveNode_transferEffectModifier(self, (cMoveNode*)src);
    }
    sNodeVEntryP_355F10* vp = *(sNodeVEntryP_355F10**)(src + 0xC);
    char* m = vp[24].fn(src + vp[24].delta);
    if (m == (char*)self->Instance + 0x10) {
        sNodeVEntryD_355F10* vd = *(sNodeVEntryD_355F10**)(src + 0xC);
        vd[1].fn(src + vd[1].delta, 3);
        return;
    }
    sMat44_355F10 copy;
    vu0CopyMatrix_355F10(&copy, m);
    int keep = *(int*)((char*)self->Instance + 8);
    sNodeVEntryD_355F10* vd = *(sNodeVEntryD_355F10**)(src + 0xC);
    vd[1].fn(src + vd[1].delta, 3);
    *(int*)((char*)self->Instance + 8) = keep;
    func_00355918(self, &copy);
    self->TransferHack = 1;
    sNodeVEntryV_355F10* vv = (sNodeVEntryV_355F10*)self->mVtbl;
    vv[50].fn((char*)self + vv[50].delta);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_transferEffectModifier);
#ifdef SKIP_ASM
extern "C" void func_00353398(void* block, void* other);

/**
 * Moves another node's effect links into this node's modifier block, creating the block when needed.
 *
 * Uses func_00353398 (On Tour tModifierBlock::transferEffectModifier) on the other block's effect link (+0x10).
 *
 * @param other  the node giving up its effects (On Tour oldNode)
 */
extern "C" void cMoveNode_transferEffectModifier(cMoveNode* self, cMoveNode* other)
{
    if (((void*)other->ModifierBlock) != 0) {
        if (((void*)self->ModifierBlock) == 0) {
            cMoveNode_addModifierBlock(self);
        }
        func_00353398(((void*)self->ModifierBlock), (char*)((void*)other->ModifierBlock) + 0x10);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_getRootTransform);
#ifdef SKIP_ASM
struct sMnVEntryP_6078 {
    short delta;
    short index;
    void* (*fn)(void*);
};
// Virtual call through the modifier object's vtable (slot 18); taking the object by reference matches the register allocation.
static inline void* VCall18_6078(char*& o)
{
    sMnVEntryP_6078* vt = *(sMnVEntryP_6078**)o;
    return vt[18].fn(o + vt[18].delta);
}
/**
 * Returns the node's world transform: its root transform modifier's (slot 18), or the object's own matrix.
 *
 * @return the transform matrix (On Tour const t4Matrix*)
 */
extern "C" void* cMoveNode_getRootTransform(cMoveNode* self)
{
    char** h = (char**)self->ModifierBlock;
    char* o = h ? *h : 0;
    if (o != 0)
        return VCall18_6078(o);
    return (char*)self->Instance + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_getInstAnimMats);
#ifdef SKIP_ASM
struct sMnVEntryI2_60C0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};
struct sMnVEntryI_60C0 {
    short delta;
    short index;
    int (*fn)(void*);
};
// Taking the object by reference matches the target's register allocation (see cMoveNode_getRootTransform).
static inline int VCall23_60C0(char*& o, int a)
{
    sMnVEntryI2_60C0* vt = *(sMnVEntryI2_60C0**)o;
    return vt[23].fn(o + vt[23].delta, a);
}
/**
 * Returns the animation matrices for an instance: the root transform modifier's (slot 23), else the node's
 * own (slot 27).
 *
 * @param inst  the instance record (On Tour tInstanceRecord*, passed as int)
 * @return the matrices (On Tour const t4Matrix*, as int), or 0
 */
// Asks the modifier object (slot 23, passing a through), then falls back to the node's own slot 27.
extern "C" int cMoveNode_getInstAnimMats(cMoveNode* self, int inst)
{
    char** h = (char**)self->ModifierBlock;
    char* o = h ? *h : 0;
    if (o != 0) {
        int r = VCall23_60C0(o, inst);
        if (r != 0) return r;
    }
    sMnVEntryI_60C0* vs = (sMnVEntryI_60C0*)self->mVtbl;
    return vs[27].fn(((char*)self) + vs[27].delta);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_getRootTransform1);
#ifdef SKIP_ASM
struct sMnVEntryP_6128 {
    short delta;
    short index;
    void* (*fn)(void*);
};
// Taking the object by reference matches the target's register allocation (see cMoveNode_getRootTransform).
static inline void* VCall_6128(char*& o, int i)
{
    sMnVEntryP_6128* vt = *(sMnVEntryP_6128**)o;
    return vt[i].fn(o + vt[i].delta);
}
/**
 * Returns the transform for an instance: the root transform modifier's slot 19 or else slot 18, or the
 * object's own matrix.
 *
 * The C declaration drops the instance argument (On Tour tInstanceRecord* inst).
 *
 * @return the transform matrix (On Tour const t4Matrix*)
 */
// Returns the modifier object's slot-19 value, else its slot-18 value; without one, the node's object position.
extern "C" void* cMoveNode_getRootTransform1(cMoveNode* self)
{
    char** h = (char**)self->ModifierBlock;
    char* o = h ? *h : 0;
    if (o != 0) {
        void* r = VCall_6128(o, 19);
        if (r != 0) return r;
        return VCall_6128(o, 18);
    }
    return (char*)self->Instance + 0x10;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_update);
#ifdef SKIP_ASM
int cInstanceNode_runOnLoseFocusScript(void* self);
extern "C" int tModifierBlock_isComplete(void* block);
extern "C" void tModifierBlock_clearComplete(void* block);

struct sMnVEntryI_356198 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMnVEntryV_356198 {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sMnVEntryVi_356198 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Updates a moving node for one tick: debounce, losing focus, its modifiers, and expiry once they complete.
 *
 * A node with a focus count that has dropped to 0 loses focus (slot 35) unless its lose-focus script handles
 * it.
 * When the modifier block reports completion the node expires (slot 34 with 1).
 *
 * @param selfv  the node
 */
extern "C" void cMoveNode_update(cMoveNode* selfv)
{
    cMoveNode* self = (cMoveNode*)selfv;
    if (selfv->Debounce > 0) {
        selfv->Debounce -= 1;
    }
    sMnVEntryI_356198* vi = (sMnVEntryI_356198*)selfv->mVtbl;
    if (vi[20].fn(((char*)selfv) + vi[20].delta) != 0) {
        sMnVEntryI_356198* vi2 = (sMnVEntryI_356198*)selfv->mVtbl;
        if (vi2[23].fn(((char*)selfv) + vi2[23].delta) <= 0 && cInstanceNode_runOnLoseFocusScript(self) == 0) {
            sMnVEntryV_356198* vv = (sMnVEntryV_356198*)selfv->mVtbl;
            vv[35].fn(((char*)selfv) + vv[35].delta);
            return;
        }
    }
    cMoveNode_preUpdateModifierBlock(self);
    sMnVEntryI_356198* vi3 = (sMnVEntryI_356198*)selfv->mVtbl;
    if (vi3[15].fn(((char*)selfv) + vi3[15].delta) != 0) {
        cMoveNode_postUpdateModifierBlock(self);
        if (((void*)self->ModifierBlock) != 0 && tModifierBlock_isComplete(((void*)self->ModifierBlock)) != 0) {
            tModifierBlock_clearComplete(((void*)self->ModifierBlock));
            sMnVEntryVi_356198* vw = (sMnVEntryVi_356198*)selfv->mVtbl;
            vw[34].fn(((char*)selfv) + vw[34].delta, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMoveQuad;
extern sMoveQuad t4mat_Identity[];

struct sMnVec_6298 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMnMat_6298 {
    sMnVec_6298 r[4];
};

struct sMnSlot_6298 {
    int f0;
    int type;                       // 0x4 (array at 0x20 + 4)
};

struct sMnCam_6298 {
    char pad_0x0[0x20];
    sMnSlot_6298 slots[(0x3F0 - 0x20) / 8];  // 0x20
    struct {
        int kind;
        char pad[0x14];
    } paths[1];                     // 0x3F0
};

struct sMnObj_6298 {
    char pad_0x0[0x8];
    int flags;                      // 0x8
    char pad_0xC[0x50 - 0xC];
    char box[0x10];                 // 0x50
    float pos[3];                   // 0x60
    float scl[3];                   // 0x6C
    char pad_0x78[0x7D - 0x78];
    unsigned char slot;             // 0x7D
    short path;                     // 0x7E
    char pad_0x80[0x94 - 0x80];
    void* light;                    // 0x94
};

class cMnOwner_6298 {
public:
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
    virtual int v26();
};

struct sMnMod_6298 {
    cMnOwner_6298* owner;           // 0x0
    void* rot;                      // 0x4
    char* volatile light;           // 0x8
};

class cMnNode_6298 {
public:
    char pad[0xC];
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
    virtual void* v24();
    virtual void v25();
    virtual int v26();
    virtual void* v27();
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
    virtual void* v44();
    virtual sMnVec_6298* v45();
};

struct sMnGfxVt_6298 {
    short delta;
    short index;
    void* fn;
};

struct cMnGfx_6298 {
    char pad_0x0[0x10D8];
    sMnGfxVt_6298* vt;              // 0x10D8
};

typedef sMnMat_6298 (*MnViewFn_6298)(void*);
typedef int (*MnCullFn_6298)(void*, sMnVec_6298*, sMnVec_6298*, const sMnMat_6298&);
typedef void (*MnDrawFn_6298)(void*, void*, void*, void*, void*, void*, int, float);

extern "C" void tModifierBlock_render(void* self);
extern "C" int func_002D2058(void* v);
extern "C" void* cUVScrollModifier_getTextureMat(void* self, void* m);

#define OBJ_6298(s) (*(sMnObj_6298**)((char*)(s) + 0x18))
#define MOD_6298(s) (*(sMnMod_6298**)((char*)(s) + 0x1C))

static inline int HasRot_6298(sMnMod_6298* m)
{
    return m != 0 && m->rot != 0;
}

static inline int HasLight_6298(sMnMod_6298* m)
{
    return m != 0 && m->light != 0;
}

static inline void* Mat_6298(void* rot, sMnMat_6298* m)
{
    void* r;
    if (rot) {
        r = cUVScrollModifier_getTextureMat(rot, m);
    } else {
        r = 0;
    }
    return r;
}

static inline void* LmSub_6298(char* l)
{
    return l + 0x24;
}

static inline void* Lm_6298(char* l)
{
    if (l == 0) {
        return 0;
    }
    return LmSub_6298(l);
}

static inline void* RotOf_6298(cMnNode_6298* n, sMnMat_6298* m)
{
    if (HasRot_6298(MOD_6298(n))) {
        return Mat_6298(MOD_6298(n)->rot, m);
    }
    return 0;
}

static inline void* LightOf_6298(cMnNode_6298* n)
{
    if (HasLight_6298(MOD_6298(n))) {
        return Lm_6298(MOD_6298(n)->light);
    }
    return OBJ_6298(n)->light;
}

static inline bool IsT6_6298(sMnCam_6298* c, int i)
{
    return c->slots[i].type == 6;
}

static inline int Visible_6298(sMnCam_6298* cam, sMnObj_6298* obj)
{
    if (obj->slot == 0xFF) {
        return 1;
    }
    if (IsT6_6298(cam, obj->slot)) {
        return cam->paths[obj->path].kind == 3;
    }
    return 0;
}

/**
 * Draws a moving node's object when it is visible, enabled, not culled, and not hidden by the transfer flag.
 *
 * Culls with its bound box or the object's bounds, picks animation matrices or the root transform, applies
 * modifier rotation and lighting, and draws with its fade through GrMan slot 95.
 */
extern "C" void cMoveNode_render(cMoveNode* self)
{
    sMnCam_6298* cam = (sMnCam_6298*)cWorld_m_pTheWorld->m_pWorldCache;
    sMnObj_6298* obj = OBJ_6298(self);
    if (!Visible_6298(cam, obj)) {
        return;
    }
    int flags = 0;
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    if (MOD_6298(self)) {
        tModifierBlock_render(MOD_6298(self));
        cMnOwner_6298* o = MOD_6298(self)->owner;
        if (o) {
            if (o->v26() == 7) {
                flags |= 0x2000;
            }
        }
    }
    if (!(OBJ_6298(self)->flags & 4)) {
        return;
    }
    if (self->TransferHack) {
        return;
    }
    void* mat = ((cMnNode_6298*)self)->v44();
    int res;
    if (mat) {
        sMnVec_6298* m = ((cMnNode_6298*)self)->v45();
        sMnGfxVt_6298* vt = (sMnGfxVt_6298*)gfx->mVtbl;
        char* thisp = (char*)gfx + vt[93].delta;
        res = ((MnCullFn_6298)vt[93].fn)(thisp, m, m + 1, ((MnViewFn_6298)((sMnGfxVt_6298*)gfx->mVtbl)[43].fn)((char*)gfx + ((sMnGfxVt_6298*)gfx->mVtbl)[43].delta));
    } else {
        sMnGfxVt_6298* vt = (sMnGfxVt_6298*)gfx->mVtbl;
        char* thisp = (char*)gfx + vt[93].delta;
        sMnVec_6298 b[2];
        b[0].x = OBJ_6298(self)->pos[0];
        b[0].y = OBJ_6298(self)->pos[1];
        b[0].z = OBJ_6298(self)->pos[2];
        b[0].w = 1.0f;
        b[1].x = OBJ_6298(self)->scl[0];
        b[1].y = OBJ_6298(self)->scl[1];
        b[1].z = OBJ_6298(self)->scl[2];
        b[1].w = 1.0f;
        res = ((MnCullFn_6298)vt[93].fn)(thisp, &b[0], &b[1], ((MnViewFn_6298)((sMnGfxVt_6298*)gfx->mVtbl)[43].fn)((char*)gfx + ((sMnGfxVt_6298*)gfx->mVtbl)[43].delta));
    }
    if (res == 1) {
        return;
    }
    flags |= 0x400;
    if (res) {
        flags |= 0x20;
    }
    if (!mat) {
        if (func_002D2058(OBJ_6298(self)->box)) {
            return;
        }
    }
    void* rot = 0;
    void* tex;
    void* xf;
    if (((cMnNode_6298*)self)->v26()) {
        tex = ((cMnNode_6298*)self)->v27();
        xf = t4mat_Identity;
    } else {
        tex = 0;
        xf = ((cMnNode_6298*)self)->v24();
    }
    sMnMat_6298 rm;
    if (HasRot_6298(MOD_6298(self))) {
        rot = RotOf_6298(((cMnNode_6298*)self), &rm);
        flags |= 2;
    }
    void* light = HasLight_6298(MOD_6298(self)) ? LightOf_6298(((cMnNode_6298*)self)) : OBJ_6298(self)->light;
    sMnGfxVt_6298* vt = (sMnGfxVt_6298*)gfx->mVtbl;
    ((MnDrawFn_6298)vt[95].fn)((char*)gfx + vt[95].delta, OBJ_6298(self), light, xf, tex, rot, flags, self->Fade);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_renderModel);
#ifdef SKIP_ASM
/**
 * Draws the node's model with caller-supplied animation matrices and draw flags.
 *
 * Returns early when the same world-cache visibility test as cMoveNode_render() fails. Otherwise draws through
 * the graphics manager's virtual slot 95 with an identity root transform, the modifier block's texture matrix
 * (adding flag 2) and light override when present, and the node's Fade (+0x24).
 *
 * @param tex    the animation matrices to draw with (On Tour renderModel's animMats)
 * @param flags  draw flags passed on to the graphics manager
 */
// Draws the node with a caller-supplied texture and flags (visibility, rotation and light as in cMoveNode_render).
extern "C" void cMoveNode_renderModel(cMoveNode* self, void* tex, int flags)
{
    sMnCam_6298* cam = (sMnCam_6298*)cWorld_m_pTheWorld->m_pWorldCache;
    sMnObj_6298* obj = OBJ_6298(self);
    if (!Visible_6298(cam, obj)) {
        return;
    }
    void* rot = 0;
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    sMnMat_6298 rm;
    if (HasRot_6298(MOD_6298(self))) {
        rot = RotOf_6298(((cMnNode_6298*)self), &rm);
        flags |= 2;
    }
    void* light = HasLight_6298(MOD_6298(self)) ? LightOf_6298(((cMnNode_6298*)self)) : OBJ_6298(self)->light;
    sMnGfxVt_6298* vt = (sMnGfxVt_6298*)gfx->mVtbl;
    ((MnDrawFn_6298)vt[95].fn)((char*)gfx + vt[95].delta, OBJ_6298(self), light, t4mat_Identity, tex, rot, flags, self->Fade);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_setupOverlapSystem);
#ifdef SKIP_ASM
struct tModifierBlock;
extern "C" void cInstanceNode_getBoundBoxInfo(void* self, void* box, float* radius);
// PORT: these take the box / radius as a second argument; the symbols are mangled with one parameter.
void tModifierBlock_setBoundBox_K(tModifierBlock* mb, void* box) __asm__("tModifierBlock_setBoundBox__FP14tModifierBlock");
void tModifierBlock_setRadius_K(tModifierBlock* mb, float r) __asm__("tModifierBlock_setRadius__FP14tModifierBlock");

struct sBox_3567 {
    float v[8];
} __attribute__((aligned(16)));

/**
 * Copies the instance's bounding box and radius into the node's modifier block.
 *
 * Does nothing unless the node has a modifier block that holds a transform modifier. The box and radius come
 * from cInstanceNode_getBoundBoxInfo().
 *
 * @param self_  the move node
 */
// Copies the instance's bounding box and radius into its modifier block, when it has one with data.
extern "C" void cMoveNode_setupOverlapSystem(cMoveNode* self_)
{
    int* mb = (int*)self_->ModifierBlock;
    int n = mb != 0;
    if (n)
        n = *mb;
    else
        n = 0;
    if (n) {
        sBox_3567 box;
        float radius;
        cInstanceNode_getBoundBoxInfo(((char*)self_), &box, &radius);
        tModifierBlock_setBoundBox_K(self_->ModifierBlock, &box);
        tModifierBlock_setRadius_K(self_->ModifierBlock, radius);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_restoreOverlapSystem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
#include "common.h"
struct tModifierBlock;
void tModifierBlock_setBoundBox_K(tModifierBlock* mb, void* box) __asm__("tModifierBlock_setBoundBox__FP14tModifierBlock");
struct sMnVec4_67E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMnBox_67E0 {
    sMnVec4_67E0 min, max;
};

extern "C" sMnBox_67E0* tModifierBlock_getBoundBox_67E0(void* mb) __asm__("tModifierBlock_getBoundBox");
extern "C" void* cObjectInterface_getNash(void);
extern "C" void nash_Nash_movenode_67E0(void* mgr, int a, void* obj, sMnBox_67E0* box, sMnBox_67E0* old) __asm__("nash_Nash_movenode");

/**
 * Puts the node back in the overlap system with the object's own bound box, moving it from the old box.
 *
 * @param vself  the node
 */
// Sets the modifier block's bound box from the node's own extents (self->18 + 0x60) and re-files it.
extern "C" void cMoveNode_restoreOverlapSystem(cMoveNode* vself)
{
    char** h = (char**)vself->ModifierBlock;
    char* blk = h ? *h : 0;
    // Binding a reference to the object matches the target register allocation (see cMoveNode_getRootTransform).
    char*& o = blk;
    if (o == 0)
        return;
    sMnBox_67E0 box;
    sMnBox_67E0 old = *tModifierBlock_getBoundBox_67E0(vself->ModifierBlock);
    char* n = (char*)vself->Instance;
    sMnVec4_67E0 v;
    v.x = *(float*)(n + 0x60);
    v.y = *(float*)(n + 0x64);
    v.z = *(float*)(n + 0x68);
    v.w = 1.0f;
    box.min = v;
    v.x = *(float*)(n + 0x6C);
    v.y = *(float*)(n + 0x70);
    v.z = *(float*)(n + 0x74);
    box.max = v;
    tModifierBlock_setBoundBox_K(vself->ModifierBlock, &box);
    void* obj = vself->Instance;
    nash_Nash_movenode_67E0(cObjectInterface_getNash(), 0, obj, &box, &old);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_updateOverlapSystem);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sMnVec4_68B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMnBox_68B0 {
    sMnVec4_68B0 min, max;
};

struct sMnVEntM_68B0 {
    short delta;
    short index;
    sMnVec4_68B0* (*fn)(void*);
};
struct sMnVEnt_68B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" sMnBox_68B0* tModifierBlock_getBoundBox_68B0(void* mb) __asm__("tModifierBlock_getBoundBox");
extern "C" float tModifierBlock_getRadius(void* mb);
extern "C" void* cObjectInterface_getNash(void);
extern "C" void nash_Nash_movenode(void* mgr, int a, void* obj, sMnBox_68B0* box, sMnBox_68B0* old);

// PORT: PS2-only VU0 inline asm (a - b).
static inline void vu0Sub_68B0(sMnVec4_68B0& out, const sMnVec4_68B0& a, const sMnVec4_68B0& b)
{
    sMnVec4_68B0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    out = r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline void vu0Add_68B0(sMnVec4_68B0& out, const sMnVec4_68B0& a, const sMnVec4_68B0& b)
{
    sMnVec4_68B0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    out = r;
}

// World position: translation row of the node's transform (virtual slot 24).
static inline sMnVec4_68B0 pos_68B0(char* self)
{
    sMnVEntM_68B0* vt = *(sMnVEntM_68B0**)(self + 0xC);
    return vt[24].fn(self + vt[24].delta)[3];
}

static inline void VCall24_68B0(char*& o)
{
    sMnVEnt_68B0* vt = *(sMnVEnt_68B0**)o;
    vt[24].fn(o + vt[24].delta);
}

/**
 * Refits the node's overlap box around its position with the root transform's radius and re-files it in the
 * overlap system.
 */
// Refits the modifier block's bound box around the node's position (radius from the block) and re-files it.
extern "C" void cMoveNode_updateOverlapSystem(cMoveNode* self)
{
    char** h = (char**)self->ModifierBlock;
    char* o = h ? *h : 0;
    if (o == 0)
        return;
    sMnBox_68B0 box;
    sMnBox_68B0 old = *tModifierBlock_getBoundBox_68B0(self->ModifierBlock);
    float r = tModifierBlock_getRadius(self->ModifierBlock);
    sMnVec4_68B0 rv;
    rv.w = 0.0f;
    rv.x = r;
    rv.y = r;
    rv.z = r;
    sMnVec4_68B0 v;
    vu0Sub_68B0(v, pos_68B0(((char*)self)), rv);
    box.min = v;
    vu0Add_68B0(v, pos_68B0(((char*)self)), rv);
    box.max = v;
    tModifierBlock_setBoundBox_K(self->ModifierBlock, &box);
    void* obj = self->Instance;
    nash_Nash_movenode(cObjectInterface_getNash(), 0, obj, &box, &old);
    VCall24_68B0(o);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_hasBoundBox);
#ifdef SKIP_ASM
extern "C" int tModifierBlock_getBoundBox(void*);

/**
 * Tells whether the node's modifier block provides a bounding box.
 *
 * @return 1 when the node has a modifier block whose tModifierBlock_getBoundBox() is non-null, otherwise 0
 */
extern "C" int cMoveNode_hasBoundBox(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        return tModifierBlock_getBoundBox(((void*)self->ModifierBlock)) != 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_getBoundBox);
#ifdef SKIP_ASM
extern "C" int tModifierBlock_getBoundBox(void*);

/**
 * Returns the bounding box of the node's modifier block.
 *
 * @return tModifierBlock_getBoundBox() of the block at +0x1C, or 0 when the node has no modifier block
 */
extern "C" int cMoveNode_getBoundBox(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        return tModifierBlock_getBoundBox(((void*)self->ModifierBlock));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_hasIntersect);
#ifdef SKIP_ASM
struct sMoveNodeVEntryA28 {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Tells whether the node can be intersected, asking its transform modifier.
 *
 * @return 0 without a modifier block or transform modifier, otherwise the modifier's hasIntersect() (vtable
 *         slot 20)
 */
extern "C" int cMoveNode_hasIntersect(cMoveNode* self)
{
    void* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryA28* vt = *(sMoveNodeVEntryA28**)node;
    return vt[20].fn((char*)node + vt[20].delta);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_intersect);
#ifdef SKIP_ASM
struct sMoveNodeVEntryA70 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int, int);
};

/**
 * Intersects against the node's root transform modifier (slot 21), if it has one.
 *
 * @param w  the intersection query (On Tour cWorldIntersectType&, passed as int)
 * @param intInfo  receives the hits (On Tour tIntersectInfo*, passed as int)
 * @param num  maximum number of hits
 * @param flags  intersection flags
 * @return the number of hits, or 0 without a root transform
 */
extern "C" int cMoveNode_intersect(cMoveNode* self, int w, int intInfo, int num, int flags)
{
    void* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryA70* vt = *(sMoveNodeVEntryA70**)node;
    return vt[21].fn((char*)node + vt[21].delta, w, intInfo, num, flags);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_applyForce);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_applyForce(void*);

/**
 * Passes a force to the node's modifier block through tModifierBlock_applyForce(), when it has one.
 *
 * The intersect info and force (On Tour applyForce(intInfo, force)) pass through in their registers.
 */
extern "C" void cMoveNode_applyForce(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        tModifierBlock_applyForce(((void*)self->ModifierBlock));
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_updateIntersectInfo);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_updateIntersectInfo(void*);

/**
 * Lets the node's modifier block fill in intersect info through tModifierBlock_updateIntersectInfo().
 *
 * Does nothing without a modifier block. The intersect info (On Tour updateIntersectInfo(intInfo)) passes
 * through in its register.
 */
extern "C" void cMoveNode_updateIntersectInfo(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        tModifierBlock_updateIntersectInfo(((void*)self->ModifierBlock));
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_signal);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_signal(void*);

/**
 * Passes a signal to the node's modifier block (tModifierBlock_signal, On Tour tModifierBlock::signal), when it has
 * one.
 *
 * The signal number and argument (On Tour signal(int signal, float arg)) pass through in their registers.
 */
extern "C" void cMoveNode_signal(cMoveNode* self)
{
    if (((void*)self->ModifierBlock) != 0) {
        tModifierBlock_signal(((void*)self->ModifierBlock));
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_56B30 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);
extern "C" void tModifierBlock_writeToReplayFrame(void* block, void* stream);

// PORT: the unit declares cMoveNode_writeToReplayFrame as `void* (void*)` for its callers; the body writes
// to the stream in a1, so the 2-arg body is bound by asm label.
void cMoveNode_writeToReplayFrame_2(void* self, void* stream) __asm__("cMoveNode_writeToReplayFrame");

/**
 * Writes a moving node to a replay frame: the instance-node part, debounce, fade, transfer flag and its
 * modifiers.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
void cMoveNode_writeToReplayFrame_2(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sSerVEntry_56B30* e = &(*(sSerVEntry_56B30**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x20, 4);
    e = &(*(sSerVEntry_56B30**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x24, 4);
    e = &(*(sSerVEntry_56B30**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x28, 4);
    int flag = *(void**)((char*)self + 0x1C) != 0;
    e = &(*(sSerVEntry_56B30**)stream)[1];
    e->fn((char*)stream + e->delta, &flag, 4);
    void* block = *(void**)((char*)self + 0x1C);
    if (block != 0) {
        tModifierBlock_writeToReplayFrame(block, stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_hasFocusCount);
#ifdef SKIP_ASM
struct sMnVEntryI_6BF0 {
    short delta;
    short index;
    int (*fn)(void*);
};
static inline int VCall_6BF0(char* o, int i)
{
    sMnVEntryI_6BF0* vt = *(sMnVEntryI_6BF0**)o;
    return vt[i].fn(o + vt[i].delta);
}
/**
 * Tells whether the node keeps a focus count, which only a type-2 root transform modifier does.
 *
 * @return 1 when it does, otherwise 0
 */
// Does the modifier object's virtual slot 26 report state 2?
extern "C" int cMoveNode_hasFocusCount(cMoveNode* self)
{
    char** h = (char**)self->ModifierBlock;
    if (h != 0) {
        char* o = *h;
        if (o != 0 && VCall_6BF0(o, 26) == 2)
            return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_incFocusCount);
#ifdef SKIP_ASM
struct sMnVEntryI6C48 {
    short delta;
    short index;
    int (*fn)(void*);
};

// PORT: func_0035AAD0 takes one argument, but this caller passes a second ($5 = a1);
// the original declaration it was compiled against must have had two parameters.
int func_0035AAD0_2(void* self, void* a1) __asm__("func_0035AAD0__FPv");

/**
 * Increments the focus count of a type-2 root transform modifier.
 *
 * @param pInst  the instance gaining focus (unused by the counter)
 * @return the new count, or 0 when the node keeps no focus count
 */
extern "C" int cMoveNode_incFocusCount(cMoveNode* self, void* pInst)
{
    void* block = ((void*)self->ModifierBlock);
    if (block != 0) {
        void* node = *(void**)block;
        if (node != 0) {
            sMnVEntryI6C48* vt = *(sMnVEntryI6C48**)node;
            if (vt[26].fn((char*)node + vt[26].delta) == 2) {
                return func_0035AAD0_2(*(void**)((void*)self->ModifierBlock), pInst);
            }
        }
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_decFocusCount);
#ifdef SKIP_ASM
struct sMnVEntryI6CC8 {
    short delta;
    short index;
    int (*fn)(void*);
};

// PORT: func_0035AAE0 takes one argument, but this caller passes a second ($5 = a1);
// the original declaration it was compiled against must have had two parameters.
int func_0035AAE0_2(void* self, void* a1) __asm__("func_0035AAE0__FPv");

/**
 * Decrements the focus count of a type-2 root transform modifier.
 *
 * @param pInst  the instance losing focus (unused by the counter)
 * @return the new count, or 0 when the node keeps no focus count
 */
extern "C" int cMoveNode_decFocusCount(cMoveNode* self, void* pInst)
{
    void* block = ((void*)self->ModifierBlock);
    if (block != 0) {
        void* node = *(void**)block;
        if (node != 0) {
            sMnVEntryI6CC8* vt = *(sMnVEntryI6CC8**)node;
            if (vt[26].fn((char*)node + vt[26].delta) == 2) {
                return func_0035AAE0_2(*(void**)((void*)self->ModifierBlock), pInst);
            }
        }
        return 0;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_getFocusCount);
#ifdef SKIP_ASM
/**
 * Returns the focus count of a node driven by a multi-spline modifier.
 *
 * Only a transform modifier whose getTransformType() (slot 26) is 2, cMultiSplineModifier, has one: the word at
 * +0x34 of that modifier.
 *
 * @return the modifier's count, or 0 when there is no modifier block, no modifier or another modifier type
 */
extern "C" int cMoveNode_getFocusCount(cMoveNode* self)
{
    void* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    void* node = *(void**)block;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryI* vt = *(sMoveNodeVEntryI**)node;
    if (vt[26].fn((char*)node + vt[26].delta) != 2) {
        return 0;
    }
    return *(int*)((char*)*(void**)self->ModifierBlock + 0x34);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356DB0);
#ifdef SKIP_ASM
struct sMoveFlags_6DB0 {
    char pad_0x00[0x8];
    unsigned int w;     // 0x8
};

extern void* D_00490E80[];
extern "C" void* func_00355280(void* self, void* a1, int type, void* a3);

extern "C" void* func_00356DB0(void* self, void* a1, void* a2, int a3, int a4)
{
    func_00355280(self, a1, 0x11, a2);
    *(void***)((char*)self + 0xC) = D_00490E80;
    if (a4 != 0 || ((*(sMoveFlags_6DB0**)((char*)self + 0x18))->w & 3) == 3) {
        sMoveFlags_6DB0* f = *(sMoveFlags_6DB0**)((char*)self + 0x18);
        f->w = (f->w & ~2) | 4;
    }
    if (a3 != 0) {
        sMoveFlags_6DB0* f = *(sMoveFlags_6DB0**)((char*)self + 0x18);
        f->w = (f->w & ~0x40) | 0x20;
    }
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00356E60);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self);
extern void* D_00490E80[];

extern "C" void* func_00356E60(void* self)
{
    cMoveNode_cMoveNode(self);
    *(void***)((char*)self + 0xC) = D_00490E80;
    return self;
}
#endif

extern "C" void* cMoveNode_writeToReplayFrame(void* self);

//99.29%
INCLUDE_ASM("object/movenode", func_00356E98__FPv);
#ifdef SKIP_ASM
void* func_00356E98(void* self)
{
    return cMoveNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356EB8);
#ifdef SKIP_ASM
struct sMoveQuad {
    float v[4];
} __attribute__((aligned(16)));

extern void* cPositionModifier_vtbl[];
extern sMoveQuad t4mat_Identity[];

extern "C" void* func_00356EB8(cPositionModifier* self, sMoveQuad* pos)
{
    *(void***)self = cPositionModifier_vtbl;
    *(sMoveQuad*)&self->Offset = *pos;
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, t4mat_Identity -> self+0x50).
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x50), "r"(t4mat_Identity)
        : "memory");
    self->RootTransformDirty = 1;
    *(sMoveQuad*)&self->Transform.row[3] = *(sMoveQuad*)&self->Offset;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356F10);
#ifdef SKIP_ASM
struct sMoveQuad2 {
    float v[4];
} __attribute__((aligned(16)));

extern void* cPositionModifier_vtbl[];

extern "C" void* func_00356F10(cPositionModifier* self, void* src)
{
    *(void***)self = cPositionModifier_vtbl;
    sMoveQuad2 v = *(sMoveQuad2*)((char*)src + 0x30);
    *(sMoveQuad2*)&self->Offset = v;
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, src -> self+0x50).
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x50), "r"(src)
        : "memory");
    self->RootTransformDirty = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00356F68);
#ifdef SKIP_ASM
struct sMnVEntrySer6F68 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// PORT: callers declare this void; the body returns self, so bind it via an asm label.
extern "C" void* func_00356F68_ret(void* self, void* stream) __asm__("func_00356F68");
extern "C" void* func_00356F68_ret(void* self, void* stream)
{
    *(void***)self = cPositionModifier_vtbl;
    sMnVEntrySer6F68* vt = *(sMnVEntrySer6F68**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x10, 0x40);
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, t4mat_Identity -> self+0x50).
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x50), "r"(t4mat_Identity)
        : "memory");
    *(int*)((char*)self + 0x44) = 1;
    return self;
}
#endif

typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_updateRootTransform);
#ifdef SKIP_ASM
/**
 * Rebuilds the position modifier's transform: Offset goes into the matrix's row 3 (translation) and the dirty flag
 * is cleared.
 */
extern "C" void cPositionModifier_updateRootTransform(cPositionModifier* self)
{
    cQuad128 v = *(cQuad128*)&self->Offset;
    self->RootTransformDirty = 0;
    *(cQuad128*)&self->Transform.row[3] = v;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_writeToReplayFrame);
#ifdef SKIP_ASM
class cPositionModifier_writeToReplayFrame_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

/**
 * Saves a position modifier to a replay frame: the 0x40 bytes from +0x10 (BoundBox, Offset, Radius,
 * RootTransformDirty) through the frame's write (slot 1).
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cPositionModifier_writeToReplayFrame(cPositionModifier* self, cPositionModifier_writeToReplayFrame_cObj* obj)
{
    obj->v01((char*)self + 0x10, 0x40);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357038);
#ifdef SKIP_ASM
extern char cParentModifier_vtbl[];

extern "C" void* func_00357038(cParentModifier* self, void* a1, void* a2, sMoveQuad* pos)
{
    func_00356EB8((cPositionModifier*)self, pos);
    self->mVtbl = (void**)cParentModifier_vtbl;
    self->ParentInst = (tInstanceRecord*)a1;
    self->Mesh = (int)a2;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParentModifier_cParentModifier);
#ifdef SKIP_ASM
struct sMnVEntrySer7090 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMnVEntryRd7090 {
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void func_00356F68(void* self, void* stream);
extern char cParentModifier_vtbl[];

/**
 * Constructs a parent modifier from a replay frame: the position-modifier part, then Mesh and ParentInst.
 *
 * Runs func_00356F68 (On Tour cPositionModifier(cBxReplayFrame*)), stores cParentModifier_vtbl, reads 4 bytes into
 * Mesh (+0x90, frame slot 2) and ParentInst (slot 3).
 *
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cParentModifier_cParentModifier(cParentModifier* self, void* stream)
{
    func_00356F68(self, stream);
    self->mVtbl = (void**)cParentModifier_vtbl;
    sMnVEntrySer7090* e = &(*(sMnVEntrySer7090**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x90, 4);
    sMnVEntryRd7090* r = &(*(sMnVEntryRd7090**)stream)[3];
    self->ParentInst = (tInstanceRecord*)r->fn((char*)stream + r->delta);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cParentModifier_updateRootTransform);
#ifdef SKIP_ASM
struct sMnVec4_357108 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMnMtx_357108 {
    sMnVec4_357108 r[4];
};

extern "C" void func_0034FED8(void* model, int bone, sMnMtx_357108* out);

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMtx_357108(void* dst, void* src)
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

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sMnVec4_357108 vu0MtxApply_357108(sMnMtx_357108* m, sMnVec4_357108* v)
{
    sMnVec4_357108 out;
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
        : "m"(*v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sMnVec4_357108 vu0Add_357108(const sMnVec4_357108& a, const sMnVec4_357108& b)
{
    sMnVec4_357108 r;
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

static inline sMnVec4_357108 vel_357108(char* self)
{
    return *(sMnVec4_357108*)(self + 0x80);
}

struct sMnVEntryI_357108 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMnVEntryM_357108 {
    short delta;
    short index;
    sMnMtx_357108* (*fn)(void*, int);
};

/**
 * Follows the parent instance: copies the parent mesh's matrix and places the offset in its space.
 *
 * With the parent's node set (+0xC), Transform is the node's mesh matrix for Mesh (slot 29) when its slot 26 is
 * nonzero, else func_0034FED8 (On Tour cInstanceNode::buildMeshMat) builds it; row 3 becomes that position plus the
 * matrix applied to Offset (the local view's vel_357108 is Transform row 3, +0x80). Clears RootTransformDirty in
 * any case.
 */
extern "C" void cParentModifier_updateRootTransform(cParentModifier* self)
{
    char* o = *(char**)((char*)self->ParentInst + 0xC);
    if (o != 0) {
        sMnVEntryI_357108* vi = *(sMnVEntryI_357108**)(o + 0xC);
        if (vi[26].fn(o + vi[26].delta) != 0) {
            sMnVEntryM_357108* vm = *(sMnVEntryM_357108**)(o + 0xC);
            vu0CopyMtx_357108(((char*)self) + 0x50, vm[29].fn(o + vm[29].delta, self->Mesh));
        } else {
            func_0034FED8(self->ParentInst, self->Mesh, (sMnMtx_357108*)(((char*)self) + 0x50));
        }
        *(sMnVec4_357108*)&self->Transform.row[3] = vu0Add_357108(vel_357108(((char*)self)), vu0MtxApply_357108((sMnMtx_357108*)(((char*)self) + 0x50), (sMnVec4_357108*)(((char*)self) + 0x30)));
    }
    self->RootTransformDirty = 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cParentModifier_updateIntersectInfo);
#ifdef SKIP_ASM
struct sMnVEntry7210 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Lets the parent instance's node fill an intersection record as if it hit Mesh.
 *
 * Temporarily stores Mesh in the record's mesh index (+0x5C), calls the parent node's slot 42 with it, then
 * restores the index. Does nothing without a parent or its node.
 *
 * @param obj  the intersection record (On Tour tIntersectInfo& intInfo)
 */
extern "C" void cParentModifier_updateIntersectInfo(cParentModifier* self, void* obj)
{
    void* p = self->ParentInst;
    if (p == 0) {
        return;
    }
    void* q = *(void**)((char*)p + 0xC);
    if (q == 0) {
        return;
    }
    int saved = *(int*)((char*)obj + 0x5C);
    *(int*)((char*)obj + 0x5C) = self->Mesh;
    sMnVEntry7210* vt = *(sMnVEntry7210**)((char*)q + 0xC);
    vt[42].fn((char*)q + vt[42].delta, obj);
    *(int*)((char*)obj + 0x5C) = saved;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParentModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sMnVEntry7278a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMnVEntry7278b {
    short delta;
    short index;
    void (*fn)(void*, void*);
};


/**
 * Saves a parent modifier to a replay frame: the position-modifier state, Mesh (+0x90) and ParentInst
 * (writeReplayInstance, slot 5).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cParentModifier_writeToReplayFrame(cParentModifier* self, void* stream)
{
    cPositionModifier_writeToReplayFrame((cPositionModifier*)self, (cPositionModifier_writeToReplayFrame_cObj*)stream);
    sMnVEntry7278a* vt1 = *(sMnVEntry7278a**)stream;
    vt1[1].fn((char*)stream + vt1[1].delta, (char*)self + 0x90, 4);
    sMnVEntry7278b* vt2 = *(sMnVEntry7278b**)stream;
    vt2[5].fn((char*)stream + vt2[5].delta, self->ParentInst);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_003572E0);
#ifdef SKIP_ASM
extern char cMagnetModifier_vtbl[];

extern "C" void* func_003572E0(cMagnetModifier* self, void* a1, void* mat, float f40, float speed)
{
    *(void**)self = cMagnetModifier_vtbl;
    self->Instance = (tInstanceRecord*)a1;
    self->Radius = f40;
    self->Speed = speed * 27.77777862548828f;
    self->Being = -1;
    *(int*)&self->Time = 0;
    self->AllowCollide = 0;
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy, mat -> self+0x60).
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(((char*)self) + 0x60), "r"(mat)
        : "memory");
    self->RootTransformDirty = 1;
    sMoveQuad v = *(sMoveQuad*)&self->Transform.row[3];
    *(sMoveQuad*)&self->Offset = v;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357358);
#ifdef SKIP_ASM
struct sMnVEntrySer7358 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMnVEntryRd7358 {
    short delta;
    short index;
    void* (*fn)(void*);
};

extern char cMagnetModifier_vtbl[];

extern "C" void* func_00357358(cMagnetModifier* self, void* stream)
{
    char* m = (char*)self + 0x10;
    *(void**)self = cMagnetModifier_vtbl;
    sMnVEntryRd7358* r = &(*(sMnVEntryRd7358**)stream)[3];
    self->Instance = (tInstanceRecord*)r->fn((char*)stream + r->delta);
    sMnVEntrySer7358* e = &(*(sMnVEntrySer7358**)stream)[2];
    e->fn((char*)stream + e->delta, m, 0x50);
    e = &(*(sMnVEntrySer7358**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x60, 0x40);
    self->RootTransformDirty = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_update);
#ifdef SKIP_ASM
extern "C" float func_002D1C70();
extern "C" void* func_002D1B58(int id);

struct sMnVec4_3573F8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a - b).
static inline sMnVec4_3573F8 vu0Sub_3573F8(const sMnVec4_3573F8& a, const sMnVec4_3573F8& b)
{
    sMnVec4_3573F8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_3573F8(const sMnVec4_3573F8& v)
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
static inline sMnVec4_3573F8 vu0Scale_3573F8(const sMnVec4_3573F8& v, float s)
{
    sMnVec4_3573F8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += v).
static inline void vu0AddTo_3573F8(sMnVec4_3573F8& dst, const sMnVec4_3573F8& v)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(v));
}

struct sMnMover_3573F8 {
    char pad0[0x10];
    sMnVec4_3573F8 pos;
    char pad20[0x24];
    int target;
    float speed;
    float time;
    int moved;
    int arrived;
};

/**
 * Pulls the magnet's object toward the rider it has caught, faster the longer it chases.
 *
 * With a rider attached (Being >= 0) Time grows by the frame time; d = the rider's position (func_002D1B58) -
 * Offset. Within 50 units AllowCollide is set; otherwise Offset moves by d * Speed * Time / |d|, clamped to d
 * (which also sets AllowCollide). Marks the root transform dirty.
 */
extern "C" void cMagnetModifier_update(cMagnetModifier* self)
{
    if (self->Being < 0) {
        return;
    }
    self->Time += func_002D1C70();
    sMnVec4_3573F8 d = vu0Sub_3573F8(*(sMnVec4_3573F8*)func_002D1B58(self->Being), (*(sMnVec4_3573F8*)&self->Offset));
    float len = vu0Length_3573F8(d);
    if (len < 50.0f) {
        self->AllowCollide = 1;
    } else {
        sMnVec4_3573F8 step = vu0Scale_3573F8(d, self->Speed / len * self->Time);
        if (len < vu0Length_3573F8(step)) {
            self->AllowCollide = 1;
            step = d;
        }
        vu0AddTo_3573F8((*(sMnVec4_3573F8*)&self->Offset), step);
    }
    self->RootTransformDirty = 1;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_updateRootTransform);
#ifdef SKIP_ASM
/**
 * Rebuilds the magnet's transform: Offset goes into the matrix's row 3 (translation) and the dirty flag is cleared.
 */
extern "C" void cMagnetModifier_updateRootTransform(cMagnetModifier* self)
{
    cQuad128 v = *(cQuad128*)&self->Offset;
    self->RootTransformDirty = 0;
    *(cQuad128*)&self->Transform.row[3] = v;
}
#endif

INCLUDE_ASM("object/movenode", cMagnetModifier_intersect);

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_runCollide);
#ifdef SKIP_ASM
struct sMnVEntryI7660 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void* cObjectInterface_getNumBeings(int id);

/**
 * Decides whether the magnet object collides with a rider, catching the first human rider that touches it.
 *
 * With a rider attached only that rider may collide, once AllowCollide is set. Otherwise the rider
 * (cObjectInterface_getNumBeings(id), which returns the being, not a count) is attached when its slot 8 (On Tour
 * cBeing::isHuman) is set.
 *
 * @param id  the rider's being index (On Tour being)
 * @return AllowCollide for the attached rider, otherwise 0
 */
extern "C" int cMagnetModifier_runCollide(cMagnetModifier* self, int id)
{
    int cur = self->Being;
    if (cur >= 0) {
        if (cur == id) {
            return self->AllowCollide;
        }
        return 0;
    }
    void* obj = cObjectInterface_getNumBeings(id);
    sMnVEntryI7660* vt = *(sMnVEntryI7660**)obj;
    if (vt[8].fn((char*)obj + vt[8].delta) != 0) {
        self->Being = id;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sMnVEntryPtr76D0 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sMnVEntrySer76D0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Saves a magnet modifier to a replay frame: Instance (writeReplayInstance, slot 5), the 0x50 bytes from +0x10
 * (Offset through AllowCollide) and Transform (0x40 bytes from +0x60).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cMagnetModifier_writeToReplayFrame(cMagnetModifier* self, void* stream)
{
    sMnVEntryPtr76D0* p = &(*(sMnVEntryPtr76D0**)stream)[5];
    p->fn((char*)stream + p->delta, self->Instance);
    sMnVEntrySer76D0* e = &(*(sMnVEntrySer76D0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 0x50);
    e = &(*(sMnVEntrySer76D0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x60, 0x40);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357750);
#ifdef SKIP_ASM
extern void* cAvaSplineModifier_vtbl[];

// PORT: PS2-only VU0 inline asm (4x4 matrix copy, src+0x10 -> self+0x40); the PC
// port needs a plain 64-byte copy.
extern "C" void* func_00357750(cAvaSplineModifier* self, void* src)
{
    *(void***)self = cAvaSplineModifier_vtbl;
    self->Instance = (tInstanceRecord*)src;
    *(int*)&self->Radius = 0;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x40), "r"((char*)src + 0x10)
        : "memory");
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357798);
#ifdef SKIP_ASM
struct sMnVEntrySer7798 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMnVEntryRd7798 {
    short delta;
    short index;
    void* (*fn)(void*);
};

// PORT: the project names this func_002D1CF0__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* func_002D1CF0_2(void*, void*) __asm__("func_002D1CF0__FPv");

extern "C" void* func_00357798(cAvaSplineModifier* self, void* stream)
{
    char* m = (char*)self + 0x10;
    *(void***)self = cAvaSplineModifier_vtbl;
    sMnVEntryRd7798* r = &(*(sMnVEntryRd7798**)stream)[3];
    self->Instance = (tInstanceRecord*)r->fn((char*)stream + r->delta);
    sMnVEntrySer7798* e = &(*(sMnVEntrySer7798**)stream)[2];
    e->fn((char*)stream + e->delta, m, 0x30);
    func_002D1CF0_2(self->Instance, (char*)self + 0x40);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_update);
#ifdef SKIP_ASM
// PORT: the project names this func_002D1CF0__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* func_002D1CF0_2(void*, void*) __asm__("func_002D1CF0__FPv");

/**
 * Updates the avalanche spline modifier: fills Transform (+0x40) from its instance (func_002D1CF0, called with two
 * arguments though declared with one).
 */
extern "C" void cAvaSplineModifier_update(cAvaSplineModifier* self)
{
    func_002D1CF0_2(self->Instance, (char*)self + 0x40);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_writeToReplayFrame);
#ifdef SKIP_ASM
struct sMnVEntry7848a {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sMnVEntry7848b {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Saves an avalanche-spline modifier to a replay frame: Instance (writeReplayInstance, slot 5), then the 0x30 bytes
 * from +0x10 (BoundBox, Radius).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cAvaSplineModifier_writeToReplayFrame(cAvaSplineModifier* self, void* stream)
{
    sMnVEntry7848a* vt1 = *(sMnVEntry7848a**)stream;
    vt1[5].fn((char*)stream + vt1[5].delta, self->Instance);
    sMnVEntry7848b* vt2 = *(sMnVEntry7848b**)stream;
    vt2[1].fn((char*)stream + vt2[1].delta, (char*)self + 0x10, 0x30);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_003578A8);
#ifdef SKIP_ASM
extern "C" void* func_00355280(void* self, void* a1, int type, void* a3);
extern "C" void cMoveNode_transferFromOld(cMoveNode* self, void* a1);
extern void* cParticleNode_vtbl[];

extern "C" void* func_003578A8(cMoveNode* self, void* a1, void* a2, void* particle, void* a4)
{
    func_00355280(self, a1, 0xD, a2);
    *(void***)((char*)self + 0xC) = cParticleNode_vtbl;
    cMoveNode_transferFromOld(self, a4);
    cMoveNode_addParticle(self, particle);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParticleNode_cParticleNode);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self);
extern void* cParticleNode_vtbl[];

/**
 * Constructs a particle node: runs the cMoveNode constructor, then stores cParticleNode's vtable at +0xC.
 *
 * On Tour's version takes a bucket and a replay frame; they pass through in their registers to
 * cMoveNode_cMoveNode().
 *
 * @return @p self
 */
extern "C" void* cParticleNode_cParticleNode(void* self)
{
    cMoveNode_cMoveNode(self);
    *(void***)((char*)self + 0xC) = cParticleNode_vtbl;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParticleNode_update);
#ifdef SKIP_ASM
struct sMnDtorVEntry7950 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cMoveNode_update(cMoveNode* self);

/**
 * Updates a particle node and deletes it once nothing keeps it alive.
 *
 * After cMoveNode_update(), returns when the modifier block's word at +0x10 is set (probably the head of its
 * effect link list, so an effect is still attached); otherwise calls the virtual destructor with flags 3.
 */
extern "C" void cParticleNode_update(void* self)
{
    cMoveNode_update((cMoveNode*)self);
    void* mb = *(void**)((char*)self + 0x1C);
    if (mb != 0 && *(int*)((char*)mb + 0x10) != 0) {
        return;
    }
    if (self != 0) {
        sMnDtorVEntry7950* vt = *(sMnDtorVEntry7950**)((char*)self + 0xC);
        vt[1].fn((char*)self + vt[1].delta, 3);
    }
}
#endif

//99.29%
INCLUDE_ASM("object/movenode", cParticleNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes the particle node to the replay frame using the cMoveNode implementation.
 *
 * @return the result of cMoveNode_writeToReplayFrame()
 */
void* cParticleNode_writeToReplayFrame(void* self)
{
    return cMoveNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_003579C8);
#ifdef SKIP_ASM
extern void* D_004A4028;

extern "C" void* func_003579C8(void* self)
{
    int i;
    D_004A4028 = self;
    for (i = 0; i < 4; i++) {
        ((void**)self)[i] = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357A00);
#ifdef SKIP_ASM
extern void* D_004A4028;
extern "C" void func_00357FA0(void* p, int flags);
void operator_delete(int* ptr);

extern "C" void func_00357A00(void* self, int flags)
{
    int i;
    for (i = 0; i < 4; i++) {
        void* p = ((void**)self)[i];
        if (p != 0) {
            func_00357FA0(p, 3);
        }
    }
    D_004A4028 = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357A78);
#ifdef SKIP_ASM
extern char D_0048EA00[];
extern "C" void* func_00357DD8(void* mem, void* a1, void* a2);

extern "C" void func_00357A78(void* self, int i, void* a2, void* a3)
{
    void** slot = (void**)((char*)self + (i << 2));
    if (*slot == 0) {
        *slot = func_00357DD8(cMemMan_alloc(0x50, D_0048EA00, 0x20000000, 0), a2, a3);
    }
}
#endif

extern "C" void* func_003581B8(int, int);

//100%
INCLUDE_ASM("object/movenode", func_00357AE8__FPvii);
#ifdef SKIP_ASM
void* func_00357AE8(void* self, int a1, int a2)
{
    return func_003581B8(*(int*)((char*)self + (a1 << 2)), a2);
}
#endif

extern "C" void* func_003581F0(int, int);

//100%
INCLUDE_ASM("object/movenode", func_00357B10__FPvii);
#ifdef SKIP_ASM
void* func_00357B10(void* self, int a1, int a2)
{
    return func_003581F0(*(int*)((char*)self + (a1 << 2)), a2);
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357B38);
#ifdef SKIP_ASM
extern "C" void func_00357FA0(void* p, int flags);

extern "C" void func_00357B38(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00357FA0(self[i], 3);
        }
        self[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357B90);
#ifdef SKIP_ASM
extern "C" void func_00358140(void* p, void* arg);

extern "C" void func_00357B90(void** self, void* arg)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            func_00358140(self[i], arg);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357BF8);
#ifdef SKIP_ASM
extern "C" void cMultiParticle_update(void* p);

extern "C" void func_00357BF8(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            cMultiParticle_update(self[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_render);
#ifdef SKIP_ASM
extern "C" void cMultiParticle_render(void* p);

/**
 * Renders each of the manager's four multi-particle slots that holds an object.
 */
extern "C" void cMultiParticleMan_render(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            cMultiParticle_render(self[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_writeToReplayFrame);
#ifdef SKIP_ASM
struct sMnVEntrySer7CA8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cMultiParticle_writeToReplayFrame(void* obj, void* stream);

/**
 * Writes the multi-particle manager's four slots to a replay frame, each as a present flag and then its data.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cMultiParticleMan_writeToReplayFrame(void** self, void* stream)
{
    int i;
    for (i = 0; i < 4; i++) {
        int has = self[i] != 0;
        sMnVEntrySer7CA8* e = &(*(sMnVEntrySer7CA8**)stream)[1];
        e->fn((char*)stream + e->delta, &has, 4);
        if (has) {
            cMultiParticle_writeToReplayFrame(self[i], stream);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", func_00357D28);
#ifdef SKIP_ASM
struct sMnVEntrySer7D28 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_00357E80(void* mem, void* stream);

extern "C" void func_00357D28(void** self, void* stream)
{
    int i;
    func_00357B38(self);
    for (i = 0; i < 4; i++) {
        int has;
        sMnVEntrySer7D28* e = &(*(sMnVEntrySer7D28**)stream)[2];
        e->fn((char*)stream + e->delta, &has, 4);
        if (has) {
            self[i] = func_00357E80(cMemMan_alloc(0x50, D_0048EA00, 0x20000000, 0), stream);
        } else {
            self[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357DD8);
#ifdef SKIP_ASM
struct sQuad_7DD8 {
    float v[4];

    sQuad_7DD8() {}
    sQuad_7DD8(float x, float y, float z, float w)
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }
} __attribute__((aligned(16)));

struct sPartDesc_7DD8 {
    char pad_0x000[0xC0];
    float pos[3];           // 0xC0
    char pad_0x0cc[0x94];
    sQuad_7DD8 a;           // 0x160
    sQuad_7DD8 b;           // 0x170
};

struct sMultiPart_7DD8 {
    void* owner;            // 0x0
    int field_0x4;          // 0x4
    void* data;             // 0x8
    int field_0xc;          // 0xC
    sPartDesc_7DD8* desc;   // 0x10
    char pad_0x14[0xC];
    sQuad_7DD8 a;           // 0x20
    sQuad_7DD8 b;           // 0x30
    sQuad_7DD8 pos;         // 0x40
};

extern char D_0048EA10[];
extern "C" void MEM_copy(void* dst, const void* src, int n);
extern "C" void cMultiParticle_setupMultiParticle(void* self);

extern "C" void* func_00357DD8(void* mem, void* a1, void* a2)
{
    sMultiPart_7DD8* self = (sMultiPart_7DD8*)mem;
    self->owner = a2;
    self->field_0x4 = 0;
    void* buf = cMemMan_alloc(0xD8, D_0048EA10, 0x20000000, 0);
    self->data = buf;
    MEM_copy(buf, a1, 0xD8);
    cMultiParticle_setupMultiParticle(self);
    sPartDesc_7DD8* d = self->desc;
    self->a = d->a;
    self->b = d->b;
    self->pos = sQuad_7DD8(d->pos[0], d->pos[1], d->pos[2], 0.0f);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357E80);
#ifdef SKIP_ASM
extern "C" void func_00370AF8(void* self, void* stream);

struct sMnSerVEntry_357E80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

static inline void read_357E80(void* stream, void* dst, int n)
{
    sMnSerVEntry_357E80* vt = *(sMnSerVEntry_357E80**)stream;
    vt[2].fn((char*)stream + vt[2].delta, dst, n);
}

extern "C" void* func_00357E80(void* mem, void* stream)
{
    sMultiPart_7DD8* self = (sMultiPart_7DD8*)mem;
    sQuad_7DD8* ab = &self->a;
    read_357E80(stream, self, 4);
    read_357E80(stream, &self->field_0x4, 4);
    void* buf = cMemMan_alloc(0xD8, D_0048EA10, 0x20000000, 0);
    self->data = buf;
    read_357E80(stream, buf, 0xD8);
    cMultiParticle_setupMultiParticle(self);
    func_00370AF8(self->desc, stream);
    read_357E80(stream, &self->pos, 0x10);
    read_357E80(stream, ab, 0x20);
    if (self->field_0x4 > 0) {
        read_357E80(stream, (void*)self->field_0xc, self->field_0x4 * 4);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", func_00357FA0);
#ifdef SKIP_ASM
struct sMnDtorVEntry7FA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void operator_delete(int* ptr);
void cMemMan_free(void*);

extern "C" void func_00357FA0(void* self, int flags)
{
    operator_delete(*(int**)((char*)self + 0x8));
    void* buf = *(void**)((char*)self + 0xC);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    void* obj = *(void**)((char*)self + 0x10);
    if (obj != 0) {
        sMnDtorVEntry7FA0* vt = *(sMnDtorVEntry7FA0**)((char*)obj + 0x18C);
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

