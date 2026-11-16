#include "common.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"

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
struct sWorldVEntry0 {
    short delta;
    short index;
    void (*fn)(void*);
};

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
    sWorldVEntry0* vtEntry = &((sWorldVEntry0*)obj->mVtbl)[107];
    vtEntry->fn((char*)obj + vtEntry->delta);
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
extern "C" void cWorld_init(cWorld* self)
{
    cWorld_resetMap(self);
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
 * @param selfp  the world
 */
extern "C" void cWorld_resetMap(cWorld* selfp)
{
    sWRMWorld** self = (sWRMWorld**)selfp;
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
void* cWorld_update(void* self)
{
    return cWorldCache_update(*(int*)self);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6948);
#ifdef SKIP_ASM
struct cBXString;
extern "C" void* cBXString_cBXString2(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" cBXString* func_00318630(cBXString* self, cBXString* a, const char* str);
extern "C" cBXString* func_003186D0(cBXString* self, const char* str, cBXString* b);
extern "C" void cBXString_makeUpper(cBXString* self);
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" int func_004165A8(const void* a, const void* b);
void cMemMan_free(void*);
extern char D_004A47C0[];
extern char str_sdb[];

struct sName_3A6948 {
    char* name;
    int ok;
};

struct sEntry_3A6948 {
    char name[0x58];
};

struct sFile_3A6948 {
    unsigned short magic;       // 0x00
    char pad_2[6];
    unsigned int count;         // 0x08
    char pad_c[8];
    sEntry_3A6948* entries;     // 0x14
};

struct sStr_3A6948 {
    char* s;
    sStr_3A6948() {}
    sStr_3A6948(const sStr_3A6948&);
};

/**
 * Checks which of @p n section names exist in a track's .sdb section table.
 *
 * Loads the file named D_004A47C0 + @p path + str_sdb with FILE_loadz. When it loaded and its first halfword is 0x3800,
 * each name is upper-cased and compared (func_004165A8) with the 0x58-byte section records after the 0x50-byte header
 * (count at +0x8); a found name gets its flag set (names[i].ok = 1, else 0). The file is freed afterwards. When the
 * file is missing or has the wrong cookie the flags are left untouched. Called by cTrackFileMan_scanLocations' helpers
 * (func_0017A830, func_0017A978).
 *
 * @param path  the track's base name
 * @param n  how many names to check
 * @param names  the names, each with a found flag
 * @return 1 when at least one name was found, else 0
 */
extern "C" int func_003A6948(const char* path, int n, sName_3A6948* names)
{
    sFile_3A6948* data;
    int ret;
    {
        sStr_3A6948 full;
        sStr_3A6948 tmp;
        sStr_3A6948 str;
        char* pre = D_004A47C0;
        cBXString* t = (cBXString*)&tmp;
        cBXString_cBXString2(&str, path);
        func_003186D0(t, pre, (cBXString*)&str);
        func_00318630((cBXString*)&full, t, str_sdb);
        data = (sFile_3A6948*)FILE_loadz(full.s, 0x3000000);
        cBXString__cBXString(&full, 2);
        cBXString__cBXString(t, 2);
        cBXString__cBXString(&str, 2);
    }
    ret = data && data->magic == 0x3800;
    if (ret)
    {
        int i;
        data->entries = (sEntry_3A6948*)((char*)data + 0x50);
        ret = 0;
        for (i = 0; i < n; i++)
        {
            sStr_3A6948 s;
            unsigned int j;
            names[i].ok = 0;
            cBXString_cBXString2(&s, names[i].name);
            cBXString_makeUpper((cBXString*)&s);
            for (j = 0; j < data->count; j++)
            {
                if (func_004165A8(&data->entries[j], s.s) == 0)
                {
                    names[i].ok = ret = 1;
                    break;
                }
            }
            cBXString__cBXString(&s, 2);
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
extern "C" void cWorld_load(void** self, const char* name, int nash)
{
    cWorld_load_sBXString nameStr;
    cBXString_cBXString2(&nameStr, name);
    cWorldCache_reset(self[0]);
    cWorldCache_init(self[0], name, nash);
    cWScriptCache_init(self[1], cWorldView_getNumSections((cWorldView*)((char*)self[0] + 0x10)));
    cWorldPainterMan_init(self[2], cWorldView_getNumSections((cWorldView*)((char*)self[0] + 0x10)));
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

extern "C" int func_003AD120(cWorld_getWScriptHeaderId_sEntry* e, void* obj, int i);

/**
 * Returns the resource id of one of an instance's world-script headers, from the script cache of the
 * instance's section (+0x78).
 *
 * @param obj  the instance (On Tour const tInstanceRecord*)
 * @param i    which script entry
 * @return the header's resource id (On Tour tResourceID)
 */
extern "C" int cWorld_getWScriptHeaderId(cWorld* self, void* obj, int i)
{
    return func_003AD120(&((cWorld_getWScriptHeaderId_sTable*)self->m_pWScriptCache)->entries[*(unsigned char*)((char*)obj + 0x78)], obj, i);
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

extern "C" int func_003ADC80(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns how many global world scripts a section has.
 *
 * @param i  section id
 * @return the number of global scripts
 */
extern "C" int cWorld_getNumGlobalScripts(cWorld* self, int i)
{
    return func_003ADC80(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getGlobalScripts);
#ifdef SKIP_ASM
extern "C" int func_003ADCB0(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns a section's global world scripts.
 *
 * @param i  section id
 * @return the global script records (On Tour const tWScriptGlobalData*)
 */
extern "C" int cWorld_getGlobalScripts(cWorld* self, int i)
{
    return func_003ADCB0(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
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
extern "C" int cWorld_getNumProcessScripts(cWorld* self, int i)
{
    return cWScriptCacheEntry_getNumProcessScripts(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
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
extern "C" int cWorld_getProcessScripts(cWorld* self, int i)
{
    return cWScriptCacheEntry_getProcessScripts(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getNumMissionScripts);
#ifdef SKIP_ASM
extern "C" int func_003ADD40(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns how many mission scripts a section has.
 *
 * @param i  section id
 * @return the number of mission scripts
 */
extern "C" int cWorld_getNumMissionScripts(cWorld* self, int i)
{
    return func_003ADD40(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getMissionScripts);
#ifdef SKIP_ASM
extern "C" int func_003ADD70(cWorld_getNumGlobalScripts_sEntry* e);

/**
 * Returns a section's mission scripts.
 *
 * @param i  section id
 * @return the mission script records (On Tour const tWScriptMissionData*)
 */
extern "C" int cWorld_getMissionScripts(cWorld* self, int i)
{
    return func_003ADD70(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[i]);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6CC8__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns a world instance's sphere-tree list: the word at +0x8 of the record that the instance's +0x88 record links at
 * +0xC (func_003ACCD8 puts a collide model there).
 *
 * No null checks. The body is identical to func_003A6CD8's.
 *
 * @param a1  the tInstanceRecord
 * @return the list pointer
 */
int func_003A6CC8(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)a1 + 0x88) + 0xc) + 0x8);
}
#endif

//100%
INCLUDE_ASM("world/world", func_003A6CD8__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns a world instance's poly-model list: the word at +0x8 of the record that the instance's +0x88 record links at
 * +0xC (func_003ACCD8 puts a collide model there).
 *
 * No null checks. The body is identical to func_003A6CC8's; func_0035DF70 (railmodifier) passes its result to
 * tSphTreeList_getMeshTree.
 *
 * @param a1  the tInstanceRecord
 * @return the list pointer
 */
int func_003A6CD8(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)*(void**)((char*)a1 + 0x88) + 0xc) + 0x8);
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
extern "C" void* tSphTreeList_getMeshTree(void* self, int meshNum)
{
    return (char*)(*(void**)self) + meshNum * 0x140;
}
#endif

//100%
INCLUDE_ASM("world/world", tPolyModelList_getMeshPolyModel__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the collision poly model of one mesh of a model.
 *
 * @param meshNum  mesh index (On Tour meshNum)
 * @return entry @p meshNum of the pointer array at +0x0
 */
int tPolyModelList_getMeshPolyModel(void* self, int meshNum)
{
    return *(int*)((char*)*(void**)self + meshNum * 4);
}
#endif

//100%
INCLUDE_ASM("world/world", cWorld_getWScriptScript);
#ifdef SKIP_ASM
extern "C" int func_003ADC50(cWorld_getNumGlobalScripts_sEntry* e, unsigned int i);

/**
 * Returns the code of a world script given its resource id (low byte: section; the rest: index).
 *
 * @param handle  the script's resource id (On Tour const tResourceID&)
 * @return the script code (On Tour const char*)
 */
extern "C" int cWorld_getWScriptScript(cWorld* self, unsigned int* handle)
{
    unsigned int packed = *handle;
    return func_003ADC50(&((cWorld_getNumGlobalScripts_sTable*)self->m_pWScriptCache)->entries[packed & 0xFF], packed >> 8);
}
#endif

