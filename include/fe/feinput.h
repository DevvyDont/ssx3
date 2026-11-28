#ifndef FE_FEINPUT_H
#define FE_FEINPUT_H
#pragma interface

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
#include "bx/memman.h"

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
    cInput* getInput() __asm__("cFEInput_getInput");
    virtual ~cFEInput();   // g++'s own symbol _$_8cFEInput (the retail label renamed)
    virtual bool isActive() __asm__("cFEInput_isActive");
    virtual int getAsciiKey() const __asm__("cFEInput_getAsciiKey");
    virtual bool keyPressed() const __asm__("cFEInput_keyPressed");
    virtual void update() __asm__("cFEInput_update");
    virtual int getVKey() const __asm__("cFEInput_getVKey");
    virtual bool isStart() const __asm__("cFEInput_isStart__FPv");
    virtual bool isSelect() const __asm__("cFEInput_isSelect__FPv");
    virtual bool isNext() const __asm__("cFEInput_isNext__FPv");
    virtual bool isPrevious() const __asm__("cFEInput_isPrevious__FPv");
    virtual bool isOption() const __asm__("cFEInput_isOption__FPv");
    virtual bool isMisc() const __asm__("cFEInput_isMisc__FPv");
    virtual bool isNextHeld() const __asm__("cFEInput_isNextHeld__FPv");
    virtual bool isPreviousHeld() const __asm__("cFEInput_isPreviousHeld__FPv");
    virtual bool isOptionHeld() const __asm__("cFEInput_isOptionHeld__FPv");
    virtual bool isMiscHeld() const __asm__("cFEInput_isMiscHeld__FPv");
    virtual bool isNextReleased() const __asm__("cFEInput_isNextReleased__FPv");
    virtual bool isPreviousReleased() const __asm__("cFEInput_isPreviousReleased__FPv");
    virtual bool isOptionReleased() const __asm__("cFEInput_isOptionReleased__FPv");
    virtual bool isMiscReleased() const __asm__("cFEInput_isMiscReleased__FPv");
    virtual bool isUp() const __asm__("cFEInput_isUp__FPv");
    virtual bool isDown() const __asm__("cFEInput_isDown__FPv");
    virtual bool isLeft() const __asm__("cFEInput_isLeft__FPv");
    virtual bool isRight() const __asm__("cFEInput_isRight__FPv");
    virtual bool isUpHeld() const __asm__("cFEInput_isUpHeld__FPv");
    virtual bool isDownHeld() const __asm__("cFEInput_isDownHeld__FPv");
    virtual bool isLeftHeld() const __asm__("cFEInput_isLeftHeld__FPv");
    virtual bool isRightHeld() const __asm__("cFEInput_isRightHeld__FPv");
    virtual bool isRShift1() const __asm__("cFEInput_isRShift1__FPv");
    virtual bool isRShift2() const __asm__("cFEInput_isRShift2__FPv");
    virtual bool isLShift1() const __asm__("cFEInput_isLShift1__FPv");
    virtual bool isLShift2() const __asm__("cFEInput_isLShift2__FPv");
    virtual bool isRShift1Held() const __asm__("cFEInput_isRShift1Held__FPv");
    virtual bool isRShift2Held() const __asm__("cFEInput_isRShift2Held__FPv");
    virtual bool isLShift1Held() const __asm__("cFEInput_isLShift1Held__FPv");
    virtual bool isLShift2Held() const __asm__("cFEInput_isLShift2Held__FPv");
    virtual bool isCamZoom() const __asm__("cFEInput_isCamZoom__FPv");
    virtual bool isCamPan() const __asm__("cFEInput_isCamPan__FPv");
    virtual bool isCamRotate() const __asm__("cFEInput_isCamRotate__FPv");
    virtual float getCamZoom() const __asm__("cFEInput_getCamZoom__FPv");
    virtual float getCamPan() const __asm__("cFEInput_getCamPan__FPv");
    virtual float getCamRotate() const __asm__("cFEInput_getCamRotate__FPv");
    virtual void setPadNum(int idx) __asm__("cFEInput_setPadNum");
};

CHECK_OFFSET(cFEInput, mbKeyReady, 0x0C);
CHECK_OFFSET(cFEInput, mpInput, 0x18);
CHECK_SIZE(cFEInput, 0x1C);
__asm__("_vt$8cFEInput = cFEInput_vtbl");   // g++'s name for the retail vtable

#endif
