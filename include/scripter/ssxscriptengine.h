#ifndef SCRIPTER_SSXSCRIPTENGINE_H
#define SCRIPTER_SSXSCRIPTENGINE_H

// cSSXScriptEngine: the NIS / in-game cut-scene script engine (one instance: ssxapp's
// `cSSXScriptEngine_cSSXScriptEngine(cMemMan_alloc(0xA50, "ScriptEngine", 0, 0))`; the class static
// cSSXScriptEngine::mpScriptEngine is D_004A34B0, set by the constructor and cleared by the destructor).
// Its methods live in scripter/datamanager (ctor, dtor, InitOnce, DeInitOnce) and scripter/ssxscriptengine
// (0x00278358 Load ... 0x0027BF90 ClearNISPlayListState).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet.
// The real class (On Tour DWARF; retail vtables in builds/x/v2/vtables_v2.tsv) is
//   cSSXScriptEngine : cBXScriptEngine, cVidEngine, cDataManager
//   cBXScriptEngine  : cScriptDatabaseObject (0x000, size 0x2AC, vptr at 0x2A8), cInSeqComm (0x2AC, vptr only)
// and g++ 2.95 puts each base's vptr after that base's own data, so the object holds four vtable pointers:
//   0x2A8 _vt$16cSSXScriptEngine (primary), 0x2AC ...$10cInSeqComm, 0x518 ...$10cVidEngine, 0x528 ...$12cDataManager.
// The bases' fields are written out below (each block says which base it is); member objects
// (camera managers, game-state / event script objects, bank managers) stay raw bytes.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cSSXScriptEngine and its bases), placed by:
//   [P] the July 2003 preview: GetActiveNIS = lw 0x548 (retail 0x550, tools/layout_xlate.py "seen"),
//       IsBankAvailable reads the bank array at 0x628 (same offset as retail);
//   [R] retail's own initialisation: the constructor (cSSXScriptEngine_cSSXScriptEngine), InitOnce, Load and the
//       base constructors, with allocation tags read from the retail ELF ("SetupInfoHeap", "NISLists", "IdleLists",
//       "BXScriptDatabase", "InSeq", "InSeqHeap", "tDataInstanceInfo");
//   [U] how retail code uses the field;
//   [O] On Tour's member order. Retail's own block starts at 0x52C, On Tour's at 0x520 (retail's cVidEngine is
//       0x1C, not 0x10), and retail = On Tour + 0xC up to mNumFramesNoSkip; retail then has 50 anchor tables
//       (On Tour 60), 2 camera managers (On Tour 4) and a 0xFC-byte cScriptObjEvent (On Tour 0xF8).
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "scripter/scriptdatabaseobject.h"
#include "scripter/scriptobjgs.h"
#include "scripter/scanimmgr.h"
#include "scripter/scriptobjevent.h"
#include "inseq/scriptplayerchannel.h"   // tInSeqObject (tMainAnim / tSound data)
#include "scripter/scriptobjcammgr.h"

struct cScriptObject;
struct cInSeq;
struct cMoviePlayer;
struct tBXScriptState;
struct tDataInstanceInfo;
struct tSSXHiveScriptMasterTableHeader;
struct tSSXHiveScriptMaster;
struct tSSXHiveScriptFilterTableHeader;
struct tSSXHiveScriptFilterCategory;
struct tNISList;

// One script data bank (On Tour struct tSSXScriptBankInfo, 0x10; retail clears it with tSSXScriptBankInfo_Clear).
// The hive script tables (On Tour, both 2 variants: the file stores offsets, InitOnce / ProcessAsyncLoadTrackTableComplete
// turn them into pointers in place).
// tSSXHiveScriptMaster (0xC): one script of the master table. [U] cSSXScriptEngine_RequestScriptByHashID compares
// hashID (+0x4) while walking mpMasterTable 0xC apart (view sEnt_002796B8).
union tSSXHiveScriptNameDebug {
    const char* pScriptName;        // [O]
    unsigned int scriptNameOffset;  // [O]
};

struct tSSXHiveScriptMaster {
    int numTimesPlayed;             // 0x0 [O]
    unsigned int hashID;            // 0x4 [O]; [U] RequestScriptByHashID
    tSSXHiveScriptNameDebug debugNamePtr; // 0x8 [O]
};

CHECK_OFFSET(tSSXHiveScriptMaster, numTimesPlayed, 0x0);
CHECK_OFFSET(tSSXHiveScriptMaster, hashID, 0x4);
CHECK_OFFSET(tSSXHiveScriptMaster, debugNamePtr, 0x8);
CHECK_SIZE(tSSXHiveScriptMaster, 0xC);

