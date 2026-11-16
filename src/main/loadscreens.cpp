#include "common.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "online/commsystem.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("main/loadscreens", cBackgroundMan_loadImages);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* ptr);
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" void* SHAPE_unpack(void* shape);
extern char str_data_textures_backs_ssh[];
extern char str_BackTID[];
extern char str_back[];

struct sRVEntry_17ACC8 {
    short delta;
    short index;
    int (*fn)(void*, void*, const char*, int, int, int);
};

struct sRCtx_17ACC8 {
    char pad[0x10D8];
    sRVEntry_17ACC8* vt;
};

struct sBgFile_17ACC8 {
    int f0;
    int f4;
    int count;
    int fC;
    int f10;
    struct {
        int offset;
        int f4;
    } shapes[1];
};

struct sBgMan_17ACC8 {
    int* images;
    int count;
};

/**
 * Loads the background images from data/textures/backs.ssh and registers each one with the graphics manager.
 *
 * Allocates the texture-ID table ("BackTID") with one entry per shape, unpacks each shape, registers it as a
 * "back" texture (graphics manager vtable slot 46) and frees the unpacked copy and the file afterwards.
 *
 * @return 1 when the file was loaded, 0 if it could not be read
 */
extern "C" int cBackgroundMan_loadImages(sBgMan_17ACC8* self)
{
    sBgFile_17ACC8* file = (sBgFile_17ACC8*)FILE_loadz(str_data_textures_backs_ssh, 0x100);
    if (file == 0) {
        return 0;
    }
    int n = file->count;
    self->count = n;
    self->images = (int*)operator_new_tag(n * 4, str_BackTID, 0, 0);
    for (int i = 0; i < self->count; i++) {
        char* shape = (char*)file + file->shapes[i].offset;
        void* img = SHAPE_unpack(shape);
        self->images[i] = ((sRVEntry_17ACC8*)GrMan->mVtbl)[46].fn((char*)GrMan + ((sRVEntry_17ACC8*)GrMan->mVtbl)[46].delta, img, str_back, 0, 1, -1);
        if (img != shape && img != 0) {
            cMemMan_free(img);
        }
    }
    if (file != 0) {
        cMemMan_free(file);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens", cBackgroundMan_renderImage);
#ifdef SKIP_ASM
extern char t4mat_Identity[];

struct sRS_AE00 {
    int f0;                     // 0x00
    unsigned int pad4a : 2;     // 0x04
    unsigned int mode : 5;
    unsigned int pad4b : 5;
    unsigned int func : 8;
    unsigned int test : 2;
    unsigned int pad4c : 1;
    unsigned int blend : 2;
    unsigned int pad4d : 7;
    unsigned int pad8a : 5;     // 0x08
    unsigned int alpha : 5;
    unsigned int pad8b : 22;
    int fC;                     // 0x0C
    short tex;                  // 0x10
    short pad12;
};

struct sVEnt_AE00 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int);
};

struct sCtx_AE00 {
    char pad[0xE84];
    sRS_AE00* top;              // 0xE84
    char pad_E88[0x10D8 - 0xE88];
    sVEnt_AE00* vt;             // 0x10D8
};

struct sPos_AE00 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVert_AE00 {
    float u, v, q, f0C;         // 0x00
    int r, g, b, a;             // 0x10
    sPos_AE00 pos;              // 0x20
    sVert_AE00() {}
};


struct sBgMan_AE00 {
    int* images;                // 0x0
    int count;                  // 0x4
    int cur;                    // 0x8
};

typedef void (*fnV_AE00)(void*);
typedef void (*fnRect_AE00)(void*, int, int, float, float, float, float, float, float);
typedef void (*fnP_AE00)(void*, void*);

static inline sCtx_AE00* ctx_AE00() { return (sCtx_AE00*)GrMan; }

static inline void call0_AE00(int slot)
{
    sCtx_AE00* g = ctx_AE00();
    sVEnt_AE00* e = &g->vt[slot];
    ((fnV_AE00)e->fn)((char*)g + e->delta);
}

static inline void setAlpha_AE00(sRS_AE00* rs, int v) { rs->alpha = v; }
static inline void setBlend_AE00(sRS_AE00* rs, int v) { rs->blend = v; }
static inline void setTest_AE00(sRS_AE00* rs, int v) { rs->test = v; }
static inline void setFunc_AE00(sRS_AE00* rs, int v) { rs->func = v; }
static inline void setMode_AE00(sRS_AE00* rs, int v) { rs->mode = v; }
static inline int imageOf_AE00(sBgMan_AE00* m) { return m->images[m->cur]; }

static inline void setState_AE00()
{
    setAlpha_AE00(ctx_AE00()->top, 9);
    setBlend_AE00(ctx_AE00()->top, 1);
    setTest_AE00(ctx_AE00()->top, 3);
    setFunc_AE00(ctx_AE00()->top, 0xD);
    setMode_AE00(ctx_AE00()->top, 5);
}

