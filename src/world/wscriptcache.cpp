#include "common.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCache_init);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void* cWScriptCacheEntry_cWScriptCacheEntry(void* self);
extern "C" void cWScriptCacheEntry__cWScriptCacheEntry(int* self, int flags);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_tWScriptCache[];

/**
 * Sizes the world-script cache for a number of sections by replacing its section array with empty entries.
 *
 * Destroys any old array at +0x4 (cWScriptCacheEntry__cWScriptCacheEntry on each entry, last to first) and frees it, stores @p count
 * at +0x8, allocates @p count 12-byte "tWScriptCache" entries behind an array cookie, constructs each
 * (cWScriptCacheEntry_cWScriptCacheEntry: state 0, +0x4 = -1, no data) and clears +0xC.
 *
 * @param count  number of world sections (On Tour numSections)
 */
// PORT: array cookie arithmetic through int (pointer in int).
extern "C" void cWScriptCache_init(void* self, int count)
{
    char* old = *(char**)((char*)self + 0x4);
    if (old != 0) {
        char* oldEntry = (char*)(*(int*)(old - 0x10) * 0xC + (int)old);
        while (*(char**)((char*)self + 0x4) != oldEntry) {
            oldEntry -= 0xC;
            cWScriptCacheEntry__cWScriptCacheEntry((int*)oldEntry, 0);
        }
        cMemMan_free(*(char**)((char*)self + 0x4) - 0x10);
    }
    *(int*)((char*)self + 0x8) = count;
    char** slot = (char**)((char*)self + 0x4);
    char* mem = (char*)operator_new_tag(count * 0xC + 0x10, str_tWScriptCache, 0x20000000, 0);
    char* arr = mem + 0x10;
    *(int*)mem = count;
    char* newEntry = arr;
    int i;
    for (i = count - 1; i != -1; i--, newEntry += 0xC) {
        cWScriptCacheEntry_cWScriptCacheEntry(newEntry);
    }
    *slot = arr;
    *(int*)((char*)self + 0xC) = 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCache__cWScriptCache);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int*);
extern "C" void cWScriptCacheEntry__cWScriptCacheEntry(int* self, int flags);
extern int cWScriptCache_mpWScriptCache;

/**
 * Destroys the world script cache and its per-section entries.
 *
 * If the entry array at +0x4 exists, runs cWScriptCacheEntry__cWScriptCacheEntry (flag 0) on each 0xC-byte entry from the last to the first (the
 * count is the cookie at array - 0x10) and frees the block with cMemMan_free. Then clears cWScriptCache_mpWScriptCache
 * and calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
// PORT: array cookie arithmetic through int (pointer in int).
extern "C" void cWScriptCache__cWScriptCache(int* self, int flags)
{
    char* old = *(char**)((char*)self + 0x4);
    if (old != 0) {
        char* p = (char*)(*(int*)(old - 0x10) * 0xC + (int)old);
        while (*(char**)((char*)self + 0x4) != p) {
            p -= 0xC;
            cWScriptCacheEntry__cWScriptCacheEntry((int*)p, 0);
        }
        cMemMan_free(*(char**)((char*)self + 0x4) - 0x10);
    }
    cWScriptCache_mpWScriptCache = 0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCache_update);
#ifdef SKIP_ASM
extern "C" void cWScriptCacheEntry_update(int);

/**
 * Updates every section's script cache entry: cWScriptCacheEntry_update on each of the mNumSections entries (0xC bytes each).
 *
 * No C caller in the source.
 */
extern "C" void cWScriptCache_update(void *self) {
    unsigned int i;

    for (i = 0; i < *(unsigned int *)((char *)self + 8); i++) {
        cWScriptCacheEntry_update(*(int *)((char *)self + 4) + i * 0xC);
    }
}
#endif
//100%
INCLUDE_ASM("world/wscriptcache", func_003AC9E0);
#ifdef SKIP_ASM
extern "C" void func_003ACBB8(int, int);

/**
 * Starts loading section @p index's script file by name: func_003ACBB8 on that cache entry.
 *
 * No C caller in the source.
 *
 * @param cache  the cWScriptCache
 * @param index  the section id
 * @param fileNum  the file name passed on to func_003ACBB8
 */
extern "C" void func_003AC9E0(void *cache, int index, int fileNum) {
    func_003ACBB8((*(int *)((char*)(cache) + (4))) + (index * 0xC), fileNum);
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACA10);
#ifdef SKIP_ASM
struct sWScriptCacheItem {
    char pad_0x00[0xC];
};

extern "C" void func_003ACC50(sWScriptCacheItem* e, int index, int value);

/**
 * Hands section @p index's script data to its cache entry (func_003ACC50), which resolves it.
 *
 * Called by cPS2ChunkResolve_resolve when a section's script chunk arrives.
 *
 * @param index  the section id
 * @param value  the script file header
 */
extern "C" void func_003ACA10(void* self, int index, int value)
{
    sWScriptCacheItem* items = *(sWScriptCacheItem**)((char*)self + 0x4);
    func_003ACC50(&items[index], index, value);
}
#endif

