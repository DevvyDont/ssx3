#include "common.h"
#include "world/world.h"
#include "vtable.h"
#include "worldpainter/quadtree.h"
#include "bx/math3d.h"
#include "render/2drender.h"
#include "ai/rider.h"
#include "sound/bankmanager.h"
#include "sound/asyncsys.h"
#include "replay/frameptr.h"
#include "sound/bankmonitor.h"
#include "sound/ssxaudio.h"
#include "sound/crowdsys.h"
#include "sound/banksys.h"
#include "worldpainter/worldpainterquery.h"
#include "worldpainter/worldpainterman.h"
#include "worldpainter/worldpainterlayer.h"
#include "sound/speechsys.h"
#include "sound/musicsys.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"
#include "render/graphicsman.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("bx/seg_1BA100", bxLogPrint);
#ifdef SKIP_ASM
/**
 * Discards a formatted log message: the body is empty in this build.
 *
 * @param fmt  printf-style format string (ignored, as are the variable arguments)
 */
extern "C" void bxLogPrint(const char *fmt, ...) {
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSoundSys_GetSoundManager);
#ifdef SKIP_ASM
/**
 * Returns the first word of a cSoundSys, its sound manager pointer (pManager).
 *
 * @param soundSys  the cSoundSys
 * @return pManager
 */
extern "C" int cSoundSys_GetSoundManager(int *soundSys) {
    return *soundSys;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Instance);
#ifdef SKIP_ASM
/**
 * Returns mInst, the play-option stack's current level (read as volatile).
 *
 * @return mInst
 */
int cSndPlayOpts::OPTS_Instance()
{
    return mInst;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Current);
#ifdef SKIP_ASM
/**
 * Returns the current level's SNDPLAYOPTS record, &mCurrent[mInst].
 *
 * @return the current level's play options
 */
