#ifndef INCLUDE_AI_CONTROL_HANDPLANTCONTROL_CLASSES_H
#define INCLUDE_AI_CONTROL_HANDPLANTCONTROL_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// Controller classes derive virtually from cRider. g++ 2.95 then emits, in their destructors, the vtable
// fix-ups for the virtual base (stack copies of the cRider-in-D vtables with the dtor's this-delta adjusted
// when not most-derived). Only the cRider layout that matters to that code is modelled: polymorphic bases at
// +0x6C0 (primary, 21 virtuals), +0x6D0 and +0x6E8 (7 virtuals), each with a virtual dtor except +0x6D0.
class cRiderVbA0_hpc {
public:
    virtual ~cRiderVbA0_hpc();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_hpc : public cRiderVbA0_hpc { public: char padA[0xC]; };

class cRiderVbB0_hpc { public: virtual void b01(); };

class cRiderVbB_hpc : public cRiderVbB0_hpc { public: char padB[0x14]; };

class cRiderVbC0_hpc {
public:
    virtual ~cRiderVbC0_hpc();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_hpc : public cRiderVbC0_hpc { public: char padC[0x10]; };

class cRiderVbBC_hpc : public cRiderVbB_hpc, public cRiderVbC_hpc { public: };

class cRiderVbPad_hpc { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_hpc : public cRiderVbPad_hpc, public cRiderVbA_hpc, public cRiderVbBC_hpc {
public:
    virtual ~cRiderVb_hpc();
};

void operator_delete(int* p);

struct cVbHead00140D80 {
    char pad[0x18];
};

class cVbDtor00140D80 : public cVbHead00140D80, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00140D80();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00140EE0 {
    char pad[0x4];
};

class cVbDtor00140EE0 : public cVbHead00140EE0, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00140EE0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141040 {
    char pad[0x50];
};

class cVbDtor00141040 : public cVbHead00141040, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141040();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cHandPlant11C0 : virtual public cRiderVb_hpc {
public:
    virtual ~cHandPlant11C0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141320 {
    char pad[0xA0];
};

class cVbDtor00141320 : public cVbHead00141320, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141320();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbDtor001415C8 : virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor001415C8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141728 {
    char pad[0x14];
};

class cVbDtor00141728 : public cVbHead00141728, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141728();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbDtor00141888 : virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141888();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbDtor001419E8 : virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor001419E8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141B48 {
    char pad[0x14];
};

class cVbDtor00141B48 : public cVbHead00141B48, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141B48();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141CA8 {
    char pad[0x58];
};

class cVbDtor00141CA8 : public cVbHead00141CA8, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141CA8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141E08 {
    char pad[0x14];
};

class cVbDtor00141E08 : public cVbHead00141E08, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141E08();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00141F68 {
    char pad[0x8];
};

class cVbDtor00141F68 : public cVbHead00141F68, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00141F68();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead001420C8 {
    char pad[0x8];
};

class cVbDtor001420C8 : public cVbHead001420C8, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor001420C8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00142228 {
    char pad[0x80];
};

class cVbDtor00142228 : public cVbHead00142228, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00142228();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead001424D0 {
    char pad[0x4];
};

class cVbDtor001424D0 : public cVbHead001424D0, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor001424D0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead00142630 {
    char pad[0x8];
};

class cVbDtor00142630 : public cVbHead00142630, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00142630();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

class cVbDtor00142790 : virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor00142790();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cVbHead001428F0 {
    char pad[0x14];
};

class cVbDtor001428F0 : public cVbHead001428F0, virtual public cRiderVb_hpc {
public:
    virtual ~cVbDtor001428F0();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

// The controller classes defined above are this class's bases; their destructors were inline in the original
// (retail expands each one here), so these twins repeat the same layouts with in-class destructors. The matched
// out-of-line copies above keep their own definitions. The retail TU was built without RTTI: every class shares
// cRider's +0x6D0 vtable (D_00459B90), which g++ 2.95 only does with -fno-rtti.
#define VB_INLINE_BASE(addr, Head)                                                                   \
    class cVbIn##addr : public Head, virtual public cRiderVb_hpc {                                   \
    public:                                                                                          \
        virtual ~cVbIn##addr() {}                                                                    \
    };

#define VB_INLINE_BASE0(addr)                                                                        \
    class cVbIn##addr : virtual public cRiderVb_hpc {                                                \
    public:                                                                                          \
        virtual ~cVbIn##addr() {}                                                                    \
    };

VB_INLINE_BASE(00140D80, cVbHead00140D80)
VB_INLINE_BASE(00140EE0, cVbHead00140EE0)
VB_INLINE_BASE0(001411C0)
VB_INLINE_BASE(00141040, cVbHead00141040)
VB_INLINE_BASE(00141320, cVbHead00141320)
VB_INLINE_BASE0(001415C8)
VB_INLINE_BASE(00141728, cVbHead00141728)
VB_INLINE_BASE0(00141888)
VB_INLINE_BASE0(001419E8)
VB_INLINE_BASE(00141B48, cVbHead00141B48)
VB_INLINE_BASE(00141CA8, cVbHead00141CA8)
VB_INLINE_BASE(00141E08, cVbHead00141E08)
VB_INLINE_BASE(00141F68, cVbHead00141F68)
VB_INLINE_BASE(00142228, cVbHead00142228)
VB_INLINE_BASE(00142630, cVbHead00142630)
VB_INLINE_BASE(001424D0, cVbHead001424D0)
VB_INLINE_BASE(001428F0, cVbHead001428F0)
VB_INLINE_BASE(001420C8, cVbHead001420C8)
VB_INLINE_BASE0(00142790)

// Bases whose destructors live in other units (out of line there).
#define VB_EXTERN_BASE(addr, n)                                                                      \
    class cVbDtor##addr : virtual public cRiderVb_hpc {                                              \
    public:                                                                                          \
        char body[n];                                                                                \
        virtual ~cVbDtor##addr();                                                                    \
    };

VB_EXTERN_BASE(00136AD0, 0x6C)
VB_EXTERN_BASE(002DCDA0, 0xBC)
VB_EXTERN_BASE(002DA8E8, 0xAC)
VB_EXTERN_BASE(002E83F0, 0xEC)
VB_EXTERN_BASE(002E64C0, 0x3AC)
VB_EXTERN_BASE(002EAC60, 0x10C)
VB_EXTERN_BASE(002EF530, 0x1C)
VB_EXTERN_BASE(002D4A70, 0x4)
VB_EXTERN_BASE(002E37D0, 0x3C)
VB_EXTERN_BASE(002DF1C8, 0x12C)
VB_EXTERN_BASE(002F0FE8, 0xAC)
VB_EXTERN_BASE(002F6388, 0x0C)

class cVbDtor00142A50
    : public cVbIn00140D80, public cVbIn00140EE0, public cVbDtor00136AD0, public cVbIn001411C0,
      public cVbIn00141040, public cVbIn00141320, public cVbIn001415C8, public cVbIn00141728,
      public cVbIn00141888, public cVbIn001419E8, public cVbIn00141B48, public cVbIn00141CA8,
      public cVbIn00141E08, public cVbIn00141F68, public cVbIn00142228, public cVbIn00142630,
      public cVbIn001424D0, public cVbIn001428F0, public cVbIn001420C8, public cVbIn00142790,
      public cVbDtor002DCDA0, public cVbDtor002DA8E8, public cVbDtor002E83F0, public cVbDtor002E64C0,
      public cVbDtor002EAC60, public cVbDtor002EF530, public cVbDtor002D4A70, public cVbDtor002E37D0,
      public cVbDtor002DF1C8, public cVbDtor002F0FE8, public cVbDtor002F6388 {
public:
    char pad[0xB8];
    virtual ~cVbDtor00142A50();
    // Own vtable D_00458710: a pure slot, then func_00111AC0 and func_00111D98.
    virtual void vf00() = 0;
    virtual void vf01(char* s);
    virtual void vf02(char* s);
    static void operator delete(void* p) { operator_delete((int*)p); }
};

struct cHpHead_1446F8 { char pad[0x18]; };

class cHpB_1446F8 : public cHpHead_1446F8, virtual public cRiderVb_hpc {
public:
    char body[0xDCC];
    // PORT: overrides of the last five cRider virtuals (their vtable entries get this-adjusted copies)
    virtual void a15(); virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
    ~cHpB_1446F8();
};

class cHpD1_1446F8 : public cHpB_1446F8 {
public:
    virtual void d01();
};

class cHpD_1446F8 : public cHpD1_1446F8 {
public:
    char tail[0x20];
    virtual ~cHpD_1446F8();
    static void operator delete(void* p) { operator_delete((int*)p); }
};

#endif // INCLUDE_AI_CONTROL_HANDPLANTCONTROL_CLASSES_H
