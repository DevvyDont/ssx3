#include "vtable.h"
#include "common.h"
#include "luno/lunotable.h"
#include "luno/lunofunc.h"
#include "replay/bxreplayframe.h"

// A script value: {word0, word1, type}. Type 4 is a table, word0 -> cLunoTable.
struct cLunoValue {
    int word0;
    int word1;
    int type;
};

// Table chain node: 0x20 bytes, vtable pointer at +0x1C (gcc 2.95 layout).
struct cLunoVtEntry {
    short delta;
    short index;
    void (*func)(void*, int);
};

struct cLunoTableEntry {
    char pad_0x00[0x1C];
    cLunoVtEntry* vtable;
};


// Table reference: word0 of a type-4 value.
struct cLunoTableRef {
    luno_cLunoTableData* table;
};

// The cookie g++ 2.95's new[] keeps in front of an array whose element type has a destructor (16 bytes, the
// element count first); delete[] reads the count from it and frees from its start. A table's Entries array has one.
struct tArrayNewCookie {
    int count;
    int pad[3];
};

void operator_delete(int* ptr);
void cMemMan_free(void* ptr);
extern "C" void luno_cLunoTable_destroyTable(cLunoTableRef* ref);
extern "C" void luno_cLunoTable__cLunoTable(cLunoTableRef* ref, int flags);

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVM_execute);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// luno::cLunoVM::execute: the Luno script interpreter (lunovm.cpp), with the inline classes of lunovar.h,
// lunofunc.h and lunovm.h. Out-of-line members are bound to their retail symbols.

// eLunoVarType: luno/lunovar.h.
class cLunoVar;
extern cLunoVar gLunoNil __asm__("luno_gLunoNil");

// luno::cLunoFunc: compiled code block and function index (stored in a value's data area; trivial destructor).
struct cLunoFunc {
    const char* Code;
    int Index;
    void init(const char* code, int index) __asm__("luno_cLunoFunc_init__FPvii");
    ~cLunoFunc() {}
};

// luno::cLunoTable: reference-counted table handle.
class cLunoTab {
public:
    luno_cLunoTableData* m_pTable;
    cLunoTab& operator=(const cLunoTab& o) __asm__("luno_cLunoTable_operatorE");
    void set(const cLunoVar& key, const cLunoVar& val) __asm__("luno_cLunoTable_set");
    cLunoVar& get(const cLunoVar& key) __asm__("luno_cLunoTable_get1");
    ~cLunoTab();
};
__asm__("_$_8cLunoTab = luno_cLunoTable__cLunoTable");

void cMemMan_free(void* ptr);

class cLunoVar {
public:
    union {
        int Int;
        float Float;
        unsigned int Index;
        int Raw[2];
    } Data;
    eLunoVarType Type;

    cLunoVar()
    {
        Type = eLunoVar_Nil;
        Data.Int = 0;
    }
    cLunoVar(int i)
    {
        Type = eLunoVar_Int;
        castInt() = i;
    }
    cLunoVar(float f)
    {
        Type = eLunoVar_Float;
        castFloat() = f;
    }
    cLunoVar(eLunoVarType t, int i)
    {
        Type = t;
        castInt() = i;
    }
    cLunoVar(eLunoVarType t, float f)
    {
        Type = t;
        castFloat() = f;
    }
    cLunoVar(eLunoVarType t, unsigned int idx)
    {
        Type = t;
        castIndex() = idx;
    }
    ~cLunoVar();

    int& castInt() const __asm__("luno_cLunoVar_castInt__FPv");
    float& castFloat() const __asm__("luno_cLunoVar_castFloat__FPv");
    unsigned int& castIndex() const __asm__("luno_cLunoVar_castIndex__FPv");
    cLunoTab& castTable()
    {
        return *(cLunoTab*)&Data;
    }
    cLunoFunc& castFunc()
    {
        return *(cLunoFunc*)&Data;
    }
    const cLunoTab& castTable() const { return *(const cLunoTab*)&Data; }
    const cLunoFunc& castFunc() const { return *(const cLunoFunc*)&Data; }

    void Clear()
    {
        if (Type == eLunoVar_Table) {
            castTable().~cLunoTab();
        } else if (Type == eLunoVar_Func) {
            castFunc().~cLunoFunc();
        }
        Type = eLunoVar_Nil;
        Data.Int = 0;
    }

    cLunoVar& operator=(const cLunoVar& o)
    {
        Clear();
        if (o.Type == eLunoVar_Table) {
            castTable() = o.castTable();
        } else if (o.Type == eLunoVar_Func) {
            castFunc() = o.castFunc();
        } else {
            Data.Int = o.Data.Int;
        }
        Type = o.Type;
        return *this;
    }
    cLunoVar& operator=(const int& i)
    {
        Clear();
        Data.Int = i;
        Type = eLunoVar_Int;
        return *this;
    }
    cLunoVar& operator=(const float& f)
    {
        Clear();
        Data.Float = f;
        Type = eLunoVar_Float;
        return *this;
    }
    cLunoVar& operator=(const unsigned int& idx)
    {
        Clear();
        Data.Index = idx;
        Type = eLunoVar_Index;
        return *this;
    }
    cLunoVar& operator=(const cLunoTab& t)
    {
        Clear();
        castTable() = t;
        Type = eLunoVar_Table;
        return *this;
    }
    cLunoVar& operator=(const cLunoFunc& f)
    {
        Clear();
        castFunc() = f;
        Type = eLunoVar_Func;
        return *this;
    }

    int isTrue() const
    {
        if (Type == eLunoVar_Nil) {
            return 0;
        }
        if (Type == eLunoVar_Int) {
            return Data.Int != 0;
        }
        if (Type == eLunoVar_Float) {
            return Data.Float != 0.0f;
        }
        if (Type == eLunoVar_Index) {
            return Data.Index != 0;
        }
        return 1;
    }
    int isFalse() const
    {
        if (Type == eLunoVar_Nil) {
            return 1;
        }
        if (Type == eLunoVar_Int) {
            return Data.Int == 0;
        }
        if (Type == eLunoVar_Float) {
            return Data.Float == 0.0f;
        }
        if (Type == eLunoVar_Index) {
            return Data.Index == 0;
        }
        return 0;
    }

