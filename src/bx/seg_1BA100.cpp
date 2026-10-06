#include "common.h"

INCLUDE_ASM("bx/seg_1BA100", bxLogPrint);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9138);
#ifdef SKIP_ASM
extern "C" int func_002B9138(int *arg0) {
    return *arg0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9140);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9150);
#ifdef SKIP_ASM
extern "C" int func_002B9150(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x20))) + ((*(int *)((char*)(arg0) + (0x1C))) * 0x18);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9168);

INCLUDE_ASM("bx/seg_1BA100", func_002B9180);

INCLUDE_ASM("bx/seg_1BA100", func_002B9198);

INCLUDE_ASM("bx/seg_1BA100", func_002B91B0);

INCLUDE_ASM("bx/seg_1BA100", func_002B91C8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9208);
#ifdef SKIP_ASM
extern "C" unsigned short func_002B9208(void *arg0) {
    return (*(unsigned short *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9228);
#ifdef SKIP_ASM
extern "C" short func_002B9228(void *arg0) {
    return (*(short *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xA)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9248);
#ifdef SKIP_ASM
extern "C" int func_002B9248(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x70))) + ((*(int *)((char*)(arg0) + (0x1C))) * 0x10);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9260);

INCLUDE_ASM("bx/seg_1BA100", func_002B9278);

INCLUDE_ASM("bx/seg_1BA100", func_002B9290);

INCLUDE_ASM("bx/seg_1BA100", func_002B92A8);

INCLUDE_ASM("bx/seg_1BA100", func_002B92C0);

INCLUDE_ASM("bx/seg_1BA100", func_002B92D8);

INCLUDE_ASM("bx/seg_1BA100", func_002B92F0);

INCLUDE_ASM("bx/seg_1BA100", func_002B9308);

INCLUDE_ASM("bx/seg_1BA100", func_002B9320);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9338);
#ifdef SKIP_ASM
extern "C" void func_003BA020(int);

extern "C" void func_002B9338(void *arg0) {
    func_003BA020((*(int *)((char*)(arg0) + (0x20))) + ((*(int *)((char*)(arg0) + (0x1C))) * 0x18));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9368);

INCLUDE_ASM("bx/seg_1BA100", func_002B93B8);

INCLUDE_ASM("bx/seg_1BA100", func_002B95B0);

INCLUDE_ASM("bx/seg_1BA100", func_002B97B8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B97D0);
#ifdef SKIP_ASM
extern "C" void func_002B97D0(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (2))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B97F0);
#ifdef SKIP_ASM
extern "C" void func_002B97F0(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (3))) = arg1;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9810);

INCLUDE_ASM("bx/seg_1BA100", func_002B98A8);

