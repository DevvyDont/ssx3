#ifndef INCLUDE_AI_COMPUTER_CLASSES_H
#define INCLUDE_AI_COMPUTER_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// The computer rider's handler: a composite of 31 rider behaviour parts that each derive virtually from
// cRider (the same model as seg/seg_1218, which destroys it). Constructing it is compiler-generated g++ 2.95
// code: the cRider base and the vbase pointers when in charge, every part in order (the out-of-line part
// constructors are called, the others are inlined), then the vtable fix-ups for the virtual base (stack
// copies of the cRider-in-part vtables with the this-delta adjusted) and the handler's own fields.
class cRiderVbA0_cmp {
public:
    virtual ~cRiderVbA0_cmp();
    virtual void a01(); virtual void a02(); virtual void a03(); virtual void a04(); virtual void a05();
    virtual void a06(); virtual void a07(); virtual void a08(); virtual void a09(); virtual void a10();
    virtual void a11(); virtual void a12(); virtual void a13(); virtual void a14(); virtual void a15();
    virtual void a16(); virtual void a17(); virtual void a18(); virtual void a19();
};

class cRiderVbA_cmp : public cRiderVbA0_cmp { public: char padA[0xC]; };

class cRiderVbB0_cmp { public: virtual void b01(); };

class cRiderVbB_cmp : public cRiderVbB0_cmp { public: char padB[0x14]; };

class cRiderVbC0_cmp {
public:
    virtual ~cRiderVbC0_cmp();
    virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06();
};

class cRiderVbC_cmp : public cRiderVbC0_cmp { public: char padC[0x10]; };

class cRiderVbBC_cmp : public cRiderVbB_cmp, public cRiderVbC_cmp { public: };

class cRiderVbPad_cmp { public: char pad[0x6C0]; } __attribute__((aligned(16)));

class cRiderVb_cmp : public cRiderVbPad_cmp, public cRiderVbA_cmp, public cRiderVbBC_cmp {
public:
    char padD[0x77C - 0x6FC];
    void* mRiderHandler;            // 0x77C
    cRiderVb_cmp();                 // cRider_cRider
    virtual ~cRiderVb_cmp();
};

extern t4Vector D_004FF160;

struct cCompHead00_cmp { char pad[0x18]; };

class cCompPart00_cmp : public cCompHead00_cmp, virtual public cRiderVb_cmp {
public:
    char body[0x4];
    cCompPart00_cmp();              // func_0013C650
};

struct cCompHead01_cmp { char pad[0x4]; };

class cCompPart01_cmp : public cCompHead01_cmp, virtual public cRiderVb_cmp { public: char body[0x8]; };

struct cCompHead02_cmp { char pad[0x40]; };

class cCompPart02_cmp : public cCompHead02_cmp, virtual public cRiderVb_cmp {
public:
    char body[0x2C];
    cCompPart02_cmp();              // func_00136980
    virtual ~cCompPart02_cmp();
};

class cCompPart03_cmp : virtual public cRiderVb_cmp { public: char body[0xC]; };

// A handle at 0x24 of part 4's leading base: invalid (~0) when constructed, and reset to that by the part.
struct cCompHandle_cmp {
    unsigned int id;
    cCompHandle_cmp() { id = 0xFFFFFFFF; }
};

struct cCompHead04_cmp {
    t4Vector v;                     // 0x00
    char pad10[0x8];
    int f18;                        // 0x18
    char pad1C[0x8];
    cCompHandle_cmp h;              // 0x24
    char pad28[0x28];
};

class cCompPart04_cmp : public cCompHead04_cmp, virtual public cRiderVb_cmp {
public:
    char body[0xC];
    cCompPart04_cmp()
    {
        h = cCompHandle_cmp();
        f18 = 0;
        v = D_004FF160;
    }
};

struct cCompHead05_cmp { char pad[0xA0]; };

class cCompPart05_cmp : public cCompHead05_cmp, virtual public cRiderVb_cmp {
public:
    char body[0xC];
    cCompPart05_cmp();              // func_00141480
};

class cCompPart06_cmp : virtual public cRiderVb_cmp { public: char body[0xC]; };

struct cCompHead07_cmp { char pad[0x14]; };

class cCompPart07_cmp : public cCompHead07_cmp, virtual public cRiderVb_cmp { public: char body[0x8]; };

class cCompPart08_cmp : virtual public cRiderVb_cmp { public: char body[0xC]; };

class cCompPart09_cmp : virtual public cRiderVb_cmp { public: char body[0xC]; };

struct cCompHead10_cmp { char pad[0x14]; };

class cCompPart10_cmp : public cCompHead10_cmp, virtual public cRiderVb_cmp { public: char body[0x8]; };

struct cCompHead11_cmp { char pad[0x58]; };

class cCompPart11_cmp : public cCompHead11_cmp, virtual public cRiderVb_cmp { public: char body[0x4]; };

struct cCompHead12_cmp { int f00; char pad[0x10]; };

class cCompPart12_cmp : public cCompHead12_cmp, virtual public cRiderVb_cmp {
public:
    char body[0x8];
    cCompPart12_cmp() { f00 = 0; }
};

struct cCompHead13_cmp { char pad[0x8]; };

