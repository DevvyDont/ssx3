#include "common.h"
#include "movie/movieplayer.h"
#include "bx/math3d.h"
#include "fe/festatetransition.h"
#include "fe/feinput.h"
#include "vtable.h"
#include "be/beintoption.h"
#include "sound/ssxaudio.h"
#include "bx/appman.h"
#include "fe/fesound.h"
#include "fe/festateselectmp.h"
#include "fe/fereal.h"
#include "fe/fecustom.h"
#include "ui/uistate.h"
#include "ui/uiengine.h"
#include "online/lobbyman.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "online/commsystem.h"
#include "online/gamecomm.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fereal", cFECustom_cFECustom);
#ifdef SKIP_ASM
struct sVE0720 {
    short delta;
    short index;
    int (*fn)(void*);
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIState_cUIState(void* self, void* owner);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern char str_cFEStateTransition[];
extern void* cFECustom_vtbl[];
extern sVE0720 cFEStateTransition_vtbl[];

/**
 * Constructs the front end's custom interface and pushes its transition state on the UI engine's stack.
 *
 * Stores the engine at +0x0 and the vtable cFECustom_vtbl, allocates a "cFEStateTransition" state (cUIState on
 * the same engine, +0x48 and +0x4C zeroed), keeps it at +0x10, runs its slot 4 (onCreateScreen) and pushes it
 * with cUIStateStack_pushExplicit().
 *
 * @param owner  the UI engine
 * @return @p self
 */
extern "C" void* cFECustom_cFECustom(cFECustom* self, cUIEngine* owner)
{
    self->mpEngine = (cUIEngine*)((void*)owner);
    self->mpTransitionState = 0;
    self->mVtbl = cFECustom_vtbl;
    cFEStateTransition* transition = (cFEStateTransition*)cMemMan_alloc(80, str_cFEStateTransition, 0, 0);
    cUIState_cUIState(transition, self->mpEngine);
    transition->mpSnow = 0;
    transition->mpWhite = 0;
    transition->mVtbl = (void**)cFEStateTransition_vtbl;
    self->mpTransitionState = (cFEStateTransition*)transition;
    if (((char*)transition) != 0) {
        cFEStateTransition_vtbl[4].fn(vthis(transition, cFEStateTransition_vtbl[4]));
        cUIStateStack_pushExplicit(&self->mpEngine->mStateStack, self->mpTransitionState);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFECustom_onScreenEvent);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_whitefade[];
struct sVEntry001A07C8 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

/**
 * Handles the front end's screen events: answers the "whitefade" event (cFECustom_vtbl entry 3, onScreenEvent).
 *
 * For the event whose hash is "whitefade" sends handleEvent (vtable entry 24) with (1, 0) to the state at +0x10 when
 * there is one and reports the event handled.
 *
 * @param screen    the screen that sent the event
 * @param hash  the event (a name hash)
 * @return 1 for "whitefade", else 0
 */
extern "C" int cFECustom_onScreenEvent(cFECustom* self, int screen, int hash)
{
    if (hash == GetHashValue32(str_whitefade)) {
        void* obj = self->mpTransitionState;
        if (obj != 0) {
            sVEntry001A07C8* vt = *(sVEntry001A07C8**)((char*)obj + 8);
            vt[24].fn(vthis(obj, vt[24]), 1, 0);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFECustom_getNextState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(void* self, void* owner);
extern "C" void* cFEStateCharSelect_cFEStateCharSelect(void* self, void* owner, int a);
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* self, void* owner, int a, int b);
extern "C" void* cFEStateCharSetup_cFEStateCharSetup(void* self, void* owner, int a);
extern "C" void* cFEStateEventSelect_cFEStateEventSelect1(void* self, void* owner, int a);
extern "C" void* cFEStateMainMenu_cFEStateMainMenu1(void* self, void* owner);
extern "C" void* cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(void* self, void* owner);
unsigned int cPreGameLoadScreen_cPreGameLoadScreen(void* mem);
void* cPreFELoadScreen_cPreFELoadScreen(void* self);
class cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cFEStateTitle_cFEStateTitle1(void* self, void* owner);
extern "C" void* cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(void* self, void* owner);
extern "C" void* cFEStateRiderDetail_cFEStateRiderDetail(void* self, void* owner, int a);
extern "C" void* cFEStateMountainRoom_cFEStateMountainRoom(void* self, void* owner, int a);
extern "C" void* cFEStatePeakRoom_cFEStatePeakRoom(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardsRoom_cFEStateRewardsRoom(void* self, void* owner, int a, int b);
extern "C" void cFEFlowState_setValue(void* flow, int a, int b);
extern "C" void func_001A86E8(int a);
extern "C" void* cFEStateBonusMaterial_cFEStateBonusMaterial1(void* self, void* owner);
extern "C" void* cFEStateNetConfig_cFEStateNetConfig(void* self, void* owner);
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int ev);
extern "C" int cBENewPlayerInterface_isFirstTimeWC(void* self, int index);
extern "C" int cBENewPlayerInterface_getLastHub(void* self, int index);
extern "C" void* cFEStateMPCircuit_cFEStateMPCircuit(void* self, void* owner);
extern "C" void* cFEStateCharEquip_cFEStateCharEquip(void* self, void* owner, int a, int b, int c);
extern "C" void* cFEStateRequestLine_cFEStateRequestLine(void* self, void* owner, int a, int b);
extern "C" void* cFEStateBraggingRights_cFEStateBraggingRights(void* self, void* owner);
extern "C" void* cFEStateRuleSelect_cFEStateRuleSelect(void* self, void* owner);
extern "C" void* func_00188EE8(void* self, void* owner);
extern "C" void* func_0018A258(void* self, void* owner);
extern "C" void* func_0018C0E8(void* self, void* owner);
extern "C" void* func_0018C658(void* self, void* owner);
extern "C" void* cFEStateCredits_cFEStateCredits1(void* self, void* owner);
extern "C" void* cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(void* self, void* owner, int a, int b, int c);
extern "C" void* cFEStateProfileLoad_cFEStateProfileLoad(void* self, void* owner, int a, int b);
extern "C" void* cFEStateBuyAttrib_cFEStateBuyAttrib(void* self, void* owner);
extern "C" void* cFEStateUberTrick_cFEStateUberTrick(void* self, void* owner, int a);
extern "C" void* cFEStateCareerStats_cFEStateCareerStats(void* self, void* owner);
extern "C" void* cFEStateRiderBio_cFEStateRiderBio(void* self, void* owner, int a);
extern "C" void* cFEStateTrophyRoom_cFEStateTrophyRoom(void* self, void* owner, int a, int b, int c);
extern "C" void* cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardGalleryToys_cFEStateRewardGalleryToys(void* self, void* owner, int a, int b);
extern "C" void* cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars(void* self, void* owner, int a, int b);
extern char gFlowState;
extern int gInLodge;
extern int D_004A19D4;
extern int D_004A19DC;
extern int cBENewRaceInterface_bIsBamLocation;
extern int D_004A1A70;
extern int cUITemplate_MAP_mMapStyle;
extern int cUITemplate_MAP_mMapFEStart;
extern int cUITemplate_MAP_mPeak;
extern char str_cPreFELoadScreen[];
extern char str_cPreGameLoadScreen[];
extern char str_cFEStateProfileLoad[];
extern char str_cFEStateTitle[];
extern char str_cFEStateOnlineMainMenu[];
extern char str_cFEStateRequestLine[];
extern char str_cFEStateAudioOptions[];
extern char str_cFEStateMainMenu[];
extern char str_cFEStateCharSelect[];
extern char str_cFEStateBonusMaterial[];
extern char str_cFEStateNetConfig[];
extern char str_cFEStateCharSetup[];
extern char str_cFEStateMPCircuit[];
extern char str_cFEStateEventSelect[];
extern char str_cFEStateCharEquip[];
extern char str_cFEStateRiderDetail[];
extern char str_cFEStateRewardsRoom[];
extern char str_cFEStateUberTrick[];
extern char str_cFEStateRiderBio[];
extern char str_cFEStateBraggingRights[];
extern char str_cFEStateRuleSelect[];
extern char str_cFEStateOptionsGame[];
extern char str_cFEStateOptionsSound[];
extern char str_cFEStateOptionsHUD[];
extern char str_cFEStateOptionsController[];
extern char str_cFEStateOptionsSaveLoad[];
extern char str_cFEOptionsCredits[];
extern char str_cFEStateOptionsDeviceSelect[];
extern char str_cFEStateBuyAttrib[];
extern char str_cFEStateLodgeRiderDetail[];
extern char str_cFEStateMountainRoom[];
extern char str_cFEStateCareerStats[];
extern char str_cFEStatePeakRoom[];
extern char str_cFEStateTrophyRoom[];
extern char str_cFEStateRewardGalleryPosters[];
extern char str_cFEStateRewardGalleryTradingCards[];
extern char str_cFEStateRewardGalleryConceptArts[];
extern char str_cFEStateRewardGalleryToys[];
extern char str_cFEStateRewardGalleryVideos[];
extern char str_cFEStateRewardGalleryCheatChars[];

static inline int isOneGNS(int x) { return x == 1; }

struct sVEntryGNS {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sFEStateGNS {
    char pad0[0xC];
    int id;
    char pad10[0x34];
    signed char x44;
};

struct sVEntry0GNS {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Creates the front-end state to go to from a state, given the state's id and the choice made in it.
 *
 * The current state's id (its word at +0xC) and @p pData select the next state, allocated under its class
 * name and built on the same engine, usually with the current state's user number (+0x44): the main menu,
 * character select and setup, event select, the options screens, rider bio and details, the rooms and reward
 * galleries, the lodge states and the online states. Some choices switch the app module instead
 * (cPreFELoadScreen, cPreGameLoadScreen, or a game at the player's last hub with game mode 4, event 12) and
 * return 0.
 *
 * Several cases let unlisted @p pData values fall through into the next case's code (for example id 12 into
 * 13 and on to 16, the request line).
 *
 * @param st  the current state (On Tour: cUIState *pCurrentState)
 * @param pData  the choice, usually the selected option's id (On Tour: void *pData)
 * @return the new state to push, or 0 when there is none or the app module was switched
 */
// Cross-jumping keeps the later copy of identical case bodies, so the cases
// whose code the original shares with a later case come first.
extern "C" void* cFECustom_getNextState(void** self, sFEStateGNS* st, int pData)
{
    switch (st->id) {
    case 0:
        switch (pData) {
        case 0: {
            void* obj = self[4];
            if (obj != 0) {
                sVEntryGNS* vt = *(sVEntryGNS**)((char*)obj + 8);
                vt[24].fn(vthis(obj, vt[24]), 0, 0);
            }
            break;
        }
        case 1:
            SSXApp->UseRealFE = 0;
            cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreFELoadScreen_cPreFELoadScreen(cMemMan_alloc(8, str_cPreFELoadScreen, 0x100, 0)));
            return 0;
        }
    case 1:
        cFEFlowState_setValue(&gFlowState, 2, 0);
        return cFEStateMainMenu_cFEStateMainMenu1(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), *self);
    case 59:
        func_001A86E8(1);
        return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, st->x44);
    case 2:
        switch (pData) {
        case 11:
            func_001A86E8(0);
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
        case 9:
            func_001A86E8(0);
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
        case 22:
            func_001A86E8(0);
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
        case 5:
            return cFEStateBonusMaterial_cFEStateBonusMaterial1(cMemMan_alloc(72, str_cFEStateBonusMaterial, 0, 0), *self);
        case 49:
            func_001A86E8(1);
            return cFEStateNetConfig_cFEStateNetConfig(cMemMan_alloc(2332, str_cFEStateNetConfig, 0, 0), *self);
        }
    case 22:
        D_004A19D4 = pData != 0;
        return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
    case 23:
        cAppMan_setNextModule((cAppMan*)SSXApp, cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        return 0;
    case 9:
    case 11:
        if (isOneGNS(D_004A1A70))
            return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, 0);
        {
            int one = 1;
            cBENewRaceInterface* nr = (cBENewRaceInterface*)cBE_getInterface_Fv(cBE_getBE(), 0);
            void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
            switch (beRaceDB->cGamePath) {
            case 1:
                return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, 0);
            case 0: {
                cBENewRaceInterface_setGameMode(nr, 4);
                cBENewRaceInterface_setGameEvent(nr, 12);
                if (cBENewPlayerInterface_isFirstTimeWC(pl, 0)) {
                    cBENewRaceInterface_bIsBamLocation = one;
                    beRaceDB->iLocationIdx = 14;
                } else {
                    beRaceDB->iLocationIdx = cBENewPlayerInterface_getLastHub(pl, 0);
                    cBENewRaceInterface_bIsBamLocation = one;
                }
                sVEntry0GNS* vt = (sVEntry0GNS*)nr->mVtbl;
                vt[1].fn(vthis(nr, vt[1]));
                D_004A19DC = 1;
                cAppMan_setNextModule((cAppMan*)SSXApp, cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
                break;
            }
            case 2:
                return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, st->x44);
            default:
                return 0;
            }
        }
        break;
    case 12:
        switch (pData) {
        case 0:
            if (isOneGNS(D_004A1A70))
                return cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), *self);
            if (D_004A19D4 != 0 && st->x44 == 1)
                return cFEStateMPCircuit_cFEStateMPCircuit(cMemMan_alloc(72, str_cFEStateMPCircuit, 0, 0), *self);
            cBE_getInterface_Fv(cBE_getBE(), 0);
            switch (cBENewRaceInterface_newRace.cGamePath) {
            case 0:
            case 1: {
                cUITemplate_MAP_mPeak = 1;
                cUITemplate_MAP_mMapStyle = 0;
                cUITemplate_MAP_mMapFEStart = 0;
                char* eventSelect = (char*)cFEStateEventSelect_cFEStateEventSelect1(cMemMan_alloc(828, str_cFEStateEventSelect, 0, 0), *self, 0);
                eventSelect[0x44] = 0;
                return eventSelect;
            }
            case 2:
                if (st->x44 == 0)
                    return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 1);
                if (D_004A19D4 != 0)
                    return cFEStateMPCircuit_cFEStateMPCircuit(cMemMan_alloc(72, str_cFEStateMPCircuit, 0, 0), *self);
                cUITemplate_MAP_mPeak = 1;
                cUITemplate_MAP_mMapStyle = 0;
                cUITemplate_MAP_mMapFEStart = 0;
                return cFEStateEventSelect_cFEStateEventSelect1(cMemMan_alloc(828, str_cFEStateEventSelect, 0, 0), *self, 0);
            }
            break;
        case 1:
            return cFEStateCharEquip_cFEStateCharEquip(cMemMan_alloc(2800, str_cFEStateCharEquip, 0, 0), *self, st->x44, 0, 0);
        case 2:
            return cFEStateRiderDetail_cFEStateRiderDetail(cMemMan_alloc(84, str_cFEStateRiderDetail, 0, 0), *self, st->x44);
        case 3:
            return cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, str_cFEStateAudioOptions, 0, 0), *self, st->x44, 0);
        case 4:
            break;
        }
    case 13:
        switch (pData) {
        case 5:
            return cFEStateRewardsRoom_cFEStateRewardsRoom(cMemMan_alloc(80, str_cFEStateRewardsRoom, 0, 0), *self, st->x44, 1);
        case 7:
            return cFEStateUberTrick_cFEStateUberTrick(cMemMan_alloc(0x60, str_cFEStateUberTrick, 0, 0), *self, st->x44);
        case 9:
            return cFEStateRiderBio_cFEStateRiderBio(cMemMan_alloc(80, str_cFEStateRiderBio, 0, 0), *self, st->x44);
        }
    case 16:
        return cFEStateRequestLine_cFEStateRequestLine(cMemMan_alloc(528, str_cFEStateRequestLine, 0, 0), *self, st->x44, gInLodge);
    case 19:
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (cBENewRaceInterface_newRace.cGamePath) {
        case 0:
        case 1:
            return cFEStateRuleSelect_cFEStateRuleSelect(cMemMan_alloc(156, str_cFEStateRuleSelect, 0, 0), *self);
        case 2:
            return cFEStateBraggingRights_cFEStateBraggingRights(cMemMan_alloc(1852, str_cFEStateBraggingRights, 0, 0), *self);
        }
        return cFEStateRuleSelect_cFEStateRuleSelect(cMemMan_alloc(156, str_cFEStateRuleSelect, 0, 0), *self);
    case 24:
        switch (pData) {
        case 28:
            return func_00188EE8(cMemMan_alloc(76, str_cFEStateOptionsGame, 0, 0), *self);
        case 29:
            return func_0018A258(cMemMan_alloc(72, str_cFEStateOptionsSound, 0, 0), *self);
        case 30:
            return func_0018C0E8(cMemMan_alloc(72, str_cFEStateOptionsHUD, 0, 0), *self);
        case 31:
            return func_0018C658(cMemMan_alloc(84, str_cFEStateOptionsController, 0, 0), *self);
        case 32:
            ((int*)self)[3] = 0x18;
            return cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(cMemMan_alloc(76, str_cFEStateOptionsSaveLoad, 0, 0), *self);
        case 36:
            return cFEStateCredits_cFEStateCredits1(cMemMan_alloc(104, str_cFEOptionsCredits, 0, 0), *self);
        case 34:
            ((int*)self)[3] = 34;
            return cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(cMemMan_alloc(572, str_cFEStateOptionsDeviceSelect, 0, 0), *self, 2, 1, 0);
        }
        break;
    case 32:
    case 37:
    case 38:
        switch (pData) {
        case 0:
            return cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(cMemMan_alloc(572, str_cFEStateOptionsDeviceSelect, 0, 0), *self, 2, 1, 0);
        case 1:
            return cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(cMemMan_alloc(572, str_cFEStateOptionsDeviceSelect, 0, 0), *self, 1, 0, 0);
        case 2:
            return cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(cMemMan_alloc(572, str_cFEStateOptionsDeviceSelect, 0, 0), *self, 2, 0, 0);
        }
        {
            int hi = pData >> 8;
            int lo = pData & 0xFF;
            return cFEStateProfileLoad_cFEStateProfileLoad(cMemMan_alloc(640, str_cFEStateProfileLoad, 0, 0), *self, hi, lo);
        }
    case 8:
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (cBENewRaceInterface_newRace.cGamePath) {
        case 0:
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
        case 1:
        case 2:
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 0);
        }
        break;
    case 20:
    case 21:
        cAppMan_setNextModule((cAppMan*)SSXApp, cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        break;
    case 39:
        switch (pData) {
        case 0: {
            cBE_getInterface_Fv(cBE_getBE(), 4);
            cBENewRaceInterface* nr = (cBENewRaceInterface*)cBE_getInterface_Fv(cBE_getBE(), 0);
            void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBENewRaceInterface_setGameMode(nr, 4);
            cBENewRaceInterface_setGameEvent(nr, 12);
            cBENewRaceInterface_newRace.iLocationIdx = cBENewPlayerInterface_getLastHub(pl, 0);
            cBENewRaceInterface_bIsBamLocation = 1;
            sVEntry0GNS* vt = (sVEntry0GNS*)nr->mVtbl;
            vt[1].fn(vthis(nr, vt[1]));
            cAppMan_setNextModule((cAppMan*)SSXApp, cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
            break;
        }
        case 1:
            return cFEStateCharEquip_cFEStateCharEquip(cMemMan_alloc(2800, str_cFEStateCharEquip, 0, 0), *self, 0, 0, 1);
        case 2:
            return cFEStateCharEquip_cFEStateCharEquip(cMemMan_alloc(2800, str_cFEStateCharEquip, 0, 0), *self, 0, 1, 1);
        case 3:
            return cFEStateBuyAttrib_cFEStateBuyAttrib(cMemMan_alloc(176, str_cFEStateBuyAttrib, 0, 0), *self);
        case 4:
            return cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cMemMan_alloc(76, str_cFEStateLodgeRiderDetail, 0, 0), *self);
        case 5:
            return cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, str_cFEStateAudioOptions, 0, 0), *self, 0, 1);
        case 7:
            break;
        case 6:
            gInLodge = 0;
            cBE_getInterface_Fv(cBE_getBE(), 4);
            return cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle, 0, 0), *self);
        }
        break;
    case 40:
        switch (pData) {
        case 9:
            return cFEStateRewardsRoom_cFEStateRewardsRoom(cMemMan_alloc(80, str_cFEStateRewardsRoom, 0, 0), *self, st->x44, 1);
        case 10:
            return cFEStateMountainRoom_cFEStateMountainRoom(cMemMan_alloc(200, str_cFEStateMountainRoom, 0, 0), *self, st->x44);
        case 12:
            return cFEStateUberTrick_cFEStateUberTrick(cMemMan_alloc(0x60, str_cFEStateUberTrick, 0, 0), *self, st->x44);
        case 13:
            return cFEStateCareerStats_cFEStateCareerStats(cMemMan_alloc(0x78, str_cFEStateCareerStats, 0, 0), *self);
        case 15:
            return cFEStateRiderBio_cFEStateRiderBio(cMemMan_alloc(80, str_cFEStateRiderBio, 0, 0), *self, st->x44);
        }
        break;
    case 43:
        return cFEStatePeakRoom_cFEStatePeakRoom(cMemMan_alloc(228, str_cFEStatePeakRoom, 0, 0), *self, st->x44, pData);
    case 44:
        return cFEStateTrophyRoom_cFEStateTrophyRoom(cMemMan_alloc(160, str_cFEStateTrophyRoom, 0, 0), *self, st->x44, pData >> 16, pData & 0xFFFF);
    case 46:
        switch (pData) {
        case 3:
            return cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters(cMemMan_alloc(2772, str_cFEStateRewardGalleryPosters, 0, 0), *self, st->x44, gInLodge);
        case 4:
            return cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards(cMemMan_alloc(3064, str_cFEStateRewardGalleryTradingCards, 0, 0), *self, st->x44, gInLodge);
        case 5:
            return cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts(cMemMan_alloc(3000, str_cFEStateRewardGalleryConceptArts, 0, 0), *self, st->x44, gInLodge);
        case 7:
            return cFEStateRewardGalleryToys_cFEStateRewardGalleryToys(cMemMan_alloc(2712, str_cFEStateRewardGalleryToys, 0, 0), *self, st->x44, gInLodge);
        case 6:
            return cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos(cMemMan_alloc(2608, str_cFEStateRewardGalleryVideos, 0, 0), *self, st->x44, gInLodge);
        case 8:
            return cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars(cMemMan_alloc(2712, str_cFEStateRewardGalleryCheatChars, 0, 0), *self, st->x44, gInLodge);
        }
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFECustom_getPrevState);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cAppMan_setNextModule(struct cAppMan* app, unsigned int module);
unsigned int cPreGameLoadScreen_cPreGameLoadScreen(void* mem);
extern "C" void* cFEStateCharSelect_cFEStateCharSelect(void* mem, void* owner, int a2);
extern "C" void* cFEStateCharSetup_cFEStateCharSetup(void* mem, void* owner, int a2);
extern "C" void* cFEStateRiderDetail_cFEStateRiderDetail(void* mem, void* owner, int a2);
extern "C" void* cFEStateEventSelect_cFEStateEventSelect1(void* mem, void* owner, int a2);
extern "C" void* func_001887A0(void* mem, void* owner, int a2);
extern "C" void* cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(void* mem, void* owner);
extern "C" void* cFEStateTitle_cFEStateTitle1(void* self, void* owner);
extern "C" void* cFEStateMainMenu_cFEStateMainMenu1(void* mem, void* owner);
extern "C" void* cFEStateAudioOptions_cFEStateAudioOptions(void* mem, void* owner, int a2, int a3);
extern "C" void* cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(void* mem, void* owner);
extern "C" void* cFEStateNetSSXWorld_cFEStateNetSSXWorld(void* mem, void* owner);
extern "C" void* cFEStateMountainRoom_cFEStateMountainRoom(void* mem, void* owner, int a2);
extern "C" void* cFEStatePeakRoom_cFEStatePeakRoom(void* mem, void* owner, int a2, int a3);
extern "C" void* cFEStateRewardsRoom_cFEStateRewardsRoom(void* mem, void* owner, int a2, int a3);
extern "C" void* cFEStateLodge_cFEStateLodge(void* self, void* owner);
extern "C" void* cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(void* mem, void* owner);
extern int gInLodge;
extern int D_004A1A70;
extern int cUITemplate_MAP_mMapStyle;
extern int cUITemplate_MAP_mMapFEStart;
extern char str_cPreGameLoadScreen[];
extern char str_cFEStateTitle[];
extern char str_cFEStateOnlineMainMenu[];
extern char str_cFEStateAudioOptions[];
extern char str_cFEStateMainMenu[];
extern char str_cFEStateCharSelect[];
extern char str_cFEStateCharSetup[];
extern char str_cFEStateEventSelect[];
extern char str_cFEStateRiderDetail[];
extern char str_cFEStateRewardsRoom[];
extern char str_cFEStateOptionsSaveLoad[];
extern char str_cFEStateLodgeRiderDetail[];
extern char str_cFEStateMountainRoom[];
extern char str_cFEStatePeakRoom[];
extern char str_cFEStateLodge[];
extern char str_cFEStateOptions[];
extern char str_cFEStateNetSSXWorld[];