    int operator==(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int == o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float == (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int == (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Int == o.Data.Int;
        }
        return 0;
    }
    int operator!=(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int != o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float != (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int != (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Int != o.Data.Int;
        }
        return 0;
    }
    int operator>=(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int >= o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float >= (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int >= (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Index >= o.Data.Index;
        }
        return 0;
    }
    int operator>(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int > o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float > (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int > (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Index > o.Data.Index;
        }
        return 0;
    }
    int operator<=(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int <= o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float <= (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int <= (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Index <= o.Data.Index;
        }
        return 0;
    }
    int operator<(const cLunoVar& o) const
    {
        if (o.Type == Type) {
            return Data.Int < o.Data.Int;
        }
        if (Type == eLunoVar_Float && o.Type == eLunoVar_Int) {
            return Data.Float < (float)o.Data.Int;
        }
        if (Type == eLunoVar_Int && o.Type == eLunoVar_Float) {
            return Data.Int < (int)o.Data.Float;
        }
        if (Type == eLunoVar_Index && o.Type == eLunoVar_Int) {
            return Data.Index < o.Data.Index;
        }
        return 0;
    }
    int operator||(const cLunoVar& o) const
    {
        if (o.Type == eLunoVar_Nil) {
            if (Type == eLunoVar_Nil) {
                return 0;
            }
            return Data.Int != 0;
        }
        if (Type == eLunoVar_Nil) {
            return 0;
        }
        return Data.Int != 0 || o.Data.Int != 0;
    }
    int operator&&(const cLunoVar& o) const
    {
        if (o.Type == eLunoVar_Nil) {
            return 0;
        }
        if (Type == eLunoVar_Nil) {
            return 0;
        }
        return Data.Int != 0 && o.Data.Int != 0;
    }

    cLunoVar operator!() const
    {
        if (Type == eLunoVar_Nil) {
            return gLunoNil;
        }
        if (Type == eLunoVar_Int) {
            return cLunoVar(Data.Int == 0);
        }
        if (Type == eLunoVar_Float) {
            return cLunoVar(Data.Float == 0.0f);
        }
        return gLunoNil;
    }
    cLunoVar operator-() const
    {
        if (Type == eLunoVar_Nil) {
            return gLunoNil;
        }
        if (Type == eLunoVar_Int) {
            return cLunoVar(-Data.Int);
        }
        if (Type == eLunoVar_Float) {
            return cLunoVar(-Data.Float);
        }
        return gLunoNil;
    }
    cLunoVar operator+(const cLunoVar& o) const __asm__("luno_cLunoVar_operatorPlus");
    cLunoVar& operator+=(const cLunoVar& o)
    {
        return *this = *this + o;
    }

    static void operator delete[](void* p) { cMemMan_free(p); }
};
__asm__("_$_8cLunoVar = luno_cLunoVar__cLunoVar");

// Binary arithmetic (operator+ is also emitted out of line as luno_cLunoVar_operatorPlus).
static inline cLunoVar lunoAdd(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        if (a.Type == eLunoVar_Float) {
            return cLunoVar(a.Type, a.Data.Float + b.Data.Float);
        }
        if (a.Type == eLunoVar_Int) {
            return cLunoVar(a.Type, a.Data.Int + b.Data.Int);
        }
        if (a.Type == eLunoVar_Index) {
            return cLunoVar(a.Type, a.Data.Index + b.Data.Index);
        }
        return gLunoNil;
    }
    if (a.Type == eLunoVar_Float && b.Type == eLunoVar_Int) {
        return cLunoVar(a.Type, a.Data.Float + (float)b.Data.Int);
    }
    if (a.Type == eLunoVar_Int && b.Type == eLunoVar_Float) {
        return cLunoVar(a.Type, a.Data.Int + (int)b.Data.Float);
    }
    return gLunoNil;
}

static inline cLunoVar lunoSub(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        if (a.Type == eLunoVar_Float) {
            return cLunoVar(a.Type, a.Data.Float - b.Data.Float);
        }
        if (a.Type == eLunoVar_Int) {
            return cLunoVar(a.Type, a.Data.Int - b.Data.Int);
        }
        return gLunoNil;
    }
    if (a.Type == eLunoVar_Float && b.Type == eLunoVar_Int) {
        return cLunoVar(a.Type, a.Data.Float - (float)b.Data.Int);
    }
    if (a.Type == eLunoVar_Int && b.Type == eLunoVar_Float) {
        return cLunoVar(a.Type, a.Data.Int - (int)b.Data.Float);
    }
    return gLunoNil;
}

static inline cLunoVar lunoMul(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        if (a.Type == eLunoVar_Float) {
            return cLunoVar(a.Type, a.Data.Float * b.Data.Float);
        }
        if (a.Type == eLunoVar_Int) {
            return cLunoVar(a.Type, a.Data.Int * b.Data.Int);
        }
        return gLunoNil;
    }
    if (a.Type == eLunoVar_Float && b.Type == eLunoVar_Int) {
        return cLunoVar(a.Type, a.Data.Float * (float)b.Data.Int);
    }
    if (a.Type == eLunoVar_Int && b.Type == eLunoVar_Float) {
        return cLunoVar(a.Type, a.Data.Int * (int)b.Data.Float);
    }
    return gLunoNil;
}

static inline cLunoVar lunoDiv(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        if (a.Type == eLunoVar_Float) {
            return cLunoVar(a.Type, a.Data.Float / b.Data.Float);
        }
        if (a.Type == eLunoVar_Int) {
            return cLunoVar(a.Type, a.Data.Int / b.Data.Int);
        }
        return gLunoNil;
    }
    if (a.Type == eLunoVar_Float && b.Type == eLunoVar_Int) {
        return cLunoVar(a.Type, a.Data.Float / (float)b.Data.Int);
    }
    if (a.Type == eLunoVar_Int && b.Type == eLunoVar_Float) {
        return cLunoVar(a.Type, a.Data.Int / (int)b.Data.Float);
    }
    return gLunoNil;
}

static inline cLunoVar lunoMod(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        if (a.Type == eLunoVar_Int) {
            return cLunoVar(a.Type, a.Data.Int % b.Data.Int);
        }
        return gLunoNil;
    }
    return gLunoNil;
}

// Power is not implemented for any operand types. The two identical returns are real: the July 2003 prototype
// keeps the type test and two separate nil copies; retail cross-jumped them (only the type loads remain).
static inline cLunoVar lunoPow(const cLunoVar& a, const cLunoVar& b)
{
    if (b.Type == a.Type) {
        return gLunoNil;
    }
    return gLunoNil;
}

// One pushed C-function argument: the register index it came from and its value.
struct cLunoArg {
    int Index;
    cLunoVar Value;
    static void operator delete[](void* p) { cMemMan_free(p); }
};

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
// PORT: the game's operator new is cMemMan_alloc(size, tag, flags, 0); bound by asm label
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
// PORT: the table handle constructor with a size (luno_cLunoTable_cLunoTable) is bound by asm label.
extern "C++" cLunoTab* lunoNewTable(cLunoTab* self, int size) __asm__("luno_cLunoTable_cLunoTable");
extern "C" int func_002224A0(void);
extern char str_LunoVMCallParam[];
extern char str_cLunoTable[];

typedef cLunoVar (*tLunoCFunc)(int numArgs, cLunoArg* args);
extern tLunoCFunc gLunoCFunctions[] __asm__("D_00441F38");

// Compiled code: a 16-byte header (tChunkHeader, luno/lunofunc.h), the instruction words, then the function table.
struct sLunoFuncEntry {
    int Name;
    int Start;
    int Size;
    int NumArgs;
};

// A Luno instruction's op code (tLunoOp's first byte; names ours, from what cLunoVM::execute below does with each:
// [U]).
// Three pairs run the same code in the VM (Move / Move2, LoadInt / LoadInt2, PushInt / PushInt2); why the compiler
// emits both of each is not known. 26 is not handled.
enum eLunoOpCode {
    kLunoOp_Jump = 0,               // pc = the jump target
    kLunoOp_JumpIfTrue = 1,         // jump when register A is true
    kLunoOp_JumpIfFalse = 2,        // jump when register A is false
    kLunoOp_Equal = 3,              // A = B == C
    kLunoOp_NotEqual = 4,           // A = B != C
    kLunoOp_GreaterEqual = 5,       // A = B >= C
    kLunoOp_Greater = 6,            // A = B > C
    kLunoOp_LessEqual = 7,          // A = B <= C
    kLunoOp_Less = 8,               // A = B < C
    kLunoOp_Or = 9,                 // A = B || C
    kLunoOp_And = 10,               // A = B && C
    kLunoOp_Move = 11,              // A = B
    kLunoOp_Add = 12,               // A = B + C
    kLunoOp_Sub = 13,               // A = B - C
    kLunoOp_Mul = 14,               // A = B * C
    kLunoOp_Div = 15,               // A = B / C
    kLunoOp_Pow = 16,               // A = B ^ C (always nil)
    kLunoOp_Mod = 17,               // A = B % C
    kLunoOp_TableSet = 18,          // table A[B] = C
    kLunoOp_Move2 = 19,             // A = B, as Move
    kLunoOp_LoadIndex = 20,         // A = the next word as an index
    kLunoOp_LoadInt2 = 21,          // A = the next word as an int, as LoadInt
    kLunoOp_LoadInt = 22,           // A = the next word as an int
    kLunoOp_LoadFloat = 23,         // A = the next word as a float
    kLunoOp_TableGet = 24,          // A = table B[C]
    kLunoOp_NewTable = 25,          // A = a new 32-slot table
    kLunoOp_Call = 27,              // A = script function B(C), on a fresh cLunoVM
    kLunoOp_TableAppend = 28,       // table A[B] = C, then B's index + 1
    kLunoOp_LoadFunc = 29,          // A = function (the next word) of this chunk
    kLunoOp_LoadNil = 30,           // A = nil
    kLunoOp_Return = 31,            // return A
    kLunoOp_PushArg = 32,           // push C-function argument (key A, register B)
    kLunoOp_CallC = 33,             // A = gLunoCFunctions[B](the last C arguments)
    kLunoOp_Not = 34,               // A = !B
    kLunoOp_Neg = 35,               // A = -B
    kLunoOp_ForStep = 36,           // numeric for-loop step: A += C and jump while A has not passed B
    kLunoOp_PushInt = 37,           // push argument (key A, the next word as an int)
    kLunoOp_PushFloat = 38,         // push argument (key A, the next word as a float)
    kLunoOp_PushInt2 = 39,          // as PushInt
    kLunoOp_PushSmallInt = 40,      // push argument (key A, byte B as an int)
    kLunoOp_PushSmallFloat = 41,    // push argument (key A, byte B as a float)
    kLunoOp_ReturnNil = 42          // return nil
};

