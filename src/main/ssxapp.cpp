#include "common.h"
#include "be/beinterface.h"
#include "be/beintoption.h"
#include "util/consoleconfig.h"
#include "intersect/bxnash.h"
#include "main/game.h"
#include "world/worldrecords.h"
#include "world/instancerecord.h"
#include "render/occlusionman.h"
#include "vtable.h"
#include "input/inputparse.h"
#include "bx/math3d.h"
#include "main/crowdman2d.h"
#include "main/gamerender.h"
#include "sound/icepick/worldtriggermanager.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "world/world.h"
#include "online/commsystem.h"
#include "scripter/ssxscriptengine.h"
#include "bx/bxstring.h"
#include "main/ssxapp.h"
#include "replay/bxreplayframe.h"
#include "render/graphicsman.h"
#include "input/padcache.h"
#include "main/loadindicator.h"
#include "util/locale.h"
#include "world/patchrecord.h"
#include "world/worldcache.h"
#include "visualfx/lensfx.h"

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_cSSXApp);
#ifdef SKIP_ASM
extern "C" void* cAppMan_cAppMan(void* self);
extern "C" void MEM_fill(void* dst, int c, int n);
extern int cBXString_EmptyString;
extern void* cSSXApp_vtbl[];


/**
 * Constructs the SSX application object on top of cAppMan, with empty settings.
 *
 * After cAppMan_cAppMan() and the vtable, UserName, Password and CommandLineTrack point at the shared empty
 * string, several state words are zeroed, +0x60 and +0x64 are set to 1, and the two-entry InputPads, InputMaps
 * and Inputs arrays are cleared.
 *
 * @return @p self
 */
extern "C" cSSXApp* cSSXApp_cSSXApp(cSSXApp* self)
{
    cAppMan_cAppMan((self));
    self->mVtbl = cSSXApp_vtbl;
    self->UserName = (char*)cBXString_EmptyString;
    self->Password = (char*)cBXString_EmptyString;
    self->CommandLineTrack = (char*)cBXString_EmptyString;
    self->mAudioBigFile = 0;
    self->mMusicBigFile = 0;
    self->mMusic2BigFile = 0;
    self->mSpeechBigFile = 0;
    self->mScriptDataBigFile = 0;
    self->mModelBigFile = 0;
    int i;
    for (i = 0; i < 11; i++) {
        self->mTextureBigFile[i] = 0;
    }
    self->mRewardsBigFile = 0;
    self->AutoLobby = 0;
    self->unk_68 = 0;
    self->unk_6C = 0;
    self->unk_70 = 0;
    self->UseRealFE = 1;
    self->ShowHUD = 1;
    MEM_fill(((char*)&self->InputPads[0]), 0, 8);
    MEM_fill(((char*)&self->InputMaps[0]), 0, 8);
    MEM_fill(((char*)&self->Inputs[0]), 0, 8);
    return (self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_init);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

extern "C" void cPadCache_init(cPadCache* self);

// tARGB from its four channels, in member order (taken by reference).
struct tARGBInit : tARGB {
    tARGBInit(const float& a, const float& r, const float& g, const float& b) { A = a; R = r; G = g; B = b; }
};

// cLoadIndicator with its inline constructor: phase, alpha and locale 0, then an opaque white colour.
struct cLoadIndicatorNew : cLoadIndicator {
    cLoadIndicatorNew() { phase = 0; alpha = 0; locale = 0; colour = tARGBInit(1.0f, 1.0f, 1.0f, 1.0f); }
};

extern void* PadCache;
extern void* G2D;
extern char str_PadCache[];
extern char str_2DRender[];
extern char str_InputPad[];
extern char str_loadicon[];
extern char str_LoadIndicator[];
extern char str_PreStartScreen[];
extern char str_FEAsyncManager[];
extern char loadicon[];
extern char str_load[];
extern "C" void* cConsoleConfig_construct();
extern "C" void* cGraphicsMan_construct();
void cLightMan_construct();
void cBezierMan_construct();
extern "C" void cSSXApp_parseCommandLine(cSSXApp* self, int argc, char** argv);
extern "C" void* SHAPE_locate(const char* name, void* list);
extern "C" void cGraphicsMan_UploadTextures(void* gfx);
void* cPreStartScreen_cPreStartScreen(void* p);
extern "C" void* cFEAsyncManager_cFEAsyncManager(void* p);
extern "C" void cFEAsyncManager_Init(void* p);
extern "C" void* cGameModeMan_getGM();
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);

/**
 * Initialises the application: creates the core subsystems, parses the command line and queues the first
 * module.
 *
 * Sets SSXApp; creates ConsoleConfig, the pad cache ("PadCache", also the global PadCache), GrMan (then its
 * Init, slot 2), the light and bezier managers, a "2DRender" object and the two "InputPad"s; clears Game,
 * DebugFE and RealFE; parses the arguments; loads the "loadicon" texture (+0xA0) from an embedded shape file;
 * creates the white LoadIndicator; queues a new cPreStartScreen; creates and inits the cFEAsyncManager.
 *
 * @param argc  number of command-line arguments (On Tour ac)
 * @param argv  the command-line arguments (On Tour av)
 */
void cSSXApp::init(int argc, char** argv)
{
    SSXApp = (cSSXApp*)this;
    *(void**)&ConsoleConfig = cConsoleConfig_construct();
    cPadCache* volatile pm = (cPadCache*)cMemMan_alloc(12028, str_PadCache, 0, 0);
    cPadCache_init(pm);
    PadCache = pm;
    cPS2GraphicsMan* graphicsMan = (cPS2GraphicsMan*)cGraphicsMan_construct();
    GrMan = graphicsMan;
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cGraphicsMan, graphicsMan);
    vt[2].fn(vthis(graphicsMan, vt[2])); // slot 2: Init
    cLightMan_construct();
    cBezierMan_construct();
    G2D = cMemMan_alloc(1, str_2DRender, 0, 0);
    int i;
    for (i = 0; i < 2; i++) {
        int* inputPad = (int*)cMemMan_alloc(676, str_InputPad, 0, 0);
        InputPads[i] = (cInputPad*)inputPad;
        *inputPad = 0;
    }
    *(int*)&Game = 0;
    *(int*)&DebugFE = 0;
    *(int*)&RealFE = 0;
    parseCommandLine(argc, argv);
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    vt2[44].fn(vthis(ps2GraphicsMan, vt2[44]), 0x100); // slot 44: InitTextures
    cPS2GraphicsMan* gfxMan = GrMan;
    sVEntry<int(void*, void*, const char*, int, int, int)>* vt3 =
        (sVEntry<int(void*, void*, const char*, int, int, int)>*)VPTR_OF(cGraphicsMan, gfxMan);
    void* gfxThis = vthis(gfxMan, vt3[46]); // slot 46: AddTexture
    void* loadShape = SHAPE_locate(loadicon, str_load);
    SSXApp->LoadIconTextureID = vt3[46].fn(gfxThis, loadShape, str_loadicon, 0, 1, -1);
    cGraphicsMan_UploadTextures(GrMan);
    LoadIndicator = new (str_LoadIndicator, 0, 0) cLoadIndicatorNew;
    cAppMan_setNextModule((cAppMan*)this, (unsigned int)cPreStartScreen_cPreStartScreen(cMemMan_alloc(0xC,
            str_PreStartScreen, 0x100, 0)));
    *(void**)&GameModeMan = cGameModeMan_getGM();
    void* feAsyncManager = cFEAsyncManager_cFEAsyncManager(cMemMan_alloc(1424, str_FEAsyncManager, 0, 0));
    *(void**)&mFEAsyncManager = feAsyncManager;
    cFEAsyncManager_Init(feAsyncManager);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_loadInputMap);
#ifdef SKIP_ASM
extern "C" void* cInputPreProcessor_loadFileSize(void* buf, const char* path, int* outSize);
extern "C" void cInputMap_init(void* self, int count);
extern "C" void cInputMap_loadMapFile(void* self, void* data, int size);
extern "C" void cInputMap_compileMap(void* self, int idx, char* src);
extern "C" void cInputMap_purgeMapFile(void* self);
void cMemMan_free(void* p);
extern char str_InputMap[];
extern char str_CruiseTurn[];
extern char str_CruiseCrouch[];
extern char str_CruiseBrake[];
extern char str_BoardPress[];
extern char str_BoardPivot[];
extern char str_AttackLeft[];
extern char str_AttackRight[];
extern char str_PrewindTurn[];
extern char str_PrewindSpin[];
extern char str_PrewindFlip[];
extern char str_AirAdjRotFB[];
extern char str_AirAdjRotLR[];
extern char str_LateSpin[];
extern char str_RailSpin[];
extern char str_Spin[];
extern char str_Flip[];
extern char str_WipeoutRecover[];
extern char str_JumpPressed[];
extern char str_JumpHeld[];
extern char str_BoostPressed[];
extern char str_BoostHeld[];
extern char str_Tweak[];
extern char str_OllieHeld[];
extern char str_RailBalance[];
extern char str_HandplantBalance[];
extern char str_Handplant[];
extern char str_GateAnticipate[];
extern char str_UberGrind1[];
extern char str_UberGrind2[];
extern char str_UberGrind3[];
extern char str_UberGrind4[];
extern char str_Trick1[];
extern char str_Trick2[];
extern char str_Trick3[];
extern char str_Trick4[];
extern char str_Trick5[];
extern char str_Trick6[];
extern char str_Trick7[];
extern char str_Trick8[];
extern char str_Trick9[];
extern char str_Trick10[];
extern char str_Trick11[];
extern char str_Trick12[];
extern char str_Trick13[];
extern char str_Trick14[];
extern char str_Trick15[];
extern char str_ResetPath[];
extern char str_CameraActivate[];
extern char str_CameraTurn[];
extern char str_CameraPitch[];
extern char str_CameraMove[];
extern char str_CameraStrafe[];
extern char str_CameraBoost[];
extern char str_CameraInGameTurn[];
extern char str_CameraInGamePitch[];
extern char str_CameraInGameZoom[];
extern char str_CameraInGameNext[];
extern char str_CameraNextStatCam[];
extern char str_CameraPrevStatCam[];
extern char str_CameraGoToStatCam[];
extern char str_MenuButton[];
extern char str_MenuKey[];
extern char str_MenuLeft[];
extern char str_MenuRight[];
extern char str_MenuUp[];
extern char str_MenuDown[];
extern char str_MenuLeftShift[];
extern char str_MenuRightShift[];
extern char str_EditorHelp[];
extern char str_EditorCursorSnap[];
extern char str_EditorRotateLeft[];
extern char str_EditorRotateRight[];
extern char str_EditorRotateUp[];
extern char str_EditorRotateDown[];
extern char str_EditorRotateHold[];
extern char str_EditorZoomIn[];
extern char str_EditorZoomOut[];
extern char str_EditorModeOrient[];
extern char str_EditorModeSlow[];
extern char str_EditorModeFast[];
extern char str_EditorModeFaster[];
extern char str_EditorModeLeft[];
extern char str_EditorModeRight[];
extern char str_EditorModeUp[];
extern char str_EditorModeDown[];
extern char str_EditorTeleportAI[];
extern char str_PEAddEvent[];
extern char str_PEStartPath[];
extern char str_PEMoveNode[];
extern char str_PEDeleteNode[];
extern char str_PEWaitForNoInput[];
extern char str_CVToggleDisplayMode[];
extern char str_CVMoveCursor[];
extern char str_CVTriggerHeld[];
extern char str_CVTriggerReleased[];
extern char str_CVMoveModelChangeAxis[];
extern char str_CVMoveModelToggle[];
extern char str_CVMoveModelCancel[];
extern char str_CVEditModelSelect[];
extern char str_CVEditModelHideMenu[];
extern char str_CVEditModelZeroValue[];
extern char str_FEAdvance[];
extern char str_ReplayExit[];
extern char str_ReplayPlay[];
extern char str_ReplayFForward[];
extern char str_ReplayFBackward[];
extern char str_ReplayStepForward[];
extern char str_ReplayStepSlow[];
extern char str_ReplayCycleCamera[];
extern char str_ReplayDebugLoad[];
extern char str_ReplayDebugSave[];
extern char str_NISSkip[];
extern char str_UIStart[];
extern char str_UISelect[];
extern char str_UIUp[];
extern char str_UIDown[];
extern char str_UILeft[];
extern char str_UIRight[];
extern char str_UIUp2[];
extern char str_UIDown2[];
extern char str_UILeft2[];
extern char str_UIRight2[];
extern char str_UINext[];
extern char str_UIPrevious[];
extern char str_UIOption[];
extern char str_UIMisc[];
extern char str_UIRShift1[];
extern char str_UIRShift2[];
extern char str_UILShift1[];
extern char str_UILShift2[];
extern char str_UIRShift1Held[];
extern char str_UIRShift2Held[];
extern char str_UILShift1Held[];
extern char str_UILShift2Held[];
extern char str_UICheatMode[];
extern char str_UICheatUp[];
extern char str_UICheatDown[];
extern char str_UICheatLeft[];
extern char str_UICheatRight[];
extern char str_UICheatButton1[];
extern char str_UICheatButton2[];
extern char str_UICheatButton3[];
extern char str_UICheatButton4[];
extern char str_UICheatAnyKey[];
extern char str_UICamZoom[];
extern char str_UICamRotate[];
extern char str_UICamPan[];
extern char str_UIUpHeld[];
extern char str_UIDownHeld[];
extern char str_UILeftHeld[];
extern char str_UIRightHeld[];
extern char str_UINextHeld[];
extern char str_UIPreviousHeld[];
extern char str_UIOptionHeld[];
extern char str_UIMiscHeld[];
extern char str_UINextReleased[];
extern char str_UIPreviousReleased[];
extern char str_UIOptionReleased[];
extern char str_UIMiscReleased[];
extern char str_ScriptMenuKey[];
extern char str_ScriptEditHelp[];
extern char str_ScriptEditHeadingDelta[];
extern char str_ScriptEditPitchDelta[];
extern char str_ScriptEditFOVDelta[];
extern char str_ScriptEditRollDelta[];
extern char str_ScriptEditJitterDelta[];
extern char str_ScriptEditJitterRollDelta[];
extern char str_ScriptEditManualForwardMove[];
extern char str_ScriptEditManualSideMove[];
extern char str_ScriptEditManualUpMove[];
extern char str_ScriptEditTargetForwardMove[];
extern char str_ScriptEditTargetSideMove[];
extern char str_ScriptEditTargetUpMove[];
extern char str_ScriptEditTargetCamDistDelta[];
extern char str_ScriptEditSubjectLeadDistDelta[];
extern char str_ScriptEditSubjectLeadHeightDelta[];
extern char str_ScriptEditSubjectMoveCamForward[];
extern char str_ScriptEditAlgoLeadDistDelta[];
extern char str_ScriptEditAlgoLeadHeightDelta[];
extern char str_ScriptEditAlgoCamDistDelta[];
extern char str_ScreenCapture[];
extern char str_VideoCapture[];
extern char str_NetRiderAnalyzerToggle[];
extern char str_MemCardKey[];
extern char str_TestCameraFB[];
extern char str_TestCameraLR[];
extern char str_TestCameraOnOff[];
extern char str_ChaseCamZoom[];
extern char str_ChaseCamLR[];
extern char str_ChaseCam[];
extern char str_ChaseManualCamHeading[];
extern char str_ChaseManualCamPitch[];
extern char str_ChaseManualCamStrafe[];
extern char str_ChaseManualCamForward[];
extern char str_ChaseManualCamFast[];
extern char str_TestTerrainType[];
extern char str_MissionDebugPlay[];
extern char str_MissionDebugPreview[];
extern char str_MissionDebugRecord[];
extern char str_MissionDebugAnyPressed[];
extern char str_VoipOnOff[];
extern char str_ConsoleReset[];
extern char str_ReplayMenuUp[];
extern char str_ReplayMenuDown[];
extern char str_ReplayTimeLine[];


/**
 * Loads an input map file and compiles it into a new cInputMap.
 *
 * Reads the file (cInputPreProcessor_loadFileSize()), creates a 0x18-byte "InputMap" for 205 actions, parses
 * the file data and frees it, compiles actions 0-204 by name ("CruiseTurn", "CruiseCrouch" ...
 * "ReplayTimeLine"), then drops the parsed file.
 *
 * @param path  the map file, e.g. "data/config/input.map"
 * @return the new cInputMap, as an int (On Tour returns cInputMap*)
 */
