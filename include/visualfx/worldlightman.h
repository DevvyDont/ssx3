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
// The irradiance sets are cIrradiance (render/irradiance.h: 0xA0 bytes, copied as 10 qwords; the preview's
// SetIrradiance__15cPS2GraphicsManRC15tIrradianceInfob takes the same block as tIrradianceInfo).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */
#include "render/irradiance.h" /* cIrradiance */
#include "world/worldcache.h" /* tResourceID */

struct cBXNash;
struct cOverlapList;
struct tWorldPainterQuery;

// A world light (On Tour struct tLightRecord : tLightRecord_PSP, 0x68; retail 0x70: [R] cPS2LightMan_CreateLights
// steps its record array by 0x70). It is a Nash space node of type kSNT_Light (6): cOverlapList::typed[] points at
// it (cWorldLightMan_IntersectLights keeps the typed nodes whose type word (+0x8) is 6 and scores them with
// GetIntensity). Retail agrees with On Tour's offsets from 0x14 on; its light type is at 0x10 (On Tour: m_eLightType at
// 0xC, m_iFlags at 0x10). Readers: cWorldLightMan_GetIntensity ([O] GetIntensity(const t4Vector&, const tLightRecord&)),
// cWorldLightMan_CalcLightDist, cWorldLightMan_IntersectLights ([O] ..., tLightRecord** Lights, int Max),
// cPS2LightMan_CreateLights (render/lightman). EA's header for it is not known.
struct tLightRecord {
    int m_iIntersect[3];            // 0x00 [O] (the Nash space-node header: next, prev, type); [U] IntersectLights
                                    //      tests [2] == 6 (kSNT_Light)
    int m_iFlags;                   // 0x0C [O] m_iFlags (On Tour's m_eLightType / m_iFlags pair is swapped here: the
                                    //      halo records keep their flags at 0xC too, see cLightHalo_Process); [U]
                                    //      cChunkResolve_resolveLight / unresolveLight reject a light with 0x100 set
    int m_eLightType;               // 0x10 [O] eNEWLightType (On Tour at 0xC): 0 directional, 1 spot, 2 point,
                                    //      3 ambient; [U] GetIntensity and CreateLights switch on it
    float m_fIntensity;             // 0x14 [O]; [U] GetIntensity, CreateLights
    float m_fRGBIntensity;          // 0x18 [O]; [U] GetIntensity scales by it (and returns it for type 0)
    float m_fMaxDistance;           // 0x1C [O]; [U] GetIntensity (fade past 3750 when >= 5000), CalcLightDist (range)
    float m_fvColor[3];             // 0x20 [O] tf3Vector; [U] CreateLights: the colour's r, g, b
    float m_fvDirection[3];         // 0x2C [O] tf3Vector; [U] CalcLightDist, CreateLights (normalised)
    float m_fvWorldPos[3];          // 0x38 [O] tf3Vector; [U] CalcLightDist, CreateLights
    float m_fvMin[3];               // 0x44 [O] tf3Vector (the bound box)
    float m_fvMax[3];               // 0x50 [O] tf3Vector
    float unk_05C;                  // 0x5C [O] anonymous union (spot cone); [U] GetIntensity: inner cosine; CreateLights:
                                    //      cone angle = pi/2 - func_0031C128(it)
    float unk_060;                  // 0x60 [O] anonymous union; [U] GetIntensity: outer cosine
    signed char m_cDecayRate;       // 0x64 [O] char; [U] GetIntensity falloff power (1..3), CreateLights decay (lb)
    signed char m_cDropOff;         // 0x65 [O] char; [U] GetIntensity cone power, CreateLights dropoff (lb)
    char unk_066[0xA];              // 0x66 padding to the 0x70 record size
};

CHECK_OFFSET(tLightRecord, m_iIntersect, 0x00);
CHECK_OFFSET(tLightRecord, m_iFlags, 0x0C);
CHECK_OFFSET(tLightRecord, m_eLightType, 0x10);
CHECK_OFFSET(tLightRecord, m_fIntensity, 0x14);
CHECK_OFFSET(tLightRecord, m_fRGBIntensity, 0x18);
CHECK_OFFSET(tLightRecord, m_fMaxDistance, 0x1C);
CHECK_OFFSET(tLightRecord, m_fvColor, 0x20);
CHECK_OFFSET(tLightRecord, m_fvDirection, 0x2C);
CHECK_OFFSET(tLightRecord, m_fvWorldPos, 0x38);
CHECK_OFFSET(tLightRecord, m_fvMin, 0x44);
CHECK_OFFSET(tLightRecord, m_fvMax, 0x50);
CHECK_OFFSET(tLightRecord, unk_05C, 0x5C);
CHECK_OFFSET(tLightRecord, unk_060, 0x60);
CHECK_OFFSET(tLightRecord, m_cDecayRate, 0x64);
CHECK_OFFSET(tLightRecord, m_cDropOff, 0x65);
CHECK_OFFSET(tLightRecord, unk_066, 0x66);
CHECK_SIZE(tLightRecord, 0x70);

// A cached world-light overlap list around a point (On Tour tOverlapListNode, 0x40; same layout in retail).
struct tOverlapListNode {
    tResourceID RID;                // 0x000 [O] tResourceID (world/worldcache.h; Invalid() = 0xFFFFFFFF); [R] ctor and
                                    //       initLightCache set -1; [U] flushOverlapCache sets -1, evictOverlapNode
                                    //       takes a -1 node first (RID == tResourceID::Invalid(), a word compare),
                                    //       getRealTimeLights stores the owning entry's id
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
    cIrradiance RTlightInfo;        // 0x010 [O] (On Tour tRTLightInfo; retail a cIrradiance, render/irradiance.h); [U] getRealTimeLights
                                    //       copies ObjIrradiance here, adds the 4 strongest lights
                                    //       (IntersectLights1) and returns its address
    t4Vector pos;                   // 0x0B0 [O]; [R] initLightCache t4vect_Zero; [U] getRealTimeLights: the position
                                    //       the entry was lit at (relit after a move of 10, reloaded after 500)
    tOverlapListNode* pOverlapNode; // 0x0C0 [O]; [R] initLightCache 0; [U] getRealTimeLights takes it from
                                    //       evictOverlapNode
    char unk_0C4[0xC];              // 0x0C4 padding
    cIrradiance ObjIrradiance;      // 0x0D0 [O] cIrradiance (On Tour's PSP one is 0x44; retail's 0xA0); [U] getRealTimeLights
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
