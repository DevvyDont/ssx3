#ifndef OBJECT_INSTANCEMAN_H
#define OBJECT_INSTANCEMAN_H

// cInstanceMan: the game's pool of dynamic instance records (tInstanceManRecord, 0xB0 each: a tInstanceRecord + 4 pad
// bytes) for objects created at run time (world scripts, replays): a record array and a free list. One instance:
// cGame_load (src/main/gameload.cpp) does `operator new(0x10, "InstancesMan", 0x20000000)` + the constructor with
// section id 0x80 and 0x40 records, kept as cGame::InstanceMan (main/game.h 0x88; the preview's getInstanceMan__C5cGame).
// Methods: the constructor cInstanceMan_cInstanceMan (0x00351030; formerly misnamed cWScriptCompile_parseKeywords), the destructor, allocInstance /
// allocInstance1 (take a record off the free list and stamp its id), freeInstance, copyInstance, and func_003512C0 (claim
// record id >> 8 when the id's section byte matches).
// No EA path in the preview; its inline asserts "(((tInstanceManRecord*)(inst)) - Instances) < NumInstances" and
// "instId.m_bfRID.m_uiUID < (unsigned int)NumInstances" sit in the object/ string pool after instancenode.cpp. Retail
// code: 0x00351030..0x003512C0, split by the decomp between units wscript/wscriptcompile and object/instanceman; the
// header takes object/ (the class is the object system's).
//
// Size 0x10: [R] the allocation in cGame_load; [O] On Tour's cInstanceMan is 0x10 with the same four members.
// No base classes, no vtable (the destructor cInstanceMan__cInstanceMan is not virtual).
// Tags: [R] the constructor (stores all four members) and the allocation; [P] the preview asserts above; [U] how retail
// code uses the field; [O] SSX On Tour's DWARF (class cInstanceMan: same names, types, order and offsets).

#include "layout.h"

struct tInstanceList;       // On Tour: { tInstanceList* Next; } - the first word of a free record
struct tInstanceManRecord;  // On Tour 0xB0: a tInstanceRecord (0xAC) + m_PadBytes[4]

struct cInstanceMan {
    unsigned int InstanceSectionId; // 0x0 [O]; [R] ctor stores its first argument (0x80); [U] allocInstance /
                                    //     allocInstance1 copy its low byte (lbu) into the record's id byte (+0x78);
                                    //     func_003512C0 compares an id's low byte with it (lw)
    int NumInstances;               // 0x4 [O]; [P] asserts "... < NumInstances"; [R] ctor: the record count (0x40)
    tInstanceList* FreeInstances;   // 0x8 [O]; [R] ctor links every record into it; [U] allocInstance / allocInstance1 /
                                    //     freeInstance pop / search / push it
    tInstanceManRecord* Instances;  // 0xC [O]; [P] asserts "(tInstanceManRecord*)(inst) - Instances"; [R] ctor:
                                    //     operator new(n * 0xB0, "Instance"); [U] a record's index = inst - Instances;
                                    //     the dtor frees it (cMemMan_free)
};

CHECK_OFFSET(cInstanceMan, InstanceSectionId, 0x0);
CHECK_OFFSET(cInstanceMan, NumInstances, 0x4);
CHECK_OFFSET(cInstanceMan, FreeInstances, 0x8);
CHECK_OFFSET(cInstanceMan, Instances, 0xC);
CHECK_SIZE(cInstanceMan, 0x10);

#endif /* OBJECT_INSTANCEMAN_H */
