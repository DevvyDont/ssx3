#ifndef BE_BEINTERFACE_H
#define BE_BEINTERFACE_H

// cBEInterface: the base of every back-end interface (cBENewPlayerInterface, cBEMissionInterface, cBEOptionInterface,
// BEBoltInt, ...). cBE::getInterface(kind) (cBE_getInterface__Fv) hands one out by kind through the current
// cBEState; callers that only submit or discard it hold it as a cBEInterface*: `iface->submit()`.
//
// A real polymorphic class: three bools, then the vptr. ee-gcc 2.95.3 puts the vptr after the own data of the class
// that declares the first virtual, so it lands at 0xC; nothing defines a member here, so no vtable or type_info is
// emitted. The interfaces' own headers (be/beint*.h) stay flat (their bools and `mVtbl` written out at the same
// offsets); their getThis functions store the vtables.
// Evidence:
//   [R] every interface's getThis allocates 0x10 + its own data and stores its vtable at 0xC; cBEInterface_vtbl
//       (4 slots, no destructor): cBEInterface_submit, _discard, _writeToReplayFrame, _readFromReplayFrame (all
//       empty); the interfaces override submit / discard (cBENewPlayerInterface_vtbl ...) and the replay pair
//       (cBEMissionInterface_vtbl);
//   [O] SSX On Tour's DWARF: class cBEInterface { bool bCanSubmit, bDataChanged, bIsActive; virtual submit, discard,
//       writeToReplayFrame(cReplayFramePtr*), readFromReplayFrame(cReplayFramePtr*) } (On Tour's bools are bytes,
//       retail's words: PS2 g++ bools are 4 bytes).

#include "layout.h"

class cBxReplayFrame;

class cBEInterface {
public:
    int bCanSubmit;                                              // 0x000 [O] bool; no retail access seen
    int bDataChanged;                                            // 0x004 [O] bool; no retail access seen
    int bIsActive;                                               // 0x008 [O] bool; [R] the getThis functions set 0
    // 0x00C: the vptr. Slots [R] cBEInterface_vtbl, names and order [O]:
    virtual void submit();                                       // slot 1 cBEInterface_submit (empty)
    virtual void discard();                                      // slot 2 cBEInterface_discard (empty)
    virtual void writeToReplayFrame(cBxReplayFrame* data);       // slot 3 cBEInterface_writeToReplayFrame (empty)
    virtual void readFromReplayFrame(cBxReplayFrame* data);      // slot 4 cBEInterface_readFromReplayFrame (empty)
};

CHECK_OFFSET(cBEInterface, bCanSubmit, 0x000);
CHECK_OFFSET(cBEInterface, bDataChanged, 0x004);
CHECK_OFFSET(cBEInterface, bIsActive, 0x008);
CHECK_SIZE(cBEInterface, 0x10);

#endif /* BE_BEINTERFACE_H */