// 0xc-byte elements reached through a pointer at self+0x4
struct sWScriptCacheEntry {
    char pad_0x00[0x8];
    int field_0x8;
};

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCache_getScriptBase);
#ifdef SKIP_ASM
/**
 * Returns section @p a1's script data (its cache entry's mpScriptHeader).
 *
 * The wscript code uses it as the section's script base (func_0030BB10, cWScriptMan_readFromReplayFrame).
 *
 * @param a1  the section id
 * @return the script file header
 */
extern "C" int cWScriptCache_getScriptBase(void* self, int a1)
{
    sWScriptCacheEntry* p = *(sWScriptCacheEntry**)((char*)self + 0x4);
    return p[a1].field_0x8;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_cWScriptCacheEntry__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a script cache entry: state 0, mStreamed 0, async handle -1, no data.
 *
 * @return @p self
 */
void* cWScriptCacheEntry_cWScriptCacheEntry(void* self)
{
    int zero = 0;
    *(short*)self = (short)zero;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = zero;
    *(short*)((char*)self + 0x2) = (short)zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry__cWScriptCacheEntry);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int*);

/**
 * Destroys a script cache entry: frees its data unless it is streamed (mStreamed), and deletes the entry when bit 0 of
 * @p flags is set.
 *
 * @param flags  bit 0 set to delete the object
 */
