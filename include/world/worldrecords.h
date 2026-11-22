#ifndef WORLD_WORLDRECORDS_H
#define WORLD_WORLDRECORDS_H

// The streamed world records an instance points into: its model (tModelRecord) with the mesh hierarchy, meshes,
// material block / materials / texture flips and animations, the spline and spline-segment records, the particle
// model / instance records and the collision records. They are loaded from the .sdb / section data and fixed up in
// place by cChunkResolve / cPS2ChunkResolve (src/world/worldview.cpp: resolveModel, resolveModelHeirarchy,
// resolveMesh, resolveMaterial, resolveAnimation, resolveChannel, resolveSpline, resolveParticleModel,
// resolveParticleInstance, resolveCollideModel; offsets of -1 become 0 or t4mat_Identity, other offsets become
// pointers into the record), and by cWScriptCacheEntry_resolve for the collide and spline records.
//
// Names and member order are SSX On Tour's (PSP DWARF: tModelRecord : tModelRecord_PSP, tMeshRecord_PSP,
// _tPSPSplineRecord, _tPSPSplineSegment, ...; the ...ID members there are unions of a pointer and an offset, written
// here as the pointer). Retail differs from On Tour where noted: the model and particle-model records lack one of
// On Tour's two leading words (everything from the mesh count on sits 4 bytes lower), the material record is 0x14
// (its texture flip is at 0x10 and the flip data follows it), and the mesh contexts are a pointer table. Members
// without a retail use stay unk_XXX with On Tour's name in the comment.
// Streamed records are never allocated or arrayed by size in retail except where a CHECK_SIZE says [R]; a size marked
// "laid out through" covers the members retail uses and the record may continue.
//
// Evidence tags: [O] On Tour's DWARF; [P] the July 2003 preview's assert strings; [R] retail fix-up strides;
// [U] how retail code uses the field.

#include "layout.h"
#include "world/instancerecord.h"   // tf3Vector / tf4Vector / tf4Matrix, tInstanceRecord

struct tMeshContextRecord;          // [P] meshRecord->m_pContexts[context]->m_sMaterialIndex (not laid out)
struct tAnimRecord;
struct tSplineRecord;
struct tInstanceAudio;              // [O] tInstanceRecord / tParticleInstanceRecord::m_pInstanceAudio
struct bxSphereTree;                // object/rollermodifier.h (0x140)
struct tPolyModelMesh;              // [O] 0x14 (laid out below)
struct tPolyBoundBox;               // [O] (not laid out)
struct t4Vector;                    // bx/math3d.h

// A model's mesh table entry (On Tour tMeshHeirarchyRecord, 0x10; same layout).
struct tMeshHeirarchyRecord {
    int m_iParentID;                // 0x0 [O]; [U] cConveyorNode_updateIntersectInfo walks the parents to -1
                                    //      (view sBone3130 parent)
    struct tMeshRecord* m_Mesh;     // 0x4 [O] tMeshID; [U] resolveModelHeirarchy fixes it up and resolves the mesh;
                                    //      worldsphtree func_4888 reads the mesh box (view sWsBone_4888.mesh)
    tAnimRecord* m_MeshAnim;        // 0x8 [O] tMeshAnimID; [U] resolveModelHeirarchy fixes it up and resolves the
                                    //      animation; cAnimTeeterNode_applyForce tests it (the mesh is animated)
    tf4Matrix* m_pMeshMatrix;       // 0xC [O]; [U] resolveModelHeirarchy: -1 -> t4mat_Identity, else fixed up;
                                    //      the conveyor bone walk reads the matrix (view sBone3130 mat)
};

CHECK_OFFSET(tMeshHeirarchyRecord, m_iParentID, 0x0);
CHECK_OFFSET(tMeshHeirarchyRecord, m_Mesh, 0x4);
CHECK_OFFSET(tMeshHeirarchyRecord, m_MeshAnim, 0x8);
CHECK_OFFSET(tMeshHeirarchyRecord, m_pMeshMatrix, 0xC);
CHECK_SIZE(tMeshHeirarchyRecord, 0x10);

