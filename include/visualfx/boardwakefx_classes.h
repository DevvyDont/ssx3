#ifndef INCLUDE_VISUALFX_BOARDWAKEFX_CLASSES_H
#define INCLUDE_VISUALFX_BOARDWAKEFX_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

void operator_delete(int* p);

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_bwf {
public:
    virtual ~cRiderVbA0_bwf();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_bwf : public cRiderVbA0_bwf { public: char padA[0xC]; };

class cRiderVbB0_bwf { public: virtual void b01(); };

class cRiderVbB_bwf : public cRiderVbB0_bwf { public: char padB[0x14]; };

class cRiderVbC0_bwf {
public:
    virtual ~cRiderVbC0_bwf();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_bwf : public cRiderVbC0_bwf { public: char padC[0x10]; };

class cRiderVbBC_bwf : public cRiderVbB_bwf, public cRiderVbC_bwf { public: };

class cRiderVbPad_bwf { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_bwf : public cRiderVbPad_bwf, public cRiderVbA_bwf, public cRiderVbBC_bwf {
public:
    virtual ~cRiderVb_bwf();
};

void cMemMan_free(void* p);

class cVbDtor002DCDA0 : virtual public cRiderVb_bwf {
public:
    char pad04[0x8];
    void* m0C;
    void* m10;
    char pad14[0xA8];
    virtual ~cVbDtor002DCDA0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbElemBase002DF1C8 {
public:
    char pad[0x1F8];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual ~cVbElemBase002DF1C8();
};

class cVbElem002DF1C8 : public cVbElemBase002DF1C8 {
public:
    char pad2[0x14];
    virtual ~cVbElem002DF1C8();
    static void operator delete[](void* p) { cMemMan_free(p); }
};

class cVbDtor002DF1C8 : virtual public cRiderVb_bwf {
public:
    char pad04[0x24];
    cVbElem002DF1C8* m28;
    void* m2C;
    char pad30[0x4C];
    void* m7C;
    char pad80[0xAC];
    virtual ~cVbDtor002DF1C8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbDtor002E37D0 : virtual public cRiderVb_bwf {
public:
    char body[0x3C];
    virtual ~cVbDtor002E37D0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

extern char* D_004A289C;

extern "C" void func_0037C720(void* mgr, float f);

class cVbDtor002E64C0 : virtual public cRiderVb_bwf {
public:
    char pad04[0x3A8];
    void* m3AC;
    virtual ~cVbDtor002E64C0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_VISUALFX_BOARDWAKEFX_CLASSES_H