INCLUDE_ASM("bx/seg_1BA100", func_002B99E8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B48);
#ifdef SKIP_ASM
extern "C" void func_002B9B48(void *arg0, signed char arg1) {
    (*(signed char *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (1))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B68);
#ifdef SKIP_ASM
extern "C" void func_002B9B68(void *arg0, short arg1) {
    (*(short *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9B88);
#ifdef SKIP_ASM
extern "C" void func_002B9B88(void *arg0, int arg1) {
    (*(int *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (4))) = arg1;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9BA0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BB8);
#ifdef SKIP_ASM
extern "C" void func_002B9BB8(void *arg0, int arg1) {
    (*(int *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BD0);
#ifdef SKIP_ASM
extern "C" void func_002B9BD0(void *arg0, int arg1) {
    (*(int *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x10) + (*(int *)((char*)(arg0) + (0x70))))) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BE8);
#ifdef SKIP_ASM
extern "C" void func_002B9BE8(void *arg0, short arg1) {
    (*(short *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C08);
#ifdef SKIP_ASM
extern "C" void func_002B9C08(void *arg0, short arg1) {
    (*(short *)((char*)((((*(int *)((char*)(arg0) + (0x1C))) * 0x18) + (*(int *)((char*)(arg0) + (0x20))))) + (0xA))) = arg1;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9C28);

INCLUDE_ASM("bx/seg_1BA100", func_002B9C40);

INCLUDE_ASM("bx/seg_1BA100", func_002B9C60);

INCLUDE_ASM("bx/seg_1BA100", func_002B9C78);

INCLUDE_ASM("bx/seg_1BA100", func_002B9CB0);

INCLUDE_ASM("bx/seg_1BA100", func_002B9CC8);

INCLUDE_ASM("bx/seg_1BA100", func_002B9CE0);

INCLUDE_ASM("bx/seg_1BA100", func_002B9D10);

INCLUDE_ASM("bx/seg_1BA100", func_002B9D28);

INCLUDE_ASM("bx/seg_1BA100", func_002B9D40);

INCLUDE_ASM("bx/seg_1BA100", func_002B9D58);

INCLUDE_ASM("bx/seg_1BA100", func_002B9D70);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D88);
#ifdef SKIP_ASM
extern "C" void func_002B9D88(void *arg0) {
    (*(int *)((char*)(arg0) + (0x94))) = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D98);
#ifdef SKIP_ASM
extern "C" void func_002B9D98(void *arg0) {
    (*(int *)((char*)(arg0) + (0x94))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DA0);
#ifdef SKIP_ASM
extern "C" int func_002B9DA0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x94)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9DA8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DB8);
#ifdef SKIP_ASM
extern "C" float func_002B9DB8(void *arg0) {
    int temp_2;
    int temp_3;

    temp_2 = (*(int *)((char*)(arg0) + (0x88)));
    temp_3 = (*(int *)((char*)(arg0) + (0x8C)));
    (*(int *)((char*)(arg0) + (0x8C))) = temp_2;
    return (float) (temp_2 - temp_3) * 0.01f;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DE0);
#ifdef SKIP_ASM
extern "C" int func_002B9DE0(void *arg0, int arg1) {
    return (*(int *)((char*)(arg0) + (0x8EC))) + (arg1 * 0xC0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9DF8);
#ifdef SKIP_ASM
extern "C" int func_002B9DF8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E00);
#ifdef SKIP_ASM
extern "C" void func_002B9E00(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x8E0))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E08);
#ifdef SKIP_ASM
extern "C" void func_002B9E08(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x8E4))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E10);
#ifdef SKIP_ASM
extern "C" int func_002B9E10(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E18);
#ifdef SKIP_ASM
extern "C" int func_002B9E18(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x8E4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E20);
#ifdef SKIP_ASM
extern "C" int func_002B9E20(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002B9E28);

INCLUDE_ASM("bx/seg_1BA100", func_002B9E70);

INCLUDE_ASM("bx/seg_1BA100", func_002B9E90);

INCLUDE_ASM("bx/seg_1BA100", func_002B9EB0);

INCLUDE_ASM("bx/seg_1BA100", func_002B9EC8);

INCLUDE_ASM("bx/seg_1BA100", func_002B9EE8);

INCLUDE_ASM("bx/seg_1BA100", func_002B9F08);

INCLUDE_ASM("bx/seg_1BA100", func_002B9F88);

INCLUDE_ASM("bx/seg_1BA100", func_002B9FB8);

INCLUDE_ASM("bx/seg_1BA100", func_002B9FD8);

INCLUDE_ASM("bx/seg_1BA100", func_002B9FF8);

INCLUDE_ASM("bx/seg_1BA100", func_002BA208);

INCLUDE_ASM("bx/seg_1BA100", func_002BA448);

INCLUDE_ASM("bx/seg_1BA100", func_002BA6B0);

INCLUDE_ASM("bx/seg_1BA100", func_002BA920);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC30);
#ifdef SKIP_ASM
extern "C" int func_002BAC30(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x408)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC38);
#ifdef SKIP_ASM
extern "C" int func_002B2488(int);

