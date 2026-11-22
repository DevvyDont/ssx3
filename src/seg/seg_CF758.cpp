#include "common.h"
#include "vtable.h"
#include "ui/uiengine.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("seg/seg_CF758", setChallengeName);
#ifdef SKIP_ASM
struct cUIText;
int GetHashValue32(char* str);
extern "C" void* cBE_getBE();
void* cBE_getInterface_CE758(void* be, int kind) __asm__("cBE_getInterface__Fv");
void cUIText_setAsciiString(cUIText* text, const char* str);
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern "C" const char* cBENewRaceInterface_getLocationName(void* iface, int track);
extern "C" void func_002C2540(void* dst, void* src);
// PORT: sprintf1 is a varargs formatter (dst, fmt, ...)
extern "C" void sprintf1_CE758(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void cUIText_setUnicodeString(cUIText* text, void* str);

struct sReq_CE758
{
    signed char type;
    signed char track;
};
extern sReq_CE758 D_0045AAD8_CE758[][4][5] __asm__("gMedalsInfo");

extern char str_kT_OVRCMNEarnings[];
extern char str_kT_OVRCMNCollectibles[];
extern char str_kT_TITLEBigChallenges[];
extern char str_Happiness_pct_s[];
extern char str_Ruthless_pct_s[];
extern char str_Throne_pct_s[];
extern char str_kT_cmnRaceCaps[];
extern char str_Happiness_Jam[];
extern char str_Ruthless_Jam[];
extern char str_Throne_Jam[];
extern char str_kT_EventPk1Race[];
extern char str_kT_EventPk2Race[];
extern char str_kT_EventAllPeakRace[];
extern char str_kT_EventPk1Jam[];
extern char str_kT_EventPk2Jam[];
extern char str_kT_EventAllPeakJam[];

struct sVEntry_CE758 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_CE758(char* key)
{
    char* obj = (char*)SSXApp->FELocale;
    sVEntry_CE758* vt = *(sVEntry_CE758**)(obj + 4);
    return vt[4].fn(vthis(obj, vt[4]), GetHashValue32(key));
}

/**
 * Sets a text widget to the name of one entry of a peak's challenge table (gMedalsInfo).
 *
 * Kind 3 gives "kT_OVRCMNEarnings"; kind 2 gives "kT_OVRCMNCollectibles" for entry 0 and
 * "kT_TITLEBigChallenges" otherwise. Other kinds look up the entry's type: 4 is the peak's
 * "Happiness/Ruthless/Throne %s" with the localised "kT_cmnRaceCaps", 5 the peak's "... Jam", 6 to 11 the peak
 * race and jam event names ("kT_EventPk1Race" and so on), and any other type the name of the entry's location.
 *
 * @param text  the text widget to set
 * @param peak  the peak, 0 to 2
 * @param kind  the table group, 0 to 3
 * @param idx   the entry within the group, 0 to 4
 */
extern "C" void setChallengeName(cUIText* text, int peak, int kind, int idx)
{
    if (kind == 3) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNEarnings));
        return;
    }
    if (kind == 2) {
        if (idx == 0) {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_OVRCMNCollectibles));
        } else {
            cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_TITLEBigChallenges));
        }
        return;
    }
    void* iface = cBE_getInterface_CE758(cBE_getBE(), 0);
    int type = D_0045AAD8_CE758[peak][kind][idx].type;
    int track = D_0045AAD8_CE758[peak][kind][idx].track;
    if (type == 4) {
        unsigned short buf[200];
        unsigned short fmt[32];
        if (peak == 0) {
            func_002C2540(fmt, str_Happiness_pct_s);
        } else if (peak == 1) {
            func_002C2540(fmt, str_Ruthless_pct_s);
        } else if (peak == 2) {
            func_002C2540(fmt, str_Throne_pct_s);
        }
        sprintf1_CE758(buf, fmt, Loc_CE758(str_kT_cmnRaceCaps));
        cUIText_setUnicodeString(text, buf);
    } else if (type == 5) {
        if (peak == 0) {
            cUIText_setAsciiString(text, str_Happiness_Jam);
        } else if (peak == 1) {
            cUIText_setAsciiString(text, str_Ruthless_Jam);
        } else if (peak == 2) {
            cUIText_setAsciiString(text, str_Throne_Jam);
        }
    } else if (type == 6) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventPk1Race));
    } else if (type == 7) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventPk2Race));
    } else if (type == 8) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventAllPeakRace));
    } else if (type == 9) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventPk1Jam));
    } else if (type == 10) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventPk2Jam));
    } else if (type == 11) {
        cUIText_setUnicodeStringByID(text, GetHashValue32(str_kT_EventAllPeakJam));
    } else {
        cUIText_setAsciiString(text, cBENewRaceInterface_getLocationName(iface, track));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_CF758", setChallengeStats);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBEEconInterface_getTotalMoneyEarned(void* econ, int a, int b);
extern "C" const char* getDollarString_price(int price) __asm__("getDollarString");
extern "C" int sprintf2(void* buf, const char* fmt, ...);
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: SN soft-float float->double helper called by name
extern "C" double fptodp(float f);
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* iface, int peak);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak1(void* iface, int a, int b, int peak);
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* iface, int peak);
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(void* iface, int a, int b, int peak);
extern "C" unsigned int cBERewardInterface_getTrackHighScore(void* self, int b, int c, int d, int e);
extern char str_pct_S[];
extern char str_kT_CMNYouEarnedNum[];
extern char str_kT_CMNColllectNum[];
extern char str_kT_CMNChalCompNum[];
extern char str_pct_02_0f_pct_02_0f[];
extern char str_kT_OVRCMNYourBestTime[];
extern char str_kT_OVRCMNYourBestScore[];

