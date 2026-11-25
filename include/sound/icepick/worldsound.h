#ifndef SOUND_ICEPICK_WORLDSOUND_H
#define SOUND_ICEPICK_WORLDSOUND_H

// cWorldSound: the world's static audio volumes (Icepick world audio). For every streamed world section it resolves
// the section's instance audio list (InitWorldAudioFile -> ResolveAudioFile -> CreateAudioGrid) and puts each audio
// instance in the world's Nash space hash (AddInstanceToGrid); per view port it keeps the list of audio instances near
// the listener (an overlap list, refreshed when the listener moved 10 units) and asks cWorldTriggerManager to play the
// volumes the listener is in (TriggerWorldAudio -> TriggerGridAudioNode -> cWorldTriggerManager_HandleTrigger).
// One instance per track: cGame::WorldSound (+0x4C). EA's source: d:/ssx/code/source/sound/icepick/worldsound.cpp
// and worldsound.h (preview assert paths); retail keeps the methods in sound/icepick/worldtriggermanager
// (0x002B6AA0 ctor .. 0x002B82B8 CalculateVolume).
//
// Size 0x250: [R] cGame_loadTrack allocates cMemMan_alloc(0x250, "cWorldSound") and runs
// cWorldSound_cWorldSound(mem, 0) on it; the ctor initialises two 0x120-byte overlap lists (count 0 at +0x0,
// last position = t4vect_Zero at +0x110). [O] On Tour's cWorldSound is 0x250: m_OverlapLists[2] at 0x0, then
// m_AudioRenderInstance / m_AudioRenderHull at 0x240 / 0x244. No bases, no vtable (the destructor is a plain
// operator delete).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] the constructor and the allocation; [U] how retail code uses the field; [O] SSX On Tour's DWARF.

#include "layout.h"
#include "bx/math3d.h"
#include "intersect/bxnash.h"

struct tInstanceRecord;

// The head of an audio instance's data (On Tour struct tAudioData, 0x8; the On Tour locals that read it are named
// pAData). m_iNumCollisionTriggers trigger words (eAudioTrigger) follow it, then m_iNumVolumes variable-length volume
// records, each starting with its type ([O] tAudioVolume::m_eType; [U] the walks step 0x1C for type 0, 0x30 for 1 and
// 2, 0x18 for 3: On Tour's tAudioVolumeSphere, tAudioVolumeEllipse / tAudioVolumeCone and tAudioVolumeAmbient sizes)
// and its trigger number at +0x4.
struct tAudioData {
    int m_iNumCollisionTriggers;    // 0x0 [O]; [U] cWorldSound_CalcInstAudioBoundingBox / GetAudioVolumeTriggers /
                                    //      TriggerGridAudioNode skip this many words after the header
    int m_iNumVolumes;              // 0x4 [O]; [U] the volume records they walk; AddInstanceToGrid /
                                    //      RemoveInstanceFromGrid skip an instance without volumes
};

CHECK_OFFSET(tAudioData, m_iNumCollisionTriggers, 0x0);
CHECK_OFFSET(tAudioData, m_iNumVolumes, 0x4);
CHECK_SIZE(tAudioData, 0x8);

// The head of each volume record after tAudioData (On Tour struct tAudioVolume, 0x4, and its four record types;
// names from On Tour's DWARF). The type is On Tour's eAudioVolumeType: kAUDIO_VOLUME_SPHERE 0, kAUDIO_VOLUME_ELLIPSE 1,
// kAUDIO_VOLUME_CONE 2, kAUDIO_VOLUME_AMBIENT 3; [U] the walks step 0x1C, 0x30, 0x30 and 0x18 by it, On Tour's sizes.
enum eAudioVolumeType {
    kAUDIO_VOLUME_SPHERE = 0, kAUDIO_VOLUME_ELLIPSE = 1, kAUDIO_VOLUME_CONE = 2, kAUDIO_VOLUME_AMBIENT = 3
};

struct tAudioVolume {
    int m_eType;                    // 0x0 [O] eAudioVolumeType; [U] cWorldSound_CalcAudioVolumeBox switches on it
};

struct tAudioVolumeSphere : public tAudioVolume {
    int m_eTrigger;                 // 0x04 [O] eAudioTrigger (the record's trigger number)
    float m_f3pos[3];               // 0x08 [O]; [U] CalcAudioVolumeBox: the centre
    float m_fRadius;                // 0x14 [O]; [U] CalcAudioVolumeBox: the box is the centre +- it
    float m_fDecayRate;             // 0x18 [O]
};

