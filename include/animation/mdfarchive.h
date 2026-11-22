#ifndef ANIMATION_MDFARCHIVE_H
#define ANIMATION_MDFARCHIVE_H

// cMdfArchive: an animation (MDF) archive in memory: the raw archive, its header, the model headers and the model data.
// Methods in animation/animsequencer and animation/mdfarchive.
//
// Size 0x10 (On Tour's; retail's methods reach Data at size - 4). Member names, order and offsets are EA's, from SSX On
// Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start (tools/gen/self_offsets.py).

#include "layout.h"

struct tMdfArchive;
struct tMdfModelHeader;

// The header of a loaded MDF (model) archive (On Tour struct tMdfArchive, 0xC). [O] cMdfArchive::linkArchive(const
// tMdfArchive* archive); [U] cMdfArchive_linkArchive: ModelHeaders = archive + mHeaderOffset (lh +0x6), Data = archive +
// mBodyOffset (lw +0x8); cMdfArchive_findModelPart / getModelPartPtr loop over mNumModels (lh +0x4).
struct tMdfArchive {
    int unk_00;                     // 0x0 [O] mCookie (not read in retail)
    short mNumModels;               // 0x4 [O]; [U] findModelPart / getModelPartPtr loop bound
    short mHeaderOffset;            // 0x6 [O]; [U] linkArchive: ModelHeaders = archive + it
    int mBodyOffset;                // 0x8 [O]; [U] linkArchive: Data = archive + it
};

CHECK_OFFSET(tMdfArchive, unk_00, 0x0);
CHECK_OFFSET(tMdfArchive, mNumModels, 0x4);
CHECK_OFFSET(tMdfArchive, mHeaderOffset, 0x6);
CHECK_OFFSET(tMdfArchive, mBodyOffset, 0x8);
CHECK_SIZE(tMdfArchive, 0xC);

struct cMdfArchive {
    void* Archive;                  // 0x000 [O]
    tMdfArchive* ArchiveHeader;     // 0x004 [O]
    tMdfModelHeader* ModelHeaders;  // 0x008 [O]
    char* Data;                     // 0x00C [O]
};

CHECK_OFFSET(cMdfArchive, Archive, 0x000);
CHECK_OFFSET(cMdfArchive, ArchiveHeader, 0x004);
CHECK_OFFSET(cMdfArchive, ModelHeaders, 0x008);
CHECK_OFFSET(cMdfArchive, Data, 0x00C);
CHECK_SIZE(cMdfArchive, 0x10);

#endif
