#include "common.h"

INCLUDE_ASM("seg/seg_14B10", cAI_setAIState);

INCLUDE_ASM("seg/seg_14B10", cAI_forceAIState);

INCLUDE_ASM("seg/seg_14B10", func_00113C20);

extern "C" void func_00113CB8(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00113CC0);

extern "C" void func_00113CE0(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00113CE8);

INCLUDE_ASM("seg/seg_14B10", func_00113CF0);

extern "C" void func_00113D10(void) {
}

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113D18);
#ifdef SKIP_ASM
extern "C" void func_00113D18(void *arg0, void *arg1) {
    (*(int *)((char*)(arg1) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
}
#endif

extern "C" void func_00113D28(void) {
}

extern "C" void func_00113D30(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00113D38);

INCLUDE_ASM("seg/seg_14B10", func_00113D48);

INCLUDE_ASM("seg/seg_14B10", func_00113D80);

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113D88);
#ifdef SKIP_ASM
extern "C" void func_00258988(int);
extern int D_004A2EEC;

extern "C" void func_00113D88(void) {
    if (D_004A2EEC != 0) {
        func_00258988(D_004A2EEC);
    }
}
#endif

INCLUDE_ASM("seg/seg_14B10", func_00113DB0);

//100%
INCLUDE_ASM("seg/seg_14B10", func_00113E18);
#ifdef SKIP_ASM
extern "C" void func_00258A48(int);
extern int D_004A2EEC;

extern "C" void func_00113E18(void) {
    if (D_004A2EEC != 0) {
        func_00258A48(D_004A2EEC);
    }
}
#endif

extern "C" void func_00113E40(void) {
}

extern "C" void func_00113E48(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00113E50);

extern "C" void func_00113E70(void) {
}

extern "C" void func_00113E78(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00113E80);

INCLUDE_ASM("seg/seg_14B10", func_00113F38);

INCLUDE_ASM("seg/seg_14B10", func_00113F88);

INCLUDE_ASM("seg/seg_14B10", func_00114130);

INCLUDE_ASM("seg/seg_14B10", func_00114298);

INCLUDE_ASM("seg/seg_14B10", func_00114CC0);

INCLUDE_ASM("seg/seg_14B10", func_00114DB8);

INCLUDE_ASM("seg/seg_14B10", func_00115168);

INCLUDE_ASM("seg/seg_14B10", func_00115358);

INCLUDE_ASM("seg/seg_14B10", func_00115640);

INCLUDE_ASM("seg/seg_14B10", func_001158B8);

//100%
INCLUDE_ASM("seg/seg_14B10", func_00115AB0);
#ifdef SKIP_ASM
extern "C" int cBE_getBE();
extern "C" int cBE_getInterface__Fv(int, int);
extern "C" float func_0014EFA8(int, int);

extern "C" float func_00115AB0(void *arg0) {
    return (func_0014EFA8(cBE_getInterface__Fv(cBE_getBE(), 2), (*(int *)((char*)(arg0) + (0x86C)))) * -90.0f) + 75.0f;
}
#endif

//100%
INCLUDE_ASM("seg/seg_14B10", func_00115B08);
#ifdef SKIP_ASM
extern "C" int cBE_getBE();
extern "C" int cBE_getInterface__Fv(int, int);
extern "C" float func_0014EFA8(int, int);

extern "C" float func_00115B08(void *arg0) {
    return (func_0014EFA8(cBE_getInterface__Fv(cBE_getBE(), 2), (*(int *)((char*)(arg0) + (0x86C)))) * 90.0f) - 90.0f;
}
#endif

INCLUDE_ASM("seg/seg_14B10", func_00115B58);

INCLUDE_ASM("seg/seg_14B10", func_00115D48);

INCLUDE_ASM("seg/seg_14B10", func_00116120);

INCLUDE_ASM("seg/seg_14B10", func_001161D0);

INCLUDE_ASM("seg/seg_14B10", func_001162C8);

INCLUDE_ASM("seg/seg_14B10", func_00116378);

INCLUDE_ASM("seg/seg_14B10", func_001163B0);

extern "C" void func_00116930(void) {
}

INCLUDE_ASM("seg/seg_14B10", func_00116938);

INCLUDE_ASM("seg/seg_14B10", gGenTrickName);

INCLUDE_ASM("seg/seg_14B10", gGenMonsterTrickReqName);

INCLUDE_ASM("seg/seg_14B10", func_00116FA8);

INCLUDE_ASM("seg/seg_14B10", func_00116FB8);

INCLUDE_ASM("seg/seg_14B10", func_00117008);

INCLUDE_ASM("seg/seg_14B10", func_00117048);

INCLUDE_ASM("seg/seg_14B10", func_001170A8);

INCLUDE_ASM("seg/seg_14B10", func_00117138);

INCLUDE_ASM("seg/seg_14B10", func_001171A8);

INCLUDE_ASM("seg/seg_14B10", func_00117248);

//100%
INCLUDE_ASM("seg/seg_14B10", func_00117290);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);
extern "C" void operator_delete__FPi(void *);

extern "C" void func_00117290(void *arg0, int arg1) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (0x1B0)));
    if (temp_4 != 0) {
        cMemMan_free__FPv(temp_4);
    }
    if (arg1 & 1) {
        operator_delete__FPi(arg0);
    }
}
#endif
