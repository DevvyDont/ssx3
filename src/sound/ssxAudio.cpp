#include "vtable.h"
#include "common.h"
#include "sound/bankmanager.h"
#include "bx/math3d.h"
#include "bx/appman.h"
#include "sound/icepick/worldtriggermanager.h"
#include "sound/asyncsys.h"
#include "sound/bankmonitor.h"
#include "worldpainter/worldpainterquery.h"
#include "ai/rider.h"
#include "be/beintnewrace.h"
#include "sound/ssxaudio.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/player.h"

//100%
INCLUDE_ASM("sound/ssxAudio", SSXAUDIO_Init);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" char* cSSXAudio_cSSXAudio(cSSXAudio* mem, int a1, int a2, int a3, int a4);
void cMonitor_MONITOR_BeginMonitoring(void*);
extern const char str_SSXAudioSystem[];

/**
 * Creates the game's audio system (gSSXAudio) on first use, then calls cMonitor_MONITOR_BeginMonitoring (unknown) on its bank
 * monitor.
 *
 * The cSSXAudio ("SSXAudioSystem", 0x7780 bytes) is built for 28 banks and 3 streams.
 *
 * @param mode  audio mode, passed to the cSSXAudio constructor (On Tour mode, an eSOUNDDEF_MODE)
 */
extern "C" void SSXAUDIO_Init(int mode)
{
    if (gSSXAudio == 0) {
        gSSXAudio = (cSSXAudio*)(cSSXAudio_cSSXAudio((cSSXAudio*)cMemMan_alloc(30592, str_SSXAudioSystem, 0x80000400, 0), 1, 0x1C, 3, mode));
    }
    cMonitor_MONITOR_BeginMonitoring(*((char**)gSSXAudio->MusicSys.mpVStreamSys) + 0x1D8);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", SSXAUDIO_DeInit);
#ifdef SKIP_ASM
class SSXAUDIO_DeInit_cObj {
public:
    char data[0x1D4];
    virtual void v01(int flags);
};


/**
 * Shuts the game audio down: deletes the music system's stream system, then clears gSSXAudio.
 *
 * The stream system (MusicSys.mpVStreamSys) is deleted through its virtual destructor with flags 3. Nothing is
 * deleted when gSSXAudio is already null.
 */
extern "C" void SSXAUDIO_DeInit()
{
    if (gSSXAudio != 0) {
        (*((SSXAUDIO_DeInit_cObj**)gSSXAudio->MusicSys.mpVStreamSys))->v01(3);
    }
    gSSXAudio = 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_cSSXAudio);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cSoundSys_cSoundSys_4C68(void* self, int heap) __asm__("cSoundSys_cSoundSys");
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cStreamSys_cStreamSys(void* self, int inchrg, int banks, int count, int heap);
extern "C" char* cController_cController(char* self, int embedded, int a2, int a3, int heap);
extern "C" void* operator_new__FUi(unsigned int size, const char* tag, int flags, int d);
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float).
void cSSXAudio_SetCurrVolume_4C68(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");
extern "C" void cSSXAudio_SetPreset(cSSXAudio* self, int mode);
extern "C" void func_00289470(cSSXAudio* self);
extern "C" void func_00289520(cSSXAudio* self, const char* name);
void cSSXAudio_LoadMixes_4C68(void* self) __asm__("cSSXAudio_LoadMixes");
extern "C" void cSSXAudio_InitAttachedSounds(void* self);
extern "C" void SNDSYS_adduserdataclient(void (*cb)(void*));
extern "C" void cSSXAudio_HandleUserChunk(void* msg);
extern "C" void cSSXAudio_ResetLift(void* self);
extern "C" void func_002A4590(void* self, int flag);
extern "C" void func_0029ED90(char* self, int restart);
extern "C" void SPCH_Init(void* self);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(cSSXAudio* self, int a1, int a2);
extern "C" void func_002B0E60(void* self, int a, int b, char* name, int c, int d, int e);
extern "C" void func_0028F700(void* self);
extern char str_CircFIFOBuff[];
extern char str_CHAR[];
extern void* D_00483688[];
extern void* cSSXAudio_vtbl[];
extern void* D_004836D8[];

struct sSaVEntry4C68 {
    short delta;
    short index;
    void* fn;
};
struct sSaVtbl4_4C68 {
    sSaVEntry4C68 e[4];
} __attribute__((aligned(8)));
struct sSaVtbl8_4C68 {
    sSaVEntry4C68 e[8];
} __attribute__((aligned(8)));
struct sSaVtbl7_4C68 {
    sSaVEntry4C68 e[7];
} __attribute__((aligned(8)));
extern const sSaVtbl4_4C68 D_00483708_4C68 __asm__("D_00483708");
extern const sSaVtbl8_4C68 D_00483648_4C68 __asm__("D_00483648");
extern const sSaVtbl7_4C68 D_00483728_4C68 __asm__("D_00483728");

// Member arrays whose element class has an empty constructor (only the vec_new loop counter remains;
// element counts 30 and 3 come from the binary, element sizes and offsets are unknown and only placeholders).
struct sEmpty34_4C68 {
    void* operator new[](unsigned int, void* p) { return p; }
    char pad[0x34];
    sEmpty34_4C68() {}
};
struct sEmpty4_4C68 {
    void* operator new[](unsigned int, void* p) { return p; }
    char pad[4];
    sEmpty4_4C68() {}
};

// Member arrays of cSSXAudio touched by the constructor; m5560 and m60A4 are subobjects whose real extents are unknown.
struct sAudioArrs4C68 {
    char pad0[0x5560];
    char m5560[0x579C - 0x5560];
    int m579C[23];
    char pad57F8[0x6030 - 0x57F8];
    int m6030[3];
    char pad603C[0x60A4 - 0x603C];
    char m60A4[0x6394 - 0x60A4];
    float m6394[11];
    char pad63C0[0x63E4 - 0x63C0];
    float m63E4[11];
    char pad6410[0x6C70 - 0x6410];
    int m6C70[2];
    int m6C78[2];
};
#define A_4C68 ((sAudioArrs4C68*)self)
#define S_4C68 ((char*)self)
#define BM_4C68 (**(char***)(S_4C68 + 0x118))

/**
 * Constructs the audio system: its sound, bank and stream bases, its mixer defaults and its speech system.
 *
 * When @p inchrg is set it also builds the virtual bases (cSoundSys at +0x6C90, cBankManager at +0x6C98,
 * cStreamSys at +0x7768). It then patches the base vtables (the g++ 2.95 this-adjust fix-ups when not in
 * charge) and sets the defaults. It allocates an 800-byte "CircFIFOBuff" and 30 attached-sound slots, sets
 * the master volume to 10/11, and loads the mixes. It also inits the attached sounds, resets the lift sounds
 * and runs SPCH_Init() on the speech system.
 *
 * @param inchrg  nonzero when this object is complete and must construct its virtual bases
 * @param maxBanks      the number of banks, passed to the bank manager and stream system (On Tour MaxBanks)
 * @param maxStreams      the number of streams, passed to the stream system (On Tour MaxStreams)
 * @param heap    the audio mode (On Tour audioMode), stored at +0x62B0
 * @return self
 */
// PORT: g++ 2.95 constructor of a class with virtual bases (stack vtable this-adjust fix-ups written out by hand).
extern "C" char* cSSXAudio_cSSXAudio(cSSXAudio* self, int inchrg, int maxBanks, int maxStreams, int heap)
{
    if (inchrg) {
        *(char**)&self->SpeechSys = S_4C68 + 0x7768;
        self->mpStreamVBankManager = (cBankManager*)(S_4C68 + 0x6C98);
        *(char**)S_4C68 = S_4C68 + 0x6C98;
        *(char**)&self->BankMonitor = S_4C68 + 0x6C90;
        *(char**)&self->MusicSys = S_4C68 + 0x7768;
        cSoundSys_cSoundSys_4C68(S_4C68 + 0x6C90, heap);
        cBankManager_cBankManager(self->mpStreamVBankManager, 0, maxBanks, heap);
        cStreamSys_cStreamSys(*(void**)&self->SpeechSys, 0, maxBanks, maxStreams, heap);
    }
    cController_cController(S_4C68, 0, maxBanks, maxStreams, heap);
    *(const sSaVtbl4_4C68**)(*(char**)(BM_4C68 + 0x1D8) + 4) = &D_00483708_4C68;
    *(const sSaVtbl8_4C68**)(BM_4C68 + 0xAB0) = &D_00483648_4C68;
    *(const sSaVtbl7_4C68**)(BM_4C68 + 0x1D4) = &D_00483728_4C68;
    if (inchrg == 0) {
        sSaVtbl4_4C68 vtbl1 = D_00483708_4C68;
        *(sSaVtbl4_4C68**)(*(char**)(BM_4C68 + 0x1D8) + 4) = &vtbl1;
        char* base1 = *(char**)(BM_4C68 + 0x1D8) - 0x6C90;
        int delta1 = S_4C68 - base1;
        vtbl1.e[1].delta = D_00483708_4C68.e[1].delta + delta1;
        vtbl1.e[2].delta = D_00483708_4C68.e[2].delta + delta1;
        sSaVtbl8_4C68 vtbl2 = D_00483648_4C68;
        *(sSaVtbl8_4C68**)(BM_4C68 + 0xAB0) = &vtbl2;
        char* base2 = BM_4C68 - 0x6C98;
        int delta2 = S_4C68 - base2;
        vtbl2.e[1].delta = D_00483648_4C68.e[1].delta + delta2;
        vtbl2.e[2].delta = D_00483648_4C68.e[2].delta + delta2;
        vtbl2.e[3].delta = D_00483648_4C68.e[3].delta + delta2;
        vtbl2.e[5].delta = D_00483648_4C68.e[5].delta + delta2;
        vtbl2.e[6].delta = D_00483648_4C68.e[6].delta + delta2;
        sSaVtbl7_4C68 vtbl3 = D_00483728_4C68;
        *(sSaVtbl7_4C68**)(BM_4C68 + 0x1D4) = &vtbl3;
        vtbl3.e[1].delta = D_00483728_4C68.e[1].delta + delta2;
    }
    self->MusicSys.mVtbl = D_00483688;
    self->SpeechSys.mVtbl = D_004836D8;
    self->mVtbl = cSSXAudio_vtbl;
    self->unk_5728 = 0;
    self->unk_572C = -1;
    self->unk_5730 = 1.0f;
    self->unk_5734 = 0;
    self->unk_5740 = 0;
    self->unk_5744 = 0;
    self->unk_574C = 0;
    self->unk_5750 = 0;
    self->unk_5754 = 0;
    self->unk_5758 = 0;
    self->unk_5764 = -1;
    self->unk_5768 = 0;
    self->unk_576C = 0;
    self->unk_5770 = 0;
    self->unk_5774 = 0;
    self->unk_577C = 0;
    self->unk_5780 = 1;
    self->unk_5784 = 0;
    self->unk_5788 = 0;
    self->unk_578C = 0;
    self->unk_5790 = 1;
    self->unk_5794 = 1;
    self->unk_5798 = -1;
    self->unk_5818 = 0;
    self->unk_581C = -1;
    self->unk_5820 = 0;
    self->unk_5828 = 0;
    self->mPlayHistory.ArraySize = 200;
    self->mPlayHistory.Head = 0;
    self->mPlayHistory.Tail = 0;
    self->mPlayHistory.Array = (int*)operator_new__FUi(800, str_CircFIFOBuff, 0, 0);
    new (S_4C68 + 0x5990) sEmpty34_4C68[30];
    self->mLastPatchRID = 0xFFFFFFFF;
    self->mToneHandle = -1;
    self->mInBigChallenge = 0;
    self->mBigChallengeStarting = 0;
    self->mUberSpinHandle = -1;
    new (S_4C68 + 0x5FF4) sEmpty4_4C68[3];
    self->mAvalancheHitIndex = -1;
    self->mAvalancheInstanceCount = 0;
    self->unk_608C = 0;
    self->unk_6090 = 0;
    *(long*)&self->mRadioPlayList = 0;
    self->unk_6254 = 0;
    self->unk_627C = 1;
    self->unk_6284 = 23;
    self->unk_6294 = 0;
    self->unk_6298 = 0;
    self->unk_62A4 = -1;
    self->unk_62A8 = -1;
    self->mArcadeSFX = 0;
    self->unk_62B8 = 0;
    self->mDucking = 0;
    self->mDuckCurValue = 1.0f;
    self->mDuckStream = 3;
    self->mDuckScale = 1.0f;
    self->mStreamIsDucked = 0;
    self->mLastMix = -1;
    self->mAudioUpdateState = 0;
    self->mRMSDataFrameRate = 60;
    self->mValidRMSData = 0;
    self->mRMSStepFrame = 1;
    self->mPlayIntroMovieWithDJ = 1;
    self->mAudioMode = heap;
    self->unk_6250 = 0;
    for (int i = 0; i < 11; i++) {
        A_4C68->m6394[i] = 1.0f;
        A_4C68->m63E4[i] = 1.0f;
    }
    cSSXAudio_SetCurrVolume_4C68(self, 0, 0.9090909361839294f);
    self->mPreset = 0;
    cSSXAudio_SetPreset(self, 0);
    func_00289470(self);
    func_00289520((cSSXAudio*)self, A_4C68->m60A4);
    cSSXAudio_LoadMixes_4C68(self);
    cSSXAudio_InitAttachedSounds(self);
    SNDSYS_adduserdataclient(cSSXAudio_HandleUserChunk);
    self->mSFXPaused = 0;
    self->mAudioPaused = 0;
    for (int i = 0; i < 2; i++) {
        A_4C68->m6C70[i] = 0;
        A_4C68->m6C78[i] = -1;
    }
    self->mBoostBarPreviousValue[0] = 0;
    self->mBoostBarPreviousValue[1] = 0;
    self->mUberSecPreviousValue[0] = 0;
    self->mUberSecPreviousValue[1] = 0;
    for (int i = 0; i < 3; i++)
        A_4C68->m6030[i] = -1;
    cSSXAudio_ResetLift(self);
    for (int i = 0; i < 23; i++)
        A_4C68->m579C[i] = 0;
    func_002A4590(self, 1);
    func_0029ED90(S_4C68, 0);
    SPCH_Init(A_4C68->m5560);
    func_002B0E60(A_4C68->m5560, 0, 0, str_CHAR, (int)cSSXAudio_GetCurrAppliedVolumePtr((cSSXAudio*)self, 4, 0), 1, -1);
    func_0028F700(self);
    return S_4C68;
}
#undef S_4C68
#undef BM_4C68
#undef A_4C68
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio__cSSXAudio);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cSSXAudio_HandleUserChunk(void* msg);
extern "C" void func_0028F730(void* self);
extern "C" void SNDSYS_removeuserdataclient(void (*cb)(void*));
extern "C" void func_002B0DA0(void* p);
extern "C" void FILESYS_delbigsync(void* h, int a);
extern "C" void cController__cController(void* self, int flags);
extern "C" void cStreamSys__cStreamSys(void* self, int flags);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern void* D_00483688[];
extern void* cSSXAudio_vtbl[];
extern void* D_004836D8[];

struct sSaVEntry285210 {
    short delta;
    short index;
    void* fn;
};
struct sSaVtbl4_285210 {
    sSaVEntry285210 e[4];
} __attribute__((aligned(8)));
struct sSaVtbl8_285210 {
    sSaVEntry285210 e[8];
} __attribute__((aligned(8)));
struct sSaVtbl7_285210 {
    sSaVEntry285210 e[7];
} __attribute__((aligned(8)));
extern const sSaVtbl4_285210 D_00483708_285210 __asm__("D_00483708");
extern const sSaVtbl8_285210 D_00483648_285210 __asm__("D_00483648");
extern const sSaVtbl7_285210 D_00483728_285210 __asm__("D_00483728");

#define S_285210 ((char*)self)
#define BM_285210 (**(char***)(S_285210 + 0x118))

/**
 * Destroys the audio system: shuts down its parts, frees the "CircFIFOBuff" and runs the base destructors.
 *
 * It restores the base vtables first, then calls func_0028F730, unregisters the cSSXAudio_HandleUserChunk callback and
 * calls func_002B0DA0 on the speech system. It also releases +0x5728 (FILESYS_delbigsync) and runs the base part's
 * destructor (cController__cController). With bit 1 of @p flags it also destroys the virtual bases (stream system, bank
 * manager, cSoundSys).
 *
 * @param flags  g++ destructor flags: bit 1 destroys the virtual bases, bit 0 frees the object
 */
extern "C" void cSSXAudio__cSSXAudio(cSSXAudio* self, int flags)
{
    char* speechSys = S_285210 + 0x5560;
    self->MusicSys.mVtbl = D_00483688;
    self->SpeechSys.mVtbl = D_004836D8;
    self->mVtbl = cSSXAudio_vtbl;
    *(const sSaVtbl4_285210**)(*(char**)(BM_285210 + 0x1D8) + 4) = &D_00483708_285210;
    *(const sSaVtbl8_285210**)(BM_285210 + 0xAB0) = &D_00483648_285210;
    *(const sSaVtbl7_285210**)(BM_285210 + 0x1D4) = &D_00483728_285210;
    if (flags == 0) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-up (copied vtables on the stack), written out by hand.
        sSaVtbl4_285210 vtbl1 = D_00483708_285210;
        *(sSaVtbl4_285210**)(*(char**)(BM_285210 + 0x1D8) + 4) = &vtbl1;
        char* base1 = *(char**)(BM_285210 + 0x1D8) - 0x6C90;
        int delta1 = S_285210 - base1;
        vtbl1.e[1].delta = D_00483708_285210.e[1].delta + delta1;
        vtbl1.e[2].delta = D_00483708_285210.e[2].delta + delta1;
        sSaVtbl8_285210 vtbl2 = D_00483648_285210;
        *(sSaVtbl8_285210**)(BM_285210 + 0xAB0) = &vtbl2;
        char* base2 = BM_285210 - 0x6C98;
        int delta2 = S_285210 - base2;
        vtbl2.e[1].delta = D_00483648_285210.e[1].delta + delta2;
        vtbl2.e[2].delta = D_00483648_285210.e[2].delta + delta2;
        vtbl2.e[3].delta = D_00483648_285210.e[3].delta + delta2;
        vtbl2.e[5].delta = D_00483648_285210.e[5].delta + delta2;
        vtbl2.e[6].delta = D_00483648_285210.e[6].delta + delta2;
        sSaVtbl7_285210 vtbl3 = D_00483728_285210;
        *(sSaVtbl7_285210**)(BM_285210 + 0x1D4) = &vtbl3;
        vtbl3.e[1].delta = D_00483728_285210.e[1].delta + delta2;
    }
    func_0028F730(self);
    SNDSYS_removeuserdataclient(cSSXAudio_HandleUserChunk);
    func_002B0DA0(speechSys);
    if (self->unk_5728)
        FILESYS_delbigsync(self->unk_5728, 100);
    if (self->mPlayHistory.Array)
        cMemMan_free(self->mPlayHistory.Array);
    cController__cController(self, 0);
    if (flags & 2) {
        cStreamSys__cStreamSys(*(void**)&self->MusicSys, 0);
        cBankManager__cBankManager(BM_285210, 0);
        cSoundSys__cSoundSys(*(void**)(BM_285210 + 0x1D8), 0);
    }
    if (flags & 1)
        operator_delete((int*)self);
}
#undef S_285210
#undef BM_285210
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetMode);
#ifdef SKIP_ASM
// PORT: the target leaves value in $a1 for this call; bind a 2-arg form.
extern "C" int cSoundSys_SetMode_2(void* bank, int a1) __asm__("cSoundSys_SetMode");

/**
 * Switches the sound system's output mode and remembers the mode when the switch succeeds.
 *
 * The mode is stored at +0x62B0 (On Tour mAudioMode) only when the sound system returns 2.
 *
 * @param value  the new mode (On Tour an eSOUNDDEF_MODE)
 * @return the sound system's result (On Tour eSOUNDMANAGER_SETMODE); 2 means the mode was taken
 */
