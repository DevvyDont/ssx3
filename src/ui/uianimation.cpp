#include "vtable.h"
#include "common.h"
#include "ui/uidata.h"
#include "ui/uithing.h"
#include "ui/uianimation.h"

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimationBank_getAnimationByHashName);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern const char str_UIAnim[];

struct sUIAnimEntry {
    int f0;
    int f4;
    int hash;
    int size;
};

/**
 * Finds an animation in the bank by hash name and returns a new cUIAnimation over it.
 *
 * Walks the bank's records (count in the first word, then records with the hash at +0x8 and their size at
 * +0xC); on a match it allocates 0x14 bytes ("UIAnim") and constructs a cUIAnimation over the record. The
 * caller owns the result.
 *
 * @param hash  the animation's name hash (On Tour index)
 * @return the new animation, or 0 when no record has that hash
 */
extern "C" void* cUIAnimationBank_getAnimationByHashName(unsigned int** self, int hash)
{
    sUIAnimEntry* entry = (sUIAnimEntry*)(*self + 1);
    for (unsigned int i = 0; i < **self; i++) {
        if (entry->hash == hash) {
            return new (str_UIAnim, 0x100, 0) cUIAnimation((tUIAnimation*)entry);
        }
        entry = (sUIAnimEntry*)((char*)entry + entry->size);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimationChannel_evalAtFrameFloat);
#ifdef SKIP_ASM
struct cUIAnimationChannel_evalAtFrameFloat_sKey {
    unsigned short dt;
    short value;
};

struct cUIAnimationChannel_evalAtFrameFloat_sAnim {
    char pad[6];
    unsigned short count;
    cUIAnimationChannel_evalAtFrameFloat_sKey keys[1];
};

/**
 * Evaluates an animation channel at a frame by linear interpolation between its keys.
 *
 * The first key's value is the value at frame 0; every later key adds its frame delta. Interpolates inside the
 * first span that reaches @p FrameNum (a negative @p FrameNum extrapolates along the first span); past the last key it returns
 * the last value. A channel without data, or with a single key, gives 0.
 *
 * @param FrameNum  frame number (On Tour: FrameNum)
 * @return the channel's value at that frame
 */
extern "C" float cUIAnimationChannel_evalAtFrameFloat(cUIAnimationChannel_evalAtFrameFloat_sAnim** self, int FrameNum)
{
    cUIAnimationChannel_evalAtFrameFloat_sAnim* anim = *self;
    if (anim == 0) {
        return 0.0f;
    }
    unsigned int i;
    int prevT = 0;
    float prevV = anim->keys[0].value;
    int accT = 0;
    float keyValue = 0.0f;
    cUIAnimationChannel_evalAtFrameFloat_sKey* k = &anim->keys[1];
    for (i = 1; i < anim->count; i++, k++) {
        int dt = k->dt;
        accT += dt;
        keyValue = k->value;
        if (accT >= FrameNum) {
            return (float)(FrameNum - prevT) / (float)dt * (keyValue - prevV) + prevV;
        }
        prevT = accT;
        prevV = keyValue;
    }
    return keyValue;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimationChannel_getAnimChanType);
#ifdef SKIP_ASM
/**
 * Returns the channel's type, the 16-bit word at +0x2 of its data, or 0 when it has no data.
 *
 * The type selects the property the channel drives; cUIAnimation_updateThing() passes it to the object's
 * setChannelData.
 *
 * @return the channel type
 */
unsigned short cUIAnimationChannel::getAnimChanType()
{
    tUIAnimChannel* anim = mpAnimChan;
    if (anim != 0) {
        return anim->mType;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimationChannel_setAnimChanData__FPvi);
#ifdef SKIP_ASM
/**
 * Points an animation channel at its channel data record (stores @p val at +0x0).
 *
 * cUIAnimation_cUIAnimation calls it for each channel slot with that channel's record from the animation data.
 *
 * @param val  the channel data record
 */
void cUIAnimationChannel::setAnimChanData(int val)
{
    mpAnimChan = (tUIAnimChannel*)val;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_cUIAnimation);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UIAnimChan[];

/**
 * Constructs an animation over an animation record and points one channel at each of its tracks.
 *
 * Keeps @p pAnimHeader, and marks no frame applied (mLastFrameEval -1), frame 0 and no start frame
 * (kNoStartFrame). When the record has tracks (mNumChannels) it array-allocates the channels ("UIAnimChan") and
 * points each at its track; tracks follow the 0x10-byte header, each sized by its mByteSize.
 *
 * @param pAnimHeader  the animation record from the bank
 */
cUIAnimation::cUIAnimation(tUIAnimation* pAnimHeader)
{
    mpAnimHeader = pAnimHeader;
    mpAnimChannels = 0;
    mLastFrameEval = -1;
    mFrameNum = 0;
    mScreenStartFrame = kNoStartFrame;
    int** slot = (int**)(&mpAnimChannels);
    unsigned int n = pAnimHeader->mNumChannels;
    if (n != 0) {
        int* block = (int*)operator_new_tag(n * 4 + 0x10, str_UIAnimChan, 0x100, 0);
        block[0] = n;
        int* arr = block + 4;
        int* elem = arr;
        for (int i = n - 1; i != -1; i--, elem++) {
            *elem = 0;
        }
        *slot = arr;
        char* chanData = (char*)(mpAnimHeader + 1);
        for (unsigned int i = 0; i < mpAnimHeader->mNumChannels; i++) {
            // PORT: the entry pointer is passed as int
            mpAnimChannels[i].setAnimChanData((int)chanData);
            chanData += ((tUIAnimChannel*)chanData)->mByteSize;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", _$_12cUIAnimation);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Destroys a UI animation: frees its channel array (mpAnimChannels, allocated with new[]: its count lives 0x10
 * bytes before it).
 *
 * The loop that walks the array back to its start destroys nothing (the elements have no destructor).
 */
cUIAnimation::~cUIAnimation()
{
    char* base = (char*)mpAnimChannels;
    if (base != 0) {
        char* elemPtr = base + *(int*)(base - 0x10) * 4;
        while ((char*)mpAnimChannels != elemPtr) {
            elemPtr -= 4;
        }
        cMemMan_free((char*)mpAnimChannels - 0x10);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_isLastFrame);
#ifdef SKIP_ASM
/**
 * Returns whether an animation's current frame (+0xA) has reached its frame count (header +0x2).
 *
 * cUIThing_isLastAnimFrame uses it.
 *
 * @return true at or past the last frame, else false
 */
bool cUIAnimation::isLastFrame()
{
    return (mFrameNum) >= (int) (mpAnimHeader->mNumFrames);
}
#endif
//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_getNumFrames);
#ifdef SKIP_ASM
/**
 * Returns an animation's frame count: the short at +0x2 of its header (+0x0).
 *
 * cUIThing_setFrameByPercent uses it.
 *
 * @return the number of frames
 */
short cUIAnimation::getNumFrames()
{
    return (short)mpAnimHeader->mNumFrames;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_start);
#ifdef SKIP_ASM
/**
 * Starts the animation in a playback mode.
 *
 * Only the low 3 bits of @p mode count (ePlayback). kPlay_Clock (driven by an external frame clock) records @p
 * FrameNum as the start frame (+0xC). kPlay_Backward starts at the last frame with the direction byte (+0xE) set to
 * UIANIMDIR_BW; every other mode starts at frame 0 with it UIANIMDIR_FW.
 *
 * @param mode      playback mode (On Tour playback): 0 external clock, 1 forward, 2 backward, 3 ping-pong
 * @param FrameNum  the current clock frame, kept as the start frame in mode 0
 */
void cUIAnimation::start(int mode, unsigned short FrameNum)
{
    mode &= 7;
    if (mode == kPlay_Clock) {
        mScreenStartFrame = FrameNum;
    }
    if (mode == kPlay_Backward) {
        mFrameDirection = UIANIMDIR_BW;
        mFrameNum = mpAnimHeader->mNumFrames - 1;
    } else {
        mFrameDirection = UIANIMDIR_FW;
        mFrameNum = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_updateFrame);
#ifdef SKIP_ASM
struct cUIAnimation_updateFrame_sData {
    short pad0;
    unsigned short count;
};


/**
 * Steps the animation's frame once according to the play-mode flags.
 *
 * The low three bits of @p flags select the mode (ePlayback): kPlay_Forward steps forward, kPlay_Backward steps
 * backward and kPlay_PingPong ping-pongs between the ends using mFrameDirection. In the first two, kPlay_Wrap (0x8)
 * wraps to the other end instead of stopping at the last or first frame. kPlay_Clock, modes 4-7 and an animation
 * without data leave the frame alone.
 *
 * @param flags  play mode in bits 0-2, wrap flag in bit 3
 */
void cUIAnimation::updateFrame(unsigned char flags)
{
    cUIAnimation_updateFrame_sData* data = ((cUIAnimation_updateFrame_sData*)mpAnimHeader);
    int mode;
    if (data == 0) {
        return;
    }
    mode = flags & 7;
    if (mode == kPlay_Clock) {
        return;
    }
    switch (mode) {
    case kPlay_Backward:
        mFrameNum--;
        if (mFrameNum < 0) {
            if (flags & kPlay_Wrap) {
                mFrameNum = data->count - 1;
            } else {
                mFrameNum = 0;
            }
        }
        break;
    case kPlay_Forward:
        mFrameNum++;
        if (mFrameNum >= data->count) {
            if (!(flags & kPlay_Wrap)) {
                mFrameNum = data->count - 1;
            } else {
                mFrameNum = 0;
            }
        }
        break;
    case kPlay_PingPong:
        if ((mFrameDirection) == UIANIMDIR_FW) {
            mFrameNum++;
        } else {
            mFrameNum--;
        }
        if ((mFrameDirection) == UIANIMDIR_FW) {
            if (mFrameNum >= ((cUIAnimation_updateFrame_sData*)mpAnimHeader)->count) {
                mFrameDirection = UIANIMDIR_BW;
                mFrameNum = ((cUIAnimation_updateFrame_sData*)mpAnimHeader)->count - 1;
                return;
            }
        }
        if ((mFrameDirection) == UIANIMDIR_BW && mFrameNum < 0) {
            mFrameDirection = UIANIMDIR_FW;
            mFrameNum = 0;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", cUIAnimation_updateThing);
#ifdef SKIP_ASM

/**
 * Applies the animation's current frame to a UI object, once per new frame.
 *
 * In mode 0 with a start frame the frame is @p FrameNum minus the start (nothing happens before the start or
 * when it is unchanged). If the frame differs from the last applied one (+0x8) and lies inside the animation,
 * each channel is evaluated there and passed with its type to the object's setChannelData (vtable slot 12);
 * the frame is then recorded as applied.
 *
 * @param obj       the UI object to animate (On Tour pThing)
 * @param mode      playback mode; only the low 3 bits count
 * @param FrameNum  the external clock frame, used in mode 0
 */
void cUIAnimation::updateThing(cUIThing* obj, int mode, unsigned short FrameNum)
{
    unsigned short frame = mFrameNum;
    if ((mode & 7) == kPlay_Clock && mScreenStartFrame != kNoStartFrame) {
        if (FrameNum < mScreenStartFrame) return;
        frame = FrameNum - mScreenStartFrame;
        if (mFrameNum == frame) return;
        mFrameNum = frame;
    }
    if (mLastFrameEval == frame) return;
    if (frame < mpAnimHeader->mNumFrames) {
        for (unsigned int i = 0; i < mpAnimHeader->mNumChannels; i++) {
            float chanValue = cUIAnimationChannel_evalAtFrameFloat((cUIAnimationChannel_evalAtFrameFloat_sAnim**)&mpAnimChannels[i], frame);
            sVEntry<void(void*, int, float)>* vt = (sVEntry<void(void*, int, float)>*)VPTR_OF(cListNode, obj);
            vt[12].fn(vthis(obj, vt[12]), mpAnimChannels[i].getAnimChanType(), chanValue);
        }
    }
    mLastFrameEval = frame;
}
#endif

