#ifndef REPLAY_BXREPLAYFRAME_H
#define REPLAY_BXREPLAYFRAME_H

// cBxReplayFrame: the replay stream interface. Every replayable object writes its state to one and reads it back
// (writeToReplayFrame(cBxReplayFrame*) / readFromReplayFrame(cBxReplayFrame*) in On Tour); the object behind it is a
// cReplayFramePtr (replay/frameptr.h; a cReplayFrame or cPlaybackMan's saved frame), whose vtable implements these.
//
// A real polymorphic class, unlike the flat headers: it has no data, only the vptr at 0 (On Tour size 4), and the code
// only calls through it (it is never constructed or copied here, so no vtable or type_info is emitted for it). g++ 2.95
// keeps slot 0 of a vtable for the type_info entry, so write is slot 1 (+0x8), read slot 2 (+0x10) ... as in
// cReplayFramePtr_vtbl (slots 1..6 below, 7 the cReplayFramePtr destructor).
// [P] the preview's linkonce names (writeToReplayFrame__8cObjNodeP14cBxReplayFrame ...); [O] SSX On Tour's DWARF:
// class cBxReplayFrame (size 0x4), these six virtuals in this order (cReplayFramePtr's overrides list the same order); [R] retail cReplayFramePtr_vtbl / cReplayFrame_vtbl slots 1..6 = cReplayFramePtr_write,
// _read, _readReplayInstance, _readReplayInstance1, _writeReplayInstance, _writeReplayInstance1; [U] every
// writeToReplayFrame calls slot 1 with (data, byte count), every readFromReplayFrame slot 2.

#include "layout.h"

struct tInstanceRecord;
struct cInstanceNode;

class cBxReplayFrame {
public:
    virtual void write(const char* data, int len);                       // slot 1 (+0x08)
    virtual void read(char* data, int len);                              // slot 2 (+0x10)
    virtual tInstanceRecord* readReplayInstance();                       // slot 3 (+0x18)
    virtual tInstanceRecord* readReplayInstance(cInstanceNode* node);    // slot 4 (+0x20)
    virtual void writeReplayInstance(const tInstanceRecord* pInst);      // slot 5 (+0x28)
    virtual void writeReplayInstance(cInstanceNode* node);               // slot 6 (+0x30)
};

CHECK_SIZE(cBxReplayFrame, 0x4);

#endif
