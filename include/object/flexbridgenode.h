#ifndef OBJECT_FLEXBRIDGENODE_H
#define OBJECT_FLEXBRIDGENODE_H

// cFlexBridgeNode: the scene node of a flexible rope bridge: a row of spring nodes (cSpring Mesh, one tSpringNode per
// plank column) that riders push down and twist (applyForce), drawn as a Width x 2 grid mesh (cGridMesh: two rows of
// Width vertices, (Width - 1) * 2 triangles) and kept in the world's Nash space hash with a bound box grown by 100 on
// each side. Node type 5. Built by the world script's MakeFlexBridgeFunc (tMakeFlexBridgeData) and the replay loader.
// EA's source: d:/ssx/code/source/object/flexbridgenode.cpp (preview assert path). Retail methods: ctors/dtor at the
// end of the object/effectlink splat unit (0x00346350..0x00346568), the rest in object/flexbridgenode (0x003466A0..
// 0x00347D38) and four one-liners in object/railmodifier (0x00361558..0x00361570).
// Size 0xB0: [R] cMemMan_alloc(0xB0, "FlexBridge") in cReplay_restoreObject; BoundBox (0x20) ends at 0xB0
// (dtor/setupGrid lq/sq 0x90 and 0xA0); [O] On Tour 0xB0 with the same member offsets (its smaller cObjNode is absorbed
// by the 16-byte alignment of cFlexBridgeNodeState); [P] the preview's paired methods use the same offsets (0xC ..
// 0xA0).
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h): the ctors call
// cInstanceNode_cInstanceNode1 (bucket, type 5, record) / cInstanceNode_cInstanceNode directly - no cMoveNode (nothing
// at 0x1C). The state block cFlexBridgeNodeState (On Tour 0x30) follows at 0x20; its vptr is cInstanceNode's at 0xC.
// Constructors: cFlexBridgeNode_cFlexBridgeNode (bucket, record, tMakeFlexBridgeData*: RailHeight 100, TextureId 12,
// cSpring_setupSpring(Mesh, data, the record's bound-box diagonal), setupGrid, initSpringNodes, updateGrid; the
// record's flags move bit 1 -> bit 2 and bit 5 -> bit 6) and cFlexBridgeNode_cFlexBridgeNode1 (bucket, cBxReplayFrame*:
// cSpring_cSpring1 on +0x50, setupGrid, reads the 0x30-byte state into +0x20, updateGrid). Destructor
// cFlexBridgeNode__cFlexBridgeNode. Vtable cFlexBridgeNode_vtbl (0x00490270, 46 slots: 1 dtor, 2 update, 4 render, 12
// writeToReplayFrame, 19 getNodeState, 38 hasIntersect, 39 intersect, 42 updateIntersectInfo, 43 applyForce, 44
// hasBoundBox, 45 getBoundBox; the rest are cInstanceNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/effectlink.cpp) and the 0xB0 allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py: 9 paired methods, preview == retail at 0xC, 0x18, 0x68, 0x74,
//       0x78, 0x7C, 0x80, 0x84, 0x90, 0xA0 (no accessors);
//   [U] how retail code uses the field (src/object/flexbridgenode.cpp, railmodifier.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cFlexBridgeNodeState, cFlexBridgeNode, cSpringState, cSpring).

#include "layout.h"
#include "object/instancenode.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cObjNode;
struct tInstanceRecord;
struct tSpringNode;
struct cGridMesh;

