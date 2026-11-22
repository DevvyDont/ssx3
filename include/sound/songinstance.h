#ifndef SOUND_SONGINSTANCE_H
#define SOUND_SONGINSTANCE_H

// cSongConfiguration and cSongInstance: cMusicSys's per-song data (sound/musicsys.h). cMusicSys's ctor reads one
// cSongConfiguration per section of data/config/music.inf into cMusicSys::mSongConfig[]; the playing song is a
// cSongInstance (cMusicSys::mSongInstance) that streams the song's interactive-music data ("Pathfinder") and its loop
// overlay. EA source d:/ssx/code/source/sound/musicsys.cpp (preview strings "SongInstance", "PathfinderStream",
// "PathfinderOverlay", "cSongConfiguration", "mSongConfigurations[mNumSongs]->mNumBankData < MUSIC_MAXLOOPFILES";
// preview vtable _vt$18cSongConfiguration). Retail methods: sound/bankmonitor unit 0x2B1A98..0x2B2850, just before
// cMusicSys's. On Tour has no music system: no [O] evidence.
// Sizes [R]: cMusicSys ctor cMusicSys_cMusicSys: cSongConfiguration_cSongConfiguration(cMemMan_alloc(0xAC, "cSongConfiguration"));
// func_002B1B98: func_002B1C50(cMemMan_alloc(0xC4, "SongInstance"), ...).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] constructors cSongConfiguration_cSongConfiguration / func_002B1C50, destructors cSongConfiguration__cSongConfiguration / func_002B1F78, the music.inf keys
//       cMusicSys's ctor parses into each cSongConfiguration (member names follow the key; EA's own names unknown);
//   [P] the preview's assert strings;
//   [U] how the retail methods use the field. Names marked "(our name)" describe that use; the rest are unk_.

#include "layout.h"
#include "sound/bankmonitor.h"

// cSongConfiguration (0xAC): one music.inf song section. vptr last (g++ 2.95: after the data).
struct cSongConfiguration {
    char mPathData[0x20];    // 0x00 [R] music.inf PATHDATA (strcpy); [U] cSongInstance ctor loads "<song path><PATHDATA>" as the MPF file (func_002B4A20)
    char mMusData[0x20];     // 0x20 [R] music.inf MUSDATA; [U] func_002B21E0 opens it as the main stream (0x11000001)
    int mNumBankData;        // 0x40 [P] "mSongConfigurations[mNumSongs]->mNumBankData < MUSIC_MAXLOOPFILES"; [R] ctor 0, LOOPDATA ++; [U] > 0 opens / closes the overlay stream (func_002B21E0, dtor func_002B1F78); func_002B24C0 % it
    char mBankData[1][0x20]; // 0x44 [P] MUSIC_MAXLOOPFILES = 1 (ctor's init loop runs once); [R] music.inf LOOPDATA; [U] func_002B21E0 opens it as the overlay stream (0x11000002)
    int mBeatsPerMeasure;    // 0x64 [R] music.inf BeatsPerMeasure (ctor 4); [U] func_002B26B8
    int mMeasuresPerBar;     // 0x68 [R] music.inf MeasuresPerBar (ctor 2); [U] func_002B26B8
    int mPhrasesPerBank;     // 0x6C [R] music.inf PhrasesPerBank (ctor 4); [U] func_002B2500
    int mBeatsPerPhrase;     // 0x70 [R] music.inf BeatsPerPhrase (ctor 8)
    int mPhraseAlign;        // 0x74 [R] music.inf PhraseAlign (ctor 16); [U] func_002B2500
    int mDelayCount;         // 0x78 [R] music.inf DelayCount (ctor 0)
    int mDelayFeedback;      // 0x7C [R] music.inf DelayFeedback (ctor 90)
    int mDelayTime;          // 0x80 [R] music.inf DelayTime (ctor 100)
    int mDelayLevel;         // 0x84 [R] music.inf DelayLevel (ctor 50)
    int mPathlevel;          // 0x88 [R] music.inf Pathlevel (ctor 100); [U] cSongInstance ctor / func_002B21E0 scale the volume by it / 100
    int mAsynclevel;         // 0x8C [R] music.inf Asynclevel (ctor 100)
    float mBPM;              // 0x90 [R] music.inf BPM (AssignFloat; ctor 120.0f); [U] func_002B2690: 60000 / mBPM ms per beat
    int mDuckToLoops;        // 0x94 [R] music.inf DUCKTOLOOPS (ctor 1)
    int mSedValue;           // 0x98 [R] music.inf SEDVALUE (ctor -1)
    int mLowPass;            // 0x9C [R] music.inf LOWPASS (ctor 0xFFFF); [U] func_002B21E0 -> func_002B2160 (low-pass cutoff, 0..0xFFFF)
    int mPreview;            // 0xA0 [R] music.inf PREVIEW (ctor -1)
    int mSongBig;            // 0xA4 [R] music.inf SONGBIG (ctor 1); [U] cSongInstance ctor: 1 = "audio/music.big", else "audio/music2.big"
    void** mVtbl;            // 0xA8 [R] D_00483980 (_vt$18cSongConfiguration: slot 1 = the dtor cSongConfiguration__cSongConfiguration)
};

