#ifndef PATH_PATHSYS_H
#define PATH_PATHSYS_H

// cPathSys: the path system (path/pathsys): the world's start points, AI paths and track paths, the six race
// checkpoints and the loaded path file. Methods: loadAIPaths / loadCheckpoints / appendPaths, the path and start-point
// getters, the near-path queries.
//
// Size 0x50 (On Tour's; retail's methods reach 0x4C, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py). Enums and bools are words; members of class type are byte arrays
// (type in the comment).

#include "layout.h"

struct cAIPath;
struct cTrackPath;
struct tPathStartpoint;

struct cPathSys {
    int NumStartpoints;             // 0x000 [O]
    tPathStartpoint* Startpoints;   // 0x004 [O]
    int NumAIPaths;                 // 0x008 [O]
    cAIPath* AIPaths;               // 0x00C [O]
    int NumTrackPaths;              // 0x010 [O]
    cTrackPath* TrackPaths;         // 0x014 [O]
    char CheckpointList[0x30];      // 0x018 [O] tPathCheckpoint[6]
    char* Data;                     // 0x048 [O]
    int m_bIsFileLoaded;            // 0x04C [O] bool
};

CHECK_OFFSET(cPathSys, NumStartpoints, 0x000);
CHECK_OFFSET(cPathSys, Startpoints, 0x004);
CHECK_OFFSET(cPathSys, NumAIPaths, 0x008);
CHECK_OFFSET(cPathSys, AIPaths, 0x00C);
CHECK_OFFSET(cPathSys, NumTrackPaths, 0x010);
CHECK_OFFSET(cPathSys, TrackPaths, 0x014);
CHECK_OFFSET(cPathSys, CheckpointList, 0x018);
CHECK_OFFSET(cPathSys, Data, 0x048);
CHECK_OFFSET(cPathSys, m_bIsFileLoaded, 0x04C);
CHECK_SIZE(cPathSys, 0x50);

#endif