// One mesh (On Tour tMeshRecord, 0x24; same layout).
struct tMeshRecord {
    tf3Vector m_fvMin;              // 0x00 [O]; [U] worldsphtree func_4888 passes mesh / mesh + 0xC as a box
    tf3Vector m_fvMax;              // 0x0C [O]; [U] ditto
    int m_iFlags;                   // 0x18 [O]; [U] func_4888 tests bit 0
    int m_iNumContexts;             // 0x1C [O]; [U] resolveMesh, func_003ABFC0 loop over the contexts
    tMeshContextRecord** m_pContexts; // 0x20 [O] (On Tour a record pointer); [P] "meshRecord->m_pContexts[context]->
                                    //      m_sMaterialIndex"; [U] resolveMesh fixes it up, func_003ABFC0 walks the
                                    //      pointer table
};

CHECK_OFFSET(tMeshRecord, m_fvMin, 0x00);
CHECK_OFFSET(tMeshRecord, m_fvMax, 0x0C);
CHECK_OFFSET(tMeshRecord, m_iFlags, 0x18);
CHECK_OFFSET(tMeshRecord, m_iNumContexts, 0x1C);
CHECK_OFFSET(tMeshRecord, m_pContexts, 0x20);
CHECK_SIZE(tMeshRecord, 0x24);

// A texture-flip sequence (On Tour tTextureFlipRecord, 4 bytes): the count, then the texture table (4 bytes per
// entry; retail reads the low halfword of entry i as the texture id).
struct tTextureFlipRecord {
    int m_iNumTextures;             // 0x0 [O]; [U] the table at +0x4 follows it (setupTexFlip / setupMaterialRecords /
                                    //      func_00344730 read GetTextureTable()[index])
    // [O] tTextureFlipRecord_PSP::GetTextureTable(): the table right after the count.
    int* GetTextureTable() const { return (int*)(this + 1); }
};

CHECK_OFFSET(tTextureFlipRecord, m_iNumTextures, 0x0);
CHECK_SIZE(tTextureFlipRecord, 0x4);

// [O] tTextureFlipID: the texture flip as a pointer after the fix-up (an offset in the file).
union tTextureFlipID {
    int m_iTextureFlip;             // [O]; [U] resolveMaterial tests it before the fix-up
    tTextureFlipRecord* m_pTextureFlip; // [O]; [P] "pMaterialRecord->m_TextureFlip.m_pTextureFlip"
};

CHECK_SIZE(tTextureFlipID, 0x4);

// One material (On Tour tMaterialRecord, 0xC there; retail 0x14: the texture flip sits at 0x10 and its data follows
// the record, resolveMaterial points it at this + 0x14).
struct tMaterialRecord {
    short m_sTexture;               // 0x00 [O]; [U] cMeshAnimNode_render checks the texture's resource is cached
    char unk_002[0xE];              // 0x02 [O] m_sPad, m_iFlags (retail layout of 0x2..0xF not confirmed; +0xC is
                                    //      copied into the tex-flip slots by func_003443A8)
    tTextureFlipID m_TextureFlip;   // 0x10 [O] (0x8 there); [P] assert above; [U] resolveMaterial = this + 0x14 or 0;
                                    //      setupTexFlip / setupMaterialRecords read the table through it
};

CHECK_OFFSET(tMaterialRecord, m_sTexture, 0x00);
CHECK_OFFSET(tMaterialRecord, unk_002, 0x02);
CHECK_OFFSET(tMaterialRecord, m_TextureFlip, 0x10);
CHECK_SIZE(tMaterialRecord, 0x14);

// A model's material list (On Tour tMaterialBlockRecord, 4 bytes): the count, then that many tMaterialRecord pointers.
struct tMaterialBlockRecord {
    int m_iNumMaterials;            // 0x0 [O]; [U] cMeshAnimNode_render's loop bound (model->m_MaterialBlock)
    // [O] GetMaterialRecordList(): the pointer list right after the count ([U] resolveModel fills it; texflip and
    // cMeshAnimNode_render read list[i]).
    tMaterialRecord** GetMaterialRecordList() const { return (tMaterialRecord**)(this + 1); }
};

