#include "common.h"

//100%
INCLUDE_ASM("fe/festatelegal", cFEStateLegal_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045DD10[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

extern "C" void cFEStateLegal_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0045DD10), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x48) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", func_00187CB8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0045DC60[];
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_0039E510(void* self);
extern "C" void func_0039F190(void* list, int a1);

extern "C" void func_00187CB8(void* self)
{
    func_0039E510(self);
    if (++*(int*)((char*)self + 0x48) == 0xF0) {
        int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)((char*)self + 0x40), GetHashValue32(D_0045DC60));
        if (frame != 0xFFFF) {
            cUIScreen_playFrame(*(void**)((char*)self + 0x40), frame, 1);
        }
        func_0039F190((char*)*(void**)((char*)self + 0x10) + 0x18, 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", func_00187D38);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void func_002C2868(char* s);
extern "C" int func_00158618(void* iface, int a, int i, int item, int b);
extern "C" int func_00150B88(void* iface, int a, int i, int item, int b);
int func_00157350(void* iface);
int func_001572B0(void* iface);
int func_001572D0(void* iface);
int func_001572F0(void* iface);
int func_00157310(void* iface);
int func_00157330(void* iface);
extern "C" int func_00158558(void* iface, int a, int i, int j, int b);
extern "C" void func_00158848(void* iface, int a, int i, long mask);
extern "C" int func_00157FD0(void* iface, int a, int i, int j, int b);
extern "C" int func_001580F8(void* iface, int a, int i, int j, int b);
extern "C" int func_00158220(void* iface, int a, int i, int j, int b);
extern "C" int func_00158470(void* iface, int a, int i, int j, int b);
extern "C" int func_00158348(void* iface, int a, int i, int j, int b);
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
extern "C" char* func_0014D998(void* db, int type);
int func_0014D988(void* db, int type);
extern "C" void* func_0014AD28(void* self, int a1, int a2);
extern "C" int func_0014D908(void* db, char* item, int id);
extern "C" void func_00151C48(void* prof, int id, int on);
extern int D_004A11E0;

static inline int ItemType_7D38(char* item)
{
    return item[2];
}

extern "C" int func_00187D38(char* code)
{
    void* gear = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    void* unlocks = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    void* profiles = cBE_getInterface_Fv(cBE_getBE(), 9);
    char buf[0x80];
    strcpy(buf, code);
    func_002C2868(buf);
    if (GetHashValue32(buf) == 0x7E68D) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0xB, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x699649F) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0xD, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x80C4F45) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0xF, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xFA26044) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x11, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x539AB42) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0xA, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x61C8197) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x12, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x64694E5) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x13, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x3F22E2B) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x14, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xCD28689) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x15, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xB582D1C) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x16, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x1CB90E5) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x17, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x715E0E4) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x18, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xF3223E) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x19, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xEAFC69E) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x1B, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xDD36D88) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x1C, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xDE3475) {
        for (int i = 0; i < 10; i++)
            func_00158618(unlocks, 0, i, 0x1A, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x82DE683) {
        for (int i = 0; i < 10; i++)
            func_00150B88(gear, 0, i, 1, 1);
        return 1;
    } else if (GetHashValue32(buf) == 0x4B1FF67) {
        for (int i = 0; i < 10; i++) {
            func_00150B88(gear, 0, i, 1, 1);
            func_00150B88(gear, 0, i, 2, 1);
        }
        return 1;
    } else if (GetHashValue32(buf) == 0x61F9F02) {
        int n = func_00157350(unlocks);
        for (int i = 0; i < 10; i++) {
            long mask = 0;
            for (int j = 0; j < n; j++) {
                func_00158558(unlocks, 0, i, j, 0);
                mask |= (long)1 << j;
            }
            func_00158848(unlocks, 0, i, mask);
        }
        return 1;
    } else if (GetHashValue32(buf) == 0x5B65653) {
        int n = func_001572B0(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                func_00157FD0(unlocks, 0, i, j, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xA0CF7C4) {
        int n = func_001572D0(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                func_001580F8(unlocks, 0, i, j, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xB5D8914) {
        int n = func_001572F0(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                func_00158220(unlocks, 0, i, j, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x717257D) {
        int n = func_00157330(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                func_00158470(unlocks, 0, i, j, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0x74F6CF4) {
        int n = func_00157310(unlocks);
        for (int j = 0; j < n; j++)
            for (int i = 0; i < 10; i++)
                func_00158348(unlocks, 0, i, j, 0);
        return 1;
    } else if (GetHashValue32(buf) == 0xBCF3974) {
        for (int team = 0; team < 3; team++) {
            for (int type = 0; type < 10; type++) {
                void* db = func_0014BDB8_noarg();
                char* item = func_0014D998(db, type);
                int count = func_0014D988(db, type);
                void* prof = func_0014AD28(profiles, team, type);
                for (int k = 0; k < count; k++, item += 0x38) {
                    if (ItemType_7D38(item) == 1 || ItemType_7D38(item) == 2 || ItemType_7D38(item) == 3) {
                        if (func_0014D908(db, item, D_004A11E0))
                            func_00151C48(prof, *(short*)(item + 4), 1);
                    }
                }
            }
        }
        return 1;
    } else if (GetHashValue32(buf) == 0x70694D5) {
        for (int team = 0; team < 3; team++) {
            for (int type = 0; type < 10; type++) {
                char* item = func_0014D998(func_0014BDB8_noarg(), type);
                int count = func_0014D988(func_0014BDB8_noarg(), type);
                void* prof = func_0014AD28(profiles, team, type);
                for (int k = 0; k < count; k++, item += 0x38) {
                    if (ItemType_7D38(item) == 1)
                        func_00151C48(prof, *(short*)(item + 4), 1);
                }
            }
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatelegal", func_001887A0);
#ifdef SKIP_ASM
struct sFlowState_887A0 {
    signed char count;
    char* states;
};
// NOTE: placeholder for the object at $gp+0x1D90 (no symbol in the target)
extern sFlowState_887A0 D_004A4E80;
extern void* D_0046C7C0[];
extern "C" void* func_0039E2A0(void* self);
extern "C" void func_001A0708(void* self, int a1, int a2);

extern "C" void* func_001887A0(void* self, int a1, int reset)
{
    func_0039E2A0(self);
    *(int*)((char*)self + 0xC) = 0x18;
    *(void***)((char*)self + 0x8) = D_0046C7C0;
    if (reset != 0) {
        func_001A0708(&D_004A4E80, 0x18, 0);
    }
    return self;
}
#endif

