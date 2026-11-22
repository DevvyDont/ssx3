#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "bx/bxstring.h"
#include "main/gamemode.h"
#include "ui/uiscrollbar.h"
#include "ui/uitextscroll.h"
#include "fe/ovstate.h"
#include "render/2drender.h"
#include "ai/rider.h"
#include "fe/hudradar.h"
#include "fe/ovstatetrophy.h"
#include "be/beintnetwork.h"
#include "fe/ovstateprofile.h"
#include "render/ps2graphicsman.h"
#include "online/gamecomm.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovstatetrophy", cOVStateTrophy_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern char str_34trophy[];

/**
 * Builds the trophy screen ("34trophy") and plays its first frame.
 */
extern "C" void cOVStateTrophy_onCreateScreen(cOVStateTrophy* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_34trophy), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", cOVStateTrophy_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* list, void* item);

struct sColor_20E9A0 {
    float r, g, b, a;
    sColor_20E9A0(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};

class cUIObj_20E9A0 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
    virtual void v10();
    virtual int setColor(const sColor_20E9A0& c);
};


/**
 * Handles the trophy screen's buttons: Cross recolours the item and pushes the next state, Triangle goes
 * back.
 *
 * Cross sets the item's colour (slot 11) to (1, 1, 0, 0), opaque red in tARGB's A, R, G, B order, then pushes
 * getNextState(self, 1) for choice 1, or a null state otherwise. Triangle pushes getPrevState(self, miscData)
 * when it is not null.
 *
 * @param item  the item; ignored when null
 * @param msg  the widget event: 5 Cross, 6 Triangle
 */