struct tAudioVolumeEllipse : public tAudioVolume {
    int m_eTrigger;                 // 0x04 [O] eAudioTrigger
    float m_f3pos[3];               // 0x08 [O]; [U] CalcAudioVolumeBox: the centre
    float m_f3radius[3];            // 0x14 [O]; [U] CalcAudioVolumeBox: the half-extents along the axis, side and up
    float m_f3dir[3];               // 0x20 [O]; [U] CalcAudioVolumeBox: the axis its basis is built from
    float m_fDecayRate;             // 0x2C [O]
};

struct tAudioVolumeCone : public tAudioVolume {
    int m_eTrigger;                 // 0x04 [O] eAudioTrigger
    float m_f3pos[3];               // 0x08 [O]; [U] CalcAudioVolumeBox: the centre
    float m_fRadius;                // 0x14 [O]; [U] CalcAudioVolumeBox: the box is the centre +- it
    float m_f3dir[3];               // 0x18 [O]
    float m_fDecayRate;             // 0x24 [O]
    float m_fcoscone;               // 0x28 [O]
    float m_fDropOff;               // 0x2C [O]
};

struct tAudioVolumeAmbient : public tAudioVolume {
    int m_eTrigger;                 // 0x04 [O] eAudioTrigger
    float m_f3pos[3];               // 0x08 [O]; [U] CalcAudioVolumeBox: the centre
    float m_fRadius;                // 0x14 [O]; [U] CalcAudioVolumeBox: the box is the centre +- it
};

CHECK_OFFSET(tAudioVolume, m_eType, 0x0);
CHECK_SIZE(tAudioVolume, 0x4);
CHECK_OFFSET(tAudioVolumeSphere, m_eTrigger, 0x04);
CHECK_OFFSET(tAudioVolumeSphere, m_f3pos, 0x08);
CHECK_OFFSET(tAudioVolumeSphere, m_fRadius, 0x14);
CHECK_OFFSET(tAudioVolumeSphere, m_fDecayRate, 0x18);
CHECK_SIZE(tAudioVolumeSphere, 0x1C);
CHECK_OFFSET(tAudioVolumeEllipse, m_eTrigger, 0x04);
CHECK_OFFSET(tAudioVolumeEllipse, m_f3pos, 0x08);
CHECK_OFFSET(tAudioVolumeEllipse, m_f3radius, 0x14);
CHECK_OFFSET(tAudioVolumeEllipse, m_f3dir, 0x20);
CHECK_OFFSET(tAudioVolumeEllipse, m_fDecayRate, 0x2C);
CHECK_SIZE(tAudioVolumeEllipse, 0x30);
CHECK_OFFSET(tAudioVolumeCone, m_eTrigger, 0x04);
CHECK_OFFSET(tAudioVolumeCone, m_f3pos, 0x08);
CHECK_OFFSET(tAudioVolumeCone, m_fRadius, 0x14);
CHECK_OFFSET(tAudioVolumeCone, m_f3dir, 0x18);
CHECK_OFFSET(tAudioVolumeCone, m_fDecayRate, 0x24);
CHECK_OFFSET(tAudioVolumeCone, m_fcoscone, 0x28);
CHECK_OFFSET(tAudioVolumeCone, m_fDropOff, 0x2C);
CHECK_SIZE(tAudioVolumeCone, 0x30);
CHECK_OFFSET(tAudioVolumeAmbient, m_eTrigger, 0x04);
CHECK_OFFSET(tAudioVolumeAmbient, m_f3pos, 0x08);
CHECK_OFFSET(tAudioVolumeAmbient, m_fRadius, 0x14);
CHECK_SIZE(tAudioVolumeAmbient, 0x18);

// One placed instance's audio (On Tour struct tInstanceAudio, 0x18), filed in the world's nash as a typed space node.
// On Tour declares the members inside two anonymous unions (at 0x0 and 0x10) whose members its dump does not list:
// the names below are ours. [U] func_002B6B50 (the world audio file's resolve) walks the file's array of them (count
// at +0xC, the array at +0x10, 0x18 each): it turns mpAudioData from a file offset into a pointer and mpInstance from
// a resource id into the instance record (0 when it is not loaded), and points the record's m_pInstanceAudio back here.
struct tInstanceAudio {
    cTypedSpaceNode mSpaceNode;     // 0x00 [U] AddInstanceToGrid sets type 5 (kSNT_AudioVolume) and files it in the
                                    //      nash; RemoveInstanceFromGrid unfiles it; cAudioOverlapList collects it
    tInstanceRecord* mpInstance;    // 0x0C [U] the instance (On Tour locals pRec / pInst): CreateAudioGrid /
                                    //      DesolveAudioFile / AddInstanceToGrid skip an audio instance without one;
                                    //      CalcInstAudioBoundingBox / TriggerGridAudioNode offset the volumes by its
                                    //      position
    tAudioData* mpAudioData;        // 0x10 [U] the resolve (from a file offset); the walks above read it
    int unk_14;                     // 0x14 no retail access seen (On Tour's second union is 8 bytes)
};

