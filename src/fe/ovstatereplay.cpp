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

INCLUDE_ASM("fe/ovstatereplay", func_0020DF38);

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_onUpdate);

INCLUDE_ASM("fe/ovstatereplay", cOVState_REPLAY_setupCameraName);

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

