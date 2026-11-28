#include "common.h"
#include "movie/movieplayer.h"
#include "vtable.h"
#include "scripter/nislist.h"
#include "sound/ssxaudio.h"
#include "scripter/videngine.h"
#include "scripter/circularbuffer.h"
#include "visualfx/cutscenetransitionman.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_ReadyVideo);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* __12cMoviePlayer(void* mem);
extern "C" void tMoviePlayOptions_SetDefaults(void* self);
extern "C" int cMoviePlayer_init(void* p, void* q);
extern "C" void _$_12cMoviePlayer(void* p, int a1);
extern const char str_VidEnginePlayer[];


/**
 * Creates the video player and prepares a video for playback, unless a player already exists.
 *
 * Allocates and constructs the 0x44-byte "VidEnginePlayer" at +0x0, fills a parameter block from the defaults
 * (tMoviePlayOptions_SetDefaults), sets @p file and the words +0x8 = 1, +0x18 = 0x100 and +0x1C = 9 in it and hands it
 * to cMoviePlayer_init. Its failure branch (destroy the player, return false) is dead: cMoviePlayer_init always returns
 * 1.
 *
 * @param file  the video to play (On Tour pVideoFileName)
 * @return true when the player was created, false when one already existed
 */
extern "C" bool cVidEngine_ReadyVideo(void** self, int file)
{
    if (*self != 0) return false;
    *self = __12cMoviePlayer(cMemMan_alloc(68, str_VidEnginePlayer, 0x100, 0));
    tMoviePlayOptions params;
    tMoviePlayOptions_SetDefaults(&params);
    //S
    (*(int*)&params.sVideoFileName) = file;
    params.bSoundOn = 1;
    params.iMemType = 0x100;
    params.iDrawLayer = 9;
    params.bFullScreen = 0;
    params.bPreLoad = 0;
    //E
    if (cMoviePlayer_init(*self, &params) == 0) {
        if (*self != 0) _$_12cMoviePlayer(*self, 3);
        *self = 0;
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_PlayVideo);
#ifdef SKIP_ASM
extern "C" bool cMoviePlayer_asyncPlay(void*);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PauseSFX(void*);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void*);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cPauseState_Stack[];
extern int D_00442918[];

struct sVidObj { char pad0[0x10]; int state; };
struct sVidOwner { char pad0[0x64]; sVidObj* obj; };

struct sVidPlayer { void* p; int active; int f8; int fC; int f10; int f14; };
/**
 * Starts the readied video: pauses the game for it, saves part of the render state and pauses the sound effects.
 *
 * Returns false without a movie player (+0x0, On Tour mpPlayer) and true when already playing (+0x4, mIsPlaying).
 * Otherwise starts the player with cMoviePlayer_asyncPlay (On Tour cMoviePlayer::asyncPlay), marks it playing and
 * pushes pause state 10: cPauseState_Stack[++cPauseState_StackIdx] = 10 and cPauseState_Flags = D_00442918 (the flags
 * table's entry 10). On Tour asserts the pause stack has room; retail does not. It then records in +0x14 whether the
 * state (+0x10) of the game's +0x64 object (the cCutSceneTransitionMan) is neither kRegularMode nor
 * kTransitionToRegularMode, calls cCutSceneTransitionMan_ForceToRegularMode on that object, saves GrMan's virtual slot
 * 42 result in +0x10, calls GrMan's slot 40 with 0, and pauses the sound effects (cSSXAudio_PauseSFX on
 * cAudioInterface_bxAudio()).
 *
 * @return true if the video is (now) playing, false without a movie player
 */
bool cVidEngine::PlayVideo()
{
    void* player = mpPlayer;
    if (player == 0) return false;
    if (mIsPlaying != 0) return true;
    cMoviePlayer_asyncPlay(player);
    int n = cPauseState_StackIdx + 1;
    mIsPlaying = 1;
    cPauseState_Stack[n] = 10;
    cPauseState_Flags = D_00442918[0];
    cPauseState_StackIdx = n;
    sVidObj* cutSceneMan = ((sVidOwner*)SSXApp->Game)->obj;
    mRestoreCutSceneMode = 0;
    int cutSceneState = cutSceneMan->state;
    if (cutSceneState != cCutSceneTransitionMan::kRegularMode
        && cutSceneState != cCutSceneTransitionMan::kTransitionToRegularMode) mRestoreCutSceneMode = 1;
    cCutSceneTransitionMan_ForceToRegularMode(cutSceneMan);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        mSavedDisplayFormat = *(int*)ps2GraphicsMan->GetTVDisplayFormat();
    }
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->SetTVDisplayFormat(0);
    }
    cSSXAudio_PauseSFX(cAudioInterface_bxAudio());
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_UpdateVideo);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncUpdate(void* self);

