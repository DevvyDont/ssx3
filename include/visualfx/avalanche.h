#ifndef VISUALFX_AVALANCHE_H
#define VISUALFX_AVALANCHE_H

// cAvalanche: the avalanche visual effect - recorded avalanches (a list of tAvalanche, each a list of tAvalancheNode:
// an instance of the track that falls, with its packed per-frame history from the avalanche data file) played back by
// up to 16 tActiveAvalanche slots that drive 64 tActiveAvalancheNode (position, rotation, colour and a dust emitter per
// falling instance). A cVisualEffect: the object holds only the vptr; everything else is static (On Tour class
// cAvalanche, size 0x4, ~40 static members). EA's source: d:/ssx/code/source/visualfx/avalanche.cpp (preview assert
// path); retail: splat unit visualfx/avalanche (0x002D5778 .. 0x002DA1C0), the packed-frame setters and
// ~tActiveAvalancheNode in visualfx/angerfx (0x002D5598 .. 0x002D5750), the static-member getters (Enable() ..
// Seed(), fileDataDeleteable, isBadCookie) and the 4-byte ctor cAvalanche_cAvalanche in visualfx/worldlightman.
//
// Sizes:
//   cAvalanche            0x4   [R] cPS2AIVisualEffectsMan_cPS2AIVisualEffectsMan: cMemMan_alloc(4, "Avalanche"), stores
//                               cAvalanche_vtbl at +0 and calls cAvalanche_init (cAvalanche_cAvalanche does the same);
//                               [O] 0x4.
//   tAvalanche            0x210 [R] cAvalanche_addAvalancheNode: cMemMan_alloc(0x210, "Avalanche"); [O] 0x210.
//   tAvalancheNode        0x120 [R] cAvalanche_addAvalancheNode: cMemMan_alloc(0x120, "AvalancheNode"); [O] 0x120.
//   tActiveAvalanche      0x1C  [U] cAvalanche_m_activeAvalanches is walked with stride 0x1C, 16 slots
//                               (write/readFromReplayFrame, triggerAvalanche, end); [O] 0x1C.
//   tActiveAvalancheNode  0x2F0 [R] cAvalanche__cAvalanche purges the 64 nodes' emitters at cAvalanche_m_activeNodes + 0xD0
//                               stepping 0x2F0 (0xBC00 bytes); [O] 0x2F0.
//   tAvalanchePackedFrameData 0xA [U] cAvalanche_resolveDataPointers advances its file cursor by m_life * 10 per node;
//                               the three setters in visualfx/angerfx write bytes 0..9; [O] 0xA.
// Base: cAvalanche derives from cVisualEffect (vptr only; its vtable cVisualEffect_vtbl is restored by cAvalanche__cAvalanche);
// the t* structs have no bases and no vtables.
// Vtable cAvalanche_vtbl (D_00488648, 5 slots): 1 dtor cAvalanche__cAvalanche, 2 cAvalanche_update, 3 cAvalanche_render,
// 4 cVisualEffect_deferredRender, 5 cAvalanche_init.
//
// Statics (not declared here; the .cpp files declare them with differing types): cAvalanche_m_activeNodes
// (0x004EE770, tActiveAvalancheNode[64]), cAvalanche_m_activeAvalanches (0x00538938, tActiveAvalanche[16]),
// cAvalanche_m_nodeList (0x00538450, tAvalancheNode*[256]), cAvalanche_m_avalancheList (tAvalanche*),
// cAvalanche_m_filedata, m_resolved, m_fileDataDeleteable, m_numNodes, m_badCookie and the m_Enable .. m_Seed tunables.
//
// A FLAT layout, like main/game.h. Tags: [R] retail allocations / constructors; [U] how retail code uses the field;
// [O] SSX On Tour's DWARF (all six types; the same sizes and, here, the same offsets: no bools before the vectors).
// [P]: the preview keeps only cAvalanche's static getters (isBadCookie, Seed, ...); layout_xlate.py sees only the vptr.

#include "layout.h"
#include "bx/math3d.h"
#include "render/particle.h"   /* cDynamicColourEmitter */

struct tInstanceRecord;
struct tAvalanche;
struct tAvalancheNode;
struct tActiveAvalancheNode;

