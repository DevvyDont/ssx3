#ifndef SOUND_MUSICSYS_H
#define SOUND_MUSICSYS_H

// cMusicSys: the interactive music system (song list from the music .ini, the active-song playlist, the current song's
// streams and loops). EA source d:/ssx/code/source/sound/musicsys.cpp (preview assert strings, with
// "gMusicSys->GetMPFFile() != NULL"); retail methods are the tail of the sound/bankmonitor unit (0x2B28C0..0x2B4B04)
// plus out-of-line inline accessors in bx/seg_1BA100 (0x2BAC30..0x2BACEC).
// One instance: the cMusicSys base of cController, i.e. gSSXAudio + 0x118 (sound/ssxaudio.h), built by the cController
// ctor func_002B28C0(this + 0x118, 0, ...); gMusicSys (gMusicSys) points at it.
//   cMusicSys : virtual cStreamSys (: virtual cBankManager : cAsyncSys, cBankMonitor, virtual cSoundSys)
// Non-virtual part 0x5448 [R]: in a complete cMusicSys the ctor (inchrg != 0) puts cSoundSys at +0x5448, cBankManager
// at +0x5450 and cStreamSys at +0x5F20 (and the vptr at +0x5440 is the last non-virtual field); in cSSXAudio the next
// base (cSpeechSys) starts at 0x118 + 0x5448 = 0x5560. Vtable D_004838F8 (cMusicSys / cController),
// D_00483688 (cSSXAudio). The struct below is the NON-VIRTUAL part, so it can be embedded where the object lives.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] retail constructor func_002B28C0 (zeroes the fields, allocates "MPF data file", parses the music .ini: section
//       GLOBAL key BASEPATH, then one section per song with keys TITLE ARTIST ALBUM ADDTOFE CATEGORY PATHDATA MUSDATA
//       LOOPDATA ... into a cMemMan_alloc(0xAC, "cSongConfiguration") per song) and destructor func_002B3398;
//   [P] the July 2003 preview's inline accessors (MUSIC_*, IsSongPlaying, Get/SetShortcut), retail copies at
//       0x2BAC30..0x2BACE8 (preview and retail offsets agree up to 0x418; tools/layout_xlate.py: the tail moved -0x104);
//   [U] how retail code uses the field.
// On Tour has no cMusicSys (its audio was restructured), so there is no [O] evidence. Member names are EA's accessor
// names where [P] gives them; the others are named after the .ini key or allocation tag that fills them, or unk_.

#include "layout.h"

struct cStreamSys;
struct cSongConfiguration;

