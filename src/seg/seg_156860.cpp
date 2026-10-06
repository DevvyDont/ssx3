#include "common.h"

INCLUDE_ASM("seg/seg_156860", cCommSystem_construct);

//100%
INCLUDE_ASM("seg/seg_156860", func_00255898);
#ifdef SKIP_ASM
extern "C" void cCommSystem__cCommSystem(int, int);
extern int D_004A2EB8;

extern "C" void func_00255898(void) {
    if (D_004A2EB8 != 0) {
        cCommSystem__cCommSystem(D_004A2EB8, 3);
        D_004A2EB8 = 0;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", cCommSystem_cCommSystem);

INCLUDE_ASM("seg/seg_156860", cCommSystem__cCommSystem);

INCLUDE_ASM("seg/seg_156860", cCommSystem_getSignature);

INCLUDE_ASM("seg/seg_156860", func_00255A20);

INCLUDE_ASM("seg/seg_156860", cCommSystem_openDSockChannel);

INCLUDE_ASM("seg/seg_156860", func_00255CC8);

INCLUDE_ASM("seg/seg_156860", func_00255D38);

INCLUDE_ASM("seg/seg_156860", func_00255D50);

INCLUDE_ASM("seg/seg_156860", func_00255D88);

INCLUDE_ASM("seg/seg_156860", func_00255DC0);

INCLUDE_ASM("seg/seg_156860", func_00255E00);

INCLUDE_ASM("seg/seg_156860", func_00255E40);

INCLUDE_ASM("seg/seg_156860", func_00255E68);

INCLUDE_ASM("seg/seg_156860", func_00255EC0);

INCLUDE_ASM("seg/seg_156860", func_00255F20);

INCLUDE_ASM("seg/seg_156860", func_00255F50);

INCLUDE_ASM("seg/seg_156860", func_00255FA0);

INCLUDE_ASM("seg/seg_156860", func_00255FE8);

INCLUDE_ASM("seg/seg_156860", func_00256038);

INCLUDE_ASM("seg/seg_156860", func_00256088);

INCLUDE_ASM("seg/seg_156860", func_002560B0);

INCLUDE_ASM("seg/seg_156860", func_00256188);

//100%
INCLUDE_ASM("seg/seg_156860", func_002561B8);
#ifdef SKIP_ASM
extern "C" void func_00256228();

extern "C" int func_002561B8(int arg0) {
    func_00256228();
    return arg0;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002561E0);

INCLUDE_ASM("seg/seg_156860", func_00256228);

extern "C" void func_00256250(void) {
}

INCLUDE_ASM("seg/seg_156860", func_00256258);

INCLUDE_ASM("seg/seg_156860", func_002562D0);

INCLUDE_ASM("seg/seg_156860", func_002563A8);

INCLUDE_ASM("seg/seg_156860", func_002563F8);

INCLUDE_ASM("seg/seg_156860", func_00256460);

INCLUDE_ASM("seg/seg_156860", func_00256510);

INCLUDE_ASM("seg/seg_156860", func_002565E0);

INCLUDE_ASM("seg/seg_156860", func_00256698);

INCLUDE_ASM("seg/seg_156860", func_002566E8);

INCLUDE_ASM("seg/seg_156860", func_00256730);

INCLUDE_ASM("seg/seg_156860", func_002567E0);

INCLUDE_ASM("seg/seg_156860", func_00256860);

INCLUDE_ASM("seg/seg_156860", func_002568F8);

INCLUDE_ASM("seg/seg_156860", func_00256998);

INCLUDE_ASM("seg/seg_156860", func_00256A00);

INCLUDE_ASM("seg/seg_156860", func_00256A58);

INCLUDE_ASM("seg/seg_156860", func_00256AA8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256BA8);
#ifdef SKIP_ASM
extern "C" float func_00256BA8(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x10)));
}
#endif

INCLUDE_ASM("seg/seg_156860", RpcAlloc);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256BD8);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

