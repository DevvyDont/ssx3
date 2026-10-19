#include "common.h"

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onWidgetCreate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char D_00471BF0[];
extern char D_00471C08[];
extern char D_00471C20[];
extern void* D_004A28A8;

class cUIObj_20DE30 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int v);
    virtual void v07(int v);
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
};

extern "C" void cOVState_REPLAY_onWidgetCreate(void* self, cUIObj_20DE30* w)
{
    int id = *(int*)((char*)w + 0x38);
    if (id == GetHashValue32(D_00471BF0)) {
        *(int*)((char*)w + 0x18) = 0;
        if (*(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28) + 0x608) != 0) {
            w->v06(0);
            w->setEnabled(1);
            w->setVisible(0);
        }
    } else {
        int id2 = *(int*)((char*)w + 0x38);
        if (id2 == GetHashValue32(D_00471C08)) {
            *(int*)((char*)w + 0x18) = 1;
        } else {
            int id3 = *(int*)((char*)w + 0x38);
            if (id3 == GetHashValue32(D_00471C20)) {
                *(int*)((char*)w + 0x18) = 2;
            }
        }
    }
}
#endif

INCLUDE_ASM("fe/ovstatereplay", func_0020DF10);

//100%
INCLUDE_ASM("fe/ovstatereplay", func_0020DF38);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
class cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
int func_00320C48(void* self, int button);
extern "C" void func_0039F190(void* self, int a1);
extern "C" void func_0026F980(void* replay);
extern "C" void func_002706B8(void* replay);
extern "C" void func_0020AB50(int state);
extern "C" void cGame_exit(void* game, int a1);
extern "C" int func_0026FB88(void* replay, int a1);
extern int D_004A2A50;
extern int D_004A2A54;
extern int D_005366E8[];
extern int D_004428F0[];
extern int D_004428F8[];
extern char D_004A2720[];
extern char D_00471BC0[];
extern char D_00471BD0[];
extern char D_00471BE0[];
extern char D_0046E050[];

class cSnd_20DF38 {
public:
    virtual void v01();
    virtual void play(int id);
};
extern cSnd_20DF38 D_004A5A58;

class cPad_20DF38 {
public:
    int f0;
    unsigned char port;     // 0x4
    // vptr at 0x8
    virtual void v01();
    virtual void v02();
    virtual int v03();
    virtual void v04();
    virtual int v05();
    virtual int v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual int v17();
    virtual int v18();
};

struct sMgr_20DF38 {
    char pad0[0x84];
    char* game;             // 0x84
    char pad88[0x28];
    void* pads[8];          // 0xB0
};
// Typed view of the game manager (the unit declares D_004A28A8 as void*).
extern sMgr_20DF38* D_004A28A8_20DF38 __asm__("D_004A28A8");

struct sReplay_20DF38 {
    char pad_0x0[0x10];
    char* owner;            // 0x10
    char pad_0x14[0x2C];
    void* screen;           // 0x40
    char pad_0x44[0x58];
    int paused;             // 0x9C
    int hudHidden;          // 0xA0
    int pos;                // 0xA4
    int speed;              // 0xA8
    int start;              // 0xAC
    int end;                // 0xB0
    int fB4;                // 0xB4
};

static inline char* replay_20DF38() { return *(char**)(D_004A28A8_20DF38->game + 0x28); }

