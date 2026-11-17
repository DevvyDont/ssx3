#include "common.h"
#include "render/ps2graphicsman.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_ReadyVideo);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002533C8(void* mem);
extern "C" void func_00253390(void* self);
extern "C" int func_002534A8(void* p, void* q);
extern "C" void func_00253418(void* p, int a1);
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
 * (func_00253390), sets @p file and the words +0x8 = 1, +0x18 = 0x100 and +0x1C = 9 in it and hands it to
 * func_002534A8. Its failure branch (destroy the player, return 0) is dead: func_002534A8 always returns 1.
 *
 * @param file  the video to play (On Tour pVideoFileName)
 * @return 1 when the player was created, 0 when one already existed
 */
extern "C" int cVidEngine_ReadyVideo(void** self, int file)
{
    if (*self != 0) return 0;
    *self = func_002533C8(cMemMan_alloc(68, str_VidEnginePlayer, 0x100, 0));
    sVidParams38E8 params;
    func_00253390(&params);
    //S
    params.file = file;
    params.f8 = 1;
    params.f18 = 0x100;
    params.f1C = 9;
    params.fC = 0;
    params.f10 = 0;
    //E
    if (func_002534A8(*self, &params) == 0) {
        if (*self != 0) func_00253418(*self, 3);
        *self = 0;
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", cVidEngine_PlayVideo);
#ifdef SKIP_ASM
extern "C" int func_00253860(void*);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PauseSFX(void*);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void*);
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_00442918[];

struct sVidObj { char pad0[0x10]; int state; };
struct sVidOwner { char pad0[0x64]; sVidObj* obj; };

struct sVidPlayer { void* p; int active; int f8; int fC; int f10; int f14; };
struct sVtEnt { short delta; short index; void* fn; };

/**
 * Starts the readied video: pauses the game for it, saves part of the render state and pauses the sound effects.
 *
 * Returns 0 without a movie player (+0x0, On Tour mpPlayer) and 1 when already playing (+0x4, mIsPlaying). Otherwise
 * starts the player with func_00253860 (On Tour cMoviePlayer::asyncPlay), marks it playing and pushes pause state 10:
 * D_005366E8[++D_004A2A54] = 10 and cPauseState_Flags = D_00442918 (the flags table's entry 10). On Tour asserts the
 * pause stack has room; retail does not. It then records in +0x14 whether the state (+0x10) of the game's +0x64 object
 * is neither 0 nor 2, calls cCutSceneTransitionMan_ForceToRegularMode on that object, saves GrMan's virtual slot 42 result in +0x10, calls GrMan's
 * slot 40 with 0, and pauses the sound effects (cSSXAudio_PauseSFX on cAudioInterface_bxAudio()).
 *
 * @return 1 if the video is (now) playing, 0 without a movie player
 */
extern "C" int cVidEngine_PlayVideo(sVidPlayer* self)
{
    void* player = self->p;
    if (player == 0) return 0;
    if (self->active != 0) return 1;
    func_00253860(player);
    int n = D_004A2A54 + 1;
    self->active = 1;
    D_005366E8[n] = 10;
    cPauseState_Flags = D_00442918[0];
    D_004A2A54 = n;
    sVidObj* cutSceneMan = ((sVidOwner*)SSXApp->Game)->obj;
    self->f14 = 0;
    int cutSceneState = cutSceneMan->state;
    if (cutSceneState != 0 && cutSceneState != 2) self->f14 = 1;
    cCutSceneTransitionMan_ForceToRegularMode(cutSceneMan);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sVtEnt* vt = (sVtEnt*)ps2GraphicsMan->mVtbl;
        self->f10 = *((int*(*)(void*))vt[42].fn)((char*)ps2GraphicsMan + vt[42].delta);
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
extern "C" void cVidEngine_UpdateVideo(void* self)
{
    void* player = *(void**)self;
    if (player != 0 && *(int*)((char*)self + 0x4) != 0 && *(int*)((char*)self + 0xC) == 0) {
        cMoviePlayer_asyncUpdate(player);
        *(int*)((char*)self + 0x8) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283AF8);
#ifdef SKIP_ASM
extern "C" void func_00253890(void* p, int a1);
extern "C" void func_002539E0(void* self);
extern "C" void func_00253A40(void* self);

extern "C" void func_00283AF8(void* self)
{
    void* videoPlayer = *(void**)((char*)self + 0x0);
    if (videoPlayer != 0 && *(int*)((char*)self + 0x4) != 0) {
        if (*(int*)((char*)self + 0x8) != 0) {
            if (*(int*)((char*)videoPlayer + 0x8) != 0) {
                func_00253A40(videoPlayer);
            }
        } else {
            if (*(int*)((char*)videoPlayer + 0x8) == 0) {
                func_002539E0(videoPlayer);
            }
        }
        func_00253890(*(void**)((char*)self + 0x0), *(int*)((char*)self + 0x8) ^ 1);
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283B78);
#ifdef SKIP_ASM
extern "C" void func_00283B78(void* self, int inc)
{
    if (*(int*)((char*)self + 0x0) != 0 && *(int*)((char*)self + 0x4) != 0) {
        if (inc != 0) {
            *(int*)((char*)self + 0xc) += 1;
        } else if (*(int*)((char*)self + 0xc) != 0) {
            *(int*)((char*)self + 0xc) -= 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283BB8);
#ifdef SKIP_ASM
extern "C" int func_00283BB8(void* self)
{
    int* videoPlayer = *(int**)((char*)self + 0x0);
    if (videoPlayer == 0 || *(int*)((char*)self + 0x4) == 0) {
        return 0;
    }
    return *videoPlayer;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283BE0);
#ifdef SKIP_ASM
extern "C" void* func_00283BE0(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C20__FPv);
#ifdef SKIP_ASM
void func_00283C20(void* self)
{
    *(int*)((char*)self + 0x8c) = 0;
    *(int*)((char*)self + 0x90) = 0;
    *(int*)((char*)self + 0x94) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283C30);
#ifdef SKIP_ASM
void func_00283C20(void*);

extern "C" void* func_00283C30(void* self)
{
    func_00283C20(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C58__FPv);
#ifdef SKIP_ASM
int func_00283C58(void* self)
{
    return *(int*)((char*)self + 0x94);
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C60);
#ifdef SKIP_ASM
extern "C" int func_00283C60(void* self, int index)
{
    if (index == 4) {
        return 0;
    }
    return index + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283C80);
#ifdef SKIP_ASM
int func_00283C58(void*);
extern "C" int func_00283C60(void* self, int a1);

struct sVidCmd3C80 {
    int v[7];
};

extern "C" int func_00283C80(void* self, sVidCmd3C80* cmd)
{
    if (func_00283C58(self) != 0) return 0;
    ((sVidCmd3C80*)self)[*(int*)((char*)self + 0x90)] = *cmd;
    *(int*)((char*)self + 0x90) = func_00283C60(self, *(int*)((char*)self + 0x90));
    if (*(int*)((char*)self + 0x90) == *(int*)((char*)self + 0x8C)) {
        *(int*)((char*)self + 0x94) = 1;
    }
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283D28);
#ifdef SKIP_ASM
int func_00283C58(void*);

extern "C" int func_00283D28(void* self)
{
    int n;
    if (func_00283C58(self) != 0) {
        return 5;
    }
    n = *(int*)((char*)self + 0x90) - *(int*)((char*)self + 0x8c);
    if (n < 0) {
        n += 5;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283D70);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* self, int index)
{
    return (char*)self + ((*(int*)((char*)self + 0x8c) + index) % 5) * 0x1c;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283DA0);
#ifdef SKIP_ASM
extern "C" int func_00283DA0(void* self)
{
    int isEmpty = 0;
    if (*(int*)((char*)self + 0x90) == *(int*)((char*)self + 0x8c)) {
        isEmpty = *(int*)((char*)self + 0x94) == 0;
    }
    return isEmpty;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283DC0);
#ifdef SKIP_ASM
extern "C" int func_00283DA0(void* self);
extern "C" int func_00283C60(void* self, int a1);

struct sVidEntry1C { int v[7]; };

extern "C" int func_00283DC0(void* self, sVidEntry1C* out)
{
    if (func_00283DA0(self) != 0) {
        return 0;
    }
    *(int*)((char*)self + 0x94) = 0;
    if (out != 0) {
        *out = ((sVidEntry1C*)self)[*(int*)((char*)self + 0x8c)];
    }
    *(int*)((char*)self + 0x8c) = func_00283C60(self, *(int*)((char*)self + 0x8c));
    return 1;
}
#endif

