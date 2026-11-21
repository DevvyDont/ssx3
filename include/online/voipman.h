#ifndef ONLINE_VOIPMAN_H
#define ONLINE_VOIPMAN_H

// cVoipMan: the voice-chat manager for online races (the headset). It connects the voice module to the peer's address
// (func_00266D58, from cGameConnMan's func_002639E0), turns the voice on / off / toggles it, and sets the voice level
// from the options (D_004A33F8, cOVState_PAUSE_OPTIONS).
// EA's source: d:/ssx/code/source/online/voipman.cpp (the July 2003 preview's assert strings, with "!VoipMan" and the
// allocation tag "cVoipMan"). Retail code: dirtysock/tagsunk 0x00266CA0..0x00266F90. One instance, VoipMan:
// func_00266CA0 creates it, func_00266CD8 destroys it. No On Tour counterpart.
//
// Size 8: [R] func_00266CA0: cMemMan_alloc(8, "cVoipMan", 0x20000000) + the constructor func_00266D00 (clears both
// words). No vtable, no base class (the destructor func_00266D10 is called directly).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the constructor; [U] how retail code uses the field
// (src/dirtysock/tagsunk.cpp). Names are ours.

#include "layout.h"

struct cVoipMan {
    int mConnected;                 // 0x0 [R] ctor 0; [U] func_00266D58 sets 1 after func_003F6AB8 (connect),
                                    //     func_00266DF8 clears it; every other method tests it first
    int mVoiceOn;                   // 0x4 [R] ctor 0; [U] func_00266E30 sets 1 (func_003F6B20), func_00266E88 clears
                                    //     it (func_003F6B50), func_00266ED0 toggles on it
};

CHECK_OFFSET(cVoipMan, mConnected, 0x0);
CHECK_OFFSET(cVoipMan, mVoiceOn, 0x4);
CHECK_SIZE(cVoipMan, 0x8);

#endif /* ONLINE_VOIPMAN_H */
