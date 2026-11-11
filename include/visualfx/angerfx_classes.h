#ifndef INCLUDE_VISUALFX_ANGERFX_CLASSES_H
#define INCLUDE_VISUALFX_ANGERFX_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_agf {
public:
    virtual ~cRiderVbA0_agf();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_agf : public cRiderVbA0_agf { public: char padA[0xC]; };

class cRiderVbB0_agf { public: virtual void b01(); };

class cRiderVbB_agf : public cRiderVbB0_agf { public: char padB[0x14]; };

class cRiderVbC0_agf {
public:
    virtual ~cRiderVbC0_agf();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_agf : public cRiderVbC0_agf { public: char padC[0x10]; };

class cRiderVbBC_agf : public cRiderVbB_agf, public cRiderVbC_agf { public: };

class cRiderVbPad_agf { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_agf : public cRiderVbPad_agf, public cRiderVbA_agf, public cRiderVbBC_agf {
public:
    cRiderVb_agf();
    virtual ~cRiderVb_agf();
};

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

extern char D_00487518[];

extern "C" void func_002D4BE0_erFX(void* self) __asm__("func_002D4BE0");

struct cAngerPart_ang {
    char data[0x2C];
    cAngerPart_ang() {}
};

class cAngerFX_ang : virtual public cRiderVb_agf {
public:
    cAngerPart_ang* parts;      // 0x4
    char pad08[0x8];
    cAngerFX_ang();
    virtual ~cAngerFX_ang();
};

void operator_delete(int* p);

void cMemMan_free(void* p);

class cVbDtor002D4A70 : virtual public cRiderVb_agf {
public:
    void* m04;
    virtual ~cVbDtor002D4A70();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_ANGERFX_CLASSES_H
