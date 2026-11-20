#ifndef AI_MISSIONRIDER_H
#define AI_MISSIONRIDER_H

// cMissionRider: the scripted "mission" rider of a big challenge / mission event (at most one: cAI::MissionRiders[1],
// ai/ai.h +0x70). It is a cPlayer driven by the playback input (cAI_initMissionRiders sets Input = PlaybackMan +0x6C,
// Pad = 0) and adds no data of its own.
// EA's source file is not known (no preview assert names it; On Tour has it in ai/all_in_one_ai.cpp); the header path
// follows the units that use it (ai/ai.cpp builds it, ai/control/handplantcontrol.cpp holds its destructor).
// Size 0x1950 [R]: cAI_initMissionRiders allocates it with cMemMan_alloc(6480, "MissionRider", 0x80000000, 0) and runs
// its (inlined) constructor: the 31 virtual-base pointers = this + 0xE10, cRider_cRider(this + 0xE10), cPlayer_cPlayer(this, 0),
// then the cRider base's vtables _vt$13cMissionRider$6cRider (D_00458288, at Rider + 0x6C0), D_00459B90 (+0x6D0) and
// _vt$13cMissionRider$13cScriptObject (D_00458240, +0x6E8), and cPlayer's own vtable cPlayer_vtbl at 0xDE8 (cMissionRider
// overrides none of cPlayer's three virtuals). Destructor cMissionRider__cMissionRider (0x1446F8).
// Bases (On Tour: class cMissionRider : cPlayer; cPlayer : cRiderHandler with cRider a virtual base): the layout is
// cPlayer's (ai/player.h), written out inline below with the same names and evidence.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] the inlined ctor in cAI_initMissionRiders and the dtor cMissionRider__cMissionRider (lw 0x18 seven times,
//       sw cPlayer_vtbl at 0xDE8), the allocation size;
//   [P] the preview's paired dtor via tools/layout_xlate.py (preview 0x10 -> retail 0x18 for the first virtual-base
//       pointer, 0xDA8 -> 0xDE8 for the vptr: the same +0x40 shift ai/player.h measured for cPlayer);
//   [U] how retail code uses the field; [O] SSX On Tour's DWARF (cPlayer, cRiderHandler).

#include "layout.h"
#include "ai/rider.h"

struct cInput;
struct cPadDevice;
struct cReplayCache;

struct cMissionRider {
    // ---- base cPlayer (ai/player.h) ----
    // ---- its base cRiderHandler (0x000-0xDF0) ----
    char unk_000[0x18];        // 0x000 the first part's own data
    cRider* mRider;            // 0x018 [R] the first part's virtual-base pointer (= &Rider, set by the inlined ctor along
                               //       with 30 others); [U] cMissionRider__cMissionRider, cAI_initMissionRiders and ai/ai.cpp
                               //       (Rider_AF0) reach the cRider through it
    char unk_01C[0xDC4];       // 0x01C the other rider parts of cRiderHandler (virtual-base pointers at 0x024 ... 0xD20,
                               //       see ai/player.h)
    int MotionState;           // 0xDE0 [P] cRiderHandler::_getMotionState (preview 0xDA0); [O]
    int ControlState;          // 0xDE4 [P] _getControlState (preview 0xDA4); [O]
    void** mVtbl;              // 0xDE8 [R] cPlayer_vtbl (D_00458338), stored by the inlined ctor and by the dtor; [P] preview 0xDA8
    char pad_DEC[0x4];         // 0xDEC cRiderHandler is 16-byte aligned
    // ---- cPlayer's own fields ----
    cInput* Input;             // 0xDF0 [R] cAI_initMissionRiders stores PlaybackMan +0x6C; [O]
    cPadDevice* Pad;           // 0xDF4 [R] cAI_initMissionRiders stores 0; [O]
    cReplayCache* ReplayCache; // 0xDF8 [R] cPlayer_cPlayer; [O]
    float unk_DFC;             // 0xDFC [R] cPlayer_cPlayer = 0 (vibration, see ai/player.h)
    float unk_E00;             // 0xE00 [R] cPlayer_cPlayer = 0 (vibration, see ai/player.h)
    char pad_E04[0xC];         // 0xE04 the cRider virtual base is 16-byte aligned
    // ---- virtual base cRider (0xE10-0x1950) ----
    cRider Rider;              // 0xE10 [R] the inlined ctor: cRider_cRider(this + 0xE10), then the cMissionRider vtables at
                               //       Rider +0x6C0 / +0x6D0 / +0x6E8
};

CHECK_OFFSET(cMissionRider, unk_000, 0x000);
CHECK_OFFSET(cMissionRider, mRider, 0x018);
CHECK_OFFSET(cMissionRider, unk_01C, 0x01C);
CHECK_OFFSET(cMissionRider, MotionState, 0xDE0);
CHECK_OFFSET(cMissionRider, ControlState, 0xDE4);
CHECK_OFFSET(cMissionRider, mVtbl, 0xDE8);
CHECK_OFFSET(cMissionRider, pad_DEC, 0xDEC);
CHECK_OFFSET(cMissionRider, Input, 0xDF0);
CHECK_OFFSET(cMissionRider, Pad, 0xDF4);
CHECK_OFFSET(cMissionRider, ReplayCache, 0xDF8);
CHECK_OFFSET(cMissionRider, unk_DFC, 0xDFC);
CHECK_OFFSET(cMissionRider, unk_E00, 0xE00);
CHECK_OFFSET(cMissionRider, pad_E04, 0xE04);
CHECK_OFFSET(cMissionRider, Rider, 0xE10);
CHECK_SIZE(cMissionRider, 0x1950);

#endif /* AI_MISSIONRIDER_H */
