#ifndef EALIB_SST_H
#define EALIB_SST_H

// EA SND's stream requests (sst.c): a stream channel keeps a list of queued requests, each one a sound to stream
// (views group D: Q_6B10 in ealib/seg_2B4578).
// EA source: snd/source/library/cmn/sst.c (On Tour: snd 9.06.03-psp). Retail code: unit ealib/seg_2B4578 (SNDSTRMI_*).
// The channel itself (On Tour SNDSTREAMCHANNEL, 0x214; view C_6B10, the current request at +0x11C in retail) is not
// laid out here: retail's layout differs from On Tour's by a word before allocatedrequests.
// Tags: [O] SSX On Tour's DWARF (names, members, offsets, volatile qualifiers, the paired functions); [U] retail use.

#include "layout.h"

// C units (ealib/seg_2B4578.c) name these types without the struct keyword.
#ifndef __cplusplus
typedef struct SNDLINKNODE SNDLINKNODE;
typedef struct SNDSTREAMREQUEST SNDSTREAMREQUEST;
typedef struct SNDLINKLIST SNDLINKLIST;
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

#endif /* EALIB_SST_H */
