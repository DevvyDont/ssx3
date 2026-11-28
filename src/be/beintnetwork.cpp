#include "common.h"
#include "be/bedbprofile.h"
#include "vtable.h"
#include "be/beintplayer.h"
#include "be/beintoption.h"
#include "be/beintreward.h"
#include "be/beintcharacter.h"
#include "be/beintnetwork.h"
#include "be/beinterface.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BENetworkInt[];
extern void* cBENetworkInterface_vtbl[16];

//99.58%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's network interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BENetworkInt", clears bIsActive and stores the vtable.
 *
 * @return the network interface
 */
void* cBENetworkInterface_getThis()
{
    if (cBENetworkInterface_instance == 0) {
        cBENetworkInterface* mem = (cBENetworkInterface*)cMemMan_alloc(0x10, str_BENetworkInt, 0, 0);
        mem->bIsActive = 0;
        VPTR_OF(cBEInterface, mem) = cBENetworkInterface_vtbl;
        cBENetworkInterface_instance = mem;
    }
    return cBENetworkInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_submit);
#ifdef SKIP_ASM
// The submitted copy of the network options (discard restores it).
extern tBENetworkDB cBELibrary_networkDB;

/**
 * Commits the network options: copies cBENetworkInterface_networkOpt into the saved copy cBELibrary_networkDB.
 */
extern "C" void cBENetworkInterface_submit(void)
{
    cBELibrary_networkDB = cBENetworkInterface_networkOpt;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_discard);
#ifdef SKIP_ASM
extern tBENetworkDB cBELibrary_networkDB;

/**
 * Drops changes to the network options: copies the saved copy cBELibrary_networkDB back into cBENetworkInterface_networkOpt.
 */
