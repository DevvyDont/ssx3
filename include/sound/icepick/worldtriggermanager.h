#ifndef SOUND_ICEPICK_WORLDTRIGGERMANAGER_H
#define SOUND_ICEPICK_WORLDTRIGGERMANAGER_H

// cWorldTriggerManager: plays the world's positional sound triggers (Icepick audio). It owns a pool of 40
// cWorldTriggerInstance (one playing trigger each: object ref, volume number, view port, position, voice), the
// trigger table loaded from data/config/watrig.adl (LoadTriggerInfo: banked / advanced / swap / ... entries), and a
// list of up to 200 objects the riders have hit (ObjectHit / AddHitObject, for the after-collision triggers).
// One global instance, gWorldTriggerManager (WORLDTRIGGERMANAGER_Init / DeInit). EA's source:
// d:/ssx/code/source/sound/icepick/worldtriggermanager.cpp (preview assert path); retail methods 0x002B58D0 ctor ..
// 0x002B6A68 TouchAllUsed in sound/icepick/worldtriggermanager.
//
// Size 0x344: [R] WORLDTRIGGERMANAGER_Init__Fv allocates cMemMan_alloc(0x344, "WorldTriggerManager") and runs
// cWorldTriggerManager_cWorldTriggerManager on it. [O] On Tour's cWorldTriggerManager is 0x340 with the same members
// in the same order; it packs mLastHitObjectResult / mMonitorTriggers as two 1-byte bools at 0x330 / 0x331, retail
// stores each as a word (sw 0x330, sw 0x334), so every later member sits 4 bytes higher. No bases, no vtable.
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] the constructor (cWorldTriggerManager_cWorldTriggerManager) and WORLDTRIGGERMANAGER_Init; [U] how retail
// code uses the field; [O] SSX On Tour's DWARF.

#include "layout.h"
#include "bx/math3d.h"

// A trigger record's type (tTriggerType::m_TriggerType, tTriggerTypePointer::m_TriggerType). [O] On Tour's enum
// eTriggerType, names and values; [U] LoadTriggerInfo steps 6, 14, 12 and 4 words over types 1-4, and TriggerToPatch
// reads one bank and patch for 1, picks a weighted patch for 2 and gives swap triggers (3) their bank.
enum eTriggerType {
    eTRIGGERTYPE_None = 0, eTRIGGERTYPE_Banked = 1, eTRIGGERTYPE_Random = 2, eTRIGGERTYPE_Swap = 3,
    eTRIGGERTYPE_Advanced = 4, eTRIGGERTYPE_Max = 5
};

// TriggerToPatch's check argument. [O] On Tour's enum eTriggerToPatchCheck, names and values; [U] CrowdTrigger asks
// with 1, SwapTrigger with 2, the play path with 0 (only 0 lets an advanced trigger through).
enum eTriggerToPatchCheck {
    eTriggerToPatchCheck_General = 0, eTriggerToPatchCheck_Crowd = 1, eTriggerToPatchCheck_Swap = 2
};

// The sound banks TriggerToPatch hands out that mark a trigger's kind (names ours). [U] TriggerToPatch gives every swap
// trigger bank 3 (cWorldTriggerInstance_Update then plays its swap string, SwapTrigger tests 3); CrowdTrigger tests 5
// for a trigger the crowd system plays.
enum { kSwapTriggerBank = 3, kCrowdTriggerBank = 5 };

// The one advanced trigger retail knows. [O] On Tour's eAudioTrigger member kAUDIO_TRIGGER_PA_Trigger = 77;
// [U] AdvancedTriggerToPatch sends trigger 77 to the PA's place announcement (func_002A34D0).
enum { kAUDIO_TRIGGER_PA_Trigger = 77 };

// The fixed head of every trigger record in the trigger file (views group D: sTrigObj_6A38). [O] On Tour struct
// tTriggerType (0x10): m_TriggerType, m_HitObjectTrigger, m_SyncToAnimation, m_DuckAmbientAudio; the specific trigger
// structs derive from it. [U] tTriggerTypePointer::m_pTriggerTypeStruct points at one; cWorldTriggerManager's
// DuckAmbientAudioTrigger(int trigger) ([O] the paired function) tests m_DuckAmbientAudio == 1 (+0xC).
struct tTriggerType {
    int m_TriggerType;              // 0x0 [O] eTriggerType
    int m_HitObjectTrigger;         // 0x4 [O] int
    int m_SyncToAnimation;          // 0x8 [O] int
    int m_DuckAmbientAudio;         // 0xC [O] int; [U] DuckAmbientAudioTrigger
};

