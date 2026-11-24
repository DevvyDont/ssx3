#ifndef INSEQ_SCRIPTPLAYERCHANNEL_H
#define INSEQ_SCRIPTPLAYERCHANNEL_H

// The in-game sequence (InSeq) script player's channels: one per animated channel of a script entity, created by the
// entity's Play (cScriptPlayerEntity_SetupPlay) by channel type from the entity's heap: 0 spline, 1 object, 2 block, 3 event
// (func_00272488(size, heap) + constructor). Each channel has a vtable (slots 1..10: Play, Stop, Update, GetValue,
// GetCustom, GetNextKeyFrame, GetType, GetOwner, IsPlaying, destructor; the spline channel has no destructor slot).
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert for these classes), hence inseq/. Retail code: unit replay/replaycache 0x002725B8-0x002738F8 plus the
// out-of-line accessors at 0x002752F0-0x002753A8.
//
// Base cScriptPlayerChannel (abstract; no retail vtable of its own seen). On Tour's base is only a vptr (size 4) and
// every derived member there is 4 bytes lower than here: retail's base has one more word at 0x0 and the vptr at 0x4
// [R] (every constructor stores its vtable at +4, and the virtual calls in cScriptPlayerEntity_SetupPlay/cScriptPlayerEntity_Stop/cScriptPlayerEntity_Restart
// load it from +4). Sizes [R]: cScriptPlayerEntity_SetupPlay allocates 0x28 (spline), 0x28 (object), 0x2C (block), 0x28 (event);
// On Tour 0x24/0x24/0x28/0x24.
//
// FLAT layouts, like main/game.h: every field at its retail offset, the base fields written out in each class.
// Names are EA's (On Tour), placed by [R] the constructors (cScriptPlayerChannelSpline_cScriptPlayerChannelSpline, cScriptPlayerChannelObject_cScriptPlayerChannelObject, cScriptPlayerChannelBlock_cScriptPlayerChannelBlock,
// cScriptPlayerChannelEvent_cScriptPlayerChannelEvent) and [U] how retail code uses the field; [O] = On Tour's member (same order, +4).

#include "layout.h"

struct cScriptPlayerEntity;
struct tInSeqBlockChannelHeader;
struct tInSeqBlockChannelKey;
struct tInSeqBlock;
struct tBlockEntry;       // On Tour 0x1C; retail Play allocates count x 0x1C
struct tInSeqEventChannelHeader;
struct tInSeqEventChannelKey;
struct tEventEntry;       // On Tour 0x8; retail Play allocates count x 8
struct tInSeqObjectChannelHeader;
struct tInSeqObjectChannelKey;
struct tObjectEntry;      // On Tour 0x24; retail Play allocates count x 0x24
struct tInSeqSplineChannelHeader;
struct tInSeqSplineChannelKey;

// GetType() values (On Tour eChannelType order; [U] the four GetType accessors return them).
enum eChannelType {
    kChannelSpline = 0,
    kChannelObject = 1,
    kChannelBlock = 2,
    kChannelEvent = 3
};

// The abstract base, as seen through a channel pointer of unknown type (e.g. the entity's channel array). A real
// C++ class: g++ 2.95 puts the vptr after the data of the class that declares the virtuals, so it lands at 0x4,
// where [R] every constructor stores its vtable. Slots [O] (On Tour's order, names, parameters and return types;
// int& events as int*); [U] cScriptPlayerEntity_SetupPlay / _Restart call Play, _Restart Stop, _DoFrameUpdate
// Update, _Update GetNextKeyFrame. Slot 10 is the destructor (cScriptPlayerEntity_Stop calls it with flags 3 for the
// object, block and event channels); not declared here.
struct cScriptPlayerChannel {
    int unk_00;                    // 0x000 retail-only base data (On Tour's cScriptPlayerChannel has none: its vptr
                                   //       is at 0); no retail access seen. It is why the vptr sits at 0x4 and every
                                   //       On Tour member is +4 here
    // 0x004: the vptr
    virtual bool Play(const char* pScriptData, cScriptPlayerEntity* pOwner, int* events) = 0;  // 1
    virtual void Stop() = 0;                                                                  // 2
    virtual bool Update(float currFrameNum, float frameDelta, int* events) = 0;               // 3
    virtual void* GetValue() = 0;                                                             // 4
    virtual void* GetCustom() = 0;                                                            // 5
    virtual int GetNextKeyFrame() = 0;                                                        // 6
    virtual eChannelType GetType() = 0;                                                       // 7
    virtual cScriptPlayerEntity* GetOwner() = 0;                                              // 8
    virtual bool IsPlaying() = 0;                                                             // 9
};

