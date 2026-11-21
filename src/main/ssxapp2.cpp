#include "common.h"

INCLUDE_ASM("main/ssxapp2", cSSXApp__cSSXApp);

INCLUDE_ASM("main/ssxapp2", cFadeScreen__cFadeScreen);

INCLUDE_ASM("main/ssxapp2", cPreStartScreen__cPreStartScreen);

INCLUDE_ASM("main/ssxapp2", cPreStartScreen_load);

INCLUDE_ASM("main/ssxapp2", cLoadScreen__cLoadScreen);

INCLUDE_ASM("main/ssxapp2", cStartScreen__cStartScreen);

INCLUDE_ASM("main/ssxapp2", cGameLoadScreen__cGameLoadScreen);

INCLUDE_ASM("main/ssxapp2", cFELoadScreen__cFELoadScreen);

INCLUDE_ASM("main/ssxapp2", cGameState__cGameState);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadInit);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadUpdate);

INCLUDE_ASM("main/ssxapp2", cGameState_preloadCancel);

INCLUDE_ASM("main/ssxapp2", cGameState_gainFocus);

INCLUDE_ASM("main/ssxapp2", cGameState_update);

INCLUDE_ASM("main/ssxapp2", cGameState_loseFocus);

INCLUDE_ASM("main/ssxapp2", cGameState_isPreloadComplete);

INCLUDE_ASM("main/ssxapp2", cGameState_onWidgetEvent);

INCLUDE_ASM("main/ssxapp2", cGFEndState__cGFEndState);

INCLUDE_ASM("main/ssxapp2", cGFEndWaitState__cGFEndWaitState);

INCLUDE_ASM("main/ssxapp2", cGFFinishlineState__cGFFinishlineState);

INCLUDE_ASM("main/ssxapp2", cGFTransportWaitState__cGFTransportWaitState);

INCLUDE_ASM("main/ssxapp2", cGFEventStartState__cGFEventStartState);

INCLUDE_ASM("main/ssxapp2", cGFGateState__cGFGateState);

INCLUDE_ASM("main/ssxapp2", cGFRidingState__cGFRidingState);

INCLUDE_ASM("main/ssxapp2", cGFTrackStartState__cGFTrackStartState);

INCLUDE_ASM("main/ssxapp2", cGFNextHeatState__cGFNextHeatState);

INCLUDE_ASM("main/ssxapp2", cGFSessionTeleportState__cGFSessionTeleportState);

INCLUDE_ASM("main/ssxapp2", cGFUserSelectState__cGFUserSelectState);

INCLUDE_ASM("main/ssxapp2", cGFSectionLoadState__cGFSectionLoadState);

INCLUDE_ASM("main/ssxapp2", cGFPreEventState__cGFPreEventState);

INCLUDE_ASM("main/ssxapp2", cGFPostEventState__cGFPostEventState);

INCLUDE_ASM("main/ssxapp2", cGFPodiumState__cGFPodiumState);

INCLUDE_ASM("main/ssxapp2", cGFReplayStartState__cGFReplayStartState);

INCLUDE_ASM("main/ssxapp2", func_00244240);

INCLUDE_ASM("main/ssxapp2", func_002443C0);

INCLUDE_ASM("main/ssxapp2", cGenericStreamPause_StreamFocusRender);

INCLUDE_ASM("main/ssxapp2", cGenericStreamPause_Flush);

INCLUDE_ASM("main/ssxapp2", cInput_setPad);

INCLUDE_ASM("main/ssxapp2", cInput_setMap);

INCLUDE_ASM("main/ssxapp2", cVisualEffectsObjNode__cVisualEffectsObjNode);

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

INCLUDE_ASM("main/ssxapp2", cGMBigAirHandler__cGMBigAirHandler);

INCLUDE_ASM("main/ssxapp2", cGMRaceHandler__cGMRaceHandler);

INCLUDE_ASM("main/ssxapp2", cGMMPRaceHandler__cGMMPRaceHandler);

INCLUDE_ASM("main/ssxapp2", cGMMPSlopeHandler__cGMMPSlopeHandler);

INCLUDE_ASM("main/ssxapp2", cGMMPSlopeHandler_writeToReplayFrame);

INCLUDE_ASM("main/ssxapp2", cGMMPSlopeHandler_readFromReplayFrame);

INCLUDE_ASM("main/ssxapp2", func_00244630);

INCLUDE_ASM("main/ssxapp2", cGMTimeHandler__cGMTimeHandler);

INCLUDE_ASM("main/ssxapp2", func_00244690);

INCLUDE_ASM("main/ssxapp2", func_002446C0);

INCLUDE_ASM("main/ssxapp2", cGMPointHandler__cGMPointHandler);

INCLUDE_ASM("main/ssxapp2", func_00244720);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackMemcard);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackFalse);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_CallbackTrue);

INCLUDE_ASM("main/ssxapp2", cMCOverlayManager_Callbackpdate);

INCLUDE_ASM("main/ssxapp2", cGameComm_stopSyncInput);

INCLUDE_ASM("main/ssxapp2", cAIVisualEffectsObjNode__cAIVisualEffectsObjNode);

INCLUDE_ASM("main/ssxapp2", cAIVisualEffectsObjNode_update);

INCLUDE_ASM("main/ssxapp2", cAIVisualEffectsObjNode_render);

INCLUDE_ASM("main/ssxapp2", cSSXStatsObjNode__cSSXStatsObjNode);

INCLUDE_ASM("main/ssxapp2", cSSXStatsObjNode_update);

INCLUDE_ASM("main/ssxapp2", cSSXStatsObjNode_render);

INCLUDE_ASM("main/ssxapp2", func_00245228);

INCLUDE_ASM("main/ssxapp2", func_00245248);

INCLUDE_ASM("main/ssxapp2", func_002452D8);

INCLUDE_ASM("main/ssxapp2", func_002453E0);

INCLUDE_ASM("main/ssxapp2", cGameLoadState__cGameLoadState);

INCLUDE_ASM("main/ssxapp2", cGameLoadState_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", func_002454F8);

INCLUDE_ASM("main/ssxapp2", cGameLoadState_onWidgetCreate);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge__cGameLoadStateOutLodge);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge_onCreateScreen);

INCLUDE_ASM("main/ssxapp2", cGameLoadStateOutLodge_onWidgetCreate);

INCLUDE_ASM("main/ssxapp2", func_00245698);

INCLUDE_ASM("main/ssxapp2", func_00245708);

INCLUDE_ASM("main/ssxapp2", func_00245730);

INCLUDE_ASM("main/ssxapp2", func_002458B0);

INCLUDE_ASM("main/ssxapp2", func_002458B8);

INCLUDE_ASM("main/ssxapp2", cFELoadHintState__cFELoadHintState);

