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
// Size 0x5C: [R] cCameraTriggerMan::cCameraTriggerMan constructs the trigger list at 0x0, 2 active lists 4 bytes apart
// from 0x14 and 2 trigger stacks 0x20 apart from 0x1C (ending at 0x5C); the static initialiser func_00175A20 builds
// gCamTriggerMan (D_004C5830) with it, and the next .bss object (D_004C5890) is 0x60 further (0x5C rounded to 16).
// [O] On Tour's cCameraTriggerMan is 0x5C with the same three members at the same offsets. No bases, no vtable.
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] the constructor / destructor / init; [U] how retail code uses the field; [O] SSX On Tour's DWARF.

#include "layout.h"
#include "camera/trigger/cameratriggerlist.h"

struct cActiveTriggerNode;

// One trigger a view's rider is inside (On Tour class cActiveTriggerNode, 0xC with two bools; retail's bools are
// words, so 0x10). [R] cActiveTriggerList_add: cMemMan_alloc(0x10, "Active Camera Trigger"), Trigger = the item, Next =
// the old head, IsNewFlag = IsActiveFlag = 1; [U] cCameraTriggerMan_checkCollisions plays the enter action of nodes
// whose IsNewFlag is set and the exit action (then cActiveTriggerList_remove) of nodes whose IsActiveFlag is clear.
struct cActiveTriggerNode {
    tCameraTrigger* Trigger;        // 0x0 [O]; [R] add stores the item; [U] checkCollisions reads its TriggerID (+0x0)
                                    //       and Enter / Exit actions (+0xC / +0x10)
    cActiveTriggerNode* Next;       // 0x4 [O]; [R] add links the old head; [U] checkCollisions walks it
    int IsNewFlag;                  // 0x8 [O] bool (word); [R] add = 1; [U] checkCollisions: enter when set
    int IsActiveFlag;               // 0xC [O] bool (word); [R] add = 1; [U] checkCollisions: exit when clear
};

CHECK_OFFSET(cActiveTriggerNode, Trigger, 0x0);
CHECK_OFFSET(cActiveTriggerNode, Next, 0x4);
CHECK_OFFSET(cActiveTriggerNode, IsNewFlag, 0x8);
CHECK_OFFSET(cActiveTriggerNode, IsActiveFlag, 0xC);
CHECK_SIZE(cActiveTriggerNode, 0x10);

// The triggers a view's rider is currently inside (On Tour class cActiveTriggerList, 0x4): a singly linked node list
// (methods cActiveTriggerList_* in camera/trigger/triggeralgorithms and cameratriggervolumes).
struct tCameraTrigger;

void operator_delete(int* p);

struct cActiveTriggerList {
    cActiveTriggerNode* Head;       // 0x0 [O]; [R] cActiveTriggerList_cActiveTriggerList__FPv zeroes it; [U] purge /
                                    //       find / add / remove walk it

    // Methods bound to their retail symbols by asm labels.
    void purge() __asm__("cActiveTriggerList_purge__FP18cActiveTriggerList");
    void clearFlags() __asm__("cActiveTriggerList_clearFlags");
    cActiveTriggerNode* find(void* key) __asm__("cActiveTriggerList_find");
    void* find1(void* key) __asm__("cActiveTriggerList_find1");
    void add(tCameraTrigger* item) __asm__("cActiveTriggerList_add");
    cActiveTriggerNode* remove(cActiveTriggerNode* target) __asm__("cActiveTriggerList_remove");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    ~cActiveTriggerList();
    static void operator delete(void* p) { operator_delete((int*)p); }
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

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cCameraTriggerStack_init__FP19cCameraTriggerStack");
    void recordEnter(int triggerId) __asm__("cCameraTriggerStack_recordEnter__FPvi");
};

CHECK_OFFSET(cCameraTriggerStack, NumTriggers, 0x00);
CHECK_OFFSET(cCameraTriggerStack, LastEnterPlayed, 0x04);
CHECK_OFFSET(cCameraTriggerStack, TriggerIDStack, 0x08);
CHECK_SIZE(cCameraTriggerStack, 0x20);

struct cRider;