CHECK_OFFSET(tMaterialBlockRecord, m_iNumMaterials, 0x0);
CHECK_SIZE(tMaterialBlockRecord, 0x4);

// A model (On Tour tModelRecord : tModelRecord_PSP, 0x34 there). Retail has one leading word fewer: the mesh count is
// at 0x4 (On Tour 0x8), so every later member sits 4 bytes lower; retail has one word (0x10) where On Tour has
// m_ModelBuffer and m_iFlags. Laid out through the last word retail reads (0x24).
struct tModelRecord {
    unsigned int m_RID;             // 0x00 [O] tResourceID (first member); [U] cFlagSet_SameKind / AddFlag compare and
                                    //      store it as the flag set's mModelID
    int m_iNumMeshes;               // 0x04 [O]; [U] resolveModelHeirarchy's count; cMeshAnimNode init/update/render,
                                    //      cAnimNode ctors (AnimMats new[]), cMultiSplineModifier_allocNodes
    tMeshHeirarchyRecord* m_MeshHeirarchy; // 0x08 [O] tMeshHeirarchyID; [U] resolveModelHeirarchy walks it; the bone
                                    //      walks (conveyor, teeter, updateMeshVelocity) index it
    tMaterialBlockRecord* m_MaterialBlock; // 0x0C [O] tMaterialBlockID; [U] cPS2ChunkResolve_resolveModel resolves it;
                                    //      resolveInstance copies it to tInstanceRecord::m_MaterialBlock;
                                    //      cSkyBox_render and cMeshAnimNode_render pass / read it
    int unk_010;                    // 0x10 [O] m_ModelBuffer or m_iFlags (not placed)
    float m_fAnimTime;              // 0x14 [O]; [U] cAnimObjectNode_signal / the anim-node ctors (setRateMax) use it as
                                    //      the default end time
    char unk_018[0xC];              // 0x18 [O] m_fvVertexScale (not placed)
    void* unk_024;                  // 0x24 [U] the model's DMA data base (func_003AC048 / func_003AC508 read ctx +0x24)
};

CHECK_OFFSET(tModelRecord, m_RID, 0x00);
CHECK_OFFSET(tModelRecord, m_iNumMeshes, 0x04);
CHECK_OFFSET(tModelRecord, m_MeshHeirarchy, 0x08);
CHECK_OFFSET(tModelRecord, m_MaterialBlock, 0x0C);
CHECK_OFFSET(tModelRecord, unk_010, 0x10);
CHECK_OFFSET(tModelRecord, m_fAnimTime, 0x14);
CHECK_OFFSET(tModelRecord, unk_018, 0x18);
CHECK_OFFSET(tModelRecord, unk_024, 0x24);
CHECK_SIZE(tModelRecord, 0x28);

// One animation segment (On Tour tSegmentRecord, 0x18; same layout).
struct tSegmentRecord {
    tf4Vector m_fvInv;              // 0x00 [O]; [U] cAnimFrame_update (view sImSeg_351538) evaluates
                                    //      ((m[0] * t + m[1]) * t + m[2]) * t + m[3]
    float m_fStart;                 // 0x10 [O]; [U] cAnimFrame_getStartTime reads channel->m_Segments->m_fStart
    float m_fEnd;                   // 0x14 [O]; [U] cAnimFrame_update keeps the segment while m_fStart <= t < m_fEnd
};

CHECK_OFFSET(tSegmentRecord, m_fvInv, 0x00);
CHECK_OFFSET(tSegmentRecord, m_fStart, 0x10);
CHECK_OFFSET(tSegmentRecord, m_fEnd, 0x14);
CHECK_SIZE(tSegmentRecord, 0x18);

// One animation channel (On Tour tChannelRecord, 8 bytes; same layout).
struct tChannelRecord {
    int m_NumSegments;              // 0x0 [O]; [U] resolveChannel's count
    tSegmentRecord* m_Segments;     // 0x4 [O] tSegmentID; [U] resolveChannel fixes it up; cAnimFrame_getStartTime
};

