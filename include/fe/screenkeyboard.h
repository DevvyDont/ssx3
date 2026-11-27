#ifndef FE_SCREENKEYBOARD_H
#define FE_SCREENKEYBOARD_H

// cScreenKeyboard: a debug front-end on-screen keyboard (a cUIScreen subclass): a grid of key objects with a cursor,
// shift, an input text of up to mMaxChars characters and a password text. No retail code constructs it (no caller of
// either ctor), so it is dead debug code.
// EA's source file is not known: the preview keeps its inline ctor / dtor and accessors as linkonce copies
// (__15cScreenKeyboard, _$_15cScreenKeyboard, getInputText, getCurrentKey, setInclude*, cursorLeft / cursorRight),
// so the class is declared in a header (the preview's assert strings name fe/festatedebug.h, a candidate); this one is
// named after the class. Retail code: the ctor cScreenKeyboard_cScreenKeyboard1 and the methods in fe/debugfe (0x00180508 ... 0x00180A90),
// initScreenKeyboard / onInput in fe/festatedebug (0x00180B60, 0x00180EF0), and the out-of-line inlines in
// main/all_in_one_main (0x001DD598 ... 0x001DD690).
//
// Size 0x174:
//   [R] the ctor cScreenKeyboard_cScreenKeyboard1 writes every own field up to mCurrentKey at 0x170 (no allocation site: nothing
//       constructs one);
//   [O] On Tour's class ends with mCurrentKey too (0x180 + 4, padded to 0x190 by its 16-byte cUIScreen alignment);
//       its cUIScreen is 0xF0 (retail 0xD4) and its five bools are bytes (retail: words), which moves its 0xF0 ...
//       0x180 to retail's 0xD4 ... 0x170 exactly.
// Base: cUIScreen (ui/uiscreen.h, 0xD4) : cListNode (vptr at 0x8). Both ctors build the cUIScreen part inline (links,
// cUIScreen_vtbl, tAttrScreen_tAttrScreen at 0x14, the thread cList at 0x18, cUIThing_cUIThing + cUIGroup_vtbl at 0x40,
// its cList at 0xB4, mpState at 0xD0) and then store cScreenKeyboard_vtbl (0x0046D1A0, 4 slots: 1 dtor
// cScreenKeyboard__cScreenKeyboard, 2-3 cListNode); the dtor runs cUIScreen__cUIScreen.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes). The cUIScreen part uses ui/uiscreen.h's names; its two embedded objects
// (cList mThreads, cUIGroup mRootGroup) are raw regions here so that this header does not pull in ui/uigroup.h's
// tARGB (fe/debugfe.cpp defines its own tARGB).
// Evidence tags:
//   [R] retail's constructors cScreenKeyboard_cScreenKeyboard1 / cScreenKeyboard_cScreenKeyboard;
//   [P] the July 2003 preview's inline accessors (tools/layout_xlate.py: +0x8 from 0xCC to 0x168; cursorRight
//       0xCC -> 0xD4, cursorLeft / cursorRight 0x164 -> 0x16C, getCurrentKey 0x168 -> 0x170; setIncludeNumbers /
//       setIncludeShift / setIncludeSpace 0xD0 / 0xD4 / 0xD8 -> 0xD8 / 0xDC / 0xE0, bracketed);
//   [U] how retail code uses the field (C bodies in src/fe/debugfe.cpp, src/fe/festatedebug.cpp,
//       src/main/all_in_one_main.cpp, and the asm);
//   [O] SSX On Tour's DWARF (class cScreenKeyboard, cUIScreen): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIState;
struct cUIThing;
struct tUIScreenHeader;
struct tUIKeyframeBlock;
struct tUIObjectBlock;

struct cUIScreen;

struct cScreenKeyboard {
    // --- cUIScreen (ui/uiscreen.h, 0xD4)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the ctors self-link it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked
    void** mVtbl;                    // 0x008 [R] cUIScreen_vtbl, then cScreenKeyboard_vtbl (both ctors)
    char* mpBuffer;                  // 0x00C [O]; [R] 0
    tUIScreenHeader* mpScreenHeader; // 0x010 [O]; [R] 0
    int mAttrScreen;                 // 0x014 [O] tAttrScreen (word, see ui/uiscreen.h); [R] tAttrScreen_tAttrScreen(this + 0x14)
    char mThreads[0x1C];             // 0x018 [O] cList (ui/uiscreen.h: cList mThreads); [R] cList_cList(this + 0x18)
    tUIKeyframeBlock* mpKeyframeBlock; // 0x034 [O]; [R] 0
    tUIObjectBlock* mpObjectBlock;   // 0x038 [O]; [R] 0
    cUIThing** mppObjectList;        // 0x03C [O]; [R] 0
    char mRootGroup[0x90];           // 0x040 [O] cUIGroup (ui/uiscreen.h: cUIGroup mRootGroup); [R] cUIThing_cUIThing(this +
                                     //       0x40), cUIGroup_vtbl at 0x48, cList_cList(this + 0xB4)
    cUIState* mpState;               // 0x0D0 [O]; [R] the ctor's state argument (cScreenKeyboard_cScreenKeyboard1), 0 (the default ctor);
                                     //       [U] pressKey calls its virtual slot 19 after each key press
    // --- cScreenKeyboard
    int mMaxChars;                   // 0x0D4 [O][P] (preview 0xCC, cursorRight); [R] 45; [U] cursorRight / pressKey cap
                                     //       the cursor with it
    int mIncludeNumbers;             // 0x0D8 [O][P] setIncludeNumbers (preview 0xD0) (bool); [R] 0; [U] kbMoveUp /
                                     //       kbMoveDown
    int mIncludeShift;               // 0x0DC [O][P] setIncludeShift (preview 0xD4) (bool); [R] 0
    int mIncludeSpace;               // 0x0E0 [O][P] setIncludeSpace (preview 0xD8) (bool); [R] 0; [U] the kbMove* functions
    int mShiftOn;                    // 0x0E4 [O] (bool); [R] 1; [U] toggleShift flips it, pressKey picks the case by it
    int mFirstKey;                   // 0x0E8 [O] (bool); [R] 1 (no other retail access)
    char mHiScoreName[64];           // 0x0EC [O]; [R] empty; [U] getInputText returns it; setInputText strcpy()s into
                                     //       it; pressKey writes the key's character; initScreenKeyboard shows it
    char mPasswordText[64];          // 0x12C [O]; [U] getPasswordText fills it with one '*' per input character
    int mCursorPos;                  // 0x16C [O][P] cursorLeft / cursorRight (preview 0x164); [R] 0; [U] setInputText puts
                                     //       it at the text's end, pressKey advances it
    int mCurrentKey;                 // 0x170 [O][P] getCurrentKey (preview 0x168); [R] 10 (the "A" key); [U] the kbMove*
                                     //       functions and onInput move it; pressKey indexes the key table with it
                                     //       (also reads its low byte with lbu)

