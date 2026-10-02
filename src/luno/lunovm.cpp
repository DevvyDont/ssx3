#include "common.h"

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

struct cLunoTable {
    char pad_0x00[0x8];
    int refCount;
    int modulus;
    cLunoTableEntry* nodes;
};

// Table reference: word0 of a type-4 value.
struct cLunoTableRef {
    cLunoTable* table;
};

void operator_delete(int* ptr);
void cMemMan_free(void* ptr);
extern "C" void func_00224F30(cLunoTableRef* ref);
extern "C" void func_00224DF0(cLunoTableRef* ref, int flags);

INCLUDE_ASM("luno/lunovm", luno_cLunoVM_execute);

//100%
INCLUDE_ASM("luno/lunovm", func_00224C00);
#ifdef SKIP_ASM
struct sVEntry00224C00 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00224C00(void* self, sVEntry00224C00** stream, int base)
{
    int off;
    int v;
    off = *(int*)((char*)self + 0x0) - base;
    (*stream)[1].fn((char*)stream + (*stream)[1].delta, &off, 4);
    v = *(int*)((char*)self + 0x4);
    (*stream)[1].fn((char*)stream + (*stream)[1].delta, &v, 4);
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00224C78);
#ifdef SKIP_ASM
struct sVEntry00224C78 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00224C78(void* self, sVEntry00224C78** stream, int base)
{
    int buf[2];
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, &buf[0], 4);
    *(int*)((char*)self + 0x0) = base + buf[0];
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, &buf[1], 4);
    *(int*)((char*)self + 0x4) = buf[1];
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00224D00);
#ifdef SKIP_ASM
extern "C" void func_00224D00(cLunoTable* self, int flags)
{
    cLunoTableEntry* nodes = self->nodes;

    if (nodes != 0) {
        cLunoTableEntry* e = nodes + ((int*)nodes)[-4];

        while (self->nodes != e) {
            e--;
            e->vtable[1].func((char*)e + e->vtable[1].delta, 0);
        }
        cMemMan_free((char*)self->nodes - 0x10);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00224DA0);
#ifdef SKIP_ASM
extern "C" void luno_cLunoTable_constructTable(cLunoTableRef* self);

extern "C" cLunoTableRef* func_00224DA0(cLunoTableRef* self)
{
    self->table = 0;
    luno_cLunoTable_constructTable(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00224DF0);
#ifdef SKIP_ASM
extern "C" void func_00224DF0(cLunoTableRef* self, int flags)
{
    if (--self->table->refCount == 0) {
        func_00224F30(self);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("luno/lunovm", luno_cLunoTable_constructTable);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("luno/lunovm", func_00224F30);
#ifdef SKIP_ASM
extern "C" void func_00224D00(cLunoTable* table, int flags);

extern "C" void func_00224F30(cLunoTableRef* ref)
{
    cLunoTable* t = ref->table;
    if (t != 0) {
        func_00224D00(t, 3);
    }
    ref->table = 0;
}
#endif

INCLUDE_ASM("luno/lunovm", func_00224F68);

//100%
INCLUDE_ASM("luno/lunovm", func_00225068);
#ifdef SKIP_ASM
extern "C" cLunoTableRef* func_00225068(cLunoTableRef* self, cLunoTableRef* other)
{
    if (self->table != other->table) {
        if (self->table != 0) {
            if (--self->table->refCount == 0) {
                func_00224F30(self);
            }
        }
        self->table = other->table;
        self->table->refCount++;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00225248);
#ifdef SKIP_ASM
extern cLunoValue D_004C9098;

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
    if (a->type == 2 && b->type == 1) {
        return *(float*)&a->word0 == (float)b->word0;
    }
    if (a->type == 1 && b->type == 2) {
        return a->word0 == (int)*(float*)&b->word0;
    }
    if (a->type == 3 && b->type == 1) {
        return a->word0 == b->word0;
    }
    return 0;
}

// Table lookup: returns the value stored under key, or the nil value.
extern "C" cLunoValue* func_00225248(cLunoTableRef* self, cLunoValue* key)
{
    cLunoTable* t = self->table;
    cLunoNode* n = (cLunoNode*)t->nodes + (unsigned int)key->word0 % (unsigned int)t->modulus;
    if (n->value.type != 0) {
        do {
            if (lunoKeyEquals(&n->key, key)) {
                return &n->value;
            }
            n = n->next;
        } while (n != 0);
    }
    return &D_004C9098;
}
#endif

INCLUDE_ASM("luno/lunovm", luno_cLunoTable_set);

INCLUDE_ASM("luno/lunovm", func_002257E0);

INCLUDE_ASM("luno/lunovm", func_002259A8);

//100%
INCLUDE_ASM("luno/lunovm", func_00225B90);
#ifdef SKIP_ASM
extern "C" void func_00225B90(cLunoValue* self, int flags)
{
    if (self->type == 4) {
        func_00224DF0((cLunoTableRef*)self, 2);
    }
    self->type = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("luno/lunovm", func_00225BE8);
#ifdef SKIP_ASM
extern "C" void func_00224C00(void* self, sVEntry00224C00** stream, int base);
extern "C" void func_002257E0(void* self, sVEntry00224C00** stream, int base);

extern "C" void func_00225BE8(cLunoValue* self, sVEntry00224C00** stream, int base)
{
    (*stream)[1].fn((char*)stream + (*stream)[1].delta, &self->type, 4);
    if (self->type == 5) {
        func_00224C00(self, stream, base);
    } else if (self->type == 4) {
        func_002257E0(self, stream, base);
    } else {
        (*stream)[1].fn((char*)stream + (*stream)[1].delta, self, 4);
    }
}
#endif

INCLUDE_ASM("luno/lunovm", func_00225CA0);

INCLUDE_ASM("luno/lunovm", func_00225DE8);

//100%
INCLUDE_ASM("luno/lunovm", func_00226600__FPvii);
#ifdef SKIP_ASM
void func_00226600(void* self, int a1, int a2)
{
    *(int*)self = a1;
    *(int*)((char*)self + 0x4) = a2;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00226610__FPv);
#ifdef SKIP_ASM
void* func_00226610(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00226618__FPv);
#ifdef SKIP_ASM
void* func_00226618(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00226620__FPv);
#ifdef SKIP_ASM
void* func_00226620(void* self)
{
    return self;
}
#endif

extern cLunoValue D_004C9098;

//100%
INCLUDE_ASM("luno/lunovm", func_00226628);
#ifdef SKIP_ASM
// Value add (for-loop step): same-type int/float/symbol add, float+int
// gives float, int+float gives int; anything else gives nil.
extern "C" cLunoValue* func_00226628(cLunoValue* self, const cLunoValue& a, const cLunoValue& b)
{
    if (b.type == a.type) {
        if (a.type == 2) {
            float f = *(float*)&a.word0 + *(float*)&b.word0;
            self->type = a.type;
            *(float*)func_00226618(self) = f;
        } else if (a.type == 1) {
            int i = a.word0 + b.word0;
            self->type = a.type;
            *(int*)func_00226610(self) = i;
        } else if (a.type == 3) {
            int i = a.word0 + b.word0;
            self->type = a.type;
            *(int*)func_00226620(self) = i;
        } else {
            *self = D_004C9098;
        }
    } else if (a.type == 2 && b.type == 1) {
        float f = *(float*)&a.word0 + (float)b.word0;
        self->type = a.type;
        *(float*)func_00226618(self) = f;
    } else if (a.type == 1 && b.type == 2) {
        int i = a.word0 + (int)*(float*)&b.word0;
        self->type = a.type;
        *(int*)func_00226610(self) = i;
    } else {
        *self = D_004C9098;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("luno/lunovm", func_00226768);
#ifdef SKIP_ASM
struct sVEntry00226768 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
extern char D_00479890[];

extern "C" void func_00226768(void* self, int flags)
{
    *(void**)((char*)self + 0x1C) = D_00479890;
    void* obj = *(void**)((char*)self + 0x18);
    if (obj != 0) {
        sVEntry00226768* vt = *(sVEntry00226768**)((char*)obj + 0x1C);
        vt[1].fn((char*)obj + vt[1].delta, 3);
    }
    func_00225B90((cLunoValue*)((char*)self + 0xC), 2);
    func_00225B90((cLunoValue*)self, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* func_00225DE8(int, int);

//99.38%
INCLUDE_ASM("luno/lunovm", func_002267F0__FPv);
#ifdef SKIP_ASM
void* func_002267F0(void* self)
{
    return func_00225DE8(1, 0xffff);
}
#endif

extern "C" void* func_00225DE8(int, int);

//99.38%
INCLUDE_ASM("luno/lunovm", func_00226810__FPv);
#ifdef SKIP_ASM
void* func_00226810(void* self)
{
    return func_00225DE8(0, 0xffff);
}
#endif

