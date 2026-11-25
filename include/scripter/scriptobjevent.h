#ifndef SCRIPTER_SCRIPTOBJEVENT_H
#define SCRIPTER_SCRIPTOBJEVENT_H

// cScriptObjEvent: the NIS script object (object type 5) that plays a cut-scene's game events: on each key event of
// an entity of type 7 (IsMatchingEntity) it runs the frame's world-script functions (and remembers each one's abort
// script, run by OnEndObjectUse), switches the render-state flag, sends music and speech events, and starts / stops up
// to 4 script sounds from the script's sound bank. One instance: cSSXScriptEngine::mObjectEvent (+0x92C,
// scripter/ssxscriptengine.h), built by the engine ctor with cScriptObjEvent_cScriptObjEvent; Update runs every frame
// from cSSXScriptEngine_Update.
// EA's header is d:/ssx/code/source/scripter/scriptobjevent.h (July 2003 preview assert strings; scriptobjevent.cpp
// too). Retail methods: scripter/ssxscriptengine (0x002805B8 ctor .. 0x00281000 ScriptSoundIDToGameSoundInfo) and
// scripter/all_in_one_scripter (0x00284950 dtor, 0x002849B8 GetNumScriptEntities, 0x002849C0 GetScriptEntity).
//
// Size 0xFC: [R] cSSXScriptEngine's ctor builds it at +0x92C and its next member (mAnimationBankManager) is at +0xA28
// = 0x92C + 0xFC; the ctor / OnStartObjectUse write up to 0xF8 (mpSoundBankInfo). On Tour: 0xF8 (its two bools are
// bytes at 0xF0/0xF1; retail stores words, so mCheckForStopOnSpeechEnd and mpSoundBankInfo are On Tour + 4).
// Bases, in retail order (the ctor: cScriptEntity_cScriptEntity__FPv(self), cScriptObject_cScriptObject__FPvi(self + 0xC, 5)),
// as in cScriptObjRider (scripter/scriptobjrider.h):
//   cScriptEntity (scripter/scriptentity.h, 0xC) at 0x00: vptr 0x00 = cScriptObjEvent_vtbl (0x00482260, 5 slots:
//                 1/2 cScriptEntity_OnInSeqLock / OnInSeqRelease, 3 cScriptObjEvent_OnInSeqKeyEvent,
//                 4 cScriptEntity_OnInSeqUpdate, 5 cScriptObjEvent_IsMatchingEntity);
//   cScriptObject (scripter/scriptobj.h, 0x10) at 0x0C: vptr 0x18 = D_00482298 (_vt$15cScriptObjEvent$13cScriptObject:
//                 1 dtor, 2/3 cScriptObject_Pause/ResumeObjScript, 4 OnStartObjectUse, 5 OnEndObjectUse,
//                 6 GetNumScriptEntities, 7 GetScriptEntity; slots 1 and 4-7 carry delta -0xC, so they get the whole object).
//
// A FLAT layout, like main/game.h: every field at its retail offset, bases written out inline. Evidence tags:
//   [R] retail ctor cScriptObjEvent_cScriptObjEvent and dtor cScriptObjEvent__cScriptObjEvent;
//   [P] tools/layout_xlate.py pairs the preview's dtor with retail's (0x0 and 0x18 unchanged); no inline accessors;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptObjEvent 0xF8: mAbortWorldScripts 0x1C, mSounds 0xBC, mNumAbortWorldScripts 0xEC,
//       mHasTriggeredSpeechEvents 0xF0, mCheckForStopOnSpeechEnd 0xF1, mpSoundBankInfo 0xF4; tScriptObjEventWSAbort 0x8,
//       tScriptObjEventSound 0xC; cScriptEntity; cScriptObject).

#include "layout.h"
#include "inseq/scriptplayerchannel.h" /* tInSeqObject, tInSeqBlock */

struct cScriptPlayerEntity;
struct tScriptSoundBankInfo;
struct tTagCustomWorldScriptEvent;   // [O] (not laid out): its first word is the abort script's hash
struct tTagCustomSpeechEvent;        // [O] (not laid out)
struct tTagCustomSound;              // [O] (not laid out): its first byte is read signed