    // The key grid, four rows of ten (names ours). [U] getKeyObjName / getCursorObjName: keys 0-9 are the digits,
    // 10-35 the letters A-Z, then the widgets "arrLeft", "arrRight", "spc" and "end"; pressKey moves the cursor left /
    // right, types a space or ends the input with them; the kbMove* functions wrap within a row of kKeysPerRow and
    // skip kKeySpace when mIncludeSpace is clear.
    enum {
        kKeysPerRow = 10, kKeyFirstLetter = 10, kKeyLeft = 36, kKeyRight = 37, kKeySpace = 38, kKeyEnd = 39,
        kNumKeys = 40
    };

    // Methods bound to their retail symbols by asm labels.
    void setInputText(const char* text) __asm__("cScreenKeyboard_setInputText");
    void kbMoveLeft() __asm__("cScreenKeyboard_kbMoveLeft");
    void kbMoveRight() __asm__("cScreenKeyboard_kbMoveRight");
    void kbMoveUp() __asm__("cScreenKeyboard_kbMoveUp");
    void kbMoveDown() __asm__("cScreenKeyboard_kbMoveDown");
    int toggleShift() __asm__("cScreenKeyboard_toggleShift");
    char* getPasswordText() __asm__("cScreenKeyboard_getPasswordText");
    void getKeyObjName(int keyIndex, char* dst) __asm__("cScreenKeyboard_getKeyObjName");
    void getCursorObjName(int keyIndex, char* dst) __asm__("cScreenKeyboard_getCursorObjName");
    void initScreenKeyboard(cUIScreen* screen) __asm__("cScreenKeyboard_initScreenKeyboard");
    int getCurrentKey() __asm__("cScreenKeyboard_getCurrentKey");
    void setIncludeNumbers(int includeNumbers) __asm__("cScreenKeyboard_setIncludeNumbers");
    void setIncludeShift(int includeShift) __asm__("cScreenKeyboard_setIncludeShift");
    void setIncludeSpace(int includeSpace) __asm__("cScreenKeyboard_setIncludeSpace");
    void cursorRight() __asm__("cScreenKeyboard_cursorRight");
    void cursorLeft() __asm__("cScreenKeyboard_cursorLeft");
};

CHECK_OFFSET(cScreenKeyboard, mpPrev, 0x000);
CHECK_OFFSET(cScreenKeyboard, mpNext, 0x004);
CHECK_OFFSET(cScreenKeyboard, mVtbl, 0x008);
CHECK_OFFSET(cScreenKeyboard, mpBuffer, 0x00C);
CHECK_OFFSET(cScreenKeyboard, mpScreenHeader, 0x010);
CHECK_OFFSET(cScreenKeyboard, mAttrScreen, 0x014);
CHECK_OFFSET(cScreenKeyboard, mThreads, 0x018);
CHECK_OFFSET(cScreenKeyboard, mpKeyframeBlock, 0x034);
CHECK_OFFSET(cScreenKeyboard, mpObjectBlock, 0x038);
CHECK_OFFSET(cScreenKeyboard, mppObjectList, 0x03C);
CHECK_OFFSET(cScreenKeyboard, mRootGroup, 0x040);
CHECK_OFFSET(cScreenKeyboard, mpState, 0x0D0);
CHECK_OFFSET(cScreenKeyboard, mMaxChars, 0x0D4);
CHECK_OFFSET(cScreenKeyboard, mIncludeNumbers, 0x0D8);
CHECK_OFFSET(cScreenKeyboard, mIncludeShift, 0x0DC);
CHECK_OFFSET(cScreenKeyboard, mIncludeSpace, 0x0E0);
CHECK_OFFSET(cScreenKeyboard, mShiftOn, 0x0E4);
CHECK_OFFSET(cScreenKeyboard, mFirstKey, 0x0E8);
CHECK_OFFSET(cScreenKeyboard, mHiScoreName, 0x0EC);
CHECK_OFFSET(cScreenKeyboard, mPasswordText, 0x12C);
CHECK_OFFSET(cScreenKeyboard, mCursorPos, 0x16C);
CHECK_OFFSET(cScreenKeyboard, mCurrentKey, 0x170);
CHECK_SIZE(cScreenKeyboard, 0x174);

#endif /* FE_SCREENKEYBOARD_H */
