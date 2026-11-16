#ifndef INSEQ_SCRIPTPLAYERCHANNEL_H
#define INSEQ_SCRIPTPLAYERCHANNEL_H

// The in-game sequence (InSeq) script player's channels: one per animated channel of a script entity, created by the
// entity's Play (func_00273AA8) by channel type from the entity's heap: 0 spline, 1 object, 2 block, 3 event
// (func_00272488(size, heap) + constructor). Each channel has a vtable (slots 1..10: Play, Stop, Update, GetValue,
// GetCustom, GetNextKeyFrame, GetType, GetOwner, IsPlaying, destructor; the spline channel has no destructor slot).
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert for these classes), hence inseq/. Retail code: unit replay/replaycache 0x002725B8-0x002738F8 plus the
// out-of-line accessors at 0x002752F0-0x002753A8.
//
// Base cScriptPlayerChannel (abstract; no retail vtable of its own seen). On Tour's base is only a vptr (size 4) and
// every derived member there is 4 bytes lower than here: retail's base has one more word at 0x0 and the vptr at 0x4
// [R] (every constructor stores its vtable at +4, and the virtual calls in func_00273AA8/cScriptPlayerEntity_Stop/cScriptPlayerEntity_Restart
// load it from +4). Sizes [R]: func_00273AA8 allocates 0x28 (spline), 0x28 (object), 0x2C (block), 0x28 (event);
// On Tour 0x24/0x24/0x28/0x24.
//
// FLAT layouts, like main/game.h: every field at its retail offset, the base fields written out in each class.
// Names are EA's (On Tour), placed by [R] the constructors (func_002725B8, func_002734A8, func_00272D68,
// func_00273180) and [U] how retail code uses the field; [O] = On Tour's member (same order, +4).

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

// The abstract base, as seen through a channel pointer of unknown type (e.g. the entity's channel array).
struct cScriptPlayerChannel {
    int unk_00;                    // 0x000 retail-only base data (On Tour's cScriptPlayerChannel has none: its vptr
                                   //       is at 0); no retail access seen. It is why the vptr sits at 0x4 and every
                                   //       On Tour member is +4 here
    void** mVtbl;                  // 0x004 [R] the constructors store the class's vtable here; slots 1..10: Play,
                                   //       Stop, Update, GetValue, GetCustom, GetNextKeyFrame, GetType, GetOwner,
                                   //       IsPlaying, destructor
};

// cScriptPlayerChannelSpline: size 0x28 [R] (func_00273AA8 allocates 0x28); GetType() = 0;
// vtable cScriptPlayerChannelSpline_vtbl (0x00481A10), constructor func_002725B8, no destructor.
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
    tInSeqSplineChannelKey* mpCurrKey; // 0x014 [O]; [U] Play: data + 4; Update (func_002726A8) advances it to
                                   //       mpNextKey
    unsigned int mCurrKeyIndex;    // 0x018 [O]; [U] Play 0; Update ++ it, compares it with header[0] - 1
    float mCurrKeyValue;           // 0x01C [O]; [U] Play/Update store the evaluated cubic (func_00272570) with swc1;
                                   //       GetValue returns its address
    const char* mpCurrKeyCustom;   // 0x020 [O]; [U] key + 0x14 when the key is longer than 0x14 bytes, else 0;
                                   //       GetCustom returns it
    tInSeqSplineChannelKey* mpNextKey; // 0x024 [O]; [U] Play: next key when there are 2+, else 0; Update returns 0
                                   //       when it is 0
};

// cScriptPlayerChannelObject: size 0x28 [R] (func_00273AA8 allocates 0x28); GetType() = 1;
// vtable cScriptPlayerChannelObject_vtbl (0x004818F0), constructor func_002734A8, destructor func_002734E0.
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
    tInSeqObjectChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; func_002738F8 (LoadNextScriptKey) steps it
    int mNextKeyIndex;             // 0x018 [O]; [U] func_002738F8 counts it
    tObjectEntry* mpObjectArray;   // 0x01C [O]; [U] Play allocates header[1] x 0x24 (tObjectEntry); Stop frees it;
                                   //       func_00273860 indexes it
    tObjectEntry* mpActiveObjectList; // 0x020 [O]; [U] func_00273860 (AddActiveKey) pushes onto it; Update
                                   //       (func_00273698) unlinks ended entries; GetCustom returns it
    int mNumActiveObjects;         // 0x024 [O]; [U] ++ by AddActiveKey, -- by Update; GetValue returns its address
};

// cScriptPlayerChannelBlock: size 0x2C [R] (func_00273AA8 allocates 0x2C); GetType() = 2;
// vtable cScriptPlayerChannelBlock_vtbl (0x004819B0), constructor func_00272D68, destructor func_00272DA0.
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
    tInSeqBlockChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; func_00273140 (LoadNextScriptKey) steps it by
                                   //       the key's size (0 at the end)
    int mNextKeyIndex;             // 0x018 [O]; [U] func_00273140 counts it up to header[0] - 1
    tInSeqBlock* mpDataArray;      // 0x01C [O]; no retail access seen (the ctor and Stop skip it: placed by the
                                   //       order)
    tBlockEntry* mpBlockArray;     // 0x020 [O]; [U] Play allocates header[1] x 0x1C (tBlockEntry) from the owner's
                                   //       heap; Stop frees it; func_002730C0 indexes it
    tBlockEntry* mpActiveBlockList; // 0x024 [O]; [U] func_002730C0 (AddActiveKey) pushes onto it; Update
                                   //       (func_00272F28) unlinks ended entries; GetCustom returns it
    int mNumActiveBlocks;          // 0x028 [O]; [U] ++ by AddActiveKey, -- by Update; GetValue returns its address
};

// cScriptPlayerChannelEvent: size 0x28 [R] (func_00273AA8 allocates 0x28); GetType() = 3;
// vtable cScriptPlayerChannelEvent_vtbl (0x00481950), constructor func_00273180, destructor func_002731B8.
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
    tInSeqEventChannelKey* mpNextKey; // 0x014 [O]; [U] Play: data + 4; func_00273468 (LoadNextScriptKey) steps it
    int mNextKeyIndex;             // 0x018 [O]; [U] func_00273468 counts it
    tEventEntry* mpEventArray;     // 0x01C [O]; [U] Play allocates header[1] x 8 (tEventEntry); Stop frees it;
                                   //       func_00273418 fills entry [mNumActiveEvents]; GetCustom returns it
    int mNumActiveEvents;          // 0x020 [O]; [U] func_00273418 ++; Update (func_00273330) resets it to 0 on a new
                                   //       frame; GetValue returns its address
    float mEventFrame;             // 0x024 [O]; [U] Update compares/stores it as a float (lwc1/swc1); Play and Stop
                                   //       store 0 with sw $zero
};

CHECK_OFFSET(cScriptPlayerChannel, unk_00, 0x0);
CHECK_OFFSET(cScriptPlayerChannel, mVtbl, 0x4);
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

#endif /* INSEQ_SCRIPTPLAYERCHANNEL_H */
