#ifndef SOUND_MUSICSYS_H
#define SOUND_MUSICSYS_H

// cMusicSys: the interactive music system (song list from the music .ini, the active-song playlist, the current song's
// streams and loops). EA source d:/ssx/code/source/sound/musicsys.cpp (preview assert strings, with
// "gMusicSys->GetMPFFile() != NULL"); retail methods are the tail of the sound/bankmonitor unit (0x2B28C0..0x2B4B04)
// plus out-of-line inline accessors in bx/seg_1BA100 (0x2BAC30..0x2BACEC).
// One instance: the cMusicSys base of cController, i.e. gSSXAudio + 0x118 (sound/ssxaudio.h), built by the cController
// ctor cMusicSys_cMusicSys(this + 0x118, 0, ...); gMusicSys (gMusicSys) points at it.
//   cMusicSys : virtual cStreamSys (: virtual cBankManager : cAsyncSys, cBankMonitor, virtual cSoundSys)
// Non-virtual part 0x5448 [R]: in a complete cMusicSys the ctor (inchrg != 0) puts cSoundSys at +0x5448, cBankManager
// at +0x5450 and cStreamSys at +0x5F20 (and the vptr at +0x5440 is the last non-virtual field); in cSSXAudio the next
// base (cSpeechSys) starts at 0x118 + 0x5448 = 0x5560. Vtable cMusicSys_vtbl (cMusicSys / cController),
// D_00483688 (cSSXAudio). The struct below is the NON-VIRTUAL part, so it can be embedded where the object lives.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] retail constructor cMusicSys_cMusicSys (zeroes the fields, allocates "MPF data file", parses the music .ini: section
//       GLOBAL key BASEPATH, then one section per song with keys TITLE ARTIST ALBUM ADDTOFE CATEGORY PATHDATA MUSDATA
//       LOOPDATA ... into a cMemMan_alloc(0xAC, "cSongConfiguration") per song) and destructor cMusicSys__cMusicSys;
//   [P] the July 2003 preview's inline accessors (MUSIC_*, IsSongPlaying, Get/SetShortcut), retail copies at
//       0x2BAC30..0x2BACE8 (preview and retail offsets agree up to 0x418; tools/layout_xlate.py: the tail moved -0x104);
//   [U] how retail code uses the field.
// On Tour has no cMusicSys (its audio was restructured), so there is no [O] evidence. Member names are EA's accessor
// names where [P] gives them; the others are named after the .ini key or allocation tag that fills them, or unk_.

#include "layout.h"

struct cStreamSys;
struct cSongConfiguration;
struct cSongInstance;  // sound/songinstance.h

// The tag the audio code puts on the song it starts (cSongInstance::mSongTag; cMusicSys_SetSongTag sets it, cMusicSys_GetSongTag
// returns it). Names ours, after the song each tag marks: [U] cSSXAudio_StartLocationMusic tags the peak themes "Peak1" .. "Peak3" 1-3
// and the peak ambiences "Peak1Amb" .. "Peak3Amb" 101-103, cSSXAudio_PlayCharTune the podium tune "chartune" 201, cSSXAudio_PlayCharSelSong
// and cSSXAudio_FrontEndLoad "charsel" 301, cSSXAudio_PlayPkTransSong and cSSXAudio_HandleMusicEvent the peak transition "pktrans" 401; [R] the
// cSongInstance ctor stores -1, and cMusicSys_GetSongTag returns -1 with no song playing.
enum eSongTag {
    kSONGTAG_None = -1,
    kSONGTAG_Peak1 = 1, kSONGTAG_Peak2 = 2, kSONGTAG_Peak3 = 3,
    kSONGTAG_Peak1Amb = 101, kSONGTAG_Peak2Amb = 102, kSONGTAG_Peak3Amb = 103,
    kSONGTAG_CharTune = 201,
    kSONGTAG_CharSel = 301,
    kSONGTAG_PkTrans = 401
};

// A song without a SEDVALUE (cSongConfiguration::mSedValue; cMusicSys_GetCurrentSongSedValue returns the current song's). Name ours.
// [R] the cSongConfiguration ctor stores -1 and only a music.inf SEDVALUE key replaces it.
enum { kNoSedValue = -1 };

struct cSongInstance;

