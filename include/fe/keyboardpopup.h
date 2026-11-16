#ifndef FE_KEYBOARDPOPUP_H
#define FE_KEYBOARDPOPUP_H

// cKeyboardPopup: the front end's on-screen keyboard (84 keys in 6 rows of 14, screen "Fullkeyboard"), opened by a
// cScreenPopup's text box and by the profile/lodge/trophy screens. A cUIState; its methods are in fe/fepopup.cpp.
// Retail vtable _vt$14cKeyboardPopup = cKeyboardPopup_vtbl. Created with cMemMan_alloc(0x444, "cKeyboardPopup", ...) +
// cKeyboardPopup(cUIEngine*, cUIState* parent, bool Password, unsigned char CtrlBits) = func_001CB030; destroyed by
// cKeyboardPopup__cKeyboardPopup.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*, cUIState*) = func_0039E318 (ui/uiscreen.cpp), func_001CB030,
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

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIVector;

struct cKeyboardPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] _vt$14cKeyboardPopup cKeyboardPopup_vtbl (func_001CB030, destructor)
    // --- cUIState (On Tour: size 0x48, same offsets)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] state stack at mpEngine + 0x18
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] func_001CB030 stores its CtrlBits argument (sb)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word (ui/uistate.h); [R] ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's parent argument; [U] receives onWidgetEvent (slot 19),
                                    //       handleEvent (slot 24) and onUpdate (slot 12) calls
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C);
                                    //       [R] constructed by func_003977E8(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] cKeyboardPopup_onCreateScreen stores "Fullkeyboard"
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cKeyboardPopup
    cUIVector* mpBackCompact;       // 0x048 [O]; [R] ctor zeroes; [U] widget "back_com", shown for keyboard type 1 ("compact")
    cUIVector* mpBackRegular;       // 0x04C [O]; [R] ctor zeroes; [U] widget "back_reg", shown for type 0 ("regular")
    cUIVector* mpBackExpand;        // 0x050 [O]; [R] ctor zeroes; [U] widget "back_exp", shown for type 2 ("expanded")
    int mbClearOnFirst;             // 0x054 [O]; [R] ctor zeroes; [U] cKeyboardPopup_pressKey presses "clear" (key 70)
                                    //       before the first key, then resets it
    int mbDisableForFirst;          // 0x058 [O]; [R] ctor zeroes; [U] when set the space key (75) is enabled only once
                                    //       the text is non-empty (setInputText, pressKey, func_001CE2A8)
    int mbAcceptText;               // 0x05C [O]; [R] ctor zeroes; [U] pressKey sets it on the accept key (55)
    int miMaxCharacters;            // 0x060 [P] getMaxCharacters (preview 0x44, seen); [R] ctor 0x3F; [U] setMaxCharacters
                                    //       (func_001CD088) clamps to 0..0x3F
    int miMinCharacters;            // 0x064 [O]; [R] ctor zeroes; [U] setMinCharacters (func_001CD0B0); the accept key is
                                    //       disabled while the text is shorter
    int mbCapsOn;                   // 0x068 [O]; [R] ctor 1 (or bit 1 of D_004A33F0 + 0x88); [U] key 42 toggles it
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
    float mvPosEnter[3];            // 0x3E8 [O] t3Vector; [U] position of "EnterValue" (onGainTransition, updateCursor)
    float mvSizeEnter[3];           // 0x3F4 [O] t3Vector; [U] size of "EnterValue" (vt slot 20)
    float mvSizeCursor[3];          // 0x400 [O] t3Vector; [U] size of "CharacterCursor" (onGainTransition)
    float mvPosCursor[3];           // 0x40C [O] t3Vector; [U] position of "CharacterCursor"
    int mbSetInputText;             // 0x418 [O]; [R] ctor zeroes; [U] setInputText sets it; onGainTransition re-applies
                                    //       mszEditString when set
    int mbTransistionDone;          // 0x41C [O] (EA's spelling); [R] ctor zeroes; [U] set by the first onGainTransition;
                                    //       setInputText calls updateEdit only when set
    int mbPopOnEnd;                 // 0x420 [O]; [R] ctor 1; [U] pressKey's accept key pops the state stack when set
    char* mszStatic;                // 0x424 [O] cBXString; [R] ctor stores the empty-string rep D_004A3E90 then
                                    //       cBXString_Reset, the dtor destroys it; [U] setStatic(const char*) (func_001CCF00)
    unsigned short* mpustrStatic;   // 0x428 [O]; [R] ctor zeroes, the dtor frees it; [U] cKeyboardPopup_setStatic copies
                                    //       the unicode string into it before the screen exists
    unsigned int muiStatic;         // 0x42C [O]; [R] ctor zeroes; [U] cKeyboardPopup_setStatic1 stores the string id
    int meLastDirection;            // 0x430 [O] eMovementDirection; [U] the kbMove* functions store 0..3
    int miLastKey;                  // 0x434 [O]; [R] ctor 0x2B; [U] the kbMove* functions save miCurrentKey into it
    int mbReferEventsToParent;      // 0x438 [O]; [R] ctor zeroes; [U] handleEvent (func_001CB3D0) forwards to the parent
    int meType;                     // 0x43C [O] eKeyboardType; [R] ctor 1; [U] onGainTransition: 0 "regular", 1 "compact",
                                    //       2 "expanded"
    int mbCallParentsUpdate;        // 0x440 [O]; [R] ctor zeroes; [U] onUpdate (cKeyboardPopup_onUpdate) calls the parent's onUpdate
};

CHECK_OFFSET(cKeyboardPopup, mpPrev, 0x0);
CHECK_OFFSET(cKeyboardPopup, mpNext, 0x4);
CHECK_OFFSET(cKeyboardPopup, mVtbl, 0x8);
CHECK_OFFSET(cKeyboardPopup, mHashName, 0xC);
CHECK_OFFSET(cKeyboardPopup, mpEngine, 0x10);
CHECK_OFFSET(cKeyboardPopup, mCurrentCtrl, 0x14);
CHECK_OFFSET(cKeyboardPopup, mCtrlBits, 0x15);
CHECK_OFFSET(cKeyboardPopup, unk_016, 0x16);
CHECK_OFFSET(cKeyboardPopup, miMiscData, 0x18);
CHECK_OFFSET(cKeyboardPopup, mAttrState, 0x1C);
CHECK_OFFSET(cKeyboardPopup, mpParentState, 0x20);
CHECK_OFFSET(cKeyboardPopup, mScreens, 0x24);
CHECK_OFFSET(cKeyboardPopup, mpScreen, 0x40);
CHECK_OFFSET(cKeyboardPopup, mUserNum, 0x44);
CHECK_OFFSET(cKeyboardPopup, unk_045, 0x45);
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
