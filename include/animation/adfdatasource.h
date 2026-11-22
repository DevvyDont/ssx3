#ifndef ANIMATION_ADFDATASOURCE_H
#define ANIMATION_ADFDATASOURCE_H

// cAdfDataSource: one loaded ADF animation archive (headers, chunks, tags and channel data); allocated with
// cMemMan_alloc(0x18, "cAdfDataSource").
// Size 0x18 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct tAdfAnimChunk;
struct tAdfAnimHeader;
struct tAdfArchive;
struct tAdfTag;

// The header of a loaded ADF archive (On Tour struct tAdfArchive, 0x10). [U] cAdfDataSource_populateFromData (animation/
// animmodel): Anims = archive + 0x10, Chunks / Tags / ChannelData = archive + the words at +0x4 / +0x8 / +0xC ("the
// release build drops On Tour's cookie and header checks"); cGameAnimMap_addSource / _remSource (animation/mdfarchive)
// loop over mNumAnims (lh +0x2).
struct tAdfArchive {
    char unk_00;                        // 0x0 [O] mHeader (not read in retail)
    unsigned char unk_01;               // 0x1 [O] mCookie (not read in retail)
    short mNumAnims;                    // 0x2 [O]; [U] addSource / remSource loop bound
    unsigned int mAnimChunkOffset;      // 0x4 [O]; [U] populateFromData: Chunks = archive + it
    unsigned int mTagOffset;            // 0x8 [O]; [U] populateFromData: Tags = archive + it
    unsigned int mChannelOffset;        // 0xC [O]; [U] populateFromData: ChannelData = archive + it
};

CHECK_OFFSET(tAdfArchive, unk_00, 0x0);
CHECK_OFFSET(tAdfArchive, unk_01, 0x1);
CHECK_OFFSET(tAdfArchive, mNumAnims, 0x2);
CHECK_OFFSET(tAdfArchive, mAnimChunkOffset, 0x4);
CHECK_OFFSET(tAdfArchive, mTagOffset, 0x8);
CHECK_OFFSET(tAdfArchive, mChannelOffset, 0xC);
CHECK_SIZE(tAdfArchive, 0x10);

// One animation's header in an ADF archive (On Tour struct tAdfAnimHeader, 0x14); cAdfDataSource::Anims points at an
// array of them. [U] indexed by an anim id >> 8 times 0x14 (rideranimbase getAIAnimDuration / getAIAnimEventTime /
// onInput, animsequencer, mdfarchive addSource / remSource; view sSeqAM in animation/animmodel getFramePartData).
struct tAdfAnimHeader {
    unsigned int mHashName;             // 0x00 [O]; [U] cGameAnimMap_addSource / remSource map this word (setMap)
    unsigned int mAnimChunkIndex;       // 0x04 [O]; [U] getFramePartData: the first chunk entry (sSeqAM::base x 8)
    unsigned short unk_08;              // 0x08 [O] mNumParts (no retail read seen)
    unsigned short mNumChunksPerPart;   // 0x0A [O]; [U] getFramePartData: chunks per part (sSeqAM::a)
    unsigned short mNumAnimFrames;      // 0x0C [O]; [U] getAIAnimDuration: (frames - 1) / 30; getFramePartData (sSeqAM::n)
    unsigned short mNumFramesPerChunk;  // 0x0E [O]; [U] getFramePartData: frames per chunk (sSeqAM::b)
    unsigned short mTagIndex;           // 0x10 [O]; [U] getAIAnimEventTime / onInput: first entry in Tags
    unsigned short mNumTags;            // 0x12 [O]; [U] getAIAnimEventTime / onInput: the tag count
};

CHECK_OFFSET(tAdfAnimHeader, mHashName, 0x00);
CHECK_OFFSET(tAdfAnimHeader, mAnimChunkIndex, 0x04);
CHECK_OFFSET(tAdfAnimHeader, unk_08, 0x08);
CHECK_OFFSET(tAdfAnimHeader, mNumChunksPerPart, 0x0A);
CHECK_OFFSET(tAdfAnimHeader, mNumAnimFrames, 0x0C);
CHECK_OFFSET(tAdfAnimHeader, mNumFramesPerChunk, 0x0E);
CHECK_OFFSET(tAdfAnimHeader, mTagIndex, 0x10);
CHECK_OFFSET(tAdfAnimHeader, mNumTags, 0x12);
CHECK_SIZE(tAdfAnimHeader, 0x14);

// One compressed channel chunk of an animation part (On Tour struct tAdfAnimChunk, 0x8); cAdfDataSource::Chunks points
// at an array of them, mNumChunksPerPart per part from the anim's mAnimChunkIndex. [U] cAdfDataSource_getFramePartData
// (animation/animmodel; view sEntAM): ChannelData + mChannelIndex is the chunk's stream, mPartID picks the per-part
// decompression cache pool (gChanCache[mPartID]), mCacheSlot is the cached slot (0xFF = none; the LRU eviction resets the
// old owner's to 0xFF), mNumChunkFrames is passed to dcmpInitFrameAccessor.
struct tAdfAnimChunk {
    unsigned int mChannelIndex;         // 0x0 [O]; [U] getFramePartData: ChannelData + it (sEntAM::off)
    unsigned char mPartID;              // 0x4 [O]; [U] getFramePartData: gChanCache[mPartID] (sEntAM::kind)
    unsigned char mCacheSlot;           // 0x5 [O]; [U] getFramePartData: 0xFF = not cached (sEntAM::slot)
    unsigned short mNumChunkFrames;     // 0x6 [O]; [U] getFramePartData: dcmpInitFrameAccessor's frame count (sEntAM::len)
};

CHECK_OFFSET(tAdfAnimChunk, mChannelIndex, 0x0);
CHECK_OFFSET(tAdfAnimChunk, mPartID, 0x4);
CHECK_OFFSET(tAdfAnimChunk, mCacheSlot, 0x5);
CHECK_OFFSET(tAdfAnimChunk, mNumChunkFrames, 0x6);
CHECK_SIZE(tAdfAnimChunk, 0x8);

struct cAdfDataSource {
    void* Data;                     // 0x000 [O]
    tAdfArchive* Archive;           // 0x004 [O]
    tAdfAnimHeader* Anims;          // 0x008 [O]
    tAdfAnimChunk* Chunks;          // 0x00C [O]
    tAdfTag* Tags;                  // 0x010 [O]
    unsigned char* ChannelData;     // 0x014 [O]
};

CHECK_OFFSET(cAdfDataSource, Data, 0x000);
CHECK_OFFSET(cAdfDataSource, Archive, 0x004);
CHECK_OFFSET(cAdfDataSource, Anims, 0x008);
CHECK_OFFSET(cAdfDataSource, Chunks, 0x00C);
CHECK_OFFSET(cAdfDataSource, Tags, 0x010);
CHECK_OFFSET(cAdfDataSource, ChannelData, 0x014);
CHECK_SIZE(cAdfDataSource, 0x18);

#endif
