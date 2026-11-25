#ifndef FE_FESTATEOPTIONSSOUND_H
#define FE_FESTATEOPTIONSSOUND_H

// cFEStateOptionsSound: a front-end state (a cUIState), 7 methods; allocated with cMemMan_alloc(72, "cFEStateOptionsSound").
// Size 0x48 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateOptionsSound : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsSound

    // The speaker choices: the "1lb" list's entry data and setSoundMode's argument (names ours). [U]
    // cFEStateOptionsSound_updateWidget adds kT_20SpeakerStereo 0, kT_20SpeakerSurround 1, kT_20SpeakerMono 2 and
    // kT_20SpeakerDTS 3; cSSXAudio_AudioModeToBEAudioMode maps the driver modes to the same values.
    enum { kSpeakerStereo = 0, kSpeakerSurround = 1, kSpeakerMono = 2, kSpeakerDTS = 3 };

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsSound_updateWidget");
    void setSoundMode(unsigned int mode) __asm__("cFEStateOptionsSound_setSoundMode");
};

CHECK_SIZE(cFEStateOptionsSound, 0x48);

#endif
