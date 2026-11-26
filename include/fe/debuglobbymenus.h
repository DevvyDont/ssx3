#ifndef FE_DEBUGLOBBYMENUS_H
#define FE_DEBUGLOBBYMENUS_H

// The debug online lobby's menus: the cMenu subclasses (util/menu.h) that cDebugLobbyScreen (fe/debugfe.h) embeds from
// +0x070 on and drives through its cMenuSys (login, persona, room list, room occupants, chat, challenge and abort).
// Retail keeps no names for them; the July 2003 preview build does, twice per class:
//   - its vtable sections `.gnu.linkonce.d._vt$t...` name each class (most are templates over cDebugLobbyScreen's
//     functor types), and every one pairs with a retail vtable (builds/x/v2/vtables_v2.tsv, same 9 cMenu slots);
//   - its `.gnu.linkonce.t` constructors (e.g. ChallengeResponseMenu<...>::ChallengeResponseMenu(const &, const &))
//     pair with the retail constructors (builds/x/v2/name_map_v2.tsv, the bodies match).
// The template instantiations are written below as one flat struct per class layout: both AccountDetailsMenu
// instantiations (cDebugLobbyScreen::OnCreate at vtable AccountDetailsMenu_vtbl, ::OnLogin at AccountDetailsMenu1_vtbl) share one struct.
// A functor argument (cDebugLobbyScreen::OnX, one word: the screen) is copied into the menu, and the menu's onCommand
// (cMenu vtable slot 3) passes that word to the matching cDebugLobbyScreen callback (func_0017E030 ... func_0017E558).
// Sizes: each is the end of its last row (the embedding strides of util/menu.h) and equals the char[] member size
// cDebugLobbyScreen gives it (mLoginMenu 0x594, mContinueMenu 0x148, mPersonaMenu 0x5E0, mRoomMenu 0x318,
// mOccupantsMenu 0x4F0, mChatMenu 0x38C, mChallengedMenu 0x1EC, mAbortMenu 0x158); the destructors in
// main/all_in_one_main destroy exactly these rows. No allocation: they are all members. On Tour has none of them.
// EA's source file is not known (no assert string); the constructors sit in fe/festatenethelppopup and fe/debugfe,
// the destructors and onCommand slots in main/all_in_one_main.
//
// A FLAT layout, like main/game.h: base cMenu written out inline (its vptr at 0x12C), rows embedded by value. No row
// or functor has a name source; the own members' names are ours: each row after its caption (the constructor's
// string), each functor word after the screen callback it is passed to (mScreen, or mAcceptScreen / mDeclineScreen
// where a menu has two).
// Evidence tags: [R] retail constructors / destructors; [U] retail usage; [O] On Tour (cMenu's own members only).

#include "layout.h"
#include "util/menu.h"

struct cMenuSys;
struct cDebugLobbyScreen;


// ContinueMenu (0x148): a single "Continue" row. Preview _vt$12ContinueMenu = retail ContinueMenu_vtbl; ctor
// ContinueMenu_ContinueMenu (fe/debugfe), dtor ContinueMenu__ContinueMenu (slot 1). cDebugLobbyScreen +0x604.

struct ContinueMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] ContinueMenu_ContinueMenu stores ContinueMenu_vtbl
    cPopMenuItem mContinueItem;      // 0x130 [R] ctor cPopMenuItem_cPopMenuItem(this + 0x130, "Continue");
                                     //       ContinueMenu__ContinueMenu destroys it
};

CHECK_OFFSET(ContinueMenu, NumItems, 0x000);
CHECK_OFFSET(ContinueMenu, FocusItem, 0x004);
CHECK_OFFSET(ContinueMenu, TopItem, 0x008);
CHECK_OFFSET(ContinueMenu, Items, 0x00C);
CHECK_OFFSET(ContinueMenu, MenuSys, 0x124);
CHECK_OFFSET(ContinueMenu, DrawLayer, 0x128);
CHECK_OFFSET(ContinueMenu, mVtbl, 0x12C);
CHECK_OFFSET(ContinueMenu, mContinueItem, 0x130);
CHECK_SIZE(ContinueMenu, 0x148);

