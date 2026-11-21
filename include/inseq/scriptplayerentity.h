#ifndef INSEQ_SCRIPTPLAYERENTITY_H
#define INSEQ_SCRIPTPLAYERENTITY_H

// cScriptPlayerEntity: one entity (actor, camera, game-state track...) of an in-game sequence (InSeq) script. A
// cScriptPlayerMain owns an array of them, one per entity block of the script data, and an entity owns one
// cScriptPlayerChannel per animated channel (inseq/scriptplayerchannel.h). No virtual functions, no base class.
// Size 0x3C [R]: cScriptPlayerMain's Init (cScriptPlayerMain_Init) allocates numEntities x 0x3C (+ the 0x10 array cookie) from
// the cInSeq heap with func_002724C8 and runs the constructor cScriptPlayerEntity_cScriptPlayerEntity on each 0x3C stride; the destructor loop
// in cScriptPlayerMain_DeInit walks the same stride. On Tour's class is 0x38.
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert or out-of-line accessor for this class), hence inseq/. Retail code: unit replay/replaycache
// 0x00273938-0x00274514.
//
// Retail has one more word than On Tour at 0x0 (as in the channel classes and cInSeq's tScriptMainNode): every On
// Tour member is 4 bytes higher here, placed by [R] the constructor (zeroes 0x4..0x2C = On Tour's 0x0..0x28) and
// [U] Init (cScriptPlayerEntity_Init), DeInit (cScriptPlayerEntity_DeInit), SetEntityVariation (cScriptPlayerEntity_SetEntityVariation), SetupPlay (cScriptPlayerEntity_SetupPlay),
// ExecutePlay (cScriptPlayerEntity_ExecutePlay) and the accessors. On Tour's bools are 1 byte; retail's are words.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] the constructor cScriptPlayerEntity_cScriptPlayerEntity and the allocation in cScriptPlayerMain_Init;
//   [U] how retail code uses the field (asm of the 22 methods listed in methods.txt);
//   [O] SSX On Tour's DWARF (class cScriptPlayerEntity: same member order, +4).

#include "layout.h"

struct cScriptPlayerMain;
struct cScriptPlayerChannel;
struct cInSeqCommEntity;  // On Tour: interface, vptr only (OnInSeqLock, OnInSeqRelease, OnInSeqKeyEvent, OnInSeqUpdate)

// The entity block at the start of an entity's script data (On Tour tInSeqScriptEntityBlockHeader, 0xC).
struct tInSeqScriptEntityBlockHeader {
    unsigned int blockSize;           // 0x0 [O]; [U] Init returns it (the caller steps to the next block); GetDataSize
    unsigned int numVariations;       // 0x4 [O]; [U] GetNumVariations (cScriptPlayerEntity_GetNumVariations)
    unsigned int varOffsetTableStart; // 0x8 [O] first entry of the variation offset table (mpVariationTable)
};

// One variation's header (On Tour tInSeqEntityHeader, 0xC).
struct tInSeqEntityHeader {
    unsigned short headerSize;        // 0x0 [O]; [U] channel records start at this + headerSize; > 8 = has a custom header
    unsigned char numChannels;        // 0x2 [O]; [U] SetupPlay/Stop/Restart/DoFrameUpdate loop to it
    unsigned char type;               // 0x3 [O]; [U] GetType (cScriptPlayerEntity_GetType); passed to the cInSeqCommEntity calls
    int numFrames;                    // 0x4 [O]; [U] GetScriptLength (cScriptPlayerEntity_GetScriptLength, func_00274358)
    char nextSectionStart;            // 0x8 [O] the custom header (mpHeader) when headerSize > 8
    char dummyPad[3];                 // 0x9 [O]
};

struct cScriptPlayerEntity {
    int unk_00;                                   // 0x00 retail-only word (On Tour has none); the constructor and DeInit
                                                  //      leave it alone; no retail access seen
    const char* mpData;                           // 0x04 [O]; [R] ctor 0; [U] Init stores pScriptData; DeInit tests it;
                                                  //      GetHeader(int) = mpData + mpVariationTable[i]
    cScriptPlayerMain* mpOwner;                   // 0x08 [O]; [U] Init's 2nd argument; SetupPlay/Stop reach the cInSeq
                                                  //      heap as mpOwner->mpManager + 8; GetProjectID (cScriptPlayerEntity_GetProjectID)
    void* mpUserData;                             // 0x0C [O]; [R] ctor 0; [U] SetUserData (cScriptPlayerEntity_SetUserData) swaps it
    tInSeqScriptEntityBlockHeader* mpBlockHeader; // 0x10 [O]; [U] Init = pScriptData and returns ->blockSize;
                                                  //      GetNumVariations, GetDataSize (cScriptPlayerEntity_GetDataSize)
    unsigned int* mpVariationTable;               // 0x14 [O]; [U] Init = pScriptData + 8; SetEntityVariation and
                                                  //      GetHeader(int) index it
    int mCurrVariation;                           // 0x18 [O]; [U] Init 0; SetEntityVariation stores varIndex;
                                                  //      GetCurrVariation (cScriptPlayerEntity_GetCurrVariation)
    tInSeqEntityHeader* mpSystemHeader;           // 0x1C [O]; [U] Init/SetEntityVariation: mpData +
                                                  //      mpVariationTable[var]; numChannels/type/numFrames readers
    const char* mpHeader;                         // 0x20 [O]; [U] &mpSystemHeader->nextSectionStart when headerSize > 8,
                                                  //      else 0
    cScriptPlayerChannel** mpChannelPlayers;      // 0x24 [O]; [U] SetupPlay allocates numChannels x 4 from the cInSeq
                                                  //      heap (cScriptHeap_AllocMem) and creates each channel; GetChannel
                                                  //      (cScriptPlayerEntity_GetChannel); Stop frees it (cScriptHeap_FreeMem)
    int mIsPlaying;                               // 0x28 [O] (On Tour bool); [U] SetupPlay 1 (and returns it); Stop,
                                                  //      Restart, Update test it; Stop clears it
    cInSeqCommEntity* mpEntityComm;               // 0x2C [O]; [U] SetupPlay: GetInSeqCommInterface() ->
                                                  //      OnInSeqRequestEntityCommInterface (slot 6); SetEntityComm
                                                  //      (cScriptPlayerEntity_SetEntityComm) swaps it; its vtable calls (slots 1..4)
    int mSetupScriptHasKeysLeft;                  // 0x30 [O] (bool); [U] SetupPlay 0, then ORs each channel Play result
                                                  //      into it; ExecutePlay returns and clears it
    int mSetupScriptInitialEventBits;             // 0x34 [O]; [U] SetupPlay 0, then ORs each channel's event bits;
                                                  //      ExecutePlay passes it to OnInSeqKeyEvent/OnInSeqUpdate, clears it
    int mWaitingPlayExec;                         // 0x38 [O] (bool); [U] SetupPlay 1; ExecutePlay and DeInit 0
};

