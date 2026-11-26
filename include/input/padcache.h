#ifndef INPUT_PADCACHE_H
#define INPUT_PADCACHE_H

// cPadCache: the application's controller sample cache - a 30-line ring of button samples for up to 4 pad devices.
// cacheInput reads every cPadDevice into the line at the write position (each device's virtual read(buf, 0x18)),
// nextInput / flush move the read position, numButtons / buttonValues return a pad's samples in the read line.
// One instance: cSSXApp_init does cMemMan_alloc(12028, "PadCache", 0, 0) + cPadCache_init and keeps it in the global
// PadCache (0x004A28A0); cSSXApp_purge calls cPadCache_purge. EA's source: d:/ssx/code/source/input/padcache.cpp and
// .../input/padcache.h (preview assert paths); retail code is in the decomp's unit input/inputparse.cpp
// (0x00326A28 .. 0x00326CE8), next to cPadDevice / cPS2PadDevice. (It does not belong in input/inputparse.h: that is
// cInputMapParser's header, and EA had a padcache.h of its own.)
// Size 0x2EFC: [R] the allocation size 12028 = 30 * 4 * 0x64 + 0xC + 4 * 4. No base classes, no vtable.
//
// A FLAT layout, like main/game.h. Tags: [R] cPadCache_init (cPadDevice_construct(self + 0x2EEC, 4) -> 0x2EE8,
// read position 29, write position 0) and the allocation size; [U] how retail code uses the field; [O] SSX On Tour's
// DWARF (class cPadCache, size 0xBC8: the same members with one pad device - CacheSize 30, MaxPadDevices 1).
// No preview evidence (no accessors).
//
// Global instance: PadCache (0x004A28A0). Units declare it `extern void* PadCache` (7x) or `extern char* PadCache` (3x),
// so it is not declared here.

#include "layout.h"

struct cPadDevice;   // polymorphic: slot 1 = destructor (purge), slot 2 = read(buf, 0x18) (cacheInput)

// One sample of one pad. [O] struct CacheLine (size 0x64, same members); [U] cacheInput stores the device's read()
// result (the button count) in NumButtons and lets it fill Buttons; numButtons / buttonValues return them.
struct CacheLine {
    int NumButtons;                 // 0x000 [O]; [U] cPadCache_numButtons returns it
    float Buttons[24];              // 0x004 [O]; [U] cPadCache_buttonValues returns its address; cacheInput reads 0x18 values into it
};
CHECK_OFFSET(CacheLine, NumButtons, 0x0);
CHECK_OFFSET(CacheLine, Buttons, 0x4);
CHECK_SIZE(CacheLine, 0x64);

struct cPadCache {
    CacheLine InputCache[30][4];    // 0x0000 [O] InputCache (On Tour [1][30]); [U] indexed [line][pad]: cacheInput writes
                                    //        line CacheWritePos, numButtons / buttonValues read line CacheReadPos
                                    //        (offset (line * 4 + pad) * 0x64); [R] 30 lines (the % 30 ring), 4 pads
    unsigned int CacheReadPos;      // 0x2EE0 [O] (unsigned); [R] init: 29; [U] nextInput / numButtons / buttonValues (divu);
                                    //        flush sets it to (write + 29) % 30 through int locals (signed div there)
    unsigned int CacheWritePos;     // 0x2EE4 [O] (On Tour: volatile unsigned int; retail code matches without volatile);
                                    //        [R] init: 0; [U] cacheInput advances it (divu), nextInput / flush read it
    int NumPadDevices;              // 0x2EE8 [O]; [R] init stores cPadDevice_construct's count; [U] purge / cacheInput /
                                    //        clearAllVibrations loop over it; cPreStartScreen_update tests it > 0
    cPadDevice* PadDevices[4];      // 0x2EEC [O] (On Tour [1]); [R] cPadDevice_construct(self + 0x2EEC, 4) fills it;
                                    //        [U] purge deletes each (slot 1, flags 3); fepopupmisc / festateoptions read
                                    //        PadCache + 0x2EEC + 4 * i

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cPadCache_init");
    void purge() __asm__("cPadCache_purge");
    void clearAllVibrations() __asm__("cPadCache_clearAllVibrations");
    bool nextInput() __asm__("cPadCache_nextInput");
    void cacheInput() __asm__("cPadCache_cacheInput");
    int flush() __asm__("cPadCache_flush");
    int numButtons(int i) __asm__("cPadCache_numButtons");
    void* buttonValues(int i) __asm__("cPadCache_buttonValues");
};
CHECK_OFFSET(cPadCache, InputCache, 0x0);
CHECK_OFFSET(cPadCache, CacheReadPos, 0x2EE0);
CHECK_OFFSET(cPadCache, CacheWritePos, 0x2EE4);
CHECK_OFFSET(cPadCache, NumPadDevices, 0x2EE8);
CHECK_OFFSET(cPadCache, PadDevices, 0x2EEC);
CHECK_SIZE(cPadCache, 0x2EFC);

#endif /* INPUT_PADCACHE_H */
