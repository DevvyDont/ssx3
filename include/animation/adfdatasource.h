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

// One tag of an animation (On Tour struct tAdfTag, 0x4); cAdfDataSource::Tags points at the archive's table, an anim's
// tags are mNumTags entries from its mTagIndex. [U] cRiderAnimBase_getAIAnimEventTime reads the u16 at +0x0 of tag
// mTagIndex + n (a frame, / 30 for seconds; view sAnimKey00312820). [Staged by views2 group A.]
struct tAdfTag {
    unsigned short mTagFrame;           // 0x0 [O]; [U] getAIAnimEventTime: the tag's frame
    unsigned short mTagID;              // 0x2 [O] (no retail read seen)
};

CHECK_OFFSET(tAdfTag, mTagFrame, 0x0);
CHECK_OFFSET(tAdfTag, mTagID, 0x2);
CHECK_SIZE(tAdfTag, 0x4);

// One decompression channel (On Tour struct tCmpChannel, 0x14: eChanType channelType, a 0x10-byte union uParam).
// [R] gGenCacheBucket gives each cache node numChan of them ("AnimChanCache"); [U] dcmpInitFrameAccessor indexes them
// with a 0x14 stride. [Staged by views2 group A.]
struct tCmpChannel {
    int channelType;                    // 0x00 [O] eChanType (a word)
    char uParam[0x10];                  // 0x04 [O] union __C1 (per channel type)
};

CHECK_OFFSET(tCmpChannel, channelType, 0x00);
CHECK_OFFSET(tCmpChannel, uParam, 0x04);
CHECK_SIZE(tCmpChannel, 0x14);

// A decompressor's view of one chunk (On Tour struct tCmpFrameAccessor, 0x18; [O] dcmpInitFrameAccessor and
// dcmpGetFrame(const tCmpFrameAccessor*, float findex, int startChan, int numChan, float* frameBuf)). [U]
// dcmpInitFrameAccessor reads mChannels, stores the four data pointers and counts the channel kinds into the four
// bytes.
// [Staged by views2 group A.]
struct tCmpFrameAccessor {
    tCmpChannel* mChannels;             // 0x00 [O]; [U] dcmpInitFrameAccessor (lw 0x0, 0x14 per channel)
    unsigned char* mDataQB;             // 0x04 [O]; [U] dcmpInitFrameAccessor (sw)
    unsigned char* mDataQB2;            // 0x08 [O]; [U] ditto
    unsigned short* mDataQS;            // 0x0C [O]; [U] ditto
    float* mDataRaw;                    // 0x10 [O]; [U] ditto
    unsigned char mNumQB;               // 0x14 [O]; [U] dcmpInitFrameAccessor zeroes and counts it
    unsigned char mNumQB2;              // 0x15 [O]; [U] ditto
    unsigned char mNumQS;               // 0x16 [O]; [U] ditto
    unsigned char mNumRaw;              // 0x17 [O]; [U] ditto
};

CHECK_OFFSET(tCmpFrameAccessor, mChannels, 0x00);
CHECK_OFFSET(tCmpFrameAccessor, mDataQB, 0x04);
CHECK_OFFSET(tCmpFrameAccessor, mDataQB2, 0x08);
CHECK_OFFSET(tCmpFrameAccessor, mDataQS, 0x0C);
CHECK_OFFSET(tCmpFrameAccessor, mDataRaw, 0x10);
CHECK_OFFSET(tCmpFrameAccessor, mNumQB, 0x14);
CHECK_OFFSET(tCmpFrameAccessor, mNumQB2, 0x15);
CHECK_OFFSET(tCmpFrameAccessor, mNumQS, 0x16);
CHECK_OFFSET(tCmpFrameAccessor, mNumRaw, 0x17);
CHECK_SIZE(tCmpFrameAccessor, 0x18);

// One node of a part's channel cache (On Tour struct tCmpChanCache, 0x24; [O] tCmpChanCache* gGenCacheBucket(int
// partID, int numChan)). gChanCache[partID] holds 20 nodes in a ring, gChanCacheLRU[partID] the next to reuse. [R]
// gGenCacheBucket links prev / next, zeroes animChunk and allocates cmpAccess.mChannels; [U]
// cAdfDataSource_getFramePartData stores the owning chunk in animChunk, resets the old owner's mCacheSlot, passes
// &cmpAccess to dcmpInitFrameAccessor / dcmpGetFrame and relinks prev / next (views sAnimNode_310F18,
// sAnimNode_311110). [Staged by views2 group A.]
struct tCmpChanCache {
    tAdfAnimChunk* animChunk;           // 0x00 [O]; [R] 0 (unowned); [U] getFramePartData stores the chunk
    tCmpFrameAccessor cmpAccess;        // 0x04 [O]; [U] getFramePartData passes its address to dcmpGetFrame
    tCmpChanCache* prev;                // 0x1C [O]; [R] gGenCacheBucket links the ring
    tCmpChanCache* next;                // 0x20 [O]; [R] ditto
};

CHECK_OFFSET(tCmpChanCache, animChunk, 0x00);
CHECK_OFFSET(tCmpChanCache, cmpAccess, 0x04);
CHECK_OFFSET(tCmpChanCache, prev, 0x1C);
CHECK_OFFSET(tCmpChanCache, next, 0x20);
CHECK_SIZE(tCmpChanCache, 0x24);

struct cAdfDataSource {
    void* Data;                     // 0x000 [O]
    tAdfArchive* Archive;           // 0x004 [O]
    tAdfAnimHeader* Anims;          // 0x008 [O]
    tAdfAnimChunk* Chunks;          // 0x00C [O]
    tAdfTag* Tags;                  // 0x010 [O]
    unsigned char* ChannelData;     // 0x014 [O]

    // Methods bound to their retail symbols by asm labels.
    void populateFromData(tAdfArchive* data) __asm__("cAdfDataSource_populateFromData");
};

CHECK_OFFSET(cAdfDataSource, Data, 0x000);
CHECK_OFFSET(cAdfDataSource, Archive, 0x004);
CHECK_OFFSET(cAdfDataSource, Anims, 0x008);
CHECK_OFFSET(cAdfDataSource, Chunks, 0x00C);
CHECK_OFFSET(cAdfDataSource, Tags, 0x010);
CHECK_OFFSET(cAdfDataSource, ChannelData, 0x014);
CHECK_SIZE(cAdfDataSource, 0x18);

#endif
