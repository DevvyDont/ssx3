#include "common.h"
#include "world/instancerecord.h"
#include "bx/bxstring.h"
#include "fe/debugfe.h"
#include "world/worldcache.h"
#include "world/worldrecords.h"
#include "vtable.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "world/worldview.h"
#include "object/rollermodifier.h"
//100%
INCLUDE_ASM("world/world", cWorld_cWorld);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldCache_cWorldCache(void* mem);
extern "C" void* cWScriptCache_cWScriptCache(void* mem, void* world);
extern "C" void* cWorldPainterMan_cWorldPainterMan(void* mem);
extern "C" void SphereTree_initCache(int a, int b, int c, int d);
extern char str_cWorldCache[];
extern char str_cWScriptCache[];
extern char str_cWorldPainterMan_b[];
// The unit declares cWorld_m_pTheWorld as int (later); this view stores the world pointer.

/**
 * Constructs the world and makes it the current one (cWorld_m_pTheWorld).
 *
 * Creates the world cache, the world-script cache and the world painter manager, clears +0x10/+0x14 and sets
 * up the sphere-tree cache (SphereTree_initCache(10, 4, 2, 5)).
 *
 * @return self
 */
extern "C" void* cWorld_cWorld(void** self)
{
    cWorld_m_pTheWorld = (cWorld*)(self);
    self[0] = cWorldCache_cWorldCache(cMemMan_alloc(0x1C00, str_cWorldCache, 0, 0));
    self[1] = cWScriptCache_cWScriptCache(cMemMan_alloc(0x10, str_cWScriptCache, 0, 0), self);
    self[2] = cWorldPainterMan_cWorldPainterMan(cMemMan_alloc(8, str_cWorldPainterMan_b, 0, 0));
    self[4] = 0;
    self[5] = 0;
    SphereTree_initCache(10, 4, 2, 5);
    return self;
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld__cWorld);
#ifdef SKIP_ASM
extern "C" void cWorldCache__cWorldCache(int* self, int flags);
extern "C" void cWScriptCache__cWScriptCache(void* self, int flags);
extern "C" void cWorldPainterMan__cWorldPainterMan(void* self, int flags);
extern "C" void SphereTree_purgeCache(void);
void operator_delete(int* p);

/**
 * Destroys the world: graphics manager slot 107, then deletes its caches and the world painter manager.
 *
 * Also calls SphereTree_purgeCache() and clears cWorld_m_pTheWorld.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorld__cWorld(void** self, int flags)
{
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    obj->PurgeMeshData();
    if (self[0] != 0) {
        cWorldCache__cWorldCache((int*)self[0], 3);
    }
    if (self[1] != 0) {
        cWScriptCache__cWScriptCache(self[1], 3);
    }
    if (self[2] != 0) {
        cWorldPainterMan__cWorldPainterMan(self[2], 3);
    }
    SphereTree_purgeCache();
    cWorld_m_pTheWorld = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void cWorld_resetMap(cWorld*);

//100%
INCLUDE_ASM("world/world", cWorld_init);
#ifdef SKIP_ASM
/**
 * Initialises the world by resetting its map with cWorld_resetMap().
 */
void cWorld::init()
{
    resetMap();
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_resetMap);
#ifdef SKIP_ASM
struct cWorldView;
int cWorldView_getNumSections(cWorldView* view);
int cWorldView_isSectionLoaded(cWorldView* view, int i);

struct sWRMEntry {
    unsigned int lo : 8;
    unsigned int ptr : 24;
};

struct sWRMTable {
    char pad00[0x1C];
    sWRMEntry* entries;
};

struct sWRMObj {
    int f0;
    int f4;
    int f8;
};

struct sWRMSection {
    char pad00[0x26];
    short count;
    char pad28[0x30];
};

struct sWRMMap {
    char pad00[0x14];
    sWRMSection* sections;
};

struct sWRMWorld {
    sWRMMap* map;
    int f04;
    sWRMTable** tables;
    int f0C;
    char view[4];
};

static inline sWRMObj* sWRM_toPtr(unsigned int p)
{
    return (sWRMObj*)(p << 2);
}

static inline sWRMObj* sWRM_lookup(sWRMTable* t, unsigned int idx)
{
    unsigned int p = t->entries[idx].ptr;
    if (p == 0) return 0;
    return sWRM_toPtr(p);
}

