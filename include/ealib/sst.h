#ifndef EALIB_SST_H
#define EALIB_SST_H

// EA SND's stream requests (sst.c): a stream channel keeps a list of queued requests, each one a sound to stream
// (views group D: Q_6B10 in ealib/seg_2B4578).
// EA source: snd/source/library/cmn/sst.c (On Tour: snd 9.06.03-psp). Retail code: unit ealib/seg_2B4578 (SNDSTRMI_*).
// The channel itself: SNDSTREAMCHANNEL below (retail 0x138; On Tour 0x214, which adds pFxVolume before filter[] and
// sourceChannelState[6] at the end).
// Tags: [O] SSX On Tour's DWARF (names, members, offsets, volatile qualifiers, the paired functions); [U] retail use.

#include "layout.h"
#include "ealib/snd.h"
#include "ealib/sfilter.h"

// C units (ealib/seg_2B4578.c) name these types without the struct keyword.
#ifndef __cplusplus
typedef struct SNDLINKNODE SNDLINKNODE;
typedef struct SNDSTREAMREQUEST SNDSTREAMREQUEST;
typedef struct SNDLINKLIST SNDLINKLIST;
typedef struct SNDSTREAMCHANNEL SNDSTREAMCHANNEL;
#endif

// [O] struct SNDLINKNODE {SNDLINKNODE* pnext; SNDLINKNODE* pprev;} (0x8): the link of EA SND's intrusive lists.
struct SNDLINKNODE {
    struct SNDLINKNODE* pnext;      // 0x0 [O]
    struct SNDLINKNODE* pprev;      // 0x4 [O]
};

CHECK_OFFSET(SNDLINKNODE, pnext, 0x0);
CHECK_OFFSET(SNDLINKNODE, pprev, 0x4);
CHECK_SIZE(SNDLINKNODE, 0x8);

// [O] struct SNDLINKLIST {SNDLINKNODE* phead; SNDLINKNODE* ptail; int items;} (0xC) and the list functions
// SNDLINKI_push(SNDLINKLIST* plist, SNDLINKNODE* pnode) / pushtail / pop / remove / init; [U] the retail SNDLINKI_*
// functions (unit ealib/seg_2B4578) read and write phead 0x0, ptail 0x4 and the count 0x8.
struct SNDLINKLIST {
    struct SNDLINKNODE* phead;      // 0x0 [O]; [U] push / pop / remove
    struct SNDLINKNODE* ptail;      // 0x4 [O]; [U] pushtail / remove
    int items;                      // 0x8 [O]; [U] incremented by push / pushtail, decremented by pop / remove
};

CHECK_OFFSET(SNDLINKLIST, phead, 0x0);
CHECK_OFFSET(SNDLINKLIST, ptail, 0x4);
CHECK_OFFSET(SNDLINKLIST, items, 0x8);
CHECK_SIZE(SNDLINKLIST, 0xC);

// [O] struct SNDSTREAMREQUEST (0x28) and SNDSTRMI_framescallback(int pktchan, int numframes, void* pclientdata),
// SNDSTRMI_removerequest(int sndrequesthandle); [U] the retail frames callback (view Q_6B10) passes sndrequesthandle
// (0xC) to SNDSTRMI_removerequest and steps currentframe / totalframes / outstandingframes (0x14 / 0x18 / 0x1C, volatile
// as in On Tour); [R] the unit's request pool entries are 0x28 bytes.
struct SNDSTREAMREQUEST {
    struct SNDLINKNODE node;                // 0x00 [O]
    volatile int streamrequestid;           // 0x08 [O]
    volatile int sndrequesthandle;          // 0x0C [O]; [U] SNDSTRMI_removerequest's argument
    volatile unsigned int avgdatarate;      // 0x10 [O]
    volatile unsigned int currentframe;     // 0x14 [O]; [U] the frames callback advances it
    volatile int totalframes;               // 0x18 [O]; [U]
    volatile unsigned int outstandingframes; // 0x1C [O]; [U]
    volatile int holdtime;                  // 0x20 [O]
    unsigned char continuation;             // 0x24 [O]
    volatile unsigned char isgeneric;       // 0x25 [O]
    char pad[2];                            // 0x26 [O]
};

CHECK_OFFSET(SNDSTREAMREQUEST, node, 0x00);
CHECK_OFFSET(SNDSTREAMREQUEST, streamrequestid, 0x08);
CHECK_OFFSET(SNDSTREAMREQUEST, sndrequesthandle, 0x0C);
CHECK_OFFSET(SNDSTREAMREQUEST, avgdatarate, 0x10);
CHECK_OFFSET(SNDSTREAMREQUEST, currentframe, 0x14);
CHECK_OFFSET(SNDSTREAMREQUEST, totalframes, 0x18);
CHECK_OFFSET(SNDSTREAMREQUEST, outstandingframes, 0x1C);
CHECK_OFFSET(SNDSTREAMREQUEST, holdtime, 0x20);
CHECK_OFFSET(SNDSTREAMREQUEST, continuation, 0x24);
CHECK_OFFSET(SNDSTREAMREQUEST, isgeneric, 0x25);
CHECK_OFFSET(SNDSTREAMREQUEST, pad, 0x26);
CHECK_SIZE(SNDSTREAMREQUEST, 0x28);

