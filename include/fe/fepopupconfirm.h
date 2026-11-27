#ifndef FE_FEPOPUPCONFIRM_H
#define FE_FEPOPUPCONFIRM_H

// cFEPopupConfirm: the front end's yes/no confirmation popup (a cScreenPopup with its own copies of the message and
// button texts): message plus 0, 1 (yes), 2 (yes/no) or 3 buttons. Created by the memory-card code (fe/fememcardcallbacks,
// fe/fememcard2), the options (fe/festateoptions) and the lodge (fe/festatelodge). No EA source path in the preview's
// asserts (On Tour: fe/all_in_one_fe.cpp); retail code: cFEPopupConfirm_cFEPopupConfirm ...
// cFEPopupConfirm_setDefaultButton in the fe/fememcardcallbacks unit (0x001D99C8 ... 0x001D9D18) and the default
// ctor, the dtor and setUsePreFE in main/all_in_one_main (0x001DDE78 ... 0x001DDEE0). The header path follows the
// unit's directory and the class name.
//
// Size 0xBFC: [R] cMemMan_alloc(0xBFC, "cFEPopupConfirm", 0x100) + cFEPopupConfirm_cFEPopupConfirm
// (src/fe/fememcardcallbacks.cpp, src/fe/festatelodge.cpp); the last member, mNumButtons, is the word at 0xBF8.
// [O] On Tour's class is 0xC00 with the same members at the same offsets (its tail padding differs).
// Base: cScreenPopup (fe/screenpopup.h, 0x360) : cUIState (0x48) : cListNode (vptr at 0x8). The ctor runs
// cScreenPopup_cScreenPopup(this, pEngine, pParentState), stores cFEPopupConfirm_vtbl (0x0046C890, 28 slots: 1 dtor,
// 25 setDefaultButton, 26 setButtonText, 27 setMessage, 28 initPopup, the rest cScreenPopup's / cUIState's), picks the
// background (info.meBackgroundType 1, or 2 with info.miLayerOffset 4 while the static cFEPopupConfirm_mUsePreFE is
// set) and runs initPopup; the dtor stores cFEPopupConfirm_vtbl and runs cScreenPopup__cScreenPopup.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The base's fields are copied from
// fe/screenpopup.h (first line of each comment). Evidence tags: [R] retail ctors / dtor and the
// allocation size; [P] the July 2003 preview's inline accessors (base fields); [U] how retail code uses the field
// (src/fe/fememcardcallbacks.cpp and the asm); [O] SSX On Tour's DWARF (class cFEPopupConfirm, cScreenPopup).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/screenpopup.h"

struct cFEPopupConfirm : public cScreenPopup {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEPopupConfirm_vtbl (both ctors, the dtor) over cScreenPopup_vtbl
    // cFEPopupConfirm's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName
    //   miMiscData: [P] cUIState::getMiscData lw/sw (ui/uistate.h); [U] cScreenPopup_initScreenPopup copies the popup
    //       info's miMiscData
    //   mpParentState: [U] receives onPopupEvent (slot 17)
    //   mpScreen: [R] ctor zeroes it; [U] cScreenPopup_onCreateScreen stores the "popup" screen
    // cScreenPopup (fe/screenpopup.h, 0x360)
    // cFEPopupConfirm's notes on cScreenPopup's fields:
    //   info: [O]; [R] cPopupInfo_cPopupInfo(self + 0xBC); [U] initPopup / setMessage / setDefaultButton write its
    //       meType, mtMessage, miLayerOffset, mbPassUpdate, mbReceivePrev, miDefaultOption; the ctor its
    //       meBackgroundType
    // cFEPopupConfirm's own slots ([R] cFEPopupConfirm_vtbl after cScreenPopup's 24; types [O] On Tour's class cFEPopupConfirm,
    // which derives from cScreenPopup):
    virtual void setDefaultButton(unsigned int uiButton);                          // 25
    virtual void setButtonText(unsigned short* ustrText, unsigned int uiButton)
        __asm__("cFEPopupConfirm_setButtonText");    // 26
    virtual void setMessage(unsigned short* ustrText)
        __asm__("cFEPopupConfirm_setMessage");                             // 27
    virtual void initPopup(unsigned short* ustrDisplay, int bYesOnly, int bNoButtons, int layerOffset, int passUpdate,
                           int bSnowFlake) __asm__("cFEPopupConfirm_initPopup"); // 28 (On Tour: the flags are bool)
    // --- cFEPopupConfirm
    unsigned short mDisplayText[800]; // 0x360 [O]; [U] initPopup / setMessage copy the 16-bit message here and point
                                    //       info.mtMessage's value (+0xC4) at it
    unsigned short mYesText[100];   // 0x9A0 [O]; [U] setButtonText(text, 0) copies the first button's label here
    unsigned short mNoText[100];    // 0xA68 [O]; [U] setButtonText(text, 1), when mNumButtons >= 2
    unsigned short mAltText[100];   // 0xB30 [O]; [U] setButtonText(text, other): makes it a 3-button popup
    int mNumButtons;                // 0xBF8 [O]; [U] initPopup stores 0 (no buttons), 1 (yes only) or 2;
                                    //       setButtonText stores 3 for a third button and tests >= 2

    // Methods bound to their retail symbols by asm labels.
    void * cFEPopupConfirm1() __asm__("cFEPopupConfirm_cFEPopupConfirm1");
};

CHECK_OFFSET(cFEPopupConfirm, mDisplayText, 0x360);
CHECK_OFFSET(cFEPopupConfirm, mYesText, 0x9A0);
CHECK_OFFSET(cFEPopupConfirm, mNoText, 0xA68);
CHECK_OFFSET(cFEPopupConfirm, mAltText, 0xB30);
CHECK_OFFSET(cFEPopupConfirm, mNumButtons, 0xBF8);
CHECK_SIZE(cFEPopupConfirm, 0xBFC);

#endif /* FE_FEPOPUPCONFIRM_H */
