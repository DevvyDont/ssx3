#ifndef CAMERA_TRIGGER_CAMERATRIGGERMAN_H
#define CAMERA_TRIGGER_CAMERATRIGGERMAN_H

// cCameraTriggerMan: the level's camera trigger volumes. It holds the trigger list streamed in with the track
// (streamIn / resolve), registers the triggers of the current game mode in the world's space hash (setTriggers:
// in-game or replay), and per view keeps the triggers a human rider is inside (an active list) and the order it
// entered them (a trigger stack), from which checkCollisions plays the enter / exit camera actions. One global
// instance, gCamTriggerMan. EA's source: d:/ssx/code/source/camera/trigger/cameratriggerman.cpp (preview assert
// path); retail keeps the ctor, dtor, init and initOnRestart in camera/trigger/triggeralgorithms and the rest in
// camera/trigger/cameratriggerman (0x0016C9B0 cleanupOnExit ... 0x0016D320 checkCollisions).
//
// Size 0x5C: [R] cCameraTriggerMan_cCameraTriggerMan constructs the trigger list at 0x0, 2 active lists 4 bytes apart
// from 0x14 and 2 trigger stacks 0x20 apart from 0x1C (ending at 0x5C); the static initialiser func_00175A20 builds
// gCamTriggerMan (D_004C5830) with it, and the next .bss object (D_004C5890) is 0x60 further (0x5C rounded to 16).
// [O] On Tour's cCameraTriggerMan is 0x5C with the same three members at the same offsets. No bases, no vtable.
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] the constructor / destructor / init; [U] how retail code uses the field; [O] SSX On Tour's DWARF.

#include "layout.h"
#include "camera/trigger/cameratriggerlist.h"

struct cActiveTriggerNode;

// The triggers a view's rider is currently inside (On Tour class cActiveTriggerList, 0x4): a singly linked node list
// (methods cActiveTriggerList_* in camera/trigger/triggeralgorithms and cameratriggervolumes).
struct cActiveTriggerList {
    cActiveTriggerNode* Head;       // 0x0 [O]; [R] cActiveTriggerList_cActiveTriggerList__FPv zeroes it; [U] purge /
                                    //       find / add / remove walk it
};

CHECK_OFFSET(cActiveTriggerList, Head, 0x0);
CHECK_SIZE(cActiveTriggerList, 0x4);

// The order in which a view's rider entered the triggers it is inside (On Tour class cCameraTriggerStack, 0x20).
struct cCameraTriggerStack {
    unsigned int NumTriggers;       // 0x00 [O]; [R] cCameraTriggerStack_init stores 0; [U] recordEnter pushes at
                                    //      TriggerIDStack[NumTriggers++]; recordExitGetNext compares it unsigned (sltu)
    unsigned int LastEnterPlayed;   // 0x04 [O]; [R] init stores 0; [U] recordEnter stores the entered id;
                                    //      recordExitGetNext compares the new top with it
    unsigned int TriggerIDStack[6]; // 0x08 [O]; [U] recordEnter / recordExitGetNext (+0x8 + i * 4); the stride of
                                    //      cCameraTriggerMan's stack array (0x20) bounds it to 6
};

CHECK_OFFSET(cCameraTriggerStack, NumTriggers, 0x00);
CHECK_OFFSET(cCameraTriggerStack, LastEnterPlayed, 0x04);
CHECK_OFFSET(cCameraTriggerStack, TriggerIDStack, 0x08);
CHECK_SIZE(cCameraTriggerStack, 0x20);

struct cCameraTriggerMan {
    cCameraTriggerList TriggerList;             // 0x00 [O]; [R] the ctor runs
                                                //       cCameraTriggerList_cCameraTriggerList__FPv on this, the dtor
                                                //       cCameraTriggerList__cCameraTriggerList; [U] init
                                                //       (cCameraTriggerList_init), streamIn (loadFromBuffer),
                                                //       loadTriggers (loadFromFile), purge / setTriggers / streamOut
                                                //       (func_0016CA18) walk Triggers (0x0) and NumTriggers (0x4);
                                                //       getTriggerList returns this
    cActiveTriggerList ActiveTriggerList[2];    // 0x14 [O]; [R] the ctor's 2-element loop (stride 4), the dtor's
                                                //       reverse loop; [U] init / initOnRestart / cleanupOnExit / purge
                                                //       purge both; checkCollisions uses [rider->mViewIndex]
    cCameraTriggerStack CameraTriggerStack[2];  // 0x1C [O]; [R] the ctor's 2-element loop (stride 0x20); [U] init /
                                                //       initOnRestart / cleanupOnExit / purge / streamOut reset 0x1C
                                                //       and 0x3C; checkCollisions uses [rider->mViewIndex]
};

CHECK_OFFSET(cCameraTriggerMan, TriggerList, 0x00);
CHECK_OFFSET(cCameraTriggerMan, ActiveTriggerList, 0x14);
CHECK_OFFSET(cCameraTriggerMan, CameraTriggerStack, 0x1C);
CHECK_SIZE(cCameraTriggerMan, 0x5C);

// The one instance (D_004C5830; built by the static initialiser func_00175A20, destroyed by it with flags 2).
extern cCameraTriggerMan gCamTriggerMan;

#endif /* CAMERA_TRIGGER_CAMERATRIGGERMAN_H */