// PORT: the unit declares it returning int (callers store the map pointer in an int field).
int cSSXApp::loadInputMap(const char* path)
{
    cInputPreProcessor buf;
    int size;
    void* data = cInputPreProcessor_loadFileSize(&buf, path, &size);
    void* map = cMemMan_alloc(0x18, str_InputMap, 0, 0);
    cInputMap_init(map, 205);
    cInputMap_loadMapFile(map, data, size);
    if (data != 0) {
        cMemMan_free(data);
    }
    cInputMap_compileMap(map, 0, str_CruiseTurn);
    cInputMap_compileMap(map, 1, str_CruiseCrouch);
    cInputMap_compileMap(map, 2, str_CruiseBrake);
    cInputMap_compileMap(map, 3, str_BoardPress);
    cInputMap_compileMap(map, 4, str_BoardPivot);
    cInputMap_compileMap(map, 5, str_AttackLeft);
    cInputMap_compileMap(map, 6, str_AttackRight);
    cInputMap_compileMap(map, 7, str_PrewindTurn);
    cInputMap_compileMap(map, 8, str_PrewindSpin);
    cInputMap_compileMap(map, 9, str_PrewindFlip);
    cInputMap_compileMap(map, 10, str_AirAdjRotFB);
    cInputMap_compileMap(map, 11, str_AirAdjRotLR);
    cInputMap_compileMap(map, 12, str_LateSpin);
    cInputMap_compileMap(map, 13, str_RailSpin);
    cInputMap_compileMap(map, 14, str_Spin);
    cInputMap_compileMap(map, 15, str_Flip);
    cInputMap_compileMap(map, 16, str_WipeoutRecover);
    cInputMap_compileMap(map, 17, str_JumpPressed);
    cInputMap_compileMap(map, 18, str_JumpHeld);
    cInputMap_compileMap(map, 19, str_BoostPressed);
    cInputMap_compileMap(map, 20, str_BoostHeld);
    cInputMap_compileMap(map, 21, str_Tweak);
    cInputMap_compileMap(map, 22, str_OllieHeld);
    cInputMap_compileMap(map, 23, str_RailBalance);
    cInputMap_compileMap(map, 24, str_HandplantBalance);
    cInputMap_compileMap(map, 25, str_Handplant);
    cInputMap_compileMap(map, 26, str_GateAnticipate);
    cInputMap_compileMap(map, 27, str_UberGrind1);
    cInputMap_compileMap(map, 28, str_UberGrind2);
    cInputMap_compileMap(map, 29, str_UberGrind3);
    cInputMap_compileMap(map, 30, str_UberGrind4);
    cInputMap_compileMap(map, 31, str_Trick1);
    cInputMap_compileMap(map, 32, str_Trick2);
    cInputMap_compileMap(map, 33, str_Trick3);
    cInputMap_compileMap(map, 34, str_Trick4);
    cInputMap_compileMap(map, 35, str_Trick5);
    cInputMap_compileMap(map, 36, str_Trick6);
    cInputMap_compileMap(map, 37, str_Trick7);
    cInputMap_compileMap(map, 38, str_Trick8);
    cInputMap_compileMap(map, 39, str_Trick9);
    cInputMap_compileMap(map, 40, str_Trick10);
    cInputMap_compileMap(map, 41, str_Trick11);
    cInputMap_compileMap(map, 42, str_Trick12);
    cInputMap_compileMap(map, 43, str_Trick13);
    cInputMap_compileMap(map, 44, str_Trick14);
    cInputMap_compileMap(map, 45, str_Trick15);
    cInputMap_compileMap(map, 46, str_ResetPath);
    cInputMap_compileMap(map, 47, str_CameraActivate);
    cInputMap_compileMap(map, 48, str_CameraTurn);
    cInputMap_compileMap(map, 49, str_CameraPitch);
    cInputMap_compileMap(map, 50, str_CameraMove);
    cInputMap_compileMap(map, 51, str_CameraStrafe);
    cInputMap_compileMap(map, 52, str_CameraBoost);
    cInputMap_compileMap(map, 53, str_CameraInGameTurn);
    cInputMap_compileMap(map, 54, str_CameraInGamePitch);
    cInputMap_compileMap(map, 55, str_CameraInGameZoom);
    cInputMap_compileMap(map, 56, str_CameraInGameNext);
    cInputMap_compileMap(map, 57, str_CameraNextStatCam);
    cInputMap_compileMap(map, 58, str_CameraPrevStatCam);
    cInputMap_compileMap(map, 59, str_CameraGoToStatCam);
    cInputMap_compileMap(map, 60, str_MenuButton);
    cInputMap_compileMap(map, 61, str_MenuKey);
    cInputMap_compileMap(map, 62, str_MenuLeft);
    cInputMap_compileMap(map, 63, str_MenuRight);
    cInputMap_compileMap(map, 64, str_MenuUp);
    cInputMap_compileMap(map, 65, str_MenuDown);
    cInputMap_compileMap(map, 66, str_MenuLeftShift);
    cInputMap_compileMap(map, 67, str_MenuRightShift);
    cInputMap_compileMap(map, 68, str_EditorHelp);
    cInputMap_compileMap(map, 69, str_EditorCursorSnap);
    cInputMap_compileMap(map, 70, str_EditorRotateLeft);
    cInputMap_compileMap(map, 71, str_EditorRotateRight);
    cInputMap_compileMap(map, 72, str_EditorRotateUp);
    cInputMap_compileMap(map, 73, str_EditorRotateDown);
    cInputMap_compileMap(map, 74, str_EditorRotateHold);
    cInputMap_compileMap(map, 75, str_EditorZoomIn);
    cInputMap_compileMap(map, 76, str_EditorZoomOut);
    cInputMap_compileMap(map, 77, str_EditorModeOrient);
    cInputMap_compileMap(map, 78, str_EditorModeSlow);
    cInputMap_compileMap(map, 79, str_EditorModeFast);
    cInputMap_compileMap(map, 80, str_EditorModeFaster);
    cInputMap_compileMap(map, 81, str_EditorModeLeft);
    cInputMap_compileMap(map, 82, str_EditorModeRight);
    cInputMap_compileMap(map, 83, str_EditorModeUp);
    cInputMap_compileMap(map, 84, str_EditorModeDown);
    cInputMap_compileMap(map, 85, str_EditorTeleportAI);
    cInputMap_compileMap(map, 86, str_PEAddEvent);
    cInputMap_compileMap(map, 87, str_PEStartPath);
    cInputMap_compileMap(map, 88, str_PEMoveNode);
    cInputMap_compileMap(map, 89, str_PEDeleteNode);
    cInputMap_compileMap(map, 90, str_PEWaitForNoInput);
    cInputMap_compileMap(map, 91, str_CVToggleDisplayMode);
    cInputMap_compileMap(map, 92, str_CVMoveCursor);
    cInputMap_compileMap(map, 93, str_CVTriggerHeld);
    cInputMap_compileMap(map, 94, str_CVTriggerReleased);
    cInputMap_compileMap(map, 95, str_CVMoveModelChangeAxis);
    cInputMap_compileMap(map, 96, str_CVMoveModelToggle);
    cInputMap_compileMap(map, 97, str_CVMoveModelCancel);
    cInputMap_compileMap(map, 98, str_CVEditModelSelect);
    cInputMap_compileMap(map, 99, str_CVEditModelHideMenu);
    cInputMap_compileMap(map, 100, str_CVEditModelZeroValue);
    cInputMap_compileMap(map, 101, str_FEAdvance);
    cInputMap_compileMap(map, 102, str_ReplayExit);
    cInputMap_compileMap(map, 103, str_ReplayPlay);
    cInputMap_compileMap(map, 104, str_ReplayFForward);
    cInputMap_compileMap(map, 105, str_ReplayFBackward);
    cInputMap_compileMap(map, 106, str_ReplayStepForward);
    cInputMap_compileMap(map, 107, str_ReplayStepSlow);
    cInputMap_compileMap(map, 108, str_ReplayCycleCamera);
    cInputMap_compileMap(map, 109, str_ReplayDebugLoad);
    cInputMap_compileMap(map, 110, str_ReplayDebugSave);
    cInputMap_compileMap(map, 111, str_NISSkip);
    cInputMap_compileMap(map, 112, str_UIStart);
    cInputMap_compileMap(map, 113, str_UISelect);
    cInputMap_compileMap(map, 114, str_UIUp);
    cInputMap_compileMap(map, 115, str_UIDown);
    cInputMap_compileMap(map, 116, str_UILeft);
    cInputMap_compileMap(map, 117, str_UIRight);
    cInputMap_compileMap(map, 118, str_UIUp2);
    cInputMap_compileMap(map, 119, str_UIDown2);
    cInputMap_compileMap(map, 120, str_UILeft2);
    cInputMap_compileMap(map, 121, str_UIRight2);
    cInputMap_compileMap(map, 122, str_UINext);
    cInputMap_compileMap(map, 123, str_UIPrevious);
    cInputMap_compileMap(map, 124, str_UIOption);
    cInputMap_compileMap(map, 125, str_UIMisc);
    cInputMap_compileMap(map, 126, str_UIRShift1);
    cInputMap_compileMap(map, 127, str_UIRShift2);
    cInputMap_compileMap(map, 128, str_UILShift1);
    cInputMap_compileMap(map, 129, str_UILShift2);
    cInputMap_compileMap(map, 130, str_UIRShift1Held);
    cInputMap_compileMap(map, 131, str_UIRShift2Held);
    cInputMap_compileMap(map, 132, str_UILShift1Held);
    cInputMap_compileMap(map, 133, str_UILShift2Held);
    cInputMap_compileMap(map, 134, str_UICheatMode);
    cInputMap_compileMap(map, 135, str_UICheatUp);
    cInputMap_compileMap(map, 136, str_UICheatDown);
    cInputMap_compileMap(map, 137, str_UICheatLeft);
    cInputMap_compileMap(map, 138, str_UICheatRight);
    cInputMap_compileMap(map, 139, str_UICheatButton1);
    cInputMap_compileMap(map, 140, str_UICheatButton2);
    cInputMap_compileMap(map, 141, str_UICheatButton3);
    cInputMap_compileMap(map, 142, str_UICheatButton4);
    cInputMap_compileMap(map, 143, str_UICheatAnyKey);
    cInputMap_compileMap(map, 144, str_UICamZoom);
    cInputMap_compileMap(map, 145, str_UICamRotate);
    cInputMap_compileMap(map, 146, str_UICamPan);
    cInputMap_compileMap(map, 147, str_UIUpHeld);
    cInputMap_compileMap(map, 148, str_UIDownHeld);
    cInputMap_compileMap(map, 149, str_UILeftHeld);
    cInputMap_compileMap(map, 150, str_UIRightHeld);
    cInputMap_compileMap(map, 151, str_UINextHeld);
    cInputMap_compileMap(map, 152, str_UIPreviousHeld);
    cInputMap_compileMap(map, 153, str_UIOptionHeld);
    cInputMap_compileMap(map, 154, str_UIMiscHeld);
    cInputMap_compileMap(map, 155, str_UINextReleased);
    cInputMap_compileMap(map, 156, str_UIPreviousReleased);
    cInputMap_compileMap(map, 157, str_UIOptionReleased);
    cInputMap_compileMap(map, 158, str_UIMiscReleased);
    cInputMap_compileMap(map, 159, str_ScriptMenuKey);
    cInputMap_compileMap(map, 160, str_ScriptEditHelp);
    cInputMap_compileMap(map, 161, str_ScriptEditHeadingDelta);
    cInputMap_compileMap(map, 162, str_ScriptEditPitchDelta);
    cInputMap_compileMap(map, 163, str_ScriptEditFOVDelta);
    cInputMap_compileMap(map, 164, str_ScriptEditRollDelta);
    cInputMap_compileMap(map, 165, str_ScriptEditJitterDelta);
    cInputMap_compileMap(map, 166, str_ScriptEditJitterRollDelta);
    cInputMap_compileMap(map, 167, str_ScriptEditManualForwardMove);
    cInputMap_compileMap(map, 168, str_ScriptEditManualSideMove);
    cInputMap_compileMap(map, 169, str_ScriptEditManualUpMove);
    cInputMap_compileMap(map, 170, str_ScriptEditTargetForwardMove);
    cInputMap_compileMap(map, 171, str_ScriptEditTargetSideMove);
    cInputMap_compileMap(map, 172, str_ScriptEditTargetUpMove);
    cInputMap_compileMap(map, 173, str_ScriptEditTargetCamDistDelta);
    cInputMap_compileMap(map, 174, str_ScriptEditSubjectLeadDistDelta);
    cInputMap_compileMap(map, 175, str_ScriptEditSubjectLeadHeightDelta);
    cInputMap_compileMap(map, 176, str_ScriptEditSubjectMoveCamForward);
    cInputMap_compileMap(map, 177, str_ScriptEditAlgoLeadDistDelta);
    cInputMap_compileMap(map, 178, str_ScriptEditAlgoLeadHeightDelta);
    cInputMap_compileMap(map, 179, str_ScriptEditAlgoCamDistDelta);
    cInputMap_compileMap(map, 180, str_ScreenCapture);
    cInputMap_compileMap(map, 181, str_VideoCapture);
    cInputMap_compileMap(map, 182, str_NetRiderAnalyzerToggle);
    cInputMap_compileMap(map, 183, str_MemCardKey);
    cInputMap_compileMap(map, 184, str_TestCameraFB);
    cInputMap_compileMap(map, 185, str_TestCameraLR);
    cInputMap_compileMap(map, 186, str_TestCameraOnOff);
    cInputMap_compileMap(map, 187, str_ChaseCamZoom);
    cInputMap_compileMap(map, 188, str_ChaseCamLR);
    cInputMap_compileMap(map, 189, str_ChaseCam);
    cInputMap_compileMap(map, 190, str_ChaseManualCamHeading);
    cInputMap_compileMap(map, 191, str_ChaseManualCamPitch);
    cInputMap_compileMap(map, 192, str_ChaseManualCamStrafe);
    cInputMap_compileMap(map, 193, str_ChaseManualCamForward);
    cInputMap_compileMap(map, 194, str_ChaseManualCamFast);
    cInputMap_compileMap(map, 195, str_TestTerrainType);
    cInputMap_compileMap(map, 196, str_MissionDebugPlay);
    cInputMap_compileMap(map, 197, str_MissionDebugPreview);
    cInputMap_compileMap(map, 198, str_MissionDebugRecord);
    cInputMap_compileMap(map, 199, str_MissionDebugAnyPressed);
    cInputMap_compileMap(map, 200, str_VoipOnOff);
    cInputMap_compileMap(map, 201, str_ConsoleReset);
    cInputMap_compileMap(map, 202, str_ReplayMenuUp);
    cInputMap_compileMap(map, 203, str_ReplayMenuDown);
    cInputMap_compileMap(map, 204, str_ReplayTimeLine);
    cInputMap_purgeMapFile(map);
    return (int)map;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_parseCommandLine);
#ifdef SKIP_ASM
extern "C" void* cBXString_cBXString2(cBXString* self, const char* str);
extern "C" void* cBXString_operatorE1(void* self, const char* str);
extern "C" cBXString* cBXString_cBXString5(cBXString* self, cBXString* other, int n);
extern "C" cBXString* cBXString_right(cBXString* self, cBXString* other, int n);
extern "C" void* cBXString_operatorE(void* self, void* other);
extern "C" void cBXString__cBXString(void* self, int flags);
int cBXString_reverseFind(cBXString* self, char ch);
int cBXString_find(cBXString* self, char ch, int len);
extern "C" int strcasecmp(const char* a, const char* b);
extern int cRenderToggles_m_DisableHUD;
extern char str_track[];
extern char str_realfe[];
extern char str_nohud[];
extern char str_username_b[];
extern char str_password[];
extern char str_autolobby[];

#define MAX_7BF0(x, y) ((x) > (y) ? (x) : (y))

/**
 * Reads the game's command-line options (each starts with '-'; names compare case-insensitively).
 *
 * -track NAME stores NAME without its directory and cut at its first '.' in CommandLineTrack (+0xCC);
 * -username and -password store the next argument in UserName and Password; -realfe sets unk_60 (already 1
 * from the constructor), -nohud clears unk_64 and -autolobby sets unk_74. An option missing its value ends
 * the scan.
 *
 * Finally cRenderToggles_m_DisableHUD is set to (unk_64 == 0), so -nohud sets it.
 *
 * @param argc  number of arguments; argv[0] is skipped
 * @param argv  the arguments
 */
// PORT: cBXString keeps its length in a header 8 bytes before the text (pointer held in int).
void cSSXApp::parseCommandLine(int argc, char** argv)
{
    int i = 1;
    while (i < argc) {
        if (argv[i][0] == '-') {
            char* opt = &argv[i][1];
            if (strcasecmp(opt, str_track) == 0) {
                i++;
                if (i >= argc) {
                    break;
                }
                cBXString track;
                cBXString_cBXString2(&track, argv[i]);
                int slash = cBXString_reverseFind(&track, '/');
                int backslash = cBXString_reverseFind(&track, '\\');
                int colon = cBXString_reverseFind(&track, ':');
                int sepPos = MAX_7BF0(slash, MAX_7BF0(backslash, colon));
                if (sepPos >= 0) {
                    cBXString trimmed;
                    cBXString_right(&trimmed, &track, track.getData()->DataLength - sepPos - 1);
                    cBXString_operatorE(&track, &trimmed);
                    cBXString__cBXString(&trimmed, 2);
                }
                int dot = cBXString_find(&track, '.', 0);
                if (dot >= 0) {
                    cBXString trimmed;
                    cBXString_cBXString5(&trimmed, &track, dot);
                    cBXString_operatorE(&track, &trimmed);
                    cBXString__cBXString(&trimmed, 2);
                }
                cBXString_operatorE(&CommandLineTrack, &track);
                cBXString__cBXString(&track, 2);
            } else if (strcasecmp(opt, str_username_b) == 0) {
                i++;
                if (i >= argc) {
                    break;
                }
                cBXString_operatorE1(&UserName, argv[i]);
            } else if (strcasecmp(opt, str_password) == 0) {
                i++;
                if (i >= argc) {
                    break;
                }
                cBXString_operatorE1(&Password, argv[i]);
            } else if (strcasecmp(opt, str_realfe) == 0) {
                UseRealFE = 1;
            } else if (strcasecmp(opt, str_nohud) == 0) {
                ShowHUD = 0;
            } else if (strcasecmp(opt, str_autolobby) == 0) {
                AutoLobby = 1;
            }
        }
        i++;
    }
    cRenderToggles_m_DisableHUD = ShowHUD == 0;
}
#endif

extern "C" int cPadCache_flush(void* mgr);
extern void* PadCache;

//99.9%
INCLUDE_ASM("main/ssxapp", cSSXApp_flush__Fv);
#ifdef SKIP_ASM
/**
 * Flushes the pad input cache, if one exists.
 *
 * @return the result of cPadCache_flush(), or 0 when there is no PadCache
 */
int cSSXApp_flush()
{
    if (PadCache != 0) {
        return cPadCache_flush(PadCache);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_preUpdate);
#ifdef SKIP_ASM
extern "C" bool cPadCache_nextInput(void* pad);
extern "C" void cCommSystem_update(void* p);
extern "C" void func_002668E8(void* p);
extern "C" int cPadCache_numButtons(void* self, int i);
extern "C" void* cPadCache_buttonValues(void* self, int i);
extern "C" void cInputPad_update(void* target, int a, void* data);
extern void* PadCache;
extern void* Keyboard;

/**
 * Starts a frame's input: when the pad cache has a new sample, updates the network and keyboard and feeds the
 * pads.
 *
 * Advances the pad cache's read position (cPadCache_nextInput); on success it updates cCommSystem_mCommSystem and
 * the keyboard object Keyboard when they exist, and passes each of the two sample slots (value count and
 * values) to InputPads[i] through cInputPad_update().
 *
 * @return true when a new input sample was taken, false when none was ready
 */
bool cSSXApp::preUpdate()
{
    if (PadCache != 0 && cPadCache_nextInput(PadCache) != 0) {
        if (cCommSystem_mCommSystem != 0) {
            cCommSystem_update(cCommSystem_mCommSystem);
        }
        if (Keyboard != 0) {
            func_002668E8(Keyboard);
        }
        int i;
        for (i = 0; i < 2; i++) {
            int valueCount = cPadCache_numButtons(PadCache, i);
            void* values = cPadCache_buttonValues(PadCache, i);
            cInputPad_update(InputPads[i], valueCount, values);
        }
        return true;
    }
    return false;
}
#endif

extern "C" void cPadCache_cacheInput(void* mgr);

//99.89%
INCLUDE_ASM("main/ssxapp", cSSXApp_timerCallback__Fv);
#ifdef SKIP_ASM
/**
 * Timer callback that caches the current pad input, if a pad cache exists.
 */
void cSSXApp_timerCallback()
{
    if (PadCache != 0) {
        cPadCache_cacheInput(PadCache);
    }
}
#endif

void* cMCOverlayManager_getManager();

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_getMCManager);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager.
 *
 * @return the result of cMCOverlayManager_getManager()
 */
