#ifndef INCLUDE_SEG_SEG_1F1548_CLASSES_H
#define INCLUDE_SEG_SEG_1F1548_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_s1f {
public:
    virtual ~cRiderVbA0_s1f();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_s1f : public cRiderVbA0_s1f { public: char padA[0xC]; };

class cRiderVbB0_s1f { public: virtual void b01(); };

class cRiderVbB_s1f : public cRiderVbB0_s1f { public: char padB[0x14]; };

class cRiderVbC0_s1f {
public:
    virtual ~cRiderVbC0_s1f();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_s1f : public cRiderVbC0_s1f { public: char padC[0x10]; };

class cRiderVbBC_s1f : public cRiderVbB_s1f, public cRiderVbC_s1f { public: };

class cRiderVbPad_s1f { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_s1f : public cRiderVbPad_s1f, public cRiderVbA_s1f, public cRiderVbBC_s1f {
public:
    cRiderVb_s1f();
    virtual ~cRiderVb_s1f();
};

extern "C" void func_002F1148_0E88(void* self) __asm__("func_002F1148");

struct cSegItem_2F0E88 {
    char data[0x10];
    cSegItem_2F0E88() {}
};

class cSeg_2F0E88 : virtual public cRiderVb_s1f {
public:
    char pad04[0xC];
    cSegItem_2F0E88 a[4];
    cSegItem_2F0E88 b[4];
    char pad90[0x20];
    cSeg_2F0E88();
    virtual ~cSeg_2F0E88();
};

class cVbDtor002F0FE8 : virtual public cRiderVb_s1f {
public:
    char body[0xAC];
    virtual ~cVbDtor002F0FE8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_SEG_SEG_1F1548_CLASSES_H