// AccountDetailsMenu<cDebugLobbyScreen::OnCreate> / <cDebugLobbyScreen::OnLogin> (0x1FC): title, spacer, login name
// and password entry rows, the action row and "Cancel". Preview vtables pair with retail AccountDetailsMenu_vtbl (OnCreate; ctor
// AccountDetailsMenu_AccountDetailsMenu, onCommand AccountDetailsMenu_onCommand, dtor AccountDetailsMenu__AccountDetailsMenu) and AccountDetailsMenu1_vtbl (OnLogin; ctor AccountDetailsMenu_AccountDetailsMenu1, onCommand
// AccountDetailsMenu_onCommand1, dtor AccountDetailsMenu__AccountDetailsMenu1). AccountMenu embeds one of each at 0x130 / 0x32C.

struct AccountDetailsMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] ctors store AccountDetailsMenu_vtbl (OnCreate) /
                                     //       AccountDetailsMenu1_vtbl (OnLogin)
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnCreate / OnLogin: the owning screen); [U]
                                     //       onCommand passes it to func_0017E030 / func_0017E118
    cNullMenuItem mTitleItem;        // 0x134 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x134, title ("Create Account"
                                     //       / "Login")); destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x150 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x150, h); destroyed by the
                                     //       dtor
    cTextMenuItem mLoginNameItem;    // 0x168 [R] ctor cTextMenuItem_cTextMenuItem(this + 0x168, "Login Name");
                                     //       destroyed by the dtor [U] onCommand passes its Text (this + 0x180)
    cTextMenuItem mPasswordItem;     // 0x194 [R] ctor cTextMenuItem_cTextMenuItem(this + 0x194, "Password"); destroyed
                                     //       by the dtor [U] onCommand passes its Text (this + 0x1AC)
    cCommandMenuItem mActionItem;    // 0x1C0 [R] ctor cCommandMenuItem_cCommandMenuItem2 (the action row); destroyed by
                                     //       the dtor
    cPopMenuItem mCancelItem;        // 0x1E4 [R] ctor cPopMenuItem_cPopMenuItem(this + 0x1E4, "Cancel"); destroyed by
                                     //       the dtor

    // Methods bound to their retail symbols by asm labels.
    void* AccountDetailsMenu1(void* vinfo, void* actionLabel) __asm__("AccountDetailsMenu_AccountDetailsMenu1");
    int onCommand() __asm__("AccountDetailsMenu_onCommand");
    int onCommand1() __asm__("AccountDetailsMenu_onCommand1");
};

CHECK_OFFSET(AccountDetailsMenu, NumItems, 0x000);
CHECK_OFFSET(AccountDetailsMenu, FocusItem, 0x004);
CHECK_OFFSET(AccountDetailsMenu, TopItem, 0x008);
CHECK_OFFSET(AccountDetailsMenu, Items, 0x00C);
CHECK_OFFSET(AccountDetailsMenu, MenuSys, 0x124);
CHECK_OFFSET(AccountDetailsMenu, DrawLayer, 0x128);
CHECK_OFFSET(AccountDetailsMenu, mVtbl, 0x12C);
CHECK_OFFSET(AccountDetailsMenu, mScreen, 0x130);
CHECK_OFFSET(AccountDetailsMenu, mTitleItem, 0x134);
CHECK_OFFSET(AccountDetailsMenu, mSpaceItem, 0x150);
CHECK_OFFSET(AccountDetailsMenu, mLoginNameItem, 0x168);
CHECK_OFFSET(AccountDetailsMenu, mPasswordItem, 0x194);
CHECK_OFFSET(AccountDetailsMenu, mActionItem, 0x1C0);
CHECK_OFFSET(AccountDetailsMenu, mCancelItem, 0x1E4);
CHECK_SIZE(AccountDetailsMenu, 0x1FC);

// AccountMenu<cDebugLobbyScreen::OnCreate, cDebugLobbyScreen::OnLogin> (0x594): "Login Method" with two sub-menu rows
// that open the two account menus. Preview vtable pairs with retail AccountMenu_vtbl; ctor AccountMenu_AccountMenu, dtor
// AccountMenu__AccountMenu (it inlines both AccountDetailsMenu destructors). cDebugLobbyScreen +0x070 (mLoginMenu).

