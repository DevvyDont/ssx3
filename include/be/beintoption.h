#ifndef BE_BEINTOPTION_H
#define BE_BEINTOPTION_H

// cBEOptionInterface: the back end's options interface - language, units, widescreen, brightness, replay, autosave,
// the sound settings (mode, mix, volumes, arcade effects, DJ speech, voice chat), the HUD detail, the screen offsets
// and the online quick-chat messages, with their defaults (defaultSound/Game/HUD/Ctrl). The object holds no options:
// like every BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and
// work on the static cBEOptionInterface::mOption (retail cBEOptionInterface_mOption, D_00535610, a 0x288-byte
// tBEOptionDB below: getOptions / setOptions copy it whole); submit copies it to the saved copy cBELibrary_optionDB
// (On Tour: cBELibrary::optionDB), discard copies it back, revert resets the defaults. One instance, created on demand
// by cBEOptionInterface_getThis
// (`cMemMan_alloc(0x10, "BEOptionInt", 0, 0)`, vtable cBEOptionInterface_vtbl, then cBEOptionInterface_revert, then
// bIsActive = 0) into cBEOptionInterface::instance (retail cBEOptionInterface_instance, D_004A120C); the rest of the
// game reaches it as cBE_getInterface(cBE_getBE(), 4). EA's source: d:/ssx/code/source/be/beintoption.cpp, with
// inline methods in ../source/be/beintoption.h (preview assert strings).
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's cBEOptionInterface
// has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// cBEOptionInterface_vtbl (0x45ADC8), slot 1 submit = cBEOptionInterface_submit, slot 2 discard =
// cBEOptionInterface_discard, slots 3/4 cBEInterface_writeToReplayFrame / cBEInterface_readFromReplayFrame.
//
// Evidence tags:
//   [R] retail's getThis (allocation, vptr, bIsActive) and constructor cBEOptionInterface_cBEOptionInterface (vptr);
//   [P] the July 2003 preview's inline methods: __18cBEOptionInterface stores the vtable at 0xC; its 22 other inline
//       accessors address mOption, not `this`;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBEInterface: bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools
//       are 1 byte, retail's are words).

#include "layout.h"

struct cBEOptionInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U]
                                     //       cBEOptionInterface_setOptionActive (static: instance->bIsActive = arg);
                                     //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit,
                                     //       cBERaceState_onEnter/onExit set 0/1
    void** mVtbl;                    // 0x00C [R] getThis and cBEOptionInterface_cBEOptionInterface store
                                     //       cBEOptionInterface_vtbl; [P] __18cBEOptionInterface; [U]
                                     //       defaultSound/defaultGame/defaultHUD/defaultCtrl and func_001C2BA8 call
                                     //       slot 1 (submit) through it, cBESaveInterface_loadSaveGame slot 2 (discard)
};

extern cBEOptionInterface* cBEOptionInterface_instance;

CHECK_OFFSET(cBEOptionInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBEOptionInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBEOptionInterface, bIsActive, 0x08);
CHECK_OFFSET(cBEOptionInterface, mVtbl, 0x0C);
CHECK_SIZE(cBEOptionInterface, 0x10);

// The one options interface (On Tour: cBEOptionInterface::instance), created by cBEOptionInterface_getThis.

// tBEOptionDB: the option block itself, the static cBEOptionInterface::mOption (retail cBEOptionInterface_mOption,
// D_00535610) and its saved copy cBELibrary_optionDB. Name and members: [O] On Tour struct tBEOptionDB (0x288: one
// word of bit-fields, the HUD / camera byte, the two screen offsets, the quick-chat messages); [R] the same 0x288
// bytes in retail (getOptions / setOptions in be/bedbreward copy 0x288; the save game stores it), and retail's
// accessors place every bit-field below at On Tour's position (be/bedbreward: setRadioBigVolume bits 0-3 ..
// setReplayOption bit 29; the FE options screens bits 19-24) except the HUD detail, which retail keeps in bits 30-31 of
// the first word (cBEOptionInterface_defaultHUD, the pause menu's HUD list box) where On Tour has iAutosave and then a
// byte-4 field. g++ 2.95 on the EE reads these bit-fields with lw / sw (also the byte-4 one).
struct tBEOptionDB {
    unsigned int iRadioBigVolume : 4;   // 0x0 bits 0-3 [O]; [R] defaultSound 10; setRadioBigVolume / get
    unsigned int iCharVolume : 4;       // 0x0 bits 4-7 [O]; [R] defaultSound 10
    unsigned int iSFXVolume : 4;        // 0x0 bits 8-11 [O]; [R] defaultSound 10
    unsigned int iSoundMode : 2;        // 0x0 bits 12-13 [O]; [R] defaultSound 0; the FE sound screens
    unsigned int iAudioMix : 2;         // 0x0 bits 14-15 [O]; [R] defaultSound 0
    unsigned int iUseArcadeSFX : 1;     // 0x0 bit 16 [O]; [R] defaultSound 1
    unsigned int iUseDJSpeech : 1;      // 0x0 bit 17 [O]; [R] defaultSound 1
    unsigned int iUseVOIP : 1;          // 0x0 bit 18 [O]; [R] defaultSound 1
    unsigned int iUseMetricUnits : 1;   // 0x0 bit 19 [O]; [R] defaultGame 0
    unsigned int iUseWidescreen : 2;    // 0x0 bits 20-21 [O]; [R] defaultGame from the console config
    unsigned int iLanguage : 3;         // 0x0 bits 22-24 [O]; [R] defaultGame 0; [U] func_0029F198 maps it to the
                                        //     speech language bit
    unsigned int iBrightness : 4;       // 0x0 bits 25-28 [O]; [R] defaultGame 9
    unsigned int iReplayOption : 1;     // 0x0 bit 29 [O]; [R] defaultGame 0
    unsigned int iHUDDetail : 2;        // 0x0 bits 30-31 [R] defaultHUD 0; the pause menu's HUD list (Full / Minimal /
                                        //     None) (On Tour: iAutosave here, iHUDDetail in byte 4)
    unsigned char unk_04 : 2;           // 0x4 bits 0-1 [R] defaultSound 1 (On Tour's byte 4 holds iHUDDetail /
                                        //     iCameraMode)
    signed char iVtDelta;               // 0x5 [O]; [R] defaultGame 0; bedbreward's vertical-offset setter
    signed char iHzDelta;               // 0x6 [O]; [R] defaultGame 0; the horizontal-offset setter
    char szQuickMessage[10][64];        // 0x7 [O]; [R] setQuickKeyMessage / resetQuickKey: 64-byte entries from +7
};

