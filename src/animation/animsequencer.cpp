#include "common.h"
#include "bx/math3d.h"
#include "animation/animdatabase.h"
#include "vtable.h"
#include "animation/mdfarchive.h"
#include "animation/animsequencer.h"
#include "replay/bxreplayframe.h"


//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_fadeWeight__FP13cAnimSequenceff);
#ifdef SKIP_ASM
/**
 * Starts fading the sequence's blend weight towards @p targetWeight over @p time.
 *
 * Stores TargetWeight and WeightTimer and clears IsFadeOutExpire (On Tour's cAnimSequence::fadeWeight(float
 * targetWeight, float time) with the same field layout).
 *
 * @param targetWeight  the weight to reach
 * @param time  the fade time
 */
void cAnimSequence_fadeWeight(cAnimSequence* self, float targetWeight, float time)
{
    self->TargetWeight = targetWeight;
    self->WeightTimer = time;
    self->IsFadeOutExpire = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_fadeOutAndExpire);
#ifdef SKIP_ASM
/**
 * Starts fading the sequence out and marks it to be removed when the fade ends.
 *
 * The fade-out time (+0x9C) becomes @p time unless a fade-out is already running (fade-in +0x98 is 0, +0x9C nonzero) and
 * is shorter; then the fade-in (+0x98) is cleared and the expire flag (+0xA0) set.
 *
 * @param time  the fade-out time in seconds (On Tour time)
 */
extern "C" void cAnimSequence_fadeOutAndExpire(cAnimSequence* self, float time)
{
    if (self->TargetWeight != 0.0f || self->WeightTimer == 0.0f || time < self->WeightTimer)
    {
        self->WeightTimer = time;
    }
    self->TargetWeight = 0.0f;
    self->IsFadeOutExpire = 1;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_addTrigger);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimTrigger[];


/**
 * Adds a timed trigger to the sequence: an event to fire when playback reaches a time.
 *
 * Pushes a 16-byte node (tag 'AnimTrigger') on the list at +0xAC: {@p key, 0 = timed trigger, @p weight, next}.
 *
 * @param key  the trigger's event id (On Tour id)
 * @param weight  the time it fires at (On Tour time); the current name is misleading
 */