// One instruction word.
union tLunoOp {
    unsigned int Raw;
    int Int;
    float Float;
    struct {
        unsigned char Op;
        unsigned char A;
        unsigned char B;
        unsigned char C;
    } ABC;
    struct {
        unsigned char Op;
        unsigned char A;
        unsigned short Target;
    } Jump;
    struct {
        unsigned char Op;
        unsigned char Ret;
        unsigned char CFunc;
        unsigned char NumArgs;
    } CCall;
};

class cLunoVM {
public:
    cLunoVar* m_Regs;
    cLunoVM() __asm__("luno_cLunoVM_cLunoVM");
    ~cLunoVM() {}
    void call(const cLunoFunc& func, cLunoVar& self, const cLunoVar& arg, cLunoVar& result) __asm__("luno_cLunoVM_run1");
    void execute(const cLunoFunc& func, cLunoVar& result) __asm__("luno_cLunoVM_execute");
};

/**
 * Runs one compiled Luno script function to completion on this VM's registers.
 *
 * Starts at the function table entry's first instruction and interprets op-codes (eLunoOpCode) until a return op
 * (kLunoOp_Return stores a register into @p result, kLunoOp_ReturnNil returns nil) or the end of the code: jumps,
 * comparisons, arithmetic, table get/set, numeric for-loop steps, script calls (each through a fresh cLunoVM) and
 * C-function calls through gLunoCFunctions with arguments pushed onto a local stack. On exit it frees the argument
 * stack and also the VM's register array (m_Regs).
 *
 * @param func    the compiled function (code block and function index)
 * @param result  receives the returned value; left untouched if the code runs off the end
 */