// cScriptPlayerChannelSpline: size 0x28 [R] (cScriptPlayerEntity_SetupPlay allocates 0x28); GetType() = 0;
// vtable cScriptPlayerChannelSpline_vtbl (0x00481A10), constructor cScriptPlayerChannelSpline_cScriptPlayerChannelSpline, no destructor.
struct cScriptPlayerChannelSpline {
    // --- cScriptPlayerChannel (see above)
    int unk_00;                    // 0x000 retail-only base word; no retail access seen
    void** mVtbl;                  // 0x004 [R] constructor stores cScriptPlayerChannelSpline_vtbl
    const char* mpData;            // 0x008 [O]; [U] Play stores the script data; IsPlaying = (mpData != 0); Stop
                                   //       zeroes
    cScriptPlayerEntity* mpOwner;  // 0x00C [O]; [U] Play stores its owner argument; GetOwner returns it; the
                                   //       block/event/object Play/Stop (de)allocate from the owner heap at
                                   //       *(*(owner + 8) + 8) + 8
    // --- cScriptPlayerChannelSpline
    tInSeqSplineChannelHeader* mpHeader; // 0x010 [O]; [U] Play stores the data; header[0] (a word) = key count
    tInSeqSplineChannelKey* mpCurrKey; // 0x014 [O]; [U] Play: data + 4; Update (cScriptPlayerChannelSpline_Update) advances it to
                                   //       mpNextKey
    unsigned int mCurrKeyIndex;    // 0x018 [O]; [U] Play 0; Update ++ it, compares it with header[0] - 1
    float mCurrKeyValue;           // 0x01C [O]; [U] Play/Update store the evaluated cubic (GetCubicSplineValue) with swc1;
                                   //       GetValue returns its address
    const char* mpCurrKeyCustom;   // 0x020 [O]; [U] key + 0x14 when the key is longer than 0x14 bytes, else 0;
                                   //       GetCustom returns it
    tInSeqSplineChannelKey* mpNextKey; // 0x024 [O]; [U] Play: next key when there are 2+, else 0; Update returns 0
                                   //       when it is 0
};

