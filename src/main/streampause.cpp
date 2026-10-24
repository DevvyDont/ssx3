#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0047C520[];
extern void* D_0047D0E8[16];

struct cStreamPause {
    void* vtable;
};

//99.67%
INCLUDE_ASM("main/streampause", cStreamPause_construct__Fv);
#ifdef SKIP_ASM
cStreamPause* cStreamPause_construct()
{
    cStreamPause* self = (cStreamPause*)cMemMan_alloc(4, D_0047C520, 0x20000000, 0);
    self->vtable = D_0047D0E8;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/streampause", func_00242D50);
#ifdef SKIP_ASM
extern "C" void* func_00242D50(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("main/streampause", func_00242D90);
#ifdef SKIP_ASM
extern "C" int func_00242D90(signed char *first1, signed char *last1, signed char *first2, signed char *last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return 1;
        if (*first2 < *first1) return 0;
    }
    return first1 == last1 && first2 != last2;
}
#endif
//100%
INCLUDE_ASM("main/streampause", func_00242DF8);
#ifdef SKIP_ASM
extern "C" int func_00242DF8(signed char *first1, signed char *last1, signed char *first2, signed char *last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return -1;
        if (*first2 < *first1) return 1;
    }
    if (first2 == last2) return first1 != last1;
    return -1;
}
#endif
//100%
INCLUDE_ASM("main/streampause", func_00242E58);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00242E58(void *self) {
    int **head = *(int ***)((char*)self + 4);
    int **node = (int **)*head;
    int **next;

    while (node != head) {
        next = (int **)*node;
        operator_delete((int*)node);
        node = next;
        head = *(int ***)((char*)self + 4);
    }
    **(int ***)((char*)self + 4) = *(int **)((char*)self + 4);
    *(int ***)((char*)*(int ***)((char*)self + 4) + 4) = *(int ***)((char*)self + 4);
}
#endif

//100%
INCLUDE_ASM("main/streampause", func_00242EB8);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sV4_2EB8 {
    float x, y, z, w;
    sV4_2EB8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sSlot_2EB8 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_2EB8(int n) { a = n; b = n; }
};

extern "C" void* cSSXApp_cSSXApp(void* self);
void func_00317930(void* self);
extern "C" void cBXString__cBXString(void* self, int flags);
void cAppMan__cAppMan(void* self, int flags);

struct sStr_2EB8 {
    char* p;
    ~sStr_2EB8() { cBXString__cBXString(this, 2); }
};

struct sAppMan_2EB8 {
    char pad[0xC4];
    ~sAppMan_2EB8() { cAppMan__cAppMan(this, 2); }
};

struct sSSXApp_2EB8 : sAppMan_2EB8 {
    sStr_2EB8 s0;
    sStr_2EB8 s1;
    sStr_2EB8 s2;
};

extern char D_004C90A8[], D_004C90B8[], D_004C90C8[], D_004C90D8[], D_004C90E8[], D_004C90F8[], D_004C9108[], D_004C9118[], D_004C9128[], D_004C9138[], D_004C9148[], D_004C9158[], D_004C9168[], D_004C9178[], D_004C9188[], D_004C9198[], D_004C91A8[], D_004C91B8[], D_004C91C8[], D_004C91D8[], D_004C91E8[], D_004C91F8[], D_004C9208[], D_004C9218[], D_004C9228[], D_004C9238[], D_004C9248[], D_004C9258[], D_004C9268[], D_004C9278[], D_004C9288[], D_004C9298[], D_004C92A8[], D_004C92B8[], D_004C92C8[], D_004C92D8[], D_004C92E8[], D_004C92F8[], D_004C9308[], D_004C9318[], D_004C9328[], D_004C9338[], D_004C9348[], D_004C9358[], D_004C9368[], D_004C9378[], D_004C9388[], D_004C9398[], D_004C93A8[], D_004C93B8[], D_004C93C8[], D_004C93D8[], D_004C93E8[], D_004C93F8[], D_004C9408[], D_004C9418[];
extern char D_004C9428[];
extern char D_004C9548[];
extern int D_004A4F70;
extern int D_004A4F74;
extern sSlot_2EB8 D_004A4F68, D_004A4F78, D_004A4F80, D_004A4F88, D_004A4F90, D_004A4F98, D_004A4FA0, D_004A4FA8, D_004A4FB0, D_004A4FB8, D_004A4FC0, D_004A4FC8, D_004A4FD0, D_004A4FD8, D_004A4FE0;

