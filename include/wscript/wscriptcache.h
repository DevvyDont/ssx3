#ifndef WSCRIPT_WSCRIPTCACHE_H
#define WSCRIPT_WSCRIPTCACHE_H

// cWScriptCache: the world's per-section Luno script cache (cWorld::m_pWScriptCache; cWorld_cWorld allocates it as
// cWScriptCache_cWScriptCache(cMemMan_alloc(0x10, "cWScriptCache"), this)). Methods in world/wscriptcache.
// Size 0x10 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cWScriptCacheEntry;
struct cWorld;

struct cWScriptCache {
    cWorld* mpWorld;                // 0x000 [O]
    cWScriptCacheEntry* mpCache;    // 0x004 [O]
    unsigned int mNumSections;      // 0x008 [O]
    int mNumLoading;                // 0x00C [O]
};

CHECK_OFFSET(cWScriptCache, mpWorld, 0x000);
CHECK_OFFSET(cWScriptCache, mpCache, 0x004);
CHECK_OFFSET(cWScriptCache, mNumSections, 0x008);
CHECK_OFFSET(cWScriptCache, mNumLoading, 0x00C);
CHECK_SIZE(cWScriptCache, 0x10);

#endif
