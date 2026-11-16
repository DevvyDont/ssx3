#include "common.h"

//100%
INCLUDE_ASM("fe/ovstaterewardslist", cOVStateRewardsList_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_rewardslist[];

/**
 * Creates the overlay rewards-list screen "rewardslist", then runs cOVState_onCreateScreen().
 */
extern "C" void cOVStateRewardsList_onCreateScreen(void* self)
{
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_rewardslist), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstaterewardslist", cOVStateRewardsList_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* list, void* item);

struct sColor_200288 {
    float r, g, b, a;
    sColor_200288(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cUIObj_200288 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setEnabled(int v);
    virtual void setVisible(int v);
    virtual void v10();
    virtual int setColor(const sColor_200288& c);
};

struct sVEntry_200288 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

/**
 * Handles the rewards list's select and back events by pushing the state the custom interface returns.
 *
 * Event 5 colours the item and asks the custom interface's slot 4 (getNextState) for item id 1 only; the
 * result is pushed even when it is 0. Event 6 pushes the slot-5 result for the item's id when there is one.
 *
 * @param item  the widget (ignored when 0); its miscellaneous data (+0x18) is the id
 * @param msg  the widget event
 */
extern "C" void cOVStateRewardsList_onWidgetEvent(void* self, cUIObj_200288* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        item->setColor(sColor_200288(1.0f, 1.0f, 0.0f, 0.0f));
        void* nextState = 0;
        if (*(int*)((char*)item + 0x18) == 1) {
            void* obj = **(void***)((char*)self + 0x10);
            sVEntry_200288* vt = *(sVEntry_200288**)((char*)obj + 4);
            nextState = vt[4].fn((char*)obj + vt[4].delta, self, 1);
        }
        cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        break;
    }
    case 6: {
        void* obj = **(void***)((char*)self + 0x10);
        sVEntry_200288* vt = *(sVEntry_200288**)((char*)obj + 4);
        void* nextState = vt[5].fn((char*)obj + vt[5].delta, self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)*(void**)((char*)self + 0x10) + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstaterewardslist", func_00200388);
#ifdef SKIP_ASM
struct sRewardDesc_0388 {
    int w[0x6C / 4];
};
extern sRewardDesc_0388 D_004781D0[][4];
extern sRewardDesc_0388 D_004786E0[][5];
extern sRewardDesc_0388 D_00478D38[][8];

extern "C" sRewardDesc_0388 func_00200388(int a, int b, int kind)
{
    if (kind == 0) {
        return D_004781D0[a][b];
    } else if (kind == 1) {
        return D_004786E0[a][b];
    } else if (kind == 2) {
        return D_00478D38[a][b];
    }
    return D_004781D0[0][0];
}
#endif

//100%
INCLUDE_ASM("fe/ovstaterewardslist", func_002006B8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBENewRaceInterface_getLocation(void* iface);
extern unsigned int D_004A2594;
extern int cUITemplate_MAP_mPeak;

extern "C" void func_002006B8(void)
{
    if (D_004A2594 >= 2) {
        cUITemplate_MAP_mPeak = *(int*)((char*)cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) + 0x54);
    }
}
#endif