extern "C" void cBENetworkInterface_discard(void)
{
    cBENetworkInterface_networkOpt = cBELibrary_networkDB;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getSyncDataSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns the size of the career data block exchanged between consoles in an online game.
 *
 * Always 0xC44 (3140) bytes.
 *
 * @return 0xC44
 */
int cBENetworkInterface::getSyncDataSize()
{
    return 3140;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_getSyncData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sNetHdr_E2C8 {
    int w[0x4C / 4];
};

struct sNetPair_E2C8 {
    int w[2];
};

struct sNetTrip_E2C8 {
    int w[3];
};

struct sNetHW_E2C8 {
    short h[2];
};

struct sNetDW_E2C8 {
    int w[2];
};

struct sNetTag_E2C8 {
    char c[7];
};

struct sNetKey_E2C8 {
    short h[3];
};


struct sNetState_E2C8 {
    sNetHdr_E2C8 hdr;                   // 0x000
    cBEPlayerDB me;             // 0x04C
    cBEPlayerDB chars[6];       // 0x068
    signed char charID;                 // 0x110
    sNetTag_E2C8 tag;                   // 0x111
    sNetKey_E2C8 keys[0x55];            // 0x118
    sNetHW_E2C8 blob[0x20D];            // 0x316
    char pad_0xB4A[2];
    sNetDW_E2C8 moves[0x1A];            // 0xB4C
    sNetTrip_E2C8 t0;                   // 0xC1C
    sNetTrip_E2C8 t1;                   // 0xC28
    sNetPair_E2C8 pair;                 // 0xC34
    int a;                              // 0xC3C
    int b;                              // 0xC40
};

extern int cBENewRaceInterface_bIsBamLocation;
extern int cBENewRaceInterface_mNextLocation;
extern sNetHdr_E2C8 cBELibrary_raceDB;
extern char D_005308D0[];
extern char D_004A6F38[];
extern char D_004A7778[];
extern char D_004A7887[];

/**
 * Packs one player's back-end race data into the 0xC44-byte network sync buffer.
 *
 * Copies the race settings (cBELibrary_raceDB) and the words cBENewRaceInterface_bIsBamLocation/cBENewRaceInterface_mNextLocation, all six cBELibrary_playerDB
 * records, the player's own record and character ID, the shared option pair (D_005308D0) and both players'
 * option records, then from that character's cBELibrary_profileLib record the entries at +0x290, +0xAD0 and
 * +0xBDF and the BAGT data at +0xBE6.
 *
 * @param player    the player whose data is packed (cGameComm_syncGame uses 0 on the host, 1 on the client)
 * @param out  the sync buffer (cGameComm mBEBuffer)
 */
extern "C" void cBENetworkInterface_getSyncData(cBENetworkInterface* self, int player, sNetState_E2C8* out)
{
    out->a = cBENewRaceInterface_bIsBamLocation;
    out->b = cBENewRaceInterface_mNextLocation;
    out->hdr = cBELibrary_raceDB;
    for (int i = 0; i < 6; i++) {
        out->chars[i] = cBELibrary_playerDB[i];
    }
    out->me = cBELibrary_playerDB[player];
    signed char characterId = cBELibrary_playerDB[player].cCharacterID;
    out->charID = characterId;
    out->pair = *(sNetPair_E2C8*)D_005308D0;
    out->t0 = *(sNetTrip_E2C8*)(D_005308D0 - 0x18);
    out->t1 = *(sNetTrip_E2C8*)(D_005308D0 - 0xC);
    for (int k = 0; k < 525; k++) {
        out->blob[k] = ((sNetHW_E2C8*)(D_004A6F38 + (characterId * 0xF88 + player * 0x9B50)))[k];
    }
    for (int k = 0; k < 26; k++) {
        out->moves[k] = ((sNetDW_E2C8*)(D_004A7778 + (characterId * 0xF88 + player * 0x9B50)))[k];
    }
    out->tag = *(sNetTag_E2C8*)(D_004A7887 + (characterId * 0xF88 + player * 0x9B50));
    for (int k = 0; k < 85; k++) {
        out->keys[k] = ((sNetKey_E2C8*)(D_004A7887 + 7 + (characterId * 0xF88 + player * 0x9B50)))[k];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_setSyncData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBECharProfileDB_init(void* p, int i, int arg);
extern "C" void cBEProfileLib_resetAllRelations(void* self);


extern cBECharProfileDB cBELibrary_profileLib_E5C8[][10] __asm__("cBELibrary_profileLib");

class cBEIface_E5C8 {
public:
    int pad[3];
    virtual void v01();
    virtual void discard();
};

/**
 * Unpacks one player's back-end race data from the network sync buffer and reloads the affected interfaces.
 *
 * Restores the race settings and reloads NewRace (BE interface 0); on the client it also takes records 2-5
 * and the shared option pair. It stores the player's record and option record, re-initialises that
 * character's profile (cBECharProfileDB_init, cBEProfileLib_resetAllRelations), copies the profile pieces back and reloads NewPlayer
 * (1) and BAGT (6), each reload being the interface's virtual discard (slot 2).
 *
 * @param player  the player slot the data belongs to (0 host, 1 client)
 * @param in      the sync buffer, as filled by cBENetworkInterface_getSyncData()
 */
extern "C" void cBENetworkInterface_setSyncData(cBENetworkInterface* self, int player, sNetState_E2C8* in)
{
    cBENewRaceInterface_bIsBamLocation = in->a;
    cBENewRaceInterface_mNextLocation = in->b;
    cBELibrary_raceDB = in->hdr;
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 0))->discard();
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
    {
        for (int i = 2; i < 6; i++)
            cBELibrary_playerDB[i] = in->chars[i];
    }
    cBELibrary_playerDB[player] = in->me;
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 1))->discard();
    signed char characterId = in->charID;
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
        *(sNetPair_E2C8*)D_005308D0 = in->pair;
    ((sNetTrip_E2C8*)&cBERules_mRules)[player] = (&in->t0)[player];
    cBECharProfileDB_init(&cBELibrary_profileLib_E5C8[player][characterId], characterId, 1);
    cBEProfileLib_resetAllRelations(cBELibrary_profileLib_E5C8[player]);
    for (int k = 0; k < 525; k++)
        ((sNetHW_E2C8*)((char*)cBELibrary_profileLib_E5C8 + 0x290 + (characterId * 0xF88 + player * 0x9B50)))[k] = in->blob[k];
    for (int k = 0; k < 26; k++)
        ((sNetDW_E2C8*)(D_004A7778 + (characterId * 0xF88 + player * 0x9B50)))[k] = in->moves[k];
    *(sNetTag_E2C8*)(D_004A7887 + (characterId * 0xF88 + player * 0x9B50)) = in->tag;
    for (int k = 0; k < 85; k++)
        ((sNetKey_E2C8*)(D_004A7887 + 7 + (characterId * 0xF88 + player * 0x9B50)))[k] = in->keys[k];
    ((cBEIface_E5C8*)cBE_getInterface_Fv(cBE_getBE(), 6))->discard();
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_copyProfile);
#ifdef SKIP_ASM
struct sBEPlayerData {
    char data[0x9B50];
};
extern sBEPlayerData cBELibrary_profileLib[];

