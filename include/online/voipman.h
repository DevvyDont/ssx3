#ifndef ONLINE_VOIPMAN_H
#define ONLINE_VOIPMAN_H

// cVoipMan: the voice-chat manager for online races (the headset). It connects the voice module to the peer's address
// (cVoipMan_connect, from cGameConnMan's cGameConnMan_connect), turns the voice on / off / toggles it, and sets the voice level
// from the options (D_004A33F8, cOVState_PAUSE_OPTIONS).
// EA's source: d:/ssx/code/source/online/voipman.cpp (the July 2003 preview's assert strings, with "!VoipMan" and the
// allocation tag "cVoipMan"). Retail code: dirtysock/tagsunk 0x00266CA0..0x00266F90. One instance, VoipMan:
// cVoipMan_createInstance creates it, cVoipMan_destroyInstance destroys it. No On Tour counterpart.
//
// Size 8: [R] cVoipMan_createInstance: cMemMan_alloc(8, "cVoipMan", 0x20000000) + the constructor cVoipMan_cVoipMan (clears both
// words). No vtable, no base class (the destructor cVoipMan__cVoipMan is called directly).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the constructor; [U] how retail code uses the field
// (src/dirtysock/tagsunk.cpp). Names are ours.

#include "layout.h"

struct cVoipMan {
    int mConnected;                 // 0x0 [R] ctor 0; [U] cVoipMan_connect sets 1 after func_003F6AB8 (connect),
                                    //     cVoipMan_disconnect clears it; every other method tests it first
    int mVoiceOn;                   // 0x4 [R] ctor 0; [U] cVoipMan_turnVoiceOn sets 1 (func_003F6B20), cVoipMan_turnVoiceOff clears
                                    //     it (func_003F6B50), cVoipMan_toggleVoice toggles on it
};

CHECK_OFFSET(cVoipMan, mConnected, 0x0);
CHECK_OFFSET(cVoipMan, mVoiceOn, 0x4);
CHECK_SIZE(cVoipMan, 0x8);

#endif /* ONLINE_VOIPMAN_H */
