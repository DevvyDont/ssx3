#ifndef MOVIE_MOVIEPLAYER_H
#define MOVIE_MOVIEPLAYER_H

// cMoviePlayer and tMoviePlayOptions: plays an FMV (EA's REAL decoder plus a PS2 video driver) either to the end
// (syncPlay) or a frame at a time from the caller's loop (asyncPlay / asyncUpdate / asyncRender / asyncPause /
// asyncResume). EA source d:/ssx/code/source/movie/movieplayer.cpp (the July 2003 preview's assert strings
// "mpCurrFrame" and "((int(pMem)+headersize)&63)==0", probably the allocator callback's alignment check); retail
// methods tMoviePlayOptions_SetDefaults (0x253390) and cMoviePlayer_cMoviePlayer .. cMoviePlayer_SSXFreeMemFunc
// (0x2533C8..0x253B18), which splat put in the unit mem/memstd. No vtable, no base.
//
// Size 0x44 [R]: cVidEngine_ReadyVideo = cMoviePlayer_cMoviePlayer(cMemMan_alloc(68, "VidEnginePlayer", 0x100, 0));
// cRealFE_update the same with "MoviePlayer"; cFEStateVideoPlayer keeps one too. On Tour's cMoviePlayer (0x30) is the
// PSP one (PspAvPlayer, byte bools, a 0x18-byte tMoviePlayOptions): retail has its member order with 4-byte bools,
// which places every retail access exactly. cMoviePlayer::m_isPlaying [O] (On Tour static int) is
// cMoviePlayer_m_isPlaying (retail 0x4A2E9C):
// startPlay increments it, purge decrements it when it deletes the video driver.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructors cMoviePlayer_cMoviePlayer / tMoviePlayOptions_SetDefaults and the allocations above;
//   [O] SSX On Tour's DWARF (class cMoviePlayer, struct tMoviePlayOptions): member order and names;
//   [U] how the retail methods (and their callers) use the field.

#include "layout.h"

struct RCMP_VIDEO_DRIVER; // movie/videodriver.h
struct RCMP_AV_PLAYER;    // ealib/avplayer.h

struct cMoviePlayer;

// tMoviePlayOptions (0x24 [R] tMoviePlayOptions_SetDefaults writes 0x0..0x20; callers keep it on the stack; [O] On Tour
// 0x18 with byte bools).
struct tMoviePlayOptions {
    const char* sVideoFileName; // 0x00 [O]; [R] SetDefaults 0; [U] callers store the movie file (cVidEngine_ReadyVideo,
                                //       cRealFE_update); cMoviePlayer_init passes it to the decoder
    const char* sAudioFileName; // 0x04 [O]; [R] SetDefaults 0; [U] cRealFE_update sets it for the intro movie;
                                //       cMoviePlayer_init builds an audio decoder when it is non-empty
    int bSoundOn;               // 0x08 [O] (On Tour bool); [R] SetDefaults 0; [U] callers set 1; cMoviePlayer_init
                                //       passes !bSoundOn to the decoder
    int bFullScreen;            // 0x0C [O] (On Tour bool); [R] SetDefaults 0; [U] cVidEngine_ReadyVideo 0; no retail
                                //       read seen
    int bPreLoad;               // 0x10 [O] (On Tour bool); [R] SetDefaults 0; [U] cMoviePlayer_init passes (bPreLoad !=
                                //       0) to the decoder
    int bSoftware;              // 0x14 [O] (On Tour bool); [R] SetDefaults 0; no retail read seen
    int iMemType;               // 0x18 [O]; [R] SetDefaults 0; [U] cVidEngine_ReadyVideo 0x100; no retail read seen
    int iDrawLayer;             // 0x1C [O]; [R] SetDefaults 31; [U] cVidEngine_ReadyVideo 9; startPlay passes it to
                                //       SSX_VD_create
    float fPlaySpeed;           // 0x20 [O]; [R] SetDefaults 1.0f; [U] the ctor and init copy it to m_fSpeed
};

