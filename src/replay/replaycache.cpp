#include "common.h"
#include "replay/replaycache.h"
#include "ai/ridermetrix.h"
#include "vtable.h"
#include "replay/frame.h"
#include "ai/rider.h"
#include "inseq/scriptplayerchannel.h"
#include "inseq/scriptheap.h"
#include "replay/frameptr.h"
#include "inseq/inseq.h"
#include "inseq/scriptplayermain.h"
#include "inseq/scriptplayerentity.h"
#include "replay/replay.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"
#include "main/gameviewman.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"

//100%
INCLUDE_ASM("replay/replaycache", cReplay_addCache);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cReplayCache_cReplayCache(void* self);
extern const char str_ReplayCache[];

/**
 * Adds a recorded-input cache to the replay: builds a new cReplayCache in the first free InputCache slot.
 *
 * Checks InputCache[0] and [1] (+0x48C); for the first empty one it allocates 0x10 bytes tagged "ReplayCache"
 * and runs the cache constructor cReplayCache_cReplayCache (which allocates its 0x8000-byte "CmdFrame" buffer and clears
 * its counters). Nothing is allocated when both slots are in use.
 *
 * @return the new cache, or 0 when both slots were already taken
 */
void* cReplay::addCache()
{
    void** slot = (void**)&InputCache;
    for (int i = 0; i < 2; i++, slot++) {
        if (*slot == 0) {
            return *slot = cReplayCache_cReplayCache(cMemMan_alloc(0x10, str_ReplayCache, 0, 0));
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_removeCache);
#ifdef SKIP_ASM
extern "C" void cReplayCache__cReplayCache(void* p, int mode);

/**
 * Removes a rider's input cache: finds @p pCache in InputCache[2], destroys it (cReplayCache__cReplayCache, flags 3) and clears
 * the slot. A cache that is not registered is left alone.
 *
 * @param pCache  the cReplayCache to remove
 */
void cReplay::removeCache(void* pCache)
{
    if (pCache != 0) {
        void** slot = (void**)&InputCache;
        for (int i = 0; i < 2; i++, slot++) {
            if (*slot == pCache) {
                if (*slot != 0) {
                    cReplayCache__cReplayCache(*slot, 3);
                }
                *slot = 0;
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_allocReplayFrame);
#ifdef SKIP_ASM
extern "C" void cReplay_freeOneReplayFrame(cReplay* self);
extern "C" void* cReplayFrameList_remTail(void* list);


/**
 * Takes a frame off FreeFrameList, first freeing a recorded one (cReplay_freeOneReplayFrame) when the free list is empty.
 *
 * The frame (cReplayFrameList_remTail, the free list's oldest) gets frame type 0, -1 in the words at +0x20, +0x24 and +0x34, and
 * FrameStartTick (+0x30) 0.
 *
 * @return the frame (On Tour cReplayFrame*)
 */
void* cReplay::allocReplayFrame()
{
    void* list = (char*)&FreeFrameList;
    if (FreeFrameList.NumFrames <= 0) {
        freeOneReplayFrame();
    }
    cReplayFrame* node = (cReplayFrame*)cReplayFrameList_remTail(list);
    tReplayFrameType desc;
    desc.Type = 0;
    desc.Highlights = 0;
    desc.Chapter = 0;
    node->FrameType = desc;
    node->Ranking[0] = -1;
    node->Ranking[1] = -1;
    node->FrameEndTick = -1;
    node->FrameStartTick = 0;
    return ((void*)node);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_freeOneReplayFrame);
#ifdef SKIP_ASM
extern "C" void cReplay_freeReplayFrame(cReplay* self, void* node);

struct sRcNode1A0 {
    char pad_0x00[0x14];
    sRcNode1A0* next;       // 0x14
    char pad_0x18[0x6];
    unsigned char locked;   // 0x1E
    unsigned char used;     // 0x1F
};


/**
 * Frees one recorded frame to make room (On Tour freeOneReplayFrame).
 *
 * Walks the frame list from NextCandidate, looking past the candidate for a frame that is neither a highlight nor a
 * chapter (FrameType bytes +0x1E and +0x1F both 0) and is not the newest; it moves NextCandidate past that frame and
 * frees it. When the walk reaches the end it restarts from the oldest frame, once; if nothing qualifies nothing is
 * freed.
 */
void cReplay::freeOneReplayFrame()
{
    cReplay* replay = this;
    int i;
    for (i = 0; i < 2; i++) {
        while (((sRcNode1A0*)replay->NextCandidate) != 0 && ((sRcNode1A0*)replay->NextCandidate)->next != 0 && ((sRcNode1A0*)replay->NextCandidate)->next->next != 0) {
            sRcNode1A0* n = ((sRcNode1A0*)replay->NextCandidate)->next;
            if (n->used == 0) {
                if (n->locked == 0) {
                    *(sRcNode1A0**)&replay->NextCandidate = n->next;
                    freeReplayFrame(n);
                    return;
                }
            }
            *(sRcNode1A0**)&replay->NextCandidate = ((sRcNode1A0*)replay->NextCandidate)->next;
        }
        *(sRcNode1A0**)&replay->NextCandidate = *(sRcNode1A0**)&replay->FrameList;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_freeReplayFrame);
#ifdef SKIP_ASM
extern "C" void cReplayFrameList_remFrame(void* list, void* node);
extern "C" void cReplayFrame_clearFrame(void* node);
extern "C" void cReplayFrameList_addFrame(void* list, void* node);

/**
 * Gives a frame back: unlinks it from FrameList (cReplayFrameList_remFrame), clears it (cReplayFrame_clearFrame) and adds it to
 * FreeFrameList (cReplayFrameList_addFrame).
 *
 * @param node  the frame (On Tour cReplayFrame*)
 */
void cReplay::freeReplayFrame(void* node)
{
    cReplayFrameList_remFrame((char*)&FrameList, node);
    cReplayFrame_clearFrame(node);
    cReplayFrameList_addFrame((char*)&FreeFrameList, node);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_isPauseAI);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sS270280 {
    int state;
    char pad4[0x3CC - 4];
    int f3CC;
    char pad3D0[0x484 - 0x3D0];
    int f484;
    char pad488[0x604 - 0x488];
    int f604;
};
/**
 * Says whether the AI should hold still for the current replay state.
 *
 * Never while recording or in states 2 and 10-15. Always in 3, 5, 6, 8 and 9. In 1 and 7 when NextFrame is clear or
 * playback has reached EndRecordTick; in 4 only at EndRecordTick.
 *
 * @return 1 to pause the AI, else 0 (On Tour returns bool)
 */
int cReplay::isPauseAI()
{
    int paused = 0;
    switch (State) {
    case kGameRecording: case kInstantPlayback: case kPreRace: case kStartRace: case kStartRace1: case kReplayEnd:
    case kReplayPaused: case kReplayInit:
        break;
    case kGamePlayback:
    case kPlaybackStart:
        if (NextFrame == 0)
            paused = 1;
        if (PlaybackTick >= EndRecordTick)
            paused = 1;
        break;
    case kPlaybackStep:
        paused = PlaybackTick >= EndRecordTick;
        break;
    case kPlaybackInput: case kPlaybackUpdate: case kPlaybackStepUpdate: case kPlaybackLoad: case kStartAutoReplay:
        paused = 1;
        break;
    }
    return paused;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_rewind);
#ifdef SKIP_ASM
extern "C" int cReplayCache_moveTo(void* stream, unsigned int pos);

struct sReplay002702F8 {
    char pad_0x000[0x3B0];
    void* head;
    char pad_0x3B4[0x484 - 0x3B4];
    int pos;
    void* frame;
    void* streams[2];
    char pad_0x494[0x604 - 0x494];
    int unk_0x604;
};

/**
 * Moves playback to the start of @p frame, or of the oldest recorded frame when it is null.
 *
 * Clears NextFrame, sets PlaybackFrame, takes PlaybackTick from the frame's FrameStartTick and moves each input cache
 * to it (cReplayCache_moveTo, cReplayCache::moveTo).
 *
 * @param frame  the frame to start from, or 0 for the first
 */
void cReplay::rewind(void* frame)
{
    int i;
    NextFrame = 0;
    if (frame != 0) {
        PlaybackFrame = (cReplayFrame*)frame;
    } else {
        PlaybackFrame = (cReplayFrame*)*(void**)&FrameList;
    }
    PlaybackTick = PlaybackFrame->FrameStartTick;
    for (i = 0; i < 2; i++) {
        if (InputCache[i] != 0) {
            cReplayCache_moveTo(InputCache[i], PlaybackTick);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_lastFrame);
#ifdef SKIP_ASM
extern "C" int cReplayCache_moveTo(void* stream, unsigned int pos);
// PORT: cReplay_findFrame is declared elsewhere with one parameter, but this caller
// passes (self, pos); bind the 2-arg form to the symbol.
void* cReplay_findFrame_2(void* self, int pos) __asm__("cReplay_findFrame__FPv");


/**
 * Moves the playback cursor to the newest recorded frame: clears NextFrame, PlaybackFrame = the frame containing tick
 * 100000000, PlaybackTick = its FrameStartTick, and every input cache moves there (cReplayCache_moveTo,
 * cReplayCache::moveTo).
 */
void cReplay::lastFrame()
{
    int i;
    NextFrame = 0;
    cReplayFrame* frame = (cReplayFrame*)cReplay_findFrame_2((this), 100000000);
    PlaybackFrame = frame;
    PlaybackTick = frame->FrameStartTick;
    for (i = 0; i < 2; i++) {
        if (InputCache[i] != 0) {
            cReplayCache_moveTo(InputCache[i], PlaybackTick);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stepFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cReplay_restoreReplayFrame(void* self, void* frame);
extern "C" void cChaseCameraController_switchCamera_Chapter(void* self);
extern "C" void cGameViewMan_updateAll(void* list);
void* cReplay_findFrame_2(void* self, int pos) __asm__("cReplay_findFrame__FPv");
void cReplay_setReplayState(void* self, int arg);


/**
 * Advances playback by one tick, or (@p reset) jumps to the frame holding PlaybackTick and restores it.
 *
 * With @p reset, PlaybackFrame becomes the frame for PlaybackTick (cReplay_findFrame); SaveFrame is skipped (its +0x18
 * link); PlaybackTick moves to that frame's start, the input caches follow and the frame is restored. Otherwise the
 * tick advances until EndRecordTick: there an automatic replay rewinds, steps, switches the first view's chase camera
 * to its chapter camera (slot 4 of its controller) and updates every view; any other replay stops at EndRecordTick in
 * state 3 (kPlaybackInput).
 *
 * @param reset  nonzero to jump to and restore the frame for PlaybackTick (On Tour bool setframe)
 */
void cReplay::stepFrame(int reset)
{
    int i;
    if (reset) {
        PlaybackFrame = (cReplayFrame*)cReplay_findFrame_2((this), PlaybackTick);
    }
    cReplayFrame* frame = PlaybackFrame;
    if (((void*)frame) != 0 && ((void*)frame) == (void*)SaveFrame) {
        PlaybackFrame = (cReplayFrame*)(frame->Prev);
    }
    if (reset) {
        PlaybackTick = PlaybackFrame->FrameStartTick;
        for (i = 0; i < 2; i++) {
            if (InputCache[i] != 0) {
                cReplayCache_moveTo(InputCache[i], PlaybackTick);
            }
        }
        cReplay_restoreReplayFrame((this), PlaybackFrame);
        return;
    }
    if (PlaybackTick >= EndRecordTick) {
        if (AutoReplay != 0) {
            rewind(0);
            stepFrame(1);
            cGameViewMan* views = SSXApp->Game->ViewMan;
            cChaseCameraController* controller = views->PlayerCamera[0]->ChaseController;
            cChaseCameraController_switchCamera_Chapter(controller);
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)controller->mVtbl;
            vt[4].fn(vthis(controller, vt[4]));
            cGameViewMan_updateAll(views);
        } else {
            PlaybackTick = EndRecordTick;
            cReplay_setReplayState((this), 3);
        }
    } else {
        PlaybackTick++;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeIdleFrames);
#ifdef SKIP_ASM

/**
 * Frees every recorded frame that carries nothing: plain frames (FrameType.Type 0) that are not marked as a highlight
 * or a chapter (bytes +0x1E and +0x1F both 0).
 */
void cReplay::purgeIdleFrames()
{
    cReplayFrame* node = *(cReplayFrame**)&FrameList;
    while (node != 0) {
        cReplayFrame* next = node->Next;
        if (node->FrameType.Type != 0) {
        } else if (node->FrameType.Chapter != 0) {
        } else if (node->FrameType.Highlights != 0) {
        } else {
            freeReplayFrame(node);
        }
        node = next;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_beginMarkInstantReplay__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (empty body).
 *
 * cReplay_eventJump and cReplay_eventWScript call it where an instant-replay mark would begin;
 * cReplay_endMarkInstantReplay and cReplay_cancelMarkInstantReplay do the real work.
 */
void cReplay::beginMarkInstantReplay()
{
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_endMarkInstantReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_startInstantReplay(cReplay* self);

/**
 * Finishes marking an instant replay after a landing or wipeout (On Tour endMarkInstantReplay(const tRiderStats*
 * stats, bool land)).
 *
 * Only when a mark is open (InstantReplayFrame) and exactly one player is allocated (cAI NumberOfPlayersAllocated, AI
 * + 0x7C): compares a stats counter with its value when the mark began (InstantReplayStats), the int at +0x9C for a
 * landing or +0xA4 for a wipeout. A gain of 1000 or more starts the instant replay; otherwise the mark is dropped
 * (InstantReplayFrame = 0).
 *
 * @param obj  the rider's tRiderStats (declared int; On Tour stats)
 * @param alt  non-zero for a landing, 0 for a wipeout (On Tour land)
 */
// PORT: the unit declares the object argument as int.
void cReplay::endMarkInstantReplay(int obj, int alt)
{
    tRiderStats* stats = (tRiderStats*)obj;
    if ((int)InstantReplayFrame == 0) {
        return;
    }
    if (SSXApp->Game->AI->NumberOfPlayersAllocated != cAI::kOnePlayer) {
        return;
    }
    int now;
    int start;
    if (alt != 0) {
        now = stats->TotalPoints;
        start = InstantReplayStats.TotalPoints;
    } else {
        now = stats->TotalPointsLostToWipeouts;
        start = InstantReplayStats.TotalPointsLostToWipeouts;
    }
    now -= start;
    if (now >= 1000) {
        startInstantReplay();
    } else {
        InstantReplayFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkInstantReplay);
#ifdef SKIP_ASM

/**
 * Drops a marked instant replay (InstantReplayFrame) when the AI manager's +0x7C word is 1.
 */
// FAKEMATCH: the clear is written as three identical arms on the sign of v (`if (v > 0) {*on = 0;} else if (v < 0)
// {*on = 0;} else {*on = 0;}`), which keeps v a global pseudo with priority over self; plain form 97.92%.
// Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void cReplay_cancelMarkInstantReplay(cReplay* self)
{
    int* on = (int*)&self->InstantReplayFrame;
    int markFrame = *on;
    if (markFrame != 0) {
        int x = SSXApp->Game->AI->NumberOfPlayersAllocated;
        if (x == 1) {
            if (markFrame > 0) {
                *on = 0;
            } else if (markFrame < 0) {
                *on = 0;
            } else {
                *on = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_startInstantReplay);
#ifdef SKIP_ASM
/**
 * Requests an instant replay: sets StartInstantReplay unless one is already playing (InstantReplay). cReplay_update
 * starts it.
 */
void cReplay::startInstantReplay()
{
    if (InstantReplay == 0) {
        StartInstantReplay = 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stopInstantReplay);
#ifdef SKIP_ASM
extern "C" void cReplay_stopReplay(void* self);
extern "C" void cRiderMetrix_resetHUDfx(void* p);

/**
 * Stops a playing instant replay: stopReplay, then resets the HUD effects of player 0's rider metrix
 * (cRiderMetrix_resetHUDfx on Players[0] + 0x18, then + 0x790). Does nothing when no instant replay is playing.
 */
void cReplay::stopInstantReplay()
{
    if (InstantReplay != 0) {
        cReplay_stopReplay(this);
        cRiderMetrix_resetHUDfx(SSXApp->Game->AI->Players[0]->mRider->mMetrix);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_startAutoReplay);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cReplay_setReplayState_cpp(void* self, int arg) __asm__("cReplay_setReplayState__FPvi");

/**
 * Switches to kStartAutoReplay unless an automatic replay is already on or the state is already kStartAutoReplay.
 */
void cReplay::startAutoReplay()
{
    if (AutoReplay == 0 && *(int*)this != kStartAutoReplay) {
        cReplay_setReplayState_cpp(this, kStartAutoReplay);
    }
}
#endif

extern "C" void cReplay_stopReplay(void* self);
void cReplay_setReplayState(void* self, int arg);

//100%
INCLUDE_ASM("replay/replaycache", cReplay_stopAutoReplay__FPv);
#ifdef SKIP_ASM
/**
 * Stops an auto replay and puts the replay into the kReplayEnd state (13).
 *
 * When AutoReplay (+0x61C) is set it first calls cReplay_stopReplay (probably stopReplay: it clears AutoReplay,
 * InstantReplay and StartInstantReplay and, unless Loaded, restores the saved frame). The state is then set
 * to 13 either way, overriding the one cReplay_stopReplay chose.
 */
void cReplay::stopAutoReplay()
{
    if (AutoReplay != 0) {
        cReplay_stopReplay(this);
    }
    cReplay_setReplayState(this, 13);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", findPlayerId);
#ifdef SKIP_ASM

// The race's AI manager (an inline call: retail reloads SSXApp->Game->AI for each use).
static inline cAI* replayAI()
{
    return SSXApp->Game->AI;
}

/**
 * Finds the AI rider slot that belongs to a given player id.
 *
 * Walks SSXApp->Game->AI's Players (NumberOfPlayersAllocated of them) and asks each player's rider (mRider) for its
 * AI index (cRider::getAIIndex, vtable slot 7). The cReplay event recorders (cReplay_eventJump, _eventLand,
 * _eventWipeout, _eventReset, _eventWScript, _eventChapter) use it to file events per rider.
 *
 * @param id  the player id to look for
 * @return the rider index, or -1 when no rider has that id
 */
extern "C" int findPlayerId(int id)
{
    int i;
    for (i = 0; i < replayAI()->NumberOfPlayersAllocated; i++) {
        if (replayAI()->Players[i]->mRider->getAIIndex() == id) {
            return i;
        }
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventJump);
#ifdef SKIP_ASM
extern "C" int findPlayerId(int id);
extern "C" void cReplay_beginMarkHighlight(cReplay* self, int slot, int id);
// PORT: the unit defines cReplay_beginMarkInstantReplay(void*) (an empty stub), but this caller
// passes (self, id); bind the 2-arg call by asm label.
void cReplay_beginMarkInstantReplay_2(void* self, int id) __asm__("cReplay_beginMarkInstantReplay__FPv");

/**
 * Records a rider's jump while recording (On Tour eventJump(int rider, const tRiderStats& stats)).
 *
 * Ignored during auto or instant replays, after ReplayError or EndReplay, or outside kGameRecording. Otherwise, for a
 * rider that is a player (findPlayerId, findPlayerId), begins a highlight mark for that player (cReplay_beginMarkHighlight) and
 * an instant-replay mark (cReplay_beginMarkInstantReplay).
 *
 * @param key  the rider index (On Tour rider)
 * @param id  the rider's tRiderStats (declared int; On Tour stats)
 */
void cReplay::eventJump(int key, int id)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        ReplayError == 0 && EndReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            beginMarkHighlight(slot, id);
            cReplay_beginMarkInstantReplay_2(this, id);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventLand);
#ifdef SKIP_ASM
extern "C" int findPlayerId(int id);
extern "C" void cReplay_endMarkHighlight(cReplay* self, int slot, int id, int flag);
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int id, int flag);

/**
 * Records a rider's landing while recording (On Tour eventLand(int rider, const tRiderStats& stats)).
 *
 * Ignored during auto or instant replays or outside kGameRecording. For a player rider (findPlayerId) ends the
 * highlight mark as a landing (cReplay_endMarkHighlight with land 1) and ends the instant-replay mark
 * (cReplay_endMarkInstantReplay with land 1).
 *
 * @param key  the rider index (On Tour rider)
 * @param id  the rider's tRiderStats (declared int; On Tour stats)
 */
void cReplay::eventLand(int key, int id)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            endMarkHighlight(slot, id, 1);
            endMarkInstantReplay(id, 1);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventWipeout);
#ifdef SKIP_ASM
extern "C" int findPlayerId(int id);
extern "C" void cReplay_endMarkHighlight(cReplay* self, int slot, int id, int flag);
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int id, int flag);

/**
 * Records a rider's wipeout while recording (On Tour eventWipeout(int rider, const tRiderStats& stats)).
 *
 * Ignored during auto or instant replays or outside kGameRecording. For a player rider (findPlayerId) ends the
 * highlight mark as a wipeout (cReplay_endMarkHighlight with land 0) and ends the instant-replay mark
 * (cReplay_endMarkInstantReplay with land 0).
 *
 * @param key  the rider index (On Tour rider)
 * @param id  the rider's tRiderStats (declared int; On Tour stats)
 */
void cReplay::eventWipeout(int key, int id)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            endMarkHighlight(slot, id, 0);
            endMarkInstantReplay(id, 0);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventReset);
#ifdef SKIP_ASM
extern "C" int findPlayerId(int id);
struct sRcSlots;
extern "C" void cReplay_cancelMarkHighlight(cReplay* self, int i);
extern "C" void cReplay_cancelMarkInstantReplay(cReplay* self);

/**
 * Records a rider's reset during live recording: cancels that rider's marked highlight and any marked instant replay.
 *
 * Only while recording (state 0) outside automatic and instant replays, and when findPlayerId (findPlayerId) finds
 * the rider.
 *
 * @param key  the rider (On Tour int rider)
 */
void cReplay::eventReset(int key)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cancelMarkHighlight(slot);
            cReplay_cancelMarkInstantReplay(this);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_eventWScript);
#ifdef SKIP_ASM
extern "C" int findPlayerId(int id);
extern "C" void cReplay_beginMarkHighlight(cReplay* self, int slot, int id);
// PORT: the unit defines cReplay_beginMarkInstantReplay(void*) (an empty stub), but this caller
// passes (self, id); bind the 2-arg call by asm label.
void cReplay_beginMarkInstantReplay_2(void* self, int id) __asm__("cReplay_beginMarkInstantReplay__FPv");

/**
 * Handles a world-script replay command for a rider while recording (On Tour eventWScript(int rider, int cmd)).
 *
 * Ignored during auto or instant replays, after ReplayError or EndReplay, or outside kGameRecording. For a player
 * rider (findPlayerId) it takes the rider's stats (its +0x790 object + 0xFC, from cAI Riders[rider]): command 0
 * begins an instant-replay mark (cReplay_beginMarkInstantReplay), command 1 begins a highlight mark (cReplay_beginMarkHighlight); others are
 * ignored.
 *
 * @param key  the rider index (On Tour rider)
 * @param mode  the command, 0 or 1 (On Tour cmd)
 */
void cReplay::eventWScript(int key, int mode)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        ReplayError == 0 && EndReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cRider* rider = SSXApp->Game->AI->Riders[key];
            switch (mode) {
            case kWScriptReplay_MarkInstantReplay:
                cReplay_beginMarkInstantReplay_2(this, (int)rider->mMetrix + 0xFC);
                break;
            case kWScriptReplay_MarkHighlight:
                beginMarkHighlight(slot, (int)rider->mMetrix + 0xFC);
                break;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_eventChapter);
#ifdef SKIP_ASM
static inline int cReplay_eventChapter_isClear(int v, int bit)
{
    return !((v >> bit) & 1);
}

/**
 * Marks a chapter (a race checkpoint) for a rider during live recording.
 *
 * Only while recording (state 0) with no automatic or instant replay, ReplayError or EndReplay. When the rider is
 * found (findPlayerId) and @p bit is below MaxChapters and not yet set in Chapters[player], sets the player's bit in
 * the newest frame's byte +0x1F and the chapter bit in Chapters[player].
 *
 * @param key  the rider (On Tour int rider)
 * @param bit  the chapter (On Tour int pos)
 */
void cReplay::eventChapter(int key, int bit)
{
    if (AutoReplay == 0 && InstantReplay == 0 &&
        ReplayError == 0 && EndReplay == 0 &&
        State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0 && bit >= 0 && bit < MaxChapters) {
            int off = slot << 2;
            char* base = (char*)&Chapters;
            int* chapterBits = (int*)(base + off);
            if (cReplay_eventChapter_isClear(*chapterBits, bit)) {
                unsigned char* headFrame = (unsigned char*)FrameList.Head;
                headFrame[0x1F] |= 1 << slot;
                *chapterBits |= 1 << bit;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_beginMarkHighlight);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cReplayHighlight_begin_3(void* slot, void* rider, int src) __asm__("cReplayHighlight_begin");

// Whether a highlight slot is idle (not marking a frame).
static inline bool highlightIsFree(cReplayHighlight* highlight)
{
    return highlight->Frame == 0;
}


/**
 * Begins marking a highlight for player @p playerId on the newest recorded frame, while the replay is recording.
 *
 * Only in kGameRecording (State 0) and when Highlight[playerId] is not already marking (its Frame is null): runs
 * cReplayHighlight_begin on it with FrameList's Head (the newest frame) and @p id. cReplay_eventJump and cReplay_eventWScript
 * call it.
 *
 * @param playerId  the player (0 or 1)
 * @param id  the rider stats at the start (a tRiderStats pointer passed as int)
 */
void cReplay::beginMarkHighlight(int playerId, int id)
{
    cReplay* self = this;
    if (self->State == 0 && highlightIsFree(&self->Highlight[playerId])) {
        cReplayHighlight_begin_3(&self->Highlight[playerId], ((void*)self->FrameList.Head), id);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_countHighlights);
#ifdef SKIP_ASM
struct sReplayCacheNode {
    char pad[0x14];
    sReplayCacheNode* next;
    char pad2[0x1E - 0x18];
    unsigned char mask;
    char pad3;
    int values[1];
};

/**
 * Counts player @p i's highlight frames of one kind (On Tour countHighlights(int playerId, bool land)).
 *
 * Counts the frames whose highlight bits (+0x1E) include bit @p i and whose ranking for the player (+0x20 + i * 4) is
 * positive (landings) or negative (wipeouts).
 *
 * @param i  the player index (On Tour playerId)
 * @param positive  non-zero to count landings, 0 for wipeouts (On Tour land)
 * @return the number of matching frames
 */
int cReplay::countHighlights(int i, int positive)
{
    int count = 0;
    sReplayCacheNode* n = *(sReplayCacheNode**)&FrameList;
    while (n != 0) {
        if ((n->mask >> i) & 1) {
            if (positive) {
                if (n->values[i] > 0) count++;
            } else if (n->values[i] < 0) {
                count++;
            }
        }
        n = n->next;
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_maxHighlights);
#ifdef SKIP_ASM
/**
 * Returns how many highlights a player may keep of one kind: MaxPlayerHighlights for landings, 1 for wipeouts.
 *
 * @param land  non-zero for landings (On Tour land)
 * @return the highlight limit
 */
int cReplay::maxHighlights(int land)
{
    if (land != 0) {
        return MaxPlayerHighlights;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_compareHighlights);
#ifdef SKIP_ASM
struct sReplayCacheNode2 {
    char pad[0x14];
    sReplayCacheNode2* next;
    char pad2[0x1E - 0x18];
    unsigned char mask;
    unsigned char busy;
    int values[1];
};

/**
 * Finds player @p i's weakest highlight of one kind, starting from @p best (On Tour compareHighlights(playerId,
 * frame1, land)).
 *
 * Scans the frames marked for the player that are not chapters (+0x1F zero): for landings keeps the lowest positive
 * ranking, for wipeouts the negative ranking closest to zero (ties go to the later frame).
 *
 * @param i  the player index (On Tour playerId)
 * @param best  the frame to compare against (On Tour frame1)
 * @param positive  non-zero for landings, 0 for wipeouts (On Tour land)
 * @return @p best or a weaker matching frame
 */
extern "C" sReplayCacheNode2* cReplay_compareHighlights(cReplay* self, int i, sReplayCacheNode2* best, int positive)
{
    sReplayCacheNode2* result = best;
    sReplayCacheNode2* n = *(sReplayCacheNode2**)&self->FrameList;
    while (n != 0) {
        if (((n->mask >> i) & 1) && n->busy == 0) {
            if (positive) {
                if (n->values[i] > 0 && !(best->values[i] < n->values[i])) {
                    best = result = n;
                }
            } else {
                if (n->values[i] < 0 && !(n->values[i] < best->values[i])) {
                    best = result = n;
                }
            }
        }
        n = n->next;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_endMarkHighlight);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" bool cReplayHighlight_setRanking_s(void* self, int id, int mode) __asm__("cReplayHighlight_setRanking");
extern "C" void cReplayHighlight_cancel(void* p);
void cReplayHighlight_end(void* self);

// Whether a highlight slot is marking a frame.
static inline bool highlightIsMarking(cReplayHighlight* highlight)
{
    return highlight->Frame != 0;
}

// The frame a highlight slot is marking.
static inline cReplayFrame* highlightFrame(cReplayHighlight* highlight)
{
    return highlight->Frame;
}


/**
 * Ends player @p playerId's highlight mark: keeps it if it ranks, replacing the weakest highlight when the list is
 * full.
 *
 * Only when Highlight[playerId] is marking a frame. cReplayHighlight_setRanking ranks it from @p id and @p positive; when it ranks
 * and the player already has cReplay_maxHighlights(@p positive) highlights (cReplay_countHighlights), the frame
 * cReplay_compareHighlights picks loses this player's highlight bit (+0x1E). When it does not rank the mark is
 * cancelled (cReplayHighlight_cancel). Either way the mark ends with cReplayHighlight_end.
 *
 * @param playerId  the player (0 or 1)
 * @param id  the rider stats at the end (a tRiderStats pointer passed as int)
 * @param positive  non-zero for a landing (cReplay_eventLand), 0 for a wipeout (cReplay_eventWipeout)
 */
void cReplay::endMarkHighlight(int playerId, int id, int positive)
{
    cReplay* self = this;
    if (highlightIsMarking(&self->Highlight[playerId])) {
        if (cReplayHighlight_setRanking_s(&self->Highlight[playerId], id, positive)) {
            if (!(self->countHighlights(playerId, positive) < self->maxHighlights(positive))) {
                sReplayCacheNode2* n =
                    cReplay_compareHighlights(self, playerId,
                                              (sReplayCacheNode2*)highlightFrame(&self->Highlight[playerId]), positive);
                n->mask &= ~(1 << playerId);
            }
        } else {
            cReplayHighlight_cancel(&self->Highlight[playerId]);
        }
        cReplayHighlight_end(&self->Highlight[playerId]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkHighlight);
#ifdef SKIP_ASM
extern "C" void cReplayHighlight_cancel(void* p);


struct sRcSlots {
    int state;
    char pad[0x494 - 4];
    cReplayHighlight slots[1];
};

/**
 * Cancels a player's marked highlight (cReplayHighlight_cancel on Highlight[i]), only while recording (state 0).
 *
 * @param i  the player (On Tour int playerId)
 */
void cReplay::cancelMarkHighlight(int i)
{
    if (State == 0) {
        cReplayHighlight_cancel(&((&Highlight[0]))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkHighlights);
#ifdef SKIP_ASM
/**
 * Cancels both players' open highlight marks (cReplayHighlight_cancel on Highlight[0] and [1]) while recording (State 0).
 */
void cReplay::cancelMarkHighlights()
{
    if (State == 0) {
        for (int i = 0; i < 2; i++) {
            cReplayHighlight_cancel(&((&Highlight[0]))[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_initSave);
#ifdef SKIP_ASM
extern "C" void cReplay_allocSaveSpace(cReplay* self);

/**
 * Prepares the replay for saving: allocates the save buffers (allocSaveSpace), clears every writer field and seeds
 * WriteCheckSum (Size 0, CheckSum 0xDEADBEEF).
 */
void cReplay::initSave()
{
    allocSaveSpace();
    WriteState = 0;
    WriteSrc = 0;
    WriteSrcSize = 0;
    WriteSize = 0;
    WriteData = 0;
    WriteIndex = 0;
    WriteIndexMax = 0;
    WriteFrame = 0;
    WriteBlockData = 0;
    WriteBlockDataSize = 0;
    WriteCmdFrame = 0;
    WriteCacheSave = 0;
    WriteSaveSize = 0;
    WriteRemaining = 0;
    WriteCheckSum.Size = 0;
    WriteCheckSum.CheckSum = (int)0xDEADBEEF;
}
#endif

extern "C" void* cReplay_purgeSaveSpace(cReplay* self);

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeSave__FPv);
#ifdef SKIP_ASM
/**
 * Releases the replay save buffers by calling cReplay_purgeSaveSpace.
 *
 * The replay save screen (cOVState_SAVEREPLAY_onDestroyScreen, _onWidgetEvent, _onPopupEvent) calls it when it drops a
 * held save buffer.
 *
 * @return cReplay_purgeSaveSpace's (meaningless) result
 */
void* cReplay::purgeSave()
{
    return ((cReplay*)this)->purgeSaveSpace();
}
#endif

extern "C" void* cReplay_reset(void* self);

//100%
INCLUDE_ASM("replay/replaycache", cReplay_initLoad__FPv);
#ifdef SKIP_ASM
/**
 * Prepares the replay system for loading a saved replay: resets it.
 *
 * Tail call to cReplay_reset. cGame_load calls it, then cReplay_purgeLoad.
 *
 * @return cReplay_reset's result as this unit declares it
 */
void* cReplay::initLoad()
{
    return cReplay_reset(this);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeLoad__FPv);
#ifdef SKIP_ASM
/**
 * Releases replay-load resources: a no-op in this build.
 *
 * An empty stub, as in On Tour. cGame_load calls it right after cReplay_initLoad.
 */
void cReplay::purgeLoad()
{
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_allocSaveSpace);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_WriteBlock[];
extern const char str_WriteTmp[];

/**
 * Allocates the save buffers: WriteBlock, a 0x4000-byte memory-card block (cMemMan_alloc, tag str_WriteBlock
 * "WriteBlock"), and WriteTmp, a 0xC800-byte staging buffer (operator_new_tag, tag str_WriteTmp "WriteTmp").
 */
void cReplay::allocSaveSpace()
{
    WriteBlock = (tReplayMemCardBlock*)cMemMan_alloc(0x4000, str_WriteBlock, 0x20000000, 0);
    WriteTmp = (char*)operator_new_tag(51200, str_WriteTmp, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_purgeSaveSpace);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* p);

/**
 * Frees the save buffers and clears WriteBlock and WriteTmp.
 *
 * WriteBlock (from cMemMan_alloc) goes to operator_delete and WriteTmp (from operator_new_tag) to cMemMan_free: each
 * is released by the other allocator's partner.
 *
 * @return nothing meaningful (declared void*; the unit's tail-call wrapper cReplay_purgeSave uses this declaration)
 */
// The unit declares this as returning void* (line ~198, used by the tail-call
// wrapper cReplay_purgeSave); nothing meaningful is returned.
void* cReplay::purgeSaveSpace()
{
    operator_delete((int*)WriteBlock);
    void* tmpBuffer = WriteTmp;
    if (tmpBuffer != 0) {
        cMemMan_free(tmpBuffer);
    }
    WriteBlock = 0;
    WriteTmp = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeBuffered);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);

struct sRcStream0FC0 {
    char pad_0x000[0x644];
    char* src;      // 0x644
    int left;       // 0x648
    int used;       // 0x64C
    char* dst;      // 0x650
};

/**
 * Copies as much pending save data (WriteSrc, WriteSrcSize bytes) as fits into the rest of the 0x4000-byte
 * WriteBlock, advancing WriteData, WriteSrc and WriteSize.
 */
void cReplay::writeBuffered()
{
    if (WriteSrc != 0) {
        if (WriteSrcSize > 0) {
            int space = 0x4000 - WriteSize;
            int n = space < WriteSrcSize ? space : WriteSrcSize;
            if (n > 0) {
                MEM_copy(WriteData, WriteSrc, n);
                WriteData += n;
                WriteSrc += n;
                WriteSrcSize -= n;
                WriteSize += n;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeSetup);
#ifdef SKIP_ASM
/**
 * Queues @p size bytes of @p data for saving: adds the size to WriteCheckSum.Size and WriteSaveSize and every
 * (signed) byte to WriteCheckSum.CheckSum, then points WriteSrc / WriteSrcSize at it.
 *
 * @param data  the bytes to save
 * @param size  how many bytes
 */
void cReplay::writeSetup(signed char* data, int size)
{
    WriteCheckSum.Size += size;
    for (int i = 0; i < size; i++) {
        WriteCheckSum.CheckSum += data[i];
    }
    WriteSrc = (char*)data;
    WriteSrcSize = size;
    WriteSaveSize += size;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_getSaveData);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cReplay_writeHeaderStart(cReplay* self);
extern "C" void cReplay_writeHeader(cReplay* self);
extern "C" void cReplay_writeFramesStart(cReplay* self);
struct sReplayCache1380;
extern "C" void cReplay_writeFrames(cReplay* self);
extern "C" void cReplay_writeInputStart(cReplay* self);
struct sReplayCache1510;
extern "C" void cReplay_writeInput(cReplay* self);
extern "C" void cReplay_writePadStart(cReplay* self);
extern "C" void cReplay_writePad(cReplay* self);
void* cReplay_writeCheckSumStart(void* self);
extern "C" void cReplay_writeCheckSum(cReplay* self);


/**
 * Produces the next block of the replay save (On Tour getSaveData(tReplayMemCardBlock** data, int& size)).
 *
 * Unless the writer is done (WriteState kWrite_Done), fills WriteBlock from its start: copies pending data
 * (writeBuffered) and runs the writer step for WriteState (0 header start, 1 header, 2 frames start, 3 frames, 4 input
 * start, 5 input, 6 pad start, 7 pad, 8 checksum start, 9 checksum, 10 finish; cReplay::eWriteState) until the
 * 0x4000-byte block is full or the writer is done.
 *
 * @param block  receives WriteBlock
 * @param size  receives the bytes in the block (0 when done)
 * @return true when a block was produced, false when the save is complete
 */
bool cReplay::getSaveData(char** block, int* size)
{
    if (WriteState == kWrite_Done) {
        *size = 0;
        return false;
    }
    WriteSize = 0;
    WriteData = (char*)WriteBlock;
    while (WriteSize < 0x4000 && WriteState != kWrite_Done) {
        writeBuffered();
        if (WriteSize >= 0x4000) {
            goto done;
        }
        switch (WriteState) {
        case kWrite_HeaderStart: writeHeaderStart(); break;
        case kWrite_Header: writeHeader(); break;
        case kWrite_FramesStart: writeFramesStart(); break;
        case kWrite_Frames: writeFrames(); break;
        case kWrite_InputStart: writeInputStart(); break;
        case kWrite_Input: writeInput(); break;
        case kWrite_PadStart: writePadStart(); break;
        case kWrite_Pad: writePad(); break;
        case kWrite_CheckSumStart: writeCheckSumStart(); break;
        case kWrite_CheckSum: writeCheckSum(); break;
        case kWrite_Finish: WriteState++; break;
        case kWrite_Done: break;
        }
    }
done:
    *size = WriteSize;
    *block = (char*)WriteBlock;
    return true;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeHeaderStart);
#ifdef SKIP_ASM
struct sReplayCache;
extern "C" int cReplay_writeFrameCount(cReplay* self);
extern "C" void cAI_getPodiumActorList(void* self, int* dst);


/**
 * Builds the 0x28-byte save header in WriteTmp and advances WriteState.
 *
 * Header: magic 0xABCD0002, the frame count (writeFrameCount), SaveFrameTicks, ReplayError, RecordTick,
 * EndRecordTick, the number of players (cAI NumberOfPlayersAllocated) and at +0x20 the podium actor list
 * (cAI_getPodiumActorList). The word at +0x1C is not written, so it keeps whatever WriteTmp held.
 */
void cReplay::writeHeaderStart()
{
    tReplayManSaveHeader* hdr = (tReplayManSaveHeader*)WriteTmp;
    hdr->NumSaveFrames = writeFrameCount();
    cSSXApp* app = SSXApp;
    hdr->SaveFrameTicks = SaveFrameTicks;
    hdr->ReplayError = ReplayError;
    hdr->RecordTick = RecordTick;
    hdr->EndRecordTick = EndRecordTick;
    int mode = app->Game->AI->NumberOfPlayersAllocated;
    hdr->Version = 0xABCD0002;
    hdr->Players = mode;
    cAI_getPodiumActorList(app->Game->AI, hdr->PodiumActorList);
    WriteSrc = 0;
    WriteSrcSize = 0;
    WriteState = WriteState + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writeHeader);
#ifdef SKIP_ASM
/**
 * Queues the 0x28-byte header from WriteTmp (writeSetup) and advances WriteState.
 */
void cReplay::writeHeader()
{
    writeSetup((signed char*)WriteTmp, 40);
    WriteState += 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeFrameCount);
#ifdef SKIP_ASM
struct sCacheNode {
    char pad[0x14];
    sCacheNode* next;
};


/**
 * Counts the recorded frames that will be saved: walks FrameList from the oldest frame, leaving out SaveFrame. Leaves
 * WriteFrame null.
 *
 * When it meets SaveFrame it skips it and then steps once more without counting, so the frame after SaveFrame is not
 * counted either; the count is only right when SaveFrame is the newest frame (or absent).
 *
 * @return the number of frames
 */
int cReplay::writeFrameCount()
{
    int count = 0;
    *(sCacheNode**)&WriteFrame = *(sCacheNode**)&FrameList;
    while (((sCacheNode*)WriteFrame)) {
        if (((sCacheNode*)WriteFrame) == ((sCacheNode*)SaveFrame)) {
            *(sCacheNode**)&WriteFrame = ((sCacheNode*)WriteFrame)->next;
        } else {
            count++;
        }
        if (((sCacheNode*)WriteFrame)) {
            *(sCacheNode**)&WriteFrame = ((sCacheNode*)WriteFrame)->next;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeFramesStart);
#ifdef SKIP_ASM
/**
 * Starts writing frames: WriteFrame = the oldest recorded frame (or the next one when the oldest is SaveFrame),
 * clears WriteBlockData, WriteSrc, WriteSrcSize and WriteFrameCount, and advances WriteState.
 */
void cReplay::writeFramesStart()
{
    cReplayFrame* oldest = (cReplayFrame*)(*(void**)&FrameList);
    WriteFrame = (cReplayFrame*)oldest;
    if (((void*)oldest) == (void*)SaveFrame) {
        WriteFrame = (cReplayFrame*)(oldest->Next);
    }
    WriteBlockData = 0;
    WriteState += 1;
    WriteSrc = 0;
    WriteSrcSize = 0;
    WriteFrameCount = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeFrames);
#ifdef SKIP_ASM
extern "C" int HUFF_encode(void* src, int size, void* dst, int* state);
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size);
// PORT: cReplayFramePtr_getFrameBlock is mangled with no parameters (__Fv) but is
// called with (frame, n); bind the 2-arg form to the symbol.
void cReplayFramePtr_getFrameBlock_2(void* frame, int n) __asm__("cReplayFramePtr_getFrameBlock__Fv");



/**
 * Writes the next piece of the recorded frames to the replay save data; one call per step of cReplay_getSaveData.
 *
 * If a frame block is pending (WriteBlockData) it is compressed with HUFF_encode into WriteTmp after a size word and
 * written (cReplay_writeSetup), and the pending block is cleared. Otherwise, while WriteFrame is set, it counts the
 * frame (WriteFrameCount), fetches its block, writes a 40-byte header (tag 0x11111113, size, the words at +0x30/+0x34
 * and +0x20..+0x2C, end tag 0x11111111), makes the block pending and moves WriteFrame to the next frame, skipping
 * SaveFrame. With no frame left it clears WriteSrc / WriteSrcSize and advances WriteState.
 */
void cReplay::writeFrames()
{
    if (((void*)WriteBlockData) != 0) {
        int state = 0;
        int* sized = (int*)WriteTmp;  // a size word, then the encoded bytes
        int n = HUFF_encode(((void*)WriteBlockData), WriteBlockDataSize, sized + 1, &state);
        *((int*)WriteTmp) = n;
        writeSetup((signed char*)((int*)WriteTmp), n + 4);
        *(void**)&WriteBlockData = 0;
        return;
    }
    if (WriteFrame != 0) {
        WriteFrameCount++;
        cReplayFramePtr_getFrameBlock_2(WriteFrame, 1);
        int* hdr = ((int*)WriteTmp);
        hdr[0] = 0x11111113;
        hdr[2] = WriteFrame->FrameStartTick;
        hdr[3] = WriteFrame->FrameEndTick;
        hdr[1] = WriteFrame->Size;
        hdr[9] = 0x11111111;
        hdr[5] = WriteFrame->Ranking[0];
        hdr[6] = WriteFrame->Ranking[1];
        hdr[7] = WriteFrame->HighlightEndTick[0];
        hdr[8] = WriteFrame->HighlightEndTick[1];
        hdr[7] = WriteFrame->HighlightEndTick[0];
        hdr[8] = WriteFrame->HighlightEndTick[1];
        writeSetup((signed char*)hdr, 40);
        cReplayFrame* currentFrame = WriteFrame;
        *(void**)&WriteBlockData = ((void*)currentFrame->Block);
        WriteBlockDataSize = currentFrame->Size;
        cReplayFrame* next = currentFrame->Next;
        *(cReplayFrame**)&WriteFrame = next;
        if (next != 0 && next == SaveFrame) {
            *(cReplayFrame**)&WriteFrame = next->Next;
        }
        return;
    }
    *(int*)&WriteSrc = 0;
    WriteSrcSize = 0;
    WriteState++;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeInputStart);
#ifdef SKIP_ASM
/**
 * Starts writing the input caches: WriteIndexMax = 2 (the InputCache count), WriteIndex 0, no pending cache header or
 * command data, and advances WriteState.
 */
void cReplay::writeInputStart()
{
    WriteIndexMax = 2;
    WriteState += 1;
    WriteCacheSave = 0;
    WriteCmdFrame = 0;
    WriteCmdFrameSize = 0;
    WriteSrc = 0;
    WriteSrcSize = 0;
    WriteIndex = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeInput);
#ifdef SKIP_ASM
extern "C" int HUFF_encode(void* src, int size, void* dst, int* state);



/**
 * Writes the next piece of the input caches to the replay save data; one call per step of cReplay_getSaveData.
 *
 * A pending 8-byte cache header (WriteCacheSave) is written first, then the pending command data (WriteCmdFrame,
 * WriteCmdFrameSize) compressed with HUFF_encode after a size word. Otherwise the next InputCache[WriteIndex] is taken:
 * a present cache gets its header prepared in WriteTmp (tag 0x11111112, its chunk count) and its 0x8000-byte buffer
 * (+0xC) queued; a missing one is skipped. After the last cache WriteState is advanced. The words at +0x644 and +0x648
 * are cleared when moving on.
 */
void cReplay::writeInput()
{
    if (((int*)WriteCacheSave) != 0) {
        writeSetup((signed char*)((int*)WriteCacheSave), 8);
        (*(int**)&WriteCacheSave) = 0;
        return;
    }
    if (((void*)WriteCmdFrame) != 0) {
        int state = 0;
        int* sized = (int*)WriteTmp;  // a size word, then the encoded bytes
        int n = HUFF_encode(((void*)WriteCmdFrame), WriteCmdFrameSize, sized + 1, &state);
        *((int*)WriteTmp) = n;
        writeSetup((signed char*)((int*)WriteTmp), n + 4);
        (*(void**)&WriteCmdFrame) = 0;
        return;
    }
    int cur = WriteIndex;
    if (cur < WriteIndexMax) {
        (*(void**)&WriteCmdFrame) = 0;
        (*(int**)&WriteCacheSave) = 0;
        WriteCmdFrameSize = 0;
        cReplayCache* inputCache = InputCache[cur];
        if (inputCache != 0) {
            (*(void**)&WriteCmdFrame) = ((void*)inputCache->CmdFrame);
            (*(int**)&WriteCacheSave) = ((int*)WriteTmp);
            WriteCmdFrameSize = 0x8000;
            *((int*)WriteTmp) = 0x11111112;
            WriteCacheSave->RecordCursor = inputCache->RecordCursor;
            WriteIndex++;
        } else {
            WriteIndex = cur + 1;
            WriteSrcSize = 0;
            (*(int*)&WriteSrc) = 0;
        }
    } else {
        WriteState++;
        WriteSrcSize = 0;
        (*(int*)&WriteSrc) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writePadStart);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
extern "C" void* cBE_getBE();
extern "C" int cBE_getDataSize(void* be);

/**
 * Starts the padding: clears WriteTmp and sets WriteRemaining so the save fills 0x7FFF8 bytes (minus the BE data size
 * from cBE::getDataSize and WriteSaveSize); advances WriteState. With the 8-byte checksum that makes 512 KB.
 */
void cReplay::writePadStart()
{
    MEM_fill(WriteTmp, 0, 51200);
    int used = cBE_getDataSize(cBE_getBE()) + WriteSaveSize;
    WriteRemaining = 0x7FFF8 - used;
    WriteState += 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writePad);
#ifdef SKIP_ASM
/**
 * Queues up to 0x4000 zero bytes of padding from WriteTmp per call until WriteRemaining is used up, then advances
 * WriteState.
 */
void cReplay::writePad()
{
    int left = WriteRemaining;
    if (left > 0) {
        int n = (left < 16385) ? left : 0x4000;
        writeSetup((signed char*)WriteTmp, n);
        WriteRemaining -= n;
    } else {
        WriteState += 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeCheckSumStart__FPv);
#ifdef SKIP_ASM
/**
 * Starts the checksum step: advances WriteState and clears WriteSrc / WriteSrcSize.
 *
 * @return the new WriteState (declared void*)
 */
void* cReplay::writeCheckSumStart()
{
    int zero = 0;
    int nextState = WriteState + 0x1;
    WriteSrc = (char*)zero;
    WriteSrcSize = zero;
    WriteState = nextState;
    return (void*)nextState;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writeCheckSum);
#ifdef SKIP_ASM
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size);

/**
 * Queues the 8-byte WriteCheckSum (Size, CheckSum) from a copy in WriteTmp and advances WriteState. writeSetup adds
 * the copied bytes to the checksum afterwards, which does not change the bytes already copied.
 */
void cReplay::writeCheckSum()
{
    MEM_copy(WriteTmp, (char*)&WriteCheckSum, 8);
    writeSetup((signed char*)WriteTmp, 8);
    WriteState += 1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_load);
#ifdef SKIP_ASM
extern "C" int cReplayCache_load(void* self, char** pp);
extern "C" int cReplayFrame_loadFrame(void* self, char** pp);
void cReplayFrameList_reset(void* self);
extern "C" void cReplayFrameList_addFrame(void* list, void* node);


/**
 * Loads a saved replay from a memory-card buffer and starts playing it.
 *
 * Fails unless its rider count matches the race's AI manager. Sets Loaded, SaveFrameTicks, ReplayError (+0xC != 0),
 * RecordTick and EndRecordTick from the header, clears SaveFrame and PlaybackTick, empties FrameList, then loads each
 * frame (cReplayFrame::loadFrame, from offset 0x28) into a fresh frame added to FrameList and each input cache
 * (cReplayCache::load). Ends in state 8 (kPlaybackLoad) with Cmd 1.
 *
 * @param buf  the saved replay (On Tour char* data)
 * @return true when loaded, false when the rider count differs (On Tour returns bool)
 */
bool cReplay::load(char* buf)
{
    tReplayManSaveHeader* data = (tReplayManSaveHeader*)buf;
    char** pp = &buf;
    if (data->Players != SSXApp->Game->AI->NumberOfPlayersAllocated) {
        return false;
    }
    {
        Loaded = 1;
        SaveFrameTicks = data->SaveFrameTicks;
        ReplayError = data->ReplayError != 0;
        RecordTick = data->RecordTick;
        EndRecordTick = data->EndRecordTick;
        SaveFrame = 0;
        PlaybackTick = 0;
        cReplayFrameList_reset(((char*)&FrameList));
        *pp = (char*)&data[1];
        for (int i = 0; i < data->NumSaveFrames; i++) {
            void* node = allocReplayFrame();
            cReplayFrame_loadFrame(node, pp);
            cReplayFrameList_addFrame(((char*)&FrameList), node);
        }
        for (int i = 0; i < data->Players; i++) {
            if (InputCache[i] != 0) {
                cReplayCache_load(InputCache[i], pp);
            }
        }
        State = 8;
        Cmd = 1;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002718A8);
#ifdef SKIP_ASM
/**
 * Returns its first argument; the variadic body only spills its register arguments and does nothing else.
 *
 * No retail caller was found.
 *
 * @return @p self
 */
extern "C" void* func_002718A8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002718E8);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_18E8 {
    float x, y, z, w;
    sV4_18E8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern char D_004D33F0[], D_004D3400[], D_004D3410[], D_004D3420[], D_004D3430[], D_004D3440[], D_004D3450[], D_004D3460[], D_004D3470[], D_004D3480[], D_004D3490[], D_004D34A0[], D_004D34B0[], D_004D34C0[], D_004D34D0[], D_004D34E0[], D_004D34F0[], D_004D3500[], D_004D3510[], D_004D3520[], D_004D3530[], D_004D3540[], D_004D3550[], D_004D3560[], D_004D3570[], D_004D3580[], D_004D3590[], D_004D35A0[], D_004D35B0[], D_004D35C0[], D_004D35D0[], D_004D35E0[], D_004D35F0[], D_004D3600[], D_004D3610[], D_004D3620[], D_004D3630[], D_004D3640[], D_004D3650[], D_004D3660[], D_004D3670[], D_004D3680[], D_004D3690[], D_004D36A0[], D_004D36B0[], D_004D36C0[], D_004D36D0[], D_004D36E0[], D_004D36F0[], D_004D3700[], D_004D3710[], D_004D3720[], D_004D3730[], D_004D3740[], D_004D3750[], D_004D3760[];
struct sSlot_18E8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_18E8(int n) { a = n; b = n; }
};
extern int D_004A5160;
extern int D_004A5164;
extern sSlot_18E8 D_004A5168, D_004A5170, D_004A5178, D_004A5180, D_004A5188, D_004A5190, D_004A5198, D_004A51A0, D_004A51A8, D_004A51B0, D_004A51B8, D_004A51C0, D_004A51C8, D_004A51D0;

// PORT: the unit declares func_002718E8 as void*(int, int); it returns nothing.
extern "C" void func_002718E8_impl(int initialize, int priority) __asm__("func_002718E8");
/**
 * Runs the replay/replaycache unit's static initialisers: builds its 56 float4 constants and 14 two-word slots.
 *
 * Only for @p initialize set and @p priority 0xFFFF (the g++ static-initialisation convention). It constructs the
 * 16-byte vectors D_004D33F0 to D_004D3760 in place, zeroes D_004A5160 and D_004A5164 and fills the 8-byte slots
 * D_004A5168 to D_004A51D0 with 0 to 13 (each slot's low byte and upper 24 bits both get its number). The unit's global
 * constructor func_002722C0 calls it.
 *
 * @param initialize  non-zero to construct (0 would destroy; nothing is done then)
 * @param priority  the initialisation priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void func_002718E8_impl(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        new (D_004D33F0) sV4_18E8(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D3400) sV4_18E8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D3410) sV4_18E8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D3420) sV4_18E8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D3430) sV4_18E8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D3440) sV4_18E8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D3450) sV4_18E8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D3460) sV4_18E8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D3470) sV4_18E8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D3480) sV4_18E8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D3490) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34A0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D34B0) sV4_18E8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D34C0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34D0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D34E0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D34F0) sV4_18E8(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3500) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3510) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3520) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3530) sV4_18E8(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D3540) sV4_18E8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D3550) sV4_18E8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D3560) sV4_18E8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D3570) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3580) sV4_18E8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D3590) sV4_18E8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D35A0) sV4_18E8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D35B0) sV4_18E8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D35C0) sV4_18E8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D35D0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D35E0) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D35F0) sV4_18E8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D3600) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3610) sV4_18E8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D3620) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3630) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3640) sV4_18E8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D3650) sV4_18E8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D3660) sV4_18E8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D3670) sV4_18E8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D3680) sV4_18E8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D3690) sV4_18E8(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D36A0) sV4_18E8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D36B0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36C0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36D0) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D36E0) sV4_18E8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D36F0) sV4_18E8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D3700) sV4_18E8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D3710) sV4_18E8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D3720) sV4_18E8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D3730) sV4_18E8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D3740) sV4_18E8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D3750) sV4_18E8(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D3760) sV4_18E8(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A5164 = 0;
        D_004A5160 = 0;
        new (&D_004A5168) sSlot_18E8(0);
        new (&D_004A5170) sSlot_18E8(1);
        new (&D_004A5178) sSlot_18E8(2);
        new (&D_004A5180) sSlot_18E8(3);
        new (&D_004A5188) sSlot_18E8(4);
        new (&D_004A5190) sSlot_18E8(5);
        new (&D_004A5198) sSlot_18E8(6);
        new (&D_004A51A0) sSlot_18E8(7);
        new (&D_004A51A8) sSlot_18E8(8);
        new (&D_004A51B0) sSlot_18E8(9);
        new (&D_004A51B8) sSlot_18E8(10);
        new (&D_004A51C0) sSlot_18E8(11);
        new (&D_004A51C8) sSlot_18E8(12);
        new (&D_004A51D0) sSlot_18E8(13);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr__cReplayFramePtr);
#ifdef SKIP_ASM
void operator_delete(int* p);

extern void* cReplayFramePtr_vtbl[];

/**
 * Destroys a replay frame: restores cReplayFramePtr_vtbl and frees the object when bit 0 of @p flags is set (the
 * block itself is not freed here).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cReplayFramePtr__cReplayFramePtr(cReplayFramePtr* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_clearBlock);
#ifdef SKIP_ASM
/**
 * Empties the frame without freeing its block: clears WriteData, ReadData and Size.
 */
void cReplayFramePtr::clearBlock()
{
    WriteData = 0;
    ReadData = 0;
    Size = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_getBlock);
#ifdef SKIP_ASM
/**
 * Returns the frame pointer's data block (Block).
 *
 * @return Block (as an int)
 */
int cReplayFramePtr::getBlock()
{
    return ((int)Block);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_getReadPos);
#ifdef SKIP_ASM
/**
 * Saves the frame pointer's read position: stores ReadData in the position record @p outPos (On Tour tFramePos).
 *
 * cReplayFramePtr_setReadPos restores it. The preview's version asserted Block and ReadData; this one checks nothing.
 *
 * @param outPos  receives the read position
 */
void cReplayFramePtr::getReadPos(int *outPos)
{
    *outPos = ((int)ReadData);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_setReadPos);
#ifdef SKIP_ASM
/**
 * Restores the frame pointer's read position: sets ReadData (+0xC) from the position record @p readPos.
 *
 * The record comes from cReplayFramePtr_getReadPos. The preview's version asserted Block and the saved position; this one checks
 * nothing.
 *
 * @param readPos  the saved read position
 */
void cReplayFramePtr::setReadPos(int *readPos)
{
    ReadData = (char*)((int) *readPos);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_getBlockSaveSize);
#ifdef SKIP_ASM
/**
 * Returns the number of bytes written to the frame block (Size), the amount a save stores.
 *
 * cReplayFramePtr_GetSize returns the same field.
 *
 * @return Size
 */
int cReplayFramePtr::getBlockSaveSize()
{
    return (Size);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_GetSize);
#ifdef SKIP_ASM
/**
 * Returns the number of bytes written to the frame block (Size).
 *
 * Same body as cReplayFramePtr_getBlockSaveSize.
 *
 * @return Size
 */
int cReplayFramePtr::GetSize()
{
    return (Size);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272248);
#ifdef SKIP_ASM
/**
 * Stores @p newValue into the int @p dest points to.
 *
 * One of the small out-of-line copies kept after the cReplayFramePtr accessors; no retail caller was found.
 *
 * @param dest  the int to write
 * @param newValue  the value to store
 */
extern "C" void func_00272248(int *dest, int newValue) {
    *dest = newValue;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272250);
#ifdef SKIP_ASM
/**
 * Stores @p block in the word at +0x4 of @p framePtr (Block, for a cReplayFramePtr).
 *
 * One of the small out-of-line copies kept after the cReplayFramePtr accessors; no retail caller was found.
 *
 * @param framePtr  the object to write
 * @param block  the value to store
 */
extern "C" void func_00272250(cReplayFramePtr* framePtr, int block) {
    framePtr->Block = (char*)block;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFrame__cReplayFrame);
#ifdef SKIP_ASM
void operator_delete(int* p);

extern void* cReplayFramePtr_vtbl[];

/**
 * Destroys a replay frame: installs the base cReplayFramePtr_vtbl and frees the object (operator_delete) when
 * @p flags bit 0 is set.
 *
 * Slot 7 of cReplayFrame_vtbl; the replay block itself is not freed here.
 *
 * @param flags  bit 0 frees the memory
 */
extern "C" void cReplayFrame__cReplayFrame(cReplayFrame* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayHighlight_reset);
#ifdef SKIP_ASM
/**
 * Clears a highlight record: PlayerId = -1, Frame (+0xB0) = 0 and the 0xAC bytes of Stats (+0x4) zeroed.
 *
 * cReplay_cReplay runs it on both Highlight records; cReplay_reset then gives them their player numbers.
 */
void cReplayHighlight::reset()
{
    ((int*)this)[0] = -1;
    ((int*)this)[176 / 4] = 0;
    MEM_fill(((int*)this) + 1, 0, 172);
}
#endif

extern "C" void* func_002718E8(int, int);

//99.38%
INCLUDE_ASM("replay/replaycache", func_002722C0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the replay/replaycache unit's static initialisers: calls func_002718E8(1, 0xFFFF) (the unit's global
 * constructor).
 *
 * @return func_002718E8's (meaningless) result
 */
void* func_002722C0(void* self)
{
    return func_002718E8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptHeap_cScriptHeap__FPvN20);
#ifdef SKIP_ASM
/**
 * Constructs a script heap over a caller-supplied buffer: the whole buffer becomes one free block.
 *
 * Stores @p pHeap in mpHeap (+0x0) and mpTopFreeBlock (+0x8), @p heapSize in mHeapSize (+0x4), and writes the first free block's
 * header: size @p heapSize - 8, no next block. cInSeq_cInSeq builds its heap with it.
 *
 * @param pHeap  the heap buffer
 * @param heapSize  the buffer size in bytes
 * @return the constructed heap (self)
 */
void* cScriptHeap_cScriptHeap(void* vself, void* pHeap, void* heapSize)
{
    cScriptHeap* self = (cScriptHeap*)vself;
    cScriptHeap* heap = (cScriptHeap*)self;
    tFreeBlock* first = (tFreeBlock*)pHeap;
    heap->mpTopFreeBlock = first;
    heap->mpHeap = (char*)pHeap;
    heap->mHeapSize = (int)heapSize;
    first->pNext = 0;
    heap->mpTopFreeBlock->size = (unsigned int)heapSize - 0x8;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptHeap_AllocMem);
#ifdef SKIP_ASM

// First-fit allocation from the heap's free list (8-byte block headers); splits a block that has room to spare.
// PORT: the unit declares it as void*(void*, int); defined under its own name bound to the symbol.
extern "C" void* cScriptHeap_AllocMem_impl(cScriptHeap* h, unsigned int size) __asm__("cScriptHeap_AllocMem");
/**
 * Allocates a block from the script heap (first fit).
 *
 * Rounds @p size up to 4 bytes and walks the address-ordered free list (mpTopFreeBlock) for the first block big enough;
 * a block more than 16 bytes larger is split, the remainder staying free. The block is unlinked and gets an 8-byte
 * header (size, next = 0). Likely bug, shared with On Tour: when nothing fits it returns 8 (a null block plus the
 * header size), not 0.
 *
 * @param heap     the script heap (this)
 * @param size  the number of bytes wanted
 * @return the usable memory, just past the block header
 */
extern "C" void* cScriptHeap_AllocMem_impl(cScriptHeap* heap, unsigned int size)
{
    size = (size + 3) & ~3;
    tFreeBlock* prev = 0;
    tFreeBlock* block = heap->mpTopFreeBlock;
    tFreeBlock* found = 0;
    unsigned int split = size + 16;
    if (block != 0) {
        do {
            if (block->size >= size) {
                if (split < block->size) {
                    // ORIGINAL-PTR: an allocator carving a byte heap: the remainder block starts size + 8 bytes in
                    tFreeBlock* nb = (tFreeBlock*)((char*)block + (size + 8));
                    nb->size = (block->size - size) - 8;
                    nb->pNext = block->pNext;
                    block->pNext = nb;
                    block->size = size;
                }
                found = block;
                if (prev == 0)
                    (*(tFreeBlock**)&heap->mpTopFreeBlock) = found->pNext;
                else
                    prev->pNext = found->pNext;
                block->pNext = 0;
                break;
            }
            prev = block;
            block = block->pNext;
        } while (block != 0);
    }
    return (char*)&found->blockStart;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptHeap_FreeMem);
#ifdef SKIP_ASM
struct sRcBlock {
    int size;
    sRcBlock* next;
};


/**
 * Returns a block to the script heap, merging it with free neighbours.
 *
 * Does nothing for a null pointer. The block header sits 8 bytes before @p pMem. The block is inserted into the
 * address-ordered free list (mpTopFreeBlock); a free block directly before it absorbs it, and it absorbs a free block
 * directly after it. cScriptMemOp's operator delete (func_002724E8) and the script channel and entity Stop methods free
 * through it.
 *
 * @param pMem           memory returned by cScriptHeap_AllocMem
 */
void cScriptHeap::FreeMem(void* pMem)
{
    if (pMem) {
        // ORIGINAL-PTR: an allocator over a byte heap: headers sit 8 bytes before the memory, blocks are size + 8 long
        sRcBlock* block = (sRcBlock*)((char*)pMem - 8);
        if (!(sRcBlock*)mpTopFreeBlock) {
            mpTopFreeBlock = (tFreeBlock*)block;
            return;
        }
        sRcBlock* prev = 0;
        sRcBlock* cur = (sRcBlock*)mpTopFreeBlock;
        do {
            if (block < cur) {
                int size = block->size + 8;
                if ((sRcBlock*)((char*)block + size) == cur) {
                    block->next = cur->next;
                    block->size = size + cur->size;
                    if (!prev)
                        mpTopFreeBlock = (tFreeBlock*)block;
                    else
                        prev->next = block;
                    return;
                }
                if (!prev) {
                    block->next = (sRcBlock*)mpTopFreeBlock;
                    mpTopFreeBlock = (tFreeBlock*)block;
                } else {
                    block->next = prev->next;
                    prev->next = block;
                }
                return;
            }
            int size = cur->size + 8;
            if ((sRcBlock*)((char*)cur + size) == block) {
                cur->size = size + block->size;
                block = cur;
                cur = block->next;
                block->next = 0;
                if (!prev)
                    mpTopFreeBlock = (tFreeBlock*)cur;
                else
                    prev->next = cur;
            } else {
                prev = cur;
                cur = cur->next;
            }
        } while (cur);
        if (!prev) {
            block->next = (sRcBlock*)mpTopFreeBlock;
            mpTopFreeBlock = (tFreeBlock*)block;
        } else {
            block->next = prev->next;
            prev->next = block;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", func_00272488);
#ifdef SKIP_ASM
extern "C" void* cScriptHeap_AllocMem(void* heap, int size);
extern int kScriptHeapPointerSize;

// PORT: real arity is (size, heap); the unit declares func_00272488(void*) for its callers.
extern "C" void* func_00272488_impl(int size, void* heap) __asm__("func_00272488");
/**
 * Allocates @p size bytes from script heap @p heap, with the heap pointer stored in front so the block can be freed
 * alone.
 *
 * Takes size + kScriptHeapPointerSize bytes with cScriptHeap_AllocMem, stores @p heap in the first word and returns the
 * memory after that header. func_002724E8 is the matching free. Declared in the unit with one argument; the real
 * arguments are (size, heap), the form of cScriptMemOp's operator new.
 *
 * @param size  the number of bytes wanted
 * @param heap  the script heap to allocate from
 * @return the usable memory, just past the stored heap pointer
 */
extern "C" void* func_00272488_impl(int size, void* heap)
{
    char* heapBlock = (char*)cScriptHeap_AllocMem(heap, size + kScriptHeapPointerSize);
    *(void**)heapBlock = heap;
    return heapBlock + kScriptHeapPointerSize;
}
#endif

extern "C" void* func_00272488(void* self);

//99.29%
INCLUDE_ASM("replay/replaycache", func_002724C8__FPv);
#ifdef SKIP_ASM
/**
 * Allocates script heap memory by forwarding its arguments (size, heap) to func_00272488.
 *
 * Declared with only self; the size and heap stay in the argument registers. cScriptPlayerMain__cScriptPlayerMain uses it for its entity
 * player array.
 *
 * @return the allocated memory
 */
void* func_002724C8(void* self)
{
    return func_00272488(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_002724E8);
#ifdef SKIP_ASM
extern int kScriptHeapPointerSize;

// PORT: returns void; the unit declares func_002724E8 as returning void*.
extern "C" void func_002724E8_impl(void* p) __asm__("func_002724E8");
/**
 * Frees memory from func_00272488: steps back over the stored heap pointer and returns the block to that script heap.
 *
 * Reads the cScriptHeap pointer kScriptHeapPointerSize bytes before @p userBlock and calls cScriptHeap_FreeMem on it with the
 * block start. The script channel and entity destructors free themselves through it.
 *
 * @param userBlock  the memory func_00272488 returned
 */
extern "C" void func_002724E8_impl(void* userBlock)
{
    char* userBytes = (char*)userBlock;
    char* allocBase = userBytes - kScriptHeapPointerSize;
    (*(cScriptHeap**)allocBase)->FreeMem(allocBase);
}
#endif

extern "C" void* func_002724E8(void* self);

//99.29%
INCLUDE_ASM("replay/replaycache", func_00272510__FPv);
#ifdef SKIP_ASM
/**
 * Frees script heap memory by forwarding to func_002724E8.
 *
 * cScriptPlayerMain_DeInit frees its entity player array (allocated with func_002724C8) through it.
 *
 * @return func_002724E8's (meaningless) result
 */
void* func_00272510(void* self)
{
    return func_002724E8(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00272530);
#ifdef SKIP_ASM
/**
 * Frees script heap memory by forwarding @p mem to func_002724E8.
 *
 * @param mem  the memory func_00272488 returned
 */
extern "C" void func_00272530(void *mem) {
    func_002724E8(mem);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", func_00272550);
#ifdef SKIP_ASM
/**
 * Frees script heap memory by forwarding @p userBlock to func_002724E8.
 *
 * @param userBlock  the memory func_00272488 returned
 */
extern "C" void func_00272550(void *userBlock) {
    func_002724E8(userBlock);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", GetCubicSplineValue);
#ifdef SKIP_ASM
/**
 * Evaluates a cubic spline segment at a parameter value.
 *
 * Returns p[0] * t^3 + p[1] * t^2 + p[2] * t + p[3] by Horner's rule (just p[3] when @p paramValue is 0). The script spline
 * channel (cScriptPlayerChannelSpline_Play, cScriptPlayerChannelSpline_Update) uses it with the frames elapsed since
 * the current key.
 *
 * @param pCoefficientArray  the four coefficients, highest power first
 * @param paramValue  the parameter value
 * @return the spline value
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float GetCubicSplineValue(float* pCoefficientArray, float paramValue)
{
    float result;
    if (paramValue != 0.0f) {
        result = pCoefficientArray[0];
        result = result * paramValue + pCoefficientArray[1];
        result = result * paramValue + pCoefficientArray[2];
        result = result * paramValue + pCoefficientArray[3];
    } else {
        result = pCoefficientArray[3];
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_cScriptPlayerChannelSpline);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelSpline_vtbl[];

/**
 * Constructs a spline channel: stores its vtable and clears the data, owner, header, current-key and next-key fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelSpline_cScriptPlayerChannelSpline(cScriptPlayerChannelSpline* self)
{
    self->mVtbl = cScriptPlayerChannelSpline_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpCurrKey = 0;
    self->mCurrKeyIndex = (unsigned int)0;
    *(int*)&self->mCurrKeyValue = 0;
    self->mpCurrKeyCustom = 0;
    self->mpNextKey = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Play);
#ifdef SKIP_ASM
extern "C" float GetCubicSplineValue(float* coef, float t);


/**
 * Starts a spline channel of an in-game script sequence at its first key and evaluates its value at frame 0.
 *
 * Stores the data and owner, the header (numKeys) and the first key (keyStart), resets mCurrKeyIndex and points
 * the next key at the first key plus its keySize when there are at least two keys. The value is
 * GetCubicSplineValue of the key's coefficients at 0; keys larger than 0x14 bytes carry custom data
 * (customDataStart).
 *
 * @param data  the channel's script data (On Tour pScriptData)
 * @param key  the script player entity that owns the channel (On Tour pOwner)
 * @return true when there is a next key to reach, otherwise false
 */
bool cScriptPlayerChannelSpline::Play(unsigned int* data, int key)
{
    tInSeqSplineChannelHeader* header = (tInSeqSplineChannelHeader*)data;
    char* body = &header->keyStart;
    tInSeqSplineChannelKey* first = (tInSeqSplineChannelKey*)body;
    *(unsigned int**)&mpData = data;
    *(int*)&mpOwner = key;
    mpHeader = header;
    mpCurrKey = first;
    mCurrKeyIndex = 0;
    if (header->numKeys >= 2) {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        mpNextKey = (tInSeqSplineChannelKey*)(body + first->keySize);
    } else {
        mpNextKey = 0;
    }
    mCurrKeyValue = GetCubicSplineValue(&mpCurrKey->splineCoef[0], 0.0f);
    tInSeqSplineChannelKey* keyLoc = mpCurrKey;
    if (keyLoc->keySize >= 21) {
        mpCurrKeyCustom = &keyLoc->customDataStart;
    } else {
        mpCurrKeyCustom = 0;
    }
    return mpNextKey != 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Stop__FPv);
#ifdef SKIP_ASM
/**
 * Stops a spline channel: clears its data, owner, header, keys, key index, value and custom data (+0x8 .. +0x24).
 */
void cScriptPlayerChannelSpline::Stop()
{
    mpData = 0;
    mpOwner = 0;
    mpHeader = 0;
    mpCurrKey = 0;
    mCurrKeyIndex = 0;
    *(int*)&mCurrKeyValue = 0;
    mpCurrKeyCustom = 0;
    mpNextKey = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Update);
#ifdef SKIP_ASM
/**
 * Advances a spline channel to a frame and evaluates the current key's spline there.
 *
 * When the next key's start frame (frameNum) has been reached it becomes the current key (with its custom data)
 * and the index grows; at the last key the value is evaluated at the key's start and 0 is returned. Otherwise the
 * value is GetCubicSplineValue (GetCubicSplineValue) at @p currFrameNum minus the current key's start frame.
 *
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return true while the channel runs, false once it reached its last key (or has none to reach)
 */
bool cScriptPlayerChannelSpline::Update(float currFrameNum)
{
    tInSeqSplineChannelKey* n = mpNextKey;
    if (n == 0) return false;
    if ((float)n->frameNum <= currFrameNum) {
        mpCurrKey = n;
        mpCurrKeyCustom = 0;
        (*(int*)&mCurrKeyIndex)++;
        if (n->keySize >= 21) mpCurrKeyCustom = &n->customDataStart;
        if (((int)mCurrKeyIndex) == mpHeader->numKeys - 1) {
            mpNextKey = 0;
            mCurrKeyValue = GetCubicSplineValue(&mpCurrKey->splineCoef[0], 0.0f);
            return false;
        }
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        char* curr = (char*)mpCurrKey;
        mpNextKey = (tInSeqSplineChannelKey*)(curr + mpCurrKey->keySize);
    }
    tInSeqSplineChannelKey* keyLoc = mpCurrKey;
    mCurrKeyValue = GetCubicSplineValue(&keyLoc->splineCoef[0], currFrameNum - (float)keyLoc->frameNum);
    return true;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_cInSeq);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerMain_cScriptPlayerMain(cScriptPlayerMain* self);
// PORT: array new[] with a heap argument; same symbol as the unit's func_002724C8(void*).
void* operator new[](unsigned int size, void* heap) __asm__("func_002724C8__FPv");

// A pool node as new[] builds it: tScriptMainNode with its element constructor (cScriptPlayerMain's, on node + 8)
// and a sized operator delete[] (with it new[] keeps an array cookie in front of the nodes).
struct tScriptMainNodeNew : tScriptMainNode {
    tScriptMainNodeNew() { cScriptPlayerMain_cScriptPlayerMain(&script); }
    void operator delete[](void* p, unsigned int size);
};


/**
 * Constructs the in-game sequence (InSeq) manager: a heap over the caller's buffer and a pool of script
 * players.
 *
 * Sets up the heap at +0x8 over @p pHeap (cScriptHeap_cScriptHeap), stores @p tag at +0x0 and @p count at +0x4, then
 * allocates @p count 0x58-byte script nodes from that heap (each runs cScriptPlayerMain_cScriptPlayerMain()
 * on node + 8), numbers them and links them all into the free list (+0x18); the active list (+0x1C) starts
 * empty.
 *
 * @param pHeap     start of the memory the manager's heap manages
 * @param heapSize  size of that memory in bytes
 * @param count     number of script players to create (On Tour maxScripts)
 * @param tag       despite the name, the comm interface (On Tour pCommInterface):
 *                  cScriptPlayerMain_GetInSeqCommInterface() falls back to this word (+0x0)
 * @return @p self
 */
extern "C" cInSeq* cInSeq_cInSeq(cInSeq* self, void* pHeap, void* heapSize, unsigned int count, int tag)
{
    cScriptHeap_cScriptHeap(((char*)&self->mHeap), pHeap, heapSize);
    *(int*)&self->mpCommInterface = tag;
    self->mMaxScripts = count;
    self->mpScriptHeap = 0;
    self->mpScriptUnused = 0;
    tScriptMainNode** slot = &self->mpScriptHeap;
    *slot = new (((char*)&self->mHeap)) tScriptMainNodeNew[count];
    for (unsigned int i = 0; i < self->mMaxScripts; i++) {
        self->mpScriptHeap[i].index = i;
        self->mpScriptHeap[i].pNext = &self->mpScriptHeap[i + 1];
        self->mpScriptHeap[i].pPrev = &self->mpScriptHeap[i - 1];
    }
    self->mpScriptHeap[0].pPrev = 0;
    self->mpScriptHeap[self->mMaxScripts - 1].pNext = 0;
    *(int*)&self->mpScriptUsed = 0;
    self->mpScriptUnused = self->mpScriptHeap;
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq__cInSeq);
#ifdef SKIP_ASM
void operator_delete(int* p);

/**
 * Destroys an input sequence; the only work is freeing the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cInSeq__cInSeq(cInSeq* self, int flags)
{
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_UpdateFrame);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_UpdateFrame(cScriptPlayerMain* p);

/**
 * Updates every playing script: cScriptPlayerMain::UpdateFrame (cScriptPlayerMain_UpdateFrame) on each node of mpScriptUsed.
 */
void cInSeq::UpdateFrame()
{
    tScriptMainNode* n = mpScriptUsed;
    while (((char*)n) != 0) {
        n->script.UpdateFrame();
        n = (tScriptMainNode*)n->pNext;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_ReadyPlayer);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerMain_Init(void* data, void* src, void* owner, int id);



/**
 * Readies a script player for a script (On Tour ReadyPlayer(const char* pScriptData)).
 *
 * Pops the first node of the unused list (mpScriptUnused), initialises its cScriptPlayerMain with the script, the
 * sequencer and the node's id (cScriptPlayerMain_Init, cScriptPlayerMain::Init) and pushes it on the used list (mpScriptUsed).
 * There is no check for an empty unused list.
 *
 * @param src  the script data (On Tour pScriptData)
 * @return the readied player's id
 */
int cInSeq::ReadyPlayer(void* src)
{
    tScriptMainNode* n = mpScriptUnused;
    *(tScriptMainNode**)&mpScriptUnused = n->pNext;
    n->pNext = 0;
    if (mpScriptUnused != 0) {
        mpScriptUnused->pPrev = 0;
    }
    cScriptPlayerMain_Init(((char*)&n->script), src, this, n->index);
    n->pNext = mpScriptUsed;
    if (mpScriptUsed != 0) {
        mpScriptUsed->pPrev = n;
    }
    *(tScriptMainNode**)&mpScriptUsed = n;
    return n->index;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_RecyclePlayer);
#ifdef SKIP_ASM
extern "C" void cInSeq_Stop(cInSeq* self, int a1);
extern "C" void cScriptPlayerMain_DeInit(cScriptPlayerMain* p);

/**
 * Gives a script player back to the free pool.
 *
 * Calls cInSeq_Stop() on it, shuts its cScriptPlayerMain down (cScriptPlayerMain_DeInit), unlinks its 0x58-byte node from
 * mpScriptUsed and pushes it on mpScriptUnused.
 *
 * @param playerID  the player id
 */
void cInSeq::RecyclePlayer(int playerID)
{
    Stop(playerID);
    tScriptMainNode* nodes = mpScriptHeap;
    tScriptMainNode* n = &nodes[playerID];
    n->script.DeInit();
    if (n->pPrev == 0) {
        *(tScriptMainNode**)&mpScriptUsed = n->pNext;
        n->pNext = 0;
        if (mpScriptUsed != 0) {
            mpScriptUsed->pPrev = 0;
        }
    } else {
        n->pPrev->pNext = n->pNext;
        if (n->pNext != 0) {
            n->pNext->pPrev = n->pPrev;
        }
        n->pNext = 0;
        n->pPrev = 0;
    }
    n->pNext = mpScriptUnused;
    if (mpScriptUnused != 0) {
        mpScriptUnused->pPrev = n;
    }
    *(tScriptMainNode**)&mpScriptUnused = n;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_RecycleAll);
#ifdef SKIP_ASM
/**
 * Gives every playing script's player back to the free pool (cInSeq_RecyclePlayer, RecyclePlayer, on each used node).
 */
void cInSeq::RecycleAll()
{
    tScriptMainNode* n = mpScriptUsed;
    while (n != 0) {
        tScriptMainNode* next = n->pNext;
        RecyclePlayer(n->index);
        n = next;
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Preview);
#ifdef SKIP_ASM
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);
extern "C" void cScriptPlayerMain_Preview(cScriptPlayerMain* self, int flag);

/**
 * Previews one script player (cScriptPlayerMain_Preview).
 *
 * On Tour's Preview takes a cInSeqComm* (pPreviewCommInterface) as its last argument; here it is an int, passed
 * through unchanged.
 *
 * @param playerID  the script player
 * @param flag  the preview argument (On Tour cInSeqComm* pPreviewCommInterface)
 */
void cInSeq::Preview(int playerID, int flag)
{
    ((cScriptPlayerMain*)cInSeq_GetPlayer(this, playerID))->Preview(flag);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Play);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerMain_Play1(void*, int);
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);

/**
 * Plays one script player (cScriptPlayerMain_Play1, cScriptPlayerMain::Play).
 *
 * @param playerID  the script player
 * @param loopScript  nonzero to loop the script
 * @return cScriptPlayerMain_Play1's result (On Tour's Play returns void)
 */
void* cInSeq::Play(int playerID, int loopScript)
{
    return cScriptPlayerMain_Play1(GetPlayer(playerID), loopScript);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_PlayAll);
#ifdef SKIP_ASM
/**
 * Plays every script player on the used list (mpScriptUsed, linked through +0x50) with cInSeq_Play.
 *
 * Each node's id (+0x4) is passed with @p loopScript.
 *
 * @param loopScript  non-zero to loop the scripts
 */
void cInSeq::PlayAll(int loopScript)
{
    tScriptMainNode* node;

    node = (tScriptMainNode*)((void*)mpScriptUsed);
    if (((void*)node) != 0) {
        do {
            Play(node->index, loopScript);
            node = (tScriptMainNode*)(((void*)node->pNext));
        } while (((void*)node) != 0);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Stop);
#ifdef SKIP_ASM
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int a1);
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* p);

/**
 * Stops one script player (cScriptPlayerMain_Stop).
 *
 * @param playerID  the script player
 */
void cInSeq::Stop(int playerID)
{
    ((cScriptPlayerMain*)cInSeq_GetPlayer(this, playerID))->Stop();
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_StopAll);
#ifdef SKIP_ASM
/**
 * Stops every playing script player (cInSeq_Stop on each node of mpScriptUsed).
 */
void cInSeq::StopAll()
{
    tScriptMainNode* n;
    for (n = mpScriptUsed; n != 0; n = n->pNext) {
        Stop(n->index);
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Pause);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self);

/**
 * Pauses one script player (cScriptPlayerMain_Pause).
 *
 * @param playerID  the script player
 */
void cInSeq::Pause(int playerID)
{
    ((cScriptPlayerMain*)cInSeq_GetPlayer(this, playerID))->Pause();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_PauseAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Pause(cInSeq* self, int a1);

/**
 * Pauses every playing script player (cInSeq_Pause on each node of mpScriptUsed).
 */
void cInSeq::PauseAll()
{
    tScriptMainNode* n;
    for (n = mpScriptUsed; n != 0; n = n->pNext) {
        Pause(n->index);
    }
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_Resume);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(cScriptPlayerMain* self);

/**
 * Resumes one script player (cScriptPlayerMain_Resume).
 *
 * @param playerID  the script player
 */
void cInSeq::Resume(int playerID)
{
    ((cScriptPlayerMain*)cInSeq_GetPlayer(this, playerID))->Resume();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_ResumeAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Resume(cInSeq* self, int a1);

/**
 * Resumes every playing script player (cInSeq_Resume on each node of mpScriptUsed).
 */
void cInSeq::ResumeAll()
{
    tScriptMainNode* n;
    for (n = mpScriptUsed; n != 0; n = n->pNext) {
        Resume(n->index);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetPlayer);
#ifdef SKIP_ASM
/**
 * Returns the script player of pool slot @p playerID (mpScriptHeap[playerID].script) when it has script data
 * (mpData).
 *
 * @param playerID  the slot
 * @return the cScriptPlayerMain, or 0 when that slot's player is not set up
 */
void* cInSeq::GetPlayer(int playerID)
{
    tScriptMainNode* nodes = mpScriptHeap;
    tScriptMainNode* playerNode = &nodes[playerID];
    return playerNode->script.mpData != 0 ? &playerNode->script : 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetScriptDataProjectID);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_GetProjectID();

/**
 * Returns the project id of an InSeq script by forwarding the script data to cScriptPlayerMain_GetProjectID (header word +0x4).
 *
 * Declared without arguments: the script data pointer passes through in $a0. No retail caller was found.
 */
extern "C" void cInSeq_GetScriptDataProjectID(void) {
    cScriptPlayerMain_GetProjectID();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetScriptDataSize);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_GetDataSize();

/**
 * Returns the size of an InSeq script by forwarding the script data to cScriptPlayerMain_GetDataSize (header word +0x8).
 *
 * Declared without arguments: the script data pointer passes through in $a0. No retail caller was found.
 */
extern "C" void cInSeq_GetScriptDataSize(void) {
    cScriptPlayerMain_GetDataSize();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetScriptDataNumEntities);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_GetNumEntities();

/**
 * Returns the entity count of an InSeq script by forwarding the script data to cScriptPlayerMain_GetNumEntities (header byte +0x1).
 *
 * Declared without arguments: the script data pointer passes through in $a0. No retail caller was found.
 */
extern "C" void cInSeq_GetScriptDataNumEntities(void) {
    cScriptPlayerMain_GetNumEntities();
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_GetScriptDataNumEntityVariations);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetNumVariations1(int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the number of variations of one entity of an InSeq script.
 *
 * Declared without arguments: the script data and entity index pass through in $a0/$a1 to cScriptPlayerMain_GetEntityScriptData, which finds
 * the entity's block; cScriptPlayerEntity_GetNumVariations1 reads its numVariations. No retail caller was found.
 */
extern "C" void cInSeq_GetScriptDataNumEntityVariations(void) {
    cScriptPlayerEntity_GetNumVariations1(cScriptPlayerMain_GetEntityScriptData());
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_cScriptPlayerChannelBlock);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelBlock_vtbl[];

/**
 * Constructs a block channel: stores its vtable and clears the data, owner, header, next-key, block-array and
 * active-block fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelBlock_cScriptPlayerChannelBlock(cScriptPlayerChannelBlock* self)
{
    self->mVtbl = cScriptPlayerChannelBlock_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpBlockArray = 0;
    self->mpActiveBlockList = 0;
    self->mNumActiveBlocks = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock__cScriptPlayerChannelBlock);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelBlock_Stop(cScriptPlayerChannelBlock* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelBlock_vtbl[];

/**
 * Destroys a block channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelBlock__cScriptPlayerChannelBlock(cScriptPlayerChannelBlock* self, int flags)
{
    self->mVtbl = cScriptPlayerChannelBlock_vtbl;
    self->Stop();
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Play);
#ifdef SKIP_ASM
struct sCacheRecord;
struct sCacheEntry;
extern "C" void* cScriptHeap_AllocMem(void* heap, int size);
extern "C" sCacheEntry* cScriptPlayerChannelBlock_AddActiveKey(cScriptPlayerChannelBlock* self, sCacheRecord* rec);
extern "C" void cScriptPlayerChannelBlock_LoadNextScriptKey(cScriptPlayerChannelBlock* self);

/**
 * Starts a block channel of an in-game script sequence and activates every key that starts at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] active-key records of 0x1C bytes from
 * the owner's script heap (cScriptHeap_AllocMem, On Tour cScriptHeap::AllocMem), resets the index and starts at the first
 * key (data + 4). Each key whose start frame (+4) is 0 sets 0x4 in @p flags and is activated (cScriptPlayerChannelBlock_AddActiveKey
 * AddActiveKey, cScriptPlayerChannelBlock_LoadNextScriptKey LoadNextScriptKey).
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, active-key count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return true when the channel has keys, otherwise false
 */
bool cScriptPlayerChannelBlock::Play(unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    mpData = (const char*)data;
    mpOwner = (cScriptPlayerEntity*)owner;
    mpHeader = (tInSeqBlockChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        mpBlockArray = (tBlockEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 0x1C);
        mNextKeyIndex = 0;
        tInSeqBlockChannelHeader* base = mpHeader;
        mpNextKey = (tInSeqBlockChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqBlockChannelKey*)&base->keyStart)->frameStart == 0) do {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)this, (sCacheRecord*)mpNextKey);
            LoadNextScriptKey();
        } while (mpNextKey != 0 &&
               mpNextKey->frameStart == 0);
    }
    return mpHeader->numKeys != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Stop);
#ifdef SKIP_ASM
/**
 * Stops a block channel: frees its active-key buffer to the owner's script heap and clears its state (only when it
 * has an owner).
 */
void cScriptPlayerChannelBlock::Stop()
{
    cScriptPlayerEntity* owner = mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = mpBlockArray;
        if (heapBlock != 0) {
            owner->mpOwner->mpManager->mHeap.FreeMem(heapBlock);
        }
        mpData = 0;
        mpOwner = 0;
        mpHeader = 0;
        mpNextKey = 0;
        mNextKeyIndex = 0;
        mpBlockArray = 0;
        mpActiveBlockList = 0;
        mNumActiveBlocks = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Update);
#ifdef SKIP_ASM



/**
 * Advances a block channel to a frame: drops active keys that have ended and activates every key that has started.
 *
 * Each active key whose end frame has passed is unlinked (0x8 in @p flags) and the active count drops; then every
 * pending key whose start frame is reached sets 0x4 and is activated (AddActiveKey, LoadNextScriptKey).
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return false when no key is active or pending, otherwise true
 */
bool cScriptPlayerChannelBlock::Update(int* flags, float currFrameNum)
{
    float zeroF;
    tBlockEntry* prev = 0;
    tBlockEntry* event = mpActiveBlockList;
    while (event != 0) {
        if ((float)event->data.scriptEndFrame <= currFrameNum) {
            *flags |= 8;
            if (prev == 0) {
                event = *(tBlockEntry**)&mpActiveBlockList = event->pNext;
            } else {
                event = event->pNext;
                prev->pNext = event;
            }
            mNumActiveBlocks--;
        } else {
            prev = event;
            event = event->pNext;
        }
    }
    zeroF = 0.0f;
loop:
    do {
        if (mpNextKey != 0 && currFrameNum - (float)(mpNextKey)->frameStart >= zeroF) {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)this, (sCacheRecord*)(mpNextKey));
            LoadNextScriptKey();
            goto loop;
        }
    } while (0);
    if (mpActiveBlockList == 0 && mpNextKey == 0)
        return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetNextKeyFrame);
#ifdef SKIP_ASM
struct sCacheTimeNode {
    char pad[0xC];
    int time;
    char pad2[0x18 - 0x10];
    sCacheTimeNode* next;
};

/**
 * Returns the earliest frame at which the block channel changes: the soonest end of an active key or the next key's
 * start.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or nothing is left
 */
int cScriptPlayerChannelBlock::GetNextKeyFrame()
{
    if ((int)mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)mpHeader == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode* n = (sCacheTimeNode*)mpActiveBlockList;
    unsigned short* cur = (unsigned short*)mpNextKey;
    for (; n != 0; n = n->next) {
        int keyTime = n->time;
        if (best < 0 || keyTime < best) {
            best = keyTime;
        }
    }
    if (cur != 0) {
        int keyTime = cur[2];
        if (best < 0 || keyTime < best) {
            best = keyTime;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_AddActiveKey);
#ifdef SKIP_ASM
struct sCacheRecord {
    unsigned short size;
    unsigned short id;
    unsigned short a;
    unsigned short b;
    int x;
    int y;
};

struct sCacheEntry {
    int x;
    int id;
    int a;
    int b;
    int y;
    sCacheRecord* data;
    sCacheEntry* next;
};

/**
 * Starts a block key: fills mpBlockArray[key id] from the key and pushes it on mpActiveBlockList.
 *
 * Copies the key's fields, points the entry's data at the bytes after the key's 0x10-byte header when the key is
 * larger, and counts mNumActiveBlocks up.
 *
 * @param rec  the key (On Tour tInSeqBlockChannelKey* pKey)
 * @return the entry (On Tour tBlockEntry*)
 */
extern "C" sCacheEntry* cScriptPlayerChannelBlock_AddActiveKey(cScriptPlayerChannelBlock* self, sCacheRecord* rec)
{
    sCacheEntry* newKey = (sCacheEntry*)&self->mpBlockArray[rec->id];
    newKey->x = rec->x;
    newKey->id = rec->id;
    newKey->y = rec->y;
    newKey->a = rec->a;
    newKey->b = rec->b;
    newKey->data = 0;
    if (rec->size > 0x10) {
        newKey->data = rec + 1;
    }
    newKey->next = (sCacheEntry*)self->mpActiveBlockList;
    self->mpActiveBlockList = (tBlockEntry*)newKey;
    (self->mNumActiveBlocks)++;
    return newKey;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
void cScriptPlayerChannelBlock::LoadNextScriptKey()
{
    int idx = mNextKeyIndex;
    if (idx == mpHeader->numKeys - 1) {
        mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqBlockChannelKey* key = mpNextKey;
        char* cur = (char*)key;
        mpNextKey = (tInSeqBlockChannelKey*)(cur + key->keySize);
        mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_cScriptPlayerChannelEvent);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelEvent_vtbl[];

/**
 * Constructs an event channel: stores its vtable and clears the data, owner, header, next-key, event-array fields,
 * mNumActiveEvents and mEventFrame.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelEvent_cScriptPlayerChannelEvent(cScriptPlayerChannelEvent* self)
{
    self->mVtbl = cScriptPlayerChannelEvent_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpEventArray = 0;
    self->mNumActiveEvents = 0;
    *(int*)&self->mEventFrame = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent__cScriptPlayerChannelEvent);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelEvent_Stop(cScriptPlayerChannelEvent* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelEvent_vtbl[];

/**
 * Destroys an event channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelEvent__cScriptPlayerChannelEvent(cScriptPlayerChannelEvent* self, int flags)
{
    self->mVtbl = cScriptPlayerChannelEvent_vtbl;
    self->Stop();
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Play);
#ifdef SKIP_ASM
struct sCacheRec;
extern "C" void* cScriptHeap_AllocMem(void* heap, int size);
extern "C" tEventEntry* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self,
                                                               tInSeqEventChannelKey* rec);
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self);

/**
 * Starts an event channel of an in-game script sequence and fires every event key at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] records of 8 bytes from the owner's
 * script heap (cScriptHeap_AllocMem), resets the index and starts at the first key (data + 4). Each key whose frame (+2)
 * is 0 sets 0x10 in @p flags and is fired (cScriptPlayerChannelEvent_AddActiveKey AddActiveKey, cScriptPlayerChannelEvent_LoadNextScriptKey LoadNextScriptKey); the
 * last-fired frame (+0x24) is reset each time.
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, buffer count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return true while a key is still pending, otherwise false
 */
bool cScriptPlayerChannelEvent::Play(unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    mpData = (const char*)data;
    mpOwner = (cScriptPlayerEntity*)owner;
    mpHeader = (tInSeqEventChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        mpEventArray = (tEventEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 8);
        mNextKeyIndex = 0;
        tInSeqEventChannelHeader* base = mpHeader;
        mpNextKey = (tInSeqEventChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqEventChannelKey*)&base->keyStart)->frameNum == 0) do {
            *flags |= 0x10;
            AddActiveKey(mpNextKey);
            LoadNextScriptKey();
            *(int*)&mEventFrame = 0;
        } while (mpNextKey != 0 &&
               mpNextKey->frameNum == 0);
    }
    return mpNextKey != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Stop);
#ifdef SKIP_ASM
/**
 * Stops an event channel: frees its key buffer to the owner's script heap and clears its state (only when it has an
 * owner).
 */
void cScriptPlayerChannelEvent::Stop()
{
    cScriptPlayerEntity* owner = mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = mpEventArray;
        if (heapBlock != 0) {
            owner->mpOwner->mpManager->mHeap.FreeMem(heapBlock);
        }
        mpData = 0;
        mpOwner = 0;
        mpHeader = 0;
        mpNextKey = 0;
        mNextKeyIndex = 0;
        mpEventArray = 0;
        mNumActiveEvents = 0;
        *(int*)&mEventFrame = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Update);
#ifdef SKIP_ASM
struct sCacheRec;
extern "C" tEventEntry* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self,
                                                               tInSeqEventChannelKey* rec);
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self);

/**
 * Advances an event channel to a frame and fires every event key that has been reached.
 *
 * When @p currFrameNum differs from the last fired frame (+0x24) the per-frame count (+0x20) is cleared. Each pending key
 * whose frame is reached sets 0x10 in @p flags, is fired (AddActiveKey), records its frame and loads the next key.
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param currFrameNum  the sequence's current frame (On Tour currFrameNum)
 * @return true while a key is still pending, otherwise false
 */
bool cScriptPlayerChannelEvent::Update(int* flags, float currFrameNum)
{
    if (mEventFrame != currFrameNum) {
        mNumActiveEvents = 0;
    }
    while ((char*)mpNextKey != 0) {
        float keyFrameNum = (float)mpNextKey->frameNum;
        if (currFrameNum < keyFrameNum) break;
        *flags |= 0x10;
        AddActiveKey(mpNextKey);
        mEventFrame = keyFrameNum;
        LoadNextScriptKey();
    }
    return (char*)mpNextKey != 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetNextKeyFrame);
#ifdef SKIP_ASM
/**
 * Returns the frame of the event channel's next pending key.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or none is pending
 */
int cScriptPlayerChannelEvent::GetNextKeyFrame()
{
    if ((int)mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)mpHeader == 0) {
        return -1;
    }
    unsigned short* cur = (unsigned short*)mpNextKey;
    int nextKeyFrame = -1;
    if (cur) {
        nextKeyFrame = cur[1];
    }
    return nextKeyFrame;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_AddActiveKey);
#ifdef SKIP_ASM


/**
 * Fires an event key: fills mpEventArray[mNumActiveEvents] with the key's id and its payload (the bytes after the
 * 8-byte header, when any), and counts mNumActiveEvents up.
 *
 * @param rec  the key (On Tour tInSeqEventChannelKey* pKey)
 * @return the entry (On Tour tEventEntry*)
 */
tEventEntry* cScriptPlayerChannelEvent::AddActiveKey(tInSeqEventChannelKey* rec)
{
    tEventEntry* newKey = &mpEventArray[mNumActiveEvents];
    newKey->eventID = rec->enumVal;
    newKey->pCustom = 0;
    if (rec->keySize > 8) {
        newKey->pCustom = &rec->customDataStart;
    }
    mNumActiveEvents += 1;
    return newKey;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
void cScriptPlayerChannelEvent::LoadNextScriptKey()
{
    int idx = mNextKeyIndex;
    if (idx == mpHeader->numKeys - 1) {
        mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqEventChannelKey* key = mpNextKey;
        char* cur = (char*)key;
        mpNextKey = (tInSeqEventChannelKey*)(cur + key->keySize);
        mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_cScriptPlayerChannelObject);
#ifdef SKIP_ASM
extern void* cScriptPlayerChannelObject_vtbl[];

/**
 * Constructs an object channel: stores its vtable and clears the data, owner, header, next-key, object-array and
 * active-object fields.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerChannelObject_cScriptPlayerChannelObject(cScriptPlayerChannelObject* self)
{
    self->mVtbl = cScriptPlayerChannelObject_vtbl;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpNextKey = 0;
    self->mNextKeyIndex = 0;
    self->mpObjectArray = 0;
    self->mpActiveObjectList = 0;
    self->mNumActiveObjects = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject__cScriptPlayerChannelObject);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerChannelObject_Stop(cScriptPlayerChannelObject* self);
extern "C" void* func_002724E8(void* self);
extern void* cScriptPlayerChannelObject_vtbl[];

/**
 * Destroys an object channel: restores its vtable (+0x4), stops it (freeing its key buffer) and, when bit 0 of @p
 * flags is set, frees it with func_002724E8 (On Tour cScriptMemOp::operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerChannelObject__cScriptPlayerChannelObject(cScriptPlayerChannelObject* self, int flags)
{
    self->mVtbl = cScriptPlayerChannelObject_vtbl;
    self->Stop();
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Play);
#ifdef SKIP_ASM
extern "C" void* cScriptHeap_AllocMem(void* heap, int size);
struct sCacheRecord2;
struct sCacheEntry2;
extern "C" sCacheEntry2* cScriptPlayerChannelObject_AddActiveKey(cScriptPlayerChannelObject* self, sCacheRecord2* rec);
extern "C" void cScriptPlayerChannelObject_LoadNextScriptKey(cScriptPlayerChannelObject* self);

/**
 * Starts an object channel of an in-game script sequence and activates every key that starts at frame 0.
 *
 * Stores the data and owner; when the channel has keys it allocates data[1] active-key records of 0x24 bytes from
 * the owner's script heap (cScriptHeap_AllocMem; an empty loop over them follows), resets the index and starts at the
 * first key. Each key whose start frame (+4) is 0 sets 0x1 in @p flags and is activated (cScriptPlayerChannelObject_AddActiveKey
 * AddActiveKey, cScriptPlayerChannelObject_LoadNextScriptKey LoadNextScriptKey).
 *
 * @param data  the channel's script data (On Tour pScriptData): key count, active-key count, keys
 * @param owner  the script player entity that owns the channel (On Tour pOwner)
 * @param flags  the sequence's event bits (On Tour int& events)
 * @return true when the channel has keys, otherwise false
 */
bool cScriptPlayerChannelObject::Play(unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    mpData = (const char*)data;
    mpOwner = (cScriptPlayerEntity*)owner;
    mpHeader = (tInSeqObjectChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        mpObjectArray = (tObjectEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 36);
        for (int i = 0; i < mpHeader->numTracks; i++) {
        }
        mNextKeyIndex = 0;
        tInSeqObjectChannelHeader* base = mpHeader;
        mpNextKey = (tInSeqObjectChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqObjectChannelKey*)&base->keyStart)->frameStart == 0) do {
            *flags |= 1;
            cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)this, (sCacheRecord2*)mpNextKey);
            LoadNextScriptKey();
        } while (mpNextKey != 0 &&
               mpNextKey->frameStart == 0);
    }
    return mpHeader->numKeys != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Stop);
#ifdef SKIP_ASM
/**
 * Stops an object channel: frees its active-key buffer to the owner's script heap and clears its state (only when
 * it has an owner).
 */
void cScriptPlayerChannelObject::Stop()
{
    cScriptPlayerEntity* owner = mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = mpObjectArray;
        if (heapBlock != 0) {
            owner->mpOwner->mpManager->mHeap.FreeMem(heapBlock);
        }
        mpData = 0;
        mpOwner = 0;
        mpHeader = 0;
        mpNextKey = 0;
        mNextKeyIndex = 0;
        mpObjectArray = 0;
        mpActiveObjectList = 0;
        mNumActiveObjects = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Update);
#ifdef SKIP_ASM



/**
 * Advances an object channel to a frame: moves active keys on, drops ended ones and activates every key that has
 * started.
 *
 * Active keys that have ended are unlinked (0x2 in @p flags); the others advance their value by speed * @p dt. Each
 * pending key whose start frame is reached sets 0x1, is activated and has its value advanced by the frames it is
 * late (t - start) times its speed.
 *
 * @param flags  the sequence's event bits (On Tour int& events)
 * @param frameDelta  the sequence's current frame (On Tour currFrameNum)
 * @param dt  the frames since the last update (On Tour frameDelta)
 * @return false when no key is active or pending, otherwise true
 */
bool cScriptPlayerChannelObject::Update(int* flags, float frameDelta, float dt)
{
    float zeroF;
    tObjectEntry* prev = 0;
    tObjectEntry* event = mpActiveObjectList;
    while (event != 0) {
        if ((float)event->data.scriptEndFrame <= frameDelta) {
            *flags |= 2;
            if (prev == 0) {
                event = *(tObjectEntry**)&mpActiveObjectList = event->pNext;
            } else {
                event = event->pNext;
                prev->pNext = event;
            }
            mNumActiveObjects--;
        } else {
            prev = event;
            event->data.currObjectFrame += event->data.playRate * dt;
            event = event->pNext;
        }
    }
    zeroF = 0.0f;
loop:
    do {
        if (mpNextKey != 0) {
            float frameDiff = frameDelta - (float)(mpNextKey)->frameStart;
            if (frameDiff >= zeroF) {
                *flags |= 1;
                tObjectEntry* n = (tObjectEntry*)cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)this, (sCacheRecord2*)(mpNextKey));
                n->data.currObjectFrame += frameDiff * n->data.playRate;
                LoadNextScriptKey();
                goto loop;
            }
        }
    } while (0);
    if (mpActiveObjectList == 0 && mpNextKey == 0)
        return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetNextKeyFrame);
#ifdef SKIP_ASM
struct sCacheTimeNode2 {
    char pad[0x8];
    int time;
    char pad2[0x20 - 0xC];
    sCacheTimeNode2* next;
};

/**
 * Returns the earliest frame at which the object channel changes: the soonest end of an active key or the next
 * key's start.
 *
 * @return that frame, or -1 when the channel is not playing, has no keys or nothing is left
 */
int cScriptPlayerChannelObject::GetNextKeyFrame()
{
    if ((int)mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)mpHeader == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode2* n = (sCacheTimeNode2*)mpActiveObjectList;
    unsigned short* cur = (unsigned short*)mpNextKey;
    for (; n != 0; n = n->next) {
        int keyTime = n->time;
        if (best < 0 || keyTime < best) {
            best = keyTime;
        }
    }
    if (cur != 0) {
        int keyTime = cur[2];
        if (best < 0 || keyTime < best) {
            best = keyTime;
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_AddActiveKey);
#ifdef SKIP_ASM
struct sCacheRecord2 {
    unsigned short size;
    unsigned short id;
    unsigned short a;
    unsigned short b;
    int x;
    int ix;
    float f;
    int y;
};

struct sCacheEntry2 {
    int x;
    int a;
    int b;
    int id;
    float fx;
    float f;
    int y;
    sCacheRecord2* data;
    sCacheEntry2* next;
};

/**
 * Starts an object key: fills mpObjectArray[key id] from the key and pushes it on mpActiveObjectList.
 *
 * Copies the key's fields, points the entry's data at the bytes after the key's 0x18-byte header when the key is
 * larger, and counts mNumActiveObjects up.
 *
 * @param rec  the key (On Tour tInSeqObjectChannelKey* pKey)
 * @return the entry (On Tour tObjectEntry*)
 */
extern "C" sCacheEntry2* cScriptPlayerChannelObject_AddActiveKey(cScriptPlayerChannelObject* self, sCacheRecord2* rec)
{
    sCacheEntry2* newKey = (sCacheEntry2*)&self->mpObjectArray[rec->id];
    newKey->x = rec->x;
    newKey->y = rec->y;
    newKey->fx = rec->ix;
    newKey->f = rec->f;
    newKey->id = rec->id;
    newKey->a = rec->a;
    newKey->b = rec->b;
    newKey->data = 0;
    if (rec->size > 0x18) {
        newKey->data = rec + 1;
    }
    newKey->next = (sCacheEntry2*)self->mpActiveObjectList;
    self->mpActiveObjectList = (tObjectEntry*)newKey;
    (self->mNumActiveObjects)++;
    return newKey;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
void cScriptPlayerChannelObject::LoadNextScriptKey()
{
    int idx = mNextKeyIndex;
    if (idx == mpHeader->numKeys - 1) {
        mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqObjectChannelKey* key = mpNextKey;
        char* cur = (char*)key;
        mpNextKey = (tInSeqObjectChannelKey*)(cur + key->keySize);
        mNextKeyIndex = idx + 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_cScriptPlayerEntity);
#ifdef SKIP_ASM
/**
 * Constructs an InSeq script entity player with no script, channels or entity comm interface.
 *
 * Zeroes mpData through mpEntityComm (+0x4 to +0x2C); unk_00 and the three setup flags (+0x30 to +0x38) are
 * left alone.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerEntity_cScriptPlayerEntity(cScriptPlayerEntity* self)
{
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpUserData = 0;
    self->mpBlockHeader = 0;
    self->mpVariationTable = 0;
    self->mCurrVariation = 0;
    self->mpSystemHeader = 0;
    self->mpHeader = 0;
    self->mpChannelPlayers = 0;
    self->mIsPlaying = 0;
    self->mpEntityComm = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity__cScriptPlayerEntity);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_DeInit(cScriptPlayerEntity* self);

/**
 * Destroys an entity player: runs cScriptPlayerEntity_DeInit(), then frees the object when asked to.
 *
 * The memory goes back to the cInSeq heap it came from, through func_002724E8.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerEntity__cScriptPlayerEntity(cScriptPlayerEntity* self, int flags)
{
    self->DeInit();
    if (flags & 1) {
        func_002724E8(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Init);
#ifdef SKIP_ASM
/**
 * Binds the entity player to its block of script data and selects variation 0.
 *
 * Stores @p data as mpData and mpBlockHeader, the owner, the variation table (data + 8) and the first
 * variation's header (data + table[0]); mpHeader points at that variation's custom header when its headerSize
 * is above 8, else 0.
 *
 * @param data  the entity's script block (a tInSeqScriptEntityBlockHeader first)
 * @param size  despite the name, the owning cScriptPlayerMain (On Tour pOwner), stored in mpOwner
 * @return the block's blockSize, so the caller can step to the next entity block
 */
int cScriptPlayerEntity::Init(tInSeqScriptEntityBlockHeader* data, int size)
{
    mpData = (const char*)data;
    mpBlockHeader = (tInSeqScriptEntityBlockHeader*)data;
    mpOwner = (cScriptPlayerMain*)size;
    mpVariationTable = (unsigned int*)(&data->varOffsetTableStart);
    mCurrVariation = 0;
    // ORIGINAL-PTR: the variation offset table holds byte offsets into the entity data
    tInSeqEntityHeader* rec = (tInSeqEntityHeader*)(((char*)data) + (int)data->varOffsetTableStart);
    mpSystemHeader = rec;
    if (rec->headerSize > 8) {
        mpHeader = (const char*)(&rec->nextSectionStart);
    } else {
        mpHeader = 0;
    }
    return mpBlockHeader->blockSize;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_DeInit);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self);

/**
 * Unbinds the entity player from its script data, stopping it first.
 *
 * Only acts when mpData is set: calls cScriptPlayerEntity_Stop(), then clears the data, owner, user data,
 * header and variation fields and the three setup flags (mSetupScriptHasKeysLeft,
 * mSetupScriptInitialEventBits, mWaitingPlayExec).
 */
void cScriptPlayerEntity::DeInit()
{
    if ((int)mpData != 0) {
        Stop();
        mpData = 0;
        mpOwner = 0;
        mpUserData = 0;
        mpBlockHeader = 0;
        mpVariationTable = 0;
        mCurrVariation = 0;
        mpSystemHeader = 0;
        mpHeader = 0;
        mSetupScriptHasKeysLeft = 0;
        mSetupScriptInitialEventBits = 0;
        mWaitingPlayExec = 0;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetEntityVariation);
#ifdef SKIP_ASM
/**
 * Selects which variation of the entity's script is played.
 *
 * Stores @p idx in mCurrVariation and points mpSystemHeader at mpData + mpVariationTable[idx]; mpHeader is
 * the variation's custom header when its headerSize is above 8, else 0. The index is not range-checked.
 *
 * @param idx  variation index (On Tour varIndex)
 */
void cScriptPlayerEntity::SetEntityVariation(int idx)
{
    mCurrVariation = idx;
    // ORIGINAL-PTR: the variation offset table holds byte offsets into the entity data
    tInSeqEntityHeader* rec = (tInSeqEntityHeader*)(mpData + mpVariationTable[idx]);
    mpSystemHeader = rec;
    if (rec->headerSize > 8) {
        mpHeader = (const char*)(&rec->nextSectionStart);
    } else {
        mpHeader = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetupPlay);
#ifdef SKIP_ASM
extern "C" void* cScriptHeap_AllocMem(void* heap, int size);
// PORT: real arity is (size, heap); the unit declares func_00272488(void*) for its callers.
extern "C" void* func_00272488_impl(int size, void* heap) __asm__("func_00272488");
extern "C" void* cScriptPlayerChannelSpline_cScriptPlayerChannelSpline(cScriptPlayerChannelSpline* self);
extern "C" void* cScriptPlayerChannelBlock_cScriptPlayerChannelBlock(cScriptPlayerChannelBlock* self);
extern "C" void* cScriptPlayerChannelEvent_cScriptPlayerChannelEvent(cScriptPlayerChannelEvent* self);
extern "C" void* cScriptPlayerChannelObject_cScriptPlayerChannelObject(cScriptPlayerChannelObject* self);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);

/**
 * Creates and starts the channel players of one script entity.
 *
 * Clears mSetupScriptHasKeysLeft (+0x30) and the event bits (+0x34). For an entity with channels it allocates
 * mpChannelPlayers (+0x24) from the cInSeq heap (mpOwner->mpManager + 8, cScriptHeap_AllocMem) and, per channel record
 * (type +0x0, size +0x2), creates a spline (0x28 bytes), object (0x28), block (0x2C) or event (0x28) channel with
 * cScriptMemOp's operator new (func_00272488), then starts it through its vtable slot 1 (Play), ORing in whether keys
 * are left and its event bits. Sets mIsPlaying (+0x28), asks the comm interface
 * (cScriptPlayerMain_GetInSeqCommInterface, slot 6) for the entity's comm object (mpEntityComm, +0x2C) and notifies it
 * (slot 1), and sets +0x38.
 *
 * @return mIsPlaying (1)
 */
int cScriptPlayerEntity::SetupPlay()
{
    mSetupScriptHasKeysLeft = 0;
    mSetupScriptInitialEventBits = 0;
    if (mpSystemHeader->numChannels) {
        void* heap = (char*)&mpOwner->mpManager->mHeap;
        mpChannelPlayers =
            (cScriptPlayerChannel**)cScriptHeap_AllocMem(heap, mpSystemHeader->numChannels * 4);
        // ORIGINAL-PTR: the channel records follow the variation header (headerSize bytes) and are dataSize bytes long
        char* channelData = (char*)(mpSystemHeader) + mpSystemHeader->headerSize;
        for (int i = 0; i < mpSystemHeader->numChannels; i++) {
            tInSeqEntityChannelHeader* channelHeader = (tInSeqEntityChannelHeader*)channelData;
            channelData += 4;
            // &mpChannelPlayers[i] in integer form below: the subscript changes retail's register allocation
            switch (channelHeader->type) {
            case kChannelSpline: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelSpline_cScriptPlayerChannelSpline((cScriptPlayerChannelSpline*)func_00272488_impl(40, heap));
                break;
            }
            case kChannelObject: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelObject_cScriptPlayerChannelObject((cScriptPlayerChannelObject*)func_00272488_impl(40, heap));
                break;
            }
            case kChannelBlock: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelBlock_cScriptPlayerChannelBlock((cScriptPlayerChannelBlock*)func_00272488_impl(44, heap));
                break;
            }
            case kChannelEvent: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelEvent_cScriptPlayerChannelEvent((cScriptPlayerChannelEvent*)func_00272488_impl(40, heap));
                break;
            }
            }
            int flag = 0;
            int* pd = &mSetupScriptHasKeysLeft;
            int keysLeft = 0;
            cScriptPlayerChannel* channel = mpChannelPlayers[i];
            if (channel->Play(channelData, this, &flag) || mSetupScriptHasKeysLeft)
                keysLeft = 1;
            *pd = keysLeft;
            mSetupScriptInitialEventBits |= flag;
            channelData += channelHeader->dataSize;
        }
    }
    mIsPlaying = 1;
    cInSeqComm* comm = (cInSeqComm*)mpOwner->GetInSeqCommInterface();
    if (comm) {
        mpEntityComm = comm->OnInSeqRequestEntityCommInterface(this, mpSystemHeader->type);
    }
    if (mpEntityComm) {
        cInSeqCommEntity* entityComm = mpEntityComm;
        entityComm->OnInSeqLock(this, mpSystemHeader->type);
    }
    mWaitingPlayExec = 1;
    return mIsPlaying;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_ExecutePlay);
#ifdef SKIP_ASM
/**
 * Finishes a pending play: reports the setup's initial events to the entity comm interface and clears the
 * setup state.
 *
 * With an entity comm interface (mpEntityComm) it calls OnInSeqKeyEvent (slot 3) when
 * mSetupScriptInitialEventBits is nonzero, then OnInSeqUpdate (slot 4), with the entity type and frame 0.
 * Then clears mSetupScriptHasKeysLeft, mSetupScriptInitialEventBits and mWaitingPlayExec.
 *
 * @return mSetupScriptHasKeysLeft as it was: nonzero when a channel still has keys to play
 */
int cScriptPlayerEntity::ExecutePlay()
{
    cInSeqCommEntity* obj = mpEntityComm;
    int ret = mSetupScriptHasKeysLeft;
    if (obj != 0) {
        int eventBits = mSetupScriptInitialEventBits;
        if (eventBits != 0) {
            obj->OnInSeqKeyEvent(this, mpSystemHeader->type, 0.0f, eventBits);
        }
        mpEntityComm->OnInSeqUpdate(this,
            mpSystemHeader->type,
            0.0f, 0.0f, mSetupScriptInitialEventBits);
    }
    mSetupScriptHasKeysLeft = 0;
    mSetupScriptInitialEventBits = 0;
    mWaitingPlayExec = 0;
    return ret;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Stop);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_002724E8_impl(void* p) __asm__("func_002724E8");
extern "C" void cScriptHeap_FreeMem(cScriptHeap* h, void* p);

class cRcObj_3DC0 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void Stop();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int GetType();
    virtual void v08();
    virtual void v09();
    virtual void v10(int);
};

class cRcListener_3DC0 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* src, int id);
    virtual void OnInSeqRelease(void* src, int id);
};

/**
 * Stops the entity's playback: releases its comm interface and deletes all of its channel players.
 *
 * Does nothing unless mIsPlaying. Calls the comm interface's OnInSeqRelease (slot 2) and clears mpEntityComm;
 * each channel then gets Stop (slot 2) and is deleted by type: spline channels (no destructor) through
 * func_002724E8, the others through their virtual destructor (slot 10, flags 3). The channel array goes back
 * to the owner's cInSeq heap (cScriptHeap_FreeMem) and mIsPlaying is cleared.
 */
void cScriptPlayerEntity::Stop()
{
    if (mIsPlaying == 0)
        return;
    cRcListener_3DC0* entityComm = (cRcListener_3DC0*)mpEntityComm;
    if (entityComm != 0)
        entityComm->OnInSeqRelease(this, mpSystemHeader->type);
    mpEntityComm = 0;
    if (mpSystemHeader->numChannels != 0)
    {
        for (int i = 0; i < mpSystemHeader->numChannels; i++)
        {
            cRcObj_3DC0** objs = (cRcObj_3DC0**)mpChannelPlayers;
            objs[i]->Stop();
            switch (((cRcObj_3DC0*)mpChannelPlayers[i])->GetType())
            {
            case kChannelSpline:
                func_002724E8_impl(((cRcObj_3DC0*)mpChannelPlayers[i]));
                break;
            case kChannelObject:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            case kChannelBlock:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            case kChannelEvent:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            }
        }
        mpOwner->mpManager->mHeap.FreeMem(mpChannelPlayers);
        mpChannelPlayers = 0;
    }
    mIsPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Restart);
#ifdef SKIP_ASM
/**
 * Restarts every channel of a playing entity from the start of its data and reports the initial events.
 *
 * Walks the channel records after the variation header (4-byte record header, size at +2): each channel gets
 * Stop (slot 2), then Play(record + 4, self, &bits) (slot 1), and the event bits are ORed. With an entity
 * comm interface it then calls OnInSeqKeyEvent (slot 3, only when bits were raised) and OnInSeqUpdate (slot
 * 4) at frame 0.
 *
 * @return nonzero when any channel's Play returned nonzero (still has keys to play); 0 when not playing
 */
int cScriptPlayerEntity::Restart()
{
    if (mIsPlaying == 0)
        return 0;
    tInSeqEntityHeader* hdr = mpSystemHeader;
    int mask = 0;
    int i = 0;
    int changed = 0;
    // ORIGINAL-PTR: the channel records follow the variation header (headerSize bytes) and are dataSize bytes long
    char* channelData = (char*)hdr + hdr->headerSize;
    if (hdr->numChannels != 0)
    {
        do
        {
            tInSeqEntityChannelHeader* rec = (tInSeqEntityChannelHeader*)channelData;
            int ch = 0;
            channelData += 4;
            mpChannelPlayers[i]->Stop();
            int channelEvents = 0;
            if (mpChannelPlayers[i]->Play(channelData, this, &channelEvents) != 0 || changed)
                ch = 1;
            i++;
            channelData += rec->dataSize;
            changed = ch;
            mask |= channelEvents;
        } while (i < mpSystemHeader->numChannels);
    }
    cInSeqCommEntity* entityComm = mpEntityComm;
    if (entityComm == 0)
        return changed;
    if (mask)
        entityComm->OnInSeqKeyEvent(this, mpSystemHeader->type, 0.0f, mask);
    mpEntityComm->OnInSeqUpdate(this, mpSystemHeader->type, 0.0f, 0.0f, mask);
    return changed;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_Update);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerEntity_DoFrameUpdate(cScriptPlayerEntity* self, float t, float dt);

/**
 * Advances one script entity's channels to a new frame.
 *
 * Returns 1 at once when the entity is not playing (mIsPlaying). Without @p jump, a single
 * cScriptPlayerEntity_DoFrameUpdate(@p time, @p speed). With @p jump it walks from @p prev to @p time in steps that
 * stop at every channel key in between (each channel's next key time, vtable slot 6) and updates at each, stopping
 * early when an update returns 0.
 *
 * @param prev   the previous frame number
 * @param time   the new frame number
 * @param speed  the frame delta
 * @param jump   nonzero to visit every key between @p prev and @p time
 * @return the last cScriptPlayerEntity_DoFrameUpdate result (1 when not playing)
 */
int cScriptPlayerEntity::Update(float prev, float time, float speed, int jump)
{
    cScriptPlayerEntity* self = this;
    if (self->mIsPlaying == 0) {
        return 1;
    }
    if (jump) {
        for (;;) {
            float next = time;
            for (int i = 0; i < (self->mpSystemHeader)->numChannels; i++) {
                float channelNextTime = (float)self->mpChannelPlayers[i]->GetNextKeyFrame();
                if (channelNextTime >= 0.0f && channelNextTime < next) {
                    next = channelNextTime;
                }
            }
            if (next == time) {
                return self->DoFrameUpdate(time, time - prev);
            }
            float delta = next - prev;
            prev = next;
            if (!self->DoFrameUpdate(prev, delta)) {
                return 0;
            }
        }
    }
    return self->DoFrameUpdate(time, speed);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetUserData);
#ifdef SKIP_ASM
/**
 * Replaces the entity's mpUserData with @p userData and returns the previous value.
 *
 * @param userData  the new user data
 * @return the old mpUserData
 */
int cScriptPlayerEntity::SetUserData(int userData)
{
    int temp_2;

    temp_2 = ((int)mpUserData);
    mpUserData = (void*)userData;
    return temp_2;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetEntityComm);
#ifdef SKIP_ASM
/**
 * Replaces the entity's comm interface, releasing the old one and locking the new one.
 *
 * Calls OnInSeqRelease (slot 2) on the old interface and OnInSeqLock (slot 1) on the new one, each with the
 * entity and its type (mpSystemHeader->type).
 *
 * @param pEntityComm  the new entity comm interface (On Tour cInSeqCommEntity*), or 0 for none
 * @return the previous interface
 */
cInSeqCommEntity* cScriptPlayerEntity::SetEntityComm(cInSeqCommEntity* pEntityComm)
{
    cInSeqCommEntity* old = mpEntityComm;
    if (old != 0) {
        old->OnInSeqRelease(this, mpSystemHeader->type);
    }
    mpEntityComm = pEntityComm;
    if (pEntityComm != 0) {
        pEntityComm->OnInSeqLock(this, mpSystemHeader->type);
    }
    return old;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetChannel__FPvi);
#ifdef SKIP_ASM
/**
 * Returns one of the entity's channel players.
 *
 * Reads mpChannelPlayers[@p channelIndex] (+0x24, the table cScriptPlayerEntity_SetupPlay fills); no bounds check.
 *
 * @param channelIndex  the channel index
 * @return the channel player (cScriptPlayerChannel*)
 */
int cScriptPlayerEntity::GetChannel(int channelIndex)
{
    return (int)mpChannelPlayers[channelIndex];
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetHeader);
#ifdef SKIP_ASM
/**
 * Returns the custom header of variation @p i of the entity's script, or 0 when the variation has none.
 *
 * The variation's tInSeqEntityHeader is at mpData + mpVariationTable[@p i]; when its headerSize is above 8 the custom
 * header starts at +0x8 (nextSectionStart). The script engine's per-type getters (GetSSXCameraManualHeader1 and the like) use it.
 *
 * @param i  the variation index
 * @return the custom header, or 0
 */
void* cScriptPlayerEntity::GetHeader(int i)
{
    int* table = (int*)mpVariationTable;
    const char* entityData = mpData + table[i];  // the table holds byte offsets into the entity data
    tInSeqEntityHeader* entityHeader = (tInSeqEntityHeader*)entityData;
    if (entityHeader->headerSize > 8) {
        return &entityHeader->nextSectionStart;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetHeader1);
#ifdef SKIP_ASM
/**
 * Returns the custom header of variation @p idx of an entity's script data block, or 0 when it has none.
 *
 * The variation header is at @p base + the variation offset table entry (@p base + 8 + 4 * @p idx); with a headerSize
 * above 8 the custom header starts at its +0x8. The script engine passes the block cScriptPlayerMain_GetEntityScriptData found.
 *
 * @param base  the entity's script data block (tInSeqScriptEntityBlockHeader)
 * @param idx  the variation index
 * @return the custom header, or 0
 */
extern "C" void* cScriptPlayerEntity_GetHeader1(char* base, int idx)
{
    tInSeqScriptEntityBlockHeader* block = (tInSeqScriptEntityBlockHeader*)base;
    unsigned int* table = &block->varOffsetTableStart;
    char* entityData = base + (int)table[idx];  // a byte offset into the block
    tInSeqEntityHeader* entityHeader = (tInSeqEntityHeader*)entityData;
    if (entityHeader->headerSize > 8)
        return &entityHeader->nextSectionStart;
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetScriptLength__FPv);
#ifdef SKIP_ASM
/**
 * Returns the frame count of the entity's current variation (mpSystemHeader->numFrames).
 *
 * cScriptPlayerMain_GetScriptLength takes the maximum over all entities.
 *
 * @return the length in frames
 */
int cScriptPlayerEntity::GetScriptLength()
{
    return mpSystemHeader->numFrames;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", func_00274358);
#ifdef SKIP_ASM
/**
 * Returns the frame count of the entity's current variation, looked up through the variation table.
 *
 * Reads numFrames from the header at mpData + mpVariationTable[mCurrVariation]; the same value cScriptPlayerEntity_GetScriptLength reads
 * through mpSystemHeader once a variation is set. No retail caller was found.
 *
 * @return the length in frames
 */
extern "C" int func_00274358(cScriptPlayerEntity* self)
{
    const char* base = self->mpData;
    int* tab = (int*)self->mpVariationTable;
    int i = self->mCurrVariation;
    const char* entityData = base + tab[i];  // the table holds byte offsets into the entity data
    return ((tInSeqEntityHeader*)entityData)->numFrames;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetCurrVariation);
#ifdef SKIP_ASM
/**
 * Returns the entity's current variation index (mCurrVariation).
 *
 * @return mCurrVariation
 */
int cScriptPlayerEntity::GetCurrVariation()
{
    return (mCurrVariation);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetNumVariations__FPv);
#ifdef SKIP_ASM
/**
 * Returns how many variations the entity's script block has (mpBlockHeader->numVariations).
 *
 * cSSXScriptEngine_OnStartScript uses it to pick a variation.
 *
 * @return the variation count
 */
int cScriptPlayerEntity::GetNumVariations()
{
    return (int)mpBlockHeader->numVariations;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetNumVariations1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cScriptPlayerEntity_GetNumVariations1_impl(tInSeqScriptEntityBlockHeader* blockHeader)
    __asm__("cScriptPlayerEntity_GetNumVariations1");
/**
 * Returns the variation count (+0x4) of an entity script data block (tInSeqScriptEntityBlockHeader).
 *
 * @param blockHeader  the entity's script data block
 * @return its numVariations
 */
extern "C" int cScriptPlayerEntity_GetNumVariations1_impl(tInSeqScriptEntityBlockHeader* blockHeader) {
    return blockHeader->numVariations;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetProjectID);
#ifdef SKIP_ASM
/**
 * Returns the project id of the script this entity belongs to (mpOwner's mpSystemHeader, word +0x4).
 *
 * @return the script's projectID
 */
int cScriptPlayerEntity::GetProjectID()
{
    return ((int)mpOwner->mpSystemHeader->projectID);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetDataSize);
#ifdef SKIP_ASM
/**
 * Returns the size in bytes of the entity's script data block (mpBlockHeader->blockSize).
 *
 * @return the block size
 */
int cScriptPlayerEntity::GetDataSize()
{
    return *((int*)mpBlockHeader);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetDataSize1);
#ifdef SKIP_ASM
/**
 * Returns the size (blockSize, word +0x0) of an entity script data block.
 *
 * cScriptPlayerMain_GetEntityScriptData steps over entity blocks with it.
 *
 * @param blockHeader  the entity's script data block
 * @return its blockSize
 */
extern "C" int cScriptPlayerEntity_GetDataSize1(int *blockHeader) {
    return *blockHeader;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the type of the entity's current variation (mpSystemHeader->type).
 *
 * The script engines (cSSXScriptEngine_OnStartScript, cScriptEntityCamera_OnInSeqLock) and cScriptCameraController_addCamera switch on it
 * to tell entity kinds apart (5 in OnStartScript; manual, target, subject and algorithmic cameras).
 *
 * @return the entity type
 */
unsigned char cScriptPlayerEntity::GetType()
{
    return mpSystemHeader->type;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetType1);
#ifdef SKIP_ASM
/**
 * Returns the entity type of an entity script data block: the type byte of the variation header its first table entry
 * (+0x8) points to.
 *
 * No retail caller was found.
 *
 * @param blockHeader  the entity's script data block
 * @return the type byte
 */
extern "C" unsigned char cScriptPlayerEntity_GetType1(tInSeqScriptEntityBlockHeader* blockHeader) {
    char* block = (char*)blockHeader;
    char* entityData = block + (int)blockHeader->varOffsetTableStart;  // a byte offset into the block
    return ((tInSeqEntityHeader*)entityData)->type;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_DoFrameUpdate);
#ifdef SKIP_ASM

/**
 * Advances every channel of the entity to a frame and passes the resulting events to the entity comm
 * interface.
 *
 * Calls each channel's Update (slot 3) with @p currFrameNum and @p dt, ORing the results and the event bits
 * they raise. With an entity comm interface it then calls OnInSeqKeyEvent (slot 3, only when bits were
 * raised) and OnInSeqUpdate (slot 4) with the frame, the step and the entity type.
 *
 * @param currFrameNum  script frame to update to
 * @param dt            frame step since the last update (On Tour frameDelta)
 * @return nonzero while any channel is still playing (its Update returned true)
 */
int cScriptPlayerEntity::DoFrameUpdate(float currFrameNum, float dt)
{
    cScriptPlayerEntity* self = this;
    int flags = 0;
    bool any = false;
    for (int i = 0; i < (self->mpSystemHeader)->numChannels; i++) {
        int out = 0;
        any = self->mpChannelPlayers[i]->Update(currFrameNum, dt, &out) | any;
        flags |= out;
    }
    cInSeqCommEntity* entityComm = self->mpEntityComm;
    if (entityComm != 0) {
        if (flags != 0) {
            entityComm->OnInSeqKeyEvent(self, self->mpSystemHeader->type, currFrameNum, flags);
        }
        self->mpEntityComm->OnInSeqUpdate(self, self->mpSystemHeader->type, currFrameNum, dt, flags);
    }
    return any;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_cScriptPlayerMain);
#ifdef SKIP_ASM
/**
 * Constructs a stopped InSeq script player with no script loaded.
 *
 * mID, mScriptLength and mJumpFrame start at -1 and mPlaySpeed at 1.0; every other field (data, manager,
 * entity players, play state, frame, pause count, update flags, preview interface) is 0.
 *
 * @return @p self
 */
extern "C" void* cScriptPlayerMain_cScriptPlayerMain(cScriptPlayerMain* self)
{
    self->mID = -1;
    self->mpData = 0;
    self->mpManager = 0;
    self->mpUserData = 0;
    self->mpSystemHeader = 0;
    self->mpDataHeader = 0;
    self->mpEntityPlayers = 0;
    self->mLoopScript = 0;
    self->mPlayState = 0;
    self->mPlaySpeed = 1.0f;
    *(int*)&self->mCurrFrame = 0;
    self->mNumPaused = 0;
    self->mScriptLength = -1;
    self->mJumpFrame = -1;
    self->mNumSteps = 0;
    self->mEntityUpdateInProgress = 0;
    self->mDoStopInEntityUpdate = 0;
    self->mpPreviewCommInterface = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain__cScriptPlayerMain);
#ifdef SKIP_ASM
/**
 * Destroys a script player: shuts it down with cScriptPlayerMain_DeInit and frees the object when asked.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptPlayerMain__cScriptPlayerMain(cScriptPlayerMain* self, int flags)
{
    ((cScriptPlayerMain*)self)->DeInit();
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Init);
#ifdef SKIP_ASM
// PORT: array new[] with a heap argument; same symbol as the unit's func_002724C8(void*).
void* operator new[](unsigned int size, void* heap) __asm__("func_002724C8__FPv");

// An entity player as new[] builds it: cScriptPlayerEntity with its element constructor
// (cScriptPlayerEntity_cScriptPlayerEntity) and a sized operator delete[] (with it new[] keeps an array cookie).
struct cScriptPlayerEntityNew : cScriptPlayerEntity {
    cScriptPlayerEntityNew() { cScriptPlayerEntity_cScriptPlayerEntity(this); }
    void operator delete[](void* p, unsigned int size);
};



// PORT: cScriptPlayerEntity_Init's third argument receives the owning stream pointer as an int.
// PORT: the unit declares cScriptPlayerMain_Init as returning void*; it returns the header word.
extern "C" int cScriptPlayerMain_Init_impl(cScriptPlayerMain* self, tInSeqScriptHeader* data, char* owner, int a3) __asm__("cScriptPlayerMain_Init");
/**
 * Binds a script player to its script data and creates one entity player per script entity.
 *
 * Stores @p playerId in mID, @p owner in mpManager and @p data in mpData and mpSystemHeader; mJumpFrame = -1, mNumSteps = 0,
 * mLoopScript = 0; mpDataHeader = @p data + 0xC when the header is at least 0xD bytes. With entities it allocates
 * numEntities 0x3C-byte cScriptPlayerEntity objects from the manager's heap (owner + 8, cScriptMemOp's operator new[]
 * func_002724C8), constructs them, and calls cScriptPlayerEntity_Init on each with the data that follows the header,
 * each consuming its block. cInSeq_ReadyPlayer calls it.
 *
 * @param data   the script data (On Tour pScriptData)
 * @param owner  the owning cInSeq manager (On Tour pManager)
 * @param playerId     the player id
 * @return the script header's size word (+0x8)
 */
extern "C" int cScriptPlayerMain_Init_impl(cScriptPlayerMain* self, tInSeqScriptHeader* data, char* owner, int playerId)
{
    self->mID = playerId;
    *(char**)&self->mpManager = owner;
    *(tInSeqScriptHeader**)&self->mpData = data;
    *(tInSeqScriptHeader**)&self->mpSystemHeader = data;
    self->mJumpFrame = -1;
    self->mNumSteps = 0;
    self->mLoopScript = 0;
    if (data->headerSize >= 13) {
        *(char**)&self->mpDataHeader = (char*)&data->nextSectionStart;
    }
    // ORIGINAL-PTR: the entity blocks follow the script header (headerSize bytes); each is blockSize bytes long
    char* entityData = (char*)data + (self->mpSystemHeader)->headerSize;
    if (self->mpSystemHeader->numEntities != 0) {
        cScriptPlayerEntity** slot = &self->mpEntityPlayers;
        *slot = new (owner + 8) cScriptPlayerEntityNew[(self->mpSystemHeader)->numEntities];
        for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
            entityData += self->mpEntityPlayers[i].Init((tInSeqScriptEntityBlockHeader*)entityData, (int)self);
        }
    }
    return ((int)(self->mpSystemHeader)->size);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_DeInit);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity__cScriptPlayerEntity(cScriptPlayerEntity* self, int flags);
void* func_00272510(void* self);

// The cookie g++ 2.95's new[] keeps in front of an array whose element type has a destructor (16 bytes, the
// element count first); delete[] reads the count from it and frees from its start.
struct tArrayNewCookie {
    int count;
    int pad[3];
};




/**
 * Shuts a script player down: stops it, frees its entities and resets every field.
 *
 * Does nothing when no script is loaded (mpData null). Otherwise calls cScriptPlayerMain_Stop,
 * cScriptPlayerEntity_DeInit on each of the numEntities entities, destroys the entity array back to front (count from
 * the 16-byte array cookie, cScriptPlayerEntity__cScriptPlayerEntity) and frees it with func_00272510. Then clears mID
 * (-1), mpData, mpManager, mpUserData, the header pointers, mLoopScript, mPlayState, mCurrFrame, mNumPaused and
 * mNumSteps, and sets mScriptLength and mJumpFrame to -1 and mPlaySpeed to 1.0. cInSeq_RecyclePlayer and the destructor
 * cScriptPlayerMain__cScriptPlayerMain call it.
 *
 */
void cScriptPlayerMain::DeInit()
{
    cScriptPlayerMain* self = this;
    if (((void*)self->mpData) == 0) {
        return;
    }
    self->Stop();
    for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
        ((self->mpEntityPlayers)[i]).DeInit();
    }
    // delete[] self->mpEntityPlayers (element count in the 16-byte array cookie)
    cScriptPlayerEntity* scriptPlayerEntity = self->mpEntityPlayers;
    if (scriptPlayerEntity != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)scriptPlayerEntity;
        cScriptPlayerEntity* entity = scriptPlayerEntity + cookies[-1].count;
        while (self->mpEntityPlayers != entity) {
            entity--;
            cScriptPlayerEntity__cScriptPlayerEntity(entity, 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)self->mpEntityPlayers;
        func_00272510(&array[-1]);
    }
    self->mNumSteps = 0;
    self->mID = -1;
    *(void**)&self->mpData = 0;
    *(void**)&self->mpManager = 0;
    *(int*)&self->mpUserData = 0;
    *(tInSeqScriptHeader**)&self->mpSystemHeader = 0;
    *(char**)&self->mpDataHeader = 0;
    *(cScriptPlayerEntity**)&self->mpEntityPlayers = 0;
    self->mLoopScript = 0;
    self->mPlayState = 0;
    *(int*)&self->mCurrFrame = 0;
    self->mNumPaused = 0;
    self->mScriptLength = -1;
    self->mPlaySpeed = 1.0f;
    self->mJumpFrame = -1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_SetPlaySpeed__FPvf);
#ifdef SKIP_ASM
/**
 * Sets the script player's playback speed (mPlaySpeed).
 *
 * The BX script engine sets it from a play request's rate (cBXScriptEngine_SetScriptPlayRate, cBXScriptEngine_UpdatePendingRequestState).
 *
 * @param val  the new speed (1.0 = normal)
 */
void cScriptPlayerMain::SetPlaySpeed(float val)
{
    mPlaySpeed = val;
}
#endif

extern "C" void* cScriptPlayerMain_Play1(void*, int);

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Play__FPv);
#ifdef SKIP_ASM
/**
 * Starts the script player with its current loop setting: calls cScriptPlayerMain_Play1 with mLoopScript.
 *
 * cScriptPlayerMain_UpdateFrame calls it to restart a looping script.
 *
 * @return cScriptPlayerMain_Play1's (meaningless) result
 */
void* cScriptPlayerMain::Play()
{
    return cScriptPlayerMain_Play1(this, mLoopScript);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Play1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// the unit declares this as returning void*; the body returns nothing,
// so bind a void body to the symbol
void cScriptPlayerMain_Play1_impl(void* self, int a1) __asm__("cScriptPlayerMain_Play1");

/**
 * Starts the script player: a stopped player becomes PlayPending (mPlayState 1) with mLoopScript set to @p loopScript.
 *
 * If the player is not stopped and an entity update is in progress (mEntityUpdateInProgress), a stop requested during
 * it (mDoStopInEntityUpdate) is cancelled instead; otherwise a running player is left alone. cInSeq_Play,
 * cInSeq_Preview and cScriptPlayerMain_Play call it.
 *
 * @param loopScript  non-zero to loop the script
 */
void cScriptPlayerMain_Play1_impl(void* vself, int loopScript)
{
    cScriptPlayerMain* self = (cScriptPlayerMain*)vself;
    if (self->mPlayState != 0) {
        if (self->mEntityUpdateInProgress != 0) {
            self->mDoStopInEntityUpdate = 0;
        }
    } else {
        self->mLoopScript = loopScript;
        self->mPlayState = 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_SetLoop);
#ifdef SKIP_ASM
/**
 * Sets whether the script player loops (mLoopScript).
 *
 * @param loopScript  non-zero to loop
 */
void cScriptPlayerMain::SetLoop(int loopScript)
{
    mLoopScript = loopScript;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Preview);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cScriptPlayerMain_Play1_v(void*, int) __asm__("cScriptPlayerMain_Play1");
extern "C" void cScriptPlayerMain_UpdateFrame(cScriptPlayerMain* p);

/**
 * Runs the whole script at once through a preview comm interface, then restores the player's speed and loop
 * setting.
 *
 * Sets mpPreviewCommInterface, starts playback without looping (cScriptPlayerMain_Play1, which only starts a stopped
 * player) and calls the frame update cScriptPlayerMain_UpdateFrame at speed 1.0 until the play state is back to stopped; then
 * restores mPlaySpeed and mLoopScript and clears the preview interface.
 *
 * @param flag  despite the name, the comm interface to preview through (On Tour pPreviewCommInterface)
 */
void cScriptPlayerMain::Preview(int flag)
{
    float savedScale = mPlaySpeed;
    mpPreviewCommInterface = (cInSeqComm*)flag;
    mPlaySpeed = 1.0f;
    int savedIdx = mLoopScript;
    cScriptPlayerMain_Play1_v(((char*)this), 0);
    while (mPlayState != 0) {
        mPlaySpeed = 1.0f;
        UpdateFrame();
    }
    mPlaySpeed = savedScale;
    mLoopScript = savedIdx;
    mpPreviewCommInterface = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GoToFrame);
#ifdef SKIP_ASM
/**
 * Asks the script player to jump to frame @p frameNum on its next update (mJumpFrame) and clears mNumSteps.
 *
 * @param frameNum  the frame to jump to
 */
void cScriptPlayerMain::GoToFrame(int frameNum)
{
    mJumpFrame = frameNum;
    mNumSteps = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_StepNextFrame);
#ifdef SKIP_ASM
/**
 * Asks the script player to advance one more frame step (increments mNumSteps).
 *
 */
void cScriptPlayerMain::StepNextFrame()
{
    mNumSteps = (int) (mNumSteps + 1);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Stop);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);

/**
 * Stops the script: stops every entity and resets the play state, frame and cached length.
 *
 * Does nothing when already stopped. During an entity update (mEntityUpdateInProgress) it only sets
 * mDoStopInEntityUpdate, so the update stops afterwards. Otherwise it stops each entity
 * (cScriptPlayerEntity_Stop), zeroes mPlayState, mCurrFrame, mNumPaused and mNumSteps, sets mScriptLength and
 * mJumpFrame to -1 and calls the comm interface's slot 4 (OnInSeqStop by On Tour's order).
 */
void cScriptPlayerMain::Stop()
{
    if (mPlayState == 0) return;
    if (mEntityUpdateInProgress != 0) {
        mDoStopInEntityUpdate = 1;
        return;
    }
    for (int i = 0; i < mpSystemHeader->numEntities; i++) {
        mpEntityPlayers[i].Stop();
    }
    mPlayState = 0;
    *(int*)&mCurrFrame = 0;
    mNumPaused = 0;
    mNumSteps = 0;
    mScriptLength = -1;
    mJumpFrame = -1;
    cInSeqComm* inSeqComm = (cInSeqComm*)GetInSeqCommInterface();
    if (inSeqComm != 0) {
        inSeqComm->OnInSeqStop(this);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Pause);
#ifdef SKIP_ASM
/**
 * Pauses a playing script by raising its pause count (mNumPaused).
 *
 * Pauses nest: each needs a matching cScriptPlayerMain_Resume(). Nothing happens while stopped.
 */
void cScriptPlayerMain::Pause()
{
    if (mPlayState != 0) {
        mNumPaused += 1;
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_Resume);
#ifdef SKIP_ASM
/**
 * Undoes one pause of a playing script: lowers mNumPaused, not below 0.
 *
 * Nothing happens while stopped.
 */
void cScriptPlayerMain::Resume()
{
    if (mPlayState != 0) {
        int numPaused = mNumPaused - 1;
        mNumPaused = numPaused;
        if (numPaused < 0) {
            mNumPaused = 0;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_UpdateFrame);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self);
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* p);
void* cScriptPlayerMain_Play(void* self);
extern "C" void cScriptPlayerMain_DoUpdateFrame(void* self);
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self);


/**
 * Advances a script player by one frame, honouring a pending jump or single-step request.
 *
 * A pending mJumpFrame (wrapped by the script length when looping) is played up to with mNumPaused cleared: back to the
 * start (Stop, Play, update) when the target is earlier, then a partial step to the next whole frame and whole-frame
 * updates until mCurrFrame reaches it. A pending mNumSteps runs that many updates and then pauses
 * (cScriptPlayerMain_Pause) unless already paused. Otherwise it runs one cScriptPlayerMain_DoUpdateFrame.
 * cInSeq_UpdateFrame calls it for each player.
 *
 */
void cScriptPlayerMain::UpdateFrame()
{
    cScriptPlayerMain* self = this;
    int need = 1;
    if (self->mJumpFrame >= 0)
    {
        int target = self->mJumpFrame;
        self->mJumpFrame = -1;
        int len = self->GetScriptLength();
        int saved = self->mNumPaused;
        self->mNumPaused = 0;
        if (self->mLoopScript != 0)
            target = target % len;
        float ft = (float)target;
        if (self->mPlayState == keScriptPlayState_PlayPending)
            cScriptPlayerMain_DoUpdateFrame(self);
        if (ft != self->mCurrFrame)
        {
            if (ft < self->mCurrFrame)
            {
                self->Stop();
                self->Play();
                cScriptPlayerMain_DoUpdateFrame(self);
            }
            float one = 1.0f;
            float pos = self->mCurrFrame;
            float frac = (float)((int)pos + 1) - pos;
            float speed = self->mPlaySpeed;
            if (frac < one)
            {
                self->mPlaySpeed = frac;
                cScriptPlayerMain_DoUpdateFrame(self);
            }
            self->mPlaySpeed = one;
            while (self->mPlayState != 0 && self->mCurrFrame < ft)
                cScriptPlayerMain_DoUpdateFrame(self);
            self->mPlaySpeed = speed;
        }
        if (self->mPlayState != 0)
            self->mNumPaused = saved;
        need = 0;
    }
    if (self->mNumSteps != 0)
    {
        int steps = self->mNumSteps;
        int saved2 = self->mNumPaused;
        self->mNumSteps = 0;
        self->mNumPaused = 0;
        if (self->mPlayState != 0)
        {
            do
            {
                cScriptPlayerMain_DoUpdateFrame(self);
                steps--;
                if (self->mPlayState == 0)
                    goto skip;
            } while (steps != 0);
            self->mNumPaused = saved2;
        }
    skip:
        if (self->mNumPaused <= 0)
            self->Pause();
        need = 0;
    }
    if (need)
        cScriptPlayerMain_DoUpdateFrame(self);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_SetUserData__FPvi);
#ifdef SKIP_ASM
/**
 * Replaces the script player's mpUserData with @p userData and returns the previous value.
 *
 * The BX script engine stores its play request there (cBXScriptEngine_RequestAnimationScript, cBXScriptEngine_UpdatePendingRequestState, cBXScriptEngine_OnInSeqRemoteScriptPlayerReady) and clears it
 * (cBXScriptEngine_ReleaseRequest).
 *
 * @param userData  the new user data
 * @return the old mpUserData
 */
int cScriptPlayerMain::SetUserData(int userData)
{
    int old = (int)mpUserData;
    mpUserData = (void*)userData;
    return old;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetEntity);
#ifdef SKIP_ASM
/**
 * Returns one of the script's entity players: mpEntityPlayers + index * 0x3C.
 *
 * @param entityIndex  entity index (On Tour entityIndex)
 * @return the entity player at that index (not range-checked)
 */
void* cScriptPlayerMain::GetEntity(int entityIndex)
{
    return (char*)&mpEntityPlayers[entityIndex];
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetHeader);
#ifdef SKIP_ASM
/**
 * Returns the data header of InSeq script data (+0xC, nextSectionStart) when its headerSize (+0x2) is at least 13, else
 * 0.
 *
 * GetSSXScriptHeader1 uses it.
 *
 * @param scriptHeader  the script data (its tInSeqScriptHeader)
 * @return the data header, or 0
 */
extern "C" void* cScriptPlayerMain_GetHeader(tInSeqScriptHeader* scriptHeader)
{
    if (scriptHeader->headerSize >= 13) {
        return &scriptHeader->nextSectionStart;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetProjectID);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cScriptPlayerMain_GetProjectID_impl(tInSeqScriptHeader* scriptHeader)
    __asm__("cScriptPlayerMain_GetProjectID");
/**
 * Returns the projectID (+0x4) of InSeq script data (tInSeqScriptHeader).
 *
 * @param scriptHeader  the script data
 * @return its projectID
 */
extern "C" int cScriptPlayerMain_GetProjectID_impl(tInSeqScriptHeader* scriptHeader) {
    return scriptHeader->projectID;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetDataSize);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cScriptPlayerMain_GetDataSize_impl(tInSeqScriptHeader* scriptHeader)
    __asm__("cScriptPlayerMain_GetDataSize");
/**
 * Returns the size (+0x8) of InSeq script data (tInSeqScriptHeader).
 *
 * @param scriptHeader  the script data
 * @return its size in bytes
 */
extern "C" int cScriptPlayerMain_GetDataSize_impl(tInSeqScriptHeader* scriptHeader) {
    return scriptHeader->size;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetNumEntities);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned char cScriptPlayerMain_GetNumEntities_impl(tInSeqScriptHeader* scriptHeader)
    __asm__("cScriptPlayerMain_GetNumEntities");
/**
 * Returns the entity count (numEntities, byte +0x1) of InSeq script data (tInSeqScriptHeader).
 *
 * @param scriptHeader  the script data
 * @return its numEntities
 */
extern "C" unsigned char cScriptPlayerMain_GetNumEntities_impl(tInSeqScriptHeader* scriptHeader) {
    return scriptHeader->numEntities;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetScriptLength);
#ifdef SKIP_ASM

/**
 * Returns the script's length in frames: the longest numFrames among its entities' current variations.
 *
 * Uses mScriptLength when it is not negative; otherwise computes the maximum with cScriptPlayerEntity_GetScriptLength over the
 * entities and caches it only while the script is playing.
 *
 * @return the length in frames
 */
int cScriptPlayerMain::GetScriptLength()
{
    int scriptLength = mScriptLength;
    if (scriptLength < 0) {
        int best = 0;
        int i;
        for (i = 0; i < mpSystemHeader->numEntities; i++) {
            int entityLength = mpEntityPlayers[i].GetScriptLength();
            if (best < entityLength) {
                best = entityLength;
            }
        }
        if (mPlayState != 0) {
            mScriptLength = best;
        }
        scriptLength = best;
    }
    return scriptLength;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetNumUpdatesToEndLeft);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self);

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ftrunc_00274D08(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

/**
 * Returns how many more updates the script needs to reach its end at the current play speed.
 *
 * Computes (cScriptPlayerMain_GetScriptLength() - mCurrFrame) / mPlaySpeed and rounds it up.
 *
 * @return the number of updates left, rounded up
 */
int cScriptPlayerMain::GetNumUpdatesToEndLeft()
{
    float x = ((float)GetScriptLength() - mCurrFrame) / mPlaySpeed;
    float ceilVal = ftrunc_00274D08(x);
    if (ceilVal < x) {
        ceilVal += 1.0f;
    }
    return (int)ceilVal;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetInSeqCommInterface);
#ifdef SKIP_ASM
/**
 * Returns the comm interface the script reports to: the preview interface when set, else the manager's.
 *
 * The manager's interface is the first word of the cInSeq (mpManager + 0x0).
 *
 * @return the cInSeqComm interface, as an int
 */
int cScriptPlayerMain::GetInSeqCommInterface()
{
    int previewComm = (int)mpPreviewCommInterface;
    if (previewComm != 0) {
        return previewComm;
    }
    return *(int*)mpManager;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GetEntityScriptData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// LANDING: unit declares `extern "C" int cScriptPlayerMain_GetEntityScriptData();` at line ~1887 (cInSeq_GetScriptDataNumEntityVariations caller); alias used here.
extern "C" char* cScriptPlayerMain_GetEntityScriptData_impl(char* self, int count) __asm__("cScriptPlayerMain_GetEntityScriptData");

/**
 * Returns the script data block of entity @p count of InSeq script data @p scriptData.
 *
 * Starts after the script header (@p scriptData + headerSize) and steps over @p count entity blocks by their blockSize
 * (cScriptPlayerEntity_GetDataSize1). The script engine looks entity headers up in the block (cScriptPlayerEntity_GetHeader1).
 *
 * @param scriptData  the script data (it starts with its tInSeqScriptHeader)
 * @param count  the entity index
 * @return the entity's script data block
 */
extern "C" char* cScriptPlayerMain_GetEntityScriptData_impl(char* scriptData, int count)
{
    tInSeqScriptHeader* header = (tInSeqScriptHeader*)scriptData;
    char* block = scriptData + header->headerSize;
    while (count != 0) {
        count--;
        block += cScriptPlayerEntity_GetDataSize1((int*)block);
    }
    return block;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_DoUpdateFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Advances playback by one step: plays tracks up to the new time, handles looping and stop/finish events.
// Like cScriptPlayerMain_DoUpdate, it collects the tracks' "changed" results; this build never reads them.
extern "C" void cScriptPlayerEntity_SetupPlay_4DE0(void* p) __asm__("cScriptPlayerEntity_SetupPlay");
extern "C" int cScriptPlayerEntity_ExecutePlay_4DE0(void* p) __asm__("cScriptPlayerEntity_ExecutePlay");
extern "C" int cScriptPlayerEntity_Restart_4DE0(void* p) __asm__("cScriptPlayerEntity_Restart");
extern "C" int cScriptPlayerMain_DoUpdate_4DE0(void* self, float a, float b, float c) __asm__("cScriptPlayerMain_DoUpdate");
extern "C" int cScriptPlayerMain_GetScriptLength_4DE0(void* self) __asm__("cScriptPlayerMain_GetScriptLength");
extern "C" void cScriptPlayerMain_Stop_4DE0(void* p) __asm__("cScriptPlayerMain_Stop");
extern "C" void* cScriptPlayerMain_GetInSeqCommInterface_p(void) __asm__("cScriptPlayerMain_GetInSeqCommInterface");

class cRcListener4DE0 {
public:
    virtual void OnInSeqPlay(void* src);
    virtual void OnInSeqUpdate(void* src, float a, float b);
    virtual void OnInSeqPlayStopping(void* src);
    virtual void v04(void* src);
    virtual int OnInSeqLoop(void* src);
};


extern "C" void cScriptPlayerMain_DoUpdateFrame_impl(char* self) __asm__("cScriptPlayerMain_DoUpdateFrame");
/**
 * Runs one playback step of a script player: starts the entities on the first step, then advances them, handling the
 * end of the script.
 *
 * Skipped while stopped or paused (mNumPaused > 0). State 1 becomes 2 and the comm interface hears about it (slot 1).
 * In states 3/4 it advances by mPlaySpeed through cScriptPlayerMain_DoUpdate, clamped to the script length; otherwise
 * it sets up and executes every entity (cScriptPlayerEntity_SetupPlay, _ExecutePlay), enters state 3 and notifies the
 * interface (slot 2). A pending stop request (mDoStopInEntityUpdate, +0x40) or state 4 stops the player. At the end of
 * the script a looping player restarts its entities (cScriptPlayerEntity_Restart) and replays the overshoot; otherwise
 * the comm interface (slot 5) is consulted before stopping.
 */
extern "C" void cScriptPlayerMain_DoUpdateFrame_impl(char* vself)
{
    cScriptPlayerMain* self = (cScriptPlayerMain*)vself;
    if (self->mPlayState == 0 || self->mNumPaused > 0) {
        return;
    }
    cRcListener4DE0* comm = (cRcListener4DE0*)cScriptPlayerMain_GetInSeqCommInterface_p();
    if (self->mPlayState == keScriptPlayState_PlayPending) {
        self->mPlayState = keScriptPlayState_PlayExecuting;
        if (comm != 0) {
            comm->OnInSeqPlay(self);
        }
    }
    if (self->mNumPaused > 0 || self->mPlayState == 0) {
        return;
    }
    int changed = 0;
    float scriptLength = (float)cScriptPlayerMain_GetScriptLength_4DE0(self);
    int hit = 0;
    float rem = 0.0f;
    if (self->mPlayState == keScriptPlayState_PlayEnding && scriptLength <= self->mCurrFrame) {
        cScriptPlayerMain_Stop_4DE0(self);
        return;
    }
    if ((unsigned)(self->mPlayState - 3) < 2) {
        float step = self->mPlaySpeed;
        float currFrame = self->mCurrFrame;
        float fullStep = step;
        float lim = currFrame + step;
        if (self->mPlayState == keScriptPlayState_PlayEnding) {
            lim = scriptLength;
        }
        if (scriptLength <= lim) {
            step = lim - currFrame;
            hit = 1;
            lim = scriptLength;
            rem = fullStep - step;
        }
        self->mEntityUpdateInProgress = 1;
        changed = cScriptPlayerMain_DoUpdate_4DE0(self, lim, currFrame, step);
        self->mEntityUpdateInProgress = 0;
    } else {
        if (scriptLength == 0.0f) {
            hit = 1;
        }
        self->mEntityUpdateInProgress = 1;
        for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
            cScriptPlayerEntity_SetupPlay_4DE0(&self->mpEntityPlayers[i]);
        }
        for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
            int anyChanged = 0;
            if (cScriptPlayerEntity_ExecutePlay_4DE0(&self->mpEntityPlayers[i]) != 0 || changed != 0) {
                anyChanged = 1;
            }
            changed = anyChanged;
        }
        self->mEntityUpdateInProgress = 0;
        self->mPlayState = 3;
        if (comm != 0) {
            comm->OnInSeqUpdate(self, 0.0f, 0.0f);
        }
    }
    if (self->mDoStopInEntityUpdate != 0 || self->mPlayState == keScriptPlayState_PlayEnding) {
        cScriptPlayerMain_Stop_4DE0(self);
        return;
    }
    if (hit != 0 && self->mLoopScript != 0) {
        int changed2 = 0;
        for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
            int anyChanged = 0;
            if (cScriptPlayerEntity_Restart_4DE0(&self->mpEntityPlayers[i]) != 0 || changed2 != 0) {
                anyChanged = 1;
            }
            changed2 = anyChanged;
        }
        *(int*)&self->mCurrFrame = 0;
        while (scriptLength < rem) {
            rem -= scriptLength;
        }
        if (rem != 0.0f) {
            self->mEntityUpdateInProgress = 1;
            changed2 = cScriptPlayerMain_DoUpdate_4DE0(self, self->mCurrFrame + rem, self->mCurrFrame, rem);
            self->mEntityUpdateInProgress = 0;
        }
        if (self->mDoStopInEntityUpdate != 0) {
            cScriptPlayerMain_Stop_4DE0(self);
        }
    } else {
        if (scriptLength <= self->mCurrFrame + self->mPlaySpeed) {
            int keepPlaying = self->mLoopScript;
            if (keepPlaying != 0) {
                if (comm != 0) {
                    keepPlaying = comm->OnInSeqLoop(self);
                }
                if (self->mPlayState == 0 || keepPlaying != 0) {
                    return;
                }
            }
            self->mPlayState = 4;
            if (comm != 0) {
                comm->OnInSeqPlayStopping(self);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_DoUpdate);
#ifdef SKIP_ASM
extern "C" int cScriptPlayerEntity_Update(cScriptPlayerEntity* track, float prev, float time, float speed, int jump);
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self);


/**
 * Updates every entity of a script player to a new frame.
 *
 * Stores @p time in mCurrFrame and treats the step as a jump when @p speed is above 1 or the step crosses an integer
 * frame, then calls cScriptPlayerEntity_Update on each entity (0x3C bytes apiece) and tells the comm interface
 * (cScriptPlayerMain_GetInSeqCommInterface, slot 2) the new time and speed.
 *
 * @param time   the new frame number
 * @param prev   the previous frame number
 * @param speed  the frame delta
 * @return nonzero when any entity reported a change
 */
int cScriptPlayerMain::DoUpdate(float time, float prev, float speed)
{
    int changed = 0;
    int jump = 0;
    mCurrFrame = time;
    if (speed > 1.0f) {
        jump = 1;
    } else {
        int ti = (int)time;
        int pi = (int)prev;
        if ((float)ti != time) {
            jump = pi != ti;
        }
    }
    for (int i = 0; i < mpSystemHeader->numEntities; i++) {
        int anyChanged = 0;
        if (mpEntityPlayers[i].Update(prev, mCurrFrame, speed, jump) != 0 || changed != 0) {
            anyChanged = 1;
        }
        changed = anyChanged;
    }
    cInSeqComm* comm = (cInSeqComm*)GetInSeqCommInterface();
    if (comm != 0) {
        comm->OnInSeqUpdate(this, mCurrFrame, speed);
    }
    return changed;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the spline channel's value storage at +0x1C.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelSpline::GetValue()
{
    return &((cScriptPlayerChannelSpline*)this)->mCurrKeyValue;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline channel's custom data word at +0x20.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelSpline::GetCustom()
{
    return (int)mpCurrKeyCustom;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetNextKeyFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reports that a spline channel has no next key frame.
 *
 * @return always -1
 */
int cScriptPlayerChannelSpline::GetNextKeyFrame()
{
    return -0x1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of a spline channel.
 *
 * @return always 0 (object channels return 1, block channels 2, event channels 3)
 */
int cScriptPlayerChannelSpline::GetType()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelSpline::GetOwner()
{
    return (int)mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the spline channel is playing, from its word at +0x8.
 *
 * @return true if the word at +0x8 is nonzero, otherwise false
 */
bool cScriptPlayerChannelSpline::IsPlaying()
{
    return ((int)mpData != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the block channel's value storage at +0x28.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelBlock::GetValue()
{
    return &((cScriptPlayerChannelBlock*)this)->mNumActiveBlocks;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the block channel's custom data word at +0x24.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelBlock::GetCustom()
{
    return (int)mpActiveBlockList;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of a block channel.
 *
 * @return always 2 (spline channels return 0, object channels 1, event channels 3)
 */
int cScriptPlayerChannelBlock::GetType()
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the block channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelBlock::GetOwner()
{
    return (int)mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the block channel is playing, from its word at +0x8.
 *
 * @return true if the word at +0x8 is nonzero, otherwise false
 */
bool cScriptPlayerChannelBlock::IsPlaying()
{
    return ((int)mpData != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the event channel's value storage at +0x20.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelEvent::GetValue()
{
    return &((cScriptPlayerChannelEvent*)this)->mNumActiveEvents;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the event channel's custom data word at +0x1C.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelEvent::GetCustom()
{
    return (int)mpEventArray;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of an event channel.
 *
 * @return always 3 (spline channels return 0, object channels 1, block channels 2)
 */
int cScriptPlayerChannelEvent::GetType()
{
    return 0x3;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the event channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelEvent::GetOwner()
{
    return (int)mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the event channel is playing, from its word at +0x8.
 *
 * @return true if the word at +0x8 is nonzero, otherwise false
 */
bool cScriptPlayerChannelEvent::IsPlaying()
{
    return ((int)mpData != 0);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetValue__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the object channel's value storage at +0x24.
 *
 * @return a pointer into the channel object (On Tour: void*)
 */
void* cScriptPlayerChannelObject::GetValue()
{
    return &((cScriptPlayerChannelObject*)this)->mNumActiveObjects;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetCustom__FPv);
#ifdef SKIP_ASM
/**
 * Returns the object channel's custom data word at +0x20.
 *
 * @return the custom data (a void* in On Tour, typed int here)
 */
int cScriptPlayerChannelObject::GetCustom()
{
    return (int)mpActiveObjectList;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the channel type id of an object channel.
 *
 * @return always 1 (spline channels return 0, block channels 2, event channels 3)
 */
int cScriptPlayerChannelObject::GetType()
{
    return 0x1;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_GetOwner__FPv);
#ifdef SKIP_ASM
/**
 * Returns the object channel's owner, the word at +0xC.
 *
 * @return the owning entity (On Tour: cScriptPlayerEntity*), typed int here
 */
int cScriptPlayerChannelObject::GetOwner()
{
    return (int)mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the object channel is playing, from its word at +0x8.
 *
 * @return true if the word at +0x8 is nonzero, otherwise false
 */
bool cScriptPlayerChannelObject::IsPlaying()
{
    return ((int)mpData != 0);
}
#endif