struct cMusicSys {
    cStreamSys* mpVStreamSys;                 // 0x0000 [R] g++ 2.95 virtual-base pointer to cStreamSys (= this + 0x5F20 in a complete object; cSSXAudio + 0x7768); [U] every method reaches cBankManager as **(this) and its cBankMonitor as **(this) + 0x1D8
    cSongConfiguration* mSongConfig[64];      // 0x0004 [R] one cMemMan_alloc(0xAC, "cSongConfiguration") + ctor cSongConfiguration_cSongConfiguration per song (PATHDATA/MUSDATA/LOOPDATA/BeatsPerMeasure.. go into it); dtor deletes each through its vtable (+0xA8); [U] func_002B3838, func_002B4908__FPv, func_002B4978
    char mBasePath[0x20];                     // 0x0104 [R] ini GLOBAL/BASEPATH; [U] func_002B3838 passes it to the song instance ctor
    char mSongName[64][11];                   // 0x0124 [P] MUSIC_SongName returns this + 0x124 + 11 * mCurrentSongNum; [R] ctor copies each song's section name; [U] func_002B3838/func_002B3EE8 search it
    int mNumSongs;                            // 0x03E4 [P] MUSIC_GetNumSongs (retail 0x2BAC60); [R] ctor counts the songs
    int mNumActiveSongs;                      // 0x03E8 [P] MUSIC_GetNumActiveSongs (retail 0x2BAC68); [U] func_002B3EB0 0, func_002B3EE8/func_002B3FE8 ++ (max 64)
    int mNumFESongs;                          // 0x03EC [P] MUSIC_GetNumFESongs (retail 0x2BAC70); [R] ctor counts ADDTOFE = 1; [U] func_002B4070 loop bound
    int mCurrentSongNum;                      // 0x03F0 [P] MUSIC_GetCurrentSongNum (retail 0x2BAC90); retail setter 0x2BACE0; [U] func_002B41E8/func_002B42B8 step it through mActiveMap
    char unk_3F4[0x4];                        // 0x03F4 padding (mActiveMap is 8-aligned)
    unsigned long mActiveMap;                 // 0x03F8 PORT: 64-bit long on the EE. [P] MUSIC_StoreDefaultActiveMap (ld); [R] ctor sets bit n for song n; [U] func_002B3EE8/func_002B3FE8/func_002B4068/func_002B4AF0
    unsigned long mDefaultActiveMap;          // 0x0400 [P] MUSIC_StoreDefaultActiveMap (sd) / MUSIC_GetDefaultActiveMap (retail 0x2BACD8)
    void* mCurrent;                           // 0x0408 [P] MUSIC_Current (retail 0x2BAC30), MUSIC_GetStreamHandle, MUSIC_SetDelay; [U] func_002B3838: func_002B1B98 = cMemMan_alloc(0xC4, "SongInstance") + ctor func_002B1C50; dtor func_002B1F78
    float unk_40C;                            // 0x040C [U] func_002B3D80: volume fade step per tick; func_002B3E20 (monitor callback) adds it
    float unk_410;                            // 0x0410 [U] func_002B3D80/func_002B3E20: current faded volume
    int unk_414;                              // 0x0414 [U] func_002B3D80/func_002B3E20: fade target volume
    int mSongPlaying;                         // 0x0418 [P] IsSongPlaying (retail 0x2BACB8); retail 0x2BACC0 zeroes it; [U] func_002B3838 1, func_002B3AC0 0; tested by most MUSIC_ wrappers
    int unk_41C;                              // 0x041C [U] func_002B3838 -1; func_002B3BC0 compares/stores its argument; retail accessor 0x2BACE8
    int mCategory[64][5];                     // 0x0420 [R] ctor zeroes; ini CATEGORY n (< 5) sets mCategory[song][n] = 1; [U] func_002B41E8/func_002B42B8 filter by it
    int mCategoryCount[5];                    // 0x0920 [R] ctor zeroes; [U] func_002B3EE8/func_002B3FE8 count active songs per category, func_002B3EB0 resets
    char mTitle[64][100];                     // 0x0934 [R] ini TITLE; [U] func_002B40F0 returns mTitle[i or mCurrentSongNum]
    char mArtist[64][100];                    // 0x2234 [R] ini ARTIST; [U] func_002B4120
    char mAlbum[64][100];                     // 0x3B34 [R] ini ALBUM; [U] func_002B4150
    char* mMPFFile;                           // 0x5434 [R] operator new[](0x6400, "MPF data file"), freed by the dtor; preview assert "gMusicSys->GetMPFFile() != NULL"; [U] func_002B4AE8__FPv returns it, func_002B4A20 loads into it
    void* unk_5438;                           // 0x5438 [R] ctor 0; [U] func_002B4A20: async file handle of the MPF load (ASYNCFILE_cancel/release), func_002B4AA0 polls it
    int mShortcut;                            // 0x543C [R] ctor 0; [P] SetShortcut / GetOnShortcut (preview 0x5540, bracketed 0x5434..0x5440)
    void** mVtbl;                             // 0x5440 [R] cMusicSys vptr: D_004838F8 (cMusicSys ctor/dtor), D_00483688 in cSSXAudio; [U] func_002B3BC0/func_002B4540/func_002B4978 call through it
    char unk_5444[0x4];                       // 0x5444 padding (the class is 8-aligned: unsigned long members)
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
CHECK_OFFSET(cMusicSys, mCurrent, 0x0408);
CHECK_OFFSET(cMusicSys, unk_40C, 0x040C);
CHECK_OFFSET(cMusicSys, unk_410, 0x0410);
CHECK_OFFSET(cMusicSys, unk_414, 0x0414);
CHECK_OFFSET(cMusicSys, mSongPlaying, 0x0418);
CHECK_OFFSET(cMusicSys, unk_41C, 0x041C);
CHECK_OFFSET(cMusicSys, mCategory, 0x0420);
CHECK_OFFSET(cMusicSys, mCategoryCount, 0x0920);
CHECK_OFFSET(cMusicSys, mTitle, 0x0934);
CHECK_OFFSET(cMusicSys, mArtist, 0x2234);
CHECK_OFFSET(cMusicSys, mAlbum, 0x3B34);
CHECK_OFFSET(cMusicSys, mMPFFile, 0x5434);
CHECK_OFFSET(cMusicSys, unk_5438, 0x5438);
CHECK_OFFSET(cMusicSys, mShortcut, 0x543C);
CHECK_OFFSET(cMusicSys, mVtbl, 0x5440);
CHECK_OFFSET(cMusicSys, unk_5444, 0x5444);
CHECK_SIZE(cMusicSys, 0x5448);

// The music system inside the audio system (cSSXAudio::MusicSys): its constructor stores itself here (preview
// assert "gMusicSys->GetMPFFile() != NULL"), its destructor clears it.
extern cMusicSys* gMusicSys;

#endif /* SOUND_MUSICSYS_H */