extern "C" int cSSXAudio_SetMode(cSSXAudio* self, int value)
{
    int retVal = cSoundSys_SetMode_2(*(void**)(**(char***)&self->MusicSys + 0x1D8), value);
    if (retVal == 2) {
        self->mAudioMode = value;
    }
    return retVal;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PlayPositional);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBankMonitor_BANKMONITOR_Create(void* sub, int id, int vol, int a3);
extern "C" int cSSXAudio_GetClosestCamera(cSSXAudio* self, int id);
extern "C" int cSSXAudio_ReplayMode(cSSXAudio* self);
extern "C" void cSndPlayOpts_OPTS_Radius(void* self, float a, float b);
extern "C" int cStreamSys_STREAM_Queue(void* self, int i, int a, int b, int c);
int cStreamSys_STREAM_GetInstance(void* self, int a1);
extern "C" void cBankMonitorInstance_SetStream(void* self, int a, const char* name, int b);
extern "C" void* func_00416210(void* dst, int c, int n);

struct sSnd54F8Ch {
    signed char vol;
    char pad1[0x17];
};
struct sSnd54F8E16 {
    int f0;
    int f4;
    int f8;
    int fC;
};
struct sSnd54F8Sub {
    char pad0[4];
    sSnd54F8Ch def;          // 0x04
    volatile int cur;        // 0x1C
    sSnd54F8Ch* ch;          // 0x20
    int tgt[4];              // 0x24
    int* p34;                // 0x34
    int* vals;               // 0x38
    int* p3C;                // 0x3C
    char e40[4][8];          // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    float* p68;              // 0x68
    float* p6C;              // 0x6C
    sSnd54F8E16* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};
struct sSnd54F8Mgr {
    char pad0[0x1D8];
    sSnd54F8Sub sub;         // 0x1D8
};

/**
 * Plays a speech file from a stream at a position (or 2D) and registers the voice with the bank monitor.
 *
 * Starts stream @p streamIndex on the file @p fileName at @p fileOffset (cStreamSys_STREAM_Queue with the hold time @p holdTime); a negative handle is
 * returned as is. Then pushes a play-option level with the defaults, sets the 3D radius (cSndPlayOpts_OPTS_Radius): @p radius when
 * above 0, else 29.997713 for a human speaker and 100 otherwise, stores @p userInfo as the voice's user pointer and, with a
 * position @p position, follows it on the closest view (cSSXAudio_GetClosestCamera). In the multiplayer game path a human speaker's voice
 * is panned to its side @p viewport (two-player pan). The handle goes to cBankMonitor_BANKMONITOR_Create(.., 0x7F, type 3)
 * and cBankMonitorInstance_SetStream records the stream's slot, file and offset on the instance; the level is popped. It fills slot 1 of
 * cSSXAudio's cSpeechSys vtable (D_004836D8; pure virtual in cSpeechSys), which func_002B04D8 calls to start a speech
 * line.
 *
 * @param vself  the audio system
 * @param streamIndex  the stream index
 * @param fileName  the file name (the speech .big path)
 * @param fileOffset  the offset of the line in the file
 * @param position  the speaker's position (t4Vector*), or 0 for none
 * @param holdTime  the stream hold time
 * @param userInfo  the user pointer stored on the voice (the request's identity)
 * @param isHuman  nonzero when the speaker is a human rider
 * @param radius  the 3D radius, or 0 for the default
 * @param viewport  the speaker's view (two-player side)
 * @return the stream voice handle, or a negative value on failure
 */