// A spring's parameters (On Tour struct tSpringData, 0x20; same offsets): the tail of the world script's
// tMakeFlexBridgeData. [O] On Tour's cSpring::setupSpring(const tSpringData& sdata, float len); [U]
// cSpring_setupSpring (view sSpringDesc_3E08) copies LSpring .. ForceScale into cSpring's members of the same names,
// TorqueScale * 1e-6 into TorqueScale, LengthScale into Length and Width into Width.
struct tSpringData {
    float LSpring;                  // 0x00 [O]; [U] -> LSpring
    float LDamp;                    // 0x04 [O]; [U] -> LDamp
    float ASpring;                  // 0x08 [O]; [U] -> ASpring
    float ADamp;                    // 0x0C [O]; [U] -> ADamp
    float ForceScale;               // 0x10 [O]; [U] -> ForceScale
    float TorqueScale;              // 0x14 [O]; [U] * 1e-6 -> TorqueScale
    float LengthScale;              // 0x18 [O]; [U] -> Length
    int Width;                      // 0x1C [O]; [U] -> Width (the node count)
};

CHECK_OFFSET(tSpringData, LSpring, 0x00);
CHECK_OFFSET(tSpringData, LDamp, 0x04);
CHECK_OFFSET(tSpringData, ASpring, 0x08);
CHECK_OFFSET(tSpringData, ADamp, 0x0C);
CHECK_OFFSET(tSpringData, ForceScale, 0x10);
CHECK_OFFSET(tSpringData, TorqueScale, 0x14);
CHECK_OFFSET(tSpringData, LengthScale, 0x18);
CHECK_OFFSET(tSpringData, Width, 0x1C);
CHECK_SIZE(tSpringData, 0x20);

// A chain of damped linear + angular springs (On Tour cSpring = cSpringState 0x24 + Nodes; no vptr). EA's
// object/spring.cpp (src/object/spring.cpp: cSpring_cSpring__FPv, cSpring_cSpring1 (replay), cSpring::~cSpring,
// cSpring_setupSpring, cSpring_setupNodes, cSpring_update, cSpring_writeToReplayFrame). Move it to object/spring.h
// when that header is written. [U] cSpring_setupSpring stores 0x0..0x24; writeToReplayFrame writes the 0x24-byte
// state, then Width * 0x50 bytes of Nodes.
// One node of a cSpring (On Tour struct tSpringNode, 0x50). [R] cSpring's setupSpring allocates Width x 0x50
// ("SpringNodes"); [U] cFlexBridgeNode_applyForce adds into F / T (view sSeg_6E38: force 0x30, torque 0x40),
// cFlexBridgeNode_updateGrid reads P / O (view sFlexNode_75D8: pos 0x00, angle 0x20).
struct tSpringNode {
    t4Vector P;                     // 0x00 [O]
    t4Vector V;                     // 0x10 [O]
    float O;                        // 0x20 [O]; [U] updateGrid
    float W;                        // 0x24 [O]
    char unk_028[0x8];              // 0x28 padding (F is 16-byte aligned)
    t4Vector F;                     // 0x30 [O]; [U] applyForce
    float T;                        // 0x40 [O]; [U] applyForce
    char unk_044[0xC];              // 0x44 padding
};

CHECK_OFFSET(tSpringNode, P, 0x00);
CHECK_OFFSET(tSpringNode, V, 0x10);
CHECK_OFFSET(tSpringNode, O, 0x20);
CHECK_OFFSET(tSpringNode, W, 0x24);
CHECK_OFFSET(tSpringNode, F, 0x30);
CHECK_OFFSET(tSpringNode, T, 0x40);
CHECK_SIZE(tSpringNode, 0x50);

void operator_delete(int* p);

struct cSpring {
    int Width;                      // 0x00 [O]; [U] setupSpring (sw), setupNodes / update loop bounds (lw);
                                    //      cFlexBridgeNode grid size (lw 0x50)
    float LSpring;                  // 0x04 [O]; [U] setupSpring, update (lwc1)
    float LDamp;                    // 0x08 [O]; [U] setupSpring, update
    float ASpring;                  // 0x0C [O]; [U] setupSpring, update
    float ADamp;                    // 0x10 [O]; [U] setupSpring, update
    float ForceScale;               // 0x14 [O]; [U] setupSpring; cFlexBridgeNode_applyForce (lwc1 0x64)
    float TorqueScale;              // 0x18 [O]; [U] setupSpring; cFlexBridgeNode_applyForce (lwc1 0x68)
    float Length;                   // 0x1C [O]; [U] setupSpring, update
    float SegLen;                   // 0x20 [O]; [U] setupSpring, update
    tSpringNode* Nodes;             // 0x24 [O]; [U] setupSpring allocates Width 0x50-byte nodes ("SpringNodes"),
                                    //      setupNodes / update walk them; the cFlexBridgeNode methods read +0x74