struct AccountMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] AccountMenu_AccountMenu stores AccountMenu_vtbl
    AccountDetailsMenu mCreateAccountMenu; // 0x130 [R] ctor AccountDetailsMenu_AccountDetailsMenu(this + 0x130,
                                     //       functor, "Create Account"); dtor destroys its rows
    AccountDetailsMenu mLoginMenu;   // 0x32C [R] ctor AccountDetailsMenu_AccountDetailsMenu1(this + 0x32C, functor,
                                     //       "Login"); dtor destroys its rows
    cNullMenuItem mTitleItem;        // 0x528 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x528, "Login Method");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x544 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x544, h); destroyed by the
                                     //       dtor
    cSubMenuItem mCreateAccountItem; // 0x55C [R] ctor cSubMenuItem_cSubMenuItem(this + 0x55C, "Create Account", this +
                                     //       0x130); destroyed by the dtor
    cSubMenuItem mLoginItem;         // 0x578 [R] ctor cSubMenuItem_cSubMenuItem(this + 0x578, "Login", this + 0x32C);
                                     //       destroyed by the dtor
};

CHECK_OFFSET(AccountMenu, NumItems, 0x000);
CHECK_OFFSET(AccountMenu, FocusItem, 0x004);
CHECK_OFFSET(AccountMenu, TopItem, 0x008);
CHECK_OFFSET(AccountMenu, Items, 0x00C);
CHECK_OFFSET(AccountMenu, MenuSys, 0x124);
CHECK_OFFSET(AccountMenu, DrawLayer, 0x128);
CHECK_OFFSET(AccountMenu, mVtbl, 0x12C);
CHECK_OFFSET(AccountMenu, mCreateAccountMenu, 0x130);
CHECK_OFFSET(AccountMenu, mLoginMenu, 0x32C);
CHECK_OFFSET(AccountMenu, mTitleItem, 0x528);
CHECK_OFFSET(AccountMenu, mSpaceItem, 0x544);
CHECK_OFFSET(AccountMenu, mCreateAccountItem, 0x55C);
CHECK_OFFSET(AccountMenu, mLoginItem, 0x578);
CHECK_SIZE(AccountMenu, 0x594);

// CreatePersonaMenu<cDebugLobbyScreen::OnCreatePersona> (0x1B8): "Create Persona", a name entry row and "Create".
// Preview vtable pairs with retail CreatePersonaMenu_vtbl; ctor CreatePersonaMenu_CreatePersonaMenu, onCommand CreatePersonaMenu_onCommand, dtor CreatePersonaMenu__CreatePersonaMenu.

struct CreatePersonaMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] CreatePersonaMenu_CreatePersonaMenu stores CreatePersonaMenu_vtbl
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnCreatePersona: the owning screen); [U]
                                     //       CreatePersonaMenu_onCommand passes it to func_0017E1C0
    cNullMenuItem mTitleItem;        // 0x134 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x134, "Create Persona");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x150 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x150, h); destroyed by the
                                     //       dtor
    cTextMenuItem mPersonaNameItem;  // 0x168 [R] ctor cTextMenuItem_cTextMenuItem(this + 0x168, "Persona Name");
                                     //       destroyed by the dtor [U] CreatePersonaMenu_onCommand passes its Text
                                     //       (this + 0x180)
    cCommandMenuItem mCreateItem;    // 0x194 [R] ctor cCommandMenuItem_cCommandMenuItem2 ("Create"); destroyed by the
                                     //       dtor

    // Methods bound to their retail symbols by asm labels.
    int onCommand() __asm__("CreatePersonaMenu_onCommand");
};

CHECK_OFFSET(CreatePersonaMenu, NumItems, 0x000);
CHECK_OFFSET(CreatePersonaMenu, FocusItem, 0x004);
CHECK_OFFSET(CreatePersonaMenu, TopItem, 0x008);
CHECK_OFFSET(CreatePersonaMenu, Items, 0x00C);
CHECK_OFFSET(CreatePersonaMenu, MenuSys, 0x124);
CHECK_OFFSET(CreatePersonaMenu, DrawLayer, 0x128);
CHECK_OFFSET(CreatePersonaMenu, mVtbl, 0x12C);
CHECK_OFFSET(CreatePersonaMenu, mScreen, 0x130);
CHECK_OFFSET(CreatePersonaMenu, mTitleItem, 0x134);
CHECK_OFFSET(CreatePersonaMenu, mSpaceItem, 0x150);
CHECK_OFFSET(CreatePersonaMenu, mPersonaNameItem, 0x168);
CHECK_OFFSET(CreatePersonaMenu, mCreateItem, 0x194);
CHECK_SIZE(CreatePersonaMenu, 0x1B8);

