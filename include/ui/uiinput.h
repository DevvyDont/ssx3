#ifndef UI_UIINPUT_H
#define UI_UIINPUT_H

// cUIInput: the UI engine's input interface (EA's UI library; cUIEngine::mpInput). The base class keeps the last key
// and the controlling pad; the game's front end derives cFEInput (fe/feinput.h), which reads the pad. Most of the
// virtuals (getKeyReady, getVKey, getAsciiKey, isStart, the camera queries ...) are stubs in the base. Methods in
// fe/fepopupmisc and main/all_in_one_main.
//
// Size 0xC (On Tour's is the same). A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP
// DWARF ([O]); [U] how retail code uses the field. The vtable pointer is g++ 2.95's, after the members.

#include "layout.h"

struct cUIInput {
    int mKey;                       // 0x0 [O]; [U] cUIInput_setKey
    unsigned char mPadNum;          // 0x4 [O]; [U] cUIInput_setPadNum
    char unk_5[0x3];                // 0x5 padding
    void** mVtbl;                   // 0x8 [U] the destructor restores cUIInput's vtable
};

CHECK_OFFSET(cUIInput, mPadNum, 0x4);
CHECK_OFFSET(cUIInput, mVtbl, 0x8);
CHECK_SIZE(cUIInput, 0xC);

#endif