static inline bool IsMode1_1A16C0()
{
    return D_004A1A70 == 1;
}

/**
 * Builds the state to go back to from the current front-end state (cFECustom_vtbl entry 5, getPrevState).
 *
 * Switches on the current state's id (+0xC) and returns a newly allocated (cMemMan_alloc) and constructed state: the
 * main menu, character select or character setup by the new race's cGamePath (newRace.cGamePath) and the user number (+0x44);
 * the lodge or lodge rider detail instead of character setup / rider detail while gInLodge is set; audio options;
 * event select (clearing cUITemplate_MAP_mMapStyle, setting cUITemplate_MAP_mMapFEStart); options; options save/load; title; mountain,
 * peak and rewards rooms; the online main menu or SSX world when D_004A1A70 is 1. For state 35 and out-of-range paths
 * it goes by the custom interface's +0xC (24 options save/load, 9 character select, 39 lodge). State 33 with @p stateData 1
 * sets the next app module to a new cPreGameLoadScreen and returns 0.
 *
 * @param desc  the current state
 * @param stateData   the state data passed along (picks the branch for state 33; the peak and rewards rooms get it)
 * @return the new state, or 0 when there is none
 */
extern "C" void* cFECustom_getPrevState(void** self, cUIState* desc, int stateData)
{
    switch (desc->mHashName) {
    case 9:
        if (IsMode1_1A16C0()) {
            return cFEStateOnlineMainMenu_cFEStateOnlineMainMenu(cMemMan_alloc(1744, str_cFEStateOnlineMainMenu, 0, 0), *self);
        }
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (cBENewRaceInterface_newRace.cGamePath) {
        case 1:
            return cFEStateMainMenu_cFEStateMainMenu1(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), *self);
        case 0:
            return cFEStateMainMenu_cFEStateMainMenu1(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), *self);
        case 2:
            if (*(char*)&desc->mUserNum == 1) {
                return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, 0);
            }
            return cFEStateMainMenu_cFEStateMainMenu1(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), *self);
        }
        break;
    case 23:
        return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, 1);
    case 13:
    case 14:
    case 16:
        if (gInLodge != 0) {
            return cFEStateLodge_cFEStateLodge(cMemMan_alloc(88, str_cFEStateLodge, 0, 0), *self);
        }
        return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, *(char*)&desc->mUserNum);
    case 15:
    case 18:
        if (gInLodge != 0) {
            return cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cMemMan_alloc(76, str_cFEStateLodgeRiderDetail, 0, 0), *self);
        }
        return cFEStateRiderDetail_cFEStateRiderDetail(cMemMan_alloc(84, str_cFEStateRiderDetail, 0, 0), *self, *(char*)&desc->mUserNum);
    case 17:
        return cFEStateAudioOptions_cFEStateAudioOptions(cMemMan_alloc(0x80, str_cFEStateAudioOptions, 0, 0), *self, *(char*)&desc->mUserNum, gInLodge);
    case 19:
        cBE_getInterface_Fv(cBE_getBE(), 0);
        switch (cBENewRaceInterface_newRace.cGamePath) {
        case 0:
        case 1:
            return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, 0);
        case 2:
            return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, 1);
        }
        break;
    case 20: {
        cUITemplate_MAP_mMapFEStart = 1;
        cUITemplate_MAP_mMapStyle = 0;
        char* eventSelect = (char*)cFEStateEventSelect_cFEStateEventSelect1(cMemMan_alloc(828, str_cFEStateEventSelect, 0, 0), *self, 0);
        eventSelect[0x44] = 0;
        return eventSelect;
    }
    case 21:
        cUITemplate_MAP_mMapFEStart = 1;
        cUITemplate_MAP_mMapStyle = 0;
        return cFEStateEventSelect_cFEStateEventSelect1(cMemMan_alloc(828, str_cFEStateEventSelect, 0, 0), *self, 0);
    case 12:
        return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, *(char*)&desc->mUserNum);
    case 8: {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        int gamePath = cBENewRaceInterface_newRace.cGamePath;
        if (gamePath >= 3) {
            goto other;
        }
        if (gamePath < 0) {
            goto other;
        }
    }
    case 5:
    case 11:
    case 22:
        return cFEStateMainMenu_cFEStateMainMenu1(cMemMan_alloc(88, str_cFEStateMainMenu, 0, 0), *self);
    case 35:
    other: {
        int sc = ((int*)self)[3];
        if (sc == 24) {
            return cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(cMemMan_alloc(76, str_cFEStateOptionsSaveLoad, 0, 0), *self);
        } else if (sc == 9) {
            return cFEStateCharSelect_cFEStateCharSelect(cMemMan_alloc(144, str_cFEStateCharSelect, 0, 0), *self, *(char*)&desc->mUserNum);
        } else if (sc == 39) {
            return cFEStateLodge_cFEStateLodge(cMemMan_alloc(88, str_cFEStateLodge, 0, 0), *self);
        }
        break;
    }
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 36:
    case 37:
    case 38:
        return func_001887A0(cMemMan_alloc(84, str_cFEStateOptions, 0, 0), *self, 0);
    case 33:
        switch (stateData) {
        case 0:
            if (((int*)self)[3] == 34) {
                return func_001887A0(cMemMan_alloc(84, str_cFEStateOptions, 0, 0), *self, 0);
            }
            return cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(cMemMan_alloc(76, str_cFEStateOptionsSaveLoad, 0, 0), *self);
        case 1:
            cAppMan_setNextModule((struct cAppMan*)SSXApp, cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
            break;
        case 2:
            if (IsMode1_1A16C0()) {
                return cFEStateNetSSXWorld_cFEStateNetSSXWorld(cMemMan_alloc(1744, str_cFEStateNetSSXWorld, 0, 0), *self);
            }
            break;
        }
        break;
    case 2:
    case 39:
        return cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle, 0, 0), *self);
    case 40:
    case 41:
        return cFEStateLodge_cFEStateLodge(cMemMan_alloc(88, str_cFEStateLodge, 0, 0), *self);
    case 43:
        if (gInLodge != 0) {
            return cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cMemMan_alloc(76, str_cFEStateLodgeRiderDetail, 0, 0), *self);
        }
        return cFEStateCharSetup_cFEStateCharSetup(cMemMan_alloc(80, str_cFEStateCharSetup, 0, 0), *self, *(char*)&desc->mUserNum);
    case 42:
        return cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cMemMan_alloc(76, str_cFEStateLodgeRiderDetail, 0, 0), *self);
    case 46:
        if (gInLodge != 0) {
            return cFEStateLodgeRiderDetail_cFEStateLodgeRiderDetail(cMemMan_alloc(76, str_cFEStateLodgeRiderDetail, 0, 0), *self);
        }
        return cFEStateRiderDetail_cFEStateRiderDetail(cMemMan_alloc(84, str_cFEStateRiderDetail, 0, 0), *self, *(char*)&desc->mUserNum);
    case 44:
        return cFEStateMountainRoom_cFEStateMountainRoom(cMemMan_alloc(200, str_cFEStateMountainRoom, 0, 0), *self, *(char*)&desc->mUserNum);
    case 45:
        return cFEStatePeakRoom_cFEStatePeakRoom(cMemMan_alloc(228, str_cFEStatePeakRoom, 0, 0), *self, *(char*)&desc->mUserNum, stateData);
    case 47:
        return cFEStateRewardsRoom_cFEStateRewardsRoom(cMemMan_alloc(80, str_cFEStateRewardsRoom, 0, 0), *self, *(char*)&desc->mUserNum, stateData != 0);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFECustom_setPlayerCtrlMask);
