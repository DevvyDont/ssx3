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
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
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
#include "be/beinterface.h"

struct cBEOptionInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [U] cBEOptionInterface_setOptionActive (static: instance->bIsActive = arg);
    //       cBEPreRaceState_onEnter/onExit, cBEPostRaceState_onEnter/onExit, cBERaceState_onEnter/onExit set 0/1
    //   the vptr (0x0C): [R] getThis and cBEOptionInterface_cBEOptionInterface store cBEOptionInterface_vtbl; [P]
    //       __18cBEOptionInterface; [U] defaultSound/defaultGame/defaultHUD/defaultCtrl and cFEStateNetQuickMessages_onResetMessagesPopupEvent call slot 1
    //       (submit) through it, cBESaveInterface_loadSaveGame slot 2 (discard)

    // Methods bound to their retail symbols by asm labels.
    void setUseMetricUnits(int useMetricUnits) __asm__("cBEOptionInterface_setUseMetricUnits");
    void setUseWidescreen(int widescreen) __asm__("cBEOptionInterface_setUseWidescreen");
    void setLanguage(int language) __asm__("cBEOptionInterface_setLanguage");
    void setBrightness(int brightness) __asm__("cBEOptionInterface_setBrightness");
    void setReplayOption(int replayOption) __asm__("cBEOptionInterface_setReplayOption");
    void setSoundMode(int soundMode) __asm__("cBEOptionInterface_setSoundMode");
    void setAudioMix(int audioMix) __asm__("cBEOptionInterface_setAudioMix");
    void setRadioBigVolume(int radioVolume) __asm__("cBEOptionInterface_setRadioBigVolume");
    void setCharacterVolume(int charVolume) __asm__("cBEOptionInterface_setCharacterVolume");
    void setSFXVolume(int sfxVolume) __asm__("cBEOptionInterface_setSFXVolume");
    void setUseArcadeSFX(int useArcadeSfx) __asm__("cBEOptionInterface_setUseArcadeSFX");
    void setUseDJSpeechSFX(int useDJSpeech) __asm__("cBEOptionInterface_setUseDJSpeechSFX");
    void setUseVOIP(int useVoip) __asm__("cBEOptionInterface_setUseVOIP");
    void setQuickKeyMessage(int idx, const char* name) __asm__("cBEOptionInterface_setQuickKeyMessage");
    void setHUDDetail(int value) __asm__("cBEOptionInterface_setHUDDetail");
    void setVtDelta(int vertDelta) __asm__("cBEOptionInterface_setVtDelta");
    void setHzDelta(int horzDelta) __asm__("cBEOptionInterface_setHzDelta");
    void revert() __asm__("cBEOptionInterface_revert");
    void defaultSound() __asm__("cBEOptionInterface_defaultSound");
    void defaultGame() __asm__("cBEOptionInterface_defaultGame");
    void defaultHUD() __asm__("cBEOptionInterface_defaultHUD");
    void defaultCtrl() __asm__("cBEOptionInterface_defaultCtrl");
    char* getQuickKeyMessage(int i) __asm__("cBEOptionInterface_getQuickKeyMessage");
    void resetQuickKey(int i) __asm__("cBEOptionInterface_resetQuickKey");
    void resetAllQuickKeys() __asm__("cBEOptionInterface_resetAllQuickKeys");
    int getDefaultQuickKeyMessageHashValue(int value)
        __asm__("cBEOptionInterface_getDefaultQuickKeyMessageHashValue__FPvi");
};

extern cBEOptionInterface* cBEOptionInterface_instance;

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
    unsigned int iLanguage : 3;         // 0x0 bits 22-24 [O]; [R] defaultGame 0; [U] cSSXAudio_GetSpeechLanguage maps it to the
                                        //     speech language bit
    unsigned int iBrightness : 4;       // 0x0 bits 25-28 [O]; [R] defaultGame 9
    unsigned int iReplayOption : 1;     // 0x0 bit 29 [O]; [R] defaultGame 0
    unsigned int iHUDDetail : 2;        // 0x0 bits 30-31 [R] defaultHUD 0; the pause menu's HUD list (Full / Minimal /
                                        //     None) (On Tour: iAutosave here, iHUDDetail in byte 4)
    unsigned char iExtraSoundOption : 2; // 0x4 bits 0-1 (name ours: a 2-bit option reset with the sound settings)
                                        //     [R] defaultSound 1; no retail reader found (On Tour's byte 4 holds
                                        //     iHUDDetail / iCameraMode)
    signed char iVtDelta;               // 0x5 [O]; [R] defaultGame 0; bedbreward's vertical-offset setter
    signed char iHzDelta;               // 0x6 [O]; [R] defaultGame 0; the horizontal-offset setter
    char szQuickMessage[10][64];        // 0x7 [O]; [R] setQuickKeyMessage / resetQuickKey: 64-byte entries from +7
};

