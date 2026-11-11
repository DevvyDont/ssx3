#ifndef INCLUDE_VISUALFX_RENDERSTATEMAN_CLASSES_H
#define INCLUDE_VISUALFX_RENDERSTATEMAN_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_rsm {
public:
    virtual ~cRiderVbA0_rsm();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_rsm : public cRiderVbA0_rsm { public: char padA[0xC]; };

class cRiderVbB0_rsm { public: virtual void b01(); };

class cRiderVbB_rsm : public cRiderVbB0_rsm { public: char padB[0x14]; };

class cRiderVbC0_rsm {
public:
    virtual ~cRiderVbC0_rsm();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_rsm : public cRiderVbC0_rsm { public: char padC[0x10]; };

class cRiderVbBC_rsm : public cRiderVbB_rsm, public cRiderVbC_rsm { public: };

class cRiderVbPad_rsm { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_rsm : public cRiderVbPad_rsm, public cRiderVbA_rsm, public cRiderVbBC_rsm {
public:
    cRiderVb_rsm();
    virtual ~cRiderVb_rsm();
};

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

extern char D_00487B78[];

extern "C" void func_002EF6A0_F3B0(void* self) __asm__("func_002EF6A0");

struct cRsmItem_2EF3B0 {
    char data[0x20];
    cRsmItem_2EF3B0() {}
};

class cRsm_2EF3B0 : virtual public cRiderVb_rsm {
public:
    cRsmItem_2EF3B0* items;     // 0x4
    char pad08[0x18];
    cRsm_2EF3B0();
    virtual ~cRsm_2EF3B0();
};

void operator_delete(int* p);

void cMemMan_free(void* p);

class cVbDtor002EF530 : virtual public cRiderVb_rsm {
public:
    void* m04;
    char pad08[0x18];
    virtual ~cVbDtor002EF530();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_RENDERSTATEMAN_CLASSES_H