CHECK_OFFSET(tChannelRecord, m_NumSegments, 0x0);
CHECK_OFFSET(tChannelRecord, m_Segments, 0x4);
CHECK_SIZE(tChannelRecord, 0x8);

// A mesh animation (On Tour tAnimRecord, 0x24; same layout). cAnimFrame::AnimRecord (object/animframe.h) points here.
struct tAnimRecord {
    char unk_000[0x18];             // 0x00 [O] tf3Vector m_fvTranslation, m_fvRotation (no retail read seen)
    int m_ChannelMask;              // 0x18 [O]; [U] cAnimFrame_update / getDeriv test channel bits
    int m_NumChannels;              // 0x1C [O]; [U] resolveAnimation's count
    tChannelRecord** m_pChannels;   // 0x20 [O] tChannelID*; [U] resolveAnimation fixes up the table;
                                    //      cAnimFrame_getStartTime / update / getDeriv read m_pChannels[i]
};

CHECK_OFFSET(tAnimRecord, unk_000, 0x00);
CHECK_OFFSET(tAnimRecord, m_ChannelMask, 0x18);
CHECK_OFFSET(tAnimRecord, m_NumChannels, 0x1C);
CHECK_OFFSET(tAnimRecord, m_pChannels, 0x20);
CHECK_SIZE(tAnimRecord, 0x24);

// One spline segment (On Tour tSplineSegmentRecord : _tPSPSplineSegment, 0x88 + pad; retail 0x90: resolveSpline
// steps 0x90). The first three words are the nash link and the space-node type (cTypedSpaceNode, set to 1 =
// kSNT_SplineSegment when the segment is filed in the nash).
struct tSplineSegmentRecord {
    int m_iIntersect[3];            // 0x00 [O]; [U] nash next / prev, +0x8 the type word (cPS2ChunkResolve_resolve /
                                    //      unresolve set it to 1); the overlap queries switch on it
    float unk_00C;                  // 0x0C [O] m_fLength
    tf4Matrix m_fmBasisMat;         // 0x10 [O]; [U] cSpline_draw evaluates the segment with it
    char unk_050[0x10];             // 0x50 [O] tf4Vector m_fvInv
    tSplineSegmentRecord* m_PrevSeg; // 0x60 [O] tSplineSegmentID; [U] resolveSpline links it
    tSplineSegmentRecord* m_NextSeg; // 0x64 [O]; [U] resolveSpline links it; cSpline_draw, cRailManNode_enable/
                                    //      disableRail, cPS2ChunkResolve_resolve / unresolve walk it
    tSplineRecord* m_Parent;        // 0x68 [O] tSplineID; [U] resolveSpline stores the owner; the spline queries read
                                    //      m_Parent->m_iFlags / m_eSurfaceType
    tf3Vector m_fvMin;              // 0x6C [O]; [U] the segment box filed in the nash (box offset 108) and moved by
                                    //      cRailManNode_enable/disableRail, cSplineSegment_isVisible
    tf3Vector m_fvMax;              // 0x78 [O]; [U] ditto
    float m_fDistance;              // 0x84 [O]; [U] cSpline_getPos steps to the next segment while the distance
                                    //      is past it (view sDebKey5248 t0)
    char unk_088[0x8];              // 0x88 [O] m_PadBytes (tSplineSegmentRecord_PSP); [R] stride 0x90
};

CHECK_OFFSET(tSplineSegmentRecord, m_iIntersect, 0x00);
CHECK_OFFSET(tSplineSegmentRecord, unk_00C, 0x0C);
CHECK_OFFSET(tSplineSegmentRecord, m_fmBasisMat, 0x10);
CHECK_OFFSET(tSplineSegmentRecord, unk_050, 0x50);
CHECK_OFFSET(tSplineSegmentRecord, m_PrevSeg, 0x60);
CHECK_OFFSET(tSplineSegmentRecord, m_NextSeg, 0x64);
CHECK_OFFSET(tSplineSegmentRecord, m_Parent, 0x68);
CHECK_OFFSET(tSplineSegmentRecord, m_fvMin, 0x6C);
CHECK_OFFSET(tSplineSegmentRecord, m_fvMax, 0x78);
CHECK_OFFSET(tSplineSegmentRecord, m_fDistance, 0x84);
CHECK_OFFSET(tSplineSegmentRecord, unk_088, 0x88);
CHECK_SIZE(tSplineSegmentRecord, 0x90);

