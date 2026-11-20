#include "common.h"
#include "animation/mdfarchive.h"

struct cAnimSequence {
    char pad_0x00[0x98];
    float TargetWeight;   // 0x98 (On Tour names)
    float WeightTimer;    // 0x9C
    int IsFadeOutExpire;  // 0xA0 (a bool in On Tour; cleared here as a whole word)
};

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
extern "C" void cAnimSequence_fadeOutAndExpire(void* self, float time)
{
    if (*(float*)((char*)self + 0x98) != 0.0f || *(float*)((char*)self + 0x9C) == 0.0f || time < *(float*)((char*)self + 0x9C))
    {
        *(float*)((char*)self + 0x9C) = time;
    }
    *(float*)((char*)self + 0x98) = 0.0f;
    *(int*)((char*)self + 0xA0) = 1;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_addTrigger);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimTrigger[];

struct sSeqNode313A70
{
    int key;
    int active;
    float weight;
    sSeqNode313A70* next;
};

/**
 * Adds a timed trigger to the sequence: an event to fire when playback reaches a time.
 *
 * Pushes a 16-byte node (tag 'AnimTrigger') on the list at +0xAC: {@p key, 0 = timed trigger, @p weight, next}.
 *
 * @param key  the trigger's event id (On Tour id)
 * @param weight  the time it fires at (On Tour time); the current name is misleading
 */