CHECK_OFFSET(tTriggerType, m_TriggerType, 0x0);
CHECK_OFFSET(tTriggerType, m_HitObjectTrigger, 0x4);
CHECK_OFFSET(tTriggerType, m_SyncToAnimation, 0x8);
CHECK_OFFSET(tTriggerType, m_DuckAmbientAudio, 0xC);
CHECK_SIZE(tTriggerType, 0x10);

// cWorldTriggerInstance: one playing world trigger (On Tour class cWorldTriggerInstance, 0x70): which object volume
// and view port it is for, its trigger number, volume and position, and the sound it plays (bank-monitor handle, or a
// crowd-system handle for crowd triggers; -9999 = a trigger with no audio). The manager owns 40 of them
// (m_aWorldTriggerInstances); GetFreeInstance hands one out, Update (cWorldTriggerManager_Update for touched
// instances) starts / refreshes its sound, Stop / Reset release it. No bases, no vtable (the dtor only deletes).
// Retail methods: sound/icepick/worldtriggermanager (0x2B4B70 ctor, 0x2B4BE0 dtor, 0x2B4C08 SameTriggerInstance,
// 0x2B4C38 Update, 0x2B5758 Stop, 0x2B5818 TriggerToPatch, 0x2B5838 CrowdTrigger, 0x2B5878 SwapTriggerToString,
// 0x2B5898 Reset); EA's worldtriggermanager.cpp (file_hints: Update at line 139).
// Size 0x70 [R]: cWorldTriggerManager_cWorldTriggerManager allocates new[](4496 = 40 * 0x70 + 0x10,
// "m_aWorldTriggerInstances") and constructs each 0x70 apart. [O] On Tour is 0x70 too, but it packs m_bTouch /
// m_bCrowdTrigger as two 1-byte bools at 0x34 / 0x35; retail stores each as a word (sw 0x34, sw 0x38), so every
// later member sits 4 bytes higher (m_iViewPort 0x38 -> 0x3C ... m_bLastAnimFrameDecrease 0x68 -> 0x6C). The other
// On Tour bools (m_bInUse, m_bPositional) are words in retail as well. Tags as for cWorldTriggerManager; [R] here is
// the instance ctor cWorldTriggerInstance_cWorldTriggerInstance (and Reset / Stop, which re-initialise the tail).
struct cWorldTriggerInstance {
    int m_bInUse;                       // 0x00 [O] (On Tour bool; retail sw); [R] ctor 0, Reset 0; [U] the manager's
                                        //      GetFreeInstance / GetTriggerInstance / Update / TouchAllUsed test it
    int m_iObjectRef;                   // 0x04 [O]; [R] ctor 0; [U] SameTriggerInstance compares it; Update passes it to
                                        //      cWorldTriggerManager_PlayingTriggerInstance
    int m_iVolNum;                      // 0x08 [O]; [R] ctor 0; [U] SameTriggerInstance compares it
    int m_eAudioTrigger;                // 0x0C [O] the trigger number; [R] ctor 0; [U] Update passes it to CrowdTrigger /
                                        //      TriggerToPatch / SwapTriggerToString / SwapTriggerPlaying
    float m_fVolume;                    // 0x10 [O]; [R] ctor 0 (sw $0); [U] Update lwc1 (scales the applied volume)
    int m_bPositional;                  // 0x14 [O] (On Tour bool; retail word); [R] ctor 0; [U] Update lw
    char unk_18[0x8];                   // 0x18 alignment gap before the 16-byte-aligned t4Vector
    t4Vector m_vPosition;               // 0x20 [O]; [R] ctor x/y/z 0, w 1.0f (sw 0x20/0x24/0x28/0x2C); [U] Update passes
                                        //      &m_vPosition to the 3D sound
    int m_iAudioHandle;                 // 0x30 [O]; [R] ctor -1, Reset / Stop -1; [U] Update stores the voice / crowd
                                        //      handle (-9999 for no audio), Stop stops it with a 0.25 s fade
    // m_iAudioHandle's two markers (names ours). [R] ctor / Reset / Stop store -1, so Update starts the sound only
    // then; [U] Update stores -9999 for a trigger that plays no bank sound (TriggerToPatch's noAudio, or a
    // sync-to-animation trigger off its first frame), and Stop / Update leave such a handle alone.
    enum { kAudioNotStarted = -1, kAudioNone = -9999 };
    int m_bTouch;                       // 0x34 [O] (a 1-byte bool in On Tour; retail sw); [R] ctor 0, Reset 0; [U] the
                                        //      manager's TouchAllUsed / Update (view sWorldTrigger70.unk34)
    int m_bCrowdTrigger;                // 0x38 [O] (a 1-byte bool at 0x35 in On Tour; retail sw); [R] ctor 0; [U] Update
                                        //      stores CrowdTrigger's result; Stop picks the crowd system when set
    int m_iViewPort;                    // 0x3C [O] (On Tour 0x38); [U] SameTriggerInstance compares it; Update passes it to
                                        //      TriggerToPatch (not set by the ctor)
    char m_iSwapString[31];             // 0x40 [O] (On Tour 0x3C); [R] ctor / Reset / Stop store "" (sb of
                                        //      str_empty_j[0]); [U] Update fills it with SwapTriggerToString
    char unk_5F[0x1];                   // 0x5F padding
    int m_iBankReference;               // 0x60 [O] (On Tour 0x5C); [R] ctor / Reset / Stop 0; [U] Update stores the bank
                                        //      TriggerToPatch returns
    int m_iPatchReference;              // 0x64 [O] (On Tour 0x60); [R] ctor / Reset / Stop 0; [U] Update stores the patch
    float m_iLastAnimFrame;             // 0x68 [O] (On Tour 0x64; EA's name, a float); [R] ctor / Reset / Stop -1.0f
    int m_bLastAnimFrameDecrease;       // 0x6C [O] (On Tour bool at 0x68; retail word); [R] ctor / Reset / Stop 0