extern "C" int func_002BAC38(void *arg0) {
    int temp_4;
    int var_2;

    temp_4 = (*(int *)((char*)(arg0) + (0x408)));
    var_2 = -1;
    if (temp_4 != 0) {
        var_2 = func_002B2488(temp_4);
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC60);
#ifdef SKIP_ASM
extern "C" int func_002BAC60(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x3E4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC68);
#ifdef SKIP_ASM
extern "C" int func_002BAC68(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x3E8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC70);
#ifdef SKIP_ASM
extern "C" int func_002BAC70(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x3EC)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BAC78);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC90);
#ifdef SKIP_ASM
extern "C" int func_002BAC90(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x3F0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC98);
#ifdef SKIP_ASM
extern "C" void func_002B2850(int, void *);

extern "C" void func_002BAC98(void *arg0) {
    func_002B2850((*(int *)((char*)(arg0) + (0x408))), arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACB8);
#ifdef SKIP_ASM
extern "C" int func_002BACB8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x418)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACC0);
#ifdef SKIP_ASM
extern "C" void func_002BACC0(void *arg0) {
    (*(int *)((char*)(arg0) + (0x418))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACC8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void func_002BACC8(void *arg0) {
    (*(long *)((char*)(arg0) + (0x400))) = (long) (*(long *)((char*)(arg0) + (0x3F8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACD8);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long func_002BACD8(void *arg0) {
    return (*(long *)((char*)(arg0) + (0x400)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACE0);
#ifdef SKIP_ASM
extern "C" void func_002BACE0(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x3F0))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACE8);
#ifdef SKIP_ASM
extern "C" int func_002BACE8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x41C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACF0);
#ifdef SKIP_ASM
extern "C" int func_002BACF0(void) {
    return -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BACF8);
#ifdef SKIP_ASM
extern "C" int func_002BACF8(void) {
    return 0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BAD00);

INCLUDE_ASM("bx/seg_1BA100", func_002BAD18);

INCLUDE_ASM("bx/seg_1BA100", func_002BAD30);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD48);
#ifdef SKIP_ASM
extern "C" int func_002BAD48(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x578C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD50);
#ifdef SKIP_ASM
extern "C" int func_002BAD50(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x5790)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD58);
#ifdef SKIP_ASM
extern "C" void func_002BAD58(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x598C))) = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD60);
#ifdef SKIP_ASM
extern "C" int func_002BAD60(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x5FB0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD68);
#ifdef SKIP_ASM
extern "C" int func_002BAD68(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x608C)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD70);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long func_002BAD70(void *arg0) {
    return (*(long *)((char*)(arg0) + (0x6098)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD78);
#ifdef SKIP_ASM
extern "C" int func_002BAD78(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6234)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BAD80);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD98);
#ifdef SKIP_ASM
extern "C" int func_002BAD98(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6238)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADA0);
#ifdef SKIP_ASM
extern "C" int func_002BADA0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6280)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADA8);
#ifdef SKIP_ASM
extern "C" int func_002BADA8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B0)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADB0);
#ifdef SKIP_ASM
extern "C" int func_002BADB0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B8)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADB8);
#ifdef SKIP_ASM
extern "C" int func_002BADB8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x62B4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADC0);
#ifdef SKIP_ASM
extern "C" int func_002BADC0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6470)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BADC8);
#ifdef SKIP_ASM
extern "C" int func_002BADC8(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x6474)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BADD0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAEE8);
#ifdef SKIP_ASM
extern "C" void func_002B8818(int, int);

extern "C" void func_002BAEE8(void) {
    func_002B8818(1, 0xFFFF);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BAF08);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAF90);
#ifdef SKIP_ASM
extern "C" void *func_002C1CD8(int);

extern "C" int func_002BAF90(int *arg0) {
    return (*(int *)((char*)(func_002C1CD8(*arg0)) + (4)));
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BAFB0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BB0E0);
#ifdef SKIP_ASM
extern "C" void func_002BAFB0(int, int);

