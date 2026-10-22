#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldPainterQuery_cWorldPainterQuery(void* self, void* arg);
extern const char D_00483DE8[];

struct cWorldPainterManArg {
    void* field_0x0;
};

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", cWorldPainterMan_getQuery__FPvP19cWorldPainterManArg);
#ifdef SKIP_ASM
void* cWorldPainterMan_getQuery(void* self, cWorldPainterManArg* x)
{
    void* mem = cMemMan_alloc(0x18, D_00483DE8, 0, 0);
    void* arg = x->field_0x0;
    return cWorldPainterQuery_cWorldPainterQuery(mem, arg);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0AA0);
#ifdef SKIP_ASM
extern "C" void func_002C0AA0(void* self, int* dst, int* src, float t)
{
    if (t == 1.0f) {
        *dst = *src;
    } else {
        *dst = (int)((float)*dst * (1.0f - t) + (float)*src * t);
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0AF0);
#ifdef SKIP_ASM
extern "C" void func_002C0AF0(void* self, float* dst, float* src, float t)
{
    if (t == 1.0f) {
        *dst = *src;
    } else {
        *dst = *dst * (1.0f - t) + *src * t;
    }
}
#endif
//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0B30);
#ifdef SKIP_ASM
extern "C" void* func_002C0B30(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C0B70);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_2C0B70 {
    float r, g, b, a;
    sColor_2C0B70(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};


struct sFlags_2C0B70 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_2C0B70(unsigned char aa, unsigned int ab) : a(aa), b(ab) {}
};

extern int D_004A53D0;
extern int D_004A53D4;
extern sFlags_2C0B70 D_004A53D8;
extern sFlags_2C0B70 D_004A53E0;
extern sFlags_2C0B70 D_004A53E8;
extern sFlags_2C0B70 D_004A53F0;
extern sFlags_2C0B70 D_004A53F8;
extern sFlags_2C0B70 D_004A5400;
extern sFlags_2C0B70 D_004A5408;
extern sFlags_2C0B70 D_004A5410;
extern sFlags_2C0B70 D_004A5418;
extern sFlags_2C0B70 D_004A5420;
extern sFlags_2C0B70 D_004A5428;
extern sFlags_2C0B70 D_004A5430;
extern sFlags_2C0B70 D_004A5438;
extern sFlags_2C0B70 D_004A5440;

extern sColor_2C0B70 D_004D4638;
extern sColor_2C0B70 D_004D4648;
extern sColor_2C0B70 D_004D4658;
extern sColor_2C0B70 D_004D4668;
extern sColor_2C0B70 D_004D4678;
extern sColor_2C0B70 D_004D4688;
extern sColor_2C0B70 D_004D4698;
extern sColor_2C0B70 D_004D46A8;
extern sColor_2C0B70 D_004D46B8;
extern sColor_2C0B70 D_004D46C8;
extern sColor_2C0B70 D_004D46D8;
extern sColor_2C0B70 D_004D46E8;
extern sColor_2C0B70 D_004D46F8;
extern sColor_2C0B70 D_004D4708;
extern sColor_2C0B70 D_004D4718;
extern sColor_2C0B70 D_004D4728;
extern sColor_2C0B70 D_004D4738;
extern sColor_2C0B70 D_004D4748;
extern sColor_2C0B70 D_004D4758;
extern sColor_2C0B70 D_004D4768;
extern sColor_2C0B70 D_004D4778;
extern sColor_2C0B70 D_004D4788;
extern sColor_2C0B70 D_004D4798;
extern sColor_2C0B70 D_004D47A8;
extern sColor_2C0B70 D_004D47B8;
extern sColor_2C0B70 D_004D47C8;
extern sColor_2C0B70 D_004D47D8;
extern sColor_2C0B70 D_004D47E8;
extern sColor_2C0B70 D_004D47F8;
extern sColor_2C0B70 D_004D4808;
extern sColor_2C0B70 D_004D4818;
extern sColor_2C0B70 D_004D4828;
extern sColor_2C0B70 D_004D4838;
extern sColor_2C0B70 D_004D4848;
extern sColor_2C0B70 D_004D4858;
extern sColor_2C0B70 D_004D4868;
extern sColor_2C0B70 D_004D4878;
extern sColor_2C0B70 D_004D4888;
extern sColor_2C0B70 D_004D4898;
extern sColor_2C0B70 D_004D48A8;
extern sColor_2C0B70 D_004D48B8;
extern sColor_2C0B70 D_004D48C8;
extern sColor_2C0B70 D_004D48D8;
extern sColor_2C0B70 D_004D48E8;
extern sColor_2C0B70 D_004D48F8;
extern sColor_2C0B70 D_004D4908;
extern sColor_2C0B70 D_004D4918;
extern sColor_2C0B70 D_004D4928;
extern sColor_2C0B70 D_004D4938;
extern sColor_2C0B70 D_004D4948;
extern sColor_2C0B70 D_004D4958;
extern sColor_2C0B70 D_004D4968;
extern sColor_2C0B70 D_004D4978;
extern sColor_2C0B70 D_004D4988;
extern sColor_2C0B70 D_004D4998;
extern sColor_2C0B70 D_004D49A8;

// The unit declares func_002C0B70 later as returning void*; bind this body by asm label.
extern "C" void func_002C0B70_impl(int initialize, int priority) __asm__("func_002C0B70");

extern "C" void func_002C0B70_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004D4638) sColor_2C0B70(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D4648) sColor_2C0B70(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4658) sColor_2C0B70(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4668) sColor_2C0B70(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D4678) sColor_2C0B70(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D4688) sColor_2C0B70(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D4698) sColor_2C0B70(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D46A8) sColor_2C0B70(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D46B8) sColor_2C0B70(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D46C8) sColor_2C0B70(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D46D8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D46E8) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D46F8) sColor_2C0B70(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D4708) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4718) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4728) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4738) sColor_2C0B70(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4748) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4758) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4768) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4778) sColor_2C0B70(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D4788) sColor_2C0B70(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D4798) sColor_2C0B70(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D47A8) sColor_2C0B70(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D47B8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D47C8) sColor_2C0B70(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D47D8) sColor_2C0B70(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D47E8) sColor_2C0B70(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D47F8) sColor_2C0B70(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D4808) sColor_2C0B70(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D4818) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4828) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4838) sColor_2C0B70(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D4848) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4858) sColor_2C0B70(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D4868) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D4878) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4888) sColor_2C0B70(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D4898) sColor_2C0B70(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D48A8) sColor_2C0B70(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D48B8) sColor_2C0B70(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D48C8) sColor_2C0B70(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D48D8) sColor_2C0B70(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D48E8) sColor_2C0B70(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D48F8) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4908) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4918) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D4928) sColor_2C0B70(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D4938) sColor_2C0B70(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D4948) sColor_2C0B70(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D4958) sColor_2C0B70(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D4968) sColor_2C0B70(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D4978) sColor_2C0B70(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D4988) sColor_2C0B70(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D4998) sColor_2C0B70(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D49A8) sColor_2C0B70(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A53D4 = 0;
        D_004A53D0 = 0;
        new (&D_004A53D8) sFlags_2C0B70(0, 0);
        new (&D_004A53E0) sFlags_2C0B70(1, 1);
        new (&D_004A53E8) sFlags_2C0B70(2, 2);
        new (&D_004A53F0) sFlags_2C0B70(3, 3);
        new (&D_004A53F8) sFlags_2C0B70(4, 4);
        new (&D_004A5400) sFlags_2C0B70(5, 5);
        new (&D_004A5408) sFlags_2C0B70(6, 6);
        new (&D_004A5410) sFlags_2C0B70(7, 7);
        new (&D_004A5418) sFlags_2C0B70(8, 8);
        new (&D_004A5420) sFlags_2C0B70(9, 9);
        new (&D_004A5428) sFlags_2C0B70(10, 10);
        new (&D_004A5430) sFlags_2C0B70(11, 11);
        new (&D_004A5438) sFlags_2C0B70(12, 12);
        new (&D_004A5440) sFlags_2C0B70(13, 13);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1458__FPv);