// A spline (On Tour tSplineRecord : _tPSPSplineRecord, 0x30; same layout). Its segments follow it ([U]
// cChunkResolve_resolveSpline's first segment is this + 0x30).
struct tSplineRecord {
    unsigned int unk_000;           // 0x00 [O] tResourceID m_RID
    char unk_004[0x18];             // 0x04 [O] tf3Vector m_fvMin, m_fvMax
    int m_iFlags;                   // 0x1C [O]; [U] cWScriptCacheEntry_resolve stores it (tSplineData::Flags); the
                                    //      spline queries test m_Parent->m_iFlags
    int m_iNumSegments;             // 0x20 [O]; [U] resolveSpline, cSpline_draw, cPS2ChunkResolve_resolve / unresolve
    tSplineSegmentRecord* m_HeaderSeg; // 0x24 [O] tSplineSegmentID; [U] resolveSpline fixes it up; the same walks
    int m_eSurfaceType;             // 0x28 [O]; [U] cWScriptCacheEntry_resolve stores it (tSplineData::SurfaceType);
                                    //      intersect_staticSplineSegmentIntersect reads owner +0x28
    char unk_02C[0x4];              // 0x2C [O] m_PadBytes
};

CHECK_OFFSET(tSplineRecord, unk_000, 0x00);
CHECK_OFFSET(tSplineRecord, unk_004, 0x04);
CHECK_OFFSET(tSplineRecord, m_iFlags, 0x1C);
CHECK_OFFSET(tSplineRecord, m_iNumSegments, 0x20);
CHECK_OFFSET(tSplineRecord, m_HeaderSeg, 0x24);
CHECK_OFFSET(tSplineRecord, m_eSurfaceType, 0x28);
CHECK_OFFSET(tSplineRecord, unk_02C, 0x2C);
CHECK_SIZE(tSplineRecord, 0x30);

// A particle model (On Tour tParticleModelRecord_PSP, 0x1C there). Same one-word shift as tModelRecord. Laid out
// through the members retail reads.
struct tParticleModelRecord {
    unsigned int unk_000;           // 0x0 [O] tResourceID m_RID (by the tModelRecord shift)
    int m_iNumParticleMeshes;       // 0x4 [O] (0x8 there); [U] cChunkResolve_resolveParticleModel's count
    void* m_ParticleMeshHeirarchy;  // 0x8 [O] tParticleMeshHeirarchyID (0xC there); [U] resolveParticleModel fixes
                                    //      it up and walks it
};

CHECK_OFFSET(tParticleModelRecord, unk_000, 0x0);
CHECK_OFFSET(tParticleModelRecord, m_iNumParticleMeshes, 0x4);
CHECK_OFFSET(tParticleModelRecord, m_ParticleMeshHeirarchy, 0x8);
CHECK_SIZE(tParticleModelRecord, 0xC);

// A placed particle emitter (On Tour tParticleInstanceRecord : _tPSPParticleInstance, 0x88 + pad). The two members
// retail uses sit at On Tour's offsets.
struct tParticleInstanceRecord {
    char unk_000[0x50];             // 0x00 [O] m_iIntersect[3], m_iFlags, m_fmTransform
    tf4Vector m_fvBSphere;          // 0x50 [O] (_tPSPParticleInstance 0x50); [U] FrustumIntersector_crossingfogvolume
                                    //      loads it into VU0 with the box (view sVisObj_A270 sphere) [views2 group A]
    char unk_060[0x4];              // 0x60 [O] m_RID
    tParticleModelRecord* m_Model;  // 0x64 [O] tParticleModelID; [U] cChunkResolve_resolveParticleInstance resolves it
    tf3Vector m_fvMin;              // 0x68 [O] (0x68 there); [U] FrustumIntersector_crossingfogvolume: the box minimum
                                    //      (view sVisObj_A270 boxMin) [views2 group A]
    tf3Vector m_fvMax;              // 0x74 [O] (0x74 there); [U] ditto, the box maximum (boxMax) [views2 group A]
    char unk_080[0x4];              // 0x80 [O] m_pActiveNode
    tInstanceAudio* m_pInstanceAudio; // 0x84 [O]; [U] resolveParticleInstance zeroes it
};