// cScriptPlayerChannelObject: size 0x28 [R] (cScriptPlayerEntity_SetupPlay allocates 0x28); GetType() = 1;
// vtable cScriptPlayerChannelObject_vtbl (0x004818F0), constructor cScriptPlayerChannelObject_cScriptPlayerChannelObject, destructor cScriptPlayerChannelObject__cScriptPlayerChannelObject.
struct cScriptPlayerChannelObject {
    // --- cScriptPlayerChannel (see above)
    int unk_00;                    // 0x000 retail-only base word; no retail access seen
    void** mVtbl;                  // 0x004 [R] constructor stores cScriptPlayerChannelObject_vtbl
    const char* mpData;            // 0x008 [O]; [U] Play stores the script data; IsPlaying = (mpData != 0); Stop
                                   //       zeroes
    cScriptPlayerEntity* mpOwner;  // 0x00C [O]; [U] Play stores its owner argument; GetOwner returns it; the
                                   //       block/event/object Play/Stop (de)allocate from the owner heap at
                                   //       *(*(owner + 8) + 8) + 8
    // --- cScriptPlayerChannelObject
    tInSeqObjectChannelHeader* mpHeader; // 0x010 [O]; [U] Play stores the data; header[0] = key count, header[1] =
                                   //       object count
    tInSeqObjectChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; cScriptPlayerChannelObject_LoadNextScriptKey steps it
    int mNextKeyIndex;             // 0x018 [O]; [U] cScriptPlayerChannelObject_LoadNextScriptKey counts it
    tObjectEntry* mpObjectArray;   // 0x01C [O]; [U] Play allocates header[1] x 0x24 (tObjectEntry); Stop frees it;
                                   //       cScriptPlayerChannelObject_AddActiveKey indexes it
    tObjectEntry* mpActiveObjectList; // 0x020 [O]; [U] cScriptPlayerChannelObject_AddActiveKey pushes onto it; Update
                                   //       (cScriptPlayerChannelObject_Update) unlinks ended entries; GetCustom returns it
    int mNumActiveObjects;         // 0x024 [O]; [U] ++ by AddActiveKey, -- by Update; GetValue returns its address
};

// cScriptPlayerChannelBlock: size 0x2C [R] (cScriptPlayerEntity_SetupPlay allocates 0x2C); GetType() = 2;
// vtable cScriptPlayerChannelBlock_vtbl (0x004819B0), constructor cScriptPlayerChannelBlock_cScriptPlayerChannelBlock, destructor cScriptPlayerChannelBlock__cScriptPlayerChannelBlock.
struct cScriptPlayerChannelBlock {
    // --- cScriptPlayerChannel (see above)
    int unk_00;                    // 0x000 retail-only base word; no retail access seen
    void** mVtbl;                  // 0x004 [R] constructor stores cScriptPlayerChannelBlock_vtbl
    const char* mpData;            // 0x008 [O]; [U] Play stores the script data; IsPlaying = (mpData != 0); Stop
                                   //       zeroes
    cScriptPlayerEntity* mpOwner;  // 0x00C [O]; [U] Play stores its owner argument; GetOwner returns it; the
                                   //       block/event/object Play/Stop (de)allocate from the owner heap at
                                   //       *(*(owner + 8) + 8) + 8
    // --- cScriptPlayerChannelBlock
    tInSeqBlockChannelHeader* mpHeader; // 0x010 [O]; [U] Play stores the data; header[0] = key count, header[1] =
                                   //       block count
    tInSeqBlockChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; cScriptPlayerChannelBlock_LoadNextScriptKey steps it by
                                   //       the key's size (0 at the end)
    int mNextKeyIndex;             // 0x018 [O]; [U] cScriptPlayerChannelBlock_LoadNextScriptKey counts it up to header[0] - 1
    tInSeqBlock* mpDataArray;      // 0x01C [O]; no retail access seen (the ctor and Stop skip it: placed by the
                                   //       order)
    tBlockEntry* mpBlockArray;     // 0x020 [O]; [U] Play allocates header[1] x 0x1C (tBlockEntry) from the owner's
                                   //       heap; Stop frees it; cScriptPlayerChannelBlock_AddActiveKey indexes it
    tBlockEntry* mpActiveBlockList; // 0x024 [O]; [U] cScriptPlayerChannelBlock_AddActiveKey pushes onto it; Update
                                   //       (cScriptPlayerChannelBlock_Update) unlinks ended entries; GetCustom returns it
    int mNumActiveBlocks;          // 0x028 [O]; [U] ++ by AddActiveKey, -- by Update; GetValue returns its address
};

