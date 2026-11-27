#ifndef INSEQ_SCRIPTPLAYERMAIN_H
#define INSEQ_SCRIPTPLAYERMAIN_H

// cScriptPlayerMain: the player of one in-game sequence (InSeq) script: play state, current frame and speed, and one
// cScriptPlayerEntity per entity of the script (inseq/scriptplayerentity.h). cInSeq owns a fixed pool of them inside
// its tScriptMainNode list. No virtual functions, no base class.
// Size 0x48 [R]: cInSeq_cInSeq allocates maxScripts x 0x58 nodes (func_002724C8) and runs this constructor
// (cScriptPlayerMain_cScriptPlayerMain) on node + 8; the node's next/prev links follow at node + 0x50 / + 0x54 (On Tour tScriptMainNode:
// index, script, pNext, pPrev; retail has one more word at the node's start). On Tour's class is 0x44: its two bools
// at 0x3C/0x3D are 1 byte; retail's are words (the constructor zeroes 0x3C, 0x40 and 0x44 separately).
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert or out-of-line accessor for this class), hence inseq/. Retail code: unit replay/replaycache
// 0x00274518-0x002752E8.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the constructor cScriptPlayerMain_cScriptPlayerMain (stores every field) and cInSeq_cInSeq;
//   [U] how retail code uses the field (asm of the 22 methods listed in methods.txt);
//   [O] SSX On Tour's DWARF (class cScriptPlayerMain): same member order and offsets up to mNumSteps (0x38).

#include "layout.h"

struct cInSeq;
struct cInSeqComm;          // On Tour: interface, vptr only (OnInSeqPlay, OnInSeqUpdate, OnInSeqPlayStopping, OnInSeqStop,
                            // OnInSeqLoop, OnInSeqRequestEntityCommInterface, ...); cBXScriptEngine implements it
struct cScriptPlayerEntity;
struct cInSeqCommEntity;    // inseq/scriptplayerentity.h

// mPlayState values (On Tour eScriptPlayState). [U] Play(bool) 0 -> 1; DoUpdateFrame 1 -> 2 (calls OnInSeqPlay), 2 -> 3;
// UpdateFrame/DoUpdateFrame stop at the end in state 4.
enum eScriptPlayState {
    keScriptPlayState_Stopped = 0,
    keScriptPlayState_PlayPending = 1,
    keScriptPlayState_PlayExecuting = 2,
    keScriptPlayState_Playing = 3,
    keScriptPlayState_PlayEnding = 4
};

// The script data's header (On Tour tInSeqScriptHeader, 0x10).
struct tInSeqScriptHeader {
    unsigned char isSmallEndian;      // 0x0 [O]
    unsigned char numEntities;        // 0x1 [O]; [U] Init/DeInit/Stop/DoUpdate loop to it; cScriptPlayerMain_GetNumEntities
    unsigned short headerSize;        // 0x2 [O]; [U] entity blocks start at this + headerSize; >= 0xD = has a data header
    unsigned int projectID;           // 0x4 [O]; [U] cScriptPlayerEntity GetProjectID; cScriptPlayerMain_GetProjectID
    unsigned int size;                // 0x8 [O]; [U] Init returns it; cScriptPlayerMain_GetDataSize
    char nextSectionStart;            // 0xC [O] the data header (mpDataHeader) when headerSize >= 0xD
    char dummyPad[3];                 // 0xD [O]
};

