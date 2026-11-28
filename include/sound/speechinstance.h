#ifndef SOUND_SPEECHINSTANCE_H
#define SOUND_SPEECHINSTANCE_H

// cSpeechInstance and cSpeechBank: the per-stream halves of cSpeechSys (sound/speechsys.h). cSpeechSys owns two
// cSpeechInstance (cSpeechSys::mSpeechInstance[2], one per speech stream); each loads the speech event files and
// up to 500 speech banks (cSpeechBank, one per .hdr file) listed by data/config/speech.inf and plays requests on
// its cStreamSys stream. EA source d:/ssx/code/source/sound/speechsys.cpp (preview strings "SpeechBank",
// "SpeechInstance", "%sheaders.big", "%seventdat\events.evt", "mNumTotalBanks >= 0"); retail methods in the
// sound/bankmonitor unit 0x2AF8A8..0x2B07F8, just before cSpeechSys's. Neither class has a vtable.
// Class names from the allocation tags "SpeechInstance" / "SpeechBank". On Tour has no speech system.
// Sizes [R]: cSpeechSys cSpeechSys_CreateInstance = cSpeechInstance_cSpeechInstance(cMemMan_alloc(0x904, "SpeechInstance")); cSpeechBank_Create =
// cSpeechBank_cSpeechBank(cMemMan_alloc(0x4C, "SpeechBank")).
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] constructors cSpeechInstance_cSpeechInstance / cSpeechBank_cSpeechBank, destructors cSpeechInstance__cSpeechInstance / cSpeechBank__cSpeechBank, the speech.inf keys
//       cSpeechInstance_ReadConfig reads;
//   [U] how the retail methods use the field. Names marked "(our name)" describe that use; the rest are unk_.

#include "layout.h"

// cSpeechBank (0x4C): one loaded speech bank header (a "<name>.hdr" file).
struct cSpeechBank {
    int mBankId;       // 0x00 (our name) [R] ctor -1; cSpeechBank_Create = SPCH_addTable(data) (the driver's bank id);
                       //      [U] cSpeechBank_HasBankId compares it with a requested bank id; dtor SPCH_removeTable()s it
    char mName[0x3C];  // 0x04 (our name) [R] ctor clears byte 0; cSpeechBank_Create strcpy()s the bank path; [U] cSpeechInstance_PlayEvent builds the stream file name from it and its language suffix (SPCH_GetBankLanguage)
    char* mShortName;  // 0x40 (our name) [R] cSpeechBank_Create: the part of mName after the last '\\'; [U] cSpeechInstance_PlayEvent passes it to cSpeechSys's vtable slot 2
    void* mpData;      // 0x44 (our name) [R] ctor 0; cSpeechBank_Create: the .hdr data (passed in, or FILE_load()ed); dtor MEM_free()s it when mOwnsData
    int mOwnsData;     // 0x48 (our name) [R] ctor 0; cSpeechBank_Create 1 when it loaded mpData itself

    // Methods bound to their retail symbols by asm labels.
    bool HasBankId(int bankId) __asm__("cSpeechBank_HasBankId");
};

// One speech event file of a cSpeechInstance (8 bytes; our names).
struct tSpeechEventFile {
    void* mpData; // 0x0 [U] cSpeechInstance_LoadEventFile: the event data (located inside the loaded headers .big, or FILE_loadz("%seventdat\events.evt")); registered with SPCH_registerBank
    int mOwned;   // 0x4 [U] cSpeechInstance_LoadEventFile 1 when loaded on its own; dtor MEM_free()s mpData then
};