CHECK_OFFSET(tParticleInstanceRecord, unk_000, 0x00);
CHECK_OFFSET(tParticleInstanceRecord, m_fvBSphere, 0x50);
CHECK_OFFSET(tParticleInstanceRecord, unk_060, 0x60);
CHECK_OFFSET(tParticleInstanceRecord, m_Model, 0x64);
CHECK_OFFSET(tParticleInstanceRecord, m_fvMin, 0x68);
CHECK_OFFSET(tParticleInstanceRecord, m_fvMax, 0x74);
CHECK_OFFSET(tParticleInstanceRecord, unk_080, 0x80);
CHECK_OFFSET(tParticleInstanceRecord, m_pInstanceAudio, 0x84);
CHECK_SIZE(tParticleInstanceRecord, 0x88);

// A collision model's sphere trees (On Tour tSphTreeList, 8 bytes; method 3). [O] getMeshTree(meshNum) =
// &MeshTree[meshNum] (retail tSphTreeList_getMeshTree: + meshNum * 0x140).
struct tSphTreeList {
    bxSphereTree* MeshTree;         // 0x0 [O]; [U] resolveCollideModel points it at the tree array (0x140 each) and
                                    //      loads each (bxSphereTree_Load)
    int NumMesh;                    // 0x4 [O]; [U] resolveCollideModel stores the mesh count and loops to it
};

CHECK_OFFSET(tSphTreeList, MeshTree, 0x0);
CHECK_OFFSET(tSphTreeList, NumMesh, 0x4);
CHECK_SIZE(tSphTreeList, 0x8);

// One collision mesh (On Tour struct tPolyModelMesh, 0x14). [O] tPolyModelList::MeshPolyModel /
// tCollideModelRecord::m_pPolyMesh point at them; [U] cFlexBridgeNode_intersect (src/object/flexbridgenode.cpp, view
// sFMesh_7268) builds one on the stack from its own grid (faces, boxes, vertices, normals) for
// cWorldIntersectType_IsMeshInt.
struct tPolyModelMesh {
    unsigned short NumFaces;        // 0x00 [O]; [U] the triangle count
    unsigned short NumVertices;     // 0x02 [O]; [U] the vertex count
    unsigned char (*Face)[3];       // 0x04 [O] unsigned char[3]*; [U] 3-byte vertex index triples
    tPolyBoundBox* Boxes;           // 0x08 [O]; [U] the per-face boxes
    t4Vector* Vertex;               // 0x0C [O]; [U] the vertices
    t4Vector* Normal;               // 0x10 [O]; [U] the face normals
};

CHECK_OFFSET(tPolyModelMesh, NumFaces, 0x00);
CHECK_OFFSET(tPolyModelMesh, NumVertices, 0x02);
CHECK_OFFSET(tPolyModelMesh, Face, 0x04);
CHECK_OFFSET(tPolyModelMesh, Boxes, 0x08);
CHECK_OFFSET(tPolyModelMesh, Vertex, 0x0C);
CHECK_OFFSET(tPolyModelMesh, Normal, 0x10);
CHECK_SIZE(tPolyModelMesh, 0x14);

// A collision model's polygon meshes (On Tour tPolyModelList, 8 bytes; method 1). [O] getMeshPolyModel(meshNum) =
// MeshPolyModel[meshNum] (retail tPolyModelList_getMeshPolyModel: + meshNum * 4).
struct tPolyModelList {
    tPolyModelMesh** MeshPolyModel; // 0x0 [O]; [U] resolveCollideModel (method 1) fills it;
                                    //      tPolyModelList_getMeshPolyModel indexes it
    int NumMesh;                    // 0x4 [O]; [U] resolveCollideModel (method 1) stores the count and loops to it
};

