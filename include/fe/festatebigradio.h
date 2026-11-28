#ifndef FE_FESTATEBIGRADIO_H
#define FE_FESTATEBIGRADIO_H

// cFEStateRequestLine: the "request line" (radio playlist) screen: eight visible song rows with check boxes, the
// current song / artist, and in the store (opened from the shop) the song prices, the free-song markers and the
// player's cash. The playlist and the owned songs are 64-bit masks (one bit per song) kept by the BE reward
// interface (cBERewardInterface_getPlayList / _getOwnedSongList).
// EA's source is fe/festatebigradio.cpp (the July 2003 preview's assert strings; builds/x/v2 file_hints for retail
// cFEStateRequestLine_cFEStateRequestLine, _onGainFocus and _buySong). Retail code: fe/festateaudiooptions
// (0x00196B90 ... 0x00196F50), fe/messagecenter (0x00196F98 ... 0x00198AE8) and the out-of-line inlines in
// fe/fepopupmisc (0x001E0578 ... 0x001E0708).
//
// Size 0x210:
//   [R] cMemMan_alloc(528, "cFEStateRequestLine") + cFEStateRequestLine_cFEStateRequestLine(mem, engine, userNum,
//       inStore) in cFECustom_getNextState (fe/fereal) and cFEStateAudioOptions_onWidgetEvent (fe/festateaudiooptions);
//   [O] On Tour's DWARF says 0x210 too, and its members fall on every retail use at the same offsets (its one bool,
//       mInStore at 0x16C, is followed by a pointer, so a 4-byte bool moves nothing).
// Base: cFEState (On Tour 0x48, no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at
// 0x8). Constructors cFEStateRequestLine_cFEStateRequestLine (cUIState_cUIState, mHashName 17,
// cFEStateRequestLine_vtbl = D_00469378, 24 slots: 1 dtor, 4 onCreateScreen, 5 onDestroyScreen, 6 onGainFocus, 7
// onGainTransition, 17 onPopupEvent, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu) and the default
// cFEStateRequestLine_cFEStateRequestLine1; destructor cFEStateRequestLine__cFEStateRequestLine__FPv (cFEState_vtbl,
// cUIState__cUIState).
//
// Derives from cFEState (fe/festate.h); its own members at their retail offsets. Note that ee-gcc's `long` is 64-bit:
// the two song masks are `unsigned long` here, as in the C bodies (`long long` is 16
// bytes in ee-gcc).
// Evidence tags:
//   [R] retail's constructor / destructor named above;
//   [P] the July 2003 preview (layout_xlate: onCreateScreen 0x148 -> 0x14C, 0x170 -> 0x170);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFEStateRequestLine, cUIState, cListNode): names, types and order.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIText;
struct cUIIcon;
struct cUIMenu;
struct cTextureElement;