CHECK_OFFSET(tBEOptionDB, iVtDelta, 0x5);
CHECK_OFFSET(tBEOptionDB, iHzDelta, 0x6);
CHECK_OFFSET(tBEOptionDB, szQuickMessage, 0x7);
CHECK_SIZE(tBEOptionDB, 0x288);

// ---- The race rules block cBERules::mRules (views group D) ----
//
// tBEBraggingRights: one human player's bragging-rights handicap. [O] On Tour struct tBEBraggingRights (0xC): an
// anonymous union of rule flags at 0x0 (its members are not in the dump), StartingPoints, HeadStartSecs;
// [U] cBERules_getPlayerStartDelaySecs(i) reads HeadStartSecs of record i (+0x8, 0xC stride), the option setters /
// getters (cBEOptionInterface_*) read and write the three words.
struct tBEBraggingRights {
    unsigned int unk_00;            // 0x0 [O] an anonymous union (the rule flags; member names not in On Tour's dump)
    int StartingPoints;             // 0x4 [O]
    int HeadStartSecs;              // 0x8 [O]; [U] cBERules_getPlayerStartDelaySecs
};

CHECK_OFFSET(tBEBraggingRights, unk_00, 0x0);
CHECK_OFFSET(tBEBraggingRights, StartingPoints, 0x4);
CHECK_OFFSET(tBEBraggingRights, HeadStartSecs, 0x8);
CHECK_SIZE(tBEBraggingRights, 0xC);

// tBEMyRules: the rules that are not per player. [O] On Tour struct tBEMyRules (0x8): an anonymous union, then
// AIHeadStartSecs; [U] cBERules_getAIStartDelaySecs reads AIHeadStartSecs (retail D_005308D4 = cBERules_mRules + 0x1C).
struct tBEMyRules {
    unsigned int unk_00;            // 0x0 [O] an anonymous union (member names not in On Tour's dump)
    int AIHeadStartSecs;            // 0x4 [O]; [U] cBERules_getAIStartDelaySecs
};

CHECK_OFFSET(tBEMyRules, unk_00, 0x0);
CHECK_OFFSET(tBEMyRules, AIHeadStartSecs, 0x4);
CHECK_SIZE(tBEMyRules, 0x8);

// tBERuleOptionDB: [O] On Tour's static cBERules::mRules is a tBERuleOptionDB {tBEBraggingRights Bragging[4];
// tBEMyRules MyRules;} (0x38, four players). Retail has two players: [U] the getters index Bragging[0..1] (0xC stride
// from cBERules_mRules) and MyRules.AIHeadStartSecs is at +0x1C, so MyRules is at 0x18 and the block is 0x20.
struct tBERuleOptionDB {
    tBEBraggingRights Bragging[2];  // 0x00 [O] ([4] in On Tour); [U] cBERules_getPlayerStartDelaySecs(i)
    tBEMyRules MyRules;             // 0x18 [O]; [U] + 0x1C read by cBERules_getAIStartDelaySecs
};

CHECK_OFFSET(tBERuleOptionDB, Bragging, 0x00);
CHECK_OFFSET(tBERuleOptionDB, MyRules, 0x18);
CHECK_SIZE(tBERuleOptionDB, 0x20);
extern tBERuleOptionDB cBERules_mRules;         // [O] cBERules::mRules; [R] 0x005308B8


#endif /* BE_BEINTOPTION_H */