extern "C" void* cSSXApp_getMCManager()
{
    return cMCOverlayManager_getManager();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_hasMCManager);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_hasManager();

/**
 * Asks whether the memory card overlay manager exists: calls cMCOverlayManager_hasManager, which tests cMCOverlayManager_instance.
 *
 * The source declares it void, but the callee's result stays in the return register.
 */
extern "C" void cSSXApp_hasMCManager(void) {
    cMCOverlayManager_hasManager();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_purge);
#ifdef SKIP_ASM
void operator_delete(int* p);
extern "C" void cUIFontInterface_purgeFonts(void* p);
extern "C" void cPadCache_purge(void* p);
extern "C" void cSSXScriptEngine_DeInitOnce(void* p);
extern "C" void FILESYS_delbigsync(int bighandle, int priority);
// PORT: WORLDTRIGGERMANAGER_DeInit is declared (void*) but never reads it; this caller passes nothing.
extern "C" void WORLDTRIGGERMANAGER_DeInit_noarg() __asm__("WORLDTRIGGERMANAGER_DeInit");
extern "C" void SSXAUDIO_DeInit();
extern "C" void cBE_purgeBE(void* p);
extern "C" void cGameModeMan_purgeGM(void* p);
extern "C" void cFEAsyncManager__cFEAsyncManager(void* p, int flags);
extern void* PadCache;

struct sVEPurge0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sVEPurge {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shuts the application down: frees what init and initload created and closes the big files.
 *
 * Deletes both InputMaps, the font interface, the pad cache, ScriptEngine and GrMan (after its slot 3);
 * closes the script, character and audio big files (handles +0xD4 to +0x118); deinits the world triggers and
 * audio; purges the BE and GameModeMan; deletes the cFEAsyncManager and ConsoleConfig; clears SSXApp.
 *
 * InputPads, Inputs, LoadIndicator and FELocale are not freed here.
 */
void cSSXApp::purge()
{
    int i;
    for (i = 0; i < 2; i++) {
        if (InputMaps[i]) {
            operator_delete((int*)InputMaps[i]);
        }
    }
    cUIFontInterface* fontInterface = Fonts;
    if (fontInterface) {
        cUIFontInterface_purgeFonts(fontInterface);
        operator_delete((int*)fontInterface);
    }
    int* mgr = (int*)PadCache;
    Fonts = 0;
    PadCache = 0;
    cPadCache_purge(mgr);
    operator_delete(mgr);
    if (ScriptEngine) {
        cSSXScriptEngine_DeInitOnce(ScriptEngine);
        cSSXScriptEngine* ssxScriptEngine = ScriptEngine;
        if (ssxScriptEngine) {  // delete ScriptEngine: the virtual destructor (slot 1) with flags 3
            sVEPurge* vt = (sVEPurge*)ssxScriptEngine->mVtbl;
            vt[1].fn(vthis(ssxScriptEngine, vt[1]), 3);
        }
        ScriptEngine = 0;
    }
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        ps2GraphicsMan->Purge();
    }
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        if (ps2GraphicsMan) {
            delete ps2GraphicsMan;
        }
    }
    GrMan = 0;
    if (mScriptDataBigFile) FILESYS_delbigsync(mScriptDataBigFile, 100);
    if (mModelBigFile) FILESYS_delbigsync(mModelBigFile, 100);
    for (i = 0; i < 11; i++) {
        if (mTextureBigFile[i]) {
            FILESYS_delbigsync(mTextureBigFile[i], 100);
        }
    }
    if (mRewardsBigFile) FILESYS_delbigsync(mRewardsBigFile, 100);
    WORLDTRIGGERMANAGER_DeInit_noarg();
    SSXAUDIO_DeInit();
    if (mAudioBigFile) FILESYS_delbigsync(mAudioBigFile, 100);
    if (mMusicBigFile) FILESYS_delbigsync(mMusicBigFile, 100);
    if (mMusic2BigFile) FILESYS_delbigsync(mMusic2BigFile, 100);
    if (mSpeechBigFile) FILESYS_delbigsync(mSpeechBigFile, 100);
    if (BE) cBE_purgeBE(BE);
    cGameModeMan_purgeGM(GameModeMan);
    if (mFEAsyncManager) cFEAsyncManager__cFEAsyncManager(mFEAsyncManager, 3);
    {
        cConsoleConfig* consoleConfig = ConsoleConfig;
        if (consoleConfig) {
            sVEPurge* vt = (sVEPurge*)consoleConfig->mVtbl;
            vt[1].fn(vthis(consoleConfig, vt[1]), 3);
        }
    }
    SSXApp = 0;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_startGameLoad);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cGame_cGame(void* self);
struct cAppMan;
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cGameModeMan_getGM();
extern char str_GameModule[];

/**
 * Starts the in-game module: creates the cGame and queues it as the application's next module.
 *
 * Allocates 0x22C bytes ("GameModule"), runs cGame_cGame(), stores the result in Game and refreshes
 * GameModeMan from cGameModeMan_getGM().
 */
void cSSXApp::startGameLoad()
{
    void* game = cGame_cGame(cMemMan_alloc(556, str_GameModule, 0, 0));
    *(void**)&Game = game;
    cAppMan_setNextModule((cAppMan*)this, (unsigned int)game);
    *(void**)&GameModeMan = cGameModeMan_getGM();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_startFELoad);
#ifdef SKIP_ASM
extern int cFEPopupConfirm_mUsePreFE;
extern char str_FEModule[];
extern "C" void* cRealFE_cRealFE(void* p);
extern "C" void* cDebugFE_cDebugFE(void* p);

/**
 * Starts the front-end module: creates the real or the debug front end and queues it as the next module.
 *
 * Clears cFEPopupConfirm_mUsePreFE, then builds the 0xB5AE0-byte real front end (RealFE) when unk_60 is set, as the
 * constructor leaves it, or else the 0x238-byte debug front end (DebugFE); both are tagged "FEModule".
 */