extern "C" int cSSXAudio_PlayPositional(cSSXAudio* vself, int streamIndex, const char* fileName, int fileOffset, int position, int holdTime, int userInfo, int isHuman, float radius, int viewport)
{
    cSSXAudio* self = vself;
    // PORT: a2 (a name pointer) is passed through cStreamSys_STREAM_Queue as an int
    int voiceHandle = cStreamSys_STREAM_Queue(((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys), streamIndex, holdTime, (int)fileName, fileOffset);
    if (voiceHandle < 0) {
        return voiceHandle;
    }
    sSnd54F8Mgr* bankMgr = *((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys);
    sSnd54F8Sub* playOpts = &bankMgr->sub;
    bankMgr->sub.cur++;
    playOpts->ch[bankMgr->sub.cur] = bankMgr->sub.def;
    bankMgr->sub.tgt[bankMgr->sub.cur] = 0;
    playOpts->p34[bankMgr->sub.cur] = 0;
    playOpts->vals[bankMgr->sub.cur] = playOpts->ch[bankMgr->sub.cur].vol;
    playOpts->p3C[bankMgr->sub.cur] = 0;
    func_00416210((char*)playOpts + bankMgr->sub.cur * 8 + 0x40, 0, 8);
    playOpts->p60[bankMgr->sub.cur] = 0;
    playOpts->p64[bankMgr->sub.cur] = 0;
    playOpts->p68[bankMgr->sub.cur] = 0.0f;
    playOpts->p6C[bankMgr->sub.cur] = 0.0f;
    playOpts->p70[bankMgr->sub.cur].f4 = 0;
    playOpts->p70[bankMgr->sub.cur].f0 = 100;
    playOpts->p70[bankMgr->sub.cur].f8 = 90;
    playOpts->p70[bankMgr->sub.cur].fC = 50;
    playOpts->p74[bankMgr->sub.cur] = 0;
    playOpts->p80[bankMgr->sub.cur] = 0;
    playOpts->p84[bankMgr->sub.cur] = 0;
    playOpts->p78[bankMgr->sub.cur] = 0x7F;
    playOpts->p7C[bankMgr->sub.cur] = 1;
    if (radius <= 0.0f) {
        if (isHuman != 0) {
            cSndPlayOpts_OPTS_Radius(&(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub, 29.997713088989258f, -1.0f);
        } else {
            cSndPlayOpts_OPTS_Radius(&(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub, 100.0f, -1.0f);
        }
    } else {
        cSndPlayOpts_OPTS_Radius(&(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub, radius, -1.0f);
    }
    (*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.p60[(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.cur] = userInfo;
    if (position != 0) {
        (*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.tgt[(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.cur] = position;
        sSnd54F8Mgr** pp = ((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys);
        int closestView = cSSXAudio_GetClosestCamera(self, position);
        (*pp)->sub.p64[(*pp)->sub.cur] = closestView;
    }
    if (cBE_getInterface_Fv(cBE_getBE(), 0) != 0 && cBENewRaceInterface_newRace.cGamePath == 2 && isHuman != 0 && cSSXAudio_ReplayMode(self) == 0) {
        (*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.p80[(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.cur] = 1;
        (*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.p84[(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.cur] = viewport;
    }
    void* bm = cBankMonitor_BANKMONITOR_Create(&(*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub, voiceHandle, 0x7F, 3);
    if (bm != 0) {
        cBankMonitorInstance_SetStream(bm, cStreamSys_STREAM_GetInstance(((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys), streamIndex), fileName, fileOffset);
    }
    (*((sSnd54F8Mgr**)self->MusicSys.mpVStreamSys))->sub.cur--;
    return voiceHandle;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_Calc3DPos);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" cSSXApp* cAudioInterface_getApp();
extern "C" cGame* cAudioInterface_getGame();
extern "C" float func_0031C228(float);

// PORT: PS2-only vector layout (16-byte aligned quads, copied with lq/sq).
struct sV4_5930 {
    float x, y, z, w;
    sV4_5930() {}
    sV4_5930(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sObjList_5930 {
    int count;
    char* items[1];
};

struct sV3_5930 {
    float x, y, z;
};

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sV4_5930 vu0MtxApply_5930(char* m, const sV4_5930& v)
{
    sV4_5930 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(v), "r"(m)
        : "memory");
    return out;
}

// PORT: PS2 sqrt.s asm helper.
static inline float sqrt_5930(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Atan2_5930(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

/**
 * Computes a positional voice's place relative to its view's camera: pan and elevation angles and squared distance.
 *
 * Returns 1.0 outside the game (no cGame). The voice's position (+0x68) is transformed by the camera matrix of its
 * viewport (+0x84) with VU0, offset by (0, 25, -100) and scaled to metres (0.01). With mAudioMode 0 the angles at +8 /
 * +10 of @p outAngles are 0; otherwise the azimuth at +8 is atan2(x, z) in 1/65536 turns (0x4000 or 0xC000 for a
 * two-player-panned voice, by its side at +0xA4) and the elevation at +10 is atan2(y, horizontal distance). It fills
 * slot 6 of cSSXAudio's cBankMonitor vtable (D_00483640), pure virtual in cBankMonitor's cBankMonitor_vtbl.
 *
 * @param audio  the audio system
 * @param source  the voice's monitor instance (cBankMonitorInstance)
 * @param outAngles  the sound driver's play options (SNDPLAYOPTS) to fill
 * @return the squared distance in square metres
 */
extern "C" float cSSXAudio_Calc3DPos(cSSXAudio* audio, cBankMonitorInstance* source, SNDPLAYOPTS* outAngles)
{
    if ((int)cAudioInterface_getApp()->Game == 0)
        return 1.0f;
    char* game = (char*)cAudioInterface_getGame();
    char* obj = (*(sObjList_5930**)(game + 0x84))->items[source->mViewport];
    float* src = (float*)source->mPosition;
    sV3_5930 relPos;
    sV4_5930 tp = vu0MtxApply_5930(obj + 0x40, sV4_5930(src[0], src[1], src[2], 1.0f));
    float cy;
    float cz = -100.0f;
    relPos.x = tp.x * 0.009999999776482582f;
    cy = 25.0f;
    relPos.y = (tp.y + cy) * 0.009999999776482582f;
    relPos.z = (tp.z + cz) * 0.009999999776482582f;
    if (audio->mAudioMode == 0) {
        *(short*)&outAngles->azimuth = 0;
        outAngles->elevation = 0;
    } else {
        if (source->mTwoPlayerPan != 0) {
            if (source->mTwoPlayerSide != 0)
                *(short*)&outAngles->azimuth = 0x4000;
            else
                outAngles->azimuth = 0xC000;
        } else {
            *(short*)&outAngles->azimuth = (int)(Atan2_5930(relPos.x, relPos.z) * 10430.3779296875f);
        }
        float len = sqrt_5930(relPos.x * relPos.x + relPos.z * relPos.z);
        outAngles->elevation = (int)(Atan2_5930(relPos.y, len) * 10430.3779296875f);
    }
    return relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetAudioUpdateState);
#ifdef SKIP_ASM
/**
 * Sets which update the audio system runs each frame (+0x6474, On Tour mAudioUpdateState).
 *
 * @param State  the update state (On Tour eUpdateState: 0 default, 1 front end, 2 music, 3 pre-game, 4 pre-race,
 *            5 pre-countdown, 6 in game)
 */
extern "C" void cSSXAudio_SetAudioUpdateState(cSSXAudio* self, int State)
{
    int* updateState = (int*)&self->mAudioUpdateState;
    if (*updateState != State) {
        *updateState = State;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_Update);
#ifdef SKIP_ASM
extern int cPauseState_Flags;
extern "C" int func_00390EF8();
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_PlayThunder(void* p, float f);
extern "C" void cController_Update(void* self, float dt);
extern "C" int cStreamSys_STREAM_Playing(void* p, int idx);
extern "C" void cSSXAudio_UpdateDuck(cSSXAudio* self);
struct sSaMix87F8;
extern "C" void cSSXAudio_UpdateMix(sSaMix87F8* self);
extern "C" void func_0028C8C8(void* self);
extern "C" void func_0028F000(void* self);
extern "C" void cSSXAudio_PlayHistoryUpdate(void* self);
extern "C" void cSSXAudio_UpdateLift(void* self);
extern "C" void cSSXAudio_UpdateAttachedSounds(void* self);
extern "C" void func_0028E100(void* self);
extern "C" void cSSXAudio_UpdateTrackBanks(void* self);
extern "C" void cSSXAudio_UpdateBoostBar(void* self);
extern "C" void cSSXAudio_UpdateLandedUber(void* self);
extern "C" void cSSXAudio_UpdateTimedEvent(void* self);
extern "C" void cSSXAudio_UpdateTone(void* self);
extern "C" void cSSXAudio_ApplyRMSData(cSSXAudio* self);
extern "C" void cSSXAudio_DuckToStream(cSSXAudio* self, int a1, float f0, float f1);
extern "C" cSSXApp* cAudioInterface_getApp();
extern "C" cGame* cAudioInterface_getGame();
extern "C" void cSSXAudio_ResetLift(void* self);
extern "C" void cSSXAudio_ResumeAudio(cSSXAudio* self);

/**
 * Updates the audio each frame: the sound system, ducking and mixing, then the in-game sound events.
 *
 * Plays thunder when the screen tint asks (func_00390EF8, cSSXAudio_PlayThunder), runs cController_Update, and while a
 * stream is ducked (mDuckStream not 3) un-ducks it (cSSXAudio_DuckToStream(self, 3, 0.5, 1.0)) once that stream has started and
 * stopped again. Then cSSXAudio_UpdateDuck and cSSXAudio_UpdateMix. In game: func_0028C8C8 and, while mAudioUpdateState is 6,
 * func_0028F000 (not paused), cSSXAudio_PlayHistoryUpdate, the lift update (cSSXAudio_UpdateLift in game state 4, else
 * cSSXAudio_ResetLift), cSSXAudio_UpdateAttachedSounds, func_0028E100, cSSXAudio_UpdateTrackBanks, cSSXAudio_UpdateBoostBar,
 * cSSXAudio_UpdateLandedUber, cSSXAudio_UpdateTimedEvent, cSSXAudio_UpdateTone, and cSSXAudio_ResumeAudio when mAudioPaused is set and
 * cPauseState_Flags bit 1 is clear. Finally cSSXAudio_ApplyRMSData. cGame_render, cRealFE_render and func_0022C830 call
 * it.
 *
 * @param dt  the frame time
 */
extern "C" void cSSXAudio_Update(cSSXAudio* self, float dt)
{
    if (func_00390EF8() != 0)
        cSSXAudio_PlayThunder(cAudioInterface_bxAudio(), 0.0f);
    cController_Update(((char*)self), dt);
    int st = self->mDuckStream;
    if (st != 3) {
        if (self->mDuckedStreamHasStarted == 0) {
            if (cStreamSys_STREAM_Playing(*(void**)&self->MusicSys, st) != 0)
                self->mDuckedStreamHasStarted = 1;
        } else if (cStreamSys_STREAM_Playing(*(void**)&self->MusicSys, st) == 0) {
            cSSXAudio_DuckToStream(self, 3, 0.5f, 1.0f);
        }
    }
    cSSXAudio_UpdateDuck((cSSXAudio*)self);
    cSSXAudio_UpdateMix((sSaMix87F8*)self);
    if ((int)cAudioInterface_getApp()->Game != 0) {
        func_0028C8C8(((char*)self));
        if (self->mAudioUpdateState == 6) {
            if (self->mAudioPaused == 0)
                func_0028F000(((char*)self));
            cSSXAudio_PlayHistoryUpdate(((char*)self));
            if (cAudioInterface_getGame()->mCurrentStateID == 4)
                cSSXAudio_UpdateLift(((char*)self));
            else
                cSSXAudio_ResetLift(((char*)self));
            cSSXAudio_UpdateAttachedSounds(((char*)self));
            func_0028E100(((char*)self));
            cSSXAudio_UpdateTrackBanks(((char*)self));
            cSSXAudio_UpdateBoostBar(((char*)self));
            cSSXAudio_UpdateLandedUber(((char*)self));
            cSSXAudio_UpdateTimedEvent(((char*)self));
            cSSXAudio_UpdateTone(((char*)self));
            if (self->mAudioPaused != 0 && !(cPauseState_Flags & 2))
                cSSXAudio_ResumeAudio(self);
        }
    }
    cSSXAudio_ApplyRMSData((cSSXAudio*)self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetFocusRider);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_ReplayMode(cSSXAudio* self);
extern "C" char* cAudioInterface_getAI();

static inline int isLocalHuman(cRider* rider)
{
    return rider->mIsHuman && rider->mIsLocal;
}

/**
 * Returns the rider the audio should follow for a viewport.
 *
 * In replay mode (cSSXAudio_ReplayMode) it is player 0's rider (cAI Players[0]->mRider, 0 without a player).
 * Otherwise, from cAI's Riders (cAudioInterface_getAI): -1 picks, among the first NumberOfPlayersAllocated riders,
 * the local human (mIsHuman and mIsLocal) with the lowest +0xEC value; -2 the first local human of all
 * NumberOfRidersAllocated riders; any other value the rider whose mViewIndex equals it. Falls back to Riders[0].
 * The fork's C returns the pointer as an int.
 *
 * @param which  viewport index (On Tour viewport), or -1 / -2 as above
 * @return the rider (cRider*)
 */
// PORT: returns a rider pointer; the unit declares the return type as int.
extern "C" int cSSXAudio_GetFocusRider(cSSXAudio* self, int which)
{
    if (cSSXAudio_ReplayMode(self) != 0) {
        cPlayer* player = ((cAI*)cAudioInterface_getAI())->Players[0];
        return player ? (int)player->mRider : 0;
    }
    if (which == -1) {
        int best = 0;
        int bestPrio = 30000;
        for (int i = 0; i < ((cAI*)cAudioInterface_getAI())->NumberOfPlayersAllocated; i++) {
            cRider* curRider = ((cAI*)cAudioInterface_getAI())->Riders[i];
            if (isLocalHuman(curRider) && curRider->RaceRanking < bestPrio) {
                bestPrio = curRider->RaceRanking;
                best = i;
            }
        }
        return (int)((cAI*)cAudioInterface_getAI())->Riders[best];
    }
    if (which == -2) {
        for (int i = 0; i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated; i++) {
            cRider* curRider = ((cAI*)cAudioInterface_getAI())->Riders[i];
            if (isLocalHuman(curRider))
                return (int)curRider;
        }
    } else {
        for (int i = 0; i < ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated; i++) {
            cRider* curRider = ((cAI*)cAudioInterface_getAI())->Riders[i];
            if (curRider->mViewIndex == which)
                return (int)curRider;
        }
    }
    return (int)((cAI*)cAudioInterface_getAI())->Riders[0];
}

#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_BANKMONITOR_DestroyAll);
#ifdef SKIP_ASM
extern "C" void func_002A7718(void*);
extern "C" void cBankMonitor_BANKMONITOR_DestroyAll(void*);

/**
 * Destroys every bank monitor instance: first the crowd voices (func_002A7718), then the base cBankMonitor's
 * (cBankMonitor_BANKMONITOR_DestroyAll).
 *
 * It fills slot 2 of cSSXAudio's cBankMonitor vtable (D_00483640), where cBankMonitor has cBankMonitor_BANKMONITOR_DestroyAll.
 */
extern "C" void cSSXAudio_BANKMONITOR_DestroyAll(cSSXAudio* self)
{
    func_002A7718(self);
    void* inner = *(void**)self->MusicSys.mpVStreamSys;
    cBankMonitor_BANKMONITOR_DestroyAll((char*)inner + 0x1D8);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_BANKMONITOR_DestroyAll1);
#ifdef SKIP_ASM
// PORT: callee declared variadic to reproduce the original by-value struct passing; real signature takes an 8-byte
// struct.
struct cSSXAudio_BANKMONITOR_DestroyAll1_sPair {
    int a;
    int b;
};

extern "C" void cBankMonitor_BANKMONITOR_DestroyAll1(void* obj, ...);

/**
 * Destroys the bank monitor instances that use a callback: forwards to the base cBankMonitor's cBankMonitor_BANKMONITOR_DestroyAll1.
 *
 * It fills slot 3 of cSSXAudio's cBankMonitor vtable (D_00483640), where cBankMonitor has cBankMonitor_BANKMONITOR_DestroyAll1.
 *
 * @param key  the callback (pointer to member)
 */
extern "C" void cSSXAudio_BANKMONITOR_DestroyAll1(cSSXAudio* self, cSSXAudio_BANKMONITOR_DestroyAll1_sPair key)
{
    void* inner = *(void**)self->MusicSys.mpVStreamSys;
    cBankMonitor_BANKMONITOR_DestroyAll1((char*)inner + 0x1D8, key);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_FrontEndLoad);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_InitSoundLevels(cSSXAudio* self, int a1);
extern "C" void cSSXAudio_SetAudioUpdateState(cSSXAudio* self, int a1);
// PORT: FileParser_FileParser/FileParser_AssignString return values (the unit declares them void); bound by asm label.
extern "C" void* FileParser_FileParser_r(void* self, char* name, int size) __asm__("FileParser_FileParser");
extern "C" int FileParser_FindSection(void* self, const char* section);
extern "C" int FileParser_IsEOF(void* self);
extern "C" char* FileParser_GetName(void* self);
extern "C" int FileParser_AssignString_r(void* self, const char* key, char* out) __asm__("FileParser_AssignString");
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void cBankManager_BANK_SyncLoad(void* p, int a1, char* msg, int a3);
extern "C" void cSSXAudio_DuckToStream(cSSXAudio* self, int a1, float f0, float f1);
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(cSSXAudio* self, int a1, int a2);
extern "C" void func_002B49C0(void* mon, int id);
extern "C" void cSSXAudio_ResumeAudio(cSSXAudio* self);
extern "C" void FileParser__FileParser(void* self, int flags);
extern char* str_H_H;
extern char str_GLOBAL[];
extern char str_FE[];
extern char str_MAIN[];
extern char str_pct_spct_spct_s_b[];
extern char str_data_b[];
extern char str_charsel[];
extern char str_BASEPATH[];

struct sIniParser5FB0 {
    int f0;
    char* start;    // 0x4
    char* cur;      // 0x8
    char pad[0x810 - 0xC];
};

struct sSaVt5FB0 { short delta; short index; void (*fn)(void*, char*, int, void*, int); };

/**
 * Loads the front-end audio: the front-end sound levels, its main bank from banks.inf and the character-select music.
 *
 * Sets sound level 0 (cSSXAudio_InitSoundLevels) and update state 1, reads data/config/banks.inf: BASEPATH of the
 * GLOBAL section, then section FE, whose MAIN bank is loaded synchronously into slot 0 (cBankManager_BANK_SyncLoad). Un-ducks
 * (cSSXAudio_DuckToStream(self, 3, 0.5, 1.0)), starts the "charsel" stream through the music system's vtable slot 2 with the
 * Music volume control, sets the song state 301, unk_6290 to 10 and unk_6090 to unk_608C, and resumes the audio
 * (cSSXAudio_ResumeAudio). cRealFE_load and the sound-mode setters of the options and memory-card screens call it.
 */
extern "C" void cSSXAudio_FrontEndLoad(cSSXAudio* self)
{
    sIniParser5FB0 parser;
    char key[0x80];
    char name[0x80];
    char val[0x60];
    char msg[0x40];
    cSSXAudio_InitSoundLevels((cSSXAudio*)self, 0);
    cSSXAudio_SetAudioUpdateState((cSSXAudio*)self, 1);
    FileParser_FileParser_r(&parser, str_H_H, 0x100);
    parser.cur = parser.start;
    if (FileParser_FindSection(&parser, str_GLOBAL)) {
        while (FileParser_IsEOF(&parser) == 0) {
            FileParser_GetName(&parser);
            FileParser_AssignString_r(&parser, str_BASEPATH, key);
        }
    }
    parser.cur = parser.start;
    if (FileParser_FindSection(&parser, str_FE)) {
        strcpy(name, key);
        while (FileParser_IsEOF(&parser) == 0) {
            char* k = FileParser_GetName(&parser);
            FileParser_AssignString_r(&parser, str_BASEPATH, name);
            if (strcasecmp(k, str_MAIN) == 0) {
                FileParser_AssignString_r(&parser, str_MAIN, val);
                sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 0, msg, 0);
            }
        }
    }
    cSSXAudio_DuckToStream(self, 3, 0.5f, 1.0f);
    char* mon = (char*)&self->MusicSys;
    sSaVt5FB0* vt = (sSaVt5FB0*)self->MusicSys.mVtbl;
    char* thisp = mon + vt[2].delta;
    vt[2].fn(thisp, str_charsel, 0xC, cSSXAudio_GetCurrAppliedVolumePtr((cSSXAudio*)self, 1, 0), 1);
    func_002B49C0(mon, 301);
    self->unk_6290 = 10;
    self->unk_6090 = self->unk_608C;
    cSSXAudio_ResumeAudio(self);
    FileParser__FileParser(&parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002861D0);
#ifdef SKIP_ASM
struct sSaVt61D0 { short delta; short index; int (*fn)(void*, int); };

/**
 * Calls the music system's vtable slot 4 (set track) with 1; the retail image has no call or data reference to it.
 */
extern "C" void func_002861D0(cSSXAudio* self)
{
    char* mon = (char*)&self->MusicSys;
    sSaVt61D0* vt = (sSaVt61D0*)self->MusicSys.mVtbl;
    vt[4].fn(vthis(mon, vt[4]), 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_FrontEndPurge);
#ifdef SKIP_ASM
struct cSSXAudio_FrontEndPurge_vte { short delta; short index; void (*fn)(void*); };
extern "C" void func_002B3AC0(void*);
extern "C" void func_002B3D48(void*, float);
extern "C" void cMonitor_MONITOR_DestroyAll(void*);
extern "C" void cBankManager_BANK_UnLoad(void*, int);

/**
 * Releases the front end's audio before leaving it: stops or fades the streams and purges bank slot 0.
 *
 * When unk_608C or unk_6090 is 2 (probably a radio mode) it calls func_002B3AC0 on the stream system.
 * Otherwise it calls func_002B3D48 with 1.0 (probably a one-second fade). It then sets the update state to 0
 * and resets the bank monitor (cMonitor_MONITOR_DestroyAll) and the bank manager (its virtual slot 2).
 */
extern "C" void cSSXAudio_FrontEndPurge(cSSXAudio* self)
{
    if (self->unk_608C == 2 || self->unk_6090 == 2) {
        func_002B3AC0((char*)&self->MusicSys);
    } else {
        func_002B3D48((char*)&self->MusicSys, 1.0f);
    }
    cSSXAudio_SetAudioUpdateState(self, 0);
    cMonitor_MONITOR_DestroyAll(*(char**)self->MusicSys.mpVStreamSys + 0x1D8);
    char* in = *(char**)self->MusicSys.mpVStreamSys;
    char* obj = in + 0x1D8;
    cSSXAudio_FrontEndPurge_vte* vt = *(cSSXAudio_FrontEndPurge_vte**)(in + 0xAB0);
    vt[2].fn(vthis(obj, vt[2]));
    cBankManager_BANK_UnLoad(*(void**)self->MusicSys.mpVStreamSys, 0);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InGameLoad);
#ifdef SKIP_ASM
struct cAsyncSys;
// PORT: cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi is called with a third argument here; bound by asm label.
void cAsyncSys_Init3(cAsyncSys* self, unsigned int a1, int a2) __asm__("cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi");
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" char* FileParser_FindNextSection(void* self);
extern "C" void cBankManager_BANK_AllocMem(void* p, int pool, int size, int a3, int a4);
extern "C" void func_002A6D78(void* self, char* name, char* ext, int mode, int a4);
extern "C" void func_0029F3F8(void* self, int a1);
extern char* str_H;
// Default stream-group extension string (gcc inlines strcpy of the literal as an aligned block copy).
extern char D_004A35A0[] __attribute__((aligned(8)));
extern char D_004A35A8[];
extern char str_BOARD[];
extern char str_CROWD[];
extern char str_AUX[];
extern char str_TRICKY[];
extern char str_LAND[];
extern char str_SPUBOARD[];
extern char str_MOUNTAIN[];
extern char str_TRANSPORT[];

/**
 * Sets up the in-game audio: the mountain sound levels and the world's sound banks from
 * data/config/banks.inf.
 *
 * It reads the GLOBAL section's BASEPATH. Then, for each WORLD section (whose BASEPATH overrides the global
 * one), it registers each bank key with the bank manager (cBankManager_BANK_SyncLoad) as "data/" + path + file. The keys
 * and their slots are MAIN 0, BOARD 1, SPUBOARD 16, CROWD 5, AUX 6, TRICKY 7, MOUNTAIN 2, TRANSPORT 4 and
 * LAND 10. It reserves slots 3 (0x19000 bytes) and 12 (0x6A400), probably as reserved space. The WORLD crowd
 * sounds load from data/config/crowd.inf, with 2 or 1 by the race type (newRace +0x49). The // comment above
 * calls this front-end setup; it is the in-game load.
 */
extern "C" void cSSXAudio_InGameLoad(cSSXAudio* self)
{
    char ext[0x40];
    char key[0x80];
    sIniParser5FB0 parser;
    char name[0x80];
    char val[0x60];
    char msg[0x70];
    __builtin_memcpy(ext, D_004A35A0, 6);
    cSSXAudio_InitSoundLevels((cSSXAudio*)self, 1);
    cAsyncSys_Init3(**(cAsyncSys***)&self->MusicSys, 0, 0);
    self->mUsedBank[0] = D_004A35A8[0];
    self->mNextUsedBank = 0;
    FileParser_FileParser_r(&parser, str_H_H, 0x100);
    parser.cur = parser.start;
    if (FileParser_FindSection(&parser, str_GLOBAL)) {
        while (FileParser_IsEOF(&parser) == 0) {
            if (strcasecmp(FileParser_GetName(&parser), str_BASEPATH) == 0) {
                FileParser_AssignString_r(&parser, str_BASEPATH, key);
            }
        }
    }
    parser.cur = parser.start;
    char* section;
    while ((section = FileParser_FindNextSection(&parser)) != 0) {
        if (func_004165A8(section, ext) == 0) {
            strcpy(name, key);
            strcpy(self->mSwapBasePath, name);
            while (FileParser_IsEOF(&parser) == 0) {
                char* k = FileParser_GetName(&parser);
                FileParser_AssignString_r(&parser, str_BASEPATH, name);
                if (strcasecmp(k, str_MAIN) == 0) {
                    FileParser_AssignString_r(&parser, str_MAIN, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 0, msg, 0);
                } else if (strcasecmp(k, str_BOARD) == 0) {
                    FileParser_AssignString_r(&parser, str_BOARD, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 1, msg, 0);
                } else if (strcasecmp(k, str_SPUBOARD) == 0) {
                    FileParser_AssignString_r(&parser, str_SPUBOARD, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 0x10, msg, 0);
                } else if (strcasecmp(k, str_CROWD) == 0) {
                    FileParser_AssignString_r(&parser, str_CROWD, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 5, msg, 0);
                } else if (strcasecmp(k, str_AUX) == 0) {
                    FileParser_AssignString_r(&parser, str_AUX, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 6, msg, 0);
                } else if (strcasecmp(k, str_TRICKY) == 0) {
                    FileParser_AssignString_r(&parser, str_TRICKY, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 7, msg, 0);
                } else if (strcasecmp(k, str_MOUNTAIN) == 0) {
                    FileParser_AssignString_r(&parser, str_MOUNTAIN, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 2, msg, 0);
                } else if (strcasecmp(k, str_TRANSPORT) == 0) {
                    FileParser_AssignString_r(&parser, str_TRANSPORT, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 4, msg, 0);
                } else if (strcasecmp(k, str_LAND) == 0) {
                    FileParser_AssignString_r(&parser, str_LAND, val);
                    sprintf(msg, str_pct_spct_spct_s_b, str_data_b, name, val);
                    cBankManager_BANK_SyncLoad(**(void***)&self->MusicSys, 10, msg, 0);
                }
            }
        }
    }
    cBankManager_BANK_AllocMem(**(void***)&self->MusicSys, 3, 0x19000, 0, 0);
    cBankManager_BANK_AllocMem(**(void***)&self->MusicSys, 12, 0x6A400, 0, 0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 2) {
        func_002A6D78(((char*)self), str_H, ext, 2, 5);
    } else {
        func_002A6D78(((char*)self), str_H, ext, 1, 5);
    }
    func_0029F3F8(((char*)self), 0);
    self->unk_627C = 1;
    FileParser__FileParser(&parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InGameInit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* cAudioInterface_getAI();
extern "C" void* cRider_getIdentity(void*);
extern "C" int cBERewardInterface_getPlayList(void* self, int a1, int a2);
extern "C" int cBERewardInterface_getRadioMode(void* self, int a1, int a2);
extern "C" void func_0028BF78(void* self, int a1);
extern "C" void func_0028C2D0(void* self, int a1);
extern "C" int func_002B49E0(void*);
extern "C" void func_0028D488(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void cSSXAudio_ResetLift(void* self);
extern "C" void cSSXAudio_DuckToStream(cSSXAudio* self, int a1, float f0, float f1);
extern "C" int cBENewPlayerInterface_isFirstTimeOnTrack(void* self, int index, int bit);
void func_0028DF08_C7B0(void* p) __asm__("func_0028DF08__FPv");
extern "C" void cSSXAudio_StartEvent(cSSXAudio* self);
extern "C" void func_002A4A78(void* self);
struct cWorldPainterManArg;
void* cWorldPainterMan_getQuery(void*, cWorldPainterManArg*);
extern char D_004A5268;
extern char D_004A5270;
extern char D_004A5278;

static inline int sa_state_867E8()
{
    signed char* g = (signed char*)&cBENewRaceInterface_newRace;
    return g[0x49];
}

class cSSXAudio_InGameInit_cObj {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int getAIIndex();
};

/**
 * Sets up the in-game audio when a race gains focus: music playlist, radio mode, world-painter queries and levels.
 *
 * Reads the player rider's playlist and radio mode from the reward interface (cBERewardInterface_getPlayList, cBERewardInterface_getRadioMode) and applies
 * them (func_0028BF78, func_0028C2D0). Unless the music stream already plays track 0x191, it restarts the music
 * (func_0028D488, func_0028CF98 with 0 or 0x24 depending on the race state byte). Then it resets the lift state,
 * creates six world-painter queries (+0x625C/+0x6260, +0x5FA0/+0x5FA4, +0x6C80/+0x6C84) from the world's painter
 * manager, resets the ambience bookkeeping, sets mix channel 3 (cSSXAudio_DuckToStream), records per-track first-time flags for
 * 23 tracks when the race state is 0 (cBENewPlayerInterface_isFirstTimeOnTrack), and finishes with func_0028DF08, cSSXAudio_StartEvent and func_002A4A78.
 */
extern "C" void cSSXAudio_InGameInit(cSSXAudio* self)
{
    void* be = cBE_getInterface_Fv(cBE_getBE(), 13);
    cAI* ai = (cAI*)cAudioInterface_getAI();
    void* rider = cRider_getIdentity(ai->Riders[0]);
    int radioPlaylist = cBERewardInterface_getPlayList(be, 0, (int)rider);
    func_0028BF78(((char*)self), cBERewardInterface_getRadioMode(be, 0, (int)rider));
    func_0028C2D0(((char*)self), radioPlaylist);
    char* musicSys = (char*)&self->MusicSys;
    if (self->MusicSys.mSongPlaying == 0 || func_002B49E0(musicSys) != 401) {
        func_0028D488(((char*)self));
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if (sa_state_867E8() == 0) {
            func_0028CF98(((char*)self), 0, 0, -1, 0);
        } else {
            func_0028CF98(((char*)self), 36, 0, -1, 0);
        }
    }
    cSSXAudio_ResetLift(((char*)self));
    cWorldPainterManArg* pa = (cWorldPainterManArg*)&D_004A5268;
    cWorldPainterManArg* pb = (cWorldPainterManArg*)&D_004A5278;
    cWorldPainterManArg* pc = (cWorldPainterManArg*)&D_004A5270;
    void* painterMan = *(void**)((char*)SSXApp->Game->World + 8);
    self->unk_625C[0] = cWorldPainterMan_getQuery(painterMan, pa);
    self->unk_625C[1] = cWorldPainterMan_getQuery(painterMan, pa);
    self->mpAmbienceQuery[0] = cWorldPainterMan_getQuery(painterMan, pb);
    self->mpAmbienceQuery[1] = cWorldPainterMan_getQuery(painterMan, pb);
    self->mpMixQuery[0] = cWorldPainterMan_getQuery(painterMan, pc);
    void* secondMixQuery = cWorldPainterMan_getQuery(painterMan, pc);
    self->unk_626C = 0;
    self->unk_6274 = 0;
    self->mPlayingTone = 0;
    self->unk_6264 = -1;
    self->unk_6268 = -1;
    self->unk_6270 = 1;
    *(char*)&self->mLastPatchRID = 50;
    self->mpMixQuery[1] = secondMixQuery;
    cSSXAudio_DuckToStream(self, 3, 0.5f, 1.0f);
    self->unk_6288 = 0;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (sa_state_867E8() == 0) {
        int i = 0;
        int* firstTimeFlag = (int*)(&self->unk_579C[0]);
        do {
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
            cRider* playerRider = ((cAI*)cAudioInterface_getAI())->Riders[0];
            *firstTimeFlag = cBENewPlayerInterface_isFirstTimeOnTrack(iface, ((cSSXAudio_InGameInit_cObj*)(&playerRider->mVtbl))->getAIIndex(), i);
            i++;
            firstTimeFlag++;
        } while (i < 23);
    }
    func_0028DF08_C7B0(((char*)self));
    cSSXAudio_StartEvent(self);
    func_002A4A78(((char*)self));
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InGameDeInit);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResumeAudio(cSSXAudio* self);
extern "C" void cSSXAudio_SetAudioUpdateState(cSSXAudio* self, int a1);
extern "C" void func_002B3AC0(void*);
extern "C" void cSSXAudio_StopTone(void* self, float v);
extern "C" void func_002A4550(void*);
extern "C" void cSSXAudio_StopAllRiderSounds(void* self);
extern "C" void cSSXAudio_ResetAttachedSounds(void* self);
extern "C" void cSSXAudio_MixVolume(cSSXAudio* self, int id);
extern "C" void cMonitor_MONITOR_DestroyAll(void*);
extern "C" void func_002B11B0(void*, int);

struct sSaDelVt6A80 { short delta; short index; void (*fn)(void*, int); };

static inline void saDeleteObj(void** slot)
{
    char* p = (char*)*slot;
    if (p) {
        sSaDelVt6A80* vt = *(sSaDelVt6A80**)(p + 4);
        vt[1].fn(vthis(p, vt[1]), 3);
    }
    *slot = 0;
}

/**
 * Shuts down the in-game audio: resumes and stops everything, and deletes the per-race objects.
 *
 * It resumes the audio (cSSXAudio_ResumeAudio) and sets the update state to 0. It then stops the streams
 * (func_002B3AC0) and the tone, and deletes the objects at +0x625C/+0x6260, +0x5FA0/+0x5FA4 and
 * +0x6C80/+0x6C84 (probably the world painter queries). It resets the attached sounds, switches to mix 0,
 * resets the bank monitor and bank manager as FrontEndPurge does, and calls func_002B11B0 on the speech
 * system.
 */
extern "C" void cSSXAudio_InGameDeInit(cSSXAudio* self)
{
    cSSXAudio_ResumeAudio(self);
    cSSXAudio_SetAudioUpdateState(self, 0);
    func_002B3AC0((char*)&self->MusicSys);
    cSSXAudio_StopTone(self, 0.0f);
    func_002A4550(self);
    cSSXAudio_StopAllRiderSounds(self);
    saDeleteObj((void**)(&self->unk_625C[0]));
    saDeleteObj((void**)(&self->unk_625C[1]));
    saDeleteObj((void**)&self->mpAmbienceQuery);
    saDeleteObj((void**)(&self->mpAmbienceQuery[1]));
    saDeleteObj((void**)&self->mpMixQuery);
    saDeleteObj((void**)(&self->mpMixQuery[1]));
    cSSXAudio_ResetAttachedSounds(self);
    cSSXAudio_MixVolume(self, 0);
    cMonitor_MONITOR_DestroyAll(*(char**)self->MusicSys.mpVStreamSys + 0x1D8);
    char* in = *(char**)self->MusicSys.mpVStreamSys;
    char* obj = in + 0x1D8;
    cSSXAudio_FrontEndPurge_vte* vt = *(cSSXAudio_FrontEndPurge_vte**)(in + 0xAB0);
    vt[2].fn(vthis(obj, vt[2]));
    func_002B11B0((char*)&self->SpeechSys, 0);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InGamePurge);
#ifdef SKIP_ASM
extern "C" void func_002A6F38(void*);
extern "C" void cAsyncSys_DoCancel(cAsyncSys*, int);
extern "C" void cBankManager_BANK_UnLoad(void*, int);
extern "C" void cBankManager_BANK_DeallocMem(void*, int);
extern "C" void cAsyncSys_ASYNCSYS_DeInit(void*);
extern "C" void func_0029F5E0(void*);

/**
 * Purges the in-game sound banks: every bank slot 0-16 except 13, then the reserved slots 3 and 12.
 *
 * Also calls func_002A6F38, cAsyncSys_DoCancel (on the bank manager), cAsyncSys_ASYNCSYS_DeInit and func_0029F5E0 (all
 * unknown).
 */
extern "C" void cSSXAudio_InGamePurge(cSSXAudio* self)
{
    int i;
    func_002A6F38(self);
    cAsyncSys_DoCancel((cAsyncSys*)(*(void**)self->MusicSys.mpVStreamSys), 1);
    for (i = 0; i < 17; i++) {
        if (i != 13) {
            cBankManager_BANK_UnLoad(*(void**)self->MusicSys.mpVStreamSys, i);
        }
    }
    cBankManager_BANK_DeallocMem(*(void**)self->MusicSys.mpVStreamSys, 3);
    cBankManager_BANK_DeallocMem(*(void**)self->MusicSys.mpVStreamSys, 12);
    cAsyncSys_ASYNCSYS_DeInit(*(void**)self->MusicSys.mpVStreamSys);
    func_0029F5E0(self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_AttachAudioBank);
#ifdef SKIP_ASM
struct sSsxAudioBanks {
    char pad[0x6C70];
    int loaded[2];
    int sub[2];
};
extern "C" void cBankManager_BANK_Attach(void* self, int i, int a, int b);

/**
 * Loads one of the two track sound banks from data in memory and records its section.
 *
 * The handle carries the slot (0 or 1) in bits 8 and up and the section in the low byte. With data, bank-manager slot 8
 * + slot gets it (cBankManager_BANK_Attach, 0x20000 bytes), mTrackBankLoaded[slot] is set and mTrackBankLoadedSection[slot]
 * records the section. cSSXAudio_UnAttachAudioBank undoes it.
 *
 * @param id  the handle: slot << 8 | section
 * @param on  the bank data in main memory, or 0 to do nothing
 */
extern "C" void cSSXAudio_AttachAudioBank(cSSXAudio* self, unsigned int id, int on)
{
    unsigned int bank = id >> 8;
    if (on != 0) {
        cBankManager_BANK_Attach(**(void***)&self->MusicSys, bank + 8, on, 0x20000);
        self->mTrackBankLoaded[bank] = 1;
        self->mTrackBankLoadedSection[bank] = id & 0xFF;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_UnAttachAudioBank);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopBank(void* p, int id, float v);
extern "C" void cSSXAudio_StopTone(void* self, float v);
extern "C" void cBankManager_BANK_UnAttach(void* mgr, int i);

static inline int saIsOn(char* cfg, int off) { return *(int*)(cfg + off) == 1; }

/**
 * Unloads one of the two track sound banks, given the handle it was loaded with.
 *
 * The handle is the slot (0 or 1) in bits 8 and up and the section in the low byte. Nothing happens unless
 * that section is the one loaded in the slot (+0x6C78). The bank configured for the slot (if any) is stopped
 * at once; slot 1 also stops the tone. If the slot is loaded (+0x6C70), bank-manager slot 8 + slot is freed
 * and the slot cleared.
 *
 * @param handle  the bank handle: slot << 8 | section
 */
extern "C" void cSSXAudio_UnAttachAudioBank(cSSXAudio* self, unsigned int handle)
{
    unsigned int slot = handle >> 8;
    if ((unsigned int)self->mTrackBankLoadedSection[slot] != (handle & 0xFF)) return;
    if (slot == 0) {
        cBankManager* mgr = (cBankManager*)(*(char**)self->MusicSys.mpVStreamSys);
        char* cfg = (char*)mgr->mBank;
        char* bm = (char*)&mgr->BankMonitor;
        int id = saIsOn(cfg, 0x300);
        if (id) id = *(int*)(cfg + 0x304); else id = -1;
        cBankMonitor_BANKMONITOR_StopBank(bm, id, 0.0f);
    } else {
        cSSXAudio_StopTone(self, 0.0f);
        cBankManager* mgr = (cBankManager*)(*(char**)self->MusicSys.mpVStreamSys);
        char* cfg = (char*)mgr->mBank;
        char* bm = (char*)&mgr->BankMonitor;
        int id = saIsOn(cfg, 864);
        if (id) id = *(int*)(cfg + 0x364); else id = -1;
        cBankMonitor_BANKMONITOR_StopBank(bm, id, 0.0f);
    }
    int off = slot << 2;
    char* tbl = (char*)&self->mTrackBankLoaded;
    int* bankEntry = (int*)(tbl + off);
    if (*bankEntry != 0) {
        cBankManager_BANK_UnAttach(*(void**)self->MusicSys.mpVStreamSys, slot + 8);
        *bankEntry = 0;
        self->mTrackBankLoadedSection[slot] = -1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_StartEvent);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SetAudioUpdateState(cSSXAudio* self, int a1);
void cWorldTriggerManager_ResetStates(void* self);
extern "C" void cSSXAudio_BeginAllRiderSound(void* self);
extern "C" cGame* cAudioInterface_getGame();
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_002A39E0(void* self);

/**
 * Starts the in-game audio of an event: update state 6, world triggers reset, all rider sounds, and the grunt timer.
 *
 * Sets the update state 6 (cSSXAudio_SetAudioUpdateState), resets the world triggers, begins every rider's sounds
 * (cSSXAudio_BeginAllRiderSound) and stores the cAI's AITick in mGruntCycle. In game paths 1 and 2 (newRace.cGamePath) it also
 * requests the PA's location line func_002A39E0. cSSXAudio_InGameInit calls it.
 */
extern "C" void cSSXAudio_StartEvent(cSSXAudio* self)
{
    cSSXAudio_SetAudioUpdateState((cSSXAudio*)self, 6);
    cWorldTriggerManager_ResetStates(gWorldTriggerManager);
    cSSXAudio_BeginAllRiderSound(self);
    self->mGruntCycle = cAudioInterface_getGame()->AI->AITick;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int mode = cBENewRaceInterface_newRace.cGamePath;
    if (mode == 1) {
        goto load;
    }
    if (mode == 2) {
    load:
        func_002A39E0(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_OnFinishEvent);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cSSXAudio_GetFocusRider(cSSXAudio* self, int which);
extern "C" int cSSXAudio_ReplayMode(cSSXAudio* self);
extern "C" void func_002A73A8(void* p, int a1, int id);
extern "C" void cSSXAudio_WipeoutSlideEnd(void* self, int id);
extern "C" void cSSXAudio_WipeoutTumbleEnd(void* self, int id);
extern "C" void cSSXAudio_EndUberSpin(void* self, int a1);
extern "C" int func_0028D960(void* self);
extern "C" int func_002A4078(void* self);
extern "C" int cAI_isRaceFinished(void* p);
extern "C" char* cAudioInterface_getAI();
extern "C" void func_002A3708(void* self, void* rider);
extern "C" void func_002A34D0(void* self, void* rider, int a2, int a3);
extern "C" void func_002A45C0(void* self);

static inline int sa_state6EA0()
{
    signed char* g = (signed char*)&cBENewRaceInterface_newRace;
    return g[0x49];
}

struct sSaVt6EA0 { short delta; short index; int (*fn)(void*, int); };

/**
 * Plays the finish of a rider's event for its view: crowd cheer, music, the PA's place line and the result record.
 *
 * Only for the focus rider of its view. Raises a level-2 TRICK cheer (func_002A73A8; view 0 when replaying), stops the
 * rider's wipeout slide and tumble (cSSXAudio_WipeoutSlideEnd, cSSXAudio_WipeoutTumbleEnd) and uber spin. In game modes 2 and 3 before the last
 * heat the music gets track 36 (and unk_627C is cleared), otherwise track 10, each when func_0028D960 allows. A rider
 * that finished (mDidNotFinish 0) gets the PA's place line: func_002A3708 at the locations of func_002A4078; in the
 * multiplayer game path func_002A3708 when the race is over (cAI_isRaceFinished), else func_002A34D0; otherwise
 * func_002A34D0 before heat 3 and func_002A3708 from heat 3. In game path 0 the result is recorded (func_002A45C0).
 * cRider_doFinishEvent calls it.
 *
 * @param rider  the rider (cRider)
 */
// PORT: rider pointers passed through int parameters of some callees.
extern "C" void cSSXAudio_OnFinishEvent(cSSXAudio* self, cRider* rider)
{
    if (((char*)rider) != (char*)cSSXAudio_GetFocusRider((cSSXAudio*)self, rider->mViewIndex))
        return;
    func_002A73A8((char*)gSSXAudio, 2, cSSXAudio_ReplayMode((cSSXAudio*)self) ? 0 : rider->mViewIndex);
    int flag = 1;
    cSSXAudio_WipeoutSlideEnd(((char*)self), (int)rider);
    cSSXAudio_WipeoutTumbleEnd(((char*)self), (int)rider);
    cSSXAudio_EndUberSpin(((char*)self), (int)rider);
    char* cfg = (char*)SSXApp->GameModeMan;
    int nextHeat = *(int*)(cfg + 0x70);
    int heatNum = *(int*)(cfg + 0x0);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* raceDb = (signed char*)&cBENewRaceInterface_newRace;
    int mode = raceDb[0x48];
    if ((mode == 2 || mode == 3) && heatNum < nextHeat) {
        if (func_0028D960(((char*)self))) {
            char* mon = (char*)&self->MusicSys;
            sSaVt6EA0* vt = (sSaVt6EA0*)self->MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), 36);
            flag = 0;
            self->unk_627C = 0;
        }
    }
    if (flag) {
        if (func_0028D960(((char*)self))) {
            char* mon = (char*)&self->MusicSys;
            sSaVt6EA0* vt = (sSaVt6EA0*)self->MusicSys.mVtbl;
            vt[4].fn(vthis(mon, vt[4]), 10);
        }
    }
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (rider->mDidNotFinish == 0) {
        if (func_002A4078(((char*)self)))
            func_002A3708(((char*)self), ((char*)rider));
        else if (sa_state6EA0() == 2) {
            if (cAI_isRaceFinished(cAudioInterface_getAI()))
                func_002A3708(((char*)self), ((char*)rider));
            else
                func_002A34D0(((char*)self), ((char*)rider), 0, -1);
        } else if (heatNum < 3)
            func_002A34D0(((char*)self), ((char*)rider), 0, -1);
        else
            func_002A3708(((char*)self), ((char*)rider));
    }
    if (sa_state6EA0() == 0)
        func_002A45C0(((char*)self));
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_RestartEvent);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResetAudioVariables(cSSXAudio* self);
extern "C" int func_002A4040(void* self);
extern "C" int cSSXAudio_PlayHistoryExists(void* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(void* self, int a, int b, int c, int d, int e);

/**
 * Resets the audio for a restarted event and records a (0, 0) play-history entry if none exists.
 *
 * Calls cSSXAudio_ResetAudioVariables (On Tour ResetAudioVariables). The entry is only added when func_002A4040 returns 0.
 */
extern "C" void cSSXAudio_RestartEvent(cSSXAudio* self)
{
    cSSXAudio_ResetAudioVariables(self);
    if (func_002A4040(self) == 0 && cSSXAudio_PlayHistoryExists(self, 0, 0) == 0) {
        cSSXAudio_PlayHistoryAdd(self, 0, 0, -1, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PartialRestart);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResetAudioVariables(cSSXAudio* self);
extern "C" int func_002A4040(void* self);
extern "C" int cSSXAudio_PlayHistoryExists(void* self, int a, int b);
extern "C" void cSSXAudio_PlayHistoryAdd(void* self, int a, int b, int c, int d, int e);
extern "C" int func_002A4168(void* self);
extern "C" int func_002A41D8(void* self);

/**
 * Resets the audio for a partial restart (cSSXAudio_ResetAudioVariables), and in game modes of func_002A4168 / func_002A41D8 marks
 * play-history event 0.
 *
 * That event is added (cSSXAudio_PlayHistoryAdd(self, 0, 0, -1, 0, 0)) when the game mode is not 4, unk_5824 has
 * reached cGameModeMan iNextHeat and the event is not live yet. cGame_partialRestart calls it.
 */
extern "C" void cSSXAudio_PartialRestart(cSSXAudio* self)
{
    cSSXAudio_ResetAudioVariables(self);
    if (func_002A4168(self) != 0 || func_002A41D8(self) != 0) {
        if (func_002A4040(self) == 0
            && self->unk_5824 >= *(int*)((char*)SSXApp->GameModeMan + 0x70)
            && cSSXAudio_PlayHistoryExists(self, 0, 0) == 0) {
            cSSXAudio_PlayHistoryAdd(self, 0, 0, -1, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ResetAudioVariables);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_ResetLift(void* self);
extern "C" void cSSXAudio_EndUberSpin(void* self, int a1);
extern "C" void cSSXAudio_DuckToStream(cSSXAudio* self, int a1, float f0, float f1);
extern "C" char* cAudioInterface_getAI();
extern "C" void cSSXAudio_WipeoutSlideEnd(void* self, int id);
extern "C" void cSSXAudio_WipeoutTumbleEnd(void* self, int id);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* p, int idx, int a2, float v);

/**
 * Resets the in-game audio state: lift, uber spin, ducking, world triggers, every rider's wipeout loops and the turbo.
 *
 * Calls cSSXAudio_ResetLift, cSSXAudio_EndUberSpin, cSSXAudio_DuckToStream(self, 3, 0.5, 1.0) and
 * cWorldTriggerManager_ResetStates, stops the wipeout slide and tumble of every allocated rider (cSSXAudio_WipeoutSlideEnd,
 * cSSXAudio_WipeoutTumbleEnd), clears mCurTrickPower[0..1], and while mTurboRef is above 0 stops mTurboHandle (mode 1, the fade read
 * back from the just-cleared mCurTrickPower[0]) and clears mTurboRef. cSSXAudio_RestartEvent, cSSXAudio_PartialRestart and
 * cGFPreEventState_ResetGameState call it.
 */
extern "C" void cSSXAudio_ResetAudioVariables(cSSXAudio* self)
{
    int i = 0;
    cSSXAudio_ResetLift(self);
    cSSXAudio_EndUberSpin(self, 0);
    cSSXAudio_DuckToStream(self, 3, 0.5f, 1.0f);
    cWorldTriggerManager_ResetStates(gWorldTriggerManager);
    int n = ((cAI*)cAudioInterface_getAI())->NumberOfRidersAllocated;
    for (; i < n; i++) {
        cSSXAudio_WipeoutSlideEnd(self, (int)((cAI*)cAudioInterface_getAI())->Riders[i]);
        cSSXAudio_WipeoutTumbleEnd(self, (int)((cAI*)cAudioInterface_getAI())->Riders[i]);
    }
    *(int*)&self->mCurTrickPower[0] = 0;
    *(int*)&self->mCurTrickPower[1] = 0;
    if (self->mTurboRef > 0) {
        cBankMonitor_BANKMONITOR_StopHandle(**(char***)&self->MusicSys + 0x1D8, self->mTurboHandle, 1, self->mCurTrickPower[0]);
        self->mTurboRef = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002872A8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0028D488(void* self);
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" void func_002A26F0(void* self, int a1, int a2);
void func_002B1758(void* self, int a1);
int func_002B4908(void* mon);

struct sSaVt72A8 { short delta; short index; int (*fn)(void*, int); };

/**
 * Starts the music and DJ intro of an event without a start gate (game modes 5 and 6).
 *
 * Stores the cAI's AITick in mGruntCycle, then only in game modes 5 and 6: with unk_530 set the music system's vtable
 * slot 4 gets track 0, else func_0028D488 and func_0028CF98(self, 0, 0, -1, 0). Unless play-history event 0 is live: in
 * game events 4 and 5 it queues the DJ line func_002A1138 (unk_5788), or in the multiplayer path plays func_002A26F0(0,
 * 0) at once, then queues the song announcement (unk_5774, unk_5778 = func_002B4908). The pre-event overlays in
 * fe/ovstatepause.cpp call it when they close.
 */
extern "C" void func_002872A8(cSSXAudio* self)
{
    self->mGruntCycle = cAudioInterface_getGame()->AI->AITick;
    cBE_getInterface_Fv(cBE_getBE(), 0);
    signed char* raceDb = (signed char*)&cBENewRaceInterface_newRace;
    int mode = raceDb[0x48];
    if (mode != 5 && mode != 6) return;
    char* mon = (char*)&self->MusicSys;
    if (self->MusicSys.mSongPlaying != 0) {
        sSaVt72A8* vt = (sSaVt72A8*)self->MusicSys.mVtbl;
        vt[4].fn(vthis(mon, vt[4]), 0);
    } else {
        func_0028D488(self);
        func_0028CF98(self, 0, 0, -1, 0);
    }
    if (cSSXAudio_PlayHistoryExists(self, 0, 0) != 0) return;
    signed char* newRace = (signed char*)&cBENewRaceInterface_newRace;
    int gameEvent = newRace[0x4A];
    if (gameEvent == 4 || gameEvent == 5) {
        if (newRace[0x49] != 2) {
            self->unk_5788 = 1;
        } else {
            func_002A26F0(self, 0, 0);
            func_002B1758(&self->SpeechSys, 0);
        }
    }
    self->unk_5774 = 1;
    self->unk_5778 = func_002B4908(mon);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_Volume2Float);
#ifdef SKIP_ASM
// PORT: real arity is (self, n); the unit declares cSSXAudio_Volume2Float(void*) for its callers.
// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float clamp_73D8(float f, float lo, float hi)
{
    if (f >= lo) {
        return f <? hi;
    }
    return lo;
}

extern "C" float cSSXAudio_Volume2Float_impl(void* self, int n) __asm__("cSSXAudio_Volume2Float");
/**
 * Converts a volume setting 0..11 to a level 0.0..1.0 (n / 11, clamped).
 *
 * @param n  the setting
 * @return the level
 */
extern "C" float cSSXAudio_Volume2Float_impl(void* self, int n)
{
    return clamp_73D8(n * 0.09090909361839294f, 0.0f, 1.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287410);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_Volume2Float(void*);
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float); the unit declares (self, float, int).
void cSSXAudio_SetCurrVolume_if(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");

/**
 * Applies the music volume setting (0..11, passed on in $a1 to cSSXAudio_Volume2Float) to the Music, Ambient and DJ controls.
 *
 * Sets controls 1 and 8 to the level, and control 2 too when DJ speech is on (unk_62B8) and unk_608C is not 2. The
 * options screens, the pause options, the memory-card load and cSSXApp_initload call it. The setting is not a declared
 * parameter: it stays in $a1 for cSSXAudio_Volume2Float.
 */
extern "C" void func_00287410(cSSXAudio* self)
{
    float level = cSSXAudio_Volume2Float(self);
    cSSXAudio_SetCurrVolume_if(self, 1, level);
    cSSXAudio_SetCurrVolume_if(self, 8, level);
    if (self->unk_62B8 != 0 && self->unk_608C != 2) {
        cSSXAudio_SetCurrVolume_if(self, 2, level);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetSFXVolume);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_Volume2Float(void*);
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float); the unit declares (self, float, int).
void cSSXAudio_SetCurrVolume_if(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");

/**
 * Applies the sound-effect volume setting (0..11, passed on in $a1) to the SFX, PA, Board and Collision controls.
 *
 * Sets controls 5, 3, 6 and 7 to cSSXAudio_Volume2Float's level, and with mArcadeSFX set also 9 and 10 (ArcadeSFX,
 * ArcadeSpeech). The setting is not a declared parameter: it stays in $a1 for cSSXAudio_Volume2Float.
 */
extern "C" void cSSXAudio_SetSFXVolume(cSSXAudio* self)
{
    float level = cSSXAudio_Volume2Float(self);
    cSSXAudio_SetCurrVolume_if(self, 5, level);
    cSSXAudio_SetCurrVolume_if(self, 3, level);
    cSSXAudio_SetCurrVolume_if(self, 6, level);
    cSSXAudio_SetCurrVolume_if(self, 7, level);
    if (self->mArcadeSFX != 0) {
        cSSXAudio_SetCurrVolume_if(self, 9, level);
        cSSXAudio_SetCurrVolume_if(self, 10, level);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", func_00287520);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_Volume2Float(void*);
extern "C" void cSSXAudio_SetCurrVolume(cSSXAudio*, float, int);

/**
 * Applies the character speech volume setting (0..11, passed on in $a1) to control 4 (Character).
 *
 * The setting is not a declared parameter: it stays in $a1 for cSSXAudio_Volume2Float.
 */
extern "C" void func_00287520(cSSXAudio* self)
{
    cSSXAudio_SetCurrVolume(self, cSSXAudio_Volume2Float(self), 4);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetDJSpeech);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SetCurrVolume(cSSXAudio*, float, int);
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Turns the arcade sound effects on or off.
 *
 * When @p on is set and the sound mode at +0x608C is neither 2 nor 3, volume control 2 is set to its user level
 * (cSSXAudio_GetCurrSetVolume(self, 1), i.e. control 1's level) and +0x62B8 becomes 1; otherwise control 2 is set to 0 and +0x62B8
 * cleared.
 *
 * @param on  nonzero to enable the arcade effects (On Tour bOn)
 */
extern "C" void cSSXAudio_SetDJSpeech(cSSXAudio* self, int on)
{
    if (on != 0 && self->unk_608C != 2 && self->unk_608C != 3) {
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, cSSXAudio_GetCurrSetVolume(self, 1), 2);
        self->unk_62B8 = 1;
    } else {
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, 0.0f, 2);
        self->unk_62B8 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetArcadeSFX);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SetCurrVolume(cSSXAudio*, float, int);
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Turns the arcade sound effects and speech on or off: controls 9 and 10 get the SFX level (control 5) or 0.
 *
 * Records @p on in mArcadeSFX. cSSXAudio_SetPreset calls it for every preset (on for presets 0..3, off for 4 and 5).
 *
 * @param on  nonzero to enable the arcade sounds
 */
extern "C" void cSSXAudio_SetArcadeSFX(cSSXAudio* self, int on)
{
    if (on != 0) {
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, cSSXAudio_GetCurrSetVolume(self, 5), 9);
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, cSSXAudio_GetCurrSetVolume(self, 5), 10);
    } else {
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, 0.0f, 9);
        cSSXAudio_SetCurrVolume((cSSXAudio*)self, 0.0f, 10);
    }
    self->mArcadeSFX = on;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00287670);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Returns the music volume setting 0..11: the set level of control 1 (Music) times 11, truncated.
 *
 * @return the setting
 */
extern "C" int func_00287670(void* self)
{
    return (int)(cSSXAudio_GetCurrSetVolume(self, 1) * 11.0f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetSFXVolume);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Returns the sound-effect volume setting 0..11: the set level of control 5 (SFX) times 11, truncated.
 *
 * @return the setting
 */
extern "C" int cSSXAudio_GetSFXVolume(cSSXAudio* self)
{
    return (int)(cSSXAudio_GetCurrSetVolume(self, 5) * 11.0f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_002876D0);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Returns the character speech volume setting 0..11: the set level of control 4 (Character) times 11, truncated.
 *
 * @return the setting
 */
extern "C" int func_002876D0(void* self)
{
    return (int)(cSSXAudio_GetCurrSetVolume(self, 4) * 11.0f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetCurrVolume);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern float gSFXScaling;
extern float gMusicScaling;
extern float gCharSpchScaling;
extern float gDJSpchScaling;
extern float gPASpchScaling;
extern float gBoardScaling;
extern float gCollisionScaling;
extern float gAmbientScaling;
extern float gArcadeSFXScaling;
extern float gArcadeSpeechScaling;
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;
extern "C" void func_002B4388(void* mon);
extern "C" void SNDmastervol(signed char vol);



/**
 * Sets one volume control's level and applies it.
 *
 * @p volume is clamped to 0..1 and stored as the control's user level; its base level is the level itself for control 0 and
 * the level times a per-control constant (gSFXScaling .. gArcadeSpeechScaling) for the others. The gain is base * the control's
 * volume (+0x6394) and the output gain * the current factor (+0x63E4). Control 0 then sets the library master volume
 * (SNDmastervol, out * 127), control 1 updates the music monitor (func_002B4388) and control 4 rescales the ten mix
 * levels at +0x636C. Quirk: control 1 calls func_002B4388 twice, the first time before the new gain is computed.
 *
 * @param self_  the audio system (this)
 * @param volume      the new level, 0..1 (On Tour v)
 * @param ch     the volume control (On Tour eVolumeControl volumeControl)
 */
// PORT: `<?`/`>?` are the g++ min/max operators (EE min.s/max.s).
extern "C" void cSSXAudio_SetCurrVolume(cSSXAudio* self_, float volume, int ch)
{
    cSSXAudio* self = self_;
    volume = (volume <? 1.0f) >? 0.0f;
    tSoundVolume* currVolume = &((&self->mSoundVolume[0]))[ch];
    switch (ch) {
    case 0:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume;
        break;
    case 1:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gMusicScaling;
        func_002B4388(&self->MusicSys);
        break;
    case 2:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gDJSpchScaling;
        break;
    case 3:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gPASpchScaling;
        break;
    case 4:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gCharSpchScaling;
        break;
    case 5:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gSFXScaling;
        break;
    case 6:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gBoardScaling;
        break;
    case 7:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gCollisionScaling;
        break;
    case 8:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gAmbientScaling;
        break;
    case 9:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gArcadeSFXScaling;
        break;
    case 10:
        currVolume->setVolume = volume;
        currVolume->curVolume = volume * gArcadeSpeechScaling;
        break;
    }
    currVolume->duckedVolume = currVolume->curVolume * self->mDuckLevel[ch];
    currVolume->appliedVolume = currVolume->duckedVolume * self->mMixLevel[ch];
    switch (ch) {
    case 0:
        SNDmastervol((int)(currVolume->appliedVolume * 127.0f));
        break;
    case 1:
        func_002B4388(&self->MusicSys);
        break;
    case 4:
        self->unk_636C[0] = currVolume->appliedVolume * gMobyScaling;
        self->unk_636C[1] = currVolume->appliedVolume * gKaoriScaling;
        self->unk_636C[2] = currVolume->appliedVolume * gAerielScaling;
        self->unk_636C[3] = currVolume->appliedVolume * gMacScaling;
        self->unk_636C[4] = currVolume->appliedVolume * gZoeScaling;
        self->unk_636C[5] = currVolume->appliedVolume * gGrommetScaling;
        self->unk_636C[6] = currVolume->appliedVolume * gEliseScaling;
        self->unk_636C[7] = currVolume->appliedVolume * gRocoScaling;
        self->unk_636C[8] = currVolume->appliedVolume * gPsymonScaling;
        self->unk_636C[9] = currVolume->appliedVolume * gDeiterScaling;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCurrSetVolume);
#ifdef SKIP_ASM
/**
 * Returns the set level of a volume control (mSoundVolume[i].setVolume).
 *
 * @param volumeIndex  the volume control (eVolumeControl)
 * @return the level, 0.0..1.0
 */
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int volumeIndex)
{
    self = (char*)self + (volumeIndex << 4);
    return *(float*)((char*)self + 0x62bc);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCurrAppliedVolume);
#ifdef SKIP_ASM
/**
 * Returns the applied level of a volume control, or a character's own level for control 4.
 *
 * Control 4 (Character) with a character below 10 reads unk_636C[charIndex]; others read mSoundVolume[i].appliedVolume.
 *
 * @param volumeIndex  the volume control (eVolumeControl)
 * @param charIndex  the character for control 4
 * @return the level
 */
extern "C" float cSSXAudio_GetCurrAppliedVolume(cSSXAudio* self, int volumeIndex, int charIndex)
{
    if (volumeIndex != 4 || charIndex >= 10) {
        return self->mSoundVolume[volumeIndex].appliedVolume;
    }
    return self->unk_636C[charIndex];
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCurrAppliedVolumePtr);
#ifdef SKIP_ASM
/**
 * Returns a pointer to the applied level of a volume control, or to a character's own level for control 4.
 *
 * Same choice as cSSXAudio_GetCurrAppliedVolume. The sound calls pass it as the voice's volume control (mVolControl).
 *
 * @param volumeIndex  the volume control (eVolumeControl)
 * @param charIndex  the character for control 4
 * @return a pointer to the level (float)
 */
extern "C" void* cSSXAudio_GetCurrAppliedVolumePtr(cSSXAudio* self, int volumeIndex, int charIndex)
{
    if (volumeIndex != 4 || charIndex >= 10) {
        // PORT: pointer held in int (only spelling found that gives idx-first addu)
        return (char*)((volumeIndex << 4) + (int)self + 0x62C8);
    }
    return (char*)&self->unk_636C[charIndex];
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCurrSetVolumePtr);
#ifdef SKIP_ASM
/**
 * Returns a pointer to the set level of a volume control (&mSoundVolume[idx].setVolume).
 *
 * @param idx  the volume control (eVolumeControl)
 * @return a pointer to the level
 */
extern "C" char* cSSXAudio_GetCurrSetVolumePtr(cSSXAudio* self, int idx)
{
    return &((char*)self)[25276 + (idx << 4)];
}
#endif
//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_IncCurrVolume);
#ifdef SKIP_ASM
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float); the unit declares (self, float, int).
void cSSXAudio_SetCurrVolume_if(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");

/**
 * Raises one volume control by one step (1/11) through SetCurrVolume (cSSXAudio_SetCurrVolume).
 *
 * @param idx  the volume control (On Tour eVolumeControl: 0 master, 1 music, 2 DJ, 3 PA, 4 character, 5 SFX,
 *             ...)
 */
extern "C" void cSSXAudio_IncCurrVolume(cSSXAudio* self, int idx)
{
    cSSXAudio_SetCurrVolume_if(((char*)self), idx, self->mSoundVolume[idx].setVolume + 0.09090909361839294f);
}
#endif
//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_DecCurrVolume);
#ifdef SKIP_ASM
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float); the unit declares (self, float, int).
void cSSXAudio_SetCurrVolume_if(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");

/**
 * Lowers one volume control by one step (1/11) through SetCurrVolume (cSSXAudio_SetCurrVolume).
 *
 * @param idx  the volume control (On Tour eVolumeControl: 0 master, 1 music, 2 DJ, 3 PA, 4 character, 5 SFX,
 *             ...)
 */
extern "C" void cSSXAudio_DecCurrVolume(cSSXAudio* self, int idx)
{
    cSSXAudio_SetCurrVolume_if(((char*)self), idx, self->mSoundVolume[idx].setVolume - 0.09090909361839294f);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_DuckVolume);
#ifdef SKIP_ASM
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;

struct sSaChan7A10 {
    float base;     // +0x0
    float gain;     // +0x4
    float out;      // +0x8
    float pad;
};


struct sSaMix7A10 {
    char pad0[0x62C0];
    sSaChan7A10 chan[11];   // 0x62C0 (chan[10].pad overlaps mix[0])
    char pad6370[0x6398 - 0x6370];
    float vol[10];          // 0x6398 (vol[j-1])
    int fading;             // 0x63C0
    int fadeMask;           // 0x63C4
    float fadeRate;         // 0x63C8
    float fadeTarget;       // 0x63CC
    float fadeCur;          // 0x63D0
    char pad63D4[0x63E8 - 0x63D4];
    float cur[10];          // 0x63E8 (cur[j-1])
};

static inline void sSaMix7A10_set(sSaMix7A10* self, int j, float v)
{
    self->vol[j - 1] = v;
    self->chan[j].gain = self->chan[j].base * v;
    self->chan[j].out = self->chan[j].gain * self->cur[j - 1];
}

/**
 * Ducks a set of volume controls to a level, at once or over a time.
 *
 * Mask bits pick the controls: 1 music, 2 SFX, 4 character, 8 DJ, 0x10 ambient, 0x20 board, 0x40 collision,
 * 0x80 PA, 0x100 arcade SFX and 0x200 arcade speech. Setting the character control also refreshes the ten
 * per-character mix values. With a nonzero @p time the duck fades per frame (UpdateDuck) from the current
 * level instead.
 *
 * @param self_  the audio system
 * @param mask   which volume controls to duck (On Tour duck)
 * @param finalLevel      the duck level to reach (On Tour finalLevel)
 * @param time   fade time in seconds; 0 applies the level at once
 */
extern "C" void cSSXAudio_DuckVolume(cSSXAudio* self_, int mask, float finalLevel, float time)
{
    sSaMix7A10* self = (sSaMix7A10*)self_;
    if (time == 0.0f) {
        if (mask & 1)
            sSaMix7A10_set(self, 1, finalLevel);
        if (mask & 2)
            sSaMix7A10_set(self, 5, finalLevel);
        if (mask & 4) {
            sSaMix7A10_set(self, 4, finalLevel);
            float level = self->chan[4].out;
            cSSXAudio* mixOut = (cSSXAudio*)self;
            mixOut->unk_636C[0] = level * gMobyScaling;
            mixOut->unk_636C[1] = level * gKaoriScaling;
            mixOut->unk_636C[2] = level * gAerielScaling;
            mixOut->unk_636C[3] = level * gMacScaling;
            mixOut->unk_636C[4] = level * gZoeScaling;
            mixOut->unk_636C[5] = level * gGrommetScaling;
            mixOut->unk_636C[6] = level * gEliseScaling;
            mixOut->unk_636C[7] = level * gRocoScaling;
            mixOut->unk_636C[8] = level * gPsymonScaling;
            mixOut->unk_636C[9] = level * gDeiterScaling;
        }
        if (mask & 8)
            sSaMix7A10_set(self, 2, finalLevel);
        if (mask & 0x80)
            sSaMix7A10_set(self, 3, finalLevel);
        if (mask & 0x20)
            sSaMix7A10_set(self, 6, finalLevel);
        if (mask & 0x40)
            sSaMix7A10_set(self, 7, finalLevel);
        if (mask & 0x10)
            sSaMix7A10_set(self, 8, finalLevel);
        if (mask & 0x100)
            sSaMix7A10_set(self, 9, finalLevel);
        if (mask & 0x200)
            sSaMix7A10_set(self, 10, finalLevel);
    } else {
        self->fadeMask = mask;
        self->fadeTarget = finalLevel;
        self->fadeRate = (finalLevel - self->fadeCur) / time * 0.01666666753590107f;
        if (self->fadeRate != 0.0f)
            self->fading = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_UpdateDuck);
#ifdef SKIP_ASM
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;

struct sSaChan7C48 {
    float base;     // +0x0
    float gain;     // +0x4
    float out;      // +0x8
    float pad;
};


struct sSaMix7C48 {
    char pad0[0x62C0];
    sSaChan7C48 chan[11];   // 0x62C0 (chan[10].pad overlaps mix[0])
    char pad6370[0x6398 - 0x6370];
    float vol[10];          // 0x6398 (vol[j-1])
    int fading;             // 0x63C0
    int fadeMask;           // 0x63C4
    float fadeRate;         // 0x63C8
    float fadeTarget;       // 0x63CC
    float fadeCur;          // 0x63D0
    char pad63D4[0x63E8 - 0x63D4];
    float cur[10];          // 0x63E8 (cur[j-1])
};

static inline void sSaMix7C48_set(sSaMix7C48* self, int j, float v)
{
    self->vol[j - 1] = v;
    self->chan[j].gain = self->chan[j].base * v;
    self->chan[j].out = self->chan[j].gain * self->cur[j - 1];
}

/**
 * Advances a duck fade by one frame and applies the current duck level to the controls being ducked.
 *
 * The level stops at its target, which ends the fade. The controls are those of the DuckVolume mask.
 *
 * @param self_  the audio system
 */
extern "C" void cSSXAudio_UpdateDuck(cSSXAudio* self_)
{
    sSaMix7C48* self = (sSaMix7C48*)self_;
    if (self->fading) {
        self->fadeCur += self->fadeRate;
        if (self->fadeRate < 0.0f) {
            if (self->fadeCur < self->fadeTarget) {
                self->fadeCur = self->fadeTarget;
                self->fading = 0;
            }
        } else if (self->fadeCur > self->fadeTarget) {
            self->fadeCur = self->fadeTarget;
            self->fading = 0;
        }
        if (self->fadeMask & 1)
            sSaMix7C48_set(self, 1, self->fadeCur);
        if (self->fadeMask & 2)
            sSaMix7C48_set(self, 5, self->fadeCur);
        if (self->fadeMask & 4) {
            sSaMix7C48_set(self, 4, self->fadeCur);
            float level = self->chan[4].out;
            cSSXAudio* mixOut = (cSSXAudio*)self;
            mixOut->unk_636C[0] = level * gMobyScaling;
            mixOut->unk_636C[1] = level * gKaoriScaling;
            mixOut->unk_636C[2] = level * gAerielScaling;
            mixOut->unk_636C[3] = level * gMacScaling;
            mixOut->unk_636C[4] = level * gZoeScaling;
            mixOut->unk_636C[5] = level * gGrommetScaling;
            mixOut->unk_636C[6] = level * gEliseScaling;
            mixOut->unk_636C[7] = level * gRocoScaling;
            mixOut->unk_636C[8] = level * gPsymonScaling;
            mixOut->unk_636C[9] = level * gDeiterScaling;
        }
        if (self->fadeMask & 8)
            sSaMix7C48_set(self, 2, self->fadeCur);
        if (self->fadeMask & 0x80)
            sSaMix7C48_set(self, 3, self->fadeCur);
        if (self->fadeMask & 0x20)
            sSaMix7C48_set(self, 6, self->fadeCur);
        if (self->fadeMask & 0x40)
            sSaMix7C48_set(self, 7, self->fadeCur);
        if (self->fadeMask & 0x10)
            sSaMix7C48_set(self, 8, self->fadeCur);
        if (self->fadeMask & 0x100)
            sSaMix7C48_set(self, 9, self->fadeCur);
        if (self->fadeMask & 0x200)
            sSaMix7C48_set(self, 10, self->fadeCur);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_DuckToStream);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_DuckVolume(cSSXAudio* self, int a1, float a2, float a3);
extern "C" int func_00284BA0(int);

/**
 * Ducks the other sounds while a stream plays, or with stream 3 (none) restores them.
 *
 * Stream 3 while ducked (mStreamIsDucked): un-ducks the controls of mDuckStream (cSSXAudio_DuckVolume with the mask
 * func_00284BA0, level 1.0 over @p fadeTime), then mDuckScale = @p duckLevel, mDuckStream = 3, mStreamIsDucked = 0.
 * Another stream while not ducked: ducks that stream's controls to @p duckLevel over @p fadeTime and records it
 * (mDuckStream, mStreamIsDucked 1, mDuckScale, mDuckedStreamHasStarted 0).
 *
 * @param duckStream  the stream that ducks the others, or 3 to restore
 * @param fadeTime  the fade time
 * @param duckLevel  the ducked level
 */
extern "C" void cSSXAudio_DuckToStream(cSSXAudio* self, int duckStream, float fadeTime, float duckLevel)
{
    if (duckStream == 3) {
        if (self->mStreamIsDucked != 0) {
            cSSXAudio_DuckVolume((cSSXAudio*)self, func_00284BA0(self->mDuckStream), 1.0f, fadeTime);
            self->mDuckScale = duckLevel;
            self->mDuckStream = duckStream;
            self->mStreamIsDucked = 0;
        }
    } else if (self->mStreamIsDucked == 0) {
        cSSXAudio_DuckVolume((cSSXAudio*)self, func_00284BA0(duckStream), duckLevel, fadeTime);
        self->mDuckStream = duckStream;
        self->mStreamIsDucked = 1;
        self->mDuckScale = duckLevel;
        self->mDuckedStreamHasStarted = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_LoadMixes);
#ifdef SKIP_ASM
// PORT: FileParser_FileParser returns a value (the unit declares it void); bound by asm label.
extern "C" void* FileParser_FileParser_r(void* self, char* name, int size) __asm__("FileParser_FileParser");
extern "C" void FileParser__FileParser(void* self, int flags);
extern "C" char* FileParser_FindNextSection(void* self);
extern "C" char* FileParser_GetName(void* self);
extern "C" int FileParser_IsEOF(void* self);
extern "C" int FileParser_AssignInt(void* self, const char* key, int* out);
extern "C" int strcasecmp(const char* a, const char* b);
extern char* D_004A3560;
extern char str_MUSIC[];
extern char str_DJ[];
extern char str_PA[];
extern char str_CHARACTER[];
extern char str_BOARD[];
extern char str_COLLISION[];
extern char str_AMBIENT[];
extern char str_ARCADESFX[];
extern char str_ARCADESPEECH[];
extern char str_TIME_b[];

struct sIniParser7FC8 {
    int f0;
    char* start;    // 0x4
    char* cur;      // 0x8
    char pad[0x810 - 0xC];
};

struct sSfxEntry7FC8 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    float f24;
};

struct sAudio7FC8 {
    char pad[0x6478];
    int count;                      // 0x6478
    sSfxEntry7FC8 entries[25];      // 0x647C
};

/**
 * Loads up to 25 audio mixes from "data/config/mix.inf".
 *
 * Every mix first gets 100 for each of its nine volumes and a time of 0. Then each section of the file fills
 * the next mix from its MUSIC, DJ, PA, CHARACTER, BOARD, COLLISION, AMBIENT, ARCADESFX and ARCADESPEECH values;
 * TIME is given in milliseconds and stored in seconds. The number of mixes read goes into the count.
 */
extern "C" void cSSXAudio_LoadMixes(sAudio7FC8* self)
{
    sIniParser7FC8 parser;
    int val;
    self->count = 0;
    FileParser_FileParser_r(&parser, D_004A3560, 0x100);
    parser.cur = parser.start;
    for (int i = 0; i < 25; i++) {
        self->entries[i].f0 = 100;
        self->entries[i].f4 = 100;
        self->entries[i].f8 = 100;
        self->entries[i].fC = 100;
        self->entries[i].f10 = 100;
        self->entries[i].f14 = 100;
        self->entries[i].f18 = 100;
        self->entries[i].f1C = 100;
        self->entries[i].f20 = 100;
        self->entries[i].f24 = 0.0f;
    }
    while (self->count < 25 && FileParser_FindNextSection(&parser) != 0) {
        while (FileParser_IsEOF(&parser) == 0) {
            char* key = FileParser_GetName(&parser);
            if (strcasecmp(key, str_MUSIC) == 0) {
                FileParser_AssignInt(&parser, str_MUSIC, &val);
                self->entries[self->count].f0 = val;
            } else if (strcasecmp(key, str_DJ) == 0) {
                FileParser_AssignInt(&parser, str_DJ, &val);
                self->entries[self->count].f4 = val;
            } else if (strcasecmp(key, str_PA) == 0) {
                FileParser_AssignInt(&parser, str_PA, &val);
                self->entries[self->count].f8 = val;
            } else if (strcasecmp(key, str_CHARACTER) == 0) {
                FileParser_AssignInt(&parser, str_CHARACTER, &val);
                self->entries[self->count].fC = val;
            } else if (strcasecmp(key, str_BOARD) == 0) {
                FileParser_AssignInt(&parser, str_BOARD, &val);
                self->entries[self->count].f10 = val;
            } else if (strcasecmp(key, str_COLLISION) == 0) {
                FileParser_AssignInt(&parser, str_COLLISION, &val);
                self->entries[self->count].f14 = val;
            } else if (strcasecmp(key, str_AMBIENT) == 0) {
                FileParser_AssignInt(&parser, str_AMBIENT, &val);
                self->entries[self->count].f18 = val;
            } else if (strcasecmp(key, str_ARCADESFX) == 0) {
                FileParser_AssignInt(&parser, str_ARCADESFX, &val);
                self->entries[self->count].f1C = val;
            } else if (strcasecmp(key, str_ARCADESPEECH) == 0) {
                FileParser_AssignInt(&parser, str_ARCADESPEECH, &val);
                self->entries[self->count].f20 = val;
            } else if (strcasecmp(key, str_TIME_b) == 0) {
                FileParser_AssignInt(&parser, str_TIME_b, &val);
                self->entries[self->count].f24 = (float)val * 0.0010000000474974513f;
            }
        }
        self->count++;
    }
    FileParser__FileParser(&parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_MixIntToFloat);
#ifdef SKIP_ASM
/**
 * Converts a mix value to a level: a percentage (0.01 * value), or for a negative value the current level of the mix
 * control (its final mix value while a mix is fading, else mMixLevel).
 *
 * @param i  the mix control
 * @param pct  the mix value (percent), or negative to keep the control's level
 * @return the level
 */
extern "C" float cSSXAudio_MixIntToFloat(cSSXAudio* self, int i, int pct)
{
    if (pct >= 0) {
        return pct * 0.009999999776482582f;
    }
    if (self->mMixing != 0) {
        return self->mMixFinalValue[i];
    }
    return self->mMixLevel[i];
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_MixVolume);
#ifdef SKIP_ASM
extern "C" float cSSXAudio_MixIntToFloat(cSSXAudio* self, int i, int pct);
struct sAudioFade_87A8;
extern "C" void cSSXAudio_ChangeMixFinalAndStepValues(sAudioFade_87A8* self, int i, int smooth, float target, float time);
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;

struct sSaChan83B0 {
    float base;     // +0x0
    float gain;     // +0x4
    float out;      // +0x8
    float pad;
};

struct sSfxEntry83B0 {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    float f24;
};

struct sSaMixOut83B0 {
    char pad0[0x636C];
    float mix[10];          // 0x636C
};

struct sAudio83B0 {
    char pad0[0x62C0];
    sSaChan83B0 chan[11];   // 0x62C0
    char pad6370[0x63E4 - 0x6370];
    float cur[11];          // 0x63E4
    int faded;              // 0x6410
    char pad6414[0x646C - 0x6414];
    int preset;             // 0x646C
    char pad6470[0x647C - 0x6470];
    sSfxEntry83B0 entries[25];      // 0x647C
};

/**
 * Switches to one of the audio mix presets, at once or with a fade.
 *
 * Ignored when the preset is already current; a negative id counts as 0. The preset's levels for controls 1-4
 * and 6-10 go through MixIntToFloat (cSSXAudio_MixIntToFloat); SFX (5) is not part of a mix. A preset time of 0 applies
 * them at once, otherwise each control fades to its level (ChangeMixFinalAndStepValues, cSSXAudio_ChangeMixFinalAndStepValues).
 *
 * @param vself  the audio system
 * @param id     the mix preset, 0-24 (On Tour Parm)
 */
extern "C" void cSSXAudio_MixVolume(cSSXAudio* vself, int id)
{
    sAudio83B0* self = (sAudio83B0*)vself;
    if (id < 0) {
        id = 0;
    }
    if (id == self->preset) {
        return;
    }
    self->preset = id;
    float musicVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 1, self->entries[id].f0);
    float djVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 2, self->entries[id].f4);
    float paVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 3, self->entries[id].f8);
    float characterVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 4, self->entries[id].fC);
    float boardVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 6, self->entries[id].f10);
    float collisionVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 7, self->entries[id].f14);
    float ambientVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 8, self->entries[id].f18);
    float arcadeSfxVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 9, self->entries[id].f1C);
    float arcadeSpeechVol = cSSXAudio_MixIntToFloat((cSSXAudio*)self, 10, self->entries[id].f20);
    float fadeTime = self->entries[id].f24;
    if (fadeTime == 0.0f) {
        self->cur[1] = musicVol;
        self->cur[2] = djVol;
        self->cur[3] = paVol;
        self->cur[4] = characterVol;
        self->cur[6] = boardVol;
        self->cur[7] = collisionVol;
        self->cur[8] = ambientVol;
        self->cur[9] = arcadeSfxVol;
        self->cur[10] = arcadeSpeechVol;
        for (int k = 0; k < 11; k++) {
            self->chan[k].out = self->chan[k].gain * self->cur[k];
        }
        float characterOut = self->chan[4].out;
        sSaMixOut83B0* mixOut = (sSaMixOut83B0*)self;
        mixOut->mix[0] = characterOut * gMobyScaling;
        mixOut->mix[1] = characterOut * gKaoriScaling;
        mixOut->mix[2] = characterOut * gAerielScaling;
        mixOut->mix[3] = characterOut * gMacScaling;
        mixOut->mix[4] = characterOut * gZoeScaling;
        mixOut->mix[5] = characterOut * gGrommetScaling;
        mixOut->mix[6] = characterOut * gEliseScaling;
        mixOut->mix[7] = characterOut * gRocoScaling;
        mixOut->mix[8] = characterOut * gPsymonScaling;
        mixOut->mix[9] = characterOut * gDeiterScaling;
    } else {
        sAudioFade_87A8* fader = (sAudioFade_87A8*)self;
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 1, self->entries[id].f0 != -1, musicVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 2, self->entries[id].f4 != -1, djVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 3, self->entries[id].f8 != -1, paVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 4, self->entries[id].fC != -1, characterVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 6, self->entries[id].f10 != -1, boardVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 7, self->entries[id].f14 != -1, collisionVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 8, self->entries[id].f18 != -1, ambientVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 9, self->entries[id].f1C != -1, arcadeSfxVol, fadeTime);
        cSSXAudio_ChangeMixFinalAndStepValues(fader, 10, self->entries[id].f20 != -1, arcadeSpeechVol, fadeTime);
        self->faded = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ChangeMixFinalAndStepValues);
#ifdef SKIP_ASM
struct sAudioFade_87A8 {
    char pad[0x63E4];
    float cur[12];     // 0x63E4
    float rate[11];    // 0x6414
    float target[11];  // 0x6440
};

/**
 * Sets a mix control's final value and, when asked, its per-frame step to reach it in a given time (at 60 fps).
 *
 * A target equal to the current level clears the step.
 *
 * @param i  the mix control
 * @param smooth  nonzero to compute the step
 * @param target  the final level
 * @param time  the fade time in seconds
 */
extern "C" void cSSXAudio_ChangeMixFinalAndStepValues(sAudioFade_87A8* self, int i, int smooth, float target, float time)
{
    self->target[i] = target;
    float cur = self->cur[i];
    if (target != cur) {
        if (smooth != 0) {
            self->rate[i] = (target - cur) / time * 0.01666666753590107f;
        }
    } else {
        self->rate[i] = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_UpdateMix);
#ifdef SKIP_ASM
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;

struct sSaMix87F8 {
    char pad0[0x62C4];
    float chan[11][4];          // 0x62C4 (gain, out, -, -)
    char pad6374[0x63E4 - 0x6374];
    float cur[11];              // 0x63E4
    int dirty;                  // 0x6410
    float delta[11];            // 0x6414
    float target[11];           // 0x6440
};


/**
 * Steps the mix levels toward their final values and re-applies the volumes, while a mix is fading.
 *
 * Only while mMixing is set (cleared first, set again while a control still moves). Each of the 11 controls with a step
 * moves mMixLevel by mMixStep, stopping at mMixFinalValue, and gets appliedVolume = duckedVolume * mMixLevel. Then the
 * ten character levels unk_636C are the Character control's applied volume times the constants gMobyScaling ..
 * gDeiterScaling.
 */
extern "C" void cSSXAudio_UpdateMix(sSaMix87F8* self)
{
    if (self->dirty == 0) return;
    self->dirty = 0;
    for (int i = 0; i < 11; i++) {
        if (self->delta[i] == 0.0f) continue;
        float mixValue = self->cur[i] + self->delta[i];
        self->cur[i] = mixValue;
        if (self->delta[i] < 0.0f) {
            if (mixValue <= self->target[i]) {
                self->cur[i] = self->target[i];
                self->delta[i] = 0.0f;
            } else {
                self->dirty = 1;
            }
        } else {
            if (self->target[i] <= mixValue) {
                self->cur[i] = self->target[i];
                self->delta[i] = 0.0f;
            } else {
                self->dirty = 1;
            }
        }
        self->chan[i][1] = self->chan[i][0] * self->cur[i];
    }
    cSSXAudio* mixOut = (cSSXAudio*)self;
    float charLevel = mixOut->mSoundVolume[4].appliedVolume;
    mixOut->unk_636C[0] = charLevel * gMobyScaling;
    mixOut->unk_636C[1] = charLevel * gKaoriScaling;
    mixOut->unk_636C[2] = charLevel * gAerielScaling;
    mixOut->unk_636C[3] = charLevel * gMacScaling;
    mixOut->unk_636C[4] = charLevel * gZoeScaling;
    mixOut->unk_636C[5] = charLevel * gGrommetScaling;
    mixOut->unk_636C[6] = charLevel * gEliseScaling;
    mixOut->unk_636C[7] = charLevel * gRocoScaling;
    mixOut->unk_636C[8] = charLevel * gPsymonScaling;
    mixOut->unk_636C[9] = charLevel * gDeiterScaling;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetClosestCamera);
#ifdef SKIP_ASM
#include "fakematch.h"

extern "C" int cSSXAudio_GetNumViews();
void* cSSXAudio_GetCameraPos(void* self, int a1);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector audVu0Sub_8940(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float audVu0Dot_8940(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__ __volatile__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Returns the view (0 or 1) whose camera is nearer a position; 0 with fewer than two views.
 *
 * @param id  the position (t4Vector*)
 * @return the view index
 */
// PORT: second parameter is really an sAudV4_8940*; kept int to agree with cSSXAudio_GetClosestCamera1's declaration.
// FAKEMATCH: register pin `register int r FM_REG("$2")` (local-alloc gives v0 to the dot asm's block-local "=&r"
// temps before the global r; retail keeps r in v0); plain form 94.02% (unpositioned).
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" int cSSXAudio_GetClosestCamera(cSSXAudio* self, int id)
{
    if (cSSXAudio_GetNumViews() < 2) {
        return 0;
    }
    t4Vector offset0 = audVu0Sub_8940(*(t4Vector*)cSSXAudio_GetCameraPos(self, 0), *(t4Vector*)id);
    t4Vector offset1 = audVu0Sub_8940(*(t4Vector*)cSSXAudio_GetCameraPos(self, 1), *(t4Vector*)id);
    register int nearest FM_REG("$2") = 0;
    if (!(audVu0Dot_8940(offset0, offset0) < audVu0Dot_8940(offset1, offset1))) {
        nearest = 1;
    }
    return nearest;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetClosestCamera1);
#ifdef SKIP_ASM
extern "C" int cAudioInterface_isInGame(void);
extern "C" int cSSXAudio_GetClosestCamera(cSSXAudio* self, int id);
// PORT: callers pass self, but cSSXAudio_GetNumViews's body takes no arguments (the unit declares it ()).
int cSSXAudio_GetNumViews_self(void* self) __asm__("cSSXAudio_GetNumViews");

struct sAudVtEnt { short delta; short index; int (*fn)(void*); };

/**
 * Returns the view a rider's sounds play on: its own view for a local human, else the view nearer its position.
 *
 * Outside the game 0; with one view 0; otherwise cSSXAudio_GetClosestCamera of the rider's position (cRider vtable slot 5).
 *
 * @param obj  the rider (cRider)
 * @return the view index
 */
extern "C" int cSSXAudio_GetClosestCamera1(cSSXAudio* self, cRider* obj)
{
    if (cAudioInterface_isInGame() == 0) {
        return 0;
    }
    bool ok = obj->mIsHuman && obj->mIsLocal;
    if (ok) {
        return obj->mViewIndex;
    }
    if (cSSXAudio_GetNumViews_self(self) == 1) {
        return 0;
    }
    char* being = (char*)&obj->mVtbl;
    sAudVtEnt* entry = &(*(sAudVtEnt**)being)[5];
    return cSSXAudio_GetClosestCamera(self, entry->fn(vthis(being, *entry)));
}
#endif

extern "C" void* cAudioInterface_getGameView(int);

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCameraPos__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the camera position of one game view.
 *
 * cAudioInterface_getGameView returns the game view; its position is at +0x20.
 *
 * @param viewport  the view index (On Tour viewport)
 * @return pointer to the view's position (On Tour const t4Vector&)
 */
void* cSSXAudio_GetCameraPos(void* self, int viewport)
{
    return (char*)cAudioInterface_getGameView(viewport) + 0x20;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ReplayMode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cAudioInterface_isInGame(void);
extern "C" cGame* cAudioInterface_getGame();

/**
 * Returns whether the game is playing back a replay.
 *
 * True when cAudioInterface_isInGame (in game) is set and the game's replay object (+0x28) exists with a non-zero state below 10.
 *
 * @return 1 during replay playback, else 0
 */
extern "C" int cSSXAudio_ReplayMode(cSSXAudio* self)
{
    if (cAudioInterface_isInGame() != 0) {
        if ((int*)cAudioInterface_getGame()->Replay != 0) {
            int replayState = *(int*)cAudioInterface_getGame()->Replay;
            int busy = replayState != 0 && replayState < 10;
            if (busy) {
                return 1;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_CameraInRange);
#ifdef SKIP_ASM
struct sAudV4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

void* cSSXAudio_GetCameraPos(void* self, int a1);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sAudV4 audVu0Sub(const sAudV4& a, const sAudV4& b)
{
    sAudV4 r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float audVu0Dot(const sAudV4& a, const sAudV4& b)
{
    float r;
    int t;
    __asm__ __volatile__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Returns the first game view whose camera is within a distance of a position.
 *
 * The distance is scaled by 100 and compared squared against the VU0 dot product of (camera - @p pos) for each view
 * (cSSXAudio_GetNumViews views, positions from cSSXAudio_GetCameraPos__FPvi).
 *
 * @param pos  the world position (On Tour Pos)
 * @param radius  the distance, in units of 100 (On Tour Dist)
 * @return the view index, or -1 when no camera is in range
 */
extern "C" int cSSXAudio_CameraInRange(cSSXAudio* self, sAudV4* pos, float radius)
{
    float rangeSq = radius * 100.0f;
    rangeSq = rangeSq * rangeSq;
    int i;
    for (i = 0; i < cSSXAudio_GetNumViews_self(self); i++) {
        sAudV4 toCamera = audVu0Sub(*(sAudV4*)cSSXAudio_GetCameraPos(self, i), *pos);
        if (audVu0Dot(toCamera, toCamera) < rangeSq) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InCameraView);
#ifdef SKIP_ASM
void* cSSXAudio_GetCameraPos(void* self, int a1);

/**
 * Tells whether a position lies within a radius (in metres) of any view's camera and in front of it (z above 0).
 *
 * cSSXAudio_RiderCollision uses it.
 *
 * @param pos  the position
 * @param radius  the radius in metres (scaled by 100)
 * @return 1 or 0
 */
// PORT: PS2-only VU0 inline asm (vector subtract).
// PORT: PS2-only VU0 inline asm (4-component dot product).
extern "C" int cSSXAudio_InCameraView(cSSXAudio* self, sAudV4* pos, float radius)
{
    float radiusSq = radius * 100.0f;
    radiusSq = radiusSq * radiusSq;
    int i;
    for (i = 0; i < cSSXAudio_GetNumViews_self(self); i++) {
        sAudV4 offset = audVu0Sub(*(sAudV4*)cSSXAudio_GetCameraPos(self, i), *pos);
        if (audVu0Dot(offset, offset) < radiusSq && 0.0f < offset.z) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetNumViews);
#ifdef SKIP_ASM
extern "C" cSSXApp* cAudioInterface_getApp();
extern "C" int cAudioInterface_getNumGameView();

/**
 * Returns the number of game views (viewports) the audio has to consider.
 *
 * When the app (cAudioInterface_getApp) has its multi-view flag at +0x84 set, returns cAudioInterface_getNumGameView's view count, else 1.
 *
 * @return the number of views
 */
extern "C" int cSSXAudio_GetNumViews()
{
    if ((int)cAudioInterface_getApp()->Game != 0) {
        return cAudioInterface_getNumGameView();
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_InitSoundLevels);
#ifdef SKIP_ASM
extern float gSFXScaling;
extern float gMusicScaling;
extern float gCharSpchScaling;
extern float gDJSpchScaling;
extern float gPASpchScaling;
extern float gBoardScaling;
extern float gCollisionScaling;
extern float gAmbientScaling;
extern float gArcadeSFXScaling;
extern float gArcadeSpeechScaling;
extern float gMobyScaling;
extern float gKaoriScaling;
extern float gAerielScaling;
extern float gMacScaling;
extern float gZoeScaling;
extern float gGrommetScaling;
extern float gEliseScaling;
extern float gRocoScaling;
extern float gPsymonScaling;
extern float gDeiterScaling;
extern "C" void cSSXAudio_SetCurrVolume(cSSXAudio*, float, int);
extern "C" float cSSXAudio_GetCurrSetVolume(void* self, int a1);

/**
 * Sets the fixed level scales for the front end or the mountain and reapplies every volume control.
 *
 * Mode 0 (front end) sets all of them to 1 except one at 0.6; mode 1 (mountain) sets the in-game balance.
 * Each of the eleven controls is then set again to its current value (GetCurrSetVolume, SetCurrVolume).
 *
 * @param mode  0 front end, 1 mountain (On Tour eGameAudioStates)
 */
extern "C" void cSSXAudio_InitSoundLevels(cSSXAudio* self, int mode)
{
    if (mode == 0) {
        gSFXScaling = 1.0f;
        gMusicScaling = 1.0f;
        gCharSpchScaling = 0.5999999642372131f;
        gDJSpchScaling = 1.0f;
        gPASpchScaling = 1.0f;
        gBoardScaling = 1.0f;
        gCollisionScaling = 1.0f;
        gAmbientScaling = 1.0f;
        gArcadeSFXScaling = 1.0f;
        gArcadeSpeechScaling = 1.0f;
        gMobyScaling = 1.0f;
        gKaoriScaling = 1.0f;
        gAerielScaling = 1.0f;
        gMacScaling = 1.0f;
        gZoeScaling = 1.0f;
        gGrommetScaling = 1.0f;
        gEliseScaling = 1.0f;
        gRocoScaling = 1.0f;
        gPsymonScaling = 1.0f;
        gDeiterScaling = 1.0f;
    } else if (mode == 1) {
        gSFXScaling = 0.7999999523162842f;
        gMusicScaling = 1.0f;
        gCharSpchScaling = 0.7999999523162842f;
        gDJSpchScaling = 0.550000011920929f;
        gPASpchScaling = 0.429999977350235f;
        gBoardScaling = 0.75f;
        gCollisionScaling = 0.7999999523162842f;
        gAmbientScaling = 0.5f;
        gArcadeSFXScaling = 0.75f;
        gArcadeSpeechScaling = 0.75f;
        gMobyScaling = 0.8999999761581421f;
        gKaoriScaling = 0.8999999761581421f;
        gAerielScaling = 0.8999999761581421f;
        gMacScaling = 0.8999999761581421f;
        gZoeScaling = 0.7999999523162842f;
        gGrommetScaling = 0.8499999642372131f;
        gEliseScaling = 0.8999999761581421f;
        gRocoScaling = 1.0f;
        gPsymonScaling = 1.0f;
        gDeiterScaling = 0.949999988079071f;
    }
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 0), 0);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 1), 1);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 2), 2);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 3), 3);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 4), 4);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 5), 5);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 6), 6);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 7), 7);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 8), 8);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 9), 9);
    cSSXAudio_SetCurrVolume(self, cSSXAudio_GetCurrSetVolume(self, 10), 10);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_SetPreset);
#ifdef SKIP_ASM
// PORT: cSSXAudio_SetCurrVolume's real parameter order is (self, int, float); the unit declares (self, float, int).
void cSSXAudio_SetCurrVolume_if(void*, int, float) __asm__("cSSXAudio_SetCurrVolume");

/**
 * Applies a sound preset: stores it in mPreset and sets the ten volume controls and the arcade sounds for it.
 *
 * Preset 0 sets controls 1..10 to 10/11 with arcade on; 1 silences speech (2, 3, 4, 9, 10); 2 silences the music (1); 3
 * keeps only the effects (5..8); 4 keeps only the music; 5 silences all; 4 and 5 turn the arcade sounds off
 * (cSSXAudio_SetArcadeSFX).
 *
 * @param mode  the preset, 0..5
 */
extern "C" void cSSXAudio_SetPreset(cSSXAudio* self, int mode)
{
    self->mPreset = mode;
    switch (mode) {
    case 0:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.9090909361839294f);
        cSSXAudio_SetArcadeSFX(self, 1);
        break;
    case 1:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.0f);
        cSSXAudio_SetArcadeSFX(self, 1);
        break;
    case 2:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.9090909361839294f);
        cSSXAudio_SetArcadeSFX(self, 1);
        break;
    case 3:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.0f);
        cSSXAudio_SetArcadeSFX(self, 1);
        break;
    case 4:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.9090909361839294f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.0f);
        cSSXAudio_SetArcadeSFX(self, 0);
        break;
    case 5:
        cSSXAudio_SetCurrVolume_if(self, 1, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 5, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 2, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 3, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 4, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 6, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 7, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 8, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 9, 0.0f);
        cSSXAudio_SetCurrVolume_if(self, 10, 0.0f);
        cSSXAudio_SetArcadeSFX(self, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PrevPreset);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SetPreset(cSSXAudio* self, int mode);

/**
 * Steps to the previous sound preset (wrapping 0 to 5) and applies it (cSSXAudio_SetPreset).
 */
extern "C" void cSSXAudio_PrevPreset(cSSXAudio* self)
{
    int cur = self->mPreset;
    if (cur == 0) {
        self->mPreset = 5;
    } else {
        self->mPreset = cur - 1;
    }
    cSSXAudio_SetPreset(self, self->mPreset);
}
#endif
//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_NextPreset);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_SetPreset(cSSXAudio* self, int mode);

/**
 * Steps to the next sound preset (wrapping 5 to 0) and applies it (cSSXAudio_SetPreset).
 */
extern "C" void cSSXAudio_NextPreset(cSSXAudio* self)
{
    int n = self->mPreset + 1;
    self->mPreset = n;
    if (n == 6) {
        self->mPreset = 0;
    }
    cSSXAudio_SetPreset(self, self->mPreset);
}
#endif
//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_DelayMonitorTriggers);
#ifdef SKIP_ASM
struct sSndKey8_9400 {
    long v;
} __attribute__((packed));
extern const sSndKey8_9400 D_004A3600[];
extern "C" void cMonitor_MONITOR_Create(void*, int, long, int, int, int, int, int);

/**
 * Registers a 250 ms callback (D_004A3600: cSSXAudio_HandleDelayMonitorTriggers) with the bank monitor through cMonitor_MONITOR_Create.
 *
 * cMonitor_MONITOR_Create is probably MONITOR_Create; this probably holds back the monitor triggers for that time.
 */
// PORT: 64-bit long sound key
extern "C" void cSSXAudio_DelayMonitorTriggers(cSSXAudio* self)
{
    cMonitor_MONITOR_Create(*(char**)self->MusicSys.mpVStreamSys + 0x1D8, 250, D_004A3600->v, 0, 0, 0, 0, 0);
}
#endif
//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_HandleDelayMonitorTriggers);
#ifdef SKIP_ASM
extern "C" void cWorldTriggerManager_MonitorTriggers(int, int, int);

/**
 * Monitor callback: re-enables the world triggers (cWorldTriggerManager_MonitorTriggers(gWorldTriggerManager, 1, 0)); returns 1.
 *
 * cSSXAudio_DelayMonitorTriggers installs it (D_004A3600) as a 250 ms monitor.
 *
 * @return 1
 */
extern "C" int cSSXAudio_HandleDelayMonitorTriggers(void) {
    cWorldTriggerManager_MonitorTriggers(*(int*)&gWorldTriggerManager, 1, 0);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289470);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(char* name);
extern "C" void FileParser_FileParser(void* self, char* name, int size);
extern "C" char* FileParser_FindNextSection(void* self);
extern "C" void FileParser__FileParser(void* self, int flags);
extern "C" char* strcpy(char* dst, const char* src);
extern char* str_x_H;

/**
 * Reads the play-list names: up to 20 section names of data/config/playlist.inf into the table, counted in
 * mNumPlayLists (0 when the file is missing).
 *
 * The cSSXAudio constructor calls it.
 */
extern "C" void func_00289470(cSSXAudio* self)
{
    if (BXFILE_exists(str_x_H) != 0) {
        char parser[2064];
        FileParser_FileParser(parser, str_x_H, 0x100);
        self->mNumPlayLists = 0;
        while (self->mNumPlayLists < 20) {
            char* section = FileParser_FindNextSection(parser);
            if (section == 0) {
                break;
            }
            strcpy((self)->mPlayListName[self->mNumPlayLists], section);
            self->mNumPlayLists = self->mNumPlayLists + 1;
        }
        FileParser__FileParser(parser, 2);
        return;
    }
    self->mNumPlayLists = 0;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", func_00289520);
#ifdef SKIP_ASM
extern "C" void FileParser_FileParser(void* self, char* name, int size);
extern "C" void FileParser__FileParser(void* self, int flags);
extern "C" int FileParser_FindPartialSection(void* self, const char* section);
extern "C" void FileParser_SkipSectionNames(void* self);
extern "C" int FileParser_IsEOF(void* self);
extern "C" char* FileParser_GetName(void* self);
extern "C" void FileParser_AssignString(void* self, const char* key, char* out);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" void func_002B3EB0(void* self);
extern "C" void func_002B3EE8(void* self, const char* name);
void func_002B4060(void* self);
extern char* str_x_H;
extern char str_SONG[];

/**
 * Selects a play list by name: loads its SONG entries into the music system and records its index.
 *
 * Clears the music system's song list (func_002B3EB0), reads the section of data/config/playlist.inf that matches
 * @p name and adds each SONG (func_002B3EE8), sets mPlayListIndex to the index of @p name in the play-list table (20
 * when absent), finishes the list (func_002B4060) and copies MusicSys.mActiveMap into +0x518. The constructor and the
 * debug menu call it.
 *
 * @param name  the play list (section) name
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern "C" void func_00289520(cSSXAudio* self, const char* name)
{
    char parser[2064];
    char buf[0x40];
    func_002B3EB0((char*)&self->MusicSys);
    FileParser_FileParser(parser, str_x_H, 0x100);
    if (FileParser_FindPartialSection(parser, name) != 0) {
        FileParser_SkipSectionNames(parser);
        while (FileParser_IsEOF(parser) == 0) {
            char* key = FileParser_GetName(parser);
            if (strcasecmp(str_SONG, key) == 0) {
                FileParser_AssignString(parser, str_SONG, buf);
                func_002B3EE8((char*)&self->MusicSys, buf);
            }
        }
    }
    self->mPlayListIndex = 20;
    for (int i = 0; i < 20; i++) {
        if (func_004165A8((self)->mPlayListName[i], name) == 0) {
            self->mPlayListIndex = i;
            break;
        }
    }
    func_002B4060((char*)&self->MusicSys);
    *(long*)&self->MusicSys.mDefaultActiveMap = (long)self->MusicSys.mActiveMap;
    FileParser__FileParser(parser, 2);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_HandleUserChunk);
#ifdef SKIP_ASM
// PORT: cSSXAudio_HandleStreamChunk is defined (void*) but this caller passes (system, msg).
void cSSXAudio_HandleStreamChunk_2(void* self, void* msg) __asm__("cSSXAudio_HandleStreamChunk__FPv");

/**
 * Sound-driver user-data callback: passes a stream chunk of type 3 to the audio system (cSSXAudio_HandleStreamChunk on gSSXAudio).
 *
 * @param msg  the user chunk (SNDUSERDATACBINFO; its first word is the type)
 */
extern "C" void cSSXAudio_HandleUserChunk(void* msg)
{
    if (*(int*)msg == 3) {
        cSSXAudio_HandleStreamChunk_2((char*)gSSXAudio, msg);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_HandleStreamChunk__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: handles a stream user-data chunk passed on by cSSXAudio_HandleUserChunk.
 */
void cSSXAudio_HandleStreamChunk(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ApplyRMSData);
#ifdef SKIP_ASM
extern "C" int func_0029F088(void* self, int a1);
extern "C" void cSSXAudio_DuckByRMS(cSSXAudio* self, float v);


/**
 * Ducks the music to the loudness (RMS) data of the current stream, if it has valid RMS data.
 *
 * Every second frame it reads the RMS byte for the current time and picks a target level. In state 2 of
 * func_0029F088 a value of 30 or more gives level 0; otherwise the level is 1. The level goes to DuckByRMS
 * (cSSXAudio_DuckByRMS) right away. The ramp branch only runs if something else sets the step counter negative.
 *
 * @param self_  the audio system
 */
extern "C" void cSSXAudio_ApplyRMSData(cSSXAudio* self_)
{
    cSSXAudio* audio = self_;
    if (audio->mValidRMSData != 0) {
        int frame = audio->mRMSFrameTick;
        if (frame == (frame / 2) * 2) {
            audio->mRMSStepFrame = 0;
            audio->mRMSCurValue = audio->mRMSFinalValue;
            int idx = (int)((float)(frame * audio->mRMSDataFrameRate) / cAppMan_TheAppMan->FrameRate);
            if (idx < audio->mRMSSize) {
                int val = *(signed char*)(((char*)self_) + idx + 0x6868);
                if (val > 0) {
                    int lowThreshold, midThreshold, highThreshold;
                    float vb, va;
                    switch (func_0029F088(((char*)self_), 0)) {
                    case 2:
                        lowThreshold = 0;
                        midThreshold = 0;
                        highThreshold = 30;
                        vb = 1.0f;
                        va = 0.0f;
                        break;
                    case 3:
                        va = 1.0f;
                        lowThreshold = 30;
                        midThreshold = 40;
                        highThreshold = 50;
                        vb = 1.0f;
                        break;
                    default:
                        va = 1.0f;
                        lowThreshold = 30;
                        midThreshold = 40;
                        highThreshold = 50;
                        vb = 1.0f;
                        break;
                    }
                    if (val < lowThreshold)
                        audio->mRMSFinalValue = 1.0f;
                    else if (val < midThreshold)
                        audio->mRMSFinalValue = vb;
                    else if (val < highThreshold)
                        audio->mRMSFinalValue = vb;
                    else
                        audio->mRMSFinalValue = va;
                } else {
                    audio->mRMSFinalValue = 1.0f;
                }
            } else {
                audio->mValidRMSData = 0;
                audio->mRMSFinalValue = 1.0f;
            }
            audio->mRMSStepValue = audio->mRMSFinalValue - audio->mRMSCurValue;
        }
        (audio->mRMSFrameTick)++;
    }
    int stepFrame = audio->mRMSStepFrame;
    if (stepFrame <= 0) {
        if (stepFrame == 0) {
            cSSXAudio_DuckByRMS((cSSXAudio*)(((char*)self_)), audio->mRMSFinalValue);
            (audio->mRMSStepFrame)++;
        } else {
            audio->mRMSCurValue = audio->mRMSCurValue + audio->mRMSStepValue;
            cSSXAudio_DuckByRMS((cSSXAudio*)(((char*)self_)), audio->mRMSCurValue);
            (audio->mRMSStepFrame)++;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_DuckByRMS);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_DuckVolume(cSSXAudio* self, int a1, float a2, float a3);

/**
 * Ducks the music by the stream's RMS level: when the level differs from mLastRMSDuckLevel, cSSXAudio_DuckVolume(1,
 * level, 0) and record it.
 *
 * @param duckLevel  the duck level
 */
extern "C" void cSSXAudio_DuckByRMS(cSSXAudio* self, float duckLevel)
{
    if (self->mLastRMSDuckLevel != duckLevel) {
        cSSXAudio_DuckVolume((cSSXAudio*)self, 1, duckLevel, 0.0f);
        self->mLastRMSDuckLevel = duckLevel;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_CancelRMSData);
#ifdef SKIP_ASM
/**
 * Cancels the RMS ducking data: mValidRMSData = 0, the final value back to 1.0, the step frame 0 and the step 1.0 -
 * mRMSCurValue.
 *
 * The retail image has no call or data reference to it.
 *
 * @param audio  the audio system
 */
extern "C" void cSSXAudio_CancelRMSData(cSSXAudio* audio) {
    (audio->mValidRMSData) = 0;
    (audio->mRMSFinalValue) = 1.0f;
    (audio->mRMSStepFrame) = 0;
    (audio->mRMSStepValue) = (float) (1.0f - (audio->mRMSCurValue));
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_UpdateAudioWorldPainter);
#ifdef SKIP_ASM
extern "C" int func_0028D960(void* self);
extern "C" void func_0028D988(void* self, void* rider, void* obj);
extern "C" int cSSXAudio_NeedWPAmbience(cSSXAudio* self);
extern "C" void cSSXAudio_ReactToWPAmbienceEvent(cSSXAudio* self, cRider* rider, tWPInGameData* obj);
extern "C" void cSSXAudio_ReactToWPMixEvent(cSSXAudio* self, int id, tWPInGameData* obj);


struct sSaObjs98A8 {
    char pad0[0x5FA0];
    char* c[2];                 // 0x5FA0
    char pad5FA8[0x625C - 0x5FA8];
    char* a[2];                 // 0x625C
    char pad6264[0x6C80 - 0x6264];
    char* b[2];                 // 0x6C80
};

/**
 * Applies the world painter's audio data under a player rider: mix changes and ambience.
 *
 * Only the two players (rider index +0x870 below 2) count. The painter queries for that player are evaluated
 * at the rider's x/y. When func_0028D960 allows it, two results go to func_0028D988 and to ReactToWPMixEvent.
 * When ambience is needed, a third result goes to ReactToWPAmbienceEvent (cSSXAudio_ReactToWPAmbienceEvent).
 *
 * @param rider  the rider (On Tour cRider*)
 */
extern "C" void cSSXAudio_UpdateAudioWorldPainter(cSSXAudio* self, cRider* rider)
{
    unsigned int idx = (unsigned int)rider->mViewIndex;
    if (idx >= 2) return;
    t4Vector pos;
    {
        char* being = (char*)&rider->mVtbl;
        sVEntry<t4Vector*(void*)>* vt = *(sVEntry<t4Vector*(void*)>**)being;
        pos = *vt[5].fn(vthis(being, vt[5]));
    }
    if (func_0028D960(self) != 0) {
        float k = -99999.0f;
        char* painterQuery = ((sSaObjs98A8*)self)->a[idx];
        sVEntry<void*(void*, float, float, float)>* va = *(sVEntry<void*(void*, float, float, float)>**)(painterQuery + 4);
        func_0028D988(self, ((char*)rider), va[2].fn(vthis(painterQuery, va[2]), pos.x, pos.y, k));
        char* mixQuery = ((sSaObjs98A8*)self)->b[idx];
        sVEntry<void*(void*, float, float, float)>* vb = *(sVEntry<void*(void*, float, float, float)>**)(mixQuery + 4);
        // PORT: cSSXAudio_ReactToWPMixEvent's unit declaration takes the rider as int
        cSSXAudio_ReactToWPMixEvent(self, (int)rider, (tWPInGameData*)vb[2].fn(vthis(mixQuery, vb[2]), pos.x, pos.y, k));
    }
    if (cSSXAudio_NeedWPAmbience(self) != 0) {
        char* ambienceQuery = ((sSaObjs98A8*)self)->c[idx];
        sVEntry<void*(void*, float, float, float)>* vc = *(sVEntry<void*(void*, float, float, float)>**)(ambienceQuery + 4);
        cSSXAudio_ReactToWPAmbienceEvent(self, rider, (tWPInGameData*)vc[2].fn(vthis(ambienceQuery, vc[2]), pos.x, pos.y, -99999.0f));
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_NeedWPAmbience);
#ifdef SKIP_ASM
/**
 * Tells whether the world painter's ambience should play: when unk_608C is 2 (probably a radio mode).
 *
 * @return 1 when it is needed, otherwise 0
 */
extern "C" int cSSXAudio_NeedWPAmbience(cSSXAudio* self)
{
    return self->unk_608C == 2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ReactToWPAmbienceEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0028CF98(void* self, int a1, int a2, int a3, int a4);
extern "C" int func_002B49E0(void* monitor);
// cSSXAudio_GetFocusRider returns a rider pointer here (the unit declares it int).
void* cSSXAudio_GetFocusRider_p(void* self, int which) __asm__("cSSXAudio_GetFocusRider");


/**
 * Reacts to a world-painter music event for the focus rider: switches the song or the music track.
 *
 * Only for the focus rider (cSSXAudio_GetFocusRider(self, -1)) and with data. The id from the data's vtable slot 4: -1
 * nothing; 0x1F the music state 101 (func_0028CF98(.., 1, ..)) unless already there; 0x20 state 102 likewise; 33
 * nothing; any other id goes to the music system's vtable slot 4 (set track). cSSXAudio_UpdateAudioWorldPainter calls
 * it.
 *
 * @param rider  the rider
 * @param obj  the world-painter data (tWPInGameData)
 */
extern "C" void cSSXAudio_ReactToWPAmbienceEvent(cSSXAudio* self, cRider* rider, tWPInGameData* obj)
{
    if (((void*)rider) != cSSXAudio_GetFocusRider_p(self, -1)) return;
    if (((void*)obj) == 0) return;
    sVEntry<int*(void*)>* vt = (sVEntry<int*(void*)>*)obj->mVtbl;
    int id = *vt[4].fn(vthis(obj, vt[4]));
    if (id == -1) return;
    if (id == 0x1F) {
        if (func_002B49E0((char*)&self->MusicSys) != 101) {
            func_0028CF98(self, 0, 0, 1, 0);
        }
    } else if (id == 0x20) {
        if (func_002B49E0((char*)&self->MusicSys) != 102) {
            func_0028CF98(self, 0, 0, 2, 0);
        }
    } else if (id != 33) {
        char* mon = (char*)&self->MusicSys;
        sVEntry<int(void*, int)>* vb = (sVEntry<int(void*, int)>*)self->MusicSys.mVtbl;
        vb[4].fn(vthis(mon, vb[4]), id);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_RiderCreated__FPv);
#ifdef SKIP_ASM
/**
 * Notification that a rider was created; does nothing.
 *
 * Called from cRider_cRider. On Tour's version takes the rider (cRider* pRider) and is empty too.
 */
void cSSXAudio_RiderCreated(void* self)
{
}
#endif

extern "C" void* cSSXAudio_StopRiderSound(void* self);

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_RiderDeleted__FPv);
#ifdef SKIP_ASM
/**
 * Stops a rider's sounds when the rider is destroyed: cSSXAudio_StopRiderSound with the rider passed on in $a1.
 *
 * cRider__cRider calls it.
 *
 * @return the result of cSSXAudio_StopRiderSound (unused)
 */
void* cSSXAudio_RiderDeleted(void* self)
{
    return cSSXAudio_StopRiderSound(self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PlayAniTag);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_BindingsOff(void* a0, void* a1);
extern "C" void cSSXAudio_BindingsOn(void* a0, void* a1);
extern "C" void cSSXAudio_UndoBindings(void* a0, void* a1);

/**
 * Plays an animation's sound tag: 80 cSSXAudio_BindingsOff, 81 cSSXAudio_BindingsOn, 82 cSSXAudio_UndoBindings.
 *
 * @param ssxAudio  the audio system
 * @param rider  the rider
 * @param type  the tag (patch)
 */
extern "C" void cSSXAudio_PlayAniTag(cSSXAudio* ssxAudio, cRider* rider, int type)
{
    if (type == 80) {
        cSSXAudio_BindingsOff(ssxAudio, rider);
    } else if (type == 81) {
        cSSXAudio_BindingsOn(ssxAudio, rider);
    } else if (type == 82) {
        cSSXAudio_UndoBindings(ssxAudio, rider);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PauseAudio);
#ifdef SKIP_ASM
extern "C" void cSSXAudio_PauseSFX(void* self);
extern "C" void func_002B3A70(void*);

/**
 * Pauses the sound effects (PauseSFX) and the stream part at +0x118, unless the audio is already paused.
 */
extern "C" void cSSXAudio_PauseAudio(cSSXAudio* self)
{
    if (self->mAudioPaused == 0) {
        cSSXAudio_PauseSFX(self);
        func_002B3A70((char*)&self->MusicSys);
        self->mAudioPaused = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ResumeAudio);
#ifdef SKIP_ASM
extern "C" void func_002B3A98(void*);
extern "C" void cSSXAudio_ResumeSFX(void*);
extern "C" void func_002B11B0(void*, int);
extern "C" void func_002A4550(void*);

/**
 * Resumes paused audio: the music (func_002B3A98) and the effects (cSSXAudio_ResumeSFX), clearing mAudioPaused.
 *
 * When unk_5828 is set it also stops speech instance 0 (func_002B11B0), clears the pending speech flags (func_002A4550)
 * and unk_5828. cSSXAudio_Update, cSSXAudio_FrontEndLoad, cSSXAudio_InGameDeInit and a game state call it.
 */
extern "C" void cSSXAudio_ResumeAudio(cSSXAudio* self)
{
    if (self->mAudioPaused != 0) {
        func_002B3A98((char*)&self->MusicSys);
        cSSXAudio_ResumeSFX(self);
        self->mAudioPaused = 0;
        if (self->unk_5828 != 0) {
            func_002B11B0((char*)&self->SpeechSys, 0);
            func_002A4550(self);
            self->unk_5828 = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCharacterLevel);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEStatInterface_getStatLevel(void* iface, int id);

struct sSsxAudioVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};

/**
 * Returns a rider's character level from its stat level: below 4 gives 0, below 8 gives 1, else 2.
 *
 * The stat level comes from cBEStatInterface_getStatLevel (BE interface 3) for the rider's AI index. func_0028F000 and
 * func_0028F108 use it.
 *
 * @param src  the rider (cRider)
 * @return 0, 1 or 2
 */
extern "C" int cSSXAudio_GetCharacterLevel(cSSXAudio* self, cRider* src)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 3);
    char* obj = (char*)&src->mVtbl;
    sSsxAudioVEntryI* vt = *(sSsxAudioVEntryI**)obj;
    int n = cBEStatInterface_getStatLevel(iface, vt[7].fn(vthis(obj, vt[7])));
    if (n < 4) {
        return 0;
    }
    if (n < 8) {
        return 1;
    }
    return 2;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_ReactToWPMixEvent);
#ifdef SKIP_ASM
extern "C" int cSSXAudio_GetFocusRider(cSSXAudio* self, int which);
extern "C" void cSSXAudio_MixVolume(cSSXAudio* self, int id);

struct sSsxAudioVEntryP {
    short delta;
    short index;
    int* (*fn)(void*);
};

/**
 * Applies the mix preset from a world-painter result, when it is for the listening rider.
 *
 * The preset is the value that virtual slot 3 of @p obj returns. Nothing happens unless @p id is the rider
 * cSSXAudio_GetFocusRider(self, -1) returns, and @p obj is set.
 *
 * @param id   the rider (On Tour pRider), passed as an int
 * @param obj  the painter data at the rider's position (On Tour const tWPInGameData*), or 0
 */
extern "C" void cSSXAudio_ReactToWPMixEvent(cSSXAudio* self, int id, tWPInGameData* obj)
{
    if (id == cSSXAudio_GetFocusRider(self, -1) && ((void*)obj) != 0) {
        sSsxAudioVEntryP* vt = (sSsxAudioVEntryP*)obj->mVtbl;
        cSSXAudio_MixVolume(self, *vt[3].fn(vthis(obj, vt[3])));
    }
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_BEAudioModeToAudioMode);
#ifdef SKIP_ASM
/**
 * Converts the options' sound mode to the sound driver's mode: 0 gives 1, 1 gives 2, 2 gives 0, 3 gives 3.
 *
 * cSSXApp_initload uses it.
 *
 * @param beMode  the options' sound mode
 * @return the driver mode (eSOUNDDEF_MODE)
 */
extern "C" int cSSXAudio_BEAudioModeToAudioMode(cSSXAudio* self, unsigned char beMode)
{
    int result = 1;
    switch (beMode) {
    case 0:
        break;
    case 1:
        result = 2;
        break;
    case 2:
        result = 0;
        break;
    case 3:
        result = 3;
        break;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_AudioModeToBEAudioMode);
#ifdef SKIP_ASM
/**
 * Converts the sound driver's mode to the options' sound mode: 1 gives 0, 2 gives 1, 0 gives 2, 3 gives 3.
 *
 * @param audioMode  the driver mode (eSOUNDDEF_MODE)
 * @return the options' sound mode
 */
extern "C" void* cSSXAudio_AudioModeToBEAudioMode(cSSXAudio* self, int audioMode)
{
    int result = 0;
    switch (audioMode) {
    case 1:
        break;
    case 2:
        result = 1;
        break;
    case 0:
        result = 2;
        break;
    case 3:
        result = 3;
        break;
    }
    return (void*)result;
}
#endif

extern "C" void* cSSXAudio_AudioModeToBEAudioMode(cSSXAudio*, int);

//99.29%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_GetCurrentSoundMode__FPv);
#ifdef SKIP_ASM
/**
 * Returns the current sound mode for the options: cSSXAudio_AudioModeToBEAudioMode of mAudioMode.
 *
 * cFEStateOptionsSound_updateWidget uses it.
 *
 * @return the options' sound mode
 */
void* cSSXAudio_GetCurrentSoundMode(void* vself)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    return cSSXAudio_AudioModeToBEAudioMode((cSSXAudio*)self, self->mAudioMode);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cSSXAudio_PlayIntroMovieWithDJ__FPv);
#ifdef SKIP_ASM
/**
 * Returns mPlayIntroMovieWithDJ and clears it (a one-shot flag; the constructor sets it).
 *
 * cRealFE_update reads it.
 *
 * @return the previous flag
 */
int cSSXAudio_PlayIntroMovieWithDJ(void* vself)
{
    cSSXAudio* self = (cSSXAudio*)vself;
    int old = self->mPlayIntroMovieWithDJ;
    self->mPlayIntroMovieWithDJ = 0;
    return old;
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cAsyncSys_DoCancel);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_cancel(int h);
extern "C" int ASYNCFILE_release(int h, int a, int b);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(void* self);

struct sRing289DF0 {
    int size;
    int* data;
    int head;
    int tail;
};

struct sAsyncVt289DF0 {
    short delta;
    short index;
    void (*fn)(void*, int, int, int, char*, int, int);
};

struct sAsync289DF0 {
    int handle;            // 0x0
    int busy;              // 0x4
    int slot;              // 0x8
    char names[6][0x40];   // 0xC
    char pad18C[0x18C - 0xC - 6 * 0x40];
    sRing289DF0 q18C;      // 0x18C
    sRing289DF0 q19C;      // 0x19C
    sRing289DF0 q1AC;      // 0x1AC
    sRing289DF0 q1BC;      // 0x1BC
    int pad1CC[2];
    // vptr at 0x1D4
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int id, int a, int mode, char* name, int b, int c);
};

/**
 * Cancels the current asynchronous bank load, optionally the whole queue, then starts the next one.
 *
 * Pops the front entries of the four request queues (user handle, slot, kind, flags). While a load is busy, kinds 0..2
 * cancel and release the async file and report the end through the object's vtable slot 5 (state 1 when the cancel
 * succeeded, else 2, with the slot's file name), clearing the busy flag. With @p flush set the queues are emptied.
 * Finally cAsyncSys_ASYNCSYS_StartLoad. cAsyncSys_ASYNCSYS_Cancel, cAsyncSys_ASYNCSYS_CancelCurrent,
 * cAsyncSys_ASYNCSYS_CancelAll, cAsyncSys_ASYNCSYS_DeInit, cAsyncSys_ASYNCSYS_Update and cSSXAudio_InGamePurge call it.
 *
 * @param flush  nonzero to cancel every queued request too
 */
extern "C" void cAsyncSys_DoCancel(cAsyncSys* self, int flush)
{
    sAsync289DF0* asyncSys = (sAsync289DF0*)self;
    int id = -1;
    {
        sRing289DF0* queue = &asyncSys->q18C;
        if (asyncSys->q18C.head != asyncSys->q18C.tail) {
            int frontValue = queue->data[queue->tail];
            if (++asyncSys->q18C.tail >= asyncSys->q18C.size)
                asyncSys->q18C.tail = 0;
            id = frontValue;
        }
    }
    int slot;
    {
        sRing289DF0* queue = &asyncSys->q19C;
        if (asyncSys->q19C.head == asyncSys->q19C.tail) {
            slot = -1;
        } else {
            int frontValue = queue->data[queue->tail];
            if (++asyncSys->q19C.tail >= asyncSys->q19C.size)
                asyncSys->q19C.tail = 0;
            slot = frontValue;
        }
    }
    int kind;
    {
        sRing289DF0* queue = &asyncSys->q1AC;
        if (asyncSys->q1AC.head == asyncSys->q1AC.tail) {
            kind = -1;
        } else {
            int frontValue = queue->data[queue->tail];
            if (++asyncSys->q1AC.tail >= asyncSys->q1AC.size)
                asyncSys->q1AC.tail = 0;
            kind = frontValue;
        }
    }
    {
        sRing289DF0* queue = &asyncSys->q1BC;
        if (asyncSys->q1BC.head != asyncSys->q1BC.tail) {
            int frontValue = queue->data[queue->tail];
            if (++asyncSys->q1BC.tail >= asyncSys->q1BC.size)
                asyncSys->q1BC.tail = 0;
        }
    }
    if (asyncSys->busy) {
        switch (kind) {
        case 0:
        case 2: {
            int cancelResult = ASYNCFILE_cancel(asyncSys->handle);
            ASYNCFILE_release(asyncSys->handle, 0, 0);
            asyncSys->v05(id, -1, cancelResult == 1 ? 1 : 2, asyncSys->names[slot], 0, 0);
            asyncSys->busy = 0;
            break;
        }
        case 1: {
            int cancelResult = ASYNCFILE_cancel(asyncSys->handle);
            ASYNCFILE_release(asyncSys->handle, 0, 0);
            asyncSys->v05(id, -1, cancelResult == 1 ? 1 : 2, asyncSys->names[slot], 0, 0);
            asyncSys->busy = 0;
            break;
        }
        }
    }
    if (flush) {
        asyncSys->q18C.tail = asyncSys->q18C.head;
        asyncSys->q19C.tail = asyncSys->q19C.head;
        asyncSys->q1AC.tail = asyncSys->q1AC.head;
        asyncSys->q1BC.tail = asyncSys->q1BC.head;
    }
    cAsyncSys_ASYNCSYS_StartLoad(self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cAsyncSys_cAsyncSys);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char cAsyncSys_vtbl[];
extern char str_CircFIFOBuff[];
extern void* gpAsyncSys;

/**
 * Constructs the asynchronous sound-bank loader.
 *
 * Creates four circular FIFOs of 6 entries (each a 0x18-byte "CircFIFOBuff" buffer at +0x190, +0x1A0, +0x1B0, +0x1C0
 * with zeroed read/write indices), clears the other counters, installs the vtable cAsyncSys_vtbl at +0x1D4 and stores
 * itself in gpAsyncSys.
 *
 * @return this
 */
extern "C" void* cAsyncSys_cAsyncSys(cAsyncSys* self)
{
    self->mUserHandle.ArraySize = 6;
    self->mBusy = 0;
    self->mFileIndex = 0;
    self->mUserHandle.Head = 0;
    self->mUserHandle.Tail = 0;
    self->mVtbl = (void**)cAsyncSys_vtbl;
    self->mUserHandle.Array = (int*)operator_new_tag(0x18, str_CircFIFOBuff, 0, 0);
    self->mFilenameIndex.ArraySize = 6;
    self->mFilenameIndex.Head = 0;
    self->mFilenameIndex.Tail = 0;
    self->mFilenameIndex.Array = (int*)operator_new_tag(0x18, str_CircFIFOBuff, 0, 0);
    self->mMemType.ArraySize = 6;
    self->mMemType.Head = 0;
    self->mMemType.Tail = 0;
    self->mMemType.Array = (int*)operator_new_tag(0x18, str_CircFIFOBuff, 0, 0);
    self->mMainMemFlags.ArraySize = 6;
    self->mMainMemFlags.Head = 0;
    self->mMainMemFlags.Tail = 0;
    self->mMainMemFlags.Array = (int*)operator_new_tag(0x18, str_CircFIFOBuff, 0, 0);
    self->mpBuffer = 0;
    self->mBufferSize = 0;
    gpAsyncSys = ((void*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("sound/ssxAudio", cAsyncSys__cAsyncSys);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_ASYNCSYS_DeInit(void*);
void cMemMan_free(void*);
void operator_delete(int*);
extern char cAsyncSys_vtbl[];
extern void* gpAsyncSys;


/**
 * Destroys the asynchronous sound-bank loader.
 *
 * Calls cAsyncSys_ASYNCSYS_DeInit, clears the singleton gpAsyncSys, frees the FIFO buffers at +0x1C0, +0x1B0, +0x1A0
 * and +0x190 and deletes the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ 2.95 in-charge flags; bit 0 deletes the object
 */
extern "C" void cAsyncSys__cAsyncSys(cAsyncSys* self, int flags)
{
    self->mVtbl = (void**)cAsyncSys_vtbl;
    cAsyncSys_ASYNCSYS_DeInit((self));
    gpAsyncSys = 0;
    if (self->mMainMemFlags.Array != 0) {
        cMemMan_free(self->mMainMemFlags.Array);
    }
    if (self->mMemType.Array != 0) {
        cMemMan_free(self->mMemType.Array);
    }
    if (self->mFilenameIndex.Array != 0) {
        cMemMan_free(self->mFilenameIndex.Array);
    }
    if (self->mUserHandle.Array != 0) {
        cMemMan_free(self->mUserHandle.Array);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