extern "C" void cWScriptCacheEntry__cWScriptCacheEntry(int* self, int flags)
{
    if (*(void**)((char*)self + 0x8) != 0 && *(short*)((char*)self + 0x2) == 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void ASYNCFILE_release_CAD0(int, int *, int *) __asm__("ASYNCFILE_release");
extern "C" void MEM_free_CAD0(int) __asm__("MEM_free");
extern "C" void unpack_CAD0(int, int) __asm__("unpack");
extern "C" int unpacksizez_CAD0(int) __asm__("unpacksizez");
extern "C" int ASYNCFILE_getstatus_CAD0(int) __asm__("ASYNCFILE_getstatus");
extern "C" void MEM_copy_CAD0(int, int, int) __asm__("MEM_copy");
int operator_new_CAD0(unsigned int, void*, int, int) __asm__("operator_new__FUi");
extern char str_WScriptFile[];
extern "C" void cWScriptCacheEntry_update_impl(void *self_) __asm__("cWScriptCacheEntry_update");
/**
 * Finishes a script file load: once the entry is loading (state 1) and its async file reports status 1, takes the data,
 * unpacks or copies it into a new "WScriptFile" buffer (mpScriptHeader) and sets state 2.
 *
 * ASYNCFILE_release gives the data and size; with unpacksizez above 0 the data is unpacked, otherwise copied as is. The
 * async buffer is freed with MEM_free. State 2 is set even when no data came back.
 *
 * @param self_  the cWScriptCacheEntry
 */
extern "C" void cWScriptCacheEntry_update_impl(void *self_) {
    char *self = (char*)self_;
    short st = *(short*)self;
    if (st == 1 && ASYNCFILE_getstatus_CAD0(*(int*)(self + 4)) == st) {
        int data = 0;
        int size = 0;
        ASYNCFILE_release_CAD0(*(int*)(self + 4), &data, &size);
        if (data != 0) {
            if (size > 0) {
                int unpackedSize = unpacksizez_CAD0(data);
                if (unpackedSize > 0) {
                    int buffer = operator_new_CAD0(unpackedSize, str_WScriptFile, 0x20000000, 0);
                    *(int*)(self + 8) = buffer;
                    unpack_CAD0(data, buffer);
                } else {
                    int buffer = operator_new_CAD0(size, str_WScriptFile, 0x20000000, 0);
                    *(int*)(self + 8) = buffer;
                    MEM_copy_CAD0(buffer, data, size);
                }
                MEM_free_CAD0(data);
            }
        }
        *(short*)self = 2;
    }
}
#endif
//100%
INCLUDE_ASM("world/wscriptcache", func_003ACBB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBXStr_CBB8 { char *p; int pad[3]; sBXStr_CBB8() {} };
extern "C" void func_003186D0_CBB8(sBXStr_CBB8*, const char*, int) __asm__("func_003186D0");
extern "C" void func_00318630_CBB8(sBXStr_CBB8*, sBXStr_CBB8*, const char*) __asm__("func_00318630");
extern "C" int ASYNCFILE_load_CBB8(const char*, int) __asm__("ASYNCFILE_load");
extern "C" void cBXString_dt_CBB8(sBXStr_CBB8*, int) __asm__("cBXString__cBXString");
extern char D_004A47C0[];
extern char str_ssf[];
extern "C" void func_003ACBB8_impl(void *self, int arg) __asm__("func_003ACBB8");
/**
 * Starts loading an entry's script file asynchronously from D_004A47C0 + @p arg + str_ssf.
 *
 * Clears mStreamed and stores the ASYNCFILE_load handle (flags 0x100); the state becomes 1 (loading), or 2 when the
 * handle is 0 or negative.
 *
 * @param arg  the file name (a cBXString)
 */
extern "C" void func_003ACBB8_impl(void *self, int arg) {
    sBXStr_CBB8 a;
    sBXStr_CBB8 b;
    *(short*)((char*)self + 2) = 0;
    func_003186D0_CBB8(&b, D_004A47C0, arg);
    func_00318630_CBB8(&a, &b, str_ssf);
    *(int*)((char*)self + 4) = ASYNCFILE_load_CBB8(a.p, 0x100);
    cBXString_dt_CBB8(&a, 2);
    cBXString_dt_CBB8(&b, 2);
    int n = *(int*)((char*)self + 4);
    *(short*)((char*)self + 0) = n <= 0 ? 2 : 1;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ACC50);
#ifdef SKIP_ASM
struct sWSCItemFields {
    short type;   // 0x0
    short owned;  // 0x2
    int pad4;
    int value;    // 0x8
};

extern "C" void cWScriptCacheEntry_resolve(sWScriptCacheItem* e, void* world, int index);

/**
 * Gives an entry the script data that came with the stream: stores @p value, marks it streamed (mStreamed 1, not freed
 * by the entry), sets state 2 and resolves it (cWScriptCacheEntry_resolve with cWorld_m_pTheWorld and @p index).
 *
 * @param item  the cWScriptCacheEntry
 * @param index  the section id
 * @param value  the script file header
 */
extern "C" void func_003ACC50(sWScriptCacheItem* item, int index, int value)
{
    sWSCItemFields* fields = (sWSCItemFields*)item;
    fields->value = value;
    fields->owned = 1;
    fields->type = 2;
    cWScriptCacheEntry_resolve(item, cWorld_m_pTheWorld, index);
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_unload);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Unloads an entry's script data: frees it unless streamed, clears mpScriptHeader and sets state 0.
 */
extern "C" void cWScriptCacheEntry_unload(void* self)
{
    void* buf = *(void**)((char*)self + 0x8);
    if (buf != 0) {
        if (*(short*)((char*)self + 0x2) == 0) {
            cMemMan_free(buf);
        }
        *(void**)((char*)self + 0x8) = 0;
    }
    *(short*)self = 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_resolve);
#ifdef SKIP_ASM
struct sWScriptCacheItem;
void cMemMan_free(void* p);

struct sRef_CCD8 {
    unsigned int idx : 8;
    unsigned int slot : 24;
};

struct sPair_CCD8 {
    short a;
    unsigned short b;
};

struct sRec40_CCD8 {
    int f0;
    int f4;
    int f8;
    char pad[0x34];
};

struct sRec54_CCD8 {
    int f0;
    int f4;
    int f8;
    char pad[0x48];
};

#define WSD_CCD8 (*(char**)((char*)e + 0x8))

static inline void* toPtr_CCD8(unsigned int v)
{
    return (void*)(v << 2);
}

static inline void* lookup_CCD8(char* obj, int tblOff, unsigned int slot)
{
    unsigned int v = (*(unsigned int**)(obj + tblOff))[slot] >> 8;
    if (v == 0) {
        return 0;
    }
    return toPtr_CCD8(v);
}

static inline void* resolve_CCD8(char* w, sRef_CCD8 ref, int tblOff)
{
    char* obj = (*(char***)(w + 0x8))[ref.idx];
    if (obj == 0) {
        return 0;
    }
    return lookup_CCD8(obj, tblOff, ref.slot);
}

/**
 * Resolves a section's script data: checks it against the world, turns its offsets into pointers and links the
 * section's instances and other resources to it. Always ends in state 3.
 *
 * Without data it only sets state 3. Data whose first halfword is not 0x1000 or whose +0xC word differs from the
 * world's .sdb checksum (mp_DataHeader->m_uiCheckSum) is dropped (freed unless streamed). Otherwise the ten offsets at
 * +0x14 .. +0x58 and the tables at +0x3C (count +0x38) and +0x50 (count +0x4C, relative to +0x40) become pointers. Each
 * instance of the section (count +0x44, kBxStreamData_Instance) gets its script record at +0x88 (from +0x48) and a
 * rebuilt flag word at +0x8; script records of type 1 or 3 get their collide model (kBxStreamData_CollideModel) at
 * +0xC, others 0; each spline (count +0x54, kBxStreamData_Spline) gets two values from the +0x58 pairs at +0x28 and
 * +0x1C; and the records of the tables at +0x2C (0x40 bytes) and +0x24 (0x54 bytes) get their +0x8 offsets fixed. There
 * are no null checks on the looked-up resources.
 *
 * @param ep  the cWScriptCacheEntry
 * @param world  the cWorld
 * @param index  the section id
 */
extern "C" void cWScriptCacheEntry_resolve(sWScriptCacheItem* ep, void* world, int index)
{
    char* e = (char*)ep;
    char** wp = (char**)world;
    char* d = WSD_CCD8;
    if (d == 0) {
        *(short*)e = 3;
        return;
    }
    {
        if (!(*(unsigned short*)d == 0x1000 && *(int*)(d + 0xC) == *(int*)(*(char**)*wp + 0x4))) {
            if (*(short*)(e + 0x2) == 0) {
                cMemMan_free(d);
            }
            WSD_CCD8 = 0;
        }
        if (WSD_CCD8 != 0) {
            *(int*)(WSD_CCD8 + 0x14) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x14);
            *(int*)(WSD_CCD8 + 0x1C) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x1C);
            *(int*)(WSD_CCD8 + 0x24) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x24);
            *(int*)(WSD_CCD8 + 0x2C) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x2C);
            *(int*)(WSD_CCD8 + 0x34) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x34);
            *(int*)(WSD_CCD8 + 0x3C) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x3C);
            *(int*)(WSD_CCD8 + 0x40) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x40);
            *(int*)(WSD_CCD8 + 0x48) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x48);
            *(int*)(WSD_CCD8 + 0x50) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x50);
            *(int*)(WSD_CCD8 + 0x58) = (int)WSD_CCD8 + *(int*)(WSD_CCD8 + 0x58);
            int* t3c = *(int**)(WSD_CCD8 + 0x3C);
            for (int i = 0; i < *(int*)(WSD_CCD8 + 0x38); i++) {
                t3c[i] = (int)WSD_CCD8 + t3c[i];
            }
            for (int i = 0; i < *(int*)(WSD_CCD8 + 0x4C); i++) {
                (*(int**)(WSD_CCD8 + 0x50))[i] = *(int*)(WSD_CCD8 + 0x40) + (*(int**)(WSD_CCD8 + 0x50))[i];
            }
            sRef_CCD8 ref;
            *(unsigned int*)&ref = 0xFFFFFFFF;
            ref.idx = index;
            for (int i = 0; i < *(int*)(WSD_CCD8 + 0x44); i++) {
                ref.slot = i;
                char* o = (char*)resolve_CCD8(*wp, ref, 0x1C);
                char* x = (char*)(*(int**)(WSD_CCD8 + 0x50))[*(unsigned short*)(*(char**)(WSD_CCD8 + 0x48) + (i << 1))];
                *(char**)(o + 0x88) = x;
                int v = *(int*)(x + 0x4) & 0xFFFF0000;
                *(int*)(o + 0x8) = v | (v >> 16) | 2;
            }
            for (int i = 0; i < *(int*)(WSD_CCD8 + 0x4C); i++) {
                char* r = (*(char***)(WSD_CCD8 + 0x50))[i];
                int type = *(int*)r;
                if (type == 1 || type == 3) {
                    *(void**)(r + 0xC) = resolve_CCD8(*wp, *(sRef_CCD8*)(r + 0xC), 100);
                } else {
                    *(void**)(r + 0xC) = 0;
                }
            }
            int n54 = *(int*)(WSD_CCD8 + 0x54);
            for (int i = 0; i < n54; i++) {
                char* w = *wp;
                ref.slot = i;
                char* o = (char*)resolve_CCD8(w, ref, 68);
                sPair_CCD8 p = (*(sPair_CCD8**)(WSD_CCD8 + 0x58))[i];
                *(int*)(o + 0x28) = (short)p.b;
                unsigned int v = p.a << 16;
                *(unsigned int*)(o + 0x1C) = (v & 0xFFFF0000) | (v >> 16);
            }
            {
                sRec40_CCD8* r = *(sRec40_CCD8**)(WSD_CCD8 + 0x2C);
                int n = *(int*)(WSD_CCD8 + 0x28);
                for (int i = 0; i < n; i++, r++) {
                    r->f8 = (int)WSD_CCD8 + r->f8;
                }
            }
            {
                sRec54_CCD8* r = *(sRec54_CCD8**)(WSD_CCD8 + 0x24);
                int n = *(int*)(WSD_CCD8 + 0x20);
                for (int i = 0; i < n; i++, r++) {
                    r->f8 = (int)WSD_CCD8 + r->f8;
                }
            }
        }
    }
    *(short*)e = 3;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_getWScriptHeaderId);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sRef_D120 {
    unsigned int idx : 8;
    unsigned int slot : 24;
};

