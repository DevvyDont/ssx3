#ifndef REPLAY_PLAYBACKMAN_H
#define REPLAY_PLAYBACKMAN_H

// cPlaybackMan: the mission playback manager: plays back recorded rider input (the "playback.big" demo files, one
// tPlaybackFile per BE mission) for previews and "perform" runs, and records into CurrentFrame. One instance,
// cGame::PlaybackMan (main/game.h, 0x034): cGame_load does cPlaybackMan::cPlaybackMan(cMemMan_alloc(0x7C,
// "PlaybackMan", 0x20000000, 0), 40).
//
// Size 0x7C: [R] that allocation; the constructor writes up to 0x78 (AsyncHandle). No base class, no virtual functions.
// EA's source: d:/ssx/code/source/replay/playbackman.cpp, header ../source/replay/playbackman.h (preview strings);
// retail methods 0x0026C458-0x0026D020 (unit replay/playbackman) plus the out-of-line isRecord copy
// cPlaybackMan_isRecord
// (0x001448A8, unit ai/control/handplantcontrol).
//
// A FLAT layout, like main/game.h. Member names, order and types are EA's, from SSX On Tour's PSP DWARF ([O] class
// cPlaybackMan, size 0xA4). Retail is smaller: Location is char[8] (On Tour [32]: retail initLocation strcpy's to +0x8
// and clears SaveFrame at +0x10), SaveFrame is a word, and the embedded cReplayFrame is 0x38 (On Tour 0x48; see
// replay/replay.h), so retail = On Tour - 0x28 from SaveFrameData on. [P] the July 2003 preview's
// isRecord__C12cPlaybackMan reads State at 0x70 (lw 0x70; == 3), the retail offset.
// Enums and bools are words.

#include "layout.h"
#include "replay/frame.h" // cReplayFrame

struct cBigFile;
struct cGame;
struct cAI;
struct cInput;
struct cInputPad;
// The recorded pad input of one playback (On Tour struct tPlaybackInputBuffer, 0x1C20C) and the playback frame that
// holds it (On Tour struct tPlaybackFrame, 0x1C22C; retail allocates 0x1C22C). [U] cPlaybackMan_isPauseAI reads /
// writes Buffer.Index / Buffer.IndexMax and the packed bytes at Buffer.Buffer (view sPbBuf_26CE50: size 0x4,
// pos 0x8, end 0xC, data 0x10).
struct tPlaybackInputBuffer {
    int Size;                       // 0x00000 [O]
    int Index;                      // 0x00004 [O]; [U] the read / write cursor
    int IndexMax;                   // 0x00008 [O]; [U] the end of the recorded data
    unsigned char Buffer[115200];   // 0x0000C [O]; [U] a count byte, then one byte per pad value
};

CHECK_OFFSET(tPlaybackInputBuffer, Size, 0x0);
CHECK_OFFSET(tPlaybackInputBuffer, Index, 0x4);
CHECK_OFFSET(tPlaybackInputBuffer, IndexMax, 0x8);
CHECK_OFFSET(tPlaybackInputBuffer, Buffer, 0xC);
CHECK_SIZE(tPlaybackInputBuffer, 0x1C20C);

struct tPlaybackFrame {
    unsigned int Version;           // 0x00000 [O]
    tPlaybackInputBuffer Buffer;    // 0x00004 [O]
    int AITick;                     // 0x1C210 [O]
    unsigned int AISeed[6];         // 0x1C214 [O]
};

CHECK_OFFSET(tPlaybackFrame, Version, 0x0);
CHECK_OFFSET(tPlaybackFrame, Buffer, 0x4);
CHECK_OFFSET(tPlaybackFrame, AITick, 0x1C210);
CHECK_OFFSET(tPlaybackFrame, AISeed, 0x1C214);
CHECK_SIZE(tPlaybackFrame, 0x1C22C);

