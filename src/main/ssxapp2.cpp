#include "common.h"

INCLUDE_ASM("main/ssxapp2", cSSXApp__cSSXApp);

INCLUDE_ASM("main/ssxapp2", cFadeScreen__cFadeScreen);

INCLUDE_ASM("main/ssxapp2", _$_15cPreStartScreen);

INCLUDE_ASM("main/ssxapp2", cPreStartScreen_load);

INCLUDE_ASM("main/ssxapp2", cLoadScreen__cLoadScreen);

INCLUDE_ASM("main/ssxapp2", _$_12cStartScreen);

INCLUDE_ASM("main/ssxapp2", _$_15cGameLoadScreen);

INCLUDE_ASM("main/ssxapp2", _$_13cFELoadScreen);

INCLUDE_ASM("main/ssxapp2", cGameState__cGameState);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadInit);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadUpdate);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadCancel);

INCLUDE_ASM("main/ssxapp2", cGameState_gainFocus);

INCLUDE_ASM("main/ssxapp2", cGameState_update);

INCLUDE_ASM("main/ssxapp2", cGameState_loseFocus);

INCLUDE_ASM("main/ssxapp2", cGameState_isPreloadComplete);

INCLUDE_ASM("main/ssxapp2", cGameState_onWidgetEvent);

INCLUDE_ASM("main/ssxapp2", _$_11cGFEndState);

INCLUDE_ASM("main/ssxapp2", _$_15cGFEndWaitState);

INCLUDE_ASM("main/ssxapp2", _$_18cGFFinishlineState);

INCLUDE_ASM("main/ssxapp2", _$_21cGFTransportWaitState);

INCLUDE_ASM("main/ssxapp2", _$_18cGFEventStartState);

INCLUDE_ASM("main/ssxapp2", _$_12cGFGateState);

INCLUDE_ASM("main/ssxapp2", _$_14cGFRidingState);

INCLUDE_ASM("main/ssxapp2", _$_18cGFTrackStartState);

INCLUDE_ASM("main/ssxapp2", _$_16cGFNextHeatState);

INCLUDE_ASM("main/ssxapp2", _$_23cGFSessionTeleportState);

INCLUDE_ASM("main/ssxapp2", _$_18cGFUserSelectState);

INCLUDE_ASM("main/ssxapp2", _$_19cGFSectionLoadState);

INCLUDE_ASM("main/ssxapp2", _$_16cGFPreEventState);

INCLUDE_ASM("main/ssxapp2", _$_17cGFPostEventState);

INCLUDE_ASM("main/ssxapp2", _$_14cGFPodiumState);

INCLUDE_ASM("main/ssxapp2", _$_19cGFReplayStartState);

INCLUDE_ASM("main/ssxapp2", cPauseState_set);

INCLUDE_ASM("main/ssxapp2", cGenericStreamPause__cGenericStreamPause);

INCLUDE_ASM("main/ssxapp2", cGenericStreamPause_StreamFocusRender);

INCLUDE_ASM("main/ssxapp2", cGenericStreamPause_Flush);

INCLUDE_ASM("main/ssxapp2", cInput_setPad);

INCLUDE_ASM("main/ssxapp2", cInput_setMap);

INCLUDE_ASM("main/ssxapp2", _$_21cVisualEffectsObjNode);

INCLUDE_ASM("main/ssxapp2", cVisualEffectsObjNode_update);

INCLUDE_ASM("main/ssxapp2", cVisualEffectsObjNode_render);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler__cGameModeHandler);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_init);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_riderCheckpointHit);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_riderGateHit);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_raceFinish);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_getHeatScore);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_writeToReplayFrame);

INCLUDE_ASM("main/ssxapp2", cGameModeHandler_readFromReplayFrame);

INCLUDE_ASM("main/ssxapp2", _$_16cGMBigAirHandler);

INCLUDE_ASM("main/ssxapp2", _$_14cGMRaceHandler);

INCLUDE_ASM("main/ssxapp2", _$_16cGMMPRaceHandler);

INCLUDE_ASM("main/ssxapp2", _$_17cGMMPSlopeHandler);

INCLUDE_ASM("main/ssxapp2", cGMMPSlopeHandler_writeToReplayFrame);

INCLUDE_ASM("main/ssxapp2", cGMMPSlopeHandler_readFromReplayFrame);

INCLUDE_ASM("main/ssxapp2", _$_18cGMFreerideHandler);

INCLUDE_ASM("main/ssxapp2", _$_14cGMTimeHandler);

INCLUDE_ASM("main/ssxapp2", _$_19cGMRivalTimeHandler);

INCLUDE_ASM("main/ssxapp2", _$_20cGMRivalPointHandler);

INCLUDE_ASM("main/ssxapp2", _$_15cGMPointHandler);

INCLUDE_ASM("main/ssxapp2", cGMDebugHandler__cGMDebugHandler);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackMemcard);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackFalse);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackTrue);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_Callbackpdate);

INCLUDE_ASM("main/ssxapp2", cGameComm_stopSyncInput);

INCLUDE_ASM("main/ssxapp2", _$_23cAIVisualEffectsObjNode);

INCLUDE_ASM("main/ssxapp2", cAIVisualEffectsObjNode_update);

INCLUDE_ASM("main/ssxapp2", cAIVisualEffectsObjNode_render);

INCLUDE_ASM("main/ssxapp2", _$_16cSSXStatsObjNode);

INCLUDE_ASM("main/ssxapp2", cSSXStatsObjNode_update);

INCLUDE_ASM("main/ssxapp2", cSSXStatsObjNode_render);

INCLUDE_ASM("main/ssxapp2", cFEStateProgressive__cFEStateProgressive);

INCLUDE_ASM("main/ssxapp2", cFEStateProgressive_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", cFEStateProgressive_onWidgetEvent);

INCLUDE_ASM("main/ssxapp2", cFEStateProgressive_onWidgetMenu);

INCLUDE_ASM("main/ssxapp2", _$_14cGameLoadState);

INCLUDE_ASM("main/ssxapp2", cGameLoadState_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", cGameLoadState_onGainTransition);

INCLUDE_ASM("main/ssxapp2", cGameLoadState_onWidgetCreate);

INCLUDE_ASM("main/ssxapp2", _$_22cGameLoadStateOutLodge);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge_onWidgetCreate);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge_onGainTransition);

INCLUDE_ASM("main/ssxapp2", _$_21cGameLoadStateConquer);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateConquer_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateConquer_onWidgetCreate);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateConquer_onGainTransition);

INCLUDE_ASM("main/ssxapp2", _$_16cFELoadHintState);