// DeletePersonaMenu<cDebugLobbyScreen::OnDeletePersona> (0x1F8): "Delete Persona" and four persona rows. Preview vtable
// pairs with retail DeletePersonaMenu_vtbl; ctor DeletePersonaMenu_DeletePersonaMenu, onCommand DeletePersonaMenu_onCommand, dtor DeletePersonaMenu__DeletePersonaMenu, row fill
// DeletePersonaMenu_setpersonas.

struct DeletePersonaMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] DeletePersonaMenu_DeletePersonaMenu stores DeletePersonaMenu_vtbl
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnDeletePersona: the owning screen); [U]
                                     //       DeletePersonaMenu_onCommand passes it to func_0017E228
    cNullMenuItem mTitleItem;        // 0x134 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x134, "Delete Persona");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x150 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x150, h); destroyed by the
                                     //       dtor
    cCommandMenuItem mPersonaItems[4]; // 0x168 [R] ctor cCommandMenuItem_cCommandMenuItem2 (4 persona rows); destroyed
                                     //       by the dtor (loop of 4) [U] DeletePersonaMenu_setpersonas sets their
                                     //       captions and IDs

    // Methods bound to their retail symbols by asm labels.
    int onCommand(cMenuItem* item) __asm__("DeletePersonaMenu_onCommand");
};

CHECK_OFFSET(DeletePersonaMenu, NumItems, 0x000);
CHECK_OFFSET(DeletePersonaMenu, FocusItem, 0x004);
CHECK_OFFSET(DeletePersonaMenu, TopItem, 0x008);
CHECK_OFFSET(DeletePersonaMenu, Items, 0x00C);
CHECK_OFFSET(DeletePersonaMenu, MenuSys, 0x124);
CHECK_OFFSET(DeletePersonaMenu, DrawLayer, 0x128);
CHECK_OFFSET(DeletePersonaMenu, mVtbl, 0x12C);
CHECK_OFFSET(DeletePersonaMenu, mScreen, 0x130);
CHECK_OFFSET(DeletePersonaMenu, mTitleItem, 0x134);
CHECK_OFFSET(DeletePersonaMenu, mSpaceItem, 0x150);
CHECK_OFFSET(DeletePersonaMenu, mPersonaItems, 0x168);
CHECK_SIZE(DeletePersonaMenu, 0x1F8);

// PersonaMenu<cDebugLobbyScreen::OnSelectPersona, ::OnCreatePersona, ::OnDeletePersona> (0x5E0): "Select Persona",
// four persona rows and sub-menu rows for the create / delete menus. Preview vtable pairs with retail PersonaMenu_vtbl; ctor
// PersonaMenu_PersonaMenu, onCommand PersonaMenu_onCommand, dtor PersonaMenu__PersonaMenu, row fill PersonaMenu_setpersonas. cDebugLobbyScreen +0x74C.

struct PersonaMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] PersonaMenu_PersonaMenu stores PersonaMenu_vtbl
    CreatePersonaMenu mCreatePersonaMenu; // 0x130 [R] ctor CreatePersonaMenu_CreatePersonaMenu(this + 0x130, functor);
                                     //       dtor
    DeletePersonaMenu mDeletePersonaMenu; // 0x2E8 [R] ctor DeletePersonaMenu_DeletePersonaMenu(this + 0x2E8, functor);
                                     //       dtor; [U] PersonaMenu_setpersonas fills it (DeletePersonaMenu_setpersonas)
    cDebugLobbyScreen* mScreen;      // 0x4E0 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnSelectPersona: the owning screen); [U]
                                     //       PersonaMenu_onCommand passes it to func_0017E290
    cNullMenuItem mTitleItem;        // 0x4E4 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x4E4, "Select Persona");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x500 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x500, h); destroyed by the
                                     //       dtor
    cCommandMenuItem mPersonaItems[4]; // 0x518 [R] ctor cCommandMenuItem_cCommandMenuItem2 (4 persona rows); destroyed
                                     //       by the dtor [U] PersonaMenu_setpersonas sets their data and IDs (view
                                     //       Menu_B418 items[4])
    cSubMenuItem mCreatePersonaItem; // 0x5A8 [R] ctor cSubMenuItem_cSubMenuItem(this + 0x5A8, "Create Persona", this +
                                     //       0x130); destroyed by the dtor
    cSubMenuItem mDeletePersonaItem; // 0x5C4 [R] ctor cSubMenuItem_cSubMenuItem(this + 0x5C4, "Delete Persona", this +
                                     //       0x2E8); destroyed by the dtor
};