// cSpeechInstance (0x904): one speech stream.
struct cSpeechInstance {
    int mIndex;                        // 0x000 (our name) [R] ctor arg 1 = the instance's index in
                                       //       cSpeechSys::mSpeechInstance; [U] cSpeechInstance_PlayEvent passes it to
                                       //       cSpeechSys (request match cSpeechSys_ApplyRequest, vtable slots 2/3)
    int mStreamId;                     // 0x004 (our name) [R] ctor arg 2; [U] the cStreamSys stream id: STREAM_
                                       //       calls (cStreamSys_STREAM_Create in the ctor, cStreamSys_STREAM_Destroy
                                       //       in the dtor, cStreamSys_STREAM_Requests / cStreamSys_STREAM_Purge /
                                       //       cStreamSys_STREAM_TimeRemaining / cStreamSys_STREAM_Playing /
                                       //       cStreamSys_STREAM_SetVolControl) and STREAM_SetBigFile
                                       //       (cStreamSys_STREAM_SetBigFile, speech.inf BIGFILE)
    int unk_008;                       // 0x008 [R] ctor arg 6; no read found
    char unk_00C[0x60];                // 0x00C [R] ctor clears byte 0 (an empty string); no other access found
    char mBasePath[0x60];              // 0x06C [R] speech.inf BASEPATH (cSpeechInstance_ReadConfig AssignString); [U] cSpeechInstance_LoadEventFile / cSpeechInstance_ReadConfig sprintf the .big / events / bank paths from it
    tSpeechEventFile mEventFile[4];    // 0x0CC [R] ctor zeroes 4; [U] cSpeechInstance_LoadEventFile fills [mNumEventFiles]; dtor frees
    int mNumEventFiles;                // 0x0EC (our name) [R] ctor 0; [U] number of event files loaded (cSpeechInstance_ReadConfig
                                       //       ++ per speech.inf BASEPATH entry); dtor loop bound
    int mNumBanks;                     // 0x0F0 (our name) [R] ctor 0; [U] number of speech banks in mBanks
                                       //       (cSpeechInstance_AddBank ++, cSpeechInstance_mNumTotalBanks counts them all);
                                       //       dtor / cSpeechInstance_PlayEvent loop bound
    cSpeechBank* mBanks[500];          // 0x0F4 (our name) [U] cSpeechInstance_AddBank appends cSpeechBank_Create(...); cSpeechInstance_PlayEvent
                                       //       searches by bank id; dtor destroys them (cSpeechBank__cSpeechBank)
    int unk_8C4;                       // 0x8C4 [R] ctor 0; [U] set by func_002B07E8 (cSpeechSys func_002B0FE8)
    int mHoldTime;                     // 0x8C8 (our name) [R] ctor 0; [U] set by cSpeechInstance_SetHoldTime (cSpeechSys
                                       //       cSpeechSys_SetHoldTime); cSpeechInstance_PlayEvent passes it to cSpeechSys's play slot
                                       //       (vtable slot 1, cSSXAudio_PlayPositional) as holdTime on an idle stream
    int mPosition;                     // 0x8CC (our name; a t4Vector*) [R] ctor 0; [U] cSpeechSys cSpeechSys_TriggerSpeech /
                                       //       cSpeechSys_ApplyRequest (the request rider's getPosition()) set it; cSpeechInstance_PlayEvent
                                       //       passes it to the play slot as position
    int mUserInfo;                     // 0x8D0 (our name) [R] ctor 0; [U] set as mPosition (tSpeechRequest::unk_14);
                                       //       the play slot's userInfo; cSpeechInstance_PlayEvent / cSpeechSys_ApplyVolumeCallback also pass it to
                                       //       slot 3
    void* mHeaderBigData[4][2];        // 0x8D4 (our name) [U] cSpeechInstance_LoadEventFile: per event file, the loaded "%sheaders.big"
                                       //       ([.][0]) / "%slanghead.big" ([.][1]) data; dtor MEM_free()s them
    float mRadius;                     // 0x8F4 (our name) [R] ctor 0 (sw zero); [U] cSpeechSys cSpeechSys_TriggerSpeech /
                                       //       cSpeechSys_ApplyRequest store a float; cSpeechInstance_PlayEvent passes it to the play slot as
                                       //       radius (lwc1)
    int mIsHuman;                      // 0x8F8 (our name) [U] cSpeechSys cSpeechSys_ApplyRequest (the request rider's isHuman());
                                       //       cSpeechInstance_PlayEvent passes it to the play slot as isHuman
    int mViewport;                     // 0x8FC (our name) [U] cSpeechSys cSpeechSys_ApplyRequest (the request rider's mViewIndex,
                                       //       -1 without one); cSpeechInstance_PlayEvent passes it to the play slot as viewport
    int mReadyForSpeech;               // 0x900 (our name) [R] ctor 1; [U] set while the stream can take the next
                                       //       speech: cSpeechInstance_Update / cSpeechInstance_StartNextSpeech clear it once SPCH_startNextRequest
                                       //       starts one, cSpeechInstance_Update sets it again (SPCH_updateChannel) or while busy

