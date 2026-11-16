#ifndef MOVIE_MCOVERLAYMANAGER_H
#define MOVIE_MCOVERLAYMANAGER_H

// cMCOverlayManager: the memory-card save/load/delete state machine behind the front end's and the overlays'
// memory-card screens. One instance, made on demand by cMCOverlayManager_getManager__Fv
// (cMemMan_alloc(0x74C, "MCManager"), pointer in cMCOverlayManager_instance; cSSXApp_getMCManager returns it). Its methods live in
// src/movie/movieplayer.cpp (the unit mixes them with movie-player code; some local views there call the card
// object a "decoder"). The front end (cFEMemCard, fe/festatetrophyroom.cpp func_001D5488) plugs its handlers into
// the 45 callback slots at 0x000-0x0B0.
//
// A FLAT layout, like main/game.h: every field at its retail offset. The class has no base; g++ 2.95 puts its vptr
// after the data, at 0x748 (On Tour: after mFileListCache, at 0x4CC of 0x4D0).
// Statics (not in the object): cMCOverlayManager_mSaveInProgress (gp-relative int), the instance cMCOverlayManager_instance.
// Evidence tags:
//   [R] retail's constructor cMCOverlayManager_cMCOverlayManager (word stores, the default callback stubs,
//       MEM_fill sizes), init func_0023D5A8, SetAction, the vtable cMCOverlayManager_vtbl and getManager's alloc;
//   [P] the July 2003 preview's cMCOverlayManager::getMemCard (lw 0x330), carried to retail 0x434 by
//       tools/layout_xlate.py (10 votes);
//   [U] how matched retail code uses the field (movieplayer.cpp unless another unit is named);
//   [O] SSX On Tour's DWARF (class cMCOverlayManager, size 0x4D0, and cMCOverlayManager::sdevice): same member
//       order; On Tour's bools are 1 byte, retail's are words, and retail's strings and file cache are longer.
//       Every field that retail usage pins down lands exactly where that order (with 4-byte bools) predicts, so
//       [O] alone names only fields whose neighbours on both sides are placed by other evidence.

#include "layout.h"

struct cMemCard;

// cMCOverlayManager::sdevice (On Tour name), one record per memory-card device. [R] 0xE0 bytes: the ctor and
// GatherDeviceInfo1 MEM_fill(&mDevices[i], 0, 0xE0) and index by 0xE0.
struct cMCOverlayManager_sdevice {
    int bValid;                 // 0x000 [O]; [U] GatherDeviceInfo1 sets 1, clears it when the card reports an error
    int bFormatted;             // 0x004 [O]; [U] GatherDeviceInfo1 sets 1, clears it on !card->v56(i)
    int bCorrectEncoding;       // 0x008 [O]; [U] GatherDeviceInfo1 sets 1, clears it on !card->v57(i)
    int bPluggedIn;             // 0x00C [O]; [U] GatherDeviceInfo1 sets 1; GetDeviceTotalString returns when 0
    int iFreeSpace;             // 0x010 [O]; [U] GatherDeviceInfo1 stores card->v20() (the value getDeviceState
                                //       compares with v23() for "card full")
    int iTotalSpace;            // 0x014 [O]; [U] GetDeviceTotalString prints it as "%d %s" with "Total"
    unsigned short szName[100]; // 0x018 [O]; [U] GatherDeviceInfo1 clears 200 bytes and has the card write 100
                                //       chars; GetDeviceDisplayString strlen1 / strcpy1 (16-bit strings) it
};