// cSongInstance (0xC4): the playing song (cMusicSys::mSongInstance). No vtable.
struct cSongInstance {
    cSongConfiguration* mpConfig; // 0x00 (our name) [R] ctor arg 3; [U] every method reads the song's settings through it
    SNDPLAYOPTS mPlayOpts;        // 0x04 (our name) [R] ctor copies the current cSndPlayOpts level (mCurrent[mInst], sdl/sdr); [U] func_002B21E0 passes &mPlayOpts to the stream opens
    int mHandle;                   // 0x1C [P] assert "mHandle == -1" (musicsys.cpp:295, this+0x1C); [R] ctor -1; [U] func_002B21E0: main stream handle (func_003D25F0(0x11000001, ...))
    int unk_20;                   // 0x20 [U] func_002B21E0: overlay stream handle (func_003D25F0(0x11000002, ...))
    void* unk_24;                 // 0x24 [R] ctor operator new(0x37000, "PathfinderStream"); dtor cMemMan_free; [U] main stream buffer
    void* unk_28;                 // 0x28 [R] ctor operator new(func_003D3058(1, 0), "PathfinderOverlay"); dtor cMemMan_free; [U] overlay stream buffer (0x6A400)
    int unk_2C;                   // 0x2C [R] ctor 0; [U] func_002B21E0 sets 1 once the streams are open; func_002B20C8 tests it
    int unk_30;                   // 0x30 [R] ctor -1; [U] func_002B20C8 parks a value here until the streams open; func_002B21E0 applies and resets it
    int unk_34;                   // 0x34 [R] ctor arg 7; [U] a cBankManager bank index: func_002B27C0 checks BANK_Used / BANK_Handle of it, the dtor unloads it (cAsyncSys_ASYNCSYS_Cancel)
    int unk_38;                   // 0x38 [R] ctor 0
    float* unk_3C;                // 0x3C [R] ctor arg 6; [U] func_002B21E0: optional volume control (*unk_3C scales the volume)
    signed char unk_40;           // 0x40 [R] ctor arg 5 (sb); [U] func_002B21E0 lb: the base volume
    char unk_41[0x3];             // 0x41 padding
    int unk_44;                   // 0x44 [R] ctor = the play options' low-pass cutoff (SNDPLAYOPTS lowpasscutoff); [U] func_002B2160 caches the cutoff it set
    int unk_48;                   // 0x48 [R] ctor 0; [U] func_002B2418 1 (fade out started); func_002B21E0 stops updating the volume then
    int unk_4C;                   // 0x4C [R] ctor 0; [U] func_002B27C0: 0 = check the bank first
    int unk_50;                   // 0x50 [R] ctor 0
    int unk_54;                   // 0x54 [R] ctor 1; [U] func_002B21E0 / func_002B23E8 test it
    int unk_58;                   // 0x58 [R] ctor -1
    int unk_5C;                   // 0x5C [R] ctor 0; [U] func_002B20C8: when set, only a forced call applies
    char mPath[0x64];             // 0x60 (our name) [R] ctor sprintf("%s%s%s", "data/", "audio/music.big" or "audio/music2.big", name); [U] func_002B21E0 prefixes the stream file names with it
};