#ifdef SKIP_ASM
/**
 * Stores the control mask byte of player @p index (+0x8 + index of the front end's custom interface).
 *
 * @param index  the player (a signed char)
 * @param val    the mask
 */
extern "C" void cFECustom_setPlayerCtrlMask(cFECustom* self, int index, int val)
{
    char* entry = (char*)self + (signed char)index;
    *(char*)(entry + 0x8) = val;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFECustom_getPlayerCtrlMask);
#ifdef SKIP_ASM
/**
 * Returns the control mask byte of player @p index (+0x8 + index of the front end's custom interface).
 *
 * State constructors copy it into the new state's mCtrlBits (+0x15).
 *
 * @param index  the player (a signed char)
 * @return the mask
 */
extern "C" unsigned char cFECustom_getPlayerCtrlMask(cFECustom* self, int index)
{
    char* entry = (char*)self + (signed char)index;
    return *(unsigned char*)(entry + 0x8);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_cRealFE);
#ifdef SKIP_ASM
extern "C" void* cFE_cFE(void* self);
void cConnMan_cConnMan(void* p);
void cGameConnMan_cGameConnMan(void* p);
extern void* cRealFE_vtbl[];

/**
 * Constructs the front-end module.
 *
 * Runs cFE_cFE, stores cRealFE_vtbl, constructs the connection managers at +0xB5AB0 (cConnMan_cConnMan) and +0xB5ABC
 * (cGameConnMan_cGameConnMan) and zeroes +0xB5AD4, +0xB5AD8 and +0xB5ADC.
 *
 * @return @p self
 */