CHECK_OFFSET(cMCOverlayManager_sdevice, bValid, 0x0);
CHECK_OFFSET(cMCOverlayManager_sdevice, bFormatted, 0x4);
CHECK_OFFSET(cMCOverlayManager_sdevice, bCorrectEncoding, 0x8);
CHECK_OFFSET(cMCOverlayManager_sdevice, bPluggedIn, 0xC);
CHECK_OFFSET(cMCOverlayManager_sdevice, iFreeSpace, 0x10);
CHECK_OFFSET(cMCOverlayManager_sdevice, iTotalSpace, 0x14);
CHECK_OFFSET(cMCOverlayManager_sdevice, szName, 0x18);
CHECK_SIZE(cMCOverlayManager_sdevice, 0xE0);

struct cMCOverlayManager {
    // --- 45 front-end callbacks. [R] the ctor fills all 45 words with default stubs whose kind matches On Tour's
    // return type slot for slot: void slots get func_00244860 (returns nothing), bool slots func_00244868 /
    // func_00244870 / func_00244878 (return 0 / 1 / 0). [U] func_001D5488 (fe/festatetrophyroom.cpp) stores
    // cFEMemCard_callback<Name> into the slots of the same name at 0x0C, 0x28, 0x30, 0x50, 0x54 and 0xA0.
    void (*callbackWriteStart)();                  // 0x000 [O][R]
    int (*callbackWriteUpdate)(int);               // 0x004 [O][R]; [U] UpdateSaveProgress calls it with an arg
    void (*callbackWriteEnd)();                    // 0x008 [O][R]; [U] UpdateSaveProgress
    void (*callbackSaveSuccess)();                 // 0x00C [O][R]; [U] = cFEMemCard_callbackSaveSuccess; UpdateSaveClosing
    void (*callbackConfirmSaveFailed)();           // 0x010 [O][R]
    void (*callbackLoadStart)();                   // 0x014 [O][R]
    void (*callbackReadStart)();                   // 0x018 [O][R]
    int (*callbackReadUpdate)(int);                // 0x01C [O][R]; [U] UpdateLoadProgress calls it with an arg
    int (*callbackReadEnd)();                      // 0x020 [O][R]; [U] UpdateLoadProgress
    void (*callbackConfirmLoadFailed)();           // 0x024 [O][R]
    void (*callbackReadSuccess)();                 // 0x028 [O][R]; [U] = cFEMemCard_callbackReadSuccess; UpdateLoadDoneWait
    void (*callbackReadFailed)();                  // 0x02C [O][R]; [U] UpdateLoadStart/Progress/DoneWait/Closing
    int (*callbackDeleteDone)();                   // 0x030 [O][R]; [U] = cFEMemCard_callbackDeleteDone; UpdateDeleteDone
    int (*callbackDeleteFailed)();                 // 0x034 [O][R]
    void (*callbackFileNotFound)();                // 0x038 [O][R]; [U] UpdateLoadFileNotFound
    void (*callbackConfirmNoCards)();              // 0x03C [O][R]
    void (*callbackFileCorrupt)();                 // 0x040 [O][R]
    void (*callbackConfirmOverwrite)();            // 0x044 [O][R]
    void (*callbackConfirmFormat)();               // 0x048 [O][R]
    void (*callbackConfirmFormatFail)();           // 0x04C [O][R]
    void (*callbackConfirmFormatDone)();           // 0x050 [O][R]; [U] = cFEMemCard_callbackConfirmFormatDone
    void (*callbackConfirmDelete)();               // 0x054 [O][R]; [U] = cFEMemCard_callbackConfirmDelete; the state
                                                   //       handler (vtable slot 1) sets mDelete, then calls it
    void (*callbackConfirmInvalidCard)();          // 0x058 [O][R]
    void (*callbackConfirmNoFreeBlocks)();         // 0x05C [O][R]
    void (*callbackConfirmSaveTooBig)();           // 0x060 [O][R]
    int (*callbackIsSavePressed)();                // 0x064 [O][R]; [U] UpdateSave
    int (*callbackIsDeletePressed)();              // 0x068 [O][R]; [U] UpdateSave (with mDeleteOk), UpdateDeleteConfirm
    int (*callbackIsCancelPressed)();              // 0x06C [O][R]; [U] 22 Update* states; -> CancelCardOp
    int (*callbackIsOverwritePressed)();           // 0x070 [O][R]; [U] UpdateOverwriteConfirm
    int (*callbackIsDeviceSelected)();             // 0x074 [O][R]
    int (*callbackIsLoadPressed)();                // 0x078 [O][R]; [U] UpdateLoadConfirm
    int (*callbackIsNextDevicePressed)();          // 0x07C [O][R]
    int (*callbackIsPrevDevicePressed)();          // 0x080 [O][R]
    int (*callbackIsRetryPressed)();               // 0x084 [O][R]; [U] UpdateCardDamaged / UpdateCardInvalid / ...
    int (*callbackIsFormatPressed)();              // 0x088 [O][R]; [U] UpdateFormatConfirm buffers it in mFormatPressedBuffered
    void (*callbackSaveFileCorrupt)();             // 0x08C [O][R]
    void (*callbackCardsChecked)();                // 0x090 [O][R]
    void (*callbackFileChecked)();                 // 0x094 [O][R]
    void (*callbackDisplayNoFiles)();              // 0x098 [O][R]; [U] UpdateInitFileCheckDevice
    void (*callbackDisplayNoCards)();              // 0x09C [O][R]; [U] UpdateInitFileCheckDevice
    void (*callbackFileExists)();                  // 0x0A0 [O][R]; [U] = cFEMemCard_callbackFileExists
    void (*callbackLoadReady)();                   // 0x0A4 [O][R]; [U] UpdateLoad
    void (*callbackCardChanged)();                 // 0x0A8 [O][R]
    void (*callbackSysRecheckFile)();              // 0x0AC [O][R]; [U] InitiateFileCheck
    int (*callbackAcknowledgeCardStatusChange)(int); // 0x0B0 [O][R]; [U] updateDeviceStateMonitor calls it with an arg
    // --- operation
    int mFileType;              // 0x0B4 [O] tMCFileType; [R] ctor 2; [U] func_0023C8F0 / func_0023CAE8 store their
                                //       type arg, pick mMaxFiles from it and pass it to func_00241D40 (On Tour
                                //       getFileName(tMCFileType, ...)); setTitleString switches on it
    int mAction;                // 0x0B8 [O] tMCOverlayManagerAction; [U] SetAction stores it, update switches on it
    int mSave;                  // 0x0BC [O] (bool); [U] func_0023CAE8 (used by the autosave overlay) sets 1,
                                //       func_0023C8F0 sets 0
    int mDelete;                // 0x0C0 [O] (bool); [U] the state handler sets 1 before callbackConfirmDelete
    int mFormatPressedBuffered; // 0x0C4 [O] (bool); [U] UpdateFormatConfirm sets it when callbackIsFormatPressed()
                                //       and consumes it later
    int mCardChanged;           // 0x0C8 [O] (bool); [U] UpdateSave: with mShowCardChangeNotify -> state 0x38, else
                                //       cleared and RecheckCard
    int mSaveOk;                // 0x0CC [O] (bool); [U] ManagerLoadSaveUpdate clears the four *Ok flags each frame;
                                //       UpdateSave reads it after callbackIsSavePressed()
    int mLoadOk;                // 0x0D0 [O] (bool); [U] ditto; UpdateLoad
    int mDeleteOk;              // 0x0D4 [O] (bool); [U] ditto; UpdateSave: callbackIsDeletePressed() && mDeleteOk
    int mCancelOk;              // 0x0D8 [O] (bool); [U] ditto; ManagerLoadSaveUpdate sets it in states 7 / 0x1D
    int mSaved;                 // 0x0DC [O] (bool); [U] func_0023C8B8 returns it (On Tour isManagerSaved);
                                //       func_00241380 clears it
    int mGetDir;                // 0x0E0 [O] (bool); [U] OnHighlightNext/PrevDevice clear it
    int mSavedSize;             // 0x0E4 [O]; [U] func_00241138 adds to it; UpdateSaveFile / UpdateSaveProgress set
                                //       mProgress = mSavedSize / mSize; the state handler zeroes it when the save starts
    int mLoadedSize;            // 0x0E8 [O]; [U] func_00241200 adds to it; UpdateLoadProgress divides it by mSize
    int mMaxFiles;              // 0x0EC [O]; [U] func_0023C8F0 / func_0023CAE8 store 4 / 6 / 1 by file type
    int mSize;                  // 0x0F0 [O]; [U] the divisor of both progress ratios
    int mCheckSize;             // 0x0F4 [O]; [R] ctor zeroes it
    int mFileCursor;            // 0x0F8 [O]; [U] the file slot index for the card calls (UpdateSave's 6-file loop
                                //       skips it; setTitleString / the create-file state use it)
    int mSaveProfileID;         // 0x0FC [O]; [U] func_0023C8F0 stores -1, func_0023CAE8 its profile arg;
                                //       setTitleString passes it to cBENewPlayerInterface_getPlayerName
    int mWrapItems;             // 0x100 [O] (bool); [U] OnHighlightPrevDevice wraps to mMaxDevices - 1 when set
    int mStackDepth;            // 0x104 [O]; [R] ctor and func_0023D5A8 zero it (no other access)
    int mDisplayWaitTicks;      // 0x108 [O]; [U] SetAction(1) and the state handler store 0x3C / 0x5A / 0x78;
                                //       the *Wait states count it down
    float mProgress;            // 0x10C [O]; [U] = mSavedSize / mSize, mLoadedSize / mSize
    float mFlash;               // 0x110 [O]; [U] SetAction(1), func_0023D5A8 and UpdateSave store 1.0
    float mFlashStep;           // 0x114 [O]; [U] stored with mFlash: 0.01667 (1/60)
    int mWriteInProgress;       // 0x118 [O] (bool); [U] the create-file state sets 1 before the card write;
                                //       WriteFailed / RetryCardOp read it
    int mPollForFullCard;       // 0x11C [O] (bool); [U] tested with the "full" bit (4) of mCurrDeviceStateLast
    int mPollForUnformatted;    // 0x120 [O] (bool); [U] tested with the "unformatted" bit (0x10)
    int mPollForNoCard;         // 0x124 [O] (bool); [U] tested with the "no card" bit (8)
    int mShowCardChangeNotify;  // 0x128 [O] (bool); [U] with mCardChanged -> state 0x38 (UpdateSave, UpdateCheckCards)
    int mRetryToInit;           // 0x12C [O] (bool); [U] RetryCardOp
    int mState;                 // 0x130 [O] tMCOverlayManagerState; [U] the state handler (vtable slot 1,
                                //       cMCOverlayManager_SetState) stores it; ManagerLoadSaveUpdate / fe code read it
    // --- 16-bit strings (func_002C2540 widens, strcpy1 / sprintf1 work on unsigned short)
    unsigned short mFileName[64];          // 0x134 [O]; [U] the create-file state sprintf1s "%s-%s%04d" (type, name,
                                           //       cursor + 1) into it and hands it to the card's write call
    unsigned short mMatch[64];             // 0x1B4 [O]; [U] func_0023C8F0 / func_0023CAE8 sprintf1 the wildcard
                                           //       "%s-%s*" into it; passed to the card's directory call
    unsigned short mActiveTitleString[33]; // 0x234 [O]; [R] ctor MEM_fill(0x234, 0, 0x42); [U] setTitleString
                                           //       strcpy1s the title into it; UpdateSave reads the cursor's file name into it
    unsigned short mNewFileName[65];       // 0x276 [O]; [U] UpdateSave compares each card file's name with it;
                                           //       festateprofile.cpp / ovstateprofile.cpp widen the profile name into it.
                                           //       Length not evidenced: runs to unk_2F8
    char unk_2F8[0x40];                    // 0x2F8 no access in retail; On Tour has mStateStack[16] (tMCOverlayManagerState)
                                           //       right before mCheckCard, and retail keeps mStackDepth
    // --- card checking
    int mCheckCard;             // 0x338 [O] (bool); [U] RecheckCard and the state handler set it; UpdateSave returns
                                //       while set; preview 0x260 (layout_xlate, 2 votes)
    int mCheckDeviceTicks;      // 0x33C [O]; [R] ctor 5; [U] ManagerLoadSaveUpdate counts it down and resets it to 5
    int mCheckAllowed;          // 0x340 [O] (bool); [R] ctor 0; [U] func_00241380 sets 1; fememcardcallbacks.cpp tests it
    cMCOverlayManager_sdevice mDevices[1]; // 0x344 [O]; [R] MEM_fill(&mDevices[0], 0, 0xE0); [U] indexed by device in
                                           //       GatherDeviceInfo1 / GetDevice*String / ManagerLoadSaveUpdate
    int mMaxDevices;            // 0x424 [O]; [R] ctor -1; [U] getMaxDevicesSupported returns it
    int mSelectedDevice;        // 0x428 [O]; [U] func_0023FB18 stores it; the fe passes it to GetDeviceDisplayString;
                                //       preview 0x328
    int mCurrCheckDevice;       // 0x42C [O]; [U] checkDevice stores its arg; getDeviceState compares it with mSelectedDevice
    int mLastCheckDevice;       // 0x430 [O]; [U] ManagerLoadSaveUpdate's device poll index (MEM_fill(&mDevices[it]))
    cMemCard* mpMemCard;        // 0x434 [P] getMemCard; [O]; [R] func_0023D5A8 stores func_002C22C8(), func_0023D5E8
                                //       frees and zeroes it; [U] every card call goes through its vtable
    int mCurrDeleteFile;        // 0x438 [O]; [R] ctor 0; [U] UpdateInitDeleteProgress
    int mCurrDeviceStateLast;   // 0x43C [O] sDeviceState (bits: 1 / 2 / 4 full / 8 no card / 0x10 unformatted);
                                //       [U] getDeviceState returns it while mCurrCheckDevice != mSelectedDevice
    int mCurrDeviceStateAck;    // 0x440 [O] (bool); [U] func_0023FAE0 clears it with mCurrDeviceStateLast on a
                                //       device change; CancelCardOp sets 1
    int mLastMountedDevice;     // 0x444 [O]; [R] ctor -1; [U] func_0023FAE0 compares it with mSelectedDevice, then copies
    char unk_448[0x300];        // 0x448 no access in retail; On Tour's mFileListCache[32][6] (ushort, 0x180) sits here,
                                //       retail's region is twice that
    void** mVtbl;               // 0x748 [R] _vt$17cMCOverlayManager cMCOverlayManager_vtbl (slot 1 cMCOverlayManager_SetState the state
                                //       handler, slot 2 the dtor cMCOverlayManager__cMCOverlayManager); preview 0x638
};

