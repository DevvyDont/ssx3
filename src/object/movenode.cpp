#include "vtable.h"
#include "common.h"
#include "object/being.h"
#include "object/multisplinemodifier.h"
#include "world/worldrecords.h"
#include "world/instancerecord.h"
#include "ai/rider.h"
#include "world/worldview.h"
#include "bx/appman.h"
#include "object/objectnode.h"
#include "render/particle.h"
#include "object/modifierblock.h"
#include "object/instancenode.h"
#include "object/railmodifier.h"
#include "object/physicsmodifier.h"
#include "object/texflipmodifier.h"
#include "object/uvscrollmodifier.h"
#include "object/transformmodifier.h"
#include "bx/math3d.h"
#include "object/particlenode.h"
#include "object/multiparticle.h"
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
 * @param bucket  the bucket the node belongs to (On Tour const int bucket)
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 * @return @p self
 */
void* cMoveNode_cMoveNode_3(void* vself, void* bucket, void* stream)
{
    cMoveNode* self = (cMoveNode*)vself;
    int flag;
    cInstanceNode_cInstanceNode(self, bucket, stream);
    self->ModifierBlock = 0;
    VPTR_OF(cObjNode, self) = (void**)cMoveNode_vtbl;
    sVEntry<void(void*, void*, int)>* readEntry = &(*(sVEntry<void(void*, void*, int)>**)stream)[2];
    readEntry->fn(vthis(stream, *readEntry), (char*)&self->Debounce, 4);
    readEntry = &(*(sVEntry<void(void*, void*, int)>**)stream)[2];
    readEntry->fn(vthis(stream, *readEntry), (char*)&self->Fade, 4);
    readEntry = &(*(sVEntry<void(void*, void*, int)>**)stream)[2];
    readEntry->fn(vthis(stream, *readEntry), (char*)&self->TransferHack, 4);
    readEntry = &(*(sVEntry<void(void*, void*, int)>**)stream)[2];
    readEntry->fn(vthis(stream, *readEntry), &flag, 4);
    if (flag) {
        cMoveNode_addModifierBlock((cMoveNode*)self);
        tModifierBlock_readFromReplayFrame(self->ModifierBlock, stream);
    }
    cMoveNode_setupOverlapSystem((cMoveNode*)self);
    self->Flags |= 1;
    return ((void*)self);
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
    VPTR_OF(cObjNode, self) = (void**)cMoveNode_vtbl;
    cMoveNode_restoreOverlapSystem(self);
    tModifierBlock* block = self->ModifierBlock;
    if (block != 0) {
        tModifierBlock__tModifierBlock(block, 3);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_shouldRebound);
#ifdef SKIP_ASM
/**
 * Tells whether a rider should rebound off this node, asking its transform modifier when it has one.
 *
 * @return 1 when the node has no modifier block or no transform modifier in it, otherwise the modifier's
 *         shouldRebound() (vtable slot 8)
 */
extern "C" int cMoveNode_shouldRebound(cMoveNode* self)
{
    tModifierBlock* block = self->ModifierBlock;
    if (block == 0) {
        return 1;
    }
    cTransformModifier* node = block->RootTransform;
    if (node == 0) {
        return 1;
    }
    return node->shouldRebound();
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
struct tModifierBlock;
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self);
extern const char str_ModifierBlock[];


//100%
INCLUDE_ASM("object/movenode", cMoveNode_addModifierBlock__FP9cMoveNode);
#ifdef SKIP_ASM
/**
 * Gives the node a new, empty modifier block ("ModifierBlock").
 */
void cMoveNode_addModifierBlock(cMoveNode* self)
{
    void* mem = cMemMan_alloc(40, str_ModifierBlock, 0x20000000, 0);
    *(void**)&self->ModifierBlock = tModifierBlock_tModifierBlock((tModifierBlock*)mem);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addTransformModifier);
#ifdef SKIP_ASM

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
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod)
{
    self->restoreOverlapSystem();
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remTransformModifier(((void*)self->ModifierBlock));
    }
    *(void**)self->ModifierBlock = ((void*)mod);
    self->setupOverlapSystem();
    unsigned int* instFlags = (unsigned int*)(&self->Instance->m_iFlags);
    *instFlags = (*instFlags & ~0x20u) | 0x40;
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
extern "C" void cMoveNode_addUVScrollModifier(cMoveNode* self, cUVScrollModifier* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remUVScrollModifier(((void*)self->ModifierBlock));
    }
    self->ModifierBlock->UVScroll = (cUVScrollModifier*)((void*)modifier);
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
extern "C" void cMoveNode_addTexFlipModifier(cMoveNode* self, cTexFlipModifier* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remTexFlipModifier(((void*)self->ModifierBlock));
    }
    self->ModifierBlock->TexFlip = (cTexFlipModifier*)((void*)modifier);
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
extern "C" void cMoveNode_addPhysicsModifier(cMoveNode* self, cPhysicsModifier* modifier)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    } else {
        tModifierBlock_remPhysicsModifier(((void*)self->ModifierBlock));
    }
    self->ModifierBlock->Physics = (cPhysicsModifier*)((void*)modifier);
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
    cEffectLink_add((cEffectLink*)(&self->ModifierBlock->Effect), (cEffectLink*)effect);
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
extern "C" void cMoveNode_addRailModifier(cMoveNode* self, cRailModifier* item)
{
    if (((void*)self->ModifierBlock) == 0) {
        cMoveNode_addModifierBlock(self);
    }
    cRailLink_add(&self->ModifierBlock->Rail, item);
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
extern "C" int cObjectInterface_getAIIndex(int);
void cInstanceNode_runCollideScript(void*, int, int, int);

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
    virtual int collide(int a, int b);
};


/**
 * Handles a collision with the node: the root transform reacts first, then the collision is passed on at most
 * every half second.
 *
 * When the root transform's slot-9 test rejects the colliding being, nothing more happens. Otherwise, once
 * the
 * debounce has run out it is re-armed to half a second and cInstanceNode_runCollideScript forwards the collision (probably to
 * the
 * script system, cWScriptMan_TheWScriptMan).
 *
 * @param tc  collision info (On Tour const tCollideInfo*, passed as int)
 * @param ti  intersection info (On Tour const tIntersectInfo*, passed as int)
 * @param pBeing  the colliding being (cBeing*, passed as int)
 */
extern "C" void cMoveNode_onCollide(cMoveNode* self, int tc, int ti, int pBeing)
{
    cMN_355770_Obj** modBlock = (cMN_355770_Obj**)((void*)self->ModifierBlock);
    if (modBlock != 0) {
        cMN_355770_Obj* obj = *modBlock;
        if (obj != 0) {
            obj->collide(tc, ti);
            char* transformModifier = *(char**)self->ModifierBlock;
            sVEntry<int(void*, int)>* vt = *(sVEntry<int(void*, int)>**)transformModifier;
            if (vt[9].fn(vthis(transformModifier, vt[9]), cObjectInterface_getAIIndex(pBeing)) == 0) {
                return;
            }
        }
    }
    if (self->Debounce <= 0) {
        self->Debounce = cAppMan_TheAppMan->TimerHz / 2;
        cInstanceNode_runCollideScript(self, tc, ti, pBeing);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_setDebounce);
#ifdef SKIP_ASM

/**
 * Sets the node's collision debounce, in seconds, as a count of app timer ticks.
 *
 * @param secs  debounce time in seconds
 */
extern "C" void cMoveNode_setDebounce(cMoveNode* self, float secs)
{
    self->Debounce = (int)((float)cAppMan_TheAppMan->TimerHz * secs);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addFade__FPvT0);
#ifdef SKIP_ASM
/**
 * Sets a move node's fade from add-fade data: Fade (+0x24) = fadeData +0x4.
 *
 * @param fadeData  add-fade make-data
 * @return the new Fade (left in f0; callers ignore it)
 */
float cMoveNode_addFade(void* vself, void* vfadeData)
{
    tAddFadeData* fadeData = (tAddFadeData*)vfadeData;
    cMoveNode* self = (cMoveNode*)vself;
    float fade = fadeData->Fade;
    self->Fade = fade;
    return fade;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addDebounce);
#ifdef SKIP_ASM
/**
 * Sets the node's debounce time from a placement record: setDebounce (slot 41) with the record's float at +0x4.
 *
 * @param data  the debounce data (On Tour const tAddDebounceData& data); +0x4 holds the time in seconds
 */
extern "C" void cMoveNode_addDebounce(cMoveNode* self, tAddDebounceData* data)
{
    self->setDebounce(data->Time);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addPosition);
#ifdef SKIP_ASM
struct sMoveQuad;
extern char str_PositionModifier[];
extern "C" void* cPositionModifier_cPositionModifier(cPositionModifier* self, sMoveQuad* pos);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Gives a move node a fixed position offset.
 *
 * Allocates a 144-byte "PositionModifier", builds it from pos (cPositionModifier_cPositionModifier) and installs it with
 * cMoveNode_addTransformModifier.
 *
 * @param pos  offset to apply
 */
extern "C" void cMoveNode_addPosition(cMoveNode* self, sMoveQuad* pos)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cPositionModifier_cPositionModifier((cPositionModifier*)cMemMan_alloc(144, str_PositionModifier, 0x20000000, 0), pos));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addPosition1);
#ifdef SKIP_ASM
extern char str_PositionModifier[];
extern "C" void* cPositionModifier_cPositionModifier1(cPositionModifier* mem, t4Matrix* desc);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Gives a move node a fixed transform taken from a matrix.
 *
 * Allocates a 144-byte "PositionModifier", builds it from the matrix (cPositionModifier_cPositionModifier1) and installs it with
 * cMoveNode_addTransformModifier.
 *
 * @param desc  matrix (t4Matrix) to use
 */