#ifdef SKIP_ASM
void func_002C1458(void* self)
{
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1460);
#ifdef SKIP_ASM
extern void* D_00485D30[];
void operator_delete(int* ptr);

// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void func_002C1460(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00485D30;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1490__FPv);
#ifdef SKIP_ASM
void* func_002C1490(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1498__FPv);
#ifdef SKIP_ASM
void* func_002C1498(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14A0__FPv);
#ifdef SKIP_ASM
void* func_002C14A0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14A8__FPv);
#ifdef SKIP_ASM
void* func_002C14A8(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14B0__FPv);
#ifdef SKIP_ASM
void* func_002C14B0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14B8__FPv);
#ifdef SKIP_ASM
void* func_002C14B8(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14C0__FPv);
#ifdef SKIP_ASM
void* func_002C14C0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14C8__FPv);
#ifdef SKIP_ASM
void* func_002C14C8(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14D0__FPv);
#ifdef SKIP_ASM
void* func_002C14D0(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14D8__FPv);
#ifdef SKIP_ASM
void* func_002C14D8(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14E0__FPv);
#ifdef SKIP_ASM
void* func_002C14E0(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14E8__FPv);
#ifdef SKIP_ASM
void* func_002C14E8(void* self)
{
    return (char*)self + 0x30;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14F0__FPv);
