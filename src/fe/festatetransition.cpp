#include "common.h"

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
extern "C" void cFEStateTransition_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* engine2;
    *(void**)((char*)self + 0x48) = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_transition), 0);
    engine2 = *(void**)((char*)self + 0x10);
    *(void**)((char*)self + 0x4C) = cUIEngine_addScreenByHashName(engine2, self, GetHashValue32(str_whitefade), 0);
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
extern "C" int cFEStateTransition_onScreenEvent(void* self, void* screen, int id)
{
    int eventHash1 = GetHashValue32(str_end);
    int eventHash2 = GetHashValue32(str_start);
    if (id == eventHash1) {
        *(int*)((char*)self + 0x1C) &= ~8;
        *(int*)((char*)screen + 0x54) &= ~0x40;
    } else if (id == eventHash2) {
        *(int*)((char*)self + 0x1C) |= 8;
        *(int*)((char*)screen + 0x54) |= 0x40;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", cFEStateTransition_handleEvent);
#ifdef SKIP_ASM
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* func_0028B180();
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
extern "C" int cFEStateTransition_handleEvent(void* self, int which)
{
    if (which == 0) {
        void* screen = *(void**)((char*)self + 0x48);
        if (screen != 0) {
            *(int*)((char*)screen + 0x54) |= 0x40;
            cUIScreen_playFrame(*(void**)((char*)self + 0x48), 0, 0);
            cSSXAudio_MenuSound(func_0028B180(), 0xF);
        }
    } else if (which == 1) {
        void* screen = *(void**)((char*)self + 0x4C);
        if (screen != 0) {
            *(int*)((char*)screen + 0x54) |= 0x40;
            cUIScreen_playFrame(*(void**)((char*)self + 0x4C), 0, 0);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatetransition", func_00194738);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* cFEStateTitle_vtbl[];

extern "C" void* func_00194738(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = cFEStateTitle_vtbl;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