extern "C" void func_002BB0E0(void) {
    func_002BAFB0(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB100);
#ifdef SKIP_ASM
extern "C" int func_002BB100(void *arg0, void *arg1) {
    int var_6;

    var_6 = 0;
    if (((*(int *)((char*)(arg0) + (0))) != (*(int *)((char*)(arg1) + (0)))) || ((*(int *)((char*)(arg0) + (4))) != (*(int *)((char*)(arg1) + (4))))) {
        var_6 = 1;
    }
    return var_6;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB130);
#ifdef SKIP_ASM
extern "C" void *func_002BB130(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB140);

INCLUDE_ASM("bx/seg_1BA100", func_002BB1D0);

INCLUDE_ASM("bx/seg_1BA100", func_002BB2B8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB318);
#ifdef SKIP_ASM
extern "C" void func_002BB4D8(int, int);

extern "C" void func_002BB318(void *arg0, int arg1, int arg2) {
    func_002BB4D8((*(int *)((char*)(arg0) + (4))) + (arg1 * 0xC), arg2);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB348);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB370);
#ifdef SKIP_ASM
extern "C" void *func_002BB370(void *arg0) {
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (4))) = -1;
    (*(short *)((char*)(arg0) + (0))) = 0;
    (*(short *)((char*)(arg0) + (2))) = 0;
    return arg0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB390);

INCLUDE_ASM("bx/seg_1BA100", func_002BB3F0);

INCLUDE_ASM("bx/seg_1BA100", func_002BB4D8);

INCLUDE_ASM("bx/seg_1BA100", func_002BB570);

INCLUDE_ASM("bx/seg_1BA100", func_002BB5A8);

INCLUDE_ASM("bx/seg_1BA100", func_002BB5F0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB6B8);
#ifdef SKIP_ASM
extern "C" void func_002BB6E0(int, void *);
extern void *D_004A28A8;

extern "C" void func_002BB6B8(void) {
    void *temp_5;

    temp_5 = (*(void **)((char*)((*(void **)((char*)(D_004A28A8) + (0x84)))) + (0x10)));
    func_002BB6E0((*(int *)((char*)(temp_5) + (8))), temp_5);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB6E0);

INCLUDE_ASM("bx/seg_1BA100", func_002BB928);

INCLUDE_ASM("bx/seg_1BA100", func_002BBBF8);

INCLUDE_ASM("bx/seg_1BA100", func_002BBC38);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("bx/seg_1BA100", func_002BC4E0);
#ifdef SKIP_ASM
extern "C" void func_002BBC38(int, int);