static inline sWRMObj* sWRM_get(sWRMWorld* w, unsigned int id)
{
    sWRMTable* t = w->tables[id & 0xFF];
    if (t == 0) return 0;
    return sWRM_lookup(t, id >> 8);
}

/**
 * Resets the state flags of every object in the loaded world sections.
 *
 * For each loaded section of the world view, looks up each of its objects by handle (object index << 8 | section
 * index) and rewrites the object's flag word: the low half is replaced by the high half, probably the initial
 * state, and bit 1 (value 2) is set. cInstanceNode_loseFocus applies the same change to one instance.
 *
 */
void cWorld::resetMap()
{
    sWRMWorld** self = (sWRMWorld**)this;
    unsigned int ref = 0xFFFFFFFF;
    unsigned int i;
    for (i = 0; i < (unsigned int)cWorldView_getNumSections((cWorldView*)(*self)->view); i++) {
        if (cWorldView_isSectionLoaded((cWorldView*)(*self)->view, i)) {
            int n = (*self)->map->sections[i].count;
            unsigned int slotIndex = i & 0xFF;
            ref = (int)ref & ~0xFF;
            ref |= slotIndex;
            int j;
            for (j = 0; j < n; j++) {
                ref = (ref & 0xFF) | (j << 8);
                sWRMObj* mapObj = sWRM_get(*self, ref);
                if (mapObj) {
                    int highBits = mapObj->f8 & 0xFFFF0000;
                    mapObj->f8 = highBits | (highBits >> 16) | 2;
                }
            }
        }
    }
}
#endif

extern "C" void* cWorldCache_update(int);

//100%
INCLUDE_ASM("world/world", cWorld_update__FPv);
#ifdef SKIP_ASM
/**
 * Runs the world's per-frame streaming update.
 *
 * Calls cWorldCache_update on the cache at +0x0 and returns its result.
 *
 * @return cWorldCache_update's result
 */
void* cWorld::update()
{
    return cWorldCache_update(*(int*)this);
}
#endif

//100%
INCLUDE_ASM("world/world", isValidMap);
#ifdef SKIP_ASM
struct cBXString;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" cBXString* func_00318630(cBXString* self, cBXString* a, const char* str);
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* b);
extern "C" void cBXString_makeUpper(cBXString* self);
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" int strcmp(const void* a, const void* b);
void cMemMan_free(void*);
extern char str_bar_e[];
extern char str_sdb[];



/**
 * Checks which of @p n section names exist in a track's .sdb section table.
 *
 * Loads the file named str_bar_e + @p path + str_sdb with FILE_loadz. When it loaded and its cookie's first halfword is
 * tStreamDataHeader::kCookie (0x3800), each name is upper-cased and compared (strcmp) with the m_uiNumSections section
 * records (tBxSection, after the 0x50-byte tStreamDataHeader); a found name gets its flag set (names[i].bFound = 1,
 * else 0). The file is freed afterwards. When the file is missing or has the wrong cookie the flags are left untouched.
 * Called by cTrackFileMan_scanLocations' helpers (cTrackFileMan_checkLocation, cTrackFileMan_checkLocation1).
 *
 * @param path  the track's base name
 * @param n  how many names to check
 * @param names  the names, each with a found flag
 * @return 1 when at least one name was found, else 0
 */
