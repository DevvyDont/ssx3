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
#include "ui/uithing.h"

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

    // Methods bound to their retail symbols by asm labels.
    int loadInjectedPaths(char* data) __asm__("cPathSys_loadInjectedPaths");
    void loadPaths(const char* name) __asm__("cPathSys_loadPaths");
    int isPathfileLoaded() __asm__("cPathSys_isPathfileLoaded");
    void resolvePaths() __asm__("cPathSys_resolvePaths");
    void appendPaths(const char* name) __asm__("cPathSys_appendPaths");
    void purgePaths() __asm__("cPathSys_purgePaths");
    cTrackPath* getTrackPath(int pathIndex) __asm__("cPathSys_getTrackPath");
    cAIPath* getAIPath(int index) __asm__("cPathSys_getAIPath");
    int getNearTrackPaths(float* pos, cTrackPath** out, int max) __asm__("cPathSys_getNearTrackPaths");
    int loadTrackPaths(char* data) __asm__("cPathSys_loadTrackPaths");
    int loadAIPaths(char* data) __asm__("cPathSys_loadAIPaths");
    int getTrackPathIndex(cTrackPath* entry) __asm__("cPathSys_getTrackPathIndex");
    int getAIPathIndex(cAIPath* entry) __asm__("cPathSys_getAIPathIndex");
    bool isStartpoint(int type, int startpointId) __asm__("cPathSys_isStartpoint");
    int getStartpointByIndex(int index) __asm__("cPathSys_getStartpointByIndex");
    int loadCheckpoints(char* cursor) __asm__("cPathSys_loadCheckpoints");
    int loadStartpoints(char* cursor) __asm__("cPathSys_loadStartpoints");
    void removeInjectedPathFile() __asm__("cPathSys_removeInjectedPathFile__FPv");
    int getNearResetPaths(void* position, void** out, int maxOut) __asm__("cPathSys_getNearResetPaths__FPv");
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

// A start point's kind (tPathStartpoint::Type). [O] On Tour's enum tPathStartType, names and values; [U] retail fits:
// cSSXScriptEngine::FillOrientationFromStartPointInfo places a rider at its kPATH_START_Default point, and
// cRider::putAtDropOffBC, cPathSys_getNearestSessionPoint and the map screen's session list use kPATH_START_Session.
enum tPathStartType {
    kPATH_START_Null = 0,
    kPATH_START_Default = 1,
    kPATH_START_Session = 2,
    kPATH_START_Num = 3
};

// One race start point (On Tour struct tPathStartpoint, 0x28): its type / argument key, position and direction,
// and the paths a rider starting there begins on. [R] cPathSys_getStartpoint walks Startpoints 0x28 apart comparing
// Type (+0x4) and Arg (+0x0); [U] cSSXScriptEngine_FillOrientationFromStartPointInfo reads Pos (+0x8) and Dir
// (+0x14) (view sNode_27B750).
struct tPathStartpoint {
    int Arg;                        // 0x00 [O]; [R] getStartpoint compares it with its arg
    int Type;                       // 0x04 [O] tPathStartType; [R] getStartpoint compares it with its type
    t3Vector Pos;                   // 0x08 [O]; [U] FillOrientationFromStartPointInfo
    t3Vector Dir;                   // 0x14 [O]; [U] ditto (yaw / pitch)
    cTrackPath* InitTrackPath;      // 0x20 [O]
    cAIPath* InitAIPath;            // 0x24 [O]
};

CHECK_OFFSET(tPathStartpoint, Arg, 0x00);
CHECK_OFFSET(tPathStartpoint, Type, 0x04);
CHECK_OFFSET(tPathStartpoint, Pos, 0x08);
CHECK_OFFSET(tPathStartpoint, Dir, 0x14);
CHECK_OFFSET(tPathStartpoint, InitTrackPath, 0x20);
CHECK_OFFSET(tPathStartpoint, InitAIPath, 0x24);
CHECK_SIZE(tPathStartpoint, 0x28);

#endif