extern "C" void cOVStateTrophy_onWidgetEvent(cOVStateTrophy* self, cUIObj_20E9A0* item, int msg)
{
    if (item == 0) {
        return;
    }
    switch (msg) {
    case 5: {
        item->setColor(sColor_20E9A0(1.0f, 1.0f, 0.0f, 0.0f));
        void* nextState = 0;
        if (*(int*)((char*)item + 0x18) == 1) {
            void* obj = *(void**)self->mpEngine;
            sVEntry<void*(void*, void*, int)>* vt = *(sVEntry<void*(void*, void*, int)>**)((char*)obj + 4);
            nextState = vt[4].fn(vthis(obj, vt[4]), self, 1);
        }
        cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        break;
    }
    case 6: {
        void* obj = *(void**)self->mpEngine;
        sVEntry<void*(void*, void*, int)>* vt = *(sVEntry<void*(void*, void*, int)>**)((char*)obj + 4);
        void* nextState = vt[5].fn(vthis(obj, vt[5]), self, *(int*)((char*)item + 0x18));
        if (nextState != 0) {
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        }
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", cHUDRadar_cHUDRadar);
#ifdef SKIP_ASM
extern char D_00474230[];
extern int D_004A276C;
extern "C" void func_0020EC18(cHUDRadar* self, tRect* a, tRect* r);

/**
 * Constructs a course-progress tracker, the 0x3C-byte object at +0x8 of each cOVStateHUD1P panel.
 *
 * Installs its vtable D_00474230 at +0x38, lays it out over the whole 640 x 480 screen with func_0020EC18 and counts
 * the instance in D_004A276C (cHUDRadar__cHUDRadar drops the count). cOVStateHUD1P_cOVStateHUD1P and func_0021CBA0 call it.
 *
 * @return @p self
 */
extern "C" void* cHUDRadar_cHUDRadar(cHUDRadar* self)
{
    tRect screenRect;
    self->mVtbl = (void**)D_00474230;
    screenRect.X = 0.0f;
    screenRect.Y = 0.0f;
    screenRect.W = 640.0f;
    screenRect.H = 480.0f;
    func_0020EC18(self, &screenRect, &screenRect);
    D_004A276C++;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", cHUDRadar__cHUDRadar);
#ifdef SKIP_ASM
extern char D_00474230[];
extern int D_004A276C;
extern int D_004A2768;
void operator_delete(int*);

/**
 * Destroys a course-progress tracker: restores its vtable D_00474230 and drops the instance count D_004A276C.
 *
 * When the last tracker goes, D_004A2768 is cleared so the next layout (func_0020EB50) looks the icon textures and the
 * font up again. It is the only slot of D_00474230 (+0xC); cOVStateHUD1P__cOVStateHUD1P calls it for the panels'
 * trackers.
 *
 * @param flags  bit 0 frees the object with operator_delete (the g++ deleting-destructor flag)
 */
extern "C" void cHUDRadar__cHUDRadar(cHUDRadar* self, int flags)
{
    int n = D_004A276C - 1;
    self->mVtbl = (void**)D_00474230;
    D_004A276C = n;
    if (n == 0) {
        D_004A2768 = 0;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EB50);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void func_0020EC18(cHUDRadar* self, tRect* a, tRect* r);
extern "C" int cUITextureBank_getElementByHashname(void* bank, int name);
extern "C" int cUIFontInterface_getFontByHashName(void* bank, int hash);
extern int D_004A2764;
extern int D_004A2768;
extern char str_fefont[];
struct sTex_20EB50 {
    int name;
    int f4;
    int f8;
    int handle;
};
extern sTex_20EB50 D_004C8C58[];

/**
 * Lays out a course-progress tracker in a screen rectangle, loading the shared icons and font first when needed.
 *
 * On the first call after the last tracker was destroyed (D_004A2768 clear) it looks up the 11 icon records of
 * D_004C8C58 (name hash, width, height, texture) in the UI texture bank (SSXApp->Game->UI + 0x58,
 * cUITextureBank_getElementByHashname) and the "fefont" font in the UI's font bank (cUIFontInterface_getFontByHashName on UI + 0x8) into
 * D_004A2764, and sets D_004A2768. Then func_0020EC18 does the layout. cOVStateHUD1P_onCreateScreen calls it for both
 * panels' trackers.
 *
 * @param screenRect  the screen bounds (the HUD passes 0, 0, 640, 480); not used beyond being passed on
 * @param area  the rectangle (x, y, w, h) the tracker fills
 */
extern "C" void func_0020EB50(cHUDRadar* self, tRect* screenRect, tRect* area)
{
    if (D_004A2768 == 0) {
        char* res = (char*)SSXApp->Game->UI;
        int i;
        for (i = 0; i < 11; i++) {
            D_004C8C58[i].handle = cUITextureBank_getElementByHashname(res + 0x58, D_004C8C58[i].name);
        }
        void* bank = *(void**)(res + 8);
        D_004A2764 = cUIFontInterface_getFontByHashName(bank, GetHashValue32(str_fefont));
        D_004A2768 = 1;
    }
    func_0020EC18(self, screenRect, area);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EC18);
#ifdef SKIP_ASM
extern int D_004A2764;
extern int D_004A2768;
// D_004C8C58 is the 11-entry texture table (size 0xB0); this float sits at +0x48 inside it.
extern float D_004C8C58_f[] __asm__("D_004C8C58");


/**
 * Computes a course-progress tracker's screen layout from the rectangle it is drawn in.
 *
 * h is the height of the percentage text (the shared font's MaxCharHeight * BaseScale.y * 0.6667 once loaded, else 30)
 * and m the arrow icon's height (record 4 of D_004C8C58). It stores the text position (+0x0, +0x4) at the bottom centre
 * of @p area, the top y (+0x8), the bottom of the bar area above the text and the bottom arrow row (+0xC, again at +0x18),
 * the centre x (+0x10), the bar top below the top arrow row (+0x14 = y + m), the pixels per course distance unit for
 * the single-player bar (+0x1C = (+0xC - +0x8 - m) / 10000, the window being 10000 units long), the rectangle itself
 * (+0x20..+0x2C) and its height / 10000 for the split-screen bar (+0x30).
 *
 * @param screenRect  the screen bounds; not used
 * @param area  the rectangle (x, y, w, h)
 */
// PORT: D_004A2764 holds a texture pointer in an int (declared int elsewhere in the unit).
extern "C" void func_0020EC18(cHUDRadar* self, tRect* screenRect, tRect* area)
{
    float textHeight;
    if (D_004A2768 != 0) {
        char* tex = (char*)D_004A2764;
        textHeight = (float)*(int*)(tex + 0x14) * *(float*)(tex + 0x34) * 0.666700005531311f;
    } else {
        textHeight = 30.0f;
    }
    float arrowHeight = D_004C8C58_f[72 / 4];
    (*(t2Vector*)&(self)->unk_000[0]) = t2Vector(area->X + area->W * 0.5f, (area->Y + area->H) - textHeight);
    (self)->unk_008 = area->Y;
    (self)->unk_00C = area->Y + area->H - textHeight - arrowHeight;
    (self)->unk_010 = area->X + area->W * 0.5f;
    (self)->unk_014 = area->Y + arrowHeight;
    (self)->unk_018 = (self)->unk_00C;
    (self)->unk_01C = ((self)->unk_00C - (self)->unk_008 - arrowHeight) * 9.999999747378752e-05f;
    (*(&(self)->unk_020)) = *area;
    (self)->unk_030 = (*(&(self)->unk_020)).H * 9.999999747378752e-05f;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020ED20);
#ifdef SKIP_ASM
struct sTrophySlot20ED20 {
    int active;
    int pad[5];
};
struct sPad16;
extern sPad16 D_004C8BC8;
extern char* cHUDRadar_mpRadarHeader;
extern int D_004A2760;
extern "C" void func_00210618(int i);

/**
 * Updates every rider's course progress for the trackers.
 *
 * When course data is loaded (cHUDRadar_mpRadarHeader) it runs func_00210618 for each rider of the race (SSXApp->Game->AI, count at
 * +0x78), clears that rider's reset flag in D_004C8BC8 and finally clears D_004A2760 so the trackers may draw.
 * cOVStateHUD1P_onUpdate calls it.
 */
extern "C" void func_0020ED20(void)
{
    if (cHUDRadar_mpRadarHeader != 0) {
        char* race = (char*)SSXApp->Game->AI;
        int i;
        for (i = 0; i < *(int*)(race + 0x78); i++) {
            func_00210618(i);
            ((sTrophySlot20ED20*)&D_004C8BC8)[i].active = 0;
        }
        D_004A2760 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020EDA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// The render device's 2D state block (as in fe/ovstatehud and render/lightman).
struct sGs_EDA0 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    int w8;
    unsigned int wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    // Replace the bits under mask (bits 5..9 of word 8 = the primitive/alpha mode).
    void SetBits(int mask, int bits) { int w = w8 & ~mask; w8 = w | bits; }
    void SetPrim(int t) { SetBits(0x3E0, t << 5); }
};

struct sPos_EDA0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVert_EDA0 {
    float u, v, q, f0C;
    int r, g, b, a;
    sPos_EDA0 pos;
    sVert_EDA0() {}
};

class cGfx_EDA0 {
public:
    char pad[0xE84];
    sGs_EDA0* stateTop;
    char padE88[0x10D8 - 0xE88];
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29();
    virtual void PushView();
    virtual void PopView();
    virtual void v32();
    virtual void SetView(void*);
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void RenderTriStrip(int n, sVert_EDA0* v, int flags);
};
extern sGs_EDA0 D_00501420_EDA0 __asm__("sDrawState_s_defaultDrawState");
extern char t4mat_Identity[];

// The rider's cBeing interface (rider + 0x6C0), slots in the prototype's order (as in seg/seg_1FBE38).
struct t4Vector;
struct t4Matrix;
struct tInstanceRecord;
class cBeing_EDA0 {
public:
    virtual ~cBeing_EDA0();
    virtual const t4Vector* getVelocity() const;
    virtual int isIntersecting(tInstanceRecord* rec) const;
    virtual void applyBoost(const t4Vector& impulse);
    virtual const t4Vector* getPosition() const;
    virtual const t4Matrix* getTransform() const;
    virtual int getAIIndex() const;
    virtual bool isHuman() const;
};

struct sTex_EDA0 {
    int handle;     // 0x00
    char pad4[8];
    int GetHandle() { return handle; }
    float v0;       // 0x0C
    float u0;       // 0x10
    float u1;       // 0x14
    float v1;       // 0x18
};
// Trophy-room icon table (func_0020EB50 loads the textures): name hash, icon size, texture.
struct sIcon_EDA0 {
    int name;
    float w;
    float h;
    sTex_EDA0* tex;
};
extern sIcon_EDA0 D_004C8C58_EDA0[] __asm__("D_004C8C58");

struct sSlot_EDA0 {
    int active;
    int seg;
    float dist;
    float pos;
    float rel;
    int relValid;
};
extern sSlot_EDA0 D_004C8BC8_EDA0[6] __asm__("D_004C8BC8");

struct sMarker_EDA0 {
    int type;
    float dist;
};

// The trophy box (func_0020EC18 lays it out).
struct sTrophyBox_EDA0 {
    float x, y;         // 0x00 text position
    float top;          // 0x08
    float bottom;       // 0x0C
    float cx;           // 0x10
    float y14;          // 0x14
    float y18;          // 0x18
    float s1C;          // 0x1C
    float rx, ry, rw, rh; // 0x20
    float scale;        // 0x30
    int last;           // 0x34
};

struct sColour_EDA0 {
    float r, g, b, a;
};
struct sV2_EDA0 {
    float x, y;
};
struct sFont_EDA0 {
    char pad0[0x28];
    sV2_EDA0 scale;     // 0x28
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sColour_EDA0 color; // 0x40
    sColour_EDA0 color2; // 0x50
    // Written through float pointers: typed member stores (f38 = f30 * sx) leave the global font
    // pointer cached across them, where the target reloads it (96.7%).
    void SetScale(float sx, float sy)
    {
        *(float*)((char*)this + 0x38) = *(float*)((char*)this + 0x30) * sx;
        *(float*)((char*)this + 0x3C) = *(float*)((char*)this + 0x34) * sy;
    }
};
extern sFont_EDA0* D_004A2764_EDA0 __asm__("D_004A2764");
extern int D_004A4EB4;
extern int D_004A4EB8;
extern int D_004A4EC8;
extern sColour_EDA0 D_004C6868;
extern sColour_EDA0 D_004C6878;
extern sV2_EDA0 D_004A4EC0;
extern char str_pct_dpct_pct[];
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFont_centertext(void* font, float x, float y, const char* str);

extern int D_004A275C;
extern char* D_004A2758;
extern int D_005308D0[];

// The race (SSXApp->0x84->0xC; as in seg/seg_1FBE38): rider objects at 0x28, rider count at 0x78.
struct sRace_EDA0 {
    char pad[0x28];
    char* riders[6];    // 0x28
    char* players[12];  // 0x40
    char pad70[0x8];
    int count;          // 0x78
};
static inline float limitStep_EDA0(float v, float prev, float lim)
{
    float dd = v - prev;
    if (dd > lim)
        v = prev + lim;
    else if (dd < -lim)
        v = prev - lim;
    return v;
}


/**
 * Draws the single-player course-progress tracker for @p rider: a vertical bar over the 10000-unit stretch of course
 * around the rider, with every racer, the course markers and the percentage done.
 *
 * It draws nothing until the progress table has been updated (D_004A2760 clear), while the tracker is off (D_004A275C,
 * cHUDRadar_ShowRadar), without a rider or without course data. The viewer is the rider's AI index (the cBeing at rider +
 * 0x6C0). First each racer's lateral offset from the viewer (its slot position minus the viewer's, clamped to +-2000)
 * is smoothed into the slot (+0x10), moving at most 46.3 per frame unless the slot is invalid, and all slots are
 * invalidated when the viewer changed since the last draw (+0x34). The drawing runs inside cPS2GraphicsMan_PushView and
 * a pushed draw state reset to sDrawState_s_defaultDrawState with an identity view (cPS2GraphicsMan_SetView1):
 * - "%d%%" of the course done (distance / course length, cHUDRadar_mpRadarHeader + 0x10), centred at the text position
 *   (cFont_centertext) in the shared font at 0.6667 scale, white with a black shadow offset by (2, 2);
 * - an arrow (icon 4) at the top for each racer ahead of the window and at the bottom for each one behind it, shifted
 *   sideways by -offset * 0.005;
 * - the bar (icon 0, 11 px wide), cut where the window passes the start or the finish, and a red untextured strip over
 *   the part done;
 * - each racer inside the window (plus a 20 px margin) at its distance, shifted sideways by its offset and clipped to
 *   the bar with matching uvs: the viewer icon 1, humans icon 3, AI racers icon 2 (layers 12, 13, 14);
 * - the course markers (D_004A2758: type and distance pairs) inside the window as icon 5 + type, except type 1 when bit
 *   9 of D_005308D0 is set and GameModeMan + 0x88 is nonzero.
 *
 * Each quad goes through cPS2GraphicsMan_RenderTriStrip (GrMan's virtual slot 70) with its layer in bits 5-9 of the
 * draw state's word 2. Finally the state and view are popped and the viewer is kept in +0x34. cOVStateHUD1P_onRender2D
 * calls it with the panel's tracker when fewer than two players play.
 *
 * @param rider  the rider the panel follows (the cBeing at +0x6C0 gives its AI index)
 */
extern "C" void func_0020EDA0(cHUDRadar* self, cRider* rider)
{
    if (D_004A2760 != 0)
        return;
    if (D_004A275C == 0)
        return;
    if (((char*)rider) == 0)
        return;
    sMarker_EDA0* markers = (sMarker_EDA0*)D_004A2758;
    char* course = cHUDRadar_mpRadarHeader;
    if (course == 0)
        return;
    int me = ((cBeing_EDA0*)(&rider->mVtbl))->getAIIndex();
    sRace_EDA0* race = (sRace_EDA0*)SSXApp->Game->AI;
    float myDist = D_004C8BC8_EDA0[me].dist;
    float myPos = D_004C8BC8_EDA0[me].pos;
    int n = race->count;
    for (int i = 0; i < 6; i++) {
        if (self->unk_034 != me)
            D_004C8BC8_EDA0[i].relValid = 0;
        if (i < n) {
            float d = D_004C8BC8_EDA0[i].pos - myPos;
            if (d < -2000.0f)
                d = -2000.0f;
            else if (d > 2000.0f)
                d = 2000.0f;
            if (D_004C8BC8_EDA0[i].relValid != 0)
                d = limitStep_EDA0(d, D_004C8BC8_EDA0[i].rel, 46.29629898071289f);
            D_004C8BC8_EDA0[i].rel = d;
            D_004C8BC8_EDA0[i].relValid = 1;
        }
    }
    sVert_EDA0 verts[4];
    for (int i = 0; i < 4; i++) {
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
        verts[i].a = 0x80;
        verts[i].q = 1.0f;
    }
    ((cGfx_EDA0*)GrMan)->PushView();
    ((cGfx_EDA0*)GrMan)->PushState();
    *((cGfx_EDA0*)GrMan)->stateTop = D_00501420_EDA0;
    ((cGfx_EDA0*)GrMan)->stateTop->f4_23 = 2;
    ((cGfx_EDA0*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_EDA0*)GrMan)->stateTop->f4_12 = 20;
    ((cGfx_EDA0*)GrMan)->stateTop->f4_2 = 5;
    ((cGfx_EDA0*)GrMan)->SetView(t4mat_Identity);

    float lo = myDist - 5000.0f;
    float hi = lo + 10000.0f;
    float frac = D_004C8BC8_EDA0[me].dist / *(float*)(course + 0x10);
    if (D_004A4EB4 == 0) {
        D_004C6868.r = 1.0f;
        D_004C6868.g = 1.0f;
        D_004C6868.b = 1.0f;
        D_004C6868.a = 1.0f;
        D_004A4EB4 = 1;
    }
    if (D_004A4EB8 == 0) {
        D_004C6878.r = 1.0f;
        D_004C6878.g = 0.0f;
        D_004C6878.b = 0.0f;
        D_004C6878.a = 0.0f;
        D_004A4EB8 = 1;
    }
    if (D_004A4EC8 == 0) {
        D_004A4EC0.x = 2.0f;
        D_004A4EC0.y = 2.0f;
        D_004A4EC8 = 1;
    }
    char buf[16];
    sprintf(buf, str_pct_dpct_pct, (int)(frac * 100.0f + 0.5f));
    D_004A2764_EDA0->color = D_004C6868;
    D_004A2764_EDA0->SetScale(0.666700005531311f, 0.666700005531311f);
    D_004A2764_EDA0->color2 = D_004C6878;
    D_004A2764_EDA0->scale = D_004A4EC0;
    ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(10);
    cFont_centertext(D_004A2764_EDA0, self->unk_000[0], self->unk_000[1], buf);
    D_004A2764_EDA0->SetScale(1.0f, 1.0f);

    sIcon_EDA0* arrow = &D_004C8C58_EDA0[4];
    sTex_EDA0* atex = arrow->tex;
    for (int i = 0; i < n; i++) {
        float d = D_004C8BC8_EDA0[i].dist;
        if (lo <= d && d <= hi)
            continue;
        float va = atex->v1;
        float vb = atex->v0;
        float y = self->unk_00C;
        // retail scale literal is 0x3BA3D70B, one ulp above 0.005f
        float ax = self->unk_010 + -D_004C8BC8_EDA0[i].rel * 0.0050000004f;
        float hw = arrow->w * 0.5f;
        if (d > hi) {
            y = self->unk_008;
            va = atex->v0;
            vb = atex->v1;
        }
        verts[0].u = atex->u0;
        verts[0].v = va;
        verts[1].u = atex->u1;
        verts[1].v = va;
        verts[2].u = atex->u0;
        verts[2].v = vb;
        verts[3].u = atex->u1;
        verts[3].v = vb;
        sPos_EDA0 corner;
        corner.x = ax - hw;
        corner.y = y;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[0].pos = corner;
        corner.x = ax + hw;
        corner.y = y;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[1].pos = corner;
        corner.x = ax - hw;
        corner.y = y + arrow->h;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[2].pos = corner;
        corner.x = ax + hw;
        corner.y = y + arrow->h;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[3].pos = corner;
        ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(10);
        *(short*)((char*)((cGfx_EDA0*)GrMan)->stateTop + 0x10) = atex->GetHandle();
        ((cGfx_EDA0*)GrMan)->RenderTriStrip(4, verts, 0);
    }

    sTex_EDA0* tex = D_004C8C58_EDA0[0].tex;
    float barTop = self->unk_018;
    float barBottom = self->unk_014;
    if (*(float*)(course + 0x10) < hi)
        barBottom += (hi - *(float*)(course + 0x10)) * self->unk_01C;
    if (lo < 0.0f)
        barTop += lo * self->unk_01C;
    verts[0].u = tex->u0;
    verts[0].v = tex->v0;
    verts[1].u = tex->u1;
    verts[1].v = tex->v0;
    verts[2].u = tex->u0;
    verts[2].v = tex->v1;
    verts[3].u = tex->u1;
    verts[3].v = tex->v1;
    sPos_EDA0 corner;
    corner.x = self->unk_010 - 5.499532222747803f;
    corner.y = barTop;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[0].pos = corner;
    corner.x = self->unk_010 + 5.499532222747803f;
    corner.y = barTop;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[1].pos = corner;
    corner.x = self->unk_010 - 5.499532222747803f;
    corner.y = barBottom;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[2].pos = corner;
    corner.x = self->unk_010 + 5.499532222747803f;
    corner.y = barBottom;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[3].pos = corner;
    ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(9);
    *(short*)((char*)((cGfx_EDA0*)GrMan)->stateTop + 0x10) = tex->GetHandle();
    ((cGfx_EDA0*)GrMan)->RenderTriStrip(4, verts, 0);

    float markY = barTop + (barBottom - barTop) * frac;
    corner.x = self->unk_010 - 2.7497661113739014f;
    corner.y = barTop;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[0].pos = corner;
    corner.x = self->unk_010 + 2.7497661113739014f;
    corner.y = barTop;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[1].pos = corner;
    corner.x = self->unk_010 - 2.7497661113739014f;
    corner.y = markY;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[2].pos = corner;
    corner.x = self->unk_010 + 2.7497661113739014f;
    corner.y = markY;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[3].pos = corner;
    for (int i = 0; i < 4; i++) {
        verts[i].r = 0x80;
        verts[i].g = 0;
        verts[i].b = 0;
    }
    ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(10);
    *(short*)((char*)((cGfx_EDA0*)GrMan)->stateTop + 0x10) = -1;
    ((cGfx_EDA0*)GrMan)->RenderTriStrip(4, verts, 0);
    for (int i = 0; i < 4; i++) {
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
    }

    float margin = 20.0f / self->unk_01C;
    float lo2 = lo - margin;
    float hi2 = hi + margin;
    for (int i = 0; i < n; i++) {
        float d = D_004C8BC8_EDA0[i].dist;
        if (d < lo2)
            continue;
        if (d > hi2)
            continue;
        float x = self->unk_010;
        float y = self->unk_018 - (d - lo) * self->unk_01C;
        int prim = 0xC;
        int icon = 1;
        if (me != i) {
            x -= D_004C8BC8_EDA0[i].rel * 0.0050000004f;
            char* otherRider = race->riders[i];
            if (((cBeing_EDA0*)(otherRider + 0x6C0))->isHuman()) {
                prim = 13;
                icon = 3;
            } else {
                prim = 0xE;
                icon = 2;
            }
        }
        sIcon_EDA0* iconEntry = &D_004C8C58_EDA0[icon];
        sTex_EDA0* iconTexture = iconEntry->tex;
        float hh = iconEntry->h * 0.5f;
        float y0 = y - hh;
        float y1 = y + hh;
        if (self->unk_018 <= y0)
            continue;
        if (y1 <= self->unk_014)
            continue;
        float va = iconTexture->v0;
        float vb = iconTexture->v1;
        if (y0 < self->unk_014) {
            va = vb + (va - vb) * (y1 - self->unk_014) / (hh * 2.0f);
            y0 = self->unk_014;
        } else if (self->unk_018 < y1) {
            vb = va + (vb - va) * (self->unk_018 - y0) / (hh * 2.0f);
            y1 = self->unk_018;
        }
        verts[0].u = iconTexture->u0;
        verts[0].v = va;
        verts[1].u = iconTexture->u1;
        verts[1].v = va;
        verts[2].u = iconTexture->u0;
        verts[2].v = vb;
        verts[3].u = iconTexture->u1;
        verts[3].v = vb;
        float hw = iconEntry->w * 0.5f;
        corner.x = x - hw;
        corner.y = y0;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[0].pos = corner;
        corner.x = x + hw;
        corner.y = y0;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[1].pos = corner;
        corner.x = x - hw;
        corner.y = y1;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[2].pos = corner;
        corner.x = x + hw;
        corner.y = y1;
        corner.z = 0.0f;
        corner.w = 1.0f;
        verts[3].pos = corner;
        ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(prim);
        *(short*)((char*)((cGfx_EDA0*)GrMan)->stateTop + 0x10) = iconTexture->GetHandle();
        ((cGfx_EDA0*)GrMan)->RenderTriStrip(4, verts, 0);
    }

    if (*(int*)(course + 8) != 0) {
        int show = 1;
        if ((D_005308D0[0] >> 9) & 1) {
            if (SSXApp->GameModeMan->bIsTimedEvent != 0)
                show = 0;
        }
        for (int i = 0; i < *(int*)(course + 8); i++) {
            float d = markers[i].dist;
            if (d < lo2)
                continue;
            if (d > hi2)
                continue;
            if (markers[i].type == 1 && !show)
                continue;
            sIcon_EDA0* iconEntry = &D_004C8C58_EDA0[markers[i].type + 5];
            sTex_EDA0* iconTexture = iconEntry->tex;
            float y = self->unk_018 - (d - lo) * self->unk_01C;
            float hw = iconEntry->w * 0.5f;
            float hh = iconEntry->h * 0.5f;
            float y0 = y - hh;
            float y1 = y + hh;
            if (self->unk_018 <= y0)
                continue;
            if (y1 <= self->unk_014)
                continue;
            float va = iconTexture->v0;
            float vb = iconTexture->v1;
            if (y0 < self->unk_014) {
                va = vb + (va - vb) * (y1 - self->unk_014) / iconEntry->h;
                y0 = self->unk_014;
            } else if (self->unk_018 < y1) {
                vb = va + (vb - va) * (self->unk_018 - y0) / iconEntry->h;
                y1 = self->unk_018;
            }
            verts[0].u = iconTexture->u0;
            verts[0].v = va;
            verts[1].u = iconTexture->u1;
            verts[1].v = va;
            verts[2].u = iconTexture->u0;
            verts[2].v = vb;
            verts[3].u = iconTexture->u1;
            verts[3].v = vb;
            corner.x = self->unk_010 - hw;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[0].pos = corner;
            corner.x = self->unk_010 + hw;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[1].pos = corner;
            corner.x = self->unk_010 - hw;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[2].pos = corner;
            corner.x = self->unk_010 + hw;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[3].pos = corner;
            ((cGfx_EDA0*)GrMan)->stateTop->SetPrim(11);
            *(short*)((char*)((cGfx_EDA0*)GrMan)->stateTop + 0x10) = iconTexture->GetHandle();
            ((cGfx_EDA0*)GrMan)->RenderTriStrip(4, verts, 0);
        }
    }
    ((cGfx_EDA0*)GrMan)->stateTop--;
    ((cGfx_EDA0*)GrMan)->PopView();
    self->unk_034 = me;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_0020FB40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// The render device's 2D state block (as in fe/ovstatehud and render/lightman).
struct sGs_FB40 {
    unsigned int w0;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    int w8;
    unsigned int wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    // Replace the bits under mask (bits 5..9 of word 8 = the primitive/alpha mode).
    void SetBits(int mask, int bits) { int w = w8 & ~mask; w8 = w | bits; }
    void SetPrim(int t) { SetBits(0x3E0, t << 5); }
};

struct sPos_FB40 {
    float x, y, z, w;
    void Set(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
} __attribute__((aligned(16)));

struct sVert_FB40 {
    float u, v, q, f0C;
    int r, g, b, a;
    sPos_FB40 pos;
    sVert_FB40() {}
};

class cGfx_FB40 {
public:
    char pad[0xE84];
    sGs_FB40* stateTop;
    char padE88[0x10D8 - 0xE88];
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8(); virtual void v9(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29();
    virtual void PushView();
    virtual void PopView();
    virtual void v32();
    virtual void SetView(void*);
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void RenderTriStrip(int n, sVert_FB40* v, int flags);
};
extern sGs_FB40 D_00501420_FB40 __asm__("sDrawState_s_defaultDrawState");
extern char t4mat_Identity[];

// The rider's cBeing interface (rider + 0x6C0), slots in the prototype's order (as in seg/seg_1FBE38).
struct t4Vector;
struct t4Matrix;
struct tInstanceRecord;
class cBeing_FB40 {
public:
    virtual ~cBeing_FB40();
    virtual const t4Vector* getVelocity() const;
    virtual int isIntersecting(tInstanceRecord* rec) const;
    virtual void applyBoost(const t4Vector& impulse);
    virtual const t4Vector* getPosition() const;
    virtual const t4Matrix* getTransform() const;
    virtual int getAIIndex() const;
    virtual bool isHuman() const;
};

struct sTex_FB40 {
    int handle;     // 0x00
    char pad4[8];
    int GetHandle() { return handle; }
    float v0;       // 0x0C
    float u0;       // 0x10
    float u1;       // 0x14
    float v1;       // 0x18
};
// Trophy-room icon table (func_0020EB50 loads the textures): name hash, icon size, texture.
struct sIcon_FB40 {
    int name;
    float w;
    float h;
    sTex_FB40* tex;
};
extern sIcon_FB40 D_004C8C58_FB40[] __asm__("D_004C8C58");

struct sSlot_FB40 {
    int active;
    int seg;
    float dist;
    float pos;
    float rel;
    int relValid;
};
extern sSlot_FB40 D_004C8BC8_FB40[6] __asm__("D_004C8BC8");

struct sMarker_FB40 {
    int type;
    float dist;
};

struct sTrophyBox_FB40 {
    char pad[0x20];
    float x, y, w, h;   // 0x20
    float scale;        // 0x30
    int last;           // 0x34
};

extern int D_004A275C;
extern char* D_004A2758;
extern int D_005308D0[];

// The race (SSXApp->0x84->0xC; as in seg/seg_1FBE38): rider objects at 0x28, rider count at 0x78.
struct sRace_FB40 {
    char pad[0x28];
    char* riders[6];    // 0x28
    char* players[12];  // 0x40
    char pad70[0x8];
    int count;          // 0x78
};

static inline float limitStep_FB40(float v, float prev, float lim)
{
    float dd = v - prev;
    if (dd > lim)
        v = prev + lim;
    else if (dd < -lim)
        v = prev - lim;
    return v;
}

/**
 * Draws the split-screen course-progress tracker for @p rider: a thin vertical line along one side of the panel with
 * racer and marker icons beside it.
 *
 * It has func_0020EDA0's early-outs and lateral-offset smoothing (the offsets are updated but not used here). The line
 * (icon 0, 5.5 px wide) runs down the left edge of the tracker rectangle (+0x20..+0x2C) when @p right is set and down
 * its right edge otherwise, covering the 10000-unit window centred on the viewer at +0x30 pixels per unit. Each racer
 * inside the window is drawn beside the line at its distance (the viewer icon 8, humans icon 10, AI racers icon 9;
 * layers 12, 13, 14), and so are the course markers (half of icon 5 + type, with func_0020EDA0's type-1 rule). Icons
 * point away from the line (uvs mirrored on the left side), are skipped when they reach either end and fade out over
 * the 60 px nearest the ends. The viewer is kept in +0x34. cOVStateHUD1P_onRender2D calls it for each panel in
 * two-player games, with @p right = (panel index != 0).
 *
 * @param rider  the rider the panel follows (the cBeing at +0x6C0 gives its AI index)
 * @param right  nonzero: line at the rectangle's left edge with the icons to its right; 0: line at the right edge,
 *               icons to the left
 */
extern "C" void func_0020FB40(cHUDRadar* self, cRider* rider, int right)
{
    if (D_004A2760 != 0)
        return;
    if (D_004A275C == 0)
        return;
    if (((char*)rider) == 0)
        return;
    sMarker_FB40* markers = (sMarker_FB40*)D_004A2758;
    char* course = cHUDRadar_mpRadarHeader;
    if (course == 0)
        return;
    int me = ((cBeing_FB40*)(&rider->mVtbl))->getAIIndex();
    sRace_FB40* race = (sRace_FB40*)SSXApp->Game->AI;
    float myDist = D_004C8BC8_FB40[me].dist;
    float myPos = D_004C8BC8_FB40[me].pos;
    int n = race->count;
    for (int i = 0; i < 6; i++) {
        if (self->unk_034 != me)
            D_004C8BC8_FB40[i].relValid = 0;
        if (i < n) {
            float d = D_004C8BC8_FB40[i].pos - myPos;
            if (d < -2000.0f)
                d = -2000.0f;
            else if (d > 2000.0f)
                d = 2000.0f;
            if (D_004C8BC8_FB40[i].relValid != 0)
                d = limitStep_FB40(d, D_004C8BC8_FB40[i].rel, 46.29629898071289f);
            D_004C8BC8_FB40[i].rel = d;
            D_004C8BC8_FB40[i].relValid = 1;
        }
    }
    sVert_FB40 verts[4];
    for (int i = 0; i < 4; i++) {
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
        verts[i].a = 0x80;
        verts[i].q = 1.0f;
    }
    float x = self->unk_020.X;
    if (right == 0)
        x += self->unk_020.W;
    ((cGfx_FB40*)GrMan)->PushView();
    ((cGfx_FB40*)GrMan)->PushState();
    *((cGfx_FB40*)GrMan)->stateTop = D_00501420_FB40;
    ((cGfx_FB40*)GrMan)->stateTop->f4_23 = 2;
    ((cGfx_FB40*)GrMan)->stateTop->f4_20 = 0;
    ((cGfx_FB40*)GrMan)->stateTop->f4_12 = 20;
    ((cGfx_FB40*)GrMan)->stateTop->f4_2 = 5;
    ((cGfx_FB40*)GrMan)->SetView(t4mat_Identity);

    sTex_FB40* tex = D_004C8C58_FB40[0].tex;
    float top = self->unk_020.Y;
    float bottom = top + self->unk_020.H;
    float lo = myDist - 5000.0f;
    float hi = lo + 10000.0f;
    sPos_FB40 corner;
    verts[0].u = tex->u0;
    verts[0].v = tex->v0;
    verts[1].u = tex->u1;
    verts[1].v = tex->v0;
    verts[2].u = tex->u0;
    verts[2].v = tex->v1;
    verts[3].u = tex->u1;
    verts[3].v = tex->v1;
    corner.x = x - 2.7497661113739014f;
    corner.y = bottom;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[0].pos = corner;
    corner.x = x + 2.7497661113739014f;
    corner.y = bottom;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[1].pos = corner;
    corner.x = x - 2.7497661113739014f;
    corner.y = top;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[2].pos = corner;
    corner.x = x + 2.7497661113739014f;
    corner.y = top;
    corner.z = 0.0f;
    corner.w = 1.0f;
    verts[3].pos = corner;
    ((cGfx_FB40*)GrMan)->stateTop->SetPrim(9);
    *(short*)((char*)((cGfx_FB40*)GrMan)->stateTop + 0x10) = tex->GetHandle();
    ((cGfx_FB40*)GrMan)->RenderTriStrip(4, verts, 0);

    float topFade = top + 60.0f;
    float bottomFade = bottom - 60.0f;
    for (int i = 0; i < n; i++) {
        float d = D_004C8BC8_FB40[i].dist;
        if (d < lo)
            continue;
        if (d > hi)
            continue;
        int prim = 0xC;
        int icon = 8;
        float y = bottom - (d - lo) * self->unk_030;
        if (me != i) {
            char* otherRider = race->riders[i];
            if (((cBeing_FB40*)(otherRider + 0x6C0))->isHuman()) {
                prim = 13;
                icon = 10;
            } else {
                prim = 0xE;
                icon = 9;
            }
        }
        sIcon_FB40* iconEntry = &D_004C8C58_FB40[icon];
        sTex_FB40* iconTexture = iconEntry->tex;
        float y0 = y - iconEntry->h * 0.5f;
        float y1 = y + iconEntry->h * 0.5f;
        if (y0 <= top)
            continue;
        if (bottom <= y1)
            continue;
        float fadeAlpha = 1.0f;
        if (y0 < topFade)
            fadeAlpha = 1.0f - (topFade - y0) * 0.01666666753590107f;
        else if (y1 > bottomFade)
            fadeAlpha = 1.0f - (y1 - bottomFade) * 0.01666666753590107f;
        if (fadeAlpha < 0.0f)
            fadeAlpha = 0.0f;
        float u0 = iconTexture->u0;
        float u1 = iconTexture->u1;
        if (right == 0) {
            u0 = iconTexture->u1;
            u1 = iconTexture->u0;
        }
        int alpha = (int)(fadeAlpha * 128.0f);
        int k;
        for (k = 0; k < 4; k++)
            verts[k].a = alpha;
        verts[0].u = u0;
        verts[0].v = iconTexture->v0;
        verts[1].u = u1;
        verts[1].v = iconTexture->v0;
        verts[2].u = u0;
        verts[2].v = iconTexture->v1;
        verts[3].u = u1;
        verts[3].v = iconTexture->v1;
        if (right != 0) {
            corner.x = x;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[0].pos = corner;
            corner.x = x + iconEntry->w;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[1].pos = corner;
            corner.x = x;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[2].pos = corner;
            corner.x = x + iconEntry->w;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[3].pos = corner;
        } else {
            corner.x = x - iconEntry->w;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[0].pos = corner;
            corner.x = x;
            corner.y = y0;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[1].pos = corner;
            corner.x = x - iconEntry->w;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[2].pos = corner;
            corner.x = x;
            corner.y = y1;
            corner.z = 0.0f;
            corner.w = 1.0f;
            verts[3].pos = corner;
        }
        ((cGfx_FB40*)GrMan)->stateTop->SetPrim(prim);
        *(short*)((char*)((cGfx_FB40*)GrMan)->stateTop + 0x10) = iconTexture->GetHandle();
        ((cGfx_FB40*)GrMan)->RenderTriStrip(4, verts, 0);
    }

    for (int i = 0; i < 4; i++)
        verts[i].a = 0x80;
    if (*(int*)(course + 8) != 0) {
        int show = 1;
        if ((D_005308D0[0] >> 9) & 1) {
            if (SSXApp->GameModeMan->bIsTimedEvent != 0)
                show = 0;
        }
        for (int i = 0; i < *(int*)(course + 8); i++) {
            float d = markers[i].dist;
            if (d < lo)
                continue;
            if (d > hi)
                continue;
            if (markers[i].type == 1 && !show)
                continue;
            sIcon_FB40* iconEntry = &D_004C8C58_FB40[markers[i].type + 5];
            sTex_FB40* iconTexture = iconEntry->tex;
            float hw = iconEntry->w * 0.5f;
            float hh = iconEntry->h * 0.5f;
            float y = bottom - (d - lo) * self->unk_030;
            float y0 = y - hh;
            float y1 = y + hh;
            if (y0 <= top)
                continue;
            if (bottom <= y1)
                continue;
            float fadeAlpha = 1.0f;
            if (y0 < topFade)
                fadeAlpha = 1.0f - (topFade - y0) * 0.01666666753590107f;
            else if (y1 > bottomFade)
                fadeAlpha = 1.0f - (y1 - bottomFade) * 0.01666666753590107f;
            if (fadeAlpha < 0.0f)
                fadeAlpha = 0.0f;
            float u0 = iconTexture->u0;
            float u1 = iconTexture->u1;
            if (right != 0)
                u0 = (u0 + u1) * 0.5f;
            else
                u1 = (u0 + u1) * 0.5f;
            verts[0].u = u0;
            verts[0].v = iconTexture->v0;
            verts[1].u = u1;
            verts[1].v = iconTexture->v0;
            verts[2].u = u0;
            verts[2].v = iconTexture->v1;
            verts[3].u = u1;
            verts[3].v = iconTexture->v1;
            int alpha = (int)(fadeAlpha * 128.0f);
            int k;
            for (k = 0; k < 4; k++)
                verts[k].a = alpha;
            if (right != 0) {
                corner.x = x;
                corner.y = y0;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[0].pos = corner;
                corner.x = x + hw;
                corner.y = y0;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[1].pos = corner;
                corner.x = x;
                corner.y = y1;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[2].pos = corner;
                corner.x = x + hw;
                corner.y = y1;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[3].pos = corner;
            } else {
                corner.x = x - hw;
                corner.y = y0;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[0].pos = corner;
                corner.x = x;
                corner.y = y0;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[1].pos = corner;
                corner.x = x - hw;
                corner.y = y1;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[2].pos = corner;
                corner.x = x;
                corner.y = y1;
                corner.z = 0.0f;
                corner.w = 1.0f;
                verts[3].pos = corner;
            }
            ((cGfx_FB40*)GrMan)->stateTop->SetPrim(11);
            *(short*)((char*)((cGfx_FB40*)GrMan)->stateTop + 0x10) = iconTexture->GetHandle();
            ((cGfx_FB40*)GrMan)->RenderTriStrip(4, verts, 0);
        }
    }
    ((cGfx_FB40*)GrMan)->stateTop--;
    ((cGfx_FB40*)GrMan)->PopView();
    self->unk_034 = me;
}
#endif

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_004C8BC8;

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002105B0);
#ifdef SKIP_ASM
/**
 * Flags rider @p riderIndex's course-progress slot for a reset.
 *
 * Sets the first word of slot @p riderIndex of D_004C8BC8 (0x18-byte slots): the next update (func_00210618) then takes the
 * rider's real progress instead of limiting the change, and func_00210820 invalidates the slot's smoothed offset.
 * cRider_putAt calls it when a rider is moved.
 *
 * @param riderIndex  the rider's index in the race
 */
extern "C" void func_002105B0(int riderIndex)
{
    char* slot = (char*)&D_004C8BC8 + riderIndex * 0x18;
    *(int*)slot = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002105D0);
#ifdef SKIP_ASM
extern char* cHUDRadar_mpRadarHeader;
extern char* D_004A2754;
extern char* D_004A2758;
extern "C" void func_002108F8(void);

/**
 * Installs the course-progress data of a resolved world chunk (kind 21 in cPS2ChunkResolve_resolve).
 *
 * Stores @p data in cHUDRadar_mpRadarHeader, its segment table (data + the offset at +0x4) in D_004A2754 and its marker table (data
 * + the offset at +0xC) in D_004A2758, then resets every rider slot (func_002108F8). The data also holds the segment
 * count (+0x0), the marker count (+0x8) and the course length (+0x10).
 *
 * @param data  the chunk's data
 */
extern "C" void func_002105D0(char* data)
{
    char* segments = data + *(int*)(data + 4);
    char* markers = data + *(int*)(data + 0xC);
    cHUDRadar_mpRadarHeader = data;
    D_004A2754 = segments;
    D_004A2758 = markers;
    func_002108F8();
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210608);
#ifdef SKIP_ASM
extern char* cHUDRadar_mpRadarHeader;
extern char* D_004A2754;
extern char* D_004A2758;

/**
 * Forgets the course-progress data: clears cHUDRadar_mpRadarHeader, D_004A2754 and D_004A2758 (cPS2ChunkResolve_unresolve calls it).
 */
extern "C" void func_00210608(void)
{
    cHUDRadar_mpRadarHeader = 0;
    D_004A2754 = 0;
    D_004A2758 = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210618);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char* cHUDRadar_mpRadarHeader;
extern char* D_004A2754;
float cRider_getDistanceLeft(void* rider);
extern "C" float func_00210820(int i);

struct sTrophyEntry_210618 {
    int active;
    int seg;
    float dist;
    float pos;
    int pad[2];
};

struct sTrophySeg_210618 {
    char pad[0x10];
    float dist;
};

// The unit declares D_004C8BC8 as a 16-byte pad struct; view it as the per-rider table by asm label.
extern sTrophyEntry_210618 D_004C8BC8_210618[] __asm__("D_004C8BC8");

/**
 * Updates rider @p i's course progress: its distance along the course, current segment and lateral position.
 *
 * Does nothing without course data or for an index past the race's rider count. The distance is the course length
 * (+0x10) minus cRider_getDistanceLeft; unless the slot's reset flag is set it may change by at most 138.89 per call.
 * At or below 0 the slot gets segment 0 and distance 0; at or past the last segment's start it gets the last segment
 * and the full course length; otherwise the segment index is walked from its previous value to the segment holding the
 * distance (0x14-byte segments, start distance at +0x10). The lateral position comes from func_00210820.
 *
 * @param i  the rider's index in the race
 */
extern "C" void func_00210618(int i)
{
    char* course = cHUDRadar_mpRadarHeader;
    sTrophySeg_210618* segs = (sTrophySeg_210618*)D_004A2754;
    if (course == 0)
        return;
    char* race = (char*)SSXApp->Game->AI;
    if (i >= *(int*)(race + 0x78))
        return;
    float travelled = *(float*)(course + 0x10) - cRider_getDistanceLeft(*(void**)(race + (i << 2) + 0x28));
    if (D_004C8BC8_210618[i].active == 0)
    {
        float prev = D_004C8BC8_210618[i].dist;
        float dd = travelled - prev;
        float lim = 138.88890075683594f;
        if (dd > lim)
            travelled = prev + lim;
        else if (dd < -lim)
            travelled = prev - lim;
    }
    if (travelled <= 0.0f)
    {
        D_004C8BC8_210618[i].seg = 0;
        D_004C8BC8_210618[i].dist = 0.0f;
        D_004C8BC8_210618[i].pos = func_00210820(i);
    }
    else
    {
        int last = *(int*)course - 1;
        if (segs[last].dist <= travelled)
        {
            D_004C8BC8_210618[i].seg = last;
            D_004C8BC8_210618[i].dist = *(float*)(course + 0x10);
            D_004C8BC8_210618[i].pos = func_00210820(i);
        }
        else
        {
            int k = D_004C8BC8_210618[i].seg;
            while (segs[k].dist <= travelled)
                k++;
            while (travelled < segs[k].dist)
                k--;
            D_004C8BC8_210618[i].seg = k;
            D_004C8BC8_210618[i].dist = travelled;
            D_004C8BC8_210618[i].pos = func_00210820(i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210820);
#ifdef SKIP_ASM
class cPosObj_210820 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual t4Vector* getPosition();
};
struct sZone_210820 {
    float sx, sy, ox, oy, f10;
};
struct sSlot_210820 {
    int active;
    int zone;
    int b, c, d, e;
};
extern sSlot_210820 D_slots210820[6] __asm__("D_004C8BC8");
extern char* D_004A2754;

static inline float subx_210820(sZone_210820* z, t4Vector& p)
{
    return p.x - z->ox;
}
static inline float suby_210820(sZone_210820* z, t4Vector& p)
{
    return p.y - z->oy;
}

static inline float sx_210820(sZone_210820* z)
{
    return z->sx;
}
static inline float sy_210820(sZone_210820* z)
{
    return z->sy;
}

/**
 * Returns rider @p idx's lateral position in its current course segment.
 *
 * Projects the rider's position (cBeing getPosition, at rider + 0x6C0) relative to the segment's origin (+0x8, +0xC)
 * onto the segment's axis (+0x0, +0x4): (x - ox) * ax + (y - oy) * ay. When the slot's reset flag is set it also
 * invalidates the slot's smoothed offset (+0x14).
 *
 * @param idx  the rider's index in the race
 * @return the lateral position
 */
extern "C" float func_00210820(int idx)
{
    sZone_210820* zones = (sZone_210820*)D_004A2754;
    char* world = (char*)SSXApp->Game->AI;
    char* rider = *(char**)(world + (idx << 2) + 0x28);
    t4Vector pos = *((cPosObj_210820*)(rider + 0x6C0))->getPosition();
    float dy = suby_210820(&zones[D_slots210820[idx].zone], pos);
    float dx = subx_210820(&zones[D_slots210820[idx].zone], pos);
    float lateral = dy * sy_210820(&zones[D_slots210820[idx].zone]) + dx * sx_210820(&zones[D_slots210820[idx].zone]);
    if (D_slots210820[idx].active) {
        D_slots210820[idx].e = 0;
    }
    return lateral;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_002108F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sTrophySlot2108F8 {
    int active;
    int a, b, c, d, e;
};
extern sTrophySlot2108F8 D_slots2108F8[6] __asm__("D_004C8BC8");
extern int D_004A2760;

static inline void resetSlot2108F8(sTrophySlot2108F8* s)
{
    s->active = 1;
    s->a = 0;
    s->b = 0;
    s->c = 0;
    s->d = 0;
    s->e = 0;
}

/**
 * Resets all six rider slots of the course-progress table D_004C8BC8.
 *
 * Each slot gets its reset flag set and its segment, distance, position and smoothed offset cleared; then D_004A2760 is
 * set so the trackers wait for the next update (func_0020ED20).
 */
extern "C" void func_002108F8(void)
{
    int i;
    for (i = 0; i < 6; i++) {
        resetSlot2108F8(&D_slots2108F8[i]);
    }
    D_004A2760 = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210940);
#ifdef SKIP_ASM
struct sColor_210940 {
    float r, g, b, a;
    sColor_210940(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};
class cUIText;
class cUIObj_210940 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int v);
    virtual void v10();
    virtual int setColor(const sColor_210940& c);
};

struct sRect_210940 { int a, b, c, d; };
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
void cUIText_setAsciiString(cUIText* text, const char* s);
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cUITextScroll_setInputMap(void* self, int a1);
extern char str_282ReadPrivateMessage_b[];
extern char str_TextFromX_b[];
extern char str_AgreementSlider_b[];
extern char str_ChatUpArrow_b[];
extern char str_ChatDownArrow_b[];
extern char str_slideroutline2_b[];
extern char str_textScrollChat_b[];
extern sRect_210940 D_004C8D08;

/**
 * Builds the in-race private-message screen ("282ReadPrivateMessage") of the online chat overlay (overlay id 40).
 *
 * Shows "TextFromX" with the opponent's name (cBENetworkInterface_networkOpt.unk_498, as an ASCII string). "AgreementSlider" becomes a vertical
 * scroll bar (mAttr bit 0) with "ChatUpArrow", "ChatDownArrow" and "slideroutline2" as its mpUpArrow, mpDownArrow and
 * mpHideOnEmpty and meHideState 2. The chat scroller "textScrollChat" gets bit 0 of mAttrTextScroll and input map 1
 * (cUITextScroll_setInputMap), is shown, gets the wrap bit (mAttr 0x80), the colour (1, 0, 0, 0) through its virtual
 * slot 11, mSelectedColour D_004C8D08 and the scroll bar as mpScrollBar. Ends with cOVState_onCreateScreen; returns
 * early when the screen cannot be added. "AgreementSlider" is used without a null check.
 */
extern "C" void func_00210940(cOVState* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_282ReadPrivateMessage_b), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen == 0)
        return;
    cUIScreen_playFrame(screen, 0, 0);
    cUIObj_210940* fromText = (cUIObj_210940*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_TextFromX_b));
    if (fromText != 0)
    {
        fromText->setVisible(1);
        cBE_getInterface_Fv(SSXApp->BE, 7);
        cUIText_setAsciiString((cUIText*)fromText, cBENetworkInterface_networkOpt.unk_498);
    }
    cUIScrollBar* scrollBar = (cUIScrollBar*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_AgreementSlider_b));
    scrollBar->mAttr |= 1;
    void* upArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatUpArrow_b));
    void* downArrow = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ChatDownArrow_b));
    void* sliderOutline = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_slideroutline2_b));
    scrollBar->mpUpArrow = (cUIThing*)upArrow;
    scrollBar->mpDownArrow = (cUIThing*)downArrow;
    scrollBar->mpHideOnEmpty = (cUIThing*)sliderOutline;
    scrollBar->meHideState = 2;
    cUIObj_210940* chatScroll = (cUIObj_210940*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_textScrollChat_b));
    if (chatScroll != 0)
    {
        *(int*)((char*)chatScroll + 0x74) |= 1;
        cUITextScroll_setInputMap(chatScroll, 1);
        chatScroll->setVisible(1);
        *(int*)((char*)chatScroll + 0x14) |= 0x80;
        chatScroll->setColor(sColor_210940(1.0f, 0.0f, 0.0f, 0.0f));
        *(sRect_210940*)((char*)chatScroll + 0xA8) = D_004C8D08;
        *(char**)((char*)chatScroll + 0xB8) = ((char*)scrollBar);
    }
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210B58);
#ifdef SKIP_ASM
extern char D_004A2560[];
extern "C" int cGameComm_getQueuedChat(int a, char* name, int len);
extern "C" void func_00210BD8(void* self, char* name, int a);
extern "C" void func_00210F40(void* self, int a1, int a2, int a3, int a4, int a5, int a6);