// One playback file (On Tour struct tPlaybackFile, 0xC); cPlaybackMan_initLocation allocates MaxPlaybackFiles of them
// ("tPlaybackFile") and sets {-1, 0, 0}.
struct tPlaybackFile {
    unsigned int Id;        // 0x0 [O]; [U] initLocation stores the BE mission ids; cPlaybackMan_findFileById compares
                            //       it
    char* PackData;         // 0x4 [O]; [U] cPlaybackMan_update stores the loaded data, cPlaybackMan_load tests it,
                            //     cPlaybackMan_purgeLocation frees it
    int Size;               // 0x8 [O]; [U] cPlaybackMan_update stores the loaded size
};

CHECK_OFFSET(tPlaybackFile, Id, 0x0);
CHECK_OFFSET(tPlaybackFile, PackData, 0x4);
CHECK_OFFSET(tPlaybackFile, Size, 0x8);
CHECK_SIZE(tPlaybackFile, 0xC);

struct cPlaybackMan {
    cBigFile* BigFile;              // 0x00 [O]; [R] the ctor opens "|data\missions\playback.big" in a new cBigFile
                                    //      ("BigFile", 8 bytes); the dtor destroys it
    tPlaybackFrame* CurrentFrame;   // 0x04 [O]; [U] initLocation: cMemMan_alloc(0x1C22C, "CurrentFrame"), header
                                    //      0xDBAC0101 / 0x1C200; purgeLocation frees it; the ctor zeroes it
    char Location[8];               // 0x08 [O] (char[32] in On Tour); [U] initLocation strcpy's the location name here
    int SaveFrame;                  // 0x10 [O] bool; [R] ctor 0; [U] initLocation 0, saveFrame / restoreFrame
    cReplayFrame SaveFramePtr;      // 0x14 [O] cReplayFrame (retail 0x38: replay/frame.h); [R] the ctor runs
                                    //      cReplayFrame_cReplayFrame(self+0x14), the dtor stores cReplayFramePtr_vtbl
                                    //       there;
                                    //      [U] initLocation: cReplayFrame_clearFrame / cReplayFramePtr_setBlock on it
    char* SaveFrameData;            // 0x4C [O]; [R] ctor 0; [U] initLocation: new (0x14000, "SaveFrameData"), the block
                                    //       of
                                    //      SaveFramePtr; purgeLocation frees it
    int NumPlaybackFiles;           // 0x50 [O]; [R] ctor 0; [U] initLocation: cBEMissionInterface_getNumberMissions
    int MaxPlaybackFiles;           // 0x54 [O]; [R] the ctor stores its maxPlaybackFiles argument (cGame_load: 40);
                                    //      [U] initLocation allocates that many tPlaybackFile
    tPlaybackFile* PlaybackFiles;   // 0x58 [O]; [R] ctor 0; [U] initLocation: new (n * 12, "tPlaybackFile")
    cGame* Game;                    // 0x5C [O]; [R] the ctor stores SSXApp->Game
    cAI* AI;                        // 0x60 [O]; [R] the ctor stores Game->AI (cGame +0xC)
    cInputPad* RealInputPad;        // 0x64 [O]; [R] the ctor stores player 0's cInput pad (AI->Players[0] +0xDF0, word
                                    //       0)
    cInputPad* InputPad;            // 0x68 [O]; [R] the ctor: cMemMan_alloc(0x2A4, "cInputPad"); the dtor deletes it;
                                    //      [U] cPlaybackMan_isPauseAI fills it
    cInput* Input;                  // 0x6C [O]; [R] the ctor: cMemMan_alloc(8, "cInput") with cInput_setPad(InputPad);
                                    //      the dtor deletes it
    // State values (names ours). [P] the methods' names: startPreview sets kPreview and stopPreview kIdle,
    // startPerform kPerform, isRecord__C12cPlaybackMan tests kRecord; [U] stop and isPauseAI switch on them.
    enum eState { kIdle = 0, kPreview = 1, kPerform = 2, kRecord = 3 };
    // AsyncHandle with no read pending (name ours): [R] ctor; [U] update and purgeLocation reset it to this.
    enum { kNoAsyncHandle = -1 };
    int State;                      // 0x70 [O] eState (On Tour int); [P] isRecord__C12cPlaybackMan lw 0x70 == 3;
                                    //      [R] ctor 0; [U] start/stopPreview, start/stopPerform, restart and
                                    //      purgeLocation set it; stop, setupPreview and isPauseAI test it
    int AsyncFileNum;               // 0x74 [O]; [R] ctor -1; [U] cPlaybackMan_load stores the file index being loaded,
                                    //      cPlaybackMan_update stores the loaded data in PlaybackFiles[AsyncFileNum]
    int AsyncHandle;                // 0x78 [O]; [R] ctor -1; [U] cPlaybackMan_load: the async-file handle (-1 on
                                    //       failure),
                                    //      cPlaybackMan_update polls it (ASYNCFILE_getstatus) and resets it to -1