CHECK_OFFSET(PersonaMenu, NumItems, 0x000);
CHECK_OFFSET(PersonaMenu, FocusItem, 0x004);
CHECK_OFFSET(PersonaMenu, TopItem, 0x008);
CHECK_OFFSET(PersonaMenu, Items, 0x00C);
CHECK_OFFSET(PersonaMenu, MenuSys, 0x124);
CHECK_OFFSET(PersonaMenu, DrawLayer, 0x128);
CHECK_OFFSET(PersonaMenu, mVtbl, 0x12C);
CHECK_OFFSET(PersonaMenu, mCreatePersonaMenu, 0x130);
CHECK_OFFSET(PersonaMenu, mDeletePersonaMenu, 0x2E8);
CHECK_OFFSET(PersonaMenu, mScreen, 0x4E0);
CHECK_OFFSET(PersonaMenu, mTitleItem, 0x4E4);
CHECK_OFFSET(PersonaMenu, mSpaceItem, 0x500);
CHECK_OFFSET(PersonaMenu, mPersonaItems, 0x518);
CHECK_OFFSET(PersonaMenu, mCreatePersonaItem, 0x5A8);
CHECK_OFFSET(PersonaMenu, mDeletePersonaItem, 0x5C4);
CHECK_SIZE(PersonaMenu, 0x5E0);

// LobbyMenu<cDebugLobbyScreen::OnSelectRoom> (0x318): "Select Room" and twelve room rows. Preview vtable pairs with
// retail LobbyMenu_vtbl; ctor LobbyMenu_LobbyMenu, onCommand LobbyMenu_onCommand, dtor LobbyMenu__LobbyMenu, row fill LobbyMenu_updaterooms.
// cDebugLobbyScreen +0xD2C.

struct LobbyMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] LobbyMenu_LobbyMenu stores LobbyMenu_vtbl
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnSelectRoom: the owning screen); [U]
                                     //       LobbyMenu_onCommand passes it to func_0017E2F8
    cNullMenuItem mTitleItem;        // 0x134 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x134, "Select Room");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x150 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x150, h); destroyed by the
                                     //       dtor
    cCommandMenuItem mRoomItems[12]; // 0x168 [R] ctor cCommandMenuItem_cCommandMenuItem2 (12 room rows); destroyed by
                                     //       the dtor (loop of 12) [U] LobbyMenu_updaterooms sets their data (+0x14
                                     //       Caption) and ID (view Menu_B528 items[12])
};

CHECK_OFFSET(LobbyMenu, NumItems, 0x000);
CHECK_OFFSET(LobbyMenu, FocusItem, 0x004);
CHECK_OFFSET(LobbyMenu, TopItem, 0x008);
CHECK_OFFSET(LobbyMenu, Items, 0x00C);
CHECK_OFFSET(LobbyMenu, MenuSys, 0x124);
CHECK_OFFSET(LobbyMenu, DrawLayer, 0x128);
CHECK_OFFSET(LobbyMenu, mVtbl, 0x12C);
CHECK_OFFSET(LobbyMenu, mScreen, 0x130);
CHECK_OFFSET(LobbyMenu, mTitleItem, 0x134);
CHECK_OFFSET(LobbyMenu, mSpaceItem, 0x150);
CHECK_OFFSET(LobbyMenu, mRoomItems, 0x168);
CHECK_SIZE(LobbyMenu, 0x318);

