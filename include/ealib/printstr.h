#ifndef EALIB_PRINTSTR_H
#define EALIB_PRINTSTR_H

// REAL core's debug print router (printstr.cpp): PRINT_vstring(channel, format, args) formats once and hands the text
// to every enabled output device of an enabled channel; PRINT_setchannelstate / PRINT_setdevicestate switch them,
// PRINT_init fills both tables once (PRINTinitialized). EA source: realcore/source/system/cmn/printstr.cpp (SSX On Tour:
// realcore 1.06.00). Retail code: unit ealib/seg_2DCAE8 (PRINT_init, PRINT_setchannelstate, PRINT_setdevicestate,
// PRINT_vstring and the PRINT_string wrappers).
//
// Names, members and table sizes: [O] On Tour's DWARF: struct PRINTCHANNELINFO (0x8) and PRINTDEVICEINFO (0xC), the
// globals PRINTchannellist (PRINTCHANNELINFO[64], 512 bytes), PRINTinitialized (int) and PRINTdevicelist
// (PRINTDEVICEINFO[8], 96 bytes), void PRINT_vstring(PRINTCHANNEL channel, const char* format, char* arglist);
// [R] retail's tables have the same sizes (PRINTchannellist 0x00450E08 .. PRINTinitialized 0x00451008 = 512 bytes;
// PRINT_vstring walks PRINTdevicelist in 12-byte steps up to +0x60) and the same bit use (enabled = bit 0, the
// channel's group flag real = bit 1 in PRINT_setchannelstate). C structs. Tags: [O] On Tour; [R] retail use.

#include "layout.h"

#ifndef __cplusplus
typedef struct PRINTCHANNELINFO PRINTCHANNELINFO;
typedef struct PRINTDEVICEINFO PRINTDEVICEINFO;
#endif

// One print channel (PRINTchannellist[64], indexed by PRINTCHANNEL).
struct PRINTCHANNELINFO {
    const char* name;               // 0x0 [O]
    unsigned int enabled : 1;       // 0x4 bit 0 [O]; [R] PRINT_vstring prints only when set; PRINT_setchannelstate
    unsigned int real : 1;          // 0x4 bit 1 [O]; [R] PRINT_setchannelstate(1, ..) switches the channels with it set
    unsigned int pad : 30;          // 0x4 [O]
};

CHECK_OFFSET(PRINTCHANNELINFO, name, 0x0);
CHECK_SIZE(PRINTCHANNELINFO, 0x8);

// One output device (PRINTdevicelist[8]): PRINT_vstring calls func(channel, text) on each enabled one.
struct PRINTDEVICEINFO {
    const char* name;               // 0x0 [O]; [R] PRINT_init clears it
    void (*func)(int channel, char* text); // 0x4 [O] (the dump prints `void (*)() *`); [R] PRINT_vstring calls it
                                    //     with the channel and the formatted text; PRINT_init clears it
    unsigned int enabled : 1;       // 0x8 bit 0 [O]; [R] PRINT_vstring tests it; PRINT_setdevicestate / PRINT_init
    unsigned int pad : 31;          // 0x8 [O]
};

CHECK_OFFSET(PRINTDEVICEINFO, name, 0x0);
CHECK_OFFSET(PRINTDEVICEINFO, func, 0x4);
CHECK_SIZE(PRINTDEVICEINFO, 0xC);

extern PRINTCHANNELINFO PRINTchannellist[64];   // [O]; [R] 0x00450E08
extern int PRINTinitialized;                    // [O]; [R] 0x00451008
extern PRINTDEVICEINFO PRINTdevicelist[8];      // [O]; [R] 0x00451250

#endif /* EALIB_PRINTSTR_H */