extern "C" int isValidMap(const char* path, int n, tWorldSectionScan* names)
{
    tStreamDataHeader* data;
    int ret;
    {
        cBXString full;
        cBXString prefixedBuf;
        cBXString str;
        char* pre = str_bar_e;
        cBXString* prefixed = (cBXString*)&prefixedBuf;
        cBXString_cBXString2(&str, path);
        func_003186D0(prefixed, pre, (cBXString*)&str);
        func_00318630((cBXString*)&full, prefixed, str_sdb);
        data = (tStreamDataHeader*)FILE_loadz(full.Str, 0x3000000);
        cBXString__cBXString(&full, 2);
        cBXString__cBXString(prefixed, 2);
        cBXString__cBXString(&str, 2);
    }
    ret = data && *(unsigned short*)data->m_cCookie == tStreamDataHeader::kCookie;
    if (ret)
    {
        int i;
        // ORIGINAL-PTR: m_pSection is a pointer in the loaded .sdb file (On Tour tStreamDataHeader); like
        // cWorldCache_init this fixes it up to the section records that follow the 0x50-byte header
        data->m_pSection = (tBxSection*)((char*)data + 0x50);
        ret = 0;
        for (i = 0; i < n; i++)
        {
            cBXString upperName;
            unsigned int j;
            names[i].bFound = 0;
            cBXString_cBXString2(&upperName, ((char*)names[i].sectionName));
            cBXString_makeUpper((cBXString*)&upperName);
            for (j = 0; j < data->m_uiNumSections; j++)
            {
                if (strcmp(data->m_pSection[j].m_sSectionName, upperName.Str) == 0)
                {
                    names[i].bFound = ret = 1;
                    break;
                }
            }
            cBXString__cBXString(&upperName, 2);
        }
    }
    if (data)
        cMemMan_free(data);
    return ret;
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_load);
#ifdef SKIP_ASM
struct cWorldView;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cWorldCache_reset(void* cache);
extern "C" void cWorldCache_init(void* cache, const char* name, int a2);
int cWorldView_getNumSections(cWorldView* view);
extern "C" void cWScriptCache_init(void* cache, int n);
extern "C" void cWorldPainterMan_init(void* p, int n);

struct cWorld_load_sBXString {
    char* str;
};

/**
 * Loads a world: resets and initialises the world cache, then sizes the script cache and the world painter
 * for its sections.
 *
 * A cBXString copy of the name is made and destroyed without being used.
 *
 * @param name  the world's base file name (On Tour base)
 * @param nash  the Nash collision grid to use (On Tour nash::Nash*)
 */
void cWorld::load(const char* name, int nash)
{
    cWorld_load_sBXString nameStr;
    cBXString_cBXString2(&nameStr, name);
    cWorldCache_reset(m_pWorldCache);
    cWorldCache_init(m_pWorldCache, name, nash);
    cWScriptCache_init(m_pWScriptCache, cWorldView_getNumSections(&m_pWorldCache->m_WorldView));
    cWorldPainterMan_init(m_pWorldPainterMan, cWorldView_getNumSections(&m_pWorldCache->m_WorldView));
    cBXString__cBXString(&nameStr, 2);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getWScriptHeaderId);
#ifdef SKIP_ASM
struct cWorld_getWScriptHeaderId_sEntry {
    short type;
    char pad2[6];
    void* data;
};

struct cWorld_getWScriptHeaderId_sTable {
    char pad[4];
    cWorld_getWScriptHeaderId_sEntry* entries;
};

extern "C" int cWScriptCacheEntry_getWScriptHeaderId(cWorld_getWScriptHeaderId_sEntry* e, void* obj, int i);

/**
 * Returns the resource id of one of an instance's world-script headers, from the script cache of the
 * instance's section (+0x78).
 *
 * @param obj  the instance (On Tour const tInstanceRecord*)
 * @param i    which script entry
 * @return the header's resource id (On Tour tResourceID)
 */
int cWorld::getWScriptHeaderId(tInstanceRecord* obj, int i)
{
    return cWScriptCacheEntry_getWScriptHeaderId(&((cWorld_getWScriptHeaderId_sTable*)m_pWScriptCache)->entries[*(unsigned char*)&obj->m_RID], obj, i);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getNumGlobalScripts);
#ifdef SKIP_ASM
struct cWorld_getNumGlobalScripts_sEntry {
    short type;
    char pad2[6];
    void* data;
};

struct cWorld_getNumGlobalScripts_sTable {
    char pad[4];
    cWorld_getNumGlobalScripts_sEntry* entries;
};

extern "C" int cWScriptCacheEntry_getNumGlobalScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns how many global world scripts a section has.
 *
 * @param i  section id
 * @return the number of global scripts
 */
int cWorld::getNumGlobalScripts(int i)
{
    return cWScriptCacheEntry_getNumGlobalScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getGlobalScripts);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getGlobalScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns a section's global world scripts.
 *
 * @param i  section id
 * @return the global script records (On Tour const tWScriptGlobalData*)
 */
