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
struct tCollideRecord;          // world/worldrecords.h
struct tWScriptGlobalData;      // [O] 4 bytes (not laid out)
struct tWScriptMissionData;     // [O] 0x54 (not laid out; resolve's view sRec54_CCD8)
struct tWScriptProcessData;     // [O] 0x40 (laid out below; resolve's view sRec40_CCD8)
struct tWScriptTaskData;        // [O] 0x38 (not laid out)
struct tWScriptScriptHeader;    // [O] 4 bytes (not laid out)

// One instance's event scripts in a section's script file (On Tour tWScriptInstanceData, 0x18).
// A world-script process's data in the resolved script file (On Tour struct tWScriptProcessData, 0x40; same offsets;
// the base of tWScriptMissionData, 0x54). [O] On Tour's cWScriptProcess(cWScriptMan*, unsigned int sectionId, const
// tWScriptProcessData* data); [U] cWScriptProcess_cWScriptProcess (src/wscript/wscriptprocess.cpp, view sDef_307598):
// ProcessId -> ProcessId, NumTasks -> NumTasks and the Tasks new[] count, the 0x38-byte task records at unk_008 to
// cWScriptTask_initTask, the script ids through cWScriptMan_getScriptCode.
struct tWScriptProcessData {
    unsigned int ProcessId;         // 0x00 [O]; [U] -> cWScriptProcess::ProcessId
    int NumTasks;                   // 0x04 [O]; [U] -> NumTasks; the new[] count
    tWScriptTaskData* unk_008;      // 0x08 [O] an anonymous union (member names lost); [U] the NumTasks task records
                                    //      (0x38 each) handed to cWScriptTask_initTask
    unsigned int ProcessScriptId[3]; // 0x0C [O] tResourceID[3]; [U] -> ProcessScriptCode[i] (getScriptCode)
    unsigned int EventScriptId[10]; // 0x18 [O] tResourceID[10]; [U] -> EventScriptCode[j] (getScriptCode)
};

CHECK_OFFSET(tWScriptProcessData, ProcessId, 0x00);
CHECK_OFFSET(tWScriptProcessData, NumTasks, 0x04);
CHECK_OFFSET(tWScriptProcessData, unk_008, 0x08);
CHECK_OFFSET(tWScriptProcessData, ProcessScriptId, 0x0C);
CHECK_OFFSET(tWScriptProcessData, EventScriptId, 0x18);
CHECK_SIZE(tWScriptProcessData, 0x40);

struct tWScriptInstanceData {
    unsigned int EventScriptId[6];  // 0x00 [O] tResourceID[6]; [U] cWScriptCacheEntry_getWScriptHeaderId returns
                                    //      InstanceScriptData[slot].EventScriptId[entry] (0x18 stride)
};

CHECK_OFFSET(tWScriptInstanceData, EventScriptId, 0x00);
CHECK_SIZE(tWScriptInstanceData, 0x18);

// One spline's script settings (On Tour tSplineData, 4 bytes).
struct tSplineData {
    short Flags;                    // 0x0 [O]; [U] cWScriptCacheEntry_resolve builds the spline record's m_iFlags
                                    //      from it
    short SurfaceType;              // 0x2 [O]; [U] resolve stores it (sign-extended) in the spline's m_eSurfaceType
};

CHECK_OFFSET(tSplineData, Flags, 0x0);
CHECK_OFFSET(tSplineData, SurfaceType, 0x2);
CHECK_SIZE(tSplineData, 0x4);

// A collision-handle table entry (On Tour tCollideRecordHandle, 2 bytes): an index into CollideRecordEntryTable.
struct tCollideRecordHandle {
    unsigned short RecordHandle;    // 0x0 [O]; [U] resolve reads CollideHandleTable[i] (lhu) to pick the record
};

CHECK_SIZE(tCollideRecordHandle, 0x2);

// A collide-record table entry (On Tour union tCollideRecordEntry, 4 bytes): an offset from CollideRecords in the
// file, a pointer after cWScriptCacheEntry_resolve.
union tCollideRecordEntry {
    tCollideRecord* Record;         // [O]; [U] resolve stores CollideRecords + offset; the handle loop reads it
    int RecordOffset;               // [O]; [U] the offset before the fix-up
};

CHECK_SIZE(tCollideRecordEntry, 0x4);

