#include "common.h"

INCLUDE_ASM("visualfx/boardtrailfx", cBoardTrailFX_initialize);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E83F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E8560);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E86F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E87E8);

INCLUDE_ASM("visualfx/boardtrailfx", func_002E8938);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA480);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA538);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA670);
#ifdef SKIP_ASM
extern void* D_00487F30[];
extern "C" void* func_00354648(void* self);
extern "C" void func_002EAA28(void* self);

extern "C" void* func_002EA670(void* self)
{
    func_00354648(self);
    *(void***)((char*)self + 0xC) = D_00487F30;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    func_002EAA28(self);
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA6B8);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA780);
#ifdef SKIP_ASM
struct sTrailFade {
    char pad[0x10];
    int state;
    float value;
    float time;
};

extern "C" void func_002EA780(sTrailFade* self, float t)
{
    switch (self->state) {
    case 0:
        return;
    case 1:
        self->time = t;
        self->value = 0.0f;
        break;
    case 2:
        self->value = (self->value / self->time) * t;
        self->time = t;
        break;
    case 3:
        self->value = (1.0f - self->value / self->time) * t;
        self->time = t;
        break;
    }
    self->state = 2;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA820);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA860);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EA8B8);
#ifdef SKIP_ASM
extern "C" void func_002EA8B8(void* self, int up)
{
    int s = *(int*)((char*)self + 0x10);
    if (s == 2 || s == 3) {
        *(int*)((char*)self + 0x1C) = 0;
        return;
    }
    if (up) {
        *(int*)((char*)self + 0x1C) += 1;
        return;
    }
    if (*(int*)((char*)self + 0x1C) > 0) {
        *(int*)((char*)self + 0x1C) -= 1;
    }
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EA900);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAA28);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAAE0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EAC60);

//100%
INCLUDE_ASM("visualfx/boardtrailfx", func_002EADC0__FPv);
#ifdef SKIP_ASM
void func_002EADC0(void* self)
{
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

INCLUDE_ASM("visualfx/boardtrailfx", func_002EADD0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB198);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB8F0);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EB938);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBB10);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBBA8);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBC40);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBE20);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EBF48);

INCLUDE_ASM("visualfx/boardtrailfx", func_002EC060);

