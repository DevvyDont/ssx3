#include "common.h"

//100%
INCLUDE_ASM("bx/seg_1BA100", bxLogPrint);
#ifdef SKIP_ASM
extern "C" void bxLogPrint(const char *fmt, ...) {
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9168);
#ifdef SKIP_ASM
extern "C" int func_002B9168(void *p) {
    return *(int*)((char*)p + (*(int*)((char*)p + 0x1C) << 2) + 0x24);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9180);
#ifdef SKIP_ASM
extern "C" int func_002B9180(void *p) {
    return (*(int**)((char*)p + 0x34))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9198);
#ifdef SKIP_ASM
extern "C" int func_002B9198(void *p) {
    return (*(int**)((char*)p + 0x38))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B91B0);
#ifdef SKIP_ASM
extern "C" int func_002B91B0(void *p) {
    return (*(int**)((char*)p + 0x3C))[*(int*)((char*)p + 0x1C)];
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9260);
#ifdef SKIP_ASM
extern "C" int func_002B9260(void *p) {
    return (*(int**)((char*)p + 0x60))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9278);
#ifdef SKIP_ASM
extern "C" int func_002B9278(void *p) {
    return (*(int**)((char*)p + 0x64))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9290);
#ifdef SKIP_ASM
extern "C" float func_002B9290(void *p) {
    return (*(float**)((char*)p + 0x68))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92A8);
#ifdef SKIP_ASM
extern "C" float func_002B92A8(void *p) {
    return (*(float**)((char*)p + 0x6C))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92C0);
#ifdef SKIP_ASM
extern "C" int func_002B92C0(void *p) {
    return (*(int**)((char*)p + 0x74))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92D8);
#ifdef SKIP_ASM
extern "C" int func_002B92D8(void *p) {
    return (*(int**)((char*)p + 0x78))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B92F0);
#ifdef SKIP_ASM
extern "C" int func_002B92F0(void *p) {
    return (*(int**)((char*)p + 0x7C))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9308);
#ifdef SKIP_ASM
extern "C" int func_002B9308(void *p) {
    return (*(int**)((char*)p + 0x80))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9320);
#ifdef SKIP_ASM
extern "C" int func_002B9320(void *p) {
    return (*(int**)((char*)p + 0x84))[*(int*)((char*)p + 0x1C)];
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9338);
#ifdef SKIP_ASM
extern "C" void func_003BA020(int);

extern "C" void func_002B9338(void *arg0) {
    func_003BA020((*(int *)((char*)(arg0) + (0x20))) + ((*(int *)((char*)(arg0) + (0x1C))) * 0x18));
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9368);
#ifdef SKIP_ASM
struct S9368 { int a[6]; };
extern "C" void func_002B9368(void *self) {
    *(S9368*)((char*)self + 4) = *(S9368*)((char*)(*(int*)((char*)self + 0x20)) + *(int*)((char*)self + 0x1C) * 0x18);
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9BA0);
#ifdef SKIP_ASM
struct E_9BA0 { int v[4]; };
extern "C" void func_002B9BA0(void *p, int v) {
    (*(E_9BA0**)((char*)p + 0x70))[*(int*)((char*)p + 0x1C)].v[0] = v;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C28);
#ifdef SKIP_ASM
extern "C" void func_002B9C28(void *p, int v) {
    *(int*)((char*)p + (*(int*)((char*)p + 0x1C) << 2) + 0x24) = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C40);
#ifdef SKIP_ASM
extern "C" void func_002B9C40(void *arg0) {
    int *base = *(int **)((char *)arg0 + 0x34);
    base[*(int *)((char *)arg0 + 0x1C)] = 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C60);
