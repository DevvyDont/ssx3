#ifndef SOUND_PS2SOUNDMAN_H
#define SOUND_PS2SOUNDMAN_H

// cPS2SoundManager: the PS2 sound back end owned by cSoundSys (cSoundSys::pManager): initialises the EA sound
// driver for the audio mode and runs a 32-node allocator for SPU sound RAM (bank uploads), growing down from
// mMainMemHighAddr. EA source d:/ssx/code/source/sound/ps2soundman.cpp (preview assert strings "mSoundHeap == NULL",
// "pCurrFreeNode->state == kePS2SoundMemAllocState_Free", "memHandle >= 0 && memHandle <
// kPS2SOUNDMANAGER_MAX_SOUNDMEM_ALLOCS", "pNode->lowAddr == mMainMemHighAddr", ...); preview vtable
// _vt$16cPS2SoundManager. Retail methods: sound/soundsys unit 0x2A8C20..0x2A9350.
//   cPS2SoundManager : cSoundManager (On Tour: an interface with only a vptr, size 0x4; On Tour's PSP version is
//   cPSPSoundManager, 0x8)
// Size 0x320 [R]: cSoundManager::Construct (func_002A8FA0) = ctor func_002A8C20(cMemMan_alloc(0x320,
// "PS2SoundManager")). Vtable D_004835F8: Init, SetMode, Shutdown, AllocSoundMem, DeallocSoundMem,
// SetSoundMemLoadLimits, RestoreSoundMemLoadLimits, dtor (preview cPS2SoundManager::<virtual slot 1..8>, On Tour's
// cSoundManager method order). The preview's offsets agree with retail (asserts at preview 0x27A37C..0x27AD4C).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] the constructor func_002A8C20, Init func_002A8CB0 (allocation tag), the destructor func_002A8C50;
//   [P] the preview's assert strings next to the field they test;
//   [O] SSX On Tour's DWARF (cSoundManager / cPSPSoundManager: method names only);
//   [U] how the retail methods use the field.

#include "layout.h"

// One sound-RAM allocation node (0x18; our struct name, EA member names from the preview's asserts, which call the
// nodes pCurrFreeNode / pNewNode / pNode).
struct tPS2SoundMemNode {
    int index;    // 0x00 [R] Init sets node i's to i; [U] AllocSoundMem returns it as the memory handle
    int lowAddr;  // 0x04 [P] "pCurrFreeNode->lowAddr != -1 && pCurrFreeNode->highAddr != -1" (preview lw 4); [R] Init -1
    int highAddr; // 0x08 [P] same assert (preview lw 8); [R] Init -1
    int size;     // 0x0C [P] "pCurrFreeNode->size > 0" (preview lw 12); [R] Init 0
    int state;    // 0x10 [P] kePS2SoundMemAllocState_Free (1) / _Used (2) asserts (preview lw 16); [R] Init 0 (_Unused)
    int next;     // 0x14 [R] Init i - 1 (unused-node chain); [U] free-list link, -1 ends it
};

struct cPS2SoundManager {
    void** mVtbl;                     // 0x000 [R] D_004835F8 (_vt$16cPS2SoundManager), stored by the ctor and dtor (cSoundManager has no data: the vptr is first)
    void* mSoundHeap;                 // 0x004 [P] "mSoundHeap == NULL" (preview dtor lw 4); [R] Init: operator new(0x32000, "mSoundHeap"), passed to the sound driver; Shutdown / dtor cMemMan_free() it
    int unk_008;                      // 0x008 [R] ctor 0; [U] SetMode (func_002A8D90) records the driver mode set (1 / 2) and returns early when unchanged
    tPS2SoundMemNode mNodes[32];      // 0x00C [P] kPS2SOUNDMANAGER_MAX_SOUNDMEM_ALLOCS (preview sltiu 32, stride 24, base 12); [R] Init chains them
    int mMainMemHighAddr;             // 0x30C [P] "(mainMemLowAddr + mainMemSize) == mMainMemHighAddr", "pNode->lowAddr == mMainMemHighAddr" (preview lw 780); [R] ctor -1, Init = sound RAM top; [U] AllocSoundMem moves it down
    int unk_310;                      // 0x310 [R] ctor -1, Init 31; [U] head of the unused-node chain (func_002A92B8 pops, func_002A92F8 pushes)
    char unk_314[0x4];                // 0x314 no access
    int unk_318;                      // 0x318 [R] ctor -1; Init -1; [U] head of the free-block list (sorted by address: AllocSoundMem / DeallocSoundMem)
    int unk_31C;                      // 0x31C [R] ctor 0; [U] SetSoundMemLoadLimits (func_002A9250) 1, RestoreSoundMemLoadLimits (func_002A9288) 0; AllocSoundMem returns -2 while set (preview lw 796)
};

CHECK_OFFSET(tPS2SoundMemNode, index, 0x00);
CHECK_OFFSET(tPS2SoundMemNode, lowAddr, 0x04);
CHECK_OFFSET(tPS2SoundMemNode, highAddr, 0x08);
CHECK_OFFSET(tPS2SoundMemNode, size, 0x0C);
CHECK_OFFSET(tPS2SoundMemNode, state, 0x10);
CHECK_OFFSET(tPS2SoundMemNode, next, 0x14);
CHECK_SIZE(tPS2SoundMemNode, 0x18);
CHECK_OFFSET(cPS2SoundManager, mVtbl, 0x000);
CHECK_OFFSET(cPS2SoundManager, mSoundHeap, 0x004);
CHECK_OFFSET(cPS2SoundManager, unk_008, 0x008);
CHECK_OFFSET(cPS2SoundManager, mNodes, 0x00C);
CHECK_OFFSET(cPS2SoundManager, mMainMemHighAddr, 0x30C);
CHECK_OFFSET(cPS2SoundManager, unk_310, 0x310);
CHECK_OFFSET(cPS2SoundManager, unk_314, 0x314);
CHECK_OFFSET(cPS2SoundManager, unk_318, 0x318);
CHECK_OFFSET(cPS2SoundManager, unk_31C, 0x31C);
CHECK_SIZE(cPS2SoundManager, 0x320);

#endif /* SOUND_PS2SOUNDMAN_H */
