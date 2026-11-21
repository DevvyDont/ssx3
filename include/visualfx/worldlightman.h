#ifndef VISUALFX_WORLDLIGHTMAN_H
#define VISUALFX_WORLDLIGHTMAN_H

// cWorldLightMan: the world light manager. It caches, per lit instance, the real-time lighting (an irradiance set plus
// the strongest nearby world lights), keyed by the instance's resource id in a 32-chain hash, and keeps 8 overlap
// lists (the world lights around a cached position) that the entries share. The game's instance is
// cGame::WorldLightMan (main/game.h, +0x90). EA's source: d:/ssx/code/source/visualfx/worldlightman.cpp (preview assert
// strings); retail code: the ctor and dtor in seg/seg_1F1548 (0x2F5180, 0x2F5230), the rest in visualfx/worldlightman
// (cWorldLightMan_initLightCache 0x2F5298 ... cWorldLightMan_CalcLightDist 0x2F6168).
//
// Size 0x32A0: [R] cGame's load allocates it with operator new(0x32A0, "WorldLightMan") + cWorldLightMan_cWorldLightMan
// (src/main/gameload.cpp); the last field, the entry count at 0x3290, plus the 16-byte alignment gives 0x32A0.
// [O] On Tour's class is 0x14A0 with the same members in the same order; its cache entries are 0x90 (retail 0x180: two
// 0xA0-byte irradiance sets instead of a tRTLightInfo and a 0x44-byte cIrradiance), so offsets from 0x210 on differ.
// Base: none, no virtuals (no vptr).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail's constructor / destructor / initLightCache and the allocation size;
//   [U] how retail code uses the field (the C bodies in src/visualfx/worldlightman.cpp and src/seg/seg_1F1548.cpp and
//       their asm);
//   [O] SSX On Tour's DWARF (cWorldLightMan, tOverlapListNode, tLightCacheEntry): names, types and order.
// The irradiance sets are EA's tIrradianceInfo (type name from the preview's
// SetIrradiance__15cPS2GraphicsManRC15tIrradianceInfob; 0xA0 bytes, copied as 10 qwords), not laid out yet.

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cBXNash;
struct cOverlapList;
struct tWorldPainterQuery;

// A cached world-light overlap list around a point (On Tour tOverlapListNode, 0x40; same layout in retail).
struct tOverlapListNode {
    unsigned int RID;               // 0x000 [O] tResourceID (a word; Invalid() = 0xFFFFFFFF); [R] ctor and
                                    //       initLightCache set -1; [U] flushOverlapCache sets -1, evictOverlapNode
                                    //       takes a -1 node first, getRealTimeLights stores the owning entry's id
    int iTick;                      // 0x004 [O]; [R] initLightCache 0; [U] getRealTimeLights stores the app tick;
                                    //       evictOverlapNode picks the oldest
    char unk_008[0x8];              // 0x008 padding (tBoundBox is 16-aligned)
    tBoundBox box;                  // 0x010 [O]; [U] getRealTimeLights tests the entry position against it and
                                    //       rebuilds it as a 2000-unit cube around the position
    cOverlapList* pOverlapList;     // 0x030 [O]; [R] initLightCache allocates it (cMemMan_alloc(0xA14,
                                    //       "RTLOverlapList"), On Tour cOverlapList 0xA14); [U] purgeLightCache frees it,
                                    //       getRealTimeLights fills it (cOverlapList_gettyped), IntersectLights walks it
    char unk_034[0xC];              // 0x034 padding
};

CHECK_OFFSET(tOverlapListNode, RID, 0x000);
CHECK_OFFSET(tOverlapListNode, iTick, 0x004);
CHECK_OFFSET(tOverlapListNode, unk_008, 0x008);
CHECK_OFFSET(tOverlapListNode, box, 0x010);
CHECK_OFFSET(tOverlapListNode, pOverlapList, 0x030);
CHECK_OFFSET(tOverlapListNode, unk_034, 0x034);
CHECK_SIZE(tOverlapListNode, 0x40);