struct cMusicSys {
    cStreamSys* mpVStreamSys;                 // 0x0000 [R] g++ 2.95 virtual-base pointer to cStreamSys (= this + 0x5F20 in a complete object; cSSXAudio + 0x7768); [U] every method reaches cBankManager as **(this) and its cBankMonitor as **(this) + 0x1D8
    cSongConfiguration* mSongConfig[64];      // 0x0004 [R] one cMemMan_alloc(0xAC, "cSongConfiguration") + ctor cSongConfiguration_cSongConfiguration per song (PATHDATA/MUSDATA/LOOPDATA/BeatsPerMeasure.. go into it); dtor deletes each through its vtable (+0xA8); [U] cMusicSys_StartSong, cMusicSys_GetCurrentSongSedValue__FPv, cMusicSys_PreviewSong
    char mBasePath[0x20];                     // 0x0104 [R] ini GLOBAL/BASEPATH; [U] cMusicSys_StartSong passes it to the song instance ctor
    char mSongName[64][11];                   // 0x0124 [P] MUSIC_SongName returns this + 0x124 + 11 * mCurrentSongNum; [R] ctor copies each song's section name; [U] cMusicSys_StartSong/cMusicSys_AddActiveSongByName search it
    int mNumSongs;                            // 0x03E4 [P] MUSIC_GetNumSongs (retail 0x2BAC60); [R] ctor counts the songs
    int mNumActiveSongs;                      // 0x03E8 [P] MUSIC_GetNumActiveSongs (retail 0x2BAC68); [U] cMusicSys_ClearActiveSongs 0, cMusicSys_AddActiveSongByName/cMusicSys_AddActiveSong ++ (max 64)
    int mNumFESongs;                          // 0x03EC [P] MUSIC_GetNumFESongs (retail 0x2BAC70); [R] ctor counts ADDTOFE = 1; [U] cMusicSys_SetActiveSongs loop bound
    int mCurrentSongNum;                      // 0x03F0 [P] MUSIC_GetCurrentSongNum (retail 0x2BAC90); retail setter 0x2BACE0; [U] cMusicSys_PrevActiveSong/cMusicSys_NextActiveSong step it through mActiveMap
    char unk_3F4[0x4];                        // 0x03F4 padding (mActiveMap is 8-aligned)
    unsigned long mActiveMap;                 // 0x03F8 PORT: 64-bit long on the EE. [P] MUSIC_StoreDefaultActiveMap (ld); [R] ctor sets bit n for song n; [U] cMusicSys_AddActiveSongByName/cMusicSys_AddActiveSong/cMusicSys_getActiveMap/cMusicSys_IsCurrentSongActive
    unsigned long mDefaultActiveMap;          // 0x0400 [P] MUSIC_StoreDefaultActiveMap (sd) / MUSIC_GetDefaultActiveMap (retail 0x2BACD8)
    cSongInstance* mSongInstance;                  // 0x0408 [P] assert "mSongInstance" (musicsys.cpp:1487, this+0x408); MUSIC_Current (retail 0x2BAC30), MUSIC_GetStreamHandle, MUSIC_SetDelay; [U] cMusicSys_StartSong: cSongInstance_Create = cMemMan_alloc(0xC4, "SongInstance") + ctor cSongInstance_cSongInstance; dtor cSongInstance::~cSongInstance
    float mFadeStep;                          // 0x040C (our name) [U] cMusicSys_FadeBaseVolume: volume fade step per tick;
                                              //        cMusicSys_StepBaseVolumeFade (monitor callback) adds it
    float mFadeVolume;                        // 0x0410 (our name) [U] cMusicSys_FadeBaseVolume/cMusicSys_StepBaseVolumeFade: current faded volume
    int mFadeTarget;                          // 0x0414 (our name) [U] cMusicSys_FadeBaseVolume/cMusicSys_StepBaseVolumeFade: fade target volume
    int mSongPlaying;                         // 0x0418 [P] IsSongPlaying (retail 0x2BACB8); retail 0x2BACC0 zeroes it; [U] cMusicSys_StartSong 1, cMusicSys_StopSong 0; tested by most MUSIC_ wrappers
    int mMusicState;                          // 0x041C (our name) [U] the current music state: cMusicSys_StartSong -1;
                                              //        cMusicSys_SetMusicState (virtual slot 4) compares / stores its argument;
                                              //        retail accessor 0x2BACE8
    int mCategory[64][5];                     // 0x0420 [R] ctor zeroes; ini CATEGORY n (< 5) sets mCategory[song][n] = 1; [U] cMusicSys_PrevActiveSong/cMusicSys_NextActiveSong filter by it
    int mCategoryCount[5];                    // 0x0920 [R] ctor zeroes; [U] cMusicSys_AddActiveSongByName/cMusicSys_AddActiveSong count active songs per category, cMusicSys_ClearActiveSongs resets
    char mDisplaySongName[64][100];                     // 0x0934 [P] assert strlen(mDisplaySongName[...]) (musicsys.cpp, preview +0xA38, -0x104); [R] ini TITLE; [U] cMusicSys_GetSongTitle returns [i or mCurrentSongNum]
    char mDisplaySongArtist[64][100];                    // 0x2234 [P] assert strlen(mDisplaySongArtist[...]) (preview +0x2338); [R] ini ARTIST; [U] cMusicSys_GetSongArtist
    char mDisplaySongAlbum[64][100];                     // 0x3B34 [P] assert strlen(mDisplaySongAlbum[...]) (preview +0x3C38); [R] ini ALBUM; [U] cMusicSys_GetSongAlbum
    char* mMPFFile;                           // 0x5434 [R] operator new[](0x6400, "MPF data file"), freed by the dtor; preview assert "gMusicSys->GetMPFFile() != NULL"; [U] cMusicSys_GetMPFFile__FPv returns it, cMusicSys_LoadMPFFile loads into it
    void* mpMpfFile;                          // 0x5438 (our name) [R] ctor 0; [U] cMusicSys_LoadMPFFile: async file handle of
                                              //        the MPF load (ASYNCFILE_cancel/release), cMusicSys_IsMPFLoaded polls it
    int mShortcut;                            // 0x543C [R] ctor 0; [P] SetShortcut / GetOnShortcut (preview 0x5540, bracketed 0x5434..0x5440)
    void** mVtbl;                             // 0x5440 [R] cMusicSys vptr: cMusicSys_vtbl (cMusicSys ctor/dtor), D_00483688 in cSSXAudio; [U] cMusicSys_SetMusicState/cMusicSys_OverlayBeatCallback/cMusicSys_PreviewSong call through it
    char unk_5444[0x4];                       // 0x5444 padding (the class is 8-aligned: unsigned long members)