// tBEOptionDB::iUseWidescreen's settings (names ours). [U] the game options screen (cFEStateOptionsGame_onCreateScreen)
// lists kT_CMNOff, kT_19Widescreen169 and kT_19WidescreenAnimorphic in this order and selects iUseWidescreen.
enum eWidescreenSetting {
    kWidescreen_Off = 0,
    kWidescreen_16x9 = 1,
    kWidescreen_Anamorphic = 2
};

// The options' language index (tBEOptionDB::iLanguage, bits 22..24 of cBEOptionInterface_mOption's first word; names
// ours). [U] cSSXAudio_GetSpeechLanguage maps 0..3 to kSPEECHLANG_English, French, German and Spanish (anything else to English);
// cBENewPlayerInterface_getRiderName names an unnamed player PLAYER, JOUEUR, SPIELER or JUGADOR by it.
enum eOptionSpeechLanguage {
    kOPTLANG_English = 0, kOPTLANG_French = 1, kOPTLANG_German = 2, kOPTLANG_Spanish = 3
};

// iUseWidescreen's place in the option block's first word (bits 20-21), for code that tests the whole word (names
// ours; [U] tNISList_GetVideoDBRecord).
enum {
    kOptionWidescreenShift = 20,
    kOptionWidescreenMask = 3 << kOptionWidescreenShift
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
    unsigned int Flags;             // 0x0 [O] anonymous union { unsigned int Flags; bits } (DWARF union @5e7b8 in
                                    //     ssxd_dvd.prx; LSB first: FastRecovery, Level1Character, HalfPoints,
                                    //     StartFullBoost, NoUberTricks, LooseAdrenalin); [U] cAI_applyPlayerRulesCustom
                                    //     tests bits 0..5 with exactly those effects. [Renamed from unk_00 by lstruct L2.]
    int StartingPoints;             // 0x4 [O]
    int HeadStartSecs;              // 0x8 [O]; [U] cBERules_getPlayerStartDelaySecs
};

CHECK_OFFSET(tBEBraggingRights, Flags, 0x0);
CHECK_OFFSET(tBEBraggingRights, StartingPoints, 0x4);
CHECK_OFFSET(tBEBraggingRights, HeadStartSecs, 0x8);
CHECK_SIZE(tBEBraggingRights, 0xC);

// tBEMyRules: the rules that are not per player. [O] On Tour struct tBEMyRules (0x8): an anonymous union, then
// AIHeadStartSecs; [U] cBERules_getAIStartDelaySecs reads AIHeadStartSecs (retail D_005308D4 = cBERules_mRules + 0x1C).
struct tBEMyRules {
    unsigned int Flags;             // 0x0 [O] anonymous union { unsigned int Flags; bits } (DWARF union @5ea5c in
                                    //     ssxd_dvd.prx; LSB first: UnlimitedBoost, NoUberRails, SuperAI, NoBoost,
                                    //     KOBoostOnly, NoKnockDowns, NoIconMultiplier, NoIconPoints, NoIconPowerUp,
                                    //     NoCheckPoints); [U] the ai/ai OptBit helpers test its bits (view
                                    //     sOptView_12B498 f18). [Renamed from unk_00 by lstruct L2.]
    int AIHeadStartSecs;            // 0x4 [O]; [U] cBERules_getAIStartDelaySecs
};

CHECK_OFFSET(tBEMyRules, Flags, 0x0);
CHECK_OFFSET(tBEMyRules, AIHeadStartSecs, 0x4);
CHECK_SIZE(tBEMyRules, 0x8);

// The bit-field half of tBEMyRules::Flags' anonymous union ([O] On Tour union @5ea5c, LSB first; the struct name is
// ours), for code that sets single rules: ((tBEMyRulesBits*)&cBERules_mRules.MyRules.Flags)->NoIconPoints = 1, as
// tAttrThing names cUIThing::mAttr's bits. [U] cFEStateChallengeReceivedPopup_onCreateScreen sets bits 6-8 from the
// challenge (cChallenger mbMultipliers / mbPointIcons / mbPowerUps).
struct tBEMyRulesBits {
    unsigned int UnlimitedBoost : 1;   // bit 0 [O]
    unsigned int NoUberRails : 1;      // bit 1 [O]
    unsigned int SuperAI : 1;          // bit 2 [O]
    unsigned int NoBoost : 1;          // bit 3 [O]
    unsigned int KOBoostOnly : 1;      // bit 4 [O]
    unsigned int NoKnockDowns : 1;     // bit 5 [O]
    unsigned int NoIconMultiplier : 1; // bit 6 [O]
    unsigned int NoIconPoints : 1;     // bit 7 [O]
    unsigned int NoIconPowerUp : 1;    // bit 8 [O]
    unsigned int NoCheckPoints : 1;    // bit 9 [O]
};

CHECK_SIZE(tBEMyRulesBits, 0x4);

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
