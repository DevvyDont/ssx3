#include "vtable.h"
#include "common.h"
#include "object/railmodifier.h"
#include "object/effectmodifier.h"
#include "bx/math3d.h"
#include "object/movenode.h"
#include "world/worldrecords.h"
#include "world/instancerecord.h"
#include "replay/frameptr.h"
#include "object/skybox.h"
#include "object/flexbridgenode.h"
#include "object/physicsmodifier.h"
#include "object/effectlink.h"
#include "render/ps2graphicsman.h"
#include "object/modifierblock.h"
#include "replay/bxreplayframe.h"
#include "object/transformmodifier.h"
#include "render/graphicsman.h"


extern void* cEffectLink_vtbl[16];
extern void* cRailLink_vtbl[16];

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_tModifierBlock__FP14tModifierBlock);
#ifdef SKIP_ASM
/**
 * Constructs an empty modifier block.
 *
 * Clears RootTransform, UVScroll, TexFlip and Physics and the heads of the Effect and Rail lists, and stores the
 * cEffectLink and cRailLink vtables in those two heads.
 *
 * @return @p self
 */
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self)
{
    VPTR_OF(cEffectLink, &self->Effect) = cEffectLink_vtbl;
    VPTR_OF(cRailLink, &self->Rail) = cRailLink_vtbl;
    self->Effect.Next = 0;
    self->Effect.Prev = 0;
    self->Rail.Next = 0;
    self->Rail.Prev = 0;
    self->RootTransform = 0;
    self->UVScroll = 0;
    self->TexFlip = 0;
    self->Physics = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock__tModifierBlock);
#ifdef SKIP_ASM
extern "C" void tModifierBlock_remTransformModifier(tModifierBlock* self);
extern "C" void tModifierBlock_remUVScrollModifier(tModifierBlock* self);
extern "C" void tModifierBlock_remTexFlipModifier(tModifierBlock* self);
extern "C" void tModifierBlock_remPhysicsModifier(tModifierBlock* self);
extern "C" void tModifierBlock_remEffectModifier(tModifierBlock* self);
extern "C" void tModifierBlock_remRailModifier(tModifierBlock* self, unsigned int mask);
extern "C" void cRailLink_unlink(void* self);
extern "C" void cEffectLink_unlink(void* self);
void operator_delete(int* ptr);

/**
 * Destroys a modifier block and everything it owns.
 *
 * Deletes the node, frees the two buffers at offsets 4 and 8, deletes the object at offset 0xC, every effect
 * link and every rail link (tModifierBlock_remRailModifier with id ~0), then tears down the two list heads.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void tModifierBlock__tModifierBlock(tModifierBlock* self, int flags)
{
    self->remTransformModifier();
    self->remUVScrollModifier();
    self->remTexFlipModifier();
    self->remPhysicsModifier();
    self->remEffectModifier();
    self->remRailModifier(0xFFFFFFFF);
    VPTR_OF(cRailLink, &self->Rail) = cRailLink_vtbl;
    cRailLink_unlink(&self->Rail);
    VPTR_OF(cEffectLink, &self->Effect) = cEffectLink_vtbl;
    cEffectLink_unlink(&self->Effect);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_getBoundBox);
#ifdef SKIP_ASM
/**
 * Returns the bounding box of the modifier block's node.
 *
 * Calls slot 12 of the node at offset 0.
 *
 * @return the node's result, or 0 when the block has no node
 */
const tBoundBox* tModifierBlock::getBoundBox()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        return node->getBoundBox();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_setBoundBox__FP14tModifierBlock);
#ifdef SKIP_ASM
// PORT: the symbol's mangling says one parameter, but the body passes $5 through
// to the virtual call untouched, so the real function takes a second argument.

/**
 * Passes a bounding box to the modifier block's node.
 *
 * Calls slot 13 of the node with @p box; does nothing when the block has no node.
 *
 * @param box  the bounding box, passed through unchanged
 */
void tModifierBlock::setBoundBox(void* box)
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->setBoundBox(*(const tBoundBox*)box);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_getRadius);
#ifdef SKIP_ASM
/**
 * Returns the block's bounding radius from its transform modifier (slot 14), or 0 without one.
 *
 * @return the radius (0.0 when the block has no transform modifier)
 */
float tModifierBlock::getRadius()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        return node->getRadius();
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_setRadius__FP14tModifierBlock);
#ifdef SKIP_ASM
/**
 * Passes a radius to the modifier block's root transform modifier through its virtual setRadius (slot 15).
 *
 * Does nothing without a root transform modifier (+0x0, On Tour RootTransform). The radius (On Tour's float
 * argument) travels in $f12 untouched; the mangled name declares no float parameter.
 *
 * @param radius  the new radius
 */