extern "C" void* cRealFE_cRealFE(cRealFE* self)
{
    cFE_cFE(self);
    *(void***)self = cRealFE_vtbl;
    cConnMan_cConnMan(self->mConnMan);
    cGameConnMan_cGameConnMan(self->mGameConnMan);
    self->mpMoviePlayer = 0;
    self->mMovieSkippable = 0;
    self->mWaitRenderNum = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE__cRealFE);
#ifdef SKIP_ASM
extern void* cRealFE_vtbl[];
extern "C" void cGameConnMan__cGameConnMan(void* self, int flags);
extern "C" void cConnMan__cConnMan(void* self, int flags);
extern "C" void cFE__cFE(void* self, int flags);

/**
 * Destroys the front-end module.
 *
 * Restores cRealFE_vtbl, destroys the members at +0xB5ABC and +0xB5AB0 (cGameConnMan__cGameConnMan and cConnMan__cConnMan, each with
 * flags 2: destroy without freeing), then runs the base-class destructor cFE__cFE with @p flags, which frees
 * the object when bit 0 is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cRealFE__cRealFE(cRealFE* self, int flags)
{
    *(void***)self = cRealFE_vtbl;
    cGameConnMan__cGameConnMan(self->mGameConnMan, 2);
    cConnMan__cConnMan(self->mConnMan, 2);
    cFE__cFE(self, flags);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_gainFocus);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* cBE_getBE();
extern "C" void cBE_newProfile(void* be, int b);
extern "C" void* MEM_fill(void* p, int c, int n);
extern "C" void cBE_setState(int state);
extern "C" void cGraphicsMan_UploadTextures(void* p);
extern "C" void cFELocale_unloadOV(void* p);
extern "C" void cFEFlowState_setNumStates(void* flow, int n);
extern char gFlowState;
extern int gWeHaveToClearTheBackEnd;
extern int cDepthFog_m_Enable;
extern int cScreenTint_m_Enable;
extern int cLightGlow_m_Enable;

struct sVec3K1A1DC0 {
    int x, y, z;
    sVec3K1A1DC0() { x = 0; y = 0; z = 0; }
};
typedef void (*tFn_1A1DC0)(void*, const sVec3K1A1DC0&);
typedef void (*tVoid_1A1DC0)(void*);

/**
 * Prepares the front end when it gains focus and pushes its first states.
 *
 * If gWeHaveToClearTheBackEnd is set, it resets back-end entries 0-2 (cBE_newProfile), zeroes the 0x20 bytes at
 * cBERules_mRules and clears the flag. It then clears +0xB5AD8 and +0xB5ADC, sets back-end state 0 (cBE_setState()),
 * uploads the textures (cGraphicsMan_UploadTextures()), calls cFELocale_unloadOV on the front-end locale, turns off depth
 * fog, screen tint and light glow, sets the graphics manager's clear colour to black (slot 15 with (0, 0, 0)), gives
 * the front-end flow state gFlowState 86 states and finally calls its own virtual slot 8 (cRealFE_loadStartState()).
 */
