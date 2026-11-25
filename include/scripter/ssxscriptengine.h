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
struct cScriptPlayerMain;
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
// hideIllegalBoltOn, isRidingBoard) is in 0x2..0x7, in On Tour's order (the character mask is a short at 0x2; On
// Tour's is 64-bit at 0x8). Laid out through the last member retail reads.
struct tSSXHeaderActor {
    signed char role;               // 0x00 [O]; [U] cSSXScriptEngine_OnStartScript maps it to a character slot through
                                    //      kRoleToFilterEnumMappingTable (view sScriptOpt_9A70 slot)
    signed char anchorPos;          // 0x01 [O]; [U] cScriptObjRider_OnStartObjectUse passes it to GetAnchorOrientation
    short character;                // 0x02 [O] character (the 64-bit character mask at 0x8 there); [U]
                                    //      cSSXScriptEngine_OnStartScript: 0 or a mask that meets the free
                                    //      characters' flags picks the variation (view sScriptOpt_9A70 mask)
    signed char hidden;             // 0x04 [O] (0x10 there); [U] cRider_OnStartObjectUse: mHiddenNIS = hidden != 0
    signed char scriptEndLocation;  // 0x05 [O] (0x11 there; On Tour order hidden, scriptEndLocation, hideIllegalBoltOn,
                                    //      isRidingBoard after character); [U] cRider_GetNISEndPosition switches on
                                    //      it (lb 5: cases 0-4 pick the end point), as On Tour's GetNISEndPosition
                                    //      does (lb 17(pHeaderInfo))
    signed char hideIllegalBoltOn;  // 0x06 [O] (0x12 there); [U] cRider_OnStartObjectUse / OnEndObjectUse hide /
                                    //      show the model parts of the rider's flag-0x40 items when it is set
    signed char isRidingBoard;      // 0x07 [O] (0x13 there); [U] cRider_OnStartObjectUse: mNISShowBoardVisualFX =
                                    //      isRidingBoard != 0
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
CHECK_OFFSET(tSSXHeaderActor, hidden, 0x04);
CHECK_OFFSET(tSSXHeaderActor, scriptEndLocation, 0x05);
CHECK_OFFSET(tSSXHeaderActor, hideIllegalBoltOn, 0x06);
CHECK_OFFSET(tSSXHeaderActor, isRidingBoard, 0x07);
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

// A game-state entity's frame (On Tour struct tSSXFrameGameStateManager, 0x8; [O] GetSSXGameStateManagerFrame(
// cScriptPlayerEntity*, tSSXFrameGameStateManager*)). [R] retail GetSSXGameStateManagerFrame stores these 2 words from
// channel 0 (its value word, then its custom word).
struct tGameState;  // On Tour: a 0x1C list node (an inseq block, then pIgnore and pNext); not defined here

struct tSSXFrameGameStateManager {
    int numGameState;               // 0x00 [O]
    tGameState* pGameStateList;     // 0x04 [O]
};

CHECK_OFFSET(tSSXFrameGameStateManager, numGameState, 0x00);
CHECK_OFFSET(tSSXFrameGameStateManager, pGameStateList, 0x04);
CHECK_SIZE(tSSXFrameGameStateManager, 0x8);

// The custom data of a camera-manager cut (On Tour struct tTagCustomActiveCamera, 0x8). [U]
// cScriptObjCameraManager_DoCameraTransition hands the four values to the transition object; OnInSeqKeyEvent and
// OnInSeqUpdate switch on transitionIn.
struct tTagCustomActiveCamera {
    signed char transitionIn;       // 0x0 [O]; [U] 0 blend, 1-6 cScriptObjTransition_SetTransition1, 7 and up
                                    //     cScriptObjTransition_SetTransition, 8 cut
    signed char transitionColour;   // 0x1 [O]; [U] DoCameraTransition passes it to SetTransition1
    short transitionHoldFrames;     // 0x2 [O]; [U] DoCameraTransition: SetTransition1's hold time (as float)
    short transitionOutFrames;      // 0x4 [O]; [U] DoCameraTransition: SetTransition1's out time (as float)
    signed char padding[2];         // 0x6 [O]
};

CHECK_OFFSET(tTagCustomActiveCamera, transitionIn, 0x0);
CHECK_OFFSET(tTagCustomActiveCamera, transitionColour, 0x1);
CHECK_OFFSET(tTagCustomActiveCamera, transitionHoldFrames, 0x2);
CHECK_OFFSET(tTagCustomActiveCamera, transitionOutFrames, 0x4);
CHECK_SIZE(tTagCustomActiveCamera, 0x8);

// An active camera of a camera-manager frame, a list node (On Tour struct tActiveCamera, 0x1C). [U] the camera
// manager's OnInSeqKeyEvent / OnInSeqUpdate / DoCameraTransition read the list head; OnInSeqKeyEvent walks pNext.
struct tActiveCamera {
    tInSeqBlock data;                // 0x00 [O]; [U] value: the camera to set up (SetupCameraObject), track: its
                                     //      track (0 / 1), scriptStartFrame and overlapInFrames time the transition
    tTagCustomActiveCamera* pCustom; // 0x14 [O]
    tActiveCamera* pNext;            // 0x18 [O]
};

CHECK_OFFSET(tActiveCamera, data, 0x00);
CHECK_OFFSET(tActiveCamera, pCustom, 0x14);
CHECK_OFFSET(tActiveCamera, pNext, 0x18);
CHECK_SIZE(tActiveCamera, 0x1C);

// A camera-manager entity's frame (On Tour struct tSSXFrameCameraManager, 0x8; [O] GetSSXCameraManagerFrame(
// cScriptPlayerEntity*, tSSXFrameCameraManager*)). [R] retail GetSSXCameraManagerFrame stores these 2 words from
// channel 0 (its value word, then its custom word).
struct tSSXFrameCameraManager {
    int numActiveCamera;               // 0x00 [O]
    tActiveCamera* pActiveCameraList;  // 0x04 [O]
};

CHECK_OFFSET(tSSXFrameCameraManager, numActiveCamera, 0x00);
CHECK_OFFSET(tSSXFrameCameraManager, pActiveCameraList, 0x04);
CHECK_SIZE(tSSXFrameCameraManager, 0x8);

// A camera-manager entity's SSX header (On Tour struct tSSXHeaderCameraManager, 0x4; [O]
// GetSSXCameraManagerHeader returns it). [U] the camera manager's OnInSeqKeyEvent / OnInSeqUpdate test musicSync.
struct tSSXHeaderCameraManager {
    signed char musicSync;          // 0x0 [O]; [U] nonzero: cuts wait for the music beat
    signed char padding[3];         // 0x1 [O]
};

CHECK_OFFSET(tSSXHeaderCameraManager, musicSync, 0x0);
CHECK_SIZE(tSSXHeaderCameraManager, 0x4);

// tSSXHeaderCameraAlgorithmic (0x20): the SSX header of an algorithmic camera entity's script data
// (GetSSXCameraAlgorithmicHeader; On Tour tSSXHeaderCameraAlgorithmic, same offsets). [U]
// cScriptObjCamera_CalculateBaseTransform_SE reads the three rotations (degrees) at On Tour's offsets;
// cScriptEntityCamera_OnInSeqLock reads the sub type at +0x1C (src/scripter/bxscriptengine.cpp).
struct tSSXHeaderCameraAlgorithmic {
    float nearPlane;                // 0x00 [O]; [U] cScriptRelativeAlgorithm_snap / _update: Near
    float farPlane;                 // 0x04 [O]; [U] ditto: Far
    float baseRotZ;                 // 0x08 [O]; [U] -> rz
    float baseRotY;                 // 0x0C [O]; [U] -> ry
    float baseRotX;                 // 0x10 [O]; [U] -> rx
    float stickiness;               // 0x14 [O]; [U] cScriptRelativeAlgorithm_update: blend weight of the ideal place
    float maxSpeed;                 // 0x18 [O]; [U] cScriptRelativeAlgorithm_update: the per-second step cap
    signed char subType;            // 0x1C [O]; [U] OnInSeqLock: mSubType = this byte (entity type 3)
    signed char subject;            // 0x1D [O]; [U] snap / update: cSSXScriptEngine_GetSubjectRider's subject
    signed char areAnglesWorldRelative; // 0x1E [O]; [U] snap / update: 0 = angles follow the rider's front row
    signed char unk_01F;            // 0x1F [O] padding
};

CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, nearPlane, 0x00);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, farPlane, 0x04);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotZ, 0x08);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotY, 0x0C);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, baseRotX, 0x10);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, stickiness, 0x14);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, maxSpeed, 0x18);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, subType, 0x1C);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, subject, 0x1D);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, areAnglesWorldRelative, 0x1E);
CHECK_OFFSET(tSSXHeaderCameraAlgorithmic, unk_01F, 0x1F);
CHECK_SIZE(tSSXHeaderCameraAlgorithmic, 0x20);

