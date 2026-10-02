#include "common.h"

INCLUDE_ASM("scripter/videngine", cVidEngine_ReadyVideo);

INCLUDE_ASM("scripter/videngine", func_002839A8);

//100%
INCLUDE_ASM("scripter/videngine", func_00283AA0);
#ifdef SKIP_ASM
extern "C" void func_00253938(void* self);

extern "C" void func_00283AA0(void* self)
{
    void* p = *(void**)self;
    if (p != 0 && *(int*)((char*)self + 0x4) != 0 && *(int*)((char*)self + 0xC) == 0) {
        func_00253938(p);
        *(int*)((char*)self + 0x8) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283AF8);
#ifdef SKIP_ASM
extern "C" void func_00253890(void* p, int a1);
extern "C" void func_002539E0(void* self);
extern "C" void func_00253A40(void* self);

extern "C" void func_00283AF8(void* self)
{
    void* v = *(void**)((char*)self + 0x0);
    if (v != 0 && *(int*)((char*)self + 0x4) != 0) {
        if (*(int*)((char*)self + 0x8) != 0) {
            if (*(int*)((char*)v + 0x8) != 0) {
                func_00253A40(v);
            }
        } else {
            if (*(int*)((char*)v + 0x8) == 0) {
                func_002539E0(v);
            }
        }
        func_00253890(*(void**)((char*)self + 0x0), *(int*)((char*)self + 0x8) ^ 1);
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283B78);
#ifdef SKIP_ASM
extern "C" void func_00283B78(void* self, int inc)
{
    if (*(int*)((char*)self + 0x0) != 0 && *(int*)((char*)self + 0x4) != 0) {
        if (inc != 0) {
            *(int*)((char*)self + 0xc) += 1;
        } else if (*(int*)((char*)self + 0xc) != 0) {
            *(int*)((char*)self + 0xc) -= 1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283BB8);
#ifdef SKIP_ASM
extern "C" int func_00283BB8(void* self)
{
    int* p = *(int**)((char*)self + 0x0);
    if (p == 0 || *(int*)((char*)self + 0x4) == 0) {
        return 0;
    }
    return *p;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C20__FPv);
#ifdef SKIP_ASM
void func_00283C20(void* self)
{
    *(int*)((char*)self + 0x8c) = 0;
    *(int*)((char*)self + 0x90) = 0;
    *(int*)((char*)self + 0x94) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283C30);
#ifdef SKIP_ASM
void func_00283C20(void*);

extern "C" void* func_00283C30(void* self)
{
    func_00283C20(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C58__FPv);
#ifdef SKIP_ASM
int func_00283C58(void* self)
{
    return *(int*)((char*)self + 0x94);
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283C60);
#ifdef SKIP_ASM
extern "C" int func_00283C60(void* self, int a1)
{
    if (a1 == 4) {
        return 0;
    }
    return a1 + 1;
}
#endif

INCLUDE_ASM("scripter/videngine", func_00283C80);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283D28);
#ifdef SKIP_ASM
int func_00283C58(void*);

extern "C" int func_00283D28(void* self)
{
    int n;
    if (func_00283C58(self) != 0) {
        return 5;
    }
    n = *(int*)((char*)self + 0x90) - *(int*)((char*)self + 0x8c);
    if (n < 0) {
        n += 5;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283D70);
#ifdef SKIP_ASM
extern "C" void* func_00283D70(void* self, int a1)
{
    return (char*)self + ((*(int*)((char*)self + 0x8c) + a1) % 5) * 0x1c;
}
#endif

//100%
INCLUDE_ASM("scripter/videngine", func_00283DA0);
#ifdef SKIP_ASM
extern "C" int func_00283DA0(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x90) == *(int*)((char*)self + 0x8c)) {
        r = *(int*)((char*)self + 0x94) == 0;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/videngine", func_00283DC0);
#ifdef SKIP_ASM
extern "C" int func_00283DA0(void* self);
extern "C" int func_00283C60(void* self, int a1);

struct sVidEntry1C { int v[7]; };

extern "C" int func_00283DC0(void* self, sVidEntry1C* out)
{
    if (func_00283DA0(self) != 0) {
        return 0;
    }
    *(int*)((char*)self + 0x94) = 0;
    if (out != 0) {
        *out = ((sVidEntry1C*)self)[*(int*)((char*)self + 0x8c)];
    }
    *(int*)((char*)self + 0x8c) = func_00283C60(self, *(int*)((char*)self + 0x8c));
    return 1;
}
#endif