    // Methods bound to their retail symbols by asm labels.
    cSongInstance* MUSIC_Current() __asm__("cMusicSys_MUSIC_Current");
    int MUSIC_GetStreamHandle() __asm__("cMusicSys_MUSIC_GetStreamHandle");
    int MUSIC_GetNumSongs() __asm__("cMusicSys_MUSIC_GetNumSongs");
    int MUSIC_GetNumActiveSongs() __asm__("cMusicSys_MUSIC_GetNumActiveSongs");
    int MUSIC_GetNumFESongs() __asm__("cMusicSys_MUSIC_GetNumFESongs");
    char* MUSIC_SongName() __asm__("cMusicSys_MUSIC_SongName");
    int MUSIC_GetCurrentSongNum() __asm__("cMusicSys_MUSIC_GetCurrentSongNum");
    void MUSIC_SetDelay() __asm__("cMusicSys_MUSIC_SetDelay");
    int IsSongPlaying() __asm__("cMusicSys_IsSongPlaying");
    void MUSIC_StoreDefaultActiveMap() __asm__("cMusicSys_MUSIC_StoreDefaultActiveMap");
    long MUSIC_GetDefaultActiveMap() __asm__("cMusicSys_MUSIC_GetDefaultActiveMap");
    void setCurrentSongNum(int songNum) __asm__("cMusicSys_setCurrentSongNum");
    long getActiveMap() __asm__("cMusicSys_getActiveMap");
    int GetMPFFile() __asm__("cMusicSys_GetMPFFile__FPv");
    void clearSongPlaying() __asm__("cMusicSys_clearSongPlaying");
    int getMusicState() __asm__("cMusicSys_getMusicState");
    void PlaySong(int songName, int bankIndex, int volumeControl, int syncLoad) __asm__("cMusicSys_PlaySong");
    void StartSong(int songName, int bankIndex, int syncLoad) __asm__("cMusicSys_StartSong");
    void PauseSong() __asm__("cMusicSys_PauseSong");
    void ResumeSong() __asm__("cMusicSys_ResumeSong");
    void StopSong() __asm__("cMusicSys_StopSong");
    void SendMusicEvent(int musicEvent) __asm__("cMusicSys_SendMusicEvent");
    int SetMusicState(int musicState) __asm__("cMusicSys_SetMusicState");
    void SetVolume() __asm__("cMusicSys_SetVolume");
    void SetBaseVolume(signed char baseVol) __asm__("cMusicSys_SetBaseVolume");
    void SetVolumeEnabled(int enabled) __asm__("cMusicSys_SetVolumeEnabled");
    void SetLowPass(int cutoff) __asm__("cMusicSys_SetLowPass");
    void SetOverlayLevel(signed char levelPercent) __asm__("cMusicSys_SetOverlayLevel");
    void FadeOut(float fadeTime) __asm__("cMusicSys_FadeOut");
    void FadeBaseVolume(signed char target, float duration) __asm__("cMusicSys_FadeBaseVolume");
    bool StepBaseVolumeFade() __asm__("cMusicSys_StepBaseVolumeFade");
    void ClearActiveSongs() __asm__("cMusicSys_ClearActiveSongs");
    void AddActiveSongByName(const char* name) __asm__("cMusicSys_AddActiveSongByName");
    void AddActiveSong(int idx) __asm__("cMusicSys_AddActiveSong");
    void SetActiveSongs(ulong mask) __asm__("cMusicSys_SetActiveSongs");
    void SelectActiveSong(int n) __asm__("cMusicSys_SelectActiveSong");
    void UpdateSong() __asm__("cMusicSys_UpdateSong");
    void LoadRandomLoopBank() __asm__("cMusicSys_LoadRandomLoopBank");
    void ArmOverlay() __asm__("cMusicSys_ArmOverlay");
    void CancelOverlay() __asm__("cMusicSys_CancelOverlay");
    void OverlayBeatCallback(int monitorInst) __asm__("cMusicSys_OverlayBeatCallback");
    void ArmBeatCallback(cSongInstance* ent) __asm__("cMusicSys_ArmBeatCallback");
    int StartOverlay() __asm__("cMusicSys_StartOverlay");
    void RequestOverlay() __asm__("cMusicSys_RequestOverlay");
    void* IsOverlayArmed() __asm__("cMusicSys_IsOverlayArmed");
    void ClearOverlayArmed() __asm__("cMusicSys_ClearOverlayArmed");
    int GetBaseVolume() __asm__("cMusicSys_GetBaseVolume");
    bool WaitLoopBankCallback() __asm__("cMusicSys_WaitLoopBankCallback");
    bool CheckLoopBankCallback() __asm__("cMusicSys_CheckLoopBankCallback");
    int GetDuckToLoops() __asm__("cMusicSys_GetDuckToLoops");
    int GetSedValue() __asm__("cMusicSys_GetSedValue");
    int GetCurrentSongSedValue() __asm__("cMusicSys_GetCurrentSongSedValue__FPv");
    float GetTimeToNextBeat() __asm__("cMusicSys_GetTimeToNextBeat");
    void SetSongTag(int songTag) __asm__("cMusicSys_SetSongTag");
    int GetSongTag() __asm__("cMusicSys_GetSongTag");
    void SetHoldEvents(int holdEvents) __asm__("cMusicSys_SetHoldEvents");
    void LoadMPFFile(void* name, int sync) __asm__("cMusicSys_LoadMPFFile");
    bool IsMPFLoaded(int force) __asm__("cMusicSys_IsMPFLoaded");
    ulong IsCurrentSongActive() __asm__("cMusicSys_IsCurrentSongActive");
};