// The SSX headers of manual, target and subject camera entities' script data (GetSSXCameraManualHeader,
// GetSSXCameraTargetHeader, GetSSXCameraSubjectHeader). [O] On Tour's tSSXHeaderCameraManual / Target / Subject
// (0x24 each, the same first 0x22 bytes), same offsets as retail's reads.
struct tSSXHeaderCameraManual {
    float nearPlane;                // 0x00 [O]; [U] cScriptManualAlgorithm_update: Near (manual)
    float farPlane;                 // 0x04 [O]; [U] ditto: Far
    float basePosX;                 // 0x08 [O]; [U] cScriptObjCamera_CalculateBaseTransform_SE: base translation
    float basePosY;                 // 0x0C [O]
    float basePosZ;                 // 0x10 [O]
    float baseRotZ;                 // 0x14 [O]; [U] ditto: rz (degrees)
    float baseRotY;                 // 0x18 [O]; [U] ditto: ry
    float baseRotX;                 // 0x1C [O]; [U] ditto: rx
    signed char anchorPos;          // 0x20 [O]; [U] ditto: the anchor (cSSXScriptEngine_GetAnchorOrientation)
    signed char subType;            // 0x21 [O]; [U] cScriptEntityCamera_OnInSeqLock: mSubType = this byte
    signed char padding[2];         // 0x22 [O]
};

