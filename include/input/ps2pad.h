#ifndef INPUT_PS2PAD_H
#define INPUT_PS2PAD_H

// cPS2PadDevice: one PS2 controller (a libpad port/slot): it opens the port, walks libpad's setup sequence (pad type,
// locked analogue mode, pressure mode, actuator alignment) one step per sample, reads the buttons / pressures / sticks
// into 24 values (sampleButtons) and drives the two vibration motors. cPadDevice_construct (the static
// cPadDevice::construct) makes one per port, at most two: `cMemMan_alloc(0x180, "PS2Pad", 0x4000000, 0)` +
// cPS2PadDevice_cPS2PadDevice(port i, slot 0), stored in cPadCache::PadDevices (input/padcache.h).
// EA's source: d:/ssx/code/source/input/ps2pad.cpp (preview assert path; the preview's class strings _vt$13cPS2PadDevice,
// "PS2Pad"). Retail code: unit input/inputparse (0x00326DF0..0x00327740, after cPadDevice_construct).
//
// Size 0x180: [R] the allocation in cPadDevice_construct. [O] On Tour's PSP counterpart cPSPPadDevice is 4 bytes (a
// vptr) - a different implementation, no member names to borrow.
// Base: cPadDevice (an interface: a vptr only; On Tour cPadDevice size 4; vtable cPadDevice_vtbl 0x0048E4B8, slots 1
// destructor cPadDevice__cPadDevice, 2 sampleButtons (pure: func_00411530), 3 numActuators (0), 4 setActuator (empty)).
// Its vptr is at 0: cPadDevice declares the virtuals and has no data. Vtable cPS2PadDevice_vtbl (0x0048E488, 4 slots:
// 1 cPS2PadDevice__cPS2PadDevice, 2 sampleButtons, 3 numActuators, 4 setActuator).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] the constructor cPS2PadDevice_cPS2PadDevice (vptr, port 0x4, slot 0x8, memset(0xC, 0, 36), state words
//       0x48..0x5C) and the destructor cPS2PadDevice__cPS2PadDevice; the allocation size;
//   [P] tools/layout_xlate.py: the preview's slot-1/slot-2 bodies touch the same offsets (0x0..0x5C, +0x0 shift); the
//       preview has no cPS2PadDevice accessors, so it names nothing;
//   [U] how retail code uses the field (src/input/inputparse.cpp; the view sPad_326EB0 in cPS2PadDevice_sampleButtons
//       has the same member offsets; the field names below are from that use: libpad calls take port/slot, the setup
//       states, the motor values);
//   [O] none (see above).
// No member names are EA's.

#include "layout.h"

// cPadDevice: the pad interface (views group D: sObj00326A68). [O] On Tour class cPadDevice (0x4, the vptr only)
// declaring ~cPadDevice, sampleButtons (pure), numActuators, setActuator(int, float); [R] cPadDevice_vtbl 0x0048E4B8
// has those four slots (above); [U] cPadCache_purge deletes each device through slot 1, the pad cache's actuator code
// calls slots 3 / 4 (views CountFn00326CF0 / SetFn00326CF0 are those entries: sVEntry<int(void*)> and
// sVEntry<void(void*, int, float)>, include/vtable.h). input/padcache.h forward-declares it (cPadCache::PadDevices).
// fe/fepopupmisc.cpp and fe/festateoptions.cpp each define a local `class cPadDevice` with real virtuals and do not
// include this header; a unit that includes both would have to drop its local class.
struct cPadDevice {
    void** mVtbl;                   // 0x000 [R] cPadDevice_vtbl / the derived device's vtable
};

CHECK_OFFSET(cPadDevice, mVtbl, 0x000);
CHECK_SIZE(cPadDevice, 0x4);

// cPadDevice's vtable slots ([O] On Tour's virtual order; [R] cPadDevice_vtbl, cPS2PadDevice_vtbl).
enum {
    kPadDeviceVt_dtor = 1,          // ~cPadDevice (deleting)
    kPadDeviceVt_sampleButtons = 2, // int sampleButtons(float* values, int max) ([O] cPSPPadDevice; pure in cPadDevice)
    kPadDeviceVt_numActuators = 3,  // int numActuators() (0 in cPadDevice)
    kPadDeviceVt_setActuator = 4    // void setActuator(int act, float level) (empty in cPadDevice)
};

