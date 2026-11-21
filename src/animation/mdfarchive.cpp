#include "common.h"
#include "animation/mdfarchive.h"
#include "animation/gameanimmap.h"
#include "animation/adfdatasource.h"

//100%
INCLUDE_ASM("animation/mdfarchive", cMdfArchive_getModelPartByIndex);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void MEM_copy(void*, void*, int);
extern "C" void unpack(void* src, void* dst);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_MdfModel_pct_s[];

struct sMdfPart_00314CB0
{
    char name[0x10];
    int dataOffset;
    int dataSize;
    char pad_0x18[0x48];
};


/**
 * Returns a new heap copy of one archive part: its 0x60-byte header followed by the unpacked data.
 *
 * The block is dataSize + 0x60 bytes, tagged "MdfModel <part name>" with flags 0x3000000. When dataSize is
 * not 0, the part's packed data at data + dataOffset is unpacked behind the header with unpack (EA's
 * packed-data decoder).
 *
 * @param i  part index in the archive
 * @return the new block (On Tour: tMdfModelHeader*)
 */
extern "C" void* cMdfArchive_getModelPartByIndex(cMdfArchive* self, int i)
{
    char name[0x40];
    sprintf(name, str_MdfModel_pct_s, &((sMdfPart_00314CB0*)self->ModelHeaders)[i]);
    char* model = (char*)operator_new_tag(((sMdfPart_00314CB0*)self->ModelHeaders)[i].dataSize + 0x60, name, 0x3000000, 0);
    MEM_copy(model, &((sMdfPart_00314CB0*)self->ModelHeaders)[i], 0x60);
    if (((sMdfPart_00314CB0*)self->ModelHeaders)[i].dataSize)
    {
        unpack(self->Data + ((sMdfPart_00314CB0*)self->ModelHeaders)[i].dataOffset, model + 0x60);
    }
    return model;
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", gameanimcmp);
#ifdef SKIP_ASM
extern unsigned int* gameanimcmphack;

/**
 * qsort comparator that orders animation indices by their key in gameanimcmphack.
 *
 * Returns -1 when gameanimcmphack[*@p lhs] < gameanimcmphack[*@p rhs], else 1 (never 0, so equal keys are not reported as
 * equal).
 *
 * @param lhs  the first animation index (On Tour p0)
 * @param rhs  the second animation index (On Tour p1)
 * @return -1 or 1
 */
extern "C" int gameanimcmp(int* lhs, int* rhs)
{
    return gameanimcmphack[*lhs] < gameanimcmphack[*rhs] ? -1 : 1;
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_cGameAnimMap);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }
extern void* cGameAnimMap_TheGameAnimMap;

/**
 * Constructs the game-animation map and makes it the global cGameAnimMap_TheGameAnimMap; returns @p self.
 *
 * The 518-entry member array has an empty constructor, so nothing else is written (cGameAnimMap_initMap fills it).
 * main/ssxapp allocates the 6216-byte object (tag "GameAnimMap").
 *
 * @return @p self
 */