void tModifierBlock::setRadius(float radius)
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->setRadius(radius);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_preUpdate);
#ifdef SKIP_ASM



extern "C" void cUVScrollModifier_update(cUVScrollModifier* modifier);
extern "C" void cTexFlipModifier_update(cTexFlipModifier* modifier);
extern "C" void tModifierBlock_remPhysicsModifier(tModifierBlock* self);

/**
 * Updates every modifier in the block before the instance moves, dropping a physics modifier that has gone
 * inactive.
 *
 * Runs RootTransform's update (slot 2), cUVScrollModifier_update and cTexFlipModifier_update, then Physics' update
 * (slot 2); when Physics' isActive (slot 4) is 0 afterwards it is removed (tModifierBlock_remPhysicsModifier).
 */
void tModifierBlock::preUpdate()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->update();
    }
    if (UVScroll != 0) {
        cUVScrollModifier_update(UVScroll);
    }
    if (TexFlip != 0) {
        cTexFlipModifier_update(TexFlip);
    }
    cPhysicsModifier* physicsModifier = Physics;
    if (physicsModifier != 0) {
        physicsModifier->update();
        physicsModifier = Physics;
        if (physicsModifier->isActive() == 0) {
            remPhysicsModifier();
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_postUpdate);
#ifdef SKIP_ASM


/**
 * Runs a modifier block's post-update pass: updates its effects and rails, then its root transform modifier.
 *
 * Calls update (slot 5) on every effect link (Effect, +0x10), update (slot 3) on every rail modifier (Rail,
 * +0x1C) and postUpdate (slot 4) on the root transform modifier (+0x0) when there is one. Each link's next
 * pointer is read before its call.
 */
void tModifierBlock::postUpdate()
{
    void* head = Effect.Next;  // one local for both list heads (the effect list, then the rail list)
    if (head != 0) {
        cEffectModifier* link = (cEffectModifier*)head;
        do {
            cEffectModifier* next = (cEffectModifier*)link->Next;
            link->update();
            link = next;
        } while (link != 0);
    }
    head = Rail.Next;
    if (head != 0) {
        cRailModifier* link = (cRailModifier*)head;
        do {
            cRailModifier* next = (cRailModifier*)link->Next;
            link->update();
            link = next;
        } while (link != 0);
    }
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->postUpdate();
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_render);
#ifdef SKIP_ASM

/**
 * Renders a modifier block: slot 6 of each effect link in the list at +0x10, then slot 11 of the transform modifier.
 */
void tModifierBlock::render()
{
    if (Effect.Next != 0) {
        cEffectModifier* effect = (cEffectModifier*)Effect.Next;
        cEffectModifier* next;
        do {
            next = (cEffectModifier*)effect->Next;
            effect->render();
            effect = next;
        } while (effect != 0);
    }
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->render();
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_isRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Tells whether the block's transform modifier reports a changed transform (slot 17).
 *
 * @return true when there is a transform modifier and it is dirty
 */
bool tModifierBlock::isRootTransformDirty()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        if (node->isRootTransformDirty() != 0) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_updateRootTransform);
#ifdef SKIP_ASM

/**
 * Updates the block's root transform: slot 3 of its transform modifier (+0x0), when it has one.
 */
void tModifierBlock::updateRootTransform()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->updateRootTransform();
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_isComplete);
#ifdef SKIP_ASM
/**
 * Asks the modifier block's node whether it is complete.
 *
 * Calls slot 6 of the node at offset 0.
 *
 * @return the node's answer, or 0 when the block has no node
 */
int tModifierBlock::isComplete()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        return node->isComplete();
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_clearComplete);
#ifdef SKIP_ASM
/**
 * Clears the modifier block node's complete state.
 *
 * Calls slot 7 of the node at offset 0; does nothing when the block has no node.
 */
void tModifierBlock::clearComplete()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->clearComplete();
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_signal);
#ifdef SKIP_ASM



extern "C" void cUVScrollModifier_signal(void* self, int id, float v);
extern "C" void cTexFlipModifier_signal(void* self, int id, float v);

/**
 * Passes a script signal to every modifier in the block and to each linked effect.
 *
 * Sends (@p id, @p value) to RootTransform (slot 5), the UV-scroll and tex-flip modifiers, Physics (slot 3) and every
 * node of the effect link list (+0x10, slot 7).
 *
 * @param id  the signal id (On Tour signal)
 * @param value  the signal's argument value (On Tour arg)
 */
