#ifndef FE_BUYPOPUPINFO_H
#define FE_BUYPOPUPINFO_H

// cBuyPopupInfo: what a buy popup (cUIStateBuyPopup, fe/uistatebuypopup.h) shows: the price, the player's cash or
// remaining credits, and three strings (the item's name, its type and the question). cUIStateBuyPopup embeds one at
// 0x48 (mInfo; getPopupInfo func_001DDCB8 returns this + 0x48); the shop states fill it through the init* methods.
//
// Size 0x24: SSX On Tour's DWARF (class cBuyPopupInfo, size 0x24: mCost, mMoney, mCreditsLeft and three
// tPopupElements, no virtuals), and cUIStateBuyPopup's next member (mAccept) sits at 0x6C = 0x48 + 0x24. EA's source:
// all_in_one_fe.cpp in On Tour; retail keeps the methods in fe/fepopup (0x1CA8A0-0x1CAB10).
// Methods: cBuyPopupInfo_clear clear() (On Tour's first method; the constructor is inlined into cUIStateBuyPopup's),
// cBuyPopupInfo_initBuySongByCredit (songName, creditsLeft), cBuyPopupInfo_initBuySong (songName, cost, money),
// func_001CAA18 initBuyReward (rewardType, rewardName, cost, money), cBuyPopupInfo_initBuyBolt (boltName, cost,
// money). The setters (setCost, setCash, setItemName ...) are inlined at every call site.
//
// A FLAT layout. Member names, order and types are EA's (On Tour), every offset checked against retail. Evidence tags:
//   [O] On Tour's DWARF;
//   [R] retail clear (cBuyPopupInfo_clear): MEM_fill(0xC/0x14/0x1C, 0, 8), 0 to 0x0/0x4/0x8;
//   [U] how retail code uses the field (the init* methods, cUIStateBuyPopup_onWidgetCreate).

#include "layout.h"
#include "fe/popupinfo.h"

struct cBuyPopupInfo {
    int mCost;                      // 0x00 [O]; [R] 0; [U] initBuySong / initBuyBolt / initBuyReward store the price;
                                    //      onWidgetCreate shows it as dollars ("cost_answer") when mCreditsLeft <= 0
    int mMoney;                     // 0x04 [O]; [R] 0; [U] the player's cash ("you have_answer")
    int mCreditsLeft;               // 0x08 [O]; [R] 0; [U] initBuySongByCredit stores it, the cash variants -1;
                                    //      onWidgetCreate shows "credits remain" and the credit / cash groups by it
    tPopupElement mItemName;        // 0x0C [O]; [R] memset 8; [U] eType 2 (ascii) + the song / bolt / reward name;
                                    //      shown in widget "item name"
    tPopupElement mItemType;        // 0x14 [O]; [R] memset 8; [U] eType 3 + a text id ("kT_OVRSong", "kT_CMNItemColon",
                                    //      "kT_OVRCMNPosterColon" ...); widget "item type"
    tPopupElement mQuestion;        // 0x1C [O]; [R] memset 8; [U] eType 3 + "kT_16BuyMusicTrack" /
                                    //      "kT_OVRCMNBuySongCredit" / "kT_OVRCMNBuyItem"; widget "question"
};

CHECK_OFFSET(cBuyPopupInfo, mCost, 0x00);
CHECK_OFFSET(cBuyPopupInfo, mMoney, 0x04);
CHECK_OFFSET(cBuyPopupInfo, mCreditsLeft, 0x08);
CHECK_OFFSET(cBuyPopupInfo, mItemName, 0x0C);
CHECK_OFFSET(cBuyPopupInfo, mItemType, 0x14);
CHECK_OFFSET(cBuyPopupInfo, mQuestion, 0x1C);
CHECK_SIZE(cBuyPopupInfo, 0x24);

#endif /* FE_BUYPOPUPINFO_H */
