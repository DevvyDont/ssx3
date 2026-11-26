#ifndef ONLINE_COMMCHANNEL_H
#define ONLINE_COMMCHANNEL_H

// cCommChannel: the abstract peer-to-peer channel of the online code; cDSockChannel (online/dsockchannel.h) is the
// only implementation. cCommSystem::mChannels holds them. EA's source: d:/ssx/code/source/online/commchannel.cpp
// (the preview build's assert string). Size 0xC: two 4-byte bools, then the vptr (g++ 2.95 puts it after the data of the class
// that declares the virtuals); cDSockChannel's first own field is at 0xC. Retail vtable D_00481320.
// Retail methods: constructor cCommChannel_cCommChannel (stores the vtable, clears both flags), destructor cCommChannel__cCommChannel,
// channelOpen cCommChannel_channelOpen, channelClose cCommChannel_channelClose (all in mem/memstd), and the default virtuals in tagsunk
// (cCommChannel_disconnect = channelClose, update/isConnected/getChannelType/readPacket/writePacket return 0,
// getLatency returns 1/60). Retail keeps out-of-line copies of the preview's inline isChannelOpen/setError/getError
// (cCommChannel_isChannelOpen/setError/getError, 0x267D88..0x267DA0).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructor/destructor; [P] the July 2003 preview's inline accessors (same offsets as retail);
//   [U] how retail code uses the field; [O] SSX On Tour's DWARF (class cCommChannel: mChannelOpen, mError as 1-byte
//   bools at 0/1, vptr at 4; retail's bools are words).

#include "layout.h"

// Virtual slots of cCommChannel (retail D_00481320, preview _vt$12cCommChannel; entry 0 is the empty RTTI entry).
// Names from the preview's slot functions (disconnect__12cCommChannel ... writePacket__12cCommChannelPCvi); a call
// through slot N reads the delta at vtbl + 8*N and the function at vtbl + 8*N + 4.
enum {
    kCommChannelVt_dtor = 1,            // cCommChannel__cCommChannel (cDSockChannel: cDSockChannel__cDSockChannel)
    kCommChannelVt_disconnect = 2,      // cCommChannel_disconnect (cDSockChannel: cDSockChannel_disconnect)
    kCommChannelVt_update = 3,          // cCommChannel_update (cDSockChannel: cDSockChannel_update)
    kCommChannelVt_isConnected = 4,     // cCommChannel_isConnected (cDSockChannel_isConnected)
    kCommChannelVt_getLatency = 5,      // cCommChannel_getLatency (cDSockChannel: cDSockChannel_getLatency)
    kCommChannelVt_getChannelType = 6,  // cCommChannel_getChannelType (cDSockChannel_getChannelType: 1)
    kCommChannelVt_readPacket = 7,      // cCommChannel_readPacket (cDSockChannel: cDSockChannel_readPacket)
    kCommChannelVt_writePacket = 8      // cCommChannel_writePacket (cDSockChannel: cDSockChannel_writePacket)
};

struct cCommChannel {
    int mChannelOpen;   // 0x0 [O]; [P] isChannelOpen__C12cCommChannel = lw 0x0; [R] ctor 0; [U] channelOpen sets 1,
                        //     channelClose clears it; cCommSystem's closeChannel tests it before disconnecting
    int mError;         // 0x4 [O]; [P] getError = lw 0x4, setError = sw 1 at 0x4; [R] ctor 0; [U] channelOpen /
                        //     channelClose clear it; cCommSystem getError (cCommSystem_getError) returns it
    void** mVtbl;       // 0x8 [R] D_00481320 (cCommChannel), D_004812D0 (cDSockChannel)

    // Methods bound to their retail symbols by asm labels.
    void setError() __asm__("cCommChannel_setError");
    int getError() __asm__("cCommChannel_getError");
    void* disconnect() __asm__("cCommChannel_disconnect__FPv");
    void update() __asm__("cCommChannel_update__FPv");
    int isConnected() __asm__("cCommChannel_isConnected__FPv");
    int getChannelType() __asm__("cCommChannel_getChannelType__FPv");
    int readPacket() __asm__("cCommChannel_readPacket__FPv");
    int writePacket() __asm__("cCommChannel_writePacket__FPv");
};

CHECK_OFFSET(cCommChannel, mChannelOpen, 0x0);
CHECK_OFFSET(cCommChannel, mError, 0x4);
CHECK_OFFSET(cCommChannel, mVtbl, 0x8);
CHECK_SIZE(cCommChannel, 0xC);

#endif /* ONLINE_COMMCHANNEL_H */
