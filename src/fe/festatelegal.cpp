#include "common.h"
#include "vtable.h"
#include "be/beinterface.h"
#include "ui/uiengine.h"
#include "fe/festatelegal.h"
#include "fe/festateoptions.h"
#include "ui/uiscreen.h"

//100%
INCLUDE_ASM("fe/festatelegal", cFEStateLegal_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_122bAutosave[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the autosave notice screen ("122bAutosave") and resets its frame counter.
 */
extern "C" void cFEStateLegal_onCreateScreen(cFEStateLegal* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_122bAutosave), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    self->mDisplayTicks = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", cFEStateLegal_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_TransitionOut[];
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIState_onUpdate(void* self);
extern "C" void cUIStateStack_pop(void* list, int a1);

/**
 * Shows the autosave notice for 240 frames, then plays "TransitionOut" and closes it.
 */
extern "C" void cFEStateLegal_onUpdate(cFEStateLegal* self)
{
    cUIState_onUpdate(self);
    if (++self->mDisplayTicks == cFEStateLegal::kDisplayTicks) {
        int frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_TransitionOut));
        if (frame != cUIScreen::kNoFrame) {
            cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
        cUIStateStack_pop(&self->mpEngine->mStateStack, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", func_00187D38);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_002C2868(char* s);
extern "C" int cBERewardInterface_buyCheatChar(void* iface, int a, int i, int item, int b);
extern "C" int cBEEconInterface_getPeakPass(void* iface, int a, int i, int item, int b);
int cBERewardInterface_getTotalNumSongs(void* iface);
int cBERewardInterface_getTotalNumPosters(void* iface);
int cBERewardInterface_getTotalNumTradingCards(void* iface);
int cBERewardInterface_getTotalNumConceptArts(void* iface);
int cBERewardInterface_getTotalNumToys(void* iface);
int cBERewardInterface_getTotalNumVideos(void* iface);
extern "C" int cBERewardInterface_buySong(void* iface, int a, int i, int j, int b);
extern "C" void cBERewardInterface_setPlayList(void* iface, int a, int i, long mask);
extern "C" int cBERewardInterface_buyPoster(void* iface, int a, int i, int j, int b);
extern "C" int cBERewardInterface_buyTradingCard(void* iface, int a, int i, int j, int b);
extern "C" int cBERewardInterface_buyConceptArt(void* iface, int a, int i, int j, int b);
extern "C" int cBERewardInterface_buyVideo(void* iface, int a, int i, int j, int b);
extern "C" int cBERewardInterface_buyToy(void* iface, int a, int i, int j, int b);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" char* func_0014D998(void* db, int type);
int func_0014D988(void* db, int type);
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0014D908(void* db, char* item, int id);
extern "C" void func_00151C48(void* prof, int id, int on);
extern int D_004A11E0;

static inline int ItemType_7D38(char* item)
{
    return item[2];
}

/**
 * Applies a cheat code: lower-cases @p code, hashes it and, when the hash is a known code, unlocks rewards or items for
 * every character of profile 0.
 *
 * Copies @p code into a 128-byte buffer (strcpy, no length check), lower-cases it (func_002C2868()) and compares
 * GetHashValue32() of it (recomputed for every comparison) against a fixed list. Uses the BE interfaces 11 (econ), 13
 * (rewards) and 9 (profiles). Most codes call cBERewardInterface_buyCheatChar() for characters 0-9 with one reward index (10, 11, 13, 15,
 * 17-28) and no money award. 0x82DE683 unlocks peak 1 and 0x4B1FF67 peaks 1 and 2 for all characters through
 * cBEEconInterface_getPeakPass(). 0x61F9F02 sets every bit below cBERewardInterface_getTotalNumSongs()'s count with cBERewardInterface_buySong() and stores the mask with
 * cBERewardInterface_setPlayList(); 0x5B65653, 0xA0CF7C4, 0xB5D8914, 0x717257D and 0x74F6CF4 do every index below the matching count
 * (cBERewardInterface_getTotalNumPosters, cBERewardInterface_getTotalNumTradingCards, cBERewardInterface_getTotalNumConceptArts, cBERewardInterface_getTotalNumVideos, cBERewardInterface_getTotalNumToys) with cBERewardInterface_buyPoster, cBERewardInterface_buyTradingCard,
 * cBERewardInterface_buyConceptArt, cBERewardInterface_buyVideo and cBERewardInterface_buyToy. 0xBCF3974 marks every library item of types 1-3 that passes
 * func_0014D908() against D_004A11E0 as owned (func_00151C48()) for profiles 0-2 and characters 0-9; 0x70694D5 does the
 * same for every type-1 item without the check. cFEStateOptions_onWidgetEvent() calls it with the text typed into its
 * popup.
 *
 * @param code  the typed code (copied, not modified)
 * @return true when the code was recognised and applied
 */
extern "C" bool func_00187D38(char* code)
{
    void* gear = cBE_getInterface_Fv(cBE_getBE(), 11);
    void* unlocks = cBE_getInterface_Fv(cBE_getBE(), 13);
    void* profiles = cBE_getInterface_Fv(cBE_getBE(), 9);
    char buf[0x80];
    strcpy(buf, code);
    func_002C2868(buf);
    if (GetHashValue32(buf) == 0x7E68D) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 11, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x699649F) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 13, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x80C4F45) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 0xF, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xFA26044) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 17, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x539AB42) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 10, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x61C8197) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 18, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x64694E5) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 19, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x3F22E2B) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 20, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xCD28689) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 21, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xB582D1C) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 22, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x1CB90E5) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 23, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x715E0E4) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 0x18, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xF3223E) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 25, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xEAFC69E) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 27, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xDD36D88) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 0x1C, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xDE3475) {
        for (int i = 0; i < 10; i++)
            cBERewardInterface_buyCheatChar(unlocks, 0, i, 26, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x82DE683) {
        for (int i = 0; i < 10; i++)
            cBEEconInterface_getPeakPass(gear, 0, i, 1, 1);
        return true;
    } else if (GetHashValue32(buf) == 0x4B1FF67) {
        for (int i = 0; i < 10; i++) {
            cBEEconInterface_getPeakPass(gear, 0, i, 1, 1);
            cBEEconInterface_getPeakPass(gear, 0, i, 2, 1);
        }
        return true;
    } else if (GetHashValue32(buf) == 0x61F9F02) {
        int n = cBERewardInterface_getTotalNumSongs(unlocks);
        for (int i = 0; i < 10; i++) {
            long mask = 0;
            for (int j = 0; j < n; j++) {
                cBERewardInterface_buySong(unlocks, 0, i, j, 0);
                mask |= (long)1 << j;
            }
            cBERewardInterface_setPlayList(unlocks, 0, i, mask);
        }
        return true;
    } else if (GetHashValue32(buf) == 0x5B65653) {
        int n = cBERewardInterface_getTotalNumPosters(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                cBERewardInterface_buyPoster(unlocks, 0, i, j, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xA0CF7C4) {
        int n = cBERewardInterface_getTotalNumTradingCards(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                cBERewardInterface_buyTradingCard(unlocks, 0, i, j, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xB5D8914) {
        int n = cBERewardInterface_getTotalNumConceptArts(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                cBERewardInterface_buyConceptArt(unlocks, 0, i, j, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x717257D) {
        int n = cBERewardInterface_getTotalNumVideos(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                cBERewardInterface_buyVideo(unlocks, 0, i, j, 0);
        return true;
    } else if (GetHashValue32(buf) == 0x74F6CF4) {
        int n = cBERewardInterface_getTotalNumToys(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                cBERewardInterface_buyToy(unlocks, 0, i, j, 0);
        return true;
    } else if (GetHashValue32(buf) == 0xBCF3974) {
        for (int team = 0; team < 3; team++) {
            for (int type = 0; type < 10; type++) {
                void* db = func_0014BDB8_noarg();
                char* item = func_0014D998(db, type);
                int count = func_0014D988(db, type);
                void* prof = func_0014AD28(profiles, team, type);
                for (int k = 0; k < count; k++, item += 0x38) {
                    if (ItemType_7D38(item) == 1 || ItemType_7D38(item) == 2 || ItemType_7D38(item) == 3) {
                        if (func_0014D908(db, item, D_004A11E0))
                            func_00151C48(prof, *(short*)(item + 4), 1);
                    }
                }
            }
        }
        return true;
    } else if (GetHashValue32(buf) == 0x70694D5) {
        for (int team = 0; team < 3; team++) {
            for (int type = 0; type < 10; type++) {
                char* item = func_0014D998(func_0014BDB8_noarg(), type);
                int count = func_0014D988(func_0014BDB8_noarg(), type);
                void* prof = func_0014AD28(profiles, team, type);
                for (int k = 0; k < count; k++, item += 0x38) {
                    if (ItemType_7D38(item) == 1)
                        func_00151C48(prof, *(short*)(item + 4), 1);
                }
            }
        }
        return true;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", func_001887A0);
#ifdef SKIP_ASM
struct sFlowState_887A0 {
    signed char count;
    char* states;
};
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
extern sFlowState_887A0 gFlowState;
extern void* cFEStateOptions_vtbl[];
extern "C" void* cUIState_cUIState(void* self);
extern "C" void cFEFlowState_setValue(void* self, int a1, int a2);

/**
 * Constructs the options state: base cUIState, cFEStateOptions_vtbl, state id 0x18, optionally resetting its flow
 * entry.
 *
 * Runs the cUIState constructor cUIState_cUIState(), stores mHashName 0x18 and cFEStateOptions_vtbl. When @p reset is
 * non-zero, writes 0 to entry 0x18 of the flow-state table gFlowState (cFEFlowState_setValue()). Callers allocate 84 bytes
 * ("cFEStateOptions" or "FEOptions").
 *
 * @param engine  the UI engine (passed to the base constructor)
 * @param reset  non-zero to clear the options entry in gFlowState
 * @return @p self
 */
extern "C" void* func_001887A0(cFEStateOptions* self, int engine, int reset)
{
    cUIState_cUIState(self);
    self->mHashName = 0x18;
    VPTR_OF(cListNode, self) = cFEStateOptions_vtbl;
    if (reset != 0) {
        cFEFlowState_setValue(&gFlowState, 0x18, 0);
    }
    return ((void*)self);
}
#endif

