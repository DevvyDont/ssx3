#ifndef ANIMATION_ANIMDATABASE_H
#define ANIMATION_ANIMDATABASE_H

// cAnimDataBase: the table of loaded animation banks (one cAdfDataSource per bank id), reached through the class static
// cAnimDataBase::TheAnimDataBase (retail cAnimDataBase_TheAnimDataBase; cSSXApp::AnimDataBase holds the same object).
// EA's header: animation/animdatabase.h ([P] the July 2003 preview's assert path, with the inline accessor's asserts
// "TheAnimDataBase" and "id != 255"). Methods in animation/animmodel (cAnimDataBase::cAnimDataBase, __cAnimDataBase,
// initChanCache, purgeChanCache).
// Size 0x3FC: [R] cSSXApp_init: cAnimDataBase::cAnimDataBase(cMemMan_alloc(0x3FC, "AnimDataBase")); [O] On Tour class
// cAnimDataBase (0x3FC: cAdfDataSource* DataSources[255]; static cAnimDataBase* TheAnimDataBase).
// [U] the anim code indexes it by an anim id's low byte (rideranimbase: getAIAnimDuration, getAIAnimEventTime, onInput;
// ai: cAI_purgeAnims) and reads the bank's Anims (+0x8) and Tags (+0x10).

#include "layout.h"

struct cAdfDataSource;

void operator_delete(int* p);

struct cAnimDataBase {
    cAdfDataSource* DataSources[255];   // 0x000 [O]; [P] "id != 255"; [U] DataSources[id & 0xFF] -> the bank

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cAnimDataBase();
    ~cAnimDataBase();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

CHECK_OFFSET(cAnimDataBase, DataSources, 0x000);
CHECK_SIZE(cAnimDataBase, 0x3FC);

// [O] static cAnimDataBase* cAnimDataBase::TheAnimDataBase (On Tour globals: cAnimDataBase *); [P] assert
// "TheAnimDataBase"; [U] loaded, then indexed (lw (id & 0xFF) << 2).
extern cAnimDataBase* cAnimDataBase_TheAnimDataBase;

#endif /* ANIMATION_ANIMDATABASE_H */
