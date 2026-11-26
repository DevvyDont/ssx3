#ifndef UI_UIPROGRESS_H
#define UI_UIPROGRESS_H

// cUIProgress: a progress bar (record type 0x19): a background object and a knob moved along it by value / number of
// values. Size 0x88: cUIScreen_createObjectByStruct does operator new(0x88, "UIProgress") + the cUIThing ctor
// cUIThing_cUIThing, zeroes 0x74..0x84, stores cUIProgress_vtbl (D_00494408) and adopts the two children the record names
// (rec+0x20 -> mpObjBackground, rec+0x24 -> mpObjKnob). Default ctors cUIProgress_cUIProgress1 / cUIProgress_cUIProgress (ui/uivector).
// Base: cUIThing (ui/uithing.h, 0x74) <- cListNode, written out flat below. EA source: d:/ssx/code/source/ui/
// uiprogress.cpp (preview assert path); retail keeps its methods in ui/uimenu (cUIProgress__cUIProgress ..
// cUIProgress_setValue) and its inline accessors in ui/uivector (cUIProgress_setBackgroundObject..cUIProgress_setNumValues).
// Methods: vtable slots 0 ~cUIProgress, 9 setUIData, 16 render2D, 18 update, 21 getType; cUIProgress_setValue setValue(int).
//
// A real subclass of cUIThing (its own members FLAT), like main/game.h. Members: SSX On Tour's DWARF (class cUIProgress 0xA0; each member 0xC later there).

#include "layout.h"
#include "ui/uithing.h"

struct cUIProgress : public cUIThing {
    // cUIThing (ui/uithing.h, 0x74): the vptr at 0x008:
    //   [R] cUIProgress_vtbl D_00494408 (createObjectByStruct, cUIProgress_cUIProgress1, cUIProgress_cUIProgress, the
    //       dtor)
    // --- cUIProgress
    // cUIProgress's own slots ([R] cUIProgress_vtbl; types [O] On Tour's class cUIProgress):
    virtual char getType();                                               // 22 cUIProgress_getType__FPv
    cUIThing* mpObjBackground;       // 0x074 [O]; [R] = 0, then the record's child (hash rec+0x20); [U] cUIProgress_setBackgroundObject
                                     //       setBackground (sw); the dtor deletes it; render2D, update, cUIProgress_setValue
    cUIThing* mpObjKnob;             // 0x078 [O]; [R] = 0, then the child named at rec+0x24; [U] cUIProgress_setKnobObject setKnob;
                                     //       the dtor deletes it; update, cUIProgress_setValue moves it
    int mValue;                      // 0x07C [O]; [R] = 0; [U] cUIProgress_getValue getValue (lw), cUIProgress_setValue setValue (sw)
    int mNumValues;                  // 0x080 [O]; [R] = 0; [U] cUIProgress_setUIData (sw), cUIProgress_setNumValues setNumValues
    int mBackWidth;                  // 0x084 [O]; [R] = 0; [U] cUIProgress_setUIData (sw); cUIProgress_setValue

    // Methods bound to their retail symbols by asm labels.
    void setValue(int pos) __asm__("cUIProgress_setValue");
    void* cUIProgress1() __asm__("cUIProgress_cUIProgress1");
    void setBackgroundObject(int background) __asm__("cUIProgress_setBackgroundObject");
    void setKnobObject(int knob) __asm__("cUIProgress_setKnobObject");
    int getValue() __asm__("cUIProgress_getValue");
    void setNumValues(int numValues) __asm__("cUIProgress_setNumValues");
    virtual void render2D(float x, float y) __asm__("cUIProgress_render2D");
    virtual void update(unsigned short ev) __asm__("cUIProgress_update");
};

CHECK_OFFSET(cUIProgress, mpObjBackground, 0x074);
CHECK_OFFSET(cUIProgress, mpObjKnob, 0x078);
CHECK_OFFSET(cUIProgress, mValue, 0x07C);
CHECK_OFFSET(cUIProgress, mNumValues, 0x080);
CHECK_OFFSET(cUIProgress, mBackWidth, 0x084);
CHECK_SIZE(cUIProgress, 0x88);

#endif /* UI_UIPROGRESS_H */
