#include "common.h"
#include "be/beinterface.h"
#include "ai/missionrider.h"
#include "ai/ai.h"
#include "vtable.h"
#include "main/gameviewman.h"
#include "replay/frame.h"
#include "replay/replaycache.h"
#include "replay/playbackman.h"
#include "replay/replay.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "replay/bxreplayframe.h"
#include "ai/player.h"
#include "input/input.h"
#include "input/inputpad.h"
#include "object/instancenode.h"
#include "world/instancerecord.h"

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_cPlaybackMan);
#ifdef SKIP_ASM
struct cBigFile;
cBigFile* cBigFile_cBigFile(cBigFile* self);
extern "C" int cBigFile_open(cBigFile* self, const char* name, int memtype);
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
// PORT: the second argument is a pointer passed as int (mangled FPvi).
void cInput_setPad(void* self, int buf);
void cInput_setMap(void* self, int n);
extern "C" void* cReplayFrame_cReplayFrame(cReplayFrame* self);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char* kPlaybackBigFmt;
extern int kPlaybackPath;
extern char str_cInputPad[];
extern char str_cInput[];
extern char str_BigFile_b[];


/**
 * Constructs the mission playback manager and opens the playback archive.
 *
 * Builds the embedded replay frame at +0x14 (cReplayFrame_cReplayFrame), keeps SSXApp->Game (+0x5C) and its AI (+0x60),
 * clears the state words and sets the current entry (+0x74) and async handle (+0x78) to -1. From player 1's
 * input (AI->Players[0]->Input) it keeps the first word at +0x64 and builds an 8-byte "cInput" (+0x6C)
 * holding a new 0x2A4-byte "cInputPad" block (+0x68) and the input's second word. Finally opens
 * "|data\missions\playback.big" through a new cBigFile (+0x0).
 *
 * @param maxPlaybackFiles  number of playback-file entries cPlaybackMan_initLocation() allocates (cGame
 *                          passes 40)
 * @return @p self
 */