#ifdef SKIP_ASM
void* func_002C14F0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C14F8__FPv);
#ifdef SKIP_ASM
void* func_002C14F8(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1500__FPv);
#ifdef SKIP_ASM
void* func_002C1500(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1508__FPv);
#ifdef SKIP_ASM
void* func_002C1508(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1510__FPv);
#ifdef SKIP_ASM
void* func_002C1510(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1518__FPv);
#ifdef SKIP_ASM
void* func_002C1518(void* self)
{
    return (char*)self + 0x30;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1520__FPv);
#ifdef SKIP_ASM
void* func_002C1520(void* self)
{
    return (char*)self + 0x38;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1528__FPv);
#ifdef SKIP_ASM
void* func_002C1528(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1530__FPv);
#ifdef SKIP_ASM
void* func_002C1530(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1538__FPv);
#ifdef SKIP_ASM
void* func_002C1538(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1540__FPv);
#ifdef SKIP_ASM
void* func_002C1540(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1548__FPv);
#ifdef SKIP_ASM
void* func_002C1548(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1550__FPv);
#ifdef SKIP_ASM
void* func_002C1550(void* self)
{
    return (char*)self + 0x30;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1558__FPv);
#ifdef SKIP_ASM
void* func_002C1558(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1560__FPv);
#ifdef SKIP_ASM
void* func_002C1560(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1568__FPv);
#ifdef SKIP_ASM
void* func_002C1568(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1570__FPv);
#ifdef SKIP_ASM
void* func_002C1570(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1578__FPv);
#ifdef SKIP_ASM
void* func_002C1578(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1580__FPv);
#ifdef SKIP_ASM
void* func_002C1580(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1588__FPv);
#ifdef SKIP_ASM
void* func_002C1588(void* self)
{
    return (char*)self + 0x30;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1590__FPv);
#ifdef SKIP_ASM
void* func_002C1590(void* self)
{
    return (char*)self + 0x38;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1598__FPv);
#ifdef SKIP_ASM
void* func_002C1598(void* self)
{
    return (char*)self + 0x40;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15A0__FPv);
#ifdef SKIP_ASM
void* func_002C15A0(void* self)
{
    return (char*)self + 0x48;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15A8__FPv);
#ifdef SKIP_ASM
void* func_002C15A8(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15B0__FPv);
#ifdef SKIP_ASM
void* func_002C15B0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15B8__FPv);
#ifdef SKIP_ASM
void* func_002C15B8(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15C0__FPv);
#ifdef SKIP_ASM
void* func_002C15C0(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15C8__FPv);
#ifdef SKIP_ASM
void* func_002C15C8(void* self)
{
    return (char*)self + 0x38;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15D0__FPv);
#ifdef SKIP_ASM
void* func_002C15D0(void* self)
{
    return (char*)self + 0x48;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15D8__FPv);
#ifdef SKIP_ASM
void* func_002C15D8(void* self)
{
    return (char*)self + 0x50;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15E0__FPv);
#ifdef SKIP_ASM
void* func_002C15E0(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15E8__FPv);
#ifdef SKIP_ASM
void* func_002C15E8(void* self)
{
    return (char*)self + 0x10;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15F0__FPv);
#ifdef SKIP_ASM
void* func_002C15F0(void* self)
{
    return (char*)self + 0x18;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C15F8__FPv);
#ifdef SKIP_ASM
void* func_002C15F8(void* self)
{
    return (char*)self + 0x20;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1600__FPv);
#ifdef SKIP_ASM
void* func_002C1600(void* self)
{
    return (char*)self + 0x28;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1608__FPv);
#ifdef SKIP_ASM
void* func_002C1608(void* self)
{
    return (char*)self + 0x30;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1610__FPv);
#ifdef SKIP_ASM
void* func_002C1610(void* self)
{
    return (char*)self + 0x38;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1618__FPv);
#ifdef SKIP_ASM
void* func_002C1618(void* self)
{
    return (char*)self + 0x40;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1620__FPv);
#ifdef SKIP_ASM
void* func_002C1620(void* self)
{
    return (char*)self + 0x48;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1628__FPv);