extern "C" void func_002BC4E0(void) {
    func_002BBC38(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC500);
#ifdef SKIP_ASM
extern "C" int func_002BC500(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC508);
#ifdef SKIP_ASM
extern "C" int func_002BC508(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC510);
#ifdef SKIP_ASM
extern "C" int func_002BC510(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC518);
#ifdef SKIP_ASM
extern "C" int func_002BC518(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC520);
#ifdef SKIP_ASM
extern "C" int func_002BC520(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC528);
#ifdef SKIP_ASM
extern "C" int func_002BC528(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC530);
#ifdef SKIP_ASM
extern "C" int func_002BC530(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC538);
#ifdef SKIP_ASM
extern "C" int func_002BC538(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC540);
#ifdef SKIP_ASM
extern "C" int func_002BC540(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC548);
#ifdef SKIP_ASM
extern "C" int func_002BC548(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC550);
#ifdef SKIP_ASM
extern "C" int func_002BC550(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC558);
#ifdef SKIP_ASM
extern "C" int func_002BC558(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC560);
#ifdef SKIP_ASM
extern "C" int func_002BC560(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC568);
#ifdef SKIP_ASM
extern "C" int func_002BC568(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC570);
#ifdef SKIP_ASM
extern "C" int func_002BC570(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC578);
#ifdef SKIP_ASM
extern "C" int func_002BC578(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC580);
#ifdef SKIP_ASM
extern "C" int func_002BC580(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC588);
#ifdef SKIP_ASM
extern "C" int func_002BC588(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC590);
#ifdef SKIP_ASM
extern "C" int func_002BC590(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC598);
#ifdef SKIP_ASM
extern "C" int func_002BC598(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5A0);
#ifdef SKIP_ASM
extern "C" int func_002BC5A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5A8);
#ifdef SKIP_ASM
extern "C" int func_002BC5A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5B0);
#ifdef SKIP_ASM
extern "C" int func_002BC5B0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5B8);
#ifdef SKIP_ASM
extern "C" int func_002BC5B8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5C0);
#ifdef SKIP_ASM
extern "C" int func_002BC5C0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5C8);
#ifdef SKIP_ASM
extern "C" int func_002BC5C8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5D0);
#ifdef SKIP_ASM
extern "C" int func_002BC5D0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5D8);
#ifdef SKIP_ASM
extern "C" int func_002BC5D8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5E0);
#ifdef SKIP_ASM
extern "C" int func_002BC5E0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5E8);
#ifdef SKIP_ASM
extern "C" int func_002BC5E8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5F0);
#ifdef SKIP_ASM
extern "C" int func_002BC5F0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC5F8);
#ifdef SKIP_ASM
extern "C" int func_002BC5F8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC600);
#ifdef SKIP_ASM
extern "C" int func_002BC600(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC608);
#ifdef SKIP_ASM
extern "C" int func_002BC608(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC610);
#ifdef SKIP_ASM
extern "C" int func_002BC610(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC618);
#ifdef SKIP_ASM
extern "C" int func_002BC618(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC620);
#ifdef SKIP_ASM
extern "C" int func_002BC620(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC628);
#ifdef SKIP_ASM
extern "C" int func_002BC628(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC630);
#ifdef SKIP_ASM
extern "C" int func_002BC630(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC638);
#ifdef SKIP_ASM
extern "C" int func_002BC638(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC640);
#ifdef SKIP_ASM
extern "C" int func_002BC640(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC648);
#ifdef SKIP_ASM
extern "C" int func_002BC648(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC650);
#ifdef SKIP_ASM
extern "C" int func_002BC650(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC658);
#ifdef SKIP_ASM
extern "C" int func_002BC658(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC660);
#ifdef SKIP_ASM
extern "C" int func_002BC660(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC668);
#ifdef SKIP_ASM
extern "C" int func_002BC668(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC670);
#ifdef SKIP_ASM
extern "C" int func_002BC670(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC678);
#ifdef SKIP_ASM
extern "C" int func_002BC678(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC680);
#ifdef SKIP_ASM
extern "C" int func_002BC680(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC688);
#ifdef SKIP_ASM
extern "C" int func_002BC688(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC690);
#ifdef SKIP_ASM
extern "C" int func_002BC690(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC698);
#ifdef SKIP_ASM
extern "C" int func_002BC698(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6A0);
#ifdef SKIP_ASM
extern "C" int func_002BC6A0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6A8);
#ifdef SKIP_ASM
extern "C" int func_002BC6A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6B0);
#ifdef SKIP_ASM
extern "C" int func_002BC6B0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6B8);
#ifdef SKIP_ASM
extern "C" int func_002BC6B8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6C0);
#ifdef SKIP_ASM
extern "C" int func_002BC6C0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6C8);
#ifdef SKIP_ASM
extern "C" int func_002BC6C8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6D0);
#ifdef SKIP_ASM
extern "C" int func_002BC6D0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6D8);
#ifdef SKIP_ASM
extern "C" int func_002BC6D8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6E0);
#ifdef SKIP_ASM
extern "C" int func_002BC6E0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6E8);
#ifdef SKIP_ASM
extern "C" int func_002BC6E8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6F0);
#ifdef SKIP_ASM
extern "C" int func_002BC6F0(void) {
    return 0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BC6F8);

INCLUDE_ASM("bx/seg_1BA100", func_002BC728);

