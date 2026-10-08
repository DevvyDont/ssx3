#include "common.h"

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_construct);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A2EB8;
extern "C" int cCommSystem_cCommSystem_5860(int) __asm__("cCommSystem_cCommSystem");
extern "C" int cMemMan_alloc_5860(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char D_00480300[];

extern "C" void cCommSystem_construct(void) {
    D_004A2EB8 = cCommSystem_cCommSystem_5860(cMemMan_alloc_5860(0x34, D_00480300, 0x20000000, 0));
}
#endif

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

//100%
INCLUDE_ASM("seg/seg_156860", cCommSystem_getSignature);
#ifdef SKIP_ASM
extern char D_00481370[];

extern "C" char *cCommSystem_getSignature(void) {
    return D_00481370;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00255A20);

INCLUDE_ASM("seg/seg_156860", cCommSystem_openDSockChannel);

INCLUDE_ASM("seg/seg_156860", func_00255CC8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D38);
#ifdef SKIP_ASM
extern "C" int func_00255D38(void *arg0, int arg1) {
    return *(int *)((char*)arg0 + (arg1 << 2)) != 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D50);
#ifdef SKIP_ASM
extern "C" int func_00255D50(void *arg0, int arg1) {
    char *obj = *(char **)((char*)arg0 + (arg1 << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *))(vt + 0x24))(obj + *(short *)(vt + 0x20));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255D88);
#ifdef SKIP_ASM
extern "C" float func_00255D88(int arg0, int arg1) {
    char *obj = *(char **)(arg0 + (arg1 << 2));
    char *vt = *(char **)(obj + 8);
    return (*(float (**)(void *))(vt + 0x2C))(obj + *(short *)(vt + 0x28));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255DC0);
#ifdef SKIP_ASM
extern "C" int func_00255DC0(void *arg0, int arg1, int arg2, int arg3) {
    char *obj = *(char **)((char*)arg0 + (arg1 << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *, int, int))(vt + 0x3C))(obj + *(short *)(vt + 0x38), arg2, arg3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255E00);
