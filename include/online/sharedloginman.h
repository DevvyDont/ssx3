#ifndef ONLINE_SHAREDLOGINMAN_H
#define ONLINE_SHAREDLOGINMAN_H

// cSharedLoginMan: the shared-login manager. It keeps the online account (user name + password, the network-account
// library's "shared login" record) on the memory card: read, save, create, delete and format, as a small state machine
// driven every frame by func_00265A68 from the account-on-memory-card front-end state (cFEStateNetAccountMem). The card
// object and the account buffers are statics (D_004A33D8 the cPS2MemCard, D_004A33D4 the current card operation,
// D_004A33DC the port, D_004A33E0 / D_004A33E4, D_00536760 the 36-byte account record), not members.
// EA's source: d:/ssx/code/source/online/sharedloginman.cpp (the July 2003 preview's assert strings, with
// "!SharedLoginMan" and the allocation tag "cSharedLoginMan"). Retail code: dirtysock/tagsunk 0x00265880..0x00265F00.
// One instance, D_004A33D0: func_00265880 creates it, func_002658B8 destroys it. No On Tour counterpart.
//
// Size 8: [R] func_00265880: cMemMan_alloc(8, "cSharedLoginMan", 0x20000000) + the constructor func_002658E0, which runs
// the init func_00265950 (state 1, result 0). No vtable, no base class (the destructor func_00265908 is called directly).
//
// A FLAT layout, like main/game.h. Evidence tags: [R] the init func_00265950 / shutdown func_002659C0; [U] how retail
// code uses the field (src/dirtysock/tagsunk.cpp, src/fe/feasyncfile.cpp).
// mState values (func_00265A68): 0 shut down, 1 idle; requests 2 read, 6 save, 10 create, 14 library operation 3, 18
// format (func_00265DF0 .. func_00265E30 set them); each request starts in the next state (3, 7, 11, 15 / 19), polls,
// then moves one state on when done (4, 8, 12, 16) or two on when it failed; 20 / 21 / 22 format results.

#include "layout.h"

struct cSharedLoginMan {
    int mState;                     // 0x0 [R] init 1, shutdown 0; [U] the state machine above; the front end's
                                    //     cFEStateNetAccountMem tests it (*D_004A33D0 == state)
    int mError;                     // 0x4 [R] init 0; [U] each request step clears it, func_00265A68 stores the
                                    //     library poll's error (func_003F80E0)
};

CHECK_OFFSET(cSharedLoginMan, mState, 0x0);
CHECK_OFFSET(cSharedLoginMan, mError, 0x4);
CHECK_SIZE(cSharedLoginMan, 0x8);

#endif /* ONLINE_SHAREDLOGINMAN_H */
