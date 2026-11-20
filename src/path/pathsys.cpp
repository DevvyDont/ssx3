#include "common.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("path/pathsys", cPathSys_loadTrackPaths);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void pathREAD_tInt32(void* dst, void* src);
extern "C" int cPath_initPath(void* path, char* data);
extern char str_TrackPaths[];
extern void* D_00481448[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sPathB310 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    sPathB310() { vtable = D_00481448; }
};

struct sPathSysB310 {
    char pad_0x0[0x10];
    int count;              // 0x10
    sPathB310* paths;       // 0x14
};

/**
 * Loads the track paths ("TrackPaths") from the level's path data (On Tour's loadTrackPaths; this was once named
 * resolvePaths, which in On Tour is a separate void(this) function).
 *
 * @param data  the serialized data: a count, then each path
 * @return number of bytes read
 */
extern "C" int cPathSys_loadTrackPaths(sPathSysB310* self, char* data)
{
    int count;
    sPathB310** slot = &self->paths;
    char* start = data;
    pathREAD_tInt32(&count, data);
    data += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB310), str_TrackPaths, 0, 0)) sPathB310[count];
    for (int i = 0; i < self->count; i++) {
        data += cPath_initPath(&self->paths[i], data);
    }
    return data - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_loadAIPaths);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void pathREAD_tInt32(void* dst, void* src);
extern "C" int cPath_initPath(void* path, char* data);
extern char str_AIPaths[];
extern void* D_00481478[];