/**
 * Advances the playing video by one step unless it is paused.
 *
 * With a movie player (+0x0) that is playing (+0x4) and no pause count (+0xC, On Tour mNumPause) it calls
 * cMoviePlayer_asyncUpdate (On Tour cMoviePlayer::asyncUpdate) and sets the frame-updated flag (+0x8, On Tour
 * mFrameUpdated). cSSXScriptEngine_Update calls it every frame on the engine's video engine (+0x500).
 */
void cVidEngine::UpdateVideo()
{
    void* player = *(void**)this;
    if (player != 0 && mIsPlaying != 0 && mNumPause == 0) {
        cMoviePlayer_asyncUpdate(player);
        mFrameUpdated = 1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_RenderVideo);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncRender(void* p, int a1);
extern "C" void cMoviePlayer_asyncPause(void* self);
extern "C" void cMoviePlayer_asyncResume(void* self);

/**
 * Renders the video for this frame, holding the movie paused on frames the engine did not update.
 *
 * Only with a movie player (mpPlayer) that is playing (mIsPlaying): when mFrameUpdated is set the player is resumed if
 * paused (cMoviePlayer_asyncResume), otherwise it is paused if running (cMoviePlayer_asyncPause); then
 * cMoviePlayer_asyncRender(mpPlayer, !mFrameUpdated) and mFrameUpdated is cleared.
 */
void cVidEngine::RenderVideo()
{
    cMoviePlayer* videoPlayer = mpPlayer;
    if (((void*)videoPlayer) != 0 && mIsPlaying != 0) {
        if (mFrameUpdated != 0) {
            if (videoPlayer->mbPaused != 0) {
                cMoviePlayer_asyncResume(videoPlayer);
            }
        } else {
            if (videoPlayer->mbPaused == 0) {
                cMoviePlayer_asyncPause(videoPlayer);
            }
        }
        cMoviePlayer_asyncRender(mpPlayer, mFrameUpdated ^ 1);
        mFrameUpdated = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_PauseVideo);
#ifdef SKIP_ASM
/**
 * Pauses or resumes the video: while a movie player (mpPlayer) is playing (mIsPlaying), @p inc counts mNumPause up and
 * 0 counts it down (not below 0).
 *
 * @param inc  non-zero to pause, 0 to resume
 */
void cVidEngine::PauseVideo(int inc)
{
    if ((int)mpPlayer != 0 && mIsPlaying != 0) {
        if (inc != 0) {
            mNumPause += 1;
        } else if (mNumPause != 0) {
            mNumPause -= 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_IsVideoOnLastFrame);
#ifdef SKIP_ASM
/**
 * Returns the movie player's first word (its end status) while a video plays: 0 without a player (mpPlayer) or when not
 * playing (mIsPlaying).
 *
 * @return the player's word +0x0, or 0
 */
int cVidEngine::IsVideoOnLastFrame()
{
    int* videoPlayer = (int*)mpPlayer;
    if (videoPlayer == 0 || mIsPlaying == 0) {
        return 0;
    }
    return *videoPlayer;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283BE0);
#ifdef SKIP_ASM
/**
 * Returns its first argument and does nothing else; the variable arguments are only spilled to the stack.
 *
 * It has no callers in the build (a variadic stub, probably a compiled-out print).
 *
 * @return the first argument
 */
extern "C" void* func_00283BE0(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_Reset__FPv);
#ifdef SKIP_ASM
/**
 * Empties a circular buffer of NIS list entries: head (+0x8C, On Tour mHeadIndex), tail (+0x90, mTailIndex) and full
 * flag (+0x94, mIsFull) become 0.
 */
void cCircularBuffer::Reset()
{
    mHeadIndex = 0;
    mTailIndex = 0;
    mIsFull = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", __15cCircularBuffer);
#ifdef SKIP_ASM
void cCircularBuffer_Reset(void*);

/**
 * Constructs a circular buffer of NIS list entries: resets it (cCircularBuffer_Reset).
 */
cCircularBuffer::cCircularBuffer()
{
    Reset();
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_IsFull__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether a circular buffer of NIS list entries is full: its full flag (+0x94, On Tour mIsFull).
 *
 * @return the flag
 */
int cCircularBuffer::IsFull()
{
    return mIsFull;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_IncBufferIndex);
#ifdef SKIP_ASM
/**
 * Returns the buffer index after @p index in a kNumEntries-entry circular buffer: @p index + 1, wrapping
 * kNumEntries - 1 to 0.
 *
 * @param index  the index (0 to kNumEntries - 1)
 * @return the next index
 */
int cCircularBuffer::IncBufferIndex(int index)
{
    if (index == kNumEntries - 1) {
        return 0;
    }
    return index + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", cCircularBuffer_AddToTail);
#ifdef SKIP_ASM
int cCircularBuffer_IsFull(void*);
extern "C" int cCircularBuffer_IncBufferIndex(cCircularBuffer* self, int a1);


/**
 * Appends a NIS list entry to a circular buffer: copies the 0x1C-byte @p cmd to the tail slot (+0x90), advances the
 * tail (cCircularBuffer_IncBufferIndex) and sets the full flag (+0x94) when the tail reaches the head (+0x8C).
 *
 * @param cmd  the entry to copy
 * @return true, or false when the buffer was full
 */
bool cCircularBuffer::AddToTail(tNISListEntry* cmd)
{
    if (IsFull() != 0) return false;
    ((tNISListEntry*)this)[mTailIndex] = *cmd;
    mTailIndex = IncBufferIndex(mTailIndex);
    if (mTailIndex == mHeadIndex) {
        mIsFull = 1;
    }
    return true;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", cCircularBuffer_GetNumEntries);
#ifdef SKIP_ASM
int cCircularBuffer_IsFull(void*);

/**
 * Returns the number of entries in a circular buffer of NIS list entries: kNumEntries when full, else tail (+0x90)
 * minus head (+0x8C), modulo kNumEntries.
 *
 * @return the entry count (0 to kNumEntries)
 */
int cCircularBuffer::GetNumEntries()
{
    int n;
    if (IsFull() != 0) {
        return kNumEntries;
    }
    n = mTailIndex - mHeadIndex;
    if (n < 0) {
        n += kNumEntries;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_operatorIndex);
#ifdef SKIP_ASM
/**
 * Returns entry @p index of a circular buffer, counted from the head: slot (head + @p index) mod 5 (0x1C bytes each).
 * The index is not range-checked against the entry count.
 *
 * @param index  the position from the head
 * @return the entry
 */
void* cCircularBuffer::operatorIndex(int index)
{
    return (char*)this + ((mHeadIndex + index) % 5) * 0x1c;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_IsEmpty);
#ifdef SKIP_ASM
/**
 * Returns whether a circular buffer of NIS list entries is empty: head (+0x8C) equals tail (+0x90) and the full flag
 * (+0x94) is clear.
 *
 * @return 1 when empty, else 0
 */
int cCircularBuffer::IsEmpty()
{
    int isEmpty = 0;
    if (mTailIndex == mHeadIndex) {
        isEmpty = mIsFull == 0;
    }
    return isEmpty;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", cCircularBuffer_RemoveFromHead);
#ifdef SKIP_ASM
extern "C" int cCircularBuffer_IsEmpty(cCircularBuffer* self);
extern "C" int cCircularBuffer_IncBufferIndex(cCircularBuffer* self, int a1);

struct sVidEntry1C { int v[7]; };

/**
 * Removes the head entry of a circular buffer of NIS list entries: clears the full flag (+0x94), copies the entry to
 * @p out when given, and advances the head (+0x8C, cCircularBuffer_IncBufferIndex).
 *
 * @param out  receives the 0x1C-byte entry, or 0
 * @return true, or false when the buffer was empty
 */
extern "C" bool cCircularBuffer_RemoveFromHead(cCircularBuffer* self, sVidEntry1C* out)
{
    if (self->IsEmpty() != 0) {
        return false;
    }
    self->mIsFull = 0;
    if (out != 0) {
        *out = ((sVidEntry1C*)self)[self->mHeadIndex];
    }
    self->mHeadIndex = self->IncBufferIndex(self->mHeadIndex);
    return true;
}
#endif

