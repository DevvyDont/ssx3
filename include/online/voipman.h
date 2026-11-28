#ifndef ONLINE_VOIPMAN_H
#define ONLINE_VOIPMAN_H

// cVoipMan: the voice-chat manager for online races (the headset). It connects the voice module to the peer's address
// (cVoipMan_connect, from cGameConnMan's cGameConnMan_connect), turns the voice on / off / toggles it, and sets the voice level
// from the options (cVoipMan_voiceLevel, cOVState_PAUSE_OPTIONS).
// EA's source: d:/ssx/code/source/online/voipman.cpp (the July 2003 preview's assert strings, with "!VoipMan" and the
// allocation tag "cVoipMan"). Retail code: dirtysock/tagsunk 0x00266CA0..0x00266F90. One instance, VoipMan:
// cVoipMan_createInstance creates it, cVoipMan_destroyInstance destroys it. No On Tour counterpart.
//
// Size 8: [R] cVoipMan_createInstance: cMemMan_alloc(8, "cVoipMan", 0x20000000) + the constructor cVoipMan_cVoipMan (clears both
// words). No vtable, no base class (the destructor cVoipMan::~cVoipMan is called directly).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the constructor; [U] how retail code uses the field
// (src/dirtysock/tagsunk.cpp). Names are ours.

#include "layout.h"

void operator_delete(int* p);

struct cVoipMan {
    int mConnected;                 // 0x0 [R] ctor 0; [U] cVoipMan_connect sets 1 after voipRpc_connect (connect),
                                    //     cVoipMan_disconnect clears it; every other method tests it first
    int mVoiceOn;                   // 0x4 [R] ctor 0; [U] cVoipMan_turnVoiceOn sets 1 (voipRpc_turnVoiceOn), cVoipMan_turnVoiceOff clears
                                    //     it (voipRpc_turnVoiceOff), cVoipMan_toggleVoice toggles on it

    // Methods bound to their retail symbols by asm labels.
    void connect(void* peerAddr) __asm__("cVoipMan_connect");
    void disconnect() __asm__("cVoipMan_disconnect");
    void turnVoiceOn() __asm__("cVoipMan_turnVoiceOn");
    void turnVoiceOff() __asm__("cVoipMan_turnVoiceOff");
    void toggleVoice() __asm__("cVoipMan_toggleVoice");
    void* getVoiceStatus() __asm__("cVoipMan_getVoiceStatus__FPv");
    void setVoiceLevel(int level, int maxLevel) __asm__("cVoipMan_setVoiceLevel");
    void getVoiceLevel(void* outLevel, void* outMax) __asm__("cVoipMan_getVoiceLevel");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    ~cVoipMan();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cVoipMan, mConnected, 0x0);
CHECK_OFFSET(cVoipMan, mVoiceOn, 0x4);
CHECK_SIZE(cVoipMan, 0x8);

#endif /* ONLINE_VOIPMAN_H */