    // Methods bound to their retail symbols by asm labels.
    bool SameTriggerInstance(int objectRef, int volNum, int viewPort)
        __asm__("cWorldTriggerInstance_SameTriggerInstance");
    void Update() __asm__("cWorldTriggerInstance_Update");
    void Stop() __asm__("cWorldTriggerInstance_Stop");
    int TriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
        __asm__("cWorldTriggerInstance_TriggerToPatch");
    bool CrowdTrigger(int trigger) __asm__("cWorldTriggerInstance_CrowdTrigger");
    int SwapTriggerToString(int idx, char* dst) __asm__("cWorldTriggerInstance_SwapTriggerToString");
    void Reset() __asm__("cWorldTriggerInstance_Reset");
};

CHECK_OFFSET(cWorldTriggerInstance, m_bInUse, 0x00);
CHECK_OFFSET(cWorldTriggerInstance, m_iObjectRef, 0x04);
CHECK_OFFSET(cWorldTriggerInstance, m_iVolNum, 0x08);
CHECK_OFFSET(cWorldTriggerInstance, m_eAudioTrigger, 0x0C);
CHECK_OFFSET(cWorldTriggerInstance, m_fVolume, 0x10);
CHECK_OFFSET(cWorldTriggerInstance, m_bPositional, 0x14);
CHECK_OFFSET(cWorldTriggerInstance, unk_18, 0x18);
CHECK_OFFSET(cWorldTriggerInstance, m_vPosition, 0x20);
CHECK_OFFSET(cWorldTriggerInstance, m_iAudioHandle, 0x30);
CHECK_OFFSET(cWorldTriggerInstance, m_bTouch, 0x34);
CHECK_OFFSET(cWorldTriggerInstance, m_bCrowdTrigger, 0x38);
CHECK_OFFSET(cWorldTriggerInstance, m_iViewPort, 0x3C);
CHECK_OFFSET(cWorldTriggerInstance, m_iSwapString, 0x40);
CHECK_OFFSET(cWorldTriggerInstance, unk_5F, 0x5F);
CHECK_OFFSET(cWorldTriggerInstance, m_iBankReference, 0x60);
CHECK_OFFSET(cWorldTriggerInstance, m_iPatchReference, 0x64);
CHECK_OFFSET(cWorldTriggerInstance, m_iLastAnimFrame, 0x68);
CHECK_OFFSET(cWorldTriggerInstance, m_bLastAnimFrameDecrease, 0x6C);
CHECK_SIZE(cWorldTriggerInstance, 0x70);