// The master script table file (On Tour struct tSSXHiveScriptMasterTableHeader, 0x10): the count, then the
// tSSXHiveScriptMaster records. [O]; [R] InitOnce loads it into mpMasterTableHeader and points mpMasterTable at
// &masterTableStart (header + 4); [U] its loops run to numScripts (view sDmEngine8210 hdr).
struct tSSXHiveScriptMasterTableHeader {
    int numScripts;                 // 0x0 [O]; [U] the master table's loop bound
    tSSXHiveScriptMaster masterTableStart; // 0x4 [O]; [R] mpMasterTable = &masterTableStart (the first record)
};

CHECK_OFFSET(tSSXHiveScriptMasterTableHeader, numScripts, 0x0);
CHECK_OFFSET(tSSXHiveScriptMasterTableHeader, masterTableStart, 0x4);
CHECK_SIZE(tSSXHiveScriptMasterTableHeader, 0x10);

// tSSXHiveScriptFilter (0x30): one script of a category with its 21 character filters; tSSXHiveScriptFilterCategory
// (0x8): a category's count and script list. [U] cSSXScriptEngine_GetScriptFromCategory indexes mpCategoryFilterTable
// 8 apart and the scripts 0x30 apart (views sCategory_27B0C0, sScript_27B0C0).
struct tSSXHiveScriptFilter {
    int scriptIndex;                // 0x00 [O]
    short charFilters[21];          // 0x04 [O]; [U] GetScriptFromCategory tests the riders' entries
};

CHECK_OFFSET(tSSXHiveScriptFilter, scriptIndex, 0x00);
CHECK_OFFSET(tSSXHiveScriptFilter, charFilters, 0x04);
CHECK_SIZE(tSSXHiveScriptFilter, 0x30);

union tSSXHiveScriptFilterPtr {
    tSSXHiveScriptFilter* pScriptFilter; // [O]
    unsigned int scriptFilterOffset;     // [O]
};

struct tSSXHiveScriptFilterCategory {
    int numScripts;                 // 0x0 [O]; [U] GetScriptFromCategory's count
    tSSXHiveScriptFilterPtr scriptFilterPtr; // 0x4 [O]; [U] the category's scripts
};

CHECK_OFFSET(tSSXHiveScriptFilterCategory, numScripts, 0x0);
CHECK_OFFSET(tSSXHiveScriptFilterCategory, scriptFilterPtr, 0x4);
CHECK_SIZE(tSSXHiveScriptFilterCategory, 0x8);

// The per-track script filter table file (On Tour struct tSSXHiveScriptFilterTableHeader, 0xC): the category count,
// then the tSSXHiveScriptFilterCategory records. [O]; [U] cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete stores
// the loaded file in mpFilterTableHeader, loops to numScriptCategories and points mpCategoryFilterTable at header + 4.
struct tSSXHiveScriptFilterTableHeader {
    int numScriptCategories;        // 0x0 [O]; [U] the category loop bound
    tSSXHiveScriptFilterCategory scriptFilterCategoryStart; // 0x4 [O]; [U] mpCategoryFilterTable = header + 4
};

CHECK_OFFSET(tSSXHiveScriptFilterTableHeader, numScriptCategories, 0x0);
CHECK_OFFSET(tSSXHiveScriptFilterTableHeader, scriptFilterCategoryStart, 0x4);
CHECK_SIZE(tSSXHiveScriptFilterTableHeader, 0xC);

// tSSXScriptHeader (0x20): the SSX header at the start of a script's data (GetSSXScriptHeader). [U]
// tNISList_DoTransitionToNext reads the end transition (+0x10 type, +0x11 colour, +0x12 / +0x14 / +0x16 frames;
// view sDmFade_7980).
struct tSSXScriptHeader {
    signed char isTransport;        // 0x00 [O]
    signed char padding[3];         // 0x01 [O]
    int track;                      // 0x04 [O]
    signed char startTransitionType; // 0x08 [O]
    signed char startTransitionColour; // 0x09 [O]
    short startFadeToTransitionFrames; // 0x0A [O]
    short startHoldTransitionFrames;   // 0x0C [O]
    short startFadeFromTransitionFrames; // 0x0E [O]
    signed char endTransitionType;  // 0x10 [O]; [U] DoTransitionToNext
    signed char endTransitionColour; // 0x11 [O]; [U] ditto
    short endFadeToTransitionFrames; // 0x12 [O]; [U] ditto
    short endHoldTransitionFrames;  // 0x14 [O]; [U] ditto
    short endFadeFromTransitionFrames; // 0x16 [O]; [U] ditto
    int scriptIndex;                // 0x18 [O]
    int bankNum;                    // 0x1C [O]
};