// cScriptPlayerChannelEvent: size 0x28 [R] (cScriptPlayerEntity_SetupPlay allocates 0x28); GetType() = 3;
// vtable cScriptPlayerChannelEvent_vtbl (0x00481950), constructor cScriptPlayerChannelEvent_cScriptPlayerChannelEvent, destructor cScriptPlayerChannelEvent__cScriptPlayerChannelEvent.
struct cScriptPlayerChannelEvent {
    // --- cScriptPlayerChannel (see above)
    int unk_00;                    // 0x000 retail-only base word; no retail access seen
    void** mVtbl;                  // 0x004 [R] constructor stores cScriptPlayerChannelEvent_vtbl
    const char* mpData;            // 0x008 [O]; [U] Play stores the script data; IsPlaying = (mpData != 0); Stop
                                   //       zeroes
    cScriptPlayerEntity* mpOwner;  // 0x00C [O]; [U] Play stores its owner argument; GetOwner returns it; the
                                   //       block/event/object Play/Stop (de)allocate from the owner heap at
                                   //       *(*(owner + 8) + 8) + 8
    // --- cScriptPlayerChannelEvent
    tInSeqEventChannelHeader* mpHeader; // 0x010 [O]; [U] Play stores the data; header[0] = key count, header[1] =
                                   //       event count
    tInSeqEventChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; cScriptPlayerChannelEvent_LoadNextScriptKey steps it
    int mNextKeyIndex;             // 0x018 [O]; [U] cScriptPlayerChannelEvent_LoadNextScriptKey counts it
    tEventEntry* mpEventArray;     // 0x01C [O]; [U] Play allocates header[1] x 8 (tEventEntry); Stop frees it;
                                   //       cScriptPlayerChannelEvent_AddActiveKey fills entry [mNumActiveEvents]; GetCustom returns it
    int mNumActiveEvents;          // 0x020 [O]; [U] cScriptPlayerChannelEvent_AddActiveKey ++; Update (cScriptPlayerChannelEvent_Update) resets it to 0 on a new
                                   //       frame; GetValue returns its address
    float mEventFrame;             // 0x024 [O]; [U] Update compares/stores it as a float (lwc1/swc1); Play and Stop
                                   //       store 0 with sw $zero
};

CHECK_OFFSET(cScriptPlayerChannel, unk_00, 0x0);
CHECK_SIZE(cScriptPlayerChannel, 0x8);
CHECK_OFFSET(cScriptPlayerChannelSpline, unk_00, 0x0);
CHECK_OFFSET(cScriptPlayerChannelSpline, mVtbl, 0x4);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpData, 0x8);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpOwner, 0xC);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpHeader, 0x10);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpCurrKey, 0x14);
CHECK_OFFSET(cScriptPlayerChannelSpline, mCurrKeyIndex, 0x18);
CHECK_OFFSET(cScriptPlayerChannelSpline, mCurrKeyValue, 0x1C);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpCurrKeyCustom, 0x20);
CHECK_OFFSET(cScriptPlayerChannelSpline, mpNextKey, 0x24);
CHECK_SIZE(cScriptPlayerChannelSpline, 0x28);
CHECK_OFFSET(cScriptPlayerChannelObject, unk_00, 0x0);
CHECK_OFFSET(cScriptPlayerChannelObject, mVtbl, 0x4);
CHECK_OFFSET(cScriptPlayerChannelObject, mpData, 0x8);
CHECK_OFFSET(cScriptPlayerChannelObject, mpOwner, 0xC);
CHECK_OFFSET(cScriptPlayerChannelObject, mpHeader, 0x10);
CHECK_OFFSET(cScriptPlayerChannelObject, mpNextKey, 0x14);
CHECK_OFFSET(cScriptPlayerChannelObject, mNextKeyIndex, 0x18);
CHECK_OFFSET(cScriptPlayerChannelObject, mpObjectArray, 0x1C);
CHECK_OFFSET(cScriptPlayerChannelObject, mpActiveObjectList, 0x20);
CHECK_OFFSET(cScriptPlayerChannelObject, mNumActiveObjects, 0x24);
CHECK_SIZE(cScriptPlayerChannelObject, 0x28);
CHECK_OFFSET(cScriptPlayerChannelBlock, unk_00, 0x0);
CHECK_OFFSET(cScriptPlayerChannelBlock, mVtbl, 0x4);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpData, 0x8);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpOwner, 0xC);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpHeader, 0x10);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpNextKey, 0x14);
CHECK_OFFSET(cScriptPlayerChannelBlock, mNextKeyIndex, 0x18);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpDataArray, 0x1C);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpBlockArray, 0x20);
CHECK_OFFSET(cScriptPlayerChannelBlock, mpActiveBlockList, 0x24);
CHECK_OFFSET(cScriptPlayerChannelBlock, mNumActiveBlocks, 0x28);
CHECK_SIZE(cScriptPlayerChannelBlock, 0x2C);
CHECK_OFFSET(cScriptPlayerChannelEvent, unk_00, 0x0);
CHECK_OFFSET(cScriptPlayerChannelEvent, mVtbl, 0x4);
CHECK_OFFSET(cScriptPlayerChannelEvent, mpData, 0x8);
CHECK_OFFSET(cScriptPlayerChannelEvent, mpOwner, 0xC);
CHECK_OFFSET(cScriptPlayerChannelEvent, mpHeader, 0x10);
CHECK_OFFSET(cScriptPlayerChannelEvent, mpNextKey, 0x14);
CHECK_OFFSET(cScriptPlayerChannelEvent, mNextKeyIndex, 0x18);
CHECK_OFFSET(cScriptPlayerChannelEvent, mpEventArray, 0x1C);
CHECK_OFFSET(cScriptPlayerChannelEvent, mNumActiveEvents, 0x20);
CHECK_OFFSET(cScriptPlayerChannelEvent, mEventFrame, 0x24);
CHECK_SIZE(cScriptPlayerChannelEvent, 0x28);

