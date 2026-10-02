#include "common.h"

INCLUDE_ASM("main/gamemode", cGameModeMan_getGM);

INCLUDE_ASM("main/gamemode", func_002380E8);

INCLUDE_ASM("main/gamemode", cGameModeMan_initGameMode);

INCLUDE_ASM("main/gamemode", cGameModeMan_restartHeat);

//100%
INCLUDE_ASM("main/gamemode", func_00238348__FPv);
#ifdef SKIP_ASM
int func_00238348(void* self)
{
    int t0 = *(int*)((char*)self + 0x74);
    *(int*)((char*)self + 0x70) = t0;
    return t0;
}
#endif

INCLUDE_ASM("main/gamemode", func_00238358);

INCLUDE_ASM("main/gamemode", func_00238510);

INCLUDE_ASM("main/gamemode", func_00238550);

INCLUDE_ASM("main/gamemode", func_00238590);

INCLUDE_ASM("main/gamemode", func_00238B70);

INCLUDE_ASM("main/gamemode", func_00238BF8);

INCLUDE_ASM("main/gamemode", func_00238C80);

INCLUDE_ASM("main/gamemode", func_00238D30);

INCLUDE_ASM("main/gamemode", func_00238DA8);

INCLUDE_ASM("main/gamemode", func_00238E20);

INCLUDE_ASM("main/gamemode", func_00239230);

INCLUDE_ASM("main/gamemode", func_002398E8);

INCLUDE_ASM("main/gamemode", func_00239938);

INCLUDE_ASM("main/gamemode", func_00239AA0);

INCLUDE_ASM("main/gamemode", func_00239CE0);

//100%
INCLUDE_ASM("main/gamemode", func_00239D18);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
class cGameModeStream {
public:
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00239D18(void* self, cGameModeStream* s)
{
    s->v01((char*)self + 0x4, 0xA0);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00239D50);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_00239D50(void* self, cGameModeStream* s)
{
    s->v02((char*)self + 0x4, 0xA0);
}
#endif

INCLUDE_ASM("main/gamemode", func_00239D88);

INCLUDE_ASM("main/gamemode", func_00239EC8);

INCLUDE_ASM("main/gamemode", func_0023A070);

INCLUDE_ASM("main/gamemode", func_0023A108);

INCLUDE_ASM("main/gamemode", func_0023A4F0);

INCLUDE_ASM("main/gamemode", func_0023A668);

INCLUDE_ASM("main/gamemode", func_0023A760);

//100%
INCLUDE_ASM("main/gamemode", func_0023AC10__FPv);
#ifdef SKIP_ASM
void func_0023AC10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC18);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023AC18(void* self, cGameModeStream* s)
{
    s->v01(self, 0xD0);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023AC50);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023AC50(void* self, cGameModeStream* s)
{
    s->v02(self, 0xD0);
}
#endif

INCLUDE_ASM("main/gamemode", func_0023AC88);

INCLUDE_ASM("main/gamemode", func_0023AE00);

//100%
INCLUDE_ASM("main/gamemode", func_0023B038);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023B038(void* self, cGameModeStream* s)
{
    s->v01(self, 0x2C);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_0023B070);
#ifdef SKIP_ASM
// Serialisation stream: v01 = read(buf, size), v02 = write(buf, size).
extern "C" void func_0023B070(void* self, cGameModeStream* s)
{
    s->v02(self, 0x2C);
}
#endif

INCLUDE_ASM("main/gamemode", func_0023B0A8);

INCLUDE_ASM("main/gamemode", func_0023B170);

INCLUDE_ASM("main/gamemode", func_0023B268);

INCLUDE_ASM("main/gamemode", func_0023B468);

INCLUDE_ASM("main/gamemode", func_0023B5F8);

INCLUDE_ASM("main/gamemode", func_0023B6C0);

INCLUDE_ASM("main/gamemode", func_0023B8C8);

INCLUDE_ASM("main/gamemode", func_0023BB98);

INCLUDE_ASM("main/gamemode", func_0023BDB8);

INCLUDE_ASM("main/gamemode", func_0023C0D0);

INCLUDE_ASM("main/gamemode", func_0023C2D8);

INCLUDE_ASM("main/gamemode", func_0023C560);

INCLUDE_ASM("main/gamemode", func_0023C618);

INCLUDE_ASM("main/gamemode", func_0023C770);

