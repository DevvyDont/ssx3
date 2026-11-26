#ifndef UI_UISCROLLBAR_H
#define UI_UISCROLLBAR_H

// cUIScrollBar: a scroll bar (record type 0x21): background, knob, up/down arrows and an object hidden when the list
// is empty; cUITextScroll points at one (mpScrollBar). Size 0xCC: cUIScreen_createObjectByStruct does
// operator new(0xCC, "UIScroll"...) + the cUIThing ctor cUIThing_cUIThing, zeroes 0x74..0x88 and 0xC0..0xC8, clears the
// two animation names, stores its vtable cUIScrollBar_vtbl and adopts the children named at rec+0x20 / rec+0x24.
// Default ctors cUIScrollBar_cUIScrollBar1 / cUIScrollBar_cUIScrollBar (ui/uivector). Base: cUIThing (ui/uithing.h, 0x74) <- cListNode,
// written out flat below. Class name: SSX On Tour's DWARF (class cUIScrollBar 0xE0, the only On Tour widget with these
// members); the preview keeps no name for it. EA's source file is not known; ui/uiscrollbar.h is our name.
// Methods: vtable cUIScrollBar_vtbl slots 0 ~cUIScrollBar (cUIScrollBar__cUIScrollBar), 9 setUIData (cUIScrollBar_setUIData), 16 render2D
// (cUIScrollBar_render2D), 21 getType (cUIScrollBar_getType); cUIScrollBar_setValues (update position: pos, visible, total), cUIScrollBar_setScrollVisibile
// (show/hide arrows); accessors cUIScrollBar_setBackgroundObject..cUIScrollBar_getValue (ui/uivector). Retail methods: ui/uiscreen 0x39D8C0-0x39DF28.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: On Tour's (each 0xC later there), placed by [R] the ctors and [U] the
// accessors, which follow On Tour's member order.

#include "layout.h"
#include "ui/uithing.h"

// What cUIScrollBar::meHideState hides when the knob fills the track. [O] On Tour enum eScrollBarHide, names and
// values; [U] cUIScrollBar_setValues hides the whole bar (All), the arrows and knob (Slider_HArrows) or only the knob,
// showing the arrows (Slider_SArrows).
enum eScrollBarHide {
    eScrollBarHide_All = 0,
    eScrollBarHide_Slider_HArrows = 1,
    eScrollBarHide_Slider_SArrows = 2,
    eScrollBarHide_None = 3
};

struct cUIScrollBar : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIScrollBar_vtbl (createObjectByStruct, cUIScrollBar_cUIScrollBar1, cUIScrollBar_cUIScrollBar, the dtor
    //       cUIScrollBar__cUIScrollBar)
    // --- cUIScrollBar
    // cUIScrollBar's own slots ([R] cUIScrollBar_vtbl; types [O] On Tour's class cUIScrollBar):
    virtual char getType();                                               // 22 cUIScrollBar_getType__FPv
    cUIThing* mpObjBackground;       // 0x074 [O]; [R] = 0, then the record's child (hash rec+0x20); [U] cUIScrollBar_setBackgroundObject (sw);
                                     //       the dtor cUIScrollBar__cUIScrollBar deletes it; render2D cUIScrollBar_render2D; cUIScrollBar_setValues
    cUIThing* mpObjKnob;             // 0x078 [O]; [R] = 0, then the child named at rec+0x24; [U] cUIScrollBar_setKnobObject; the dtor
                                     //       deletes it; cUIScrollBar_setValues / cUIScrollBar_setScrollVisibile move it
    cUIThing* mpUpArrow;             // 0x07C [O]; [R] = 0; [U] cUIScrollBar_setUpArrowObject (sw); cUIScrollBar_setScrollVisibile
    cUIThing* mpDownArrow;           // 0x080 [O]; [R] = 0; [U] cUIScrollBar_setDownArrowObject (sw); cUIScrollBar_setScrollVisibile
    cUIThing* mpHideOnEmpty;         // 0x084 [O]; [R] = 0; [U] cUIScrollBar_setHideOnEmpty (sw); cUIScrollBar_setScrollVisibile
    int meHideState;                 // 0x088 [O] eScrollBarHide; [R] = 0; [U] cUIScrollBar_setHideState (sw)
    char mszUpAnimation[25];         // 0x08C [O]; [R] [0] = 0 (sb); [U] cUIScrollBar_setUpAnimationName strcpy()s the name in; cUIScrollBar_setValues
                                     //       plays the frame it labels
    char mszDownAnimation[25];       // 0x0A5 [O]; [R] [0] = 0 (sb); [U] cUIScrollBar_setDownAnimationName strcpy; cUIScrollBar_setValues
    char unk_0BE[0x2];               // 0x0BE padding
    int mValue;                      // 0x0C0 [O]; [R] = 0; [U] cUIScrollBar_getValue getValue (lw); cUIScrollBar_setValues stores the position
    int mNumValues;                  // 0x0C4 [O]; [R] = 0; [U] cUIScrollBar_setValues (sw)
    int mPage;                       // 0x0C8 [O]; [R] = 0; [U] cUIScrollBar_setValues (sw)

    // Methods bound to their retail symbols by asm labels.
    void setScrollVisibile(int bVisible) __asm__("cUIScrollBar_setScrollVisibile");
    void* cUIScrollBar1() __asm__("cUIScrollBar_cUIScrollBar1");
    void setBackgroundObject(int background) __asm__("cUIScrollBar_setBackgroundObject");
    void setKnobObject(int knob) __asm__("cUIScrollBar_setKnobObject");
    void setUpArrowObject(int upArrow) __asm__("cUIScrollBar_setUpArrowObject");
    void setDownArrowObject(int downArrow) __asm__("cUIScrollBar_setDownArrowObject");
    void setHideOnEmpty(int hideOnEmpty) __asm__("cUIScrollBar_setHideOnEmpty");
    void setHideState(int hideState) __asm__("cUIScrollBar_setHideState");
    void setUpAnimationName(char* str) __asm__("cUIScrollBar_setUpAnimationName");
    void setDownAnimationName(char* str) __asm__("cUIScrollBar_setDownAnimationName");
    int getValue() __asm__("cUIScrollBar_getValue");
};

CHECK_OFFSET(cUIScrollBar, mpObjBackground, 0x074);
CHECK_OFFSET(cUIScrollBar, mpObjKnob, 0x078);
CHECK_OFFSET(cUIScrollBar, mpUpArrow, 0x07C);
CHECK_OFFSET(cUIScrollBar, mpDownArrow, 0x080);
CHECK_OFFSET(cUIScrollBar, mpHideOnEmpty, 0x084);
CHECK_OFFSET(cUIScrollBar, meHideState, 0x088);
CHECK_OFFSET(cUIScrollBar, mszUpAnimation, 0x08C);
CHECK_OFFSET(cUIScrollBar, mszDownAnimation, 0x0A5);
CHECK_OFFSET(cUIScrollBar, unk_0BE, 0x0BE);
CHECK_OFFSET(cUIScrollBar, mValue, 0x0C0);
CHECK_OFFSET(cUIScrollBar, mNumValues, 0x0C4);
CHECK_OFFSET(cUIScrollBar, mPage, 0x0C8);
CHECK_SIZE(cUIScrollBar, 0xCC);

#endif /* UI_UISCROLLBAR_H */
