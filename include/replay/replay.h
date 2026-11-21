#ifndef REPLAY_REPLAY_H
#define REPLAY_REPLAY_H

// cReplay: the race replay recorder/player (record the race as key frames + per-rider input caches, play it back,
// instant replays, highlights, saving to the memory card). EA's header is ../source/replay/replay.h (the July 2003
// preview files its inline asserts under it). One instance per game: cGame::Replay (main/game.h 0x028), created in
// main/gameload.cpp by operator new(0x688, "Replay") + the constructor cReplay_cReplay; no global, no vtable, no base.
// Size 0x688 [R] (allocation); the last field, WriteCheckSum, ends exactly there. On Tour's cReplay is 0x8F8: same
// member order, but its cReplayFrame is 0x48 (retail 0x38), its tRiderStats 0xB0 (retail 0xAC), it keeps 4 input
// caches / highlights / chapters (retail 2) and its bools are bytes (retail words).
//
// Methods: retail units replay/playbackman (0x0026D740-0x0026DBD0), replay/replay (0x0026DBF0-0x0026E568),
// replay/frameptr (0x0026ED98-0x0026FE50) and replay/replaycache (0x00270070-0x00271758).
//
// A FLAT layout, like main/game.h: every field at its retail offset. Names are EA's, from SSX On Tour's PSP DWARF
// (class cReplay), placed by:
//   [R] retail's constructor cReplay_cReplay / destructor cReplay__cReplay and allocations (tags read from the retail ELF:
//       "Replay", "ReplayCache", "WriteBlock", "WriteTmp");
//   [U] how retail code uses the field (cReplay_init, cReplay_reset and the methods named per field);
//   [O] On Tour's member order. Between the fixed points (PadNum 0x28 = setReplayPadNum's field, Frames 0x2C..0x3AC,
//       the camera indices 0x630/0x634 cycled % 9, WriteBlock/WriteTmp 0x63C/0x640 by their tags) every On Tour
//       member has exactly one 4-byte slot.
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "replay/frame.h"
#include "ai/ridermetrix.h" // tRiderStats

struct cReplayFrame;      // replay/frame.h: retail 0x38 bytes (vptr at 0, Next 0x14, Prev 0x18, FrameType 0x1C,
                          // FrameStartTick 0x30)
struct cReplayCache;      // 0x10 bytes ("ReplayCache"): one rider's recorded input
struct tReplayMemCardBlock;
union tControlCmd;
struct tReplayCacheSave;

// tReplayState (On Tour enum; every value below is what retail stores or tests at the same transition).
enum tReplayState {
    kGameRecording = 0,
    kGamePlayback = 1,
    kInstantPlayback = 2,
    kPlaybackInput = 3,
    kPlaybackStep = 4,
    kPlaybackUpdate = 5,
    kPlaybackStepUpdate = 6,
    kPlaybackStart = 7,
    kPlaybackLoad = 8,
    kStartAutoReplay = 9,
    kPreRace = 10,
    kStartRace = 11,
    kStartRace1 = 12,
    kReplayEnd = 13,
    kReplayPaused = 14,
    kReplayInit = 15
};

// A doubly linked list of cReplayFrames (On Tour cReplayFrameList, 0xC): Tail = oldest, Head = newest.
// [R] cReplayFrameList_cReplayFrameList constructs it (zeroes the three words); methods cReplayFrameList_reset, cReplayFrameList_addFrame,
// cReplayFrameList_remFrame, cReplayFrameList_remTail, cReplayFrameList_findFrame.
struct cReplayFrameList {
    cReplayFrame* Tail;    // 0x000 [O]; [U] the oldest frame: cReplayFrameList_remTail unlinks it, cReplayFrameList_addFrame
                           //       (addFrame) sets it on an empty list
    cReplayFrame* Head;    // 0x004 [O]; [U] the newest frame: cReplayFrameList_addFrame appends there (frame->Prev = Head)
    int NumFrames;         // 0x008 [O]; [U] ++/-- by cReplayFrameList_addFrame/cReplayFrameList_remFrame; cReplay_allocReplayFrame tests
                           //       FreeFrameList.NumFrames
};

