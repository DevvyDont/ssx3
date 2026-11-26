#ifndef FE_FESTATERULESELECT_H
#define FE_FESTATERULESELECT_H

// cFEStateRuleSelect: the rule select screen (a cUIState); cFEReal creates it with cMemMan_alloc(156, "cFEStateRuleSelect").
// Size 0x9C [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h" // tFEStateNetMsgList (cFEStateBraggingRights's cFEStateNet base)

struct cListNode;
struct cUIEngine;
struct cUIPair;
struct cUIScreen;
struct cUIState;

struct cFEStateRuleSelect : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateRuleSelect
    char* mpGameEventMapping;        // 0x048 [O]
    int mCurrentMenuEnableState;     // 0x04C [O]
    char mcArraySize;                // 0x050 [O]
    char mcRuleConflictRefTable[15]; // 0x051 [O]
    cUIPair* mpMenuItemTable[15];    // 0x060 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRuleSelect1() __asm__("cFEStateRuleSelect_cFEStateRuleSelect1");
    void setupMenu(signed char first) __asm__("cFEStateRuleSelect_setupMenu");
    void updateBE(int idx, int ruleValue) __asm__("cFEStateRuleSelect_updateBE");
    int getBEData(int rule) __asm__("cFEStateRuleSelect_getBEData");
    void checkConflictForMenu(cUIListBox* item) __asm__("cFEStateRuleSelect_checkConflictForMenu");
    void updateMenuColor() __asm__("cFEStateRuleSelect_updateMenuColor");
    // retail's destructor cFEStateRuleSelect__cFEStateRuleSelect stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRuleSelect();
    virtual void onWidgetEvent(cUIThing* msg, unsigned int type) __asm__("cFEStateRuleSelect_onWidgetEvent");
};

CHECK_OFFSET(cFEStateRuleSelect, mpGameEventMapping, 0x048);
CHECK_OFFSET(cFEStateRuleSelect, mCurrentMenuEnableState, 0x04C);
CHECK_OFFSET(cFEStateRuleSelect, mcArraySize, 0x050);
CHECK_OFFSET(cFEStateRuleSelect, mcRuleConflictRefTable, 0x051);
CHECK_OFFSET(cFEStateRuleSelect, mpMenuItemTable, 0x060);
CHECK_SIZE(cFEStateRuleSelect, 0x9C);

// ---------------------------------------------------------------------------------------------------------------------
// cBraggingRightsMenu: one player's column of the bragging-rights screen (a cUIMenu, its group and up to eight list
// boxes, one per rule). Embedded twice in cFEStateBraggingRights.
// Size 0x34: [R] cFEStateBraggingRights_cFEStateBraggingRights constructs mMenuCtrl[2] with a 0x34 stride (0x6D0, 0x704;
// cFEStateBraggingRights's next member is at 0x738); [O] On Tour's class is 0x34 too. It has virtuals (EA's
// ~cBraggingRightsMenu): g++ 2.95 puts the vptr after the data, at 0x30.
// [R] cBraggingRightsMenu_cBraggingRightsMenu (fe/festateruleselect 0x001926F0) stores its vtable cBraggingRightsMenu_vtbl (1 slot:
// the destructor cBraggingRightsMenu__cBraggingRightsMenu, fe/fepopupmisc) at 0x30 and zeroes 0x0, 0x4, the eight list boxes, 0x28 and 0x2C.

struct cUIMenu;
struct cUIGroup;
struct cUIListBox;

struct cBraggingRightsMenu {
    cUIMenu* mpPlayerMenu;          // 0x000 [O]; [R] 0; [U] setMenuItemColor reads it
    cUIGroup* mpGroup;              // 0x004 [O]; [R] 0; [U] cFEStateBraggingRights_onWidgetCreate stores the player's
                                    //       group
    cUIListBox* mpListBox[8];       // 0x008 [O]; [R] all 0 (loop from 0x24 down); [U] setConfirmed / setMenuItemColor /
                                    //       setEnableItem walk them, findListBoxP searches them, onWidgetCreate stores them
    int mbConfirmed;                // 0x028 [O] (bool); [R] 0; [U] setConfirmed stores it; cFEStateBraggingRights's
                                    //       onGainFocus / onWidgetEvent / onWidgetMenu / updateConfirmedState test it
    int mcItemEnableFlag;           // 0x02C [O]; [R] 0; [U] setEnableFlag stores it, isItemEnable / setEnableItem test its
                                    //       bits
    void** mVtbl;                   // 0x030 [R] cBraggingRightsMenu_vtbl (ctor); the dtor cBraggingRightsMenu__cBraggingRightsMenu stores it again

