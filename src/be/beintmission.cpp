#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0045A848[];
extern void* D_0045ADF8[16];
extern void* D_004A1238;

struct cBEMissionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getThis__Fv);
#ifdef SKIP_ASM
void* cBEMissionInterface_getThis()
{
    if (D_004A1238 == 0) {
        cBEMissionInterface* mem = (cBEMissionInterface*)cMemMan_alloc(0x14, D_0045A848, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045ADF8;
        D_004A1238 = mem;
    }
    return D_004A1238;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153350);
#ifdef SKIP_ASM
extern signed char D_0043FA70[];

extern "C" int func_00153350(void* self, int start)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        sum += D_0043FA70[start + i];
    }
    return sum;
}
#endif

INCLUDE_ASM("be/beintmission", func_00153390);

INCLUDE_ASM("be/beintmission", func_00153498);

INCLUDE_ASM("be/beintmission", func_00153520);

INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForPeak);

INCLUDE_ASM("be/beintmission", func_00153688);

INCLUDE_ASM("be/beintmission", func_00153708);

INCLUDE_ASM("be/beintmission", func_001538E8);

INCLUDE_ASM("be/beintmission", func_00153B00);

INCLUDE_ASM("be/beintmission", func_00153C88);

INCLUDE_ASM("be/beintmission", func_00153D28);

INCLUDE_ASM("be/beintmission", func_00153D78);

INCLUDE_ASM("be/beintmission", func_00153DD0);

INCLUDE_ASM("be/beintmission", func_00153E28);

INCLUDE_ASM("be/beintmission", func_00153E80);

INCLUDE_ASM("be/beintmission", func_00153ED8);

INCLUDE_ASM("be/beintmission", func_00153FA0);

INCLUDE_ASM("be/beintmission", func_00153FD8);

INCLUDE_ASM("be/beintmission", func_00154010);

INCLUDE_ASM("be/beintmission", func_00154080);

INCLUDE_ASM("be/beintmission", func_001540F0);

INCLUDE_ASM("be/beintmission", func_00154160);

INCLUDE_ASM("be/beintmission", func_001541D0);

//100%
INCLUDE_ASM("be/beintmission", func_00154240);
#ifdef SKIP_ASM
struct sMissionDef_0043EE10
{
    int id;             // 0x00
    char pad_0x04[0xC];
    int group;          // 0x10
    char pad_0x14[0xE];
    short value;        // 0x22
};

extern sMissionDef_0043EE10 D_0043EE10[];

extern "C" int func_00154240(void* self, int id)
{
    for (int i = 0; i < 88; i++) {
        if (D_0043EE10[i].id == id) {
            return i;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("be/beintmission", func_00154278);

//100%
INCLUDE_ASM("be/beintmission", func_001542A0);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 D_0043EE10[];

extern "C" int func_001542A0(void* self, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (D_0043EE10[i].group == group) {
            n++;
        }
    }
    return n;
}
#endif

INCLUDE_ASM("be/beintmission", func_001542E0);

INCLUDE_ASM("be/beintmission", func_00154368);

INCLUDE_ASM("be/beintmission", func_001543E0);

INCLUDE_ASM("be/beintmission", func_001544D0);

INCLUDE_ASM("be/beintmission", func_00154588);

//100%
INCLUDE_ASM("be/beintmission", func_001545B0);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 D_0043EE10[];

extern "C" int func_001545B0(void* self, int index, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (D_0043EE10[i].group == group) {
            if (n == index) {
                return D_0043EE10[i].id;
            }
            n++;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001545F8);
#ifdef SKIP_ASM
struct sMissionEntry {
    int id;              // 0x00
    char pad_0x04[0x14];
    int value;           // 0x18
    char pad_0x1c[0x10];
};

extern sMissionEntry D_00440770[];

extern "C" int func_001545F8(void* self, int id)
{
    for (int i = 0; i < 22; i++) {
        if (D_00440770[i].id == id) {
            return D_00440770[i].value;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154630);
#ifdef SKIP_ASM
extern "C" int func_00154630(void* self, int value, int id)
{
    for (int i = 0; i < 22; i++) {
        if (D_00440770[i].id == id && D_00440770[i].value >= value) {
            return 1;
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("be/beintmission", func_00154678);

//100%
INCLUDE_ASM("be/beintmission", func_00154760);
#ifdef SKIP_ASM
extern "C" void* func_00154760(void* self, int id)
{
    for (int i = 0; i < 22; i++) {
        if (D_00440770[i].id == id) {
            return D_00440770[i].pad_0x04;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547A0);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 D_0043EE10[];

extern "C" int func_001547A0(void* self, int id)
{
    for (int i = 0; i < 88; i++) {
        if (D_0043EE10[i].id == id) {
            return D_0043EE10[i].value;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547D8__FPv);
#ifdef SKIP_ASM
void func_001547D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547E0__FPv);
#ifdef SKIP_ASM
void func_001547E0(void* self)
{
}
#endif

INCLUDE_ASM("be/beintmission", func_001547E8);

INCLUDE_ASM("be/beintmission", func_00154898);