    // Methods bound to their retail symbols by asm labels.
    void setupSpring(tSpringData* sdata, float len) __asm__("cSpring_setupSpring");
    void* cSpring1(cBxReplayFrame* stream) __asm__("cSpring_cSpring1");
    void setupNodes() __asm__("cSpring_setupNodes");
    void update() __asm__("cSpring_update");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cSpring_writeToReplayFrame");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    ~cSpring();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cSpring, Width, 0x00);
CHECK_OFFSET(cSpring, LSpring, 0x04);
CHECK_OFFSET(cSpring, LDamp, 0x08);
CHECK_OFFSET(cSpring, ASpring, 0x0C);
CHECK_OFFSET(cSpring, ADamp, 0x10);
CHECK_OFFSET(cSpring, ForceScale, 0x14);
CHECK_OFFSET(cSpring, TorqueScale, 0x18);
CHECK_OFFSET(cSpring, Length, 0x1C);
CHECK_OFFSET(cSpring, SegLen, 0x20);
CHECK_OFFSET(cSpring, Nodes, 0x24);
CHECK_SIZE(cSpring, 0x28);

struct cBxReplayFrame;

struct cFlexBridgeNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cFlexBridgeNode_vtbl (0x00490270) in both ctors and the dtor; [P]; [U] updateGrid / intersect / applyForce
    //       call slot 24 (getRootTransform)
    // cFlexBridgeNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 5 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] the record arg; the ctor sets its flags (+0x8) and reads its bound box (graphics slot 97); [U]
    //       setupGrid / render read the model, the dtor / setupGrid / updateGrid pass it to the Nash hash as id
    // --- cFlexBridgeNodeState (On Tour 0x30, 16-byte aligned; [R] the replay ctor reads 0x30 bytes at +0x20,
    //       writeToReplayFrame writes them)
    char unk_01C[0x4];              // 0x01C alignment gap before the 16-byte-aligned state (never accessed)
    t4Vector BottomLeftOffset;      // 0x020 [O]; [U] initSpringNodes stores Verts[Width] - Nodes[0].pos (sq),
                                    //      updateGrid reads it (lq)
    t4Vector BottomRightOffset;     // 0x030 [O]; [U] initSpringNodes stores Verts[0] - Nodes[0].pos (sq), updateGrid /
                                    //      applyForce read it (lq)
    float RailHeight;               // 0x040 [O]; [R] ctor 100.0 (swc1)
    int TextureId;                  // 0x044 [O]; [R] ctor 12 (sw)
    char unk_048[0x8];              // 0x048 the state's tail padding (On Tour state 0x28..0x30; never accessed)
    // --- cFlexBridgeNode
    cSpring Mesh;                   // 0x050 [O] cSpring (0x28); [R] ctors cSpring_cSpring__FPv / cSpring_cSpring1 on
                                    //      +0x50, setupSpring; dtor cSpring::~cSpring(+0x50, 2); [U] update
                                    //      cSpring_update(+0x50), writeToReplayFrame cSpring_writeToReplayFrame(+0x50);
                                    //      Width (0x50) / ForceScale (0x64) / TorqueScale (0x68) / Nodes (0x74) read
                                    //      directly
    cGridMesh* GridMesh;            // 0x078 [O]; [P]; [U] setupGrid: graphics slot 76 (Width, 2); the dtor frees it
                                    //      (slot 77); updateGrid (its slot 3) and render draw it
    t4Vector* Verts;                // 0x07C [O]; [P]; [U] setupGrid: new t4Vector[Width * 2] ("BridgeVerts");
                                    //      updateGrid / intersect / initSpringNodes read it; the dtor frees it
    t4Vector* FaceNorms;            // 0x080 [O]; [P]; [U] setupGrid: new t4Vector[Width * 2] ("BridgeNorms"); intersect
                                    //      passes it as the mesh normals; the dtor frees it
    unsigned char* Faces;           // 0x084 [O]; [P]; [U] setupGrid: new unsigned char[(Width - 1) * 6] ("BridgeFaces")
                                    //      filled with the triangle indices; intersect; the dtor frees it
    char unk_088[0x8];              // 0x088 alignment gap before the 16-byte-aligned BoundBox (On Tour too; never
                                    //      accessed)
    tBoundBox BoundBox;             // 0x090 [O]; [P]; [U] setupGrid / updateGrid recompute it (sq 0x90 / 0xA0) and move
                                    //      the Nash node; getBoundBox returns +0x90; render culls with it; the dtor
                                    //      removes it