// SNDSTREAMCHANNEL, retail version (0x138 [R]: SNDSTRMI_create clears 312 bytes at the start of the stream's memory and
// puts the 40-byte requests after it; SNDSTRM_overheadtap = maxrequests * 40 + 312; On Tour 0x214): one stream channel,
// D_0050A0A0[32] (On Tour SNDSTREAMSTATE::pssc[32]). [O] struct SNDSTREAMCHANNEL and SNDSTRMI_getstreamptr(int)
// returning it; SNDSTRM_timemult / SNDSTRM_pitchmult keep it in `SNDSTREAMCHANNEL* pssc`. Retail lacks On Tour's
// pFxVolume (0x104 there), so filter[] and the lists sit a word lower; [U] the unit's view sA0A0Strm (every use in
// SNDSTRMI_*: f104 a 0x18 record, f11C / f128 the request lists, f134 the current request) has exactly this layout.
struct SNDSTREAMCHANNEL {
    int streamhandle;                       // 0x000 [O]; [U] sA0A0Strm f00 (the handle)
    volatile int shandle;                   // 0x004 [O]; [U] the voice: SNDvol / SNDautovol / SNDCTRL_timemult take it
    volatile int packetinstancehandle;      // 0x008 [O]; [U] sA0A0Strm f08 (the packet queue)
    volatile int lastsndrequesthandle;      // 0x00C [O]; [U] sA0A0Strm f0C
    volatile signed char state;             // 0x010 [O]; [U] sA0A0Strm f10 (parseheader sets 2 = switch pending)
    volatile signed char fromtap;           // 0x011 [O]; [U] sA0A0Strm f11
    char pad[2];                            // 0x012 [U] not accessed (On Tour: useOldAzimuthBehaviour, pad)
    SNDSAMPLEFORMAT cursf;                  // 0x014 [O]; [U] sA0A0Strm f14 (current format)
    SNDSAMPLEFORMAT newsf;                  // 0x018 [O]; [U] f18 (next format, SNDI_patchtohdr fills it)
    SNDSAMPLEATTR cursa;                    // 0x01C [O]; [U] f1C
    SNDSAMPLEATTR newsa;                    // 0x084 [O]; [U] f84
    SNDPLAYOPTS po;                         // 0x0EC [O]; [R] SNDSTRMI_create copies the play options here (sdr 0xEC);
                                            //      [U] func_003B7E70 / func_003B7EB0 vol (0xEC), func_003B8010 drylevel
                                            //      (0xF0), SNDSTRM_timemult timemult (0xFA), func_003B80A0 lowpasscutoff (0x100)
    SNDFILTERDEF filter[1];                 // 0x104 [O] (0x108 there); [U] sA0A0Strm f104 (0x18 bytes)
    SNDLINKLIST allocatedrequests;          // 0x11C [O]; [R] SNDSTRMI_create (addiu 0x11C); [U] f11C, the queued
                                            //      requests (view C_6B10 reads its head)
    SNDLINKLIST freerequests;               // 0x128 [O]; [U] f128
    volatile SNDSTREAMREQUEST* parsingrequest; // 0x134 [O]; [U] f134 (the current request)
};

CHECK_OFFSET(SNDSTREAMCHANNEL, streamhandle, 0x000);
CHECK_OFFSET(SNDSTREAMCHANNEL, shandle, 0x004);
CHECK_OFFSET(SNDSTREAMCHANNEL, packetinstancehandle, 0x008);
CHECK_OFFSET(SNDSTREAMCHANNEL, lastsndrequesthandle, 0x00C);
CHECK_OFFSET(SNDSTREAMCHANNEL, state, 0x010);
CHECK_OFFSET(SNDSTREAMCHANNEL, fromtap, 0x011);
CHECK_OFFSET(SNDSTREAMCHANNEL, pad, 0x012);
CHECK_OFFSET(SNDSTREAMCHANNEL, cursf, 0x014);
CHECK_OFFSET(SNDSTREAMCHANNEL, newsf, 0x018);
CHECK_OFFSET(SNDSTREAMCHANNEL, cursa, 0x01C);
CHECK_OFFSET(SNDSTREAMCHANNEL, newsa, 0x084);
CHECK_OFFSET(SNDSTREAMCHANNEL, po, 0x0EC);
CHECK_OFFSET(SNDSTREAMCHANNEL, filter, 0x104);
CHECK_OFFSET(SNDSTREAMCHANNEL, allocatedrequests, 0x11C);
CHECK_OFFSET(SNDSTREAMCHANNEL, freerequests, 0x128);
CHECK_OFFSET(SNDSTREAMCHANNEL, parsingrequest, 0x134);
CHECK_SIZE(SNDSTREAMCHANNEL, 0x138);

#endif /* EALIB_SST_H */
