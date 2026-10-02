#include "common.h"

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackSaveSuccess);

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackReadSuccess);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D68E8);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D69E0);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D6AD0);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D6C28);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D6D88);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7010);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7190);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D72A8);
#ifdef SKIP_ASM
struct sVEntry001D72A8 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

extern void* D_004A2028;
extern void* D_004A28A8;
extern "C" void* func_00227F80(void* app);
extern "C" void func_0023D570(void*, int);

extern "C" void func_001D72A8(void)
{
    char* fe = (char*)D_004A2028;
    sVEntry001D72A8* vt = *(sVEntry001D72A8**)(fe + 8);
    vt[41].fn(fe + vt[41].delta);
    void* app = D_004A28A8;
    *(int*)((char*)D_004A2028 + 0x1E4) = 0;
    char* obj = (char*)func_00227F80(app);
    sVEntry001D72A8* vt2 = *(sVEntry001D72A8**)(obj + 0x748);
    vt2[1].fn(obj + vt2[1].delta, 0x24);
    func_0023D570(obj, 0);
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7318);

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmFormatDone);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7548);

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmDelete);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D77E8);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7998);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7B18);

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackFileExists);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D7D70);

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDeleteDone);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8020);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8160);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8240);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D8358);
#ifdef SKIP_ASM
struct sVEntry001D8358 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern void* D_004A2028;

extern "C" void func_001D8358(void)
{
    char* fe = (char*)D_004A2028;
    if (*(int*)(fe + 0x214) != 2 && *(int*)(fe + 0x1C0) == 0) {
        *(int*)(fe + 0x214) = 2;
        sVEntry001D8358* vt = *(sVEntry001D8358**)(fe + 8);
        vt[35].fn(fe + vt[35].delta, 2);
    }
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D83A8);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8530);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D8670);
#ifdef SKIP_ASM
extern void* D_004A2028;

extern "C" void func_001D8670(void)
{
    if (*(int*)((char*)D_004A2028 + 0x1C0) == 4) {
        *(int*)((char*)D_004A2028 + 0x1C0) = 1;
    }
    char* fe = (char*)D_004A2028;
    *(int*)(fe + 0x1A8) = 0;
    *(int*)(fe + 0x1E0) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D86A0__FPv);
#ifdef SKIP_ASM
void func_001D86A0(void* self)
{
    *(int*)((char*)self + 0x1C0) = 6;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D86B0);
#ifdef SKIP_ASM
struct sVEntry001D86B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void func_001D9308(void*, int);

extern "C" void func_001D86B0(void* self)
{
    func_001D9308(self, 1);
    *(int*)((char*)self + 0x1A8) = 1;
    sVEntry001D86B0* vt = *(sVEntry001D86B0**)((char*)self + 8);
    *(int*)((char*)self + 0x1C0) = 5;
    vt[47].fn((char*)self + vt[47].delta);
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8700);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D8BE0);
#ifdef SKIP_ASM
extern "C" void func_001D9308(void*, int);

extern "C" void func_001D8BE0(void* self)
{
    *(int*)((char*)self + 0x1A8) = 0;
    *(int*)((char*)self + 0x1F0) = 0;
    *(int*)((char*)self + 0x1EC) = 0;
    *(int*)((char*)self + 0x1B0) = 0;
    *(int*)((char*)self + 0x1E4) = 1;
    func_001D9308(self, 0);
    *(int*)((char*)self + 0x1C0) = 2;
    *(int*)((char*)self + 0x1E0) = 0;
    *(int*)((char*)self + 0x1E8) = 0;
    *(int*)((char*)self + 0x1DC) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D8C38__FPv);
#ifdef SKIP_ASM
void func_001D8C38(void* self)
{
    *(int*)((char*)self + 0x1A8) = 0;
    *(int*)((char*)self + 0x1C0) = 1;
    *(int*)((char*)self + 0x1DC) = 0;
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onInputBegin);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D8D68);
#ifdef SKIP_ASM
struct sVEntry001D8D68 {
    short delta;
    short index;
    void (*fn)(void*, unsigned short*, int, int, int, int, int);
};

