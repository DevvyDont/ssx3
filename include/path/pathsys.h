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

// One race checkpoint (On Tour struct tPathCheckpoint, 0x8). [Staged by views group A.]
// [U] cRider_checkTrackPathEvents walks cPathSys::CheckpointList[0..5] (view tPathCheckpoint_112FB0 in
// src/ai/computer.cpp; a zero DistanceLeft ends the list) and passes each checkpoint it crossed to
// cRider_flagEventPathCheckpoint, which scores TimeBonusSec (cRiderMetrix_evCheckpoint).
struct tPathCheckpoint {
    int TimeBonusSec;               // 0x0 [O]; [U] cRider_flagEventPathCheckpoint passes it to cRiderMetrix_evCheckpoint
    float DistanceLeft;             // 0x4 [O]; [U] cRider_checkTrackPathEvents compares it with mDistToFinish and
                                    //      MinDistanceLeft (the distance to the finish at the checkpoint)
};

CHECK_OFFSET(tPathCheckpoint, TimeBonusSec, 0x0);
CHECK_OFFSET(tPathCheckpoint, DistanceLeft, 0x4);
CHECK_SIZE(tPathCheckpoint, 0x8);

struct cPathSys {
    int NumStartpoints;             // 0x000 [O]
    tPathStartpoint* Startpoints;   // 0x004 [O]
    int NumAIPaths;                 // 0x008 [O]
    cAIPath* AIPaths;               // 0x00C [O]
    int NumTrackPaths;              // 0x010 [O]
    cTrackPath* TrackPaths;         // 0x014 [O]
    tPathCheckpoint CheckpointList[6]; // 0x018 [O]; [U] cRider_checkTrackPathEvents (view sPathSys_112FB0) [views group A:
                                    //       was char[0x30]]
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