CHECK_OFFSET(tInstanceAudio, mSpaceNode, 0x00);
CHECK_OFFSET(tInstanceAudio, mpInstance, 0x0C);
CHECK_OFFSET(tInstanceAudio, mpAudioData, 0x10);
CHECK_OFFSET(tInstanceAudio, unk_14, 0x14);
CHECK_SIZE(tInstanceAudio, 0x18);

// The audio instances near one view port's listener (On Tour class cAudioOverlapList, 0x120; its method
// cAudioOverlapList_update is in the same retail unit).
struct cAudioOverlapList {
    int numoverlaps;                // 0x000 [O]; [R] cWorldSound_cWorldSound stores 0; [U] cAudioOverlapList_update
                                    //       clears it before re-querying, the query helper func_002B83F8 appends
                                    //       (overlaps[numoverlaps++], no bound check in retail); TriggerWorldAudio /
                                    //       RemoveInstanceFromOverlapList loop over it
    tInstanceAudio* overlaps[64];   // 0x004 [O]; [U] TriggerWorldAudio passes each non-zero entry to
                                    //       TriggerGridAudioNode; RemoveInstanceFromOverlapList zeroes matches
                                    //       (view sTriggerIdSlot120.ids)
    char unk_104[0xC];              // 0x104 alignment gap before the 16-byte-aligned lastposition (as in On Tour)
    t4Vector lastposition;          // 0x110 [O]; [R] the ctor copies t4vect_Zero (lq/sq); [U] cAudioOverlapList_update
                                    //       re-queries when its w is 0 or the listener moved more than 10, then stores
                                    //       it

    // Methods bound to their retail symbols by asm labels.
    void update(const t4Vector* pos) __asm__("cAudioOverlapList_update");
};

CHECK_OFFSET(cAudioOverlapList, numoverlaps, 0x000);
CHECK_OFFSET(cAudioOverlapList, overlaps, 0x004);
CHECK_OFFSET(cAudioOverlapList, unk_104, 0x104);
CHECK_OFFSET(cAudioOverlapList, lastposition, 0x110);
CHECK_SIZE(cAudioOverlapList, 0x120);

struct t3Vector;
struct t4Vector;

struct cWorldSound {
    cAudioOverlapList m_OverlapLists[2];    // 0x000 [O]; [R] the ctor's 2-element loop (stride 0x120); [U]
                                            //       TriggerWorldAudio updates and walks [ViewPort];
                                            //       RemoveInstanceFromOverlapList clears an instance from both
    tInstanceRecord* m_AudioRenderInstance; // 0x240 [O] only: no retail code reads or writes it (On Tour's debug render
                                            //       of the audio volumes); placed by On Tour, whose class matches
                                            //       retail's up to here and in size (0x250)
    int m_AudioRenderHull;                  // 0x244 [O] only: as above, never accessed in retail
    char unk_248[0x8];                      // 0x248 tail padding to the 16-byte-aligned size 0x250 (On Tour has it too)

    // Methods bound to their retail symbols by asm labels.
    int InitWorldAudioFile(int icePickData) __asm__("cWorldSound_InitWorldAudioFile");
    void PurgeWorldAudioFile(int icePickData) __asm__("cWorldSound_PurgeWorldAudioFile");
    int CreateAudioGrid(int icePickData, tInstanceAudio* entries, int n) __asm__("cWorldSound_CreateAudioGrid");
    void CalcInstAudioBoundingBox(tInstanceAudio* entry, t3Vector* lo, t3Vector* hi)
        __asm__("cWorldSound_CalcInstAudioBoundingBox");
    void AddInstanceToGrid(tInstanceAudio* entry) __asm__("cWorldSound_AddInstanceToGrid");
    void RemoveInstanceFromGrid(tInstanceAudio* instAudio) __asm__("cWorldSound_RemoveInstanceFromGrid");
    void RemoveInstanceFromOverlapList(tInstanceAudio* pInst) __asm__("cWorldSound_RemoveInstanceFromOverlapList");
    void TriggerWorldAudio(const t4Vector* pos, int k) __asm__("cWorldSound_TriggerWorldAudio");
    void GetAudioVolumeTriggers(tInstanceRecord* obj, int* id0, int* id1, int* id2, int* id3)
        __asm__("cWorldSound_GetAudioVolumeTriggers");
};

CHECK_OFFSET(cWorldSound, m_OverlapLists, 0x000);
CHECK_OFFSET(cWorldSound, m_AudioRenderInstance, 0x240);
CHECK_OFFSET(cWorldSound, m_AudioRenderHull, 0x244);
CHECK_OFFSET(cWorldSound, unk_248, 0x248);
CHECK_SIZE(cWorldSound, 0x250);

#endif /* SOUND_ICEPICK_WORLDSOUND_H */