extern "C" void func_00242EB8(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            new (D_004C90A8) sV4_2EB8(1.0f, 1.0f, 1.0f, 0.5f);
            new (D_004C90B8) sV4_2EB8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
            new (D_004C90C8) sV4_2EB8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
            new (D_004C90D8) sV4_2EB8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
            new (D_004C90E8) sV4_2EB8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
            new (D_004C90F8) sV4_2EB8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
            new (D_004C9108) sV4_2EB8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
            new (D_004C9118) sV4_2EB8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
            new (D_004C9128) sV4_2EB8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
            new (D_004C9138) sV4_2EB8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
            new (D_004C9148) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9158) sV4_2EB8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C9168) sV4_2EB8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
            new (D_004C9178) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9188) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9198) sV4_2EB8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C91A8) sV4_2EB8(1.0f, 0.0f, 0.0f, 0.0f);
            new (D_004C91B8) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C91C8) sV4_2EB8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C91D8) sV4_2EB8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C91E8) sV4_2EB8(1.0f, 0.5f, 1.0f, 0.5f);
            new (D_004C91F8) sV4_2EB8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
            new (D_004C9208) sV4_2EB8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
            new (D_004C9218) sV4_2EB8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
            new (D_004C9228) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9238) sV4_2EB8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
            new (D_004C9248) sV4_2EB8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
            new (D_004C9258) sV4_2EB8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
            new (D_004C9268) sV4_2EB8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
            new (D_004C9278) sV4_2EB8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
            new (D_004C9288) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9298) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C92A8) sV4_2EB8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
            new (D_004C92B8) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C92C8) sV4_2EB8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
            new (D_004C92D8) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C92E8) sV4_2EB8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C92F8) sV4_2EB8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
            new (D_004C9308) sV4_2EB8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
            new (D_004C9318) sV4_2EB8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
            new (D_004C9328) sV4_2EB8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
            new (D_004C9338) sV4_2EB8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
            new (D_004C9348) sV4_2EB8(1.0f, 1.0f, 1.0f, 1.0f);
            new (D_004C9358) sV4_2EB8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
            new (D_004C9368) sV4_2EB8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C9378) sV4_2EB8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C9388) sV4_2EB8(1.0f, 1.0f, 0.0f, 0.0f);
            new (D_004C9398) sV4_2EB8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
            new (D_004C93A8) sV4_2EB8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
            new (D_004C93B8) sV4_2EB8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
            new (D_004C93C8) sV4_2EB8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
            new (D_004C93D8) sV4_2EB8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
            new (D_004C93E8) sV4_2EB8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
            new (D_004C93F8) sV4_2EB8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
            new (D_004C9408) sV4_2EB8(1.0f, 0.0f, 1.0f, 0.0f);
            new (D_004C9418) sV4_2EB8(1.0f, 1.0f, 0.0f, 0.0f);
            new (&D_004A4F68) sSlot_2EB8(-1);
            D_004A4F74 = 0;
            D_004A4F70 = 0;
            new (&D_004A4F78) sSlot_2EB8(0);
            new (&D_004A4F80) sSlot_2EB8(1);
            new (&D_004A4F88) sSlot_2EB8(2);
            new (&D_004A4F90) sSlot_2EB8(3);
            new (&D_004A4F98) sSlot_2EB8(4);
            new (&D_004A4FA0) sSlot_2EB8(5);
            new (&D_004A4FA8) sSlot_2EB8(6);
            new (&D_004A4FB0) sSlot_2EB8(7);
            new (&D_004A4FB8) sSlot_2EB8(8);
            new (&D_004A4FC0) sSlot_2EB8(9);
            new (&D_004A4FC8) sSlot_2EB8(10);
            new (&D_004A4FD0) sSlot_2EB8(11);
            new (&D_004A4FD8) sSlot_2EB8(12);
            new (&D_004A4FE0) sSlot_2EB8(13);
            cSSXApp_cSSXApp(D_004C9428);
            func_00317930(D_004C9548);
        } else {
            ((sSSXApp_2EB8*)D_004C9428)->~sSSXApp_2EB8();
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/streampause", func_00243838);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0047D6E8[];
extern void* D_0046D970[];

extern "C" void func_00243838(void* self, int flags)
{
    void** m = D_0047D6E8;
    *(void***)((char*)self + 0x0) = D_0046D970;
    *(void***)((char*)self + 0x1F8) = m;
    *(void***)((char*)self + 0x1E8) = m;
    *(void***)((char*)self + 0x1C4) = m;
    *(void***)((char*)self + 0x1A8) = m;
    *(void***)((char*)self + 0x198) = m;
    *(void***)((char*)self + 0x184) = m;
    *(void***)((char*)self + 0x164) = m;
    *(void***)((char*)self + 0x154) = m;
    *(void***)((char*)self + 0x144) = m;
    *(void***)((char*)self + 0x134) = m;
    *(void***)((char*)self + 0x124) = m;
    *(void***)((char*)self + 0x10C) = m;
    *(void***)((char*)self + 0xFC) = m;
    *(void***)((char*)self + 0xEC) = m;
    *(void***)((char*)self + 0xDC) = m;
    *(void***)((char*)self + 0xBC) = m;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif


//100%
INCLUDE_ASM("main/streampause", func_002438B0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_002438B0(void* self, int flags)
{
    *(void***)((char*)self + 0x0) = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif
//100%
INCLUDE_ASM("main/streampause", func_002438E0);
#ifdef SKIP_ASM
extern "C" void func_002438E0(void) {

}
#endif
//100%
INCLUDE_ASM("main/streampause", func_002438E8);
#ifdef SKIP_ASM
extern "C" void func_002438E8(void) {

}
#endif