// Block-channel key and active-block record (On Tour tInSeqBlockChannelKey 0x14, tInSeqBlock 0x14, tBlockEntry 0x1C).
// [R] cScriptPlayerChannelBlock_Play allocates header[1] x 0x1C entries; [U] cScriptPlayerChannelBlock_AddActiveKey
// copies key +0x8 / +0x2 / +0x4 / +0x6 / +0xC into entry +0x0 / +0x4 / +0x8 / +0xC / +0x10, stores the custom
// pointer at +0x14 and pushes the entry through +0x18; Update reads key +0x4 (view sCacheRec_2F28) and entry +0xC /
// +0x18 (view sCacheEvent_2F28).
struct tInSeqBlockChannelKey {
    unsigned short keySize;         // 0x00 [O]; [U] the custom data exists when keySize > 0x14
    unsigned short track;           // 0x02 [O]
    unsigned short frameStart;      // 0x04 [O]; [U] Play / Update compare it with the frame
    unsigned short frameEnd;        // 0x06 [O]
    int enumVal;                    // 0x08 [O]
    unsigned int numOverlapFrames;  // 0x0C [O]
    char customDataStart;           // 0x10 [O]
    char dummyPad[3];               // 0x11 [O]
};

CHECK_OFFSET(tInSeqBlockChannelKey, keySize, 0x00);
CHECK_OFFSET(tInSeqBlockChannelKey, track, 0x02);
CHECK_OFFSET(tInSeqBlockChannelKey, frameStart, 0x04);
CHECK_OFFSET(tInSeqBlockChannelKey, frameEnd, 0x06);
CHECK_OFFSET(tInSeqBlockChannelKey, enumVal, 0x08);
CHECK_OFFSET(tInSeqBlockChannelKey, numOverlapFrames, 0x0C);
CHECK_OFFSET(tInSeqBlockChannelKey, customDataStart, 0x10);
CHECK_SIZE(tInSeqBlockChannelKey, 0x14);