struct cMoviePlayer {
    int mbMovieFinished;        // 0x00 [O] (On Tour bool); [U] startPlay / purge 0; syncPlay / asyncUpdate store
                                //       func_003AE860's "finished"; asyncRender / asyncPause / asyncResume test it;
                                //       cRealFE_update reads it (isMovieFinished)
    int mbReDraw;               // 0x04 [O] (On Tour bool); [U] startPlay / purge 0; asyncUpdate / syncPlay set it when
                                //       no new frame was decoded; asyncRender / syncPlay redraw (driver slot 5) when
                                //       set
    int mbPaused;               // 0x08 [O] (On Tour bool); [R] ctor 0; [U] asyncPause 1, asyncResume 0, purge 0;
                                //       asyncRender redraws while set
    void* mpCurrFrame;          // 0x0C [O] FRAME*; [R] ctor 0; [U] startPlay / asyncUpdate / syncPlay store the
                                //       decoder's frame (func_003AE450 / func_003AE6C8), asyncRender draws it, purge
                                //       releases it (RCMP_DECODER_ReleaseFrame); preview assert "mpCurrFrame"
    float m_fSpeed;             // 0x10 [O]; [R] ctor = m_PlayOp.fPlaySpeed; [U] cMoviePlayer_init copies it again
    tMoviePlayOptions m_PlayOp; // 0x14 [O]; [R] ctor tMoviePlayOptions_SetDefaults(this + 0x14); [U] cMoviePlayer_init
                                //       copies the caller's options (0x24 bytes) and reads +0x14 / +0x18 / +0x1C /
                                //       +0x24; startPlay +0x30 (iDrawLayer); ctor / init +0x34 (fPlaySpeed)
    RCMP_AV_PLAYER* m_pPlayer;  // 0x38 [O] (On Tour PspAvPlayer*; on PS2 the AV player); [R] ctor 0; [U]
                                //       cMoviePlayer_init creates it (0x78 bytes from the movie allocator,
                                //       func_003ADE08 / func_003ADDC0), purge deletes it (func_003AE300(p, 3)); every
                                //       method tests it first
    RCMP_VIDEO_DRIVER* m_pVd;   // 0x3C [O] VIDEO_DRIVER*; [R] ctor 0; [U] startPlay = SSX_VD_create(...); asyncRender /
                                //       syncPlay / asyncUpdate call its vtable slots 4 / 5 / 6 (vptr at +0x4); purge
                                //       deletes it (slot 1, flags 3)
    int mbNewFrame;             // 0x40 [O] (On Tour bool); [R] ctor 0; [U] startPlay / asyncUpdate set it, asyncRender
                                //       clears it after drawing, syncPlay / purge clear it; purge releases mpCurrFrame
                                //       only while it is set

    // Methods bound to their retail symbols by asm labels.
    void init1(int videoFile) __asm__("cMoviePlayer_init1");
    void purge() __asm__("cMoviePlayer_purge");
    void startPlay() __asm__("cMoviePlayer_startPlay");
    void syncPlay() __asm__("cMoviePlayer_syncPlay");
    bool asyncPlay() __asm__("cMoviePlayer_asyncPlay");
    void asyncRender(int force) __asm__("cMoviePlayer_asyncRender");
    void asyncUpdate() __asm__("cMoviePlayer_asyncUpdate");
    void asyncPause() __asm__("cMoviePlayer_asyncPause");
    void asyncResume() __asm__("cMoviePlayer_asyncResume");
};

CHECK_OFFSET(tMoviePlayOptions, sVideoFileName, 0x00);
CHECK_OFFSET(tMoviePlayOptions, sAudioFileName, 0x04);
CHECK_OFFSET(tMoviePlayOptions, bSoundOn, 0x08);
CHECK_OFFSET(tMoviePlayOptions, bFullScreen, 0x0C);
CHECK_OFFSET(tMoviePlayOptions, bPreLoad, 0x10);
CHECK_OFFSET(tMoviePlayOptions, bSoftware, 0x14);
CHECK_OFFSET(tMoviePlayOptions, iMemType, 0x18);
CHECK_OFFSET(tMoviePlayOptions, iDrawLayer, 0x1C);
CHECK_OFFSET(tMoviePlayOptions, fPlaySpeed, 0x20);
CHECK_SIZE(tMoviePlayOptions, 0x24);

CHECK_OFFSET(cMoviePlayer, mbMovieFinished, 0x00);
CHECK_OFFSET(cMoviePlayer, mbReDraw, 0x04);
CHECK_OFFSET(cMoviePlayer, mbPaused, 0x08);
CHECK_OFFSET(cMoviePlayer, mpCurrFrame, 0x0C);
CHECK_OFFSET(cMoviePlayer, m_fSpeed, 0x10);
CHECK_OFFSET(cMoviePlayer, m_PlayOp, 0x14);
CHECK_OFFSET(cMoviePlayer, m_pPlayer, 0x38);
CHECK_OFFSET(cMoviePlayer, m_pVd, 0x3C);
CHECK_OFFSET(cMoviePlayer, mbNewFrame, 0x40);
CHECK_SIZE(cMoviePlayer, 0x44);

#endif /* MOVIE_MOVIEPLAYER_H */
