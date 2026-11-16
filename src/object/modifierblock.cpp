#include "common.h"
#include "render/ps2graphicsman.h"

struct sBoundBoxTarget {
    char pad_0x00[0x68];
    short field_0x68;
    char pad_0x6A[2];
    void (*fnBoundBox)(void*); // 0x6C
    char pad_0x70[0x78 - 0x70];
    short field_0x78;
    char pad_0x7A[2];
    void (*fnRadius)(void*); // 0x7C
};

struct sBoundBoxNode {
    sBoundBoxTarget* target; // 0x0
};

struct tModifierBlock {
    sBoundBoxNode* node; // 0x0
    int field_0x4;
    int field_0x8;
    int field_0xC;
    int field_0x10;
    int field_0x14;
    void* field_0x18;
    int field_0x1C;
    int field_0x20;
    void* field_0x24;
};

extern void* cEffectLink_vtbl[16];
extern void* cRailLink_vtbl[16];

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_tModifierBlock__FP14tModifierBlock);
#ifdef SKIP_ASM
/**
 * Constructs an empty modifier block.
 *
 * Clears the node pointer, the two buffers, the object at offset 0xC and the heads of the effect-link and
 * rail-link lists, and stores the cEffectLink and cRailLink vtables of those two lists.
 *
 * @return @p self
 */
tModifierBlock* tModifierBlock_tModifierBlock(tModifierBlock* self)
{
    self->field_0x18 = cEffectLink_vtbl;
    self->field_0x24 = cRailLink_vtbl;
    self->field_0x10 = 0;
    self->field_0x14 = 0;
    self->field_0x1C = 0;
    self->field_0x20 = 0;
    self->node = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xC = 0;
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
extern "C" void func_003531D0(void* self);
extern "C" void func_00353300(void* self, unsigned int mask);
extern "C" void func_0035B6D0(void* self);
extern "C" void func_00345760(void* self);
void operator_delete(int* ptr);

/**
 * Destroys a modifier block and everything it owns.
 *
 * Deletes the node, frees the two buffers at offsets 4 and 8, deletes the object at offset 0xC, every effect
 * link and every rail link (func_00353300 with id ~0), then tears down the two list heads.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void tModifierBlock__tModifierBlock(tModifierBlock* self, int flags)
{
    tModifierBlock_remTransformModifier(self);
    tModifierBlock_remUVScrollModifier(self);
    tModifierBlock_remTexFlipModifier(self);
    tModifierBlock_remPhysicsModifier(self);
    func_003531D0(self);
    func_00353300(self, 0xFFFFFFFF);
    self->field_0x24 = cRailLink_vtbl;
    func_0035B6D0(&self->field_0x1C);
    self->field_0x18 = cEffectLink_vtbl;
    func_00345760(&self->field_0x10);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_getBoundBox);
#ifdef SKIP_ASM
struct sMbVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Returns the bounding box of the modifier block's node.
 *
 * Calls slot 12 of the node at offset 0.
 *
 * @return the node's result, or 0 when the block has no node
 */
extern "C" int tModifierBlock_getBoundBox(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        return vt[12].fn((char*)node + vt[12].delta);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_setBoundBox__FP14tModifierBlock);
#ifdef SKIP_ASM
struct sMbVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

// PORT: the symbol's mangling says one parameter, but the body passes $5 through
// to the virtual call untouched, so the real function takes a second argument.
void tModifierBlock_setBoundBox_impl(tModifierBlock* self, void* box) __asm__("tModifierBlock_setBoundBox__FP14tModifierBlock");

/**
 * Passes a bounding box to the modifier block's node.
 *
 * Calls slot 13 of the node with @p box; does nothing when the block has no node.
 *
 * @param box  the bounding box, passed through unchanged
 */
void tModifierBlock_setBoundBox_impl(tModifierBlock* self, void* box)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntry* vt = *(sMbVEntry**)node;
        vt[13].fn((char*)node + vt[13].delta, box);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_getRadius);
#ifdef SKIP_ASM
struct sMbVEntryF {
    short delta;
    short index;
    float (*fn)(void*);
};

/**
 * Returns the block's bounding radius from its transform modifier (slot 14), or 0 without one.
 *
 * @return the radius (0.0 when the block has no transform modifier)
 */
extern "C" float tModifierBlock_getRadius(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryF* vt = *(sMbVEntryF**)node;
        return vt[14].fn((char*)node + vt[14].delta);
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
 * argument) travels in $f12 untouched; the mangled name and the C declare no float parameter.
 */