extern "C" void cMoveNode_addPosition1(cMoveNode* self, void* desc)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cPositionModifier_cPositionModifier1((cPositionModifier*)cMemMan_alloc(144, str_PositionModifier, 0x20000000, 0), (t4Matrix*)desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addParent);
#ifdef SKIP_ASM
struct sMoveQuad;
extern char str_ParentModifier[];
extern "C" void* cParentModifier_cParentModifier1(cParentModifier* self, void* a1, void* a2, sMoveQuad* pos);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Attaches the node to a parent instance: a new cParentModifier ("ParentModifier", cParentModifier_cParentModifier1) as its transform
 * modifier.
 *
 * @param parentinst  the parent instance (On Tour tInstanceRecord* parentinst)
 * @param mesh  the parent's mesh index (On Tour int mesh)
 * @param pos  the offset from the parent (On Tour const t4Vector& offset)
 */
extern "C" void cMoveNode_addParent(cMoveNode* self, void* parentinst, void* mesh, sMoveQuad* pos)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cParentModifier_cParentModifier1((cParentModifier*)cMemMan_alloc(176, str_ParentModifier, 0x20000000, 0), parentinst, mesh, pos));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addMagnet);
#ifdef SKIP_ASM
extern char str_MagnetModifier[];
extern "C" void* cMagnetModifier_cMagnetModifier(cMagnetModifier* self, void* owner, void* a2, float f0, float f1);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Makes the node a magnet: a new cMagnetModifier ("MagnetModifier", cMagnetModifier_cMagnetModifier) on its instance as its transform
 * modifier.
 *
 * @param mat  the magnet's matrix (On Tour const t4Matrix& mat)
 * @param radius  the radius (On Tour radius)
 * @param magnetSpeed  the speed (On Tour speed)
 */
extern "C" void cMoveNode_addMagnet(cMoveNode* self, t4Matrix* mat, float radius, float magnetSpeed)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cMagnetModifier_cMagnetModifier((cMagnetModifier*)cMemMan_alloc(176, str_MagnetModifier, 0x20000000, 0),
                                      self->Instance, mat, radius, magnetSpeed));
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_addAvaSpline);
#ifdef SKIP_ASM
extern int cAvalanche_m_badCookie;
extern char str_AvaSplineModifier[];
extern "C" void* cAvaSplineModifier_cAvaSplineModifier(cAvaSplineModifier* self, tInstanceRecord* src);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Puts a move node on the avalanche spline, unless the avalanche cookie check failed.
 *
 * When cAvalanche_m_badCookie is 0, allocates a 144-byte "AvaSplineModifier" for Instance (cAvaSplineModifier_cAvaSplineModifier) and installs
 * it with cMoveNode_addTransformModifier. Its caller in seg_1FBE38 passes the instance record as a second argument,
 * which this body never reads.
 */
