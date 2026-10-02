#include "common.h"

INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onCreateScreen);

INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8448);

INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F85C0);

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8720__FPv);
#ifdef SKIP_ASM
void func_001F8720(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8728);

extern "C" void* func_0020A430(void*);

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F88E8__FPv);
#ifdef SKIP_ASM
void* func_001F88E8(void* self)
{
    *(int*)((char*)self + 0xc4) = 0;
    return func_0020A430(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8908);
#ifdef SKIP_ASM
extern "C" int func_001F8908(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 8:
    case 9:
        return 0x100;
    case 6:
        return 0;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/ovtemplatepausemenu", cOVTemplate_PauseMenu_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8DF0__FPv);
#ifdef SKIP_ASM
int func_001F8DF0(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8DF8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8EE0);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478160[];

extern "C" void func_001F8EE0(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 6;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478160[i];
    }
    *(int*)((char*)self + 0x110) = 1;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8F48);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478178[];

extern "C" void func_001F8F48(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 6;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478178[i];
    }
    *(int*)((char*)self + 0x110) = 2;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F8FB0);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478140[];

extern "C" void func_001F8FB0(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 7;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478140[i];
    }
    *(int*)((char*)self + 0x110) = 3;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F9018);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_00478190[];

extern "C" void func_001F9018(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 5;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_00478190[i];
    }
    *(int*)((char*)self + 0x110) = 4;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F9080);
#ifdef SKIP_ASM
extern "C" void cOVTemplate_PauseMenu_onCreateScreen(void* self);
extern int D_004781A8[];

extern "C" void func_001F9080(void* self)
{
    int i;
    *(int*)((char*)self + 0xBC) = 5;
    for (i = 0; i < *(int*)((char*)self + 0xBC); i++) {
        ((int*)((char*)self + 0x9C))[i] = D_004781A8[i];
    }
    *(int*)((char*)self + 0x110) = 5;
    cOVTemplate_PauseMenu_onCreateScreen(self);
}
#endif

INCLUDE_ASM("fe/ovtemplatepausemenu", func_001F90E8);

