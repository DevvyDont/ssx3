#ifndef FE_KEYBOARDPOPUP_H
#define FE_KEYBOARDPOPUP_H

// cKeyboardPopup: the front end's on-screen keyboard (84 keys in 6 rows of 14, screen "Fullkeyboard"), opened by a
// cScreenPopup's text box and by the profile/lodge/trophy screens. A cUIState; its methods are in fe/fepopup.cpp.
// Retail vtable _vt$14cKeyboardPopup = cKeyboardPopup_vtbl. Created with cMemMan_alloc(0x444, "cKeyboardPopup", ...) +
// cKeyboardPopup(cUIEngine*, cUIState* parent, bool Password, unsigned char CtrlBits) = cKeyboardPopup_cKeyboardPopup; destroyed by
// cKeyboardPopup__cKeyboardPopup.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*, cUIState*) = cUIState_cUIState1 (ui/uiscreen.cpp), cKeyboardPopup_cKeyboardPopup,
//       the destructor, and the allocation size (0x444 at every site: fepopup, feasyncfile, festateriderbio, ...);
//   [P] the July 2003 preview's inline accessors, carried to retail by tools/layout_xlate.py;
//   [U] how matched retail code uses the field (UI object names are strings read from the retail ELF);
//   [O] SSX On Tour's DWARF (class cListNode / cUIState / cKeyboardPopup, size 0x234 there): the same members in the
//       same order. On Tour's bools are 1 byte; retail's are words (bool is 4 bytes in this compiler), which is the
//       whole difference: every retail field below falls where On Tour's order puts it with 4-byte bools, and the
//       last one ends exactly at retail's 0x444.
// Vectors are EA's t3Vector (3 floats, 4-aligned), written as float arrays because fe/fepopup.cpp defines its own
// t3Vector.

#include "layout.h"
#include "bx/bxstring.h"
#include "bx/math3d.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIVector;

struct cUIText;