// One player's highlight being marked (On Tour cReplayHighlight, 0xB8; retail 0xB4 [R]: cReplayHighlight_reset
// writes +0, +0xB0 and fills +4..+0xB0, cReplay_reset strides 0xB4). Methods cReplayHighlight_begin, cReplayHighlight_end,
// cReplayHighlight_cancel, cReplayHighlight_setRanking.
struct cReplayHighlight {
    int PlayerId;          // 0x000 [O]; [R] cReplayHighlight_reset stores -1; [U] cReplay_reset stores 0 / 1
    tRiderStats Stats;     // 0x004 [O] tRiderStats (On Tour 0xB0; retail 0xAC, ai/ridermetrix.h); [R] ctor MEM_fill(this + 4, 0,
                           //       0xAC); [U] cReplay_reset walks the highlights from +0x498 (Highlight[0].Stats)
    cReplayFrame* Frame;   // 0x0B0 [O]; [R] ctor 0; [U] non-zero = marking (cReplay_beginMarkHighlight begins only when 0,
                           //       cReplay_endMarkHighlight)
};

// The save stream's running size and byte checksum (On Tour tWriteCheckSum, 0x8).
struct tWriteCheckSum {
    int Size;              // 0x000 [O]; [U] cReplay_writeSetup adds every written size; cReplay_initSave zeroes it
    int CheckSum;          // 0x004 [O]; [U] cReplay_writeSetup adds every written (signed) byte; cReplay_initSave seeds
                           //       0xDEADBEEF
};

