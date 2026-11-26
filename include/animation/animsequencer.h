#ifndef ANIMATION_ANIMSEQUENCER_H
#define ANIMATION_ANIMSEQUENCER_H

// cAnimSequencer (at the end of this file): one animation channel of a rider's cRiderAnimBase, a counted singly linked
// list of the cAnimSequences playing on it (newest first; newSequence pushes at the head, appendSequence at the tail).
//
// cAnimSequence: one playing animation sequence of a cAnimSequencer: up to three blended sub-animations, an
// orientation offset, flip / priority / bone mask, weight fading, time triggers and the event flags they raise.
// cAnimSequencer (8 bytes: NumPlaying, PlayList) keeps its sequences in a list linked through Next;
// cAnimSequencer_newSequence allocates each one: cAnimSequence_cAnimSequence(cMemMan_alloc(0xD0, "AnimSequence",
// 0x20000000, 0), sequenceID).
//
// Size 0xD0: [R] that allocation; the last member is Next (0xC8, cAnimSequencer_appendSequence / expire / expireAll),
// then tail padding to the 16-byte alignment of OrientationOffset (tPose). On Tour's size is also 0xD0. No base
// class, no virtual functions. EA's source: d:/ssx/code/source/animation/animsequencer.cpp, header
// ../source/animation/animsequencer.h (preview strings). Retail methods: unit animation/rideranimbase 0x00313508-
// 0x003139A8 (ctor, updateSubAnim, updateWeight, checkTriggers, checkTriggersAtTime, setEndTriggers) and unit
// animation/animsequencer 0x00313A10-0x00314050.
//
// A FLAT layout, like main/game.h. Member names, order and types are EA's, from SSX On Tour's PSP DWARF ([O] class
// cAnimSequence, tSubAnim, tTrigger, tEventFlags). Retail = On Tour up to EndEvent (0xC0); On Tour's two bools packed
// at 0xC0-0xC1 are words in retail, so CheckTriggersAtCurrentFrame is 0xC4 (On Tour 0xC1) and Next 0xC8 (On Tour
// 0xC4). [P] the July 2003 preview's cAnimSequence::tEventFlags::isSet / isJustChanged / clear read IsSet at +0x0 and
// IsJustChanged at +0x8 with ld/sd (tools/layout_xlate.py: same offsets in retail).
// Enums and bools are words. The 64-bit masks are `unsigned long` (64-bit on the EE, as in animation/rideranimbase.h;
// On Tour: unsigned long long). PORT: use a 64-bit type on other targets.

#include "layout.h"
#include "bx/math3d.h"

// One sub-animation of a sequence (On Tour struct tSubAnim, 0x1C). cAnimSequence_setSubAnim and the other indexed
// setters address self + 4 + sub * 0x1C.
struct tSubAnim {
    unsigned int AnimID;    // 0x00 [O] tAnimID {SourceID : 8, AnimID : 24}; [U] setSubAnim stores its animID argument
                            //      and splits it (low byte: source, >> 8: anim) to look the anim up in the database
    float Time;             // 0x04 [O]; [U] setSubAnimTime stores it; setSubAnim 0
    float TimeScale;        // 0x08 [O]; [U] setSubAnimTimeScale stores it; setSubAnim 1.0
    float TotalTime;        // 0x0C [O]; [U] setSubAnim: (frames - 1) / 30
    float Weight;           // 0x10 [O]; [U] setSubAnimWeight stores it; setSubAnim 1.0; getNetWeight reads it
    int IsActive;           // 0x14 [O] bool; [R] the ctor clears it for all 3; [U] setSubAnim sets 1,
                            //      resetSubAnim 0
    int Type;               // 0x18 [O] eSequenceType; [U] setSubAnimType stores it; setSubAnim 0
#ifdef __cplusplus
    // [O] On Tour's tSubAnim lists a default constructor (its AnimID is a tAnimID, whose empty constructor is
    // user-declared); [R] cAnimSequence_cAnimSequence and cAnimSequencer_readFromReplayFrame's `new cAnimSequence`
    // still run the dead array-constructor loop over SubAnims (li 2, li -1), which only a non-POD element type
    // produces.
    tSubAnim() {}
#endif
};

