#ifndef ANIMATION_GAMEANIMMAP_H
#define ANIMATION_GAMEANIMMAP_H

// cGameAnimMap: the game's table from its 518 named game animations (GameAnimNames, cGameAnimMap_GameAnimNames: one string per
// "gad" index) to the loaded animation clips (tAnimID: bank/source id + clip index). initMap hashes the names
// (GetHashValue32) and sorts an index by hash; addSource / remSource register or clear every clip of a loaded ADF bank
// (cAdfDataSource) by its name hash (lookupGAD = binary search, setMap); the rider animation code then reads
// GameAnimMap[gad] directly (the inline getMap). One instance: cGameAnimMap::TheGameAnimMap (cGameAnimMap_TheGameAnimMap,
// gp-relative), set by the ctor and cleared by the dtor. EA's source: d:/ssx/code/source/animation/gameanimmap.cpp and
// gameanimmap.h (preview asserts); retail methods are in the animation/mdfarchive splat unit (0x00314D98..0x003150B0)
// plus the empty testResolve in animation/gameanimmap (0x003150B0). No vtable (no virtual dtor: the dtor is called
// directly).
// Size 0x1848: [R] cSSXApp's init allocates cMemMan_alloc(6216 = 0x1848, "GameAnimMap") + cGameAnimMap_cGameAnimMap
// (main/ssxapp.cpp); three 518-word arrays (initMap / lookupGAD / getAnimName loop to 518 = 0x206) end there:
// 0x1030 + 518 * 4 = 0x1848. [O] On Tour's cGameAnimMap is 0x25A4: the same three arrays with 803 entries.
// No base class.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the allocation and the ctor (its empty tAnimID array-ctor loop counts 0x205 down);
//   [U] how retail code uses the field (src/animation/mdfarchive.cpp: initMap, lookupGAD, setMap and getAnimName use
//       the three arrays; src/animation/rideranimbase.cpp reads TheGameAnimMap + 0x1030 + 4 * gad and splits it into
//       SourceID (& 0xFF) and AnimID (>> 8));
//   [O] SSX On Tour's DWARF (class cGameAnimMap: GameAnimHashes, GameAnimSort, GameAnimMap; struct tAnimID
//       {SourceID : 8, AnimID : 24}): same member order, 803 entries instead of 518.

#include "layout.h"

// EA's animation clip id (On Tour struct tAnimID, 0x4: unsigned int SourceID : 8, AnimID : 24): the low byte is the
// bank / source, the upper 24 bits the clip index in it. kAnimIDInvalid (-1) is "no clip". [Staged by views group A.]
// [U] cGameAnimMap_addSource builds one per bank clip (SourceID = the bank's kind, AnimID = i) and passes it by value
// to cGameAnimMap_setMap; the readers split GameAnimMap[gad] the same way (& 0xFF, >> 8). EA's class has a
// user-declared empty constructor: cGameAnimMap's ctor still emits the (dead) array constructor loop over GameAnimMap
// (`new (this) tAnimID[518]`), which only a non-POD element type produces.
struct tAnimID {
    unsigned int SourceID : 8;      // bits 0-7 [O]; [U] addSource stores the bank kind; readers: GameAnimMap[gad] & 0xFF
    unsigned int AnimID : 24;       // bits 8-31 [O]; [U] addSource stores the clip index; readers: GameAnimMap[gad] >> 8
#ifdef __cplusplus
    tAnimID() {}
    // [O] tAnimID(int s, int i); [R] the scripter unit's static init builds kAnimIDInvalid as (-1, -1) (view sSlot_3E60,
    // D_004A51E0).
    tAnimID(int s, int i) { SourceID = s; AnimID = i; }
#endif
};

CHECK_SIZE(tAnimID, 0x4);

struct cGameAnimMap {
    unsigned int GameAnimHashes[518]; // 0x000 [O]; [U] initMap stores GetHashValue32(GameAnimNames[i]) in [i];
                                    //       lookupGAD compares the hash against GameAnimHashes[GameAnimSort[mid]]; initMap
                                    //       points the qsort comparator (gameanimcmphack) at it
    unsigned int GameAnimSort[518]; // 0x818 [O]; [U] initMap fills it with 0..517 and qsorts it by hash; lookupGAD
                                    //       binary-searches through it
    unsigned int GameAnimMap[518];  // 0x1030 [O] tAnimID {SourceID : 8 (low byte), AnimID : 24}; [U] initMap fills it
                                    //       with -1 bytes (memset(.., -1, 2072)); setMap stores a clip, remSource
                                    //       kAnimIDInvalid; getAnimName searches it; cRiderAnimBase_play /
                                    //       getAIAnimFrame / getAIAnimDuration / getAIAnimEventTime read [gad]
};

CHECK_OFFSET(cGameAnimMap, GameAnimHashes, 0x000);
CHECK_OFFSET(cGameAnimMap, GameAnimSort, 0x818);
CHECK_OFFSET(cGameAnimMap, GameAnimMap, 0x1030);
CHECK_SIZE(cGameAnimMap, 0x1848);

#endif /* ANIMATION_GAMEANIMMAP_H */