// One instance's cached lighting (On Tour tLightCacheEntry, 0x90; retail 0x180).
struct tLightCacheEntry {
    unsigned int RID;               // 0x000 [O] tResourceID; [R] ctor and initLightCache set -1; [U] getRealTimeLights
                                    //       matches the instance id (+0x78); evictHashTableEntry hashes it (it loads
                                    //       RID and iTick as one doubleword, see the report)
    int iTick;                      // 0x004 [O]; [R] initLightCache 0; [U] getRealTimeLights stores the app tick;
                                    //       evictHashTableEntry picks the lowest
    char unk_008[0x8];              // 0x008 padding
    char RTlightInfo[0xA0];         // 0x010 [O] (On Tour tRTLightInfo; retail a tIrradianceInfo); [U] getRealTimeLights
                                    //       copies ObjIrradiance here, adds the 4 strongest lights
                                    //       (IntersectLights1) and returns its address
    t4Vector pos;                   // 0x0B0 [O]; [R] initLightCache t4vect_Zero; [U] getRealTimeLights: the position
                                    //       the entry was lit at (relit after a move of 10, reloaded after 500)
    tOverlapListNode* pOverlapNode; // 0x0C0 [O]; [R] initLightCache 0; [U] getRealTimeLights takes it from
                                    //       evictOverlapNode
    char unk_0C4[0xC];              // 0x0C4 padding
    char ObjIrradiance[0xA0];       // 0x0D0 [O] (On Tour cIrradiance; retail a tIrradianceInfo); [U] getRealTimeLights
                                    //       loads it from the world painter's object / light irradiance name, the
                                    //       default, or the custom irradiance
    tLightCacheEntry* pNext;        // 0x170 [O]; [R] initLightCache 0; [U] the hash chain (getRealTimeLights,
                                    //       evictHashTableEntry)
    tLightCacheEntry* pPrev;        // 0x174 [O]; [R] initLightCache 0; [U] ditto
    char unk_178[0x8];              // 0x178 padding
};

CHECK_OFFSET(tLightCacheEntry, RID, 0x000);
CHECK_OFFSET(tLightCacheEntry, iTick, 0x004);
CHECK_OFFSET(tLightCacheEntry, unk_008, 0x008);
CHECK_OFFSET(tLightCacheEntry, RTlightInfo, 0x010);
CHECK_OFFSET(tLightCacheEntry, pos, 0x0B0);
CHECK_OFFSET(tLightCacheEntry, pOverlapNode, 0x0C0);
CHECK_OFFSET(tLightCacheEntry, unk_0C4, 0x0C4);
CHECK_OFFSET(tLightCacheEntry, ObjIrradiance, 0x0D0);
CHECK_OFFSET(tLightCacheEntry, pNext, 0x170);
CHECK_OFFSET(tLightCacheEntry, pPrev, 0x174);
CHECK_OFFSET(tLightCacheEntry, unk_178, 0x178);
CHECK_SIZE(tLightCacheEntry, 0x180);

struct cWorldLightMan {
    cBXNash* m_pNash;               // 0x000 [O]; [R] the ctor's pNash (cGame's BXNash); [U] initLightCache gives each
                                    //       overlap list it
    tWorldPainterQuery* m_pWPQuery; // 0x004 [O]; [R] ctor: cWorldPainterMan_getQuery(world painter, kWPR_Lighting);
                                    //       the dtor deletes it (virtual dtor); [U] getRealTimeLights queries it at
                                    //       the entry position (slot 2) and reads its irradiance names
    char unk_008[0x8];              // 0x008 padding (tOverlapListNode is 16-aligned)
    tOverlapListNode OverlapNodeList[8]; // 0x010 [O]; [R] ctor/initLightCache (see the node); [U] flushOverlapCache,
                                    //       evictOverlapNode, purgeLightCache
    tLightCacheEntry LightCacheArray[32]; // 0x210 [O]; [R] ctor/initLightCache; [U] getRealTimeLights takes the next
                                    //       free one, evictHashTableEntry the least recently used
    tLightCacheEntry* LightCacheHashTable[32]; // 0x3210 [O]; [R] initLightCache zeroes; [U] getRealTimeLights: chain
                                    //       (RID >> 8) & 0x1F; evictHashTableEntry unlinks from it
    unsigned int m_uiNumLightCacheEntry; // 0x3290 [O]; [R] initLightCache 0; [U] getRealTimeLights counts up to 32
    char unk_3294[0xC];             // 0x3294 no retail access (On Tour's next member is unsigned int
                                    //       m_uiNumOverlapList), then padding to the 16-aligned size
};

CHECK_OFFSET(cWorldLightMan, m_pNash, 0x000);
CHECK_OFFSET(cWorldLightMan, m_pWPQuery, 0x004);
CHECK_OFFSET(cWorldLightMan, unk_008, 0x008);
CHECK_OFFSET(cWorldLightMan, OverlapNodeList, 0x010);
CHECK_OFFSET(cWorldLightMan, LightCacheArray, 0x210);
CHECK_OFFSET(cWorldLightMan, LightCacheHashTable, 0x3210);
CHECK_OFFSET(cWorldLightMan, m_uiNumLightCacheEntry, 0x3290);
CHECK_OFFSET(cWorldLightMan, unk_3294, 0x3294);
CHECK_SIZE(cWorldLightMan, 0x32A0);

#endif /* VISUALFX_WORLDLIGHTMAN_H */