INCLUDE_ASM("bx/seg_1BA100", func_002BC750);

INCLUDE_ASM("bx/seg_1BA100", func_002BC778);

INCLUDE_ASM("bx/seg_1BA100", func_002BC7A0);

INCLUDE_ASM("bx/seg_1BA100", func_002BC7C8);

INCLUDE_ASM("bx/seg_1BA100", func_002BC830);

INCLUDE_ASM("bx/seg_1BA100", func_002BC890);

INCLUDE_ASM("bx/seg_1BA100", func_002BC8E8);

INCLUDE_ASM("bx/seg_1BA100", func_002BC910);

INCLUDE_ASM("bx/seg_1BA100", func_002BC980);

INCLUDE_ASM("bx/seg_1BA100", func_002BC9B0);

INCLUDE_ASM("bx/seg_1BA100", func_002BCAF8);

INCLUDE_ASM("bx/seg_1BA100", func_002BCBD0);

INCLUDE_ASM("bx/seg_1BA100", func_002BCC00);

INCLUDE_ASM("bx/seg_1BA100", func_002BCCC8);

INCLUDE_ASM("bx/seg_1BA100", func_002BCD68);

INCLUDE_ASM("bx/seg_1BA100", func_002BCE08);

INCLUDE_ASM("bx/seg_1BA100", func_002BCEA8);

INCLUDE_ASM("bx/seg_1BA100", func_002BCF38);

INCLUDE_ASM("bx/seg_1BA100", func_002BD068);

INCLUDE_ASM("bx/seg_1BA100", func_002BD1B8);

INCLUDE_ASM("bx/seg_1BA100", func_002BD2E8);

INCLUDE_ASM("bx/seg_1BA100", func_002BD378);

INCLUDE_ASM("bx/seg_1BA100", func_002BD518);

INCLUDE_ASM("bx/seg_1BA100", func_002BD5A8);

INCLUDE_ASM("bx/seg_1BA100", func_002BD698);

INCLUDE_ASM("bx/seg_1BA100", func_002BD968);

extern "C" void func_002BDA28(void) {
}

extern "C" void func_002BDA30(void) {
}

extern "C" void func_002BDA38(void) {
}

extern "C" void func_002BDA40(void) {
}

extern "C" void func_002BDA48(void) {
}

extern "C" void func_002BDA50(void) {
}

extern "C" void func_002BDA58(void) {
}

extern "C" void func_002BDA60(void) {
}

extern "C" void func_002BDA68(void) {
}

extern "C" void func_002BDA70(void) {
}

extern "C" void func_002BDA78(void) {
}

extern "C" void func_002BDA80(void) {
}

extern "C" void func_002BDA88(void) {
}

extern "C" void func_002BDA90(void) {
}

INCLUDE_ASM("bx/seg_1BA100", func_002BDA98);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAC8);
#ifdef SKIP_ASM
extern "C" int func_002BDAC8(void *arg0, void *arg1) {
    return (*(int *)((char*)(arg0) + (8))) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAE0);
#ifdef SKIP_ASM
extern "C" int func_002BDAE0(void *arg0, void *arg1) {
    return (*(int *)((char*)(arg0) + (8))) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDAF8);
#ifdef SKIP_ASM
extern "C" int func_002BDAF8(void *arg0, void *arg1) {
    return (*(int *)((char*)(arg0) + (8))) == (*(int *)((char*)(arg1) + (4)));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDB10);
#ifdef SKIP_ASM
extern "C" int func_002BDB10(void *arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((*(float *)((char*)(arg0) + (8))) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BDB38);

INCLUDE_ASM("bx/seg_1BA100", func_002BDBD0);

INCLUDE_ASM("bx/seg_1BA100", func_002BDC78);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDD10);
#ifdef SKIP_ASM
extern "C" int func_002BDD10(void *arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((*(float *)((char*)(arg0) + (8))) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BDD38);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDE08);
#ifdef SKIP_ASM
extern "C" int func_002BDE08(void *arg0, void *arg1) {
    int var_2;

    var_2 = 0;
    if ((*(float *)((char*)(arg0) + (8))) == (*(float *)((char*)(arg1) + (4)))) {
        var_2 = 1;
    }
    return var_2;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BDE30);

