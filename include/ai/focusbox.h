#ifndef AI_FOCUSBOX_H
#define AI_FOCUSBOX_H

// cFocusMan: the AI's focus manager - keeps up to 6 focus boxes (riders, the script engine's focus box) and the list of
// world instances currently "in focus" around them. update walks the game's nash cell tree when a box moved 2000 or
// more (or every 20 AI ticks), collects the instances in range (FocusQuery_addcell), flags them (0x100 in focus, 0x200
// pass parity) and runs cWScriptMan_up/downFocusScript for those that entered / left focus. One instance in
// cAI::FocusMan (ai/ai.h, 0xA4). EA's source: d:/ssx/code/source/ai/focusbox.cpp (10 of the preview's 12 references
// to that assert path are inside its FocusQuery::addcell and cFocusMan methods, 0x00101380 .. 0x00103240); retail code
// is in the decomp's unit seg/seg_1218.cpp (0x00101B60 .. 0x0010369C).
// Size 0x8E0: [R] cAI_cAI does cMemMan_alloc(0x8E0, "FocusMan", 0, 0) and constructs it inline: NumBoxes = 0, six
// empty FocusRecord ctors, LastUpdateTick = -1, IsEven = 1 (a word: sw), NumFocusInstances = 0. cAI's destructor frees
// it with plain operator delete. No base classes, no vtable.
//
// A FLAT layout, like main/game.h. Tags: [R] the inline constructor and allocation in cAI_cAI; [U] how retail code uses
// the field (seg/seg_1218.cpp, the asm); [O] SSX On Tour's DWARF (class cFocusMan, size 0x8E0; struct FocusRecord,
// size 0x20): the same members at the same offsets (On Tour's bool IsEven is padded to the next int anyway).
// No preview evidence (no accessors; layout_xlate has no observations).

#include "layout.h"
#include "bx/math3d.h"   // t4Vector

struct cFocusBox;          // a focus box; [U] its first 16 bytes are the position cFocusMan_addbox copies (lq)
struct tInstanceRecord;    // a world instance record; [U] +0x8 flags (0x100 in focus, 0x200 parity), +0x78 section id (byte)

// One focus box slot. [O] struct FocusRecord (size 0x20: Box at 0x0, LastUpdatePos at 0x10).
struct FocusRecord {
    cFocusBox* Box;                 // 0x000 [O]; [U] cFocusMan_addbox stores the box; removebox looks it up; update reads
                                    //       the box's position through it
    char unk_004[0xC];              // 0x004 padding (t4Vector alignment)
    t4Vector LastUpdatePos;         // 0x010 [O]; [U] addbox copies the box's position into it (lq/sq); update compares the
                                    //       current position with it (moved >= 2000) and stores it after a pass
};
CHECK_OFFSET(FocusRecord, Box, 0x0);
CHECK_OFFSET(FocusRecord, unk_004, 0x4);
CHECK_OFFSET(FocusRecord, LastUpdatePos, 0x10);
CHECK_SIZE(FocusRecord, 0x20);

struct cFocusMan {
    int NumBoxes;                   // 0x000 [O]; [R] 0; [U] addbox appends at it, removebox shifts the boxes down and
                                    //       decrements it; update / write/readFromReplayFrame loop over it
    char unk_004[0xC];              // 0x004 padding (FocusRecord is 16-aligned)
    FocusRecord Boxes[6];           // 0x010 [O]; [R] six FocusRecord ctors (empty); [U] addbox / removebox / update;
                                    //       writeToReplayFrame / readFromReplayFrame save 0x10 bytes of each
    int LastUpdateTick;             // 0x0D0 [O]; [R] -1; [U] update stores the AI tick (negative = no pass yet); reset /
                                    //       addbox / removebox set -1 to force a pass; saved in the replay frame
    int IsEven;                     // 0x0D4 [O] (bool; a word: sw 1 in the ctor); [U] update flips it (the 0x200 parity);
                                    //       addfocusinstance tests it; readFromReplayFrame sets 1
    int NumFocusInstances;          // 0x0D8 [O]; [R] 0; [U] addfocusinstance appends, purgesection compacts, reset zeroes it
    tInstanceRecord* FocusInstances[512]; // 0x0DC [O]; [U] update rebuilds it (MEM_copy of the new list); purgesection
                                    //       drops the instances of a section (+0x78); saved / restored in the replay frame
};
CHECK_OFFSET(cFocusMan, NumBoxes, 0x0);
CHECK_OFFSET(cFocusMan, unk_004, 0x4);
CHECK_OFFSET(cFocusMan, Boxes, 0x10);
CHECK_OFFSET(cFocusMan, LastUpdateTick, 0xD0);
CHECK_OFFSET(cFocusMan, IsEven, 0xD4);
CHECK_OFFSET(cFocusMan, NumFocusInstances, 0xD8);
CHECK_OFFSET(cFocusMan, FocusInstances, 0xDC);
CHECK_SIZE(cFocusMan, 0x8E0);

#endif /* AI_FOCUSBOX_H */
