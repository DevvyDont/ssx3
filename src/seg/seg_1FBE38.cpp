#include "common.h"
#include "wscript/wscriptman.h"
#include "world/instancerecord.h"
#include "world/worldcache.h"
#include "wscript/wscriptmission.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/ssxapp.h"
#include "luno/lunovar.h"
//100%
INCLUDE_ASM("seg/seg_1FBE38", checkActiveNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern char *cWScriptMan_TheWScriptMan_AE38 __asm__("cWScriptMan_TheWScriptMan");
extern char D_00489618_AE38[] __asm__("D_00489618");
extern char D_00489628_AE38[] __asm__("D_00489628");
extern char D_00489638_AE38[] __asm__("D_00489638");
extern char D_00489648_AE38[] __asm__("D_00489648");
extern "C" void cParticleNode_cParticleNode1_AE38(void*, int, void*, void*, void*) __asm__("cParticleNode_cParticleNode1");
extern "C" void cMeshAnimNode_cMeshAnimNode1_AE38(void*, int, void*, void*, int, void*) __asm__("cMeshAnimNode_cMeshAnimNode1");
extern "C" void cDebounceNode_cDebounceNode1_AE38(void*, int, void*, void*, void*) __asm__("cDebounceNode_cDebounceNode1");
extern "C" void cDeadFadeNode_cDeadFadeNode_AE38(void*, int, void*, void*, void*) __asm__("cDeadFadeNode_cDeadFadeNode");
struct N_AE38 { int p0, p1, p2; virtual ~N_AE38(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); };
/**
 * Decides what to do with an object's existing node before a script attaches a node of the given type.
 *
 * Returns 1 (leave it) when the node is a dead-fade or another busy kind (9, 6, 22), or already of @p type
 * (it is
 * retriggered through slot 37). Particle (13), mesh-anim (11), debounce (0) and dead-fade (22) requests
 * replace it
 * with a new node built from @p buf and return -1; any other type deletes it and returns 0.
 *
 * @param node  the object's current node
 * @param type  the node type being requested
 * @param buf  the new node's parameter block
 * @return 1, -1 or 0 as above
 */
extern "C" int checkActiveNode(void *node, int type, void *buf) {
    N_AE38 *n = (N_AE38*)node;
    if (n->v10(9) || n->v10(6) || n->v10(22))
        return 1;
    if (n->v10(type)) {
        n->v37();
        return 1;
    }
    tInstanceRecord* inst = (tInstanceRecord*)(*(char**)((char*)n + 0x18));
    switch (type) {
    case 13:
        inst->m_pActiveNode = 0;
        cParticleNode_cParticleNode1_AE38(cMemMan_alloc(44, D_00489618_AE38, 0x20000000, 0), 1, inst, buf, n);
        return -1;
    case 11:
        inst->m_pActiveNode = 0;
        cMeshAnimNode_cMeshAnimNode1_AE38(cMemMan_alloc(1728, D_00489628_AE38, 0x20000000, 0), 1, inst, buf, *(int*)(cWScriptMan_TheWScriptMan_AE38 + 0x294), n);
        return -1;
    case 0:
        inst->m_pActiveNode = 0;
        cDebounceNode_cDebounceNode1_AE38(cMemMan_alloc(52, D_00489638_AE38, 0x20000000, 0), 1, inst, buf, n);
        return -1;
    case 22:
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
extern char *cWScriptMan_TheWScriptMan_B060 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_B060(unsigned i) { return (char*)(i << 2); }
static inline char *conv_B060(unsigned i) { if (i == 0) return 0; return refToPtr_B060(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimDeltaNode_cAnimDeltaNode1_B060(void*, int, void*, void*) __asm__("cAnimDeltaNode_cAnimDeltaNode1");
extern char D_00489658_B060[] __asm__("D_00489658");
struct V3_B060 { float x, y, z; };
extern V3_B060 luno_gLunoNil_B060 __asm__("luno_gLunoNil");
struct Tag_B060 { char c; Tag_B060() {} Tag_B060(const Tag_B060 &o) : c(o.c) {} };
extern unsigned D_B060[] __asm__("D_004FB390");
extern int D_004A55C0;
extern const int D_00445E70[];
extern "C" void* MEM_copy_B060(void*, void*, int) __asm__("MEM_copy");
struct Ent_B060 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[4]): animates a world instance with a new AnimDelta node, unless its current node is
 * busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimDeltaData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_00445E70 says 2.
 * Defaults: InstId -1, Type 1, FromTime -1, ToTime -1, Speed 30, StartTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 2 = ATYPE_ANIMDELTA, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cAnimDeltaNode (cAnimDeltaNode_cAnimDeltaNode1, 0x70 bytes, bucket
 * 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_B060 MakeAnimDeltaFunc(int n, Ent_B060 *params) {
    if (D_004A55C0 == 0) {
        D_B060[0] = 0xFFFFFFFF;
        ((int*)D_B060)[1] = 1;
        ((int*)D_B060)[2] = 0;
        *(float*)&D_B060[3] = -1.0f;
        *(float*)&D_B060[4] = -1.0f;
        *(float*)&D_B060[5] = 30.0f;
        ((int*)D_B060)[6] = 0;
        *(float*)&D_B060[7] = -1.0f;
        ((int*)D_B060)[8] = 0;
        ((int*)D_B060)[9] = 0;
        D_004A55C0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_B060*)((char*)D_B060 + 40)) = Tag_B060((*(Tag_B060*)((char*)D_B060 + 40)));
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B060(buf, D_B060, 44);
    Ent_B060 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445E70[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_B060 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_B060(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B060;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 2, buf))
        return luno_gLunoNil_B060;
    cAnimDeltaNode_cAnimDeltaNode1_B060(cMemMan_alloc(0x70, D_00489658_B060, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B060;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimComboFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_B270 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_B270(unsigned i) { return (char*)(i << 2); }
static inline char *conv_B270(unsigned i) { if (i == 0) return 0; return refToPtr_B270(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimComboNode_cAnimComboNode1_B270(void*, int, void*, void*) __asm__("cAnimComboNode_cAnimComboNode1");
extern char D_00489668_B270[] __asm__("D_00489668");
struct V3_B270 { float x, y, z; };
extern V3_B270 luno_gLunoNil_B270 __asm__("luno_gLunoNil");
struct Tag_B270 { char c; Tag_B270() {} Tag_B270(const Tag_B270 &o) : c(o.c) {} };
extern unsigned D_B270[] __asm__("D_004FB3C0");
extern int D_004A55C4;
extern const int D_00445E98[];
extern "C" void* MEM_copy_B270(void*, void*, int) __asm__("MEM_copy");
struct Ent_B270 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[5]): plays a two-stage animation on a world instance with a new AnimCombo node,
 * unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 60-byte parameter block (On Tour tMakeAnimComboData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible, FromTime2, ToTime2,
 * Speed2, Stop) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_00445E98 says 2. Defaults: InstId -1, Type 1, FromTime -1, ToTime 30, Speed 30, StartTime -1, FromTime2
 * 31, ToTime2 -1, Speed2 30.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 3 = ATYPE_ANIMCOMBO, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cAnimComboNode (cAnimComboNode_cAnimComboNode1, 136 bytes, bucket
 * 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_B270 MakeAnimComboFunc(int n, Ent_B270 *params) {
    if (D_004A55C4 == 0) {
        D_B270[0] = 0xFFFFFFFF;
        ((int*)D_B270)[1] = 1;
        ((int*)D_B270)[2] = 0;
        *(float*)&D_B270[3] = -1.0f;
        *(float*)&D_B270[4] = 30.0f;
        *(float*)&D_B270[5] = 30.0f;
        ((int*)D_B270)[6] = 0;
        *(float*)&D_B270[7] = -1.0f;
        ((int*)D_B270)[8] = 0;
        ((int*)D_B270)[9] = 0;
        *(float*)&D_B270[10] = 31.0f;
        *(float*)&D_B270[11] = -1.0f;
        *(float*)&D_B270[12] = 30.0f;
        ((int*)D_B270)[13] = 0;
        D_004A55C4 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_B270*)((char*)D_B270 + 56)) = Tag_B270((*(Tag_B270*)((char*)D_B270 + 56)));
    }
    unsigned buf64[15];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B270(buf, D_B270, 60);
    Ent_B270 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445E98[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_B270 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_B270(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B270;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 3, buf))
        return luno_gLunoNil_B270;
    cAnimComboNode_cAnimComboNode1_B270(cMemMan_alloc(136, D_00489668_B270, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B270;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimTeeterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_B498 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_B498(unsigned i) { return (char*)(i << 2); }
static inline char *conv_B498(unsigned i) { if (i == 0) return 0; return refToPtr_B498(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimTeeterNode_cAnimTeeterNode1_B498(void*, int, void*, void*) __asm__("cAnimTeeterNode_cAnimTeeterNode1");
extern char D_00489678_B498[] __asm__("D_00489678");
struct V3_B498 { float x, y, z; };
extern V3_B498 luno_gLunoNil_B498 __asm__("luno_gLunoNil");
struct Tag_B498 { char c; Tag_B498() {} Tag_B498(const Tag_B498 &o) : c(o.c) {} };
extern unsigned D_B498[] __asm__("D_004FB400");
extern int D_004A55C8;
extern const int D_00445ED0[];
extern "C" void* MEM_copy_B498(void*, void*, int) __asm__("MEM_copy");
struct Ent_B498 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[6]): makes a world instance teeter (a new AnimTeeter node), unless its current node
 * is busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimTeeterData:
 * InstId, TorqueScale, BounceFactor, MaxSpeed, Spring, PivotTime, Damp, AnimSign, FromTime, ToTime) and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_00445ED0 says
 * 2. Defaults: InstId -1, TorqueScale 0.6, MaxSpeed 60, Spring 0.02, PivotTime -10, Damp 0.6, FromTime -1, ToTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 4 = ATYPE_ANIMTEETER, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cAnimTeeterNode (cAnimTeeterNode_cAnimTeeterNode1, 0x80 bytes,
 * bucket 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_B498 MakeAnimTeeterFunc(int n, Ent_B498 *params) {
    if (D_004A55C8 == 0) {
        D_B498[0] = 0xFFFFFFFF;
        *(float*)&D_B498[1] = 0.6000000238418579f;
        ((int*)D_B498)[2] = 0;
        *(float*)&D_B498[3] = 60.0f;
        *(float*)&D_B498[4] = 0.019999999552965164f;
        *(float*)&D_B498[5] = -10.0f;
        *(float*)&D_B498[6] = 0.6000000238418579f;
        ((int*)D_B498)[7] = 0;
        *(float*)&D_B498[8] = -1.0f;
        *(float*)&D_B498[9] = -1.0f;
        D_004A55C8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_B498*)((char*)D_B498 + 40)) = Tag_B498((*(Tag_B498*)((char*)D_B498 + 40)));
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B498(buf, D_B498, 44);
    Ent_B498 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445ED0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_B498 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_B498(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B498;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 4, buf))
        return luno_gLunoNil_B498;
    cAnimTeeterNode_cAnimTeeterNode1_B498(cMemMan_alloc(0x80, D_00489678_B498, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B498;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeConveyorFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_B6B8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_B6B8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_B6B8(unsigned i) { if (i == 0) return 0; return refToPtr_B6B8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cConveyorNode_cConveyorNode_B6B8(void*, int, void*, void*) __asm__("cConveyorNode_cConveyorNode");
extern char D_00489688_B6B8[] __asm__("D_00489688");
struct V3_B6B8 { float x, y, z; };
extern V3_B6B8 luno_gLunoNil_B6B8 __asm__("luno_gLunoNil");
struct Tag_B6B8 { char c; Tag_B6B8() {} Tag_B6B8(const Tag_B6B8 &o) : c(o.c) {} };
extern unsigned D_B6B8[] __asm__("D_004FB430");
extern int D_004A55CC;
extern const int D_00445EF8[];
extern "C" void* MEM_copy_B6B8(void*, void*, int) __asm__("MEM_copy");
struct Ent_B6B8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[8]): turns a world instance into a conveyor (a new Conveyor node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 52-byte parameter block (On Tour tMakeConveyorData:
 * InstId, Vel, UScroll, VScroll, Rotate0, Rotate1, Rotate2, Rotate3, Rotate4, Rotate5, Rotate6, Rotate7) and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_00445EF8 says
 * 2. Defaults: InstId -1, Vel 100.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 15 = ATYPE_CONVEYOR, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cConveyorNode (cConveyorNode_cConveyorNode, 72 bytes, bucket 1) on
 * the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_B6B8 MakeConveyorFunc(int n, Ent_B6B8 *params) {
    if (D_004A55CC == 0) {
        D_B6B8[0] = 0xFFFFFFFF;
        *(float*)&D_B6B8[1] = 100.0f;
        float z = 0.0f;
        *(float*)&D_B6B8[2] = z;
        *(float*)&D_B6B8[3] = z;
        ((int*)D_B6B8)[4] = 0;
        ((int*)D_B6B8)[5] = 0;
        ((int*)D_B6B8)[6] = 0;
        ((int*)D_B6B8)[7] = 0;
        ((int*)D_B6B8)[8] = 0;
        ((int*)D_B6B8)[9] = 0;
        ((int*)D_B6B8)[10] = 0;
        ((int*)D_B6B8)[11] = 0;
        D_004A55CC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_B6B8*)((char*)D_B6B8 + 48)) = Tag_B6B8((*(Tag_B6B8*)((char*)D_B6B8 + 48)));
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B6B8(buf, D_B6B8, 52);
    Ent_B6B8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445EF8[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_B6B8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_B6B8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B6B8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 15, buf))
        return luno_gLunoNil_B6B8;
    cConveyorNode_cConveyorNode_B6B8(cMemMan_alloc(72, D_00489688_B6B8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B6B8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeCounterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_B8D0 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_B8D0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_B8D0(unsigned i) { if (i == 0) return 0; return refToPtr_B8D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cCounterNode_cCounterNode1_B8D0(void*, int, void*, void*) __asm__("cCounterNode_cCounterNode1");
extern char D_004A3BE8_B8D0[] __asm__("D_004A3BE8");
struct V3_B8D0 { float x, y, z; };
extern V3_B8D0 luno_gLunoNil_B8D0 __asm__("luno_gLunoNil");
struct Tag_B8D0 { char c; Tag_B8D0() {} Tag_B8D0(const Tag_B8D0 &o) : c(o.c) {} };
extern unsigned D_B8D0[] __asm__("D_004FB468");
extern int D_004A55D0;
extern int D_004A3BE0_B8D0[2] __asm__("D_004A3BE0");
extern "C" void* MEM_copy_B8D0(void*, void*, int) __asm__("MEM_copy");
struct Ent_B8D0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[9]): gives a world instance a hit counter (a new Counter node), unless its current
 * node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMakeCounterData:
 * InstId, StartCount) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3BE0 says 2. Defaults: InstId -1, StartCount 3.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 7 = ATYPE_COUNTER, block), which keeps a busy or same-type node and replaces
 * some kinds itself; otherwise it builds a cCounterNode (cCounterNode_cCounterNode1, 36 bytes, bucket 1) on the
 * instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_B8D0 MakeCounterFunc(int n, Ent_B8D0 *params) {
    if (D_004A55D0 == 0) {
        D_B8D0[0] = 0xFFFFFFFF;
        ((int*)D_B8D0)[1] = 3;
        D_004A55D0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_B8D0*)((char*)D_B8D0 + 8)) = Tag_B8D0((*(Tag_B8D0*)((char*)D_B8D0 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_B8D0(buf, D_B8D0, 12);
    Ent_B8D0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3BE0_B8D0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_B8D0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_B8D0(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_B8D0;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 7, buf))
        return luno_gLunoNil_B8D0;
    cCounterNode_cCounterNode1_B8D0(cMemMan_alloc(36, D_004A3BE8_B8D0, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_B8D0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFloatingFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_BAB0 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_BAB0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_BAB0(unsigned i) { if (i == 0) return 0; return refToPtr_BAB0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFloatingNode_cFloatingNode_BAB0(void*, int, void*, void*) __asm__("cFloatingNode_cFloatingNode");
extern char D_00489698_BAB0[] __asm__("D_00489698");
struct V3_BAB0 { float x, y, z; };
extern V3_BAB0 luno_gLunoNil_BAB0 __asm__("luno_gLunoNil");
struct Tag_BAB0 { char c; Tag_BAB0() {} Tag_BAB0(const Tag_BAB0 &o) : c(o.c) {} };
extern unsigned D_BAB0[] __asm__("D_004FB478");
extern int D_004A55D4;
extern const int D_00445F28[];
extern "C" void* MEM_copy_BAB0(void*, void*, int) __asm__("MEM_copy");
struct Ent_BAB0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[14]): makes a world instance float on springs (a new Floating node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 32-byte parameter block (On Tour tMakeFloatingData:
 * InstId, LSpring, LDamp, ASpring, ADamp, ForceScale, TorqueScale) and lets each argument override the field its index
 * names, turning an int into a float where the field type table D_00445F28 says 2. Defaults: InstId -1, LSpring 10,
 * LDamp 10, ASpring 40, ADamp 10, ForceScale 0.2, TorqueScale 10.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 14 = ATYPE_FLOATING, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cFloatingNode (cFloatingNode_cFloatingNode, 0x7C bytes, bucket 1)
 * on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_BAB0 MakeFloatingFunc(int n, Ent_BAB0 *params) {
    if (D_004A55D4 == 0) {
        D_BAB0[0] = 0xFFFFFFFF;
        *(float*)&D_BAB0[1] = 10.0f;
        *(float*)&D_BAB0[2] = 10.0f;
        *(float*)&D_BAB0[3] = 40.0f;
        *(float*)&D_BAB0[4] = 10.0f;
        *(float*)&D_BAB0[5] = 0.20000000298023224f;
        *(float*)&D_BAB0[6] = 10.0f;
        D_004A55D4 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_BAB0*)((char*)D_BAB0 + 28)) = Tag_BAB0((*(Tag_BAB0*)((char*)D_BAB0 + 28)));
    }
    unsigned buf64[8];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BAB0(buf, D_BAB0, 32);
    Ent_BAB0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445F28[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_BAB0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_BAB0(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BAB0;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 14, buf))
        return luno_gLunoNil_BAB0;
    cFloatingNode_cFloatingNode_BAB0(cMemMan_alloc(0x7C, D_00489698_BAB0, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_BAB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeAnimObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_BCB8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_BCB8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_BCB8(unsigned i) { if (i == 0) return 0; return refToPtr_BCB8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cAnimObjectNode_cAnimObjectNode1_BCB8(void*, int, int, void*, void*) __asm__("cAnimObjectNode_cAnimObjectNode1");
extern char D_004896A8_BCB8[] __asm__("D_004896A8");
struct V3_BCB8 { float x, y, z; };
extern V3_BCB8 luno_gLunoNil_BCB8 __asm__("luno_gLunoNil");
struct Tag_BCB8 { char c; Tag_BCB8() {} Tag_BCB8(const Tag_BCB8 &o) : c(o.c) {} };
extern unsigned D_BCB8[] __asm__("D_004FB498");
extern int D_004A55D8;
extern const int D_00445F48[];
extern "C" void* MEM_copy_BCB8(void*, void*, int) __asm__("MEM_copy");
struct Ent_BCB8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[3]): animates a world instance as an animated object (a new AnimObject node), unless
 * its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 44-byte parameter block (On Tour tMakeAnimObjectData:
 * InstId, Type, Dir, FromTime, ToTime, Speed, SpeedRange, StartTime, RandomStart, ForceVisible) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_00445F48 says 2.
 * Defaults: InstId -1, Type 1, FromTime -1, ToTime -1, Speed 30, StartTime -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 1 = ATYPE_ANIMOBJECT, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cAnimObjectNode (cAnimObjectNode_cAnimObjectNode1, 108 bytes,
 * bucket 1, node type 1) on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_BCB8 MakeAnimObjectFunc(int n, Ent_BCB8 *params) {
    if (D_004A55D8 == 0) {
        D_BCB8[0] = 0xFFFFFFFF;
        ((int*)D_BCB8)[1] = 1;
        ((int*)D_BCB8)[2] = 0;
        *(float*)&D_BCB8[3] = -1.0f;
        *(float*)&D_BCB8[4] = -1.0f;
        *(float*)&D_BCB8[5] = 30.0f;
        ((int*)D_BCB8)[6] = 0;
        *(float*)&D_BCB8[7] = -1.0f;
        ((int*)D_BCB8)[8] = 0;
        ((int*)D_BCB8)[9] = 0;
        D_004A55D8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_BCB8*)((char*)D_BCB8 + 40)) = Tag_BCB8((*(Tag_BCB8*)((char*)D_BCB8 + 40)));
    }
    unsigned buf64[11];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BCB8(buf, D_BCB8, 44);
    Ent_BCB8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445F48[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_BCB8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_BCB8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BCB8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 1, buf))
        return luno_gLunoNil_BCB8;
    cAnimObjectNode_cAnimObjectNode1_BCB8(cMemMan_alloc(108, D_004896A8_BCB8, 0x20000000, 0), 1, 1, obj, buf);
    return luno_gLunoNil_BCB8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeBoostFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_BEC8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_BEC8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_BEC8(unsigned i) { if (i == 0) return 0; return refToPtr_BEC8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cBoostNode_cBoostNode1_BEC8(void*, int, int, void*, void*) __asm__("cBoostNode_cBoostNode1");
extern char D_004A3BF0_BEC8[] __asm__("D_004A3BF0");
struct V3_BEC8 { float x, y, z; };
extern V3_BEC8 luno_gLunoNil_BEC8 __asm__("luno_gLunoNil");
struct Tag_BEC8 { char c; Tag_BEC8() {} Tag_BEC8(const Tag_BEC8 &o) : c(o.c) {} };
extern unsigned D_BEC8[] __asm__("D_004FB4C8");
extern int D_004A55DC;
extern const int D_00445F70[];
extern "C" void* MEM_copy_BEC8(void*, void*, int) __asm__("MEM_copy");
struct Ent_BEC8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[7]): turns a world instance into a boost object (a new Boost node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 36-byte parameter block (On Tour tMakeBoostData: InstId,
 * Type, BoostTime, Drag, MaxSpeed, BoostX, BoostY, BoostZ) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_00445F70 says 2. Defaults: InstId -1, BoostTime 1, MaxSpeed
 * 1, BoostY 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 8 = ATYPE_BOOST, block), which keeps a busy or same-type node and replaces
 * some kinds itself; otherwise it builds a cBoostNode (cBoostNode_cBoostNode1, 80 bytes, bucket 1, node type 8) on the
 * instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_BEC8 MakeBoostFunc(int n, Ent_BEC8 *params) {
    if (D_004A55DC == 0) {
        D_BEC8[0] = 0xFFFFFFFF;
        ((int*)D_BEC8)[1] = 0;
        *(float*)&D_BEC8[2] = 1.0f;
        float z = 0.0f;
        *(float*)&D_BEC8[3] = z;
        *(float*)&D_BEC8[4] = 1.0f;
        *(float*)&D_BEC8[5] = z;
        *(float*)&D_BEC8[6] = 1.0f;
        *(float*)&D_BEC8[7] = z;
        D_004A55DC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_BEC8*)((char*)D_BEC8 + 32)) = Tag_BEC8((*(Tag_BEC8*)((char*)D_BEC8 + 32)));
    }
    unsigned buf64[9];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_BEC8(buf, D_BEC8, 36);
    Ent_BEC8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445F70[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_BEC8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_BEC8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_BEC8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 8, buf))
        return luno_gLunoNil_BEC8;
    cBoostNode_cBoostNode1_BEC8(cMemMan_alloc(80, D_004A3BF0_BEC8, 0x20000000, 0), 1, 8, obj, buf);
    return luno_gLunoNil_BEC8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_C0D0 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_C0D0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C0D0(unsigned i) { if (i == 0) return 0; return refToPtr_C0D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cObjectNode_cObjectNode_C0D0(void*, int, void*, int, bool) __asm__("cObjectNode_cObjectNode");
extern char D_004A3BF8_C0D0[] __asm__("D_004A3BF8");
struct V3_C0D0 { float x, y, z; };
extern V3_C0D0 luno_gLunoNil_C0D0 __asm__("luno_gLunoNil");
struct Tag_C0D0 { char c; Tag_C0D0() {} Tag_C0D0(const Tag_C0D0 &o) : c(o.c) {} };
extern unsigned D_C0D0[] __asm__("D_004FB4F0");
extern int D_004A55E0;
extern const int D_00445F90[];
extern "C" void* MEM_copy_C0D0(void*, void*, int) __asm__("MEM_copy");
struct Ent_C0D0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[0]): gives a world instance a plain object node (a new Object node), unless its
 * current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMakeObjectData:
 * InstId, Solid, ForceVisible) and lets each argument override the field its index names, turning an int into a float
 * where the field type table D_00445F90 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 17 = ATYPE_OBJECT, block), which keeps a busy or same-type node and replaces
 * some kinds itself; otherwise it builds a cObjectNode (cObjectNode_cObjectNode, 44 bytes, bucket 1) with Solid and
 * ForceVisible != 0 as its arguments on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_C0D0 MakeObjectFunc(int n, Ent_C0D0 *params) {
    if (D_004A55E0 == 0) {
        D_C0D0[0] = 0xFFFFFFFF;
        ((int*)D_C0D0)[1] = 0;
        ((int*)D_C0D0)[2] = 0;
        D_004A55E0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_C0D0*)((char*)D_C0D0 + 12)) = Tag_C0D0((*(Tag_C0D0*)((char*)D_C0D0 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C0D0(buf, D_C0D0, 16);
    Ent_C0D0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445F90[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_C0D0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_C0D0(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C0D0;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 17, buf))
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

typedef void (*DtorFn)(void*, int);

// virtual-destructor dispatch (delete obj): vtable entry {short delta @+8, fn @+0xC}
static inline void del_obj(char* o) {
    char* vt = *(char**)(o + 0xC);
    ((DtorFn) * (void**)(vt + 0xC))(o + *(short*)(vt + 8), 3);
}

/**
 * Applies a dead/restore request to a world instance by the request's Type.
 *
 * @p ev is a tMakeDeadData-style block (On Tour: InstId, Type). Type 0 replaces the instance's node (+0xC) with a new
 * cDeadNode (cMemMan_alloc 0x1C, "DeadNode"); type 3 replaces it with a new cRestoreNode (0x1C, "RestoreNode"); type 1
 * deletes the node and rewrites the instance flags (+0x8) as (f & 0xFFFF0300) | ((f & 0xFFFF0300) >> 16) | 2. An
 * existing node is deleted through its virtual destructor first, except that a dead node (node type 6 at +0x10) is
 * never touched (and a restore node, type 19, is kept for type 3), which ends the request. Other types do nothing.
 *
 * MakeDeadFunc calls it for the script, cObjectInterface_makeDead for cDeadFadeNode when its fade ends.
 *
 * @param ev  the request block: InstId at +0x0, Type at +0x4
 */
extern "C" void makeDead(tInstanceRecord* self, char* ev) {
    switch (*(int*)(ev + 4)) {
    case 1: {
        char* curNode = (char*)self->m_pActiveNode;
        if (curNode) {
            if (*(short*)(curNode + 0x10) == 6)
                break;
            del_obj(curNode);
        }
        int maskedFlags = self->m_iFlags & 0xFFFF0300;
        self->m_iFlags = maskedFlags | (maskedFlags >> 16) | 2;
        break;
    }
    case 0: {
        char* curNode = (char*)self->m_pActiveNode;
        if (curNode) {
            if (*(short*)(curNode + 0x10) == 6)
                break;
            del_obj(curNode);
        }
        cDeadNode_cDeadNode(cMemMan_alloc(0x1C, str_DeadNode, 0x20000000, 0), self);
        break;
    }
    case 3: {
        char* curNode = (char*)self->m_pActiveNode;
        if (curNode) {
            short ty = *(short*)(curNode + 0x10);
            if (ty == 6 || ty == 19)
                break;
            del_obj(curNode);
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
extern char *cWScriptMan_TheWScriptMan_C420 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_C420 __asm__("SSXApp");
static inline char *refToPtr_C420(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C420(unsigned i) { if (i == 0) return 0; return refToPtr_C420(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void makeDead_C420(void*, void*) __asm__("makeDead");
struct V3_C420 { float x, y, z; };
extern V3_C420 luno_gLunoNil_C420 __asm__("luno_gLunoNil");
struct Tag_C420 { char c; Tag_C420() {} Tag_C420(const Tag_C420 &o) : c(o.c) {} };
extern unsigned D_C420[] __asm__("D_004FB500");
extern int D_004A55E4;
extern int D_004A3C00;
extern "C" void* MEM_copy_C420(void*, void*, int) __asm__("MEM_copy");
struct Ent_C420 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[2]): makes a world instance dead or restores it, unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMakeDeadData: InstId,
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C00 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found. When it has no
 * active node (+0xC), or checkActiveNode(node, 6 = ATYPE_DEAD, block) lets the node go, the instance and block go to
 * makeDead, which acts by Type (0 dead node, 1 delete the node and flag the instance, 3 restore node).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_C420 MakeDeadFunc(int n, Ent_C420 *params) {
    if (D_004A55E4 == 0) {
        D_C420[0] = 0xFFFFFFFF;
        ((int*)D_C420)[1] = 0;
        D_004A55E4 = 1;
        (*(Tag_C420*)((char*)D_C420 + 8)) = Tag_C420((*(Tag_C420*)((char*)D_C420 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C420(buf, D_C420, 12);
    Ent_C420 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C00)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_C420 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_C420(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C420;
    void *activeNode = *(void**)(obj + 0xC);
    if (!activeNode || !checkActiveNode(activeNode, 6, buf))
        makeDead_C420(obj, buf);
    return luno_gLunoNil_C420;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeDeadFadeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_C5D8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_C5D8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C5D8(unsigned i) { if (i == 0) return 0; return refToPtr_C5D8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cDeadFadeNode_cDeadFadeNode_C5D8(void*, int, void*, void*, int) __asm__("cDeadFadeNode_cDeadFadeNode");
extern char D_004896D8_C5D8[] __asm__("D_004896D8");
struct V3_C5D8 { float x, y, z; };
extern V3_C5D8 luno_gLunoNil_C5D8 __asm__("luno_gLunoNil");
struct Tag_C5D8 { char c; Tag_C5D8() {} Tag_C5D8(const Tag_C5D8 &o) : c(o.c) {} };
extern unsigned D_C5D8[] __asm__("D_004FB510");
extern int D_004A55E8;
extern const int D_00445FA0[];
extern "C" void* MEM_copy_C5D8(void*, void*, int) __asm__("MEM_copy");
struct Ent_C5D8 { int idx; int val; int pad; int type; };
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
extern "C" V3_C5D8 MakeDeadFadeFunc(int n, Ent_C5D8 *params) {
    if (D_004A55E8 == 0) {
        D_C5D8[0] = 0xFFFFFFFF;
        ((int*)D_C5D8)[1] = 0;
        *(float*)&D_C5D8[2] = 2.0f;
        *(float*)&D_C5D8[3] = 25.0f;
        D_004A55E8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_C5D8*)((char*)D_C5D8 + 16)) = Tag_C5D8((*(Tag_C5D8*)((char*)D_C5D8 + 16)));
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C5D8(buf, D_C5D8, 20);
    Ent_C5D8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445FA0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_C5D8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_C5D8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C5D8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 22, buf))
        return luno_gLunoNil_C5D8;
    cDeadFadeNode_cDeadFadeNode_C5D8(cMemMan_alloc(0x3C, D_004896D8_C5D8, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_C5D8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeDebounceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_C7D0 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_C7D0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C7D0(unsigned i) { if (i == 0) return 0; return refToPtr_C7D0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cDebounceNode_cDebounceNode1_C7D0(void*, int, void*, void*, int) __asm__("cDebounceNode_cDebounceNode1");
extern char D_004896E8_C7D0[] __asm__("D_004896E8");
struct V3_C7D0 { float x, y, z; };
extern V3_C7D0 luno_gLunoNil_C7D0 __asm__("luno_gLunoNil");
struct Tag_C7D0 { char c; Tag_C7D0() {} Tag_C7D0(const Tag_C7D0 &o) : c(o.c) {} };
extern unsigned D_C7D0[] __asm__("D_004FB528");
extern int D_004A55EC;
extern const int D_00445FB0[];
extern "C" void* MEM_copy_C7D0(void*, void*, int) __asm__("MEM_copy");
struct Ent_C7D0 { int idx; int val; int pad; int type; };
/**
 * Script command MakeDebounce: turns an instance into a debounce node.
 *
 * Copies the static defaults (D_004FB528, set once under D_004A55EC: instance -1, 1.0, 1, -1) and applies each keyword
 * argument by index, converting an int to float where the keyword table D_00445FB0 says float (2). The instance ID -1
 * means cWScriptMan_TheWScriptMan's current instance (+0x290); otherwise it is looked up in cWorld_m_pTheWorld's world
 * cache (section = low byte, entry = the rest). Nothing happens if there is no instance, or if it has an active node
 * (+0xC) and checkActiveNode(node, 0, data) says it is already one; otherwise cDebounceNode_cDebounceNode1 constructs a cDebounceNode
 * (bucket 1) in cMemMan_alloc(0x34, "Debounce").
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" V3_C7D0 MakeDebounceFunc(int n, Ent_C7D0 *params) {
    if (D_004A55EC == 0) {
        D_C7D0[0] = 0xFFFFFFFF;
        *(float*)&D_C7D0[1] = 1.0f;
        ((int*)D_C7D0)[2] = 1;
        ((int*)D_C7D0)[3] = -1;
        D_004A55EC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_C7D0*)((char*)D_C7D0 + 16)) = Tag_C7D0((*(Tag_C7D0*)((char*)D_C7D0 + 16)));
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C7D0(buf, D_C7D0, 20);
    Ent_C7D0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445FB0[idx];
        unsigned *field = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)field = (float)x->val;
        else
            *field = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_C7D0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_C7D0(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C7D0;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 0, buf))
        return luno_gLunoNil_C7D0;
    cDebounceNode_cDebounceNode1_C7D0(cMemMan_alloc(52, D_004896E8_C7D0, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_C7D0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFlagFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_C9C8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_C9C8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_C9C8(unsigned i) { if (i == 0) return 0; return refToPtr_C9C8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFlagNode_cFlagNode1_C9C8(void*, int, void*, void*) __asm__("cFlagNode_cFlagNode1");
extern char D_004A3C08_C9C8[] __asm__("D_004A3C08");
struct V3_C9C8 { float x, y, z; };
extern V3_C9C8 luno_gLunoNil_C9C8 __asm__("luno_gLunoNil");
struct Tag_C9C8 { char c; Tag_C9C8() {} Tag_C9C8(const Tag_C9C8 &o) : c(o.c) {} };
extern unsigned D_C9C8[] __asm__("D_004FB540");
extern int D_004A55F0;
extern const int D_00445FC0[];
extern "C" void* MEM_copy_C9C8(void*, void*, int) __asm__("MEM_copy");
struct Ent_C9C8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[12]): makes a world instance wave like a flag (a new Flag node), unless its current
 * node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 108-byte parameter block (On Tour tMakeFlagData: InstId,
 * BaseSideHeld, OppositeSideHeld, PerpSideHeld, Speed1, Magnitude1, Frequency1, Speed2, Magnitude2, Frequency2, Speed3,
 * Magnitude3, Frequency3, Speed4, Magnitude4, Frequency4, LowWindX, LowWindY, LowWindZ, HighWindX, HighWindY,
 * HighWindZ, MinWindFactor, UScroll, VScroll, OverSaturate) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_00445FC0 says 2. Defaults: InstId -1, BaseSideHeld 1, Speed1
 * 1, Magnitude1 25, Frequency1 1, Speed2 1, Frequency2 1, Speed3 1, Frequency3 1, Speed4 1, Frequency4 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 10 = ATYPE_FLAG, block), which keeps a busy or same-type node and replaces
 * some kinds itself; otherwise it builds a cFlagNode (cFlagNode_cFlagNode1, 0x1C bytes, bucket 1) on the instance from
 * the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_C9C8 MakeFlagFunc(int n, Ent_C9C8 *params) {
    if (D_004A55F0 == 0) {
        D_C9C8[0] = 0xFFFFFFFF;
        ((int*)D_C9C8)[1] = 1;
        ((int*)D_C9C8)[2] = 0;
        ((int*)D_C9C8)[3] = 0;
        *(float*)&D_C9C8[4] = 1.0f;
        *(float*)&D_C9C8[5] = 25.0f;
        *(float*)&D_C9C8[6] = 1.0f;
        *(float*)&D_C9C8[7] = 1.0f;
        float z = 0.0f;
        *(float*)&D_C9C8[8] = z;
        *(float*)&D_C9C8[9] = 1.0f;
        *(float*)&D_C9C8[10] = 1.0f;
        *(float*)&D_C9C8[11] = z;
        *(float*)&D_C9C8[12] = 1.0f;
        *(float*)&D_C9C8[13] = 1.0f;
        *(float*)&D_C9C8[14] = z;
        *(float*)&D_C9C8[15] = 1.0f;
        *(float*)&D_C9C8[16] = z;
        *(float*)&D_C9C8[17] = z;
        *(float*)&D_C9C8[18] = z;
        *(float*)&D_C9C8[19] = z;
        *(float*)&D_C9C8[20] = z;
        *(float*)&D_C9C8[21] = z;
        *(float*)&D_C9C8[22] = z;
        *(float*)&D_C9C8[23] = z;
        *(float*)&D_C9C8[24] = z;
        ((int*)D_C9C8)[25] = 0;
        D_004A55F0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_C9C8*)((char*)D_C9C8 + 104)) = Tag_C9C8((*(Tag_C9C8*)((char*)D_C9C8 + 104)));
    }
    unsigned buf64[27];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_C9C8(buf, D_C9C8, 108);
    Ent_C9C8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00445FC0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_C9C8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_C9C8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_C9C8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 10, buf))
        return luno_gLunoNil_C9C8;
    cFlagNode_cFlagNode1_C9C8(cMemMan_alloc(0x1C, D_004A3C08_C9C8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_C9C8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeFenceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_CC20 __asm__("cWScriptMan_TheWScriptMan");
struct V3_CC20 { float x, y, z; };
extern V3_CC20 luno_gLunoNil_CC20 __asm__("luno_gLunoNil");
struct Tag_CC20 { char c; Tag_CC20() {} Tag_CC20(const Tag_CC20 &o) : c(o.c) {} };
extern unsigned D_CC20[] __asm__("D_004FB5B0");
extern int D_004A55F4;
extern const int D_00446028[];
extern "C" void* MEM_copy_CC20(void*, void*, int) __asm__("MEM_copy");
struct Ent_CC20 { int idx; int val; int pad; int type; };
extern "C" int checkActiveNode(void*, int, void*);
#define ASSERT_CC20(c) do { } while (0)

/**
 * Script function (gLunoCFunctions[10]): settles a world instance's current node as a fence request; builds no node.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMakeFenceData: InstId,
 * Type, Flexibility) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_00446028 says 2. Defaults: InstId -1, Flexibility 0.5.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance exists and has an active node (+0xC), it only
 * calls checkActiveNode(node, 9 = ATYPE_FENCE, block) and ignores the result: a busy or fence node is kept (a fence is
 * retriggered) and any other node is deleted. Unlike its siblings it never builds a fence node. The nil value is
 * written through the hidden result pointer @p ret.
 *
 * @param ret  where the returned script value is stored
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
// PORT: struct return written with the hidden return pointer explicit.
extern "C" V3_CC20* MakeFenceFunc(V3_CC20* ret, int n, Ent_CC20 *params) {
    if (D_004A55F4 == 0) {
        D_CC20[0] = 0xFFFFFFFF;
        *(float*)&D_CC20[2] = 0.5f;
        ((int*)D_CC20)[1] = 0;
        D_004A55F4 = 1;
        (*(Tag_CC20*)((char*)D_CC20 + 12)) = Tag_CC20((*(Tag_CC20*)((char*)D_CC20 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_CC20(buf, D_CC20, 16);
    Ent_CC20 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446028[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_CC20 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        void* node = *(void**)(obj + 0xC);
        if (node)
            checkActiveNode(node, 9, buf);
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
extern char *cWScriptMan_TheWScriptMan_CDC8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_CDC8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_CDC8(unsigned i) { if (i == 0) return 0; return refToPtr_CDC8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFlexBridgeNode_cFlexBridgeNode_CDC8(void*, int, void*, void*) __asm__("cFlexBridgeNode_cFlexBridgeNode");
extern char D_004896F8_CDC8[] __asm__("D_004896F8");
struct V3_CDC8 { float x, y, z; };
extern V3_CDC8 luno_gLunoNil_CDC8 __asm__("luno_gLunoNil");
struct Tag_CDC8 { char c; Tag_CDC8() {} Tag_CDC8(const Tag_CDC8 &o) : c(o.c) {} };
extern unsigned D_CDC8[] __asm__("D_004FB5C0");
extern int D_004A55F8;
extern const int D_00446038[];
extern "C" void* MEM_copy_CDC8(void*, void*, int) __asm__("MEM_copy");
struct Ent_CDC8 { int idx; int val; int pad; int type; };
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
extern "C" V3_CDC8 MakeFlexBridgeFunc(int n, Ent_CDC8 *params) {
    if (D_004A55F8 == 0) {
        *(float*)&D_CDC8[0] = 500.0f;
        *(float*)&D_CDC8[1] = 1.0f;
        *(float*)&D_CDC8[2] = 2400.0f;
        *(float*)&D_CDC8[3] = 0.10000000149011612f;
        *(float*)&D_CDC8[4] = 0.10000000149011612f;
        *(float*)&D_CDC8[5] = 10.0f;
        ((int*)D_CDC8)[6] = 0;
        ((int*)D_CDC8)[7] = 16;
        D_CDC8[8] = 0xFFFFFFFF;
        D_004A55F8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_CDC8*)((char*)D_CDC8 + 36)) = Tag_CDC8((*(Tag_CDC8*)((char*)D_CDC8 + 36)));
    }
    unsigned buf64[10];
    unsigned *buf = buf64;
    buf64[8] = 0xFFFFFFFF;
    MEM_copy_CDC8(buf, D_CDC8, 40);
    Ent_CDC8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446038[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[8];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_CDC8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_CDC8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_CDC8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 5, buf))
        return luno_gLunoNil_CDC8;
    cFlexBridgeNode_cFlexBridgeNode_CDC8(cMemMan_alloc(176, D_004896F8_CDC8, 0x20000000, 0), 1, obj, buf);
    return luno_gLunoNil_CDC8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeMeshAnimFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_CFF0 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_CFF0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_CFF0(unsigned i) { if (i == 0) return 0; return refToPtr_CFF0(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cMeshAnimNode_cMeshAnimNode1_CFF0(void*, int, void*, void*, int, int) __asm__("cMeshAnimNode_cMeshAnimNode1");
extern char D_00489708_CFF0[] __asm__("D_00489708");
struct V3_CFF0 { float x, y, z; };
extern V3_CFF0 luno_gLunoNil_CFF0 __asm__("luno_gLunoNil");
struct Tag_CFF0 { char c; Tag_CFF0() {} Tag_CFF0(const Tag_CFF0 &o) : c(o.c) {} };
extern unsigned D_CFF0[] __asm__("D_004FB5E8");
extern int D_004A55FC;
extern const int D_00446060[];
extern "C" void* MEM_copy_CFF0(void*, void*, int) __asm__("MEM_copy");
struct Ent_CFF0 { int idx; int val; int pad; int type; };
/**
 * Script command MakeMeshAnim: turns an instance into an animated-mesh node.
 *
 * Copies the static 0x40-byte defaults (D_004FB5E8, set once under D_004A55FC: instance -1, 0, 0.1, 1.0, 2.0, 600, 600,
 * 300, 0, 0, 0, 360, -1, -1, -1) and applies the keyword arguments (types from D_00446060; an int becomes a float where
 * the type is 2). The instance is resolved as in MakeDebounceFunc (-1 = the script's current instance). Unless it is
 * missing or checkActiveNode(node, 11, data) finds it already animated, cMeshAnimNode_cMeshAnimNode1 constructs a cMeshAnimNode
 * (bucket 1, with cWScriptMan_TheWScriptMan +0x294) in cMemMan_alloc(0x6C0, "MeshAnim").
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" V3_CFF0 MakeMeshAnimFunc(int n, Ent_CFF0 *params) {
    if (D_004A55FC == 0) {
        D_CFF0[0] = 0xFFFFFFFF;
        ((int*)D_CFF0)[1] = 0;
        *(float*)&D_CFF0[2] = 0.10000000149011612f;
        *(float*)&D_CFF0[3] = 1.0f;
        *(float*)&D_CFF0[4] = 2.0f;
        *(float*)&D_CFF0[5] = 600.0f;
        *(float*)&D_CFF0[6] = 600.0f;
        *(float*)&D_CFF0[7] = 300.0f;
        float z = 0.0f;
        *(float*)&D_CFF0[8] = z;
        *(float*)&D_CFF0[9] = z;
        *(float*)&D_CFF0[10] = z;
        *(float*)&D_CFF0[11] = 360.0f;
        *(float*)&D_CFF0[12] = -1.0f;
        D_CFF0[13] = 0xFFFFFFFF;
        ((int*)D_CFF0)[14] = -1;
        D_004A55FC = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_CFF0*)((char*)D_CFF0 + 60)) = Tag_CFF0((*(Tag_CFF0*)((char*)D_CFF0 + 60)));
    }
    unsigned buf64[16];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[13] = 0xFFFFFFFF;
    MEM_copy_CFF0(buf, D_CFF0, 64);
    Ent_CFF0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446060[idx];
        unsigned *field = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)field = (float)x->val;
        else
            *field = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_CFF0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_CFF0(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_CFF0;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 11, buf))
        return luno_gLunoNil_CFF0;
    cMeshAnimNode_cMeshAnimNode1_CFF0(cMemMan_alloc(1728, D_00489708_CFF0, 0x20000000, 0), 1, obj, buf, *(int*)(cWScriptMan_TheWScriptMan_CFF0 + 0x294), 0);
    return luno_gLunoNil_CFF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddRollerFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_D250 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_D250 __asm__("SSXApp");
extern "C" void cMoveNode_addRoller_D250(void*, void*, int, int) __asm__("cMoveNode_addRoller");
struct O_D250 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_D250 { float x, y, z; };
extern V3_D250 luno_gLunoNil_D250 __asm__("luno_gLunoNil");
struct Tag_D250 { char c; Tag_D250() {} Tag_D250(const Tag_D250 &o) : c(o.c) {} };
extern unsigned D_D250[] __asm__("D_004FB628");
extern int D_004A5600;
extern const int D_004460A0[];
extern "C" void* MEM_copy_D250(void*, void*, int) __asm__("MEM_copy");
struct Ent_D250 { int idx; int val; int pad; int type; };
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
extern "C" V3_D250 AddRollerFunc(int n, Ent_D250 *params) {
    if (D_004A5600 == 0) {
        D_D250[0] = 0xFFFFFFFF;
        *(float*)&D_D250[1] = 1.0f;
        *(float*)&D_D250[2] = 0.699999988079071f;
        *(float*)&D_D250[3] = 0.5f;
        D_004A5600 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_D250*)((char*)D_D250 + 16)) = Tag_D250((*(Tag_D250*)((char*)D_D250 + 16)));
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_D250(buf, D_D250, 20);
    Ent_D250 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004460A0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_D250 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_D250 *activeNode = *(O_D250**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16())
            cMoveNode_addRoller_D250(activeNode, buf, *(int*)(cWScriptMan_TheWScriptMan_D250 + 0x298), *(int*)(cWScriptMan_TheWScriptMan_D250 + 0x294));
    }
    return luno_gLunoNil_D250;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_D420 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_D420(unsigned i) { return (char*)(i << 2); }
static inline char *conv_D420(unsigned i) { if (i == 0) return 0; return refToPtr_D420(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cParticleNode_cParticleNode1_D420(void*, int, void*, void*, int) __asm__("cParticleNode_cParticleNode1");
void operator_delete(int*);
extern char D_00489718_D420[] __asm__("D_00489718");
extern char D_00489730_D420[] __asm__("D_00489730");
struct V3_D420 { float x, y, z; };
extern V3_D420 luno_gLunoNil_D420 __asm__("luno_gLunoNil");
struct Tag_D420 { char c; Tag_D420() {} Tag_D420(const Tag_D420 &o) : c(o.c) {} };
extern unsigned D_D420[] __asm__("D_004FB640");
extern int D_004A5604;
extern const int D_004460B0[];
extern "C" void* MEM_copy_D420(void*, void*, int) __asm__("MEM_copy");
struct Ent_D420 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[16]): attaches a particle effect to a world instance (a new Particle node), unless
 * its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into a 224-byte parameter block (On Tour tMakeParticleData:
 * NumParticles, NumBlur, Duration, Damp, Size, Life, SizeR, LifeR, BlurStep, OffX, OffY, OffZ, R0X, R0Y, R0Z, R1X, R1Y,
 * R1Z, VelX, VelY, VelZ, R0VX, R0VY, R0VZ, R1VX, R1VY, R1VZ, R2VX, R2VY, R2VZ, ForceX, ForceY, ForceZ, StartColA,
 * StartColR, StartColG, StartColB, EndColA, EndColR, EndColG, EndColB, R0A, R0R, R0G, R0B, R1A, R1R, R1G, R1B,
 * TextureId, BlendMode, SizeFinal, NumFlipTextures, FlipTextureRate, InstId) and lets each argument override the field
 * its index names, turning an int into a float where the field type table D_004460B0 says 2. Defaults: NumParticles 1,
 * Duration -1, Damp 1, Size 4, StartColA 1, EndColA 1, EndColR 1, EndColG 1, EndColB 1, R0A 1, R1A 1, TextureId 16,
 * NumFlipTextures 1, FlipTextureRate 20, InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
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
extern "C" V3_D420 MakeParticleFunc(int n, Ent_D420 *params) {
    if (D_004A5604 == 0) {
        ((int*)D_D420)[0] = 1;
        ((int*)D_D420)[1] = 0;
        *(float*)&D_D420[2] = -1.0f;
        *(float*)&D_D420[3] = 1.0f;
        *(float*)&D_D420[4] = 4.0f;
        float z = 0.0f;
        *(float*)&D_D420[5] = z;
        *(float*)&D_D420[6] = z;
        *(float*)&D_D420[7] = z;
        *(float*)&D_D420[8] = z;
        *(float*)&D_D420[9] = z;
        *(float*)&D_D420[10] = z;
        *(float*)&D_D420[11] = z;
        *(float*)&D_D420[12] = z;
        *(float*)&D_D420[13] = z;
        *(float*)&D_D420[14] = z;
        *(float*)&D_D420[15] = z;
        *(float*)&D_D420[16] = z;
        *(float*)&D_D420[17] = z;
        *(float*)&D_D420[18] = z;
        *(float*)&D_D420[19] = z;
        *(float*)&D_D420[20] = z;
        *(float*)&D_D420[21] = z;
        *(float*)&D_D420[22] = z;
        *(float*)&D_D420[23] = z;
        *(float*)&D_D420[24] = z;
        *(float*)&D_D420[25] = z;
        *(float*)&D_D420[26] = z;
        *(float*)&D_D420[27] = z;
        *(float*)&D_D420[28] = z;
        *(float*)&D_D420[29] = z;
        *(float*)&D_D420[30] = z;
        *(float*)&D_D420[31] = z;
        *(float*)&D_D420[32] = z;
        *(float*)&D_D420[33] = 1.0f;
        *(float*)&D_D420[34] = z;
        *(float*)&D_D420[35] = z;
        *(float*)&D_D420[36] = z;
        *(float*)&D_D420[37] = 1.0f;
        *(float*)&D_D420[38] = 1.0f;
        *(float*)&D_D420[39] = 1.0f;
        *(float*)&D_D420[40] = 1.0f;
        *(float*)&D_D420[41] = 1.0f;
        *(float*)&D_D420[42] = z;
        *(float*)&D_D420[43] = z;
        *(float*)&D_D420[44] = z;
        *(float*)&D_D420[45] = 1.0f;
        *(float*)&D_D420[46] = z;
        *(float*)&D_D420[47] = z;
        *(float*)&D_D420[48] = z;
        ((int*)D_D420)[49] = 16;
        ((int*)D_D420)[50] = 0;
        *(float*)&D_D420[51] = z;
        ((int*)D_D420)[52] = 1;
        *(float*)&D_D420[53] = 20.0f;
        D_D420[54] = 0xFFFFFFFF;
        D_004A5604 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_D420*)((char*)D_D420 + 0xDC)) = Tag_D420((*(Tag_D420*)((char*)D_D420 + 0xDC)));
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xE0, D_00489718_D420, 0x20000000, 0);
    buf[54] = 0xFFFFFFFF;
    MEM_copy_D420(buf, D_D420, 0xE0);
    Ent_D420 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004460B0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[54];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_D420 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!section)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_D420(i);
            r = t;
        }
        obj = r;
    }
    if (obj == 0) {
        operator_delete((int*)buf);
        return luno_gLunoNil_D420;
    }
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode) {
        int r = checkActiveNode(activeNode, 13, buf);
        if (r != 0) {
            if (r > 0)
                operator_delete((int*)buf);
            return luno_gLunoNil_D420;
        }
    }
    if (*(float*)&buf[7] > *(float*)&buf[5])
        *(float*)&buf[7] = *(float*)&buf[5];
    cParticleNode_cParticleNode1_D420(cMemMan_alloc(44, D_00489730_D420, 0x20000000, 0), 1, obj, buf, 0);
    return luno_gLunoNil_D420;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddRiderPositionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_D758 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_D758 __asm__("SSXApp");
struct V4_D758 { float x, y, z, w; V4_D758() {} V4_D758(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} } __attribute__((aligned(16)));
struct M4_D758 { V4_D758 r[4]; };
struct Sh_D758 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual V4_D758 *v05(); virtual M4_D758 *v06(); };
extern "C" void cMoveNode_addPosition_D758(void*, V4_D758*) __asm__("cMoveNode_addPosition");
struct Lvl_D758 { char pad[0x28]; char *sh[1]; };
static inline Sh_D758 *shape_D758(int k) {
    Lvl_D758 *lvl = *(Lvl_D758**)(*(char**)(SSXApp_D758 + 0x84) + 0xC);
    char *q = lvl->sh[k];
    return (Sh_D758*)(q ? q + 0x6C0 : 0);
}
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline V4_D758 mtxApply_D758(M4_D758 *m, const V4_D758 &v) {
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
struct O_D758 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_D758 { float x, y, z; };
extern V3_D758 luno_gLunoNil_D758 __asm__("luno_gLunoNil");
struct Tag_D758 { char c; Tag_D758() {} Tag_D758(const Tag_D758 &o) : c(o.c) {} };
extern unsigned D_D758[] __asm__("D_004FB720");
extern int D_004A5608;
extern const int D_00446190[];
extern "C" void* MEM_copy_D758(void*, void*, int) __asm__("MEM_copy");
struct Ent_D758 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[23]): adds a position modifier that puts a moving world instance at a rider, with an
 * offset.
 *
 * Copies the static defaults (set up on the first call) into a 24-byte parameter block (On Tour tAddRiderPositionData:
 * InstId, Being, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_00446190 says 2. Defaults: InstId -1, Being -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode) and Being is not -1, the target is rider Being's position (SSXApp->Game->AI->Riders[Being], cRider
 * virtual getPosition) plus the offset (OffsetX, OffsetY, OffsetZ) turned by that rider's transform (virtual
 * getTransform); cMoveNode_addPosition gets it.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer. An empty rider slot is
 * not checked either.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_D758 AddRiderPositionFunc(int n, Ent_D758 *params) {
    if (D_004A5608 == 0) {
        D_D758[0] = 0xFFFFFFFF;
        ((int*)D_D758)[1] = -1;
        float z = 0.0f;
        *(float*)&D_D758[2] = z;
        *(float*)&D_D758[3] = z;
        *(float*)&D_D758[4] = z;
        D_004A5608 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_D758*)((char*)D_D758 + 20)) = Tag_D758((*(Tag_D758*)((char*)D_D758 + 20)));
    }
    unsigned buf64[6];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_D758(buf, D_D758, 24);
    Ent_D758 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446190[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_D758 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_D758 *activeNode = *(O_D758**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16()) {
            int k = buf[1];
            if (k != -1) {
                V4_D758 pos = *shape_D758(k)->v05();
                M4_D758 *riderTransform = shape_D758(buf[1])->v06();
                vAddTo_D758(pos, mtxApply_D758(riderTransform, V4_D758(*(float*)&buf[2], *(float*)&buf[3], *(float*)&buf[4], 0.0f)));
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
extern char *cWScriptMan_TheWScriptMan_D9F8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_D9F8 __asm__("SSXApp");
struct V4_D9F8 { float x, y, z, w; V4_D9F8(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern "C" void cMoveNode_addParent_D9F8(void*, int, int, V4_D9F8*) __asm__("cMoveNode_addParent");
struct O_D9F8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
static inline char *refToPtr_D9F8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_D9F8(unsigned i) { if (i == 0) return 0; return refToPtr_D9F8(i); }
struct V3_D9F8 { float x, y, z; };
extern V3_D9F8 luno_gLunoNil_D9F8 __asm__("luno_gLunoNil");
struct Tag_D9F8 { char c; Tag_D9F8() {} Tag_D9F8(const Tag_D9F8 &o) : c(o.c) {} };
extern unsigned D_D9F8[] __asm__("D_004FB738");
extern int D_004A560C;
extern const int D_004461A8[];
extern "C" void* MEM_copy_D9F8(void*, void*, int) __asm__("MEM_copy");
struct Ent_D9F8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[17]): attaches a moving world instance to a parent instance (a parent modifier).
 *
 * Copies the static defaults (set up on the first call) into a 28-byte parameter block (On Tour tAddParentData: InstId,
 * ParentId, Mesh, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_004461A8 says 2. Defaults: InstId -1, ParentId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when ParentId is -1, the parent is not found, has
 * no active node or that node is ATYPE_DEAD (virtual isType(6)). Otherwise, when the instance's own active node is a
 * move node (virtual isMoveNode), cMoveNode_addParent gets the parent node's instance record (+0x18), Mesh and the
 * offset (OffsetX, OffsetY, OffsetZ, 0).
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_D9F8 AddParentFunc(int n, Ent_D9F8 *params) {
    if (D_004A560C == 0) {
        D_D9F8[0] = 0xFFFFFFFF;
        D_D9F8[1] = 0xFFFFFFFF;
        ((int*)D_D9F8)[2] = 0;
        float z = 0.0f;
        *(float*)&D_D9F8[3] = z;
        *(float*)&D_D9F8[4] = z;
        *(float*)&D_D9F8[5] = z;
        D_004A560C = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_D9F8*)((char*)D_D9F8 + 24)) = Tag_D9F8((*(Tag_D9F8*)((char*)D_D9F8 + 24)));
    }
    unsigned buf64[7];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_D9F8(buf, D_D9F8, 28);
    Ent_D9F8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004461A8[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_D9F8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_D9F8(i);
            inst = t;
        }
        obj = inst;
    }
    unsigned id2 = buf[1];
    if (~id2 != 0) {
        char *parentSection = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id2 & 0xFF];
        unsigned parentRef;
        char *parentInst;
        if (!parentSection || (parentRef = ((unsigned*)*(char**)(parentSection + 0x1C))[id2 >> 8] >> 8) == 0)
            parentInst = 0;
        else
            parentInst = (char*)(parentRef << 2);
        O_D9F8 *parentNode = parentInst ? *(O_D9F8**)(parentInst + 0xC) : 0;
        if (parentNode && !parentNode->v10(6)) {
            O_D9F8 *activeNode = *(O_D9F8**)(obj + 0xC);
            if (activeNode && activeNode->v16()) {
                int k = *(int*)((char*)parentNode + 0x18);
                V4_D9F8 offset(*(float*)&buf[3], *(float*)&buf[4], *(float*)&buf[5], 0.0f);
                cMoveNode_addParent_D9F8(activeNode, k, buf[2], &offset);
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
extern char *cWScriptMan_TheWScriptMan_DC60 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_DC60 __asm__("SSXApp");
struct V4_DC60 { float x, y, z, w; V4_DC60(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
extern "C" void cMoveNode_addParent_DC60(void*, int, int, V4_DC60*) __asm__("cMoveNode_addParent");
struct O_DC60 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
static inline char *refToPtr_DC60(unsigned i) { return (char*)(i << 2); }
static inline char *conv_DC60(unsigned i) { if (i == 0) return 0; return refToPtr_DC60(i); }
struct V3_DC60 { float x, y, z; };
extern V3_DC60 luno_gLunoNil_DC60 __asm__("luno_gLunoNil");
struct Tag_DC60 { char c; Tag_DC60() {} Tag_DC60(const Tag_DC60 &o) : c(o.c) {} };
extern unsigned D_DC60[] __asm__("D_004FB758");
extern int D_004A5610;
extern const int D_004461C0[];
extern "C" void* MEM_copy_DC60(void*, void*, int) __asm__("MEM_copy");
struct Ent_DC60 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[18]): makes a world instance the parent of a moving child instance (a parent
 * modifier on the child).
 *
 * Copies the static defaults (set up on the first call) into a 28-byte parameter block (On Tour tAddChildData: InstId,
 * ChildId, Mesh, OffsetX, OffsetY, OffsetZ) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_004461C0 says 2. Defaults: InstId -1, ChildId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when ChildId is -1, the instance has no active
 * node (+0xC) or that node is ATYPE_DEAD (virtual isType(6)). Otherwise ChildId is looked up the same way and, when the
 * child's active node is a move node (virtual isMoveNode), cMoveNode_addParent gets the instance's node record (+0x18),
 * Mesh and the offset (OffsetX, OffsetY, OffsetZ, 0), so the child follows this instance.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_DC60 AddChildFunc(int n, Ent_DC60 *params) {
    if (D_004A5610 == 0) {
        D_DC60[0] = 0xFFFFFFFF;
        D_DC60[1] = 0xFFFFFFFF;
        ((int*)D_DC60)[2] = 0;
        float z = 0.0f;
        *(float*)&D_DC60[3] = z;
        *(float*)&D_DC60[4] = z;
        *(float*)&D_DC60[5] = z;
        D_004A5610 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_DC60*)((char*)D_DC60 + 24)) = Tag_DC60((*(Tag_DC60*)((char*)D_DC60 + 24)));
    }
    unsigned buf64[7];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_DC60(buf, D_DC60, 28);
    Ent_DC60 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004461C0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_DC60 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_DC60(i);
            inst = t;
        }
        obj = inst;
    }
    if (~buf[1] != 0) {
        O_DC60 *parentNode = *(O_DC60**)(obj + 0xC);
        if (parentNode && !parentNode->v10(6)) {
            unsigned id2 = buf[1];
            char *childSection = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id2 & 0xFF];
            unsigned childRef;
            char *childInst;
            if (!childSection || (childRef = ((unsigned*)*(char**)(childSection + 0x1C))[id2 >> 8] >> 8) == 0)
                childInst = 0;
            else
                childInst = (char*)(childRef << 2);
            O_DC60 *childNode = childInst ? *(O_DC60**)(childInst + 0xC) : 0;
            if (childNode && childNode->v16()) {
                int k = *(int*)((char*)parentNode + 0x18);
                V4_DC60 offset(*(float*)&buf[3], *(float*)&buf[4], *(float*)&buf[5], 0.0f);
                cMoveNode_addParent_DC60(childNode, k, buf[2], &offset);
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
extern char *cWScriptMan_TheWScriptMan_DED0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_DED0 __asm__("SSXApp");
extern "C" void cMoveNode_addSpline1_DED0(void*, void*) __asm__("cMoveNode_addSpline1");
struct O_DED0 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
static inline char *refToPtr_DED0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_DED0(unsigned i) { if (i == 0) return 0; return refToPtr_DED0(i); }
struct V3_DED0 { float x, y, z; };
extern V3_DED0 luno_gLunoNil_DED0 __asm__("luno_gLunoNil");
struct Tag_DED0 { char c; Tag_DED0() {} Tag_DED0(const Tag_DED0 &o) : c(o.c) {} };
extern unsigned D_DED0[] __asm__("D_004FB778");
extern int D_004A5614;
extern const int D_004461D8[];
extern "C" void* MEM_copy_DED0(void*, void*, int) __asm__("MEM_copy");
struct Ent_DED0 { int idx; int val; int pad; int type; };
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
extern "C" V3_DED0 AddSplineFunc(int n, Ent_DED0 *params) {
    if (D_004A5614 == 0) {
        D_DED0[0] = 0xFFFFFFFF;
        D_DED0[1] = 0xFFFFFFFF;
        ((int*)D_DED0)[2] = 1;
        ((int*)D_DED0)[3] = 3;
        *(float*)&D_DED0[4] = 20.0f;
        float z = 0.0f;
        *(float*)&D_DED0[5] = z;
        *(float*)&D_DED0[6] = z;
        *(float*)&D_DED0[7] = z;
        ((int*)D_DED0)[8] = 1;
        *(float*)&D_DED0[9] = z;
        *(float*)&D_DED0[10] = z;
        D_004A5614 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_DED0*)((char*)D_DED0 + 44)) = Tag_DED0((*(Tag_DED0*)((char*)D_DED0 + 44)));
    }
    unsigned buf64[12];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_DED0(buf, D_DED0, 48);
    Ent_DED0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004461D8[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_DED0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_DED0(i);
            inst = t;
        }
        obj = inst;
    }
    if (~buf[1] != 0) {
        O_DED0 *activeNode = *(O_DED0**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
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
extern char *cWScriptMan_TheWScriptMan_E0C0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_E0C0 __asm__("SSXApp");
extern "C" void cMoveNode_addSpline_E0C0(void*, void*) __asm__("cMoveNode_addSpline");
struct O_E0C0 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
static inline char *refToPtr_E0C0(unsigned i) { return (char*)(i << 2); }
static inline char *conv_E0C0(unsigned i) { if (i == 0) return 0; return refToPtr_E0C0(i); }
struct V3_E0C0 { float x, y, z; };
extern V3_E0C0 luno_gLunoNil_E0C0 __asm__("luno_gLunoNil");
struct Tag_E0C0 { char c; Tag_E0C0() {} Tag_E0C0(const Tag_E0C0 &o) : c(o.c) {} };
extern unsigned D_E0C0[] __asm__("D_004FB7A8");
extern int D_004A5618;
extern const int D_00446208[];
extern "C" void* MEM_copy_E0C0(void*, void*, int) __asm__("MEM_copy");
struct Ent_E0C0 { int idx; int val; int pad; int type; };
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
extern "C" V3_E0C0 AddMultiSplineFunc(int n, Ent_E0C0 *params) {
    if (D_004A5618 == 0) {
        D_E0C0[0] = 0xFFFFFFFF;
        D_E0C0[1] = 0xFFFFFFFF;
        ((int*)D_E0C0)[2] = 3;
        *(float*)&D_E0C0[3] = 20.0f;
        float z = 0.0f;
        *(float*)&D_E0C0[4] = z;
        *(float*)&D_E0C0[5] = z;
        *(float*)&D_E0C0[6] = 1.0f;
        *(float*)&D_E0C0[7] = z;
        *(float*)&D_E0C0[8] = z;
        *(float*)&D_E0C0[9] = z;
        ((int*)D_E0C0)[10] = 1;
        ((int*)D_E0C0)[11] = 2;
        D_004A5618 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_E0C0*)((char*)D_E0C0 + 48)) = Tag_E0C0((*(Tag_E0C0*)((char*)D_E0C0 + 48)));
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_E0C0(buf, D_E0C0, 52);
    Ent_E0C0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446208[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_E0C0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_E0C0(i);
            inst = t;
        }
        obj = inst;
    }
    if (~buf[1] != 0) {
        O_E0C0 *activeNode = *(O_E0C0**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
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
extern char *cWScriptMan_TheWScriptMan_E2C0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_E2C0 __asm__("SSXApp");
extern "C" void cMoveNode_addUVScroll_E2C0(void*, void*) __asm__("cMoveNode_addUVScroll");
struct O_E2C0 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_E2C0 { float x, y, z; };
extern V3_E2C0 luno_gLunoNil_E2C0 __asm__("luno_gLunoNil");
struct Tag_E2C0 { char c; Tag_E2C0() {} Tag_E2C0(const Tag_E2C0 &o) : c(o.c) {} };
extern unsigned D_E2C0[] __asm__("D_004FB7E0");
extern int D_004A561C;
extern const int D_00446238[];
extern "C" void* MEM_copy_E2C0(void*, void*, int) __asm__("MEM_copy");
struct Ent_E2C0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[21]): adds a UV-scroll modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 52-byte parameter block (On Tour tAddUVScrollData:
 * InstId, Type, UStart, VStart, UScroll, VScroll, TimeOn, TimeOff, AngVel, AxisX, AxisY, AxisZ) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_00446238 says 2.
 * Defaults: InstId -1, Type 5, UScroll 0.0166667, VScroll 0.0166667, TimeOn 1, AxisZ 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode), cMoveNode_addUVScroll gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_E2C0 AddUVScrollFunc(int n, Ent_E2C0 *params) {
    if (D_004A561C == 0) {
        D_E2C0[0] = 0xFFFFFFFF;
        ((int*)D_E2C0)[1] = 5;
        float z = 0.0f;
        *(float*)&D_E2C0[2] = z;
        *(float*)&D_E2C0[3] = z;
        *(float*)&D_E2C0[4] = 0.016666699200868607f;
        *(float*)&D_E2C0[5] = 0.016666699200868607f;
        *(float*)&D_E2C0[6] = 1.0f;
        *(float*)&D_E2C0[7] = z;
        *(float*)&D_E2C0[8] = z;
        *(float*)&D_E2C0[9] = z;
        *(float*)&D_E2C0[10] = z;
        *(float*)&D_E2C0[11] = 1.0f;
        D_004A561C = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_E2C0*)((char*)D_E2C0 + 48)) = Tag_E2C0((*(Tag_E2C0*)((char*)D_E2C0 + 48)));
    }
    unsigned buf64[13];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E2C0(buf, D_E2C0, 52);
    Ent_E2C0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446238[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_E2C0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_E2C0 *activeNode = *(O_E2C0**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16())
            cMoveNode_addUVScroll_E2C0(activeNode, buf);
    }
    return luno_gLunoNil_E2C0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddTexFlipFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_E4A8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_E4A8 __asm__("SSXApp");
extern "C" void cMoveNode_addTexFlip_E4A8(void*, void*) __asm__("cMoveNode_addTexFlip");
struct O_E4A8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_E4A8 { float x, y, z; };
extern V3_E4A8 luno_gLunoNil_E4A8 __asm__("luno_gLunoNil");
struct Tag_E4A8 { char c; Tag_E4A8() {} Tag_E4A8(const Tag_E4A8 &o) : c(o.c) {} };
extern unsigned D_E4A8[] __asm__("D_004FB818");
extern int D_004A5620;
extern const int D_00446268[];
extern "C" void* MEM_copy_E4A8(void*, void*, int) __asm__("MEM_copy");
struct Ent_E4A8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[22]): adds a texture-flip modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 24-byte parameter block (On Tour tAddTexFlipData:
 * InstId, Type, StartId, Dir, Speed) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_00446268 says 2. Defaults: InstId -1, Speed 2.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode), cMoveNode_addTexFlip gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_E4A8 AddTexFlipFunc(int n, Ent_E4A8 *params) {
    if (D_004A5620 == 0) {
        D_E4A8[0] = 0xFFFFFFFF;
        ((int*)D_E4A8)[1] = 0;
        ((int*)D_E4A8)[2] = 0;
        ((int*)D_E4A8)[3] = 0;
        *(float*)&D_E4A8[4] = 2.0f;
        D_004A5620 = 1;
        (*(Tag_E4A8*)((char*)D_E4A8 + 20)) = Tag_E4A8((*(Tag_E4A8*)((char*)D_E4A8 + 20)));
    }
    unsigned buf64[6];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E4A8(buf, D_E4A8, 24);
    Ent_E4A8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446268[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_E4A8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_E4A8 *activeNode = *(O_E4A8**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16())
            cMoveNode_addTexFlip_E4A8(activeNode, buf);
    }
    return luno_gLunoNil_E4A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddBoostFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_E668 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_E668 __asm__("SSXApp");
extern "C" void cMoveNode_addBoost_E668(void*, void*) __asm__("cMoveNode_addBoost");
struct O_E668 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_E668 { float x, y, z; };
extern V3_E668 luno_gLunoNil_E668 __asm__("luno_gLunoNil");
struct Tag_E668 { char c; Tag_E668() {} Tag_E668(const Tag_E668 &o) : c(o.c) {} };
extern unsigned D_E668[] __asm__("D_004FB830");
extern int D_004A5624;
extern const int D_00446280[];
extern "C" void* MEM_copy_E668(void*, void*, int) __asm__("MEM_copy");
struct Ent_E668 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[24]): adds a boost modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 36-byte parameter block (On Tour tAddBoostData: InstId,
 * Type, BoostTime, Drag, MaxSpeed, BoostX, BoostY, BoostZ) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_00446280 says 2. Defaults: InstId -1, Type 1, Drag 0.1,
 * MaxSpeed 20, BoostZ 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode), cMoveNode_addBoost gets the whole block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_E668 AddBoostFunc(int n, Ent_E668 *params) {
    if (D_004A5624 == 0) {
        D_E668[0] = 0xFFFFFFFF;
        ((int*)D_E668)[1] = 1;
        float z = 0.0f;
        *(float*)&D_E668[2] = z;
        *(float*)&D_E668[3] = 0.10000000149011612f;
        *(float*)&D_E668[4] = 20.0f;
        *(float*)&D_E668[5] = z;
        *(float*)&D_E668[6] = z;
        *(float*)&D_E668[7] = 1.0f;
        D_004A5624 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_E668*)((char*)D_E668 + 32)) = Tag_E668((*(Tag_E668*)((char*)D_E668 + 32)));
    }
    unsigned buf64[9];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_E668(buf, D_E668, 36);
    Ent_E668 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446280[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_E668 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_E668 *activeNode = *(O_E668**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16())
            cMoveNode_addBoost_E668(activeNode, buf);
    }
    return luno_gLunoNil_E668;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_E840 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_E840(unsigned i) { return (char*)(i << 2); }
static inline char *conv_E840(unsigned i) { if (i == 0) return 0; return refToPtr_E840(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cParticleNode_cParticleNode1_E840(void*, int, void*, void*, int) __asm__("cParticleNode_cParticleNode1");
void operator_delete(int*);
extern char D_00489740_E840[] __asm__("D_00489740");
extern char D_00489730_E840[] __asm__("D_00489730");
struct V4_E840 { float x, y, z, w; } __attribute__((aligned(16)));
struct Sh_E840 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual V4_E840 *v05(); };
struct Ent_E840x { char pad[0x18]; char *obj; };
struct Lvl_E840 { char pad[0x40]; Ent_E840x *a1[12]; Ent_E840x *a2[3]; int n1; int pad80[2]; int n2; };
struct O_E840 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual char *v24(); };
extern char *SSXApp_E840 __asm__("SSXApp");
extern "C" void cMoveNode_addParticle_E840(void*, void*) __asm__("cMoveNode_addParticle");
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline V4_E840 vSub_E840(const V4_E840 &a, const V4_E840 &b) {
    V4_E840 r;
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
static inline float vLen_E840(const V4_E840 &v) {
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
static inline Lvl_E840 *lvl_E840() { return *(Lvl_E840**)(*(char**)(SSXApp_E840 + 0x84) + 0xC); }
static inline Sh_E840 *shape_E840(Ent_E840x *e) { return (Sh_E840*)(e->obj + 0x6C0); }
static inline int nearViewer_E840(const V4_E840 &pos) {
    int notEdit = ScriptEngine->mPlayingNISList != 2;
    if (notEdit)
        return 1;
    int n = lvl_E840()->n1;
    for (int i = 0; i < n; i++) {
        V4_E840 d = vSub_E840(pos, *shape_E840(lvl_E840()->a1[i])->v05());
        if (vLen_E840(d) < 30000.0f)
            return 1;
    }
    n = lvl_E840()->n2;
    for (int i = 0; i < n; i++) {
        V4_E840 d = vSub_E840(pos, *shape_E840(lvl_E840()->a2[i])->v05());
        if (vLen_E840(d) < 30000.0f)
            return 1;
    }
    return 0;
}
struct V3_E840 { float x, y, z; };
extern V3_E840 luno_gLunoNil_E840 __asm__("luno_gLunoNil");
struct Tag_E840 { char c; Tag_E840() {} Tag_E840(const Tag_E840 &o) : c(o.c) {} };
extern unsigned D_E840[] __asm__("D_004FB858");
extern int D_004A5628;
extern const int D_004462A0[];
extern "C" void* MEM_copy_E840(void*, void*, int) __asm__("MEM_copy");
struct Ent_E840 { int idx; int val; int pad; int type; };
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
extern "C" V3_E840 AddParticleFunc(int n, Ent_E840 *params) {
    if (D_004A5628 == 0) {
        ((int*)D_E840)[0] = 1;
        ((int*)D_E840)[1] = 0;
        *(float*)&D_E840[2] = -1.0f;
        *(float*)&D_E840[3] = 1.0f;
        *(float*)&D_E840[4] = 4.0f;
        float z = 0.0f;
        *(float*)&D_E840[5] = z;
        *(float*)&D_E840[6] = z;
        *(float*)&D_E840[7] = z;
        *(float*)&D_E840[8] = z;
        *(float*)&D_E840[9] = z;
        *(float*)&D_E840[10] = z;
        *(float*)&D_E840[11] = z;
        *(float*)&D_E840[12] = z;
        *(float*)&D_E840[13] = z;
        *(float*)&D_E840[14] = z;
        *(float*)&D_E840[15] = z;
        *(float*)&D_E840[16] = z;
        *(float*)&D_E840[17] = z;
        *(float*)&D_E840[18] = z;
        *(float*)&D_E840[19] = z;
        *(float*)&D_E840[20] = z;
        *(float*)&D_E840[21] = z;
        *(float*)&D_E840[22] = z;
        *(float*)&D_E840[23] = z;
        *(float*)&D_E840[24] = z;
        *(float*)&D_E840[25] = z;
        *(float*)&D_E840[26] = z;
        *(float*)&D_E840[27] = z;
        *(float*)&D_E840[28] = z;
        *(float*)&D_E840[29] = z;
        *(float*)&D_E840[30] = z;
        *(float*)&D_E840[31] = z;
        *(float*)&D_E840[32] = z;
        *(float*)&D_E840[33] = 1.0f;
        *(float*)&D_E840[34] = z;
        *(float*)&D_E840[35] = z;
        *(float*)&D_E840[36] = z;
        *(float*)&D_E840[37] = 1.0f;
        *(float*)&D_E840[38] = 1.0f;
        *(float*)&D_E840[39] = 1.0f;
        *(float*)&D_E840[40] = 1.0f;
        *(float*)&D_E840[41] = 1.0f;
        *(float*)&D_E840[42] = z;
        *(float*)&D_E840[43] = z;
        *(float*)&D_E840[44] = z;
        *(float*)&D_E840[45] = 1.0f;
        *(float*)&D_E840[46] = z;
        *(float*)&D_E840[47] = z;
        *(float*)&D_E840[48] = z;
        ((int*)D_E840)[49] = 16;
        ((int*)D_E840)[50] = 0;
        *(float*)&D_E840[51] = z;
        ((int*)D_E840)[52] = 1;
        *(float*)&D_E840[53] = 20.0f;
        D_E840[54] = 0xFFFFFFFF;
        D_004A5628 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_E840*)((char*)D_E840 + 0xDC)) = Tag_E840((*(Tag_E840*)((char*)D_E840 + 0xDC)));
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xE0, D_00489740_E840, 0x20000000, 0);
    buf[54] = 0xFFFFFFFF;
    MEM_copy_E840(buf, D_E840, 0xE0);
    Ent_E840 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004462A0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    if (*(float*)&buf[7] > *(float*)&buf[5])
        *(float*)&buf[7] = *(float*)&buf[5];
    unsigned id = buf[54];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_E840 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_E840(i);
            inst = t;
        }
        obj = inst;
    }
    O_E840 *activeNode = *(O_E840**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v16()) {
            V4_E840 pos = *(V4_E840*)(activeNode->v24() + 0x30);
            if (nearViewer_E840(pos)) {
                cMoveNode_addParticle_E840(activeNode, buf);
                return luno_gLunoNil_E840;
            }
        }
    } else {
        V4_E840 pos = *(V4_E840*)(obj + 0x40);
        if (nearViewer_E840(pos)) {
            cParticleNode_cParticleNode1_E840(cMemMan_alloc(44, D_00489730_E840, 0x20000000, 0), 1, obj, buf, 0);
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
extern char *cWScriptMan_TheWScriptMan_EE98 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_EE98(unsigned i) { return (char*)(i << 2); }
static inline char *conv_EE98(unsigned i) { if (i == 0) return 0; return refToPtr_EE98(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
struct V4_EE98 { float x, y, z, w; V4_EE98(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
struct O_EE98 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
extern "C" void cMoveNode_addDynamicParticle_EE98(void*, void*, int, V4_EE98*) __asm__("cMoveNode_addDynamicParticle");
void operator_delete(int*);
extern char D_00489750_EE98[] __asm__("D_00489750");
struct V3_EE98 { float x, y, z; };
extern V3_EE98 luno_gLunoNil_EE98 __asm__("luno_gLunoNil");
struct Tag_EE98 { char c; Tag_EE98() {} Tag_EE98(const Tag_EE98 &o) : c(o.c) {} };
extern unsigned D_EE98[] __asm__("D_004FB938");
extern int D_004A562C;
extern const int D_00446380[];
extern "C" void* MEM_copy_EE98(void*, void*, int) __asm__("MEM_copy");
struct Ent_EE98 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[26]): adds a dynamic (moving) particle emitter to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 240-byte parameter block (On Tour
 * tAddDynamicParticleData: NumParticles, NumBlur, Duration, Damp, Size, Life, SizeR, LifeR, BlurStep, OffX, OffY, OffZ,
 * R0X, R0Y, R0Z, R1X, R1Y, R1Z, VelX, VelY, VelZ, R0VX, R0VY, R0VZ, R1VX, R1VY, R1VZ, R2VX, R2VY, R2VZ, ForceX, ForceY,
 * ForceZ, StartColA, StartColR, StartColG, StartColB, EndColA, EndColR, EndColG, EndColB, R0A, R0R, R0G, R0B, R1A, R1R,
 * R1G, R1B, TextureId, BlendMode, SizeFinal, NumFlipTextures, FlipTextureRate, InstId, Mesh, OffsetX, OffsetY, OffsetZ)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_00446380 says 2. Defaults: NumParticles 1, Duration -1, Damp 1, Size 4, StartColA 1, EndColA 1, EndColR 1, EndColG
 * 1, EndColB 1, R0A 1, R1A 1, TextureId 16, NumFlipTextures 1, FlipTextureRate 20, InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan +0x290); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The block is heap-allocated (cMemMan_alloc 0xF0,
 * "AddDynamicParticleData") because the modifier keeps it; InstId sits at +0xD8. When the instance's active node (+0xC)
 * is a move node (virtual isMoveNode), cMoveNode_addDynamicParticle gets the block, Mesh and the offset (OffsetX,
 * OffsetY, OffsetZ, 1); otherwise the block is freed.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_EE98 AddDynamicParticleFunc(int n, Ent_EE98 *params) {
    if (D_004A562C == 0) {
        ((int*)D_EE98)[0] = 1;
        ((int*)D_EE98)[1] = 0;
        *(float*)&D_EE98[2] = -1.0f;
        *(float*)&D_EE98[3] = 1.0f;
        *(float*)&D_EE98[4] = 4.0f;
        float z = 0.0f;
        *(float*)&D_EE98[5] = z;
        *(float*)&D_EE98[6] = z;
        *(float*)&D_EE98[7] = z;
        *(float*)&D_EE98[8] = z;
        *(float*)&D_EE98[9] = z;
        *(float*)&D_EE98[10] = z;
        *(float*)&D_EE98[11] = z;
        *(float*)&D_EE98[12] = z;
        *(float*)&D_EE98[13] = z;
        *(float*)&D_EE98[14] = z;
        *(float*)&D_EE98[15] = z;
        *(float*)&D_EE98[16] = z;
        *(float*)&D_EE98[17] = z;
        *(float*)&D_EE98[18] = z;
        *(float*)&D_EE98[19] = z;
        *(float*)&D_EE98[20] = z;
        *(float*)&D_EE98[21] = z;
        *(float*)&D_EE98[22] = z;
        *(float*)&D_EE98[23] = z;
        *(float*)&D_EE98[24] = z;
        *(float*)&D_EE98[25] = z;
        *(float*)&D_EE98[26] = z;
        *(float*)&D_EE98[27] = z;
        *(float*)&D_EE98[28] = z;
        *(float*)&D_EE98[29] = z;
        *(float*)&D_EE98[30] = z;
        *(float*)&D_EE98[31] = z;
        *(float*)&D_EE98[32] = z;
        *(float*)&D_EE98[33] = 1.0f;
        *(float*)&D_EE98[34] = z;
        *(float*)&D_EE98[35] = z;
        *(float*)&D_EE98[36] = z;
        *(float*)&D_EE98[37] = 1.0f;
        *(float*)&D_EE98[38] = 1.0f;
        *(float*)&D_EE98[39] = 1.0f;
        *(float*)&D_EE98[40] = 1.0f;
        *(float*)&D_EE98[41] = 1.0f;
        *(float*)&D_EE98[42] = z;
        *(float*)&D_EE98[43] = z;
        *(float*)&D_EE98[44] = z;
        *(float*)&D_EE98[45] = 1.0f;
        *(float*)&D_EE98[46] = z;
        *(float*)&D_EE98[47] = z;
        *(float*)&D_EE98[48] = z;
        ((int*)D_EE98)[49] = 16;
        ((int*)D_EE98)[50] = 0;
        *(float*)&D_EE98[51] = z;
        ((int*)D_EE98)[52] = 1;
        *(float*)&D_EE98[53] = 20.0f;
        D_EE98[54] = 0xFFFFFFFF;
        ((int*)D_EE98)[55] = 0;
        *(float*)&D_EE98[56] = z;
        *(float*)&D_EE98[57] = z;
        *(float*)&D_EE98[58] = z;
        D_004A562C = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_EE98*)((char*)D_EE98 + 0xEC)) = Tag_EE98((*(Tag_EE98*)((char*)D_EE98 + 0xEC)));
    }
    unsigned *buf = (unsigned*)cMemMan_alloc(0xF0, D_00489750_EE98, 0x20000000, 0);
    buf[54] = 0xFFFFFFFF;
    MEM_copy_EE98(buf, D_EE98, 0xF0);
    Ent_EE98 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446380[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    if (*(float*)&buf[7] > *(float*)&buf[5])
        *(float*)&buf[7] = *(float*)&buf[5];
    unsigned id = buf[54];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_EE98 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_EE98 *activeNode = *(O_EE98**)(obj + 0xC);
    if (activeNode && activeNode->v16()) {
        float one = 1.0f;
        V4_EE98 offset(*(float*)&buf[56], *(float*)&buf[57], *(float*)&buf[58], one);
        cMoveNode_addDynamicParticle_EE98(activeNode, buf, buf[55], &offset);
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
extern char *cWScriptMan_TheWScriptMan_F1C8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_F1C8 __asm__("SSXApp");
extern "C" void cMoveNode_addRail_F1C8(void*, void*) __asm__("cMoveNode_addRail");
struct O_F1C8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_F1C8 { float x, y, z; };
extern V3_F1C8 luno_gLunoNil_F1C8 __asm__("luno_gLunoNil");
struct Tag_F1C8 { char c; Tag_F1C8() {} Tag_F1C8(const Tag_F1C8 &o) : c(o.c) {} };
extern unsigned D_F1C8[] __asm__("D_004FBA28");
extern int D_004A5630;
extern const int D_00446470[];
extern "C" void* MEM_copy_F1C8(void*, void*, int) __asm__("MEM_copy");
struct Ent_F1C8 { int idx; int val; int pad; int type; };
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
extern "C" V3_F1C8 AddRailFunc(int n, Ent_F1C8 *params) {
    if (D_004A5630 == 0) {
        D_F1C8[0] = 0xFFFFFFFF;
        D_F1C8[1] = 0xFFFFFFFF;
        ((int*)D_F1C8)[2] = -1;
        D_004A5630 = 1;
        (*(Tag_F1C8*)((char*)D_F1C8 + 12)) = Tag_F1C8((*(Tag_F1C8*)((char*)D_F1C8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_F1C8(buf, D_F1C8, 16);
    Ent_F1C8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446470[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_F1C8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (~buf[1] != 0) {
        O_F1C8 *activeNode = *(O_F1C8**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
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
extern char *cWScriptMan_TheWScriptMan_F390 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_F390(unsigned i) { return (char*)(i << 2); }
static inline char *conv_F390(unsigned i) { if (i == 0) return 0; return refToPtr_F390(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cFloatRailNode_cFloatRailNode_F390(void*, void*, void*) __asm__("cFloatRailNode_cFloatRailNode");
extern char D_00489768_F390[] __asm__("D_00489768");
struct V3_F390 { float x, y, z; };
extern V3_F390 luno_gLunoNil_F390 __asm__("luno_gLunoNil");
struct Tag_F390 { char c; Tag_F390() {} Tag_F390(const Tag_F390 &o) : c(o.c) {} };
extern unsigned D_F390[] __asm__("D_004FBA38");
extern int D_004A5634;
extern const int D_00446480[];
extern "C" void* MEM_copy_F390(void*, void*, int) __asm__("MEM_copy");
struct Ent_F390 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[49]): hangs a world instance on up to eight others as a floating rail (a new
 * FloatRail node), unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tMakeFloatRailData:
 * InstId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, Mesh1, Mesh2, Mesh3, Mesh4, Mesh5,
 * Mesh6, Mesh7, Mesh8, Flags, LSpring, LDamp, ForceScale) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_00446480 says 2. Defaults: InstId -1, InstId1 -1, InstId2
 * -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7 -1, InstId8 -1, Mesh1 -1, Mesh2 -1, Mesh3 -1, Mesh4 -1,
 * Mesh5 -1, Mesh6 -1, Mesh7 -1, Mesh8 -1, Flags 1, LSpring 0.5, LDamp 0.5, ForceScale 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 20 = ATYPE_FLOATRAIL, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cFloatRailNode (cFloatRailNode_cFloatRailNode, cMemMan_alloc 0x80,
 * "FloatRail") on the instance from the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_F390 MakeFloatRailFunc(int n, Ent_F390 *params) {
    if (D_004A5634 == 0) {
        D_F390[0] = 0xFFFFFFFF;
        D_F390[1] = 0xFFFFFFFF;
        D_F390[2] = 0xFFFFFFFF;
        D_F390[3] = 0xFFFFFFFF;
        D_F390[4] = 0xFFFFFFFF;
        D_F390[5] = 0xFFFFFFFF;
        D_F390[6] = 0xFFFFFFFF;
        D_F390[7] = 0xFFFFFFFF;
        D_F390[8] = 0xFFFFFFFF;
        ((int*)D_F390)[9] = -1;
        ((int*)D_F390)[10] = -1;
        ((int*)D_F390)[11] = -1;
        ((int*)D_F390)[12] = -1;
        ((int*)D_F390)[13] = -1;
        ((int*)D_F390)[14] = -1;
        ((int*)D_F390)[15] = -1;
        ((int*)D_F390)[16] = -1;
        ((int*)D_F390)[17] = 1;
        *(float*)&D_F390[18] = 0.5f;
        *(float*)&D_F390[19] = 0.5f;
        *(float*)&D_F390[20] = 1.0f;
        D_004A5634 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_F390*)((char*)D_F390 + 84)) = Tag_F390((*(Tag_F390*)((char*)D_F390 + 84)));
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
    MEM_copy_F390(buf, D_F390, 88);
    Ent_F390 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446480[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_F390 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_F390(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_F390;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 20, buf))
        return luno_gLunoNil_F390;
    cFloatRailNode_cFloatRailNode_F390(cMemMan_alloc(0x80, D_00489768_F390, 0x20000000, 0), obj, buf);
    return luno_gLunoNil_F390;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MakeSpringRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_F5E8 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_F5E8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_F5E8(unsigned i) { if (i == 0) return 0; return refToPtr_F5E8(i); }
extern "C" int checkActiveNode(void*, int, void*);
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cSpringRailNode_cSpringRailNode_F5E8(void*, void*, void*) __asm__("cSpringRailNode_cSpringRailNode");
extern char D_00489778_F5E8[] __asm__("D_00489778");
struct V3_F5E8 { float x, y, z; };
extern V3_F5E8 luno_gLunoNil_F5E8 __asm__("luno_gLunoNil");
struct Tag_F5E8 { char c; Tag_F5E8() {} Tag_F5E8(const Tag_F5E8 &o) : c(o.c) {} };
extern unsigned D_F5E8[] __asm__("D_004FBA90");
extern int D_004A5638;
extern const int D_004464D8[];
extern "C" void* MEM_copy_F5E8(void*, void*, int) __asm__("MEM_copy");
struct Ent_F5E8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[70]): hangs a world instance on up to eight others as a springy rail (a new
 * SpringRail node), unless its current node is busy.
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tMakeSpringRailData:
 * InstId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, Mesh1, Mesh2, Mesh3, Mesh4, Mesh5,
 * Mesh6, Mesh7, Mesh8, Flags, ASpring, ADamp, TorqueScale) and lets each argument override the field its index names,
 * turning an int into a float where the field type table D_004464D8 says 2. Defaults: InstId -1, InstId1 -1, InstId2
 * -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7 -1, InstId8 -1, Mesh1 -1, Mesh2 -1, Mesh3 -1, Mesh4 -1,
 * Mesh5 -1, Mesh6 -1, Mesh7 -1, Mesh8 -1, Flags 1, ASpring 0.5, ADamp 0.5, TorqueScale 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is kept by checkActiveNode(node, 21 = ATYPE_SPRINGRAIL, block), which keeps a busy or same-type node and
 * replaces some kinds itself; otherwise it builds a cSpringRailNode (cSpringRailNode_cSpringRailNode, cMemMan_alloc
 * 0x80, "SpringRail") on the instance from the block, after scaling TorqueScale by 0.001.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_F5E8 MakeSpringRailFunc(int n, Ent_F5E8 *params) {
    if (D_004A5638 == 0) {
        D_F5E8[0] = 0xFFFFFFFF;
        D_F5E8[1] = 0xFFFFFFFF;
        D_F5E8[2] = 0xFFFFFFFF;
        D_F5E8[3] = 0xFFFFFFFF;
        D_F5E8[4] = 0xFFFFFFFF;
        D_F5E8[5] = 0xFFFFFFFF;
        D_F5E8[6] = 0xFFFFFFFF;
        D_F5E8[7] = 0xFFFFFFFF;
        D_F5E8[8] = 0xFFFFFFFF;
        ((int*)D_F5E8)[9] = -1;
        ((int*)D_F5E8)[10] = -1;
        ((int*)D_F5E8)[11] = -1;
        ((int*)D_F5E8)[12] = -1;
        ((int*)D_F5E8)[13] = -1;
        ((int*)D_F5E8)[14] = -1;
        ((int*)D_F5E8)[15] = -1;
        ((int*)D_F5E8)[16] = -1;
        ((int*)D_F5E8)[17] = 1;
        *(float*)&D_F5E8[18] = 0.5f;
        *(float*)&D_F5E8[19] = 0.5f;
        *(float*)&D_F5E8[20] = 1.0f;
        D_004A5638 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_F5E8*)((char*)D_F5E8 + 84)) = Tag_F5E8((*(Tag_F5E8*)((char*)D_F5E8 + 84)));
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
    MEM_copy_F5E8(buf, D_F5E8, 88);
    Ent_F5E8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004464D8[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_F5E8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_F5E8(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_F5E8;
    void *activeNode = *(void**)(obj + 0xC);
    if (activeNode && checkActiveNode(activeNode, 21, buf))
        return luno_gLunoNil_F5E8;
    *(float*)&buf[20] *= 0.0010000000474974513f;
    cSpringRailNode_cSpringRailNode_F5E8(cMemMan_alloc(0x80, D_00489778_F5E8, 0x20000000, 0), obj, buf);
    return luno_gLunoNil_F5E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AIEventFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_F850 { char c; Flag_F850() {} Flag_F850(const Flag_F850& o) : c(o.c) {} };
struct Ent_F850 { int idx; int val; int pad; int kind; };
struct R12_F850 { int a; int b; int c; };
extern R12_F850 luno_gLunoNil_F850 __asm__("luno_gLunoNil");

extern int D_00446530_F850[] __asm__("D_00446530");
extern "C" void cRider_flagEventWorld(int, int, float);
extern int* cWScriptMan_TheWScriptMan_F850 __asm__("cWScriptMan_TheWScriptMan");
extern char* SSXApp_F850 __asm__("SSXApp");
extern int D_004A563C_F850 __asm__("D_004A563C");
struct T12_F850 { int a; int b; int e; Flag_F850 f; };
extern T12_F850 D_004FBAE8_F850 __asm__("D_004FBAE8");
/**
 * Script function (gLunoCFunctions[27]): flags a world event on a rider (cRider_flagEventWorld).
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAIEventData: Being,
 * EventId, Arg) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_00446530 says 2. Defaults: Being -1, EventId 0, Arg 0.
 *
 * Being -1 means the script's rider (cWScriptMan_TheWScriptMan->Being). Calls
 * cRider_flagEventWorld(SSXApp->Game->AI->Riders[Being], EventId, Arg); the rider slot is not checked.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_F850 AIEventFunc(int n, Ent_F850* params) {
    if (D_004A563C_F850 == 0) {
        D_004FBAE8_F850.a = -1;
        D_004A563C_F850 = 1;
        D_004FBAE8_F850.b = 0;
        D_004FBAE8_F850.e = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBAE8_F850.f = Flag_F850(D_004FBAE8_F850.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBAE8_F850, 16);
    Ent_F850* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_00446530_F850[k] && D_00446530_F850[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    if (vp[0] == -1)
        vp[0] = *cWScriptMan_TheWScriptMan_F850;
    char* game = *(char**)(SSXApp_F850 + 0x84);
    char* base = *(char**)(game + 0xC);
    cRider_flagEventWorld(*(int*)(base + (vp[0] << 2) + 0x28), vp[1], *(float*)&vp[2]);
    return luno_gLunoNil_F850;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SignalFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_F9A8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_F9A8 __asm__("SSXApp");
static inline char *look_F9A8(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_F9A8 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

struct O_F9A8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(int, float); };
struct V3_F9A8 { float x, y, z; };
extern V3_F9A8 luno_gLunoNil_F9A8 __asm__("luno_gLunoNil");
struct Tag_F9A8 { char c; Tag_F9A8() {} Tag_F9A8(const Tag_F9A8 &o) : c(o.c) {} };
extern unsigned D_F9A8[] __asm__("D_004FBAF8");
extern int D_004A5640;
extern const int D_00446540[];
extern "C" void* MEM_copy_F9A8(void*, void*, int) __asm__("MEM_copy");
struct Ent_F9A8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[28]): sends a signal to a world instance's active node.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tSignalData: InstId,
 * Signal, Arg) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_00446540 says 2. Defaults: InstId -1, Signal -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (+0xC), its virtual signal
 * (cInstanceNode_signal, slot 36) gets Signal and Arg.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_F9A8 SignalFunc(int n, Ent_F9A8 *params) {
    if (D_004A5640 == 0) {
        D_F9A8[0] = 0xFFFFFFFF;
        ((int*)D_F9A8)[1] = -1;
        ((int*)D_F9A8)[2] = 0;
        D_004A5640 = 1;
        (*(Tag_F9A8*)((char*)D_F9A8 + 12)) = Tag_F9A8((*(Tag_F9A8*)((char*)D_F9A8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_F9A8(buf, D_F9A8, 16);
    Ent_F9A8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446540[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    char *obj = look_F9A8(buf[0]);
    O_F9A8 *activeNode = *(O_F9A8**)(obj + 0xC);
    if (activeNode)
        activeNode->v36(buf[1], *(float*)&buf[2]);
    return luno_gLunoNil_F9A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", HideInstanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_FB50 { char c; Flag_FB50() {} Flag_FB50(const Flag_FB50& o) : c(o.c) {} };
extern unsigned D_004A5648_FB50 __asm__("D_004A5648");
extern Flag_FB50 D_004A564C_FB50 __asm__("D_004A564C");
extern int D_004A5650_FB50 __asm__("D_004A5650");
extern int D_004A3C10_FB50[2] __asm__("D_004A3C10");
extern char *cWScriptMan_TheWScriptMan_FB50 __asm__("cWScriptMan_TheWScriptMan");
static inline char *refToPtr_FB50(unsigned i) { return (char*)(i << 2); }
static inline char *conv_FB50(unsigned i) { if (i == 0) return 0; return refToPtr_FB50(i); }
extern "C" void* cMemMan_alloc(int, void*, int, int);
extern "C" void cHideNode_cHideNode_FB50(void*, void*) __asm__("cHideNode_cHideNode");
extern char D_004A3C18_FB50[] __asm__("D_004A3C18");
struct O_FB50 { int p0, p1, p2; virtual void v01(int); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); };
struct V3_FB50 { float x, y, z; };
extern V3_FB50 luno_gLunoNil_FB50 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_FB50(void*, void*, int) __asm__("MEM_copy");
struct Ent_FB50 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[29]): hides a world instance by giving it a new Hide node.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tHideInstanceData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C10 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. Nothing happens when the instance is not found or its active node
 * (+0xC) is already ATYPE_HIDE (16) or ATYPE_DEAD (6) (virtual isType); any other node is deleted (virtual slot 1,
 * flags 3). Then a cHideNode (cHideNode_cHideNode, cMemMan_alloc 0x1C, "Hide") is built on the instance.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_FB50 HideInstanceFunc(int n, Ent_FB50 *params) {
    if (D_004A5650_FB50 == 0) {
        D_004A5648_FB50 = 0xFFFFFFFF;
        D_004A5650_FB50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A564C_FB50 = Flag_FB50(D_004A564C_FB50);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_FB50(buf, &D_004A5648_FB50, 8);
    Ent_FB50 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3C10_FB50[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_FB50 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_FB50(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj == 0)
        return luno_gLunoNil_FB50;
    O_FB50 *activeNode = *(O_FB50**)(obj + 0xC);
    if (activeNode) {
        if (activeNode->v10(16) || activeNode->v10(6))
            return luno_gLunoNil_FB50;
        activeNode->v01(3);
    }
    cHideNode_cHideNode_FB50(cMemMan_alloc(0x1C, D_004A3C18_FB50, 0x20000000, 0), obj);
    return luno_gLunoNil_FB50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ShowInstanceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_FD58 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_FD58 __asm__("SSXApp");
static inline char *look_FD58(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_FD58 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

struct O_FD58 { int p0, p1, p2; virtual void v01(int); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual int v10(int); };
struct V3_FD58 { float x, y, z; };
extern V3_FD58 luno_gLunoNil_FD58 __asm__("luno_gLunoNil");
struct Tag_FD58 { char c; Tag_FD58() {} Tag_FD58(const Tag_FD58 &o) : c(o.c) {} };
extern unsigned D_FD58 __asm__("D_004A5658");
extern Tag_FD58 D_004A565C;
extern int D_004A5660;
extern int D_004A3C20;
extern "C" void* MEM_copy_FD58(void*, void*, int) __asm__("MEM_copy");
struct Ent_FD58 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[58]): shows a hidden world instance again by deleting its Hide node.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tShowInstanceData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C20 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance exists and its active node (+0xC) is ATYPE_HIDE
 * (16, virtual isType), that node is deleted (virtual slot 1, flags 3).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_FD58 ShowInstanceFunc(int n, Ent_FD58 *params) {
    if (D_004A5660 == 0) {
        D_FD58 = 0xFFFFFFFF;
        D_004A5660 = 1;
        D_004A565C = Tag_FD58(D_004A565C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_FD58(buf, &D_FD58, 8);
    Ent_FD58 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C20)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    tInstanceRecord* obj = (tInstanceRecord *)look_FD58(buf[0]);
    if (obj) {
        O_FD58 *activeNode = (O_FD58 *)obj->m_pActiveNode;
        if (activeNode) {
            if (activeNode->v10(0x10))
                activeNode->v01(3);
        }
    }
    return luno_gLunoNil_FD58;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", PlayAudioFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_FF00 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_FF00 __asm__("SSXApp");
static inline char *refToPtr_FF00(unsigned i) { return (char*)(i << 2); }
static inline char *conv_FF00(unsigned i) { if (i == 0) return 0; return refToPtr_FF00(i); }
extern "C" void *cAudioInterface_bxAudio_FF00(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_WorldSound_FF00(void*, void*, int) __asm__("cSSXAudio_WorldSound");
struct V3_FF00 { float x, y, z; };
extern V3_FF00 luno_gLunoNil_FF00 __asm__("luno_gLunoNil");
struct Tag_FF00 { char c; Tag_FF00() {} Tag_FF00(const Tag_FF00 &o) : c(o.c) {} };
extern unsigned D_FF00[] __asm__("D_004FBB08");
extern int D_004A5664;
extern int D_004A3C28;
extern "C" void* MEM_copy_FF00(void*, void*, int) __asm__("MEM_copy");
struct Ent_FF00 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[30]): plays a world sound at a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tPlayAudioData: InstId,
 * SoundId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C28 says 2. Defaults: InstId -1, SoundId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_WorldSound(cAudioInterface_bxAudio(), instance, SoundId) plays it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_FF00 PlayAudioFunc(int n, Ent_FF00 *params) {
    if (D_004A5664 == 0) {
        D_FF00[0] = 0xFFFFFFFF;
        D_FF00[1] = 0xFFFFFFFF;
        D_004A5664 = 1;
        (*(Tag_FF00*)((char*)D_FF00 + 8)) = Tag_FF00((*(Tag_FF00*)((char*)D_FF00 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_FF00(buf, D_FF00, 12);
    Ent_FF00 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3C28)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_FF00 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_FF00(i);
            inst = t;
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
extern char *cWScriptMan_TheWScriptMan_00A8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_00A8 __asm__("SSXApp");
static inline char *refToPtr_00A8(unsigned i) { return (char*)(i << 2); }
static inline char *conv_00A8(unsigned i) { if (i == 0) return 0; return refToPtr_00A8(i); }
extern "C" void *cAudioInterface_bxAudio_00A8(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_AttachSound_00A8(void*, void*, int, int) __asm__("cSSXAudio_AttachSound");
struct V3_00A8 { float x, y, z; };
extern V3_00A8 luno_gLunoNil_00A8 __asm__("luno_gLunoNil");
struct Tag_00A8 { char c; Tag_00A8() {} Tag_00A8(const Tag_00A8 &o) : c(o.c) {} };
extern unsigned D_00A8[] __asm__("D_004FBB18");
extern int D_004A5668;
extern const int D_00446550[];
extern "C" void* MEM_copy_00A8(void*, void*, int) __asm__("MEM_copy");
struct Ent_00A8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[31]): attaches a looping sound to a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAttachAudioData:
 * InstId, SoundId, Mesh) and lets each argument override the field its index names, turning an int into a float where
 * the field type table D_00446550 says 2. Defaults: InstId -1, SoundId -1, Mesh -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_AttachSound(cAudioInterface_bxAudio(), instance, SoundId, Mesh) attaches it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_00A8 AttachAudioFunc(int n, Ent_00A8 *params) {
    if (D_004A5668 == 0) {
        D_00A8[0] = 0xFFFFFFFF;
        D_00A8[1] = 0xFFFFFFFF;
        ((int*)D_00A8)[2] = -1;
        D_004A5668 = 1;
        (*(Tag_00A8*)((char*)D_00A8 + 12)) = Tag_00A8((*(Tag_00A8*)((char*)D_00A8 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_00A8(buf, D_00A8, 16);
    Ent_00A8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446550[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_00A8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_00A8(i);
            inst = t;
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
extern char *cWScriptMan_TheWScriptMan_0260 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_0260 __asm__("SSXApp");
static inline char *refToPtr_0260(unsigned i) { return (char*)(i << 2); }
static inline char *conv_0260(unsigned i) { if (i == 0) return 0; return refToPtr_0260(i); }
extern "C" void *cAudioInterface_bxAudio_0260(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_DetachSound_0260(void*, void*, int, int) __asm__("cSSXAudio_DetachSound");
struct V3_0260 { float x, y, z; };
extern V3_0260 luno_gLunoNil_0260 __asm__("luno_gLunoNil");
struct Tag_0260 { char c; Tag_0260() {} Tag_0260(const Tag_0260 &o) : c(o.c) {} };
extern unsigned D_0260[] __asm__("D_004FBB28");
extern int D_004A566C;
extern const int D_00446560[];
extern "C" void* MEM_copy_0260(void*, void*, int) __asm__("MEM_copy");
struct Ent_0260 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[73]): detaches a sound from a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tDetachAudioData:
 * InstId, SoundId, Mesh) and lets each argument override the field its index names, turning an int into a float where
 * the field type table D_00446560 says 2. Defaults: InstId -1, SoundId -1, Mesh -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found,
 * cSSXAudio_DetachSound(cAudioInterface_bxAudio(), instance, SoundId, Mesh) detaches it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0260 DetachAudioFunc(int n, Ent_0260 *params) {
    if (D_004A566C == 0) {
        D_0260[0] = 0xFFFFFFFF;
        D_0260[1] = 0xFFFFFFFF;
        ((int*)D_0260)[2] = -1;
        D_004A566C = 1;
        (*(Tag_0260*)((char*)D_0260 + 12)) = Tag_0260((*(Tag_0260*)((char*)D_0260 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0260(buf, D_0260, 16);
    Ent_0260 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446560[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_0260 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_0260(i);
            inst = t;
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
extern char *cWScriptMan_TheWScriptMan_0418 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_0418 __asm__("SSXApp");
struct O_0418 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(int); };
struct V3_0418 { float x, y, z; };
extern V3_0418 luno_gLunoNil_0418 __asm__("luno_gLunoNil");
struct Tag_0418 { char c; Tag_0418() {} Tag_0418(const Tag_0418 &o) : c(o.c) {} };
extern unsigned D_0418[] __asm__("D_004FBB38");
extern int D_004A5670;
extern int D_004A3C30;
extern "C" void* MEM_copy_0418(void*, void*, int) __asm__("MEM_copy");
struct Ent_0418 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[32]): expires a world instance's active node.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tExpireData: InstId,
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C30 says 2. Defaults: InstId -1, Type 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (+0xC), Type 0 or 1 is
 * passed to its virtual expire (cInstanceNode_expire, slot 34); other types do nothing.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0418 ExpireFunc(int n, Ent_0418 *params) {
    if (D_004A5670 == 0) {
        D_0418[0] = 0xFFFFFFFF;
        ((int*)D_0418)[1] = 0;
        D_004A5670 = 1;
        (*(Tag_0418*)((char*)D_0418 + 8)) = Tag_0418((*(Tag_0418*)((char*)D_0418 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0418(buf, D_0418, 12);
    Ent_0418 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C30)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_0418 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_0418 *activeNode = *(O_0418**)(obj + 0xC);
    if (activeNode) {
        int expireType = buf[1];
        if (expireType == 0)
            activeNode->v34(0);
        else if (expireType == 1)
            activeNode->v34(1);
    }
    return luno_gLunoNil_0418;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", LoseFocusFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cWScriptMan_TheWScriptMan_05E8 __asm__("cWScriptMan_TheWScriptMan");
struct O_05E8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); };
struct V3_05E8 { float x, y, z; };
extern V3_05E8 luno_gLunoNil_05E8 __asm__("luno_gLunoNil");
struct Tag_05E8 { char c; Tag_05E8() {} Tag_05E8(const Tag_05E8 &o) : c(o.c) {} };
extern unsigned D_05E8 __asm__("D_004A5678");
extern Tag_05E8 D_004A567C;
extern int D_004A5680;
extern int D_004A3C38;
extern "C" void* MEM_copy_05E8(void*, void*, int) __asm__("MEM_copy");
struct Ent_05E8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[33]): tells a world instance's active node that it lost focus.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tLoseFocusData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C38 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance has an active node (+0xC), its virtual
 * loseFocus (cInstanceNode_loseFocus, slot 35) runs.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_05E8 LoseFocusFunc(int n, Ent_05E8 *params) {
    if (D_004A5680 == 0) {
        D_05E8 = 0xFFFFFFFF;
        D_004A5680 = 1;
        D_004A567C = Tag_05E8(D_004A567C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_05E8(buf, &D_05E8, 8);
    Ent_05E8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C38)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(*(char**)&cWScriptMan_TheWScriptMan_05E8 + 0x290);
    } else {
        char *tb = (char*)cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable;
        char *section = *(char**)(tb + (id & 0xFF) * 4);
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_05E8 *activeNode = *(O_05E8**)(obj + 0xC);
    if (activeNode)
        activeNode->v35();
    return luno_gLunoNil_05E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", TransportFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_0770 { char c; Flag_0770() {} Flag_0770(const Flag_0770& o) : c(o.c) {} };
extern unsigned D_004A5688_0770 __asm__("D_004A5688");
extern Flag_0770 D_004A568C_0770 __asm__("D_004A568C");
extern int D_004A5690_0770 __asm__("D_004A5690");
extern int D_004A3C40_0770[2] __asm__("D_004A3C40");
extern int *cWScriptMan_TheWScriptMan_0770 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_0770 __asm__("SSXApp");
struct O_0770 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void *v24(); };
struct Sh_0770 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(void*); };
struct Lvl_0770 { char pad[0x28]; char *sh[1]; };
static inline char *refToPtr_0770(unsigned i) { return (char*)(i << 2); }
static inline char *conv_0770(unsigned i) { if (i == 0) return 0; return refToPtr_0770(i); }
struct V3_0770 { float x, y, z; };
extern V3_0770 luno_gLunoNil_0770 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_0770(void*, void*, int) __asm__("MEM_copy");
struct Ent_0770 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[34]): transports the script's rider to a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tTransportData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C40 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the script's rider index (cWScriptMan_TheWScriptMan->Being)
 * is not negative and the instance is found, the target transform is the instance node's virtual getRootTransform (slot
 * 24) or, without a node, the instance's own transform (+0x10); the rider (SSXApp->Game->AI->Riders[Being]) gets it
 * through its virtual transport (cRider_transport, slot 10).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0770 TransportFunc(int n, Ent_0770 *params) {
    if (D_004A5690_0770 == 0) {
        D_004A5688_0770 = 0xFFFFFFFF;
        D_004A5690_0770 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A568C_0770 = Flag_0770(D_004A568C_0770);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0770(buf, &D_004A5688_0770, 8);
    Ent_0770 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3C40_0770[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)((char*)cWScriptMan_TheWScriptMan_0770 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_0770(i);
            inst = t;
        }
        obj = inst;
    }
    int k = *cWScriptMan_TheWScriptMan_0770;
    if (k >= 0 && obj) {
        O_0770 *activeNode = *(O_0770**)(obj + 0xC);
        void *rootTransform = obj + 0x10;
        if (activeNode)
            rootTransform = activeNode->v24();
        Lvl_0770 *lvl = *(Lvl_0770**)(*(char**)(SSXApp_0770 + 0x84) + 0xC);
        ((Sh_0770*)(lvl->sh[k] + 0x6C0))->v10(rootTransform);
    }
    return luno_gLunoNil_0770;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetupMissionRiderFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_0948 { char c; Flag_0948() {} Flag_0948(const Flag_0948& o) : c(o.c) {} };
extern unsigned D_004A5698_0948 __asm__("D_004A5698");
extern Flag_0948 D_004A569C_0948 __asm__("D_004A569C");
extern int D_004A56A0_0948 __asm__("D_004A56A0");
extern int D_004A3C48_0948[2] __asm__("D_004A3C48");
extern int *cWScriptMan_TheWScriptMan_0948 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_0948 __asm__("SSXApp");
struct O_0948 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void *v24(); };
struct Sh_0948 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(void*); };
struct Lvl_0948 { char pad[0x28]; char *sh[1]; };
static inline char *refToPtr_0948(unsigned i) { return (char*)(i << 2); }
static inline char *conv_0948(unsigned i) { if (i == 0) return 0; return refToPtr_0948(i); }
struct V3_0948 { float x, y, z; };
extern V3_0948 luno_gLunoNil_0948 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_0948(void*, void*, int) __asm__("MEM_copy");
struct Ent_0948 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[64]): sets the script's rider up for a mission at a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tSetupMissionRiderData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C48 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the script's rider index (cWScriptMan_TheWScriptMan->Being)
 * is not negative and the instance is found, the transform is the instance node's virtual getRootTransform (slot 24)
 * or, without a node, the instance's own transform (+0x10); the rider (SSXApp->Game->AI->Riders[Being]) gets it through
 * its virtual initForMission (cRider_initForMission, slot 12).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0948 SetupMissionRiderFunc(int n, Ent_0948 *params) {
    if (D_004A56A0_0948 == 0) {
        D_004A5698_0948 = 0xFFFFFFFF;
        D_004A56A0_0948 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A569C_0948 = Flag_0948(D_004A569C_0948);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_0948(buf, &D_004A5698_0948, 8);
    Ent_0948 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3C48_0948[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)((char*)cWScriptMan_TheWScriptMan_0948 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_0948(i);
            inst = t;
        }
        obj = inst;
    }
    int k = *cWScriptMan_TheWScriptMan_0948;
    if (k >= 0 && obj) {
        O_0948 *activeNode = *(O_0948**)(obj + 0xC);
        void *rootTransform = obj + 0x10;
        if (activeNode)
            rootTransform = activeNode->v24();
        Lvl_0948 *lvl = *(Lvl_0948**)(*(char**)(SSXApp_0948 + 0x84) + 0xC);
        ((Sh_0948*)(lvl->sh[k] + 0x6C0))->v12(rootTransform);
    }
    return luno_gLunoNil_0948;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_0B20 { char c; Flag_0B20() {} Flag_0B20(const Flag_0B20& o) : c(o.c) {} };
struct Ent_0B20 { int idx; int val; int pad; int kind; };
struct R12_0B20 { int a; int b; int c; };
extern R12_0B20 luno_gLunoNil_0B20 __asm__("luno_gLunoNil");

extern int D_004A3C50_0B20[2] __asm__("D_004A3C50");
extern "C" void cRailManNode_addRail(unsigned, int);
extern int D_004A56A4_0B20 __asm__("D_004A56A4");
struct T12_0B20 { unsigned a; int b; Flag_0B20 f; };
extern T12_0B20 D_004FBB48_0B20 __asm__("D_004FBB48");
/**
 * Script function (gLunoCFunctions[35]): switches a rail (spline) on or off.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tRailData: SplineId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C50 says 2. Defaults: SplineId -1, Cmd 1.
 *
 * When SplineId is not -1, Cmd 1 switches the rail on and Cmd 0 off through cRailManNode_addRail (the rail manager); other
 * values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_0B20 RailFunc(int n, Ent_0B20* params) {
    if (D_004A56A4_0B20 == 0) {
        D_004FBB48_0B20.a = -1;
        D_004FBB48_0B20.b = 1;
        D_004A56A4_0B20 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBB48_0B20.f = Flag_0B20(D_004FBB48_0B20.f);
    }
    unsigned block[4] __attribute__((aligned(16)));
    unsigned* vp = block;
    vp[0] = 0xFFFFFFFFu;
    MEM_copy(vp, &D_004FBB48_0B20, 12);
    Ent_0B20* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3C50_0B20[k] && D_004A3C50_0B20[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    if (~vp[0] != 0) {
        int y = vp[1];
        if (y == 1)
            cRailManNode_addRail(vp[0], 1);
        else if (y == 0)
            cRailManNode_addRail(vp[0], 0);
    }
    return luno_gLunoNil_0B20;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", NISFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory_0C78(int, int, int, int, int, int, int, int) __asm__("cSSXScriptEngine_AddToNISListByScriptCategory");
extern "C" void cSSXScriptEngine_AddToNISListByScriptHashID_0C78(int, int, int, int, int, int, int) __asm__("cSSXScriptEngine_AddToNISListByScriptHashID");
extern "C" void cSSXScriptEngine_ReadyNISList_0C78(int, int) __asm__("cSSXScriptEngine_ReadyNISList");
extern "C" void cSSXScriptEngine_PlayNISList_0C78(int, int, int, int) __asm__("cSSXScriptEngine_PlayNISList");
struct V3_0C78 { float x, y, z; };
extern V3_0C78 luno_gLunoNil_0C78 __asm__("luno_gLunoNil");
struct Tag_0C78 { char c; Tag_0C78() {} Tag_0C78(const Tag_0C78 &o) : c(o.c) {} };
extern unsigned D_0C78[] __asm__("D_004FBB58");
extern int D_004A56A8;
extern int D_004A3C58;
extern "C" void* MEM_copy_0C78(void*, void*, int) __asm__("MEM_copy");
struct Ent_0C78 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[36]): queues or plays an in-game cinematic (NIS) through the script engine.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tNISData: ScriptId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C58 says 2. Defaults: ScriptId -1.
 *
 * Cmd 0 adds the NIS whose script hash is ScriptId to list 0 (cSSXScriptEngine_AddToNISListByScriptHashID, mode 3) and
 * readies the list (cSSXScriptEngine_ReadyNISList); Cmd 1 adds the NIS of script category ScriptId instead
 * (cSSXScriptEngine_AddToNISListByScriptCategory) and readies it; Cmd 2 plays list 0 (cSSXScriptEngine_PlayNISList).
 * Other values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0C78 NISFunc(int n, Ent_0C78 *params) {
    if (D_004A56A8 == 0) {
        D_0C78[0] = 0xFFFFFFFF;
        ((int*)D_0C78)[1] = 0;
        D_004A56A8 = 1;
        (*(Tag_0C78*)((char*)D_0C78 + 8)) = Tag_0C78((*(Tag_0C78*)((char*)D_0C78 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    MEM_copy_0C78(buf, D_0C78, 12);
    Ent_0C78 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C58)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    switch ((int)buf[1]) {
    case 0:
        cSSXScriptEngine_AddToNISListByScriptHashID_0C78((int)ScriptEngine, 0, id, 3, -1, -1, -1);
        cSSXScriptEngine_ReadyNISList_0C78((int)ScriptEngine, 0);
        break;
    case 1:
        cSSXScriptEngine_AddToNISListByScriptCategory_0C78((int)ScriptEngine, 0, id, 3, 0, -1, -1, -1);
        cSSXScriptEngine_ReadyNISList_0C78((int)ScriptEngine, 0);
        break;
    case 2:
        cSSXScriptEngine_PlayNISList_0C78((int)ScriptEngine, 0, 1, 0);
        break;
    }
    return luno_gLunoNil_0C78;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectPoolFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
struct Ent_0E28 { int idx; int val; int pad; int kind; };
struct R12_0E28 { int a; int b; int c; };
extern R12_0E28 luno_gLunoNil_0E28 __asm__("luno_gLunoNil");
struct Flag_0E28 { char c; Flag_0E28() {} Flag_0E28(const Flag_0E28& o) : c(o.c) {} };

extern int D_004A3C60_0E28[2] __asm__("D_004A3C60");
extern "C" void cWScriptMan_collectPool(int, unsigned, int);
extern int cWScriptMan_TheWScriptMan_0E28 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A56AC_0E28 __asm__("D_004A56AC");
struct T12u_0E28 { unsigned a; int b; Flag_0E28 f; };
extern T12u_0E28 D_004FBB68_0E28 __asm__("D_004FBB68");
/**
 * Script function (gLunoCFunctions[37]): sets up a collection pool (cWScriptMan_collectPool).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCollectPoolData:
 * PoolId, PoolSize) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3C60 says 2. Defaults: PoolId -1, PoolSize 1.
 *
 * When PoolId is not -1, cWScriptMan_collectPool(cWScriptMan_TheWScriptMan, PoolId, PoolSize) runs.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_0E28 CollectPoolFunc(int n, Ent_0E28* params) {
    if (D_004A56AC_0E28 == 0) {
        D_004FBB68_0E28.a = 0xFFFFFFFFu;
        D_004FBB68_0E28.b = 1;
        D_004A56AC_0E28 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBB68_0E28.f = Flag_0E28(D_004FBB68_0E28.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBB68_0E28, 12);
    Ent_0E28* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3C60_0E28[k] && D_004A3C60_0E28[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu)
        cWScriptMan_collectPool(cWScriptMan_TheWScriptMan_0E28, x, vp[1]);
    return luno_gLunoNil_0E28;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectInstFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_0F50 __asm__("cWScriptMan_TheWScriptMan");
struct V3_0F50 { float x, y, z; };
extern V3_0F50 luno_gLunoNil_0F50 __asm__("luno_gLunoNil");
struct Tag_0F50 { char c; Tag_0F50() {} Tag_0F50(const Tag_0F50 &o) : c(o.c) {} };
extern unsigned D_0F50[] __asm__("D_004FBB78");
extern int D_56B0_0F50 __asm__("D_004A56B0");
extern const int D_00446570_0F50[] __asm__("D_00446570");
extern "C" void* MEM_copy_0F50(void*, void*, int) __asm__("MEM_copy");
extern "C" void cWScriptMan_collectInst_0F50(void*, unsigned, unsigned*, int) __asm__("cWScriptMan_collectInst");
struct Ent_0F50 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[38]): registers up to 20 instances with a collection pool (cWScriptMan_collectInst).
 *
 * Copies the static defaults (set up on the first call) into an 88-byte parameter block (On Tour tCollectInstData:
 * PoolId, InstId1, InstId2, InstId3, InstId4, InstId5, InstId6, InstId7, InstId8, InstId9, InstId10, InstId11,
 * InstId12, InstId13, InstId14, InstId15, InstId16, InstId17, InstId18, InstId19, InstId20) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_00446570 says 2.
 * Defaults: PoolId -1, InstId1 -1, InstId2 -1, InstId3 -1, InstId4 -1, InstId5 -1, InstId6 -1, InstId7 -1, InstId8 -1,
 * InstId9 -1, InstId10 -1, InstId11 -1, InstId12 -1, InstId13 -1, InstId14 -1, InstId15 -1, InstId16 -1, InstId17 -1,
 * InstId18 -1, InstId19 -1, InstId20 -1.
 *
 * When PoolId is not -1, cWScriptMan_collectInst(cWScriptMan_TheWScriptMan, PoolId, &InstId1, 20) runs.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_0F50 CollectInstFunc(int n, Ent_0F50 *params) {
    if (D_56B0_0F50 == 0) {
        D_0F50[0] = 0xFFFFFFFF;
        D_0F50[1] = 0xFFFFFFFF;
        D_0F50[2] = 0xFFFFFFFF;
        D_0F50[3] = 0xFFFFFFFF;
        D_0F50[4] = 0xFFFFFFFF;
        D_0F50[5] = 0xFFFFFFFF;
        D_0F50[6] = 0xFFFFFFFF;
        D_0F50[7] = 0xFFFFFFFF;
        D_0F50[8] = 0xFFFFFFFF;
        D_0F50[9] = 0xFFFFFFFF;
        D_0F50[10] = 0xFFFFFFFF;
        D_0F50[11] = 0xFFFFFFFF;
        D_0F50[12] = 0xFFFFFFFF;
        D_0F50[13] = 0xFFFFFFFF;
        D_0F50[14] = 0xFFFFFFFF;
        D_0F50[15] = 0xFFFFFFFF;
        D_0F50[16] = 0xFFFFFFFF;
        D_0F50[17] = 0xFFFFFFFF;
        D_0F50[18] = 0xFFFFFFFF;
        D_0F50[19] = 0xFFFFFFFF;
        D_0F50[20] = 0xFFFFFFFF;
        D_56B0_0F50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        (*(Tag_0F50*)((char*)D_0F50 + 0x54)) = Tag_0F50((*(Tag_0F50*)((char*)D_0F50 + 0x54)));
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
    MEM_copy_0F50(buf, D_0F50, 88);
    Ent_0F50 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446570_0F50[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    if (buf[0] != 0xFFFFFFFF)
        cWScriptMan_collectInst_0F50(cWScriptMan_TheWScriptMan_0F50, buf[0], buf + 1, 20);
    return luno_gLunoNil_0F50;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CollectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_1120 { char c; Flag_1120() {} Flag_1120(const Flag_1120& o) : c(o.c) {} };
struct Ent_1120 { int idx; int val; int pad; int kind; };
extern int D_004A3C68_1120[2] __asm__("D_004A3C68");
extern "C" int cWScriptMan_collect_1120(char*, int, unsigned, unsigned) __asm__("cWScriptMan_collect");
extern char *cWScriptMan_TheWScriptMan_1120 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_1120 __asm__("SSXApp");
extern int D_004A56B4_1120 __asm__("D_004A56B4");
struct T12_1120 { unsigned a; unsigned b; Flag_1120 f; };
extern T12_1120 D_004FBBD0_1120 __asm__("D_004FBBD0");
struct A_1120 { char pad[0x6C0]; };
struct B_1120 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual bool v09(); };
struct D_1120 : A_1120, B_1120 { };
struct Tab_1120 { char pad[0x28]; D_1120 *arr[1]; };
static inline bool isIdle_1120(int slot) {
    if (slot != -1) {
        D_1120 *r = (*(Tab_1120**)(*(char**)(SSXApp_1120 + 0x84) + 0xC))->arr[slot];
        B_1120 *b = r;
        D_1120 *d = (D_1120*)b;
        return !d->v09();
    }
    return false;
}
/**
 * Script command Collect: makes the script's rider collect an item; returns whether it did.
 *
 * Only acts when the script's rider slot (*cWScriptMan_TheWScriptMan) is set and that rider (the game's rider table
 * +0x28) is not busy (its virtual slot 9 returns false). The defaults (D_004FBBD0, under D_004A56B4: -1, -1) take the
 * keyword arguments (types from D_004A3C68). With an item set, an instance of -1 becomes the current instance's
 * resource ID (TheWScriptMan +0x290, +0x78) and cWScriptMan_collect(slot, item, instance) runs; the result is the int
 * 1, otherwise the int 0 (type at +0x8, value through luno_cLunoVar_castInt).
 *
 * @param self  the cLunoVar result to fill (g++'s hidden struct-return pointer)
 * @param n     the number of keyword arguments
 * @param params     the keyword arguments (index, value, type)
 * @return @p self
 */
extern "C" void* CollectFunc(void* self, int n, Ent_1120* params) {
    int slot = *(int*)cWScriptMan_TheWScriptMan_1120;
    if (isIdle_1120(slot)) {
        if (D_004A56B4_1120 == 0) {
            D_004FBBD0_1120.a = 0xFFFFFFFF;
            D_004FBBD0_1120.b = 0xFFFFFFFF;
            D_004A56B4_1120 = 1;
            // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back;
            //       sb 0(sp)).
            D_004FBBD0_1120.f = Flag_1120(D_004FBBD0_1120.f);
        }
        unsigned argBlock[4] __attribute__((aligned(16)));
        unsigned* vp = argBlock;
        argBlock[1] = 0xFFFFFFFF;
        MEM_copy(vp, &D_004FBBD0_1120, 12);
        Ent_1120* keyArg = params;
        for (int i = 0; i < n; i++, keyArg++) {
            int k = keyArg->idx;
            unsigned* field = (unsigned*)((k << 2) + (int)vp);
            if (keyArg->kind != D_004A3C68_1120[k] && D_004A3C68_1120[k] == 2)
                *(float*)field = (float)keyArg->val;
            else
                *field = keyArg->val;
        }
        unsigned x = vp[0];
        if (x != 0xFFFFFFFFu) {
            if (~vp[1] == 0)
                vp[1] = *(int*)(*(char**)(cWScriptMan_TheWScriptMan_1120 + 0x290) + 0x78);
            unsigned instanceId = vp[1];
            unsigned itemId = vp[0];
            cWScriptMan_collect_1120(cWScriptMan_TheWScriptMan_1120, slot, itemId, instanceId);
            *(int*)((char*)self + 8) = 1;
            *luno_cLunoVar_castInt(self) = 1;
            return self;
        }
    }
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MissionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_12F0 { char c; Flag_12F0() {} Flag_12F0(const Flag_12F0& o) : c(o.c) {} };
struct Ent_12F0 { int idx; int val; int pad; int kind; };
struct R12_12F0 { int a; int b; int c; };
extern R12_12F0 luno_gLunoNil_12F0 __asm__("luno_gLunoNil");

extern int D_004A3C70_12F0[2] __asm__("D_004A3C70");
extern "C" int cWScriptMan_doMissionCmd_12F0(int, unsigned, int, int) __asm__("cWScriptMan_doMissionCmd");
extern int cWScriptMan_TheWScriptMan_12F0 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A56B8_12F0 __asm__("D_004A56B8");
struct T12_12F0 { unsigned a; int b; Flag_12F0 f; };
extern T12_12F0 D_004FBBE0_12F0 __asm__("D_004FBBE0");
/**
 * Script function (gLunoCFunctions[40]): runs a mission command and returns its result (cWScriptMan_doMissionCmd).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMissionData:
 * MissionId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3C70 says 2. Defaults: MissionId -1, Cmd -1.
 *
 * When MissionId is not -1, the result of cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan, MissionId, Cmd, -1) is
 * returned as an int script value (type 1); otherwise nil. The value is built in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to an int script value, or nil
 */
extern "C" void* MissionFunc(void* self, int n, Ent_12F0* params) {
    if (D_004A56B8_12F0 == 0) {
        D_004FBBE0_12F0.a = -1;
        D_004FBBE0_12F0.b = -1;
        D_004A56B8_12F0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBBE0_12F0.f = Flag_12F0(D_004FBBE0_12F0.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBBE0_12F0, 12);
    Ent_12F0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3C70_12F0[k] && D_004A3C70_12F0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu) {
        int cmdResult = cWScriptMan_doMissionCmd_12F0(cWScriptMan_TheWScriptMan_12F0, x, vp[1], -1);
        *(int*)((char*)self + 8) = 1;
        *luno_cLunoVar_castInt(self) = cmdResult;
    } else {
        *(R12_12F0*)self = luno_gLunoNil_12F0;
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
        sRefSet_1440 *section = ((sRefSet_1440**)(cWorld_m_pTheWorld->m_pWorldCache)->m_pSectionCacheTable)[id & 0xFF];
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
struct Flag_1560 { char c; Flag_1560() {} Flag_1560(const Flag_1560& o) : c(o.c) {} };
struct Ent_1560 { int idx; int val; int pad; int kind; };
struct R12_1560 { int a; int b; int c; };
extern R12_1560 luno_gLunoNil_1560 __asm__("luno_gLunoNil");

extern int D_004A3C80_1560[2] __asm__("D_004A3C80");
extern "C" int cWScriptMan_collectCount(int, unsigned);
extern int cWScriptMan_TheWScriptMan_1560 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A56C8_1560 __asm__("D_004A56C8");
extern unsigned D_004A56C0_1560 __asm__("D_004A56C0");
extern Flag_1560 D_004A56C4_1560 __asm__("D_004A56C4");
/**
 * Script function (gLunoCFunctions[53]): returns how many items of a collection pool are collected
 * (cWScriptMan_collectCount).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tCollectCountData:
 * PoolId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3C80 says 2. Defaults: PoolId -1.
 *
 * Returns cWScriptMan_collectCount(cWScriptMan_TheWScriptMan, PoolId) as an int script value, or int 0 when PoolId is
 * -1. The value is built in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to an int script value
 */
extern "C" void* CollectCountFunc(void* self, int n, Ent_1560* params) {
    if (D_004A56C8_1560 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A56C4_1560 = Flag_1560(D_004A56C4_1560);
        D_004A56C0_1560 = -1;
        D_004A56C8_1560 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A56C0_1560, 8);
    Ent_1560* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3C80_1560[k] && D_004A3C80_1560[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    unsigned x = vp[0];
    if (x != 0xFFFFFFFFu) {
        int collected = cWScriptMan_collectCount(cWScriptMan_TheWScriptMan_1560, x);
        *(int*)((char*)self + 8) = 1;
        *luno_cLunoVar_castInt(self) = collected;
    } else {
        *(int*)((char*)self + 8) = 1;
        *luno_cLunoVar_castInt(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AnimFrameFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_1680 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_1680 __asm__("SSXApp");
static inline char *look_1680(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_1680 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

float *luno_cLunoVar_castFloat_1680(void*) __asm__("luno_cLunoVar_castFloat__FPv");
struct O_1680 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual float v30(); };
struct V3_1680 { int w0, w1, w2; };
struct Tag_1680 { char c; Tag_1680() {} Tag_1680(const Tag_1680 &o) : c(o.c) {} };
extern unsigned D_1680 __asm__("D_004A56D0");
extern Tag_1680 D_004A56D4;
extern int D_004A56D8;
extern int D_004A3C88;
extern "C" void* MEM_copy_1680(void*, void*, int) __asm__("MEM_copy");
struct Ent_1680 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[54]): returns a world instance's current animation frame as a float.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAnimFrameData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C88 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (+0xC) virtual getAnimFrame
 * (cInstanceNode_getAnimFrame, slot 30), or -1.0 when the instance or its node is missing; it is stored in @p ret as a
 * float script value (type 2).
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" V3_1680* AnimFrameFunc(V3_1680 *ret, int n, Ent_1680 *params) {
    if (D_004A56D8 == 0) {
        D_1680 = 0xFFFFFFFF;
        D_004A56D8 = 1;
        D_004A56D4 = Tag_1680(D_004A56D4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_1680(buf, &D_1680, 8);
    Ent_1680 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C88)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    char *obj = look_1680(buf[0]);
    float animFrame;
    if (obj) {
        O_1680 *activeNode = *(O_1680**)(obj + 0xC);
        if (activeNode)
            animFrame = activeNode->v30();
        else
            animFrame = -1.0f;
        ((int*)ret)[2] = 2;
        *luno_cLunoVar_castFloat_1680(ret) = animFrame;
    } else {
        animFrame = -1.0f;
        ((int*)ret)[2] = 2;
        *luno_cLunoVar_castFloat_1680(ret) = animFrame;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AnimDirFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_1830 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_1830 __asm__("SSXApp");
static inline char *look_1830(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_1830 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

int *luno_cLunoVar_castInt(void*);
struct O_1830 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual int v31(); };
struct V3_1830 { int w0, w1, w2; };
struct Tag_1830 { char c; Tag_1830() {} Tag_1830(const Tag_1830 &o) : c(o.c) {} };
extern unsigned D_1830 __asm__("D_004A56E0");
extern Tag_1830 D_004A56E4;
extern int D_004A56E8;
extern int D_004A3C90;
extern "C" void* MEM_copy_1830(void*, void*, int) __asm__("MEM_copy");
struct Ent_1830 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[109]): returns a world instance's animation direction as an int.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAnimDirData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3C90 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (+0xC) virtual getAnimDir
 * (cInstanceNode_getAnimDir, slot 31), or 0 when the instance or its node is missing; it is stored in @p ret as an int
 * script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" V3_1830* AnimDirFunc(V3_1830 *ret, int n, Ent_1830 *params) {
    if (D_004A56E8 == 0) {
        D_1830 = 0xFFFFFFFF;
        D_004A56E8 = 1;
        D_004A56E4 = Tag_1830(D_004A56E4);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_1830(buf, &D_1830, 8);
    Ent_1830 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C90)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_1830 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (obj) {
        O_1830 *activeNode = *(O_1830**)(obj + 0xC);
        int animDir;
        if (activeNode)
            animDir = activeNode->v31();
        else
            animDir = 0;
        ((int*)ret)[2] = 1;
        *luno_cLunoVar_castInt(ret) = animDir;
    } else {
        ((int*)ret)[2] = 1;
        *luno_cLunoVar_castInt(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", FrameUpdateEventFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_19C8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_19C8 __asm__("SSXApp");
int *luno_cLunoVar_castInt(void*);
struct O_19C8 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual int v32(float); };
struct V3_19C8 { int w0, w1, w2; };
struct Tag_19C8 { char c; Tag_19C8() {} Tag_19C8(const Tag_19C8 &o) : c(o.c) {} };
extern unsigned D_19C8[] __asm__("D_004FBC00");
extern int D_004A56EC;
extern int D_004A3C98;
extern "C" void* MEM_copy_19C8(void*, void*, int) __asm__("MEM_copy");
struct Ent_19C8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[55]): asks a world instance's active node whether its animation has passed a frame
 * (frameUpdateEvent).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tFrameUpdateEventData:
 * InstId, Frame) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3C98 says 2. Defaults: InstId -1, Frame -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The result is the active node's (+0xC) virtual frameUpdateEvent
 * (cInstanceNode_frameUpdateEvent, slot 32) called with Frame / 30 (frames turned into seconds), or 0 when the instance
 * or its node is missing; it is stored in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" V3_19C8* FrameUpdateEventFunc(V3_19C8 *ret, int n, Ent_19C8 *params) {
    if (D_004A56EC == 0) {
        D_19C8[0] = 0xFFFFFFFF;
        *(float*)&D_19C8[1] = -1.0f;
        D_004A56EC = 1;
        (*(Tag_19C8*)((char*)D_19C8 + 8)) = Tag_19C8((*(Tag_19C8*)((char*)D_19C8 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_19C8(buf, D_19C8, 12);
    Ent_19C8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3C98)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_19C8 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        O_19C8 *activeNode = *(O_19C8**)(obj + 0xC);
        float frameArg = *(float*)&buf[1];
        int eventHit;
        if (activeNode)
            eventHit = activeNode->v32(frameArg * 0.03333333507180214f);
        else
            eventHit = 0;
        ((int*)ret)[2] = 1;
        *luno_cLunoVar_castInt(ret) = eventHit;
    } else {
        ((int*)ret)[2] = 1;
        *luno_cLunoVar_castInt(ret) = 0;
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", GotoTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1B88 { char c; Flag_1B88() {} Flag_1B88(const Flag_1B88& o) : c(o.c) {} };
extern unsigned D_004A56F0_1B88 __asm__("D_004A56F0");
extern Flag_1B88 D_004A56F4_1B88 __asm__("D_004A56F4");
extern int D_004A56F8_1B88 __asm__("D_004A56F8");
extern int D_004A3CA0_1B88[2] __asm__("D_004A3CA0");
extern int cWScriptMan_TheWScriptMan_1B88 __asm__("cWScriptMan_TheWScriptMan");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void cWScriptMan_gotoTask(int, int);
int* luno_cLunoVar_castInt(void*);

struct Ent_1B88 { int idx; int val; int pad; int kind; };

/**
 * Script function (gLunoCFunctions[56]): makes the script's process jump to a task (cWScriptMan_gotoTask).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tGotoTaskData: TaskId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CA0 says 2. Defaults: TaskId -1.
 *
 * Calls cWScriptMan_gotoTask(cWScriptMan_TheWScriptMan, TaskId), even for the default -1, and returns int 0 in the
 * hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 0
 */
extern "C" void* GotoTaskFunc(void* self, int n, Ent_1B88* params) {
    if (D_004A56F8_1B88 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A56F4_1B88 = Flag_1B88(D_004A56F4_1B88);
        D_004A56F0_1B88 = -1;
        D_004A56F8_1B88 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A56F0_1B88, 8);
    Ent_1B88* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CA0_1B88[k] && D_004A3CA0_1B88[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_gotoTask(cWScriptMan_TheWScriptMan_1B88, vp[0]);
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AdvanceToTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1C80 { char c; Flag_1C80() {} Flag_1C80(const Flag_1C80& o) : c(o.c) {} };
extern unsigned D_004A5700_1C80 __asm__("D_004A5700");
extern Flag_1C80 D_004A5704_1C80 __asm__("D_004A5704");
extern int D_004A5708_1C80 __asm__("D_004A5708");
extern int D_004A3CA8_1C80[2] __asm__("D_004A3CA8");
extern int cWScriptMan_TheWScriptMan_1C80 __asm__("cWScriptMan_TheWScriptMan");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void cWScriptMan_advanceToTask(int, int);
int* luno_cLunoVar_castInt(void*);

struct Ent_1C80 { int idx; int val; int pad; int kind; };

/**
 * Script function (gLunoCFunctions[57]): advances the script's process to a task (cWScriptMan_advanceToTask).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tAdvanceToTaskData:
 * TaskId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3CA8 says 2. Defaults: TaskId -1.
 *
 * Calls cWScriptMan_advanceToTask(cWScriptMan_TheWScriptMan, TaskId), even for the default -1, and returns int 0 in the
 * hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 0
 */
extern "C" void* AdvanceToTaskFunc(void* self, int n, Ent_1C80* params) {
    if (D_004A5708_1C80 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5704_1C80 = Flag_1C80(D_004A5704_1C80);
        D_004A5700_1C80 = -1;
        D_004A5708_1C80 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5700_1C80, 8);
    Ent_1C80* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CA8_1C80[k] && D_004A3CA8_1C80[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_advanceToTask(cWScriptMan_TheWScriptMan_1C80, vp[0]);
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderInVolumeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1D78 { char c; Flag_1D78() {} Flag_1D78(const Flag_1D78& o) : c(o.c) {} };
extern unsigned D_004A5710_1D78 __asm__("D_004A5710");
extern Flag_1D78 D_004A5714_1D78 __asm__("D_004A5714");
extern int D_004A5718_1D78 __asm__("D_004A5718");
extern int D_004A3CB0_1D78[2] __asm__("D_004A3CB0");
extern int *cWScriptMan_TheWScriptMan_1D78 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_1D78 __asm__("SSXApp");
struct O_1D78 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void *v24(); };
struct Sh_1D78 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(void*); };
struct Lvl_1D78 { char pad[0x28]; char *sh[1]; };
static inline char *refToPtr_1D78(unsigned i) { return (char*)(i << 2); }
static inline char *conv_1D78(unsigned i) { if (i == 0) return 0; return refToPtr_1D78(i); }
struct V3_1D78 { float x, y, z; };
extern V3_1D78 luno_gLunoNil_1D78 __asm__("luno_gLunoNil");
extern "C" void* MEM_copy_1D78(void*, void*, int) __asm__("MEM_copy");
struct Chk_1D78 { virtual void v01(); virtual void v02(); virtual int v03(char*); };
int* luno_cLunoVar_castInt_1D78(void*) __asm__("luno_cLunoVar_castInt__FPv");
struct Ent_1D78 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[65]): returns whether the script's rider has the world instance in its list of
 * visited ids.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderInVolumeData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3CB0 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, the script's rider
 * (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) is asked through its virtual slot 3 (cRider_isIntersecting: is
 * the instance's id, +0x78, among the rider's 64 mVisitedIds); the int result in the hidden result slot self is 1 when
 * it is, else 0.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 1 or 0
 */
extern "C" void* RiderInVolumeFunc(void* self, int n, Ent_1D78 *params) {
    if (D_004A5718_1D78 == 0) {
        D_004A5710_1D78 = 0xFFFFFFFF;
        D_004A5718_1D78 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb 0(sp)).
        D_004A5714_1D78 = Flag_1D78(D_004A5714_1D78);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_1D78(buf, &D_004A5710_1D78, 8);
    Ent_1D78 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3CB0_1D78[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)((char*)cWScriptMan_TheWScriptMan_1D78 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *r;
        if (!section)
            r = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_1D78(i);
            r = t;
        }
        obj = r;
    }
    if (obj) {
        int found = 1;
        char *lvl = *(char**)(*(char**)(SSXApp_1D78 + 0x84) + 0xC);
        char *riderEntry = *(char**)(lvl + (*cWScriptMan_TheWScriptMan_1D78 << 2) + 0x28);
        char *riderPart = riderEntry ? riderEntry + 0x6C0 : 0;
        char *r = riderPart ? riderPart - 0x6C0 : 0;
        Chk_1D78 *riderIface = (Chk_1D78*)(r + 0x6C0);
        if (riderIface->v03(obj)) {
            *(int*)((char*)self + 8) = found;
            *luno_cLunoVar_castInt_1D78(self) = found;
            goto done;
        }
    }
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt_1D78(self) = 0;
done:
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetGamePathFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_1F48 { char c; Flag_1F48() {} Flag_1F48(const Flag_1F48& o) : c(o.c) {} };
extern int D_004A5720_1F48 __asm__("D_004A5720");
extern Flag_1F48 D_004A5724_1F48 __asm__("D_004A5724");
extern int D_004A5728_1F48 __asm__("D_004A5728");
extern int D_004A3CB8_1F48[2] __asm__("D_004A3CB8");
extern "C" void MEM_copy(void*, void*, int);
extern "C" void setGamePath_1F48(int) __asm__("setGamePath");

struct Ent_1F48 { int idx; int val; int pad; int kind; };
struct R12_1F48 { int a; int b; int c; };
extern R12_1F48 luno_gLunoNil_1F48 __asm__("luno_gLunoNil");

/**
 * Script function (gLunoCFunctions[66]): does nothing in effect: it hands Path to the empty stub setGamePath.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tSetGamePathData: Path)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CB8 says 2. Defaults: Path -1.
 *
 * Calls setGamePath(Path), which has an empty body in this build, so the game path is not changed.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_1F48 SetGamePathFunc(int n, Ent_1F48* params) {
    if (D_004A5728_1F48 == 0) {
        D_004A5720_1F48 = -1;
        D_004A5728_1F48 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5724_1F48 = Flag_1F48(D_004A5724_1F48);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5720_1F48, 8);
    Ent_1F48* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CB8_1F48[k] && D_004A3CB8_1F48[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    setGamePath_1F48(vp[0]);
    return luno_gLunoNil_1F48;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", SetGameModeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *cWScriptMan_TheWScriptMan_2048 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_2048 __asm__("SSXApp");
extern int D_00445E40_2048[] __asm__("D_00445E40");
extern "C" void cWScriptMan_failActiveLocalMission_2048(int*, int) __asm__("cWScriptMan_failActiveLocalMission");
extern "C" void cSectionMan_eventbegin_2048(int, int) __asm__("cSectionMan_eventbegin");
struct Sh_2048 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual int v09(); };
struct V3_2048 { float x, y, z; };
extern V3_2048 luno_gLunoNil_2048 __asm__("luno_gLunoNil");
struct Tag_2048 { char c; Tag_2048() {} Tag_2048(const Tag_2048 &o) : c(o.c) {} };
extern unsigned D_2048[] __asm__("D_004FBC10");
extern int D_004A572C;
extern int D_004A3CC0;
extern "C" void* MEM_copy_2048(void*, void*, int) __asm__("MEM_copy");
struct Ent_2048 { int idx; int val; int pad; int type; };
/**
 * Script command SetGameMode: switches the section manager to a game mode, failing the running mission first.
 *
 * Defaults (D_004FBC10, under D_004A572C: -1, -1) take the keyword arguments (types from D_004A3CC0). Only when the
 * script's rider is set and not busy (virtual slot 9): the mode index (< 8) selects the eSectionManMode in D_00445E40;
 * if the second value is 0, cWScriptMan_failActiveLocalMission runs unless that mode is 1, then
 * cSectionMan_eventbegin(SSXApp->Game->SectionMan, mode).
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" V3_2048 SetGameModeFunc(int n, Ent_2048 *params) {
    if (D_004A572C == 0) {
        ((int*)D_2048)[0] = -1;
        ((int*)D_2048)[1] = -1;
        D_004A572C = 1;
        (*(Tag_2048*)((char*)D_2048 + 8)) = Tag_2048((*(Tag_2048*)((char*)D_2048 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    MEM_copy_2048(buf, D_2048, 12);
    Ent_2048 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int argType = (&D_004A3CC0)[idx];
        unsigned *field = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != argType && argType == 2)
            *(float*)field = (float)x->val;
        else
            *field = x->val;
    }
    int notBusy;
    int k = *cWScriptMan_TheWScriptMan_2048;
    if (k != -1) {
        int slotOffset = k * 4;
        char *lvl = *(char**)(*(char**)(SSXApp_2048 + 0x84) + 0xC);
        char *tableRider = *(char**)(lvl + slotOffset + 0x28);
        char *riderPart = tableRider ? tableRider + 0x6C0 : 0;
        char *rider = riderPart ? riderPart - 0x6C0 : 0;
        Sh_2048 *sh = (Sh_2048*)(rider + 0x6C0);
        notBusy = sh->v09() ^ 1;
    } else {
        notBusy = 0;
    }
    if (notBusy) {
        int mode = 0;
        unsigned id = buf[0];
        int secondValue = buf[1];
        int w = *(int*)(*(char**)(SSXApp_2048 + 0x84) + 0x78);
        if (id < 8)
            mode = D_00445E40_2048[id];
        if (secondValue == 0) {
            if (mode != 1)
                cWScriptMan_failActiveLocalMission_2048(cWScriptMan_TheWScriptMan_2048, id);
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
extern "C" void* MEM_copy_2210(void*, void*, int) __asm__("MEM_copy");
struct Flag_2210 { char c; Flag_2210() {} Flag_2210(const Flag_2210& o) : c(o.c) {} };
struct Defaults_2210 { int v[3]; Flag_2210 flag; };
extern Defaults_2210 D_004FBC20_2210 __asm__("D_004FBC20");
extern int D_004A5730_2210 __asm__("D_004A5730");
extern const int D_004465C8_2210[] __asm__("D_004465C8");
extern char* const D_00445E40_2210[] __asm__("D_00445E40");
extern int* cWScriptMan_TheWScriptMan_2210 __asm__("cWScriptMan_TheWScriptMan");
extern char* SSXApp_2210 __asm__("SSXApp");
struct V3_2210 { float x, y, z; };
extern V3_2210 luno_gLunoNil_2210 __asm__("luno_gLunoNil");
extern "C" void* cBE_getBE_2210() __asm__("cBE_getBE");
extern "C" void* cBE_getInterface_2210(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationIDFromScript_2210(void* iface, int id) __asm__("cBENewRaceInterface_getLocationIDFromScript");
extern "C" void cSectionMan_load_2210(void* self, int hash, char* name) __asm__("cSectionMan_load");
extern "C" void cSectionMan_activate_2210(void* self, int hash, char* name) __asm__("cSectionMan_activate");
extern "C" void cGame_setState_2210(void* self, int a, int b, int refresh) __asm__("cGame_setState");
extern "C" void* cAudioInterface_bxAudio_2210() __asm__("cAudioInterface_bxAudio");
extern "C" void func_002A49E8_2210(void* self) __asm__("func_002A49E8");

struct Ent_2210 { int idx; int val; int pad; int kind; };

class cRider_2210 {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool isBusy();
};

/**
 * Script command Section: loads or activates a location's world sections, or switches the game state.
 *
 * Defaults (D_004FBC20, under D_004A5730: -1, -1, -1) take the keyword arguments (types from D_004465C8). Only when the
 * script's rider is set and not busy: the location comes from the script ID through cBENewRaceInterface_getLocationIDFromScript (BE interface 0) and
 * the mode from D_00445E40 (index < 8). The action: 0 cSectionMan_load, 2 cSectionMan_activate, 3
 * cGame_setState(game, 0xE, 2, 0) and func_002A49E8(cAudioInterface_bxAudio()), 4 cGame_setState(game, 0xE, 0, 0), 5 sets
 * SectionMan +0x1A4; 1 does nothing (load and activate act on SSXApp->Game->SectionMan).
 *
 * The view types D_00445E40 as strings (name) and the location as a hash, but the table is the eSectionManMode one that
 * SetGameModeFunc reads as ints, and the value is the location ID.
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" V3_2210 SectionFunc(int n, Ent_2210* params)
{
    if (D_004A5730_2210 == 0) {
        D_004FBC20_2210.v[0] = -1;
        D_004FBC20_2210.v[1] = -1;
        D_004FBC20_2210.v[2] = -1;
        D_004A5730_2210 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBC20_2210.flag = Flag_2210(D_004FBC20_2210.flag);
    }
    int argBlock[4] __attribute__((aligned(16)));
    int* vp = argBlock;
    MEM_copy_2210(vp, &D_004FBC20_2210, 0x10);
    Ent_2210* keyArg = params;
    for (int i = 0; i < n; i++, keyArg++) {
        int k = keyArg->idx;
        int* field = (int*)((k << 2) + (int)vp);
        if (keyArg->kind != D_004465C8_2210[k] && D_004465C8_2210[k] == 2)
            *(float*)field = (float)keyArg->val;
        else
            *field = keyArg->val;
    }
    int ok;
    int cur = *cWScriptMan_TheWScriptMan_2210;
    if (cur != -1) {
        char* lvl = *(char**)(*(char**)(SSXApp_2210 + 0x84) + 0xC);
        char* tableRider = *(char**)(lvl + (cur << 2) + 0x28);
        char* riderPart = tableRider ? tableRider + 0x6C0 : 0;
        char* rider = riderPart ? riderPart - 0x6C0 : 0;
        ok = !((cRider_2210*)(rider + 0x6C0))->isBusy();
    } else {
        ok = 0;
    }
    if (ok) {
        int sel = vp[2];
        int action = vp[1];
        int id = vp[0];
        char* name = 0;
        void* iface = cBE_getInterface_2210(cBE_getBE_2210(), 0);
        char* ui = *(char**)(*(char**)(SSXApp_2210 + 0x84) + 0x78);
        if ((unsigned int)sel < 8) {
            name = D_00445E40_2210[sel];
        }
        int hash = cBENewRaceInterface_getLocationIDFromScript_2210(iface, id);
        switch ((unsigned int)action) {
        case 0:
            cSectionMan_load_2210(ui, hash, name);
            break;
        case 1:
            break;
        case 2:
            cSectionMan_activate_2210(ui, hash, name);
            break;
        case 3:
            cGame_setState_2210(*(void**)(SSXApp_2210 + 0x84), 0xE, 2, 0);
            func_002A49E8_2210(cAudioInterface_bxAudio_2210());
            break;
        case 4:
            cGame_setState_2210(*(void**)(SSXApp_2210 + 0x84), 0xE, 0, 0);
            break;
        case 5:
            *(int*)(ui + 0x1A4) = 1;
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
extern char *cWScriptMan_TheWScriptMan_2490 __asm__("cWScriptMan_TheWScriptMan");
struct O_2490 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_2490 { float x, y, z; };
extern V3_2490 luno_gLunoNil_2490 __asm__("luno_gLunoNil");
struct Tag_2490 { char c; Tag_2490() {} Tag_2490(const Tag_2490 &o) : c(o.c) {} };
struct T12_2490 { unsigned a; int b; Tag_2490 f; };
extern T12_2490 D_4FBC30_2490 __asm__("D_004FBC30");
extern int D_5734_2490 __asm__("D_004A5734");
extern int D_3CC8_2490 __asm__("D_004A3CC8");
extern "C" void* MEM_copy_2490(void*, void*, int) __asm__("MEM_copy");
extern "C" void tModifierBlock_fadeEffectModifier_2490(char*) __asm__("tModifierBlock_fadeEffectModifier");
extern "C" void tModifierBlock_remHaloModifier_2490(char*) __asm__("tModifierBlock_remHaloModifier");
struct Ent_2490 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[69]): removes a modifier (effect fade or halo) from a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tRemoveModifierData:
 * InstId, Type) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3CC8 says 2. Defaults: InstId -1, Type -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (+0xC) is a move
 * node (virtual isMoveNode), Type 0 calls tModifierBlock_fadeEffectModifier and Type 1 tModifierBlock_remHaloModifier
 * on the node's modifier block (+0x1C) when it has one; other types do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_2490 RemoveModifierFunc(int n, Ent_2490 *params) {
    if (D_5734_2490 == 0) {
        D_4FBC30_2490.a = 0xFFFFFFFF;
        D_4FBC30_2490.b = -1;
        D_5734_2490 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_4FBC30_2490.f = Tag_2490(D_4FBC30_2490.f);
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_2490(buf, &D_4FBC30_2490, 12);
    Ent_2490 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_3CC8_2490)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_2490 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (obj) {
        O_2490 *activeNode = *(O_2490**)(obj + 0xC);
        int mode = buf[1];
        if (activeNode) {
            if (activeNode->v16()) {
                char *moveNode = *(char**)(obj + 0xC);
                switch (mode) {
                case 0: {
                    char *modBlock = *(char**)(moveNode + 0x1C);
                    if (modBlock)
                        tModifierBlock_fadeEffectModifier_2490(modBlock);
                    break;
                }
                case 1: {
                    char *modBlock = *(char**)(moveNode + 0x1C);
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
struct sBindArg_1FBE38 { int idx; int val; int pad; int type; };
struct sBindEnd_1FBE38 {};  // empty trailing member of a defaults record; value-init copies its byte through a stack
                            //       temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CD0_2680[2] __asm__("D_004A3CD0");
extern "C" int isGamePath(unsigned);
extern int D_004A5740_2680 __asm__("D_004A5740");
extern int D_004A5738_2680 __asm__("D_004A5738");
extern sBindEnd_1FBE38 D_004A573C_2680 __asm__("D_004A573C");
/**
 * Script function (gLunoCFunctions[42]): returns whether the game is on a given game path.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGamePathData: Path)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CD0 says 2. Defaults: Path -1.
 *
 * Stores isGamePath(Path) (1 when the race DB's cGamePath matches) in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param e  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGamePathFunc(cLunoValue* ret, int n, sBindArg_1FBE38* e) {
    if (D_004A5740_2680 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, sBindEnd_1FBE38 }).
        D_004A5738_2680 = -1;
        D_004A573C_2680 = sBindEnd_1FBE38();
        D_004A5740_2680 = 1;
    }
    int v[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, v, &D_004A5738_2680, 8, D_004A3CD0_2680, n, e);
    BIND_SET_RESULT_1FBE38(ret, isGamePath(vp[0]));
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsGameModeFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CD8_2778[2] __asm__("D_004A3CD8");
extern "C" int isGameMode(unsigned);
extern int D_004A5750_2778 __asm__("D_004A5750");
extern int D_004A5748_2778 __asm__("D_004A5748");
extern sBindEnd_1FBE38 D_004A574C_2778 __asm__("D_004A574C");
/**
 * Script function (gLunoCFunctions[43]): returns whether the game is in a given game mode.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGameModeData: Mode)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3CD8 says 2. Defaults: Mode -1.
 *
 * Stores isGameMode(Mode) (1 when the race DB's cGameMode matches) in @p ret as an int script value.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param e  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGameModeFunc(cLunoValue* ret, int n, sBindArg_1FBE38* e) {
    if (D_004A5750_2778 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, sBindEnd_1FBE38 }).
        D_004A5748_2778 = -1;
        D_004A574C_2778 = sBindEnd_1FBE38();
        D_004A5750_2778 = 1;
    }
    int v[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, v, &D_004A5748_2778, 8, D_004A3CD8_2778, n, e);
    BIND_SET_RESULT_1FBE38(ret, isGameMode(vp[0]));
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", IsGameEventFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A3CE0_2870[2] __asm__("D_004A3CE0");
extern "C" int isGameEvent(unsigned);
extern int D_004A5760_2870 __asm__("D_004A5760");
extern int D_004A5758_2870 __asm__("D_004A5758");
extern sBindEnd_1FBE38 D_004A575C_2870 __asm__("D_004A575C");
/**
 * Script function (gLunoCFunctions[108]): returns whether the game is running a given game event.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsGameEventData:
 * Event) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3CE0 says 2. Defaults: Event -1.
 *
 * Stores isGameEvent(Event) (1 when the race DB's cGameEvent matches) in @p ret as an int script value. It sits at
 * table index 108, after the On Tour-era entries.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param e  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsGameEventFunc(cLunoValue* ret, int n, sBindArg_1FBE38* e) {
    if (D_004A5760_2870 == 0) {
        // NOTE: guarded static-local init (defaults record = { int, sBindEnd_1FBE38 }).
        D_004A5758_2870 = -1;
        D_004A575C_2870 = sBindEnd_1FBE38();
        D_004A5760_2870 = 1;
    }
    int v[4];
    int* vp;
    BIND_UNPACK_ARGS_1FBE38(vp, v, &D_004A5758_2870, 8, D_004A3CE0_2870, n, e);
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
struct Flag_2968 { char c; Flag_2968() {} Flag_2968(const Flag_2968& o) : c(o.c) {} };
struct Ent_2968 { int idx; int val; int pad; int kind; };
struct R12_2968 { int a; int b; int c; };
extern R12_2968 luno_gLunoNil_2968 __asm__("luno_gLunoNil");

extern int D_004A3CE8_2968[2] __asm__("D_004A3CE8");
extern char* cWScriptMan_TheWScriptMan_2968 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A5770_2968 __asm__("D_004A5770");
extern unsigned D_004A5768_2968 __asm__("D_004A5768");
extern Flag_2968 D_004A576C_2968 __asm__("D_004A576C");
/**
 * Script function (gLunoCFunctions[44]): makes a world instance the script's current instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour
 * tSetCurrentInstanceData: InstId) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3CE8 says 2. Defaults: InstId -1.
 *
 * Sets cWScriptMan_TheWScriptMan->Inst to the instance InstId names (its low byte picks a world-cache section, the rest
 * an instance in it), or to 0 when it is not found. -1 is not special here: it looks in section 0xFF.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_2968 SetCurrentInstanceFunc(int n, Ent_2968* params) {
    if (D_004A5770_2968 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A576C_2968 = Flag_2968(D_004A576C_2968);
        D_004A5768_2968 = -1;
        D_004A5770_2968 = 1;
    }
    unsigned block[4] __attribute__((aligned(16)));
    unsigned* vp = block;
    vp[0] = 0xFFFFFFFFu;
    MEM_copy(vp, &D_004A5768_2968, 8);
    Ent_2968* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CE8_2968[k] && D_004A3CE8_2968[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    char* scriptMan = cWScriptMan_TheWScriptMan_2968;
    unsigned x = vp[0];
    char** tbl = (char**)cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable;
    char* section = tbl[x & 0xFF];
    unsigned w;
    int inst;
    if (section == 0 || (w = (*(unsigned**)(section + 0x1C))[x >> 8] >> 8) == 0)
        inst = 0;
    else
        inst = w << 2;
    *(int*)(scriptMan + 0x290) = inst;
    return luno_gLunoNil_2968;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ProcessFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_2AC0 { char c; Flag_2AC0() {} Flag_2AC0(const Flag_2AC0& o) : c(o.c) {} };
struct Ent_2AC0 { int idx; int val; int pad; int kind; };
struct R12_2AC0 { int a; int b; int c; };
extern R12_2AC0 luno_gLunoNil_2AC0 __asm__("luno_gLunoNil");

extern int D_004A3CF0_2AC0[2] __asm__("D_004A3CF0");
extern "C" void cWScriptMan_doProcessCmd(int, int, int);
extern int cWScriptMan_TheWScriptMan_2AC0 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A5774_2AC0 __asm__("D_004A5774");
struct T12_2AC0 { unsigned a; int b; Flag_2AC0 f; };
extern T12_2AC0 D_004FBC40_2AC0 __asm__("D_004FBC40");
/**
 * Script function (gLunoCFunctions[45]): sends a command to a script process (cWScriptMan_doProcessCmd).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tProcessData:
 * ProcessId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3CF0 says 2. Defaults: ProcessId -1, Cmd 0.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, Cmd): -1 is the current process; Cmd 1 starts
 * it, 0 stops it, 2 ends its task, 18 ends it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_2AC0 ProcessFunc(int n, Ent_2AC0* params) {
    if (D_004A5774_2AC0 == 0) {
        D_004FBC40_2AC0.a = -1;
        D_004FBC40_2AC0.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBC40_2AC0.f = Flag_2AC0(D_004FBC40_2AC0.f);
        D_004A5774_2AC0 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBC40_2AC0, 12);
    Ent_2AC0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CF0_2AC0[k] && D_004A3CF0_2AC0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan_2AC0, vp[0], vp[1]);
    return luno_gLunoNil_2AC0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndMissionFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_2BD8 { char c; Flag_2BD8() {} Flag_2BD8(const Flag_2BD8& o) : c(o.c) {} };
struct Ent_2BD8 { int idx; int val; int pad; int kind; };
struct R12_2BD8 { int a; int b; int c; };
extern R12_2BD8 luno_gLunoNil_2BD8 __asm__("luno_gLunoNil");

extern int D_004A3CF8_2BD8[2] __asm__("D_004A3CF8");
extern "C" void cWScriptMan_doMissionCmd(int, int, int, int);
extern int cWScriptMan_TheWScriptMan_2BD8 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A5780_2BD8 __asm__("D_004A5780");
extern unsigned D_004A5778_2BD8 __asm__("D_004A5778");
extern Flag_2BD8 D_004A577C_2BD8 __asm__("D_004A577C");
/**
 * Script function (gLunoCFunctions[51]): ends a mission (mission command 0).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndMissionData:
 * MissionId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3CF8 says 2. Defaults: MissionId -1.
 *
 * Calls cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan, MissionId, 0, -1); -1 is the current mission.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_2BD8 EndMissionFunc(int n, Ent_2BD8* params) {
    if (D_004A5780_2BD8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A577C_2BD8 = Flag_2BD8(D_004A577C_2BD8);
        D_004A5778_2BD8 = -1;
        D_004A5780_2BD8 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5778_2BD8, 8);
    Ent_2BD8* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3CF8_2BD8[k] && D_004A3CF8_2BD8[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_doMissionCmd(cWScriptMan_TheWScriptMan_2BD8, vp[0], 0, -1);
    return luno_gLunoNil_2BD8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndProcessFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_2CE8 { char c; Flag_2CE8() {} Flag_2CE8(const Flag_2CE8& o) : c(o.c) {} };
struct Ent_2CE8 { int idx; int val; int pad; int kind; };
struct R12_2CE8 { int a; int b; int c; };
extern R12_2CE8 luno_gLunoNil_2CE8 __asm__("luno_gLunoNil");

extern int D_004A3D00_2CE8[2] __asm__("D_004A3D00");
extern "C" void cWScriptMan_doProcessCmd(int, int, int);
extern int cWScriptMan_TheWScriptMan_2CE8 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A5790_2CE8 __asm__("D_004A5790");
extern unsigned D_004A5788_2CE8 __asm__("D_004A5788");
extern Flag_2CE8 D_004A578C_2CE8 __asm__("D_004A578C");
/**
 * Script function (gLunoCFunctions[46]): stops a script process (process command 0).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndProcessData:
 * ProcessId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D00 says 2. Defaults: ProcessId -1.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, 0); -1 is the current process.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_2CE8 EndProcessFunc(int n, Ent_2CE8* params) {
    if (D_004A5790_2CE8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A578C_2CE8 = Flag_2CE8(D_004A578C_2CE8);
        D_004A5788_2CE8 = -1;
        D_004A5790_2CE8 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5788_2CE8, 8);
    Ent_2CE8* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D00_2CE8[k] && D_004A3D00_2CE8[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan_2CE8, vp[0], 0);
    return luno_gLunoNil_2CE8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EndTaskFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_2DF0 { char c; Flag_2DF0() {} Flag_2DF0(const Flag_2DF0& o) : c(o.c) {} };
struct Ent_2DF0 { int idx; int val; int pad; int kind; };
struct R12_2DF0 { int a; int b; int c; };
extern R12_2DF0 luno_gLunoNil_2DF0 __asm__("luno_gLunoNil");

extern int D_004A3D08_2DF0[2] __asm__("D_004A3D08");
extern "C" void cWScriptMan_doProcessCmd(int, int, int);
extern int cWScriptMan_TheWScriptMan_2DF0 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A57A0_2DF0 __asm__("D_004A57A0");
extern unsigned D_004A5798_2DF0 __asm__("D_004A5798");
extern Flag_2DF0 D_004A579C_2DF0 __asm__("D_004A579C");
/**
 * Script function (gLunoCFunctions[47]): ends a script process's current task (process command 2).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tEndTaskData:
 * ProcessId) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D08 says 2. Defaults: ProcessId -1.
 *
 * Calls cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan, ProcessId, 2); -1 is the current process.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_2DF0 EndTaskFunc(int n, Ent_2DF0* params) {
    if (D_004A57A0_2DF0 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A579C_2DF0 = Flag_2DF0(D_004A579C_2DF0);
        D_004A5798_2DF0 = -1;
        D_004A57A0_2DF0 = 1;
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5798_2DF0, 8);
    Ent_2DF0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D08_2DF0[k] && D_004A3D08_2DF0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_doProcessCmd(cWScriptMan_TheWScriptMan_2DF0, vp[0], 2);
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
struct Flag_2EF8 { char c; Flag_2EF8() {} Flag_2EF8(const Flag_2EF8& o) : c(o.c) {} };
struct Ent_2EF8 { int idx; int val; int pad; int kind; };
extern int D_004A3D10_2EF8[2] __asm__("D_004A3D10");
extern char *cWScriptMan_TheWScriptMan_2EF8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_2EF8 __asm__("SSXApp");
extern int D_004A57B0_2EF8 __asm__("D_004A57B0");
extern int D_004A57A8_2EF8 __asm__("D_004A57A8");
extern Flag_2EF8 D_004A57AC_2EF8 __asm__("D_004A57AC");
struct A_2EF8 { char pad[0x6C0]; };
struct B_2EF8 { virtual void v01(); };
struct D_2EF8 : A_2EF8, B_2EF8 { };
struct Tab_2EF8 { char pad[0x28]; D_2EF8 *arr[1]; };
extern "C" int cRider_getMissionInt_2EF8(D_2EF8*, int) __asm__("cRider_getMissionInt");
/**
 * Script function (gLunoCFunctions[50]): answers a rider-action question about the script's rider
 * (cRider_getMissionInt).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderActionData: Type)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D10 says 2. Defaults: Type 0.
 *
 * Cmd/Type is mapped to a cRider_getMissionInt query (0 ground 3, 1 air 2, 2 railslide 4, 3 wipeout 0, 4 reset 1, 5-7
 * grab/tweak/uber 5-7, 8 query 17, 9 points 8, 10 and 11 PressMode 9 and 10, 12 boosting 11, 13-17 queries 12-16, 18
 * query 18); any other value gives 0. The answer for the script's rider
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
extern "C" void* RiderActionFunc(void* self, int n, Ent_2EF8* params) {
    if (D_004A57B0_2EF8 == 0) {
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57B0_2EF8 = 1;
        D_004A57A8_2EF8 = 0;
        D_004A57AC_2EF8 = Flag_2EF8(D_004A57AC_2EF8);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57A8_2EF8, 8);
    Ent_2EF8* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* d = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D10_2EF8[k] && D_004A3D10_2EF8[k] == 2)
            *(float*)d = (float)curArg->val;
        else
            *d = curArg->val;
    }
    D_2EF8* riderEntry = (*(Tab_2EF8**)(*(char**)(SSXApp_2EF8 + 0x84) + 0xC))->arr[*(int*)cWScriptMan_TheWScriptMan_2EF8];
    B_2EF8* riderIface = riderEntry;
    D_2EF8* d = (D_2EF8*)riderIface;
    int res;
    switch (vp[0]) {
    case 3: res = cRider_getMissionInt_2EF8(d, 0); break;
    case 4: res = cRider_getMissionInt_2EF8(d, 1); break;
    case 1: res = cRider_getMissionInt_2EF8(d, 2); break;
    case 0: res = cRider_getMissionInt_2EF8(d, 3); break;
    case 2: res = cRider_getMissionInt_2EF8(d, 4); break;
    case 5: res = cRider_getMissionInt_2EF8(d, 5); break;
    case 6: res = cRider_getMissionInt_2EF8(d, 6); break;
    case 7: res = cRider_getMissionInt_2EF8(d, 7); break;
    case 9: res = cRider_getMissionInt_2EF8(d, 8); break;
    case 10: res = cRider_getMissionInt_2EF8(d, 9); break;
    case 11: res = cRider_getMissionInt_2EF8(d, 10); break;
    case 12: res = cRider_getMissionInt_2EF8(d, 11); break;
    case 13: res = cRider_getMissionInt_2EF8(d, 12); break;
    case 14: res = cRider_getMissionInt_2EF8(d, 13); break;
    case 15: res = cRider_getMissionInt_2EF8(d, 14); break;
    case 16: res = cRider_getMissionInt_2EF8(d, 15); break;
    case 17: res = cRider_getMissionInt_2EF8(d, 16); break;
    case 8: res = cRider_getMissionInt_2EF8(d, 17); break;
    case 18: res = cRider_getMissionInt_2EF8(d, 18); break;
    default: res = 0; break;
    }
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", HasActiveObjectFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_3130 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_3130 __asm__("SSXApp");
static inline char *look_3130(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_3130 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

int *luno_cLunoVar_castInt(void*);
struct V3_3130 { int w0, w1, w2; };
struct Tag_3130 { char c; Tag_3130() {} Tag_3130(const Tag_3130 &o) : c(o.c) {} };
extern unsigned D_3130[] __asm__("D_004FBC50");
extern int D_004A57B4;
extern int D_004A3D18;
extern "C" void* MEM_copy_3130(void*, void*, int) __asm__("MEM_copy");
struct Ent_3130 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[52]): returns whether a world instance has an active node.
 *
 * Copies the static defaults (InstId -1, then 1; set up on the first call) into a 12-byte parameter block and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D18 says
 * 2. Only InstId is used.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The int result stored in @p ret is 1 when the instance exists and
 * has an active node (+0xC), else 0.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" V3_3130* HasActiveObjectFunc(V3_3130 *ret, int n, Ent_3130 *params) {
    if (D_004A57B4 == 0) {
        D_3130[0] = 0xFFFFFFFF;
        D_3130[1] = 1;
        D_004A57B4 = 1;
        (*(Tag_3130*)((char*)D_3130 + 8)) = Tag_3130((*(Tag_3130*)((char*)D_3130 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_3130(buf, D_3130, 12);
    Ent_3130 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3D18)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    char *obj = look_3130(buf[0]);
    if (obj && *(int*)(obj + 0xC)) {
        ((int*)ret)[2] = 1;
        *luno_cLunoVar_castInt(ret) = 1;
    } else {
        ((int*)ret)[2] = 1;
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
extern char *cWScriptMan_TheWScriptMan_32C0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_32C0 __asm__("SSXApp");
struct Vec_32C0 { float x, y, z, w; } __attribute__((aligned(16)));
struct A_32C0 { char pad[0x6C0]; };
struct B_32C0 { virtual void v01(); virtual Vec_32C0 *v02(); };
struct D_32C0 : A_32C0, B_32C0 { };
struct Tab_32C0 { char pad[0x28]; D_32C0 *arr[1]; };
static inline float len_32C0(const Vec_32C0 &v) {
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
 * script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) times the frame time (SSXApp + 0x14),
 * stored in the hidden result slot self.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @return self
 */
extern "C" void* RiderDistanceFunc(void* self) {
    D_32C0 *riderEntry = (*(Tab_32C0**)(*(char**)(SSXApp_32C0 + 0x84) + 0xC))->arr[*(int*)cWScriptMan_TheWScriptMan_32C0];
    B_32C0 *riderIface = riderEntry;
    D_32C0 *rider = (D_32C0*)riderIface;
    float len = len_32C0(*rider->v02());
    float res = len * *(float*)(SSXApp_32C0 + 0x14);
    *(int*)((char*)self + 8) = 2;
    *(float*)luno_cLunoVar_castFloat(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderSpeedFunc);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector length).
void* luno_cLunoVar_castFloat(void*);
extern char *cWScriptMan_TheWScriptMan_3380 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_3380 __asm__("SSXApp");
struct Vec_3380 { float x, y, z, w; } __attribute__((aligned(16)));
struct A_3380 { char pad[0x6C0]; };
struct B_3380 { virtual void v01(); virtual Vec_3380 *v02(); };
struct D_3380 : A_3380, B_3380 { };
struct Tab_3380 { char pad[0x28]; D_3380 *arr[1]; };
static inline float len_3380(const Vec_3380 &v) {
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
extern "C" void* RiderSpeedFunc(void* self) {
    D_3380 *riderEntry = (*(Tab_3380**)(*(char**)(SSXApp_3380 + 0x84) + 0xC))->arr[*(int*)cWScriptMan_TheWScriptMan_3380];
    B_3380 *riderIface = riderEntry;
    D_3380 *rider = (D_3380*)riderIface;
    float len = len_3380(*rider->v02());
    *(int*)((char*)self + 8) = 2;
    *(float*)luno_cLunoVar_castFloat(self) = len;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentInstanceFunc);
#ifdef SKIP_ASM
extern cWScriptMan* cWScriptMan_TheWScriptMan;
// A being: the script/AI-facing interface of a rider. Slot order of the prototype's vtable _vt$6cBeing; parameter
// types from its mangled names (applyBoost__6cBeingRC8t4Vector, transport__6cBeingRC8t4Matrix, ...).
extern "C" void* cParticleNode_cParticleNode1(void*, int, void*, void*, void*);
// Int result setter taking the value by const reference; the by-value macro form lets jump1 hoist the -1 arm.
static inline void setInt_3430(cLunoValue* r, const unsigned& v) {
    r->type = 1;
    *luno_cLunoVar_castInt(r) = v;
}
/**
 * Script function (gLunoCFunctions[61]): returns the id of the script's current instance.
 *
 * Takes no script arguments. The int result stored in @p ret is the resource id (+0x78) of
 * cWScriptMan_TheWScriptMan->Inst, or -1 when there is none.
 *
 * @param ret  the result slot
 * @return @p ret
 */
extern "C" cLunoValue* CurrentInstanceFunc(cLunoValue* ret) {
    tInstanceRecord* curInst = (tInstanceRecord *)((char*)cWScriptMan_TheWScriptMan->Inst);
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
extern char *cWScriptMan_TheWScriptMan_3490 __asm__("cWScriptMan_TheWScriptMan");
extern "C" unsigned cWScriptProcess_getRunningTaskId_3490(char*) __asm__("cWScriptProcess_getRunningTaskId");
/**
 * Script function (gLunoCFunctions[62]): returns the id of the running task of the current script process.
 *
 * Takes no script arguments. The result is cWScriptProcess_getRunningTaskId on
 * cWScriptMan_TheWScriptMan->CurrentProcess, or -1 without a process, stored as an index script value (type 3) in the
 * hidden result slot self.
 *
 * @return self
 */
extern "C" void* CurrentTaskFunc(void* self) {
    char *curProcess = *(char**)(cWScriptMan_TheWScriptMan_3490 + 0x2A4);
    unsigned taskId;
    if (curProcess)
        taskId = cWScriptProcess_getRunningTaskId_3490(curProcess);
    else
        taskId = 0xFFFFFFFF;
    *(int*)((char*)self + 8) = 3;
    *luno_cLunoVar_castIndex_3490(self) = taskId;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentRailFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int* luno_cLunoVar_castInt(void*);
extern char *cWScriptMan_TheWScriptMan_34F8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_34F8 __asm__("SSXApp");
struct A_34F8 { char pad[0x6C0]; };
struct B_34F8 { virtual void v01(); };
struct D_34F8 : A_34F8, B_34F8 { };
struct Tab_34F8 { char pad[0x28]; D_34F8 *arr[1]; };
extern "C" int cRider_getMissionInt_34F8(D_34F8*, int) __asm__("cRider_getMissionInt");
/**
 * Script function (gLunoCFunctions[63]): returns the rail the script's rider is grinding.
 *
 * Takes no script arguments. When the script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being])
 * railslides (cRider_getMissionInt query 4), the int result is the rail spline id of its rider handler (mRiderHandler,
 * SplineID at +0xD4); otherwise -1. It is stored in the hidden result slot self.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @return self
 */
extern "C" void* CurrentRailFunc(void* self) {
    D_34F8 *riderEntry = (*(Tab_34F8**)(*(char**)(SSXApp_34F8 + 0x84) + 0xC))->arr[*(int*)cWScriptMan_TheWScriptMan_34F8];
    B_34F8 *riderIface = riderEntry;
    D_34F8 *rider = (D_34F8*)riderIface;
    unsigned splineId;
    if (cRider_getMissionInt_34F8(rider, 4))
        splineId = *(unsigned*)(*(char**)((char*)rider + 0x77C) + 0xD4);
    else
        splineId = 0xFFFFFFFF;
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = splineId;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AIRandomFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_3598 { char c; Flag_3598() {} Flag_3598(const Flag_3598& o) : c(o.c) {} };
struct Ent_3598 { int idx; int val; int pad; int kind; };
struct R12_3598 { int a; int b; int c; };
extern R12_3598 luno_gLunoNil_3598 __asm__("luno_gLunoNil");

extern int D_004A3D20_3598[2] __asm__("D_004A3D20");
float AIrandf(float, float);
void* luno_cLunoVar_castFloat(void*);
extern int D_004A57B8_3598 __asm__("D_004A57B8");
struct T12_3598 { float a; float b; Flag_3598 f; };
extern T12_3598 D_004FBC60_3598 __asm__("D_004FBC60");
/**
 * Script function (gLunoCFunctions[77]): returns a random float between two bounds (AIrandf).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAIRandomData: Lo, Hi)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D20 says 2. Defaults: Lo 0, Hi 1.
 *
 * Returns AIrandf(Lo, Hi) as a float script value in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the random float
 */
extern "C" void* AIRandomFunc(void* self, int n, Ent_3598* params) {
    if (D_004A57B8_3598 == 0) {
        D_004FBC60_3598.a = 0;
        D_004FBC60_3598.b = 1.0f;
        D_004A57B8_3598 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBC60_3598.f = Flag_3598(D_004FBC60_3598.f);
    }
    float block[4] __attribute__((aligned(16)));
    float* vp = block;
    MEM_copy(vp, &D_004FBC60_3598, 12);
    Ent_3598* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D20_3598[k] && D_004A3D20_3598[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    float randValue = AIrandf(vp[0], vp[1]);
    *(int*)((char*)self + 8) = 2;
    *(float*)luno_cLunoVar_castFloat(self) = randValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CurrentTrickFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Flag_36B0 { char c; Flag_36B0() {} Flag_36B0(const Flag_36B0& o) : c(o.c) {} };
extern int D_004A57C0_36B0 __asm__("D_004A57C0");
extern Flag_36B0 D_004A57C4_36B0 __asm__("D_004A57C4");
extern int D_004A57C8_36B0 __asm__("D_004A57C8");
extern int D_004A3D28_36B0[2] __asm__("D_004A3D28");
extern "C" void MEM_copy(void*, void*, int);
extern "C" int cWScriptMan_getCurrentTrick(int*, int, int);
extern int* cWScriptMan_TheWScriptMan_36B0 __asm__("cWScriptMan_TheWScriptMan");
int* luno_cLunoVar_castInt(void*);

struct Ent_36B0 { int idx; int val; int pad; int kind; };

/**
 * Script function (gLunoCFunctions[78]): returns a field of the current trick definition (cWScriptMan_getCurrentTrick).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tCurrentTrickData: Cmd)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D28 says 2. Defaults: Cmd -1.
 *
 * Returns cWScriptMan_getCurrentTrick(cWScriptMan_TheWScriptMan, Being, Cmd), field Cmd of cWScriptMan::TrickDef (0 for
 * the default -1), as an int in the hidden result slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the field value
 */
extern "C" void* CurrentTrickFunc(void* self, int n, Ent_36B0* params) {
    if (D_004A57C8_36B0 == 0) {
        D_004A57C0_36B0 = -1;
        D_004A57C8_36B0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57C4_36B0 = Flag_36B0(D_004A57C4_36B0);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57C0_36B0, 8);
    Ent_36B0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D28_36B0[k] && D_004A3D28_36B0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    int trickField = cWScriptMan_getCurrentTrick(cWScriptMan_TheWScriptMan_36B0, *cWScriptMan_TheWScriptMan_36B0, vp[0]);
    *(int*)((char*)self + 8) = 1;
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
struct Flag_37B0 { char c; Flag_37B0() {} Flag_37B0(const Flag_37B0& o) : c(o.c) {} };
extern int D_004A57D0_37B0 __asm__("D_004A57D0");
extern Flag_37B0 D_004A57D4_37B0 __asm__("D_004A57D4");
extern int D_004A57D8_37B0 __asm__("D_004A57D8");
extern int D_004A3D30_37B0[2] __asm__("D_004A3D30");
extern int* cWScriptMan_TheWScriptMan_37B0 __asm__("cWScriptMan_TheWScriptMan");
extern char* SSXApp_37B0 __asm__("SSXApp");
extern "C" int cRider_getMissionInt_37B0(char* rider, int which) __asm__("cRider_getMissionInt");

struct Ent_37B0 { int idx; int val; int pad; int kind; };

/**
 * Script function (gLunoCFunctions[79]): answers a rider-metric question about the script's rider
 * (cRider_getMissionInt).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tRiderMetricData: Cmd)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D30 says 2. Defaults: Cmd -1.
 *
 * Cmd/Type is mapped to a cRider_getMissionInt query (0 ground 3, 1 air 2, 2 railslide 4, 3 wipeout 0, 4 reset 1, 5-7
 * grab/tweak/uber 5-7, 8 query 17, 9 points 8, 10 and 11 PressMode 9 and 10, 12 boosting 11, 13-17 queries 12-16, 18
 * query 18); any other value gives 0. The answer for the script's rider
 * (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) is returned as an int in the hidden result slot self.
 * Same body as RiderActionFunc.
 *
 * The rider slot is not checked for -1 or an empty entry.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the int answer
 */
extern "C" void* RiderMetricFunc(void* self, int n, Ent_37B0* params)
{
    if (D_004A57D8_37B0 == 0) {
        D_004A57D0_37B0 = -1;
        D_004A57D8_37B0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A57D4_37B0 = Flag_37B0(D_004A57D4_37B0);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A57D0_37B0, 8);
    Ent_37B0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D30_37B0[k] && D_004A3D30_37B0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    char* lvl = *(char**)(*(char**)(SSXApp_37B0 + 0x84) + 0xC);
    char* riderEntry = *(char**)(lvl + (*cWScriptMan_TheWScriptMan_37B0 << 2) + 0x28);
    char* riderPart = riderEntry ? riderEntry + 0x6C0 : 0;
    char* rider = riderPart ? riderPart - 0x6C0 : 0;
    int res;
    switch (vp[0]) {
    case 3: res = cRider_getMissionInt_37B0(rider, 0); break;
    case 4: res = cRider_getMissionInt_37B0(rider, 1); break;
    case 1: res = cRider_getMissionInt_37B0(rider, 2); break;
    case 0: res = cRider_getMissionInt_37B0(rider, 3); break;
    case 2: res = cRider_getMissionInt_37B0(rider, 4); break;
    case 5: res = cRider_getMissionInt_37B0(rider, 5); break;
    case 6: res = cRider_getMissionInt_37B0(rider, 6); break;
    case 7: res = cRider_getMissionInt_37B0(rider, 7); break;
    case 9: res = cRider_getMissionInt_37B0(rider, 8); break;
    case 10: res = cRider_getMissionInt_37B0(rider, 9); break;
    case 11: res = cRider_getMissionInt_37B0(rider, 10); break;
    case 12: res = cRider_getMissionInt_37B0(rider, 11); break;
    case 13: res = cRider_getMissionInt_37B0(rider, 12); break;
    case 14: res = cRider_getMissionInt_37B0(rider, 13); break;
    case 15: res = cRider_getMissionInt_37B0(rider, 14); break;
    case 16: res = cRider_getMissionInt_37B0(rider, 15); break;
    case 17: res = cRider_getMissionInt_37B0(rider, 16); break;
    case 8: res = cRider_getMissionInt_37B0(rider, 17); break;
    case 18: res = cRider_getMissionInt_37B0(rider, 18); break;
    default: res = 0; break;
    }
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = res;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", Crowd3dFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_39F0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_39F0 __asm__("SSXApp");
extern int TheCrowdMan3d_39F0 __asm__("TheCrowdMan3d");
extern "C" void func_00343C60_39F0(int, int) __asm__("func_00343C60");
extern "C" void func_00343F38_39F0(int, int) __asm__("func_00343F38");
struct V3_39F0 { float x, y, z; };
extern V3_39F0 luno_gLunoNil_39F0 __asm__("luno_gLunoNil");
struct Tag_39F0 { char c; Tag_39F0() {} Tag_39F0(const Tag_39F0 &o) : c(o.c) {} };
extern unsigned D_39F0[] __asm__("D_004FBC70");
extern int D_004A57DC;
extern int D_004A3D38;
extern "C" void* MEM_copy_39F0(void*, void*, int) __asm__("MEM_copy");
struct Ent_39F0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[80]): adds a world instance to, or removes it from, its 3D crowd.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCrowd3dData: InstId,
 * Cmd) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D38 says 2. Defaults: InstId -1, Cmd -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, Cmd 0 passes its id (+0x78) to
 * func_00343C60 (joins the crowd block for its model) and Cmd 1 to func_00343F38 (leaves it), both on the 3D crowd
 * manager TheCrowdMan3d; other values do nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_39F0 Crowd3dFunc(int n, Ent_39F0 *params) {
    if (D_004A57DC == 0) {
        D_39F0[0] = 0xFFFFFFFF;
        ((int*)D_39F0)[1] = -1;
        D_004A57DC = 1;
        (*(Tag_39F0*)((char*)D_39F0 + 8)) = Tag_39F0((*(Tag_39F0*)((char*)D_39F0 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_39F0(buf, D_39F0, 12);
    Ent_39F0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3D38)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_39F0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        int crowdCmd = buf[1];
        int w = *(int*)(obj + 0x78);
        if (crowdCmd == 0)
            func_00343C60_39F0(TheCrowdMan3d_39F0, w);
        else if (crowdCmd == 1)
            func_00343F38_39F0(TheCrowdMan3d_39F0, w);
    }
    return luno_gLunoNil_39F0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", Crowd2dFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_3BA0 __asm__("cWScriptMan_TheWScriptMan");
extern int TheCrowdMan2d_3BA0 __asm__("TheCrowdMan2d");
struct V3_3BA0 { float x, y, z; };
extern V3_3BA0 luno_gLunoNil_3BA0 __asm__("luno_gLunoNil");
struct Tag_3BA0 { char c; Tag_3BA0() {} Tag_3BA0(const Tag_3BA0 &o) : c(o.c) {} };
struct T20_3BA0 { unsigned a; int b; int c; int d; Tag_3BA0 f; };
extern T20_3BA0 D_4FBC80_3BA0 __asm__("D_004FBC80");
extern int D_57E0_3BA0 __asm__("D_004A57E0");
extern const int D_004465D8_3BA0[] __asm__("D_004465D8");
extern "C" void* MEM_copy_3BA0(void*, void*, int) __asm__("MEM_copy");
extern "C" void func_00229820_3BA0(int, unsigned, int, int) __asm__("func_00229820");
extern "C" void func_00229788_3BA0(int, unsigned, int, int) __asm__("func_00229788");
extern "C" void func_002297D8_3BA0(int, unsigned, int, int) __asm__("func_002297D8");
struct Ent_3BA0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[88]): runs a 2D-crowd command on a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 20-byte parameter block (On Tour tCrowd2dData: InstId,
 * Cmd, Mesh, Context) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004465D8 says 2. Defaults: InstId -1, Cmd -1, Mesh 0, Context 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, its id (+0x78) goes to the 2D crowd
 * manager TheCrowdMan2d: Cmd 0 calls func_00229820 (points bone Context of part Mesh at the shared crowd record), Cmd 2
 * func_00229788 (takes a crowd quad slot for the instance) and Cmd 3 func_002297D8 (releases it); Cmd 1 and others do
 * nothing.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_3BA0 Crowd2dFunc(int n, Ent_3BA0 *params) {
    if (D_57E0_3BA0 == 0) {
        D_4FBC80_3BA0.a = 0xFFFFFFFF;
        D_4FBC80_3BA0.b = -1;
        D_4FBC80_3BA0.c = 0;
        D_4FBC80_3BA0.d = 0;
        D_57E0_3BA0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_4FBC80_3BA0.f = Tag_3BA0(D_4FBC80_3BA0.f);
    }
    unsigned buf64[5];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_3BA0(buf, &D_4FBC80_3BA0, 20);
    Ent_3BA0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004465D8_3BA0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_3BA0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        int mode = buf[1];
        int meshIndex = buf[2];
        int contextIndex = buf[3];
        unsigned instId = *(unsigned*)(obj + 0x78);
        if (mode == 0)
            func_00229820_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
        else if (mode == 1) {
        } else if (mode == 2)
            func_00229788_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
        else if (mode == 3)
            func_002297D8_3BA0(TheCrowdMan2d_3BA0, instId, meshIndex, contextIndex);
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
void* cBE_getInterface_Fv_3D90(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int D_004465E8_3D90[] __asm__("D_004465E8");
/**
 * Returns whether script game path @p i is the current game path.
 *
 * For @p i below 4 it maps @p i through D_004465E8 (0, 1, 2, 0) and compares it with the race DB's cGamePath
 * (newRace.cGamePath); 4 and above give 0. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game path (0-3)
 * @return 1 when it matches, else 0
 */
extern "C" int isGamePath(unsigned i) {
    if (i < 4) {
        cBE_getInterface_Fv_3D90(cBE_getBE_3D90(), 0);
        return D_004465E8_3D90[i] == cBENewRaceInterface_newRace.cGamePath;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", isGameMode);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_3DF8(void) __asm__("cBE_getBE");
void* cBE_getInterface_Fv_3DF8(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int D_004465F8_3DF8[] __asm__("D_004465F8");
/**
 * Returns whether script game mode @p i is the current game mode.
 *
 * For @p i below 8 it maps @p i through D_004465F8 (0-6, then 0) and compares it with the race DB's cGameMode
 * (newRace.cGameMode); 8 and above give 0. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game mode (0-7)
 * @return 1 when it matches, else 0
 */
extern "C" int isGameMode(unsigned i) {
    if (i < 8) {
        cBE_getInterface_Fv_3DF8(cBE_getBE_3DF8(), 0);
        return D_004465F8_3DF8[i] == cBENewRaceInterface_newRace.cGameMode;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", isGameEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" int cBE_getBE_3E60(void) __asm__("cBE_getBE");
void* cBE_getInterface_Fv_3E60(int be, int kind) __asm__("cBE_getInterface__Fv");
extern const int D_00446618_3E60[] __asm__("D_00446618");
/**
 * Returns whether script game event @p i is the current game event.
 *
 * For @p i below 15 it maps @p i through D_00446618 (0-13, then 0) and compares it with the race DB's cGameEvent
 * (newRace.cGameEvent); 15 and above give 0. The cBE_getInterface call before it has no effect on the result.
 *
 * @param i  the script game event (0-14)
 * @return 1 when it matches, else 0
 */
extern "C" int isGameEvent(unsigned i) {
    if (i < 0xF) {
        cBE_getInterface_Fv_3E60(cBE_getBE_3E60(), 0);
        return D_00446618_3E60[i] == cBENewRaceInterface_newRace.cGameEvent;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderIsHumanFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int* luno_cLunoVar_castInt(void*);
extern char *cWScriptMan_TheWScriptMan_3EC8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_3EC8 __asm__("SSXApp");
struct A_3EC8 { char pad[0x6C0]; };
struct B_3EC8 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual bool v09(); };
struct D_3EC8 : A_3EC8, B_3EC8 { };
struct Tab_3EC8 { char pad[0x28]; D_3EC8 *arr[1]; };
static inline bool isIdle_3EC8(int slot) {
    if (slot != -1) {
        D_3EC8 *r = (*(Tab_3EC8**)(*(char**)(SSXApp_3EC8 + 0x84) + 0xC))->arr[slot];
        B_3EC8 *b = r;
        D_3EC8 *d = (D_3EC8*)b;
        return !d->v09();
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
    if (isIdle_3EC8(*(int*)cWScriptMan_TheWScriptMan_3EC8)) {
        ((luno_cLunoVar*)self)->Type = 1;
        *luno_cLunoVar_castInt(self) = 1;
    } else {
        ((luno_cLunoVar*)self)->Type = 1;
        *luno_cLunoVar_castInt(self) = 0;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderInTunnelFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_3F80 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_3F80 __asm__("SSXApp");
struct V3_3F80 { float x, y, z; };
extern V3_3F80 luno_gLunoNil_3F80 __asm__("luno_gLunoNil");
struct A_3F80 { char pad[0x6C0]; };
struct B_3F80 { virtual void v01(); };
struct D_3F80 : A_3F80, B_3F80 { };
struct Tab_3F80 { char pad[0x28]; D_3F80 *arr[1]; };
/**
 * Script function (gLunoCFunctions[74]): marks the script's rider as being in a tunnel.
 *
 * Takes no script arguments. When cWScriptMan_TheWScriptMan->Being is not -1, sets that rider's +0x3FC (the word
 * cRider_setInTunnel and cRider_isInTunnel use; cRider::InTunnel) to 1. cRider_updateSplit1 clears it again.
 *
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_3F80 RiderInTunnelFunc(void) {
    int slot = *(int*)cWScriptMan_TheWScriptMan_3F80;
    if (slot != -1) {
        D_3F80 *riderEntry = (*(Tab_3F80**)(*(char**)(SSXApp_3F80 + 0x84) + 0xC))->arr[slot];
        B_3F80 *riderIface = riderEntry;
        D_3F80 *rider = (D_3F80*)riderIface;
        *(int*)((char*)rider + 0x3FC) = 1;
    }
    return luno_gLunoNil_3F80;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayPointsFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_3FF0 { char c; Flag_3FF0() {} Flag_3FF0(const Flag_3FF0& o) : c(o.c) {} };
struct Ent_3FF0 { int idx; int val; int pad; int kind; };
struct R12_3FF0 { int a; int b; int c; };
extern R12_3FF0 luno_gLunoNil_3FF0 __asm__("luno_gLunoNil");

extern int D_004A3D40_3FF0[2] __asm__("D_004A3D40");
void cWScriptMan_updateMissionPoints(void*, int, int);
extern void* cWScriptMan_TheWScriptMan_3FF0 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A57E4_3FF0 __asm__("D_004A57E4");
struct T12_3FF0 { int a; int b; Flag_3FF0 f; };
extern T12_3FF0 D_004FBC98_3FF0 __asm__("D_004FBC98");
/**
 * Script function (gLunoCFunctions[98]): shows the mission points display with a value and a goal.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayPointsData: Points, Goal) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_004A3D40 says 2. Defaults: Points 0, Goal -1.
 *
 * Calls cWScriptMan_updateMissionPoints(cWScriptMan_TheWScriptMan, Points, Goal), which sets MissionPointsEnable,
 * MissionPoints and MissionPointsGoal.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_3FF0 UpdateDisplayPointsFunc(int n, Ent_3FF0* params) {
    if (D_004A57E4_3FF0 == 0) {
        D_004FBC98_3FF0.a = 0;
        D_004FBC98_3FF0.b = -1;
        D_004A57E4_3FF0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBC98_3FF0.f = Flag_3FF0(D_004FBC98_3FF0.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBC98_3FF0, 12);
    Ent_3FF0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D40_3FF0[k] && D_004A3D40_3FF0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_updateMissionPoints(cWScriptMan_TheWScriptMan_3FF0, vp[0], vp[1]);
    return luno_gLunoNil_3FF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayCountFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_4100 { char c; Flag_4100() {} Flag_4100(const Flag_4100& o) : c(o.c) {} };
struct Ent_4100 { int idx; int val; int pad; int kind; };
struct R12_4100 { int a; int b; int c; };
extern R12_4100 luno_gLunoNil_4100 __asm__("luno_gLunoNil");

extern int D_004A3D48_4100[2] __asm__("D_004A3D48");
void cWScriptMan_updateMissionCount(void*, int, int);
extern void* cWScriptMan_TheWScriptMan_4100 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A57E8_4100 __asm__("D_004A57E8");
struct T12_4100 { int a; int b; Flag_4100 f; };
extern T12_4100 D_004FBCA8_4100 __asm__("D_004FBCA8");
/**
 * Script function (gLunoCFunctions[75]): shows the mission count display with a count and a maximum.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayCountData: Count, Max) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3D48 says 2. Defaults: Count 0, Max 0.
 *
 * Calls cWScriptMan_updateMissionCount(cWScriptMan_TheWScriptMan, Count, Max), which sets MissionCountEnable,
 * MissionCount and MissionCountMax.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_4100 UpdateDisplayCountFunc(int n, Ent_4100* params) {
    if (D_004A57E8_4100 == 0) {
        D_004FBCA8_4100.a = 0;
        D_004A57E8_4100 = 1;
        D_004FBCA8_4100.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBCA8_4100.f = Flag_4100(D_004FBCA8_4100.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBCA8_4100, 12);
    Ent_4100* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D48_4100[k] && D_004A3D48_4100[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_updateMissionCount(cWScriptMan_TheWScriptMan_4100, vp[0], vp[1]);
    return luno_gLunoNil_4100;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayTicksFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_4210 { char c; Flag_4210() {} Flag_4210(const Flag_4210& o) : c(o.c) {} };
struct Ent_4210 { int idx; int val; int pad; int kind; };
struct R12_4210 { int a; int b; int c; };
extern R12_4210 luno_gLunoNil_4210 __asm__("luno_gLunoNil");

extern int D_004A3D50_4210[2] __asm__("D_004A3D50");
void cWScriptMan_updateMissionTicks(void*, int, int);
extern void* cWScriptMan_TheWScriptMan_4210 __asm__("cWScriptMan_TheWScriptMan");
extern int D_004A57EC_4210 __asm__("D_004A57EC");
struct T12_4210 { int a; int b; Flag_4210 f; };
extern T12_4210 D_004FBCB8_4210 __asm__("D_004FBCB8");
/**
 * Script function (gLunoCFunctions[76]): shows the mission clock with a tick count and a direction.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour
 * tUpdateDisplayTicksData: Ticks, Dir) and lets each argument override the field its index names, turning an int into a
 * float where the field type table D_004A3D50 says 2. Defaults: Ticks 0, Dir -1.
 *
 * Calls cWScriptMan_updateMissionTicks(cWScriptMan_TheWScriptMan, Ticks, Dir), which sets MissionTicksEnable,
 * MissionTicks and MissionTicksDir.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_4210 UpdateDisplayTicksFunc(int n, Ent_4210* params) {
    if (D_004A57EC_4210 == 0) {
        D_004FBCB8_4210.a = 0;
        D_004FBCB8_4210.b = -1;
        D_004A57EC_4210 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBCB8_4210.f = Flag_4210(D_004FBCB8_4210.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBCB8_4210, 12);
    Ent_4210* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D50_4210[k] && D_004A3D50_4210[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cWScriptMan_updateMissionTicks(cWScriptMan_TheWScriptMan_4210, vp[0], vp[1]);
    return luno_gLunoNil_4210;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayHeightFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern cLunoValue luno_gLunoNil;
// Reference sets of the current world (wscript/wscriptman.cpp layout). A reference id packs the set index (low 8 bits)
// and the slot; refs[slot] >> 8 is the object address >> 2.
struct sRefSet {
    char pad_0x0[0x1C];
    unsigned int* refs;     // 0x1C
};
// The script manager (cWScriptMan in the prototype, whose setBeing stores at 0x0); cWScriptMan_TheWScriptMan points at
// the instance.
extern cWScriptMan* cWScriptMan_TheWScriptMan;
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

static inline char *refToPtr_1FBE38(unsigned i) { return (char*)(i << 2); }
static inline char *refToObj_1FBE38(unsigned i) { if (i == 0) return 0; return refToPtr_1FBE38(i); }
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned D_004A57F0_4320 __asm__("D_004A57F0");
extern sBindEnd_1FBE38 D_004A57F4_4320 __asm__("D_004A57F4");
extern int D_004A57F8_4320 __asm__("D_004A57F8");
extern int D_004A3D58_4320[2] __asm__("D_004A3D58");
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
 * float where the field type table D_004A3D58 says 2. Defaults: InstId -1.
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
extern "C" cLunoValue UpdateDisplayHeightFunc(int n, sBindArg_1FBE38* params) {
    if (D_004A57F8_4320 == 0) {
        D_004A57F0_4320 = 0xFFFFFFFF;
        D_004A57F8_4320 = 1;
        // NOTE: guarded static-local init; the trailing empty member's value-init copies its byte through a stack temp
        //       (lbu; sb 0(sp)).
        D_004A57F4_4320 = sBindEnd_1FBE38();
    }
    unsigned buf64[2];
    unsigned* buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy(buf, &D_004A57F0_4320, 8);
    sBindArg_1FBE38* x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_004A3D58_4320[idx];
        unsigned* slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    int gv = curProcess_4320()->Being;
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = ((char*)curProcess_4320()->Inst);
    } else {
        sRefSet *section = ((sRefSet**)(cWorld_m_pTheWorld->m_pWorldCache)->m_pSectionCacheTable)[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = section->refs[id >> 8] >> 8;
            char *t = refToObj_1FBE38(i);
            inst = t;
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
struct Flag_44A8 { char c; Flag_44A8() {} Flag_44A8(const Flag_44A8& o) : c(o.c) {} };
struct Ent_44A8 { int idx; int val; int pad; int kind; };
struct R12_44A8 { int a; int b; int c; };
extern R12_44A8 luno_gLunoNil_44A8 __asm__("luno_gLunoNil");

extern int D_004A3D60_44A8[2] __asm__("D_004A3D60");
extern "C" void cGameModeMan_riderGateHit(int, int, int);
extern int* cWScriptMan_TheWScriptMan_44A8 __asm__("cWScriptMan_TheWScriptMan");
extern char* SSXApp_44A8 __asm__("SSXApp");
extern int D_004A5808_44A8 __asm__("D_004A5808");
extern int D_004A5800_44A8 __asm__("D_004A5800");
extern Flag_44A8 D_004A5804_44A8 __asm__("D_004A5804");
/**
 * Script function (gLunoCFunctions[82]): reports that the script's rider passed a gate.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tPassthroughGateData:
 * GateId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D60 says 2. Defaults: GateId -1.
 *
 * Calls cGameModeMan_riderGateHit(SSXApp->GameModeMan, cWScriptMan_TheWScriptMan->Being, GateId), which forwards it to
 * the current game-mode handler.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_44A8 PassthroughGateFunc(int n, Ent_44A8* params) {
    if (D_004A5808_44A8 == 0) {
        D_004A5800_44A8 = -1;
        D_004A5808_44A8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5804_44A8 = Flag_44A8(D_004A5804_44A8);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5800_44A8, 8);
    Ent_44A8* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D60_44A8[k] && D_004A3D60_44A8[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    int beingIndex = *cWScriptMan_TheWScriptMan_44A8;
    cGameModeMan_riderGateHit(*(int*)(SSXApp_44A8 + 0xC0), beingIndex, vp[0]);
    return luno_gLunoNil_44A8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", UpdateDisplayTrickFunc);
#ifdef SKIP_ASM
// PORT: 64-bit bitfield container (unsigned long is 8 bytes on the EE).
extern char *cWScriptMan_TheWScriptMan_45B8 __asm__("cWScriptMan_TheWScriptMan");
struct V3_45B8 { float x, y, z; };
extern V3_45B8 luno_gLunoNil_45B8 __asm__("luno_gLunoNil");
struct Tag_45B8 { char c; Tag_45B8() {} Tag_45B8(const Tag_45B8 &o) : c(o.c) {} };
struct Rec_45B8 {
    unsigned a0; int a1; int a2; int a3;
    unsigned a4; unsigned a5; unsigned a6; unsigned a7;
    unsigned a8; unsigned a9; unsigned a10; unsigned a11;
    unsigned a12; unsigned a13; unsigned a14; unsigned a15;
    Tag_45B8 f;
};
extern Rec_45B8 D_4FBCC8_45B8 __asm__("D_004FBCC8");
extern int D_580C_45B8 __asm__("D_004A580C");
extern const int D_00446650_45B8[] __asm__("D_00446650");
extern "C" void* MEM_copy_45B8(void*, void*, int) __asm__("MEM_copy");
struct BF_45B8 {
    unsigned long f0 : 3; unsigned long f1 : 4; unsigned long f2 : 3; unsigned long f3 : 2;
    unsigned long f4 : 4; unsigned long f5 : 3; unsigned long f6 : 3; unsigned long f7 : 6;
    unsigned long f8 : 4; unsigned long f9 : 3; unsigned long f10 : 7; unsigned long f11 : 1;
    unsigned long f12 : 7; unsigned long f13 : 2; unsigned long f14 : 4; unsigned long f15 : 3;
    unsigned long f16 : 5;
};
extern "C" void cWScriptMan_updateMissionTrick_45B8(char*, BF_45B8*) __asm__("cWScriptMan_updateMissionTrick");
struct Ent_45B8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[83]): shows the mission trick display for a trick built from script arguments.
 *
 * Copies the static defaults (set up on the first call) into a 68-byte parameter block (On Tour
 * tUpdateDisplayTrickData: Rail, RailSpin, TakeOff, Orient, Spin, Reps, Flip, Special, PostSpin, UberGrind, Grab,
 * ToLate, LateGrab, HandPlant, Land, Press) and lets each argument override the field its index names, turning an int
 * into a float where the field type table D_00446650 says 2. Defaults: all 0.
 *
 * Packs the 16 values into an 8-byte trick definition (bit fields; Land and Press are stored in the opposite order of
 * the block) and passes it to cWScriptMan_updateMissionTrick, which sets MissionTrickEnable and MissionTrick.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_45B8 UpdateDisplayTrickFunc(int n, Ent_45B8 *params) {
    if (D_580C_45B8 == 0) {
        D_4FBCC8_45B8.a0 = 0;
        D_4FBCC8_45B8.a1 = 0;
        D_4FBCC8_45B8.a2 = 0;
        D_4FBCC8_45B8.a3 = 0;
        D_4FBCC8_45B8.a4 = 0;
        D_4FBCC8_45B8.a5 = 0;
        D_4FBCC8_45B8.a6 = 0;
        D_4FBCC8_45B8.a7 = 0;
        D_4FBCC8_45B8.a8 = 0;
        D_4FBCC8_45B8.a9 = 0;
        D_4FBCC8_45B8.a10 = 0;
        D_4FBCC8_45B8.a11 = 0;
        D_4FBCC8_45B8.a12 = 0;
        D_4FBCC8_45B8.a13 = 0;
        D_4FBCC8_45B8.a14 = 0;
        D_4FBCC8_45B8.a15 = 0;
        D_580C_45B8 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_4FBCC8_45B8.f = Tag_45B8(D_4FBCC8_45B8.f);
    }
    Rec_45B8 rec;
    Rec_45B8 *buf = &rec;
    MEM_copy_45B8(buf, &D_4FBCC8_45B8, 68);
    BF_45B8 bf;
    Ent_45B8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446650_45B8[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    bf.f0 = buf->a0;
    bf.f1 = buf->a1;
    bf.f2 = buf->a2;
    bf.f3 = buf->a3;
    bf.f4 = buf->a4;
    bf.f5 = buf->a5;
    bf.f6 = buf->a6;
    bf.f7 = buf->a7;
    bf.f8 = buf->a8;
    bf.f9 = buf->a9;
    bf.f10 = buf->a10;
    bf.f11 = buf->a11;
    bf.f12 = buf->a12;
    bf.f13 = buf->a13;
    bf.f15 = buf->a14;
    bf.f14 = buf->a15;
    bf.f16 = 0;
    cWScriptMan_updateMissionTrick_45B8(cWScriptMan_TheWScriptMan_45B8, &bf);
    return luno_gLunoNil_45B8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddFadeFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_4980 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_4980 __asm__("SSXApp");
void cMoveNode_addFade(void*, void*);
struct O_4980 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_4980 { float x, y, z; };
extern V3_4980 luno_gLunoNil_4980 __asm__("luno_gLunoNil");
struct Tag_4980 { char c; Tag_4980() {} Tag_4980(const Tag_4980 &o) : c(o.c) {} };
extern unsigned D_4980[] __asm__("D_004FBD10");
extern int D_004A5810;
extern int D_004A3D68;
extern "C" void* MEM_copy_4980(void*, void*, int) __asm__("MEM_copy");
struct Ent_4980 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[84]): adds a fade modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAddFadeData: InstId,
 * Fade) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3D68 says 2. Defaults: InstId -1, Fade 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode), cMoveNode_addFade gets the block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_4980 AddFadeFunc(int n, Ent_4980 *params) {
    if (D_004A5810 == 0) {
        D_4980[0] = 0xFFFFFFFF;
        *(float*)&D_4980[1] = 1.0f;
        D_004A5810 = 1;
        (*(Tag_4980*)((char*)D_4980 + 8)) = Tag_4980((*(Tag_4980*)((char*)D_4980 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_4980(buf, D_4980, 12);
    Ent_4980 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3D68)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_4980 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_4980 *activeNode = *(O_4980**)(obj + 0xC);
    if (activeNode == 0)
        return luno_gLunoNil_4980;
    if (activeNode->v16())
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
struct Flag_4B38 { char c; Flag_4B38() {} Flag_4B38(const Flag_4B38& o) : c(o.c) {} };
struct Ent_4B38 { int idx; int val; int pad; int kind; };
struct R12_4B38 { int a; int b; int c; };
extern R12_4B38 luno_gLunoNil_4B38 __asm__("luno_gLunoNil");

extern int D_004A3D70_4B38[2] __asm__("D_004A3D70");
extern "C" void cReplay_eventWScript(int, int, int);
extern int* cWScriptMan_TheWScriptMan_4B38 __asm__("cWScriptMan_TheWScriptMan");
extern char* SSXApp_4B38 __asm__("SSXApp");
extern int D_004A5820_4B38 __asm__("D_004A5820");
extern int D_004A5818_4B38 __asm__("D_004A5818");
extern Flag_4B38 D_004A581C_4B38 __asm__("D_004A581C");
/**
 * Script function (gLunoCFunctions[85]): records a world-script event in the replay.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tReplayData: Cmd) and
 * lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D70 says 2. Defaults: Cmd -1.
 *
 * Calls cReplay_eventWScript(SSXApp->Game->Replay, cWScriptMan_TheWScriptMan->Being, Cmd).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_4B38 ReplayFunc(int n, Ent_4B38* params) {
    if (D_004A5820_4B38 == 0) {
        D_004A5818_4B38 = -1;
        D_004A5820_4B38 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A581C_4B38 = Flag_4B38(D_004A581C_4B38);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5818_4B38, 8);
    Ent_4B38* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D70_4B38[k] && D_004A3D70_4B38[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    cReplay_eventWScript(*(int*)(*(char**)(SSXApp_4B38 + 0x84) + 0x28), *cWScriptMan_TheWScriptMan_4B38, vp[0]);
    return luno_gLunoNil_4B38;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", RiderHeightFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* luno_cLunoVar_castFloat(void*);
extern "C" void MEM_copy(void*, void*, int);
struct Tag_4C50 { char c; Tag_4C50() {} Tag_4C50(const Tag_4C50 &o) : c(o.c) {} };
struct Ent_4C50 { int idx; int val; int pad; int type; };
extern char *cWScriptMan_TheWScriptMan_4C50 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_4C50 __asm__("SSXApp");
extern unsigned D_5828_4C50 __asm__("D_004A5828");
extern Tag_4C50 D_582C_4C50 __asm__("D_004A582C");
extern int D_5830_4C50 __asm__("D_004A5830");
extern int D_3D78_4C50 __asm__("D_004A3D78");
struct Vec_4C50 { float x, y, z, w; } __attribute__((aligned(16)));
struct A_4C50 { char pad[0x6C0]; };
struct B_4C50 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual Vec_4C50 *v05(); };
struct D_4C50 : A_4C50, B_4C50 { };
struct Tab_4C50 { char pad[0x28]; D_4C50 *arr[1]; };
static inline Vec_4C50 pos_4C50(char *o) { return *(Vec_4C50*)(o + 0x40); }
/**
 * Script function (gLunoCFunctions[86]): returns how high the script's rider is above a world instance.
 *
 * Copies the static defaults (InstId -1; set up on the first call) into an 8-byte parameter block and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D78 says
 * 2.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. The float result in the hidden result slot self is the z of the
 * script's rider (SSXApp->Game->AI->Riders[cWScriptMan_TheWScriptMan->Being]) (cRider virtual getPosition) minus the
 * instance's position z (+0x40), 0 when the rider slot is empty, and 0 when the instance is not found.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to the height difference
 */
extern "C" void* RiderHeightFunc(void* self, int n, Ent_4C50 *params) {
    if (D_5830_4C50 == 0) {
        D_5828_4C50 = 0xFFFFFFFF;
        D_5830_4C50 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_582C_4C50 = Tag_4C50(D_582C_4C50);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy(buf, &D_5828_4C50, 8);
    Ent_4C50 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_3D78_4C50)[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    unsigned id = buf[0];
    int slot = *(int*)cWScriptMan_TheWScriptMan_4C50;
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_4C50 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (obj) {
        D_4C50 *rr = (*(Tab_4C50**)(*(char**)(SSXApp_4C50 + 0x84) + 0xC))->arr[slot];
        B_4C50 *riderIface = rr;
        D_4C50 *d = (D_4C50*)riderIface;
        float heightDiff;
        if (d) {
            float z = d->v05()->z;
            heightDiff = z - pos_4C50(obj).z;
        }
        else
            heightDiff = 0.0f;
        *(int*)((char*)self + 8) = 2;
        *(float*)luno_cLunoVar_castFloat(self) = heightDiff;
    } else {
        *(int*)((char*)self + 8) = 2;
        *(int*)luno_cLunoVar_castFloat(self) = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddDebounceFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_4E38 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_4E38 __asm__("SSXApp");
extern "C" void cMoveNode_addDebounce_4E38(void*, void*) __asm__("cMoveNode_addDebounce");
struct O_4E38 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_4E38 { float x, y, z; };
extern V3_4E38 luno_gLunoNil_4E38 __asm__("luno_gLunoNil");
struct Tag_4E38 { char c; Tag_4E38() {} Tag_4E38(const Tag_4E38 &o) : c(o.c) {} };
extern unsigned D_4E38[] __asm__("D_004FBD20");
extern int D_004A5834;
extern int D_004A3D80;
extern "C" void* MEM_copy_4E38(void*, void*, int) __asm__("MEM_copy");
struct Ent_4E38 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[87]): adds a debounce modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tAddDebounceData:
 * InstId, Time) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3D80 says 2. Defaults: InstId -1, Time 1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance's active node (+0xC) is a move node (virtual
 * isMoveNode), cMoveNode_addDebounce gets the block.
 *
 * Quirk: an instance that is not found is not checked; its node is read through a null pointer.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_4E38 AddDebounceFunc(int n, Ent_4E38 *params) {
    if (D_004A5834 == 0) {
        D_4E38[0] = 0xFFFFFFFF;
        *(float*)&D_4E38[1] = 1.0f;
        D_004A5834 = 1;
        (*(Tag_4E38*)((char*)D_4E38 + 8)) = Tag_4E38((*(Tag_4E38*)((char*)D_4E38 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    MEM_copy_4E38(buf, D_4E38, 12);
    Ent_4E38 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3D80)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_4E38 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    O_4E38 *activeNode = *(O_4E38**)(obj + 0xC);
    if (activeNode == 0)
        return luno_gLunoNil_4E38;
    if (activeNode->v16())
        cMoveNode_addDebounce_4E38(activeNode, buf);
    return luno_gLunoNil_4E38;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CameraFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A5838_4FF0 __asm__("D_004A5838");
extern int D_004A3D88_4FF0[2] __asm__("D_004A3D88");
extern "C" void MEM_copy(void*, void*, int);

struct Flag_4FF0 { char c; Flag_4FF0() {} Flag_4FF0(const Flag_4FF0& o) : c(o.c) {} };
struct Ent_4FF0 { int idx; int val; int pad; int kind; };
struct R12_4FF0 { int a; int b; int c; };
struct T12_4FF0 { int a; int b; Flag_4FF0 f; };
extern T12_4FF0 D_004FBD30_4FF0 __asm__("D_004FBD30");
extern R12_4FF0 luno_gLunoNil_4FF0 __asm__("luno_gLunoNil");

/**
 * Script function (gLunoCFunctions[89]): reads its arguments and does nothing else (a stubbed camera command).
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tCameraData: Cmd, Type)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D88 says 2. Defaults: Cmd 0, Type 0.
 *
 * No camera call follows; the block is unused.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_4FF0 CameraFunc(int n, Ent_4FF0* params) {
    if (D_004A5838_4FF0 == 0) {
        D_004FBD30_4FF0.a = 0;
        D_004A5838_4FF0 = 1;
        D_004FBD30_4FF0.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBD30_4FF0.f = Flag_4FF0(D_004FBD30_4FF0.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBD30_4FF0, 12);
    Ent_4FF0* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D88_4FF0[k] && D_004A3D88_4FF0[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    return luno_gLunoNil_4FF0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CameraShakeFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern cLunoValue luno_gLunoNil;
// Reference sets of the current world (wscript/wscriptman.cpp layout). A reference id packs the set index (low 8 bits)
// and the slot; refs[slot] >> 8 is the object address >> 2.
// The script manager (cWScriptMan in the prototype, whose setBeing stores at 0x0); cWScriptMan_TheWScriptMan points at
// the instance.
extern cWScriptMan* cWScriptMan_TheWScriptMan;
struct tInstanceRecord;
class cBeing {
public:
    virtual ~cBeing();
    virtual const t4Vector* getVelocity() const;
    virtual int isIntersecting(tInstanceRecord* rec) const;
    virtual void applyBoost(const t4Vector& impulse);
    virtual const t4Vector* getPosition() const;
    virtual const t4Matrix* getTransform() const;
    virtual int getAIIndex() const;
    virtual bool isHuman() const;
    virtual bool isComputer() const;
    virtual void transport(const t4Matrix& m);
    virtual void resetToPath();
    virtual void initForMission(const t4Matrix& m);
};
// The scene node of a world object (the object holds it at +0xC): the prototype's cObjNode / cTypeObjNode /
// cInstanceNode family. Slot 1 is the virtual destructor; isType (10) and isMoveNode (16) are named as in the
// prototype's _vt$13cInstanceNode; the other slots are known only by position.
struct cInstanceNode_1FBE38 {
    int p0, p1, p2;
    virtual ~cInstanceNode_1FBE38();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual int isType(int type) const;
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int isMoveNode();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual t4Matrix* getRootTransform();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual float v30();
    virtual int v31();
    virtual int v32(float);
    virtual void v33();
    virtual void v34(int);
    virtual void v35();
    virtual void v36(int, float);
    virtual void v37();
};
// A plain (not 16-byte aligned) 4-float vector built in place for the node calls that take a vector pointer.
struct sVec4_1FBE38 { float x, y, z, w; sVec4_1FBE38(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {} };
// A rider as these bindings see it (include/ai/rider.h has the flat layout): its cBeing base sits at 0x6C0, after the
// rider's own data, so converting between cRider_1FBE38* and cBeing* adds/subtracts 0x6C0 with a null check.
struct cRiderData_1FBE38 {
    char pad_0x0[0x3FC];
    int InTunnel;               // 0x3FC (cRider::InTunnel)
    char pad_0x400[0x2C0];
};
struct sRiderHandler_1FBE38 {
    char pad_0x0[0xD4];
    unsigned int fD4;           // 0xD4
};
class cRider_1FBE38 : public cRiderData_1FBE38, public cBeing {
public:
    char pad_0x6C4[0xB8];
    sRiderHandler_1FBE38* mRiderHandler;    // 0x77C (cRider::mRiderHandler)
    char pad_0x780[0xF0];
    int mViewIndex;             // 0x870 (cRider::mViewIndex)
    int mIsHuman;               // 0x874
    int mIsActor;               // 0x878
    int mIsLocal;               // 0x87C
};
// The race (main/gamemode.cpp layout): riders by being index at 0x28; each player holds its rider at +0x18.
struct sRacePlayer {
    char pad_0x0[0x18];
    cRider_1FBE38* rider;       // 0x18
};
struct sRace {
    char pad_0x0[0x28];
    cRider_1FBE38* riders[6];   // 0x28
    sRacePlayer* players[12];   // 0x40
    sRacePlayer* players2[3];   // 0x70
    int numPlayers;             // 0x7C
    char pad_0x80[0x8];
    int numPlayers2;            // 0x88
    char pad_0x8C[0x1C];
    void* fA8;                  // 0xA8
};
// The game's cameras (camera/camera.cpp); one per view, indexed by cRider::mViewIndex.
struct sCameras {
    int f0;                     // 0x0
    char* cam[1];               // 0x4
};
struct sGame {
    char pad_0x0[0xC];
    sRace* race;                // 0xC
    char pad_0x10[0x18];
    void* f28;                  // 0x28
    char pad_0x2C[0x4C];
    char* f78;                  // 0x78
    char pad_0x7C[0x8];
    sCameras* cameras;          // 0x84
};
// The game singleton (SSXApp).
extern void* cMultiParticleMan_TheMultiParticleMan;
// One script argument as the VM passes it to a binding: the slot index in the defaults record, then the value
// (cLunoValue layout: value word, second word, type).
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_1FBE38(const t4Vector &a, const t4Vector &b) {
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
static inline float vLen_1FBE38(const t4Vector &v) {
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
// True when the race slot (-1 = none) holds a rider that is not computer-controlled.
// Defaults record of this camera-shake binding; keyword indexes the shake keyword table (the prototype asserts
// dp->Index < kNumCameraShakeKeywords).
struct Rec_50F0 { int keyword; int useObj; unsigned id; float scale; sBindEnd_1FBE38 f; };
extern Rec_50F0 D_004FBD40_50F0 __asm__("D_004FBD40");
extern int D_004A583C;
extern const int D_00446690[];
extern int D_004A3BD8[2];  // shake type per camera-shake keyword
extern "C" void cCamera_shakeCamera(char* cam, int type, float amount, float dist);
// Translation row of a transform (by value).
static inline t4Vector trans_50F0(const t4Matrix *m) { return m->row[3]; }
/**
 * Script function (gLunoCFunctions[91]): shakes the cameras of local riders, scaled by their distance from a world
 * instance, or of the script's rider only.
 *
 * Copies the static defaults (set up on the first call) into a 20-byte parameter block (On Tour tCameraShakeData: Type,
 * Proximity, InstId, Scale) and lets each argument override the field its index names, turning an int into a float
 * where the field type table D_00446690 says 2. Defaults: Type 0, Proximity 1, InstId -1, Scale 1.
 *
 * Only Type 0 does anything (shake type D_004A3BD8[Type]). With Proximity set the instance is looked up (InstId -1:
 * cWScriptMan_TheWScriptMan->Inst). With an instance, each race player's rider (SSXApp->Game +0xC: players at +0x40,
 * count at +0x7C) that is local (mIsLocal) gets cCamera_shakeCamera on its view's camera (mViewIndex) with weight 1 and
 * its distance to the instance's root transform (node virtual getRootTransform, or the instance's own transform) times
 * Scale. Without one, only the player whose rider's AI index (virtual getAIIndex) is the script's Being shakes, at
 * distance 0.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" cLunoValue CameraShakeFunc(int n, sBindArg_1FBE38 *params) {
    if (D_004A583C == 0) {
        D_004FBD40_50F0.keyword = 0;
        D_004FBD40_50F0.useObj = 1;
        // NOTE: guarded static-local init; the trailing empty member's value-init copies its byte through a stack temp
        //       (lbu; sb 0(sp)).
        D_004FBD40_50F0.f = sBindEnd_1FBE38();
        D_004FBD40_50F0.scale = 1.0f;
        D_004FBD40_50F0.id = 0xFFFFFFFF;
        D_004A583C = 1;
    }
    Rec_50F0 rec;
    Rec_50F0 *buf = &rec;
    rec.id = 0xFFFFFFFF;
    MEM_copy(buf, &D_004FBD40_50F0, 20);
    sBindArg_1FBE38 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446690[idx];
        unsigned *d = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)d = (float)x->val;
        else
            *d = x->val;
    }
    char *obj = 0;
    if (buf->useObj) {
        unsigned id = buf->id;
        if (~id == 0) {
            obj = ((char*)cWScriptMan_TheWScriptMan->Inst);
        } else {
            sRefSet *section = ((sRefSet**)(cWorld_m_pTheWorld->m_pWorldCache)->m_pSectionCacheTable)[id & 0xFF];
            char *r;
            if (!section)
                r = 0;
            else
                r = refToObj_1FBE38(section->refs[id >> 8] >> 8);
            obj = r;
        }
    }
    int being = cWScriptMan_TheWScriptMan->Being;
    float scale = buf->scale;
    int keyword = buf->keyword;
    if (keyword == 0) {
        sGame *game = (sGame*)SSXApp->Game;
        int np = game->race->numPlayers;
        if (obj) {
            for (int i = 0; i < np; i++) {
                const t4Vector *rp = game->race->players[i]->rider->getPosition();
                cInstanceNode_1FBE38 *node = *(cInstanceNode_1FBE38**)(obj + 0xC);
                t4Matrix *rootTransform = node ? node->getRootTransform() : (t4Matrix*)(obj + 0x10);
                t4Vector d = vSub_1FBE38(*rp, trans_50F0(rootTransform));
                float dist = vLen_1FBE38(d);
                cRider_1FBE38 *r = game->race->players[i]->rider;
                int view = r->mViewIndex;
                if (r->mIsLocal)
                    cCamera_shakeCamera(game->cameras->cam[view], D_004A3BD8[keyword], 1.0f, dist * scale);
            }
        } else {
            for (int i = 0; i < np; i++) {
                if (game->race->players[i]->rider->getAIIndex() == being) {
                    cRider_1FBE38 *r = game->race->players[i]->rider;
                    int view = r->mViewIndex;
                    if (r->mIsLocal)
                        cCamera_shakeCamera(game->cameras->cam[view], D_004A3BD8[keyword], 1.0f, 0.0f);
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
extern char *cWScriptMan_TheWScriptMan_5478 __asm__("cWScriptMan_TheWScriptMan");
struct O_5478 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void *v24(); };
struct V3_5478 { float x, y, z; };
extern V3_5478 luno_gLunoNil_5478 __asm__("luno_gLunoNil");
struct Tag_5478 { char c; Tag_5478() {} Tag_5478(const Tag_5478 &o) : c(o.c) {} };
struct Rec_5478 { unsigned id; float a; float b; Tag_5478 f; };
extern Rec_5478 D_4FBD58_5478 __asm__("D_004FBD58");
extern int D_5840_5478 __asm__("D_004A5840");
extern const int D_004466A0_5478[] __asm__("D_004466A0");
extern "C" void* MEM_copy_5478(void*, void*, int) __asm__("MEM_copy");
extern "C" void cMoveNode_addMagnet_5478(O_5478*, void*, float, float) __asm__("cMoveNode_addMagnet");
struct Ent_5478 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[90]): adds a magnet modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tAddMagnetData: InstId,
 * Radius, Speed) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004466A0 says 2. Defaults: InstId -1, Radius 400, Speed 30.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (+0xC) is a move
 * node (virtual isMoveNode), cMoveNode_addMagnet gets the node's root transform (virtual getRootTransform), Radius and
 * Speed.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_5478 AddMagnetFunc(int n, Ent_5478 *params) {
    if (D_5840_5478 == 0) {
        D_4FBD58_5478.id = 0xFFFFFFFF;
        D_4FBD58_5478.a = 400.0f;
        D_4FBD58_5478.b = 30.0f;
        D_5840_5478 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_4FBD58_5478.f = Tag_5478(D_4FBD58_5478.f);
    }
    Rec_5478 rec;
    Rec_5478 *buf = &rec;
    rec.id = 0xFFFFFFFF;
    MEM_copy_5478(buf, &D_4FBD58_5478, 0x10);
    Ent_5478 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004466A0_5478[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf->id;
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_5478 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        O_5478 *activeNode = *(O_5478**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
                cMoveNode_addMagnet_5478(activeNode, activeNode->v24(), buf->a, buf->b);
        }
    }
    return luno_gLunoNil_5478;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", LightningFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int *cWScriptMan_TheWScriptMan_5660 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_5660 __asm__("SSXApp");
struct Sh_5660 { virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual int v08(); };
extern "C" void DoLightningFlash_5660(float) __asm__("DoLightningFlash");
struct V3_5660 { float x, y, z; };
extern V3_5660 luno_gLunoNil_5660 __asm__("luno_gLunoNil");
struct Tag_5660 { char c; Tag_5660() {} Tag_5660(const Tag_5660 &o) : c(o.c) {} };
extern unsigned D_5660 __asm__("D_004A5848");
extern Tag_5660 D_004A584C;
extern int D_004A5850;
extern int D_004A3D90;
extern "C" void* MEM_copy_5660(void*, void*, int) __asm__("MEM_copy");
struct Ent_5660 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[92]): starts a lightning flash when the script's rider is human.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tLightningData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3D90 says 2. Defaults: InstId -1.
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
extern "C" V3_5660 LightningFunc(int n, Ent_5660 *params) {
    if (D_004A5850 == 0) {
        D_5660 = 0xFFFFFFFF;
        D_004A5850 = 1;
        D_004A584C = Tag_5660(D_004A584C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_5660(buf, &D_5660, 8);
    Ent_5660 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3D90)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    int k = *cWScriptMan_TheWScriptMan_5660;
    if (k != -1) {
        int slotOffset = k * 4;
        char *lvl = *(char**)(*(char**)(SSXApp_5660 + 0x84) + 0xC);
        char *riderEntry = *(char**)(lvl + slotOffset + 0x28);
        Sh_5660 *sh = riderEntry ? (Sh_5660*)(riderEntry + 0x6C0) : 0;
        if (sh->v08())
            DoLightningFlash_5660(0.0f);
    }
    return luno_gLunoNil_5660;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", AddHaloFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_57C0 __asm__("cWScriptMan_TheWScriptMan");
struct O_57C0 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void *v24(); };
struct V3_57C0 { float x, y, z; };
extern V3_57C0 luno_gLunoNil_57C0 __asm__("luno_gLunoNil");
struct Tag_57C0 { char c; Tag_57C0() {} Tag_57C0(const Tag_57C0 &o) : c(o.c) {} };
struct Rec_57C0 { unsigned id; int a; int b; float c; float d; float e; float g; float h; int k; Tag_57C0 f; };
extern Rec_57C0 D_4FBD68_57C0 __asm__("D_004FBD68");
extern int D_5854_57C0 __asm__("D_004A5854");
extern const int D_004466B0_57C0[] __asm__("D_004466B0");
extern "C" void* MEM_copy_57C0(void*, void*, int) __asm__("MEM_copy");
extern "C" void cMoveNode_addHalo_57C0(void*, void*) __asm__("cMoveNode_addHalo");
struct Ent_57C0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[97]): adds a halo modifier to a moving world instance.
 *
 * Copies the static defaults (set up on the first call) into a 40-byte parameter block (On Tour tAddHaloData: InstId,
 * Type, Mesh, ColourA, ColourR, ColourG, ColourB, Radius, Speed) and lets each argument override the field its index
 * names, turning an int into a float where the field type table D_004466B0 says 2. Defaults: InstId -1, Type 0, Mesh
 * -1, ColourA, ColourR, ColourG and ColourB 1, Radius 100, Speed 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (+0xC) is a move
 * node (virtual isMoveNode), cMoveNode_addHalo gets the block.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_57C0 AddHaloFunc(int n, Ent_57C0 *params) {
    if (D_5854_57C0 == 0) {
        D_4FBD68_57C0.id = 0xFFFFFFFF;
        D_4FBD68_57C0.a = 0;
        D_4FBD68_57C0.b = -1;
        D_4FBD68_57C0.c = 1.0f;
        D_4FBD68_57C0.d = 1.0f;
        D_4FBD68_57C0.e = 1.0f;
        D_4FBD68_57C0.g = 1.0f;
        D_4FBD68_57C0.h = 100.0f;
        D_4FBD68_57C0.k = 0;
        D_5854_57C0 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_4FBD68_57C0.f = Tag_57C0(D_4FBD68_57C0.f);
    }
    Rec_57C0 rec;
    Rec_57C0 *buf = &rec;
    rec.id = 0xFFFFFFFF;
    MEM_copy_57C0(buf, &D_4FBD68_57C0, 40);
    Ent_57C0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004466B0_57C0[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf->id;
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_57C0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        O_57C0 *activeNode = *(O_57C0**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
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
// Reference sets of the current world (wscript/wscriptman.cpp layout). A reference id packs the set index (low 8 bits)
// and the slot; refs[slot] >> 8 is the object address >> 2.
// The script manager (cWScriptMan in the prototype, whose setBeing stores at 0x0); cWScriptMan_TheWScriptMan points at
// the instance.
extern cWScriptMan* cWScriptMan_TheWScriptMan;
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
        } \
    } while (0)
#define BIND_SET_RESULT_1FBE38(ret, value) do { \
        int r_ = (value); \
        (ret)->type = 1; \
        *luno_cLunoVar_castInt(ret) = r_; \
    } while (0)

struct Rec_59A0 { int head; unsigned id[8]; int cnt[8]; float f0[8]; float f1[8]; float f2[8]; float f3[8]; sBindEnd_1FBE38 f; };
extern Rec_59A0 D_004FBD90_59A0 __asm__("D_004FBD90");
extern const int D_004466D8_59A0[] __asm__("D_004466D8");
extern int D_004A5858_59A0 __asm__("D_004A5858");
static inline char *conv_59A0(unsigned i) { return i ? refToPtr_1FBE38(i) : 0; }
extern "C" void cAvalanche_addAvalancheNode(int, void*, float, float, float, float, int);
/**
 * Script function (gLunoCFunctions[93]): adds up to eight world instances as falling nodes of an avalanche.
 *
 * Copies the static defaults (set up on the first call) into a 200-byte parameter block and lets each argument override
 * the field its index names, turning an int into a float where the field type table D_004466D8 says 2. The block holds
 * the avalanche id (default 0) and eight sets of instance id (-1), type (0), life (10 s), fade-in (0 s), fade-out (3 s)
 * and speed (1).
 *
 * For each instance id that is not -1, the instance is looked up (a missing one passes 0) and
 * cAvalanche_addAvalancheNode(avalanche id, instance, type, life, fade-in, fade-out, speed) adds it.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" cLunoValue MiniAvalancheFunc(int n, sBindArg_1FBE38* params) {
    if (D_004A5858_59A0 == 0) {
        D_004FBD90_59A0.head = 0;
        D_004FBD90_59A0.id[7] = D_004FBD90_59A0.id[6] = D_004FBD90_59A0.id[5] = D_004FBD90_59A0.id[4] = D_004FBD90_59A0.id[3] = D_004FBD90_59A0.id[2] = D_004FBD90_59A0.id[1] = D_004FBD90_59A0.id[0] = 0xFFFFFFFF;
        D_004FBD90_59A0.cnt[7] = D_004FBD90_59A0.cnt[6] = D_004FBD90_59A0.cnt[5] = D_004FBD90_59A0.cnt[4] = D_004FBD90_59A0.cnt[3] = D_004FBD90_59A0.cnt[2] = D_004FBD90_59A0.cnt[1] = D_004FBD90_59A0.cnt[0] = 0;
        D_004FBD90_59A0.f0[7] = D_004FBD90_59A0.f0[6] = D_004FBD90_59A0.f0[5] = D_004FBD90_59A0.f0[4] = D_004FBD90_59A0.f0[3] = D_004FBD90_59A0.f0[2] = D_004FBD90_59A0.f0[1] = D_004FBD90_59A0.f0[0] = 10.0f;
        D_004FBD90_59A0.f1[7] = D_004FBD90_59A0.f1[6] = D_004FBD90_59A0.f1[5] = D_004FBD90_59A0.f1[4] = D_004FBD90_59A0.f1[3] = D_004FBD90_59A0.f1[2] = D_004FBD90_59A0.f1[1] = D_004FBD90_59A0.f1[0] = 0.0f;
        D_004FBD90_59A0.f2[7] = D_004FBD90_59A0.f2[6] = D_004FBD90_59A0.f2[5] = D_004FBD90_59A0.f2[4] = D_004FBD90_59A0.f2[3] = D_004FBD90_59A0.f2[2] = D_004FBD90_59A0.f2[1] = D_004FBD90_59A0.f2[0] = 3.0f;
        D_004FBD90_59A0.f3[7] = D_004FBD90_59A0.f3[6] = D_004FBD90_59A0.f3[5] = D_004FBD90_59A0.f3[4] = D_004FBD90_59A0.f3[3] = D_004FBD90_59A0.f3[2] = D_004FBD90_59A0.f3[1] = D_004FBD90_59A0.f3[0] = 1.0f;
        D_004A5858_59A0 = 1;
        // NOTE: guarded static-local init; the trailing empty member's value-init copies its byte through a stack temp
        //       (lbu; sb 0(sp)).
        D_004FBD90_59A0.f = sBindEnd_1FBE38();
    }
    Rec_59A0 rec;
    Rec_59A0* vp = &rec;
    rec.id[7] = rec.id[6] = rec.id[5] = rec.id[4] = rec.id[3] = rec.id[2] = rec.id[1] = rec.id[0] = 0xFFFFFFFF;
    MEM_copy(vp, &D_004FBD90_59A0, 200);
    sBindArg_1FBE38* x = params;
    for (int i = 0; i < n; i++, x++) {
        int k = x->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (x->type != D_004466D8_59A0[k] && D_004466D8_59A0[k] == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    int head = vp->head;
    int* cp = vp->cnt;
    float* f3 = vp->f3;
    float* f2 = vp->f2;
    float* f1 = vp->f1;
    float* f0 = vp->f0;
    for (int i = 0; i < 8; i++) {
        unsigned id = vp->id[i];
        if (id != 0xFFFFFFFF) {
            sRefSet *section = ((sRefSet**)(cWorld_m_pTheWorld->m_pWorldCache)->m_pSectionCacheTable)[id & 0xFF];
            char *inst;
            if (!section)
                inst = 0;
            else {
                unsigned j = section->refs[id >> 8] >> 8;
                inst = conv_59A0(j);
            }
            cAvalanche_addAvalancheNode(head, inst, *f0, *f1, *f2, *f3, *cp);
        }
        cp++; f3++; f2++; f1++; f0++;
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
struct Flag_5C88 { char c; Flag_5C88() {} Flag_5C88(const Flag_5C88& o) : c(o.c) {} };
struct Ent_5C88 { int idx; int val; int pad; int kind; };
struct R12_5C88 { int a; int b; int c; };
extern R12_5C88 luno_gLunoNil_5C88 __asm__("luno_gLunoNil");

extern int D_004A3D98_5C88[2] __asm__("D_004A3D98");
extern "C" int cAvalanche_triggerAvalanche(int);
extern int cAvalanche_m_badCookie_5C88 __asm__("cAvalanche_m_badCookie");
extern int D_004A5868_5C88 __asm__("D_004A5868");
extern int D_004A5860_5C88 __asm__("D_004A5860");
extern Flag_5C88 D_004A5864_5C88 __asm__("D_004A5864");
/**
 * Script function (gLunoCFunctions[94]): triggers an avalanche (cAvalanche_triggerAvalanche).
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (the avalanche id) and lets each
 * argument override the field its index names, turning an int into a float where the field type table D_004A3D98 says
 * 2. Defaults: avalanche id 0.
 *
 * Calls cAvalanche_triggerAvalanche with the avalanche id when cAvalanche_m_badCookie is 0.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_5C88 TriggerMiniAvalancheFunc(int n, Ent_5C88* params) {
    if (D_004A5868_5C88 == 0) {
        D_004A5868_5C88 = 1;
        D_004A5860_5C88 = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004A5864_5C88 = Flag_5C88(D_004A5864_5C88);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004A5860_5C88, 8);
    Ent_5C88* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3D98_5C88[k] && D_004A3D98_5C88[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
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
extern char *cWScriptMan_TheWScriptMan_5D90 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_5D90 __asm__("SSXApp");
static inline char *refToPtr_5D90(unsigned i) { return (char*)(i << 2); }
static inline char *conv_5D90(unsigned i) { if (i == 0) return 0; return refToPtr_5D90(i); }
extern "C" void cMoveNode_addAvaSpline_5D90(void*, void*) __asm__("cMoveNode_addAvaSpline");
struct O_5D90 { int p0, p1, p2; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual int v16(); };
struct V3_5D90 { float x, y, z; };
extern V3_5D90 luno_gLunoNil_5D90 __asm__("luno_gLunoNil");
struct Tag_5D90 { char c; Tag_5D90() {} Tag_5D90(const Tag_5D90 &o) : c(o.c) {} };
extern unsigned D_5D90 __asm__("D_004A5870");
extern Tag_5D90 D_004A5874;
extern int D_004A5878;
extern int D_004A3DA0;
extern "C" void* MEM_copy_5D90(void*, void*, int) __asm__("MEM_copy");
struct Ent_5D90 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[95]): puts a moving world instance on the avalanche spline.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (InstId) and lets each argument
 * override the field its index names, turning an int into a float where the field type table D_004A3DA0 says 2.
 * Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found and its active node (+0xC) is a move
 * node (virtual isMoveNode), cMoveNode_addAvaSpline gets the node and the instance (which it does not read).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_5D90 AddAvaSplineFunc(int n, Ent_5D90 *params) {
    if (D_004A5878 == 0) {
        D_5D90 = 0xFFFFFFFF;
        D_004A5878 = 1;
        D_004A5874 = Tag_5D90(D_004A5874);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_5D90(buf, &D_5D90, 8);
    Ent_5D90 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = (&D_004A3DA0)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_5D90 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section)
            inst = 0;
        else {
            i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_5D90(i);
            inst = t;
        }
        obj = inst;
    }
    if (obj) {
        O_5D90 *activeNode = *(O_5D90**)(obj + 0xC);
        if (activeNode) {
            if (activeNode->v16())
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
struct V3_5F40 { float x, y, z; };
extern V3_5F40 luno_gLunoNil_5F40 __asm__("luno_gLunoNil");
struct Tag_5F40 { char c; Tag_5F40() {} Tag_5F40(const Tag_5F40 &o) : c(o.c) {} };
struct Rec_5F40 {
    int a; int b; float c; float d; float e;
    float m[28];
    float v[16];
    int k; int l; float n; int o; float p; unsigned id;
    Tag_5F40 f;
};
extern Rec_5F40 D_4FBE58_5F40 __asm__("D_004FBE58");
extern int D_587C_5F40 __asm__("D_004A587C");
extern const int D_004467A0_5F40[] __asm__("D_004467A0");
extern "C" void* MEM_copy_5F40(void*, void*, int) __asm__("MEM_copy");
extern "C" void cAvalanche_setAvalancheNodeEmitterData_5F40(char*, Rec_5F40*) __asm__("cAvalanche_setAvalancheNodeEmitterData");
static inline char *refToPtr_5F40(unsigned i) { return (char*)(i << 2); }
static inline char *conv_5F40(unsigned i) { if (i == 0) return 0; return refToPtr_5F40(i); }
struct Ent_5F40 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[96]): sets the particle-emitter parameters of the avalanche node bound to a world
 * instance.
 *
 * Copies the static defaults (set up on the first call) into a 224-byte parameter block and lets each argument override
 * the field its index names, turning an int into a float where the field type table D_004467A0 says 2. The first 0xD8
 * bytes are emitter parameters in the layout of On Tour's tAddDynamicParticleData (NumParticles 1, Duration -1, Damp 1,
 * Size 4, ...); InstId sits at +0xD8 (default -1).
 *
 * When InstId is not -1 the instance is looked up (no current-instance shortcut; a missing one passes 0) and
 * cAvalanche_setAvalancheNodeEmitterData copies the parameters to its node.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_5F40 SetAvalancheEmitterDataFunc(int n, Ent_5F40 *params) {
    if (D_587C_5F40 == 0) {
        Rec_5F40 &defaultBlock = D_4FBE58_5F40;
        float zero = 0.0f;
        float one = 1.0f;
        defaultBlock.a = 1;
        defaultBlock.b = 0;
        defaultBlock.c = -1.0f;
        defaultBlock.d = one;
        defaultBlock.e = 4.0f;
        defaultBlock.m[0] = zero;
        defaultBlock.m[1] = zero;
        defaultBlock.m[2] = zero;
        defaultBlock.m[3] = zero;
        defaultBlock.m[4] = zero;
        defaultBlock.m[5] = zero;
        defaultBlock.m[6] = zero;
        defaultBlock.m[7] = zero;
        defaultBlock.m[8] = zero;
        defaultBlock.m[9] = zero;
        defaultBlock.m[10] = zero;
        defaultBlock.m[11] = zero;
        defaultBlock.m[12] = zero;
        defaultBlock.m[13] = zero;
        defaultBlock.m[14] = zero;
        defaultBlock.m[15] = zero;
        defaultBlock.m[16] = zero;
        defaultBlock.m[17] = zero;
        defaultBlock.m[18] = zero;
        defaultBlock.m[19] = zero;
        defaultBlock.m[20] = zero;
        defaultBlock.m[21] = zero;
        defaultBlock.m[22] = zero;
        defaultBlock.m[23] = zero;
        defaultBlock.m[24] = zero;
        defaultBlock.m[25] = zero;
        defaultBlock.m[26] = zero;
        defaultBlock.m[27] = zero;
        defaultBlock.v[0] = one;
        defaultBlock.v[1] = zero;
        defaultBlock.v[2] = zero;
        defaultBlock.v[3] = zero;
        defaultBlock.v[4] = one;
        defaultBlock.v[5] = one;
        defaultBlock.v[6] = one;
        defaultBlock.v[7] = one;
        defaultBlock.v[8] = one;
        defaultBlock.v[9] = zero;
        defaultBlock.v[10] = zero;
        defaultBlock.v[11] = zero;
        defaultBlock.v[12] = one;
        defaultBlock.v[13] = zero;
        defaultBlock.v[14] = zero;
        defaultBlock.v[15] = zero;
        defaultBlock.k = 16;
        defaultBlock.l = 0;
        defaultBlock.n = zero;
        defaultBlock.o = 1;
        defaultBlock.p = 20.0f;
        defaultBlock.id = 0xFFFFFFFF;
        D_587C_5F40 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        defaultBlock.f = Tag_5F40(defaultBlock.f);
    }
    Rec_5F40 rec;
    Rec_5F40 *buf = &rec;
    rec.id = 0xFFFFFFFF;
    MEM_copy_5F40(buf, &D_4FBE58_5F40, 0xE0);
    Ent_5F40 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_004467A0_5F40[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf->id;
    if (~id != 0) {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        char *obj;
        if (!section)
            obj = 0;
        else
            obj = conv_5F40(((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8);
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
// Reference sets of the current world (wscript/wscriptman.cpp layout). A reference id packs the set index (low 8 bits)
// and the slot; refs[slot] >> 8 is the object address >> 2.
// The script manager (cWScriptMan in the prototype, whose setBeing stores at 0x0); cWScriptMan_TheWScriptMan points at
// the instance.
extern cWScriptMan* cWScriptMan_TheWScriptMan;
// empty trailing member of a defaults record; value-init copies its byte through a stack temp
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
#define BIND_UNPACK_ARGS_1FBE38(vp, rec, defaults, size, types, n, args) do { \
        vp = rec; \
        MEM_copy(vp, defaults, size); \
        sBindArg_1FBE38* p_ = args; \
        for (int i_ = 0; i_ < n; i_++, p_++) { \
            int k_ = p_->idx; \
            int* d_ = (int*)((k_ << 2) + (int)vp); \
            if (p_->type != (types)[k_] && (types)[k_] == 2) \
                *(float*)d_ = (float)p_->val; \
            else \
                *d_ = p_->val; \
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
extern sBindEnd_1FBE38 D_004A5884_61B0 __asm__("D_004A5884");
/**
 * Script function (gLunoCFunctions[99]): returns whether a type of on-course icon is enabled by the custom race rules.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tIsIconEnabledData:
 * Type) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3DA8 says 2. Defaults: Type 0.
 *
 * Type 0, 1 and 2 test bits 6, 8 and 7 of the rule word D_005308D0 (rules 10, 12 and 11 of
 * cFEStateRuleSelect_updateBE): the int result in @p ret is 1 when the bit is clear, else 0. Other types give 1.
 *
 * @param ret  the result slot
 * @param n  the number of script arguments
 * @param e  the script arguments (each: field index, value, value type)
 * @return @p ret
 */
extern "C" cLunoValue* IsIconEnabledFunc(cLunoValue* ret, int n, sBindArg_1FBE38 *e) {
    if (D_004A5888_61B0 == 0) {
        // NOTE: guarded static-local init; the trailing empty member's value-init copies its byte through a stack temp
        //       (lbu; sb 0(sp)).
        D_004A5880_61B0 = 0;
        D_004A5884_61B0 = sBindEnd_1FBE38();
        D_004A5888_61B0 = 1;
    }
    int v[4];
    int *vp;
    BIND_UNPACK_ARGS_1FBE38(vp, v, &D_004A5880_61B0, 8, D_004A3DA8_61B0, n, e);
    int r;
    switch (vp[0]) {
    case 0: {
        int flags = D_005308D0_61B0[0] >> 6;
        r = 0;
        if ((flags & 1) == 0)
            r++;
        break;
    }
    case 1: {
        int flags = D_005308D0_61B0[0] >> 8;
        r = 0;
        if ((flags & 1) == 0)
            r++;
        break;
    }
    case 2: {
        int flags = D_005308D0_61B0[0] >> 7;
        r = 0;
        if ((flags & 1) == 0)
            r++;
        break;
    }
    default:
        r = 1;
        break;
    }
    BIND_SET_RESULT_1FBE38(ret, r);
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MessageCenterFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_6300 { char c; Flag_6300() {} Flag_6300(const Flag_6300& o) : c(o.c) {} };
struct Ent_6300 { int idx; int val; int pad; int kind; };
struct R12_6300 { int a; int b; int c; };
extern R12_6300 luno_gLunoNil_6300 __asm__("luno_gLunoNil");

extern int D_004A3DB0_6300[2] __asm__("D_004A3DB0");
extern "C" void func_001E3510(int, int);
extern "C" void* cBE_getInterface_6300(int, int) __asm__("cBE_getInterface__Fv");
extern char* SSXApp_6300 __asm__("SSXApp");
extern int D_004A588C_6300 __asm__("D_004A588C");
struct T12_6300 { int a; int b; Flag_6300 f; };
extern T12_6300 D_004FBF38_6300 __asm__("D_004FBF38");
/**
 * Script function (gLunoCFunctions[100]): shows a message-centre popup, except in a network game.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tMessageCenterData:
 * TitleId, MsgId) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3DB0 says 2. Defaults: TitleId 0, MsgId 0.
 *
 * Unless cBENetworkInterface_networkOpt.bIsNetworkGame is set, calls func_001E3510(TitleId, MsgId) (fe/fepopup3). The
 * cBE_getInterface call before it has no effect.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" R12_6300 MessageCenterFunc(int n, Ent_6300* params) {
    if (D_004A588C_6300 == 0) {
        D_004FBF38_6300.a = 0;
        D_004A588C_6300 = 1;
        D_004FBF38_6300.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FBF38_6300.f = Flag_6300(D_004FBF38_6300.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FBF38_6300, 12);
    Ent_6300* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3DB0_6300[k] && D_004A3DB0_6300[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    int x = vp[0];
    int y = vp[1];
    cBE_getInterface_6300(*(int*)(SSXApp_6300 + 0x78), 7);
    if (cBENetworkInterface_networkOpt.bIsNetworkGame == 0)
        func_001E3510(x, y);
    return luno_gLunoNil_6300;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DangerStartFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_6438 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_6438 __asm__("SSXApp");
extern "C" void cDangerMan_AddDangerElem_6438(void*, void*, float, float) __asm__("cDangerMan_AddDangerElem");
struct V3_6438 { float x, y, z; };
extern V3_6438 luno_gLunoNil_6438 __asm__("luno_gLunoNil");
struct Tag_6438 { char c; Tag_6438() {} Tag_6438(const Tag_6438 &o) : c(o.c) {} };
extern unsigned D_6438[] __asm__("D_004FBF48");
extern int D_004A5890;
extern const int D_00446880[];
extern "C" void* MEM_copy_6438(void*, void*, int) __asm__("MEM_copy");
struct Ent_6438 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[101]): registers a world instance as a danger the AI riders track.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tDangerStartData:
 * InstId, Radius, Delay) and lets each argument override the field its index names, turning an int into a float where
 * the field type table D_00446880 says 2. Defaults: InstId -1, Radius 1000, Delay 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_AddDangerElem on the AI's danger
 * manager (SSXApp->Game->AI +0xA8) records its id (+0x78), Radius, Delay and position, replacing any earlier record for
 * it (a dead instance is skipped).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_6438 DangerStartFunc(int n, Ent_6438 *params) {
    if (D_004A5890 == 0) {
        D_6438[0] = 0xFFFFFFFF;
        *(float*)&D_6438[1] = 1000.0f;
        ((int*)D_6438)[2] = 0;
        D_004A5890 = 1;
        (*(Tag_6438*)((char*)D_6438 + 12)) = Tag_6438((*(Tag_6438*)((char*)D_6438 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_6438(buf, D_6438, 16);
    Ent_6438 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446880[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_6438 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            obj = 0;
        else
            obj = (char*)(i << 2);
    }
    if (obj) {
        float radius = *(float*)&buf[1];
        float delay = *(float*)&buf[2];
        int instIdBuf[1];
        instIdBuf[0] = *(int*)(obj + 0x78);
        cDangerMan_AddDangerElem_6438(*(void**)(*(char**)(*(char**)(SSXApp_6438 + 0x84) + 0xC) + 0xA8), instIdBuf, radius, delay);
    }
    return luno_gLunoNil_6438;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DangerEndFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_65E8 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_65E8 __asm__("SSXApp");
static inline char *look_65E8(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_65E8 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

extern "C" void cDangerMan_RemoveDangerElem_65E8(void*, void*) __asm__("cDangerMan_RemoveDangerElem");
struct V3_65E8 { float x, y, z; };
extern V3_65E8 luno_gLunoNil_65E8 __asm__("luno_gLunoNil");
struct Tag_65E8 { char c; Tag_65E8() {} Tag_65E8(const Tag_65E8 &o) : c(o.c) {} };
extern unsigned D_65E8 __asm__("D_004A5898");
extern Tag_65E8 D_004A589C;
extern int D_004A58A0;
extern int D_004A3DB8;
extern "C" void* MEM_copy_65E8(void*, void*, int) __asm__("MEM_copy");
struct Ent_65E8 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[102]): stops tracking a world instance as a danger.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tDangerEndData: InstId)
 * and lets each argument override the field its index names, turning an int into a float where the field type table
 * D_004A3DB8 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_RemoveDangerElem on the AI's danger
 * manager (SSXApp->Game->AI +0xA8) deletes its tracked danger record (cDangerMan_DeleteTrackedDangerElemRecord).
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_65E8 DangerEndFunc(int n, Ent_65E8 *params) {
    if (D_004A58A0 == 0) {
        D_65E8 = 0xFFFFFFFF;
        D_004A58A0 = 1;
        D_004A589C = Tag_65E8(D_004A589C);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_65E8(buf, &D_65E8, 8);
    Ent_65E8 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3DB8)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    char *obj = look_65E8(buf[0]);
    if (obj) {
        int instIdBuf[1]; instIdBuf[0] = *(int*)(obj + 0x78);
        cDangerMan_RemoveDangerElem_65E8(*(void**)(*(char**)(*(char**)(SSXApp_65E8 + 0x84) + 0xC) + 0xA8), instIdBuf);
    }
    return luno_gLunoNil_65E8;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", EnableDangerZoneFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_6770 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_6770 __asm__("SSXApp");
static inline char *look_6770(unsigned id) {
    if (~id == 0)
        return *(char**)(cWScriptMan_TheWScriptMan_6770 + 0x290);
    char *s = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
    unsigned i;
    if (!s || (i = ((unsigned*)*(char**)(s + 0x1C))[id >> 8] >> 8) == 0)
        return 0;
    return (char*)(i << 2);
}

extern "C" void cDangerMan_AddDangerVolume_6770(void*, void*, int) __asm__("cDangerMan_AddDangerVolume");
struct V3_6770 { float x, y, z; };
extern V3_6770 luno_gLunoNil_6770 __asm__("luno_gLunoNil");
struct Tag_6770 { char c; Tag_6770() {} Tag_6770(const Tag_6770 &o) : c(o.c) {} };
extern unsigned D_6770[] __asm__("D_004FBF58");
extern int D_004A58A4;
extern int D_004A3DC0;
extern "C" void* MEM_copy_6770(void*, void*, int) __asm__("MEM_copy");
struct Ent_6770 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[103]): enables a danger zone around a world instance.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tEnableDangerZoneData:
 * InstId, Level) and lets each argument override the field its index names, turning an int into a float where the field
 * type table D_004A3DC0 says 2. Defaults: InstId -1, Level 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_AddDangerVolume on the AI's danger
 * manager (SSXApp->Game->AI +0xA8) adds a zone for its id (+0x78) with Level, or counts one more reference when the
 * zone is already active.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_6770 EnableDangerZoneFunc(int n, Ent_6770 *params) {
    if (D_004A58A4 == 0) {
        D_6770[0] = 0xFFFFFFFF;
        D_6770[1] = 0;
        D_004A58A4 = 1;
        (*(Tag_6770*)((char*)D_6770 + 8)) = Tag_6770((*(Tag_6770*)((char*)D_6770 + 8)));
    }
    unsigned buf64[3];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_6770(buf, D_6770, 12);
    Ent_6770 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3DC0)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_6770 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }

    if (obj) {
        int zoneLevel = buf[1];
        cDangerMan_AddDangerVolume_6770(*(void**)(*(char**)(*(char**)(SSXApp_6770 + 0x84) + 0xC) + 0xA8), obj + 0x78, zoneLevel);
    }
    return luno_gLunoNil_6770;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", DisableDangerZoneFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_6908 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_6908 __asm__("SSXApp");
extern "C" void cDangerMan_RemoveDangerVolume_6908(void*, void*) __asm__("cDangerMan_RemoveDangerVolume");
struct V3_6908 { float x, y, z; };
extern V3_6908 luno_gLunoNil_6908 __asm__("luno_gLunoNil");
struct Tag_6908 { char c; Tag_6908() {} Tag_6908(const Tag_6908 &o) : c(o.c) {} };
extern unsigned D_6908 __asm__("D_004A58A8");
extern Tag_6908 D_004A58AC;
extern int D_004A58B0;
extern int D_004A3DC8;
extern "C" void* MEM_copy_6908(void*, void*, int) __asm__("MEM_copy");
struct Ent_6908 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[104]): disables a danger zone around a world instance.
 *
 * Copies the static defaults (set up on the first call) into an 8-byte parameter block (On Tour tDisableDangerZoneData:
 * InstId) and lets each argument override the field its index names, turning an int into a float where the field type
 * table D_004A3DC8 says 2. Defaults: InstId -1.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, cDangerMan_RemoveDangerVolume on the AI's danger
 * manager (SSXApp->Game->AI +0xA8) drops one reference to its zone and frees the zone at zero.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_6908 DisableDangerZoneFunc(int n, Ent_6908 *params) {
    if (D_004A58B0 == 0) {
        D_6908 = 0xFFFFFFFF;
        D_004A58B0 = 1;
        D_004A58AC = Tag_6908(D_004A58AC);
    }
    unsigned buf64[2];
    unsigned *buf = buf64;
    buf[0] = 0xFFFFFFFF;
    MEM_copy_6908(buf, &D_6908, 8);
    Ent_6908 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = (&D_004A3DC8)[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[0];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_6908 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (obj)
        cDangerMan_RemoveDangerVolume_6908(*(void**)(*(char**)(*(char**)(SSXApp_6908 + 0x84) + 0xC) + 0xA8), obj + 0x78);
    return luno_gLunoNil_6908;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MultiParticleFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V3_6A90 { float x, y, z; };
extern V3_6A90 luno_gLunoNil_6A90 __asm__("luno_gLunoNil");
struct Tag_6A90 { char c; Tag_6A90() {} Tag_6A90(const Tag_6A90 &o) : c(o.c) {} };
struct Rec_6A90 {
    int a; int b; float c; float d; float e;
    float m[28];
    float v[16];
    int k; int l; float n; int o; float p; int q; int w;
    Tag_6A90 f;
};
extern Rec_6A90 D_4FBF68_6A90 __asm__("D_004FBF68");
extern int D_58B4_6A90 __asm__("D_004A58B4");
extern const int D_00446890_6A90[] __asm__("D_00446890");
extern "C" void* MEM_copy_6A90(void*, void*, int) __asm__("MEM_copy");
extern int cMultiParticleMan_TheMultiParticleMan_6A90 __asm__("cMultiParticleMan_TheMultiParticleMan");
extern "C" void cMultiParticleMan_addParticle_6A90(int, int, Rec_6A90*, int) __asm__("cMultiParticleMan_addParticle");
struct Ent_6A90 { int idx; int val; int pad; int type; };
/**
 * Script command MultiParticle: adds a particle system to the multi-particle manager.
 *
 * Copies the static tParticleData defaults (D_004FBF68, set once under D_004A58B4: identity colour/vectors, 16, 20.0,
 * 0x80, ...) into a local, applies the keyword arguments (types from D_00446890; int to float where the type is 2), and
 * calls cMultiParticleMan_addParticle(TheMultiParticleMan, data +0xD8, data, data +0xDC).
 *
 * @param n  the number of keyword arguments
 * @param params  the keyword arguments (index, value, type)
 * @return luno_gLunoNil (no value)
 */
extern "C" V3_6A90 MultiParticleFunc(int n, Ent_6A90 *params) {
    if (D_58B4_6A90 == 0) {
        Rec_6A90 &defaults = D_4FBF68_6A90;
        float zero = 0.0f;
        float one = 1.0f;
        defaults.a = 1;
        defaults.b = 0;
        defaults.c = -1.0f;
        defaults.d = one;
        defaults.e = 4.0f;
        defaults.m[0] = zero;
        defaults.m[1] = zero;
        defaults.m[2] = zero;
        defaults.m[3] = zero;
        defaults.m[4] = zero;
        defaults.m[5] = zero;
        defaults.m[6] = zero;
        defaults.m[7] = zero;
        defaults.m[8] = zero;
        defaults.m[9] = zero;
        defaults.m[10] = zero;
        defaults.m[11] = zero;
        defaults.m[12] = zero;
        defaults.m[13] = zero;
        defaults.m[14] = zero;
        defaults.m[15] = zero;
        defaults.m[16] = zero;
        defaults.m[17] = zero;
        defaults.m[18] = zero;
        defaults.m[19] = zero;
        defaults.m[20] = zero;
        defaults.m[21] = zero;
        defaults.m[22] = zero;
        defaults.m[23] = zero;
        defaults.m[24] = zero;
        defaults.m[25] = zero;
        defaults.m[26] = zero;
        defaults.m[27] = zero;
        defaults.v[0] = one;
        defaults.v[1] = zero;
        defaults.v[2] = zero;
        defaults.v[3] = zero;
        defaults.v[4] = one;
        defaults.v[5] = one;
        defaults.v[6] = one;
        defaults.v[7] = one;
        defaults.v[8] = one;
        defaults.v[9] = zero;
        defaults.v[10] = zero;
        defaults.v[11] = zero;
        defaults.v[12] = one;
        defaults.v[13] = zero;
        defaults.v[14] = zero;
        defaults.v[15] = zero;
        defaults.k = 16;
        defaults.l = 0;
        defaults.n = zero;
        defaults.o = 1;
        defaults.p = 20.0f;
        defaults.q = 0;
        defaults.w = 0x80;
        D_58B4_6A90 = 1;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        defaults.f = Tag_6A90(defaults.f);
    }
    Rec_6A90 rec;
    Rec_6A90 *buf = &rec;
    MEM_copy_6A90(buf, &D_4FBF68_6A90, 228);
    Ent_6A90 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int argType = D_00446890_6A90[idx];
        unsigned *field = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != argType && argType == 2)
            *(float*)field = (float)x->val;
        else
            *field = x->val;
    }
    cMultiParticleMan_addParticle_6A90(cMultiParticleMan_TheMultiParticleMan_6A90, buf->q, buf, buf->w);
    return luno_gLunoNil_6A90;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", MultiParticleInstFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char *cWScriptMan_TheWScriptMan_6CB0 __asm__("cWScriptMan_TheWScriptMan");
extern char *SSXApp_6CB0 __asm__("SSXApp");
extern void *cMultiParticleMan_TheMultiParticleMan_6CB0 __asm__("cMultiParticleMan_TheMultiParticleMan");
void cMultiParticleMan_addInst(void*, int, int);
void cMultiParticleMan_remInst(void*, int, int);
struct V3_6CB0 { float x, y, z; };
extern V3_6CB0 luno_gLunoNil_6CB0 __asm__("luno_gLunoNil");
struct Tag_6CB0 { char c; Tag_6CB0() {} Tag_6CB0(const Tag_6CB0 &o) : c(o.c) {} };
extern unsigned D_6CB0[] __asm__("D_004FC050");
extern int D_004A58B8;
extern const int D_00446970[];
extern "C" void* MEM_copy_6CB0(void*, void*, int) __asm__("MEM_copy");
struct Ent_6CB0 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[106]): attaches a world instance to a multi-particle effect, or detaches it.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tMultiParticleInstData:
 * Id, InstId, Cmd) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_00446970 says 2. Defaults: Id 0, InstId -1, Cmd 0.
 *
 * InstId -1 means the script's current instance (cWScriptMan_TheWScriptMan->Inst); otherwise its low byte picks a
 * world-cache section and the rest an instance in it. When the instance is found, Cmd 0 calls
 * cMultiParticleMan_addInst(cMultiParticleMan_TheMultiParticleMan, Id, the instance's id +0x78) and any other Cmd
 * cMultiParticleMan_remInst.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return the nil script value (luno_gLunoNil)
 */
extern "C" V3_6CB0 MultiParticleInstFunc(int n, Ent_6CB0 *params) {
    if (D_004A58B8 == 0) {
        ((int*)D_6CB0)[0] = 0;
        D_6CB0[1] = 0xFFFFFFFF;
        ((int*)D_6CB0)[2] = 0;
        D_004A58B8 = 1;
        (*(Tag_6CB0*)((char*)D_6CB0 + 12)) = Tag_6CB0((*(Tag_6CB0*)((char*)D_6CB0 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[1] = 0xFFFFFFFF;
    MEM_copy_6CB0(buf, D_6CB0, 16);
    Ent_6CB0 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int keywordType = D_00446970[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != keywordType && keywordType == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    unsigned id = buf[1];
    char *obj;
    if (~id == 0) {
        obj = *(char**)(cWScriptMan_TheWScriptMan_6CB0 + 0x290);
    } else {
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        unsigned i;
        char *inst;
        if (!section || (i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8) == 0)
            inst = 0;
        else
            inst = (char*)(i << 2);
        obj = inst;
    }
    if (obj) {
        if (buf[2] == 0)
            cMultiParticleMan_addInst(cMultiParticleMan_TheMultiParticleMan_6CB0, buf[0], *(int*)(obj + 0x78));
        else
            cMultiParticleMan_remInst(cMultiParticleMan_TheMultiParticleMan_6CB0, buf[0], *(int*)(obj + 0x78));
    }
    return luno_gLunoNil_6CB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", CheckGateFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cWScriptMan_TheWScriptMan_6E68 __asm__("cWScriptMan_TheWScriptMan");
extern "C" int cWScriptMan_checkGate_6E68(int, int, int, int) __asm__("cWScriptMan_checkGate");
extern "C" void *cAudioInterface_bxAudio_6E68() __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_WorldSound_6E68(void*, void*, int) __asm__("cSSXAudio_WorldSound");
int *luno_cLunoVar_castInt(void*);
static inline char *refToPtr_6E68(unsigned i) { return (char*)(i << 2); }
static inline char *conv_6E68(unsigned i) { if (i == 0) return 0; return refToPtr_6E68(i); }
struct V3_6E68 { int w0, w1, w2; };
struct Tag_6E68 { char c; Tag_6E68() {} Tag_6E68(const Tag_6E68 &o) : c(o.c) {} };
extern unsigned D_6E68[] __asm__("D_004FC060");
extern int D_004A58BC;
extern const int D_00446980[];
extern "C" void* MEM_copy_6E68(void*, void*, int) __asm__("MEM_copy");
struct Ent_6E68 { int idx; int val; int pad; int type; };
/**
 * Script function (gLunoCFunctions[107]): checks the script's rider against a gate (cWScriptMan_checkGate) and plays a
 * sound when it passed.
 *
 * Copies the static defaults (set up on the first call) into a 16-byte parameter block (On Tour tCheckGateData:
 * InstId1, InstId2, InstId3) and lets each argument override the field its index names, turning an int into a float
 * where the field type table D_00446980 says 2. Defaults: InstId1 -1, InstId2 -1, InstId3 -1.
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
extern "C" V3_6E68* CheckGateFunc(V3_6E68 *ret, int n, Ent_6E68 *params) {
    if (D_004A58BC == 0) {
        D_6E68[0] = 0xFFFFFFFF;
        D_6E68[1] = 0xFFFFFFFF;
        D_6E68[2] = 0xFFFFFFFF;
        D_004A58BC = 1;
        (*(Tag_6E68*)((char*)D_6E68 + 12)) = Tag_6E68((*(Tag_6E68*)((char*)D_6E68 + 12)));
    }
    unsigned buf64[4];
    unsigned *buf = buf64;
    buf64[0] = 0xFFFFFFFF;
    buf64[1] = 0xFFFFFFFF;
    buf64[2] = 0xFFFFFFFF;
    MEM_copy_6E68(buf, D_6E68, 16);
    Ent_6E68 *x = params;
    for (int i = 0; i < n; i++, x++) {
        int idx = x->idx;
        int t = D_00446980[idx];
        unsigned *slot = (unsigned*)((idx << 2) + (int)buf);
        if (x->type != t && t == 2)
            *(float*)slot = (float)x->val;
        else
            *slot = x->val;
    }
    int gateId = buf[1];
    int nextGateId = buf[2];
    int gateResult = cWScriptMan_checkGate_6E68(cWScriptMan_TheWScriptMan_6E68, buf[0], gateId, nextGateId);
    if (gateResult == 1) {
        unsigned id = buf[1];
        char *obj;
        char *section = (*(char***)((char*)(char*)cWorld_m_pTheWorld->m_pWorldCache + 8))[id & 0xFF];
        if (!section) {
            obj = 0;
        } else {
            unsigned i = ((unsigned*)*(char**)(section + 0x1C))[id >> 8] >> 8;
            char *t = conv_6E68(i);
            obj = t;
        }
        if (obj)
            cSSXAudio_WorldSound_6E68(cAudioInterface_bxAudio_6E68(), obj, 92);
    }
    ((int*)ret)[2] = 1;
    *luno_cLunoVar_castInt(ret) = gateResult;
    return ret;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", ShowMessageIconFunc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void MEM_copy(void*, void*, int);
int* luno_cLunoVar_castInt(void*);
struct Flag_7020 { char c; Flag_7020() {} Flag_7020(const Flag_7020& o) : c(o.c) {} };
struct Ent_7020 { int idx; int val; int pad; int kind; };
struct R12_7020 { int a; int b; int c; };
extern R12_7020 luno_gLunoNil_7020 __asm__("luno_gLunoNil");

extern int D_004A3DD0_7020[2] __asm__("D_004A3DD0");
extern "C" int func_001E3570(int, int);
extern int D_004A58C0_7020 __asm__("D_004A58C0");
struct T12_7020 { int a; int b; Flag_7020 f; };
extern T12_7020 D_004FC070_7020 __asm__("D_004FC070");
/**
 * Script function (gLunoCFunctions[110]): shows a message icon and returns whether it was shown.
 *
 * Copies the static defaults (set up on the first call) into a 12-byte parameter block (On Tour tShowMessageIconData:
 * TitleId, MsgId) and lets each argument override the field its index names, turning an int into a float where the
 * field type table D_004A3DD0 says 2. Defaults: TitleId 0, MsgId 0.
 *
 * Returns 1 when func_001E3570(TitleId, MsgId) (fe/fepopup3) returns non-zero, else 0, as an int in the hidden result
 * slot self.
 *
 * @param n  the number of script arguments
 * @param params  the script arguments (each: field index, value, value type)
 * @return self, set to int 1 or 0
 */
extern "C" void* ShowMessageIconFunc(void* self, int n, Ent_7020* params) {
    if (D_004A58C0_7020 == 0) {
        D_004FC070_7020.a = 0;
        D_004A58C0_7020 = 1;
        D_004FC070_7020.b = 0;
        // NOTE: guarded static-local init; the target copies the trailing byte through its copy ctor (lbu; sb back; sb
        //       0(sp)).
        D_004FC070_7020.f = Flag_7020(D_004FC070_7020.f);
    }
    int block[4] __attribute__((aligned(16)));
    int* vp = block;
    MEM_copy(vp, &D_004FC070_7020, 12);
    Ent_7020* curArg = params;
    for (int i = 0; i < n; i++, curArg++) {
        int k = curArg->idx;
        int* slot = (int*)((k << 2) + (int)vp);
        if (curArg->kind != D_004A3DD0_7020[k] && D_004A3DD0_7020[k] == 2)
            *(float*)slot = (float)curArg->val;
        else
            *slot = curArg->val;
    }
    bool iconShown = func_001E3570(vp[0], vp[1]) != 0;
    *(int*)((char*)self + 8) = 1;
    *luno_cLunoVar_castInt(self) = iconShown;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_cWScriptMission);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char cWScriptMission_vtbl_7128[] __asm__("cWScriptMission_vtbl");
extern "C" void* cWScriptProcess_cWScriptProcess(void* self, void* man, int a1, int a2);
extern "C" int cWScriptMan_getScriptCode_7128(int, int) __asm__("cWScriptMan_getScriptCode");
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
extern "C" void* cWScriptMission_cWScriptMission(cWScriptMission* self, void* man, int sectionId, char* data, int key) {
    cWScriptProcess_cWScriptProcess(self, man, sectionId, (int)data);
    self->mVtbl = (void**)cWScriptMission_vtbl_7128;
    self->Scope = *(int*)(data + 0x50);
    self->SortKey = key;
    for (int i = 0; i < 4; i++)
        ((int*)(&self->MissionScriptCode[0]))[i] = cWScriptMan_getScriptCode_7128((int)self->Manager, ((int*)(data + 0x40))[i]);
    return ((void*)self);
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
extern char *SSXApp_71F0 __asm__("SSXApp");
void cRider_flagEventMissionStart_71F0(void*) __asm__("cRider_flagEventMissionStart__FPv");
extern "C" void cWScriptProcess_processStart_71F0(void*) __asm__("cWScriptProcess_processStart");
struct Tab_71F0 { char pad[0x28]; void *arr[1]; };
/**
 * Starts a mission: shows HUD effect 0x1A for one second on the manager's rider (cRider_flagEventMissionStart), then runs
 * the base process start.
 */
extern "C" void cWScriptMission_processStart(cWScriptMission* self) {
    cRider_flagEventMissionStart_71F0((*(Tab_71F0**)(*(char**)(SSXApp_71F0 + 0x84) + 0xC))->arr[*(int*)self->Manager]);
    cWScriptProcess_processStart_71F0(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_processComplete);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct P_7240 { int state; char pad[0x58]; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); };
int cWScriptMan_isProcessActive_7240(void*, void*) __asm__("cWScriptMan_isProcessActive__FPvT0");
extern "C" void *cAudioInterface_bxAudio_7240(void) __asm__("cAudioInterface_bxAudio");
extern "C" void cSSXAudio_BigChallengeComplete_7240(void*) __asm__("cSSXAudio_BigChallengeComplete");
/**
 * Completes a mission that the script manager still lists: runs its virtual slots 6 and 18, then calls
 * cSSXAudio_BigChallengeComplete on the sound system.
 */
extern "C" void cWScriptMission_processComplete(cWScriptMission* self) {
    if (cWScriptMan_isProcessActive_7240(self->Manager, ((P_7240*)self))) {
        ((P_7240*)self)->v06();
        ((P_7240*)self)->v18();
        cSSXAudio_BigChallengeComplete_7240(cAudioInterface_bxAudio_7240());
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionRestart);
#ifdef SKIP_ASM
struct P_72B0 { int state; char pad[0x58]; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); };
/**
 * Restarts a mission script: ends it if it is running, then starts it again.
 *
 * When the state (+0x00) is 2 (running) it calls the process's virtual processEnd (vtable +0x30, cWScriptProcess_processEnd), then
 * processStart (vtable +0x20, cWScriptMission_processStart).
 */
extern "C" void cWScriptMission_missionRestart(P_72B0* self) {
    if (self->state == 2)
        self->v06();
    self->v04();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionPass);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char* SSXApp_7308 __asm__("SSXApp");
extern "C" int cBEMissionInterface_isMissionCompleted(void* missions, int id);
extern "C" void cBEMissionInterface_setIsCompleted(void* missions, int id, int done);
extern "C" int cBEMissionInterface_getDiffLevel(void* missions, int id);
extern "C" int cBEEconInterface_getMissionCash(void* econ, int reward);
extern "C" int cBENewRaceInterface_getLocationPeak1(void* iface);
extern "C" void cRider_flagEventMissionSuccess(void* rider, int player, int money);
extern "C" unsigned int cBEMissionInterface_getNextMissionID(void* missions, int id);
extern "C" void cBEMissionInterface_setIsShowIcon(void* missions, int id, int active);
extern "C" void cWScriptMan_runScriptCode(int, void *, int);
extern "C" int cBEMissionInterface_isMissionLocked(void* missions, int id);
extern "C" void cBEMissionInterface_setIsLocked(void* missions, int id, int a);
extern "C" void cBEMissionInterface_setIsReady(void* missions, int id, int a);
extern "C" void cWScriptMan_updateWorld(int script);
extern "C" void cWScriptMan_processPass(int script, void* self);
extern "C" void cReplay_reset(void* replay);
extern "C" void cReplay_startRace(void* replay);

struct sGame_7308 {
    char pad_0x0[0x28];
    void* riders[8];        // 0x28
};

/**
 * Handles a passed mission: records it and pays the reward, unlocks the next mission and runs the pass script.
 *
 * On BE interface 0xA (missions), for the mission ID (+0x20): if not yet completed, cBEMissionInterface_setIsCompleted marks it completed
 * and the reward is cBEEconInterface_getMissionCash (econ interface 0xB) of its cBEMissionInterface_getDiffLevel level. cRider_flagEventMissionSuccess flags the success on
 * the script's rider with the BE interface 0 value from cBENewRaceInterface_getLocationPeak1 and the money. If there is a next mission
 * (cBEMissionInterface_getNextMissionID != -1) this one's icon is hidden (cBEMissionInterface_setIsShowIcon), the pass script (+0x6C) runs
 * through cWScriptMan_runScriptCode, and a locked next mission is unlocked, made ready and shown (cBEMissionInterface_setIsLocked,
 * cBEMissionInterface_setIsReady, cBEMissionInterface_setIsShowIcon); with no next mission only the pass script runs. Ends with cWScriptMan_updateWorld
 * and cWScriptMan_processPass on the script manager (+0x10), then cReplay_reset and cReplay_startRace on
 * SSXApp->Game->Replay.
 *
 * On Tour also awards leaderboard points and a medal and triggers an overlay; retail has none of that.
 */
// Mission completed: pays the reward the first time, unlocks the follow-up mission and resets the script.
extern "C" void cWScriptMission_missionPass(cWScriptMission* self)
{
    int money = 0;
    void* missions = cBE_getInterface_Fv(cBE_getBE(), 10);
    if (cBEMissionInterface_isMissionCompleted(missions, (int)self->ProcessId) == 0) {
        cBEMissionInterface_setIsCompleted(missions, (int)self->ProcessId, 1);
        int reward = cBEMissionInterface_getDiffLevel(missions, (int)self->ProcessId);
        money = cBEEconInterface_getMissionCash(cBE_getInterface_Fv(cBE_getBE(), 11), reward);
    }
    int pl = cBENewRaceInterface_getLocationPeak1(cBE_getInterface_Fv(cBE_getBE(), 0));
    sGame_7308* game = *(sGame_7308**)(*(char**)(SSXApp_7308 + 0x84) + 0xC);
    cRider_flagEventMissionSuccess(game->riders[*(int*)self->Manager], pl, money);
    unsigned int next = cBEMissionInterface_getNextMissionID(missions, (int)self->ProcessId);
    if (next != 0xFFFFFFFF) {
        cBEMissionInterface_setIsShowIcon(missions, (int)self->ProcessId, 0);
        if ((int)self->MissionScriptCode[1] != 0) {
            cWScriptMan_runScriptCode((int)self->Manager, ((char*)self), (int)self->MissionScriptCode[1]);
        }
        if (cBEMissionInterface_isMissionLocked(missions, next) != 0) {
            cBEMissionInterface_setIsLocked(missions, next, 0);
            cBEMissionInterface_setIsReady(missions, next, 1);
            cBEMissionInterface_setIsShowIcon(missions, next, 1);
        }
    } else if ((int)self->MissionScriptCode[1] != 0) {
        cWScriptMan_runScriptCode((int)self->Manager, ((char*)self), (int)self->MissionScriptCode[1]);
    }
    cWScriptMan_updateWorld((int)self->Manager);
    cWScriptMan_processPass((int)self->Manager, ((char*)self));
    cReplay_reset(*(void**)(*(char**)(SSXApp_7308 + 0x84) + 0x28));
    cReplay_startRace(*(void**)(*(char**)(SSXApp_7308 + 0x84) + 0x28));
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
extern "C" void cWScriptMan_runScriptCode(int, void *, int);
struct P_74C0 { int state; char pad[0x58]; virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); };
/**
 * Handles a failed mission: makes it available again, runs the fail script and ends the process.
 *
 * If the mission (+0x20) is not available on BE interface 0xA, cBEMissionInterface_setIsAvailable sets it available. The fail script (+0x70)
 * runs through cWScriptMan_runScriptCode if there is one; otherwise the virtual at vtable +0xA0 (cWScriptMission_missionUpdateWorld,
 * missionUpdateWorld) runs. A running process (state 2) is then ended with cWScriptProcess_end.
 */
extern "C" void cWScriptMission_missionFail(P_74C0* self) {
    int iface = cBE_getInterface_Fv_74C0(cBE_getBE_74C0(), 10);
    if (!cBEMissionInterface_isMissionAvailable_74C0(iface, *(int*)((char*)self + 0x20)))
        cBEMissionInterface_setIsAvailable_74C0(iface, *(int*)((char*)self + 0x20), 1);
    int failScript = *(int*)((char*)self + 0x70);
    if (failScript)
        cWScriptMan_runScriptCode(*(int*)((char*)self + 0x10), self, failScript);
    else
        self->v20();
    if (self->state == 2)
        cWScriptProcess_end_74C0(self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1FBE38", cWScriptMission_missionUpdateWorld);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_runScriptCode(int, void *, int);

/**
 * Runs the mission's update-world script, if it has one.
 *
 * Calls cWScriptMan_runScriptCode(script manager +0x10, this, code) with the fourth script entry point (+0x74).
 * cWScriptMission_missionFail calls it through the vtable when the mission has no fail script.
 *
 * @param mission  the mission (this)
 */
extern "C" void cWScriptMission_missionUpdateWorld(cWScriptMission* mission) {
    int temp_6;

    temp_6 = ((int)mission->MissionScriptCode[3]);
    if (temp_6 != 0) {
        cWScriptMan_runScriptCode(((int)mission->Manager), mission, temp_6);
    }
}
#endif