void cLunoVM::execute(const cLunoFunc& func, cLunoVar& result)
{
    const char* code = func.Code;
    // ORIGINAL-PTR: a compiled Luno chunk is a file blob: its instruction words follow the tChunkHeader and its
    // function table sits FunctionOffset bytes from the chunk's start
    const int* ops = (const int*)(code + sizeof(tChunkHeader));
    int numOps = (((const tChunkHeader*)code)->FunctionOffset - sizeof(tChunkHeader)) / sizeof(int);
    cLunoArg* args = 0;
    int argDepth = 0;
    const sLunoFuncEntry* entry = (const sLunoFuncEntry*)(code + ((const tChunkHeader*)code)->FunctionOffset);
    entry += func.Index;
    int argDepthMax = entry->NumArgs;

    if (argDepthMax > 0) {
        args = new (str_LunoVMCallParam, 0xA0000000, 0) cLunoArg[argDepthMax];
    }
    int pc = entry->Start;
    while (pc < numOps) {
        tLunoOp op;
        op.Raw = ops[pc++];
        switch (op.ABC.Op) {
        case kLunoOp_PushArg:
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = m_Regs[op.ABC.B];
            argDepth++;
            break;
        case kLunoOp_LoadFloat: {
            tLunoOp value;
            value.Int = ops[pc++];
            m_Regs[op.ABC.A] = value.Float;
            break;
        }
        case kLunoOp_LoadInt: {
            tLunoOp value;
            value.Int = ops[pc++];
            m_Regs[op.ABC.A] = value.Int;
            break;
        }
        case kLunoOp_JumpIfTrue:
            if (m_Regs[op.ABC.A].isTrue()) {
                pc = op.Jump.Target;
            }
            break;
        case kLunoOp_JumpIfFalse:
            if (m_Regs[op.ABC.A].isFalse()) {
                pc = op.Jump.Target;
            }
            break;
        case kLunoOp_Jump:
            pc = op.Jump.Target;
            break;
        case kLunoOp_Not:
            m_Regs[op.ABC.A] = !m_Regs[op.ABC.B];
            break;
        case kLunoOp_Neg:
            m_Regs[op.ABC.A] = -m_Regs[op.ABC.B];
            break;
        case kLunoOp_Equal:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] == m_Regs[op.ABC.C];
            break;
        case kLunoOp_NotEqual:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] != m_Regs[op.ABC.C];
            break;
        case kLunoOp_GreaterEqual:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] >= m_Regs[op.ABC.C];
            break;
        case kLunoOp_Greater:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] > m_Regs[op.ABC.C];
            break;
        case kLunoOp_LessEqual:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] <= m_Regs[op.ABC.C];
            break;
        case kLunoOp_Less:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] < m_Regs[op.ABC.C];
            break;
        case kLunoOp_Or:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] || m_Regs[op.ABC.C];
            break;
        case kLunoOp_And:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B] && m_Regs[op.ABC.C];
            break;
        case kLunoOp_Move:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B];
            break;
        case kLunoOp_Add:
            m_Regs[op.ABC.A] = lunoAdd(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Sub:
            m_Regs[op.ABC.A] = lunoSub(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Mul:
            m_Regs[op.ABC.A] = lunoMul(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Div:
            m_Regs[op.ABC.A] = lunoDiv(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Mod:
            m_Regs[op.ABC.A] = lunoMod(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Pow:
            m_Regs[op.ABC.A] = lunoPow(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_Call:
            if (m_Regs[op.ABC.B].Type != eLunoVar_Nil) {
                cLunoVar& callArg = m_Regs[op.ABC.C];
                cLunoVM vm;
                vm.call(m_Regs[op.ABC.B].castFunc(), m_Regs[0], callArg, m_Regs[op.ABC.A]);
            } else {
                func_002224A0();
            }
            break;
        case kLunoOp_CallC:
            m_Regs[op.CCall.Ret] = gLunoCFunctions[op.CCall.CFunc](op.CCall.NumArgs, &args[argDepth - op.CCall.NumArgs]);
            argDepth -= op.CCall.NumArgs;
            break;
        case kLunoOp_TableSet:
            m_Regs[op.ABC.A].castTable().set(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            break;
        case kLunoOp_TableAppend:
            m_Regs[op.ABC.A].castTable().set(m_Regs[op.ABC.B], m_Regs[op.ABC.C]);
            m_Regs[op.ABC.B].Data.Index++;
            break;
        case kLunoOp_Move2:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B];
            break;
        case kLunoOp_LoadIndex: {
            unsigned int value = ops[pc++];
            m_Regs[op.ABC.A] = value;
            break;
        }
        case kLunoOp_LoadInt2: {
            tLunoOp value;
            value.Int = ops[pc++];
            m_Regs[op.ABC.A] = value.Int;
            break;
        }
        case kLunoOp_LoadFunc: {
            cLunoFunc newFunc;
            newFunc.init(code, ops[pc++]);
            m_Regs[op.ABC.A] = newFunc;
            break;
        }
        case kLunoOp_ForStep: {
            int target = ops[pc++];
            int down = m_Regs[op.ABC.C] < cLunoVar(0.0f);
            if (down) {
                if (m_Regs[op.ABC.A] >= m_Regs[op.ABC.B]) {
                    m_Regs[op.ABC.A] += m_Regs[op.ABC.C];
                    pc = target;
                }
            } else {
                if (m_Regs[op.ABC.A] <= m_Regs[op.ABC.B]) {
                    m_Regs[op.ABC.A] += m_Regs[op.ABC.C];
                    pc = target;
                }
            }
            break;
        }
        case kLunoOp_TableGet:
            m_Regs[op.ABC.A] = m_Regs[op.ABC.B].castTable().get(m_Regs[op.ABC.C]);
            break;
        case kLunoOp_NewTable:
            m_Regs[op.ABC.A] = *lunoNewTable((cLunoTab*)operator new(sizeof(cLunoTab), str_cLunoTable, 0x20000000, 0), 0x20);
            break;
        case kLunoOp_LoadNil:
            m_Regs[op.ABC.A] = gLunoNil;
            break;
        case kLunoOp_Return:
            result = m_Regs[op.ABC.A];
            pc = numOps;
            break;
        case kLunoOp_PushInt: {
            tLunoOp value;
            value.Int = ops[pc++];
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = value.Int;
            argDepth++;
            break;
        }
        case kLunoOp_PushFloat: {
            tLunoOp value;
            value.Int = ops[pc++];
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = value.Float;
            argDepth++;
            break;
        }
        case kLunoOp_PushInt2: {
            tLunoOp value;
            value.Int = ops[pc++];
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = value.Int;
            argDepth++;
            break;
        }
        case kLunoOp_PushSmallInt:
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = op.ABC.B;
            argDepth++;
            break;
        case kLunoOp_PushSmallFloat:
            args[argDepth].Index = op.ABC.A;
            args[argDepth].Value = (float)op.ABC.B;
            argDepth++;
            break;
        case kLunoOp_ReturnNil:
            result = gLunoNil;
            pc = numOps;
            break;
        }
    }
    delete[] args;
    delete[] m_Regs;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoFunc_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes a Luno function value (type 5) to a replay frame: its code-chunk pointer relative to @p base, then its index.
 *
 * Writes the first word (On Tour cLunoFunc Chunk) minus @p base and the second word (FuncNum), 4 bytes each, through
 * the frame's write virtual (slot 1). Called by luno_cLunoVar_writeToReplayFrame for type 5;
 * luno_cLunoFunc_readFromReplayFrame is the inverse.
 *
 * @param stream  the replay frame to write to
 * @param base  the address the chunk pointer is stored relative to
 */
void luno_cLunoFunc::writeToReplayFrame(cBxReplayFrame* stream, int base)
{
    int off;
    int secondWord;
    off = (int)Chunk - base;
    stream->write((const char*)&off, 4);
    secondWord = FuncNum;
    stream->write((const char*)&secondWord, 4);
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoFunc_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads a Luno function value (type 5) back from a replay frame, turning the stored offset into a pointer again.
 *
 * Reads two 4-byte words through the frame's read virtual (slot 2): the first plus @p base becomes the code-chunk
 * pointer (+0x0), the second the function index (+0x4). Called by luno_cLunoVar_readFromReplayFrame; inverse of
 * luno_cLunoFunc_writeToReplayFrame.
 *
 * @param stream  the replay frame to read from
 * @param base  the address the chunk pointer was stored relative to
 */
void luno_cLunoFunc::readFromReplayFrame(cBxReplayFrame* stream, const char* base)
{
    int buf[2];
    stream->read((char*)&buf[0], 4);
    Chunk = base + buf[0];
    stream->read((char*)&buf[1], 4);
    FuncNum = buf[1];
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTableData__cLunoTableData);
#ifdef SKIP_ASM
/**
 * Destroys a Luno table's shared data: every entry of its entry array, the array, and optionally the object.
 *
 * When the entry array (+0x10, On Tour Entries) exists it runs each entry's virtual destructor (slot 1, flags 0) from
 * the last to the first (the count sits in the array's 0x10-byte allocation header) and frees the block with
 * cMemMan_free. It frees the object itself with operator_delete when bit 0 of @p flags is set.
 * luno_cLunoTable_destroyTable calls it with 3 once the last reference goes.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void luno_cLunoTableData__cLunoTableData(luno_cLunoTableData* self, int flags)
{
    luno_cLunoTableEntry* nodes = self->Entries;

    if (nodes != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)nodes;
        luno_cLunoTableEntry* entry = nodes + cookies[-1].count;

        while (self->Entries != entry) {
            entry--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)entry->mVtbl;
            vt[1].fn(vthis(entry, vt[1]), 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)self->Entries;
        cMemMan_free(&array[-1]);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_cLunoTable);
#ifdef SKIP_ASM
extern "C" void luno_cLunoTable_constructTable(cLunoTableRef* self);

/**
 * Constructs a Luno table reference holding a new table.
 *
 * Clears the table pointer, then creates the table with luno_cLunoTable_constructTable().
 *
 * @return self
 */
extern "C" cLunoTableRef* luno_cLunoTable_cLunoTable(cLunoTableRef* self)
{
    self->table = 0;
    luno_cLunoTable_constructTable(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_cLunoTable1);
#ifdef SKIP_ASM
/**
 * Copies a Luno table handle: shares the source's table data and adds a reference to it.
 *
 * Stores *@p src in *@p dest and increments the data's reference count (+0x8).
 *
 * @param dest  the new handle
 * @param src  the handle to copy
 * @return @p dest
 */
extern "C" void **luno_cLunoTable_cLunoTable1(void **dest, void **src) {
    luno_cLunoTableData* temp_6;

    temp_6 = (luno_cLunoTableData*)(*src);
    *dest = ((void*)temp_6);
    temp_6->RefCount = (int) ((temp_6->RefCount) + 1);
    return dest;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable__cLunoTable);
#ifdef SKIP_ASM
/**
 * Destroys a Luno table reference, releasing the shared table when this was its last reference.
 *
 * Decrements the table's refCount; at zero luno_cLunoTable_destroyTable() deletes the table and clears the pointer.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the reference object's memory
 */
extern "C" void luno_cLunoTable__cLunoTable(cLunoTableRef* self, int flags)
{
    if (--self->table->RefCount == 0) {
        luno_cLunoTable_destroyTable(self);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_constructTable);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: the game's operator new is cMemMan_alloc(size, tag, flags, 0); bound by asm label
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char str_LunoTable[];
extern char str_TableData[];
extern char luno_cLunoTableEntry_vtbl[];

struct cLunoNode_ct {
    cLunoValue key;
    cLunoValue val;
    cLunoNode_ct* next;
    void* vt;
    cLunoNode_ct()
    {
        vt = luno_cLunoTableEntry_vtbl;
        key.type = 0;
        key.word0 = 0;
        val.type = 0;
        val.word0 = 0;
        next = 0;
    }
    ~cLunoNode_ct() {}
};

extern "C" void luno_cLunoTable_constructTable_n(cLunoTableRef* ref, int n) __asm__("luno_cLunoTable_constructTable");
/**
 * Creates a Luno script table with room for @p n nodes and points a table reference at it.
 *
 * Allocates the table ("LunoTable") and its node array ("TableData"); the modulus is set to n - 1 and the
 * reference count to 1.
 *
 * @param ref  the table reference that receives the new table
 * @param n    the number of node slots (the modulus n - 1 suggests a power of two)
 */
extern "C" void luno_cLunoTable_constructTable_n(cLunoTableRef* ref, int n)
{
    luno_cLunoTableData* newTable = (luno_cLunoTableData*)new (str_LunoTable, 0xA0000000, 0) luno_cLunoTableData;
    luno_cLunoTableEntry** slot = &newTable->Entries;
    newTable->Entries = 0;
    newTable->RefCount = 0;
    newTable->Modulus = 0;
    ref->table = newTable;
    *slot = (luno_cLunoTableEntry*)new (str_TableData, 0xA0000000, 0) cLunoNode_ct[n];
    ref->table->Modulus = n - 1;
    ref->table->RefCount = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_destroyTable);
#ifdef SKIP_ASM
extern "C" void luno_cLunoTableData__cLunoTableData(luno_cLunoTableData* table, int flags);

/**
 * Destroys the table data a Luno table handle points at and clears the handle.
 *
 * Runs luno_cLunoTableData__cLunoTableData on the data with flags 3 (destroy and free) when there is one, then sets the
 * handle to null. luno_cLunoTable_operatorE and luno_cLunoTable__cLunoTable call it when the reference count drops to
 * 0.
 *
 * @param ref  the table handle
 */
extern "C" void luno_cLunoTable_destroyTable(cLunoTableRef* ref)
{
    luno_cLunoTableData* ownedTable = ref->table;
    if (ownedTable != 0) {
        luno_cLunoTableData__cLunoTableData(ownedTable, 3);
    }
    ref->table = 0;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_emptyTable);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern char str_TableData[];
extern char luno_cLunoTableEntry_vtbl[];


/**
 * Empties a Luno table: destroys every entry of its array and allocates a fresh array of empty entries.
 *
 * The old entries (0x20 bytes: key, value, chain link +0x18, vtable luno_cLunoTableEntry_vtbl at +0x1C) are destroyed through their
 * virtual destructor (slot 1) from the last to the first and the block (count in its 0x10-byte header) is freed. The
 * new array has modulus (+0xC) + 1 nil entries, allocated with operator_new__FUi tagged "TableData", flags 0xA0000000.
 * Called by cWScriptMan_readFromReplayFrame.
 *
 * @param ref  the table handle
 */
extern "C" void luno_cLunoTable_emptyTable(cLunoTableRef* ref)
{
    if (ref->table->Entries != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)ref->table->Entries;
        luno_cLunoTableEntry* entry = ref->table->Entries + cookies[-1].count;
        while (ref->table->Entries != entry) {
            entry--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)entry->mVtbl;
            vt[1].fn(vthis(entry, vt[1]), 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)ref->table->Entries;
        cMemMan_free(&array[-1]);
    }
    luno_cLunoTableEntry** slot = &ref->table->Entries;
    *slot = (luno_cLunoTableEntry*)new (str_TableData, 0xA0000000, 0) cLunoNode_ct[ref->table->Modulus + 1];
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_operatorE);
#ifdef SKIP_ASM
/**
 * Assigns one Luno table handle to another: shares the other's table data, releasing the old data on its last
 * reference.
 *
 * Does nothing when both already share the data. Otherwise it decrements the old data's reference count (+0x8) and
 * destroys it with luno_cLunoTable_destroyTable at 0, then takes @p other's data and increments its count. Used by
 * luno_cLunoTable_set, luno_cLunoVM_execute and luno_cLunoVar_readFromReplayFrame.
 *
 * @param other  the handle to copy from
 * @return @p self
 */
extern "C" cLunoTableRef* luno_cLunoTable_operatorE(cLunoTableRef* self, cLunoTableRef* other)
{
    if (self->table != other->table) {
        if (self->table != 0) {
            if (--self->table->RefCount == 0) {
                luno_cLunoTable_destroyTable(self);
            }
        }
        self->table = other->table;
        self->table->RefCount++;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_get);
#ifdef SKIP_ASM
extern "C" void luno_cLunoVar__cLunoVar(cLunoValue* self, int flags);
extern cLunoValue luno_gLunoNil;
void* luno_cLunoVar_castIndex(void* self);


static inline int lunoKeyEquals_50E0(cLunoValue* a, cLunoValue* b)
{
    if (b->type == a->type) {
        return a->word0 == b->word0;
    }
    if (a->type == eLunoVar_Float && b->type == eLunoVar_Int) {
        return *(float*)&a->word0 == (float)b->word0;
    }
    if (a->type == eLunoVar_Int && b->type == eLunoVar_Float) {
        return a->word0 == (int)*(float*)&b->word0;
    }
    if (a->type == eLunoVar_Index && b->type == eLunoVar_Int) {
        return a->word0 == b->word0;
    }
    return 0;
}

/**
 * Looks up integer key @p intKey in a Luno table and returns the value stored under it, or luno_gLunoNil.
 *
 * Starts at hash bucket intKey % modulus of the table's node array and follows the chain (+0x18) while the bucket is in
 * use. Each node key is compared with a temporary index key (type 3): equal types compare the words, a float key (type
 * 2) matches an int (type 1) by value, and an index matches an int; the temporary is destroyed
 * (luno_cLunoVar__cLunoVar) after each comparison.
 *
 * @param intKey  the integer key
 * @return the node's value, or &luno_gLunoNil when absent
 */
extern "C" cLunoValue* luno_cLunoTable_get(cLunoTableRef* self, unsigned int intKey)
{
    luno_cLunoTableData* hashTable = self->table;
    luno_cLunoTableEntry* n = hashTable->Entries + intKey % (unsigned int)hashTable->Modulus;
    if ((*(cLunoValue*)&n->Value).type != 0) {
        do {
            cLunoValue key;
            key.type = 3;
            *(int*)luno_cLunoVar_castIndex(&key) = intKey;
            int eq = lunoKeyEquals_50E0(&(*(cLunoValue*)&n->Index), &key);
            luno_cLunoVar__cLunoVar(&key, 2);
            if (eq) {
                return &(*(cLunoValue*)&n->Value);
            }
            n = n->Next;
        } while (n != 0);
    }
    return &luno_gLunoNil;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_get1);
#ifdef SKIP_ASM
extern cLunoValue luno_gLunoNil;

// Hash node: key, value, chain link (0x20 bytes, vtable at +0x1C).
struct cLunoNode {
    cLunoValue key;     // 0x00
    cLunoValue value;   // 0x0C
    cLunoNode* next;    // 0x18
    void* vtable;       // 0x1C
};

static inline int lunoKeyEquals(cLunoValue* a, cLunoValue* b)
{
    if (b->type == a->type) {
        return a->word0 == b->word0;
    }
    if (a->type == eLunoVar_Float && b->type == eLunoVar_Int) {
        return *(float*)&a->word0 == (float)b->word0;
    }
    if (a->type == eLunoVar_Int && b->type == eLunoVar_Float) {
        return a->word0 == (int)*(float*)&b->word0;
    }
    if (a->type == eLunoVar_Index && b->type == eLunoVar_Int) {
        return a->word0 == b->word0;
    }
    return 0;
}

/**
 * Looks a key value up in a Luno table and returns a reference to its value, or to luno_gLunoNil when absent.
 *
 * Hashes the key's first word modulo the modulus (+0xC) into the entry array (+0x10). An empty bucket head (value type
 * 0) means absent; otherwise it walks the chain (+0x18) comparing keys: equal types compare the first word, a stored
 * float key matches an int key by value, a stored int key matches a float key truncated, and a stored index key (type
 * 3) matches an int key with the same word. Quirk: the hash uses the raw word, so an int and a float key of the same
 * number usually land in different buckets and the cross-type matches rarely apply. Used by cWScriptMan_isFunc,
 * cWScriptMan_runFunc and luno_cLunoVM_execute.
 *
 * @param key  the key value
 * @return the stored value, or luno_gLunoNil
 */
extern "C" cLunoValue* luno_cLunoTable_get1(cLunoTableRef* self, cLunoValue* key)
{
    luno_cLunoTableData* hashTable = self->table;
    luno_cLunoTableEntry* n = hashTable->Entries + (unsigned int)key->word0 % (unsigned int)hashTable->Modulus;
    if (n->Value.Type != 0) {
        do {
            // cLunoValue, this unit's script value {word0, word1, type}, has luno_cLunoVar's layout
            if (lunoKeyEquals((cLunoValue*)&n->Index, key)) {
                return (cLunoValue*)&n->Value;
            }
            n = n->Next;
        } while (n != 0);
    }
    return &luno_gLunoNil;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_set);
#ifdef SKIP_ASM
extern char str_cLunoTableEntry[];
extern char luno_cLunoTableEntry_vtbl[];

struct sLunoPair_set {
    int word0;
    int word1;
};

// PORT: the game's operator new is cMemMan_alloc(size, tag, flags, 0); bound by asm label
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

// Table chain node (same layout as cLunoNode and luno_cLunoTableEntry).
struct cLunoNode_set {
    cLunoValue key;            // 0x00
    cLunoValue value;          // 0x0C
    cLunoNode_set* next;       // 0x18
    cLunoVtEntry* vtable;      // 0x1C
};

// cLunoValue::operator= (inline Clear of the destination, then copy by type)
#define LUNO_ASSIGN_SET(D, S)                                              \
    {                                                                      \
        cLunoValue* d = &(D);                                              \
        if ((D).type == eLunoVar_Table) {                                  \
            luno_cLunoTable__cLunoTable((cLunoTableRef*)d, 2);                           \
        }                                                                  \
        d->type = 0;                                                       \
        (D).word0 = 0;                                                     \
        if ((S)->type == eLunoVar_Table) {                                 \
            luno_cLunoTable_operatorE((cLunoTableRef*)d, (cLunoTableRef*)(S));         \
        } else if ((S)->type == eLunoVar_Func) {                           \
            *(sLunoPair_set*)&(D) = *(sLunoPair_set*)(S);                  \
        } else {                                                           \
            (D).word0 = (S)->word0;                                        \
        }                                                                  \
        d->type = (S)->type;                                               \
    }

static inline int lunoKeyEquals_set(cLunoValue* a, cLunoValue* b)
{
    if (b->type == a->type) {
        return a->word0 == b->word0;
    }
    if (a->type == eLunoVar_Float && b->type == eLunoVar_Int) {
        return *(float*)&a->word0 == (float)b->word0;
    }
    if (a->type == eLunoVar_Int && b->type == eLunoVar_Float) {
        return a->word0 == (int)*(float*)&b->word0;
    }
    if (a->type == eLunoVar_Index && b->type == eLunoVar_Int) {
        return a->word0 == b->word0;
    }
    return 0;
}

/**
 * Stores a value under a key in a Luno script table; a nil key does nothing.
 *
 * The key's first word modulo the table size picks the bucket. An empty bucket head, or one with an equal
 * key, takes the value directly. A matching chained node gets the new value and is unlinked and destroyed if
 * that value is nil. A new key with a non-nil value is appended to the chain in a new 0x20-byte node.
 *
 * @param key  the key value
 * @param val  the value to store (nil removes a chained entry)
 */
extern "C" void luno_cLunoTable_set(cLunoTableRef* self, cLunoValue* key, cLunoValue* val)
{
    if (key->type == eLunoVar_Nil) {
        return;
    }
    luno_cLunoTableData* lunoTable = self->table;
    // cLunoNode_set has luno_cLunoTableEntry's layout, with this unit's cLunoValue key and value
    cLunoNode_set* n = (cLunoNode_set*)&lunoTable->Entries[(unsigned int)key->word0 % (unsigned int)lunoTable->Modulus];
    if (n->value.type == eLunoVar_Nil) {
        LUNO_ASSIGN_SET(n->key, key)
        LUNO_ASSIGN_SET(n->value, val)
        return;
    }
    if (lunoKeyEquals_set(&n->key, key)) {
        LUNO_ASSIGN_SET(n->value, val)
        return;
    }
    cLunoNode_set* prev = n;
    for (n = n->next; n != 0; n = n->next) {
        if (lunoKeyEquals_set(&n->key, key)) {
            {
                cLunoValue* d = &n->value;
                if (n->value.type == eLunoVar_Table) {
                    luno_cLunoTable__cLunoTable((cLunoTableRef*)d, 2);
                }
                d->type = 0;
                // Expanded Clear(); the do/while(0) is the original macro's wrapper (changes scheduling).
                do {
                    n->value.word0 = 0;
                } while (0);
                if (val->type == eLunoVar_Table) {
                    luno_cLunoTable_operatorE((cLunoTableRef*)d, (cLunoTableRef*)val);
                } else if (val->type == eLunoVar_Func) {
                    *(sLunoPair_set*)&n->value = *(sLunoPair_set*)val;
                } else {
                    n->value.word0 = val->word0;
                }
                d->type = val->type;
            }
            if (n->value.type == eLunoVar_Nil) {
                prev->next = n->next;
                if (n != 0) {
                    n->vtable[1].func(vthis(n, n->vtable[1]), 3);
                }
            }
            return;
        }
        prev = n;
    }
    if (val->type != 0) {
        cLunoNode_set* node = (cLunoNode_set*)operator new(0x20, str_cLunoTableEntry, 0x20000000, 0);
        node->value.type = 0;
        node->value.word0 = 0;
        node->vtable = (cLunoVtEntry*)luno_cLunoTableEntry_vtbl;
        node->key.type = 0;
        node->key.word0 = 0;
        LUNO_ASSIGN_SET(node->key, key)
        LUNO_ASSIGN_SET(node->value, val)
        node->next = 0;
        prev->next = node;
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void luno_cLunoVar_writeToReplayFrame(cLunoValue* self, cBxReplayFrame* stream, int base);

/**
 * Writes a Luno table to a replay frame: the data's first 8 bytes, the number of used entries, then each one's key and
 * value.
 *
 * Counts the entries whose value type is non-zero over all modulus + 1 array slots and their chains (+0x18), writes the
 * count (4 bytes, write virtual slot 1), then writes key and value of each used entry with
 * luno_cLunoVar_writeToReplayFrame (pointers relative to @p base). luno_cLunoTable_readFromReplayFrame reads it back.
 * Called by luno_cLunoVar_writeToReplayFrame for type 4, cWScriptProcess_writeToReplayFrame and
 * cWScriptMan_writeToReplayFrame.
 *
 * @param vself  the table handle (this)
 * @param stream  the replay frame to write to
 * @param base  the address pointers are stored relative to
 */
extern "C" void luno_cLunoTable_writeToReplayFrame(void* vself, cBxReplayFrame* stream, int base)
{
    cLunoTableRef* self = (cLunoTableRef*)vself;
    stream->write((const char*)self->table, 8);
    int cnt = 0;
    luno_cLunoTableData* hashTable = self->table;
    cLunoNode* n = (cLunoNode*)((cLunoTableEntry*)hashTable->Entries);
    for (int i = 0; i <= hashTable->Modulus; i++) {
        if (n->value.type != 0) {
            cnt++;
        }
        cLunoNode* next = n + 1;
        for (cLunoNode* chainNode = n->next; chainNode != 0; chainNode = chainNode->next) {
            if (chainNode->value.type != 0) {
                cnt++;
            }
        }
        n = next;
    }
    stream->write((const char*)&cnt, 4);
    if (cnt > 0) {
        cLunoNode* n = (cLunoNode*)((cLunoTableEntry*)self->table->Entries);
        for (int i = 0; i <= self->table->Modulus; i++) {
            cLunoValue* nodeValue = &n->value;
            if (n->value.type != 0) {
                cnt--;
                luno_cLunoVar_writeToReplayFrame(&n->key, stream, base);
                luno_cLunoVar_writeToReplayFrame(nodeValue, stream, base);
            }
            cLunoNode* next = n + 1;
            for (cLunoNode* chainNode = n->next; chainNode != 0; chainNode = chainNode->next) {
                cLunoValue* mv = &chainNode->value;
                if (chainNode->value.type != 0) {
                    cnt--;
                    luno_cLunoVar_writeToReplayFrame(&chainNode->key, stream, base);
                    luno_cLunoVar_writeToReplayFrame(mv, stream, base);
                }
            }
            n = next;
        }
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTable_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void luno_cLunoVar__cLunoVar(cLunoValue* self, int flags);
extern "C" void luno_cLunoVar_readFromReplayFrame(cLunoValue* self, cBxReplayFrame* stream, int base);
extern "C" void luno_cLunoTable_set(cLunoTableRef* self, cLunoValue* key, cLunoValue* val);


struct cLunoNode_9A8 {
    cLunoValue key;
    cLunoValue val;
    cLunoNode_9A8* next;
    void* vt;
    cLunoNode_9A8()
    {
        vt = luno_cLunoTableEntry_vtbl;
        key.type = 0;
        key.word0 = 0;
        val.type = 0;
        val.word0 = 0;
        next = 0;
    }
    void operator delete[](void* p, unsigned int size);
};

/**
 * Rebuilds a Luno table from a replay frame, replacing its current contents.
 *
 * Destroys and frees the old node array, reads the table header (8 bytes) through the frame's read method,
 * allocates modulus + 1 empty nodes, then reads an entry count and that many key/value pairs
 * (luno_cLunoVar_readFromReplayFrame) and stores each pair with luno_cLunoTable_set().
 *
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame*)
 * @param base    passed through to the value reader (On Tour: const char* base)
 */
extern "C" void luno_cLunoTable_readFromReplayFrame(cLunoTableRef* self, cBxReplayFrame* stream, int base)
{
    if (self->table->Entries != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)self->table->Entries;
        luno_cLunoTableEntry* node = self->table->Entries + cookies[-1].count;
        while (self->table->Entries != node) {
            node--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)node->mVtbl;
            vt[1].fn(vthis(node, vt[1]), 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)self->table->Entries;
        cMemMan_free(&array[-1]);
    }
    stream->read((char*)self->table, 8);
    luno_cLunoTableEntry** slot = &self->table->Entries;
    *slot = (luno_cLunoTableEntry*)new (str_TableData, 0xA0000000, 0) cLunoNode_9A8[self->table->Modulus + 1];
    int i;
    int cnt;
    cLunoValue key;
    cLunoValue val;
    stream->read((char*)&cnt, 4);
    for (i = 0; i < cnt; i++) {
        key.type = 0;
        key.word0 = 0;
        luno_cLunoVar_readFromReplayFrame(&key, stream, base);
        val.type = 0;
        val.word0 = 0;
        luno_cLunoVar_readFromReplayFrame(&val, stream, base);
        luno_cLunoTable_set(self, &key, &val);
        luno_cLunoVar__cLunoVar(&val, 2);
        luno_cLunoVar__cLunoVar(&key, 2);
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVar__cLunoVar);
#ifdef SKIP_ASM
/**
 * Destroys a Luno variable, releasing its table reference when it holds a table.
 *
 * A variable of type eLunoVar_Table runs the table-reference destructor without freeing; the type is then reset to
 * eLunoVar_Nil.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the variable's memory
 */
extern "C" void luno_cLunoVar__cLunoVar(cLunoValue* self, int flags)
{
    if (self->type == eLunoVar_Table) {
        luno_cLunoTable__cLunoTable((cLunoTableRef*)self, 2);
    }
    self->type = eLunoVar_Nil;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void luno_cLunoFunc_writeToReplayFrame(luno_cLunoFunc* self, cBxReplayFrame* stream, int base);
extern "C" void luno_cLunoTable_writeToReplayFrame(void* self, cBxReplayFrame* stream, int base);

/**
 * Saves a Luno value into a replay frame.
 *
 * Writes the 4-byte type through the stream's slot 1, then the payload: type 5 (eLunoVar_Func, a function value)
 * with luno_cLunoFunc_writeToReplayFrame, type 4 (table) with luno_cLunoTable_writeToReplayFrame, anything else as one
 * 4-byte word. luno_cLunoVar_readFromReplayFrame() is the inverse.
 *
 * @param stream  the replay frame
 * @param base  the base address string pointers are stored relative to
 */
extern "C" void luno_cLunoVar_writeToReplayFrame(cLunoValue* self, cBxReplayFrame* stream, int base)
{
    stream->write((const char*)&self->type, 4);
    if (self->type == eLunoVar_Func) {
        ((luno_cLunoFunc*)self)->writeToReplayFrame(stream, base);
    } else if (self->type == eLunoVar_Table) {
        luno_cLunoTable_writeToReplayFrame(self, stream, base);
    } else {
        stream->write((const char*)self, 4);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_readFromReplayFrame);
#ifdef SKIP_ASM

// PORT: luno_cLunoTable_cLunoTable forwards a size argument to constructTable (unit declares 1 arg).
extern "C++" cLunoTableRef* luno_cLunoTable_cLunoTable_sz(cLunoTableRef* self, int size) __asm__("luno_cLunoTable_cLunoTable");
extern "C" void luno_cLunoTable_readFromReplayFrame(cLunoTableRef* self, cBxReplayFrame* stream, int base);

static inline int lunoReadInt_225CA0(cBxReplayFrame* stream)
{
    int v;
    stream->read((char*)&v, 4);
    return v;
}

/**
 * Restores a Luno value from a replay frame.
 *
 * Reads the type word. Type 5 (eLunoVar_Func) reads the function value (luno_cLunoFunc_readFromReplayFrame); type 4
 * reads a whole table into a temporary (luno_cLunoTable_cLunoTable_sz + luno_cLunoTable_readFromReplayFrame) and copies
 * it in (luno_cLunoTable_operatorE); otherwise it reads one 4-byte word. A table this value held before is released
 * first (luno_cLunoTable__cLunoTable with flags 2).
 *
 * @param stream  the replay frame
 * @param base  the base address string pointers are relative to
 */
extern "C" void luno_cLunoVar_readFromReplayFrame(cLunoValue* self, cBxReplayFrame* stream, int base)
{
    int type = lunoReadInt_225CA0(stream);
    if (type == eLunoVar_Func) {
        luno_cLunoFunc pair;
        (*(int*)&pair.Chunk) = 0;
        pair.FuncNum = 0;
        pair.readFromReplayFrame(stream, (const char*)base);
        if (self->type == eLunoVar_Table) {
            luno_cLunoTable__cLunoTable((cLunoTableRef*)self, 2);
        }
        self->type = eLunoVar_Nil;
        self->word0 = 0;
        *(luno_cLunoFunc*)self = pair;
        self->type = type;
    } else if (type == eLunoVar_Table) {
        luno_cLunoTable ref;
        luno_cLunoTable_cLunoTable_sz((cLunoTableRef*)&ref, 0x20);
        luno_cLunoTable_readFromReplayFrame((cLunoTableRef*)&ref, stream, base);
        if (self->type == type) {
            luno_cLunoTable__cLunoTable((cLunoTableRef*)self, 2);
        }
        self->type = eLunoVar_Nil;
        self->word0 = 0;
        luno_cLunoTable_operatorE((cLunoTableRef*)self, (cLunoTableRef*)&ref);
        self->type = type;
        luno_cLunoTable__cLunoTable((cLunoTableRef*)&ref, 2);
    } else {
        self->type = type;
        stream->read((char*)self, 4);
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00225DE8);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_5DE8 {
    float x, y, z, w;
    sV4_5DE8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern char D_004C8D18[], D_004C8D28[], D_004C8D38[], D_004C8D48[], D_004C8D58[], D_004C8D68[], D_004C8D78[], D_004C8D88[], D_004C8D98[], D_004C8DA8[], D_004C8DB8[], D_004C8DC8[], D_004C8DD8[], D_004C8DE8[], D_004C8DF8[], D_004C8E08[], D_004C8E18[], D_004C8E28[], D_004C8E38[], D_004C8E48[], D_004C8E58[], D_004C8E68[], D_004C8E78[], D_004C8E88[], D_004C8E98[], D_004C8EA8[], D_004C8EB8[], D_004C8EC8[], D_004C8ED8[], D_004C8EE8[], D_004C8EF8[], D_004C8F08[], D_004C8F18[], D_004C8F28[], D_004C8F38[], D_004C8F48[], D_004C8F58[], D_004C8F68[], D_004C8F78[], D_004C8F88[], D_004C8F98[], D_004C8FA8[], D_004C8FB8[], D_004C8FC8[], D_004C8FD8[], D_004C8FE8[], D_004C8FF8[], D_004C9008[], D_004C9018[], D_004C9028[], D_004C9038[], D_004C9048[], D_004C9058[], D_004C9068[], D_004C9078[], D_004C9088[];
extern "C" void luno_cLunoVar__cLunoVar(cLunoValue* self, int flags);

// PORT: the unit declares func_00225DE8 as void*(int, int); it returns nothing.
extern "C" void func_00225DE8_impl(int initialize, int priority) __asm__("func_00225DE8");
/**
 * Constructs or destroys the unit's static objects: 56 colours and the Luno nil value luno_gLunoNil.
 *
 * Only for @p priority DEFAULT_INIT_PRIORITY (65535). With @p initialize set it builds the colour table
 * D_004C8D18..D_004C9088 (four floats each, the table other units build too) and sets luno_gLunoNil to nil (type 0,
 * word 0); otherwise it destroys luno_gLunoNil with luno_cLunoVar__cLunoVar(.., 2). func_002267F0 and func_00226810
 * call it as the unit's constructor and destructor (g++ __static_initialization_and_destruction_0).
 *
 * @param initialize  non-zero to construct, 0 to destroy
 * @param priority  the init priority; only DEFAULT_INIT_PRIORITY does anything
 */
extern "C" void func_00225DE8_impl(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY) {
        if (initialize != 0) {
            new (D_004C8D18) sV4_5DE8(1.0f, 1.0f, 1.0f, 0.5f);
            new (D_004C8D28) sV4_5DE8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (D_004C8D38) sV4_5DE8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (D_004C8D48) sV4_5DE8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (D_004C8D58) sV4_5DE8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (D_004C8D68) sV4_5DE8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
            new (D_004C8D78) sV4_5DE8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
            new (D_004C8D88) sV4_5DE8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
            new (D_004C8D98) sV4_5DE8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
            new (D_004C8DA8) sV4_5DE8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
            new (D_004C8DB8) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8DC8) sV4_5DE8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C8DD8) sV4_5DE8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
            new (D_004C8DE8) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8DF8) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8E08) sV4_5DE8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C8E18) sV4_5DE8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C8E28) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8E38) sV4_5DE8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C8E48) sV4_5DE8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C8E58) sV4_5DE8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C8E68) sV4_5DE8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
            new (D_004C8E78) sV4_5DE8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
            new (D_004C8E88) sV4_5DE8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
            new (D_004C8E98) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8EA8) sV4_5DE8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
            new (D_004C8EB8) sV4_5DE8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
            new (D_004C8EC8) sV4_5DE8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
            new (D_004C8ED8) sV4_5DE8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
            new (D_004C8EE8) sV4_5DE8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
            new (D_004C8EF8) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8F08) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8F18) sV4_5DE8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
            new (D_004C8F28) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8F38) sV4_5DE8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
            new (D_004C8F48) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8F58) sV4_5DE8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C8F68) sV4_5DE8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
            new (D_004C8F78) sV4_5DE8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
            new (D_004C8F88) sV4_5DE8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
            new (D_004C8F98) sV4_5DE8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
            new (D_004C8FA8) sV4_5DE8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
            new (D_004C8FB8) sV4_5DE8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C8FC8) sV4_5DE8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
            new (D_004C8FD8) sV4_5DE8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C8FE8) sV4_5DE8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C8FF8) sV4_5DE8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C9008) sV4_5DE8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
            new (D_004C9018) sV4_5DE8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
            new (D_004C9028) sV4_5DE8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
            new (D_004C9038) sV4_5DE8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
            new (D_004C9048) sV4_5DE8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
            new (D_004C9058) sV4_5DE8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
            new (D_004C9068) sV4_5DE8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
            new (D_004C9078) sV4_5DE8(1.0f, 0.0f, 1.0f, 0.0f);
            new (D_004C9088) sV4_5DE8(1.0f, 1.0f, 0.0f, 0.0f);
            luno_gLunoNil.type = 0;
            luno_gLunoNil.word0 = 0;
        } else {
            luno_cLunoVar__cLunoVar(&luno_gLunoNil, 2);
        }
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoFunc_init__FPvii);
#ifdef SKIP_ASM
/**
 * Sets a Luno function value's code chunk and function index.
 *
 * @param firstWord  the code chunk (On Tour data, stored as Chunk)
 * @param secondWord  the function index (FuncNum)
 */