// RoomMenu<cDebugLobbyScreen::OnSelectUser, cDebugLobbyScreen::OnChat> (0x4F0): "Room Occupants", 24 user rows and
// "Chat". Preview vtable pairs with retail RoomMenu_vtbl; ctor RoomMenu_RoomMenu, onCommand RoomMenu_onCommand, dtor
// RoomMenu__RoomMenu, row fill RoomMenu_updateusers. cDebugLobbyScreen +0x1044 (mOccupantsMenu).

struct LobbyMan;

struct RoomMenu {
    // Item IDs other than the occupant rows' 0.. (names ours): [U] RoomMenu_RoomMenu gives the "Chat" item kItem_Chat,
    // RoomMenu_updateusers gives the "No users in room" line kItem_NoUsers, and RoomMenu_onCommand opens the chat menu
    // for kItem_Chat and ignores kItem_NoUsers.
    enum { kItem_Chat = -2, kItem_NoUsers = -1 };
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] RoomMenu_RoomMenu stores RoomMenu_vtbl
    cDebugLobbyScreen* mSelectUserScreen; // 0x130 [R] ctor stores the first word of its functor argument (preview
                                     //       template argument cDebugLobbyScreen::OnSelectUser: the owning screen); [U]
                                     //       RoomMenu_onCommand passes it to func_0017E368 (a user row)
    cDebugLobbyScreen* mChatScreen;  // 0x134 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnChat: the owning screen); [U]
                                     //       RoomMenu_onCommand passes it to func_0017E3B8 (ID -2, "Chat")
    cNullMenuItem mTitleItem;        // 0x138 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x138, "Room Occupants");
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x154 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x154, h); destroyed by the
                                     //       dtor
    cCommandMenuItem mUserItems[24]; // 0x16C [R] ctor cCommandMenuItem_cCommandMenuItem2 (24 user rows); destroyed by
                                     //       the dtor [U] RoomMenu_updateusers sets their data and ID (view Menu_B620
                                     //       items[24])
    cCommandMenuItem mChatItem;      // 0x4CC [R] ctor cCommandMenuItem_cCommandMenuItem2 ("Chat"); destroyed by the
                                     //       dtor

    // Methods bound to their retail symbols by asm labels.
    void updateusers(LobbyMan* src) __asm__("RoomMenu_updateusers");
    int onCommand(cMenuItem* item) __asm__("RoomMenu_onCommand");
};

CHECK_OFFSET(RoomMenu, NumItems, 0x000);
CHECK_OFFSET(RoomMenu, FocusItem, 0x004);
CHECK_OFFSET(RoomMenu, TopItem, 0x008);
CHECK_OFFSET(RoomMenu, Items, 0x00C);
CHECK_OFFSET(RoomMenu, MenuSys, 0x124);
CHECK_OFFSET(RoomMenu, DrawLayer, 0x128);
CHECK_OFFSET(RoomMenu, mVtbl, 0x12C);
CHECK_OFFSET(RoomMenu, mSelectUserScreen, 0x130);
CHECK_OFFSET(RoomMenu, mChatScreen, 0x134);
CHECK_OFFSET(RoomMenu, mTitleItem, 0x138);
CHECK_OFFSET(RoomMenu, mSpaceItem, 0x154);
CHECK_OFFSET(RoomMenu, mUserItems, 0x16C);
CHECK_OFFSET(RoomMenu, mChatItem, 0x4CC);
CHECK_SIZE(RoomMenu, 0x4F0);

// ChatMenu<cDebugLobbyScreen::OnMessage> (0x38C): "Chat Menu", an "Enter Chat" entry row and 14 message rows.
// Preview vtable pairs with retail ChatMenu_vtbl; ctor ChatMenu_ChatMenu, onCommand ChatMenu_onCommand, dtor ChatMenu__ChatMenu.
// cDebugLobbyScreen +0x1534.

struct cCommandMenuItem;