CHECK_OFFSET(tPolyModelList, MeshPolyModel, 0x0);
CHECK_OFFSET(tPolyModelList, NumMesh, 0x4);
CHECK_SIZE(tPolyModelList, 0x8);

// A collision model (On Tour tCollideModelRecord : tCollideModelRecord_PSP, 0x10; same layout). The three words at
// 0x4 / 0x8 / 0xC are offsets in the file (-1 = none) that resolveCollideModel turns into pointers (view
// sChunk_3AAA08 in src/world/worldview.cpp). Union member names from the prx DIEs.
struct tCollideModelRecord {
    unsigned short m_usMethod;      // 0x0 [O]; [U] resolveCollideModel: 3 = sphere trees, 1 = polygon meshes
    unsigned short m_usNumMesh;     // 0x2 [O]; [U] resolveCollideModel copies it into the list's NumMesh
    union {
        char* m_pColData;           // 0x4 [O]; [U] the packed tree data bxSphereTree_Load reads
        int m_iColDataOffset;       // 0x4 [O]; [U] the offset before the fix-up
    };
    union {
        tPolyModelList* m_pPolyModelList; // 0x8 [O]; [O] cWorld::getPolyModelInfo returns it
        tSphTreeList* m_pSphTreeList;     // 0x8 [O]; [O] cWorld::getSphereTreeInfo returns it; [U] resolveCollideModel
        int m_iListOffset;          // 0x8 [O]; [U] the offset before the fix-up
    };
    union {
        bxSphereTree* m_pSphTree;   // 0xC [O]; [U] the tree array (method 3)
        tPolyModelMesh** m_pPolyMesh; // 0xC [O]; [U] the mesh pointer array (method 1)
        int m_iGeomOffset;          // 0xC [O]; [U] the offset before the fix-up
    };
};

CHECK_OFFSET(tCollideModelRecord, m_usMethod, 0x0);
CHECK_OFFSET(tCollideModelRecord, m_usNumMesh, 0x2);
CHECK_OFFSET(tCollideModelRecord, m_pColData, 0x4);
CHECK_OFFSET(tCollideModelRecord, m_pSphTreeList, 0x8);
CHECK_OFFSET(tCollideModelRecord, m_pSphTree, 0xC);
CHECK_SIZE(tCollideModelRecord, 0x10);

// An instance's collision record (On Tour tCollideRecord, 0x10). tInstanceRecord::m_pCollideRecord points here
// (cWScriptCacheEntry_resolve stores it from the script file's CollideRecordEntryTable). Retail reads a 12-byte
// per-mesh table right after the record (+0x10 + 12 * mesh; the halfword at +0xA of an entry is the mesh's surface
// type, cFlexRailNode_splineIntersect / cFlexBridgeNode_intersect); On Tour has no type for it, so it is not laid out.
struct tCollideRecord {
    int Method;                     // 0x0 [O]; [P] "->Method == CR_Polygon"; [U] cWScriptCacheEntry_resolve resolves
                                    //      the model for methods 1 and 3; the terrain / static queries test it
    int unk_004;                    // 0x4 [O] InitFlags
    unsigned int unk_008;           // 0x8 [O] tResourceID InstanceScriptId
    union {
        tCollideModelRecord* CollideModel; // 0xC [O]; [P] "->CollideModel"; [U] cWScriptCacheEntry_resolve stores the
                                    //      resolved model (or 0); cWorld_getSphereTreeInfo / getPolyModelInfo read it
        unsigned int CollideModelRID; // 0xC [O]; [U] the model's resource id before the fix-up (resolved with type 100)
    };
};

CHECK_OFFSET(tCollideRecord, Method, 0x0);
CHECK_OFFSET(tCollideRecord, unk_004, 0x4);
CHECK_OFFSET(tCollideRecord, unk_008, 0x8);
CHECK_OFFSET(tCollideRecord, CollideModel, 0xC);
CHECK_SIZE(tCollideRecord, 0x10);

#endif /* WORLD_WORLDRECORDS_H */
