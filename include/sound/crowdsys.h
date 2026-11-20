#ifndef SOUND_CROWDSYS_H
#define SOUND_CROWDSYS_H

// cCrowdSys and cCrowdInstance: the crowd ambience / cheer system. One cCrowdInstance per viewport tracks up to 15
// crowd objects (world positions registered by the crowd managers), groups them into 3 ambient loops and plays
// one-shot cheers (TRICK / FALL / ANTICIPATE sample lists from data/config/crowd.inf, per crowd intensity).
// EA source d:/ssx/code/source/sound/crowdsys.cpp (preview assert strings: "gpCrowdSys", "mpCrowdInstances",
// "mViewPort >= 0", "mCrowdObjectsInfo[ crowdObjectHandle ].pCrowdObjectPos != NULL", "mNumCrowdAmbience", ...).
// Retail methods: sound/soundsys unit 0x2A4BA8..0x2A77F8, plus the out-of-line inline CROWD_GetData (cCrowdSys_CROWD_GetData,
// bx/seg_1BA100).
//   cCrowdSys : cMidiSys (0x0) : cBankSys, virtual cBankManager
// cCrowdSys non-virtual part 0x118 [R]: its ctor cCrowdSys_cCrowdSys (inchrg != 0) puts cSoundSys at this + 0x118 and
// cBankManager at this + 0x120 (vtables D_00483818 / D_00483838 have deltas -0x118 / -0x120), stores vtable
// D_004837F8 at 0xC (_vt$9cCrowdSys: slot 1 cBankSys's cBankSys_SFX_Play2D, slot 2 pure virtual) and itself in
// D_004A36F8 (EA name gpCrowdSys). In cSSXAudio the next base, cMusicSys, starts at 0x118 (sound/ssxaudio.h). The struct
// below is the NON-VIRTUAL part. The preview's cCrowdSys / cCrowdInstance offsets agree with retail (asserts checked
// at preview 0x275AF8..0x278E0C).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline. Evidence tags:
//   [R] retail constructors / destructors (cCrowdSys_cCrowdSys / cCrowdSys__cCrowdSys, cCrowdInstance func_002A4D68 /
//       func_002A4E38), allocation tags, the crowd.inf keys func_002A7040 parses;
//   [P] the July 2003 preview: assert strings next to the field they test, CROWD_GetData's linkonce;
//   [U] how the retail methods use the field.
// On Tour has no crowd sound system: no [O] evidence. Struct names other than cCrowdSys / cCrowdInstance are ours.

#include "layout.h"
#include "bx/math3d.h"

struct cBankManager;
struct tMidiBank;

// One crowd sample list (0x18; our name): up to 5 loaded sample banks (kCROWDSYS_MAXVARIATIONS_PER_CATEGORY), one
// picked at random. Methods: ctor func_002A4BA8, dtor func_002A4BE0, Add func_002A4C28 (FILE_loadz the file),
// Pick func_002A4CA0 (mpData[BXrand() % mNumEventData]), Clear func_002A4CF8 (cMemMan_free each).
struct tCrowdEventData {
    int mNumEventData; // 0x00 [P] assert "mNumEventData < kCROWDSYS_MAXVARIATIONS_PER_CATEGORY" (Add); [R] ctor 0
    void* mpData[5];   // 0x04 [P] asserts "mpData[ mNumEventData ] == NULL", "mpData[ iCount ]"; [R] ctor zeroes 5
};

// One crowd intensity's cheer data (0x54; [P] cCrowdSys::CROWD_GetData(eCrowdIntensity) returns &mCrowdData[i]).
// The crowd.inf keys (func_002A7040, "<KEY><intensity + 1>") name the members.
struct tCrowdData {
    tCrowdEventData mTrick;      // 0x00 [R] crowd.inf TRICK%d sample files
    tCrowdEventData mFall;       // 0x18 [R] crowd.inf FALL%d
    tCrowdEventData mAnticipate; // 0x30 [R] crowd.inf ANTICIPATE%d
    float mAttack;               // 0x48 [R] crowd.inf ATTACK%d (AssignFloat); [U] cheer volume ramp (func_002A68B0), start time (func_002A64A0: mAttack + mDecay)
    float mDecay;                // 0x4C [R] crowd.inf DECAY%d (AssignFloat); [U] func_002A68B0
    float mMaxBend;              // 0x50 [R] crowd.inf MAXBEND%d (AssignFloat); [U] func_002A68B0 scales the cheer's bend by it
};