CHECK_OFFSET(cMusicSys, mpVStreamSys, 0x0000);
CHECK_OFFSET(cMusicSys, mSongConfig, 0x0004);
CHECK_OFFSET(cMusicSys, mBasePath, 0x0104);
CHECK_OFFSET(cMusicSys, mSongName, 0x0124);
CHECK_OFFSET(cMusicSys, mNumSongs, 0x03E4);
CHECK_OFFSET(cMusicSys, mNumActiveSongs, 0x03E8);
CHECK_OFFSET(cMusicSys, mNumFESongs, 0x03EC);
CHECK_OFFSET(cMusicSys, mCurrentSongNum, 0x03F0);
CHECK_OFFSET(cMusicSys, unk_3F4, 0x03F4);
CHECK_OFFSET(cMusicSys, mActiveMap, 0x03F8);
CHECK_OFFSET(cMusicSys, mDefaultActiveMap, 0x0400);
CHECK_OFFSET(cMusicSys, mSongInstance, 0x0408);
CHECK_OFFSET(cMusicSys, mFadeStep, 0x040C);
CHECK_OFFSET(cMusicSys, mFadeVolume, 0x0410);
CHECK_OFFSET(cMusicSys, mFadeTarget, 0x0414);
CHECK_OFFSET(cMusicSys, mSongPlaying, 0x0418);
CHECK_OFFSET(cMusicSys, mMusicState, 0x041C);
CHECK_OFFSET(cMusicSys, mCategory, 0x0420);
CHECK_OFFSET(cMusicSys, mCategoryCount, 0x0920);
CHECK_OFFSET(cMusicSys, mDisplaySongName, 0x0934);
CHECK_OFFSET(cMusicSys, mDisplaySongArtist, 0x2234);
CHECK_OFFSET(cMusicSys, mDisplaySongAlbum, 0x3B34);
CHECK_OFFSET(cMusicSys, mMPFFile, 0x5434);
CHECK_OFFSET(cMusicSys, mpMpfFile, 0x5438);
CHECK_OFFSET(cMusicSys, mShortcut, 0x543C);
CHECK_OFFSET(cMusicSys, mVtbl, 0x5440);
CHECK_OFFSET(cMusicSys, unk_5444, 0x5444);
CHECK_SIZE(cMusicSys, 0x5448);

// The music system inside the audio system (cSSXAudio::MusicSys): its constructor stores itself here (preview
// assert "gMusicSys->GetMPFFile() != NULL"), its destructor clears it.
extern cMusicSys* gMusicSys;

#endif /* SOUND_MUSICSYS_H */