CHECK_OFFSET(cSongConfiguration, mPathData, 0x00);
CHECK_OFFSET(cSongConfiguration, mMusData, 0x20);
CHECK_OFFSET(cSongConfiguration, mNumBankData, 0x40);
CHECK_OFFSET(cSongConfiguration, mBankData, 0x44);
CHECK_OFFSET(cSongConfiguration, mBeatsPerMeasure, 0x64);
CHECK_OFFSET(cSongConfiguration, mMeasuresPerBar, 0x68);
CHECK_OFFSET(cSongConfiguration, mPhrasesPerBank, 0x6C);
CHECK_OFFSET(cSongConfiguration, mBeatsPerPhrase, 0x70);
CHECK_OFFSET(cSongConfiguration, mPhraseAlign, 0x74);
CHECK_OFFSET(cSongConfiguration, mDelayCount, 0x78);
CHECK_OFFSET(cSongConfiguration, mDelayFeedback, 0x7C);
CHECK_OFFSET(cSongConfiguration, mDelayTime, 0x80);
CHECK_OFFSET(cSongConfiguration, mDelayLevel, 0x84);
CHECK_OFFSET(cSongConfiguration, mPathlevel, 0x88);
CHECK_OFFSET(cSongConfiguration, mAsynclevel, 0x8C);
CHECK_OFFSET(cSongConfiguration, mBPM, 0x90);
CHECK_OFFSET(cSongConfiguration, mDuckToLoops, 0x94);
CHECK_OFFSET(cSongConfiguration, mSedValue, 0x98);
CHECK_OFFSET(cSongConfiguration, mLowPass, 0x9C);
CHECK_OFFSET(cSongConfiguration, mPreview, 0xA0);
CHECK_OFFSET(cSongConfiguration, mSongBig, 0xA4);
CHECK_OFFSET(cSongConfiguration, mVtbl, 0xA8);
CHECK_SIZE(cSongConfiguration, 0xAC);
CHECK_OFFSET(cSongInstance, mpConfig, 0x00);
CHECK_OFFSET(cSongInstance, mPlayOpts, 0x04);
CHECK_OFFSET(cSongInstance, mHandle, 0x1C);
CHECK_OFFSET(cSongInstance, unk_20, 0x20);
CHECK_OFFSET(cSongInstance, unk_24, 0x24);
CHECK_OFFSET(cSongInstance, unk_28, 0x28);
CHECK_OFFSET(cSongInstance, unk_2C, 0x2C);
CHECK_OFFSET(cSongInstance, unk_30, 0x30);
CHECK_OFFSET(cSongInstance, unk_34, 0x34);
CHECK_OFFSET(cSongInstance, unk_38, 0x38);
CHECK_OFFSET(cSongInstance, unk_3C, 0x3C);
CHECK_OFFSET(cSongInstance, unk_40, 0x40);
CHECK_OFFSET(cSongInstance, unk_41, 0x41);
CHECK_OFFSET(cSongInstance, unk_44, 0x44);
CHECK_OFFSET(cSongInstance, unk_48, 0x48);
CHECK_OFFSET(cSongInstance, unk_4C, 0x4C);
CHECK_OFFSET(cSongInstance, unk_50, 0x50);
CHECK_OFFSET(cSongInstance, unk_54, 0x54);
CHECK_OFFSET(cSongInstance, unk_58, 0x58);
CHECK_OFFSET(cSongInstance, unk_5C, 0x5C);
CHECK_OFFSET(cSongInstance, mPath, 0x60);
CHECK_SIZE(cSongInstance, 0xC4);

#endif /* SOUND_SONGINSTANCE_H */