#define WSD_D120 (*(char**)((char*)cache + 0x8))

static inline int valid_D120(sRef_D120 r)
{
    return *(unsigned int*)&r != 0xFFFFFFFF;
}

/**
 * Returns the script header id of an instance's @p entry: word @p entry of the 0x18-byte row, picked by the instance's
 * script reference, of the table at the script data's +0x1C.
 *
 * Returns -1 when the instance has no script record (+0x88), the entry has no data or the record's id (+0x8) is -1.
 *
 * @param cache  the cWScriptCacheEntry of the instance's section
 * @param inst  the tInstanceRecord
 * @param entry  the script slot
 * @return the tResourceID, or -1
 */
extern "C" unsigned int cWScriptCacheEntry_getWScriptHeaderId(void* cache, void* inst, int entry)
{
    char* data = *(char**)((char*)inst + 0x88);
    if (data == 0 || WSD_D120 == 0) {
        return 0xFFFFFFFF;
    }
    sRef_D120 id = *(sRef_D120*)(data + 0x8);
    if (!valid_D120(id)) {
        return 0xFFFFFFFF;
    }
    return *(unsigned int*)(*(char**)(WSD_D120 + 0x1C) + id.slot * 0x18 + entry * 4);
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD188);
#ifdef SKIP_ASM

/**
 * Returns SSXApp->Game.
 *
 * cPS2ChunkResolve_resolve passes it to func_00230430 with the section index.
 *
 * @return the cGame
 */
extern "C" void* func_003AD188(void)
{
    return SSXApp->Game;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD198);
#ifdef SKIP_ASM