// One entry of the trigger table (On Tour struct tTriggerTypePointer, 0x8): the entry's type and its data in the
// loaded watrig.adl file.
struct tTriggerTypePointer {
    int m_TriggerType;                  // 0x0 [O] eTriggerType; [U] LoadTriggerInfo copies the entry's first word (1
                                        //       banked: 6 words, 2: 14, 3 swap: 12, 4: 4); TriggerToPatch switches on
                                        //       it
    tTriggerType* m_pTriggerTypeStruct; // 0x4 [O]; [U] LoadTriggerInfo points it at the entry in the file
};

CHECK_OFFSET(tTriggerTypePointer, m_TriggerType, 0x0);
CHECK_OFFSET(tTriggerTypePointer, m_pTriggerTypeStruct, 0x4);
CHECK_SIZE(tTriggerTypePointer, 0x8);

// The head of the trigger file data/config/watrig.adl as FILE_loadpack loads it (m_pcTriggerInfoFile; names ours).
// [U] cWorldTriggerManager_LoadTriggerInfo (its only reader): rejects the file unless the first byte is 0 (lb), takes
// the trigger count from +0x8 (m_iNumTriggers = count + 2) and walks the variable-length trigger records from +0xC
// (each starts with its type: 1 = 6 words, 2 = 14, 3 = 12, 4 = 4), pointing m_aTriggerInfo[2..] at them.
struct tTriggerInfoFileHeader {
    signed char mFormatFlag;            // 0x0 [U] LoadTriggerInfo returns 0 (no triggers indexed) unless it is 0
    char unk_1[0x7];                    // 0x1 no retail access seen
    int mNumTriggers;                   // 0x8 [U] the record count (m_iNumTriggers - 2)
    int mRecords[1];                    // 0xC [U] the first trigger record (variable length; see above)
};

CHECK_OFFSET(tTriggerInfoFileHeader, mFormatFlag, 0x0);
CHECK_OFFSET(tTriggerInfoFileHeader, unk_1, 0x1);
CHECK_OFFSET(tTriggerInfoFileHeader, mNumTriggers, 0x8);
CHECK_OFFSET(tTriggerInfoFileHeader, mRecords, 0xC);

struct cRider;
struct t3Vector;
struct tInstanceRecord;

struct cWorldTriggerManager {
    cWorldTriggerInstance* m_aWorldTriggerInstances; // 0x000 [O]; [R] new[40] 0x70-byte instances
                                                     //       ("m_aWorldTriggerInstances", count word at -0x10); [U] the
                                                     //       destructor destroys and frees them; GetFreeInstance /
                                                     //       GetTriggerInstance / Update / StopAllTriggers /
                                                     //       TouchAllUsed walk the 40
    int mHitObjects[200];               // 0x004 [O]; [U] cWorldTriggerManager_ObjectHit searches [0, mNumHitObjects),
                                        //       AddHitObject appends (max 200)
    int mNumHitObjects;                 // 0x324 [O]; [R] 0; [U] ObjectHit / AddHitObject; ResetStates clears it
    int mLastHitObject;                 // 0x328 [O]; [R] -1; [U] HandleCollision compares the colliding object with it
                                        //       and stores it; ResetStates sets -1
    int mLastHitObjectChecked;          // 0x32C [O]; [R] -1; [U] ObjectHit's one-entry cache key (AddHitObject resets
                                        //       it to -1); ResetStates sets -1
    int mLastHitObjectResult;           // 0x330 [O] (a 1-byte bool in On Tour; retail sw); [R] 0; [U] ObjectHit's
                                        //       cached answer; ResetStates clears it
    int mMonitorTriggers;               // 0x334 [O] (a 1-byte bool at 0x331 in On Tour; retail sw); [R] 1; [U]
                                        //       MonitorTriggers stores its value argument (unless a delayed enable goes
                                        //       to cSSXAudio_DelayMonitorTriggers); HandleTrigger only plays while it
                                        //       is set
    char* m_pcTriggerInfoFile;          // 0x338 [O]; [R] 0, then LoadTriggerInfo:
                                        //       FILE_loadpack("data/config/watrig.adl"); [U] UnloadTriggerInfo
                                        //       MEM_free's it
    tTriggerTypePointer* m_aTriggerInfo; // 0x33C [O]; [U] LoadTriggerInfo: new (m_iNumTriggers * 8,
                                        //       "m_aTriggerInfoInstances"), entries 2.. filled; TriggerToPatch /
                                        //       SwapTriggerToString / AfterCollisionTrigger / SyncToAnimationTrigger /
                                        //       DuckAmbientAudioTrigger / HandleCollision index it by trigger number
    int m_iNumTriggers;                 // 0x340 [O]; [U] LoadTriggerInfo: the file's count (+0x8) + 2; TriggerToPatch /
                                        //       SwapTriggerToString bound the trigger number by it

