#ifndef ONLINE_DSOCKCHANNEL_H
#define ONLINE_DSOCKCHANNEL_H

// cDSockChannel: the DirtySock implementation of cCommChannel: one peer-to-peer game link (NetGameUtil to connect or
// listen on "addr:port:port", then a NetGameLink to send and receive packets). Created by
// cCommSystem_openDSockChannel: `cMemMan_alloc(0x20, "cDSockChannel", ...)` + the constructor func_00256698, then
// listen (func_002568F8) or connect (func_00256860). Size 0x20 [R]. Base class cCommChannel (online/commchannel.h,
// 0xC bytes), written out inline. Retail vtable D_004812D0 (slots: see kCommChannelVt_* in online/commchannel.h).
// EA's source: d:/ssx/code/source/online/dsockchannel.cpp (the preview build's assert string).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] retail's constructor func_00256698 / destructor func_002566E8 and the allocation;
//   [P] the July 2003 preview's inline accessors (cCommChannel's and isConnected__C13cDSockChannel = lw 0xC == 2;
//       same offsets as retail);
//   [U] how retail code uses the field (the methods func_00256698..func_00256BA8 in seg_156860 and the two inline
//       virtuals cDSockChannel_isConnected / cDSockChannel_getChannelType in tagsunk);
//   [O] SSX On Tour's DWARF (class cDSockChannel, size 0x1C): the same five members in the same order, 4 bytes
//       earlier (its base is 8 bytes: 1-byte bools); its mPacketError is a 1-byte bool.

#include "layout.h"

struct NetGameUtilRefT;
struct NetGameLinkRefT;

struct cDSockChannel {
    // --- cCommChannel (0x0-0xB)
    int mChannelOpen;               // 0x00 [O] cCommChannel::mChannelOpen; [P] isChannelOpen; [R] base ctor 0;
                                    //      [U] connect/listen call channelOpen, disconnect calls channelClose
    int mError;                     // 0x04 [O] cCommChannel::mError; [P] getError; [U] listen clears it;
                                    //      updateConnected sets 1 on a timeout or a lost link; writePacket requires 0
    void** mVtbl;                   // 0x08 [R] D_004812D0 (ctor and dtor)
    // --- cDSockChannel
    int mState;                     // 0x0C [O]; [P] isConnected = (mState == 2); [R] 0; [U] 0 closed, 1 connecting
                                    //      (connect/listen), 2 connected (updateConnecting), 3 timed out and 4 packet
                                    //      error (updateConnected); update (cDSockChannel_update) switches on it
    float mLatency;                 // 0x10 [O]; [R] 0.1; [U] updateConnected smooths the link's ping into it
                                    //      ((4 * old + new) / 5, capped at 1 s); getLatency (func_00256BA8) returns it
    int mPacketError;               // 0x14 [O] (On Tour: bool); [R] 0; [U] readPacket (cDSockChannel_readPacket) sets 1 on a
                                    //      packet of the wrong kind; updateConnected turns it into state 4
    NetGameUtilRefT* mGameUtil;     // 0x18 [O]; [R] 0; [U] connect/listen store the NetGameUtil (func_003E6E08) and
                                    //      start it (func_003E6F40, 0x101 connect / 0x102 listen); disconnect destroys it
    NetGameLinkRefT* mGameLink;     // 0x1C [O]; [R] 0; [U] updateConnecting creates it (func_003E66F0) once the util
                                    //      completes; read/writePacket and updateConnected use it; disconnect destroys it
};

CHECK_OFFSET(cDSockChannel, mChannelOpen, 0x00);
CHECK_OFFSET(cDSockChannel, mError, 0x04);
CHECK_OFFSET(cDSockChannel, mVtbl, 0x08);
CHECK_OFFSET(cDSockChannel, mState, 0x0C);
CHECK_OFFSET(cDSockChannel, mLatency, 0x10);
CHECK_OFFSET(cDSockChannel, mPacketError, 0x14);
CHECK_OFFSET(cDSockChannel, mGameUtil, 0x18);
CHECK_OFFSET(cDSockChannel, mGameLink, 0x1C);
CHECK_SIZE(cDSockChannel, 0x20);

#endif /* ONLINE_DSOCKCHANNEL_H */