CHECK_OFFSET(tInSeqScriptEntityBlockHeader, blockSize, 0x0);
CHECK_OFFSET(tInSeqScriptEntityBlockHeader, numVariations, 0x4);
CHECK_OFFSET(tInSeqScriptEntityBlockHeader, varOffsetTableStart, 0x8);
CHECK_SIZE(tInSeqScriptEntityBlockHeader, 0xC);
CHECK_OFFSET(tInSeqEntityHeader, headerSize, 0x0);
CHECK_OFFSET(tInSeqEntityHeader, numChannels, 0x2);
CHECK_OFFSET(tInSeqEntityHeader, type, 0x3);
CHECK_OFFSET(tInSeqEntityHeader, numFrames, 0x4);
CHECK_OFFSET(tInSeqEntityHeader, nextSectionStart, 0x8);
CHECK_OFFSET(tInSeqEntityHeader, dummyPad, 0x9);
CHECK_SIZE(tInSeqEntityHeader, 0xC);

CHECK_OFFSET(cScriptPlayerEntity, unk_00, 0x00);
CHECK_OFFSET(cScriptPlayerEntity, mpData, 0x04);
CHECK_OFFSET(cScriptPlayerEntity, mpOwner, 0x08);
CHECK_OFFSET(cScriptPlayerEntity, mpUserData, 0x0C);
CHECK_OFFSET(cScriptPlayerEntity, mpBlockHeader, 0x10);
CHECK_OFFSET(cScriptPlayerEntity, mpVariationTable, 0x14);
CHECK_OFFSET(cScriptPlayerEntity, mCurrVariation, 0x18);
CHECK_OFFSET(cScriptPlayerEntity, mpSystemHeader, 0x1C);
CHECK_OFFSET(cScriptPlayerEntity, mpHeader, 0x20);
CHECK_OFFSET(cScriptPlayerEntity, mpChannelPlayers, 0x24);
CHECK_OFFSET(cScriptPlayerEntity, mIsPlaying, 0x28);
CHECK_OFFSET(cScriptPlayerEntity, mpEntityComm, 0x2C);
CHECK_OFFSET(cScriptPlayerEntity, mSetupScriptHasKeysLeft, 0x30);
CHECK_OFFSET(cScriptPlayerEntity, mSetupScriptInitialEventBits, 0x34);
CHECK_OFFSET(cScriptPlayerEntity, mWaitingPlayExec, 0x38);
CHECK_SIZE(cScriptPlayerEntity, 0x3C);

// cInSeqCommEntity: the per-entity callback interface (On Tour class cInSeqCommEntity: pure virtuals OnInSeqLock 1,
// OnInSeqRelease 2, OnInSeqKeyEvent 3, OnInSeqUpdate 4). cScriptEntity implements it with its vptr at +0
// (scripter/scriptentity.h: cScriptEntity_vtbl slots 1..4 are these). [U] cScriptPlayerEntity_SetEntityComm calls
// slots 1 / 2 (view cRcListener4240), cScriptPlayerEntity_ExecutePlay slots 3 / 4 (view cRcListener3D20).
struct cInSeqCommEntity {
    void** mVtbl;                   // 0x0 [O] (interface: vptr only); [U] the callers load it at +0
};

CHECK_OFFSET(cInSeqCommEntity, mVtbl, 0x0);
CHECK_SIZE(cInSeqCommEntity, 0x4);

// One channel record of a variation (On Tour tInSeqEntityChannelHeader, 0x8): the channel type and the size of its
// data. [U] cScriptPlayerEntity_SetupPlay reads type (+0) and steps by dataSize (+2) (view sRcRec_273AA8).
struct tInSeqEntityChannelHeader {
    unsigned short type;            // 0x0 [O]; [U] SetupPlay switches on it (eChannelType)
    unsigned short dataSize;        // 0x2 [O]; [U] SetupPlay steps to the next record by it
    char dataStart;                 // 0x4 [O] the channel data
    char dummyPad[3];               // 0x5 [O]
};

CHECK_OFFSET(tInSeqEntityChannelHeader, type, 0x0);
CHECK_OFFSET(tInSeqEntityChannelHeader, dataSize, 0x2);
CHECK_OFFSET(tInSeqEntityChannelHeader, dataStart, 0x4);
CHECK_OFFSET(tInSeqEntityChannelHeader, dummyPad, 0x5);
CHECK_SIZE(tInSeqEntityChannelHeader, 0x8);

#endif /* INSEQ_SCRIPTPLAYERENTITY_H */