struct cReplay {
    int State;                           // 0x000 [O] tReplayState (values below); [U] cReplay_setReplayState stores
                                         //       it; every method tests it
    int SaveState;                       // 0x004 [O]; [U] cReplay_reset stores 15; cPlaybackMan cPlaybackMan_startPreview saves
                                         //       State here before pausing (14), cPlaybackMan_stopPreview restores it
    int MaxFrames;                       // 0x008 [O]; [U] cReplay_init: 0xC9800 / FrameBlockSize (= 10); bounds the
                                         //       Frames loops (cReplay_init, cReplay_reset, cReplay_purge)
    int MaxPlayerHighlights;             // 0x00C [O]; [U] cReplay_reset: 3 with one rider, else 1; cReplay_maxHighlights
                                         //       (maxHighlights(land)) returns it
    int MaxChapters;                     // 0x010 [O]; [U] cReplay_reset: 3 with one rider, else 0; cReplay_eventChapter
                                         //       (eventChapter) bounds the chapter bit by it
    int FrameBlockSize;                  // 0x014 [O]; [U] cReplay_init stores 0x14000 and passes it to
                                         //       cReplayFramePtr_initBlock
    int SaveFrameTicks;                  // 0x018 [O]; [U] cReplay_init 0; cReplay_update saves a full (type 1) frame
                                         //       when RecordTick % SaveFrameTicks == 0; written by cReplay_writeHeaderStart,
                                         //       read by cReplay_load
    int SkipFrameTicks;                  // 0x01C [O]; [U] cReplay_updateInput sets 10 and counts it down; cReplay_reset
                                         //       zeroes it
    int SkipFrame;                       // 0x020 [O] (On Tour: bool); [U] cReplay_updateInput toggles it (^= 1);
                                         //       cReplay_update clears it outside states 3/4
    int RestoreCamera;                   // 0x024 [O] (On Tour: bool); no retail access seen (placed by the order:
                                         //       SkipFrame 0x20, PadNum 0x28)
    int PadNum;                          // 0x028 [O]; [U] cReplay_setReplayPadNum stores it;
                                         //       cReplay_updateCameraSelect/cReplay_updateInput/cReplay_update read SSXApp's pad[PadNum]
                                         //       (+0xB0)
    cReplayFrame Frames[16];             // 0x02C [O] cReplayFrame Frames[16] (retail 0x38 each, On Tour 0x48); [R]
                                         //       ctor cReplay_cReplay constructs 16 x 0x38 (cReplayFrame_cReplayFrame), dtor
                                         //       cReplay__cReplay destroys them through vtable slot 7; each frame's
                                         //       vptr is at +0
    cReplayFrame* NextCandidate;         // 0x3AC [O]; [U] cReplay_freeOneReplayFrame walks from it for a
                                         //       frame to free; cReplay_reset zeroes it
    cReplayFrameList FrameList;          // 0x3B0 [O]; [R] ctor cReplayFrameList_cReplayFrameList(this + 0x3B0); [U] the recorded frames,
                                         //       oldest first
    cReplayFrameList FreeFrameList;      // 0x3BC [O]; [R] ctor cReplayFrameList_cReplayFrameList(this + 0x3BC); [U] cReplay_reset adds
                                         //       every Frames[i]; cReplay_allocReplayFrame takes one
    int RecordTick;                      // 0x3C8 [O]; [U] cReplay_update counts it up while recording; cReplay_saveReplayFrame
                                         //       stamps new frames (+0x30) with it
    int EndRecordTick;                   // 0x3CC [O]; [U] cReplay_update counts it while no ReplayError; playback
                                         //       stops there (cReplay_stepFrame, cReplay_isPauseAI, cReplay_updateInput)
    cReplayFrame* SaveFrame;             // 0x3D0 [O]; [U] cReplay_pushReplaySaveFrame saves it (frame type 1), cReplay_popReplaySaveFrame
                                         //       restores and frees it; playback and the writer skip it
                                         //       (cReplay_stepFrame, cReplay_writeFrameCount, cReplay_writeFrames)
    cReplayFrame* InstantReplayFrame;    // 0x3D4 [O]; [U] tested by cReplay_endMarkInstantReplay, cleared by
                                         //       cReplay_cancelMarkInstantReplay; cReplay_update starts the
                                         //       instant replay from it
    tRiderStats InstantReplayStats;      // 0x3D8 [O] tRiderStats (retail 0xAC like cReplayHighlight::Stats, On Tour
                                         //       0xB0; ai/ridermetrix.h); [U] cReplay_endMarkInstantReplay compares the rider's
                                         //       stats with TotalPoints (+0x9C, 0x474) on a landing and TotalPointsLostToWipeouts
                                         //       (+0xA4, 0x47C) on a wipeout, as On Tour's does at +0xA0 / +0xA8 (its BestTrick
                                         //       is 4 bytes longer)
    int PlaybackTick;                    // 0x484 [O]; [U] the playback cursor: cReplay_rewind, cReplay_stepFrame
                                         //       (stepFrame), cReplay_rewindFrame/E528/E568
    cReplayFrame* PlaybackFrame;         // 0x488 [O]; [U] ditto: the frame PlaybackTick lies in
    cReplayCache* InputCache[2];         // 0x48C [O] (On Tour 4); [R] cReplay_addCache: cMemMan_alloc(0x10,
                                         //       "ReplayCache") into the first free slot of 2, ctor cReplay_cReplay
                                         //       zeroes 2; [U] moved with the playback cursor (cReplayCache_moveTo)
    cReplayHighlight Highlight[2];       // 0x494 [O] (On Tour 4 x 0xB8); [R] reset by cReplayHighlight_reset in cReplay_cReplay
                                         //       on 2 x 0xB4; [U] cReplay_reset sets PlayerId 0/1;
                                         //       cReplay_beginMarkHighlight/cReplay_endMarkHighlight/cReplay_cancelMarkHighlight index it by player
    int Chapters[2];                     // 0x5FC [O] (On Tour 4); [U] cReplay_eventChapter sets bit `pos` of
                                         //       Chapters[player] (a bit mask)
    int NextFrame;                       // 0x604 [O] (On Tour: bool); [U] cReplay_update clears it every update;
                                         //       cReplay_isPauseAI tests it; cReplay_rewind/cReplay_lastFrame clear it
    int Loaded;                          // 0x608 [O] (On Tour: bool); [U] cReplay_load sets it,
                                         //       cReplay_reset clears it; fe/ovstatereplay.cpp,
                                         //       fe/ovtemplatedialog.cpp test it
    int EndReplay;                       // 0x60C [O] (On Tour: bool); [U] cReplay_endReplay sets it;
                                         //       cReplay_update ends the recording (state 13) on it
    int ReplayError;                     // 0x610 [O] (On Tour: bool); [U] while set cReplay_update stops saving
                                         //       frames and EndRecordTick; blocks event marking (cReplay_eventJump,
                                         //       cReplay_eventChapter); saved by cReplay_writeHeaderStart, loaded by cReplay_load
    int ShowError;                       // 0x614 [O]; [U] cReplay_update counts it down to 0
    int Cmd;                             // 0x618 [O]; [U] cReplay_updateInput stores the replay-menu command (0..8);
                                         //       cReplay_processInput switches on it
    int AutoReplay;                      // 0x61C [O] (On Tour: bool); [U] cReplay_update sets it (state 9);
                                         //       cReplay_startAutoReplay, cReplay_stopAutoReplay, cReplay_stepFrame (loop at the
                                         //       end)
    int InstantReplay;                   // 0x620 [O] (On Tour: bool); [U] cReplay_startReplay stores its
                                         //       instantReplay argument; cReplay_stopInstantReplay
    int StartInstantReplay;              // 0x624 [O] (On Tour: bool); [U] cReplay_startInstantReplay sets it
                                         //       unless InstantReplay; cReplay_update acts on it
    int PauseRecord;                     // 0x628 [O] (On Tour: bool); [U] cReplay_reset clears it (no other retail
                                         //       access seen)
    int PauseRecordTick;                 // 0x62C [O]; [U] cReplay_reset clears it (no other retail access seen)
    int CurrentReplayCamIndex;           // 0x630 [O]; [U] cReplay_cycleReplayCameraAlgorithm steps it % 9;
                                         //       cReplay_setCurrentReplayCamera indexes camSubset with it
    int CurrentInstantReplayCamIndex;    // 0x634 [O]; [U] cReplay_setupInstantReplayCameras steps it % 9
    int WriteState;                      // 0x638 [O]; [U] the save writer's state (0..11): cReplay_getSaveData switches on
                                         //       it, every step ++ it
    tReplayMemCardBlock* WriteBlock;     // 0x63C [O]; [R] cReplay_allocSaveSpace: cMemMan_alloc(0x4000, "WriteBlock"); [U]
                                         //       cReplay_getSaveData fills and returns it
    char* WriteTmp;                      // 0x640 [O]; [R] cReplay_allocSaveSpace: operator new(0xC800, "WriteTmp"); [U]
                                         //       header/frame staging buffer (cReplay_writeHeaderStart, cReplay_writeFrames)
    char* WriteSrc;                      // 0x644 [O]; [U] cReplay_writeSetup points it at the data to write,
                                         //       cReplay_writeBuffered copies from it
    int WriteSrcSize;                    // 0x648 [O]; [U] bytes left in WriteSrc (cReplay_writeBuffered)
    int WriteSize;                       // 0x64C [O]; [U] bytes used in WriteBlock (cReplay_getSaveData, cReplay_writeBuffered)
    char* WriteData;                     // 0x650 [O]; [U] write cursor in WriteBlock (cReplay_writeBuffered)
    int WriteIndex;                      // 0x654 [O]; [U] cReplay_writeInput walks InputCache[WriteIndex]
    int WriteIndexMax;                   // 0x658 [O]; [U] cReplay_writeInputStart stores 2 (the InputCache count)
    cReplayFrame* WriteFrame;            // 0x65C [O]; [U] frame being written (cReplay_writeFramesStart, cReplay_writeFrames,
                                         //       cReplay_writeFrameCount)
    char* WriteBlockData;                // 0x660 [O]; [U] cReplay_writeFrames: the frame block pending compression
    int WriteBlockDataSize;              // 0x664 [O]; [U] its size
    int WriteFrameCount;                 // 0x668 [O]; [U] frames written (cReplay_writeFramesStart, cReplay_writeFrames)
    tControlCmd* WriteCmdFrame;          // 0x66C [O]; [U] cReplay_writeInput: an input cache's command data pending
                                         //       compression
    int WriteCmdFrameSize;               // 0x670 [O]; [U] cReplay_writeInput (0x8000)
    tReplayCacheSave* WriteCacheSave;    // 0x674 [O]; [U] cReplay_writeInput: the 8-byte cache header pending write
                                         //       (points into WriteTmp)
    int WriteSaveSize;                   // 0x678 [O]; [U] cReplay_writeSetup adds every written size; cReplay_writePadStart sizes
                                         //       the padding from it
    int WriteRemaining;                  // 0x67C [O]; [U] cReplay_writePadStart sets 0x7FFF8 - used, cReplay_writePad writes it
                                         //       out as padding
    tWriteCheckSum WriteCheckSum;        // 0x680 [O]; [U] cReplay_writeSetup updates it, cReplay_writeCheckSum writes its 8 bytes
                                         //       last
};