CHECK_OFFSET(tSSXHeaderCameraManual, anchorPos, 0x20);
CHECK_OFFSET(tSSXHeaderCameraManual, subType, 0x21);
CHECK_SIZE(tSSXHeaderCameraManual, 0x24);

struct tSSXHeaderCameraTarget {
    float nearPlane;                // 0x00 [O]; [U] cScriptManualAlgorithm_update: Near (manual)
    float farPlane;                 // 0x04 [O]; [U] ditto: Far
    float basePosX;                 // 0x08 [O]; [U] cScriptObjCamera_CalculateBaseTransform_SE: base translation
    float basePosY;                 // 0x0C [O]
    float basePosZ;                 // 0x10 [O]
    float baseRotZ;                 // 0x14 [O]; [U] ditto: rz (degrees)
    float baseRotY;                 // 0x18 [O]; [U] ditto: ry
    float baseRotX;                 // 0x1C [O]; [U] ditto: rx
    signed char anchorPos;          // 0x20 [O]; [U] ditto: the anchor (cSSXScriptEngine_GetAnchorOrientation)
    signed char subType;            // 0x21 [O]; [U] cScriptEntityCamera_OnInSeqLock: mSubType = this byte
    signed char areAnglesWorldRelative; // 0x22 [O]
    signed char padding;            // 0x23 [O]
};

CHECK_OFFSET(tSSXHeaderCameraTarget, anchorPos, 0x20);
CHECK_OFFSET(tSSXHeaderCameraTarget, subType, 0x21);
CHECK_SIZE(tSSXHeaderCameraTarget, 0x24);