/**
 * Starts the chat overlay: on the first transition it shows a waiting incoming message, or else opens the keyboard.
 *
 * When @p ok is set it reads an incoming chat packet into the buffer at +0x9C (cGameComm_getQueuedChat, up to 65 bytes) and
 * appends it as the opponent's line (func_00210BD8); without one it opens the keyboard (func_00210F40 with empty texts
 * and a 61-character limit).
 *
 * Likely bug: 65 bytes are allowed into the buffer at +0x9C, which ends at +0xDC, where func_00210FA0 keeps a word.
 *
 * @param ok  nonzero on the first transition
 * @return 1 (the transition is done)
 */
extern "C" int func_00210B58(void* self, int ok)
{
    if (ok != 0) {
        char* name = (char*)self + 0x9C;
        if (cGameComm_getQueuedChat((int)cGameComm_mGameComm, name, 65) != 0) {
            func_00210BD8(self, name, 0);
        } else {
            // PORT: func_00210F40's unit definition takes these string pointers as int.
            func_00210F40(self, 0, (int)D_004A2560, (int)D_004A2560, 0, 0, 61);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210BD8);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cUITextScroll_addAsciiString(void* text, char* str);
extern "C" void func_001A83D8(void* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim);
extern char str_textScrollChat_b[];
extern char str_colon_sp_b[];


/**
 * Appends a chat line "<name>: <text>" to the chat overlay's scroller.
 *
 * Finds "textScrollChat" and does nothing without it. It saves the scroller's mLineSize, mSelectedIndex and
 * mLinesOffset, builds the line in a cBXString from our name (cBENetworkInterface_networkOpt.unk_488) when @p ownMessage is set or the opponent's
 * (cBENetworkInterface_networkOpt.unk_498) otherwise, ": " (str_colon_sp_b) and @p name, adds it with cUITextScroll_addAsciiString, then func_001A83D8
 * trims the scroller to 30 lines and restores its scroll state from the saved values.
 *
 * @param name  the message text
 * @param ownMessage  nonzero for our own message, 0 for the opponent's
 */
extern "C" void func_00210BD8(void* self, char* name, int ownMessage)
{
    cUITextScroll* obj = (cUITextScroll*)((char*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(str_textScrollChat_b)));
    if (((char*)obj) == 0) {
        return;
    }
    unsigned short lineCount = obj->mLineSize;
    unsigned short selectedIndex = obj->mSelectedIndex;
    unsigned short linesOffset = obj->mLinesOffset;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    if (ownMessage != 0) {
        cBXString lineText;
        cBXString_cBXString2(&lineText, cBENetworkInterface_networkOpt.unk_488);
        cBXString_Concat(&lineText, str_colon_sp_b);
        cBXString_Concat(&lineText, name);
        cUITextScroll_addAsciiString(obj, lineText.Str);
        cBXString__cBXString(&lineText, 2);
    } else {
        cBXString lineText;
        cBXString_cBXString2(&lineText, cBENetworkInterface_networkOpt.unk_498);
        cBXString_Concat(&lineText, str_colon_sp_b);
        cBXString_Concat(&lineText, name);
        cUITextScroll_addAsciiString(obj, lineText.Str);
        cBXString__cBXString(&lineText, 2);
    }
    func_001A83D8(obj, 0x1E, 8, lineCount, selectedIndex, linesOffset, 0, 1);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210D20);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cOVState_onUpdate(void* self);
extern "C" void* cUIStateStack_popToState(void* list);
extern "C" int cGameComm_getQueuedChat(int a, char* name, int len);
extern "C" void func_00210BD8(void* self, char* name, int a);

/**
 * Updates the chat overlay each frame: closes it when the online pause runs out and shows incoming messages.
 *
 * After cOVState_onUpdate, if cGameComm_mGameComm exists and its mPauseTimeout or mOverlayTimeout has reached 0, it
 * closes the engine's popups (cUIStateStack_popToState on the state stack) and sets this state's mAttrState to mode 7 with
 * mPendingDelete. While the screen exists it then reads an incoming chat packet into +0x9C (cGameComm_getQueuedChat, up to 65
 * bytes) and appends it as the opponent's line (func_00210BD8).
 */
extern "C" void func_00210D20(cOVState* self)
{
    cOVState_onUpdate(self);
    cGameComm* gameComm = cGameComm_mGameComm;
    if (gameComm != 0) {
        int ok = gameComm->mPauseTimeout == 0 || gameComm->mOverlayTimeout == 0;
        if (ok) {
            cUIStateStack_popToState((char*)self->mpEngine + 0x18);
            self->mAttrState = (self->mAttrState & ~0x3F00) | 0x780;
        }
    }
    if ((int)self->mpScreen != 0) {
        char* name = (char*)self + 0x9C;
        if (cGameComm_getQueuedChat((int)cGameComm_mGameComm, name, 65) != 0) {
            func_00210BD8(self, name, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210DD0);
#ifdef SKIP_ASM
/**
 * Forgets the chat overlay's keyboard popup (+0xE0) when that popup reports event 22.
 *
 * @param popup  the popup that sent the event
 * @param event  the event id; only 22 is handled
 */
extern "C" void func_00210DD0(void* self, int popup, int event)
{
    if (event == 22) {
        if (popup == *(int*)((char*)self + 0xe0)) {
            *(int*)((char*)self + 0xe0) = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210DF0);
#ifdef SKIP_ASM
extern char D_004A2560[];
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void cOVState_onWidgetEvent(void* self);
extern "C" void cGameComm_sendChat(int a, char* name);
// PORT: cUIStateStack_pop returns the last node it touched; bound with its value return.
void* cUIStateStack_pop_r(void* list, int a1) __asm__("cUIStateStack_pop");
extern "C" int strlen(const char* s);

/**
 * Handles the chat overlay's widget events: closing it, opening the keyboard and sending the typed message.
 *
 * Ignored without a screen. Event 6, while no keyboard is open, pops the top state (cUIStateStack_pop on the engine's
 * stack) and sets this state's mAttrState to mode 7 with mPendingDelete. Event 5, while no keyboard is open, opens the
 * keyboard (func_00210F40, empty texts, 61 characters) and calls cKeyboardPopup_setIncludeKey(75, 1, 2) on it. Event 13
 * first shows any incoming message (cGameComm_getQueuedChat, func_00210BD8), then sends the keyboard's mszEditString (+0x74) when
 * it is not empty (cGameComm_sendChat on cGameComm_mGameComm) and appends it as our own line. Other events go to
 * cOVState_onWidgetEvent. Event 13 reads the keyboard at +0xE0 without checking that one is open.
 *
 * @param item  the widget (not read)
 * @param key  the event id
 */
extern "C" void func_00210DF0(cOVState* self, void* item, unsigned int key)
{
    if ((int)self->mpScreen == 0) {
        return;
    }
    switch (key) {
    case 6:
        if (*(int*)((char*)self + 0xE0) == 0) {
            cUIStateStack_pop_r((char*)self->mpEngine + 0x18, 1);
            self->mAttrState = (self->mAttrState & ~0x3F00) | 0x780;
        }
        break;
    case 5:
        if (*(int*)((char*)self + 0xE0) == 0) {
            // PORT: func_00210F40's unit definition takes these string pointers as int.
            func_00210F40(self, 0, (int)D_004A2560, (int)D_004A2560, 0, 0, 61);
            cKeyboardPopup_setIncludeKey(*(void**)((char*)self + 0xE0), 75, 1, 2);
        }
        break;
    case 13: {
        char* name = (char*)self + 0x9C;
        if (cGameComm_getQueuedChat((int)cGameComm_mGameComm, name, 65) != 0) {
            func_00210BD8(self, name, 0);
        }
        char* typedText = *(char**)((char*)self + 0xE0) + 0x74;
        if (strlen(typedText) != 0) {
            cGameComm_sendChat((int)cGameComm_mGameComm, typedText);
            func_00210BD8(self, typedText, 1);
        }
        break;
    }
    default:
        cOVState_onWidgetEvent(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210F40);
#ifdef SKIP_ASM
extern "C" void func_00210FA0(cOVState* self, int a1, int a2, int a3, int a4);
extern "C" void cKeyboardPopup_setStatic2(void* p, int a);
extern "C" void cKeyboardPopup_setInputText(void* p, int a);

/**
 * Opens the chat overlay's keyboard with a prompt and a starting text.
 *
 * func_00210FA0 creates and pushes the keyboard popup, cKeyboardPopup_setStatic2 sets its static (prompt) text to @p staticText and
 * cKeyboardPopup_setInputText its edit text to @p inputText.
 *
 * @param textObj  stored at +0xDC by func_00210FA0 (the callers pass 0)
 * @param staticText  the prompt text (a char*)
 * @param inputText  the initial edit text (a char*)
 * @param password  nonzero for a password keyboard
 * @param miscData  the popup's miMiscData
 * @param maxChars  the text length limit
 */
extern "C" void func_00210F40(void* self, int textObj, int staticText, int inputText, int password, int miscData, int maxChars)
{
    func_00210FA0((cOVState*)self, textObj, password, miscData, maxChars);
    cKeyboardPopup_setStatic2(*(void**)((char*)self + 0xE0), staticText);
    cKeyboardPopup_setInputText(*(void**)((char*)self + 0xE0), inputText);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", func_00210FA0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, int a4);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern "C" void cKeyboardPopup_setMaxCharacters(void* self, int v);
extern "C" void cKeyboardPopup_setIncludeKey(void* self, int i, int v, int mode);
extern "C" void cKeyboardPopup_setIncludeQuickKeyRow(void* self, int v);
extern char str_cKeyboardPopup_b[];

/**
 * Creates the chat overlay's keyboard popup and pushes it on the UI state stack.
 *
 * Allocates a 1092-byte cKeyboardPopup ("cKeyboardPopup", flags 0x100), constructs it for this state's engine with this
 * state as owner, @p password and controller bits 0xF, stores it at +0xE0 and pushes it with cUIStateStack_pushPopup on
 * the stack reached through the screen (mpScreen + 0xD0, then + 0x10, + 0x18). Then it sets the popup's miMiscData
 * to @p miscData, cKeyboardPopup_setMaxCharacters(@p maxChars), meType 0, includes key 75 and keys 0..13
 * (cKeyboardPopup_setIncludeKey, cKeyboardPopup_setIncludeQuickKeyRow), sets mbCallParentsUpdate and stores @p textObj at +0xDC.
 *
 * @param textObj  stored at +0xDC (the callers pass 0)
 * @param password  nonzero for a password keyboard
 * @param miscData  the popup's miMiscData
 * @param maxChars  the text length limit
 */
extern "C" void func_00210FA0(cOVState* self, int textObj, int password, int miscData, int maxChars)
{
    void* w = cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(1092, str_cKeyboardPopup_b, 0x100, 0), self->mpEngine, self, password, 0xF);
    *(void**)((char*)self + 0xE0) = w;
    cUIStateStack_pushPopup((char*)*(void**)((char*)*(void**)((char*)self->mpScreen + 0xD0) + 0x10) + 0x18, w);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x18) = miscData;
    cKeyboardPopup_setMaxCharacters(*(void**)((char*)self + 0xE0), maxChars);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x43C) = 0;
    cKeyboardPopup_setIncludeKey(*(void**)((char*)self + 0xE0), 75, 1, 2);
    cKeyboardPopup_setIncludeQuickKeyRow(*(void**)((char*)self + 0xE0), 1);
    *(int*)((char*)*(void**)((char*)self + 0xE0) + 0x440) = 1;
    *(int*)((char*)self + 0xDC) = textObj;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatetrophy", cOVState_PROFILE_cOVState_PROFILE);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char cBENewRaceInterface_newRace[];
extern char cOVState_PROFILE_vtbl[];
extern "C" void cFEMemCard_cFEMemCard2(void* self, int a, int b);
extern "C" int cBENewPlayerInterface_getPlayerCtrlID(void* iface, int i);
/**
 * Constructs the in-game profile (load/save) overlay for the controller that opened it.
 *
 * Runs cFEMemCard_cFEMemCard2 (the memory card screen base, type 0), sets the state id (+0xC) to 0x24, installs
 * cOVState_PROFILE_vtbl, stores @p parentOverlay (the overlay to return to) at +0x278 and clears +0x1CC, +0x22C and +0x230.
 * Then turns @p ucActiveControl into a controller index and stores at +0x1CC which player (one, or two in a two-player race)
 * uses that controller (cBENewPlayerInterface_getPlayerCtrlID).
 *
 * Likely bug (also in On Tour): the loop shifts @p ucActiveControl right by the loop counter each pass (1, then 2 more, ...)
 * instead of by 1, so bits 0 and 1 give controllers 0 and 1 but bit 2 gives 1 and bit 3 gives 2.
 *
 * @param pEngine  the UI engine
 * @param ucActiveControl  bit mask of the active controller (On Tour ucActiveControl)
 * @param parentOverlay  the overlay to return to (On Tour ov, kept in mParentOV)
 * @return @p self
 */
extern "C" cOVState_PROFILE* cOVState_PROFILE_cOVState_PROFILE(cOVState_PROFILE* self, int pEngine, unsigned char ucActiveControl, int parentOverlay)
{
    int idx = 0;
    cFEMemCard_cFEMemCard2((self), pEngine, 0);
    self->mHashName = 36;
    self->mVtbl = (void**)cOVState_PROFILE_vtbl;
    self->mParentOV = parentOverlay;
    self->mPlayerIdx = 0;
    self->mIsOn = 0;
    self->mIsHidden = 0;
    int i;
    for (i = 1; i < 9; i++) {
        ucActiveControl >>= i;
        if (ucActiveControl == 0) {
            idx = i - 1;
            break;
        }
    }
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int lim = 1;
    if (cBENewRaceInterface_newRace[0x49] == 2) lim = 2;
    for (int n = 0; n < lim; n++) {
        if (cBENewPlayerInterface_getPlayerCtrlID(iface, n) == idx) {
            self->mPlayerIdx = n;
            break;
        }
    }
    return (self);
}
#endif