CHECK_OFFSET(tSSXScriptHeader, isTransport, 0x00);
CHECK_OFFSET(tSSXScriptHeader, track, 0x04);
CHECK_OFFSET(tSSXScriptHeader, startTransitionType, 0x08);
CHECK_OFFSET(tSSXScriptHeader, startTransitionColour, 0x09);
CHECK_OFFSET(tSSXScriptHeader, startFadeToTransitionFrames, 0x0A);
CHECK_OFFSET(tSSXScriptHeader, startHoldTransitionFrames, 0x0C);
CHECK_OFFSET(tSSXScriptHeader, startFadeFromTransitionFrames, 0x0E);
CHECK_OFFSET(tSSXScriptHeader, endTransitionType, 0x10);
CHECK_OFFSET(tSSXScriptHeader, endTransitionColour, 0x11);
CHECK_OFFSET(tSSXScriptHeader, endFadeToTransitionFrames, 0x12);
CHECK_OFFSET(tSSXScriptHeader, endHoldTransitionFrames, 0x14);
CHECK_OFFSET(tSSXScriptHeader, endFadeFromTransitionFrames, 0x16);
CHECK_OFFSET(tSSXScriptHeader, scriptIndex, 0x18);
CHECK_OFFSET(tSSXScriptHeader, bankNum, 0x1C);
CHECK_SIZE(tSSXScriptHeader, 0x20);

// tSSXScriptSetupInfo (0x10): a script request's cast and view (On Tour tSSXScriptSetupInfo; same layout). [U]
// cSSXScriptEngine_RequestScriptByCategory fills the three AI indexes from its parameters of the same names;
// cSSXScriptEngine_GetSubjectRider reads them for subjects 0..2 (a negative index = no rider);
// cSSXScriptEngine_ReleaseScriptSetupInfo sets all four words to -1; PlayRequestScript stores the view index and
// cScriptObjCameraManager_IsMatchingEntity compares it.
struct tSSXScriptSetupInfo {
    int mainCharAIIndex;            // 0x0 [O]; [U] subject 0
    int supportChar0AIIndex;        // 0x4 [O]; [U] subject 1
    int supportChar1AIIndex;        // 0x8 [O]; [U] subject 2
    int viewIndex;                  // 0xC [O]; [U] PlayRequestScript / IsMatchingEntity
};

CHECK_OFFSET(tSSXScriptSetupInfo, mainCharAIIndex, 0x0);
CHECK_OFFSET(tSSXScriptSetupInfo, supportChar0AIIndex, 0x4);
CHECK_OFFSET(tSSXScriptSetupInfo, supportChar1AIIndex, 0x8);
CHECK_OFFSET(tSSXScriptSetupInfo, viewIndex, 0xC);
CHECK_SIZE(tSSXScriptSetupInfo, 0x10);

// tSSXScriptSetupInfoLinked (0x14): a setup info on the engine's free list (On Tour : tSSXScriptSetupInfo + pNext).
// [R] Load allocates 10 x 0x14 ("SetupInfoHeap", mpSetupInfoHeap). FLAT: the base's members are written out.
struct tSSXScriptSetupInfoLinked {
    // --- tSSXScriptSetupInfo
    int mainCharAIIndex;            // 0x00 [O]
    int supportChar0AIIndex;        // 0x04 [O]
    int supportChar1AIIndex;        // 0x08 [O]
    int viewIndex;                  // 0x0C [O]
    // --- tSSXScriptSetupInfoLinked
    tSSXScriptSetupInfoLinked* pNext; // 0x10 [O]; [U] AllocScriptSetupInfo pops mpSetupInfoUnused through it and
                                    //      clears it; ReleaseScriptSetupInfo pushes the record back
};

CHECK_OFFSET(tSSXScriptSetupInfoLinked, mainCharAIIndex, 0x00);
CHECK_OFFSET(tSSXScriptSetupInfoLinked, viewIndex, 0x0C);
CHECK_OFFSET(tSSXScriptSetupInfoLinked, pNext, 0x10);
CHECK_SIZE(tSSXScriptSetupInfoLinked, 0x14);

// tSSXHeaderActor: the SSX header of an actor entity's script data (GetSSXActorHeader; On Tour tSSXHeaderActor, 0x30
// there). Retail has no 64-bit `character` word at 0x8: its base position and rotation sit 0xC lower than On Tour's
// (0x8.. here, 0x14.. there); what retail keeps of On Tour's 0x2..0x13 (character, hidden, scriptEndLocation,
// hideIllegalBoltOn, isRidingBoard) is in 0x2..0x7: only the character mask is placed (a short at 0x2; On Tour's is
// 64-bit at 0x8). Laid out through the last member retail reads.
struct tSSXHeaderActor {
    signed char role;               // 0x00 [O]; [U] cSSXScriptEngine_OnStartScript maps it to a character slot through
                                    //      D_00482608 (view sScriptOpt_9A70 slot)
    signed char anchorPos;          // 0x01 [O]; [U] cScriptObjRider_OnStartObjectUse passes it to GetAnchorOrientation
    short character;                // 0x02 [O] character (the 64-bit character mask at 0x8 there); [U]
                                    //      cSSXScriptEngine_OnStartScript: 0 or a mask that meets the free
                                    //      characters' flags picks the variation (view sScriptOpt_9A70 mask)
    char unk_004[0x4];              // 0x04 see above
    float basePosX;                 // 0x08 [O] (0x14 there); [U] OnStartObjectUse's base offset x
    float basePosY;                 // 0x0C [O]; [U] ... y
    float basePosZ;                 // 0x10 [O]; [U] ... z
    float baseRotZ;                 // 0x14 [O]; [U] OnStartObjectUse: degrees -> rz (+0x78), applied negated about Z
    float baseRotY;                 // 0x18 [O]; [U] ... ry (+0x74), about Y
    float baseRotX;                 // 0x1C [O]; [U] ... rx (+0x70), about X
};

