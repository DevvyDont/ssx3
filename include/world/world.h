#ifndef WORLD_WORLD_H
#define WORLD_WORLD_H

// cWorld: the open mountain (one instance per loaded track). cGame_load allocates it as
// `cWorld_cWorld(operator new(0x18, "World"))` and stores it in cGame::World; the constructor also stores itself in
// the class static cWorld::m_pTheWorld (cWorld_m_pTheWorld; preview asserts "m_pTheWorld == 0" / "m_pTheWorld == this"), which
// over 100 retail functions read. The cWorld itself only owns three sub-objects; the streaming lives in cWorldCache
// (world/worldcache.h). No vtable, no base class. EA's source: d:/ssx/code/source/world/world.cpp (preview assert
// strings). Methods (world/world, 0x003A6688-0x003A6D50; names from On Tour's cWorld methods):
//   cWorld_cWorld, cWorld__cWorld, cWorld_init, cWorld_resetMap, cWorld_update (update), cWorld_load,
//   cWorld_getWScriptHeaderId (getWScriptHeaderId), cWorld_getNumGlobalScripts (getNumGlobalScripts), cWorld_getGlobalScripts (getGlobalScripts),
//   cWorld_getNumProcessScripts, cWorld_getProcessScripts, cWorld_getNumMissionScripts (getNumMissionScripts),
//   cWorld_getMissionScripts (getMissionScripts), func_003A6CC8 (getSphereTreeInfo), func_003A6CD8 (getPolyModelInfo),
//   cWorld_getWScriptScript (getWScriptScript). Not methods: func_003A6948 (a static .sdb section-name check: path, count,
//   names), tSphTreeList_getMeshTree and tPolyModelList_getMeshPolyModel (their first argument is a tPolyModelList / tSphTreeList).
//
// Size 0x18 [R] (the allocation; On Tour 0x18 with the same six members).
// A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF (class cWorld), placed by:
//   [R] retail's constructor cWorld_cWorld (allocation tags "cWorldCache", "cWScriptCache", "cWorldPainterMan");
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [R] or [U].

#include "layout.h"

struct cWorldCache;
struct cWScriptCache;
struct cWorldPainterMan;
struct tSphTreeList;
struct tPolyModelList;

struct cWorld {
    cWorldCache* m_pWorldCache;          // 0x00 [O]; [R] cWorldCache_cWorldCache(cMemMan_alloc(0x1C00, "cWorldCache"));
                                         //      [U] update/load/resetMap; ~110 functions read m_pTheWorld->m_pWorldCache
    cWScriptCache* m_pWScriptCache;      // 0x04 [O]; [R] cWScriptCache_cWScriptCache(cMemMan_alloc(0x10,
                                         //      "cWScriptCache"), this); [U] the script getters (cWorld_getWScriptHeaderId ...)
    cWorldPainterMan* m_pWorldPainterMan; // 0x08 [O]; [R] cWorldPainterMan_cWorldPainterMan(cMemMan_alloc(8,
                                         //      "cWorldPainterMan")); [U] load sizes it per section (cWorldPainterMan_init)
    char* m_pDataMaster;                 // 0x0C [O] alone: no retail code reads or writes it (the ctor leaves it)
    tSphTreeList* m_pSphTrees;           // 0x10 [O]; [R] ctor = 0
    tPolyModelList* m_pPolyModels;       // 0x14 [O]; [R] ctor = 0
};

CHECK_OFFSET(cWorld, m_pWorldCache, 0x00);
CHECK_OFFSET(cWorld, m_pWScriptCache, 0x04);
CHECK_OFFSET(cWorld, m_pWorldPainterMan, 0x08);
CHECK_OFFSET(cWorld, m_pDataMaster, 0x0C);
CHECK_OFFSET(cWorld, m_pSphTrees, 0x10);
CHECK_OFFSET(cWorld, m_pPolyModels, 0x14);
CHECK_SIZE(cWorld, 0x18);

// The one world (On Tour: cWorld::m_pTheWorld): cWorld_cWorld stores this, the destructor clears it.
extern cWorld* cWorld_m_pTheWorld;

#endif /* WORLD_WORLD_H */
