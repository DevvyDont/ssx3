#ifndef INPUT_INPUTPAD_H
#define INPUT_INPUTPAD_H

// cInputPad: one controller's button / axis states as the game reads them (a count, then one tButton per value).
// EA's header: ../source/input/inputpad.h ([P] the preview's inline assert "btn >= 0" files under it).
// Size 0x2A4: [R] cPlaybackMan's ctor allocates cMemMan_alloc(0x2A4, "cInputPad"); [O] On Tour's cInputPad is
// {int NumButtons; tButton Buttons[24]} (0x184 with 1-byte bools; retail's bools are words, tButton 0x1C:
// 4 + 24 x 0x1C = 0x2A4). Retail code: cInputPad_update (unit input/inputmap) writes a value per entry 0x1C apart
// (Value at +0, the flag words at +4 / +8 / +0xC, a counter at +0x18).
// Users here: cPlaybackMan_isPauseAI packs / unpacks the live pad (cPlaybackMan::RealInputPad, view sPbAxes_26CE50:
// count, then 0x1C-byte entries with the value at +0).
//
// Evidence tags: [O] On Tour's DWARF; [P] the preview; [R] the allocation; [U] retail use.

#include "layout.h"

struct tButton {
    float Value;                    // 0x00 [O]; [U] cInputPad_update stores the value (swc1); isPauseAI packs it * 255
    int IsPressed;                  // 0x04 [O] bool (retail word); [U] cInputPad_update
    int IsReleased;                 // 0x08 [O] bool (retail word); [U] cInputPad_update
    int IsHeld;                     // 0x0C [O] bool (retail word); [U] cInputPad_update
    int IsRepeated;                 // 0x10 [O] bool (retail word)
    int RepeatCount;                // 0x14 [O]
    int DebounceCount;              // 0x18 [O]; [U] cInputPad_update counts it
};

CHECK_OFFSET(tButton, Value, 0x00);
CHECK_OFFSET(tButton, IsPressed, 0x04);
CHECK_OFFSET(tButton, IsReleased, 0x08);
CHECK_OFFSET(tButton, IsHeld, 0x0C);
CHECK_OFFSET(tButton, IsRepeated, 0x10);
CHECK_OFFSET(tButton, RepeatCount, 0x14);
CHECK_OFFSET(tButton, DebounceCount, 0x18);
CHECK_SIZE(tButton, 0x1C);

struct cInputPad {
    int NumButtons;                 // 0x000 [O]; [U] cInputPad_update stores its count; isPauseAI loops to it
    tButton Buttons[24];            // 0x004 [O] MaxButtons = 24; [U] cInputPad_update, isPauseAI

    // Methods bound to their retail symbols by asm labels.
    void update(int n, float* vals) __asm__("cInputPad_update");
};

CHECK_OFFSET(cInputPad, NumButtons, 0x000);
CHECK_OFFSET(cInputPad, Buttons, 0x004);
CHECK_SIZE(cInputPad, 0x2A4);

#endif /* INPUT_INPUTPAD_H */