void luno_cLunoFunc::init(int firstWord, int secondWord)
{
    *(int*)this = firstWord;
    FuncNum = secondWord;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_castInt__FPv);
#ifdef SKIP_ASM
void* luno_cLunoVar_castInt(void* self);

/**
 * Returns a Luno value's data as an int reference: the value's own address, since the data is its first word.
 *
 * @return this variable
 */
void* luno_cLunoVar::castInt()
{
    return this;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_castFloat__FPv);
#ifdef SKIP_ASM
void* luno_cLunoVar_castFloat(void* self);

/**
 * Returns a Luno value's data as a float reference: the value's own address.
 *
 * @return this variable
 */
void* luno_cLunoVar::castFloat()
{
    return this;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_castIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns a Luno value's data as an index reference: the value's own address.
 *
 * @return this variable
 */
void* luno_cLunoVar::castIndex()
{
    return this;
}
#endif

extern cLunoValue luno_gLunoNil;

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoVar_operatorPlus);
#ifdef SKIP_ASM
/**
 * Adds two Luno values (cLunoVar operator+), returning the sum by value in @p self.
 *
 * Same types: floats and ints add, index values (type 3) add as ints; any other same-type pair gives nil. A float plus
 * an int gives a float; an int plus a float gives an int (the float truncated). Mixed types otherwise give nil
 * (luno_gLunoNil). Used by luno_cLunoVM_execute.
 *
 * @param lhs  the left operand (this)
 * @param rhs  the right operand
 * @return @p self
 */