extern "C" void cBEProfileLib_copy(sBEPlayerData* dst, sBEPlayerData* src);

/**
 * Copies one profile's ten character records over another's (cBEProfileLib_copy).
 *
 * Each destination record keeps its own lookup-table pointer (+0x288), which is rebuilt from the copied 0x20D
 * entries at +0x290.
 *
 * @param srcProfile  source profile index into cBELibrary_profileLib
 * @param destProfile  destination profile index
 */
void cBENetworkInterface::copyProfile(int srcProfile, int destProfile)
{
    sBEPlayerData* src = &cBELibrary_profileLib[srcProfile];
    cBEProfileLib_copy(&cBELibrary_profileLib[destProfile], src);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_preInitBE);
#ifdef SKIP_ASM
extern "C" void cBEProfileLib_init(void* self, int arg);
// PORT: cBENetworkInterface_swapBraggingRecords is defined (void) in this unit but called with self here.
void cBENetworkInterface_swapBraggingRecords_self(void* self) __asm__("cBENetworkInterface_swapBraggingRecords");

struct sRiderEntry_0014E9F8 {
    int w[7];
};

extern char D_004BA348[];
extern sRiderEntry_0014E9F8 D_00535B3C;

/**
 * Prepares the back end before the online handshake exchanges sync data.
 *
 * Re-initialises profile 2's ten character records (cBEProfileLib_init(D_004BA348, 0)); on the client it copies
 * player record 0 into record 1 and profile 0 into profile 1 (the client is player 1 online). Then it swaps
 * the two players' option records (cBENetworkInterface_swapBraggingRecords), which postInitBE swaps back.
 */
void cBENetworkInterface::preInitBE()
{
    cBEProfileLib_init(D_004BA348, 0);
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
    {
        sRiderEntry_0014E9F8* riderEntry = &D_00535B3C;
        riderEntry[0] = riderEntry[-1];
        copyProfile(0, 1);
    }
    cBENetworkInterface_swapBraggingRecords_self(this);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_postInitBE);
#ifdef SKIP_ASM

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBENetworkInterface_swapBraggingRecords is defined (void) in this unit but called with self here.
void cBENetworkInterface_swapBraggingRecords_self(void* self) __asm__("cBENetworkInterface_swapBraggingRecords");

/**
 * Finishes the back-end setup after the online handshake.
 *
 * Reloads the Stat (BE interface 3) and Aggression (12) interfaces through their virtual discard (slot 2),
 * then swaps the two players' option records again (cBENetworkInterface_swapBraggingRecords), undoing preInitBE's swap.
 */
void cBENetworkInterface::postInitBE()
{
    cBEInterface* statInterface = (cBEInterface*)cBE_getInterface_Fv(cBE_getBE(), 3);
    statInterface->discard();
    cBEInterface* aggressionInterface = (cBEInterface*)cBE_getInterface_Fv(cBE_getBE(), 0xC);
    aggressionInterface->discard();
    cBENetworkInterface_swapBraggingRecords_self(this);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_swapBraggingRecords);
#ifdef SKIP_ASM
struct sNetSlot_0014EB08
{
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 1;
    unsigned int rest : 28;
    int x;
    int y;
};


/**
 * Swaps most of the two 12-byte bragging-rights records at the start of the rules block cBERules_mRules (see
 * cBERules_reset).
 *
 * Exchanges bit 0 and bit 3 of each record's flag word and its second and third words (starting points, head-start
 * seconds); flag bits 1-2 and 4-31 stay.
 * cBENetworkInterface_preInitBE swaps them before the online handshake and cBENetworkInterface_postInitBE swaps them
 * back (both pass self, which is ignored).
 */
