#include "common.h"

INCLUDE_ASM("wscript/wscriptmission", cWScriptMan_checkGate);

//100%
INCLUDE_ASM("wscript/wscriptmission", func_00184520);
#ifdef SKIP_ASM
struct cList;
struct cUIText;

struct sWSMEntry4520 {
    int f0;
    short f4;
    unsigned short id;          // 0x6
};

struct sWSMRider4520 {
    char pad0[0x18];
    signed char slot;           // 0x18
};

struct sWSMPlayer4520 {
    char pad0[0xA0];
    sWSMRider4520* rider;       // 0xA0
};

struct sWSMission4520 {
    char pad0[0x24];
    char list[0x20];            // 0x24 (cList)
    signed char player;         // 0x44
    char pad45[0x7];
    sWSMPlayer4520* p4C;        // 0x4C
    sWSMPlayer4520* p50;        // 0x50
};

extern char D_0045D9B8[];
extern char D_0045D9C8[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
void* cList_first(cList* list);
int GetHashValue32(char*);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" sWSMEntry4520* func_0014FF90(void* iface, int player, int slot, void* out);
extern "C" int func_00150928(void* iface, int player, int charID);
extern "C" const char* func_00198AF0(int id);

extern "C" void func_00184520(sWSMission4520* self)
{
    signed char b = self->p4C->rider->slot;
    signed char a = self->p50->rider->slot;
    char buf[16];
    sWSMEntry4520* tbl = func_0014FF90(cBE_getInterface_Fv(cBE_getBE(), 6), self->player, b, buf);
    void* screen = cList_first((cList*)self->list);
    cUIText* t = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9B8));
    if (t != 0) {
        cUIText_setAsciiString(t, func_00198AF0(tbl[a].id));
    }
    cUIText* t2 = (cUIText*)cUIScreen_getObjectByHashName(screen, GetHashValue32(D_0045D9C8));
    if (t2 != 0) {
        signed char cid = cBENewPlayerInterface_getPlayerCharID(cBE_getInterface_Fv(cBE_getBE(), 1), self->player);
        cUIText_setAsciiString(t2, func_00198AF0(func_00150928(cBE_getInterface_Fv(cBE_getBE(), 0xB), self->player, cid)));
    }
}
#endif