extern "C" void cAnimSequence_addTrigger(cAnimSequence* self, int key, float weight)
{
    tTrigger* n = (tTrigger*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
    tTrigger* old = self->Triggers;
    self->Triggers = (tTrigger*)n;
    n->ID = key;
    n->TriggerTime = weight;
    n->Next = old;
    n->IsEndTrigger = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_addEndTrigger);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimTrigger[];


/**
 * Adds a trigger that fires when the sequence ends.
 *
 * Pushes a 16-byte node (tag 'AnimTrigger') on the list at +0xAC: {@p key, 1 = end trigger, 0, next}.
 *
 * @param key  the trigger's event id (On Tour id)
 */
extern "C" void cAnimSequence_addEndTrigger(cAnimSequence* self, int key)
{
    tTrigger* n = (tTrigger*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
    tTrigger* old = self->Triggers;
    self->Triggers = (tTrigger*)n;
    n->ID = key;
    n->IsEndTrigger = 1;
    n->Next = old;
    (*(int*)&n->TriggerTime) = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_removeTrigger);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Removes the sequence's first timed trigger with event id @p id and time @p time.
 *
 * Walks the trigger list at +0xAC (16-byte nodes: id, end-trigger flag, time, next); a node matches when its id and
 * time are equal and its end-trigger flag is 0. The match is unlinked and freed with operator_delete. Counterpart of
 * cAnimSequence_addTrigger, as cAnimSequence_removeEndTrigger is of cAnimSequence_addEndTrigger.
 *
 * @param id  the trigger's event id
 * @param time  the time the trigger fires at
 */
extern "C" void cAnimSequence_removeTrigger(cAnimSequence* self, int id, float time) {
    tTrigger** link;
    for (link = (tTrigger**)(&self->Triggers); *link != 0; link = &(*link)->Next) {
        tTrigger* n = *link;
        if (n->ID == id && n->TriggerTime == time && n->IsEndTrigger == 0) {
            *link = n->Next;
            operator_delete((int*)n);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_removeEndTrigger);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Removes the sequence's first end trigger with the given id.
 *
 * Walks the trigger list (+0xAC); timed triggers with the same id are skipped.
 *
 * @param key  the trigger's event id (On Tour id)
 */
extern "C" void cAnimSequence_removeEndTrigger(cAnimSequence* self, int key)
{
    tTrigger** pp;
    for (pp = (tTrigger**)&self->Triggers; *pp != 0; pp = &(*pp)->Next)
    {
        if ((*pp)->ID == key)
        {
            tTrigger* n = *pp;
            if (n->IsEndTrigger != 0)
            {
                *pp = n->Next;
                operator_delete((int*)n);
                return;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_removeAllTriggers);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees all of the sequence's triggers.
 *
 * Pops and deletes nodes from the list at +0xAC until it is empty.
 */
extern "C" void cAnimSequence_removeAllTriggers(cAnimSequence* self)
{
    while ((char*)self->Triggers != 0)
    {
        tTrigger* node = self->Triggers;
        self->Triggers = (tTrigger*)node->Next;
        operator_delete((int*)node);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnim);
#ifdef SKIP_ASM

struct sSeqEntry_00313C50
{
    int anim;
    int t;
    float w;
    float len;
    float speed;
    int active;
    int f18;
};

struct sSeq_00313C50
{
    int f0;
    sSeqEntry_00313C50 e[5];
    char pad90[0xC4 - 0x90];
    int dirty;
};

/**
 * Puts an animation into one of the sequence's sub-animation slots.
 *
 * Slot @p i (0x1C bytes from +0x4) gets @p anim and its length, (frame count - 1) / 30 s, read from the bank (@p anim &
 * 0xFF) of cAnimDataBase_TheAnimDataBase, clip (@p anim >> 8). An inactive slot is also reset: time 0, weight 1, speed
 * 1, active, and the sequence marked dirty (+0xC4).
 *
 * @param i  the sub-animation slot (On Tour sub)
 * @param anim  the animation: bank in the low byte, clip above it (On Tour tAnimID)
 */
extern "C" void cAnimSequence_setSubAnim(cAnimSequence* self, int i, int anim)
{
    sSeq_00313C50* sequence = (sSeq_00313C50*)self;
    (sequence->e + i)->anim = anim;
    char* clip = (char*)cAnimDataBase_TheAnimDataBase->DataSources[anim & 0xFF];
    char* clipEntry = (char*)(((unsigned)anim >> 8) * 0x14 + *(int*)(clip + 0x8));
    sequence->e[i].len = (*(unsigned short*)(clipEntry + 0xC) - 1) * 0.03333333507180214f;
    if (sequence->e[i].active == 0)
    {
        sequence->e[i].t = 0;
        sequence->e[i].w = 1.0f;
        sequence->e[i].speed = 1.0f;
        sequence->e[i].active = 1;
        sequence->e[i].f18 = 0;
        sequence->dirty = 1;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimTime);
#ifdef SKIP_ASM
/**
 * Sets the current time of sub-animation slot @p index and sets CheckTriggersAtCurrentFrame.
 *
 * Stores SubAnims[@p index].Time. CheckTriggersAtCurrentFrame is also set by cAnimSequence_setSubAnim when it activates
 * a slot. cAnimSequencer_playAndHold and playAndLoop use it
 * for the start time; riders use it to jump to a frame.
 *
 * @param index  the sub-animation slot
 * @param val  the time in seconds
 */
extern "C" void cAnimSequence_setSubAnimTime(cAnimSequence* self, int index, float val)
{
    self->SubAnims[index].Time = val;
    self->CheckTriggersAtCurrentFrame = 1;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimTimeScale);
#ifdef SKIP_ASM
/**
 * Sets the time scale (playback speed) of sub-animation slot @p index of @p sequence.
 *
 * Stores @p fparg0 at +0x8 of the 0x1C-byte slot (at +0xC + index * 0x1C of the sequence), the field
 * cAnimSequence_setSubAnim resets to 1.
 *
 * @param sequence  the animation sequence
 * @param index  the sub-animation slot
 * @param fparg0  the time scale
 */
extern "C" void cAnimSequence_setSubAnimTimeScale(cAnimSequence* sequence, int index, float fparg0) {
    sequence->SubAnims[index].TimeScale = fparg0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimWeight);
#ifdef SKIP_ASM
/**
 * Sets the blend weight of sub-animation slot @p index.
 *
 * Stores @p val in SubAnims[@p index].Weight; cAnimSequence_getNetWeight multiplies it into the slot's net weight.
 * seg_1218 cross-fades two slots with it (w and 1 - w).
 *
 * @param index  the sub-animation slot
 * @param val  the weight, normally 0-1
 */
extern "C" void cAnimSequence_setSubAnimWeight(cAnimSequence* self, int index, float val)
{
    self->SubAnims[index].Weight = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimType);
#ifdef SKIP_ASM
/**
 * Sets the play type of sub-animation slot @p index: 0 plays once and holds the last frame, 1 loops.
 *
 * Stores @p val in SubAnims[@p index].Type. cAnimSequencer_playAndHold passes 0 and cAnimSequencer_playAndLoop 1.
 *
 * @param index  the sub-animation slot
 * @param val  the play type (0 hold, 1 loop)
 */
extern "C" void cAnimSequence_setSubAnimType(cAnimSequence* self, int index, int val)
{
    self->SubAnims[index].Type = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_resetSubAnim);
#ifdef SKIP_ASM
/**
 * Marks sub-animation slot @p index of @p sequence inactive.
 *
 * Clears the active flag at +0x14 of the 0x1C-byte slot (+0x18 + index * 0x1C); cAnimSequencer_getAnimBlendFrames
 * skips inactive slots, and cAnimSequence_setSubAnim reinitialises the slot the next time it fills it.
 *
 * @param sequence  the animation sequence
 * @param index  the sub-animation slot
 */
extern "C" void cAnimSequence_resetSubAnim(cAnimSequence* sequence, int index) {
    sequence->SubAnims[index].IsActive = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_getNetWeight);
#ifdef SKIP_ASM
/**
 * Returns the effective blend weight of sub-animation slot @p i, with the sequence's start and end fades applied.
 *
 * The weight is the sequence's CurrentWeight times the slot's Weight. While the slot's Time is below StartFadeTime it
 * is limited to time / start fade; while the time left (the slot's TotalTime minus its Time) is below EndFadeTime it
 * is limited to time left / end fade.
 *
 * @param i  the sub-animation slot
 * @return the net weight
 */
// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" float cAnimSequence_getNetWeight(cAnimSequence* self, int i)
{
    float time = self->SubAnims[i].Time;
    float w = self->CurrentWeight * self->SubAnims[i].Weight;
    float end = self->SubAnims[i].TotalTime;
    if (time < self->StartFadeTime)
    {
        w = (time / self->StartFadeTime) <? w;
    }
    float rem = end - time;
    if (rem < self->EndFadeTime)
    {
        w = (rem / self->EndFadeTime) <? w;
    }
    return w;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry_313DD0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

static inline void Ser_313DD0(char* s, void* p, int n)
{
    sVEntry_313DD0* e = &(*(sVEntry_313DD0**)s)[1];
    e->fn(vthis(s, *e), p, n);
}

class cAsStream;

/**
 * Writes an animation sequence to @p stream: its fields, then the number of triggers and each trigger.
 *
 * Each write goes through the stream's first virtual (vtable slot 1: data, size). Fields written: SequenceID, the
 * three SubAnims (84), OrientationOffset (0x20), IsFlipped, Priority, Mask (8), TimeScale .. StartFadeTime (4 each),
 * Events (0x10), EndEvent and CheckTriggersAtCurrentFrame. Then the count of the Triggers list and each 16-byte
 * tTrigger, Next pointer included. cAnimSequence_readFromReplayFrame reads the same format back.
 *
 * @param node  the sequence to write
 * @param stream  the output stream
 */
extern "C" void cAnimSequence_writeToReplayFrame(cAnimSequence* node, cBxReplayFrame* stream)
{
    cAnimSequence* self = (cAnimSequence*)node;
    char* streamPtr = (char*)stream;
    Ser_313DD0(streamPtr, &self->SequenceID, 0x4);
    Ser_313DD0(streamPtr, &self->SubAnims[0], 84);
    Ser_313DD0(streamPtr, &self->OrientationOffset, 0x20);
    Ser_313DD0(streamPtr, &self->IsFlipped, 0x4);
    Ser_313DD0(streamPtr, &self->Priority, 0x4);
    Ser_313DD0(streamPtr, &self->Mask, 0x8);
    Ser_313DD0(streamPtr, &self->TimeScale, 0x4);
    Ser_313DD0(streamPtr, &self->CurrentWeight, 0x4);
    Ser_313DD0(streamPtr, &self->TargetWeight, 0x4);
    Ser_313DD0(streamPtr, &self->WeightTimer, 0x4);
    Ser_313DD0(streamPtr, &self->IsFadeOutExpire, 0x4);
    Ser_313DD0(streamPtr, &self->EndFadeTime, 0x4);
    Ser_313DD0(streamPtr, &self->StartFadeTime, 0x4);
    Ser_313DD0(streamPtr, &self->Events, 0x10);
    Ser_313DD0(streamPtr, &self->EndEvent, 0x4);
    Ser_313DD0(streamPtr, &self->CheckTriggersAtCurrentFrame, 0x4);
    int n = 0;
    tTrigger* trigger = self->Triggers;
    if (trigger != 0) {
        int count = 0;
        do {
            trigger = trigger->Next;
            count++;
        } while (trigger != 0);
        n = count;
    }
    Ser_313DD0(streamPtr, &n, 4);
    for (trigger = self->Triggers; trigger; trigger = trigger->Next) {
        Ser_313DD0(streamPtr, trigger, 0x10);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimTrigger[];

struct sVEntry_314050 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

static inline void Read_314050(char* s, void* p, int n)
{
    sVEntry_314050* e = &(*(sVEntry_314050**)s)[2];
    e->fn(vthis(s, *e), p, n);
}

/**
 * Reads an animation sequence from @p stream in the format cAnimSequence_writeToReplayFrame writes, rebuilding its trigger list.
 *
 * Each read goes through the stream's second virtual (vtable slot 2: data, size), into the same fields cAnimSequence_writeToReplayFrame
 * writes. Then it reads the trigger count and, for each trigger, allocates a 16-byte node tagged "AnimTrigger"
 * (cMemMan_alloc, flags 0x20000000), reads it and appends it to the list at +0xAC, keeping file order. The read next
 * pointer of the last node is not cleared, so the list relies on the saved value being 0.
 *
 * @param node  the sequence to fill (zeroed by the caller)
 * @param stream  the input stream
 */
extern "C" void cAnimSequence_readFromReplayFrame(void* node, void* stream)
{
    cAnimSequence* self = (cAnimSequence*)node;
    char* streamPtr = (char*)stream;
    Read_314050(streamPtr, &self->SequenceID, 0x4);
    Read_314050(streamPtr, &self->SubAnims[0], 84);
    Read_314050(streamPtr, &self->OrientationOffset, 0x20);
    Read_314050(streamPtr, &self->IsFlipped, 0x4);
    Read_314050(streamPtr, &self->Priority, 0x4);
    Read_314050(streamPtr, &self->Mask, 0x8);
    Read_314050(streamPtr, &self->TimeScale, 0x4);
    Read_314050(streamPtr, &self->CurrentWeight, 0x4);
    Read_314050(streamPtr, &self->TargetWeight, 0x4);
    Read_314050(streamPtr, &self->WeightTimer, 0x4);
    Read_314050(streamPtr, &self->IsFadeOutExpire, 0x4);
    Read_314050(streamPtr, &self->EndFadeTime, 0x4);
    Read_314050(streamPtr, &self->StartFadeTime, 0x4);
    Read_314050(streamPtr, &self->Events, 0x10);
    Read_314050(streamPtr, &self->EndEvent, 0x4);
    Read_314050(streamPtr, &self->CheckTriggersAtCurrentFrame, 0x4);
    int n;
    Read_314050(streamPtr, &n, 4);
    tTrigger* tail = self->Triggers;
    for (int i = 0; i < n; i++) {
        char* trigger = (char*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
        Read_314050(streamPtr, trigger, 0x10);
        if ((char*)self->Triggers != 0) {
            tail->Next = (tTrigger*)trigger;
        } else {
            self->Triggers = (tTrigger*)trigger;
        }
        tail = (tTrigger*)trigger;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_init__FPv);
#ifdef SKIP_ASM
/**
 * Initialises an animation sequencer as empty: zeroes its count (On Tour NumPlaying) and its sequence list
 * head.
 */
void cAnimSequencer_init(void* vself)
{
    cAnimSequencer* self = (cAnimSequencer*)vself;
    *(int*)self = 0;
    self->PlayList = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_newSequence);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimSequence[];



extern "C" void* cAnimSequence_cAnimSequence(void* mem, int arg);

/**
 * Creates a new animation sequence and adds it to the sequencer.
 *
 * Allocates 0xD0 bytes tagged 'AnimSequence', constructs it with cAnimSequence_cAnimSequence(@p sequenceID), links it at the head of the
 * list (+0x4, next at +0xC8) and increments the count (+0x0).
 *
 * @param seq  the sequencer (this)
 * @param sequenceID  the new sequence's id (On Tour sequenceID)
 * @return the new sequence
 */
extern "C" void* cAnimSequencer_newSequence(cAnimSequencer* seq, int sequenceID)
{
    cAnimSequencer* self = seq;
    cAnimSequence* node = (cAnimSequence*)cAnimSequence_cAnimSequence(cMemMan_alloc(208, str_AnimSequence, 0x20000000, 0), sequenceID);
    node->Next = self->PlayList;
    *(cAnimSequence**)&self->PlayList = node;
    self->NumPlaying++;
    return node;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_appendSequence);
#ifdef SKIP_ASM

struct sSequencer_00314368 {
    int mCount;                      // 0x0
    cAnimSequence* mFirstSequence; // 0x4
};

/**
 * Appends @p node at the end of the sequencer's list (+0x4, linked through +0xC8) and increments the count (+0x0).
 *
 * Clears the node's next pointer. cAnimSequencer_newSequence links new sequences at the head instead; cAnimSequencer_readFromReplayFrame
 * uses this to keep the saved order.
 *
 * @param node  the sequence to append
 */
extern "C" void cAnimSequencer_appendSequence(cAnimSequencer* self, cAnimSequence* node)
{
    if (self->PlayList != 0)
    {
        cAnimSequence* tail = self->PlayList;
        while (tail->Next != 0)
        {
            tail = tail->Next;
        }
        tail->Next = node;
    }
    else
    {
        self->PlayList = (cAnimSequence*)node;
    }
    node->Next = 0;
    self->NumPlaying++;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_removeSequence);
#ifdef SKIP_ASM

struct sSequencer_003143C8 {
    int mCount;                        // 0x0
    cAnimSequence* mFirstSequence; // 0x4
};

/**
 * Unlinks @p node from the sequencer's list (+0x4, linked through +0xC8) and decrements the count (+0x0).
 *
 * The node is not freed; its next pointer is cleared. Likely bug: a node that is not in the list ends the search with a
 * null predecessor, which is then written through.
 *
 * @param node  the sequence to unlink
 */
extern "C" void cAnimSequencer_removeSequence(cAnimSequencer* self, cAnimSequence* node)
{
    if (self->PlayList == node) {
        self->PlayList = (cAnimSequence*)node->Next;
    } else {
        cAnimSequence* cur = self->PlayList;
        while (cur != 0) {
            if (cur->Next == node) {
                break;
            }
            cur = cur->Next;
        }
        cur->Next = node->Next;
    }
    node->Next = 0;
    self->NumPlaying--;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_getAnimBlendFrames);
#ifdef SKIP_ASM
extern "C" float cAnimSequence_getNetWeight(cAnimSequence* self, int i);

/**
 * Collects the weighted animation frames of all playing sequences for blending.
 *
 * For each sequence (PlayList) and each of its 3 sub-animations with IsActive set, writes a tAnimBlendFrame to @p out:
 * AnimID, Time, the OrientationOffset (two vectors), IsFlipped, Priority, Mask and the net weight from
 * cAnimSequence_getNetWeight. Unlike On Tour there is no bound check on @p out.
 *
 * @param out  receives the frames (On Tour tAnimBlendFrame *frames); must be large enough
 * @return the number of frames written
 */
// PORT: Mask is 64-bit (ld/sd).
extern "C" int cAnimSequencer_getAnimBlendFrames(cAnimSequencer* self, tAnimBlendFrame* out)
{
    int count = 0;
    cAnimSequence* n = self->PlayList;
    while (n != 0)
    {
        for (int j = 0; j < 3; j++)
        {
            if (n->SubAnims[j].IsActive != 0)
            {
                tAnimBlendFrame* blendFrame = &out[count];
                blendFrame->SourceFrame.AnimID = n->SubAnims[j].AnimID;
                blendFrame->SourceFrame.Time = n->SubAnims[j].Time;
                blendFrame->OrientationOffset.P = n->OrientationOffset.P;
                blendFrame->OrientationOffset.Q = n->OrientationOffset.Q;
                blendFrame->IsFlipped = n->IsFlipped;
                blendFrame->Priority = n->Priority;
                blendFrame->Weight = cAnimSequence_getNetWeight(n, j);
                blendFrame->Mask = n->Mask;
                count++;
            }
        }
        n = n->Next;
    }
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_playAndHold);
#ifdef SKIP_ASM
extern "C" void* cAnimSequencer_newSequence(cAnimSequencer* seq, int arg);
extern "C" void cAnimSequence_setSubAnim(cAnimSequence* self, int i, int anim);

/**
 * Starts a new sequence that plays an animation once and holds its last frame.
 *
 * cAnimSequencer_newSequence, then sub-animation 0 gets @p anim (cAnimSequence_setSubAnim), time @p val (cAnimSequence_setSubAnimTime) and type
 * 0, hold (cAnimSequence_setSubAnimType).
 *
 * @param sequenceID  the sequence id (On Tour sequenceID)
 * @param anim  the animation to play (On Tour tAnimID)
 * @param val  the start time in seconds (On Tour starttime)
 * @return the new sequence
 */
extern "C" void* cAnimSequencer_playAndHold(cAnimSequencer* self, int sequenceID, int anim, float val)
{
    void* node = cAnimSequencer_newSequence(self, sequenceID);
    cAnimSequence_setSubAnim((cAnimSequence*)node, 0, anim);
    cAnimSequence_setSubAnimTime((cAnimSequence*)node, 0, val);
    cAnimSequence_setSubAnimType((cAnimSequence*)node, 0, 0);
    return node;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_playAndLoop);
#ifdef SKIP_ASM
extern "C" void* cAnimSequencer_newSequence(cAnimSequencer* seq, int arg);
extern "C" void cAnimSequence_setSubAnim(cAnimSequence* self, int i, int anim);

/**
 * Starts a new sequence that loops an animation.
 *
 * cAnimSequencer_newSequence, then sub-animation 0 gets @p anim (cAnimSequence_setSubAnim), time @p val (cAnimSequence_setSubAnimTime) and type
 * 1, loop (cAnimSequence_setSubAnimType).
 *
 * @param sequenceID  the sequence id (On Tour sequenceID)
 * @param anim  the animation to loop (On Tour tAnimID)
 * @param val  the start time in seconds (On Tour starttime)
 * @return the new sequence
 */
extern "C" void* cAnimSequencer_playAndLoop(cAnimSequencer* self, int sequenceID, int anim, float val)
{
    void* node = cAnimSequencer_newSequence(self, sequenceID);
    cAnimSequence_setSubAnim((cAnimSequence*)node, 0, anim);
    cAnimSequence_setSubAnimTime((cAnimSequence*)node, 0, val);
    cAnimSequence_setSubAnimType((cAnimSequence*)node, 0, 1);
    return node;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_expire);
#ifdef SKIP_ASM

struct sSequencer_003145F8 {
    int mCount;                        // 0x0
    cAnimSequence* mFirstSequence; // 0x4
};

/**
 * Removes a sequence from the sequencer and deletes it.
 *
 * Finds @p node in the list (+0x4, next at +0xC8), unlinks it, frees its triggers (cAnimSequence_removeAllTriggers), deletes it and
 * decrements the count. A sequence not in the list is left alone (On Tour asserts).
 *
 * @param node  the sequence to remove (On Tour seq)
 */
extern "C" void cAnimSequencer_expire(cAnimSequencer* self, cAnimSequence* node)
{
    cAnimSequence** pp;
    for (pp = (cAnimSequence**)&self->PlayList; *pp != 0; pp = &(*pp)->Next)
    {
        cAnimSequence* n = *pp;
        if (n == node)
        {
            *pp = n->Next;
            if (n != 0)
            {
                cAnimSequence_removeAllTriggers((cAnimSequence*)n);
                operator_delete((int*)n);
            }
            self->NumPlaying--;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_expireAll);
#ifdef SKIP_ASM

struct sSequencer_00314668 {
    int mCount;                        // 0x0
    cAnimSequence* mFirstSequence; // 0x4
};

/**
 * Removes and deletes every sequence in the animation sequencer.
 *
 * Each sequence is unlinked from the list head, has its triggers removed (cAnimSequence_removeAllTriggers, On Tour
 * cAnimSequence::removeAllTriggers) and is freed with operator delete; the count drops by one for each.
 */
extern "C" void cAnimSequencer_expireAll(cAnimSequencer* self)
{
    while (self->PlayList != 0)
    {
        cAnimSequence* n = self->PlayList;
        self->PlayList = (cAnimSequence*)n->Next;
        if (n != 0)
        {
            cAnimSequence_removeAllTriggers((cAnimSequence*)n);
            operator_delete((int*)n);
        }
        self->NumPlaying--;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_fadeOutAndExpireAll);
#ifdef SKIP_ASM
/**
 * Fades out every sequence of the sequencer and marks them all to expire.
 *
 * cAnimSequence_fadeOutAndExpire on each sequence of the PlayList.
 *
 * @param time  the fade-out time in seconds (On Tour time)
 */
extern "C" void cAnimSequencer_fadeOutAndExpireAll(cAnimSequencer* self, float time)
{
    for (cAnimSequence* node = self->PlayList; node != 0; node = node->Next)
    {
        cAnimSequence_fadeOutAndExpire(node, time);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_removeAllEndTriggers);
#ifdef SKIP_ASM
extern "C" void cAnimSequence_removeEndTrigger(cAnimSequence* self, int v);

/**
 * Removes the given end trigger from every sequence of the sequencer.
 *
 * cAnimSequence_removeEndTrigger on each sequence of the PlayList.
 *
 * @param trig  the end trigger's event id (On Tour trig)
 */
extern "C" void cAnimSequencer_removeAllEndTriggers(cAnimSequencer* self, int trig)
{
    for (cAnimSequence* node = self->PlayList; node != 0; node = node->Next)
    {
        cAnimSequence_removeEndTrigger(node, trig);
    }
}
#endif



//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_getSequence__FP14cAnimSequenceri);
#ifdef SKIP_ASM
/**
 * Returns the sequence node at a position in the sequencer's list.
 *
 * Follows @p index next links from mFirstSequence; a negative index returns the first node.
 *
 * @param index  zero-based position in the list
 * @return the node, or 0 if the list has no node at that position
 */
cAnimSequence* cAnimSequencer_getSequence(cAnimSequencer* self, int index)
{
    cAnimSequence* cur = self->PlayList;
    while (cur != 0 && index-- > 0) {
        cur = cur->Next;
    }
    return cur;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_isPlaying);
#ifdef SKIP_ASM
/**
 * Tells whether a sequence with the given id is in the sequencer's list (mFirstSequence chain).
 *
 * @param id  the sequence id (the node's first word)
 * @return 1 when found, else 0
 */
extern "C" int cAnimSequencer_isPlaying(cAnimSequencer* self, int id) {
    for (cAnimSequence *n = self->PlayList; n != 0; n = n->Next) {
        if (*(int *)n == id) {
            return 1;
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_getPosition);
#ifdef SKIP_ASM
/**
 * Returns where a sequence sits in the sequencer's list.
 *
 * @param id  the sequence id (the node's first word; On Tour sequenceID)
 * @return the sequence's zero-based position from mFirstSequence, or -1 when it is not queued
 */
extern "C" int cAnimSequencer_getPosition(cAnimSequencer* self, int id) {
    int index = 0;
    for (cAnimSequence *n = self->PlayList; n != 0; n = n->Next) {
        if (*(int *)n == id) {
            return index;
        }
        index++;
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_writeToReplayFrame);
#ifdef SKIP_ASM
class cAsStream {
public:
    virtual void serialize(void* data, int size);
};



extern "C" void cAnimSequence_writeToReplayFrame(cAnimSequence* node, cBxReplayFrame* s);

/**
 * Writes the sequencer to @p stream: its 4-byte count, then each of its sequences (cAnimSequence_writeToReplayFrame) in list order.
 *
 * The count goes through the stream's first virtual (vtable slot 1). cRiderAnim_writeToReplayFrame writes a rider's six sequencers
 * with it; cAnimSequencer_readFromReplayFrame reads the format back.
 *
 * @param stream  the output stream
 */
extern "C" void cAnimSequencer_writeToReplayFrame(cAnimSequencer* self, cBxReplayFrame* stream)
{
    stream->write((const char*)self, 4);
    cAnimSequence* n = self->PlayList;
    for (int i = 0; i < self->NumPlaying; i++)
    {
        cAnimSequence_writeToReplayFrame(n, stream);
        n = n->Next;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void cAnimSequence_readFromReplayFrame(void* node, void* stream);
extern const char str_AnimSequence[];


/**
 * Reads a sequencer from @p stream in the format cAnimSequencer_writeToReplayFrame writes, recreating each sequence.
 *
 * Reads the count through the stream's second virtual (vtable slot 2) and zeroes the sequencer's NumPlaying, then for
 * each sequence allocates 0xD0 bytes tagged "AnimSequence" (flags 0x20000000), zero-fills them (MEM_fill), reads the
 * sequence (cAnimSequence_readFromReplayFrame) and appends it (cAnimSequencer_appendSequence). The PlayList head is not
 * cleared here: cRiderAnim_readFromReplayFrame empties the sequencer with cAnimSequencer_expireAll first.
 *
 * @param stream  the input stream
 */
extern "C" void cAnimSequencer_readFromReplayFrame(cAnimSequencer* self, cBxReplayFrame* stream)
{
    int n;
    stream->read((char*)&n, 4);
    self->NumPlaying = 0;
    for (int i = 0; i < n; i++)
    {
        cAnimSequence* node = new (str_AnimSequence, 0x20000000, 0) cAnimSequence;
        MEM_fill(node, 0, 208);
        cAnimSequence_readFromReplayFrame(node, stream);
        cAnimSequencer_appendSequence(self, node);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_cMdfArchive__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty MDF archive wrapper: clears the owned-file pointer at +0x0 and returns @p self.
 *
 * ai/rider and fe/feridermanager build a local one, attach archive data (cMdfArchive_attachArchive, cMdfArchive_attachConstArchive) and destroy it
 * with cMdfArchive__cMdfArchive.
 *
 * @return @p self
 */
void* cMdfArchive_cMdfArchive(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive__cMdfArchive);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cMdfArchive_purge(void** self);

/**
 * Destroys an MDF archive wrapper: releases it with cMdfArchive_purge, then frees @p self if bit 0 of @p flags is set.
 *
 * Callers with a local archive pass 2 (no delete).
 *
 * @param flags  bit 0: delete the object itself
 */
extern "C" void cMdfArchive__cMdfArchive(int* self, int flags)
{
    cMdfArchive_purge((void**)self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);

/**
 * Releases the archive: frees the owned file buffer (+0x0) with cMemMan_free if there is one and zeroes all four words.
 *
 * The four words are the owned buffer, the archive header (+0x4), the model part table (+0x8) and the data base (+0xC).
 * Archives attached without ownership (cMdfArchive_attachConstArchive) have 0 at +0x0, so nothing is freed.
 */
extern "C" void cMdfArchive_purge(void** self)
{
    if (self[0] != 0)
    {
        cMemMan_free(self[0]);
    }
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
}
#endif

extern "C" void* cMdfArchive_linkArchive(void*);

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_attachArchive__FPvi);
#ifdef SKIP_ASM
// PORT: asm label (same binding the unit uses for cMdfArchive_linkArchive's two-argument body).
void cMdfArchive_linkArchive_impl(void* self, char* data) __asm__("cMdfArchive_linkArchive");

/**
 * Attaches archive data that the wrapper will own: stores @p data at +0x0 (freed by cMdfArchive_purge) and links it.
 *
 * Linking (cMdfArchive_linkArchive) sets the header, part table and data pointers from @p data.
 *
 * @param data  the MDF archive in memory
 */
void cMdfArchive_attachArchive(void* self, int data)
{
    *(int*)self = data;
    cMdfArchive_linkArchive_impl(self, (char*)data);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_attachConstArchive__FPv);
#ifdef SKIP_ASM
/**
 * Attaches archive data without taking ownership: clears the owned pointer at +0x0 and links the archive.
 *
 * The archive pointer arrives in the second argument register, which this matched body does not declare; it passes
 * through unchanged to cMdfArchive_linkArchive. fe/feridermanager calls it with the archive data.
 *
 * @return what cMdfArchive_linkArchive leaves in the return register (callers ignore it)
 */
void* cMdfArchive_attachConstArchive(void* self)
{
    *(int*)self = 0;
    return cMdfArchive_linkArchive(self);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_loadArchive);
#ifdef SKIP_ASM
// PORT: asm label (same two-argument binding cMdfArchive_attachArchive uses for cMdfArchive_linkArchive).
void cMdfArchive_linkArchive_impl(void* self, char* data) __asm__("cMdfArchive_linkArchive");
extern "C" char *FILE_loadpack(int, int);

/**
 * Loads an MDF archive file into memory, owns the buffer (+0x0) and links it.
 *
 * Loads with FILE_loadpack(@p name, @p flags), then cMdfArchive_linkArchive sets the header, part table and data pointers.
 *
 * @param name  the archive's file name
 * @param flags  memory flags for the load
 */
extern "C" void cMdfArchive_loadArchive(cMdfArchive* self, int name, int flags) {
    char *data = FILE_loadpack(name, flags);
    *(char **)self = data;
    cMdfArchive_linkArchive_impl(self, data);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_linkArchive);
#ifdef SKIP_ASM
// The original had the same mismatch: callers (cMdfArchive_attachArchive/cMdfArchive_attachConstArchive, the
// latter already 100%) see the one-argument declaration above and leave $5 as-is,
// while the definition takes two. The asm label binds this two-argument body to
// the symbol without changing the callers' prototype.
// PORT: asm labels are GCC/Clang-only; the port should give callers the real signature.
void cMdfArchive_linkArchive_impl(void* self, char* data) __asm__("cMdfArchive_linkArchive");
/**
 * Points the wrapper at archive @p data: ArchiveHeader, ModelHeaders (the part table) and Data (the packed data base).
 *
 * The part table starts at @p data plus the header's mHeaderOffset; the data base at @p data plus its mBodyOffset.
 * Called by cMdfArchive_attachArchive, cMdfArchive_attachConstArchive and cMdfArchive_loadArchive after they set the
 * owned pointer.
 *
 * @param data  the MDF archive header
 */
void cMdfArchive_linkArchive_impl(void* vself, char* data)
{
    cMdfArchive* self = (cMdfArchive*)vself;
    tMdfArchive* archive = (tMdfArchive*)data;
    self->ArchiveHeader = archive;
    // ORIGINAL-PTR: an MDF file blob: its header holds the byte offsets of the model header table (mHeaderOffset) and
    // the packed data (mBodyOffset) from the start of the file.
    self->ModelHeaders = (tMdfModelHeader*)(data + archive->mHeaderOffset);
    self->Data = data + archive->mBodyOffset;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_getNumModelParts);
#ifdef SKIP_ASM
/**
 * Returns the number of model parts in the archive (the header's short at +0x4), or 0 when no archive is linked.
 *
 * @return the model part count
 */
extern "C" short cMdfArchive_getNumModelParts(cMdfArchive* self) {
    tMdfArchive* header = self->ArchiveHeader;
    if (((char*)header) != 0) {
        return header->mNumModels;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_findModelPart);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Returns 1 if the archive has a model part named @p name (case-insensitive), else 0.
 *
 * Compares @p name with each 0x60-byte entry of the part table (+0x8), for the header's part count. Unlike
 * cMdfArchive_getNumModelParts it does not check for a missing header.
 *
 * @param name  the part name to look for
 * @return 1 when found, else 0
 */
extern "C" int cMdfArchive_findModelPart(cMdfArchive* self, const char* name) {
    int i;
    for (i = 0; i < self->ArchiveHeader->mNumModels; i++) {
        if (strcasecmp(name, (char*)&self->ModelHeaders[i]) == 0)
            return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_getModelPartPtr);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Returns the archive's 0x60-byte header of the model part named @p name (case-insensitive), or 0 if none.
 *
 * The pointer is into the archive's part table (+0x8); unlike cMdfArchive_getModelPart nothing is copied or unpacked.
 *
 * @param name  the part name to look for
 * @return the part header inside the archive, or 0
 */
extern "C" char* cMdfArchive_getModelPartPtr(cMdfArchive* self, const char* name) {
    int i;
    for (i = 0; i < self->ArchiveHeader->mNumModels; i++) {
        if (strcasecmp(name, (char*)&self->ModelHeaders[i]) == 0)
            return (char*)&self->ModelHeaders[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_getModelPart);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" void* cMdfArchive_getModelPartByIndex(void* self, int index);

struct sMdfHeader {
    int pad0;
    short count; // 0x4
};


/**
 * Finds a model part in the MDF archive by name.
 *
 * Compares @p name against each part's name case-insensitively (strcasecmp) and returns the first match
 * through cMdfArchive_getModelPartByIndex().
 *
 * @param name  part name to look for
 * @return the model part, or 0 if no part has that name
 */
extern "C" void* cMdfArchive_getModelPart(cMdfArchive* self, const char* name)
{
    for (int i = 0; i < ((sMdfHeader*)self->ArchiveHeader)->count; i++)
    {
        if (strcasecmp(name, self->ModelHeaders[i].mName) == 0)
            return cMdfArchive_getModelPartByIndex(self, i);
    }
    return 0;
}
#endif


//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_getModelPartPtrByIndex);
#ifdef SKIP_ASM
/**
 * Returns the 0x60-byte header of model part @p index inside the archive's part table (+0x8), without copying it.
 *
 * cMdfArchive_getModelPartByIndex returns an unpacked heap copy instead. No bounds check.
 *
 * @param archive  the archive wrapper
 * @param index  the part index
 * @return the part header inside the archive
 */
extern "C" int cMdfArchive_getModelPartPtrByIndex(cMdfArchive* archive, int index) {
    return ((int)archive->ModelHeaders) + (index * 0x60);
}
#endif