    // Methods bound to their retail symbols by asm labels.
    bool isRecord() __asm__("cPlaybackMan_isRecord");
    void update() __asm__("cPlaybackMan_update");
    void initLocation(const char* name) __asm__("cPlaybackMan_initLocation");
    void purgeLocation() __asm__("cPlaybackMan_purgeLocation");
    bool isLoaded(int fileId) __asm__("cPlaybackMan_isLoaded");
    bool load(int key) __asm__("cPlaybackMan_load");
    void restart() __asm__("cPlaybackMan_restart");
    bool setupPreview(int fileId) __asm__("cPlaybackMan_setupPreview");
    void reset() __asm__("cPlaybackMan_reset__FPv");
    void saveFrame() __asm__("cPlaybackMan_saveFrame");
    void* restoreFrame() __asm__("cPlaybackMan_restoreFrame");
    void startPreview() __asm__("cPlaybackMan_startPreview");
    void stopPreview() __asm__("cPlaybackMan_stopPreview");
    void startPerform() __asm__("cPlaybackMan_startPerform");
    void stopPerform() __asm__("cPlaybackMan_stopPerform__FPv");
    void stop() __asm__("cPlaybackMan_stop");
    bool isPauseAI() __asm__("cPlaybackMan_isPauseAI");
    int findFileById(int id) __asm__("cPlaybackMan_findFileById");
    bool setupFile(int idx) __asm__("cPlaybackMan_setupFile");

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cPlaybackMan(int maxPlaybackFiles);
};

CHECK_OFFSET(cPlaybackMan, BigFile, 0x00);
CHECK_OFFSET(cPlaybackMan, CurrentFrame, 0x04);
CHECK_OFFSET(cPlaybackMan, Location, 0x08);
CHECK_OFFSET(cPlaybackMan, SaveFrame, 0x10);
CHECK_OFFSET(cPlaybackMan, SaveFramePtr, 0x14);
CHECK_OFFSET(cPlaybackMan, SaveFrameData, 0x4C);
CHECK_OFFSET(cPlaybackMan, NumPlaybackFiles, 0x50);
CHECK_OFFSET(cPlaybackMan, MaxPlaybackFiles, 0x54);
CHECK_OFFSET(cPlaybackMan, PlaybackFiles, 0x58);
CHECK_OFFSET(cPlaybackMan, Game, 0x5C);
CHECK_OFFSET(cPlaybackMan, AI, 0x60);
CHECK_OFFSET(cPlaybackMan, RealInputPad, 0x64);
CHECK_OFFSET(cPlaybackMan, InputPad, 0x68);
CHECK_OFFSET(cPlaybackMan, Input, 0x6C);
CHECK_OFFSET(cPlaybackMan, State, 0x70);
CHECK_OFFSET(cPlaybackMan, AsyncFileNum, 0x74);
CHECK_OFFSET(cPlaybackMan, AsyncHandle, 0x78);
CHECK_SIZE(cPlaybackMan, 0x7C);

#endif /* REPLAY_PLAYBACKMAN_H */