// One crowd object of a cCrowdInstance (0xC; our struct name, EA member names from the preview's asserts).
struct tCrowdAmbientInfo;
struct tCrowdObjectInfo {
    const t4Vector* pCrowdObjectPos; // 0x0 [P] "mCrowdObjectsInfo[ crowdObjectHandle ].pCrowdObjectPos != NULL", "pCurrCrowdObjectInfo->pCrowdObjectPos"; [U] 0 = free slot
    int unk_4;                       // 0x4 [U] set per object through cCrowdSys func_002A7340 -> func_002A5CD8; func_002A4E88 weights the ambient loops by it
    tCrowdAmbientInfo* pInfo;        // 0x8 [P] "pCurrCrowdObjectInfo->pInfo" (the ambient loop the object feeds; func_002A4E88 calls it pCurrAmbientInfo)
};

// One ambient loop of a cCrowdInstance (0x30; our struct name, EA member names from the preview's asserts).
struct tCrowdAmbientInfo {
    int sampleID;      // 0x00 [P] "mCrowdAmbientInfo[ iCount ].sampleID == -1" (preview lw 0xE0 + i * 0x30); [R] ctor -1
    int handle;        // 0x04 [P] "mCrowdAmbientInfo[ iCount ].handle == -1" (preview lw 0xE4 + i * 0x30); [R] ctor -1
    int numObjects;    // 0x08 [P] "pInfo->numObjects"; [R] ctor 0; [U] crowd objects feeding this loop
    char unk_0C[0x4];  // 0x0C padding (t4Vector alignment)
    t4Vector unk_10;   // 0x10 [U] func_002A4E88: the loop's averaged object position
    int unk_20;        // 0x20 [R] ctor stores the loop's index (0..2); [U] func_002A4E88 indexes its per-loop sums with it
    char unk_24[0xC];  // 0x24 padding (16-byte stride)
};