struct tInSeqBlock {
    int value;                      // 0x00 [O]; [U] AddActiveKey: key enumVal
    int track;                      // 0x04 [O]; [U] key track
    int scriptStartFrame;           // 0x08 [O]; [U] key frameStart
    int scriptEndFrame;             // 0x0C [O]; [U] key frameEnd; Update ends the block there
    unsigned int overlapInFrames;   // 0x10 [O]; [U] key numOverlapFrames
};

CHECK_OFFSET(tInSeqBlock, value, 0x00);
CHECK_OFFSET(tInSeqBlock, track, 0x04);
CHECK_OFFSET(tInSeqBlock, scriptStartFrame, 0x08);
CHECK_OFFSET(tInSeqBlock, scriptEndFrame, 0x0C);
CHECK_OFFSET(tInSeqBlock, overlapInFrames, 0x10);
CHECK_SIZE(tInSeqBlock, 0x14);

struct tBlockEntry {
    tInSeqBlock data;               // 0x00 [O]
    void* pCustom;                  // 0x14 [O]; [U] AddActiveKey: key + 0x10 or 0
    tBlockEntry* pNext;             // 0x18 [O]; [U] the active list link (mpActiveBlockList)
};

CHECK_OFFSET(tBlockEntry, data, 0x00);
CHECK_OFFSET(tBlockEntry, pCustom, 0x14);
CHECK_OFFSET(tBlockEntry, pNext, 0x18);
CHECK_SIZE(tBlockEntry, 0x1C);

// Object-channel key and active-object record (On Tour tInSeqObjectChannelKey 0x1C, tInSeqObject 0x1C,
// tObjectEntry 0x24). [R] cScriptPlayerChannelObject_Play allocates header[1] x 0x24 entries; [U]
// cScriptPlayerChannelObject_Update reads key +0x4 (view sCacheRec_3698) and entry +0x8 / +0x10 / +0x14 / +0x20
// (view sCacheEvent_3698).
struct tInSeqObjectChannelKey {
    unsigned short keySize;         // 0x00 [O]
    unsigned short track;           // 0x02 [O]
    unsigned short frameStart;      // 0x04 [O]; [U] Play / Update compare it with the frame
    unsigned short frameEnd;        // 0x06 [O]
    int objectID;                   // 0x08 [O]
    int objectFrameStart;           // 0x0C [O]
    float playRate;                 // 0x10 [O]
    unsigned int blendInFrames;     // 0x14 [O]
    char customDataStart;           // 0x18 [O]
    char dummyPad[3];               // 0x19 [O]
};

CHECK_OFFSET(tInSeqObjectChannelKey, keySize, 0x00);
CHECK_OFFSET(tInSeqObjectChannelKey, track, 0x02);
CHECK_OFFSET(tInSeqObjectChannelKey, frameStart, 0x04);
CHECK_OFFSET(tInSeqObjectChannelKey, frameEnd, 0x06);
CHECK_OFFSET(tInSeqObjectChannelKey, objectID, 0x08);
CHECK_OFFSET(tInSeqObjectChannelKey, objectFrameStart, 0x0C);
CHECK_OFFSET(tInSeqObjectChannelKey, playRate, 0x10);
CHECK_OFFSET(tInSeqObjectChannelKey, blendInFrames, 0x14);
CHECK_OFFSET(tInSeqObjectChannelKey, customDataStart, 0x18);
CHECK_SIZE(tInSeqObjectChannelKey, 0x1C);

struct tInSeqObject {
    int objectID;                   // 0x00 [O]
    int scriptStartFrame;           // 0x04 [O]
    int scriptEndFrame;             // 0x08 [O]; [U] Update ends the object there
    int trackIndex;                 // 0x0C [O]
    float currObjectFrame;          // 0x10 [O]; [U] Update advances it by playRate
    float playRate;                 // 0x14 [O]; [U] Update
    unsigned int blendInFrames;     // 0x18 [O]
};

