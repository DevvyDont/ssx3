#ifndef FE_FESTATEVIDEOPLAYER_H
#define FE_FESTATEVIDEOPLAYER_H

// cFEStateVideoPlayer: the front end's full-screen movie state (reward videos, bonus material): a cFEState with no
// screen of its own that owns a cMoviePlayer (movie/movieplayer.h), plays the video / audio files named in it and
// pops itself when the movie ends or the accept / back input stops it.
// EA's source file is not known (no assert of its own; fe/festaterewards.h already names the reward screens).
// Retail code: fe/festaterewards (0x001D22F0 ... 0x001D2638) and the out-of-line inline copies in fe/fepopupmisc
// (0x001E0020 ... 0x001E0048).
//
// Size 0x280:
//   [R] cMemMan_alloc(0x280, "cFEStateVideoPlayer") + cFEStateVideoPlayer_cFEStateVideoPlayer(mem, engine, parent,
//       userNum) in cFEStateRewardGalleryBase_showReward and cFEStateBonusMaterial_onWidgetEvent; the ctor writes up
//       to 0x27C (mStopPending), so nothing follows it.
//   [O] On Tour 0x268 = cFEState 0x48 + two char[256] + a pointer + tMoviePlayOptions (0x18) + four bools; retail's
//       tMoviePlayOptions is 0x24 (movie/movieplayer.h: 4-byte bools) and its four bools are words: 0x268 + 0xC + 0xC
//       = 0x280.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// runs cUIState_cUIState1(this, pEngine, pParentState) and stores cFEStateVideoPlayer_vtbl (0x00469928, 24 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen, 7 onGainTransition__FPv, 8 onInputBegin, 10 onLoseTransition__FPv, 12
// onUpdate, 15 onRender2D); the destructor deletes the movie player, stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes).
// Evidence tags:
//   [R] retail's constructor / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x144 -> 0x248, 0x164 -> 0x270, 0x168 -> 0x274, 0x16C -> 0x278,
//       0x170 -> 0x27C, from onRender2D / onUpdate / onInputBegin / the dtor);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFEStateVideoPlayer, cUIState, cListNode): names, types and order.

#include "layout.h"
#include "movie/movieplayer.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cMoviePlayer;

struct cFEStateVideoPlayer {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState1 links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateVideoPlayer_vtbl (ctor; dtor first), cFEState_vtbl (dtor);
                                    //       [P] preview slot-1 dtor
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 6
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument (cUIState_cUIState1); [U] Stop pops the state
                                    //       through it
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's pParentState (cUIState_cUIState1)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum argument (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateVideoPlayer
    char mVideoFileName[256];       // 0x048 [O]; [R] the ctor empties it and points mOptions.sVideoFileName at it;
                                    //       [U] setVideoSourceFiles copies the video file into it
    char mAudioFileName[256];       // 0x148 [O]; [R] emptied, mOptions.sAudioFileName points at it; [U]
                                    //       setVideoSourceFiles copies the audio file into it, or empties it
    cMoviePlayer* mpMoviePlayer;    // 0x248 [O][P] (preview 0x144); [R] 0; the dtor deletes it; [U] onCreateScreen:
                                    //       cMoviePlayer_cMoviePlayer(cMemMan_alloc(68, "MoviePlayer")); Start /
                                    //       onUpdate / onRender2D drive it, onDestroyScreen deletes it and clears it
    tMoviePlayOptions mOptions;     // 0x24C [O] (On Tour 0x18, retail 0x24); [R] tMoviePlayOptions_SetDefaults, then
                                    //       the two file-name pointers; [U] setOptions copies a whole block in (0x24
                                    //       bytes, ldl/sdl); setSound (+0x8 = 0x254) and setFullScreen (+0xC = 0x258)
                                    //       store one option; Start sets bSoundOn and passes &mOptions to
                                    //       cMoviePlayer_init
    int mInterruptible;             // 0x270 [O][P] (bool; preview 0x164); [R] 1; [U] setInterruptible stores it,
                                    //       onInputBegin stops the movie on accept / back only while it is set
    int mUpdatedOnce;               // 0x274 [O][P] (bool; preview 0x168); [R] 0; [U] Start clears it, onUpdate sets it
    int mRenderOnce;                // 0x278 [O][P] (bool; preview 0x16C); [R] 0; [U] Start clears it, onRender2D sets
                                    //       it; onUpdate stops a pending stop only after a frame was drawn
    int mStopPending;               // 0x27C [O][P] (bool; preview 0x170); [R] 0; [U] onInputBegin sets it, onUpdate
                                    //       tests it, Stop clears it
};

CHECK_OFFSET(cFEStateVideoPlayer, mpPrev, 0x000);
CHECK_OFFSET(cFEStateVideoPlayer, mpNext, 0x004);
CHECK_OFFSET(cFEStateVideoPlayer, mVtbl, 0x008);
CHECK_OFFSET(cFEStateVideoPlayer, mHashName, 0x00C);
CHECK_OFFSET(cFEStateVideoPlayer, mpEngine, 0x010);
CHECK_OFFSET(cFEStateVideoPlayer, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateVideoPlayer, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateVideoPlayer, unk_016, 0x016);
CHECK_OFFSET(cFEStateVideoPlayer, miMiscData, 0x018);
CHECK_OFFSET(cFEStateVideoPlayer, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateVideoPlayer, mpParentState, 0x020);
CHECK_OFFSET(cFEStateVideoPlayer, mScreens, 0x024);
CHECK_OFFSET(cFEStateVideoPlayer, mpScreen, 0x040);
CHECK_OFFSET(cFEStateVideoPlayer, mUserNum, 0x044);
CHECK_OFFSET(cFEStateVideoPlayer, unk_045, 0x045);
CHECK_OFFSET(cFEStateVideoPlayer, mVideoFileName, 0x048);
CHECK_OFFSET(cFEStateVideoPlayer, mAudioFileName, 0x148);
CHECK_OFFSET(cFEStateVideoPlayer, mpMoviePlayer, 0x248);
CHECK_OFFSET(cFEStateVideoPlayer, mOptions, 0x24C);
CHECK_OFFSET(cFEStateVideoPlayer, mInterruptible, 0x270);
CHECK_OFFSET(cFEStateVideoPlayer, mUpdatedOnce, 0x274);
CHECK_OFFSET(cFEStateVideoPlayer, mRenderOnce, 0x278);
CHECK_OFFSET(cFEStateVideoPlayer, mStopPending, 0x27C);
CHECK_SIZE(cFEStateVideoPlayer, 0x280);

#endif /* FE_FESTATEVIDEOPLAYER_H */