extern "C" void cRealFE_gainFocus(cRealFE* self)
{
    char* graphicsMan;
    char* vt;
    if (gWeHaveToClearTheBackEnd != 0) {
        cBE_newProfile(cBE_getBE(), 0);
        cBE_newProfile(cBE_getBE(), 1);
        cBE_newProfile(cBE_getBE(), 2);
        MEM_fill(((char*)&cBERules_mRules), 0, 0x20);
        gWeHaveToClearTheBackEnd = 0;
    }
    self->mMovieSkippable = 0;
    self->mWaitRenderNum = 0;
    cBE_setState(0);
    cGraphicsMan_UploadTextures(GrMan);
    cFELocale_unloadOV(SSXApp->FELocale);
    cDepthFog_m_Enable = 0;
    cScreenTint_m_Enable = 0;
    cLightGlow_m_Enable = 0;
    graphicsMan = (char*)GrMan;
    vt = *(char**)(graphicsMan + 0x10D8);
    ((tFn_1A1DC0)*(void**)(vt + 0x7C))(graphicsMan + *(short*)(vt + 0x78), sVec3K1A1DC0());
    cFEFlowState_setNumStates(&gFlowState, 86);
    ((tVoid_1A1DC0)*(void**)(*(char**)self + 0x44))(((char*)self) + *(short*)(*(char**)self + 0x40));
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_load);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cLoadTimer_startload(void* self, float t);
void cLoadTimer_setnextpercent(void* self, float t);
void func_0014BD78(void* iface);
extern "C" void func_0014AF10(void* iface);
extern "C" void cFELocale_loadFE(void* self);
extern "C" void cUIEngine_loadFile(void* engine, const char* path);
// PORT: the unit declares cRealFE_loadCharAnimations without parameters; this caller passes self.
extern "C" void cRealFE_loadCharAnimations_1(void* self) __asm__("cRealFE_loadCharAnimations");
extern "C" void func_0019CE68(void* self);
extern "C" void cFERiderManager_init(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFECustom_cFECustom(cFECustom* self, cUIEngine* owner);
extern "C" void* cFESound_cFESound(cFESound* self);
extern "C" void cIrradianceDataBase_Load(void* db);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_FrontEndLoad(void* snd);
extern char str_data_ui_fe[];
extern char str_FEUIInput[];
extern char str_FEUICustom[];
extern char str_FEUISound[];
extern void* cFEInput_vtbl[];
extern char cIrradianceDataBase_TheDataBase[];

struct sStr11_1E80 { char c[11]; };



class cCamObj_1E80 {
public:
    char pad0[0x90];
    virtual void v01();
    virtual void v02();
    virtual void update();
};


struct sUI_1E80 {
    void* custom;                   // 0x0
    char pad4[0x4];
    void* f8;                       // 0x8
    void* input;                    // 0xC
    void* f10;                      // 0x10
    void* sound;                    // 0x14
};


static inline void progress_1E80(float t)
{
    cLoadTimer_setnextpercent(&cAppMan_TheAppMan->LoadTimer, t);
}

/**
 * Loads the front end: UI files, character animations, riders, camera, input and sound.
 *
 * Advances the loading bar (20, 50, 75, 99) as it goes. It calls func_0014BD78 and func_0014AF10 on BE
 * interface 9 and cFELocale_loadFE on the front-end locale, calls the graphics manager's slot 44 with 0, loads
 * "data/ui/fe" into the UI engine and the character animations, and calls func_0019CE68 on +0x1A70 and
 * cFERiderManager_init on the rider manager (+0xB0). The camera gets (0, 0, -195) and (0, 1, 0) and is updated. The
 * engine gets a cFEInput ("FEUIInput"), a cFECustom ("FEUICustom"), a sound object ("FEUISound"), the app's
 * fonts and locale; finally it loads the irradiance database and calls cSSXAudio_FrontEndLoad(cAudioInterface_bxAudio()).
 */
// Front end: loads the UI files, rider data and sound for the front end.
extern "C" void cRealFE_load(cRealFE* self)
{
    cLoadTimer_startload(&cAppMan_TheAppMan->LoadTimer, 3.5f);
    progress_1E80(20.0f);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 9);
    func_0014BD78(iface);
    func_0014AF10(iface);
    cFELocale_loadFE(SSXApp->FELocale);
    progress_1E80(50.0f);
    {
        cPS2GraphicsMan* g = GrMan;
        g->InitTextures(0);
    }
    sUI_1E80* ui = ((sUI_1E80*)self->mpUI);
    // Looks like a macro expansion: a local copy of the path plus the load call.
    do {
        sStr11_1E80 path = *(sStr11_1E80*)str_data_ui_fe;
        cUIEngine_loadFile(ui, str_data_ui_fe);
    } while (0);
    progress_1E80(75.0f);
    cRealFE_loadCharAnimations_1(self);
    func_0019CE68(self->mBoltFileManager);
    cFERiderManager_init(&self->mRiderManager);
    cFE* cam = SSXApp->RealFE;
    t4Vector pos;
    t4Vector up;
    t4Vector scratchVec;
    scratchVec.x = 0.0f;
    scratchVec.y = 0.0f;
    scratchVec.z = -195.0f;
    scratchVec.w = 0.0f;
    pos = scratchVec;
    scratchVec.w = 1.0f;
    scratchVec.x = 0.0f;
    scratchVec.y = 1.0f;
    scratchVec.z = 0.0f;
    up = scratchVec;
    *(t4Vector*)&cam->mCamera.Pose = pos;
    *(&cam->mCamera.Pose.Q) = up;
    ((cCamObj_1E80*)(&cam->mCamera))->update();
    progress_1E80(99.0f);
    cFEInput* in = (cFEInput*)cMemMan_alloc(0x1C, str_FEUIInput, 0, 0);
    sUI_1E80* ui2 = ((sUI_1E80*)self->mpUI);
    in->mPadNum = 0;
    in->mVtbl = cFEInput_vtbl;
    in->mpInput = 0;
    in->mbKeyReady = 0;
    in->miVKey = 0;
    in->miAsciiKey = 0;
    ui2->input = ((char*)in);
    ((sUI_1E80*)self->mpUI)->custom = cFECustom_cFECustom((cFECustom*)cMemMan_alloc(20, str_FEUICustom, 0, 0), (cUIEngine*)((sUI_1E80*)self->mpUI));
    ((sUI_1E80*)self->mpUI)->sound = cFESound_cFESound((cFESound*)cMemMan_alloc(8, str_FEUISound, 0, 0));
    char* g = (char*)SSXApp;
    void* feLocale = *(void**)(g + 0x8C);
    ((sUI_1E80*)self->mpUI)->f10 = feLocale;
    void* fontInterface = *(void**)(g + 0x88);
    ((sUI_1E80*)self->mpUI)->f8 = fontInterface;
    cIrradianceDataBase_Load(cIrradianceDataBase_TheDataBase);
    cSSXAudio_FrontEndLoad(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_loseFocus);
#ifdef SKIP_ASM
extern "C" void* cFadeScreen_loseFocus(void* self);
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int a1);
extern "C" void cFEFlowState_purge(void* flow);
extern int cRealFE_mMovieList;
extern char gFlowState;

/**
 * Lets the front end lose focus once the base fade allows it, stopping any movie that is playing.
 *
 * Returns 0 while cFadeScreen_loseFocus() still refuses. Otherwise it destroys the movie player at +0xB5AD4 (On Tour
 * mpMoviePlayer; cMoviePlayer__cMoviePlayer with flags 3) and clears it, clears +0xB5AD8, cRealFE_mMovieList and +0xB5ADC, calls
 * cFEFlowState_purge on the front-end flow state gFlowState and returns 1.
 *
 * @return 1 when focus may move on, 0 to keep it
 */