extern "C" void func_00256BD8(int arg0) {
    if (arg0 != 0) {
        cMemMan_free__FPv();
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", cGameComm_construct);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256C50);
#ifdef SKIP_ASM
extern "C" void func_00256EE0(int, int);
extern int D_004A2EEC;

extern "C" void func_00256C50(void) {
    if (D_004A2EEC != 0) {
        func_00256EE0(D_004A2EEC, 3);
        D_004A2EEC = 0;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00256C78);

INCLUDE_ASM("seg/seg_156860", func_00256EE0);

INCLUDE_ASM("seg/seg_156860", func_00257038);

INCLUDE_ASM("seg/seg_156860", func_002570D8);

INCLUDE_ASM("seg/seg_156860", func_00257610);

INCLUDE_ASM("seg/seg_156860", func_00257678);

INCLUDE_ASM("seg/seg_156860", func_00257750);

INCLUDE_ASM("seg/seg_156860", func_002577C8);

INCLUDE_ASM("seg/seg_156860", func_002578F8);

INCLUDE_ASM("seg/seg_156860", func_002579C8);

INCLUDE_ASM("seg/seg_156860", func_00257A80);

INCLUDE_ASM("seg/seg_156860", func_00257BD8);

INCLUDE_ASM("seg/seg_156860", func_00257CD8);

INCLUDE_ASM("seg/seg_156860", func_00257DC0);

INCLUDE_ASM("seg/seg_156860", func_00257E98);

INCLUDE_ASM("seg/seg_156860", func_00257F98);

INCLUDE_ASM("seg/seg_156860", func_00258160);

INCLUDE_ASM("seg/seg_156860", func_00258188);

INCLUDE_ASM("seg/seg_156860", func_002581B8);

//100%
INCLUDE_ASM("seg/seg_156860", func_002581D8);
#ifdef SKIP_ASM
extern "C" int func_002581D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x124))) == 0;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002581E8);

INCLUDE_ASM("seg/seg_156860", func_002581F8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00258280);
#ifdef SKIP_ASM
extern "C" int func_00258280(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x12C))) == 0;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00258290);

INCLUDE_ASM("seg/seg_156860", func_002582A0);

INCLUDE_ASM("seg/seg_156860", func_00258330);

INCLUDE_ASM("seg/seg_156860", func_00258360);

INCLUDE_ASM("seg/seg_156860", func_002583A8);

INCLUDE_ASM("seg/seg_156860", cGameComm_syncGame);

INCLUDE_ASM("seg/seg_156860", func_002586B0);

INCLUDE_ASM("seg/seg_156860", func_00258790);

//100%
INCLUDE_ASM("seg/seg_156860", func_00258870);
#ifdef SKIP_ASM
extern "C" void func_00258870(void *arg0, int arg1, int arg2) {
    if ((arg2 != 0) || (((*(int *)((char*)(arg0) + (0x78))) == 0) && ((*(int *)((char*)(arg0) + (0x74))) == 0)) || ((*(int *)((char*)(arg0) + (0x7C))) != 0)) {
        (*(int *)((char*)(arg0) + (0x94))) = arg1;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002588A8);

INCLUDE_ASM("seg/seg_156860", func_00258950);

//100%
INCLUDE_ASM("seg/seg_156860", func_00258968);
#ifdef SKIP_ASM
extern "C" void func_00258968(void *arg0) {
    if ((*(int *)((char*)(arg0) + (0x70))) < 0xF) {
        (*(int *)((char*)(arg0) + (0x94))) = 0;
        (*(int *)((char*)(arg0) + (0x7C))) = 1;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00258988);

INCLUDE_ASM("seg/seg_156860", func_00258A48);

INCLUDE_ASM("seg/seg_156860", func_00258AE0);

INCLUDE_ASM("seg/seg_156860", func_00258BC8);

INCLUDE_ASM("seg/seg_156860", func_00258C00);

INCLUDE_ASM("seg/seg_156860", func_00258C50);

INCLUDE_ASM("seg/seg_156860", func_00258CB0);

INCLUDE_ASM("seg/seg_156860", func_00258F20);

INCLUDE_ASM("seg/seg_156860", func_00259098);

INCLUDE_ASM("seg/seg_156860", func_00259198);

INCLUDE_ASM("seg/seg_156860", func_002591A8);

//100%
INCLUDE_ASM("seg/seg_156860", func_002591B8);
#ifdef SKIP_ASM
extern void *D_004A28A8;

extern "C" int func_002591B8(void) {
    return (*(int *)((char*)(D_004A28A8) + (0x7C))) + 0xB5AB0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591D0);
#ifdef SKIP_ASM
extern void *D_004A28A8;

extern "C" int func_002591D0(void) {
    return (*(int *)((char*)(D_004A28A8) + (0x7C))) + 0xB5ABC;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002591E8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00259210);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv();

extern "C" void func_00259210(int arg0) {
    if (arg0 != 0) {
        cMemMan_free__FPv();
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00259230);
