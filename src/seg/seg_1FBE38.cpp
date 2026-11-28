#include "common.h"
#include "be/beinterface.h"
#include "wscript/wscriptman.h"
#include "world/instancerecord.h"
#include "object/instancenode.h"
#include "world/worldcache.h"
#include "wscript/wscriptmission.h"
#include "wscript/wscriptcache.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/ssxapp.h"
#include "luno/lunovar.h"
#include "main/game.h"
#include "ai/ai.h"
#include "ai/rider.h"
#include "ai/player.h"
#include "ai/missionrider.h"
#include "main/sectionman.h"
#include "object/movenode.h"
#include "ai/riderhandler.h"
#include "main/gameviewman.h"
#include "wscript/wscriptprocess.h"
#include "ai/ridermetrix.h"
#include "object/animdeltanode.h"
#include "object/animcombonode.h"
#include "object/animteeternode.h"
#include "object/conveyornode.h"
#include "object/counternode.h"
#include "object/animobject.h"
#include "object/boostnode.h"
#include "object/deadfadenode.h"
#include "object/debouncenode.h"
#include "object/fencenode.h"
#include "object/rollermodifier.h"
#include "object/texflipmodifier.h"
#include "object/boostmodifier.h"
#include "object/splinemodifier.h"
#include "object/multisplinemodifier.h"
#include "object/floatrailnode.h"
#include "object/springrailnode.h"
#include "object/effectmodifier.h"

// The script functions' command and type arguments (names ours, from what each function does with the value; the On
// Tour parameter blocks (tExpireData, tRailData, ...) type them int). Evidence: [U] the function each is used in.
// tMakeDeadData::Type (makeDead): a cDeadNode, the node deleted and m_iFlags reset from the copy in their upper half,
// or a cRestoreNode.
enum eMakeDeadType { kMakeDead_Dead = 0, kMakeDead_ResetFlags = 1, kMakeDead_Restore = 3 };
// tExpireData::Type (ExpireFunc): the node's expire(runscript) argument ([O] cInstanceNode::expire(const bool
// runscript)).
enum eExpireType { kExpire_Silent = 0, kExpire_RunScript = 1 };
// tRailData::Cmd (RailFunc): cRailManNode_addRail's on flag.
enum eRailCmd { kRailCmd_Off = 0, kRailCmd_On = 1 };
// tNISData::Cmd (NISFunc): queue the NIS of a script hash or of a script category on list 0, or play list 0.
enum eNISCmd { kNISCmd_QueueScript = 0, kNISCmd_QueueCategory = 1, kNISCmd_Play = 2 };
// tSectionData::Cmd (SectionFunc): cSectionMan_load; nothing (1 is handled and ignored); cSectionMan_activate; game
// state kGFSM_UserSelect with startup flag 2 and a scripted DJ line (cSSXAudio_BeginScriptedDJLine), or with flag 0; a sky swap.
enum eSectionCmd {
    kSectionCmd_Load = 0,
    kSectionCmd_Ignored = 1,
    kSectionCmd_Activate = 2,
    kSectionCmd_UserSelectDJ = 3,
    kSectionCmd_UserSelect = 4,
    kSectionCmd_SkySwap = 5
};
// tRemoveModifierData::Type (RemoveModifierFunc): tModifierBlock_fadeEffectModifier or remHaloModifier.
enum eRemoveModifierType { kRemoveModifier_Effect = 0, kRemoveModifier_Halo = 1 };
// tRiderActionData::Type / tRiderMetricData::Cmd (RiderActionFunc, RiderMetricFunc): each names the eMissionQuery
// (ai/rider.h) it is mapped to.
enum eScriptRiderQuery {
    kRiderQuery_Ground = 0,
    kRiderQuery_Air = 1,
    kRiderQuery_Railslide = 2,
    kRiderQuery_Wipeout = 3,
    kRiderQuery_Reset = 4,
    kRiderQuery_GrabHeld = 5,
    kRiderQuery_TweakHeld = 6,
    kRiderQuery_UberHeld = 7,
    kRiderQuery_TotalStylePoints = 8,
    kRiderQuery_TotalPoints = 9,
    kRiderQuery_NosePress = 10,
    kRiderQuery_TailPress = 11,
    kRiderQuery_Boosting = 12,
    kRiderQuery_Stall = 13,
    kRiderQuery_OffAxis = 14,
    kRiderQuery_Inverted = 15,
    kRiderQuery_NumPerfectLanding = 16,
    kRiderQuery_NumLastSecondLanding = 17,
    kRiderQuery_Punching = 18
};
// tCrowd3dData::Cmd (Crowd3dFunc): func_00343C60 joins the instance's crowd block, cCrowdMan3d_removeMember leaves it.
enum eCrowd3dCmd { kCrowd3dCmd_Join = 0, kCrowd3dCmd_Leave = 1 };
// tCrowd2dData::Cmd (Crowd2dFunc): cCrowdMan2d_attachBoneToCrowdPose points a bone at the shared crowd record; 1 is handled and ignored;
// cCrowdMan2d_addQuad takes a crowd quad slot, cCrowdMan2d_removeQuad releases it.
enum eCrowd2dCmd {
    kCrowd2dCmd_Attach = 0,
    kCrowd2dCmd_Ignored = 1,
    kCrowd2dCmd_AddQuad = 2,
    kCrowd2dCmd_RemoveQuad = 3
};
// tIsIconEnabledData::Type (IsIconEnabledFunc): the custom race rule each tests, by the rule's name text
// (D_00441018: rule 10 kT_11Multipliers, bit 6; rule 12 kT_11PowerUps, bit 8; rule 11 kT_OVRCMNNoPtIcons, bit 7).
enum eIconType { kIconType_Multiplier = 0, kIconType_PowerUp = 1, kIconType_Points = 2 };

// The world script manager (cWScriptMan::TheWScriptMan): its constructor stores itself here.
extern cWScriptMan* cWScriptMan_TheWScriptMan;

// The instance record a world-cache data tag points at: m_bfuiMemLoc holds its address / 4 (On Tour's
// cWorld::getInstanceResource, inlined into these functions, decodes the same packed pointer).
static inline tInstanceRecord* instanceAtMemLoc(unsigned memLoc) {
    return (tInstanceRecord*)(memLoc << 2); // PORT: packed pointer in a 32-bit handle
}

// Stores one script argument in the parameter-block field it names (the argument loop of every script function
// below): as a float when the field type table says eLunoVar_Float and the argument is not already one (an int turned
// into a float), otherwise as given.
static inline void setArgField(void* field, const luno_cLunoCArg* arg, int fieldType) {
    if (arg->Arg.Type != fieldType && fieldType == eLunoVar_Float)
        *(float*)field = (float)arg->Arg.Data.Int;
    else
        *(int*)field = arg->Arg.Data.Int;
}
//100%
INCLUDE_ASM("seg/seg_1FBE38", checkActiveNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern char D_00489618_AE38[] __asm__("D_00489618");
extern char D_00489628_AE38[] __asm__("D_00489628");
extern char D_00489638_AE38[] __asm__("D_00489638");
extern char D_00489648_AE38[] __asm__("D_00489648");
extern "C" void cParticleNode_cParticleNode1_AE38(void*, int, void*, void*, void*) __asm__("cParticleNode_cParticleNode1");
extern "C" void cMeshAnimNode_cMeshAnimNode1_AE38(void*, int, void*, void*, tCollideInfo*, void*)
    __asm__("cMeshAnimNode_cMeshAnimNode1");
extern "C" void cDebounceNode_cDebounceNode1_AE38(void*, int, void*, void*, void*) __asm__("cDebounceNode_cDebounceNode1");
extern "C" void cDeadFadeNode_cDeadFadeNode_AE38(void*, int, void*, void*, void*) __asm__("cDeadFadeNode_cDeadFadeNode");
/**
 * Decides what to do with an object's existing node before a script attaches a node of the given type.
 *
 * Returns 1 (leave it) when the node is a dead-fade or another busy kind (ATYPE_FENCE, ATYPE_DEAD, ATYPE_DEADFADE),
 * or already of @p type (it is retriggered through slot 37). Particle (ATYPE_PARTICLE), mesh-anim (ATYPE_MESHANIM),
 * debounce (ATYPE_DEBOUNCE) and dead-fade (ATYPE_DEADFADE) requests replace it with a new node built from @p buf and
 * return -1; any other type deletes it and returns 0.
 *
 * @param node  the object's current node
 * @param type  the node type being requested
 * @param buf  the new node's parameter block
 * @return 1, -1 or 0 as above
 */
extern "C" int checkActiveNode(void *node, int type, void *buf) {
    cInstanceNode *n = (cInstanceNode*)node;
    if (n->isType(ATYPE_FENCE) || n->isType(ATYPE_DEAD) || n->isType(ATYPE_DEADFADE))
        return 1;
    if (n->isType(type)) {
        n->makeActive();
        return 1;
    }
    tInstanceRecord* inst = n->Instance;
    switch (type) {
    case ATYPE_PARTICLE:
        inst->m_pActiveNode = 0;
        cParticleNode_cParticleNode1_AE38(cMemMan_alloc(44, D_00489618_AE38, 0x20000000, 0), 1, inst, buf, n);
        return -1;
    case ATYPE_MESHANIM:
        inst->m_pActiveNode = 0;
        cMeshAnimNode_cMeshAnimNode1_AE38(cMemMan_alloc(1728, D_00489628_AE38, 0x20000000, 0), 1, inst, buf,
                                          cWScriptMan_TheWScriptMan->CollideInfo, n);
        return -1;
    case ATYPE_DEBOUNCE:
        inst->m_pActiveNode = 0;
        cDebounceNode_cDebounceNode1_AE38(cMemMan_alloc(52, D_00489638_AE38, 0x20000000, 0), 1, inst, buf, n);
        return -1;
    case ATYPE_DEADFADE:
        inst->m_pActiveNode = 0;
        cDeadFadeNode_cDeadFadeNode_AE38(cMemMan_alloc(0x3C, D_00489648_AE38, 0x20000000, 0), 1, inst, buf, n);
        return -1;
    }
    delete n;
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimDeltaFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_B060(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_B060(unsigned i) { if (i == 0) return 0; return refToPtr_B060(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimDeltaNode_cAnimDeltaNode1_B060(void*, int, void*, void*) __asm__("cAnimDeltaNode_cAnimDeltaNode1");
extern char D_00489658_B060[] __asm__("D_00489658");
extern t3Vector luno_gLunoNil_B060 __asm__("luno_gLunoNil");
extern tMakeAnimDeltaData sAnimDeltaDefaults __asm__("D_004FB390");
extern int D_004A55C0;
extern const int MakeAnimDeltaFunc_types[];
extern "C" void* MEM_copy_B060(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[4]): animates a world instance with a new AnimDelta node, unless its current node is
 * busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimDeltaData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible) and lets each argument
 * override the field its index names, turning an int into a float where the field type table MakeAnimDeltaFunc_types
 * says eLunoVar_Float. Defaults: InstId -1, Type 1, FromTime -1, ToTime -1, Speed 30, StartTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 2 = ATYPE_ANIMDELTA, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cAnimDeltaNode (cAnimDeltaNode_cAnimDeltaNode1, 0x70 bytes,
 * bucket 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeAnimDeltaFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55C0 == 0) {
        sAnimDeltaDefaults.InstId = 0xFFFFFFFF;
        sAnimDeltaDefaults.Type = 1;
        sAnimDeltaDefaults.Dir = 0;
        sAnimDeltaDefaults.FromTime = -1.0f;
        sAnimDeltaDefaults.ToTime = -1.0f;
        sAnimDeltaDefaults.Speed = 30.0f;
        sAnimDeltaDefaults.SpeedRange = 0.0f;
        sAnimDeltaDefaults.StartTime = -1.0f;
        sAnimDeltaDefaults.RandomStart = 0;
        sAnimDeltaDefaults.ForceVisible = 0;
        D_004A55C0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAnimDeltaDefaults.null = WScriptNullType(sAnimDeltaDefaults.null);
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B060(buf, &sAnimDeltaDefaults, sizeof(tMakeAnimDeltaData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeAnimDeltaFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_B060(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B060;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_ANIMDELTA, buf))
        return luno_gLunoNil_B060;
    cAnimDeltaNode_cAnimDeltaNode1_B060(cMemMan_alloc(0x70, D_00489658_B060, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B060;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimComboFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_B270(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_B270(unsigned i) { if (i == 0) return 0; return refToPtr_B270(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimComboNode_cAnimComboNode1_B270(void*, int, void*, void*) __asm__("cAnimComboNode_cAnimComboNode1");
extern char D_00489668_B270[] __asm__("D_00489668");
extern t3Vector luno_gLunoNil_B270 __asm__("luno_gLunoNil");
extern tMakeAnimComboData sAnimComboDefaults __asm__("D_004FB3C0");
extern int D_004A55C4;
extern const int MakeAnimComboFunc_types[];
extern "C" void* MEM_copy_B270(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[5]): plays a two-stage animation on a world instance with a new AnimCombo node,
 * unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 60-byte parameter block (On Tour tMakeAnimComboData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible, FromTime2, ToTime2,
 * Speed2, Stop) and lets each argument override the field its index names, turning an int into a float where the field
 * type table MakeAnimComboFunc_types says eLunoVar_Float. Defaults: InstId -1, Type 1, FromTime -1, ToTime 30, Speed
 * 30, StartTime -1, FromTime2 31, ToTime2 -1, Speed2 30.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 3 = ATYPE_ANIMCOMBO, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cAnimComboNode (cAnimComboNode_cAnimComboNode1, 136 bytes,
 * bucket 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeAnimComboFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55C4 == 0) {
        sAnimComboDefaults.InstId = 0xFFFFFFFF;
        sAnimComboDefaults.Type = 1;
        sAnimComboDefaults.Dir = 0;
        sAnimComboDefaults.FromTime = -1.0f;
        sAnimComboDefaults.ToTime = 30.0f;
        sAnimComboDefaults.Speed = 30.0f;
        sAnimComboDefaults.SpeedRange = 0.0f;
        sAnimComboDefaults.StartTime = -1.0f;
        sAnimComboDefaults.RandomStart = 0;
        sAnimComboDefaults.ForceVisible = 0;
        sAnimComboDefaults.FromTime2 = 31.0f;
        sAnimComboDefaults.ToTime2 = -1.0f;
        sAnimComboDefaults.Speed2 = 30.0f;
        sAnimComboDefaults.Stop = 0;
        D_004A55C4 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAnimComboDefaults.null = WScriptNullType(sAnimComboDefaults.null);
    }
    unsigned buf64[15];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B270(buf, &sAnimComboDefaults, sizeof(tMakeAnimComboData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeAnimComboFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_B270(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B270;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_ANIMCOMBO, buf))
        return luno_gLunoNil_B270;
    cAnimComboNode_cAnimComboNode1_B270(cMemMan_alloc(136, D_00489668_B270, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B270;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimTeeterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_B498(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_B498(unsigned i) { if (i == 0) return 0; return refToPtr_B498(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimTeeterNode_cAnimTeeterNode1_B498(void*, int, void*, void*) __asm__("cAnimTeeterNode_cAnimTeeterNode1");
extern char D_00489678_B498[] __asm__("D_00489678");
extern t3Vector luno_gLunoNil_B498 __asm__("luno_gLunoNil");
extern tMakeAnimTeeterData sAnimTeeterDefaults __asm__("D_004FB400");
extern int D_004A55C8;
extern const int MakeAnimTeeterFunc_types[];
extern "C" void* MEM_copy_B498(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[6]): makes a world instance teeter (a new AnimTeeter node), unless its current node
 * is busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimTeeterData:
 * InstId, TorqueScale, BounceFactor, MaxSpeed, Spring, PivotTime, Damp, AnimSign, FromTime, ToTime) and lets each
 * argument override the field its index names, turning an int into a float where the field type table
 * MakeAnimTeeterFunc_types says eLunoVar_Float. Defaults: InstId -1, TorqueScale 0.6, MaxSpeed 60, Spring 0.02,
 * PivotTime -10, Damp 0.6, FromTime -1, ToTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 4 = ATYPE_ANIMTEETER, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cAnimTeeterNode (cAnimTeeterNode_cAnimTeeterNode1, 0x80 bytes,
 * bucket 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeAnimTeeterFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55C8 == 0) {
        sAnimTeeterDefaults.InstId = 0xFFFFFFFF;
        sAnimTeeterDefaults.TorqueScale = 0.6000000238418579f;
        sAnimTeeterDefaults.BounceFactor = 0.0f;
        sAnimTeeterDefaults.MaxSpeed = 60.0f;
        sAnimTeeterDefaults.Spring = 0.019999999552965164f;
        sAnimTeeterDefaults.PivotTime = -10.0f;
        sAnimTeeterDefaults.Damp = 0.6000000238418579f;
        sAnimTeeterDefaults.AnimSign = 0;
        sAnimTeeterDefaults.FromTime = -1.0f;
        sAnimTeeterDefaults.ToTime = -1.0f;
        D_004A55C8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAnimTeeterDefaults.null = WScriptNullType(sAnimTeeterDefaults.null);
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B498(buf, &sAnimTeeterDefaults, sizeof(tMakeAnimTeeterData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeAnimTeeterFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_B498(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B498;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_ANIMTEETER, buf))
        return luno_gLunoNil_B498;
    cAnimTeeterNode_cAnimTeeterNode1_B498(cMemMan_alloc(0x80, D_00489678_B498, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B498;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeConveyorFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_B6B8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_B6B8(unsigned i) { if (i == 0) return 0; return refToPtr_B6B8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cConveyorNode_cConveyorNode_B6B8(void*, int, void*, void*) __asm__("cConveyorNode_cConveyorNode");
extern char D_00489688_B6B8[] __asm__("D_00489688");
extern t3Vector luno_gLunoNil_B6B8 __asm__("luno_gLunoNil");
extern tMakeConveyorData sConveyorDefaults __asm__("D_004FB430");
extern int D_004A55CC;
extern const int MakeConveyorFunc_types[];
extern "C" void* MEM_copy_B6B8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[8]): turns a world instance into a conveyor (a new Conveyor node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 52-byte parameter block (On Tour tMakeConveyorData:
 * InstId, Vel, UScroll, VScroll, Rotate0, Rotate1, Rotate2, Rotate3, Rotate4, Rotate5, Rotate6, Rotate7) and lets each
 * argument override the field its index names, turning an int into a float where the field type table
 * MakeConveyorFunc_types says eLunoVar_Float. Defaults: InstId -1, Vel 100.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 15 = ATYPE_CONVEYOR, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cConveyorNode (cConveyorNode_cConveyorNode, 72 bytes, bucket 1)
 * on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeConveyorFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55CC == 0) {
        sConveyorDefaults.InstId = 0xFFFFFFFF;
        sConveyorDefaults.Vel = 100.0f;
        float z = 0.0f;
        sConveyorDefaults.UScroll = z;
        sConveyorDefaults.VScroll = z;
        sConveyorDefaults.Rotate0 = 0;
        sConveyorDefaults.Rotate1 = 0;
        sConveyorDefaults.Rotate2 = 0;
        sConveyorDefaults.Rotate3 = 0;
        sConveyorDefaults.Rotate4 = 0;
        sConveyorDefaults.Rotate5 = 0;
        sConveyorDefaults.Rotate6 = 0;
        sConveyorDefaults.Rotate7 = 0;
        D_004A55CC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sConveyorDefaults.null = WScriptNullType(sConveyorDefaults.null);
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B6B8(buf, &sConveyorDefaults, sizeof(tMakeConveyorData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeConveyorFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_B6B8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B6B8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_CONVEYOR, buf))
        return luno_gLunoNil_B6B8;
    cConveyorNode_cConveyorNode_B6B8(cMemMan_alloc(72, D_00489688_B6B8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B6B8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeCounterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_B8D0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_B8D0(unsigned i) { if (i == 0) return 0; return refToPtr_B8D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cCounterNode_cCounterNode1_B8D0(void*, int, void*, void*) __asm__("cCounterNode_cCounterNode1");
extern char D_004A3BE8_B8D0[] __asm__("D_004A3BE8");
extern t3Vector luno_gLunoNil_B8D0 __asm__("luno_gLunoNil");
extern tMakeCounterData sCounterDefaults __asm__("D_004FB468");
extern int D_004A55D0;
extern int MakeCounterFunc_types[2];
extern "C" void* MEM_copy_B8D0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[9]): gives a world instance a hit counter (a new Counter node), unless its current
 * node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMakeCounterData:
 * InstId, StartCount) and lets each argument override the field its index names, turning an int into a float where the
 * field type table MakeCounterFunc_types says eLunoVar_Float. Defaults: InstId -1, StartCount 3.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 7 = ATYPE_COUNTER, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cCounterNode (cCounterNode_cCounterNode1, 36 bytes, bucket 1) on
 * the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeCounterFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55D0 == 0) {
        sCounterDefaults.InstId = 0xFFFFFFFF;
        sCounterDefaults.StartCount = 3;
        D_004A55D0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sCounterDefaults.null = WScriptNullType(sCounterDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B8D0(buf, &sCounterDefaults, sizeof(tMakeCounterData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeCounterFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_B8D0(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B8D0;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_COUNTER, buf))
        return luno_gLunoNil_B8D0;
    cCounterNode_cCounterNode1_B8D0(cMemMan_alloc(36, D_004A3BE8_B8D0, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B8D0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFloatingFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_BAB0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_BAB0(unsigned i) { if (i == 0) return 0; return refToPtr_BAB0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFloatingNode_cFloatingNode_BAB0(void*, int, void*, void*) __asm__("cFloatingNode_cFloatingNode");
extern char D_00489698_BAB0[] __asm__("D_00489698");
extern t3Vector luno_gLunoNil_BAB0 __asm__("luno_gLunoNil");
// World-script data that makes a floating node (On Tour tMakeFloatingData, 0x20; same offsets): the block below
// fills it, cFloatingNode_cFloatingNode reads it.
struct tMakeFloatingData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    float LSpring;                  // 0x04 [O]
    float LDamp;                    // 0x08 [O]
    float ASpring;                  // 0x0C [O]
    float ADamp;                    // 0x10 [O]
    float ForceScale;               // 0x14 [O]
    float TorqueScale;              // 0x18 [O]
    WScriptNullType null;           // 0x1C [O]
    char unk_01D[0x3];              // 0x1D padding
};

CHECK_OFFSET(tMakeFloatingData, InstId, 0x00);
CHECK_OFFSET(tMakeFloatingData, LSpring, 0x04);
CHECK_OFFSET(tMakeFloatingData, LDamp, 0x08);
CHECK_OFFSET(tMakeFloatingData, ASpring, 0x0C);
CHECK_OFFSET(tMakeFloatingData, ADamp, 0x10);
CHECK_OFFSET(tMakeFloatingData, ForceScale, 0x14);
CHECK_OFFSET(tMakeFloatingData, TorqueScale, 0x18);
CHECK_OFFSET(tMakeFloatingData, null, 0x1C);
CHECK_SIZE(tMakeFloatingData, 0x20);
extern tMakeFloatingData sFloatingDefaults __asm__("D_004FB478");
extern int D_004A55D4;
extern const int MakeFloatingFunc_types[];
extern "C" void* MEM_copy_BAB0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[14]): makes a world instance float on springs (a new Floating node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 32-byte parameter block (On Tour tMakeFloatingData:
 * InstId, LSpring, LDamp, ASpring, ADamp, ForceScale, TorqueScale) and lets each argument override the field its index
 * names, turning an int into a float where the field type table MakeFloatingFunc_types says eLunoVar_Float. Defaults:
 * InstId -1, LSpring 10, LDamp 10, ASpring 40, ADamp 10, ForceScale 0.2, TorqueScale 10.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 14 = ATYPE_FLOATING, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cFloatingNode (cFloatingNode_cFloatingNode, 0x7C bytes, bucket
 * 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeFloatingFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55D4 == 0) {
        sFloatingDefaults.InstId = 0xFFFFFFFF;
        sFloatingDefaults.LSpring = 10.0f;
        sFloatingDefaults.LDamp = 10.0f;
        sFloatingDefaults.ASpring = 40.0f;
        sFloatingDefaults.ADamp = 10.0f;
        sFloatingDefaults.ForceScale = 0.20000000298023224f;
        sFloatingDefaults.TorqueScale = 10.0f;
        D_004A55D4 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sFloatingDefaults.null = WScriptNullType(sFloatingDefaults.null);
    }
    unsigned buf64[8];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BAB0(buf, &sFloatingDefaults, sizeof(tMakeFloatingData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeFloatingFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_BAB0(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BAB0;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_FLOATING, buf))
        return luno_gLunoNil_BAB0;
    cFloatingNode_cFloatingNode_BAB0(cMemMan_alloc(0x7C, D_00489698_BAB0, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_BAB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_BCB8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_BCB8(unsigned i) { if (i == 0) return 0; return refToPtr_BCB8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimObjectNode_cAnimObjectNode1_BCB8(void*, int, int, void*, void*) __asm__("cAnimObjectNode_cAnimObjectNode1");
extern char D_004896A8_BCB8[] __asm__("D_004896A8");
extern t3Vector luno_gLunoNil_BCB8 __asm__("luno_gLunoNil");
extern tMakeAnimObjectData sAnimObjectDefaults __asm__("D_004FB498");
extern int D_004A55D8;
extern const int MakeAnimObjectFunc_types[];
extern "C" void* MEM_copy_BCB8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[3]): animates a world instance as an animated object (a new AnimObject node), unless
 * its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimObjectData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible) and lets each argument
 * override the field its index names, turning an int into a float where the field type table MakeAnimObjectFunc_types
 * says eLunoVar_Float. Defaults: InstId -1, Type 1, FromTime -1, ToTime -1, Speed 30, StartTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 1 = ATYPE_ANIMOBJECT, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cAnimObjectNode (cAnimObjectNode_cAnimObjectNode1, 108 bytes,
 * bucket 1, node type 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeAnimObjectFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55D8 == 0) {
        sAnimObjectDefaults.InstId = 0xFFFFFFFF;
        sAnimObjectDefaults.Type = 1;
        sAnimObjectDefaults.Dir = 0;
        sAnimObjectDefaults.FromTime = -1.0f;
        sAnimObjectDefaults.ToTime = -1.0f;
        sAnimObjectDefaults.Speed = 30.0f;
        sAnimObjectDefaults.SpeedRange = 0.0f;
        sAnimObjectDefaults.StartTime = -1.0f;
        sAnimObjectDefaults.RandomStart = 0;
        sAnimObjectDefaults.ForceVisible = 0;
        D_004A55D8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAnimObjectDefaults.null = WScriptNullType(sAnimObjectDefaults.null);
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BCB8(buf, &sAnimObjectDefaults, sizeof(tMakeAnimObjectData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeAnimObjectFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_BCB8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BCB8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_ANIMOBJECT, buf))
        return luno_gLunoNil_BCB8;
    cAnimObjectNode_cAnimObjectNode1_BCB8(cMemMan_alloc(108, D_004896A8_BCB8, 0x20000000, 0), 1, 1, obj, buf);
    return luno_gLunoNil_BCB8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeBoostFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_BEC8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_BEC8(unsigned i) { if (i == 0) return 0; return refToPtr_BEC8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cBoostNode_cBoostNode1_BEC8(void*, int, int, void*, void*) __asm__("cBoostNode_cBoostNode1");
extern char D_004A3BF0_BEC8[] __asm__("D_004A3BF0");
extern t3Vector luno_gLunoNil_BEC8 __asm__("luno_gLunoNil");
extern tMakeBoostData sBoostDefaults __asm__("D_004FB4C8");
extern int D_004A55DC;
extern const int MakeBoostFunc_types[];
extern "C" void* MEM_copy_BEC8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[7]): turns a world instance into a boost object (a new Boost node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 36-byte parameter block (On Tour tMakeBoostData: InstId,
 * Type, BoostTime, Drag, MaxSpeed, BoostX, BoostY, BoostZ) and lets each argument override the field its index names,
 * turning an int into a float where the field type table MakeBoostFunc_types says eLunoVar_Float. Defaults: InstId -1,
 * BoostTime 1, MaxSpeed 1, BoostY 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 8 = ATYPE_BOOST, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cBoostNode (cBoostNode_cBoostNode1, 80 bytes, bucket 1, node type
 * 8) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeBoostFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55DC == 0) {
        sBoostDefaults.InstId = 0xFFFFFFFF;
        sBoostDefaults.Type = 0;
        sBoostDefaults.BoostTime = 1.0f;
        float z = 0.0f;
        sBoostDefaults.Drag = z;
        sBoostDefaults.MaxSpeed = 1.0f;
        sBoostDefaults.BoostX = z;
        sBoostDefaults.BoostY = 1.0f;
        sBoostDefaults.BoostZ = z;
        D_004A55DC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sBoostDefaults.null = WScriptNullType(sBoostDefaults.null);
    }
    unsigned buf64[9];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BEC8(buf, &sBoostDefaults, sizeof(tMakeBoostData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeBoostFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_BEC8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BEC8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_BOOST, buf))
        return luno_gLunoNil_BEC8;
    cBoostNode_cBoostNode1_BEC8(cMemMan_alloc(80, D_004A3BF0_BEC8, 0x20000000, 0), 1, 8, obj, buf);
    return luno_gLunoNil_BEC8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_C0D0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_C0D0(unsigned i) { if (i == 0) return 0; return refToPtr_C0D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cObjectNode_cObjectNode_C0D0(void*, int, void*, int, bool) __asm__("cObjectNode_cObjectNode");
extern char D_004A3BF8_C0D0[] __asm__("D_004A3BF8");
extern t3Vector luno_gLunoNil_C0D0 __asm__("luno_gLunoNil");
// World-script data that makes an object node (On Tour tMakeObjectData, 0x10; same offsets): the block below fills
// it and passes Solid and ForceVisible to cObjectNode_cObjectNode.
struct tMakeObjectData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Solid;                      // 0x04 [O]
    int ForceVisible;               // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tMakeObjectData, InstId, 0x00);
CHECK_OFFSET(tMakeObjectData, Solid, 0x04);
CHECK_OFFSET(tMakeObjectData, ForceVisible, 0x08);
CHECK_OFFSET(tMakeObjectData, null, 0x0C);
CHECK_SIZE(tMakeObjectData, 0x10);
extern tMakeObjectData sObjectDefaults __asm__("D_004FB4F0");
extern int D_004A55E0;
extern const int MakeObjectFunc_types[];
extern "C" void* MEM_copy_C0D0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[0]): gives a world instance a plain object node (a new Object node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMakeObjectData:
 * InstId, Solid, ForceVisible) and lets each argument override the field its index names, turning an int into a float
 * where the field type table MakeObjectFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 17 = ATYPE_OBJECT, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cObjectNode (cObjectNode_cObjectNode, 44 bytes, bucket 1) with
 * Solid and ForceVisible != 0 as its arguments on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeObjectFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55E0 == 0) {
        sObjectDefaults.InstId = 0xFFFFFFFF;
        sObjectDefaults.Solid = 0;
        sObjectDefaults.ForceVisible = 0;
        D_004A55E0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sObjectDefaults.null = WScriptNullType(sObjectDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C0D0(buf, &sObjectDefaults, sizeof(tMakeObjectData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeObjectFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_C0D0(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C0D0;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_OBJECT, buf))
        return luno_gLunoNil_C0D0;
    cObjectNode_cObjectNode_C0D0(cMemMan_alloc(44, D_004A3BF8_C0D0, 0x20000000, 0), 1, obj, buf[1], buf[2] != 0);
    return luno_gLunoNil_C0D0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", makeDead);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cDeadNode_cDeadNode(void*, void*);
extern "C" void cRestoreNode_cRestoreNode(void*, void*);
extern char str_DeadNode[];
extern char str_RestoreNode[];

/**
 * Applies a dead/restore request to a world instance by the request's Type.
 *
 * @p data is the request (On Tour tMakeDeadData: InstId, Type). Type kMakeDead_Dead replaces the instance's node
 * (m_pActiveNode) with a new cDeadNode (cMemMan_alloc 0x1C, "DeadNode"); kMakeDead_Restore replaces it with a new
 * cRestoreNode (0x1C, "RestoreNode"); kMakeDead_ResetFlags deletes the node and rewrites the instance flags (m_iFlags)
 * as (f & 0xFFFF0300) | ((f & 0xFFFF0300) >> 16) | 2. An existing node is deleted through its virtual destructor
 * first, except that a dead node (ATYPE_DEAD) is never touched (and a restore node, ATYPE_RESTORE, is kept for
 * kMakeDead_Restore), which ends the request. Other types do nothing.
 *
 * MakeDeadFunc calls it for the script, cObjectInterface_makeDead for cDeadFadeNode when its fade ends.
 *
 * @param data  the request: its Type picks what happens
 */
extern "C" void makeDead(tInstanceRecord* self, const tMakeDeadData* data) {
    switch (data->Type) {
    case kMakeDead_ResetFlags: {
        cInstanceNode* curNode = self->m_pActiveNode;
        if (curNode) {
            if (curNode->ObjNodeType == ATYPE_DEAD)
                break;
            delete curNode;
        }
        int maskedFlags = self->m_iFlags & 0xFFFF0300;
        self->m_iFlags = maskedFlags | (maskedFlags >> 16) | 2;
        break;
    }
    case kMakeDead_Dead: {
        cInstanceNode* curNode = self->m_pActiveNode;
        if (curNode) {
            if (curNode->ObjNodeType == ATYPE_DEAD)
                break;
            delete curNode;
        }
        cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode, 0x20000000, 0), self);
        break;
    }
    case kMakeDead_Restore: {
        cInstanceNode* curNode = self->m_pActiveNode;
        if (curNode) {
            short ty = curNode->ObjNodeType;
            if (ty == ATYPE_DEAD || ty == ATYPE_RESTORE)
                break;
            delete curNode;
        }
        cRestoreNode_cRestoreNode(cMemMan_alloc(0x1C, str_RestoreNode, 0x20000000, 0), self);
        break;
    }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1FBE38", MakeDeadFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_C420(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_C420(unsigned i) { if (i == 0) return 0; return refToPtr_C420(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void makeDead_C420(void*, void*) __asm__("makeDead");
extern t3Vector luno_gLunoNil_C420 __asm__("luno_gLunoNil");
extern tMakeDeadData sDeadDefaults __asm__("D_004FB500");
extern int MakeDeadFunc_initFlag;
extern int MakeDeadFunc_types[2];
extern "C" void* MEM_copy_C420(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[2]): makes a world instance dead or restores it, unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMakeDeadData: InstId,
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table MakeDeadFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found. When it has no
 * active node (m_pActiveNode), or checkActiveNode(node, 6 = ATYPE_DEAD, block) lets the node go, the instance and block
 * go to makeDead, which acts by Type (0 dead node, 1 delete the node and flag the instance, 3 restore node).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeDeadFunc(int n, luno_cLunoCArg *params) {
    if (MakeDeadFunc_initFlag == 0) {
        sDeadDefaults.InstId = 0xFFFFFFFF;
        sDeadDefaults.Type = 0;
        MakeDeadFunc_initFlag = 1;
        sDeadDefaults.null = WScriptNullType(sDeadDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C420(buf, &sDeadDefaults, sizeof(tMakeDeadData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeDeadFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_C420(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C420;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (!activeNode || !checkActiveNode(activeNode, ATYPE_DEAD, buf))
        makeDead_C420(obj, buf);
    return luno_gLunoNil_C420;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeDeadFadeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_C5D8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_C5D8(unsigned i) { if (i == 0) return 0; return refToPtr_C5D8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cDeadFadeNode_cDeadFadeNode_C5D8(void*, int, void*, void*, int) __asm__("cDeadFadeNode_cDeadFadeNode");
extern char D_004896D8_C5D8[] __asm__("D_004896D8");
extern t3Vector luno_gLunoNil_C5D8 __asm__("luno_gLunoNil");
extern tMakeDeadFadeData sDeadFadeDefaults __asm__("D_004FB510");
extern int D_004A55E8;
extern const int MakeDeadFadeFunc_types[];
extern "C" void* MEM_copy_C5D8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: makes a world instance fade out and die (a new "DeadFade" node), unless it is already
 * busy.
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. Nothing happens when the instance is
 * not found.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector MakeDeadFadeFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55E8 == 0) {
        sDeadFadeDefaults.InstId = 0xFFFFFFFF;
        sDeadFadeDefaults.Type = 0;
        sDeadFadeDefaults.FadeTime = 2.0f;
        sDeadFadeDefaults.ZVel = 25.0f;
        D_004A55E8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDeadFadeDefaults.null = WScriptNullType(sDeadFadeDefaults.null);
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C5D8(buf, &sDeadFadeDefaults, sizeof(tMakeDeadFadeData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeDeadFadeFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_C5D8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C5D8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_DEADFADE, buf))
        return luno_gLunoNil_C5D8;
    cDeadFadeNode_cDeadFadeNode_C5D8(cMemMan_alloc(0x3C, D_004896D8_C5D8, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_C5D8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeDebounceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_C7D0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_C7D0(unsigned i) { if (i == 0) return 0; return refToPtr_C7D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cDebounceNode_cDebounceNode1_C7D0(void*, int, void*, void*, int) __asm__("cDebounceNode_cDebounceNode1");
extern char D_004896E8_C7D0[] __asm__("D_004896E8");
extern t3Vector luno_gLunoNil_C7D0 __asm__("luno_gLunoNil");
extern tMakeDebounceData sDebounceDefaults __asm__("D_004FB528");
extern int D_004A55EC;
extern const int MakeDebounceFunc_types[];
extern "C" void* MEM_copy_C7D0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script command MakeDebounce: turns an instance into a debounce node.
 *
 * Copies the static defaults (D_004FB528, set once under D_004A55EC: instance -1, 1.0, 1, -1) and applies each keyword
 * argument by index, converting an int to float where the keyword table MakeDebounceFunc_types says eLunoVar_Float. The
 * instance ID -1 means cWScriptMan_TheWScriptMan's current instance (Inst); otherwise it is looked up in
 * cWorld_m_pTheWorld's world cache (section = low byte, entry = the rest). Nothing happens if there is no instance, or
 * if it has an active node (m_pActiveNode) and checkActiveNode(node, 0, data) says it is already one; otherwise
 * cDebounceNode_cDebounceNode1 constructs a cDebounceNode (bucket 1) in cMemMan_alloc(0x34, "Debounce").
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" t3Vector MakeDebounceFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55EC == 0) {
        sDebounceDefaults.InstId = 0xFFFFFFFF;
        sDebounceDefaults.Time = 1.0f;
        sDebounceDefaults.Type = 1;
        sDebounceDefaults.Visible = -1;
        D_004A55EC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDebounceDefaults.null = WScriptNullType(sDebounceDefaults.null);
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C7D0(buf, &sDebounceDefaults, sizeof(tMakeDebounceData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeDebounceFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_C7D0(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C7D0;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_DEBOUNCE, buf))
        return luno_gLunoNil_C7D0;
    cDebounceNode_cDebounceNode1_C7D0(cMemMan_alloc(52, D_004896E8_C7D0, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_C7D0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFlagFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_C9C8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_C9C8(unsigned i) { if (i == 0) return 0; return refToPtr_C9C8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFlagNode_cFlagNode1_C9C8(void*, int, void*, void*) __asm__("cFlagNode_cFlagNode1");
extern char D_004A3C08_C9C8[] __asm__("D_004A3C08");
extern t3Vector luno_gLunoNil_C9C8 __asm__("luno_gLunoNil");
// World-script data that makes a flag node (On Tour tMakeFlagData, 0x6C; same offsets): the block below fills it,
// cFlagNode_cFlagNode reads it.
struct tMakeFlagData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int BaseSideHeld;               // 0x04 [O]
    int OppositeSideHeld;           // 0x08 [O]
    int PerpSideHeld;               // 0x0C [O]
    float Speed1;                   // 0x10 [O]
    float Magnitude1;               // 0x14 [O]
    float Frequency1;               // 0x18 [O]
    float Speed2;                   // 0x1C [O]
    float Magnitude2;               // 0x20 [O]
    float Frequency2;               // 0x24 [O]
    float Speed3;                   // 0x28 [O]
    float Magnitude3;               // 0x2C [O]
    float Frequency3;               // 0x30 [O]
    float Speed4;                   // 0x34 [O]
    float Magnitude4;               // 0x38 [O]
    float Frequency4;               // 0x3C [O]
    float LowWindX;                 // 0x40 [O]
    float LowWindY;                 // 0x44 [O]
    float LowWindZ;                 // 0x48 [O]
    float HighWindX;                // 0x4C [O]
    float HighWindY;                // 0x50 [O]
    float HighWindZ;                // 0x54 [O]
    float MinWindFactor;            // 0x58 [O]
    float UScroll;                  // 0x5C [O]
    float VScroll;                  // 0x60 [O]
    int OverSaturate;               // 0x64 [O]
    WScriptNullType null;           // 0x68 [O]
    char unk_069[0x3];              // 0x69 padding
};

CHECK_OFFSET(tMakeFlagData, InstId, 0x00);
CHECK_OFFSET(tMakeFlagData, BaseSideHeld, 0x04);
CHECK_OFFSET(tMakeFlagData, OppositeSideHeld, 0x08);
CHECK_OFFSET(tMakeFlagData, PerpSideHeld, 0x0C);
CHECK_OFFSET(tMakeFlagData, Speed1, 0x10);
CHECK_OFFSET(tMakeFlagData, Magnitude1, 0x14);
CHECK_OFFSET(tMakeFlagData, Frequency1, 0x18);
CHECK_OFFSET(tMakeFlagData, Speed2, 0x1C);
CHECK_OFFSET(tMakeFlagData, Magnitude2, 0x20);
CHECK_OFFSET(tMakeFlagData, Frequency2, 0x24);
CHECK_OFFSET(tMakeFlagData, Speed3, 0x28);
CHECK_OFFSET(tMakeFlagData, Magnitude3, 0x2C);
CHECK_OFFSET(tMakeFlagData, Frequency3, 0x30);
CHECK_OFFSET(tMakeFlagData, Speed4, 0x34);
CHECK_OFFSET(tMakeFlagData, Magnitude4, 0x38);
CHECK_OFFSET(tMakeFlagData, Frequency4, 0x3C);
CHECK_OFFSET(tMakeFlagData, LowWindX, 0x40);
CHECK_OFFSET(tMakeFlagData, LowWindY, 0x44);
CHECK_OFFSET(tMakeFlagData, LowWindZ, 0x48);
CHECK_OFFSET(tMakeFlagData, HighWindX, 0x4C);
CHECK_OFFSET(tMakeFlagData, HighWindY, 0x50);
CHECK_OFFSET(tMakeFlagData, HighWindZ, 0x54);
CHECK_OFFSET(tMakeFlagData, MinWindFactor, 0x58);
CHECK_OFFSET(tMakeFlagData, UScroll, 0x5C);
CHECK_OFFSET(tMakeFlagData, VScroll, 0x60);
CHECK_OFFSET(tMakeFlagData, OverSaturate, 0x64);
CHECK_OFFSET(tMakeFlagData, null, 0x68);
CHECK_SIZE(tMakeFlagData, 0x6C);
extern tMakeFlagData sFlagDefaults __asm__("D_004FB540");
extern int D_004A55F0;
extern const int MakeFlagFunc_types[];
extern "C" void* MEM_copy_C9C8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[12]): makes a world instance wave like a flag (a new Flag node), unless its current
 * node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 108-byte parameter block (On Tour tMakeFlagData: InstId,
 * BaseSideHeld, OppositeSideHeld, PerpSideHeld, Speed1, Magnitude1, Frequency1, Speed2, Magnitude2, Frequency2, Speed3,
 * Magnitude3, Frequency3, Speed4, Magnitude4, Frequency4, LowWindX, LowWindY, LowWindZ, HighWindX, HighWindY,
 * HighWindZ, MinWindFactor, UScroll, VScroll, OverSaturate) and lets each argument override the field its index names,
 * turning an int into a float where the field type table MakeFlagFunc_types says eLunoVar_Float. Defaults: InstId -1,
 * BaseSideHeld 1, Speed1 1, Magnitude1 25, Frequency1 1, Speed2 1, Frequency2 1, Speed3 1, Frequency3 1, Speed4 1,
 * Frequency4 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 10 = ATYPE_FLAG, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cFlagNode (cFlagNode_cFlagNode1, 0x1C bytes, bucket 1) on the
 * instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeFlagFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55F0 == 0) {
        sFlagDefaults.InstId = 0xFFFFFFFF;
        sFlagDefaults.BaseSideHeld = 1;
        sFlagDefaults.OppositeSideHeld = 0;
        sFlagDefaults.PerpSideHeld = 0;
        sFlagDefaults.Speed1 = 1.0f;
        sFlagDefaults.Magnitude1 = 25.0f;
        sFlagDefaults.Frequency1 = 1.0f;
        sFlagDefaults.Speed2 = 1.0f;
        float z = 0.0f;
        sFlagDefaults.Magnitude2 = z;
        sFlagDefaults.Frequency2 = 1.0f;
        sFlagDefaults.Speed3 = 1.0f;
        sFlagDefaults.Magnitude3 = z;
        sFlagDefaults.Frequency3 = 1.0f;
        sFlagDefaults.Speed4 = 1.0f;
        sFlagDefaults.Magnitude4 = z;
        sFlagDefaults.Frequency4 = 1.0f;
        sFlagDefaults.LowWindX = z;
        sFlagDefaults.LowWindY = z;
        sFlagDefaults.LowWindZ = z;
        sFlagDefaults.HighWindX = z;
        sFlagDefaults.HighWindY = z;
        sFlagDefaults.HighWindZ = z;
        sFlagDefaults.MinWindFactor = z;
        sFlagDefaults.UScroll = z;
        sFlagDefaults.VScroll = z;
        sFlagDefaults.OverSaturate = 0;
        D_004A55F0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sFlagDefaults.null = WScriptNullType(sFlagDefaults.null);
    }
    unsigned buf64[27];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C9C8(buf, &sFlagDefaults, sizeof(tMakeFlagData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeFlagFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_C9C8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C9C8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_FLAG, buf))
        return luno_gLunoNil_C9C8;
    cFlagNode_cFlagNode1_C9C8(cMemMan_alloc(0x1C, D_004A3C08_C9C8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_C9C8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFenceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_CC20 __asm__("luno_gLunoNil");
extern tMakeFenceData sFenceDefaults __asm__("D_004FB5B0");
extern int D_004A55F4;
extern const int D_00446028[];
extern "C" void* MEM_copy_CC20(void*, void*, int) __asm__("MEM_copy");
extern "C" int checkActiveNode(void*, int, void*);
#define ASSERT_CC20(c) do { } while (0)

/**
 * Script function (gLunoCFunctions[10]): settles a world instance's current node as a fence request; builds no node.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMakeFenceData: InstId,
 * Type, Flexibility) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_00446028 says eLunoVar_Float. Defaults: InstId -1, Flexibility 0.5.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance exists and has an active node (m_pActiveNode),
 * it only calls checkActiveNode(node, 9 = ATYPE_FENCE, block) and ignores the result: a busy or fence node is kept (a
 * fence is retriggered) and any other node is deleted. Unlike its siblings it never builds a fence node. The nil value
 * is written through the hidden result pointer @p ret.
 *
 * @param ret  where the returned script value is stored
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
// PORT: struct return written with the hidden return pointer explicit.
extern "C" t3Vector* MakeFenceFunc(t3Vector* ret, int n, luno_cLunoCArg *params) {
    if (D_004A55F4 == 0) {
        sFenceDefaults.InstId = 0xFFFFFFFF;
        sFenceDefaults.Flexibility = 0.5f;
        sFenceDefaults.Type = 0;
        D_004A55F4 = 1;
        sFenceDefaults.null = WScriptNullType(sFenceDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_CC20(buf, &sFenceDefaults, sizeof(tMakeFenceData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446028[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        cInstanceNode* node = obj->m_pActiveNode;
        if (node)
            checkActiveNode(node, ATYPE_FENCE, buf);
    }
    // compiled-out debug macros around the return; they shape the register allocation here
    do {
        *ret = luno_gLunoNil_CC20;
        ASSERT_CC20(ret);
        return ret;
    } while (0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFlexBridgeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_CDC8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_CDC8(unsigned i) { if (i == 0) return 0; return refToPtr_CDC8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFlexBridgeNode_cFlexBridgeNode_CDC8(void*, int, void*, void*) __asm__("cFlexBridgeNode_cFlexBridgeNode");
extern char D_004896F8_CDC8[] __asm__("D_004896F8");
extern t3Vector luno_gLunoNil_CDC8 __asm__("luno_gLunoNil");
// World-script data that makes a flex bridge node (On Tour tMakeFlexBridgeData; same offsets): the block below fills
// it.
struct tMakeFlexBridgeData {
    float LSpring;                  // 0x00 [O]
    float LDamp;                    // 0x04 [O]
    float ASpring;                  // 0x08 [O]
    float ADamp;                    // 0x0C [O]
    float ForceScale;               // 0x10 [O]
    float TorqueScale;              // 0x14 [O]
    float LengthScale;              // 0x18 [O]
    int Width;                      // 0x1C [O]
    unsigned int InstId;            // 0x20 [O] tResourceID
    WScriptNullType null;           // 0x24 [O]
    char unk_025[0x3];              // 0x25 padding
};

CHECK_OFFSET(tMakeFlexBridgeData, LSpring, 0x00);
CHECK_OFFSET(tMakeFlexBridgeData, LDamp, 0x04);
CHECK_OFFSET(tMakeFlexBridgeData, ASpring, 0x08);
CHECK_OFFSET(tMakeFlexBridgeData, ADamp, 0x0C);
CHECK_OFFSET(tMakeFlexBridgeData, ForceScale, 0x10);
CHECK_OFFSET(tMakeFlexBridgeData, TorqueScale, 0x14);
CHECK_OFFSET(tMakeFlexBridgeData, LengthScale, 0x18);
CHECK_OFFSET(tMakeFlexBridgeData, Width, 0x1C);
CHECK_OFFSET(tMakeFlexBridgeData, InstId, 0x20);
CHECK_OFFSET(tMakeFlexBridgeData, null, 0x24);
CHECK_SIZE(tMakeFlexBridgeData, 0x28);
extern tMakeFlexBridgeData sFlexBridgeDefaults __asm__("D_004FB5C0");
extern int D_004A55F8;
extern const int MakeFlexBridgeFunc_types[];
extern "C" void* MEM_copy_CDC8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: turns a world instance into a flexing bridge (a new "FlexBridge" node), unless it is
 * already busy.
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. Nothing happens when the instance is
 * not found.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector MakeFlexBridgeFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55F8 == 0) {
        sFlexBridgeDefaults.LSpring = 500.0f;
        sFlexBridgeDefaults.LDamp = 1.0f;
        sFlexBridgeDefaults.ASpring = 2400.0f;
        sFlexBridgeDefaults.ADamp = 0.10000000149011612f;
        sFlexBridgeDefaults.ForceScale = 0.10000000149011612f;
        sFlexBridgeDefaults.TorqueScale = 10.0f;
        sFlexBridgeDefaults.LengthScale = 0.0f;
        sFlexBridgeDefaults.Width = 16;
        sFlexBridgeDefaults.InstId = 0xFFFFFFFF;
        D_004A55F8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sFlexBridgeDefaults.null = WScriptNullType(sFlexBridgeDefaults.null);
    }
    unsigned buf64[10];
    unsigned *buf = buf64;
    buf64[8] = 0xFFFFFFFF;
    MEM_copy_CDC8(buf, &sFlexBridgeDefaults, sizeof(tMakeFlexBridgeData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeFlexBridgeFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[8];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_CDC8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_CDC8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_FLEXBRIDGE, buf))
        return luno_gLunoNil_CDC8;
    cFlexBridgeNode_cFlexBridgeNode_CDC8(cMemMan_alloc(176, D_004896F8_CDC8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_CDC8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeMeshAnimFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_CFF0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_CFF0(unsigned i) { if (i == 0) return 0; return refToPtr_CFF0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cMeshAnimNode_cMeshAnimNode1_CFF0(void*, int, void*, void*, tCollideInfo*, int)
    __asm__("cMeshAnimNode_cMeshAnimNode1");
extern char D_00489708_CFF0[] __asm__("D_00489708");
extern t3Vector luno_gLunoNil_CFF0 __asm__("luno_gLunoNil");
// World-script data that makes a mesh-anim node (On Tour tMakeMeshAnimData; same offsets): the block below fills it.
struct tMakeMeshAnimData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Type;                       // 0x04 [O]
    float Drag;                     // 0x08 [O]
    float VelScale;                 // 0x0C [O]
    float Life;                     // 0x10 [O]
    float DisperseX;                // 0x14 [O]
    float DisperseY;                // 0x18 [O]
    float DisperseZ;                // 0x1C [O]
    float VelX;                     // 0x20 [O]
    float VelY;                     // 0x24 [O]
    float VelZ;                     // 0x28 [O]
    float AngVel;                   // 0x2C [O]
    float FadeTime;                 // 0x30 [O]
    unsigned int RootInstId;        // 0x34 [O] tResourceID
    int RootMesh;                   // 0x38 [O]
    WScriptNullType null;           // 0x3C [O]
    char unk_03D[0x3];              // 0x3D padding
};

CHECK_OFFSET(tMakeMeshAnimData, InstId, 0x00);
CHECK_OFFSET(tMakeMeshAnimData, Type, 0x04);
CHECK_OFFSET(tMakeMeshAnimData, Drag, 0x08);
CHECK_OFFSET(tMakeMeshAnimData, VelScale, 0x0C);
CHECK_OFFSET(tMakeMeshAnimData, Life, 0x10);
CHECK_OFFSET(tMakeMeshAnimData, DisperseX, 0x14);
CHECK_OFFSET(tMakeMeshAnimData, DisperseY, 0x18);
CHECK_OFFSET(tMakeMeshAnimData, DisperseZ, 0x1C);
CHECK_OFFSET(tMakeMeshAnimData, VelX, 0x20);
CHECK_OFFSET(tMakeMeshAnimData, VelY, 0x24);
CHECK_OFFSET(tMakeMeshAnimData, VelZ, 0x28);
CHECK_OFFSET(tMakeMeshAnimData, AngVel, 0x2C);
CHECK_OFFSET(tMakeMeshAnimData, FadeTime, 0x30);
CHECK_OFFSET(tMakeMeshAnimData, RootInstId, 0x34);
CHECK_OFFSET(tMakeMeshAnimData, RootMesh, 0x38);
CHECK_OFFSET(tMakeMeshAnimData, null, 0x3C);
CHECK_SIZE(tMakeMeshAnimData, 0x40);
extern tMakeMeshAnimData sMeshAnimDefaults __asm__("D_004FB5E8");
extern int D_004A55FC;
extern const int MakeMeshAnimFunc_types[];
extern "C" void* MEM_copy_CFF0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script command MakeMeshAnim: turns an instance into an animated-mesh node.
 *
 * Copies the static 0x40-byte defaults (D_004FB5E8, set once under D_004A55FC: instance -1, 0, 0.1, 1.0, 2.0, 600, 600,
 * 300, 0, 0, 0, 360, -1, -1, -1) and applies the keyword arguments (types from MakeMeshAnimFunc_types; an int becomes a
 * float where the type is eLunoVar_Float). The instance is resolved as in MakeDebounceFunc (-1 = the script's current
 * instance). Unless it is missing or checkActiveNode(node, 11, data) finds it already animated,
 * cMeshAnimNode_cMeshAnimNode1 constructs a cMeshAnimNode (bucket 1, with cWScriptMan_TheWScriptMan->CollideInfo) in
 * cMemMan_alloc(0x6C0, "MeshAnim").
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" t3Vector MakeMeshAnimFunc(int n, luno_cLunoCArg *params) {
    if (D_004A55FC == 0) {
        sMeshAnimDefaults.InstId = 0xFFFFFFFF;
        sMeshAnimDefaults.Type = 0;
        sMeshAnimDefaults.Drag = 0.10000000149011612f;
        sMeshAnimDefaults.VelScale = 1.0f;
        sMeshAnimDefaults.Life = 2.0f;
        sMeshAnimDefaults.DisperseX = 600.0f;
        sMeshAnimDefaults.DisperseY = 600.0f;
        sMeshAnimDefaults.DisperseZ = 300.0f;
        float z = 0.0f;
        sMeshAnimDefaults.VelX = z;
        sMeshAnimDefaults.VelY = z;
        sMeshAnimDefaults.VelZ = z;
        sMeshAnimDefaults.AngVel = 360.0f;
        sMeshAnimDefaults.FadeTime = -1.0f;
        sMeshAnimDefaults.RootInstId = 0xFFFFFFFF;
        sMeshAnimDefaults.RootMesh = -1;
        D_004A55FC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMeshAnimDefaults.null = WScriptNullType(sMeshAnimDefaults.null);
    }
    unsigned buf64[16];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[13] = 0xFFFFFFFF;
    MEM_copy_CFF0(buf, &sMeshAnimDefaults, sizeof(tMakeMeshAnimData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeMeshAnimFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_CFF0(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_CFF0;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_MESHANIM, buf))
        return luno_gLunoNil_CFF0;
    cMeshAnimNode_cMeshAnimNode1_CFF0(cMemMan_alloc(1728, D_00489708_CFF0, 0x20000000, 0), 1, obj, buf,
                                      cWScriptMan_TheWScriptMan->CollideInfo, 0);
    return luno_gLunoNil_CFF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddRollerFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addRoller_D250(void*, void*, tIntersectInfo*, tCollideInfo*) __asm__("cMoveNode_addRoller");
extern t3Vector luno_gLunoNil_D250 __asm__("luno_gLunoNil");
extern tAddRollerData sRollerDefaults __asm__("D_004FB628");
extern int D_004A5600;
extern const int D_004460A0[];
extern "C" void* MEM_copy_D250(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: adds a roller modifier to a moving world instance, with the script's current collision.
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. An instance ID that is not found is
 * not checked and is dereferenced as null.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector AddRollerFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5600 == 0) {
        sRollerDefaults.InstId = 0xFFFFFFFF;
        sRollerDefaults.Mass = 1.0f;
        sRollerDefaults.Elast = 0.699999988079071f;
        sRollerDefaults.Friction = 0.5f;
        D_004A5600 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sRollerDefaults.null = WScriptNullType(sRollerDefaults.null);
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_D250(buf, &sRollerDefaults, sizeof(tAddRollerData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_004460A0[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode())
            cMoveNode_addRoller_D250(activeNode, buf, cWScriptMan_TheWScriptMan->IntersectInfo,
                                     cWScriptMan_TheWScriptMan->CollideInfo);
    }
    return luno_gLunoNil_D250;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_D420(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_D420(unsigned i) { if (i == 0) return 0; return refToPtr_D420(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cParticleNode_cParticleNode1_D420(void*, int, void*, void*, int) __asm__("cParticleNode_cParticleNode1");
void operator_delete(int*);
extern char D_00489718_D420[] __asm__("D_00489718");
extern char str_ParticleNode_D420[] __asm__("str_ParticleNode");
extern t3Vector luno_gLunoNil_D420 __asm__("luno_gLunoNil");
// World-script data that makes a particle node (On Tour tMakeParticleData; same offsets): the block below fills it.
struct tMakeParticleData {
    int NumParticles;               // 0x00 [O]
    int NumBlur;                    // 0x04 [O]
    float Duration;                 // 0x08 [O]
    float Damp;                     // 0x0C [O]
    float Size;                     // 0x10 [O]
    float Life;                     // 0x14 [O]
    float SizeR;                    // 0x18 [O]
    float LifeR;                    // 0x1C [O]
    float BlurStep;                 // 0x20 [O]
    float OffX;                     // 0x24 [O]
    float OffY;                     // 0x28 [O]
    float OffZ;                     // 0x2C [O]
    float R0X;                      // 0x30 [O]
    float R0Y;                      // 0x34 [O]
    float R0Z;                      // 0x38 [O]
    float R1X;                      // 0x3C [O]
    float R1Y;                      // 0x40 [O]
    float R1Z;                      // 0x44 [O]
    float VelX;                     // 0x48 [O]
    float VelY;                     // 0x4C [O]
    float VelZ;                     // 0x50 [O]
    float R0VX;                     // 0x54 [O]
    float R0VY;                     // 0x58 [O]
    float R0VZ;                     // 0x5C [O]
    float R1VX;                     // 0x60 [O]
    float R1VY;                     // 0x64 [O]
    float R1VZ;                     // 0x68 [O]
    float R2VX;                     // 0x6C [O]
    float R2VY;                     // 0x70 [O]
    float R2VZ;                     // 0x74 [O]
    float ForceX;                   // 0x78 [O]
    float ForceY;                   // 0x7C [O]
    float ForceZ;                   // 0x80 [O]
    float StartColA;                // 0x84 [O]
    float StartColR;                // 0x88 [O]
    float StartColG;                // 0x8C [O]
    float StartColB;                // 0x90 [O]
    float EndColA;                  // 0x94 [O]
    float EndColR;                  // 0x98 [O]
    float EndColG;                  // 0x9C [O]
    float EndColB;                  // 0xA0 [O]
    float R0A;                      // 0xA4 [O]
    float R0R;                      // 0xA8 [O]
    float R0G;                      // 0xAC [O]
    float R0B;                      // 0xB0 [O]
    float R1A;                      // 0xB4 [O]
    float R1R;                      // 0xB8 [O]
    float R1G;                      // 0xBC [O]
    float R1B;                      // 0xC0 [O]
    int TextureId;                  // 0xC4 [O]
    int BlendMode;                  // 0xC8 [O]
    float SizeFinal;                // 0xCC [O]
    int NumFlipTextures;            // 0xD0 [O]
    float FlipTextureRate;          // 0xD4 [O]
    unsigned int InstId;            // 0xD8 [O] tResourceID
    WScriptNullType null;           // 0xDC [O]
    char unk_0DD[0x3];              // 0xDD padding
};

CHECK_OFFSET(tMakeParticleData, NumParticles, 0x00);
CHECK_OFFSET(tMakeParticleData, NumBlur, 0x04);
CHECK_OFFSET(tMakeParticleData, Duration, 0x08);
CHECK_OFFSET(tMakeParticleData, Damp, 0x0C);
CHECK_OFFSET(tMakeParticleData, Size, 0x10);
CHECK_OFFSET(tMakeParticleData, Life, 0x14);
CHECK_OFFSET(tMakeParticleData, SizeR, 0x18);
CHECK_OFFSET(tMakeParticleData, LifeR, 0x1C);
CHECK_OFFSET(tMakeParticleData, BlurStep, 0x20);
CHECK_OFFSET(tMakeParticleData, OffX, 0x24);
CHECK_OFFSET(tMakeParticleData, OffY, 0x28);
CHECK_OFFSET(tMakeParticleData, OffZ, 0x2C);
CHECK_OFFSET(tMakeParticleData, R0X, 0x30);
CHECK_OFFSET(tMakeParticleData, R0Y, 0x34);
CHECK_OFFSET(tMakeParticleData, R0Z, 0x38);
CHECK_OFFSET(tMakeParticleData, R1X, 0x3C);
CHECK_OFFSET(tMakeParticleData, R1Y, 0x40);
CHECK_OFFSET(tMakeParticleData, R1Z, 0x44);
CHECK_OFFSET(tMakeParticleData, VelX, 0x48);
CHECK_OFFSET(tMakeParticleData, VelY, 0x4C);
CHECK_OFFSET(tMakeParticleData, VelZ, 0x50);
CHECK_OFFSET(tMakeParticleData, R0VX, 0x54);
CHECK_OFFSET(tMakeParticleData, R0VY, 0x58);
CHECK_OFFSET(tMakeParticleData, R0VZ, 0x5C);
CHECK_OFFSET(tMakeParticleData, R1VX, 0x60);
CHECK_OFFSET(tMakeParticleData, R1VY, 0x64);
CHECK_OFFSET(tMakeParticleData, R1VZ, 0x68);
CHECK_OFFSET(tMakeParticleData, R2VX, 0x6C);
CHECK_OFFSET(tMakeParticleData, R2VY, 0x70);
CHECK_OFFSET(tMakeParticleData, R2VZ, 0x74);
CHECK_OFFSET(tMakeParticleData, ForceX, 0x78);
CHECK_OFFSET(tMakeParticleData, ForceY, 0x7C);
CHECK_OFFSET(tMakeParticleData, ForceZ, 0x80);
CHECK_OFFSET(tMakeParticleData, StartColA, 0x84);
CHECK_OFFSET(tMakeParticleData, StartColR, 0x88);
CHECK_OFFSET(tMakeParticleData, StartColG, 0x8C);
CHECK_OFFSET(tMakeParticleData, StartColB, 0x90);
CHECK_OFFSET(tMakeParticleData, EndColA, 0x94);
CHECK_OFFSET(tMakeParticleData, EndColR, 0x98);
CHECK_OFFSET(tMakeParticleData, EndColG, 0x9C);
CHECK_OFFSET(tMakeParticleData, EndColB, 0xA0);
CHECK_OFFSET(tMakeParticleData, R0A, 0xA4);
CHECK_OFFSET(tMakeParticleData, R0R, 0xA8);
CHECK_OFFSET(tMakeParticleData, R0G, 0xAC);
CHECK_OFFSET(tMakeParticleData, R0B, 0xB0);
CHECK_OFFSET(tMakeParticleData, R1A, 0xB4);
CHECK_OFFSET(tMakeParticleData, R1R, 0xB8);
CHECK_OFFSET(tMakeParticleData, R1G, 0xBC);
CHECK_OFFSET(tMakeParticleData, R1B, 0xC0);
CHECK_OFFSET(tMakeParticleData, TextureId, 0xC4);
CHECK_OFFSET(tMakeParticleData, BlendMode, 0xC8);
CHECK_OFFSET(tMakeParticleData, SizeFinal, 0xCC);
CHECK_OFFSET(tMakeParticleData, NumFlipTextures, 0xD0);
CHECK_OFFSET(tMakeParticleData, FlipTextureRate, 0xD4);
CHECK_OFFSET(tMakeParticleData, InstId, 0xD8);
CHECK_OFFSET(tMakeParticleData, null, 0xDC);
CHECK_SIZE(tMakeParticleData, 0xE0);
extern tMakeParticleData sParticleDefaults __asm__("D_004FB640");
extern int D_004A5604;
extern const int D_004460B0[];
extern "C" void* MEM_copy_D420(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[16]): attaches a particle effect to a world instance (a new Particle node), unless
 * its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 224-byte parameter block (On Tour tMakeParticleData:
 * NumParticles, NumBlur, Duration, Damp, Size, Life, SizeR, LifeR, BlurStep, OffX, OffY, OffZ, R0X, R0Y, R0Z, R1X, R1Y,
 * R1Z, VelX, VelY, VelZ, R0VX, R0VY, R0VZ, R1VX, R1VY, R1VZ, R2VX, R2VY, R2VZ, ForceX, ForceY, ForceZ, StartColA,
 * StartColR, StartColG, StartColB, EndColA, EndColR, EndColG, EndColB, R0A, R0R, R0G, R0B, R1A, R1R, R1G, R1B,
 * TextureId, BlendMode, SizeFinal, NumFlipTextures, FlipTextureRate, InstId) and lets each argument override the field
 * its index names, turning an int into a float where the field type table D_004460B0 says eLunoVar_Float. Defaults:
 * NumParticles 1, Duration -1, Damp 1, Size 4, StartColA 1, EndColA 1, EndColR 1, EndColG 1, EndColB 1, R0A 1, R1A 1,
 * TextureId 16, NumFlipTextures 1, FlipTextureRate 20, InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The block is heap-allocated (cMemMan_alloc 0xE0) because the node
 * keeps it; InstId is its last field (+0xD8). It is freed when the instance is not found or checkActiveNode(node, 13 =
 * ATYPE_PARTICLE, block) keeps the old node (a positive result); a negative result means checkActiveNode already built
 * the replacement from it. Otherwise LifeR is capped at Life and a cParticleNode (cParticleNode_cParticleNode1, 44
 * bytes, bucket 1, no old node) is built on the instance with the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeParticleFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5604 == 0) {
        sParticleDefaults.NumParticles = 1;
        sParticleDefaults.NumBlur = 0;
        sParticleDefaults.Duration = -1.0f;
        sParticleDefaults.Damp = 1.0f;
        sParticleDefaults.Size = 4.0f;
        float z = 0.0f;
        sParticleDefaults.Life = z;
        sParticleDefaults.SizeR = z;
        sParticleDefaults.LifeR = z;
        sParticleDefaults.BlurStep = z;
        sParticleDefaults.OffX = z;
        sParticleDefaults.OffY = z;
        sParticleDefaults.OffZ = z;
        sParticleDefaults.R0X = z;
        sParticleDefaults.R0Y = z;
        sParticleDefaults.R0Z = z;
        sParticleDefaults.R1X = z;
        sParticleDefaults.R1Y = z;
        sParticleDefaults.R1Z = z;
        sParticleDefaults.VelX = z;
        sParticleDefaults.VelY = z;
        sParticleDefaults.VelZ = z;
        sParticleDefaults.R0VX = z;
        sParticleDefaults.R0VY = z;
        sParticleDefaults.R0VZ = z;
        sParticleDefaults.R1VX = z;
        sParticleDefaults.R1VY = z;
        sParticleDefaults.R1VZ = z;
        sParticleDefaults.R2VX = z;
        sParticleDefaults.R2VY = z;
        sParticleDefaults.R2VZ = z;
        sParticleDefaults.ForceX = z;
        sParticleDefaults.ForceY = z;
        sParticleDefaults.ForceZ = z;
        sParticleDefaults.StartColA = 1.0f;
        sParticleDefaults.StartColR = z;
        sParticleDefaults.StartColG = z;
        sParticleDefaults.StartColB = z;
        sParticleDefaults.EndColA = 1.0f;
        sParticleDefaults.EndColR = 1.0f;
        sParticleDefaults.EndColG = 1.0f;
        sParticleDefaults.EndColB = 1.0f;
        sParticleDefaults.R0A = 1.0f;
        sParticleDefaults.R0R = z;
        sParticleDefaults.R0G = z;
        sParticleDefaults.R0B = z;
        sParticleDefaults.R1A = 1.0f;
        sParticleDefaults.R1R = z;
        sParticleDefaults.R1G = z;
        sParticleDefaults.R1B = z;
        sParticleDefaults.TextureId = 16;
        sParticleDefaults.BlendMode = 0;
        sParticleDefaults.SizeFinal = z;
        sParticleDefaults.NumFlipTextures = 1;
        sParticleDefaults.FlipTextureRate = 20.0f;
        sParticleDefaults.InstId = 0xFFFFFFFF;
        D_004A5604 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sParticleDefaults.null = WScriptNullType(sParticleDefaults.null);
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xE0, D_00489718_D420, 0x20000000, 0);
    tMakeParticleData *data = (tMakeParticleData*)buf;
    data->InstId = 0xFFFFFFFF;
    MEM_copy_D420(buf, &sParticleDefaults, sizeof(tMakeParticleData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = D_004460B0[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *instance;
        if (!section)
            instance = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_D420(i);
            instance = found;
        }
        obj = instance;
    }
    if (obj == 0) {
        operator_delete((int*)buf);
        return luno_gLunoNil_D420;
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        int checkResult = checkActiveNode(activeNode, ATYPE_PARTICLE, buf);
        if (checkResult != 0) {
            if (checkResult > 0)
                operator_delete((int*)buf);
            return luno_gLunoNil_D420;
        }
    }
    if (data->LifeR > data->Life)
        data->LifeR = data->Life;
    cParticleNode_cParticleNode1_D420(cMemMan_alloc(44, str_ParticleNode_D420, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_D420;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddRiderPositionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V4_D758 { float x, y, z, w; V4_D758() {} V4_D758(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} } __attribute__((aligned(16)));
extern "C" void cMoveNode_addPosition_D758(void*, V4_D758*) __asm__("cMoveNode_addPosition");
// Rider k as a cBeing (the null-checked upcast of SSXApp->Game->AI->Riders[k]).
static inline cBeing *shape_D758(int k) {
    return SSXApp->Game->AI->Riders[k];
}
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline V4_D758 mtxApply_D758(t4Matrix *m, const V4_D758 &v) {
    V4_D758 out;
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
        : "m"(v), "r"(m)
        : "memory");
    return out;
}
// PORT: PS2-only VU0 inline asm (vector add in place).
static inline void vAddTo_D758(V4_D758 &a, const V4_D758 &b) {
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}
extern t3Vector luno_gLunoNil_D758 __asm__("luno_gLunoNil");
// World-script data that adds a rider-position modifier (On Tour tAddRiderPositionData; same offsets): the block below
// fills it.
struct tAddRiderPositionData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Being;                      // 0x04 [O]
    float OffsetX;                  // 0x08 [O]
    float OffsetY;                  // 0x0C [O]
    float OffsetZ;                  // 0x10 [O]
    WScriptNullType null;           // 0x14 [O]
    char unk_015[0x3];              // 0x15 padding
};

CHECK_OFFSET(tAddRiderPositionData, InstId, 0x00);
CHECK_OFFSET(tAddRiderPositionData, Being, 0x04);
CHECK_OFFSET(tAddRiderPositionData, OffsetX, 0x08);
CHECK_OFFSET(tAddRiderPositionData, OffsetY, 0x0C);
CHECK_OFFSET(tAddRiderPositionData, OffsetZ, 0x10);
CHECK_OFFSET(tAddRiderPositionData, null, 0x14);
CHECK_SIZE(tAddRiderPositionData, 0x18);
extern tAddRiderPositionData sRiderPositionDefaults __asm__("D_004FB720");
extern int D_004A5608;
extern const int D_00446190[];
extern "C" void* MEM_copy_D758(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[23]): adds a position modifier that puts a moving world instance at a rider, with an
 * offset.
 *
 * Copies the static defaults (set up on the first call) into a 24-byte parameter block (On Tour tAddRiderPositionData:
 * InstId, Being, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_00446190 says eLunoVar_Float. Defaults: InstId -1, Being -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode) and Being is not -1, the target is rider Being's position (SSXApp->Game->AI->Riders[Being],
 * cRider virtual getPosition) plus the offset (OffsetX, OffsetY, OffsetZ) turned by that rider's transform (virtual
 * getTransform); cMoveNode_addPosition gets it.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer. An empty rider slot is
 * not checked either.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddRiderPositionFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5608 == 0) {
        sRiderPositionDefaults.InstId = 0xFFFFFFFF;
        sRiderPositionDefaults.Being = -1;
        float z = 0.0f;
        sRiderPositionDefaults.OffsetX = z;
        sRiderPositionDefaults.OffsetY = z;
        sRiderPositionDefaults.OffsetZ = z;
        D_004A5608 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sRiderPositionDefaults.null = WScriptNullType(sRiderPositionDefaults.null);
    }
    unsigned buf64[6];
    unsigned *buf = buf64;
    tAddRiderPositionData *data = (tAddRiderPositionData*)buf;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_D758(buf, &sRiderPositionDefaults, sizeof(tAddRiderPositionData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446190[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode()) {
            int k = data->Being;
            if (k != kWScriptBeing_None) {
                V4_D758 pos = *(V4_D758*)shape_D758(k)->getPosition();
                t4Matrix *riderTransform = shape_D758(data->Being)->getTransform();
                vAddTo_D758(pos, mtxApply_D758(riderTransform, V4_D758(data->OffsetX, data->OffsetY, data->OffsetZ,
                        0.0f)));
                cMoveNode_addPosition_D758(activeNode, &pos);
            }
        }
    }
    return luno_gLunoNil_D758;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddParentFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V4_D9F8 { float x, y, z, w; V4_D9F8(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern "C" void cMoveNode_addParent_D9F8(void*, tInstanceRecord*, int, V4_D9F8*) __asm__("cMoveNode_addParent");
static inline tInstanceRecord *refToPtr_D9F8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_D9F8(unsigned i) { if (i == 0) return 0; return refToPtr_D9F8(i); }
extern t3Vector luno_gLunoNil_D9F8 __asm__("luno_gLunoNil");
// World-script data that parents one instance's node to another (On Tour tAddParentData; same offsets): the block below
// fills it.
struct tAddParentData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    unsigned int ParentId;          // 0x04 [O] tResourceID
    int Mesh;                       // 0x08 [O]
    float OffsetX;                  // 0x0C [O]
    float OffsetY;                  // 0x10 [O]
    float OffsetZ;                  // 0x14 [O]
    WScriptNullType null;           // 0x18 [O]
    char unk_019[0x3];              // 0x19 padding
};

CHECK_OFFSET(tAddParentData, InstId, 0x00);
CHECK_OFFSET(tAddParentData, ParentId, 0x04);
CHECK_OFFSET(tAddParentData, Mesh, 0x08);
CHECK_OFFSET(tAddParentData, OffsetX, 0x0C);
CHECK_OFFSET(tAddParentData, OffsetY, 0x10);
CHECK_OFFSET(tAddParentData, OffsetZ, 0x14);
CHECK_OFFSET(tAddParentData, null, 0x18);
CHECK_SIZE(tAddParentData, 0x1C);
extern tAddParentData sParentDefaults __asm__("D_004FB738");
extern int AddParentFunc_initFlag;
extern const int AddParentFunc_types[];
extern "C" void* MEM_copy_D9F8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[17]): attaches a moving world instance to a parent instance (a parent modifier).
 *
 * Copies the static defaults (set up on the first call) into a 28-byte parameter block (On Tour tAddParentData: InstId,
 * ParentId, Mesh, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table AddParentFunc_types says eLunoVar_Float. Defaults: InstId -1, ParentId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when ParentId is -1, the parent is not found, has
 * no active node or that node is ATYPE_DEAD (virtual isType(6)). Otherwise, when the instance's own active node is a
 * move node (virtual isMoveNode), cMoveNode_addParent gets the parent node's instance record (Instance), Mesh and the
 * offset (OffsetX, OffsetY, OffsetZ, 0).
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddParentFunc(int n, luno_cLunoCArg *params) {
    if (AddParentFunc_initFlag == 0) {
        sParentDefaults.InstId = 0xFFFFFFFF;
        sParentDefaults.ParentId = 0xFFFFFFFF;
        sParentDefaults.Mesh = 0;
        float z = 0.0f;
        sParentDefaults.OffsetX = z;
        sParentDefaults.OffsetY = z;
        sParentDefaults.OffsetZ = z;
        AddParentFunc_initFlag = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sParentDefaults.null = WScriptNullType(sParentDefaults.null);
    }
    unsigned buf64[7];
    unsigned *buf = buf64;
    tAddParentData *data = (tAddParentData*)buf;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_D9F8(buf, &sParentDefaults, sizeof(tAddParentData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = AddParentFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_D9F8(i);
            inst = found;
        }
        obj = inst;
    }
    unsigned id2 = data->ParentId;
    if (~id2 != 0) {
        cWorldCacheTable *parentSection = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id2 & 0xFF];
        unsigned parentRef;
        tInstanceRecord *parentInst;
        if (!parentSection || (parentRef = parentSection->m_pCacheTable[3].m_pDataTag[id2 >> 8].m_bfuiMemLoc) == 0)
            parentInst = 0;
        else
            parentInst = instanceAtMemLoc(parentRef);
        cInstanceNode *parentNode = parentInst ? parentInst->m_pActiveNode : 0;
        if (parentNode && !parentNode->isType(6)) {
            cInstanceNode *activeNode = obj->m_pActiveNode;
            if (activeNode && activeNode->isMoveNode()) {
                tInstanceRecord *parentRecord = parentNode->Instance;
                V4_D9F8 offset(data->OffsetX, data->OffsetY, data->OffsetZ, 0.0f);
                cMoveNode_addParent_D9F8(activeNode, parentRecord, data->Mesh, &offset);
            }
        }
    }
    return luno_gLunoNil_D9F8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddChildFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V4_DC60 { float x, y, z, w; V4_DC60(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern "C" void cMoveNode_addParent_DC60(void*, tInstanceRecord*, int, V4_DC60*) __asm__("cMoveNode_addParent");
static inline tInstanceRecord *refToPtr_DC60(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_DC60(unsigned i) { if (i == 0) return 0; return refToPtr_DC60(i); }
extern t3Vector luno_gLunoNil_DC60 __asm__("luno_gLunoNil");
// World-script data that adds a child instance to a node (On Tour tAddChildData; same offsets): the block below fills
// it.
struct tAddChildData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    unsigned int ChildId;           // 0x04 [O] tResourceID
    int Mesh;                       // 0x08 [O]
    float OffsetX;                  // 0x0C [O]
    float OffsetY;                  // 0x10 [O]
    float OffsetZ;                  // 0x14 [O]
    WScriptNullType null;           // 0x18 [O]
    char unk_019[0x3];              // 0x19 padding
};

CHECK_OFFSET(tAddChildData, InstId, 0x00);
CHECK_OFFSET(tAddChildData, ChildId, 0x04);
CHECK_OFFSET(tAddChildData, Mesh, 0x08);
CHECK_OFFSET(tAddChildData, OffsetX, 0x0C);
CHECK_OFFSET(tAddChildData, OffsetY, 0x10);
CHECK_OFFSET(tAddChildData, OffsetZ, 0x14);
CHECK_OFFSET(tAddChildData, null, 0x18);
CHECK_SIZE(tAddChildData, 0x1C);
extern tAddChildData sChildDefaults __asm__("D_004FB758");
extern int AddChildFunc_initFlag;
extern const int AddChildFunc_types[];
extern "C" void* MEM_copy_DC60(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[18]): makes a world instance the parent of a moving child instance (a parent
 * modifier on the child).
 *
 * Copies the static defaults (set up on the first call) into a 28-byte parameter block (On Tour tAddChildData: InstId,
 * ChildId, Mesh, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table AddChildFunc_types says eLunoVar_Float. Defaults: InstId -1, ChildId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when ChildId is -1, the instance has no active
 * node (m_pActiveNode) or that node is ATYPE_DEAD (virtual isType(6)). Otherwise ChildId is looked up the same way and,
 * when the child's active node is a move node (virtual isMoveNode), cMoveNode_addParent gets the instance's node record
 * (Instance), Mesh and the offset (OffsetX, OffsetY, OffsetZ, 0), so the child follows this instance.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddChildFunc(int n, luno_cLunoCArg *params) {
    if (AddChildFunc_initFlag == 0) {
        sChildDefaults.InstId = 0xFFFFFFFF;
        sChildDefaults.ChildId = 0xFFFFFFFF;
        sChildDefaults.Mesh = 0;
        float z = 0.0f;
        sChildDefaults.OffsetX = z;
        sChildDefaults.OffsetY = z;
        sChildDefaults.OffsetZ = z;
        AddChildFunc_initFlag = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sChildDefaults.null = WScriptNullType(sChildDefaults.null);
    }
    unsigned buf64[7];
    unsigned *buf = buf64;
    tAddChildData *data = (tAddChildData*)buf;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_DC60(buf, &sChildDefaults, sizeof(tAddChildData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = AddChildFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_DC60(i);
            inst = found;
        }
        obj = inst;
    }
    if (~data->ChildId != 0) {
        cInstanceNode *parentNode = obj->m_pActiveNode;
        if (parentNode && !parentNode->isType(6)) {
            unsigned id2 = data->ChildId;
            cWorldCacheTable *childSection = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id2 & 0xFF];
            unsigned childRef;
            tInstanceRecord *childInst;
            if (!childSection || (childRef = childSection->m_pCacheTable[3].m_pDataTag[id2 >> 8].m_bfuiMemLoc) == 0)
                childInst = 0;
            else
                childInst = instanceAtMemLoc(childRef);
            cInstanceNode *childNode = childInst ? childInst->m_pActiveNode : 0;
            if (childNode && childNode->isMoveNode()) {
                tInstanceRecord *parentRecord = parentNode->Instance;
                V4_DC60 offset(data->OffsetX, data->OffsetY, data->OffsetZ, 0.0f);
                cMoveNode_addParent_DC60(childNode, parentRecord, data->Mesh, &offset);
            }
        }
    }
    return luno_gLunoNil_DC60;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddSplineFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addSpline1_DED0(void*, void*) __asm__("cMoveNode_addSpline1");
static inline tInstanceRecord *refToPtr_DED0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_DED0(unsigned i) { if (i == 0) return 0; return refToPtr_DED0(i); }
extern t3Vector luno_gLunoNil_DED0 __asm__("luno_gLunoNil");
extern tAddSplineData sSplineDefaults __asm__("D_004FB778");
extern int AddSplineFunc_initFlag;
extern const int AddSplineFunc_types[];
extern "C" void* MEM_copy_DED0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: puts a moving world instance on a spline path (a single-spline description).
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. Nothing happens without a spline ID;
 * an instance that is not found is not checked.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector AddSplineFunc(int n, luno_cLunoCArg *params) {
    if (AddSplineFunc_initFlag == 0) {
        sSplineDefaults.InstId = 0xFFFFFFFF;
        sSplineDefaults.SplineId = 0xFFFFFFFF;
        sSplineDefaults.Sequence = 1;
        sSplineDefaults.Orient = 3;
        sSplineDefaults.Speed = 20.0f;
        float z = 0.0f;
        sSplineDefaults.Heading = z;
        sSplineDefaults.StartDistance = z;
        sSplineDefaults.RandomDistance = z;
        sSplineDefaults.Start = 1;
        sSplineDefaults.Force = z;
        sSplineDefaults.ForceTime = z;
        AddSplineFunc_initFlag = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sSplineDefaults.null = WScriptNullType(sSplineDefaults.null);
    }
    unsigned buf64[12];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_DED0(buf, &sSplineDefaults, sizeof(tAddSplineData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = AddSplineFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_DED0(i);
            inst = found;
        }
        obj = inst;
    }
    if (~buf[1] != 0) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addSpline1_DED0(activeNode, buf);
        }
    }
    return luno_gLunoNil_DED0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddMultiSplineFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addSpline_E0C0(void*, void*) __asm__("cMoveNode_addSpline");
static inline tInstanceRecord *refToPtr_E0C0(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_E0C0(unsigned i) { if (i == 0) return 0; return refToPtr_E0C0(i); }
extern t3Vector luno_gLunoNil_E0C0 __asm__("luno_gLunoNil");
extern tAddMultiSplineData sMultiSplineDefaults __asm__("D_004FB7A8");
extern int AddMultiSplineFunc_initFlag;
extern const int AddMultiSplineFunc_types[];
extern "C" void* MEM_copy_E0C0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: puts a moving world instance on a multi-spline path.
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. Nothing happens without a spline ID;
 * an instance that is not found is not checked.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector AddMultiSplineFunc(int n, luno_cLunoCArg *params) {
    if (AddMultiSplineFunc_initFlag == 0) {
        sMultiSplineDefaults.InstId = 0xFFFFFFFF;
        sMultiSplineDefaults.SplineId = 0xFFFFFFFF;
        sMultiSplineDefaults.Orient = 3;
        sMultiSplineDefaults.Speed = 20.0f;
        float z = 0.0f;
        sMultiSplineDefaults.Heading = z;
        sMultiSplineDefaults.StartDistance = z;
        sMultiSplineDefaults.ColourA = 1.0f;
        sMultiSplineDefaults.ColourR = z;
        sMultiSplineDefaults.ColourG = z;
        sMultiSplineDefaults.ColourB = z;
        sMultiSplineDefaults.DrawSpline = 1;
        sMultiSplineDefaults.NumModels = 2;
        AddMultiSplineFunc_initFlag = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMultiSplineDefaults.null = WScriptNullType(sMultiSplineDefaults.null);
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_E0C0(buf, &sMultiSplineDefaults, sizeof(tAddMultiSplineData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = AddMultiSplineFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_E0C0(i);
            inst = found;
        }
        obj = inst;
    }
    if (~buf[1] != 0) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addSpline_E0C0(activeNode, buf);
        }
    }
    return luno_gLunoNil_E0C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddUVScrollFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addUVScroll_E2C0(void*, void*) __asm__("cMoveNode_addUVScroll");
extern t3Vector luno_gLunoNil_E2C0 __asm__("luno_gLunoNil");
// World-script data that adds a UV-scroll modifier (On Tour tAddUVScrollData; same offsets): the block below fills it.
struct tAddUVScrollData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Type;                       // 0x04 [O]
    float UStart;                   // 0x08 [O]
    float VStart;                   // 0x0C [O]
    float UScroll;                  // 0x10 [O]
    float VScroll;                  // 0x14 [O]
    float TimeOn;                   // 0x18 [O]
    float TimeOff;                  // 0x1C [O]
    float AngVel;                   // 0x20 [O]
    float AxisX;                    // 0x24 [O]
    float AxisY;                    // 0x28 [O]
    float AxisZ;                    // 0x2C [O]
    WScriptNullType null;           // 0x30 [O]
    char unk_031[0x3];              // 0x31 padding
};

CHECK_OFFSET(tAddUVScrollData, InstId, 0x00);
CHECK_OFFSET(tAddUVScrollData, Type, 0x04);
CHECK_OFFSET(tAddUVScrollData, UStart, 0x08);
CHECK_OFFSET(tAddUVScrollData, VStart, 0x0C);
CHECK_OFFSET(tAddUVScrollData, UScroll, 0x10);
CHECK_OFFSET(tAddUVScrollData, VScroll, 0x14);
CHECK_OFFSET(tAddUVScrollData, TimeOn, 0x18);
CHECK_OFFSET(tAddUVScrollData, TimeOff, 0x1C);
CHECK_OFFSET(tAddUVScrollData, AngVel, 0x20);
CHECK_OFFSET(tAddUVScrollData, AxisX, 0x24);
CHECK_OFFSET(tAddUVScrollData, AxisY, 0x28);
CHECK_OFFSET(tAddUVScrollData, AxisZ, 0x2C);
CHECK_OFFSET(tAddUVScrollData, null, 0x30);
CHECK_SIZE(tAddUVScrollData, 0x34);
extern tAddUVScrollData sUVScrollDefaults __asm__("D_004FB7E0");
extern int D_004A561C;
extern const int D_00446238[];
extern "C" void* MEM_copy_E2C0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[21]): adds a UV-scroll modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 52-byte parameter block (On Tour tAddUVScrollData:
 * InstId, Type, UStart, VStart, UScroll, VScroll, TimeOn, TimeOff, AngVel, AxisX, AxisY, AxisZ) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_00446238 says
 * eLunoVar_Float. Defaults: InstId -1, Type 5, UScroll 0.0166667, VScroll 0.0166667, TimeOn 1, AxisZ 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode), cMoveNode_addUVScroll gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddUVScrollFunc(int n, luno_cLunoCArg *params) {
    if (D_004A561C == 0) {
        sUVScrollDefaults.InstId = 0xFFFFFFFF;
        sUVScrollDefaults.Type = 5;
        float z = 0.0f;
        sUVScrollDefaults.UStart = z;
        sUVScrollDefaults.VStart = z;
        sUVScrollDefaults.UScroll = 0.016666699200868607f;
        sUVScrollDefaults.VScroll = 0.016666699200868607f;
        sUVScrollDefaults.TimeOn = 1.0f;
        sUVScrollDefaults.TimeOff = z;
        sUVScrollDefaults.AngVel = z;
        sUVScrollDefaults.AxisX = z;
        sUVScrollDefaults.AxisY = z;
        sUVScrollDefaults.AxisZ = 1.0f;
        D_004A561C = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sUVScrollDefaults.null = WScriptNullType(sUVScrollDefaults.null);
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E2C0(buf, &sUVScrollDefaults, sizeof(tAddUVScrollData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446238[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode())
            cMoveNode_addUVScroll_E2C0(activeNode, buf);
    }
    return luno_gLunoNil_E2C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddTexFlipFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addTexFlip_E4A8(void*, void*) __asm__("cMoveNode_addTexFlip");
extern t3Vector luno_gLunoNil_E4A8 __asm__("luno_gLunoNil");
extern tAddTexFlipData sTexFlipDefaults __asm__("D_004FB818");
extern int D_004A5620;
extern const int D_00446268[];
extern "C" void* MEM_copy_E4A8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[22]): adds a texture-flip modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 24-byte parameter block (On Tour tAddTexFlipData:
 * InstId, Type, StartId, Dir, Speed) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_00446268 says eLunoVar_Float. Defaults: InstId -1, Speed 2.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode), cMoveNode_addTexFlip gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddTexFlipFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5620 == 0) {
        sTexFlipDefaults.InstId = 0xFFFFFFFF;
        sTexFlipDefaults.Type = 0;
        sTexFlipDefaults.StartId = 0;
        sTexFlipDefaults.Dir = 0;
        sTexFlipDefaults.Speed = 2.0f;
        D_004A5620 = 1;
        sTexFlipDefaults.null = WScriptNullType(sTexFlipDefaults.null);
    }
    unsigned buf64[6];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E4A8(buf, &sTexFlipDefaults, sizeof(tAddTexFlipData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446268[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode())
            cMoveNode_addTexFlip_E4A8(activeNode, buf);
    }
    return luno_gLunoNil_E4A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddBoostFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addBoost_E668(void*, void*) __asm__("cMoveNode_addBoost");
extern t3Vector luno_gLunoNil_E668 __asm__("luno_gLunoNil");
extern tAddBoostData sAddBoostDefaults __asm__("D_004FB830");
extern int D_004A5624;
extern const int D_00446280[];
extern "C" void* MEM_copy_E668(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[24]): adds a boost modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 36-byte parameter block (On Tour tAddBoostData: InstId,
 * Type, BoostTime, Drag, MaxSpeed, BoostX, BoostY, BoostZ) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_00446280 says eLunoVar_Float. Defaults: InstId -1, Type 1,
 * Drag 0.1, MaxSpeed 20, BoostZ 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode), cMoveNode_addBoost gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddBoostFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5624 == 0) {
        sAddBoostDefaults.InstId = 0xFFFFFFFF;
        sAddBoostDefaults.Type = 1;
        float z = 0.0f;
        sAddBoostDefaults.BoostTime = z;
        sAddBoostDefaults.Drag = 0.10000000149011612f;
        sAddBoostDefaults.MaxSpeed = 20.0f;
        sAddBoostDefaults.BoostX = z;
        sAddBoostDefaults.BoostY = z;
        sAddBoostDefaults.BoostZ = 1.0f;
        D_004A5624 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAddBoostDefaults.null = WScriptNullType(sAddBoostDefaults.null);
    }
    unsigned buf64[9];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E668(buf, &sAddBoostDefaults, sizeof(tAddBoostData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446280[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode())
            cMoveNode_addBoost_E668(activeNode, buf);
    }
    return luno_gLunoNil_E668;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_E840(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_E840(unsigned i) { if (i == 0) return 0; return refToPtr_E840(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cParticleNode_cParticleNode1_E840(void*, int, void*, void*, int) __asm__("cParticleNode_cParticleNode1");
void operator_delete(int*);
extern char D_00489740_E840[] __asm__("D_00489740");
extern char str_ParticleNode_E840[] __asm__("str_ParticleNode");
extern "C" void cMoveNode_addParticle_E840(void*, void*) __asm__("cMoveNode_addParticle");
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_E840(const t4Vector &a, const t4Vector &b) {
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
static inline float vLen_E840(const t4Vector &v) {
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
static inline cAI *lvl_E840() { return SSXApp->Game->AI; }
static inline int nearViewer_E840(const t4Vector &pos) {
    int notEdit = ScriptEngine->mPlayingNISList != 2;
    if (notEdit)
        return 1;
    int n = lvl_E840()->NumberOfPlayersAllocated;
    for (int i = 0; i < n; i++) {
        t4Vector d = vSub_E840(pos, *lvl_E840()->Players[i]->mRider->getPosition());
        if (vLen_E840(d) < 30000.0f)
            return 1;
    }
    n = lvl_E840()->NumberOfMissionRidersLoaded;
    for (int i = 0; i < n; i++) {
        t4Vector d = vSub_E840(pos, *lvl_E840()->MissionRiders[i]->mRider->getPosition());
        if (vLen_E840(d) < 30000.0f)
            return 1;
    }
    return 0;
}
extern t3Vector luno_gLunoNil_E840 __asm__("luno_gLunoNil");
// World-script data that adds a particle effect to a node (On Tour tAddParticleData; same offsets): the block below
// fills it.
struct tAddParticleData {
    int NumParticles;               // 0x00 [O]
    int NumBlur;                    // 0x04 [O]
    float Duration;                 // 0x08 [O]
    float Damp;                     // 0x0C [O]
    float Size;                     // 0x10 [O]
    float Life;                     // 0x14 [O]
    float SizeR;                    // 0x18 [O]
    float LifeR;                    // 0x1C [O]
    float BlurStep;                 // 0x20 [O]
    float OffX;                     // 0x24 [O]
    float OffY;                     // 0x28 [O]
    float OffZ;                     // 0x2C [O]
    float R0X;                      // 0x30 [O]
    float R0Y;                      // 0x34 [O]
    float R0Z;                      // 0x38 [O]
    float R1X;                      // 0x3C [O]
    float R1Y;                      // 0x40 [O]
    float R1Z;                      // 0x44 [O]
    float VelX;                     // 0x48 [O]
    float VelY;                     // 0x4C [O]
    float VelZ;                     // 0x50 [O]
    float R0VX;                     // 0x54 [O]
    float R0VY;                     // 0x58 [O]
    float R0VZ;                     // 0x5C [O]
    float R1VX;                     // 0x60 [O]
    float R1VY;                     // 0x64 [O]
    float R1VZ;                     // 0x68 [O]
    float R2VX;                     // 0x6C [O]
    float R2VY;                     // 0x70 [O]
    float R2VZ;                     // 0x74 [O]
    float ForceX;                   // 0x78 [O]
    float ForceY;                   // 0x7C [O]
    float ForceZ;                   // 0x80 [O]
    float StartColA;                // 0x84 [O]
    float StartColR;                // 0x88 [O]
    float StartColG;                // 0x8C [O]
    float StartColB;                // 0x90 [O]
    float EndColA;                  // 0x94 [O]
    float EndColR;                  // 0x98 [O]
    float EndColG;                  // 0x9C [O]
    float EndColB;                  // 0xA0 [O]
    float R0A;                      // 0xA4 [O]
    float R0R;                      // 0xA8 [O]
    float R0G;                      // 0xAC [O]
    float R0B;                      // 0xB0 [O]
    float R1A;                      // 0xB4 [O]
    float R1R;                      // 0xB8 [O]
    float R1G;                      // 0xBC [O]
    float R1B;                      // 0xC0 [O]
    int TextureId;                  // 0xC4 [O]
    int BlendMode;                  // 0xC8 [O]
    float SizeFinal;                // 0xCC [O]
    int NumFlipTextures;            // 0xD0 [O]
    float FlipTextureRate;          // 0xD4 [O]
    unsigned int InstId;            // 0xD8 [O] tResourceID
    WScriptNullType null;           // 0xDC [O]
    char unk_0DD[0x3];              // 0xDD padding
};

CHECK_OFFSET(tAddParticleData, NumParticles, 0x00);
CHECK_OFFSET(tAddParticleData, NumBlur, 0x04);
CHECK_OFFSET(tAddParticleData, Duration, 0x08);
CHECK_OFFSET(tAddParticleData, Damp, 0x0C);
CHECK_OFFSET(tAddParticleData, Size, 0x10);
CHECK_OFFSET(tAddParticleData, Life, 0x14);
CHECK_OFFSET(tAddParticleData, SizeR, 0x18);
CHECK_OFFSET(tAddParticleData, LifeR, 0x1C);
CHECK_OFFSET(tAddParticleData, BlurStep, 0x20);
CHECK_OFFSET(tAddParticleData, OffX, 0x24);
CHECK_OFFSET(tAddParticleData, OffY, 0x28);
CHECK_OFFSET(tAddParticleData, OffZ, 0x2C);
CHECK_OFFSET(tAddParticleData, R0X, 0x30);
CHECK_OFFSET(tAddParticleData, R0Y, 0x34);
CHECK_OFFSET(tAddParticleData, R0Z, 0x38);
CHECK_OFFSET(tAddParticleData, R1X, 0x3C);
CHECK_OFFSET(tAddParticleData, R1Y, 0x40);
CHECK_OFFSET(tAddParticleData, R1Z, 0x44);
CHECK_OFFSET(tAddParticleData, VelX, 0x48);
CHECK_OFFSET(tAddParticleData, VelY, 0x4C);
CHECK_OFFSET(tAddParticleData, VelZ, 0x50);
CHECK_OFFSET(tAddParticleData, R0VX, 0x54);
CHECK_OFFSET(tAddParticleData, R0VY, 0x58);
CHECK_OFFSET(tAddParticleData, R0VZ, 0x5C);
CHECK_OFFSET(tAddParticleData, R1VX, 0x60);
CHECK_OFFSET(tAddParticleData, R1VY, 0x64);
CHECK_OFFSET(tAddParticleData, R1VZ, 0x68);
CHECK_OFFSET(tAddParticleData, R2VX, 0x6C);
CHECK_OFFSET(tAddParticleData, R2VY, 0x70);
CHECK_OFFSET(tAddParticleData, R2VZ, 0x74);
CHECK_OFFSET(tAddParticleData, ForceX, 0x78);
CHECK_OFFSET(tAddParticleData, ForceY, 0x7C);
CHECK_OFFSET(tAddParticleData, ForceZ, 0x80);
CHECK_OFFSET(tAddParticleData, StartColA, 0x84);
CHECK_OFFSET(tAddParticleData, StartColR, 0x88);
CHECK_OFFSET(tAddParticleData, StartColG, 0x8C);
CHECK_OFFSET(tAddParticleData, StartColB, 0x90);
CHECK_OFFSET(tAddParticleData, EndColA, 0x94);
CHECK_OFFSET(tAddParticleData, EndColR, 0x98);
CHECK_OFFSET(tAddParticleData, EndColG, 0x9C);
CHECK_OFFSET(tAddParticleData, EndColB, 0xA0);
CHECK_OFFSET(tAddParticleData, R0A, 0xA4);
CHECK_OFFSET(tAddParticleData, R0R, 0xA8);
CHECK_OFFSET(tAddParticleData, R0G, 0xAC);
CHECK_OFFSET(tAddParticleData, R0B, 0xB0);
CHECK_OFFSET(tAddParticleData, R1A, 0xB4);
CHECK_OFFSET(tAddParticleData, R1R, 0xB8);
CHECK_OFFSET(tAddParticleData, R1G, 0xBC);
CHECK_OFFSET(tAddParticleData, R1B, 0xC0);
CHECK_OFFSET(tAddParticleData, TextureId, 0xC4);
CHECK_OFFSET(tAddParticleData, BlendMode, 0xC8);
CHECK_OFFSET(tAddParticleData, SizeFinal, 0xCC);
CHECK_OFFSET(tAddParticleData, NumFlipTextures, 0xD0);
CHECK_OFFSET(tAddParticleData, FlipTextureRate, 0xD4);
CHECK_OFFSET(tAddParticleData, InstId, 0xD8);
CHECK_OFFSET(tAddParticleData, null, 0xDC);
CHECK_SIZE(tAddParticleData, 0xE0);
extern tAddParticleData sAddParticleDefaults __asm__("D_004FB858");
extern int D_004A5628;
extern const int D_004462A0[];
extern "C" void* MEM_copy_E840(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function AddParticle (gLunoCFunctions, between AddBoost and AddDynamicParticle): adds a particle emitter
 * to a world instance near the viewer.
 *
 * Builds a heap "AddParticleData" block from defaults and the arguments; a moving instance gets a particle
 * modifier, a static one a new "ParticleNode". Far from the viewer the block is freed. An instance ID of -1
 * means the script's current instance; one that is not found is not checked.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector AddParticleFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5628 == 0) {
        sAddParticleDefaults.NumParticles = 1;
        sAddParticleDefaults.NumBlur = 0;
        sAddParticleDefaults.Duration = -1.0f;
        sAddParticleDefaults.Damp = 1.0f;
        sAddParticleDefaults.Size = 4.0f;
        float z = 0.0f;
        sAddParticleDefaults.Life = z;
        sAddParticleDefaults.SizeR = z;
        sAddParticleDefaults.LifeR = z;
        sAddParticleDefaults.BlurStep = z;
        sAddParticleDefaults.OffX = z;
        sAddParticleDefaults.OffY = z;
        sAddParticleDefaults.OffZ = z;
        sAddParticleDefaults.R0X = z;
        sAddParticleDefaults.R0Y = z;
        sAddParticleDefaults.R0Z = z;
        sAddParticleDefaults.R1X = z;
        sAddParticleDefaults.R1Y = z;
        sAddParticleDefaults.R1Z = z;
        sAddParticleDefaults.VelX = z;
        sAddParticleDefaults.VelY = z;
        sAddParticleDefaults.VelZ = z;
        sAddParticleDefaults.R0VX = z;
        sAddParticleDefaults.R0VY = z;
        sAddParticleDefaults.R0VZ = z;
        sAddParticleDefaults.R1VX = z;
        sAddParticleDefaults.R1VY = z;
        sAddParticleDefaults.R1VZ = z;
        sAddParticleDefaults.R2VX = z;
        sAddParticleDefaults.R2VY = z;
        sAddParticleDefaults.R2VZ = z;
        sAddParticleDefaults.ForceX = z;
        sAddParticleDefaults.ForceY = z;
        sAddParticleDefaults.ForceZ = z;
        sAddParticleDefaults.StartColA = 1.0f;
        sAddParticleDefaults.StartColR = z;
        sAddParticleDefaults.StartColG = z;
        sAddParticleDefaults.StartColB = z;
        sAddParticleDefaults.EndColA = 1.0f;
        sAddParticleDefaults.EndColR = 1.0f;
        sAddParticleDefaults.EndColG = 1.0f;
        sAddParticleDefaults.EndColB = 1.0f;
        sAddParticleDefaults.R0A = 1.0f;
        sAddParticleDefaults.R0R = z;
        sAddParticleDefaults.R0G = z;
        sAddParticleDefaults.R0B = z;
        sAddParticleDefaults.R1A = 1.0f;
        sAddParticleDefaults.R1R = z;
        sAddParticleDefaults.R1G = z;
        sAddParticleDefaults.R1B = z;
        sAddParticleDefaults.TextureId = 16;
        sAddParticleDefaults.BlendMode = 0;
        sAddParticleDefaults.SizeFinal = z;
        sAddParticleDefaults.NumFlipTextures = 1;
        sAddParticleDefaults.FlipTextureRate = 20.0f;
        sAddParticleDefaults.InstId = 0xFFFFFFFF;
        D_004A5628 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAddParticleDefaults.null = WScriptNullType(sAddParticleDefaults.null);
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xE0, D_00489740_E840, 0x20000000, 0);
    tAddParticleData *data = (tAddParticleData*)buf;
    data->InstId = 0xFFFFFFFF;
    MEM_copy_E840(buf, &sAddParticleDefaults, sizeof(tAddParticleData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = D_004462A0[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    if (data->LifeR > data->Life)
        data->LifeR = data->Life;
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_E840(i);
            inst = found;
        }
        obj = inst;
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isMoveNode()) {
            t4Vector pos = activeNode->getRootTransform()->row[3];
            if (nearViewer_E840(pos)) {
                cMoveNode_addParticle_E840(activeNode, buf);
                return luno_gLunoNil_E840;
            }
        }
    } else {
        t4Vector pos = *(t4Vector*)obj->m_fmTransform.m[3];
        if (nearViewer_E840(pos)) {
            cParticleNode_cParticleNode1_E840(cMemMan_alloc(44, str_ParticleNode_E840, 0x20000000, 0), 1, obj, buf, 0);
            return luno_gLunoNil_E840;
        }
    }
    operator_delete((int*)buf);
    return luno_gLunoNil_E840;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddDynamicParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline char *refToPtr_EE98(unsigned i) { return (char*)(i << 2); }
static inline char *conv_EE98(unsigned i) { if (i == 0) return 0; return refToPtr_EE98(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
struct V4_EE98 { float x, y, z, w; V4_EE98(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern "C" void cMoveNode_addDynamicParticle_EE98(void*, void*, int, V4_EE98*) __asm__("cMoveNode_addDynamicParticle");
void operator_delete(int*);
extern char D_00489750_EE98[] __asm__("D_00489750");
extern t3Vector luno_gLunoNil_EE98 __asm__("luno_gLunoNil");
// World-script data that adds a dynamic particle effect to a node (On Tour tAddDynamicParticleData; same offsets): the
// block below fills it.
struct tAddDynamicParticleData {
    int NumParticles;               // 0x00 [O]
    int NumBlur;                    // 0x04 [O]
    float Duration;                 // 0x08 [O]
    float Damp;                     // 0x0C [O]
    float Size;                     // 0x10 [O]
    float Life;                     // 0x14 [O]
    float SizeR;                    // 0x18 [O]
    float LifeR;                    // 0x1C [O]
    float BlurStep;                 // 0x20 [O]
    float OffX;                     // 0x24 [O]
    float OffY;                     // 0x28 [O]
    float OffZ;                     // 0x2C [O]
    float R0X;                      // 0x30 [O]
    float R0Y;                      // 0x34 [O]
    float R0Z;                      // 0x38 [O]
    float R1X;                      // 0x3C [O]
    float R1Y;                      // 0x40 [O]
    float R1Z;                      // 0x44 [O]
    float VelX;                     // 0x48 [O]
    float VelY;                     // 0x4C [O]
    float VelZ;                     // 0x50 [O]
    float R0VX;                     // 0x54 [O]
    float R0VY;                     // 0x58 [O]
    float R0VZ;                     // 0x5C [O]
    float R1VX;                     // 0x60 [O]
    float R1VY;                     // 0x64 [O]
    float R1VZ;                     // 0x68 [O]
    float R2VX;                     // 0x6C [O]
    float R2VY;                     // 0x70 [O]
    float R2VZ;                     // 0x74 [O]
    float ForceX;                   // 0x78 [O]
    float ForceY;                   // 0x7C [O]
    float ForceZ;                   // 0x80 [O]
    float StartColA;                // 0x84 [O]
    float StartColR;                // 0x88 [O]
    float StartColG;                // 0x8C [O]
    float StartColB;                // 0x90 [O]
    float EndColA;                  // 0x94 [O]
    float EndColR;                  // 0x98 [O]
    float EndColG;                  // 0x9C [O]
    float EndColB;                  // 0xA0 [O]
    float R0A;                      // 0xA4 [O]
    float R0R;                      // 0xA8 [O]
    float R0G;                      // 0xAC [O]
    float R0B;                      // 0xB0 [O]
    float R1A;                      // 0xB4 [O]
    float R1R;                      // 0xB8 [O]
    float R1G;                      // 0xBC [O]
    float R1B;                      // 0xC0 [O]
    int TextureId;                  // 0xC4 [O]
    int BlendMode;                  // 0xC8 [O]
    float SizeFinal;                // 0xCC [O]
    int NumFlipTextures;            // 0xD0 [O]
    float FlipTextureRate;          // 0xD4 [O]
    unsigned int InstId;            // 0xD8 [O] tResourceID
    int Mesh;                       // 0xDC [O]
    float OffsetX;                  // 0xE0 [O]
    float OffsetY;                  // 0xE4 [O]
    float OffsetZ;                  // 0xE8 [O]
    WScriptNullType null;           // 0xEC [O]
    char unk_0ED[0x3];              // 0xED padding
};

CHECK_OFFSET(tAddDynamicParticleData, NumParticles, 0x00);
CHECK_OFFSET(tAddDynamicParticleData, NumBlur, 0x04);
CHECK_OFFSET(tAddDynamicParticleData, Duration, 0x08);
CHECK_OFFSET(tAddDynamicParticleData, Damp, 0x0C);
CHECK_OFFSET(tAddDynamicParticleData, Size, 0x10);
CHECK_OFFSET(tAddDynamicParticleData, Life, 0x14);
CHECK_OFFSET(tAddDynamicParticleData, SizeR, 0x18);
CHECK_OFFSET(tAddDynamicParticleData, LifeR, 0x1C);
CHECK_OFFSET(tAddDynamicParticleData, BlurStep, 0x20);
CHECK_OFFSET(tAddDynamicParticleData, OffX, 0x24);
CHECK_OFFSET(tAddDynamicParticleData, OffY, 0x28);
CHECK_OFFSET(tAddDynamicParticleData, OffZ, 0x2C);
CHECK_OFFSET(tAddDynamicParticleData, R0X, 0x30);
CHECK_OFFSET(tAddDynamicParticleData, R0Y, 0x34);
CHECK_OFFSET(tAddDynamicParticleData, R0Z, 0x38);
CHECK_OFFSET(tAddDynamicParticleData, R1X, 0x3C);
CHECK_OFFSET(tAddDynamicParticleData, R1Y, 0x40);
CHECK_OFFSET(tAddDynamicParticleData, R1Z, 0x44);
CHECK_OFFSET(tAddDynamicParticleData, VelX, 0x48);
CHECK_OFFSET(tAddDynamicParticleData, VelY, 0x4C);
CHECK_OFFSET(tAddDynamicParticleData, VelZ, 0x50);
CHECK_OFFSET(tAddDynamicParticleData, R0VX, 0x54);
CHECK_OFFSET(tAddDynamicParticleData, R0VY, 0x58);
CHECK_OFFSET(tAddDynamicParticleData, R0VZ, 0x5C);
CHECK_OFFSET(tAddDynamicParticleData, R1VX, 0x60);
CHECK_OFFSET(tAddDynamicParticleData, R1VY, 0x64);
CHECK_OFFSET(tAddDynamicParticleData, R1VZ, 0x68);
CHECK_OFFSET(tAddDynamicParticleData, R2VX, 0x6C);
CHECK_OFFSET(tAddDynamicParticleData, R2VY, 0x70);
CHECK_OFFSET(tAddDynamicParticleData, R2VZ, 0x74);
CHECK_OFFSET(tAddDynamicParticleData, ForceX, 0x78);
CHECK_OFFSET(tAddDynamicParticleData, ForceY, 0x7C);
CHECK_OFFSET(tAddDynamicParticleData, ForceZ, 0x80);
CHECK_OFFSET(tAddDynamicParticleData, StartColA, 0x84);
CHECK_OFFSET(tAddDynamicParticleData, StartColR, 0x88);
CHECK_OFFSET(tAddDynamicParticleData, StartColG, 0x8C);
CHECK_OFFSET(tAddDynamicParticleData, StartColB, 0x90);
CHECK_OFFSET(tAddDynamicParticleData, EndColA, 0x94);
CHECK_OFFSET(tAddDynamicParticleData, EndColR, 0x98);
CHECK_OFFSET(tAddDynamicParticleData, EndColG, 0x9C);
CHECK_OFFSET(tAddDynamicParticleData, EndColB, 0xA0);
CHECK_OFFSET(tAddDynamicParticleData, R0A, 0xA4);
CHECK_OFFSET(tAddDynamicParticleData, R0R, 0xA8);
CHECK_OFFSET(tAddDynamicParticleData, R0G, 0xAC);
CHECK_OFFSET(tAddDynamicParticleData, R0B, 0xB0);
CHECK_OFFSET(tAddDynamicParticleData, R1A, 0xB4);
CHECK_OFFSET(tAddDynamicParticleData, R1R, 0xB8);
CHECK_OFFSET(tAddDynamicParticleData, R1G, 0xBC);
CHECK_OFFSET(tAddDynamicParticleData, R1B, 0xC0);
CHECK_OFFSET(tAddDynamicParticleData, TextureId, 0xC4);
CHECK_OFFSET(tAddDynamicParticleData, BlendMode, 0xC8);
CHECK_OFFSET(tAddDynamicParticleData, SizeFinal, 0xCC);
CHECK_OFFSET(tAddDynamicParticleData, NumFlipTextures, 0xD0);
CHECK_OFFSET(tAddDynamicParticleData, FlipTextureRate, 0xD4);
CHECK_OFFSET(tAddDynamicParticleData, InstId, 0xD8);
CHECK_OFFSET(tAddDynamicParticleData, Mesh, 0xDC);
CHECK_OFFSET(tAddDynamicParticleData, OffsetX, 0xE0);
CHECK_OFFSET(tAddDynamicParticleData, OffsetY, 0xE4);
CHECK_OFFSET(tAddDynamicParticleData, OffsetZ, 0xE8);
CHECK_OFFSET(tAddDynamicParticleData, null, 0xEC);
CHECK_SIZE(tAddDynamicParticleData, 0xF0);
extern tAddDynamicParticleData sDynamicParticleDefaults __asm__("D_004FB938");
extern int D_004A562C;
extern const int D_00446380[];
extern "C" void* MEM_copy_EE98(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[26]): adds a dynamic (moving) particle emitter to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 240-byte parameter block (On Tour
 * tAddDynamicParticleData: NumParticles, NumBlur, Duration, Damp, Size, Life, SizeR, LifeR, BlurStep, OffX, OffY, OffZ,
 * R0X, R0Y, R0Z, R1X, R1Y, R1Z, VelX, VelY, VelZ, R0VX, R0VY, R0VZ, R1VX, R1VY, R1VZ, R2VX, R2VY, R2VZ, ForceX, ForceY,
 * ForceZ, StartColA, StartColR, StartColG, StartColB, EndColA, EndColR, EndColG, EndColB, R0A, R0R, R0G, R0B, R1A, R1R,
 * R1G, R1B, TextureId, BlendMode, SizeFinal, NumFlipTextures, FlipTextureRate, InstId, Mesh, OffsetX, OffsetY, OffsetZ)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_00446380 says eLunoVar_Float. Defaults: NumParticles 1, Duration -1, Damp 1, Size 4, StartColA 1, EndColA 1,
 * EndColR 1, EndColG 1, EndColB 1, R0A 1, R1A 1, TextureId 16, NumFlipTextures 1, FlipTextureRate 20, InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The block is heap-allocated (cMemMan_alloc 0xF0,
 * "AddDynamicParticleData") because the modifier keeps it; InstId sits at +0xD8. When the instance's active node
 * (m_pActiveNode) is a move node (virtual isMoveNode), cMoveNode_addDynamicParticle gets the block, Mesh and the offset
 * (OffsetX, OffsetY, OffsetZ, 1); otherwise the block is freed.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddDynamicParticleFunc(int n, luno_cLunoCArg *params) {
    if (D_004A562C == 0) {
        sDynamicParticleDefaults.NumParticles = 1;
        sDynamicParticleDefaults.NumBlur = 0;
        sDynamicParticleDefaults.Duration = -1.0f;
        sDynamicParticleDefaults.Damp = 1.0f;
        sDynamicParticleDefaults.Size = 4.0f;
        float z = 0.0f;
        sDynamicParticleDefaults.Life = z;
        sDynamicParticleDefaults.SizeR = z;
        sDynamicParticleDefaults.LifeR = z;
        sDynamicParticleDefaults.BlurStep = z;
        sDynamicParticleDefaults.OffX = z;
        sDynamicParticleDefaults.OffY = z;
        sDynamicParticleDefaults.OffZ = z;
        sDynamicParticleDefaults.R0X = z;
        sDynamicParticleDefaults.R0Y = z;
        sDynamicParticleDefaults.R0Z = z;
        sDynamicParticleDefaults.R1X = z;
        sDynamicParticleDefaults.R1Y = z;
        sDynamicParticleDefaults.R1Z = z;
        sDynamicParticleDefaults.VelX = z;
        sDynamicParticleDefaults.VelY = z;
        sDynamicParticleDefaults.VelZ = z;
        sDynamicParticleDefaults.R0VX = z;
        sDynamicParticleDefaults.R0VY = z;
        sDynamicParticleDefaults.R0VZ = z;
        sDynamicParticleDefaults.R1VX = z;
        sDynamicParticleDefaults.R1VY = z;
        sDynamicParticleDefaults.R1VZ = z;
        sDynamicParticleDefaults.R2VX = z;
        sDynamicParticleDefaults.R2VY = z;
        sDynamicParticleDefaults.R2VZ = z;
        sDynamicParticleDefaults.ForceX = z;
        sDynamicParticleDefaults.ForceY = z;
        sDynamicParticleDefaults.ForceZ = z;
        sDynamicParticleDefaults.StartColA = 1.0f;
        sDynamicParticleDefaults.StartColR = z;
        sDynamicParticleDefaults.StartColG = z;
        sDynamicParticleDefaults.StartColB = z;
        sDynamicParticleDefaults.EndColA = 1.0f;
        sDynamicParticleDefaults.EndColR = 1.0f;
        sDynamicParticleDefaults.EndColG = 1.0f;
        sDynamicParticleDefaults.EndColB = 1.0f;
        sDynamicParticleDefaults.R0A = 1.0f;
        sDynamicParticleDefaults.R0R = z;
        sDynamicParticleDefaults.R0G = z;
        sDynamicParticleDefaults.R0B = z;
        sDynamicParticleDefaults.R1A = 1.0f;
        sDynamicParticleDefaults.R1R = z;
        sDynamicParticleDefaults.R1G = z;
        sDynamicParticleDefaults.R1B = z;
        sDynamicParticleDefaults.TextureId = 16;
        sDynamicParticleDefaults.BlendMode = 0;
        sDynamicParticleDefaults.SizeFinal = z;
        sDynamicParticleDefaults.NumFlipTextures = 1;
        sDynamicParticleDefaults.FlipTextureRate = 20.0f;
        sDynamicParticleDefaults.InstId = 0xFFFFFFFF;
        sDynamicParticleDefaults.Mesh = 0;
        sDynamicParticleDefaults.OffsetX = z;
        sDynamicParticleDefaults.OffsetY = z;
        sDynamicParticleDefaults.OffsetZ = z;
        D_004A562C = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDynamicParticleDefaults.null = WScriptNullType(sDynamicParticleDefaults.null);
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xF0, D_00489750_EE98, 0x20000000, 0);
    tAddDynamicParticleData *data = (tAddDynamicParticleData*)buf;
    data->InstId = 0xFFFFFFFF;
    MEM_copy_EE98(buf, &sDynamicParticleDefaults, sizeof(tAddDynamicParticleData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = D_00446380[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    if (data->LifeR > data->Life)
        data->LifeR = data->Life;
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && activeNode->isMoveNode()) {
        float one = 1.0f;
        V4_EE98 offset(data->OffsetX, data->OffsetY, data->OffsetZ, one);
        cMoveNode_addDynamicParticle_EE98(activeNode, buf, data->Mesh, &offset);
        return luno_gLunoNil_EE98;
    }
    operator_delete((int*)buf);
    return luno_gLunoNil_EE98;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addRail_F1C8(void*, void*) __asm__("cMoveNode_addRail");
extern t3Vector luno_gLunoNil_F1C8 __asm__("luno_gLunoNil");
extern tAddRailData sRailDefaults __asm__("D_004FBA28");
extern int D_004A5630;
extern const int D_00446470[];
extern "C" void* MEM_copy_F1C8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function: adds a rail to a moving world instance.
 *
 * Defaults come from a static block; each argument overrides one field (ints become floats where the field
 * is a float). An instance ID of -1 means the script's current instance. Nothing happens without a rail ID;
 * an instance that is not found is not checked.
 *
 * @param n  number of arguments (On Tour numArgs)
 * @param params  the arguments (On Tour cLunoCArg*)
 * @return a void script value
 */
extern "C" t3Vector AddRailFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5630 == 0) {
        sRailDefaults.InstId = 0xFFFFFFFF;
        sRailDefaults.SplineId = 0xFFFFFFFF;
        sRailDefaults.Mesh = -1;
        D_004A5630 = 1;
        sRailDefaults.null = WScriptNullType(sRailDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_F1C8(buf, &sRailDefaults, sizeof(tAddRailData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446470[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (~buf[1] != 0) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addRail_F1C8(activeNode, buf);
        }
    }
    return luno_gLunoNil_F1C8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFloatRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_F390(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_F390(unsigned i) { if (i == 0) return 0; return refToPtr_F390(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFloatRailNode_cFloatRailNode_F390(void*, void*, void*) __asm__("cFloatRailNode_cFloatRailNode");
extern char D_00489768_F390[] __asm__("D_00489768");
extern t3Vector luno_gLunoNil_F390 __asm__("luno_gLunoNil");
extern tMakeFloatRailData sFloatRailDefaults __asm__("D_004FBA38");
extern int D_004A5634;
extern const int MakeFloatRailFunc_types[];
extern "C" void* MEM_copy_F390(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[49]): hangs a world instance on up to eight others as a floating rail (a new
 * FloatRail node), unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tMakeFloatRailData:
 * InstId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, Mesh1, Mesh2, Mesh3, Mesh4, Mesh5,
 * Mesh6, Mesh7, Mesh8, Flags, LSpring, LDamp, ForceScale) and lets each argument override the field its index names,
 * turning an int into a float where the field type table MakeFloatRailFunc_types says eLunoVar_Float. Defaults: InstId
 * -1, InstId1 -1, InstId2 -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7 -1, InstId8 -1, Mesh1 -1, Mesh2
 * -1, Mesh3 -1, Mesh4 -1, Mesh5 -1, Mesh6 -1, Mesh7 -1, Mesh8 -1, Flags 1, LSpring 0.5, LDamp 0.5, ForceScale 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 20 = ATYPE_FLOATRAIL, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cFloatRailNode (cFloatRailNode_cFloatRailNode, cMemMan_alloc
 * 0x80, "FloatRail") on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeFloatRailFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5634 == 0) {
        sFloatRailDefaults.InstId = 0xFFFFFFFF;
        sFloatRailDefaults.InstId1 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId2 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId3 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId4 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId5 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId6 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId7 = 0xFFFFFFFF;
        sFloatRailDefaults.InstId8 = 0xFFFFFFFF;
        sFloatRailDefaults.Mesh1 = -1;
        sFloatRailDefaults.Mesh2 = -1;
        sFloatRailDefaults.Mesh3 = -1;
        sFloatRailDefaults.Mesh4 = -1;
        sFloatRailDefaults.Mesh5 = -1;
        sFloatRailDefaults.Mesh6 = -1;
        sFloatRailDefaults.Mesh7 = -1;
        sFloatRailDefaults.Mesh8 = -1;
        sFloatRailDefaults.Flags = 1;
        sFloatRailDefaults.LSpring = 0.5f;
        sFloatRailDefaults.LDamp = 0.5f;
        sFloatRailDefaults.ForceScale = 1.0f;
        D_004A5634 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sFloatRailDefaults.null = WScriptNullType(sFloatRailDefaults.null);
    }
    unsigned buf64[22];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    buf64[3] = 0xFFFFFFFF;
    buf64[4] = 0xFFFFFFFF;
    buf64[5] = 0xFFFFFFFF;
    buf64[6] = 0xFFFFFFFF;
    buf64[7] = 0xFFFFFFFF;
    buf64[8] = 0xFFFFFFFF;
    MEM_copy_F390(buf, &sFloatRailDefaults, sizeof(tMakeFloatRailData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeFloatRailFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_F390(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_F390;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_FLOATRAIL, buf))
        return luno_gLunoNil_F390;
    cFloatRailNode_cFloatRailNode_F390(cMemMan_alloc(0x80, D_00489768_F390, 0x20000000, 0), obj, buf);
    return luno_gLunoNil_F390;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeSpringRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_F5E8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_F5E8(unsigned i) { if (i == 0) return 0; return refToPtr_F5E8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cSpringRailNode_cSpringRailNode_F5E8(void*, void*, void*) __asm__("cSpringRailNode_cSpringRailNode");
extern char D_00489778_F5E8[] __asm__("D_00489778");
extern t3Vector luno_gLunoNil_F5E8 __asm__("luno_gLunoNil");
extern tMakeSpringRailData sSpringRailDefaults __asm__("D_004FBA90");
extern int D_004A5638;
extern const int MakeSpringRailFunc_types[];
extern "C" void* MEM_copy_F5E8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[70]): hangs a world instance on up to eight others as a springy rail (a new
 * SpringRail node), unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tMakeSpringRailData:
 * InstId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, Mesh1, Mesh2, Mesh3, Mesh4, Mesh5,
 * Mesh6, Mesh7, Mesh8, Flags, ASpring, ADamp, TorqueScale) and lets each argument override the field its index names,
 * turning an int into a float where the field type table MakeSpringRailFunc_types says eLunoVar_Float. Defaults: InstId
 * -1, InstId1 -1, InstId2 -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7 -1, InstId8 -1, Mesh1 -1, Mesh2
 * -1, Mesh3 -1, Mesh4 -1, Mesh5 -1, Mesh6 -1, Mesh7 -1, Mesh8 -1, Flags 1, ASpring 0.5, ADamp 0.5, TorqueScale 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is kept by checkActiveNode(node, 21 = ATYPE_SPRINGRAIL, block), which keeps a busy or same-type node
 * and replaces some kinds itself; otherwise it builds a cSpringRailNode (cSpringRailNode_cSpringRailNode, cMemMan_alloc
 * 0x80, "SpringRail") on the instance from the block, after scaling TorqueScale by 0.001.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MakeSpringRailFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5638 == 0) {
        sSpringRailDefaults.InstId = 0xFFFFFFFF;
        sSpringRailDefaults.InstId1 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId2 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId3 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId4 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId5 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId6 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId7 = 0xFFFFFFFF;
        sSpringRailDefaults.InstId8 = 0xFFFFFFFF;
        sSpringRailDefaults.Mesh1 = -1;
        sSpringRailDefaults.Mesh2 = -1;
        sSpringRailDefaults.Mesh3 = -1;
        sSpringRailDefaults.Mesh4 = -1;
        sSpringRailDefaults.Mesh5 = -1;
        sSpringRailDefaults.Mesh6 = -1;
        sSpringRailDefaults.Mesh7 = -1;
        sSpringRailDefaults.Mesh8 = -1;
        sSpringRailDefaults.Flags = 1;
        sSpringRailDefaults.ASpring = 0.5f;
        sSpringRailDefaults.ADamp = 0.5f;
        sSpringRailDefaults.TorqueScale = 1.0f;
        D_004A5638 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sSpringRailDefaults.null = WScriptNullType(sSpringRailDefaults.null);
    }
    unsigned buf64[22];
    unsigned *buf = buf64;
    tMakeSpringRailData *data = (tMakeSpringRailData*)buf;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    buf64[3] = 0xFFFFFFFF;
    buf64[4] = 0xFFFFFFFF;
    buf64[5] = 0xFFFFFFFF;
    buf64[6] = 0xFFFFFFFF;
    buf64[7] = 0xFFFFFFFF;
    buf64[8] = 0xFFFFFFFF;
    MEM_copy_F5E8(buf, &sSpringRailDefaults, sizeof(tMakeSpringRailData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = MakeSpringRailFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_F5E8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_F5E8;
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode && checkActiveNode(activeNode, ATYPE_SPRINGRAIL, buf))
        return luno_gLunoNil_F5E8;
    data->TorqueScale *= 0.0010000000474974513f;
    cSpringRailNode_cSpringRailNode_F5E8(cMemMan_alloc(0x80, D_00489778_F5E8, 0x20000000, 0), obj, buf);
    return luno_gLunoNil_F5E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AIEventFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data that flags a world event on a rider (On Tour tAIEventData; same offsets): the block below fills
// it.
struct tAIEventData {
    int Being;                      // 0x00 [O]
    int EventId;                    // 0x04 [O]
    float Arg;                      // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tAIEventData, Being, 0x00);
CHECK_OFFSET(tAIEventData, EventId, 0x04);
CHECK_OFFSET(tAIEventData, Arg, 0x08);
CHECK_OFFSET(tAIEventData, null, 0x0C);
CHECK_SIZE(tAIEventData, 0x10);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_F850 __asm__("luno_gLunoNil");

extern int D_00446530_F850[] __asm__("D_00446530");
extern "C" void cRider_flagEventWorld(cRider*, int, float);
extern int D_004A563C_F850 __asm__("D_004A563C");
extern tAIEventData sAIEventDefaults __asm__("D_004FBAE8");
/**
 * Script function (gLunoCFunctions[27]): flags a world event on a rider (cRider_flagEventWorld).
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAIEventData: Being,
 * EventId, Arg) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_00446530 says eLunoVar_Float. Defaults: Being -1, EventId 0, Arg 0.
 *
 * Being -1 means the script's rider (cWScriptMan_TheWScriptMan->Being). Calls
 * cRider_flagEventWorld(SSXApp->Game->AI->Riders[Being], EventId, Arg); the rider slot is not checked.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar AIEventFunc(int n, luno_cLunoCArg* params) {
    if (D_004A563C_F850 == 0) {
        sAIEventDefaults.Being = -1;
        D_004A563C_F850 = 1;
        sAIEventDefaults.EventId = 0;
        sAIEventDefaults.Arg = 0.0f;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAIEventDefaults.null = WScriptNullType(sAIEventDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    tAIEventData *data = (tAIEventData*)vp;
    MEM_copy(vp, &sAIEventDefaults, sizeof(tAIEventData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_00446530_F850[k]);
    }
    if (data->Being == kWScriptBeing_None)
        data->Being = cWScriptMan_TheWScriptMan->Being;
    cGame* game = SSXApp->Game;
    cAI* ai = game->AI;
    cRider_flagEventWorld(ai->Riders[data->Being], data->EventId, data->Arg);
    return luno_gLunoNil_F850;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SignalFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_F9A8(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

extern t3Vector luno_gLunoNil_F9A8 __asm__("luno_gLunoNil");
// World-script data that signals an instance's node (On Tour tSignalData; same offsets): the block below fills it.
struct tSignalData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Signal;                     // 0x04 [O]
    float Arg;                      // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tSignalData, InstId, 0x00);
CHECK_OFFSET(tSignalData, Signal, 0x04);
CHECK_OFFSET(tSignalData, Arg, 0x08);
CHECK_OFFSET(tSignalData, null, 0x0C);
CHECK_SIZE(tSignalData, 0x10);
extern tSignalData sSignalDefaults __asm__("D_004FBAF8");
extern int D_004A5640;
extern const int D_00446540[];
extern "C" void* MEM_copy_F9A8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[28]): sends a signal to a world instance's active node.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tSignalData: InstId,
 * Signal, Arg) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_00446540 says eLunoVar_Float. Defaults: InstId -1, Signal -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (m_pActiveNode), its virtual
 * signal (cInstanceNode_signal, slot 36) gets Signal and Arg.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector SignalFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5640 == 0) {
        sSignalDefaults.InstId = 0xFFFFFFFF;
        sSignalDefaults.Signal = -1;
        sSignalDefaults.Arg = 0.0f;
        D_004A5640 = 1;
        sSignalDefaults.null = WScriptNullType(sSignalDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    tSignalData *data = (tSignalData*)buf;
    data->InstId = 0xFFFFFFFF;
    MEM_copy_F9A8(buf, &sSignalDefaults, sizeof(tSignalData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446540[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    tInstanceRecord *obj = look_F9A8(data->InstId);
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode)
        activeNode->signal(data->Signal, data->Arg);
    return luno_gLunoNil_F9A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", HideInstanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A5648_FB50 __asm__("D_004A5648");
extern WScriptNullType D_004A564C_FB50 __asm__("D_004A564C");
extern int D_004A5650_FB50 __asm__("D_004A5650");
extern int HideInstanceFunc_types[2];
static inline tInstanceRecord *refToPtr_FB50(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_FB50(unsigned i) { if (i == 0) return 0; return refToPtr_FB50(i); }
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cHideNode_cHideNode_FB50(void*, void*) __asm__("cHideNode_cHideNode");
extern char D_004A3C18_FB50[] __asm__("D_004A3C18");
extern t3Vector luno_gLunoNil_FB50 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_FB50(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[29]): hides a world instance by giving it a new Hide node.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tHideInstanceData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table HideInstanceFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (m_pActiveNode) is already ATYPE_HIDE (16) or ATYPE_DEAD (6) (virtual isType); any other node is deleted (virtual
 * slot 1, flags 3). Then a cHideNode (cHideNode_cHideNode, cMemMan_alloc 0x1C, "Hide") is built on the instance.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector HideInstanceFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5650_FB50 == 0) {
        D_004A5648_FB50 = 0xFFFFFFFF;
        D_004A5650_FB50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A564C_FB50 = WScriptNullType(D_004A564C_FB50);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_FB50(buf, &D_004A5648_FB50, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = HideInstanceFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_FB50(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_FB50;
    cInstanceNode* activeNode = obj->m_pActiveNode;
    if (activeNode) {
        if (activeNode->isType(16) || activeNode->isType(6))
            return luno_gLunoNil_FB50;
        delete activeNode;
    }
    cHideNode_cHideNode_FB50(cMemMan_alloc(0x1C, D_004A3C18_FB50, 0x20000000, 0), obj);
    return luno_gLunoNil_FB50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ShowInstanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_FD58(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

extern t3Vector luno_gLunoNil_FD58 __asm__("luno_gLunoNil");
extern unsigned D_FD58 __asm__("D_004A5658");
extern WScriptNullType D_004A565C;
extern int D_004A5660;
extern int D_004A3C20;
extern "C" void* MEM_copy_FD58(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[58]): shows a hidden world instance again by deleting its Hide node.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tShowInstanceData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C20 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance exists and its active node (m_pActiveNode) is
 * ATYPE_HIDE (16, virtual isType), that node is deleted (virtual slot 1, flags 3).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector ShowInstanceFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5660 == 0) {
        D_FD58 = 0xFFFFFFFF;
        D_004A5660 = 1;
        D_004A565C = WScriptNullType(D_004A565C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_FD58(buf, &D_FD58, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C20)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    tInstanceRecord* obj = (tInstanceRecord *)look_FD58(buf[0]);
    if (obj) {
        cInstanceNode* activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isType(0x10))
                delete activeNode;
        }
    }
    return luno_gLunoNil_FD58;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", PlayAudioFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_FF00(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_FF00(unsigned i) { if (i == 0) return 0; return refToPtr_FF00(i); }
extern "C" void *cAudioInterface_bxAudio_FF00(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_WorldSound_FF00(void*, void*, int) __asm__("cSSXAudio_WorldSound");
extern t3Vector luno_gLunoNil_FF00 __asm__("luno_gLunoNil");
// World-script data that plays a sound on an instance (On Tour tPlayAudioData; same offsets): the block below fills it.
struct tPlayAudioData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    unsigned int SoundId;           // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tPlayAudioData, InstId, 0x00);
CHECK_OFFSET(tPlayAudioData, SoundId, 0x04);
CHECK_OFFSET(tPlayAudioData, null, 0x08);
CHECK_SIZE(tPlayAudioData, 0xC);
extern tPlayAudioData sPlayAudioDefaults __asm__("D_004FBB08");
extern int PlayAudioFunc_initFlag;
extern int PlayAudioFunc_types[2];
extern "C" void* MEM_copy_FF00(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[30]): plays a world sound at a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tPlayAudioData: InstId,
 * SoundId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table PlayAudioFunc_types says eLunoVar_Float. Defaults: InstId -1, SoundId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_WorldSound(cAudioInterface_bxAudio(), instance, SoundId) plays it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector PlayAudioFunc(int n, luno_cLunoCArg *params) {
    if (PlayAudioFunc_initFlag == 0) {
        sPlayAudioDefaults.InstId = 0xFFFFFFFF;
        sPlayAudioDefaults.SoundId = 0xFFFFFFFF;
        PlayAudioFunc_initFlag = 1;
        sPlayAudioDefaults.null = WScriptNullType(sPlayAudioDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_FF00(buf, &sPlayAudioDefaults, sizeof(tPlayAudioData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = PlayAudioFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_FF00(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj) {
        int soundId = buf[1];
        cSSXAudio_WorldSound_FF00(cAudioInterface_bxAudio_FF00(), obj, soundId);
    }
    return luno_gLunoNil_FF00;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AttachAudioFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_00A8(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_00A8(unsigned i) { if (i == 0) return 0; return refToPtr_00A8(i); }
extern "C" void *cAudioInterface_bxAudio_00A8(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_AttachSound_00A8(void*, void*, int, int) __asm__("cSSXAudio_AttachSound");
extern t3Vector luno_gLunoNil_00A8 __asm__("luno_gLunoNil");
// World-script data that attaches a sound to an instance (On Tour tAttachAudioData; same offsets): the block below
// fills it.
struct tAttachAudioData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    unsigned int SoundId;           // 0x04 [O]
    int Mesh;                       // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tAttachAudioData, InstId, 0x00);
CHECK_OFFSET(tAttachAudioData, SoundId, 0x04);
CHECK_OFFSET(tAttachAudioData, Mesh, 0x08);
CHECK_OFFSET(tAttachAudioData, null, 0x0C);
CHECK_SIZE(tAttachAudioData, 0x10);
extern tAttachAudioData sAttachAudioDefaults __asm__("D_004FBB18");
extern int AttachAudioFunc_initFlag;
extern const int AttachAudioFunc_types[];
extern "C" void* MEM_copy_00A8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[31]): attaches a looping sound to a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAttachAudioData:
 * InstId, SoundId, Mesh) and lets each argument override the field its index names, turning an int into a float where
 * the field type table AttachAudioFunc_types says eLunoVar_Float. Defaults: InstId -1, SoundId -1, Mesh -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_AttachSound(cAudioInterface_bxAudio(), instance, SoundId, Mesh) attaches it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AttachAudioFunc(int n, luno_cLunoCArg *params) {
    if (AttachAudioFunc_initFlag == 0) {
        sAttachAudioDefaults.InstId = 0xFFFFFFFF;
        sAttachAudioDefaults.SoundId = 0xFFFFFFFF;
        sAttachAudioDefaults.Mesh = -1;
        AttachAudioFunc_initFlag = 1;
        sAttachAudioDefaults.null = WScriptNullType(sAttachAudioDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_00A8(buf, &sAttachAudioDefaults, sizeof(tAttachAudioData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = AttachAudioFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_00A8(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj) {
        int soundId = buf[1];
        int meshIndex = buf[2];
        cSSXAudio_AttachSound_00A8(cAudioInterface_bxAudio_00A8(), obj, soundId, meshIndex);
    }
    return luno_gLunoNil_00A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DetachAudioFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_0260(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_0260(unsigned i) { if (i == 0) return 0; return refToPtr_0260(i); }
extern "C" void *cAudioInterface_bxAudio_0260(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_DetachSound_0260(void*, void*, int, int) __asm__("cSSXAudio_DetachSound");
extern t3Vector luno_gLunoNil_0260 __asm__("luno_gLunoNil");
// World-script data that detaches a sound from an instance (On Tour tDetachAudioData; same offsets): the block below
// fills it.
struct tDetachAudioData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    unsigned int SoundId;           // 0x04 [O]
    int Mesh;                       // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tDetachAudioData, InstId, 0x00);
CHECK_OFFSET(tDetachAudioData, SoundId, 0x04);
CHECK_OFFSET(tDetachAudioData, Mesh, 0x08);
CHECK_OFFSET(tDetachAudioData, null, 0x0C);
CHECK_SIZE(tDetachAudioData, 0x10);
extern tDetachAudioData sDetachAudioDefaults __asm__("D_004FBB28");
extern int DetachAudioFunc_initFlag;
extern const int DetachAudioFunc_types[];
extern "C" void* MEM_copy_0260(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[73]): detaches a sound from a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tDetachAudioData:
 * InstId, SoundId, Mesh) and lets each argument override the field its index names, turning an int into a float where
 * the field type table DetachAudioFunc_types says eLunoVar_Float. Defaults: InstId -1, SoundId -1, Mesh -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_DetachSound(cAudioInterface_bxAudio(), instance, SoundId, Mesh) detaches it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector DetachAudioFunc(int n, luno_cLunoCArg *params) {
    if (DetachAudioFunc_initFlag == 0) {
        sDetachAudioDefaults.InstId = 0xFFFFFFFF;
        sDetachAudioDefaults.SoundId = 0xFFFFFFFF;
        sDetachAudioDefaults.Mesh = -1;
        DetachAudioFunc_initFlag = 1;
        sDetachAudioDefaults.null = WScriptNullType(sDetachAudioDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0260(buf, &sDetachAudioDefaults, sizeof(tDetachAudioData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = DetachAudioFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_0260(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj) {
        int soundId = buf[1];
        int meshIndex = buf[2];
        cSSXAudio_DetachSound_0260(cAudioInterface_bxAudio_0260(), obj, soundId, meshIndex);
    }
    return luno_gLunoNil_0260;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ExpireFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_0418 __asm__("luno_gLunoNil");
// World-script data that expires an instance's node (On Tour tExpireData; same offsets): the block below fills it.
struct tExpireData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Type;                       // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tExpireData, InstId, 0x00);
CHECK_OFFSET(tExpireData, Type, 0x04);
CHECK_OFFSET(tExpireData, null, 0x08);
CHECK_SIZE(tExpireData, 0xC);
extern tExpireData sExpireDefaults __asm__("D_004FBB38");
extern int D_004A5670;
extern int D_004A3C30;
extern "C" void* MEM_copy_0418(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[32]): expires a world instance's active node.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tExpireData: InstId,
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C30 says eLunoVar_Float. Defaults: InstId -1, Type 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (m_pActiveNode), Type
 * kExpire_Silent (0) or kExpire_RunScript (1) is passed to its virtual expire (cInstanceNode_expire, slot 34) as its
 * runscript flag; other types do nothing.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector ExpireFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5670 == 0) {
        sExpireDefaults.InstId = 0xFFFFFFFF;
        sExpireDefaults.Type = 0;
        D_004A5670 = 1;
        sExpireDefaults.null = WScriptNullType(sExpireDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0418(buf, &sExpireDefaults, sizeof(tExpireData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C30)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode) {
        int expireType = buf[1];
        if (expireType == kExpire_Silent)
            activeNode->expire(kExpire_Silent);
        else if (expireType == kExpire_RunScript)
            activeNode->expire(kExpire_RunScript);
    }
    return luno_gLunoNil_0418;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", LoseFocusFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_05E8 __asm__("luno_gLunoNil");
extern unsigned D_05E8 __asm__("D_004A5678");
extern WScriptNullType D_004A567C;
extern int D_004A5680;
extern int D_004A3C38;
extern "C" void* MEM_copy_05E8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[33]): tells a world instance's active node that it lost focus.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tLoseFocusData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C38 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (m_pActiveNode), its virtual
 * loseFocus (cInstanceNode_loseFocus, slot 35) runs.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector LoseFocusFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5680 == 0) {
        D_05E8 = 0xFFFFFFFF;
        D_004A5680 = 1;
        D_004A567C = WScriptNullType(D_004A567C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_05E8(buf, &D_05E8, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C38)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable **tb = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable;
        cWorldCacheTable *section = tb[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode)
        activeNode->loseFocus();
    return luno_gLunoNil_05E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", TransportFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A5688_0770 __asm__("D_004A5688");
extern WScriptNullType D_004A568C_0770 __asm__("D_004A568C");
extern int D_004A5690_0770 __asm__("D_004A5690");
extern int TransportFunc_types[2];
static inline tInstanceRecord *refToPtr_0770(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_0770(unsigned i) { if (i == 0) return 0; return refToPtr_0770(i); }
extern t3Vector luno_gLunoNil_0770 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_0770(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[34]): transports the script's rider to a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tTransportData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * TransportFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the script's rider index (cWScriptMan_TheWScriptMan->Being)
 * is not negative and the instance is found, the target transform is the instance node's virtual getRootTransform (slot
 * 24) or, without a node, the instance's own transform (m_fmTransform); the rider (SSXApp->Game->AI->Riders[Being])
 * gets it through its virtual transport (cRider_transport, slot 10).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector TransportFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5690_0770 == 0) {
        D_004A5688_0770 = 0xFFFFFFFF;
        D_004A5690_0770 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A568C_0770 = WScriptNullType(D_004A568C_0770);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0770(buf, &D_004A5688_0770, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = TransportFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_0770(i);
            inst = found;
        }
        obj = inst;
    }
    int k = cWScriptMan_TheWScriptMan->Being;
    if (k >= 0 && obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        const t4Matrix *rootTransform = (const t4Matrix*)&obj->m_fmTransform;
        if (activeNode)
            rootTransform = activeNode->getRootTransform();
        cAI *ai = SSXApp->Game->AI;
        ai->Riders[k]->transport((t4Matrix*)rootTransform);
    }
    return luno_gLunoNil_0770;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetupMissionRiderFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A5698_0948 __asm__("D_004A5698");
extern WScriptNullType D_004A569C_0948 __asm__("D_004A569C");
extern int D_004A56A0_0948 __asm__("D_004A56A0");
extern int SetupMissionRiderFunc_types[2];
static inline tInstanceRecord *refToPtr_0948(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_0948(unsigned i) { if (i == 0) return 0; return refToPtr_0948(i); }
extern t3Vector luno_gLunoNil_0948 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_0948(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[64]): sets the script's rider up for a mission at a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tSetupMissionRiderData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table SetupMissionRiderFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the script's rider index (cWScriptMan_TheWScriptMan->Being)
 * is not negative and the instance is found, the transform is the instance node's virtual getRootTransform (slot 24)
 * or, without a node, the instance's own transform (m_fmTransform); the rider (SSXApp->Game->AI->Riders[Being]) gets it
 * through its virtual initForMission (cRider_initForMission, slot 12).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector SetupMissionRiderFunc(int n, luno_cLunoCArg *params) {
    if (D_004A56A0_0948 == 0) {
        D_004A5698_0948 = 0xFFFFFFFF;
        D_004A56A0_0948 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A569C_0948 = WScriptNullType(D_004A569C_0948);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0948(buf, &D_004A5698_0948, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = SetupMissionRiderFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_0948(i);
            inst = found;
        }
        obj = inst;
    }
    int k = cWScriptMan_TheWScriptMan->Being;
    if (k >= 0 && obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        const t4Matrix *rootTransform = (const t4Matrix*)&obj->m_fmTransform;
        if (activeNode)
            rootTransform = activeNode->getRootTransform();
        cAI *ai = SSXApp->Game->AI;
        ai->Riders[k]->initForMission((t4Matrix*)rootTransform);
    }
    return luno_gLunoNil_0948;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of RailFunc (On Tour tRailData; same offsets): the block below fills it.
struct tRailData {
    unsigned int SplineId;          // 0x00 [O] tResourceID
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tRailData, SplineId, 0x00);
CHECK_OFFSET(tRailData, Cmd, 0x04);
CHECK_OFFSET(tRailData, null, 0x08);
CHECK_SIZE(tRailData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_0B20 __asm__("luno_gLunoNil");

extern int D_004A3C50_0B20[2] __asm__("D_004A3C50");
extern "C" void cRailManNode_addRail(unsigned, int);
extern int D_004A56A4_0B20 __asm__("D_004A56A4");
extern tRailData sRailCmdDefaults __asm__("D_004FBB48");
/**
 * Script function (gLunoCFunctions[35]): switches a rail (spline) on or off.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tRailData: SplineId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C50 says eLunoVar_Float. Defaults: SplineId -1, Cmd 1.
 *
 * When SplineId is not -1, Cmd kRailCmd_On switches the rail on and kRailCmd_Off off through cRailManNode_addRail (the
 * rail manager); other values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar RailFunc(int n, luno_cLunoCArg* params) {
    if (D_004A56A4_0B20 == 0) {
        sRailCmdDefaults.SplineId = -1;
        sRailCmdDefaults.Cmd = 1;
        D_004A56A4_0B20 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sRailCmdDefaults.null = WScriptNullType(sRailCmdDefaults.null);
    }
    unsigned block[4] __attribute__((aligned(16)));
    unsigned* vp = block;
    vp[0] = 0xFFFFFFFFu;
    MEM_copy(vp, &sRailCmdDefaults, sizeof(tRailData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3C50_0B20[k]);
    }
    if (~vp[0] != 0) {
        int y = vp[1];
        if (y == kRailCmd_On)
            cRailManNode_addRail(vp[0], kRailCmd_On);
        else if (y == kRailCmd_Off)
            cRailManNode_addRail(vp[0], kRailCmd_Off);
    }
    return luno_gLunoNil_0B20;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", NISFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_0C78 __asm__("luno_gLunoNil");
// World-script data that queues or plays an NIS (On Tour tNISData; same offsets): the block below fills it.
struct tNISData {
    unsigned int ScriptId;          // 0x00 [O]
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tNISData, ScriptId, 0x00);
CHECK_OFFSET(tNISData, Cmd, 0x04);
CHECK_OFFSET(tNISData, null, 0x08);
CHECK_SIZE(tNISData, 0xC);
extern tNISData sNISDefaults __asm__("D_004FBB58");
extern int D_004A56A8;
extern int D_004A3C58;
extern "C" void* MEM_copy_0C78(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[36]): queues or plays an in-game cinematic (NIS) through the script engine.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tNISData: ScriptId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C58 says eLunoVar_Float. Defaults: ScriptId -1.
 *
 * Cmd kNISCmd_QueueScript adds the NIS whose script hash is ScriptId to list 0
 * (cSSXScriptEngine_AddToNISListByScriptHashID, mode 3) and readies the list (cSSXScriptEngine_ReadyNISList);
 * kNISCmd_QueueCategory adds the NIS of script category ScriptId instead
 * (cSSXScriptEngine_AddToNISListByScriptCategory) and readies it; kNISCmd_Play plays list 0
 * (cSSXScriptEngine_PlayNISList). Other values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector NISFunc(int n, luno_cLunoCArg *params) {
    if (D_004A56A8 == 0) {
        sNISDefaults.ScriptId = 0xFFFFFFFF;
        sNISDefaults.Cmd = 0;
        D_004A56A8 = 1;
        sNISDefaults.null = WScriptNullType(sNISDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    tNISData *data = (tNISData*)buf;
    MEM_copy_0C78(buf, &sNISDefaults, sizeof(tNISData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C58)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = data->ScriptId;
    switch (data->Cmd) {
    case kNISCmd_QueueScript:
        ScriptEngine->AddToNISListByScriptHashID(0, id, 3, -1, -1, -1);
        ScriptEngine->ReadyNISList(0);
        break;
    case kNISCmd_QueueCategory:
        ScriptEngine->AddToNISListByScriptCategory(0, id, 3, 0, -1, -1, -1);
        ScriptEngine->ReadyNISList(0);
        break;
    case kNISCmd_Play:
        ScriptEngine->PlayNISList(0, 1, 0);
        break;
    }
    return luno_gLunoNil_0C78;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectPoolFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of CollectPoolFunc (On Tour tCollectPoolData; same offsets): the block below fills it.
struct tCollectPoolData {
    unsigned int PoolId;            // 0x00 [O]
    int PoolSize;                   // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tCollectPoolData, PoolId, 0x00);
CHECK_OFFSET(tCollectPoolData, PoolSize, 0x04);
CHECK_OFFSET(tCollectPoolData, null, 0x08);
CHECK_SIZE(tCollectPoolData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
extern luno_cLunoVar luno_gLunoNil_0E28 __asm__("luno_gLunoNil");

extern int D_004A3C60_0E28[2] __asm__("D_004A3C60");
extern "C" void cWScriptMan_collectPool(cWScriptMan*, unsigned, int);
extern int D_004A56AC_0E28 __asm__("D_004A56AC");
extern tCollectPoolData sCollectPoolDefaults __asm__("D_004FBB68");
/**
 * Script function (gLunoCFunctions[37]): sets up a collection pool (cWScriptMan_collectPool).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCollectPoolData:
 * PoolId, PoolSize) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3C60 says eLunoVar_Float. Defaults: PoolId -1, PoolSize 1.
 *
 * When PoolId is not -1, cWScriptMan_collectPool(cWScriptMan_TheWScriptMan, PoolId, PoolSize) runs.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar CollectPoolFunc(int n, luno_cLunoCArg* params) {
    if (D_004A56AC_0E28 == 0) {
        sCollectPoolDefaults.PoolId = 0xFFFFFFFFu;
        sCollectPoolDefaults.PoolSize = 1;
        D_004A56AC_0E28 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sCollectPoolDefaults.null = WScriptNullType(sCollectPoolDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sCollectPoolDefaults, sizeof(tCollectPoolData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3C60_0E28[k]);
    }
    unsigned x = vp[0];
    if (x != kWScriptId_None)
        cWScriptMan_collectPool(cWScriptMan_TheWScriptMan, x, vp[1]);
    return luno_gLunoNil_0E28;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectInstFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_0F50 __asm__("luno_gLunoNil");
// World-script data that fills a collect pool (On Tour tCollectInstData; same offsets): the block below fills it.
struct tCollectInstData {
    unsigned int PoolId;            // 0x00 [O]
    unsigned int InstId1;           // 0x04 [O] tResourceID
    unsigned int InstId2;           // 0x08 [O] tResourceID
    unsigned int InstId3;           // 0x0C [O] tResourceID
    unsigned int InstId4;           // 0x10 [O] tResourceID
    unsigned int InstId5;           // 0x14 [O] tResourceID
    unsigned int InstId6;           // 0x18 [O] tResourceID
    unsigned int InstId7;           // 0x1C [O] tResourceID
    unsigned int InstId8;           // 0x20 [O] tResourceID
    unsigned int InstId9;           // 0x24 [O] tResourceID
    unsigned int InstId10;          // 0x28 [O] tResourceID
    unsigned int InstId11;          // 0x2C [O] tResourceID
    unsigned int InstId12;          // 0x30 [O] tResourceID
    unsigned int InstId13;          // 0x34 [O] tResourceID
    unsigned int InstId14;          // 0x38 [O] tResourceID
    unsigned int InstId15;          // 0x3C [O] tResourceID
    unsigned int InstId16;          // 0x40 [O] tResourceID
    unsigned int InstId17;          // 0x44 [O] tResourceID
    unsigned int InstId18;          // 0x48 [O] tResourceID
    unsigned int InstId19;          // 0x4C [O] tResourceID
    unsigned int InstId20;          // 0x50 [O] tResourceID
    WScriptNullType null;           // 0x54 [O]
    char unk_055[0x3];              // 0x55 padding
};

CHECK_OFFSET(tCollectInstData, PoolId, 0x00);
CHECK_OFFSET(tCollectInstData, InstId1, 0x04);
CHECK_OFFSET(tCollectInstData, InstId2, 0x08);
CHECK_OFFSET(tCollectInstData, InstId3, 0x0C);
CHECK_OFFSET(tCollectInstData, InstId4, 0x10);
CHECK_OFFSET(tCollectInstData, InstId5, 0x14);
CHECK_OFFSET(tCollectInstData, InstId6, 0x18);
CHECK_OFFSET(tCollectInstData, InstId7, 0x1C);
CHECK_OFFSET(tCollectInstData, InstId8, 0x20);
CHECK_OFFSET(tCollectInstData, InstId9, 0x24);
CHECK_OFFSET(tCollectInstData, InstId10, 0x28);
CHECK_OFFSET(tCollectInstData, InstId11, 0x2C);
CHECK_OFFSET(tCollectInstData, InstId12, 0x30);
CHECK_OFFSET(tCollectInstData, InstId13, 0x34);
CHECK_OFFSET(tCollectInstData, InstId14, 0x38);
CHECK_OFFSET(tCollectInstData, InstId15, 0x3C);
CHECK_OFFSET(tCollectInstData, InstId16, 0x40);
CHECK_OFFSET(tCollectInstData, InstId17, 0x44);
CHECK_OFFSET(tCollectInstData, InstId18, 0x48);
CHECK_OFFSET(tCollectInstData, InstId19, 0x4C);
CHECK_OFFSET(tCollectInstData, InstId20, 0x50);
CHECK_OFFSET(tCollectInstData, null, 0x54);
CHECK_SIZE(tCollectInstData, 0x58);
extern tCollectInstData sCollectInstDefaults __asm__("D_004FBB78");
extern int D_56B0_0F50 __asm__("D_004A56B0");
extern const int CollectInstFunc_types_0F50[] __asm__("CollectInstFunc_types");
extern "C" void* MEM_copy_0F50(void*, void*, int) __asm__("MEM_copy");
extern "C" void cWScriptMan_collectInst_0F50(cWScriptMan*, unsigned, unsigned*, int) __asm__("cWScriptMan_collectInst");
/**
 * Script function (gLunoCFunctions[38]): registers up to 20 instances with a collection pool (cWScriptMan_collectInst).
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tCollectInstData:
 * PoolId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, InstId9, InstId10, InstId11,
 * InstId12, InstId13, InstId14, InstId15, InstId16, InstId17, InstId18, InstId19, InstId20) and lets each argument
 * override the field its index names, turning an int into a float where the field type table CollectInstFunc_types says
 * eLunoVar_Float. Defaults: PoolId -1, InstId1 -1, InstId2 -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7
 * -1, InstId8 -1, InstId9 -1, InstId10 -1, InstId11 -1, InstId12 -1, InstId13 -1, InstId14 -1, InstId15 -1, InstId16
 * -1, InstId17 -1, InstId18 -1, InstId19 -1, InstId20 -1.
 *
 * When PoolId is not -1, cWScriptMan_collectInst(cWScriptMan_TheWScriptMan, PoolId, &InstId1, 20) runs.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector CollectInstFunc(int n, luno_cLunoCArg *params) {
    if (D_56B0_0F50 == 0) {
        sCollectInstDefaults.PoolId = 0xFFFFFFFF;
        sCollectInstDefaults.InstId1 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId2 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId3 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId4 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId5 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId6 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId7 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId8 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId9 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId10 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId11 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId12 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId13 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId14 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId15 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId16 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId17 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId18 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId19 = 0xFFFFFFFF;
        sCollectInstDefaults.InstId20 = 0xFFFFFFFF;
        D_56B0_0F50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sCollectInstDefaults.null = WScriptNullType(sCollectInstDefaults.null);
    }
    unsigned buf64[22];
    unsigned *buf = buf64;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    buf64[3] = 0xFFFFFFFF;
    buf64[4] = 0xFFFFFFFF;
    buf64[5] = 0xFFFFFFFF;
    buf64[6] = 0xFFFFFFFF;
    buf64[7] = 0xFFFFFFFF;
    buf64[8] = 0xFFFFFFFF;
    buf64[9] = 0xFFFFFFFF;
    buf64[10] = 0xFFFFFFFF;
    buf64[11] = 0xFFFFFFFF;
    buf64[12] = 0xFFFFFFFF;
    buf64[13] = 0xFFFFFFFF;
    buf64[14] = 0xFFFFFFFF;
    buf64[15] = 0xFFFFFFFF;
    buf64[16] = 0xFFFFFFFF;
    buf64[17] = 0xFFFFFFFF;
    buf64[18] = 0xFFFFFFFF;
    buf64[19] = 0xFFFFFFFF;
    buf64[20] = 0xFFFFFFFF;
    MEM_copy_0F50(buf, &sCollectInstDefaults, sizeof(tCollectInstData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = CollectInstFunc_types_0F50[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    if (buf[0] != kWScriptId_None)
        cWScriptMan_collectInst_0F50(cWScriptMan_TheWScriptMan, buf[0], buf + 1, 20);
    return luno_gLunoNil_0F50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of CollectFunc (On Tour tCollectData; same offsets): the block below fills it.
struct tCollectData {
    unsigned int PoolId;            // 0x00 [O]
    unsigned int InstId;            // 0x04 [O] tResourceID
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tCollectData, PoolId, 0x00);
CHECK_OFFSET(tCollectData, InstId, 0x04);
CHECK_OFFSET(tCollectData, null, 0x08);
CHECK_SIZE(tCollectData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern int D_004A3C68_1120[2] __asm__("D_004A3C68");
extern "C" int cWScriptMan_collect_1120(cWScriptMan*, int, unsigned, unsigned) __asm__("cWScriptMan_collect");
extern int D_004A56B4_1120 __asm__("D_004A56B4");
extern tCollectData sCollectDefaults __asm__("D_004FBBD0");
static inline bool isIdle_1120(int slot) {
    if (slot != -1) {
        cRider *r = SSXApp->Game->AI->Riders[slot];
        cBeing *b = r;
        cRider *d = (cRider*)b;
        return !d->isComputer();
    }
    return false;
}
/**
 * Script command Collect: makes the script's rider collect an item; returns whether it did.
 *
 * Only acts when the script's rider slot (cWScriptMan_TheWScriptMan->Being) is set and that rider
 * (SSXApp->Game->AI->Riders[slot]) is not a computer rider (cBeing virtual isComputer, slot 9, returns false). The
 * defaults (D_004FBBD0, under D_004A56B4: -1, -1) take the keyword arguments (types from D_004A3C68). With an item set,
 * an instance of -1 becomes the current instance's resource ID (cWScriptMan_TheWScriptMan->Inst->m_RID) and
 * cWScriptMan_collect(slot, item, instance) runs; the result is the int 1, otherwise the int 0 (Type, value through
 * luno_cLunoVar_castInt).
 *
 * @param self  the cLunoVar result to fill (g++'s hidden struct-return pointer)
 * @param n     the number of keyword arguments
 * @param params     the keyword arguments (index, value, type)
 * @return @p self
 */
extern "C" void* CollectFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    int slot = cWScriptMan_TheWScriptMan->Being;
    if (isIdle_1120(slot)) {
        if (D_004A56B4_1120 == 0) {
            sCollectDefaults.PoolId = 0xFFFFFFFF;
            sCollectDefaults.InstId = 0xFFFFFFFF;
            D_004A56B4_1120 = 1;
            // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back;
            //       sb 0(sp)).
            sCollectDefaults.null = WScriptNullType(sCollectDefaults.null);
        }
        unsigned argBlock[4] __attribute__((aligned(16)));
        unsigned* vp = argBlock;
        argBlock[1] = 0xFFFFFFFF;
        MEM_copy(vp, &sCollectDefaults, sizeof(tCollectData));
        luno_cLunoCArg* keyArg = params;
        for (int i = 0; i < n; i++, keyArg++) {
            int k = keyArg->Index;
            setArgField(&vp[k], keyArg, D_004A3C68_1120[k]);
        }
        unsigned x = vp[0];
        if (x != kWScriptId_None) {
            if (~vp[1] == 0)
                vp[1] = cWScriptMan_TheWScriptMan->Inst->m_RID;
            unsigned instanceId = vp[1];
            unsigned itemId = vp[0];
            cWScriptMan_collect_1120(cWScriptMan_TheWScriptMan, slot, itemId, instanceId);
            self->Type = 1;
            *luno_cLunoVar_castInt(self) = 1;
            return self;
        }
    }
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MissionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of MissionFunc (On Tour tMissionData; same offsets): the block below fills it.
struct tMissionData {
    unsigned int MissionId;         // 0x00 [O]
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tMissionData, MissionId, 0x00);
CHECK_OFFSET(tMissionData, Cmd, 0x04);
CHECK_OFFSET(tMissionData, null, 0x08);
CHECK_SIZE(tMissionData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_12F0 __asm__("luno_gLunoNil");

extern int D_004A3C70_12F0[2] __asm__("D_004A3C70");
extern "C" int cWScriptMan_doMissionCmd_12F0(cWScriptMan*, unsigned, int, int) __asm__("cWScriptMan_doMissionCmd");
extern int D_004A56B8_12F0 __asm__("D_004A56B8");
extern tMissionData sMissionDefaults __asm__("D_004FBBE0");
/**
 * Script function (gLunoCFunctions[40]): runs a mission command and returns its result (cWScriptMan_doMissionCmd).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMissionData:
 * MissionId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3C70 says eLunoVar_Float. Defaults: MissionId -1, Cmd -1.
 *
 * When MissionId is not -1, the result of cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan, MissionId, Cmd, -1) is
 * returned as an int script value (type 1); otherwise nil. The value is built in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to an int script value, or nil
 */
extern "C" void* MissionFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A56B8_12F0 == 0) {
        sMissionDefaults.MissionId = -1;
        sMissionDefaults.Cmd = -1;
        D_004A56B8_12F0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMissionDefaults.null = WScriptNullType(sMissionDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sMissionDefaults, sizeof(tMissionData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3C70_12F0[k]);
    }
    unsigned x = vp[0];
    if (x != kWScriptId_None) {
        int cmdResult = cWScriptMan_doMissionCmd_12F0(cWScriptMan_TheWScriptMan, x, vp[1], -1);
        self->Type = 1;
        *luno_cLunoVar_castInt(self) = cmdResult;
    } else {
        *(luno_cLunoVar*)self = luno_gLunoNil_12F0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MovieFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct cLunoValue_1440 {
    int word0;
    int word1;
    int type;
};
extern cLunoValue_1440 luno_gLunoNil_1440 __asm__("luno_gLunoNil");
// Reference sets of the current world (wscript/wscriptman.cpp layout). A reference id packs the set index (low 8 bits)
// and the slot; refs[slot] >> 8 is the object address >> 2.
struct sRefSet_1440 {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
extern cWScriptMan* cWScriptMan_TheWScriptMan_1440 __asm__("cWScriptMan_TheWScriptMan");
struct sBindArg_1FBE38_1440 { int idx; int val; int pad; int type; };
struct sBindEnd_1FBE38_1440 {};  // empty trailing member of a defaults record; value-init copies its byte through a
                                 //       stack temp
extern "C" void MEM_copy(void*, void*, int);
struct T12_1440 { unsigned a; int b; sBindEnd_1FBE38_1440 f; };
extern T12_1440 D_004FBBF0_1440 __asm__("D_004FBBF0");
extern int D_004A56BC_1440 __asm__("D_004A56BC");
extern int D_004A3C78_1440[2] __asm__("D_004A3C78");
/**
 * Script function (gLunoCFunctions[41]): looks up a world instance and does nothing else (a stubbed movie command).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMovieData: InstId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C78 says 2. Defaults: InstId -1, Cmd 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The instance is resolved but not used; Cmd is ignored.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
// FAKEMATCH: the dead lookup of the object for the packed reference id buf[0] (`obj`, never used: in retail only its
// `lw; nor` survive, the branch being removed after register allocation; the release build's user was debug code);
// plain form 97.22%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" cLunoValue_1440 MovieFunc(int n, sBindArg_1FBE38_1440* params) {
    if (D_004A56BC_1440 == 0) {
        D_004FBBF0_1440.a = 0xFFFFFFFF;
        // NOTE: guarded static-local init; the trailing empty member's value-init copies its byte through a stack temp
        //       (lbu; sb 0(sp)).
        D_004FBBF0_1440.b = 1;
        D_004FBBF0_1440.f = sBindEnd_1FBE38_1440();
        D_004A56BC_1440 = 1;
    }
    unsigned buf64[3] __attribute__((aligned(16)));
    unsigned* buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy(buf, &D_004FBBF0_1440, 12);
    sBindArg_1FBE38_1440* x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004A3C78_1440[idx];
        unsigned* slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = ((char*)cWScriptMan_TheWScriptMan_1440->Inst);
    } else {
        sRefSet_1440 *section = ((sRefSet_1440*)(cWorld_m_pTheWorld->m_pWorldCache)->m_pSectionCacheTable[id & 0xFF]);
        unsigned i;
        if (!section || (i = section->refs[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    return luno_gLunoNil_1440;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectCountFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_1560 __asm__("luno_gLunoNil");

extern int D_004A3C80_1560[2] __asm__("D_004A3C80");
extern "C" int cWScriptMan_collectCount(cWScriptMan*, unsigned);
extern int D_004A56C8_1560 __asm__("D_004A56C8");
extern unsigned D_004A56C0_1560 __asm__("D_004A56C0");
extern WScriptNullType D_004A56C4_1560 __asm__("D_004A56C4");
/**
 * Script function (gLunoCFunctions[53]): returns how many items of a collection pool are collected
 * (cWScriptMan_collectCount).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tCollectCountData:
 * PoolId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C80 says eLunoVar_Float. Defaults: PoolId -1.
 *
 * Returns cWScriptMan_collectCount(cWScriptMan_TheWScriptMan, PoolId) as an int script value, or int 0 when PoolId is
 * -1. The value is built in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to an int script value
 */
extern "C" void* CollectCountFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A56C8_1560 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A56C4_1560 = WScriptNullType(D_004A56C4_1560);
        D_004A56C0_1560 = -1;
        D_004A56C8_1560 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A56C0_1560, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3C80_1560[k]);
    }
    unsigned x = vp[0];
    if (x != kWScriptId_None) {
        int collected = cWScriptMan_collectCount(cWScriptMan_TheWScriptMan, x);
        self->Type = 1;
        *luno_cLunoVar_castInt(self) = collected;
    } else {
        self->Type = 1;
        *luno_cLunoVar_castInt(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AnimFrameFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_1680(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

float *luno_cLunoVar_castFloat_1680(void*) __asm__("luno_cLunoVar_castFloat__FPv");
extern unsigned D_1680 __asm__("D_004A56D0");
extern WScriptNullType D_004A56D4;
extern int D_004A56D8;
extern int D_004A3C88;
extern "C" void* MEM_copy_1680(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[54]): returns a world instance's current animation frame as a float.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAnimFrameData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C88 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (m_pActiveNode) virtual
 * getAnimFrame (cInstanceNode_getAnimFrame, slot 30), or -1.0 when the instance or its node is missing; it is stored in
 * @p ret as a float script value (eLunoVar_Float).
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" luno_cLunoVar* AnimFrameFunc(luno_cLunoVar *ret, int n, luno_cLunoCArg *params) {
    if (D_004A56D8 == 0) {
        D_1680 = 0xFFFFFFFF;
        D_004A56D8 = 1;
        D_004A56D4 = WScriptNullType(D_004A56D4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_1680(buf, &D_1680, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C88)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    tInstanceRecord *obj = look_1680(buf[0]);
    float animFrame;
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode)
            animFrame = activeNode->getAnimFrame();
        else
            animFrame = -1.0f;
        ret->Type = eLunoVar_Float;
        *luno_cLunoVar_castFloat_1680(ret) = animFrame;
    } else {
        animFrame = -1.0f;
        ret->Type = eLunoVar_Float;
        *luno_cLunoVar_castFloat_1680(ret) = animFrame;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AnimDirFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_1830(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

int *luno_cLunoVar_castInt(void*);
extern unsigned D_1830 __asm__("D_004A56E0");
extern WScriptNullType D_004A56E4;
extern int D_004A56E8;
extern int D_004A3C90;
extern "C" void* MEM_copy_1830(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[109]): returns a world instance's animation direction as an int.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAnimDirData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C90 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (m_pActiveNode) virtual
 * getAnimDir (cInstanceNode_getAnimDir, slot 31), or 0 when the instance or its node is missing; it is stored in @p ret
 * as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" luno_cLunoVar* AnimDirFunc(luno_cLunoVar *ret, int n, luno_cLunoCArg *params) {
    if (D_004A56E8 == 0) {
        D_1830 = 0xFFFFFFFF;
        D_004A56E8 = 1;
        D_004A56E4 = WScriptNullType(D_004A56E4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_1830(buf, &D_1830, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C90)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        int animDir;
        if (activeNode)
            animDir = activeNode->getAnimDir();
        else
            animDir = 0;
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = animDir;
    } else {
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", FrameUpdateEventFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int *luno_cLunoVar_castInt(void*);
// World-script data that sets an instance's frame-update event (On Tour tFrameUpdateEventData; same offsets): the block
// below fills it.
struct tFrameUpdateEventData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    float Frame;                    // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tFrameUpdateEventData, InstId, 0x00);
CHECK_OFFSET(tFrameUpdateEventData, Frame, 0x04);
CHECK_OFFSET(tFrameUpdateEventData, null, 0x08);
CHECK_SIZE(tFrameUpdateEventData, 0xC);
extern tFrameUpdateEventData sFrameUpdateEventDefaults __asm__("D_004FBC00");
extern int D_004A56EC;
extern int D_004A3C98;
extern "C" void* MEM_copy_19C8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[55]): asks a world instance's active node whether its animation has passed a frame
 * (frameUpdateEvent).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tFrameUpdateEventData:
 * InstId, Frame) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3C98 says eLunoVar_Float. Defaults: InstId -1, Frame -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (m_pActiveNode) virtual
 * frameUpdateEvent (cInstanceNode_frameUpdateEvent, slot 32) called with Frame / 30 (frames turned into seconds), or 0
 * when the instance or its node is missing; it is stored in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" luno_cLunoVar* FrameUpdateEventFunc(luno_cLunoVar *ret, int n, luno_cLunoCArg *params) {
    if (D_004A56EC == 0) {
        sFrameUpdateEventDefaults.InstId = 0xFFFFFFFF;
        sFrameUpdateEventDefaults.Frame = -1.0f;
        D_004A56EC = 1;
        sFrameUpdateEventDefaults.null = WScriptNullType(sFrameUpdateEventDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    tFrameUpdateEventData *data = (tFrameUpdateEventData*)buf;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_19C8(buf, &sFrameUpdateEventDefaults, sizeof(tFrameUpdateEventData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3C98)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        float frameArg = data->Frame;
        int eventHit;
        if (activeNode)
            eventHit = activeNode->frameUpdateEvent(frameArg * 0.03333333507180214f);
        else
            eventHit = 0;
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = eventHit;
    } else {
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", GotoTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A56F0_1B88 __asm__("D_004A56F0");
extern WScriptNullType D_004A56F4_1B88 __asm__("D_004A56F4");
extern int D_004A56F8_1B88 __asm__("D_004A56F8");
extern int D_004A3CA0_1B88[2] __asm__("D_004A3CA0");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void cWScriptMan_gotoTask(cWScriptMan*, int);
int* luno_cLunoVar_castInt(void*);


/**
 * Script function (gLunoCFunctions[56]): makes the script's process jump to a task (cWScriptMan_gotoTask).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tGotoTaskData: TaskId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CA0 says eLunoVar_Float. Defaults: TaskId -1.
 *
 * Calls cWScriptMan_gotoTask(cWScriptMan_TheWScriptMan, TaskId), even for the default -1, and returns int 0 in the
 * hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 0
 */
extern "C" void* GotoTaskFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A56F8_1B88 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A56F4_1B88 = WScriptNullType(D_004A56F4_1B88);
        D_004A56F0_1B88 = -1;
        D_004A56F8_1B88 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A56F0_1B88, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CA0_1B88[k]);
    }
    cWScriptMan_gotoTask(cWScriptMan_TheWScriptMan, vp[0]);
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AdvanceToTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A5700_1C80 __asm__("D_004A5700");
extern WScriptNullType D_004A5704_1C80 __asm__("D_004A5704");
extern int D_004A5708_1C80 __asm__("D_004A5708");
extern int D_004A3CA8_1C80[2] __asm__("D_004A3CA8");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void cWScriptMan_advanceToTask(cWScriptMan*, int);
int* luno_cLunoVar_castInt(void*);


/**
 * Script function (gLunoCFunctions[57]): advances the script's process to a task (cWScriptMan_advanceToTask).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAdvanceToTaskData:
 * TaskId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3CA8 says eLunoVar_Float. Defaults: TaskId -1.
 *
 * Calls cWScriptMan_advanceToTask(cWScriptMan_TheWScriptMan, TaskId), even for the default -1, and returns int 0 in the
 * hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 0
 */
extern "C" void* AdvanceToTaskFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A5708_1C80 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5704_1C80 = WScriptNullType(D_004A5704_1C80);
        D_004A5700_1C80 = -1;
        D_004A5708_1C80 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5700_1C80, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CA8_1C80[k]);
    }
    cWScriptMan_advanceToTask(cWScriptMan_TheWScriptMan, vp[0]);
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderInVolumeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A5710_1D78 __asm__("D_004A5710");
extern WScriptNullType D_004A5714_1D78 __asm__("D_004A5714");
extern int D_004A5718_1D78 __asm__("D_004A5718");
extern int RiderInVolumeFunc_types[2];
static inline tInstanceRecord *refToPtr_1D78(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_1D78(unsigned i) { if (i == 0) return 0; return refToPtr_1D78(i); }
extern t3Vector luno_gLunoNil_1D78 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_1D78(void*, void*, int) __asm__("MEM_copy");
int* luno_cLunoVar_castInt_1D78(void*) __asm__("luno_cLunoVar_castInt__FPv");
/**
 * Script function (gLunoCFunctions[65]): returns whether the script's rider has the world instance in its list of
 * visited ids.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderInVolumeData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table RiderInVolumeFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, the script's rider
 * (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) is asked through its virtual slot 3
 * (cRider_isIntersecting: is the instance's id, m_RID, among the rider's 64 mVisitedIds); the int result in the hidden
 * result slot self is 1 when it is, else 0.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 1 or 0
 */
extern "C" void* RiderInVolumeFunc(luno_cLunoVar* self, int n, luno_cLunoCArg *params) {
    if (D_004A5718_1D78 == 0) {
        D_004A5710_1D78 = 0xFFFFFFFF;
        D_004A5718_1D78 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A5714_1D78 = WScriptNullType(D_004A5714_1D78);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_1D78(buf, &D_004A5710_1D78, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = RiderInVolumeFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *instance;
        if (!section)
            instance = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_1D78(i);
            instance = found;
        }
        obj = instance;
    }
    if (obj) {
        int found = 1;
        cAI *ai = SSXApp->Game->AI;
        cRider *riderEntry = ai->Riders[cWScriptMan_TheWScriptMan->Being];
        cBeing *riderPart = riderEntry;
        cRider *rider = (cRider*)riderPart;
        if (rider->isIntersecting(obj)) {
            self->Type = found;
            *luno_cLunoVar_castInt_1D78(self) = found;
            goto done;
        }
    }
    self->Type = 1;
    *luno_cLunoVar_castInt_1D78(self) = 0;
done:
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetGamePathFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A5720_1F48 __asm__("D_004A5720");
extern WScriptNullType D_004A5724_1F48 __asm__("D_004A5724");
extern int D_004A5728_1F48 __asm__("D_004A5728");
extern int D_004A3CB8_1F48[2] __asm__("D_004A3CB8");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void setGamePath_1F48(int) __asm__("setGamePath");

extern luno_cLunoVar luno_gLunoNil_1F48 __asm__("luno_gLunoNil");

/**
 * Script function (gLunoCFunctions[66]): does nothing in effect: it hands Path to the empty stub setGamePath.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tSetGamePathData: Path)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CB8 says eLunoVar_Float. Defaults: Path -1.
 *
 * Calls setGamePath(Path), which has an empty body in this build, so the game path is not changed.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar SetGamePathFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5728_1F48 == 0) {
        D_004A5720_1F48 = -1;
        D_004A5728_1F48 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5724_1F48 = WScriptNullType(D_004A5724_1F48);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5720_1F48, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CB8_1F48[k]);
    }
    setGamePath_1F48(vp[0]);
    return luno_gLunoNil_1F48;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetGameModeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int SetGameModeFunc_sectionModeTable_2048[] __asm__("SetGameModeFunc_sectionModeTable");
extern "C" void cWScriptMan_failActiveLocalMission_2048(cWScriptMan*, int)
    __asm__("cWScriptMan_failActiveLocalMission");
extern "C" void cSectionMan_eventbegin_2048(cSectionMan*, int) __asm__("cSectionMan_eventbegin");
extern t3Vector luno_gLunoNil_2048 __asm__("luno_gLunoNil");
// World-script data that sets a game mode (On Tour tSetGameModeData; same offsets): the block below fills it.
struct tSetGameModeData {
    int Mode;                       // 0x00 [O]
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tSetGameModeData, Mode, 0x00);
CHECK_OFFSET(tSetGameModeData, Cmd, 0x04);
CHECK_OFFSET(tSetGameModeData, null, 0x08);
CHECK_SIZE(tSetGameModeData, 0xC);
extern tSetGameModeData sSetGameModeDefaults __asm__("D_004FBC10");
extern int D_004A572C;
extern int D_004A3CC0;
extern "C" void* MEM_copy_2048(void*, void*, int) __asm__("MEM_copy");
/**
 * Script command SetGameMode: switches the section manager to a game mode, failing the running mission first.
 *
 * Defaults (D_004FBC10, under D_004A572C: -1, -1) take the keyword arguments (types from D_004A3CC0). Only when the
 * script's rider is set and not a computer rider (cBeing virtual isComputer, slot 9): the mode index (< 8) selects the
 * eSectionManMode in SetGameModeFunc_sectionModeTable;
 * if the second value is 0, cWScriptMan_failActiveLocalMission runs unless that mode is kSMMode_Freeride, then
 * cSectionMan_eventbegin(SSXApp->Game->SectionMan, mode).
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" t3Vector SetGameModeFunc(int n, luno_cLunoCArg *params) {
    if (D_004A572C == 0) {
        sSetGameModeDefaults.Mode = -1;
        sSetGameModeDefaults.Cmd = -1;
        D_004A572C = 1;
        sSetGameModeDefaults.null = WScriptNullType(sSetGameModeDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    MEM_copy_2048(buf, &sSetGameModeDefaults, sizeof(tSetGameModeData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int argType = (&D_004A3CC0)[idx];
        setArgField(&buf[idx], x, argType);
    }
    int notBusy;
    int k = cWScriptMan_TheWScriptMan->Being;
    if (k != kWScriptBeing_None) {
        cAI *ai = SSXApp->Game->AI;
        cRider *tableRider = ai->Riders[k];
        cBeing *riderPart = tableRider;
        cRider *rider = (cRider*)riderPart;
        notBusy = rider->isComputer() ^ 1;
    } else {
        notBusy = 0;
    }
    if (notBusy) {
        int mode = 0;
        unsigned id = buf[0];
        int secondValue = buf[1];
        cSectionMan *w = SSXApp->Game->SectionMan;
        if (id < 8)
            mode = SetGameModeFunc_sectionModeTable_2048[id];
        if (secondValue == 0) {
            if (mode != kSMMode_Freeride)
                cWScriptMan_failActiveLocalMission_2048(cWScriptMan_TheWScriptMan, id);
            cSectionMan_eventbegin_2048(w, mode);
        }
    }
    return luno_gLunoNil_2048;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SectionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// World-script data of SectionFunc (On Tour tSectionData; same offsets): the block below fills it.
struct tSectionData {
    int Location;                   // 0x00 [O]
    int Cmd;                        // 0x04 [O]
    int Mode;                       // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tSectionData, Location, 0x00);
CHECK_OFFSET(tSectionData, Cmd, 0x04);
CHECK_OFFSET(tSectionData, Mode, 0x08);
CHECK_OFFSET(tSectionData, null, 0x0C);
CHECK_SIZE(tSectionData, 0x10);
extern "C" void* MEM_copy_2210(void*, void*, int) __asm__("MEM_copy");
extern tSectionData sSectionDefaults __asm__("D_004FBC20");
extern int D_004A5730_2210 __asm__("D_004A5730");
extern const int D_004465C8_2210[] __asm__("D_004465C8");
extern char* const SetGameModeFunc_sectionModeTable_2210[] __asm__("SetGameModeFunc_sectionModeTable");
extern t3Vector luno_gLunoNil_2210 __asm__("luno_gLunoNil");
extern "C" void* cBE_getBE_2210() __asm__("cBE_getBE");
extern "C" cBEInterface* cBE_getInterface_2210(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationIDFromScript_2210(void* iface, int id) __asm__("cBENewRaceInterface_getLocationIDFromScript");
extern "C" void cSectionMan_load_2210(void* self, int hash, char* name) __asm__("cSectionMan_load");
extern "C" void cSectionMan_activate_2210(void* self, int hash, char* name) __asm__("cSectionMan_activate");
extern "C" void cGame_setState_2210(void* self, int a, int b, int refresh) __asm__("cGame_setState");
extern "C" void* cAudioInterface_bxAudio_2210() __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_BeginScriptedDJLine_2210(void* self) __asm__("cSSXAudio_BeginScriptedDJLine");


/**
 * Script command Section: loads or activates a location's world sections, or switches the game state.
 *
 * Defaults (D_004FBC20, under D_004A5730: -1, -1, -1) take the keyword arguments (types from D_004465C8). Only when the
 * script's rider is set and not a computer rider (cBeing virtual isComputer): the location comes from the script ID
 * through cBENewRaceInterface_getLocationIDFromScript (BE interface 0) and the mode from SetGameModeFunc_sectionModeTable (index < 8). The
 * action (eSectionCmd): kSectionCmd_Load cSectionMan_load, kSectionCmd_Activate cSectionMan_activate,
 * kSectionCmd_UserSelectDJ cGame_setState(game, 0xE, 2, 0) and cSSXAudio_BeginScriptedDJLine(cAudioInterface_bxAudio()),
 * kSectionCmd_UserSelect cGame_setState(game, 0xE, 0, 0), kSectionCmd_SkySwap sets SectionMan->m_bTriggerSkySwap;
 * kSectionCmd_Ignored does nothing (load and activate act on SSXApp->Game->SectionMan).
 *
 * The view types SetGameModeFunc_sectionModeTable as strings (name) and the location as a hash, but the table is the eSectionManMode one that
 * SetGameModeFunc reads as ints, and the value is the location ID.
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" t3Vector SectionFunc(int n, luno_cLunoCArg* params)
{
    if (D_004A5730_2210 == 0) {
        sSectionDefaults.Location = -1;
        sSectionDefaults.Cmd = -1;
        sSectionDefaults.Mode = -1;
        D_004A5730_2210 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sSectionDefaults.null = WScriptNullType(sSectionDefaults.null);
    }
    int argBlock[4] __attribute__((aligned(16)));
    int* vp = argBlock;
    MEM_copy_2210(vp, &sSectionDefaults, sizeof(tSectionData));
    luno_cLunoCArg* keyArg = params;
    for (int i = 0; i < n; i++, keyArg++) {
        int k = keyArg->Index;
        setArgField(&vp[k], keyArg, D_004465C8_2210[k]);
    }
    int ok;
    int cur = cWScriptMan_TheWScriptMan->Being;
    if (cur != kWScriptBeing_None) {
        cAI* ai = SSXApp->Game->AI;
        cRider* tableRider = ai->Riders[cur];
        cBeing* riderPart = tableRider;
        cRider* rider = (cRider*)riderPart;
        ok = !rider->isComputer();
    } else {
        ok = 0;
    }
    if (ok) {
        int sel = vp[2];
        int action = vp[1];
        int id = vp[0];
        char* name = 0;
        void* iface = cBE_getInterface_2210(cBE_getBE_2210(), 0);
        cSectionMan* ui = SSXApp->Game->SectionMan;
        if ((unsigned int)sel < 8) {
            name = SetGameModeFunc_sectionModeTable_2210[sel];
        }
        int hash = cBENewRaceInterface_getLocationIDFromScript_2210(iface, id);
        switch ((unsigned int)action) {
        case kSectionCmd_Load:
            cSectionMan_load_2210(ui, hash, name);
            break;
        case kSectionCmd_Ignored:
            break;
        case kSectionCmd_Activate:
            cSectionMan_activate_2210(ui, hash, name);
            break;
        case kSectionCmd_UserSelectDJ:
            cGame_setState_2210(SSXApp->Game, kGFSM_UserSelect, 2, 0);
            cSSXAudio_BeginScriptedDJLine_2210(cAudioInterface_bxAudio_2210());
            break;
        case kSectionCmd_UserSelect:
            cGame_setState_2210(SSXApp->Game, kGFSM_UserSelect, 0, 0);
            break;
        case kSectionCmd_SkySwap:
            ui->m_bTriggerSkySwap = 1;
            break;
        }
    }
    return luno_gLunoNil_2210;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RemoveModifierFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of RemoveModifierFunc (On Tour tRemoveModifierData; same offsets): the block below fills it.
struct tRemoveModifierData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Type;                       // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tRemoveModifierData, InstId, 0x00);
CHECK_OFFSET(tRemoveModifierData, Type, 0x04);
CHECK_OFFSET(tRemoveModifierData, null, 0x08);
CHECK_SIZE(tRemoveModifierData, 0xC);
extern t3Vector luno_gLunoNil_2490 __asm__("luno_gLunoNil");
extern tRemoveModifierData sRemoveModifierDefaults __asm__("D_004FBC30");
extern int D_5734_2490 __asm__("D_004A5734");
extern int D_3CC8_2490 __asm__("D_004A3CC8");
extern "C" void* MEM_copy_2490(void*, void*, int) __asm__("MEM_copy");
extern "C" void tModifierBlock_fadeEffectModifier_2490(tModifierBlock*) __asm__("tModifierBlock_fadeEffectModifier");
extern "C" void tModifierBlock_remHaloModifier_2490(tModifierBlock*) __asm__("tModifierBlock_remHaloModifier");
/**
 * Script function (gLunoCFunctions[69]): removes a modifier (effect fade or halo) from a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tRemoveModifierData:
 * InstId, Type) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3CC8 says eLunoVar_Float. Defaults: InstId -1, Type -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (m_pActiveNode) is
 * a move node (virtual isMoveNode), Type kRemoveModifier_Effect calls tModifierBlock_fadeEffectModifier and
 * kRemoveModifier_Halo tModifierBlock_remHaloModifier on the node's modifier block (cMoveNode::ModifierBlock) when it
 * has one; other types do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector RemoveModifierFunc(int n, luno_cLunoCArg *params) {
    if (D_5734_2490 == 0) {
        sRemoveModifierDefaults.InstId = 0xFFFFFFFF;
        sRemoveModifierDefaults.Type = -1;
        D_5734_2490 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sRemoveModifierDefaults.null = WScriptNullType(sRemoveModifierDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_2490(buf, &sRemoveModifierDefaults, sizeof(tRemoveModifierData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_3CC8_2490)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        int mode = buf[1];
        if (activeNode) {
            if (activeNode->isMoveNode()) {
                cMoveNode *moveNode = (cMoveNode*)obj->m_pActiveNode;
                switch (mode) {
                case kRemoveModifier_Effect: {
                    tModifierBlock *modBlock = moveNode->ModifierBlock;
                    if (modBlock)
                        tModifierBlock_fadeEffectModifier_2490(modBlock);
                    break;
                }
                case kRemoveModifier_Halo: {
                    tModifierBlock *modBlock = moveNode->ModifierBlock;
                    if (modBlock)
                        tModifierBlock_remHaloModifier_2490(modBlock);
                    break;
                }
                }
            }
        }
    }
    return luno_gLunoNil_2490;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsGamePathFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cLunoValue {
    int word0;
    int word1;
    int type;
};
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CD0_2680[2] __asm__("D_004A3CD0");
extern "C" bool isGamePath(unsigned);
extern int D_004A5740_2680 __asm__("D_004A5740");
extern int D_004A5738_2680 __asm__("D_004A5738");
extern WScriptNullType D_004A573C_2680 __asm__("D_004A573C");
/**
 * Script function (gLunoCFunctions[42]): returns whether the game is on a given game path.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGamePathData: Path)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CD0 says eLunoVar_Float. Defaults: Path -1.
 *
 * Stores isGamePath(Path) (1 when the race DB's cGamePath matches) in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGamePathFunc(cLunoValue* ret, int n, luno_cLunoCArg* params) {
    if (D_004A5740_2680 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, WScriptNullType }).
        D_004A5738_2680 = -1;
        D_004A573C_2680 = WScriptNullType(D_004A573C_2680);
        D_004A5740_2680 = 1;
    }
    int fields[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, fields, &D_004A5738_2680, 8, D_004A3CD0_2680, n, params);
    BIND_SET_RESULT_1FBE38(ret, isGamePath(vp[0]));
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsGameModeFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CD8_2778[2] __asm__("D_004A3CD8");
extern "C" bool isGameMode(unsigned);
extern int D_004A5750_2778 __asm__("D_004A5750");
extern int D_004A5748_2778 __asm__("D_004A5748");
extern WScriptNullType D_004A574C_2778 __asm__("D_004A574C");
/**
 * Script function (gLunoCFunctions[43]): returns whether the game is in a given game mode.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGameModeData: Mode)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CD8 says eLunoVar_Float. Defaults: Mode -1.
 *
 * Stores isGameMode(Mode) (1 when the race DB's cGameMode matches) in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGameModeFunc(cLunoValue* ret, int n, luno_cLunoCArg* params) {
    if (D_004A5750_2778 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, WScriptNullType }).
        D_004A5748_2778 = -1;
        D_004A574C_2778 = WScriptNullType(D_004A574C_2778);
        D_004A5750_2778 = 1;
    }
    int fields[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, fields, &D_004A5748_2778, 8, D_004A3CD8_2778, n, params);
    BIND_SET_RESULT_1FBE38(ret, isGameMode(vp[0]));
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsGameEventFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CE0_2870[2] __asm__("D_004A3CE0");
extern "C" bool isGameEvent(unsigned);
extern int D_004A5760_2870 __asm__("D_004A5760");
extern int D_004A5758_2870 __asm__("D_004A5758");
extern WScriptNullType D_004A575C_2870 __asm__("D_004A575C");
/**
 * Script function (gLunoCFunctions[108]): returns whether the game is running a given game event.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGameEventData:
 * Event) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3CE0 says eLunoVar_Float. Defaults: Event -1.
 *
 * Stores isGameEvent(Event) (1 when the race DB's cGameEvent matches) in @p ret as an int script value. It sits at
 * table index 108, after the On Tour-era entries.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGameEventFunc(cLunoValue* ret, int n, luno_cLunoCArg* params) {
    if (D_004A5760_2870 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, WScriptNullType }).
        D_004A5758_2870 = -1;
        D_004A575C_2870 = WScriptNullType(D_004A575C_2870);
        D_004A5760_2870 = 1;
    }
    int fields[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, fields, &D_004A5758_2870, 8, D_004A3CE0_2870, n, params);
    BIND_SET_RESULT_1FBE38(ret, isGameEvent(vp[0]));
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetCurrentInstanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_2968 __asm__("luno_gLunoNil");

extern int D_004A3CE8_2968[2] __asm__("D_004A3CE8");
extern int D_004A5770_2968 __asm__("D_004A5770");
extern unsigned D_004A5768_2968 __asm__("D_004A5768");
extern WScriptNullType D_004A576C_2968 __asm__("D_004A576C");
/**
 * Script function (gLunoCFunctions[44]): makes a world instance the script's current instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour
 * tSetCurrentInstanceData: InstId) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3CE8 says eLunoVar_Float. Defaults: InstId -1.
 *
 * Sets cWScriptMan_TheWScriptMan->Inst to the instance InstId names (its low byte picks a world-cache section, the rest
 * an instance in it), or to 0 when it is not found. -1 is not special here: it looks in section 0xFF.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar SetCurrentInstanceFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5770_2968 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A576C_2968 = WScriptNullType(D_004A576C_2968);
        D_004A5768_2968 = -1;
        D_004A5770_2968 = 1;
    }
    unsigned block[4] __attribute__((aligned(16)));
    unsigned* vp = block;
    vp[0] = 0xFFFFFFFFu;
    MEM_copy(vp, &D_004A5768_2968, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CE8_2968[k]);
    }
    cWScriptMan* scriptMan = cWScriptMan_TheWScriptMan;
    unsigned x = vp[0];
    cWorldCacheTable** tbl = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable;
    cWorldCacheTable* section = tbl[x & 0xFF];
    unsigned w;
    tInstanceRecord* inst;
    if (section == 0 || (w = section->m_pCacheTable[3].m_pDataTag[x >> 8].m_bfuiMemLoc) == 0)
        inst = 0;
    else
        inst = instanceAtMemLoc(w);
    scriptMan->Inst = inst;
    return luno_gLunoNil_2968;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ProcessFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of ProcessFunc (On Tour tProcessData; same offsets): the block below fills it.
struct tProcessData {
    unsigned int ProcessId;         // 0x00 [O]
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tProcessData, ProcessId, 0x00);
CHECK_OFFSET(tProcessData, Cmd, 0x04);
CHECK_OFFSET(tProcessData, null, 0x08);
CHECK_SIZE(tProcessData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_2AC0 __asm__("luno_gLunoNil");

extern int D_004A3CF0_2AC0[2] __asm__("D_004A3CF0");
extern "C" void cWScriptMan_doProcessCmd(cWScriptMan*, int, int);
extern int D_004A5774_2AC0 __asm__("D_004A5774");
extern tProcessData sProcessDefaults __asm__("D_004FBC40");
/**
 * Script function (gLunoCFunctions[45]): sends a command to a script process (cWScriptMan_doProcessCmd).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tProcessData:
 * ProcessId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3CF0 says eLunoVar_Float. Defaults: ProcessId -1, Cmd 0.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, Cmd): -1 is the current process; Cmd 1 starts
 * it, 0 stops it, 2 ends its task, 18 ends it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar ProcessFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5774_2AC0 == 0) {
        sProcessDefaults.ProcessId = -1;
        sProcessDefaults.Cmd = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sProcessDefaults.null = WScriptNullType(sProcessDefaults.null);
        D_004A5774_2AC0 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sProcessDefaults, sizeof(tProcessData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CF0_2AC0[k]);
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, vp[0], vp[1]);
    return luno_gLunoNil_2AC0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndMissionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_2BD8 __asm__("luno_gLunoNil");

extern int D_004A3CF8_2BD8[2] __asm__("D_004A3CF8");
extern "C" void cWScriptMan_doMissionCmd(cWScriptMan*, int, int, int);
extern int D_004A5780_2BD8 __asm__("D_004A5780");
extern unsigned D_004A5778_2BD8 __asm__("D_004A5778");
extern WScriptNullType D_004A577C_2BD8 __asm__("D_004A577C");
/**
 * Script function (gLunoCFunctions[51]): ends a mission (mission command 0).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndMissionData:
 * MissionId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3CF8 says eLunoVar_Float. Defaults: MissionId -1.
 *
 * Calls cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan, MissionId, 0, -1); -1 is the current mission.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar EndMissionFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5780_2BD8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A577C_2BD8 = WScriptNullType(D_004A577C_2BD8);
        D_004A5778_2BD8 = -1;
        D_004A5780_2BD8 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5778_2BD8, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3CF8_2BD8[k]);
    }
    cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan, vp[0], 0, -1);
    return luno_gLunoNil_2BD8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndProcessFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_2CE8 __asm__("luno_gLunoNil");

extern int D_004A3D00_2CE8[2] __asm__("D_004A3D00");
extern "C" void cWScriptMan_doProcessCmd(cWScriptMan*, int, int);
extern int D_004A5790_2CE8 __asm__("D_004A5790");
extern unsigned D_004A5788_2CE8 __asm__("D_004A5788");
extern WScriptNullType D_004A578C_2CE8 __asm__("D_004A578C");
/**
 * Script function (gLunoCFunctions[46]): stops a script process (process command 0).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndProcessData:
 * ProcessId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D00 says eLunoVar_Float. Defaults: ProcessId -1.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, 0); -1 is the current process.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar EndProcessFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5790_2CE8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A578C_2CE8 = WScriptNullType(D_004A578C_2CE8);
        D_004A5788_2CE8 = -1;
        D_004A5790_2CE8 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5788_2CE8, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D00_2CE8[k]);
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, vp[0], 0);
    return luno_gLunoNil_2CE8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_2DF0 __asm__("luno_gLunoNil");

extern int D_004A3D08_2DF0[2] __asm__("D_004A3D08");
extern "C" void cWScriptMan_doProcessCmd(cWScriptMan*, int, int);
extern int D_004A57A0_2DF0 __asm__("D_004A57A0");
extern unsigned D_004A5798_2DF0 __asm__("D_004A5798");
extern WScriptNullType D_004A579C_2DF0 __asm__("D_004A579C");
/**
 * Script function (gLunoCFunctions[47]): ends a script process's current task (process command 2).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndTaskData:
 * ProcessId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D08 says eLunoVar_Float. Defaults: ProcessId -1.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, 2); -1 is the current process.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar EndTaskFunc(int n, luno_cLunoCArg* params) {
    if (D_004A57A0_2DF0 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A579C_2DF0 = WScriptNullType(D_004A579C_2DF0);
        D_004A5798_2DF0 = -1;
        D_004A57A0_2DF0 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5798_2DF0, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D08_2DF0[k]);
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, vp[0], 2);
    return luno_gLunoNil_2DF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderActionFunc);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern int D_004A3D10_2EF8[2] __asm__("D_004A3D10");
extern int D_004A57B0_2EF8 __asm__("D_004A57B0");
extern int D_004A57A8_2EF8 __asm__("D_004A57A8");
extern WScriptNullType D_004A57AC_2EF8 __asm__("D_004A57AC");
extern "C" int cRider_getMissionInt_2EF8(cRider*, int) __asm__("cRider_getMissionInt");
/**
 * Script function (gLunoCFunctions[50]): answers a rider-action question about the script's rider
 * (cRider_getMissionInt).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderActionData: Type)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D10 says eLunoVar_Float. Defaults: Type 0.
 *
 * Cmd/Type (eScriptRiderQuery) is mapped to the cRider_getMissionInt query (eMissionQuery) of the same name (0
 * ground kMQ_Ground, 1 air, 2 railslide, 3 wipeout, 4 reset, 5-7 grab/tweak/uber held, 8 TotalStylePoints, 9
 * TotalPoints, 10 and 11 nose and tail press, 12 boosting, 13-17 stall ... NumLastSecondLanding, 18 punching); any
 * other value gives 0. The answer for the script's rider
 * (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) is returned as an int in the hidden result slot self.
 * RiderMetricFunc does the same.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the int answer
 */
// Script call: maps the script condition id to a rider condition and returns whether the current rider meets it.
extern "C" void* RiderActionFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A57B0_2EF8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57B0_2EF8 = 1;
        D_004A57A8_2EF8 = 0;
        D_004A57AC_2EF8 = WScriptNullType(D_004A57AC_2EF8);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57A8_2EF8, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D10_2EF8[k]);
    }
    cRider* riderEntry = SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being];
    cBeing* riderIface = riderEntry;
    cRider* rider = (cRider*)riderIface;
    int res;
    switch (vp[0]) {
    case kRiderQuery_Wipeout: res = cRider_getMissionInt_2EF8(rider, kMQ_Wipeout); break;
    case kRiderQuery_Reset: res = cRider_getMissionInt_2EF8(rider, kMQ_Reset); break;
    case kRiderQuery_Air: res = cRider_getMissionInt_2EF8(rider, kMQ_Air); break;
    case kRiderQuery_Ground: res = cRider_getMissionInt_2EF8(rider, kMQ_Ground); break;
    case kRiderQuery_Railslide: res = cRider_getMissionInt_2EF8(rider, kMQ_Railslide); break;
    case kRiderQuery_GrabHeld: res = cRider_getMissionInt_2EF8(rider, kMQ_GrabHeld); break;
    case kRiderQuery_TweakHeld: res = cRider_getMissionInt_2EF8(rider, kMQ_TweakHeld); break;
    case kRiderQuery_UberHeld: res = cRider_getMissionInt_2EF8(rider, kMQ_UberHeld); break;
    case kRiderQuery_TotalPoints: res = cRider_getMissionInt_2EF8(rider, kMQ_TotalPoints); break;
    case kRiderQuery_NosePress: res = cRider_getMissionInt_2EF8(rider, kMQ_NosePress); break;
    case kRiderQuery_TailPress: res = cRider_getMissionInt_2EF8(rider, kMQ_TailPress); break;
    case kRiderQuery_Boosting: res = cRider_getMissionInt_2EF8(rider, kMQ_Boosting); break;
    case kRiderQuery_Stall: res = cRider_getMissionInt_2EF8(rider, kMQ_Stall); break;
    case kRiderQuery_OffAxis: res = cRider_getMissionInt_2EF8(rider, kMQ_OffAxis); break;
    case kRiderQuery_Inverted: res = cRider_getMissionInt_2EF8(rider, kMQ_Inverted); break;
    case kRiderQuery_NumPerfectLanding: res = cRider_getMissionInt_2EF8(rider, kMQ_NumPerfectLanding); break;
    case kRiderQuery_NumLastSecondLanding: res = cRider_getMissionInt_2EF8(rider, kMQ_NumLastSecondLanding); break;
    case kRiderQuery_TotalStylePoints: res = cRider_getMissionInt_2EF8(rider, kMQ_TotalStylePoints); break;
    case kRiderQuery_Punching: res = cRider_getMissionInt_2EF8(rider, kMQ_Punching); break;
    default: res = 0; break;
    }
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", HasActiveObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_3130(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

int *luno_cLunoVar_castInt(void*);
// World-script data that names an instance (On Tour reuses MovieFunc's block here) (On Tour tMovieData; same offsets):
// the block below fills it.
struct tMovieData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tMovieData, InstId, 0x00);
CHECK_OFFSET(tMovieData, Cmd, 0x04);
CHECK_OFFSET(tMovieData, null, 0x08);
CHECK_SIZE(tMovieData, 0xC);
extern tMovieData sHasActiveObjectDefaults __asm__("D_004FBC50");
extern int D_004A57B4;
extern int D_004A3D18;
extern "C" void* MEM_copy_3130(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[52]): returns whether a world instance has an active node.
 *
 * Copies the static defaults (InstId -1, then 1; set up on the first call) into a 12-byte parameter block and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D18 says
 * eLunoVar_Float. Only InstId is used.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The int result stored in @p ret is 1 when the instance exists and
 * has an active node (m_pActiveNode), else 0.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" luno_cLunoVar* HasActiveObjectFunc(luno_cLunoVar *ret, int n, luno_cLunoCArg *params) {
    if (D_004A57B4 == 0) {
        sHasActiveObjectDefaults.InstId = 0xFFFFFFFF;
        sHasActiveObjectDefaults.Cmd = 1;
        D_004A57B4 = 1;
        sHasActiveObjectDefaults.null = WScriptNullType(sHasActiveObjectDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_3130(buf, &sHasActiveObjectDefaults, sizeof(tMovieData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3D18)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    tInstanceRecord *obj = look_3130(buf[0]);
    if (obj && obj->m_pActiveNode) {
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = 1;
    } else {
        ret->Type = eLunoVar_Int;
        *luno_cLunoVar_castInt(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderDistanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector length).
void* luno_cLunoVar_castFloat(void*);
static inline float len_32C0(const t4Vector &v) {
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
/**
 * Script function (gLunoCFunctions[59]): returns how far the script's rider moves per frame.
 *
 * Takes no script arguments. The float result is the length of the velocity (cRider virtual getVelocity) of the
 * script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) times the frame time
 * (SSXApp->TimerPeriod), stored in the hidden result slot self.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @return self
 */
extern "C" void* RiderDistanceFunc(luno_cLunoVar* self) {
    cRider *riderEntry = SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being];
    cBeing *riderIface = riderEntry;
    cRider *rider = (cRider*)riderIface;
    float len = len_32C0(*rider->getVelocity());
    float res = len * SSXApp->TimerPeriod;
    self->Type = 2;
    *(float*)luno_cLunoVar_castFloat(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderSpeedFunc);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector length).
void* luno_cLunoVar_castFloat(void*);
static inline float len_3380(const t4Vector &v) {
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
/**
 * Script function (gLunoCFunctions[60]): returns the script's rider's speed.
 *
 * Takes no script arguments. The float result is the length of the velocity (cRider virtual getVelocity) of the
 * script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]), stored in the hidden result slot self.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @return self
 */
extern "C" void* RiderSpeedFunc(luno_cLunoVar* self) {
    cRider *riderEntry = SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being];
    cBeing *riderIface = riderEntry;
    cRider *rider = (cRider*)riderIface;
    float len = len_3380(*rider->getVelocity());
    self->Type = 2;
    *(float*)luno_cLunoVar_castFloat(self) = len;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentInstanceFunc);
#ifdef SKIP_ASM
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
// Int result setter taking the value by const reference; the by-value macro form lets jump1 hoist the -1 arm.
static inline void setInt_3430(cLunoValue* r, const unsigned& v) {
    r->type = 1;
    *luno_cLunoVar_castInt(r) = v;
}
/**
 * Script function (gLunoCFunctions[61]): returns the id of the script's current instance.
 *
 * Takes no script arguments. The int result stored in @p ret is the resource id (m_RID) of
 * cWScriptMan_TheWScriptMan->Inst, or -1 when there is none.
 *
 * @param ret  the result slot
 * @return @p ret
 */
extern "C" cLunoValue* CurrentInstanceFunc(cLunoValue* ret) {
    tInstanceRecord* curInst = cWScriptMan_TheWScriptMan->Inst;
    unsigned instId;
    if (curInst)
        instId = curInst->m_RID;
    else
        instId = 0xFFFFFFFF;
    setInt_3430(ret, instId);
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int* luno_cLunoVar_castIndex_3490(void*) __asm__("luno_cLunoVar_castIndex__FPv");
extern "C" unsigned cWScriptProcess_getRunningTaskId_3490(cWScriptProcess*) __asm__("cWScriptProcess_getRunningTaskId");
/**
 * Script function (gLunoCFunctions[62]): returns the id of the running task of the current script process.
 *
 * Takes no script arguments. The result is cWScriptProcess_getRunningTaskId on
 * cWScriptMan_TheWScriptMan->CurrentProcess, or -1 without a process, stored as an index script value (type 3) in the
 * hidden result slot self.
 *
 * @return self
 */
extern "C" void* CurrentTaskFunc(luno_cLunoVar* self) {
    cWScriptProcess *curProcess = cWScriptMan_TheWScriptMan->CurrentProcess;
    unsigned taskId;
    if (curProcess)
        taskId = cWScriptProcess_getRunningTaskId_3490(curProcess);
    else
        taskId = 0xFFFFFFFF;
    self->Type = 3;
    *luno_cLunoVar_castIndex_3490(self) = taskId;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int* luno_cLunoVar_castInt(void*);
extern "C" int cRider_getMissionInt_34F8(cRider*, int) __asm__("cRider_getMissionInt");
/**
 * Script function (gLunoCFunctions[63]): returns the rail the script's rider is grinding.
 *
 * Takes no script arguments. When the script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being])
 * railslides (cRider_getMissionInt query 4), the int result is the rail spline id of its rider handler
 * (mRiderHandler->RailslideMotion.SplineID); otherwise -1. It is stored in the hidden result slot self.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @return self
 */
extern "C" void* CurrentRailFunc(luno_cLunoVar* self) {
    cRider *riderEntry = SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being];
    cBeing *riderIface = riderEntry;
    cRider *rider = (cRider*)riderIface;
    unsigned splineId;
    if (cRider_getMissionInt_34F8(rider, 4))
        splineId = rider->mRiderHandler->RailslideMotion.SplineID;
    else
        splineId = 0xFFFFFFFF;
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = splineId;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AIRandomFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of AIRandomFunc (On Tour tAIRandomData; same offsets): the block below fills it.
struct tAIRandomData {
    float Lo;                       // 0x00 [O]
    float Hi;                       // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tAIRandomData, Lo, 0x00);
CHECK_OFFSET(tAIRandomData, Hi, 0x04);
CHECK_OFFSET(tAIRandomData, null, 0x08);
CHECK_SIZE(tAIRandomData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_3598 __asm__("luno_gLunoNil");

extern int D_004A3D20_3598[2] __asm__("D_004A3D20");
float AIrandf(float, float);
void* luno_cLunoVar_castFloat(void*);
extern int D_004A57B8_3598 __asm__("D_004A57B8");
extern tAIRandomData sAIRandomDefaults __asm__("D_004FBC60");
/**
 * Script function (gLunoCFunctions[77]): returns a random float between two bounds (AIrandf).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAIRandomData: Lo, Hi)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D20 says eLunoVar_Float. Defaults: Lo 0, Hi 1.
 *
 * Returns AIrandf(Lo, Hi) as a float script value in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the random float
 */
extern "C" void* AIRandomFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A57B8_3598 == 0) {
        sAIRandomDefaults.Lo = 0;
        sAIRandomDefaults.Hi = 1.0f;
        D_004A57B8_3598 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sAIRandomDefaults.null = WScriptNullType(sAIRandomDefaults.null);
    }
    float block[4] __attribute__((aligned(16)));
    float* vp = block;
    MEM_copy(vp, &sAIRandomDefaults, sizeof(tAIRandomData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D20_3598[k]);
    }
    float randValue = AIrandf(vp[0], vp[1]);
    self->Type = 2;
    *(float*)luno_cLunoVar_castFloat(self) = randValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentTrickFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A57C0_36B0 __asm__("D_004A57C0");
extern WScriptNullType D_004A57C4_36B0 __asm__("D_004A57C4");
extern int D_004A57C8_36B0 __asm__("D_004A57C8");
extern int D_004A3D28_36B0[2] __asm__("D_004A3D28");
extern "C" void MEM_copy(void*, void*, int);
extern "C" int cWScriptMan_getCurrentTrick(cWScriptMan*, int, int);
int* luno_cLunoVar_castInt(void*);


/**
 * Script function (gLunoCFunctions[78]): returns a field of the current trick definition (cWScriptMan_getCurrentTrick).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tCurrentTrickData: Cmd)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D28 says eLunoVar_Float. Defaults: Cmd -1.
 *
 * Returns cWScriptMan_getCurrentTrick(cWScriptMan_TheWScriptMan, Being, Cmd), field Cmd of cWScriptMan::TrickDef (0 for
 * the default -1), as an int in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the field value
 */
extern "C" void* CurrentTrickFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A57C8_36B0 == 0) {
        D_004A57C0_36B0 = -1;
        D_004A57C8_36B0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57C4_36B0 = WScriptNullType(D_004A57C4_36B0);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57C0_36B0, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D28_36B0[k]);
    }
    int trickField = cWScriptMan_getCurrentTrick(cWScriptMan_TheWScriptMan, cWScriptMan_TheWScriptMan->Being, vp[0]);
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = trickField;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderMetricFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern int D_004A57D0_37B0 __asm__("D_004A57D0");
extern WScriptNullType D_004A57D4_37B0 __asm__("D_004A57D4");
extern int D_004A57D8_37B0 __asm__("D_004A57D8");
extern int D_004A3D30_37B0[2] __asm__("D_004A3D30");
extern "C" int cRider_getMissionInt_37B0(cRider* rider, int which) __asm__("cRider_getMissionInt");


/**
 * Script function (gLunoCFunctions[79]): answers a rider-metric question about the script's rider
 * (cRider_getMissionInt).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderMetricData: Cmd)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D30 says eLunoVar_Float. Defaults: Cmd -1.
 *
 * Cmd/Type (eScriptRiderQuery) is mapped to the cRider_getMissionInt query (eMissionQuery) of the same name (0
 * ground kMQ_Ground, 1 air, 2 railslide, 3 wipeout, 4 reset, 5-7 grab/tweak/uber held, 8 TotalStylePoints, 9
 * TotalPoints, 10 and 11 nose and tail press, 12 boosting, 13-17 stall ... NumLastSecondLanding, 18 punching); any
 * other value gives 0. The answer for the script's rider
 * (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) is returned as an int in the hidden result slot self.
 * Same body as RiderActionFunc.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the int answer
 */
extern "C" void* RiderMetricFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params)
{
    if (D_004A57D8_37B0 == 0) {
        D_004A57D0_37B0 = -1;
        D_004A57D8_37B0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57D4_37B0 = WScriptNullType(D_004A57D4_37B0);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57D0_37B0, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D30_37B0[k]);
    }
    cAI* ai = SSXApp->Game->AI;
    cRider* riderEntry = ai->Riders[cWScriptMan_TheWScriptMan->Being];
    cBeing* riderPart = riderEntry;
    cRider* rider = (cRider*)riderPart;
    int res;
    switch (vp[0]) {
    case kRiderQuery_Wipeout: res = cRider_getMissionInt_37B0(rider, kMQ_Wipeout); break;
    case kRiderQuery_Reset: res = cRider_getMissionInt_37B0(rider, kMQ_Reset); break;
    case kRiderQuery_Air: res = cRider_getMissionInt_37B0(rider, kMQ_Air); break;
    case kRiderQuery_Ground: res = cRider_getMissionInt_37B0(rider, kMQ_Ground); break;
    case kRiderQuery_Railslide: res = cRider_getMissionInt_37B0(rider, kMQ_Railslide); break;
    case kRiderQuery_GrabHeld: res = cRider_getMissionInt_37B0(rider, kMQ_GrabHeld); break;
    case kRiderQuery_TweakHeld: res = cRider_getMissionInt_37B0(rider, kMQ_TweakHeld); break;
    case kRiderQuery_UberHeld: res = cRider_getMissionInt_37B0(rider, kMQ_UberHeld); break;
    case kRiderQuery_TotalPoints: res = cRider_getMissionInt_37B0(rider, kMQ_TotalPoints); break;
    case kRiderQuery_NosePress: res = cRider_getMissionInt_37B0(rider, kMQ_NosePress); break;
    case kRiderQuery_TailPress: res = cRider_getMissionInt_37B0(rider, kMQ_TailPress); break;
    case kRiderQuery_Boosting: res = cRider_getMissionInt_37B0(rider, kMQ_Boosting); break;
    case kRiderQuery_Stall: res = cRider_getMissionInt_37B0(rider, kMQ_Stall); break;
    case kRiderQuery_OffAxis: res = cRider_getMissionInt_37B0(rider, kMQ_OffAxis); break;
    case kRiderQuery_Inverted: res = cRider_getMissionInt_37B0(rider, kMQ_Inverted); break;
    case kRiderQuery_NumPerfectLanding: res = cRider_getMissionInt_37B0(rider, kMQ_NumPerfectLanding); break;
    case kRiderQuery_NumLastSecondLanding: res = cRider_getMissionInt_37B0(rider, kMQ_NumLastSecondLanding); break;
    case kRiderQuery_TotalStylePoints: res = cRider_getMissionInt_37B0(rider, kMQ_TotalStylePoints); break;
    case kRiderQuery_Punching: res = cRider_getMissionInt_37B0(rider, kMQ_Punching); break;
    default: res = 0; break;
    }
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", Crowd3dFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int TheCrowdMan3d_39F0 __asm__("TheCrowdMan3d");
extern "C" void func_00343C60_39F0(int, int) __asm__("func_00343C60");
extern "C" void cCrowdMan3d_removeMember_39F0(int, int) __asm__("cCrowdMan3d_removeMember");
extern t3Vector luno_gLunoNil_39F0 __asm__("luno_gLunoNil");
// World-script data that joins or leaves a 3D crowd (On Tour tCrowd3dData; same offsets): the block below fills it.
struct tCrowd3dData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Cmd;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tCrowd3dData, InstId, 0x00);
CHECK_OFFSET(tCrowd3dData, Cmd, 0x04);
CHECK_OFFSET(tCrowd3dData, null, 0x08);
CHECK_SIZE(tCrowd3dData, 0xC);
extern tCrowd3dData sCrowd3dDefaults __asm__("D_004FBC70");
extern int D_004A57DC;
extern int D_004A3D38;
extern "C" void* MEM_copy_39F0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[80]): adds a world instance to, or removes it from, its 3D crowd.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCrowd3dData: InstId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D38 says eLunoVar_Float. Defaults: InstId -1, Cmd -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, Cmd kCrowd3dCmd_Join passes its id
 * (m_RID) to func_00343C60 (joins the crowd block for its model) and kCrowd3dCmd_Leave to cCrowdMan3d_removeMember (leaves it),
 * both on the 3D crowd manager TheCrowdMan3d; other values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector Crowd3dFunc(int n, luno_cLunoCArg *params) {
    if (D_004A57DC == 0) {
        sCrowd3dDefaults.InstId = 0xFFFFFFFF;
        sCrowd3dDefaults.Cmd = -1;
        D_004A57DC = 1;
        sCrowd3dDefaults.null = WScriptNullType(sCrowd3dDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_39F0(buf, &sCrowd3dDefaults, sizeof(tCrowd3dData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3D38)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        int crowdCmd = buf[1];
        int w = obj->m_RID;
        if (crowdCmd == kCrowd3dCmd_Join)
            func_00343C60_39F0(TheCrowdMan3d_39F0, w);
        else if (crowdCmd == kCrowd3dCmd_Leave)
            cCrowdMan3d_removeMember_39F0(TheCrowdMan3d_39F0, w);
    }
    return luno_gLunoNil_39F0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", Crowd2dFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of Crowd2dFunc (On Tour tCrowd2dData; same offsets): the block below fills it.
struct tCrowd2dData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Cmd;                        // 0x04 [O]
    int Mesh;                       // 0x08 [O]
    int Context;                    // 0x0C [O]
    WScriptNullType null;           // 0x10 [O]
    char unk_011[0x3];              // 0x11 padding
};

CHECK_OFFSET(tCrowd2dData, InstId, 0x00);
CHECK_OFFSET(tCrowd2dData, Cmd, 0x04);
CHECK_OFFSET(tCrowd2dData, Mesh, 0x08);
CHECK_OFFSET(tCrowd2dData, Context, 0x0C);
CHECK_OFFSET(tCrowd2dData, null, 0x10);
CHECK_SIZE(tCrowd2dData, 0x14);
extern int TheCrowdMan2d_3BA0 __asm__("TheCrowdMan2d");
extern t3Vector luno_gLunoNil_3BA0 __asm__("luno_gLunoNil");
extern tCrowd2dData sCrowd2dDefaults __asm__("D_004FBC80");
extern int D_57E0_3BA0 __asm__("D_004A57E0");
extern const int Crowd2dFunc_types_3BA0[] __asm__("Crowd2dFunc_types");
extern "C" void* MEM_copy_3BA0(void*, void*, int) __asm__("MEM_copy");
extern "C" void cCrowdMan2d_attachBoneToCrowdPose_3BA0(int, unsigned, int, int) __asm__("cCrowdMan2d_attachBoneToCrowdPose");
extern "C" void cCrowdMan2d_addQuad_3BA0(int, unsigned, int, int) __asm__("cCrowdMan2d_addQuad");
extern "C" void cCrowdMan2d_removeQuad_3BA0(int, unsigned, int, int) __asm__("cCrowdMan2d_removeQuad");
/**
 * Script function (gLunoCFunctions[88]): runs a 2D-crowd command on a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 20-byte parameter block (On Tour tCrowd2dData: InstId,
 * Cmd, Mesh, Context) and lets each argument override the field its index names, turning an int into a float where the
 * field type table Crowd2dFunc_types says eLunoVar_Float. Defaults: InstId -1, Cmd -1, Mesh 0, Context 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, its id (m_RID) goes to the 2D crowd
 * manager TheCrowdMan2d: Cmd kCrowd2dCmd_Attach calls cCrowdMan2d_attachBoneToCrowdPose (points bone Context of part Mesh at the shared
 * crowd record), kCrowd2dCmd_AddQuad cCrowdMan2d_addQuad (takes a crowd quad slot for the instance) and
 * kCrowd2dCmd_RemoveQuad cCrowdMan2d_removeQuad (releases it); kCrowd2dCmd_Ignored and others do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector Crowd2dFunc(int n, luno_cLunoCArg *params) {
    if (D_57E0_3BA0 == 0) {
        sCrowd2dDefaults.InstId = 0xFFFFFFFF;
        sCrowd2dDefaults.Cmd = -1;
        sCrowd2dDefaults.Mesh = 0;
        sCrowd2dDefaults.Context = 0;
        D_57E0_3BA0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sCrowd2dDefaults.null = WScriptNullType(sCrowd2dDefaults.null);
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_3BA0(buf, &sCrowd2dDefaults, sizeof(tCrowd2dData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = Crowd2dFunc_types_3BA0[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        int mode = buf[1];
        int meshIndex = buf[2];
        int contextIndex = buf[3];
        unsigned instId = obj->m_RID;
        if (mode == kCrowd2dCmd_Attach)
            cCrowdMan2d_attachBoneToCrowdPose_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
        else if (mode == kCrowd2dCmd_Ignored) {
        } else if (mode == kCrowd2dCmd_AddQuad)
            cCrowdMan2d_addQuad_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
        else if (mode == kCrowd2dCmd_RemoveQuad)
            cCrowdMan2d_removeQuad_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
    }
    return luno_gLunoNil_3BA0;
}
#endif

/**
 * Does nothing (an empty function).
 *
 * SetGamePathFunc, the script command that takes a game path, calls it with the path, so setting the game path from a
 * script has no effect in this build.
 */
extern "C" void setGamePath(void) {
}

//100%
INCLUDE_ASM("seg/seg_1FBE38", isGamePath);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_3D90(void) __asm__("cBE_getBE");
cBEInterface* cBE_getInterface_Fv_3D90(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int isGamePath_map_3D90[] __asm__("isGamePath_map");
/**
 * Returns whether script game path @p i is the current game path.
 *
 * For @p i below 4 it maps @p i through isGamePath_map (0, 1, 2, 0) and compares it with the race DB's cGamePath
 * (newRace.cGamePath); 4 and above give false. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game path (0-3)
 * @return true when it matches
 */
extern "C" bool isGamePath(unsigned i) {
    if (i < 4) {
        cBE_getInterface_Fv_3D90(cBE_getBE_3D90(), 0);
        return isGamePath_map_3D90[i] == cBENewRaceInterface_newRace.cGamePath;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", isGameMode);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_3DF8(void) __asm__("cBE_getBE");
cBEInterface* cBE_getInterface_Fv_3DF8(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int isGameMode_map_3DF8[] __asm__("isGameMode_map");
/**
 * Returns whether script game mode @p i is the current game mode.
 *
 * For @p i below 8 it maps @p i through isGameMode_map (0-6, then 0) and compares it with the race DB's cGameMode
 * (newRace.cGameMode); 8 and above give false. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game mode (0-7)
 * @return true when it matches
 */
extern "C" bool isGameMode(unsigned i) {
    if (i < 8) {
        cBE_getInterface_Fv_3DF8(cBE_getBE_3DF8(), 0);
        return isGameMode_map_3DF8[i] == cBENewRaceInterface_newRace.cGameMode;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", isGameEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_3E60(void) __asm__("cBE_getBE");
cBEInterface* cBE_getInterface_Fv_3E60(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int isGameEvent_map_3E60[] __asm__("isGameEvent_map");
/**
 * Returns whether script game event @p i is the current game event.
 *
 * For @p i below 15 it maps @p i through isGameEvent_map (0-13, then 0) and compares it with the race DB's cGameEvent
 * (newRace.cGameEvent); 15 and above give false. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game event (0-14)
 * @return true when it matches
 */
extern "C" bool isGameEvent(unsigned i) {
    if (i < 0xF) {
        cBE_getInterface_Fv_3E60(cBE_getBE_3E60(), 0);
        return isGameEvent_map_3E60[i] == cBENewRaceInterface_newRace.cGameEvent;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderIsHumanFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int* luno_cLunoVar_castInt(void*);
static inline bool isIdle_3EC8(int slot) {
    if (slot != -1) {
        cRider *r = SSXApp->Game->AI->Riders[slot];
        cBeing *b = r;
        cRider *d = (cRider*)b;
        return !d->isComputer();
    }
    return false;
}
/**
 * Script function (gLunoCFunctions[72]): returns whether the script's rider is human.
 *
 * Takes no script arguments. The int result in the hidden result slot self is 1 when cWScriptMan_TheWScriptMan->Being
 * is not -1 and that rider (SSXApp->Game->AI->Riders[Being]) is not a computer rider (cRider virtual isComputer), else
 * 0.
 *
 * @return self
 */
extern "C" void* RiderIsHumanFunc(luno_cLunoVar* self) {
    if (isIdle_3EC8(cWScriptMan_TheWScriptMan->Being)) {
        ((luno_cLunoVar*)self)->Type = 1;
        *luno_cLunoVar_castInt(self) = 1;
    } else {
        ((luno_cLunoVar*)self)->Type = 1;
        *luno_cLunoVar_castInt(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderInTunnelFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_3F80 __asm__("luno_gLunoNil");
/**
 * Script function (gLunoCFunctions[74]): marks the script's rider as being in a tunnel.
 *
 * Takes no script arguments. When cWScriptMan_TheWScriptMan->Being is not -1, sets that rider's InTunnel (the word
 * cRider_setInTunnel and cRider_isInTunnel use) to 1. cRider_updateSplit1 clears it again.
 *
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector RiderInTunnelFunc(void) {
    int slot = cWScriptMan_TheWScriptMan->Being;
    if (slot != kWScriptBeing_None) {
        cRider *riderEntry = SSXApp->Game->AI->Riders[slot];
        cBeing *riderIface = riderEntry;
        cRider *rider = (cRider*)riderIface;
        rider->InTunnel = 1;
    }
    return luno_gLunoNil_3F80;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayPointsFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of UpdateDisplayPointsFunc (On Tour tUpdateDisplayPointsData; same offsets): the block below fills
// it.
struct tUpdateDisplayPointsData {
    int Points;                     // 0x00 [O]
    int Goal;                       // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tUpdateDisplayPointsData, Points, 0x00);
CHECK_OFFSET(tUpdateDisplayPointsData, Goal, 0x04);
CHECK_OFFSET(tUpdateDisplayPointsData, null, 0x08);
CHECK_SIZE(tUpdateDisplayPointsData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_3FF0 __asm__("luno_gLunoNil");

extern int D_004A3D40_3FF0[2] __asm__("D_004A3D40");
void cWScriptMan_updateMissionPoints(void*, int, int);
extern int D_004A57E4_3FF0 __asm__("D_004A57E4");
extern tUpdateDisplayPointsData sDisplayPointsDefaults __asm__("D_004FBC98");
/**
 * Script function (gLunoCFunctions[98]): shows the mission points display with a value and a goal.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayPointsData: Points, Goal) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_004A3D40 says eLunoVar_Float. Defaults: Points 0, Goal -1.
 *
 * Calls cWScriptMan_updateMissionPoints(cWScriptMan_TheWScriptMan, Points, Goal), which sets MissionPointsEnable,
 * MissionPoints and MissionPointsGoal.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar UpdateDisplayPointsFunc(int n, luno_cLunoCArg* params) {
    if (D_004A57E4_3FF0 == 0) {
        sDisplayPointsDefaults.Points = 0;
        sDisplayPointsDefaults.Goal = -1;
        D_004A57E4_3FF0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDisplayPointsDefaults.null = WScriptNullType(sDisplayPointsDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sDisplayPointsDefaults, sizeof(tUpdateDisplayPointsData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D40_3FF0[k]);
    }
    cWScriptMan_updateMissionPoints(cWScriptMan_TheWScriptMan, vp[0], vp[1]);
    return luno_gLunoNil_3FF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayCountFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of UpdateDisplayCountFunc (On Tour tUpdateDisplayCountData; same offsets): the block below fills
// it.
struct tUpdateDisplayCountData {
    int Count;                      // 0x00 [O]
    int Max;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tUpdateDisplayCountData, Count, 0x00);
CHECK_OFFSET(tUpdateDisplayCountData, Max, 0x04);
CHECK_OFFSET(tUpdateDisplayCountData, null, 0x08);
CHECK_SIZE(tUpdateDisplayCountData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_4100 __asm__("luno_gLunoNil");

extern int D_004A3D48_4100[2] __asm__("D_004A3D48");
void cWScriptMan_updateMissionCount(void*, int, int);
extern int D_004A57E8_4100 __asm__("D_004A57E8");
extern tUpdateDisplayCountData sDisplayCountDefaults __asm__("D_004FBCA8");
/**
 * Script function (gLunoCFunctions[75]): shows the mission count display with a count and a maximum.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayCountData: Count, Max) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3D48 says eLunoVar_Float. Defaults: Count 0, Max 0.
 *
 * Calls cWScriptMan_updateMissionCount(cWScriptMan_TheWScriptMan, Count, Max), which sets MissionCountEnable,
 * MissionCount and MissionCountMax.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar UpdateDisplayCountFunc(int n, luno_cLunoCArg* params) {
    if (D_004A57E8_4100 == 0) {
        sDisplayCountDefaults.Count = 0;
        D_004A57E8_4100 = 1;
        sDisplayCountDefaults.Max = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDisplayCountDefaults.null = WScriptNullType(sDisplayCountDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sDisplayCountDefaults, sizeof(tUpdateDisplayCountData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D48_4100[k]);
    }
    cWScriptMan_updateMissionCount(cWScriptMan_TheWScriptMan, vp[0], vp[1]);
    return luno_gLunoNil_4100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayTicksFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of UpdateDisplayTicksFunc (On Tour tUpdateDisplayTicksData; same offsets): the block below fills
// it.
struct tUpdateDisplayTicksData {
    int Ticks;                      // 0x00 [O]
    int Dir;                        // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tUpdateDisplayTicksData, Ticks, 0x00);
CHECK_OFFSET(tUpdateDisplayTicksData, Dir, 0x04);
CHECK_OFFSET(tUpdateDisplayTicksData, null, 0x08);
CHECK_SIZE(tUpdateDisplayTicksData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_4210 __asm__("luno_gLunoNil");

extern int D_004A3D50_4210[2] __asm__("D_004A3D50");
void cWScriptMan_updateMissionTicks(void*, int, int);
extern int D_004A57EC_4210 __asm__("D_004A57EC");
extern tUpdateDisplayTicksData sDisplayTicksDefaults __asm__("D_004FBCB8");
/**
 * Script function (gLunoCFunctions[76]): shows the mission clock with a tick count and a direction.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayTicksData: Ticks, Dir) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3D50 says eLunoVar_Float. Defaults: Ticks 0, Dir -1.
 *
 * Calls cWScriptMan_updateMissionTicks(cWScriptMan_TheWScriptMan, Ticks, Dir), which sets MissionTicksEnable,
 * MissionTicks and MissionTicksDir.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar UpdateDisplayTicksFunc(int n, luno_cLunoCArg* params) {
    if (D_004A57EC_4210 == 0) {
        sDisplayTicksDefaults.Ticks = 0;
        sDisplayTicksDefaults.Dir = -1;
        D_004A57EC_4210 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDisplayTicksDefaults.null = WScriptNullType(sDisplayTicksDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sDisplayTicksDefaults, sizeof(tUpdateDisplayTicksData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D50_4210[k]);
    }
    cWScriptMan_updateMissionTicks(cWScriptMan_TheWScriptMan, vp[0], vp[1]);
    return luno_gLunoNil_4210;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayHeightFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

static inline tInstanceRecord *refToPtr_1FBE38(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *refToObj_1FBE38(unsigned i) { if (i == 0) return 0; return refToPtr_1FBE38(i); }
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A57F0_4320 __asm__("D_004A57F0");
extern WScriptNullType D_004A57F4_4320 __asm__("D_004A57F4");
extern int D_004A57F8_4320 __asm__("D_004A57F8");
extern int UpdateDisplayHeightFunc_types[2];
// Inline accessor of the script manager, cWScriptMan_TheWScriptMan (wscript/wscriptman cWScriptMan_getTheWScriptMan is
// its
// out-of-line copy). Calling it at each use gives the shared copy of the pointer the target keeps (lw; move) instead of
// one cached local.
static inline cWScriptMan *curProcess_4320() { return cWScriptMan_TheWScriptMan; }
void cWScriptMan_updateMissionHeight(void*, int, void*);
/**
 * Script function (gLunoCFunctions[81]): shows the mission height display for the script's rider above a world
 * instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour
 * tUpdateDisplayHeightData: InstId) and lets each argument override the field its index names, turning an int into a
 * float where the field type table UpdateDisplayHeightFunc_types says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cWScriptMan_updateMissionHeight(cWScriptMan_TheWScriptMan, Being, instance) sets MissionHeightEnable,
 * MissionHeightBeing and MissionHeightInstId.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" cLunoValue UpdateDisplayHeightFunc(int n, luno_cLunoCArg* params) {
    if (D_004A57F8_4320 == 0) {
        D_004A57F0_4320 = 0xFFFFFFFF;
        D_004A57F8_4320 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor's stack temp
        //       (lbu; sb 0(sp)).
        D_004A57F4_4320 = WScriptNullType(D_004A57F4_4320);
    }
    unsigned buf64[2];
    unsigned* buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy(buf, &D_004A57F0_4320, 8);
    luno_cLunoCArg* x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = UpdateDisplayHeightFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    int gv = curProcess_4320()->Being;
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = curProcess_4320()->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = refToObj_1FBE38(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj)
        cWScriptMan_updateMissionHeight(curProcess_4320(), gv, obj);
    return luno_gLunoNil;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", PassthroughGateFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_44A8 __asm__("luno_gLunoNil");

extern int D_004A3D60_44A8[2] __asm__("D_004A3D60");
extern "C" void cGameModeMan_riderGateHit(cGameModeMan*, int, int);
extern int D_004A5808_44A8 __asm__("D_004A5808");
extern int D_004A5800_44A8 __asm__("D_004A5800");
extern WScriptNullType D_004A5804_44A8 __asm__("D_004A5804");
/**
 * Script function (gLunoCFunctions[82]): reports that the script's rider passed a gate.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tPassthroughGateData:
 * GateId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D60 says eLunoVar_Float. Defaults: GateId -1.
 *
 * Calls cGameModeMan_riderGateHit(SSXApp->GameModeMan, cWScriptMan_TheWScriptMan->Being, GateId), which forwards it to
 * the current game-mode handler.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar PassthroughGateFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5808_44A8 == 0) {
        D_004A5800_44A8 = -1;
        D_004A5808_44A8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5804_44A8 = WScriptNullType(D_004A5804_44A8);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5800_44A8, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D60_44A8[k]);
    }
    int beingIndex = cWScriptMan_TheWScriptMan->Being;
    cGameModeMan_riderGateHit(SSXApp->GameModeMan, beingIndex, vp[0]);
    return luno_gLunoNil_44A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayTrickFunc);
#ifdef SKIP_ASM
// PORT: 64-bit bitfield container (unsigned long is 8 bytes on the EE).
// World-script data that names the mission's current trick (On Tour tUpdateDisplayTrickData; same offsets): the
// block below fills it.
struct tUpdateDisplayTrickData {
    int Rail;                       // 0x00 [O]
    int RailSpin;                   // 0x04 [O]
    int TakeOff;                    // 0x08 [O]
    int Orient;                     // 0x0C [O]
    int Spin;                       // 0x10 [O]
    int Reps;                       // 0x14 [O]
    int Flip;                       // 0x18 [O]
    int Special;                    // 0x1C [O]
    int PostSpin;                   // 0x20 [O]
    int UberGrind;                  // 0x24 [O]
    int Grab;                       // 0x28 [O]
    int ToLate;                     // 0x2C [O]
    int LateGrab;                   // 0x30 [O]
    int HandPlant;                  // 0x34 [O]
    int Land;                       // 0x38 [O]
    int Press;                      // 0x3C [O]
    WScriptNullType null;           // 0x40 [O]
    char unk_041[0x3];              // 0x41 padding
};

CHECK_OFFSET(tUpdateDisplayTrickData, Rail, 0x00);
CHECK_OFFSET(tUpdateDisplayTrickData, RailSpin, 0x04);
CHECK_OFFSET(tUpdateDisplayTrickData, TakeOff, 0x08);
CHECK_OFFSET(tUpdateDisplayTrickData, Orient, 0x0C);
CHECK_OFFSET(tUpdateDisplayTrickData, Spin, 0x10);
CHECK_OFFSET(tUpdateDisplayTrickData, Reps, 0x14);
CHECK_OFFSET(tUpdateDisplayTrickData, Flip, 0x18);
CHECK_OFFSET(tUpdateDisplayTrickData, Special, 0x1C);
CHECK_OFFSET(tUpdateDisplayTrickData, PostSpin, 0x20);
CHECK_OFFSET(tUpdateDisplayTrickData, UberGrind, 0x24);
CHECK_OFFSET(tUpdateDisplayTrickData, Grab, 0x28);
CHECK_OFFSET(tUpdateDisplayTrickData, ToLate, 0x2C);
CHECK_OFFSET(tUpdateDisplayTrickData, LateGrab, 0x30);
CHECK_OFFSET(tUpdateDisplayTrickData, HandPlant, 0x34);
CHECK_OFFSET(tUpdateDisplayTrickData, Land, 0x38);
CHECK_OFFSET(tUpdateDisplayTrickData, Press, 0x3C);
CHECK_OFFSET(tUpdateDisplayTrickData, null, 0x40);
CHECK_SIZE(tUpdateDisplayTrickData, 0x44);
extern t3Vector luno_gLunoNil_45B8 __asm__("luno_gLunoNil");
extern tUpdateDisplayTrickData sDisplayTrickDefaults __asm__("D_004FBCC8");
extern int D_580C_45B8 __asm__("D_004A580C");
extern const int UpdateDisplayTrickFunc_types_45B8[] __asm__("UpdateDisplayTrickFunc_types");
extern "C" void* MEM_copy_45B8(void*, void*, int) __asm__("MEM_copy");
extern "C" void cWScriptMan_updateMissionTrick_45B8(cWScriptMan*,
        tTrickNameDef*) __asm__("cWScriptMan_updateMissionTrick");
/**
 * Script function (gLunoCFunctions[83]): shows the mission trick display for a trick built from script arguments.
 *
 * Copies the static defaults (set up on the first call) into a 68-byte parameter block (On Tour
 * tUpdateDisplayTrickData: Rail, RailSpin, TakeOff, Orient, Spin, Reps, Flip, Special, PostSpin, UberGrind, Grab,
 * ToLate, LateGrab, HandPlant, Land, Press) and lets each argument override the field its index names, turning an int
 * into a float where the field type table UpdateDisplayTrickFunc_types says eLunoVar_Float. Defaults: all 0.
 *
 * Packs the 16 values into an 8-byte trick definition (bit fields; Land and Press are stored in the opposite order of
 * the block) and passes it to cWScriptMan_updateMissionTrick, which sets MissionTrickEnable and MissionTrick.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector UpdateDisplayTrickFunc(int n, luno_cLunoCArg *params) {
    if (D_580C_45B8 == 0) {
        sDisplayTrickDefaults.Rail = 0;
        sDisplayTrickDefaults.RailSpin = 0;
        sDisplayTrickDefaults.TakeOff = 0;
        sDisplayTrickDefaults.Orient = 0;
        sDisplayTrickDefaults.Spin = 0;
        sDisplayTrickDefaults.Reps = 0;
        sDisplayTrickDefaults.Flip = 0;
        sDisplayTrickDefaults.Special = 0;
        sDisplayTrickDefaults.PostSpin = 0;
        sDisplayTrickDefaults.UberGrind = 0;
        sDisplayTrickDefaults.Grab = 0;
        sDisplayTrickDefaults.ToLate = 0;
        sDisplayTrickDefaults.LateGrab = 0;
        sDisplayTrickDefaults.HandPlant = 0;
        sDisplayTrickDefaults.Land = 0;
        sDisplayTrickDefaults.Press = 0;
        D_580C_45B8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sDisplayTrickDefaults.null = WScriptNullType(sDisplayTrickDefaults.null);
    }
    tUpdateDisplayTrickData rec;
    tUpdateDisplayTrickData *buf = &rec;
    MEM_copy_45B8(buf, &sDisplayTrickDefaults, sizeof(tUpdateDisplayTrickData));
    tTrickNameDef trick;
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = UpdateDisplayTrickFunc_types_45B8[idx];
        setArgField(&fields[idx], x, keywordType);
    }
    trick.rail = buf->Rail;
    trick.railSpin = buf->RailSpin;
    trick.takeoff = buf->TakeOff;
    trick.orient = buf->Orient;
    trick.spin = buf->Spin;
    trick.reps = buf->Reps;
    trick.flip = buf->Flip;
    trick.special = buf->Special;
    trick.postSpin = buf->PostSpin;
    trick.uberGrind = buf->UberGrind;
    trick.grab = buf->Grab;
    trick.toLate = buf->ToLate;
    trick.lateGrab = buf->LateGrab;
    trick.handplant = buf->HandPlant;
    trick.land = buf->Land;
    trick.press = buf->Press;
    trick.monster = 0;
    cWScriptMan_updateMissionTrick_45B8(cWScriptMan_TheWScriptMan, &trick);
    return luno_gLunoNil_45B8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddFadeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cMoveNode_addFade(void*, void*);
extern t3Vector luno_gLunoNil_4980 __asm__("luno_gLunoNil");
extern tAddFadeData sFadeDefaults __asm__("D_004FBD10");
extern int D_004A5810;
extern int D_004A3D68;
extern "C" void* MEM_copy_4980(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[84]): adds a fade modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAddFadeData: InstId,
 * Fade) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D68 says eLunoVar_Float. Defaults: InstId -1, Fade 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode), cMoveNode_addFade gets the block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddFadeFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5810 == 0) {
        sFadeDefaults.InstId = 0xFFFFFFFF;
        sFadeDefaults.Fade = 1.0f;
        D_004A5810 = 1;
        sFadeDefaults.null = WScriptNullType(sFadeDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_4980(buf, &sFadeDefaults, sizeof(tAddFadeData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3D68)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode == 0)
        return luno_gLunoNil_4980;
    if (activeNode->isMoveNode())
        cMoveNode_addFade(activeNode, buf);
    return luno_gLunoNil_4980;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ReplayFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_4B38 __asm__("luno_gLunoNil");

extern int D_004A3D70_4B38[2] __asm__("D_004A3D70");
extern "C" void cReplay_eventWScript(cReplay*, int, int);
extern int D_004A5820_4B38 __asm__("D_004A5820");
extern int D_004A5818_4B38 __asm__("D_004A5818");
extern WScriptNullType D_004A581C_4B38 __asm__("D_004A581C");
/**
 * Script function (gLunoCFunctions[85]): records a world-script event in the replay.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tReplayData: Cmd) and
 * lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D70 says eLunoVar_Float. Defaults: Cmd -1.
 *
 * Calls cReplay_eventWScript(SSXApp->Game->Replay, cWScriptMan_TheWScriptMan->Being, Cmd).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar ReplayFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5820_4B38 == 0) {
        D_004A5818_4B38 = -1;
        D_004A5820_4B38 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A581C_4B38 = WScriptNullType(D_004A581C_4B38);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5818_4B38, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D70_4B38[k]);
    }
    cReplay_eventWScript(SSXApp->Game->Replay, cWScriptMan_TheWScriptMan->Being, vp[0]);
    return luno_gLunoNil_4B38;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderHeightFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* luno_cLunoVar_castFloat(void*);
extern "C" void MEM_copy(void*, void*, int);
extern unsigned D_5828_4C50 __asm__("D_004A5828");
extern WScriptNullType D_582C_4C50 __asm__("D_004A582C");
extern int D_5830_4C50 __asm__("D_004A5830");
extern int D_3D78_4C50 __asm__("D_004A3D78");
static inline t4Vector pos_4C50(tInstanceRecord *o) { return *(t4Vector*)o->m_fmTransform.m[3]; }
/**
 * Script function (gLunoCFunctions[86]): returns how high the script's rider is above a world instance.
 *
 * Copies the static defaults (InstId -1; set up on the first call) into an 8-byte parameter block and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D78 says
 * eLunoVar_Float.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The float result in the hidden result slot self is the z of the
 * script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) (cRider virtual getPosition) minus the
 * instance's position z (m_fmTransform row 3), 0 when the rider slot is empty, and 0 when the instance is not found.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the height difference
 */
extern "C" void* RiderHeightFunc(luno_cLunoVar* self, int n, luno_cLunoCArg *params) {
    if (D_5830_4C50 == 0) {
        D_5828_4C50 = 0xFFFFFFFF;
        D_5830_4C50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_582C_4C50 = WScriptNullType(D_582C_4C50);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy(buf, &D_5828_4C50, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_3D78_4C50)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    int slot = cWScriptMan_TheWScriptMan->Being;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (obj) {
        cRider *rr = SSXApp->Game->AI->Riders[slot];
        cBeing *riderIface = rr;
        cRider *rider = (cRider*)riderIface;
        float heightDiff;
        if (rider) {
            float z = rider->getPosition()->z;
            heightDiff = z - pos_4C50(obj).z;
        }
        else
            heightDiff = 0.0f;
        self->Type = 2;
        *(float*)luno_cLunoVar_castFloat(self) = heightDiff;
    } else {
        self->Type = 2;
        *(int*)luno_cLunoVar_castFloat(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddDebounceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cMoveNode_addDebounce_4E38(void*, void*) __asm__("cMoveNode_addDebounce");
extern t3Vector luno_gLunoNil_4E38 __asm__("luno_gLunoNil");
extern tAddDebounceData sAddDebounceDefaults __asm__("D_004FBD20");
extern int D_004A5834;
extern int D_004A3D80;
extern "C" void* MEM_copy_4E38(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[87]): adds a debounce modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAddDebounceData:
 * InstId, Time) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D80 says eLunoVar_Float. Defaults: InstId -1, Time 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (m_pActiveNode) is a move node
 * (virtual isMoveNode), cMoveNode_addDebounce gets the block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddDebounceFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5834 == 0) {
        sAddDebounceDefaults.InstId = 0xFFFFFFFF;
        sAddDebounceDefaults.Time = 1.0f;
        D_004A5834 = 1;
        sAddDebounceDefaults.null = WScriptNullType(sAddDebounceDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_4E38(buf, &sAddDebounceDefaults, sizeof(tAddDebounceData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3D80)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    cInstanceNode *activeNode = obj->m_pActiveNode;
    if (activeNode == 0)
        return luno_gLunoNil_4E38;
    if (activeNode->isMoveNode())
        cMoveNode_addDebounce_4E38(activeNode, buf);
    return luno_gLunoNil_4E38;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CameraFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of CameraFunc (On Tour tCameraData; same offsets): the block below fills it.
struct tCameraData {
    int Cmd;                        // 0x00 [O]
    int Type;                       // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tCameraData, Cmd, 0x00);
CHECK_OFFSET(tCameraData, Type, 0x04);
CHECK_OFFSET(tCameraData, null, 0x08);
CHECK_SIZE(tCameraData, 0xC);
extern int D_004A5838_4FF0 __asm__("D_004A5838");
extern int D_004A3D88_4FF0[2] __asm__("D_004A3D88");
extern "C" void MEM_copy(void*, void*, int);

extern tCameraData sCameraDefaults __asm__("D_004FBD30");
extern luno_cLunoVar luno_gLunoNil_4FF0 __asm__("luno_gLunoNil");

/**
 * Script function (gLunoCFunctions[89]): reads its arguments and does nothing else (a stubbed camera command).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCameraData: Cmd, Type)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D88 says eLunoVar_Float. Defaults: Cmd 0, Type 0.
 *
 * No camera call follows; the block is unused.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar CameraFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5838_4FF0 == 0) {
        sCameraDefaults.Cmd = 0;
        D_004A5838_4FF0 = 1;
        sCameraDefaults.Type = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sCameraDefaults.null = WScriptNullType(sCameraDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sCameraDefaults, sizeof(tCameraData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D88_4FF0[k]);
    }
    return luno_gLunoNil_4FF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CameraShakeFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// World-script data that shakes the camera (On Tour tCameraShakeData; same offsets): the block below fills it.
struct tCameraShakeData {
    int Type;                       // 0x00 [O]
    int Proximity;                  // 0x04 [O]
    unsigned int InstId;            // 0x08 [O] tResourceID
    float Scale;                    // 0x0C [O]
    WScriptNullType null;           // 0x10 [O]
    char unk_011[0x3];              // 0x11 padding
};

CHECK_OFFSET(tCameraShakeData, Type, 0x00);
CHECK_OFFSET(tCameraShakeData, Proximity, 0x04);
CHECK_OFFSET(tCameraShakeData, InstId, 0x08);
CHECK_OFFSET(tCameraShakeData, Scale, 0x0C);
CHECK_OFFSET(tCameraShakeData, null, 0x10);
CHECK_SIZE(tCameraShakeData, 0x14);
#include "bx/math3d.h"
extern cLunoValue luno_gLunoNil;
// A plain (not 16-byte aligned) 4-float vector built in place for the node calls that take a vector pointer.
struct sVec4_1FBE38 { float x, y, z, w; sVec4_1FBE38(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern void* cMultiParticleMan_TheMultiParticleMan;
// One script argument as the VM passes it to a binding: the slot index in the defaults record, then the value
// (cLunoValue layout: value word, second word, type).
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_1FBE38(const t4Vector &a, const t4Vector &b) {
    t4Vector result;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(result)
        : "m"(a), "m"(b));
    return result;
}
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_1FBE38(const t4Vector &v) {
    float length;
    int bits;
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
        : "=f"(length), "=&r"(bits)
        : "m"(v));
    return length;
}
// True when the race slot (-1 = none) holds a rider that is not computer-controlled.
// Defaults record of this camera-shake binding; keyword indexes the shake keyword table (the prototype asserts
// dp->Index < kNumCameraShakeKeywords).
extern tCameraShakeData sCameraShakeDefaults __asm__("D_004FBD40");
extern int D_004A583C;
extern const int D_00446690[];
extern int D_004A3BD8[2];  // shake type per camera-shake keyword
extern "C" void cCamera_shakeCamera(cCamera* cam, int type, float amount, float dist);
// Translation row of a transform (by value).
static inline t4Vector trans_50F0(const t4Matrix *m) { return m->row[3]; }
/**
 * Script function (gLunoCFunctions[91]): shakes the cameras of local riders, scaled by their distance from a world
 * instance, or of the script's rider only.
 *
 * Copies the static defaults (set up on the first call) into a 20-byte parameter block (On Tour tCameraShakeData: Type,
 * Proximity, InstId, Scale) and lets each argument override the field its index names, turning an int into a float
 * where the field type table D_00446690 says eLunoVar_Float. Defaults: Type 0, Proximity 1, InstId -1, Scale 1.
 *
 * Only Type 0 does anything (shake type D_004A3BD8[Type]). With Proximity set the instance is looked up (InstId -1:
 * cWScriptMan_TheWScriptMan->Inst). With an instance, each race player's rider (SSXApp->Game->AI->Players[i]->mRider,
 * NumberOfPlayersAllocated of them) that is local (mIsLocal) gets cCamera_shakeCamera on its view's camera
 * (ViewMan->PlayerCamera[mViewIndex]) with weight 1 and its distance to the instance's root transform (node virtual
 * getRootTransform, or the instance's own transform) times Scale. Without one, only the player whose rider's AI index
 * (virtual getAIIndex) is the script's Being shakes, at distance 0.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" cLunoValue CameraShakeFunc(int n, luno_cLunoCArg *params) {
    if (D_004A583C == 0) {
        sCameraShakeDefaults.Type = 0;
        sCameraShakeDefaults.Proximity = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor's stack temp
        //       (lbu; sb 0(sp)).
        sCameraShakeDefaults.null = WScriptNullType(sCameraShakeDefaults.null);
        sCameraShakeDefaults.Scale = 1.0f;
        sCameraShakeDefaults.InstId = 0xFFFFFFFF;
        D_004A583C = 1;
    }
    tCameraShakeData rec;
    tCameraShakeData *buf = &rec;
    rec.InstId = 0xFFFFFFFF;
    MEM_copy(buf, &sCameraShakeDefaults, sizeof(tCameraShakeData));
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446690[idx];
        setArgField(&fields[idx], x, keywordType);
    }
    tInstanceRecord *obj = 0;
    if (buf->Proximity) {
        unsigned id = buf->InstId;
        if (~id == 0) {
            obj = cWScriptMan_TheWScriptMan->Inst;
        } else {
            cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
            tInstanceRecord *instance;
            if (!section)
                instance = 0;
            else
                instance = refToObj_1FBE38(section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc);
            obj = instance;
        }
    }
    int being = cWScriptMan_TheWScriptMan->Being;
    float scale = buf->Scale;
    int keyword = buf->Type;
    if (keyword == 0) {
        cGame *game = SSXApp->Game;
        int np = game->AI->NumberOfPlayersAllocated;
        if (obj) {
            for (int i = 0; i < np; i++) {
                const t4Vector *rp = game->AI->Players[i]->mRider->getPosition();
                cInstanceNode *node = obj->m_pActiveNode;
                const t4Matrix *rootTransform = node ? node->getRootTransform() : (const t4Matrix*)&obj->m_fmTransform;
                t4Vector delta = vSub_1FBE38(*rp, trans_50F0(rootTransform));
                float dist = vLen_1FBE38(delta);
                cRider *rider = game->AI->Players[i]->mRider;
                int view = rider->mViewIndex;
                if (rider->mIsLocal)
                    cCamera_shakeCamera(game->ViewMan->PlayerCamera[view], D_004A3BD8[keyword], 1.0f, dist * scale);
            }
        } else {
            for (int i = 0; i < np; i++) {
                if (game->AI->Players[i]->mRider->getAIIndex() == being) {
                    cRider *rider = game->AI->Players[i]->mRider;
                    int view = rider->mViewIndex;
                    if (rider->mIsLocal)
                        cCamera_shakeCamera(game->ViewMan->PlayerCamera[view], D_004A3BD8[keyword], 1.0f, 0.0f);
                    break;
                }
            }
        }
    }
    return luno_gLunoNil;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddMagnetFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data that makes a node a magnet (On Tour tAddMagnetData; same offsets): the block below fills it.
struct tAddMagnetData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    float Radius;                   // 0x04 [O]
    float Speed;                    // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tAddMagnetData, InstId, 0x00);
CHECK_OFFSET(tAddMagnetData, Radius, 0x04);
CHECK_OFFSET(tAddMagnetData, Speed, 0x08);
CHECK_OFFSET(tAddMagnetData, null, 0x0C);
CHECK_SIZE(tAddMagnetData, 0x10);
extern t3Vector luno_gLunoNil_5478 __asm__("luno_gLunoNil");
extern tAddMagnetData sMagnetDefaults __asm__("D_004FBD58");
extern int D_5840_5478 __asm__("D_004A5840");
extern const int AddMagnetFunc_types_5478[] __asm__("AddMagnetFunc_types");
extern "C" void* MEM_copy_5478(void*, void*, int) __asm__("MEM_copy");
extern "C" void cMoveNode_addMagnet_5478(cInstanceNode*, const t4Matrix*, float, float) __asm__("cMoveNode_addMagnet");
/**
 * Script function (gLunoCFunctions[90]): adds a magnet modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAddMagnetData: InstId,
 * Radius, Speed) and lets each argument override the field its index names, turning an int into a float where the field
 * type table AddMagnetFunc_types says eLunoVar_Float. Defaults: InstId -1, Radius 400, Speed 30.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (m_pActiveNode) is
 * a move node (virtual isMoveNode), cMoveNode_addMagnet gets the node's root transform (virtual getRootTransform),
 * Radius and Speed.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddMagnetFunc(int n, luno_cLunoCArg *params) {
    if (D_5840_5478 == 0) {
        sMagnetDefaults.InstId = 0xFFFFFFFF;
        sMagnetDefaults.Radius = 400.0f;
        sMagnetDefaults.Speed = 30.0f;
        D_5840_5478 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMagnetDefaults.null = WScriptNullType(sMagnetDefaults.null);
    }
    tAddMagnetData rec;
    tAddMagnetData *buf = &rec;
    rec.InstId = 0xFFFFFFFF;
    MEM_copy_5478(buf, &sMagnetDefaults, sizeof(tAddMagnetData));
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = AddMagnetFunc_types_5478[idx];
        setArgField(&fields[idx], x, keywordType);
    }
    unsigned id = buf->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addMagnet_5478(activeNode, activeNode->getRootTransform(), buf->Radius, buf->Speed);
        }
    }
    return luno_gLunoNil_5478;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", LightningFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void DoLightningFlash_5660(float) __asm__("DoLightningFlash");
extern t3Vector luno_gLunoNil_5660 __asm__("luno_gLunoNil");
extern unsigned D_5660 __asm__("D_004A5848");
extern WScriptNullType D_004A584C;
extern int D_004A5850;
extern int D_004A3D90;
extern "C" void* MEM_copy_5660(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[92]): starts a lightning flash when the script's rider is human.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tLightningData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D90 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId is not used. When cWScriptMan_TheWScriptMan->Being is not -1 and that rider (SSXApp->Game->AI->Riders[Being])
 * is human (cRider virtual isHuman), DoLightningFlash(0) starts a flash at distance 0.
 *
 * Quirk: an empty rider slot is not checked; its virtual call goes through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector LightningFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5850 == 0) {
        D_5660 = 0xFFFFFFFF;
        D_004A5850 = 1;
        D_004A584C = WScriptNullType(D_004A584C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_5660(buf, &D_5660, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3D90)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    int k = cWScriptMan_TheWScriptMan->Being;
    if (k != kWScriptBeing_None) {
        cAI *ai = SSXApp->Game->AI;
        cRider *riderEntry = ai->Riders[k];
        cBeing *sh = riderEntry;
        if (sh->isHuman())
            DoLightningFlash_5660(0.0f);
    }
    return luno_gLunoNil_5660;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddHaloFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_57C0 __asm__("luno_gLunoNil");
extern tAddHaloData sHaloDefaults __asm__("D_004FBD68");
extern int D_5854_57C0 __asm__("D_004A5854");
extern const int AddHaloFunc_types_57C0[] __asm__("AddHaloFunc_types");
extern "C" void* MEM_copy_57C0(void*, void*, int) __asm__("MEM_copy");
extern "C" void cMoveNode_addHalo_57C0(void*, void*) __asm__("cMoveNode_addHalo");
/**
 * Script function (gLunoCFunctions[97]): adds a halo modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 40-byte parameter block (On Tour tAddHaloData: InstId,
 * Type, Mesh, ColourA, ColourR, ColourG, ColourB, Radius, Speed) and lets each argument override the field its index
 * names, turning an int into a float where the field type table AddHaloFunc_types says eLunoVar_Float. Defaults: InstId -1,
 * Type 0, Mesh -1, ColourA, ColourR, ColourG and ColourB 1, Radius 100, Speed 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (m_pActiveNode) is
 * a move node (virtual isMoveNode), cMoveNode_addHalo gets the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddHaloFunc(int n, luno_cLunoCArg *params) {
    if (D_5854_57C0 == 0) {
        sHaloDefaults.InstId = 0xFFFFFFFF;
        sHaloDefaults.Type = 0;
        sHaloDefaults.Mesh = -1;
        sHaloDefaults.ColourA = 1.0f;
        sHaloDefaults.ColourR = 1.0f;
        sHaloDefaults.ColourG = 1.0f;
        sHaloDefaults.ColourB = 1.0f;
        sHaloDefaults.Radius = 100.0f;
        sHaloDefaults.Speed = 0;
        D_5854_57C0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sHaloDefaults.null = WScriptNullType(sHaloDefaults.null);
    }
    tAddHaloData rec;
    tAddHaloData *buf = &rec;
    rec.InstId = 0xFFFFFFFF;
    MEM_copy_57C0(buf, &sHaloDefaults, sizeof(tAddHaloData));
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = AddHaloFunc_types_57C0[idx];
        setArgField(&fields[idx], x, keywordType);
    }
    unsigned id = buf->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addHalo_57C0(activeNode, buf);
        }
    }
    return luno_gLunoNil_57C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MiniAvalancheFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// World-script data that adds up to 8 instances to a mini avalanche, one cAvalanche_addAvalancheNode call per slot.
// On Tour's MiniAvalancheFunc is an empty stub, so the type name is ours; the members take the names of
// addAvalancheNode's On Tour parameters (avalancheId, inst, life, fadein, fadeout, speed, type) that each one feeds.
struct tMiniAvalancheData {
    int AvalancheId;                // 0x00 [U] -> addAvalancheNode's avalancheId
    unsigned int InstId[8];         // 0x04 [U] tResourceID of each slot's instance; kInvalidRID skips the slot
    int Type[8];                    // 0x24 [U] -> type
    float Life[8];                  // 0x44 [U] -> life
    float FadeIn[8];                // 0x64 [U] -> fadein
    float FadeOut[8];               // 0x84 [U] -> fadeout
    float Speed[8];                 // 0xA4 [U] -> speed
    WScriptNullType null;           // 0xC4 the block's end marker
    char unk_0C5[0x3];              // 0xC5 padding
};
CHECK_OFFSET(tMiniAvalancheData, InstId, 0x04);
CHECK_OFFSET(tMiniAvalancheData, Speed, 0xA4);
CHECK_OFFSET(tMiniAvalancheData, null, 0xC4);
CHECK_SIZE(tMiniAvalancheData, 0xC8);
extern tMiniAvalancheData sMiniAvalancheDefaults __asm__("D_004FBD90");
extern const int D_004466D8_59A0[] __asm__("D_004466D8");
extern int D_004A5858_59A0 __asm__("D_004A5858");
static inline tInstanceRecord *conv_59A0(unsigned i) { return i ? refToPtr_1FBE38(i) : 0; }
extern "C" void cAvalanche_addAvalancheNode(int, tInstanceRecord*, float, float, float, float, int);
/**
 * Script function (gLunoCFunctions[93]): adds up to eight world instances as falling nodes of an avalanche.
 *
 * Copies the static defaults (set up on the first call) into a 200-byte parameter block and lets each argument override
 * the field its index names, turning an int into a float where the field type table D_004466D8 says eLunoVar_Float. The
 * block holds the avalanche id (default 0) and eight sets of instance id (-1), type (0), life (10 s), fade-in (0 s),
 * fade-out (3 s) and speed (1).
 *
 * For each instance id that is not -1, the instance is looked up (a missing one passes 0) and
 * cAvalanche_addAvalancheNode(avalanche id, instance, life, fade-in, fade-out, speed, type) adds it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" cLunoValue MiniAvalancheFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5858_59A0 == 0) {
        sMiniAvalancheDefaults.AvalancheId = 0;
        sMiniAvalancheDefaults.InstId[7] = sMiniAvalancheDefaults.InstId[6] = sMiniAvalancheDefaults.InstId[5] =
                sMiniAvalancheDefaults.InstId[4] = sMiniAvalancheDefaults.InstId[3] = sMiniAvalancheDefaults.InstId[2] =
                sMiniAvalancheDefaults.InstId[1] = sMiniAvalancheDefaults.InstId[0] = 0xFFFFFFFF;
        sMiniAvalancheDefaults.Type[7] = sMiniAvalancheDefaults.Type[6] = sMiniAvalancheDefaults.Type[5] =
                sMiniAvalancheDefaults.Type[4] = sMiniAvalancheDefaults.Type[3] = sMiniAvalancheDefaults.Type[2] =
                sMiniAvalancheDefaults.Type[1] = sMiniAvalancheDefaults.Type[0] = 0;
        sMiniAvalancheDefaults.Life[7] = sMiniAvalancheDefaults.Life[6] = sMiniAvalancheDefaults.Life[5] =
                sMiniAvalancheDefaults.Life[4] = sMiniAvalancheDefaults.Life[3] = sMiniAvalancheDefaults.Life[2] =
                sMiniAvalancheDefaults.Life[1] = sMiniAvalancheDefaults.Life[0] = 10.0f;
        sMiniAvalancheDefaults.FadeIn[7] = sMiniAvalancheDefaults.FadeIn[6] = sMiniAvalancheDefaults.FadeIn[5] =
                sMiniAvalancheDefaults.FadeIn[4] = sMiniAvalancheDefaults.FadeIn[3] = sMiniAvalancheDefaults.FadeIn[2] =
                sMiniAvalancheDefaults.FadeIn[1] = sMiniAvalancheDefaults.FadeIn[0] = 0.0f;
        sMiniAvalancheDefaults.FadeOut[7] = sMiniAvalancheDefaults.FadeOut[6] = sMiniAvalancheDefaults.FadeOut[5] =
                sMiniAvalancheDefaults.FadeOut[4] = sMiniAvalancheDefaults.FadeOut[3] =
                sMiniAvalancheDefaults.FadeOut[2] = sMiniAvalancheDefaults.FadeOut[1] =
                sMiniAvalancheDefaults.FadeOut[0] = 3.0f;
        sMiniAvalancheDefaults.Speed[7] = sMiniAvalancheDefaults.Speed[6] = sMiniAvalancheDefaults.Speed[5] =
                sMiniAvalancheDefaults.Speed[4] = sMiniAvalancheDefaults.Speed[3] = sMiniAvalancheDefaults.Speed[2] =
                sMiniAvalancheDefaults.Speed[1] = sMiniAvalancheDefaults.Speed[0] = 1.0f;
        D_004A5858_59A0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor's stack temp
        //       (lbu; sb 0(sp)).
        sMiniAvalancheDefaults.null = WScriptNullType(sMiniAvalancheDefaults.null);
    }
    tMiniAvalancheData rec;
    tMiniAvalancheData* vp = &rec;
    rec.InstId[7] = rec.InstId[6] = rec.InstId[5] = rec.InstId[4] = rec.InstId[3] = rec.InstId[2] = rec.InstId[1] =
            rec.InstId[0] = 0xFFFFFFFF;
    MEM_copy(vp, &sMiniAvalancheDefaults, sizeof(tMiniAvalancheData));
    int *fields = (int*)vp;
    luno_cLunoCArg* x = params;
    for (int i = 0; i < n; i++, x++) {
        int k = x->Index;
        setArgField(&fields[k], x, D_004466D8_59A0[k]);
    }
    int avalancheId = vp->AvalancheId;
    int* type = vp->Type;
    float* speed = vp->Speed;
    float* fadeOut = vp->FadeOut;
    float* fadeIn = vp->FadeIn;
    float* life = vp->Life;
    for (int i = 0; i < 8; i++) {
        unsigned id = vp->InstId[i];
        if (id != tResourceID::kInvalidRID) {
            cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
            tInstanceRecord *inst;
            if (!section)
                inst = 0;
            else {
                unsigned j = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
                inst = conv_59A0(j);
            }
            cAvalanche_addAvalancheNode(avalancheId, inst, *life, *fadeIn, *fadeOut, *speed, *type);
        }
        type++; speed++; fadeOut++; fadeIn++; life++;
    }
    return luno_gLunoNil;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", TriggerMiniAvalancheFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_5C88 __asm__("luno_gLunoNil");

extern int D_004A3D98_5C88[2] __asm__("D_004A3D98");
extern "C" int cAvalanche_triggerAvalanche(int);
extern int cAvalanche_m_badCookie_5C88 __asm__("cAvalanche_m_badCookie");
extern int D_004A5868_5C88 __asm__("D_004A5868");
extern int D_004A5860_5C88 __asm__("D_004A5860");
extern WScriptNullType D_004A5864_5C88 __asm__("D_004A5864");
/**
 * Script function (gLunoCFunctions[94]): triggers an avalanche (cAvalanche_triggerAvalanche).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (the avalanche id) and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D98 says
 * eLunoVar_Float. Defaults: avalanche id 0.
 *
 * Calls cAvalanche_triggerAvalanche with the avalanche id when cAvalanche_m_badCookie is 0.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar TriggerMiniAvalancheFunc(int n, luno_cLunoCArg* params) {
    if (D_004A5868_5C88 == 0) {
        D_004A5868_5C88 = 1;
        D_004A5860_5C88 = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5864_5C88 = WScriptNullType(D_004A5864_5C88);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5860_5C88, 8);
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3D98_5C88[k]);
    }
    int avalancheId = vp[0];
    if (cAvalanche_m_badCookie_5C88 == 0)
        cAvalanche_triggerAvalanche(avalancheId);
    return luno_gLunoNil_5C88;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddAvaSplineFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *refToPtr_5D90(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_5D90(unsigned i) { if (i == 0) return 0; return refToPtr_5D90(i); }
extern "C" void cMoveNode_addAvaSpline_5D90(void*, void*) __asm__("cMoveNode_addAvaSpline");
extern t3Vector luno_gLunoNil_5D90 __asm__("luno_gLunoNil");
extern unsigned D_5D90 __asm__("D_004A5870");
extern WScriptNullType D_004A5874;
extern int D_004A5878;
extern int D_004A3DA0;
extern "C" void* MEM_copy_5D90(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[95]): puts a moving world instance on the avalanche spline.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (InstId) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_004A3DA0 says
 * eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (m_pActiveNode) is
 * a move node (virtual isMoveNode), cMoveNode_addAvaSpline gets the node and the instance (which it does not read).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector AddAvaSplineFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5878 == 0) {
        D_5D90 = 0xFFFFFFFF;
        D_004A5878 = 1;
        D_004A5874 = WScriptNullType(D_004A5874);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_5D90(buf, &D_5D90, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = (&D_004A3DA0)[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section)
            inst = 0;
        else {
            i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            tInstanceRecord *found = conv_5D90(i);
            inst = found;
        }
        obj = inst;
    }
    if (obj) {
        cInstanceNode *activeNode = obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->isMoveNode())
                cMoveNode_addAvaSpline_5D90(activeNode, obj);
        }
    }
    return luno_gLunoNil_5D90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetAvalancheEmitterDataFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern t3Vector luno_gLunoNil_5F40 __asm__("luno_gLunoNil");
// The block is laid out as AddParticleFunc's tAddParticleData (the particle data, then the instance at 0xD8) and gets
// the same defaults; On Tour's SetAvalancheEmitterDataFunc is an empty stub, so its own type name is unknown.
extern tAddParticleData sAvalancheEmitterDefaults __asm__("D_004FBE58");
extern int D_587C_5F40 __asm__("D_004A587C");
extern const int SetAvalancheEmitterDataFunc_types_5F40[] __asm__("SetAvalancheEmitterDataFunc_types");
extern "C" void* MEM_copy_5F40(void*, void*, int) __asm__("MEM_copy");
extern "C" void cAvalanche_setAvalancheNodeEmitterData_5F40(tInstanceRecord*, tAddParticleData*)
    __asm__("cAvalanche_setAvalancheNodeEmitterData");
static inline tInstanceRecord *refToPtr_5F40(unsigned i) { return (tInstanceRecord*)(i << 2); }
static inline tInstanceRecord *conv_5F40(unsigned i) { if (i == 0) return 0; return refToPtr_5F40(i); }
/**
 * Script function (gLunoCFunctions[96]): sets the particle-emitter parameters of the avalanche node bound to a world
 * instance.
 *
 * Copies the static defaults (set up on the first call) into a 224-byte parameter block and lets each argument override
 * the field its index names, turning an int into a float where the field type table SetAvalancheEmitterDataFunc_types says eLunoVar_Float. The
 * block has the layout of AddParticleFunc's tAddParticleData: emitter parameters first (NumParticles 1, Duration -1,
 * Damp 1, Size 4, ...), then InstId at +0xD8 (default -1).
 *
 * When InstId is not -1 the instance is looked up (no current-instance shortcut; a missing one passes 0) and
 * cAvalanche_setAvalancheNodeEmitterData copies the parameters to its node.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector SetAvalancheEmitterDataFunc(int n, luno_cLunoCArg *params) {
    if (D_587C_5F40 == 0) {
        tAddParticleData &defaultBlock = sAvalancheEmitterDefaults;
        float zero = 0.0f;
        float one = 1.0f;
        defaultBlock.NumParticles = 1;
        defaultBlock.NumBlur = 0;
        defaultBlock.Duration = -1.0f;
        defaultBlock.Damp = one;
        defaultBlock.Size = 4.0f;
        defaultBlock.Life = zero;
        defaultBlock.SizeR = zero;
        defaultBlock.LifeR = zero;
        defaultBlock.BlurStep = zero;
        defaultBlock.OffX = zero;
        defaultBlock.OffY = zero;
        defaultBlock.OffZ = zero;
        defaultBlock.R0X = zero;
        defaultBlock.R0Y = zero;
        defaultBlock.R0Z = zero;
        defaultBlock.R1X = zero;
        defaultBlock.R1Y = zero;
        defaultBlock.R1Z = zero;
        defaultBlock.VelX = zero;
        defaultBlock.VelY = zero;
        defaultBlock.VelZ = zero;
        defaultBlock.R0VX = zero;
        defaultBlock.R0VY = zero;
        defaultBlock.R0VZ = zero;
        defaultBlock.R1VX = zero;
        defaultBlock.R1VY = zero;
        defaultBlock.R1VZ = zero;
        defaultBlock.R2VX = zero;
        defaultBlock.R2VY = zero;
        defaultBlock.R2VZ = zero;
        defaultBlock.ForceX = zero;
        defaultBlock.ForceY = zero;
        defaultBlock.ForceZ = zero;
        defaultBlock.StartColA = one;
        defaultBlock.StartColR = zero;
        defaultBlock.StartColG = zero;
        defaultBlock.StartColB = zero;
        defaultBlock.EndColA = one;
        defaultBlock.EndColR = one;
        defaultBlock.EndColG = one;
        defaultBlock.EndColB = one;
        defaultBlock.R0A = one;
        defaultBlock.R0R = zero;
        defaultBlock.R0G = zero;
        defaultBlock.R0B = zero;
        defaultBlock.R1A = one;
        defaultBlock.R1R = zero;
        defaultBlock.R1G = zero;
        defaultBlock.R1B = zero;
        defaultBlock.TextureId = 16;
        defaultBlock.BlendMode = 0;
        defaultBlock.SizeFinal = zero;
        defaultBlock.NumFlipTextures = 1;
        defaultBlock.FlipTextureRate = 20.0f;
        defaultBlock.InstId = 0xFFFFFFFF;
        D_587C_5F40 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        defaultBlock.null = WScriptNullType(defaultBlock.null);
    }
    tAddParticleData rec;
    tAddParticleData *buf = &rec;
    rec.InstId = 0xFFFFFFFF;
    MEM_copy_5F40(buf, &sAvalancheEmitterDefaults, sizeof(tAddParticleData));
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = SetAvalancheEmitterDataFunc_types_5F40[idx];
        setArgField(&fields[idx], x, keywordType);
    }
    unsigned id = buf->InstId;
    if (~id != 0) {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        tInstanceRecord *obj;
        if (!section)
            obj = 0;
        else
            obj = conv_5F40(section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc);
        cAvalanche_setAvalancheNodeEmitterData_5F40(obj, buf);
    }
    return luno_gLunoNil_5F40;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsIconEnabledFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        luno_cLunoCArg* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->Index; \
            setArgField(&vp[k_], p_, (types)[k_]); \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.

extern int D_004A3DA8_61B0[2] __asm__("D_004A3DA8");
extern int D_005308D0_61B0[] __asm__("D_005308D0");
extern int D_004A5888_61B0 __asm__("D_004A5888");
extern int D_004A5880_61B0 __asm__("D_004A5880");
extern WScriptNullType D_004A5884_61B0 __asm__("D_004A5884");
/**
 * Script function (gLunoCFunctions[99]): returns whether a type of on-course icon is enabled by the custom race rules.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsIconEnabledData:
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3DA8 says eLunoVar_Float. Defaults: Type 0.
 *
 * Type kIconType_Multiplier, kIconType_PowerUp and kIconType_Points test bits 6, 8 and 7 of the rule word D_005308D0
 * (rules 10, 12 and 11 of cFEStateRuleSelect_updateBE): the int result in @p ret is 1 when the bit is clear, else 0.
 * Other types give 1.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsIconEnabledFunc(cLunoValue* ret, int n, luno_cLunoCArg *params) {
    if (D_004A5888_61B0 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor's stack temp
        //       (lbu; sb 0(sp)).
        D_004A5880_61B0 = 0;
        D_004A5884_61B0 = WScriptNullType(D_004A5884_61B0);
        D_004A5888_61B0 = 1;
    }
    int fields[4];
    int *vp;
    BIND_UNPACK_ARGS_1FBE38(vp, fields, &D_004A5880_61B0, 8, D_004A3DA8_61B0, n, params);
    int enabled;
    switch (vp[0]) {
    case kIconType_Multiplier: {
        int flags = D_005308D0_61B0[0] >> 6;
        enabled = 0;
        if ((flags & 1) == 0)
            enabled++;
        break;
    }
    case kIconType_PowerUp: {
        int flags = D_005308D0_61B0[0] >> 8;
        enabled = 0;
        if ((flags & 1) == 0)
            enabled++;
        break;
    }
    case kIconType_Points: {
        int flags = D_005308D0_61B0[0] >> 7;
        enabled = 0;
        if ((flags & 1) == 0)
            enabled++;
        break;
    }
    default:
        enabled = 1;
        break;
    }
    BIND_SET_RESULT_1FBE38(ret, enabled);
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MessageCenterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of MessageCenterFunc (On Tour tMessageCenterData; same offsets): the block below fills it.
struct tMessageCenterData {
    unsigned int TitleId;           // 0x00 [O]
    unsigned int MsgId;             // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tMessageCenterData, TitleId, 0x00);
CHECK_OFFSET(tMessageCenterData, MsgId, 0x04);
CHECK_OFFSET(tMessageCenterData, null, 0x08);
CHECK_SIZE(tMessageCenterData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_6300 __asm__("luno_gLunoNil");

extern int D_004A3DB0_6300[2] __asm__("D_004A3DB0");
extern "C" void requestMessageCenter(int, int);
extern "C" void* cBE_getInterface_6300(cBE*, int) __asm__("cBE_getInterface__Fv");
extern int D_004A588C_6300 __asm__("D_004A588C");
extern tMessageCenterData sMessageCenterDefaults __asm__("D_004FBF38");
/**
 * Script function (gLunoCFunctions[100]): shows a message-centre popup, except in a network game.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMessageCenterData:
 * TitleId, MsgId) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3DB0 says eLunoVar_Float. Defaults: TitleId 0, MsgId 0.
 *
 * Unless cBENetworkInterface_networkOpt.bIsNetworkGame is set, calls requestMessageCenter(TitleId, MsgId) (fe/fepopup3). The
 * cBE_getInterface call before it has no effect.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" luno_cLunoVar MessageCenterFunc(int n, luno_cLunoCArg* params) {
    if (D_004A588C_6300 == 0) {
        sMessageCenterDefaults.TitleId = 0;
        D_004A588C_6300 = 1;
        sMessageCenterDefaults.MsgId = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMessageCenterDefaults.null = WScriptNullType(sMessageCenterDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sMessageCenterDefaults, sizeof(tMessageCenterData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3DB0_6300[k]);
    }
    int x = vp[0];
    int y = vp[1];
    cBE_getInterface_6300(SSXApp->BE, 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0)
        requestMessageCenter(x, y);
    return luno_gLunoNil_6300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DangerStartFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cDangerMan_AddDangerElem_6438(void*, void*, float, float) __asm__("cDangerMan_AddDangerElem");
extern t3Vector luno_gLunoNil_6438 __asm__("luno_gLunoNil");
// World-script data that starts a danger zone (On Tour tDangerStartData; same offsets): the block below fills it.
struct tDangerStartData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    float Radius;                   // 0x04 [O]
    float Delay;                    // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tDangerStartData, InstId, 0x00);
CHECK_OFFSET(tDangerStartData, Radius, 0x04);
CHECK_OFFSET(tDangerStartData, Delay, 0x08);
CHECK_OFFSET(tDangerStartData, null, 0x0C);
CHECK_SIZE(tDangerStartData, 0x10);
extern tDangerStartData sDangerStartDefaults __asm__("D_004FBF48");
extern int D_004A5890;
extern const int D_00446880[];
extern "C" void* MEM_copy_6438(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[101]): registers a world instance as a danger the AI riders track.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tDangerStartData:
 * InstId, Radius, Delay) and lets each argument override the field its index names, turning an int into a float where
 * the field type table D_00446880 says eLunoVar_Float. Defaults: InstId -1, Radius 1000, Delay 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_AddDangerElem on the AI's
 * danger manager (SSXApp->Game->AI->DangerMan) records its id (m_RID), Radius, Delay and position, replacing any
 * earlier record for it (a dead instance is skipped).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector DangerStartFunc(int n, luno_cLunoCArg *params) {
    if (D_004A5890 == 0) {
        sDangerStartDefaults.InstId = 0xFFFFFFFF;
        sDangerStartDefaults.Radius = 1000.0f;
        sDangerStartDefaults.Delay = 0.0f;
        D_004A5890 = 1;
        sDangerStartDefaults.null = WScriptNullType(sDangerStartDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    tDangerStartData *data = (tDangerStartData*)buf;
    data->InstId = 0xFFFFFFFF;
    MEM_copy_6438(buf, &sDangerStartDefaults, sizeof(tDangerStartData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446880[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            obj = 0;
        else
            obj = instanceAtMemLoc(i);
    }
    if (obj) {
        float radius = data->Radius;
        float delay = data->Delay;
        int instIdBuf[1];
        instIdBuf[0] = obj->m_RID;
        cDangerMan_AddDangerElem_6438(SSXApp->Game->AI->DangerMan, instIdBuf, radius, delay);
    }
    return luno_gLunoNil_6438;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DangerEndFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_65E8(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

extern "C" void cDangerMan_RemoveDangerElem_65E8(void*, void*) __asm__("cDangerMan_RemoveDangerElem");
extern t3Vector luno_gLunoNil_65E8 __asm__("luno_gLunoNil");
extern unsigned D_65E8 __asm__("D_004A5898");
extern WScriptNullType D_004A589C;
extern int D_004A58A0;
extern int D_004A3DB8;
extern "C" void* MEM_copy_65E8(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[102]): stops tracking a world instance as a danger.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tDangerEndData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3DB8 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_RemoveDangerElem on the
 * AI's danger manager (SSXApp->Game->AI->DangerMan) deletes its tracked danger record
 * (cDangerMan_DeleteTrackedDangerElemRecord).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector DangerEndFunc(int n, luno_cLunoCArg *params) {
    if (D_004A58A0 == 0) {
        D_65E8 = 0xFFFFFFFF;
        D_004A58A0 = 1;
        D_004A589C = WScriptNullType(D_004A589C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_65E8(buf, &D_65E8, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3DB8)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    tInstanceRecord *obj = look_65E8(buf[0]);
    if (obj) {
        int instIdBuf[1]; instIdBuf[0] = obj->m_RID;
        cDangerMan_RemoveDangerElem_65E8(SSXApp->Game->AI->DangerMan, instIdBuf);
    }
    return luno_gLunoNil_65E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EnableDangerZoneFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
static inline tInstanceRecord *look_6770(unsigned id) {
    if (~id == 0)
        return cWScriptMan_TheWScriptMan->Inst;
    cWorldCacheTable *s = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    unsigned i;
    if (!s || (i = s->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
        return 0;
    return (tInstanceRecord*)(i << 2);
}

extern "C" void cDangerMan_AddDangerVolume_6770(void*, void*, int) __asm__("cDangerMan_AddDangerVolume");
extern t3Vector luno_gLunoNil_6770 __asm__("luno_gLunoNil");
// World-script data that enables a danger zone (On Tour tEnableDangerZoneData; same offsets): the block below fills it.
struct tEnableDangerZoneData {
    unsigned int InstId;            // 0x00 [O] tResourceID
    int Level;                      // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tEnableDangerZoneData, InstId, 0x00);
CHECK_OFFSET(tEnableDangerZoneData, Level, 0x04);
CHECK_OFFSET(tEnableDangerZoneData, null, 0x08);
CHECK_SIZE(tEnableDangerZoneData, 0xC);
extern tEnableDangerZoneData sEnableDangerZoneDefaults __asm__("D_004FBF58");
extern int D_004A58A4;
extern int D_004A3DC0;
extern "C" void* MEM_copy_6770(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[103]): enables a danger zone around a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tEnableDangerZoneData:
 * InstId, Level) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3DC0 says eLunoVar_Float. Defaults: InstId -1, Level 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_AddDangerVolume on the
 * AI's danger manager (SSXApp->Game->AI->DangerMan) adds a zone for its id (m_RID) with Level, or counts one more
 * reference when the zone is already active.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector EnableDangerZoneFunc(int n, luno_cLunoCArg *params) {
    if (D_004A58A4 == 0) {
        sEnableDangerZoneDefaults.InstId = 0xFFFFFFFF;
        sEnableDangerZoneDefaults.Level = 0;
        D_004A58A4 = 1;
        sEnableDangerZoneDefaults.null = WScriptNullType(sEnableDangerZoneDefaults.null);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_6770(buf, &sEnableDangerZoneDefaults, sizeof(tEnableDangerZoneData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3DC0)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }

    if (obj) {
        int zoneLevel = buf[1];
        cDangerMan_AddDangerVolume_6770(SSXApp->Game->AI->DangerMan, &obj->m_RID, zoneLevel);
    }
    return luno_gLunoNil_6770;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DisableDangerZoneFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cDangerMan_RemoveDangerVolume_6908(void*, void*) __asm__("cDangerMan_RemoveDangerVolume");
extern t3Vector luno_gLunoNil_6908 __asm__("luno_gLunoNil");
extern unsigned D_6908 __asm__("D_004A58A8");
extern WScriptNullType D_004A58AC;
extern int D_004A58B0;
extern int D_004A3DC8;
extern "C" void* MEM_copy_6908(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[104]): disables a danger zone around a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tDisableDangerZoneData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3DC8 says eLunoVar_Float. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_RemoveDangerVolume on the
 * AI's danger manager (SSXApp->Game->AI->DangerMan) drops one reference to its zone and frees the zone at zero.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector DisableDangerZoneFunc(int n, luno_cLunoCArg *params) {
    if (D_004A58B0 == 0) {
        D_6908 = 0xFFFFFFFF;
        D_004A58B0 = 1;
        D_004A58AC = WScriptNullType(D_004A58AC);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_6908(buf, &D_6908, 8);
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = (&D_004A3DC8)[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = buf[0];
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (obj)
        cDangerMan_RemoveDangerVolume_6908(SSXApp->Game->AI->DangerMan, &obj->m_RID);
    return luno_gLunoNil_6908;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MultiParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data that adds a multi-particle effect (On Tour tMultiParticleData; same offsets): the block below
// fills it.
struct tMultiParticleData {
    int NumParticles;               // 0x00 [O]
    int NumBlur;                    // 0x04 [O]
    float Duration;                 // 0x08 [O]
    float Damp;                     // 0x0C [O]
    float Size;                     // 0x10 [O]
    float Life;                     // 0x14 [O]
    float SizeR;                    // 0x18 [O]
    float LifeR;                    // 0x1C [O]
    float BlurStep;                 // 0x20 [O]
    float OffX;                     // 0x24 [O]
    float OffY;                     // 0x28 [O]
    float OffZ;                     // 0x2C [O]
    float R0X;                      // 0x30 [O]
    float R0Y;                      // 0x34 [O]
    float R0Z;                      // 0x38 [O]
    float R1X;                      // 0x3C [O]
    float R1Y;                      // 0x40 [O]
    float R1Z;                      // 0x44 [O]
    float VelX;                     // 0x48 [O]
    float VelY;                     // 0x4C [O]
    float VelZ;                     // 0x50 [O]
    float R0VX;                     // 0x54 [O]
    float R0VY;                     // 0x58 [O]
    float R0VZ;                     // 0x5C [O]
    float R1VX;                     // 0x60 [O]
    float R1VY;                     // 0x64 [O]
    float R1VZ;                     // 0x68 [O]
    float R2VX;                     // 0x6C [O]
    float R2VY;                     // 0x70 [O]
    float R2VZ;                     // 0x74 [O]
    float ForceX;                   // 0x78 [O]
    float ForceY;                   // 0x7C [O]
    float ForceZ;                   // 0x80 [O]
    float StartColA;                // 0x84 [O]
    float StartColR;                // 0x88 [O]
    float StartColG;                // 0x8C [O]
    float StartColB;                // 0x90 [O]
    float EndColA;                  // 0x94 [O]
    float EndColR;                  // 0x98 [O]
    float EndColG;                  // 0x9C [O]
    float EndColB;                  // 0xA0 [O]
    float R0A;                      // 0xA4 [O]
    float R0R;                      // 0xA8 [O]
    float R0G;                      // 0xAC [O]
    float R0B;                      // 0xB0 [O]
    float R1A;                      // 0xB4 [O]
    float R1R;                      // 0xB8 [O]
    float R1G;                      // 0xBC [O]
    float R1B;                      // 0xC0 [O]
    int TextureId;                  // 0xC4 [O]
    int BlendMode;                  // 0xC8 [O]
    float SizeFinal;                // 0xCC [O]
    int NumFlipTextures;            // 0xD0 [O]
    float FlipTextureRate;          // 0xD4 [O]
    int Id;                         // 0xD8 [O]
    int MaxInst;                    // 0xDC [O]
    WScriptNullType null;           // 0xE0 [O]
    char unk_0E1[0x3];              // 0xE1 padding
};

CHECK_OFFSET(tMultiParticleData, NumParticles, 0x00);
CHECK_OFFSET(tMultiParticleData, NumBlur, 0x04);
CHECK_OFFSET(tMultiParticleData, Duration, 0x08);
CHECK_OFFSET(tMultiParticleData, Damp, 0x0C);
CHECK_OFFSET(tMultiParticleData, Size, 0x10);
CHECK_OFFSET(tMultiParticleData, Life, 0x14);
CHECK_OFFSET(tMultiParticleData, SizeR, 0x18);
CHECK_OFFSET(tMultiParticleData, LifeR, 0x1C);
CHECK_OFFSET(tMultiParticleData, BlurStep, 0x20);
CHECK_OFFSET(tMultiParticleData, OffX, 0x24);
CHECK_OFFSET(tMultiParticleData, OffY, 0x28);
CHECK_OFFSET(tMultiParticleData, OffZ, 0x2C);
CHECK_OFFSET(tMultiParticleData, R0X, 0x30);
CHECK_OFFSET(tMultiParticleData, R0Y, 0x34);
CHECK_OFFSET(tMultiParticleData, R0Z, 0x38);
CHECK_OFFSET(tMultiParticleData, R1X, 0x3C);
CHECK_OFFSET(tMultiParticleData, R1Y, 0x40);
CHECK_OFFSET(tMultiParticleData, R1Z, 0x44);
CHECK_OFFSET(tMultiParticleData, VelX, 0x48);
CHECK_OFFSET(tMultiParticleData, VelY, 0x4C);
CHECK_OFFSET(tMultiParticleData, VelZ, 0x50);
CHECK_OFFSET(tMultiParticleData, R0VX, 0x54);
CHECK_OFFSET(tMultiParticleData, R0VY, 0x58);
CHECK_OFFSET(tMultiParticleData, R0VZ, 0x5C);
CHECK_OFFSET(tMultiParticleData, R1VX, 0x60);
CHECK_OFFSET(tMultiParticleData, R1VY, 0x64);
CHECK_OFFSET(tMultiParticleData, R1VZ, 0x68);
CHECK_OFFSET(tMultiParticleData, R2VX, 0x6C);
CHECK_OFFSET(tMultiParticleData, R2VY, 0x70);
CHECK_OFFSET(tMultiParticleData, R2VZ, 0x74);
CHECK_OFFSET(tMultiParticleData, ForceX, 0x78);
CHECK_OFFSET(tMultiParticleData, ForceY, 0x7C);
CHECK_OFFSET(tMultiParticleData, ForceZ, 0x80);
CHECK_OFFSET(tMultiParticleData, StartColA, 0x84);
CHECK_OFFSET(tMultiParticleData, StartColR, 0x88);
CHECK_OFFSET(tMultiParticleData, StartColG, 0x8C);
CHECK_OFFSET(tMultiParticleData, StartColB, 0x90);
CHECK_OFFSET(tMultiParticleData, EndColA, 0x94);
CHECK_OFFSET(tMultiParticleData, EndColR, 0x98);
CHECK_OFFSET(tMultiParticleData, EndColG, 0x9C);
CHECK_OFFSET(tMultiParticleData, EndColB, 0xA0);
CHECK_OFFSET(tMultiParticleData, R0A, 0xA4);
CHECK_OFFSET(tMultiParticleData, R0R, 0xA8);
CHECK_OFFSET(tMultiParticleData, R0G, 0xAC);
CHECK_OFFSET(tMultiParticleData, R0B, 0xB0);
CHECK_OFFSET(tMultiParticleData, R1A, 0xB4);
CHECK_OFFSET(tMultiParticleData, R1R, 0xB8);
CHECK_OFFSET(tMultiParticleData, R1G, 0xBC);
CHECK_OFFSET(tMultiParticleData, R1B, 0xC0);
CHECK_OFFSET(tMultiParticleData, TextureId, 0xC4);
CHECK_OFFSET(tMultiParticleData, BlendMode, 0xC8);
CHECK_OFFSET(tMultiParticleData, SizeFinal, 0xCC);
CHECK_OFFSET(tMultiParticleData, NumFlipTextures, 0xD0);
CHECK_OFFSET(tMultiParticleData, FlipTextureRate, 0xD4);
CHECK_OFFSET(tMultiParticleData, Id, 0xD8);
CHECK_OFFSET(tMultiParticleData, MaxInst, 0xDC);
CHECK_OFFSET(tMultiParticleData, null, 0xE0);
CHECK_SIZE(tMultiParticleData, 0xE4);
extern t3Vector luno_gLunoNil_6A90 __asm__("luno_gLunoNil");
extern tMultiParticleData sMultiParticleDefaults __asm__("D_004FBF68");
extern int D_58B4_6A90 __asm__("D_004A58B4");
extern const int MultiParticleFunc_types_6A90[] __asm__("MultiParticleFunc_types");
extern "C" void* MEM_copy_6A90(void*, void*, int) __asm__("MEM_copy");
extern int cMultiParticleMan_TheMultiParticleMan_6A90 __asm__("cMultiParticleMan_TheMultiParticleMan");
extern "C" void cMultiParticleMan_addParticle_6A90(int, int, tMultiParticleData*,
        int) __asm__("cMultiParticleMan_addParticle");
/**
 * Script command MultiParticle: adds a particle system to the multi-particle manager.
 *
 * Copies the static tMultiParticleData defaults (D_004FBF68, set once under D_004A58B4: identity colour/vectors, 16,
 * 20.0, MaxInst 0x80, ...) into a local, applies the keyword arguments (types from MultiParticleFunc_types; int to float where the
 * type is eLunoVar_Float), and calls cMultiParticleMan_addParticle(TheMultiParticleMan, Id, data, MaxInst).
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" t3Vector MultiParticleFunc(int n, luno_cLunoCArg *params) {
    if (D_58B4_6A90 == 0) {
        tMultiParticleData &defaults = sMultiParticleDefaults;
        float zero = 0.0f;
        float one = 1.0f;
        defaults.NumParticles = 1;
        defaults.NumBlur = 0;
        defaults.Duration = -1.0f;
        defaults.Damp = one;
        defaults.Size = 4.0f;
        defaults.Life = zero;
        defaults.SizeR = zero;
        defaults.LifeR = zero;
        defaults.BlurStep = zero;
        defaults.OffX = zero;
        defaults.OffY = zero;
        defaults.OffZ = zero;
        defaults.R0X = zero;
        defaults.R0Y = zero;
        defaults.R0Z = zero;
        defaults.R1X = zero;
        defaults.R1Y = zero;
        defaults.R1Z = zero;
        defaults.VelX = zero;
        defaults.VelY = zero;
        defaults.VelZ = zero;
        defaults.R0VX = zero;
        defaults.R0VY = zero;
        defaults.R0VZ = zero;
        defaults.R1VX = zero;
        defaults.R1VY = zero;
        defaults.R1VZ = zero;
        defaults.R2VX = zero;
        defaults.R2VY = zero;
        defaults.R2VZ = zero;
        defaults.ForceX = zero;
        defaults.ForceY = zero;
        defaults.ForceZ = zero;
        defaults.StartColA = one;
        defaults.StartColR = zero;
        defaults.StartColG = zero;
        defaults.StartColB = zero;
        defaults.EndColA = one;
        defaults.EndColR = one;
        defaults.EndColG = one;
        defaults.EndColB = one;
        defaults.R0A = one;
        defaults.R0R = zero;
        defaults.R0G = zero;
        defaults.R0B = zero;
        defaults.R1A = one;
        defaults.R1R = zero;
        defaults.R1G = zero;
        defaults.R1B = zero;
        defaults.TextureId = 16;
        defaults.BlendMode = 0;
        defaults.SizeFinal = zero;
        defaults.NumFlipTextures = 1;
        defaults.FlipTextureRate = 20.0f;
        defaults.Id = 0;
        defaults.MaxInst = 0x80;
        D_58B4_6A90 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        defaults.null = WScriptNullType(defaults.null);
    }
    tMultiParticleData rec;
    tMultiParticleData *buf = &rec;
    MEM_copy_6A90(buf, &sMultiParticleDefaults, sizeof(tMultiParticleData));
    int *fields = (int*)buf;
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int argType = MultiParticleFunc_types_6A90[idx];
        setArgField(&fields[idx], x, argType);
    }
    cMultiParticleMan_addParticle_6A90(cMultiParticleMan_TheMultiParticleMan_6A90, buf->Id, buf, buf->MaxInst);
    return luno_gLunoNil_6A90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MultiParticleInstFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cMultiParticleMan_TheMultiParticleMan_6CB0 __asm__("cMultiParticleMan_TheMultiParticleMan");
void cMultiParticleMan_addInst(void*, int, int);
void cMultiParticleMan_remInst(void*, int, int);
extern t3Vector luno_gLunoNil_6CB0 __asm__("luno_gLunoNil");
// World-script data that adds or removes a multi-particle instance (On Tour tMultiParticleInstData; same offsets): the
// block below fills it.
struct tMultiParticleInstData {
    int Id;                         // 0x00 [O]
    unsigned int InstId;            // 0x04 [O] tResourceID
    int Cmd;                        // 0x08 [O]
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tMultiParticleInstData, Id, 0x00);
CHECK_OFFSET(tMultiParticleInstData, InstId, 0x04);
CHECK_OFFSET(tMultiParticleInstData, Cmd, 0x08);
CHECK_OFFSET(tMultiParticleInstData, null, 0x0C);
CHECK_SIZE(tMultiParticleInstData, 0x10);
extern tMultiParticleInstData sMultiParticleInstDefaults __asm__("D_004FC050");
extern int D_004A58B8;
extern const int D_00446970[];
extern "C" void* MEM_copy_6CB0(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[106]): attaches a world instance to a multi-particle effect, or detaches it.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMultiParticleInstData:
 * Id, InstId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_00446970 says eLunoVar_Float. Defaults: Id 0, InstId -1, Cmd 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, Cmd 0 calls
 * cMultiParticleMan_addInst(cMultiParticleMan_TheMultiParticleMan, Id, the instance's id m_RID) and any other Cmd
 * cMultiParticleMan_remInst.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" t3Vector MultiParticleInstFunc(int n, luno_cLunoCArg *params) {
    if (D_004A58B8 == 0) {
        sMultiParticleInstDefaults.Id = 0;
        sMultiParticleInstDefaults.InstId = 0xFFFFFFFF;
        sMultiParticleInstDefaults.Cmd = 0;
        D_004A58B8 = 1;
        sMultiParticleInstDefaults.null = WScriptNullType(sMultiParticleInstDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    tMultiParticleInstData *data = (tMultiParticleInstData*)buf;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_6CB0(buf, &sMultiParticleInstDefaults, sizeof(tMultiParticleInstData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int keywordType = D_00446970[idx];
        setArgField(&buf[idx], x, keywordType);
    }
    unsigned id = data->InstId;
    tInstanceRecord *obj;
    if (~id == 0) {
        obj = cWScriptMan_TheWScriptMan->Inst;
    } else {
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        unsigned i;
        tInstanceRecord *inst;
        if (!section || (i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc) == 0)
            inst = 0;
        else
            inst = instanceAtMemLoc(i);
        obj = inst;
    }
    if (obj) {
        if (data->Cmd == 0)
            cMultiParticleMan_addInst(cMultiParticleMan_TheMultiParticleMan_6CB0, data->Id, obj->m_RID);
        else
            cMultiParticleMan_remInst(cMultiParticleMan_TheMultiParticleMan_6CB0, data->Id, obj->m_RID);
    }
    return luno_gLunoNil_6CB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CheckGateFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cWScriptMan_checkGate_6E68(cWScriptMan*, int, int, int) __asm__("cWScriptMan_checkGate");
extern "C" void *cAudioInterface_bxAudio_6E68() __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_WorldSound_6E68(void*, void*, int) __asm__("cSSXAudio_WorldSound");
int *luno_cLunoVar_castInt(void*);
static inline char *refToPtr_6E68(unsigned i) { return (char*)(i << 2); }
static inline char *conv_6E68(unsigned i) { if (i == 0) return 0; return refToPtr_6E68(i); }
// World-script data that checks a gate (On Tour tCheckGateData; same offsets): the block below fills it.
struct tCheckGateData {
    unsigned int InstId1;           // 0x00 [O] tResourceID
    unsigned int InstId2;           // 0x04 [O] tResourceID
    unsigned int InstId3;           // 0x08 [O] tResourceID
    WScriptNullType null;           // 0x0C [O]
    char unk_00D[0x3];              // 0x0D padding
};

CHECK_OFFSET(tCheckGateData, InstId1, 0x00);
CHECK_OFFSET(tCheckGateData, InstId2, 0x04);
CHECK_OFFSET(tCheckGateData, InstId3, 0x08);
CHECK_OFFSET(tCheckGateData, null, 0x0C);
CHECK_SIZE(tCheckGateData, 0x10);
extern tCheckGateData sCheckGateDefaults __asm__("D_004FC060");
extern int CheckGateFunc_initFlag;
extern const int CheckGateFunc_types[];
extern "C" void* MEM_copy_6E68(void*, void*, int) __asm__("MEM_copy");
/**
 * Script function (gLunoCFunctions[107]): checks the script's rider against a gate (cWScriptMan_checkGate) and plays a
 * sound when it passed.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tCheckGateData:
 * InstId1, InstId2, InstId3) and lets each argument override the field its index names, turning an int into a float
 * where the field type table CheckGateFunc_types says eLunoVar_Float. Defaults: InstId1 -1, InstId2 -1, InstId3 -1.
 *
 * Returns cWScriptMan_checkGate(cWScriptMan_TheWScriptMan, InstId1, InstId2, InstId3) (previous, gate, next: 1 passed,
 * 0 near, -1 missed) as an int in @p ret; on 1 it plays world sound 92 at the gate instance InstId2
 * (cSSXAudio_WorldSound).
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" luno_cLunoVar* CheckGateFunc(luno_cLunoVar *ret, int n, luno_cLunoCArg *params) {
    if (CheckGateFunc_initFlag == 0) {
        sCheckGateDefaults.InstId1 = 0xFFFFFFFF;
        sCheckGateDefaults.InstId2 = 0xFFFFFFFF;
        sCheckGateDefaults.InstId3 = 0xFFFFFFFF;
        CheckGateFunc_initFlag = 1;
        sCheckGateDefaults.null = WScriptNullType(sCheckGateDefaults.null);
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    tCheckGateData *data = (tCheckGateData*)buf;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    MEM_copy_6E68(buf, &sCheckGateDefaults, sizeof(tCheckGateData));
    luno_cLunoCArg *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->Index;
        int expectedType = CheckGateFunc_types[idx];
        setArgField(&buf[idx], x, expectedType);
    }
    int gateId = data->InstId2;
    int nextGateId = data->InstId3;
    int gateResult = cWScriptMan_checkGate_6E68(cWScriptMan_TheWScriptMan, data->InstId1, gateId, nextGateId);
    if (gateResult == kGate_Passed) {
        unsigned id = data->InstId2;
        char *obj;
        cWorldCacheTable *section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
        if (!section) {
            obj = 0;
        } else {
            unsigned i = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
            char *found = conv_6E68(i);
            obj = found;
        }
        if (obj)
            cSSXAudio_WorldSound_6E68(cAudioInterface_bxAudio_6E68(), obj, 92);
    }
    ret->Type = eLunoVar_Int;
    *luno_cLunoVar_castInt(ret) = gateResult;
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ShowMessageIconFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// World-script data of ShowMessageIconFunc (On Tour tShowMessageIconData; same offsets): the block below fills it.
struct tShowMessageIconData {
    unsigned int TitleId;           // 0x00 [O]
    unsigned int MsgId;             // 0x04 [O]
    WScriptNullType null;           // 0x08 [O]
    char unk_009[0x3];              // 0x09 padding
};

CHECK_OFFSET(tShowMessageIconData, TitleId, 0x00);
CHECK_OFFSET(tShowMessageIconData, MsgId, 0x04);
CHECK_OFFSET(tShowMessageIconData, null, 0x08);
CHECK_SIZE(tShowMessageIconData, 0xC);
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern luno_cLunoVar luno_gLunoNil_7020 __asm__("luno_gLunoNil");

extern int D_004A3DD0_7020[2] __asm__("D_004A3DD0");
extern "C" int canOpenMessageCenter(int, int);
extern int D_004A58C0_7020 __asm__("D_004A58C0");
extern tShowMessageIconData sMessageIconDefaults __asm__("D_004FC070");
/**
 * Script function (gLunoCFunctions[110]): shows a message icon and returns whether it was shown.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tShowMessageIconData:
 * TitleId, MsgId) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3DD0 says eLunoVar_Float. Defaults: TitleId 0, MsgId 0.
 *
 * Returns 1 when canOpenMessageCenter(TitleId, MsgId) (fe/fepopup3) returns non-zero, else 0, as an int in the hidden result
 * slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 1 or 0
 */
extern "C" void* ShowMessageIconFunc(luno_cLunoVar* self, int n, luno_cLunoCArg* params) {
    if (D_004A58C0_7020 == 0) {
        sMessageIconDefaults.TitleId = 0;
        D_004A58C0_7020 = 1;
        sMessageIconDefaults.MsgId = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        sMessageIconDefaults.null = WScriptNullType(sMessageIconDefaults.null);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &sMessageIconDefaults, sizeof(tShowMessageIconData));
    luno_cLunoCArg* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->Index;
        setArgField(&vp[k], curArg, D_004A3DD0_7020[k]);
    }
    bool iconShown = canOpenMessageCenter(vp[0], vp[1]) != 0;
    self->Type = 1;
    *luno_cLunoVar_castInt(self) = iconShown;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_cWScriptMission);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char cWScriptMission_vtbl_7128[] __asm__("cWScriptMission_vtbl");
extern "C" void* cWScriptProcess_cWScriptProcess(void* self, void* man, int a1, const tWScriptProcessData* def);
extern "C" char* cWScriptMan_getScriptCode_7128(cWScriptMan*, int) __asm__("cWScriptMan_getScriptCode");
/**
 * Constructs a mission script process: the script-process base, the mission's ID and key, and its four script
 * entry points.
 *
 * @param man  the script manager (On Tour cWScriptMan* manager)
 * @param sectionId  the world section the script belongs to
 * @param data  the mission data (On Tour const tWScriptMissionData*)
 * @param key  the mission key
 * @return @p self
 */
extern "C" void* cWScriptMission_cWScriptMission(cWScriptMission* self, void* man, int sectionId,
                                                 const tWScriptMissionData* data, int key) {
    cWScriptProcess_cWScriptProcess(self, man, sectionId, data);
    self->mVtbl = (void**)cWScriptMission_vtbl_7128;
    self->Scope = data->Scope;
    self->SortKey = key;
    for (int i = 0; i < 4; i++)
        self->MissionScriptCode[i] = cWScriptMan_getScriptCode_7128(self->Manager, data->MissionScriptId[i]);
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission__cWScriptMission);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char cWScriptMission_vtbl_71C8[] __asm__("cWScriptMission_vtbl");
extern "C" void cWScriptProcess__cWScriptProcess_71C8(void*) __asm__("cWScriptProcess__cWScriptProcess");
/**
 * Destroys a mission script process: restores its vtable, then destroys the script-process base.
 *
 * There is no flags argument here; the base destructor gets it in the untouched second argument register.
 */
extern "C" void cWScriptMission__cWScriptMission(cWScriptMission* self) {
    self->mVtbl = (void**)cWScriptMission_vtbl_71C8;
    cWScriptProcess__cWScriptProcess_71C8(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_processStart);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cRider_flagEventMissionStart_71F0(void*) __asm__("cRider_flagEventMissionStart__FPv");
extern "C" void cWScriptProcess_processStart_71F0(void*) __asm__("cWScriptProcess_processStart");
/**
 * Starts a mission: shows HUD effect 0x1A for one second on the manager's rider (cRider_flagEventMissionStart), then
 * runs the base process start.
 */
void cWScriptMission::processStart()
{
    cRider_flagEventMissionStart_71F0(SSXApp->Game->AI->Riders[Manager->Being]);
    cWScriptProcess_processStart_71F0(this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_processComplete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_7240 {
    int state;
    char pad[0x58];
    // vtable slots 1..18; the named ones [R] cWScriptMission_vtbl
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void processEnd(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void missionPass();
};
int cWScriptMan_isProcessActive_7240(void*, void*) __asm__("cWScriptMan_isProcessActive__FPvT0");
extern "C" void *cAudioInterface_bxAudio_7240(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_BigChallengeComplete_7240(void*) __asm__("cSSXAudio_BigChallengeComplete");
/**
 * Completes a mission that the script manager still lists: runs its virtual slots 6 and 18 (processEnd,
 * missionPass), then calls cSSXAudio_BigChallengeComplete on the sound system.
 */
void cWScriptMission::processComplete()
{
    if (cWScriptMan_isProcessActive_7240(Manager, ((P_7240*)this))) {
        ((P_7240*)this)->processEnd();
        ((P_7240*)this)->missionPass();
        cSSXAudio_BigChallengeComplete_7240(cAudioInterface_bxAudio_7240());
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionRestart);
#ifdef SKIP_ASM
struct P_72B0 {
    int state;
    char pad[0x58];
    // vtable slots 1..6; the named ones [R] cWScriptMission_vtbl
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void processStart(); virtual void v05();
    virtual void processEnd();
};
/**
 * Restarts a mission script: ends it if it is running, then starts it again.
 *
 * When the State is kTaskState_Update (running) it calls the process's virtual processEnd (vtable +0x30,
 * cWScriptProcess_processEnd), then processStart (vtable +0x20, cWScriptMission_processStart).
 */
extern "C" void cWScriptMission_missionRestart(P_72B0* self) {
    if (self->state == kTaskState_Update)
        self->processEnd();
    self->processStart();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionPass);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_isMissionCompleted(void* missions, int id);
extern "C" void cBEMissionInterface_setIsCompleted(void* missions, int id, int done);
extern "C" int cBEMissionInterface_getDiffLevel(void* missions, int id);
extern "C" int cBEEconInterface_getMissionCash(void* econ, int reward);
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);
extern "C" void cRider_flagEventMissionSuccess(void* rider, int player, int money);
extern "C" unsigned int cBEMissionInterface_getNextMissionID(void* missions, int id);
extern "C" void cBEMissionInterface_setIsShowIcon(void* missions, int id, int active);
extern "C" void cWScriptMan_runScriptCode(cWScriptMan*, void *, char* scriptCode);
extern "C" int cBEMissionInterface_isMissionLocked(void* missions, int id);
extern "C" void cBEMissionInterface_setIsLocked(void* missions, int id, int a);
extern "C" void cBEMissionInterface_setIsReady(void* missions, int id, int a);
extern "C" void cWScriptMan_updateWorld(cWScriptMan* man);
extern "C" void cWScriptMan_processPass(cWScriptMan* man, void* self);
extern "C" void cReplay_reset(void* replay);
extern "C" void cReplay_startRace(void* replay);

/**
 * Handles a passed mission: records it and pays the reward, unlocks the next mission and runs the pass script.
 *
 * On BE interface 0xA (missions), for the mission ID (ProcessId): if not yet completed,
 * cBEMissionInterface_setIsCompleted marks it completed and the reward is cBEEconInterface_getMissionCash (econ
 * interface 0xB) of its cBEMissionInterface_getDiffLevel level. cRider_flagEventMissionSuccess flags the success on the
 * script's rider with the BE interface 0 value from cBENewRaceInterface_getLocationPeak1 and the money. If there is a
 * next mission (cBEMissionInterface_getNextMissionID != -1) this one's icon is hidden
 * (cBEMissionInterface_setIsShowIcon), the pass script (MissionScriptCode[1]) runs through cWScriptMan_runScriptCode,
 * and a locked next mission is unlocked, made ready and shown (cBEMissionInterface_setIsLocked,
 * cBEMissionInterface_setIsReady, cBEMissionInterface_setIsShowIcon); with no next mission only the pass script runs.
 * Ends with cWScriptMan_updateWorld and cWScriptMan_processPass on the script manager (Manager), then cReplay_reset and
 * cReplay_startRace on SSXApp->Game->Replay.
 *
 * On Tour also awards leaderboard points and a medal and triggers an overlay; retail has none of that.
 */
// Mission completed: pays the reward the first time, unlocks the follow-up mission and resets the script.
void cWScriptMission::missionPass()
{
    int money = 0;
    void* missions = cBE_getInterface_Fv(cBE_getBE(), 10);
    if (cBEMissionInterface_isMissionCompleted(missions, (int)ProcessId) == 0) {
        cBEMissionInterface_setIsCompleted(missions, (int)ProcessId, 1);
        int reward = cBEMissionInterface_getDiffLevel(missions, (int)ProcessId);
        money = cBEEconInterface_getMissionCash(cBE_getInterface_Fv(cBE_getBE(), 11), reward);
    }
    int pl = cBENewRaceInterface_getLocationPeak1(cBE_getInterface_Fv(cBE_getBE(), 0));
    cAI* ai = SSXApp->Game->AI;
    cRider_flagEventMissionSuccess(ai->Riders[Manager->Being], pl, money);
    unsigned int next = cBEMissionInterface_getNextMissionID(missions, (int)ProcessId);
    if (next != kWScriptId_None) {
        cBEMissionInterface_setIsShowIcon(missions, (int)ProcessId, 0);
        if (MissionScriptCode[1] != 0) {
            cWScriptMan_runScriptCode(Manager, this, MissionScriptCode[1]);
        }
        if (cBEMissionInterface_isMissionLocked(missions, next) != 0) {
            cBEMissionInterface_setIsLocked(missions, next, 0);
            cBEMissionInterface_setIsReady(missions, next, 1);
            cBEMissionInterface_setIsShowIcon(missions, next, 1);
        }
    } else if (MissionScriptCode[1] != 0) {
        cWScriptMan_runScriptCode(Manager, this, MissionScriptCode[1]);
    }
    cWScriptMan_updateWorld(Manager);
    cWScriptMan_processPass(Manager, this);
    cReplay_reset(SSXApp->Game->Replay);
    cReplay_startRace(SSXApp->Game->Replay);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionFail);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_74C0(void) __asm__("cBE_getBE");
int cBE_getInterface_Fv_74C0(int be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_isMissionAvailable_74C0(int, int) __asm__("cBEMissionInterface_isMissionAvailable");
extern "C" void cBEMissionInterface_setIsAvailable_74C0(int, int, int) __asm__("cBEMissionInterface_setIsAvailable");
extern "C" void cWScriptProcess_end_74C0(void*) __asm__("cWScriptProcess_end");
extern "C" void cWScriptMan_runScriptCode(cWScriptMan*, void *, char* scriptCode);
struct P_74C0 {
    int state;
    char pad[0x58];
    // vtable slots 1..20; the named ones [R] cWScriptMission_vtbl
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void missionUpdateWorld();
};
/**
 * Handles a failed mission: makes it available again, runs the fail script and ends the process.
 *
 * If the mission (ProcessId) is not available on BE interface 0xA, cBEMissionInterface_setIsAvailable sets it
 * available. The fail script (MissionScriptCode[2]) runs through cWScriptMan_runScriptCode if there is one; otherwise
 * the virtual at vtable +0xA0 (cWScriptMission_missionUpdateWorld, missionUpdateWorld) runs. A running process (State
 * kTaskState_Update) is then ended with cWScriptProcess_end.
 */
void cWScriptMission::missionFail()
{
    int iface = cBE_getInterface_Fv_74C0(cBE_getBE_74C0(), 10);
    if (!cBEMissionInterface_isMissionAvailable_74C0(iface, ProcessId))
        cBEMissionInterface_setIsAvailable_74C0(iface, ProcessId, 1);
    char* failScript = MissionScriptCode[2];
    if (failScript)
        cWScriptMan_runScriptCode(Manager, this, failScript);
    else
        ((P_74C0*)this)->missionUpdateWorld();
    if (State == kTaskState_Update)
        cWScriptProcess_end_74C0(this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionUpdateWorld);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(cWScriptMan*, void *, char* scriptCode);

/**
 * Runs the mission's update-world script, if it has one.
 *
 * Calls cWScriptMan_runScriptCode(Manager, this, code) with the fourth script entry point (MissionScriptCode[3]).
 * cWScriptMission_missionFail calls it through the vtable when the mission has no fail script.
 *
 */
void cWScriptMission::missionUpdateWorld()
{
    char* worldScript = MissionScriptCode[3];
    if (worldScript != 0) {
        cWScriptMan_runScriptCode(Manager, this, worldScript);
    }
}
#endif
