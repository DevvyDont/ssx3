#ifndef UI_UIINPUT_H
#define UI_UIINPUT_H
#pragma interface

// cUIInput: the UI engine's input interface (EA's UI library; cUIEngine::mpInput). The base class keeps the last key
// and the controlling pad; the game's front end derives cFEInput (fe/feinput.h), which reads the pad. Most of the
// virtuals (getKeyReady, getVKey, getAsciiKey, isStart, the camera queries ...) are stubs in the base. Methods in
// fe/fepopupmisc and main/all_in_one_main.
//
// Size 0xC (On Tour's is the same). A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP
// DWARF ([O]); [U] how retail code uses the field. The vtable pointer is g++ 2.95's, after the members.

#include "layout.h"

#include "ui/virtualkey.h"         // tUIVirtualKey

struct cUIInput {
    int mKey;                       // 0x0 [O]; [U] cUIInput_setKey
    unsigned char mPadNum;          // 0x4 [O]; [U] cUIInput_setPadNum
    char unk_5[0x3];                // 0x5 padding
    // 0x8: the vptr. [U] the destructor restores cUIInput's vtable. Slots: [R] cUIInput_vtbl's 46 entries; [O] On Tour's
    //   virtuals, same names and order (setPadNum takes an int in retail: cUIInput_setPadNum__FPvi).
    // 1; inline and empty: cFEInput's destructor inlines it (retail's out-of-line copy cUIInput__cUIInput stays a
    // C function)
    virtual ~cUIInput() {}
    virtual bool getState(char state) const __asm__("cUIInput_getState__FPv");                    // 2
    virtual bool isStart() const __asm__("cUIInput_isStart__FPv");                               // 3
    virtual bool isSelect() const __asm__("cUIInput_isSelect__FPv");                              // 4
    virtual bool isNext() const __asm__("cUIInput_isNext__FPv");                                // 5
    virtual bool isPrevious() const __asm__("cUIInput_isPrevious__FPv");                            // 6
    virtual bool isOption() const __asm__("cUIInput_isOption__FPv");                              // 7
    virtual bool isMisc() const __asm__("cUIInput_isMisc__FPv");                                // 8
    virtual bool isNextHeld() const __asm__("cUIInput_isNextHeld__FPv");                            // 9
    virtual bool isPreviousHeld() const __asm__("cUIInput_isPreviousHeld__FPv");                        // 10
    virtual bool isOptionHeld() const __asm__("cUIInput_isOptionHeld__FPv");                          // 11
    virtual bool isMiscHeld() const __asm__("cUIInput_isMiscHeld__FPv");                            // 12
    virtual bool isNextReleased() const __asm__("cUIInput_isNextReleased__FPv");                        // 13
    virtual bool isPreviousReleased() const __asm__("cUIInput_isPreviousReleased__FPv");                    // 14
    virtual bool isOptionReleased() const __asm__("cUIInput_isOptionReleased__FPv");                      // 15
    virtual bool isMiscReleased() const __asm__("cUIInput_isMiscReleased__FPv");                        // 16
    virtual bool isUp() const __asm__("cUIInput_isUp__FPv");                                  // 17
    virtual bool isDown() const __asm__("cUIInput_isDown__FPv");                                // 18
    virtual bool isLeft() const __asm__("cUIInput_isLeft__FPv");                                // 19
    virtual bool isRight() const __asm__("cUIInput_isRight__FPv");                               // 20
    virtual bool isUpHeld() const __asm__("cUIInput_isUpHeld__FPv");                              // 21
    virtual bool isDownHeld() const __asm__("cUIInput_isDownHeld__FPv");                            // 22
    virtual bool isLeftHeld() const __asm__("cUIInput_isLeftHeld__FPv");                            // 23
    virtual bool isRightHeld() const __asm__("cUIInput_isRightHeld__FPv");                           // 24
    virtual bool isRShift1() const __asm__("cUIInput_isRShift1__FPv");                             // 25
    virtual bool isRShift2() const __asm__("cUIInput_isRShift2__FPv");                             // 26
    virtual bool isLShift1() const __asm__("cUIInput_isLShift1__FPv");                             // 27
    virtual bool isLShift2() const __asm__("cUIInput_isLShift2__FPv");                             // 28
    virtual bool isRShift1Held() const __asm__("cUIInput_isRShift1Held__FPv");                         // 29
    virtual bool isRShift2Held() const __asm__("cUIInput_isRShift2Held__FPv");                         // 30
    virtual bool isLShift1Held() const __asm__("cUIInput_isLShift1Held__FPv");                         // 31
    virtual bool isLShift2Held() const __asm__("cUIInput_isLShift2Held__FPv");                         // 32
    virtual bool isCamZoom() const __asm__("cUIInput_isCamZoom__FPv");                             // 33
    virtual bool isCamPan() const __asm__("cUIInput_isCamPan__FPv");                              // 34
    virtual bool isCamRotate() const __asm__("cUIInput_isCamRotate__FPv");                           // 35
    virtual float getCamZoom() const __asm__("cUIInput_getCamZoom__FPv");                           // 36
    virtual float getCamPan() const __asm__("cUIInput_getCamPan__FPv");                            // 37
    virtual float getCamRotate() const __asm__("cUIInput_getCamRotate__FPv");                         // 38
    virtual void setKey(int key);                               // 39
    virtual bool keyPressed() const __asm__("cUIInput_keyPressed__FPv");                            // 40
    virtual bool getKeyReady() const __asm__("cUIInput_getKeyReady__FPv");                           // 41
    virtual int getVKey() const __asm__("cUIInput_getVKey__FPv");                                // 42
    virtual int getAsciiKey() const __asm__("cUIInput_getAsciiKey__FPv");                            // 43
    virtual void setPadNum(int num) __asm__("cUIInput_setPadNum__FPvi");                            // 44
    virtual bool isActive() __asm__("cUIInput_isActive__FPv");                                    // 45
    virtual void update() __asm__("cUIInput_update__FPv");                                      // 46
};

CHECK_OFFSET(cUIInput, mPadNum, 0x4);
CHECK_SIZE(cUIInput, 0xC);
__asm__("_vt$8cUIInput = cUIInput_vtbl");   // g++'s name for the retail vtable

#endif