int cWorld::getGlobalScripts(int i)
{
    return cWScriptCacheEntry_getGlobalScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getNumProcessScripts);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getNumProcessScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns how many process scripts a world-script cache entry holds.
 *
 * @param i  index into the world-script cache's entries
 * @return the entry data's count at offset 0x28, or 0 when the entry's state is not 3 (probably loaded) or it has
 *         no data
 */
int cWorld::getNumProcessScripts(int i)
{
    return cWScriptCacheEntry_getNumProcessScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getProcessScripts);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getProcessScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns the process-script list of a world-script cache entry.
 *
 * @param i  index into the world-script cache's entries
 * @return the entry data's word at offset 0x2C, or 0 when the entry's state is not 3 (probably loaded) or it has
 *         no data
 */
int cWorld::getProcessScripts(int i)
{
    return cWScriptCacheEntry_getProcessScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getNumMissionScripts);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getNumMissionScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns how many mission scripts a section has.
 *
 * @param i  section id
 * @return the number of mission scripts
 */
int cWorld::getNumMissionScripts(int i)
{
    return cWScriptCacheEntry_getNumMissionScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getMissionScripts);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getMissionScripts(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns a section's mission scripts.
 *
 * @param i  section id
 * @return the mission script records (On Tour const tWScriptMissionData*)
 */
int cWorld::getMissionScripts(int i)
{
    return cWScriptCacheEntry_getMissionScripts(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getSphereTreeInfo__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns a world instance's sphere-tree list: m_pSphTreeList of the collide model (CollideModel) that the instance's
 * m_pCollideRecord links (cWScriptCacheEntry_resolve puts the model there).
 *
 * No null checks. The body is identical to cWorld_getPolyModelInfo's.
 *
 * @param instance  the tInstanceRecord
 * @return the list pointer
 */
int cWorld::getSphereTreeInfo(void* instance)
{
    return (int)((tInstanceRecord*)instance)->m_pCollideRecord->CollideModel->m_pSphTreeList;
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getPolyModelInfo__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns a world instance's poly-model list: m_pPolyModelList of the collide model (CollideModel) that the
 * instance's m_pCollideRecord links (cWScriptCacheEntry_resolve puts the model there).
 *
 * No null checks. The body is identical to cWorld_getSphereTreeInfo's; cRollerModifier_cRollerModifier1 (railmodifier) passes its result to
 * tSphTreeList_getMeshTree.
 *
 * @param instance  the tInstanceRecord
 * @return the list pointer
 */
int cWorld::getPolyModelInfo(void* instance)
{
    return (int)((tInstanceRecord*)instance)->m_pCollideRecord->CollideModel->m_pPolyModelList;
}
#endif

//100%
INCLUDE_ASM("world/world", tSphTreeList_getMeshTree);
#ifdef SKIP_ASM
/**
 * Returns the collision sphere tree of one mesh of a model.
 *
 * @param meshNum  mesh index (On Tour meshNum)
 * @return the tree at index @p meshNum of the array at +0x0 (0x140 bytes each)
 */
void* tSphTreeList::getMeshTree(int meshNum)
{
    return &MeshTree[meshNum];
}
#endif

//100%
INCLUDE_ASM("world/world", tPolyModelList_getMeshPolyModel__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the collision poly model of one mesh of a model.
 *
 * @param meshNum  mesh index (On Tour meshNum)
 * @return entry @p meshNum of MeshPolyModel
 */
int tPolyModelList_getMeshPolyModel(void* vself, int meshNum)
{
    tPolyModelList* self = (tPolyModelList*)vself;
    return (int)((tPolyModelList*)self)->MeshPolyModel[meshNum];
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getWScriptScript);
#ifdef SKIP_ASM
extern "C" int cWScriptCacheEntry_getWScriptScript(cWorld_getNumGlobalScripts_sEntry* e, unsigned int i);

/**
 * Returns the code of a world script given its resource id (low byte: section; the rest: index).
 *
 * @param handle  the script's resource id (On Tour const tResourceID&)
 * @return the script code (On Tour const char*)
 */
int cWorld::getWScriptScript(unsigned int* handle)
{
    unsigned int packed = *handle;
    return cWScriptCacheEntry_getWScriptScript(&((cWorld_getNumGlobalScripts_sTable*)m_pWScriptCache)->entries[packed & 0xFF], packed >> 8);
}
#endif

