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

struct cWorldTriggerInstance;
struct tTriggerType;

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