    // Methods bound to their retail symbols by asm labels.
    void setConfirmed(int enabled) __asm__("cBraggingRightsMenu_setConfirmed");
    void setMenuItemColor() __asm__("cBraggingRightsMenu_setMenuItemColor");
    int findListBoxP(cUIListBox* val) __asm__("cBraggingRightsMenu_findListBoxP");
    bool isItemEnable(int itemIndex) __asm__("cBraggingRightsMenu_isItemEnable");
    void setEnableFlag(int val) __asm__("cBraggingRightsMenu_setEnableFlag__FPvi");

    // findListBoxP's "not one of mine" result (names ours): [U] cBraggingRightsMenu_findListBoxP returns it when no
    // mpListBox entry matches.
    enum { kNotFound = -1 };
};

CHECK_OFFSET(cBraggingRightsMenu, mpPlayerMenu, 0x000);
CHECK_OFFSET(cBraggingRightsMenu, mpGroup, 0x004);
CHECK_OFFSET(cBraggingRightsMenu, mpListBox, 0x008);
CHECK_OFFSET(cBraggingRightsMenu, mbConfirmed, 0x028);
CHECK_OFFSET(cBraggingRightsMenu, mcItemEnableFlag, 0x02C);
CHECK_OFFSET(cBraggingRightsMenu, mVtbl, 0x030);
CHECK_SIZE(cBraggingRightsMenu, 0x34);

// EA's eBRAGRIGHTs: one bragging-rights rule (cFEStateBraggingRights::updateBE's `which`, the rule ids of
// cFEStateBraggingRights::mpGameEventMapping). [O] On Tour's enum eBRAGRIGHTs, names and values; [U] retail fits:
// updateBE / cFEStateBraggingRights_getBEData write / read the other player's HeadStartSecs (0), StartFullBoost (1),
// FastRecovery (2) and StartingPoints (7) (tBEBraggingRights, be/beintoption.h) and the player's own NoUberTricks (3),
// LooseAdrenalin (4), Level1Character (5) and HalfPoints (6); onCreateScreen fills rule 0 with Off / 5s .. 20s,
// rule 7 with Off / 3000 / 5000 and the others with No / Yes.
enum eBRAGRIGHTs {
    kBRAGRIGHTS_GIVEHEADSTART = 0, kBRAGRIGHTS_GIVEFULLBOOST = 1, kBRAGRIGHTS_GIVEFASTRECOVERY = 2,
    kBRAGRIGHTS_TAKENOUBER = 3, kBRAGRIGHTS_TAKELEAKYAD = 4, kBRAGRIGHTS_TAKELV1CHAR = 5,
    kBRAGRIGHTS_TAKEHALFPOINTS = 6, kBRAGRIGHTS_STARTINHOLE = 7, kBRAGRIGHTS_NUM = 8
};