SNDPLAYOPTS* cSndPlayOpts::OPTS_Current()
{
    return &mCurrent[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetPos);
#ifdef SKIP_ASM
/**
 * Returns mPosition[mInst], the current level's sound position.
 *
 * @return the position vector pointer, or 0
 */
const t4Vector* cSndPlayOpts::OPTS_GetPos()
{
    return mPosition[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetStaticPos);
#ifdef SKIP_ASM
/**
 * Returns mPosStatic[mInst], the current level's static-position flag.
 *
 * @return nonzero when the position is static
 */
int cSndPlayOpts::OPTS_GetStaticPos()
{
    return mPosStatic[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetVol);
#ifdef SKIP_ASM
/**
 * Returns mBaseVol[mInst], the current level's unclamped base volume.
 *
 * @return the base volume
 */
int cSndPlayOpts::OPTS_GetVol()
{
    return mBaseVol[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetVolControl);
#ifdef SKIP_ASM
/**
 * Returns mVolControl[mInst], the current level's volume-control float pointer.
 *
 * @return the volume control, or 0
 */
float* cSndPlayOpts::OPTS_GetVolControl()
{
    return mVolControl[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetCallback);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Returns mCallback[mInst], the current level's sound callback (an 8-byte cController member-function pointer).
 *
 * @param opts  the play options
 * @return the callback
 */
// cSndPlayOpts::OPTS_GetCallback (prototype name)
tSndCallback cSndPlayOpts::OPTS_GetCallback()
{
    return mCallback[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetAzimuth);
#ifdef SKIP_ASM
/**
 * Returns the current level's azimuth, mCurrent[mInst].azimuth.
 *
 * @return the azimuth
 */
unsigned short cSndPlayOpts::OPTS_GetAzimuth()
{
    return mCurrent[mInst].azimuth;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetElevation);
#ifdef SKIP_ASM
/**
 * Returns the current level's elevation, mCurrent[mInst].elevation.
 *
 * @return the elevation
 */
short cSndPlayOpts::OPTS_GetElevation()
{
    return mCurrent[mInst].elevation;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetDelay);
#ifdef SKIP_ASM
/**
 * Returns the current level's delay settings, &mDelay[mInst].
 *
 * @return the cDelayOpts record
 */
cDelayOpts* cSndPlayOpts::OPTS_GetDelay()
{
    return &mDelay[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetPtr);
#ifdef SKIP_ASM
/**
 * Returns mPtr[mInst], the current level's user pointer.
 *
 * @return the user pointer
 */
void* cSndPlayOpts::OPTS_GetPtr()
{
    return mPtr[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetViewport);
#ifdef SKIP_ASM
/**
 * Returns mViewport[mInst], the current level's viewport.
 *
 * @return the viewport
 */
int cSndPlayOpts::OPTS_GetViewport()
{
    return mViewport[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetVanish);
#ifdef SKIP_ASM
/**
 * Returns mVanish[mInst], the current level's vanish distance.
 *
 * @return the vanish distance
 */
float cSndPlayOpts::OPTS_GetVanish()
{
    return mVanish[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetRadius);
#ifdef SKIP_ASM
/**
 * Returns mRadius[mInst], the current level's radius.
 *
 * @return the radius
 */
float cSndPlayOpts::OPTS_GetRadius()
{
    return mRadius[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetExternalVol);
#ifdef SKIP_ASM
/**
 * Returns mExternalVol[mInst], the current level's external-volume flag.
 *
 * @return the flag
 */
int cSndPlayOpts::OPTS_GetExternalVol()
{
    return mExternalVol[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetMapToMaxVol);
#ifdef SKIP_ASM
/**
 * Returns mMapToMaxVol[mInst], the current level's map-to-max-volume value.
 *
 * @return the value
 */
int cSndPlayOpts::OPTS_GetMapToMaxVol()
{
    return mMapToMaxVol[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetCloseOnEnd);
#ifdef SKIP_ASM
/**
 * Returns mCloseOnEnd[mInst], the current level's close-on-end flag.
 *
 * @return the flag
 */
int cSndPlayOpts::OPTS_GetCloseOnEnd()
{
    return mCloseOnEnd[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetTwoPlayerPan);
#ifdef SKIP_ASM
/**
 * Returns mTwoPlayerPan[mInst], the current level's two-player pan flag.
 *
 * @return the flag
 */
int cSndPlayOpts::OPTS_GetTwoPlayerPan()
{
    return mTwoPlayerPan[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_GetTwoPlayerSide);
#ifdef SKIP_ASM
/**
 * Returns mTwoPlayerSide[mInst], the current level's two-player side.
 *
 * @return the side
 */
int cSndPlayOpts::OPTS_GetTwoPlayerSide()
{
    return mTwoPlayerSide[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_LibDefault);
#ifdef SKIP_ASM
extern "C" void SNDplaysetdef(SNDPLAYOPTS* opts);

/**
 * Fills the current level's SNDPLAYOPTS (mCurrent[mInst]) with the audio library's defaults via SNDplaysetdef.
 *
 */
void cSndPlayOpts::OPTS_LibDefault()
{
    SNDplaysetdef(&mCurrent[mInst]);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_SetDefault);
#ifdef SKIP_ASM
/**
 * Copies the current level's SNDPLAYOPTS (mCurrent[mInst]) into mDefault, the record each push starts from.
 */
void cSndPlayOpts::OPTS_SetDefault()
{
    mDefault = mCurrent[mInst];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Reset);
#ifdef SKIP_ASM
/**
 * Resets the current play-option level (mInst) to the defaults.
 *
 * mCurrent[mInst] gets mDefault and mBaseVol[mInst] its volume; mPosition, mPosStatic, mVolControl, mCallback,
 * mPtr, mViewport, mVanish, mRadius, mExternalVol, mTwoPlayerPan and mTwoPlayerSide are cleared; mDelay[mInst] becomes
 * time 100, count 0, feedback 90, level 50; mMapToMaxVol becomes 127 and mCloseOnEnd 1.
 *
 */
void cSndPlayOpts::OPTS_Reset()
{
    mCurrent[mInst] = mDefault;
    mPosition[mInst] = 0;
    mPosStatic[mInst] = 0;
    mBaseVol[mInst] = mCurrent[mInst].vol;
    mVolControl[mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    mCallback[mInst] = 0;
    mPtr[mInst] = 0;
    mViewport[mInst] = 0;
    mVanish[mInst] = 0;
    mRadius[mInst] = 0;
    mDelay[mInst].mCount = 0;
    mDelay[mInst].mTime = 100;
    mDelay[mInst].mFeedback = 90;
    mDelay[mInst].mLevel = 50;
    mExternalVol[mInst] = 0;
    mTwoPlayerPan[mInst] = 0;
    mTwoPlayerSide[mInst] = 0;
    mMapToMaxVol[mInst] = 127;
    mCloseOnEnd[mInst] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Open);
#ifdef SKIP_ASM
/**
 * Pushes a new play-option level: increments mInst, then resets that level as cSndPlayOpts_OPTS_Reset does.
 *
 * mCurrent[mInst] gets mDefault and mBaseVol[mInst] its volume; mPosition, mPosStatic, mVolControl, mCallback,
 * mPtr, mViewport, mVanish, mRadius, mExternalVol, mTwoPlayerPan and mTwoPlayerSide are cleared; mDelay[mInst] becomes
 * time 100, count 0, feedback 90, level 50; mMapToMaxVol becomes 127 and mCloseOnEnd 1.
 *
 * There is no check against the four levels the arrays hold.
 *
 */
void cSndPlayOpts::OPTS_Open()
{
    mInst++;
    mCurrent[mInst] = mDefault;
    mPosition[mInst] = 0;
    mPosStatic[mInst] = 0;
    mBaseVol[mInst] = mCurrent[mInst].vol;
    mVolControl[mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    mCallback[mInst] = 0;
    mPtr[mInst] = 0;
    mViewport[mInst] = 0;
    mVanish[mInst] = 0;
    mRadius[mInst] = 0;
    mDelay[mInst].mCount = 0;
    mDelay[mInst].mTime = 100;
    mDelay[mInst].mFeedback = 90;
    mDelay[mInst].mLevel = 50;
    mExternalVol[mInst] = 0;
    mTwoPlayerPan[mInst] = 0;
    mTwoPlayerSide[mInst] = 0;
    mMapToMaxVol[mInst] = 127;
    mCloseOnEnd[mInst] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Close);
#ifdef SKIP_ASM
/**
 * Pops the current play-option level: decrements mInst (no check against 0).
 *
 */
void cSndPlayOpts::OPTS_Close()
{
    mInst = mInst - 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Key);
#ifdef SKIP_ASM
/**
 * Sets the current level's key number, mCurrent[mInst].keynum, to @p value.
 *
 * @param value  the key number
 */
void cSndPlayOpts::OPTS_Key(signed char value)
{
    mCurrent[mInst].keynum = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Velocity);
#ifdef SKIP_ASM
/**
 * Sets the current level's velocity, mCurrent[mInst].velocity, to @p value.
 *
 * @param value  the velocity
 */
void cSndPlayOpts::OPTS_Velocity(signed char value)
{
    mCurrent[mInst].velocity = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Vol);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#define MIN_9810(a,b) ((a) < (b) ? (a) : (b))
#define MAX_9810(a,b) ((a) < (b) ? (b) : (a))
/**
 * Sets the current level's volume: mBaseVol[mInst] = @p vol and mCurrent[mInst].vol = @p vol clamped to 0..127.
 *
 * @param vol   the volume
 */
void cSndPlayOpts::OPTS_Vol(int vol)
{
    mBaseVol[mInst] = vol;
    mCurrent[mInst].vol = MAX_9810(MIN_9810(mBaseVol[mInst], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_ScaleVol);
#ifdef SKIP_ASM
#define MIN_98A8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_98A8(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Scales the current level's volume by a float factor.
 *
 * mBaseVol[mInst] becomes mBaseVol[mInst] * @p f clamped to 0..127 (truncated to int), and mCurrent[mInst].vol takes
 * that value.
 *
 * @param scale  the scale factor
 */
void cSndPlayOpts::OPTS_ScaleVol(float scale)
{
    mBaseVol[mInst] = (int)MAX_98A8(MIN_98A8(mBaseVol[mInst] * scale, 127.0f), 0.0f);
    mCurrent[mInst].vol = MAX_98A8(MIN_98A8(mBaseVol[mInst], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_ScaleVol1);
#ifdef SKIP_ASM
#define MIN_99E8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_99E8(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Scales the current level's volume by @p n / 127.
 *
 * mBaseVol[mInst] becomes mBaseVol[mInst] * @p n / 127 (integer division) clamped to 0..127, and mCurrent[mInst].vol
 * takes that value.
 *
 * @param n     the scale, 127 for unchanged
 */
void cSndPlayOpts::OPTS_ScaleVol1(int n)
{
    mBaseVol[mInst] = MAX_99E8(MIN_99E8(mBaseVol[mInst] * n / 127, 127), 0);
    mCurrent[mInst].vol = MAX_99E8(MIN_99E8(mBaseVol[mInst], 127), 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Bend);
#ifdef SKIP_ASM
/**
 * Sets the current level's pitch bend, mCurrent[mInst].bend, to @p value.
 *
 * @param value  the bend
 */
void cSndPlayOpts::OPTS_Bend(signed char value)
{
    mCurrent[mInst].bend = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_PitchMult);
#ifdef SKIP_ASM
/**
 * Sets the current level's pitch multiplier, mCurrent[mInst].pitchmult, to @p value.
 *
 * @param value  the pitch multiplier
 */
void cSndPlayOpts::OPTS_PitchMult(short value)
{
    mCurrent[mInst].pitchmult = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_DelayCount);
#ifdef SKIP_ASM
/**
 * Sets the current level's delay count, mDelay[mInst].mCount, to @p value.
 *
 * @param value  the new count
 */
void cSndPlayOpts::OPTS_DelayCount(int value)
{
    mDelay[mInst].mCount = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_DelayTime);
#ifdef SKIP_ASM
/**
 * Sets the current level's delay time, mDelay[mInst].mTime, to @p value.
 *
 * @param value  the new time
 */
void cSndPlayOpts::OPTS_DelayTime(int value)
{
    mDelay[mInst].mTime = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_DelayFeedback);
#ifdef SKIP_ASM
/**
 * Sets the current level's delay feedback, mDelay[mInst].mFeedback, to @p value.
 *
 * @param value  the new feedback
 */
void cSndPlayOpts::OPTS_DelayFeedback(int value)
{
    mDelay[mInst].mFeedback = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_DelayLevel);
#ifdef SKIP_ASM
/**
 * Sets the current level's delay level, mDelay[mInst].mLevel, to @p value.
 *
 * @param value  the new level
 */
void cSndPlayOpts::OPTS_DelayLevel(int value)
{
    mDelay[mInst].mLevel = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Azimuth);
#ifdef SKIP_ASM
/**
 * Sets the current level's azimuth, mCurrent[mInst].azimuth, to @p value.
 *
 * @param value  the azimuth
 */
void cSndPlayOpts::OPTS_Azimuth(short value)
{
    mCurrent[mInst].azimuth = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Elevation);
#ifdef SKIP_ASM
/**
 * Sets the current level's elevation, mCurrent[mInst].elevation, to @p value.
 *
 * @param value  the elevation
 */
void cSndPlayOpts::OPTS_Elevation(short value)
{
    mCurrent[mInst].elevation = value;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Position);
#ifdef SKIP_ASM
/**
 * Sets the current level's sound position, mPosition[mInst], to @p position.
 *
 * @param position  the position vector pointer (kept, not copied)
 */
void cSndPlayOpts::OPTS_Position(const t4Vector* position)
{
    mPosition[mInst] = position;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_StaticPos);
#ifdef SKIP_ASM
/**
 * Marks the current level's position as static: sets mPosStatic[mInst] to 1.
 *
 */
void cSndPlayOpts::OPTS_StaticPos()
{
    int *base = mPosStatic;
    base[mInst] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Viewport);
#ifdef SKIP_ASM
/**
 * Sets the current level's viewport, mViewport[mInst], to @p viewport.
 *
 * @param viewport  the viewport
 */
void cSndPlayOpts::OPTS_Viewport(int viewport)
{
    mViewport[mInst] = viewport;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_LowPassCutOffFrequency);
#ifdef SKIP_ASM
/**
 * Sets the current level's low-pass cutoff, mCurrent[mInst].lowpasscutoff, to @p value clamped to 0..0xFFFF.
 *
 * @param value  the cutoff frequency
 */
void cSndPlayOpts::OPTS_LowPassCutOffFrequency(int value)
{
    int clamped = (value <= 0xFFFF) ? value : 0xFFFF;
    mCurrent[mInst].lowpasscutoff = (clamped <= -1) ? 0 : clamped;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Ptr);
#ifdef SKIP_ASM
/**
 * Sets the current level's user pointer, mPtr[mInst], to @p userPtr.
 *
 * @param userPtr  the user pointer
 */
void cSndPlayOpts::OPTS_Ptr(void* userPtr)
{
    mPtr[mInst] = userPtr;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_VolControl);
#ifdef SKIP_ASM
/**
 * Sets the current level's volume control, mVolControl[mInst], to @p volControl.
 *
 * @param volControl  the float the sound's volume follows (a float pointer), or 0
 */
void cSndPlayOpts::OPTS_VolControl(float* volControl)
{
    mVolControl[mInst] = volControl;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_Callback);
#ifdef SKIP_ASM
/**
 * Sets the current level's sound callback, mCallback[mInst], to @p callback (an 8-byte member-function pointer).
 *
 * @param callback  the cController member-function pointer
 */
void cSndPlayOpts::OPTS_Callback(tSndCallback callback)
{
    mCallback[mInst] = callback;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_ExternalVol);
#ifdef SKIP_ASM
/**
 * Sets the current level's external-volume flag, mExternalVol[mInst], to @p externalVol.
 *
 * @param externalVol  the flag
 */
void cSndPlayOpts::OPTS_ExternalVol(int externalVol)
{
    mExternalVol[mInst] = externalVol;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_MapToMaxVol);
#ifdef SKIP_ASM
/**
 * Sets the current level's map-to-max-volume value, mMapToMaxVol[mInst], to @p mapToMaxVol.
 *
 * @param mapToMaxVol  the value (cSndPlayOpts_OPTS_Reset resets it to 127)
 */
void cSndPlayOpts::OPTS_MapToMaxVol(int mapToMaxVol)
{
    mMapToMaxVol[mInst] = mapToMaxVol;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_CloseOnEnd);
#ifdef SKIP_ASM
/**
 * Sets the current level's close-on-end flag, mCloseOnEnd[mInst], to @p closeOnEnd.
 *
 * @param closeOnEnd  the flag (cSndPlayOpts_OPTS_Reset resets it to 1)
 */
void cSndPlayOpts::OPTS_CloseOnEnd(int closeOnEnd)
{
    int *base = mCloseOnEnd;
    base[mInst] = closeOnEnd;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_TwoPlayerPan);
#ifdef SKIP_ASM
/**
 * Sets the current level's two-player pan flag, mTwoPlayerPan[mInst], to @p twoPlayerPan.
 *
 * @param twoPlayerPan  the flag
 */
void cSndPlayOpts::OPTS_TwoPlayerPan(int twoPlayerPan)
{
    int *base = mTwoPlayerPan;
    base[mInst] = twoPlayerPan;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSndPlayOpts_OPTS_TwoPlayerSide);
#ifdef SKIP_ASM
/**
 * Sets the current level's two-player side, mTwoPlayerSide[mInst], to @p twoPlayerSide.
 *
 * @param twoPlayerSide  the side
 */
void cSndPlayOpts::OPTS_TwoPlayerSide(int twoPlayerSide)
{
    int *base = mTwoPlayerSide;
    base[mInst] = twoPlayerSide;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMonitor_MONITOR_Pause);
#ifdef SKIP_ASM
/**
 * Pauses the monitor: sets mPaused to 1.
 *
 */
void cMonitor::MONITOR_Pause()
{
    mPaused = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMonitor_MONITOR_Resume);
#ifdef SKIP_ASM
/**
 * Resumes the monitor: clears mPaused.
 *
 */
void cMonitor::MONITOR_Resume()
{
    mPaused = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMonitor_MONITOR_Paused);
#ifdef SKIP_ASM
/**
 * Returns the monitor's mPaused flag.
 *
 * @return mPaused
 */
int cMonitor::MONITOR_Paused()
{
    return (mPaused);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMonitor_MONITOR_Counter);
#ifdef SKIP_ASM
/**
 * Returns the monitor's tick counter, mCounter (read as volatile).
 *
 * @param monitor  the cMonitor
 * @return mCounter
 */
// From peak3project's fork (github.com/peak3project/ssx3).
// volatile keeps the load out of the jr delay slot, as in the target.
int cMonitor::MONITOR_Counter()
{
    return (mCounter);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMonitor_MONITOR_Delta);
#ifdef SKIP_ASM
/**
 * Returns the monitor ticks since the last call, in hundredths: (mCounter - mLastCounter) * 0.01.
 *
 * Stores mCounter in mLastCounter for the next call.
 *
 * @return the elapsed ticks times 0.01
 */
float cMonitor::MONITOR_Delta()
{
    int temp_2;
    int temp_3;

    temp_2 = (mCounter);
    temp_3 = ((int)mLastCounter);
    ((int)mLastCounter) = temp_2;
    return (float) (temp_2 - temp_3) * 0.01f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_GetInstance);
#ifdef SKIP_ASM
/**
 * Returns monitored sound @p instance's record, &mBankMonitorInstance[instance] (0xC0 bytes each).
 *
 * @param instance  the instance index (64 records)
 * @return the cBankMonitorInstance
 */
cBankMonitorInstance* cBankMonitor::BANKMONITOR_GetInstance(int instance)
{
    return &mBankMonitorInstance[instance];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_Paused);
#ifdef SKIP_ASM
/**
 * Returns the bank monitor's pause flag, mBankMonitorPaused.
 *
 * @return mBankMonitorPaused
 */
int cBankMonitor::BANKMONITOR_Paused()
{
    return (mBankMonitorPaused);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_SetDoppler);
#ifdef SKIP_ASM
/**
 * Sets the bank monitor's Doppler switch, mDopplerEnable, to @p enable.
 *
 * @param enable  nonzero to enable Doppler
 */
void cBankMonitor::BANKMONITOR_SetDoppler(int enable)
{
    mDopplerEnable = enable;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_SetPositional);
#ifdef SKIP_ASM
/**
 * Sets the bank monitor's positional switch, mPositionalEnable, to @p enable.
 *
 * @param enable  nonzero to enable positional sound
 */
void cBankMonitor::BANKMONITOR_SetPositional(int enable)
{
    mPositionalEnable = enable;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_DopplerEnabled);
#ifdef SKIP_ASM
/**
 * Returns the bank monitor's Doppler switch, mDopplerEnable.
 *
 * @return mDopplerEnable
 */
int cBankMonitor::BANKMONITOR_DopplerEnabled()
{
    return (mDopplerEnable);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankMonitor_BANKMONITOR_PositionalEnabled);
#ifdef SKIP_ASM
/**
 * Returns the bank monitor's positional switch, mPositionalEnable.
 *
 * @return mPositionalEnable
 */
int cBankMonitor::BANKMONITOR_PositionalEnabled()
{
    return (mPositionalEnable);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cAsyncSys_ASYNCSYS_Busy);
#ifdef SKIP_ASM
/**
 * Returns the async loader's busy flag (+0x4, On Tour cAsyncSys::mBusy).
 *
 * @return nonzero while a load is in progress
 */
int cAsyncSys::ASYNCSYS_Busy()
{
    return (mBusy);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cAsyncSys_ASYNCSYS_GetCurrOperationUserHandle);
#ifdef SKIP_ASM
/**
 * Returns the user handle of the async loader's current operation, or -1.
 *
 * -1 when the loader is not busy (+0x4) or its user-handle queue (On Tour mUserHandle, a circular buffer at +0x18C)
 * is empty (its +0x8 and +0xC indices equal); otherwise the queue's array entry at its +0xC index.
 *
 * @return the user handle, or -1
 */
int cAsyncSys::ASYNCSYS_GetCurrOperationUserHandle()
{
    int userHandle;
    if (mBusy != 0) {
        cCircBuffer *handleQueue = ((&mUserHandle));
        if (mUserHandle.Head == mUserHandle.Tail) userHandle = -1;
        else userHandle = handleQueue->Array[handleQueue->Tail];
    } else userHandle = -1;
    return userHandle;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cAsyncSys_ASYNCSYS_CancelCurrent);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_DoCancel(void*, int);
/**
 * Cancels the async loader's current operation: cAsyncSys_DoCancel(@p asyncSys, 0).
 *
 */
void cAsyncSys::ASYNCSYS_CancelCurrent()
{
    cAsyncSys_DoCancel(this, 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cAsyncSys_ASYNCSYS_CancelAll);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_DoCancel(void*, int);
/**
 * Cancels every queued async load: cAsyncSys_DoCancel(@p asyncSys, 1).
 *
 */
void cAsyncSys::ASYNCSYS_CancelAll()
{
    cAsyncSys_DoCancel(this, 1);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankManager_BANK_Status);
#ifdef SKIP_ASM
/**
 * Returns bank @p bankId's status (the first word of its 0x60-byte record in the cBankManager's bank array at +0xACC).
 *
 * On Tour's eBankStatus: 0 empty, 1 used, 2 loading.
 *
 * @param bankId  the bank index
 * @return the bank status
 */
int cBankManager::BANK_Status(int bankId)
{
    cBankInstance *base = mBank;
    return base[bankId].mBankStatus;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankManager_BANK_Used);
#ifdef SKIP_ASM
/**
 * Returns whether bank @p bankId is loaded: its status (cBankManager +0xACC record) is USED.
 *
 * @param bankId  the bank index
 * @return true when the bank's status is USED
 */
bool cBankManager::BANK_Used(int bankId)
{
    cBankInstance *base = mBank;
    return base[bankId].mBankStatus == USED;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankManager_BANK_Loading);
#ifdef SKIP_ASM
/**
 * Returns whether bank @p bankId is loading: its status (cBankManager +0xACC record) is LOADING.
 *
 * @param bankId  the bank index
 * @return true when the bank's status is LOADING
 */
bool cBankManager::BANK_Loading(int bankId)
{
    cBankInstance *base = mBank;
    return base[bankId].mBankStatus == LOADING;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankManager_BANK_InUse);
#ifdef SKIP_ASM
extern "C" int cBankManager_BANK_Handle(cBankManager*, int);
extern "C" void SNDSYS_entercritical();
extern "C" void SNDSYS_leavecritical();
extern "C" int SNDbankinuse(int);

/**
 * Returns whether a loaded bank is in use by the audio library.
 *
 * Between SNDSYS_entercritical and SNDSYS_leavecritical (the lock pair cBankSys_SFX_End also uses around library calls), checks that
 * the bank's status is USED and then whether SNDbankinuse returns true for its handle (cBankManager_BANK_Handle).
 *
 * @param idx  the bank index
 * @return 1 when the bank is loaded and SNDbankinuse reports it in use, else 0
 */
int cBankManager::BANK_InUse(int idx)
{
    int ready = 0;
    SNDSYS_entercritical();
    bool ok = mBank[idx].mBankStatus != USED;
    if (!ok) {
        ready = SNDbankinuse(BANK_Handle(idx)) == true;
    }
    SNDSYS_leavecritical();
    return ready;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankManager_BANK_Handle);
#ifdef SKIP_ASM
/**
 * Returns bank @p bankId's library handle (the second word of its record) when the bank is loaded, else
 * kNoBankHandle (-1).
 *
 * @param bankId  the bank index
 * @return the bank handle, or cBankInstance::kNoBankHandle when its status is not USED
 */
int cBankManager::BANK_Handle(int bankId)
{
    cBankInstance *base = mBank;
    bool ne = base[bankId].mBankStatus != USED;
    if (!ne) {
        return base[bankId].mBankHandle;
    }
    return cBankInstance::kNoBankHandle;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Bank);
#ifdef SKIP_ASM
/**
 * Sets the bank of the current play-option level: mBank[mInst] = @p bank.
 *
 * mInst is read from the cBankManager's cBankMonitor (mpVBankManager + 0x1D8).
 *
 * @param bank  the bank index
 */
void cBankSys::SFX_Bank(int bank)
{
    int *base = mBank;
    base[mpVBankManager->BankMonitor.mInst] = bank;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Patch);
#ifdef SKIP_ASM
/**
 * Sets the patch of the current play-option level: mPatch[mInst] = @p patch.
 *
 * mInst is read from the cBankManager's cBankMonitor (mpVBankManager + 0x1D8).
 *
 * @param patch  the patch index
 */
void cBankSys::SFX_Patch(int patch)
{
    int *base = mPatch;
    base[mpVBankManager->BankMonitor.mInst] = patch;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Begin);
#ifdef SKIP_ASM
/**
 * Starts an SFX setup: pushes a play-option level.
 *
 * Through mpVBankManager, the cBankManager's play options (its cBankMonitor BankMonitor, mpVBankManager + 0x1D8) get a
 * new level exactly as cSndPlayOpts_OPTS_Open pushes one (mInst incremented, the level reset to mDefault and the
 * defaults).
 *
 */
void cBankSys::SFX_Begin()
{
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 127;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Begin1);
#ifdef SKIP_ASM
/**
 * Starts an SFX setup for a bank: pushes a play-option level and sets its bank.
 *
 * Through mpVBankManager, the cBankManager's play options (its cBankMonitor BankMonitor, mpVBankManager + 0x1D8) get a
 * new level exactly as cSndPlayOpts_OPTS_Open pushes one (mInst incremented, the level reset to mDefault and the
 * defaults), then
 * mBank[mInst] = @p bank.
 *
 * @param bank  the bank index
 */
void cBankSys::SFX_Begin1(int bank)
{
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 127;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[mpVBankManager->BankMonitor.mInst] = bank;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Begin2);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Starts an SFX setup for a bank and patch: pushes a play-option level and sets both.
 *
 * Through mpVBankManager, the cBankManager's play options (its cBankMonitor BankMonitor, mpVBankManager + 0x1D8) get a
 * new level exactly as cSndPlayOpts_OPTS_Open pushes one (mInst incremented, the level reset to mDefault and the
 * defaults), then
 * mBank[mInst] = @p bank and mPatch[mInst] = @p patch.
 *
 * @param bank  the bank index
 * @param patch  the patch index
 */
void cBankSys::SFX_Begin2(int bank, int patch)
{
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 127;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[mpVBankManager->BankMonitor.mInst] = bank;
    mPatch[mpVBankManager->BankMonitor.mInst] = patch;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Play2D);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBankSys_SFX_End(void*);
/**
 * Plays a bank patch with the default options: pushes a play-option level, sets its bank and patch, and plays it.
 *
 * Through mpVBankManager, the cBankManager's play options (its cBankMonitor BankMonitor, mpVBankManager + 0x1D8) get a
 * new level exactly as cSndPlayOpts_OPTS_Open pushes one (mInst incremented, the level reset to mDefault and the
 * defaults), then
 * mBank[mInst] = @p bank, mPatch[mInst] = @p patch, and cBankSys_SFX_End plays the patch (and pops the level).
 *
 * @param bank  the bank index
 * @param patch  the patch index
 */
void cBankSys::SFX_Play2D(int bank, int patch)
{
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 127;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[mpVBankManager->BankMonitor.mInst] = bank;
    mPatch[mpVBankManager->BankMonitor.mInst] = patch;
    cBankSys_SFX_End(this);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cBankSys_SFX_Play2DVol);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#define MIN_A920(a,b) ((a) < (b) ? (a) : (b))
#define MAX_A920(a,b) ((a) < (b) ? (b) : (a))
extern "C" int cBankSys_SFX_End(void*);
/**
 * Plays a bank patch at a volume: pushes a play-option level, sets its bank, patch and volume, and plays it.
 *
 * Through mpVBankManager, the cBankManager's play options (its cBankMonitor BankMonitor, mpVBankManager + 0x1D8) get a
 * new level exactly as cSndPlayOpts_OPTS_Open pushes one (mInst incremented, the level reset to mDefault and the
 * defaults), then
 * mBank[mInst] = @p bank, mPatch[mInst] = @p patch, the level's volume is set as cSndPlayOpts_OPTS_Vol does
 * (mBaseVol = @p volume, SNDPLAYOPTS volume clamped to 0..127), and cBankSys_SFX_End plays the patch.
 *
 * @param bank  the bank index
 * @param patch  the patch index
 * @param volume  the volume
 */
void cBankSys::SFX_Play2DVol(int bank, int patch, int volume)
{
    cBankManager* bankManager = mpVBankManager;
    cBankMonitor* playOpts = &bankManager->BankMonitor;
    bankManager->BankMonitor.mInst++;
    playOpts->mCurrent[bankManager->BankMonitor.mInst] = bankManager->BankMonitor.mDefault;
    bankManager->BankMonitor.mPosition[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPosStatic[bankManager->BankMonitor.mInst] = 0;
    playOpts->mBaseVol[bankManager->BankMonitor.mInst] = playOpts->mCurrent[bankManager->BankMonitor.mInst].vol;
    playOpts->mVolControl[bankManager->BankMonitor.mInst] = 0;
    // a null member-function pointer: g++ 2.95 clears the 8 bytes with memset(&mCallback[mInst], 0, 8)
    playOpts->mCallback[bankManager->BankMonitor.mInst] = 0;
    playOpts->mPtr[bankManager->BankMonitor.mInst] = 0;
    playOpts->mViewport[bankManager->BankMonitor.mInst] = 0;
    playOpts->mVanish[bankManager->BankMonitor.mInst] = 0;
    playOpts->mRadius[bankManager->BankMonitor.mInst] = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mCount = 0;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mTime = 100;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mFeedback = 90;
    playOpts->mDelay[bankManager->BankMonitor.mInst].mLevel = 50;
    playOpts->mExternalVol[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerPan[bankManager->BankMonitor.mInst] = 0;
    playOpts->mTwoPlayerSide[bankManager->BankMonitor.mInst] = 0;
    playOpts->mMapToMaxVol[bankManager->BankMonitor.mInst] = 127;
    playOpts->mCloseOnEnd[bankManager->BankMonitor.mInst] = 1;
    mBank[mpVBankManager->BankMonitor.mInst] = bank;
    mPatch[mpVBankManager->BankMonitor.mInst] = patch;
    {
        cBankManager* volBankManager = mpVBankManager;
        cBankMonitor* volPlayOpts = &volBankManager->BankMonitor;
        volPlayOpts->mBaseVol[volBankManager->BankMonitor.mInst] = volume;
        volPlayOpts->mCurrent[volBankManager->BankMonitor.mInst].vol =
            MAX_A920(MIN_A920(volPlayOpts->mBaseVol[volPlayOpts->mInst], 127), 0);
    }
    cBankSys_SFX_End(this);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_Current);
#ifdef SKIP_ASM
/**
 * Returns the music system's current song instance, mCurrent.
 *
 * @return mCurrent, or 0 when no song is set up
 */
cSongInstance* cMusicSys::MUSIC_Current()
{
    return mSongInstance;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetStreamHandle);
#ifdef SKIP_ASM
extern "C" int func_002B2488(cSongInstance* song);

/**
 * Returns the current song's stream handle: func_002B2488(mCurrent), or -1 when there is no current song.
 *
 * @return the stream handle, or -1
 */
int cMusicSys::MUSIC_GetStreamHandle()
{
    cSongInstance* song;
    int handle;

    song = mSongInstance;
    handle = -1;
    if (song != 0) {
        handle = func_002B2488(song);
    }
    return handle;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetNumSongs);
#ifdef SKIP_ASM
/**
 * Returns mNumSongs, the number of songs in the music .ini.
 *
 * @return mNumSongs
 */
int cMusicSys::MUSIC_GetNumSongs()
{
    return (mNumSongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetNumActiveSongs);
#ifdef SKIP_ASM
/**
 * Returns mNumActiveSongs, the number of songs in the active playlist.
 *
 * @return mNumActiveSongs
 */
int cMusicSys::MUSIC_GetNumActiveSongs()
{
    return (mNumActiveSongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetNumFESongs);
#ifdef SKIP_ASM
/**
 * Returns mNumFESongs, the number of songs flagged for the front end (ADDTOFE).
 *
 * @return mNumFESongs
 */
int cMusicSys::MUSIC_GetNumFESongs()
{
    return (mNumFESongs);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_SongName);
#ifdef SKIP_ASM
/**
 * Returns the current song's section name, &mSongName[mCurrentSongNum] (11 bytes each).
 *
 * @return the song name
 */
char* cMusicSys::MUSIC_SongName()
{
    return mSongName[mCurrentSongNum];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetCurrentSongNum);
#ifdef SKIP_ASM
/**
 * Returns mCurrentSongNum, the current song's index.
 *
 * @return mCurrentSongNum
 */
int cMusicSys::MUSIC_GetCurrentSongNum()
{
    return (mCurrentSongNum);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_SetDelay);
#ifdef SKIP_ASM
extern "C" void func_002B2850(cSongInstance* song, void* musicSys);

/**
 * Applies the delay settings to the current song: func_002B2850(mCurrent, @p musicSys).
 *
 */
void cMusicSys::MUSIC_SetDelay()
{
    func_002B2850(mSongInstance, this);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_IsSongPlaying);
#ifdef SKIP_ASM
/**
 * Returns whether a song is playing, mSongPlaying.
 *
 * @return mSongPlaying
 */
int cMusicSys::IsSongPlaying()
{
    return (mSongPlaying);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACC0);
#ifdef SKIP_ASM
/**
 * Clears the music system's mSongPlaying flag.
 *
 * @param musicSys  the cMusicSys
 */
extern "C" void func_002BACC0(cMusicSys* musicSys) {
    musicSys->mSongPlaying = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_StoreDefaultActiveMap);
#ifdef SKIP_ASM
/**
 * Saves the active-song map as the default: mDefaultActiveMap = mActiveMap (64-bit).
 *
 * @param musicSys  the cMusicSys
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
void cMusicSys::MUSIC_StoreDefaultActiveMap()
{
    mDefaultActiveMap = mActiveMap;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_MUSIC_GetDefaultActiveMap);
#ifdef SKIP_ASM
/**
 * Returns the default active-song map, mDefaultActiveMap (64-bit).
 *
 * @param musicSys  the cMusicSys
 * @return mDefaultActiveMap
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
long cMusicSys::MUSIC_GetDefaultActiveMap()
{
    return mDefaultActiveMap;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cMusicSys_setCurrentSongNum);
#ifdef SKIP_ASM
/**
 * Sets the current song index, mCurrentSongNum, to @p songNum.
 *
 * @param songNum  the song index
 */
void cMusicSys::setCurrentSongNum(int songNum)
{
    mCurrentSongNum = songNum;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACE8);
#ifdef SKIP_ASM
/**
 * Returns the music system's unk_41C word.
 *
 * func_002B3838 sets it to -1; func_002B3BC0 compares and stores it.
 *
 * @param musicSys  the cMusicSys
 * @return unk_41C
 */
extern "C" int func_002BACE8(cMusicSys* musicSys) {
    return (musicSys->unk_41C);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSpeechSys_GetSpeechVolumeIdentifier);
#ifdef SKIP_ASM
/**
 * Returns -1 (slot 2 of cSpeechSys's vtable cSpeechSys_vtbl).
 *
 * @return -1
 */
extern "C" int cSpeechSys_GetSpeechVolumeIdentifier(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSpeechSys_UpdateSpeechVolume);
#ifdef SKIP_ASM
/**
 * Returns 0 (slot 3 of cSpeechSys's vtable cSpeechSys_vtbl).
 *
 * @return 0
 */
extern "C" int cSpeechSys_UpdateSpeechVolume(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD00);
#ifdef SKIP_ASM
/**
 * Returns big-file name @p i of the speech system, &mBigFileName[i] (20 bytes each).
 *
 * The constructor fills the five names ("audio/speech.big" and the four language files).
 *
 * @param speechSys  the cSpeechSys
 * @param i  the name index, 0-4
 * @return the file name
 */
extern "C" char* func_002BAD00(cSpeechSys* speechSys, int i) {
    int off = i * 20 + 344;
    return ((char*)speechSys) + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cCrowdSys_CROWD_GetData);
#ifdef SKIP_ASM
/**
 * Returns the crowd system's cheer data for intensity @p i, &mCrowdData[i] (0x54 bytes each).
 *
 * @param i  the crowd intensity (three entries)
 * @return the tCrowdData
 */
tCrowdData* cCrowdSys::CROWD_GetData(int i)
{
    return &mCrowdData[i];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD30);
#ifdef SKIP_ASM
/**
 * Sets three cSSXAudio words: unk_5790 to 0, unk_578C to 1 and unk_6254 to 1.
 *
 * @param audio  the cSSXAudio
 */
extern "C" void func_002BAD30(cSSXAudio* audio) {
    audio->unk_5790 = 0;
    audio->unk_578C = 1;
    audio->unk_6254 = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD48);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's unk_578C (set by func_002BAD30; func_002A2E50 and func_002A4A78 use it).
 *
 * @param audio  the cSSXAudio
 * @return unk_578C
 */
extern "C" int func_002BAD48(cSSXAudio* audio) {
    return (audio->unk_578C);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD50);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's unk_5790 (1 after construction, cleared by func_002BAD30).
 *
 * @param audio  the cSSXAudio
 * @return unk_5790
 */
extern "C" int func_002BAD50(cSSXAudio* audio) {
    return (audio->unk_5790);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_SetRailSurface);
#ifdef SKIP_ASM
/**
 * Sets the rail surface type the audio uses for a rider on a rail, mRailType.
 *
 * @param railType  the rail surface type
 */
void cSSXAudio::SetRailSurface(int railType)
{
    mRailType = railType;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetSFXPaused);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's mSFXPaused flag.
 *
 * @return mSFXPaused
 */
int cSSXAudio::GetSFXPaused()
{
    return (mSFXPaused);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetRadioModeID);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's unk_608C (2 selects a radio path in several methods).
 *
 * @return unk_608C
 */
int cSSXAudio::GetRadioModeID()
{
    return (mRadioModeID);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetRadioPlayList);
#ifdef SKIP_ASM
/**
 * Returns the radio playlist mask, mRadioPlayList (64-bit).
 *
 * @param audio  the cSSXAudio
 * @return mRadioPlayList
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
long cSSXAudio::GetRadioPlayList()
{
    return ((long)mRadioPlayList);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetNumPlayLists);
#ifdef SKIP_ASM
/**
 * Returns the number of radio playlists, mNumPlayLists.
 *
 * @return mNumPlayLists
 */
int cSSXAudio::GetNumPlayLists()
{
    return (mNumPlayLists);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetPlayListName);
#ifdef SKIP_ASM
/**
 * Returns the name of radio playlist @p i, &mPlayListName[i] (20 bytes each).
 *
 * @param i  the playlist index
 * @return the playlist name
 */
char* cSSXAudio::GetPlayListName(int i)
{
    return mPlayListName[i];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetPlayListIndex);
#ifdef SKIP_ASM
/**
 * Returns the current radio playlist index, mPlayListIndex.
 *
 * @return mPlayListIndex
 */
int cSSXAudio::GetPlayListIndex()
{
    return (mPlayListIndex);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADA0);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's unk_6280.
 *
 * @param audio  the cSSXAudio
 * @return unk_6280
 */
extern "C" int func_002BADA0(cSSXAudio* audio) {
    return (audio->mPathfinderCanChangeVolume);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetMode);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's audio mode, mAudioMode (eSOUNDDEF_MODE).
 *
 * @return mAudioMode
 */
int cSSXAudio::GetMode()
{
    return (mAudioMode);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADB0);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's mDJSpeech (cSSXAudio_SetDJSpeech stores 1 or 0 there).
 *
 * @param audio  the cSSXAudio
 * @return mDJSpeech
 */
extern "C" int func_002BADB0(cSSXAudio* audio) {
    return (audio->mDJSpeech);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetArcadeSFX);
#ifdef SKIP_ASM
/**
 * Returns cSSXAudio's mArcadeSFX flag.
 *
 * @return mArcadeSFX
 */
int cSSXAudio::GetArcadeSFX()
{
    return (mArcadeSFX);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetPreset);
#ifdef SKIP_ASM
/**
 * Returns the audio preset, mPreset.
 *
 * @return mPreset
 */
int cSSXAudio::GetPreset()
{
    return (mPreset);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cSSXAudio_GetAudioUpdateState);
#ifdef SKIP_ASM
/**
 * Returns the audio update state, mAudioUpdateState.
 *
 * @return mAudioUpdateState
 */
int cSSXAudio::GetAudioUpdateState()
{
    return (mAudioUpdateState);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADD0);
#ifdef SKIP_ASM
struct Item_ADD0 { Item_ADD0* next; int pad; int type; };
struct List_ADD0 { int count; Item_ADD0* items[1]; };
struct Node_ADD0 { Node_ADD0* child[8]; int pad[2]; Item_ADD0* items; };

/**
 * Collects every type-5 item in an octree node and all its descendants into a list.
 *
 * Walks the node's item chain (+0x28; next at +0x0, type at +0x8), appending each item of type 5 to (*@p out)'s array
 * and incrementing its count, then recurses into each of the eight children that is set. func_002B83F8 calls it for
 * nodes that lie wholly inside the query box. No bound on the list's capacity.
 *
 * @param node  the octree node
 * @param out  the list to append to (count, then the item pointers)
 */
extern "C" void func_002BADD0(Node_ADD0* node, List_ADD0** out) {
    Item_ADD0* it;
    for (it = node->items; it != 0; it = it->next) {
        if (it->type == 5) {
            List_ADD0* outList = *out;
            outList->items[outList->count++] = it;
        }
    }
    if (node->child[0]) func_002BADD0(node->child[0], out);
    if (node->child[1]) func_002BADD0(node->child[1], out);
    if (node->child[2]) func_002BADD0(node->child[2], out);
    if (node->child[3]) func_002BADD0(node->child[3], out);
    if (node->child[4]) func_002BADD0(node->child[4], out);
    if (node->child[5]) func_002BADD0(node->child[5], out);
    if (node->child[6]) func_002BADD0(node->child[6], out);
    if (node->child[7]) func_002BADD0(node->child[7], out);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAEE8);
#ifdef SKIP_ASM
extern "C" void __sti__all_in_one_sound_cpp(int, int);

/**
 * Runs __sti__all_in_one_sound_cpp(1, 0xFFFF): the global constructor entry listed in the constructor table at 0x43CE88.
 *
 * __sti__all_in_one_sound_cpp is the static initializer in sound/icepick/worldtriggermanager; (1, 0xFFFF) asks it to initialise at
 * the default priority.
 */
extern "C" void func_002BAEE8(void) {
    __sti__all_in_one_sound_cpp(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterLayer_resolve);
#ifdef SKIP_ASM
void cQuadTree_resolve(void *);
/**
 * Relocates a loaded world-painter layer in place: turns its file offsets into pointers.
 *
 * m_iQuadTreeOffset becomes m_pQuadTree (this + offset), which cQuadTree_resolve then resolves; m_iRegionOffset becomes
 * the region array pointer; each of the m_iNumRegions regions' m_iDataOffset becomes m_pData (this + offset), or 0
 * when the offset is tWPRegion::kNoOffset (-1).
 */
void cWorldPainterLayer::resolve()
{
    // ORIGINAL-PTR: a loaded file blob: the layer's quad tree, region table and region data sit at byte offsets from
    // the layer ([P] "m_iQuadTreeOffset >= 0", "m_iRegionOffset >= 0")
    m_pQuadTree = (cQuadTree*)((char*)this + m_iQuadTreeOffset);
    cQuadTree_resolve(m_pQuadTree);
    m_pRegions = (tWPRegion*)((char*)this + m_iRegionOffset);
    tWPRegion* region = m_pRegions;
    int i;
    for (i = 0; i < m_iNumRegions; i++, region++) {
        int off = region->m_iDataOffset;
        if (off != tWPRegion::kNoOffset) {
            region->m_pData = (const tWPData*)((char*)this + off);
        } else {
            region->m_pData = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterLayer_getQuadTreeContents);
#ifdef SKIP_ASM
extern "C" tQuadTreeLeafNode *cQuadTree_getLeaf(int);

/**
 * Returns the contents word of the quad-tree leaf at a position in a world-painter layer.
 *
 * Calls cQuadTree_getLeaf on the layer's quad tree (its first word); the position is not declared here and passes
 * through in $f12/$f13. Returns the leaf's second word, the region index cWorldPainterQuery_lookupRegion uses (-1 for
 * none).
 *
 * @param layer  the world-painter layer
 * @return the leaf's contents (a region index, or -1)
 */
extern "C" int cWorldPainterLayer_getQuadTreeContents(int *layer) {
    return cQuadTree_getLeaf(*layer)->m_key;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAFB0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Placement new for constructing the key table in place.
inline void* operator new(unsigned int, void* p) { return p; }

// 8-byte key: a byte id at offset 0, overlaid by the packed word id | (index << 8).
struct sKey_AFB0 {
    union {
        unsigned char id;
        int packed;
    };
    int pad;

    sKey_AFB0(int n)
    {
        id = n;
        packed = (packed & 0xFF) | (n << 8);
    }
};

// Zeroed 8-byte header placed before the key table.
struct sPair_AFB0 {
    int a;
    int b;

    sPair_AFB0()
    {
        b = 0;
        a = 0;
    }
};

extern sPair_AFB0 D_004A52E0;
extern sKey_AFB0 D_004A52E8;
extern sKey_AFB0 D_004A52F0;
extern sKey_AFB0 D_004A52F8;
extern sKey_AFB0 D_004A5300;
extern sKey_AFB0 D_004A5308;
extern sKey_AFB0 D_004A5310;
extern sKey_AFB0 D_004A5318;
extern sKey_AFB0 D_004A5320;
extern sKey_AFB0 D_004A5328;
extern sKey_AFB0 D_004A5330;
extern sKey_AFB0 D_004A5338;
extern sKey_AFB0 D_004A5340;
extern sKey_AFB0 D_004A5348;
extern sKey_AFB0 D_004A5350;

/**
 * Constructs a static table of fourteen 8-byte keys (D_004A52E8..D_004A5350) and zeroes its header D_004A52E0.
 *
 * Runs only when @p initialize is set and @p priority is DEFAULT_INIT_PRIORITY. Key n (0-13) gets n in its low byte and
 * n << 8 in the rest of its first word. func_002BB0E0 calls it from the constructor table.
 *
 * @param initialize  nonzero to construct (0 would be destruction; nothing is done then)
 * @param priority  the init priority; only DEFAULT_INIT_PRIORITY does anything
 */
// Static initializer (initialize, priority) for the key table.
extern "C" void func_002BAFB0(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        new (&D_004A52E0) sPair_AFB0();
        new (&D_004A52E8) sKey_AFB0(0);
        new (&D_004A52F0) sKey_AFB0(1);
        new (&D_004A52F8) sKey_AFB0(2);
        new (&D_004A5300) sKey_AFB0(3);
        new (&D_004A5308) sKey_AFB0(4);
        new (&D_004A5310) sKey_AFB0(5);
        new (&D_004A5318) sKey_AFB0(6);
        new (&D_004A5320) sKey_AFB0(7);
        new (&D_004A5328) sKey_AFB0(8);
        new (&D_004A5330) sKey_AFB0(9);
        new (&D_004A5338) sKey_AFB0(10);
        new (&D_004A5340) sKey_AFB0(11);
        new (&D_004A5348) sKey_AFB0(12);
        new (&D_004A5350) sKey_AFB0(13);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BB0E0);
#ifdef SKIP_ASM
extern "C" void func_002BAFB0(int, int);

/**
 * Runs func_002BAFB0(1, 0xFFFF): the global constructor entry listed in the constructor table at 0x43CE88.
 */
extern "C" void func_002BB0E0(void) {
    func_002BAFB0(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB100);
#ifdef SKIP_ASM
/**
 * Returns whether two world-painter name fields differ: 1 when either of the two words of m_nameId differs.
 *
 * @param wpNameField  the name field
 * @param rhs  the name field to compare with
 * @return 1 when they differ, else 0
 */
extern "C" int func_002BB100(tWPNameField* wpNameField, tWPNameField* rhs) {
    int var_6;

    var_6 = 0;
    if (((wpNameField->m_nameId[0]) != (rhs->m_nameId[0])) || ((wpNameField->m_nameId[1]) != (rhs->m_nameId[1]))) {
        var_6 = 1;
    }
    return var_6;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_cWorldPainterMan);
#ifdef SKIP_ASM
/**
 * Constructs the world painter manager: clears the word at +0x4.
 *
 * @return @p self
 */
extern "C" void *cWorldPainterMan_cWorldPainterMan(cWorldPainterMan* self) {
    self->m_pCache = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan__cWorldPainterMan);
#ifdef SKIP_ASM
void cMemMan_free(void *);
void operator_delete(int *);
extern "C" void cWorldPainterSection__cWorldPainterSection(cWorldPainterSection*, int);
extern "C" void *cWorldPainterSection_cWorldPainterSection(cWorldPainterSection* arg0);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
int operator_new_B1D0(unsigned int, void*, int, int) __asm__("operator_new__FUi");

// cWorldPainterSection's constructor and destructor as new[] / delete[] inline them per element (ctor and dtor only).
// operator delete[] frees the block (the array cookie sits 0x10 bytes before the sections).
struct cWorldPainterSectionArr : cWorldPainterSection {
    cWorldPainterSectionArr() { cWorldPainterSection_cWorldPainterSection(this); }
    ~cWorldPainterSectionArr() { cWorldPainterSection__cWorldPainterSection(this, 0); }
    void* operator new[](unsigned int size, char* tag, int flags, int d)
    {
        return (void*)operator_new_B1D0(size, tag, flags, d);
    }
    void operator delete[](void* p) { cMemMan_free(p); }
};

/**
 * Destroys the world-painter manager: destroys and frees its section cache.
 *
 * Runs cWorldPainterSection__cWorldPainterSection (flags 0) on each element of m_pCache, last first, using the new[]
 * count cookie 0x10 bytes before the array, then frees the block with cMemMan_free.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object itself (operator delete)
 */
extern "C" void cWorldPainterMan__cWorldPainterMan(cWorldPainterMan* self, int flags) {
    // Same-layout cast: cWorldPainterSectionArr only adds the constructor and destructor.
    delete[] (cWorldPainterSectionArr*)self->m_pCache;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_init);
#ifdef SKIP_ASM
extern char str_cWorldPainterMan[];

/**
 * Sizes the section cache for a world: frees any old array and allocates @p n fresh sections.
 *
 * Destroys and frees the old m_pCache as the destructor does, stores @p n in m_uiNumSections and allocates n * 12 +
 * 0x10 bytes tagged "cWorldPainterMan" (count cookie in the first word), constructing each 12-byte section with
 * cWorldPainterSection_cWorldPainterSection (the cWorldPainterSection constructor).
 *
 * @param n  the number of sections (On Tour iNumSections)
 */
// Reallocates the 12-byte element array (delete[] then new[] with a count cookie).
void cWorldPainterMan::init(int n)
{
    // Same-layout cast: cWorldPainterSectionArr only adds the constructor and destructor.
    delete[] (cWorldPainterSectionArr*)m_pCache;
    m_uiNumSections = (unsigned int)n;
    cWorldPainterSection*& cache = m_pCache;
    cache = new (str_cWorldPainterMan, 0x20000000, 0) cWorldPainterSectionArr[n];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_update);
#ifdef SKIP_ASM
extern "C" void cWorldPainterSection_update(cWorldPainterSection*);
/**
 * Updates every world-painter section: calls cWorldPainterSection_update on each of the m_uiNumSections entries of m_pCache.
 */
void cWorldPainterMan::update()
{
    unsigned i;
    for (i = 0; i < *(unsigned*)this; i++) {
        m_pCache[i].update();
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_load);
#ifdef SKIP_ASM
extern "C" void cWorldPainterSection_load(cWorldPainterSection* section, int fileName);

/**
 * Starts loading a section's world-painter file: m_pCache[@p sectionId].load(@p fileName) via cWorldPainterSection_load.
 *
 * @param sectionId  the section index into m_pCache (12-byte cWorldPainterSection entries)
 * @param fileName  the file name (a cBXString, passed through to cWorldPainterSection_load)
 */
void cWorldPainterMan::load(int sectionId, int fileName)
{
    cWorldPainterSection_load(&m_pCache[sectionId], fileName);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_load1);
#ifdef SKIP_ASM
extern "C" void cWorldPainterSection_load1(cWorldPainterSection* section, int sectionId, tWPHeader* wpData);
/**
 * Installs already-loaded world-painter data for a section: m_pCache[@p sectionId] gets it through cWorldPainterSection_load1.
 *
 * @param sectionId  the section index into m_pCache
 * @param wpData  the section's world-painter data (a tWPHeader), kept in place (streamed)
 */
void cWorldPainterMan::load1(int sectionId, tWPHeader* wpData)
{
    m_pCache[sectionId].load1(sectionId, wpData);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_cWorldPainterSection);
#ifdef SKIP_ASM
/**
 * Constructs a world-painter section: m_pWPHeader 0, m_iAsyncHandle -1, m_iState (kWPSection_Unloaded) and m_iStreamed
 * 0.
 *
 * @param section  the section
 * @return @p section
 */
extern "C" void *cWorldPainterSection_cWorldPainterSection(cWorldPainterSection* section) {
    section->m_pWPHeader = 0;
    section->m_iAsyncHandle = -1;
    section->m_iState = 0;
    section->m_iStreamed = 0;
    return ((void*)section);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection__cWorldPainterSection);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
/**
 * Destroys a world-painter section: frees m_pWPHeader unless it is null or streamed data (m_iStreamed set).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object itself (operator delete)
 */
extern "C" void cWorldPainterSection__cWorldPainterSection(cWorldPainterSection* self, int flags) {
    void *wpHeader = self->m_pWPHeader;
    if (wpHeader != 0) {
        if (self->m_iStreamed == 0) {
            cMemMan_free(wpHeader);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void ASYNCFILE_release_B3F0(int handle, void** data, int* size) __asm__("ASYNCFILE_release");
extern "C" void MEM_free_B3F0(void* p) __asm__("MEM_free");
extern "C" void unpack_B3F0(void* src, void* dst) __asm__("unpack");
extern "C" int unpacksizez_B3F0(void* src) __asm__("unpacksizez");
extern "C" int ASYNCFILE_getstatus_B3F0(int) __asm__("ASYNCFILE_getstatus");
extern "C" void MEM_copy_B3F0(void* dst, void* src, int size) __asm__("MEM_copy");
void* operator_new_B3F0(unsigned int, void*, int, int) __asm__("operator_new__FUi");
extern char str_WPFile[];
/**
 * Finishes a section's file load once its async read completes (m_iState kWPSection_Loading).
 *
 * When ASYNCFILE_getstatus on m_iAsyncHandle returns 1 (the same value), releases the read (ASYNCFILE_release gives its
 * data and size) and, for a non-empty read, allocates m_pWPHeader with operator new (tag "WPFile", str_WPFile):
 * decompressed with unpack when unpacksizez reports a packed size, otherwise copied with MEM_copy; the read buffer is
 * then freed (MEM_free). Sets m_iState to 2 (kWPSection_Loaded) either way, so a failed read leaves m_pWPHeader 0.
 *
 */
void cWorldPainterSection::update()
{
    short st = m_iState;
    if (st == kWPSection_Loading && ASYNCFILE_getstatus_B3F0(m_iAsyncHandle) == st) {
        void* packData = 0;
        int packSize = 0;
        ASYNCFILE_release_B3F0(m_iAsyncHandle, &packData, &packSize);
        if (packData != 0) {
            if (packSize > 0) {
                int unpackedSize = unpacksizez_B3F0(packData);
                if (unpackedSize > 0) {
                    void* header = operator_new_B3F0(unpackedSize, str_WPFile, 0x20000000, 0);
                    m_pWPHeader = (tWPHeader*)header;
                    unpack_B3F0(packData, header);
                } else {
                    void* header = operator_new_B3F0(packSize, str_WPFile, 0x20000000, 0);
                    m_pWPHeader = (tWPHeader*)header;
                    MEM_copy_B3F0(header, packData, packSize);
                }
                MEM_free_B3F0(packData);
            }
        }
        m_iState = 2;
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_load);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sBXStr_B4D8 { char *p; int pad[3]; sBXStr_B4D8() {} };
extern "C" void func_003186D0_B4D8(sBXStr_B4D8*, const char*, int) __asm__("func_003186D0");
extern "C" void func_00318630_B4D8(sBXStr_B4D8*, sBXStr_B4D8*, const char*) __asm__("func_00318630");
extern "C" int ASYNCFILE_load_B4D8(const char*, int) __asm__("ASYNCFILE_load");
extern "C" void cBXString_dt_B4D8(sBXStr_B4D8*, int) __asm__("cBXString__cBXString");
extern char str_bar_d[];
extern char str_swp[];
/**
 * Starts an asynchronous load of a section's world-painter file.
 *
 * Clears m_iStreamed, builds the path str_bar_d + the name (func_003186D0), appends str_swp (".swp", func_00318630)
 * and opens it with ASYNCFILE_load; the handle goes to m_iAsyncHandle. m_iState becomes 1 (loading, finished by
 * cWorldPainterSection_update) for a positive handle, else 2 (kWPSection_Loaded, with no data).
 *
 * @param fileName  the file name (a cBXString)
 */
void cWorldPainterSection::load(int fileName)
{
    sBXStr_B4D8 swpPath;
    sBXStr_B4D8 basePath;
    m_iStreamed = 0;
    func_003186D0_B4D8(&basePath, str_bar_d, fileName);
    func_00318630_B4D8(&swpPath, &basePath, str_swp);
    m_iAsyncHandle = ASYNCFILE_load_B4D8(swpPath.p, 0);
    cBXString_dt_B4D8(&swpPath, 2);
    cBXString_dt_B4D8(&basePath, 2);
    int n = m_iAsyncHandle;
    m_iState = n <= 0 ? 2 : 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_load1);
#ifdef SKIP_ASM
extern "C" void cWorldPainterSection_resolve(cWorldPainterSection*);

/**
 * Installs already-loaded data in a section and resolves it.
 *
 * Sets m_iStreamed (the section does not own the data), m_iState to 2 (kWPSection_Loaded) and m_pWPHeader to
 * @p wpData, then relocates it with cWorldPainterSection_resolve. @p sectionId is not used.
 *
 * @param sectionId  the section index (unused)
 * @param wpData  the world-painter data (a tWPHeader)
 */
void cWorldPainterSection::load1(int sectionId, tWPHeader* wpData)
{
    m_iStreamed = 1;
    m_iState = 2;
    m_pWPHeader = wpData;
    resolve();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_unload);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Unloads a section: frees m_pWPHeader with cMemMan_free unless m_iStreamed is set, clears it, and sets m_iState to 0.
 *
 */
void cWorldPainterSection::unload()
{
    tWPHeader* header;

    header = m_pWPHeader;
    if (header != 0) {
        if ((m_iStreamed) == 0) {
            cMemMan_free(header);
        }
        m_pWPHeader = 0;
    }
    m_iState = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterSection_resolve);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cWorldPainterLayer_resolve_B5F0(int p) __asm__("cWorldPainterLayer_resolve");

struct sReloc_B5F0 {
    int magic;          // 0x0 (0x10 once relocated)
    int count;          // 0x4
    int offs[1];        // 0x8
};


/**
 * Relocates a section's loaded data and marks it resolved.
 *
 * A header whose version word is not tWPHeader::kVersion is dropped (freed with cMemMan_free unless m_iStreamed). For a
 * valid header, each of its layer slots becomes header + offset and is resolved with cWorldPainterLayer_resolve, or
 * becomes 0 when the offset is tWPRegion::kNoOffset. m_iState is set to kWPSection_Resolved in every case, even with
 * no data.
 *
 * @param vself  the section
 */
// Relocates the loaded block's offset table (dropping it if it has the wrong magic) and marks it ready.
void cWorldPainterSection::resolve()
{
    cWorldPainterSection* self = this;
    if (((sReloc_B5F0*)self->m_pWPHeader) == 0) {
        self->m_iState = kWPSection_Resolved;
        return;
    }
    {
        if (((sReloc_B5F0*)self->m_pWPHeader)->magic != tWPHeader::kVersion) {
            if (self->m_iStreamed == 0) {
                cMemMan_free(((sReloc_B5F0*)self->m_pWPHeader));
            }
            self->m_pWPHeader = 0;
        }
        if (((sReloc_B5F0*)self->m_pWPHeader) != 0) {
            int* layerPtr = ((sReloc_B5F0*)self->m_pWPHeader)->offs;
            for (int i = 0; i < ((sReloc_B5F0*)self->m_pWPHeader)->count; i++, layerPtr++) {
                if (*layerPtr == tWPRegion::kNoOffset) {
                    *layerPtr = 0;
                } else {
                    *layerPtr = (int)((sReloc_B5F0*)self->m_pWPHeader) + *layerPtr;
                    cWorldPainterLayer_resolve_B5F0(*layerPtr);
                }
            }
        }
    }
    self->m_iState = kWPSection_Resolved;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterManRender);
#ifdef SKIP_ASM
extern "C" void cWorldPainterMan_render(cWorldPainterMan* man, void* world);

/**
 * Draws the world painter debug overlay of the current world.
 *
 * Calls cWorldPainterMan_render (cWorldPainterMan::render) on the painter manager stored at +0x8 of SSXApp->Game->World.
 */
extern "C" void cWorldPainterManRender(void) {
    cWorld* temp_5;

    temp_5 = (cWorld*)((void*)SSXApp->Game->World);
    cWorldPainterMan_render(temp_5->m_pWorldPainterMan, temp_5);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// cWorldPainterMan::render() (On Tour name and locals): draws the active painter layer's quad tree.
#define MIN_B6E0(a, b) ((a) < (b) ? (a) : (b))
#define MAX_B6E0(a, b) ((a) > (b) ? (a) : (b))
struct cGraphicsMan_B6E0 { char pad[0xE84]; sDrawState* m_pCurrentDrawState; };
struct tBxVertex_B6E0 {
    tBxVertex_B6E0() {}
    float x, y, z, w;
    int r, g, b, a;
    int pad[4];
};
// t4Vector with On Tour's t4vect accessors (x() ... w() return float&); the ctor stores through them.
struct t4Vector_B6E0 { float m_x, m_y, m_z, m_w;
    float& x() { return m_x; } float& y() { return m_y; } float& z() { return m_z; } float& w() { return m_w; }
    t4Vector_B6E0() {}
    t4Vector_B6E0(float ix, float iy, float iz, float iw) { x() = ix; y() = iy; z() = iz; w() = iw; } } __attribute__((aligned(16)));
struct cQuadTree_B6E0 { float scale; float minx; float miny; char pad0C[8]; unsigned short root; char pad16[10]; char* nodes; };
struct tWPLayer_B6E0 { cQuadTree_B6E0* tree; };
struct tWPHeader_B6E0 { int pad0; int numLayers; tWPLayer_B6E0* layers[1]; };
struct cWorldPainterSection_B6E0 { int pad0; int pad4; tWPHeader_B6E0* m_pWPHeader; };
struct cWorldPainterMan_B6E0 { unsigned int m_uiNumSections; cWorldPainterSection_B6E0* m_pCache; };

extern int cVisualEffectsTest_m_Enable;   // cVisualEffectsTest::m_Enable
extern int cWorldPainterMan_m_iCurrSection;   // cWorldPainterMan::m_iCurrSection
extern int cVisualEffectsTest_m_LayerId;   // cVisualEffectsTest::m_LayerId
static inline int layerId_B6E0() { return cVisualEffectsTest_m_LayerId; }
extern sDrawState sDrawState_s_defaultDrawState;  // sDrawState::s_defaultDrawState
// cWorldPainterMan::render(cQuadTree*, const tQuadTreeNode*, tBxVertex*, t4Vector&, t4Vector&)
extern "C" void cWorldPainterMan_render1_B6E0(cWorldPainterMan_B6E0*, cQuadTree_B6E0*, char*, tBxVertex_B6E0*, t4Vector_B6E0*, t4Vector_B6E0*) __asm__("cWorldPainterMan_render1");

extern "C" void cWorldPainterMan_render_impl(cWorldPainterMan_B6E0* self) __asm__("cWorldPainterMan_render");
/**
 * Debug-draws the quad tree of the current section's selected world-painter layer.
 *
 * Only while cVisualEffectsTest_m_Enable is set. Takes m_pCache[cWorldPainterMan_m_iCurrSection] and its layer
 * cVisualEffectsTest_m_LayerId (clamped to the header's layer count). If that layer has a quad tree, pushes a default
 * draw state on cGraphicsMan_TheGraphicsMan, fills five green opaque vertices, builds the tree's corner rectangle
 * (min = the tree's min x/y; max = min + 32768 / scale, all truncated to whole numbers) and walks the tree from its
 * root with cWorldPainterMan_render1, then pops the draw state.
 */
extern "C" void cWorldPainterMan_render_impl(cWorldPainterMan_B6E0* self) {
    if (cVisualEffectsTest_m_Enable) {
        cWorldPainterSection_B6E0* section = &self->m_pCache[cWorldPainterMan_m_iCurrSection];
        if (section) {
            tWPLayer_B6E0** layers = section->m_pWPHeader->layers;
            if (layers) {
                tWPLayer_B6E0* layer = layers[MIN_B6E0(MAX_B6E0(layerId_B6E0(), 0), section->m_pWPHeader->numLayers - 1)];
                if (layer) {
                    cQuadTree_B6E0* tree = layer->tree;
                    if (tree) {
                        cPS2GraphicsMan* gm = cGraphicsMan_TheGraphicsMan;
                        // PushDrawState + SetDefaultDrawState
                        (gm->m_pCurrentDrawState)[1] = (gm->m_pCurrentDrawState)[0];
                        (gm->m_pCurrentDrawState)++;
                        *(gm->m_pCurrentDrawState) = sDrawState_s_defaultDrawState;
                        tBxVertex_B6E0 quadVerts[5];
                        tBxVertex_B6E0* quadVert;
                        int i;
                        for (i = 0; i < 5; i++) {
                            quadVert = &quadVerts[i];
                            quadVert->a = 255;
                            quadVert->r = 0;
                            quadVert->g = 255;
                            quadVert->b = 0;
                            quadVert->z = 1.0f;
                            quadVert->w = 0;
                            quadVert->x = 0;
                            quadVert->y = 0;
                        }
                        t4Vector_B6E0 mincorner((float)(int)tree->minx, (float)(int)tree->miny, 0.0f, 1.0f);
                        t4Vector_B6E0 maxcorner((float)(int)(32768.0f / tree->scale + tree->minx),
                                                (float)(int)(32768.0f / tree->scale + tree->miny), 0.0f, 1.0f);
                        cWorldPainterMan_render1_B6E0(self, tree, tree->nodes + tree->root * 8, quadVerts, &mincorner, &maxcorner);
                        (gm->m_pCurrentDrawState)--;  // PopDrawState
                    }
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", cWorldPainterMan_render1);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector qtAdd_B928(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector qtDiv_B928(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Draws a world-painter quad tree recursively: each leaf's rectangle as a five-point outline.
 *
 * An inner node (bit 0 of its first child word set) splits the rectangle (@p lo, @p hi) at its midpoint and recurses
 * into its four children (child index = word >> 1): lower-left (lo, mid), the left/top quarter, the bottom/right
 * quarter and upper-right (mid, hi). A leaf writes the rectangle's four corners plus the first again into @p verts at the
 * height (z) of the first AI rider's position (SSXApp->Game->AI; cBeing::getPosition, virtual slot 5 of its cBeing
 * base at +0x6C0), and submits the five vertices through virtual slot 73 of cGraphicsMan_TheGraphicsMan.
 *
 * @param ctx  the world-painter manager (passed down, otherwise unused)
 * @param tree  the quad tree (its node array m_nodes)
 * @param node  the node to draw
 * @param verts  the five-vertex scratch array (positions at +0x20 of each vertex)
 * @param lo  the rectangle's minimum corner
 * @param hi  the rectangle's maximum corner
 */
// Walks a quadtree: inner nodes split the rect (lo, hi) at its midpoint into four children;
// leaves submit the rect's outline as a 5-vertex strip at the camera's height.
void cWorldPainterMan::render1(cQuadTree* tree, tQuadTreeNode* node, tBxVertex* verts, t4Vector* lo, t4Vector* hi)
{
    if (node->m_bits.m_flags[0] & 1) {
        t4Vector mid = qtDiv_B928(qtAdd_B928(*lo, *hi), 2.0f);
        t4Vector leftEdgeMid;
        leftEdgeMid.x = lo->x;
        leftEdgeMid.y = mid.y;
        leftEdgeMid.z = 0.0f;
        leftEdgeMid.w = 1.0f;
        t4Vector rightEdgeMid;
        rightEdgeMid.x = hi->x;
        rightEdgeMid.y = mid.y;
        rightEdgeMid.z = 0.0f;
        rightEdgeMid.w = 1.0f;
        t4Vector bottomEdgeMid;
        bottomEdgeMid.x = mid.x;
        bottomEdgeMid.y = lo->y;
        bottomEdgeMid.z = 0.0f;
        bottomEdgeMid.w = 1.0f;
        t4Vector topEdgeMid;
        topEdgeMid.x = mid.x;
        topEdgeMid.y = hi->y;
        topEdgeMid.z = 0.0f;
        topEdgeMid.w = 1.0f;
        render1(tree, (tree->m_nodes) + (node->m_sub.m_children[0] >> 1), verts, lo, &mid);
        render1(tree, (tree->m_nodes) + (node->m_sub.m_children[1] >> 1), verts, &leftEdgeMid,
                                 &topEdgeMid);
        render1(tree, (tree->m_nodes) + (node->m_sub.m_children[2] >> 1), verts, &bottomEdgeMid,
                                 &rightEdgeMid);
        render1(tree, (tree->m_nodes) + (node->m_sub.m_children[3] >> 1), verts, &mid, hi);
    } else {
        cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
        cRider* rider = SSXApp->Game->AI->Players[0]->mRider;
        cBeing& being = *rider;
        t4Vector cam = *being.getPosition();
        t4Vector corner;
        corner.x = lo->x;
        corner.y = lo->y;
        corner.z = cam.z;
        corner.w = 1.0f;
        (*(&verts[0].p)) = corner;
        corner.x = hi->x;
        corner.y = lo->y;
        corner.z = cam.z;
        corner.w = 1.0f;
        (*(&verts[1].p)) = corner;
        corner.x = hi->x;
        corner.y = hi->y;
        corner.z = cam.z;
        corner.w = 1.0f;
        (*(&verts[2].p)) = corner;
        corner.x = lo->x;
        corner.y = hi->y;
        corner.z = cam.z;
        corner.w = 1.0f;
        (*(&verts[3].p)) = corner;
        corner.x = lo->x;
        corner.y = lo->y;
        corner.z = cam.z;
        corner.w = 1.0f;
        (*(&verts[4].p)) = corner;
        ps2GraphicsMan->RenderLineStrip(5, verts, 0);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BBBF8);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (a varargs stub, like a compiled-out debug print).
 *
 * @param first  the value returned
 * @return @p first
 */
extern "C" int func_002BBBF8(int first, ...) {
    return first;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BBC38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sColor_2BBC38 {
    float r, g, b, a;
    sColor_2BBC38(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};


struct sFlags_2BBC38 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_2BBC38(unsigned char aa, unsigned int ab) : a(aa), b(ab) {}
};

extern int D_004A5358;
extern int D_004A535C;
extern sFlags_2BBC38 D_004A5360;
extern sFlags_2BBC38 D_004A5368;
extern sFlags_2BBC38 D_004A5370;
extern sFlags_2BBC38 D_004A5378;
extern sFlags_2BBC38 D_004A5380;
extern sFlags_2BBC38 D_004A5388;
extern sFlags_2BBC38 D_004A5390;
extern sFlags_2BBC38 D_004A5398;
extern sFlags_2BBC38 D_004A53A0;
extern sFlags_2BBC38 D_004A53A8;
extern sFlags_2BBC38 D_004A53B0;
extern sFlags_2BBC38 D_004A53B8;
extern sFlags_2BBC38 D_004A53C0;
extern sFlags_2BBC38 D_004A53C8;

extern sColor_2BBC38 D_004D42B8;
extern sColor_2BBC38 D_004D42C8;
extern sColor_2BBC38 D_004D42D8;
extern sColor_2BBC38 D_004D42E8;
extern sColor_2BBC38 D_004D42F8;
extern sColor_2BBC38 D_004D4308;
extern sColor_2BBC38 D_004D4318;
extern sColor_2BBC38 D_004D4328;
extern sColor_2BBC38 D_004D4338;
extern sColor_2BBC38 D_004D4348;
extern sColor_2BBC38 D_004D4358;
extern sColor_2BBC38 D_004D4368;
extern sColor_2BBC38 D_004D4378;
extern sColor_2BBC38 D_004D4388;
extern sColor_2BBC38 D_004D4398;
extern sColor_2BBC38 D_004D43A8;
extern sColor_2BBC38 D_004D43B8;
extern sColor_2BBC38 D_004D43C8;
extern sColor_2BBC38 D_004D43D8;
extern sColor_2BBC38 D_004D43E8;
extern sColor_2BBC38 D_004D43F8;
extern sColor_2BBC38 D_004D4408;
extern sColor_2BBC38 D_004D4418;
extern sColor_2BBC38 D_004D4428;
extern sColor_2BBC38 D_004D4438;
extern sColor_2BBC38 D_004D4448;
extern sColor_2BBC38 D_004D4458;
extern sColor_2BBC38 D_004D4468;
extern sColor_2BBC38 D_004D4478;
extern sColor_2BBC38 D_004D4488;
extern sColor_2BBC38 D_004D4498;
extern sColor_2BBC38 D_004D44A8;
extern sColor_2BBC38 D_004D44B8;
extern sColor_2BBC38 D_004D44C8;
extern sColor_2BBC38 D_004D44D8;
extern sColor_2BBC38 D_004D44E8;
extern sColor_2BBC38 D_004D44F8;
extern sColor_2BBC38 D_004D4508;
extern sColor_2BBC38 D_004D4518;
extern sColor_2BBC38 D_004D4528;
extern sColor_2BBC38 D_004D4538;
extern sColor_2BBC38 D_004D4548;
extern sColor_2BBC38 D_004D4558;
extern sColor_2BBC38 D_004D4568;
extern sColor_2BBC38 D_004D4578;
extern sColor_2BBC38 D_004D4588;
extern sColor_2BBC38 D_004D4598;
extern sColor_2BBC38 D_004D45A8;
extern sColor_2BBC38 D_004D45B8;
extern sColor_2BBC38 D_004D45C8;
extern sColor_2BBC38 D_004D45D8;
extern sColor_2BBC38 D_004D45E8;
extern sColor_2BBC38 D_004D45F8;
extern sColor_2BBC38 D_004D4608;
extern sColor_2BBC38 D_004D4618;
extern sColor_2BBC38 D_004D4628;

// The unit declares func_002BBC38 later as returning void*; bind this body by asm label.
extern "C" void func_002BBC38_impl(int initialize, int priority) __asm__("func_002BBC38");

/**
 * Constructs the world-painter manager unit's static constants (its static initializer).
 *
 * Runs only when @p initialize is set and @p priority is 0xFFFF. Zeroes the 8-byte name field at D_004A5358 (On Tour
 * has such per-unit copies of kNullNameField), builds the fourteen 8-byte region-type keys D_004A5360..D_004A53C8 with
 * ids 0-13 (the kWPR_MusicTrigger..kWPR_Danger copies in On Tour's layout) and constructs 56 float4 colour
 * constants D_004D42B8..D_004D4628 from literal values. func_002BC4E0 calls it from the constructor table.
 *
 * @param initialize  nonzero to construct (nothing is done for 0)
 * @param priority  the init priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void func_002BBC38_impl(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        D_004A535C = 0;
        D_004A5358 = 0;
        new (&D_004A5360) sFlags_2BBC38(0, 0);
        new (&D_004A5368) sFlags_2BBC38(1, 1);
        new (&D_004A5370) sFlags_2BBC38(2, 2);
        new (&D_004A5378) sFlags_2BBC38(3, 3);
        new (&D_004A5380) sFlags_2BBC38(4, 4);
        new (&D_004A5388) sFlags_2BBC38(5, 5);
        new (&D_004A5390) sFlags_2BBC38(6, 6);
        new (&D_004A5398) sFlags_2BBC38(7, 7);
        new (&D_004A53A0) sFlags_2BBC38(8, 8);
        new (&D_004A53A8) sFlags_2BBC38(9, 9);
        new (&D_004A53B0) sFlags_2BBC38(10, 10);
        new (&D_004A53B8) sFlags_2BBC38(11, 11);
        new (&D_004A53C0) sFlags_2BBC38(12, 12);
        new (&D_004A53C8) sFlags_2BBC38(13, 13);
        new (&D_004D42B8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D42C8) sColor_2BBC38(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D42D8) sColor_2BBC38(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D42E8) sColor_2BBC38(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D42F8) sColor_2BBC38(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D4308) sColor_2BBC38(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4318) sColor_2BBC38(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D4328) sColor_2BBC38(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D4338) sColor_2BBC38(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D4348) sColor_2BBC38(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D4358) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4368) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4378) sColor_2BBC38(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D4388) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4398) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D43A8) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D43B8) sColor_2BBC38(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D43C8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D43D8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D43E8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D43F8) sColor_2BBC38(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4408) sColor_2BBC38(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D4418) sColor_2BBC38(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D4428) sColor_2BBC38(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D4438) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4448) sColor_2BBC38(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D4458) sColor_2BBC38(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D4468) sColor_2BBC38(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D4478) sColor_2BBC38(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D4488) sColor_2BBC38(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D4498) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44A8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44B8) sColor_2BBC38(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D44C8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44D8) sColor_2BBC38(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D44E8) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D44F8) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4508) sColor_2BBC38(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4518) sColor_2BBC38(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D4528) sColor_2BBC38(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D4538) sColor_2BBC38(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D4548) sColor_2BBC38(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D4558) sColor_2BBC38(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4568) sColor_2BBC38(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D4578) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4588) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4598) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D45A8) sColor_2BBC38(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D45B8) sColor_2BBC38(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D45C8) sColor_2BBC38(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D45D8) sColor_2BBC38(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D45E8) sColor_2BBC38(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D45F8) sColor_2BBC38(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D4608) sColor_2BBC38(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D4618) sColor_2BBC38(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D4628) sColor_2BBC38(1.0f, 1.0f, 0.0f, 0.0f);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BC4E0);
#ifdef SKIP_ASM
extern "C" void func_002BBC38(int, int);

/**
 * Runs func_002BBC38(1, 0xFFFF): a global constructor entry.
 */
extern "C" void func_002BC4E0(void) {
    func_002BBC38(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Type);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default Type, vtable slot 1 (tWPInGameData_MusicTrigger overrides it with tWPInGameData_MusicTrigger_Type).
 *
 * @return 0
 */
extern "C" int tWPInGameData_Type(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Value);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default Value, vtable slot 2 (tWPInGameData_MusicTrigger overrides it with tWPInGameData_MusicTrigger_Value).
 *
 * @return 0
 */
extern "C" int tWPInGameData_Value(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MixPreset);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default MixPreset, vtable slot 3 (tWPInGameData_Mix overrides it with tWPInGameData_Mix_MixPreset).
 *
 * @return 0
 */
extern "C" int tWPInGameData_MixPreset(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_BankID);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default BankID, vtable slot 4 (tWPInGameData_Ambience overrides it with tWPInGameData_Ambience_BankID).
 *
 * @return 0
 */
extern "C" int tWPInGameData_BankID(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SpeechID);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SpeechID, vtable slot 5 (tWPInGameData_Speech overrides it with tWPInGameData_Speech_SpeechID).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SpeechID(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_DrawDistance);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default DrawDistance, vtable slot 6 (tWPInGameData_Camera overrides it with
 * tWPInGameData_Camera_DrawDistance).
 *
 * @return 0
 */
extern "C" int tWPInGameData_DrawDistance(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogDensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogDensity, vtable slot 7 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogDensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogDensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogNear);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogNear, vtable slot 8 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogNear).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogNear(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogFar);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogFar, vtable slot 9 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogFar).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogFar(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogColourR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogColourR, vtable slot 10 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogColourR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogColourR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogColourG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogColourG, vtable slot 11 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogColourG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogColourG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FogColourB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FogColourB, vtable slot 12 (tWPInGameData_Fog overrides it with tWPInGameData_Fog_FogColourB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FogColourB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowCutoff);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowCutoff, vtable slot 13 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowCutoff).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowCutoff(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowCutoffScale);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowCutoffScale, vtable slot 14 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowCutoffScale).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowCutoffScale(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowCopyIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowCopyIntensity, vtable slot 15 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowCopyIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowCopyIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowSourceIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowSourceIntensity, vtable slot 16 (tWPInGameData_LightGlow overrides it
 * with tWPInGameData_LightGlow_LightGlowSourceIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowSourceIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowFrameIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowFrameIntensity, vtable slot 17 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowFrameIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowFrameIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowBlend2);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowBlend2, vtable slot 18 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowBlend2).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowBlend2(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlowBlend3);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightGlowBlend3, vtable slot 19 (tWPInGameData_LightGlow overrides it with
 * tWPInGameData_LightGlow_LightGlowBlend3).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightGlowBlend3(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintScaleR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintScaleR, vtable slot 20 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintScaleR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintScaleR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintScaleG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintScaleG, vtable slot 21 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintScaleG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintScaleG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintScaleB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintScaleB, vtable slot 22 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintScaleB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintScaleB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintOffsetR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintOffsetR, vtable slot 23 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintOffsetR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintOffsetR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintOffsetG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintOffsetG, vtable slot 24 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintOffsetG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintOffsetG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTintOffsetB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ScreenTintOffsetB, vtable slot 25 (tWPInGameData_ScreenTint overrides it with
 * tWPInGameData_ScreenTint_ScreenTintOffsetB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ScreenTintOffsetB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBoxRotation);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SkyBoxRotation, vtable slot 26 (tWPInGameData_SkyBox overrides it with
 * tWPInGameData_SkyBox_SkyBoxRotation).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SkyBoxRotation(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunPitchOffset);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunPitchOffset, vtable slot 27 (tWPInGameData_Sun overrides it with
 * tWPInGameData_Sun_SunPitchOffset).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunPitchOffset(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunYawOffset);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunYawOffset, vtable slot 28 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunYawOffset).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunYawOffset(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunColorR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunColorR, vtable slot 29 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunColorR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunColorR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunColorG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunColorG, vtable slot 30 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunColorG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunColorG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunColorB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunColorB, vtable slot 31 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunColorB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunColorB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunIntensity, vtable slot 32 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunType);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunType, vtable slot 33 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunType).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunType(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LensIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LensIntensity, vtable slot 34 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_LensIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LensIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SunRadius);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SunRadius, vtable slot 35 (tWPInGameData_Sun overrides it with tWPInGameData_Sun_SunRadius).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SunRadius(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowSparkleDensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowSparkleDensity, vtable slot 36 (tWPInGameData_Surface overrides it with
 * tWPInGameData_Surface_SnowSparkleDensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowSparkleDensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightIrradianceName);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightIrradianceName, vtable slot 37 (tWPInGameData_Lighting overrides it with
 * tWPInGameData_Lighting_LightIrradianceName).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightIrradianceName(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ShadowIrradianceName);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ShadowIrradianceName, vtable slot 38 (tWPInGameData_Lighting overrides it with
 * tWPInGameData_Lighting_ShadowIrradianceName).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ShadowIrradianceName(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_TunnelIrradianceName);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default TunnelIrradianceName, vtable slot 39 (tWPInGameData_Lighting overrides it with
 * tWPInGameData_Lighting_TunnelIrradianceName).
 *
 * @return 0
 */
extern "C" int tWPInGameData_TunnelIrradianceName(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ObjectIrradianceName);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default ObjectIrradianceName, vtable slot 40 (tWPInGameData_Lighting overrides it with
 * tWPInGameData_Lighting_ObjectIrradianceName).
 *
 * @return 0
 */
extern "C" int tWPInGameData_ObjectIrradianceName(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_AmbientLightIntensityOffset);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default AmbientLightIntensityOffset, vtable slot 41 (tWPInGameData_Lighting overrides it
 * with tWPInGameData_Lighting_AmbientLightIntensityOffset).
 *
 * @return 0
 */
extern "C" int tWPInGameData_AmbientLightIntensityOffset(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_RimLightIntensityOffset);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default RimLightIntensityOffset, vtable slot 42 (tWPInGameData_Lighting overrides it with
 * tWPInGameData_Lighting_RimLightIntensityOffset).
 *
 * @return 0
 */
extern "C" int tWPInGameData_RimLightIntensityOffset(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowIntensity, vtable slot 43 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_FlakeSize);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default FlakeSize, vtable slot 44 (tWPInGameData_Weather overrides it with tWPInGameData_Weather_FlakeSize).
 *
 * @return 0
 */
extern "C" int tWPInGameData_FlakeSize(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_WindIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default WindIntensity, vtable slot 45 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_WindIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_WindIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_WindRotation);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default WindRotation, vtable slot 46 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_WindRotation).
 *
 * @return 0
 */
extern "C" int tWPInGameData_WindRotation(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_PlayerWindIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default PlayerWindIntensity, vtable slot 47 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_PlayerWindIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_PlayerWindIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Temperature);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default Temperature, vtable slot 48 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_Temperature).
 *
 * @return 0
 */
extern "C" int tWPInGameData_Temperature(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFlurries);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFlurries, vtable slot 49 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFlurries).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFlurries(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowGravityMultiplier);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowGravityMultiplier, vtable slot 50 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowGravityMultiplier).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowGravityMultiplier(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffItensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffItensity, vtable slot 51 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffItensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffItensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffWindIntensity);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffWindIntensity, vtable slot 52 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffWindIntensity).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffWindIntensity(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightningChance);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default LightningChance, vtable slot 53 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_LightningChance).
 *
 * @return 0
 */
extern "C" int tWPInGameData_LightningChance(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFlakeColourA);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFlakeColourA, vtable slot 54 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFlakeColourA).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFlakeColourA(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFlakeColourR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFlakeColourR, vtable slot 55 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFlakeColourR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFlakeColourR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFlakeColourG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFlakeColourG, vtable slot 56 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFlakeColourG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFlakeColourG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFlakeColourB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFlakeColourB, vtable slot 57 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFlakeColourB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFlakeColourB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffColourA);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffColourA, vtable slot 58 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffColourA).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffColourA(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffColourR);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffColourR, vtable slot 59 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffColourR).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffColourR(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffColourG);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffColourG, vtable slot 60 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffColourG).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffColourG(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SnowFluffColourB);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default SnowFluffColourB, vtable slot 61 (tWPInGameData_Weather overrides it with
 * tWPInGameData_Weather_SnowFluffColourB).
 *
 * @return 0
 */
extern "C" int tWPInGameData_SnowFluffColourB(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Level);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default Level, vtable slot 62 (tWPInGameData_Danger overrides it with tWPInGameData_Danger_Level).
 *
 * @return 0
 */
extern "C" int tWPInGameData_Level(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_PointRate);
#ifdef SKIP_ASM
/**
 * Returns 0: tWPInGameData's default PointRate, vtable slot 63 (tWPInGameData_Danger overrides it with tWPInGameData_Danger_PointRate).
 *
 * @return 0
 */
extern "C" int tWPInGameData_PointRate(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger);
#ifdef SKIP_ASM
extern char tWPInGameData_MusicTrigger_vtbl[];
/**
 * Constructs a tWPInGameData_MusicTrigger: vtable tWPInGameData_MusicTrigger_vtbl, m_wpTravelDistance -99999.0f (no position yet), and
 * m_Type, m_lastType, m_Value and m_lastValue -1.
 *
 * @param musicTrigger  the object (0x18 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag
 *              "tWPIGD_MusicTrigger")
 * @return @p musicTrigger
 */
extern "C" void *tWPInGameData_MusicTrigger_tWPInGameData_MusicTrigger(tWPInGameData_MusicTrigger* musicTrigger) {
    VPTR_OF(tWPInGameData, musicTrigger) = (void**)tWPInGameData_MusicTrigger_vtbl;
    musicTrigger->m_wpTravelDistance = -99999.0f;
    musicTrigger->m_lastType = -1;
    musicTrigger->m_Type = -1;
    musicTrigger->m_lastValue = -1;
    musicTrigger->m_Value = -1;
    return ((void*)musicTrigger);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_tWPInGameData_Mix);
#ifdef SKIP_ASM
extern char tWPInGameData_Mix_vtbl[];
/**
 * Constructs a tWPInGameData_Mix: vtable tWPInGameData_Mix_vtbl, m_wpTravelDistance -99999.0f (no position yet), and m_MixPreset
 * and m_lastMixPreset -1.
 *
 * @param mix  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Mix")
 * @return @p mix
 */
extern "C" void *tWPInGameData_Mix_tWPInGameData_Mix(tWPInGameData_Mix* mix) {
    VPTR_OF(tWPInGameData, mix) = (void**)tWPInGameData_Mix_vtbl;
    mix->m_wpTravelDistance = -99999.0f;
    mix->m_lastMixPreset = -1;
    mix->m_MixPreset = -1;
    return ((void*)mix);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_tWPInGameData_Ambience);
#ifdef SKIP_ASM
extern char tWPInGameData_Ambience_vtbl[];
/**
 * Constructs a tWPInGameData_Ambience: vtable tWPInGameData_Ambience_vtbl, m_wpTravelDistance -99999.0f (no position yet), and m_BankID
 * and m_lastBankID -1.
 *
 * @param ambience  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Ambience")
 * @return @p ambience
 */
extern "C" void *tWPInGameData_Ambience_tWPInGameData_Ambience(tWPInGameData_Ambience* ambience) {
    VPTR_OF(tWPInGameData, ambience) = (void**)tWPInGameData_Ambience_vtbl;
    ambience->m_wpTravelDistance = -99999.0f;
    ambience->m_lastBankID = -1;
    ambience->m_BankID = -1;
    return ((void*)ambience);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_tWPInGameData_Speech);
#ifdef SKIP_ASM
extern char tWPInGameData_Speech_vtbl[];
/**
 * Constructs a tWPInGameData_Speech: vtable tWPInGameData_Speech_vtbl, m_wpTravelDistance -99999.0f (no position yet), and m_SpeechID
 * and m_lastSpeechID -1.
 *
 * @param speech  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Speech")
 * @return @p speech
 */
extern "C" void *tWPInGameData_Speech_tWPInGameData_Speech(tWPInGameData_Speech* speech) {
    VPTR_OF(tWPInGameData, speech) = (void**)tWPInGameData_Speech_vtbl;
    speech->m_wpTravelDistance = -99999.0f;
    speech->m_lastSpeechID = -1;
    speech->m_SpeechID = -1;
    return ((void*)speech);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_tWPInGameData_Camera);
#ifdef SKIP_ASM
extern char tWPInGameData_Camera_vtbl[];
/**
 * Constructs a tWPInGameData_Camera: vtable tWPInGameData_Camera_vtbl, m_wpTravelDistance -99999.0f (no position yet), and
 * m_DrawDistance and m_lastDrawDistance 30000.0f.
 *
 * @param camera  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Camera")
 * @return @p camera
 */
extern "C" void *tWPInGameData_Camera_tWPInGameData_Camera(tWPInGameData_Camera* camera) {
    VPTR_OF(tWPInGameData, camera) = (void**)tWPInGameData_Camera_vtbl;
    camera->m_lastDrawDistance = 30000.0f;
    camera->m_wpTravelDistance = -99999.0f;
    camera->m_DrawDistance = 30000.0f;
    return ((void*)camera);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_tWPInGameData_Fog);
#ifdef SKIP_ASM
extern char tWPInGameData_Fog_vtbl[];
/**
 * Constructs a tWPInGameData_Fog: vtable tWPInGameData_Fog_vtbl, m_wpTravelDistance -99999.0f (no position yet), and both the
 * current and the last values of m_FogDensity (0), m_FogNear (3000), m_FogFar (30000) and the fog colour (0.43, 0.55,
 * 0.71).
 *
 * @param fog  the object (0x38 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Fog")
 * @return @p fog
 */
extern "C" void *tWPInGameData_Fog_tWPInGameData_Fog(tWPInGameData_Fog* fog) {
    float zero = 0.0f;
    float fogNear = 3000.0f, fogFar = 30000.0f, fogRed = 0.43f, fogGreen = 0.55f, fogBlue = 0.71f;
    VPTR_OF(tWPInGameData, fog) = (void**)((int*)tWPInGameData_Fog_vtbl);
    fog->m_wpTravelDistance = -99999.0f;
    fog->m_lastFogDensity = zero;
    fog->m_lastFogNear = fogNear;
    fog->m_lastFogFar = fogFar;
    fog->m_lastFogColourR = fogRed;
    fog->m_lastFogColourG = fogGreen;
    fog->m_lastFogColourB = fogBlue;
    fog->m_FogDensity = zero;
    fog->m_FogNear = fogNear;
    fog->m_FogFar = fogFar;
    fog->m_FogColourR = fogRed;
    fog->m_FogColourG = fogGreen;
    fog->m_FogColourB = fogBlue;
    return ((void*)fog);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_tWPInGameData_LightGlow);
#ifdef SKIP_ASM
extern char tWPInGameData_LightGlow_vtbl[];
/**
 * Constructs the blended light-glow values with their defaults.
 *
 * Stores the vtable tWPInGameData_LightGlow_vtbl and -99999 in m_wpTravelDistance; sets the cutoff, cutoff scale, copy, source and
 * frame intensities (and their m_last copies) to 1, and m_LightGlowBlend2 / m_LightGlowBlend3 (and lasts) to 0.
 *
 * @return @p self
 */
extern "C" void *tWPInGameData_LightGlow_tWPInGameData_LightGlow(tWPInGameData_LightGlow* self) {
    float one = 1.0f, zeroF = 0.0f;
    float *floats = (float*)self;
    floats[0] = -99999.0f;
    VPTR_OF(tWPInGameData, self) = (void**)tWPInGameData_LightGlow_vtbl;
    floats[3] = one;
    floats[2] = one;
    floats[5] = one;
    floats[4] = one;
    floats[7] = one;
    floats[6] = one;
    floats[9] = one;
    floats[8] = one;
    floats[11] = one;
    floats[10] = one;
    floats[13] = zeroF;
    floats[12] = zeroF;
    floats[15] = zeroF;
    floats[14] = zeroF;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_tWPInGameData_ScreenTint);
#ifdef SKIP_ASM
extern char tWPInGameData_ScreenTint_vtbl[];
/**
 * Constructs the blended screen-tint values with their defaults: scale RGB 1, offset RGB 0.
 *
 * Stores the vtable tWPInGameData_ScreenTint_vtbl and -99999 in m_wpTravelDistance; each m_last copy gets the same default.
 *
 * @return @p self
 */
extern "C" void *tWPInGameData_ScreenTint_tWPInGameData_ScreenTint(tWPInGameData_ScreenTint* self) {
    float one = 1.0f, zeroF = 0.0f;
    float *floats = (float*)self;
    floats[0] = -99999.0f;
    VPTR_OF(tWPInGameData, self) = (void**)tWPInGameData_ScreenTint_vtbl;
    floats[3] = one;
    floats[2] = one;
    floats[5] = one;
    floats[4] = one;
    floats[7] = one;
    floats[6] = one;
    floats[9] = zeroF;
    floats[8] = zeroF;
    floats[11] = zeroF;
    floats[10] = zeroF;
    floats[13] = zeroF;
    floats[12] = zeroF;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_tWPInGameData_SkyBox);
#ifdef SKIP_ASM
extern char tWPInGameData_SkyBox_vtbl[];
/**
 * Constructs a tWPInGameData_SkyBox: vtable tWPInGameData_SkyBox_vtbl, m_wpTravelDistance -99999.0f (no position yet), and
 * m_SkyBoxRotation and m_lastSkyBoxRotation 0.
 *
 * @param skyBox  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_SkyBox")
 * @return @p skyBox
 */
extern "C" void *tWPInGameData_SkyBox_tWPInGameData_SkyBox(tWPInGameData_SkyBox* skyBox) {
    float zero = 0.0f;
    VPTR_OF(tWPInGameData, skyBox) = (void**)tWPInGameData_SkyBox_vtbl;
    skyBox->m_lastSkyBoxRotation = zero;
    skyBox->m_wpTravelDistance = -99999.0f;
    skyBox->m_SkyBoxRotation = zero;
    return ((void*)skyBox);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_tWPInGameData_Sun);
#ifdef SKIP_ASM
extern char tWPInGameData_Sun_vtbl[];
/**
 * Constructs the blended sun values with their defaults.
 *
 * Stores the vtable tWPInGameData_Sun_vtbl and -99999 in m_wpTravelDistance; pitch and yaw offsets 0, colour RGB and intensity 1,
 * m_SunType 0, lens intensity and radius 1, each m_last copy the same.
 *
 * @return @p self
 */
extern "C" void *tWPInGameData_Sun_tWPInGameData_Sun(tWPInGameData_Sun* self) {
    float one = 1.0f, zeroF = 0.0f;
    self->m_wpTravelDistance = -99999.0f;
    VPTR_OF(tWPInGameData, self) = (void**)tWPInGameData_Sun_vtbl;
    self->m_lastSunPitchOffset = zeroF;
    self->m_SunPitchOffset = zeroF;
    self->m_lastSunYawOffset = zeroF;
    self->m_SunYawOffset = zeroF;
    self->m_lastSunColorR = one;
    self->m_SunColorR = one;
    self->m_lastSunColorG = one;
    self->m_SunColorG = one;
    self->m_lastSunColorB = one;
    self->m_SunColorB = one;
    self->m_lastSunIntensity = one;
    self->m_SunIntensity = one;
    self->m_lastSunType = 0;
    self->m_SunType = 0;
    self->m_lastLensIntensity = one;
    self->m_LensIntensity = one;
    self->m_lastSunRadius = one;
    self->m_SunRadius = one;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_tWPInGameData_Surface);
#ifdef SKIP_ASM
extern char tWPInGameData_Surface_vtbl[];
/**
 * Constructs a tWPInGameData_Surface: vtable tWPInGameData_Surface_vtbl, m_wpTravelDistance -99999.0f (no position yet), and
 * m_SnowSparkleDensity and m_lastSnowSparkleDensity 1.0f.
 *
 * @param surface  the object (0x10 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Surface")
 * @return @p surface
 */
extern "C" void *tWPInGameData_Surface_tWPInGameData_Surface(tWPInGameData_Surface* surface) {
    float one = 1.0f;
    VPTR_OF(tWPInGameData, surface) = (void**)tWPInGameData_Surface_vtbl;
    surface->m_lastSnowSparkleDensity = one;
    surface->m_wpTravelDistance = -99999.0f;
    surface->m_SnowSparkleDensity = one;
    return ((void*)surface);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_tWPInGameData_Lighting);
#ifdef SKIP_ASM
extern tWPNameField kNullNameField_2BC9B0 __asm__("kNullNameField");
extern char tWPInGameData_Lighting_vtbl[];
/**
 * Constructs a tWPInGameData_Lighting: vtable tWPInGameData_Lighting_vtbl, m_wpTravelDistance -99999.0f (no position yet), the four
 * irradiance names and their m_last copies (eight tWPNameFields from m_LightIrradianceName on) zeroed word by word and
 * then set to the null name kNullNameField (which the worldpainterquery static initializer func_002C0B70 zeroes), and
 * m_AmbientLightIntensityOffset 1.0f and m_RimLightIntensityOffset 0.75f (current and last).
 *
 * @param lighting  the object (0x58 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Lighting")
 * @return @p lighting
 */
extern "C" void* tWPInGameData_Lighting_tWPInGameData_Lighting(tWPInGameData_Lighting* lighting)
{
    VPTR_OF(tWPInGameData, lighting) = (void**)tWPInGameData_Lighting_vtbl;
    lighting->m_wpTravelDistance = -99999.0f;
    ((&lighting->m_LightIrradianceName))[0].m_nameId[0] = ((&lighting->m_LightIrradianceName))[0].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[1].m_nameId[0] = ((&lighting->m_LightIrradianceName))[1].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[2].m_nameId[0] = ((&lighting->m_LightIrradianceName))[2].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[3].m_nameId[0] = ((&lighting->m_LightIrradianceName))[3].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[4].m_nameId[0] = ((&lighting->m_LightIrradianceName))[4].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[5].m_nameId[0] = ((&lighting->m_LightIrradianceName))[5].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[6].m_nameId[0] = ((&lighting->m_LightIrradianceName))[6].m_nameId[1] = 0;
    ((&lighting->m_LightIrradianceName))[7].m_nameId[0] = ((&lighting->m_LightIrradianceName))[7].m_nameId[1] = 0;
    { tWPNameField nullName = kNullNameField_2BC9B0; ((&lighting->m_LightIrradianceName))[0] = ((&lighting->m_LightIrradianceName))[1] = nullName; }
    { tWPNameField nullName = kNullNameField_2BC9B0; ((&lighting->m_LightIrradianceName))[2] = ((&lighting->m_LightIrradianceName))[3] = nullName; }
    { tWPNameField nullName = kNullNameField_2BC9B0; ((&lighting->m_LightIrradianceName))[4] = ((&lighting->m_LightIrradianceName))[5] = nullName; }
    { tWPNameField nullName = kNullNameField_2BC9B0; ((&lighting->m_LightIrradianceName))[6] = ((&lighting->m_LightIrradianceName))[7] = nullName; }
    lighting->m_lastAmbientLightIntensityOffset = 1.0f;
    lighting->m_lastRimLightIntensityOffset = 0.75f;
    lighting->m_AmbientLightIntensityOffset = 1.0f;
    lighting->m_RimLightIntensityOffset = 0.75f;
    return (lighting);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_tWPInGameData_Weather);
#ifdef SKIP_ASM
extern char tWPInGameData_Weather_vtbl[];

/**
 * Constructs a tWPInGameData_Weather: vtable tWPInGameData_Weather_vtbl, m_wpTravelDistance -99999.0f (no position yet), and both the
 * current and the last value of each weather field: m_SnowIntensity 0, m_FlakeSize 6, m_WindIntensity, m_WindRotation
 * and m_PlayerWindIntensity 0, m_Temperature 10, m_SnowFlurries and m_SnowGravityMultiplier 1, m_SnowFluffItensity,
 * m_SnowFluffWindIntensity and m_LightningChance 0, the snow-flake colour (A, R, G, B) all 1, and the snow-fluff colour
 * A 0.06 with R, G, B 1. Written through raw offsets.
 *
 * @return @p self (0xA0 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Weather")
 */
extern "C" void* tWPInGameData_Weather_tWPInGameData_Weather(tWPInGameData_Weather* self) {
    VPTR_OF(tWPInGameData, self) = (void**)tWPInGameData_Weather_vtbl;
    self->m_wpTravelDistance = -99999.0f;
    self->m_SnowIntensity = self->m_lastSnowIntensity = 0.0f;
    self->m_FlakeSize = self->m_lastFlakeSize = 6.0f;
    self->m_WindIntensity = self->m_lastWindIntensity = 0.0f;
    self->m_WindRotation = self->m_lastWindRotation = 0.0f;
    self->m_PlayerWindIntensity = self->m_lastPlayerWindIntensity = 0.0f;
    self->m_Temperature = self->m_lastTemperature = 10.0f;
    self->m_SnowFlurries = self->m_lastSnowFlurries = 1.0f;
    self->m_SnowGravityMultiplier = self->m_lastSnowGravityMultiplier = 1.0f;
    self->m_SnowFluffItensity = self->m_lastSnowFluffItensity = 0.0f;
    self->m_SnowFluffWindIntensity = self->m_lastSnowFluffWindIntensity = 0.0f;
    self->m_LightningChance = self->m_lastLightningChance = 0.0f;
    self->m_SnowFlakeColourA = self->m_lastSnowFlakeColourA = 1.0f;
    self->m_SnowFlakeColourR = self->m_lastSnowFlakeColourR = 1.0f;
    self->m_SnowFlakeColourG = self->m_lastSnowFlakeColourG = 1.0f;
    self->m_SnowFlakeColourB = self->m_lastSnowFlakeColourB = 1.0f;
    self->m_SnowFluffColourA = self->m_lastSnowFluffColourA = 0.05999999865889549f;
    self->m_SnowFluffColourR = self->m_lastSnowFluffColourR = 1.0f;
    self->m_SnowFluffColourG = self->m_lastSnowFluffColourG = 1.0f;
    self->m_SnowFluffColourB = self->m_lastSnowFluffColourB = 1.0f;
    return self;
}
#undef F_CAF8
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_tWPInGameData_Danger);
#ifdef SKIP_ASM
extern char tWPInGameData_Danger_vtbl[];
/**
 * Constructs a tWPInGameData_Danger: vtable tWPInGameData_Danger_vtbl, m_wpTravelDistance -99999.0f (no position yet), and m_Level,
 * m_lastLevel, m_PointRate and m_lastPointRate 0.
 *
 * @param danger  the object (0x18 bytes; cWorldPainterQuery_cWorldPainterQuery allocates it with tag "tWPIGD_Danger")
 * @return @p danger
 */
extern "C" void *tWPInGameData_Danger_tWPInGameData_Danger(tWPInGameData_Danger* danger) {
    float zero = 0.0f;
    VPTR_OF(tWPInGameData, danger) = (void**)tWPInGameData_Danger_vtbl;
    danger->m_lastLevel = 0;
    danger->m_Level = 0;
    danger->m_wpTravelDistance = -99999.0f;
    danger->m_lastPointRate = zero;
    danger->m_PointRate = zero;
    return ((void*)danger);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_blend);
#ifdef SKIP_ASM
/**
 * Blends the music-trigger values towards a region's data.
 *
 * Squares the blend factor b, then m_Type = b * rhs.m_Type + (1 - b) * m_Type, likewise m_Value (both ints, computed
 * in float and truncated), and stores the targets in m_lastType / m_lastValue. Calls the empty virtual at vtable
 * +0x218 before and after.
 *
 * @param rhs  the region's tWPData_MusicTrigger (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_MusicTrigger_blend(tWPInGameData_MusicTrigger* self, tWPData_MusicTrigger* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_Type = (int)(blendFactor * (float)rhs->m_Type + (1.0f - blendFactor) * (float)self->m_Type);
    self->m_lastType = rhs->m_Type;
    self->m_Value = (int)(blendFactor * (float)rhs->m_Value + (1.0f - blendFactor) * (float)self->m_Value);
    self->m_lastValue = rhs->m_Value;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_blend);
#ifdef SKIP_ASM
/**
 * Blends the mix preset towards a region's data: with b = factor squared, m_MixPreset = b * target + (1 - b) *
 * current.
 *
 * The int is computed in float and truncated; the target is stored in m_lastMixPreset. Calls the empty virtual at
 * vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Mix (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Mix_blend(tWPInGameData_Mix* self, tWPData_Mix* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_MixPreset = (int)(blendFactor * (float)rhs->m_MixPreset + (1.0f - blendFactor) * (float)self->m_MixPreset);
    self->m_lastMixPreset = rhs->m_MixPreset;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_blend);
#ifdef SKIP_ASM
/**
 * Blends the ambience bank id towards a region's data: with b = factor squared, m_BankID = b * target + (1 - b) * it.
 *
 * The int is computed in float and truncated, so a bank id is interpolated like a number; the target is stored in
 * m_lastBankID. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Ambience (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Ambience_blend(tWPInGameData_Ambience* self, tWPData_Ambience* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_BankID = (int)(blendFactor * (float)rhs->m_BankID + (1.0f - blendFactor) * (float)self->m_BankID);
    self->m_lastBankID = rhs->m_BankID;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_blend);
#ifdef SKIP_ASM
/**
 * Blends the speech id towards a region's data: with b = factor squared, m_SpeechID = b * target + (1 - b) * it.
 *
 * The int is computed in float and truncated; the target is stored in m_lastSpeechID. Calls the empty virtual at
 * vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Speech (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Speech_blend(tWPInGameData_Speech* self, tWPData_Speech* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_SpeechID = (int)(blendFactor * (float)rhs->m_SpeechID + (1.0f - blendFactor) * (float)self->m_SpeechID);
    self->m_lastSpeechID = rhs->m_SpeechID;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_blend);
#ifdef SKIP_ASM
/**
 * Blends the camera draw distance towards a region's data: with b = factor squared, b * target + (1 - b) * current.
 *
 * The target is stored in m_lastDrawDistance. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Camera (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Camera_blend(tWPInGameData_Camera* self, tWPData_Camera* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_DrawDistance = blendFactor * rhs->m_DrawDistance + (1.0f - blendFactor) * self->m_DrawDistance;
    self->m_lastDrawDistance = rhs->m_DrawDistance;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_blend);
#ifdef SKIP_ASM
/**
 * Blends the six fog values towards a region's data.
 *
 * With b = @p blendFactor squared, each of density, near, far and colour R/G/B becomes b * target + (1 - b) * current and its
 * m_last copy takes the target. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param src  the region's tWPData_Fog values (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Fog_blend(tWPInGameData_Fog* self, tWPData_Fog* src, float blendFactor) {
    float inv;
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    inv = 1.0f - blendFactor;
    self->m_FogDensity = blendFactor * src->m_FogDensity + inv * self->m_FogDensity;
    self->m_lastFogDensity = src->m_FogDensity;
    self->m_FogNear = blendFactor * src->m_FogNear + inv * self->m_FogNear;
    self->m_lastFogNear = src->m_FogNear;
    self->m_FogFar = blendFactor * src->m_FogFar + inv * self->m_FogFar;
    self->m_lastFogFar = src->m_FogFar;
    self->m_FogColourR = blendFactor * src->m_FogColourR + inv * self->m_FogColourR;
    self->m_lastFogColourR = src->m_FogColourR;
    self->m_FogColourG = blendFactor * src->m_FogColourG + inv * self->m_FogColourG;
    self->m_lastFogColourG = src->m_FogColourG;
    self->m_FogColourB = blendFactor * src->m_FogColourB + inv * self->m_FogColourB;
    self->m_lastFogColourB = src->m_FogColourB;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_blend);
#ifdef SKIP_ASM
/**
 * Blends the seven light-glow values towards a region's data.
 *
 * With b = @p blendFactor squared, cutoff, cutoff scale, copy, source and frame intensity, m_LightGlowBlend2 and
 * m_LightGlowBlend3 each become b * target + (1 - b) * current, and each m_last copy takes the target. Calls the
 * empty virtual at vtable +0x218 before and after.
 *
 * @param src  the region's light-glow values (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_LightGlow_blend(tWPInGameData_LightGlow* self, tWPData_LightGlow* src, float blendFactor) {
    float inv;
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    inv = 1.0f - blendFactor;
    self->m_LightGlowCutoff = blendFactor * src->m_LightGlowCutoff + inv * self->m_LightGlowCutoff;
    self->m_lastLightGlowCutoff = src->m_LightGlowCutoff;
    self->m_LightGlowCutoffScale = blendFactor * src->m_LightGlowCutoffScale + inv * self->m_LightGlowCutoffScale;
    self->m_lastLightGlowCutoffScale = src->m_LightGlowCutoffScale;
    self->m_LightGlowCopyIntensity = blendFactor * src->m_LightGlowCopyIntensity + inv * self->m_LightGlowCopyIntensity;
    self->m_lastLightGlowCopyIntensity = src->m_LightGlowCopyIntensity;
    self->m_LightGlowSourceIntensity =
        blendFactor * src->m_LightGlowSourceIntensity + inv * self->m_LightGlowSourceIntensity;
    self->m_lastLightGlowSourceIntensity = src->m_LightGlowSourceIntensity;
    self->m_LightGlowFrameIntensity =
        blendFactor * src->m_LightGlowFrameIntensity + inv * self->m_LightGlowFrameIntensity;
    self->m_lastLightGlowFrameIntensity = src->m_LightGlowFrameIntensity;
    self->m_LightGlowBlend2 = blendFactor * src->m_LightGlowBlend2 + inv * self->m_LightGlowBlend2;
    self->m_lastLightGlowBlend2 = src->m_LightGlowBlend2;
    self->m_LightGlowBlend3 = blendFactor * src->m_LightGlowBlend3 + inv * self->m_LightGlowBlend3;
    self->m_lastLightGlowBlend3 = src->m_LightGlowBlend3;
    {
        self->debugBlend();
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_blend);
#ifdef SKIP_ASM
/**
 * Blends the six screen-tint values (scale RGB, offset RGB) towards a region's data.
 *
 * With b = @p blendFactor squared, each becomes b * target + (1 - b) * current and its m_last copy takes the target. Calls the
 * empty virtual at vtable +0x218 before and after.
 *
 * @param src  the region's tWPData_ScreenTint values (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_ScreenTint_blend(tWPInGameData_ScreenTint* self, tWPData_ScreenTint* src, float blendFactor) {
    float inv;
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    inv = 1.0f - blendFactor;
    self->m_ScreenTintScaleR = blendFactor * src->m_ScreenTintScaleR + inv * self->m_ScreenTintScaleR;
    self->m_lastScreenTintScaleR = src->m_ScreenTintScaleR;
    self->m_ScreenTintScaleG = blendFactor * src->m_ScreenTintScaleG + inv * self->m_ScreenTintScaleG;
    self->m_lastScreenTintScaleG = src->m_ScreenTintScaleG;
    self->m_ScreenTintScaleB = blendFactor * src->m_ScreenTintScaleB + inv * self->m_ScreenTintScaleB;
    self->m_lastScreenTintScaleB = src->m_ScreenTintScaleB;
    self->m_ScreenTintOffsetR = blendFactor * src->m_ScreenTintOffsetR + inv * self->m_ScreenTintOffsetR;
    self->m_lastScreenTintOffsetR = src->m_ScreenTintOffsetR;
    self->m_ScreenTintOffsetG = blendFactor * src->m_ScreenTintOffsetG + inv * self->m_ScreenTintOffsetG;
    self->m_lastScreenTintOffsetG = src->m_ScreenTintOffsetG;
    self->m_ScreenTintOffsetB = blendFactor * src->m_ScreenTintOffsetB + inv * self->m_ScreenTintOffsetB;
    self->m_lastScreenTintOffsetB = src->m_ScreenTintOffsetB;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_blend);
#ifdef SKIP_ASM
/**
 * Blends the sky-box rotation towards a region's data: with b = factor squared, b * target + (1 - b) * current.
 *
 * The target is stored in m_lastSkyBoxRotation. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_SkyBox (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_SkyBox_blend(tWPInGameData_SkyBox* self, tWPData_SkyBox* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_SkyBoxRotation = blendFactor * rhs->m_SkyBoxRotation + (1.0f - blendFactor) * self->m_SkyBoxRotation;
    self->m_lastSkyBoxRotation = rhs->m_SkyBoxRotation;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_blend);
#ifdef SKIP_ASM
/**
 * Blends the sun values towards a region's data.
 *
 * With b = @p blendFactor squared, pitch and yaw offsets, colour R/G/B, intensity, m_SunType (an int, computed in float and
 * truncated), lens intensity and radius each become b * target + (1 - b) * current, and each m_last copy takes the
 * target. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param src  the region's tWPData_Sun values (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Sun_blend(tWPInGameData_Sun* self, tWPData_Sun* src, float blendFactor) {
    float inv;
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    inv = 1.0f - blendFactor;
    self->m_SunPitchOffset = blendFactor * src->m_SunPitchOffset + inv * self->m_SunPitchOffset;
    self->m_lastSunPitchOffset = src->m_SunPitchOffset;
    self->m_SunYawOffset = blendFactor * src->m_SunYawOffset + inv * self->m_SunYawOffset;
    self->m_lastSunYawOffset = src->m_SunYawOffset;
    self->m_SunColorR = blendFactor * src->m_SunColorR + inv * self->m_SunColorR;
    self->m_lastSunColorR = src->m_SunColorR;
    self->m_SunColorG = blendFactor * src->m_SunColorG + inv * self->m_SunColorG;
    self->m_lastSunColorG = src->m_SunColorG;
    self->m_SunColorB = blendFactor * src->m_SunColorB + inv * self->m_SunColorB;
    self->m_lastSunColorB = src->m_SunColorB;
    self->m_SunIntensity = blendFactor * src->m_SunIntensity + inv * self->m_SunIntensity;
    self->m_lastSunIntensity = src->m_SunIntensity;
    self->m_SunType = (int)(blendFactor * src->m_SunType + inv * self->m_SunType);
    self->m_lastSunType = src->m_SunType;
    self->m_LensIntensity = blendFactor * src->m_LensIntensity + inv * self->m_LensIntensity;
    self->m_lastLensIntensity = src->m_LensIntensity;
    self->m_SunRadius = blendFactor * src->m_SunRadius + inv * self->m_SunRadius;
    self->m_lastSunRadius = src->m_SunRadius;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_blend);
#ifdef SKIP_ASM
/**
 * Blends the snow sparkle density towards a region's data: with b = factor squared, b * target + (1 - b) * current.
 *
 * The target is stored in m_lastSnowSparkleDensity. Calls the empty virtual at vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Surface (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Surface_blend(tWPInGameData_Surface* self, tWPData_Surface* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_SnowSparkleDensity = blendFactor * rhs->m_SnowSparkleDensity + (1.0f - blendFactor) * self->m_SnowSparkleDensity;
    self->m_lastSnowSparkleDensity = rhs->m_SnowSparkleDensity;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_blend);
#ifdef SKIP_ASM
/**
 * Blends the lighting values towards a region's data.
 *
 * The four irradiance names (light, shadow, tunnel, object) are copied from the region as they are, not blended, and
 * their m_last copies are left alone. With b = factor squared, the ambient and rim light intensity offsets become b *
 * target + (1 - b) * current and their m_last copies take the targets. Calls the empty virtual at vtable +0x218
 * before and after.
 *
 * @param rhs  the region's tWPData_Lighting (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Lighting_blend(tWPInGameData_Lighting* self, tWPData_Lighting* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_LightIrradianceName = rhs->m_LightIrradianceName;
    self->m_ShadowIrradianceName = rhs->m_ShadowIrradianceName;
    self->m_TunnelIrradianceName = rhs->m_TunnelIrradianceName;
    self->m_ObjectIrradianceName = rhs->m_ObjectIrradianceName;
    self->m_AmbientLightIntensityOffset = blendFactor * rhs->m_AmbientLightIntensityOffset + (1.0f - blendFactor) * self->m_AmbientLightIntensityOffset;
    self->m_lastAmbientLightIntensityOffset = rhs->m_AmbientLightIntensityOffset;
    self->m_RimLightIntensityOffset = blendFactor * rhs->m_RimLightIntensityOffset + (1.0f - blendFactor) * self->m_RimLightIntensityOffset;
    self->m_lastRimLightIntensityOffset = rhs->m_RimLightIntensityOffset;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_blend);
#ifdef SKIP_ASM
// Environment property blend: w^2 blend of all 19 current values toward the
// incoming set, which is then stored as the target.
/**
 * Blends the 19 weather values towards a region's data.
 *
 * With b = factor squared, snow intensity, flake size, wind intensity and rotation, player wind, temperature,
 * flurries, snow gravity, fluff intensity and fluff wind, lightning chance and the snow-flake and fluff ARGB colours
 * each become b * target + (1 - b) * current, and each m_last copy takes the target. Calls the empty virtual at
 * vtable +0x218 before and after.
 *
 * @param rhs  the region's tWPData_Weather (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Weather_blend(tWPInGameData_Weather* self, tWPData_Weather* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_SnowIntensity = blendFactor * rhs->m_SnowIntensity + (1.0f - blendFactor) * self->m_SnowIntensity;
    self->m_lastSnowIntensity = rhs->m_SnowIntensity;
    self->m_FlakeSize = blendFactor * rhs->m_FlakeSize + (1.0f - blendFactor) * self->m_FlakeSize;
    self->m_lastFlakeSize = rhs->m_FlakeSize;
    self->m_WindIntensity = blendFactor * rhs->m_WindIntensity + (1.0f - blendFactor) * self->m_WindIntensity;
    self->m_lastWindIntensity = rhs->m_WindIntensity;
    self->m_WindRotation = blendFactor * rhs->m_WindRotation + (1.0f - blendFactor) * self->m_WindRotation;
    self->m_lastWindRotation = rhs->m_WindRotation;
    self->m_PlayerWindIntensity = blendFactor * rhs->m_PlayerWindIntensity + (1.0f - blendFactor) * self->m_PlayerWindIntensity;
    self->m_lastPlayerWindIntensity = rhs->m_PlayerWindIntensity;
    self->m_Temperature = blendFactor * rhs->m_Temperature + (1.0f - blendFactor) * self->m_Temperature;
    self->m_lastTemperature = rhs->m_Temperature;
    self->m_SnowFlurries = blendFactor * rhs->m_SnowFlurries + (1.0f - blendFactor) * self->m_SnowFlurries;
    self->m_lastSnowFlurries = rhs->m_SnowFlurries;
    self->m_SnowGravityMultiplier = blendFactor * rhs->m_SnowGravityMultiplier + (1.0f - blendFactor) * self->m_SnowGravityMultiplier;
    self->m_lastSnowGravityMultiplier = rhs->m_SnowGravityMultiplier;
    self->m_SnowFluffItensity = blendFactor * rhs->m_SnowFluffItensity + (1.0f - blendFactor) * self->m_SnowFluffItensity;
    self->m_lastSnowFluffItensity = rhs->m_SnowFluffItensity;
    self->m_SnowFluffWindIntensity = blendFactor * rhs->m_SnowFluffWindIntensity + (1.0f - blendFactor) * self->m_SnowFluffWindIntensity;
    self->m_lastSnowFluffWindIntensity = rhs->m_SnowFluffWindIntensity;
    self->m_LightningChance = blendFactor * rhs->m_LightningChance + (1.0f - blendFactor) * self->m_LightningChance;
    self->m_lastLightningChance = rhs->m_LightningChance;
    self->m_SnowFlakeColourA = blendFactor * rhs->m_SnowFlakeColourA + (1.0f - blendFactor) * self->m_SnowFlakeColourA;
    self->m_lastSnowFlakeColourA = rhs->m_SnowFlakeColourA;
    self->m_SnowFlakeColourR = blendFactor * rhs->m_SnowFlakeColourR + (1.0f - blendFactor) * self->m_SnowFlakeColourR;
    self->m_lastSnowFlakeColourR = rhs->m_SnowFlakeColourR;
    self->m_SnowFlakeColourG = blendFactor * rhs->m_SnowFlakeColourG + (1.0f - blendFactor) * self->m_SnowFlakeColourG;
    self->m_lastSnowFlakeColourG = rhs->m_SnowFlakeColourG;
    self->m_SnowFlakeColourB = blendFactor * rhs->m_SnowFlakeColourB + (1.0f - blendFactor) * self->m_SnowFlakeColourB;
    self->m_lastSnowFlakeColourB = rhs->m_SnowFlakeColourB;
    self->m_SnowFluffColourA = blendFactor * rhs->m_SnowFluffColourA + (1.0f - blendFactor) * self->m_SnowFluffColourA;
    self->m_lastSnowFluffColourA = rhs->m_SnowFluffColourA;
    self->m_SnowFluffColourR = blendFactor * rhs->m_SnowFluffColourR + (1.0f - blendFactor) * self->m_SnowFluffColourR;
    self->m_lastSnowFluffColourR = rhs->m_SnowFluffColourR;
    self->m_SnowFluffColourG = blendFactor * rhs->m_SnowFluffColourG + (1.0f - blendFactor) * self->m_SnowFluffColourG;
    self->m_lastSnowFluffColourG = rhs->m_SnowFluffColourG;
    self->m_SnowFluffColourB = blendFactor * rhs->m_SnowFluffColourB + (1.0f - blendFactor) * self->m_SnowFluffColourB;
    self->m_lastSnowFluffColourB = rhs->m_SnowFluffColourB;
    self->debugBlend();
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_blend);
#ifdef SKIP_ASM
/**
 * Blends the danger values towards a region's data.
 *
 * With b = factor squared, m_Level (an int, computed in float and truncated) and m_PointRate become b * target + (1 -
 * b) * current, and m_lastLevel / m_lastPointRate take the targets. Calls the empty virtual at vtable +0x218 before
 * and after.
 *
 * @param rhs  the region's tWPData_Danger (On Tour const tWPData* rhs)
 * @param blendFactor  the blend factor, squared before use (On Tour blend)
 */
extern "C" void tWPInGameData_Danger_blend(tWPInGameData_Danger* self, tWPData_Danger* rhs, float blendFactor) {
    self->debugBlend();
    blendFactor = blendFactor * blendFactor;
    self->m_Level = (int)(blendFactor * (float)rhs->m_Level + (1.0f - blendFactor) * (float)self->m_Level);
    self->m_lastLevel = rhs->m_Level;
    self->m_PointRate = blendFactor * rhs->m_PointRate + (1.0f - blendFactor) * self->m_PointRate;
    self->m_lastPointRate = rhs->m_PointRate;
    self->debugBlend();
}
#endif

/**
 * Does nothing: slot 67 of tWPInGameData_MusicTrigger's vtable tWPInGameData_MusicTrigger_vtbl, the slot the class's blend
 * (tWPInGameData_MusicTrigger_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_MusicTrigger_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Ambience's vtable tWPInGameData_Ambience_vtbl, the slot the class's blend
 * (tWPInGameData_Ambience_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Ambience_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Camera's vtable tWPInGameData_Camera_vtbl, the slot the class's blend
 * (tWPInGameData_Camera_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Camera_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Danger's vtable tWPInGameData_Danger_vtbl, the slot the class's blend
 * (tWPInGameData_Danger_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Danger_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Fog's vtable tWPInGameData_Fog_vtbl, the slot the class's blend (tWPInGameData_Fog_blend)
 * calls before and after blending.
 */
extern "C" void tWPInGameData_Fog_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_LightGlow's vtable tWPInGameData_LightGlow_vtbl, the slot the class's blend
 * (tWPInGameData_LightGlow_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_LightGlow_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Lighting's vtable tWPInGameData_Lighting_vtbl, the slot the class's blend
 * (tWPInGameData_Lighting_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Lighting_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Mix's vtable tWPInGameData_Mix_vtbl, the slot the class's blend (tWPInGameData_Mix_blend)
 * calls before and after blending.
 */
extern "C" void tWPInGameData_Mix_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_ScreenTint's vtable tWPInGameData_ScreenTint_vtbl, the slot the class's blend
 * (tWPInGameData_ScreenTint_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_ScreenTint_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_SkyBox's vtable tWPInGameData_SkyBox_vtbl, the slot the class's blend
 * (tWPInGameData_SkyBox_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_SkyBox_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Speech's vtable tWPInGameData_Speech_vtbl, the slot the class's blend
 * (tWPInGameData_Speech_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Speech_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Sun's vtable tWPInGameData_Sun_vtbl, the slot the class's blend (tWPInGameData_Sun_blend)
 * calls before and after blending.
 */
extern "C" void tWPInGameData_Sun_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Surface's vtable tWPInGameData_Surface_vtbl, the slot the class's blend
 * (tWPInGameData_Surface_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Surface_debugBlend(void) {
}

/**
 * Does nothing: slot 67 of tWPInGameData_Weather's vtable tWPInGameData_Weather_vtbl, the slot the class's blend
 * (tWPInGameData_Weather_blend) calls before and after blending.
 */
extern "C" void tWPInGameData_Weather_debugBlend(void) {
}

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_Type and m_Value (ints) equal the region data's two values (at +4 and +8); the m_last copies are
 * not compared.
 *
 * Slot 68 of tWPInGameData_MusicTrigger's vtable tWPInGameData_MusicTrigger_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_MusicTrigger (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_MusicTrigger_isEqual(tWPInGameData_MusicTrigger* lhs, tWPData_MusicTrigger* rhs) {
    if ((lhs->m_Type) != (rhs->m_Type)) {
        return false;
    }
    return (lhs->m_Value) == (rhs->m_Value);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_MixPreset equals the region data's preset (at +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Mix's vtable tWPInGameData_Mix_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Mix (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Mix_isEqual(tWPInGameData_Mix* lhs, tWPData_Mix* rhs) {
    return (lhs->m_MixPreset) == (rhs->m_MixPreset);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_BankID equals the region data's bank id (at +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Ambience's vtable tWPInGameData_Ambience_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Ambience (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Ambience_isEqual(tWPInGameData_Ambience* lhs, tWPData_Ambience* rhs) {
    return (lhs->m_BankID) == (rhs->m_BankID);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_SpeechID equals the region data's speech id (at +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Speech's vtable tWPInGameData_Speech_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Speech (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Speech_isEqual(tWPInGameData_Speech* lhs, tWPData_Speech* rhs) {
    return (lhs->m_SpeechID) == (rhs->m_SpeechID);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_DrawDistance equals the region data's draw distance (at +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Camera's vtable tWPInGameData_Camera_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Camera (its values start at +4, after tWPData's own word)
 * @return 1 if every value matches, else 0
 */
extern "C" int tWPInGameData_Camera_isEqual(tWPInGameData_Camera* lhs, tWPData_Camera* rhs) {
    int var_2;

    var_2 = 0;
    if ((lhs->m_DrawDistance) == (rhs->m_DrawDistance)) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_FogDensity, m_FogNear, m_FogFar and m_FogColourR/G/B all equal the region data's six floats (from
 * +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Fog's vtable tWPInGameData_Fog_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Fog (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Fog_isEqual(tWPInGameData_Fog* lhs, tWPData_Fog* rhs) {
    if (lhs->m_FogDensity != rhs->m_FogDensity) return false;
    if (lhs->m_FogNear != rhs->m_FogNear) return false;
    if (lhs->m_FogFar != rhs->m_FogFar) return false;
    if (lhs->m_FogColourR != rhs->m_FogColourR) return false;
    if (lhs->m_FogColourG != rhs->m_FogColourG) return false;
    if (lhs->m_FogColourB != rhs->m_FogColourB) return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether the seven current light-glow values (m_LightGlowCutoff .. m_LightGlowBlend3) all equal the region
 * data's (from +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_LightGlow's vtable tWPInGameData_LightGlow_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_LightGlow (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_LightGlow_isEqual(tWPInGameData_LightGlow* lhs, tWPData_LightGlow* rhs) {
    if (lhs->m_LightGlowCutoff != rhs->m_LightGlowCutoff) return false;
    if (lhs->m_LightGlowCutoffScale != rhs->m_LightGlowCutoffScale) return false;
    if (lhs->m_LightGlowCopyIntensity != rhs->m_LightGlowCopyIntensity) return false;
    if (lhs->m_LightGlowSourceIntensity != rhs->m_LightGlowSourceIntensity) return false;
    if (lhs->m_LightGlowFrameIntensity != rhs->m_LightGlowFrameIntensity) return false;
    if (lhs->m_LightGlowBlend2 != rhs->m_LightGlowBlend2) return false;
    if (lhs->m_LightGlowBlend3 != rhs->m_LightGlowBlend3) return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_ScreenTintScaleR/G/B and m_ScreenTintOffsetR/G/B all equal the region data's six floats (from +4);
 * the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_ScreenTint's vtable tWPInGameData_ScreenTint_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_ScreenTint (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_ScreenTint_isEqual(tWPInGameData_ScreenTint* lhs, tWPData_ScreenTint* rhs) {
    if (lhs->m_ScreenTintScaleR != rhs->m_ScreenTintScaleR) return false;
    if (lhs->m_ScreenTintScaleG != rhs->m_ScreenTintScaleG) return false;
    if (lhs->m_ScreenTintScaleB != rhs->m_ScreenTintScaleB) return false;
    if (lhs->m_ScreenTintOffsetR != rhs->m_ScreenTintOffsetR) return false;
    if (lhs->m_ScreenTintOffsetG != rhs->m_ScreenTintOffsetG) return false;
    if (lhs->m_ScreenTintOffsetB != rhs->m_ScreenTintOffsetB) return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_SkyBoxRotation equals the region data's rotation (at +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_SkyBox's vtable tWPInGameData_SkyBox_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_SkyBox (its values start at +4, after tWPData's own word)
 * @return 1 if every value matches, else 0
 */
extern "C" int tWPInGameData_SkyBox_isEqual(tWPInGameData_SkyBox* lhs, tWPData_SkyBox* rhs) {
    int var_2;

    var_2 = 0;
    if ((lhs->m_SkyBoxRotation) == (rhs->m_SkyBoxRotation)) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether the nine current sun values (m_SunPitchOffset .. m_SunRadius; m_SunType compared as an int) all equal
 * the region data's (from +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Sun's vtable tWPInGameData_Sun_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Sun (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Sun_isEqual(tWPInGameData_Sun* lhs, tWPData_Sun* rhs) {
    if (lhs->m_SunPitchOffset != rhs->m_SunPitchOffset) return false;
    if (lhs->m_SunYawOffset != rhs->m_SunYawOffset) return false;
    if (lhs->m_SunColorR != rhs->m_SunColorR) return false;
    if (lhs->m_SunColorG != rhs->m_SunColorG) return false;
    if (lhs->m_SunColorB != rhs->m_SunColorB) return false;
    if (lhs->m_SunIntensity != rhs->m_SunIntensity) return false;
    if (lhs->m_SunType != rhs->m_SunType) return false;
    if (lhs->m_LensIntensity != rhs->m_LensIntensity) return false;
    if (lhs->m_SunRadius != rhs->m_SunRadius) return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_SnowSparkleDensity equals the region data's sparkle density (at +4); the m_last copies are not
 * compared.
 *
 * Slot 68 of tWPInGameData_Surface's vtable tWPInGameData_Surface_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Surface (its values start at +4, after tWPData's own word)
 * @return 1 if every value matches, else 0
 */
extern "C" int tWPInGameData_Surface_isEqual(tWPInGameData_Surface* lhs, tWPData_Surface* rhs) {
    int var_2;

    var_2 = 0;
    if ((lhs->m_SnowSparkleDensity) == (rhs->m_SnowSparkleDensity)) {
        var_2 = 1;
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_isEqual);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_002BB100_DE30(char *, char *) __asm__("func_002BB100");
/**
 * Returns whether the four current irradiance names (compared with func_002BB100, the tWPNameField inequality test,
 * against the region data's names at +4, +0xC, +0x14 and +0x1C) and m_AmbientLightIntensityOffset and
 * m_RimLightIntensityOffset (against +0x24 and +0x28) all match; the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Lighting's vtable tWPInGameData_Lighting_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Lighting (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Lighting_isEqual(tWPInGameData_Lighting* lhs, tWPData_Lighting* rhs) {
    if (func_002BB100_DE30((char*)&lhs->m_LightIrradianceName, (char*)&rhs->m_LightIrradianceName)) return false;
    if (func_002BB100_DE30((char*)&lhs->m_ShadowIrradianceName, (char*)&rhs->m_ShadowIrradianceName)) return false;
    if (func_002BB100_DE30((char*)&lhs->m_TunnelIrradianceName, (char*)&rhs->m_TunnelIrradianceName)) return false;
    if (func_002BB100_DE30((char*)&lhs->m_ObjectIrradianceName, (char*)&rhs->m_ObjectIrradianceName)) return false;
    if (lhs->m_AmbientLightIntensityOffset != rhs->m_AmbientLightIntensityOffset) return false;
    if (lhs->m_RimLightIntensityOffset != rhs->m_RimLightIntensityOffset) return false;
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether all nineteen current weather values (m_SnowIntensity .. m_SnowFluffColourB) equal the region data's
 * (from +4); the m_last copies are not compared.
 *
 * Slot 68 of tWPInGameData_Weather's vtable tWPInGameData_Weather_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Weather (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
// Environment property compare: all 19 current values against the incoming set.
extern "C" bool tWPInGameData_Weather_isEqual(tWPInGameData_Weather* lhs, tWPData_Weather* rhs) {
    if ((lhs->m_SnowIntensity) != (rhs->m_SnowIntensity)) {
        return false;
    }
    if ((lhs->m_FlakeSize) != (rhs->m_FlakeSize)) {
        return false;
    }
    if ((lhs->m_WindIntensity) != (rhs->m_WindIntensity)) {
        return false;
    }
    if ((lhs->m_WindRotation) != (rhs->m_WindRotation)) {
        return false;
    }
    if ((lhs->m_PlayerWindIntensity) != (rhs->m_PlayerWindIntensity)) {
        return false;
    }
    if ((lhs->m_Temperature) != (rhs->m_Temperature)) {
        return false;
    }
    if ((lhs->m_SnowFlurries) != (rhs->m_SnowFlurries)) {
        return false;
    }
    if ((lhs->m_SnowGravityMultiplier) != (rhs->m_SnowGravityMultiplier)) {
        return false;
    }
    if ((lhs->m_SnowFluffItensity) != (rhs->m_SnowFluffItensity)) {
        return false;
    }
    if ((lhs->m_SnowFluffWindIntensity) != (rhs->m_SnowFluffWindIntensity)) {
        return false;
    }
    if ((lhs->m_LightningChance) != (rhs->m_LightningChance)) {
        return false;
    }
    if ((lhs->m_SnowFlakeColourA) != (rhs->m_SnowFlakeColourA)) {
        return false;
    }
    if ((lhs->m_SnowFlakeColourR) != (rhs->m_SnowFlakeColourR)) {
        return false;
    }
    if ((lhs->m_SnowFlakeColourG) != (rhs->m_SnowFlakeColourG)) {
        return false;
    }
    if ((lhs->m_SnowFlakeColourB) != (rhs->m_SnowFlakeColourB)) {
        return false;
    }
    if ((lhs->m_SnowFluffColourA) != (rhs->m_SnowFluffColourA)) {
        return false;
    }
    if ((lhs->m_SnowFluffColourR) != (rhs->m_SnowFluffColourR)) {
        return false;
    }
    if ((lhs->m_SnowFluffColourG) != (rhs->m_SnowFluffColourG)) {
        return false;
    }
    if ((lhs->m_SnowFluffColourB) != (rhs->m_SnowFluffColourB)) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_isEqual);
#ifdef SKIP_ASM
/**
 * Returns whether m_Level (an int) and m_PointRate equal the region data's (at +4 and +8); the m_last copies are not
 * compared.
 *
 * Slot 68 of tWPInGameData_Danger's vtable tWPInGameData_Danger_vtbl; the world-painter query calls it with the region's data.
 *
 * @param lhs  the in-game data
 * @param rhs  the region's tWPData_Danger (its values start at +4, after tWPData's own word)
 * @return true if every value matches
 */
extern "C" bool tWPInGameData_Danger_isEqual(tWPInGameData_Danger* lhs, tWPData_Danger* rhs) {
    if ((lhs->m_Level) != (rhs->m_Level)) {
        return false;
    }
    if ((lhs->m_PointRate) != (rhs->m_PointRate)) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_Type and m_Value to -1; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_MusicTrigger's vtable tWPInGameData_MusicTrigger_vtbl.
 *
 * @param musicTrigger  the in-game data
 */
extern "C" void tWPInGameData_MusicTrigger_setDefaults(tWPInGameData_MusicTrigger* musicTrigger) {
    musicTrigger->m_wpTravelDistance = 0.0f;
    musicTrigger->m_Type = -1;
    musicTrigger->m_Value = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_MixPreset to -1; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Mix's vtable tWPInGameData_Mix_vtbl.
 *
 * @param mix  the in-game data
 */
extern "C" void tWPInGameData_Mix_setDefaults(tWPInGameData_Mix* mix) {
    mix->m_wpTravelDistance = 0.0f;
    mix->m_MixPreset = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_BankID to -1; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Ambience's vtable tWPInGameData_Ambience_vtbl.
 *
 * @param ambience  the in-game data
 */
extern "C" void tWPInGameData_Ambience_setDefaults(tWPInGameData_Ambience* ambience) {
    ambience->m_wpTravelDistance = 0.0f;
    ambience->m_BankID = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_SpeechID to -1; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Speech's vtable tWPInGameData_Speech_vtbl.
 *
 * @param speech  the in-game data
 */
extern "C" void tWPInGameData_Speech_setDefaults(tWPInGameData_Speech* speech) {
    speech->m_wpTravelDistance = 0.0f;
    speech->m_SpeechID = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_DrawDistance to 30000.0f; the m_last copies are left
 * alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Camera's vtable tWPInGameData_Camera_vtbl.
 *
 * @param camera  the in-game data
 */
extern "C" void tWPInGameData_Camera_setDefaults(tWPInGameData_Camera* camera) {
    camera->m_wpTravelDistance = 0.0f;
    camera->m_DrawDistance = 30000.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_FogDensity to 0, m_FogNear to 3000, m_FogFar to 30000 and
 * the fog colour to (0.43, 0.55, 0.71); the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Fog's vtable tWPInGameData_Fog_vtbl.
 *
 * @param fog  the in-game data
 */
extern "C" void tWPInGameData_Fog_setDefaults(tWPInGameData_Fog* fog) {
    fog->m_FogNear = 3000.0f;
    fog->m_FogFar = 30000.0f;
    fog->m_FogColourR = 0.43f;
    fog->m_FogColourG = 0.55f;
    fog->m_FogColourB = 0.71f;
    fog->m_FogDensity = 0.0f;
    fog->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_LightGlowCutoff, m_LightGlowCutoffScale,
 * m_LightGlowCopyIntensity, m_LightGlowSourceIntensity and m_LightGlowFrameIntensity to 1.0f and m_LightGlowBlend2 and
 * m_LightGlowBlend3 to 0; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_LightGlow's vtable tWPInGameData_LightGlow_vtbl.
 *
 * @param lightGlow  the in-game data
 */
extern "C" void tWPInGameData_LightGlow_setDefaults(tWPInGameData_LightGlow* lightGlow) {
    lightGlow->m_LightGlowCutoff = 1.0f;
    lightGlow->m_LightGlowCutoffScale = 1.0f;
    lightGlow->m_LightGlowCopyIntensity = 1.0f;
    lightGlow->m_LightGlowSourceIntensity = 1.0f;
    lightGlow->m_LightGlowFrameIntensity = 1.0f;
    lightGlow->m_LightGlowBlend2 = 0.0f;
    lightGlow->m_LightGlowBlend3 = 0.0f;
    lightGlow->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: the tint scale (R, G, B) to 1.0f and the tint offset (R, G,
 * B) to 0; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_ScreenTint's vtable tWPInGameData_ScreenTint_vtbl.
 *
 * @param screenTint  the in-game data
 */
extern "C" void tWPInGameData_ScreenTint_setDefaults(tWPInGameData_ScreenTint* screenTint) {
    screenTint->m_ScreenTintScaleR = 1.0f;
    screenTint->m_ScreenTintScaleG = 1.0f;
    screenTint->m_ScreenTintScaleB = 1.0f;
    screenTint->m_ScreenTintOffsetR = 0.0f;
    screenTint->m_ScreenTintOffsetG = 0.0f;
    screenTint->m_ScreenTintOffsetB = 0.0f;
    screenTint->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_SkyBoxRotation to 0; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_SkyBox's vtable tWPInGameData_SkyBox_vtbl.
 *
 * @param skyBox  the in-game data
 */
extern "C" void tWPInGameData_SkyBox_setDefaults(tWPInGameData_SkyBox* skyBox) {
    skyBox->m_SkyBoxRotation = 0.0f;
    skyBox->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_SunPitchOffset and m_SunYawOffset to 0, the sun colour (R,
 * G, B), m_SunIntensity, m_LensIntensity and m_SunRadius to 1.0f, and m_SunType to 0; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Sun's vtable tWPInGameData_Sun_vtbl.
 *
 * @param sun  the in-game data
 */
extern "C" void tWPInGameData_Sun_setDefaults(tWPInGameData_Sun* sun) {
    sun->m_SunPitchOffset = 0.0f;
    sun->m_SunYawOffset = 0.0f;
    sun->m_SunColorR = 1.0f;
    sun->m_SunColorG = 1.0f;
    sun->m_SunColorB = 1.0f;
    sun->m_SunIntensity = 1.0f;
    sun->m_SunType = 0;
    sun->m_LensIntensity = 1.0f;
    sun->m_SunRadius = 1.0f;
    sun->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_SnowSparkleDensity to 1.0f; the m_last copies are left
 * alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Surface's vtable tWPInGameData_Surface_vtbl.
 *
 * @param surface  the in-game data
 */
extern "C" void tWPInGameData_Surface_setDefaults(tWPInGameData_Surface* surface) {
    surface->m_wpTravelDistance = 0.0f;
    surface->m_SnowSparkleDensity = 1.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_setDefaults);
#ifdef SKIP_ASM
extern tWPNameField kNullNameField;
struct sPairSet_002BE1F8
{
    int flags;              // 0x00
    char pad_0x04[4];
    tWPNameField k0;      // 0x08
    char pad_0x10[8];
    tWPNameField k1;      // 0x18
    char pad_0x20[8];
    tWPNameField k2;      // 0x28
    char pad_0x30[8];
    tWPNameField k3;      // 0x38
    char pad_0x40[8];
    float field_0x48;       // 0x48
    char pad_0x4C[4];
    float field_0x50;       // 0x50
};

/**
 * Resets the current values to the constructor's defaults: the four irradiance names to the null name kNullNameField,
 * m_AmbientLightIntensityOffset to 1.0f and m_RimLightIntensityOffset to 0.75f; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Lighting's vtable tWPInGameData_Lighting_vtbl.
 *
 * @param lighting  the in-game data
 */
extern "C" void tWPInGameData_Lighting_setDefaults(tWPInGameData_Lighting* lighting)
{
    (*(&lighting->m_LightIrradianceName)) = kNullNameField;
    (*(&lighting->m_ShadowIrradianceName)) = kNullNameField;
    (*(&lighting->m_TunnelIrradianceName)) = kNullNameField;
    (*(&lighting->m_ObjectIrradianceName)) = kNullNameField;
    lighting->m_AmbientLightIntensityOffset = 1.0f;
    lighting->m_RimLightIntensityOffset = 0.75f;
    lighting->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_FlakeSize 6, m_Temperature 10, m_SnowFlurries,
 * m_SnowGravityMultiplier and the snow-flake and snow-fluff colours 1 except m_SnowFluffColourA 0.06, and the other
 * weather values 0; the m_last copies are left alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Weather's vtable tWPInGameData_Weather_vtbl.
 *
 * @param weather  the in-game data
 */
extern "C" void tWPInGameData_Weather_setDefaults(tWPInGameData_Weather* weather) {
    weather->m_SnowIntensity = 0.0f;
    weather->m_FlakeSize = 6.0f;
    weather->m_WindIntensity = 0.0f;
    weather->m_WindRotation = 0.0f;
    weather->m_PlayerWindIntensity = 0.0f;
    weather->m_Temperature = 10.0f;
    weather->m_SnowFlurries = 1.0f;
    weather->m_SnowGravityMultiplier = 1.0f;
    weather->m_SnowFluffItensity = 0.0f;
    weather->m_SnowFluffWindIntensity = 0.0f;
    weather->m_LightningChance = 0.0f;
    weather->m_SnowFlakeColourA = 1.0f;
    weather->m_SnowFlakeColourR = 1.0f;
    weather->m_SnowFlakeColourG = 1.0f;
    weather->m_SnowFlakeColourB = 1.0f;
    weather->m_SnowFluffColourA = 0.06f;
    weather->m_SnowFluffColourR = 1.0f;
    weather->m_SnowFluffColourG = 1.0f;
    weather->m_SnowFluffColourB = 1.0f;
    weather->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_setDefaults);
#ifdef SKIP_ASM
/**
 * Resets the current values to the constructor's defaults: m_Level and m_PointRate to 0; the m_last copies are left
 * alone.
 *
 * Also sets m_wpTravelDistance to 0 (the constructor and cWorldPainterQuery_reset use -99999.0f). Slot 69 of
 * tWPInGameData_Danger's vtable tWPInGameData_Danger_vtbl.
 *
 * @param danger  the in-game data
 */
extern "C" void tWPInGameData_Danger_setDefaults(tWPInGameData_Danger* danger) {
    danger->m_Level = 0;
    danger->m_PointRate = 0.0f;
    danger->m_wpTravelDistance = 0.0f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_readFromReplayFrame);
#ifdef SKIP_ASM
struct VE_E2D8 { short delta; short idx; int (*fn)(void *, void *, int); };
/**
 * Restores the music-trigger values from a replay frame.
 *
 * Reads 4 bytes into m_Type, then 4 into m_wpTravelDistance (+0x0), then m_Value and m_wpTravelDistance again,
 * through the frame's slot 2 (read). The m_last targets are not restored; the writer stores the travel distance in
 * their place.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_MusicTrigger::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_Type, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
    frame->read((char*)&m_Value, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_MixPreset and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's virtual
 * read (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_Mix's vtable tWPInGameData_Mix_vtbl; the order matches the class's writeToReplayFrame (tWPInGameData_Mix_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Mix::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_MixPreset, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_BankID and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's virtual read
 * (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_Ambience's vtable tWPInGameData_Ambience_vtbl; the order matches the class's writeToReplayFrame
 * (tWPInGameData_Ambience_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Ambience::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_BankID, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_SpeechID and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's virtual read
 * (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_Speech's vtable tWPInGameData_Speech_vtbl; the order matches the class's writeToReplayFrame
 * (tWPInGameData_Speech_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Speech::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_SpeechID, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_DrawDistance and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's virtual
 * read (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_Camera's vtable tWPInGameData_Camera_vtbl; the order matches the class's writeToReplayFrame
 * (tWPInGameData_Camera_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Camera::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_DrawDistance, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_readFromReplayFrame);
#ifdef SKIP_ASM
static inline void Wr_E518(cReplayFramePtr* s, void* p, int n) {
    s->read((char*)p, n);
}

/**
 * Restores the six fog values from a replay frame.
 *
 * For density, near, far and colour R/G/B in turn, reads 4 bytes into the value and 4 into m_wpTravelDistance (+0x0),
 * through the frame's slot 2 (read); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_Fog::readFromReplayFrame(cReplayFramePtr* frame)
{
    Wr_E518(frame, &m_FogDensity, 4);
    Wr_E518(frame, ((char*)this), 4);
    Wr_E518(frame, &m_FogNear, 4);
    Wr_E518(frame, ((char*)this), 4);
    Wr_E518(frame, &m_FogFar, 4);
    Wr_E518(frame, ((char*)this), 4);
    Wr_E518(frame, &m_FogColourR, 4);
    Wr_E518(frame, ((char*)this), 4);
    Wr_E518(frame, &m_FogColourG, 4);
    Wr_E518(frame, ((char*)this), 4);
    Wr_E518(frame, &m_FogColourB, 4);
    Wr_E518(frame, ((char*)this), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_readFromReplayFrame);
#ifdef SKIP_ASM
static inline void Wr_E698(cReplayFramePtr* s, void* p, int n) {
    s->read((char*)p, n);
}

/**
 * Restores the seven light-glow values from a replay frame.
 *
 * For each value (+0x8 to +0x38 in steps of 8) reads 4 bytes into it and 4 into m_wpTravelDistance (+0x0), through
 * the frame's slot 2 (read); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_LightGlow::readFromReplayFrame(cReplayFramePtr* frame)
{
    Wr_E698(frame, &m_LightGlowCutoff, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowCutoffScale, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowCopyIntensity, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowSourceIntensity, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowFrameIntensity, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowBlend2, 4);
    Wr_E698(frame, ((char*)this), 4);
    Wr_E698(frame, &m_LightGlowBlend3, 4);
    Wr_E698(frame, ((char*)this), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_readFromReplayFrame);
#ifdef SKIP_ASM
static inline void Wr_E850(cReplayFramePtr* s, void* p, int n) {
    s->read((char*)p, n);
}

/**
 * Restores the six screen-tint values from a replay frame.
 *
 * For scale R/G/B and offset R/G/B in turn, reads 4 bytes into the value and 4 into m_wpTravelDistance (+0x0),
 * through the frame's slot 2 (read); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_ScreenTint::readFromReplayFrame(cReplayFramePtr* frame)
{
    Wr_E850(frame, &m_ScreenTintScaleR, 4);
    Wr_E850(frame, ((char*)this), 4);
    Wr_E850(frame, &m_ScreenTintScaleG, 4);
    Wr_E850(frame, ((char*)this), 4);
    Wr_E850(frame, &m_ScreenTintScaleB, 4);
    Wr_E850(frame, ((char*)this), 4);
    Wr_E850(frame, &m_ScreenTintOffsetR, 4);
    Wr_E850(frame, ((char*)this), 4);
    Wr_E850(frame, &m_ScreenTintOffsetG, 4);
    Wr_E850(frame, ((char*)this), 4);
    Wr_E850(frame, &m_ScreenTintOffsetB, 4);
    Wr_E850(frame, ((char*)this), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_SkyBoxRotation and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's
 * virtual read (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_SkyBox's vtable tWPInGameData_SkyBox_vtbl; the order matches the class's writeToReplayFrame
 * (tWPInGameData_SkyBox_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_SkyBox::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_SkyBoxRotation, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_EA38 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_EA38 { char* vt; };

static inline void Wr_EA38(Stream_EA38* s, void* p, int n) {
    VEnt_EA38* e = (VEnt_EA38*)(s->vt + 0x10);
    e->fn(vthis(s, *e), p, n);
}

/**
 * Restores the nine sun values from a replay frame.
 *
 * For pitch, yaw, colour R/G/B, intensity, type, lens intensity and radius in turn, reads 4 bytes into the value and
 * 4 into m_wpTravelDistance (+0x0), through the frame's slot 2 (read); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_Sun_readFromReplayFrame(tWPInGameData_Sun* self, Stream_EA38* frame) {
    Wr_EA38(frame, &self->m_SunPitchOffset, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunYawOffset, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunColorR, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunColorG, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunColorB, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunIntensity, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunType, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_LensIntensity, 4);
    Wr_EA38(frame, ((char*)self), 4);
    Wr_EA38(frame, &self->m_SunRadius, 4);
    Wr_EA38(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Reads m_SnowSparkleDensity and then m_wpTravelDistance (4 bytes each) back from a replay frame through the frame's
 * virtual read (slot 2 of its vtable, cReplayFramePtr_read for a cReplayFramePtr).
 *
 * Slot 65 of tWPInGameData_Surface's vtable tWPInGameData_Surface_vtbl; the order matches the class's writeToReplayFrame
 * (tWPInGameData_Surface_writeToReplayFrame).
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Surface::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_SnowSparkleDensity, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_readFromReplayFrame);
#ifdef SKIP_ASM
struct VEnt_ECC8 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_ECC8 { char* vt; };

static inline void Wr_ECC8(Stream_ECC8* s, void* p, int n) {
    VEnt_ECC8* e = (VEnt_ECC8*)(s->vt + 0x10);
    e->fn(vthis(s, *e), p, n);
}

/**
 * Restores the lighting values from a replay frame.
 *
 * Reads the four 8-byte irradiance names (+0x8, +0x18, +0x28, +0x38) and the ambient and rim intensity offsets
 * (+0x48, +0x50), each followed by 4 bytes into m_wpTravelDistance (+0x0), through the frame's slot 2 (read); the
 * m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_Lighting_readFromReplayFrame(tWPInGameData_Lighting* self, Stream_ECC8* frame) {
    Wr_ECC8(frame, &self->m_LightIrradianceName, 8);
    Wr_ECC8(frame, ((char*)self), 4);
    Wr_ECC8(frame, &self->m_ShadowIrradianceName, 8);
    Wr_ECC8(frame, ((char*)self), 4);
    Wr_ECC8(frame, &self->m_TunnelIrradianceName, 8);
    Wr_ECC8(frame, ((char*)self), 4);
    Wr_ECC8(frame, &self->m_ObjectIrradianceName, 8);
    Wr_ECC8(frame, ((char*)self), 4);
    Wr_ECC8(frame, &self->m_AmbientLightIntensityOffset, 4);
    Wr_ECC8(frame, ((char*)self), 4);
    Wr_ECC8(frame, &self->m_RimLightIntensityOffset, 4);
    Wr_ECC8(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_EE48 {
public:
    virtual void v01();
    virtual int write(void* p, int n);
};

/**
 * Restores the 19 weather values from a replay frame.
 *
 * For each value (+0x8 to +0x98 in steps of 8) reads 4 bytes into it and 4 into m_wpTravelDistance (+0x0), through
 * the frame's slot 2 (read; the local class calls that slot "write"); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
// Streams 19 words of self (at +0x8 .. +0x98), each followed by the word at +0x0.
extern "C" void tWPInGameData_Weather_readFromReplayFrame(tWPInGameData_Weather* self, cStream_EE48* frame)
{
    frame->write(&self->m_SnowIntensity, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_FlakeSize, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_WindIntensity, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_WindRotation, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_PlayerWindIntensity, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_Temperature, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFlurries, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowGravityMultiplier, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffItensity, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffWindIntensity, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_LightningChance, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFlakeColourA, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFlakeColourR, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFlakeColourG, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFlakeColourB, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffColourA, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffColourR, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffColourG, 4);
    frame->write(((char*)self), 4);
    frame->write(&self->m_SnowFluffColourB, 4);
    frame->write(((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_readFromReplayFrame);
#ifdef SKIP_ASM
struct VE_F2A0 { short delta; short idx; int (*fn)(void *, void *, int); };
/**
 * Restores the danger values from a replay frame.
 *
 * Reads 4 bytes into m_Level, 4 into m_wpTravelDistance (+0x0), then m_PointRate and m_wpTravelDistance again,
 * through the frame's slot 2 (read); the m_last targets are not restored.
 *
 * @param frame  the replay frame to read from (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_Danger::readFromReplayFrame(cReplayFramePtr* frame)
{
    frame->read((char*)&m_Level, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
    frame->read((char*)&m_PointRate, 4);
    frame->read((char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_MusicTrigger_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves the music-trigger values to a replay frame.
 *
 * Writes m_Type, m_wpTravelDistance (+0x0), m_Value and m_wpTravelDistance again, 4 bytes each, through the frame's
 * slot 1 (write). The m_last targets are not saved: the second word of each pair is always the travel distance.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_MusicTrigger::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_Type, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
    frame->write((const char*)&m_Value, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Mix_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_MixPreset and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual write
 * (slot 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_Mix's vtable tWPInGameData_Mix_vtbl; tWPInGameData_Mix_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Mix::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_MixPreset, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Ambience_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_BankID and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual write (slot
 * 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_Ambience's vtable tWPInGameData_Ambience_vtbl; tWPInGameData_Ambience_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Ambience::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_BankID, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Speech_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_SpeechID and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual write
 * (slot 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_Speech's vtable tWPInGameData_Speech_vtbl; tWPInGameData_Speech_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Speech::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_SpeechID, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Camera_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_DrawDistance and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual write
 * (slot 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_Camera's vtable tWPInGameData_Camera_vtbl; tWPInGameData_Camera_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Camera::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_DrawDistance, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Fog_writeToReplayFrame);
#ifdef SKIP_ASM
static inline void Wr_F580(cReplayFramePtr* s, void* p, int n) {
    s->write((const char*)p, n);
}

/**
 * Saves the six fog values to a replay frame.
 *
 * For density, near, far and colour R/G/B in turn, writes the value and then m_wpTravelDistance (+0x0), 4 bytes each,
 * through the frame's slot 1 (write); the m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_Fog_writeToReplayFrame(tWPInGameData_Fog* self, cReplayFramePtr* frame) {
    Wr_F580(frame, &self->m_FogDensity, 4);
    Wr_F580(frame, ((char*)self), 4);
    Wr_F580(frame, &self->m_FogNear, 4);
    Wr_F580(frame, ((char*)self), 4);
    Wr_F580(frame, &self->m_FogFar, 4);
    Wr_F580(frame, ((char*)self), 4);
    Wr_F580(frame, &self->m_FogColourR, 4);
    Wr_F580(frame, ((char*)self), 4);
    Wr_F580(frame, &self->m_FogColourG, 4);
    Wr_F580(frame, ((char*)self), 4);
    Wr_F580(frame, &self->m_FogColourB, 4);
    Wr_F580(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_LightGlow_writeToReplayFrame);
#ifdef SKIP_ASM
static inline void Wr_F700(cReplayFramePtr* s, void* p, int n) {
    s->write((const char*)p, n);
}

/**
 * Saves the seven light-glow values to a replay frame.
 *
 * For each value (+0x8 to +0x38 in steps of 8) writes it and then m_wpTravelDistance (+0x0), 4 bytes each, through
 * the frame's slot 1 (write); the m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_LightGlow_writeToReplayFrame(tWPInGameData_LightGlow* self, cReplayFramePtr* frame) {
    Wr_F700(frame, &self->m_LightGlowCutoff, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowCutoffScale, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowCopyIntensity, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowSourceIntensity, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowFrameIntensity, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowBlend2, 4);
    Wr_F700(frame, ((char*)self), 4);
    Wr_F700(frame, &self->m_LightGlowBlend3, 4);
    Wr_F700(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_ScreenTint_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_F8B8 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_F8B8 { char* vt; };

static inline void Wr_F8B8(Stream_F8B8* s, void* p, int n) {
    VEnt_F8B8* e = (VEnt_F8B8*)(s->vt + 0x8);
    e->fn(vthis(s, *e), p, n);
}

/**
 * Saves the six screen-tint values to a replay frame.
 *
 * For scale R/G/B and offset R/G/B in turn, writes the value and then m_wpTravelDistance (+0x0), 4 bytes each,
 * through the frame's slot 1 (write); the m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_ScreenTint_writeToReplayFrame(tWPInGameData_ScreenTint* self, Stream_F8B8* frame) {
    Wr_F8B8(frame, &self->m_ScreenTintScaleR, 4);
    Wr_F8B8(frame, ((char*)self), 4);
    Wr_F8B8(frame, &self->m_ScreenTintScaleG, 4);
    Wr_F8B8(frame, ((char*)self), 4);
    Wr_F8B8(frame, &self->m_ScreenTintScaleB, 4);
    Wr_F8B8(frame, ((char*)self), 4);
    Wr_F8B8(frame, &self->m_ScreenTintOffsetR, 4);
    Wr_F8B8(frame, ((char*)self), 4);
    Wr_F8B8(frame, &self->m_ScreenTintOffsetG, 4);
    Wr_F8B8(frame, ((char*)self), 4);
    Wr_F8B8(frame, &self->m_ScreenTintOffsetB, 4);
    Wr_F8B8(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_SkyBox_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_SkyBoxRotation and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual
 * write (slot 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_SkyBox's vtable tWPInGameData_SkyBox_vtbl; tWPInGameData_SkyBox_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_SkyBox::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_SkyBoxRotation, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Sun_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_FAA0 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_FAA0 { char* vt; };

static inline void Wr_FAA0(Stream_FAA0* s, void* p, int n) {
    VEnt_FAA0* e = (VEnt_FAA0*)(s->vt + 0x8);
    e->fn(vthis(s, *e), p, n);
}

/**
 * Saves the nine sun values to a replay frame.
 *
 * For pitch, yaw, colour R/G/B, intensity, type, lens intensity and radius in turn, writes the value and then
 * m_wpTravelDistance (+0x0), 4 bytes each, through the frame's slot 1 (write); the m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_Sun_writeToReplayFrame(tWPInGameData_Sun* self, Stream_FAA0* frame) {
    Wr_FAA0(frame, &self->m_SunPitchOffset, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunYawOffset, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunColorR, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunColorG, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunColorB, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunIntensity, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunType, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_LensIntensity, 4);
    Wr_FAA0(frame, ((char*)self), 4);
    Wr_FAA0(frame, &self->m_SunRadius, 4);
    Wr_FAA0(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Surface_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Writes m_SnowSparkleDensity and then m_wpTravelDistance (4 bytes each) to a replay frame through the frame's virtual
 * write (slot 1 of its vtable, cReplayFramePtr_write for a cReplayFramePtr).
 *
 * Slot 64 of tWPInGameData_Surface's vtable tWPInGameData_Surface_vtbl; tWPInGameData_Surface_readFromReplayFrame reads them back in the same order.
 *
 * @param frame  the replay frame (a cReplayFramePtr)
 */
void tWPInGameData_Surface::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_SnowSparkleDensity, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Lighting_writeToReplayFrame);
#ifdef SKIP_ASM
struct VEnt_FD30 { short delta; short idx; void (*fn)(void*, void*, int); };
struct Stream_FD30 { char* vt; };

static inline void Wr_FD30(Stream_FD30* s, void* p, int n) {
    VEnt_FD30* e = (VEnt_FD30*)(s->vt + 0x8);
    e->fn(vthis(s, *e), p, n);
}

/**
 * Saves the lighting values to a replay frame.
 *
 * Writes the four 8-byte irradiance names (+0x8, +0x18, +0x28, +0x38) and the ambient and rim intensity offsets
 * (+0x48, +0x50), each followed by the 4-byte m_wpTravelDistance (+0x0), through the frame's slot 1 (write); the
 * m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
extern "C" void tWPInGameData_Lighting_writeToReplayFrame(tWPInGameData_Lighting* self, Stream_FD30* frame) {
    Wr_FD30(frame, &self->m_LightIrradianceName, 8);
    Wr_FD30(frame, ((char*)self), 4);
    Wr_FD30(frame, &self->m_ShadowIrradianceName, 8);
    Wr_FD30(frame, ((char*)self), 4);
    Wr_FD30(frame, &self->m_TunnelIrradianceName, 8);
    Wr_FD30(frame, ((char*)self), 4);
    Wr_FD30(frame, &self->m_ObjectIrradianceName, 8);
    Wr_FD30(frame, ((char*)self), 4);
    Wr_FD30(frame, &self->m_AmbientLightIntensityOffset, 4);
    Wr_FD30(frame, ((char*)self), 4);
    Wr_FD30(frame, &self->m_RimLightIntensityOffset, 4);
    Wr_FD30(frame, ((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Weather_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_FEB0 {
public:
    virtual int read(void* p, int n);
    virtual int write(void* p, int n);
};

/**
 * Saves the 19 weather values to a replay frame.
 *
 * For each value (+0x8 to +0x98 in steps of 8) writes it and then m_wpTravelDistance (+0x0), 4 bytes each, through
 * the frame's slot 1 (write). The local class names that slot "read" and the comment above says "Reads"; slot 1 of
 * cReplayFramePtr is write. The m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
// Reads 19 words into self (at +0x8 .. +0x98), each followed by one into +0x0
// (the read-side twin of tWPInGameData_Weather_readFromReplayFrame).
extern "C" void tWPInGameData_Weather_writeToReplayFrame(tWPInGameData_Weather* self, cStream_FEB0* frame)
{
    frame->read(&self->m_SnowIntensity, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_FlakeSize, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_WindIntensity, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_WindRotation, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_PlayerWindIntensity, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_Temperature, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFlurries, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowGravityMultiplier, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffItensity, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffWindIntensity, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_LightningChance, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFlakeColourA, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFlakeColourR, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFlakeColourG, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFlakeColourB, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffColourA, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffColourR, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffColourG, 4);
    frame->read(((char*)self), 4);
    frame->read(&self->m_SnowFluffColourB, 4);
    frame->read(((char*)self), 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", tWPInGameData_Danger_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves the danger values to a replay frame.
 *
 * Writes m_Level, m_wpTravelDistance (+0x0), m_PointRate and m_wpTravelDistance again, 4 bytes each, through the
 * frame's slot 1 (write); the m_last targets are not saved.
 *
 * @param frame  the replay frame to write to (On Tour cReplayFramePtr* data)
 */
void tWPInGameData_Danger::writeToReplayFrame(cReplayFramePtr* frame) const
{
    frame->write((const char*)&m_Level, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
    frame->write((const char*)&m_PointRate, 4);
    frame->write((const char*)&m_wpTravelDistance, 4);
}
#endif