struct tSSXHeaderCameraSubject {
    float nearPlane;                // 0x00 [O]; [U] cScriptManualAlgorithm_update: Near (manual)
    float farPlane;                 // 0x04 [O]; [U] ditto: Far
    float basePosX;                 // 0x08 [O]; [U] cScriptObjCamera_CalculateBaseTransform_SE: base translation
    float basePosY;                 // 0x0C [O]
    float basePosZ;                 // 0x10 [O]
    float baseRotZ;                 // 0x14 [O]; [U] ditto: rz (degrees)
    float baseRotY;                 // 0x18 [O]; [U] ditto: ry
    float baseRotX;                 // 0x1C [O]; [U] ditto: rx
    signed char anchorPos;          // 0x20 [O]; [U] ditto: the anchor (cSSXScriptEngine_GetAnchorOrientation)
    signed char subType;            // 0x21 [O]; [U] cScriptEntityCamera_OnInSeqLock: mSubType = this byte
    signed char subject;            // 0x22 [O]
    signed char padding;            // 0x23 [O]
};

CHECK_OFFSET(tSSXHeaderCameraSubject, anchorPos, 0x20);
CHECK_OFFSET(tSSXHeaderCameraSubject, subType, 0x21);
CHECK_SIZE(tSSXHeaderCameraSubject, 0x24);

// An SSX script entity's type (tInSeqEntityHeader::type, cScriptPlayerEntity_GetType). Names ours (On Tour's DWARF has
// no such enum). [U] the camera code picks the header getter by it: 0 GetSSXCameraManualHeader, 1 ...TargetHeader,
// 2 ...SubjectHeader, 3 ...AlgorithmicHeader (cScriptEntityCamera_OnInSeqLock, cScriptCameraController_addCamera,
// cScriptObjCamera_CalculateBaseTransform_SE); cScriptObjCameraManager_IsMatchingEntity takes only type 4; the role
// filter reads type 5 with GetSSXActorHeader1; cScriptObjGS_IsMatchingEntity takes 6 and
// cScriptObjEvent_IsMatchingEntity 7. The header getters' retail order (camera manual, target, subject,
// algorithmic, manager, actor, then the game state manager and game event frames) agrees.
enum eSSXEntityType {
    keSSXEntityType_CameraManual = 0,
    keSSXEntityType_CameraTarget = 1,
    keSSXEntityType_CameraSubject = 2,
    keSSXEntityType_CameraAlgorithmic = 3,
    keSSXEntityType_CameraManager = 4,
    keSSXEntityType_Actor = 5,
    keSSXEntityType_GameStateManager = 6,
    keSSXEntityType_GameEvent = 7
};

// A script subject: which cast member or rider a script refers to (cSSXScriptEngine::GetSubjectRider's slot, the index
// of BuildCharacterBitRoleArray's flags; names ours). [U] both map 0-2 to the request's main and supporting characters
// (tSSXScriptSetupInfo's AI indexes), 3-4 to the human players, 5-9 to the computer riders and then the actors, 10-14
// to the actors and then the computer riders, and 15-20 to the AI's riders by index.
enum eSSXScriptSubject {
    keSSXSubject_MainChar = 0,
    keSSXSubject_SupportChar0 = 1,
    keSSXSubject_SupportChar1 = 2,
    keSSXSubject_Player0 = 3,
    keSSXSubject_Player1 = 4,
    keSSXSubject_Computer0 = 5,
    keSSXSubject_Computer1 = 6,
    keSSXSubject_Computer2 = 7,
    keSSXSubject_Computer3 = 8,
    keSSXSubject_Computer4 = 9,
    keSSXSubject_Actor0 = 10,
    keSSXSubject_Actor1 = 11,
    keSSXSubject_Actor2 = 12,
    keSSXSubject_Actor3 = 13,
    keSSXSubject_Actor4 = 14,
    keSSXSubject_Rider0 = 15,
    keSSXSubject_Rider1 = 16,
    keSSXSubject_Rider2 = 17,
    keSSXSubject_Rider3 = 18,
    keSSXSubject_Rider4 = 19,
    keSSXSubject_Rider5 = 20,
    keSSXSubject_Num = 21
};

