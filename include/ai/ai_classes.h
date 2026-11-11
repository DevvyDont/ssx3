#ifndef INCLUDE_AI_AI_CLASSES_H
#define INCLUDE_AI_AI_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_ai {
public:
    virtual ~cRiderVbA0_ai();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_ai : public cRiderVbA0_ai { public: char padA[0xC]; };

class cRiderVbB0_ai { public: virtual void b01(); };

class cRiderVbB_ai : public cRiderVbB0_ai { public: char padB[0x14]; };

class cRiderVbC0_ai {
public:
    virtual ~cRiderVbC0_ai();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_ai : public cRiderVbC0_ai { public: char padC[0x10]; };

class cRiderVbBC_ai : public cRiderVbB_ai, public cRiderVbC_ai { public: };

class cRiderVbPad_ai { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_ai : public cRiderVbPad_ai, public cRiderVbA_ai, public cRiderVbBC_ai {
public:
    virtual ~cRiderVb_ai();
};

class cVbMember00136AD0 {
public:
    ~cVbMember00136AD0();
};

struct cVbHead00136AD0 {
    char pad[0x40];
};

class cVbDtor00136AD0 : public cVbHead00136AD0, virtual public cRiderVb_ai {
public:
    cVbMember00136AD0* m44;     // 0x44
    char body[0x28];
    virtual ~cVbDtor00136AD0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_AI_AI_CLASSES_H