#ifdef SKIP_ASM
extern "C" void func_002B9C60(void *p, int v) {
    (*(int**)((char*)p + 0x64))[*(int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9C78);
#ifdef SKIP_ASM
extern "C" void func_002B9C78(void *self, int v) {
    int t = (v <= 0xFFFF) ? v : 0xFFFF;
    *(short*)((char*)(*(int*)((char*)self + 0x1C) * 0x18 + *(int*)((char*)self + 0x20)) + 0x14) = (t <= -1) ? 0 : t;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CB0);
#ifdef SKIP_ASM
extern "C" void func_002B9CB0(void *p, int v) {
    (*(int**)((char*)p + 0x60))[*(int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CC8);
#ifdef SKIP_ASM
extern "C" void func_002B9CC8(void *p, int v) {
    (*(int**)((char*)p + 0x3C))[*(int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9CE0);
#ifdef SKIP_ASM
struct V8_9CE0 { int a, b; };
extern "C" void func_002B9CE0(char *arg0, V8_9CE0 arg1) {
    V8_9CE0 *d = (V8_9CE0 *)(arg0 + *(int *)(arg0 + 0x1C) * 8);
    d[8] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D10);
#ifdef SKIP_ASM
extern "C" void func_002B9D10(void *p, int v) {
    (*(int**)((char*)p + 0x74))[*(int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D28);
#ifdef SKIP_ASM
extern "C" void func_002B9D28(void *p, int v) {
    (*(int**)((char*)p + 0x78))[*(int*)((char*)p + 0x1C)] = v;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D40);
#ifdef SKIP_ASM
extern "C" void func_002B9D40(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x7C);
    base[*(int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D58);
#ifdef SKIP_ASM
extern "C" void func_002B9D58(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x80);
    base[*(int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9D70);
#ifdef SKIP_ASM
extern "C" void func_002B9D70(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 0x84);
    base[*(int *)((char *)arg0 + 0x1C)] = arg1;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E70);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void*, int);
extern "C" void func_002B9E70(void *a) {
    func_00289DF0(a, 0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9E90);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void*, int);
extern "C" void func_002B9E90(void *a) {
    func_00289DF0(a, 1);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EB0);
#ifdef SKIP_ASM
struct S9EB0 { int a; char pad[0x5C]; };
extern "C" int func_002B9EB0(void *arg0, int arg1) {
    S9EB0 *base = *(S9EB0 **)((char *)arg0 + 0xACC);
    return base[arg1].a;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EC8);
#ifdef SKIP_ASM
struct S9EC8 { int a; char pad[0x5C]; };
extern "C" int func_002B9EC8(void *arg0, int arg1) {
    S9EC8 *base = *(S9EC8 **)((char *)arg0 + 0xACC);
    return base[arg1].a == 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9EE8);
#ifdef SKIP_ASM
struct S9EE8 { int a; char pad[0x5C]; };
extern "C" int func_002B9EE8(void *arg0, int arg1) {
    S9EE8 *base = *(S9EE8 **)((char *)arg0 + 0xACC);
    return base[arg1].a == 2;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9F08);
#ifdef SKIP_ASM
extern "C" int func_002B9F88(void *, int);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B65D0(int);

extern "C" int func_002B9F08(void *self, int idx) {
    int r = 0;
    func_003B58A0();
    bool ok = *(int *)((char*)(*(int *)((char*)self + 0xACC)) + idx * 0x60) != 1;
    if (!ok) {
        r = func_003B65D0(func_002B9F88(self, idx)) == 1;
    }
    func_003B58D8();
    return r;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9F88);
#ifdef SKIP_ASM
struct S9F88 { int a; int b; char pad[0x58]; };
extern "C" int func_002B9F88(void *arg0, int arg1) {
    S9F88 *base = *(S9F88 **)((char *)arg0 + 0xACC);
    bool ne = base[arg1].a != 1;
    if (!ne) {
        return base[arg1].b;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9FB8);
#ifdef SKIP_ASM
extern "C" void func_002B9FB8(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 4);
    base[*(int *)(*(char **)arg0 + 0x1F4)] = arg1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002B9FD8);
#ifdef SKIP_ASM
extern "C" void func_002B9FD8(void *arg0, int arg1) {
    int *base = *(int **)((char *)arg0 + 8);
    base[*(int *)(*(char **)arg0 + 0x1F4)] = arg1;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAC78);
#ifdef SKIP_ASM
struct S78 { char x[0xB]; };
extern "C" void *func_002BAC78(void *arg0) {
    return (S78 *)((char *)arg0 + 0x124) + *(int *)((char *)arg0 + 0x3F0);
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD00);
#ifdef SKIP_ASM
extern "C" char* func_002BAD00(char *p, int i) {
    int off = i * 0x14 + 0x158;
    return p + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD18);
#ifdef SKIP_ASM
extern "C" char* func_002BAD18(char *p, int i) {
    int off = i * 0x54 + 0x1C;
    return p + off;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD30);
#ifdef SKIP_ASM
extern "C" void func_002BAD30(void *p) {
    *(int*)((char*)p + 0x5790) = 0;
    *(int*)((char*)p + 0x578C) = 1;
    *(int*)((char*)p + 0x6254) = 1;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAD80);
#ifdef SKIP_ASM
extern "C" char* func_002BAD80(char *p, int i) {
    int off = i * 0x14 + 0x60A4;
    return p + off;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BAF08);
#ifdef SKIP_ASM
void func_002C17C0(void *);
struct E_AF08 { int a; int off; };
extern "C" void func_002BAF08(void *self) {
    char *base = (char*)self;
    *(char**)base = base + *(int*)base;
    func_002C17C0(*(char**)base);
    *(char**)(base + 8) = base + *(int*)(base + 8);
    E_AF08 *e = *(E_AF08**)(base + 8);
    int i;
    for (i = 0; i < *(int*)(base + 4); i++, e++) {
        int off = e->off;
        if (off != -1) {
            e->off = (int)(base + off);
        } else {
            e->off = 0;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB140);
#ifdef SKIP_ASM
void cMemMan_free(void *);
void operator_delete(int *);
extern "C" void func_002BB390(void *, int);
struct S12_B140 { int a, b, c; };
extern "C" void func_002BB140(void *self, int flags) {
    S12_B140 *arr = *(S12_B140**)((char*)self + 4);
    if (arr != 0) {
        S12_B140 *p = arr + *(int*)((char*)arr - 0x10);
        if (arr != p) {
            do {
                p--;
                func_002BB390(p, 0);
            } while (*(S12_B140**)((char*)self + 4) != p);
        }
        cMemMan_free((char*)*(S12_B140**)((char*)self + 4) - 0x10);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB1D0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB2B8);
#ifdef SKIP_ASM
extern "C" void func_002BB3F0(void *);
extern "C" void func_002BB2B8(void *self) {
    unsigned i;
    for (i = 0; i < *(unsigned*)self; i++) {
        func_002BB3F0((char*)(*(void**)((char*)self + 4)) + i * 12);
    }
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB318);
#ifdef SKIP_ASM
extern "C" void func_002BB4D8(int, int);

extern "C" void func_002BB318(void *arg0, int arg1, int arg2) {
    func_002BB4D8((*(int *)((char*)(arg0) + (4))) + (arg1 * 0xC), arg2);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB348);
#ifdef SKIP_ASM
extern "C" void func_002BB570(void*, int, int);
extern "C" void func_002BB348(void *arg0, int arg1, int arg2) {
    func_002BB570((char*)*(void **)((char *)arg0 + 4) + arg1 * 12, arg1, arg2);
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB390);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void func_002BB390(void *self, int flags) {
    void *p = *(void**)((char*)self + 8);
    if (p != 0) {
        if (*(short*)((char*)self + 2) == 0) {
            cMemMan_free(p);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BB3F0);

INCLUDE_ASM("bx/seg_1BA100", func_002BB4D8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB570);
#ifdef SKIP_ASM
extern "C" void func_002BB5F0(void *);

extern "C" void func_002BB570(void *arg0, int arg1, int arg2) {
    (*(short *)((char*)(arg0) + (2))) = 1;
    (*(short *)((char*)(arg0) + (0))) = 2;
    (*(int *)((char*)(arg0) + (8))) = arg2;
    func_002BB5F0(arg0);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BB5A8);
#ifdef SKIP_ASM
extern "C" void cMemMan_free__FPv(int);

extern "C" void func_002BB5A8(void *arg0) {
    int temp_4;

    temp_4 = (*(int *)((char*)(arg0) + (8)));
    if (temp_4 != 0) {
        if ((*(short *)((char*)(arg0) + (2))) == 0) {
            cMemMan_free__FPv(temp_4);
        }
        (*(int *)((char*)(arg0) + (8))) = 0;
    }
    (*(short *)((char*)(arg0) + (0))) = 0;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BBBF8);
#ifdef SKIP_ASM
extern "C" int func_002BBBF8(int a, ...) {
    return a;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC6F8);
#ifdef SKIP_ASM
extern char D_00485AF8[];
extern "C" void *func_002BC6F8(void *arg0) {
    *(char **)((char *)arg0 + 4) = D_00485AF8;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(int *)((char *)arg0 + 0xC) = -1;
    *(int *)((char *)arg0 + 8) = -1;
    *(int *)((char *)arg0 + 0x14) = -1;
    *(int *)((char *)arg0 + 0x10) = -1;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC728);
#ifdef SKIP_ASM
extern char D_004858C0[];
extern "C" void *func_002BC728(void *arg0) {
    *(char **)((char *)arg0 + 4) = D_004858C0;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(int *)((char *)arg0 + 0xC) = -1;
    *(int *)((char *)arg0 + 8) = -1;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC750);
#ifdef SKIP_ASM
extern char D_00485688[];
extern "C" void *func_002BC750(void *arg0) {
    *(char **)((char *)arg0 + 4) = D_00485688;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(int *)((char *)arg0 + 0xC) = -1;
    *(int *)((char *)arg0 + 8) = -1;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC778);
#ifdef SKIP_ASM
extern char D_00485450[];
extern "C" void *func_002BC778(void *arg0) {
    *(char **)((char *)arg0 + 4) = D_00485450;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(int *)((char *)arg0 + 0xC) = -1;
    *(int *)((char *)arg0 + 8) = -1;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC7A0);
#ifdef SKIP_ASM
extern char D_00485218[];
extern "C" void *func_002BC7A0(void *arg0) {
    *(char **)((char *)arg0 + 4) = D_00485218;
    *(float *)((char *)arg0 + 0xC) = 30000.0f;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(float *)((char *)arg0 + 8) = 30000.0f;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC7C8);
#ifdef SKIP_ASM
extern char D_00484FE0[];
extern "C" void *func_002BC7C8(void *arg0) {
    float zero = 0.0f;
    float a = 3000.0f, b = 30000.0f, c = 0.43f, d = 0.55f, e = 0.71f;
    *(int **)((char*)arg0 + 4) = (int*)D_00484FE0;
    *(float *)((char*)arg0 + 0) = -99999.0f;
    *(float *)((char*)arg0 + 0xC) = zero;
    *(float *)((char*)arg0 + 0x14) = a;
    *(float *)((char*)arg0 + 0x1C) = b;
    *(float *)((char*)arg0 + 0x24) = c;
    *(float *)((char*)arg0 + 0x2C) = d;
    *(float *)((char*)arg0 + 0x34) = e;
    *(float *)((char*)arg0 + 0x8) = zero;
    *(float *)((char*)arg0 + 0x10) = a;
    *(float *)((char*)arg0 + 0x18) = b;
    *(float *)((char*)arg0 + 0x20) = c;
    *(float *)((char*)arg0 + 0x28) = d;
    *(float *)((char*)arg0 + 0x30) = e;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC830);
#ifdef SKIP_ASM
extern char D_00484DA8[];
extern "C" void *func_002BC830(void *self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    *(void**)((char*)self + 4) = D_00484DA8;
    p[3] = one;
    p[2] = one;
    p[5] = one;
    p[4] = one;
    p[7] = one;
    p[6] = one;
    p[9] = one;
    p[8] = one;
    p[11] = one;
    p[10] = one;
    p[13] = zero;
    p[12] = zero;
    p[15] = zero;
    p[14] = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC890);
#ifdef SKIP_ASM
extern char D_00484B70[];
extern "C" void *func_002BC890(void *self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    *(void**)((char*)self + 4) = D_00484B70;
    p[3] = one;
    p[2] = one;
    p[5] = one;
    p[4] = one;
    p[7] = one;
    p[6] = one;
    p[9] = zero;
    p[8] = zero;
    p[11] = zero;
    p[10] = zero;
    p[13] = zero;
    p[12] = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC8E8);
#ifdef SKIP_ASM
extern char D_00484938[];
extern "C" void *func_002BC8E8(void *arg0) {
    float zero = 0.0f;
    *(char **)((char *)arg0 + 4) = D_00484938;
    *(float *)((char *)arg0 + 0xC) = zero;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(float *)((char *)arg0 + 8) = zero;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC910);
#ifdef SKIP_ASM
extern char D_00484700[];
extern "C" void *func_002BC910(void *self) {
    float one = 1.0f, zero = 0.0f;
    float *p = (float*)self;
    p[0] = -99999.0f;
    *(void**)((char*)self + 4) = D_00484700;
    p[3] = zero;
    p[2] = zero;
    p[5] = zero;
    p[4] = zero;
    p[7] = one;
    p[6] = one;
    p[9] = one;
    p[8] = one;
    p[11] = one;
    p[10] = one;
    p[13] = one;
    p[12] = one;
    ((int*)p)[15] = 0;
    ((int*)p)[14] = 0;
    p[17] = one;
    p[16] = one;
    p[19] = one;
    p[18] = one;
    return self;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BC980);
#ifdef SKIP_ASM
extern char D_004844C8[];
extern "C" void *func_002BC980(void *arg0) {
    float one = 1.0f;
    *(char **)((char *)arg0 + 4) = D_004844C8;
    *(float *)((char *)arg0 + 0xC) = one;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(float *)((char *)arg0 + 8) = one;
    return arg0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BC9B0);

INCLUDE_ASM("bx/seg_1BA100", func_002BCAF8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BCBD0);
#ifdef SKIP_ASM
extern char D_00483E20[];
extern "C" void *func_002BCBD0(void *arg0) {
    float zero = 0.0f;
    *(char **)((char *)arg0 + 4) = D_00483E20;
    *(int *)((char *)arg0 + 0xC) = 0;
    *(int *)((char *)arg0 + 0x8) = 0;
    *(float *)((char *)arg0 + 0) = -99999.0f;
    *(float *)((char *)arg0 + 0x14) = zero;
    *(float *)((char *)arg0 + 0x10) = zero;
    return arg0;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BDA98);
#ifdef SKIP_ASM
extern "C" int func_002BDA98(void *arg0, void *arg1) {
    if ((*(int *)((char*)(arg0) + (8))) != (*(int *)((char*)(arg1) + (4)))) {
        return 0;
    }
    return (*(int *)((char*)(arg0) + (0x10))) == (*(int *)((char*)(arg1) + (8)));
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE078);
#ifdef SKIP_ASM
extern "C" int func_002BE078(void *arg0, void *arg1) {
    if ((*(int *)((char*)(arg0) + (8))) != (*(int *)((char*)(arg1) + (4)))) {
        return 0;
    }
    if ((*(float *)((char*)(arg0) + (0x10))) != (*(float *)((char*)(arg1) + (8)))) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE0B0);
#ifdef SKIP_ASM
extern "C" void func_002BE0B0(void *p) {
    *(int*)((char*)p + 0) = 0;
    *(int*)((char*)p + 8) = -1;
    *(int*)((char*)p + 0x10) = -1;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE108);
#ifdef SKIP_ASM
extern "C" void func_002BE108(void *arg0) {
    (*(float *)((char*)(arg0) + (0x10))) = 3000.0f;
    (*(float *)((char*)(arg0) + (0x18))) = 30000.0f;
    (*(float *)((char*)(arg0) + (0x20))) = 0.43f;
    (*(float *)((char*)(arg0) + (0x28))) = 0.55f;
    (*(float *)((char*)(arg0) + (0x30))) = 0.71f;
    (*(int *)((char*)(arg0) + (8))) = 0;
    (*(int *)((char*)(arg0) + (0))) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE140);
#ifdef SKIP_ASM
extern "C" void func_002BE140(void *arg0) {
    *(float *)((char *)arg0 + 0x8) = 1.0f;
    *(float *)((char *)arg0 + 0x10) = 1.0f;
    *(float *)((char *)arg0 + 0x18) = 1.0f;
    *(float *)((char *)arg0 + 0x20) = 1.0f;
    *(float *)((char *)arg0 + 0x28) = 1.0f;
    *(int *)((char *)arg0 + 0x30) = 0;
    *(int *)((char *)arg0 + 0x38) = 0;
    *(int *)((char *)arg0 + 0x0) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE170);
#ifdef SKIP_ASM
extern "C" void func_002BE170(void *arg0) {
    *(float *)((char *)arg0 + 0x8) = 1.0f;
    *(float *)((char *)arg0 + 0x10) = 1.0f;
    *(float *)((char *)arg0 + 0x18) = 1.0f;
    *(int *)((char *)arg0 + 0x20) = 0;
    *(int *)((char *)arg0 + 0x28) = 0;
    *(int *)((char *)arg0 + 0x30) = 0;
    *(int *)((char *)arg0 + 0x0) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE198);
#ifdef SKIP_ASM
extern "C" void func_002BE198(void *p) {
    *(int*)((char*)p + 8) = 0;
    *(int*)((char*)p + 0) = 0;
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE1A8);
#ifdef SKIP_ASM
extern "C" void func_002BE1A8(void *arg0) {
    *(int *)((char *)arg0 + 0x8) = 0;
    *(int *)((char *)arg0 + 0x10) = 0;
    *(float *)((char *)arg0 + 0x18) = 1.0f;
    *(float *)((char *)arg0 + 0x20) = 1.0f;
    *(float *)((char *)arg0 + 0x28) = 1.0f;
    *(float *)((char *)arg0 + 0x30) = 1.0f;
    *(int *)((char *)arg0 + 0x38) = 0;
    *(float *)((char *)arg0 + 0x40) = 1.0f;
    *(float *)((char *)arg0 + 0x48) = 1.0f;
    *(int *)((char *)arg0 + 0x0) = 0;
}
#endif

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

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE2C8);
#ifdef SKIP_ASM
extern "C" void func_002BE2C8(void *p) {
    *(int*)((char*)p + 8) = 0;
    *(int*)((char*)p + 0x10) = 0;
    *(int*)((char*)p + 0) = 0;
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BE2D8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE378);
#ifdef SKIP_ASM
extern "C" void func_002BE378(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE3E0);
#ifdef SKIP_ASM
extern "C" void func_002BE3E0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE448);
#ifdef SKIP_ASM
extern "C" void func_002BE448(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE4B0);
#ifdef SKIP_ASM
extern "C" void func_002BE4B0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BE518);

INCLUDE_ASM("bx/seg_1BA100", func_002BE698);

INCLUDE_ASM("bx/seg_1BA100", func_002BE850);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BE9D0);
#ifdef SKIP_ASM
extern "C" void func_002BE9D0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BEA38);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BEC60);
#ifdef SKIP_ASM
extern "C" void func_002BEC60(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x10))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0x14)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x10))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BECC8);

INCLUDE_ASM("bx/seg_1BA100", func_002BEE48);

INCLUDE_ASM("bx/seg_1BA100", func_002BF2A0);

INCLUDE_ASM("bx/seg_1BA100", func_002BF340);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF3E0);
#ifdef SKIP_ASM
extern "C" void func_002BF3E0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF448);
#ifdef SKIP_ASM
extern "C" void func_002BF448(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF4B0);
#ifdef SKIP_ASM
extern "C" void func_002BF4B0(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BF518);
#ifdef SKIP_ASM
extern "C" void func_002BF518(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BF580);

INCLUDE_ASM("bx/seg_1BA100", func_002BF700);

INCLUDE_ASM("bx/seg_1BA100", func_002BF8B8);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BFA38);
#ifdef SKIP_ASM
extern "C" void func_002BFA38(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BFAA0);

//100%
INCLUDE_ASM("bx/seg_1BA100", func_002BFCC8);
#ifdef SKIP_ASM
extern "C" void func_002BFCC8(int arg0, void **arg1) {
    void *temp_3;
    void *temp_3_2;

    temp_3 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3) + (0x8))), arg0 + 8, 4);
    temp_3_2 = *arg1;
    (*(int (**)(void *, int, int))((char*)(temp_3_2) + (0xC)))((char*)arg1 + (*(short *)((char*)(temp_3_2) + (0x8))), arg0, 4);
}
#endif

INCLUDE_ASM("bx/seg_1BA100", func_002BFD30);

INCLUDE_ASM("bx/seg_1BA100", func_002BFEB0);

INCLUDE_ASM("bx/seg_1BA100", func_002C0308);