CHECK_OFFSET(tSubAnim, AnimID, 0x00);
CHECK_OFFSET(tSubAnim, Time, 0x04);
CHECK_OFFSET(tSubAnim, TimeScale, 0x08);
CHECK_OFFSET(tSubAnim, TotalTime, 0x0C);
CHECK_OFFSET(tSubAnim, Weight, 0x10);
CHECK_OFFSET(tSubAnim, IsActive, 0x14);
CHECK_OFFSET(tSubAnim, Type, 0x18);
CHECK_SIZE(tSubAnim, 0x1C);

// A time or end trigger (On Tour struct tTrigger, 0x10; [R] addTrigger / addEndTrigger: cMemMan_alloc(0x10,
// "AnimTrigger")); a singly linked list from cAnimSequence::Triggers.
struct tTrigger {
    int ID;                 // 0x0 [O]; [U] addTrigger / addEndTrigger store the id, removeTrigger / removeEndTrigger
                            //       match it
    int IsEndTrigger;       // 0x4 [O] bool; [U] addEndTrigger sets 1, addTrigger 0
    float TriggerTime;      // 0x8 [O]; [U] addTrigger stores the time (swc1), addEndTrigger 0
    tTrigger* Next;         // 0xC [O]; [U] addTrigger / addEndTrigger push at the head
};

CHECK_OFFSET(tTrigger, ID, 0x0);
CHECK_OFFSET(tTrigger, IsEndTrigger, 0x4);
CHECK_OFFSET(tTrigger, TriggerTime, 0x8);
CHECK_OFFSET(tTrigger, Next, 0xC);
CHECK_SIZE(tTrigger, 0x10);

// The event bits raised by triggers (On Tour struct tEventFlags, 0x10; EA nests it: the preview mangles
// cAnimSequence::tEventFlags).
struct tEventFlags {
    unsigned long IsSet;            // 0x0 [O]; [P] isSet / clear: ld 0x0
    unsigned long IsJustChanged;    // 0x8 [O]; [P] isJustChanged / clear: ld/sd 0x8

    // Methods bound to their retail symbols by asm labels.
    bool isSet1(int bit) __asm__("tEventFlags_isSet1");
    bool isJustChanged(int bit) __asm__("tEventFlags_isJustChanged");
};

CHECK_OFFSET(tEventFlags, IsSet, 0x0);
CHECK_OFFSET(tEventFlags, IsJustChanged, 0x8);
CHECK_SIZE(tEventFlags, 0x10);