struct cAvalanche {
    // Constants (names ours). [U] cAvalanche_SaveData writes kFileMagic as the .sav file's first word,
    // cAvalanche_LoadData and cAvalanche_resolve reject data without it; cAvalanche_triggerAvalanche fills at most
    // kMaxActiveNodes tActiveAvalancheNodes (cAvalanche_m_activeNodes[64]).
    enum { kFileMagic = 0x2BEEF00, kMaxActiveNodes = 64 };

    // --- cVisualEffect (vptr only)
    void** mVtbl;                   // 0x000 [R] cAvalanche_vtbl (cAvalanche_cAvalanche, the allocation above);
                                    //       cAvalanche__cAvalanche stores it, then cVisualEffect's cVisualEffect_vtbl

    // Methods bound to their retail symbols by asm labels.
    void reset() __asm__("cAvalanche_reset");
};

CHECK_OFFSET(cAvalanche, mVtbl, 0x000);
CHECK_SIZE(cAvalanche, 0x004);

// One frame of a node's recorded history (file data, 10 bytes per frame).
struct tAvalanchePackedFrameData {
    signed char m_x;                // 0x000 [O]; [U] setPositionOffset (sb)
    signed char m_y;                // 0x001 [O]; [U] ditto
    signed char m_z;                // 0x002 [O]; [U] ditto
    unsigned char m_r;              // 0x003 [O]; [U] setColour (sb); cAvalanche_getColour (lbu)
    unsigned char m_g;              // 0x004 [O]; [U] ditto
    unsigned char m_b;              // 0x005 [O]; [U] ditto
    signed char m_rx;               // 0x006 [O]; [U] setRotation (sb)
    signed char m_ry;               // 0x007 [O]; [U] ditto
    signed char m_rz;               // 0x008 [O]; [U] ditto
    unsigned char m_rs;             // 0x009 [O]; [U] ditto

    // Methods bound to their retail symbols by asm labels.
    void setPositionOffset(float red, float green, float blue) __asm__("tAvalanchePackedFrameData_setPositionOffset");
    void setColour(float red, float green, float blue) __asm__("tAvalanchePackedFrameData_setColour");
};

CHECK_OFFSET(tAvalanchePackedFrameData, m_x, 0x000);
CHECK_OFFSET(tAvalanchePackedFrameData, m_y, 0x001);
CHECK_OFFSET(tAvalanchePackedFrameData, m_z, 0x002);
CHECK_OFFSET(tAvalanchePackedFrameData, m_r, 0x003);
CHECK_OFFSET(tAvalanchePackedFrameData, m_g, 0x004);
CHECK_OFFSET(tAvalanchePackedFrameData, m_b, 0x005);
CHECK_OFFSET(tAvalanchePackedFrameData, m_rx, 0x006);
CHECK_OFFSET(tAvalanchePackedFrameData, m_ry, 0x007);
CHECK_OFFSET(tAvalanchePackedFrameData, m_rz, 0x008);
CHECK_OFFSET(tAvalanchePackedFrameData, m_rs, 0x009);
CHECK_SIZE(tAvalanchePackedFrameData, 0x00A);

// One unpacked avalanche animation frame (On Tour struct tAvalancheFrameData, 0x60; On Tour also gives it an empty
// ctor). [O] members, types and offsets; [R] the retail frame unpack in visualfx/avalanche (its view tFrameData_5778:
// position at 0x0, colour at 0x10, rotation matrix at 0x20, the same offsets) expands a tAvalanchePackedFrameData
// into one.
struct tAvalancheFrameData {
    t4Vector m_position;            // 0x00 [O]; [R] the unpacked position
    t4Vector m_colour;              // 0x10 [O]; [R] the unpacked colour
    t4Matrix m_rotation;            // 0x20 [O]; [R] the unpacked rotation
};

CHECK_OFFSET(tAvalancheFrameData, m_position, 0x00);
CHECK_OFFSET(tAvalancheFrameData, m_colour, 0x10);
CHECK_OFFSET(tAvalancheFrameData, m_rotation, 0x20);
CHECK_SIZE(tAvalancheFrameData, 0x60);

struct tAvalancheCollision {
    float m_time;                   // 0x000 [O]; [U] resolveDataPointers reads it from the file
    tInstanceRecord* m_inst;        // 0x004 [O]; [U] resolveDataPointers clears it
};

CHECK_OFFSET(tAvalancheCollision, m_time, 0x000);
CHECK_OFFSET(tAvalancheCollision, m_inst, 0x004);
CHECK_SIZE(tAvalancheCollision, 0x008);

