#include "common.h"

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046EFF8[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBELibrary_getCharacterID(int);
extern "C" int func_00150928(void* iface, int a1, int charID);
extern "C" void cFEStateBuyAttrib_updateTotalCost(void* self);
extern "C" void cFEStateBuyAttrib_updateLevels(void* self);
extern "C" void cFEStateBuyAttrib_updateCostPerLevel(void* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);
extern "C" void cFEStateBuyAttrib_updateBank(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

extern "C" void cFEStateBuyAttrib_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046EFF8), 0);
    *(void**)((char*)self + 0x48) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    *(int*)((char*)self + 0xA4) = func_00150928(iface, 0, cBELibrary_getCharacterID(0));
    *(int*)((char*)self + 0xA8) = 0;
    *(int*)((char*)self + 0xA0) = 0;
    cFEStateBuyAttrib_updateTotalCost(self);
    cFEStateBuyAttrib_updateLevels(self);
    cFEStateBuyAttrib_updateCostPerLevel(self);
    cFEStateBuyAttrib_updateExperienceDisplay(self);
    cFEStateBuyAttrib_updateBank(self);
    func_0028F140(func_0028B180(), 3);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_onWidgetCreate);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char D_0046F008[];
extern char D_0046F020[];
extern char D_0046F040[];
extern char D_004A2320[];
extern char D_004A2328[];

static inline int IsHash_4910(int id, char* s) { return id == GetHashValue32(s); }

extern "C" void cFEStateBuyAttrib_onWidgetCreate(void* self, void* widget)
{
    if (IsHash_4910(*(int*)((char*)widget + 0x38), D_0046F008)) {
        cUIText_setUnicodeStringByID((cUIText*)widget, GetHashValue32(D_0046F020));
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_0046F040)) {
        *(void**)((char*)self + 0xAC) = widget;
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_004A2320)) {
        *(int*)((char*)widget + 0x18) = 0;
    } else if (IsHash_4910(*(int*)((char*)widget + 0x38), D_004A2328)) {
        *(int*)((char*)widget + 0x18) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F49D0);
#ifdef SKIP_ASM
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);