/**
 * Returns SSXApp->Game->OcclusionMan.
 *
 * cPS2ChunkResolve_resolve and cPS2ChunkResolve_unresolve register and remove occluders through it.
 *
 * @return the occlusion manager
 */
extern "C" void* func_003AD198(void)
{
    return SSXApp->Game->OcclusionMan;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD1A8);
#ifdef SKIP_ASM
extern "C" void func_002F5998(void* lights);
extern "C" void cRider_flushActiveLights(void* self);

/**
 * Drops all active lights: func_002F5998 on the game's WorldLightMan, then cRider_flushActiveLights on each rider of
 * Game->AI (count +0x78, riders from +0x28).
 *
 * cWorldView_update calls it after freeing an unloaded section's memory.
 */
extern "C" void func_003AD1A8(void)
{
    func_002F5998(SSXApp->Game->WorldLightMan);
    char* riders = (char*)SSXApp->Game->AI;
    if (riders != 0) {
        for (int i = 0; i < *(int*)(riders + 0x78); i++) {
            cRider_flushActiveLights(*(void**)(riders + 0x28 + i * 4));
        }
    }
}
#endif

extern "C" void* func_002E27E8(void* self);

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD230__FPv);
#ifdef SKIP_ASM
/**
 * Forwards to func_002E27E8 and returns its result.
 *
 * cChunkResolve_resolveHalo uses it to get the radius for a halo type.
 *
 * @return func_002E27E8's result
 */
void* func_003AD230(void* self)
{
    return func_002E27E8(self);
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD250);
#ifdef SKIP_ASM
/**
 * Returns its first argument; the other arguments are ignored. No C caller in the source.
 *
 * @return its first argument
 */
extern "C" void* func_003AD250(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003AD290);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern float D_00509080[4];
extern float D_00509090[4];
extern float D_005090A0[4];
extern float D_005090B0[4];
extern float D_005090C0[4];
extern float D_005090D0[4];
extern float D_005090E0[4];
extern float D_005090F0[4];
extern float D_00509100[4];
extern float D_00509110[4];
extern float D_00509120[4];
extern float D_00509130[4];
extern float D_00509140[4];
extern float D_00509150[4];
extern float D_00509160[4];
extern float D_00509170[4];
extern float D_00509180[4];
extern float D_00509190[4];
extern float D_005091A0[4];
extern float D_005091B0[4];
extern float D_005091C0[4];
extern float D_005091D0[4];
extern float D_005091E0[4];
extern float D_005091F0[4];
extern float D_00509200[4];
extern float D_00509210[4];
extern float D_00509220[4];
extern float D_00509230[4];
extern float D_00509240[4];
extern float D_00509250[4];
extern float D_00509260[4];
extern float D_00509270[4];
extern float D_00509280[4];
extern float D_00509290[4];
extern float D_005092A0[4];
extern float D_005092B0[4];
extern float D_005092C0[4];
extern float D_005092D0[4];
extern float D_005092E0[4];
extern float D_005092F0[4];
extern float D_00509300[4];
extern float D_00509310[4];
extern float D_00509320[4];
extern float D_00509330[4];
extern float D_00509340[4];
extern float D_00509350[4];
extern float D_00509360[4];
extern float D_00509370[4];
extern float D_00509380[4];
extern float D_00509390[4];
extern float D_005093A0[4];
extern float D_005093B0[4];
extern float D_005093C0[4];
extern float D_005093D0[4];
extern float D_005093E0[4];
extern float D_005093F0[4];
extern int D_004A5A70;
extern int D_004A5A74;
extern int D_004A5A78[2];
extern int D_004A5A80[2];
extern int D_004A5A88[2];
extern int D_004A5A90[2];
extern int D_004A5A98[2];
extern int D_004A5AA0[2];
extern int D_004A5AA8[2];
extern int D_004A5AB0[2];
extern int D_004A5AB8[2];
extern int D_004A5AC0[2];
extern int D_004A5AC8[2];
extern int D_004A5AD0[2];
extern int D_004A5AD8[2];
extern int D_004A5AE0[2];
extern long D_00509400[2];
extern long D_00509410[2];
extern long D_00509420[2];
extern char D_2400[];
extern char D_2270[];
extern char D_22C8[];
static inline void initE(int* p, int n)
{
    *(char*)p = n;
    *p = (*p & 0xFF) | (n << 8);
}
static inline void initD(long* p, char* sz)
{
    p[0] = 0;
    p[1] = (long)(((unsigned)sz >> 3) | 0x14000000) << 32;
}
void sti_003AD290(int init, int prio) __asm__("func_003AD290");
/**
 * Static initialiser of the unit's render data, run with (1, 0xFFFF).
 *
 * Fills 56 four-float constants (D_00509080 .. D_005093F0), zeroes D_004A5A70 and D_004A5A74, gives the 14 two-word
 * records D_004A5A78 .. D_004A5AE0 their index 0-13 (in the low byte and in the bits above it) and builds the three
 * shared DMA packets used by func_003AC048 and func_003AC508: D_00509400, D_00509410 and D_00509420 hold a zero
 * doubleword and a VIF MSCAL of the VU1 microprograms D_2400, D_2270 and D_22C8.
 *
 * @param init  1 to construct
 * @param prio  the priority; only 0xFFFF does anything
 */