CHECK_OFFSET(tSSXHeaderActor, role, 0x00);
CHECK_OFFSET(tSSXHeaderActor, anchorPos, 0x01);
CHECK_OFFSET(tSSXHeaderActor, character, 0x02);
CHECK_OFFSET(tSSXHeaderActor, unk_004, 0x04);
CHECK_OFFSET(tSSXHeaderActor, basePosX, 0x08);
CHECK_OFFSET(tSSXHeaderActor, basePosY, 0x0C);
CHECK_OFFSET(tSSXHeaderActor, basePosZ, 0x10);
CHECK_OFFSET(tSSXHeaderActor, baseRotZ, 0x14);
CHECK_OFFSET(tSSXHeaderActor, baseRotY, 0x18);
CHECK_OFFSET(tSSXHeaderActor, baseRotX, 0x1C);
CHECK_SIZE(tSSXHeaderActor, 0x20);

// The per-frame script data of an actor (On Tour tSSXFrameActor and its channel records). GetSSXActorFrame fills a
// tSSXFrameActor for the current frame; the rider walks its three channels: the main animations
// (cRider_DoInSeqAnimUpdate), the sounds (cRider_DoInSeqSoundUpdate) and the speech events (cRider_OnInSeqKeyEvent).
// Same sizes and offsets as On Tour. [Staged by views2 group A.]

// Each channel record starts with a tInSeqObject (inseq/scriptplayerchannel.h, On Tour struct tInSeqObject, 0x1C):
// objectID (the anim name / script sound id), scriptStartFrame (DoInSeqSoundUpdate plays the sound on it), trackIndex
// (the sound handle's slot), currObjectFrame, playRate, blendInFrames (DoInSeqAnimUpdate).

// A main animation's custom tag (On Tour struct tTagCustomMainAnim, 0x4).
struct tTagCustomMainAnim {
    signed char isFlipped;          // 0x0 [O]; [U] DoInSeqAnimUpdate (lb)
    signed char alignBody;          // 0x1 [O]; [U] DoInSeqAnimUpdate (lb)
    signed char applySGOffsets;     // 0x2 [O]; [U] DoInSeqAnimUpdate (lb)
    signed char padding;            // 0x3 [O]
};

CHECK_OFFSET(tTagCustomMainAnim, isFlipped, 0x0);
CHECK_OFFSET(tTagCustomMainAnim, alignBody, 0x1);
CHECK_OFFSET(tTagCustomMainAnim, applySGOffsets, 0x2);
CHECK_OFFSET(tTagCustomMainAnim, padding, 0x3);
CHECK_SIZE(tTagCustomMainAnim, 0x4);

// A sound's custom tag (On Tour struct tTagCustomSound, 0x4).
struct tTagCustomSound {
    signed char volume;             // 0x0 [O]; [U] DoInSeqSoundUpdate clamps it to 0..127
    signed char padding[3];         // 0x1 [O]
};

CHECK_OFFSET(tTagCustomSound, volume, 0x0);
CHECK_OFFSET(tTagCustomSound, padding, 0x1);
CHECK_SIZE(tTagCustomSound, 0x4);

// A speech event's custom tag (On Tour struct tTagCustomSpeechEvent, 0x4).
struct tTagCustomSpeechEvent {
    signed char subject;            // 0x0 [O]; [U] OnInSeqKeyEvent: cSSXScriptEngine_GetSubjectRider's subject
    signed char padding[3];         // 0x1 [O]
};

CHECK_OFFSET(tTagCustomSpeechEvent, subject, 0x0);
CHECK_OFFSET(tTagCustomSpeechEvent, padding, 0x1);
CHECK_SIZE(tTagCustomSpeechEvent, 0x4);

// A main animation of the frame (On Tour struct tMainAnim, 0x24): a list linked through pNext.
struct tMainAnim {
    tInSeqObject data;              // 0x00 [O]
    tTagCustomMainAnim* pCustom;    // 0x1C [O]; [U] DoInSeqAnimUpdate reads its three flags
    tMainAnim* pNext;               // 0x20 [O]
};