extern "C" int func_001F49D0(void* self, int on)
{
    if (on != 0) {
        cFEStateBuyAttrib_updateExperienceDisplay(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F49F8);
#ifdef SKIP_ASM
struct sVEntry001F49F8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00186518(void* self);

extern "C" void func_001F49F8(void* self)
{
    func_00186518(self);
    void* obj = *(void**)((char*)self + 0xAC);
    sVEntry001F49F8* vt = *(sVEntry001F49F8**)((char*)obj + 8);
    vt[7].fn((char*)obj + vt[7].delta, 1);
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F4A38);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F4A60);
#ifdef SKIP_ASM
extern "C" void func_001F5300(void* self);

extern "C" void func_001F4A60(void* self, void* sender, int msg)
{
    if (msg == 0x16) {
        if (*(int*)((char*)sender + 0x6C) != 0) {
            func_001F5300(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F4A90);
#ifdef SKIP_ASM
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_001CABA8(void* mem, void* owner, void* state, int a3);
extern "C" void func_001CAFC0(void* self, int a1, int a2, int a3);
extern "C" void func_0039F290(void* list, void* item);
extern "C" void func_0039F400(void* list, void* item);
extern char D_0046F050[];
extern char D_0046F060[];
extern char D_0046E050[];
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

struct sVE_4A90 {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVEr_4A90 {
    short delta;
    short index;
    void* (*fn)(void*, void*, void*);
};
struct sDefObj_4A90 {
    sVE_4A90* vt;
};
extern sDefObj_4A90 D_004A5A58;

extern "C" void func_001F4A90(char* self, char* obj, int msg)
{
    if (obj == 0)
        return;
    switch (msg) {
    case 5:
        if (*(int*)(self + 0xA4) >= *(int*)(self + 0xA0) && *(int*)(self + 0xA0) > 0) {
            void* o = func_001CABA8(cMemMan_alloc(0x70, D_0046F050, 0x100, 0), *(void**)(self + 0x10), self, *(signed char*)(self + 0x44));
            func_001CAFC0(o, 0, *(int*)(self + 0xA0), *(int*)(self + 0xA4));
            func_0039F290(*(char**)(self + 0x10) + 0x18, o);
        } else {
            sDefObj_4A90* p = *(sDefObj_4A90**)(*(char**)(self + 0x10) + 0x14);
            if (p == 0)
                p = &D_004A5A58;
            if (p != 0)
                p->vt[2].fn((char*)p + p->vt[2].delta, 4);
        }
        break;
    case 6:
        if (*(int*)(self + 0xA8) != 0) {
            *(int*)(self + 0xA8) = 0;
            cUIState_hideObjSafe(self, D_0046F060);
            char* o = *(char**)(self + 0xAC);
            sVE_4A90* vt = *(sVE_4A90**)(o + 8);
            vt[7].fn(o + vt[7].delta, 1);
        } else {
            char* mgr = **(char***)(self + 0x10);
            sVEr_4A90* vt = *(sVEr_4A90**)(mgr + 4);
            void* r = vt[5].fn(mgr + vt[5].delta, self, *(void**)(obj + 0x18));
            if (r != 0) {
                int frame = cUIScreen_getFrameByLabel(*(cUIScreen**)(self + 0x48), GetHashValue32(D_0046E050));
                if (frame != 0xFFFF)
                    cUIScreen_playFrame(*(void**)(self + 0x48), frame, 1);
                func_0039F400(*(char**)(self + 0x10) + 0x18, r);
            }
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F4C30);
#ifdef SKIP_ASM
// PORT: func_0039E508__FPv is called with (self, widget) here; bind the 2-arg form to that symbol.
void func_0039E508_2(void* self, void* w) __asm__("func_0039E508__FPv");
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
int func_00150E50(void* iface, int level);
extern "C" void cFEStateBuyAttrib_updateTotalCost(void* self);
extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* self);
extern char D_0046F040[];
extern char D_0046F070[];
extern char D_004A2330[];

struct sVEi_4C30 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sBuyAttrib_4C30 {
    char pad0[0x10];
    char* owner;            // 0x10
    char pad14[0x48 - 0x14];
    void* screen;           // 0x48
    int max4C[7];           // 0x4C
    int level[7];           // 0x68
    int bought[7];          // 0x84
    int cost;               // 0xA0
    int bank;               // 0xA4
    int busy;               // 0xA8
};

static inline void defaultMsg_4C30(sBuyAttrib_4C30* s, int msg)
{
    sDefObj_4A90* p = *(sDefObj_4A90**)(s->owner + 0x14);
    if (p == 0)
        p = &D_004A5A58;
    if (p != 0)
        p->vt[2].fn((char*)p + p->vt[2].delta, msg);
}

static inline void playFrame_4C30(sBuyAttrib_4C30* s, char* name)
{
    unsigned short frame = cUIScreen_getFrameByLabel((cUIScreen*)s->screen, GetHashValue32(name));
    if (frame != 0xFFFF)
        cUIScreen_playFrame(s->screen, frame, 1);
    defaultMsg_4C30(s, 1);
}

extern "C" void func_001F4C30(sBuyAttrib_4C30* self, char* w)
{
    int changed = 0;
    func_0039E508_2(self, w);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    unsigned char idx = *((unsigned char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_0046F040)) + 0x95);
    if (self->busy == 0) {
        sVEi_4C30* vt = *(sVEi_4C30**)(w + 8);
        if (vt[19].fn(w + vt[19].delta) != 0) {
            int* bought = self->bought;
            int* cnt = &bought[idx];
            changed = 1;
            if (*cnt > 0) {
                (*cnt)--;
                self->cost -= func_00150E50(iface, self->level[idx] - 1);
                cFEStateBuyAttrib_updateTotalCost(self);
                playFrame_4C30(self, D_004A2330);
            } else {
                defaultMsg_4C30(self, 4);
            }
        } else {
            sVEi_4C30* vt2 = *(sVEi_4C30**)(w + 8);
            if (vt2[20].fn(w + vt2[20].delta) != 0) {
                int* level = self->level;
                int* lvl = &level[idx];
                changed = 1;
                int total = func_00150E50(iface, *lvl - 1) + self->cost;
                if (*lvl < 11 && total <= self->bank) {
                    int* bought = self->bought;
                    int* cnt = &bought[idx];
                    if (self->max4C[idx] + *cnt < (*lvl + 1) * 5) {
                        (*cnt)++;
                        self->cost += func_00150E50(iface, *lvl - 1);
                        cFEStateBuyAttrib_updateTotalCost(self);
                        playFrame_4C30(self, D_0046F070);
                    }
                } else {
                    defaultMsg_4C30(self, 4);
                }
            }
        }
    }
    if (changed) {
        cFEStateBuyAttrib_updateExperienceDisplay(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateLevels);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004A2338[];
extern char D_004A2340[];

struct sBuyAttribK1F4EA8 {
    char pad_0x0[0x48];
    void* screen;   // 0x48
    int pts[7];     // 0x4C
    int lvl[7];     // 0x68
};

extern "C" void cFEStateBuyAttrib_updateLevels(void* p)
{
    sBuyAttribK1F4EA8* self = (sBuyAttribK1F4EA8*)p;
    char name[0x20];
    char text[0x10];
    int i;
    for (i = 0; i < 7; i++) {
        int lvl = self->pts[i] / 5;
        if (self->lvl[i] < lvl)
            self->lvl[i] = lvl;
        int rem = self->pts[i] - lvl * 5;
        sprintf(name, D_004A2338, i);
        cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (t != 0) {
            sprintf(text, D_004A2340, lvl, rem * 2);
            cUIText_setAsciiString(t, text);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateCostPerLevel);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int func_00150E50(void* iface, int level);
extern char D_004A2348[];
struct sVE4FC0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cFEStateBuyAttrib_updateCostPerLevel(void* self)
{
    char buf[32];
    int i;
    int* levels = (int*)((char*)self + 0x68);
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0xB);
    for (i = 0; i < 7; i++, levels++) {
        sprintf(buf, D_004A2348, i);
        cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(buf));
        if (text != 0) {
            cUIText_setAsciiString(text, func_00198AF0(func_00150E50(iface, *levels - 1)));
            if (*levels >= 0xB) {
                sVE4FC0* vt = *(sVE4FC0**)((char*)text + 8);
                vt[9].fn((char*)text + vt[9].delta, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateExperienceDisplay);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_0046F080[];
extern char D_004A2350[];

struct sBuyAttribK1F50B0 {
    char pad_0x0[0x48];
    void* screen;   // 0x48
    int pts[7];     // 0x4C
    int lvl[7];     // 0x68
    int add[7];     // 0x84
};
struct sVec3K1F50B0 { float x, y, z; };
struct sVEK1F50B0a {
    short delta;
    short index;
    void (*fn)(void*, sVec3K1F50B0*);
};
struct sVEK1F50B0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void cFEStateBuyAttrib_updateExperienceDisplay(void* p)
{
    sBuyAttribK1F50B0* self = (sBuyAttribK1F50B0*)p;
    char name[0x20];
    int i;
    for (i = 0; i < 7; i++) {
        sprintf(name, D_0046F080, i);
        char* o = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (o != 0) {
            sVec3K1F50B0 v;
            v.y = 12.0f;
            v.x = (float)self->pts[i] * 3.3636362552642822f;
            v.z = 0.0f;
            sVEK1F50B0a* vt = *(sVEK1F50B0a**)(o + 8);
            vt[21].fn(o + vt[21].delta, &v);
            sVEK1F50B0b* vt2 = *(sVEK1F50B0b**)(o + 8);
            vt2[9].fn(o + vt2[9].delta, 1);
        }
        sprintf(name, D_004A2350, i);
        o = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(name));
        if (o != 0) {
            sVec3K1F50B0 v;
            v.x = (float)(self->add[i] + self->pts[i]) * 3.3636362552642822f;
            v.y = 12.0f;
            v.z = 0.0f;
            sVEK1F50B0a* vt = *(sVEK1F50B0a**)(o + 8);
            vt[21].fn(o + vt[21].delta, &v);
            sVEK1F50B0b* vt2 = *(sVEK1F50B0b**)(o + 8);
            vt2[9].fn(o + vt2[9].delta, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateTotalCost);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern char D_0046F090[];

extern "C" void cFEStateBuyAttrib_updateTotalCost(void* self)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(D_0046F090));
    if (text != 0) {
        cUIText_setAsciiString(text, func_00198AF0(*(int*)((char*)self + 0xA0)));
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateBuyAttrib_updateBank);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);
extern "C" const char* func_00198AF0(int value);
extern char D_0046F0A0[];

extern "C" void cFEStateBuyAttrib_updateBank(void* self)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x48), GetHashValue32(D_0046F0A0));
    if (text != 0) {
        cUIText_setAsciiString(text, func_00198AF0(*(int*)((char*)self + 0xA4)));
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", func_001F5300);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F54B0);
#ifdef SKIP_ASM
extern void* D_00473838[];
extern "C" void* func_0039E2A0(void* self);
extern "C" unsigned char func_001A1CD0(void* self, int a1);

extern "C" void* func_001F54B0(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_00473838;
    *(int*)((char*)self + 0xC) = 0x2A;
    *(char*)((char*)self + 0x44) = 0;
    *(char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), 0);
    return self;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_0046F168[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void func_001F6F30(void* self);
extern "C" void func_001F5A38(void* self);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

extern "C" void cFEStateCareerStats_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(D_0046F168), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    *(int*)((char*)self + 0x4C) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x64) = 0;
    *(int*)((char*)self + 0x60) = 0;
    *(int*)((char*)self + 0x68) = 0;
    *(int*)((char*)self + 0x70) = 0;
    *(int*)((char*)self + 0x6C) = 0;
    *(int*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x48) = 0;
    func_001F6F30(self);
    func_001F5A38(self);
    func_0028F140(func_0028B180(), 8);
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55C0);
#ifdef SKIP_ASM
extern "C" void cFEStateCareerStats_setupHighlightsList(void* self);

extern "C" int func_001F55C0(void* self, int on)
{
    if (on != 0) {
        cFEStateCareerStats_setupHighlightsList(self);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F55E8);
#ifdef SKIP_ASM
extern "C" void func_00186518(void* self);
extern "C" void cFEStateCareerStats_setupMenuFocus(void* self, int focus);

extern "C" void func_001F55E8(void* self)
{
    func_00186518(self);
    cFEStateCareerStats_setupMenuFocus(self, *(int*)((char*)self + 0x48));
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5618);
#ifdef SKIP_ASM
extern "C" int func_001F5618(void* self, int a1, unsigned int a2)
{
    switch (a2) {
    case 6:
    case 8:
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5650);
#ifdef SKIP_ASM
struct sVEntry001F5650 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};
extern "C" void func_0039F400(void* list, void* item);

extern "C" void func_001F5650(void* self, void* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5:
        break;
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry001F5650* vt = *(sVEntry001F5650**)((char*)obj + 4);
        void* r = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (r != 0) {
            func_0039F400((char*)*(void**)((char*)self + 0x10) + 0x18, r);
        }
        break;
    }
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_onInputBegin);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F5A38);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern char D_0046F1A8[];
extern char D_0046F1B8[];
extern char D_0046F1C8[];
extern char D_0046F1E0[];
extern char D_0046F1F0[];
extern char D_0046F200[];

struct sVEK1F5A38 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_001F5A38(void* self)
{
    cUIState_hideObjSafe(self, D_0046F1A8);
    cUIState_hideObjSafe(self, D_0046F1B8);
    cUIState_hideObjSafe(self, D_0046F1C8);
    cUIState_hideObjSafe(self, D_0046F1E0);
    int show = 1;
    switch (*(int*)((char*)self + 0x48)) {
    case 0:
        cUIState_showObjSafe(self, D_0046F1A8);
        show = 0;
        break;
    case 1:
        cUIState_showObjSafe(self, D_0046F1B8);
        break;
    case 2:
        cUIState_showObjSafe(self, D_0046F1C8);
        break;
    case 3:
        cUIState_showObjSafe(self, D_0046F1E0);
        show = 0;
        break;
    }
    char* o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046F1F0));
    if (o != 0) {
        sVEK1F5A38* vt = *(sVEK1F5A38**)(o + 8);
        vt[9].fn(o + vt[9].delta, show);
    }
    o = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_0046F200));
    if (o != 0) {
        sVEK1F5A38* vt = *(sVEK1F5A38**)(o + 8);
        vt[9].fn(o + vt[9].delta, show);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupMenuFocus);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateCareerStats_setupHighlightsList(void* self);
extern "C" void cFEStateCareerStats_setupRidersBest(void* self);
extern "C" void func_001F60E0(void* self);
extern "C" void func_001F6490(void* self);
extern char D_0046F210[];
extern char D_0046F220[];
extern char D_0046F238[];
extern char D_0046F250[];

struct sVE_5BA8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void setFocus_5BA8(char* o, int on)
{
    sVE_5BA8* vt = *(sVE_5BA8**)(o + 8);
    vt[7].fn(o + vt[7].delta, on);
}

extern "C" void cFEStateCareerStats_setupMenuFocus(void* self_, int menu)
{
    char* self = (char*)self_;
    *(int*)(self + 0x48) = menu;
    char* a = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046F210));
    if (a != 0)
        setFocus_5BA8(a, 0);
    char* b = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046F220));
    if (b != 0)
        setFocus_5BA8(b, 0);
    char* c = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046F238));
    if (c != 0)
        setFocus_5BA8(c, 0);
    char* d = (char*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(D_0046F250));
    if (d != 0)
        setFocus_5BA8(d, 0);
    switch (*(int*)(self + 0x48)) {
    case 0:
        setFocus_5BA8(a, 1);
        cFEStateCareerStats_setupHighlightsList(self);
        break;
    case 1:
        setFocus_5BA8(b, 1);
        func_001F60E0(self);
        break;
    case 2:
        setFocus_5BA8(c, 1);
        func_001F6490(self);
        break;
    case 3:
        setFocus_5BA8(d, 1);
        cFEStateCareerStats_setupRidersBest(self);
        break;
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupHighlightsList);