struct cAnimSequence {
    int SequenceID;                 // 0x000 [O]; [R] the ctor stores its sequenceID argument
    tSubAnim SubAnims[3];           // 0x004 [O]; [R] the ctor clears IsActive at 0x18 / 0x34 / 0x50; [U] the indexed
                                    //       setters, updateSubAnim, getNetWeight
    char unk_058[0x8];              // 0x058 untouched: alignment of the 16-byte tPose (On Tour has the same gap)
    tPose OrientationOffset;        // 0x060 [O]; [R] the ctor copies tPose_Identity (sq 0x60, 0x70); [U]
                                    //       cRiderAnimBase_onInput copies the rider's offset here
    int IsFlipped;                  // 0x080 [O] bool; [R] ctor 0; [U] cRiderAnimBase_onInput / changeIsFlipped
    int Priority;                   // 0x084 [O]; [R] ctor 0
    unsigned long Mask;             // 0x088 [O] (unsigned long long); [R] the ctor stores -1 (sd); [U]
                                    //       cRiderAnimBase_onInput copies the grafting mask here
    float TimeScale;                // 0x090 [O]; [R] ctor 1.0; [U] updateSubAnim reads it
    float CurrentWeight;            // 0x094 [O]; [R] ctor 1.0; [U] updateWeight fades it, getNetWeight reads it
    float TargetWeight;             // 0x098 [O]; [R] ctor 1.0; [U] fadeWeight / fadeOutAndExpire store it,
                                    //       updateWeight reads it
    float WeightTimer;              // 0x09C [O]; [R] ctor 0; [U] fadeWeight / fadeOutAndExpire / updateWeight
    int IsFadeOutExpire;            // 0x0A0 [O] bool; [R] ctor 0; [U] fadeOutAndExpire sets it, fadeWeight clears
                                    //       it, updateWeight tests it
    float EndFadeTime;              // 0x0A4 [O]; [R] ctor 0; [U] getNetWeight (lwc1)
    float StartFadeTime;            // 0x0A8 [O]; [R] ctor 0; [U] getNetWeight (lwc1)
    tTrigger* Triggers;             // 0x0AC [O]; [R] ctor 0; [U] add / remove / removeAll / check*Triggers walk it
    tEventFlags Events;             // 0x0B0 [O]; [R] the ctor zeroes both words (sd 0xB0, 0xB8); [U]
                                    //       checkTriggers / checkTriggersAtTime / setEndTriggers set bits (ld/sd)
    int EndEvent;                   // 0x0C0 [O] bool; [R] ctor 0; [U] setEndTriggers / updateSubAnim store it
    int CheckTriggersAtCurrentFrame; // 0x0C4 [O] bool (On Tour 0xC1); [R] ctor 0; [U] setSubAnim / setSubAnimTime
                                    //       set 1, updateSubAnim tests and clears it
    cAnimSequence* Next;            // 0x0C8 [O] (On Tour 0xC4); [U] cAnimSequencer_appendSequence links through it,
                                    //       cAnimSequencer_expire / expireAll walk it
    char unk_0CC[0x4];              // 0x0CC tail padding (16-byte alignment from tPose)

    // Methods bound to their retail symbols by asm labels.
    void fadeWeight(float targetWeight, float time) __asm__("cAnimSequence_fadeWeight__FP13cAnimSequenceff");
    void fadeOutAndExpire(float time) __asm__("cAnimSequence_fadeOutAndExpire");
    void addTrigger(int key, float weight) __asm__("cAnimSequence_addTrigger");
    void addEndTrigger(int key) __asm__("cAnimSequence_addEndTrigger");
    void removeTrigger(int id, float time) __asm__("cAnimSequence_removeTrigger");
    void removeEndTrigger(int key) __asm__("cAnimSequence_removeEndTrigger");
    void removeAllTriggers() __asm__("cAnimSequence_removeAllTriggers");
    void setSubAnim(int i, int anim) __asm__("cAnimSequence_setSubAnim");
    void setSubAnimTime(int index, float val) __asm__("cAnimSequence_setSubAnimTime");
    void setSubAnimTimeScale(int index, float fparg0) __asm__("cAnimSequence_setSubAnimTimeScale");
    void setSubAnimWeight(int index, float val) __asm__("cAnimSequence_setSubAnimWeight");
    void setSubAnimType(int index, int val) __asm__("cAnimSequence_setSubAnimType");
    void resetSubAnim(int index) __asm__("cAnimSequence_resetSubAnim");
    float getNetWeight(int i) __asm__("cAnimSequence_getNetWeight");
    void checkTriggers(float from, float to) __asm__("cAnimSequence_checkTriggers");
    void checkTriggersAtTime(float time) __asm__("cAnimSequence_checkTriggersAtTime");
    void setEndTriggers() __asm__("cAnimSequence_setEndTriggers");
};