struct cKeyboardPopup : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] _vt$14cKeyboardPopup cKeyboardPopup_vtbl (cKeyboardPopup_cKeyboardPopup, destructor)
    // cKeyboardPopup's notes on cUIState's fields:
    //   mpEngine: [U] state stack at mpEngine + 0x18
    //   mCtrlBits: [R] cKeyboardPopup_cKeyboardPopup stores its CtrlBits argument (sb)
    //   mpParentState: [U] receives onWidgetEvent (slot 19), handleEvent (slot 24) and onUpdate (slot 12) calls
    //   mpScreen: [R] ctor zeroes it; [U] cKeyboardPopup_onCreateScreen stores "Fullkeyboard"
    // --- cKeyboardPopup
    cUIVector* mpBackCompact;       // 0x048 [O]; [R] ctor zeroes; [U] widget "back_com", shown for keyboard type 1 ("compact")
    cUIVector* mpBackRegular;       // 0x04C [O]; [R] ctor zeroes; [U] widget "back_reg", shown for type 0 ("regular")
    cUIVector* mpBackExpand;        // 0x050 [O]; [R] ctor zeroes; [U] widget "back_exp", shown for type 2 ("expanded")
    int mbClearOnFirst;             // 0x054 [O]; [R] ctor zeroes; [U] cKeyboardPopup_pressKey presses "clear" (key 70)
                                    //       before the first key, then resets it
    int mbDisableForFirst;          // 0x058 [O]; [R] ctor zeroes; [U] when set the space key (75) is enabled only once
                                    //       the text is non-empty (setInputText, pressKey, cKeyboardPopup_fillSelectableKeys)
    int mbAcceptText;               // 0x05C [O]; [R] ctor zeroes; [U] pressKey sets it on the accept key (55)
    int miMaxCharacters;            // 0x060 [P] getMaxCharacters (preview 0x44, seen); [R] ctor 0x3F; [U] setMaxCharacters
                                    //       (cKeyboardPopup_setMaxCharacters) clamps to 0..0x3F
    int miMinCharacters;            // 0x064 [O]; [R] ctor zeroes; [U] setMinCharacters (cKeyboardPopup_setMinCharacters); the accept key is
                                    //       disabled while the text is shorter
    int mbCapsOn;                   // 0x068 [O]; [R] ctor 1 (or bit 1 of Keyboard + 0x88); [U] key 42 toggles it
    int mbShiftOn;                  // 0x06C [O]; [R] ctor zeroes; [U] key 56 toggles it, other keys clear it
    int mbPassword;                 // 0x070 [O]; [R] the ctor's Password argument; [U] updateEdit shows mszPasswordString
    char mszEditString[64];         // 0x074 [O]; [R] ctor stores 0 in byte 0; [U] setInputText strcpy, key 70 clears 0x40
    char mszViewableString[64];     // 0x0B4 [O]; [U] cKeyboardPopup_updateViewable strncpy's the visible part into it
    char mszPasswordString[64];     // 0x0F4 [O]; [U] cKeyboardPopup_setPasswordText fills it with '*'
    int miCursorPos;                // 0x134 [P] cursorRight (preview 0x128 -> retail 0x134, seen); [U] setInputText = strlen
    int miViewableStartIndex;       // 0x138 [O]; [U] updateViewable
    int miViewableEndIndex;         // 0x13C [O]; [U] updateViewable
    int miCurrentKey;               // 0x140 [P] getCurrentKey (preview 0x134, +0xC as for miCursorPos); [R] ctor 0x1D;
                                    //       [U] the selected key 0..83 in every kbMove* / pressKey
    int meSkipState;                // 0x144 [O] eKeyStateType; [R] ctor 2; [U] set after allowSelection in kbMove*
    int mbRegular[84];              // 0x148 [O] (On Tour: bool[84]); [U] cKeyboardPopup_setIncludeKey (modes 0/2) and
                                    //       cKeyboardPopup_allowSelection index it by key
    int mbAlternate[84];            // 0x298 [O] (On Tour: bool[84]); [U] setIncludeKey (modes 1/2), allowSelection
    t3Vector mvPosEnter;            // 0x3E8 [O]; [U] position of "EnterValue" (onGainTransition, updateCursor)
    t3Vector mvSizeEnter;           // 0x3F4 [O]; [U] size of "EnterValue" (vt slot 20)
    t3Vector mvSizeCursor;          // 0x400 [O]; [U] size of "CharacterCursor" (onGainTransition)
    t3Vector mvPosCursor;           // 0x40C [O]; [U] position of "CharacterCursor"
    int mbSetInputText;             // 0x418 [O]; [R] ctor zeroes; [U] setInputText sets it; onGainTransition re-applies
                                    //       mszEditString when set
    int mbTransistionDone;          // 0x41C [O] (EA's spelling); [R] ctor zeroes; [U] set by the first onGainTransition;
                                    //       setInputText calls updateEdit only when set
    int mbPopOnEnd;                 // 0x420 [O]; [R] ctor 1; [U] pressKey's accept key pops the state stack when set
    cBXString mszStatic;            // 0x424 [O]; [R] ctor stores the empty-string rep cBXString_EmptyString then
                                    //       cBXString_Reset, the dtor destroys it; [U] setStatic(const char*) (cKeyboardPopup_setStatic2)
    unsigned short* mpustrStatic;   // 0x428 [O]; [R] ctor zeroes, the dtor frees it; [U] cKeyboardPopup_setStatic copies
                                    //       the unicode string into it before the screen exists
    unsigned int muiStatic;         // 0x42C [O]; [R] ctor zeroes; [U] cKeyboardPopup_setStatic1 stores the string id
    int meLastDirection;            // 0x430 [O] eMovementDirection; [U] the kbMove* functions store 0..3
    int miLastKey;                  // 0x434 [O]; [R] ctor 0x2B; [U] the kbMove* functions save miCurrentKey into it
    int mbReferEventsToParent;      // 0x438 [O]; [R] ctor zeroes; [U] handleEvent (cKeyboardPopup_handleEvent) forwards to the parent
    int meType;                     // 0x43C [O] eKeyboardType; [R] ctor 1; [U] onGainTransition: 0 "regular", 1 "compact",
                                    //       2 "expanded"
    int mbCallParentsUpdate;        // 0x440 [O]; [R] ctor zeroes; [U] onUpdate (cKeyboardPopup_onUpdate) calls the parent's onUpdate

    // Methods bound to their retail symbols by asm labels.
    void setInputText(const char* str) __asm__("cKeyboardPopup_setInputText");
    void kbMoveLeft(int bControllerInput) __asm__("cKeyboardPopup_kbMoveLeft");
    void kbMoveRight(int bControllerInput) __asm__("cKeyboardPopup_kbMoveRight");
    void kbMoveUp(int bControllerInput) __asm__("cKeyboardPopup_kbMoveUp");
    void kbMoveDown(int bControllerInput) __asm__("cKeyboardPopup_kbMoveDown");
    int handleMovementUpFromQuickKeyRow(int bUseReg) __asm__("cKeyboardPopup_handleMovementUpFromQuickKeyRow");
    int handleMovementDownFromQuickKeyRow(int bUseReg) __asm__("cKeyboardPopup_handleMovementDownFromQuickKeyRow");
    int handleMovementDownFromLast(int bUseReg) __asm__("cKeyboardPopup_handleMovementDownFromLast");
    int handleMovementUpFromNumberRow(int bUseReg) __asm__("cKeyboardPopup_handleMovementUpFromNumberRow");
    int handleMovementUpFromLast(int bUseReg) __asm__("cKeyboardPopup_handleMovementUpFromLast");
    int handleMovementDownToLast(int bUseReg) __asm__("cKeyboardPopup_handleMovementDownToLast");
    void skipRowToTop() __asm__("cKeyboardPopup_skipRowToTop");
    void skipRowToBottom(int start) __asm__("cKeyboardPopup_skipRowToBottom");
    void moveUpRows() __asm__("cKeyboardPopup_moveUpRows");
    void moveDownRows() __asm__("cKeyboardPopup_moveDownRows");
    void pressKey(int flag) __asm__("cKeyboardPopup_pressKey");
    void AddAsciiEdit(int value) __asm__("cKeyboardPopup_AddAsciiEdit");
    int updateToAscii(int value) __asm__("cKeyboardPopup_updateToAscii");
    void AddCharacterEdit() __asm__("cKeyboardPopup_AddCharacterEdit");
    void updateEdit(int bDropFromRight) __asm__("cKeyboardPopup_updateEdit");
    void updateViewable(int bDropFromRight) __asm__("cKeyboardPopup_updateViewable");
    void updateCursor() __asm__("cKeyboardPopup_updateCursor");
    void setPasswordText(char* str) __asm__("cKeyboardPopup_setPasswordText");
    void setStatic2(const char* str) __asm__("cKeyboardPopup_setStatic2");
    void setStatic(unsigned short* str) __asm__("cKeyboardPopup_setStatic");
    void setStatic1(int id) __asm__("cKeyboardPopup_setStatic1");
    void setMaxCharacters(int iMaxCharacters) __asm__("cKeyboardPopup_setMaxCharacters");
    void setMinCharacters(int iMinCharacters) __asm__("cKeyboardPopup_setMinCharacters");
    int getObjName(int key, char* out, int shifted) __asm__("cKeyboardPopup_getObjName");
    void redrawScreen() __asm__("cKeyboardPopup_redrawScreen");
    void SetNameForKey(cUIText* text, int key) __asm__("cKeyboardPopup_SetNameForKey");
    void fillSelectableKeys() __asm__("cKeyboardPopup_fillSelectableKeys");
    void setIncludeKey(int i, int bIncludeKey, int mode) __asm__("cKeyboardPopup_setIncludeKey");
    void setDisableShiftNumbers(bool on) __asm__("cKeyboardPopup_setDisableShiftNumbers");
    void setIncludeQuickKeyRow(int included) __asm__("cKeyboardPopup_setIncludeQuickKeyRow");
    void setScale(cUIText* obj) __asm__("cKeyboardPopup_setScale");
    void checkAndFixConflict() __asm__("cKeyboardPopup_checkAndFixConflict");
    int allowSelection(int idx, int out) __asm__("cKeyboardPopup_allowSelection");
    void handleQuickKeyPress(int key) __asm__("cKeyboardPopup_handleQuickKeyPress");
    cKeyboardPopup * cKeyboardPopup1() __asm__("cKeyboardPopup_cKeyboardPopup1");
    int getCurrentKey() __asm__("cKeyboardPopup_getCurrentKey");
    void setCurrentKey(int currentKey) __asm__("cKeyboardPopup_setCurrentKey");
    void setKeyboardType(int keyboardType) __asm__("cKeyboardPopup_setKeyboardType");
    int getKeyboardType() __asm__("cKeyboardPopup_getKeyboardType");
    int getPopOnEnd() __asm__("cKeyboardPopup_getPopOnEnd");
    void setPopOnEnd(int popOnEnd) __asm__("cKeyboardPopup_setPopOnEnd");
    void setPassword(int password) __asm__("cKeyboardPopup_setPassword");
    void setClearOnFirstCharacter(int clearOnFirst) __asm__("cKeyboardPopup_setClearOnFirstCharacter");
    void setDisableSpaceForFirst(int disableForFirst) __asm__("cKeyboardPopup_setDisableSpaceForFirst");
    int getMinCharacters() __asm__("cKeyboardPopup_getMinCharacters");
    int getMaxCharacters() __asm__("cKeyboardPopup_getMaxCharacters");
    void setReferEventsToParent(int referEvents) __asm__("cKeyboardPopup_setReferEventsToParent");
    int getReferEventsToParent() __asm__("cKeyboardPopup_getReferEventsToParent");
    void setCallParentsUpdate(int callParentUpdate) __asm__("cKeyboardPopup_setCallParentsUpdate");
    int isAcceptText() __asm__("cKeyboardPopup_isAcceptText");
    void cursorRight() __asm__("cKeyboardPopup_cursorRight");
    void cursorLeft() __asm__("cKeyboardPopup_cursorLeft");
};