struct cScriptPlayerMain {
    int mID;                               // 0x00 [O]; [R] ctor -1; [U] Init's ID argument; DeInit -1
    const char* mpData;                    // 0x04 [O]; [R] ctor 0; [U] Init stores pScriptData; DeInit tests it
    cInSeq* mpManager;                     // 0x08 [O]; [U] Init's manager argument; the entity array and the channels
                                           //      come from its heap (mpManager + 8, cInSeq::mHeap);
                                           //      GetInSeqCommInterface falls back to its mpCommInterface (+0)
    void* mpUserData;                      // 0x0C [O]; [R] ctor 0; [U] SetUserData (cScriptPlayerMain_SetUserData) swaps it
    tInSeqScriptHeader* mpSystemHeader;    // 0x10 [O]; [U] Init = pScriptData and returns ->size; numEntities loops
    const char* mpDataHeader;              // 0x14 [O]; [U] Init: &mpSystemHeader->nextSectionStart when headerSize >= 0xD
    cScriptPlayerEntity* mpEntityPlayers;  // 0x18 [O]; [U] Init: new[] numEntities x 0x3C + entity ctor, then Init on
                                           //      each; GetEntity (cScriptPlayerMain_GetEntity) = + i * 0x3C; DeInit deletes them
    int mLoopScript;                       // 0x1C [O] (On Tour bool); [U] Init 0; Play(bool)/SetLoop store it; Play()
                                           //      passes it; Preview saves/restores it
    int mPlayState;                        // 0x20 [O] eScriptPlayState; [U] Play(bool) 0 -> 1; Stop 0; DoUpdateFrame
    float mPlaySpeed;                      // 0x24 [O]; [R] ctor 1.0f; [U] SetPlaySpeed swc1; Preview saves/restores it
    float mCurrFrame;                      // 0x28 [O]; [U] DoUpdate swc1; UpdateFrame/GetNumUpdatesToEndLeft lwc1;
                                           //      DoUpdateFrame stores 0.0f (sw $0)
    int mNumPaused;                        // 0x2C [O]; [U] Pause ++, Resume --; UpdateFrame/DoUpdateFrame skip while > 0
    int mScriptLength;                     // 0x30 [O]; [R] ctor -1; [U] GetScriptLength (cScriptPlayerMain_GetScriptLength) caches the
                                           //      longest entity's numFrames; Stop -1
    int mJumpFrame;                        // 0x34 [O]; [R] ctor -1; [U] Init/Stop -1; GoToFrame stores it; UpdateFrame
                                           //      consumes it
    int mNumSteps;                         // 0x38 [O]; [U] Init/Stop 0; GoToFrame 0, StepNextFrame ++; UpdateFrame
    int mEntityUpdateInProgress;           // 0x3C [O] (bool); [R] ctor 0; [U] DoUpdateFrame sets/clears it around the
                                           //      entity updates; Play(bool)/Stop test it
    int mDoStopInEntityUpdate;             // 0x40 [O] (bool); [R] ctor 0; [U] Stop sets it while an entity update is
                                           //      in progress; Play(bool) clears it; DoUpdateFrame acts on it
    cInSeqComm* mpPreviewCommInterface;    // 0x44 [O]; [R] ctor 0; [U] Preview stores/clears it;
                                           //      GetInSeqCommInterface (cScriptPlayerMain_GetInSeqCommInterface) prefers it

    // Methods bound to their retail symbols by asm labels.
    void DeInit() __asm__("cScriptPlayerMain_DeInit");
    void SetPlaySpeed(float val) __asm__("cScriptPlayerMain_SetPlaySpeed__FPvf");
    void* Play() __asm__("cScriptPlayerMain_Play__FPv");
    void SetLoop(int loopScript) __asm__("cScriptPlayerMain_SetLoop");
    void Preview(int flag) __asm__("cScriptPlayerMain_Preview");
    void GoToFrame(int frameNum) __asm__("cScriptPlayerMain_GoToFrame");
    void StepNextFrame() __asm__("cScriptPlayerMain_StepNextFrame");
    void Stop() __asm__("cScriptPlayerMain_Stop");
    void Pause() __asm__("cScriptPlayerMain_Pause");
    void Resume() __asm__("cScriptPlayerMain_Resume");
    void UpdateFrame() __asm__("cScriptPlayerMain_UpdateFrame");
    int SetUserData(int userData) __asm__("cScriptPlayerMain_SetUserData__FPvi");
    void* GetEntity(int entityIndex) __asm__("cScriptPlayerMain_GetEntity");
    int GetScriptLength() __asm__("cScriptPlayerMain_GetScriptLength");
    int GetNumUpdatesToEndLeft() __asm__("cScriptPlayerMain_GetNumUpdatesToEndLeft");
    int GetInSeqCommInterface() __asm__("cScriptPlayerMain_GetInSeqCommInterface");
    int DoUpdate(float time, float prev, float speed) __asm__("cScriptPlayerMain_DoUpdate");
    int Init(tInSeqScriptHeader* data, char* owner, int playerId) __asm__("cScriptPlayerMain_Init");
    void Play1(int loopScript) __asm__("cScriptPlayerMain_Play1");
    void DoUpdateFrame() __asm__("cScriptPlayerMain_DoUpdateFrame");
};