struct sPathB410 {
    char pad_0x0[0x34];
    void** vtable;          // 0x34
    int field_0x38;         // 0x38
    int field_0x3C;         // 0x3C
    sPathB410() { vtable = D_00481478; }
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sPathSysB410 {
    char pad_0x0[0x8];
    int count;              // 0x8
    sPathB410* paths;       // 0xC
};

/**
 * Loads the AI paths ("AIPaths") from the level's path data.
 *
 * @param data  the serialized data: a count, then each path
 * @return number of bytes read
 */
extern "C" int cPathSys_loadAIPaths(sPathSysB410* self, char* data)
{
    int count;
    sPathB410** slot = &self->paths;
    char* start = data;
    pathREAD_tInt32(&count, data);
    data += 4;
    self->count = count;
    *slot = new (operator_new_tag(count * sizeof(sPathB410), str_AIPaths, 0, 0)) sPathB410[count];
    for (int i = 0; i < self->count; i++) {
        data += cPath_initPath(&self->paths[i], data);
    }
    return data - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_getTrackPathIndex);
#ifdef SKIP_ASM
struct sPathEntry28 {
    int field_0x0;
    int field_0x4;
    char pad_0x8[0x20];
};

struct sPathEntry40 {
    char pad_0x0[0x40];
};

struct sPathEntry3C {
    char pad_0x0[0x3c];
};

struct sPathSys {
    int count28;             // 0x0
    sPathEntry28* entries28; // 0x4
    int count40;             // 0x8
    sPathEntry40* entries40; // 0xc
    int count3C;             // 0x10
    sPathEntry3C* entries3C; // 0x14
};

/**
 * Returns the index of track path @p entry in the path system's track path array (+0x14, 0x3C bytes each), or -1.
 *
 * Searches the first count (+0x10) entries by address; a null @p entry or one not in the array gives -1.
 * cRiderHandler_writeToReplayFrame stores the rider's track path (+0xAB4) this way, and the read side turns the index
 * back into a pointer with cPathSys_getTrackPath.
 *
 * @param entry  the track path to look up
 * @return its index, or -1
 */
extern "C" int cPathSys_getTrackPathIndex(sPathSys* self, sPathEntry3C* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count3C; i++) {
            if (&self->entries3C[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_getAIPathIndex);
#ifdef SKIP_ASM
/**
 * Returns the index of AI path @p entry in the path system's AI path array (+0xC, 0x40 bytes each), or -1.
 *
 * Searches the first count (+0x8) entries by address; a null @p entry or one not in the array gives -1.
 * cRiderHandler_writeToReplayFrame stores the rider's AI path (+0xAB8) this way, and the read side turns the index back
 * into a pointer with cPathSys_getAIPath.
 *
 * @param entry  the AI path to look up
 * @return its index, or -1
 */
extern "C" int cPathSys_getAIPathIndex(sPathSys* self, sPathEntry40* entry)
{
    int i;
    if (entry != 0) {
        for (i = 0; i < self->count40; i++) {
            if (&self->entries40[i] == entry) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_isStartpoint);
#ifdef SKIP_ASM
/**
 * Reports whether the path data has a startpoint of a given type and id.
 *
 * Scans the startpoints (count and 0x28-byte entries) for one whose type (+0x4) equals @p type and id (+0x0) equals
 * @p startpointId. The map screen uses it with type 2 to list the track's sessions.
 *
 * @param type  the startpoint type (On Tour tPathStartType)
 * @param startpointId  the startpoint id (a session or rider index)
 * @return 1 if such a startpoint exists, else 0
 */
extern "C" int cPathSys_isStartpoint(sPathSys* self, int type, int startpointId)
{
    int i;
    int count = self->count28;
    for (i = 0; i < count; i++) {
        if (self->entries28[i].field_0x4 == type && self->entries28[i].field_0x0 == startpointId) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_getStartpoint);
#ifdef SKIP_ASM
struct sPathEntry {
    int id;                 // 0x0
    int type;               // 0x4
    char pad_0x8[0x20];
};

struct sPathEntryList {
    int count;              // 0x0
    sPathEntry* entries;    // 0x4
};

/**
 * Finds a startpoint by type and id, falling back to the first startpoint.
 *
 * Returns the first 0x28-byte entry whose type (+0x4) and id (+0x0) match; when none matches it returns entry 0, or
 * null if there are no startpoints at all, so callers always get a position when any exists. Used to place riders
 * (cRider_putAtStartpoint, cRider_GetNISEndPosition) and by the map screen.
 *
 * @param list  the path system (the startpoint count +0x0 and array +0x4 come first)
 * @param type  the startpoint type (On Tour tPathStartType: 1 and 2 are used)
 * @param id    the startpoint id
 * @return the matching startpoint, else the first one, else null
 */
extern "C" sPathEntry* cPathSys_getStartpoint(sPathEntryList* list, int type, int id)
{
    int i;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == type && list->entries[i].id == id) {
            return &list->entries[i];
        }
    }
    if (list->count > 0) {
        return list->entries;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_getStartpointByIndex);
#ifdef SKIP_ASM
/**
 * Returns startpoint @p index of the path system (0x28-byte records at +0x4), or 0 when out of range.
 *
 * Returns 0 when the startpoint count (+0x0) is not positive, or @p index is negative or not below it.
 *
 * @param list  the path system
 * @param index  the startpoint index
 * @return the startpoint, or 0
 */
extern "C" int cPathSys_getStartpointByIndex(void *list, int index) {
    int temp_2;

    temp_2 = (*(int *)((char*)(list) + (0)));
    if ((temp_2 <= 0) || (index >= temp_2) || (index < 0)) {
        return 0;
    }
    return (*(int *)((char*)(list) + (4))) + (index * 40);
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_getNearestSessionPoint);
#ifdef SKIP_ASM
struct sPathVec3 {
    float x, y, z;
    sPathVec3() {}
    sPathVec3(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

static inline sPathVec3 operator-(const sPathVec3& a, const sPathVec3& b)
{
    return sPathVec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct sPathNearEntry {
    int id;             // 0x0
    int type;           // 0x4
    sPathVec3 pos;      // 0x8
    char pad_0x14[0x14];
};

struct sPathNearList {
    int count;                  // 0x0
    sPathNearEntry* entries;    // 0x4
};

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_isSessionPoint(void* self, int value, int id);

// PORT: sqrt.s helper
static inline float pathSqrt(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float pathLen(const sPathVec3& v)
{
    return pathSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/**
 * Finds the session startpoint nearest a position, for the map screen's initial selection.
 *
 * Among the startpoints of type 2 with a non-zero id, keeps those that the mission interface (BE interface 10,
 * cBEMissionInterface_isSessionPoint) accepts for the current location (the section manager's +0x1BC) and returns the id of the
 * closest one; the search starts at a distance of 1e7. Returns -1 when none qualifies.
 *
 * @param list  the path system (the startpoint count +0x0 and array +0x4 come first)
 * @param pos   the position to measure from (the rider's)
 * @return the nearest session point's id, or -1
 */
extern "C" int cPathSys_getNearestSessionPoint(sPathNearList* list, sPathVec3* pos)
{
    int i;
    float best = 10000000.0f;
    int bestId = -1;
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == 2 && list->entries[i].id != 0) {
            if (cBEMissionInterface_isSessionPoint(cBE_getInterface_Fv(cBE_getBE(), 10), list->entries[i].id,
                              *(int*)((char*)SSXApp->Game->SectionMan + 0x1BC)) != 0) {
                sPathVec3 difference = list->entries[i].pos - *pos;
                float dist = pathLen(difference);
                if (dist < best) {
                    best = dist;
                    bestId = list->entries[i].id;
                }
            }
        }
    }
    return bestId;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_loadCheckpoints);
#ifdef SKIP_ASM
extern "C" void pathREAD_tInt32(void* dst, void* src);
extern "C" void pathREAD_float(void* dst, void* src);

struct sPathPair_B7D8 {
    int a;      // 0x0
    int b;      // 0x4
};

struct sPathSys_B7D8 {
    char pad_0x0[0x18];
    sPathPair_B7D8 pairs[1];    // 0x18
};

/**
 * Reads the path file's checkpoint table into the path system's checkpoint list (+0x18, 8 bytes each).
 *
 * Reads the count, then per checkpoint a float into the second word (On Tour DistanceLeft) with pathREAD_float and an
 * int into the first (On Tour TimeBonusSec) with pathREAD_tInt32. cPathSys_resolvePaths calls it after the AI and track
 * paths. Likely bug: the count is not limited to the list's 6 slots (On Tour skips entries past 6), so a larger count
 * would write over the data pointer at +0x48 and beyond.
 *
 * @param cursor  the file data at the checkpoint count
 * @return the number of bytes consumed
 */
extern "C" int cPathSys_loadCheckpoints(sPathSys_B7D8* self, char* cursor)
{
    int count;
    char* start = cursor;
    pathREAD_tInt32(&count, cursor);
    cursor += 4;
    for (int i = 0; i < count; i++) {
        pathREAD_float(&self->pairs[i].b, cursor);
        cursor += 4;
        pathREAD_tInt32(&self->pairs[i].a, cursor);
        cursor += 4;
    }
    return cursor - start;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPathSys_loadStartpoints);
#ifdef SKIP_ASM
extern "C" void pathREAD_tInt32(void* dst, void* src);
extern "C" void pathREAD_t3Vector(void* dst, void* src);

extern int D_00445428[];

struct sPathNode_B880 {
    int id;             // 0x0
    int type;           // 0x4
    char pos[0xC];      // 0x8
    char dir[0xC];      // 0x14
    char* f20;          // 0x20
    char* f24;          // 0x24
};

struct sPathSys_B880 {
    int count;                  // 0x0
    sPathNode_B880* nodes;      // 0x4
    int pad_0x8;                // 0x8
    char* fC;                   // 0xC
    int pad_0x10;               // 0x10
    char* f14;                  // 0x14
};

/**
 * Reads the path file's startpoints in place: points the startpoint array (+0x4) into the file and fills each record.
 *
 * Reads the count into +0x0, then for each 0x28-byte startpoint its arg (+0x0), a type code, its position (+0x8) and
 * direction (+0x14) with pathREAD_t3Vector, and an AI path index and a track path index. The type (+0x4) becomes the
 * position of the code in D_00445428 {-1, 0, 1} (else 0), and the indices become pointers into the AI path array
 * (+0xC, 0x40 bytes each; stored at +0x24) and the track path array (+0x14, 0x3C bytes each; stored at +0x20), so it
 * must run after those are loaded. cPathSys_resolvePaths calls it last.
 *
 * @param cursor  the file data at the startpoint count
 * @return the number of bytes consumed
 */
extern "C" int cPathSys_loadStartpoints(sPathSys_B880* self, char* cursor)
{
    int fileWord;
    int aiPathIndex;
    int trackPathIndex;
    char* start = cursor;
    pathREAD_tInt32(&fileWord, cursor);
    cursor += 4;
    self->nodes = (sPathNode_B880*)cursor;
    self->count = fileWord;
    for (int i = 0; i < self->count; i++) {
        pathREAD_tInt32(&self->nodes[i].id, cursor);
        cursor += 4;
        pathREAD_tInt32(&fileWord, cursor);
        cursor += 4;
        pathREAD_t3Vector(self->nodes[i].pos, cursor);
        cursor += 0xC;
        pathREAD_t3Vector(self->nodes[i].dir, cursor);
        cursor += 0xC;
        pathREAD_tInt32(&aiPathIndex, cursor);
        cursor += 4;
        pathREAD_tInt32(&trackPathIndex, cursor);
        cursor += 4;
        self->nodes[i].type = 0;
        for (int j = 0; j < 3; j++) {
            if (D_00445428[j] == fileWord) {
                self->nodes[i].type = j;
                break;
            }
        }
        self->nodes[i].f24 = self->fC + aiPathIndex * 0x40;
        self->nodes[i].f20 = self->f14 + trackPathIndex * 0x3C;
    }
    return cursor - start;
}
#endif

extern "C" void* MEM_copy(void*, void*, int);

//100%
INCLUDE_ASM("path/pathsys", pathREAD_t3Vector);
#ifdef SKIP_ASM
/**
 * Copies a 12-byte vector (three floats) from the path file at @p src to @p dst, unless they are the same address.
 *
 * Used by cPathSys_loadStartpoints for startpoint positions and directions and by the path readers for nodes and bounds.
 *
 * @param dst  the destination vector
 * @param src  the file data
 */
extern "C" void pathREAD_t3Vector(void* dst, void* src)
{
    if (dst != src) {
        MEM_copy(dst, src, 0xc);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", pathREAD_tInt32);
#ifdef SKIP_ASM
/**
 * Reads one 32-bit integer from (possibly unaligned) path data.
 *
 * Copies 4 bytes from @p src to @p dst with MEM_copy, skipping the copy when they are the same address. The path
 * loaders use it for counts and ids.
 *
 * @param dst  where to store the integer (On Tour dest)
 * @param src  the serialized data
 */
extern "C" void pathREAD_tInt32(void* dst, void* src)
{
    if (dst != src) {
        MEM_copy(dst, src, 0x4);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", pathREAD_float);
#ifdef SKIP_ASM
/**
 * Copies a 4-byte float from the path file at @p src to @p dst, unless they are the same address.
 *
 * Used for float fields of the path data (cTrackPath_populateData's base distance at +0x38, cPathSys_loadCheckpoints's
 * checkpoint distances).
 *
 * @param dst  the destination float
 * @param src  the file data
 */
extern "C" void pathREAD_float(void* dst, void* src)
{
    if (dst != src) {
        MEM_copy(dst, src, 0x4);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BAA8);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest.
 *
 * A variadic do-nothing: it only spills the remaining argument registers (a1-a7, f12-f18) into its frame. Nothing in
 * the build calls it.
 *
 * @return @p self
 */
extern "C" void* func_0026BAA8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026BAE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// A 16-byte vector global constructed from four floats.
struct sV4_BAE8 {
    float x, y, z, w;
    sV4_BAE8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    void* operator new(unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

// A 16-byte-aligned three-float vector global.
struct sV3_BAE8 {
    float x, y, z;
    sV3_BAE8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    void* operator new(unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));

// Three packed three-float vectors (D_004D3378).
struct sV3x3_BAE8 {
    float v[9];
    void* operator new(unsigned int, void* p) { return p; }
};

struct sSlot_BAE8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_BAE8(int n) { a = n; b = n; }
    void* operator new(unsigned int, void* p) { return p; }
};

extern sV4_BAE8 D_004D2FB8;
extern sV4_BAE8 D_004D2FC8;
extern sV4_BAE8 D_004D2FD8;
extern sV4_BAE8 D_004D2FE8;
extern sV4_BAE8 D_004D2FF8;
extern sV4_BAE8 D_004D3008;
extern sV4_BAE8 D_004D3018;
extern sV4_BAE8 D_004D3028;
extern sV4_BAE8 D_004D3038;
extern sV4_BAE8 D_004D3048;
extern sV4_BAE8 D_004D3058;
extern sV4_BAE8 D_004D3068;
extern sV4_BAE8 D_004D3078;
extern sV4_BAE8 D_004D3088;
extern sV4_BAE8 D_004D3098;
extern sV4_BAE8 D_004D30A8;
extern sV4_BAE8 D_004D30B8;
extern sV4_BAE8 D_004D30C8;
extern sV4_BAE8 D_004D30D8;
extern sV4_BAE8 D_004D30E8;
extern sV4_BAE8 D_004D30F8;
extern sV4_BAE8 D_004D3108;
extern sV4_BAE8 D_004D3118;
extern sV4_BAE8 D_004D3128;
extern sV4_BAE8 D_004D3138;
extern sV4_BAE8 D_004D3148;
extern sV4_BAE8 D_004D3158;
extern sV4_BAE8 D_004D3168;
extern sV4_BAE8 D_004D3178;
extern sV4_BAE8 D_004D3188;
extern sV4_BAE8 D_004D3198;
extern sV4_BAE8 D_004D31A8;
extern sV4_BAE8 D_004D31B8;
extern sV4_BAE8 D_004D31C8;
extern sV4_BAE8 D_004D31D8;
extern sV4_BAE8 D_004D31E8;
extern sV4_BAE8 D_004D31F8;
extern sV4_BAE8 D_004D3208;
extern sV4_BAE8 D_004D3218;
extern sV4_BAE8 D_004D3228;
extern sV4_BAE8 D_004D3238;
extern sV4_BAE8 D_004D3248;
extern sV4_BAE8 D_004D3258;
extern sV4_BAE8 D_004D3268;
extern sV4_BAE8 D_004D3278;
extern sV4_BAE8 D_004D3288;
extern sV4_BAE8 D_004D3298;
extern sV4_BAE8 D_004D32A8;
extern sV4_BAE8 D_004D32B8;
extern sV4_BAE8 D_004D32C8;
extern sV4_BAE8 D_004D32D8;
extern sV4_BAE8 D_004D32E8;
extern sV4_BAE8 D_004D32F8;
extern sV4_BAE8 D_004D3308;
extern sV4_BAE8 D_004D3318;
extern sV4_BAE8 D_004D3328;
extern sV3_BAE8 D_004D3338;
extern sV3_BAE8 D_004D3348;
extern sV3_BAE8 D_004D3358;
extern sV3_BAE8 D_004D3368;
extern sV3x3_BAE8 D_004D3378;
extern sSlot_BAE8 D_004A5158;

// Static initialisation of the unit's colour globals (g++ __static_initialization_and_destruction_0).
// The unit declares func_0026BAE8 returning void*; bind this definition by asm label.
extern "C" void func_0026BAE8_init(int initialize, int priority) __asm__("func_0026BAE8");
/**
 * Builds pathsys.cpp's static constants (this unit's g++ static initialisation routine).
 *
 * Acts only when @p initialize is nonzero and @p priority is 0xFFFF; there is no destruction branch. Fills the 56
 * four-float constants D_004D2FB8..D_004D3328, the three-float vectors D_004D3338 (1, 0, 0.5), D_004D3348 (0.5, 0, 0),
 * D_004D3358 (0, 0.5, 1) and D_004D3368 (0, 0, 0.5), the nine floats D_004D3378 (1, 1, 1, 0.5, 0.5, 0.5, 0.8, 0.3,
 * 0.8), and gives the key D_004A5158 -1 in both its fields. func_0026C438 runs it with (1, 0xFFFF).
 *
 * @param initialize  nonzero to build the constants
 * @param priority  initialisation priority; nothing happens unless it is 0xFFFF (the default)
 */
extern "C" void func_0026BAE8_init(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004D2FB8) sV4_BAE8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D2FC8) sV4_BAE8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2FD8) sV4_BAE8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D2FE8) sV4_BAE8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D2FF8) sV4_BAE8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D3008) sV4_BAE8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D3018) sV4_BAE8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D3028) sV4_BAE8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D3038) sV4_BAE8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D3048) sV4_BAE8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D3058) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3068) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D3078) sV4_BAE8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D3088) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3098) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D30A8) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D30B8) sV4_BAE8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D30C8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D30D8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D30E8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D30F8) sV4_BAE8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D3108) sV4_BAE8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D3118) sV4_BAE8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D3128) sV4_BAE8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D3138) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3148) sV4_BAE8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D3158) sV4_BAE8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D3168) sV4_BAE8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D3178) sV4_BAE8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D3188) sV4_BAE8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D3198) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31A8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31B8) sV4_BAE8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D31C8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31D8) sV4_BAE8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D31E8) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D31F8) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3208) sV4_BAE8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D3218) sV4_BAE8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D3228) sV4_BAE8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D3238) sV4_BAE8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D3248) sV4_BAE8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D3258) sV4_BAE8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D3268) sV4_BAE8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D3278) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3288) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3298) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D32A8) sV4_BAE8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D32B8) sV4_BAE8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D32C8) sV4_BAE8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D32D8) sV4_BAE8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D32E8) sV4_BAE8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D32F8) sV4_BAE8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D3308) sV4_BAE8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D3318) sV4_BAE8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D3328) sV4_BAE8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D3338) sV3_BAE8(1.0f, 0.0f, 0.5f);
        new (&D_004D3348) sV3_BAE8(0.5f, 0.0f, 0.0f);
        new (&D_004D3358) sV3_BAE8(0.0f, 0.5f, 1.0f);
        new (&D_004D3368) sV3_BAE8(0.0f, 0.0f, 0.5f);
        D_004D3378.v[0] = 1.0f;
        D_004D3378.v[1] = 1.0f;
        D_004D3378.v[2] = 1.0f;
        D_004D3378.v[3] = 0.5f;
        D_004D3378.v[4] = 0.5f;
        D_004D3378.v[5] = 0.5f;
        D_004D3378.v[6] = 0.800000011920929f;
        D_004D3378.v[7] = 0.30000001192092896f;
        D_004D3378.v[8] = 0.800000011920929f;
        new (&D_004A5158) sSlot_BAE8(-1);
    }
}
#endif