CHECK_OFFSET(cAnimSequence, SequenceID, 0x000);
CHECK_OFFSET(cAnimSequence, SubAnims, 0x004);
CHECK_OFFSET(cAnimSequence, unk_058, 0x058);
CHECK_OFFSET(cAnimSequence, OrientationOffset, 0x060);
CHECK_OFFSET(cAnimSequence, IsFlipped, 0x080);
CHECK_OFFSET(cAnimSequence, Priority, 0x084);
CHECK_OFFSET(cAnimSequence, Mask, 0x088);
CHECK_OFFSET(cAnimSequence, TimeScale, 0x090);
CHECK_OFFSET(cAnimSequence, CurrentWeight, 0x094);
CHECK_OFFSET(cAnimSequence, TargetWeight, 0x098);
CHECK_OFFSET(cAnimSequence, WeightTimer, 0x09C);
CHECK_OFFSET(cAnimSequence, IsFadeOutExpire, 0x0A0);
CHECK_OFFSET(cAnimSequence, EndFadeTime, 0x0A4);
CHECK_OFFSET(cAnimSequence, StartFadeTime, 0x0A8);
CHECK_OFFSET(cAnimSequence, Triggers, 0x0AC);
CHECK_OFFSET(cAnimSequence, Events, 0x0B0);
CHECK_OFFSET(cAnimSequence, EndEvent, 0x0C0);
CHECK_OFFSET(cAnimSequence, CheckTriggersAtCurrentFrame, 0x0C4);
CHECK_OFFSET(cAnimSequence, Next, 0x0C8);
CHECK_OFFSET(cAnimSequence, unk_0CC, 0x0CC);
CHECK_SIZE(cAnimSequence, 0xD0);

// One sub-animation's clip and time in a blend frame (On Tour struct tAnimSourceFrame, 0x8). [Staged by views group A.]
struct tAnimSourceFrame {
    unsigned int AnimID;            // 0x0 [O] tAnimID {SourceID : 8, AnimID : 24}; [U] getAnimBlendFrames copies
                                    //       tSubAnim::AnimID
    float Time;                     // 0x4 [O]; [U] getAnimBlendFrames copies tSubAnim::Time
};

CHECK_OFFSET(tAnimSourceFrame, AnimID, 0x0);
CHECK_OFFSET(tAnimSourceFrame, Time, 0x4);
CHECK_SIZE(tAnimSourceFrame, 0x8);

// One weighted animation frame for the model blend (On Tour struct tAnimBlendFrame, 0x50; [O] getAnimBlendFrames(
// tAnimBlendFrame* frames, int max), cRiderAnimBase::getAIAnimFrame returns one). cAnimSequencer_getAnimBlendFrames
// writes one per active sub-animation of each playing sequence; cRiderAnimBase_setupAnimation gathers up to 64 from the
// six sequencers on the stack and passes them to the model blend (func_0030F2B0, On Tour cAnimModel::blend).
// Size 0x50 [U]: the 0x50 stride of those arrays (views sAnimState_00312598 / sAnimState_00312660 / sSeqState_314418);
// 16-byte aligned (tPose; getAIAnimFrame returns it by value with lq/sq). [Staged by views group A.]
struct tAnimBlendFrame {
    tAnimSourceFrame SourceFrame;   // 0x00 [O]; [U] getAnimBlendFrames: the sub-animation's AnimID and Time
    int Priority;                   // 0x08 [O]; [U] getAnimBlendFrames copies cAnimSequence::Priority
    float Weight;                   // 0x0C [O]; [U] getAnimBlendFrames stores cAnimSequence_getNetWeight
    unsigned long Mask;             // 0x10 [O] (unsigned long long; 64-bit on the EE); [U] getAnimBlendFrames copies
                                    //       cAnimSequence::Mask (ld/sd)
    char unk_018[0x8];              // 0x18 alignment of the 16-byte tPose (On Tour has the same gap)
    tPose OrientationOffset;        // 0x20 [O]; [U] getAnimBlendFrames copies cAnimSequence::OrientationOffset (lq/sq)
    int IsFlipped;                  // 0x40 [O] bool (a word in retail); [U] getAnimBlendFrames copies
                                    //       cAnimSequence::IsFlipped
    char unk_044[0xC];              // 0x44 tail padding to the 16-byte alignment
#ifdef __cplusplus
    // Not in On Tour's DWARF (it lists no member functions for these structs); retail's 0x40-element stack arrays in
    // cRiderAnimBase_setupAnimation / cRiderAnimBase_getAIAnimFrame keep an (empty) array-constructor loop, which only
    // a non-POD element type produces (the unit views carry the same empty constructor). Possibly implicit in EA's
    // code (a constructor on tPose's members); keep it until that is known.
    tAnimBlendFrame() {}
#endif
};