extern "C" int func_0020DF38(sReplay_20DF38* self, cPad_20DF38* pad)
{
    cSnd_20DF38* snd = &D_004A5A58;
    if (*(cSnd_20DF38**)(self->owner + 0x14) != 0)
        snd = *(cSnd_20DF38**)(self->owner + 0x14);
    if (pad->port != *(int*)(replay_20DF38() + 0x28)) {
        return 1;
    }
    if (self->paused == 0 && func_00320C48(D_004A28A8_20DF38->pads[pad->port], 0xCA)) {
        if (self->pos == self->end || self->speed == 4) {
            snd->play(2);
        }
        self->speed = -4;
        return 1;
    }
    if (self->paused == 0 && func_00320C48(D_004A28A8_20DF38->pads[pad->port], 0xCB)) {
        if (self->pos == self->start || self->speed == -4) {
            snd->play(2);
        }
        self->speed = 4;
        return 1;
    }
    if (pad->v03() && self->paused == 0) {
        snd->play(6);
        int n = D_004A2A54 + 1;
        D_005366E8[n] = 2;
        D_004A2A50 = D_004428F8[0];
        D_004A2A54 = n;
        self->paused = 1;
        char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471BE0));
        *(int*)(obj + 0x90) &= ~8;
        return 1;
    }
    if (self->paused != 0 && pad->v05()) {
        snd->play(6);
        char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471BE0));
        switch (*(int*)(*(char**)(obj + 0xA0) + 0x18)) {
        case 0:
            func_0026F980(replay_20DF38());
            func_0020AB50(0x12);
            D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
            break;
        case 1: {
            func_0039F190(self->owner + 0x18, 1);
            unsigned short frame = cUIScreen_getFrameByLabel((cUIScreen*)self->screen, GetHashValue32(D_0046E050));
            if (frame != 0xFFFF) {
                cUIScreen_playFrame(self->screen, frame, 1);
            }
            func_0026F980(replay_20DF38());
            if (self->fB4 == 0) {
                func_002706B8(replay_20DF38());
            }
            if (*(int*)(replay_20DF38() + 0x608) != 0) {
                cGame_exit(D_004A28A8_20DF38->game, 0);
            }
            D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
            break;
        }
        case 2:
            self->paused = 0;
            D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
            break;
        }
        return 1;
    }
    if (self->paused != 0 && pad->v06()) {
        snd->play(3);
        D_004A2A50 = D_004428F0[D_005366E8[--D_004A2A54]];
        self->paused = 0;
        char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471BE0));
        *(int*)(obj + 0x90) |= 8;
        return 1;
    }
    if (self->paused == 0 && func_00320C48(D_004A28A8_20DF38->pads[pad->port], 0xCC)) {
        snd->play(6);
        self->hudHidden ^= 1;
        if (self->hudHidden != 0) {
            cUIState_hideObjSafe(self, D_004A2720);
            cUIState_hideObjSafe(self, D_00471BC0);
            cUIState_showObjSafe(self, D_00471BD0);
        } else {
            cUIState_showObjSafe(self, D_004A2720);
            cUIState_showObjSafe(self, D_00471BC0);
            cUIState_hideObjSafe(self, D_00471BD0);
        }
        return 1;
    }
    if (pad->v17()) {
        char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471BE0));
        if (obj != 0 && self->fB4 != 0 && *(unsigned char*)(obj + 0x95) == 1) {
            return 1;
        }
    } else if (pad->v18()) {
        char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_00471BE0));
        if (obj != 0 && self->fB4 != 0 && *(unsigned char*)(obj + 0x95) == 2) {
            return 1;
        }
    }
    if (self->paused != 0) {
        return 0;
    }
    return func_0026FB88(replay_20DF38(), 0);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onUpdate);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void func_0020E900(void* self);
extern "C" void func_0026FA78(void* p);
extern "C" void cOVState_REPLAY_setupCameraName(void* self);
extern "C" void cOVState_REPLAY_setupTicker(void* self);
extern void* D_004A28A8;
extern char D_00471C30[];
extern char D_00471BB0[];
extern char D_00471BA0[];
extern char D_004A2728[];

struct sVec3_0020E530 {
    float x, y, z;
};

struct sReplay_0020E530 {
    char pad_0x0[0x40];
    void* screen;       // 0x40
    char pad_0x44[0x58];
    int paused;         // 0x9C
    char pad_0xA0[0x4];
    int pos;            // 0xA4
    int speed;          // 0xA8
    int start;          // 0xAC
    int end;            // 0xB0
};