void sti_003AD290(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        D_00509080[0] = 1.0f;
        D_00509080[1] = 1.0f;
        D_00509080[2] = 1.0f;
        D_00509080[3] = 0.5f;
        D_00509090[0] = 0.4998680055141449f;
        D_00509090[1] = 1.0f;
        D_00509090[2] = 1.0f;
        D_00509090[3] = 0.0f;
        D_005090A0[0] = 0.12951700389385223f;
        D_005090A0[1] = 1.0f;
        D_005090A0[2] = 1.0f;
        D_005090A0[3] = 0.0f;
        D_005090B0[0] = 0.22426900267601013f;
        D_005090B0[1] = 0.9402310252189636f;
        D_005090B0[2] = 0.5425059795379639f;
        D_005090B0[3] = 0.0004030000127386302f;
        D_005090C0[0] = 0.4652079939842224f;
        D_005090C0[1] = 0.32170501351356506f;
        D_005090C0[2] = 0.9943439960479736f;
        D_005090C0[3] = 0.0f;
        D_005090D0[0] = 0.17792600393295288f;
        D_005090D0[1] = 1.0f;
        D_005090D0[2] = 1.0f;
        D_005090D0[3] = 0.0f;
        D_005090E0[0] = 0.4411740005016327f;
        D_005090E0[1] = 0.41388601064682007f;
        D_005090E0[2] = 0.3211260139942169f;
        D_005090E0[3] = 0.4398239850997925f;
        D_005090F0[0] = 0.500997006893158f;
        D_005090F0[1] = 0.24849599599838257f;
        D_005090F0[2] = 1.0f;
        D_005090F0[3] = 1.0f;
        D_00509100[0] = 0.27687299251556396f;
        D_00509100[1] = 0.6395620107650757f;
        D_00509100[2] = 0.13423900306224823f;
        D_00509100[3] = 0.0f;
        D_00509110[0] = 0.8936439752578735f;
        D_00509110[1] = 0.5263469815254211f;
        D_00509110[2] = 0.5051450133323669f;
        D_00509110[3] = 0.8687300086021423f;
        D_00509120[0] = 1.0f;
        D_00509120[1] = 1.0f;
        D_00509120[2] = 1.0f;
        D_00509120[3] = 1.0f;
        D_00509130[0] = 1.0f;
        D_00509130[1] = 0.0f;
        D_00509130[2] = 0.0f;
        D_00509130[3] = 0.0f;
        D_00509140[0] = 1.0f;
        D_00509140[1] = 0.550000011920929f;
        D_00509140[2] = 0.5350000262260437f;
        D_00509140[3] = 0.550000011920929f;
        D_00509150[0] = 1.0f;
        D_00509150[1] = 1.0f;
        D_00509150[2] = 1.0f;
        D_00509150[3] = 1.0f;
        D_00509160[0] = 1.0f;
        D_00509160[1] = 1.0f;
        D_00509160[2] = 1.0f;
        D_00509160[3] = 1.0f;
        D_00509170[0] = 1.0f;
        D_00509170[1] = 0.0f;
        D_00509170[2] = 0.0f;
        D_00509170[3] = 0.0f;
        D_00509180[0] = 1.0f;
        D_00509180[1] = 0.0f;
        D_00509180[2] = 0.0f;
        D_00509180[3] = 0.0f;
        D_00509190[0] = 1.0f;
        D_00509190[1] = 1.0f;
        D_00509190[2] = 1.0f;
        D_00509190[3] = 1.0f;
        D_005091A0[0] = 1.0f;
        D_005091A0[1] = 0.5f;
        D_005091A0[2] = 1.0f;
        D_005091A0[3] = 0.5f;
        D_005091B0[0] = 1.0f;
        D_005091B0[1] = 0.5f;
        D_005091B0[2] = 1.0f;
        D_005091B0[3] = 0.5f;
        D_005091C0[0] = 1.0f;
        D_005091C0[1] = 0.5f;
        D_005091C0[2] = 1.0f;
        D_005091C0[3] = 0.5f;
        D_005091D0[0] = 1.0f;
        D_005091D0[1] = 0.7828119993209839f;
        D_005091D0[2] = 0.06171900033950806f;
        D_005091D0[3] = 0.03984399884939194f;
        D_005091E0[0] = 1.0f;
        D_005091E0[1] = 1.0f;
        D_005091E0[2] = 0.7988259792327881f;
        D_005091E0[3] = 0.0f;
        D_005091F0[0] = 0.9886710047721863f;
        D_005091F0[1] = 0.9230499863624573f;
        D_005091F0[2] = 0.34311699867248535f;
        D_005091F0[3] = 0.0f;
        D_00509200[0] = 1.0f;
        D_00509200[1] = 1.0f;
        D_00509200[2] = 1.0f;
        D_00509200[3] = 1.0f;
        D_00509210[0] = 1.0f;
        D_00509210[1] = 1.0f;
        D_00509210[2] = 0.7976430058479309f;
        D_00509210[3] = 0.0f;
        D_00509220[0] = 1.0f;
        D_00509220[1] = 0.9222670197486877f;
        D_00509220[2] = 0.34179699420928955f;
        D_00509220[3] = 0.0f;
        D_00509230[0] = 1.0f;
        D_00509230[1] = 0.7847639918327332f;
        D_00509230[2] = 0.06054700165987015f;
        D_00509230[3] = 0.04062499850988388f;
        D_00509240[0] = 1.0f;
        D_00509240[1] = 0.800000011920929f;
        D_00509240[2] = 0.44999998807907104f;
        D_00509240[3] = 1.0f;
        D_00509250[0] = 1.0f;
        D_00509250[1] = 0.3281260132789612f;
        D_00509250[2] = 1.0f;
        D_00509250[3] = 0.3539060056209564f;
        D_00509260[0] = 1.0f;
        D_00509260[1] = 1.0f;
        D_00509260[2] = 1.0f;
        D_00509260[3] = 1.0f;
        D_00509270[0] = 1.0f;
        D_00509270[1] = 1.0f;
        D_00509270[2] = 1.0f;
        D_00509270[3] = 1.0f;
        D_00509280[0] = 1.0f;
        D_00509280[1] = 0.8613290190696716f;
        D_00509280[2] = 0.38124799728393555f;
        D_00509280[3] = 0.0f;
        D_00509290[0] = 1.0f;
        D_00509290[1] = 1.0f;
        D_00509290[2] = 1.0f;
        D_00509290[3] = 1.0f;
        D_005092A0[0] = 1.0f;
        D_005092A0[1] = 0.3244380056858063f;
        D_005092A0[2] = 1.0f;
        D_005092A0[3] = 0.0f;
        D_005092B0[0] = 1.0f;
        D_005092B0[1] = 1.0f;
        D_005092B0[2] = 1.0f;
        D_005092B0[3] = 1.0f;
        D_005092C0[0] = 1.0f;
        D_005092C0[1] = 1.0f;
        D_005092C0[2] = 0.0f;
        D_005092C0[3] = 0.0f;
        D_005092D0[0] = 0.7007060050964355f;
        D_005092D0[1] = 0.0f;
        D_005092D0[2] = 0.0f;
        D_005092D0[3] = 0.0f;
        D_005092E0[0] = 1.0f;
        D_005092E0[1] = 0.7828530073165894f;
        D_005092E0[2] = 0.06140600144863129f;
        D_005092E0[3] = 0.04146299883723259f;
        D_005092F0[0] = 1.0f;
        D_005092F0[1] = 0.32487601041793823f;
        D_005092F0[2] = 1.0f;
        D_005092F0[3] = 0.35280099511146545f;
        D_00509300[0] = 1.0f;
        D_00509300[1] = 1.0f;
        D_00509300[2] = 0.7977359890937805f;
        D_00509300[3] = 0.0f;
        D_00509310[0] = 1.0f;
        D_00509310[1] = 0.4937489926815033f;
        D_00509310[2] = 1.0f;
        D_00509310[3] = 0.49179700016975403f;
        D_00509320[0] = 1.0f;
        D_00509320[1] = 1.0f;
        D_00509320[2] = 1.0f;
        D_00509320[3] = 1.0f;
        D_00509330[0] = 1.0f;
        D_00509330[1] = 0.800000011920929f;
        D_00509330[2] = 1.0f;
        D_00509330[3] = 0.800000011920929f;
        D_00509340[0] = 1.0f;
        D_00509340[1] = 1.0f;
        D_00509340[2] = 0.0f;
        D_00509340[3] = 0.0f;
        D_00509350[0] = 1.0f;
        D_00509350[1] = 1.0f;
        D_00509350[2] = 0.0f;
        D_00509350[3] = 0.0f;
        D_00509360[0] = 1.0f;
        D_00509360[1] = 1.0f;
        D_00509360[2] = 0.0f;
        D_00509360[3] = 0.0f;
        D_00509370[0] = 1.0f;
        D_00509370[1] = 0.3273420035839081f;
        D_00509370[2] = 1.0f;
        D_00509370[3] = 0.35390499234199524f;
        D_00509380[0] = 1.0f;
        D_00509380[1] = 1.0f;
        D_00509380[2] = 0.061000000685453415f;
        D_00509380[3] = 0.039000000804662704f;
        D_00509390[0] = 1.0f;
        D_00509390[1] = 1.0f;
        D_00509390[2] = 0.7999979853630066f;
        D_00509390[3] = 0.0f;
        D_005093A0[0] = 1.0f;
        D_005093A0[1] = 0.9589840173721313f;
        D_005093A0[2] = 0.6143789887428284f;
        D_005093A0[3] = 0.0f;
        D_005093B0[0] = 1.0f;
        D_005093B0[1] = 0.9285150170326233f;
        D_005093B0[2] = 0.3399839997291565f;
        D_005093B0[3] = 0.0f;
        D_005093C0[0] = 1.0f;
        D_005093C0[1] = 0.8738250136375427f;
        D_005093C0[2] = 0.24931700527668f;
        D_005093C0[3] = 0.017578000202775f;
        D_005093D0[0] = 1.0f;
        D_005093D0[1] = 0.7843739986419678f;
        D_005093D0[2] = 0.06132800132036209f;
        D_005093D0[3] = 0.03945299983024597f;
        D_005093E0[0] = 1.0f;
        D_005093E0[1] = 0.0f;
        D_005093E0[2] = 1.0f;
        D_005093E0[3] = 0.0f;
        D_005093F0[0] = 1.0f;
        D_005093F0[1] = 1.0f;
        D_005093F0[2] = 0.0f;
        D_005093F0[3] = 0.0f;
        D_004A5A74 = 0;
        D_004A5A70 = 0;
        initE(D_004A5A78, 0);
        initE(D_004A5A80, 1);
        initE(D_004A5A88, 2);
        initE(D_004A5A90, 3);
        initE(D_004A5A98, 4);
        initE(D_004A5AA0, 5);
        initE(D_004A5AA8, 6);
        initE(D_004A5AB0, 7);
        initE(D_004A5AB8, 8);
        initE(D_004A5AC0, 9);
        initE(D_004A5AC8, 10);
        initE(D_004A5AD0, 11);
        initE(D_004A5AD8, 12);
        initE(D_004A5AE0, 13);
        initD(D_00509400, D_2400);
        initD(D_00509410, D_2270);
        initD(D_00509420, D_22C8);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cChunkResolve__cChunkResolve);
