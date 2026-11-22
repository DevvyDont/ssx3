#ifndef MAIN_GAMERENDER_H
#define MAIN_GAMERENDER_H

// FrustumIntersector: one camera view's visibility query. cGame_render builds one on its stack per view
// (FrustumIntersector_FrustumIntersector, the ctor: it runs the whole query): it culls the world's octree cells against the view
// (FrustumCallback, FrustumCallback_operatorCall), then sorts the objects of the inside / crossing cells into fixed-size lists -
// instances, patches, fog volumes and light halos, each "inside" (whole object visible) or "crossing" (tested again on
// VU0, one object pending at a time in last*). cGame's render passes (renderModels, renderPatches, renderFogVolumes,
// renderLightHalos; On Tour signatures take const FrustumIntersector&) read the lists.
// The class name and members are SSX On Tour's (DWARF class FrustumIntersector, size 0x8210, in
// z:/main/game/source/main/all_in_one_main.cpp): every retail offset matches On Tour's member offset minus 0x40 from
// `world` on, because On Tour's two PSPFrustumAABBIntersector members are 0x60 bytes and retail's are 0x40 matrices.
// The July 2003 preview's assert strings (EA d:/ssx/code/source/main/gamerender.cpp) name the same arrays:
// "numinstancesinside < arraysize(instancesinside)", "numpatchescrossing < arraysize(patchescrossing)",
// "numfogvolumesinside < ...", "numlighthaloscrossing < ...". Header path from that file. Retail code: main/ssxapp
// 0x00229FC8..0x0022ADD8 in On Tour's source order (crossinginstance, crossingpatch, crossingfogvolume, the six
// AddAll*, FrustumCallback::operator(), then the ctor). The struct name is EA's (no c prefix).
//
// Size 0x81D0: [U] cGame_render's stack object is the matched view sWorld_22B008 {char data[0x81D0]} aligned 16 (no
// allocation); the ctor zeroes every count and last* word up to 0x7FC4, and lighthaloscrossing[128] ends at 0x81C8,
// rounded up to the 16-byte alignment of the matrices; [O] On Tour's 0x8210 minus 0x40. No vtable, no base class.
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the ctor FrustumIntersector_FrustumIntersector; [U] how retail code uses the field
// (src/main/ssxapp.cpp and the asm of the functions above); [O] SSX On Tour's DWARF (names, types, order);
// [P] the preview's asserts (array bounds).

#include "layout.h"
#include "bx/math3d.h"                  // t4Matrix

struct cOcclusionMan;
struct tBoundBox;
struct cWorld;
struct tInstanceRecord;
struct tPatchRecord;
struct tParticleInstanceRecord;
struct tHaloRecord;
struct Cell;

// On Tour's CellClipT (size 8): an octree cell and the clip planes still to test for it.
struct CellClipT {
    const Cell* pCell;              // 0x0 [O]; [U] the AddAll* walk the cell's lists (+0x20 instances, +0x24 patches,
                                    //     +0x28 typed objects)
    unsigned int uVisMask;          // 0x4 [O]; [U] passed to the crossing tests (0: no test)
};

CHECK_OFFSET(CellClipT, pCell, 0x0);
CHECK_OFFSET(CellClipT, uVisMask, 0x4);
CHECK_SIZE(CellClipT, 0x8);

struct FrustumIntersector {
    cOcclusionMan* occlusionman;    // 0x0000 [O] (a reference); [R] ctor argument 2; [U] cOcclusionMan_updateView
    const tBoundBox* camerabox;     // 0x0004 [O] (a reference); [R] ctor argument 3; [U] func_00229F80, the callback
    char unk_0008[0x8];             // 0x0008 no access (alignment of VFIntersect)
    t4Matrix VFIntersect;           // 0x0010 [O] name (On Tour: PSPFrustumAABBIntersector, 0x60); [R] ctor copies the
                                    //        graphics manager's view matrix (vtable slot 43) here; func_00229F30 loads
                                    //        it into VU0 memory
    t4Matrix GFIntersect;           // 0x0050 [O] name (PSPFrustumAABBIntersector there); [R] ctor copies GrMan + 0x5800
    cWorld* world;                  // 0x0090 [O]; [R] ctor argument 4; [U] AddAllInstancesInside / AddAllPatchesInside /
                                    //        crossinginstance read its slot table (+0x20)
    int numcellsinside;             // 0x0094 [O]; [R] ctor 0; [U] FrustumCallback appends
    CellClipT cellsinside[512];     // 0x0098 [O]; [U] the ctor passes it to the AddAll*Inside
    int numcellscrossing;           // 0x1098 [O]; [R] ctor 0
    CellClipT cellscrossing[512];   // 0x109C [O]; [U] the ctor passes it to the AddAll*Crossing
    int numinstancesinside;         // 0x209C [O] [P]; [R] ctor 0; [U] crossinginstance, AddAllInstancesInside
    tInstanceRecord* instancesinside[2048]; // 0x20A0 [O] [P]
    int numinstancescrossing;       // 0x40A0 [O] [P]; [R] ctor 0; [U] crossinginstance
    tInstanceRecord* instancescrossing[1024]; // 0x40A4 [O] [P]
    tInstanceRecord* lastinstance;  // 0x50A4 [O]; [R] ctor 0; [U] the pending VU0 test (crossinginstance,
                                    //        AddAllInstancesCrossing)
    int numpatchesinside;           // 0x50A8 [O] [P]; [R] ctor 0; [U] crossingpatch, AddAllPatchesInside
    tPatchRecord* patchesinside[2048]; // 0x50AC [O] [P]
    int numpatchescrossing;         // 0x70AC [O] [P]; [R] ctor 0; [U] crossingpatch
    tPatchRecord* patchescrossing[512]; // 0x70B0 [O] [P]
    tPatchRecord* lastpatch;        // 0x78B0 [O]; [R] ctor 0; [U] crossingpatch, AddAllPatchesCrossing
    int numfogvolumesinside;        // 0x78B4 [O] [P]; [R] ctor 0; [U] crossingfogvolume, AddAllTypedInside
    tParticleInstanceRecord* fogvolumesinside[128]; // 0x78B8 [O] [P]
    tParticleInstanceRecord* lastfogvolume; // 0x7AB8 [O]; [R] ctor 0; [U] crossingfogvolume, AddAllTypedCrossing
    int numfogvolumescrossing;      // 0x7ABC [O] [P]; [R] ctor 0; [U] crossingfogvolume
    tParticleInstanceRecord* fogvolumescrossing[64]; // 0x7AC0 [O] [P]
    int numlighthalosinside;        // 0x7BC0 [O] [P]; [R] ctor 0; [U] AddAllTypedCrossing / AddAllTypedInside (type 8)
    tHaloRecord* lighthalosinside[256]; // 0x7BC4 [O] [P]
    int numlighthaloscrossing;      // 0x7FC4 [O] [P]; [R] ctor 0
    tHaloRecord* lighthaloscrossing[128]; // 0x7FC8 [O] [P]
    char unk_81C8[0x8];             // 0x81C8 no access (tail padding to the 16-byte alignment)
};