CHECK_OFFSET(cReplayFrameList, Tail, 0x0);
CHECK_OFFSET(cReplayFrameList, Head, 0x4);
CHECK_OFFSET(cReplayFrameList, NumFrames, 0x8);
CHECK_SIZE(cReplayFrameList, 0xC);
CHECK_OFFSET(cReplayHighlight, PlayerId, 0x0);
CHECK_OFFSET(cReplayHighlight, Stats, 0x4);
CHECK_OFFSET(cReplayHighlight, Frame, 0xB0);
CHECK_SIZE(cReplayHighlight, 0xB4);
CHECK_OFFSET(tWriteCheckSum, Size, 0x0);
CHECK_OFFSET(tWriteCheckSum, CheckSum, 0x4);
CHECK_SIZE(tWriteCheckSum, 0x8);
CHECK_OFFSET(cReplay, State, 0x0);
CHECK_OFFSET(cReplay, SaveState, 0x4);
CHECK_OFFSET(cReplay, MaxFrames, 0x8);
CHECK_OFFSET(cReplay, MaxPlayerHighlights, 0xC);
CHECK_OFFSET(cReplay, MaxChapters, 0x10);
CHECK_OFFSET(cReplay, FrameBlockSize, 0x14);
CHECK_OFFSET(cReplay, SaveFrameTicks, 0x18);
CHECK_OFFSET(cReplay, SkipFrameTicks, 0x1C);
CHECK_OFFSET(cReplay, SkipFrame, 0x20);
CHECK_OFFSET(cReplay, RestoreCamera, 0x24);
CHECK_OFFSET(cReplay, PadNum, 0x28);
CHECK_OFFSET(cReplay, Frames, 0x2C);
CHECK_OFFSET(cReplay, NextCandidate, 0x3AC);
CHECK_OFFSET(cReplay, FrameList, 0x3B0);
CHECK_OFFSET(cReplay, FreeFrameList, 0x3BC);
CHECK_OFFSET(cReplay, RecordTick, 0x3C8);
CHECK_OFFSET(cReplay, EndRecordTick, 0x3CC);
CHECK_OFFSET(cReplay, SaveFrame, 0x3D0);
CHECK_OFFSET(cReplay, InstantReplayFrame, 0x3D4);
CHECK_OFFSET(cReplay, InstantReplayStats, 0x3D8);
CHECK_OFFSET(cReplay, PlaybackTick, 0x484);
CHECK_OFFSET(cReplay, PlaybackFrame, 0x488);
CHECK_OFFSET(cReplay, InputCache, 0x48C);
CHECK_OFFSET(cReplay, Highlight, 0x494);
CHECK_OFFSET(cReplay, Chapters, 0x5FC);
CHECK_OFFSET(cReplay, NextFrame, 0x604);
CHECK_OFFSET(cReplay, Loaded, 0x608);
CHECK_OFFSET(cReplay, EndReplay, 0x60C);
CHECK_OFFSET(cReplay, ReplayError, 0x610);
CHECK_OFFSET(cReplay, ShowError, 0x614);
CHECK_OFFSET(cReplay, Cmd, 0x618);
CHECK_OFFSET(cReplay, AutoReplay, 0x61C);
CHECK_OFFSET(cReplay, InstantReplay, 0x620);
CHECK_OFFSET(cReplay, StartInstantReplay, 0x624);
CHECK_OFFSET(cReplay, PauseRecord, 0x628);
CHECK_OFFSET(cReplay, PauseRecordTick, 0x62C);
CHECK_OFFSET(cReplay, CurrentReplayCamIndex, 0x630);
CHECK_OFFSET(cReplay, CurrentInstantReplayCamIndex, 0x634);
CHECK_OFFSET(cReplay, WriteState, 0x638);
CHECK_OFFSET(cReplay, WriteBlock, 0x63C);
CHECK_OFFSET(cReplay, WriteTmp, 0x640);
CHECK_OFFSET(cReplay, WriteSrc, 0x644);
CHECK_OFFSET(cReplay, WriteSrcSize, 0x648);
CHECK_OFFSET(cReplay, WriteSize, 0x64C);
CHECK_OFFSET(cReplay, WriteData, 0x650);
CHECK_OFFSET(cReplay, WriteIndex, 0x654);
CHECK_OFFSET(cReplay, WriteIndexMax, 0x658);
CHECK_OFFSET(cReplay, WriteFrame, 0x65C);
CHECK_OFFSET(cReplay, WriteBlockData, 0x660);
CHECK_OFFSET(cReplay, WriteBlockDataSize, 0x664);
CHECK_OFFSET(cReplay, WriteFrameCount, 0x668);
CHECK_OFFSET(cReplay, WriteCmdFrame, 0x66C);
CHECK_OFFSET(cReplay, WriteCmdFrameSize, 0x670);
CHECK_OFFSET(cReplay, WriteCacheSave, 0x674);
CHECK_OFFSET(cReplay, WriteSaveSize, 0x678);
CHECK_OFFSET(cReplay, WriteRemaining, 0x67C);
CHECK_OFFSET(cReplay, WriteCheckSum, 0x680);
CHECK_SIZE(cReplay, 0x688);