struct cCameraTriggerMan {
    cCameraTriggerList TriggerList;             // 0x00 [O]; [R] the ctor runs
                                                //       cCameraTriggerList_cCameraTriggerList__FPv on this, the dtor
                                                //       cCameraTriggerList::~cCameraTriggerList; [U] init
                                                //       (cCameraTriggerList_init), streamIn (loadFromBuffer),
                                                //       loadTriggers (loadFromFile), purge / setTriggers / streamOut
                                                //       (cCameraTriggerMan_reset) walk Triggers (0x0) and NumTriggers (0x4);
                                                //       getTriggerList returns this
    cActiveTriggerList ActiveTriggerList[2];    // 0x14 [O]; [R] the ctor's 2-element loop (stride 4), the dtor's
                                                //       reverse loop; [U] init / initOnRestart / cleanupOnExit / purge
                                                //       purge both; checkCollisions uses [rider->mViewIndex]
    cCameraTriggerStack CameraTriggerStack[2];  // 0x1C [O]; [R] the ctor's 2-element loop (stride 0x20); [U] init /
                                                //       initOnRestart / cleanupOnExit / purge / streamOut reset 0x1C
                                                //       and 0x3C; checkCollisions uses [rider->mViewIndex]

    // Methods bound to their retail symbols by asm labels.
    void cleanupOnExit() __asm__("cCameraTriggerMan_cleanupOnExit");
    void purge() __asm__("cCameraTriggerMan_purge");
    int loadTriggers(const char* file) __asm__("cCameraTriggerMan_loadTriggers");
    void streamIn(void* stream) __asm__("cCameraTriggerMan_streamIn");
    void init() __asm__("cCameraTriggerMan_init");
    void initOnRestart() __asm__("cCameraTriggerMan_initOnRestart");
    void loadTriggersIfNotStreaming() __asm__("cCameraTriggerMan_loadTriggersIfNotStreaming__FPv");
    void unresolve() __asm__("cCameraTriggerMan_unresolve__FPv");
    void setInGameTriggers() __asm__("cCameraTriggerMan_setInGameTriggers__FPv");
    void* setReplayTriggers() __asm__("cCameraTriggerMan_setReplayTriggers__FPv");
    void checkCollisions(cRider* rider) __asm__("cCameraTriggerMan_checkCollisions");
    void resolve(void* rid, void* stream) __asm__("cCameraTriggerMan_resolve");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cCameraTriggerMan();
};

CHECK_OFFSET(cCameraTriggerMan, TriggerList, 0x00);
CHECK_OFFSET(cCameraTriggerMan, ActiveTriggerList, 0x14);
CHECK_OFFSET(cCameraTriggerMan, CameraTriggerStack, 0x1C);
CHECK_SIZE(cCameraTriggerMan, 0x5C);

struct cRider;

// The state cCameraTriggerMan_checkCollisions hands the Nash octree walk (On Tour struct CameraCollisionCallback, 0x8:
// `cActiveTriggerList& ActiveTriggerList; cRider& rider;`, the callback type of the camera instantiation of
// nash_privy::QueryHelper::helpX). EA's references are pointers here. [Staged by views group A.]
// [U] checkCollisions builds it on the stack ({&ActiveTriggerList[view], rider}; view sCtx_D320) and the walk calls
// cActiveTriggerList_find / _add on the list and the rider's +0x6C0 interface (slot 5, its position) for each
// type-4 node.
struct CameraCollisionCallback {
    cActiveTriggerList* ActiveTriggerList; // 0x0 [O] (cActiveTriggerList&); [U] the walk's find / add list
    cRider* rider;                  // 0x4 [O] (cRider&); [U] the walk reads its position through the +0x6C0 vtable
};

CHECK_OFFSET(CameraCollisionCallback, ActiveTriggerList, 0x0);
CHECK_OFFSET(CameraCollisionCallback, rider, 0x4);
CHECK_SIZE(CameraCollisionCallback, 0x8);

// The one instance (D_004C5830; built by the static initialiser func_00175A20, destroyed by it with flags 2).
extern cCameraTriggerMan gCamTriggerMan;

#endif /* CAMERA_TRIGGER_CAMERATRIGGERMAN_H */