CHECK_OFFSET(tMainAnim, data, 0x00);
CHECK_OFFSET(tMainAnim, pCustom, 0x1C);
CHECK_OFFSET(tMainAnim, pNext, 0x20);
CHECK_SIZE(tMainAnim, 0x24);

// tSound (0x24) and tSpeechEvent (0x8), the frame's sounds and speech events: scripter/scriptobjevent.h.

// An actor's frame (On Tour struct tSSXFrameActor, 0x30; [O] GetSSXActorFrame(cScriptPlayerEntity*, tSSXFrameActor*)).
// [R] retail GetSSXActorFrame stores exactly these 12 words.
struct tSSXFrameActor {
    int numMainAnim;                // 0x00 [O]; [U] DoInSeqAnimUpdate tests it
    tMainAnim* pMainAnimList;       // 0x04 [O]; [U] DoInSeqAnimUpdate
    int numSound;                   // 0x08 [O]
    tSound* pSoundList;             // 0x0C [O]; [U] DoInSeqSoundUpdate walks it
    int numSpeechEvent;             // 0x10 [O]; [U] OnInSeqKeyEvent loops to it
    tSpeechEvent* pSpeechEventArray; // 0x14 [O]; [U] OnInSeqKeyEvent
    float posX;                     // 0x18 [O]; [U] DoInSeqAnimUpdate
    float posY;                     // 0x1C [O]
    float posZ;                     // 0x20 [O]
    float rotX;                     // 0x24 [O]; [U] DoInSeqAnimUpdate (degrees)
    float rotY;                     // 0x28 [O]
    float rotZ;                     // 0x2C [O]
};

CHECK_OFFSET(tSSXFrameActor, numMainAnim, 0x00);
CHECK_OFFSET(tSSXFrameActor, pMainAnimList, 0x04);
CHECK_OFFSET(tSSXFrameActor, numSound, 0x08);
CHECK_OFFSET(tSSXFrameActor, pSoundList, 0x0C);
CHECK_OFFSET(tSSXFrameActor, numSpeechEvent, 0x10);
CHECK_OFFSET(tSSXFrameActor, pSpeechEventArray, 0x14);
CHECK_OFFSET(tSSXFrameActor, posX, 0x18);
CHECK_OFFSET(tSSXFrameActor, posY, 0x1C);
CHECK_OFFSET(tSSXFrameActor, posZ, 0x20);
CHECK_OFFSET(tSSXFrameActor, rotX, 0x24);
CHECK_OFFSET(tSSXFrameActor, rotY, 0x28);
CHECK_OFFSET(tSSXFrameActor, rotZ, 0x2C);
CHECK_SIZE(tSSXFrameActor, 0x30);

// tSSXHeaderCameraAlgorithmic (0x20): the SSX header of an algorithmic camera entity's script data
// (GetSSXCameraAlgorithmicHeader; On Tour tSSXHeaderCameraAlgorithmic, same offsets). [U]
// cScriptObjCamera_CalculateBaseTransform_SE reads the three rotations (degrees) at On Tour's offsets;
// cScriptEntityCamera_OnInSeqLock reads the sub type at +0x1C (src/scripter/bxscriptengine.cpp).
struct tSSXHeaderCameraAlgorithmic {
    float unk_000;                  // 0x00 [O] nearPlane
    float unk_004;                  // 0x04 [O] farPlane
    float baseRotZ;                 // 0x08 [O]; [U] -> rz
    float baseRotY;                 // 0x0C [O]; [U] -> ry
    float baseRotX;                 // 0x10 [O]; [U] -> rx
    float unk_014;                  // 0x14 [O] stickiness
    float unk_018;                  // 0x18 [O] maxSpeed
    signed char subType;            // 0x1C [O]; [U] OnInSeqLock: mSubType = this byte (entity type 3)
    signed char unk_01D;            // 0x1D [O] subject
    signed char unk_01E;            // 0x1E [O] areAnglesWorldRelative
    signed char unk_01F;            // 0x1F [O] padding
};

CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_000, 0x00);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_004, 0x04);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotZ, 0x08);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotY, 0x0C);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotX, 0x10);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_014, 0x14);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_018, 0x18);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, subType, 0x1C);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_01D, 0x1D);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_01E, 0x1E);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_01F, 0x1F);
CHECK_SIZE(tSSXHeaderCameraAlgorithmic, 0x20);

struct tSSXScriptBankInfo {
    void* dataBankHandle; // 0x0 [O]; [R] cDataManager attach handle (SyncLoadScriptBank); [U] detached by PurgeScriptBank
    int dataIdentifier;   // 0x4 [O]; [R] the bank's key (SyncLoadScriptBank, cSSXScriptEngine_AsyncLoadScriptBank); Clear sets -1
    void* asyncHandle;    // 0x8 [O] (int in On Tour); [U] async file, compared with the completed file
                          //     (ProcessAsyncLoadScriptBankComplete), ASYNCFILE_cancel/release
    int dynamicMem;       // 0xC [O] (a bool in On Tour); [R] sw 1 when the engine allocated the data, 0 for a caller buffer
};