struct tAvalancheGroundCollision {
    unsigned short m_time;          // 0x000 [O]; [U] resolveDataPointers (the file's second half-word: sh 0x0 after
                                    //       sh 0x2); readFromReplayFrame compares it with the time
    unsigned short m_nodeId;        // 0x002 [O]; [U] resolveDataPointers (the file's first half-word)
};

CHECK_OFFSET(tAvalancheGroundCollision, m_time, 0x000);
CHECK_OFFSET(tAvalancheGroundCollision, m_nodeId, 0x002);
CHECK_SIZE(tAvalancheGroundCollision, 0x004);

// A recorded avalanche (one per id; cAvalanche_m_avalancheList links them through m_next).
struct tAvalanche {
    int m_id;                       // 0x000 [O]; [R] addAvalancheNode (inlined init(id)); [U] the id searches
    tAvalancheNode* m_nodeList;     // 0x004 [O]; [R] zeroed; [U] addAvalancheNode pushes the new node; calculate / getNode walk it
    tAvalanche* m_next;             // 0x008 [O]; [R] links the new record at the list head
    unsigned short m_numCollisions; // 0x00C [O]; [R] zeroed (sh); [U] resolveDataPointers
    unsigned short m_numGroundCollisions; // 0x00E [O]; [R] zeroed (sh); [U] addGroundCollision (lh/sh), resolveDataPointers
    tAvalancheCollision m_collisions[32]; // 0x010 [O]; [U] resolveDataPointers
    tAvalancheGroundCollision m_groundCollisions[64]; // 0x110 [O]; [U] addGroundCollision (+0x110 / +0x112), resolveDataPointers

    // Methods bound to their retail symbols by asm labels.
    void calculate() __asm__("tAvalanche_calculate");
    tAvalancheNode* getNode(tInstanceRecord* id) __asm__("tAvalanche_getNode");
    void addGroundCollision(int time, tAvalancheNode* stop) __asm__("tAvalanche_addGroundCollision");
};

CHECK_OFFSET(tAvalanche, m_id, 0x000);
CHECK_OFFSET(tAvalanche, m_nodeList, 0x004);
CHECK_OFFSET(tAvalanche, m_next, 0x008);
CHECK_OFFSET(tAvalanche, m_numCollisions, 0x00C);
CHECK_OFFSET(tAvalanche, m_numGroundCollisions, 0x00E);
CHECK_OFFSET(tAvalanche, m_collisions, 0x010);
CHECK_OFFSET(tAvalanche, m_groundCollisions, 0x110);
CHECK_SIZE(tAvalanche, 0x210);

// One falling instance of a recorded avalanche (cAvalanche_m_nodeList[id]; the instance's flags at +0x92 keep id << 8).
struct t4Vector;

struct tAvalancheNode {
    tParticleData m_data;           // 0x000 [O] tParticleData (render/particle.h, 0xD8); [U] setAvalancheNodeEmitterData
                                    //       copies the 0xD8-byte record in when the node gets an emitter
    char unk_0D8[0x8];              // 0x0D8 alignment gap before the 16-aligned m_start (On Tour has it too)
    t4Vector m_start;               // 0x0E0 [O]; [U] resolveDataPointers stores (x, y, z, 1.0) from the file; calculate (lq)
    // m_baseType values (names ours). [U] tAvalancheNode_calculate gives only kBase_Bounce nodes the random bounce
    // after a ground hit (cAvalanche_m_RandomBounceChance); a kBase_Permanent node never fades out (calculateScale) or
    // ends (tActiveAvalancheNode_update / end), gets no emitter and is what getAvalancheDataByPosition reports.
    enum eBaseType { kBase_Bounce = 0, kBase_NoBounce = 1, kBase_Permanent = 2 };
    // cAvalanche_addAvalancheNode's script type (names ours, by the base it gives): 2 bounces, 5 is permanent.
    enum { kScriptType_Bounce = 2, kScriptType_Permanent = 5 };
    unsigned short m_baseType;      // 0x0F0 [O] eBaseType; [R] addAvalancheNode: 2 / 0 / 1 from the instance type;
                                    //       [U] calculate (lhu)
    unsigned char m_hasEmitter;     // 0x0F2 [O]; [R] zeroed; [U] setAvalancheNodeEmitterData sets it
    unsigned char m_historyDeleteable; // 0x0F3 [O]; [R] zeroed; [U] resolveDataPointers frees m_history when set; calculate
    tAvalancheNode* m_next;         // 0x0F4 [O]; [R] pushed on tAvalanche::m_nodeList
    tInstanceRecord* m_instance;    // 0x0F8 [O]; [R] addAvalancheNode's instance (it reads its bound box at +0x60..+0x74 and
                                    //       flags at +0x92); [U] setAvalancheNodeEmitterData matches it, calculate
    tAvalanchePackedFrameData* m_history; // 0x0FC [O]; [R] zeroed; [U] resolveDataPointers points it into the file;
                                    //       cAvalanche_getColour reads frame colours from it
    float m_life;                   // 0x100 [O]; [R] life * 30 (frames); [U] resolveDataPointers: frame count
    float m_fadein;                 // 0x104 [O]; [R] fadeIn * 30
    float m_fadeout;                // 0x108 [O]; [R] fadeOut * 30; [U] calculate
    float m_speed;                  // 0x10C [O]; [R] the nodeSpeed argument
    float m_radius;                 // 0x110 [O]; [R] 0.2 * the smallest bound-box extent; [U] calculate
    tActiveAvalancheNode* m_activeNode; // 0x114 [O]; [R] zeroed; [U] cAvalanche_getColour reads its m_colour (+0xC0)
    char unk_118[0x8];              // 0x118 tail padding to the 16-byte-aligned size 0x120