struct cPS2PadDevice {
    // --- cPadDevice (vptr only)
    void** mVtbl;                   // 0x000 [R] ctor / dtor cPS2PadDevice_vtbl, then the dtor stores cPadDevice_vtbl
    // --- cPS2PadDevice
    int mPort;                      // 0x004 [R] ctor's port argument; [U] first argument of every libpad call (open
                                    //       func_003FF708, state scePadGetState, read scePadRead, close scePadPortClose ...)
    int mSlot;                      // 0x008 [R] ctor's slot argument (0); [U] second argument of the libpad calls
    char mReserved[0x24];           // 0x00C [R] the ctor zeroes 36 bytes (memset(this + 0xC, 0, 36)); never read;
                                    //       name ours
                                    //       or written otherwise (named for that: no use names it)
    int mActSmall;                  // 0x030 [U] setActuator(1, level): level != 0; sampleButtons sends it as the small
                                    //       motor's byte (actDirect[0], an lbu of the low byte) when it changed; open = 0
    int mLastSmall;                 // 0x034 [U] sampleButtons: the last value sent (open = -1)
    int mActBig;                    // 0x038 [U] setActuator(other, level): level * 205 + 50 clamped to 50..255, or 0;
                                    //       sent as actDirect[1] (lbu of the low byte); open = 0
    int mLastBig;                   // 0x03C [U] sampleButtons: the last value sent (open = -1)
    unsigned char mActDirect[8];    // 0x040 [U] sampleButtons stores [0] / [1] (sb) and passes &mActDirect to
                                    //       scePadSetActDirect (libpad's actuator-direct call: a 6-byte table; 8 with the
                                    //       padding to 0x48)
    int mPhase;                     // 0x048 [R] ctor 0 (open the port on the next sample); [U] sampleButtons' setup state
                                    //       0..10 (10 = reading)
    int mRetry;                     // 0x04C [R] ctor 0; [U] sampleButtons counts failed pressure / actuator requests to 5
    int mModeSet;                   // 0x050 [R] ctor 0; [U] set once the analogue mode lock was accepted
    int mPressure;                  // 0x054 [R] ctor 0; [U] 1 = pressure-sensitive mode: the 12 buttons are value / 255
    int mAnalog;                    // 0x058 [R] ctor 0; [U] 1 = an analogue pad: the sticks are read
    int mActuator;                  // 0x05C [R] ctor 0; [U] sampleButtons sets 1 after the actuator alignment
    char unk_060[0x20];             // 0x060 never accessed (likely padding that aligns the DMA buffer below)
    unsigned char mPadBuf[0x100];   // 0x080 [U] sampleButtons passes this + 0x80 to the port-open call func_003FF708
                                    //       (libpad's 256-byte DMA buffer); the 0x180 allocation ends with it
};

CHECK_OFFSET(cPS2PadDevice, mVtbl, 0x000);
CHECK_OFFSET(cPS2PadDevice, mPort, 0x004);
CHECK_OFFSET(cPS2PadDevice, mSlot, 0x008);
CHECK_OFFSET(cPS2PadDevice, mReserved, 0x00C);
CHECK_OFFSET(cPS2PadDevice, mActSmall, 0x030);
CHECK_OFFSET(cPS2PadDevice, mLastSmall, 0x034);
CHECK_OFFSET(cPS2PadDevice, mActBig, 0x038);
CHECK_OFFSET(cPS2PadDevice, mLastBig, 0x03C);
CHECK_OFFSET(cPS2PadDevice, mActDirect, 0x040);
CHECK_OFFSET(cPS2PadDevice, mPhase, 0x048);
CHECK_OFFSET(cPS2PadDevice, mRetry, 0x04C);
CHECK_OFFSET(cPS2PadDevice, mModeSet, 0x050);
CHECK_OFFSET(cPS2PadDevice, mPressure, 0x054);
CHECK_OFFSET(cPS2PadDevice, mAnalog, 0x058);
CHECK_OFFSET(cPS2PadDevice, mActuator, 0x05C);
CHECK_OFFSET(cPS2PadDevice, unk_060, 0x060);
CHECK_OFFSET(cPS2PadDevice, mPadBuf, 0x080);
CHECK_SIZE(cPS2PadDevice, 0x180);

#endif /* INPUT_PS2PAD_H */