#ifdef SKIP_ASM
void* func_002C1628(void* self)
{
    return (char*)self + 0x50;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1630__FPv);
#ifdef SKIP_ASM
void* func_002C1630(void* self)
{
    return (char*)self + 0x58;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1638__FPv);
#ifdef SKIP_ASM
void* func_002C1638(void* self)
{
    return (char*)self + 0x60;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1640__FPv);
#ifdef SKIP_ASM
void* func_002C1640(void* self)
{
    return (char*)self + 0x68;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1648__FPv);
#ifdef SKIP_ASM
void* func_002C1648(void* self)
{
    return (char*)self + 0x70;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1650__FPv);
#ifdef SKIP_ASM
void* func_002C1650(void* self)
{
    return (char*)self + 0x78;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1658__FPv);
#ifdef SKIP_ASM
void* func_002C1658(void* self)
{
    return (char*)self + 0x80;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1660__FPv);
#ifdef SKIP_ASM
void* func_002C1660(void* self)
{
    return (char*)self + 0x88;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1668__FPv);
#ifdef SKIP_ASM
void* func_002C1668(void* self)
{
    return (char*)self + 0x90;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1670__FPv);
#ifdef SKIP_ASM
void* func_002C1670(void* self)
{
    return (char*)self + 0x98;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1678__FPv);
#ifdef SKIP_ASM
void* func_002C1678(void* self)
{
    return (char*)self + 0x8;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1680__FPv);
#ifdef SKIP_ASM
void* func_002C1680(void* self)
{
    return (char*)self + 0x10;
}
#endif

extern "C" void* func_002C0B70(int, int);

//99.38%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1688__FPv);
#ifdef SKIP_ASM
void* func_002C1688(void* self)
{
    return func_002C0B70(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C16A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// LANDING: later declaration (line ~656) is `extern "C" void func_002C16A8(int, int, int);` used by its caller
// with an int expression; retype it to (void*, int, int) and drop the alias if the lander prefers.
extern "C" void func_002C16A8_impl(void* self, int a, int b) __asm__("func_002C16A8");

extern "C" void func_002C16A8_impl(void* self, int a, int b)
{
    *(int*)((char*)self + 0x4) = a;
    *(int*)((char*)self + 0x0) = b;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C16B8);
#ifdef SKIP_ASM
extern "C" void func_002C16B8(unsigned short* p, unsigned short a, unsigned short b, unsigned short c, unsigned short d)
{
    p[0] = (a << 1) | 1;
    p[1] = (b << 1) | (p[1] & 1);
    p[2] = (c << 1) | (p[2] & 1);
    p[3] = (d << 1) | (p[3] & 1);
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1718);
#ifdef SKIP_ASM
extern "C" void func_002C1718(void *arg0) {
    (*(int *)((char*)(arg0) + (0))) = 0;
    (*(int *)((char*)(arg0) + (4))) = 0;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C1728);
#ifdef SKIP_ASM
extern "C" unsigned short cQuadTree_getFreeNode();
extern "C" void func_002C16A8(int, int, int);

extern "C" void *func_002C1728(void *arg0, int arg1, int arg2, float fparg0, float fparg1, float fparg2) {
    (*(int *)((char*)(arg0) + (0x24))) = 0;
    (*(int *)((char*)(arg0) + (0x20))) = 0;
    (*(int *)((char*)(arg0) + (0x10))) = 0;
    (*(int *)((char*)(arg0) + (0xC))) = 0;
    (*(float *)((char*)(arg0) + (4))) = fparg0;
    (*(float *)((char*)(arg0) + (8))) = fparg1;
    (*(float *)((char*)(arg0) + (0))) = (float) (32768.0f / fparg2);
    (*(unsigned short *)((char*)(arg0) + (0x14))) = cQuadTree_getFreeNode();
    (*(short *)((char*)(arg0) + (0x16))) = 0;
    func_002C16A8((*(int *)((char*)(arg0) + (0x20))) + ((*(unsigned short *)((char*)(arg0) + (0x14))) * 8), arg1, arg2);
    (*(int *)((char*)(arg0) + (0x1C))) = arg1;
    (*(int *)((char*)(arg0) + (0x18))) = arg2;
    return arg0;
}
#endif

//100%
INCLUDE_ASM("worldpainter/worldpaintermanquery", func_002C17C0__FPv);
#ifdef SKIP_ASM
void func_002C17C0(void* self)
{
    void* t0 = (char*)self + 0x28;
    *(void**)((char*)self + 0x20) = t0;
    *(void**)((char*)self + 0x24) = t0;
}
#endif