#ifdef SKIP_ASM
extern "C" int func_00255E00(void *arg0, int arg1, int arg2, int arg3) {
    char *obj = *(char **)((char*)arg0 + (arg1 << 2));
    char *vt = *(char **)(obj + 8);
    return (*(int (**)(void *, int, int))(vt + 0x44))(obj + *(short *)(vt + 0x40), arg2, arg3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255E40);
#ifdef SKIP_ASM
extern "C" int func_00255E40(void *arg0, int arg1) {
    if ((*(int *)((char*)(arg0) + (0x28))) != 0) {
        return 1;
    }
    return *(int *)((char*)(*(void **)((char*)arg0 + (arg1 << 2))) + 4);
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00255E68);

INCLUDE_ASM("seg/seg_156860", func_00255EC0);

//100%
INCLUDE_ASM("seg/seg_156860", func_00255F20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003F3BC8_5F20(int, void *, int) __asm__("func_003F3BC8");
extern char D_00536790[];
extern int D_004A2EBC;

extern "C" int func_00255F20(void *arg0, int arg1) {
    D_004A2EBC = func_003F3BC8_5F20(arg1, D_00536790, 0xA);
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255F50);
#ifdef SKIP_ASM
extern "C" void func_00255FA0(void *);

extern "C" void func_00255F50(void *arg0, int arg1) {
    func_00255FA0(arg0);
    (*(int *)((char*)(arg0) + (0x1C))) = arg1;
    (*(int *)((char*)(arg0) + (0x30))) = 1;
    (*(int *)((char*)(arg0) + (0x24))) = 0x2328;
    (*(int *)((char*)(arg0) + (0x20))) = 2;
    (*(int *)((char*)(arg0) + (0x2C))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255FA0);
#ifdef SKIP_ASM
extern "C" void func_003F40B8();

extern "C" void func_00255FA0(void *arg0) {
    if ((*(int *)((char*)(arg0) + (0x1C))) >= 0) {
        (*(int *)((char*)(arg0) + (0x1C))) = -1;
        func_003F40B8();
    }
    (*(int *)((char*)(arg0) + (0x24))) = 0;
    (*(int *)((char*)(arg0) + (0x20))) = 1;
    (*(int *)((char*)(arg0) + (0x28))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00255FE8);
#ifdef SKIP_ASM
extern "C" void func_003F40B8();

extern "C" void func_00255FE8(void *arg0, int arg1) {
    if ((*(int *)((char*)(arg0) + (0x1C))) >= 0) {
        (*(int *)((char*)(arg0) + (0x1C))) = -1;
        func_003F40B8();
    }
    (*(int *)((char*)(arg0) + (0x28))) = arg1;
    (*(int *)((char*)(arg0) + (0x20))) = 6;
    (*(int *)((char*)(arg0) + (0x24))) = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256038);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003F3CC0_6038(int, int, int) __asm__("func_003F3CC0");

extern "C" void func_00256038(void *arg0, int *arg1) {
    int temp_2;

    *arg1 = 0;
    temp_2 = func_003F3CC0_6038(0x61646472, 0, 0);
    if (temp_2 & 0xFFFFFF00) {
        *arg1 = temp_2;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256088);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0031B008_6088(void *, int, void *) __asm__("func_0031B008");
extern char D_00480378[];
extern int D_004A2EC0;

extern "C" void func_00256088(void) {
    func_0031B008_6088(D_00480378, 1, &D_004A2EC0);
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002560B0);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_003DED50_6188(void *, int, int, void *) __asm__("func_003DED50");
extern char D_004803D0[];
extern int D_004A2ED4;

extern "C" void func_00256188(void) {
    func_003DED50_6188(D_004803D0, 0, 0x64, &D_004A2ED4);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002561B8);
#ifdef SKIP_ASM
extern "C" void func_00256228();

extern "C" int func_002561B8(int arg0) {
    func_00256228();
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002561E0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00256250_61E0(int *) __asm__("func_00256250");
extern "C" void operator_delete__FPi(int *);

extern "C" void func_002561E0(int *arg0, int arg1) {
    func_00256250_61E0(arg0);
    if (arg1 & 1) {
        operator_delete__FPi(arg0);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00256228);
#ifdef SKIP_ASM
extern "C" void func_00256228(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = 0;
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    (*(int *)((char*)(arg0) + (0x18))) = -1;
    (*(int *)((char*)(arg0) + (0))) = 0;
}
#endif

extern "C" void func_00256250(void) {
}

INCLUDE_ASM("seg/seg_156860", func_00256258);

INCLUDE_ASM("seg/seg_156860", func_002562D0);

//100%
INCLUDE_ASM("seg/seg_156860", func_002563A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMemMan_free__FPv_63A8(int) __asm__("cMemMan_free__FPv");

extern "C" void func_002563A8(void *arg0) {
    int temp_4;
    int temp_4_2;

    temp_4 = (*(int *)((char*)(arg0) + (0xC)));
    if (temp_4 != 0) {
        cMemMan_free__FPv_63A8(temp_4);
        (*(int *)((char*)(arg0) + (0xC))) = 0;
    }
    temp_4_2 = (*(int *)((char*)(arg0) + (4)));
    if (temp_4_2 != 0) {
        cMemMan_free__FPv_63A8(temp_4_2);
        (*(int *)((char*)(arg0) + (4))) = 0;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002563F8);

INCLUDE_ASM("seg/seg_156860", func_00256460);

INCLUDE_ASM("seg/seg_156860", func_00256510);

INCLUDE_ASM("seg/seg_156860", func_002565E0);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256698);
#ifdef SKIP_ASM
void func_002557E0(void *);
extern char D_004812D0[];

extern "C" void *func_00256698(void *arg0) {
    func_002557E0(arg0);
    (*(char **)((char*)(arg0) + (8))) = D_004812D0;
    (*(float *)((char*)(arg0) + (0x10))) = 0.10000000149011612f;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(int *)((char*)(arg0) + (0x18))) = 0;
    (*(int *)((char*)(arg0) + (0x1C))) = 0;
    (*(int *)((char*)(arg0) + (0x14))) = 0;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002566E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00255800_66E8(void *, int) __asm__("func_00255800");
extern "C" void func_00256998(void *);
extern char D_004812D0[];

extern "C" void func_002566E8(void *arg0, int arg1) {
    (*(char **)((char*)(arg0) + (8))) = D_004812D0;
    func_00256998(arg0);
    func_00255800_66E8(arg0, arg1);
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00256730);

INCLUDE_ASM("seg/seg_156860", func_002567E0);

INCLUDE_ASM("seg/seg_156860", func_00256860);

INCLUDE_ASM("seg/seg_156860", func_002568F8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256998);
#ifdef SKIP_ASM
extern "C" void func_00255840();
extern "C" void func_003E67F8(int);
extern "C" void func_003E6E70(int);

extern "C" void func_00256998(void *arg0) {
    int temp_4;
    int temp_4_2;

    if ((*(int *)((char*)(arg0) + (0xC))) != 0) {
        func_00255840();
        temp_4 = (*(int *)((char*)(arg0) + (0x1C)));
        if (temp_4 != 0) {
            func_003E67F8(temp_4);
            (*(int *)((char*)(arg0) + (0x1C))) = 0;
        }
        temp_4_2 = (*(int *)((char*)(arg0) + (0x18)));
        if (temp_4_2 != 0) {
            func_003E6E70(temp_4_2);
            (*(int *)((char*)(arg0) + (0x18))) = 0;
        }
        (*(int *)((char*)(arg0) + (0xC))) = 0;
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00256A00);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256A58);
#ifdef SKIP_ASM
extern "C" int func_003E66F0(int, int, int);
extern "C" int func_003E6FC8(int);

extern "C" void func_00256A58(void *arg0) {
    int temp_2;
    int temp_2_2;

    temp_2 = func_003E6FC8((*(int *)((char*)(arg0) + (0x18))));
    if (temp_2 != 0) {
        temp_2_2 = func_003E66F0(temp_2, 0, 0x237C);
        (*(int *)((char*)(arg0) + (0x1C))) = temp_2_2;
        if (temp_2_2 != 0) {
            (*(int *)((char*)(arg0) + (0xC))) = 2;
        }
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00256AA8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00256BA8);
#ifdef SKIP_ASM
extern "C" float func_00256BA8(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", RpcAlloc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *operator_new_6BB0(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern char D_00480478[];

extern "C" void *RpcAlloc(unsigned int size) {
    return operator_new_6BB0(size, D_00480478, 0x20000000, 0);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258160);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00257BD8_8160(int, int, int, int) __asm__("func_00257BD8");

extern "C" int func_00258160(int arg0, int arg1, int arg2) {
    return func_00257BD8_8160(arg0, 1, arg1, arg2) > 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258188);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *cMemMan_alloc_8188(unsigned int, void *, unsigned int, int) __asm__("cMemMan_alloc");
extern char D_004805A8[];

extern "C" void *func_00258188(void) {
    return cMemMan_alloc_8188(0xF0, D_004805A8, 0x20000000, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581B8);
#ifdef SKIP_ASM
extern "C" void operator_delete__FPi(int *);

extern "C" void func_002581B8(void *arg0, int *arg1) {
    operator_delete__FPi(arg1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581D8);
#ifdef SKIP_ASM
extern "C" int func_002581D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x124))) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002581E8);
#ifdef SKIP_ASM
extern "C" void func_002581E8(void *arg0, int arg1, int arg2) {
    (*(int *)((char*)(arg0) + (0x124))) = arg1;
    (*(int *)((char*)(arg0) + (0x128))) = arg2;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002581F8);

//100%
INCLUDE_ASM("seg/seg_156860", func_00258280);
#ifdef SKIP_ASM
extern "C" int func_00258280(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x12C))) == 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_00258290);
#ifdef SKIP_ASM
extern "C" void func_00258290(void *arg0, int arg1, int arg2) {
    (*(int *)((char*)(arg0) + (0x12C))) = arg1;
    (*(int *)((char*)(arg0) + (0x130))) = arg2;
}
#endif

INCLUDE_ASM("seg/seg_156860", func_002582A0);

//100%
INCLUDE_ASM("seg/seg_156860", func_00258330);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern void *D_004A28A8;

extern "C" void func_00258330(void *arg0) {
    char *p = (char*)D_004A28A8;
    float b = *(float *)((char*)arg0 + 0x118);
    float a = *(float *)((char*)arg0 + 0x114);
    float r = b >? a;
    *(float *)((char*)arg0 + 0x11C) = r;
    float c = *(float *)(p + 0x14);
    if (r < c) {
        *(float *)((char*)arg0 + 0x11C) = c;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258360);
#ifdef SKIP_ASM
extern "C" float func_00255D88(int, int);
extern void *D_004A28A8;
extern int D_004A2EB8;

extern "C" float func_00258360(void *arg0) {
    return func_00255D88(D_004A2EB8, (*(int *)((char*)(arg0) + (0x6C)))) + ((float) (*(int *)((char*)(arg0) + (0x120))) * (*(float *)((char*)(D_004A28A8) + (0x14))));
}
#endif

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

//100%
INCLUDE_ASM("seg/seg_156860", func_00258950);
#ifdef SKIP_ASM
extern "C" void func_00258950(void *arg0) {
    (*(int *)((char*)(arg0) + (0x84))) = 0;
    (*(int *)((char*)(arg0) + (0x48))) = 1;
    (*(int *)((char*)(arg0) + (0x78))) = 0;
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258BC8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00257E98_8BC8(void *, int, void *, int) __asm__("func_00257E98");

extern "C" void func_00258BC8(void *arg0) {
    if ((*(int *)((char*)(arg0) + (4))) == 0) {
        char buf[12];
        buf[0] = 5;
        func_00257E98_8BC8(arg0, 3, buf, 0xC);
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00258C00);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_156860", func_00258C50);
#ifdef SKIP_ASM
extern "C" void func_00257E98(void *, int, int, int);
extern "C" int strlen(int);

extern "C" void func_00258C50(void *arg0, int arg1) {
    if (((*(int *)((char*)(arg0) + (4))) == 0) && ((*(int *)((char*)(arg0) + (0))) != 0)) {
        func_00257E98(arg0, 1, arg1, strlen(arg1) + 1);
    }
}
#endif

INCLUDE_ASM("seg/seg_156860", func_00258CB0);

INCLUDE_ASM("seg/seg_156860", func_00258F20);

INCLUDE_ASM("seg/seg_156860", func_00259098);

//100%
INCLUDE_ASM("seg/seg_156860", func_00259198);
#ifdef SKIP_ASM
extern char D_004A2F18[];

extern "C" char *func_00259198(void) {
    return D_004A2F18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_156860", func_002591A8);
#ifdef SKIP_ASM
extern char D_004A2F20[];

extern "C" char *func_002591A8(void) {
    return D_004A2F20;
}
#endif

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

//100%
INCLUDE_ASM("seg/seg_156860", func_002591E8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void *operator_new_91E8(unsigned int, void *, unsigned int, int) __asm__("operator_new__FUi");
extern char D_004A2F28[];

extern "C" void *func_002591E8(unsigned int size) {
    return operator_new_91E8(size, D_004A2F28, 0x20000000, 0);
}
#endif

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
