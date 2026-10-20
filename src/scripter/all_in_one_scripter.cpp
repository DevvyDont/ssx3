#include "common.h"

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", __sti__all_in_one_scripter_cpp);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_3E60 {
    float x, y, z, w;
    sV4_3E60(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sSlot_3E60 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_3E60(int n) { a = n; b = n; }
};

int GetHashValue32(char* str);

extern char D_004D3798[], D_004D37A8[], D_004D37B8[], D_004D37C8[], D_004D37D8[], D_004D37E8[], D_004D37F8[], D_004D3808[], D_004D3818[], D_004D3828[], D_004D3838[], D_004D3848[], D_004D3858[], D_004D3868[], D_004D3878[], D_004D3888[], D_004D3898[], D_004D38A8[], D_004D38B8[], D_004D38C8[], D_004D38D8[], D_004D38E8[], D_004D38F8[], D_004D3908[], D_004D3918[], D_004D3928[], D_004D3938[], D_004D3948[], D_004D3958[], D_004D3968[], D_004D3978[], D_004D3988[], D_004D3998[], D_004D39A8[], D_004D39B8[], D_004D39C8[], D_004D39D8[], D_004D39E8[], D_004D39F8[], D_004D3A08[], D_004D3A18[], D_004D3A28[], D_004D3A38[], D_004D3A48[], D_004D3A58[], D_004D3A68[], D_004D3A78[], D_004D3A88[], D_004D3A98[], D_004D3AA8[], D_004D3AB8[], D_004D3AC8[], D_004D3AD8[], D_004D3AE8[], D_004D3AF8[], D_004D3B08[];
extern char D_004825F8[];
extern int D_004A51E8;
extern int D_004A51EC;
extern int D_004A525C;
extern sSlot_3E60 D_004A51E0, D_004A51F0, D_004A51F8, D_004A5200, D_004A5208, D_004A5210, D_004A5218, D_004A5220, D_004A5228, D_004A5230, D_004A5238, D_004A5240, D_004A5248, D_004A5250, D_004A5258;

