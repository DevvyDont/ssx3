#ifndef UI_VIRTUALKEY_H
#define UI_VIRTUALKEY_H

// tUIVirtualKey, moved out of ui/uiinput.h so C units (the USB keyboard driver in seg/seg_2F41F8) can use it too.

// The virtual key codes getVKey (slot 42) reports for a USB keyboard key (names ours; the values are the Windows VK_*
// codes). [U] cKeyboardPopup_onInputBegin moves the on-screen keyboard to the matching key: back to backspace, tab,
// return to enter, shift, caps lock, escape, the four arrows, and F1..F10 to the quick-message keys Q1..Q10;
// cMenuSys_input (util/menu) also turns insert and delete into the menu keys that insert and delete a character. The
// USB keyboard driver (seg/seg_2F41F8: kbReader_testModifierKey, kbReader_testLockKey) maps the modifier and lock keys (control, menu =
// alt, the Windows keys, num lock, scroll lock and the left / right forms) to their USB modifier and LED bits.
enum tUIVirtualKey {
    kVK_Back = 8,
    kVK_Tab = 9,
    kVK_Return = 13,
    kVK_Shift = 16,
    kVK_Control = 17,
    kVK_Menu = 18,
    kVK_Capital = 20,
    kVK_Escape = 27,
    kVK_Left = 37,
    kVK_Up = 38,
    kVK_Right = 39,
    kVK_Down = 40,
    kVK_Insert = 45,
    kVK_Delete = 46,
    kVK_LWin = 91,
    kVK_RWin = 92,
    kVK_F1 = 112,
    kVK_F2 = 113,
    kVK_F3 = 114,
    kVK_F4 = 115,
    kVK_F5 = 116,
    kVK_F6 = 117,
    kVK_F7 = 118,
    kVK_F8 = 119,
    kVK_F9 = 120,
    kVK_F10 = 121,
    kVK_NumLock = 144,
    kVK_Scroll = 145,
    kVK_LShift = 160,
    kVK_RShift = 161,
    kVK_LControl = 162,
    kVK_RControl = 163,
    kVK_LMenu = 164,
    kVK_RMenu = 165
};

#endif /* UI_VIRTUALKEY_H */