CHECK_OFFSET(FrustumIntersector, occlusionman, 0x0000);
CHECK_OFFSET(FrustumIntersector, camerabox, 0x0004);
CHECK_OFFSET(FrustumIntersector, unk_0008, 0x0008);
CHECK_OFFSET(FrustumIntersector, VFIntersect, 0x0010);
CHECK_OFFSET(FrustumIntersector, GFIntersect, 0x0050);
CHECK_OFFSET(FrustumIntersector, world, 0x0090);
CHECK_OFFSET(FrustumIntersector, numcellsinside, 0x0094);
CHECK_OFFSET(FrustumIntersector, cellsinside, 0x0098);
CHECK_OFFSET(FrustumIntersector, numcellscrossing, 0x1098);
CHECK_OFFSET(FrustumIntersector, cellscrossing, 0x109C);
CHECK_OFFSET(FrustumIntersector, numinstancesinside, 0x209C);
CHECK_OFFSET(FrustumIntersector, instancesinside, 0x20A0);
CHECK_OFFSET(FrustumIntersector, numinstancescrossing, 0x40A0);
CHECK_OFFSET(FrustumIntersector, instancescrossing, 0x40A4);
CHECK_OFFSET(FrustumIntersector, lastinstance, 0x50A4);
CHECK_OFFSET(FrustumIntersector, numpatchesinside, 0x50A8);
CHECK_OFFSET(FrustumIntersector, patchesinside, 0x50AC);
CHECK_OFFSET(FrustumIntersector, numpatchescrossing, 0x70AC);
CHECK_OFFSET(FrustumIntersector, patchescrossing, 0x70B0);
CHECK_OFFSET(FrustumIntersector, lastpatch, 0x78B0);
CHECK_OFFSET(FrustumIntersector, numfogvolumesinside, 0x78B4);
CHECK_OFFSET(FrustumIntersector, fogvolumesinside, 0x78B8);
CHECK_OFFSET(FrustumIntersector, lastfogvolume, 0x7AB8);
CHECK_OFFSET(FrustumIntersector, numfogvolumescrossing, 0x7ABC);
CHECK_OFFSET(FrustumIntersector, fogvolumescrossing, 0x7AC0);
CHECK_OFFSET(FrustumIntersector, numlighthalosinside, 0x7BC0);
CHECK_OFFSET(FrustumIntersector, lighthalosinside, 0x7BC4);
CHECK_OFFSET(FrustumIntersector, numlighthaloscrossing, 0x7FC4);
CHECK_OFFSET(FrustumIntersector, lighthaloscrossing, 0x7FC8);
CHECK_OFFSET(FrustumIntersector, unk_81C8, 0x81C8);
CHECK_SIZE(FrustumIntersector, 0x81D0);

// The per-cell visitor the ctor runs over the nash cell tree (On Tour struct FrustumCallback, 0x10:
// FrustumCallback(FrustumIntersector& i, cOcclusionMan& om, unsigned int vm, bool insidefrustum), operator()(const
// Cell&, const CellKey&)). [R] func_0022A830 is that operator: it tests the cell's sphere against the occlusion man's
// views, appends the cell to cellsinside / cellscrossing of the intersector and recurses into the children. On Tour's
// references are pointers here and its bool a word. [Staged by views2 group A.]
struct FrustumCallback {
    FrustumIntersector* intersector; // 0x0 [O] FrustumIntersector&; [U] the cell lists it appends to
    cOcclusionMan* occlusionman;    // 0x4 [O] cOcclusionMan&; [U] the occlusion views the cell sphere is tested against
    unsigned int vismask_;          // 0x8 [O]; [U] the views still to test (bits)
    int insidefrustum_;             // 0xC [O] bool (a word); [U] set when the parent cell is wholly inside
};

CHECK_OFFSET(FrustumCallback, intersector, 0x0);
CHECK_OFFSET(FrustumCallback, occlusionman, 0x4);
CHECK_OFFSET(FrustumCallback, vismask_, 0x8);
CHECK_OFFSET(FrustumCallback, insidefrustum_, 0xC);
CHECK_SIZE(FrustumCallback, 0x10);

#endif /* MAIN_GAMERENDER_H */
