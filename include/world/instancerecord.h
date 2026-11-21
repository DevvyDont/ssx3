#ifndef WORLD_INSTANCERECORD_H
#define WORLD_INSTANCERECORD_H

// tInstanceRecord: one placed world instance (a model at a transform) in a world section's instance table; cWorld
// hands out pointers to them, cInstanceMan keeps its dynamic ones 0xB0 apart (object/instanceman.h:
// tInstanceManRecord = this + 4 pad bytes), the nash space hash links them (m_iIntersect), and the cInstanceNode that
// animates one is stored in it (m_pActiveNode).
// Size 0xAC, 4-byte aligned: [O] On Tour's tInstanceRecord (: tInstanceRecord_PSP / _tPSPInstance) is 0xAC and
// tInstanceManRecord adds m_PadBytes[4] for 0xB0; [R] cInstanceMan's records are 0xB0 apart. Its vectors are the
// float-array tf* types ([O]), not the 16-byte t4Vector.
// Member order: On Tour's, except where retail places a member elsewhere (m_pActiveNode at 0x0C, m_fScale at 0x84);
// unplaced On Tour members (ambient / diffuse / light dir / audio / light cache / render flags / material) are
// somewhere in 0x8C..0xAB and stay raw.
// EA's header: unknown (On Tour's DWARF has no file). Users: intersect/worldsphtree (the nash / overlap queries),
// object/instancenode, object/movenode, object/floatingnode, object/effectlink, object/flagset, object/multiparticle,
// object/instanceman, wscript.
//
// Evidence tags: [O] On Tour's DWARF; [P] the July 2003 preview's asserts; [R] retail ctor / alloc;
// [U] how retail code uses the field.

#include "layout.h"
#include "intersect/bxnash.h"   // nash_Node

struct cInstanceNode;
struct tCollideRecord;

struct tf4Vector {
    float m[4];                     // [O]
};

struct tf3Vector {
    float m[3];                     // [O]
};

struct tf4Matrix {
    float m[4][4];                  // [O]
};

CHECK_SIZE(tf4Vector, 0x10);
CHECK_SIZE(tf3Vector, 0xC);
CHECK_SIZE(tf4Matrix, 0x40);

// An instance's streaming map (On Tour tStreamMap, 4 bytes).
struct tStreamMap {
    unsigned char m_uiLocked;       // 0x0 [O]
    unsigned char m_uiSectionID;    // 0x1 [O]; [U] cMoveNode_render reads record +0x7D (0xFF = shared)
    short m_iHullID;                // 0x2 [O]; [U] cMoveNode_render reads record +0x7E
};

CHECK_OFFSET(tStreamMap, m_uiLocked, 0x0);
CHECK_OFFSET(tStreamMap, m_uiSectionID, 0x1);
CHECK_OFFSET(tStreamMap, m_iHullID, 0x2);
CHECK_SIZE(tStreamMap, 0x4);

struct tInstanceRecord {
    nash_Node m_iIntersect;         // 0x00 [O] int m_iIntersect[2]; [U] the nash space-hash link (next / prev) the
                                    //      overlap queries walk (views sNode_3309D8 / sNode_336D40 / sNode_335B90 /
                                    //      sNode_334458)
    int m_iFlags;                   // 0x08 [O]; [U] tested by the queries and the instance nodes
    cInstanceNode* m_pActiveNode;   // 0x0C [P] asserts "!pInst->m_pActiveNode", "Instance->m_pActiveNode";
                                    //      [R] cInstanceNode_setActiveNode stores the node here, clearActiveNode clears it
    tf4Matrix m_fmTransform;        // 0x10 [O] (same offset); [U] the instance's world transform
    tf4Vector m_fvBSphere;          // 0x50 [O]; [U] cMoveNode_render passes it to isSphereOccluded
    tf3Vector m_fvMin;              // 0x60 [O]; [U] the instance's bound box (cMoveNode_render)
    tf3Vector m_fvMax;              // 0x6C [O]; [U] ditto
    unsigned int m_RID;             // 0x78 [O] tResourceID (by order); [U] cInstanceMan_allocInstance writes the section
                                    //      byte at +0x78; view sFrRider_B168 id (cFlagSet_RemoveFlag)
    tStreamMap m_SMap;              // 0x7C [O]; [U] section +0x7D, hull +0x7E (cMoveNode_render)
    void* m_Model;                  // 0x80 [O] (by order); [U] the model / bone table {?, count +4, bones +8}
                                    //      (views sWsInst_4888, sFrRider_B168 track)
    float m_fScale;                 // 0x84 [O] (name); [U] worldsphtree's instance sphere scale (sWsInst_4888)
    tCollideRecord* m_pCollideRecord; // 0x88 [O] (by order); [U] collide method at +0 (the terrain/static queries
                                    //      skip a 0 record)
    char unk_08C[0x8];              // 0x8C
    void* unk_094;                  // 0x94 [U] cMoveNode_render reads it (a light pointer)
    char unk_098[0x14];             // 0x98
};

CHECK_OFFSET(tInstanceRecord, m_iIntersect, 0x00);
CHECK_OFFSET(tInstanceRecord, m_iFlags, 0x08);
CHECK_OFFSET(tInstanceRecord, m_pActiveNode, 0x0C);
CHECK_OFFSET(tInstanceRecord, m_fmTransform, 0x10);
CHECK_OFFSET(tInstanceRecord, m_fvBSphere, 0x50);
CHECK_OFFSET(tInstanceRecord, m_fvMin, 0x60);
CHECK_OFFSET(tInstanceRecord, m_fvMax, 0x6C);
CHECK_OFFSET(tInstanceRecord, m_RID, 0x78);
CHECK_OFFSET(tInstanceRecord, m_SMap, 0x7C);
CHECK_OFFSET(tInstanceRecord, m_Model, 0x80);
CHECK_OFFSET(tInstanceRecord, m_fScale, 0x84);
CHECK_OFFSET(tInstanceRecord, m_pCollideRecord, 0x88);
CHECK_OFFSET(tInstanceRecord, unk_08C, 0x8C);
CHECK_OFFSET(tInstanceRecord, unk_094, 0x94);
CHECK_OFFSET(tInstanceRecord, unk_098, 0x98);
CHECK_SIZE(tInstanceRecord, 0xAC);

#endif /* WORLD_INSTANCERECORD_H */
