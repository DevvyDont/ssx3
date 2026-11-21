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
    int m_bTouch;                       // 0x34 [O] (a 1-byte bool in On Tour; retail sw); [R] ctor 0, Reset 0; [U] the
                                        //      manager's TouchAllUsed / Update (view sWorldTrigger70.unk34)
    int m_bCrowdTrigger;                // 0x38 [O] (a 1-byte bool at 0x35 in On Tour; retail sw); [R] ctor 0; [U] Update
                                        //      stores CrowdTrigger's result; Stop picks the crowd system when set
    int m_iViewPort;                    // 0x3C [O] (On Tour 0x38); [U] SameTriggerInstance compares it; Update passes it to
                                        //      TriggerToPatch (not set by the ctor)
    char m_iSwapString[31];             // 0x40 [O] (On Tour 0x3C); [R] ctor / Reset / Stop store "" (sb of D_004A35A8[0]);
                                        //      [U] Update fills it with SwapTriggerToString
    char unk_5F[0x1];                   // 0x5F padding
    int m_iBankReference;               // 0x60 [O] (On Tour 0x5C); [R] ctor / Reset / Stop 0; [U] Update stores the bank
                                        //      TriggerToPatch returns
    int m_iPatchReference;              // 0x64 [O] (On Tour 0x60); [R] ctor / Reset / Stop 0; [U] Update stores the patch
    float m_iLastAnimFrame;             // 0x68 [O] (On Tour 0x64; EA's name, a float); [R] ctor / Reset / Stop -1.0f
    int m_bLastAnimFrameDecrease;       // 0x6C [O] (On Tour bool at 0x68; retail word); [R] ctor / Reset / Stop 0
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
