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
extern "C" void* cReplay_addCache(cReplay* self)
{
    void** slot = (void**)&self->InputCache;
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
extern "C" void cReplay_removeCache(cReplay* self, void* pCache)
{
    if (pCache != 0) {
        void** slot = (void**)&self->InputCache;
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
extern "C" void* cReplay_allocReplayFrame(cReplay* self)
{
    void* list = (char*)&self->FreeFrameList;
    if (self->FreeFrameList.NumFrames <= 0) {
        cReplay_freeOneReplayFrame(self);
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
extern "C" void cReplay_freeOneReplayFrame(cReplay* self)
{
    cReplay* replay = self;
    int i;
    for (i = 0; i < 2; i++) {
        while (((sRcNode1A0*)replay->NextCandidate) != 0 && ((sRcNode1A0*)replay->NextCandidate)->next != 0 && ((sRcNode1A0*)replay->NextCandidate)->next->next != 0) {
            sRcNode1A0* n = ((sRcNode1A0*)replay->NextCandidate)->next;
            if (n->used == 0) {
                if (n->locked == 0) {
                    *(sRcNode1A0**)&replay->NextCandidate = n->next;
                    cReplay_freeReplayFrame(self, n);
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
extern "C" void cReplay_freeReplayFrame(cReplay* self, void* node)
{
    cReplayFrameList_remFrame((char*)&self->FrameList, node);
    cReplayFrame_clearFrame(node);
    cReplayFrameList_addFrame((char*)&self->FreeFrameList, node);
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
extern "C" int cReplay_isPauseAI(cReplay* self)
{
    int paused = 0;
    switch (self->State) {
    case 0: case 2: case 10: case 11: case 12: case 13: case 14: case 15:
        break;
    case 1:
    case 7:
        if (self->NextFrame == 0)
            paused = 1;
        if (self->PlaybackTick >= self->EndRecordTick)
            paused = 1;
        break;
    case 4:
        paused = self->PlaybackTick >= self->EndRecordTick;
        break;
    case 3: case 5: case 6: case 8: case 9:
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
extern "C" void cReplay_rewind(cReplay* self, void* frame)
{
    int i;
    self->NextFrame = 0;
    if (frame != 0) {
        self->PlaybackFrame = (cReplayFrame*)frame;
    } else {
        self->PlaybackFrame = (cReplayFrame*)*(void**)&self->FrameList;
    }
    self->PlaybackTick = self->PlaybackFrame->FrameStartTick;
    for (i = 0; i < 2; i++) {
        if (self->InputCache[i] != 0) {
            cReplayCache_moveTo(self->InputCache[i], self->PlaybackTick);
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
extern "C" void cReplay_lastFrame(cReplay* self)
{
    int i;
    self->NextFrame = 0;
    cReplayFrame* frame = (cReplayFrame*)cReplay_findFrame_2((self), 100000000);
    self->PlaybackFrame = frame;
    self->PlaybackTick = frame->FrameStartTick;
    for (i = 0; i < 2; i++) {
        if (self->InputCache[i] != 0) {
            cReplayCache_moveTo(self->InputCache[i], self->PlaybackTick);
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
extern "C" void cReplay_stepFrame(cReplay* self, int reset)
{
    int i;
    if (reset) {
        self->PlaybackFrame = (cReplayFrame*)cReplay_findFrame_2((self), self->PlaybackTick);
    }
    cReplayFrame* frame = self->PlaybackFrame;
    if (((void*)frame) != 0 && ((void*)frame) == (void*)self->SaveFrame) {
        self->PlaybackFrame = (cReplayFrame*)(frame->Prev);
    }
    if (reset) {
        self->PlaybackTick = self->PlaybackFrame->FrameStartTick;
        for (i = 0; i < 2; i++) {
            if (self->InputCache[i] != 0) {
                cReplayCache_moveTo(self->InputCache[i], self->PlaybackTick);
            }
        }
        cReplay_restoreReplayFrame((self), self->PlaybackFrame);
        return;
    }
    if (self->PlaybackTick >= self->EndRecordTick) {
        if (self->AutoReplay != 0) {
            cReplay_rewind(self, 0);
            cReplay_stepFrame(self, 1);
            cGameViewMan* views = SSXApp->Game->ViewMan;
            cChaseCameraController* controller = views->PlayerCamera[0]->ChaseController;
            cChaseCameraController_switchCamera_Chapter(controller);
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)controller->mVtbl;
            vt[4].fn(vthis(controller, vt[4]));
            cGameViewMan_updateAll(views);
        } else {
            self->PlaybackTick = self->EndRecordTick;
            cReplay_setReplayState((self), 3);
        }
    } else {
        self->PlaybackTick++;
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
extern "C" void cReplay_purgeIdleFrames(cReplay* self)
{
    cReplayFrame* node = *(cReplayFrame**)&self->FrameList;
    while (node != 0) {
        cReplayFrame* next = node->Next;
        if (node->FrameType.Type != 0) {
        } else if (node->FrameType.Chapter != 0) {
        } else if (node->FrameType.Highlights != 0) {
        } else {
            cReplay_freeReplayFrame(self, node);
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
void cReplay_beginMarkInstantReplay(void* self)
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
extern "C" void cReplay_endMarkInstantReplay(cReplay* self, int obj, int alt)
{
    tRiderStats* stats = (tRiderStats*)obj;
    if ((int)self->InstantReplayFrame == 0) {
        return;
    }
    if (SSXApp->Game->AI->NumberOfPlayersAllocated != 1) {
        return;
    }
    int now;
    int start;
    if (alt != 0) {
        now = stats->TotalPoints;
        start = self->InstantReplayStats.TotalPoints;
    } else {
        now = stats->TotalPointsLostToWipeouts;
        start = self->InstantReplayStats.TotalPointsLostToWipeouts;
    }
    now -= start;
    if (now >= 1000) {
        cReplay_startInstantReplay(self);
    } else {
        self->InstantReplayFrame = 0;
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
extern "C" void cReplay_startInstantReplay(cReplay* self)
{
    if (self->InstantReplay == 0) {
        self->StartInstantReplay = 1;
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
extern "C" void cReplay_stopInstantReplay(cReplay* self)
{
    if (self->InstantReplay != 0) {
        cReplay_stopReplay(self);
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
 * Switches to state 9 (kStartAutoReplay) unless an automatic replay is already on or the state is already 9.
 */
extern "C" void cReplay_startAutoReplay(cReplay* self)
{
    if (self->AutoReplay == 0 && *(int*)self != 9) {
        cReplay_setReplayState_cpp(self, 9);
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
void cReplay_stopAutoReplay(void* vself)
{
    cReplay* self = (cReplay*)vself;
    if (self->AutoReplay != 0) {
        cReplay_stopReplay(self);
    }
    cReplay_setReplayState(self, 13);
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
extern "C" void cReplay_eventJump(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cReplay_beginMarkHighlight(self, slot, id);
            cReplay_beginMarkInstantReplay_2(self, id);
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
extern "C" void cReplay_eventLand(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cReplay_endMarkHighlight(self, slot, id, 1);
            cReplay_endMarkInstantReplay(self, id, 1);
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
extern "C" void cReplay_eventWipeout(cReplay* self, int key, int id)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cReplay_endMarkHighlight(self, slot, id, 0);
            cReplay_endMarkInstantReplay(self, id, 0);
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
extern "C" void cReplay_eventReset(cReplay* self, int key)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cReplay_cancelMarkHighlight(self, slot);
            cReplay_cancelMarkInstantReplay(self);
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
extern "C" void cReplay_eventWScript(cReplay* self, int key, int mode)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0) {
            cRider* rider = SSXApp->Game->AI->Riders[key];
            switch (mode) {
            case 0:
                cReplay_beginMarkInstantReplay_2(self, (int)rider->mMetrix + 0xFC);
                break;
            case 1:
                cReplay_beginMarkHighlight(self, slot, (int)rider->mMetrix + 0xFC);
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
extern "C" void cReplay_eventChapter(cReplay* self, int key, int bit)
{
    if (self->AutoReplay == 0 && self->InstantReplay == 0 &&
        self->ReplayError == 0 && self->EndReplay == 0 &&
        self->State == 0) {
        int slot = findPlayerId(key);
        if (slot >= 0 && bit >= 0 && bit < self->MaxChapters) {
            int off = slot << 2;
            char* base = (char*)&self->Chapters;
            int* chapterBits = (int*)(base + off);
            if (cReplay_eventChapter_isClear(*chapterBits, bit)) {
                unsigned char* headFrame = (unsigned char*)self->FrameList.Head;
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
 * @param replay  the replay
 * @param playerId  the player (0 or 1)
 * @param id  the rider stats at the start (a tRiderStats pointer passed as int)
 */
extern "C" void cReplay_beginMarkHighlight(cReplay* replay, int playerId, int id)
{
    cReplay* self = replay;
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
extern "C" int cReplay_countHighlights(cReplay* self, int i, int positive)
{
    int count = 0;
    sReplayCacheNode* n = *(sReplayCacheNode**)&self->FrameList;
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
extern "C" int cReplay_maxHighlights(cReplay* self, int land)
{
    if (land != 0) {
        return self->MaxPlayerHighlights;
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
extern "C" int cReplayHighlight_setRanking_s(void* self, int id, int mode) __asm__("cReplayHighlight_setRanking");
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
 * @param replay  the replay
 * @param playerId  the player (0 or 1)
 * @param id  the rider stats at the end (a tRiderStats pointer passed as int)
 * @param positive  non-zero for a landing (cReplay_eventLand), 0 for a wipeout (cReplay_eventWipeout)
 */
extern "C" void cReplay_endMarkHighlight(cReplay* replay, int playerId, int id, int positive)
{
    cReplay* self = replay;
    if (highlightIsMarking(&self->Highlight[playerId])) {
        if (cReplayHighlight_setRanking_s(&self->Highlight[playerId], id, positive)) {
            if (!(cReplay_countHighlights((cReplay*)self, playerId, positive) < cReplay_maxHighlights((cReplay*)self, positive))) {
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
extern "C" void cReplay_cancelMarkHighlight(cReplay* self, int i)
{
    if (self->State == 0) {
        cReplayHighlight_cancel(&((&self->Highlight[0]))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_cancelMarkHighlights);
#ifdef SKIP_ASM
/**
 * Cancels both players' open highlight marks (cReplayHighlight_cancel on Highlight[0] and [1]) while recording (State 0).
 */
extern "C" void cReplay_cancelMarkHighlights(cReplay* self)
{
    if (self->State == 0) {
        for (int i = 0; i < 2; i++) {
            cReplayHighlight_cancel(&((&self->Highlight[0]))[i]);
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
extern "C" void cReplay_initSave(cReplay* self)
{
    cReplay_allocSaveSpace(self);
    self->WriteState = 0;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteSize = 0;
    self->WriteData = 0;
    self->WriteIndex = 0;
    self->WriteIndexMax = 0;
    self->WriteFrame = 0;
    self->WriteBlockData = 0;
    self->WriteBlockDataSize = 0;
    self->WriteCmdFrame = 0;
    self->WriteCacheSave = 0;
    self->WriteSaveSize = 0;
    self->WriteRemaining = 0;
    self->WriteCheckSum.Size = 0;
    self->WriteCheckSum.CheckSum = (int)0xDEADBEEF;
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
void* cReplay_purgeSave(void* self)
{
    return cReplay_purgeSaveSpace((cReplay*)self);
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
void* cReplay_initLoad(void* self)
{
    return cReplay_reset(self);
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
void cReplay_purgeLoad(void* self)
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
extern "C" void cReplay_allocSaveSpace(cReplay* self)
{
    self->WriteBlock = (tReplayMemCardBlock*)cMemMan_alloc(0x4000, str_WriteBlock, 0x20000000, 0);
    self->WriteTmp = (char*)operator_new_tag(51200, str_WriteTmp, 0x20000000, 0);
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
extern "C" void* cReplay_purgeSaveSpace(cReplay* self)
{
    operator_delete((int*)self->WriteBlock);
    void* tmpBuffer = self->WriteTmp;
    if (tmpBuffer != 0) {
        cMemMan_free(tmpBuffer);
    }
    self->WriteBlock = 0;
    self->WriteTmp = 0;
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
extern "C" void cReplay_writeBuffered(cReplay* self)
{
    if (self->WriteSrc != 0) {
        if (self->WriteSrcSize > 0) {
            int space = 0x4000 - self->WriteSize;
            int n = space < self->WriteSrcSize ? space : self->WriteSrcSize;
            if (n > 0) {
                MEM_copy(self->WriteData, self->WriteSrc, n);
                self->WriteData += n;
                self->WriteSrc += n;
                self->WriteSrcSize -= n;
                self->WriteSize += n;
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
extern "C" void cReplay_writeSetup(cReplay* self, signed char* data, int size)
{
    self->WriteCheckSum.Size += size;
    for (int i = 0; i < size; i++) {
        self->WriteCheckSum.CheckSum += data[i];
    }
    self->WriteSrc = (char*)data;
    self->WriteSrcSize = size;
    self->WriteSaveSize += size;
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
 * Unless the writer is done (WriteState 11), fills WriteBlock from its start: copies pending data (writeBuffered) and
 * runs the writer step for WriteState (0 header start, 1 header, 2 frames start, 3 frames, 4 input start, 5 input, 6
 * pad start, 7 pad, 8 checksum start, 9 checksum, 10 finish) until the 0x4000-byte block is full or the writer is
 * done.
 *
 * @param block  receives WriteBlock
 * @param size  receives the bytes in the block (0 when done)
 * @return 1 when a block was produced, 0 when the save is complete
 */
extern "C" int cReplay_getSaveData(cReplay* self, char** block, int* size)
{
    if (self->WriteState == 11) {
        *size = 0;
        return 0;
    }
    self->WriteSize = 0;
    self->WriteData = (char*)self->WriteBlock;
    while (self->WriteSize < 0x4000 && self->WriteState != 11) {
        cReplay_writeBuffered(self);
        if (self->WriteSize >= 0x4000) {
            goto done;
        }
        switch (self->WriteState) {
        case 0: cReplay_writeHeaderStart(self); break;
        case 1: cReplay_writeHeader(self); break;
        case 2: cReplay_writeFramesStart(self); break;
        case 3: cReplay_writeFrames(self); break;
        case 4: cReplay_writeInputStart(self); break;
        case 5: cReplay_writeInput(self); break;
        case 6: cReplay_writePadStart(self); break;
        case 7: cReplay_writePad(self); break;
        case 8: cReplay_writeCheckSumStart((self)); break;
        case 9: cReplay_writeCheckSum(self); break;
        case 10: self->WriteState++; break;
        case 11: break;
        }
    }
done:
    *size = self->WriteSize;
    *block = (char*)self->WriteBlock;
    return 1;
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
extern "C" void cReplay_writeHeaderStart(cReplay* self)
{
    tReplayManSaveHeader* hdr = (tReplayManSaveHeader*)self->WriteTmp;
    hdr->NumSaveFrames = cReplay_writeFrameCount(self);
    cSSXApp* app = SSXApp;
    hdr->SaveFrameTicks = self->SaveFrameTicks;
    hdr->ReplayError = self->ReplayError;
    hdr->RecordTick = self->RecordTick;
    hdr->EndRecordTick = self->EndRecordTick;
    int mode = app->Game->AI->NumberOfPlayersAllocated;
    hdr->Version = 0xABCD0002;
    hdr->Players = mode;
    cAI_getPodiumActorList(app->Game->AI, hdr->PodiumActorList);
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteState = self->WriteState + 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writeHeader);
#ifdef SKIP_ASM
/**
 * Queues the 0x28-byte header from WriteTmp (writeSetup) and advances WriteState.
 */
extern "C" void cReplay_writeHeader(cReplay* self)
{
    cReplay_writeSetup(self, (signed char*)self->WriteTmp, 40);
    self->WriteState += 1;
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
extern "C" int cReplay_writeFrameCount(cReplay* self)
{
    int count = 0;
    *(sCacheNode**)&self->WriteFrame = *(sCacheNode**)&self->FrameList;
    while (((sCacheNode*)self->WriteFrame)) {
        if (((sCacheNode*)self->WriteFrame) == ((sCacheNode*)self->SaveFrame)) {
            *(sCacheNode**)&self->WriteFrame = ((sCacheNode*)self->WriteFrame)->next;
        } else {
            count++;
        }
        if (((sCacheNode*)self->WriteFrame)) {
            *(sCacheNode**)&self->WriteFrame = ((sCacheNode*)self->WriteFrame)->next;
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
extern "C" void cReplay_writeFramesStart(cReplay* self)
{
    cReplayFrame* oldest = (cReplayFrame*)(*(void**)&self->FrameList);
    self->WriteFrame = (cReplayFrame*)oldest;
    if (((void*)oldest) == (void*)self->SaveFrame) {
        self->WriteFrame = (cReplayFrame*)(oldest->Next);
    }
    self->WriteBlockData = 0;
    self->WriteState += 1;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteFrameCount = 0;
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
extern "C" void cReplay_writeFrames(cReplay* self)
{
    if (((void*)self->WriteBlockData) != 0) {
        int state = 0;
        int* sized = (int*)self->WriteTmp;  // a size word, then the encoded bytes
        int n = HUFF_encode(((void*)self->WriteBlockData), self->WriteBlockDataSize, sized + 1, &state);
        *((int*)self->WriteTmp) = n;
        cReplay_writeSetup((cReplay*)self, (signed char*)((int*)self->WriteTmp), n + 4);
        *(void**)&self->WriteBlockData = 0;
        return;
    }
    if (self->WriteFrame != 0) {
        self->WriteFrameCount++;
        cReplayFramePtr_getFrameBlock_2(self->WriteFrame, 1);
        int* hdr = ((int*)self->WriteTmp);
        hdr[0] = 0x11111113;
        hdr[2] = self->WriteFrame->FrameStartTick;
        hdr[3] = self->WriteFrame->FrameEndTick;
        hdr[1] = self->WriteFrame->Size;
        hdr[9] = 0x11111111;
        hdr[5] = self->WriteFrame->Ranking[0];
        hdr[6] = self->WriteFrame->Ranking[1];
        hdr[7] = self->WriteFrame->HighlightEndTick[0];
        hdr[8] = self->WriteFrame->HighlightEndTick[1];
        hdr[7] = self->WriteFrame->HighlightEndTick[0];
        hdr[8] = self->WriteFrame->HighlightEndTick[1];
        cReplay_writeSetup((cReplay*)self, (signed char*)hdr, 40);
        cReplayFrame* currentFrame = self->WriteFrame;
        *(void**)&self->WriteBlockData = ((void*)currentFrame->Block);
        self->WriteBlockDataSize = currentFrame->Size;
        cReplayFrame* next = currentFrame->Next;
        *(cReplayFrame**)&self->WriteFrame = next;
        if (next != 0 && next == self->SaveFrame) {
            *(cReplayFrame**)&self->WriteFrame = next->Next;
        }
        return;
    }
    *(int*)&self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteState++;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplay_writeInputStart);
#ifdef SKIP_ASM
/**
 * Starts writing the input caches: WriteIndexMax = 2 (the InputCache count), WriteIndex 0, no pending cache header or
 * command data, and advances WriteState.
 */
extern "C" void cReplay_writeInputStart(cReplay* self)
{
    self->WriteIndexMax = 2;
    self->WriteState += 1;
    self->WriteCacheSave = 0;
    self->WriteCmdFrame = 0;
    self->WriteCmdFrameSize = 0;
    self->WriteSrc = 0;
    self->WriteSrcSize = 0;
    self->WriteIndex = 0;
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
extern "C" void cReplay_writeInput(cReplay* self)
{
    if (((int*)self->WriteCacheSave) != 0) {
        cReplay_writeSetup((cReplay*)self, (signed char*)((int*)self->WriteCacheSave), 8);
        (*(int**)&self->WriteCacheSave) = 0;
        return;
    }
    if (((void*)self->WriteCmdFrame) != 0) {
        int state = 0;
        int* sized = (int*)self->WriteTmp;  // a size word, then the encoded bytes
        int n = HUFF_encode(((void*)self->WriteCmdFrame), self->WriteCmdFrameSize, sized + 1, &state);
        *((int*)self->WriteTmp) = n;
        cReplay_writeSetup((cReplay*)self, (signed char*)((int*)self->WriteTmp), n + 4);
        (*(void**)&self->WriteCmdFrame) = 0;
        return;
    }
    int cur = self->WriteIndex;
    if (cur < self->WriteIndexMax) {
        (*(void**)&self->WriteCmdFrame) = 0;
        (*(int**)&self->WriteCacheSave) = 0;
        self->WriteCmdFrameSize = 0;
        cReplayCache* inputCache = self->InputCache[cur];
        if (inputCache != 0) {
            (*(void**)&self->WriteCmdFrame) = ((void*)inputCache->CmdFrame);
            (*(int**)&self->WriteCacheSave) = ((int*)self->WriteTmp);
            self->WriteCmdFrameSize = 0x8000;
            *((int*)self->WriteTmp) = 0x11111112;
            self->WriteCacheSave->RecordCursor = inputCache->RecordCursor;
            self->WriteIndex++;
        } else {
            self->WriteIndex = cur + 1;
            self->WriteSrcSize = 0;
            (*(int*)&self->WriteSrc) = 0;
        }
    } else {
        self->WriteState++;
        self->WriteSrcSize = 0;
        (*(int*)&self->WriteSrc) = 0;
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
extern "C" void cReplay_writePadStart(cReplay* self)
{
    MEM_fill(self->WriteTmp, 0, 51200);
    int used = cBE_getDataSize(cBE_getBE()) + self->WriteSaveSize;
    self->WriteRemaining = 0x7FFF8 - used;
    self->WriteState += 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cReplay_writePad);
#ifdef SKIP_ASM
/**
 * Queues up to 0x4000 zero bytes of padding from WriteTmp per call until WriteRemaining is used up, then advances
 * WriteState.
 */
extern "C" void cReplay_writePad(cReplay* self)
{
    int left = self->WriteRemaining;
    if (left > 0) {
        int n = (left < 16385) ? left : 0x4000;
        cReplay_writeSetup(self, (signed char*)self->WriteTmp, n);
        self->WriteRemaining -= n;
    } else {
        self->WriteState += 1;
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
void* cReplay_writeCheckSumStart(void* vself)
{
    cReplay* self = (cReplay*)vself;
    int zero = 0;
    int nextState = self->WriteState + 0x1;
    self->WriteSrc = (char*)zero;
    self->WriteSrcSize = zero;
    self->WriteState = nextState;
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
extern "C" void cReplay_writeCheckSum(cReplay* self)
{
    MEM_copy(self->WriteTmp, (char*)&self->WriteCheckSum, 8);
    cReplay_writeSetup(self, (signed char*)self->WriteTmp, 8);
    self->WriteState += 1;
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
 * @return 1 when loaded, 0 when the rider count differs (On Tour returns bool)
 */
extern "C" int cReplay_load(cReplay* self, char* buf)
{
    tReplayManSaveHeader* data = (tReplayManSaveHeader*)buf;
    char** pp = &buf;
    if (data->Players != SSXApp->Game->AI->NumberOfPlayersAllocated) {
        return 0;
    }
    {
        self->Loaded = 1;
        self->SaveFrameTicks = data->SaveFrameTicks;
        self->ReplayError = data->ReplayError != 0;
        self->RecordTick = data->RecordTick;
        self->EndRecordTick = data->EndRecordTick;
        self->SaveFrame = 0;
        self->PlaybackTick = 0;
        cReplayFrameList_reset(((char*)&self->FrameList));
        *pp = (char*)&data[1];
        for (int i = 0; i < data->NumSaveFrames; i++) {
            void* node = cReplay_allocReplayFrame(self);
            cReplayFrame_loadFrame(node, pp);
            cReplayFrameList_addFrame(((char*)&self->FrameList), node);
        }
        for (int i = 0; i < data->Players; i++) {
            if (self->InputCache[i] != 0) {
                cReplayCache_load(self->InputCache[i], pp);
            }
        }
        self->State = 8;
        self->Cmd = 1;
    }
    return 1;
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
 * @param priority  the initialisation priority; only 0xFFFF does anything
 */
extern "C" void func_002718E8_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
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
extern "C" void cReplayFramePtr__cReplayFramePtr(int* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_clearBlock);
#ifdef SKIP_ASM
/**
 * Empties the frame without freeing its block: clears WriteData, ReadData and Size.
 */
extern "C" void cReplayFramePtr_clearBlock(cReplayFramePtr* self)
{
    self->WriteData = 0;
    self->ReadData = 0;
    self->Size = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cReplayFramePtr_getBlock);
#ifdef SKIP_ASM
/**
 * Returns the frame pointer's data block (Block).
 *
 * @param replayFramePtr  the frame pointer
 * @return Block (as an int)
 */
extern "C" int cReplayFramePtr_getBlock(cReplayFramePtr* replayFramePtr) {
    return ((int)replayFramePtr->Block);
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
 * @param replayFramePtr  the frame pointer
 * @param outPos  receives the read position
 */
extern "C" void cReplayFramePtr_getReadPos(cReplayFramePtr* replayFramePtr, int *outPos) {
    *outPos = ((int)replayFramePtr->ReadData);
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
 * @param framePtr  the frame pointer
 * @param readPos  the saved read position
 */
extern "C" void cReplayFramePtr_setReadPos(cReplayFramePtr* framePtr, int *readPos) {
    framePtr->ReadData = (char*)((int) *readPos);
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
 * @param replayFramePtr  the frame pointer
 * @return Size
 */
extern "C" int cReplayFramePtr_getBlockSaveSize(cReplayFramePtr* replayFramePtr) {
    return (replayFramePtr->Size);
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
 * @param replayFramePtr  the frame pointer
 * @return Size
 */
extern "C" int cReplayFramePtr_GetSize(cReplayFramePtr* replayFramePtr) {
    return (replayFramePtr->Size);
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
extern "C" void cReplayFrame__cReplayFrame(int* self, int flags)
{
    *(void**)self = cReplayFramePtr_vtbl;
    if (flags & 1) {
        operator_delete(self);
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
extern "C" void cReplayHighlight_reset(int* self)
{
    self[0] = -1;
    self[176 / 4] = 0;
    MEM_fill(self + 1, 0, 172);
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
void* cScriptHeap_cScriptHeap(void* self, void* pHeap, void* heapSize)
{
    cScriptHeap* heap = (cScriptHeap*)self;
    tFreeBlock* first = (tFreeBlock*)pHeap;
    heap->mpTopFreeBlock = first;
    heap->mpHeap = (char*)pHeap;
    heap->mHeapSize = (int)heapSize;
    first->pNext = 0;
    heap->mpTopFreeBlock->size = (unsigned int)heapSize - 0x8;
    return self;
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
 * @param scriptHeap  the script heap (this)
 * @param pMem           memory returned by cScriptHeap_AllocMem
 */
extern "C" void cScriptHeap_FreeMem(cScriptHeap* scriptHeap, void* pMem)
{
    if (pMem) {
        // ORIGINAL-PTR: an allocator over a byte heap: headers sit 8 bytes before the memory, blocks are size + 8 long
        sRcBlock* block = (sRcBlock*)((char*)pMem - 8);
        if (!(sRcBlock*)scriptHeap->mpTopFreeBlock) {
            scriptHeap->mpTopFreeBlock = (tFreeBlock*)block;
            return;
        }
        sRcBlock* prev = 0;
        sRcBlock* cur = (sRcBlock*)scriptHeap->mpTopFreeBlock;
        do {
            if (block < cur) {
                int size = block->size + 8;
                if ((sRcBlock*)((char*)block + size) == cur) {
                    block->next = cur->next;
                    block->size = size + cur->size;
                    if (!prev)
                        scriptHeap->mpTopFreeBlock = (tFreeBlock*)block;
                    else
                        prev->next = block;
                    return;
                }
                if (!prev) {
                    block->next = (sRcBlock*)scriptHeap->mpTopFreeBlock;
                    scriptHeap->mpTopFreeBlock = (tFreeBlock*)block;
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
                    scriptHeap->mpTopFreeBlock = (tFreeBlock*)cur;
                else
                    prev->next = cur;
            } else {
                prev = cur;
                cur = cur->next;
            }
        } while (cur);
        if (!prev) {
            block->next = (sRcBlock*)scriptHeap->mpTopFreeBlock;
            scriptHeap->mpTopFreeBlock = (tFreeBlock*)block;
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
    cScriptHeap_FreeMem((cScriptHeap*)(*(cScriptHeap**)allocBase), allocBase);
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
 * @return 1 when there is a next key to reach, otherwise 0
 */
extern "C" int cScriptPlayerChannelSpline_Play(cScriptPlayerChannelSpline* self, unsigned int* data, int key)
{
    tInSeqSplineChannelHeader* header = (tInSeqSplineChannelHeader*)data;
    char* body = &header->keyStart;
    tInSeqSplineChannelKey* first = (tInSeqSplineChannelKey*)body;
    *(unsigned int**)&self->mpData = data;
    *(int*)&self->mpOwner = key;
    self->mpHeader = header;
    self->mpCurrKey = first;
    self->mCurrKeyIndex = 0;
    if (header->numKeys >= 2) {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        self->mpNextKey = (tInSeqSplineChannelKey*)(body + first->keySize);
    } else {
        self->mpNextKey = 0;
    }
    self->mCurrKeyValue = GetCubicSplineValue(&self->mpCurrKey->splineCoef[0], 0.0f);
    tInSeqSplineChannelKey* keyLoc = self->mpCurrKey;
    if (keyLoc->keySize >= 21) {
        self->mpCurrKeyCustom = &keyLoc->customDataStart;
    } else {
        self->mpCurrKeyCustom = 0;
    }
    return self->mpNextKey != 0;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_Stop__FPv);
#ifdef SKIP_ASM
/**
 * Stops a spline channel: clears its data, owner, header, keys, key index, value and custom data (+0x8 .. +0x24).
 */
void cScriptPlayerChannelSpline_Stop(void* vself)
{
    cScriptPlayerChannelSpline* self = (cScriptPlayerChannelSpline*)vself;
    self->mpData = 0;
    self->mpOwner = 0;
    self->mpHeader = 0;
    self->mpCurrKey = 0;
    self->mCurrKeyIndex = 0;
    *(int*)&self->mCurrKeyValue = 0;
    self->mpCurrKeyCustom = 0;
    self->mpNextKey = 0;
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
 * @return 1 while the channel runs, 0 once it reached its last key (or has none to reach)
 */
extern "C" int cScriptPlayerChannelSpline_Update(cScriptPlayerChannelSpline* self, float currFrameNum)
{
    tInSeqSplineChannelKey* n = self->mpNextKey;
    if (n == 0) return 0;
    if ((float)n->frameNum <= currFrameNum) {
        self->mpCurrKey = n;
        self->mpCurrKeyCustom = 0;
        (*(int*)&self->mCurrKeyIndex)++;
        if (n->keySize >= 21) self->mpCurrKeyCustom = &n->customDataStart;
        if (((int)self->mCurrKeyIndex) == self->mpHeader->numKeys - 1) {
            self->mpNextKey = 0;
            self->mCurrKeyValue = GetCubicSplineValue(&self->mpCurrKey->splineCoef[0], 0.0f);
            return 0;
        }
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        char* curr = (char*)self->mpCurrKey;
        self->mpNextKey = (tInSeqSplineChannelKey*)(curr + self->mpCurrKey->keySize);
    }
    tInSeqSplineChannelKey* keyLoc = self->mpCurrKey;
    self->mCurrKeyValue = GetCubicSplineValue(&keyLoc->splineCoef[0], currFrameNum - (float)keyLoc->frameNum);
    return 1;
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
extern "C" void cInSeq__cInSeq(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
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
extern "C" void cInSeq_UpdateFrame(cInSeq* self)
{
    tScriptMainNode* n = self->mpScriptUsed;
    while (((char*)n) != 0) {
        cScriptPlayerMain_UpdateFrame((cScriptPlayerMain*)(&n->script));
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
extern "C" int cInSeq_ReadyPlayer(cInSeq* self, void* src)
{
    tScriptMainNode* n = self->mpScriptUnused;
    *(tScriptMainNode**)&self->mpScriptUnused = n->pNext;
    n->pNext = 0;
    if (self->mpScriptUnused != 0) {
        self->mpScriptUnused->pPrev = 0;
    }
    cScriptPlayerMain_Init(((char*)&n->script), src, self, n->index);
    n->pNext = self->mpScriptUsed;
    if (self->mpScriptUsed != 0) {
        self->mpScriptUsed->pPrev = n;
    }
    *(tScriptMainNode**)&self->mpScriptUsed = n;
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
extern "C" void cInSeq_RecyclePlayer(cInSeq* self, int playerID)
{
    cInSeq_Stop((cInSeq*)self, playerID);
    tScriptMainNode* nodes = self->mpScriptHeap;
    tScriptMainNode* n = &nodes[playerID];
    cScriptPlayerMain_DeInit(&n->script);
    if (n->pPrev == 0) {
        *(tScriptMainNode**)&self->mpScriptUsed = n->pNext;
        n->pNext = 0;
        if (self->mpScriptUsed != 0) {
            self->mpScriptUsed->pPrev = 0;
        }
    } else {
        n->pPrev->pNext = n->pNext;
        if (n->pNext != 0) {
            n->pNext->pPrev = n->pPrev;
        }
        n->pNext = 0;
        n->pPrev = 0;
    }
    n->pNext = self->mpScriptUnused;
    if (self->mpScriptUnused != 0) {
        self->mpScriptUnused->pPrev = n;
    }
    *(tScriptMainNode**)&self->mpScriptUnused = n;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_RecycleAll);
#ifdef SKIP_ASM
/**
 * Gives every playing script's player back to the free pool (cInSeq_RecyclePlayer, RecyclePlayer, on each used node).
 */
extern "C" void cInSeq_RecycleAll(cInSeq* self)
{
    tScriptMainNode* n = self->mpScriptUsed;
    while (n != 0) {
        tScriptMainNode* next = n->pNext;
        cInSeq_RecyclePlayer((self), n->index);
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
extern "C" void cInSeq_Preview(cInSeq* self, int playerID, int flag)
{
    cScriptPlayerMain_Preview((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID), flag);
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
extern "C" void* cInSeq_Play(cInSeq* self, int playerID, int loopScript)
{
    return cScriptPlayerMain_Play1(cInSeq_GetPlayer(self, playerID), loopScript);
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
 * @param inSeq  the in-game sequencer
 * @param loopScript  non-zero to loop the scripts
 */
extern "C" void cInSeq_PlayAll(cInSeq* inSeq, int loopScript) {
    tScriptMainNode* node;

    node = (tScriptMainNode*)((void*)inSeq->mpScriptUsed);
    if (((void*)node) != 0) {
        do {
            cInSeq_Play(inSeq, node->index, loopScript);
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
extern "C" void cInSeq_Stop(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Stop((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cInSeq_StopAll);
#ifdef SKIP_ASM
/**
 * Stops every playing script player (cInSeq_Stop on each node of mpScriptUsed).
 */
extern "C" void cInSeq_StopAll(cInSeq* self)
{
    tScriptMainNode* n;
    for (n = self->mpScriptUsed; n != 0; n = n->pNext) {
        cInSeq_Stop(self, n->index);
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
extern "C" void cInSeq_Pause(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Pause((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_PauseAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Pause(cInSeq* self, int a1);

/**
 * Pauses every playing script player (cInSeq_Pause on each node of mpScriptUsed).
 */
extern "C" void cInSeq_PauseAll(cInSeq* self)
{
    tScriptMainNode* n;
    for (n = self->mpScriptUsed; n != 0; n = n->pNext) {
        cInSeq_Pause(self, n->index);
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
extern "C" void cInSeq_Resume(cInSeq* self, int playerID)
{
    cScriptPlayerMain_Resume((cScriptPlayerMain*)cInSeq_GetPlayer(self, playerID));
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cInSeq_ResumeAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_Resume(cInSeq* self, int a1);

/**
 * Resumes every playing script player (cInSeq_Resume on each node of mpScriptUsed).
 */
extern "C" void cInSeq_ResumeAll(cInSeq* self)
{
    tScriptMainNode* n;
    for (n = self->mpScriptUsed; n != 0; n = n->pNext) {
        cInSeq_Resume(self, n->index);
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
extern "C" void* cInSeq_GetPlayer(cInSeq* self, int playerID)
{
    tScriptMainNode* nodes = self->mpScriptHeap;
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
    cScriptPlayerChannelBlock_Stop(self);
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
 * @return 1 when the channel has keys, otherwise 0
 */
extern "C" int cScriptPlayerChannelBlock_Play(cScriptPlayerChannelBlock* self, unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    self->mpData = (const char*)data;
    self->mpOwner = (cScriptPlayerEntity*)owner;
    self->mpHeader = (tInSeqBlockChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        self->mpBlockArray = (tBlockEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 0x1C);
        self->mNextKeyIndex = 0;
        tInSeqBlockChannelHeader* base = self->mpHeader;
        self->mpNextKey = (tInSeqBlockChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqBlockChannelKey*)&base->keyStart)->frameStart == 0) do {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)self, (sCacheRecord*)self->mpNextKey);
            cScriptPlayerChannelBlock_LoadNextScriptKey((cScriptPlayerChannelBlock*)self);
        } while (self->mpNextKey != 0 &&
               self->mpNextKey->frameStart == 0);
    }
    return self->mpHeader->numKeys != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_Stop);
#ifdef SKIP_ASM
/**
 * Stops a block channel: frees its active-key buffer to the owner's script heap and clears its state (only when it
 * has an owner).
 */
extern "C" void cScriptPlayerChannelBlock_Stop(cScriptPlayerChannelBlock* self)
{
    cScriptPlayerEntity* owner = self->mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = self->mpBlockArray;
        if (heapBlock != 0) {
            cScriptHeap_FreeMem((cScriptHeap*)(((&owner->mpOwner->mpManager->mHeap))), heapBlock);
        }
        self->mpData = 0;
        self->mpOwner = 0;
        self->mpHeader = 0;
        self->mpNextKey = 0;
        self->mNextKeyIndex = 0;
        self->mpBlockArray = 0;
        self->mpActiveBlockList = 0;
        self->mNumActiveBlocks = 0;
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
 * @return 0 when no key is active or pending, otherwise 1
 */
extern "C" int cScriptPlayerChannelBlock_Update(cScriptPlayerChannelBlock* self, int* flags, float currFrameNum)
{
    float zeroF;
    tBlockEntry* prev = 0;
    tBlockEntry* event = self->mpActiveBlockList;
    while (event != 0) {
        if ((float)event->data.scriptEndFrame <= currFrameNum) {
            *flags |= 8;
            if (prev == 0) {
                event = *(tBlockEntry**)&self->mpActiveBlockList = event->pNext;
            } else {
                event = event->pNext;
                prev->pNext = event;
            }
            self->mNumActiveBlocks--;
        } else {
            prev = event;
            event = event->pNext;
        }
    }
    zeroF = 0.0f;
loop:
    do {
        if (self->mpNextKey != 0 && currFrameNum - (float)(self->mpNextKey)->frameStart >= zeroF) {
            *flags |= 4;
            cScriptPlayerChannelBlock_AddActiveKey((cScriptPlayerChannelBlock*)self, (sCacheRecord*)(self->mpNextKey));
            cScriptPlayerChannelBlock_LoadNextScriptKey((cScriptPlayerChannelBlock*)self);
            goto loop;
        }
    } while (0);
    if (self->mpActiveBlockList == 0 && self->mpNextKey == 0)
        return 0;
    return 1;
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
extern "C" int cScriptPlayerChannelBlock_GetNextKeyFrame(cScriptPlayerChannelBlock* self)
{
    if ((int)self->mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)self->mpHeader == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode* n = (sCacheTimeNode*)self->mpActiveBlockList;
    unsigned short* cur = (unsigned short*)self->mpNextKey;
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
extern "C" void cScriptPlayerChannelBlock_LoadNextScriptKey(cScriptPlayerChannelBlock* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == self->mpHeader->numKeys - 1) {
        self->mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqBlockChannelKey* key = self->mpNextKey;
        char* cur = (char*)key;
        self->mpNextKey = (tInSeqBlockChannelKey*)(cur + key->keySize);
        self->mNextKeyIndex = idx + 1;
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
    cScriptPlayerChannelEvent_Stop(self);
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
 * @return 1 while a key is still pending, otherwise 0
 */
extern "C" int cScriptPlayerChannelEvent_Play(cScriptPlayerChannelEvent* self, unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    self->mpData = (const char*)data;
    self->mpOwner = (cScriptPlayerEntity*)owner;
    self->mpHeader = (tInSeqEventChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        self->mpEventArray = (tEventEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 8);
        self->mNextKeyIndex = 0;
        tInSeqEventChannelHeader* base = self->mpHeader;
        self->mpNextKey = (tInSeqEventChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqEventChannelKey*)&base->keyStart)->frameNum == 0) do {
            *flags |= 0x10;
            cScriptPlayerChannelEvent_AddActiveKey((cScriptPlayerChannelEvent*)self, self->mpNextKey);
            cScriptPlayerChannelEvent_LoadNextScriptKey((cScriptPlayerChannelEvent*)self);
            *(int*)&self->mEventFrame = 0;
        } while (self->mpNextKey != 0 &&
               self->mpNextKey->frameNum == 0);
    }
    return self->mpNextKey != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_Stop);
#ifdef SKIP_ASM
/**
 * Stops an event channel: frees its key buffer to the owner's script heap and clears its state (only when it has an
 * owner).
 */
extern "C" void cScriptPlayerChannelEvent_Stop(cScriptPlayerChannelEvent* self)
{
    cScriptPlayerEntity* owner = self->mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = self->mpEventArray;
        if (heapBlock != 0) {
            cScriptHeap_FreeMem((cScriptHeap*)(((&owner->mpOwner->mpManager->mHeap))), heapBlock);
        }
        self->mpData = 0;
        self->mpOwner = 0;
        self->mpHeader = 0;
        self->mpNextKey = 0;
        self->mNextKeyIndex = 0;
        self->mpEventArray = 0;
        self->mNumActiveEvents = 0;
        *(int*)&self->mEventFrame = 0;
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
 * @return 1 while a key is still pending, otherwise 0
 */
extern "C" int cScriptPlayerChannelEvent_Update(cScriptPlayerChannelEvent* self, int* flags, float currFrameNum)
{
    if (self->mEventFrame != currFrameNum) {
        self->mNumActiveEvents = 0;
    }
    while ((char*)self->mpNextKey != 0) {
        float keyFrameNum = (float)self->mpNextKey->frameNum;
        if (currFrameNum < keyFrameNum) break;
        *flags |= 0x10;
        cScriptPlayerChannelEvent_AddActiveKey((cScriptPlayerChannelEvent*)self, self->mpNextKey);
        self->mEventFrame = keyFrameNum;
        cScriptPlayerChannelEvent_LoadNextScriptKey((cScriptPlayerChannelEvent*)self);
    }
    return (char*)self->mpNextKey != 0;
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
extern "C" int cScriptPlayerChannelEvent_GetNextKeyFrame(cScriptPlayerChannelEvent* self)
{
    if ((int)self->mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)self->mpHeader == 0) {
        return -1;
    }
    unsigned short* cur = (unsigned short*)self->mpNextKey;
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
extern "C" tEventEntry* cScriptPlayerChannelEvent_AddActiveKey(cScriptPlayerChannelEvent* self,
                                                               tInSeqEventChannelKey* rec)
{
    tEventEntry* newKey = &self->mpEventArray[self->mNumActiveEvents];
    newKey->eventID = rec->enumVal;
    newKey->pCustom = 0;
    if (rec->keySize > 8) {
        newKey->pCustom = &rec->customDataStart;
    }
    self->mNumActiveEvents += 1;
    return newKey;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_LoadNextScriptKey);
#ifdef SKIP_ASM
/**
 * Steps mpNextKey to the following key (each key starts with its size), or to 0 after the header's last key.
 */
extern "C" void cScriptPlayerChannelEvent_LoadNextScriptKey(cScriptPlayerChannelEvent* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == self->mpHeader->numKeys - 1) {
        self->mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqEventChannelKey* key = self->mpNextKey;
        char* cur = (char*)key;
        self->mpNextKey = (tInSeqEventChannelKey*)(cur + key->keySize);
        self->mNextKeyIndex = idx + 1;
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
    cScriptPlayerChannelObject_Stop(self);
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
 * @return 1 when the channel has keys, otherwise 0
 */
extern "C" int cScriptPlayerChannelObject_Play(cScriptPlayerChannelObject* self, unsigned short* data, cScriptPlayerEntity* owner, int* flags)
{
    self->mpData = (const char*)data;
    self->mpOwner = (cScriptPlayerEntity*)owner;
    self->mpHeader = (tInSeqObjectChannelHeader*)data;
    if (data[0] != 0) {
        cInSeq* heap = owner->mpOwner->mpManager;
        self->mpObjectArray = (tObjectEntry*)cScriptHeap_AllocMem(&heap->mHeap, data[1] * 36);
        for (int i = 0; i < self->mpHeader->numTracks; i++) {
        }
        self->mNextKeyIndex = 0;
        tInSeqObjectChannelHeader* base = self->mpHeader;
        self->mpNextKey = (tInSeqObjectChannelKey*)&base->keyStart;
        if (&base->keyStart != 0 && ((tInSeqObjectChannelKey*)&base->keyStart)->frameStart == 0) do {
            *flags |= 1;
            cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)self, (sCacheRecord2*)self->mpNextKey);
            cScriptPlayerChannelObject_LoadNextScriptKey((cScriptPlayerChannelObject*)self);
        } while (self->mpNextKey != 0 &&
               self->mpNextKey->frameStart == 0);
    }
    return self->mpHeader->numKeys != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_Stop);
#ifdef SKIP_ASM
/**
 * Stops an object channel: frees its active-key buffer to the owner's script heap and clears its state (only when
 * it has an owner).
 */
extern "C" void cScriptPlayerChannelObject_Stop(cScriptPlayerChannelObject* self)
{
    cScriptPlayerEntity* owner = self->mpOwner;
    if (((char*)owner) != 0) {
        void* heapBlock = self->mpObjectArray;
        if (heapBlock != 0) {
            cScriptHeap_FreeMem((cScriptHeap*)(((&owner->mpOwner->mpManager->mHeap))), heapBlock);
        }
        self->mpData = 0;
        self->mpOwner = 0;
        self->mpHeader = 0;
        self->mpNextKey = 0;
        self->mNextKeyIndex = 0;
        self->mpObjectArray = 0;
        self->mpActiveObjectList = 0;
        self->mNumActiveObjects = 0;
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
 * @return 0 when no key is active or pending, otherwise 1
 */
extern "C" int cScriptPlayerChannelObject_Update(cScriptPlayerChannelObject* self, int* flags, float frameDelta, float dt)
{
    float zeroF;
    tObjectEntry* prev = 0;
    tObjectEntry* event = self->mpActiveObjectList;
    while (event != 0) {
        if ((float)event->data.scriptEndFrame <= frameDelta) {
            *flags |= 2;
            if (prev == 0) {
                event = *(tObjectEntry**)&self->mpActiveObjectList = event->pNext;
            } else {
                event = event->pNext;
                prev->pNext = event;
            }
            self->mNumActiveObjects--;
        } else {
            prev = event;
            event->data.currObjectFrame += event->data.playRate * dt;
            event = event->pNext;
        }
    }
    zeroF = 0.0f;
loop:
    do {
        if (self->mpNextKey != 0) {
            float frameDiff = frameDelta - (float)(self->mpNextKey)->frameStart;
            if (frameDiff >= zeroF) {
                *flags |= 1;
                tObjectEntry* n = (tObjectEntry*)cScriptPlayerChannelObject_AddActiveKey((cScriptPlayerChannelObject*)self, (sCacheRecord2*)(self->mpNextKey));
                n->data.currObjectFrame += frameDiff * n->data.playRate;
                cScriptPlayerChannelObject_LoadNextScriptKey((cScriptPlayerChannelObject*)self);
                goto loop;
            }
        }
    } while (0);
    if (self->mpActiveObjectList == 0 && self->mpNextKey == 0)
        return 0;
    return 1;
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
extern "C" int cScriptPlayerChannelObject_GetNextKeyFrame(cScriptPlayerChannelObject* self)
{
    if ((int)self->mpData == 0) {
        return -1;
    }
    if (*(unsigned short*)self->mpHeader == 0) {
        return -1;
    }
    int best = -1;
    sCacheTimeNode2* n = (sCacheTimeNode2*)self->mpActiveObjectList;
    unsigned short* cur = (unsigned short*)self->mpNextKey;
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
extern "C" void cScriptPlayerChannelObject_LoadNextScriptKey(cScriptPlayerChannelObject* self)
{
    int idx = self->mNextKeyIndex;
    if (idx == self->mpHeader->numKeys - 1) {
        self->mpNextKey = 0;
    } else {
        // ORIGINAL-PTR: keys are variable-length records; the next one starts keySize bytes on
        tInSeqObjectChannelKey* key = self->mpNextKey;
        char* cur = (char*)key;
        self->mpNextKey = (tInSeqObjectChannelKey*)(cur + key->keySize);
        self->mNextKeyIndex = idx + 1;
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
    cScriptPlayerEntity_DeInit(self);
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
extern "C" int cScriptPlayerEntity_Init(cScriptPlayerEntity* self, tInSeqScriptEntityBlockHeader* data, int size)
{
    self->mpData = (const char*)data;
    self->mpBlockHeader = (tInSeqScriptEntityBlockHeader*)data;
    self->mpOwner = (cScriptPlayerMain*)size;
    self->mpVariationTable = (unsigned int*)(&data->varOffsetTableStart);
    self->mCurrVariation = 0;
    // ORIGINAL-PTR: the variation offset table holds byte offsets into the entity data
    tInSeqEntityHeader* rec = (tInSeqEntityHeader*)(((char*)data) + (int)data->varOffsetTableStart);
    self->mpSystemHeader = rec;
    if (rec->headerSize > 8) {
        self->mpHeader = (const char*)(&rec->nextSectionStart);
    } else {
        self->mpHeader = 0;
    }
    return self->mpBlockHeader->blockSize;
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
extern "C" void cScriptPlayerEntity_DeInit(cScriptPlayerEntity* self)
{
    if ((int)self->mpData != 0) {
        cScriptPlayerEntity_Stop(self);
        self->mpData = 0;
        self->mpOwner = 0;
        self->mpUserData = 0;
        self->mpBlockHeader = 0;
        self->mpVariationTable = 0;
        self->mCurrVariation = 0;
        self->mpSystemHeader = 0;
        self->mpHeader = 0;
        self->mSetupScriptHasKeysLeft = 0;
        self->mSetupScriptInitialEventBits = 0;
        self->mWaitingPlayExec = 0;
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
extern "C" void cScriptPlayerEntity_SetEntityVariation(cScriptPlayerEntity* self, int idx)
{
    self->mCurrVariation = idx;
    // ORIGINAL-PTR: the variation offset table holds byte offsets into the entity data
    tInSeqEntityHeader* rec = (tInSeqEntityHeader*)(self->mpData + self->mpVariationTable[idx]);
    self->mpSystemHeader = rec;
    if (rec->headerSize > 8) {
        self->mpHeader = (const char*)(&rec->nextSectionStart);
    } else {
        self->mpHeader = 0;
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
extern "C" int cScriptPlayerEntity_SetupPlay(cScriptPlayerEntity* self)
{
    self->mSetupScriptHasKeysLeft = 0;
    self->mSetupScriptInitialEventBits = 0;
    if (self->mpSystemHeader->numChannels) {
        void* heap = (char*)&self->mpOwner->mpManager->mHeap;
        self->mpChannelPlayers =
            (cScriptPlayerChannel**)cScriptHeap_AllocMem(heap, self->mpSystemHeader->numChannels * 4);
        // ORIGINAL-PTR: the channel records follow the variation header (headerSize bytes) and are dataSize bytes long
        char* channelData = (char*)(self->mpSystemHeader) + (self->mpSystemHeader)->headerSize;
        for (int i = 0; i < (self->mpSystemHeader)->numChannels; i++) {
            tInSeqEntityChannelHeader* channelHeader = (tInSeqEntityChannelHeader*)channelData;
            channelData += 4;
            // &mpChannelPlayers[i] in integer form below: the subscript changes retail's register allocation
            switch (channelHeader->type) {
            case 0: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)self->mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelSpline_cScriptPlayerChannelSpline((cScriptPlayerChannelSpline*)func_00272488_impl(40, heap));
                break;
            }
            case 1: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)self->mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelObject_cScriptPlayerChannelObject((cScriptPlayerChannelObject*)func_00272488_impl(40, heap));
                break;
            }
            case 2: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)self->mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelBlock_cScriptPlayerChannelBlock((cScriptPlayerChannelBlock*)func_00272488_impl(44, heap));
                break;
            }
            case 3: {
                cScriptPlayerChannel** slot = (cScriptPlayerChannel**)((i << 2) + (int)self->mpChannelPlayers);
                *slot = (cScriptPlayerChannel*)cScriptPlayerChannelEvent_cScriptPlayerChannelEvent((cScriptPlayerChannelEvent*)func_00272488_impl(40, heap));
                break;
            }
            }
            int flag = 0;
            int* pd = &self->mSetupScriptHasKeysLeft;
            int keysLeft = 0;
            cScriptPlayerChannel* channel = self->mpChannelPlayers[i];
            if (channel->Play(channelData, self, &flag) || self->mSetupScriptHasKeysLeft)
                keysLeft = 1;
            *pd = keysLeft;
            self->mSetupScriptInitialEventBits |= flag;
            channelData += channelHeader->dataSize;
        }
    }
    self->mIsPlaying = 1;
    cInSeqComm* comm = (cInSeqComm*)cScriptPlayerMain_GetInSeqCommInterface((cScriptPlayerMain*)self->mpOwner);
    if (comm) {
        self->mpEntityComm = comm->OnInSeqRequestEntityCommInterface(self, self->mpSystemHeader->type);
    }
    if (self->mpEntityComm) {
        cInSeqCommEntity* entityComm = self->mpEntityComm;
        entityComm->OnInSeqLock(self, self->mpSystemHeader->type);
    }
    self->mWaitingPlayExec = 1;
    return self->mIsPlaying;
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
extern "C" int cScriptPlayerEntity_ExecutePlay(cScriptPlayerEntity* self)
{
    cInSeqCommEntity* obj = self->mpEntityComm;
    int ret = self->mSetupScriptHasKeysLeft;
    if (obj != 0) {
        int eventBits = self->mSetupScriptInitialEventBits;
        if (eventBits != 0) {
            obj->OnInSeqKeyEvent(self, self->mpSystemHeader->type, 0.0f, eventBits);
        }
        self->mpEntityComm->OnInSeqUpdate(self,
            self->mpSystemHeader->type,
            0.0f, 0.0f, self->mSetupScriptInitialEventBits);
    }
    self->mSetupScriptHasKeysLeft = 0;
    self->mSetupScriptInitialEventBits = 0;
    self->mWaitingPlayExec = 0;
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
extern "C" void cScriptPlayerEntity_Stop(cScriptPlayerEntity* self)
{
    if (self->mIsPlaying == 0)
        return;
    cRcListener_3DC0* entityComm = (cRcListener_3DC0*)self->mpEntityComm;
    if (entityComm != 0)
        entityComm->OnInSeqRelease(self, self->mpSystemHeader->type);
    self->mpEntityComm = 0;
    if (self->mpSystemHeader->numChannels != 0)
    {
        for (int i = 0; i < self->mpSystemHeader->numChannels; i++)
        {
            cRcObj_3DC0** objs = (cRcObj_3DC0**)self->mpChannelPlayers;
            objs[i]->Stop();
            switch (((cRcObj_3DC0*)self->mpChannelPlayers[i])->GetType())
            {
            case 0:
                func_002724E8_impl(((cRcObj_3DC0*)self->mpChannelPlayers[i]));
                break;
            case 1:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)self->mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            case 2:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)self->mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            case 3:
            {
                cRcObj_3DC0* channelPlayer = ((cRcObj_3DC0*)self->mpChannelPlayers[i]);
                if (channelPlayer)
                    channelPlayer->v10(3);
                break;
            }
            }
        }
        cScriptHeap_FreeMem((cScriptHeap*)(((&self->mpOwner->mpManager->mHeap))), self->mpChannelPlayers);
        self->mpChannelPlayers = 0;
    }
    self->mIsPlaying = 0;
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
extern "C" int cScriptPlayerEntity_Restart(cScriptPlayerEntity* self)
{
    if (self->mIsPlaying == 0)
        return 0;
    tInSeqEntityHeader* hdr = self->mpSystemHeader;
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
            self->mpChannelPlayers[i]->Stop();
            int channelEvents = 0;
            if (self->mpChannelPlayers[i]->Play(channelData, self, &channelEvents) != 0 || changed)
                ch = 1;
            i++;
            channelData += rec->dataSize;
            changed = ch;
            mask |= channelEvents;
        } while (i < self->mpSystemHeader->numChannels);
    }
    cInSeqCommEntity* entityComm = self->mpEntityComm;
    if (entityComm == 0)
        return changed;
    if (mask)
        entityComm->OnInSeqKeyEvent(self, self->mpSystemHeader->type, 0.0f, mask);
    self->mpEntityComm->OnInSeqUpdate(self, self->mpSystemHeader->type, 0.0f, 0.0f, mask);
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
 * @param self_  the script entity (this)
 * @param prev   the previous frame number
 * @param time   the new frame number
 * @param speed  the frame delta
 * @param jump   nonzero to visit every key between @p prev and @p time
 * @return the last cScriptPlayerEntity_DoFrameUpdate result (1 when not playing)
 */
extern "C" int cScriptPlayerEntity_Update(cScriptPlayerEntity* self_, float prev, float time, float speed, int jump)
{
    cScriptPlayerEntity* self = self_;
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
                return cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, time, time - prev);
            }
            float delta = next - prev;
            prev = next;
            if (!cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, prev, delta)) {
                return 0;
            }
        }
    }
    return cScriptPlayerEntity_DoFrameUpdate((cScriptPlayerEntity*)self, time, speed);
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_SetUserData);
#ifdef SKIP_ASM
/**
 * Replaces the entity's mpUserData with @p userData and returns the previous value.
 *
 * @param entity  the script player entity
 * @param userData  the new user data
 * @return the old mpUserData
 */
extern "C" int cScriptPlayerEntity_SetUserData(cScriptPlayerEntity* entity, int userData) {
    int temp_2;

    temp_2 = ((int)entity->mpUserData);
    entity->mpUserData = (void*)userData;
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
extern "C" cInSeqCommEntity* cScriptPlayerEntity_SetEntityComm(cScriptPlayerEntity* self,
                                                              cInSeqCommEntity* pEntityComm)
{
    cInSeqCommEntity* old = self->mpEntityComm;
    if (old != 0) {
        old->OnInSeqRelease(self, self->mpSystemHeader->type);
    }
    self->mpEntityComm = pEntityComm;
    if (pEntityComm != 0) {
        pEntityComm->OnInSeqLock(self, self->mpSystemHeader->type);
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
int cScriptPlayerEntity_GetChannel(void* vself, int channelIndex)
{
    cScriptPlayerEntity* self = (cScriptPlayerEntity*)vself;
    return (int)self->mpChannelPlayers[channelIndex];
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
extern "C" void* cScriptPlayerEntity_GetHeader(cScriptPlayerEntity* self, int i)
{
    int* table = (int*)self->mpVariationTable;
    const char* entityData = self->mpData + table[i];  // the table holds byte offsets into the entity data
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
int cScriptPlayerEntity_GetScriptLength(void* vself)
{
    cScriptPlayerEntity* self = (cScriptPlayerEntity*)vself;
    return self->mpSystemHeader->numFrames;
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
 * @param scriptPlayerEntity  the script player entity
 * @return mCurrVariation
 */
extern "C" int cScriptPlayerEntity_GetCurrVariation(cScriptPlayerEntity* scriptPlayerEntity) {
    return (scriptPlayerEntity->mCurrVariation);
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
int cScriptPlayerEntity_GetNumVariations(void* vself)
{
    cScriptPlayerEntity* self = (cScriptPlayerEntity*)vself;
    return (int)self->mpBlockHeader->numVariations;
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
 * @param scriptPlayerEntity  the script player entity
 * @return the script's projectID
 */
extern "C" int cScriptPlayerEntity_GetProjectID(cScriptPlayerEntity* scriptPlayerEntity) {
    return ((int)scriptPlayerEntity->mpOwner->mpSystemHeader->projectID);
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerEntity_GetDataSize);
#ifdef SKIP_ASM
/**
 * Returns the size in bytes of the entity's script data block (mpBlockHeader->blockSize).
 *
 * @param scriptPlayerEntity  the script player entity
 * @return the block size
 */
extern "C" int cScriptPlayerEntity_GetDataSize(cScriptPlayerEntity* scriptPlayerEntity) {
    return *((int*)scriptPlayerEntity->mpBlockHeader);
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
unsigned char cScriptPlayerEntity_GetType(void* vself)
{
    cScriptPlayerEntity* self = (cScriptPlayerEntity*)vself;
    return self->mpSystemHeader->type;
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
 * @param entity             the entity player (this)
 * @param currFrameNum  script frame to update to
 * @param dt            frame step since the last update (On Tour frameDelta)
 * @return nonzero while any channel is still playing (its Update returned true)
 */
extern "C" int cScriptPlayerEntity_DoFrameUpdate(cScriptPlayerEntity* entity, float currFrameNum, float dt)
{
    cScriptPlayerEntity* self = entity;
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
extern "C" void cScriptPlayerMain__cScriptPlayerMain(int* self, int flags)
{
    cScriptPlayerMain_DeInit((cScriptPlayerMain*)self);
    if (flags & 1) {
        operator_delete(self);
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
            entityData += cScriptPlayerEntity_Init(&self->mpEntityPlayers[i],
                                                   (tInSeqScriptEntityBlockHeader*)entityData, (int)self);
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
 * @param scriptPlayerMain  the script player
 */
extern "C" void cScriptPlayerMain_DeInit(cScriptPlayerMain* scriptPlayerMain)
{
    cScriptPlayerMain* self = scriptPlayerMain;
    if (((void*)self->mpData) == 0) {
        return;
    }
    cScriptPlayerMain_Stop((cScriptPlayerMain*)self);
    for (int i = 0; i < (self->mpSystemHeader)->numEntities; i++) {
        cScriptPlayerEntity_DeInit((cScriptPlayerEntity*)(&(self->mpEntityPlayers)[i]));
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
void cScriptPlayerMain_SetPlaySpeed(void* vself, float val)
{
    cScriptPlayerMain* self = (cScriptPlayerMain*)vself;
    self->mPlaySpeed = val;
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
void* cScriptPlayerMain_Play(void* vself)
{
    cScriptPlayerMain* self = (cScriptPlayerMain*)vself;
    return cScriptPlayerMain_Play1(self, self->mLoopScript);
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
 * @param scriptPlayerMain  the script player
 * @param loopScript  non-zero to loop
 */
extern "C" void cScriptPlayerMain_SetLoop(cScriptPlayerMain* scriptPlayerMain, int loopScript) {
    scriptPlayerMain->mLoopScript = loopScript;
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
extern "C" void cScriptPlayerMain_Preview(cScriptPlayerMain* self, int flag)
{
    float savedScale = self->mPlaySpeed;
    self->mpPreviewCommInterface = (cInSeqComm*)flag;
    self->mPlaySpeed = 1.0f;
    int savedIdx = self->mLoopScript;
    cScriptPlayerMain_Play1_v(((char*)self), 0);
    while (self->mPlayState != 0) {
        self->mPlaySpeed = 1.0f;
        cScriptPlayerMain_UpdateFrame(self);
    }
    self->mPlaySpeed = savedScale;
    self->mLoopScript = savedIdx;
    self->mpPreviewCommInterface = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_GoToFrame);
#ifdef SKIP_ASM
/**
 * Asks the script player to jump to frame @p frameNum on its next update (mJumpFrame) and clears mNumSteps.
 *
 * @param scriptPlayerMain  the script player
 * @param frameNum  the frame to jump to
 */
extern "C" void cScriptPlayerMain_GoToFrame(cScriptPlayerMain* scriptPlayerMain, int frameNum) {
    scriptPlayerMain->mJumpFrame = frameNum;
    scriptPlayerMain->mNumSteps = 0;
}
#endif
//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerMain_StepNextFrame);
#ifdef SKIP_ASM
/**
 * Asks the script player to advance one more frame step (increments mNumSteps).
 *
 * @param scriptPlayerMain  the script player
 */
extern "C" void cScriptPlayerMain_StepNextFrame(cScriptPlayerMain* scriptPlayerMain) {
    scriptPlayerMain->mNumSteps = (int) (scriptPlayerMain->mNumSteps + 1);
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
extern "C" void cScriptPlayerMain_Stop(cScriptPlayerMain* self)
{
    if (self->mPlayState == 0) return;
    if (self->mEntityUpdateInProgress != 0) {
        self->mDoStopInEntityUpdate = 1;
        return;
    }
    for (int i = 0; i < self->mpSystemHeader->numEntities; i++) {
        cScriptPlayerEntity_Stop(&self->mpEntityPlayers[i]);
    }
    self->mPlayState = 0;
    *(int*)&self->mCurrFrame = 0;
    self->mNumPaused = 0;
    self->mNumSteps = 0;
    self->mScriptLength = -1;
    self->mJumpFrame = -1;
    cInSeqComm* inSeqComm = (cInSeqComm*)cScriptPlayerMain_GetInSeqCommInterface(self);
    if (inSeqComm != 0) {
        inSeqComm->OnInSeqStop(self);
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
extern "C" void cScriptPlayerMain_Pause(cScriptPlayerMain* self)
{
    if (self->mPlayState != 0) {
        self->mNumPaused += 1;
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
extern "C" void cScriptPlayerMain_Resume(cScriptPlayerMain* self)
{
    if (self->mPlayState != 0) {
        int numPaused = self->mNumPaused - 1;
        self->mNumPaused = numPaused;
        if (numPaused < 0) {
            self->mNumPaused = 0;
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
 * @param scriptPlayerMain  the script player (this)
 */
extern "C" void cScriptPlayerMain_UpdateFrame(cScriptPlayerMain* scriptPlayerMain)
{
    cScriptPlayerMain* self = scriptPlayerMain;
    int need = 1;
    if (self->mJumpFrame >= 0)
    {
        int target = self->mJumpFrame;
        self->mJumpFrame = -1;
        int len = cScriptPlayerMain_GetScriptLength((cScriptPlayerMain*)self);
        int saved = self->mNumPaused;
        self->mNumPaused = 0;
        if (self->mLoopScript != 0)
            target = target % len;
        float ft = (float)target;
        if (self->mPlayState == 1)
            cScriptPlayerMain_DoUpdateFrame(self);
        if (ft != self->mCurrFrame)
        {
            if (ft < self->mCurrFrame)
            {
                cScriptPlayerMain_Stop((cScriptPlayerMain*)self);
                cScriptPlayerMain_Play(self);
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
            cScriptPlayerMain_Pause((cScriptPlayerMain*)self);
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
int cScriptPlayerMain_SetUserData(void* vself, int userData)
{
    cScriptPlayerMain* self = (cScriptPlayerMain*)vself;
    int old = (int)self->mpUserData;
    self->mpUserData = (void*)userData;
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
extern "C" void* cScriptPlayerMain_GetEntity(cScriptPlayerMain* self, int entityIndex)
{
    return (char*)&self->mpEntityPlayers[entityIndex];
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
extern "C" int cScriptPlayerMain_GetScriptLength(cScriptPlayerMain* self)
{
    int scriptLength = self->mScriptLength;
    if (scriptLength < 0) {
        int best = 0;
        int i;
        for (i = 0; i < self->mpSystemHeader->numEntities; i++) {
            int entityLength = cScriptPlayerEntity_GetScriptLength(&(self->mpEntityPlayers)[i]);
            if (best < entityLength) {
                best = entityLength;
            }
        }
        if (self->mPlayState != 0) {
            self->mScriptLength = best;
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
extern "C" int cScriptPlayerMain_GetNumUpdatesToEndLeft(cScriptPlayerMain* self)
{
    float x = ((float)cScriptPlayerMain_GetScriptLength(self) - self->mCurrFrame) / self->mPlaySpeed;
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
extern "C" int cScriptPlayerMain_GetInSeqCommInterface(cScriptPlayerMain* self)
{
    int previewComm = (int)self->mpPreviewCommInterface;
    if (previewComm != 0) {
        return previewComm;
    }
    return *(int*)self->mpManager;
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
    if (self->mPlayState == 1) {
        self->mPlayState = 2;
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
    if (self->mPlayState == 4 && scriptLength <= self->mCurrFrame) {
        cScriptPlayerMain_Stop_4DE0(self);
        return;
    }
    if ((unsigned)(self->mPlayState - 3) < 2) {
        float step = self->mPlaySpeed;
        float currFrame = self->mCurrFrame;
        float fullStep = step;
        float lim = currFrame + step;
        if (self->mPlayState == 4) {
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
    if (self->mDoStopInEntityUpdate != 0 || self->mPlayState == 4) {
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
extern "C" int cScriptPlayerMain_DoUpdate(cScriptPlayerMain* self, float time, float prev, float speed)
{
    int changed = 0;
    int jump = 0;
    self->mCurrFrame = time;
    if (speed > 1.0f) {
        jump = 1;
    } else {
        int ti = (int)time;
        int pi = (int)prev;
        if ((float)ti != time) {
            jump = pi != ti;
        }
    }
    for (int i = 0; i < self->mpSystemHeader->numEntities; i++) {
        int anyChanged = 0;
        if (cScriptPlayerEntity_Update(&self->mpEntityPlayers[i], prev, self->mCurrFrame, speed, jump) != 0 || changed != 0) {
            anyChanged = 1;
        }
        changed = anyChanged;
    }
    cInSeqComm* comm = (cInSeqComm*)cScriptPlayerMain_GetInSeqCommInterface(self);
    if (comm != 0) {
        comm->OnInSeqUpdate(self, self->mCurrFrame, speed);
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
void* cScriptPlayerChannelSpline_GetValue(void* self)
{
    return &((cScriptPlayerChannelSpline*)self)->mCurrKeyValue;
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
int cScriptPlayerChannelSpline_GetCustom(void* vself)
{
    cScriptPlayerChannelSpline* self = (cScriptPlayerChannelSpline*)vself;
    return (int)self->mpCurrKeyCustom;
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
int cScriptPlayerChannelSpline_GetNextKeyFrame(void* self)
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
int cScriptPlayerChannelSpline_GetType(void* self)
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
int cScriptPlayerChannelSpline_GetOwner(void* vself)
{
    cScriptPlayerChannelSpline* self = (cScriptPlayerChannelSpline*)vself;
    return (int)self->mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelSpline_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the spline channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelSpline_IsPlaying(void* vself)
{
    cScriptPlayerChannelSpline* self = (cScriptPlayerChannelSpline*)vself;
    return ((int)self->mpData != 0);
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
void* cScriptPlayerChannelBlock_GetValue(void* self)
{
    return &((cScriptPlayerChannelBlock*)self)->mNumActiveBlocks;
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
int cScriptPlayerChannelBlock_GetCustom(void* vself)
{
    cScriptPlayerChannelBlock* self = (cScriptPlayerChannelBlock*)vself;
    return (int)self->mpActiveBlockList;
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
int cScriptPlayerChannelBlock_GetType(void* self)
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
int cScriptPlayerChannelBlock_GetOwner(void* vself)
{
    cScriptPlayerChannelBlock* self = (cScriptPlayerChannelBlock*)vself;
    return (int)self->mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelBlock_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the block channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelBlock_IsPlaying(void* vself)
{
    cScriptPlayerChannelBlock* self = (cScriptPlayerChannelBlock*)vself;
    return ((int)self->mpData != 0);
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
void* cScriptPlayerChannelEvent_GetValue(void* self)
{
    return &((cScriptPlayerChannelEvent*)self)->mNumActiveEvents;
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
int cScriptPlayerChannelEvent_GetCustom(void* vself)
{
    cScriptPlayerChannelEvent* self = (cScriptPlayerChannelEvent*)vself;
    return (int)self->mpEventArray;
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
int cScriptPlayerChannelEvent_GetType(void* self)
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
int cScriptPlayerChannelEvent_GetOwner(void* vself)
{
    cScriptPlayerChannelEvent* self = (cScriptPlayerChannelEvent*)vself;
    return (int)self->mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelEvent_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the event channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelEvent_IsPlaying(void* vself)
{
    cScriptPlayerChannelEvent* self = (cScriptPlayerChannelEvent*)vself;
    return ((int)self->mpData != 0);
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
void* cScriptPlayerChannelObject_GetValue(void* self)
{
    return &((cScriptPlayerChannelObject*)self)->mNumActiveObjects;
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
int cScriptPlayerChannelObject_GetCustom(void* vself)
{
    cScriptPlayerChannelObject* self = (cScriptPlayerChannelObject*)vself;
    return (int)self->mpActiveObjectList;
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
int cScriptPlayerChannelObject_GetType(void* self)
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
int cScriptPlayerChannelObject_GetOwner(void* vself)
{
    cScriptPlayerChannelObject* self = (cScriptPlayerChannelObject*)vself;
    return (int)self->mpOwner;
}
#endif

//100%
INCLUDE_ASM("replay/replaycache", cScriptPlayerChannelObject_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the object channel is playing, from its word at +0x8.
 *
 * @return 1 if the word at +0x8 is nonzero, otherwise 0
 */
int cScriptPlayerChannelObject_IsPlaying(void* vself)
{
    cScriptPlayerChannelObject* self = (cScriptPlayerChannelObject*)vself;
    return ((int)self->mpData != 0);
}
#endif

