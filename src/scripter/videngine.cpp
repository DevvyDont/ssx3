#include "common.h"
#include "scripter/nislist.h"
#include "sound/ssxaudio.h"
#include "scripter/videngine.h"
#include "scripter/circularbuffer.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_ReadyVideo);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cMoviePlayer_cMoviePlayer(void* mem);
extern "C" void tMoviePlayOptions_SetDefaults(void* self);
extern "C" int cMoviePlayer_init(void* p, void* q);
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int a1);
extern const char str_VidEnginePlayer[];

struct sVidParams38E8 {
    int file;       // 0x0
    int f4;         // 0x4
    int f8;         // 0x8
    int fC;         // 0xC
    int f10;        // 0x10
    int f14;        // 0x14
    int f18;        // 0x18
    int f1C;        // 0x1C
    float f20;      // 0x20
};

/**
 * Creates the video player and prepares a video for playback, unless a player already exists.
 *
 * Allocates and constructs the 0x44-byte "VidEnginePlayer" at +0x0, fills a parameter block from the defaults
 * (tMoviePlayOptions_SetDefaults), sets @p file and the words +0x8 = 1, +0x18 = 0x100 and +0x1C = 9 in it and hands it to
 * cMoviePlayer_init. Its failure branch (destroy the player, return 0) is dead: cMoviePlayer_init always returns 1.
 *
 * @param file  the video to play (On Tour pVideoFileName)
 * @return 1 when the player was created, 0 when one already existed
 */
extern "C" int cVidEngine_ReadyVideo(void** self, int file)
{
    if (*self != 0) return 0;
    *self = cMoviePlayer_cMoviePlayer(cMemMan_alloc(68, str_VidEnginePlayer, 0x100, 0));
    sVidParams38E8 params;
    tMoviePlayOptions_SetDefaults(&params);
    //S
    params.file = file;
    params.f8 = 1;
    params.f18 = 0x100;
    params.f1C = 9;
    params.fC = 0;
    params.f10 = 0;
    //E
    if (cMoviePlayer_init(*self, &params) == 0) {
        if (*self != 0) cMoviePlayer__cMoviePlayer(*self, 3);
        *self = 0;
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_PlayVideo);
#ifdef SKIP_ASM
extern "C" int cMoviePlayer_asyncPlay(void*);
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
struct sVtEnt { short delta; short index; void* fn; };

/**
 * Starts the readied video: pauses the game for it, saves part of the render state and pauses the sound effects.
 *
 * Returns 0 without a movie player (+0x0, On Tour mpPlayer) and 1 when already playing (+0x4, mIsPlaying). Otherwise
 * starts the player with cMoviePlayer_asyncPlay (On Tour cMoviePlayer::asyncPlay), marks it playing and pushes pause state 10:
 * cPauseState_Stack[++cPauseState_StackIdx] = 10 and cPauseState_Flags = D_00442918 (the flags table's entry 10). On Tour asserts the
 * pause stack has room; retail does not. It then records in +0x14 whether the state (+0x10) of the game's +0x64 object
 * is neither 0 nor 2, calls cCutSceneTransitionMan_ForceToRegularMode on that object, saves GrMan's virtual slot 42 result in +0x10, calls GrMan's
 * slot 40 with 0, and pauses the sound effects (cSSXAudio_PauseSFX on cAudioInterface_bxAudio()).
 *
 * @return 1 if the video is (now) playing, 0 without a movie player
 */
extern "C" int cVidEngine_PlayVideo(cVidEngine* self)
{
    void* player = self->mpPlayer;
    if (player == 0) return 0;
    if (self->mIsPlaying != 0) return 1;
    cMoviePlayer_asyncPlay(player);
    int n = cPauseState_StackIdx + 1;
    self->mIsPlaying = 1;
    cPauseState_Stack[n] = 10;
    cPauseState_Flags = D_00442918[0];
    cPauseState_StackIdx = n;
    sVidObj* cutSceneMan = ((sVidOwner*)SSXApp->Game)->obj;
    self->mRestoreCutSceneMode = 0;
    int cutSceneState = cutSceneMan->state;
    if (cutSceneState != 0 && cutSceneState != 2) self->mRestoreCutSceneMode = 1;
    cCutSceneTransitionMan_ForceToRegularMode(cutSceneMan);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVtEnt* vt = (sVtEnt*)ps2GraphicsMan->mVtbl;
        self->mSavedDisplayFormat = *((int*(*)(void*))vt[42].fn)((char*)ps2GraphicsMan + vt[42].delta);
    }
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVtEnt* vt = (sVtEnt*)ps2GraphicsMan->mVtbl;
        ((void(*)(void*, int))vt[40].fn)((char*)ps2GraphicsMan + vt[40].delta, 0);
    }
    cSSXAudio_PauseSFX(cAudioInterface_bxAudio());
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_UpdateVideo);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer_asyncUpdate(void* self);

/**
 * Advances the playing video by one step unless it is paused.
 *
 * With a movie player (+0x0) that is playing (+0x4) and no pause count (+0xC, On Tour mNumPause) it calls cMoviePlayer_asyncUpdate
 * (On Tour cMoviePlayer::asyncUpdate) and sets the frame-updated flag (+0x8, On Tour mFrameUpdated).
 * cSSXScriptEngine_Update calls it every frame on the engine's video engine (+0x500).
 */
