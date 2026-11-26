#include "common.h"
#include "vtable.h"
#include "be/beinterface.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "render/font.h"
#include "fe/fememcard.h"
#include "fe/ovstateeventresults.h"
#include "fe/ovstatetrophy.h"
#include "fe/ovstaterewardslist.h"
#include "fe/ovstatehud.h"
#include "ai/rider.h"
#include "be/beintnewrace.h"
#include "fe/ovtemplatedialog.h"
#include "fe/ovstateheat.h"
#include "fe/ovstateenterlodge.h"
#include "fe/ovstaterewards.h"
#include "fe/ovstatetoptimes.h"
#include "fe/ovstatereplay.h"
#include "fe/ovstatebigchallenge.h"
#include "fe/ovstatepause.h"
#include "fe/ovstatetransport.h"
#include "fe/ovstatemap.h"
#include "fe/ovstateprofile.h"
#include "fe/ovstatesavereplay.h"
#include "fe/ovstatesaveoptions.h"
#include "fe/ovstateautosave.h"
#include "fe/festatelodgeriderdetail.h"
#include "fe/festatelodge.h"
#include "fe/festatebuyattrib.h"
#include "fe/festatecareerstats.h"
#include "fe/messagecenter.h"
#include "render/ps2graphicsman.h"
#include "fe/ovstate.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "render/graphicsman.h"
#include "luno/lunovar.h"

INCLUDE_ASM("fe/all_in_one_fe", __sti__all_in_one_fe_cpp);

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C640);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_xi(void*, int) __asm__("cUIState_cUIState");
extern char cOVState_vtbl[];
/**
 * Constructs a bare cOVState without an engine: runs the cUIState constructor cUIState_cUIState with a null engine and
 * stores the cOVState vtable cOVState_vtbl.
 *
 * None of the cOVState fields (mOVID, the flags, mszChatBuff) are set, unlike cOVState_cOVState. Out-of-line copy with no
 * callers.
 *
 * @return the object
 */
extern "C" void* func_0021C640(cOVState* self) {
    cUIState_cUIState_xi(self, 0);
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_cOVState);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it), then sets mOVID (0x98) to @p overlayId, clears mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54),
 * stores the cOVState vtable cOVState_vtbl and copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b.
 * Every overlay-state constructor in this unit and cOVStateManager_triggerOverlay inline the same steps.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_cOVState(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState__cOVState);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys a cOVState.
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of the cOVState vtable cOVState_vtbl. cOVState_REPLAY__cOVState_REPLAY,
 * cOVStateHiScoreList__cOVStateHiScoreList and the other empty overlay destructors repeat the same body.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState__cOVState(cOVState* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_allowPauseMenu);
#ifdef SKIP_ASM
/**
 * Tells whether an overlay state lets the pause menu open.
 *
 * @param self  the overlay state
 * @return mAllowPauseMenu
 */
extern "C" int cOVState_allowPauseMenu(cOVState* self) {
    return (self->mAllowPauseMenu);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_Simple__cOVTemplate_Simple);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the one-option overlay menu whose vtable is cOVTemplate_Simple_vtbl.
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVTemplate_Simple_vtbl. Its onCreateScreen cOVTemplate_Simple_onCreateScreen fills the single "overlayOpt"
 * entry; nothing outside this unit stores cOVTemplate_Simple_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVTemplate_Simple__cOVTemplate_Simple(cOVTemplate_Simple* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_Simple_cOVTemplate_Simple);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVTemplate_Simple_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the one-option overlay menu (onCreateScreen cOVTemplate_Simple_onCreateScreen) for engine @p engine and overlay id @p overlayId;
 * installs cOVTemplate_Simple_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVTemplate_Simple_cOVTemplate_Simple(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVTemplate_Simple_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_Dialog__cOVTemplate_Dialog);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog-template overlay whose vtable cOVTemplate_Dialog_vtbl carries cOVTemplate_Dialog_onCreateScreen and
 * cOVTemplate_Dialog_onWidgetEvent.
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVTemplate_Dialog_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVTemplate_Dialog__cOVTemplate_Dialog(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_Dialog_cOVTemplate_Dialog);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVTemplate_Dialog_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the dialog-template overlay for engine @p engine and overlay id @p overlayId; installs cOVTemplate_Dialog_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers. Its vtable carries
 * cOVTemplate_Dialog_onCreateScreen / cOVTemplate_Dialog_onWidgetEvent, and the dialog overlays (ids 23-28 and 38)
 * reuse every slot but the destructor.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVTemplate_Dialog_cOVTemplate_Dialog(cOVTemplate_Dialog* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVTemplate_Dialog_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_Dialog_onUpdate);
#ifdef SKIP_ASM
extern "C" void cOVState_onUpdate();

/**
 * Updates a dialog overlay by running the base cOVState_onUpdate (a tail call; the state pointer passes through).
 *
 * It fills the onUpdate slot (+0x64) of the dialog-template vtable cOVTemplate_Dialog_vtbl and of the seven dialog overlays built on
 * it (cOVState_RESTART_vtbl, cOVState_QUIT_vtbl, cOVState_RESTARTCHALLENGE_vtbl, cOVState_QUITCHALLENGE_vtbl, cOVState_QUITCOMPETITION_vtbl, cOVState_GIVEUP_vtbl, cOVState_SAVEPROMPT_vtbl).
 */
extern "C" void cOVTemplate_Dialog_onUpdate(void) {
    cOVState_onUpdate();
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021C848);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the overlay with vtable D_00474B80 (overlay id 14).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_00474B80.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void func_0021C848(cOVState* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_REPLAY__cOVState_REPLAY);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the replay overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @param replayState  the replay overlay state being destroyed
 */
