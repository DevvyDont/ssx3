#ifndef SOUND_STREAMINSTANCE_H
#define SOUND_STREAMINSTANCE_H

// cStreamInstance: one streamed sound (sound/streamsys): its buffer (own or external), stream handle, base volume and
// volume control, and the big file it streams from. Methods: the constructor, SetBaseVolume, Purge.
//
// Size 0x84 (On Tour's; retail's methods reach 0x80, the last member). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods
// falls on a member start (tools/gen/self_offsets.py). Enums and bools are words.

#include "layout.h"


struct cSndPlayOpts;

struct cStreamInstance {
    char* mBuffer;                  // 0x000 [O]
    int mExternalBuffer;            // 0x004 [O] bool
    int mHandle;                    // 0x008 [O]
    signed char mBaseVol;           // 0x00C [O]
    char unk_00D[0x3];              // 0x00D padding
    float* mVolControl;             // 0x010 [O]
    char mBigFileName[96];          // 0x014 [O]
    char* mBigHeader;               // 0x074 [O]
    int mBigHandle;                 // 0x078 [O]
    int mMemAllocType;              // 0x07C [O]
    int mQueuedTimeout;             // 0x080 [O]

    // Methods bound to their retail symbols by asm labels.
    cStreamInstance* cStreamInstance1(cSndPlayOpts* p, char* mem, int size, int n, int maxChunks)
        __asm__("cStreamInstance_cStreamInstance1");
    void * cStreamInstance2(int handle) __asm__("cStreamInstance_cStreamInstance2");
    void SetBigFile(int name) __asm__("cStreamInstance_SetBigFile");
    void Set3DPosition(int azimuth, int elevation) __asm__("cStreamInstance_Set3DPosition");
    void SetBaseVolume(int val) __asm__("cStreamInstance_SetBaseVolume__FPvi");
    void SetPitch(int pitch) __asm__("cStreamInstance_SetPitch");
    void Update() __asm__("cStreamInstance_Update");
    float GetTime() __asm__("cStreamInstance_GetTime");
    float GetTimeRemaining() __asm__("cStreamInstance_GetTimeRemaining");
    bool IsBufferFilled(int minVal) __asm__("cStreamInstance_IsBufferFilled");
    bool IsCurrRequestedRead() __asm__("cStreamInstance_IsCurrRequestedRead");
    bool Playing() __asm__("cStreamInstance_Playing");
    void ReleaseHold() __asm__("cStreamInstance_ReleaseHold");
    int GetCurrActiveSampleHandle() __asm__("cStreamInstance_GetCurrActiveSampleHandle");
    void ReleaseHold1(int requestHandle) __asm__("cStreamInstance_ReleaseHold1");
    float GetRequestLength() __asm__("cStreamInstance_GetRequestLength");
    void Pause() __asm__("cStreamInstance_Pause");
    void Resume() __asm__("cStreamInstance_Resume");
    int Queue(int holdTime, int buf, int off) __asm__("cStreamInstance_Queue");
    int Queue1(int holdTime, int element, int off) __asm__("cStreamInstance_Queue1");
    void* Purge() __asm__("cStreamInstance_Purge__FPv");
    int Requests() __asm__("cStreamInstance_Requests");
    int TimeMult(int multAmount) __asm__("cStreamInstance_TimeMult");
    int GetStatus(void* out) __asm__("cStreamInstance_GetStatus");
};

CHECK_OFFSET(cStreamInstance, mBuffer, 0x000);
CHECK_OFFSET(cStreamInstance, mExternalBuffer, 0x004);
CHECK_OFFSET(cStreamInstance, mHandle, 0x008);
CHECK_OFFSET(cStreamInstance, mBaseVol, 0x00C);
CHECK_OFFSET(cStreamInstance, mVolControl, 0x010);
CHECK_OFFSET(cStreamInstance, mBigFileName, 0x014);
CHECK_OFFSET(cStreamInstance, mBigHeader, 0x074);
CHECK_OFFSET(cStreamInstance, mBigHandle, 0x078);
CHECK_OFFSET(cStreamInstance, mMemAllocType, 0x07C);
CHECK_OFFSET(cStreamInstance, mQueuedTimeout, 0x080);
CHECK_SIZE(cStreamInstance, 0x84);

#endif
