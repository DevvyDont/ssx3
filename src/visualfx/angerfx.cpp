#include "common.h"
#include "bx/math3d.h"
#include "camera/camera.h"
#include "vtable.h"
#include "visualfx/avalanche.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("visualfx/angerfx", cAngerFX_cAngerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/angerfx_classes.h"

/**
 * Constructs an anger effect: allocates its two 0x2C-byte parts tagged "cAngerFX" and clears them.
 *
 * func_002D4BE0() fills the 0x58 bytes of the parts with zeros. In this unit's model the class derives
 * virtually from cRider, so g++ also builds that base when constructing the complete object.
 */
cAngerFX_ang::cAngerFX_ang()
{
    cAngerPart_ang** slot = &parts;
    *slot = new (str_cAngerFX, 0, 0) cAngerPart_ang[2];
    func_002D4BE0_erFX(this);
}

// PORT: the compiler names this ctor __12cAngerFX_angi; expose it under the address-derived name.
__asm__(".globl cAngerFX_cAngerFX\n.type cAngerFX_cAngerFX,@function\ncAngerFX_cAngerFX = __12cAngerFX_angi\n.size cAngerFX_cAngerFX,0x180\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("__12cRiderVb_agf = cRider_cRider\n"
        "_vt$12cAngerFX_ang$13cRiderVbC_agf = D_00488AC0\n"
        "_vt$12cAngerFX_ang$14cRiderVbBC_agf = D_00459B90\n"
        "_vt$12cAngerFX_ang$12cRiderVb_agf = D_00488B08\n");
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", cAngerFX__cAngerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/angerfx_classes.h"

/**
 * Destroys a rider's anger effect: frees its state buffer (+4), then destroys the virtual cRider base when in
 * charge.
 */
cVbDtor002D4A70::~cVbDtor002D4A70()
{
    if (m04 != 0) cMemMan_free(m04);
}