extern "C" cPlaybackMan* cPlaybackMan_cPlaybackMan(cPlaybackMan* self, int maxPlaybackFiles)
{
    cReplayFrame_cReplayFrame((cReplayFrame*)&self->SaveFramePtr);
    cGame* world = SSXApp->Game;
    self->Game = world;
    cAI* level = world->AI;
    self->AI = level;
    self->AsyncFileNum = -1;
    self->MaxPlaybackFiles = maxPlaybackFiles;
    self->AsyncHandle = -1;
    self->SaveFrame = 0;
    self->SaveFrameData = 0;
    self->CurrentFrame = 0;
    self->NumPlaybackFiles = 0;
    self->PlaybackFiles = 0;
    self->State = 0;
    cInput* playerInput = level->Players[0]->Input;
    cInputMap* map = playerInput->Map;
    self->RealInputPad = playerInput->Pad;
    self->InputPad = (cInputPad*)cMemMan_alloc(676, str_cInputPad, 0x20000000, 0);
    self->InputPad->NumButtons = 0;
    cInput* replayStream = (cInput*)cMemMan_alloc(8, str_cInput, 0x20000000, 0);
    cInput_setPad(replayStream, (int)self->InputPad);
    cInput_setMap(replayStream, (int)map);
    self->Input = replayStream;
    char name[0x100];
    sprintf(name, kPlaybackBigFmt, kPlaybackPath);
    cBigFile* bf = cBigFile_cBigFile((cBigFile*)cMemMan_alloc(8, str_BigFile_b, 0, 0));
    self->BigFile = bf;
    cBigFile_open(bf, name, 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan__cPlaybackMan);
#ifdef SKIP_ASM
struct cBigFile;
void cBigFile__cBigFile(cBigFile* self, int flags);
void operator_delete(int* ptr);
extern "C" void cPlaybackMan_purgeLocation(cPlaybackMan* self);
extern void* cReplayFramePtr_vtbl[];

/**
 * Destroys the playback manager: frees its big file, input pads and location data, then itself if asked.
 *
 * Destroys BigFile (+0x0) with cBigFile__cBigFile(bf, 3), deletes the playback input objects at +0x6C and +0x68 (+0x68
 * is the pad cPlaybackMan_isPauseAI feeds), purges the location (cPlaybackMan_purgeLocation), restores
 * cReplayFramePtr's vtable on the saved frame (+0x14, the inlined frame destructor) and calls operator_delete on @p
 * flags bit 0 (cGame_purge passes 3).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPlaybackMan__cPlaybackMan(cPlaybackMan* self, int flags)
{
    cBigFile* bf = *(cBigFile**)self;
    if (bf != 0) {
        cBigFile__cBigFile(bf, 3);
    }
    operator_delete((int*)self->Input);
    operator_delete((int*)self->InputPad);
    cPlaybackMan_purgeLocation(self);
    self->SaveFramePtr.mVtbl = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_update);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" int ASYNCFILE_release(int handle, void** data, int* size);
extern "C" void MEM_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern const char str_PlaybackFile[];
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

/**
 * Finishes a pending playback-file load by copying the file into the current entry.
 *
 * When the async request (AsyncHandle) has completed (ASYNCFILE_getstatus() == 1), releases it, copies a non-empty
 * result into a new "PlaybackFile" buffer stored with its size in entries[cur], frees the loaded buffer and
 * sets the handle to -1. cPlaybackMan_load starts these loads.
 */
extern "C" void cPlaybackMan_update(cPlaybackMan* self)
{
    if (self->AsyncHandle >= 0 && ASYNCFILE_getstatus(self->AsyncHandle) == 1) {
        void* data = 0;
        int size = 0;
        ASYNCFILE_release(self->AsyncHandle, &data, &size);
        if (data != 0 && size > 0) {
            // PORT: pointer held in int for the entry address (index-first addu); not 64-bit safe. A typed
            // &PlaybackFiles[AsyncFileNum] in a local adds base-first.
            tPlaybackFile* curEntry = (tPlaybackFile*)(self->AsyncFileNum * 12 + (int)self->PlaybackFiles);
            curEntry->PackData = (char*)operator_new_tag(size, str_PlaybackFile, 0x20000000, 0);
            self->PlaybackFiles[self->AsyncFileNum].Size = size;
            MEM_copy(self->PlaybackFiles[self->AsyncFileNum].PackData, data, size);
            MEM_free(data);
        }
        self->AsyncHandle = -1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_initLocation);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_C6E8(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cReplayFrame_clearFrame(cReplayFrame* self);
extern "C" void cReplayFramePtr_setBlock(void* self, void* buf, int a2);
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" int cBEMissionInterface_getNumberMissions(void* iface);
extern "C" unsigned int cBEMissionInterface_getMissionID(void* iface, int i);
extern char str_SaveFrameData[];
extern char str_CurrentFrame[];
extern char str_tPlaybackFile[];

/**
 * Prepares the playback manager for a location: frame buffers and one playback-file entry per mission.
 *
 * Copies @p name to Location, resets the counters, allocates the 0x14000-byte "SaveFrameData" buffer and attaches
 * it to the embedded replay frame, allocates the zeroed 0x1C22C-byte "CurrentFrame" block (Version 0xDBAC0101,
 * Buffer.Size 0x1C200) and MaxPlaybackFiles "tPlaybackFile" entries (Id -1, no data), then gives the first
 * cBEMissionInterface_getNumberMissions() entries their mission ids (cBEMissionInterface_getMissionID). The mission count is not
 * checked against the number of entries.
 *
 * @param name  location name (cWScriptMan_setupMissions passes the BE location record + 0x34)
 */
extern "C" void cPlaybackMan_initLocation(cPlaybackMan* self, const char* name)
{
    strcpy(self->Location, name);
    self->SaveFrame = 0;
    self->AsyncFileNum = -1;
    self->AsyncHandle = -1;
    self->SaveFrameData = (char*)operator_new_tag(0x14000, str_SaveFrameData, 0, 0);
    cReplayFrame_clearFrame((cReplayFrame*)(&self->SaveFramePtr));
    cReplayFramePtr_setBlock(&self->SaveFramePtr, ((void*)self->SaveFrameData), 0);
    tPlaybackFrame* frame = (tPlaybackFrame*)cMemMan_alloc(0x1C22C, str_CurrentFrame, 0, 0);
    self->CurrentFrame = frame;
    MEM_fill(frame, 0, 0x1C22C);
    int n = self->MaxPlaybackFiles;
    frame->Version = 0xDBAC0101;
    frame->Buffer.Size = 0x1C200;
    self->PlaybackFiles = (tPlaybackFile*)operator_new_tag(n * 12, str_tPlaybackFile, 0x20000000, 0);
    for (int i = 0; i < self->MaxPlaybackFiles; i++) {
        self->PlaybackFiles[i].Id = 0xFFFFFFFF;
        self->PlaybackFiles[i].PackData = 0;
        self->PlaybackFiles[i].Size = 0;
    }
    void* iface = cBE_getInterface_C6E8(cBE_getBE(), 10);
    self->NumPlaybackFiles = cBEMissionInterface_getNumberMissions(iface);
    for (int j = 0; j < self->NumPlaybackFiles; j++)
        self->PlaybackFiles[j].Id = cBEMissionInterface_getMissionID(iface, j);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_purgeLocation);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void ASYNCFILE_cancel(int handle);



/**
 * Releases everything the current playback location loaded.
 *
 * Cancels the pending async read (+0x78, ASYNCFILE_cancel) and resets the handle to -1; frees each playback file's data
 * and clears its state (table +0x58, count +0x50), then the table; frees the frame buffer (+0x4C); clears +0x10 and
 * deletes the input stream (+0x4); clears +0x70. Called by the destructor and by cWScriptMan_setupMissions.
 *
 * @param playbackMan  the playback manager (this)
 */
extern "C" void cPlaybackMan_purgeLocation(cPlaybackMan* playbackMan)
{
    cPlaybackMan* self = playbackMan;
    if (self->AsyncHandle != -1) {
        ASYNCFILE_cancel(self->AsyncHandle);
        self->AsyncHandle = -1;
    }
    if (self->PlaybackFiles != 0) {
        for (int i = 0; i < self->NumPlaybackFiles; i++) {
            if (((void*)(self->PlaybackFiles)[i].PackData) != 0) {
                cMemMan_free(((void*)(self->PlaybackFiles)[i].PackData));
                (*(void**)&(self->PlaybackFiles)[i].PackData) = 0;
            }
            (self->PlaybackFiles)[i].Size = 0;
        }
        if (self->PlaybackFiles != 0) {
            cMemMan_free(self->PlaybackFiles);
        }
        *(tPlaybackFile**)&self->PlaybackFiles = 0;
    }
    if (((void*)self->SaveFrameData) != 0) {
        cMemMan_free(((void*)self->SaveFrameData));
        *(void**)&self->SaveFrameData = 0;
    }
    self->SaveFrame = 0;
    if (((int*)self->CurrentFrame) != 0) {
        operator_delete(((int*)self->CurrentFrame));
        *(int**)&self->CurrentFrame = 0;
    }
    self->State = 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_isLoaded);
#ifdef SKIP_ASM
extern "C" int cPlaybackMan_findFileById();

/**
 * Returns whether playback file @p fileId is ready to use: 1 when it is listed and its load has finished.
 *
 * Looks the id up with cPlaybackMan_findFileById (declared without arguments here: self and @p fileId pass through in registers). The
 * file counts as ready when its entry's data pointer (+0x4) is set, or when no async load is pending (handle +0x78
 * negative), so a failed load also reports ready. An unknown id gives 0.
 *
 * @param fileId  the playback file id
 * @return 1 when the file is loaded or no load is pending, 0 otherwise
 */
extern "C" int cPlaybackMan_isLoaded(cPlaybackMan* self, int fileId)
{
    int idx = cPlaybackMan_findFileById();
    if (idx >= 0) {
        if ((int)self->PlaybackFiles[idx].PackData != 0)
            return 1;
        if (self->AsyncHandle < 0)
            return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int sprintf(char* buf, const char* fmt, ...);

extern int kPlaybackPath;

extern "C" int cPlaybackMan_findFileById_C9F0(cPlaybackMan* self, int key) __asm__("cPlaybackMan_findFileById");

extern "C" int ASYNCFILE_load(char* path, int flags);
extern char* D_004A3414;
extern char str_bar_c[];

/**
 * Starts loading playback file @p key asynchronously; returns 1 when it is already loaded or the load has started.
 *
 * Looks the id up with cPlaybackMan_findFileById. A listed file whose data pointer (PackData) is set needs nothing.
 * Otherwise the entry index becomes the current async file (AsyncFileNum), the path is built with sprintf from the
 * format at D_004A3414, str_bar_c, kPlaybackPath, the location name (Location) and the file id, and
 * ASYNCFILE_load(path, 0x100) is started; its handle is kept in AsyncHandle (-1 on failure). cPlaybackMan_update
 * copies the finished file into the entry.
 *
 * @param key  the playback file id
 * @return 1 when loaded or the load started, 0 for an unknown id or a failed start
 */
extern "C" int cPlaybackMan_load(cPlaybackMan* self, int key)
{
    char path[0x100];
    int idx = cPlaybackMan_findFileById_C9F0(self, key);
    if (idx >= 0) {
        if (self->PlaybackFiles[idx].PackData != 0)
            return 1;
        self->AsyncFileNum = idx;
        self->AsyncHandle = -1;
        int num = kPlaybackPath;
        sprintf(path, D_004A3414, str_bar_c, num, self->Location, self->PlaybackFiles[idx].Id);
        int asyncHandle = ASYNCFILE_load(path, 0x100);
        self->AsyncHandle = asyncHandle;
        if (asyncHandle >= 0)
            return 1;
        self->AsyncHandle = -1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_restart);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stop(cPlaybackMan* self);
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Restarts the playback manager: ends the active playback mode and clears the CurrentFrame buffer.
 *
 * cPlaybackMan_stop ends playback mode 1 or 2; then the words at +0x10 and +0x70 (the mode) are zeroed. If the
 * CurrentFrame buffer at +0x4 exists, its 0x1C22C bytes are cleared and its header rewritten (0xDBAC0101,
 * 0x1C200), as cPlaybackMan_initLocation() does. cGame_resetWorld() and cGame_partialRestart() call it.
 */
extern "C" void cPlaybackMan_restart(cPlaybackMan* self)
{
    cPlaybackMan_stop(self);
    unsigned int* buf = (unsigned int*)self->CurrentFrame;
    self->SaveFrame = 0;
    self->State = 0;
    if (buf != 0) {
        MEM_fill(buf, 0, 0x1C22C);
        buf[0] = 0xDBAC0101;
        buf[1] = 0x1C200;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cPlaybackMan_setupPreview);
#ifdef SKIP_ASM
extern "C" void SYNCTASK_run(int);
extern "C" void THREAD_yieldticks(int);
extern "C" int cPlaybackMan_isLoaded(cPlaybackMan* self, int a1);
extern "C" int cPlaybackMan_load(cPlaybackMan* self, int a1);
extern "C" int cPlaybackMan_findFileById();
extern "C" int cPlaybackMan_setupFile(cPlaybackMan* self, int idx);

/**
 * Loads playback file @p fileId and unpacks it into the current frame, waiting for the load; returns 1 if it holds input.
 *
 * Only while the manager is idle (mode +0x70 == 0) and the id is listed (cPlaybackMan_findFileById): starts the load
 * (cPlaybackMan_load), then blocks until cPlaybackMan_isLoaded reports it ready, running cPlaybackMan_update, SYNCTASK_run(0) and
 * THREAD_yieldticks(1) each pass. cPlaybackMan_setupFile then unpacks the file into the current frame (+0x4). cWScriptMan_previewLocalMission
 * (wscript) calls it before starting a preview.
 *
 * @param fileId  the playback file id
 * @return 1 when the file was unpacked and its recorded input end (current frame +0xC) is above 0, else 0
 */
extern "C" int cPlaybackMan_setupPreview(cPlaybackMan* self, int fileId)
{
    int idx;
    if (self->State == 0 && (idx = cPlaybackMan_findFileById()) >= 0 && cPlaybackMan_load(self, fileId) != 0) {
        while (cPlaybackMan_isLoaded(self, fileId) == 0) {
            cPlaybackMan_update(self);
            SYNCTASK_run(0);
            THREAD_yieldticks(1);
        }
        if (cPlaybackMan_setupFile(self, idx) != 0) {
            if (self->CurrentFrame->Buffer.IndexMax > 0)
                return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_reset__FPv);
#ifdef SKIP_ASM
/**
 * Forgets the saved replay frame.
 *
 * Clears the saved-frame flag (+0x10) that cPlaybackMan_saveFrame sets and cPlaybackMan_restoreFrame tests. The mission
 * scripts call it after cPlaybackMan_stop when a local mission is cancelled or quit.
 */
void cPlaybackMan_reset(void* vself)
{
    cPlaybackMan* self = (cPlaybackMan*)vself;
    self->SaveFrame = 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_saveFrame);
#ifdef SKIP_ASM
struct sPbBytes4;
extern "C" void cReplay_saveFrame1(cReplay* self, cReplayFrame* out, sPbBytes4* src);


/**
 * Saves the replay's current state into the playback manager's frame, once.
 *
 * Unless the saved-frame flag (+0x10) is already set, calls cReplay_saveFrame1 (On Tour cReplay::saveFrame(frame, type)) on
 * Game->Replay (+0x5C, +0x28) with the frame at +0x14 and a type-1 descriptor, then sets the flag.
 */
extern "C" void cPlaybackMan_saveFrame(cPlaybackMan* self)
{
    if (self->SaveFrame == 0) {
        int one = 1;
        tReplayFrameType desc;
        desc.Type = one;
        desc.Highlights = 0;
        desc.Chapter = 0;
        cReplay_saveFrame1((cReplay*)(self->Game->Replay), (cReplayFrame*)((char*)&self->SaveFramePtr),
                      (sPbBytes4*)&desc);
        self->SaveFrame = one;
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_restoreFrame);
#ifdef SKIP_ASM
// PORT: cReplay_restoreFrame1 is declared in this unit with one parameter, but it passes a
// second one ($5) through to cReplay_restoreFrame; bind the 2-arg form to the symbol.
extern "C" void cReplay_restoreFrame1_2(void* replay, void* frame) __asm__("cReplay_restoreFrame1");

/**
 * Restores the replay from the playback manager's saved frame, if one was saved.
 *
 * When the saved-frame flag (+0x10) is set, calls cReplay_restoreFrame1 on Game->Replay with the frame at +0x14. The
 * flag stays set (cPlaybackMan_reset clears it).
 *
 * @return nothing meaningful: declared void* in this unit, the body returns no value
 */
extern "C" void* cPlaybackMan_restoreFrame(cPlaybackMan* self)
{
    if (self->SaveFrame) {
        cReplay_restoreFrame1_2(self->Game->Replay, (char*)&self->SaveFramePtr);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cPlaybackMan_startPreview);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void*);
extern "C" void cAI_reinitMissionRiders(void* self);
extern "C" void cCamera_switchChaseControllerTargetRider(void* self, int value);
void cReplay_setReplayState(void* self, int val);
extern "C" void cPlaybackMan_saveFrame(cPlaybackMan* self);
void AIsetseed(void* self);

/**
 * Starts previewing a recorded run: resets the mission riders, saves the replay state and plays the recording.
 *
 * Only in mode 0 (+0x70) with recorded input (input buffer +0x4, end +0xC > 0): purges and reinits the AI's (+0x60)
 * mission riders, saves the replay State into SaveState and sets state 14, saves the frame (cPlaybackMan_saveFrame),
 * rewinds the input buffer (+0x8 = 0), reseeds the AI random generator from the recording (AIsetseed__FPv on buffer +
 * 0x1C214; AI + 0x8 = the word at buffer + 0x1C210), sets mode 1 and switches the chase camera's target
 * (cCamera_switchChaseControllerTargetRider) to the AI's mission rider (+0x70).
 */
extern "C" void cPlaybackMan_startPreview(cPlaybackMan* self)
{
    if (self->State == 0 && self->CurrentFrame->Buffer.IndexMax > 0) {
        cAI_purgeMissionRiders(self->AI);
        cAI_reinitMissionRiders(self->AI);
        cReplay* replay = self->Game->Replay;
        replay->SaveState = *(int*)replay;
        cReplay_setReplayState(replay, 14);
        cPlaybackMan_saveFrame(self);
        self->CurrentFrame->Buffer.Index = 0;
        AIsetseed(&self->CurrentFrame->AISeed[0]);
        self->AI->AITick = self->CurrentFrame->AITick;
        self->State = 1;
        cMissionRider* x = self->AI->MissionRiders[0];
        int targetRider = x ? (int)x->mRider : 0;
        cCamera_switchChaseControllerTargetRider(self->Game->ViewMan->PlayerCamera[0], targetRider);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cPlaybackMan_stopPreview);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void*);
extern "C" void cAI_reinitMissionRiders(void* self);
extern "C" void cCamera_switchChaseControllerTargetRider(void* self, int value);
void cReplay_setReplayState(void* self, int val);

/**
 * Ends a preview: resets the mission riders, restores the replay state and frame, and refocuses the camera.
 *
 * Only in mode 1 (+0x70): purges and reinits the AI's mission riders, restores the replay state from SaveState,
 * restores the frame (cPlaybackMan_restoreFrame), sets mode 0 and switches the chase camera back to the first rider (AI
 * + 0x28).
 */
extern "C" void cPlaybackMan_stopPreview(cPlaybackMan* self)
{
    if (self->State == 1) {
        cAI_purgeMissionRiders(self->AI);
        cAI_reinitMissionRiders(self->AI);
        cReplay* replay = self->Game->Replay;
        cReplay_setReplayState(replay, replay->SaveState);
        cPlaybackMan_restoreFrame(self);
        self->State = 0;
        int targetRider = (int)self->AI->Riders[0];
        cCamera_switchChaseControllerTargetRider(self->Game->ViewMan->PlayerCamera[0], targetRider);
    }
}
#endif

extern "C" void* cPlaybackMan_restoreFrame(cPlaybackMan*);

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_startPerform);
#ifdef SKIP_ASM
/**
 * Starts a performance: saves the replay frame, rewinds the recorded input and sets the playback mode to 2.
 *
 * Calls cPlaybackMan_saveFrame, sets the current frame's input position (+0x8) to 0 and the mode (+0x70) to 2, which
 * cPlaybackMan_stop ends with cPlaybackMan_stopPerform. Unlike cPlaybackMan_startPreview it checks no mode and does not
 * reseed the AI. No retail caller was found.
 */
extern "C" void cPlaybackMan_startPerform(cPlaybackMan* self)
{
    cPlaybackMan_saveFrame(self);
    self->CurrentFrame->Buffer.Index = 0;
    self->State = 2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cPlaybackMan_stopPerform__FPv);
#ifdef SKIP_ASM
/**
 * Ends a performance: restores the saved replay frame and clears the playback mode.
 *
 * Calls cPlaybackMan_restoreFrame, then sets the mode (+0x70) to 0. cPlaybackMan_stop uses it for mode 2.
 */
void cPlaybackMan_stopPerform(void* vself)
{
    cPlaybackMan* self = (cPlaybackMan*)vself;
    cPlaybackMan_restoreFrame((cPlaybackMan*)self);
    self->State = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cPlaybackMan_stop);
#ifdef SKIP_ASM
extern "C" void cPlaybackMan_stopPreview(cPlaybackMan* self);
void cPlaybackMan_stopPerform(void* self);

/**
 * Stops whatever the playback manager is doing.
 *
 * Mode (+0x70) 1 calls cPlaybackMan_stopPreview, mode 2 cPlaybackMan_stopPerform; other modes do nothing. Called by
 * cPlaybackMan_restart and the mission scripts.
 */
extern "C" void cPlaybackMan_stop(cPlaybackMan* self)
{
    switch (self->State) {
    case 0:
    default:
        break;
    case 1:
        cPlaybackMan_stopPreview(self);
        break;
    case 2:
        cPlaybackMan_stopPerform(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_isPauseAI);
#ifdef SKIP_ASM
extern "C" void cInputPad_update(void* self, int n, float* vals);


/**
 * Steps the recorded pad input by one frame and says whether the AI should pause.
 *
 * Mode 1 (playback): reads one packed frame from CurrentFrame's input buffer (a count byte, then one byte per value,
 * scaled by 1/255) and feeds it to the playback pad (InputPad) with cInputPad_update (On Tour cInputPad::update).
 * Mode 3 (record): packs the live pad's button values (RealInputPad) into the buffer the same way and feeds the
 * quantised values to the playback pad. cGame_update pauses the AI while it returns nonzero.
 *
 * @return 1 when the buffer is exhausted (mode 1) or full (mode 3), else 0
 */
// Replay channel step: mode 1 plays back one packed frame of axis values, mode 3 records one.
// Returns 1 when the buffer is exhausted (or full), 0 otherwise.
extern "C" int cPlaybackMan_isPauseAI(cPlaybackMan* self)
{
    float vals[24];
    if (self->State == 1) {
        tPlaybackFrame* buf = self->CurrentFrame;
        if (buf->Buffer.Index < buf->Buffer.IndexMax) {
            unsigned char* cursor = &buf->Buffer.Buffer[buf->Buffer.Index];
            int n = *cursor++;
            for (int i = 0; i < n; i++)
                vals[i] = *cursor++ * 0.003921568859368563f;
            self->CurrentFrame->Buffer.Index += 1 + n;
            cInputPad_update(self->InputPad, n, vals);
            return 0;
        } else {
            return 1;
        }
    } else if (self->State == 3) {
        tPlaybackFrame* buf = self->CurrentFrame;
        if (buf->Buffer.Index < buf->Buffer.Size - 0x18) {
            unsigned char* cursor = &buf->Buffer.Buffer[buf->Buffer.Index];
            int n = self->RealInputPad->NumButtons;
            *cursor++ = n;
            for (int i = 0; i < n; i++) {
                cInputPad* axes = self->RealInputPad;
                float axisValue;
                if (i < axes->NumButtons)
                    axisValue = axes->Buttons[i].Value;
                else
                    axisValue = 0.0f;
                *cursor = (int)(axisValue * 255.0f);
                vals[i] = *cursor++ * 0.003921568859368563f;
            }
            self->CurrentFrame->Buffer.Index += 1 + n;
            self->CurrentFrame->Buffer.IndexMax += 1 + n;
            cInputPad_update(self->InputPad, n, vals);
            return 0;
        } else {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_findFileById);
#ifdef SKIP_ASM

/**
 * Returns the index of the playback file whose id is @p id, or -1.
 *
 * Searches the first NumPlaybackFiles (+0x50) 12-byte entries of the playback file table (+0x58), comparing each
 * entry's first word.
 *
 * @param id  the playback file id
 * @return the entry index, or -1 when no entry has that id
 */
extern "C" int cPlaybackMan_findFileById(cPlaybackMan* self, int id)
{
    int count = self->NumPlaybackFiles;
    for (int i = 0; i < count; i++) {
        tPlaybackFile *entries = self->PlaybackFiles;
        if (((int)entries[i].Id) == id) {
            return i;
        }
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_setupFile);
#ifdef SKIP_ASM
extern "C" int HUFF_decode(void* src, int a, int dst);
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Unpacks playback file entry @p idx into the current frame (CurrentFrame), or clears that frame when the file has
 * no data.
 *
 * With data (the entry's PackData) it is decoded into the frame with HUFF_decode and the result is 1. Otherwise the
 * 0x1C22C-byte frame is zeroed, its Version set to 0xDBAC0101 and its input buffer size (Buffer.Size) to 0x1C200, and
 * the result is 0.
 *
 * @param idx  the index in the playback file table (PlaybackFiles)
 * @return 1 when the file's data was unpacked, 0 when the frame was reset instead
 */
extern "C" int cPlaybackMan_setupFile(cPlaybackMan* self, int idx)
{
    char* data = self->PlaybackFiles[idx].PackData;
    if (data != 0) {
        HUFF_decode(data, 0, (int)self->CurrentFrame);
        return 1;
    }
    tPlaybackFrame* buf = self->CurrentFrame;
    MEM_fill(buf, 0, 0x1C22C);
    buf->Version = 0xDBAC0101;
    buf->Buffer.Size = 0x1C200;
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_cReplayCache);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_CmdFrame[];
void cReplayCache_reset(void* self);

/**
 * Constructs a replay input cache: allocates its 0x8000-byte command buffer ("CmdFrame", +0xC) and resets it.
 *
 * The buffer holds up to 0x1000 8-byte command chunks. cReplayCache_reset then zeroes the chunk count and read position.
 * cReplay_addCache calls it on a new 0x10-byte "ReplayCache" block.
 *
 * @return @p self
 */
extern "C" void* cReplayCache_cReplayCache(cReplayCache* self)
{
    self->CmdFrame = (tControlCmd*)operator_new_tag(0x8000, str_CmdFrame, 0, 0);
    cReplayCache_reset(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache__cReplayCache);
#ifdef SKIP_ASM
extern "C" void cReplayCache_purge(cReplayCache* self);
void operator_delete(int* ptr);

/**
 * Destroys a replay input cache: frees its command buffer (cReplayCache_purge) and the object itself when asked.
 *
 * cReplay_removeCache calls it with flags 3.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cReplayCache__cReplayCache(int* self, int flags)
{
    cReplayCache_purge((cReplayCache*)self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Frees the replay input cache's command buffer (+0xC), if any, and clears the pointer.
 *
 * cReplayCache__cReplayCache calls it when the cache is destroyed.
 */
extern "C" void cReplayCache_purge(cReplayCache* self)
{
    void* buffer = self->CmdFrame;
    if (buffer != 0) {
        cMemMan_free(buffer);
        self->CmdFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_reset__FPv);
#ifdef SKIP_ASM
/**
 * Empties a replay input cache: zeroes its chunk count (+0x0), read chunk index (+0x4) and ticks left in that chunk
 * (+0x8).
 *
 * The command buffer itself is kept. cReplay_init and cReplay_reset call it for each input cache.
 */
void cReplayCache_reset(void* vself)
{
    cReplayCache* self = (cReplayCache*)vself;
    self->RecordCursor = 0;
    self->PlaybackCursor = 0;
    self->PlaybackLeft = 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_updateControlCmd);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cReplayCache_addControlCmd(cReplayCache* self, tControlCmd* cmd);
extern "C" int cReplayCache_getControlCmd(cReplayCache* self, tControlCmd* cmd);
/**
 * Records or plays back one frame's control command, depending on the replay state.
 *
 * Returns 0 at once when the cache has no command buffer (+0xC). In kGameRecording the command is appended with
 * cReplayCache_addControlCmd, unless the replay has EndReplay, Loaded or ReplayError set; in states 10, 11, 12 and 14 nothing
 * happens and the result is 0. In kGamePlayback, kInstantPlayback and kPlaybackStep the command is replaced by the next
 * recorded one (cReplayCache_getControlCmd). Every other state leaves it and returns 1. cPlayer_getControlCmd calls it each frame
 * when a replay recorder is attached.
 *
 * @param controlCmd  the 8-byte control command: read when recording, overwritten during playback
 * @return 0 when recording or idle, cReplayCache_getControlCmd's result during playback, 1 in the other states
 */
extern "C" int cReplayCache_updateControlCmd(cReplayCache* self, tControlCmd* controlCmd)
{
    if ((int)self->CmdFrame == 0) {
        return 0;
    }
    int result = 1;
    switch (SSXApp->Game->Replay->State) {
    case 0: {
        cReplay* replay = SSXApp->Game->Replay;
        if (replay->EndReplay != 0 || replay->Loaded != 0 || replay->ReplayError != 0) {
            break;
        }
        cReplayCache_addControlCmd(self, controlCmd);
    }
        /* fall through */
    case 10: case 11: case 12: case 14:
        result = 0;
        break;
    case 1: case 2: case 4:
        result = cReplayCache_getControlCmd(self, controlCmd);
        break;
    case 3: case 5: case 6: case 7: case 8: case 9: case 13:
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_addCmdFrame);
#ifdef SKIP_ASM



/**
 * Appends control command @p chunkp to the cache as a new chunk with a repeat count of 1.
 *
 * The buffer (+0xC) holds 0x1000 8-byte chunks; the low 12 bits of a chunk's first word are its repeat count. When the
 * cache is full nothing is stored: the replay's ReplayError is set and ShowError set to 180 (frames of error display).
 *
 * @param stream  the replay input cache
 * @param chunkp  the command to append
 */
extern "C" void cReplayCache_addCmdFrame(cReplayCache* stream, tControlCmd* chunkp)
{
    cReplayCache* self = stream;
    if (self->RecordCursor >= 0x1000) {
        cReplay* replay = SSXApp->Game->Replay;
        replay->ReplayError = 1;
        replay->ShowError = 180;
        return;
    }
    self->CmdFrame[self->RecordCursor] = *chunkp;
    self->CmdFrame[self->RecordCursor].CruiseCmd.Count = 1;
    self->RecordCursor++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", cReplayCache_addControlCmd);
#ifdef SKIP_ASM
extern "C" int memcmp(const void* a, const void* b, int n);
extern "C" void cReplayCache_addCmdFrame(cReplayCache* self, tControlCmd* chunk);

/**
 * Records one frame's control command, run-length encoded: repeats of the last command only bump its count.
 *
 * The last chunk is compared with @p chunk (8 bytes, with the last chunk's 12-bit count masked to 0 and the new
 * command's own low bits as given). When they match and the count is below 0xFFF it is incremented; otherwise
 * cReplayCache_addCmdFrame appends the command as a new chunk.
 *
 * @param chunk  the command to record
 * @return always 1
 */
extern "C" int cReplayCache_addControlCmd(cReplayCache* self, tControlCmd* chunk)
{
    if (self->RecordCursor > 0) {
        int i = self->RecordCursor - 1;
        tControlCmd last = self->CmdFrame[i];
        last.CruiseCmd.Count = 0;
        if (memcmp(&last, chunk, 8) == 0 && (int)self->CmdFrame[i].CruiseCmd.Count < 0xFFF) {
            self->CmdFrame[i].CruiseCmd.Count++;
        } else {
            cReplayCache_addCmdFrame(self, chunk);
        }
    } else {
        cReplayCache_addCmdFrame(self, chunk);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_truncate);
#ifdef SKIP_ASM

/**
 * Cuts the cache's recorded commands at tick @p pos: the chunk that holds that tick becomes the last one.
 *
 * Sums the chunks' 12-bit repeat counts until the total passes @p pos (or the chunks run out), then sets that chunk's
 * count to total - pos + 1 and the chunk count (+0x0) to its index + 1. The new count is the ticks from @p pos to the
 * chunk's end (the remaining count cReplayCache_moveTo computes), not the ticks before @p pos, which looks like a
 * likely bug. No retail caller was found.
 *
 * @param pos  the tick to cut at
 */
extern "C" void cReplayCache_truncate(cReplayCache* self, unsigned int pos)
{
    int n = self->RecordCursor;
    unsigned int sum = 0;
    tControlCmd* entry = self->CmdFrame;
    int i = 0;
    if (n > 0) {
        sum = entry->CruiseCmd.Count;
        entry++;
        if (pos >= sum) {
            do {
                i++;
                if (i >= n)
                    break;
                sum += entry->CruiseCmd.Count;
                entry++;
            } while (pos >= sum);
        }
    }
    self->CmdFrame[i].CruiseCmd.Count = sum - pos + 1;
    self->RecordCursor = i + 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_getControlCmd);
#ifdef SKIP_ASM

extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Plays back the next recorded control command into @p out; returns 0 when the recording is used up.
 *
 * Zeroes @p out first. While the read chunk index (+0x4) is below the chunk count (+0x0), copies that chunk (repeat
 * count bits included) into @p out; the ticks left in it (+0x8) are loaded from its count when they reach 0 and counted
 * down, moving to the next chunk at 0.
 *
 * @param out  receives the command
 * @return 1 when a command was read, 0 past the end
 */
extern "C" int cReplayCache_getControlCmd(cReplayCache* self, tControlCmd* out)
{
    MEM_fill(out, 0, 8);
    if (self->PlaybackCursor >= self->RecordCursor) {
        return 0;
    }
    if (self->PlaybackLeft <= 0) {
        self->PlaybackLeft = self->CmdFrame[self->PlaybackCursor].CruiseCmd.Count;
    }
    *out = self->CmdFrame[self->PlaybackCursor];
    if (--self->PlaybackLeft <= 0) {
        self->PlaybackCursor++;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_moveTo);
#ifdef SKIP_ASM
struct sPlaybackChunk {
    unsigned int info;
    int data;
};

struct sPlaybackStream {
    int count;
    int index;
    int offset;
    sPlaybackChunk* chunks;
};

/**
 * Positions a replay input cache at a tick.
 *
 * Walks the cache's command chunks (+0xC; the low 12 bits of each chunk's first word are its tick count) until the
 * running total passes @p pos, then stores that chunk's index (+0x4) and the ticks left in it (+0x8). cReplay_rewind,
 * cReplay_rewindFrame, cReplay_lastFrame and cReplay_stepFrame call it for each InputCache with PlaybackTick.
 *
 * @param pos  the tick to move to
 * @return 1 on success, 0 when the tick lies past the recorded input
 */
extern "C" int cReplayCache_moveTo(cReplayCache* self, unsigned int pos)
{
    int i = 0;
    sPlaybackChunk* chunk = (sPlaybackChunk*)self->CmdFrame;
    unsigned int total = 0;
    for (; i < self->RecordCursor; i++) {
        total += (chunk++)->info & 0xFFF;
        if (pos < total) {
            break;
        }
    }
    if (i >= self->RecordCursor) {
        return 0;
    }
    self->PlaybackCursor = i;
    self->PlaybackLeft = total - pos;
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayCache_load);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" int HUFF_decode(void* src, int a, int dst);

/**
 * Loads a replay input cache from saved replay data.
 *
 * Reads the 8-byte save header from *@p pp (its second word goes to +0x0), then a 4-byte size, and decodes the
 * compressed commands into the cache buffer (+0xC) with HUFF_decode (On Tour HUFF_decode); advances *@p pp past all
 * of it. On Tour asserts the save type and size; retail does not check. Called by cReplay_load.
 *
 * @param pp  in/out: the read cursor in the save data
 * @return 1 (always)
 */
extern "C" int cReplayCache_load(cReplayCache* self, char** pp)
{
    struct {
        int tag;
        int value;
    } hdr;
    int size;
    MEM_copy(&hdr, *pp, 8);
    *pp += 8;
    *(int*)self = hdr.value;
    MEM_copy(&size, *pp, 4);
    *pp += 4;
    HUFF_decode(*pp, 0, (int)self->CmdFrame);
    *pp += size;
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayFrame_cReplayFrame);
#ifdef SKIP_ASM
void* cReplayFramePtr_cReplayFramePtr(void* self);
extern "C" void cReplayFrame_clearFrame(cReplayFrame* self);
extern void* cReplayFrame_vtbl[];

/**
 * Constructs an empty replay frame.
 *
 * Runs the cReplayFramePtr base constructor, installs the cReplayFrame vtable cReplayFrame_vtbl and clears the frame
 * (cReplayFrame_clearFrame). cReplay_cReplay builds its 16 Frames and cPlaybackMan_cPlaybackMan its saved frame (+0x14)
 * with it.
 *
 * @return the constructed frame (self)
 */
extern "C" void* cReplayFrame_cReplayFrame(cReplayFrame* self)
{
    cReplayFramePtr_cReplayFramePtr(self);
    *(void***)self = cReplayFrame_vtbl;
    cReplayFrame_clearFrame(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayFrame_clearFrame);
#ifdef SKIP_ASM
struct sPlaybackKey {
    short a;
    char b;
    char c;
};

/**
 * Resets a replay frame's fields to their empty values.
 *
 * Zeroes the frame type at +0x1C (a halfword and two bytes), the start tick at +0x30 and the words at +0x8, +0xC
 * and +0x10, and sets the words at +0x20, +0x24, +0x28, +0x2C and +0x34 to -1. The vtable and the list links
 * (+0x14, +0x18) are left alone.
 */
extern "C" void cReplayFrame_clearFrame(cReplayFrame* self)
{
    sPlaybackKey k;
    k.a = 0;
    k.b = 0;
    k.c = 0;
    self->FrameStartTick = 0;
    self->FrameEndTick = -1;
    *(sPlaybackKey*)&self->FrameType = k;
    self->Ranking[0] = -1;
    self->Ranking[1] = -1;
    self->HighlightEndTick[0] = -1;
    self->WriteData = 0;
    self->ReadData = 0;
    self->HighlightEndTick[1] = -1;
    self->Size = 0;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayFrame_loadFrame);
#ifdef SKIP_ASM


struct sPbFrame_D678 {
    char pad_0x0[0x10];
    int field_0x10;         // 0x10
    char pad_0x14[0x8];
    tReplayFrameType colour;    // 0x1C
    int field_0x20;         // 0x20
    int field_0x24;         // 0x24
    int field_0x28;         // 0x28
    int field_0x2c;         // 0x2C
    int field_0x30;         // 0x30
    int field_0x34;         // 0x34
};

extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" int cReplayFramePtr_loadBlock(void* self, char** pp, void* src);
// PORT: cReplayFramePtr_putFrameBlock is an empty (void*) stub, but this caller passes a second
// argument; bind the 2-arg form to the symbol.
void cReplayFramePtr_putFrameBlock_2(void* self, int a) __asm__("cReplayFramePtr_putFrameBlock__FPv");

/**
 * Loads a replay frame from saved replay data.
 *
 * Copies the 0x28-byte frame header from *@p pp and advances past it; restores +0x10, the tick (+0x30, written by
 * cReplay_saveFrame1 from RecordTick), +0x34, the 4-byte frame type descriptor (+0x1C, named colour in this unit), and the
 * four words +0x20..+0x2C (likely the rider rankings: cReplayFrame_setRanking stores word i there). Then loads the data block
 * (cReplayFramePtr_loadBlock, header + 0x24) and calls cReplayFramePtr_putFrameBlock(self, 1).
 *
 * @param pp  in/out: the read cursor in the save data
 * @return 1 (always; On Tour asserts the save type)
 */
extern "C" int cReplayFrame_loadFrame(cReplayFrame* self, char** pp)
{
    tReplayFrameSave header;
    char* start = *pp;
    *pp = start + 0x28;
    MEM_copy(&header, start, 40);
    self->Size = header.Size;
    self->FrameStartTick = ((int)header.FrameStartTick);
    self->FrameEndTick = ((int)header.FrameEndTick);
    (*(&self->FrameType)) = header.FrameType;
    self->Ranking[0] = header.Ranking[0];
    self->Ranking[1] = header.Ranking[1];
    self->HighlightEndTick[0] = header.HighlightEndTick[0];
    self->HighlightEndTick[1] = header.HighlightEndTick[1];
    cReplayFramePtr_loadBlock(self, pp, start + 0x24);
    cReplayFramePtr_putFrameBlock_2(self, 1);
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplayFrame_setRanking__FPvii);
#ifdef SKIP_ASM
/**
 * Stores @p value as the frame's ranking word for player @p i (the array of words at +0x20; no bounds check).
 *
 * The highlight code in replay/replay (cReplayHighlight_setRanking, cReplayHighlight_begin) stores rider scores and 0 here.
 *
 * @param i  the player index
 * @param value  the value to store
 */
void cReplayFrame_setRanking(void* vself, int i, int value)
{
    cReplayFrame* self = (cReplayFrame*)vself;
    self->Ranking[i] = value;
}
#endif

extern "C" void* cReplayFrameList_addFrame(void*);

//100%
INCLUDE_ASM("replay/playbackman", cReplay_addReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Appends a frame to the replay's recorded frame list.
 *
 * Tail call to cReplayFrameList_addFrame (cReplayFrameList_addFrame) on FrameList (+0x3B0); the frame is passed through untouched
 * in $a1 (the C declaration omits it).
 *
 * @return cReplayFrameList_addFrame's result as this unit declares it (addFrame returns nothing)
 */
void* cReplay_addReplayFrame(void* vself)
{
    cReplay* self = (cReplay*)vself;
    return cReplayFrameList_addFrame(&self->FrameList);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_saveReplayFrame);
#ifdef SKIP_ASM

extern "C" void* cReplay_allocReplayFrame(void* self);
extern "C" void cReplay_saveFrame(cReplay* self, cBxReplayFrame* out);
// PORT: cReplayFramePtr_putFrameBlock is an empty (void*) stub, but this caller passes a second
// argument; bind the 2-arg form to the symbol.
void cReplayFramePtr_putFrameBlock_2(void* self, int a) __asm__("cReplayFramePtr_putFrameBlock__FPv");

/**
 * Takes a free frame, stores its type, saves the game into it and closes its block.
 *
 * Copies the 4-byte frame type from @p src to FrameType (+0x1C), saves (cReplay_saveFrame), stores RecordTick as
 * FrameStartTick (+0x30) and calls cReplayFramePtr_putFrameBlock(frame, 0) (putFrameBlock).
 *
 * @param src  the frame type, passed by address here (On Tour tReplayFrameType type)
 * @return the saved frame
 */
extern "C" void* cReplay_saveReplayFrame(cReplay* self, tReplayFrameType* src)
{
    cReplayFrame* out = (cReplayFrame*)cReplay_allocReplayFrame(self);
    out->FrameType = *src;
    cReplay_saveFrame((cReplay*)self, (cBxReplayFrame*)out);
    out->FrameStartTick = self->RecordTick;
    cReplayFramePtr_putFrameBlock_2(out, 0);
    return ((void*)out);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_saveFrame1);
#ifdef SKIP_ASM
struct sPbBytes4 {
    char b[4];
};

extern "C" void cReplay_saveFrame(cReplay* self, cBxReplayFrame* out);

/**
 * Saves the replay's current state into frame @p out, tagged with frame type @p src and the current RecordTick.
 *
 * Copies the 4-byte frame type descriptor to +0x1C of the frame, writes the frame with cReplay_saveFrame and stores
 * RecordTick at +0x30. cPlaybackMan_saveFrame calls it with its embedded frame and a type-1 descriptor.
 *
 * @param out  the replay frame to fill
 * @param src  the 4-byte frame type descriptor
 */
extern "C" void cReplay_saveFrame1(cReplay* self, cReplayFrame* out, sPbBytes4* src)
{
    *(sPbBytes4*)&out->FrameType = *src;
    cReplay_saveFrame((cReplay*)self, (cBxReplayFrame*)out);
    out->FrameStartTick = self->RecordTick;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_saveFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPbTime {
    int v[6];
};

int cReplayFramePtr_writeRewind(void* self);
void* AIgetseed(void* self);
extern "C" void* cBE_getBE();
extern "C" void cBE_writeToReplayFrame(void* self, cBxReplayFrame* s);
extern "C" void cAvalanche_writeToReplayFrame(cBxReplayFrame* s);
extern "C" void cWScriptMan_writeToReplayFrame(void* self, cBxReplayFrame* s);
extern "C" void func_00229E20(void* self, cBxReplayFrame* s);
extern "C" void func_003441A8(void* self, cBxReplayFrame* s);
extern "C" void cMultiParticleMan_writeToReplayFrame(void* self, cBxReplayFrame* s);
extern "C" void cReplay_saveBucket(cReplay* self, cBxReplayFrame* s, int a);
extern "C" void cReplay_saveDeadBucket(cReplay* self, cBxReplayFrame* s);
extern "C" void cFocusMan_writeToReplayFrame(void* self, cBxReplayFrame* s);

extern int cSortObjNode_ObjNodeSortIndex;
extern int cSortObjNode_ObjNodeNoSortIndex;
extern void* cWScriptMan_TheWScriptMan;
extern void* TheCrowdMan2d_D818 __asm__("TheCrowdMan2d");
extern void* TheCrowdMan3d;
extern void* cMultiParticleMan_TheMultiParticleMan;

// A replay-frame writer: the writeToReplayFrame virtual of cGame (slot 8) and cAI (slot 7). Both are flat here (their
// vptr is the mVtbl member), so the slots are read by hand.
typedef sVEntry<void(void*, cBxReplayFrame*)> tSaveSlot;

/**
 * Writes one replay key frame: the state of every game system that a restore needs.
 *
 * In order: the frame start (cReplayFramePtr_writeRewind), a 0x18-byte time block (AIgetseed), the object-id counters
 * cSortObjNode_ObjNodeSortIndex and cSortObjNode_ObjNodeNoSortIndex, the BE state, the game and its AI (their save virtuals), cAvalanche_writeToReplayFrame (where
 * the restore reads the avalanche), the world-script manager, TheCrowdMan2d, TheCrowdMan3d, the multi-particle
 * manager, the object buckets (cReplay_saveBucket with 1, cReplay_saveDeadBucket) and the AI's focus manager.
 * cReplay_restoreFrame() reads them back in the same order.
 *
 * @param stream  the frame stream to write (On Tour: cReplayFramePtr*)
 */
extern "C" void cReplay_saveFrame(cReplay* self, cBxReplayFrame* stream)
{
    cBxReplayFrame* out = stream;
    sPbTime timeStamp;
    int id;
    cReplayFramePtr_writeRewind(out);
    AIgetseed(&timeStamp);
    out->write((const char*)&timeStamp, 0x18);
    id = cSortObjNode_ObjNodeSortIndex;
    out->write((const char*)&id, 4);
    id = cSortObjNode_ObjNodeNoSortIndex;
    out->write((const char*)&id, 4);
    cBE_writeToReplayFrame(cBE_getBE(), out);
    cGame* game = SSXApp->Game;
    tSaveSlot* gameVt = (tSaveSlot*)game->mVtbl;
    gameVt[8].fn(vthis(game, gameVt[8]), out);
    cAI* ai = SSXApp->Game->AI;
    tSaveSlot* aiVt = (tSaveSlot*)ai->mVtbl;
    aiVt[7].fn(vthis(ai, aiVt[7]), out);
    cAvalanche_writeToReplayFrame(out);
    cWScriptMan_writeToReplayFrame(cWScriptMan_TheWScriptMan, out);
    func_00229E20(TheCrowdMan2d_D818, out);
    func_003441A8(TheCrowdMan3d, out);
    cMultiParticleMan_writeToReplayFrame(cMultiParticleMan_TheMultiParticleMan, out);
    cReplay_saveBucket(self, out, 1);
    cReplay_saveDeadBucket(self, out);
    cFocusMan_writeToReplayFrame(SSXApp->Game->AI->FocusMan, out);
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_saveBucket);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cBucketMan;
void* cBucketMan_first(cBucketMan* self, int index);
void* cBucketMan_next(cBucketMan* self, void* node, int index);
// small-data alias of the bucket manager global (its real type is declared elsewhere; see gameload.cpp)
extern char gBucketMan;
static inline cBucketMan* bucketMan_D988() { return (cBucketMan*)&gBucketMan; }

/**
 * Writes the node count of one bucket (an 8-byte header) and then each node's saved state (its writeToReplayFrame
 * virtual, slot 12).
 *
 * @param data  the replay frame written to (On Tour cReplayFramePtr* data)
 * @param bucket  the bucket
 */
// Writes the bucket's node count (8-byte header) and then each node's serialised form to the stream.
extern "C" void cReplay_saveBucket(cReplay* self, cBxReplayFrame* data, int bucket)
{
    int hdr[2];
    void* node;

    hdr[0] = 0;
    hdr[1] = 0;
    for (node = cBucketMan_first(bucketMan_D988(), bucket); node != 0; node = cBucketMan_next(bucketMan_D988(), node, bucket)) {
        hdr[0]++;
    }
    data->write((const char*)hdr, 8);
    if (hdr[0] > 0) {
        for (node = cBucketMan_first(bucketMan_D988(), bucket); node != 0; node = cBucketMan_next(bucketMan_D988(), node, bucket)) {
            ((cObjNode*)node)->writeToReplayFrame(data);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_saveDeadBucket);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct cBucketMan;
void* cBucketMan_first(cBucketMan* self, int index);
void* cBucketMan_next(cBucketMan* self, void* node, int index);
extern char gBucketMan;
static inline cBucketMan* bucketMan_DA88() { return (cBucketMan*)&gBucketMan; }
/**
 * Writes the dead-object bucket (8): its node count (an 8-byte header), then each node's instance id (the
 * cInstanceNode's Instance->m_RID, 4 bytes).
 *
 * @param data  the replay frame written to (On Tour cReplayFramePtr* data)
 */
extern "C" void cReplay_saveDeadBucket(cReplay* self, cBxReplayFrame* data)
{
    int hdr[2];
    void* node;

    hdr[0] = 0;
    hdr[1] = 0;
    for (node = cBucketMan_first(bucketMan_DA88(), 8); node != 0; node = cBucketMan_next(bucketMan_DA88(), node, 8)) {
        hdr[0]++;
    }
    {
        data->write((const char*)hdr, 8);
    }
    if (hdr[0] > 0) {
        for (node = cBucketMan_first(bucketMan_DA88(), 8); node != 0; node = cBucketMan_next(bucketMan_DA88(), node, 8)) {
            int id[1];
            id[0] = ((cInstanceNode*)node)->Instance->m_RID;
            data->write((const char*)id, 4);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cReplay_restoreReplayFrame);
#ifdef SKIP_ASM
// PORT: cReplayFramePtr_getFrameBlock is mangled with no parameters (__Fv) but is
// called here with (frame, 1); bind the 2-arg form to the symbol.
void cReplayFramePtr_getFrameBlock_2(void* frame, int n) __asm__("cReplayFramePtr_getFrameBlock__Fv");
extern "C" void cReplay_restoreFrame1_2(void* replay, void* frame) __asm__("cReplay_restoreFrame1");

/**
 * Restores the game from a saved frame: opens its block for reading (cReplayFramePtr_putFrameBlock-style getFrameBlock(1)), then
 * cReplay_restoreFrame1.
 *
 * @param frame  the frame to restore (On Tour cReplayFrame*)
 */
extern "C" void cReplay_restoreReplayFrame(cReplay* self, void* frame)
{
    cReplayFramePtr_getFrameBlock_2(frame, 1);
    cReplay_restoreFrame1_2(self, frame);
}
#endif

extern "C" void cReplay_restoreFrame(void*);

//100%
INCLUDE_ASM("replay/playbackman", cReplay_restoreFrame1);
#ifdef SKIP_ASM
/**
 * Thin wrapper that restores a replay frame through cReplay_restoreFrame().
 */
extern "C" void cReplay_restoreFrame1(cReplay* self)
{
    cReplay_restoreFrame(self);
}
#endif

