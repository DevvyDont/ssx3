#ifndef UTIL_CONSOLECONFIG_H
#define UTIL_CONSOLECONFIG_H

// cConsoleConfig: the console's system settings and clock - the OSD date/time format, the wall clock (hour .. day,
// advanced from the app timer by updateDateTime), the TV aspect, digital sound out, the sound mode and the system
// language. One object, made by the static cConsoleConfig::construct (cConsoleConfig_construct:
// `cMemMan_alloc(0x38, "cPS2ConsoleConfig", 0, 0)` + the cPS2ConsoleConfig ctor) and kept as SSXApp->ConsoleConfig
// (0x9C, main/ssxapp.h). cPS2ConsoleConfig (below) is the PS2 implementation: its ctor fills every member from
// sceScf* / sceCdReadClock and adds no members.
// No EA path in the preview (the preview has no console-config code); On Tour compiles the class in its util unit
// (z:/main/game/source/util/all_in_one_util.cpp), and retail keeps the methods in unit util/statemachine
// (0x002C6F78..0x002C7478) and the destructors in seg/seg_1D3510, so the header goes under util/.
//
// Size 0x38: [R] cConsoleConfig_construct allocates 0x38; [O] On Tour cConsoleConfig and cPSPConsoleConfig are 0x38.
// Base classes: none. cConsoleConfig declares the first virtual (its destructor: vtable cConsoleConfig_vtbl, 1 slot =
// cConsoleConfig__cConsoleConfig), so its vptr follows its data (g++ 2.95): 0x34.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] the constructor cPS2ConsoleConfig_cPS2ConsoleConfig (src/util/statemachine.cpp: stores every word 0x00..0x34),
//       the destructors cPS2ConsoleConfig__cPS2ConsoleConfig / cConsoleConfig__cConsoleConfig (store the vtable at
//       0x34) and the 0x38 allocation;
//   [U] how retail code uses the field (getTime / getDate / updateDateTime; cBEOptionInterface_defaultGame reads
//       SSXApp->ConsoleConfig + 0x24);
//   [O] SSX On Tour's DWARF (class cConsoleConfig): member names, types and order, and the same offsets (On Tour's
//       bool m_bDigitalOut is a word too). Enums are ints.

#include "layout.h"

struct cConsoleConfig {
    int mDateFormat;                // 0x00 [O] eConsoleDateFormat; [R] ctor: sceScfGetDateFormat 0..2, else 0
    int mTimeFormat;                // 0x04 [O] eConsoleTimeFormat; [R] ctor: sceScfGetTimeNotation 0/1, else 0
    int m_iHour;                    // 0x08 [O]; [R] ctor: RTC hour (BCD) or 0; [U] getTime, updateDateTime
                                    //      (addSecondsToTime's hour)
    int m_iMinute;                  // 0x0C [O]; [R] ctor: RTC minute or 0; [U] getTime, updateDateTime
    int m_iSecond;                  // 0x10 [O]; [R] ctor: RTC second or 0; [U] getTime, updateDateTime
    int m_iYear;                    // 0x14 [O]; [R] ctor: RTC year + 2000 or 2003; [U] getDate, updateDateTime
                                    //      (addDaysToDate's year)
    int m_iMonth;                   // 0x18 [O]; [R] ctor: RTC month or 10; [U] getDate, updateDateTime
    int m_iDay;                     // 0x1C [O]; [R] ctor: RTC day or 14; [U] getDate, updateDateTime
    int m_iStartTick;               // 0x20 [O]; [R] ctor: SSXApp->TimerTick (0x18); [U] updateDateTime: elapsed ticks
                                    //      are measured from it, and it is reset to now
    int mTVAspect;                  // 0x24 [O] eConsoleTVAspect; [R] ctor 0; [U] cBEOptionInterface_defaultGame (1 -> 2,
                                    //      2 -> 1 widescreen modes)
    int m_bDigitalOut;              // 0x28 [O] bool (a word); [R] ctor: 1 when sceScfGetSpdif returns 0, else 0
    int m_SoundMode;                // 0x2C [O] eConsoleSoundMode; [R] ctor 1
    int mLanguage;                  // 0x30 [O] eConsoleLanguage; [R] ctor: sceScfGetLanguage 0..7, else 1
    void** mVtbl;                   // 0x34 [R] cPS2ConsoleConfig_vtbl (ctor, dtor), cConsoleConfig_vtbl (cConsoleConfig's, stored
                                    //      by both destructors); slot 1 = destructor
};

