#ifndef FE_FEINPUT_H
#define FE_FEINPUT_H

// cFEInput: the front end's cUIInput (ui/uiinput.h): reads the controlling pad through a cInput each frame and keeps
// whether a key is ready, the virtual key and the ASCII key for the UI engine. Methods in fe/debugfe and
// main/all_in_one_main (constructor, destructor, update, the getters, the camera queries); the load screens build the
// same object inline.
//
// Size 0x1C (On Tour's is the same). A FLAT layout, like main/game.h: the cUIInput part is written out inline (keep it
// in step with ui/uiinput.h). Member names are EA's, from SSX On Tour's PSP DWARF ([O]); [R] the retail constructor
// cFEInput_cFEInput; [U] how retail code uses the field.

#include "layout.h"
#include "ui/uiinput.h"

struct cInput;

struct cFEInput : public cUIInput {
    // cUIInput (ui/uiinput.h, 0xC): the vptr at 0x08:
    //   [R] ctor = cFEInput_vtbl
    // cFEInput's notes on cUIInput's fields:
    //   mPadNum: [R] ctor = 0; [U] setPadNum
    // --- cFEInput
    int mbKeyReady;                 // 0x0C [O] (bool there); [R] ctor = 0; [U] getKeyReady, update
    int miVKey;                     // 0x10 [O]; [R] ctor = 0; [U] getVKey, update
    int miAsciiKey;                 // 0x14 [O]; [R] ctor = 0; [U] getAsciiKey, update
    cInput* mpInput;                // 0x18 [O]; [R] ctor = 0; [U] update, the camera queries

    // Methods bound to their retail symbols by asm labels.
    int getInput() __asm__("cFEInput_getInput");
};

CHECK_OFFSET(cFEInput, mbKeyReady, 0x0C);
CHECK_OFFSET(cFEInput, mpInput, 0x18);
CHECK_SIZE(cFEInput, 0x1C);

#endif