extern void* D_004A28A8;
extern "C" void* func_00227F80(void* app);
extern "C" void func_00241E18(void*, unsigned short*, int);

extern "C" void func_001D8D68(void* self)
{
    unsigned short buf[0x320];
    void* obj = func_00227F80(D_004A28A8);
    *(int*)((char*)self + 0x19C) = 0x16;
    func_00241E18(obj, buf, 0x320);
    sVEntry001D8D68* vt = *(sVEntry001D8D68**)((char*)self + 8);
    vt[36].fn((char*)self + vt[36].delta, buf, 1, 1, 0, 0, 1);
    *(int*)((char*)self + 0x210) = 1;
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D8DE0);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9258);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004A1600[];
extern "C" unsigned char func_0039B7B0(void* menu, unsigned char start);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

extern "C" void func_001D9258(void* self)
{
    void* menu = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A1600));
    if (menu != 0) {
        void* item = *(void**)((char*)menu + 0xA0);
        int idx = *(unsigned char*)((char*)menu + 0x95);
        if (item == 0 || ((*(int*)((char*)item + 0x14) >> 5) & 1)) {
            idx = func_0039B7B0(menu, 0);
        }
        if (idx != 0xFF) {
            cUIMenu_setSelectedByIndex(menu, idx);
            *(int*)((char*)self + 0x1C4) = idx;
        } else {
            cUIMenu_setSelectedByIndex(menu, 0);
            *(int*)((char*)self + 0x1C4) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9308);
#ifdef SKIP_ASM
struct sVEntry001D9308 {
    short delta;
    short index;
    void (*fn)(void*, void*, void*);
};

extern void* D_004A28A8;
extern "C" void* func_00227F80(void* app);

extern "C" void func_001D9308(void* self, int on)
{
    *(int*)((char*)self + 0x1F4) = on;
    if (on) {
        char* r = (char*)func_00227F80(D_004A28A8);
        char* obj = *(char**)(r + 0x434);
        sVEntry001D9308* vt = *(sVEntry001D9308**)obj;
        vt[14].fn(obj + vt[14].delta, *(void**)(r + 0xF8), (char*)self + 0x9C);
    } else {
        *(short*)((char*)self + 0x9C) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9368);
#ifdef SKIP_ASM
struct sVEntry001D9368 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001D9368(void* self)
{
    if (*(int*)((char*)self + 0x214) != 1) {
        *(int*)((char*)self + 0x214) = 1;
        sVEntry001D9368* vt = *(sVEntry001D9368**)((char*)self + 8);
        vt[35].fn((char*)self + vt[35].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93A8__FPv);
#ifdef SKIP_ASM
void func_001D93A8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93B0__FPv);
#ifdef SKIP_ASM
void func_001D93B0(void* self)
{
    *(int*)((char*)self + 0x1C0) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93C0__FPv);
#ifdef SKIP_ASM
void func_001D93C0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93C8__FPv);
#ifdef SKIP_ASM
void func_001D93C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93D0__FPv);
#ifdef SKIP_ASM
void func_001D93D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93D8);
#ifdef SKIP_ASM
extern "C" int func_001D93D8(void* self)
{
    return *(int*)((char*)self + 0x1c0) == 6;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D93E8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* self, int hash);
extern "C" void func_0039FCC8(void* obj, void* anim, int a, int b, int c);