CHECK_OFFSET(tSSXScriptBankInfo, dataBankHandle, 0x0);
CHECK_OFFSET(tSSXScriptBankInfo, dataIdentifier, 0x4);
CHECK_OFFSET(tSSXScriptBankInfo, asyncHandle, 0x8);
CHECK_OFFSET(tSSXScriptBankInfo, dynamicMem, 0xC);
CHECK_SIZE(tSSXScriptBankInfo, 0x10);

struct cSSXScriptEngine {
    // --- cScriptDatabaseObject (base of cBXScriptEngine; ctor cScriptDatabaseObject_cScriptDatabaseObject, vtable _vt$21cScriptDatabaseObject)
    cScriptObject* mpObjectDatabase[20];         // 0x000 [O]; [R] cScriptDatabaseObject_cScriptDatabaseObject zeroes 20 words
    tScriptEntityRecord mEntityDatabase[30];     // 0x050 [O] tScriptEntityRecord[30]; [R] cScriptDatabaseObject_cScriptDatabaseObject sets 30
                                                 //       0x14-byte records to {0, -1, -1, 0, -1}
    void** mVtbl;                                // 0x2A8 [R] _vt$16cSSXScriptEngine (cSSXScriptEngine_vtbl), stored by the ctor
                                                 //       over _vt$15cBXScriptEngine; [U] cSSXScriptEngine_ForcePlayUpdate calls slot 4
    // --- cInSeqComm (second base of cBXScriptEngine: a vptr only)
    void** mInSeqCommVtbl;                       // 0x2AC [R] _vt$16cSSXScriptEngine$10cInSeqComm (D_00482100);
                                                 //       SetupBXEngine hands self+0x2AC to cInSeq_cInSeq
    // --- cBXScriptEngine (ctor cBXScriptEngine_cBXScriptEngine; methods in scripter/bxscriptengine)
    tBXScriptState* mpScriptDatabase;            // 0x2B0 [O]; [R] SetupBXEngine: new (count * 8, "BXScriptDatabase")
    int mScriptDatabaseSize;                     // 0x2B4 [O]; [R] SetupBXEngine stores the count
    cInSeq* mpInSeq;                             // 0x2B8 [O]; [R] SetupBXEngine: cInSeq_cInSeq(alloc(0x20, "InSeq"))
    char* mpInSeqHeap;                           // 0x2BC [O]; [R] SetupBXEngine: new (size, "InSeqHeap")
    char mScriptRequests[16][0x24];              // 0x2C0 [O] tScriptRequestInfo[16]; [R] ResetInstVars: 16 x 0x24
    // --- cVidEngine (ctor cVidEngine_cVidEngine(self+0x500), which zeroes 0x0-0x14 and sets the vptr at 0x18)
    cMoviePlayer* mpPlayer;                      // 0x500 [O]; [R] cVidEngine_ReadyVideo allocates the 0x44-byte player
    int mIsPlaying;                              // 0x504 [O] (a bool in On Tour); [U] cVidEngine_PlayVideo sets 1;
                                                 //       tested by ssxapp.cpp and all_in_one_fe.cpp
    int mFrameUpdated;                           // 0x508 [O] (a bool in On Tour); [U] cVidEngine_UpdateVideo sets 1 while
                                                 //       playing and not paused
    int mNumPause;                               // 0x50C [O]; [U] cVidEngine_PauseVideo counts it up and down
    int mSavedDisplayFormat;                     // 0x510 [U] cVidEngine_PlayVideo saves GrMan's TV display format;
                                                 //       ResetVideoEngine restores it (scripter/videngine.h)
    int mRestoreCutSceneMode;                    // 0x514 [U] cVidEngine_PlayVideo: 1 unless the cut-scene transition
                                                 //       state is 0 or 2; ResetVideoEngine clears it
    void** mVidEngineVtbl;                       // 0x518 [R] _vt$16cSSXScriptEngine$10cVidEngine (D_004821A0)
    // --- cDataManager (ctor cDataManager_cDataManager(self+0x51C, 0, 0x100))
    int mMaxDataInstances;                       // 0x51C [O]; [R] the ctor stores 0x100
    tDataInstanceInfo* mpDataInstanceHeap;       // 0x520 [O]; [R] new (0x100 * 0x18, "tDataInstanceInfo")
    tDataInstanceInfo* mpEmptyDataInstanceList;  // 0x524 [O]; [R] zeroed, then filled by AddToEmptyInstanceList
    void** mDataManagerVtbl;                     // 0x528 [R] _vt$16cSSXScriptEngine$12cDataManager (D_00482168)
    // --- cSSXScriptEngine's own members
    tSSXHiveScriptMasterTableHeader* mpMasterTableHeader; // 0x52C [O]; [R] InitOnce: FILE_loadz(kpScriptMasterTableFilename);
                                                 //       [U] its first word is the script count
    tSSXHiveScriptMaster* mpMasterTable;         // 0x530 [O]; [R] InitOnce: header + 4; [U] 0xC-byte records
                                                 //       {numTimesPlayed, hashID, name}: cSSXScriptEngine_OnStartScript counts a play,
                                                 //       cSSXScriptEngine_AddToNISListByScriptHashID / cSSXScriptEngine_RequestScriptByHashID search by hashID
    char* mpMasterTableDebug;                    // 0x534 [O]; [R] InitOnce: FILE_loadz(kpScriptMasterTableDebugFilename)
    void* mFilterTableAsyncHandle;               // 0x538 [O] (int in On Tour); [R] AsyncLoadTrack opens
                                                 //       ".../scfilter<location>" into it; [U] ASYNCFILE_* (PurgeTrack)
    tSSXHiveScriptFilterTableHeader* mpFilterTableHeader; // 0x53C [O]; [U] cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete stores the loaded table,
                                                 //       PurgeTrack frees it, GetScriptFromCategory tests it
    tSSXHiveScriptFilterCategory* mpCategoryFilterTable; // 0x540 [O]; [U] cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete: header + 4; 8-byte
                                                 //       {numScripts, scripts} records (GetScriptFromCategory)
    tSSXScriptSetupInfoLinked* mpSetupInfoHeap;  // 0x544 [O]; [R] Load: new (0xC8, "SetupInfoHeap"), 10 x 0x14
    tSSXScriptSetupInfoLinked* mpSetupInfoUnused; // 0x548 [O]; [U] free list (next at +0x10): cSSXScriptEngine_AllocScriptSetupInfo pops,
                                                 //       cSSXScriptEngine_ReleaseScriptSetupInfo pushes
    tNISList* mpNISLists;                        // 0x54C [O]; [R] Load: new[2] ("NISLists"), 0xCC each
    int mPlayingNISList;                         // 0x550 [P] GetActiveNIS (preview 0x548; IsNISActive: != 2);
                                                 //       [O] eNISList; [R] the ctor sets 2 (none)
    int mNumFramesNoSkip;                        // 0x554 [O]; [U] cSSXScriptEngine_DisallowNISListSkip stores its arg;
                                                 //       Update counts it down and ignores the skip button until 0
    unsigned int* mpAnchorTableDB[50];           // 0x558 [O] (tResourceID*[60] in On Tour); [R] the ctor zeroes 50;
                                                 //       [U] cSSXScriptEngine_AttachAnchorDataTable / cSSXScriptEngine_DetachAnchorDataTable (Attach/DetachAnchorDataTable)
                                                 //       index it by world section; cSSXScriptEngine_FillOrientationFromObjectOrientationInfo reads codes from it
    int mNumAsyncs;                              // 0x620 [O]; [U] +1 per async load started (AsyncLoadTrack,
                                                 //       cSSXScriptEngine_AsyncLoadScriptBank), -1 when it completes or is cancelled
    tNISList* mpIdleLists;                       // 0x624 [O]; [R] Load: new[2] ("IdleLists"), 0xCC each
    tSSXScriptBankInfo mScriptDataBanks[10];     // 0x628 [O]; [R] the ctor clears 10 with tSSXScriptBankInfo_Clear;
                                                 //       [P] IsBankAvailable reads 0x628 / 0x630 for bank i
    char unk_6C8[0x8];                           // 0x6C8 untouched; likely alignment (On Tour has the same gap
                                                 //       before its 16-byte-aligned camera managers)
    cScriptObjCameraManager mObjectCameraManager[2]; // 0x6D0 [O] (4 in On Tour); [R] the ctor
                                                 //       builds 2 x 0x120 (cScriptObjCameraManager_cScriptObjCameraManager); vptr at +0xC (dtor)
    cScriptObjGS mObjectGameState;                 // 0x910 [O] cScriptObjGS; [R] cScriptObjGS_cScriptObjGS(self+0x910);
                                                 //       vptrs at +0x0 and +0x18, cScriptObject part at +0xC (dtor)
    cScriptObjEvent mObjectEvent;                // 0x92C [O] cScriptObjEvent (0xF8 in On Tour; retail 0xFC, scripter/scriptobjevent.h);
                                                 //       [R] cScriptObjEvent_cScriptObjEvent(self+0x92C) writes up to +0xF8; the
                                                 //       engine dtor stores its two vptrs (+0x92C, +0x944) and runs
                                                 //       cScriptObject__cScriptObject on its cScriptObject part (+0x938)
    cScriptAnimBankManager mAnimationBankManager;  // 0xA28 [O] cScriptAnimBankManager; [R] its ctor (self+0xA28, 0, 10, 0xF0)
    char mSoundBankManager[0x10];                // 0xA38 [O] cScriptSoundBankManager; [R] its ctor (self+0xA38, 0, 10, 0x11)
    int mCurrAttachingBankNum;                   // 0xA48 [O]; [R] the ctor sets 10 (none); [U] SyncLoadScriptBank and
                                                 //       ProcessAsyncLoadScriptBankComplete set it around the cDataManager
                                                 //       attach, OnAttachData reads it
    char unk_A4C[0x4];                           // 0xA4C untouched; tail padding (On Tour also pads 0xCA8 -> 0xCB0)
};

