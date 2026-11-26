#ifndef INPUT_INPUT_H
#define INPUT_INPUT_H

// cInput: a player's input: a pad (cInputPad, the raw controller state) and the input map (cInputMap, input/inputmap.h)
// whose compiled statements turn the pad into game input states. cInput_getState / cInput_getValue evaluate one state
// through the map on the pad; cInput_setPad / cInput_setMap install the two. The front end (fe/feinput.h mpInput), the
// menu system (util/menusys.h Input) and the replay player (cPlaybackMan) each hold one. Methods in bx/execman.
//
// Size 0x8 [R] (cPlaybackMan's cMemMan_alloc(8, "cInput"), next to its 676-byte "cInputPad"; On Tour's is the same).
// Member names are EA's, from SSX On Tour's PSP DWARF ([O]); every retail access through `this` falls on a member
// start (tools/gen/self_offsets.py). [U] how retail code uses the field.

#include "layout.h"

struct cInputPad;
struct cInputMap;

struct cInput {
    cInputPad* Pad;                 // 0x00 [O]; [U] passed as the pad to cInputMap_getState / getValue
    cInputMap* Map;                 // 0x04 [O]; [U] the map whose statements getState / getValue evaluate

    // Methods bound to their retail symbols by asm labels.
    void* getValue(int state) __asm__("cInput_getValue");
    void getValue1(int state, int modifier) __asm__("cInput_getValue1");
    void getState1(int state, int modifier) __asm__("cInput_getState1");
    void setPad(int val) __asm__("cInput_setPad__FPvi");
    void setMap(int val) __asm__("cInput_setMap__FPvi");
    void* getState(int state) __asm__("cInput_getState__FPvi");
};

CHECK_OFFSET(cInput, Pad, 0x00);
CHECK_OFFSET(cInput, Map, 0x04);
CHECK_SIZE(cInput, 0x8);

#endif /* INPUT_INPUT_H */