extern "C" void func_001D93E8(void* self, char* objName, char* animName)
{
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(objName));
    if (obj != 0) {
        void* bank = (char*)*(void**)((char*)self + 0x10) + 0x50;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(animName));
        if (anim != 0) {
            func_0039FCC8(obj, anim, 3, 0, 1);
        }
    }
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_triggerDisplayState);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D97E0);
#ifdef SKIP_ASM
extern "C" int func_001D97E0(void* self, void* list, int idx, int back)
{
    int step = 1;
    if (back) {
        step = -1;
    }
    idx += step;
    if (idx >= 0 && idx <= *(int*)((char*)list + 0x424) - 1) {
        return 1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9818);
#ifdef SKIP_ASM
struct sVEntry001D9818 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void* func_0039E4C0(void* self);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_004678B0[];

extern "C" void func_001D9818(void* self)
{
    func_0039E4C0(self);
    void* obj = cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004678B0));
    if (obj != 0) {
        sVEntry001D9818* vt = *(sVEntry001D9818**)((char*)obj + 8);
        vt[9].fn((char*)obj + vt[9].delta, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9878__FPv);
#ifdef SKIP_ASM
void func_001D9878(void* self)
{
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D9880);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_createReadBuffer);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern char D_00467990[];

extern "C" void cFEMemCard_createReadBuffer(void* self, unsigned int size)
{
    if (*(int*)((char*)self + 0x228) == 1) {
        void* buf = *(void**)((char*)self + 0x1D4);
        if (buf != 0) {
            cMemMan_free(buf);
        }
        *(void**)((char*)self + 0x1D4) = 0;
        *(int*)((char*)self + 0x228) = 0;
    }
    void* p = operator_new_tag(size, D_00467990, 0x100, 0);
    *(void**)((char*)self + 0x1D0) = p;
    *(void**)((char*)self + 0x1D4) = p;
    *(int*)((char*)self + 0x228) = 1;
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D99C8);

INCLUDE_ASM("fe/fememcardcallbacks", func_001D9A80);

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9BD0);
#ifdef SKIP_ASM
extern "C" void* func_002C2508(void* dst, void* src);
void func_001C57A0(void* slots, int idx, int data);
// PORT: func_001C5DD8 takes only self; the unit's later (void*, int) declaration is a guess. Bound by asm label.
void func_001C5DD8_self(void* self) __asm__("func_001C5DD8");

extern "C" void func_001D9BD0(void* self, void* src, int mode)
{
    void* slots = (char*)self + 0xBC;
    if (mode == 0) {
        void* d = (char*)self + 0x9A0;
        func_002C2508(d, src);
        func_001C57A0(slots, 0, (int)d); // PORT: pointer passed as int
    } else if (mode == 1) {
        if (*(int*)((char*)self + 0xBF8) >= 2) {
            void* d = (char*)self + 0xA68;
            func_002C2508(d, src);
            func_001C57A0(slots, 1, (int)d);
        }
    } else {
        void* d = (char*)self + 0xB30;
        *(int*)((char*)self + 0xBF8) = 3;
        *(int*)((char*)self + 0xBC) = 3;
        func_002C2508(d, src);
        func_001C57A0(slots, 2, (int)d);
    }
    if (*(int*)((char*)self + 0x40) != 0) {
        func_001C5DD8_self(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9CA8);
#ifdef SKIP_ASM
extern "C" int func_002C24D0(void* p);
extern "C" void* func_002C2508(void* dst, void* src);
// PORT: func_001C5DD8 takes only self; the unit's later (void*, int) declaration is a guess. Bound by asm label.
void func_001C5DD8_self(void* self) __asm__("func_001C5DD8");

extern "C" void func_001D9CA8(void* self, void* src)
{
    if (src != 0 && func_002C24D0(src) != 0) {
        void* dst = (char*)self + 0x360;
        func_002C2508(dst, src);
        *(void**)((char*)self + 0xC4) = dst;
        *(int*)((char*)self + 0xC0) = 1;
        if (*(int*)((char*)self + 0x40) != 0) {
            func_001C5DD8_self(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", func_001D9D18);
#ifdef SKIP_ASM
extern "C" void func_001C5DD8(void*, int);

extern "C" void func_001D9D18(void* self, int a1)
{
    *(int*)((char*)self + 0x168) = a1;
    if (*(int*)((char*)self + 0x40) != 0) {
        func_001C5DD8(self, a1);
    }
}
#endif

INCLUDE_ASM("fe/fememcardcallbacks", func_001D9D48);

