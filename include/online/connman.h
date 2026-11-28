#ifndef ONLINE_CONNMAN_H
#define ONLINE_CONNMAN_H

// cConnMan / cGameConnMan: the front end's two online connection state machines, both embedded in cRealFE
// (fe/fereal.h; the preview's getConnMan__7cRealFE / getGameConnMan__7cRealFE, retail lobbyman_getConnMan / lobbyman_getGameConnMan,
// return this + 0xB5AB0 / + 0xB5ABC; cRealFE_cRealFE constructs both, cRealFE__cRealFE destroys both with flags 2,
// cRealFE_update ticks both).
//   cConnMan: the network (ISP / DNAS) connection: cConnMan_connect starts cCommSystem::netConnConnect with a timeout,
//     cConnMan_update watches mNetConnState (posts online event 261 on success), cConnMan_disconnect logs out and disconnects.
//   cGameConnMan: the peer-to-peer race connection to the opponent: cGameConnMan_connect opens the DirtySock channel,
//     cGameConnMan_update waits for it, runs the setup manager (cSetupMan, cSetupMan_mSetupMan) and starts the race, cGameConnMan_shutdown
//     shuts it down and posts the reason event.
// No EA source path in the preview (only the class strings _vt$8cConnMan, _vt$12cGameConnMan and the accessors
// getState__8cConnMan / getState__12cGameConnMan); retail code is in unit dirtysock/tagsunk (0x00263660..0x00264030)
// next to BuddyMan's, and both classes belong to the online front end, so the header goes under online/.
// Neither class exists in SSX On Tour (no [O] evidence); member names are from retail use, except the preview's getState.
//
// Sizes: cConnMan 0xC, cGameConnMan 0x18 - [R] cRealFE_cRealFE builds them at 0xB5AB0 and 0xB5ABC and zeroes the next
// word at 0xB5AD4, and each class's vptr is its last word (cConnMan 0x8, cGameConnMan 0x14). No base classes: each
// declares its own virtual destructor (1-slot vtables cConnMan_vtbl 0x004810C8, cGameConnMan_vtbl 0x004810B0), so the
// vptr follows the data (g++ 2.95).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructors (cConnMan_cConnMan / cGameConnMan_cGameConnMan: state 0, vtable) and destructors;
//   [P] the preview's inline getState (lw 0x0) for both classes (tools/layout_xlate.py has no paired method to measure a
//       shift; the retail ctors zero the same word);
//   [U] how retail code uses the field (src/dirtysock/tagsunk.cpp; the functions named above).

#include "layout.h"

struct cConnMan {
    int mState;                     // 0x0 [P] getState__8cConnMan (lw 0x0); [R] ctor 0; [U] 0 idle, 1 connecting
                                    //     (cConnMan_connect), 2 connected (cConnMan_update); cConnMan_disconnect sets 0
    float mTimeout;                 // 0x4 [U] cConnMan_connect: 60.0 on broadband, else 180.0 seconds; cConnMan_update counts
                                    //     it down by SSXApp->TimerPeriod and gives up (0x2D3F3F3F) below 0 (view
                                    //     sDsTimer_263828 timer)
    void** mVtbl;                   // 0x8 [R] cConnMan_vtbl (ctor, dtor); slot 1 = destructor

    // Methods bound to their retail symbols by asm labels.
    void connect() __asm__("cConnMan_connect");
};

CHECK_OFFSET(cConnMan, mState, 0x0);
CHECK_OFFSET(cConnMan, mTimeout, 0x4);
CHECK_OFFSET(cConnMan, mVtbl, 0x8);
CHECK_SIZE(cConnMan, 0xC);

struct cGameConnMan {
    int mState;                     // 0x00 [P] getState__12cGameConnMan (lw 0x0); [R] ctor 0; [U] 0 idle, 1 connecting
                                    //      (cGameConnMan_connect), 2 setting up (cGameConnMan_update); cGameConnMan_shutdown sets 0
    int mChannel;                   // 0x04 [U] cGameConnMan_connect stores cCommSystem_openDSockChannel's channel; cGameConnMan_update /
                                    //      cGameConnMan_shutdown pass it to cCommSystem_isChannelConnected / getError /
                                    //      isChannelOpen / closeChannel, cSetupMan_createInstance and cGameComm_construct
    int mIsHost;                    // 0x08 [U] cGameConnMan_connect's isHost argument; passed to cSetupMan_createInstance (cSetupMan) and
                                    //      cGameComm_construct
    int mCancel;                    // 0x0C [U] cGameConnMan_connect's last argument; cGameConnMan_update shuts down with reason 1 as
                                    //      soon as the channel connects when it is set
    float mTimeout;                 // 0x10 [U] cGameConnMan_connect: 60.0 (D_004A2F14); cGameConnMan_update counts it down by
                                    //      SSXApp->TimerPeriod and shuts down (reason 0) below 0
    void** mVtbl;                   // 0x14 [R] cGameConnMan_vtbl (ctor, dtor); slot 1 = destructor

    // Methods bound to their retail symbols by asm labels.
    void shutdown(int reason) __asm__("cGameConnMan_shutdown");
};

CHECK_OFFSET(cGameConnMan, mState, 0x00);
CHECK_OFFSET(cGameConnMan, mChannel, 0x04);
CHECK_OFFSET(cGameConnMan, mIsHost, 0x08);
CHECK_OFFSET(cGameConnMan, mCancel, 0x0C);
CHECK_OFFSET(cGameConnMan, mTimeout, 0x10);
CHECK_OFFSET(cGameConnMan, mVtbl, 0x14);
CHECK_SIZE(cGameConnMan, 0x18);

#endif /* ONLINE_CONNMAN_H */
