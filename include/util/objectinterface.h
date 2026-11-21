#ifndef UTIL_OBJECTINTERFACE_H
#define UTIL_OBJECTINTERFACE_H

// cObjectInterface: the world objects' window onto the game - static helpers that world nodes, modifiers and the
// collision code call to run world scripts (runCollideScript / runExpireScript / runOnLoseFocusScript / runInitScript /
// runScript / runFrameUpdateScript), find riders (getBeing(i): SSXApp->Game->AI's rider i + 0x6C0, getAIIndex,
// getPosition, getPeakNumber), reach the world, nash, cameras, flag manager and instance manager, read the update rate
// and AI tick, add an instance to the focus boxes, draw a light halo and test sphere occlusion.
// A STATIC-ONLY class: [O] On Tour's DWARF has `class cObjectInterface` of size 1 with no data members, and
// builds/x/ontour_kor/merged.tsv marks its paired methods `static`; [R] retail never allocates one, there is no
// vtable, and every retail function takes plain values (ids, indices, floats) or another object (a cBeing / rider part,
// an instance record, an avalanche, a bounding box) as its first argument. So there is no struct to lay out and no
// method to retype.
// EA's source: d:/ssx/code/source/util/objectinterface.cpp (preview assert path). Retail code: the decomp's units
// util/menu.cpp (0x002D19B8 .. 0x002D1CDC, 20 functions) and util/objectinterface.cpp (0x002D1CE0 .. 0x002D20F4, 4).
// The functions are not declared here: the 24 units that call them declare them with conflicting types (getWorld as
// void* / char** / sMPWorld**, getNash as void* / char*, runFrameUpdateScript taking int or a list, ...).

// tCommDataBlock: a sized byte blob that the replay / comm serialisers write and read back (views group D: sStr_2478 in
// util/objectinterface, Data_2868 in seg/seg_1D3510). [O] On Tour struct tCommDataBlock {unsigned int Size; char* Data;}
// (0x8, with a constructor and a destructor) and its serialisers put_data(char** dest, tCommDataBlock*& data) /
// get_data(char** src, tCommDataBlock*& data) (util/all_in_one_util.cpp, next to the other put_ / get_ helpers);
// [R] retail get_data allocates the block as cMemMan_alloc(8, "Comm_Data_Block") and the bytes as "Comm_Data", then
// stores Size and Data; put_data writes Size and then the Size bytes. Lives here because the retail put_data is in the
// util/objectinterface unit. Flat: no constructor (get_data's allocation is explicit in retail).
#include "layout.h"

struct tCommDataBlock {
    unsigned int Size;              // 0x0 [O]; [U] put_data / get_data: the byte count
    char* Data;                     // 0x4 [O]; [U] the bytes ("Comm_Data")
};

CHECK_OFFSET(tCommDataBlock, Size, 0x0);
CHECK_OFFSET(tCommDataBlock, Data, 0x4);
CHECK_SIZE(tCommDataBlock, 0x8);

#endif /* UTIL_OBJECTINTERFACE_H */
