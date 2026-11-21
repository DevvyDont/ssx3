#ifndef WSCRIPT_MISSIONMENUQUEUE_H
#define WSCRIPT_MISSIONMENUQUEUE_H

// tMissionMenuQueue: the world script manager's 16-entry ring of mission-menu requests (cWScriptMan::MissionMenuQueue,
// wscript/wscriptman.h 0x1C4, today a char[0xC8]). The world scripts post {mission, command, being} requests with
// addEntry (cWScriptMan's mission commands, wscript/wscriptman.cpp:1393), and the front end takes them with
// getEntry / getNextEntry (cWScriptMan's getMenuQueueEntry / popMenuQueueEntry in the preview and On Tour); empty
// drops all of them (cWScriptMan's reset, wscriptman.cpp:2438).
// Name, members and methods are EA's, from SSX On Tour's DWARF (struct tMissionMenuQueue, size 0xC8, in
// z:/main/game/source/wscript/all_in_one_wscript.cpp, with tMissionMenuQueueEntry); the fork's retail functions already
// carry the names (tMissionMenuQueue_tMissionMenuQueue, _init, _empty, _addEntry, _getNextEntry, _getEntry in
// wscript/wscriptman, 0x0030C740..), and their bodies match On Tour's method set one for one. EA's header is not known;
// the file name is ours. Retail code: wscript/wscriptman (wscriptman.cpp:3281..3405).
//
// Size 0xC8: [R] init / empty clear 0x0 and 0x4 and MEM_fill 0xC0 bytes at 0x8 (16 * 0xC); [O] On Tour's 0xC8; the
// cWScriptMan member is 0xC8 bytes. No vtable, no base class.
// A FLAT layout, like main/game.h. Evidence tags: [R] init / empty; [U] how retail code uses the field
// (src/wscript/wscriptman.cpp views cWSQueue / cWScriptQueue, sharing this layout); [O] SSX On Tour's DWARF.

#include "layout.h"

struct tMissionMenuQueueEntry {
    unsigned int MissionId;         // 0x0 [O]; [U] addEntry's missionId
    int Cmd;                        // 0x4 [O]; [U] addEntry's cmd
    int Being;                      // 0x8 [O]; [U] addEntry's being (cWScriptMan::Being at the call)
};

CHECK_OFFSET(tMissionMenuQueueEntry, MissionId, 0x0);
CHECK_OFFSET(tMissionMenuQueueEntry, Cmd, 0x4);
CHECK_OFFSET(tMissionMenuQueueEntry, Being, 0x8);
CHECK_SIZE(tMissionMenuQueueEntry, 0xC);

struct tMissionMenuQueue {
    int Current;                    // 0x00 [O]; [R] init / empty 0; [U] the read index: getEntry returns
                                    //      &Queue[Current] unless Current == Next, getNextEntry also advances it mod 16
    int Next;                       // 0x04 [O]; [R] init / empty 0; [U] the write index: addEntry fills Queue[Next]
                                    //      and advances it mod 16
    tMissionMenuQueueEntry Queue[16]; // 0x08 [O]; [R] init / empty MEM_fill 0xC0 bytes
};

CHECK_OFFSET(tMissionMenuQueue, Current, 0x00);
CHECK_OFFSET(tMissionMenuQueue, Next, 0x04);
CHECK_OFFSET(tMissionMenuQueue, Queue, 0x08);
CHECK_SIZE(tMissionMenuQueue, 0xC8);

#endif /* WSCRIPT_MISSIONMENUQUEUE_H */