struct cFEStateRequestLine : public cFEState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRequestLine_vtbl (both ctors), cFEState_vtbl (dtor)
    // cFEStateRequestLine's notes on cUIState's fields:
    //   mpEngine: [U] cFECustom at *mpEngine, the texture bank at +0x58, the state stack at +0x18
    //   mCtrlBits: [R] cFECustom_getPlayerCtrlMask(.., userNum) when there is a cFECustom
    //   mAttrState: [U] onPopupEvent's flag word
    //   mUserNum: [U] the BE interfaces' user index
    // cFEState (fe/festate.h, 0x48)
    // --- cFEStateRequestLine
    int mCharID;                    // 0x048 [O]; [R] cBENewPlayerInterface_getPlayerCharID(.., mUserNum)
    int mSongOrder[64];             // 0x04C [O]; [R] MEM_fill(this + 0x4C, 0, 0x100); [U] generatePlayList fills it,
                                    //       the rows index it (mSongOrder[mFirstVisibleSong + row])
    int mNumSongs;                  // 0x14C [O]; [P] preview 0x148; [R] the audio object's word +0x504;
                                    //       [U] onCreateScreen passes its low byte (lbu) to cUIMenu_setScrollableMenu
    int mFirstVisibleSong;          // 0x150 [O]; [R] 0; [U] the top row (onWidgetMenu scrolls it)
    int mNumSongsInPlayList;        // 0x154 [O] only: no retail access (getNumSongsInPlaylist counts mPlaylist's bits)
    unsigned long mPlaylist;        // 0x158 [O] PORT: 64-bit (long is 8 bytes on the EE); [R] 0, then
                                    //       cBERewardInterface_getPlayList (ld/sd); [U] selectSong / unselectSong /
                                    //       isSongSelected / isEmptyPlaylist / isOnlySongInPlaylist
    unsigned long mOwnedSong;       // 0x160 [O] PORT: 64-bit; [R] cBERewardInterface_getOwnedSongList (ld/sd);
                                    //       [U] isSongOwned / ownSong / getNumOwnedSongs, the free-song markers
    int mMoney;                     // 0x168 [O]; [R] cBEEconInterface_getMoney(.., mUserNum, mCharID);
                                    //       [U] "you_have_answer"
    int mInStore;                   // 0x16C [O] (bool); [R] the ctor's inStore; [U] isInStore (lw); hides the store
                                    //       widgets
    cUIMenu* mpRadioMenu;           // 0x170 [O]; [P] preview 0x170; [R] 0; [U] onWidgetCreate "Menu0000"
    cUIText* mpSongTitles[8];       // 0x174 [O]; [R] 0 x8; [U] the "songs%d" rows (misc data = row)
    cUIIcon* mpCheckMarks[8];       // 0x194 [O]; [R] 0 x8; [U] the "%dbox" check boxes (mpCheckBox / mpEmptyBox)
    cUIText* mpCurSongTitle;        // 0x1B4 [O]; [R] 0; [U] "CurrentSong"
    cUIText* mpCurSongArtist;       // 0x1B8 [O]; [R] 0; [U] "CurrentArtist"
    cUIIcon* mpFreeSongMarker[6];   // 0x1BC [O]; [R] 0 x6; [U] "%dfree", shown in the store for unused free songs
    cUIText* mpArtist;              // 0x1D4 [O]; [R] 0; [U] "song_by"
    cUIText* mpCost;                // 0x1D8 [O]; [R] 0; [U] "cost_answer"
    cUIText* mpCostText;            // 0x1DC [O]; [R] 0; [U] "cost"
    cUIText* mpCash;                // 0x1E0 [O]; [R] 0; [U] "you_have_answer", updateCash
    cUIText* mpNumSongs;            // 0x1E4 [O]; [R] 0; [U] "song_list" (kT_OVRCMNSongsInList), updatePlayListCounter
    cUIText* mpHelpText;            // 0x1E8 [O]; [R] 0; [U] "helptext", updateHelpText
    cUIIcon* mpArrowUp;             // 0x1EC [O]; [R] 0; [U] "arr_up", updatePlayListArrows
    cUIIcon* mpArrowDown;           // 0x1F0 [O]; [R] 0; [U] "arr_down"
    cUIText* mpSelectButtonText;    // 0x1F4 [O]; [R] 0; [U] "btext_select", updateButtonsText
    cUIIcon* mpSelectButtonIcon;    // 0x1F8 [O]; [R] 0; [U] the platform button icon D_004A18A0
    cUIText* mpPreviewButtonText;   // 0x1FC [O]; [R] 0; [U] "btext_preview"
    cUIIcon* mpPreviewButtonIcon;   // 0x200 [O]; [R] 0; [U] the platform button icon D_004A18A4
    cTextureElement* mpCheckBox;    // 0x204 [O]; [R] the texture bank's "checkbox" element
    cTextureElement* mpEmptyBox;    // 0x208 [O]; [R] "empty_box"
    cTextureElement* mpDollarSign;  // 0x20C [O]; [R] "yell_dollar_sign"

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRequestLine1() __asm__("cFEStateRequestLine_cFEStateRequestLine1");
    int isInStore() __asm__("cFEStateRequestLine_isInStore");
    int countBits(unsigned long bits) __asm__("cFEStateRequestLine_countBits");
    void selectSong(int bit) __asm__("cFEStateRequestLine_selectSong");
    void unselectSong(int bit) __asm__("cFEStateRequestLine_unselectSong");
    bool isEmptyPlaylist() __asm__("cFEStateRequestLine_isEmptyPlaylist");
    bool isOnlySongInPlaylist(int bit) __asm__("cFEStateRequestLine_isOnlySongInPlaylist");
    int isSongSelected(int bit) __asm__("cFEStateRequestLine_isSongSelected");
    int getNumSongsInPlaylist() __asm__("cFEStateRequestLine_getNumSongsInPlaylist");
    int isSongOwned(int bit) __asm__("cFEStateRequestLine_isSongOwned");
    void ownSong(int bit) __asm__("cFEStateRequestLine_ownSong");
    int getNumOwnedSongs() __asm__("cFEStateRequestLine_getNumOwnedSongs");
    void generatePlayList() __asm__("cFEStateRequestLine_generatePlayList");
    void updatePlayList() __asm__("cFEStateRequestLine_updatePlayList");
    void updatePlayListSongTitles() __asm__("cFEStateRequestLine_updatePlayListSongTitles");
    void updatePlayListCheckMarks() __asm__("cFEStateRequestLine_updatePlayListCheckMarks");
    void updatePlayListArrows() __asm__("cFEStateRequestLine_updatePlayListArrows");
    void updateCurrentlyPlaying() __asm__("cFEStateRequestLine_updateCurrentlyPlaying");
    void updatePlayListCounter() __asm__("cFEStateRequestLine_updatePlayListCounter");
    void updateHelpText(int idx) __asm__("cFEStateRequestLine_updateHelpText");
    void updateButtonsText(int idx) __asm__("cFEStateRequestLine_updateButtonsText");
    void updateCash() __asm__("cFEStateRequestLine_updateCash");
    void updateFreeSongMarkers() __asm__("cFEStateRequestLine_updateFreeSongMarkers");
    void updateHilightedSongInfo(int idx) __asm__("cFEStateRequestLine_updateHilightedSongInfo");
    void buySong(int idx) __asm__("cFEStateRequestLine_buySong");
    void showBuyPopup(int idx) __asm__("cFEStateRequestLine_showBuyPopup");
    void* getSongTitle(int i) __asm__("cFEStateRequestLine_getSongTitle");
    void* getSongArtist(int i) __asm__("cFEStateRequestLine_getSongArtist");
    // retail's destructor cFEStateRequestLine__cFEStateRequestLine stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRequestLine();
    virtual void onCreateScreen() __asm__("cFEStateRequestLine_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cFEStateRequestLine_onDestroyScreen");
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateRequestLine_onGainTransition");
    virtual void onPopupEvent(cUIState* item, unsigned int msg) __asm__("cFEStateRequestLine_onPopupEvent");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateRequestLine_onGainFocus");
};