// The replay save header (On Tour struct tReplayManSaveHeader, 0x28). [U] cReplay_writeHeaderStart fills it in
// WriteTmp field by field from the same cReplay members (view sReplayHdr_1228: Version 0xABCD0002, the frame count,
// SaveFrameTicks, ReplayError, RecordTick, EndRecordTick, the player count, then cAI_getPodiumActorList fills the
// actor list at +0x20); cReplay_writeHeader queues these 0x28 bytes.
struct tReplayManSaveHeader {
    int Version;                    // 0x00 [O]; [U] 0xABCD0002
    int NumSaveFrames;              // 0x04 [O]; [U] cReplay_writeFrameCount
    int SaveFrameTicks;             // 0x08 [O]; [U] cReplay::SaveFrameTicks
    int ReplayError;                // 0x0C [O]; [U] cReplay::ReplayError
    int RecordTick;                 // 0x10 [O]; [U] cReplay::RecordTick
    int EndRecordTick;              // 0x14 [O]; [U] cReplay::EndRecordTick
    int Players;                    // 0x18 [O]; [U] the AI's NumberOfPlayersAllocated
    int Player;                     // 0x1C [O]; no write in writeHeaderStart
    int PodiumActorList[2];         // 0x20 [O] eBECharacters[2]; [U] cAI_getPodiumActorList fills it
};

CHECK_OFFSET(tReplayManSaveHeader, Version, 0x00);
CHECK_OFFSET(tReplayManSaveHeader, NumSaveFrames, 0x04);
CHECK_OFFSET(tReplayManSaveHeader, SaveFrameTicks, 0x08);
CHECK_OFFSET(tReplayManSaveHeader, ReplayError, 0x0C);
CHECK_OFFSET(tReplayManSaveHeader, RecordTick, 0x10);
CHECK_OFFSET(tReplayManSaveHeader, EndRecordTick, 0x14);
CHECK_OFFSET(tReplayManSaveHeader, Players, 0x18);
CHECK_OFFSET(tReplayManSaveHeader, Player, 0x1C);
CHECK_OFFSET(tReplayManSaveHeader, PodiumActorList, 0x20);
CHECK_SIZE(tReplayManSaveHeader, 0x28);

#endif /* REPLAY_REPLAY_H */