//100%
INCLUDE_ASM("path/pathsys", cPath_getStart);
#ifdef SKIP_ASM
/**
 * Returns a path's start point as a four-float vector: the three floats at +0xC of @p src with w = 1.
 *
 * Writes the result to @p dst (a struct return).
 *
 * @param dst  where the vector is written
 * @param src  the path
 * @return @p dst
 */
extern "C" void* cPath_getStart(void* dst, void* src)
{
    *(float*)((char*)dst + 0x0) = *(float*)((char*)src + 0xC);
    *(float*)((char*)dst + 0x4) = *(float*)((char*)src + 0x10);
    *(float*)((char*)dst + 0x8) = *(float*)((char*)src + 0x14);
    *(float*)((char*)dst + 0xC) = 1.0f;
    return dst;
}
#endif
//100%
INCLUDE_ASM("path/pathsys", cPath_getAllEvents);
#ifdef SKIP_ASM
/**
 * Returns a path's event array (+0x4, On Tour Events).
 *
 * @param path  the path
 * @return the 16-byte event nodes
 */
extern "C" int cPath_getAllEvents(void *path) {
    return (*(int *)((char*)(path) + (4)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", cPath_getNumEvents);
#ifdef SKIP_ASM
/**
 * Returns a path's event count (+0x0, On Tour NumEvents).
 *
 * @param path  the path
 * @return the number of events
 */
extern "C" int cPath_getNumEvents(int *path) {
    return *path;
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C410__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty out-of-line inline method).
 */
void func_0026C410(void* self)
{
}
#endif

//100%
INCLUDE_ASM("path/pathsys", func_0026C418__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty out-of-line inline method).
 */
void func_0026C418(void* self)
{
}
#endif

extern "C" void* func_0026BAE8(int, int);

//100%
INCLUDE_ASM("path/pathsys", cAIPath_getPathAttributes);
#ifdef SKIP_ASM
/**
 * Returns an AI path's attribute word (+0x38, On Tour PathAttributes).
 *
 * @param aiPath  the AI path
 * @return the attributes
 */
extern "C" int cAIPath_getPathAttributes(void *aiPath) {
    return (*(int *)((char*)(aiPath) + (0x38)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", cAIPath_getIsResetPath);
#ifdef SKIP_ASM
/**
 * Returns an AI path's reset-path flag (+0x3C, On Tour IsResetPath).
 *
 * @param aiPath  the AI path
 * @return nonzero for a reset path
 */
extern "C" int cAIPath_getIsResetPath(void *aiPath) {
    return (*(int *)((char*)(aiPath) + (0x3C)));
}
#endif
//100%
INCLUDE_ASM("path/pathsys", cTrackPath_getBaseDistanceLeft);
#ifdef SKIP_ASM
/**
 * Returns a track path's base distance left (+0x38, On Tour BaseDistanceLeft), the float cTrackPath_populateData reads.
 *
 * @param trackPath  the track path
 * @return the distance
 */
extern "C" float cTrackPath_getBaseDistanceLeft(void *trackPath) {
    return (*(float *)((char*)(trackPath) + (0x38)));
}
#endif

//99.38%
INCLUDE_ASM("path/pathsys", func_0026C438__FPv);
#ifdef SKIP_ASM
/**
 * Runs this unit's static constructors: calls func_0026BAE8 with (1, 0xFFFF).
 *
 * Listed in the global constructor table (word at 0x0043CE7C).
 *
 * @return func_0026BAE8's result
 */
void* func_0026C438(void* self)
{
    return func_0026BAE8(1, 0xffff);
}
#endif