extern "C" void __sti__all_in_one_scripter_cpp(int init, int prio)
{
    if (prio == 0xFFFF && init != 0) {
        new (&D_004A51E0) sSlot_3E60(-1);
        new (D_004D3798) sV4_3E60(1.0f, 1.0f, 1.0f, 0.5f);
        new (D_004D37A8) sV4_3E60(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (D_004D37B8) sV4_3E60(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (D_004D37C8) sV4_3E60(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (D_004D37D8) sV4_3E60(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (D_004D37E8) sV4_3E60(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (D_004D37F8) sV4_3E60(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (D_004D3808) sV4_3E60(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (D_004D3818) sV4_3E60(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (D_004D3828) sV4_3E60(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (D_004D3838) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3848) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3858) sV4_3E60(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (D_004D3868) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3878) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3888) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D3898) sV4_3E60(1.0f, 0.0f, 0.0f, 0.0f);
        new (D_004D38A8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D38B8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38C8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38D8) sV4_3E60(1.0f, 0.5f, 1.0f, 0.5f);
        new (D_004D38E8) sV4_3E60(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (D_004D38F8) sV4_3E60(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (D_004D3908) sV4_3E60(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (D_004D3918) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3928) sV4_3E60(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (D_004D3938) sV4_3E60(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (D_004D3948) sV4_3E60(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (D_004D3958) sV4_3E60(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (D_004D3968) sV4_3E60(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (D_004D3978) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3988) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3998) sV4_3E60(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (D_004D39A8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D39B8) sV4_3E60(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (D_004D39C8) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D39D8) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D39E8) sV4_3E60(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (D_004D39F8) sV4_3E60(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (D_004D3A08) sV4_3E60(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (D_004D3A18) sV4_3E60(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (D_004D3A28) sV4_3E60(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (D_004D3A38) sV4_3E60(1.0f, 1.0f, 1.0f, 1.0f);
        new (D_004D3A48) sV4_3E60(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (D_004D3A58) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A68) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A78) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        new (D_004D3A88) sV4_3E60(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (D_004D3A98) sV4_3E60(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (D_004D3AA8) sV4_3E60(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (D_004D3AB8) sV4_3E60(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (D_004D3AC8) sV4_3E60(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (D_004D3AD8) sV4_3E60(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (D_004D3AE8) sV4_3E60(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (D_004D3AF8) sV4_3E60(1.0f, 0.0f, 1.0f, 0.0f);
        new (D_004D3B08) sV4_3E60(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A51EC = 0;
        D_004A51E8 = 0;
        new (&D_004A51F0) sSlot_3E60(0);
        new (&D_004A51F8) sSlot_3E60(1);
        new (&D_004A5200) sSlot_3E60(2);
        new (&D_004A5208) sSlot_3E60(3);
        new (&D_004A5210) sSlot_3E60(4);
        new (&D_004A5218) sSlot_3E60(5);
        new (&D_004A5220) sSlot_3E60(6);
        new (&D_004A5228) sSlot_3E60(7);
        new (&D_004A5230) sSlot_3E60(8);
        new (&D_004A5238) sSlot_3E60(9);
        new (&D_004A5240) sSlot_3E60(10);
        new (&D_004A5248) sSlot_3E60(11);
        new (&D_004A5250) sSlot_3E60(12);
        new (&D_004A5258) sSlot_3E60(13);
        D_004A525C = GetHashValue32(D_004825F8);
    }
}
#endif

extern "C" void func_002847A8(void) {
}

extern "C" void func_002847B0(void) {
}

extern "C" void func_002847B8(void) {
}

extern "C" void func_002847C0(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002847C8);
#ifdef SKIP_ASM
extern "C" int func_002847C8(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002847D0);
#ifdef SKIP_ASM
extern "C" int func_002847D0(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002847D8);
#ifdef SKIP_ASM
extern "C" int func_002847D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (8))) > 0;
}
#endif

extern "C" void func_002847E8(void) {
}

extern "C" void func_002847F0(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002847F8);
#ifdef SKIP_ASM
extern "C" int func_002847F8(void) {
    return 1;
}
#endif

extern "C" void func_00284800(void) {
}

extern "C" void func_00284808(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284810);
#ifdef SKIP_ASM
extern "C" int func_00284810(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284818);
#ifdef SKIP_ASM
extern "C" int func_00284818(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x2B4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284820);
#ifdef SKIP_ASM
extern "C" int func_00284820(void) {
    return 0;
}
#endif

extern "C" void func_00284828(void) {
}

extern "C" void func_00284830(void) {
}

extern "C" void func_00284838(void) {
}

extern "C" void func_00284840(void) {
}

extern "C" void func_00284848(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284850);
#ifdef SKIP_ASM
extern "C" int func_00284850(void *arg0) {
    return (*(int *)((char*)(arg0) + (8)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284858);
#ifdef SKIP_ASM
extern "C" int func_00284858(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284860);
#ifdef SKIP_ASM
extern "C" void func_00284860(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (4))) = arg1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284868);
#ifdef SKIP_ASM
extern "C" void func_00284868(void *arg0) {
    (*(int *)((char*)(arg0) + (4))) = -1;
}
#endif

extern "C" void func_00284878(void) {
}

extern "C" void func_00284880(void) {
}

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284888);
#ifdef SKIP_ASM
extern "C" int func_00284888(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284890);
#ifdef SKIP_ASM
extern "C" void func_00284890(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x10))) = arg1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284898);
#ifdef SKIP_ASM
extern "C" int func_00284898(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xC)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002848A0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00283228_48A0(void *) __asm__("func_00283228");
extern char D_00482360[];
extern char D_00482398[];

extern "C" void func_002848A0(void *arg0) {
    *(char **)((char*)arg0 + 0x10) = D_00482360;
    *(char **)((char*)arg0 + 0xC) = D_00482398;
    func_00283228_48A0(arg0);
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002848D0);
#ifdef SKIP_ASM
extern "C" int func_002848D0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xF8)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002848D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00283228_48D8(void *, int) __asm__("func_00283228");
void operator_delete(int *);
extern char D_004822E0[];
extern char D_00482318[];

extern "C" void func_002848D8(void *arg0, int arg1) {
    *(char **)((char*)arg0 + 0x18) = D_00482318;
    *(char **)((char*)arg0 + 0x0) = D_004822E0;
    func_00283228_48D8((char*)arg0 + 0xC, 0);
    if (arg1 & 1) {
        operator_delete((int *)arg0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284940);
#ifdef SKIP_ASM
extern "C" int func_00284940(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284948);
#ifdef SKIP_ASM
extern "C" int func_00284948(int arg0) {
    return arg0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284950);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00283228_4950(void *, int) __asm__("func_00283228");
void operator_delete(int *);
extern char D_00482260[];
extern char D_00482298[];

extern "C" void func_00284950(void *arg0, int arg1) {
    *(char **)((char*)arg0 + 0x18) = D_00482298;
    *(char **)((char*)arg0 + 0x0) = D_00482260;
    func_00283228_4950((char*)arg0 + 0xC, 0);
    if (arg1 & 1) {
        operator_delete((int *)arg0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849B8);
#ifdef SKIP_ASM
extern "C" int func_002849B8(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849C0);
#ifdef SKIP_ASM
extern "C" int func_002849C0(int arg0) {
    return arg0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849C8);
#ifdef SKIP_ASM
extern "C" int func_002849C8(int *arg0) {
    return *arg0 != 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849D8);
#ifdef SKIP_ASM
extern "C" int func_002849D8(void *arg0) {
    return (*(int *)((char*)(arg0) + (4)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849E0);
#ifdef SKIP_ASM
extern "C" int func_002849E0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x550))) != 2;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849F0);
#ifdef SKIP_ASM
extern "C" int func_002849F0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x550)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_002849F8);
#ifdef SKIP_ASM
struct sSlot_49F8 {
    int f0;     // 0x628
    int f4;
    int f8;     // 0x630
    int fC;
};

struct sEngine_49F8 {
    char pad[0x628];
    sSlot_49F8 slots[1];
};

extern "C" int func_002849F8(sEngine_49F8* self, int idx)
{
    int r = 0;
    if (self->slots[idx].f8 == 0) {
        r = self->slots[idx].f0 == 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A28);
#ifdef SKIP_ASM
extern "C" void func_00283228();

extern "C" void func_00284A28(void) {
    func_00283228();
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A48);
#ifdef SKIP_ASM
extern "C" int func_00284A48(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A50);
#ifdef SKIP_ASM
extern "C" float func_00284A50(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A58);
#ifdef SKIP_ASM
extern "C" int func_00284A58(int arg0) {
    return arg0 + 0x30;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A60);
#ifdef SKIP_ASM
extern "C" float func_00284A60(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A68);
#ifdef SKIP_ASM
extern "C" float func_00284A68(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x74)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A70);
#ifdef SKIP_ASM
extern "C" float func_00284A70(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x78)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A78);
#ifdef SKIP_ASM
extern "C" int func_00284A78(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x7C)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A80);
#ifdef SKIP_ASM
extern "C" int func_00284A80(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A88);
#ifdef SKIP_ASM
extern "C" int func_00284A88(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A90);
#ifdef SKIP_ASM
extern "C" int func_00284A90(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284A98);
#ifdef SKIP_ASM
extern "C" void func_00284A98(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0x1C))) = arg1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AA0);
#ifdef SKIP_ASM
extern "C" int func_00284AA0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x1C)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AA8);
#ifdef SKIP_ASM
extern "C" void func_00283228();

extern "C" void func_00284AA8(void) {
    func_00283228();
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AC8);
#ifdef SKIP_ASM
extern "C" int func_00284AC8(int arg0) {
    return arg0 + 0x18;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AD0);
#ifdef SKIP_ASM
extern "C" int func_00284AD0(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x10)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AD8);
#ifdef SKIP_ASM
extern "C" int func_00284AD8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AE0);
#ifdef SKIP_ASM
extern "C" int func_00284AE0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284AE8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_00283228_4AE8(void *, int) __asm__("func_00283228");
void operator_delete(int *);
extern char D_00481FD8[];
extern char D_00482018[];

extern "C" void func_00284AE8(void *arg0, int arg1) {
    *(char **)((char*)arg0 + 0x18) = D_00482018;
    *(char **)((char*)arg0 + 0x0) = D_00481FD8;
    func_00283228_4AE8((char*)arg0 + 0xC, 0);
    if (arg1 & 1) {
        operator_delete((int *)arg0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B50);
#ifdef SKIP_ASM
extern "C" int func_00284B50(void) {
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B58);
#ifdef SKIP_ASM
extern "C" int func_00284B58(int arg0) {
    return arg0;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B60);
#ifdef SKIP_ASM
extern "C" int func_00284B60(int arg0) {
    return arg0 + 0x30;
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B68);
#ifdef SKIP_ASM
extern "C" float func_00284B68(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x70)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B70);
#ifdef SKIP_ASM
extern "C" float func_00284B70(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x74)));
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B78);
#ifdef SKIP_ASM
extern "C" float func_00284B78(void *arg0) {
    return (*(float *)((char*)(arg0) + (0x78)));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284B80);
#ifdef SKIP_ASM
extern "C" void __sti__all_in_one_scripter_cpp(int, int);

extern "C" void func_00284B80(void) {
    __sti__all_in_one_scripter_cpp(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("scripter/all_in_one_scripter", func_00284BA0);
#ifdef SKIP_ASM
extern "C" int func_00284BA0(int arg0) {
    return (arg0 != 0) ? 0 : 0x173;
}
#endif