    // Methods bound to their retail symbols by asm labels.
    void UnloadTriggerInfo() __asm__("cWorldTriggerManager_UnloadTriggerInfo");
    cWorldTriggerInstance* GetFreeInstance() __asm__("cWorldTriggerManager_GetFreeInstance");
    cWorldTriggerInstance* GetTriggerInstance(int objectRef, int volNum, int viewPort)
        __asm__("cWorldTriggerManager_GetTriggerInstance");
    void HandleTrigger(int objectRef, int volNum, int viewPort, int audioTrigger, int positional, float volume,
                       const t3Vector& pos)
        __asm__("cWorldTriggerManager_HandleTrigger");
    void Update() __asm__("cWorldTriggerManager_Update");
    void StopAllTriggers() __asm__("cWorldTriggerManager_StopAllTriggers");
    void StopTrigger(int id) __asm__("cWorldTriggerManager_StopTrigger");
    int HandleCollision(void* evt, int id, cRider* rider, int vol) __asm__("cWorldTriggerManager_HandleCollision");
    int TriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort, int check)
        __asm__("cWorldTriggerManager_TriggerToPatch");
    int AdvancedTriggerToPatch(int trigger, int* bank, int* patch, int* noAudio, int viewPort)
        __asm__("cWorldTriggerManager_AdvancedTriggerToPatch");
    int SwapTriggerToString(int idx, char* out) __asm__("cWorldTriggerManager_SwapTriggerToString");
    bool HasAfterCollisionTrigger(void* obj) __asm__("cWorldTriggerManager_HasAfterCollisionTrigger");
    bool AfterCollisionTrigger(unsigned int i) __asm__("cWorldTriggerManager_AfterCollisionTrigger");
    int ObjectHit(int id) __asm__("cWorldTriggerManager_ObjectHit");
    void AddHitObject(int id) __asm__("cWorldTriggerManager_AddHitObject");
    void Reset() __asm__("cWorldTriggerManager_Reset");
    void ResetStates() __asm__("cWorldTriggerManager_ResetStates__FPv");
    bool SwapTrigger(int trigger) __asm__("cWorldTriggerManager_SwapTrigger");
    void MonitorTriggers(int enable, int delay) __asm__("cWorldTriggerManager_MonitorTriggers");
    bool SyncToAnimationTrigger(unsigned int i) __asm__("cWorldTriggerManager_SyncToAnimationTrigger");
    int ObjectRefFirstFrame(tInstanceRecord* src, cWorldTriggerInstance* state)
        __asm__("cWorldTriggerManager_ObjectRefFirstFrame");
    bool DuckAmbientAudioTrigger(unsigned int idx) __asm__("cWorldTriggerManager_DuckAmbientAudioTrigger");
    int AdvancedTriggerToString() __asm__("cWorldTriggerManager_AdvancedTriggerToString__FPv");
};

CHECK_OFFSET(cWorldTriggerManager, m_aWorldTriggerInstances, 0x000);
CHECK_OFFSET(cWorldTriggerManager, mHitObjects, 0x004);
CHECK_OFFSET(cWorldTriggerManager, mNumHitObjects, 0x324);
CHECK_OFFSET(cWorldTriggerManager, mLastHitObject, 0x328);
CHECK_OFFSET(cWorldTriggerManager, mLastHitObjectChecked, 0x32C);
CHECK_OFFSET(cWorldTriggerManager, mLastHitObjectResult, 0x330);
CHECK_OFFSET(cWorldTriggerManager, mMonitorTriggers, 0x334);
CHECK_OFFSET(cWorldTriggerManager, m_pcTriggerInfoFile, 0x338);
CHECK_OFFSET(cWorldTriggerManager, m_aTriggerInfo, 0x33C);
CHECK_OFFSET(cWorldTriggerManager, m_iNumTriggers, 0x340);
CHECK_SIZE(cWorldTriggerManager, 0x344);

// The one instance (D_004A52D4): created by WORLDTRIGGERMANAGER_Init__Fv, destroyed (flags 3) by
// WORLDTRIGGERMANAGER_DeInit.
extern cWorldTriggerManager* gWorldTriggerManager;

#endif /* SOUND_ICEPICK_WORLDTRIGGERMANAGER_H */
