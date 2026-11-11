#ifndef INCLUDE_SEG_SEG_1218_CLASSES_H
#define INCLUDE_SEG_SEG_1218_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

void operator_delete(int* p);

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_s12 {
public:
    virtual ~cRiderVbA0_s12();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_s12 : public cRiderVbA0_s12 { public: char padA[0xC]; };

class cRiderVbB0_s12 { public: virtual void b01(); };

class cRiderVbB_s12 : public cRiderVbB0_s12 { public: char padB[0x14]; };

class cRiderVbC0_s12 {
public:
    virtual ~cRiderVbC0_s12();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_s12 : public cRiderVbC0_s12 { public: char padC[0x10]; };

class cRiderVbBC_s12 : public cRiderVbB_s12, public cRiderVbC_s12 { public: };

class cRiderVbPad_s12 { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_s12 : public cRiderVbPad_s12, public cRiderVbA_s12, public cRiderVbBC_s12 {
public:
    cRiderVb_s12();
    virtual ~cRiderVb_s12();
};

// PORT: the rider class below derives from a composite of 31 parts that each derive virtually from cRider.
// Each part: an optional leading base (Head), the vbase pointer, then its own data; the parts named by a
// func_ address have out-of-line destructors (the visualfx/ai classes at those addresses), the others are
// destroyed inline. The composite's vptr sits at 0xDE8.
struct cCompHead00_s12 { char pad[0x18]; };

class cCompPart00_s12 : public cCompHead00_s12, virtual public cRiderVb_s12 { public: char body[0x4]; };

struct cCompHead01_s12 { char pad[0x4]; };

class cCompPart01_s12 : public cCompHead01_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

struct cCompHead02_s12 { char pad[0x40]; };

class cCompPart02_s12 : public cCompHead02_s12, virtual public cRiderVb_s12 {
public:
    char body[0x2C];
    virtual ~cCompPart02_s12();             // func_00136AD0
};

class cCompPart03_s12 : virtual public cRiderVb_s12 { public: char body[0xC]; };

struct cCompHead04_s12 { char pad[0x50]; };

class cCompPart04_s12 : public cCompHead04_s12, virtual public cRiderVb_s12 { public: char body[0xC]; };

struct cCompHead05_s12 { char pad[0xA0]; };

class cCompPart05_s12 : public cCompHead05_s12, virtual public cRiderVb_s12 { public: char body[0xC]; };

class cCompPart06_s12 : virtual public cRiderVb_s12 { public: char body[0xC]; };

struct cCompHead07_s12 { char pad[0x14]; };

class cCompPart07_s12 : public cCompHead07_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

class cCompPart08_s12 : virtual public cRiderVb_s12 { public: char body[0xC]; };

class cCompPart09_s12 : virtual public cRiderVb_s12 { public: char body[0xC]; };

struct cCompHead10_s12 { char pad[0x14]; };

class cCompPart10_s12 : public cCompHead10_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

struct cCompHead11_s12 { char pad[0x58]; };

class cCompPart11_s12 : public cCompHead11_s12, virtual public cRiderVb_s12 { public: char body[0x4]; };

struct cCompHead12_s12 { char pad[0x14]; };

class cCompPart12_s12 : public cCompHead12_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

struct cCompHead13_s12 { char pad[0x8]; };

class cCompPart13_s12 : public cCompHead13_s12, virtual public cRiderVb_s12 { public: char body[0x4]; };

struct cCompHead14_s12 { char pad[0x80]; };

class cCompPart14_s12 : public cCompHead14_s12, virtual public cRiderVb_s12 { public: char body[0xC]; };

struct cCompHead15_s12 { char pad[0x8]; };

class cCompPart15_s12 : public cCompHead15_s12, virtual public cRiderVb_s12 { public: char body[0x4]; };

struct cCompHead16_s12 { char pad[0x4]; };

class cCompPart16_s12 : public cCompHead16_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

struct cCompHead17_s12 { char pad[0x14]; };

class cCompPart17_s12 : public cCompHead17_s12, virtual public cRiderVb_s12 { public: char body[0x8]; };

struct cCompHead18_s12 { char pad[0x8]; };

class cCompPart18_s12 : public cCompHead18_s12, virtual public cRiderVb_s12 { public: char body[0x4]; };

class cCompPart19_s12 : virtual public cRiderVb_s12 { public: char body[0xC]; };

class cCompPart20_s12 : virtual public cRiderVb_s12 {
public:
    char body[0xBC];
    virtual ~cCompPart20_s12();             // func_002DCDA0
};

class cCompPart21_s12 : virtual public cRiderVb_s12 {
public:
    char body[0xAC];
    virtual ~cCompPart21_s12();             // func_002DA8E8
};

class cCompPart22_s12 : virtual public cRiderVb_s12 {
public:
    char body[0xEC];
    virtual ~cCompPart22_s12();             // func_002E83F0
};

class cCompPart23_s12 : virtual public cRiderVb_s12 {
public:
    char body[0x3AC];
    virtual ~cCompPart23_s12();             // func_002E64C0
};

class cCompPart24_s12 : virtual public cRiderVb_s12 {
public:
    char body[0x10C];
    virtual ~cCompPart24_s12();             // func_002EAC60
};

class cCompPart25_s12 : virtual public cRiderVb_s12 {
public:
    char body[0x1C];
    virtual ~cCompPart25_s12();             // func_002EF530
};

class cCompPart26_s12 : virtual public cRiderVb_s12 {
public:
    char body[0xC];
    virtual ~cCompPart26_s12();             // func_002D4A70
};

class cCompPart27_s12 : virtual public cRiderVb_s12 {
public:
    char body[0x3C];
    virtual ~cCompPart27_s12();             // func_002E37D0
};

class cCompPart28_s12 : virtual public cRiderVb_s12 {
public:
    char body[0x12C];
    virtual ~cCompPart28_s12();             // func_002DF1C8
};

class cCompPart29_s12 : virtual public cRiderVb_s12 {
public:
    char body[0xAC];
    virtual ~cCompPart29_s12();             // func_002F0FE8
};

class cCompPart30_s12 : virtual public cRiderVb_s12 {
public:
    virtual ~cCompPart30_s12();             // func_002F6388
};

class cComp_s12 : public cCompPart00_s12, public cCompPart01_s12, public cCompPart02_s12, public cCompPart03_s12, public cCompPart04_s12, public cCompPart05_s12, public cCompPart06_s12, public cCompPart07_s12, public cCompPart08_s12, public cCompPart09_s12, public cCompPart10_s12, public cCompPart11_s12, public cCompPart12_s12, public cCompPart13_s12, public cCompPart14_s12, public cCompPart15_s12, public cCompPart16_s12, public cCompPart17_s12, public cCompPart18_s12, public cCompPart19_s12, public cCompPart20_s12, public cCompPart21_s12, public cCompPart22_s12, public cCompPart23_s12, public cCompPart24_s12, public cCompPart25_s12, public cCompPart26_s12, public cCompPart27_s12, public cCompPart28_s12, public cCompPart29_s12, public cCompPart30_s12 {
public:
    char tail[0xC4];
    // PORT: func_0010FCD8 constructs this part (fields up to 0xDE8 and the vptr at 0xDE8)
    cComp_s12();
    ~cComp_s12() {}
    // Own vtable (D_00458710): a pure slot, then func_00111AC0 and func_00111D98.
    virtual void v01() = 0;
    virtual void v02();
    virtual void v03();
};

class cRiderE_108C80 : public cComp_s12 {
public:
    char tail[0x160];
    cRiderE_108C80(int flag);
    virtual ~cRiderE_108C80();
    virtual void a19();
    // Overrides of the composite's slots (D_004585F0): func_0010A768, func_0010A898, func_0010A8E8.
    virtual void v01();
    virtual void v02();
    virtual void v03();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_SEG_SEG_1218_CLASSES_H