#ifdef SKIP_ASM
extern void* cChunkResolve_vtbl[];
void operator_delete(int*);

/**
 * Destroys a chunk-resolve object: restores its vtable and frees it when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cChunkResolve__cChunkResolve(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = cChunkResolve_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC48__FPv);
#ifdef SKIP_ASM
/**
 * Reports success for a mesh-context record without doing any work: always returns 1.
 *
 * It sits in slot 8 of both cChunkResolve_vtbl and cPS2ChunkResolve_vtbl, between resolveMesh and resolveMaterial, so
 * it is the base chunk resolver's no-op handler for that record kind.
 *
 * @return always 1 (resolved)
 */
int func_003ADC48(void* self)
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC50);
#ifdef SKIP_ASM
/**
 * Returns the code of one script in a world-script cache entry.
 *
 * When the entry's state (the short at +0x0) is 3 (loaded), it reads the script table pointer at +0x3C of the entry's
 * data (+0x8) and returns element @p i; otherwise it returns 0. Unlike the other getters it does not check the data
 * pointer for null. cWorld_getWScriptScript calls it with the high 24 bits of a script resource id.
 *
 * @param i  index of the script within the section
 * @return the script's code pointer, or 0 when the entry is not loaded
 */
extern "C" int func_003ADC50(void* self, int i)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        return (*(int**)((char*)data + 0x3C))[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADC80);
#ifdef SKIP_ASM
/**
 * Returns how many global scripts a loaded world-script cache entry has.
 *
 * @return the count at +0x10 of the entry's data (+0x8) when the entry's state (the short at +0x0) is 3 (loaded) and it
 *         has data, else 0
 */