extern "C" int cRealFE_loseFocus(cRealFE* self)
{
    if (cFadeScreen_loseFocus(self) == 0) {
        return 0;
    }
    void* moviePlayer = self->mpMoviePlayer;
    if (moviePlayer != 0) {
        cMoviePlayer__cMoviePlayer(moviePlayer, 3);
        self->mpMoviePlayer = 0;
    }
    self->mMovieSkippable = 0;
    cRealFE_mMovieList = 0;
    self->mWaitRenderNum = 0;
    cFEFlowState_purge(&gFlowState);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_purge);
#ifdef SKIP_ASM
extern char cIrradianceDataBase_TheDataBase[];
extern "C" void cRealFE_purgeCharAnimations(cRealFE* self);
extern "C" void cFERiderManager_purge(void* p);
extern "C" void func_0019CF40(void* p);
extern "C" void cIrradianceDataBase_Purge(void* p);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_FrontEndPurge(void* p);

/**
 * Unloads the front end: flushes the renderer, then frees the character animations, front-end data and audio.
 *
 * Calls the graphics manager's slot 19 (cGraphicsMan_FlushRenderer) first, then cRealFE_purgeCharAnimations (releases the animation
 * data sources that cRealFE_loadCharAnimations() registered), cFERiderManager_purge on the rider manager (+0xB0),
 * func_0019CF40 on +0x1A70 and cIrradianceDataBase_Purge on cIrradianceDataBase_TheDataBase, clears the app's words at +0x7C and +0x80 and finally
 * calls cSSXAudio_FrontEndPurge() on the audio object.
 */
extern "C" void cRealFE_purge(cRealFE* self)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    ps2GraphicsMan->FlushRenderer();
    cRealFE_purgeCharAnimations(self);
    cFERiderManager_purge(&self->mRiderManager);
    func_0019CF40(self->mBoltFileManager);
    cIrradianceDataBase_Purge(cIrradianceDataBase_TheDataBase);
    cSSXApp* st = SSXApp;
    st->RealFE = 0;
    st->DebugFE = 0;
    cSSXAudio_FrontEndPurge(cAudioInterface_bxAudio());
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_render);
#ifdef SKIP_ASM
extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void cMoviePlayer_asyncRender(void* h, int a1);
extern "C" void cUIEngine_render3D(void* ui);
extern "C" void cFERiderManager_render(void* p);
extern "C" void cUIEngine_render2D(void* ui);
extern "C" void cSSXAudio_Update(void* snd, int a1, float t);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern int cRealFE_mMovieList;
extern char t4mat_Identity[];

struct sFeGs {
    int w0;                         // 0x0
    unsigned int g0 : 2;            // 0x4
    unsigned int g2 : 5;
    unsigned int g7 : 5;
    unsigned int g12 : 8;
    unsigned int g20 : 2;
    unsigned int g22 : 1;
    unsigned int g23 : 2;
    unsigned int g25 : 7;
    unsigned int a0 : 5;            // 0x8
    unsigned int alpha : 5;
    unsigned int a10 : 22;
    int wC;                         // 0xC
    short tex;                      // 0x10
    short pad;

    void setG2(int v) { g2 = v; }
    void setG12(int v) { g12 = v; }
    void setG20(int v) { g20 = v; }
    void setG22(int v) { g22 = v; }
    void setG23(int v) { g23 = v; }
    void setAlpha(int v) { alpha = v; }
};

extern sFeGs sDrawState_s_defaultDrawState;

struct sFeVE {
    short delta;
    short index;
    void* fn;
};

struct sFeWorld {
    char pad_0x0[0xE84];
    sFeGs* top;                     // 0xE84
    char pad_0xE88[0x10D8 - 0xE88];
    sFeVE* vt;                      // 0x10D8
};

struct sFePos {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sFeVert {
    float u, v, q, f0C;             // 0x00
    int r, g, b, a;                 // 0x10
    sFePos pos;                     // 0x20
    sFeVert() {}
};

static inline sFeWorld* feW() { return (sFeWorld*)GrMan; }

static inline void feCall(int slot)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
    ((void (*)(void*))vt[slot].fn)(vthis(ps2GraphicsMan, vt[slot]));
}

static inline unsigned int feCallU(int slot)
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
    return ((unsigned int (*)(void*))vt[slot].fn)(vthis(ps2GraphicsMan, vt[slot]));
}

/**
 * Draws the front end: a black backdrop while a movie is queued, otherwise the 3D scene and the UI.
 *
 * Returns 0 when the graphics manager's slot 17 (IsRenderReady) says no. Counts +0xB5ADC down. If cFadeScreen_drawFade (the
 * base fade) returns non-zero, or a movie player exists (+0xB5AD4; drawn with cMoviePlayer_asyncRender), it flips the frame (slot
 * 20) and returns 1. While cRealFE_mMovieList is non-zero it draws a full-screen black quad in an orthographic viewport
 * (slot 26) as a 4-vertex tri strip (slot 71). Otherwise it sets the perspective from the camera (+0x10, +0x14, +0x18
 * to slot 25) and the view matrix (+0x50 to slot 34), draws the scene with a fixed draw state (cUIEngine_render3D on the UI
 * engine at +0xC, cFERiderManager_render on the rider manager at +0xB0), then the UI (cUIEngine_render2D), flips the frame and calls
 * cSSXAudio_Update on the audio object with 0.02.
 *
 * @return 1 when a frame was drawn, 0 when the renderer was not ready
 */
extern "C" int cRealFE_render(cRealFE* self)
{
    if (feCallU(17) == 0) {
        return 0;
    }
    if (self->mWaitRenderNum != 0) {
        self->mWaitRenderNum -= 1;
    }
    if (cFadeScreen_drawFade(self) != 0) {
        feCall(20);
        return 1;
    }
    void* moviePlayer = self->mpMoviePlayer;
    if (moviePlayer != 0) {
        cMoviePlayer_asyncRender(moviePlayer, 0);
        feCall(20);
        return 1;
    }
    if (cRealFE_mMovieList != 0) {
        float w = (float)feCallU(10);
        float hgt = (float)feCallU(11);
        feCall(21);
        feCall(31);
        {
            sFeWorld* w = feW();
            w->top[1] = w->top[0];
            w->top++;
        }
        {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
            ((void (*)(void*, int, int, float, float, float, float, float, float))vt[26].fn)(
                vthis(ps2GraphicsMan, vt[26]), 0, 0, 0.0f, 0.0f, w, hgt, 0.0f, 1.0f);
        }
        {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
            ((void (*)(void*, void*))vt[34].fn)(vthis(ps2GraphicsMan, vt[34]), t4mat_Identity);
        }
        feW()->top->setAlpha(9);
        sFeVert verts[4];
        for (int i = 0; i < 4; i++) {
            verts[i].a = 0x80;
            verts[i].r = 0;
            verts[i].g = 0;
            verts[i].b = 0;
            verts[i].q = 1.0f;
        }
        sFePos corner;
        corner.x = 0.0f;
        corner.y = 0.0f;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[0].pos = corner;
        corner.x = 0.0f;
        corner.y = w;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[1].pos = corner;
        corner.x = 0.0f;
        corner.y = 0.0f;
        corner.z = hgt;
        corner.w = 1.0f;
        verts[2].pos = corner;
        corner.x = 0.0f;
        corner.y = w;
        corner.z = hgt;
        corner.w = 1.0f;
        verts[3].pos = corner;
        *(short*)((char*)GrMan->m_pCurrentDrawState + 0x10) = -1;
        {
            cPS2GraphicsMan* ps2GraphicsMan = GrMan;
            sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
            ((void (*)(void*, int, sFeVert*, int))vt[71].fn)(vthis(ps2GraphicsMan, vt[71]), 4, verts, 0);
        }
        feCall(22);
        feCall(32);
        feW()->top--;
        feCall(20);
        return 1;
    }
    feCall(21);
    feCall(31);
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
        ((void (*)(void*, float, float, float))vt[25].fn)(vthis(ps2GraphicsMan, vt[25]),
            *(float*)((char*)self + 0x10), *(float*)((char*)self + 0x14), *(float*)((char*)self + 0x18));
    }
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sFeVE* vt = (sFeVE*)VPTR_AT(ps2GraphicsMan, 0x10D8);
        ((void (*)(void*, void*))vt[34].fn)(vthis(ps2GraphicsMan, vt[34]), (char*)self + 0x50);
    }
    {
        sFeWorld* w = feW();
        w->top[1] = w->top[0];
        w->top++;
    }
    *feW()->top = sDrawState_s_defaultDrawState;
    feW()->top->setG22(0);
    feW()->top->setG23(1);
    feW()->top->setG20(3);
    feW()->top->setG12(13);
    feW()->top->setAlpha(21);
    feW()->top->setG2(5);
    cUIEngine_render3D(self->mpUI);
    cFERiderManager_render((char*)self + 0xB0);
    feW()->top--;
    feCall(32);
    feCall(22);
    cUIEngine_render2D(self->mpUI);
    feCall(20);
    cSSXAudio_Update(cAudioInterface_bxAudio(), 0, 0.019999999552965164f);
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_update);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cFadeScreen_update(void* self);
extern "C" void* cMoviePlayer_cMoviePlayer(void* mem);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_002B3A70(void* p);
extern "C" void func_002B3A98(void* p);
extern "C" void tMoviePlayOptions_SetDefaults(void* self);
extern "C" int cMoviePlayer_init(void* p, void* q);
extern "C" int cMoviePlayer_asyncPlay(void* self);
extern "C" void cMoviePlayer_asyncUpdate(void* self);
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int a1);
int cSSXAudio_PlayIntroMovieWithDJ(void* self);
void* cInput_getState(void* self, int a1);
extern "C" void cUIEngine_update(void* self);
extern "C" void func_0019D250(void* p);
extern "C" void cFERiderManager_update(void* p);
extern "C" void cFEAsyncManager_Update(void* p);
extern "C" void func_00263828(void* p);
extern "C" void func_00263CF0(void* p);
extern "C" void func_0025AC50(void* p);
extern const char str_MoviePlayer[];
extern int cRealFE_mMovieList;
extern int D_004A19D0;
extern int D_004A19E0;
extern int D_004A19E4;
extern int D_00441248[];
extern int D_0044124C[];
extern int D_00441250[];
extern int D_00441254[];
extern unsigned int cBEOptionInterface_mOption[];
extern int EnableOnline;