    // Methods bound to their retail symbols by asm labels.
    void fixVelocity(t4Vector* velocity) __asm__("tAvalancheNode_fixVelocity");
};

CHECK_OFFSET(tAvalancheNode, m_data, 0x000);
CHECK_OFFSET(tAvalancheNode, unk_0D8, 0x0D8);
CHECK_OFFSET(tAvalancheNode, m_start, 0x0E0);
CHECK_OFFSET(tAvalancheNode, m_baseType, 0x0F0);
CHECK_OFFSET(tAvalancheNode, m_hasEmitter, 0x0F2);
CHECK_OFFSET(tAvalancheNode, m_historyDeleteable, 0x0F3);
CHECK_OFFSET(tAvalancheNode, m_next, 0x0F4);
CHECK_OFFSET(tAvalancheNode, m_instance, 0x0F8);
CHECK_OFFSET(tAvalancheNode, m_history, 0x0FC);
CHECK_OFFSET(tAvalancheNode, m_life, 0x100);
CHECK_OFFSET(tAvalancheNode, m_fadein, 0x104);
CHECK_OFFSET(tAvalancheNode, m_fadeout, 0x108);
CHECK_OFFSET(tAvalancheNode, m_speed, 0x10C);
CHECK_OFFSET(tAvalancheNode, m_radius, 0x110);
CHECK_OFFSET(tAvalancheNode, m_activeNode, 0x114);
CHECK_OFFSET(tAvalancheNode, unk_118, 0x118);
CHECK_SIZE(tAvalancheNode, 0x120);

// A playing avalanche (cAvalanche_m_activeAvalanches[16]).
struct tActiveAvalanche {
    tAvalanche* m_avalanche;        // 0x000 [O]; [U] update / end / render; writeToReplayFrame saves m_avalanche->m_id
    tActiveAvalancheNode* m_activeNodeList; // 0x004 [O]; [U] buildArray / update / end walk it (m_next at +0x2E4)
    tActiveAvalancheNode** m_activeNodeArray; // 0x008 [O]; [U] buildArray allocates and fills it; end frees it
    int m_frame;                    // 0x00C [O] (no retail access through the struct seen)
    int m_nextCollision;            // 0x010 [O]; [U] update; triggerAvalanche / readFromReplayFrame zero it
    int m_nextGroundCollision;      // 0x014 [O]; [U] ditto
    float m_currTime;               // 0x018 [O]; [U] update / render (lwc1); writeToReplayFrame saves it

    // Methods bound to their retail symbols by asm labels.
    void buildArray() __asm__("tActiveAvalanche_buildArray");
    void render() __asm__("tActiveAvalanche_render");
    void update() __asm__("tActiveAvalanche_update");
};