/**
 * Draws the background manager's active image as a full-screen textured quad.
 *
 * Does nothing unless the active index (+0x8) is in [0, count) (count at +0x4). Pushes viewport and view (GrMan slots
 * 21 and 31) and a copy of the draw state, sets the blend bits, the viewport rectangle (slot 26: 0, 0, 640x480, depth
 * 0-1) and t4mat_Identity (slot 34). Draws four vertices from (0, 0) to (640, 480) with texture coordinates 0-1, colour
 * 0x80 in all channels and q = 1 through RenderTriStrip (slot 71), with the image's texture id (images[active], from
 * the array at +0x0) as the state's texture (+0x10). Then pops the draw state, view and viewport (slots 32, 22).
 * cDebugMenuScreen_draw calls it for its popup instead of the menu.
 */
extern "C" void cBackgroundMan_renderImage(sBgMan_AE00* self)
{
    if (self->cur < 0 || self->cur >= self->count) {
        return;
    }
    call0_AE00(21);
    call0_AE00(31);
    sCtx_AE00* graphicsMan = ctx_AE00();
    graphicsMan->top[1] = graphicsMan->top[0];
    graphicsMan->top++;
    setState_AE00();
    {
        sCtx_AE00* graphics = ctx_AE00();
        sVEnt_AE00* vtEntry = &graphics->vt[26];
        ((fnRect_AE00)vtEntry->fn)((char*)graphics + vtEntry->delta, 0, 0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    }
    {
        sCtx_AE00* graphics = ctx_AE00();
        sVEnt_AE00* vtEntry = &graphics->vt[34];
        ((fnP_AE00)vtEntry->fn)((char*)graphics + vtEntry->delta, t4mat_Identity);
    }
    setState_AE00();
    sVert_AE00 verts[4];
    for (int i = 0; i < 4; i++) {
        verts[i].a = 0x80;
        verts[i].r = 0x80;
        verts[i].g = 0x80;
        verts[i].b = 0x80;
        verts[i].q = 1.0f;
    }
    sPos_AE00 cornerPos;
    cornerPos.x = 0.0f;
    cornerPos.y = 0.0f;
    cornerPos.z = 0.0f;
    cornerPos.w = 1.0f;
    verts[0].pos = cornerPos;
    cornerPos.x = 640.0f;
    cornerPos.y = 0.0f;
    cornerPos.z = 0.0f;
    cornerPos.w = 1.0f;
    verts[1].pos = cornerPos;
    cornerPos.x = 0.0f;
    cornerPos.y = 480.0f;
    cornerPos.z = 0.0f;
    cornerPos.w = 1.0f;
    verts[2].pos = cornerPos;
    cornerPos.x = 640.0f;
    cornerPos.y = 480.0f;
    cornerPos.z = 0.0f;
    cornerPos.w = 1.0f;
    verts[3].pos = cornerPos;
    verts[0].u = 0.0f;
    verts[0].v = 0.0f;
    verts[1].u = 1.0f;
    verts[1].v = 0.0f;
    verts[2].u = 0.0f;
    verts[2].v = 1.0f;
    verts[3].u = 1.0f;
    verts[3].v = 1.0f;
    setAlpha_AE00(ctx_AE00()->top, 0x18);
    *(short*)((char*)ctx_AE00()->top + 0x10) = imageOf_AE00(self);
    {
        sCtx_AE00* graphics = ctx_AE00();
        sVEnt_AE00* vtEntry = &graphics->vt[71];
        vtEntry->fn((char*)graphics + vtEntry->delta, 4, verts, 0);
    }
    ctx_AE00()->top--;
    call0_AE00(32);
    call0_AE00(22);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens", cDebugMenuScreen_cDebugMenuScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern char cDebugMenuScreen_vtbl[];
extern char str_mc0[];
extern int D_004A29EC;
extern int D_004A11B8;
extern float D_0049D5D0;
void* cBE_getInterface_B1B0(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void* cBackgroundMan_cBackgroundMan(void*) __asm__("cBackgroundMan_cBackgroundMan__FPv");
extern "C" void cTrackFileMan_scanLocations(void*);
extern "C" void cBENewRaceInterface_setNumberHumans(void* self, int n) __asm__("cBENewRaceInterface_setNumberHumans__FPvi");
extern "C" void cBENewRaceInterface_setGamePath(void* iface, int mode);
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int ev);
extern "C" void func_00266CD8();
extern "C" void func_00261008();
extern "C" void func_0025B688();
extern "C" void func_00255898();
extern "C" void cCommSystem_construct();
extern "C" void func_00255F20_ls(int a, const char* b) __asm__("func_00255F20");
extern "C" void func_0025B650();
extern "C" void func_00260F80();
extern "C" void func_00266CA0();
extern "C" void cDebugMenuScreen_setGamePath(void* self, int idx);
extern "C" int cTrackFileMan_getNextWCBAMStartLocation_ls(void* self, int i) __asm__("cTrackFileMan_getNextWCBAMStartLocation");
extern "C" void cBENewRaceInterface_setDebugLocationFile(void* self, const char* name);
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(void* self, int a1, int a2);
extern "C" void cBENewPlayerInterface_setPlayerCharID(void* self, int a1, int a2);
extern "C" int cBENewPlayerInterface_getControllerMapping(void* self, int a1);
extern "C" void* func_0028B180();
extern "C" void func_00289520(void* snd, void* entry);


struct sNet_B1B0 {
    int f0;
    int f4;
    int f8;
    int fC;
    float f10;
};

struct sG_B1B0 {
    char pad[0x40];
    int f40;
};

struct sVE_B1B0 {
    short delta;
    short index;
    void (*fn)(void*);
};

// Load-screen entry table (see cTrackFileMan_getNextWCBAMStartLocation: `count` at 0, entries' `active` flag).
struct sLSEntry_B1B0 {
    char name[0x80];
    int kind;                    // 0x80
    int f84;
    int active;                  // 0x88
};                               // 0x8C

struct sLSSub_B1B0 {
    int count;
    sLSEntry_B1B0 e[256];        // 0x4
    int f8C04;
    int f8C08;
};                               // 0x8C0C

static inline int lsSubLoaded_B1B0(sLSSub_B1B0* s)
{
    return s->f8C08;
}

static inline char* lsEntryName_B1B0(sLSSub_B1B0* s, int i)
{
    return s->e[i].name;
}

struct sLS_B1B0 {
    void* vt;                    // 0x0
    sLSSub_B1B0 sub;             // 0x4
    char bg[8];                  // 0x8C10
    int pad8C18;
    int f8C1C;
    int f8C20;
    int f8C24;
    int f8C28;
    int f8C2C;
    int f8C30;
    int f8C34;
    int arr[3];                  // 0x8C38
    int f8C44;
};

/**
 * Constructs the debug front-end menu screen and sets up a default single-player race for it.
 *
 * Loads the background images, sets one human, game mode 4 and event 12, clears the network flag (and, with
 * D_004A29EC set, brings up the comm system using "mc0:"), selects the first active track entry as the race
 * location, sets the network defaults (port 3658, latency 1/60 s, not host), picks default riders for both
 * players
 * and starts the menu sound.
 *
 * @return @p self
 */
// Load-screen constructor: sets up the race/player interfaces, picks the active entry and the sound entry.
extern "C" sLS_B1B0* cDebugMenuScreen_cDebugMenuScreen(sLS_B1B0* self)
{
    self->vt = cDebugMenuScreen_vtbl;
    cBackgroundMan_cBackgroundMan(self->bg);
    cTrackFileMan_scanLocations(&self->sub);
    cBackgroundMan_loadImages((sBgMan_17ACC8*)self->bg);
    void* iface0 = cBE_getInterface_B1B0(cBE_getBE(), 0);
    void* iface7 = cBE_getInterface_B1B0(SSXApp->BE, 7);
    self->f8C24 = 0;
    *(int*)cBE_getBE() = self->f8C24;
    self->f8C24 = 0;
    cBENewRaceInterface_setNumberHumans(iface0, 1);
    for (int i = 0; i < 3; i++) {
        self->arr[i] = 4;
    }
    cBENewRaceInterface_setGamePath(iface0, 0);
    cBENewRaceInterface_setGameMode(iface0, 4);
    cBENewRaceInterface_setGameEvent(iface0, 0xC);
    self->f8C20 = 0;
    cBENetworkInterface_networkOpt.bIsNetworkGame = 0;
    if (D_004A29EC != 0) {
        func_00266CD8();
        func_00261008();
        func_0025B688();
        func_00255898();
        cCommSystem_construct();
        func_00255F20_ls((int)cCommSystem_mCommSystem, str_mc0);
        func_0025B650();
        func_00260F80();
        func_00266CA0();
    }
    if (lsSubLoaded_B1B0(&self->sub) != 0) {
        if (self->sub.e[self->f8C20].active == 0) {
            self->f8C20 = cTrackFileMan_getNextWCBAMStartLocation_ls(&self->sub, self->f8C20);
        }
    } else {
        cDebugMenuScreen_setGamePath(self, 1);
    }
    int idx = self->f8C20;
    int kind = self->sub.e[idx].kind;
    D_004A11B8 = 1;
    cBENewRaceInterface_newRace.iLocationIdx = kind;
    if (self->sub.e[self->f8C20].f84 == 0) {
        D_004A11B8 = 0;
    }
    if (kind == 22) {
        cBENewRaceInterface_setDebugLocationFile(iface0, lsEntryName_B1B0(&self->sub, self->f8C20));
    }
    self->f8C1C = 4;
    cBENetworkInterface_networkOpt.fLatency = 0.016666668f;
    cBENetworkInterface_networkOpt.iPort = 3658;
    cBENetworkInterface_networkOpt.bIsHost = 0;
    sVE_B1B0* vt7 = *(sVE_B1B0**)((char*)iface7 + 0xC);
    vt7[1].fn((char*)iface7 + vt7[1].delta);
    self->f8C44 = 0;
    self->f8C34 = 0;
    self->f8C30 = -1;
    void* iface1 = cBE_getInterface_B1B0(cBE_getBE(), 1);
    cBENewPlayerInterface_setPlayerCtrlID(iface1, 0, 0);
    cBENewPlayerInterface_setPlayerCtrlID(iface1, 1, 1);
    self->f8C28 = 2;
    cBENewPlayerInterface_setPlayerCharID(iface1, 0, 2);
    self->f8C2C = cBENewPlayerInterface_getControllerMapping(iface1, 0);
    void* snd = func_0028B180();
    func_00289520(snd, (char*)func_0028B180() + (self->f8C34 * 0x14 + 0x60A4));
    sVE_B1B0* vt1 = *(sVE_B1B0**)((char*)iface1 + 0xC);
    vt1[1].fn((char*)iface1 + vt1[1].delta);
    sVE_B1B0* vt0 = *(sVE_B1B0**)((char*)iface0 + 0xC);
    vt0[1].fn((char*)iface0 + vt0[1].delta);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens", cDebugMenuScreen_update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern int D_004A11B8;

void* cBE_getInterface_B1B0(void* be, int kind) __asm__("cBE_getInterface__Fv");

extern "C" void* cBE_getBE();

extern "C" void cDebugMenuScreen_setGamePath(void* self, int idx);

extern "C" void cBENewRaceInterface_setDebugLocationFile(void* self, const char* name);

extern "C" void cBENewPlayerInterface_setPlayerCharID(void* self, int a1, int a2);

extern "C" void* func_0028B180();

extern "C" void func_00289520(void* snd, void* entry);


struct cAppMan;
int cInput_getState(void* pad, int button);
extern "C" float cInput_getValue(void* pad, int axis);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreFELoadScreen_cPreFELoadScreen(void* self);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void func_0017D0A8(sLS_B1B0* self);
extern "C" int cDebugMenuScreen_getMaxAIRiders(sLS_B1B0* self, int quick, int mode, int event);
extern "C" int cTrackFileMan_getPrevWCBAMStartLocation(sLSSub_B1B0* s, int i);
extern "C" int cTrackFileMan_legalTrackMode(sLSSub_B1B0* s, int i, int mode);
extern "C" int cTrackFileMan_getNextLocation(sLSSub_B1B0* s, int i, int mode);
extern "C" int cTrackFileMan_getPrevLocation(sLSSub_B1B0* s, int i, int mode);
extern "C" int cTrackFileMan_getNextWCBAMStartLocation_b4(sLSSub_B1B0* s, int i) __asm__("cTrackFileMan_getNextWCBAMStartLocation");
extern "C" int cBENewRaceInterface_setGameMode_b4(void* self, int mode) __asm__("cBENewRaceInterface_setGameMode");
extern "C" int cBENewRaceInterface_setGameEvent_b4(void* self, int ev) __asm__("cBENewRaceInterface_setGameEvent");
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);
extern "C" int func_00145C38(void* self, int a1);
extern "C" void func_00145CB0(void* self, int a1, int a2);
extern "C" void cBENewPlayerInterface_setFirstTimeOnTrack(void* self, int a1, int a2, int a3);
extern "C" void func_00147658(void* self, int a1, int a2);
extern "C" void func_0017AC98(void* bg, int idx);
extern "C" void func_00256088();
extern "C" void func_002560B0();
extern "C" void func_00255FA0(void* comm);
extern "C" void func_00255F50(void* comm, int a1);
extern "C" void func_00255A20(void* comm);
extern "C" int SYNCTASK_run(int);
extern "C" void THREAD_yieldticks(int);
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
extern char str_cPreFELoadScreen[];
extern char str_cPreGameLoadScreen[];
extern signed char D_00535C11[];
extern int cCommSystem_mNumNetConn;


struct sTime_B4E0 {
    char pad[0x10];
    int f10;
    int f14;
};

struct sApp_B4E0 {
    char pad[0x60];
    int f60;                     // 0x60
    char pad64[0x78 - 0x64];
    int* be;                     // 0x78
    char pad7C[0x4];
    sTime_B4E0* f80;             // 0x80
    char pad84[0xB0 - 0x84];
    void* input;                 // 0xB0
};
extern sApp_B4E0* D_004A28A8_b4 __asm__("SSXApp");


struct sVE_B4E0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sGfxVE_B4E0 {
    short delta;
    short index;
    void* fn;
};

struct sGfx_B4E0 {
    char pad[0x10D8];
    sGfxVE_B4E0* vt;
};

static inline void update_B4E0(void* iface)
{
    sVE_B4E0* vt = *(sVE_B4E0**)((char*)iface + 0xC);
    vt[1].fn((char*)iface + vt[1].delta);
}

static inline bool commIdle_B4E0(cCommSystem* commSystem)
{
    return commSystem->mNetConnState == 1;
}

static inline bool commBusy_B4E0(cCommSystem* commSystem)
{
    return commSystem->mNetConnState == 3 || commSystem->mNetConnState == 4;
}

static inline bool netOn_B4E0(int mode)
{
    return mode != 0 && mode < 3;
}

static inline int numSongs_B4E0()
{
    return *(int*)((char*)func_0028B180() + 0x6234);
}

static inline void playSong_B4E0(sLS_B1B0* self)
{
    void* snd = func_0028B180();
    func_00289520(snd, (char*)func_0028B180() + (self->f8C34 * 0x14 + 0x60A4));
}

static inline int bgCount_B4E0(sBgMan_17ACC8* bg)
{
    return bg->count;
}

/**
 * Runs one frame of the debug start menu: moves between its 15 items, changes the selected one and starts the game.
 *
 * Input state 0x65 saves the choices (func_0017D0A8) and starts: item 14 (On Tour kRealFEItem) sets the app's word
 * +0x60 and moves to a cPreFELoadScreen module (cPreFELoadScreen_cPreFELoadScreen), any other item to a cPreGameLoadScreen. Up / down
 * (cInput_getValue(.., 0x3D) 0 / 1, only while no background is previewed, f8C30 == -1) step the item, skipping 12
 * (network) without a comm system and 13 without a network connection. Left / right (2 / 3) change the item: 0 game
 * path (3 values; path 0 needs the track list and a WCBAM start location), 1 game mode (7), 2 game event (14), 3
 * the player's first-time-on-track flag, 4 location (cTrackFileMan next / previous), 5 character (10), 6 a player
 * flag (func_00147658), 7 a one-value option stored in the BE's first word (always 0), 8 the AI rider count up to
 * getMaxAIRiders, 9 the background preview, 10 GrMan slot 8 toggled from slot 9, 11 the song, 12 the network mode
 * 0-4 (sets bIsNetworkGame / bIsHost and starts or stops the comm system), 13 (event 6) copies a word of the app's
 * +0x80 object. Mode, event, location and AI count are re-validated after each change.
 */
extern "C" void cDebugMenuScreen_update(sLS_B1B0* self)
{
    if (cInput_getState(D_004A28A8_b4->input, 101)) {
        func_0017D0A8(self);
        if (self->f8C1C == 14) {
            D_004A28A8_b4->f60 = 1;
            cAppMan_setNextModule((cAppMan*)D_004A28A8_b4, (unsigned int)cPreFELoadScreen_cPreFELoadScreen(cMemMan_alloc(8, str_cPreFELoadScreen, 0x100, 0)));
        } else {
            cAppMan_setNextModule((cAppMan*)D_004A28A8_b4, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        }
    }
    int dir = (int)cInput_getValue(D_004A28A8_b4->input, 61);
    if (dir == 0 && self->f8C30 == -1) {
        self->f8C1C = (self->f8C1C + 15 - 1) % 15;
        if (self->f8C1C == 13) {
            if (cCommSystem_mCommSystem == 0) {
                self->f8C1C = (self->f8C1C + 15 - 2) % 15;
            } else if (cCommSystem_mNumNetConn <= 0) {
                self->f8C1C = (self->f8C1C + 15 - 1) % 15;
            }
        }
        return;
    }
    if (dir == 1 && self->f8C30 == -1) {
        self->f8C1C = (self->f8C1C + 1) % 15;
        if (self->f8C1C == 12 && cCommSystem_mCommSystem == 0) {
            self->f8C1C = (self->f8C1C + 15 + 2) % 15;
        }
        if (self->f8C1C == 13) {
            if (cCommSystem_mCommSystem == 0 || cCommSystem_mNumNetConn <= 0) {
                self->f8C1C = (self->f8C1C + 15 + 1) % 15;
            }
        }
        return;
    }
    switch (self->f8C1C) {
    case 0: {
        void* race = cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        int quick = beRaceDB->cGamePath;
        if (dir == 3) {
            quick = (quick + 1) % 3;
            if (quick == 0) {
                if (lsSubLoaded_B1B0(&self->sub) != 0) {
                    if (self->sub.e[self->f8C20].active == 0) {
                        self->f8C20 = cTrackFileMan_getNextWCBAMStartLocation_b4(&self->sub, self->f8C20);
                    }
                    int kind = self->sub.e[self->f8C20].kind;
                    D_004A11B8 = 1;
                    beRaceDB->iLocationIdx = kind;
                } else {
                    quick = (quick + 1) % 3;
                }
            }
            cDebugMenuScreen_setGamePath(self, quick);
        } else if (dir == 2) {
            quick = (quick + 2) % 3;
            if (quick == 0) {
                if (lsSubLoaded_B1B0(&self->sub) != 0) {
                    if (self->sub.e[self->f8C20].active == 0) {
                        self->f8C20 = cTrackFileMan_getNextWCBAMStartLocation_b4(&self->sub, self->f8C20);
                    }
                    int kind = self->sub.e[self->f8C20].kind;
                    D_004A11B8 = 1;
                    beRaceDB->iLocationIdx = kind;
                } else {
                    quick = (quick + 2) % 3;
                }
            }
            cDebugMenuScreen_setGamePath(self, quick);
        }
        int changed = 0;
        int mode = cBENewRaceInterface_newRace.cGameMode;
        do {
            while (!cBENewRaceInterface_setGameMode_b4(race, mode)) {
                mode = (mode + 1) % 7;
            }
            if (!cTrackFileMan_legalTrackMode(&self->sub, self->f8C20, mode)) {
                changed = 1;
                self->f8C20 = cTrackFileMan_getNextLocation(&self->sub, self->f8C20, mode);
            }
        } while (self->f8C20 < 0);
        if (changed) {
            int kind = self->sub.e[self->f8C20].kind;
            D_004A11B8 = 1;
            cBENewRaceInterface_newRace.iLocationIdx = kind;
            if (self->sub.e[self->f8C20].f84 == 0) {
                D_004A11B8 = 0;
            }
            if (kind == 22) {
                cBENewRaceInterface_setDebugLocationFile(race, lsEntryName_B1B0(&self->sub, self->f8C20));
            }
        }
        update_B4E0(race);
        int ev = cBENewRaceInterface_newRace.cGameEvent;
        if (!cBENewRaceInterface_setGameEvent_b4(race, ev)) {
            ev = (ev + 1) % 14;
            while (!cBENewRaceInterface_setGameEvent_b4(race, ev)) {
                ev = (ev + 1) % 14;
            }
            update_B4E0(race);
        }
        int numAI = cBENewRaceInterface_newRace.iNumberAI;
        int maxAI = cDebugMenuScreen_getMaxAIRiders(self, cBENewRaceInterface_newRace.cGamePath, cBENewRaceInterface_newRace.cGameMode, cBENewRaceInterface_newRace.cGameEvent);
        if (maxAI < numAI) {
            cBENewRaceInterface_setNumberAI(race, maxAI);
            update_B4E0(race);
        }
        break;
    }
    case 1: {
        void* race = cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        if (beRaceDB->cGamePath == 0) {
            cBENewRaceInterface_setGameMode_b4(race, 4);
            cBENewRaceInterface_setNumberAI(race, 0);
            update_B4E0(race);
            break;
        }
        int mode = beRaceDB->cGameMode;
        if (dir != 2 && dir != 3) {
            return;
        }
        int changed = 0;
        do {
            do {
                if (dir == 3) {
                    mode = (mode + 1) % 7;
                } else if (dir == 2) {
                    mode = (mode + 6) % 7;
                }
            } while (!cBENewRaceInterface_setGameMode_b4(race, mode));
            if (!cTrackFileMan_legalTrackMode(&self->sub, self->f8C20, mode)) {
                changed = 1;
                self->f8C20 = cTrackFileMan_getNextLocation(&self->sub, self->f8C20, mode);
            }
        } while (self->f8C20 < 0);
        if (changed) {
            int kind = self->sub.e[self->f8C20].kind;
            D_004A11B8 = 1;
            cBENewRaceInterface_newRace.iLocationIdx = kind;
            if (self->sub.e[self->f8C20].f84 == 0) {
                D_004A11B8 = 0;
            }
            if (kind == 22) {
                cBENewRaceInterface_setDebugLocationFile(race, lsEntryName_B1B0(&self->sub, self->f8C20));
            }
        }
        update_B4E0(race);
        int ev = cBENewRaceInterface_newRace.cGameEvent;
        if (!cBENewRaceInterface_setGameEvent_b4(race, ev)) {
            ev = (ev + 1) % 14;
            while (!cBENewRaceInterface_setGameEvent_b4(race, ev)) {
                ev = (ev + 1) % 14;
            }
            update_B4E0(race);
        }
        int numAI = cBENewRaceInterface_newRace.iNumberAI;
        int maxAI = cDebugMenuScreen_getMaxAIRiders(self, cBENewRaceInterface_newRace.cGamePath, cBENewRaceInterface_newRace.cGameMode, cBENewRaceInterface_newRace.cGameEvent);
        if (maxAI < numAI) {
            cBENewRaceInterface_setNumberAI(race, maxAI);
            update_B4E0(race);
        }
        break;
    }
    case 2: {
        void* race = cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        int ev = beRaceDB->cGameEvent;
        if (beRaceDB->cGamePath == 0) {
            cBENewRaceInterface_setGameMode_b4(race, 4);
            cBENewRaceInterface_setNumberAI(race, 0);
        } else if (dir == 2 || dir == 3) {
            do {
                if (dir == 3) {
                    ev = (ev + 1) % 14;
                } else if (dir == 2) {
                    ev = (ev + 13) % 14;
                }
            } while (!cBENewRaceInterface_setGameEvent_b4(race, ev));
        }
        update_B4E0(race);
        int numAI = cBENewRaceInterface_newRace.iNumberAI;
        int maxAI = cDebugMenuScreen_getMaxAIRiders(self, cBENewRaceInterface_newRace.cGamePath, cBENewRaceInterface_newRace.cGameMode, cBENewRaceInterface_newRace.cGameEvent);
        if (maxAI < numAI) {
            cBENewRaceInterface_setNumberAI(race, maxAI);
            update_B4E0(race);
        }
        break;
    }
    case 3: {
        cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        void* player = cBE_getInterface_B1B0(D_004A28A8_b4->be, 1);
        int firstTimeOnTrack = func_00145C38(player, 0);
        if (D_00535C11[0] != 0) {
            break;
        }
        if (dir != 2 && dir != 3) {
            return;
        }
        if (dir == 3) {
            firstTimeOnTrack = 1;
        } else if (dir == 2) {
            firstTimeOnTrack = 0;
        }
        func_00145CB0(player, 0, firstTimeOnTrack);
        cBENewPlayerInterface_setFirstTimeOnTrack(player, firstTimeOnTrack, 0, 14);
        update_B4E0(player);
        break;
    }
    case 4: {
        void* race = cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        if (dir != 2 && dir != 3) {
            return;
        }
        cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
        if (beRaceDB->cGamePath == 0) {
            if (dir == 3) {
                self->f8C20 = cTrackFileMan_getNextWCBAMStartLocation_b4(&self->sub, self->f8C20);
            } else if (dir == 2) {
                self->f8C20 = cTrackFileMan_getPrevWCBAMStartLocation(&self->sub, self->f8C20);
            }
        } else {
            int mode = beRaceDB->cGameMode;
            do {
                if (dir == 3) {
                    self->f8C20 = cTrackFileMan_getNextLocation(&self->sub, self->f8C20, cBENewRaceInterface_newRace.cGameMode);
                } else {
                    self->f8C20 = cTrackFileMan_getPrevLocation(&self->sub, self->f8C20, cBENewRaceInterface_newRace.cGameMode);
                }
                if (self->f8C20 < 0) {
                    do {
                        mode = (mode + 1) % 7;
                    } while (!cBENewRaceInterface_setGameMode_b4(race, mode));
                }
            } while (self->f8C20 < 0);
            update_B4E0(race);
        }
        sLSSub_B1B0* sub = &self->sub;
        int kind = sub->e[self->f8C20].kind;
        D_004A11B8 = 1;
        cBENewRaceInterface_newRace.iLocationIdx = kind;
        if (sub->e[self->f8C20].f84 == 0) {
            D_004A11B8 = 0;
        }
        if (kind == 22) {
            cBENewRaceInterface_setDebugLocationFile(race, lsEntryName_B1B0(sub, self->f8C20));
        }
        update_B4E0(race);
        break;
    }
    case 7:
        // Wraps over a range of one value: both arms fold to 0 (the binary keeps both compares and one store).
        if (dir == 3) {
            self->f8C24 = (self->f8C24 + 1) % 1;
        } else if (dir == 2) {
            self->f8C24 = (self->f8C24 + 1 - 1) % 1;
        }
        *D_004A28A8_b4->be = self->f8C24;
        break;
    case 5: {
        void* player = cBE_getInterface_B1B0(cBE_getBE(), 1);
        if (dir == 3) {
            self->f8C28 = (self->f8C28 + 1) % 10;
        } else if (dir == 2) {
            self->f8C28 = (self->f8C28 + 9) % 10;
        }
        cBENewPlayerInterface_setPlayerCharID(player, 0, self->f8C28);
        update_B4E0(player);
        break;
    }
    case 6: {
        if (dir == 2 || dir == 3) {
            self->f8C2C = !self->f8C2C;
        }
        void* player = cBE_getInterface_B1B0(cBE_getBE(), 1);
        func_00147658(player, 0, self->f8C2C);
        update_B4E0(player);
        break;
    }
    case 8: {
        void* race = cBE_getInterface_B1B0(D_004A28A8_b4->be, 0);
        int numAI = cBENewRaceInterface_newRace.iNumberAI;
        int maxAI = cDebugMenuScreen_getMaxAIRiders(self, cBENewRaceInterface_newRace.cGamePath, cBENewRaceInterface_newRace.cGameMode, cBENewRaceInterface_newRace.cGameEvent);
        if (maxAI < numAI) {
            numAI = maxAI;
        }
        for (;;) {
            if (dir == 3) {
                numAI = (cBENewRaceInterface_newRace.iNumberAI + 1) % (maxAI + 1);
            } else if (dir == 2) {
                numAI = (cBENewRaceInterface_newRace.iNumberAI + maxAI) % (maxAI + 1);
            }
            if (cBENewRaceInterface_setNumberAI(race, numAI)) {
                break;
            }
            if (dir != 2 && dir != 3) {
                break;
            }
        }
        update_B4E0(race);
        break;
    }
    case 9:
        if (dir == 3) {
            self->f8C30 = (self->f8C30 + 2) % (bgCount_B4E0((sBgMan_17ACC8*)self->bg) + 1) - 1;
        } else if (dir == 2) {
            self->f8C30 = (self->f8C30 + bgCount_B4E0((sBgMan_17ACC8*)self->bg) + 1) % (bgCount_B4E0((sBgMan_17ACC8*)self->bg) + 1) - 1;
        }
        func_0017AC98(self->bg, self->f8C30);
        break;
    case 10: {
        if (dir != 2 && dir != 3) {
            return;
        }
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        sGfxVE_B4E0* vt = (sGfxVE_B4E0*)ps2GraphicsMan->mVtbl;
        ((void (*)(void*, int))vt[8].fn)((char*)ps2GraphicsMan + vt[8].delta, !((bool (*)(void*))vt[9].fn)((char*)ps2GraphicsMan + vt[9].delta));
        break;
    }
    case 11:
        if (dir == 3) {
            self->f8C34 = (self->f8C34 + 1) % numSongs_B4E0();
            playSong_B4E0(self);
        } else if (dir == 2) {
            self->f8C34 = (self->f8C34 + numSongs_B4E0() - 1) % numSongs_B4E0();
            playSong_B4E0(self);
        }
        break;
    case 12: {
        if (dir == 6) {
            if (self->f8C44 == 3) {
                func_00256088();
            }
            if (self->f8C44 == 4) {
                func_002560B0();
            }
        }
        if (cCommSystem_mCommSystem == 0) {
            return;
        }
        if (dir == 3) {
            self->f8C44++;
            if (cCommSystem_mNumNetConn <= 0 && (self->f8C44 == 1 || self->f8C44 == 2)) {
                self->f8C44 = 3;
            }
        } else if (dir == 2) {
            self->f8C44--;
            if (cCommSystem_mNumNetConn <= 0 && (self->f8C44 == 1 || self->f8C44 == 2)) {
                self->f8C44 = 0;
            }
        }
        if (self->f8C44 > 4) {
            self->f8C44 = 0;
        }
        if (self->f8C44 < 0) {
            self->f8C44 = 4;
        }
        void* net = cBE_getInterface_B1B0(D_004A28A8_b4->be, 7);
        cBENetworkInterface_networkOpt.bIsNetworkGame = netOn_B4E0(self->f8C44);
        cBENetworkInterface_networkOpt.bIsHost = self->f8C44 == 1;
        update_B4E0(net);
        if (self->f8C44 != 0) {
            if (self->f8C44 != 3) {
                if (commIdle_B4E0(cCommSystem_mCommSystem)) {
                    func_00255F50(cCommSystem_mCommSystem, 0);
                    while (commBusy_B4E0(cCommSystem_mCommSystem)) {
                        func_00255A20(cCommSystem_mCommSystem);
                        SYNCTASK_run(0);
                        THREAD_yieldticks(1);
                    }
                }
            }
        } else {
            if (!commIdle_B4E0(cCommSystem_mCommSystem)) {
                func_00255FA0(cCommSystem_mCommSystem);
            }
        }
        break;
    }
    case 13:
        if (cCommSystem_mCommSystem == 0) {
            return;
        }
        if (cCommSystem_mNumNetConn <= 0) {
            return;
        }
        if (dir == 6) {
            D_004A28A8_b4->f80->f14 = D_004A28A8_b4->f80->f10;
        }
        break;
    case 14:
        break;
    }
    if (dir == 2 || dir == 3) {
        void* player = cBE_getInterface_B1B0(cBE_getBE(), 1);
        self->f8C24 = *D_004A28A8_b4->be;
        self->f8C28 = cBENewPlayerInterface_getPlayerCharID(player, 0);
    }
}
#endif