void tModifierBlock_setRadius(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sBoundBoxTarget* target = node->target;
        target->fnRadius((char*)node + target->field_0x78);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_preUpdate);
#ifdef SKIP_ASM
struct sMbVEntryV2C70 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMbVEntryI2C70 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sMbChild2C70 {
    int field_0x0;
    sMbVEntryV2C70* vt;     // 0x4
};

extern "C" void cUVScrollModifier_update(int h);
extern "C" void cTexFlipModifier_update(int h);
extern "C" void tModifierBlock_remPhysicsModifier(tModifierBlock* self);

/**
 * Updates every modifier in the block before the instance moves, dropping a physics modifier that has gone
 * inactive.
 *
 * Runs RootTransform's update (slot 2), cUVScrollModifier_update and cTexFlipModifier_update, then Physics' update
 * (slot 2); when Physics' isActive (slot 4) is 0 afterwards it is removed (tModifierBlock_remPhysicsModifier).
 */
extern "C" void tModifierBlock_preUpdate(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV2C70* vt = *(sMbVEntryV2C70**)node;
        vt[2].fn((char*)node + vt[2].delta);
    }
    if (self->field_0x4 != 0) {
        cUVScrollModifier_update(self->field_0x4);
    }
    if (self->field_0x8 != 0) {
        cTexFlipModifier_update(self->field_0x8);
    }
    sMbChild2C70* physicsModifier = (sMbChild2C70*)self->field_0xC;
    if (physicsModifier != 0) {
        physicsModifier->vt[2].fn((char*)physicsModifier + physicsModifier->vt[2].delta);
        physicsModifier = (sMbChild2C70*)self->field_0xC;
        sMbVEntryI2C70* vt2 = (sMbVEntryI2C70*)physicsModifier->vt;
        if (vt2[4].fn((char*)physicsModifier + vt2[4].delta) == 0) {
            tModifierBlock_remPhysicsModifier(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_postUpdate);
#ifdef SKIP_ASM
struct sMbVEntryV2D20 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sMbLink2D20 {
    sMbLink2D20* next;      // 0x0
    int field_0x4;
    sMbVEntryV2D20* vt;     // 0x8
};

/**
 * Runs a modifier block's post-update pass: updates its effects and rails, then its root transform modifier.
 *
 * Calls update (slot 5) on every effect link (Effect, +0x10), update (slot 3) on every rail modifier (Rail,
 * +0x1C) and postUpdate (slot 4) on the root transform modifier (+0x0) when there is one. Each link's next
 * pointer is read before its call.
 */
extern "C" void tModifierBlock_postUpdate(tModifierBlock* self)
{
    sMbLink2D20* head = *(sMbLink2D20**)&self->field_0x10;
    if (head != 0) {
        sMbLink2D20* link = head;
        do {
            sMbLink2D20* next = link->next;
            link->vt[5].fn((char*)link + link->vt[5].delta);
            link = next;
        } while (link != 0);
    }
    head = *(sMbLink2D20**)&self->field_0x1C;
    if (head != 0) {
        sMbLink2D20* link = head;
        do {
            sMbLink2D20* next = link->next;
            link->vt[3].fn((char*)link + link->vt[3].delta);
            link = next;
        } while (link != 0);
    }
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV2D20* vt = *(sMbVEntryV2D20**)node;
        vt[4].fn((char*)node + vt[4].delta);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_render);
#ifdef SKIP_ASM
struct sMbVEntryV2DD0 {
    short delta;
    short index;
    void (*fn)(void*);
};


/**
 * Renders a modifier block: slot 6 of each effect link in the list at +0x10, then slot 11 of the transform modifier.
 */
extern "C" void tModifierBlock_render(tModifierBlock* self)
{
    if (self->field_0x10 != 0) {
        void* effect = (void*)self->field_0x10;
        void* next;
        do {
            next = *(void**)effect;
            sMbVEntryV2DD0* vt = *(sMbVEntryV2DD0**)((char*)effect + 0x8);
            vt[6].fn((char*)effect + vt[6].delta);
            effect = next;
        } while (effect != 0);
    }
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV2DD0* vt = *(sMbVEntryV2DD0**)node;
        vt[11].fn((char*)node + vt[11].delta);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_isRootTransformDirty);
#ifdef SKIP_ASM
/**
 * Tells whether the block's transform modifier reports a changed transform (slot 17).
 *
 * @return 1 when there is a transform modifier and it is dirty, otherwise 0
 */
extern "C" int tModifierBlock_isRootTransformDirty(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        if (vt[17].fn((char*)node + vt[17].delta) != 0) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_updateRootTransform);
#ifdef SKIP_ASM
struct sMbVEntryV_2E98 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Updates the block's root transform: slot 3 of its transform modifier (+0x0), when it has one.
 */
extern "C" void tModifierBlock_updateRootTransform(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV_2E98* vt = *(sMbVEntryV_2E98**)node;
        vt[3].fn((char*)node + vt[3].delta);
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
extern "C" int tModifierBlock_isComplete(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryI* vt = *(sMbVEntryI**)node;
        return vt[6].fn((char*)node + vt[6].delta);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_clearComplete);
#ifdef SKIP_ASM
struct sMbVEntryV {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Clears the modifier block node's complete state.
 *
 * Calls slot 7 of the node at offset 0; does nothing when the block has no node.
 */
extern "C" void tModifierBlock_clearComplete(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryV* vt = *(sMbVEntryV**)node;
        vt[7].fn((char*)node + vt[7].delta);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_signal);
#ifdef SKIP_ASM
struct sMbVEntryP2F40 {
    short delta;
    short index;
    void (*fn)(void*, int, float);
};

struct sMbChild2F40 {
    int field_0x0;
    sMbVEntryP2F40* vt;     // 0x4
};

struct sMbLink2F40 {
    sMbLink2F40* next;      // 0x0
    int field_0x4;
    sMbVEntryP2F40* vt;     // 0x8
};

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
extern "C" void tModifierBlock_signal(tModifierBlock* self, int id, float value)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryP2F40* vt = *(sMbVEntryP2F40**)node;
        vt[5].fn((char*)node + vt[5].delta, id, value);
    }
    if (self->field_0x4 != 0) {
        cUVScrollModifier_signal((void*)self->field_0x4, id, value);
    }
    if (self->field_0x8 != 0) {
        cTexFlipModifier_signal((void*)self->field_0x8, id, value);
    }
    sMbChild2F40* physicsModifier = (sMbChild2F40*)self->field_0xC;
    if (physicsModifier != 0) {
        physicsModifier->vt[3].fn((char*)physicsModifier + physicsModifier->vt[3].delta, id, value);
    }
    sMbLink2F40* firstEffect = *(sMbLink2F40**)&self->field_0x10;
    if (firstEffect != 0) {
        sMbLink2F40* p = firstEffect;
        do {
            sMbLink2F40* next = p->next;
            p->vt[7].fn((char*)p + p->vt[7].delta, id, value);
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_applyForce);
#ifdef SKIP_ASM
struct sMbVEntryP3020 {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sMbVEntryVII3020 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

/**
 * Passes a force on to the move node that owns the modifier block's root transform modifier.
 *
 * Asks the root transform modifier (+0x0) for its parent (getParent, slot 25) and calls that node's
 * applyForce (slot 43); does nothing without either.
 *
 * @param intInfo  the intersect info, passed through (On Tour const tIntersectInfo&)
 * @param force    the force vector, passed through (On Tour const t4Vector&)
 */
extern "C" void tModifierBlock_applyForce(tModifierBlock* self, int intInfo, int force)
{
    sBoundBoxNode* node = self->node;
    void* obj;
    if (node != 0) {
        sMbVEntryP3020* vt = *(sMbVEntryP3020**)node;
        obj = vt[25].fn((char*)node + vt[25].delta);
    } else {
        obj = 0;
    }
    if (obj != 0) {
        sMbVEntryVII3020* vt2 = *(sMbVEntryVII3020**)((char*)obj + 0xC);
        vt2[43].fn((char*)obj + vt2[43].delta, intInfo, force);
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
extern "C" void tModifierBlock_updateIntersectInfo(tModifierBlock* self, void* intInfo)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntry* vt = *(sMbVEntry**)node;
        vt[22].fn((char*)node + vt[22].delta, intInfo);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remTransformModifier);
#ifdef SKIP_ASM
struct sMbVEntryDel {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Deletes the block's transform modifier through its destructor virtual (slot 1, flags 3) and clears the slot.
 */
extern "C" void tModifierBlock_remTransformModifier(tModifierBlock* self)
{
    sBoundBoxNode* node = self->node;
    if (node != 0) {
        sMbVEntryDel* vt = *(sMbVEntryDel**)node;
        vt[1].fn((char*)node + vt[1].delta, 3);
        self->node = 0;
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
extern "C" void tModifierBlock_remUVScrollModifier(tModifierBlock* self)
{
    int** uvScrollSlot = (int**)((char*)self + 0x4);
    if (*uvScrollSlot != 0) {
        operator_delete(*uvScrollSlot);
        *uvScrollSlot = 0;
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
extern "C" void tModifierBlock_remTexFlipModifier(tModifierBlock* self)
{
    int** texFlipSlot = (int**)((char*)self + 0x8);
    if (*texFlipSlot != 0) {
        operator_delete(*texFlipSlot);
        *texFlipSlot = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_remPhysicsModifier);
#ifdef SKIP_ASM
struct sMbVEntryDel2 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Deletes the block's physics modifier through its destructor virtual (vtable at +0x4, slot 1, flags 3) and clears
 * the slot.
 */
extern "C" void tModifierBlock_remPhysicsModifier(tModifierBlock* self)
{
    void* obj = *(void**)((char*)self + 0xC);
    if (obj != 0) {
        sMbVEntryDel2* vt = *(sMbVEntryDel2**)((char*)obj + 0x4);
        vt[1].fn((char*)obj + vt[1].delta, 3);
        *(void**)((char*)self + 0xC) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_003531D0);
#ifdef SKIP_ASM
struct sModBlockDtorVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sModBlockLink {
    sModBlockLink* next;          // 0x0
    int field_0x4;
    sModBlockDtorVEntry* vt;      // 0x8
};

extern "C" void func_003531D0(void* self)
{
    sModBlockLink* head = *(sModBlockLink**)((char*)self + 0x10);
    if (head != 0) {
        sModBlockLink* p = head;
        do {
            sModBlockLink* next = p->next;
            if (p != 0) {
                p->vt[1].fn((char*)p + p->vt[1].delta, 3);
            }
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353228);
#ifdef SKIP_ASM
struct sModBlockVEntry3228 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sModBlockLink3228 {
    sModBlockLink3228* next;      // 0x0
    int field_0x4;
    sModBlockVEntry3228* vt;      // 0x8
};

extern "C" void func_00353228(void* self)
{
    sModBlockLink3228* head = *(sModBlockLink3228**)((char*)self + 0x10);
    if (head != 0) {
        sModBlockLink3228* p = head;
        do {
            sModBlockLink3228* next = p->next;
            p->vt[3].fn((char*)p + p->vt[3].delta);
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353278);
#ifdef SKIP_ASM
struct sModBlockVEntry3278 {
    short delta;
    short index;
    void* fn;
};

struct sModBlockLink3278 {
    sModBlockLink3278* next;      // 0x0
    int field_0x4;
    sModBlockVEntry3278* vt;      // 0x8
};

extern "C" void func_00353278(void* self)
{
    sModBlockLink3278* head = *(sModBlockLink3278**)((char*)self + 0x10);
    if (head != 0) {
        sModBlockLink3278* p = head;
        do {
            sModBlockLink3278* next = p->next;
            if (((int (*)(void*))p->vt[2].fn)((char*)p + p->vt[2].delta) == 3) {
                if (p != 0) {
                    ((void (*)(void*, int))p->vt[1].fn)((char*)p + p->vt[1].delta, 3);
                }
            }
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353300);
#ifdef SKIP_ASM
struct sModBlockDtorVEntry3300 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sModBlockLink3300 {
    sModBlockLink3300* next;          // 0x0
    int field_0x4;
    sModBlockDtorVEntry3300* vt;      // 0x8
    char pad_0xC[0x30 - 0xC];
    unsigned int id;                       // 0x30
};

extern "C" void func_00353300(void* self, unsigned int id)
{
    sModBlockLink3300* head = *(sModBlockLink3300**)((char*)self + 0x1C);
    if (head == 0) return;
    sModBlockLink3300* p = head;
    if (~id) {
    loop:
        if (p == 0) return;
        if (p->id == id) {
            p->vt[1].fn((char*)p + p->vt[1].delta, 3);
            return;
        }
        p = p->next;
        goto loop;
    } else {
        do {
            sModBlockLink3300* next = p->next;
            if (p != 0) {
                p->vt[1].fn((char*)p + p->vt[1].delta, 3);
            }
            p = next;
        } while (p != 0);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353398);
#ifdef SKIP_ASM
struct sMbVEntryV3398 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_00345720(void* pool, void* item);

extern "C" void func_00353398(void* self, void* list)
{
    void* p = *(void**)list;
    while (p != 0) {
        void* next = *(void**)p;
        sMbVEntryV3398* vt = *(sMbVEntryV3398**)((char*)p + 0x8);
        vt[3].fn((char*)p + vt[3].delta);
        func_00345720((char*)self + 0x10, p);
        p = next;
    }
    *(void**)list = 0;
    *(void**)((char*)list + 0x4) = 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_findRail);
#ifdef SKIP_ASM
struct sModifierLink {
    sModifierLink* next; // 0x0
    char pad_0x4[0x30 - 0x4];
    int id;              // 0x30
};

/**
 * Tells whether the modifier block has a rail modifier for a given spline.
 *
 * Walks the rail list (Rail, +0x1C) comparing each rail's SplineId (+0x30).
 *
 * @param id  the spline's resource id (On Tour tResourceID splineId)
 * @return 1 if a rail with that spline id is found, otherwise 0
 */
extern "C" int tModifierBlock_findRail(void* self, int id)
{
    sModifierLink* rail = *(sModifierLink**)((char*)self + 0x1c);
    while (rail != 0) {
        if (rail->id == id) {
            return 1;
        }
        rail = rail->next;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cTexFlipModifier_writeToReplayFrame(void* self, void* stream);
extern "C" void cUVScrollModifier_writeToReplayFrame(void* self, void* stream);

struct sVE_353448 { short delta; short index; int (*fn)(void*); };
struct sVEv_353448 { short delta; short index; void (*fn)(void*, void*); };
struct sVEw_353448 { short delta; short index; void (*fn)(void*, void*, int); };

struct sHdr_353448 {
    int flags;
    int a;
    int b;
    int count1;
    int count2;
    char ids[32];
};


static inline int VCall0_353448(char* o, int voff, int slot)
{
    sVE_353448* e = &(*(sVE_353448**)(o + voff))[slot];
    return e->fn(o + e->delta);
}

static inline void VCall1_353448(char* o, int voff, int slot, void* arg)
{
    sVEv_353448* e = &(*(sVEv_353448**)(o + voff))[slot];
    e->fn(o + e->delta, arg);
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
extern "C" void tModifierBlock_writeToReplayFrame(char* self, char* stream)
{
    sHdr_353448 info;
    info.flags = 0;
    info.b = 0;
    info.a = 0;
    info.count1 = 0;
    info.count2 = 0;
    for (int i = 0; i < 32; i++) {
        info.ids[i] = 0;
    }
    char* transformModifier = *(char**)(self + 0x0);
    if (transformModifier != 0) {
        info.flags |= 1;
        info.a = VCall0_353448(transformModifier, 0, 26);
    }
    if (*(void**)(self + 0x4) != 0) {
        info.flags |= 2;
    }
    if (*(void**)(self + 0x8) != 0) {
        info.flags |= 4;
    }
    char* oC = *(char**)(self + 0xC);
    if (oC != 0) {
        info.flags |= 8;
        info.b = VCall0_353448(oC, 4, 5);
    }
    for (char* link = *(char**)(self + 0x10); link; link = *(char**)link) {
        info.ids[info.count1++] = VCall0_353448(link, 8, 2);
    }
    if (info.count1 > 0) {
        info.flags |= 0x10;
    }
    for (char* link = *(char**)(self + 0x1C); link; link = *(char**)link) {
        info.count2++;
    }
    if (info.count2 > 0) {
        info.flags |= 0x20;
    }
    {
        sVEw_353448* writeEntry = &(*(sVEw_353448**)stream)[1];
        writeEntry->fn(stream + writeEntry->delta, &info, 0x34);
    }
    char* w0 = *(char**)(self + 0x0);
    if (w0 != 0) {
        VCall1_353448(w0, 0, 27, stream);
    }
    if (*(void**)(self + 0x4) != 0) {
        cUVScrollModifier_writeToReplayFrame(*(void**)(self + 0x4), stream);
    }
    if (*(void**)(self + 0x8) != 0) {
        cTexFlipModifier_writeToReplayFrame(*(void**)(self + 0x8), stream);
    }
    char* wC = *(char**)(self + 0xC);
    if (wC != 0) {
        VCall1_353448(wC, 4, 6, stream);
    }
    if (info.count1 > 0) {
        for (char* link = *(char**)(self + 0x10); link; link = *(char**)link) {
            VCall1_353448(link, 8, 4, stream);
        }
    }
    if (info.count2 > 0) {
        for (char* link = *(char**)(self + 0x1C); link; link = *(char**)link) {
            VCall1_353448(link, 8, 2, stream);
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", tModifierBlock_readFromReplayFrame);
#ifdef SKIP_ASM
struct sMbStreamVEntry36D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMbStream36D8 {
    sMbStreamVEntry36D8* vt;
};

struct sMbReplayHdr36D8 {
    unsigned int flags;      // 0x00
    int type;                // 0x04
    int kind;                // 0x08
    int numA;                // 0x0C
    int numB;                // 0x10
    signed char typesA[32];  // 0x14
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cSplineModifier_cSplineModifier(void* self, sMbStream36D8* s);
extern "C" void* cMultiSplineModifier_cMultiSplineModifier1(void* self, sMbStream36D8* s);
extern "C" void* cParentModifier_cParentModifier(void* self, sMbStream36D8* s);
extern "C" void* func_00356F68(void* self, sMbStream36D8* s);
extern "C" void* func_0035DF70(void* self, sMbStream36D8* s);
extern "C" void* func_00357358(void* self, sMbStream36D8* s);
extern "C" void* func_00357798(void* self, sMbStream36D8* s);
extern "C" void* func_0035F788(void* self, sMbStream36D8* s);
extern "C" void* func_0035F1A8(void* self, sMbStream36D8* s);
extern "C" void* func_00342808(void* self, sMbStream36D8* s);
extern "C" void* func_003459A8(void* self, sMbStream36D8* s);
extern "C" void* func_00345D80(void* self, sMbStream36D8* s);
extern "C" void* func_003461C0(void* self, sMbStream36D8* s);
extern "C" void* cRailModifier_cRailModifier1(void* self, sMbStream36D8* s);
extern "C" void func_00345720(void* pool, void* item);
extern "C" void cRailLink_append(void* list, void* item);

extern const char D_0048E8D0[];
extern const char D_0048E8E0[];
extern const char D_0048E8F0[];
extern const char D_0048E908[];
extern const char D_0048E918[];
extern const char D_0048E928[];
extern const char D_0048E940[];
extern const char D_0048E958[];
extern const char D_0048E968[];
extern const char D_0048E978[];
extern const char D_0048E990[];
extern const char D_0048E9A8[];
extern const char D_0048E9B8[];

/**
 * Rebuilds a modifier block from a replay frame: reads the header and re-creates every modifier it lists.
 *
 * The root transform is built by type: 1 spline, 2 multi-spline, 3 position, 4 parent, 5 roller, 6 magnet, 7
 * AvaSpline modifier; UV scroll, tex flip and (physics type 1) boost modifiers follow their flags. Effects
 * are rebuilt by type (1 particle, 2 dynamic particle, 3 halo) and appended to Effect (+0x10), rails to Rail
 * (+0x1C). An unknown effect type appends a null pointer, which func_00345720 writes through.
 *
 * @param stream  replay frame to read from (On Tour cBxReplayFrame*)
 */
extern "C" void tModifierBlock_readFromReplayFrame(tModifierBlock* self, sMbStream36D8* stream)
{
    sMbReplayHdr36D8 hdr;
    int i;

    *(void**)&self->node = 0;
    self->field_0x4 = 0;
    self->field_0x8 = 0;
    self->field_0xC = 0;
    stream->vt[2].fn((char*)stream + stream->vt[2].delta, &hdr, 0x34);

    if (hdr.flags & 1) {
        if (hdr.type == 1) {
            *(void**)&self->node = cSplineModifier_cSplineModifier(cMemMan_alloc(0xF0, D_0048E8D0, 0x20000000, 0), stream);
        } else if (hdr.type == 2) {
            *(void**)&self->node = cMultiSplineModifier_cMultiSplineModifier1(cMemMan_alloc(0x58, D_0048E8D0, 0x20000000, 0), stream);
        } else if (hdr.type == 4) {
            *(void**)&self->node = cParentModifier_cParentModifier(cMemMan_alloc(0xB0, D_0048E8E0, 0x20000000, 0), stream);
        } else if (hdr.type == 3) {
            *(void**)&self->node = func_00356F68(cMemMan_alloc(0x90, D_0048E8F0, 0x20000000, 0), stream);
        } else if (hdr.type == 5) {
            *(void**)&self->node = func_0035DF70(cMemMan_alloc(0x2D0, D_0048E908, 0x20000000, 0), stream);
        } else if (hdr.type == 6) {
            *(void**)&self->node = func_00357358(cMemMan_alloc(0xB0, D_0048E918, 0x20000000, 0), stream);
        } else if (hdr.type == 7) {
            *(void**)&self->node = func_00357798(cMemMan_alloc(0x90, D_0048E928, 0x20000000, 0), stream);
        }
    }
    if (hdr.flags & 2) {
        self->field_0x4 = (int)func_0035F788(cMemMan_alloc(0x50, D_0048E940, 0x20000000, 0), stream);
    }
    if (hdr.flags & 4) {
        self->field_0x8 = (int)func_0035F1A8(cMemMan_alloc(0xE8, D_0048E958, 0x20000000, 0), stream);
    }
    if ((hdr.flags & 8) && hdr.kind == 1) {
        self->field_0xC = (int)func_00342808(cMemMan_alloc(0x50, D_0048E968, 0x20000000, 0), stream);
    }
    for (i = 0; i < hdr.numA; i++) {
        void* p;
        if (hdr.typesA[i] == 1) {
            p = func_003459A8(cMemMan_alloc(0x1F0, D_0048E978, 0x20000000, 0), stream);
        } else if (hdr.typesA[i] == 2) {
            p = func_00345D80(cMemMan_alloc(0x270, D_0048E990, 0x20000000, 0), stream);
        } else if (hdr.typesA[i] == 3) {
            p = func_003461C0(cMemMan_alloc(0x40, D_0048E9A8, 0x20000000, 0), stream);
        } else {
            p = 0;
        }
        func_00345720((char*)self + 0x10, p);
    }
    for (i = 0; i < hdr.numB; i++) {
        cRailLink_append((char*)self + 0x1C, cRailModifier_cRailModifier1(cMemMan_alloc(0x90, D_0048E9B8, 0x20000000, 0), stream));
    }
}
#endif

extern void* cSkyBox_vtbl[];

//100%
INCLUDE_ASM("object/modifierblock", func_00353AC0__FPv);
#ifdef SKIP_ASM
void* func_00353AC0(void* self)
{
    *(void***)((char*)self + 0xc) = cSkyBox_vtbl;
    *(int*)((char*)self + 0x10) = -1;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x14) = 0;
    return self;
}
#endif

extern "C" void* cObjNode__cObjNode(void*);

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox__cSkyBox__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the sky box: restores cSkyBox's vtable, then runs the cObjNode destructor.
 *
 * Only @p self is declared here; the destructor flags the caller passed in the second argument register reach
 * cObjNode__cObjNode() unchanged. That removes the node from its render bucket and frees the object when bit 0
 * is set.
 *
 * @return cObjNode__cObjNode()'s result (it returns nothing meaningful)
 */
void* cSkyBox__cSkyBox(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cSkyBox_vtbl;
    return cObjNode__cObjNode(self);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSkyBox_render);
#ifdef SKIP_ASM
// PORT: func_002D1C58 is defined returning void*, but its value is the rider index passed to func_002D1C20(int).
extern "C" int func_002D1C58_i() __asm__("func_002D1C58");
extern "C" void* func_002D1C20(int i);
extern int D_004A452C;
extern char t4mat_Identity[];

struct sMat_53B10 {
    float m[16];
} __attribute__((aligned(16)));

struct sVec4_53B10 {
    float x, y, z, w;
} __attribute__((aligned(16)));

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
static inline void vu0CopyMatrix_53B10(sMat_53B10* dst, void* src)
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
 * Skipped when the "Disable Sky Box" debug toggle (D_004A452C) is set or there is no sky instance (+0x14, On
 * Tour m_pInstanceSky). Pushes a copy of the draw state with the blend field (bits 5-9 of word 8) cleared,
 * zon 1, blendm 2, zmode 0 and alpha 0x14, sets bit 2 and clears bit 1 of the record's flags, and draws
 * through the graphics manager's slot 95 with an identity matrix moved to the current view's position
 * (func_002D1C20) and flags 0x5420. Then pops the state and writes the saved blend bits back.
 */
extern "C" void cSkyBox_render(void* self)
{
    if (D_004A452C != 0)
        return;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (*(void**)((char*)self + 0x14) == 0)
        return;
    sMat_53B10 m;
    vu0CopyMatrix_53B10(&m, t4mat_Identity);
    sVec4_53B10 pos = *(sVec4_53B10*)func_002D1C20(func_002D1C58_i());
    *(sVec4_53B10*)&m.m[12] = pos;
    ((sRS_53B10*)ctx->m_pCurrentDrawState)[1] = ((sRS_53B10*)ctx->m_pCurrentDrawState)[0];
    sRS_53B10* drawState = (sRS_53B10*)ctx->m_pCurrentDrawState;
    *(sRS_53B10**)&ctx->m_pCurrentDrawState = drawState + 1;
    int f8 = drawState[1].f8;
    drawState[1].f8 = f8 & ~0x3E0;
    int old = (unsigned int)(f8 & 0x3E0) >> 5;
    ((sRS_53B10*)ctx->m_pCurrentDrawState)->f0b = 3;
    SetZOn_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 1);
    SetBlendM_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 2);
    SetZMode_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 0);
    SetAlpha_53B10(((sRS_53B10*)ctx->m_pCurrentDrawState), 0x14);
    char* skyInstance = *(char**)((char*)self + 0x14);
    if (skyInstance != 0)
    {
        *(unsigned int*)(skyInstance + 8) = (*(unsigned int*)(skyInstance + 8) & ~2u) | 4;
        char* instToDraw = *(char**)((char*)self + 0x14);
        sVE_53B10* vt = (sVE_53B10*)ctx->mVtbl;
        vt[95].fn((char*)ctx + vt[95].delta, instToDraw, 1.0f, *(void**)(*(char**)(instToDraw + 0x80) + 0xC), &m, 0, 0, 0x5420);
    }
    sRS_53B10* t2 = (sRS_53B10*)ctx->m_pCurrentDrawState;
    *(sRS_53B10**)&ctx->m_pCurrentDrawState = t2 - 1;
    int* pf = (int*)((char*)t2 - 0xC);
    *pf = (*pf & ~0x3E0) | (old << 5);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353CF0);
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

extern "C" sMbWorld3CF0** func_002D1BD8();
int func_00353D98(void* self);

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

extern "C" void func_00353CF0(sMbTrackRef3CF0* self, int id)
{
    self->id = id;
    if (id < 0) {
        func_00353D98(self);
        return;
    }
    sMbWorld3CF0* w = *func_002D1BD8();
    if (w->table->entries[id].count > 0) {
        sMbModelRef3CF0 ref;
        ref.id = id & 0xFF;
        self->model = ref.get(w);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353D98__FPv);
#ifdef SKIP_ASM
int func_00353D98(void* self)
{
    int t0 = -1;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x10) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353DA8__FPv);
#ifdef SKIP_ASM
void* func_00353DA8(void* self)
{
    *(int*)((char*)self + 0x24) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", cSpring__cSpring);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);

/**
 * Destroys a spring: frees its node array (+0x24, On Tour Nodes) when it has one, then frees the object when
 * asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSpring__cSpring(void* self, int flags)
{
    void* p = *(void**)((char*)self + 0x24);
    if (p != 0) {
        cMemMan_free(p);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353E08);
#ifdef SKIP_ASM
extern "C" void cSpring_setupNodes(void* self);

struct sSpringDesc_3E08 {
    float f0, f4, f8, fC, f10, f14, f18;
    int count;      // 0x1C
};

struct sSpring_3E08 {
    int count;      // 0x0
    float f4, f8, fC, f10, f14, f18, f1C;
    float seg;      // 0x20
};

extern "C" void func_00353E08(void* self, sSpringDesc_3E08* d, float len)
{
    sSpring_3E08* s = (sSpring_3E08*)self;
    int n = d->count;
    s->count = n;
    s->f4 = d->f0;
    s->f8 = d->f4;
    s->fC = d->f8;
    s->f10 = d->fC;
    s->f14 = d->f10;
    s->f18 = d->f14 * 9.999999974752427e-07f;
    s->f1C = d->f18;
    s->seg = len / (float)(n - 1);
    cSpring_setupNodes(self);
}
#endif

//100%
INCLUDE_ASM("object/modifierblock", func_00353E80);
#ifdef SKIP_ASM
struct sMbVEntrySer3E80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cSpring_setupNodes(void* self);

extern "C" void* func_00353E80(void* self, void* stream)
{
    sMbVEntrySer3E80* e = &(*(sMbVEntrySer3E80**)stream)[2];
    e->fn((char*)stream + e->delta, self, 0x24);
    cSpring_setupNodes(self);
    e = &(*(sMbVEntrySer3E80**)stream)[2];
    e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x24), *(int*)self * 0x50);
    return self;
}
#endif

