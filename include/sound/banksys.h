#ifndef SOUND_BANKSYS_H
#define SOUND_BANKSYS_H

// cBankSys: the bank-sample SFX player (SFX_Begin pushes a play-option level and records the bank/patch for it,
// SFX_End plays the patch through the shared cBankManager and hands the voice to its cBankMonitor).
//   cBankSys : virtual cBankManager (: cAsyncSys, cBankMonitor, virtual cSoundSys)
// Retail methods: sound/soundsys unit 0x28FD00..0x290B58 (ctor cBankSys_cBankSys, dtor cBankSys__cBankSys, the two
// monitor callbacks, SFX_End func_002906B8) and the out-of-line inlines in bx/seg_1BA100 0x2B9FB8..0x2BAC30 (the
// preview's linkonce SFX_Bank, SFX_Patch, SFX_Begin x3, SFX_Play2D, SFX_Play2DVol). EA's source file is unknown (no
// asserts; the preview's .gnu.linkonce.t.SFX_*__8cBankSys* show the inlines live in its header); the path is ours.
// Non-virtual part 0x10 [R]: the ctor (inchrg != 0) puts the virtual base cSoundSys at this + 0x10 and cBankManager at
// this + 0x18 (sw this + 0x18 into 0x0, this + 0x10 into cBankManager + 0x1D8 = 0x1F0); vtable D_00483A48 (one slot,
// func_002BA6B0) is stored at 0xC. The struct below is the NON-VIRTUAL part, so it can be embedded where the object
// lives: cMidiSys / cCrowdSys / cController / cSSXAudio all start with it (sound/midisys.h, sound/crowdsys.h,
// sound/ssxaudio.h). The only instance is cSSXAudio + 0x0; the ctor stores it in gBankSys (EA name unknown).
// Vtables: D_00483A48 is cBankSys's primary vtable (preview _vt$8cBankSys). cMidiSys adds no primary-table virtual, so
// its ctor/dtor (cController_cController, func_002A82D8) store the same D_00483A48 at 0xC; cBankSys's dtor lives in
// its virtual-base tables D_00483A60 (_vt$8cBankSys$9cSoundSys) and D_00483A80 (_vt$8cBankSys$12cBankManager).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructor cBankSys_cBankSys (allocation tags) and destructor cBankSys__cBankSys;
//   [O] SSX On Tour's DWARF (class cBankSys: mBank at 0x4, mPatch at 0x8; same members);
//   [U] how the retail methods use the field (SFX_Begin / SFX_End: index by the play-option level, mInst).

#include "layout.h"

struct cBankManager;

struct cBankSys {
    cBankManager* mpVBankManager; // 0x000 [R] g++ 2.95 virtual-base pointer to cBankManager (this + 0x18 in a complete cBankSys; cSSXAudio + 0x6C98); [U] every SFX_ method reaches the play options as mpVBankManager + 0x1D8 (cBankMonitor's cSndPlayOpts)
    int* mBank;                   // 0x004 [O]; [R] operator new(0x10, "mBank") (one per play-option level); [U] SFX_Bank (func_002B9FB8) / SFX_Begin store mBank[mInst], SFX_End (func_002906B8) reads it
    int* mPatch;                  // 0x008 [O]; [R] operator new(0x10, "mPath") (EA's tag spelling); [U] SFX_Patch (func_002B9FD8) / SFX_Begin store mPatch[mInst], SFX_End reads it
    void** mVtbl;                 // 0x00C [R] cBankSys vptr: D_00483A48 (cBankSys / cMidiSys ctors and dtors), D_004837F8 (cCrowdSys / cController), cSSXAudio_vtbl
};

CHECK_OFFSET(cBankSys, mpVBankManager, 0x000);
CHECK_OFFSET(cBankSys, mBank, 0x004);
CHECK_OFFSET(cBankSys, mPatch, 0x008);
CHECK_OFFSET(cBankSys, mVtbl, 0x00C);
CHECK_SIZE(cBankSys, 0x10);

// The bank SFX system: cBankSys_cBankSys stores itself here (the cBankSys base of the one cSSXAudio). EA name unknown.
extern cBankSys* gBankSys;          // [O] On Tour gBankSys; [R] cBankSys_cBankSys stores self, the dtor clears it

#endif /* SOUND_BANKSYS_H */