CHECK_OFFSET(tActiveAvalanche, m_avalanche, 0x000);
CHECK_OFFSET(tActiveAvalanche, m_activeNodeList, 0x004);
CHECK_OFFSET(tActiveAvalanche, m_activeNodeArray, 0x008);
CHECK_OFFSET(tActiveAvalanche, m_frame, 0x00C);
CHECK_OFFSET(tActiveAvalanche, m_nextCollision, 0x010);
CHECK_OFFSET(tActiveAvalanche, m_nextGroundCollision, 0x014);
CHECK_OFFSET(tActiveAvalanche, m_currTime, 0x018);
CHECK_SIZE(tActiveAvalanche, 0x01C);

// A playing node (cAvalanche_m_activeNodes[64]).
struct tActiveAvalancheNode {
    int m_lastKeyFrame;             // 0x000 [O]; [U] getFrameData (lw), triggerAvalanche
    char unk_004[0xC];              // 0x004 alignment gap before the 16-aligned vector
    t4Vector m_lastKeyFramePosition; // 0x010 [O]; [U] getFrameData, triggerAvalanche (lq)
    t4Matrix m_lastKeyFrameRotation; // 0x020 [O]; [U] getFrameData (copied word by word), triggerAvalanche (lqc2)
    t4Vector m_position;            // 0x060 [O]; [U] update (sq), triggerAvalanche
    t4Matrix m_rotation;            // 0x070 [O]; [U] update (lqc2 x4), cAvalanche_getTransform
    float m_objectScale;            // 0x0B0 [O]; [U] calculateScale (swc1)
    float m_emitterAlphaScale;      // 0x0B4 [O]; [U] calculateScale, update, getFrameData
    char unk_0B8[0x8];              // 0x0B8 alignment gap before the 16-aligned vector
    t4Vector m_colour;              // 0x0C0 [O]; [U] update (sq); cAvalanche_getColour reads +0xC0..+0xCC
    cDynamicColourEmitter m_emitter; // 0x0D0 [O]; [R] ~cAvalanche purges it (cDynamicColourEmitter_purge on + 0xD0);
                                    //       [U] ~tActiveAvalancheNode, update / getFrameData (its +0x12C), triggerAvalanche
    tAvalancheNode* m_node;         // 0x2E0 [O]; [U] calculateScale / update / end / renderLines / getFrameData
    tActiveAvalancheNode* m_next;   // 0x2E4 [O]; [U] triggerAvalanche links the slot's nodes
    unsigned int m_endFrame;        // 0x2E8 [O] (no retail access through the struct seen)
    int m_updateEmitters;           // 0x2EC [O] bool (a word in retail); [U] getFrameData, triggerAvalanche (lw/sw)

    // Methods bound to their retail symbols by asm labels.
    void calculateScale(float currTime) __asm__("tActiveAvalancheNode_calculateScale");
    bool update(float currTime) __asm__("tActiveAvalancheNode_update");
    void end(int force) __asm__("tActiveAvalancheNode_end");
    void renderLines() __asm__("tActiveAvalancheNode_renderLines");
};

CHECK_OFFSET(tActiveAvalancheNode, m_lastKeyFrame, 0x000);
CHECK_OFFSET(tActiveAvalancheNode, unk_004, 0x004);
CHECK_OFFSET(tActiveAvalancheNode, m_lastKeyFramePosition, 0x010);
CHECK_OFFSET(tActiveAvalancheNode, m_lastKeyFrameRotation, 0x020);
CHECK_OFFSET(tActiveAvalancheNode, m_position, 0x060);
CHECK_OFFSET(tActiveAvalancheNode, m_rotation, 0x070);
CHECK_OFFSET(tActiveAvalancheNode, m_objectScale, 0x0B0);
CHECK_OFFSET(tActiveAvalancheNode, m_emitterAlphaScale, 0x0B4);
CHECK_OFFSET(tActiveAvalancheNode, unk_0B8, 0x0B8);
CHECK_OFFSET(tActiveAvalancheNode, m_colour, 0x0C0);
CHECK_OFFSET(tActiveAvalancheNode, m_emitter, 0x0D0);
CHECK_OFFSET(tActiveAvalancheNode, m_node, 0x2E0);
CHECK_OFFSET(tActiveAvalancheNode, m_next, 0x2E4);
CHECK_OFFSET(tActiveAvalancheNode, m_endFrame, 0x2E8);
CHECK_OFFSET(tActiveAvalancheNode, m_updateEmitters, 0x2EC);
CHECK_SIZE(tActiveAvalancheNode, 0x2F0);

#endif /* VISUALFX_AVALANCHE_H */