CHECK_OFFSET(cConsoleConfig, mDateFormat, 0x00);
CHECK_OFFSET(cConsoleConfig, mTimeFormat, 0x04);
CHECK_OFFSET(cConsoleConfig, m_iHour, 0x08);
CHECK_OFFSET(cConsoleConfig, m_iMinute, 0x0C);
CHECK_OFFSET(cConsoleConfig, m_iSecond, 0x10);
CHECK_OFFSET(cConsoleConfig, m_iYear, 0x14);
CHECK_OFFSET(cConsoleConfig, m_iMonth, 0x18);
CHECK_OFFSET(cConsoleConfig, m_iDay, 0x1C);
CHECK_OFFSET(cConsoleConfig, m_iStartTick, 0x20);
CHECK_OFFSET(cConsoleConfig, mTVAspect, 0x24);
CHECK_OFFSET(cConsoleConfig, m_bDigitalOut, 0x28);
CHECK_OFFSET(cConsoleConfig, m_SoundMode, 0x2C);
CHECK_OFFSET(cConsoleConfig, mLanguage, 0x30);
CHECK_OFFSET(cConsoleConfig, mVtbl, 0x34);
CHECK_SIZE(cConsoleConfig, 0x38);

// cPS2ConsoleConfig: the PS2 cConsoleConfig. Adds no members ([R] its ctor stores nothing past 0x34 and the
// allocation is cConsoleConfig's 0x38; [O] On Tour's cPSPConsoleConfig is 0x38 with no members). Vtable
// cPS2ConsoleConfig_vtbl (0x00486798, 1 slot = cPS2ConsoleConfig__cPS2ConsoleConfig). Same fields as cConsoleConfig
// above (keep them in step).
struct cPS2ConsoleConfig {
    // --- cConsoleConfig
    int mDateFormat;                // 0x00 [O]; [R] ctor
    int mTimeFormat;                // 0x04 [O]; [R] ctor
    int m_iHour;                    // 0x08 [O]; [R] ctor
    int m_iMinute;                  // 0x0C [O]; [R] ctor
    int m_iSecond;                  // 0x10 [O]; [R] ctor
    int m_iYear;                    // 0x14 [O]; [R] ctor
    int m_iMonth;                   // 0x18 [O]; [R] ctor
    int m_iDay;                     // 0x1C [O]; [R] ctor
    int m_iStartTick;               // 0x20 [O]; [R] ctor: SSXApp->TimerTick
    int mTVAspect;                  // 0x24 [O]; [R] ctor 0
    int m_bDigitalOut;              // 0x28 [O] bool; [R] ctor
    int m_SoundMode;                // 0x2C [O]; [R] ctor 1
    int mLanguage;                  // 0x30 [O]; [R] ctor
    void** mVtbl;                   // 0x34 [R] cPS2ConsoleConfig_vtbl (ctor), cConsoleConfig_vtbl (dtor, then the base's)
};

CHECK_OFFSET(cPS2ConsoleConfig, mDateFormat, 0x00);
CHECK_OFFSET(cPS2ConsoleConfig, mTimeFormat, 0x04);
CHECK_OFFSET(cPS2ConsoleConfig, m_iHour, 0x08);
CHECK_OFFSET(cPS2ConsoleConfig, m_iMinute, 0x0C);
CHECK_OFFSET(cPS2ConsoleConfig, m_iSecond, 0x10);
CHECK_OFFSET(cPS2ConsoleConfig, m_iYear, 0x14);
CHECK_OFFSET(cPS2ConsoleConfig, m_iMonth, 0x18);
CHECK_OFFSET(cPS2ConsoleConfig, m_iDay, 0x1C);
CHECK_OFFSET(cPS2ConsoleConfig, m_iStartTick, 0x20);
CHECK_OFFSET(cPS2ConsoleConfig, mTVAspect, 0x24);
CHECK_OFFSET(cPS2ConsoleConfig, m_bDigitalOut, 0x28);
CHECK_OFFSET(cPS2ConsoleConfig, m_SoundMode, 0x2C);
CHECK_OFFSET(cPS2ConsoleConfig, mLanguage, 0x30);
CHECK_OFFSET(cPS2ConsoleConfig, mVtbl, 0x34);
CHECK_SIZE(cPS2ConsoleConfig, 0x38);

#endif /* UTIL_CONSOLECONFIG_H */