extern "C" int func_003ADC80(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x10);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADCB0);
#ifdef SKIP_ASM
/**
 * Returns the global-script records of a loaded world-script cache entry.
 *
 * @return the pointer at +0x14 of the entry's data (+0x8) when the entry's state (the short at +0x0) is 3 (loaded) and
 *         it has data, else 0
 */
extern "C" int func_003ADCB0(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x14);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_getNumProcessScripts);
#ifdef SKIP_ASM
/**
 * Returns how many process scripts a loaded world script section has.
 *
 * @return the count at +0x28 of the entry's data when the entry is loaded (state 3) and has data, else 0
 */
extern "C" int cWScriptCacheEntry_getNumProcessScripts(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x28);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", cWScriptCacheEntry_getProcessScripts);
#ifdef SKIP_ASM
/**
 * Returns the process-script table of a loaded world script section.
 *
 * @return the pointer at +0x2C of the entry's data when the entry is loaded (state 3) and has data, else 0
 */
extern "C" int cWScriptCacheEntry_getProcessScripts(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x2C);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADD40);
#ifdef SKIP_ASM
/**
 * Returns how many mission scripts a loaded world-script cache entry has.
 *
 * @return the count at +0x20 of the entry's data (+0x8) when the entry's state (the short at +0x0) is 3 (loaded) and it
 *         has data, else 0
 */
extern "C" int func_003ADD40(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x20);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("world/wscriptcache", func_003ADD70);
#ifdef SKIP_ASM
/**
 * Returns the mission-script records of a loaded world-script cache entry.
 *
 * @return the pointer at +0x24 of the entry's data (+0x8) when the entry's state (the short at +0x0) is 3 (loaded) and
 *         it has data, else 0
 */
extern "C" int func_003ADD70(void* self)
{
    if (*(short*)self == 3) {
        void* data = *(void**)((char*)self + 0x8);
        if (data != 0) {
            return *(int*)((char*)data + 0x24);
        }
    }
    return 0;
}
#endif

extern "C" void* func_003AD290(int, int);

//99.38%
INCLUDE_ASM("world/wscriptcache", func_003ADDA0__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static initializers: calls func_003AD290(1, 0xFFFF).
 *
 * func_003AD290 is the unit's static init/destroy routine (it fills the shared colour constants); the arguments select
 * the construct pass at the default priority 0xFFFF. The declared parameter is not used.
 *
 * @return func_003AD290's result (that routine returns nothing, so no meaningful value)
 */
void* func_003ADDA0(void* self)
{
    return func_003AD290(1, 0xffff);
}
#endif
