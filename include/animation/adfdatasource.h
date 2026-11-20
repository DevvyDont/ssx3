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
