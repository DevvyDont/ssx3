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

// One model part's header in an MDF archive (On Tour struct tMdfModelHeader : tMsfModelHeader, 0x50 there; 0x60 here).
// cMdfArchive::ModelHeaders points at an array of them. [U] cMdfArchive_getModelPartByIndex copies the 0x60-byte header
// of part i (stride 0x60), tags the copy "MdfModel <mName>" and unpacks mDataSize bytes from Data + mDataOffset behind
// it (view sMdfPart_00314CB0); cAnimModel_addModelPartLOD takes the bone and hot-spot tables at data + mOffsetBones /
// mOffsetHotSpots. Retail agrees with On Tour through 0x1C; its counts sit 8 bytes later than On Tour's (numBones at
// 0x4A, not 0x42), so the rest is not placed. [Staged by views2 group A.]
struct tMdfModelHeader {
    char mName[16];                 // 0x00 [O]; [U] getModelPartByIndex: the "MdfModel %s" tag
    int mDataOffset;                // 0x10 [O]; [U] getModelPartByIndex: Data + it
    int mDataSize;                  // 0x14 [O]; [U] getModelPartByIndex: the unpacked size (0: no data)
    int mOffsetBones;               // 0x18 [O]; [U] addModelPartLOD: the tMdfBone table (sAnimModelPart::boneNames)
    int mOffsetHotSpots;            // 0x1C [O]; [U] addModelPartLOD: the tMdfHotSpot table (sAnimModelPart +0x3C)
    char unk_020[0x40];             // 0x20 [O] On Tour's mOffsetMorphTargs .. mFlags, laid out differently here
};

CHECK_OFFSET(tMdfModelHeader, mName, 0x00);
CHECK_OFFSET(tMdfModelHeader, mDataOffset, 0x10);
CHECK_OFFSET(tMdfModelHeader, mDataSize, 0x14);
CHECK_OFFSET(tMdfModelHeader, mOffsetBones, 0x18);
CHECK_OFFSET(tMdfModelHeader, mOffsetHotSpots, 0x1C);
CHECK_OFFSET(tMdfModelHeader, unk_020, 0x20);
CHECK_SIZE(tMdfModelHeader, 0x60);

// One bone of a model part (On Tour struct tMdfBone, 0x50). [U] cAnimModel_transform walks a part's bone table (stride
// 0x50) and reads the parent index at 0x12 (view sAmBoneInfo_0200); cAnimModel_getBoneSkelIndex / getGraftMask compare
// the names. [Staged by views2 group A.]
struct tMdfBone {
    char mName[16];                 // 0x00 [O]; [U] getBoneSkelIndex compares it
    short unk_010;                  // 0x10 [O] mParentPartId (no retail read seen)
    short mParentId;                // 0x12 [O]; [U] cAnimModel_transform: the parent bone (lh 0x12)
    char unk_014[0x3C];             // 0x14 [O] mChanMask, mFlipSourceChanID, mFlipQuat, padding, mOffset[4],
                                    //      mQuaternion[4], mFlipOffsetScale[4]
};

CHECK_OFFSET(tMdfBone, mName, 0x00);
CHECK_OFFSET(tMdfBone, unk_010, 0x10);
CHECK_OFFSET(tMdfBone, mParentId, 0x12);
CHECK_OFFSET(tMdfBone, unk_014, 0x14);
CHECK_SIZE(tMdfBone, 0x50);

// One hot spot of a model part (On Tour struct tMdfHotSpot, 0x10: a point fixed to a bone). [U]
// cAnimModel_transformHotSpot reads entry index of the part's table (index << 4): the offset x, y, z and the bone (view
// sAmPoint_0458). [Staged by views2 group A.]
struct tMdfHotSpot {
    float mOffset[3];               // 0x00 [O]; [U] transformHotSpot: the point in the bone's space
    int mBoneId;                    // 0x0C [O]; [U] transformHotSpot: the bone it is fixed to
};

CHECK_OFFSET(tMdfHotSpot, mOffset, 0x00);
CHECK_OFFSET(tMdfHotSpot, mBoneId, 0x0C);
CHECK_SIZE(tMdfHotSpot, 0x10);

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