struct ChatMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] ChatMenu_ChatMenu stores ChatMenu_vtbl
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnMessage: the owning screen); [U]
                                     //       ChatMenu_onCommand passes it to func_0017E418
    cNullMenuItem mTitleItem;        // 0x134 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x134, "Chat Menu"); destroyed
                                     //       by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x150 [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x150, h); destroyed by the
                                     //       dtor
    cTextMenuItem mEnterChatItem;    // 0x168 [R] ctor cTextMenuItem_cTextMenuItem(this + 0x168, "Enter Chat");
                                     //       destroyed by the dtor
    cCommandMenuItem mMessageItems[14]; // 0x194 [R] ctor cCommandMenuItem_cCommandMenuItem2 (14 message rows);
                                     //       destroyed by the dtor (loop of 14)

    // Methods bound to their retail symbols by asm labels.
    int onCommand(cCommandMenuItem* item) __asm__("ChatMenu_onCommand");
};

CHECK_OFFSET(ChatMenu, NumItems, 0x000);
CHECK_OFFSET(ChatMenu, FocusItem, 0x004);
CHECK_OFFSET(ChatMenu, TopItem, 0x008);
CHECK_OFFSET(ChatMenu, Items, 0x00C);
CHECK_OFFSET(ChatMenu, MenuSys, 0x124);
CHECK_OFFSET(ChatMenu, DrawLayer, 0x128);
CHECK_OFFSET(ChatMenu, mVtbl, 0x12C);
CHECK_OFFSET(ChatMenu, mScreen, 0x130);
CHECK_OFFSET(ChatMenu, mTitleItem, 0x134);
CHECK_OFFSET(ChatMenu, mSpaceItem, 0x150);
CHECK_OFFSET(ChatMenu, mEnterChatItem, 0x168);
CHECK_OFFSET(ChatMenu, mMessageItems, 0x194);
CHECK_SIZE(ChatMenu, 0x38C);

// ChallengeResponseMenu<cDebugLobbyScreen::OnAcceptChallenge, cDebugLobbyScreen::OnDeclineChallenge> (0x1EC):
// "You've been Challenged!", "by", the challenger line, a spacer and Accept / Decline. Preview vtable pairs with retail
// D_0046D3D0; ctor at 0x001DB148 (misnamed ChallengeResponseMenu_ChallengeResponseMenu in retail symbols), onCommand ChallengeResponseMenu_onCommand,
// dtor ChallengeResponseMenu__ChallengeResponseMenu. cDebugLobbyScreen +0x18C0.

struct cMenuItem;

struct ChallengeResponseMenu {
    // The command items' IDs (names ours): [U] ChallengeResponseMenu_ChallengeResponseMenu builds mAcceptItem with
    // kItem_Accept and mDeclineItem with kItem_Decline; onCommand answers or declines the challenge by them.
    enum { kItem_Accept = 0, kItem_Decline = 1 };
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] ctor (0x001DB148) stores D_0046D3D0
    cDebugLobbyScreen* mAcceptScreen; // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnAcceptChallenge: the owning screen); [U]
                                     //       ChallengeResponseMenu_onCommand ID 0 calls func_0017E490 with it
    cDebugLobbyScreen* mDeclineScreen; // 0x134 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnDeclineChallenge: the owning screen); [U]
                                     //       ChallengeResponseMenu_onCommand ID 1 calls func_0017E4F0 with it
    cNullMenuItem mTitleItem;        // 0x138 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x138, "You've been
                                     //       Challenged!"); destroyed by the dtor
    cNullMenuItem mByItem;           // 0x154 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x154, "by"); destroyed by the
                                     //       dtor
    cNullMenuItem mChallengerItem;   // 0x170 [R] ctor cNullMenuItem_cNullMenuItem(this + 0x170, "" (the challenger));
                                     //       destroyed by the dtor
    cSpaceMenuItem mSpaceItem;       // 0x18C [R] ctor cSpaceMenuItem_cSpaceMenuItem(this + 0x18C, h); destroyed by the
                                     //       dtor
    cCommandMenuItem mAcceptItem;    // 0x1A4 [R] ctor cCommandMenuItem_cCommandMenuItem2 (ID 0 "Accept"); destroyed by
                                     //       the dtor
    cCommandMenuItem mDeclineItem;   // 0x1C8 [R] ctor cCommandMenuItem_cCommandMenuItem2 (ID 1 "Decline"); destroyed by
                                     //       the dtor

    // Methods bound to their retail symbols by asm labels.
    int onCommand(cMenuItem* item) __asm__("ChallengeResponseMenu_onCommand");
};

