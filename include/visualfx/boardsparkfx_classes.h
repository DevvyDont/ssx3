#ifndef INCLUDE_VISUALFX_BOARDSPARKFX_CLASSES_H
#define INCLUDE_VISUALFX_BOARDSPARKFX_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

void operator_delete(int* p);

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_bsf {
public:
    virtual ~cRiderVbA0_bsf();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_bsf : public cRiderVbA0_bsf { public: char padA[0xC]; };

class cRiderVbB0_bsf { public: virtual void b01(); };

class cRiderVbB_bsf : public cRiderVbB0_bsf { public: char padB[0x14]; };

class cRiderVbC0_bsf {
public:
    virtual ~cRiderVbC0_bsf();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_bsf : public cRiderVbC0_bsf { public: char padC[0x10]; };

class cRiderVbBC_bsf : public cRiderVbB_bsf, public cRiderVbC_bsf { public: };

class cRiderVbPad_bsf { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_bsf : public cRiderVbPad_bsf, public cRiderVbA_bsf, public cRiderVbBC_bsf {
public:
    virtual ~cRiderVb_bsf();
};

class cVbMember002DA8E8 {
public:
    char pad[0x1F8];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual ~cVbMember002DA8E8();
};

class cVbDtor002DA8E8 : virtual public cRiderVb_bsf {
public:
    char pad04[0x1C];
    cVbMember002DA8E8* m20;
    void* m24;
    void* m28;
    char pad2C[0x80];
    virtual ~cVbDtor002DA8E8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_BOARDSPARKFX_CLASSES_H
