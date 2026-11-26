#ifndef MAIN_CROWDMAN3D_H
#define MAIN_CROWDMAN3D_H

// cCrowdMan3d: the 3D crowd manager (name ours: the allocation tag "CrowdMan3d", the global TheCrowdMan3d and
// cGame::CrowdMan3d, main/game.h). Five crowd blocks; each animates one crowd model's members (world instances
// attached by script) with a shared mesh animation. One instance: cGame_load (main/gameload) allocates it and
// cGame_purge destroys it (flags 3). Retail code: object/debouncenode (cCrowdMan3d_cCrowdMan3d 0x00343A68 ..
// func_00344FF8). No On Tour class, no preview assert path.
//
// Size 0x9C4: [R] cGame_load: operator new(2500, "CrowdMan3d") + cCrowdMan3d_cCrowdMan3d, which builds five 0x1F4-byte
// blocks; the destructor cCrowdMan3d__cCrowdMan3d destroys five. No vtable, no base class.
//
// A FLAT layout. Evidence tags: [R] the constructor / destructor and the block reset func_003442E0; [U] how retail code
// uses the field (src/object/debouncenode.cpp). Names are ours.

#include "layout.h"

// One crowd member slot (8 bytes).
struct tCrowdMember3d {
    unsigned int mInstanceId;       // 0x0 [R] the ctor and func_003442E0 store -1 (free); [U] func_00343F38 finds the
                                    //     member by its instance id and frees the slot (-1)
    int mValue;                     // 0x4 [R] func_003442E0 zeroes it
};

CHECK_OFFSET(tCrowdMember3d, mInstanceId, 0x0);
CHECK_OFFSET(tCrowdMember3d, mValue, 0x4);
CHECK_SIZE(tCrowdMember3d, 0x8);

// One crowd block (0x1F4 bytes).
struct tCrowdBlock3d {
    // A free id (name ours): [R] the ctor and func_003442E0 store it in mModelId, mKey and every member's mInstanceId;
    // [U] a block is in use while its mKey is not kFree.
    enum { kFree = 0xFFFFFFFF };
    int mNumMembers;                // 0x000 [R] func_003442E0 zeroes it; [U] func_00343F38 decrements it and empties
                                    //       the block at 0
    unsigned int mModelId;          // 0x004 [R] the ctor and func_003442E0 store -1; [U] func_00344368 stores the
                                    //       crowd model's instance id
    unsigned int mKey;              // 0x008 [R] func_003442E0 stores -1; [U] the block is in use while it is not -1
                                    //       (func_003440C8, func_00344138); the replay functions write / read it
    tCrowdMember3d mMembers[32];    // 0x00C [R] the ctor and func_003442E0 free all 32
    char unk_10C[0xE8];             // 0x10C the animation state ([R] func_003442E0 clears +0x110, +0x114, +0x1DC and
                                    //       +0x1E8..+0x1F0; [U] func_00344368 sets +0x1DC and +0x1E4)
};

CHECK_OFFSET(tCrowdBlock3d, mNumMembers, 0x000);
CHECK_OFFSET(tCrowdBlock3d, mModelId, 0x004);
CHECK_OFFSET(tCrowdBlock3d, mKey, 0x008);
CHECK_OFFSET(tCrowdBlock3d, mMembers, 0x00C);
CHECK_OFFSET(tCrowdBlock3d, unk_10C, 0x10C);
CHECK_SIZE(tCrowdBlock3d, 0x1F4);

struct cCrowdMan3d {
    tCrowdBlock3d mBlocks[5];       // 0x000 [R] the ctor resets all five (func_003442E0); the dtor frees each block's
                                    //       animation data and destroys the five (func_00344348)
};

CHECK_OFFSET(cCrowdMan3d, mBlocks, 0x000);
CHECK_SIZE(cCrowdMan3d, 0x9C4);

#endif /* MAIN_CROWDMAN3D_H */