extern "C" void cOVState_REPLAY__cOVState_REPLAY(cOVState_REPLAY* replayState) {
    VPTR_OF(cListNode, replayState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(replayState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_REPLAY_cOVState_REPLAY);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_REPLAY_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_REPLAY for engine @p engine and overlay id @p overlayId; installs cOVState_REPLAY_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 17 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_REPLAY_cOVState_REPLAY(cOVState_REPLAY* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_REPLAY_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_RESTART__cOVState_RESTART);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_RESTART_vtbl (overlay id 23, dialog kind 0 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_RESTART_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_RESTART__cOVState_RESTART(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_QUIT__cOVState_QUIT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_QUIT_vtbl (overlay id 24, dialog kind 1 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_QUIT_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_QUIT__cOVState_QUIT(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_QUIT_cOVState_QUIT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_C950(void*) __asm__("cUIState_cUIState");
extern char cOVState_QUIT_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the dialog overlay with vtable cOVState_QUIT_vtbl for engine @p engine and overlay id @p overlayId and sets its dialog
 * kind (0x9C) to 1.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 24 inline with
 * the same steps; this out-of-line copy has no callers. Its slots are those of the dialog template cOVTemplate_Dialog_vtbl except
 * the destructor; the dialog kind picks the text.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_QUIT_cOVState_QUIT(cOVTemplate_Dialog* ovState, int engine, int overlayId) {
    cUIState_cUIState_C950(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_QUIT_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    ovState->mDialogType = 1;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_RESTARTCHALLENGE__cOVState_RESTARTCHALLENGE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_RESTARTCHALLENGE_vtbl (overlay id 25, dialog kind 2 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_RESTARTCHALLENGE_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_RESTARTCHALLENGE__cOVState_RESTARTCHALLENGE(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_QUITCHALLENGE__cOVState_QUITCHALLENGE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_QUITCHALLENGE_vtbl (overlay id 26, dialog kind 3 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_QUITCHALLENGE_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_QUITCHALLENGE__cOVState_QUITCHALLENGE(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_QUITCOMPETITION__cOVState_QUITCOMPETITION);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_QUITCOMPETITION_vtbl (overlay id 27, dialog kind 4 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_QUITCOMPETITION_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_QUITCOMPETITION__cOVState_QUITCOMPETITION(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_GIVEUP__cOVState_GIVEUP);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_GIVEUP_vtbl (overlay id 28, dialog kind 5 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_GIVEUP_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_GIVEUP__cOVState_GIVEUP(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_SAVEPROMPT__cOVState_SAVEPROMPT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the dialog overlay with vtable cOVState_SAVEPROMPT_vtbl (overlay id 38, dialog kind 6 at 0x9C).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_SAVEPROMPT_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_SAVEPROMPT__cOVState_SAVEPROMPT(cOVTemplate_Dialog* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cPDATemplate__cPDATemplate);
#ifdef SKIP_ASM
extern "C" void cUIState__cUIState();

/**
 * Destroys the PDA template through the UI-screen destructor cUIState__cUIState().
 *
 * Declared without parameters: the object and the destructor flags reach cUIState__cUIState() unchanged in their
 * registers.
 */
extern "C" void cPDATemplate__cPDATemplate(void) {
    cUIState__cUIState();
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cPDATemplate_cPDATemplate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_x(void*) __asm__("cUIState_cUIState");
extern char cPDATemplate_vtbl[];
/**
 * Constructs a cPDATemplate: runs the cUIState constructor cUIState_cUIState and installs cPDATemplate_vtbl.
 *
 * The engine argument is not declared here but reaches cUIState_cUIState untouched in the second argument register. No
 * callers.
 *
 * @return the object
 */
extern "C" void* cPDATemplate_cPDATemplate(cPDATemplate* self) {
    cUIState_cUIState_x(self);
    VPTR_OF(cListNode, self) = (void**)cPDATemplate_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_MESSAGECENTER__cOVState_MESSAGECENTER);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys a cOVState_MESSAGECENTER.
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of its vtable cOVState_MESSAGECENTER_vtbl. include/fe/messagecenter.h already lists it as that
 * class's destructor.
 *
 * @param messageCenter  the object being destroyed
 */
extern "C" void cOVState_MESSAGECENTER__cOVState_MESSAGECENTER(cOVState_MESSAGECENTER* messageCenter) {
    VPTR_OF(cListNode, messageCenter) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(messageCenter);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB08);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_xi(void*, int) __asm__("cUIState_cUIState");
extern char cOVState_ViewPopup_vtbl[];
/**
 * Constructs a cOVState_ViewPopup without an engine: runs the cUIState constructor cUIState_cUIState with a null engine and
 * installs cOVState_ViewPopup_vtbl.
 *
 * Nothing else is set (the real constructor, cOVState_ViewPopup_cOVState_ViewPopup, also takes the parent state, the
 * message and its number). Out-of-line copy with no callers.
 *
 * @return the object
 */
extern "C" void* func_0021CB08(cOVState_ViewPopup* self) {
    cUIState_cUIState_xi(self, 0);
    VPTR_OF(cListNode, self) = (void**)cOVState_ViewPopup_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_ViewPopup__cOVState_ViewPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_ViewPopup_vtbl[];

/**
 * Destroys the message view popup: restores cOVState_ViewPopup_vtbl and runs the cUIState destructor
 * (cUIState__cUIState).
 *
 * Only @p viewPopup is passed in C; the g++ destructor flags reach cUIState__cUIState untouched in $a1, so a port must pass
 * them explicitly.
 *
 * @param viewPopup  the popup being destroyed
 */
extern "C" void cOVState_ViewPopup__cOVState_ViewPopup(cOVState_ViewPopup* viewPopup) {
    VPTR_OF(cListNode, viewPopup) = (void**)cOVState_ViewPopup_vtbl;
    cUIState__cUIState_a(viewPopup);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_ViewPopup_onLoseTransition);
#ifdef SKIP_ASM
/**
 * Ends the message view popup's lose-focus transition at once.
 *
 * @return always 1 (the transition is done)
 */
extern "C" int cOVState_ViewPopup_onLoseTransition(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CB70);
#ifdef SKIP_ASM
/**
 * Initialises a six-word block: the first word to 1, the other five to 0.
 *
 * No callers; the block's type is not known.
 *
 * @param block  the block to fill
 */
extern "C" void func_0021CB70(int *block) {
    block[0]=1;
    block[1]=0;
    block[2]=0;
    block[3]=0;
    block[4]=0;
    block[5]=0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cHUDRadar_ShowRadar);
#ifdef SKIP_ASM
extern int D_004A275C;

/**
 * Sets the global flag D_004A275C.
 *
 * cOVStateHUD1P_onUpdate also writes it, and the HUD marker drawers func_0020EDA0 / func_0020FB40 draw nothing while it
 * is 0. No callers.
 *
 * @param newValue  the new value
 */
extern "C" void cHUDRadar_ShowRadar(int newValue) {
    D_004A275C = newValue;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cHUDRadar_IsRadarShown);
#ifdef SKIP_ASM
extern int D_004A275C;

/**
 * Returns the global flag D_004A275C (see cHUDRadar_ShowRadar). No callers.
 *
 * @return the current value of D_004A275C
 */
extern "C" int cHUDRadar_IsRadarShown(void) {
    return D_004A275C;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CBA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_CBA0(void*, int) __asm__("cUIState_cUIState");
extern "C" void cHUDRadar_cHUDRadar(void*);
extern char cOVStateHUD1P_vtbl[];
struct sElem_CBA0 {
    void* operator new[](unsigned int, void* p) { return p; }
    char pad[8];
    char sub[0x84];
    sElem_CBA0() { cHUDRadar_cHUDRadar(sub); }
};

/**
 * Constructs a cOVStateHUD1P without an engine and installs cOVStateHUD1P_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with a null engine, constructs the two 0x8C-byte elements at 0x48 with
 * cHUDRadar_cHUDRadar, sets 0x474 to -1, 0x478 / 0x47C / 0x488 / 0x48C to 1.0f and 0x480 / 0x484 to 0. The real constructor
 * cOVStateHUD1P_cOVStateHUD1P does the same with an engine and also clears 0x3D4 and 0x3D8. Out-of-line copy with no
 * callers.
 *
 * @param state  the object to construct
 * @return @p state
 */
extern "C" void *func_0021CBA0(cOVStateHUD1P* state) {
    cUIState_cUIState_CBA0(((char*)state), 0);
    VPTR_OF(cListNode, state) = (void**)cOVStateHUD1P_vtbl;
    new ((char*)&state->mRiderInfo[0]) sElem_CBA0[2];
    state->mGlow.mSourceShape = -1;
    state->mGlow.mWidth = 1.0f;
    state->mGlow.mHeight = 1.0f;
    state->mGlow.mTop = 0.0f;
    state->mGlow.mLeft = 0.0f;
    state->mGlow.mRight = 1.0f;
    state->mGlow.mBottom = 1.0f;
    return ((void*)state);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P__cOVStateHUD1P);
#ifdef SKIP_ASM
extern "C" void cHUDRadar__cHUDRadar(void *, int);
extern "C" void cUIState__cUIState(void *, int);
extern char cOVStateHUD1P_vtbl[];

/**
 * Destroys the one-player HUD overlay state: its two HUD elements, then the cUIState base.
 *
 * Restores cOVStateHUD1P_vtbl, destroys the two 0x8C-byte elements at +0x48 last to first (each through
 * cHUDRadar__cHUDRadar() on its part at +8, with flags 2 so nothing is freed), then calls the cUIState destructor
 * cUIState__cUIState(). cHUDRadar__cHUDRadar drops the shared instance count D_004A276C and clears the textures-loaded
 * flag D_004A2768 when the last element goes.
 *
 * @param flags  g++ destructor flags, passed on to cUIState__cUIState(); bit 0 set frees the object
 */
extern "C" void cOVStateHUD1P__cOVStateHUD1P(cOVStateHUD1P* self, int flags) {
    VPTR_OF(cListNode, self) = (void**)cOVStateHUD1P_vtbl;
    if ((char*)&self->mRiderInfo[0] != 0) {
        char *element = (char*)&self->mRiderInfo[0] + 0x118;
        while ((char*)&self->mRiderInfo[0] != element) {
            element -= 0x8C;
            cHUDRadar__cHUDRadar(element + 8, 2);
        }
    }
    cUIState__cUIState(((void*)self), flags);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CCC8);
#ifdef SKIP_ASM
/**
 * Clears the low four bits of a flag byte and returns its address.
 *
 * Used by cUIIcon_cUIIcon, cUIScreen_createObjectByStruct and cOVState_MAP_setupPlayerIndicator on the flag byte of a
 * UI object (see include/ui/uiicon.h).
 *
 * @param flags  the flag byte
 * @return @p flags
 */
extern "C" unsigned char *func_0021CCC8(unsigned char *flags) {
    *flags &= 0xF0;
    return flags;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_HEAT_STANDINGS__cOVState_HEAT_STANDINGS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the overlay with vtable cOVState_HEAT_STANDINGS_vtbl (overlay id 10).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_HEAT_STANDINGS_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_HEAT_STANDINGS__cOVState_HEAT_STANDINGS(cOVState_HEAT_STANDINGS* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_HEAT_STANDINGS_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_HEAT_STANDINGS_vtbl for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 10 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS(cOVState_HEAT_STANDINGS* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_HEAT_STANDINGS_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_FINAL_STANDINGS__cOVState_FINAL_STANDINGS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the overlay with vtable cOVState_FINAL_STANDINGS_vtbl (overlay id 11).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_FINAL_STANDINGS_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_FINAL_STANDINGS__cOVState_FINAL_STANDINGS(cOVState_FINAL_STANDINGS* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_FINAL_STANDINGS_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_FINAL_STANDINGS_vtbl for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 11 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS(cOVState_FINAL_STANDINGS* ovState, int engine, int overlayId) {
    cUIState_cUIState(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_FINAL_STANDINGS_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CE00);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the overlay with vtable D_00473ED8 (overlay id 22).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_00473ED8.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void func_0021CE00(cOVState* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021CE28);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_CE28(void*) __asm__("cUIState_cUIState");
extern char D_00473ED8[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_00473ED8 for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 22 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *func_0021CE28(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState_CE28(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_00473ED8;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_RACE_FINISHOV__cOVState_RACE_FINISHOV);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the finish overlay with vtable cOVState_RACE_FINISHOV_vtbl (overlay id 12).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_RACE_FINISHOV_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_RACE_FINISHOV__cOVState_RACE_FINISHOV(cOVState_RACE_FINISHOV* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_CEB8(void*) __asm__("cUIState_cUIState");
int GetHashValue32(char*);
extern char cOVState_RACE_FINISHOV_vtbl[];
extern char cOVState_vtbl[];
extern char str_finishov[];
extern unsigned short str_sp_b[];

/**
 * Constructs the finish overlay with vtable cOVState_RACE_FINISHOV_vtbl for engine @p engine and overlay id @p overlayId, sets the word at 0xA0
 * to 1 and names the state "finishov" (mHashName, 0xC, = GetHashValue32).
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 12 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV(cOVState_RACE_FINISHOV* ovState, int engine, int overlayId) {
    cUIState_cUIState_CEB8(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mCanContinue = 1;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_RACE_FINISHOV_vtbl;
    ovState->mHashName = GetHashValue32(str_finishov);
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BASSSP_QUAL_FINISHOV__cOVState_BASSSP_QUAL_FINISHOV);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the finish overlay with vtable cOVState_BASSSP_QUAL_FINISHOV_vtbl (overlay id 13).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_BASSSP_QUAL_FINISHOV_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_BASSSP_QUAL_FINISHOV__cOVState_BASSSP_QUAL_FINISHOV(cOVState_BASSSP_QUAL_FINISHOV* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_CF60(void*) __asm__("cUIState_cUIState");
int GetHashValue32(char*);
extern char cOVState_BASSSP_QUAL_FINISHOV_vtbl[];
extern char cOVState_vtbl[];
extern char str_finishov[];
extern unsigned short str_sp_b[];

/**
 * Constructs the finish overlay with vtable cOVState_BASSSP_QUAL_FINISHOV_vtbl for engine @p engine and overlay id @p overlayId, sets the word at 0xA0
 * to 1 and names the state "finishov" (mHashName, 0xC, = GetHashValue32).
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 13 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV(cOVState_BASSSP_QUAL_FINISHOV* ovState, int engine, int overlayId) {
    cUIState_cUIState_CF60(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mCanContinue = 1;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_BASSSP_QUAL_FINISHOV_vtbl;
    ovState->mHashName = GetHashValue32(str_finishov);
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TOURNAMENT_STANDINGS__cOVState_TOURNAMENT_STANDINGS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the overlay with vtable cOVState_TOURNAMENT_STANDINGS_vtbl (overlay id 35).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_TOURNAMENT_STANDINGS_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_TOURNAMENT_STANDINGS__cOVState_TOURNAMENT_STANDINGS(cOVState_TOURNAMENT_STANDINGS* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_D008(void*) __asm__("cUIState_cUIState");
extern char cOVState_TOURNAMENT_STANDINGS_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_TOURNAMENT_STANDINGS_vtbl for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 35 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS(cOVState_TOURNAMENT_STANDINGS* ovState, int engine, int overlayId) {
    cUIState_cUIState_D008(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_TOURNAMENT_STANDINGS_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021D070);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVStateHiScoreList_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateHiScoreList without an engine; installs cOVStateHiScoreList_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_0021D070(cOVStateHiScoreList* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateHiScoreList_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHiScoreList_cOVStateHiScoreList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_D0D0(void*) __asm__("cUIState_cUIState");
extern char cOVStateHiScoreList_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateHiScoreList for engine @p engine and overlay id @p overlayId; installs cOVStateHiScoreList_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVStateHiScoreList_cOVStateHiScoreList(cOVStateHiScoreList* ovState, int engine, int overlayId) {
    cUIState_cUIState_D0D0(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateHiScoreList_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHiScoreList__cOVStateHiScoreList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the high-score list overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cOVStateHiScoreList__cOVStateHiScoreList(cOVStateHiScoreList* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFont_textf2);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define cFont_textf2_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern "C" void cFont_vtextf(int, int, char*);

/**
 * Prints formatted text with a font at a position: a second, identical copy of the printf-style wrapper cFont_textf.
 *
 * Builds the variable-argument list (EE EABI va_start) and passes the font, the format and the list to cFont_vtextf;
 * the x and y floats stay in their FPU argument registers. The body is the same as cFont_textf's
 * (main/all_in_one_main); no jal to this copy and no data reference to it appear in the retail code.
 *
 * @param x  left edge of the text
 * @param y  top of the text
 * @param font  the font (cFont, the method's this)
 * @param fmt  printf-style format string
 */
extern "C" void cFont_textf2(float x, float y, int font, int fmt, ...) {
    char* ap;
    cFont_textf2_va_start(ap);
    cFont_vtextf(font, fmt, ap);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderBoostPart);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).


// Vertex position of the HUD sprite quads: 16-byte aligned, with an empty default ctor (vertex arrays get a vec_new
// loop) and the (x, y, z, const float& w) ctor the quad builders use.
struct sVtxPos {
    float x, y, z, w;
    sVtxPos() {}
    sVtxPos(float ax, float ay, float az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
    void setZW(float az, float aw) { z = az; w = aw; }
} __attribute__((aligned(16)));

// One vertex as the render context's slot-71 draw call takes it: UV + q, the colour as R, G, B, A words (the GS
// RGBAQ order), position.
struct sHudVertex {
    float u, v, q, pad;
    int r, g, b, a;
    sVtxPos pos;
};

// The render context D_004A289C_D1A0 as these functions use it: GS state block pointer at 0xE84, vptr at 0x10D8 (slot
// 71
// draws a vertex list).
struct sRenderCtx {
    char pad[0xE84];
    char* e84;              // 0xE84
};


// A HUD sprite: texture id and UV rectangle.
struct sHudSprite {
    int tex;                // 0x0
    int pad[2];
    float v0;               // 0xC
    float u0;               // 0x10
    float u1;               // 0x14
    float v1;               // 0x18
};

// Gauge layout: segment scale and number of middle segments.
struct sGaugeBar {
    float scale;
    int count;
};

static inline int texOf(sHudSprite* s)
{
    return s->tex;
}

static inline void setTex(sHudSprite* s)
{
    *GrMan->m_pCurrentDrawState->m_textureState.m_texid = texOf(s);
}

static inline void setUV(sHudVertex* v, sHudSprite* s)
{
    v[0].u = s->u0;
    v[0].v = s->v0;
    v[1].u = s->u1;
    v[1].v = s->v0;
    v[2].u = s->u0;
    v[2].v = s->v1;
    v[3].u = s->u1;
    v[3].v = s->v1;
}

static inline void setPos(sHudVertex* v, float x0, float y0, float x1, float y1)
{
    v[0].pos.x = x0;
    v[0].pos.y = y0;
    v[1].pos.x = x1;
    v[1].pos.y = y0;
    v[2].pos.x = x0;
    v[2].pos.y = y1;
    v[3].pos.x = x1;
    v[3].pos.y = y1;
}

static inline void drawQuad(sHudVertex* v)
{
    cPS2GraphicsMan* ctx = GrMan;
    ctx->RenderTriStrip(4, (tBxVertex*)v, 0);
}

extern "C" void tHUDDef_getPos(void*, void*);

extern "C" void tHUDDef_getSize(void*, void*, int);

void tHUDDef_getScale(void*, void*);

extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);

/**
 * Draws a vertical gauge from one of the HUD's boost texture sets: a Bottom piece, Mid Stacker pieces and a Top piece,
 * filled from the bottom by @p fill.
 *
 * The textures are boost[@p idx] of the HUD state (+0x4FC + idx * 0xC), three that cOVStateHUD1P_onCreateScreen looks
 * up as "<set> Top", "<set> Mid Stacker" and "<set> Bottom" for the sets Empty Coil, Empty Stem BG, White Coil, White
 * Stem BG and White Stem Fill. Element @p sel of @p elems gives position, size and scale; the scale is multiplied by
 * (@p sx, @p sy), the size by that scale, and cOVStateHUD1P_calcElemPos places the element with its alignment and the reflect
 * flags @p x. The element's layer byte (+0x22) goes to bits 5-9 of the current draw state's word +0x8. All four
 * vertices get @p col times 128 and q = 1; @p col is A, R, G, B (the tHUDDef colour order) and the vertex words are
 * R, G, B, A (the GS RGBAQ order), so col[0] goes to the last word.
 *
 * The Bottom and Mid Stacker pieces are 23 tall and the Top 31, times the element's y scale and bar->scale (On Tour
 * tBarConfig partScale), with bar->count (numMidSegments) Mid Stacker pieces between them. Each piece spans the
 * element's width, and the stack is drawn upwards from the element's bottom edge; its height comes from the piece
 * heights, not from the element's. With @p fill exactly 1.0 the whole stack is drawn and @p rect, when not null,
 * receives the element's x, y, width and height. Otherwise the fill line lies @p fill of the element height above its
 * bottom: @p rect gets x, the fill line, the width and the filled height, the pieces are drawn bottom up, and the piece
 * that reaches the fill line is cut there (its top v moved in proportion), after which the function returns. Each quad
 * goes through GrMan's virtual slot 71. cOVStateHUD1P_onRender2D calls it three times.
 *
 * @param fill  fill fraction, 0..1; exactly 1.0 takes the unclipped path
 * @param sx  extra x scale on the element's scale
 * @param sy  extra y scale on the element's scale
 * @param elems  the HUD element table (0x24-byte tHUDDef records)
 * @param sel  index of the gauge's element in @p elems
 * @param idx  which boost texture set (0-4) to draw
 * @param bar  the piece config (tBarConfig: scale of the piece heights, number of Mid Stacker pieces)
 * @param col  vertex colour, A, R, G, B (0..1)
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos (bit 0 mirrors x about 640)
 * @param rect  optional output: x, y, width and height of the drawn (filled) area
 */
extern "C" void cOVStateHUD1P_renderBoostPart(float fill, float sx, float sy, char* self, char* elems, signed char sel, int idx,
                              sGaugeBar* bar, float* col, int x, float* rect)
{
    t2Vector a;
    t2Vector drawPos;
    t2Vector size;
    t2Vector sc;
    t2Vector one;
    sHudSprite** sprites = (sHudSprite**)(self + (idx * 0xC + 0x4FC));
    char* hudDef = elems + sel * 36;
    tHUDDef_getPos(hudDef, &a);
    tHUDDef_getSize(hudDef, &size, 0);
    tHUDDef_getScale(hudDef, &sc);
    sc.x *= sx;
    sc.y *= sy;
    size.x *= sc.x;
    size.y *= sc.y;
    sc.y *= bar->scale;
    one.x = 1.0f;
    one.y = 1.0f;
    cOVStateHUD1P_calcElemPos(self, &drawPos, &a, &size, &one, hudDef[0x20], hudDef[0x21], x);
    int* w = (int*)&GrMan->m_pCurrentDrawState->m_bitGroups[2];
    *w = (*w & ~0x3E0) | ((hudDef[0x22] << 5) & 0x3E0);
    sHudVertex vtx[4];
    sHudVertex* verts = vtx;
    for (int i = 0; i < 4; i++) {
        int vtxAlpha = (int)(col[0] * 128.0f);
        int vtxRed = (int)(col[1] * 128.0f);
        int vtxGreen = (int)(col[2] * 128.0f);
        int vtxBlue = (int)(col[3] * 128.0f);
        vtx[i].a = vtxAlpha;
        vtx[i].r = vtxRed;
        vtx[i].g = vtxGreen;
        vtx[i].b = vtxBlue;
        vtx[i].q = 1.0f;
        vtx[i].pos.setZW(0.0f, 1.0f);
    }
    if (fill == 1.0f) {
        if (rect != 0) {
            rect[0] = drawPos.x;
            rect[1] = drawPos.y;
            rect[2] = size.x;
            rect[3] = size.y;
        }
        drawPos.y += size.y;
        {
            sHudSprite* pieceSprite = sprites[2];
            setTex(pieceSprite);
            float pieceHeight = sc.y * 23.0f;
            setUV(vtx, pieceSprite);
            setPos(vtx, drawPos.x, drawPos.y - pieceHeight, drawPos.x + size.x, drawPos.y);
            drawQuad(verts);
            drawPos.y -= pieceHeight;
        }
        {
            sHudSprite* pieceSprite = sprites[1];
            setTex(pieceSprite);
            float pieceHeight = sc.y * 23.0f;
            setUV(vtx, pieceSprite);
            for (int i = 0; i < bar->count; i++) {
                setPos(vtx, drawPos.x, drawPos.y - pieceHeight, drawPos.x + size.x, drawPos.y);
                drawQuad(verts);
                drawPos.y -= pieceHeight;
            }
        }
        {
            sHudSprite* pieceSprite = sprites[0];
            setTex(pieceSprite);
            setUV(vtx, pieceSprite);
            setPos(vtx, drawPos.x, drawPos.y - sc.y * 31.0f, drawPos.x + size.x, drawPos.y);
            drawQuad(verts);
        }
    } else {
        float bottom = drawPos.y + size.y;
        float top = drawPos.y + size.y * (1.0f - fill);
        if (rect != 0) {
            rect[0] = drawPos.x;
            rect[3] = bottom - top;
            rect[2] = size.x;
            rect[1] = top;
        }
        {
            sHudSprite* pieceSprite = sprites[2];
            float pieceHeight = sc.y * 23.0f;
            float y = bottom - pieceHeight;
            float tv = pieceSprite->v0;
            int clipped = y <= top;
            if (clipped) {
                tv = tv + (pieceSprite->v1 - tv) * (top - y) / pieceHeight;
                y = top;
            }
            setTex(pieceSprite);
            vtx[0].u = pieceSprite->u0;
            vtx[0].v = tv;
            vtx[1].u = pieceSprite->u1;
            vtx[1].v = tv;
            vtx[2].u = pieceSprite->u0;
            vtx[2].v = pieceSprite->v1;
            vtx[3].u = pieceSprite->u1;
            vtx[3].v = pieceSprite->v1;
            setPos(vtx, drawPos.x, y, drawPos.x + size.x, bottom);
            drawQuad(verts);
            bottom = y;
            if (clipped)
                return;
        }
        {
            sHudSprite* pieceSprite = sprites[1];
            float pieceHeight = sc.y * 23.0f;
            setTex(pieceSprite);
            setUV(vtx, pieceSprite);
            for (int i = 0; i < bar->count; i++) {
                float y = bottom - pieceHeight;
                int last = y <= top;
                if (last) {
                    float tv = pieceSprite->v0 + (pieceSprite->v1 - pieceSprite->v0) * (top - y) / pieceHeight;
                    vtx[0].v = tv;
                    vtx[1].v = tv;
                    y = top;
                }
                setPos(vtx, drawPos.x, y, drawPos.x + size.x, bottom);
                drawQuad(verts);
                bottom = y;
                if (last)
                    return;
            }
        }
        {
            sHudSprite* pieceSprite = sprites[0];
            float pieceHeight = sc.y * 31.0f;
            float y = bottom - pieceHeight;
            float tv = pieceSprite->v0;
            int clipped = y <= top;
            if (clipped) {
                tv = tv + (pieceSprite->v1 - tv) * (top - y) / pieceHeight;
                y = top;
            }
            setTex(pieceSprite);
            vtx[0].u = pieceSprite->u0;
            vtx[0].v = tv;
            vtx[1].u = pieceSprite->u1;
            vtx[1].v = tv;
            vtx[2].u = pieceSprite->u0;
            vtx[2].v = pieceSprite->v1;
            vtx[3].u = pieceSprite->u1;
            vtx[3].v = pieceSprite->v1;
            setPos(vtx, drawPos.x, y, drawPos.x + size.x, bottom);
            drawQuad(verts);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderWipeoutMeter);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// EA's 2-float vector (prototype name, e.g. setTileUV__7cUIIconG8t2Vector).


// Vertex position of the HUD sprite quads: 16-byte aligned, with an empty default ctor (vertex arrays get a vec_new
// loop) and the (x, y, z, const float& w) ctor the quad builders use.




// The render context D_004A289C_D9A0 as these functions use it: GS state block pointer at 0xE84, vptr at 0x10D8 (slot
// 71
// draws a vertex list).



// An ARGB colour as the element colour getter tHUDDef_getColour writes it and cOVStateHUD1P_renderBitmap takes it
// (the alpha comes first; the fades scale it).
struct sColour {
    float a, r, g, b;
};

// A HUD sprite: texture id and UV rectangle.


// Gauge layout: segment scale and number of middle segments.




static inline void setTex_D9A0(sHudSprite* s)
{
    *GrMan->m_pCurrentDrawState->m_textureState.m_texid = texOf(s);
}



static inline void setUVFlip(sHudVertex* v, sHudSprite* s)
{
    v[0].u = s->u1;
    v[0].v = s->v0;
    v[1].u = s->u0;
    v[1].v = s->v0;
    v[2].u = s->u1;
    v[2].v = s->v1;
    v[3].u = s->u0;
    v[3].v = s->v1;
}

static inline void drawQuad_D9A0(sHudVertex* v)
{
    cPS2GraphicsMan* ctx = GrMan;
    ctx->RenderTriStrip(4, (tBxVertex*)v, 0);
}

extern "C" void tHUDDef_getPos(void*, void*);

extern "C" void tHUDDef_getSize(void*, void*, int);

void tHUDDef_getScale(void*, void*);

extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);

extern "C" void* cBE_getBE();

cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

void tHUDDef_getColour(void*, void*);

extern "C" void drawTextPictLine(void*, void*, void*, void*, int, int);

struct sPlayerHud_D9A0 {
    char pad[0x48];
    int rider;              // 0x48
    char pad4C[0x8C - 0x4C];
};
struct sHudRoot_D9A0 {
    sPlayerHud_D9A0 players[2];     // 0x0
    char pad118[0x494 - 0x118];
    sHudSprite* sprites[0x20];    // 0x494
};
extern int D_004A4E98;
extern float D_004A5B28;
extern float D_004A5B2C;
extern sColour D_004C8668;
extern "C" int cBENewPlayerInterface_getControllerMapping(void* iface, int a1);
extern "C" void cOVStateHUD1P_renderQuad(void* self, void* pos, void* size, void* scale, void* col);

/**
 * Draws the wipeout recovery meter: a square-button icon, a capped horizontal bar and its fill, plus the RECOVER help
 * line when @p icon is set.
 *
 * The controller mapping (cBENewPlayerInterface_getControllerMapping on back-end interface 1) of panel @p idx's rider
 * (the aiIndex at +0x48 + idx * 0x8C) picks between two variants. With @p icon nonzero, element 55 of @p elems (+0x7BC)
 * gives position, scale and colour for drawTextPictLine, which draws the text-and-picture line at +0x6AC (mapping 0) or
 * the one at +0x7F8 (otherwise); cOVStateHUD1P_onCreateScreen builds both from "RECOVER = @square".
 *
 * The meter is element 54 (+0x798): its size times its scale times @p scale, placed by cOVStateHUD1P_calcElemPos with the element's
 * alignment and @p idx as the reflect flags, on the element's layer (bits 5-9 of the draw state word +0x8). The
 * vertices are white with alpha @p alpha * 128. From left to right: the button icon icons[18] (mapping 0) or icons[19]
 * (both "square NML"), 20 by 20 scaled and centred vertically; a gap of 3 (scaled); the "Wipeout Bar Cap" icons[16], 9
 * * bar->scale wide (scaled); when bar->count > 0 the "Wipeout Bar Mid" icons[17] stretched over the rest of the width
 * less two caps; and the cap again with its u mirrored. When @p fill > 0 an untextured quad (cOVStateHUD1P_renderQuad) in
 * D_004C8668, its alpha times @p alpha, one layer above the bar, fills (bar width - 2 * inset) * @p fill, the inset
 * being 3 (scaled; the function-static pair D_004A5B28 / D_004A5B2C, set once under the guard D_004A4E98).
 *
 * @param fill  fill fraction of the bar; nothing is filled at 0 or below
 * @param alpha  opacity multiplier for the whole meter
 * @param elems  the HUD element table (0x24-byte tHUDDef records)
 * @param bar  the bar config (tBarConfig: cap scale, middle-piece count)
 * @param idx  reflect flags for cOVStateHUD1P_calcElemPos, and also the panel (0 or 1) whose rider's controller mapping is read
 * @param icon  nonzero to draw the RECOVER help line
 * @param scale  x / y scale multiplier for the meter element
 */
extern "C" void cOVStateHUD1P_renderWipeoutMeter(float fill, float alpha, cOVStateHUD1P* self, char* elems, sGaugeBar* bar, int idx,
                              int icon, float* scale)
{
    int alt = cBENewPlayerInterface_getControllerMapping(cBE_getInterface_Fv(cBE_getBE(), 1), ((sHudRoot_D9A0*)self)->players[idx].rider);
    t2Vector elemPos;
    t2Vector sc;
    sVtxPos colourOrSize;     // icon colour first, then reused for the bar size
    t2Vector pos;
    t2Vector drawPos;
    t2Vector scratchVec;
    if (icon != 0) {
        char* ie = elems + 0x7BC;
        tHUDDef_getPos(ie, &elemPos);
        tHUDDef_getScale(ie, &sc);
        tHUDDef_getColour(ie, &colourOrSize);
        if (alt == 0) {
            pos = elemPos;
            drawTextPictLine(&self->mTextRecoverHelp, &pos, &sc, &colourOrSize, elems[0x7DC], elems[0x7DD]);
        } else {
            pos = elemPos;
            drawTextPictLine(&self->mTextRecoverHelpAlt, &pos, &sc, &colourOrSize, elems[0x7DC], elems[0x7DD]);
        }
    }
    char* meterDef = elems + 0x798;
    tHUDDef_getPos(meterDef, &elemPos);
    tHUDDef_getSize(meterDef, &colourOrSize, 0);
    tHUDDef_getScale(meterDef, &sc);
    sc.x *= scale[0];
    sc.y *= scale[1];
    colourOrSize.x *= sc.x;
    colourOrSize.y *= sc.y;
    scratchVec.x = 1.0f;
    scratchVec.y = 1.0f;
    cOVStateHUD1P_calcElemPos(self, &drawPos, &elemPos, &colourOrSize, &scratchVec, elems[0x7B8], elems[0x7B9], idx);
    int* w = (int*)&GrMan->m_pCurrentDrawState->m_bitGroups[2];
    *w = (*w & ~0x3E0) | ((elems[0x7BA] << 5) & 0x3E0);
    sHudVertex vtx[4];
    sHudVertex* verts = vtx;
    for (int i = 0; i < 4; i++) {
        int vtxAlpha = (int)(alpha * 128.0f);
        vtx[i].a = vtxAlpha;
        vtx[i].r = 0x80;
        vtx[i].g = 0x80;
        vtx[i].b = 0x80;
        vtx[i].q = 1.0f;
    }
    {
        sHudSprite* pieceSprite = ((sHudRoot_D9A0*)self)->sprites[alt == 0 ? 18 : 19];
        scratchVec.x = sc.x * 20.0f;
        scratchVec.y = sc.y * 20.0f;
        t2Vector iconPos;
        iconPos.x = drawPos.x;
        iconPos.y = drawPos.y + (colourOrSize.y - scratchVec.y) * 0.5f;
        setTex_D9A0(pieceSprite);
        setUV(vtx, pieceSprite);
        vtx[0].pos = sVtxPos(iconPos.x, iconPos.y, 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(iconPos.x + scratchVec.x, iconPos.y, 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(iconPos.x, iconPos.y + scratchVec.y, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(iconPos.x + scratchVec.x, iconPos.y + scratchVec.y, 0.0f, 1.0f);
        drawQuad_D9A0(verts);
    }
    float iconSpan = scratchVec.x + sc.x * 3.0f;
    drawPos.x += iconSpan;
    colourOrSize.x -= iconSpan;
    scratchVec = drawPos;
    float segW = sc.x * (bar->scale * 9.0f);
    {
        sHudSprite* pieceSprite = ((sHudRoot_D9A0*)self)->sprites[0x10];
        setTex_D9A0(pieceSprite);
        setUV(vtx, pieceSprite);
        vtx[0].pos = sVtxPos(drawPos.x, drawPos.y, 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(drawPos.x + segW, drawPos.y, 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(drawPos.x, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(drawPos.x + segW, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        drawQuad_D9A0(verts);
        drawPos.x += segW;
    }
    if (bar->count > 0) {
        sHudSprite* pieceSprite = ((sHudRoot_D9A0*)self)->sprites[17];
        setTex_D9A0(pieceSprite);
        float midW = colourOrSize.x - (segW + segW);
        setUV(vtx, pieceSprite);
        vtx[0].pos = sVtxPos(drawPos.x, drawPos.y, 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(drawPos.x + midW, drawPos.y, 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(drawPos.x, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(drawPos.x + midW, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        drawQuad_D9A0(verts);
        drawPos.x += midW;
    }
    {
        sHudSprite* pieceSprite = ((sHudRoot_D9A0*)self)->sprites[0x10];
        setTex_D9A0(pieceSprite);
        setUVFlip(vtx, pieceSprite);
        vtx[0].pos = sVtxPos(drawPos.x, drawPos.y, 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(drawPos.x + segW, drawPos.y, 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(drawPos.x, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(drawPos.x + segW, drawPos.y + colourOrSize.y, 0.0f, 1.0f);
        drawQuad_D9A0(verts);
        drawPos.x += segW;
    }
    if (fill <= 0.0f)
        return;
    {
        if (D_004A4E98 == 0) {
            D_004A4E98 = 1;
            D_004A5B28 = 3.0f;
            D_004A5B2C = 3.0f;
        }
        t2Vector fillInset;
        fillInset.x = D_004A5B28 * sc.x;
        fillInset.y = D_004A5B2C * sc.y;
        t2Vector fs;
        fs.x = (colourOrSize.x - (fillInset.x + fillInset.x)) * fill;
        fs.y = colourOrSize.y - (fillInset.y + fillInset.y);
        sColour col = D_004C8668;
        col.a *= alpha;
        scratchVec.x += fillInset.x;
        scratchVec.y += fillInset.y;
        int* w2 = (int*)&GrMan->m_pCurrentDrawState->m_bitGroups[2];
        *w2 = (*w2 & ~0x3E0) | (((elems[0x7BA] + 1) << 5) & 0x3E0);
        t2Vector one;
        one.x = 1.0f;
        one.y = 1.0f;
        cOVStateHUD1P_renderQuad(self, &scratchVec, &fs, &one, &col);
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderRank);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).

// EA's 2-float vector (prototype name, e.g. setTileUV__7cUIIconG8t2Vector).
// The render context D_004A289C_E1B0 as these functions use it: GS state block pointer at 0xE84, vptr at 0x10D8 (slot
// 71
// draws a vertex list).

// Four floats, 4-byte aligned (not a t4Vector, which is 16-aligned): the 4-float slots some callers give the element
// position / scale getters.
struct sFloat4 {
    float x, y, z, w;
};


extern float cSinCosTable_gSinCosTable[];

extern "C" void tHUDDef_getPos(void*, void*);

extern "C" void tHUDDef_getSize(void*, void*, int);

void tHUDDef_getScale(void*, void*);

extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);

extern "C" void* cBE_getBE();

cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

void tHUDDef_getColour(void*, void*);

extern "C" void cOVStateHUD1P_renderBitmap(void*, int, void*, void*, void*, void*);

extern char D_00477F30[];
extern "C" void cOVStateHUD1P_renderGlow_E1B0(char* self, sColour* colp, t2Vector* pos, t2Vector* size,
                                   float t, float sx, float sy, float amin, float amax) __asm__("cOVStateHUD1P_renderGlow");

/**
 * Draws a rider's race place: the place digit with its ordinal suffix and, under the suffix, a slash and the number of
 * riders in the race.
 *
 * The rider is AI->Riders[aiIndex] (SSXApp->Game->AI; aiIndex from @p rider) and its place is cRider::RaceRanking (0-based;
 * the ranking pass writes it). Element 24 of @p obj (+0x360) gives position and scale and D_00477F30, a 9-entry tHUDDef
 * table, the rest: entry [place] the digit (size fallback: the digit texture w444[place], "nm1w".."nm6w"), entry 6 the
 * suffix, entry 7 the slash, entry 8 the total. cOVStateHUD1P_calcElemPos places the digit-plus-suffix box with element 24's
 * alignment and @p x, on element 24's layer. Under the suffix it draws the slash (w444[10], "slash") in entry 7's
 * colour and then w444[n - 1], n being cBENewRaceInterface_newRace.iNumberRiders, in entry 8's colour. Then it draws
 * the digit w444[place] and the suffix w444[6 + min(place, 3)] ("st_w", "nd_w", "rd_w", "th_w") in entry [place]'s
 * colour. The result of the back-end interface 0 lookup is discarded.
 *
 * Animation from the panel: while its f48 (+0x48; On Tour rankAnimFrame) is >= 0, first place scales the digit and
 * suffix by 1 + 0.2 * sin(4 pi f48) * (1 - f48^2) (sine table cSinCosTable_gSinCosTable) and the other places multiply the colour's
 * alpha by 0.5 + f48 / 2. While f4C (+0x4C; On Tour rankGlowFrame) is >= 0, cOVStateHUD1P_renderGlow draws the glow over the digit
 * and over the suffix, one layer below, with phase f4C, grown 1.81 times and alpha 0.5 to 1.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 * @param rider  the HUD panel (sRiderPanel in ovstatehud; On Tour tHUDRiderInfo): aiIndex at +0, animation frames at
 *               +0x48 / +0x4C
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos
 */
void cOVStateHUD1P::renderRank(char* obj, int* rider, int x)
{
    t2Vector scale;
    sColour col;
    t2Vector sc;
    sFloat4 elemPos;
    t2Vector size1;
    t2Vector size2;
    t2Vector dim;
    t2Vector drawPos;
    sColour totalColour;
    t2Vector size3;
    t2Vector suffixPos;
    t2Vector pa;
    t2Vector sa;
    t2Vector sb;
    scale.x = 1.0f;
    scale.y = 1.0f;
    float k = 1.0f;
    cRider* panelRider = SSXApp->Game->AI->Riders[rider[0]];
    tHUDDef_getColour(D_00477F30 + panelRider->RaceRanking * 36, &col);
    float rankAnimFrame = ((float*)rider)[0x12];
    if (rankAnimFrame >= 0.0f) {
        if (panelRider->RaceRanking == 0) {
            k = cSinCosTable_gSinCosTable[(int)(rankAnimFrame * 12.566370964050293f * 81.4873275756836f) & 0x1FF] * 0.199636310338974f * (1.0f - rankAnimFrame * rankAnimFrame) + k;
        } else {
            col.a = col.a * (rankAnimFrame * 0.5f + 0.5f);
        }
    }
    tHUDDef_getScale(obj + 0x360, &sc);
    int* tex = (int*)(&mpNumWhite[0]);
    tHUDDef_getPos(obj + 0x360, &elemPos);
    tHUDDef_getSize(D_00477F30 + 0xD8, &size1, 0);
    size1.x *= sc.x;
    size1.y *= sc.y;
    int off = panelRider->RaceRanking << 2;
    tHUDDef_getSize(D_00477F30 + panelRider->RaceRanking * 36, &size2, *(int*)((char*)tex + off));
    size2.x *= sc.x;
    size2.y *= sc.y;
    dim.x = size2.x + size1.x;
    dim.y = size2.y;
    cOVStateHUD1P_calcElemPos(((char*)this), &drawPos, &elemPos, &dim, &scale, obj[0x380], obj[0x381], x);
    drawPos.x += size2.x;
    drawPos.y += size1.y;
    int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | ((obj[0x382] << 5) & 0x3E0);
    tHUDDef_getColour(D_00477F30 + 0xFC, &totalColour);
    tHUDDef_getSize(D_00477F30 + 0xFC, &size3, 0);
    size3.x *= sc.x;
    size3.y *= sc.y;
    cOVStateHUD1P_renderBitmap(((char*)this), (int)mpNumSlash, &drawPos, &size3, &scale, &totalColour);
    drawPos.x += size3.x;
    tHUDDef_getColour(D_00477F30 + 0x120, &totalColour);
    tHUDDef_getSize(D_00477F30 + 0x120, &size3, 0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int off3 = (cBENewRaceInterface_newRace.iNumberRiders - 1) << 2;
    cOVStateHUD1P_renderBitmap(((char*)this), *(int*)((char*)tex + off3), &drawPos, &size3, &sc, &totalColour);
    if (k != 1.0f) {
        size2.x *= k;
        size2.y *= k;
        size1.x *= k;
        size1.y *= k;
        dim.x *= k;
        dim.y *= k;
    }
    cOVStateHUD1P_calcElemPos(((char*)this), &drawPos, &elemPos, &dim, &scale, obj[0x380], obj[0x381], x);
    w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | ((obj[0x382] << 5) & 0x3E0);
    int off4 = panelRider->RaceRanking << 2;
    cOVStateHUD1P_renderBitmap(((char*)this), *(int*)((char*)tex + off4), &drawPos, &size2, &scale, &col);
    signed char n = panelRider->RaceRanking < 4 ? panelRider->RaceRanking : 3;
    suffixPos = drawPos;
    int off5 = n * 4;
    int tx = *(int*)(((char*)this) + off5 + 0x45C);
    suffixPos.x += size2.x;
    cOVStateHUD1P_renderBitmap(((char*)this), tx, &suffixPos, &size1, &scale, &col);
    if (((float*)rider)[0x13] >= 0.0f) {
        w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
        *w = (*w & ~0x3E0) | (((obj[0x382] - 1) << 5) & 0x3E0);
        pa = drawPos;
        sa = size2;
        cOVStateHUD1P_renderGlow_E1B0(((char*)this), &col, &pa, &sa, ((float*)rider)[0x13], 1.8101750612258911f,
                1.8101750612258911f, 0.5f, 1.0f);
        pa = suffixPos;
        sb = size1;
        cOVStateHUD1P_renderGlow_E1B0(((char*)this), &col, &pa, &sb, ((float*)rider)[0x13], 1.8101750612258911f,
                1.8101750612258911f, 0.5f, 1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_calcElemPosX);
#ifdef SKIP_ASM
/**
 * Returns an element's left edge from its x position, width and scale, its horizontal alignment and the reflect flags.
 *
 * With reflect bit 0 the position is mirrored about x = 640 and the alignment negated, unless bit 1 is also set (then
 * the alignment is kept). Alignment 0 centres the element (@p x - @p y * @p z / 2), a positive one right-aligns it
 * (@p x - @p y * @p z), a negative one leaves @p x as the left edge. cOVStateHUD1P_calcElemPos uses it for x, and
 * cOVStateHUD1P_onRender2D calls it directly.
 *
 * @param hudState  the HUD state (this); not read
 * @param align  horizontal alignment (the element's alignX; On Tour pivotX)
 * @param flags  reflect flags: bit 0 mirrors, bit 1 keeps the alignment when mirroring
 * @param x  the element's x position
 * @param y  the element's width
 * @param z  the element's x scale
 * @return the left edge
 */
extern "C" float cOVStateHUD1P_calcElemPosX(int hudState, int align, int flags, float x, float y, float z) {
    int effAlign = align;
    if (flags & 1) {
        x = 640.0f - x;
        effAlign = (flags & 2) ? align : -effAlign;
    }
    if (effAlign == 0) {
        x -= z * y * 0.5f;
    } else if (effAlign > 0) {
        x -= z * y;
    }
    return x;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_calcElemPosY);
#ifdef SKIP_ASM
/**
 * Returns an element's top edge from its y position, height and scale and its vertical alignment.
 *
 * Alignment 0 centres the element (@p pos - @p width * @p scale / 2), a positive one bottom-aligns it (@p pos
 * - @p width * @p scale), a negative one leaves @p pos as the top edge. Unlike cOVStateHUD1P_calcElemPosX there is no reflection.
 * cOVStateHUD1P_calcElemPos uses it for y, and cOVStateHUD1P_onRender2D calls it directly.
 *
 * @param hudState  the HUD state (this); not read
 * @param mode  vertical alignment (the element's alignY; On Tour pivotY)
 * @param pos  the element's y position
 * @param width  the element's height, despite the name
 * @param scale  the element's y scale
 * @return the top edge
 */
extern "C" float cOVStateHUD1P_calcElemPosY(int hudState, int mode, float pos, float width, float scale) {
    if (mode == 0) {
        pos -= scale * width * 0.5f;
    } else if (mode > 0) {
        pos -= scale * width;
    }
    return pos;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderGlow);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct Col_E7E0 { float r, g, b, a; };
struct Ctx_E7E0 { char pad[0xE84]; char* e84; };
extern "C" void cOVStateHUD1P_renderGlowStrip_E7E0(void* self, Col_E7E0* col, float* pos, float w, float h, float u0, float v0, float u1, float v1) __asm__("cOVStateHUD1P_renderGlowStrip");

/**
 * Draws the HUD's glow texture as a pulsing halo over a box, grown about its centre, with the alpha following @p phase.
 *
 * Copies *@p colp and multiplies its alpha (the first component) by @p amin + (@p amax - @p amin) * w, w being @p phase
 * up to 1 and 2 - @p phase above (a triangle over 0..2). Grows the box by (@p sx - 1) and (@p sy - 1) times its size,
 * half on each side, and writes the grown position and size back into @p pos and @p size. Binds the glow texture
 * element at +0x474 (On Tour mGlow: its id, which cOVStateHUD1P_onCreateScreen takes from GrMan's _EffectTexMap[0], and
 * its UV top +0x480, left +0x484, right +0x488, bottom +0x48C) into the draw state (+0x10). A box wider than 72 is
 * drawn as three columns through cOVStateHUD1P_renderGlowStrip: 36 wide from the left u to the middle u, a stretched middle column at
 * the middle u, and 36 wide from the middle u to the right u; a narrower box is one column over the full u range.
 * cOVStateHUD1P_renderGlowStrip splits a tall column into rows the same way, so the glow is a nine-slice. In the three-column case
 * pos[0] is left at the right column's x. Callers: cOVStateHUD1P_renderRank and cOVStateHUD1P_onRender2D; cOVStateHUD1P_renderUberChar has an
 * inlined copy.
 *
 * @param colp  the colour (A, R, G, B); not modified
 * @param pos  the box's x, y; overwritten with the grown box's (x then advanced as above)
 * @param size  the box's width, height; overwritten with the grown size
 * @param phase  animation phase, 0..2 (the alpha peaks at 1)
 * @param sx  x growth factor
 * @param sy  y growth factor
 * @param amin  alpha factor at phase 0 and 2
 * @param amax  alpha factor at phase 1
 */
extern "C" void cOVStateHUD1P_renderGlow(cOVStateHUD1P* self, Col_E7E0* colp, float* pos, float* size,
                              float phase, float sx, float sy, float amin, float amax) {
    Col_E7E0 col = *colp;
    Col_E7E0* colour = &col;
    float dw, dh;
    float range = amax - amin;
    colour->r = colour->r * (amin + (phase > 1.0f ? range * (2.0f - phase) : range * phase));
    dw = size[0] * (sx - 1.0f);
    dh = size[1] * (sy - 1.0f);
    pos[0] -= dw * 0.5f;
    pos[1] -= dh * 0.5f;
    size[0] += dw;
    size[1] += dh;
    int id = self->mGlow.mSourceShape;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = id;
    if (size[0] > 72.0f) {
        float mid = (self->mGlow.mLeft + self->mGlow.mRight) * 0.5f;
        cOVStateHUD1P_renderGlowStrip_E7E0(((char*)self), colour, pos, 36.0f, size[1], self->mGlow.mLeft, self->mGlow.mTop, mid, self->mGlow.mBottom);
        pos[0] += 36.0f;
        cOVStateHUD1P_renderGlowStrip_E7E0(((char*)self), colour, pos, size[0] - 72.0f, size[1], mid, self->mGlow.mTop, mid, self->mGlow.mBottom);
        pos[0] += size[0] - 72.0f;
        cOVStateHUD1P_renderGlowStrip_E7E0(((char*)self), colour, pos, 36.0f, size[1], mid, self->mGlow.mTop, self->mGlow.mRight, self->mGlow.mBottom);
    } else {
        cOVStateHUD1P_renderGlowStrip_E7E0(((char*)self), colour, pos, size[0], size[1], self->mGlow.mLeft, self->mGlow.mTop, self->mGlow.mRight, self->mGlow.mBottom);
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderGlowStrip);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// Vertex position of the HUD sprite quads: 16-byte aligned, with an empty default ctor (vertex arrays get a vec_new
// loop) and the (x, y, z, const float& w) ctor the quad builders use.




// The render context D_004A289C_EA00 as these functions use it: GS state block pointer at 0xE84, vptr at 0x10D8 (slot
// 71
// draws a vertex list).



/**
 * Draws one column of the glow texture (see cOVStateHUD1P_renderGlow): one quad, or for a height over 72 three rows whose top and
 * bottom rows are 36 tall.
 *
 * Every vertex gets the colour @p col times 128 (tHUDDef colours are A, R, G, B; the vertex takes R, G, B, A),
 * q = 1, and u = @p uMin on the even (left) and @p uMax on the odd (right) vertices. Above 72
 * it builds 8 vertices at y = @p pos[1], + 36, + @p height - 36 and + @p height with v = @p vMin, the middle v twice
 * and @p vMax, so the middle row stretches the texture's centre line; otherwise 4 vertices from @p vMin to @p vMax. It
 * draws them through GrMan's virtual slot 71 (vertex count 8 or 4). The texture must already be bound; the HUD state
 * (self) is not read.
 *
 * @param col  the colour (A, R, G, B, 0..1)
 * @param pos  the column's top-left x, y
 * @param w  the column's width
 * @param height  the column's height
 * @param uMin  left u
 * @param vMin  top v
 * @param uMax  right u
 * @param vMax  bottom v
 */
extern "C" void cOVStateHUD1P_renderGlowStrip(cOVStateHUD1P* self, sColour* col, float* pos, float w, float height, float uMin, float vMin, float uMax, float vMax)
{
    if (height > 72.0f) {
        sHudVertex vtx[8];
        for (int i = 0; i < 8; i++) {
            int alpha = (int)(col->a * 128.0f);
            int red = (int)(col->r * 128.0f);
            int green = (int)(col->g * 128.0f);
            int blue = (int)(col->b * 128.0f);
            vtx[i].a = alpha;
            vtx[i].r = red;
            vtx[i].g = green;
            vtx[i].b = blue;
            vtx[i].q = 1.0f;
            if (i & 1)
                vtx[i].u = uMax;
            else
                vtx[i].u = uMin;
        }
        vtx[0].v = vMin;
        vtx[1].v = vMin;
        vtx[2].v = (vMin + vMax) * 0.5f;
        vtx[3].v = (vMin + vMax) * 0.5f;
        vtx[4].v = (vMin + vMax) * 0.5f;
        vtx[5].v = (vMin + vMax) * 0.5f;
        vtx[6].v = vMax;
        vtx[7].v = vMax;
        vtx[0].pos = sVtxPos(pos[0], pos[1], 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(pos[0] + w, pos[1], 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(pos[0], pos[1] + 36.0f, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(pos[0] + w, pos[1] + 36.0f, 0.0f, 1.0f);
        vtx[4].pos = sVtxPos(pos[0], pos[1] + height - 36.0f, 0.0f, 1.0f);
        vtx[5].pos = sVtxPos(pos[0] + w, pos[1] + height - 36.0f, 0.0f, 1.0f);
        vtx[6].pos = sVtxPos(pos[0], pos[1] + height, 0.0f, 1.0f);
        vtx[7].pos = sVtxPos(pos[0] + w, pos[1] + height, 0.0f, 1.0f);
        cPS2GraphicsMan* ctx = GrMan;
        ctx->RenderTriStrip(8, (tBxVertex*)vtx, 0);
    } else {
        sHudVertex vtx[4];
        for (int i = 0; i < 4; i++) {
            int alpha = (int)(col->a * 128.0f);
            int red = (int)(col->r * 128.0f);
            int green = (int)(col->g * 128.0f);
            int blue = (int)(col->b * 128.0f);
            vtx[i].a = alpha;
            vtx[i].r = red;
            vtx[i].g = green;
            vtx[i].b = blue;
            vtx[i].q = 1.0f;
            if (i & 1)
                vtx[i].u = uMax;
            else
                vtx[i].u = uMin;
        }
        vtx[0].v = vMin;
        vtx[1].v = vMin;
        vtx[2].v = vMax;
        vtx[3].v = vMax;
        vtx[0].pos = sVtxPos(pos[0], pos[1], 0.0f, 1.0f);
        vtx[1].pos = sVtxPos(pos[0] + w, pos[1], 0.0f, 1.0f);
        vtx[2].pos = sVtxPos(pos[0], pos[1] + height, 0.0f, 1.0f);
        vtx[3].pos = sVtxPos(pos[0] + w, pos[1] + height, 0.0f, 1.0f);
        cPS2GraphicsMan* ctx = GrMan;
        ctx->RenderTriStrip(4, (tBxVertex*)vtx, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderUberChar);
#ifdef SKIP_ASM
struct V2_ED48 { float x, y; };
struct V4_ED48 { float x, y, z, w; };
struct Ent_ED48 { signed char a, b, c, d; };
struct Tab_ED48 { Ent_ED48 e[9]; } __attribute__((aligned(4)));
struct Str_ED48 { char c[2]; };
extern Tab_ED48 D_0046ED40;
extern Str_ED48 D_004A22E8[];
extern char* D_004A21C8;
extern "C" void tHUDDef_getPos(void*, void*);
extern "C" void tHUDDef_getSize(void*, void*, int);
void tHUDDef_getColour(void*, void*);
extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);
extern "C" void cOVStateHUD1P_renderBitmap(void*, int, void*, void*, void*, void*);
extern "C" void cFont_text(void*, float, float, char*);

// Inline copy of cOVStateHUD1P_renderGlow's body (the original helper was inlined here): colour/position/size by pointer, copied
// locally.
static inline void draw_ED48(char* vvself, const Col_E7E0* colp, const V2_ED48* posp, const V2_ED48* sizep,
                             float t, float sx, float sy, float amin, float amax) {
    cOVStateHUD1P* vself = (cOVStateHUD1P*)vvself;
    cOVStateHUD1P* self = (cOVStateHUD1P*)vself;
    Col_E7E0 col = *colp;
    V2_ED48 pos = *posp;
    V2_ED48 size = *sizep;
    float dw, dh;
    float range = amax - amin;
    col.r = col.r * (amin + (t > 1.0f ? range * (2.0f - t) : range * t));
    dw = size.x * (sx - 1.0f);
    dh = size.y * (sy - 1.0f);
    pos.x -= dw * 0.5f;
    pos.y -= dh * 0.5f;
    size.x += dw;
    size.y += dh;
    float* pp = &pos.x;
    int id = self->mGlow.mSourceShape;
    GrMan->m_pCurrentDrawState->m_textureState.m_texid[0] = id;
    if (size.x > 72.0f) {
        float cap = 36.0f;  // width of each end cap: left cap, stretched middle (size.x - 2 caps), right cap
        float mid = (self->mGlow.mLeft + self->mGlow.mRight) * 0.5f;
        cOVStateHUD1P_renderGlowStrip_E7E0(self, &col, pp, cap, size.y, self->mGlow.mLeft, self->mGlow.mTop, mid, self->mGlow.mBottom);
        pos.x += cap;
        cOVStateHUD1P_renderGlowStrip_E7E0(self, &col, pp, size.x - 72.0f, size.y, mid, self->mGlow.mTop, mid, self->mGlow.mBottom);
        pos.x += size.x - 72.0f;
        cOVStateHUD1P_renderGlowStrip_E7E0(self, &col, pp, cap, size.y, mid, self->mGlow.mTop, self->mGlow.mRight, self->mGlow.mBottom);
    } else {
        cOVStateHUD1P_renderGlowStrip_E7E0(self, &col, pp, size.x, size.y, self->mGlow.mLeft, self->mGlow.mTop, self->mGlow.mRight, self->mGlow.mBottom);
    }
}

/**
 * Draws one letter of the HUD's UBERSUPER word, optionally on its "ubercharbkgd" background and with a glow.
 *
 * The letter is character @p idx of "UBERSUPER" (D_004A21C8) as a one-character string. Its element in @p elems is 30 +
 * 2 * @p idx, or with nonzero @p mode entry @p idx of D_0046ED40 (36, 34, 32, 30, 46, 44, 42, 40, 38: each word's slots
 * reversed, so the mirrored layout still reads left to right); the background is the next element. cOVStateHUD1P_calcElemPos places
 * the letter with @p scale, the element's alignment and @p mode as the reflect flags. Then @p scale becomes scale *
 * element size / 18 and sets the HUD font's (+0x42C) Scale for cFont_text; the Scale is restored to BaseScale
 * afterwards. The letter colour is (0.4, 1, 1, 1) (alpha 0.4), or the element's colour when @p kind is 2; the layer is
 * the element's layer byte plus @p layer.
 *
 * With @p kind nonzero, the background element's size times @p sc2 is placed and the "ubercharbkgd" texture icons[0]
 * (+0x494) is drawn there in the background element's colour, which is also stored as the font colour. Likely
 * oversight: the background is placed with the letter element's alignment bytes, not its own. With @p kind 2 and the
 * panel's f64 (+0x64; On Tour uberGlowFrame) >= 0, the glow (an inlined copy of cOVStateHUD1P_renderGlow) is drawn one layer lower
 * in the panel's col (+0x54; uberGlowColour) with phase f64, grown 3.4 times and alpha 0.26 to 0.5.
 *
 * @param elems  the HUD element table (0x24-byte tHUDDef records), passed as an int
 * @param rider  the HUD panel (sRiderPanel in ovstatehud; On Tour tHUDRiderInfo) whose glow fields are used
 * @param mode  reflect flags for cOVStateHUD1P_calcElemPos; nonzero also selects the mirrored element order
 * @param idx  which letter of UBERSUPER, 0-8
 * @param scale  the letter's x / y scale; overwritten in place with the font scale
 * @param sc2  x / y scale for the background
 * @param kind  0 the letter only, 1 also the background, 2 lit: element colours, background and glow
 * @param layer  added to the elements' layer bytes
 */
// PORT: elems is a pointer passed as int
extern "C" void cOVStateHUD1P_renderUberChar(cOVStateHUD1P* self, int elems, tHUDRiderInfo* rider, int mode, int idx, float* scale, V2_ED48* sc2, int kind, int layer) {
    V2_ED48 one;
    Tab_ED48 tab;
    Str_ED48 str;
    V2_ED48 elemPos;
    V2_ED48 size;
    V2_ED48 drawPos;
    Col_E7E0 col;
    signed char defIndex;
    one.x = 1.0f;
    one.y = 1.0f;
    tab = D_0046ED40;
    str = D_004A22E8[0];
    if (mode == 0) {
        defIndex = idx * 2 + 30;
    } else {
        defIndex = tab.e[idx].a;
    }
    col.r = 0.4000000059604645f;
    col.g = 1.0f;
    col.b = 1.0f;
    col.a = 1.0f;
    Col_E7E0* cp = &col;
    if (kind == 2) {
        tHUDDef_getColour((char*)(defIndex * 0x24 + elems), cp);
    }
    str.c[0] = D_004A21C8[idx];
    char* letterDef = (char*)(defIndex * 0x24 + elems);
    tHUDDef_getPos(letterDef, &elemPos);
    tHUDDef_getSize(letterDef, &size, 0);
    cOVStateHUD1P_calcElemPos(((char*)self), &drawPos, &elemPos, &size, scale, letterDef[0x20], letterDef[0x21], mode);
    scale[0] = scale[0] * 0.0555555559694767f * size.x;
    scale[1] = scale[1] * 0.0555555559694767f * size.y;
    int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | (((letterDef[0x22] + layer) << 5) & 0x3E0);
    *(Col_E7E0*)&self->mpHUDFont->Color[0] = col;
    cFont* hudFont = self->mpHUDFont;
    hudFont->Scale[0] = hudFont->BaseScale[0] * scale[0];
    hudFont->Scale[1] = hudFont->BaseScale[1] * scale[1];
    cFont_text((char*)self->mpHUDFont, drawPos.x, drawPos.y, str.c);
    cFont* hudFont2 = self->mpHUDFont;
    hudFont2->Scale[0] = hudFont2->BaseScale[0] * one.x;
    hudFont2->Scale[1] = hudFont2->BaseScale[1] * one.y;
    if (kind != 0) {
        char* bkgdDef = (char*)(elems + (defIndex + 1) * 0x24);
        tHUDDef_getPos(bkgdDef, &elemPos);
        tHUDDef_getSize(bkgdDef, &size, 0);
        size.x *= sc2->x;
        size.y *= sc2->y;
        cOVStateHUD1P_calcElemPos(((char*)self), &drawPos, &elemPos, &size, &one, letterDef[0x20], letterDef[0x21], mode);
        tHUDDef_getColour(bkgdDef, cp);
        *(Col_E7E0*)&self->mpHUDFont->Color[0] = col;
        w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
        *w = (*w & ~0x3E0) | (((bkgdDef[0x22] + layer) << 5) & 0x3E0);
        cOVStateHUD1P_renderBitmap(((char*)self), (int)self->mpGenTexElem[0], &drawPos, &size, &one, cp);
        if (kind == 2 && rider->uberGlowFrame >= 0.0f) {
            int under = layer - 1;
            w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
            *w = (*w & ~0x3E0) | (((bkgdDef[0x22] + under) << 5) & 0x3E0);
            draw_ED48(((char*)self), (Col_E7E0*)(&rider->uberGlowColour), &drawPos, &size, rider->uberGlowFrame, 3.4000000953674316f, 3.4000000953674316f, 0.2603548765182495f, 0.49998536705970764f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderTrickLevel);
#ifdef SKIP_ASM
extern tARGB D_004C8568;
extern tARGB D_004C8588;
extern tARGB D_004C85A8;
extern tARGB D_004C85C8;
extern tARGB D_004C85E8;
extern "C" void tHUDDef_getPos(void*, void*);
extern "C" void tHUDDef_getSize(void*, void*, int);
void tHUDDef_getScale(void*, void*);
extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);
extern "C" void cOVStateHUD1P_renderBitmap(void*, int, void*, void*, void*, void*);

/**
 * Draws a row of @p n "ptstar" icons (the trick level), coloured by the level and faded by @p alpha.
 *
 * Nothing is drawn for @p n <= 0. Element 16 of @p obj (+0x240) gives position, scale and the star size (fallback: the
 * "ptstar" texture icons[3], +0x4A0); @p scale, when not null, multiplies the scale. Without @p pos the row (n stars
 * with gaps of 2) is placed by cOVStateHUD1P_calcElemPos with the element's alignment and @p x; with @p pos the row is centred on
 * *pos and @p x is not used. The stars use the element's layer and colour cols[n - 1] of local copies of D_004C8568,
 * D_004C8588, D_004C85A8, D_004C85C8 and D_004C85E8, its alpha times @p alpha. There is no clamp: @p n above 5 would
 * read past the five colours.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 * @param n  the trick level: number of stars, 1-5
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos
 * @param pos  optional centre of the row; null uses the element's placement
 * @param scale  optional x / y scale multiplier
 * @param alpha  opacity multiplier
 */
void cOVStateHUD1P::renderTrickLevel(char* obj, int n, int x, t2Vector* pos, t2Vector* scale, float alpha)
{
    if (n > 0) {
        tARGB cols[5];
        tARGB elemPos;
        t2Vector starPos;
        t2Vector size;
        t2Vector sc;
        t2Vector dim;
        tARGB col;
        float step;
        int i;
        cols[0] = D_004C8568;
        cols[1] = D_004C8588;
        cols[2] = D_004C85A8;
        cols[3] = D_004C85C8;
        cols[4] = D_004C85E8;
        tHUDDef_getPos(obj + 0x240, &elemPos);
        tHUDDef_getSize(obj + 0x240, &size, (int)mpGenTexElem[3]);
        tHUDDef_getScale(obj + 0x240, &sc);
        if (scale) {
            sc.x *= scale->x;
            sc.y *= scale->y;
        }
        if (pos == 0) {
            dim.x = size.x * n + (n - 1) * 2.0f;
            dim.y = size.y;
            cOVStateHUD1P_calcElemPos(((char*)this), &starPos, &elemPos, &dim, &sc, obj[0x260], obj[0x261], x);
        } else {
            starPos = *pos;
            starPos.x -= (size.x * n + (n - 1) * 2.0f) * sc.x * 0.5f;
            starPos.y -= size.y * sc.y * 0.5f;
        }
        int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
        *w = (*w & ~0x3E0) | ((obj[0x262] << 5) & 0x3E0);
        step = (size.x + 2.0f) * sc.x;
        col = cols[n - 1];
        col.A *= alpha;
        for (i = 0; i < n; i++) {
            cOVStateHUD1P_renderBitmap(((char*)this), (int)mpGenTexElem[3], &starPos, &size, &sc, &col);
            starPos.x += step;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderFinishMessage);
#ifdef SKIP_ASM
struct Obj_F660 {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};
extern int D_00536640[];
extern char str_pct_02d_pct_02d_pct_02d_b[];
extern char D_004A2138[];
extern tARGB D_004C8808;
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void tHUDDef_getPos(void*, void*);
extern "C" void tHUDDef_getSize(void*, void*, int);
void tHUDDef_getScale(void*, void*);
void tHUDDef_getColour(void*, void*);
extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);
extern "C" void cOVStateHUD1P_renderBitmap(void*, int, void*, void*, void*, void*);
extern "C" int sprintf(char*, const char*, ...);
extern "C" void cFont_centertext(void*, float, float, char*);

/**
 * Draws the finish banner ("fini") with the rider's final time or score under it, or the "timeup" banner when @p flag
 * is set.
 *
 * The rider is AI->Riders[aiIndex] (SSXApp->Game->AI; aiIndex from @p rider). The banner is element 66 of @p elems with
 * the "fini" texture icons[8] (+0x4B4), or with @p flag element 27 with the "timeup" texture icons[2] (+0x49C): its
 * size (fallback: the texture's) times its scale, placed by cOVStateHUD1P_calcElemPos with the element's alignment and @p x, drawn
 * in the element's colour on its layer. Only under "fini": the value is D_00536640[i] (the cGameModeMan score / time
 * table), i being the rider's AI index from its cBeing interface (+0x6C0, virtual getAIIndex). In game modes 0 and 5
 * (cBENewRaceInterface_newRace.cGameMode) it is a time in 1/60 s ticks, printed as "%02d:%02d:%02d" hours, minutes and
 * seconds; otherwise it is printed "%d" after adding the panel's f7C (+0x7C; On Tour pointOffset). The text is drawn
 * centred (cFont_centertext) under the banner's middle with the HUD font (+0x42C) at 1.91 times the element's scale in
 * D_004C8808; the font Scale is then restored to BaseScale. The result of the back-end interface 0 lookup is discarded.
 *
 * @param elems  the HUD element table (0x24-byte tHUDDef records), passed as an int
 * @param rider  the HUD panel (sRiderPanel in ovstatehud; On Tour tHUDRiderInfo): aiIndex at +0, f7C at +0x7C
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos
 * @param flag  nonzero: out of time, draw "timeup" and no value
 */
void cOVStateHUD1P::renderFinishMessage(int elems, int* rider, int x, int flag)
{
    t2Vector scale;
    t2Vector drawPos;
    tARGB elemPos;
    t2Vector size;
    t2Vector sc;
    tARGB spriteColour;
    char buf[64];
    tARGB col;
    int idx = 66;
    cRider* panelRider = SSXApp->Game->AI->Riders[*rider];
    int sz = (int)mpGenTexElem[8];
    if (flag) {
        sz = (int)mpGenTexElem[2];
        idx = 27;
    }
    char* msgDef = (char*)(idx * 0x24 + elems); // PORT: pointer passed as int
    scale.x = 1.0f;
    scale.y = 1.0f;
    tHUDDef_getPos(msgDef, &elemPos);
    tHUDDef_getSize(msgDef, &size, sz);
    tHUDDef_getScale(msgDef, &sc);
    size.x *= sc.x;
    size.y *= sc.y;
    cOVStateHUD1P_calcElemPos(((char*)this), &drawPos, &elemPos, &size, &scale, msgDef[0x20], msgDef[0x21], x);
    tHUDDef_getColour(msgDef, &spriteColour);
    int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | ((msgDef[0x22] << 5) & 0x3E0);
    cOVStateHUD1P_renderBitmap(((char*)this), sz, &drawPos, &size, &scale, &spriteColour);
    if (flag == 0) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        signed char mode = cBENewRaceInterface_newRace.cGameMode;
        Obj_F660* being = (Obj_F660*)(&VPTR_AT(panelRider, sizeof(cRiderState)));
        int k = being->getAIIndex();
        int val = D_00536640[k];
        if (mode == 0 || mode == 5) {
            int t = (int)((float)val * 0.01666666753590107f);
            int rem = t % 3600;
            sprintf(buf, str_pct_02d_pct_02d_pct_02d_b, t / 3600, rem / 60, rem % 60);
        } else {
            sprintf(buf, D_004A2138, val + rider[0x7C / 4]);
        }
        cFont* t = mpHUDFont;
        drawPos.x += size.x * 0.5f;
        drawPos.y += size.y;
        sc.x *= 1.907578468322754f;
        sc.y *= 1.907578468322754f;
        t->Scale[0] = t->BaseScale[0] * sc.x;
        t->Scale[1] = t->BaseScale[1] * sc.y;
        col = D_004C8808;
        *(tARGB*)&mpHUDFont->Color[0] = col;
        cFont_centertext((char*)mpHUDFont, drawPos.x, drawPos.y, buf);
        t = (cFont*)((char*)mpHUDFont);
        t->Scale[0] = t->BaseScale[0] * scale.x;
        t->Scale[1] = t->BaseScale[1] * scale.y;
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021F9B0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V2_F9B0 { float x, y; };
struct V4_F9B0 { float x, y, z, w; };
struct Obj_F9B0 {
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};
extern float cSinCosTable_gSinCosTable[];
extern V4_F9B0 D_004C8648;
extern char str_pct_d_pct_d_b[];
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void tHUDDef_getPos(void*, void*);
extern "C" void tHUDDef_getSize(void*, void*, int);
void tHUDDef_getScale(void*, void*);
void tHUDDef_getColour(void*, void*);
extern "C" void cOVStateHUD1P_calcElemPos(void*, void*, void*, void*, void*, int, int, int);
extern "C" void cOVStateHUD1P_renderBitmap(void*, int, void*, void*, void*, void*);
extern "C" int sprintf(char*, const char*, ...);
extern "C" int cSectionMan_isActive(void*, int);
extern "C" int cBEMissionInterface_getCurrentCollectForSection(void*, int, int);
extern "C" int cBEMissionInterface_getTotalCollectForSection(void*, int);
extern "C" void cFont_text(void*, float, float, char*);

/**
 * Draws the collectibles counter: the "collectibles_icon" texture and "collected/total" for the current location,
 * pulsing while @p animFrame is below 0.44.
 *
 * Draws only while cSectionMan_isActive says the SectionMan's location (mBELocationID, +0x1BC) is loaded and active.
 * With @p animFrame < 0.44 the icon and the text scale by 1 + 0.2 * sin(4 pi k) * (1 - k^2), k being @p animFrame / 0.44 (sine table
 * cSinCosTable_gSinCosTable). Element 57 of @p obj (+0x804) gives position, size (fallback: the texture icons[5], +0x4A8) and scale;
 * cOVStateHUD1P_calcElemPos places the icon with the element's alignment and @p x, and it is drawn white at the element's size
 * (times the pulse) on the element's layer; the element's scale applies to the text only. The text goes right of the
 * icon, centred on it vertically, in the HUD font (+0x42C), coloured D_004C8648 while @p animFrame <= 0.44 and in the element's
 * colour after. It prints "%d/%d" with cBEMissionInterface_getCurrentCollectForSection (the rider's collected count for the location, from its profile)
 * and cBEMissionInterface_getTotalCollectForSection (the location's total) on back-end interface 10, the rider given by its AI index (cBeing interface
 * at +0x6C0, virtual getAIIndex). The font Scale is then restored to BaseScale.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 * @param rider  the rider (cRider; its cBeing interface at +0x6C0 gives the AI index)
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos
 * @param animFrame  the counter's animation time; below 0.44 it pulses
 */
extern "C" void func_0021F9B0(cOVStateHUD1P* self, char* obj, cRider* rider, int x, float animFrame) {
    V2_F9B0 scale;
    V2_F9B0 drawPos;
    V4_F9B0 elemPos;
    V2_F9B0 size;
    V2_F9B0 sc;
    V4_F9B0 col;
    char buf[64];
    char* sectionMan = (char*)SSXApp->Game->SectionMan;
    int id = *(int*)(sectionMan + 0x1BC);
    if (cSectionMan_isActive(sectionMan, id)) {
        float pulseScale = 1.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
        if (animFrame < 0.4399542808532715f) {
            float pulsePhase = animFrame * 2.272963523864746f;
            pulseScale = cSinCosTable_gSinCosTable[(int)(pulsePhase * 12.566370964050293f * 81.4873275756836f) & 0x1FF] * 0.19996199011802673f * (1.0f - pulsePhase * pulsePhase) + 1.0f;
        }
        tHUDDef_getPos(obj + 0x804, &elemPos);
        tHUDDef_getSize(obj + 0x804, &size, (int)self->mpGenTexElem[5]);
        tHUDDef_getScale(obj + 0x804, &sc);
        size.x *= pulseScale;
        size.y *= pulseScale;
        sc.x *= pulseScale;
        sc.y *= pulseScale;
        cOVStateHUD1P_calcElemPos(((char*)self), &drawPos, &elemPos, &size, &scale, obj[0x824], obj[0x825], x);
        int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
        *w = (*w & ~0x3E0) | ((obj[0x826] << 5) & 0x3E0);
        col.x = 1.0f;
        col.y = 1.0f;
        col.z = 1.0f;
        col.w = 1.0f;
        cOVStateHUD1P_renderBitmap(((char*)self), (int)self->mpGenTexElem[5], &drawPos, &size, &scale, &col);
        cFont* tx = self->mpHUDFont;
        drawPos.x += size.x;
        drawPos.y += (size.y - (float)tx->MaxCharHeight * tx->BaseScale[1] * sc.y) * 0.5f;
        if (animFrame > 0.4399542808532715f) {
            tHUDDef_getColour(obj + 0x804, &col);
        } else {
            col = D_004C8648;
        }
        cFont* tx2 = self->mpHUDFont;
        tx2->Scale[0] = tx2->BaseScale[0] * sc.x;
        tx2->Scale[1] = tx2->BaseScale[1] * sc.y;
        *(V4_F9B0*)&self->mpHUDFont->Color[0] = col;
        Obj_F9B0* being = (Obj_F9B0*)(&VPTR_AT(rider, sizeof(cRiderState)));
        sprintf(buf, str_pct_d_pct_d_b, cBEMissionInterface_getCurrentCollectForSection(iface, being->getAIIndex(), id), cBEMissionInterface_getTotalCollectForSection(iface, id));
        cFont_text((char*)self->mpHUDFont, drawPos.x, drawPos.y, buf);
        tx = (cFont*)((char*)self->mpHUDFont);
        tx->Scale[0] = tx->BaseScale[0] * scale.x;
        tx->Scale[1] = tx->BaseScale[1] * scale.y;
    }
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_0021FD38);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the inline `scaleP()` returning &D_004A5B38 (a return-value pseudo keeps the address in $20 across 3
// calls), and the PrintAt_FD38 helper (by-value position + reference offset, so q.x loads before half.x); plain form
// 94.30% (94.35% / 99.95% without each alone). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// scaleP(): returns &D_004A5B38 (the static scale vec); the target keeps that address in $20 across 3 calls, a pointer
// local
//   or a direct &D_004A5B38 does not; only an inline returning the address (a return-value pseudo) matches.
// PrintAt_FD38(font, float x, float y, const t2Vector& off, f, n): the target loads q.x before half.x but adds half.x +
// q.x;
//   by-value position args + a reference offset reproduce it.
// Scale2(out, const float&, const float&, float s): the by-value 0.5 param fixes the block-6 FPR order.
static inline void Scale2(t2Vector& out, const float& x, const float& y, float s) { out.x = x * s; out.y = y * s; }
extern int D_004A4E9C;
extern int D_004A4EA0;
extern int D_004A4EA4;
extern float D_004A5B30;
extern float D_004A5B34;
extern float D_004A5B38;
extern float D_004A5B3C;
extern float D_004A5B40;
extern float D_004A5B44;
extern sColour D_004C8828;
extern sColour D_004C8848;
extern sColour D_004C8868;
extern char D_004A2138[];
extern "C" int cBEMissionInterface_isMissionCompleted(void*, int);
extern "C" int cBEMissionInterface_getMissionIdx(void*, int);
extern "C" void cFont_centertextf(void* font, float x, float y, const char* fmt, ...);

static inline void PrintAt_FD38(void* font, float x, float y, const t2Vector& off, float f, int n)
{
    cFont_centertextf(font, off.x + x, (off.y + y) - f, D_004A2138, n);
}
static inline float* scaleP() { return &D_004A5B38; }
/**
 * Draws the mission indicator: the "bigc" texture with a "bigcCIRC" circle, coloured by whether the current mission is
 * completed, and the mission's number in it.
 *
 * The mission is the HUD state's f18C (+0x18C in ovstatehud's sHUD1P; On Tour has mLastMissionID in that place, after
 * the time fields). Element 69 of @p obj (+0x9B4) gives position, size (fallback: "bigc", icons[14], +0x4CC) and
 * colour; cOVStateHUD1P_calcElemPos places it with its alignment and no reflection at scale (1, 1) (the function-static D_004A5B38
 * / D_004A5B3C), on the element's layer. The circle "bigcCIRC" (icons[15], +0x4D0) is 34 by 34 (D_004A5B40 /
 * D_004A5B44) at offset (6, 6) (D_004A5B30 / D_004A5B34), one layer up, in D_004C8828 when
 * cBEMissionInterface_isMissionCompleted (back-end interface 10) says so, else in D_004C8848. The number is
 * cBEMissionInterface_getMissionIdx's index of the mission in cBELibrary_missionInfo plus 1, printed "%d" with cFont_centertextf, centred
 * on the element position + (23, 23) in D_004C8868 with the HUD font (+0x42C) at 0.776 times BaseScale, three layers
 * up; the font Scale is then restored. The three statics are set once under the guards D_004A4E9C, D_004A4EA0 and
 * D_004A4EA4. The local half first holds half the circle size, then the element's placed position.
 *
 * Likely bug: the circle's y is the element's x + 6 (half.x is used for both coordinates), so the circle lines up with
 * the number only when the element's x and y are equal.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 */
extern "C" void func_0021FD38(cOVStateHUD1P* self, char* obj) {
    t2Vector circleCentre;
    t2Vector half;
    sColour col;
    sFloat4 elemPos;
    t2Vector size;
    t2Vector circlePos;
    if (D_004A4E9C == 0) {
        D_004A4E9C = 1;
        D_004A5B3C = D_004A5B38 = 1.0f;
    }
    if (D_004A4EA0 == 0) {
        D_004A4EA0 = 1;
        D_004A5B34 = D_004A5B30 = 6.0f;
    }
    if (D_004A4EA4 == 0) {
        D_004A4EA4 = 1;
        D_004A5B44 = D_004A5B40 = 34.0f;
    }
    Scale2(half, D_004A5B40, D_004A5B44, 0.5f);
    circleCentre.x = D_004A5B30 + half.x;
    circleCentre.y = D_004A5B34 + half.y;
    int drawLayer = obj[0x9D6];
    tHUDDef_getPos(obj + 0x9B4, &elemPos);
    tHUDDef_getSize(obj + 0x9B4, &size, (int)self->mpGenTexElem[14]);
    cOVStateHUD1P_calcElemPos(((char*)self), &half, &elemPos, &size, scaleP(), obj[0x9D4], obj[0x9D5], 0);
    tHUDDef_getColour(obj + 0x9B4, &col);
    int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | ((drawLayer << 5) & 0x3E0);
    cOVStateHUD1P_renderBitmap(((char*)self), (int)self->mpGenTexElem[14], &half, &size, scaleP(), &col);
    void* be = cBE_getInterface_Fv(cBE_getBE(), 10);
    if (cBEMissionInterface_isMissionCompleted(be, (int)self->mLastMissionID) == 0) {
        col = D_004C8848;
    } else {
        col = D_004C8828;
    }
    circlePos.x = half.x + D_004A5B30;
    circlePos.y = half.x + D_004A5B34;
    w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | (((++drawLayer) << 5) & 0x3E0);
    cOVStateHUD1P_renderBitmap(((char*)self), (int)self->mpGenTexElem[15], &circlePos, &D_004A5B40, scaleP(), &col);
    float halfTextHeight = (float)self->mpHUDFont->MaxCharHeight * self->mpHUDFont->BaseScale[1] * 0.38805678486824036f;
    int n = cBEMissionInterface_getMissionIdx(be, (int)self->mLastMissionID);
    col = D_004C8868;
    *(sColour*)&self->mpHUDFont->Color[0] = col;
    char* hudFont = (char*)self->mpHUDFont;
    *(float*)(hudFont + 0x38) = *(float*)(hudFont + 0x30) * 0.7761135697364807f;
    *(float*)(hudFont + 0x3C) = *(float*)(hudFont + 0x34) * 0.7761135697364807f;
    w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | (((drawLayer += 2) << 5) & 0x3E0);
    PrintAt_FD38((char*)self->mpHUDFont, circleCentre.x, circleCentre.y, half, halfTextHeight, n + 1);
    cFont* hudFont2 = self->mpHUDFont;
    hudFont2->Scale[0] = hudFont2->BaseScale[0];
    hudFont2->Scale[1] = hudFont2->BaseScale[1];
}
#endif

extern "C" void cOVStateHUD1P_renderSpeed(cOVStateHUD1P* self, char* obj, cRider* rider, int x);

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderSpeed);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: both unit labels copied into locals `kmh` / `mph` up front (keeps "MPH" in $30 across the getVelocity
// call); plain form (`units = str_MPH` in the branch) 91.11%. Byte-identical and behaves like the plain form; landed
// marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct t4Vector;

struct t4Matrix;

struct tInstanceRecord;

extern "C" void tHUDDef_getPos(void*, void*);

void tHUDDef_getScale(void*, void*);

extern "C" void* cBE_getBE();

cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

void tHUDDef_getColour(void*, void*);

extern "C" int sprintf(char*, const char*, ...);

// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sSettings_00C0 {
    unsigned int pad0 : 19;
    unsigned int metric : 1;
    unsigned int pad1 : 12;
    int rest[0xA1];
};
extern sSettings_00C0 cBEOptionInterface_mOption_00C0 __asm__("cBEOptionInterface_mOption");
extern char str_KM_H[];
extern char str_MPH[];
extern char D_004A2138[];
extern "C" void cFont_textExtent(void* self, const char* str, float* rect, int n, float sx, float sy);
extern "C" void cFont_text(void*, float, float, char*);
extern "C" void cFont_centertext(void*, float, float, char*);
extern "C" unsigned int strlen(const char*);
void cOVStateHUD1P_calcElemPos_00C0(void*, t2Vector*, void*, t2Vector*, const t2Vector&, int, int, int) __asm__("cOVStateHUD1P_calcElemPos");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_00C0(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

static inline void ScaleV2_00C0(t2Vector& v, float s)
{
    v.x *= s;
    v.y *= s;
}

struct sFont_00C0 {
    char pad_0x0[0x14];
    int lineHeight;                 // 0x14
    char pad_0x18[0x18];
    float scaleX;                   // 0x30
    float scaleY;                   // 0x34
    float drawScaleX;               // 0x38
    float drawScaleY;               // 0x3C
    sColour colour;                 // 0x40
};

static inline sFont_00C0* font_00C0(char* self)
{
    return *(sFont_00C0**)(self + 0x42C);
}

static inline void SetScale_00C0(sFont_00C0* f, const t2Vector& s)
{
    f->drawScaleX = f->scaleX * s.x;
    f->drawScaleY = f->scaleY * s.y;
}

static inline void TextSize_00C0(sFont_00C0* f, const char* str, float* rect, t2Vector& size)
{
    cFont_textExtent(f, str, rect, 0, f->drawScaleX, f->drawScaleY);
    size.x = rect[2];
    size.y = rect[3];
}

/**
 * Draws the rider's speed: a "KM/H" or "MPH" label with the rounded speed above it.
 *
 * The speed is the length of the rider's velocity (cBeing interface at +0x6C0, virtual getVelocity; a VU0 length over
 * all four components) times 0.036, in km/h; when the metric bit (bit 19) of cBEOptionInterface_mOption is clear it is
 * multiplied by 0.621 and the label is "MPH". Element 25 of @p obj (+0x384) gives position, scale and colour: the HUD
 * font (+0x42C) takes the colour and BaseScale times the scale, the label's extent (cFont_textExtent) is the size that
 * cOVStateHUD1P_calcElemPos places with the element's alignment and @p x, and the label is drawn there. The number ("%d" of speed +
 * 0.5) is drawn centred (cFont_centertext) over the label's middle, raised by half its line height plus 14, at 1.12 times
 * the element's scale for three or more digits and 1.36 times otherwise; the font Scale is then restored to BaseScale.
 * The result of the back-end interface 4 lookup is discarded.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 * @param rider  the rider (cRider; its cBeing interface at +0x6C0 gives the velocity)
 * @param x  reflect flags for cOVStateHUD1P_calcElemPos
 */
extern "C" void cOVStateHUD1P_renderSpeed(cOVStateHUD1P* self, char* obj, cRider* rider, int x)
{
    sSettings_00C0 settings;
    t2Vector elemPos;
    t2Vector sc;
    sColour col;
    float rect[4];
    t2Vector drawPos;
    t2Vector size;
    cBE_getInterface_Fv(cBE_getBE(), 4);
    settings = cBEOptionInterface_mOption_00C0;
    // Both unit labels are taken up front: the target computes "MPH" before the velocity call and keeps it in
    // $30 across that call (a literal in the branch would be materialised inside it).
    char* kmh = str_KM_H;
    char* mph = str_MPH;
    char* units = kmh;
    float speed = vu0Length_00C0(*rider->getVelocity()) * 0.035999998450279236f;
    if (!settings.metric) {
        speed *= 0.6209999918937683f;
        units = mph;
    }
    char* speedDef = obj + 0x384;
    tHUDDef_getPos(speedDef, &elemPos);
    tHUDDef_getScale(speedDef, &sc);
    tHUDDef_getColour(speedDef, &col);
    font_00C0(((char*)self))->colour = col;
    SetScale_00C0(font_00C0(((char*)self)), sc);
    TextSize_00C0(font_00C0(((char*)self)), units, rect, size);
    cOVStateHUD1P_calcElemPos_00C0(((char*)self), &drawPos, &elemPos, &size, t2Vector(1.0f, 1.0f), obj[0x3A4], obj[0x3A5], x);
    cFont_text(font_00C0(((char*)self)), drawPos.x, drawPos.y, units);
    char buf[8];
    sprintf(buf, D_004A2138, (int)(speed + 0.5f));
    if (strlen(buf) > 2)
        ScaleV2_00C0(sc, 1.1214120388031006f);
    else
        ScaleV2_00C0(sc, 1.3623675107955933f);
    SetScale_00C0(font_00C0(((char*)self)), sc);
    drawPos.x += size.x * 0.5f;
    drawPos.y -= (float)font_00C0(((char*)self))->lineHeight * font_00C0(((char*)self))->scaleY * sc.y * 0.5f + 14.003287315368652f;
    cFont_centertext(font_00C0(((char*)self)), drawPos.x, drawPos.y, buf);
    SetScale_00C0(font_00C0(((char*)self)), t2Vector(1.0f, 1.0f));
}
#endif

INCLUDE_ASM("fe/all_in_one_fe", func_002204A0);

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateHUD1P_renderNISSkipMessage);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V2_0AD0 { float x, y; };
struct V4_0AD0 { float x, y, z, w; };
struct Big_0AD0 { unsigned int pad0 : 20; unsigned int mode : 2; unsigned int pad1 : 10; int rest[0xA1]; };
struct Bits_0AD0 { int a, b; unsigned int lo : 5; int v : 5; unsigned int hi : 22; };
extern Big_0AD0 cBEOptionInterface_mOption;
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void tHUDDef_getPos(void*, void*);
void tHUDDef_getScale(void*, void*);
void tHUDDef_getColour(void*, void*);
extern "C" void drawTextPictLine(void*, void*, void*, void*, int, int);

/**
 * Draws the skip prompt (the "Press ... to skip" text-and-picture line), fading in and out over @p progress.
 *
 * The line is the one at +0xB48 that cOVStateHUD1P_onCreateScreen builds from "%s @skip %s" (kT_OVRCMNPress,
 * kT_OVRCMNToSkip). Element 77 of @p obj (+0xAD4) gives position, scale and colour. While ScriptEngine->mIsPlaying is
 * set and the display format (bits 20-21 of cBEOptionInterface_mOption) is not 2 (widescreen), the y scale is cut to
 * 0.75 and the line moved up by 60. The colour's alpha is multiplied by 2 * @p progress up to 0.5 and by 2 * (1
 * - @p progress) above. The line's font (+0xB50) gets ShadowOffset (2, 2); then drawTextPictLine draws the line with the
 * element's alignment on the element's layer. The result of the back-end interface 4 lookup is discarded.
 *
 * @param obj  the HUD element table (0x24-byte tHUDDef records)
 * @param progress  animation progress, 0..1; the prompt is fully opaque at 0.5
 */
void cOVStateHUD1P::renderNISSkipMessage(char* obj, float progress)
{
    V2_0AD0 pos;
    V4_0AD0 scale;
    Big_0AD0 big;
    V4_0AD0 colour;
    V2_0AD0 spriteScale;
    V2_0AD0 drawPos;
    void* elem = obj + 0xAD4;
    tHUDDef_getPos(elem, &pos);
    tHUDDef_getScale(elem, &scale);
    cBE_getInterface_Fv(cBE_getBE(), 4);
    big = cBEOptionInterface_mOption;
    if (ScriptEngine->mIsPlaying != 0 && big.mode != 2) {
        scale.y *= 0.75f;
        pos.y -= 60.0f;
    }
    tHUDDef_getColour(obj + 0xAD4, &colour);
    if (progress <= 0.5f) {
        colour.x *= progress * 2.0f;
    } else {
        colour.x *= (1.0f - progress) * 2.0f;
    }
    spriteScale.x = 2.0f;
    spriteScale.y = 2.0f;
    *(V2_0AD0*)&mSkipMessage.pFont->ShadowOffset[0] = spriteScale;
    int* w = (int*)(&GrMan->m_pCurrentDrawState->m_bitGroups[2]);
    *w = (*w & ~0x3E0) | ((obj[0xAF6] << 5) & 0x3E0);
    drawPos = pos;
    drawTextPictLine(&mSkipMessage, &drawPos, &scale, &colour, obj[0xAF4], obj[0xAF5]);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220D38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_xi(void*, int) __asm__("cUIState_cUIState");
extern char cFEStateLodge_vtbl[];
/**
 * Constructs a cFEStateLodge without an engine: runs the cUIState constructor cUIState_cUIState with a null engine and
 * installs cFEStateLodge_vtbl.
 *
 * Nothing else is set. Out-of-line copy with no callers; it directly precedes cFEStateLodge__cFEStateLodge.
 *
 * @return the object
 */
extern "C" void* func_00220D38(cFEStateLodge* self) {
    cUIState_cUIState_xi(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateLodge_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFEStateLodge__cFEStateLodge);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cFEState_vtbl[];

/**
 * Destroys the lodge front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cFEStateLodge__cFEStateLodge(cFEStateLodge* self) {
    VPTR_OF(cListNode, self) = (void**)cFEState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220D98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_xi(void*, int) __asm__("cUIState_cUIState");
extern char cFEStateLodgeRiderDetail_vtbl[];
/**
 * Constructs a cFEStateLodgeRiderDetail without an engine: runs the cUIState constructor cUIState_cUIState with a null
 * engine and installs cFEStateLodgeRiderDetail_vtbl.
 *
 * Nothing else is set. Out-of-line copy with no callers; it directly precedes
 * cFEStateLodgeRiderDetail__cFEStateLodgeRiderDetail.
 *
 * @return the object
 */
extern "C" void* func_00220D98(cFEStateLodgeRiderDetail* self) {
    cUIState_cUIState_xi(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateLodgeRiderDetail_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFEStateLodgeRiderDetail__cFEStateLodgeRiderDetail);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cFEState_vtbl[];

/**
 * Destroys the lodge rider-detail front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cFEStateLodgeRiderDetail__cFEStateLodgeRiderDetail(cFEStateLodgeRiderDetail* self) {
    VPTR_OF(cListNode, self) = (void**)cFEState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220DF8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState_cUIState_xi(void*, int) __asm__("cUIState_cUIState");
extern char cFEStateBuyAttrib_vtbl[];
/**
 * Constructs a cFEStateBuyAttrib without an engine: runs the cUIState constructor cUIState_cUIState with a null engine and
 * installs cFEStateBuyAttrib_vtbl.
 *
 * Nothing else is set. Out-of-line copy with no callers; it directly precedes cFEStateBuyAttrib__cFEStateBuyAttrib.
 *
 * @return the object
 */
extern "C" void* func_00220DF8(cFEStateBuyAttrib* self) {
    cUIState_cUIState_xi(self, 0);
    VPTR_OF(cListNode, self) = (void**)cFEStateBuyAttrib_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFEStateBuyAttrib__cFEStateBuyAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cFEState_vtbl[];

/**
 * Destroys the buy-attribute front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cFEStateBuyAttrib__cFEStateBuyAttrib(cFEStateBuyAttrib* self) {
    VPTR_OF(cListNode, self) = (void**)cFEState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220E58);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState(void*, ...);
extern char cFEStateCareerStats_vtbl[];

/**
 * Constructs a cFEStateCareerStats without an engine: runs the cUIState constructor cUIState_cUIState with a null engine
 * and installs cFEStateCareerStats_vtbl.
 *
 * Nothing else is set. Out-of-line copy with no callers; it directly precedes cFEStateCareerStats__cFEStateCareerStats.
 *
 * @param state  the object to construct
 * @return @p state
 */
extern "C" void *func_00220E58(cFEStateCareerStats* state) {
    cUIState_cUIState(state, 0);
    VPTR_OF(cListNode, state) = (void**)cFEStateCareerStats_vtbl;
    return ((void*)state);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFEStateCareerStats__cFEStateCareerStats);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cFEState_vtbl[];

/**
 * Destroys the career-stats front-end state through the cFEState and cUIState destructors.
 *
 * Stores the base cFEState vtable (cFEState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same
 * body as cFEState's own destructor cFEState__cFEState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cFEStateCareerStats__cFEStateCareerStats(cFEStateCareerStats* self) {
    VPTR_OF(cListNode, self) = (void**)cFEState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_ENTERLODGE__cOVState_ENTERLODGE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the enter-lodge overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @param enterLodgeState  the enter-lodge overlay state being destroyed
 */
extern "C" void cOVState_ENTERLODGE__cOVState_ENTERLODGE(cOVState_ENTERLODGE* enterLodgeState) {
    VPTR_OF(cListNode, enterLodgeState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(enterLodgeState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_ENTERLODGE_cOVState_ENTERLODGE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_0EE0(void*) __asm__("cUIState_cUIState");
extern char cOVState_ENTERLODGE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_ENTERLODGE for engine @p engine and overlay id @p overlayId; installs cOVState_ENTERLODGE_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 31 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_ENTERLODGE_cOVState_ENTERLODGE(cOVState_ENTERLODGE* ovState, int engine, int overlayId) {
    cUIState_cUIState_0EE0(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_ENTERLODGE_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00220F48);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_BIGCHALLENGE_START without an engine; installs cOVState_BIGCHALLENGE_START_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Unlike cOVState_BIGCHALLENGE_START_cOVState_BIGCHALLENGE_START
 * it does not call cOVState_BIGCHALLENGE_START_getChallengeData. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00220F48(cOVState_BIGCHALLENGE_START* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_BIGCHALLENGE_START_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BIGCHALLENGE_START_cOVState_BIGCHALLENGE_START);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cOVState_BIGCHALLENGE_START_getChallengeData(void*);
extern void *cUIState_cUIState_0FA8(void*) __asm__("cUIState_cUIState");
extern char cOVState_BIGCHALLENGE_START_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_BIGCHALLENGE_START for engine @p engine and overlay id @p overlayId; installs
 * cOVState_BIGCHALLENGE_START_vtbl, then runs cOVState_BIGCHALLENGE_START_getChallengeData on it.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 29 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_BIGCHALLENGE_START_cOVState_BIGCHALLENGE_START(cOVState_BIGCHALLENGE_START* ovState, int engine, int overlayId) {
    cUIState_cUIState_0FA8(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_BIGCHALLENGE_START_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    cOVState_BIGCHALLENGE_START_getChallengeData(ovState);
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BIGCHALLENGE_START__cOVState_BIGCHALLENGE_START);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the big-challenge start overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 */
extern "C" void cOVState_BIGCHALLENGE_START__cOVState_BIGCHALLENGE_START(cOVState_BIGCHALLENGE_START* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_BIGCHALLENGE_FAIL_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the big-challenge overlay with vtable cOVState_BIGCHALLENGE_FAIL_vtbl (overlay id 30) without an engine.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Unlike cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL_2
 * it does not call cOVState_BIGCHALLENGE_FAIL_getChallengeData. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL(cOVState_BIGCHALLENGE_FAIL* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_BIGCHALLENGE_FAIL_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BIGCHALLENGE_FAIL_cOVState_BIGCHALLENGE_FAIL_2);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cOVState_BIGCHALLENGE_FAIL_getChallengeData(void*);
extern void *cUIState_cUIState_10A0(void*) __asm__("cUIState_cUIState");
extern char cOVState_BIGCHALLENGE_FAIL_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the big-challenge overlay with vtable cOVState_BIGCHALLENGE_FAIL_vtbl for engine @p engine and overlay id @p overlayId, then runs
 * cOVState_BIGCHALLENGE_FAIL_getChallengeData on it.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 30 inline with
 * the same steps; this out-of-line copy has no callers. cOVState_BIGCHALLENGE_FAIL_getChallengeData lives next to cOVState_BIGCHALLENGE_START_getChallengeData, which the
 * cOVState_BIGCHALLENGE_START constructor calls the same way.
 *
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
void * cOVState_BIGCHALLENGE_FAIL::cOVState_BIGCHALLENGE_FAIL_2(int engine, int overlayId)
{
    cUIState_cUIState_10A0(this);
    mOVID = overlayId;
    mDoneTransition = 0;
    VPTR_OF(cListNode, this) = (void**)cOVState_vtbl;
    mAllowPauseMenu = 0;
    mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, this) = (void**)cOVState_BIGCHALLENGE_FAIL_vtbl;
    (*(unsigned short *)mszChatBuff) = chatInit;
    cOVState_BIGCHALLENGE_FAIL_getChallengeData(this);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_BIGCHALLENGE_FAIL__cOVState_BIGCHALLENGE_FAIL);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the big-challenge overlay with vtable cOVState_BIGCHALLENGE_FAIL_vtbl (overlay id 30).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_BIGCHALLENGE_FAIL_vtbl.
 *
 * @param ovState  the object being destroyed
 */
extern "C" void cOVState_BIGCHALLENGE_FAIL__cOVState_BIGCHALLENGE_FAIL(cOVState_BIGCHALLENGE_FAIL* ovState) {
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TOPTIMES__cOVState_TOPTIMES);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cUIState__cUIState_a(void *) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];

/**
 * Destroys the top-times overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @param topTimesState  the top-times overlay state being destroyed
 */
extern "C" void cOVState_TOPTIMES__cOVState_TOPTIMES(cOVState_TOPTIMES* topTimesState) {
    VPTR_OF(cListNode, topTimesState) = (void**)cOVState_vtbl;
    cUIState__cUIState_a(topTimesState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TOPTIMES_cOVState_TOPTIMES);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1160(void*) __asm__("cUIState_cUIState");
extern char cOVState_TOPTIMES_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_TOPTIMES for engine @p engine and overlay id @p overlayId; installs cOVState_TOPTIMES_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 15 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_TOPTIMES_cOVState_TOPTIMES(cOVState_TOPTIMES* ovState, int engine, int overlayId) {
    cUIState_cUIState_1160(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_TOPTIMES_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TRANSPORT_cOVState_TRANSPORT);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_TRANSPORT_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_TRANSPORT_vtbl (overlay id 33) without an engine.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Unlike cOVState_TRANSPORT_cOVState_TRANSPORT_2
 * it does not run cUITemplate_MAP_init on the member at 0x9C. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *cOVState_TRANSPORT_cOVState_TRANSPORT(cOVState_TRANSPORT* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_TRANSPORT_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TRANSPORT_cOVState_TRANSPORT_2);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cUITemplate_MAP_init(void*);
extern void *cUIState_cUIState_1228(void*) __asm__("cUIState_cUIState");
extern char cOVState_TRANSPORT_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_TRANSPORT_vtbl for engine @p engine and overlay id @p overlayId, then runs cUITemplate_MAP_init on
 * its member at 0x9C.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 33 inline with
 * the same steps; this out-of-line copy has no callers. cUITemplate_MAP_init (declared without arguments) refreshes
 * cUITemplate_MAP_mPeak from the race location when cUITemplate_MAP_mMapStyle is 2 or more.
 *
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
void * cOVState_TRANSPORT::cOVState_TRANSPORT_2(int engine, int overlayId)
{
    cUIState_cUIState_1228(this);
    mOVID = overlayId;
    mDoneTransition = 0;
    VPTR_OF(cListNode, this) = (void**)cOVState_vtbl;
    mAllowPauseMenu = 0;
    mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, this) = (void**)cOVState_TRANSPORT_vtbl;
    (*(unsigned short *)mszChatBuff) = chatInit;
    cUITemplate_MAP_init((char*)&mMap);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_TRANSPORT__cOVState_TRANSPORT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable cOVState_TRANSPORT_vtbl (overlay id 33).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_TRANSPORT_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_TRANSPORT__cOVState_TRANSPORT(cOVState_TRANSPORT* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

/**
 * Does nothing: the empty onMenuGainFocus (slot +0x6C) of the overlay vtable cOVState_TRANSPORT_vtbl (overlay id 33).
 */
extern "C" void cOVState_TRANSPORT_onMenuGainFocus(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002212C8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_12C8(void*, int) __asm__("cUIState_cUIState");
extern char cOVState_MAP_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];
inline void* operator new[](unsigned int, void* p) { return p; }
struct Elem { Elem() {} };

/**
 * Constructs a cOVState_MAP without an engine; installs cOVState_MAP_vtbl and runs the (empty) constructor loop over
 * its eight two-word entries at 0x9C.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002212C8(cOVState_MAP* ovState) {
    cUIState_cUIState_12C8(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_MAP_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    new ((char*)&ovState->mbSessionUnlocked) Elem[8];
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_MAP_cOVState_MAP);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1350(void*, int) __asm__("cUIState_cUIState");
extern char cOVState_MAP_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_MAP for engine @p engine and overlay id @p overlayId; installs cOVState_MAP_vtbl and runs the (empty)
 * constructor loop over its eight two-word entries at 0x9C.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 32 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_MAP_cOVState_MAP(cOVState_MAP* ovState, int engine, int overlayId) {
    cUIState_cUIState_1350(ovState, engine);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_MAP_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    new ((char*)&ovState->mbSessionUnlocked) Elem[8];
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_MAP__cOVState_MAP);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the map overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVState_MAP__cOVState_MAP(cOVState_MAP* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

/**
 * Does nothing: cOVState_MAP's empty onMenuGainFocus (slot +0x6C of cOVState_MAP_vtbl, cUIState_onMenuGainFocus in the
 * base).
 */
extern "C" void cOVState_MAP_onMenuGainFocus(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221408);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char D_00473248[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_00473248 (overlay id 40) without an engine.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00221408(cOVState* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_00473248;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221468);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1468(void*) __asm__("cUIState_cUIState");
extern char D_00473248[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_00473248 for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 40 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *func_00221468(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState_1468(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_00473248;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002214D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable D_00473248 (overlay id 40).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_00473248.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* func_002214D0(cOVState* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002214F8);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVTemplate_PauseMenu_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVTemplate_PauseMenu without an engine; installs cOVTemplate_PauseMenu_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Unlike cOVTemplate_PauseMenu_cOVTemplate_PauseMenu
 * and the pause-menu subclasses' constructors, it leaves the word at 0xC4 alone. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002214F8(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVTemplate_PauseMenu_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_PauseMenu_cOVTemplate_PauseMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1558(void*) __asm__("cUIState_cUIState");
extern char cOVTemplate_PauseMenu_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVTemplate_PauseMenu for engine @p engine and overlay id @p overlayId; installs cOVTemplate_PauseMenu_vtbl and
 * clears the word at 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVTemplate_PauseMenu_cOVTemplate_PauseMenu(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1558(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVTemplate_PauseMenu_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVTemplate_PauseMenu__cOVTemplate_PauseMenu);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause-menu overlay template through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVTemplate_PauseMenu__cOVTemplate_PauseMenu(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002215F0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_COMPETE_vtbl (overlay id 1, a cOVTemplate_PauseMenu subclass) without an engine
 * and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002215F0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_COMPETE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_COMPETE_cOVState_PAUSE_COMPETE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1658(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_COMPETE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_COMPETE_vtbl for engine @p engine and overlay id @p overlayId and clears the word at
 * 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 1 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_COMPETE_cOVState_PAUSE_COMPETE(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1658(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_COMPETE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_COMPETE__cOVState_PAUSE_COMPETE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause menu with vtable cOVState_PAUSE_COMPETE_vtbl (overlay id 1, a cOVTemplate_PauseMenu subclass).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_PAUSE_COMPETE_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_PAUSE_COMPETE__cOVState_PAUSE_COMPETE(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002216F0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_BIGCHAL_vtbl (overlay id 2, a cOVTemplate_PauseMenu subclass) without an engine
 * and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002216F0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_BIGCHAL_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_BIGCHAL_cOVState_PAUSE_BIGCHAL);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1758(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_BIGCHAL_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_BIGCHAL_vtbl for engine @p engine and overlay id @p overlayId and clears the word at
 * 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 2 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_BIGCHAL_cOVState_PAUSE_BIGCHAL(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1758(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_BIGCHAL_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_BIGCHAL__cOVState_PAUSE_BIGCHAL);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause menu with vtable cOVState_PAUSE_BIGCHAL_vtbl (overlay id 2, a cOVTemplate_PauseMenu subclass).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_PAUSE_BIGCHAL_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_PAUSE_BIGCHAL__cOVState_PAUSE_BIGCHAL(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002217F0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_FREERIDE_vtbl (overlay id 3, a cOVTemplate_PauseMenu subclass) without an engine
 * and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002217F0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_FREERIDE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_FREERIDE_cOVState_PAUSE_FREERIDE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1858(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_FREERIDE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_FREERIDE_vtbl for engine @p engine and overlay id @p overlayId and clears the word at
 * 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 3 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_FREERIDE_cOVState_PAUSE_FREERIDE(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1858(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_FREERIDE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_FREERIDE__cOVState_PAUSE_FREERIDE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause menu with vtable cOVState_PAUSE_FREERIDE_vtbl (overlay id 3, a cOVTemplate_PauseMenu subclass).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_PAUSE_FREERIDE_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_PAUSE_FREERIDE__cOVState_PAUSE_FREERIDE(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002218F0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_SPOKE_vtbl (overlay id 4, a cOVTemplate_PauseMenu subclass) without an engine
 * and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002218F0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_SPOKE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_SPOKE_cOVState_PAUSE_SPOKE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1958(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_SPOKE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_SPOKE_vtbl for engine @p engine and overlay id @p overlayId and clears the word at
 * 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 4 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_SPOKE_cOVState_PAUSE_SPOKE(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1958(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_SPOKE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_SPOKE__cOVState_PAUSE_SPOKE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause menu with vtable cOVState_PAUSE_SPOKE_vtbl (overlay id 4, a cOVTemplate_PauseMenu subclass).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_PAUSE_SPOKE_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_PAUSE_SPOKE__cOVState_PAUSE_SPOKE(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002219F0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_QUICKPLAY_vtbl (overlay id 5, a cOVTemplate_PauseMenu subclass) without an engine
 * and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002219F0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_QUICKPLAY_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_QUICKPLAY_cOVState_PAUSE_QUICKPLAY);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1A58(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_QUICKPLAY_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the pause menu with vtable cOVState_PAUSE_QUICKPLAY_vtbl for engine @p engine and overlay id @p overlayId and clears the word at
 * 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 5 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_QUICKPLAY_cOVState_PAUSE_QUICKPLAY(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1A58(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_QUICKPLAY_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_QUICKPLAY__cOVState_PAUSE_QUICKPLAY);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause menu with vtable cOVState_PAUSE_QUICKPLAY_vtbl (overlay id 5, a cOVTemplate_PauseMenu subclass).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_PAUSE_QUICKPLAY_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_PAUSE_QUICKPLAY__cOVState_PAUSE_QUICKPLAY(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221AF0);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_PAUSE_ONLINE without an engine; installs cOVState_PAUSE_ONLINE_vtbl and clears the word at 0xC4.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. cOVState_PAUSE_ONLINE_vtbl's
 * destructor slot is cOVState_PAUSE_ONLINE__cOVState_PAUSE_ONLINE. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00221AF0(cOVTemplate_PauseMenu* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_ONLINE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_ONLINE_cOVState_PAUSE_ONLINE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1B58(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_ONLINE_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_PAUSE_ONLINE for engine @p engine and overlay id @p overlayId; installs cOVState_PAUSE_ONLINE_vtbl and clears the word
 * at 0xC4.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 6 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_ONLINE_cOVState_PAUSE_ONLINE(cOVTemplate_PauseMenu* ovState, int engine, int overlayId) {
    cUIState_cUIState_1B58(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_ONLINE_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    ovState->mpThread = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_ONLINE__cOVState_PAUSE_ONLINE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the online pause overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl) at +0x8, then calls the cUIState destructor cUIState__cUIState: the same body as
 * cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own (the store of its own vtable
 * cOVState_PAUSE_ONLINE_vtbl is dropped). The g++ delete flags reach cUIState__cUIState untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState returns nothing)
 */
extern "C" void* cOVState_PAUSE_ONLINE__cOVState_PAUSE_ONLINE(cOVTemplate_PauseMenu* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_OPTIONS__cOVState_PAUSE_OPTIONS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause-options overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVState_PAUSE_OPTIONS__cOVState_PAUSE_OPTIONS(cOVState_PAUSE_OPTIONS* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_OPTIONS_cOVState_PAUSE_OPTIONS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1C18(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_OPTIONS_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_PAUSE_OPTIONS for engine @p engine and overlay id @p overlayId; installs cOVState_PAUSE_OPTIONS_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 7 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_OPTIONS_cOVState_PAUSE_OPTIONS(cOVState_PAUSE_OPTIONS* ovState, int engine, int overlayId) {
    cUIState_cUIState_1C18(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_OPTIONS_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221C80);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_PAUSE_ONLINE_ERROR without an engine; installs cOVState_PAUSE_ONLINE_ERROR_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Unlike cOVState_PAUSE_ONLINE_ERROR_cOVState_PAUSE_ONLINE_ERROR
 * it leaves the words at 0x9C and 0xA0 alone. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00221C80(cOVState_PAUSE_ONLINE_ERROR* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_ONLINE_ERROR_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_ONLINE_ERROR_cOVState_PAUSE_ONLINE_ERROR);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1CE0(void*) __asm__("cUIState_cUIState");
extern char cOVState_PAUSE_ONLINE_ERROR_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_PAUSE_ONLINE_ERROR for engine @p engine and overlay id @p overlayId; installs
 * cOVState_PAUSE_ONLINE_ERROR_vtbl, sets the word at 0xA0 to 1 and clears the one at 0x9C.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 39 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_PAUSE_ONLINE_ERROR_cOVState_PAUSE_ONLINE_ERROR(cOVState_PAUSE_ONLINE_ERROR* ovState, int engine, int overlayId) {
    cUIState_cUIState_1CE0(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_PAUSE_ONLINE_ERROR_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    ovState->mbShouldDisplayAuxInfo = 1;
    ((int)ovState->miStartPingTick) = 0;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PAUSE_ONLINE_ERROR__cOVState_PAUSE_ONLINE_ERROR);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the pause online-error overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVState_PAUSE_ONLINE_ERROR__cOVState_PAUSE_ONLINE_ERROR(cOVState_PAUSE_ONLINE_ERROR* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

/**
 * Does nothing: cOVState_PAUSE_ONLINE_ERROR's empty onMenuGainFocus (slot +0x6C of cOVState_PAUSE_ONLINE_ERROR_vtbl).
 */
extern "C" void cOVState_PAUSE_ONLINE_ERROR_onMenuGainFocus(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_PROFILE__cOVState_PROFILE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_x(void*) __asm__("cFEMemCard__cFEMemCard");
extern char cOVState_PROFILE_vtbl[];
/**
 * Destroys the profile overlay state through the memory-card base destructor cFEMemCard__cFEMemCard().
 *
 * Stores cOVState_PROFILE_vtbl, then calls cFEMemCard__cFEMemCard(), which clears the memory-card callbacks' state
 * pointer cFEMemCard_mInstance and runs the cUIState destructor. Only @p self is declared: the destructor flags reach
 * cFEMemCard__cFEMemCard() unchanged in the second argument register.
 *
 * @return no meaningful value (cFEMemCard__cFEMemCard() returns nothing)
 */
extern "C" void* cOVState_PROFILE__cOVState_PROFILE(cOVState_PROFILE* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_PROFILE_vtbl;
    return cFEMemCard__cFEMemCard_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221DB0);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern char cOVState_PROFILE_vtbl[];

/**
 * Constructs a cOVState_PROFILE without an engine: runs cFEMemCard_cFEMemCard2(@p state, 0, 0) and installs
 * cOVState_PROFILE_vtbl.
 *
 * Out-of-line copy with no callers; the real constructor is cOVState_PROFILE_cOVState_PROFILE.
 *
 * @param state  the object to construct
 * @return @p state
 */
extern "C" void *func_00221DB0(cOVState_PROFILE* state) {
    cFEMemCard_cFEMemCard2(state, 0, 0);
    VPTR_OF(cListNode, state) = (void**)cOVState_PROFILE_vtbl;
    return ((void*)state);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_HEAT_PREEVENT__cOVState_HEAT_PREEVENT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable cOVState_HEAT_PREEVENT_vtbl (overlay id 8).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of cOVState_HEAT_PREEVENT_vtbl.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* cOVState_HEAT_PREEVENT__cOVState_HEAT_PREEVENT(cOVState_HEAT_PREEVENT* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_HEAT_PREEVENT_cOVState_HEAT_PREEVENT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1E18(void*) __asm__("cUIState_cUIState");
extern char cOVState_HEAT_PREEVENT_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable cOVState_HEAT_PREEVENT_vtbl for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 8 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_HEAT_PREEVENT_cOVState_HEAT_PREEVENT(cOVState_HEAT_PREEVENT* ovState, int engine, int overlayId) {
    cUIState_cUIState_1E18(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_HEAT_PREEVENT_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221E80);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable D_00472778 (overlay id 9).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_00472778.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* func_00221E80(cOVState* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221EA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1EA8(void*) __asm__("cUIState_cUIState");
extern char D_00472778[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_00472778 for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 9 inline with the
 * same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *func_00221EA8(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState_1EA8(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_00472778;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221F10);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable D_004726A0 (overlay id 21).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_004726A0.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* func_00221F10(cOVState* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221F38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1F38(void*) __asm__("cUIState_cUIState");
extern char D_004726A0[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_004726A0 for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 21 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *func_00221F38(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState_1F38(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_004726A0;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221FA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the overlay with vtable D_004725C8 (overlay id 20).
 *
 * Stores the cOVState vtable cOVState_vtbl and runs the cUIState destructor cUIState__cUIState, which frees the object when the
 * g++ delete flags (left untouched in the second argument register) have bit 0 set. The class tears down nothing of its
 * own. It is the destructor slot (+0xC) of D_004725C8.
 *
 * @return whatever cUIState__cUIState returns
 */
extern "C" void* func_00221FA0(cOVState* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00221FC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_1FC8(void*) __asm__("cUIState_cUIState");
extern char D_004725C8[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs the overlay with vtable D_004725C8 for engine @p engine and overlay id @p overlayId.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 20 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *func_00221FC8(cOVState* ovState, int engine, int overlayId) {
    cUIState_cUIState_1FC8(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)D_004725C8;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_SAVEREPLAY__cOVState_SAVEREPLAY);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_x(void*) __asm__("cFEMemCard__cFEMemCard");
extern char cOVState_SAVEREPLAY_vtbl[];
/**
 * Destroys the memory-card save overlay with vtable cOVState_SAVEREPLAY_vtbl (overlay id 18).
 *
 * Stores cOVState_SAVEREPLAY_vtbl and runs the cFEMemCard destructor cFEMemCard__cFEMemCard with the delete flags still in the second argument
 * register. It is the destructor slot (+0xC) of cOVState_SAVEREPLAY_vtbl. Its onCreateScreen opens the "119Save_Replay" screen;
 * cOVState_SAVEREPLAY_cOVState_SAVEREPLAY is the constructor cOVStateManager_triggerOverlay calls for id 18.
 *
 * @return what the base destructor returns
 */
extern "C" void* cOVState_SAVEREPLAY__cOVState_SAVEREPLAY(cOVState_SAVEREPLAY* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_SAVEREPLAY_vtbl;
    return cFEMemCard__cFEMemCard_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222058);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern char cOVState_SAVEREPLAY_vtbl[];

/**
 * Constructs the memory-card save overlay with vtable cOVState_SAVEREPLAY_vtbl (overlay id 18) without an engine: runs
 * cFEMemCard_cFEMemCard2(@p ovState, 0, 0) and installs cOVState_SAVEREPLAY_vtbl.
 *
 * Out-of-line copy with no callers; cOVStateManager_triggerOverlay builds id 18 through cOVState_SAVEREPLAY_cOVState_SAVEREPLAY instead.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00222058(cOVState_SAVEREPLAY* ovState) {
    cFEMemCard_cFEMemCard2(ovState, 0, 0);
    VPTR_OF(cListNode, ovState) = (void**)cOVState_SAVEREPLAY_vtbl;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_SAVEOPTIONS__cOVState_SAVEOPTIONS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_x(void*) __asm__("cFEMemCard__cFEMemCard");
extern char cOVState_SAVEOPTIONS_vtbl[];
/**
 * Destroys the memory-card save overlay with vtable cOVState_SAVEOPTIONS_vtbl (overlay id 19).
 *
 * Stores cOVState_SAVEOPTIONS_vtbl and runs the cFEMemCard destructor cFEMemCard__cFEMemCard with the delete flags still in the second argument
 * register. It is the destructor slot (+0xC) of cOVState_SAVEOPTIONS_vtbl. Its onCreateScreen opens the "119Save_Replay" screen;
 * cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS is the constructor cOVStateManager_triggerOverlay calls for id 19.
 *
 * @return what the base destructor returns
 */
extern "C" void* cOVState_SAVEOPTIONS__cOVState_SAVEOPTIONS(cOVState_SAVEOPTIONS* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_SAVEOPTIONS_vtbl;
    return cFEMemCard__cFEMemCard_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002220C0);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern char cOVState_SAVEOPTIONS_vtbl[];

/**
 * Constructs the memory-card save overlay with vtable cOVState_SAVEOPTIONS_vtbl (overlay id 19) without an engine: runs
 * cFEMemCard_cFEMemCard2(@p ovState, 0, 0) and installs cOVState_SAVEOPTIONS_vtbl.
 *
 * Out-of-line copy with no callers; cOVStateManager_triggerOverlay builds id 19 through cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS instead.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002220C0(cOVState_SAVEOPTIONS* ovState) {
    cFEMemCard_cFEMemCard2(ovState, 0, 0);
    VPTR_OF(cListNode, ovState) = (void**)cOVState_SAVEOPTIONS_vtbl;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222100);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVStateRewardsList_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateRewardsList without an engine; installs cOVStateRewardsList_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_00222100(cOVStateRewardsList* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateRewardsList_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateRewardsList_cOVStateRewardsList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_2160(void*) __asm__("cUIState_cUIState");
extern char cOVStateRewardsList_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateRewardsList for engine @p engine and overlay id @p overlayId; installs cOVStateRewardsList_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVStateRewardsList_cOVStateRewardsList(cOVStateRewardsList* ovState, int engine, int overlayId) {
    cUIState_cUIState_2160(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateRewardsList_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateRewardsList__cOVStateRewardsList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the rewards-list overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVStateRewardsList__cOVStateRewardsList(cOVStateRewardsList* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_REWARDS__cOVState_REWARDS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the rewards overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVState_REWARDS__cOVState_REWARDS(cOVState_REWARDS* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_REWARDS_cOVState_REWARDS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_2218(void*) __asm__("cUIState_cUIState");
extern char cOVState_REWARDS_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVState_REWARDS for engine @p engine and overlay id @p overlayId; installs cOVState_REWARDS_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. cOVStateManager_triggerOverlay builds overlay id 16 inline with
 * the same steps; this out-of-line copy has no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVState_REWARDS_cOVState_REWARDS(cOVState_REWARDS* ovState, int engine, int overlayId) {
    cUIState_cUIState_2218(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVState_REWARDS_vtbl;
    (*(unsigned short *)ovState->mszChatBuff) = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVState_AUTOSAVE__cOVState_AUTOSAVE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cFEMemCard__cFEMemCard_x(void*) __asm__("cFEMemCard__cFEMemCard");
extern char cOVState_AUTOSAVE_vtbl[];
/**
 * Destroys the autosave overlay state through the memory-card base destructor cFEMemCard__cFEMemCard().
 *
 * Stores cOVState_AUTOSAVE_vtbl, then calls cFEMemCard__cFEMemCard(), which clears the memory-card callbacks' state
 * pointer cFEMemCard_mInstance and runs the cUIState destructor. Only @p self is declared: the destructor flags reach
 * cFEMemCard__cFEMemCard() unchanged in the second argument register.
 *
 * @return no meaningful value (cFEMemCard__cFEMemCard() returns nothing)
 */
extern "C" void* cOVState_AUTOSAVE__cOVState_AUTOSAVE(cOVState_AUTOSAVE* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_AUTOSAVE_vtbl;
    return cFEMemCard__cFEMemCard_x(self);
}
#endif

/**
 * Does nothing: cOVState_AUTOSAVE's empty onWidgetEvent (slot +0x9C of cOVState_AUTOSAVE_vtbl), replacing
 * cFEMemCard_onWidgetEvent.
 */
extern "C" void cOVState_AUTOSAVE_onWidgetEvent(void) {
}

/**
 * Does nothing: cOVState_AUTOSAVE's empty override of cFEMemCard_hideActionButtons (slot +0x12C of
 * cOVState_AUTOSAVE_vtbl), so the autosave screen never hides or shows the action buttons.
 */
extern "C" void cOVState_AUTOSAVE_hideActionButtons(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002222B8);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern char cOVState_AUTOSAVE_vtbl[];

/**
 * Constructs a cOVState_AUTOSAVE without an engine: runs cFEMemCard_cFEMemCard2(@p state, 0, 0) and installs
 * cOVState_AUTOSAVE_vtbl.
 *
 * Out-of-line copy with no callers; the real constructor is cOVState_AUTOSAVE_cOVState_AUTOSAVE.
 *
 * @param state  the object to construct
 * @return @p state
 */
extern "C" void *func_002222B8(cOVState_AUTOSAVE* state) {
    cFEMemCard_cFEMemCard2(state, 0, 0);
    VPTR_OF(cListNode, state) = (void**)cOVState_AUTOSAVE_vtbl;
    return ((void*)state);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002222F8);
#ifdef SKIP_ASM
extern "C" void *cUIState_cUIState(void*, ...);
extern char cOVStateTrophy_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateTrophy without an engine; installs cOVStateTrophy_vtbl.
 *
 * Runs cUIState_cUIState with a null engine, then the inlined cOVState part with overlay id 0: mOVID (0x98),
 * mDoneTransition (0x48), mAllowPauseMenu (0x4C) and mRemovePDA (0x54) are cleared, mszChatBuff (0x58) gets " " (one
 * halfword from str_sp_b) and the cOVState vtable cOVState_vtbl is stored before the class's own. Out-of-line copy with
 * no callers.
 *
 * @param ovState  the object to construct
 * @return @p ovState
 */
extern "C" void *func_002222F8(cOVStateTrophy* ovState) {
    cUIState_cUIState(ovState, 0);
    ovState->mOVID = 0;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateTrophy_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateTrophy_cOVStateTrophy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void *cUIState_cUIState_2358(void*) __asm__("cUIState_cUIState");
extern char cOVStateTrophy_vtbl[];
extern char cOVState_vtbl[];
extern unsigned short str_sp_b[];

/**
 * Constructs a cOVStateTrophy for engine @p engine and overlay id @p overlayId; installs cOVStateTrophy_vtbl.
 *
 * Runs the cUIState constructor cUIState_cUIState with the engine still in the second argument register (the C never names
 * it). The inlined cOVState part sets mOVID (0x98) to the overlay id, clears mDoneTransition (0x48), mAllowPauseMenu
 * (0x4C) and mRemovePDA (0x54), copies " " into mszChatBuff (0x58) as one halfword read from str_sp_b, and stores the
 * cOVState vtable cOVState_vtbl before the class's own. Out-of-line copy with no callers.
 *
 * @param ovState  the object to construct
 * @param engine  the UI engine, passed through to cUIState_cUIState
 * @param overlayId  the overlay id (tOVStateEnum), stored in mOVID
 * @return @p ovState
 */
extern "C" void *cOVStateTrophy_cOVStateTrophy(cOVStateTrophy* ovState, int engine, int overlayId) {
    cUIState_cUIState_2358(ovState);
    ovState->mOVID = overlayId;
    ovState->mDoneTransition = 0;
    VPTR_OF(cListNode, ovState) = (void**)cOVState_vtbl;
    ovState->mAllowPauseMenu = 0;
    ovState->mRemovePDA = 0;
    unsigned short chatInit = str_sp_b[0];
    VPTR_OF(cListNode, ovState) = (void**)cOVStateTrophy_vtbl;
    *(unsigned short*)ovState->mszChatBuff = chatInit;
    return ((void*)ovState);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cOVStateTrophy__cOVStateTrophy);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cUIState__cUIState_x(void*) __asm__("cUIState__cUIState");
extern char cOVState_vtbl[];
/**
 * Destroys the trophy overlay state through the cOVState and cUIState destructors.
 *
 * Stores the cOVState vtable (cOVState_vtbl), then calls the cUIState destructor cUIState__cUIState(): the same body
 * as cOVState's own destructor cOVState__cOVState, so the class tears down nothing of its own. The g++ delete
 * flags reach cUIState__cUIState() untouched in the second argument register.
 *
 * @return no meaningful value (cUIState__cUIState() returns nothing)
 */
extern "C" void* cOVStateTrophy__cOVStateTrophy(cOVStateTrophy* self) {
    VPTR_OF(cListNode, self) = (void**)cOVState_vtbl;
    return cUIState__cUIState_x(self);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", cFont_centertextf_2);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
#define cFont_centertextf_2_va_start(ap) \
    (ap = (char*)__builtin_next_arg() \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))
extern "C" void cFont_vcentertextf(int, int, char*);

/**
 * Draws printf-style text centred at a position: forwards the font, format and variable arguments to
 * cFont_vcentertextf.
 *
 * The position floats arrive in the float argument registers and are passed on untouched; the va_list is built by hand
 * (gcc 2.95's EE va_start). No callers.
 *
 * @param x  horizontal position (passed through in a float register)
 * @param y  vertical position (passed through in a float register)
 * @param font  the font to draw with
 * @param fmt  the printf format
 */
extern "C" void cFont_centertextf_2(float x, float y, int font, int fmt, ...) {
    char* ap;
    cFont_centertextf_2_va_start(ap);
    cFont_vcentertextf(font, fmt, ap);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/all_in_one_fe", func_00222428);
#ifdef SKIP_ASM
extern "C" void __sti__all_in_one_fe_cpp(int, int);

/**
 * Runs this unit's static constructors: calls __sti__all_in_one_fe_cpp(1, 0xFFFF).
 *
 * g++'s global-constructor entry for the unit; its address is in the constructor table __CTOR_LIST__ (+0x24).
 */
extern "C" void func_00222428(void) {
    __sti__all_in_one_fe_cpp(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_00222448);
#ifdef SKIP_ASM
extern unsigned short D_00441D38[];

/**
 * Computes a CRC-16 over @p n bytes at @p bytes with the 256-entry table D_00441D38.
 *
 * D_00441D38 is the reflected CRC-16/IBM table (0x0000, 0xC0C1, 0xC181, 0x0140, ...; polynomial 0xA001). Byte-wise: crc
 * = D_00441D38[(byte ^ crc) & 0xFF] ^ (crc >> 8), starting from 0xFBEA (64490), with no final XOR. No callers.
 *
 * @param bytes  the bytes to checksum
 * @param n  how many bytes
 * @return the CRC
 */
extern "C" unsigned int func_00222448(const unsigned char *bytes, int n) {
    const unsigned char *end = bytes + n;
    unsigned int crc = 64490;
    while (bytes < end) {
        crc = D_00441D38[(*bytes++ ^ crc) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", func_002224A0);
#ifdef SKIP_ASM
/**
 * Returns 0 and does nothing else.
 *
 * luno_cLunoVM_execute calls it, ignoring the result, when a call opcode (27) finds a nil function register: an empty
 * error hook.
 *
 * @return 0
 */
extern "C" int func_002224A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", luno_cLunoVM_cLunoVM);
#ifdef SKIP_ASM
/**
 * Constructs a Luno VM handle: does nothing and returns @p value.
 *
 * lunovm.cpp binds it as cLunoVM's constructor; cWScriptMan_runScript and luno_cLunoVM_execute construct a VM on the stack with
 * it before calling luno_cLunoVM_run / luno_cLunoVM_run1.
 *
 * @param value  the object to construct
 * @return @p value
 */
extern "C" int luno_cLunoVM_cLunoVM(int value) {
    return value;
}
#endif

/**
 * Does nothing (empty body; no callers or references).
 */
extern "C" void func_002224B0(void) {
}

//100%
INCLUDE_ASM("fe/all_in_one_fe", luno_cLunoVM_run);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
struct LV_24B8;
extern "C" void luno_cLunoTable__cLunoTable_24B8(LV_24B8*, int) __asm__("luno_cLunoTable__cLunoTable");
extern "C" void luno_cLunoTable_operatorE_24B8(LV_24B8*, const LV_24B8*) __asm__("luno_cLunoTable_operatorE");
extern "C" void luno_cLunoVM_execute_24B8(LV_24B8**, char**, int) __asm__("luno_cLunoVM_execute");
extern char str_LunoVMRegister[];

struct LVPair_24B8 { int a, b; };
struct LV_24B8 {
    union { int i; LVPair_24B8 pair; } u;
    int type;
    LV_24B8() { type = eLunoVar_Nil; u.i = 0; }
    ~LV_24B8() {}
    void Release() { if (type == eLunoVar_Table) luno_cLunoTable__cLunoTable_24B8(this, 2); }
    void Clear() { type = eLunoVar_Nil; u.i = 0; }
    void SetTable(const LV_24B8* src) {
        Release();
        Clear();
        luno_cLunoTable_operatorE_24B8(this, src);
        type = eLunoVar_Table;
    }
    void Assign(const LV_24B8* src) {
        Release();
        Clear();
        if (src->type == eLunoVar_Table) {
            luno_cLunoTable_operatorE_24B8(this, src);
        } else if (src->type == eLunoVar_Func) {
            u.pair = src->u.pair;
        } else {
            u.i = src->u.i;
        }
        type = src->type;
    }
};

extern LV_24B8 luno_gLunoNil;

/**
 * Runs a Luno function with a global table and no argument: allocates the register file, puts the table in register 0
 * and nil in register 1, then executes.
 *
 * Reads the register count from the function's chunk (at chunk + chunk[1] + 8), allocates that many 8-byte values with
 * operator new[] ("LunoVMRegister"), stores the array in the VM's first word, makes register 0 a table reference
 * to @p table (luno_cLunoTable_operatorE), copies luno_gLunoNil into register 1 (releasing a table it held with
 * luno_cLunoTable__cLunoTable) and calls luno_cLunoVM_execute(@p out, @p state, @p retVar). cWScriptMan_runScript (world scripts)
 * uses it.
 *
 * @param out  the VM; its first word receives the register array
 * @param state  the function to run (its first word points at the compiled chunk)
 * @param table  the global table, placed in register 0
 * @param retVar  receives the function's return value
 */
extern "C" void luno_cLunoVM_run(LV_24B8** out, char** state, LV_24B8* table, int retVar) {
    char* chunk = *state;
    int n = *(int*)(chunk + *(int*)(chunk + 4) + 8);
    LV_24B8* arr = new (str_LunoVMRegister, 0x20000000, 0) LV_24B8[n];
    *out = arr;
    arr[0].SetTable(table);
    LV_24B8* nilVal = &luno_gLunoNil;
    LV_24B8* regs = *out;
    LV_24B8* secondReg = &regs[1];
    if (regs[1].type == eLunoVar_Table) luno_cLunoTable__cLunoTable_24B8(secondReg, 2);
    secondReg->type = eLunoVar_Nil;
    regs[1].u.i = 0;
    if (nilVal->type == eLunoVar_Table) {
        luno_cLunoTable_operatorE_24B8(secondReg, nilVal);
    } else if (nilVal->type == eLunoVar_Func) {
        regs[1].u.pair = nilVal->u.pair;
    } else {
        regs[1].u.i = nilVal->u.i;
    }
    secondReg->type = nilVal->type;
    luno_cLunoVM_execute_24B8(out, state, retVar);
}
#endif

//100%
INCLUDE_ASM("fe/all_in_one_fe", luno_cLunoVM_run1);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
struct LV_2648;
extern "C" void luno_cLunoTable__cLunoTable_2648(LV_2648*, int) __asm__("luno_cLunoTable__cLunoTable");
extern "C" void luno_cLunoTable_operatorE_2648(LV_2648*, const LV_2648*) __asm__("luno_cLunoTable_operatorE");
extern "C" void luno_cLunoVM_execute_2648(LV_2648**, char**, int) __asm__("luno_cLunoVM_execute");
extern char str_LunoVMRegister[];

struct LVPair_2648 { int a, b; };
struct LV_2648 {
    union { int i; LVPair_2648 pair; } u;
    int type;
    LV_2648() { type = eLunoVar_Nil; u.i = 0; }
    ~LV_2648() {}
    void Release() { if (type == eLunoVar_Table) luno_cLunoTable__cLunoTable_2648(this, 2); }
    void Clear() { type = eLunoVar_Nil; u.i = 0; }
    void SetTable(const LV_2648* src) {
        Release();
        Clear();
        luno_cLunoTable_operatorE_2648(this, src);
        type = eLunoVar_Table;
    }
    void Assign(const LV_2648* src) {
        Release();
        Clear();
        if (src->type == eLunoVar_Table) {
            luno_cLunoTable_operatorE_2648(this, src);
        } else if (src->type == eLunoVar_Func) {
            u.pair = src->u.pair;
        } else {
            u.i = src->u.i;
        }
        type = src->type;
    }
};

/**
 * Runs a Luno function with a global table and one argument: allocates the register file, puts the table in register 0
 * and @p value in register 1, then executes.
 *
 * Same as luno_cLunoVM_run except that register 1 is a copy of @p value (a table reference through luno_cLunoTable_operatorE, a
 * two-word value copied whole, or a single word). luno_cLunoVM_execute uses it for its call opcode (27).
 *
 * @param out  the VM; its first word receives the register array
 * @param state  the function to run (its first word points at the compiled chunk)
 * @param table  the global table, placed in register 0
 * @param value  the argument, copied into register 1
 * @param retVar  receives the function's return value
 */
extern "C" void luno_cLunoVM_run1(LV_2648** out, char** state, LV_2648* table, LV_2648* value, int retVar) {
    char* chunk = *state;
    int n = *(int*)(chunk + *(int*)(chunk + 4) + 8);
    LV_2648* arr = new (str_LunoVMRegister, 0x20000000, 0) LV_2648[n];
    *out = arr;
    arr[0].SetTable(table);
    LV_2648* regs = *out;
    LV_2648* secondReg = &regs[1];
    if (regs[1].type == eLunoVar_Table) luno_cLunoTable__cLunoTable_2648(secondReg, 2);
    secondReg->type = eLunoVar_Nil;
    regs[1].u.i = 0;
    if (value->type == eLunoVar_Table) {
        luno_cLunoTable_operatorE_2648(secondReg, value);
    } else if (value->type == eLunoVar_Func) {
        regs[1].u.pair = value->u.pair;
    } else {
        regs[1].u.i = value->u.i;
    }
    secondReg->type = value->type;
    luno_cLunoVM_execute_2648(out, state, retVar);
}
#endif