CHECK_OFFSET(cFEStateRequestLine, mCharID, 0x048);
CHECK_OFFSET(cFEStateRequestLine, mSongOrder, 0x04C);
CHECK_OFFSET(cFEStateRequestLine, mNumSongs, 0x14C);
CHECK_OFFSET(cFEStateRequestLine, mFirstVisibleSong, 0x150);
CHECK_OFFSET(cFEStateRequestLine, mNumSongsInPlayList, 0x154);
CHECK_OFFSET(cFEStateRequestLine, mPlaylist, 0x158);
CHECK_OFFSET(cFEStateRequestLine, mOwnedSong, 0x160);
CHECK_OFFSET(cFEStateRequestLine, mMoney, 0x168);
CHECK_OFFSET(cFEStateRequestLine, mInStore, 0x16C);
CHECK_OFFSET(cFEStateRequestLine, mpRadioMenu, 0x170);
CHECK_OFFSET(cFEStateRequestLine, mpSongTitles, 0x174);
CHECK_OFFSET(cFEStateRequestLine, mpCheckMarks, 0x194);
CHECK_OFFSET(cFEStateRequestLine, mpCurSongTitle, 0x1B4);
CHECK_OFFSET(cFEStateRequestLine, mpCurSongArtist, 0x1B8);
CHECK_OFFSET(cFEStateRequestLine, mpFreeSongMarker, 0x1BC);
CHECK_OFFSET(cFEStateRequestLine, mpArtist, 0x1D4);
CHECK_OFFSET(cFEStateRequestLine, mpCost, 0x1D8);
CHECK_OFFSET(cFEStateRequestLine, mpCostText, 0x1DC);
CHECK_OFFSET(cFEStateRequestLine, mpCash, 0x1E0);
CHECK_OFFSET(cFEStateRequestLine, mpNumSongs, 0x1E4);
CHECK_OFFSET(cFEStateRequestLine, mpHelpText, 0x1E8);
CHECK_OFFSET(cFEStateRequestLine, mpArrowUp, 0x1EC);
CHECK_OFFSET(cFEStateRequestLine, mpArrowDown, 0x1F0);
CHECK_OFFSET(cFEStateRequestLine, mpSelectButtonText, 0x1F4);
CHECK_OFFSET(cFEStateRequestLine, mpSelectButtonIcon, 0x1F8);
CHECK_OFFSET(cFEStateRequestLine, mpPreviewButtonText, 0x1FC);
CHECK_OFFSET(cFEStateRequestLine, mpPreviewButtonIcon, 0x200);
CHECK_OFFSET(cFEStateRequestLine, mpCheckBox, 0x204);
CHECK_OFFSET(cFEStateRequestLine, mpEmptyBox, 0x208);
CHECK_OFFSET(cFEStateRequestLine, mpDollarSign, 0x20C);
CHECK_SIZE(cFEStateRequestLine, 0x210);

#endif /* FE_FESTATEBIGRADIO_H */
