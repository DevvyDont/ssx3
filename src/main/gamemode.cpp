#include "common.h"

INCLUDE_ASM("main/gamemode", cGameModeMan_getGM);

//100%
INCLUDE_ASM("main/gamemode", func_002380E8);
#ifdef SKIP_ASM
class cMode2380E8 {
public:
    virtual ~cMode2380E8();
};
void operator_delete(int*);
extern void* D_00536668[];
extern int* D_004A2C6C;

extern "C" void func_002380E8(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        delete (cMode2380E8*)D_00536668[i];
    }
    if (D_004A2C6C != 0) {
        operator_delete(D_004A2C6C);
    }
    D_004A2C6C = 0;
}
#endif

INCLUDE_ASM("main/gamemode", cGameModeMan_initGameMode);

//100%
INCLUDE_ASM("main/gamemode", cGameModeMan_restartHeat);
#ifdef SKIP_ASM
struct sVEntry_002382D8 {
    short delta;
    short index;
    void (*fn)(void*);
};

extern void* D_00536668[];
extern int D_005366D0[];

extern "C" void cGameModeMan_restartHeat(void* self)
{
    int i;
    void* obj = D_00536668[*(int*)((char*)self + 0x4)];
    sVEntry_002382D8* vt = *(sVEntry_002382D8**)obj;
    vt[2].fn((char*)obj + vt[2].delta);
    for (i = 5; i >= 0; i--) {
        D_005366D0[i] = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("main/gamemode", func_00238510);
#ifdef SKIP_ASM
struct sVEntry_00238510 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

extern "C" void func_00238510(void* self, int a, int b)
{
    void* obj = D_00536668[*(int*)((char*)self + 0x4)];
    sVEntry_00238510* vt = *(sVEntry_00238510**)obj;
    vt[6].fn((char*)obj + vt[6].delta, a, b);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238550);
#ifdef SKIP_ASM
struct sVEntry_00238550 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

extern "C" void func_00238550(void* self, int a, int b)
{
    void* obj = D_00536668[*(int*)((char*)self + 0x4)];
    sVEntry_00238550* vt = *(sVEntry_00238550**)obj;
    vt[7].fn((char*)obj + vt[7].delta, a, b);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238590);
#ifdef SKIP_ASM
struct sVEntry_00238590 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* D_00536668[];

extern "C" void func_00238590(void* self, int a, int b)
{
    void* obj = D_00536668[*(int*)((char*)self + 0x4)];
    sVEntry_00238590* vt = *(sVEntry_00238590**)obj;
    vt[9].fn((char*)obj + vt[9].delta, a, b);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238B70);
#ifdef SKIP_ASM
extern int D_00536730[];
extern int D_00536708[];
extern "C" int func_00245E30(const void* a, const void* b);
extern "C" int func_00245EC0(const void* a, const void* b);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));

extern "C" void func_00238B70(void* self, int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        D_00536730[i] = i;
        D_00536708[i] = i;
    }
    func_00418EF8(D_00536708, n, 4, func_00245E30);
    func_00418EF8(D_00536730, n, 4, func_00245EC0);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238BF8);
#ifdef SKIP_ASM
extern int D_00536730[];
extern int D_00536708[];
extern "C" int func_00245E78(const void* a, const void* b);
extern "C" int func_00245EC0(const void* a, const void* b);
extern "C" void func_00418EF8(void* base, int n, int size, int (*cmp)(const void*, const void*));

extern "C" void func_00238BF8(void* self, int n)
{
    int i;
    for (i = 0; i < 10; i++) {
        D_00536730[i] = i;
        D_00536708[i] = i;
    }
    func_00418EF8(D_00536708, n, 4, func_00245E78);
    func_00418EF8(D_00536730, n, 4, func_00245EC0);
}
#endif

INCLUDE_ASM("main/gamemode", func_00238C80);

//100%
INCLUDE_ASM("main/gamemode", func_00238D30);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern int D_00536730[];
extern int D_00536708[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_005366D0[];

extern "C" void func_00238D30(void)
{
    func_00416210(D_00536730, 9, 0x28);
    func_00416210(D_00536708, 0, 0x28);
    func_00416210(D_00536640, 0, 0x28);
    func_00416210(D_005366A8, 0, 0x28);
    func_00416210(D_005366D0, 0, 0x18);
}
#endif

//100%
INCLUDE_ASM("main/gamemode", func_00238DA8);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern int D_00536730[];
extern int D_00536708[];
extern unsigned int D_00536640[];
extern int D_005366A8[];
extern int D_005366D0[];

extern "C" void func_00238DA8(void)
{
    func_00416210(D_00536730, 9, 0x28);
    func_00416210(D_00536708, 0, 0x28);
    func_00416210(D_00536640, 9, 0x28);
    func_00416210(D_005366A8, 0, 0x28);
    func_00416210(D_005366D0, 0, 0x18);
}
#endif

INCLUDE_ASM("main/gamemode", func_00238E20);

INCLUDE_ASM("main/gamemode", func_00239230);

INCLUDE_ASM("main/gamemode", func_002398E8);

INCLUDE_ASM("main/gamemode", func_00239938);

INCLUDE_ASM("main/gamemode", func_00239AA0);

//100%
INCLUDE_ASM("main/gamemode", func_00239CE0);
#ifdef SKIP_ASM
struct sScoreTable239CE0 {
    int f0;
    int tbl[8][10];
};
extern char* D_004A2C70;

extern "C" int func_00239CE0(sScoreTable239CE0* self, int col, int row)
{
    if (*(int*)D_004A2C70 < row) {
        return 0;
    }
    return self->tbl[row][col];
}
#endif

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

//100%
INCLUDE_ASM("main/gamemode", func_0023A070);
#ifdef SKIP_ASM
extern char* D_004A2C70;
extern void* D_004A28A8;

extern "C" int func_0023A070(char* self, int player, int secs)
{
    short* score = (short*)(self + 4);
    short* mine = (short*)((char*)score + (player << 1));
    short s = ++*mine;
    int other = 0;
    if (player == 0) {
        other = 1;
    }
    if (*(short*)((char*)score + (other << 1)) >= s) {
        return 0;
    }
    char* m = D_004A2C70;
    if (*(int*)(m + 8) == 1) {
        unsigned int t = *(unsigned int*)(m + 0x78);
        if (t < *(unsigned int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 8)) {
            return 0;
        }
        *(unsigned int*)(m + 0x78) = secs * 60 + t;
        return 1;
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("main/gamemode", func_0023C770);
#ifdef SKIP_ASM
extern "C" unsigned int func_00237CD8(void);

extern "C" void func_0023C770(int* arr, unsigned int n)
{
    int i;
    for (i = 0; i < 25; i++) {
        unsigned int a = func_00237CD8() % n;
        unsigned int b = func_00237CD8() % n;
        int t = arr[a];
        arr[a] = arr[b];
        arr[b] = t;
    }
}
#endif