extern "C" cLunoValue* luno_cLunoVar_operatorPlus(cLunoValue* self, const cLunoValue& lhs, const cLunoValue& rhs)
{
    if (rhs.type == lhs.type) {
        if (lhs.type == eLunoVar_Float) {
            float floatSum = *(float*)&lhs.word0 + *(float*)&rhs.word0;
            self->type = lhs.type;
            *(float*)luno_cLunoVar_castFloat(self) = floatSum;
        } else if (lhs.type == eLunoVar_Int) {
            int i = lhs.word0 + rhs.word0;
            self->type = lhs.type;
            *(int*)luno_cLunoVar_castInt(self) = i;
        } else if (lhs.type == eLunoVar_Index) {
            int i = lhs.word0 + rhs.word0;
            self->type = lhs.type;
            *(int*)luno_cLunoVar_castIndex(self) = i;
        } else {
            *self = luno_gLunoNil;
        }
    } else if (lhs.type == eLunoVar_Float && rhs.type == eLunoVar_Int) {
        float floatSum = *(float*)&lhs.word0 + (float)rhs.word0;
        self->type = lhs.type;
        *(float*)luno_cLunoVar_castFloat(self) = floatSum;
    } else if (lhs.type == eLunoVar_Int && rhs.type == eLunoVar_Float) {
        int i = lhs.word0 + (int)*(float*)&rhs.word0;
        self->type = lhs.type;
        *(int*)luno_cLunoVar_castInt(self) = i;
    } else {
        *self = luno_gLunoNil;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", luno_cLunoTableEntry__cLunoTableEntry);
#ifdef SKIP_ASM
extern char luno_cLunoTableEntry_vtbl[];

/**
 * Destroys a Luno table entry: its chained successor, its value and its key.
 *
 * Restores the entry vtable luno_cLunoTableEntry_vtbl (+0x1C), destroys the next entry in the chain (+0x18) through its virtual
 * destructor with flags 3 (so a whole chain is freed recursively), runs luno_cLunoVar__cLunoVar on the value (+0xC) and
 * the key (+0x0) with flags 2, and frees the entry when bit 0 of @p flags is set. Slot 1 of luno_cLunoTableEntry_vtbl.
 *
 * @param flags  destructor flags; bit 0 frees the entry
 */
extern "C" void luno_cLunoTableEntry__cLunoTableEntry(luno_cLunoTableEntry* self, int flags)
{
    self->mVtbl = (void**)luno_cLunoTableEntry_vtbl;
    luno_cLunoTableEntry* obj = self->Next;
    if (((void*)obj) != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)obj->mVtbl;
        vt[1].fn(vthis(obj, vt[1]), 3);
    }
    luno_cLunoVar__cLunoVar((cLunoValue*)(&self->Value), 2);
    luno_cLunoVar__cLunoVar((cLunoValue*)self, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_00225DE8(int, int);

//99.38%
INCLUDE_ASM("luno/lunovm", func_002267F0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static constructor: func_00225DE8(1, 0xFFFF).
 *
 * @return func_00225DE8's result register (that function returns nothing)
 */
void* func_002267F0(void* self)
{
    return func_00225DE8(1, 0xffff);
}
#endif

extern "C" void* func_00225DE8(int, int);

//99.38%
INCLUDE_ASM("luno/lunovm", func_00226810__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static destructor: func_00225DE8(0, 0xFFFF), which destroys luno_gLunoNil.
 *
 * @return func_00225DE8's result register (that function returns nothing)
 */
void* func_00226810(void* self)
{
    return func_00225DE8(0, 0xffff);
}
#endif

