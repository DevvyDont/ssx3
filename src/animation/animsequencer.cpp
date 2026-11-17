#include "common.h"

struct cAnimSequence {
    char pad_0x00[0x98];
    float mCurrentWeight; // 0x98
    float mTargetWeight; // 0x9C
    float mFadeSpeed; // 0xA0
};

//100%
INCLUDE_ASM("animation/animsequencer", cAnimSequence_fadeWeight__FP13cAnimSequenceff);
#ifdef SKIP_ASM
/**
 * Sets the sequence's current and target blend weights and clears its fade speed.
 *
 * Despite the name, no fade rate is set: mFadeSpeed is reset to 0.
 *
 * @param weight  value stored in mCurrentWeight
 * @param target  value stored in mTargetWeight
 */
void cAnimSequence_fadeWeight(cAnimSequence* self, float weight, float target)
{
    self->mCurrentWeight = weight;
    self->mTargetWeight = target;
    self->mFadeSpeed = 0.0f;
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
INCLUDE_ASM("animation/animsequencer", func_00313B38);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
struct sNode_00313B38 {
    int id;
    int refs;
    float time;
    sNode_00313B38* next;
};

extern "C" void func_00313B38(char* self, int id, float time) {
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
INCLUDE_ASM("animation/animsequencer", func_00313CF0);
#ifdef SKIP_ASM
extern "C" void func_00313CF0(void* self, int index, float val)
{
    char* slot = (char*)self + index * 0x1c;
    *(float*)(slot + 0x8) = val;
    *(int*)((char*)self + 0xc4) = 1;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D10);
#ifdef SKIP_ASM
extern "C" void func_00313D10(int sequence, int index, float fparg0) {
    (*(float *)((char*)(((index * 0x1C) + sequence)) + (0xC))) = fparg0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D28);
#ifdef SKIP_ASM
extern "C" void func_00313D28(void* self, int index, float val)
{
    char* slot = (char*)self + index * 0x1c;
    *(float*)(slot + 0x14) = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D40);
#ifdef SKIP_ASM
extern "C" void func_00313D40(void* self, int index, int val)
{
    char* slot = (char*)self + index * 0x1c;
    *(int*)(slot + 0x1c) = val;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D58);
#ifdef SKIP_ASM
extern "C" void func_00313D58(int sequence, int index) {
    (*(int *)((char*)(((index * 0x1C) + sequence)) + (0x18))) = 0;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00313D70);
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

// PORT: g++ <? (min) operator, removed in GCC 4.3.
extern "C" float func_00313D70(sSeq_00313D70* self, int i)
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
INCLUDE_ASM("animation/animsequencer", func_00313DD0);
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

extern "C" void func_00313DD0(void* node, cAsStream* stream)
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
INCLUDE_ASM("animation/animsequencer", func_00314050);
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

extern "C" void func_00314050(void* node, void* stream)
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

extern "C" void* func_00313508(void* mem, int arg);

/**
 * Creates a new animation sequence and adds it to the sequencer.
 *
 * Allocates 0xD0 bytes tagged 'AnimSequence', constructs it with func_00313508(@p sequenceID), links it at the head of the
 * list (+0x4, next at +0xC8) and increments the count (+0x0).
 *
 * @param seq  the sequencer (this)
 * @param sequenceID  the new sequence's id (On Tour sequenceID)
 * @return the new sequence
 */
extern "C" void* cAnimSequencer_newSequence(void* seq, int sequenceID)
{
    sSequencer_003142F8* self = (sSequencer_003142F8*)seq;
    sSeqNode_003142F8* node = (sSeqNode_003142F8*)func_00313508(cMemMan_alloc(208, str_AnimSequence, 0x20000000, 0), sequenceID);
    node->next = self->mFirstSequence;
    self->mFirstSequence = node;
    self->mCount++;
    return node;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314368);
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

extern "C" void func_00314368(sSequencer_00314368* self, sSeqNode_00314368* node)
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
INCLUDE_ASM("animation/animsequencer", func_003143C8);
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

extern "C" void func_003143C8(sSequencer_003143C8* self, sSeqNode_003143C8* node)
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
extern "C" float func_00313D70_314418(sSeq_00314418* self, int i) __asm__("func_00313D70");

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
 * func_00313D70. Unlike On Tour there is no bound check on @p out.
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
                blendFrame->weight = func_00313D70_314418(n, j);
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
 * cAnimSequencer_newSequence (newSequence), then sub-animation 0 gets @p anim (cAnimSequence_setSubAnim), time @p val (func_00313CF0) and type
 * 0, hold (func_00313D40).
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
    func_00313CF0(node, 0, val);
    func_00313D40(node, 0, 0);
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
 * cAnimSequencer_newSequence (newSequence), then sub-animation 0 gets @p anim (cAnimSequence_setSubAnim), time @p val (func_00313CF0) and type
 * 1, loop (func_00313D40).
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
    func_00313CF0(node, 0, val);
    func_00313D40(node, 0, 1);
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
 * cAnimSequence_fadeOutAndExpire (fadeOutAndExpire) on each sequence of the list at +0x4.
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
 * cAnimSequence_removeEndTrigger (removeEndTrigger) on each sequence of the list at +0x4.
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
INCLUDE_ASM("animation/animsequencer", func_003147F0);
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

extern "C" void func_00313DD0(void* node, cAsStream* s);

extern "C" void func_003147F0(sSequencer_003147F0* self, cAsStream* stream)
{
    stream->serialize(self, 4);
    sSeqNode_003147F0* n = self->mFirstSequence;
    for (int i = 0; i < self->mCount; i++)
    {
        func_00313DD0(n, stream);
        n = n->next;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("animation/animsequencer", func_00314880);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void func_00314050(void* node, void* stream);
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

extern "C" void func_00314880(void* self, void* stream)
{
    int n;
    sStreamVE_314880* vt = *(sStreamVE_314880**)stream;
    vt[2].fn((char*)stream + vt[2].delta, &n, 4);
    *(int*)self = 0;
    for (int i = 0; i < n; i++)
    {
        sSeqNode_314880* node = new (str_AnimSequence, 0x20000000, 0) sSeqNode_314880;
        MEM_fill(node, 0, 208);
        func_00314050(node, stream);
        func_00314368((sSequencer_00314368*)self, (sSeqNode_00314368*)node);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314978__FPv);
#ifdef SKIP_ASM
void* func_00314978(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314988);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_003149D0(void** self);

extern "C" void func_00314988(int* self, int flags)
{
    func_003149D0((void**)self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_003149D0);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);

extern "C" void func_003149D0(void** self)
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

extern "C" void* func_00314AA0(void*);

//100%
INCLUDE_ASM("animation/animsequencer", func_00314A18__FPvi);
#ifdef SKIP_ASM
// PORT: asm label (same binding the unit uses for func_00314AA0's two-argument body).
void func_00314AA0_impl(void* self, char* data) __asm__("func_00314AA0");

void func_00314A18(void* self, int data)
{
    *(int*)self = data;
    func_00314AA0_impl(self, (char*)data);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314A40__FPv);
#ifdef SKIP_ASM
void* func_00314A40(void* self)
{
    *(int*)self = 0;
    return func_00314AA0(self);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314A60);
#ifdef SKIP_ASM
// PORT: asm label (same two-argument binding func_00314A18 uses for func_00314AA0).
void func_00314AA0_impl(void* self, char* data) __asm__("func_00314AA0");
extern "C" char *FILE_loadpack(int, int);

extern "C" void func_00314A60(void *self, int name, int flags) {
    char *data = FILE_loadpack(name, flags);
    *(char **)self = data;
    func_00314AA0_impl(self, data);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314AA0);
#ifdef SKIP_ASM
// The original had the same mismatch: callers (func_00314A18/func_00314A40, the
// latter already 100%) see the one-argument declaration above and leave $5 as-is,
// while the definition takes two. The asm label binds this two-argument body to
// the symbol without changing the callers' prototype.
// PORT: asm labels are GCC/Clang-only; the port should give callers the real signature.
void func_00314AA0_impl(void* self, char* data) __asm__("func_00314AA0");
void func_00314AA0_impl(void* self, char* data)
{
    *(char**)((char*)self + 0x4) = data;
    *(char**)((char*)self + 0x8) = data + *(short*)(data + 0x6);
    *(char**)((char*)self + 0xC) = data + *(int*)(data + 0x8);
}
#endif

//100%
INCLUDE_ASM("animation/animsequencer", func_00314AC0);
#ifdef SKIP_ASM
extern "C" short func_00314AC0(void *self) {
    char *header = *(char **)((char *)self + 4);
    if (header != 0) {
        return *(short *)(header + 4);
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("animation/animsequencer", func_00314AE0);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);

extern "C" int func_00314AE0(char* self, const char* name) {
    int i;
    for (i = 0; i < *(short*)(*(char**)(self + 4) + 4); i++) {
        if (strcasecmp(name, *(char**)(self + 8) + i * 0x60) == 0)
            return 1;
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("animation/animsequencer", func_00314B70);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);

extern "C" char* func_00314B70(char* self, const char* name) {
    int i;
    for (i = 0; i < *(short*)(*(char**)(self + 4) + 4); i++) {
        if (strcasecmp(name, *(char**)(self + 8) + i * 0x60) == 0)
            return *(char**)(self + 8) + i * 0x60;
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
INCLUDE_ASM("animation/animsequencer", func_00314C98);
#ifdef SKIP_ASM
extern "C" int func_00314C98(void *archive, int index) {
    return (*(int *)((char*)(archive) + (8))) + (index * 0x60);
}
#endif