INCLUDE_ASM("fe/festatebuyattrib", func_001F60E0);

INCLUDE_ASM("fe/festatebuyattrib", func_001F6490);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F6840);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* text, const char* str);

extern "C" void func_001F6840(void* self, char* name, const char* str)
{
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
    if (text != 0) {
        cUIText_setAsciiString(text, str);
    }
}
#endif

INCLUDE_ASM("fe/festatebuyattrib", cFEStateCareerStats_setupRidersBest);

//100%
INCLUDE_ASM("fe/festatebuyattrib", func_001F6F30);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int func_00146E98(void* player, int idx);
extern "C" int func_00157B08(void* reward, int id, int n);
extern "C" int func_00145870(void* game, int track);
int cBENewPlayerInterface_getRiderCharID(void* self, int idx);
int cBERewardInterface_getTrackMedal(void* self, int a, int charID, int mode, int track);
extern int D_004780D0[];
extern int D_00478110[];

extern "C" void func_001F6F30(void* vself)
{
    char* self = (char*)vself;
    void* reward = cBE_getInterface_Fv(cBE_getBE(), 0xD);
    void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
    void* game = cBE_getInterface_Fv(cBE_getBE(), 0);
    char charID = cBENewPlayerInterface_getRiderCharID(player, func_00146E98(player, 0));
    if (func_00157B08(reward, func_00146E98(player, 0), 0) == 0) {
        *(int*)(self + 0x58) += 3;
        *(int*)(self + 0x68) += 4;
    }
    if (func_00157B08(reward, func_00146E98(player, 0), 1) == 0) {
        *(int*)(self + 0x58) += 3;
        *(int*)(self + 0x68) += 4;
    }
    if (func_00157B08(reward, func_00146E98(player, 0), 2) == 0) {
        *(int*)(self + 0x58) += 2;
        *(int*)(self + 0x68) += 4;
    }
    for (int i = 0; i < 8; i++) {
        int mode = 0;
        if (i % 3 == 0) mode = 4;
        char medal = cBERewardInterface_getTrackMedal(reward, 0, charID, mode, D_004780D0[i]);
        if (medal == 0) {
            *(int*)(self + 0x60) |= 1 << i;
            *(int*)(self + 0x5C) += 1;
        }
    }
    for (int i = 0; i < 12; i++) {
        int mode;
        if ((i & 3) == 0) {
            mode = 5;
        } else {
            mode = func_00145870(game, D_00478110[i]);
        }
        char medal = cBERewardInterface_getTrackMedal(reward, 0, charID, mode, D_00478110[i]);
        if (medal == 0) {
            *(int*)(self + 0x70) |= 1 << i;
            *(int*)(self + 0x6C) += 1;
        }
    }
}
#endif

