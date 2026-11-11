#ifndef INCLUDE_VISUALFX_WORLDLIGHTMAN_CLASSES_H
#define INCLUDE_VISUALFX_WORLDLIGHTMAN_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_wlm {
public:
    virtual ~cRiderVbA0_wlm();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_wlm : public cRiderVbA0_wlm { public: char padA[0xC]; };

class cRiderVbB0_wlm { public: virtual void b01(); };

class cRiderVbB_wlm : public cRiderVbB0_wlm { public: char padB[0x14]; };

class cRiderVbC0_wlm {
public:
    virtual ~cRiderVbC0_wlm();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_wlm : public cRiderVbC0_wlm { public: char padC[0x10]; };

class cRiderVbBC_wlm : public cRiderVbB_wlm, public cRiderVbC_wlm { public: };

class cRiderVbPad_wlm { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_wlm : public cRiderVbPad_wlm, public cRiderVbA_wlm, public cRiderVbBC_wlm {
public:
    virtual ~cRiderVb_wlm();
};

class cVbDtor002F6388 : virtual public cRiderVb_wlm {
public:
    virtual ~cVbDtor002F6388();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_WORLDLIGHTMAN_CLASSES_H
