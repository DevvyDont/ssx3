#ifndef FE_FESTATEOPTIONSSOUND_H
#define FE_FESTATEOPTIONSSOUND_H
#pragma interface

// cFEStateOptionsSound: a front-end state (a cFEState), 7 methods; allocated with cMemMan_alloc(72, "cFEStateOptionsSound").
// Size 0x48 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateOptionsSound : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsSound

    // The speaker choices: the "1lb" list's entry data and setSoundMode's argument (names ours). [U]
    // cFEStateOptionsSound_updateWidget adds kT_20SpeakerStereo 0, kT_20SpeakerSurround 1, kT_20SpeakerMono 2 and
    // kT_20SpeakerDTS 3; cSSXAudio_AudioModeToBEAudioMode maps the driver modes to the same values.
    enum { kSpeakerStereo = 0, kSpeakerSurround = 1, kSpeakerMono = 2, kSpeakerDTS = 3 };

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsSound_updateWidget");
    void setSoundMode(unsigned int mode) __asm__("cFEStateOptionsSound_setSoundMode");
    virtual ~cFEStateOptionsSound();   // g++'s own symbol _$_20cFEStateOptionsSound (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateOptionsSound_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cFEStateOptionsSound_onWidgetEvent");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cFEStateOptionsSound_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cFEStateOptionsSound_onWidgetMenu");
    virtual int onWidgetPair(cUIPair* pair, unsigned int event) __asm__("cFEStateOptionsSound_onWidgetPair");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateOptionsSound_onGainFocus__FPv");
};

CHECK_SIZE(cFEStateOptionsSound, 0x48);
__asm__("_vt$20cFEStateOptionsSound = cFEStateOptionsSound_vtbl");   // g++'s name for the retail vtable

#endif