extern "C" void cMoveNode_addAvaSpline(cMoveNode* self)
{
    if (cAvalanche_m_badCookie == 0) {
        cMoveNode_addTransformModifier(self, (cTransformModifier*)cAvaSplineModifier_cAvaSplineModifier((cAvaSplineModifier*)cMemMan_alloc(144, str_AvaSplineModifier, 0x20000000, 0), self->Instance));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addSpline1);
#ifdef SKIP_ASM
extern char str_SplineModifier[];
extern "C" void* cSplineModifier_cSplineModifier1(void* mem, void* desc);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

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
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cSplineModifier_cSplineModifier1(cMemMan_alloc(0xF0, str_SplineModifier, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addSpline);
#ifdef SKIP_ASM
extern char str_MultiSplineModifier[];
extern "C" void* cMultiSplineModifier_cMultiSplineModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Makes a new spline-path modifier ("MultiSplineModifier") the node's root transform.
 *
 * @param desc  the spline description (On Tour const tAddSplineData& sdata)
 */
extern "C" void cMoveNode_addSpline(cMoveNode* self, void* desc)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cMultiSplineModifier_cMultiSplineModifier(cMemMan_alloc(88, str_MultiSplineModifier, 0x20000000, 0), desc,
                                      self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addUVScroll);
#ifdef SKIP_ASM
extern char str_UVScrollModifier[];
extern "C" void* cUVScrollModifier_cUVScrollModifier(void* mem, void* desc);
extern "C" void cMoveNode_addUVScrollModifier(cMoveNode* self, cUVScrollModifier* mod);

/**
 * Adds a UV-scroll modifier built from a placement record: a new "UVScrollModifier" (cUVScrollModifier_cUVScrollModifier) on the node.
 *
 * @param desc  the UV-scroll data (On Tour const tAddUVScrollData& udata)
 */
extern "C" void cMoveNode_addUVScroll(cMoveNode* self, void* desc)
{
    cMoveNode_addUVScrollModifier(self, (cUVScrollModifier*)cUVScrollModifier_cUVScrollModifier(cMemMan_alloc(80, str_UVScrollModifier, 0x20000000, 0), desc));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addTexFlip);
#ifdef SKIP_ASM
extern char str_TexFlipModifier[];
extern "C" void* cTexFlipModifier_cTexFlipModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addTexFlipModifier(cMoveNode* self, cTexFlipModifier* mod);

/**
 * Adds a texture-flip modifier built from a placement record: a new "TexFlipModifier" (cTexFlipModifier_cTexFlipModifier, with the node's
 * instance) on the node.
 *
 * @param desc  the texture-flip data (On Tour const tAddTexFlipData& tdata)
 */
extern "C" void cMoveNode_addTexFlip(cMoveNode* self, void* desc)
{
    cMoveNode_addTexFlipModifier(self, (cTexFlipModifier*)cTexFlipModifier_cTexFlipModifier(cMemMan_alloc(232, str_TexFlipModifier, 0x20000000, 0), desc, self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addBoost);
#ifdef SKIP_ASM
extern char str_BoostModifier[];
extern "C" void* cBoostModifier_cBoostModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addPhysicsModifier(cMoveNode* self, cPhysicsModifier* mod);

/**
 * Adds a boost modifier built from a placement record: a new "BoostModifier" (cBoostModifier_cBoostModifier, with the node's
 * instance) as the node's physics modifier.
 *
 * @param desc  the boost data (On Tour const tAddBoostData& adata)
 */
extern "C" void cMoveNode_addBoost(cMoveNode* self, void* desc)
{
    cMoveNode_addPhysicsModifier(self, (cPhysicsModifier*)cBoostModifier_cBoostModifier(cMemMan_alloc(80, str_BoostModifier, 0x20000000, 0), desc, self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addParticle);
#ifdef SKIP_ASM
extern char str_ParticleModifier[];
extern "C" void* cParticleModifier_cParticleModifier(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

/**
 * Adds a particle-emitter effect ("ParticleModifier") to the node.
 *
 * @param desc  the particle description (On Tour tAddParticleData* pdata)
 */
extern "C" void cMoveNode_addParticle(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, cParticleModifier_cParticleModifier(cMemMan_alloc(0x1F0, str_ParticleModifier, 0x20000000, 0), desc,
                                                    self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addDynamicParticle);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void* cDynamicParticleModifier_cDynamicParticleModifier1(void* mem, void* desc, void* inst, int index, void* a4);
extern const char str_DynamicParticleModifier[];

/**
 * Adds a particle effect tied to one of the node's meshes ("DynamicParticleModifier"); a bad mesh index frees
 * the description instead.
 *
 * @param desc  the particle description (On Tour tAddDynamicParticleData* pdata); owned by the call
 * @param index  the mesh index (On Tour mesh)
 * @param offset  emitter offset from the mesh (On Tour const t4Vector&)
 */
extern "C" void cMoveNode_addDynamicParticle(cMoveNode* self, void* desc, int index, t4Vector* offset)
{
    if (index < 0 || index >= self->Instance->m_Model->m_iNumMeshes) {
        operator_delete((int*)desc);
        return;
    }
    cMoveNode_addEffectModifier(self, cDynamicParticleModifier_cDynamicParticleModifier1(cMemMan_alloc(624, str_DynamicParticleModifier, 0x20000000, 0), desc, self->Instance, index, offset));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addRoller);
#ifdef SKIP_ASM
extern char str_RollerModifier[];
extern "C" void* cRollerModifier_cRollerModifier(void* mem, void* a1, void* a2, void* a3);
extern "C" void cMoveNode_addTransformModifier(cMoveNode* self, cTransformModifier* mod);

/**
 * Makes a new roller modifier ("RollerModifier") the node's root transform.
 *
 * @param rdata  the roller description (On Tour const tAddRollerData&)
 * @param ii  intersection info of the trigger (On Tour const tIntersectInfo*)
 * @param ci  collision info of the trigger (On Tour const tCollideInfo*)
 */
extern "C" void cMoveNode_addRoller(cMoveNode* self, void* rdata, void* ii, void* ci)
{
    cMoveNode_addTransformModifier(self, (cTransformModifier*)cRollerModifier_cRollerModifier(cMemMan_alloc(720, str_RollerModifier, 0x20000000, 0), rdata, ii, ci));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addRail);
#ifdef SKIP_ASM
extern char str_RailModifier[];
extern "C" int tModifierBlock_findRail(void* self, int id);
extern "C" void* cRailModifier_cRailModifier(void* mem, void* desc, void* owner);
extern "C" void cMoveNode_addRailModifier(cMoveNode* self, cRailModifier* item);

/**
 * Adds a rail modifier ("RailModifier") for a rail description, unless the node already has a rail with that
 * ID.
 *
 * @param desc  the rail description (On Tour const tAddRailData& rdata); its ID is at +4
 */
extern "C" void cMoveNode_addRail(cMoveNode* self, tAddRailData* desc)
{
    void* block = ((void*)self->ModifierBlock);
    if (block != 0 && tModifierBlock_findRail(block, (int)desc->SplineId) != 0) {
        return;
    }
    cMoveNode_addRailModifier(self, (cRailModifier*)cRailModifier_cRailModifier(cMemMan_alloc(144, str_RailModifier, 0x20000000, 0), desc,
                                      self->Instance));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMoveNode_addHalo);
#ifdef SKIP_ASM
extern char str_HaloModifier[];
extern "C" void* cHaloModifier_cHaloModifier(void* mem, void* desc, void* owner);
void cMoveNode_addEffectModifier(cMoveNode* self, void* mod);

/**
 * Adds a halo effect ("HaloModifier") to the node.
 *
 * @param desc  the halo description (On Tour const tAddHaloData& hdata)
 */
extern "C" void cMoveNode_addHalo(cMoveNode* self, void* desc)
{
    cMoveNode_addEffectModifier(self, cHaloModifier_cHaloModifier(cMemMan_alloc(0x40, str_HaloModifier, 0x20000000, 0), desc,
                                                    self->Instance));
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_transferFromOld);
#ifdef SKIP_ASM
extern "C" void cMoveNode_transferEffectModifier(cMoveNode* self, cMoveNode* other);


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


/**
 * Takes over an old instance node's effects and transform, then deletes the old node.
 *
 * When the old node has effects (slot 16) they move over (cMoveNode_transferEffectModifier). If its transform (slot
 * 24) is this node's own instance matrix (Instance + 0x10) the old node is just deleted; otherwise the matrix is
 * copied, the old node deleted (keeping Instance's word +0x8), the copy applied with cMoveNode_addPosition1, TransferHack
 * set and virtual slot 50 called.
 *
 * @param srcv  the node being replaced (On Tour cInstanceNode* pOldNode); null is ignored
 */
extern "C" void cMoveNode_transferFromOld(cMoveNode* self, cInstanceNode* srcv)
{
    if (srcv == 0) {
        return;
    }
    if (srcv->isMoveNode()) {
        cMoveNode_transferEffectModifier(self, (cMoveNode*)srcv);
    }
    char* transform = (char*)srcv->getRootTransform();
    if (transform == (char*)&self->Instance->m_fmTransform) {
        delete srcv;
        return;
    }
    t4Matrix copy;
    vu0CopyMatrix_355F10(&copy, transform);
    int keep = self->Instance->m_iFlags;
    delete srcv;
    self->Instance->m_iFlags = keep;
    cMoveNode_addPosition1(self, &copy);
    self->TransferHack = 1;
    self->updateOverlapSystem();
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_transferEffectModifier);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_transferEffectModifier(void* block, void* other);

/**
 * Moves another node's effect links into this node's modifier block, creating the block when needed.
 *
 * Uses tModifierBlock_transferEffectModifier (On Tour tModifierBlock::transferEffectModifier) on the other block's effect link (+0x10).
 *
 * @param other  the node giving up its effects (On Tour oldNode)
 */
extern "C" void cMoveNode_transferEffectModifier(cMoveNode* self, cMoveNode* other)
{
    if (((void*)other->ModifierBlock) != 0) {
        if (((void*)self->ModifierBlock) == 0) {
            cMoveNode_addModifierBlock(self);
        }
        tModifierBlock_transferEffectModifier(((void*)self->ModifierBlock), &other->ModifierBlock->Effect);
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
    return vt[18].fn(vthis(o, vt[18]));
}
/**
 * Returns the node's world transform: its root transform modifier's (slot 18), or the object's own matrix.
 *
 * @return the transform matrix (On Tour const t4Matrix*)
 */
extern "C" void* cMoveNode_getRootTransform(cMoveNode* self)
{
    char** modBlock = (char**)self->ModifierBlock;
    char* transformModifier = modBlock ? *modBlock : 0;
    if (transformModifier != 0)
        return VCall18_6078(transformModifier);
    return (char*)&self->Instance->m_fmTransform;
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
// Taking the object by reference matches the target's register allocation (see cMoveNode_getRootTransform).
static inline int VCall23_60C0(char*& o, int a)
{
    sMnVEntryI2_60C0* vt = *(sMnVEntryI2_60C0**)o;
    return vt[23].fn(vthis(o, vt[23]), a);
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
    char** modBlock = (char**)self->ModifierBlock;
    char* transformModifier = modBlock ? *modBlock : 0;
    if (transformModifier != 0) {
        int animMats = VCall23_60C0(transformModifier, inst);
        if (animMats != 0) return animMats;
    }
    return (int)self->getAnimMats();
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
    return vt[i].fn(vthis(o, vt[i]));
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
    char** modBlock = (char**)self->ModifierBlock;
    char* transformModifier = modBlock ? *modBlock : 0;
    if (transformModifier != 0) {
        void* transform = VCall_6128(transformModifier, 19);
        if (transform != 0) return transform;
        return VCall_6128(transformModifier, 18);
    }
    return (char*)&self->Instance->m_fmTransform;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_update);
#ifdef SKIP_ASM
int cInstanceNode_runOnLoseFocusScript(void* self);
extern "C" int tModifierBlock_isComplete(void* block);
extern "C" void tModifierBlock_clearComplete(void* block);


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
    if (selfv->hasFocusCount() != 0) {
        if (selfv->getFocusCount() <= 0 && cInstanceNode_runOnLoseFocusScript(self) == 0) {
            selfv->loseFocus();
            return;
        }
    }
    cMoveNode_preUpdateModifierBlock(self);
    if (selfv->updateNode() != 0) {
        cMoveNode_postUpdateModifierBlock(self);
        if (((void*)self->ModifierBlock) != 0 && tModifierBlock_isComplete(((void*)self->ModifierBlock)) != 0) {
            tModifierBlock_clearComplete(((void*)self->ModifierBlock));
            selfv->expire(1);
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
    virtual int getTransformType();
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
    virtual void* getRootTransform();
    virtual void v25();
    virtual int hasAnimMats();
    virtual void* getAnimMats();
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
    virtual void* hasBoundBox();
    virtual sMnVec_6298* getBoundBox();
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

typedef t4Matrix (*MnViewFn_6298)(void*);
typedef int (*MnCullFn_6298)(void*, sMnVec_6298*, sMnVec_6298*, const t4Matrix&);
typedef void (*MnDrawFn_6298)(void*, void*, void*, void*, void*, void*, int, float);

extern "C" void tModifierBlock_render(void* self);
extern "C" int cObjectInterface_isSphereOccluded(void* v);
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

static inline void* Mat_6298(void* rot, t4Matrix* m)
{
    void* r;
    if (rot) {
        r = cUVScrollModifier_getTextureMat(rot, m);
    } else {
        r = 0;
    }
    return r;
}

static inline void* LmSub_6298(char* vl)
{
    cTexFlipModifier* l = (cTexFlipModifier*)vl;
    return &l->MaterialBlock;
}

static inline void* Lm_6298(char* l)
{
    if (l == 0) {
        return 0;
    }
    return LmSub_6298(l);
}

static inline void* RotOf_6298(cMnNode_6298* n, t4Matrix* m)
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
        cMnOwner_6298* modOwner = MOD_6298(self)->owner;
        if (modOwner) {
            if (modOwner->getTransformType() == 7) {
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
    void* mat = ((cMnNode_6298*)self)->hasBoundBox();
    int res;
    if (mat) {
        sMnVec_6298* boundBox = ((cMnNode_6298*)self)->getBoundBox();
        sMnGfxVt_6298* vt = (sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx);
        char* thisp = (char*)gfx + vt[93].delta;
        res = ((MnCullFn_6298)vt[93].fn)(thisp, boundBox, boundBox + 1, ((MnViewFn_6298)((sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx))[43].fn)((char*)gfx + ((sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx))[43].delta));
    } else {
        sMnGfxVt_6298* vt = (sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx);
        char* thisp = (char*)gfx + vt[93].delta;
        sMnVec_6298 objBounds[2];
        objBounds[0].x = OBJ_6298(self)->pos[0];
        objBounds[0].y = OBJ_6298(self)->pos[1];
        objBounds[0].z = OBJ_6298(self)->pos[2];
        objBounds[0].w = 1.0f;
        objBounds[1].x = OBJ_6298(self)->scl[0];
        objBounds[1].y = OBJ_6298(self)->scl[1];
        objBounds[1].z = OBJ_6298(self)->scl[2];
        objBounds[1].w = 1.0f;
        res = ((MnCullFn_6298)vt[93].fn)(thisp, &objBounds[0], &objBounds[1], ((MnViewFn_6298)((sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx))[43].fn)((char*)gfx + ((sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx))[43].delta));
    }
    if (res == 1) {
        return;
    }
    flags |= 0x400;
    if (res) {
        flags |= 0x20;
    }
    if (!mat) {
        if (cObjectInterface_isSphereOccluded(OBJ_6298(self)->box)) {
            return;
        }
    }
    void* rot = 0;
    void* tex;
    void* xf;
    if (((cMnNode_6298*)self)->hasAnimMats()) {
        tex = ((cMnNode_6298*)self)->getAnimMats();
        xf = t4mat_Identity;
    } else {
        tex = 0;
        xf = ((cMnNode_6298*)self)->getRootTransform();
    }
    t4Matrix rm;
    if (HasRot_6298(MOD_6298(self))) {
        rot = RotOf_6298(((cMnNode_6298*)self), &rm);
        flags |= 2;
    }
    void* light = HasLight_6298(MOD_6298(self)) ? LightOf_6298(((cMnNode_6298*)self)) : OBJ_6298(self)->light;
    sMnGfxVt_6298* vt = (sMnGfxVt_6298*)VPTR_OF(cGraphicsMan, gfx);
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
extern "C" void cMoveNode_renderModel(cMoveNode* self, t4Matrix* tex, int flags)
{
    sMnCam_6298* cam = (sMnCam_6298*)cWorld_m_pTheWorld->m_pWorldCache;
    sMnObj_6298* obj = OBJ_6298(self);
    if (!Visible_6298(cam, obj)) {
        return;
    }
    void* rot = 0;
    cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
    t4Matrix rm;
    if (HasRot_6298(MOD_6298(self))) {
        rot = RotOf_6298(((cMnNode_6298*)self), &rm);
        flags |= 2;
    }
    void* light = HasLight_6298(MOD_6298(self)) ? LightOf_6298(((cMnNode_6298*)self)) : OBJ_6298(self)->light;
    gfx->RenderModel((const tInstanceRecord*)OBJ_6298(self), (const tMaterialBlockRecord*)light, (const t4Matrix*)t4mat_Identity, self->Fade, tex, (const t4Matrix*)rot, flags);
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
        tBoundBox box;
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
#include "replay/bxreplayframe.h"
struct tModifierBlock;
void tModifierBlock_setBoundBox_K(tModifierBlock* mb, void* box) __asm__("tModifierBlock_setBoundBox__FP14tModifierBlock");


extern "C" tBoundBox* tModifierBlock_getBoundBox_67E0(void* mb) __asm__("tModifierBlock_getBoundBox");
extern "C" void* cObjectInterface_getNash(void);
extern "C" void nash_Nash_movenode_67E0(void* mgr, int a, void* obj, tBoundBox* box, tBoundBox* old) __asm__("nash_Nash_movenode");

/**
 * Puts the node back in the overlap system with the object's own bound box, moving it from the old box.
 *
 * @param vself  the node
 */
// Sets the modifier block's bound box from the node's own extents (self->18 + 0x60) and re-files it.
extern "C" void cMoveNode_restoreOverlapSystem(cMoveNode* vself)
{
    char** modBlock = (char**)vself->ModifierBlock;
    char* blk = modBlock ? *modBlock : 0;
    // Binding a reference to the object matches the target register allocation (see cMoveNode_getRootTransform).
    char*& transformModifier = blk;
    if (transformModifier == 0)
        return;
    tBoundBox box;
    tBoundBox old = *tModifierBlock_getBoundBox_67E0(vself->ModifierBlock);
    tInstanceRecord* n = vself->Instance;
    t4Vector corner;
    corner.x = n->m_fvMin.m[0];
    corner.y = n->m_fvMin.m[1];
    corner.z = n->m_fvMin.m[2];
    corner.w = 1.0f;
    box.MinPoint = corner;
    corner.x = n->m_fvMax.m[0];
    corner.y = n->m_fvMax.m[1];
    corner.z = n->m_fvMax.m[2];
    box.MaxPoint = corner;
    tModifierBlock_setBoundBox_K(vself->ModifierBlock, &box);
    void* obj = vself->Instance;
    nash_Nash_movenode_67E0(cObjectInterface_getNash(), 0, obj, &box, &old);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_updateOverlapSystem);
#ifdef SKIP_ASM


struct sMnVEntM_68B0 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};
struct sMnVEnt_68B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" tBoundBox* tModifierBlock_getBoundBox_68B0(void* mb) __asm__("tModifierBlock_getBoundBox");
extern "C" float tModifierBlock_getRadius(void* mb);
extern "C" void* cObjectInterface_getNash(void);
extern "C" void nash_Nash_movenode(void* mgr, int a, void* obj, tBoundBox* box, tBoundBox* old);

// PORT: PS2-only VU0 inline asm (a - b).
static inline void vu0Sub_68B0(t4Vector& out, const t4Vector& a, const t4Vector& b)
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
    out = r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline void vu0Add_68B0(t4Vector& out, const t4Vector& a, const t4Vector& b)
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
    out = r;
}

// World position: translation row of the node's transform (virtual slot 24).
static inline t4Vector pos_68B0(char* vself)
{
    cMoveNode* self = (cMoveNode*)vself;
    sMnVEntM_68B0* vt = (sMnVEntM_68B0*)VPTR_OF(cObjNode, self);
    return vt[24].fn(vthis(self, vt[24]))[3];
}

static inline void VCall24_68B0(char*& o)
{
    sMnVEnt_68B0* vt = *(sMnVEnt_68B0**)o;
    vt[24].fn(vthis(o, vt[24]));
}

/**
 * Refits the node's overlap box around its position with the root transform's radius and re-files it in the
 * overlap system.
 */
// Refits the modifier block's bound box around the node's position (radius from the block) and re-files it.
extern "C" void cMoveNode_updateOverlapSystem(cMoveNode* self)
{
    char** modBlock = (char**)self->ModifierBlock;
    char* transformModifier = modBlock ? *modBlock : 0;
    if (transformModifier == 0)
        return;
    tBoundBox box;
    tBoundBox old = *tModifierBlock_getBoundBox_68B0(self->ModifierBlock);
    float radius = tModifierBlock_getRadius(self->ModifierBlock);
    t4Vector rv;
    rv.w = 0.0f;
    rv.x = radius;
    rv.y = radius;
    rv.z = radius;
    t4Vector corner;
    vu0Sub_68B0(corner, pos_68B0(((char*)self)), rv);
    box.MinPoint = corner;
    vu0Add_68B0(corner, pos_68B0(((char*)self)), rv);
    box.MaxPoint = corner;
    tModifierBlock_setBoundBox_K(self->ModifierBlock, &box);
    void* obj = self->Instance;
    nash_Nash_movenode(cObjectInterface_getNash(), 0, obj, &box, &old);
    VCall24_68B0(transformModifier);
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
/**
 * Tells whether the node can be intersected, asking its transform modifier.
 *
 * @return 0 without a modifier block or transform modifier, otherwise the modifier's hasIntersect() (vtable
 *         slot 20)
 */
extern "C" int cMoveNode_hasIntersect(cMoveNode* self)
{
    tModifierBlock* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    cTransformModifier* node = block->RootTransform;
    if (node == 0) {
        return 0;
    }
    return node->hasIntersect();
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
    tModifierBlock* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    cTransformModifier* node = block->RootTransform;
    if (node == 0) {
        return 0;
    }
    sMoveNodeVEntryA70* vt = *(sMoveNodeVEntryA70**)node;
    return vt[21].fn(vthis(node, vt[21]), w, intInfo, num, flags);
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

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);
extern "C" void tModifierBlock_writeToReplayFrame(void* block, void* stream);

// PORT: the unit declares cMoveNode_writeToReplayFrame as `void* (void*)` for its callers; the body writes
// to the stream in a1, so the 2-arg body is bound by asm label.
void cMoveNode_writeToReplayFrame_2(void* self, cBxReplayFrame* stream) __asm__("cMoveNode_writeToReplayFrame");

/**
 * Writes a moving node to a replay frame: the instance-node part, debounce, fade, transfer flag and its
 * modifiers.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
void cMoveNode_writeToReplayFrame_2(void* vself, cBxReplayFrame* stream)
{
    cMoveNode* self = (cMoveNode*)vself;
    cInstanceNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->Debounce, 4);
    stream->write((const char*)&self->Fade, 4);
    stream->write((const char*)&self->TransferHack, 4);
    int flag = self->ModifierBlock != 0;
    stream->write((const char*)&flag, 4);
    tModifierBlock* block = self->ModifierBlock;
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
    return vt[i].fn(vthis(o, vt[i]));
}
/**
 * Tells whether the node keeps a focus count, which only a type-2 root transform modifier does.
 *
 * @return 1 when it does, otherwise 0
 */
// Does the modifier object's virtual slot 26 report state 2?
extern "C" int cMoveNode_hasFocusCount(cMoveNode* self)
{
    char** modBlock = (char**)self->ModifierBlock;
    if (modBlock != 0) {
        char* transformModifier = *modBlock;
        if (transformModifier != 0 && VCall_6BF0(transformModifier, 26) == 2)
            return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMoveNode_incFocusCount);
#ifdef SKIP_ASM

// PORT: cMultiSplineModifier_incFocusCount takes one argument, but this caller passes a second ($5 = a1);
// the original declaration it was compiled against must have had two parameters.
int cMultiSplineModifier_incFocusCount_2(void* self, void* a1) __asm__("cMultiSplineModifier_incFocusCount__FPv");

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
            sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)node;
            if (vt[26].fn(vthis(node, vt[26])) == 2) {
                return cMultiSplineModifier_incFocusCount_2(*(void**)self->ModifierBlock, pInst);
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

// PORT: cMultiSplineModifier_decFocusCount takes one argument, but this caller passes a second ($5 = a1);
// the original declaration it was compiled against must have had two parameters.
int cMultiSplineModifier_decFocusCount_2(void* self, void* a1) __asm__("cMultiSplineModifier_decFocusCount__FPv");

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
            if (vt[26].fn(vthis(node, vt[26])) == 2) {
                return cMultiSplineModifier_decFocusCount_2(*(void**)self->ModifierBlock, pInst);
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
    tModifierBlock* block = self->ModifierBlock;
    if (block == 0) {
        return 0;
    }
    cTransformModifier* node = block->RootTransform;
    if (node == 0) {
        return 0;
    }
    if (node->getTransformType() != 2) {
        return 0;
    }
    return *(int*)((char*)*(void**)self->ModifierBlock + 0x34);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cObjectNode_cObjectNode);
#ifdef SKIP_ASM
struct sMoveFlags_6DB0 {
    char pad_0x00[0x8];
    unsigned int w;     // 0x8
};

extern void* cObjectNode_vtbl[];
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a3);

/**
 * Constructs an object node (node type 17) for a placed instance.
 *
 * Runs the cMoveNode record constructor cMoveNode_cMoveNode1 and installs cObjectNode_vtbl. When the instance is forced visible or
 * the record flags have both bits 0x1 and 0x2, the flags get 0x4 and lose 0x2; for a solid object they get 0x20 and
 * lose 0x40.
 *
 * @param bucket  update bucket
 * @param instRecord  instance record
 * @param isSolid  solid flag
 * @param forceVisible  force the instance visible
 * @return self
 */
extern "C" void* cObjectNode_cObjectNode(cObjectNode* self, void* bucket, void* instRecord, int isSolid, int forceVisible)
{
    cMoveNode_cMoveNode1(self, bucket, 17, instRecord);
    VPTR_OF(cObjNode, self) = cObjectNode_vtbl;
    if (forceVisible != 0 || (((sMoveFlags_6DB0*)self->Instance)->w & 3) == 3) {
        sMoveFlags_6DB0* recordFlags = (sMoveFlags_6DB0*)self->Instance;
        recordFlags->w = (recordFlags->w & ~2) | 4;
    }
    if (isSolid != 0) {
        sMoveFlags_6DB0* recordFlags = (sMoveFlags_6DB0*)self->Instance;
        recordFlags->w = (recordFlags->w & ~0x40) | 0x20;
    }
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cObjectNode_cObjectNode1);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode(void* self);
extern void* cObjectNode_vtbl[];

/**
 * Constructs an object node from a replay frame: cMoveNode_cMoveNode, then installs cObjectNode_vtbl.
 *
 * @return self
 */
extern "C" void* cObjectNode_cObjectNode1(cObjectNode* self)
{
    cMoveNode_cMoveNode(self);
    VPTR_OF(cObjNode, self) = cObjectNode_vtbl;
    return ((void*)self);
}
#endif

extern "C" void* cMoveNode_writeToReplayFrame(void* self);

//99.29%
INCLUDE_ASM("object/movenode", cObjectNode_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes an object node to a replay frame by forwarding to cMoveNode_writeToReplayFrame.
 *
 * @return cMoveNode_writeToReplayFrame's result (callers ignore it)
 */
void* cObjectNode_writeToReplayFrame(void* vself)
{
    cObjectNode* self = (cObjectNode*)vself;
    return cMoveNode_writeToReplayFrame(self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_cPositionModifier);
#ifdef SKIP_ASM
struct sMoveQuad {
    float v[4];
} __attribute__((aligned(16)));

extern void* cPositionModifier_vtbl[];
extern sMoveQuad t4mat_Identity[];

/**
 * Constructs a position modifier for a fixed offset.
 *
 * Installs cPositionModifier_vtbl, stores pos in Offset, copies t4mat_Identity into Transform, sets RootTransformDirty
 * and puts Offset in Transform's translation row.
 *
 * @param pos  offset
 * @return self
 */
extern "C" void* cPositionModifier_cPositionModifier(cPositionModifier* self, sMoveQuad* pos)
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
        : "r"((char*)&self->Transform), "r"(t4mat_Identity)
        : "memory");
    self->RootTransformDirty = 1;
    *(sMoveQuad*)&self->Transform.row[3] = *(sMoveQuad*)&self->Offset;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_cPositionModifier1);
#ifdef SKIP_ASM
struct sMoveQuad2 {
    float v[4];
} __attribute__((aligned(16)));

extern void* cPositionModifier_vtbl[];

/**
 * Constructs a position modifier from a matrix.
 *
 * Installs cPositionModifier_vtbl, takes Offset from the matrix's translation row (src +0x30), copies the matrix into
 * Transform and sets RootTransformDirty.
 *
 * @param src  matrix (t4Matrix)
 * @return self
 */
extern "C" void* cPositionModifier_cPositionModifier1(cPositionModifier* self, t4Matrix* src)
{
    *(void***)self = cPositionModifier_vtbl;
    sMoveQuad2 translation = *(sMoveQuad2*)&src->row[3];
    *(sMoveQuad2*)&self->Offset = translation;
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
        : "r"((char*)&self->Transform), "r"(((void*)src))
        : "memory");
    self->RootTransformDirty = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_cPositionModifier2);
#ifdef SKIP_ASM

// PORT: callers declare this void; the body returns self, so bind it via an asm label.
extern "C" void* cPositionModifier_cPositionModifier2_ret(void* self, cBxReplayFrame* stream) __asm__("cPositionModifier_cPositionModifier2");
/**
 * Constructs a position modifier from a replay frame.
 *
 * Installs cPositionModifier_vtbl, reads the 0x40-byte state at +0x10 (BoundBox, Offset, Radius, RootTransformDirty)
 * through the stream's read slot (vtable entry 2), copies t4mat_Identity into Transform and sets RootTransformDirty.
 *
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cPositionModifier_cPositionModifier2_ret(void* vself, cBxReplayFrame* stream)
{
    cPositionModifier* self = (cPositionModifier*)vself;
    *(void***)self = cPositionModifier_vtbl;
    stream->read((char*)&self->BoundBox, 0x40);
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
        : "r"((char*)&self->Transform), "r"(t4mat_Identity)
        : "memory");
    self->RootTransformDirty = 1;
    return ((void*)self);
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
    cQuad128 offset = *(cQuad128*)&self->Offset;
    self->RootTransformDirty = 0;
    *(cQuad128*)&self->Transform.row[3] = offset;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cPositionModifier_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves a position modifier to a replay frame: the 0x40 bytes from +0x10 (BoundBox, Offset, Radius,
 * RootTransformDirty) through the frame's write (slot 1).
 *
 * @param obj  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cPositionModifier_writeToReplayFrame(cPositionModifier* self, cBxReplayFrame* obj)
{
    obj->write((const char*)&self->BoundBox, 0x40);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cParentModifier_cParentModifier1);
#ifdef SKIP_ASM
extern char cParentModifier_vtbl[];

/**
 * Constructs a parent modifier: an offset that follows a mesh of another instance.
 *
 * Builds the position-modifier base from pos (cPositionModifier_cPositionModifier), installs cParentModifier_vtbl and stores ParentInst and
 * Mesh.
 *
 * @param parentInstance  instance record to follow
 * @param meshIndex  mesh of that instance
 * @param pos  offset from it
 * @return self
 */
extern "C" void* cParentModifier_cParentModifier1(cParentModifier* self, void* parentInstance, void* meshIndex, sMoveQuad* pos)
{
    cPositionModifier_cPositionModifier((cPositionModifier*)self, pos);
    VPTR_AT(self, 0) = (void**)cParentModifier_vtbl;
    self->ParentInst = (tInstanceRecord*)parentInstance;
    self->Mesh = (int)meshIndex;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParentModifier_cParentModifier);
#ifdef SKIP_ASM


extern "C" void cPositionModifier_cPositionModifier2(void* self, void* stream);
extern char cParentModifier_vtbl[];

/**
 * Constructs a parent modifier from a replay frame: the position-modifier part, then Mesh and ParentInst.
 *
 * Runs cPositionModifier_cPositionModifier2 (On Tour cPositionModifier(cBxReplayFrame*)), stores cParentModifier_vtbl, reads 4 bytes into
 * Mesh (+0x90, frame slot 2) and ParentInst (slot 3).
 *
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cParentModifier_cParentModifier(cParentModifier* self, cBxReplayFrame* stream)
{
    cPositionModifier_cPositionModifier2(self, stream);
    VPTR_AT(self, 0) = (void**)cParentModifier_vtbl;
    stream->read((char*)&self->Mesh, 4);
    self->ParentInst = (tInstanceRecord*)(void*)stream->readReplayInstance();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cParentModifier_updateRootTransform);
#ifdef SKIP_ASM


extern "C" void cInstanceNode_buildMeshMat(void* model, int bone, t4Matrix* out);

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
static inline t4Vector vu0MtxApply_357108(t4Matrix* m, t4Vector* v)
{
    t4Vector out;
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
static inline t4Vector vu0Add_357108(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector vel_357108(char* vself)
{
    cParentModifier* self = (cParentModifier*)vself;
    return self->Transform.row[3];
}


/**
 * Follows the parent instance: copies the parent mesh's matrix and places the offset in its space.
 *
 * With the parent's node set (+0xC), Transform is the node's mesh matrix for Mesh (slot 29) when its slot 26 is
 * nonzero, else cInstanceNode_buildMeshMat (On Tour cInstanceNode::buildMeshMat) builds it; row 3 becomes that position plus the
 * matrix applied to Offset (the local view's vel_357108 is Transform row 3, +0x80). Clears RootTransformDirty in
 * any case.
 */
extern "C" void cParentModifier_updateRootTransform(cParentModifier* self)
{
    cInstanceNode* parentNode = (cInstanceNode*)self->ParentInst->m_pActiveNode;
    if (((char*)parentNode) != 0) {
        if (parentNode->hasAnimMats() != 0) {
            vu0CopyMtx_357108(&self->Transform, (t4Matrix*)parentNode->getAnimMat(self->Mesh));
        } else {
            cInstanceNode_buildMeshMat(self->ParentInst, self->Mesh, ((&self->Transform)));
        }
        *(&self->Transform.row[3]) = vu0Add_357108(vel_357108(((char*)self)), vu0MtxApply_357108(((&self->Transform)), ((&self->Offset))));
    }
    self->RootTransformDirty = 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cParentModifier_updateIntersectInfo);
#ifdef SKIP_ASM

/**
 * Lets the parent instance's node fill an intersection record as if it hit Mesh.
 *
 * Temporarily stores Mesh in the record's mesh index (+0x5C), calls the parent node's slot 42 with it, then
 * restores the index. Does nothing without a parent or its node.
 *
 * @param obj  the intersection record (On Tour tIntersectInfo& intInfo)
 */
extern "C" void cParentModifier_updateIntersectInfo(cParentModifier* self, tIntersectInfo* obj)
{
    tInstanceRecord* parentInst = self->ParentInst;
    if (((void*)parentInst) == 0) {
        return;
    }
    cInstanceNode* parentNode = parentInst->m_pActiveNode;
    if (((void*)parentNode) == 0) {
        return;
    }
    int saved = obj->Mesh;
    obj->Mesh = self->Mesh;
    sVEntry<void(void*, void*)>* vt = (sVEntry<void (void*, void*)>*)VPTR_OF(cObjNode, parentNode);
    vt[42].fn(vthis(parentNode, vt[42]), obj);
    obj->Mesh = saved;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParentModifier_writeToReplayFrame);
#ifdef SKIP_ASM



/**
 * Saves a parent modifier to a replay frame: the position-modifier state, Mesh (+0x90) and ParentInst
 * (writeReplayInstance, slot 5).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cParentModifier_writeToReplayFrame(cParentModifier* self, cBxReplayFrame* stream)
{
    cPositionModifier_writeToReplayFrame((cPositionModifier*)self, stream);
    stream->write((const char*)&self->Mesh, 4);
    stream->writeReplayInstance((const tInstanceRecord*)self->ParentInst);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_cMagnetModifier);
#ifdef SKIP_ASM
extern char cMagnetModifier_vtbl[];

/**
 * Constructs a magnet modifier that pulls riders toward a point.
 *
 * Installs cMagnetModifier_vtbl, stores Instance and Radius, Speed = speed * 27.78 (100 / 3.6), Being -1, Time 0,
 * AllowCollide 0, copies mat into Transform, sets RootTransformDirty and takes Offset from the translation row.
 *
 * @param inst  instance record
 * @param mat  magnet transform
 * @param radius  pull radius
 * @param speed  pull speed (scaled by 100/3.6)
 * @return self
 */
extern "C" void* cMagnetModifier_cMagnetModifier(cMagnetModifier* self, void* inst, void* mat, float radius, float speed)
{
    *(void**)self = cMagnetModifier_vtbl;
    self->Instance = (tInstanceRecord*)inst;
    self->Radius = radius;
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
        : "r"((char*)&self->Transform), "r"(mat)
        : "memory");
    self->RootTransformDirty = 1;
    sMoveQuad translation = *(sMoveQuad*)&self->Transform.row[3];
    *(sMoveQuad*)&self->Offset = translation;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_cMagnetModifier1);
#ifdef SKIP_ASM


extern char cMagnetModifier_vtbl[];

/**
 * Constructs a magnet modifier from a replay frame.
 *
 * Installs cMagnetModifier_vtbl, reads Instance through the stream's instance slot (vtable entry 3), then 80 bytes at
 * +0x10 and 0x40 bytes at +0x60, and sets RootTransformDirty.
 *
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cMagnetModifier_cMagnetModifier1(cMagnetModifier* self, cBxReplayFrame* stream)
{
    char* payload = (char*)&self->Offset;
    *(void**)self = cMagnetModifier_vtbl;
    self->Instance = (tInstanceRecord*)(void*)stream->readReplayInstance();
    stream->read(payload, 80);
    stream->read((char*)&self->Transform, 0x40);
    self->RootTransformDirty = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_update);
#ifdef SKIP_ASM
extern "C" float cObjectInterface_getUpdateTime();
extern "C" void* cObjectInterface_getPosition(int id);


// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub_3573F8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_3573F8(const t4Vector& v)
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
static inline t4Vector vu0Scale_3573F8(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += v).
static inline void vu0AddTo_3573F8(t4Vector& dst, const t4Vector& v)
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
    t4Vector pos;
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
 * With a rider attached (Being >= 0) Time grows by the frame time; d = the rider's position (cObjectInterface_getPosition) -
 * Offset. Within 50 units AllowCollide is set; otherwise Offset moves by d * Speed * Time / |d|, clamped to d
 * (which also sets AllowCollide). Marks the root transform dirty.
 */
extern "C" void cMagnetModifier_update(cMagnetModifier* self)
{
    if (self->Being < 0) {
        return;
    }
    self->Time += cObjectInterface_getUpdateTime();
    t4Vector delta = vu0Sub_3573F8(*(t4Vector*)cObjectInterface_getPosition(self->Being), *(&self->Offset));
    float len = vu0Length_3573F8(delta);
    if (len < 50.0f) {
        self->AllowCollide = 1;
    } else {
        t4Vector step = vu0Scale_3573F8(delta, self->Speed / len * self->Time);
        if (len < vu0Length_3573F8(step)) {
            self->AllowCollide = 1;
            step = delta;
        }
        vu0AddTo_3573F8(*(&self->Offset), step);
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
    cQuad128 offset = *(cQuad128*)&self->Offset;
    self->RootTransformDirty = 0;
    *(cQuad128*)&self->Transform.row[3] = offset;
}
#endif

INCLUDE_ASM("object/movenode", cMagnetModifier_intersect);

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_runCollide);
#ifdef SKIP_ASM

extern "C" cBeing* cObjectInterface_getBeing(int id);

/**
 * Decides whether the magnet object collides with a rider, catching the first human rider that touches it.
 *
 * With a rider attached only that rider may collide, once AllowCollide is set. Otherwise the rider
 * (cObjectInterface_getBeing(id), which returns the being, not a count) is attached when its slot 8 (On Tour
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
    cBeing* obj = cObjectInterface_getBeing(id);
    if (obj->isHuman() != 0) {
        self->Being = id;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMagnetModifier_writeToReplayFrame);
#ifdef SKIP_ASM


/**
 * Saves a magnet modifier to a replay frame: Instance (writeReplayInstance, slot 5), the 0x50 bytes from +0x10
 * (Offset through AllowCollide) and Transform (0x40 bytes from +0x60).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cMagnetModifier_writeToReplayFrame(cMagnetModifier* self, cBxReplayFrame* stream)
{
    stream->writeReplayInstance((const tInstanceRecord*)self->Instance);
    stream->write((const char*)&self->Offset, 80);
    stream->write((const char*)&self->Transform, 0x40);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_cAvaSplineModifier);
#ifdef SKIP_ASM
extern void* cAvaSplineModifier_vtbl[];

/**
 * Constructs an avalanche spline modifier for an instance.
 *
 * Installs cAvaSplineModifier_vtbl, stores Instance, zeroes Radius and copies the instance's matrix into the transform.
 *
 * @param src  instance record
 * @return self
 */
// PORT: PS2-only VU0 inline asm (4x4 matrix copy, src+0x10 -> self+0x40); the PC
// port needs a plain 64-byte copy.
extern "C" void* cAvaSplineModifier_cAvaSplineModifier(cAvaSplineModifier* self, tInstanceRecord* src)
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
        : "r"((char*)&self->Transform), "r"((char*)&src->m_fmTransform)
        : "memory");
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_cAvaSplineModifier1);
#ifdef SKIP_ASM


// PORT: the project names this cObjectInterface_getAvaSplineTransform__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* cObjectInterface_getAvaSplineTransform_2(void*, void*) __asm__("cObjectInterface_getAvaSplineTransform__FPv");

/**
 * Constructs an avalanche spline modifier from a replay frame.
 *
 * Installs cAvaSplineModifier_vtbl, reads Instance (stream vtable entry 3) and 0x30 bytes at +0x10, then refetches the
 * instance transform into +0x40 (cObjectInterface_getAvaSplineTransform).
 *
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cAvaSplineModifier_cAvaSplineModifier1(cAvaSplineModifier* self, cBxReplayFrame* stream)
{
    char* payload = (char*)&self->BoundBox;
    *(void***)self = cAvaSplineModifier_vtbl;
    self->Instance = (tInstanceRecord*)(void*)stream->readReplayInstance();
    stream->read(payload, 0x30);
    cObjectInterface_getAvaSplineTransform_2(self->Instance, (char*)&self->Transform);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_update);
#ifdef SKIP_ASM
// PORT: the project names this cObjectInterface_getAvaSplineTransform__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* cObjectInterface_getAvaSplineTransform_2(void*, void*) __asm__("cObjectInterface_getAvaSplineTransform__FPv");

/**
 * Updates the avalanche spline modifier: fills Transform (+0x40) from its instance (cObjectInterface_getAvaSplineTransform, called with two
 * arguments though declared with one).
 */
extern "C" void cAvaSplineModifier_update(cAvaSplineModifier* self)
{
    cObjectInterface_getAvaSplineTransform_2(self->Instance, (char*)&self->Transform);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cAvaSplineModifier_writeToReplayFrame);
#ifdef SKIP_ASM


/**
 * Saves an avalanche-spline modifier to a replay frame: Instance (writeReplayInstance, slot 5), then the 0x30 bytes
 * from +0x10 (BoundBox, Radius).
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cAvaSplineModifier_writeToReplayFrame(cAvaSplineModifier* self, cBxReplayFrame* stream)
{
    stream->writeReplayInstance((const tInstanceRecord*)self->Instance);
    stream->write((const char*)&self->BoundBox, 0x30);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParticleNode_cParticleNode1);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a3);
extern "C" void cMoveNode_transferFromOld(cMoveNode* self, cInstanceNode* a1);
extern void* cParticleNode_vtbl[];

/**
 * Constructs a particle node (node type 13) for an instance that takes over from its old node.
 *
 * Runs the cMoveNode record constructor cMoveNode_cMoveNode1, installs cParticleNode_vtbl, takes the old node's modifiers
 * (cMoveNode_transferFromOld) and adds the particle effect (cMoveNode_addParticle).
 *
 * @param bucket  update bucket
 * @param instRecord  instance record
 * @param particle  particle make-data
 * @param oldNode  node the instance had before
 * @return self
 */
extern "C" void* cParticleNode_cParticleNode1(cParticleNode* self, void* bucket, void* instRecord, void* particle, void* oldNode)
{
    cMoveNode_cMoveNode1(((cMoveNode*)self), bucket, 13, instRecord);
    VPTR_OF(cObjNode, self) = cParticleNode_vtbl;
    cMoveNode_transferFromOld(((cMoveNode*)self), (cInstanceNode*)oldNode);
    cMoveNode_addParticle(((cMoveNode*)self), particle);
    return ((cMoveNode*)self);
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
extern "C" void* cParticleNode_cParticleNode(cParticleNode* self)
{
    cMoveNode_cMoveNode(self);
    VPTR_OF(cObjNode, self) = cParticleNode_vtbl;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cParticleNode_update);
#ifdef SKIP_ASM

extern "C" void cMoveNode_update(cMoveNode* self);

/**
 * Updates a particle node and deletes it once nothing keeps it alive.
 *
 * After cMoveNode_update(), returns when the modifier block's word at +0x10 is set (probably the head of its
 * effect link list, so an effect is still attached); otherwise calls the virtual destructor with flags 3.
 */
extern "C" void cParticleNode_update(cParticleNode* self)
{
    cMoveNode_update((cMoveNode*)self);
    tModifierBlock* mb = self->ModifierBlock;
    if (mb != 0 && *(int*)&mb->Effect != 0) {
        return;
    }
    delete self;
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
INCLUDE_ASM("object/movenode", cMultiParticleMan_cMultiParticleMan);
#ifdef SKIP_ASM
extern void* cMultiParticleMan_TheMultiParticleMan;

/**
 * Constructs the multi-particle manager and makes it the global instance.
 *
 * Stores this in cMultiParticleMan_TheMultiParticleMan and clears its four words.
 *
 * @return this
 */
extern "C" void* cMultiParticleMan_cMultiParticleMan(cMultiParticleMan* self)
{
    int i;
    cMultiParticleMan_TheMultiParticleMan = ((void*)self);
    for (i = 0; i < 4; i++) {
        ((void**)self)[i] = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan__cMultiParticleMan);
#ifdef SKIP_ASM
extern void* cMultiParticleMan_TheMultiParticleMan;
extern "C" void cMultiParticle__cMultiParticle(cMultiParticle* p, int flags);
void operator_delete(int* ptr);

/**
 * Destroys the multi-particle manager: deletes each of its up to 4 cMultiParticle (destructor flags 3) and clears the
 * global cMultiParticleMan_TheMultiParticleMan that points at the manager.
 *
 * @param flags  destructor flags; bit 0 frees the manager itself with operator_delete
 */
extern "C" void cMultiParticleMan__cMultiParticleMan(cMultiParticleMan* self, int flags)
{
    int i;
    for (i = 0; i < 4; i++) {
        void* particle = ((void**)self)[i];
        if (particle != 0) {
            cMultiParticle__cMultiParticle((cMultiParticle*)particle, 3);
        }
    }
    cMultiParticleMan_TheMultiParticleMan = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_addParticle);
#ifdef SKIP_ASM
extern char str_MultiParticle[];
extern "C" void* cMultiParticle_cMultiParticle(cMultiParticle* mem, void* a1, void* a2);

/**
 * Creates the cMultiParticle for slot @p i if the slot is empty (On Tour addParticle(int id, tParticleData* pdata,
 * int numInst)).
 *
 * Allocates 0x50 bytes (tag str_MultiParticle, "MultiParticle") and runs the cMultiParticle constructor (cMultiParticle_cMultiParticle) with
 * @p pdata and @p numInst. An occupied slot is left alone; @p i is not range-checked against the 4 slots.
 *
 * @param i  the slot index (On Tour id)
 * @param pdata  the tParticleData the particles use
 * @param numInst  how many instances the cMultiParticle can track (On Tour numInst)
 */
extern "C" void cMultiParticleMan_addParticle(cMultiParticleMan* self, int i, void* pdata, void* numInst)
{
    void** slot = (void**)&self->Particles[i];
    if (*slot == 0) {
        *slot = cMultiParticle_cMultiParticle((cMultiParticle*)cMemMan_alloc(80, str_MultiParticle, 0x20000000, 0), pdata, numInst);
    }
}
#endif

extern "C" void* cMultiParticle_addInst(int, int);

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_addInst__FPvii);
#ifdef SKIP_ASM
/**
 * Adds an instance to one of the multi-particle manager's four systems (cMultiParticle_addInst on Particles[particleIndex]).
 *
 * @param particleIndex  system index (0-3)
 * @param instId  instance id to add
 * @return whatever cMultiParticle_addInst leaves (callers ignore it)
 */
void* cMultiParticleMan_addInst(void* vself, int particleIndex, int instId)
{
    cMultiParticleMan* self = (cMultiParticleMan*)vself;
    return cMultiParticle_addInst((int)self->Particles[particleIndex], instId);
}
#endif

extern "C" void* cMultiParticle_remInst(int, int);

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_remInst__FPvii);
#ifdef SKIP_ASM
/**
 * Removes an instance from one of the multi-particle manager's four systems (cMultiParticle_remInst on
 * Particles[particleIndex]).
 *
 * @param particleIndex  system index (0-3)
 * @param instId  instance id to remove
 * @return whatever cMultiParticle_remInst leaves (callers ignore it)
 */
void* cMultiParticleMan_remInst(void* vself, int particleIndex, int instId)
{
    cMultiParticleMan* self = (cMultiParticleMan*)vself;
    return cMultiParticle_remInst((int)self->Particles[particleIndex], instId);
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_reset);
#ifdef SKIP_ASM
extern "C" void cMultiParticle__cMultiParticle(cMultiParticle* p, int flags);

/**
 * Deletes all of the multi-particle manager's particle systems.
 *
 * For each of the 4 slots (Particles, +0x0..+0xC) a non-null system is destroyed and freed with
 * cMultiParticle__cMultiParticle(p, 3), and the slot is cleared. Called on a partial restart (cGame_partialRestart)
 * and before a replay frame is restored.
 */
extern "C" void cMultiParticleMan_reset(void** self)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            cMultiParticle__cMultiParticle((cMultiParticle*)self[i], 3);
        }
        self[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_unloadSection);
#ifdef SKIP_ASM
extern "C" void cMultiParticle_unloadSection(void* p, void* arg);

/**
 * Drops a world section's instances from all four multi-particle systems.
 *
 * Calls cMultiParticle_unloadSection on each system in Particles that exists. cGame_sectionUnloading calls it.
 *
 * @param sectionId  world section being unloaded
 */
extern "C" void cMultiParticleMan_unloadSection(void** self, void* sectionId)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (self[i] != 0) {
            cMultiParticle_unloadSection(self[i], sectionId);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticleMan_update);
#ifdef SKIP_ASM
extern "C" void cMultiParticle_update(void* p);

/**
 * Updates every multi-particle system the manager holds.
 *
 * Calls cMultiParticle_update() on each non-null slot of Particles (+0x0..+0xC, 4 slots). Run every game frame by
 * cGame_update.
 */
extern "C" void cMultiParticleMan_update(void** self)
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
extern "C" void cMultiParticleMan_writeToReplayFrame(void** self, cBxReplayFrame* stream)
{
    int i;
    for (i = 0; i < 4; i++) {
        int has = self[i] != 0;
        stream->write((const char*)&has, 4);
        if (has) {
            cMultiParticle_writeToReplayFrame(self[i], stream);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/movenode", cMultiParticleMan_readFromReplayFrame);
#ifdef SKIP_ASM

extern "C" void* cMultiParticle_cMultiParticle1(cMultiParticle* mem, cBxReplayFrame* stream);

/**
 * Restores the multi-particle systems from a replay frame.
 *
 * Deletes the current systems (cMultiParticleMan_reset), then for each of the 4 slots reads a 4-byte used flag through the
 * frame's virtual read (slot 2); a used slot gets a new cMultiParticle (0x50 bytes, "MultiParticle") built from the
 * frame by cMultiParticle_cMultiParticle1, an unused one is left null. The counterpart of cMultiParticleMan_writeToReplayFrame.
 *
 * @param stream  the replay frame to read from (On Tour data)
 */
extern "C" void cMultiParticleMan_readFromReplayFrame(void** self, cBxReplayFrame* stream)
{
    int i;
    cMultiParticleMan_reset(self);
    for (i = 0; i < 4; i++) {
        int has;
        stream->read((char*)&has, 4);
        if (has) {
            self[i] = cMultiParticle_cMultiParticle1((cMultiParticle*)cMemMan_alloc(80, str_MultiParticle, 0x20000000, 0), stream);
        } else {
            self[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticle_cMultiParticle);
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


extern char str_Particle_b[];
extern "C" void MEM_copy(void* dst, const void* src, int n);
extern "C" void cMultiParticle_setupMultiParticle(void* self);

/**
 * Constructs a multi-particle system for up to a given number of instances.
 *
 * Sets MaxInst to the given maximum and NumInst to 0, copies the 216-byte particle data into a new "Particle" block,
 * sets up the emitter and id list (cMultiParticle_setupMultiParticle), then copies BoundBox from the emitter and Offset
 * from the emitter position with w = 0.
 *
 * @param mem  storage for the system
 * @param particleData  particle data (tParticleData, 216 bytes)
 * @param maxInst  maximum instance count
 * @return mem
 */
extern "C" void* cMultiParticle_cMultiParticle(cMultiParticle* mem, void* particleData, void* maxInst)
{
    cMultiParticle* self = mem;
    *(void**)&self->MaxInst = maxInst;
    self->NumInst = 0;
    void* buf = cMemMan_alloc(216, str_Particle_b, 0x20000000, 0);
    *(void**)&self->Particle = buf;
    MEM_copy(buf, particleData, 216);
    cMultiParticle_setupMultiParticle(self);
    sPartDesc_7DD8* emitterDesc = ((sPartDesc_7DD8*)self->Emitter);
    *(sQuad_7DD8*)&self->BoundBox = emitterDesc->a;
    *(sQuad_7DD8*)&self->BoundBox.MaxPoint = emitterDesc->b;
    *(sQuad_7DD8*)&self->Offset = sQuad_7DD8(emitterDesc->pos[0], emitterDesc->pos[1], emitterDesc->pos[2], 0.0f);
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticle_cMultiParticle1);
#ifdef SKIP_ASM
extern "C" void cStaticEmitter_readFromReplayFrame(void* self, void* stream);

static inline void read_357E80(cBxReplayFrame* stream, void* dst, int n)
{
    stream->read((char*)dst, n);
}

/**
 * Constructs a multi-particle system from a replay frame.
 *
 * Reads MaxInst and NumInst, a new 216-byte "Particle" block, sets up the emitter (cMultiParticle_setupMultiParticle)
 * and lets it read its state (cStaticEmitter_readFromReplayFrame), then reads Offset, BoundBox and NumInst instance
 * ids.
 *
 * @param mem  storage for the system
 * @param stream  replay stream to read from
 * @return mem
 */
extern "C" void* cMultiParticle_cMultiParticle1(cMultiParticle* mem, cBxReplayFrame* stream)
{
    cMultiParticle* self = mem;
    sQuad_7DD8* ab = (sQuad_7DD8*)&self->BoundBox;
    read_357E80(stream, self, 4);
    read_357E80(stream, &self->NumInst, 4);
    void* buf = cMemMan_alloc(216, str_Particle_b, 0x20000000, 0);
    *(void**)&self->Particle = buf;
    read_357E80(stream, buf, 216);
    cMultiParticle_setupMultiParticle(self);
    cStaticEmitter_readFromReplayFrame(((sPartDesc_7DD8*)self->Emitter), stream);
    read_357E80(stream, (sQuad_7DD8*)&self->Offset, 0x10);
    read_357E80(stream, ab, 0x20);
    if (self->NumInst > 0) {
        read_357E80(stream, (void*)((int)self->InstIds), self->NumInst * 4);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("object/movenode", cMultiParticle__cMultiParticle);
#ifdef SKIP_ASM
struct sMnDtorVEntry7FA0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

void operator_delete(int* ptr);
void cMemMan_free(void*);

/**
 * Destroys a cMultiParticle: deletes Particle, frees the InstIds buffer, destroys the Emitter through its virtual
 * destructor (vtable at +0x18C, slot 1, flags 3) and frees the object when @p flags bit 0 is set.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void cMultiParticle__cMultiParticle(cMultiParticle* self, int flags)
{
    operator_delete((int*)self->Particle);
    void* buf = self->InstIds;
    if (buf != 0) {
        cMemMan_free(buf);
    }
    cStaticEmitter* obj = self->Emitter;
    if (obj != 0) {
        sMnDtorVEntry7FA0* vt = (sMnDtorVEntry7FA0*)obj->mVtbl;
        vt[1].fn(vthis(obj, vt[1]), 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

