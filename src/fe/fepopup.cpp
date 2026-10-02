#include "common.h"

INCLUDE_ASM("fe/fepopup", cScreenPopup_cScreenPopup);

INCLUDE_ASM("fe/fepopup", func_001C5A90);

INCLUDE_ASM("fe/fepopup", cScreenPopup_onCreateScreen);

extern "C" void* func_0039E4A0(void*);

//100%
INCLUDE_ASM("fe/fepopup", func_001C5B68__FPv);
#ifdef SKIP_ASM
void* func_001C5B68(void* self)
{
    *(int*)((char*)self + 0x2b8) = 0;
    return func_0039E4A0(self);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C5B88);

INCLUDE_ASM("fe/fepopup", cScreenPopup_onGainTransition);

//100%
INCLUDE_ASM("fe/fepopup", func_001C5DD0__FPv);
#ifdef SKIP_ASM
int func_001C5DD0(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C5DD8);

INCLUDE_ASM("fe/fepopup", func_001C5F20);

INCLUDE_ASM("fe/fepopup", cScreenPopup_FillObjectPointers);

INCLUDE_ASM("fe/fepopup", func_001C66E8);

INCLUDE_ASM("fe/fepopup", func_001C6B08);

INCLUDE_ASM("fe/fepopup", func_001C6D30);

INCLUDE_ASM("fe/fepopup", func_001C7040);

INCLUDE_ASM("fe/fepopup", func_001C70F0);

INCLUDE_ASM("fe/fepopup", func_001C7258);

INCLUDE_ASM("fe/fepopup", func_001C7388);

INCLUDE_ASM("fe/fepopup", func_001C75C8);

INCLUDE_ASM("fe/fepopup", func_001C7620);

//100%
INCLUDE_ASM("fe/fepopup", func_001C7738);
#ifdef SKIP_ASM
struct sColor_7738 { int r, g, b; };

extern "C" void func_001C7738(void* self, sColor_7738* color)
{
    sColor_7738 c = *color;
    switch (*(int*)((char*)self + 0x31C)) {
    case 0:
        {
            void* p = *(void**)((char*)self + 0x48);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x4C);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 1:
        {
            void* p = *(void**)((char*)self + 0x58);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x5C);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x50);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x54);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        {
            void* p = *(void**)((char*)self + 0x60);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    case 2:
        {
            void* p = *(void**)((char*)self + 0x68);
            if (p != 0) {
                *(sColor_7738*)((char*)p + 0x50) = c;
            }
        }
        break;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C78C0);

//100%
INCLUDE_ASM("fe/fepopup", func_001C7BE0);
#ifdef SKIP_ASM
extern "C" float func_001C7BE0(void* self)
{
    if (*(int*)((char*)self + 0x2CC) != 0) {
        return 0.0f;
    }
    return (640.0f - *(float*)((char*)self + 0x260)) * 0.5f - *(float*)((char*)self + 0x1B8);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C7C20);

INCLUDE_ASM("fe/fepopup", func_001C7EB0);

INCLUDE_ASM("fe/fepopup", func_001C8050);

INCLUDE_ASM("fe/fepopup", func_001C80E8);

INCLUDE_ASM("fe/fepopup", cScreenPopup_setTextString);

INCLUDE_ASM("fe/fepopup", func_001C8320);

INCLUDE_ASM("fe/fepopup", func_001C84A8);

INCLUDE_ASM("fe/fepopup", func_001C8568);

INCLUDE_ASM("fe/fepopup", func_001C8628);

INCLUDE_ASM("fe/fepopup", func_001C8678);

INCLUDE_ASM("fe/fepopup", func_001C8758);

//100%
INCLUDE_ASM("fe/fepopup", func_001C88C8);
#ifdef SKIP_ASM
struct sVec3_88C8 { float x, y, z; };