/**
 * Per-frame update of the front end: plays a queued movie, else updates the UI, camera, riders and online managers.
 *
 * Does nothing while +0xB5ADC (a frame counter that cRealFE_render() counts down) is set; then calls
 * cFadeScreen_update(). If there is no movie player (+0xB5AD4) and cRealFE_mMovieList has a bit set, it creates one
 * ("MoviePlayer", 68 bytes) and starts the movie of the lowest set bit (1, 2 or 4; for 4 the file depends on the BE
 * option bits and cSSXAudio_PlayIntroMovieWithDJ), clearing that bit and recording it in D_004A19D0; bit 4 also makes the movie
 * skippable (+0xB5AD8). While a movie plays it runs cMoviePlayer_asyncUpdate() and, when skippable, ends it on a
 * press of input 0x70 or 122 on either pad; when it ends the player is destroyed, +0xB5ADC is set to 1 and, if no
 * movie is left, func_002B3A98 is called on the audio object (+0x118). Without a movie it updates the UI engine (+0xC,
 * cUIEngine_update; or sets +0x8 when SSXApp +0x4 is set), the camera at +0x10 (its virtual slot 3), func_0019D250 on
 * +0x1A70, the rider manager (+0xB0, cFERiderManager_update) and the async file manager, and, when EnableOnline is set, the
 * online members at +0xB5AB0 and +0xB5ABC and the lobby manager.
 */
extern "C" void cRealFE_update(cRealFE* self)
{
    if (self->mWaitRenderNum != 0) return;
    cFadeScreen_update(((char*)self));
    if (self->mpMoviePlayer == 0) {
        if (cRealFE_mMovieList != 0) {
            self->mpMoviePlayer = (cMoviePlayer*)cMoviePlayer_cMoviePlayer(cMemMan_alloc(68, str_MoviePlayer, 0x100, 0));
            func_002B3A70(&cAudioInterface_bxAudio()->MusicSys);
            tMoviePlayOptions info;
            tMoviePlayOptions_SetDefaults(&info);
            int movieBits = cRealFE_mMovieList;
            self->mMovieSkippable = 0;
            if (movieBits & 1) {
                (*(int*)&info.sVideoFileName) = D_00441248[0];
                cRealFE_mMovieList = movieBits & 0xFFFFFFFE;
                D_004A19D0 = 1;
            } else if (movieBits & 2) {
                (*(int*)&info.sVideoFileName) = D_0044124C[0];
                cRealFE_mMovieList = movieBits & 0xFFFFFFFD;
                D_004A19D0 = 2;
            } else if (movieBits & 4) {
                cBE_getInterface_Fv(cBE_getBE(), 4);
                if (((cBEOptionInterface_mOption[0] >> 22) & 7) == 0 && cSSXAudio_PlayIntroMovieWithDJ(cAudioInterface_bxAudio()) != 0) {
                    (*(int*)&info.sAudioFileName) = D_004A19E0;
                    (*(int*)&info.sVideoFileName) = D_00441250[0];
                } else {
                    (*(int*)&info.sAudioFileName) = D_004A19E4;
                    (*(int*)&info.sVideoFileName) = D_00441254[0];
                }
                cRealFE_mMovieList &= 0xFFFFFFFB;
                D_004A19D0 = 4;
                self->mMovieSkippable = 1;
            }
            info.bSoundOn = 1;
            cMoviePlayer_init(self->mpMoviePlayer, &info);
            cMoviePlayer_asyncPlay(self->mpMoviePlayer);
        }
        if (self->mpMoviePlayer == 0) goto noh;
    }
    {
    int done = *(int*)self->mpMoviePlayer;
    if (done == 0) {
        cMoviePlayer_asyncUpdate(self->mpMoviePlayer);
        if (self->mMovieSkippable != 0) {
            for (int i = 0; i < 2; i++) {
                void** input = (void**)SSXApp->Inputs[i];
                int ok = *input != 0 && **(int**)input != 0;
                if (ok) {
                    if (cInput_getState(input, 0x70) != 0 || cInput_getState(input, 122) != 0) {
                        done = 1;
                        goto out;
                    }
                }
            }
        }
    out:
        if (done == 0) return;
    }
    if (self->mpMoviePlayer != 0) {
        cMoviePlayer__cMoviePlayer(self->mpMoviePlayer, 3);
    }
    int movieBits = cRealFE_mMovieList;
    self->mWaitRenderNum = 1;
    self->mpMoviePlayer = 0;
    self->mMovieSkippable = 0;
    if (movieBits == 0) {
        func_002B3A98(&cAudioInterface_bxAudio()->MusicSys);
    }
    return;
    }
noh:
        if ((int)SSXApp->LoadingModule == 0) {
            cUIEngine_update(self->mpUI);
        } else {
            self->IsEnding = 1;
        }
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)self->mCamera.mVtbl;
        char* obj = (char*)&self->mCamera;
        vt[3].fn(vthis(obj, vt[3]));
        func_0019D250(self->mBoltFileManager);
        cFERiderManager_update(&self->mRiderManager);
        if ((void*)SSXApp->mFEAsyncManager != 0) {
            cFEAsyncManager_Update(SSXApp->mFEAsyncManager);
        }
        if (EnableOnline != 0) {
            func_00263828(self->mConnMan);
            func_00263CF0(self->mGameConnMan);
            if (LobbyMan_mLobbyMan != 0) {
                func_0025AC50(LobbyMan_mLobbyMan);
            }
        }
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_loadCharAnimations);
#ifdef SKIP_ASM
extern "C" void FILESYS_addbigsync(char* name, int a1, int a2, void* out);
extern "C" void FILESYS_delbigsync(void* handle, int arg);
extern "C" void* cAdfDataSource_cAdfDataSource(void* self, void* src);
extern "C" void cGameAnimMap_addSource(void* self, int kind, void* list);
// PORT: cGameAnimMap_testResolve__Fv is called with the map in $a0; bind the 1-arg form to that symbol.
void cGameAnimMap_testResolve_p(void* map) __asm__("cGameAnimMap_testResolve__Fv");
extern char str_data_char_anm_big_b[];
extern char str_AdfDataSourceFE[];
extern void* D_004A19C0[1];
struct sAnimTableK1A2B00 {
    void* anims[1];
};
extern sAnimTableK1A2B00* cAnimDataBase_TheAnimDataBase_K1A2B00 __asm__("cAnimDataBase_TheAnimDataBase");
extern void* cGameAnimMap_TheGameAnimMap;

/**
 * Loads the front end's character animation data from "data/char/anm.big" into the game animation map.
 *
 * Opens the big file, creates one "AdfDataSourceFE" data source (cAdfDataSource_cAdfDataSource) from D_004A19C0[0], stores it
 * in the animation table cAnimDataBase_TheAnimDataBase and registers it in the map cGameAnimMap_TheGameAnimMap (cGameAnimMap_addSource), resolves the map
 * and closes the file.
 */
extern "C" void cRealFE_loadCharAnimations(void)
{
    void* handle;
    int i;
    FILESYS_addbigsync(str_data_char_anm_big_b, 0, 100, &handle);
    for (i = 0; i < 1; i++) {
        void* anim = cAdfDataSource_cAdfDataSource(cMemMan_alloc(0x18, str_AdfDataSourceFE, 0, 0), D_004A19C0[i]);
        cAnimDataBase_TheAnimDataBase_K1A2B00->anims[(unsigned char)i] = anim;
        cGameAnimMap_addSource(cGameAnimMap_TheGameAnimMap, i, anim);
    }
    cGameAnimMap_testResolve_p(cGameAnimMap_TheGameAnimMap);
    FILESYS_delbigsync(handle, 100);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_purgeCharAnimations);