    // Methods bound to their retail symbols by asm labels.
    void* cFlexBridgeNode1(void* bucket, cBxReplayFrame* stream) __asm__("cFlexBridgeNode_cFlexBridgeNode1");
    void updateGrid() __asm__("cFlexBridgeNode_updateGrid");
    // retail's destructor cFlexBridgeNode__cFlexBridgeNode stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFlexBridgeNode();
    virtual void update() __asm__("cFlexBridgeNode_update");
    virtual void render() __asm__("cFlexBridgeNode_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFlexBridgeNode_writeToReplayFrame");
    virtual void* getNodeState() __asm__("cFlexBridgeNode_getNodeState__FPv");
    virtual bool hasIntersect() const __asm__("cFlexBridgeNode_hasIntersect__FPv");
    virtual bool hasBoundBox() __asm__("cFlexBridgeNode_hasBoundBox__FPv");
    virtual void applyForce(const tIntersectInfo& intInfo, const t4Vector& force) __asm__("cFlexBridgeNode_applyForce");
    virtual void updateIntersectInfo(tIntersectInfo& intInfo) __asm__("cFlexBridgeNode_updateIntersectInfo");
    virtual int intersect(cWorldIntersectType& tester, tIntersectInfo* out, int cap, int flags)
        __asm__("cFlexBridgeNode_intersect");
    virtual const tBoundBox* getBoundBox() const __asm__("cFlexBridgeNode_getBoundBox__FPv");
};

CHECK_OFFSET(cFlexBridgeNode, unk_01C, 0x01C);
CHECK_OFFSET(cFlexBridgeNode, BottomLeftOffset, 0x020);
CHECK_OFFSET(cFlexBridgeNode, BottomRightOffset, 0x030);
CHECK_OFFSET(cFlexBridgeNode, RailHeight, 0x040);
CHECK_OFFSET(cFlexBridgeNode, TextureId, 0x044);
CHECK_OFFSET(cFlexBridgeNode, unk_048, 0x048);
CHECK_OFFSET(cFlexBridgeNode, Mesh, 0x050);
CHECK_OFFSET(cFlexBridgeNode, GridMesh, 0x078);
CHECK_OFFSET(cFlexBridgeNode, Verts, 0x07C);
CHECK_OFFSET(cFlexBridgeNode, FaceNorms, 0x080);
CHECK_OFFSET(cFlexBridgeNode, Faces, 0x084);
CHECK_OFFSET(cFlexBridgeNode, unk_088, 0x088);
CHECK_OFFSET(cFlexBridgeNode, BoundBox, 0x090);
CHECK_SIZE(cFlexBridgeNode, 0x0B0);

#endif /* OBJECT_FLEXBRIDGENODE_H */