// A world section's script file (On Tour tScriptFileHeader, 0x5C): the header of the "WScriptFile" buffer that
// cWScriptCacheEntry::mpScriptHeader points at. The pointer members are offsets from the header in the file;
// cWScriptCacheEntry_resolve turns the ten of them into pointers once the cookie and the world checksum match. Union
// member names from the prx DIEs (types.json drops them).
struct tScriptFileHeader {
    char Cookie[4];                 // 0x00 [O]; [U] resolve checks the first halfword against 0x1000
    char unk_004[0x8];              // 0x04 [O] char Coldfusion[4], char ScriptCookie[4]
    int WorldCheckSum;              // 0x0C [O]; [U] resolve compares it with the world data header's m_uiCheckSum
    int NumGlobalScriptData;        // 0x10 [O]; [U] the cache's global-script count getter returns it
    union {
        tWScriptGlobalData* GlobalScriptData; // 0x14 [O]; [U] resolve fixes it up; the global-table getter returns it
        int GlobalScriptDataOff;    // 0x14 [O]
    };
    int unk_018;                    // 0x18 [O] NumInstanceScriptData
    union {
        tWScriptInstanceData* InstanceScriptData; // 0x1C [O]; [U] resolve fixes it up; getWScriptHeaderId indexes it
        int InstanceScriptDataOff;  // 0x1C [O]
    };
    int NumMissionScriptData;       // 0x20 [O]; [U] resolve's mission loop bound
    union {
        tWScriptMissionData* MissionScriptData; // 0x24 [O]; [U] resolve fixes it up and walks it (0x54 each)
        int MissionScriptDataOff;   // 0x24 [O]
    };
    int NumProcessScriptData;       // 0x28 [O]; [U] resolve's process loop bound; the cache's process-count getter
    union {
        tWScriptProcessData* ProcessScriptData; // 0x2C [O]; [U] resolve fixes it up and walks it (0x40 each)
        int ProcessScriptDataOff;   // 0x2C [O]
    };
    int unk_030;                    // 0x30 [O] NumTaskScriptData
    union {
        tWScriptTaskData* TaskScriptData; // 0x34 [O]; [U] resolve fixes it up
        int TaskScriptDataOff;      // 0x34 [O]
    };
    int NumWScriptScriptHeaders;    // 0x38 [O]; [U] resolve's script-header loop bound
    union {
        tWScriptScriptHeader* WScriptScriptHeaders; // 0x3C [O]; [U] resolve fixes up the table and each entry; the
                                    //      cache's script getter indexes it
        int WScriptScriptHeadersOff; // 0x3C [O]
    };
    union {
        tCollideRecord* CollideRecords; // 0x40 [O]; [U] resolve fixes it up; the entry table's offsets are from it
        int CollideRecordsOff;      // 0x40 [O]
    };
    int NumCollideHandleTable;      // 0x44 [O]; [U] resolve's handle loop bound
    union {
        tCollideRecordHandle* CollideHandleTable; // 0x48 [O]; [U] resolve fixes it up and reads the handles
        int CollideHandleTableOff;  // 0x48 [O]
    };
    int NumCollideRecords;          // 0x4C [O]; [U] resolve's entry loops
    union {
        tCollideRecordEntry* CollideRecordEntryTable; // 0x50 [O]; [U] resolve fixes it up, then each entry
        int CollideRecordEntryTableOff; // 0x50 [O]
    };
    int NumSplineData;              // 0x54 [O]; [U] resolve's spline loop bound
    union {
        tSplineData* SplineData;    // 0x58 [O]; [U] resolve fixes it up and reads one per spline
        int SplineDataOff;          // 0x58 [O]
    };
};

CHECK_OFFSET(tScriptFileHeader, Cookie, 0x00);
CHECK_OFFSET(tScriptFileHeader, unk_004, 0x04);
CHECK_OFFSET(tScriptFileHeader, WorldCheckSum, 0x0C);
CHECK_OFFSET(tScriptFileHeader, NumGlobalScriptData, 0x10);
CHECK_OFFSET(tScriptFileHeader, GlobalScriptData, 0x14);
CHECK_OFFSET(tScriptFileHeader, unk_018, 0x18);
CHECK_OFFSET(tScriptFileHeader, InstanceScriptData, 0x1C);
CHECK_OFFSET(tScriptFileHeader, NumMissionScriptData, 0x20);
CHECK_OFFSET(tScriptFileHeader, MissionScriptData, 0x24);
CHECK_OFFSET(tScriptFileHeader, NumProcessScriptData, 0x28);
CHECK_OFFSET(tScriptFileHeader, ProcessScriptData, 0x2C);
CHECK_OFFSET(tScriptFileHeader, unk_030, 0x30);
CHECK_OFFSET(tScriptFileHeader, TaskScriptData, 0x34);
CHECK_OFFSET(tScriptFileHeader, NumWScriptScriptHeaders, 0x38);
CHECK_OFFSET(tScriptFileHeader, WScriptScriptHeaders, 0x3C);
CHECK_OFFSET(tScriptFileHeader, CollideRecords, 0x40);
CHECK_OFFSET(tScriptFileHeader, NumCollideHandleTable, 0x44);
CHECK_OFFSET(tScriptFileHeader, CollideHandleTable, 0x48);
CHECK_OFFSET(tScriptFileHeader, NumCollideRecords, 0x4C);
CHECK_OFFSET(tScriptFileHeader, CollideRecordEntryTable, 0x50);
CHECK_OFFSET(tScriptFileHeader, NumSplineData, 0x54);
CHECK_OFFSET(tScriptFileHeader, SplineData, 0x58);
CHECK_SIZE(tScriptFileHeader, 0x5C);

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