void tModifierBlock::signal(int id, float value)
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->signal(id, value);
    }
    if (UVScroll != 0) {
        cUVScrollModifier_signal(UVScroll, id, value);
    }
    if (TexFlip != 0) {
        cTexFlipModifier_signal(TexFlip, id, value);
    }
    cPhysicsModifier* physicsModifier = Physics;
    if (physicsModifier != 0) {
        physicsModifier->signal(id, value);
    }
    cEffectLink* firstEffect = Effect.Next;
    if (firstEffect != 0) {
        cEffectModifier* effect = (cEffectModifier*)firstEffect;
        do {
            cEffectModifier* next = (cEffectModifier*)effect->Next;
            effect->signal(id, value);
            effect = next;
        } while (effect != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_applyForce);
#ifdef SKIP_ASM


/**
 * Passes a force on to the move node that owns the modifier block's root transform modifier.
 *
 * Asks the root transform modifier (+0x0) for its parent (getParent, slot 25) and calls that node's
 * applyForce (slot 43); does nothing without either.
 *
 * @param intInfo  the intersect info, passed through (On Tour const tIntersectInfo&)
 * @param force    the force vector, passed through (On Tour const t4Vector&)
 */
void tModifierBlock::applyForce(int intInfo, int force)
{
    cTransformModifier* node = RootTransform;
    cMoveNode* obj;
    if (node != 0) {
        obj = node->getParent();
    } else {
        obj = 0;
    }
    if (((void*)obj) != 0) {
        sVEntry<void(void*, int, int)>* vt2 = (sVEntry<void (void*, int, int)>*)VPTR_OF(cObjNode, obj);
        vt2[43].fn(vthis(obj, vt2[43]), intInfo, force);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_updateIntersectInfo);
#ifdef SKIP_ASM
/**
 * Lets the modifier block's root transform modifier add its motion to an intersect info.
 *
 * Calls the modifier's virtual updateIntersectInfo (slot 22); does nothing when the block has no root
 * transform modifier.
 *
 * @param intInfo  the intersect info to update (On Tour tIntersectInfo&)
 */
void tModifierBlock::updateIntersectInfo(void* intInfo)
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        node->updateIntersectInfo(*(tIntersectInfo*)intInfo);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remTransformModifier);
#ifdef SKIP_ASM
/**
 * Deletes the block's transform modifier through its destructor virtual (slot 1, flags 3) and clears the slot.
 */
void tModifierBlock::remTransformModifier()
{
    cTransformModifier* node = RootTransform;
    if (node != 0) {
        delete node;
        RootTransform = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remUVScrollModifier);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees the block's UV-scroll modifier (operator_delete only, no destructor runs) and clears the slot.
 */
void tModifierBlock::remUVScrollModifier()
{
    if (UVScroll != 0) {
        operator_delete((int*)UVScroll);
        UVScroll = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remTexFlipModifier);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees the block's tex-flip modifier (operator_delete only, no destructor runs) and clears the slot.
 */
void tModifierBlock::remTexFlipModifier()
{
    if (TexFlip != 0) {
        operator_delete((int*)TexFlip);
        TexFlip = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remPhysicsModifier);
#ifdef SKIP_ASM
/**
 * Deletes the block's physics modifier through its destructor virtual (vtable at +0x4, slot 1, flags 3) and clears
 * the slot.
 */
void tModifierBlock::remPhysicsModifier()
{
    cPhysicsModifier* obj = Physics;
    if (obj != 0) {
        delete obj;
        Physics = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remEffectModifier);
#ifdef SKIP_ASM
/**
 * Removes and deletes all of a modifier block's effect modifiers.
 *
 * Walks the Effect list (head at +0x10), reading each link's next first, and deletes each one through virtual slot 1
 * (the destructor, flags 3); each destructor unlinks itself (On Tour asserts the list is empty afterwards).
 */
void tModifierBlock::remEffectModifier()
{
    cEffectLink* head = Effect.Next;
    if (head != 0) {
        cEffectLink* effect = head;
        do {
            cEffectLink* next = effect->Next;
            delete effect;
            effect = next;
        } while (effect != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_fadeEffectModifier);
#ifdef SKIP_ASM

/**
 * Tells every effect on a modifier block to fade out.
 *
 * Walks the effect chain (+0x10, cEffectLink list) and calls each link's vtable entry 3 (fadeEffect), reading the next
 * link first.
 */
void tModifierBlock::fadeEffectModifier()
{
    cEffectLink* head = Effect.Next;
    if (head != 0) {
        cEffectLink* link = head;
        do {
            cEffectLink* next = link->Next;
            link->fadeEffect();
            link = next;
        } while (link != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remHaloModifier);
#ifdef SKIP_ASM

/**
 * Removes every halo effect from a modifier block.
 *
 * Walks the effect chain (+0x10) and deletes (vtable entry 1, flags 3) each link whose getType (vtable entry 2) is
 * cEffectLink::kEffect_Halo, the halo modifier's type.
 */
void tModifierBlock::remHaloModifier()
{
    cEffectLink* head = Effect.Next;
    if (head != 0) {
        cEffectLink* link = head;
        do {
            cEffectLink* next = link->Next;
            if (link->getType() == cEffectLink::kEffect_Halo) {
                if (link != 0) {
                    delete link;
                }
            }
            link = next;
        } while (link != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remRailModifier);
#ifdef SKIP_ASM

/**
 * Removes and deletes one rail modifier of a modifier block, or all of them.
 *
 * With @p id -1 (an invalid tResourceID) every link of the Rail list (head at +0x1C) is deleted through virtual slot
 * 1 (the destructor, flags 3). Otherwise only the first link whose spline id (+0x30) equals @p id is deleted.
 *
 * @param id  the rail's spline resource id (On Tour splineId), or -1 for all
 */
void tModifierBlock::remRailModifier(unsigned int id)
{
    cRailModifier* head = (cRailModifier*)Rail.Next;
    if (head == 0) return;
    cRailModifier* rail = head;
    if (~id) {
    loop:
        if (rail == 0) return;
        if (rail->SplineId == id) {
            delete rail;
            return;
        }
        rail = ((cRailModifier*)rail->Next);
        goto loop;
    } else {
        do {
            cRailModifier* next = ((cRailModifier*)rail->Next);
            if (rail != 0) {
                delete rail;
            }
            rail = next;
        } while (rail != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_transferEffectModifier);
#ifdef SKIP_ASM

extern "C" void cEffectLink_append(void* pool, void* item);

/**
 * Takes over the effect modifiers of another effect list (another node's modifier block).
 *
 * For each effect of @p list (read next first) it calls the effect's virtual slot 3, then appends it to this block's
 * Effect list (+0x10) with cEffectLink_append; finally the source list's Next and Prev are cleared.
 *
 * @param list  the effect list head to empty (On Tour link): another modifier block's +0x10
 */
void tModifierBlock::transferEffectModifier(cEffectLink* list)
{
    cEffectLink* effect = list->Next;
    while (effect != 0) {
        cEffectLink* next = effect->Next;
        effect->fadeEffect();
        cEffectLink_append(&Effect, effect);
        effect = next;
    }
    list->Next = 0;
    list->Prev = 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_findRail);
#ifdef SKIP_ASM
/**
 * Tells whether the modifier block has a rail modifier for a given spline.
 *
 * Walks the rail list (Rail, +0x1C) comparing each rail's SplineId (+0x30).
 *
 * @param id  the spline's resource id (On Tour tResourceID splineId)
 * @return true if a rail with that spline id is found
 */
bool tModifierBlock::findRail(int id)
{
    cRailModifier* rail = (cRailModifier*)Rail.Next;
    while (rail != 0) {
        if (rail->SplineId == id) {
            return true;
        }
        rail = (cRailModifier*)rail->Next;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cTexFlipModifier_writeToReplayFrame(void* self, void* stream);
extern "C" void cUVScrollModifier_writeToReplayFrame(void* self, void* stream);

struct sVE_353448 { short delta; short index; int (*fn)(void*); };
struct sVEv_353448 { short delta; short index; void (*fn)(void*, void*); };



static inline int VCall0_353448(char* o, int voff, int slot)
{
    sVE_353448* e = &(*(sVE_353448**)(o + voff))[slot];
    return e->fn(vthis(o, *e));
}

static inline void VCall1_353448(char* o, int voff, int slot, void* arg)
{
    sVEv_353448* e = &(*(sVEv_353448**)(o + voff))[slot];
    e->fn(vthis(o, *e), arg);
}

/**
 * Writes a modifier block to a replay frame: a header saying which modifiers it has, then each modifier.
 *
 * The 0x34-byte header (On Tour tModifierBlockInfo) holds the flags (1 root transform, 2 UV scroll, 4 tex
 * flip, 8 physics, 0x10 effects, 0x20 rails), the transform and physics types (getTransformType slot 26,
 * getPhysicsType slot 5), the effect and rail counts and each effect's type (slot 2). Each present modifier
 * then writes itself in that order. The 32-entry effect type table is filled without a bound.
 *
 * @param stream  replay frame to write to (On Tour cBxReplayFrame*)
 */
void tModifierBlock::writeToReplayFrame(cBxReplayFrame* stream)
{
    tModifierBlockInfo info;
    info.Flags = 0;
    info.PhysicsType = 0;
    info.TransformType = 0;
    info.NumEffects = 0;
    info.NumRails = 0;
    for (int i = 0; i < 32; i++) {
        info.EffectType[i] = 0;
    }
    char* transformModifier = (char*)RootTransform;
    if (transformModifier != 0) {
        info.Flags |= 1;
        info.TransformType = VCall0_353448(transformModifier, 0, 26);
    }
    if (UVScroll != 0) {
        info.Flags |= 2;
    }
    if (TexFlip != 0) {
        info.Flags |= 4;
    }
    char* oC = (char*)Physics;
    if (oC != 0) {
        info.Flags |= 8;
        info.PhysicsType = VCall0_353448(oC, 4, 5);
    }
    for (cEffectLink* link = Effect.Next; link; link = link->Next) {
        info.EffectType[info.NumEffects++] = VCall0_353448((char*)link, 8, 2);
    }
    if (info.NumEffects > 0) {
        info.Flags |= 0x10;
    }
    for (cRailLink* link = Rail.Next; link; link = link->Next) {
        info.NumRails++;
    }
    if (info.NumRails > 0) {
        info.Flags |= 0x20;
    }
    {
        stream->write((const char*)&info, 52);
    }
    char* w0 = (char*)RootTransform;
    if (w0 != 0) {
        VCall1_353448(w0, 0, 27, stream);
    }
    if (UVScroll != 0) {
        cUVScrollModifier_writeToReplayFrame(UVScroll, stream);
    }
    if (TexFlip != 0) {
        cTexFlipModifier_writeToReplayFrame(TexFlip, stream);
    }
    char* wC = (char*)Physics;
    if (wC != 0) {
        VCall1_353448(wC, 4, 6, stream);
    }
    if (info.NumEffects > 0) {
        for (cEffectLink* link = Effect.Next; link; link = link->Next) {
            VCall1_353448((char*)link, 8, 4, stream);
        }
    }
    if (info.NumRails > 0) {
        for (cRailLink* link = Rail.Next; link; link = link->Next) {
            VCall1_353448((char*)link, 8, 2, stream);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_readFromReplayFrame);
#ifdef SKIP_ASM


extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cSplineModifier_cSplineModifier(void* self, cReplayFramePtr* s);
extern "C" void* cMultiSplineModifier_cMultiSplineModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cParentModifier_cParentModifier(void* self, cReplayFramePtr* s);
extern "C" void* cPositionModifier_cPositionModifier2(void* self, cReplayFramePtr* s);
extern "C" void* cRollerModifier_cRollerModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cMagnetModifier_cMagnetModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cAvaSplineModifier_cAvaSplineModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cUVScrollModifier_cUVScrollModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cTexFlipModifier_cTexFlipModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cBoostModifier_cBoostModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cParticleModifier_cParticleModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cDynamicParticleModifier_cDynamicParticleModifier(void* self, cReplayFramePtr* s);
extern "C" void* cHaloModifier_cHaloModifier1(void* self, cReplayFramePtr* s);
extern "C" void* cRailModifier_cRailModifier1(void* self, cReplayFramePtr* s);
extern "C" void cEffectLink_append(void* pool, void* item);
extern "C" void cRailLink_append(void* list, void* item);

extern const char str_SplineModifier[];
extern const char str_ParentModifier[];
extern const char str_PositionModifier[];
extern const char str_RollerModifier[];
extern const char str_MagnetModifier[];
extern const char str_AvaSplineModifier[];
extern const char str_UVScrollModifier[];
extern const char str_TexFlipModifier[];
extern const char str_BoostModifier[];
extern const char str_ParticleModifier[];
extern const char str_DynamicParticleModifier[];
extern const char str_HaloModifier[];
extern const char str_RailModifier[];

/**
 * Rebuilds a modifier block from a replay frame: reads the header and re-creates every modifier it lists.
 *
 * The root transform is built by its cTransformModifier::eTransformType (kTransform_Spline ... kTransform_AvaSpline);
 * UV scroll, tex flip and (physics type kPhysics_Boost) boost modifiers follow their flags. Effects are rebuilt by
 * their cEffectLink::eEffectType (kEffect_Particle, kEffect_DynamicParticle, kEffect_Halo) and appended to Effect
 * (+0x10), rails to Rail (+0x1C). An unknown effect type appends a null pointer, which cEffectLink_append writes
 * through.
 *
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 */
void tModifierBlock::readFromReplayFrame(cBxReplayFrame* stream)
{
    tModifierBlockInfo hdr;
    int i;

    RootTransform = 0;
    UVScroll = 0;
    TexFlip = 0;
    Physics = 0;
    stream->read((char*)&hdr, 52);

    if (hdr.Flags & 1) {
        if (hdr.TransformType == cTransformModifier::kTransform_Spline) {
            RootTransform = (cTransformModifier*)cSplineModifier_cSplineModifier(cMemMan_alloc(0xF0,
                    str_SplineModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_MultiSpline) {
            RootTransform = (cTransformModifier*)cMultiSplineModifier_cMultiSplineModifier1(cMemMan_alloc(88,
                    str_SplineModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_Parent) {
            RootTransform = (cTransformModifier*)cParentModifier_cParentModifier(cMemMan_alloc(176, str_ParentModifier,
                    0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_Position) {
            RootTransform = (cTransformModifier*)cPositionModifier_cPositionModifier2(cMemMan_alloc(144,
                    str_PositionModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_Roller) {
            RootTransform = (cTransformModifier*)cRollerModifier_cRollerModifier1(cMemMan_alloc(720,
                    str_RollerModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_Magnet) {
            RootTransform = (cTransformModifier*)cMagnetModifier_cMagnetModifier1(cMemMan_alloc(176,
                    str_MagnetModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.TransformType == cTransformModifier::kTransform_AvaSpline) {
            RootTransform = (cTransformModifier*)cAvaSplineModifier_cAvaSplineModifier1(cMemMan_alloc(144,
                    str_AvaSplineModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        }
    }
    if (hdr.Flags & 2) {
        UVScroll = (cUVScrollModifier*)cUVScrollModifier_cUVScrollModifier1(cMemMan_alloc(80, str_UVScrollModifier,
                0x20000000, 0), (cReplayFramePtr*)stream);
    }
    if (hdr.Flags & 4) {
        TexFlip = (cTexFlipModifier*)cTexFlipModifier_cTexFlipModifier1(cMemMan_alloc(232, str_TexFlipModifier,
                0x20000000, 0), (cReplayFramePtr*)stream);
    }
    if ((hdr.Flags & 8) && hdr.PhysicsType == cPhysicsModifier::kPhysics_Boost) {
        Physics = (cPhysicsModifier*)cBoostModifier_cBoostModifier1(cMemMan_alloc(80, str_BoostModifier, 0x20000000,
                0), (cReplayFramePtr*)stream);
    }
    for (i = 0; i < hdr.NumEffects; i++) {
        void* effect;
        if (hdr.EffectType[i] == cEffectLink::kEffect_Particle) {
            effect = cParticleModifier_cParticleModifier1(cMemMan_alloc(0x1F0, str_ParticleModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.EffectType[i] == cEffectLink::kEffect_DynamicParticle) {
            effect = cDynamicParticleModifier_cDynamicParticleModifier(cMemMan_alloc(624, str_DynamicParticleModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else if (hdr.EffectType[i] == cEffectLink::kEffect_Halo) {
            effect = cHaloModifier_cHaloModifier1(cMemMan_alloc(0x40, str_HaloModifier, 0x20000000, 0), (cReplayFramePtr*)stream);
        } else {
            effect = 0;
        }
        cEffectLink_append(&Effect, effect);
    }
    for (i = 0; i < hdr.NumRails; i++) {
        cRailLink_append((char*)&Rail, cRailModifier_cRailModifier1(cMemMan_alloc(144, str_RailModifier, 0x20000000,
                0), (cReplayFramePtr*)stream));
    }
}
#endif

extern void* cSkyBox_vtbl[];

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox_cSkyBox__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the sky box: no section and no sky instance yet.
 *
 * Stores cSkyBox_vtbl (+0xC), clears the list links (+0x4, +0x8), sets the section id (+0x10) to -1 and the sky
 * instance (+0x14) to 0. cGame_load allocates it (0x18 bytes, "SkyBox").
 *
 * @return the constructed sky box (self)
 */
void* cSkyBox_cSkyBox(void* vself)
{
    cSkyBox* self = (cSkyBox*)vself;
    VPTR_OF(cObjNode, self) = cSkyBox_vtbl;
    self->m_iSectionID = -1;
    self->Next = 0;
    self->Prev = 0;
    self->m_pInstanceSky = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", _$_7cSkyBox);
#ifdef SKIP_ASM
/**
 * Destroys the sky box; it owns nothing of its own, so its cObjNode part does the work (it takes the node out of its
 * render bucket) and frees it when asked.
 */
cSkyBox::~cSkyBox()
{
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox_render);
#ifdef SKIP_ASM
// PORT: cObjectInterface_getCurrentCameraNum is defined returning void*, but its value is the rider index passed to cObjectInterface_getCameraPosition(int).
extern "C" int cObjectInterface_getCurrentCameraNum_i() __asm__("cObjectInterface_getCurrentCameraNum");
extern "C" void* cObjectInterface_getCameraPosition(int i);
extern int cRenderToggles_m_DisableSkyBox;
extern t4Matrix t4mat_Identity;



struct sRS_53B10 {
    unsigned int f0lo : 2;
    unsigned int f0b : 2;
    unsigned int f0hi : 28;
    unsigned int f4lo : 12;
    unsigned int alpha : 8;
    unsigned int zmode : 2;
    unsigned int zon : 1;
    unsigned int blendm : 2;
    unsigned int f4hi : 7;
    int f8;                     // bits 5..9 = blend
    int fC;
    int f10;
};

struct sVE_53B10 { short delta; short index; void (*fn)(void*, void*, float, void*, void*, int, int, int); };

struct sCtx_53B10 {
    char pad_0x0[0xE84];
    sRS_53B10* top;             // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sVE_53B10* vtable;          // 0x10D8
};


// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrix_53B10(t4Matrix* dst, void* src)
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

static inline void SetZOn_53B10(sRS_53B10* r, int v) { r->zon = v; }
static inline void SetBlendM_53B10(sRS_53B10* r, int v) { r->blendm = v; }
static inline void SetZMode_53B10(sRS_53B10* r, int v) { r->zmode = v; }
static inline void SetAlpha_53B10(sRS_53B10* r, int v) { r->alpha = v; }

/**
 * Draws the sky box instance around the current view's position with its own draw state.
 *
 * Skipped when the "Disable Sky Box" debug toggle (cRenderToggles_m_DisableSkyBox) is set or there is no sky instance (+0x14, On
 * Tour m_pInstanceSky). Pushes a copy of the draw state with the blend field (bits 5-9 of word 8) cleared,
 * zon 1, blendm 2, zmode 0 and alpha 0x14, sets bit 2 and clears bit 1 of the record's flags, and draws
 * through the graphics manager's slot 95 with an identity matrix moved to the current view's position
 * (cObjectInterface_getCameraPosition) and flags 0x5420. Then pops the state and writes the saved blend bits back.
 */
void cSkyBox::render()
{
    if (cRenderToggles_m_DisableSkyBox != 0)
        return;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (m_pInstanceSky == 0)
        return;
    t4Matrix skyMatrix;
    vu0CopyMatrix_53B10(&skyMatrix, &t4mat_Identity);
    t4Vector pos = *(t4Vector*)cObjectInterface_getCameraPosition(cObjectInterface_getCurrentCameraNum_i());
    *(t4Vector*)&((float*)&skyMatrix.row[0])[12] = pos;
    ((sRS_53B10*)ctx->m_pCurrentDrawState)[1] = *(sRS_53B10*)&ctx->m_pCurrentDrawState->m_bitGroups[0];
    sRS_53B10* drawState = (sRS_53B10*)ctx->m_pCurrentDrawState;
    *(sRS_53B10**)&ctx->m_pCurrentDrawState = drawState + 1;
    int stateFlags = drawState[1].f8;
    drawState[1].f8 = stateFlags & ~0x3E0;
    int old = (unsigned int)(stateFlags & 0x3E0) >> 5;
    ((sRS_53B10*)ctx->m_pCurrentDrawState)->f0b = 3;
    SetZOn_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 1);
    SetBlendM_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 2);
    SetZMode_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 0);
    SetAlpha_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 20);
    tInstanceRecord* skyInstance = m_pInstanceSky;
    if (((char*)skyInstance) != 0)
    {
        skyInstance->m_iFlags = ((unsigned int)skyInstance->m_iFlags & ~2u) | 4;
        tInstanceRecord* instToDraw = m_pInstanceSky;
        ctx->RenderModel(instToDraw, instToDraw->m_Model->m_MaterialBlock, skyMatrix, 1.0f, 0, (const t4Matrix*)(0), 21536);
    }
    sRS_53B10* topDrawState = (sRS_53B10*)ctx->m_pCurrentDrawState;
    *(sRS_53B10**)&ctx->m_pCurrentDrawState = topDrawState - 1;
    int* pf = (int*)((char*)topDrawState - 0xC);
    *pf = (*pf & ~0x3E0) | (old << 5);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox_setSection);
#ifdef SKIP_ASM
struct sMbTrackEntry3CF0 {
    char pad_0x0[0x26];
    short count;            // 0x26
    char pad_0x28[0x30];
};

struct sMbTrackTable3CF0 {
    char pad_0x0[0x14];
    sMbTrackEntry3CF0* entries; // 0x14
};

struct sMbModelSet3CF0 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};

struct sMbWorld3CF0 {
    sMbTrackTable3CF0* table;   // 0x0
    int field_0x4;
    sMbModelSet3CF0** sets;     // 0x8
};

extern "C" sMbWorld3CF0** cObjectInterface_getWorld();
int cSkyBox_reset(void* self);

static inline void* refToPtr3CF0(unsigned int p)
{
    return (void*)(p << 2);
}

struct sMbModelRef3CF0 {
    unsigned int id;

    void* get(sMbWorld3CF0* w)
    {
        sMbModelSet3CF0* set = w->sets[id & 0xFF];
        if (set == 0) {
            return 0;
        }
        unsigned int p = set->refs[id >> 8] >> 8;
        if (p == 0) {
            return 0;
        }
        return refToPtr3CF0(p);
    }
};

struct sMbTrackRef3CF0 {
    char pad_0x0[0x10];
    int id;                 // 0x10
    void* model;            // 0x14
};

/**
 * Points the sky box at a world section's sky instance.
 *
 * Stores id as the section (+0x10). A negative id resets the sky box (cSkyBox_reset); otherwise, when the world's
 * section table has entries for it, the sky instance (+0x14) becomes record 0 of that section's resource set.
 *
 * @param id  world section id, or negative for none
 */
void cSkyBox::setSection(int id)
{
    m_iSectionID = id;
    if (id < 0) {
        reset();
        return;
    }
    sMbWorld3CF0* w = *cObjectInterface_getWorld();
    if (w->table->entries[id].count > 0) {
        sMbModelRef3CF0 ref;
        ref.id = id & 0xFF;
        m_pInstanceSky = (tInstanceRecord*)ref.get(w);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox_reset__FPv);
#ifdef SKIP_ASM
/**
 * Resets the sky box: section -1 (+0x10) and no sky instance (+0x14).
 *
 * @return -1 (left over from the store; callers ignore it)
 */
int cSkyBox::reset()
{
    int invalid = -1;
    m_pInstanceSky = 0;
    m_iSectionID = invalid;
    return invalid;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSpring_cSpring__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a spring with no node array (Nodes, +0x24, = 0).
 *
 * @return self
 */
void* cSpring_cSpring(void* vself)
{
    cSpring* self = (cSpring*)vself;
    self->Nodes = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", _$_7cSpring);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys a spring: frees its node array (+0x24, On Tour Nodes) when it has one, then frees the object when
 * asked to.
 */
cSpring::~cSpring()
{
    void* nodes = Nodes;
    if (nodes != 0) {
        cMemMan_free(nodes);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSpring_setupSpring);
#ifdef SKIP_ASM
extern "C" void cSpring_setupNodes(void* self);



/**
 * Sets a spring up from its data and length: copies the parameters and lays the nodes out evenly.
 *
 * Copies the node count (data +0x1C, On Tour Width) to +0x0 and the parameters at data +0x0..+0x18 to +0x4..+0x1C;
 * the one at data +0x14 is stored scaled by 1e-6. The spacing (+0x20) is @p len / (count - 1); then
 * cSpring_setupNodes() builds the nodes. On Tour asserts Width <= kMaxSpringNodes; retail does not.
 *
 * @param sdata    the spring data (On Tour tSpringData* sdata)
 * @param len  the spring's length (On Tour length)
 */
void cSpring::setupSpring(tSpringData* sdata, float len)
{
    cSpring* spring = (cSpring*)this;
    int n = sdata->Width;
    spring->Width = n;
    spring->LSpring = sdata->LSpring;
    spring->LDamp = sdata->LDamp;
    spring->ASpring = sdata->ASpring;
    spring->ADamp = sdata->ADamp;
    spring->ForceScale = sdata->ForceScale;
    spring->TorqueScale = sdata->TorqueScale * 9.999999974752427e-07f;
    spring->Length = sdata->LengthScale;
    spring->SegLen = len / (float)(n - 1);
    cSpring_setupNodes(this);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSpring_cSpring1);
#ifdef SKIP_ASM

extern "C" void cSpring_setupNodes(void* self);

/**
 * Constructs a spring from a replay frame.
 *
 * Reads the 36-byte spring header (node count first) through the stream's read slot (vtable entry 2), allocates the
 * nodes (cSpring_setupNodes) and reads count * 80 bytes into Nodes (+0x24).
 *
 * @param stream  replay stream to read from
 * @return self
 */
void* cSpring::cSpring1(cBxReplayFrame* stream)
{
    stream->read((char*)this, 36);
    cSpring_setupNodes(this);
    stream->read((char*)Nodes, *(int*)this * 80);
    return ((void*)this);
}
#endif