CHECK_OFFSET(cKeyboardPopup, mpBackCompact, 0x48);
CHECK_OFFSET(cKeyboardPopup, mpBackRegular, 0x4C);
CHECK_OFFSET(cKeyboardPopup, mpBackExpand, 0x50);
CHECK_OFFSET(cKeyboardPopup, mbClearOnFirst, 0x54);
CHECK_OFFSET(cKeyboardPopup, mbDisableForFirst, 0x58);
CHECK_OFFSET(cKeyboardPopup, mbAcceptText, 0x5C);
CHECK_OFFSET(cKeyboardPopup, miMaxCharacters, 0x60);
CHECK_OFFSET(cKeyboardPopup, miMinCharacters, 0x64);
CHECK_OFFSET(cKeyboardPopup, mbCapsOn, 0x68);
CHECK_OFFSET(cKeyboardPopup, mbShiftOn, 0x6C);
CHECK_OFFSET(cKeyboardPopup, mbPassword, 0x70);
CHECK_OFFSET(cKeyboardPopup, mszEditString, 0x74);
CHECK_OFFSET(cKeyboardPopup, mszViewableString, 0xB4);
CHECK_OFFSET(cKeyboardPopup, mszPasswordString, 0xF4);
CHECK_OFFSET(cKeyboardPopup, miCursorPos, 0x134);
CHECK_OFFSET(cKeyboardPopup, miViewableStartIndex, 0x138);
CHECK_OFFSET(cKeyboardPopup, miViewableEndIndex, 0x13C);
CHECK_OFFSET(cKeyboardPopup, miCurrentKey, 0x140);
CHECK_OFFSET(cKeyboardPopup, meSkipState, 0x144);
CHECK_OFFSET(cKeyboardPopup, mbRegular, 0x148);
CHECK_OFFSET(cKeyboardPopup, mbAlternate, 0x298);
CHECK_OFFSET(cKeyboardPopup, mvPosEnter, 0x3E8);
CHECK_OFFSET(cKeyboardPopup, mvSizeEnter, 0x3F4);
CHECK_OFFSET(cKeyboardPopup, mvSizeCursor, 0x400);
CHECK_OFFSET(cKeyboardPopup, mvPosCursor, 0x40C);
CHECK_OFFSET(cKeyboardPopup, mbSetInputText, 0x418);
CHECK_OFFSET(cKeyboardPopup, mbTransistionDone, 0x41C);
CHECK_OFFSET(cKeyboardPopup, mbPopOnEnd, 0x420);
CHECK_OFFSET(cKeyboardPopup, mszStatic, 0x424);
CHECK_OFFSET(cKeyboardPopup, mpustrStatic, 0x428);
CHECK_OFFSET(cKeyboardPopup, muiStatic, 0x42C);
CHECK_OFFSET(cKeyboardPopup, meLastDirection, 0x430);
CHECK_OFFSET(cKeyboardPopup, miLastKey, 0x434);
CHECK_OFFSET(cKeyboardPopup, mbReferEventsToParent, 0x438);
CHECK_OFFSET(cKeyboardPopup, meType, 0x43C);
CHECK_OFFSET(cKeyboardPopup, mbCallParentsUpdate, 0x440);
CHECK_SIZE(cKeyboardPopup, 0x444);

#endif /* FE_KEYBOARDPOPUP_H */