extern "C" void cBENetworkInterface_swapBraggingRecords(void)
{
    sNetSlot_0014EB08 firstRecord = (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[0]);
    cBERules_mRules.Bragging[0].HeadStartSecs = cBERules_mRules.Bragging[1].HeadStartSecs;
    (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[0]).c = (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[1]).c;
    (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[0]).a = (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[1]).a;
    cBERules_mRules.Bragging[0].StartingPoints = cBERules_mRules.Bragging[1].StartingPoints;
    cBERules_mRules.Bragging[1].HeadStartSecs = firstRecord.y;
    (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[1]).c = firstRecord.c;
    (*(sNetSlot_0014EB08*)&cBERules_mRules.Bragging[1]).a = firstRecord.a;
    cBERules_mRules.Bragging[1].StartingPoints = firstRecord.x;
}
#endif

extern "C" char* strcpy(char*, const char*);

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_setOpponentName);
#ifdef SKIP_ASM
/**
 * Copies @p src into the opponent-name buffer (cBENetworkInterface_networkOpt.strOpponentName, 16 bytes; strcpy, no
 * length check), which cBENetworkInterface_getOpponentName returns.
 *
 * The lobby code passes the opponent's persona name.
 *
 * @param src  the name
 */
void cBENetworkInterface::setOpponentName(const char* src)
{
    strcpy(cBENetworkInterface_networkOpt.strOpponentName, src);
}
#endif


//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_setMyName);
#ifdef SKIP_ASM
/**
 * Copies @p src into this player's name buffer (cBENetworkInterface_networkOpt.strMyName, 16 bytes; strcpy, no length
 * check), which cBENetworkInterface_getMyName returns.
 *
 * The lobby code passes the local persona name.
 *
 * @param src  the name
 */
void cBENetworkInterface::setMyName(const char* src)
{
    strcpy(cBENetworkInterface_networkOpt.strMyName, src);
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_backupCharProfile);
#ifdef SKIP_ASM

extern cBEPlayerDB cBENetworkInterface_BackupPlayerDB[];
extern "C" void cBEProfileLib_backupCharProfile(sBEPlayerData* data, int charID);

/**
 * Saves one player's current character record and all six player records for restoreCharProfile.
 *
 * Copies the cBELibrary_profileLib record of that player's character into the backup buffer cBEProfileLib_BackupCharProfile
 * (cBEProfileLib_backupCharProfile) and the six cBELibrary_playerDB records into BackupPlayerDB (cBENetworkInterface_BackupPlayerDB).
 *
 * @param idx  the player (profile index) whose character record is saved
 */