#ifdef SKIP_ASM
extern void** cAnimDataBase_TheAnimDataBase;
extern void* cGameAnimMap_TheGameAnimMap;
extern "C" void cGameAnimMap_remSource(void* a, void* b);
extern "C" void cAnimDataBase_purgeChanCache(void* a);
extern "C" void cAdfDataSource__cAdfDataSource(void* a, int b);

/**
 * Releases the front end's character animation data loaded by cRealFE_loadCharAnimations.
 *
 * For its one animation slot: when cAnimDataBase_TheAnimDataBase holds data, removes it from
 * cGameAnimMap_TheGameAnimMap (cGameAnimMap_remSource), clears the slot, calls cAnimDataBase_purgeChanCache on the table and destroys the data
 * (cAdfDataSource__cAdfDataSource with 3).
 */
extern "C" void cRealFE_purgeCharAnimations(cRealFE* self)
{
    int i;
    for (i = 0; i < 1; i++) {
        unsigned char idx = i;
        void* animData = *(void**)((char*)cAnimDataBase_TheAnimDataBase + (idx << 2));
        if (animData != 0) {
            cGameAnimMap_remSource(cGameAnimMap_TheGameAnimMap, animData);
            void** tbl = cAnimDataBase_TheAnimDataBase;
            *(void**)((char*)tbl + (idx << 2)) = 0;
            cAnimDataBase_purgeChanCache(tbl);
            cAdfDataSource__cAdfDataSource(animData, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cRealFE_loadStartState);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cUIState_cUIState(void* self, void* owner);
extern "C" void cUIStateStack_pushExplicit(void* stack, void* state);
extern "C" void cUIStateStack_push(void* stack, void* state);
extern "C" void* cFEStateTitle_cFEStateTitle1(void* self, void* owner);
extern "C" void* cFEStateLodge_cFEStateLodge(void* self, void* owner);
extern "C" void* cFEStateRelogin_cFEStateRelogin(void* self, void* owner);
extern "C" void cCommSystem_closeChannel(void* a, int b);
extern "C" void func_00266E88(void* a);
extern "C" void func_00266DF8(void* a);
extern "C" void func_0025B6D8(void* a, int b);
extern "C" void cBENetworkInterface_restoreCharProfile(void* a, int b);
extern "C" void cBE_newProfile(void* be, int b);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern char str_cFEStateBackground[];
extern char str_cFEStateTitle[];
extern char str_cFEStateLodge[];
extern char str_cFEStateRelogin[];
extern void* cFEStateBackground_vtbl[];
extern int cRealFE_mStartState;
extern int gInLodge;
extern void* VoipMan;

// PORT: cMemMan_alloc bound as a placement operator new (gcc then treats the result as unaliased)
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
struct sRealFE_1A2C40 {
    char pad0[0xC];
    char* ui;   // 0xC
};

/**
 * Pushes the front end's first states: the background, then the title, lodge or online relogin screen.
 *
 * The cFEStateBackground is created and pushed first. cRealFE_mStartState (why the front end was entered) then picks:
 * 0 the title screen (gInLodge = 0), 0x27 the lodge (gInLodge = 1), 0x34 cFEStateRelogin after restoring
 * the online session (comm system, lobby manager, the network character profile, BE state calls). The last
 * two reset cRealFE_mStartState to 0.
 */
extern "C" void cRealFE_loadStartState(cRealFE* self)
{
    cFEStateBackground* backState = new (str_cFEStateBackground, 0, 0) cFEStateBackground;
    cUIState_cUIState(backState, (char*)self->mpUI);
    (*(sVEntry<void (void*)>**)&backState->mVtbl) = (sVEntry<void(void*)>*)cFEStateBackground_vtbl;
    ((sVEntry<void (void*)>*)backState->mVtbl)[4].fn(vthis(backState, ((sVEntry<void (void*)>*)backState->mVtbl)[4]));
    cUIStateStack_pushExplicit(&self->mpUI->mStateStack, backState);
    switch (cRealFE_mStartState) {
    case 0: {
        gInLodge = 0;
        void* st = cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle, 0, 0), (char*)self->mpUI);
        cUIStateStack_push(&self->mpUI->mStateStack, st);
        break;
    }
    case 39: {
        gInLodge = 1;
        void* st = cFEStateLodge_cFEStateLodge(cMemMan_alloc(88, str_cFEStateLodge, 0, 0), (char*)self->mpUI);
        cUIStateStack_push(&self->mpUI->mStateStack, st);
        cRealFE_mStartState = 0;
        break;
    }
    case 52: {
        gInLodge = 0;
        void* st = cFEStateRelogin_cFEStateRelogin(cMemMan_alloc(1748, str_cFEStateRelogin, 0, 0), (char*)self->mpUI);
        cUIStateStack_push(&self->mpUI->mStateStack, st);
        cCommSystem_closeChannel(cCommSystem_mCommSystem, cGameComm_mGameComm->unk_06C);
        func_00266E88(VoipMan);
        func_00266DF8(VoipMan);
        func_0025B6D8(LobbyMan_mLobbyMan, 1);
        cBENetworkInterface_restoreCharProfile(cBE_getInterface_Fv(SSXApp->BE, 7), 0);
        cBE_newProfile(cBE_getBE(), 1);
        cBE_newProfile(cBE_getBE(), 2);
        cRealFE_mStartState = 0;
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFESound_cFESound);
#ifdef SKIP_ASM
void* cUISoundInterface_cUISoundInterface(void* self);
extern void* cFESound_vtbl[];

/**
 * Constructs the front end's sound interface.
 *
 * Runs the base constructor cUISoundInterface_cUISoundInterface, stores the vtable cFESound_vtbl and zeroes +0x4.
 *
 * @return @p self
 */
extern "C" void* cFESound_cFESound(cFESound* self)
{
    cUISoundInterface_cUISoundInterface(self);
    *(void***)self = cFESound_vtbl;
    self->mDummyVar = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cFESound_playSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void* snd, int a1);
/**
 * Plays the front-end menu sound for a UI sound event (vtable cFESound_vtbl entry 2, playSound).
 *
 * Event 1 plays menu sound 2, 2 plays 3, 3 plays 1, 4 plays 4 and 5 and 6 play 0 (cSSXAudio_MenuSound on
 * cAudioInterface_bxAudio()); other events are silent.
 *
 * @param sel  the UI sound event
 */
extern "C" void cFESound_playSound(cFESound* self, int sel)
{
    switch (sel) {
    case 1:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 2);
        break;
    case 2:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 3);
        break;
    case 3:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 1);
        break;
    case 4:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 4);
        break;
    case 5:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0);
        break;
    case 6:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cOVSound_cOVSound);
#ifdef SKIP_ASM
void* cUISoundInterface_cUISoundInterface(void* self);
extern void* cOVSound_vtbl[];

/**
 * Constructs the overlay sound interface.
 *
 * Runs cUISoundInterface_cUISoundInterface (the UI sound interface base), installs cOVSound_vtbl at +0x0 and clears +0x4.
 *
 * @return @p self
 */
extern "C" void* cOVSound_cOVSound(cOVSound* self)
{
    cUISoundInterface_cUISoundInterface(self);
    *(void***)self = cOVSound_vtbl;
    self->mDummyVar = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/fereal", cOVSound_playSound);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_MenuSound(void* snd, int a1);
/**
 * Plays the overlay menu sound for a UI sound event.
 *
 * Maps @p sel to a front-end menu sound played with cSSXAudio_MenuSound() on the audio object (cAudioInterface_bxAudio()):
 * 1 -> 11, 2 -> 12, 3 -> 10, 4 -> 13, 5 and 6 -> 9; other values play nothing.
 *
 * @param sel  the sound event (On Tour: int e)
 */
extern "C" void cOVSound_playSound(cOVSound* self, int sel)
{
    switch (sel) {
    case 1:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 11);
        break;
    case 2:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 12);
        break;
    case 3:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 10);
        break;
    case 4:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 13);
        break;
    case 5:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 9);
        break;
    case 6:
        cSSXAudio_MenuSound(cAudioInterface_bxAudio(), 9);
        break;
    }
}
#endif


//100%
INCLUDE_ASM("fe/fereal", cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode);
#ifdef SKIP_ASM
extern void* cFEStateSelectMultiplayerMode_vtbl[];

/**
 * Constructs a select-multiplayer-mode state.
 *
 * Runs the cUIState constructor cUIState_cUIState(self, owner), sets the state id (+0xC) to 22 and stores
 * cFEStateSelectMultiplayerMode_vtbl.
 *
 * @param owner  the UI engine
 * @return @p self
 */
extern "C" void* cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode(cFEStateSelectMultiplayerMode* self, cUIEngine* owner)
{
    cUIState_cUIState(self, owner);
    self->mHashName = 22;
    self->mVtbl = cFEStateSelectMultiplayerMode_vtbl;
    return ((void*)self);
}
#endif
