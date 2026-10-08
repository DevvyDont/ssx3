#include "common.h"

//100%
INCLUDE_ASM("seg/seg_2125C8", cAIAnimEventMap_getBlendInTime);
#ifdef SKIP_ASM
struct sEntry_15C8 {
    char b[0x1C];
};
extern sEntry_15C8 D_00446990[];

extern "C" void *cAIAnimEventMap_getBlendInTime(int arg0) {
    return &D_00446990[arg0];
}
#endif

INCLUDE_ASM("seg/seg_2125C8", func_003115E0);

INCLUDE_ASM("seg/seg_2125C8", func_00311710);

//100%
INCLUDE_ASM("seg/seg_2125C8", func_00311860);
#ifdef SKIP_ASM
extern char D_0048A4C0[];

extern "C" void *func_00311860(void) {
    return D_0048A4C0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_2125C8", func_00311870);
#ifdef SKIP_ASM
extern "C" int cMemMan_alloc(int, char *, int, int);
extern "C" void func_003115E0(int);
extern char D_0048A4D0[];
extern int D_004A3DFC;

extern "C" void func_00311870(void) {
    int temp_2;

    temp_2 = cMemMan_alloc(0x6D4, D_0048A4D0, 0, 0);
    D_004A3DFC = temp_2;
    func_003115E0(temp_2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_2125C8", func_003118A8);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int);
extern int D_004A3DFC;

extern "C" void func_003118A8(void) {
    operator_delete__FPi(D_004A3DFC);
    D_004A3DFC = 0;
}
#endif