extern "C" void cOVState_REPLAY_onUpdate(void* p)
{
    sReplay_0020E530* self = (sReplay_0020E530*)p;
    func_0020E900(self);
    self->pos += self->speed;
    if (self->pos <= self->start) {
        self->pos = self->start;
        self->speed = 0;
    } else if (self->pos >= self->end) {
        self->pos = self->end;
        self->speed = 0;
    }
    if (self->pos == self->start) {
        cUIState_showObjSafe(self, D_00471C30);
        cUIState_hideObjSafe(self, D_00471BB0);
    } else if (self->pos == self->end) {
        cUIState_hideObjSafe(self, D_00471C30);
        cUIState_showObjSafe(self, D_00471BB0);
    } else {
        cUIState_hideObjSafe(self, D_00471C30);
        cUIState_hideObjSafe(self, D_00471BB0);
    }
    char* obj = (char*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(D_004A2728));
    sVec3_0020E530 v = *(sVec3_0020E530*)(obj + 0x44);
    v.y = (float)self->pos;
    *(sVec3_0020E530*)(obj + 0x44) = v;
    if (self->paused != 0) {
        cUIState_showObjSafe(self, D_00471BA0);
    } else {
        cUIState_hideObjSafe(self, D_00471BA0);
    }
    if (self->paused == 0) {
        func_0026FA78(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
    }
    cOVState_REPLAY_setupCameraName(self);
    cOVState_REPLAY_setupTicker(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupCameraName);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern "C" void func_0020E900(void* self);
extern "C" void func_0026FA78(void* p);
extern "C" void cOVState_REPLAY_setupCameraName(void* self);
extern "C" void cOVState_REPLAY_setupTicker(void* self);
extern void* D_004A28A8;
extern char D_00471C30[];
extern char D_00471BB0[];
extern char D_00471BA0[];
extern char D_004A2728[];

extern char D_004A2730[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setAsciiString(cUIText* text, const char* str);
extern long D_00471C40[];
extern long D_00471C50[];
extern long D_00471C60[];
extern long D_00471C70[];
extern long D_00471C80[];
extern long D_004A2738[];
extern long D_00471C90[];
extern long D_004A2740[];
extern long D_004A2748[];
extern char D_004A2138[];

// Shows the name of the current replay camera.
extern "C" void cOVState_REPLAY_setupCameraName(void* self)
{
    long buf[2];
    cUIText* text = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_004A2730));
    if (text == 0)
        return;
    unsigned int cam = *(unsigned int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28) + 0x630);
    switch (cam) {
    case 0: __builtin_memcpy(buf, D_00471C40, 11); break;
    case 1: __builtin_memcpy(buf, D_00471C50, 14); break;
    case 2: __builtin_memcpy(buf, D_00471C60, 12); break;
    case 3: __builtin_memcpy(buf, D_00471C70, 11); break;
    case 4: __builtin_memcpy(buf, D_00471C80, 11); break;
    case 5: __builtin_memcpy(buf, D_004A2738, 8); break;
    case 6: __builtin_memcpy(buf, D_00471C90, 9); break;
    case 7: __builtin_memcpy(buf, D_004A2740, 8); break;
    case 8: __builtin_memcpy(buf, D_004A2748, 8); break;
    default:
        sprintf((char*)buf, D_004A2138, cam + 1);
        break;
    }
    cUIText_setAsciiString(text, (char*)buf);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupTicker);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char D_00471CC8[];
extern void* D_004A28A8;
struct sVec3_20E818 {
    float x, y, z;
};

extern "C" void cOVState_REPLAY_setupTicker(void* self)
{
    char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(D_00471CC8));
    if (obj != 0) {
        sVec3_20E818 pos = *(sVec3_20E818*)(obj + 0x44);
        char* replay = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28);
        int total = *(int*)(replay + 0x3CC);
        float frac;
        if (total > 0) {
            frac = (float)*(int*)(replay + 0x484) / (float)total;
        } else {
            frac = 0.0f;
        }
        pos.x = frac * 315.0f + 170.0f;
        *(sVec3_20E818*)(obj + 0x44) = pos;
    }
}
#endif

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/ovstatereplay", func_0020E8E0__FPv);
#ifdef SKIP_ASM
void* func_0020E8E0(void* self)
{
    return func_0039E6B8(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatereplay", func_0020E900);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" void func_00231840(void* game, int a);
extern "C" void func_0039E510(void* self);

extern "C" void func_0020E900(void* self)
{
    func_00231840(*(void**)((char*)D_004A28A8 + 0x84), 1);
    func_0039E510(self);
}
#endif

