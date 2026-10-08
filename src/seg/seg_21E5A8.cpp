#include "common.h"

INCLUDE_ASM("seg/seg_21E5A8", func_0031D5A8);

//100%
INCLUDE_ASM("seg/seg_21E5A8", func_0031D5E8);
#ifdef SKIP_ASM
struct Flags_D5E8 {
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int pad : 8;
    unsigned int d : 8;
    unsigned int rest : 13;
    int w4;
    int w8;
};
extern "C" Flags_D5E8 *func_0031D5E8(Flags_D5E8 *self) {
    self->w8 = 0;
    self->b = 0;
    self->c = 0;
    self->d = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_21E5A8", func_0031D618);
#ifdef SKIP_ASM
void func_0031D790(void *);
void operator_delete(int *);
extern "C" void func_0031D618(int *self, int flags) {
    func_0031D790(self);
    if (flags & 1) operator_delete(self);
}
#endif