extern "C" void func_001C88C8(void* self)
{
    void* p = *(void**)((char*)self + 0x70);
    if (p != 0) {
        sVec3_88C8 v = *(sVec3_88C8*)((char*)self + 0x1AC);
        v.y += *(float*)((char*)self + 0x294);
        v.x += *(float*)((char*)self + 0x290);
        *(sVec3_88C8*)((char*)p + 0x44) = v;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001C8930);

INCLUDE_ASM("fe/fepopup", func_001C8A38);

INCLUDE_ASM("fe/fepopup", func_001C9038);

INCLUDE_ASM("fe/fepopup", func_001C9210);

INCLUDE_ASM("fe/fepopup", func_001C93B0);

INCLUDE_ASM("fe/fepopup", func_001C94B0);

INCLUDE_ASM("fe/fepopup", cScreenPopup_getMessageLineWrapSize);

INCLUDE_ASM("fe/fepopup", func_001C97E0);

INCLUDE_ASM("fe/fepopup", func_001C9938);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/fepopup", func_001C9B28);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" void func_001C9210(void* self, cUIText* text, const char* str, int a3);

extern "C" void func_001C9B28(void* self, cUIText* text, const char* str, int a3)
{
    func_001C9210(self, text, str, a3);
    cUIText_setAsciiString(text, str);
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001C9B68);
#ifdef SKIP_ASM
extern "C" void func_001C9B68(void* self, float* size, float* item)
{
    float w = item[2];
    if (size[0] < w) {
        size[0] = w;
    }
    size[1] += item[3];
}
#endif

INCLUDE_ASM("fe/fepopup", cScreenPopup_createConfirmationPopup);

INCLUDE_ASM("fe/fepopup", func_001C9CD0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA150);
#ifdef SKIP_ASM
extern "C" int func_001CA150(void* self)
{
    void* p = *(void**)((char*)self + 0x6C);
    if (p == 0) {
        return *(int*)((char*)self + 0x168);
    }
    int n = *(unsigned char*)((char*)p + 0x95);
    if (*(int*)((char*)self + 0x30C) == -1) {
        return n;
    }
    return n - 1;
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CA180);

INCLUDE_ASM("fe/fepopup", func_001CA298);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA488);
#ifdef SKIP_ASM
struct sVEntry_func_001CA488 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA488(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA488* vt = *(sVEntry_func_001CA488**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA4C0);
#ifdef SKIP_ASM
struct sVEntry_func_001CA4C0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001CA4C0(void* self, void* obj)
{
    if (obj != 0) {
        sVEntry_func_001CA4C0* vt = *(sVEntry_func_001CA4C0**)((char*)obj + 0x8);
        vt[9].fn((char*)obj + vt[9].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CA4F8);
#ifdef SKIP_ASM
class func_001CA4F8_cObj {
public:
    char pad[0x8];
    virtual int v01(int, int);
    virtual int v02(int, int);
    virtual int v03(int, int);
    virtual int v04(int, int);
    virtual int v05(int, int);
    virtual int v06(int, int);
    virtual int v07(int, int);
    virtual int v08(int, int);
    virtual int v09(int, int);
    virtual int v10(int, int);
    virtual int v11(int, int);
    virtual int v12(int, int);
    virtual int v13(int, int);
    virtual int v14(int, int);
    virtual int v15(int, int);
    virtual int v16(int, int);
    virtual int v17(int, int);
    virtual int v18(int, int);
    virtual int v19(int, int);
    virtual int v20(int, int);
    virtual int v21(int, int);
    virtual int v22(int, int);
    virtual int v23(int, int);
    virtual int v24(int, int);
};

extern "C" int func_001CA4F8(void* self, int a1, int a2)
{
    return (*(func_001CA4F8_cObj**)((char*)self + 0x20))->v24(a1, a2);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CA528);

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/fepopup", func_001CA578__FPv);
#ifdef SKIP_ASM
void* func_001CA578(void* self)
{
    return func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CA598);

INCLUDE_ASM("fe/fepopup", cScreenPopup_checkAndFixTextEntry);

INCLUDE_ASM("fe/fepopup", func_001CA8A0);

INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySongByCredit);

INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuySong);

INCLUDE_ASM("fe/fepopup", func_001CAA18);

INCLUDE_ASM("fe/fepopup", cBuyPopupInfo_initBuyBolt);

INCLUDE_ASM("fe/fepopup", func_001CABA8);

INCLUDE_ASM("fe/fepopup", func_001CAC30);

INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_onWidgetCreate);

INCLUDE_ASM("fe/fepopup", func_001CAED8);

INCLUDE_ASM("fe/fepopup", func_001CAF00);

INCLUDE_ASM("fe/fepopup", cUIStateBuyPopup_initBuyTrick);

INCLUDE_ASM("fe/fepopup", func_001CAFC0);

INCLUDE_ASM("fe/fepopup", func_001CB030);

INCLUDE_ASM("fe/fepopup", func_001CB138);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onCreateScreen);

INCLUDE_ASM("fe/fepopup", func_001CB208);

//100%
INCLUDE_ASM("fe/fepopup", func_001CB290);
#ifdef SKIP_ASM
extern "C" void func_0039F190(void* self, int a1);

