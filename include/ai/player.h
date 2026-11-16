#ifndef AI_PLAYER_H
#define AI_PLAYER_H

// cPlayer: a human-controlled rider (one per local or remote human; cAI::Players[2]). Allocated by cAI_InitPlayers
// as `cPlayer_cPlayer(cMemMan_alloc(0x1950, "Player", 0x80000000, 0), 1)`; the mission rider (cMissionRider,
// "MissionRider", also 0x1950) is a cPlayer too (cAI_initMissionRiders calls cPlayer_cPlayer(self, 0)).
// EA's source is d:/ssx/code/source/ai/player.cpp (preview assert path); the header name is the unit's.
//
// Size 0x1950: [R] the allocation above; = 0xE10 + sizeof(cRider) 0xB40.
// Bases (On Tour: class cPlayer : cRiderHandler, with cRider a *virtual* base):
//   0x000-0xDF0  cRiderHandler: 31 rider "parts" (motions, controls and FX: cAirMotion, cWipeoutMotion, ...,
//                cCruiseControl, ..., cBoardWakeFX, ...), each deriving virtually from cRider, so each holds its own
//                g++ 2.95 virtual-base pointer to the cRider below. The parts are not laid out here (raw bytes);
//                ai/computer_classes.h models the composite. cRiderHandler's own fields are MotionState,
//                ControlState and its vptr, at its end. Shared with cComputer (ai/computer.h): same offsets.
//   0xDF0-0xE10  cPlayer's own fields.
//   0xE10-0x1950 cRider, the virtual base (ai/rider.h); cRider's vtables there are _vt$7cPlayer$6cRider
//                (D_004583A8, deltas -0xE10/-0x14D0) and _vt$7cPlayer$13cScriptObject (D_00458360). cPlayer's
//                overrides in the cRider vtable include slots 16-19 vibrationReset/Bump/Rumble/Update (func_001278C0,
//                func_001278D0, func_001278E0, func_001278E8; names from builds/x/v2/name_map_v2.tsv, preview slots).
//
// Member names: [O] SSX On Tour's PSP DWARF (cPlayer: Input, Pad, ReplayCache; cRiderHandler: MotionState,
// ControlState), placed by [P] the July 2003 preview's inline accessors (getInput lw 0xDB0, setInput sw 0xDB0/0xDB4,
// cRiderHandler::_getMotionState lw 0xDA0, _getControlState lw 0xDA4), which retail moved by +0x40 (tools/
// layout_xlate.py: 62 aligned observations 0xDB0 -> 0xDF0), and by [R] retail's constructor cPlayer_cPlayer and the
// out-of-line accessors in ai/control/handplantcontrol (cPlayer_setInput setInput, func_001446E0 getInput,
// cRiderHandler__getMotionState _getMotionState, cRiderHandler__getControlState _getControlState); [U] how retail code uses the field.
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "ai/rider.h"

struct cInput;
struct cPadDevice;
struct cReplayCache;

struct cPlayer {
    // ---- base cRiderHandler (0x000-0xDF0) ----
    char unk_000[0x18];        // 0x000 the first part's own data
    cRider* mRider;            // 0x018 [R] the first part's virtual-base pointer: cPlayer_cPlayer stores &this->Rider
                               //       here (and in 30 other part pointers, 0x24 ... 0xD20); [U] every user reads
                               //       the player's cRider through it (e.g. cAI_InitPlayers, func_001278E8)
    char unk_01C[0xDC4];       // 0x01C the other parts of cRiderHandler, with their virtual-base pointers at 0x024,
                               //       0x070, 0x0A0, 0x100, 0x1B0, 0x1C0, 0x1E4, 0x1F0, 0x200, 0x224, 0x288, 0x2A4,
                               //       0x2B8, 0x340, 0x358, 0x364, 0x384, 0x398, 0x3A0, 0x3B0, 0x470, 0x520, 0x610,
                               //       0x9C0, 0xAD0, 0xAF0, 0xB00, 0xB40, 0xC70, 0xD20 (cPlayer_cPlayer)
    int MotionState;           // 0xDE0 [P] cRiderHandler::_getMotionState (preview 0xDA0); [R] cRiderHandler__getMotionState; [O]
    int ControlState;          // 0xDE4 [P] _getControlState (preview 0xDA4); [R] cRiderHandler__getControlState; [O]
    void** mVtbl;              // 0xDE8 [R] _vt$7cPlayer (D_00458338, 3 slots: cPlayer_getControlCmd, cPlayer_writeToReplayFrame)
    char pad_DEC[0x4];         // 0xDEC cRiderHandler is 16-byte aligned (the parts hold t4Vectors)
    // ---- cPlayer ----
    cInput* Input;             // 0xDF0 [P] getInput/setInput; [R] func_001446E0, cPlayer_setInput; [O]
    cPadDevice* Pad;           // 0xDF4 [P] setInput's second argument; [R] ctor = 0, cPlayer_setInput; [O];
                               //       [U] vibrationUpdate (func_001278E8) rumbles it
    cReplayCache* ReplayCache; // 0xDF8 [R] ctor = cReplay_addCache(Game->Replay), released by ~cPlayer; [O]
    float unk_DFC;             // 0xDFC [R] ctor = 0; [U] vibrationReset = 0, vibrationBump adds to it; decayed with
                               //       0xE00 by func_00125B18(&unk_DFC, Pad, on) (a pair of floats)
    float unk_E00;             // 0xE00 [R] ctor = 0; [U] vibrationReset = 0, vibrationRumble(float) stores it
    char pad_E04[0xC];         // 0xE04 the cRider virtual base is 16-byte aligned
    // ---- virtual base cRider (0xE10-0x1950) ----
    cRider Rider;              // 0xE10 [R] cPlayer_cPlayer: cRider_cRider(this + 0xE10) when in charge
};

// Virtual slots of _vt$7cPlayer (D_00458338); names from the preview vtable where known.
enum {
    kPlayerVt_getControlCmd = 1,    // cPlayer_getControlCmd (reads Input; [O] cPlayer::getControlCmd)
    kPlayerVt_slot2 = 2             // cPlayer_writeToReplayFrame (tail-calls cRiderHandler_writeToReplayFrame)
};

CHECK_OFFSET(cPlayer, unk_000, 0x000);
CHECK_OFFSET(cPlayer, mRider, 0x018);
CHECK_OFFSET(cPlayer, unk_01C, 0x01C);
CHECK_OFFSET(cPlayer, MotionState, 0xDE0);
CHECK_OFFSET(cPlayer, ControlState, 0xDE4);
CHECK_OFFSET(cPlayer, mVtbl, 0xDE8);
CHECK_OFFSET(cPlayer, pad_DEC, 0xDEC);
CHECK_OFFSET(cPlayer, Input, 0xDF0);
CHECK_OFFSET(cPlayer, Pad, 0xDF4);
CHECK_OFFSET(cPlayer, ReplayCache, 0xDF8);
CHECK_OFFSET(cPlayer, unk_DFC, 0xDFC);
CHECK_OFFSET(cPlayer, unk_E00, 0xE00);
CHECK_OFFSET(cPlayer, pad_E04, 0xE04);
CHECK_OFFSET(cPlayer, Rider, 0xE10);
CHECK_SIZE(cPlayer, 0x1950);

#endif /* AI_PLAYER_H */