// A world-script function to run when the script ends (On Tour tScriptObjEventWSAbort, 8 bytes).
struct tScriptObjEventWSAbort {
    unsigned int hashID;           // 0x0 [O]; [U] OnInSeqKeyEvent records the event's abort hash (unless the
                                   //     "no script" hash D_004A525C); OnEndObjectUse passes it to cWScriptMan_runFunc
    int worldSectionID;            // 0x4 [O]; [U] the world section it ran in (OnInSeqKeyEvent), cWScriptMan_runFunc's
                                   //     section argument (OnEndObjectUse)
};

CHECK_OFFSET(tScriptObjEventWSAbort, hashID, 0x0);
CHECK_OFFSET(tScriptObjEventWSAbort, worldSectionID, 0x4);
CHECK_SIZE(tScriptObjEventWSAbort, 0x8);

// One script sound slot (On Tour tScriptObjEventSound, 0xC; its bool is a word in retail).
struct tScriptObjEventSound {
    int soundHandle;               // 0x0 [O]; [R] ctor -1; [U] OnInSeqKeyEvent stores the cBankSys_SFX_End handle;
                                   //     Update / OnEndObjectUse stop it (cBankMonitor_BANKMONITOR_StopHandle) when >= 0
    int endSound;                  // 0x4 [O] bool (word here); [R] ctor 0; [U] OnInSeqKeyEvent (event bit 2) sets 1 on
                                   //     playing slots and 0 on the frame's; Update stops a slot that has it set
    int soundID;                   // 0x8 [O]; [R] ctor -1; [U] OnInSeqKeyEvent stores the script sound ID and compares
                                   //     it with the frame's sound
};

CHECK_OFFSET(tScriptObjEventSound, soundHandle, 0x0);
CHECK_OFFSET(tScriptObjEventSound, endSound, 0x4);
CHECK_OFFSET(tScriptObjEventSound, soundID, 0x8);
CHECK_SIZE(tScriptObjEventSound, 0xC);

// A frame's world-script event (On Tour struct tWorldScriptEvent, 0x8). [U] OnInSeqKeyEvent (view sSeLink_08E8) runs
// eventID with cWScriptMan_runFunc and records the custom record's first word as the abort hash.
struct tWorldScriptEvent {
    int eventID;                    // 0x0 [O]; [U] the world-script function run
    tTagCustomWorldScriptEvent* pCustom; // 0x4 [O]; [U] *(int*)pCustom = the abort hash (tScriptObjEventWSAbort)
};

CHECK_OFFSET(tWorldScriptEvent, eventID, 0x0);
CHECK_OFFSET(tWorldScriptEvent, pCustom, 0x4);
CHECK_SIZE(tWorldScriptEvent, 0x8);

// A frame's music event (On Tour struct tMusicEvent, 0x8). [U] OnInSeqKeyEvent sends eventID (view sSeCmd_08E8).
struct tMusicEvent {
    int eventID;                    // 0x0 [O]; [U] the music event sent
    void* unk_004;                  // 0x4 [O] void* pIgnore
};

CHECK_OFFSET(tMusicEvent, eventID, 0x0);
CHECK_OFFSET(tMusicEvent, unk_004, 0x4);
CHECK_SIZE(tMusicEvent, 0x8);

// A frame's speech event (On Tour struct tSpeechEvent, 0x8). [U] OnInSeqKeyEvent sends eventID (view sSeCmd_08E8).
struct tSpeechEvent {
    int eventID;                    // 0x0 [O]; [U] the speech event sent
    tTagCustomSpeechEvent* pCustom; // 0x4 [O]; [U] OnInSeqKeyEvent reads the subject
};

CHECK_OFFSET(tSpeechEvent, eventID, 0x0);
CHECK_OFFSET(tSpeechEvent, pCustom, 0x4);
CHECK_SIZE(tSpeechEvent, 0x8);

// A frame's script sound, a list node (On Tour struct tSound, 0x24). [U] OnInSeqKeyEvent walks pSoundList (view
// sSfxEnt_08E8: data.objectID 0x0, data.scriptStartFrame 0x4, data.trackIndex 0xC, the custom record's first byte, pNext).
struct tSound {
    tInSeqObject data;              // 0x00 [O]; [U] objectID, scriptStartFrame, trackIndex
    tTagCustomSound* pCustom;       // 0x1C [O]; [U] its first byte (signed)
    tSound* pNext;                  // 0x20 [O]; [U] the list walk
};