void cSSXApp::startFELoad()
{
    cFEPopupConfirm_mUsePreFE = 0;
    if (UseRealFE != 0) {
        void* frontEnd = cRealFE_cRealFE(cMemMan_alloc(0xB5AE0, str_FEModule, 0, 0));
        *(void**)&RealFE = frontEnd;
        cAppMan_setNextModule((cAppMan*)this, (unsigned int)frontEnd);
    } else {
        void* frontEnd = cDebugFE_cDebugFE(cMemMan_alloc(568, str_FEModule, 0, 0));
        *(void**)&DebugFE = frontEnd;
        cAppMan_setNextModule((cAppMan*)this, (unsigned int)frontEnd);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", cSSXApp_initload);
#ifdef SKIP_ASM
extern "C" void initOnline();
extern "C" int BXFILE_exists(const char* name);
extern "C" int FILESYS_addbigsync(const char* name, int a1, int a2, void* out);
extern "C" void func_00256188();
extern "C" void* cSSXScriptEngine_cSSXScriptEngine(void* self);
extern "C" void cSSXScriptEngine_InitOnce(void* self);
extern "C" int cSSXApp_loadInputMap(cSSXApp* self, const char* name);
void cInput_setPad(void* self, int a1);
void cInput_setMap(void* self, int a1);
int GetHashValue32(char* s);
extern "C" void cUIFontInterface_addFontEntry(void* self, unsigned int index, void* data, int type);
extern "C" void cUIFontInterface_loadFonts(void* self);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cSSXApp_activateBEOptions receives self here; the unit defines it later with no parameters.
extern "C" void cSSXApp_activateBEOptions_self(void* self) __asm__("cSSXApp_activateBEOptions");
extern "C" void cSSXApp_initLocale(cSSXApp* self);
extern "C" void cRiderAnimBase_initAIAnimEventMap();
extern "C" void* cGameAnimMap_cGameAnimMap(void* self);
extern "C" void cGameAnimMap_initMap(void* self);
extern "C" void* cAnimDataBase_cAnimDataBase(void* self);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" int cSSXAudio_BEAudioModeToAudioMode(void* self, unsigned char a1);
extern "C" void SSXAUDIO_Init(int a);
extern "C" void func_0031A6B8();
extern "C" void func_00287410(void* opt, int v);
extern "C" void cSSXAudio_SetSFXVolume(void* opt, int v);
extern "C" void func_00287520(void* opt, int v);
extern "C" void cSSXAudio_SetDJSpeech(void* opt, int v);
extern "C" void cSSXAudio_SetArcadeSFX(void* opt, int v);
void WORLDTRIGGERMANAGER_Init();
extern "C" int strcmp(const void* a, const void* b);

extern char str_data_audio_music_big[];
extern char str_data_audio_music2_big[];
extern char str_data_audio_audio_big[];
extern char str_data_audio_speech_big[];
extern char str_data_scripts_scdat_big[];
extern char* D_004A29C8;
extern char* D_004A29CC;
extern char* D_00442100[];
extern char str_ScriptEngine[];
extern char str_data_config_input_map[];
extern char str_data_config_input2_map[];
extern char str_Input[];
extern char str_cUIFontInterface[];
extern char str_fefont_b[];
extern char str_hudfont_b[];
extern char str_data_fonts_fefont_sfn[];
extern char str_data_fonts_hudfont_sfn[];
extern char str_GameAnimMap[];
extern char str_AnimDataBase[];
extern char D_004A29E8[];
extern int cBENewRaceInterface_commandLineTrack;

struct sSndCfg_82D0 {
    unsigned int sfxVol : 4;        // bits 0-3
    unsigned int musicVol : 4;      // bits 4-7
    unsigned int speechVol : 4;     // bits 8-11
    unsigned int output : 2;        // bits 12-13
    unsigned int pad14 : 2;
    int iUseArcadeSFX : 1;          // bit 16
    int iUseDJSpeech : 1;           // bit 17
    int pad18 : 14;
    char rest[0x288 - 4];
};
// PORT: cBEOptionInterface_mOption is viewed with different types across the program; view bound by asm label
extern sSndCfg_82D0 cBEOptionInterface_mOption_82D0 __asm__("cBEOptionInterface_mOption");

static inline sSndCfg_82D0 Cfg_82D0()
{
    return cBEOptionInterface_mOption_82D0;
}

struct sFontEntry_82D0 {
    int key;
    int unk4;
    int value;
    sFontEntry_82D0() : key(0), unk4(0), value(0) {}
};

struct sFontIface_82D0 {
    int unk0;
    sFontEntry_82D0 entries[4];
    signed char count;
    int f38;
    int f3C;
    int f40;
    sFontIface_82D0() : unk0(0) { count = 0; f38 = 0; f3C = 0; f40 = 0; }
};

struct sPlayerInput_82D0 {
    int a;
    int b;
    sPlayerInput_82D0(int x, int y) { cInput_setPad(this, x); cInput_setMap(this, y); }
};

struct sSSXApp_82D0 {
    char pad_0x00[0x78];
    void* f78;          // 0x78
    char pad_0x7C[0xC];
    sFontIface_82D0* f88; // 0x88
    char pad_0x8C[0x4];
    void* f90;          // 0x90
    void* f94;          // 0x94
    char pad_0x98[0x10];
    int fA8[2];         // 0xA8
    sPlayerInput_82D0* fB0[2]; // 0xB0
    int fB8[2];         // 0xB8
    char pad_0xC0[0xC];
    char* fCC;          // 0xCC
    char pad_0xD0[0x4];
    void* fD4;          // 0xD4
    void* fD8;          // 0xD8
    void* fDC;          // 0xDC
    void* fE0;          // 0xE0
    void* fE4;          // 0xE4
    void* fE8;          // 0xE8
    void* fEC[11];      // 0xEC
    void* f118;         // 0x118
};

/**
 * Runs the game's one-time load: network modules, big files, scripts, input, fonts, animation data and audio.
 *
 * Calls initOnline(); opens the music, music2, audio and speech big files that exist, then scdat.big,
 * mdlps2.big, the 11 character texture files and rwrdps2.big; creates the ScriptEngine; loads input.map and
 * input2.map into InputMaps and gives both players a cInput using InputMaps[0]; loads fefont and hudfont.
 *
 * Then applies the BE options and the locale, builds GameAnimMap and AnimDataBase, starts audio with the
 * saved volumes, inits the world triggers, and copies a non-empty CommandLineTrack into the string
 * cBENewRaceInterface_commandLineTrack.
 */
extern "C" void cSSXApp_initload(sSSXApp_82D0* self)
{
    initOnline();
    if (BXFILE_exists(str_data_audio_music_big)) FILESYS_addbigsync(str_data_audio_music_big, 0, 100, &self->fD8);
    if (BXFILE_exists(str_data_audio_music2_big)) FILESYS_addbigsync(str_data_audio_music2_big, 0, 100, &self->fDC);
    if (BXFILE_exists(str_data_audio_audio_big)) FILESYS_addbigsync(str_data_audio_audio_big, 0, 100, &self->fD4);
    if (BXFILE_exists(str_data_audio_speech_big)) FILESYS_addbigsync(str_data_audio_speech_big, 0, 100, &self->fE0);
    func_00256188();
    FILESYS_addbigsync(str_data_scripts_scdat_big, 0, 100, &self->fE4);
    FILESYS_addbigsync(D_004A29C8, 0, 100, &self->fE8);
    for (int i = 0; i < 11; i++) {
        FILESYS_addbigsync(D_00442100[i], 0, 100, &self->fEC[i]);
    }
    FILESYS_addbigsync(D_004A29CC, 0, 100, &self->f118);
    cSSXScriptEngine* sm = (cSSXScriptEngine*)cSSXScriptEngine_cSSXScriptEngine(cMemMan_alloc(2640, str_ScriptEngine, 0, 0));
    ScriptEngine = sm;
    cSSXScriptEngine_InitOnce(sm);
    self->fB8[0] = ((cSSXApp*)self)->loadInputMap(str_data_config_input_map);
    self->fB8[1] = ((cSSXApp*)self)->loadInputMap(str_data_config_input2_map);
    for (int j = 0; j < 2; j++) {
        self->fB0[j] = new (str_Input, 0, 0) sPlayerInput_82D0(self->fA8[j], self->fB8[0]);
    }
    sFontIface_82D0*& fonts = self->f88;
    fonts = new (str_cUIFontInterface, 0, 0) sFontIface_82D0;
    cUIFontInterface_addFontEntry(self->f88, 0, str_data_fonts_fefont_sfn, GetHashValue32(str_fefont_b));
    cUIFontInterface_addFontEntry(self->f88, 1, str_data_fonts_hudfont_sfn, GetHashValue32(str_hudfont_b));
    cUIFontInterface_loadFonts(self->f88);
    self->f78 = cBE_getBE();
    cSSXApp_activateBEOptions_self(self);
    ((cSSXApp*)self)->initLocale();
    cRiderAnimBase_initAIAnimEventMap();
    self->f94 = cGameAnimMap_cGameAnimMap(cMemMan_alloc(6216, str_GameAnimMap, 0, 0));
    cGameAnimMap_initMap(self->f94);
    self->f90 = cAnimDataBase_cAnimDataBase(cMemMan_alloc(0x3FC, str_AnimDataBase, 0, 0));
    cBE_getInterface_Fv(cBE_getBE(), 4);
    SSXAUDIO_Init(cSSXAudio_BEAudioModeToAudioMode(cAudioInterface_bxAudio(), cBEOptionInterface_mOption_82D0.output));
    func_0031A6B8();
    func_00287410(cAudioInterface_bxAudio(), Cfg_82D0().sfxVol);
    cSSXAudio_SetSFXVolume(cAudioInterface_bxAudio(), Cfg_82D0().speechVol);
    func_00287520(cAudioInterface_bxAudio(), Cfg_82D0().musicVol);
    cSSXAudio_SetDJSpeech(cAudioInterface_bxAudio(), Cfg_82D0().iUseDJSpeech != 0);
    cSSXAudio_SetArcadeSFX(cAudioInterface_bxAudio(), Cfg_82D0().iUseArcadeSFX != 0);
    WORLDTRIGGERMANAGER_Init();
    if (strcmp(self->fCC, D_004A29E8)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        cBXString_operatorE1(&cBENewRaceInterface_commandLineTrack, self->fCC);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_initLocale);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cFELocale_cFELocale(void* self);
extern "C" void cFELocale_loadCommon(void* self);
extern char str_FELocale[];

struct sOptions_228A78 {
    unsigned int pad0 : 22;
    unsigned int language : 3;
    unsigned int pad25 : 7;
    int data[0x284 / 4];
};
extern sOptions_228A78 cBEOptionInterface_mOption_228A78 __asm__("cBEOptionInterface_mOption");


static inline int optLanguage_228A78(sOptions_228A78 o)
{
    return o.language;
}

/**
 * Creates the front-end locale if needed and sets it to the language chosen in the options.
 *
 * Allocates the 0xC0-byte "FELocale" on first use, passes the options' language field (bits 22-24 of
 * cBEOptionInterface_mOption) to its setLanguage (vtable slot 3), then calls cFELocale_loadCommon() on it. BE interface
 * 4 is fetched and unused.
 */
void cSSXApp::initLocale()
{
    if (FELocale == 0) {
        *(void**)&FELocale = cFELocale_cFELocale(cMemMan_alloc(0xC0, str_FELocale, 0, 0));
    }
    cBE_getInterface_Fv(cBE_getBE(), 4);
    cFELocale* loc = FELocale;
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)VPTR_OF(cUILocaleInterface, loc);
    void* thisp = vthis(loc, vt[3]); // slot 3: setLanguage
    void (**pfn)(void*, int) = &vt[3].fn;
    (*pfn)(thisp, optLanguage_228A78(cBEOptionInterface_mOption_228A78));
    cFELocale_loadCommon(FELocale);
}
#endif

extern "C" void WORLDTRIGGERMANAGER_DeInit(void* self);
extern "C" void SSXAUDIO_DeInit();
extern "C" void cAppMan_loadexecpurge(void* self);


//99.95%
INCLUDE_ASM("main/ssxapp", cSSXApp_loadexecpurge__FPv);
#ifdef SKIP_ASM
/**
 * Shuts down the world triggers, audio, the current module and the graphics, probably before a LoadExecPS2.
 *
 * Calls WORLDTRIGGERMANAGER_DeInit(), SSXAUDIO_DeInit() and cAppMan_loadexecpurge() (the current module's
 * slot 3, then its deletion), then the graphics manager's loadexecpurge (vtable slot 119).
 */
void cSSXApp::loadexecpurge()
{
    WORLDTRIGGERMANAGER_DeInit(this);
    SSXAUDIO_DeInit();
    cAppMan_loadexecpurge(this);
    cGraphicsMan_TheGraphicsMan->loadexecpurge();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cSSXApp_activateBEOptions);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEOptionDB cBEOptionInterface_mOption;

// The option block by value, as be/bedbreward's getOptions returns it (retail copies all 0x288 bytes per read).
static inline tBEOptionDB GetOptions_228C08()
{
    return cBEOptionInterface_mOption;
}

/**
 * Applies the saved display options to the graphics manager: the TV display format and the screen offset.
 *
 * Passes the options' 2-bit display format (iUseWidescreen of cBEOptionInterface_mOption; values above 2 count as 0) to
 * GrMan's slot 40 (0 normal, 1 letterbox, 2 widescreen, by the scales cPS2GraphicsMan_SetTVDisplayFormat sets), then
 * iHzDelta and iVtDelta to slot 12 (SetScreenOffset), the screen x and y offset. The C takes no arguments; initload
 * passes the app anyway.
 */
extern "C" void cSSXApp_activateBEOptions()
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    switch (GetOptions_228C08().iUseWidescreen) {
    case kWidescreen_Off:
        cGraphicsMan_TheGraphicsMan->SetTVDisplayFormat(GM_FULLSCREEN);
        break;
    case kWidescreen_16x9:
        cGraphicsMan_TheGraphicsMan->SetTVDisplayFormat(GM_WIDESCREEN);
        break;
    case kWidescreen_Anamorphic:
        cGraphicsMan_TheGraphicsMan->SetTVDisplayFormat(GM_ANIMORPHIC);
        break;
    default:
        cGraphicsMan_TheGraphicsMan->SetTVDisplayFormat(GM_FULLSCREEN);
        break;
    }
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    sVEntry<void(void*, int, int)>* vt = (sVEntry<void(void*, int, int)>*)VPTR_OF(cGraphicsMan, ps2GraphicsMan);
    void* thisp = vthis(ps2GraphicsMan, vt[12]); // slot 12: SetScreenOffset
    void (**pfn)(void*, int, int) = &vt[12].fn;
    (*pfn)(thisp, GetOptions_228C08().iHzDelta, GetOptions_228C08().iVtDelta);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", initOnline);
#ifdef SKIP_ASM
extern int D_004A3E94;
extern int EnableOnline;
extern char str_cdrom0[];
extern char str_1[];
extern char str_PPP_IRX[];
extern char str_USBD_IRX[];
extern char str_hub_1[];
extern char str_USBKB_IRX[];
extern char str_keybd_2[];
extern char str_DEV9_IRX[];
extern char str_INET_IRX[];
extern char str_mem_96KB[];
extern char str_icon_pct_sCNF_SYS_NET_ICOpct_spct_cicons[];
extern char str_NETCNF_IRX[];
extern char str_INETCTL_IRX[];
extern char str_no_auto[];
extern char str_SMAP_IRX[];
extern char str_dial_pct_sCNF_DIAL_SPD_CNFpct_spct_c_nog[];
extern char str_SPDUART_IRX[];
extern char str_PPPOE_IRX[];
extern char str_LIBNET_IRX[];
extern char str_DRTYSCKF_IRX[];
extern char str_LGAUD_IRX[];
extern char str_rpc_0[];
extern char str_VOIPF_IRX[];
extern "C" int func_00319718(int loader, const char* name, int flags, int argLen, const char* args);
extern "C" void func_00319930(int loader);
extern "C" void func_003197D0(int loader);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void THREAD_yieldticks(int ticks);
extern "C" void func_0040B130(int a);
extern "C" void func_003F6A40();
extern "C" void func_003F6AB0();
extern "C" void func_00266788();

/**
 * Loads the IOP modules for USB, the keyboard, the network adapter, the modem and voice chat.
 *
 * Loads USBD, USBKB, DEV9, INET, NETCNF (with icon paths), INETCTL, SMAP and PPP through func_00319718.
 * EnableOnline (network hardware present) is set when SMAP (network adapter) or SPDUART (modem) loads, and
 * PPPOE is loaded only after SMAP.
 *
 * With network hardware it then loads MSIFRPC, LIBNET, DRTYSCKF (DirtySock) and LGAUD; when VOIPF loads it
 * binds a SIF RPC client, probably VOIPF's (func_003F6A40), else marks it unbound (func_003F6AB0).
 */
extern "C" void initOnline()
{
    char buf[0x100];
    func_00319718(D_004A3E94, str_USBD_IRX, 0, 43, str_hub_1);
    int ok = 0;
    func_00319718(D_004A3E94, str_USBKB_IRX, 0, 0x10, str_keybd_2);
    func_00319718(D_004A3E94, str_DEV9_IRX, 0, 0, 0);
    func_00319930(D_004A3E94);
    func_00319718(D_004A3E94, str_INET_IRX, 1, 18, str_mem_96KB);
    int n = sprintf(buf, str_icon_pct_sCNF_SYS_NET_ICOpct_spct_cicons, str_cdrom0, str_1, 0, str_cdrom0, str_1);
    func_00319718(D_004A3E94, str_NETCNF_IRX, 1, n + 1, buf);
    func_00319718(D_004A3E94, str_INETCTL_IRX, 1, 20, str_no_auto);
    EnableOnline = 0;
    if (func_00319718(D_004A3E94, str_SMAP_IRX, 0, 0, 0) >= 0) {
        EnableOnline = 1;
        ok = 1;
    }
    func_00319718(D_004A3E94, str_PPP_IRX, 1, 0, 0);
    n = sprintf(buf, str_dial_pct_sCNF_DIAL_SPD_CNFpct_spct_c_nog, str_cdrom0, str_1, 0);
    if (func_00319718(D_004A3E94, str_SPDUART_IRX, 0, n + 1, buf) >= 0) {
        EnableOnline = 1;
    }
    if (EnableOnline && ok) {
        func_00319718(D_004A3E94, str_PPPOE_IRX, 1, 0, 0);
    }
    THREAD_yieldticks(10);
    if (EnableOnline) {
        func_003197D0(D_004A3E94);
        func_00319718(D_004A3E94, str_LIBNET_IRX, 1, 0, 0);
        func_0040B130(0);
        func_00319718(D_004A3E94, str_DRTYSCKF_IRX, 1, 0, 0);
        func_00319718(D_004A3E94, str_LGAUD_IRX, 0, 18, str_rpc_0);
        if (func_00319718(D_004A3E94, str_VOIPF_IRX, 0, 0, 0) >= 0) {
            func_003F6A40();
        } else {
            func_003F6AB0();
        }
    }
    func_00266788();
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cCrowdMan2d_cCrowdMan2d);
#ifdef SKIP_ASM
extern "C" void func_002292E0(cCrowdMan2d* self);
// PORT: the ctor's mangled name says int; it receives the allocated block (pointer in int)
void* cCrowdRender2D_cCrowdRender2D(int mem);
void* cCrowdRender2D_constructCrowdAnim2D(void* r);
extern char str_cCrowdRender2D[];
extern int TheCrowdMan2d;

inline void* operator new[](unsigned int, void* p) { return p; }

struct sCrowdCfg_229180 {
    short f0;
    short f2;
    short f4;
    short f6;
    int f8;
    int fC;
    int f10;
};
extern sCrowdCfg_229180 D_00536690_229180[] __asm__("D_00536690");

struct sCrowdId_229180 {
    unsigned int id;
    sCrowdId_229180() : id(0xFFFFFFFF) {}
};

struct sCrowdSlot_229180 {
    char data[0x40];
    sCrowdSlot_229180() {}
};

struct sCrowdRest_229180 {
    char data[0x20];
    sCrowdRest_229180() {}
};

static inline int AnimCount_229180(char* anim)
{
    return *(int*)(anim + 0x10);
}


/**
 * Constructs the 2D crowd manager: empty quad slots, its crowd renderer and animation, and the shared crowd record.
 *
 * Builds 128 resource ids (+0x8) set to 0xFFFFFFFF, 128 0x40-byte quad slots (+0x210) and the 40 0x20-byte marker ring
 * entries (+0x2220); makes itself the singleton TheCrowdMan2d. Creates a cCrowdRender2D (0x10 bytes, tag "cCrowdRender2D")
 * at +0x0 and its animation (cCrowdRender2D_constructCrowdAnim2D) at +0x4. Fills the shared record D_00536690: the
 * first short takes the animation's current frame texture id (+0x10), +0xC 0x20000, the shorts at +0x2, +0x4 and +0x6
 * -1, +0x8 and +0x10 0. Then resets the slots (func_002292E0). cGame_load builds it as CrowdMan2d (10016 bytes).
 *
 * @return @p self
 */
extern "C" void* cCrowdMan2d_cCrowdMan2d(cCrowdMan2d* self)
{
    new ((char*)&self->mQuadId[0]) sCrowdId_229180[128];
    new ((char*)&self->mQuad[0]) sCrowdSlot_229180[128];
    new ((char*)&self->mMarker[0]) sCrowdRest_229180[40];
    // PORT: the singleton pointer is held in an int global
    TheCrowdMan2d = (int)self;
    self->mpRender = (cCrowdRender2D*)cCrowdRender2D_cCrowdRender2D((int)cMemMan_alloc(0x10, str_cCrowdRender2D, 0x20000000, 0));
    char* anim = (char*)cCrowdRender2D_constructCrowdAnim2D(self->mpRender);
    self->mpAnim = (cCrowdAnim2D*)anim;
    D_00536690_229180[0].f0 = AnimCount_229180(anim);
    D_00536690_229180[0].fC = 0x20000;
    D_00536690_229180[0].f10 = 0;
    D_00536690_229180[0].f2 = -1;
    D_00536690_229180[0].f4 = -1;
    D_00536690_229180[0].f8 = 0;
    D_00536690_229180[0].f6 = -1;
    func_002292E0(self);
    return (self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", cCrowdMan2d__cCrowdMan2d);
#ifdef SKIP_ASM
extern "C" void func_00229398(cCrowdMan2d* self);
extern int TheCrowdMan2d;
void operator_delete(int* p);
void cCrowdRender2D__cCrowdRender2D(int* self, int flags);

/**
 * Destroys the 2D crowd manager: releases every slot, clears the singleton TheCrowdMan2d, frees the animation (+0x4),
 * destroys the renderer (+0x0) and frees the object when bit 0 of @p flags is set.
 *
 * Uses func_00229398, operator_delete and cCrowdRender2D__cCrowdRender2D(renderer, 3). cGame_purge calls it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cCrowdMan2d__cCrowdMan2d(cCrowdMan2d* self, int flags)
{
    func_00229398(self);
    int* crowdAnim = (int*)self->mpAnim;
    TheCrowdMan2d = 0;
    operator_delete(crowdAnim);
    int* renderer = *(int**)self;
    if (renderer != 0) {
        cCrowdRender2D__cCrowdRender2D(renderer, 3);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002292E0);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
unsigned int BXrand();



/**
 * Resets the 2D crowd manager's slots and markers.
 *
 * Clears the crowd level (+0x2210) and the 128 quad slots (+0x210, 0x2000 bytes); sets every resource id (+0x8) to
 * 0xFFFFFFFF and every slot's delay (+0x30) to 300 + BXrand() % 300; clears the marker ring index (+0x2214) and its 40
 * entries (+0x2220, 1280 bytes).
 */
extern "C" void func_002292E0(cCrowdMan2d* self)
{
    cCrowdMan2d* crowd = self;
    int i;
    crowd->mCheerLevel = 0;
    MEM_fill(((&crowd->mQuad[0])), 0, 0x2000);
    for (i = 0; i < 128; i++) {
        crowd->mQuadId[i] = 0xFFFFFFFF;
        ((&crowd->mQuad[0]))[i].mDelay = BXrand() % 300 + 300;
    }
    crowd->mMarkerIndex = 0;
    MEM_fill(((char*)&crowd->mMarker[0]), 0, 1280);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229398);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void func_00229B90(cCrowdMan2d* self, int a1);

/**
 * Releases every used quad slot of the 2D crowd manager (each resource id at +0x8 other than 0xFFFFFFFF) with
 * func_00229B90.
 */
extern "C" void func_00229398(cCrowdMan2d* self)
{
    int i;
    for (i = 0; i < 0x80; i++) {
        unsigned* slotId = (unsigned*)(&self->mQuadId[0]) + i;
        if (*slotId != cCrowdMan2d::kFreeQuad) {
            func_00229B90(self, i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229408);
#ifdef SKIP_ASM
extern "C" void func_00229B90(cCrowdMan2d* self, int a1);

/**
 * Releases the 2D crowd manager's quad slots in one world section: each used slot whose resource id has @p id as its
 * low (section) byte.
 *
 * cGame_sectionUnloading calls it through the singleton TheCrowdMan2d.
 *
 * @param id  the world section id
 */
extern "C" void func_00229408(cCrowdMan2d* self, int id)
{
    int i;
    for (i = 0; i < 0x80; i++) {
        unsigned* slotId = (unsigned*)(&self->mQuadId[0]) + i;
        if (*slotId != cCrowdMan2d::kFreeQuad && *(unsigned char*)slotId == id) {
            func_00229B90(self, i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229498);
#ifdef SKIP_ASM
extern "C" void func_00229398(cCrowdMan2d* self);
extern "C" void func_002292E0(cCrowdMan2d* self);

/**
 * Restarts the 2D crowd manager: releases every slot (func_00229398) and resets slots and markers (func_002292E0).
 *
 * cGame_partialRestart calls it.
 */
extern "C" void func_00229498(cCrowdMan2d* self)
{
    func_00229398(self);
    func_002292E0(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002294C8);
#ifdef SKIP_ASM
extern "C" void cCrowdAnim2D_update(void* p);
extern "C" void func_00229530(cCrowdMan2d* self);
extern short D_00536690[];

/**
 * Updates the 2D crowd manager once per frame: the crowd animation, the shared frame, the marker lifetimes, then the
 * marker spawns.
 *
 * Calls cCrowdAnim2D_update on the animation (+0x4) and copies its current frame texture id (+0x10) into the first
 * short of D_00536690; counts down each live marker of the 40-entry ring (+0x2230 in each 0x20-byte entry); then calls
 * func_00229530. cGame_update calls it.
 */
extern "C" void func_002294C8(cCrowdMan2d* self)
{
    int i;
    char* marker;
    int frameTexId;
    cCrowdAnim2D_update(self->mpAnim);
    frameTexId = *(int*)((char*)self->mpAnim + 0x10);
    D_00536690[0] = frameTexId;
    marker = (char*)self;
    for (i = 39; i >= 0; i--) {
        if (*(int*)(marker + 0x2230) > 0) {
            *(int*)(marker + 0x2230) = *(int*)(marker + 0x2230) - 1;
        }
        marker += 0x20;
    }
    func_00229530(self);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229530);
#ifdef SKIP_ASM
unsigned int BXrand();
extern "C" int func_002A77C8(void* self, int a1);
struct sRingOwner;
struct sQuad229;
extern "C" void func_00229738(sRingOwner* self, sQuad229* q, int value);


struct sCrowdSlot_229530 {
    t4Vector pos;       // 0x00
    t4Vector dx;        // 0x10
    t4Vector dy;        // 0x20
    int delay;              // 0x30
    char pad_0x34[0xC];
};

struct sCrowd_229530 {
    int pad0;
    int* anim;                          // 0x4
    unsigned int ids[128];              // 0x8
    char pad_0x208[0x8];
    sCrowdSlot_229530 slots[128];       // 0x210
    int count;                          // 0x2210
};

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_229530(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_229530(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline float randf_229530()
{
    union { int i; float f; } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float rands_229530(float lo, float hi)
{
    return randf_229530() * (hi - lo) + lo;
}

/**
 * Spawns markers over the crowd quads at a rate that follows the crowd's cheer level.
 *
 * Takes the absolute value of func_002A77C8(gSSXAudio, 0) (crowd instance 0's cheer value); when it changes, stores it
 * as the crowd level (+0x2210) and sets the animation's target state (+0x0) to it, at least 1. Each used slot's delay
 * (+0x30) falls by 10 * level + 4; when it reaches 0, a marker goes into the ring (func_00229738, lifetime 3) at the
 * slot's centre (+0x0) plus its two half-axes (+0x10, +0x20) each scaled by a random value in [-1, 1), and the delay
 * restarts at 300 + BXrand() % 300.
 *
 * @param selfp  the 2D crowd manager
 */
extern "C" void func_00229530(cCrowdMan2d* selfp)
{
    sCrowd_229530* self = (sCrowd_229530*)selfp;
    int n = func_002A77C8(gSSXAudio, 0);
    n = n > -1 ? n : -n;
    if (n != self->count) {
        int animCount = n > 0 ? n : 1;
        self->count = n;
        *self->anim = animCount;
    }
    int dec = self->count * 10 + 4;
    for (int i = 0; i < 128; i++) {
        if (self->ids[i] != cCrowdMan2d::kFreeQuad) {
            self->slots[i].delay -= dec;
            if (self->slots[i].delay <= 0) {
                float dxScale = rands_229530(-1.0f, 1.0f);
                float dyScale = rands_229530(-1.0f, 1.0f);
                t4Vector spawnPos = Add_229530(Add_229530(self->slots[i].pos, Scale_229530(self->slots[i].dx, dxScale)),
                                            Scale_229530(self->slots[i].dy, dyScale));
                func_00229738((sRingOwner*)self, (sQuad229*)&spawnPos, 3);
                self->slots[i].delay = BXrand() % 300 + 300;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229738);
#ifdef SKIP_ASM
struct sQuad229 {
    int v[4];
} __attribute__((aligned(16)));

struct sRingEntry {
    sQuad229 q;     // 0x0
    int value;      // 0x10
    int pad[3];
};

struct sRingOwner {
    char pad_0x0[0x2214];
    int index;              // 0x2214
    int pad_0x2218[2];
    sRingEntry entries[40]; // 0x2220
};

/**
 * Adds a marker to the 2D crowd manager's 40-entry ring: stores @p pos and @p value in the entry at the ring index
 * (+0x2214), then advances the index modulo 40.
 *
 * The oldest marker is overwritten when the ring is full.
 *
 * @param pos  the marker position (a 16-byte vector)
 * @param value  the marker's lifetime in updates
 */
extern "C" void func_00229738(sRingOwner* self, sQuad229* pos, int value)
{
    self->entries[self->index].q = *pos;
    self->entries[self->index].value = value;
    self->index++;
    self->index %= 40;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229788);
#ifdef SKIP_ASM
extern "C" void func_00229910(void* self, int slot, int a1);

/**
 * Takes the first free quad slot among the first 64 and places it on the model of @p resourceId (func_00229910).
 *
 * Does nothing when those 64 are all used. Crowd2dFunc (a script command) calls it for its command 2.
 *
 * @param resourceId  the world resource id of the crowd model
 */
extern "C" void func_00229788(cCrowdMan2d* self, int resourceId)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((unsigned int*)(&self->mQuadId[0]))[i] == cCrowdMan2d::kFreeQuad) {
            func_00229910(self, i, resourceId);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_002297D8);
#ifdef SKIP_ASM
extern "C" void func_00229B90(cCrowdMan2d* self, int a1);

/**
 * Releases the first quad slot among the first 64 whose resource id equals @p val (func_00229B90).
 *
 * Crowd2dFunc (a script command) calls it for its command 3.
 *
 * @param val  the world resource id
 */
extern "C" void func_002297D8(cCrowdMan2d* self, int val)
{
    int i;
    for (i = 0; i < 0x40; i++) {
        if (((int*)self)[i + 2] == val) {
            func_00229B90(self, i);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229820);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sHTable_229820 {
    char pad[0x1C];
    unsigned int* entries;      // 0x1C
};

struct sHMgr_229820 {
    char pad[0x8];
    sHTable_229820** tables;    // 0x8
};

extern "C" sHMgr_229820** cObjectInterface_getWorld(void);




struct sModel_229820 {
    char pad[0x80];
    tModelRecord* skel;         // 0x80
    char pad84[0x94 - 0x84];
    struct {
        int f0;
        void* bones[1];
    }* pose;                    // 0x94
};

extern char D_00536690_229820[] __asm__("D_00536690");

static inline sModel_229820* refToPtr_229820(unsigned int p)
{
    return (sModel_229820*)(p << 2);
}

static inline sModel_229820* lookup_229820(sHTable_229820* t, unsigned int idx)
{
    unsigned int p = t->entries[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_229820(p);
}

static inline sModel_229820* HandleToModel_229820(unsigned int h)
{
    sHTable_229820* t = (*cObjectInterface_getWorld())->tables[h & 0xFF];
    if (t == 0) {
        return 0;
    }
    return lookup_229820(t, h >> 8);
}

/**
 * Points one bone of a model's pose at the shared crowd record D_00536690.
 *
 * Finds the model behind @p handle in the world cache (cObjectInterface_getWorld: table by the low byte, entry by the
 * rest); when part @p partIndex exists in its skeleton (+0x80) and has a bone @p boneIndex, stores D_00536690 in the pose (+0x94) entry
 * of that bone. @p self is not used. Crowd2dFunc (a script command) calls it for its command 0.
 *
 * @param handle  the model's resource handle
 * @param partIndex  the skeleton part index
 * @param boneIndex  the bone index within the part
 */
// Points bone b of part a of the model behind handle at the shared crowd pose.
extern "C" void func_00229820(cCrowdMan2d* self, unsigned int handle, int partIndex, int boneIndex)
{
    sModel_229820* model = HandleToModel_229820(handle);
    if (model != 0) {
        tModelRecord* skeleton = model->skel;
        void** dst = model->pose->bones;
        tMeshHeirarchyRecord* parts = (skeleton->m_MeshHeirarchy);
        if (partIndex < skeleton->m_iNumMeshes) {
            tMeshRecord* skelPart = (parts[partIndex].m_Mesh);
            if (boneIndex < skelPart->m_iNumContexts)
                dst[*((short*)skelPart->m_pContexts[boneIndex])] = D_00536690_229820;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229910);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV3_229910 {
    float x, y, z;
    sV3_229910() {}
};

struct sV4_229910 {
    float x, y, z, w;
    sV4_229910() {}
    sV4_229910(const sV3_229910& v, const float& d) : x(v.x), y(v.y), z(v.z), w(d) {}
} __attribute__((aligned(16)));

struct sV2_229910 {
    float u, v;
    sV2_229910() {}
};

struct sHTable_229910 {
    char pad[0x1C];
    unsigned int* entries;      // 0x1C
};

struct sHMgr_229910 {
    char pad[0x8];
    sHTable_229910** tables;    // 0x8
};


struct sModel_229910 {
    char pad[0x10];
    sV4_229910 mat[4];          // 0x10
};

struct sVEntry_229910 {
    short delta;
    short index;
    void (*fn)(void*, sModel_229910*, int, sV3_229910*, sV2_229910*, sV4_229910*);
};

struct sGfx_229910 {
    char pad[0x10D8];
    sVEntry_229910* vt;         // 0x10D8
};


static inline sModel_229910* refToPtr_229910(unsigned int p)
{
    return (sModel_229910*)(p << 2);
}

static inline sModel_229910* lookup_229910(sHTable_229910* t, unsigned int idx)
{
    unsigned int p = t->entries[idx] >> 8;
    if (p == 0) {
        return 0;
    }
    return refToPtr_229910(p);
}

struct tResourceID_229910 {
    struct {
        unsigned int m_uiSectionID : 8;
        unsigned int m_uiUID : 24;
    } m_bfRID;
};

static inline sModel_229910* HandleToModel_229910(tResourceID_229910 RID)
{
    sHTable_229910* t = ((sHMgr_229910*)cWorld_m_pTheWorld->m_pWorldCache)->tables[RID.m_bfRID.m_uiSectionID];
    if (t == 0) {
        return 0;
    }
    return lookup_229910(t, RID.m_bfRID.m_uiUID);
}

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sV4_229910 MulMV_229910(const sV4_229910* m, const sV4_229910& v)
{
    sV4_229910 r;
    __asm__(
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_229910 Sub_229910(const sV4_229910& a, const sV4_229910& b)
{
    sV4_229910 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV4_229910 Add_229910(const sV4_229910& a, const sV4_229910& b)
{
    sV4_229910 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddTo_229910(sV4_229910& a, const sV4_229910& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sV4_229910 Scale_229910(const sV4_229910& v, float s)
{
    sV4_229910 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

struct sCrowdSlot_229910 {
    sV4_229910 pos;             // 0x00
    sV4_229910 dx;              // 0x10
    sV4_229910 dy;              // 0x20
    int delay;                  // 0x30
    char pad_0x34[0xC];
};


// PORT: callers declare func_00229910 as (void*, int, int); defined under an asm label.
extern "C" void func_00229910_r(cCrowdMan2d* self, int index, tResourceID_229910 RID) __asm__("func_00229910");

/**
 * Places quad slot @p index on the crowd model of @p RID: keeps the id and derives the slot's centre and half-axes from
 * the model's quad.
 *
 * Stores @p RID in the slot's id (+0x8). Gets the model's four corners (with texture coordinates and colours) from
 * GrMan's GetMeshQuad (slot 97) and transforms them by the model's matrix (+0x10). The centre (+0x0) is corner 0 plus
 * half of (corner 3 - corner 0), raised by 150 in z; the half-axes are 0.45 * (corner 2 - corner 0) (+0x10) and 0.45 *
 * (corner 1 - corner 0) (+0x20). The model must exist: the lookup result is not checked.
 *
 * @param index  the quad slot (0-127)
 * @param RID  the world resource id of the model
 */
// Places crowd quad index on the model behind RID: corner points from the renderer, then base/width/height.
extern "C" void func_00229910_r(cCrowdMan2d* self, int index, tResourceID_229910 RID)
{
    ((tResourceID_229910*)&self->mQuadId[0])[index] = RID;
    sV4_229910 pts[4];
    sV3_229910 pos[4];
    sV2_229910 uv[4];
    sV4_229910 col[4];
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    sModel_229910* model = HandleToModel_229910(RID);
    ps2GraphicsMan->GetMeshQuad((const tInstanceRecord*)model, 0, (t3Vector*)pos, (t2Vector*)uv, (tARGB*)col);
    for (int i = 0; i < 4; i++) {
        pts[i] = MulMV_229910(model->mat, sV4_229910(pos[i], 1.0f));
    }
    ((sCrowdSlot_229910*)&self->mQuad[0])[index].pos = Add_229910(pts[0], Scale_229910(Sub_229910(pts[3], pts[0]), 0.5f));
    ((sCrowdSlot_229910*)&self->mQuad[0])[index].dx = Scale_229910(Sub_229910(pts[2], pts[0]), 0.45f);
    ((sCrowdSlot_229910*)&self->mQuad[0])[index].dy = Scale_229910(Sub_229910(pts[1], pts[0]), 0.45f);
    sV4_229910 up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 150.0f;
    up.w = 0.0f;
    AddTo_229910(((sCrowdSlot_229910*)&self->mQuad[0])[index].pos, up);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229B90);
#ifdef SKIP_ASM
/**
 * Frees quad slot @p slotIndex of the 2D crowd manager: sets its resource id (+0x8) to 0xFFFFFFFF.
 *
 * @param slotIndex  the slot (0-127)
 */
extern "C" void func_00229B90(cCrowdMan2d* self, int slotIndex)
{
    ((unsigned int*)self)[slotIndex + 2] = 0xFFFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229BA8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV2_229BA8 {
    float x, y;
    sV2_229BA8() : x(0.0f), y(0.0f) {}
    sV2_229BA8(const float& ax, const float& ay) : x(ax), y(ay) {}
};

struct sV4_229BA8 {
    float x, y, z, w;
    sV4_229BA8() {}
    sV4_229BA8(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};


extern sDrawState sDrawState_s_defaultDrawState_229BA8 __asm__("sDrawState_s_defaultDrawState");

class cGfx_229BA8 {
public:
    char pad_0x0[0xE84];
    sDrawState* top;         // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int tbl[(0x10D8 - 0xF50) / 4];  // 0xF50
    // vptr at 0x10D8; slot N at vtable offset N*8
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77();
    virtual void DrawSprite(const sV4_229BA8* pos, const sV2_229BA8* size, const sV2_229BA8* uv0,
                     const sV2_229BA8* uv1, const sV4_229BA8* col);
};

static inline int GetTexId_229BA8(cGfx_229BA8* g, int i)
{
    return g->tbl[i + 0x33];
}

static inline void set0_229BA8(cGfx_229BA8* g, int mask, int shift, int v)
{
    g->top->m_bitGroups[0] = (((int)g->top->m_bitGroups[0]) & ~mask) | ((v << shift) & mask);
}

static inline void set1_229BA8(cGfx_229BA8* g, int mask, int shift, int v)
{
    g->top->m_bitGroups[1] = (((int)g->top->m_bitGroups[1]) & ~mask) | ((v << shift) & mask);
}

// Full-mask field set (value == mask), as main/game's draw-state code writes it.
static inline void SetBits_229BA8(int* p, int mask, int bits) { *p = (*p & ~mask) | bits; }

static inline void set2_229BA8(cGfx_229BA8* g, int mask, int shift, int v)
{
    g->top->m_bitGroups[2] = (((int)g->top->m_bitGroups[2]) & ~mask) | ((v << shift) & mask);
}




/**
 * Draws each live crowd marker as a 100 x 100 white sprite.
 *
 * The first live marker (count at +0x10 of each 0x20-byte ring entry, from +0x2220) pushes a draw state reset to
 * sDrawState_s_defaultDrawState, with GrMan's texture id entry 14 (+0x1054) as texture and its blend and test bits set.
 * Each live marker is drawn with GrMan's DrawSprite (slot 78) at its position, texture coordinates (0, 0)-(1, 1) and
 * colour (1, 1, 1, 1); the state is popped at the end. cGame_render calls it for CrowdMan2d.
 *
 * @param crowdMan  the 2D crowd manager
 */
extern "C" void func_00229BA8(cCrowdMan2d* crowdMan)
{
    cCrowdMan2d* self = crowdMan;
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    int pushed = 0;
    sV2_229BA8 uv0;
    sV2_229BA8 uv1(1.0f, 1.0f);
    sV4_229BA8 col(1.0f, 1.0f, 1.0f, 1.0f);
    sV2_229BA8 size(100.0f, 100.0f);
    tCrowdMarker2d* markerList = ((&self->mMarker[0]));
    for (int i = 0; i < 40; i++) {
        if (markerList[i].mLife > 0) {
            if (!pushed) {
                pushed = 1;
                ((cGfx_229BA8*)ps2GraphicsMan)->top[1] = ((cGfx_229BA8*)ps2GraphicsMan)->top[0];
                ((cGfx_229BA8*)ps2GraphicsMan)->top++;
                *((cGfx_229BA8*)ps2GraphicsMan)->top = sDrawState_s_defaultDrawState_229BA8;
                ((cGfx_229BA8*)ps2GraphicsMan)->top->m_textureState.m_texid[0] = GetTexId_229BA8((cGfx_229BA8*)ps2GraphicsMan, 14);
                set0_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0xC, 2, 3);
                SetBits_229BA8(&(*(int*)&((cGfx_229BA8*)ps2GraphicsMan)->top->m_bitGroups[1]), 0x400000, 0x400000);
                set1_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x1800000, 23, 0);
                set1_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x300000, 20, 0);
                set1_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0xFF000, 12, 20);
                set1_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x3, 0, 1);
                set2_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x1FFFFC00, 10, 0);
                set2_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x3E0, 5, 4);
                set1_229BA8((cGfx_229BA8*)ps2GraphicsMan, 0x7C, 2, 7);
            }
            ps2GraphicsMan->DrawSprite(*(const t4Vector*)&(*(sV4_229BA8*)&((&self->mMarker[0]))[i].mPos), *(const t2Vector*)&size, *(const t2Vector*)&uv0, *(const t2Vector*)&uv1, *(const tARGB*)&col);
        }
    }
    if (pushed) {
        ((cGfx_229BA8*)ps2GraphicsMan)->top--;
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229E20);
#ifdef SKIP_ASM
// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
/**
 * Saves the 2D crowd manager into a replay frame: writes its 128 resource ids (0x200 bytes at +0x8) with the stream's
 * first virtual.
 *
 * cReplay_saveFrame calls it.
 *
 * @param stream  the replay stream
 */
extern "C" void func_00229E20(cCrowdMan2d* self, cBxReplayFrame* stream)
{
    stream->write((const char*)&self->mQuadId[0], 0x200);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229E58);
#ifdef SKIP_ASM
extern "C" void func_00229398(cCrowdMan2d* self);
extern "C" void func_002292E0(cCrowdMan2d* self);
extern "C" void func_00229910(void* self, int slot, int a1);

// Serialisation stream (a replay frame): write(buf, size) is its first virtual, read(buf, size) its second.
struct sCrowdId_229E58 {
    unsigned int id;
    sCrowdId_229E58() : id(0xFFFFFFFF) {}
};

/**
 * Restores the 2D crowd manager from a replay frame: releases and resets every slot, reads 128 resource ids (0x200
 * bytes) with the stream's second virtual and places each used one again (func_00229910).
 *
 * The slot delays restart at random values (func_002292E0). cReplay_restoreFrame calls it.
 *
 * @param stream  the replay stream
 */
extern "C" void func_00229E58(cCrowdMan2d* self, cBxReplayFrame* stream)
{
    func_00229398(self);
    func_002292E0(self);
    sCrowdId_229E58 ids[128];
    stream->read((char*)ids, 0x200);
    int i;
    for (i = 0; i < 128; i++) {
        if (ids[i].id != cCrowdMan2d::kFreeQuad) {
            func_00229910(self, i, ids[i].id);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F30);
#ifdef SKIP_ASM
/**
 * Stores two 4x4 matrices in VU0 data memory, eight quadwords from the address 0x40 put in vi1.
 *
 * The matrices go in order, @p firstMatrix then @p secondMatrix, with vsqi (which advances vi1 by one quadword).
 * FrustumIntersector_FrustumIntersector calls it with its two matrices before gathering the visible world.
 *
 * @param firstMatrix  the first 4x4 matrix (16-byte aligned)
 * @param secondMatrix  the second 4x4 matrix (16-byte aligned)
 */
// Upload two 4x4 matrices (a, b) to VU0 data memory starting at quadword 0x40 (vi1 counts quadwords).
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own matrix store.
extern "C" void func_00229F30(void* firstMatrix, void* secondMatrix)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf2, 0x10(%2)\n"
        "lqc2      $vf3, 0x20(%2)\n"
        "lqc2      $vf4, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(0x40), "r"(firstMatrix), "r"(secondMatrix)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F80__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: FrustumIntersector_FrustumIntersector calls it right after func_00229F30.
 */
void func_00229F80(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", func_00229F88);
#ifdef SKIP_ASM
/**
 * Stores a vector and a 4x4 matrix (five quadwords) in VU0 data memory at quadword 80 + 5 * @p index.
 *
 * cOcclusionMan_updateView (movie player) calls it.
 *
 * @param index  the slot
 * @param vector  the vector (16-byte aligned)
 * @param matrix  the 4x4 matrix (16-byte aligned)
 */
// Upload a vector plus a 4x4 matrix (5 qwords) to VU0 data memory slot `index`,
// starting at qword 0x50 + index * 5.
// PORT: PS2-only VU0 inline asm (ctc2/lqc2/vsqi); the PC port needs its own store.
extern "C" void func_00229F88(int index, void* vector, void* matrix)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "ctc2.ni   %0, $vi1\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x0(%2)\n"
        "lqc2      $vf3, 0x10(%2)\n"
        "lqc2      $vf4, 0x20(%2)\n"
        "lqc2      $vf5, 0x30(%2)\n"
        "vsqi.xyzw $vf1, ($vi1++)\n"
        "vsqi.xyzw $vf2, ($vi1++)\n"
        "vsqi.xyzw $vf3, ($vi1++)\n"
        "vsqi.xyzw $vf4, ($vi1++)\n"
        "vsqi.xyzw $vf5, ($vi1++)\n"
        ".set reorder\n"
        :
        : "r"(index * 5 + 80), "r"(vector), "r"(matrix)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_crossinginstance);
#ifdef SKIP_ASM
struct sVisEnt_9FC8 {
    int a;
    int f4;
};
struct sVisRec_9FC8 {
    int f0;
    int pad[5];
};
struct sVisTbl_9FC8 {
    char pad0[0x20];
    sVisEnt_9FC8 ent[0x7A];
    sVisRec_9FC8 rec[1];
};

struct sVisOwner_9FC8 {
    char pad0[0x90];
    char **tblp;                  // 0x90
    char pad94[0x209C - 0x94];
    int countA;                   // 0x209C
    void *nodesA[0x800];          // 0x20A0
    int countB;                   // 0x40A0
    void *nodesB[0x400];          // 0x40A4
    void *last;                   // 0x50A4
};

static inline int visOk_9FC8(sVisOwner_9FC8 *self, char *vvnode)
{
    tInstanceRecord* vnode = (tInstanceRecord*)vvnode;
    tInstanceRecord* node = (tInstanceRecord*)vnode;
    if ((node->m_iFlags & 3) != 3) {
        return 0;
    }
    sVisTbl_9FC8 *tbl = (sVisTbl_9FC8 *)*self->tblp;
    if (node->m_SMap.m_uiSectionID == 0xFF) {
        return 1;
    }
    if ((tbl->ent[node->m_SMap.m_uiSectionID].f4 ^ 6) == 0) {
        return tbl->rec[node->m_SMap.m_iHullID].f0 == 3;
    }
    return 0;
}

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xDB8 (box vs frustum test).
extern "C" void FrustumIntersector_crossinginstance_impl(sVisOwner_9FC8 *self, void *vnode, int frustum) __asm__("FrustumIntersector_crossinginstance");
/**
 * Files the previously tested world node by its VU0 frustum result, then starts the test for @p vnode.
 *
 * Reads the last microprogram's result (cfc2 of vi1) and files the pending node (+0x50A4): result 0 or 3 appends it to
 * the list at +0x20A0 (count +0x209C, 0x800 entries), 4 to the list at +0x40A4 (count +0x40A0, 0x400 entries), 1 or 2
 * drops it. Then @p vnode becomes pending. A node is tested only when the low two bits of its flags (+0x8) are set and
 * it is resident: its section byte (+0x7D) is 0xFF, or that section's state in the world cache's view is 6 and its hull
 * page (+0x7E) is loaded (m_State 3), the cache being m_pWorldCache of the world at +0x90. Otherwise the call returns
 * at once and the pending node waits for the next call. The test loads the node's box (+0x60 min, +0x6C max, w = 1) and
 * the vector at +0x50 into vf20-vf22 and @p frustum into vi14, and starts the VU0 microprogram at 0xDB8 (vcallms).
 * FrustumIntersector_AddAllInstancesCrossing calls it for each node, then once with null to file the last one.
 *
 * @param vnode  the node to test, or null to file the pending node
 * @param frustum  the value put in vi14 for the microprogram
 */
extern "C" void FrustumIntersector_crossinginstance_impl(sVisOwner_9FC8 *self, void *vnode, int frustum)
{
    tInstanceRecord* node = (tInstanceRecord*)vnode;
    unsigned int testResult;

    if (((char*)node) != 0) {
        if (!visOk_9FC8(self, ((char*)node))) {
            return;
        }
        {
            float lo[4] __attribute__((aligned(16)));
            float hi[4] __attribute__((aligned(16)));
            lo[0] = node->m_fvMin.m[0];
            lo[1] = node->m_fvMin.m[1];
            lo[2] = node->m_fvMin.m[2];
            hi[0] = node->m_fvMax.m[0];
            hi[1] = node->m_fvMax.m[1];
            hi[2] = node->m_fvMax.m[2];
            lo[3] = 1.0f;
            hi[3] = 1.0f;
            __asm__ __volatile__("cfc2.i    %0, $vi1\n" : "=r"(testResult));
            __asm__ __volatile__(
                "lqc2      $vf20, 0x0(%0)\n"
                "lqc2      $vf21, 0x0(%1)\n"
                "lqc2      $vf22, 0x0(%2)\n"
                "ctc2.ni   %3, $vi14\n"
                "vcallms   0xDB8\n"
                :
                : "r"(lo), "r"(hi), "r"((char*)&node->m_fvBSphere), "r"(frustum));
        }
    } else {
        __asm__ __volatile__("cfc2.i    %0, $vi1\n" : "=r"(testResult));
    }
    switch (testResult) {
    case 0:
    case 3:
        if (self->last != 0) {
            self->nodesA[self->countA++] = self->last;
        }
        break;
    case 1:
    case 2:
        break;
    case 4:
        if (self->last != 0) {
            self->nodesB[self->countB++] = self->last;
        }
        break;
    }
    self->last = ((char*)node);
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_crossingpatch);
#ifdef SKIP_ASM
static inline int visOk_A128(FrustumIntersector *self, tPatchRecord *node)
{
    int idx = node->m_SMap.m_iHullID;
    cWorldCache *cache = self->world->m_pWorldCache;
    if (idx < 0) {
        return 0;
    }
    if ((cache->m_WorldView.mp_SectionState[node->m_SMap.m_uiSectionID].state ^ 6) == 0) {
        return cache->m_HullPages[idx].m_State == 3;
    }
    return 0;
}

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xDB8 (box vs frustum test).
extern "C" void FrustumIntersector_crossingpatch_impl(FrustumIntersector *self, void *vnode, int frustum)
    __asm__("FrustumIntersector_crossingpatch");
/**
 * Files the previously tested patch (a tPatchRecord) by its VU0 frustum result, then starts the test for @p vnode.
 *
 * Same pipeline as FrustumIntersector_crossinginstance for patches: the pending one is lastpatch; result 0 or 3
 * appends it to patchesinside (count numpatchesinside, 0x800 entries), 4 to patchescrossing (count
 * numpatchescrossing, 0x200 entries). A patch is tested only when its m_SMap hull page is not negative, the state of
 * its section in the world cache's view is 6 and the page is loaded (m_State 3). The test uses the box m_fvMin /
 * m_fvMax and m_fvBSphere. FrustumIntersector_AddAllPatchesCrossing calls it.
 *
 * @param vnode  the node to test, or null to file the pending node
 * @param frustum  the value put in vi14 for the microprogram
 */
extern "C" void FrustumIntersector_crossingpatch_impl(FrustumIntersector *self, void *vnode, int frustum)
{
    tPatchRecord *node = (tPatchRecord *)vnode;
    unsigned int testResult;

    if (node != 0) {
        if (!visOk_A128(self, node)) {
            return;
        }
        {
            float lo[4] __attribute__((aligned(16)));
            float hi[4] __attribute__((aligned(16)));
            lo[0] = node->m_fvMin.m[0];
            lo[1] = node->m_fvMin.m[1];
            lo[2] = node->m_fvMin.m[2];
            hi[0] = node->m_fvMax.m[0];
            hi[1] = node->m_fvMax.m[1];
            hi[2] = node->m_fvMax.m[2];
            lo[3] = 1.0f;
            hi[3] = 1.0f;
            __asm__ __volatile__("cfc2.i    %0, $vi1\n" : "=r"(testResult));
            __asm__ __volatile__(
                "lqc2      $vf20, 0x0(%0)\n"
                "lqc2      $vf21, 0x0(%1)\n"
                "lqc2      $vf22, 0x0(%2)\n"
                "ctc2.ni   %3, $vi14\n"
                "vcallms   0xDB8\n"
                :
                : "r"(lo), "r"(hi), "r"((char*)&node->m_fvBSphere), "r"(frustum));
        }
    } else {
        __asm__ __volatile__("cfc2.i    %0, $vi1\n" : "=r"(testResult));
    }
    switch (testResult) {
    case 0:
    case 3:
        if (self->lastpatch != 0) {
            self->patchesinside[self->numpatchesinside++] = self->lastpatch;
        }
        break;
    case 1:
    case 2:
        break;
    case 4:
        if (self->lastpatch != 0) {
            self->patchescrossing[self->numpatchescrossing++] = self->lastpatch;
        }
        break;
    }
    self->lastpatch = node;
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_crossingfogvolume);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: callers declare FrustumIntersector_crossingfogvolume with other pointer types; defined under an asm label.
extern "C" void FrustumIntersector_crossingfogvolume_impl(FrustumIntersector* self, tParticleInstanceRecord* obj,
                                                          int frustum) __asm__("FrustumIntersector_crossingfogvolume");

/**
 * Files the previously tested object by its VU0 frustum result, then starts the test for @p obj.
 *
 * Same pipeline as FrustumIntersector_crossinginstance without the residency checks: the pending object is
 * lastfogvolume, result 0 or 3 appends it to fogvolumesinside, 4 to fogvolumescrossing. The test uses the box
 * (m_fvMin / m_fvMax) and the bounding sphere (m_fvBSphere). FrustumIntersector_AddAllTypedCrossing calls it for
 * objects of type 7.
 *
 * @param obj  the object to test, or null to file the pending object
 * @param frustum  the value put in vi14 for the microprogram
 */
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xDB8 (box vs frustum test).
// Files the previous object by the result of its test, then starts the test for this one.
extern "C" void FrustumIntersector_crossingfogvolume_impl(FrustumIntersector* self, tParticleInstanceRecord* obj,
                                                          int frustum)
{
    unsigned int testResult;
    if (obj != 0) {
        float lo[4] __attribute__((aligned(16)));
        float hi[4] __attribute__((aligned(16)));
        lo[0] = obj->m_fvMin.m[0];
        lo[1] = obj->m_fvMin.m[1];
        lo[2] = obj->m_fvMin.m[2];
        hi[0] = obj->m_fvMax.m[0];
        hi[1] = obj->m_fvMax.m[1];
        hi[2] = obj->m_fvMax.m[2];
        lo[3] = 1.0f;
        hi[3] = 1.0f;
        __asm__ __volatile__("cfc2.i    %0, $vi1
" : "=r"(testResult));
        __asm__ __volatile__(
            "lqc2      $vf20, 0x0(%0)
"
            "lqc2      $vf21, 0x0(%1)
"
            "lqc2      $vf22, 0x0(%2)
"
            "ctc2.ni   %3, $vi14
"
            "vcallms   3512
"
            :
            : "r"(lo), "r"(hi), "r"(obj->m_fvBSphere.m), "r"(frustum));
    } else {
        __asm__ __volatile__("cfc2.i    %0, $vi1
" : "=r"(testResult));
    }
    switch (testResult) {
    case 0:
    case 3:
        if (self->lastfogvolume != 0) {
            self->fogvolumesinside[self->numfogvolumesinside++] = self->lastfogvolume;
        }
        break;
    case 1:
    case 2:
        break;
    case 4:
        if (self->lastfogvolume != 0) {
            self->fogvolumescrossing[self->numfogvolumescrossing++] = self->lastfogvolume;
        }
        break;
    }
    self->lastfogvolume = obj;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllInstancesCrossing);
#ifdef SKIP_ASM
extern "C" void FrustumIntersector_crossinginstance(void* self, void* node, int frustum);

/**
 * Frustum-tests the nodes of each listed source with FrustumIntersector_crossinginstance, then files the last pending one.
 *
 * Each of the @p count 8-byte entries holds a cell and a frustum value; the cell's instance list (nodesinside[0], at
 * +0x20) is walked through each node's next link. Ends with FrustumIntersector_crossinginstance(self, 0, 0) when a node
 * is pending (lastinstance). FrustumIntersector_FrustumIntersector calls it.
 *
 * @param list  the source entries
 * @param count  the number of entries
 */
void FrustumIntersector::AddAllInstancesCrossing(CellClipT* list, int count)
{
    for (; count > 0; count--, list++) {
        nash_Node* n = ((nash_Cell*)list->pCell)->nodesinside[0].head;
        while (n != 0) {
            FrustumIntersector_crossinginstance(this, n, ((int)list->uVisMask));
            n = n->next;
        }
    }
    if ((int)lastinstance != 0) {
        FrustumIntersector_crossinginstance(this, 0, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllPatchesCrossing);
#ifdef SKIP_ASM
extern "C" void FrustumIntersector_crossingpatch(void* self, void* node, int frustum);

/**
 * Frustum-tests the nodes of each listed source with FrustumIntersector_crossingpatch, then files the last pending one.
 *
 * Same as FrustumIntersector_AddAllInstancesCrossing with each cell's patch list (nodesinside[1], at +0x24) and the
 * pending node in lastpatch. FrustumIntersector_FrustumIntersector calls it.
 *
 * @param list  the source entries
 * @param count  the number of entries
 */
void FrustumIntersector::AddAllPatchesCrossing(CellClipT* list, int count)
{
    for (; count > 0; count--, list++) {
        nash_Node* n = ((nash_Cell*)list->pCell)->nodesinside[1].head;
        while (n != 0) {
            FrustumIntersector_crossingpatch(this, n, ((int)list->uVisMask));
            n = n->next;
        }
    }
    if ((int)lastpatch != 0) {
        FrustumIntersector_crossingpatch(this, 0, 0);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllTypedCrossing);
#ifdef SKIP_ASM


extern "C" void FrustumIntersector_crossingfogvolume(void* self, cTypedSpaceNode* node, int arg);

/**
 * Sorts the objects of each listed source: kSNT_FogVol (7) is frustum-tested (FrustumIntersector_crossingfogvolume),
 * kSNT_Halo (8) goes straight into lighthalosinside (count numlighthalosinside).
 *
 * Walks the typed list (nodesinside[2]) of each of the @p n entries' cells (cell, frustum value), switching on each
 * object's type. Other types are skipped. Ends with FrustumIntersector_crossingfogvolume(self, 0, 0) when an object
 * is pending (lastfogvolume).
 * FrustumIntersector_FrustumIntersector calls it.
 *
 * @param items  the source entries
 * @param n  the number of entries
 */
void FrustumIntersector::AddAllTypedCrossing(CellClipT* items, int n)
{
    for (; n > 0; n--, items++) {
        cTypedSpaceNode* object = (cTypedSpaceNode*)((nash_Cell*)items->pCell)->nodesinside[2].head;
        for (; object != 0; object = ((cTypedSpaceNode*)object->next)) {
            if (object->type == kSNT_FogVol) {
                FrustumIntersector_crossingfogvolume(this, object, ((int)items->uVisMask));
            } else if (object->type == kSNT_Halo) {
                lighthalosinside[numlighthalosinside++] = (tHaloRecord*)object;
            }
        }
    }
    if ((int)lastfogvolume != 0) {
        FrustumIntersector_crossingfogvolume(this, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllInstancesInside);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVisNode_A5A0 {
    sVisNode_A5A0* next;    // 0x00
    int pad_0x4;
    unsigned int flags;     // 0x08
    char pad_0xC[0x44];
    int sphere[4];          // 0x50
    char pad_0x60[0x1D];
    unsigned char slot;     // 0x7D
    short path;             // 0x7E
};

struct sVisSrc_A5A0 {
    char pad_0x0[0x20];
    sVisNode_A5A0* head;    // 0x20
};

struct sVisList_A5A0 {
    sVisSrc_A5A0* src;
    int vuFrustum;
};

struct sVisSlot_A5A0 {
    int f0;
    int type;
};

struct sVisCam_A5A0 {
    char pad_0x0[0x20];
    sVisSlot_A5A0 slots[(0x3F0 - 0x20) / 8];  // 0x20
    struct {
        int kind;
        char pad[0x14];
    } paths[1];                     // 0x3F0
};

struct sVisOwner_A5A0 {
    char pad_0x0[0x90];
    sVisCam_A5A0** cam;             // 0x90
    char pad_0x94[0x209C - 0x94];
    int count;                      // 0x209C
    sVisNode_A5A0* nodes[1];        // 0x20A0
};

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xEF0 (sphere vs frustum test; 2 = outside).
static inline int visSphereTest_A5A0(int* sphere, int frustum)
{
    int r;
    __asm__ __volatile__(
        "lqc2      $vf22, 0x0(%1)\n"
        "ctc2.ni   %2, $vi14\n"
        "vcallms   0xEF0\n"
        "cfc2.i    %0, $vi1\n"
        : "=&r"(r)
        : "r"(sphere), "r"(frustum));
    return r;
}

static inline bool IsT6_A5A0(sVisCam_A5A0* c, int i)
{
    return c->slots[i].type == 6;
}

// Path-bound nodes are usable when unassigned (slot 0xFF) or when their slot is a type-6 camera slot on a kind-3 path.
static inline int visOnPath_A5A0(sVisCam_A5A0* cam, sVisNode_A5A0* n)
{
    if (n->slot == 0xFF)
        return 1;
    if (IsT6_A5A0(cam, n->slot))
        return cam->paths[n->path].kind == 3;
    return 0;
}

static inline int visUsable_A5A0(sVisOwner_A5A0* self, sVisNode_A5A0* n)
{
    if ((n->flags & 3) != 3)
        return 0;
    return visOnPath_A5A0(*self->cam, n);
}

/**
 * Collects the visible, path-usable objects of each listed octree cell into the world view's object list at +0x20A0.
 *
 * Each of the @p count 8-byte entries holds a cell (its linked object list starts at +0x20, next link at +0x0) and the
 * cell's remaining clip-plane mask. An object qualifies when both low bits of its flags (+0x8) are set and it is either
 * unassigned (slot +0x7D is 0xFF) or its slot in the camera-slot table (*(self + 0x90), entries at +0x20) has type 6
 * and the path it names (+0x7E) has kind 3. Qualifying objects are sphere-tested against the masked planes by the VU0
 * microprogram at 0xEF0 (skipped when the mask is 0; kSphereTestOutside means outside) and appended to the list at
 * +0x20A0, count +0x209C (no bounds check). FrustumIntersector_FrustumIntersector calls it with the fully-inside cell
 * list (+0x98, count +0x94).
 *
 * @param list_  the cell entries (cell, plane mask)
 * @param count  the number of entries
 */
void FrustumIntersector::AddAllInstancesInside(void* list_, int count)
{
    sVisOwner_A5A0* self = (sVisOwner_A5A0*)this;
    sVisList_A5A0* list = (sVisList_A5A0*)list_;
    for (; count > 0; count--, list++) {
        sVisNode_A5A0* n = list->src->head;
        while (n != 0) {

                int frustum = list->vuFrustum;
                if (visUsable_A5A0(self, n)) {
                    if (frustum == 0 || visSphereTest_A5A0(n->sphere, frustum) != kSphereTestOutside) {
                        self->nodes[self->count++] = n;
                    }
                }
                n = n->next;

        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllPatchesInside);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xEF0 (sphere vs frustum test; 2 = outside).
static inline int visSphereTest_A698(tf4Vector* sphere, int frustum)
{
    int r;
    __asm__ __volatile__(
        "lqc2      $vf22, 0x0(%1)\n"
        "ctc2.ni   %2, $vi14\n"
        "vcallms   0xEF0\n"
        "cfc2.i    %0, $vi1\n"
        : "=&r"(r)
        : "r"(sphere), "r"(frustum));
    return r;
}

static inline bool IsT6_A698(cWorldCache* c, int i)
{
    return c->m_WorldView.mp_SectionState[i].state == 6;
}

static inline int visOnPath_A698(cWorldCache* cache, tPatchRecord* n)
{
    if (n->m_SMap.m_iHullID < 0)
        return 0;
    if (IsT6_A698(cache, n->m_SMap.m_uiSectionID))
        return cache->m_HullPages[n->m_SMap.m_iHullID].m_State == 3;
    return 0;
}

/**
 * Collects the visible patches of each listed octree cell into patchesinside.
 *
 * Same walk as FrustumIntersector_AddAllInstancesInside over each cell's patch list (nodesinside[1]): a patch qualifies
 * when its m_SMap hull page is not negative, its section's state in the world cache's view is 6 and that page is loaded
 * (m_State 3). Its m_fvBSphere is tested on VU0 (microprogram 0xEF0, kSphereTestOutside = outside) unless the entry's
 * plane mask is 0; survivors go to patchesinside, count numpatchesinside (no bounds check).
 *
 * @param list_  the cell entries (cell, plane mask)
 * @param count  the number of entries
 */
void FrustumIntersector::AddAllPatchesInside(void* list_, int count)
{
    CellClipT* list = (CellClipT*)list_;
    for (; count > 0; count--, list++) {
        tPatchRecord* n = (tPatchRecord*)((nash_Cell*)list->pCell)->nodesinside[1].head;
        while (n != 0) {
            // A do/while(0) block around the body (as a macro would) reproduces the target's register choice.
            do {
                int frustum = ((int)list->uVisMask);
                if (visOnPath_A698(world->m_pWorldCache, n)) {
                    if (frustum == 0 || visSphereTest_A698(&n->m_fvBSphere, frustum) != kSphereTestOutside) {
                        patchesinside[numpatchesinside++] = n;
                    }
                }
                n = (tPatchRecord*)n->m_iIntersect.next;
            } while (0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumIntersector_AddAllTypedInside);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallms/cfc2); the PC port needs a C
// version of the microprogram at 0xEF0 (sphere vs frustum test; 2 = outside).
static inline int visSphereTest_A770(tf4Vector* sphere, int frustum)
{
    int r;
    __asm__ __volatile__(
        "lqc2      $vf22, 0x0(%1)\n"
        "ctc2.ni   %0, $vi14\n"
        "vcallms   0xEF0\n"
        "cfc2.i    %0, $vi1\n"
        : "=r"(r)
        : "r"(sphere), "0"(frustum));
    return r;
}

/**
 * Sorts the objects of each listed octree cell by type into two lists of the world view.
 *
 * Walks each entry's cell's typed list (nodesinside[2]), switching on each node's type. Fog volumes (kSNT_FogVol) are
 * sphere-tested on VU0 (microprogram 0xEF0, kSphereTestOutside = outside; skipped when the entry's plane mask is 0) and
 * appended to fogvolumesinside (count numfogvolumesinside); light halos (kSNT_Halo) go to lighthalosinside (count
 * numlighthalosinside) untested. Other types are skipped. Neither append checks for overflow.
 *
 * @param list  the cell entries (cell, plane mask)
 * @param count  the number of entries
 */
void FrustumIntersector::AddAllTypedInside(CellClipT* list, int count)
{
    for (; count > 0; count--, list++) {
        cTypedSpaceNode* n = (cTypedSpaceNode*)((nash_Cell*)list->pCell)->nodesinside[2].head;
        while (n != 0) {
            if (n->type == kSNT_FogVol) {
                // the typed records begin with the cTypedSpaceNode words: the casts view the node as its record
                tParticleInstanceRecord* fog = (tParticleInstanceRecord*)n;
                int frustum = ((int)list->uVisMask);
                if (frustum == 0 || visSphereTest_A770(&fog->m_fvBSphere, frustum) != kSphereTestOutside) {
                    fogvolumesinside[numfogvolumesinside++] = fog;
                }
            } else if (n->type == kSNT_Halo) {
                lighthalosinside[numlighthalosinside++] = (tHaloRecord*)n;
            }
            n = (cTypedSpaceNode*)n->next;
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/ssxapp", FrustumCallback_operatorCall);
#ifdef SKIP_ASM
extern char D_948[];
extern char D_00442130[];




struct sVisEntry_A830 {
    nash_Cell* node;
    int mask;
};

struct sVisOwner_A830 {
    char pad_0x0[0x94];
    int countIn;                    // 0x94
    sVisEntry_A830 in[0x200];       // 0x98
    int countPart;                  // 0x1098
    sVisEntry_A830 part[0x200];     // 0x109C
};


static inline float cellScale_A830(int level)
{
    union { int i; float f; } s;
    s.i = (level + 127) << 23;
    return s.f;
}

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallmsr/cfc2); the PC port needs a C
// version of the microprogram at D_948 (sphere vs frustum plane test).
static inline int sphereTest_A830(char* vinst, int* mask, t4Vector* sphere)
{
    cOcclusionMan* inst = (cOcclusionMan*)vinst;
    int res = 1;
    for (int i = 0; i < inst->m_iNumActive; i++) {
        int bit = 1 << i;
        if (*mask & bit) {
            t4Vector* m = ((&inst->m_InView[i].pVis->m_fvPlaneCoeffs));
            int r;
            __asm__ __volatile__(
                "lqc2      $vf1, %1\n"
                "lqc2      $vf2, %2\n"
                "lqc2      $vf3, %3\n"
                "lqc2      $vf4, %4\n"
                "lqc2      $vf5, %5\n"
                "lqc2      $vf6, %6\n"
                "ctc2.ni   %7, $vi27\n"
                "vnop\n"
                "vnop\n"
                "vcallmsr  $vi27\n"
                "cfc2.i    %0, $vi1\n"
                : "=r"(r)
                : "m"(m[0]), "m"(m[1]), "m"(m[2]), "m"(m[3]), "m"(m[4]), "m"(*sphere), "r"((int)D_948 >> 3)
                : "memory");
            if (r == 0)
                return 0;
            if (r == 2)
                res = 2;
            else
                *mask &= ~bit;
        }
    }
    return res;
}

static inline void boxResult_A830(int* r)
{
    int out, part;
    __asm__ __volatile__(
        "cfc2.i    %0, $vi1\n"
        "cfc2.ni   %1, $vi2\n"
        : "=r"(part), "=r"(out));
    if (out != 0)
        *r = 1;
    else if (part != 0)
        *r = 2;
    else
        *r = 0;
}

/**
 * Culls one octree cell against the view and, when it is visible, files it and recurses into its eight children.
 *
 * The context @p vctx holds the world view being built (+0x0), the clip-plane source (+0x4: plane count at +0x10C,
 * planes at +0x8 in 8-byte steps), the mask of planes still to test (+0x8) and an inside flag (+0xC). A cell's size is
 * 2^level (built from the float exponent). Unless the parent was fully inside, the cell box goes through the VU0
 * box-vs-frustum microprogram at 0x570 (matrices at D_00442130 and the view's +0x10; GMClipCodes): GM_CLIPCODE_OUTSIDE
 * returns, GM_CLIPCODE_CROSSING is partly inside, GM_CLIPCODE_INSIDE is inside. With a non-zero mask the cell's
 * bounding sphere (center at the cell middle, radius 1.2124 * size) is then tested against each masked plane by the
 * microprogram at D_948: 0 culls the cell, and planes the sphere is wholly in front of are cleared from the mask. A
 * fully-inside cell is appended (node, mask) to the list at +0x98 (count +0x94) and its children are visited with the
 * inside flag set; a partly-inside one goes to the list at +0x109C (count +0x1098) and its children are box-tested
 * again. Children are visited unless the level is kLeafLevel (11); child index bit 2 is +x, bit 1 +y, bit 0 +z. No list
 * bounds checks.
 *
 * @param vctx  the cull context (view, plane source, plane mask, inside flag)
 * @param vnode  the octree node (eight child pointers)
 * @param vcfg  the cell: level (+0x0) and integer x, y, z (+0x4..+0xC)
 */
extern "C" void FrustumCallback_operatorCall(void* vctx, void* vnode, void* vcfg)
{
    FrustumCallback* ctx = (FrustumCallback*)vctx;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cfg = (nash_CellKey*)vcfg;
    int vec[4] __attribute__((aligned(16)));
    t4Vector sphere;
    int mask = ((int)ctx->vismask_);
    int cullResult;
    if (ctx->insidefrustum_ != 0) {
        cullResult = GM_CLIPCODE_INSIDE;
        if (mask != 0)
            { t4Vector cellSphere; float cellSize = cellScale_A830(cfg->level); cellSphere.w = cellSize * 1.2124354839324951f; cellSphere.x = ((float)cfg->x + 0.5f) * cellSize; cellSphere.y = ((float)cfg->y + 0.5f) * cellSize; cellSphere.z = ((float)cfg->z + 0.5f) * cellSize; sphere = cellSphere; }
    } else {
        vec[0] = cfg->x;
        vec[1] = cfg->y;
        vec[2] = cfg->z;
        vec[3] = (cfg->level + 127) << 23;
        // PORT: PS2-only VU0 microprogram call (box vs frustum test at 0x570).
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%0)\n"
            "lqc2      $vf2, 0x0(%1)\n"
            "lqc2      $vf3, 0x10(%1)\n"
            "vitof0.xyz $vf1, $vf1\n"
            "lqc2      $vf10, 0x0(%2)\n"
            "lqc2      $vf11, 0x10(%2)\n"
            "lqc2      $vf12, 0x20(%2)\n"
            "lqc2      $vf13, 0x30(%2)\n"
            "vmulaw.xyz ACC, $vf1, $vf1w\n"
            "vmaddw.xyz $vf15, $vf2, $vf1w\n"
            "vmulaw.xyz ACC, $vf1, $vf1w\n"
            "vmaddw.xyz $vf16, $vf3, $vf1w\n"
            "vcallms   0x570\n"
            :
            : "r"(vec), "r"(D_00442130), "r"((char*)&ctx->intersector->VFIntersect)
            : "memory");
        if (mask != 0)
            { t4Vector cellSphere; float cellSize = cellScale_A830(cfg->level); cellSphere.w = cellSize * 1.2124354839324951f; cellSphere.x = ((float)cfg->x + 0.5f) * cellSize; cellSphere.y = ((float)cfg->y + 0.5f) * cellSize; cellSphere.z = ((float)cfg->z + 0.5f) * cellSize; sphere = cellSphere; }
        boxResult_A830(&cullResult);
    }
    bool out_ = cullResult == GM_CLIPCODE_OUTSIDE;
    if (out_)
        return;
    if (mask != 0 && sphereTest_A830(((char*)ctx->occlusionman), &mask, &sphere) == 0)
        return;
    if (cullResult == GM_CLIPCODE_INSIDE) {
        ((sVisOwner_A830*)ctx->intersector)->in[((sVisOwner_A830*)ctx->intersector)->countIn].node = node;
        ((sVisOwner_A830*)ctx->intersector)->in[((sVisOwner_A830*)ctx->intersector)->countIn].mask = mask;
        ((sVisOwner_A830*)ctx->intersector)->countIn++;
        FrustumCallback childCtx;
        childCtx.insidefrustum_ = 1;
        (*(char**)&childCtx.intersector) = ((char*)ctx->intersector);
        (*(char**)&childCtx.occlusionman) = ((char*)ctx->occlusionman);
        childCtx.vismask_ = mask;
        if (cfg->level != nash_CellKey::kLeafLevel) {
            nash_CellKey n;
            n.level = cfg->level - 1;
            n.x = cfg->x * 2;
            n.y = cfg->y * 2;
            n.z = cfg->z * 2;
            if (((nash_Cell**)&node->children[0][0][0])[0]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[0], &n);
            n.x++;
            if (((nash_Cell**)&node->children[0][0][0])[4]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[4], &n);
            n.y++;
            if (((nash_Cell**)&node->children[0][0][0])[6]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[6], &n);
            n.x--;
            if (((nash_Cell**)&node->children[0][0][0])[2]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[2], &n);
            n.z++;
            if (((nash_Cell**)&node->children[0][0][0])[3]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[3], &n);
            n.x++;
            if (((nash_Cell**)&node->children[0][0][0])[7]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[7], &n);
            n.y--;
            if (((nash_Cell**)&node->children[0][0][0])[5]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[5], &n);
            n.x--;
            if (((nash_Cell**)&node->children[0][0][0])[1]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[1], &n);
        }
    } else if (cullResult == GM_CLIPCODE_CROSSING) {
        ((sVisOwner_A830*)ctx->intersector)->part[((sVisOwner_A830*)ctx->intersector)->countPart].node = node;
        ((sVisOwner_A830*)ctx->intersector)->part[((sVisOwner_A830*)ctx->intersector)->countPart].mask = mask;
        ((sVisOwner_A830*)ctx->intersector)->countPart++;
        FrustumCallback childCtx;
        childCtx.insidefrustum_ = 0;
        (*(char**)&childCtx.intersector) = ((char*)ctx->intersector);
        (*(char**)&childCtx.occlusionman) = ((char*)ctx->occlusionman);
        childCtx.vismask_ = mask;
        if (cfg->level != nash_CellKey::kLeafLevel) {
            nash_CellKey n;
            n.level = cfg->level - 1;
            n.x = cfg->x * 2;
            n.y = cfg->y * 2;
            n.z = cfg->z * 2;
            if (((nash_Cell**)&node->children[0][0][0])[0]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[0], &n);
            n.x++;
            if (((nash_Cell**)&node->children[0][0][0])[4]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[4], &n);
            n.y++;
            if (((nash_Cell**)&node->children[0][0][0])[6]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[6], &n);
            n.x--;
            if (((nash_Cell**)&node->children[0][0][0])[2]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[2], &n);
            n.z++;
            if (((nash_Cell**)&node->children[0][0][0])[3]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[3], &n);
            n.x++;
            if (((nash_Cell**)&node->children[0][0][0])[7]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[7], &n);
            n.y--;
            if (((nash_Cell**)&node->children[0][0][0])[5]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[5], &n);
            n.x--;
            if (((nash_Cell**)&node->children[0][0][0])[1]) FrustumCallback_operatorCall(&childCtx, ((nash_Cell**)&node->children[0][0][0])[1], &n);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", FrustumIntersector_FrustumIntersector);
#ifdef SKIP_ASM
extern "C" void cOcclusionMan_updateView(void* a, void* b);
extern "C" void func_00229F30(void* a, void* b);
void func_00229F80(void* p);
extern "C" void FrustumCallback_operatorCall(void* ctx, void* data, void* cfg);
extern "C" void FrustumIntersector_AddAllPatchesInside(FrustumIntersector* self, void* list, int n);
extern "C" void FrustumIntersector_AddAllInstancesInside(FrustumIntersector* self, void* list, int n);



// PORT: PS2-only VU0 inline asm (4x4 matrix copy through vf1..vf4).
static inline void MatCopy_22ADD8(t4Matrix* dst, const t4Matrix* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

static inline void Load_22ADD8(FrustumCallback* callback, nash_Cell* root, nash_Octant* octant)
{
    if (root) {
        FrustumCallback_operatorCall(callback, root, octant);
    }
}

/**
 * Builds a camera view's visible world: culls the octree and fills the view's object lists.
 *
 * Stores @p occlusionMan, @p cameraBox and @p world and zeroes every list count (numcellsinside ..
 * numlighthaloscrossing) and pending object (lastinstance, lastpatch, lastfogvolume). Copies the matrix returned by
 * the graphics manager's GetNetMatrix (vtable slot 43) to VFIntersect and GrMan's mGuardBandMatrix to GFIntersect,
 * calls cOcclusionMan_updateView(@p occlusionMan, @p cameraBox), loads both matrices into VU0 with func_00229F30 and
 * calls func_00229F80 (empty). Then runs FrustumCallback_operatorCall on the root cell of each of the Nash's eight
 * octants that has one, with the occlusion man's m_uiVisMask as the plane mask, and sorts the culled cells' objects:
 * FrustumIntersector_AddAllPatchesInside, FrustumIntersector_AddAllInstancesInside and
 * FrustumIntersector_AddAllTypedInside over cellsinside, FrustumIntersector_AddAllPatchesCrossing,
 * FrustumIntersector_AddAllInstancesCrossing and FrustumIntersector_AddAllTypedCrossing over cellscrossing.
 * cGame_render calls it once per view with a stack-allocated view.
 *
 * @param nash  the world's Nash grid (its eight octants, walked in flat order)
 * @param occlusionMan  the occlusion manager: holds the clip planes and the initial plane mask (m_uiVisMask)
 * @param cameraBox  the view's bounding box (passed to cOcclusionMan_updateView)
 * @param world  the world (read by FrustumIntersector_AddAllInstancesInside and FrustumIntersector_AddAllPatchesInside)
 * @return @p self
 */
extern "C" FrustumIntersector* FrustumIntersector_FrustumIntersector(FrustumIntersector* self, cBXNash* nash,
                                                                     cOcclusionMan* occlusionMan,
                                                                     const tBoundBox* cameraBox, cWorld* world)
{
    self->occlusionman = occlusionMan;
    self->camerabox = cameraBox;
    self->world = world;
    self->numcellsinside = 0;
    self->numcellscrossing = 0;
    self->numinstancesinside = 0;
    self->numinstancescrossing = 0;
    self->numpatchesinside = 0;
    self->numpatchescrossing = 0;
    self->numfogvolumesinside = 0;
    self->numfogvolumescrossing = 0;
    self->numlighthalosinside = 0;
    self->numlighthaloscrossing = 0;
    self->lastinstance = 0;
    self->lastpatch = 0;
    self->lastfogvolume = 0;
    {
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        t4Matrix gfxMatrix = ps2GraphicsMan->GetNetMatrix();
        MatCopy_22ADD8(&self->VFIntersect, &gfxMatrix);
    }
    MatCopy_22ADD8(&self->GFIntersect, ((&GrMan->mGuardBandMatrix)));
    cOcclusionMan_updateView(self->occlusionman, (void*)self->camerabox);
    func_00229F30(&self->VFIntersect, &self->GFIntersect);
    func_00229F80((void*)self->camerabox);
    FrustumCallback callback;
    cOcclusionMan* om = self->occlusionman;
    unsigned int visMask = om->m_uiVisMask;
    callback.intersector = self;
    callback.occlusionman = om;
    callback.vismask_ = visMask;
    callback.insidefrustum_ = 0;
    Load_22ADD8(&callback, nash->octant[0][0][0].rootcell, &nash->octant[0][0][0]);
    Load_22ADD8(&callback, nash->octant[0][0][1].rootcell, &nash->octant[0][0][1]);
    Load_22ADD8(&callback, nash->octant[0][1][0].rootcell, &nash->octant[0][1][0]);
    Load_22ADD8(&callback, nash->octant[0][1][1].rootcell, &nash->octant[0][1][1]);
    Load_22ADD8(&callback, nash->octant[1][0][0].rootcell, &nash->octant[1][0][0]);
    Load_22ADD8(&callback, nash->octant[1][0][1].rootcell, &nash->octant[1][0][1]);
    Load_22ADD8(&callback, nash->octant[1][1][0].rootcell, &nash->octant[1][1][0]);
    Load_22ADD8(&callback, nash->octant[1][1][1].rootcell, &nash->octant[1][1][1]);
    self->AddAllPatchesInside(self->cellsinside, self->numcellsinside);
    self->AddAllInstancesInside(self->cellsinside, self->numcellsinside);
    self->AddAllTypedInside(self->cellsinside, self->numcellsinside);
    self->AddAllPatchesCrossing(self->cellscrossing, self->numcellscrossing);
    self->AddAllInstancesCrossing(self->cellscrossing, self->numcellscrossing);
    self->AddAllTypedCrossing(self->cellscrossing, self->numcellscrossing);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/ssxapp", cGame_render);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cGameViewMan_updateStreamFocus(void* views);
extern "C" int cGameViewMan_isStreamReady(void* views);
extern "C" int cFadeScreen_drawFade(void* self);
extern "C" void func_0022C830(void* self);
extern "C" void cGarbageMan_Update();
extern "C" void t4mat_invert(void* m);
extern "C" void cGame_renderBuckets(void* self);
extern "C" void func_00229BA8(cCrowdMan2d* p);
extern "C" void func_00344138(void* p);
extern "C" void cMultiParticleMan_render(void* p);
extern "C" void cGame_renderModels(void* self, void* world);
extern "C" void cGame_renderFogVolumes(void* self, void* world);
extern "C" void cGame_renderLightHalos(void* self, void* world, int view);
extern "C" void cGame_renderPatches(void* self, void* world);
extern "C" int cMenuSys_isActive(void* menu);
extern "C" void cMenuSys_render(void* menu);
extern "C" void cUIEngine_render2D(void* p);
void cWScriptMan_render(void* p);
void cSSXScriptEngine_Render(void* p);
extern "C" void cBXLoadIndicator_Render(void* p);
extern "C" void cSSXAudio_Update(void* snd, float dt, int a2);
extern "C" void cWorldTriggerManager_Update(void* p);



struct sRect_22B008 {
    float x, y, w, h;
};

struct sBox_22B008 {
    t4Vector min;
    t4Vector max;
};

struct sScale_22B008 {
    float x, y;
    sScale_22B008(float ax, float ay) : x(ax), y(ay) {}
};

struct sColor_22B008 {
    float r, g, b, a;
    sColor_22B008(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sDrawState_22B008 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
};

extern sDrawState_22B008 sDrawState_s_defaultDrawState_22B008 __asm__("sDrawState_s_defaultDrawState");
extern t4Matrix t4mat_Identity_22B008 __asm__("t4mat_Identity");

class cGfx_22B008 {
public:
    char pad_0x0[0x24];
    int cur;                        // 0x24
    char pad_0x28[0x44 - 0x28];
    int f44;                        // 0x44
    char pad_0x48[0x270 - 0x48];
    int f270;                       // 0x270
    int view;                       // 0x274
    char pad_0x278[0xE84 - 0x278];
    sDrawState_22B008* top;         // 0xE84
    char pad_0xE88[0xF50 - 0xE88];
    int tbl[(0x10A8 - 0xF50) / 4];  // 0xF50
    int f10A8;                      // 0x10A8
    int f10AC;                      // 0x10AC
    char pad_0x10B0[0x10D8 - 0x10B0];
    // vptr at 0x10D8; slot N at vtable offset N*8
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09();
    virtual unsigned int GetScreenWidth();
    virtual unsigned int GetScreenHeight();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual int IsRenderReady();
    virtual void v18(); virtual void v19();
    virtual void DrawFrame();
    virtual void v21(); virtual void v22();
    virtual void v23(float x, float y, float w, float h, int a);
    virtual void SetViewportWindow1(sRect_22B008* r, int a);
    virtual void SetViewportPerspective(float fov, float nearClip, float farClip);
    virtual void SetViewportOrthographic(float a, float b, float c, float d, float e, float f, int g, int h);
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33();
    virtual void SetView1(t4Matrix* m);
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual t4Matrix GetNetMatrix();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65();
    virtual void ClearProjTextures();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82();
    virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90();
    virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98();
    virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115();
    virtual void RenderFullScreenEffects(int view);
};


static inline int CurLayer_22B008(cGfx_22B008* g)
{
    return g->tbl[g->cur];
}

struct sCam_22B008 {
    float fov;              // 0x0
    float nearClip;         // 0x4
    float farClip;          // 0x8
    char pad_0xC[0x40 - 0xC];
    t4Matrix mat;        // 0x40
};

struct sViewMan_22B008 {
    unsigned int count;         // 0x0
    sCam_22B008* cams[3];       // 0x4
    unsigned int views;         // 0x10
    unsigned int cur;           // 0x14
    sRect_22B008 rects[2];      // 0x18
};

class cRenderer_22B008 {
public:
    char pad_0x0[0xC];
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04();
};

class cScene_22B008 {
public:
    char pad_0x0[0xCC];
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04();
};


struct sVEView_22B008 {
    short delta;
    short index;
    void (*fn)(void*, float, float, float, float, int);
};

struct sLoader_22B008 {
    sVEntry<int(void*, int)>* vt;       // 0x0
};

struct sHud_22B008 {
    char pad_0x0[0x28];
    sScale_22B008 scale;        // 0x28
    float f30;                  // 0x30
    float f34;                  // 0x34
    float f38;                  // 0x38
    float f3C;                  // 0x3C
    sColor_22B008 color;        // 0x40
};

struct sRenderTarget_22B008 {
    char pad_0x0[0x6490];
    unsigned int views;         // 0x6490
};

struct sWorld_22B008 {
    char data[0x81D0];
} __attribute__((aligned(16)));


// PORT: PS2-only VU0 inline asm (4x4 matrix copy through vf1..vf4).
static inline void MatCopy_22B008(t4Matrix* dst, const t4Matrix* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector operator+(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector operator-(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector operator*(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// Grows the box to contain c.
static inline void Grow_22B008(sBox_22B008& b, const t4Vector& c)
{
    if (c.x < b.min.x)
        b.min.x = c.x;
    if (c.y < b.min.y)
        b.min.y = c.y;
    if (c.z < b.min.z)
        b.min.z = c.z;
    if (c.x > b.max.x)
        b.max.x = c.x;
    if (c.y > b.max.y)
        b.max.y = c.y;
    if (c.z > b.max.z)
        b.max.z = c.z;
}

static inline int IsReady_22B008(cGame* self)
{
    cGameViewMan_updateStreamFocus(((sViewMan_22B008*)self->ViewMan));
    if (self->IsStreamPaused != 0) {
        return 0;
    }
    if (cGameViewMan_isStreamReady(((sViewMan_22B008*)self->ViewMan)) == 0) {
        self->IsStreamPaused = 1;
        self->m_bFlushStreamer = 1;
        return 0;
    }
    return 1;
}

/**
 * Draws one game frame: every camera view of the world, then the 2D overlays, UI and audio tick.
 *
 * Returns false when GrMan's slot 17 returns 0; returns true early while StreamPause (slot 2) holds the frame, while
 * the fade screen is fully faded (cFadeScreen_drawFade) or while a script plays (func_0022C830 draws the cut scene).
 * For each ViewMan view: sets the view, viewport (slot 24), camera matrix (slot 34) and projection (slot 25), draws
 * SkyBox (slot 4) with a near plane of 1.0, then the real near / far planes and render states; builds the world-space
 * box of the view frustum's eight corners (each corner sum, camera position included, is scaled by the near or far
 * distance), gathers the visible world (FrustumIntersector_FrustumIntersector with BXNash, OcclusionMan and World), and
 * draws cGame_renderBuckets, CrowdMan2d, CrowdMan3d, MultiParticleMan, the AI (riders, slot 4), models, fog volumes,
 * light halos and patches. Then a 640x480 2D pass: DebugFont at 0.8 scale, MenuSys when active, the UI engine and
 * WScriptMan (unless cRenderToggles_m_DisableHUD), the script engine, LoadIndicator, GrMan slot 20, the audio update
 * for mUpdateTick
 * / 60 seconds and the world triggers; clears mUpdateTick. The local view's names are off: scene is AI, renderer
 * SkyBox, target LensFxMan, cfg BXNash, hud DebugFont, loader StreamPause, frames mUpdateTick.
 *
 * @return false when GrMan's slot 17 returns 0, otherwise true
 */
bool cGame::render()
{
    if (((cGfx_22B008*)GrMan)->IsRenderReady() == 0) {
        return false;
    }
    ((cGfx_22B008*)GrMan)->RenderFullScreenEffects(-1);
    {
        sLoader_22B008* streamPause = ((sLoader_22B008*)StreamPause);
        sVEntry<int(void*, int)>* vt = streamPause->vt;
        char* thisp = (char*)streamPause + vt[2].delta;
        int (**pfn)(void*, int) = &vt[2].fn;
        if ((*pfn)(thisp, IsReady_22B008(this)) == 0) {
            return true;
        }
    }
    if (cFadeScreen_drawFade(this) != 0) {
        return true;
    }
    if (ScriptEngine->mIsPlaying != 0) {
        func_0022C830(this);
        return true;
    }
    cGarbageMan_Update();
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    ((cGfx_22B008*)ps2GraphicsMan)->f10A8 = ((cGfx_22B008*)ps2GraphicsMan)->tbl[((cGfx_22B008*)ps2GraphicsMan)->cur];
    ((cGfx_22B008*)ps2GraphicsMan)->f10AC = ((cGfx_22B008*)ps2GraphicsMan)->tbl[((cGfx_22B008*)ps2GraphicsMan)->f44];
    ((sRenderTarget_22B008*)LensFxMan)->views = ((sViewMan_22B008*)ViewMan)->views;
    ((cGfx_22B008*)ps2GraphicsMan)->f270 = ((sViewMan_22B008*)ViewMan)->views;
    sWorld_22B008* world = 0;
    for (unsigned int i = 0; i < ((sViewMan_22B008*)ViewMan)->views; i++) {
        ((cGfx_22B008*)GrMan)->view = i;
        ((cGfx_22B008*)GrMan)->ClearProjTextures();
        ((sViewMan_22B008*)ViewMan)->cur = i;
        sCam_22B008* cam = ((sViewMan_22B008*)ViewMan)->cams[((sViewMan_22B008*)ViewMan)->cur];
        ((cGfx_22B008*)GrMan)->SetViewportWindow1(&((sViewMan_22B008*)ViewMan)->rects[i], 1);
        ((cGfx_22B008*)GrMan)->SetView1(&cam->mat);
        ((cGfx_22B008*)GrMan)->SetViewportPerspective(cam->fov, 1.0f, cam->farClip);
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFE7FFFFF) | 0x800000;
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFFCFFFFF) | 0x300000;
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFFF00FFF) | 0xD000;
        ((cRenderer_22B008*)SkyBox)->v04();
        ((cGfx_22B008*)GrMan)->SetViewportPerspective(cam->fov, cam->nearClip, cam->farClip);
        ((cGfx_22B008*)GrMan)->RenderFullScreenEffects(i);
        ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x80;
        *(short*)((char*)((cGfx_22B008*)GrMan)->top + 0x12) = CurLayer_22B008((cGfx_22B008*)GrMan);
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & ~0xF80) | 0x380;
        ((cGfx_22B008*)GrMan)->top->f0 |= 0x30;
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFE7FFFFF) | 0x800000;
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFFCFFFFF) | 0x300000;
        ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & 0xFFF00FFF) | 0xD000;
        ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x80;
        sBox_22B008 box;
        t4Matrix view = ((cGfx_22B008*)GrMan)->GetNetMatrix();
        float nearClip = cam->nearClip;
        float farClip = cam->farClip;
        t4Matrix inv;
        MatCopy_22B008(&inv, &view);
        t4mat_invert(&inv);
        t4Matrix invView = inv;
        sBox_22B008& bounds = box;
        const t4Vector& right = invView.row[0];
        const t4Vector& up = invView.row[1];
        const t4Vector& front = invView.row[2];
        const t4Vector& pos = invView.row[3];
        t4Vector corner;
        corner = (pos - front + right + up) * nearClip;
        corner.w = 1.0f;
        bounds.min = corner;
        bounds.max = corner;
        corner = (pos - front + right - up) * nearClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos - front - right + up) * nearClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos - front - right - up) * nearClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos + front - right - up) * farClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos + front - right + up) * farClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos + front + right - up) * farClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        corner = (pos + front + right + up) * farClip;
        corner.w = 1.0f;
        Grow_22B008(bounds, corner);
        sWorld_22B008 worldData;
        world = &worldData;
        // sBox_22B008 has tBoundBox's layout (MinPoint, MaxPoint)
        FrustumIntersector_FrustumIntersector((FrustumIntersector*)world, BXNash, OcclusionMan,
                                              (const tBoundBox*)&box, World);
        cGame_renderBuckets(this);
        func_00229BA8((cCrowdMan2d*)((void*)CrowdMan2d));
        func_00344138(((void*)CrowdMan3d));
        cMultiParticleMan_render(((void*)MultiParticleMan));
        ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x80;
        ((cScene_22B008*)AI)->v04();
        cGame_renderModels(this, world);
        cGame_renderFogVolumes(this, world);
        cGame_renderLightHalos(this, world, i);
        ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x40;
        cGame_renderPatches(this, world);
    }
    {
        sVEView_22B008* vt = (sVEView_22B008*)VPTR_OF(cGraphicsMan, GrMan);
        char* thisp = (char*)GrMan + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_22B008*)GrMan)->GetScreenWidth(), (float)((cGfx_22B008*)GrMan)->GetScreenHeight(), 1);
    }
    ((cGfx_22B008*)GrMan)->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0, 0);
    ((cGfx_22B008*)GrMan)->SetView1(&t4mat_Identity_22B008);
    ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x300;
    ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & ~0x7C) | 0x14;
    sHud_22B008* hud = ((sHud_22B008*)DebugFont);
    hud->f38 = hud->f30 * 0.8f;
    hud->f3C = hud->f34 * 0.8f;
    ((sHud_22B008*)DebugFont)->scale = sScale_22B008(2.0f, 2.0f);
    ((sHud_22B008*)DebugFont)->color = sColor_22B008(1.0f, 0.8f, 0.8f, 0.0f);
    if (cMenuSys_isActive(((void*)MenuSys)) != 0) {
        cMenuSys_render(((void*)MenuSys));
        ((sHud_22B008*)DebugFont)->scale = sScale_22B008(2.0f, 2.0f);
        ((sHud_22B008*)DebugFont)->color = sColor_22B008(1.0f, 0.8f, 0.8f, 0.0f);
    }
    *((cGfx_22B008*)GrMan)->top = sDrawState_s_defaultDrawState_22B008;
    ((cGfx_22B008*)GrMan)->top->f8 = (((cGfx_22B008*)GrMan)->top->f8 & ~0x3E0) | 0x300;
    ((cGfx_22B008*)GrMan)->top->f4 = (((cGfx_22B008*)GrMan)->top->f4 & ~0x7C) | 0x14;
    ((sHud_22B008*)DebugFont)->scale = sScale_22B008(2.0f, 2.0f);
    ((sHud_22B008*)DebugFont)->color = sColor_22B008(1.0f, 0.8f, 0.8f, 0.0f);
    if (cRenderToggles_m_DisableHUD == 0) {
        ((sHud_22B008*)DebugFont)->scale = sScale_22B008(0.0f, 0.0f);
        cUIEngine_render2D(((void*)UI));
        cWScriptMan_render(((void*)WScriptMan));
    }
    cSSXScriptEngine_Render(ScriptEngine);
    cBXLoadIndicator_Render(((void*)LoadIndicator));
    ((cGfx_22B008*)GrMan)->DrawFrame();
    cSSXAudio_Update(gSSXAudio, (float)mUpdateTick * (1.0f / 60.0f), 1);
    cWorldTriggerManager_Update(gWorldTriggerManager);
    mUpdateTick = 0;
    return true;
}
#endif

