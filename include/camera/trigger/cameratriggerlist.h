#ifndef CAMERA_TRIGGER_CAMERATRIGGERLIST_H
#define CAMERA_TRIGGER_CAMERATRIGGERLIST_H

// cCameraTriggerList: the camera triggers of a level, loaded from a buffer: the trigger array ("Camera Trigger
// Array", one tCameraTrigger* per trigger), their count, the next trigger ID, the format cookie (version; loading
// requires 7) and a time stamp. Methods in camera/trigger/cameratriggerlist and camera/script/scriptcontroller.
//
// Size 0x14 (On Tour's; retail's methods reach Time at size - 4). Member names are EA's, from SSX On Tour's PSP DWARF
// ([O]); every retail access through `this` falls on a member start (tools/gen/self_offsets.py). [U] retail use.

#include "layout.h"

struct tCameraTrigger;
struct cCTVolume;               // camera/trigger/cameratriggervolumes.h
struct cCTAction;               // camera/trigger/cameratriggeractions.h

// A trigger's node in the world's space hash (On Tour cCTNode : cTypedSpaceNode : Node, 0x10; the same shape as
// object/railspacenode.h's cRailSpaceNode). [R] the trigger ctor inlined in cCameraTriggerList_loadFromBuffer sets
// type 4 (tCameraTrigger +0x1C); [U] the hash walks (descend / WalkList_4848 in cameratriggerfactory, walk_D320 in
// cameratriggerman) follow next, keep the nodes whose type is 4 and take their Trigger (+0xC).
struct cCTNode {
    cCTNode* next;                  // 0x00 [O] nash Node::next; [U] the walks follow it (lw 0x0)
    cCTNode* prev;                  // 0x04 [O] nash Node::prev
    int type;                       // 0x08 [O] cTypedSpaceNode::type (eSpaceNodeType); [R] 4 for a camera trigger;
                                    //      [U] the walks compare it with 4
    tCameraTrigger* Trigger;        // 0x0C [O]; [U] the walks read the trigger through it
};

CHECK_OFFSET(cCTNode, next, 0x00);
CHECK_OFFSET(cCTNode, prev, 0x04);
CHECK_OFFSET(cCTNode, type, 0x08);
CHECK_OFFSET(cCTNode, Trigger, 0x0C);
CHECK_SIZE(cCTNode, 0x10);

// One camera trigger (On Tour struct tCameraTrigger, 0x24): a volume, the actions played on entering / leaving it and
// its space-hash node. [R] cCameraTriggerList_loadFromBuffer: new (0x24, "Camera Trigger") with the inline ctor
// (view tCameraTrigger_K3: TriggerID 0, GameMode 3, Volume / EnterAction / ExitAction 0, SpaceNode.type 4), one per
// Triggers[] entry; [U] cCameraTriggerMan_checkCollisions records TriggerID on the trigger stack and plays EnterAction /
// ExitAction (doTriggerAction_D320).
struct tCameraTrigger {
    unsigned int TriggerID;         // 0x00 [O]; [R] ctor 0; [U] checkCollisions: recordEnter / recordExit key
    unsigned int GameMode;          // 0x04 [O]; [R] ctor 3
    cCTVolume* Volume;              // 0x08 [O]; [R] ctor 0
    cCTAction* EnterAction;         // 0x0C [O]; [R] ctor 0; [U] checkCollisions plays it on entry
    cCTAction* ExitAction;          // 0x10 [O]; [R] ctor 0; [U] checkCollisions plays it on exit
    cCTNode SpaceNode;              // 0x14 [O]; [R] ctor: type (0x1C) = 4
};

CHECK_OFFSET(tCameraTrigger, TriggerID, 0x00);
CHECK_OFFSET(tCameraTrigger, GameMode, 0x04);
CHECK_OFFSET(tCameraTrigger, Volume, 0x08);
CHECK_OFFSET(tCameraTrigger, EnterAction, 0x0C);
CHECK_OFFSET(tCameraTrigger, ExitAction, 0x10);
CHECK_OFFSET(tCameraTrigger, SpaceNode, 0x14);
CHECK_SIZE(tCameraTrigger, 0x24);

// The camera trigger loaders' result. [O] On Tour's enum eCamLoadErrorStatus, names and values (loadFromBuffer and
// readHeader return it); [U] cCameraTriggerList::loadFromBuffer returns eCamLoadErrorCookieMismatch for a file whose
// cookie is not kCookie.
enum eCamLoadErrorStatus {
    eCamLoadErrorNone = 0, eCamLoadErrorMissingFile = 1, eCamLoadErrorCouldNotLoadFile = 2,
    eCamLoadErrorCookieMismatch = 3, eCamLoadErrorInvalidVolumeType = 4, eCamLoadErrorInvalidBoundObjType = 5,
    eCamLoadErrorInvalidActionType = 6
};

void operator_delete(int* p);

struct cCameraTriggerList {
    // The trigger file version this build reads (name ours): [U] loadFromBuffer rejects any other Cookie and passes it
    // to readHeader and readTrigger.
    enum { kCookie = 7 };
    tCameraTrigger** Triggers;      // 0x00 [O]; [U] loadFromBuffer allocates it (NumTriggers entries)
    int NumTriggers;                // 0x04 [O]; [U] readHeader reads it from the stream
    unsigned int NextID;            // 0x08 [O]; [U] readHeader reads it
    int Cookie;                     // 0x0C [O]; [U] readCookie reads it; loadFromBuffer requires kCookie and stores it
    int Time;                       // 0x10 [O]; [U] readHeader reads it

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cCameraTriggerList_init__FP18cCameraTriggerList");
    void reset() __asm__("cCameraTriggerList_reset");
    int* findTrigger(int key) __asm__("cCameraTriggerList_findTrigger");
    int loadFromFile(const char* name) __asm__("cCameraTriggerList_loadFromFile");
    int loadFromBuffer(void* buf) __asm__("cCameraTriggerList_loadFromBuffer");
    void initHeader() __asm__("cCameraTriggerList_initHeader__FP18cCameraTriggerList");
    void readCookie(void* buffer) __asm__("cCameraTriggerList_readCookie__FP18cCameraTriggerListPv");
    int readHeader(void* buffer) __asm__("cCameraTriggerList_readHeader");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    ~cCameraTriggerList();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cCameraTriggerList, Triggers, 0x00);
CHECK_OFFSET(cCameraTriggerList, Cookie, 0x0C);
CHECK_SIZE(cCameraTriggerList, 0x14);

#endif /* CAMERA_TRIGGER_CAMERATRIGGERLIST_H */