INCLUDE_ASM("bx/seg_1BA100", func_002BDEE0);

INCLUDE_ASM("bx/seg_1BA100", func_002BE078);

INCLUDE_ASM("bx/seg_1BA100", func_002BE0B0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0C8);
#ifdef SKIP_ASM
extern "C" void func_002BE0C8(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(int *)((char*)(arg0) + (8))) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0D8);
#ifdef SKIP_ASM
extern "C" void func_002BE0D8(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(int *)((char*)(arg0) + (8))) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0E8);
#ifdef SKIP_ASM
extern "C" void func_002BE0E8(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(int *)((char*)(arg0) + (8))) = -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0F8);
#ifdef SKIP_ASM
extern "C" void func_002BE0F8(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(float *)((char*)(arg0) + (8))) = 30000.0f;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BE108);

INCLUDE_ASM("bx/seg_1BA100", func_002BE140);

INCLUDE_ASM("bx/seg_1BA100", func_002BE170);

INCLUDE_ASM("bx/seg_1BA100", func_002BE198);

INCLUDE_ASM("bx/seg_1BA100", func_002BE1A8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE1E0);
#ifdef SKIP_ASM
extern "C" void func_002BE1E0(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(float *)((char*)(arg0) + (8))) = 1.0f;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BE1F8);

INCLUDE_ASM("bx/seg_1BA100", func_002BE258);

INCLUDE_ASM("bx/seg_1BA100", func_002BE2C8);

INCLUDE_ASM("bx/seg_1BA100", func_002BE2D8);

INCLUDE_ASM("bx/seg_1BA100", func_002BE378);

INCLUDE_ASM("bx/seg_1BA100", func_002BE3E0);

INCLUDE_ASM("bx/seg_1BA100", func_002BE448);

INCLUDE_ASM("bx/seg_1BA100", func_002BE4B0);

INCLUDE_ASM("bx/seg_1BA100", func_002BE518);

INCLUDE_ASM("bx/seg_1BA100", func_002BE698);

INCLUDE_ASM("bx/seg_1BA100", func_002BE850);

INCLUDE_ASM("bx/seg_1BA100", func_002BE9D0);

INCLUDE_ASM("bx/seg_1BA100", func_002BEA38);

INCLUDE_ASM("bx/seg_1BA100", func_002BEC60);

INCLUDE_ASM("bx/seg_1BA100", func_002BECC8);

INCLUDE_ASM("bx/seg_1BA100", func_002BEE48);

INCLUDE_ASM("bx/seg_1BA100", func_002BF2A0);

INCLUDE_ASM("bx/seg_1BA100", func_002BF340);

INCLUDE_ASM("bx/seg_1BA100", func_002BF3E0);

INCLUDE_ASM("bx/seg_1BA100", func_002BF448);

INCLUDE_ASM("bx/seg_1BA100", func_002BF4B0);

INCLUDE_ASM("bx/seg_1BA100", func_002BF518);

INCLUDE_ASM("bx/seg_1BA100", func_002BF580);

INCLUDE_ASM("bx/seg_1BA100", func_002BF700);

INCLUDE_ASM("bx/seg_1BA100", func_002BF8B8);

INCLUDE_ASM("bx/seg_1BA100", func_002BFA38);

INCLUDE_ASM("bx/seg_1BA100", func_002BFAA0);

INCLUDE_ASM("bx/seg_1BA100", func_002BFCC8);

INCLUDE_ASM("bx/seg_1BA100", func_002BFD30);

INCLUDE_ASM("bx/seg_1BA100", func_002BFEB0);

INCLUDE_ASM("bx/seg_1BA100", func_002C0308);