CHECK_OFFSET(tInSeqScriptHeader, isSmallEndian, 0x0);
CHECK_OFFSET(tInSeqScriptHeader, numEntities, 0x1);
CHECK_OFFSET(tInSeqScriptHeader, headerSize, 0x2);
CHECK_OFFSET(tInSeqScriptHeader, projectID, 0x4);
CHECK_OFFSET(tInSeqScriptHeader, size, 0x8);
CHECK_OFFSET(tInSeqScriptHeader, nextSectionStart, 0xC);
CHECK_OFFSET(tInSeqScriptHeader, dummyPad, 0xD);
CHECK_SIZE(tInSeqScriptHeader, 0x10);

CHECK_OFFSET(cScriptPlayerMain, mID, 0x00);
CHECK_OFFSET(cScriptPlayerMain, mpData, 0x04);
CHECK_OFFSET(cScriptPlayerMain, mpManager, 0x08);
CHECK_OFFSET(cScriptPlayerMain, mpUserData, 0x0C);
CHECK_OFFSET(cScriptPlayerMain, mpSystemHeader, 0x10);
CHECK_OFFSET(cScriptPlayerMain, mpDataHeader, 0x14);
CHECK_OFFSET(cScriptPlayerMain, mpEntityPlayers, 0x18);
CHECK_OFFSET(cScriptPlayerMain, mLoopScript, 0x1C);
CHECK_OFFSET(cScriptPlayerMain, mPlayState, 0x20);
CHECK_OFFSET(cScriptPlayerMain, mPlaySpeed, 0x24);
CHECK_OFFSET(cScriptPlayerMain, mCurrFrame, 0x28);
CHECK_OFFSET(cScriptPlayerMain, mNumPaused, 0x2C);
CHECK_OFFSET(cScriptPlayerMain, mScriptLength, 0x30);
CHECK_OFFSET(cScriptPlayerMain, mJumpFrame, 0x34);
CHECK_OFFSET(cScriptPlayerMain, mNumSteps, 0x38);
CHECK_OFFSET(cScriptPlayerMain, mEntityUpdateInProgress, 0x3C);
CHECK_OFFSET(cScriptPlayerMain, mDoStopInEntityUpdate, 0x40);
CHECK_OFFSET(cScriptPlayerMain, mpPreviewCommInterface, 0x44);
CHECK_SIZE(cScriptPlayerMain, 0x48);

// cInSeqComm: the interface the script player calls back on (On Tour class cInSeqComm: pure virtuals only, in this
// slot order: OnInSeqPlay 1, OnInSeqUpdate 2, OnInSeqPlayStopping 3, OnInSeqStop 4, OnInSeqLoop 5,
// OnInSeqRequestEntityCommInterface 6, ...; the vptr is its only data, at 0x0). cBXScriptEngine implements it at
// +0x2AC (retail tables D_00482450 / D_00482100: 11 slots). [U] cScriptPlayerMain_Stop calls slot 4,
// cScriptPlayerMain_DoUpdate slot 2, cScriptPlayerEntity_SetupPlay slot 6. Parameters [O] (On Tour's
// cSSXScriptEngine / cBXScriptEngine overrides); slots 7.. are not declared (no retail caller seen).
struct cInSeqComm {
    // 0x0: the vptr
    virtual void OnInSeqPlay(cScriptPlayerMain* pPlayer) = 0;                                   // 1
    virtual void OnInSeqUpdate(cScriptPlayerMain* pPlayer, float currFrame, float frameDelta) = 0; // 2
    virtual void OnInSeqPlayStopping(cScriptPlayerMain* pPlayer) = 0;                           // 3
    virtual void OnInSeqStop(cScriptPlayerMain* pPlayer) = 0;                                   // 4
    virtual bool OnInSeqLoop(cScriptPlayerMain* pPlayer) = 0;                                   // 5
    virtual cInSeqCommEntity* OnInSeqRequestEntityCommInterface(cScriptPlayerEntity* pEntity, int type) = 0; // 6
};

CHECK_SIZE(cInSeqComm, 0x4);

#endif /* INSEQ_SCRIPTPLAYERMAIN_H */