CHECK_OFFSET(tInSeqObject, objectID, 0x00);
CHECK_OFFSET(tInSeqObject, scriptStartFrame, 0x04);
CHECK_OFFSET(tInSeqObject, scriptEndFrame, 0x08);
CHECK_OFFSET(tInSeqObject, trackIndex, 0x0C);
CHECK_OFFSET(tInSeqObject, currObjectFrame, 0x10);
CHECK_OFFSET(tInSeqObject, playRate, 0x14);
CHECK_OFFSET(tInSeqObject, blendInFrames, 0x18);
CHECK_SIZE(tInSeqObject, 0x1C);

struct tObjectEntry {
    tInSeqObject data;              // 0x00 [O]
    void* pCustom;                  // 0x1C [O]
    tObjectEntry* pNext;            // 0x20 [O]; [U] the active list link (mpActiveObjectList)
};

CHECK_OFFSET(tObjectEntry, data, 0x00);
CHECK_OFFSET(tObjectEntry, pCustom, 0x1C);
CHECK_OFFSET(tObjectEntry, pNext, 0x20);
CHECK_SIZE(tObjectEntry, 0x24);

// The channel data headers and the spline / event keys (On Tour tInSeq*ChannelHeader, 8 bytes each;
// tInSeqSplineChannelKey 0x18; tInSeqEventChannelKey 0xC; same layouts). A channel's script data starts with its
// header; the first key is at &keyStart (+0x4) and each key's keySize steps to the next. [U] the four Play functions
// (src/replay/replaycache.cpp) store the data as mpHeader, read the key count and the per-channel array count from
// the header and set mpNextKey (or mpCurrKey) to header + 4; Update / LoadNextScriptKey step keys by keySize.
// dummyPad bytes have no retail use (unk_005 / unk_009 / unk_015).
struct tInSeqSplineChannelHeader {
    unsigned int numKeys;           // 0x0 [O]; [U] cScriptPlayerChannelSpline_Play / Update read data[0] as a word
    char keyStart;                  // 0x4 [O]; [U] Play: mpCurrKey = data + 4
    char unk_005[3];                // 0x5 [O] dummyPad
};

CHECK_OFFSET(tInSeqSplineChannelHeader, numKeys, 0x0);
CHECK_OFFSET(tInSeqSplineChannelHeader, keyStart, 0x4);
CHECK_SIZE(tInSeqSplineChannelHeader, 0x8);

struct tInSeqSplineChannelKey {
    unsigned short keySize;         // 0x00 [O]; [U] Play / Update step to the next key by it; > 0x14 means custom data
    unsigned short frameNum;        // 0x02 [O]; [U] Update compares it with the current frame and subtracts it
    float splineCoef[4];            // 0x04 [O]; [U] passed to GetCubicSplineValue
    char customDataStart;           // 0x14 [O]; [U] mpCurrKeyCustom = key + 0x14 when keySize > 0x14
    char unk_015[3];                // 0x15 [O] dummyPad
};

CHECK_OFFSET(tInSeqSplineChannelKey, keySize, 0x00);
CHECK_OFFSET(tInSeqSplineChannelKey, frameNum, 0x02);
CHECK_OFFSET(tInSeqSplineChannelKey, splineCoef, 0x04);
CHECK_OFFSET(tInSeqSplineChannelKey, customDataStart, 0x14);
CHECK_SIZE(tInSeqSplineChannelKey, 0x18);

struct tInSeqEventChannelHeader {
    unsigned short numKeys;         // 0x0 [O]; [U] cScriptPlayerChannelEvent_Play tests data[0]; LoadNextScriptKey
                                    //      compares the index with numKeys - 1
    unsigned short maxSimultaneousEvents; // 0x2 [O]; [U] Play allocates data[1] x 8 event entries
    char keyStart;                  // 0x4 [O]; [U] Play: mpNextKey = header + 4
    char unk_005[3];                // 0x5 [O] dummyPad
};

