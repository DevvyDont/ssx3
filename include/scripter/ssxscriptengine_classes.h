#ifndef INCLUDE_SCRIPTER_SSXSCRIPTENGINE_CLASSES_H
#define INCLUDE_SCRIPTER_SSXSCRIPTENGINE_CLASSES_H
// Prototype classes for this unit's C++ functions (from peak3project's fork, github.com/peak3project/ssx3).
// `#pragma interface`: g++ only references these classes' vtables here, never emits them, so the units'
// `_vt$...` equates can bind them to the retail data (tools/vt_header.py). Codegen is unaffected.
#pragma interface

// A real C++ ctor: g++ stores the secondary vptr (+0x10) before the primary (+0xC), with the target's registers.
// PORT: the ctors are bound to their retail symbol names by asm labels.
extern "C" void func_00281A50(void* self);

class cScriptObject_1018 {
public:
    char d[0xC];
    virtual void so01();
    cScriptObject_1018(int) __asm__("cScriptObject_cScriptObject__FPvi");
};

class cScriptEntityI_1018 {
public:
    virtual void se01();
};

class cScriptEntity_1018 : public cScriptEntityI_1018 {
public:
    char d[8];
    cScriptEntity_1018() __asm__("cScriptEntity_cScriptEntity__FPv");
};

class cScriptSlot_1018 {
public:
    char pad[0x10];
    void* owner;
    cScriptSlot_1018() __asm__("func_00283088");
    void setOwner(void* o) { owner = o; }
};

class cScriptEngine_1018 : public cScriptObject_1018, public cScriptEntity_1018 {
public:
    cScriptSlot_1018 slots[9];
    int f[2];
    int fD8;
    char padDC[0x1C];
    int fF8;
    cScriptEngine_1018() __asm__("func_00281018");
    virtual void so01();
    virtual void se01();
};

#endif // INCLUDE_SCRIPTER_SSXSCRIPTENGINE_CLASSES_H