extern "C" void func_001CB290(void* self, void* a1, unsigned int a2)
{
    switch (a2) {
    case 5:
    case 6:
        func_0039F190(*(char**)((char*)self + 0x10) + 0x18, 1);
        break;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onWidgetCreate);

INCLUDE_ASM("fe/fepopup", func_001CB3D0);

INCLUDE_ASM("fe/fepopup", func_001CB418);

INCLUDE_ASM("fe/fepopup", func_001CB500);

INCLUDE_ASM("fe/fepopup", func_001CB650);

INCLUDE_ASM("fe/fepopup", func_001CB7F0);

INCLUDE_ASM("fe/fepopup", func_001CB9B0);

INCLUDE_ASM("fe/fepopup", func_001CBB70);

INCLUDE_ASM("fe/fepopup", func_001CBC48);

INCLUDE_ASM("fe/fepopup", func_001CBCE8);

INCLUDE_ASM("fe/fepopup", func_001CBDB0);

INCLUDE_ASM("fe/fepopup", func_001CBF20);

INCLUDE_ASM("fe/fepopup", func_001CBFC0);

INCLUDE_ASM("fe/fepopup", func_001CC090);

INCLUDE_ASM("fe/fepopup", func_001CC0E0);

INCLUDE_ASM("fe/fepopup", func_001CC140);

INCLUDE_ASM("fe/fepopup", func_001CC190);

INCLUDE_ASM("fe/fepopup", func_001CC1E0);

INCLUDE_ASM("fe/fepopup", func_001CC620);

INCLUDE_ASM("fe/fepopup", func_001CC6E8);

INCLUDE_ASM("fe/fepopup", func_001CC7C8);

INCLUDE_ASM("fe/fepopup", func_001CC848);

INCLUDE_ASM("fe/fepopup", func_001CC8D8);

INCLUDE_ASM("fe/fepopup", func_001CCC30);

INCLUDE_ASM("fe/fepopup", func_001CCE98);

INCLUDE_ASM("fe/fepopup", func_001CCF00);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_setStatic);

INCLUDE_ASM("fe/fepopup", func_001CD020);

//100%
INCLUDE_ASM("fe/fepopup", func_001CD088);
#ifdef SKIP_ASM
extern "C" void func_001CD088(void* self, int v)
{
    if (v < 0) {
        *(int*)((char*)self + 0x60) = 0;
        return;
    }
    if (v >= 0x40) {
        *(int*)((char*)self + 0x60) = 0x3f;
        return;
    }
    *(int*)((char*)self + 0x60) = v;
}
#endif

//100%
INCLUDE_ASM("fe/fepopup", func_001CD0B0);
#ifdef SKIP_ASM
extern "C" void func_001CD0B0(void* self, int v)
{
    if (v < 0) {
        *(int*)((char*)self + 0x64) = 0;
        return;
    }
    if (v >= 0x40) {
        *(int*)((char*)self + 0x64) = 0x3f;
        return;
    }
    *(int*)((char*)self + 0x64) = v;
}
#endif

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_getObjName);

INCLUDE_ASM("fe/fepopup", func_001CD2F0);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_onGainTransition);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_redrawScreen);

INCLUDE_ASM("fe/fepopup", cKeyboardPopup_SetNameForKey);

INCLUDE_ASM("fe/fepopup", func_001CDB98);

INCLUDE_ASM("fe/fepopup", func_001CE2A8);

//100%
INCLUDE_ASM("fe/fepopup", func_001CE3C8);
#ifdef SKIP_ASM
extern "C" void func_001CE3C8(void* self, int i, int v, int mode)
{
    if (mode == 0 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x148) = v;
    }
    if (mode == 1 || mode == 2) {
        *(int*)((char*)self + (i << 2) + 0x298) = v;
    }
}
#endif

INCLUDE_ASM("fe/fepopup", func_001CE408);

INCLUDE_ASM("fe/fepopup", func_001CE468);

INCLUDE_ASM("fe/fepopup", func_001CE4C8);

INCLUDE_ASM("fe/fepopup", func_001CE520);

INCLUDE_ASM("fe/fepopup", func_001CE560);

INCLUDE_ASM("fe/fepopup", func_001CE5E0);

//100%
INCLUDE_ASM("fe/fepopup", func_001CE6F0);
#ifdef SKIP_ASM
struct sPopupSlot_E6F0 {
    signed char id;
    signed char pad;
};

extern sPopupSlot_E6F0 D_0045AAD8[][4][5];

extern "C" int func_001CE6F0(int a, int b)
{
    int n = 0;
    for (int i = 0; i < 5; i++) {
        if (D_0045AAD8[a][b][i].id >= 0) {
            n++;
        } else {
            break;
        }
    }
    return n;
}
#endif