CHECK_OFFSET(tInSeqEventChannelHeader, numKeys, 0x0);
CHECK_OFFSET(tInSeqEventChannelHeader, maxSimultaneousEvents, 0x2);
CHECK_OFFSET(tInSeqEventChannelHeader, keyStart, 0x4);
CHECK_SIZE(tInSeqEventChannelHeader, 0x8);

struct tInSeqEventChannelKey {
    unsigned short keySize;         // 0x0 [O]; [U] LoadNextScriptKey steps by it; AddActiveKey: > 8 means custom data
    unsigned short frameNum;        // 0x2 [O]; [U] Play / Update compare it with the frame (header + 6 = first key's)
    int enumVal;                    // 0x4 [O]; [U] AddActiveKey copies it into the event entry
    char customDataStart;           // 0x8 [O]; [U] AddActiveKey: the entry's data = key + 8 when keySize > 8
    char unk_009[3];                // 0x9 [O] dummyPad
};

CHECK_OFFSET(tInSeqEventChannelKey, keySize, 0x0);
CHECK_OFFSET(tInSeqEventChannelKey, frameNum, 0x2);
CHECK_OFFSET(tInSeqEventChannelKey, enumVal, 0x4);
CHECK_OFFSET(tInSeqEventChannelKey, customDataStart, 0x8);
CHECK_SIZE(tInSeqEventChannelKey, 0xC);

// An active event (On Tour tEventEntry, 0x8; same layout). [R] cScriptPlayerChannelEvent_Play allocates
// maxSimultaneousEvents x 8 of them (mpEventArray); [U] cScriptPlayerChannelEvent_AddActiveKey fills entry
// [mNumActiveEvents] from a key.
struct tEventEntry {
    int eventID;                    // 0x0 [O]; [U] AddActiveKey: the key's enumVal
    void* pCustom;                  // 0x4 [O]; [U] AddActiveKey: &key->customDataStart when keySize > 8, else 0
};

CHECK_OFFSET(tEventEntry, eventID, 0x0);
CHECK_OFFSET(tEventEntry, pCustom, 0x4);
CHECK_SIZE(tEventEntry, 0x8);

struct tInSeqBlockChannelHeader {
    unsigned short numKeys;         // 0x0 [O]; [U] cScriptPlayerChannelBlock_Play tests data[0]; the key loaders
                                    //      compare the index with it
    unsigned short numTracks;       // 0x2 [O]; [U] Play allocates data[1] x 0x1C block entries (tBlockEntry)
    char keyStart;                  // 0x4 [O]; [U] Play: mpNextKey = header + 4
    char unk_005[3];                // 0x5 [O] dummyPad
};

CHECK_OFFSET(tInSeqBlockChannelHeader, numKeys, 0x0);
CHECK_OFFSET(tInSeqBlockChannelHeader, numTracks, 0x2);
CHECK_OFFSET(tInSeqBlockChannelHeader, keyStart, 0x4);
CHECK_SIZE(tInSeqBlockChannelHeader, 0x8);

struct tInSeqObjectChannelHeader {
    unsigned short numKeys;         // 0x0 [O]; [U] cScriptPlayerChannelObject_Play tests data[0]; the key loaders
                                    //      compare the index with it
    unsigned short numTracks;       // 0x2 [O]; [U] Play allocates data[1] x 0x24 object entries (tObjectEntry)
    char keyStart;                  // 0x4 [O]; [U] Play: mpNextKey = header + 4
    char unk_005[3];                // 0x5 [O] dummyPad
};

CHECK_OFFSET(tInSeqObjectChannelHeader, numKeys, 0x0);
CHECK_OFFSET(tInSeqObjectChannelHeader, numTracks, 0x2);
CHECK_OFFSET(tInSeqObjectChannelHeader, keyStart, 0x4);
CHECK_SIZE(tInSeqObjectChannelHeader, 0x8);

#endif /* INSEQ_SCRIPTPLAYERCHANNEL_H */