CHECK_OFFSET(cSSXScriptEngine, mpObjectDatabase, 0x000);
CHECK_OFFSET(cSSXScriptEngine, mEntityDatabase, 0x050);
CHECK_OFFSET(cSSXScriptEngine, mVtbl, 0x2A8);
CHECK_OFFSET(cSSXScriptEngine, mInSeqCommVtbl, 0x2AC);
CHECK_OFFSET(cSSXScriptEngine, mpScriptDatabase, 0x2B0);
CHECK_OFFSET(cSSXScriptEngine, mScriptDatabaseSize, 0x2B4);
CHECK_OFFSET(cSSXScriptEngine, mpInSeq, 0x2B8);
CHECK_OFFSET(cSSXScriptEngine, mpInSeqHeap, 0x2BC);
CHECK_OFFSET(cSSXScriptEngine, mScriptRequests, 0x2C0);
CHECK_OFFSET(cSSXScriptEngine, mpPlayer, 0x500);
CHECK_OFFSET(cSSXScriptEngine, mIsPlaying, 0x504);
CHECK_OFFSET(cSSXScriptEngine, mFrameUpdated, 0x508);
CHECK_OFFSET(cSSXScriptEngine, mNumPause, 0x50C);
CHECK_OFFSET(cSSXScriptEngine, mSavedDisplayFormat, 0x510);
CHECK_OFFSET(cSSXScriptEngine, mRestoreCutSceneMode, 0x514);
CHECK_OFFSET(cSSXScriptEngine, mVidEngineVtbl, 0x518);
CHECK_OFFSET(cSSXScriptEngine, mMaxDataInstances, 0x51C);
CHECK_OFFSET(cSSXScriptEngine, mpDataInstanceHeap, 0x520);
CHECK_OFFSET(cSSXScriptEngine, mpEmptyDataInstanceList, 0x524);
CHECK_OFFSET(cSSXScriptEngine, mDataManagerVtbl, 0x528);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTableHeader, 0x52C);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTable, 0x530);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTableDebug, 0x534);
CHECK_OFFSET(cSSXScriptEngine, mFilterTableAsyncHandle, 0x538);
CHECK_OFFSET(cSSXScriptEngine, mpFilterTableHeader, 0x53C);
CHECK_OFFSET(cSSXScriptEngine, mpCategoryFilterTable, 0x540);
CHECK_OFFSET(cSSXScriptEngine, mpSetupInfoHeap, 0x544);
CHECK_OFFSET(cSSXScriptEngine, mpSetupInfoUnused, 0x548);
CHECK_OFFSET(cSSXScriptEngine, mpNISLists, 0x54C);
CHECK_OFFSET(cSSXScriptEngine, mPlayingNISList, 0x550);
CHECK_OFFSET(cSSXScriptEngine, mNumFramesNoSkip, 0x554);
CHECK_OFFSET(cSSXScriptEngine, mpAnchorTableDB, 0x558);
CHECK_OFFSET(cSSXScriptEngine, mNumAsyncs, 0x620);
CHECK_OFFSET(cSSXScriptEngine, mpIdleLists, 0x624);
CHECK_OFFSET(cSSXScriptEngine, mScriptDataBanks, 0x628);
CHECK_OFFSET(cSSXScriptEngine, unk_6C8, 0x6C8);
CHECK_OFFSET(cSSXScriptEngine, mObjectCameraManager, 0x6D0);
CHECK_OFFSET(cSSXScriptEngine, mObjectGameState, 0x910);
CHECK_OFFSET(cSSXScriptEngine, mObjectEvent, 0x92C);
CHECK_OFFSET(cSSXScriptEngine, mAnimationBankManager, 0xA28);
CHECK_OFFSET(cSSXScriptEngine, mSoundBankManager, 0xA38);
CHECK_OFFSET(cSSXScriptEngine, mCurrAttachingBankNum, 0xA48);
CHECK_OFFSET(cSSXScriptEngine, unk_A4C, 0xA4C);
CHECK_SIZE(cSSXScriptEngine, 0xA50);

// The one instance (On Tour: ScriptEngine), created and destroyed by cSSXApp ("ScriptEngine", 0xA50).
extern cSSXScriptEngine* ScriptEngine;

#endif /* SCRIPTER_SSXSCRIPTENGINE_H */