// ---------------------------------------------------------------------------------------------------------------------
// cFEStateBraggingRights: the online "bragging rights" screen (state id 20): two player columns of rule list boxes that
// both players confirm before an online race; the rule rows come from one of six game-event mapping tables.
// EA's source is fe/festateruleselect.cpp (+ fe/festateruleselect.h): builds/x/v2 file_hints for retail
// cFEStateBraggingRights_onCreateScreen (lines 887 ... 934, header line 182). Retail code: the ctor at the end of
// fe/festateruleselect (0x00192D00), the methods in fe/festatebigradio (0x00192D80 ... 0x001941B0) and the out-of-line
// inlines in fe/fepopupmisc (0x001DF470 ... 0x001DF578).
//
// Size 0x73C:
//   [R] cMemMan_alloc(1852, "cFEStateBraggingRights") + cFEStateBraggingRights_cFEStateBraggingRights(mem, engine) in
//       cFECustom_getNextState (fe/fereal) and fe/feasyncfile; the ctor writes mMenuCtrl at 0x6D0 / 0x704 and 0x738.
//   [O] On Tour 0xB4 = a 0x48 cFEState base + mMenuCtrl[2] (0x68) + mpGameEventMapping. Retail's base is cFEStateNet
//       (0x6D0; On Tour has no cFEStateNet), so the same two members sit at 0x6D0 and 0x738.
// Base: cFEStateNet (fe/festatenet.h, 0x6D0) : cUIState : cListNode (vptr at 0x8); see that header for the inherited
// fields' evidence. The ctor runs cFEStateNet_cFEStateNet(this, pEngine, 0), stores cFEStateBraggingRights_vtbl
// (0x0046AF68, 32 slots: 1 dtor, 4 onCreateScreen, 6 onGainFocus, 7 onGainTransition, 12 onUpdate, 13 onMenuGainFocus,
// 15 onRender2D, 17 onPopupEvent, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu, 24 handleEvent, 25
// func_00193C00, 31 func_001941B0; the rest cFEStateNet's / cUIState's), constructs both menus and stores mHashName 20.
// Evidence tags: [R] retail's constructor / allocation size; [U] how retail code uses the field (C bodies in
// src/fe/festatebigradio.cpp, src/fe/festateruleselect.cpp, src/fe/fepopupmisc.cpp, and the asm); [O] SSX On Tour's
// DWARF (class cFEStateBraggingRights, cBraggingRightsMenu): names, types and order.

struct cFEStateBraggingRights : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] _vt$11cFEStateNet cFEStateNet_vtbl, stored by cFEStateNet_cFEStateNet/cFEStateNet_cFEStateNet1; subclasses
    //       store theirs after calling it
    // cFEStateBraggingRights's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateBraggingRights
    cBraggingRightsMenu mMenuCtrl[2]; // 0x6D0 [O]; [R] cBraggingRightsMenu_cBraggingRightsMenu x2 (0x6D0, 0x704); [U]
                                    //       onWidgetCreate stores each column's group and list boxes; onGainFocus /
                                    //       onWidgetEvent / onWidgetMenu / updateConfirmedState test mbConfirmed
                                    //       (0x6F8 / 0x72C); onCreateScreen / func_001941B0 index the list boxes
    char* mpGameEventMapping;       // 0x738 [O]; [R] 0; [U] onCreateScreen picks one of six rule-id byte tables
                                    //       (D_004A1710 ... D_004A1738) by game mode; onCreateScreen / onWidgetEvent /
                                    //       func_001941B0 read rule ids from it (lb; EE char is signed)

    // A row without a rule in mpGameEventMapping (names ours): [U] onWidgetEvent and func_001941B0 skip
    // updateBE for it.
    enum { kNoRule = -1 };
    // The miMiscData of the "cancel challenge" popup (names ours): [U] onWidgetEvent tags the popup it opens with
    // kT_POPUPCMNCancelChallenge with it; onPopupEvent hands that popup's events to func_00194440.
    enum { kCancelChallengePopup = 2104 };

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateBraggingRights1() __asm__("cFEStateBraggingRights_cFEStateBraggingRights1");
    void updateBE(int player, int which, int value) __asm__("cFEStateBraggingRights_updateBE");
    void updateConfirmedState() __asm__("cFEStateBraggingRights_updateConfirmedState");
    // retail's destructor cFEStateBraggingRights__cFEStateBraggingRights stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateBraggingRights();
    virtual void onCreateScreen() __asm__("cFEStateBraggingRights_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateBraggingRights_onUpdate");
};

CHECK_OFFSET(cFEStateBraggingRights, mMenuCtrl, 0x6D0);
CHECK_OFFSET(cFEStateBraggingRights, mpGameEventMapping, 0x738);
CHECK_SIZE(cFEStateBraggingRights, 0x73C);

#endif /* FE_FESTATERULESELECT_H */