static inline unsigned int udiv_CE758(unsigned int a, unsigned int b)
{
    return a / b;
}

static inline unsigned int umod_CE758(unsigned int a, unsigned int b)
{
    return a % b;
}

/**
 * Sets a trophy-room text to one challenge statistic of a rider: best time, best score, collectibles,
 * missions or money.
 *
 * Kinds: 0 best time (minutes:seconds) and 1 best score for the peak's challenge @p idx, 2 collectibles found
 * (@p idx 0) or missions completed, 3 total money earned.
 *
 * @param text  the text widget to set
 * @param profileIndex  the profile index (On Tour iProfileIndex)
 * @param characterID  the character (On Tour eBECharacters iCharacterID)
 * @param peak  the peak (On Tour eBEPeakID)
 * @param kind  which statistic (above)
 * @param idx  the challenge index, or 0 for collectibles and anything else for missions
 */
extern "C" void setChallengeStats(cUIText* text, int profileIndex, int characterID, int peak, int kind, int idx)
{
    unsigned short buf[100];
    char num[200];
    void* reward = cBE_getInterface_CE758(cBE_getBE(), 13);
    if (kind == 3) {
        int money = cBEEconInterface_getTotalMoneyEarned(cBE_getInterface_CE758(cBE_getBE(), 11), profileIndex, characterID);
        int fmt = Loc_CE758(str_kT_CMNYouEarnedNum);
        sprintf2(num, str_pct_S, getDollarString_price(money));
        if (fmt != 0) {
            sprintf1_CE758(buf, (void*)fmt, num);
            cUIText_setUnicodeString(text, buf);
        }
    } else if (kind == 2) {
        void* iface = cBE_getInterface_CE758(cBE_getBE(), 10);
        if (idx == 0) {
            int total = cBEMissionInterface_getTotalCollectForPeak(iface, peak);
            int current = cBEMissionInterface_getCurrentCollectForPeak1(iface, profileIndex, characterID, peak);
            int fmt = Loc_CE758(str_kT_CMNColllectNum);
            if (fmt != 0) {
                sprintf1_CE758(buf, (void*)fmt, current, total);
                cUIText_setUnicodeString(text, buf);
            }
        } else {
            int total = cBEMissionInterface_getNumberMissionsOnPeak(iface, peak);
            int current = cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(iface, profileIndex, characterID, peak);
            int fmt = Loc_CE758(str_kT_CMNChalCompNum);
            if (fmt != 0) {
                sprintf1_CE758(buf, (void*)fmt, current, total);
                cUIText_setUnicodeString(text, buf);
            }
        }
    } else if (kind == 0) {
        int track = D_0045AAD8_CE758[peak][kind][idx].track;
        int type = D_0045AAD8_CE758[peak][kind][idx].type;
        unsigned int time = cBERewardInterface_getTrackHighScore(reward, profileIndex, characterID, type, track);
        unsigned int min = udiv_CE758(time, 60);
        unsigned int sec = umod_CE758(time, 60);
        sprintf(num, str_pct_02_0f_pct_02_0f, fptodp((float)(int)min), fptodp((float)(int)sec));
        int fmt = Loc_CE758(str_kT_OVRCMNYourBestTime);
        if (fmt != 0) {
            sprintf1_CE758(buf, (void*)fmt, num);
            cUIText_setUnicodeString(text, buf);
        }
    } else if (kind == 1) {
        int track = D_0045AAD8_CE758[peak][kind][idx].track;
        int type = D_0045AAD8_CE758[peak][kind][idx].type;
        int current = cBERewardInterface_getTrackHighScore(reward, profileIndex, characterID, type, track);
        int fmt = Loc_CE758(str_kT_OVRCMNYourBestScore);
        if (fmt != 0) {
            sprintf1_CE758(buf, (void*)fmt, current);
            cUIText_setUnicodeString(text, buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_CF758", getChallengeMedal);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_ED90(void* be, int kind) __asm__("cBE_getInterface__Fv");
signed char cBERewardInterface_getTrackMedal(void* self, int a, int b, int track, int kind);
extern "C" int cBERewardInterface_getEarningsMedal(void* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getCollectiblesMedal(void* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getBigChallengesMedal(void* self, int a, int b, int kind);
struct sReq_ED90
{
    signed char track;
    signed char kind;
};
extern sReq_ED90 D_0045AAD8_ED90[][4][5] __asm__("gMedalsInfo");

/**
 * Returns the medal a profile's character has earned in one challenge of a peak's trophy list.
 *
 * Asks reward interface 13 (cBE_getInterface): trophy type 2 gives the collectibles medal for entry 0
 * (cBERewardInterface_getCollectiblesMedal) and the big-challenges medal otherwise
 * (cBERewardInterface_getBigChallengesMedal); type 3 gives the earnings medal (cBERewardInterface_getEarningsMedal);
 * types 0 and 1 look up the entry in gMedalsInfo[peak][type][entry] and return cBERewardInterface_getTrackMedal for its
 * track and kind. Declared void here, but every path ends in the getter's call, whose result the callers read
 * (cFEStateTrophyRoom_updateBigArt, cFEStatePeakRoom_updateMedalsList, cFEStateMountainRoom_updateMarkers).
 *
 * @param profileIndex  profile (user) index
 * @param charId  character id
 * @param peakNum  peak number
 * @param trophyType  trophy type (0, 1 track challenges; 2 collectibles / big challenges; 3 earnings)
 * @param challengeIndex  challenge index within the trophy type
 */
extern "C" void getChallengeMedal(int profileIndex, int charId, int peakNum, int trophyType, int challengeIndex)
{
    void* rewardIface = cBE_getInterface_ED90(cBE_getBE(), 13);
    if (trophyType == 2)
    {
        if (challengeIndex == 0)
        {
            cBERewardInterface_getCollectiblesMedal(rewardIface, profileIndex, charId, peakNum);
            return;
        }
        cBERewardInterface_getBigChallengesMedal(rewardIface, profileIndex, charId, peakNum);
        return;
    }
    if (trophyType != 3)
    {
        int kind = D_0045AAD8_ED90[peakNum][trophyType][challengeIndex].kind;
        int track = D_0045AAD8_ED90[peakNum][trophyType][challengeIndex].track;
        cBERewardInterface_getTrackMedal(rewardIface, profileIndex, charId, track, kind);
        return;
    }
    cBERewardInterface_getEarningsMedal(rewardIface, profileIndex, charId, peakNum);
}
#endif

//100%
INCLUDE_ASM("seg/seg_CF758", cFEStateRewardGalleryBase_cFEStateRewardGalleryBase);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cUIState_cUIState(void* self, void* parent);
extern "C" signed char cFECustom_getPlayerCtrlMask(void* mgr, int player);
signed char cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int cBELibrary_getRiderIndex(int flag);
extern "C" int cBENewPlayerInterface_getLastHub(void* self, int index);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern char cFEStateRewardGalleryBase_vtbl[];

struct sVEntry_1CEEA0 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sChallenge_1CEEA0 {
    char pad0[0x8];
    void* vtbl;              // 0x8
    int type;                // 0xC
    void** mgr;              // 0x10
    char f14;
    signed char f15;         // 0x15
    char pad16[0x2E];
    signed char player;      // 0x44
    char pad45[3];
    int f48;                 // 0x48
    int f4C;                 // 0x4C
    int charId;              // 0x50
    int f54;                 // 0x54
    int f58;                 // 0x58
    int f5C;                 // 0x5C
    int f60;
    int f64;                 // 0x64
    int f68;                 // 0x68
    int f6C;                 // 0x6C
    int f70;                 // 0x70
    int f74;                 // 0x74
    int f78;                 // 0x78
    char pad7C[0x8EC - 0x7C];
    int f8EC;                // 0x8EC
    int f8F0;                // 0x8F0
    int f8F4;                // 0x8F4
    int f8F8;                // 0x8F8
    int f8FC[10];            // 0x8FC
    int a[4][8];             // 0x924
    int b[4][8];             // 0x9A4
};

/**
 * Constructs the reward gallery base state for a player.
 *
 * Runs cUIState_cUIState, sets mHashName to 47 and installs cFEStateRewardGalleryBase_vtbl; stores mUserNum and its
 * controller mask (cFECustom_getPlayerCtrlMask) in mCtrlBits, mRewardType = @p rewardType, 4 rows by 8 columns
 * (mMaxRewardRows, mMaxRewardColumns) and mIconsFileIndex = -2, and clears the loading state, counts, highlighter
 * offsets, menu and text pointers and the 4 x 8 icon and icon-back tables. mCharID comes from new-player interface 1
 * (cBENewPlayerInterface_getPlayerCharID). With @p rank set (opened in a store) mStoreNum follows the player's last hub
 * (cBENewPlayerInterface_getLastHub: 17-18 -> 1, 19-20 -> 2, 21 -> 3, else 1); otherwise it is -1. Then it calls the
 * interface's virtual slot 2 and reads mMoney from economy interface 11 (cBEEconInterface_getMoney). The poster,
 * trading card, concept art, video, toy and cheat character galleries call it as their base constructor.
 *
 * @param parent  the UI engine
 * @param player  user number
 * @param rank  non-zero when the gallery is opened from a store (lodge); not a rank
 * @param rewardType  reward type (3-8 from the subclasses)
 * @return @p self
 */
extern "C" sChallenge_1CEEA0* cFEStateRewardGalleryBase_cFEStateRewardGalleryBase(sChallenge_1CEEA0* self, cUIEngine* parent, signed char player, int rank, int rewardType)
{
    int i;
    int j;
    cUIState_cUIState(self, parent);
    self->type = 47;
    self->vtbl = cFEStateRewardGalleryBase_vtbl;
    self->player = player;
    self->f15 = cFECustom_getPlayerCtrlMask(*self->mgr, player);
    self->f4C = rewardType;
    self->f58 = 4;
    self->f5C = 8;
    self->f8EC = -2;
    self->f48 = 0;
    self->f64 = 0;
    self->f8F4 = 0;
    self->f8F8 = 0;
    self->f70 = 0;
    self->f74 = 0;
    self->f8FC[0] = 0;
    self->f8FC[1] = 0;
    self->f8FC[2] = 0;
    self->f8FC[3] = 0;
    self->f8FC[4] = 0;
    self->f8FC[5] = 0;
    self->f8FC[6] = 0;
    self->f8FC[7] = 0;
    self->f8FC[8] = 0;
    self->f8FC[9] = 0;
    self->f68 = 0;
    self->f6C = 0;
    self->f78 = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            self->a[i][j] = 0;
            self->b[i][j] = 0;
        }
    }
    char* iface = (char*)cBE_getInterface_ED90(cBE_getBE(), 1);
    self->charId = cBENewPlayerInterface_getPlayerCharID(iface, self->player);
    if (rank != 0) {
        switch (cBENewPlayerInterface_getLastHub(iface, cBELibrary_getRiderIndex(self->player))) {
        case 17: self->f8F0 = 1; break;
        case 18: self->f8F0 = 1; break;
        case 19: self->f8F0 = 2; break;
        case 20: self->f8F0 = 2; break;
        case 21: self->f8F0 = 3; break;
        default: self->f8F0 = 1; break;
        }
    } else {
        self->f8F0 = -1;
    }
    sVEntry_1CEEA0* vt = *(sVEntry_1CEEA0**)(iface + 0xC);
    vt[2].fn(vthis(iface, vt[2]));
    self->f54 = cBEEconInterface_getMoney(cBE_getInterface_ED90(cBE_getBE(), 11), self->player, self->charId);
    return self;
}
#endif
