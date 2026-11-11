#ifndef INCLUDE_AI_RIDER_CLASSES_H
#define INCLUDE_AI_RIDER_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Classes here derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable fix-ups
// for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted when
// not most-derived). Only the cRider layout that code depends on is modelled (see
// ai/control/handplantcontrol.cpp).
class cRiderVbA0_rider {
public:
    virtual ~cRiderVbA0_rider();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_rider : public cRiderVbA0_rider { public: char padA[0xC]; };

class cRiderVbB0_rider { public: virtual void b01(); };

class cRiderVbB_rider : public cRiderVbB0_rider { public: char padB[0x14]; };

class cRiderVbC0_rider {
public:
    virtual ~cRiderVbC0_rider();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_rider : public cRiderVbC0_rider { public: char padC[0x10]; };

class cRiderVbBC_rider : public cRiderVbB_rider, public cRiderVbC_rider { public: };

class cRiderVbPad_rider { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_rider : public cRiderVbPad_rider, public cRiderVbA_rider, public cRiderVbBC_rider {
public:
    virtual ~cRiderVb_rider();
};

void operator_delete(int* p);

extern void* D_004A28A8;

extern "C" void func_002700D8(void* self, void* p);

// Data that sits in front of a part's virtual-base pointer.
template <int N> struct sVbHead_rider { char head[N]; };

// The rider parts with an implicit (inline) destructor.
class cVbPart000_rider : public sVbHead_rider<0x18>, virtual public cRiderVb_rider { };

class cVbPart020_rider : public sVbHead_rider<0x4>, virtual public cRiderVb_rider { };

class cVbPart0A0_rider : virtual public cRiderVb_rider { };

class cVbPart0B0_rider : public sVbHead_rider<0x50>, virtual public cRiderVb_rider { };

class cVbPart110_rider : public sVbHead_rider<0xA0>, virtual public cRiderVb_rider { };

class cVbPart1C0_rider : virtual public cRiderVb_rider { };

class cVbPart1D0_rider : public sVbHead_rider<0x14>, virtual public cRiderVb_rider { };

class cVbPart1F0_rider : virtual public cRiderVb_rider { };

class cVbPart200_rider : virtual public cRiderVb_rider { };

class cVbPart210_rider : public sVbHead_rider<0x14>, virtual public cRiderVb_rider { };

class cVbPart230_rider : public sVbHead_rider<0x58>, virtual public cRiderVb_rider { };

class cVbPart290_rider : public sVbHead_rider<0x14>, virtual public cRiderVb_rider { };

class cVbPart2B0_rider : public sVbHead_rider<0x8>, virtual public cRiderVb_rider { };

class cVbPart2C0_rider : public sVbHead_rider<0x80>, virtual public cRiderVb_rider { };

class cVbPart350_rider : public sVbHead_rider<0x8>, virtual public cRiderVb_rider { };

class cVbPart360_rider : public sVbHead_rider<0x4>, virtual public cRiderVb_rider { };

class cVbPart370_rider : public sVbHead_rider<0x14>, virtual public cRiderVb_rider { };

class cVbPart390_rider : public sVbHead_rider<0x8>, virtual public cRiderVb_rider { };

class cVbPart3A0_rider : virtual public cRiderVb_rider { };

// The rider parts with an out-of-line destructor (defined in their own units).
#define VB_PART_OOL(name, size) \
    class name : virtual public cRiderVb_rider { public: char body[size]; virtual ~name(); };

class cVbHead00136AD0 { public: char pad[0x40]; };

class cVbDtor00136AD0 : public cVbHead00136AD0, virtual public cRiderVb_rider {
public:
    void* m44;
    char body[0x28];
    virtual ~cVbDtor00136AD0();
};

VB_PART_OOL(cVbDtor002DCDA0, 0xB8)
VB_PART_OOL(cVbDtor002DA8E8, 0xA8)
VB_PART_OOL(cVbDtor002E83F0, 0xE8)
VB_PART_OOL(cVbDtor002E64C0, 0x3AC)
VB_PART_OOL(cVbDtor002EAC60, 0x10C)
VB_PART_OOL(cVbDtor002EF530, 0x1C)
VB_PART_OOL(cVbDtor002D4A70, 0x4)
VB_PART_OOL(cVbDtor002E37D0, 0x3C)
VB_PART_OOL(cVbDtor002DF1C8, 0x12C)
VB_PART_OOL(cVbDtor002F0FE8, 0xAC)
class cVbDtor002F6388 : virtual public cRiderVb_rider { public: virtual ~cVbDtor002F6388(); };

class cRiderBase_125EB8
    : public cVbPart000_rider, public cVbPart020_rider, public cVbDtor00136AD0, public cVbPart0A0_rider,
      public cVbPart0B0_rider, public cVbPart110_rider, public cVbPart1C0_rider, public cVbPart1D0_rider,
      public cVbPart1F0_rider, public cVbPart200_rider, public cVbPart210_rider, public cVbPart230_rider,
      public cVbPart290_rider, public cVbPart2B0_rider, public cVbPart2C0_rider, public cVbPart350_rider,
      public cVbPart360_rider, public cVbPart370_rider, public cVbPart390_rider, public cVbPart3A0_rider,
      public cVbDtor002DCDA0, public cVbDtor002DA8E8, public cVbDtor002E83F0, public cVbDtor002E64C0,
      public cVbDtor002EAC60, public cVbDtor002EF530, public cVbDtor002D4A70, public cVbDtor002E37D0,
      public cVbDtor002DF1C8, public cVbDtor002F0FE8, public cVbDtor002F6388 {
public:
    char fields[0xC4];
    virtual ~cRiderBase_125EB8() {}
    virtual void v1() = 0;
    virtual void* v2();
    virtual void* v3();
};

class cRider_125EB8 : public cRiderBase_125EB8 {
public:
    int mDF0;
    int mDF4;
    void* mDF8;
    float mDFC;
    float mE00;
    virtual ~cRider_125EB8();
    virtual void v1();
    virtual void* v2();
    virtual void* v3();
    virtual void a15(); virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_AI_RIDER_CLASSES_H
