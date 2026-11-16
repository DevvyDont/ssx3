#include "common.h"

INCLUDE_ASM("replay/replay2", cReplay_addCache);

INCLUDE_ASM("replay/replay2", cReplay_removeCache);

INCLUDE_ASM("replay/replay2", cReplay_allocReplayFrame);

INCLUDE_ASM("replay/replay2", cReplay_freeOneReplayFrame);

INCLUDE_ASM("replay/replay2", cReplay_freeReplayFrame);

INCLUDE_ASM("replay/replay2", cReplay_isPauseAI);

INCLUDE_ASM("replay/replay2", cReplay_rewind);

INCLUDE_ASM("replay/replay2", cReplay_lastFrame);

INCLUDE_ASM("replay/replay2", cReplay_stepFrame);

INCLUDE_ASM("replay/replay2", cReplay_purgeIdleFrames);

INCLUDE_ASM("replay/replay2", func_002705A8);

INCLUDE_ASM("replay/replay2", cReplay_endMarkInstantReplay);

INCLUDE_ASM("replay/replay2", cReplay_cancelMarkInstantReplay);

INCLUDE_ASM("replay/replay2", cReplay_startInstantReplay);

INCLUDE_ASM("replay/replay2", cReplay_stopInstantReplay);

INCLUDE_ASM("replay/replay2", cReplay_startAutoReplay);

INCLUDE_ASM("replay/replay2", cReplay_stopAutoReplay__FPv);

INCLUDE_ASM("replay/replay2", func_00270730);

INCLUDE_ASM("replay/replay2", cReplay_eventJump);

INCLUDE_ASM("replay/replay2", cReplay_eventLand);

INCLUDE_ASM("replay/replay2", cReplay_eventWipeout);

INCLUDE_ASM("replay/replay2", cReplay_eventReset);

INCLUDE_ASM("replay/replay2", cReplay_eventWScript);

INCLUDE_ASM("replay/replay2", cReplay_eventChapter);

INCLUDE_ASM("replay/replay2", func_00270B88);

INCLUDE_ASM("replay/replay2", cReplay_countHighlights);

INCLUDE_ASM("replay/replay2", cReplay_maxHighlights);

INCLUDE_ASM("replay/replay2", cReplay_compareHighlights);

INCLUDE_ASM("replay/replay2", func_00270CE8);

INCLUDE_ASM("replay/replay2", cReplay_cancelMarkHighlight);

INCLUDE_ASM("replay/replay2", cReplay_cancelMarkHighlights);

INCLUDE_ASM("replay/replay2", cReplay_initSave);

INCLUDE_ASM("replay/replay2", func_00270ED8);

INCLUDE_ASM("replay/replay2", func_00270EF8);

INCLUDE_ASM("replay/replay2", func_00270F18);

INCLUDE_ASM("replay/replay2", cReplay_allocSaveSpace);

INCLUDE_ASM("replay/replay2", cReplay_purgeSaveSpace);

INCLUDE_ASM("replay/replay2", cReplay_writeBuffered);

INCLUDE_ASM("replay/replay2", cReplay_writeSetup);

INCLUDE_ASM("replay/replay2", cReplay_getSaveData);

INCLUDE_ASM("replay/replay2", cReplay_writeHeaderStart);

INCLUDE_ASM("replay/replay2", cReplay_writeHeader);

INCLUDE_ASM("replay/replay2", cReplay_writeFrameCount);

INCLUDE_ASM("replay/replay2", cReplay_writeFramesStart);

INCLUDE_ASM("replay/replay2", func_00271380);

INCLUDE_ASM("replay/replay2", cReplay_writeInputStart);

INCLUDE_ASM("replay/replay2", func_00271510);

INCLUDE_ASM("replay/replay2", cReplay_writePadStart);

INCLUDE_ASM("replay/replay2", cReplay_writePad);

INCLUDE_ASM("replay/replay2", cReplay_writeCheckSumStart);

INCLUDE_ASM("replay/replay2", cReplay_writeCheckSum);

INCLUDE_ASM("replay/replay2", cReplay_load);

INCLUDE_ASM("replay/replay2", func_002718E8);

INCLUDE_ASM("replay/replay2", cReplayFramePtr__cReplayFramePtr);