// cCrowdInstance: one viewport's crowd sound (0x190 [R]: cCrowdSys::Init (func_002A6D78) allocates
// operator new[](n x 0x190, "CrowdInstances"), every cCrowdSys method indexes mpCrowdInstances with stride 0x190).
// Class name from that tag and the preview's "pCrowdInstance" / "kCrowdInstanceTagBits". No vtable.
struct cCrowdInstance {
    int mViewPort;                             // 0x000 [P] "mViewPort >= 0", "mViewPort == -1", "mViewPort == pOpts->OPTS_GetViewport()"; [R] ctor -1; [U] OPTS_Viewport of its sounds (func_002A64A0)
    char unk_004[0xC];                         // 0x004 no access (t4Vector alignment)
    t4Vector unk_010;                          // 0x010 [R] ctor zeroes; [U] func_002A4E88 / func_002A5D08 (sq): the crowd's averaged position; func_002A64A0 passes &unk_010 as the cheer's OPTS_Position
    tCrowdObjectInfo mCrowdObjectsInfo[15];    // 0x020 [P] "crowdObjectHandle < kCROWDSYS_MAXCROWDOBJECTS_PER_VIEWPORT" (15: preview slti 15); [R] ctor zeroes
    int mNumCrowdObjects;                      // 0x0D4 [P] "mNumCrowdObjects > 0" (preview lw 0xD4); [R] ctor 0
    int unk_0D8;                               // 0x0D8 [R] ctor -1; cCrowdSys::Init sets every instance's to its 4th argument; [U] the MIDI bank of the cheer (func_002A64A0 -> MIDI_Begin(unk_0D8, sample))
    char unk_0DC[0x4];                         // 0x0DC no access (padding)
    tCrowdAmbientInfo mCrowdAmbientInfo[3];    // 0x0E0 [P] the asserts above; [R] ctor
    int mNumCrowdAmbience;                     // 0x170 [P] "mNumCrowdAmbience" (preview lw 0x170); [R] ctor 0
    int unk_174;                               // 0x174 [R] ctor -1; [U] the cheer's voice handle (func_002A64A0 = MIDI_End(); func_002A6848 BANKMONITOR_StopHandle()s it and resets -1)
    int unk_178;                               // 0x178 [R] ctor 0; [U] the cheer's base volume (func_002A64A0 OPTS_Vol); func_002A4E88 / func_002A62F0 write it
    float* unk_17C;                            // 0x17C [R] ctor 0; [U] func_002A64A0 stores it as the cheer's OPTS_VolControl (a float*)
    int unk_180;                               // 0x180 [R] ctor 0; [U] cheer state 0/1/2 (func_002A67F0 0 -> 1, func_002A6808 / func_002A6848 -> 0, func_002A62F0 2 -> 1)
    int mAmbientCheerIntensity;                // 0x184 [P] "mAmbientCheerIntensity != keCrowdIntensity_Num" (preview lw 0x184 vs 3); [R] ctor 3 (keCrowdIntensity_Num); [U] func_002A64A0 / func_002A6648 store the intensity played
    float unk_188;                             // 0x188 [R] ctor 0; [U] cheer time left: func_002A64A0 = mAttack + mDecay, func_002A68B0 counts it down, func_002A6848 / func_002A62F0 clamp it to 0.25
    int unk_18C;                               // 0x18C [R] ctor 0; [U] func_002A64A0 = intensity + 1, func_002A62F0 / func_002A4E88 0; cCrowdSys func_002A77C8 returns it
};

struct cCrowdSys {
    // --- cBankSys (non-virtual part 0x10; sound/banksys.h)
    cBankManager* mpVBankManager;      // 0x000 [R] virtual-base pointer (this + 0x120 in a complete cCrowdSys; cSSXAudio + 0x6C98)
    int* mBank;                        // 0x004 [O] cBankSys::mBank; [R] cBankSys ctor "mBank"
    int* mPatch;                       // 0x008 [O] cBankSys::mPatch; [R] cBankSys ctor "mPath"
    void** mVtbl;                      // 0x00C [R] D_004837F8 (cCrowdSys ctor/dtor); cSSXAudio_vtbl in cSSXAudio
    // --- cMidiSys (sound/midisys.h)
    tMidiBank* mMidiBanks;             // 0x010 [R] cMidiSys ctor operator new(0x20, "MIDIBANKS")
    // --- cCrowdSys
    int mNumInstances;                 // 0x014 [P] "viewport >= 0 && viewport < mNumInstances" (preview lw 0x14); [R] ctor 0; Init (func_002A6D78) stores n; [U] func_002A6D18 / func_002A7718 / func_002A77C8 loop to it
    cCrowdInstance* mpCrowdInstances;  // 0x018 [P] "mpCrowdInstances == NULL" (preview lw 0x18); [R] ctor 0; Init: operator new[](n x 0x190, "CrowdInstances"); DeInit func_002A6F38 destroys and frees them
    tCrowdData mCrowdData[3];          // 0x01C [P] CROWD_GetData(eCrowdIntensity) returns this + 0x1C + i * 0x54; [R] ctor constructs 3 (each list by func_002A4BA8); Init/DeInit clear them
};

