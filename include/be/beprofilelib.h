#ifndef BE_BEPROFILELIB_H
#define BE_BEPROFILELIB_H

// cBEProfileLib: one player profile's career data - the ten characters' records (cBECharProfileDB, be/bedbprofile.h).
// The back end keeps three of them in the static array cBELibrary_profileLib (0x004A6CA8; [3], 0x9B50 apart; units
// declare it ad hoc: `extern int cBELibrary_profileLib[]` and per-unit views, so it is not declared here). Methods:
// init (cBECharProfileDB_init on each character), copy (record-by-record, keeping the destination's bolt lookup tables),
// backupCharProfile / restoreCharProfile (one character's record to / from the static BackupCharProfile = cBEProfileLib_BackupCharProfile,
// the index in D_004A125C), and func_00156810 (each character's kind defaults).
// No EA path for the class (the preview has be/bedbprofile.cpp / .h and be/belibrary.cpp, but no cBEProfileLib string);
// its methods sit in the retail unit be/beprepoststate (0x001567B8..0x00156988), so the header takes the be/ directory.
//
// Size 0x9B50: [R] cBEProfileLib_init / cBEProfileLib_copy / func_00156810 walk 10 records 0xF88 apart; callers index
// cBELibrary_profileLib with a 0x9B50 stride (cBELibrary_newProfile: cBELibrary_profileLib + idx * 39760; beintnetwork.cpp
// `player * 0x9B50`; belibrary.cpp copies 0x9B50-byte blocks); [O] On Tour's cBEProfileLib (0xF98) holds a single
// cBECharProfileDB charProfile (On Tour's profile has one character; retail has ten). No base classes, no vtable.
// Tags: [R] the methods' loops; [U] how retail code uses the field; [O] SSX On Tour's DWARF (member name).

#include "layout.h"
#include "be/bedbprofile.h"

struct cBEProfileLib {
    cBECharProfileDB charProfile[10];   // 0x0000 [O] charProfile (On Tour: one, not an array); [R] init / copy /
                                        //        func_00156810 loop i < 10 at i * 0xF88; [U] backupCharProfile /
                                        //        restoreCharProfile copy charProfile[idx] (3976 bytes)
};

CHECK_OFFSET(cBEProfileLib, charProfile, 0x0);
CHECK_SIZE(cBEProfileLib, 0x9B50);

#endif /* BE_BEPROFILELIB_H */
