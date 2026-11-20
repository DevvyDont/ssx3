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

struct cCameraTriggerList {
    tCameraTrigger** Triggers;      // 0x00 [O]; [U] loadFromBuffer allocates it (NumTriggers entries)
    int NumTriggers;                // 0x04 [O]; [U] readHeader reads it from the stream
    unsigned int NextID;            // 0x08 [O]; [U] readHeader reads it
    int Cookie;                     // 0x0C [O]; [U] readCookie reads it; loadFromBuffer requires 7 and stores 7
    int Time;                       // 0x10 [O]; [U] readHeader reads it
};

CHECK_OFFSET(cCameraTriggerList, Triggers, 0x00);
CHECK_OFFSET(cCameraTriggerList, Cookie, 0x0C);
CHECK_SIZE(cCameraTriggerList, 0x14);

#endif /* CAMERA_TRIGGER_CAMERATRIGGERLIST_H */