CHECK_OFFSET(tCrowdEventData, mNumEventData, 0x00);
CHECK_OFFSET(tCrowdEventData, mpData, 0x04);
CHECK_SIZE(tCrowdEventData, 0x18);
CHECK_OFFSET(tCrowdData, mTrick, 0x00);
CHECK_OFFSET(tCrowdData, mFall, 0x18);
CHECK_OFFSET(tCrowdData, mAnticipate, 0x30);
CHECK_OFFSET(tCrowdData, mAttack, 0x48);
CHECK_OFFSET(tCrowdData, mDecay, 0x4C);
CHECK_OFFSET(tCrowdData, mMaxBend, 0x50);
CHECK_SIZE(tCrowdData, 0x54);
CHECK_OFFSET(tCrowdObjectInfo, pCrowdObjectPos, 0x0);
CHECK_OFFSET(tCrowdObjectInfo, unk_4, 0x4);
CHECK_OFFSET(tCrowdObjectInfo, pInfo, 0x8);
CHECK_SIZE(tCrowdObjectInfo, 0xC);
CHECK_OFFSET(tCrowdAmbientInfo, sampleID, 0x00);
CHECK_OFFSET(tCrowdAmbientInfo, handle, 0x04);
CHECK_OFFSET(tCrowdAmbientInfo, numObjects, 0x08);
CHECK_OFFSET(tCrowdAmbientInfo, unk_0C, 0x0C);
CHECK_OFFSET(tCrowdAmbientInfo, unk_10, 0x10);
CHECK_OFFSET(tCrowdAmbientInfo, unk_20, 0x20);
CHECK_OFFSET(tCrowdAmbientInfo, unk_24, 0x24);
CHECK_SIZE(tCrowdAmbientInfo, 0x30);
CHECK_OFFSET(cCrowdInstance, mViewPort, 0x000);
CHECK_OFFSET(cCrowdInstance, unk_004, 0x004);
CHECK_OFFSET(cCrowdInstance, unk_010, 0x010);
CHECK_OFFSET(cCrowdInstance, mCrowdObjectsInfo, 0x020);
CHECK_OFFSET(cCrowdInstance, mNumCrowdObjects, 0x0D4);
CHECK_OFFSET(cCrowdInstance, unk_0D8, 0x0D8);
CHECK_OFFSET(cCrowdInstance, unk_0DC, 0x0DC);
CHECK_OFFSET(cCrowdInstance, mCrowdAmbientInfo, 0x0E0);
CHECK_OFFSET(cCrowdInstance, mNumCrowdAmbience, 0x170);
CHECK_OFFSET(cCrowdInstance, unk_174, 0x174);
CHECK_OFFSET(cCrowdInstance, unk_178, 0x178);
CHECK_OFFSET(cCrowdInstance, unk_17C, 0x17C);
CHECK_OFFSET(cCrowdInstance, unk_180, 0x180);
CHECK_OFFSET(cCrowdInstance, mAmbientCheerIntensity, 0x184);
CHECK_OFFSET(cCrowdInstance, unk_188, 0x188);
CHECK_OFFSET(cCrowdInstance, unk_18C, 0x18C);
CHECK_SIZE(cCrowdInstance, 0x190);
CHECK_OFFSET(cCrowdSys, mpVBankManager, 0x000);
CHECK_OFFSET(cCrowdSys, mBank, 0x004);
CHECK_OFFSET(cCrowdSys, mPatch, 0x008);
CHECK_OFFSET(cCrowdSys, mVtbl, 0x00C);
CHECK_OFFSET(cCrowdSys, mMidiBanks, 0x010);
CHECK_OFFSET(cCrowdSys, mNumInstances, 0x014);
CHECK_OFFSET(cCrowdSys, mpCrowdInstances, 0x018);
CHECK_OFFSET(cCrowdSys, mCrowdData, 0x01C);
CHECK_SIZE(cCrowdSys, 0x118);

// The crowd system: cCrowdSys_cCrowdSys stores itself here, cCrowdSys__cCrowdSys clears it. EA name gpCrowdSys (preview assert
// "gpCrowdSys" on the same gp-relative global; candidate for a symbol_addrs rename D_004A36F8 -> gpCrowdSys).
extern cCrowdSys* D_004A36F8;       // [R] the cCrowdSys ctor cCrowdSys_cCrowdSys stores self, its dtor clears it

#endif /* SOUND_CROWDSYS_H */
