#ifndef INCLUDE_VISUALFX_BOARDTRAILFX_CLASSES_H
#define INCLUDE_VISUALFX_BOARDTRAILFX_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

void operator_delete(int* p);

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_btf {
public:
    virtual ~cRiderVbA0_btf();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_btf : public cRiderVbA0_btf { public: char padA[0xC]; };

class cRiderVbB0_btf { public: virtual void b01(); };

class cRiderVbB_btf : public cRiderVbB0_btf { public: char padB[0x14]; };

class cRiderVbC0_btf {
public:
    virtual ~cRiderVbC0_btf();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_btf : public cRiderVbC0_btf { public: char padC[0x10]; };

class cRiderVbBC_btf : public cRiderVbB_btf, public cRiderVbC_btf { public: };

class cRiderVbPad_btf { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_btf : public cRiderVbPad_btf, public cRiderVbA_btf, public cRiderVbBC_btf {
public:
    cRiderVb_btf();
    virtual ~cRiderVb_btf();
};

void cMemMan_free(void* p);

class cVbDtor002E83F0 : virtual public cRiderVb_btf {
public:
    char pad04[0xC];
    void* m10;
    char pad14[0xD8];
    virtual ~cVbDtor002E83F0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

void func_002EADC0_AAE0(void* self) __asm__("func_002EADC0__FPv");

struct cBtfIn_2EAAE0 {
    char data[0x10];
    cBtfIn_2EAAE0() {}
};

class cBtf_2EAAE0 : virtual public cRiderVb_btf {
public:
    char pad04[0xC];
    cBtfIn_2EAAE0 a[3][4];
    char pad[0x34];
    cBtf_2EAAE0();
    virtual ~cBtf_2EAAE0();
};

class cVbDtor002EAC60 : virtual public cRiderVb_btf {
public:
    char body[0x10C];
    virtual ~cVbDtor002EAC60();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_BOARDTRAILFX_CLASSES_H