CHECK_OFFSET(cMCOverlayManager, callbackWriteStart, 0x0);
CHECK_OFFSET(cMCOverlayManager, callbackWriteUpdate, 0x4);
CHECK_OFFSET(cMCOverlayManager, callbackWriteEnd, 0x8);
CHECK_OFFSET(cMCOverlayManager, callbackSaveSuccess, 0xC);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmSaveFailed, 0x10);
CHECK_OFFSET(cMCOverlayManager, callbackLoadStart, 0x14);
CHECK_OFFSET(cMCOverlayManager, callbackReadStart, 0x18);
CHECK_OFFSET(cMCOverlayManager, callbackReadUpdate, 0x1C);
CHECK_OFFSET(cMCOverlayManager, callbackReadEnd, 0x20);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmLoadFailed, 0x24);
CHECK_OFFSET(cMCOverlayManager, callbackReadSuccess, 0x28);
CHECK_OFFSET(cMCOverlayManager, callbackReadFailed, 0x2C);
CHECK_OFFSET(cMCOverlayManager, callbackDeleteDone, 0x30);
CHECK_OFFSET(cMCOverlayManager, callbackDeleteFailed, 0x34);
CHECK_OFFSET(cMCOverlayManager, callbackFileNotFound, 0x38);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmNoCards, 0x3C);
CHECK_OFFSET(cMCOverlayManager, callbackFileCorrupt, 0x40);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmOverwrite, 0x44);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmFormat, 0x48);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmFormatFail, 0x4C);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmFormatDone, 0x50);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmDelete, 0x54);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmInvalidCard, 0x58);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmNoFreeBlocks, 0x5C);
CHECK_OFFSET(cMCOverlayManager, callbackConfirmSaveTooBig, 0x60);
CHECK_OFFSET(cMCOverlayManager, callbackIsSavePressed, 0x64);
CHECK_OFFSET(cMCOverlayManager, callbackIsDeletePressed, 0x68);
CHECK_OFFSET(cMCOverlayManager, callbackIsCancelPressed, 0x6C);
CHECK_OFFSET(cMCOverlayManager, callbackIsOverwritePressed, 0x70);
CHECK_OFFSET(cMCOverlayManager, callbackIsDeviceSelected, 0x74);
CHECK_OFFSET(cMCOverlayManager, callbackIsLoadPressed, 0x78);
CHECK_OFFSET(cMCOverlayManager, callbackIsNextDevicePressed, 0x7C);
CHECK_OFFSET(cMCOverlayManager, callbackIsPrevDevicePressed, 0x80);
CHECK_OFFSET(cMCOverlayManager, callbackIsRetryPressed, 0x84);
CHECK_OFFSET(cMCOverlayManager, callbackIsFormatPressed, 0x88);
CHECK_OFFSET(cMCOverlayManager, callbackSaveFileCorrupt, 0x8C);
CHECK_OFFSET(cMCOverlayManager, callbackCardsChecked, 0x90);
CHECK_OFFSET(cMCOverlayManager, callbackFileChecked, 0x94);
CHECK_OFFSET(cMCOverlayManager, callbackDisplayNoFiles, 0x98);
CHECK_OFFSET(cMCOverlayManager, callbackDisplayNoCards, 0x9C);
CHECK_OFFSET(cMCOverlayManager, callbackFileExists, 0xA0);
CHECK_OFFSET(cMCOverlayManager, callbackLoadReady, 0xA4);
CHECK_OFFSET(cMCOverlayManager, callbackCardChanged, 0xA8);
CHECK_OFFSET(cMCOverlayManager, callbackSysRecheckFile, 0xAC);
CHECK_OFFSET(cMCOverlayManager, callbackAcknowledgeCardStatusChange, 0xB0);
CHECK_OFFSET(cMCOverlayManager, mFileType, 0xB4);
CHECK_OFFSET(cMCOverlayManager, mAction, 0xB8);
CHECK_OFFSET(cMCOverlayManager, mSave, 0xBC);
CHECK_OFFSET(cMCOverlayManager, mDelete, 0xC0);
CHECK_OFFSET(cMCOverlayManager, mFormatPressedBuffered, 0xC4);
CHECK_OFFSET(cMCOverlayManager, mCardChanged, 0xC8);
CHECK_OFFSET(cMCOverlayManager, mSaveOk, 0xCC);
CHECK_OFFSET(cMCOverlayManager, mLoadOk, 0xD0);
CHECK_OFFSET(cMCOverlayManager, mDeleteOk, 0xD4);
CHECK_OFFSET(cMCOverlayManager, mCancelOk, 0xD8);
CHECK_OFFSET(cMCOverlayManager, mSaved, 0xDC);
CHECK_OFFSET(cMCOverlayManager, mGetDir, 0xE0);
CHECK_OFFSET(cMCOverlayManager, mSavedSize, 0xE4);
CHECK_OFFSET(cMCOverlayManager, mLoadedSize, 0xE8);
CHECK_OFFSET(cMCOverlayManager, mMaxFiles, 0xEC);
CHECK_OFFSET(cMCOverlayManager, mSize, 0xF0);
CHECK_OFFSET(cMCOverlayManager, mCheckSize, 0xF4);
CHECK_OFFSET(cMCOverlayManager, mFileCursor, 0xF8);
CHECK_OFFSET(cMCOverlayManager, mSaveProfileID, 0xFC);
CHECK_OFFSET(cMCOverlayManager, mWrapItems, 0x100);
CHECK_OFFSET(cMCOverlayManager, mStackDepth, 0x104);
CHECK_OFFSET(cMCOverlayManager, mDisplayWaitTicks, 0x108);
CHECK_OFFSET(cMCOverlayManager, mProgress, 0x10C);
CHECK_OFFSET(cMCOverlayManager, mFlash, 0x110);
CHECK_OFFSET(cMCOverlayManager, mFlashStep, 0x114);
CHECK_OFFSET(cMCOverlayManager, mWriteInProgress, 0x118);
CHECK_OFFSET(cMCOverlayManager, mPollForFullCard, 0x11C);
CHECK_OFFSET(cMCOverlayManager, mPollForUnformatted, 0x120);
CHECK_OFFSET(cMCOverlayManager, mPollForNoCard, 0x124);
CHECK_OFFSET(cMCOverlayManager, mShowCardChangeNotify, 0x128);
CHECK_OFFSET(cMCOverlayManager, mRetryToInit, 0x12C);
CHECK_OFFSET(cMCOverlayManager, mState, 0x130);
CHECK_OFFSET(cMCOverlayManager, mFileName, 0x134);
CHECK_OFFSET(cMCOverlayManager, mMatch, 0x1B4);
CHECK_OFFSET(cMCOverlayManager, mActiveTitleString, 0x234);
CHECK_OFFSET(cMCOverlayManager, mNewFileName, 0x276);
CHECK_OFFSET(cMCOverlayManager, unk_2F8, 0x2F8);
CHECK_OFFSET(cMCOverlayManager, mCheckCard, 0x338);
CHECK_OFFSET(cMCOverlayManager, mCheckDeviceTicks, 0x33C);
CHECK_OFFSET(cMCOverlayManager, mCheckAllowed, 0x340);
CHECK_OFFSET(cMCOverlayManager, mDevices, 0x344);
CHECK_OFFSET(cMCOverlayManager, mMaxDevices, 0x424);
CHECK_OFFSET(cMCOverlayManager, mSelectedDevice, 0x428);
CHECK_OFFSET(cMCOverlayManager, mCurrCheckDevice, 0x42C);
CHECK_OFFSET(cMCOverlayManager, mLastCheckDevice, 0x430);
CHECK_OFFSET(cMCOverlayManager, mpMemCard, 0x434);
CHECK_OFFSET(cMCOverlayManager, mCurrDeleteFile, 0x438);
CHECK_OFFSET(cMCOverlayManager, mCurrDeviceStateLast, 0x43C);
CHECK_OFFSET(cMCOverlayManager, mCurrDeviceStateAck, 0x440);
CHECK_OFFSET(cMCOverlayManager, mLastMountedDevice, 0x444);
CHECK_OFFSET(cMCOverlayManager, unk_448, 0x448);
CHECK_OFFSET(cMCOverlayManager, mVtbl, 0x748);
CHECK_SIZE(cMCOverlayManager, 0x74C);

#endif /* MOVIE_MCOVERLAYMANAGER_H */
