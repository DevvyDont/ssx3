#ifndef ONLINE_USBKEYBOARD_H
#define ONLINE_USBKEYBOARD_H

// cUsbKeyboard: the USB keyboard reader for the online front end (chat, account forms). It starts Sony's USB keyboard
// library, and once per frame (cUsbKeyboard_update, from cSSXApp_preUpdate) copies each connected keyboard's new key codes
// into a 64-entry ring per keyboard; the front end's input (cFEInput_update, cMenuSys_input) takes them one pair at a
// time (cUsbKeyboard_readKey).
// EA's source: d:/ssx/code/source/online/usbkeyboard.cpp and ../source/online/usbkeyboard.h (the July 2003 preview's
// assert strings: "!Keyboard", "(device >= 0) && (device < kMaxKeyboard)", "device >= 0", and the allocation tag
// "cUsbKeyboard"). Retail code: dirtysock/tagsunk 0x00266788..0x00266BA8. One instance, Keyboard: cUsbKeyboard_createInstance
// creates it, cUsbKeyboard_destroyInstance destroys it. No On Tour counterpart.
//
// Size 0x464: [R] cUsbKeyboard_createInstance: cMemMan_alloc(1124, "cUsbKeyboard", 0x20000000) + the constructor cUsbKeyboard_cUsbKeyboard, which
// sets mEnabled (0x460) and MEM_fill's the 0x460 bytes of slots. No vtable, no base class (the destructor cUsbKeyboard__cUsbKeyboard
// is called directly).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the constructor / destructor; [U] how retail code uses the field
// (src/dirtysock/tagsunk.cpp views sSlots_668E8 / sSlot_668E8, sEntTable / sEnt8C, sChanSet_266BA8 / sChan_266BA8).
// Names are ours (no EA member names survive); the slot count 8 is the preview's kMaxKeyboard (asserted range).

#include "layout.h"

// One keyboard's key ring (0x8C bytes).
struct tUsbKeyboardSlot {
    int mCount;                     // 0x00 [U] keys written so far: cUsbKeyboard_update stores at mKeys[mCount % 64] and
                                    //      increments; cUsbKeyboard_clearKeys clears it
    int mRead;                      // 0x04 [U] keys taken: cUsbKeyboard_readKey reads mKeys[mRead % 64] while mRead < mCount;
                                    //      cUsbKeyboard_update drops the oldest (mRead + 1) once 64 are waiting
    unsigned char mKeys[64][2];     // 0x08 [U] the translated key pair (kbReader_translateKey's two result bytes)
    unsigned int mFlags;            // 0x88 [U] bit 0 a keyboard is connected (kbReader_isConnected), bits 1..3 bit 0 of the
                                    //      read block's first word, bit 4 kbReader_isKeyDown(.., 0x10); the whole word is
                                    //      cleared when no keyboard is present (the C view uses bit-fields)
};

CHECK_OFFSET(tUsbKeyboardSlot, mCount, 0x00);
CHECK_OFFSET(tUsbKeyboardSlot, mRead, 0x04);
CHECK_OFFSET(tUsbKeyboardSlot, mKeys, 0x08);
CHECK_OFFSET(tUsbKeyboardSlot, mFlags, 0x88);
CHECK_SIZE(tUsbKeyboardSlot, 0x8C);

struct cUsbKeyboard {
    tUsbKeyboardSlot mSlots[8];     // 0x000 [R] ctor MEM_fill 0x460 bytes; [U] indexed by keyboard number (0..7)
    int mEnabled;                   // 0x460 [R] ctor: 1 when the library started (kbReader_init >= 0), else 0;
                                    //       dtor shuts the library down and clears it; [U] every method tests it

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cUsbKeyboard_update");
};

CHECK_OFFSET(cUsbKeyboard, mSlots, 0x000);
CHECK_OFFSET(cUsbKeyboard, mEnabled, 0x460);
CHECK_SIZE(cUsbKeyboard, 0x464);

#endif /* ONLINE_USBKEYBOARD_H */