// The kind of a script bank's attached data (OnAttachData's kind, OnDetachData's type; names ours). [U] OnAttachData
// stores Script for "scr" data, Animation for "anm" and Sound for "snd"; OnDetachData detaches each accordingly.
enum eScriptBankDataKind {
    keScriptBankData_Script = 0,
    keScriptBankData_Animation = 1,
    keScriptBankData_Sound = 2
};

// How a race start is reached (AppendStartSequence's state). [O] On Tour's enum eNISStartSituation, names and values;
// [U] AppendStartSequence queues the fly-in for InitEvent, the start frame for RestartEvent and NextHeat, and the
// start gate and frame for LastHeat.
enum eNISStartSituation {
    keNISStartSituation_InitEvent = 0,
    keNISStartSituation_RestartEvent = 1,
    keNISStartSituation_NextHeat = 2,
    keNISStartSituation_LastHeat = 3
};

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

struct cRider;
struct t4Vector;

struct cSSXScriptEngine {
    // FillOrientationFromObjectOrientationInfo's section for "the current location's" (one past mpAnchorTableDB's 50
    // sections; name ours). [U] it then looks the section up from the new-race location (D_00482660); On Tour calls
    // the parameter overrideSectionID, and GetAnchorOrientation passes this for every object anchor but two.
    enum { kAnchorSection_CurrentLocation = 50 };

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