INCLUDE_ASM("replay/replay2", func_00272258);

INCLUDE_ASM("replay/replay2", func_00272288);

INCLUDE_ASM("replay/replay2", func_002722C0);

INCLUDE_ASM("replay/replay2", func_002722E0);

INCLUDE_ASM("replay/replay2", func_00272308);

INCLUDE_ASM("replay/replay2", func_002723A8);

INCLUDE_ASM("replay/replay2", func_00272488);

INCLUDE_ASM("replay/replay2", func_002724C8);

INCLUDE_ASM("replay/replay2", func_002724E8);

INCLUDE_ASM("replay/replay2", func_00272510);

INCLUDE_ASM("replay/replay2", func_00272570);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_cScriptPlayerChannelSpline);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_Play);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_Update);

INCLUDE_ASM("replay/replay2", cInSeq_cInSeq);

INCLUDE_ASM("replay/replay2", cInSeq__cInSeq);

INCLUDE_ASM("replay/replay2", cInSeq_UpdateFrame);

INCLUDE_ASM("replay/replay2", cInSeq_ReadyPlayer);

INCLUDE_ASM("replay/replay2", func_002729A8);

INCLUDE_ASM("replay/replay2", cInSeq_Play);

INCLUDE_ASM("replay/replay2", cInSeq_Stop);

INCLUDE_ASM("replay/replay2", cInSeq_GetPlayer);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_cScriptPlayerChannelBlock);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock__cScriptPlayerChannelBlock);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_Play);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_Update);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_GetNextKeyFrame);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_AddActiveKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_LoadNextScriptKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_cScriptPlayerChannelEvent);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent__cScriptPlayerChannelEvent);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_Play);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_Update);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_GetNextKeyFrame);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_AddActiveKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_LoadNextScriptKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_cScriptPlayerChannelObject);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject__cScriptPlayerChannelObject);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_Play);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_Update);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_GetNextKeyFrame);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_AddActiveKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_LoadNextScriptKey);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_cScriptPlayerEntity);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity__cScriptPlayerEntity);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_Init);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_DeInit);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_SetEntityVariation);

INCLUDE_ASM("replay/replay2", func_00273AA8);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_ExecutePlay);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_Restart);

INCLUDE_ASM("replay/replay2", func_00274100);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_SetEntityComm);

INCLUDE_ASM("replay/replay2", func_002742C8);

INCLUDE_ASM("replay/replay2", func_002742E0);

INCLUDE_ASM("replay/replay2", func_00274348);

INCLUDE_ASM("replay/replay2", func_00274388);

INCLUDE_ASM("replay/replay2", func_002743C8);

INCLUDE_ASM("replay/replay2", cScriptPlayerEntity_DoFrameUpdate);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_cScriptPlayerMain);

INCLUDE_ASM("replay/replay2", func_002745C0);

INCLUDE_ASM("replay/replay2", func_002746F8);

INCLUDE_ASM("replay/replay2", func_00274808);

INCLUDE_ASM("replay/replay2", func_00274810);

INCLUDE_ASM("replay/replay2", func_00274830);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_Stop);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_Pause);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_Resume);

INCLUDE_ASM("replay/replay2", func_00274A30);

INCLUDE_ASM("replay/replay2", func_00274C10);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_GetEntity);

INCLUDE_ASM("replay/replay2", func_00274C38);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_GetScriptLength);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_GetNumUpdatesToEndLeft);

INCLUDE_ASM("replay/replay2", cScriptPlayerMain_GetInSeqCommInterface);

INCLUDE_ASM("replay/replay2", func_00274DE0);

INCLUDE_ASM("replay/replay2", func_002751A0);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_GetValue);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_GetCustom);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_GetNextKeyFrame);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_GetType);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_GetOwner);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelSpline_IsPlaying);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_GetValue);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_GetCustom);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_GetType);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_GetOwner);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelBlock_IsPlaying);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_GetValue);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_GetCustom);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_GetType);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_GetOwner);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelEvent_IsPlaying);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_GetValue);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_GetCustom);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_GetType);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_GetOwner);

INCLUDE_ASM("replay/replay2", cScriptPlayerChannelObject_IsPlaying);