CHECK_OFFSET(tAnimBlendFrame, SourceFrame, 0x00);
CHECK_OFFSET(tAnimBlendFrame, Priority, 0x08);
CHECK_OFFSET(tAnimBlendFrame, Weight, 0x0C);
CHECK_OFFSET(tAnimBlendFrame, Mask, 0x10);
CHECK_OFFSET(tAnimBlendFrame, OrientationOffset, 0x20);
CHECK_OFFSET(tAnimBlendFrame, IsFlipped, 0x40);
CHECK_SIZE(tAnimBlendFrame, 0x50);

// cAnimSequencer (On Tour: 0x8, NumPlaying + PlayList, no base, no virtuals). cRiderAnimBase owns 6 of them in one
// block: [R] its ctor allocates 0x30 bytes tagged "Sequencers" (6 x 8) and runs cAnimSequencer_init__FPv on each
// (animation/rideranimbase.h: cRiderAnimBase::Sequencer, +0x50). Size 0x8 [R]: that allocation; init writes 0x0 and 0x4.
// Retail methods: unit animation/animsequencer 0x003142E8 (init) .. 0x00314880 (readFromReplayFrame).
// [P]: no preview evidence (tools/layout_xlate.py pairs no cAnimSequencer method).
struct cBxReplayFrame;

struct cAnimSequencer {
    int NumPlaying;                 // 0x0 [O]; [R] cAnimSequencer_init__FPv 0; [U] newSequence / appendSequence +1,
                                    //       removeSequence / expire / expireAll -1; writeToReplayFrame serializes it (4 bytes)
                                    //       and walks that many sequences; readFromReplayFrame zeroes it and appends
    cAnimSequence* PlayList;        // 0x4 [O]; [R] init 0; [U] newSequence pushes at the head, appendSequence links at the
                                    //       tail through cAnimSequence::Next; getSequence / isPlaying / getPosition /
                                    //       getAnimBlendFrames / fadeOutAndExpireAll / removeAllEndTriggers walk it

    // Methods bound to their retail symbols by asm labels.
    void appendSequence(cAnimSequence* node) __asm__("cAnimSequencer_appendSequence");
    void expire(cAnimSequence* node) __asm__("cAnimSequencer_expire");
    void* newSequence(int sequenceID) __asm__("cAnimSequencer_newSequence");
    int getAnimBlendFrames(tAnimBlendFrame* out) __asm__("cAnimSequencer_getAnimBlendFrames");
    void* playAndHold(int sequenceID, int anim, float val) __asm__("cAnimSequencer_playAndHold");
    void expireAll() __asm__("cAnimSequencer_expireAll");
    void fadeOutAndExpireAll(float time) __asm__("cAnimSequencer_fadeOutAndExpireAll");
    cAnimSequence* getSequence(int index) __asm__("cAnimSequencer_getSequence__FP14cAnimSequenceri");
    bool isPlaying(int id) __asm__("cAnimSequencer_isPlaying");
    int getPosition(int id) __asm__("cAnimSequencer_getPosition");
    void removeSequence(cAnimSequence* node) __asm__("cAnimSequencer_removeSequence");
    void* playAndLoop(int sequenceID, int anim, float val) __asm__("cAnimSequencer_playAndLoop");
    void removeAllEndTriggers(int trig) __asm__("cAnimSequencer_removeAllEndTriggers");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cAnimSequencer_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cAnimSequencer_readFromReplayFrame");
};

CHECK_OFFSET(cAnimSequencer, NumPlaying, 0x0);
CHECK_OFFSET(cAnimSequencer, PlayList, 0x4);
CHECK_SIZE(cAnimSequencer, 0x8);

#endif /* ANIMATION_ANIMSEQUENCER_H */