CHECK_OFFSET(tSound, data, 0x00);
CHECK_OFFSET(tSound, pCustom, 0x1C);
CHECK_OFFSET(tSound, pNext, 0x20);
CHECK_SIZE(tSound, 0x24);

// A frame's environment state, a list node (On Tour struct tEnviroState, 0x1C). Retail only tests the list head.
struct tEnviroState {
    tInSeqBlock data;               // 0x00 [O]
    void* unk_014;                  // 0x14 [O] void* pIgnore
    tEnviroState* unk_018;          // 0x18 [O] pNext
};

CHECK_OFFSET(tEnviroState, data, 0x00);
CHECK_OFFSET(tEnviroState, unk_014, 0x14);
CHECK_OFFSET(tEnviroState, unk_018, 0x18);
CHECK_SIZE(tEnviroState, 0x1C);

// The game events of an NIS frame (On Tour struct tSSXFrameGameEvent, 0x28). [O] On Tour's
// GetSSXGameEventFrame(cScriptPlayerEntity*, tSSXFrameGameEvent*); [R] retail GetSSXGameEventFrame stores all 10 words;
// [U] OnInSeqKeyEvent (view sSeInfo_08E8) runs the world-script events, sends the music and speech events, walks the
// sound list and switches the render-state flag by pEnviroStateList; OnEndObjectUse (view sSeInfo07B0) tests
// pEnviroStateList.
struct tSSXFrameGameEvent {
    int numWorldScriptEvent;        // 0x00 [O]; [R]; [U] OnInSeqKeyEvent's loop bound
    tWorldScriptEvent* pWorldScriptEventArray; // 0x04 [O]; [R]; [U]
    int numMusicEvent;              // 0x08 [O]; [R]; [U]
    tMusicEvent* pMusicEventArray;  // 0x0C [O]; [R]; [U]
    int numSpeechEvent;             // 0x10 [O]; [R]; [U]
    tSpeechEvent* pSpeechEventArray; // 0x14 [O]; [R]; [U]
    int numSound;                   // 0x18 [O]; [R] GetSSXGameEventFrame stores it
    tSound* pSoundList;             // 0x1C [O]; [R]; [U] the sound list head
    int numEnviroState;             // 0x20 [O]; [R] GetSSXGameEventFrame stores it
    tEnviroState* pEnviroStateList; // 0x24 [O]; [R]; [U] nonzero: cRenderStateMan_m_indoor handling
};

CHECK_OFFSET(tSSXFrameGameEvent, numWorldScriptEvent, 0x00);
CHECK_OFFSET(tSSXFrameGameEvent, pWorldScriptEventArray, 0x04);
CHECK_OFFSET(tSSXFrameGameEvent, numMusicEvent, 0x08);
CHECK_OFFSET(tSSXFrameGameEvent, pMusicEventArray, 0x0C);
CHECK_OFFSET(tSSXFrameGameEvent, numSpeechEvent, 0x10);
CHECK_OFFSET(tSSXFrameGameEvent, pSpeechEventArray, 0x14);
CHECK_OFFSET(tSSXFrameGameEvent, numSound, 0x18);
CHECK_OFFSET(tSSXFrameGameEvent, pSoundList, 0x1C);
CHECK_OFFSET(tSSXFrameGameEvent, numEnviroState, 0x20);
CHECK_OFFSET(tSSXFrameGameEvent, pEnviroStateList, 0x24);
CHECK_SIZE(tSSXFrameGameEvent, 0x28);

