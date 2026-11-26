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

// The virtual key codes getVKey (slot 42) reports for a USB keyboard key (names ours; the values are the Windows VK_*
// codes). [U] cKeyboardPopup_onInputBegin moves the on-screen keyboard to the matching key: back to backspace, tab,
// return to enter, shift, caps lock, escape, the four arrows, and F1..F10 to the quick-message keys Q1..Q10;
// cMenuSys_input (util/menu) also turns insert and delete into the menu keys that insert and delete a character.
enum tUIVirtualKey {
    kVK_Back = 8,
    kVK_Tab = 9,
    kVK_Return = 13,
    kVK_Shift = 16,
    kVK_Capital = 20,
    kVK_Escape = 27,
    kVK_Left = 37,
    kVK_Up = 38,
    kVK_Right = 39,
    kVK_Down = 40,
    kVK_Insert = 45,
    kVK_Delete = 46,
    kVK_F1 = 112,
    kVK_F2 = 113,
    kVK_F3 = 114,
    kVK_F4 = 115,
    kVK_F5 = 116,
    kVK_F6 = 117,
    kVK_F7 = 118,
    kVK_F8 = 119,
    kVK_F9 = 120,
    kVK_F10 = 121
};

struct cUIInput {
    int mKey;                       // 0x0 [O]; [U] cUIInput_setKey
    unsigned char mPadNum;          // 0x4 [O]; [U] cUIInput_setPadNum
    char unk_5[0x3];                // 0x5 padding
    // 0x8: the vptr. [U] the destructor restores cUIInput's vtable. Slots: [R] cUIInput_vtbl's 46 entries; [O] On Tour's
    //   virtuals, same names and order (setPadNum takes an int in retail: cUIInput_setPadNum__FPvi).
    virtual ~cUIInput();                                        // 1
    virtual bool getState(char state) const;                    // 2
    virtual bool isStart() const;                               // 3
    virtual bool isSelect() const;                              // 4
    virtual bool isNext() const;                                // 5
    virtual bool isPrevious() const;                            // 6
    virtual bool isOption() const;                              // 7
    virtual bool isMisc() const;                                // 8
    virtual bool isNextHeld() const;                            // 9
    virtual bool isPreviousHeld() const;                        // 10
    virtual bool isOptionHeld() const;                          // 11
    virtual bool isMiscHeld() const;                            // 12
    virtual bool isNextReleased() const;                        // 13
    virtual bool isPreviousReleased() const;                    // 14
    virtual bool isOptionReleased() const;                      // 15
    virtual bool isMiscReleased() const;                        // 16
    virtual bool isUp() const;                                  // 17
    virtual bool isDown() const;                                // 18
    virtual bool isLeft() const;                                // 19
    virtual bool isRight() const;                               // 20
    virtual bool isUpHeld() const;                              // 21
    virtual bool isDownHeld() const;                            // 22
    virtual bool isLeftHeld() const;                            // 23
    virtual bool isRightHeld() const;                           // 24
    virtual bool isRShift1() const;                             // 25
    virtual bool isRShift2() const;                             // 26
    virtual bool isLShift1() const;                             // 27
    virtual bool isLShift2() const;                             // 28
    virtual bool isRShift1Held() const;                         // 29
    virtual bool isRShift2Held() const;                         // 30
    virtual bool isLShift1Held() const;                         // 31
    virtual bool isLShift2Held() const;                         // 32
    virtual bool isCamZoom() const;                             // 33
    virtual bool isCamPan() const;                              // 34
    virtual bool isCamRotate() const;                           // 35
    virtual float getCamZoom() const;                           // 36
    virtual float getCamPan() const;                            // 37
    virtual float getCamRotate() const;                         // 38
    virtual void setKey(int key);                               // 39
    virtual bool keyPressed() const;                            // 40
    virtual bool getKeyReady() const;                           // 41
    virtual int getVKey() const;                                // 42
    virtual int getAsciiKey() const;                            // 43
    virtual void setPadNum(int num);                            // 44
    virtual bool isActive();                                    // 45
    virtual void update();                                      // 46
};

CHECK_OFFSET(cUIInput, mPadNum, 0x4);
CHECK_SIZE(cUIInput, 0xC);

#endif