extern "C" void cVidEngine_UpdateVideo(cVidEngine* self)
{
    void* player = *(void**)self;
    if (player != 0 && self->mIsPlaying != 0 && self->mNumPause == 0) {
        cMoviePlayer_asyncUpdate(player);
        self->mFrameUpdated = 1;
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
 * paused (cMoviePlayer_asyncResume), otherwise it is paused if running (cMoviePlayer_asyncPause); then cMoviePlayer_asyncRender(mpPlayer,
 * !mFrameUpdated) and mFrameUpdated is cleared.
 */
extern "C" void cVidEngine_RenderVideo(cVidEngine* self)
{
    void* videoPlayer = self->mpPlayer;
    if (videoPlayer != 0 && self->mIsPlaying != 0) {
        if (self->mFrameUpdated != 0) {
            if (*(int*)((char*)videoPlayer + 0x8) != 0) {
                cMoviePlayer_asyncResume(videoPlayer);
            }
        } else {
            if (*(int*)((char*)videoPlayer + 0x8) == 0) {
                cMoviePlayer_asyncPause(videoPlayer);
            }
        }
        cMoviePlayer_asyncRender(self->mpPlayer, self->mFrameUpdated ^ 1);
        self->mFrameUpdated = 0;
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
extern "C" void cVidEngine_PauseVideo(cVidEngine* self, int inc)
{
    if ((int)self->mpPlayer != 0 && self->mIsPlaying != 0) {
        if (inc != 0) {
            self->mNumPause += 1;
        } else if (self->mNumPause != 0) {
            self->mNumPause -= 1;
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
extern "C" int cVidEngine_IsVideoOnLastFrame(cVidEngine* self)
{
    int* videoPlayer = (int*)self->mpPlayer;
    if (videoPlayer == 0 || self->mIsPlaying == 0) {
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
void cCircularBuffer_Reset(void* vself)
{
    cCircularBuffer* self = (cCircularBuffer*)vself;
    self->mHeadIndex = 0;
    self->mTailIndex = 0;
    self->mIsFull = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", cCircularBuffer_cCircularBuffer);
#ifdef SKIP_ASM
void cCircularBuffer_Reset(void*);

/**
 * Constructs a circular buffer of NIS list entries: resets it (cCircularBuffer_Reset).
 *
 * @return @p self
 */
extern "C" void* cCircularBuffer_cCircularBuffer(cCircularBuffer* self)
{
    cCircularBuffer_Reset(self);
    return ((void*)self);
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
int cCircularBuffer_IsFull(void* vself)
{
    cCircularBuffer* self = (cCircularBuffer*)vself;
    return self->mIsFull;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cCircularBuffer_IncBufferIndex);
#ifdef SKIP_ASM
/**
 * Returns the buffer index after @p index in a 5-entry circular buffer: @p index + 1, wrapping 4 to 0.
 *
 * @param index  the index (0-4)
 * @return the next index
 */
extern "C" int cCircularBuffer_IncBufferIndex(cCircularBuffer* self, int index)
{
    if (index == 4) {
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
 * @return 1, or 0 when the buffer was full
 */
extern "C" int cCircularBuffer_AddToTail(cCircularBuffer* self, tNISListEntry* cmd)
{
    if (cCircularBuffer_IsFull(self) != 0) return 0;
    ((tNISListEntry*)self)[self->mTailIndex] = *cmd;
    self->mTailIndex = cCircularBuffer_IncBufferIndex(self, self->mTailIndex);
    if (self->mTailIndex == self->mHeadIndex) {
        self->mIsFull = 1;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", cCircularBuffer_GetNumEntries);
#ifdef SKIP_ASM
int cCircularBuffer_IsFull(void*);

/**
 * Returns the number of entries in a circular buffer of NIS list entries: 5 when full, else tail (+0x90) minus head
 * (+0x8C), modulo 5.
 *
 * @return the entry count (0-5)
 */
extern "C" int cCircularBuffer_GetNumEntries(cCircularBuffer* self)
{
    int n;
    if (cCircularBuffer_IsFull(self) != 0) {
        return 5;
    }
    n = self->mTailIndex - self->mHeadIndex;
    if (n < 0) {
        n += 5;
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
extern "C" void* cCircularBuffer_operatorIndex(cCircularBuffer* self, int index)
{
    return (char*)self + ((self->mHeadIndex + index) % 5) * 0x1c;
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
extern "C" int cCircularBuffer_IsEmpty(cCircularBuffer* self)
{
    int isEmpty = 0;
    if (self->mTailIndex == self->mHeadIndex) {
        isEmpty = self->mIsFull == 0;
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
 * @return 1, or 0 when the buffer was empty
 */
extern "C" int cCircularBuffer_RemoveFromHead(cCircularBuffer* self, sVidEntry1C* out)
{
    if (cCircularBuffer_IsEmpty(self) != 0) {
        return 0;
    }
    self->mIsFull = 0;
    if (out != 0) {
        *out = ((sVidEntry1C*)self)[self->mHeadIndex];
    }
    self->mHeadIndex = cCircularBuffer_IncBufferIndex(self, self->mHeadIndex);
    return 1;
}
#endif

