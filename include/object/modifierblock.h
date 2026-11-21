#ifndef OBJECT_MODIFIERBLOCK_H
#define OBJECT_MODIFIERBLOCK_H

// tModifierBlock: the set of modifiers a placed instance (cMoveNode) carries: one root-transform modifier, the optional
// UV-scroll, texture-flip and physics modifiers, and two intrusive lists (effect modifiers, rail modifiers) whose heads
// are embedded by value. tModifierBlock_readFromReplayFrame allocates the modifiers its stream asks for; preUpdate /
// postUpdate / signal / applyForce forward to them. Methods in object/modifierblock.
//
// Size 0x28 [R] (cMoveNode's cMemMan_alloc(40, "ModifierBlock"); On Tour's is the same). A FLAT layout, like
// main/game.h: member names, order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access
// in the 28 methods falls on a member start or inside one of the two embedded links (tools/gen/self_offsets.py).
// Evidence tags:
//   [O] On Tour's member; [R] retail's constructor tModifierBlock_tModifierBlock; [U] how retail code uses it.

#include "layout.h"
#include "object/effectlink.h"
#include "object/raillink.h"

struct cTransformModifier;
struct cUVScrollModifier;
struct cTexFlipModifier;
struct cPhysicsModifier;

// A modifier block's replay record (On Tour struct tModifierBlockInfo, 0x34): which modifiers it has.
// [U] tModifierBlock_writeToReplayFrame fills it (flags, transform / physics type, effect and rail counts, one type
// byte per effect; view sHdr_353448) and readFromReplayFrame rebuilds the block from it (view sMbReplayHdr36D8).
struct tModifierBlockInfo {
    int Flags;                      // 0x00 [O]
    int TransformType;              // 0x04 [O]
    int PhysicsType;                // 0x08 [O]
    int NumEffects;                 // 0x0C [O]
    int NumRails;                   // 0x10 [O]
    char EffectType[32];            // 0x14 [O]
};

CHECK_OFFSET(tModifierBlockInfo, Flags, 0x00);
CHECK_OFFSET(tModifierBlockInfo, TransformType, 0x04);
CHECK_OFFSET(tModifierBlockInfo, PhysicsType, 0x08);
CHECK_OFFSET(tModifierBlockInfo, NumEffects, 0x0C);
CHECK_OFFSET(tModifierBlockInfo, NumRails, 0x10);
CHECK_OFFSET(tModifierBlockInfo, EffectType, 0x14);
CHECK_SIZE(tModifierBlockInfo, 0x34);

struct tModifierBlock {
    cTransformModifier* RootTransform; // 0x00 [O]; [R] ctor 0; [U] getBoundBox / getRadius / applyForce call through
                                       //      its vtable
    cUVScrollModifier* UVScroll;       // 0x04 [O]; [R] ctor 0; [U] preUpdate, signal, remUVScrollModifier
    cTexFlipModifier* TexFlip;         // 0x08 [O]; [R] ctor 0; [U] preUpdate, signal, remTexFlipModifier
    cPhysicsModifier* Physics;         // 0x0C [O]; [R] ctor 0; [U] preUpdate, signal; readFromReplayFrame puts a
                                       //      cBoostModifier here
    cEffectLink Effect;                // 0x10 [O]; [R] ctor: Next / Prev 0, mVtbl = cEffectLink_vtbl; [U] the effect
                                       //      modifier list's head (postUpdate, fadeEffectModifier, remEffectModifier)
    cRailLink Rail;                    // 0x1C [O]; [R] ctor: Next / Prev 0, mVtbl = cRailLink_vtbl; [U] the rail
                                       //      modifier list's head (findRail, remRailModifier)
};

CHECK_OFFSET(tModifierBlock, RootTransform, 0x00);
CHECK_OFFSET(tModifierBlock, UVScroll, 0x04);
CHECK_OFFSET(tModifierBlock, TexFlip, 0x08);
CHECK_OFFSET(tModifierBlock, Physics, 0x0C);
CHECK_OFFSET(tModifierBlock, Effect, 0x10);
CHECK_OFFSET(tModifierBlock, Rail, 0x1C);
CHECK_SIZE(tModifierBlock, 0x28);

#endif /* OBJECT_MODIFIERBLOCK_H */
