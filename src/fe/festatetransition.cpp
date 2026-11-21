#include "common.h"
#include "ui/uiscreen.h"
#include "fe/festatetransition.h"
#include "fe/festatetitle.h"

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateBackground_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_snowanim[];

/**
 * Creates the front end's background screen ("snowanim") and plays its first frame.
 */
extern "C" void cFEStateBackground_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_snowanim), 0);
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern char str_transition[];
extern char str_whitefade[];

/**
 * Creates the transition state's two screens, "transition" and "whitefade".
 */
extern "C" void cFEStateTransition_onCreateScreen(cFEStateTransition* self)
{
    void* engine = self->mpEngine;
    void* engine2;
    self->mpSnow = (cUIScreen*)cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_transition), 0);
    engine2 = (void*)self->mpEngine;
    self->mpWhite = (cUIScreen*)cUIEngine_addScreenByHashName(engine2, self, GetHashValue32(str_whitefade), 0);
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTransition_onScreenEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_end[];
extern char str_start[];

/**
 * Tracks a transition animation: "start" sets the state's busy flag (8) and the screen's 0x40 flag, "end"
 * clears them.
 *
 * @param screen  the screen that sent the event
 * @param id  the event's label hash
 * @return 0
 */
extern "C" int cFEStateTransition_onScreenEvent(cFEStateTransition* self, cUIScreen* screen, int id)
{
    int eventHash1 = GetHashValue32(str_end);
    int eventHash2 = GetHashValue32(str_start);
    if (id == eventHash1) {
        self->mAttrState &= ~8;
        screen->mRootGroup.mAttr &= ~0x40;
    } else if (id == eventHash2) {
        self->mAttrState |= 8;
        screen->mRootGroup.mAttr |= 0x40;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTransition_handleEvent);
#ifdef SKIP_ASM
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void*, int);

/**
 * Starts one of the two transition animations on request.
 *
 * For @p which 0 it marks the screen at +0x48 (its word +0x54 |= 0x40), plays it from frame 0 and plays menu sound
 * 15; for 1 it does the same with the screen at +0x4C, without a sound. Always returns 0.
 *
 * @param which  the event id (On Tour: int msg); only 0 and 1 do anything
 * @return 0
 */
extern "C" int cFEStateTransition_handleEvent(cFEStateTransition* self, int which)
{
    if (which == 0) {
        cUIScreen* screen = self->mpSnow;
        if (screen != 0) {
            screen->mRootGroup.mAttr |= 0x40;
            cUIScreen_playFrame(self->mpSnow, 0, 0);
            cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0xF);
        }
    } else if (which == 1) {
        cUIScreen* screen = self->mpWhite;
        if (screen != 0) {
            screen->mRootGroup.mAttr |= 0x40;
            cUIScreen_playFrame(self->mpWhite, 0, 0);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTitle_cFEStateTitle1);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void* self);
extern void* cFEStateTitle_vtbl[];

/**
 * Constructs the title-screen state (allocated as "cFEStateTitle", 76 bytes).
 *
 * Runs cUIState's constructor (cUIState_cUIState; the engine argument reaches it untouched in the second argument
 * register), stores cFEStateTitle_vtbl, clears +0x48 (On Tour mNumTicksIdle) and sets mHashName to 0.
 *
 * @return @p self
 */
extern "C" void* cFEStateTitle_cFEStateTitle1(cFEStateTitle* self)
{
    cUIState_cUIState(self);
    self->mVtbl = cFEStateTitle_vtbl;
    self->mNumTicksIdle = 0;
    self->mHashName = 0;
    return ((void*)self);
}
#endif

