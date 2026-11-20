#ifndef WSCRIPT_WSCRIPTCACHE_H
#define WSCRIPT_WSCRIPTCACHE_H

// cWScriptCache: the world's per-section Luno script cache (cWorld::m_pWScriptCache; cWorld_cWorld allocates it as
// cWScriptCache_cWScriptCache(cMemMan_alloc(0x10, "cWScriptCache"), this)). Methods in world/wscriptcache.
// Size 0x10 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cWorld;
struct tScriptFileHeader;

// One world section's cached script (cWScriptCache::mpCache, one per section). Size 0xC [R]: cWScriptCache_init
// allocates the array with a 0xC stride (behind a 0x10-byte count header) and constructs each entry
// (cWScriptCacheEntry_cWScriptCacheEntry: mState 0, mAsyncHandle -1, no header). Member names are EA's ([O]); every
// retail access through `this` in its 13 methods falls on a member start.
struct cWScriptCacheEntry {
    short mState;                   // 0x000 [O]; [R] ctor 0
    short mStreamed;                // 0x002 [O]; [R] ctor 0
    int mAsyncHandle;               // 0x004 [O]; [R] ctor -1
    tScriptFileHeader* mpScriptHeader; // 0x008 [O]; [R] ctor 0
};

CHECK_OFFSET(cWScriptCacheEntry, mAsyncHandle, 0x004);
CHECK_OFFSET(cWScriptCacheEntry, mpScriptHeader, 0x008);
CHECK_SIZE(cWScriptCacheEntry, 0xC);

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
