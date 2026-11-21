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

#endif /* UTIL_OBJECTINTERFACE_H */