    // Methods (retail / [O] On Tour names), each bound to its retail symbol by an asm label.
    tSSXScriptSetupInfo* GetScriptSetupInfo(cScriptPlayerMain* pPlayer)
        __asm__("cSSXScriptEngine_GetScriptSetupInfo__FPv");
    void Load() __asm__("cSSXScriptEngine_Load");
    void Purge() __asm__("cSSXScriptEngine_Purge");
    void Init() __asm__("cSSXScriptEngine_Init__FPv");
    void DeInit() __asm__("cSSXScriptEngine_DeInit__FPv");
    void Update() __asm__("cSSXScriptEngine_Update");
    void UpdateLoad() __asm__("cSSXScriptEngine_UpdateLoad");
    void* Render() __asm__("cSSXScriptEngine_Render__FPv");
    void Restart() __asm__("cSSXScriptEngine_Restart");
    void AttachAnchorDataTable(int key, unsigned int* val) __asm__("cSSXScriptEngine_AttachAnchorDataTable");
    void DetachAnchorDataTable(int key) __asm__("cSSXScriptEngine_DetachAnchorDataTable");
    void ForceEndScriptsAll() __asm__("cSSXScriptEngine_ForceEndScriptsAll");
    int ForcePlayUpdate(int key) __asm__("cSSXScriptEngine_ForcePlayUpdate");
    void AsyncLoadTrack() __asm__("cSSXScriptEngine_AsyncLoadTrack");
    void PurgeTrack() __asm__("cSSXScriptEngine_PurgeTrack");
    int IsLoadTrackComplete() __asm__("cSSXScriptEngine_IsLoadTrackComplete");
    int IsPauseMenuAllowed() __asm__("cSSXScriptEngine_IsPauseMenuAllowed");
    void DisallowNISListSkip(int val) __asm__("cSSXScriptEngine_DisallowNISListSkip__FPvi");
    void SetNISListViewIndex(int viewIndex) __asm__("cSSXScriptEngine_SetNISListViewIndex");
    void AddToNISListByVideoCategory(int i, int category, int scriptControlBits)
        __asm__("cSSXScriptEngine_AddToNISListByVideoCategory");
    void AddToNISListByScriptCategory(int i, int scriptCategory, int scriptControlBits, int selectScriptNow,
                                      int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_AddToNISListByScriptCategory");
    void AddToNISListByScriptIndex(int i, int scriptIndex, int scriptControlBits, int mainCharAIIndex,
                                   int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_AddToNISListByScriptIndex");
    void AddToNISListByScriptHashID(int nisList, int key, int scriptControlBits, int mainCharAIIndex,
                                    int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_AddToNISListByScriptHashID");
    void ReadyNISList(int i) __asm__("cSSXScriptEngine_ReadyNISList");
    int PlayNISList(int i, int startWithTransitionIn, int forceUpdate) __asm__("cSSXScriptEngine_PlayNISList");
    int TransferNISList(int destNISList, int sourceNISList) __asm__("cSSXScriptEngine_TransferNISList");
    void PauseNISList(int i, int isPaused) __asm__("cSSXScriptEngine_PauseNISList");
    void SkipNISListScripts(int i, int numScriptsToSkip, int doTransition)
        __asm__("cSSXScriptEngine_SkipNISListScripts");
    void ResetNISList(int i) __asm__("cSSXScriptEngine_ResetNISList");
    void ResetNISListAll() __asm__("cSSXScriptEngine_ResetNISListAll");
    void GetNISListScriptCategory(int i, int listIndex) __asm__("cSSXScriptEngine_GetNISListScriptCategory");
    void GetNISListVideoCategory(int nisList, int listIndex) __asm__("cSSXScriptEngine_GetNISListVideoCategory");
    void GetNISListScriptIndex(int nisList, int listIndex) __asm__("cSSXScriptEngine_GetNISListScriptIndex");
    void GetNISListState(int i) __asm__("cSSXScriptEngine_GetNISListState");
    void GetNISListPlayState(int i) __asm__("cSSXScriptEngine_GetNISListPlayState");
    void GetNISListLength(int i) __asm__("cSSXScriptEngine_GetNISListLength");
    void IsNISListNextScriptReady(int nisList) __asm__("cSSXScriptEngine_IsNISListNextScriptReady");
    int IsActiveNISInTransition() __asm__("cSSXScriptEngine_IsActiveNISInTransition");
    int IsActiveNISSkippable() __asm__("cSSXScriptEngine_IsActiveNISSkippable");
    int IsPlayingNISListPlayer(int pScriptPlayerMain) __asm__("cSSXScriptEngine_IsPlayingNISListPlayer");
    void AddToIdleListByScriptCategory(int i, int scriptCategory, int flag, int mainCharAIIndex,
                                       int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_AddToIdleListByScriptCategory");
    void ReadyIdleList(int i, int flag) __asm__("cSSXScriptEngine_ReadyIdleList");
    void PlayIdleList(int i) __asm__("cSSXScriptEngine_PlayIdleList");
    void ResetIdleList(int i) __asm__("cSSXScriptEngine_ResetIdleList");
    void ResetIdleListAll() __asm__("cSSXScriptEngine_ResetIdleListAll");
    void GetIdleListState(int i) __asm__("cSSXScriptEngine_GetIdleListState");
    int RequestScriptByCategory(int category, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_RequestScriptByCategory");
    int RequestScriptByIndex(int key, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_RequestScriptByIndex");
    int RequestScriptByHashID(int id, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
        __asm__("cSSXScriptEngine_RequestScriptByHashID");
    void PlayRequestScript(int val, int key, int autoRecycle, int loop) __asm__("cSSXScriptEngine_PlayRequestScript");
    int SelectScriptFromCategory(int category) __asm__("cSSXScriptEngine_SelectScriptFromCategory");
    int OnDataUnavailable(int key) __asm__("cSSXScriptEngine_OnDataUnavailable");
    void OnPendingDataCancelled(int scriptDataIndex) __asm__("cSSXScriptEngine_OnPendingDataCancelled");
    void OnScriptDataReleased(int scriptDataIndex) __asm__("cSSXScriptEngine_OnScriptDataReleased");
    void OnAttachData(const char* ext, void* data, int dataSize, int* kind, int* bank)
        __asm__("cSSXScriptEngine_OnAttachData");
    void OnDetachData(void* obj, int dataSize, int type, int id) __asm__("cSSXScriptEngine_OnDetachData");
    void OnStartScript(cScriptPlayerMain* script, int idx, int* players) __asm__("cSSXScriptEngine_OnStartScript");
    void OnReleaseRequest(int requestHandle, void* pUserData) __asm__("cSSXScriptEngine_OnReleaseRequest");
    int AddObject(cScriptObject* obj) __asm__("cSSXScriptEngine_AddObject");
    void RemoveObject(int idx) __asm__("cSSXScriptEngine_RemoveObject");
    void IsObjInUseByPlayer(int i, int val) __asm__("cSSXScriptEngine_IsObjInUseByPlayer");
    void* GetAnimationBankInfo() __asm__("cSSXScriptEngine_GetAnimationBankInfo__FPv");
    void* GetSoundBankInfo() __asm__("cSSXScriptEngine_GetSoundBankInfo__FPv");
    void* GetSubjectRider(int slot, int pPlayer) __asm__("cSSXScriptEngine_GetSubjectRider");
    void* MakeTransitionObject(int type, int mode) __asm__("cSSXScriptEngine_MakeTransitionObject");
    void OnInSeqPlay(cScriptPlayerMain* pPlayer) __asm__("cSSXScriptEngine_OnInSeqPlay");
    void OnInSeqUpdate(cScriptPlayerMain* pPlayer, float x, float y) __asm__("cSSXScriptEngine_OnInSeqUpdate");
    void OnInSeqPlayStopping(cScriptPlayerMain* pPlayer) __asm__("cSSXScriptEngine_OnInSeqPlayStopping");
    void OnInSeqStop(cScriptPlayerMain* pPlayer) __asm__("cSSXScriptEngine_OnInSeqStop");
    void PlayTransportSequence(int online, int intro) __asm__("cSSXScriptEngine_PlayTransportSequence");
    bool EndTransportSequence() __asm__("cSSXScriptEngine_EndTransportSequence");
    bool IsInTransportSequence() __asm__("cSSXScriptEngine_IsInTransportSequence");
    void AppendStartSequence(int state) __asm__("cSSXScriptEngine_AppendStartSequence");
    void AppendEndEventSequence() __asm__("cSSXScriptEngine_AppendEndEventSequence");
    int* GetScriptFromCategory(int cat) __asm__("cSSXScriptEngine_GetScriptFromCategory");
    void SyncLoadScriptBank(int i, const char* name, int flags, int key, int* buf)
        __asm__("cSSXScriptEngine_SyncLoadScriptBank");
    void AsyncLoadScriptBank(int i, const char* name, int flags, int key, int pMem, int memSize)
        __asm__("cSSXScriptEngine_AsyncLoadScriptBank");
    void PurgeScriptBank(int i) __asm__("cSSXScriptEngine_PurgeScriptBank");
    void CancelAsyncLoadScriptBank(int i, int dataIdentifier) __asm__("cSSXScriptEngine_CancelAsyncLoadScriptBank");
    int GetNextAvailableAsyncBankID() __asm__("cSSXScriptEngine_GetNextAvailableAsyncBankID");
    int GetBankNumberByDataIdentifier(int key, int start) __asm__("cSSXScriptEngine_GetBankNumberByDataIdentifier");
    void ProcessAsyncLoadScriptBankComplete(void* file) __asm__("cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete");
    void ProcessAsyncLoadTrackTableComplete(void* file) __asm__("cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete");
    void FillOrientationFromStartPointInfo(cRider* obj, t4Vector* pos, int* roll, float* pitch, float* yaw)
        __asm__("cSSXScriptEngine_FillOrientationFromStartPointInfo");
    void FillOrientationFromRiderOrientationInfo(cRider* obj, t4Vector* pos, int* roll, float* pitch, float* yaw)
        __asm__("cSSXScriptEngine_FillOrientationFromRiderOrientationInfo");
    void* AllocScriptSetupInfo() __asm__("cSSXScriptEngine_AllocScriptSetupInfo__FPv");
    void ReleaseScriptSetupInfo(void* vsetupInfo) __asm__("cSSXScriptEngine_ReleaseScriptSetupInfo__FPvT0");
    bool IsBankLoading(int i) __asm__("cSSXScriptEngine_IsBankLoading");
    void ClearNISPlayListState() __asm__("cSSXScriptEngine_ClearNISPlayListState");
    bool IsNISActive() __asm__("cSSXScriptEngine_IsNISActive");
    int GetActiveNIS() __asm__("cSSXScriptEngine_GetActiveNIS");
    void DeInitOnce() __asm__("cSSXScriptEngine_DeInitOnce");
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