extern "C" void cAnimSequence_addTrigger(void* self, int key, float weight)
{
    sSeqNode313A70* n = (sSeqNode313A70*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
    sSeqNode313A70* old = *(sSeqNode313A70**)((char*)self + 0xAC);
    *(sSeqNode313A70**)((char*)self + 0xAC) = n;
    n->key = key;
    n->weight = weight;
    n->next = old;
    n->active = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_addEndTrigger);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimTrigger[];

struct sSeqNode313AD8
{
    int key;
    int active;
    int unk8;
    sSeqNode313AD8* next;
};

/**
 * Adds a trigger that fires when the sequence ends.
 *
 * Pushes a 16-byte node (tag 'AnimTrigger') on the list at +0xAC: {@p key, 1 = end trigger, 0, next}.
 *
 * @param key  the trigger's event id (On Tour id)
 */
extern "C" void cAnimSequence_addEndTrigger(void* self, int key)
{
    sSeqNode313AD8* n = (sSeqNode313AD8*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
    sSeqNode313AD8* old = *(sSeqNode313AD8**)((char*)self + 0xAC);
    *(sSeqNode313AD8**)((char*)self + 0xAC) = n;
    n->key = key;
    n->active = 1;
    n->next = old;
    n->unk8 = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_removeTrigger);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
struct sNode_00313B38 {
    int id;
    int refs;
    float time;
    sNode_00313B38* next;
};

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
extern "C" void cAnimSequence_removeTrigger(char* self, int id, float time) {
    sNode_00313B38** link;
    for (link = (sNode_00313B38**)(self + 0xAC); *link != 0; link = &(*link)->next) {
        sNode_00313B38* n = *link;
        if (n->id == id && n->time == time && n->refs == 0) {
            *link = n->next;
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
struct sSeqNode313BA8
{
    int key;
    int active;
    int unk8;
    sSeqNode313BA8* next;
};

/**
 * Removes the sequence's first end trigger with the given id.
 *
 * Walks the trigger list (+0xAC); timed triggers with the same id are skipped.
 *
 * @param key  the trigger's event id (On Tour id)
 */
extern "C" void cAnimSequence_removeEndTrigger(void* self, int key)
{
    sSeqNode313BA8** pp;
    for (pp = (sSeqNode313BA8**)((char*)self + 0xAC); *pp != 0; pp = &(*pp)->next)
    {
        if ((*pp)->key == key)
        {
            sSeqNode313BA8* n = *pp;
            if (n->active != 0)
            {
                *pp = n->next;
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
extern "C" void cAnimSequence_removeAllTriggers(char* self)
{
    while (*(char**)(self + 0xAC) != 0)
    {
        char* node = *(char**)(self + 0xAC);
        *(char**)(self + 0xAC) = *(char**)(node + 0xC);
        operator_delete((int*)node);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnim);
#ifdef SKIP_ASM
extern void** cAnimDataBase_TheAnimDataBase;

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
extern "C" void cAnimSequence_setSubAnim(void* self, int i, int anim)
{
    sSeq_00313C50* sequence = (sSeq_00313C50*)self;
    (sequence->e + i)->anim = anim;
    char* clip = *(char**)((char*)cAnimDataBase_TheAnimDataBase + ((anim & 0xFF) << 2));
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
 * Sets the current time of sub-animation slot @p index and sets the flag at +0xC4.
 *
 * The slots are 0x1C bytes from +0x4 (see cAnimSequence_setSubAnim); the time is at +0x4 of the slot. The +0xC4 flag
 * is also set by cAnimSequence_setSubAnim when it activates a slot. cAnimSequencer_playAndHold and playAndLoop use it
 * for the start time; riders use it to jump to a frame.
 *
 * @param index  the sub-animation slot
 * @param val  the time in seconds
 */
extern "C" void cAnimSequence_setSubAnimTime(void* self, int index, float val)
{
    char* slot = (char*)self + index * 0x1c;
    *(float*)(slot + 0x8) = val;
    *(int*)((char*)self + 0xc4) = 1;
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
extern "C" void cAnimSequence_setSubAnimTimeScale(int sequence, int index, float fparg0) {
    (*(float *)((char*)(((index * 0x1C) + sequence)) + (0xC))) = fparg0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimWeight);
#ifdef SKIP_ASM
/**
 * Sets the blend weight of sub-animation slot @p index.
 *
 * Stores @p val at +0x10 of the 0x1C-byte slot (+0x14 + index * 0x1C); cAnimSequence_getNetWeight multiplies it into the slot's net
 * weight. seg_1218 cross-fades two slots with it (w and 1 - w).
 *
 * @param index  the sub-animation slot
 * @param val  the weight, normally 0-1
 */
extern "C" void cAnimSequence_setSubAnimWeight(void* self, int index, float val)
{
    char* slot = (char*)self + index * 0x1c;
    *(float*)(slot + 0x14) = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_setSubAnimType);
#ifdef SKIP_ASM
/**
 * Sets the play type of sub-animation slot @p index: 0 plays once and holds the last frame, 1 loops.
 *
 * Stores @p val at +0x18 of the 0x1C-byte slot (+0x1C + index * 0x1C). cAnimSequencer_playAndHold passes 0 and
 * cAnimSequencer_playAndLoop 1.
 *
 * @param index  the sub-animation slot
 * @param val  the play type (0 hold, 1 loop)
 */
extern "C" void cAnimSequence_setSubAnimType(void* self, int index, int val)
{
    char* slot = (char*)self + index * 0x1c;
    *(int*)(slot + 0x1c) = val;
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
extern "C" void cAnimSequence_resetSubAnim(int sequence, int index) {
    (*(int *)((char*)(((index * 0x1C) + sequence)) + (0x18))) = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_getNetWeight);
#ifdef SKIP_ASM
struct sSeqTrack_00313D70
{
    char pad0[0x8];
    float time;   // 0x8
    char pad1[0x4];
    float end;    // 0x10
    float weight; // 0x14
    int flag;     // 0x18
};

struct sSeq_00313D70
{
    sSeqTrack_00313D70 tracks[5]; // 0x0
    char pad0[0x8];
    float scale;   // 0x94
    char pad1[0xC];
    float fadeOut; // 0xA4
    float fadeIn;  // 0xA8
};

/**
 * Returns the effective blend weight of sub-animation slot @p i, with the sequence's start and end fades applied.
 *
 * The weight is the sequence's weight (+0x94) times the slot's weight (+0x10 of the slot). While the slot's time is
 * below the start fade time (+0xA8) it is limited to time / start fade; while the time left (the slot's length at +0xC
 * minus its time) is below the end fade time (+0xA4) it is limited to time left / end fade.
 *
 * @param i  the sub-animation slot
 * @return the net weight
 */
// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" float cAnimSequence_getNetWeight(sSeq_00313D70* self, int i)
{
    float time = self->tracks[i].time;
    float w = self->scale * self->tracks[i].weight;
    float end = self->tracks[i].end;
    if (time < self->fadeIn)
    {
        w = (time / self->fadeIn) <? w;
    }
    float rem = end - time;
    if (rem < self->fadeOut)
    {
        w = (rem / self->fadeOut) <? w;
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
    e->fn(s + e->delta, p, n);
}

class cAsStream;

/**
 * Writes an animation sequence to @p stream: its fields, then the number of triggers and each trigger.
 *
 * Each write goes through the stream's first virtual (vtable slot 1: data, size). Fields written: +0x0 (id, 4),
 * +0x4 (the three sub-animation slots, 84), +0x60 (0x20), +0x80, +0x84, +0x88 (8), +0x90 .. +0xA8 (4 each), +0xB0
 * (0x10), +0xC0 and +0xC4. Then the count of the trigger list at +0xAC and each 16-byte trigger node, next pointer
 * included. cAnimSequence_readFromReplayFrame reads the same format back.
 *
 * @param node  the sequence to write
 * @param stream  the output stream
 */
extern "C" void cAnimSequence_writeToReplayFrame(void* node, cAsStream* stream)
{
    char* self = (char*)node;
    char* streamPtr = (char*)stream;
    Ser_313DD0(streamPtr, self + 0x0, 0x4);
    Ser_313DD0(streamPtr, self + 0x4, 84);
    Ser_313DD0(streamPtr, self + 0x60, 0x20);
    Ser_313DD0(streamPtr, self + 0x80, 0x4);
    Ser_313DD0(streamPtr, self + 0x84, 0x4);
    Ser_313DD0(streamPtr, self + 0x88, 0x8);
    Ser_313DD0(streamPtr, self + 0x90, 0x4);
    Ser_313DD0(streamPtr, self + 0x94, 0x4);
    Ser_313DD0(streamPtr, self + 0x98, 0x4);
    Ser_313DD0(streamPtr, self + 0x9C, 0x4);
    Ser_313DD0(streamPtr, self + 0xA0, 0x4);
    Ser_313DD0(streamPtr, self + 0xA4, 0x4);
    Ser_313DD0(streamPtr, self + 0xA8, 0x4);
    Ser_313DD0(streamPtr, self + 0xB0, 0x10);
    Ser_313DD0(streamPtr, self + 0xC0, 0x4);
    Ser_313DD0(streamPtr, self + 0xC4, 0x4);
    int n = 0;
    char* trigger = *(char**)(self + 0xAC);
    if (trigger != 0) {
        int count = 0;
        do {
            trigger = *(char**)(trigger + 0xC);
            count++;
        } while (trigger != 0);
        n = count;
    }
    Ser_313DD0(streamPtr, &n, 4);
    for (trigger = *(char**)(self + 0xAC); trigger; trigger = *(char**)(trigger + 0xC)) {
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
    e->fn(s + e->delta, p, n);
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
    char* self = (char*)node;
    char* streamPtr = (char*)stream;
    Read_314050(streamPtr, self + 0x0, 0x4);
    Read_314050(streamPtr, self + 0x4, 84);
    Read_314050(streamPtr, self + 0x60, 0x20);
    Read_314050(streamPtr, self + 0x80, 0x4);
    Read_314050(streamPtr, self + 0x84, 0x4);
    Read_314050(streamPtr, self + 0x88, 0x8);
    Read_314050(streamPtr, self + 0x90, 0x4);
    Read_314050(streamPtr, self + 0x94, 0x4);
    Read_314050(streamPtr, self + 0x98, 0x4);
    Read_314050(streamPtr, self + 0x9C, 0x4);
    Read_314050(streamPtr, self + 0xA0, 0x4);
    Read_314050(streamPtr, self + 0xA4, 0x4);
    Read_314050(streamPtr, self + 0xA8, 0x4);
    Read_314050(streamPtr, self + 0xB0, 0x10);
    Read_314050(streamPtr, self + 0xC0, 0x4);
    Read_314050(streamPtr, self + 0xC4, 0x4);
    int n;
    Read_314050(streamPtr, &n, 4);
    char* tail = *(char**)(self + 0xAC);
    for (int i = 0; i < n; i++) {
        char* trigger = (char*)cMemMan_alloc(0x10, str_AnimTrigger, 0x20000000, 0);
        Read_314050(streamPtr, trigger, 0x10);
        if (*(char**)(self + 0xAC) != 0) {
            *(char**)(tail + 0xC) = trigger;
        } else {
            *(char**)(self + 0xAC) = trigger;
        }
        tail = trigger;
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
void cAnimSequencer_init(void* self)
{
    *(int*)self = 0;
    *(void**)((char*)self + 0x4) = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_newSequence);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_AnimSequence[];

struct sSeqNode_003142F8 {
    char pad_0x00[0xC8];
    sSeqNode_003142F8* next; // 0xC8
};

struct sSequencer_003142F8 {
    int mCount;                        // 0x0
    sSeqNode_003142F8* mFirstSequence; // 0x4
};

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
extern "C" void* cAnimSequencer_newSequence(void* seq, int sequenceID)
{
    sSequencer_003142F8* self = (sSequencer_003142F8*)seq;
    sSeqNode_003142F8* node = (sSeqNode_003142F8*)cAnimSequence_cAnimSequence(cMemMan_alloc(208, str_AnimSequence, 0x20000000, 0), sequenceID);
    node->next = self->mFirstSequence;
    self->mFirstSequence = node;
    self->mCount++;
    return node;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_appendSequence);
#ifdef SKIP_ASM
// Same layout as the unit's cAnimSequenceNode / cAnimSequencer (declared later in the unit).
struct sSeqNode_00314368 {
    char pad_0x00[0xC8];
    sSeqNode_00314368* next; // 0xC8
};

struct sSequencer_00314368 {
    int mCount;                      // 0x0
    sSeqNode_00314368* mFirstSequence; // 0x4
};

/**
 * Appends @p node at the end of the sequencer's list (+0x4, linked through +0xC8) and increments the count (+0x0).
 *
 * Clears the node's next pointer. cAnimSequencer_newSequence links new sequences at the head instead; cAnimSequencer_readFromReplayFrame
 * uses this to keep the saved order.
 *
 * @param node  the sequence to append
 */
extern "C" void cAnimSequencer_appendSequence(sSequencer_00314368* self, sSeqNode_00314368* node)
{
    if (self->mFirstSequence != 0)
    {
        sSeqNode_00314368* tail = self->mFirstSequence;
        while (tail->next != 0)
        {
            tail = tail->next;
        }
        tail->next = node;
    }
    else
    {
        self->mFirstSequence = node;
    }
    node->next = 0;
    self->mCount++;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_removeSequence);
#ifdef SKIP_ASM
// Same layout as the unit's cAnimSequenceNode / cAnimSequencer (declared later in the unit).
struct sSeqNode_003143C8 {
    char pad_0x00[0xC8];
    sSeqNode_003143C8* next; // 0xC8
};

struct sSequencer_003143C8 {
    int mCount;                        // 0x0
    sSeqNode_003143C8* mFirstSequence; // 0x4
};

/**
 * Unlinks @p node from the sequencer's list (+0x4, linked through +0xC8) and decrements the count (+0x0).
 *
 * The node is not freed; its next pointer is cleared. Likely bug: a node that is not in the list ends the search with a
 * null predecessor, which is then written through.
 *
 * @param node  the sequence to unlink
 */
extern "C" void cAnimSequencer_removeSequence(sSequencer_003143C8* self, sSeqNode_003143C8* node)
{
    if (self->mFirstSequence == node) {
        self->mFirstSequence = node->next;
    } else {
        sSeqNode_003143C8* cur = self->mFirstSequence;
        while (cur != 0) {
            if (cur->next == node) {
                break;
            }
            cur = cur->next;
        }
        cur->next = node->next;
    }
    node->next = 0;
    self->mCount--;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_getAnimBlendFrames);
#ifdef SKIP_ASM
struct sSeq_00314418;
extern "C" float cAnimSequence_getNetWeight_314418(sSeq_00314418* self, int i) __asm__("cAnimSequence_getNetWeight");

struct sVec4_314418
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSeqTrack_314418
{
    char pad0[0x4];
    int id;         // 0x4
    float time;     // 0x8
    char padC[0xC];
    int flag;       // 0x18
};

struct sSeq_00314418
{
    sSeqTrack_314418 tracks[3];     // 0x00
    char pad54[0xC];
    sVec4_314418 a;                 // 0x60
    sVec4_314418 b;                 // 0x70
    int c;                          // 0x80
    int d;                          // 0x84
    long e;                         // 0x88
    char pad90[0x38];
    sSeq_00314418* next;            // 0xC8
};

struct sSeqState_314418
{
    int id;             // 0x00
    float time;         // 0x04
    int d;              // 0x08
    float weight;       // 0x0C
    long e;             // 0x10
    char pad18[0x8];
    sVec4_314418 a;     // 0x20
    sVec4_314418 b;     // 0x30
    int c;              // 0x40
    char pad44[0xC];
};

/**
 * Collects the weighted animation frames of all playing sequences for blending.
 *
 * For each sequence (list at +0x4) and each of its 3 sub-animations with the active flag, writes a 0x50-byte frame to
 * @p out: anim id, time, the two orientation vectors (+0x60, +0x70), +0x80, +0x84, +0x88 and the net weight from
 * cAnimSequence_getNetWeight. Unlike On Tour there is no bound check on @p out.
 *
 * @param out  receives the frames (On Tour tAnimBlendFrame *frames); must be large enough
 * @return the number of frames written
 */
// PORT: `long` field is 64-bit (ld/sd).
extern "C" int cAnimSequencer_getAnimBlendFrames(void* self, sSeqState_314418* out)
{
    int count = 0;
    sSeq_00314418* n = *(sSeq_00314418**)((char*)self + 0x4);
    while (n != 0)
    {
        for (int j = 0; j < 3; j++)
        {
            if (n->tracks[j].flag != 0)
            {
                sSeqState_314418* blendFrame = &out[count];
                blendFrame->id = n->tracks[j].id;
                blendFrame->time = n->tracks[j].time;
                blendFrame->a = n->a;
                blendFrame->b = n->b;
                blendFrame->c = n->c;
                blendFrame->d = n->d;
                blendFrame->weight = cAnimSequence_getNetWeight_314418(n, j);
                blendFrame->e = n->e;
                count++;
            }
        }
        n = n->next;
    }
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_playAndHold);
#ifdef SKIP_ASM
extern "C" void* cAnimSequencer_newSequence(void* seq, int arg);
extern "C" void cAnimSequence_setSubAnim(void* self, int i, int anim);

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
extern "C" void* cAnimSequencer_playAndHold(void* self, int sequenceID, int anim, float val)
{
    void* node = cAnimSequencer_newSequence(self, sequenceID);
    cAnimSequence_setSubAnim(node, 0, anim);
    cAnimSequence_setSubAnimTime(node, 0, val);
    cAnimSequence_setSubAnimType(node, 0, 0);
    return node;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_playAndLoop);
#ifdef SKIP_ASM
extern "C" void* cAnimSequencer_newSequence(void* seq, int arg);
extern "C" void cAnimSequence_setSubAnim(void* self, int i, int anim);

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
extern "C" void* cAnimSequencer_playAndLoop(void* self, int sequenceID, int anim, float val)
{
    void* node = cAnimSequencer_newSequence(self, sequenceID);
    cAnimSequence_setSubAnim(node, 0, anim);
    cAnimSequence_setSubAnimTime(node, 0, val);
    cAnimSequence_setSubAnimType(node, 0, 1);
    return node;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_expire);
#ifdef SKIP_ASM
struct sSeqNode_003145F8 {
    char pad_0x00[0xC8];
    sSeqNode_003145F8* next; // 0xC8
};

struct sSequencer_003145F8 {
    int mCount;                        // 0x0
    sSeqNode_003145F8* mFirstSequence; // 0x4
};

/**
 * Removes a sequence from the sequencer and deletes it.
 *
 * Finds @p node in the list (+0x4, next at +0xC8), unlinks it, frees its triggers (cAnimSequence_removeAllTriggers), deletes it and
 * decrements the count. A sequence not in the list is left alone (On Tour asserts).
 *
 * @param node  the sequence to remove (On Tour seq)
 */
extern "C" void cAnimSequencer_expire(sSequencer_003145F8* self, sSeqNode_003145F8* node)
{
    sSeqNode_003145F8** pp;
    for (pp = &self->mFirstSequence; *pp != 0; pp = &(*pp)->next)
    {
        sSeqNode_003145F8* n = *pp;
        if (n == node)
        {
            *pp = n->next;
            if (n != 0)
            {
                cAnimSequence_removeAllTriggers((char*)n);
                operator_delete((int*)n);
            }
            self->mCount--;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_expireAll);
#ifdef SKIP_ASM
struct sSeqNode_00314668 {
    char pad_0x00[0xC8];
    sSeqNode_00314668* next; // 0xC8
};

struct sSequencer_00314668 {
    int mCount;                        // 0x0
    sSeqNode_00314668* mFirstSequence; // 0x4
};

/**
 * Removes and deletes every sequence in the animation sequencer.
 *
 * Each sequence is unlinked from the list head, has its triggers removed (cAnimSequence_removeAllTriggers, On Tour
 * cAnimSequence::removeAllTriggers) and is freed with operator delete; the count drops by one for each.
 */
extern "C" void cAnimSequencer_expireAll(sSequencer_00314668* self)
{
    while (self->mFirstSequence != 0)
    {
        sSeqNode_00314668* n = self->mFirstSequence;
        self->mFirstSequence = n->next;
        if (n != 0)
        {
            cAnimSequence_removeAllTriggers((char*)n);
            operator_delete((int*)n);
        }
        self->mCount--;
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_fadeOutAndExpireAll);
#ifdef SKIP_ASM
/**
 * Fades out every sequence of the sequencer and marks them all to expire.
 *
 * cAnimSequence_fadeOutAndExpire on each sequence of the list at +0x4.
 *
 * @param time  the fade-out time in seconds (On Tour time)
 */
extern "C" void cAnimSequencer_fadeOutAndExpireAll(void* self, float time)
{
    for (char* node = *(char**)((char*)self + 0x4); node != 0; node = *(char**)(node + 0xC8))
    {
        cAnimSequence_fadeOutAndExpire(node, time);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", cAnimSequencer_removeAllEndTriggers);
#ifdef SKIP_ASM
extern "C" void cAnimSequence_removeEndTrigger(void* self, int v);

/**
 * Removes the given end trigger from every sequence of the sequencer.
 *
 * cAnimSequence_removeEndTrigger on each sequence of the list at +0x4.
 *
 * @param trig  the end trigger's event id (On Tour trig)
 */
extern "C" void cAnimSequencer_removeAllEndTriggers(void* self, int trig)
{
    for (char* node = *(char**)((char*)self + 0x4); node != 0; node = *(char**)(node + 0xC8))
    {
        cAnimSequence_removeEndTrigger(node, trig);
    }
}
#endif

struct cAnimSequenceNode {
    char pad_0x00[0xC8];
    cAnimSequenceNode* next; // 0xC8
};

struct cAnimSequencer {
    char pad_0x00[0x4];
    cAnimSequenceNode* mFirstSequence; // 0x4
};

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
cAnimSequenceNode* cAnimSequencer_getSequence(cAnimSequencer* self, int index)
{
    cAnimSequenceNode* cur = self->mFirstSequence;
    while (cur != 0 && index-- > 0) {
        cur = cur->next;
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
extern "C" int cAnimSequencer_isPlaying(cAnimSequencer *self, int id) {
    for (cAnimSequenceNode *n = self->mFirstSequence; n != 0; n = n->next) {
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
extern "C" int cAnimSequencer_getPosition(cAnimSequencer *self, int id) {
    int index = 0;
    for (cAnimSequenceNode *n = self->mFirstSequence; n != 0; n = n->next) {
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

struct sSeqNode_003147F0 {
    char pad_0x00[0xC8];
    sSeqNode_003147F0* next; // 0xC8
};

struct sSequencer_003147F0 {
    int mCount;                        // 0x0
    sSeqNode_003147F0* mFirstSequence; // 0x4
};

extern "C" void cAnimSequence_writeToReplayFrame(void* node, cAsStream* s);

/**
 * Writes the sequencer to @p stream: its 4-byte count, then each of its sequences (cAnimSequence_writeToReplayFrame) in list order.
 *
 * The count goes through the stream's first virtual (vtable slot 1). func_00104D68 writes a rider's six sequencers
 * with it; cAnimSequencer_readFromReplayFrame reads the format back.
 *
 * @param stream  the output stream
 */
extern "C" void cAnimSequencer_writeToReplayFrame(sSequencer_003147F0* self, cAsStream* stream)
{
    stream->serialize(self, 4);
    sSeqNode_003147F0* n = self->mFirstSequence;
    for (int i = 0; i < self->mCount; i++)
    {
        cAnimSequence_writeToReplayFrame(n, stream);
        n = n->next;
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

struct sStreamVE_314880 { short delta; short index; void (*fn)(void*, void*, int); };

struct sSeqTrack_314880
{
    char pad[0x1C];
    sSeqTrack_314880() {}
};

struct sSeqNode_314880
{
    sSeqTrack_314880 tracks[3];
    char pad54[0xD0 - 0x54];
    sSeqNode_314880() {}
};

/**
 * Reads a sequencer from @p stream in the format cAnimSequencer_writeToReplayFrame writes, recreating each sequence.
 *
 * Reads the count through the stream's second virtual (vtable slot 2) and zeroes the sequencer's count (+0x0), then for
 * each sequence allocates 0xD0 bytes tagged "AnimSequence" (flags 0x20000000), zero-fills them (MEM_fill), reads the
 * sequence (cAnimSequence_readFromReplayFrame) and appends it (cAnimSequencer_appendSequence). The list head (+0x4) is not cleared here: func_00104DE0
 * empties the sequencer with cAnimSequencer_expireAll first.
 *
 * @param stream  the input stream
 */
extern "C" void cAnimSequencer_readFromReplayFrame(void* self, void* stream)
{
    int n;
    sStreamVE_314880* vt = *(sStreamVE_314880**)stream;
    vt[2].fn((char*)stream + vt[2].delta, &n, 4);
    *(int*)self = 0;
    for (int i = 0; i < n; i++)
    {
        sSeqNode_314880* node = new (str_AnimSequence, 0x20000000, 0) sSeqNode_314880;
        MEM_fill(node, 0, 208);
        cAnimSequence_readFromReplayFrame(node, stream);
        cAnimSequencer_appendSequence((sSequencer_00314368*)self, (sSeqNode_00314368*)node);
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
 * Points the wrapper at archive @p data: header (+0x4), part table (+0x8) and packed data base (+0xC).
 *
 * The part table starts at @p data plus the short at +0x6 of the header; the data base at @p data plus the int at
 * +0x8. Called by cMdfArchive_attachArchive, cMdfArchive_attachConstArchive and cMdfArchive_loadArchive after they set the owned pointer.
 *
 * @param data  the MDF archive header
 */
void cMdfArchive_linkArchive_impl(void* self, char* data)
{
    *(char**)((char*)self + 0x4) = data;
    *(char**)((char*)self + 0x8) = data + *(short*)(data + 0x6);
    *(char**)((char*)self + 0xC) = data + *(int*)(data + 0x8);
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
    char *header = (char*)self->ArchiveHeader;
    if (header != 0) {
        return *(short *)(header + 4);
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
    for (i = 0; i < *(short*)((char*)self->ArchiveHeader + 4); i++) {
        if (strcasecmp(name, (char*)self->ModelHeaders + i * 0x60) == 0)
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
    for (i = 0; i < *(short*)((char*)self->ArchiveHeader + 4); i++) {
        if (strcasecmp(name, (char*)self->ModelHeaders + i * 0x60) == 0)
            return (char*)self->ModelHeaders + i * 0x60;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", cMdfArchive_getModelPart);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" void* cMdfArchive_getModelPartByIndex(void* self, int index);

struct sMdfPart60 {
    char name[0x60];
};

struct sMdfHeader {
    int pad0;
    short count; // 0x4
};

struct sMdfArchive {
    int pad0;
    sMdfHeader* header; // 0x4
    sMdfPart60* parts;  // 0x8
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
extern "C" void* cMdfArchive_getModelPart(sMdfArchive* self, const char* name)
{
    for (int i = 0; i < self->header->count; i++)
    {
        if (strcasecmp(name, self->parts[i].name) == 0)
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