void cBENetworkInterface::backupCharProfile(int idx)
{
    cBEProfileLib_backupCharProfile(&cBELibrary_profileLib[idx], cBELibrary_playerDB[idx].cCharacterID);
    for (int i = 0; i < 6; i++)
    {
        cBENetworkInterface_BackupPlayerDB[i] = cBELibrary_playerDB[i];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBENetworkInterface_restoreCharProfile);
#ifdef SKIP_ASM
extern "C" void cBEProfileLib_restoreCharProfile(sBEPlayerData* data);

/**
 * Restores what backupCharProfile saved and reloads the player and BAGT interfaces.
 *
 * Copies the backup buffer back into the saved character's record of profile @p idx (cBEProfileLib_restoreCharProfile). On the
 * client, player record 0 first takes record 1; records 1-5 then come back from BackupPlayerDB (record 0 is
 * never restored). Reloads NewPlayer (BE interface 1) and BAGT (6) through their virtual discard.
 *
 * @param idx  the player (profile index) to restore
 */
void cBENetworkInterface::restoreCharProfile(int idx)
{
    cBEProfileLib_restoreCharProfile(&cBELibrary_profileLib[idx]);
    if (cBENetworkInterface_networkOpt.bIsHost == 0)
    {
        cBELibrary_playerDB[0] = cBELibrary_playerDB[1];
    }
    for (int i = 1; i < 6; i++)
    {
        cBELibrary_playerDB[i] = cBENetworkInterface_BackupPlayerDB[i];
    }
    cBEInterface* playerInterface = (cBEInterface*)cBE_getInterface_Fv(cBE_getBE(), 1);
    playerInterface->discard();
    cBEInterface* grabTable = (cBEInterface*)cBE_getInterface_Fv(cBE_getBE(), 6);
    grabTable->discard();
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getThis);
#ifdef SKIP_ASM
extern const char str_BECharInt[];
extern void* cBECharacterInterface_vtbl[16];


/**
 * Returns the back end's character interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BECharInt", clears bIsActive and stores the vtable.
 *
 * @return the character interface
 */
extern "C" void* cBECharacterInterface_getThis(void)
{
    if (cBECharacterInterface_instance == 0) {
        cBECharacterInterface* mem = (cBECharacterInterface*)cMemMan_alloc(0x10, str_BECharInt, 0, 0);
        mem->bIsActive = 0;
        VPTR_OF(cBEInterface, mem) = cBECharacterInterface_vtbl;
        cBECharacterInterface_instance = (cBECharacterInterface*)(mem);
    }
    return cBECharacterInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getName);
#ifdef SKIP_ASM
signed char cBELibrary_getCharacterID(int index);

struct sNetChar_0014EE28
{
    char data[0x88];
};

extern sNetChar_0014EE28 cBELibrary_characterDB[];

/**
 * Returns @p rider's character record (cBELibrary_characterDB + character * 0x88), whose first field is the character's name.
 *
 * @param rider  the rider (cBELibrary_getCharacterID selects the record)
 * @return the name
 */
void* cBECharacterInterface::getName(int rider)
{
    return &cBELibrary_characterDB[cBELibrary_getCharacterID(rider)];
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getShortName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBELibrary_getCharacterID(int index);
extern "C" signed char cBELibrary_getCheatCharacterID(int index);

struct sNetEntry8_0014EE58
{
    char data[8];
};

struct sNetEntry88_0014EE58
{
    char data[0x88];
};

extern sNetEntry8_0014EE58 D_0043FA38_0014EE58[] __asm__("D_0043FA38");
extern sNetEntry88_0014EE58 D_00530990_0014EE58[] __asm__("D_00530990");

/**
 * Returns a rider's short character name.
 *
 * A cheat character (cheat index 10 to 29) gets its 8-byte name from the table based at D_0043FA38 (index 10 is
 * "Brodi"); any other rider gets the address of its character's 0x88-byte record in D_00530990 (probably On
 * Tour's cBELibrary::characterDB), which presumably starts with the name.
 *
 * @param rider  the rider whose character is used
 * @return the name
 */
void* cBECharacterInterface::getShortName(int rider)
{
    int characterId = cBELibrary_getCharacterID(rider);
    int index = cBELibrary_getCheatCharacterID(rider);
    if (index >= 10 && index < 30) {
        return &D_0043FA38_0014EE58[index];
    }
    return &D_00530990_0014EE58[characterId];
}
#endif

//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getShortNameByCharacter);
#ifdef SKIP_ASM
struct sNetEntry8_0043FA38
{
    char data[8];
};

struct sNetEntry88_00530990
{
    char data[0x88];
};

extern sNetEntry8_0043FA38 D_0043FA38[];
extern sNetEntry88_00530990 D_00530990[];

/**
 * Returns a rider name for character @p player: the cheat character's name D_0043FA38[@p index] when @p index is
 * 10-29, else the text at +0x20 of the character's record (D_00530990 = cBELibrary_characterDB + 0x20).
 *
 * @param player  the character id
 * @param index  the cheat character id (0 for none)
 * @return the name
 */
void* cBECharacterInterface::getShortNameByCharacter(int player, int index)
{
    if (index >= 10 && index < 30) {
        return &D_0043FA38[index];
    }
    return &D_00530990[player];
}
#endif


//100%
INCLUDE_ASM("be/beintnetwork", cBECharacterInterface_getNickName);
#ifdef SKIP_ASM
signed char cBELibrary_getCharacterID(int index);

struct sNetChar_0014EF00
{
    char data[0x88];
};

extern sNetChar_0014EF00 D_005309A0[];

/**
 * Returns the 16-byte text at +0x30 of @p rider's character record (D_005309A0 = cBELibrary_characterDB + 0x30).
 *
 * @param rider  the rider (cBELibrary_getCharacterID selects the record)
 * @return the text
 */
void* cBECharacterInterface::getNickName(int rider)
{
    return &D_005309A0[cBELibrary_getCharacterID(rider)];
}
#endif