extern "C" void* cGameAnimMap_cGameAnimMap(cGameAnimMap* self)
{
    new (((void*)self)) tAnimID[518];
    cGameAnimMap_TheGameAnimMap = ((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap__cGameAnimMap);
#ifdef SKIP_ASM
// cGameAnimMap_TheGameAnimMap is declared void* earlier in the unit.
void operator_delete(int* ptr);

/**
 * Destroys the game-animation map: clears cGameAnimMap_TheGameAnimMap and frees @p self if bit 0 of @p flags is set.
 *
 * @param flags  bit 0: delete the object itself
 */
extern "C" void cGameAnimMap__cGameAnimMap(int* self, int flags)
{
    cGameAnimMap_TheGameAnimMap = 0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_initMap);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char* cGameAnimMap_GameAnimNames[];
extern unsigned int* gameanimcmphack;
extern "C" int gameanimcmp(int* a, int* b);
extern "C" void qsort(void* base, int n, int size, int (*cmp)(int*, int*));
extern "C" void* func_00416210(void* dst, int c, int n);

struct sMdfHashTable_00314DD8
{
    unsigned int hash[0x206];
    int order[0x206];
    char map[0x818];
};

/**
 * Builds the game-animation hash index and marks every game animation as unmapped.
 *
 * Hashes the 518 names of cGameAnimMap_GameAnimNames ("G_GATEROCK", "G_SIT_CYC", ...) with GetHashValue32() into hash[],
 * sorts order[] by hash with qsort (qsort; the comparator gameanimcmp reads the hashes through
 * gameanimcmphack, set only during the sort) and fills the 518-entry map with -1 (func_00416210). cGameAnimMap_lookupGAD
 * (On Tour lookupGAD) binary-searches this index.
 */
extern "C" void cGameAnimMap_initMap(sMdfHashTable_00314DD8* self)
{
    for (int i = 0; i < 518; i++)
    {
        int hashValue = GetHashValue32(cGameAnimMap_GameAnimNames[i]);
        self->order[i] = i;
        self->hash[i] = hashValue;
    }
    gameanimcmphack = self->hash;
    qsort(self->order, 518, 4, gameanimcmp);
    gameanimcmphack = 0;
    func_00416210(self->map, -1, 2072);
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_lookupGAD);
#ifdef SKIP_ASM
struct sMdfIndex_00314E88
{
    unsigned int hashes[0x206]; // 0x000: sorted by order[]
    int order[0x206];           // 0x818
};

/**
 * Returns the game-animation index whose name hash is @p hash, or -1 if none.
 *
 * Binary search over the 518 hashes (+0x0) in the order (+0x818) that cGameAnimMap_initMap sorted them into.
 *
 * @param hash  the name hash (GetHashValue32 of a cGameAnimMap_GameAnimNames name such as "G_GATEROCK")
 * @return the game-animation index 0-517, or -1
 */
// Binary search for `hash`; returns its entry index, or -1.
extern "C" int cGameAnimMap_lookupGAD(sMdfIndex_00314E88* self, unsigned int hash)
{
    int lo = 0;
    int hi = 518;
    while (lo < hi)
    {
        int mid = (lo + hi) / 2;
        int idx = self->order[mid];
        unsigned int entryHash = self->hashes[idx];
        if (hash == entryHash)
        {
            return idx;
        }
        if (hash < entryHash)
        {
            hi = mid;
        }
        else
        {
            lo = mid + 1;
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_setMap);
#ifdef SKIP_ASM
/**
 * Maps the game animation with name hash @p hash to @p value, if such a game animation exists.
 *
 * Looks the hash up with cGameAnimMap_lookupGAD and stores @p value in the 518-entry map at +0x1030. Unknown hashes are ignored.
 *
 * @param hash  the animation's name hash
 * @param value  the animation id to map it to (bank in the low 8 bits, index in the upper 24), or kAnimIDInvalid
 */
extern "C" void cGameAnimMap_setMap(cGameAnimMap* self, unsigned int hash, void* value)
{
    int i = cGameAnimMap_lookupGAD(((sMdfIndex_00314E88*)self), hash);
    if (i >= 0)
    {
        ((void**)(&self->GameAnimMap[0]))[i] = value;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_addSource);
#ifdef SKIP_ASM
struct sMdfHandle_00314F30
{
    unsigned int kind : 8;
    unsigned int index : 24;
};

static inline void SetKind_00314F30(sMdfHandle_00314F30* h, int v) { h->kind = v; }
static inline void SetIndex_00314F30(sMdfHandle_00314F30* h, int v) { h->index = v; }

// PORT: cGameAnimMap_setMap's value slot receives the handle struct by value here.
void cGameAnimMap_setMap_handle(sMdfIndex_00314E88* self, unsigned int hash, sMdfHandle_00314F30 value) __asm__("cGameAnimMap_setMap");

/**
 * Maps every animation of an animation bank into the game-animation map.
 *
 * For each animation i of @p list (count: short at +0x2 of its header at +0x4; 0x14-byte entries at +0x8 with the
 * name hash first) calls cGameAnimMap_setMap with the hash and the id (@p kind in the low 8 bits, i above them).
 *
 * @param kind  the bank number stored in each id
 * @param list  the animation data source (cAdfDataSource) of the bank
 */
extern "C" void cGameAnimMap_addSource(cGameAnimMap* self, int kind, cAdfDataSource* list)
{
    sMdfHandle_00314F30 handle;
    for (int i = 0; i < *(short*)((char*)list->Archive + 2); i++)
    {
        SetKind_00314F30(&handle, kind);
        SetIndex_00314F30(&handle, i);
        cGameAnimMap_setMap_handle(((sMdfIndex_00314E88*)self), *(unsigned int*)(i * 0x14 + (int)list->Anims), handle);
    }
}
#endif

//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_remSource);
#ifdef SKIP_ASM
extern void* kAnimIDInvalid;

/**
 * Unmaps every animation of an animation bank: sets each of its name hashes to kAnimIDInvalid (cGameAnimMap_setMap).
 *
 * Same walk as cGameAnimMap_addSource over @p list's animation table; called before the bank is unloaded.
 *
 * @param list  the animation data source (cAdfDataSource) of the bank
 */
extern "C" void cGameAnimMap_remSource(cGameAnimMap* self, cAdfDataSource* list)
{
    for (int i = 0; i < *(short*)((char*)list->Archive + 2); i++)
    {
        cGameAnimMap_setMap(self, *(unsigned int*)(i * 0x14 + (int)list->Anims), kAnimIDInvalid);
    }
}
#endif


//100%
INCLUDE_ASM("animation/mdfarchive", cGameAnimMap_getAnimName);
#ifdef SKIP_ASM
extern char str_error[];

/**
 * Returns the name of the first game animation mapped to @p value, or str_error when none is.
 *
 * Linear search of the 518-entry map at +0x1030; the name comes from cGameAnimMap_GameAnimNames (e.g. "G_GATEROCK").
 *
 * @param value  the animation id to look for
 * @return the game animation's name, or str_error
 */
extern "C" char* cGameAnimMap_getAnimName(cGameAnimMap* self, void* value)
{
    for (int i = 0; i < 518; i++)
    {
        if (value == ((void**)(&self->GameAnimMap[0]))[i])
        {
            return cGameAnimMap_GameAnimNames[i];
        }
    }
    return str_error;
}
#endif
