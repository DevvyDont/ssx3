#include "common.h"

INCLUDE_ASM("ui/uianimation", cUIAnimationBank_getAnimationByHashName);

//100%
INCLUDE_ASM("ui/uianimation", func_003971C8);
#ifdef SKIP_ASM
struct func_003971C8_sKey {
    unsigned short dt;
    short value;
};

struct func_003971C8_sAnim {
    char pad[6];
    unsigned short count;
    func_003971C8_sKey keys[1];
};

extern "C" float func_003971C8(func_003971C8_sAnim** self, int t)
{
    func_003971C8_sAnim* a = *self;
    if (a == 0) {
        return 0.0f;
    }
    unsigned int i;
    int prevT = 0;
    float prevV = a->keys[0].value;
    int accT = 0;
    float v = 0.0f;
    func_003971C8_sKey* k = &a->keys[1];
    for (i = 1; i < a->count; i++, k++) {
        int dt = k->dt;
        accT += dt;
        v = k->value;
        if (accT >= t) {
            return (float)(t - prevT) / (float)dt * (v - prevV) + prevV;
        }
        prevT = accT;
        prevV = v;
    }
    return v;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", func_00397278);
#ifdef SKIP_ASM
extern "C" unsigned short func_00397278(void* self)
{
    void* p = *(void**)self;
    if (p != 0) {
        return *(unsigned short*)((char*)p + 0x2);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", func_00397298__FPvi);
#ifdef SKIP_ASM
void func_00397298(void* self, int val)
{
    *(int*)((char*)self + 0x0) = val;
}
#endif

INCLUDE_ASM("ui/uianimation", cUIAnimation_cUIAnimation);

INCLUDE_ASM("ui/uianimation", func_003973A8);

//100%
INCLUDE_ASM("ui/uianimation", func_00397468);
#ifdef SKIP_ASM
extern "C" void func_00397468(void* self, int mode, unsigned short v)
{
    mode &= 7;
    if (mode == 0) {
        *(unsigned short*)((char*)self + 0xc) = v;
    }
    if (mode == 2) {
        *(unsigned char*)((char*)self + 0xe) = 1;
        *(unsigned short*)((char*)self + 0xa) = *(unsigned short*)((char*)*(void**)self + 0x2) - 1;
    } else {
        *(unsigned char*)((char*)self + 0xe) = 0;
        *(unsigned short*)((char*)self + 0xa) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uianimation", func_003974B0);
#ifdef SKIP_ASM
struct func_003974B0_sData {
    short pad0;
    unsigned short count;
};

struct func_003974B0_sAnim {
    func_003974B0_sData* data;
    char pad4[6];
    short frame;
    short padc;
    signed char dir;
};

extern "C" void func_003974B0(func_003974B0_sAnim* self, unsigned char flags)
{
    func_003974B0_sData* data = self->data;
    int mode;
    if (data == 0) {
        return;
    }
    mode = flags & 7;
    if (mode == 0) {
        return;
    }
    switch (mode) {
    case 2:
        self->frame--;
        if (self->frame < 0) {
            if (flags & 8) {
                self->frame = data->count - 1;
            } else {
                self->frame = 0;
            }
        }
        break;
    case 1:
        self->frame++;
        if (self->frame >= data->count) {
            if (!(flags & 8)) {
                self->frame = data->count - 1;
            } else {
                self->frame = 0;
            }
        }
        break;
    case 3:
        if (self->dir == 0) {
            self->frame++;
        } else {
            self->frame--;
        }
        if (self->dir == 0) {
            if (self->frame >= self->data->count) {
                self->dir = 1;
                self->frame = self->data->count - 1;
                return;
            }
        }
        if (self->dir == 1 && self->frame < 0) {
            self->dir = 0;
            self->frame = 0;
        }
        break;
    }
}
#endif

INCLUDE_ASM("ui/uianimation", func_003975E0);