CHECK_OFFSET(ChallengeResponseMenu, NumItems, 0x000);
CHECK_OFFSET(ChallengeResponseMenu, FocusItem, 0x004);
CHECK_OFFSET(ChallengeResponseMenu, TopItem, 0x008);
CHECK_OFFSET(ChallengeResponseMenu, Items, 0x00C);
CHECK_OFFSET(ChallengeResponseMenu, MenuSys, 0x124);
CHECK_OFFSET(ChallengeResponseMenu, DrawLayer, 0x128);
CHECK_OFFSET(ChallengeResponseMenu, mVtbl, 0x12C);
CHECK_OFFSET(ChallengeResponseMenu, mAcceptScreen, 0x130);
CHECK_OFFSET(ChallengeResponseMenu, mDeclineScreen, 0x134);
CHECK_OFFSET(ChallengeResponseMenu, mTitleItem, 0x138);
CHECK_OFFSET(ChallengeResponseMenu, mByItem, 0x154);
CHECK_OFFSET(ChallengeResponseMenu, mChallengerItem, 0x170);
CHECK_OFFSET(ChallengeResponseMenu, mSpaceItem, 0x18C);
CHECK_OFFSET(ChallengeResponseMenu, mAcceptItem, 0x1A4);
CHECK_OFFSET(ChallengeResponseMenu, mDeclineItem, 0x1C8);
CHECK_SIZE(ChallengeResponseMenu, 0x1EC);

// AbortMenu<cDebugLobbyScreen::OnAbortChallenge> (0x158): one "Abort" row. Preview vtable pairs with retail
// AbortMenu_vtbl; ctor AbortMenu_AbortMenu, onCommand AbortMenu_onCommand, dtor AbortMenu__AbortMenu. cDebugLobbyScreen +0x1AAC.

struct AbortMenu {
    // --- cMenu (util/menu.h, 0x130; no base class, vptr after its data)
    int NumItems;                    // 0x000 [O] cMenu::NumItems; [R] cMenu_cMenu zeroes it; [U] cMenu_addItem counts
                                     //       the rows
    int FocusItem;                   // 0x004 [O] cMenu::FocusItem; [R] cMenu_cMenu
    int TopItem;                     // 0x008 [O] cMenu::TopItem; [R] cMenu_cMenu
    cMenuItem* Items[70];            // 0x00C [O] cMenu::Items; [U] cMenu_addItem(this, item, -1) appends each row below
    cMenuSys* MenuSys;               // 0x124 [O] cMenu::MenuSys; [R] cMenu_cMenu zeroes it
    int DrawLayer;                   // 0x128 [O] cMenu::DrawLayer; no access seen
    void** mVtbl;                    // 0x12C [R] AbortMenu_AbortMenu stores AbortMenu_vtbl
    cDebugLobbyScreen* mScreen;      // 0x130 [R] ctor stores the first word of its functor argument (preview template
                                     //       argument cDebugLobbyScreen::OnAbortChallenge: the owning screen); [U]
                                     //       AbortMenu_onCommand passes it to func_0017E558
    cCommandMenuItem mAbortItem;     // 0x134 [R] ctor cCommandMenuItem_cCommandMenuItem2 (ID 0 "Abort"); destroyed by
                                     //       the dtor

    // Methods bound to their retail symbols by asm labels.
    int onCommand() __asm__("AbortMenu_onCommand");
};

CHECK_OFFSET(AbortMenu, NumItems, 0x000);
CHECK_OFFSET(AbortMenu, FocusItem, 0x004);
CHECK_OFFSET(AbortMenu, TopItem, 0x008);
CHECK_OFFSET(AbortMenu, Items, 0x00C);
CHECK_OFFSET(AbortMenu, MenuSys, 0x124);
CHECK_OFFSET(AbortMenu, DrawLayer, 0x128);
CHECK_OFFSET(AbortMenu, mVtbl, 0x12C);
CHECK_OFFSET(AbortMenu, mScreen, 0x130);
CHECK_OFFSET(AbortMenu, mAbortItem, 0x134);
CHECK_SIZE(AbortMenu, 0x158);

#endif /* FE_DEBUGLOBBYMENUS_H */
