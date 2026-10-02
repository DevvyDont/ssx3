#include "common.h"

INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance);

//100%
INCLUDE_ASM("sound/streamsys", func_002A9DF0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" int func_003B7838(int h);
extern "C" void* func_002523A8(void* self);

extern "C" void func_002A9DF0(void* self, int flags)
{
    if (*(void**)self != 0) {
        func_003B7838(*(int*)((char*)self + 0x8));
        if (*(int*)((char*)self + 0x4) == 0 && *(void**)self != 0) {
            cMemMan_free(*(void**)self);
        }
    }
    if (*(void**)((char*)self + 0x74) != 0) {
        func_002523A8(*(void**)((char*)self + 0x74));
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9E78);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" int func_003E1EC8(const char* name, int a1);

// PORT: second parameter is a const char* passed as int (unit declares func_002A9E78(void*, int)).
extern "C" void func_002A9E78(void* self, int name)
{
    if (BXFILE_exists((const char*)name) != 0) {
        strcpy((char*)self + 0x14, (const char*)name);
        *(int*)((char*)self + 0x74) = func_003E1EC8((char*)self + 0x14, *(int*)((char*)self + 0x7C));
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9ED8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7F60(int h, int a, int b);

extern "C" void func_002A9ED8(void* self, int a, int b)
{
    func_003B58A0();
    func_003B7F60(*(int*)((char*)self + 0x8), a, b);
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9F30__FPvi);
#ifdef SKIP_ASM
void func_002A9F30(void* self, int val)
{
    *(char*)((char*)self + 0xC) = (char)val;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9F38);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7FB8(int h, int v);

extern "C" void func_002A9F38(void* self, int v)
{
    func_003B58A0();
    func_003B7FB8(*(int*)((char*)self + 0x8), v);
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9F80);
#ifdef SKIP_ASM
int func_0028B240(void* self);
void* func_002AA408(void* self);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7E70(int h, int vol);

extern "C" void func_002A9F80(void* self)
{
    if (func_0028B240(self) * 2 < *(int*)((char*)self + 0x80)) {
        func_002AA408(self);
        *(int*)((char*)self + 0x80) = 0;
        return;
    }
    float* p = *(float**)((char*)self + 0x10);
    if (p != 0) {
        int v = (int)((float)*(signed char*)((char*)self + 0xC) * *p);
        if (v > 0x7F) {
            v = 0x7F;
        }
        if (v < 0) {
            v = 0;
        }
        func_003B58A0();
        func_003B7E70(*(int*)((char*)self + 0x8), v);
        func_003B58D8();
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA020);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7B48(int h, void* out);

// PORT: returns int; the unit declares it void(void*, void*) further down
int func_002AA020_impl(void* self, void* out) __asm__("func_002AA020");

int func_002AA020_impl(void* self, void* out)
{
    int r;
    func_003B58A0();
    r = func_003B7B48(*(int*)((char*)self + 0x8), out);
    func_003B58D8();
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA068);
#ifdef SKIP_ASM
extern "C" void func_002AA020(void*, void*);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7C40(int h, int a1);

// PORT: returns int and takes (self, int, out); later callers in this unit declare it void(void*, sStatus*, int)
int func_002AA068_impl(void* self, int a1, int* out) __asm__("func_002AA068");

int func_002AA068_impl(void* self, int a1, int* out)
{
    int buf[4];
    int* p = out ? out : buf;
    int r;
    func_002AA020(self, p);
    func_003B58A0();
    r = func_003B7C40(p[1], a1);
    func_003B58D8();
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA108);
#ifdef SKIP_ASM
struct func_002AA1B8_sStatus;
extern "C" void func_002AA068(void* self, func_002AA1B8_sStatus* out, int flags);

struct func_002AA108_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" float func_002AA108(void* self)
{
    func_002AA108_sStatus st;
    func_002AA068(self, (func_002AA1B8_sStatus*)&st, 0);
    if (st.state == 2) {
        return (float)st.b;
    }
    return 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA1B8);
#ifdef SKIP_ASM
struct func_002AA1B8_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" void func_002AA068(void* self, func_002AA1B8_sStatus* out, int flags);

extern "C" int func_002AA1B8(void* self)
{
    func_002AA1B8_sStatus st;
    func_002AA068(self, &st, 0);
    return st.state == 2;
}
#endif

INCLUDE_ASM("sound/streamsys", func_002AA210);

//100%
INCLUDE_ASM("sound/streamsys", func_002AA320);
#ifdef SKIP_ASM
extern "C" int func_003B78D8(int h, int a, int buf, int off);
extern "C" void func_003E2CE0(void* dec, int src, int a, int* out, int b);

// PORT: buf is a pointer carried in an int (the unit's caller declares this function with int params).
extern "C" int func_002AA320(void* self, int a, int buf, int off)
{
    if (*(void**)((char*)self + 0x74) != 0) {
        int n;
        func_003E2CE0(*(void**)((char*)self + 0x74), buf, 0, &n, 0);
        return func_003B78D8(*(int*)((char*)self + 0x8), a, (int)((char*)self + 0x14), off + n);
    }
    return func_003B78D8(*(int*)((char*)self + 0x8), a, buf, off);
}
#endif

extern "C" void* func_003B78F8(int);

//100%
INCLUDE_ASM("sound/streamsys", func_002AA408__FPv);
#ifdef SKIP_ASM
void* func_002AA408(void* self)
{
    return func_003B78F8(*(int*)((char*)self + 0x8));
}
#endif

extern "C" void func_002AA020(void*, void*);

//99.38% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("sound/streamsys", func_002AA428);
#ifdef SKIP_ASM
extern "C" int func_002AA428(void* self)
{
    int buf[4];
    func_002AA020(self, buf);
    return buf[0];
}
#endif

INCLUDE_ASM("sound/streamsys", cStreamSys_cStreamSys);

INCLUDE_ASM("sound/streamsys", func_002AA648);

INCLUDE_ASM("sound/streamsys", func_002AA7F0);

INCLUDE_ASM("sound/streamsys", func_002AA910);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAB98);
#ifdef SKIP_ASM
extern "C" void func_002A9E78(void*, int);

extern "C" void func_002AAB98(void* self, int i, int val)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        func_002A9E78(e, val);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AABD0);
#ifdef SKIP_ASM
extern "C" void func_002A9DF0(void* e, int a1);

extern "C" void func_002AABD0(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        func_002A9DF0(e, 3);
    }
    (*(void***)((char*)self + 0x8))[i] = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAC28);
#ifdef SKIP_ASM
extern "C" void func_002A9F80(void* e);

extern "C" void func_002AAC28(void* self)
{
    for (int i = 0; i < *(int*)((char*)self + 0x4); i++) {
        void* e = (*(void***)((char*)self + 0x8))[i];
        if (e != 0) {
            func_002A9F80(e);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAD78);
#ifdef SKIP_ASM
extern "C" int func_002AA320(void* e, int a, int b, int c);

extern "C" int func_002AAD78(void* self, int i, int a, int b, int c)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        return func_002AA320(e, a, b, c);
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAE08);
#ifdef SKIP_ASM
extern "C" int func_002AA428(void*);

extern "C" int func_002AAE08(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        return func_002AA428(e);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AAE40);
#ifdef SKIP_ASM
extern "C" void func_002AAE40(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        func_002AA408(e);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAE78);
#ifdef SKIP_ASM
extern "C" float func_002AA108(void* self);

extern "C" float func_002AAE78(void* self, int i)
{
    void* e;
    if (i < 0 || (e = (*(void***)((char*)self + 0x8))[i]) == 0) {
        return 0.0f;
    }
    return func_002AA108(e);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB028);
#ifdef SKIP_ASM
extern "C" int func_002AA1B8(void*);

extern "C" int func_002AB028(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        return func_002AA1B8(e);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB0D0);
#ifdef SKIP_ASM
extern "C" void func_002AB0D0(void* self, int i, int val)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        *(int*)((char*)e + 0x10) = val;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB150);
#ifdef SKIP_ASM
extern "C" int func_002AA210(void*);

extern "C" int func_002AB150(void* self, int i)
{
    void* e = (*(void***)((char*)self + 0x8))[i];
    if (e != 0) {
        return func_002AA210(e);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB188__FPvi);
#ifdef SKIP_ASM
int func_002AB188(void* self, int a1)
{
    return *(int*)((char*)*(void**)((char*)self + 0x8) + a1 * 4);
}
#endif

INCLUDE_ASM("sound/streamsys", func_002AB200);

INCLUDE_ASM("sound/streamsys", func_002AB478);

INCLUDE_ASM("sound/streamsys", func_002AB6B0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB7A0);
#ifdef SKIP_ASM
extern "C" void func_002AB6B0(void* voice, void* self, float v);

extern "C" void func_002AB7A0(void* self, float v)
{
    if (*(int*)(**(char***)((char*)self + 0x118) + 0xAC0) != 0) {
        v = 0.0f;
    }
    for (int i = 0; i < 64; i++) {
        func_002AB6B0(*(char**)(**(char***)((char*)self + 0x118) + 0xAC4) + i * 0xC0, self, v);
    }
}
#endif

INCLUDE_ASM("sound/streamsys", func_002AB828);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB8E8);
#ifdef SKIP_ASM
extern "C" void func_002ADC10(void* bm);
extern "C" void func_002AB828(void* e, void* self);

extern "C" void func_002AB8E8(void* self)
{
    char* snd = **(char***)((char*)self + 0x118);
    char* bm = snd + 0x1D8;
    if (*(int*)(snd + 0x26C) == 0) {
        func_002ADC10(bm);
        for (int i = 0; i < 48; i++) {
            func_002AB828(**(char***)((char*)self + 0x118) + 0x270 + i * 0x2C, self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB958);
#ifdef SKIP_ASM
extern "C" void func_0028AAF8(void* snd);
extern "C" void func_002A6D18(void* self);
extern "C" void func_002AAC28(void* self);
extern "C" void func_002B0C78(void* self);
extern "C" void func_002B4388(void* self);

struct sStrmVtEntF { short delta; short index; void (*fn)(void*, float); };

extern "C" void func_002AB958(void* self, float v)
{
    char** sys = (char**)((char*)self + 0x118);
    func_0028AAF8(*(void**)*sys);
    func_002A6D18(self);
    char* b = *(char**)*sys;
    char* o = b + 0x1D8;
    sStrmVtEntF* e = &(*(sStrmVtEntF**)(b + 0xAB0))[5];
    e->fn(o + e->delta, v);
    func_002B4388(sys);
    func_002B0C78((char*)self + 0x5560);
    func_002AAC28(*sys);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB9E0);
#ifdef SKIP_ASM
extern "C" void* func_002AB9E0(void* self)
{
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(float*)((char*)self + 0x90) = -1.0f;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0xB8) = -30000;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABA18);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002ABA18(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

INCLUDE_ASM("sound/streamsys", func_002ABA40);

//100%
INCLUDE_ASM("sound/streamsys", func_002ABB38);
#ifdef SKIP_ASM
extern "C" void func_002AC180(void*);

extern "C" void func_002ABB38(void* self)
{
    func_002AC180(self);
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0xB8) = 0xFFFF;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002ABB80);
#ifdef SKIP_ASM
extern "C" int func_002ABA40(void* self);
extern "C" void func_002ABB38(void* self);

extern "C" int func_002ABB80(void* self)
{
    if (*(int*)self == 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x9C) == 0) {
        return 0;
    }
    if (func_002ABA40(self) == 0) {
        return 0;
    }
    func_002ABB38(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABC18);
#ifdef SKIP_ASM
// PORT: the unit declares func_002ABC18 as void*(int); the body takes (int, float, float).
int func_002ABC18_impl(int vol, float dist, float range) __asm__("func_002ABC18");

int func_002ABC18_impl(int vol, float dist, float range)
{
    dist = dist - 0.5f;
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    int result = 0;
    if (dist < range && 0.0f < range) {
        float t = (range - dist) / range;
        result = (int)(t * t * (float)vol);
    }
    return result;
}
#endif

extern "C" void* func_002ABC18(int);

//99.29%
INCLUDE_ASM("sound/streamsys", func_002ABC80__FPvi);
#ifdef SKIP_ASM
void* func_002ABC80(void* self, int a1)
{
    return func_002ABC18(a1);
}
#endif

INCLUDE_ASM("sound/streamsys", func_002ABCA0);

//100%
INCLUDE_ASM("sound/streamsys", func_002ABCE8__FPvii);
#ifdef SKIP_ASM
void func_002ABCE8(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABCF8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_002ABCF8(void* self, int a, const char* name, int b)
{
    *(int*)((char*)self + 0xC) = a;
    strcpy((char*)self + 0x10, name);
    *(int*)((char*)self + 0x50) = b;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABD38__FPvii);
#ifdef SKIP_ASM
void func_002ABD38(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABD48);
#ifdef SKIP_ASM
class func_002ABD48_cObj {
public:
    char pad[0x8D8];
    // vptr at 0x8D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float v06(void* a, void* out);
};

extern "C" void func_002ABD48(void* self, func_002ABD48_cObj* obj)
{
    char buf[0x20];
    float d;
    float d2 = obj->v06(self, buf);
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    *(float*)((char*)self + 0x5C) = d;
}
#endif

INCLUDE_ASM("sound/streamsys", func_002ABD90);

INCLUDE_ASM("sound/streamsys", func_002ABE78);

INCLUDE_ASM("sound/streamsys", func_002ABF60);

//100%
INCLUDE_ASM("sound/streamsys", func_002AC180);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B8530(int h, int a, int b);
extern "C" void func_003B9F38(int h, int a, int b);

extern "C" void func_002AC180(void* self)
{
    if (*(int*)((char*)self + 0x54) != 0) {
        return;
    }
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x54) = 1;
    switch (*(int*)self) {
    case 1:
        func_003B58A0();
        func_003B8530(*(int*)((char*)self + 0x4), 0x19, -1);
        func_003B58D8();
        break;
    case 2:
        func_003B58A0();
        func_003B9F38(*(int*)((char*)self + 0x4), 0x19, -1);
        func_003B58D8();
        break;
    }
}
#endif

INCLUDE_ASM("sound/streamsys", func_002AC220);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AC7F0);
#ifdef SKIP_ASM
extern "C" void func_002AC180(void*);
extern "C" void func_002AC220(void*);

extern "C" int func_002AC7F0(void* self, float t)
{
    float th = *(float*)((char*)self + 0x8C);
    if (th == 0.0f) {
        return 1;
    }
    if (*(int*)((char*)self + 0x8) != 0) {
        if (th < t) {
            func_002AC180(self);
            return 0;
        }
        return 1;
    }
    if (t < th) {
        func_002AC220(self);
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/streamsys", func_002AC868);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002ACAC8);
#ifdef SKIP_ASM
extern "C" int func_002ABB80(void* self);
extern "C" void func_002ABE78(void* self, int a1);
extern "C" void func_002ABF60(void* self, int a1);
extern "C" void func_003B9D00(int h);

extern "C" void func_002ACAC8(void* self)
{
    if (func_002ABB80(self) == 0 && *(int*)((char*)self + 0x54) == 0) {
        func_002ABE78(self, 0);
        func_002ABF60(self, 0);
        if (*(int*)((char*)self + 0x0) == 2) {
            func_003B9D00(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002ACB30);
#ifdef SKIP_ASM
extern "C" int func_002ABB80(void* self);
extern "C" void func_002ABE78(void* self, int a1);
extern "C" void func_003B9DC8(int h);

extern "C" void func_002ACB30(void* self)
{
    if (func_002ABB80(self) == 0) {
        func_002ABE78(self, 0x1000);
        if (*(int*)self == 2) {
            func_003B9DC8(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

INCLUDE_ASM("sound/streamsys", func_002ACB80);

