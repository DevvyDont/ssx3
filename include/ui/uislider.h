#ifndef UI_UISLIDER_H
#define UI_UISLIDER_H

// cUISlider: a slider widget (record type 0x16): a knob moved along a background by value / number of values, stepped
// by input. Size 0x90: cUIScreen_createObjectByStruct does operator new(0x90, "UISlider") + the cUIThing ctor
// cUIThing_cUIThing, zeroes 0x74..0x80 and 0x88/0x8C, stores its vtable cUISlider_vtbl and adopts the children named at
// rec+0x24 (mpObjBackground) and rec+0x28 (mpObjKnob). Default ctors cUISlider_cUISlider1 / cUISlider_cUISlider (ui/uivector).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source: ../source/ui/uislider.h
// (preview assert path); retail keeps its methods in ui/uiscreen (0x39DF28-0x39E288).
// Methods: vtable cUISlider_vtbl slots 0 ~cUISlider (cUISlider__cUISlider), 9 setUIData (cUISlider_setUIData), 14 onInput
// (cUISlider_onInput), 16 render2D (cUISlider_render2D), 21 getType (cUISlider_getType); accessors cUISlider_getValue..cUISlider_setKnobObject.
// Class name: the preview's cUISlider::getType (builds/x/v2/name_map_v2.tsv, cUISlider_getType) and On Tour.
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUISlider 0xA0; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

struct cUISlider : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUISlider_vtbl (createObjectByStruct, cUISlider_cUISlider1, cUISlider_cUISlider, the dtor
    //       cUISlider__cUISlider)
    // --- cUISlider
    // cUISlider's own slots ([R] cUISlider_vtbl; types [O] On Tour's class cUISlider):
    virtual char getType();                                               // 22 cUISlider_getType__FPv
    int mValue;                      // 0x074 [O]; [R] = 0; [U] cUISlider_getValue getValue (lw), cUISlider_setValue setValue (sw);
                                     //       onInput cUISlider_onInput steps it; render2D cUISlider_render2D (lwc1 + cvt.s.w)
    int mNumValues;                  // 0x078 [O]; [R] = 0; [U] setUIData cUISlider_setUIData (sw), cUISlider_setNumValues setNumValues;
                                     //       onInput, render2D (lwc1 + cvt.s.w)
    int mStartPixel;                 // 0x07C [O]; [R] = 0 (no other retail access found)
    int mBackWidth;                  // 0x080 [O]; [R] = 0; [U] setUIData cUISlider_setUIData (sw)
    cUIThing* mSliderObj;            // 0x084 [O] (not stored by the retail ctors; no retail access found)
    cUIThing* mpObjBackground;       // 0x088 [O]; [R] = 0, then the record's child (hash rec+0x24); [U] cUISlider_setBackgroundObject
                                     //       setBackground (sw); the dtor cUISlider__cUISlider deletes it; render2D
    cUIThing* mpObjKnob;             // 0x08C [O]; [R] = 0, then the child named at rec+0x28; [U] cUISlider_setKnobObject setKnob;
                                     //       the dtor deletes it; render2D moves it

    // Methods bound to their retail symbols by asm labels.
    void* cUISlider1() __asm__("cUISlider_cUISlider1");
    int getValue() __asm__("cUISlider_getValue");
    void setValue(int value) __asm__("cUISlider_setValue");
    void setNumValues(int numValues) __asm__("cUISlider_setNumValues");
    void setBackgroundObject(int background) __asm__("cUISlider_setBackgroundObject");
    void setKnobObject(int knob) __asm__("cUISlider_setKnobObject");
    virtual void render2D(float dx, float dy) __asm__("cUISlider_render2D");
};

CHECK_OFFSET(cUISlider, mValue, 0x074);
CHECK_OFFSET(cUISlider, mNumValues, 0x078);
CHECK_OFFSET(cUISlider, mStartPixel, 0x07C);
CHECK_OFFSET(cUISlider, mBackWidth, 0x080);
CHECK_OFFSET(cUISlider, mSliderObj, 0x084);
CHECK_OFFSET(cUISlider, mpObjBackground, 0x088);
CHECK_OFFSET(cUISlider, mpObjKnob, 0x08C);
CHECK_SIZE(cUISlider, 0x90);

#endif /* UI_UISLIDER_H */