// PORT: the compiler names this dtor _$_15cVbDtor002D4A70; expose it under the address-derived name.
__asm__(".globl cAngerFX__cAngerFX\n.type cAngerFX__cAngerFX,@function\ncAngerFX__cAngerFX = _$_15cVbDtor002D4A70\n.size cAngerFX__cAngerFX,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002D4A70$13cRiderVbC_agf = D_00488AC0\n"
        "_vt$15cVbDtor002D4A70$14cRiderVbBC_agf = D_00459B90\n"
        "_vt$15cVbDtor002D4A70$12cRiderVb_agf = D_00488B08\n"
        "_$_12cRiderVb_agf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4BE0);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int c, int n);

/**
 * Clears an anger effect's per-view state: fills the 0x58 bytes of its two 0x2C-byte parts (+0x4) with zeros.
 *
 * Calls MEM_fill. The cAngerFX constructor, cRiderHandler_resetRiderFX and cRiderHandler_readFromReplayFrame call it on
 * the cAngerFX at cRiderHandler + 0xAF0.
 */
extern "C" void func_002D4BE0(cAngerFX* self)
{
    MEM_fill(*(void**)((char*)self + 4), 0, 88);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4C08);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int cBEAggressionInterface_getAggression(void* self, int a, int b);
extern float D_00488EA0[];
extern float D_00488F48[];
extern float D_00488FF0[];
extern float D_00489070[];
extern float D_004890F0[];
extern float D_00489150[];

struct sCol_2D4C08 {
    float r, g, b, a;
};
extern sCol_2D4C08 D_004D5B30;
extern sCol_2D4C08 D_004D5B40;
extern sCol_2D4C08 D_004D5B50;
extern sCol_2D4C08 D_004D5B60;

struct sAngerSlot_2D4C08 {
    float level;            // 0x0
    sCol_2D4C08 col;        // 0x4
    float glowA;            // 0x14
    float glowB;            // 0x18
    int anger;              // 0x1C
    int t20;                // 0x20
    int t24;                // 0x24
    int t28;                // 0x28
};

struct sAnger_2D4C08 {
    char* rider;                    // 0x0
    sAngerSlot_2D4C08* slots;       // 0x4
};

struct sPlayers_2D4C08 {
    int f0;
    char* p[3];             // 0x4
    unsigned int count;     // 0x10
};

struct sVE_2D4C08 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int riderV_2D4C08(char* rider, int n)
{
    char* obj = rider + 0x6C0;
    sVE_2D4C08* vt = *(sVE_2D4C08**)obj;
    return vt[n].fn(vthis(obj, vt[n]));
}

static inline float approach_2D4C08(float x, float target, float step)
{
    if (target + step < x) {
        return x - step;
    }
    if (x < target - step) {
        return x + step;
    }
    return target;
}

static inline sPlayers_2D4C08* players_2D4C08()
{
    return (sPlayers_2D4C08*)SSXApp->Game->ViewMan;
}

/**
 * Updates a computer rider's anger indicator, one 0x2C-byte part per viewing player.
 *
 * Does nothing for a human rider (cRider virtual slot kRiderVt_isHuman). For each view of the game's cGameViewMan
 * (count at +0x10, views at +0x4) it takes the rider that view follows (view +0xA8, +0x20, +0x4) and skips empty ones.
 * When this rider is in an NIS (mIsInNIS) or the replay (cGame Replay, first word) is in a state from 1 to 9, it zeroes
 * the part's level (+0x0) and returns, leaving the remaining views as they are.
 *
 * Otherwise it stores the aggression between the two riders (cBEAggressionInterface_getAggression with both riders'
 * kRiderVt_getAIIndex) at +0x1C and picks the colour (+0x4..+0x13): D_004D5B30 for 4 or more, D_004D5B40 for 3,
 * D_004D5B50 for 2, D_004D5B60 below. The level moves toward 1.0 by 0.1 a frame at aggression 2 or more, otherwise
 * toward 0 by 1/15, and the colour's first float (drawn as the alpha by func_002D5048) becomes level * 0.8.
 *
 * Three pulse counters follow: +0x20 (mod 41, started when the aggression changes), +0x24 (mod 31, started at
 * aggression 4 or more while +0x20 is 0) and +0x28 (mod 24, restarted when the aggression rises, forced to 0 while
 * either other counter runs). The glow scales +0x14 and +0x18 become the level times the products of the tables
 * D_00488EA0/D_00488FF0/D_004890F0 and D_00488F48/D_00489070/D_00489150 indexed by the three counters.
 * cRiderHandler_updateRiderFX and cAI_update call it on cRiderHandler + 0xAF0.
 */
extern "C" void func_002D4C08(sAnger_2D4C08* self)
{
    if (riderV_2D4C08(self->rider, 8) != 0) {
        return;
    }
    for (unsigned int i = 0; i < players_2D4C08()->count; i++) {
        cGame* game = SSXApp->Game;
        char* other = *(char**)(*(char**)(*(char**)(((sPlayers_2D4C08*)game->ViewMan)->p[i] + 0xA8) + 0x20) + 4);
        if (other == 0) {
            continue;
        }
        sAngerSlot_2D4C08* st = &self->slots[i];
        if (*(int*)(self->rider + 0xAC4) != 0) {
            st->level = 0;
            return;
        }
        int n = *(int*)game->Replay;
        int ok = n != 0 && n < 10;
        if (ok) {
            st->level = 0;
            return;
        }
        void* aggr = cBEAggressionInterface_getThis();
        int riderAIIndex = riderV_2D4C08(self->rider, 7);
        int lvl = cBEAggressionInterface_getAggression(aggr, riderAIIndex, riderV_2D4C08(other, 7));
        int old = st->anger;
        st->anger = lvl;
        if (lvl >= 4) {
            st->col = D_004D5B30;
        } else if (lvl >= 3) {
            st->col = D_004D5B40;
        } else if (lvl >= 2) {
            st->col = D_004D5B50;
        } else {
            st->col = D_004D5B60;
        }
        if (lvl >= 2) {
            st->level = approach_2D4C08(st->level, 1.0f, 0.10000000894069672f);
        } else {
            st->level = approach_2D4C08(st->level, 0.0f, 0.06666667014360428f);
        }
        st->col.r = st->level * 0.800000011920929f;
        if (st->t20 > 0 || old != lvl) {
            st->t20++;
            st->t20 %= 41;
        }
        if (st->t24 > 0 || (lvl >= 4 && st->t20 == 0)) {
            st->t24++;
            st->t24 %= 31;
        }
        if (st->t28 > 0 || old < lvl) {
            if (old < lvl) {
                st->t28 = 0;
            }
            st->t28++;
            st->t28 %= 24;
            if (st->t20 > 0 || st->t24 > 0) {
                st->t28 = 0;
            }
        }
        float x = st->level;
        float ga = x * D_00488EA0[st->t20] * D_00488FF0[st->t24] * D_004890F0[st->t28];
        float gb = x * D_00488F48[st->t20] * D_00489070[st->t24] * D_00489150[st->t28];
        st->glowA = ga;
        st->glowB = gb;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5048);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_2D5048 {
    float x, y, z, w;
    sV4_2D5048() {}
    sV4_2D5048(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));


struct sVtx_2D5048 {
    float u, v, q, pad;     // 0x00
    int c[4];               // 0x10
    sV4_2D5048 pos;         // 0x20
    sVtx_2D5048() {}
    void SetC(int r, int g, int b, int a) { c[0] = r; c[1] = g; c[2] = b; c[3] = a; }
};

struct sBones_2D5048 {
    char pad[0x30];
    t4Matrix* mats;       // 0x30
};

struct sRState_2D5048 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sVE_2D5048 {
    short delta;
    short index;
    void* fn;
};

struct sGfx_2D5048 {
    char pad0[0xE84];
    sRState_2D5048* top;    // 0xE84
    char pad1[0xFAC - 0xE88];
    int texFAC;             // 0xFAC
    char pad2[0x10D8 - 0xFB0];
    sVE_2D5048* vtable;     // 0x10D8
};

struct sViews_2D5048 {
    char pad[0x14];
    int cur;                // 0x14
};

extern char* GrMan_angerfx __asm__("GrMan");
extern sRState_2D5048 sDrawState_s_defaultDrawState_2D5048 __asm__("sDrawState_s_defaultDrawState");

static inline sViews_2D5048* views_2D5048()
{
    return (sViews_2D5048*)SSXApp->Game->ViewMan;
}

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sV4_2D5048 MatVec_2D5048(const t4Matrix* m, const sV4_2D5048& v)
{
    sV4_2D5048 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sV4_2D5048 Scale_2D5048(const sV4_2D5048& v, float s)
{
    sV4_2D5048 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV4_2D5048 Add_2D5048(const sV4_2D5048& a, const sV4_2D5048& b)
{
    sV4_2D5048 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_2D5048 Sub_2D5048(const sV4_2D5048& a, const sV4_2D5048& b)
{
    sV4_2D5048 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline int rsTexFAC_2D5048(sGfx_2D5048* g)
{
    return g->texFAC;
}

static inline void rsSetA_2D5048(sGfx_2D5048* g, int mask, int shift, int v)
{
    g->top->flagsA = (g->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_2D5048(sGfx_2D5048* g, int mask, int shift, int v)
{
    g->top->flagsB = (g->top->flagsB & ~mask) | ((v << shift) & mask);
}

/**
 * Draws a computer rider's anger indicator for the current view: a camera-facing quad above the rider's head.
 *
 * Does nothing for a human rider (kRiderVt_isHuman) or when the current view's part (cGameViewMan +0x14 selects it) has
 * level 0. Takes the head position (row 3 of the mAnimModel matrix for mBoneHead), transforms it by the matrix from
 * GrMan's virtual slot 35 and draws only when the depth z is between 0 and 2000, fading the alpha out linearly from
 * 1500 to 2000. The quad's base sits 35 units up the camera's up axis from the head; its half-width is (glow +0x14 + z
 * * 0.003) * 9 along the camera's right axis and its height (glow +0x18 + z * 0.003) * 18.
 *
 * Pushes a render state (copies the top state, then loads sDrawState_s_defaultDrawState), sets flag A bit 22 and its
 * low two bits to 2, clears flag B bits 10-28 and sets its bits 5-9 to 7, uses texture GrMan +0xFAC, and draws four
 * vertices (UVs 0..1; colour from the part's floats +0x8, +0xC, +0x10 times 255, alpha from +0x4 times 128 and the
 * fade) through GrMan's virtual slot 71 before popping the state. cRiderHandler_renderRiderFX and cAI_render call it on
 * cRiderHandler + 0xAF0.
 */
extern "C" void func_002D5048(sAnger_2D4C08* self)
{
    if (riderV_2D4C08(self->rider, 8) != 0) {
        return;
    }
    float fade = 1.0f;
    sAngerSlot_2D4C08* st = &self->slots[views_2D5048()->cur];
    if (st->level == 0.0f) {
        return;
    }
    sGfx_2D5048* graphics = (sGfx_2D5048*)GrMan_angerfx;
    t4Matrix* viewMatrix = ((t4Matrix* (*)(void*))graphics->vtable[35].fn)(vthis(graphics, graphics->vtable[35]));
    t4Matrix* bm = &(*(sBones_2D5048**)(self->rider + 0x780))->mats[*(int*)(self->rider + 0x8A8)];
    sV4_2D5048 headPos = ((sV4_2D5048*)&bm->row[0])[3];
    sV4_2D5048 viewPos = MatVec_2D5048(viewMatrix, headPos);
    float z = viewPos.z;
    if (z <= 0.0f || 2000.0f <= z) {
        return;
    }
    float distScale = z * 0.003f;
    float widthScale = st->glowA + distScale;
    float heightScale = st->glowB + distScale;
    if (1500.0f < z) {
        fade = (2000.0f - z) * 0.002f;
    }
    sV4_2D5048 right(((sV4_2D5048*)&viewMatrix->row[0])[0].x, ((sV4_2D5048*)&viewMatrix->row[0])[1].x, ((sV4_2D5048*)&viewMatrix->row[0])[2].x, 0.0f);
    sV4_2D5048 up(((sV4_2D5048*)&viewMatrix->row[0])[0].y, ((sV4_2D5048*)&viewMatrix->row[0])[1].y, ((sV4_2D5048*)&viewMatrix->row[0])[2].y, 0.0f);
    sV4_2D5048 baseCentre = Add_2D5048(headPos, Scale_2D5048(up, 35.0f));
    sV4_2D5048 topCentre = Add_2D5048(baseCentre, Scale_2D5048(up, heightScale * 18.0f));
    sV4_2D5048 rr = Scale_2D5048(right, widthScale * 9.0f);

    sGfx_2D5048* gx = (sGfx_2D5048*)GrMan_angerfx;
    gx->top[1] = gx->top[0];
    gx->top++;
    *((sGfx_2D5048*)GrMan_angerfx)->top = sDrawState_s_defaultDrawState_2D5048;
    sGfx_2D5048* gs = (sGfx_2D5048*)GrMan_angerfx;
    rsSetA_2D5048(gs, 0x400000, 22, 1);
    rsSetA_2D5048(gs, 0x3, 0, 2);
    rsSetB_2D5048(gs, 0x1FFFFC00, 10, 0);
    rsSetB_2D5048(gs, 0x3E0, 5, 7);
    gs->top->tex = rsTexFAC_2D5048(gs);

    sCol_2D4C08 col = st->col;
    col.r *= fade;
    sVtx_2D5048 vtx[4];
    {
        sV4_2D5048 bottomLeft = Sub_2D5048(baseCentre, rr);
        vtx[0].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[0].q = 1.0f;
        vtx[0].pos = bottomLeft;
        vtx[0].u = 0.0f;
        vtx[0].v = 1.0f;
    }
    {
        sV4_2D5048 bottomRight = Add_2D5048(baseCentre, rr);
        vtx[1].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[1].q = 1.0f;
        vtx[1].pos = bottomRight;
        vtx[1].u = 1.0f;
        vtx[1].v = 1.0f;
    }
    {
        sV4_2D5048 topLeft = Sub_2D5048(topCentre, rr);
        vtx[2].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[2].q = 1.0f;
        vtx[2].pos = topLeft;
        vtx[2].u = 0.0f;
        vtx[2].v = 0.0f;
    }
    {
        sV4_2D5048 topRight = Add_2D5048(topCentre, rr);
        vtx[3].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[3].q = 1.0f;
        vtx[3].pos = topRight;
        vtx[3].u = 1.0f;
        vtx[3].v = 0.0f;
    }
    sGfx_2D5048* gd = (sGfx_2D5048*)GrMan_angerfx;
    ((void (*)(void*, int, sVtx_2D5048*, int))gd->vtable[71].fn)(vthis(gd, gd->vtable[71]), 4, vtx, 0);
    ((sGfx_2D5048*)GrMan_angerfx)->top--;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", tAvalanchePackedFrameData_setPositionOffset);
#ifdef SKIP_ASM
/**
 * Writes three floats, each truncated to an integer, into bytes 0..2 of a 10-byte packed avalanche frame record.
 *
 * tAvalancheNode_calculate is its only caller.
 *
 * @param rgb  the record
 * @param red  value for byte 0
 * @param green  value for byte 1
 * @param blue  value for byte 2
 */
extern "C" void tAvalanchePackedFrameData_setPositionOffset(tAvalanchePackedFrameData* rgb, float red, float green, float blue)
{
    rgb->m_x = (int)red;
    rgb->m_y = (int)green;
    rgb->m_z = (int)blue;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", tAvalanchePackedFrameData_setColour);
#ifdef SKIP_ASM
extern float cAvalanche_m_Brightness;

/**
 * Writes a brightened, clamped colour into bytes 3..5 of a 10-byte packed avalanche frame record.
 *
 * Scales each channel by 1.3 (cAvalanche_m_Brightness), clamps it to 0..1 and stores it times 255. tAvalancheNode_calculate is its
 * only caller.
 *
 * @param record  the record
 * @param red  red, 0..1 before scaling
 * @param green  green
 * @param blue  blue
 */
// PORT: <? (g++ min operator)
extern "C" void tAvalanchePackedFrameData_setColour(tAvalanchePackedFrameData* record, float red, float green, float blue)
{
    float zero = 0.0f;
    float one = 1.0f;
    red *= cAvalanche_m_Brightness;
    green *= cAvalanche_m_Brightness;
    blue *= cAvalanche_m_Brightness;
    float cr, cg, cb;
    if (red >= zero) cr = red <? one; else cr = zero;
    if (green >= zero) cg = green <? one; else cg = zero;
    if (blue >= zero) cb = blue <? one; else cb = zero;
    record->m_r = (int)(cr * 255.0f);
    record->m_g = (int)(cg * 255.0f);
    record->m_b = (int)(cb * 255.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", tAvalanchePackedFrameData_setRotation);
#ifdef SKIP_ASM
/**
 * Packs four floats into bytes 6..9 of a 10-byte packed avalanche frame record: the first three times 127, the fourth
 * times 255.
 *
 * tAvalancheNode_calculate is its only caller. The parameter names say colour, but On Tour's layout makes these bytes a
 * rotation (signed x, y, z and an unsigned fourth).
 *
 * @param dest  the record
 * @param red  first value, -1..1
 * @param green  second value, -1..1
 * @param blue  third value, -1..1
 * @param alpha  fourth value, 0..1
 */
extern "C" void tAvalanchePackedFrameData_setRotation(unsigned char* dest, float red, float green, float blue, float alpha)
{
    dest[6] = (int)(red * 127.0f);
    dest[7] = (int)(green * 127.0f);
    dest[8] = (int)(blue * 127.0f);
    dest[9] = (int)(alpha * 255.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", tActiveAvalancheNode__tActiveAvalancheNode);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cDynamicColourEmitter_purge(void* p, int a1);
extern "C" void cDynamicColourEmitter__cDynamicColourEmitter(void* p, int a1);

/**
 * Destroys an active avalanche node: purges and destroys its colour emitter (+0xD0), then frees it when asked
 * to.
 *
 * The emitter is purged with cDynamicColourEmitter_purge(emitter, 1) and destroyed without being freed (flags
 * 2).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void tActiveAvalancheNode__tActiveAvalancheNode(tActiveAvalancheNode* self, int flags)
{
    cDynamicColourEmitter_purge(&self->m_emitter, 1);
    cDynamicColourEmitter__cDynamicColourEmitter(&self->m_emitter, 2);
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", compareAvalancheCollisions);
#ifdef SKIP_ASM
/**
 * Compares two entries by their leading float, for qsort (ascending).
 *
 * tAvalancheNode_calculate passes it to qsort (qsort) for the avalanche's 8-byte collision entries at +0x10.
 *
 * @param lhs  first entry
 * @param rhs  second entry
 * @return -1, 1 or 0 as the first float is smaller, larger or equal
 */
extern "C" int compareAvalancheCollisions(const void* lhs, const void* rhs)
{
    float x = *(const float*)lhs;
    float y = *(const float*)rhs;
    if (x < y) {
        return -1;
    }
    if (y < x) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", compareAvalancheGroundCollisions);
#ifdef SKIP_ASM
/**
 * Compares two entries by their leading unsigned short, for qsort (ascending).
 *
 * tAvalancheNode_calculate passes it to qsort (qsort) for the avalanche's 4-byte ground-collision entries at
 * +0x110 (filled by tAvalanche_addGroundCollision).
 *
 * @param lhs  first entry
 * @param rhs  second entry
 * @return -1, 1 or 0 as the first value is smaller, larger or equal
 */
extern "C" int compareAvalancheGroundCollisions(const void* lhs, const void* rhs)
{
    unsigned short x = *(const unsigned short*)lhs;
    unsigned short y = *(const unsigned short*)rhs;
    if (x < y) {
        return -1;
    }
    return y < x;
}
#endif