struct cScriptObjEvent {
    // --- cScriptEntity (scripter/scriptentity.h, 0xC; ctor cScriptEntity_cScriptEntity__FPv(self))
    void** mVtbl;                                // 0x00 [R] cScriptObjEvent_vtbl (ctor and dtor)
    int mDBID;                                   // 0x04 [O]; [R] cScriptEntity ctor -1
    cScriptPlayerEntity* mpEntity;               // 0x08 [O]; [R] cScriptEntity ctor 0; [U] OnStartObjectUse reads the
                                                 //      script header of its player (mpOwner), OnEndObjectUse passes it to
                                                 //      GetSSXGameEventFrame
    // --- cScriptObject (scripter/scriptobj.h, 0x10; ctor cScriptObject_cScriptObject__FPvi(self + 0xC, 5))
    int mObjType;                                // 0x0C [O]; [R] cScriptObject ctor stores 5 (event)
    int mObjID;                                  // 0x10 [O]; [R] cScriptObject ctor -1
    int mInUse;                                  // 0x14 [O]; [R] cScriptObject ctor 0; [U] cScriptObject_OnStart/
                                                 //      OnEndObjectUse__FPv (called on self + 0xC)
    void** mScriptObjectVtbl;                    // 0x18 [R] D_00482298 (ctor and dtor); [P] the paired dtor stores it at
                                                 //      0x18 in both builds; the dtor runs cScriptObject__cScriptObject on +0xC
    // --- cScriptObjEvent
    tScriptObjEventWSAbort mAbortWorldScripts[20]; // 0x1C [O]; [U] OnInSeqKeyEvent appends (no bound check against 20),
                                                 //      OnEndObjectUse runs and forgets them
    tScriptObjEventSound mSounds[4];             // 0xBC [O]; [R] ctor {-1, 0, -1} x 4; [U] Update, OnStartObjectUse,
                                                 //      OnEndObjectUse, OnInSeqKeyEvent (indexed by the sound's track)
    int mNumAbortWorldScripts;                   // 0xEC [O]; [R] ctor 0; [U] OnInSeqKeyEvent counts up, OnStart/
                                                 //      OnEndObjectUse clear it
    int mHasTriggeredSpeechEvents;               // 0xF0 [O] bool (word here; On Tour byte 0xF0); [R] ctor 0; [U]
                                                 //      OnInSeqKeyEvent sets 1 on a speech event, OnEndObjectUse tests
                                                 //      (func_002A1BD0) and clears it
    int mCheckForStopOnSpeechEnd;                // 0xF4 [O] bool (word here; On Tour byte 0xF1); [R] ctor 0; [U]
                                                 //      OnInSeqKeyEvent sets 1 on speech event 16; Update tests and clears
                                                 //      it (then func_002B0E28 decides cSSXScriptEngine_SkipNISListScripts)
    tScriptSoundBankInfo* mpSoundBankInfo;       // 0xF8 [O] (On Tour 0xF4); [R] ctor 0; [U] OnStartObjectUse binds the
                                                 //      script's bank (cSSXScriptEngine_GetSoundBankInfo__FPv), OnEndObjectUse
                                                 //      clears it, ScriptSoundIDToGameSoundInfo reads its bankNum (+0x4)

    // Methods bound to their retail symbols by asm labels.
    void Update() __asm__("cScriptObjEvent_Update");
    void OnStartObjectUse() __asm__("cScriptObjEvent_OnStartObjectUse");
    void OnEndObjectUse() __asm__("cScriptObjEvent_OnEndObjectUse");
    bool IsMatchingEntity(cScriptPlayerEntity* entity, int type) __asm__("cScriptObjEvent_IsMatchingEntity");
    void ScriptSoundIDToGameSoundInfo(int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
        __asm__("cScriptObjEvent_ScriptSoundIDToGameSoundInfo__FPviT0T0");
};

CHECK_OFFSET(cScriptObjEvent, mVtbl, 0x00);
CHECK_OFFSET(cScriptObjEvent, mDBID, 0x04);
CHECK_OFFSET(cScriptObjEvent, mpEntity, 0x08);
CHECK_OFFSET(cScriptObjEvent, mObjType, 0x0C);
CHECK_OFFSET(cScriptObjEvent, mObjID, 0x10);
CHECK_OFFSET(cScriptObjEvent, mInUse, 0x14);
CHECK_OFFSET(cScriptObjEvent, mScriptObjectVtbl, 0x18);
CHECK_OFFSET(cScriptObjEvent, mAbortWorldScripts, 0x1C);
CHECK_OFFSET(cScriptObjEvent, mSounds, 0xBC);
CHECK_OFFSET(cScriptObjEvent, mNumAbortWorldScripts, 0xEC);
CHECK_OFFSET(cScriptObjEvent, mHasTriggeredSpeechEvents, 0xF0);
CHECK_OFFSET(cScriptObjEvent, mCheckForStopOnSpeechEnd, 0xF4);
CHECK_OFFSET(cScriptObjEvent, mpSoundBankInfo, 0xF8);
CHECK_SIZE(cScriptObjEvent, 0xFC);

#endif /* SCRIPTER_SCRIPTOBJEVENT_H */