    // Methods bound to their retail symbols by asm labels.
    void Update(int i) __asm__("cSpeechInstance_Update");
    int StartNextSpeech(int channel) __asm__("cSpeechInstance_StartNextSpeech");
    bool LoadEventFile(int isHeaders, char* name, int instIndex, int section) __asm__("cSpeechInstance_LoadEventFile");
    void ReadConfig(int instanceIndex, int section, int loadLangHead) __asm__("cSpeechInstance_ReadConfig");
    int CanTakeSpeech() __asm__("cSpeechInstance_CanTakeSpeech");
    void Purge() __asm__("cSpeechInstance_Purge");
    void AddBank(const char* bankPath, const char* shortName, void* headerData) __asm__("cSpeechInstance_AddBank");
    void IsPlaying() __asm__("cSpeechInstance_IsPlaying");
    void SetHoldTime(int holdValue) __asm__("cSpeechInstance_SetHoldTime");
};

CHECK_OFFSET(cSpeechBank, mBankId, 0x00);
CHECK_OFFSET(cSpeechBank, mName, 0x04);
CHECK_OFFSET(cSpeechBank, mShortName, 0x40);
CHECK_OFFSET(cSpeechBank, mpData, 0x44);
CHECK_OFFSET(cSpeechBank, mOwnsData, 0x48);
CHECK_SIZE(cSpeechBank, 0x4C);
CHECK_OFFSET(tSpeechEventFile, mpData, 0x0);
CHECK_OFFSET(tSpeechEventFile, mOwned, 0x4);
CHECK_SIZE(tSpeechEventFile, 0x8);
CHECK_OFFSET(cSpeechInstance, mIndex, 0x000);
CHECK_OFFSET(cSpeechInstance, mStreamId, 0x004);
CHECK_OFFSET(cSpeechInstance, unk_008, 0x008);
CHECK_OFFSET(cSpeechInstance, unk_00C, 0x00C);
CHECK_OFFSET(cSpeechInstance, mBasePath, 0x06C);
CHECK_OFFSET(cSpeechInstance, mEventFile, 0x0CC);
CHECK_OFFSET(cSpeechInstance, mNumEventFiles, 0x0EC);
CHECK_OFFSET(cSpeechInstance, mNumBanks, 0x0F0);
CHECK_OFFSET(cSpeechInstance, mBanks, 0x0F4);
CHECK_OFFSET(cSpeechInstance, unk_8C4, 0x8C4);
CHECK_OFFSET(cSpeechInstance, mHoldTime, 0x8C8);
CHECK_OFFSET(cSpeechInstance, mPosition, 0x8CC);
CHECK_OFFSET(cSpeechInstance, mUserInfo, 0x8D0);
CHECK_OFFSET(cSpeechInstance, mHeaderBigData, 0x8D4);
CHECK_OFFSET(cSpeechInstance, mRadius, 0x8F4);
CHECK_OFFSET(cSpeechInstance, mIsHuman, 0x8F8);
CHECK_OFFSET(cSpeechInstance, mViewport, 0x8FC);
CHECK_OFFSET(cSpeechInstance, mReadyForSpeech, 0x900);
CHECK_SIZE(cSpeechInstance, 0x904);

#endif /* SOUND_SPEECHINSTANCE_H */