class cCompPart13_cmp : public cCompHead13_cmp, virtual public cRiderVb_cmp { public: char body[0x4]; };

struct cCompHead14_cmp { char pad[0x80]; };

class cCompPart14_cmp : public cCompHead14_cmp, virtual public cRiderVb_cmp {
public:
    char body[0xC];
    cCompPart14_cmp();              // func_00142388
};

struct cCompHead15_cmp { char pad[0x8]; };

class cCompPart15_cmp : public cCompHead15_cmp, virtual public cRiderVb_cmp { public: char body[0x4]; };

struct cCompHead16_cmp { char pad[0x4]; };

class cCompPart16_cmp : public cCompHead16_cmp, virtual public cRiderVb_cmp { public: char body[0x8]; };

struct cCompHead17_cmp { char pad[0x14]; };

class cCompPart17_cmp : public cCompHead17_cmp, virtual public cRiderVb_cmp { public: char body[0x8]; };

struct cCompHead18_cmp { char pad[0x8]; };

class cCompPart18_cmp : public cCompHead18_cmp, virtual public cRiderVb_cmp { public: char body[0x4]; };

class cCompPart19_cmp : virtual public cRiderVb_cmp { public: char body[0xC]; };

class cCompPart20_cmp : virtual public cRiderVb_cmp {
public:
    char body[0xBC];
    cCompPart20_cmp();              // cBoardWakeFX_cBoardWakeFX
    virtual ~cCompPart20_cmp();
};

class cCompPart21_cmp : virtual public cRiderVb_cmp {
public:
    char body[0xAC];
    cCompPart21_cmp();              // cBoardSparkFX_cBoardSparkFX
    virtual ~cCompPart21_cmp();
};

class cCompPart22_cmp : virtual public cRiderVb_cmp {
public:
    char body[0xEC];
    cCompPart22_cmp();              // cBoardTrailFX_initialize
    virtual ~cCompPart22_cmp();
};

class cCompPart23_cmp : virtual public cRiderVb_cmp {
public:
    char body[0x3AC];
    cCompPart23_cmp();              // cSpeedBoostFX_cSpeedBoostFX
    virtual ~cCompPart23_cmp();
};

class cCompPart24_cmp : virtual public cRiderVb_cmp {
public:
    char body[0x10C];
    cCompPart24_cmp();              // func_002EAAE0
    virtual ~cCompPart24_cmp();
};

class cCompPart25_cmp : virtual public cRiderVb_cmp {
public:
    char body[0x1C];
    cCompPart25_cmp();              // cAirStreamerFX_cAirStreamerFX
    virtual ~cCompPart25_cmp();
};

class cCompPart26_cmp : virtual public cRiderVb_cmp {
public:
    char body[0xC];
    cCompPart26_cmp();              // cAngerFX_cAngerFX
    virtual ~cCompPart26_cmp();
};

class cCompPart27_cmp : virtual public cRiderVb_cmp {
public:
    char body[0x3C];
    cCompPart27_cmp();              // func_002E3680
    virtual ~cCompPart27_cmp();
};

class cCompPart28_cmp : virtual public cRiderVb_cmp {
public:
    char body[0x12C];
    cCompPart28_cmp();              // func_002DE4A8
    virtual ~cCompPart28_cmp();
};

class cCompPart29_cmp : virtual public cRiderVb_cmp {
public:
    char body[0xAC];
    cCompPart29_cmp();              // func_002F0E88
    virtual ~cCompPart29_cmp();
};

class cCompPart30_cmp : virtual public cRiderVb_cmp {
public:
    cCompPart30_cmp();              // func_002F6238
    virtual ~cCompPart30_cmp();
};

/// The handler's two five-entry tables; their element constructor does nothing (the two empty array-construction
// loops before the fix-ups: retail deleted the first and kept the second, see tools/patch_vecmark.py).
struct cCompSlot_cmp { cCompSlot_cmp() {} int w[4]; };

struct cCompSlots_cmp { cCompSlot_cmp slotA[5]; cCompSlot_cmp slotB[5]; };

class cComp_cmp : public cCompPart00_cmp, public cCompPart01_cmp, public cCompPart02_cmp, public cCompPart03_cmp, public cCompPart04_cmp, public cCompPart05_cmp, public cCompPart06_cmp, public cCompPart07_cmp, public cCompPart08_cmp, public cCompPart09_cmp, public cCompPart10_cmp, public cCompPart11_cmp, public cCompPart12_cmp, public cCompPart13_cmp, public cCompPart14_cmp, public cCompPart15_cmp, public cCompPart16_cmp, public cCompPart17_cmp, public cCompPart18_cmp, public cCompPart19_cmp, public cCompPart20_cmp, public cCompPart21_cmp, public cCompPart22_cmp, public cCompPart23_cmp, public cCompPart24_cmp, public cCompPart25_cmp, public cCompPart26_cmp, public cCompPart27_cmp, public cCompPart28_cmp, public cCompPart29_cmp, public cCompPart30_cmp, public cCompSlots_cmp {
public:
    char tail[0x10];
    int fDE0;
    int fDE4;
    cComp_cmp();
    virtual void v01() = 0;
    virtual void v02();
    virtual void v03();
};

#endif // INCLUDE_AI_COMPUTER_CLASSES_H
