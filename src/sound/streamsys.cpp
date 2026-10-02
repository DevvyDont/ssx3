#include "common.h"

INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance);

INCLUDE_ASM("sound/streamsys", func_002A9DF0);

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

INCLUDE_ASM("sound/streamsys", func_002A9F80);

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

INCLUDE_ASM("sound/streamsys", func_002AA320);

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

INCLUDE_ASM("sound/streamsys", func_002AAC28);

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

INCLUDE_ASM("sound/streamsys", func_002AB7A0);

INCLUDE_ASM("sound/streamsys", func_002AB828);

INCLUDE_ASM("sound/streamsys", func_002AB8E8);

INCLUDE_ASM("sound/streamsys", func_002AB958);

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

INCLUDE_ASM("sound/streamsys", func_002AC180);

INCLUDE_ASM("sound/streamsys", func_002AC220);

INCLUDE_ASM("sound/streamsys", func_002AC7F0);

INCLUDE_ASM("sound/streamsys", func_002AC868);

INCLUDE_ASM("sound/streamsys", func_002ACAC8);

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

