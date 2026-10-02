#include "common.h"

INCLUDE_ASM("ai/ai", cAI_cAI);

//100%
INCLUDE_ASM("ai/ai", func_001287B8);
#ifdef SKIP_ASM
extern "C" void func_0012B090(void*);
void operator_delete(int*);
extern char D_00458488[];

extern "C" void func_001287B8(void* self, int flags)
{
    *(void**)((char*)self + 0xCC) = D_00458488;
    func_0012B090(self);
    operator_delete(*(int**)((char*)self + 0xA4));
    operator_delete(*(int**)((char*)self + 0xA8));
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128818);
#ifdef SKIP_ASM
extern "C" void func_00103358(void*);
extern "C" void func_00101688(void*);
extern "C" void cAI_setAIState(void*, int);
extern "C" void* func_001297C8(void*, int);
// PORT: cAI_loadAnims is defined (void) in this unit, but this caller passes self in $a0; bound by asm label.
void cAI_loadAnims_self(void* self) __asm__("cAI_loadAnims");

struct sVEntry128818 { short delta; short index; void (*fn)(void*); };

struct sAiObj128818
{
    int f0;             // 0x00
    char pad4[0xC];
    int f10;            // 0x10
    int f14;            // 0x14
    int f18;            // 0x18
    int f1C;            // 0x1C
    int a20[2];         // 0x20
    int a28[6];         // 0x28
    char pad40[0x8];
    int a48[5];         // 0x48
    char pad5C[0x14];
    int a70[1];         // 0x70
    int f74;            // 0x74
    int f78;            // 0x78
    int f7C;            // 0x7C
    int f80;            // 0x80
    int f84;            // 0x84
    int f88;            // 0x88
    int f8C;            // 0x8C
    int f90;            // 0x90
    int f94;            // 0x94
    int f98;            // 0x98
    char pad9C[0x8];
    void* fA4;          // 0xA4
    void* fA8;          // 0xA8
    char padAC[0x20];
    sVEntry128818* vt;  // 0xCC
};

extern "C" void func_00128818(sAiObj128818* self)
{
    int i;
    func_00103358(self->fA4);
    func_00101688(self->fA8);
    self->f10 = 0;
    self->f14 = 0;
    self->f90 = 0;
    self->f78 = 0;
    self->f7C = 0;
    self->f80 = 0;
    self->f84 = 0;
    self->f88 = 0;
    self->f1C = 0;
    self->f98 = 0;
    self->f0 = 0;
    cAI_setAIState(self, 1);
    for (i = 0; i < 6; i++) self->a28[i] = 0;
    for (i = 0; i < 1; i++) self->a70[i] = 0;
    for (i = 0; i < 5; i++) self->a48[i] = 0;
    cAI_loadAnims_self(self);
    self->vt[6].fn((char*)self + self->vt[6].delta);
    func_001297C8(self, 1);
    for (i = 0; i < 2; i++) self->a20[i] = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128958);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void*);
extern "C" void cAI_initComputerRiders(void*);

extern "C" void func_00128958(void* self)
{
    *(int*)((char*)self + 0x94) = 0;
    if (*(int*)((char*)self + 0x88) > 0)
    {
        cAI_purgeMissionRiders(self);
    }
    cAI_initComputerRiders(self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128998);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cAI_initComputerActors(void*);
extern signed char D_00535C11[];

extern "C" void func_00128998(void* self, int n)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 0)
    {
        *(int*)((char*)self + 0x94) = n;
        cAI_initComputerActors(self);
    }
}
#endif

extern "C" void* func_001297C8(void*, int);

//100%
INCLUDE_ASM("ai/ai", func_001289F0__FPv);
#ifdef SKIP_ASM
void* func_001289F0(void* self)
{
    return func_001297C8(self, 0);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128A10);
#ifdef SKIP_ASM
struct sAiObj128A48;
extern "C" void func_00128A48(sAiObj128A48* self, int mode);

extern "C" void func_00128A10(void* self)
{
    func_001297C8(self, 0);
    func_00128A48((sAiObj128A48*)self, 0);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128A48);
#ifdef SKIP_ASM
struct sAiObj128A48
{
    char pad0[0x28];
    char* mRiders[19];
    int mMode;
    int mCount;
};

extern "C" void func_00128A48(sAiObj128A48* self, int mode)
{
    self->mMode = mode;
    if (mode == 0)
    {
        for (int i = 0; i < self->mCount; i++)
        {
            *(int*)(self->mRiders[i] + 0xEC) = 0;
        }
    }
    else
    {
        for (int i = 0; i < self->mCount; i++)
        {
            *(int*)(self->mRiders[i] + 0xEC) = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00128AC0);
#ifdef SKIP_ASM
extern "C" void cAI_initMissionRiders(void*);

extern "C" void func_00128AC0(void* self)
{
    cAI_initMissionRiders(self);
    func_001297C8(self, 0);
}
#endif

INCLUDE_ASM("ai/ai", func_00128AF0);

//100%
INCLUDE_ASM("ai/ai", func_00129160);
#ifdef SKIP_ASM
extern "C" void func_0011EB60(void* self, float value);
extern "C" void func_0011EB98(void* self);
extern "C" void func_003103F0(void*);

extern "C" void func_00129160(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        func_0011EB60(self->mRiders[i], 1.0f);
        func_0011EB98(self->mRiders[i]);
        func_003103F0(*(void**)(self->mRiders[i] + 0x780));
    }
}
#endif

INCLUDE_ASM("ai/ai", func_001291E0);

//100%
INCLUDE_ASM("ai/ai", func_001296F8);
#ifdef SKIP_ASM
extern "C" int func_0011D640(void* self);
extern "C" void func_0011C298(void* self);

extern "C" void func_001296F8(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        if (func_0011D640(self->mRiders[i]) == 0)
        {
            func_0011C298(self->mRiders[i]);
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00129768);
#ifdef SKIP_ASM
extern "C" void func_00103358(void*);
extern "C" void func_00101688(void*);
extern "C" void cAI_setAIState(void*, int);

extern "C" void func_00129768(void* self, int state)
{
    func_00103358(*(void**)((char*)self + 0xA4));
    func_00101688(*(void**)((char*)self + 0xA8));
    *(int*)((char*)self + 0x98) = 0;
    *(int*)((char*)self + 0x0) = 0;
    cAI_setAIState(self, state);
    func_001297C8(self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001297C8);
#ifdef SKIP_ASM
void* func_0010F398(void* self);
extern "C" void func_00154A58(int rider, int a);
extern "C" void func_0011D390(void* rider);

struct sVEntry1297C8 { short delta; short index; int (*fn)(void*); };

struct sAiObj1297C8
{
    char pad0[0x8];
    int f8;             // 0x08
    int fC;             // 0x0C
    char pad10[0x8];
    int f18;            // 0x18
    int f1C;            // 0x1C
    char pad20[0x8];
    char* mRiders[18];  // 0x28
    char* mList[2];     // 0x70
    int mCount;         // 0x78
    char pad7C[0xC];
    int mCount2;        // 0x88
};

extern "C" void* func_001297C8(void* p, int flag)
{
    sAiObj1297C8* self = (sAiObj1297C8*)p;
    func_0010F398(self);
    self->f8 = 0;
    if (flag != 0)
    {
        self->fC = 0;
    }
    self->f18 = -1;
    self->f1C = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* obj = self->mRiders[i] + 0x6C0;
        sVEntry1297C8* vt = *(sVEntry1297C8**)obj;
        if (vt[9].fn(obj + vt[9].delta) != 0 || flag != 0)
        {
            func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
            func_0011D390(self->mRiders[i]);
        }
    }
    for (int i = 0; i < self->mCount2; i++)
    {
        func_0011D390(*(void**)(self->mList[i] + 0x18));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001298C8);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" int func_001298C8()
{
    return *(int*)(*(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) + 0x8);
}
#endif

INCLUDE_ASM("ai/ai", cAI_InitPlayers);

INCLUDE_ASM("ai/ai", cAI_initMissionRiders);

INCLUDE_ASM("ai/ai", cAI_initComputerRiders);

INCLUDE_ASM("ai/ai", cAI_initComputerActors);

//100%
INCLUDE_ASM("ai/ai", func_0012A180);
#ifdef SKIP_ASM
extern "C" int func_0011D640(void* self);

extern "C" int func_0012A180(sAiObj128A48* self)
{
    int result = 1;
    for (int i = 0; i < self->mCount; i++)
    {
        if (func_0011D640(self->mRiders[i]) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012A250);
#ifdef SKIP_ASM
struct sAiObj12A250
{
    char pad0[0x40];
    char* mRiders[15];
    int mCount;
};

static inline bool aiRiderOk12A250(char* rider)
{
    return *(float*)(*(char**)(rider + 0x18) + 0x470) >= 0.0f;
}

extern "C" int func_0012A250(sAiObj12A250* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        bool ok = aiRiderOk12A250(self->mRiders[i]);
        if (!ok)
        {
            return 0;
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("ai/ai", func_0012A340);

//100%
INCLUDE_ASM("ai/ai", func_0012A490);
#ifdef SKIP_ASM
extern void* D_004A28A8;
extern "C" int func_0022E0E0(void* self);
void func_0026ADA0(void* self);
extern char D_004D33A0[];

extern "C" void func_0012A490(void)
{
    if (func_0022E0E0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x78)) < 0x16)
    {
        func_0026ADA0(D_004D33A0);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012AB20);
#ifdef SKIP_ASM
struct sAiObj12AB20
{
    char pad0[0x14];
    int m14;
    char pad18[0x30];
    char* mRiders[14];
    int mCount;
};

struct sVEntry0012AB20 { short delta; short index; int (*fn)(void*); };

extern void* D_004A289C;

extern "C" void func_0012AB20(sAiObj12AB20* self)
{
    self->m14 = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* r = *(char**)(self->mRiders[i] + 0x18);
        if (*(int*)(r + 0x884) != 0)
        {
            *(int*)(r + 0x884) = 0;
            *(int*)(r + 0x888) = -1;
            char* g = (char*)D_004A289C;
            char* rider = self->mRiders[i];
            sVEntry0012AB20* vt = *(sVEntry0012AB20**)(g + 0x10D8);
            *(int*)(*(char**)(rider + 0x18) + 0x888) = vt[114].fn(g + vt[114].delta) + 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012ABD0);
#ifdef SKIP_ASM
struct sAiObj12ABD0
{
    char pad0[0x48];
    char* mRiders[14];
    int mCount;
};

extern "C" int func_0011C0E0(void*);

extern "C" int func_0012ABD0(sAiObj12ABD0* self)
{
    int result = 1;
    for (int i = 0; i < self->mCount; i++)
    {
        if (func_0011C0E0(*(void**)(self->mRiders[i] + 0x18)) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012AC48);
#ifdef SKIP_ASM
struct sAiObj12AC48
{
    char pad0[0x5C];
    char* mRiders[10];
    int mCount;
    int m88;
    int m8C;
};

struct sVEntry0012AC48 { short delta; short index; int (*fn)(void*); };

extern void* D_004A289C;

extern "C" void func_0012AC48(sAiObj12AC48* self)
{
    self->m8C = 0;
    for (int i = 0; i < self->mCount; i++)
    {
        char* r = *(char**)(self->mRiders[i] + 0x18);
        if (*(int*)(r + 0x884) != 0)
        {
            *(int*)(r + 0x884) = 0;
            *(int*)(r + 0x888) = -1;
            char* g = (char*)D_004A289C;
            char* rider = self->mRiders[i];
            sVEntry0012AC48* vt = *(sVEntry0012AC48**)(g + 0x10D8);
            *(int*)(*(char**)(rider + 0x18) + 0x888) = vt[114].fn(g + vt[114].delta) + 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012ACF8);
#ifdef SKIP_ASM
struct sAiObj12ACF8
{
    char pad0[0x5C];
    char* mRiders[10];
    int mCount;
};

extern "C" int func_0011C0E0(void*);

extern "C" int func_0012ACF8(sAiObj12ACF8* self)
{
    int result = 1;
    for (int i = 0; i < self->mCount; i++)
    {
        if (func_0011C0E0(*(void**)(self->mRiders[i] + 0x18)) == 0)
        {
            result = 0;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012AE38);
#ifdef SKIP_ASM
struct sAiMissionVEntry { short delta; short index; void (*fn)(void*, int); };

struct sAiMission
{
    char pad0[0x14];
    int m14;
    char pad18[0x30];
    void* mListA[5];
    void* mListB[5];
    void* mListC[2];
    int mTotal;
    int pad7C;
    int mCountA;
    int mCountB;
    int mCountC;
    int m8C;
    int m90;
};

extern "C" void func_0012AE38(void* p)
{
    sAiMission* self = (sAiMission*)p;
    for (int i = 0; i < self->mCountA; i++)
    {
        void* m = self->mListA[i];
        if (m)
        {
            char* obj = *(char**)((char*)m + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListA[i] = 0;
    }
    self->mTotal -= self->mCountA;
    self->mCountA = 0;
    self->m14 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012AED0);
#ifdef SKIP_ASM
extern "C" void func_0012AED0(void* p)
{
    sAiMission* self = (sAiMission*)p;
    for (int i = 0; i < self->mCountB; i++)
    {
        void* m = self->mListB[i];
        if (m)
        {
            char* obj = *(char**)((char*)m + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListB[i] = 0;
    }
    self->mTotal -= self->mCountB;
    self->mCountB = 0;
    self->m8C = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_purgeMissionRiders);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void* p)
{
    sAiMission* self = (sAiMission*)p;
    for (int i = 0; i < self->mCountC; i++)
    {
        void* m = self->mListC[i];
        if (m)
        {
            char* obj = *(char**)((char*)m + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mListC[i] = 0;
    }
    self->mTotal -= self->mCountC;
    self->mCountC = 0;
    self->m90 = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012B000);
#ifdef SKIP_ASM
extern "C" void func_0012AED0(void*);
extern "C" void func_0010F3B8(void*);

extern "C" void func_0012B000(void* self)
{
    func_0012AED0(self);
    func_0010F3B8(self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012B030);
#ifdef SKIP_ASM
extern "C" void func_0012AE38(void*);
extern "C" void func_0010F3B8(void*);

extern "C" void func_0012B030(void* self)
{
    func_0012AE38(self);
    func_0010F3B8(self);
    *(int*)((char*)self + 0x10) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012B090);
#ifdef SKIP_ASM
extern "C" void func_00154A58(int rider, int a);
extern "C" void func_0012B200(void*);

struct sAiObj12B090
{
    char pad0[0x28];
    char* mRiders[6];
    void* mList[13];
    int m74;
    int mCount;
    int mCount2;
};

extern "C" void func_0012B090(void* p)
{
    sAiObj12B090* self = (sAiObj12B090*)p;
    for (int i = 0; i < self->mCount; i++)
    {
        func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
    }
    for (int i = 0; i < self->mCount2; i++)
    {
        void* m = self->mList[i];
        if (m)
        {
            char* obj = *(char**)((char*)m + 0x18) + 0x6C0;
            sAiMissionVEntry* vt = *(sAiMissionVEntry**)obj;
            vt[1].fn(obj + vt[1].delta, 3);
        }
        self->mList[i] = 0;
    }
    cAI_purgeMissionRiders(self);
    func_0012AE38(self);
    func_0012AED0(self);
    self->mCount2 = 0;
    self->mCount = 0;
    func_0012B200(self);
    self->m74 = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012B180);
#ifdef SKIP_ASM
extern "C" void func_00154A58(int rider, int a);
extern "C" void func_001175B8(char* self);

extern "C" void func_0012B180(sAiObj128A48* self)
{
    for (int i = 0; i < self->mCount; i++)
    {
        func_00154A58(i, *(int*)(self->mRiders[i] + 0x790) + 0xFC);
        func_001175B8(*(char**)(self->mRiders[i] + 0x790));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012B200);
#ifdef SKIP_ASM
extern void** D_004A3DF8;
extern void* D_004A3E7C;
extern "C" void func_00314FE8(void*, void*);
extern "C" void func_00311110(void*);
extern "C" void func_003112C8(void*, int);

extern "C" void func_0012B200(void* self)
{
    int i;
    for (i = 0; i < 1; i++)
    {
        void* p = *(void**)((char*)D_004A3DF8 + ((unsigned char)i << 2));
        if (p != 0)
        {
            func_00314FE8(D_004A3E7C, p);
            void** t = D_004A3DF8;
            *(void**)((char*)t + ((unsigned char)i << 2)) = 0;
            func_00311110(t);
            func_003112C8(p, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", cAI_loadAnims);
#ifdef SKIP_ASM
extern "C" void func_003DED50(char* name, int a1, int a2, void* out);
extern "C" void func_003DEDC0(void* handle, int arg);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00311250(void* mem, char* name);
extern "C" void func_00314F30(void* map, int i, void* anim);
// PORT: the original passes the map to cGameAnimMap_testResolve(), whose symbol is declared with no params.
void cGameAnimMap_testResolve_impl(void* map) __asm__("cGameAnimMap_testResolve__Fv");
struct sAnimTable { void* anims[1]; };
extern char D_00457F58[];
extern char D_00457F70[];
extern char* D_004A1100[1];

extern "C" void cAI_loadAnims(void)
{
    void* handle;
    int i;
    func_003DED50(D_00457F58, 0, 0x64, &handle);
    for (i = 0; i < 1; i++)
    {
        void* p = func_00311250(cMemMan_alloc(0x18, D_00457F70, 0, 0), D_004A1100[i]);
        ((sAnimTable*)D_004A3DF8)->anims[(unsigned char)i] = p;
        func_00314F30(D_004A3E7C, i, p);
    }
    cGameAnimMap_testResolve_impl(D_004A3E7C);
    func_003DEDC0(handle, 0x64);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012B340);
#ifdef SKIP_ASM
extern char* D_004A2EEC;
extern void* D_004A28A8;
extern "C" void func_00258950(void* self);
extern "C" void func_0026FA50(void* self);
extern "C" void func_00125108(void* self);

static inline bool aiRiderFar12B340(char* r)
{
    return *(float*)(r + 0x470) >= 10.0f;
}

extern "C" void func_0012B340(sAiObj12A250* self)
{
    if (*(int*)self == 5)
    {
        func_00258950(D_004A2EEC);
        for (int i = 0; i < self->mCount; i++)
        {
            char* rider = self->mRiders[i];
            char* r = *(char**)(rider + 0x18);
            if (*(int*)(r + 0x87C) == 0)
            {
                bool far = aiRiderFar12B340(r);
                if (far)
                {
                    return;
                }
                func_0026FA50(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28));
                *(int*)(*(char**)(rider + 0x18) + 0x480) = 1;
                func_00125108(*(void**)(rider + 0x18));
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012B420);
#ifdef SKIP_ASM
unsigned int AIrand();
extern "C" int func_00125AD8(void* self);

extern "C" int func_0012B420(sAiObj128A48* self)
{
    int sum = AIrand();
    for (int i = 0; i < self->mCount; i++)
    {
        sum += func_00125AD8(self->mRiders[i]);
    }
    return sum;
}
#endif

INCLUDE_ASM("ai/ai", func_0012B498);

//100%
INCLUDE_ASM("ai/ai", func_0012B698);
#ifdef SKIP_ASM
extern "C" void func_00120D58(void* self);
extern "C" void func_00125958(void* self, int a1);
extern "C" int func_0014F7E8();

struct sOptFlags_0012B698
{
    unsigned int f0;
    char pad4[0x14];
    int f18;
};
extern sOptFlags_0012B698 D_005308B8;

static inline int OptBit_0012B698(sOptFlags_0012B698* o, int b)
{
    return (o->f18 >> b) & 1;
}

extern "C" void func_0012B698(void* self, void* rider)
{
    int v = 1;
    if (OptBit_0012B698(&D_005308B8, 3))
        v = 3;
    else if (OptBit_0012B698(&D_005308B8, 0))
        v = 0;
    else
        if ((D_005308B8.f0 >> 5) & 1) v = 2;
    char* r = *(char**)((char*)rider + 0x18);
    *(int*)(r + 0x304) = v;
    func_00120D58(r);
    if ((D_005308B8.f0 >> 3) & 1)
        *(float*)(*(char**)((char*)rider + 0x18) + 0xB24) = 1.0f;
    if (OptBit_0012B698(&D_005308B8, 4))
        *(int*)(*(char**)((char*)rider + 0x18) + 0xB28) = 2;
    if (OptBit_0012B698(&D_005308B8, 1))
        func_00125958(*(void**)((char*)rider + 0x18), 1);
    *(int*)(*(char**)((char*)rider + 0x18) + 0xB30) = func_0014F7E8();
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012B788);
#ifdef SKIP_ASM
extern "C" void func_00120E50(void*);

struct sAi12B788
{
    char pad0[0x28];
    void* items[20];
    int count;
};

extern "C" void func_0012B788(sAi12B788* self)
{
    int i;
    for (i = 0; i < self->count; i++)
    {
        func_00120E50(self->items[i]);
    }
}
#endif

INCLUDE_ASM("ai/ai", func_0012B7F0);

INCLUDE_ASM("ai/ai", cAI_readFromReplayFrame);

//100%
INCLUDE_ASM("ai/ai", func_0012BAC0);
#ifdef SKIP_ASM
extern "C" void func_0012BAC0(void* self, int* src)
{
    int* dst = (int*)((char*)self + 0x20);
    for (int i = 0; i < 2; i++)
    {
        dst[i] = src[i];
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012BAF0);
#ifdef SKIP_ASM
extern "C" void func_0012BAF0(void* self, int* dst)
{
    int* src = (int*)((char*)self + 0x20);
    for (int i = 0; i < 2; i++)
    {
        dst[i] = src[i];
    }
}
#endif

INCLUDE_ASM("ai/ai", func_0012BB20);

INCLUDE_ASM("ai/ai", func_0012BE20);

//100%
INCLUDE_ASM("ai/ai", func_0012BF68);
#ifdef SKIP_ASM
extern "C" void func_00114130(void* rider, int a, int b);
extern "C" void func_0012C0C0(void* self, void* a1);
extern "C" void func_0012C130(void* self, void* a1);
extern "C" void func_0012C230(void* self, void* a1);
extern "C" void func_0012C408(void* self, void* a1);

extern "C" void func_0012BF68(void* self, void* a1)
{
    func_00114130(*(void**)((char*)self + 0x14), 0, 0);
    switch (*(int*)self)
    {
    case 0:
        func_0012C230(self, a1);
        break;
    case 1:
        func_0012C408(self, a1);
        break;
    case 2:
        func_0012C0C0(self, a1);
        break;
    case 3:
        func_0012C130(self, a1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C028);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" int func_0012C028(void* self)
{
    int r = 0;
    char* s = *(char**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC);
    if (*(int*)s == 5)
    {
        r = *(int*)(*(char**)((char*)self + 0x14) + 0xB30) <= (int)(*(int*)(s + 0xC) * 0.01666666753590107f);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C078);
#ifdef SKIP_ASM
extern void* D_004A28A8;

extern "C" int func_0012C078(void* self)
{
    if (**(int**)(*(char**)((char*)D_004A28A8 + 0x84) + 0xC) == 5)
        return 1;
    if (*(float*)(*(char**)((char*)self + 0x14) + 0x470) >= 0.0f)
        return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C0C0);
#ifdef SKIP_ASM
// PORT: cRiderAnimBase_play returns a value here (the target uses v1 after the call); the unit declares it void.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
struct sAiBits0012C0C0
{
    int pad : 12;
    int v : 6;
};

extern "C" void func_0012C0C0(void* self, void* a1)
{
    if (((sAiBits0012C0C0*)a1)->v * 0.032258063554763794f > 0.0f)
    {
        cRiderAnimBase_play_i(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 2, 0, -1.0f);
        *(int*)self = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C130);
#ifdef SKIP_ASM
extern "C" void func_00113F88(void* rider, float a, float b);
int func_00312AA0(void* self, int i);
extern "C" float func_00312AB0(void* self, int i);
int func_0011FE98(void* self);
// PORT: prototype mismatch. func_0011FEC8/func_0011FE78 are defined with one param, but callers pass a mode in $a1.
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void func_0011FE78_impl(void* self, int v) __asm__("func_0011FE78__FPv");

struct sVec4_12C130
{
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_12C130 vu0Scale_12C130(const sVec4_12C130& in, float s)
{
    sVec4_12C130 v = in;
    sVec4_12C130 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

extern "C" void func_0012C130(void* self, void* a1)
{
    func_00113F88(*(void**)((char*)self + 0x14), 1.0f, 0.0f);
    if (func_00312AA0(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 2) == 2)
    {
        if (func_0011FE98(*(void**)((char*)self + 0x14)) == 3)
        {
            if (func_00312AB0(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 2) > 0.5f)
            {
                char* rider = *(char**)((char*)self + 0x14);
                *(sVec4_12C130*)(rider + 0x1E0) = vu0Scale_12C130(*(sVec4_12C130*)(rider + 0x1B0), 555.5555419921875f);
                func_0011FE78_impl(*(void**)((char*)self + 0x14), 0);
            }
        }
    }
    else
    {
        if (func_0011FE98(*(void**)((char*)self + 0x14)) == 1)
        {
            func_0011FEC8_impl(*(void**)((char*)self + 0x14), 4);
        }
        else
        {
            func_0011FEC8_impl(*(void**)((char*)self + 0x14), 0);
        }
        *(int*)self = 0;
    }
}
#endif

INCLUDE_ASM("ai/ai", func_0012C230);

INCLUDE_ASM("ai/ai", func_0012C408);

//100%
INCLUDE_ASM("ai/ai", func_0012C558);
#ifdef SKIP_ASM
struct sAiVEntry12C558 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012C558(void* self, void* obj)
{
    sAiVEntry12C558* vt = *(sAiVEntry12C558**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C590);
#ifdef SKIP_ASM
struct sAiVEntry12C590 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012C590(void* self, void* obj)
{
    sAiVEntry12C590* vt = *(sAiVEntry12C590**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C5C8);
#ifdef SKIP_ASM
extern "C" float func_00119368(void* self, int a1);
extern float D_004A4C8C;

struct sAiVec4_12C5C8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float aiLength_12C5C8(const sAiVec4_12C5C8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

struct sAiObj12C5C8
{
    int m0;
    char* mRider;
};

extern "C" void func_0012C5C8(sAiObj12C5C8* self)
{
    func_00119368(*(void**)(self->mRider + 0x790), 1);
    char* r = self->mRider;
    if (D_004A4C8C < aiLength_12C5C8(*(sAiVec4_12C5C8*)(r + 0x1E0)))
    {
        self->m0 = 0;
        return;
    }
    if (*(float*)(r + 0x2DC) != 0.0f)
    {
        self->m0 = 0;
        return;
    }
    if (*(float*)(r + 0x378) < 0.800000011920929f)
    {
        self->m0 = 0;
        return;
    }
    self->m0 = 1;
}
#endif

INCLUDE_ASM("ai/ai", func_0012C678);

//100%
INCLUDE_ASM("ai/ai", func_0012C9C0);
#ifdef SKIP_ASM
struct sVEntry0012C9C0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012C9C0(void* self, void* obj)
{
    sVEntry0012C9C0* vt = *(sVEntry0012C9C0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012C9F8);
#ifdef SKIP_ASM
struct sVEntry0012C9F8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012C9F8(void* self, void* obj)
{
    sVEntry0012C9F8* vt = *(sVEntry0012C9F8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012CA30);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029E578(void* self, void* a1);
extern "C" int func_00311AE8(void*, int);
extern "C" void func_002708F0(void* self, int key, int id);
extern "C" void func_00309990(void* self);
extern void* D_004A28A8;
extern void* D_004A3DD8;
extern char D_004FF120[];

struct sVec4_12CA30
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVecPair_12CA30
{
    sVec4_12CA30 a;
    sVec4_12CA30 b;
};

extern "C" void func_00136D40(char* self, sVec4_12CA30* a, sVec4_12CA30* b, sVecPair_12CA30* c);

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_12CA30(sVec4_12CA30& dst, const sVec4_12CA30& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

struct sAiObj12CA30
{
    int f0;                 // 0x00
    char pad4[0xC];
    sVecPair_12CA30 m10;    // 0x10
    sVecPair_12CA30 m30;    // 0x30
    char pad50[0x4];
    int f54;                // 0x54
    char pad58[0x18];
    int f70;                // 0x70
    char pad74[0xC];
    char* mRider;           // 0x80
};

extern "C" void func_0012CA30(sAiObj12CA30* self)
{
    sVecPair_12CA30* p30 = &self->m30;
    *(int*)(self->mRider + 0x2EC) = 0;
    *(int*)(self->mRider + 0x2E8) = 0;
    self->f54 = 0;
    self->f70 = 0;
    func_0029E578(func_0028B180(), self->mRider);
    char* r = self->mRider;
    self->m10 = (*(sVecPair_12CA30**)(*(char**)(r + 0x780) + 0x2C))[*(int*)(r + 0x89C)];
    self->m30 = (*(sVecPair_12CA30**)(*(char**)(r + 0x780) + 0x2C))[*(int*)(r + 0x8A4)];
    vu0AddEq_12CA30(self->m10.a, *(sVec4_12CA30*)(self->mRider + 0x9D0));
    vu0AddEq_12CA30(self->m30.a, *(sVec4_12CA30*)(self->mRider + 0x9D0));
    self->f0 = 0;
    if (func_00311AE8(*(void**)(self->mRider + 0x784), 2) == 0x16)
    {
        func_00136D40(*(char**)(self->mRider + 0x77C) + 0x30, (sVec4_12CA30*)(self->mRider + 0x1E0), (sVec4_12CA30*)D_004FF120, p30);
        *(int*)(self->mRider + 0x318) = 0;
    }
    func_002708F0(*(void**)(*(char**)((char*)D_004A28A8 + 0x84) + 0x28), *(int*)(self->mRider + 0x86C), *(int*)(self->mRider + 0x790) + 0xFC);
    func_00309990(D_004A3DD8);
}
#endif

INCLUDE_ASM("ai/ai", func_0012CB68);

INCLUDE_ASM("ai/ai", func_0012CD20);

INCLUDE_ASM("ai/ai", func_0012D160);

INCLUDE_ASM("ai/ai", func_0012D4E8);

INCLUDE_ASM("ai/ai", func_0012D848);

//100%
INCLUDE_ASM("ai/ai", func_0012D9D8);
#ifdef SKIP_ASM
extern "C" void func_0010F280(void* rider, int flag);
extern "C" void func_00116120(void* rider, int a, int b);
extern "C" int func_00312AE8(void* self, int i);

static inline int AiCheck_0012D9D8(char* rider)
{
    return *(float*)(rider + 0x470) >= 0.0f;
}

extern "C" void func_0012D9D8(void* self)
{
    int flag = 0;
    if (*(float*)((char*)self + 0x70) >= 1.0f || AiCheck_0012D9D8(*(char**)((char*)self + 0x80)))
    {
        flag = 1;
    }
    if (func_00312AE8(*(void**)(*(char**)((char*)self + 0x80) + 0x784), 2) || flag)
    {
        func_0010F280(*(void**)((char*)self + 0x80), flag);
        func_00116120(*(void**)((char*)self + 0x80), 0, 2);
    }
}
#endif

INCLUDE_ASM("ai/ai", func_0012DA88);

INCLUDE_ASM("ai/ai", func_0012DCB0);

INCLUDE_ASM("ai/ai", func_0012DD98);

INCLUDE_ASM("ai/ai", func_0012DE80);

INCLUDE_ASM("ai/ai", func_0012DF48);

INCLUDE_ASM("ai/ai", func_0012E010);

INCLUDE_ASM("ai/ai", func_0012E468);

INCLUDE_ASM("ai/ai", func_0012E528);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/ai", func_0012E690);
#ifdef SKIP_ASM
struct sVEntry0012E690 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_001298C8();
extern "C" void func_0010E028(void* self, int mode, float t);
extern "C" void* func_0028B180();
extern "C" void func_002A02D8(void*, void*);

extern "C" void func_0012E690(void* self)
{
    if ((func_001298C8() & 1) == 0)
    {
        char* sub = *(char**)((char*)self + 0x80) + 0x6C0;
        sVEntry0012E690* vt = *(sVEntry0012E690**)sub;
        if (vt[8].fn(sub + vt[8].delta))
        {
            func_0010E028(*(void**)((char*)self + 0x80), 4, 0.0f);
        }
        func_002A02D8(func_0028B180(), *(void**)((char*)self + 0x80));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012E708);
#ifdef SKIP_ASM
struct sVEntry0012E708 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012E708(void* self, void* obj)
{
    sVEntry0012E708* vt = *(sVEntry0012E708**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x80);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012E740);
#ifdef SKIP_ASM
struct sVEntry0012E740 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012E740(void* self, void* obj)
{
    sVEntry0012E740* vt = *(sVEntry0012E740**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x80);
}
#endif

INCLUDE_ASM("ai/ai", func_0012E778);

//100%
INCLUDE_ASM("ai/ai", func_0012E980);
#ifdef SKIP_ASM
extern "C" void func_0012EE30(void*, void*);
extern char D_0043D788[];

extern "C" void func_0012E980(void* self)
{
    char* p = *(char**)self;
    *(float*)(p + 0x200) = 0.03333333507180214f;
    *(int*)(p + 0x204) = 0;
    func_0012EE30(self, D_0043D788);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012E9B0__FPv);
#ifdef SKIP_ASM
void func_0012E9B0(void* self)
{
}
#endif

INCLUDE_ASM("ai/ai", func_0012E9B8);

INCLUDE_ASM("ai/ai", func_0012EE30);

//100%
INCLUDE_ASM("ai/ai", func_0012F118);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12F118(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_12F118(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

extern "C" float func_0012F118(void* self, int* out)
{
    char* obj = *(char**)self;
    float y = *(float*)(obj + 0x2B0);
    float x = *(float*)(obj + 0x2A4);
    if (aiAbs_12F118(y) < 0.1f) y = 0.0f;
    if (aiAbs_12F118(x) < 0.1f) x = 0.0f;
    int zero = 0;
    if (y == 0.0f && x == 0.0f) zero = 1;
    *out = zero;
    if (zero) return 0.0f;
    return aiAtan2_12F118(y, x);
}
#endif

INCLUDE_ASM("ai/ai", func_0012F230);

INCLUDE_ASM("ai/ai", func_0012F398);

//100%
INCLUDE_ASM("ai/ai", func_0012F588);
#ifdef SKIP_ASM
extern "C" int func_0012F588(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    return *(int*)((char*)p + 0x87C) != 0 && *(int*)((char*)p + 0x870) >= 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012F5B0);
#ifdef SKIP_ASM
struct sVEntry0012F5B0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012F5B0(void* self, void* obj)
{
    sVEntry0012F5B0* vt = *(sVEntry0012F5B0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012F5E8);
#ifdef SKIP_ASM
struct sVEntry0012F5E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012F5E8(void* self, void* obj)
{
    sVEntry0012F5E8* vt = *(sVEntry0012F5E8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012F620);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_12F620(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float aiAtan2_12F620(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

extern "C" void func_0012F620(void* self)
{
    char* r = *(char**)((char*)self + 0x14);
    float b = *(float*)(r + 0x220);
    float a = aiAbs_12F620(*(float*)(r + 0x1F0));
    if (a < b) a = b;
    *(float*)((char*)self + 0x4) = a;
    float ang = aiAtan2_12F620(*(float*)(r + 0x220), *(float*)(r + 0x1F0));
    // PERM_START
    *(float*)((char*)self + 0x0) = ang;
    *(int*)((char*)self + 0x8) = 1;
    *(int*)((char*)self + 0xC) = 1;
    *(int*)((char*)self + 0x10) = -1;
    // PERM_END
    char* r1 = *(char**)((char*)self + 0x14);
    *(int*)(r1 + 0x2AC) = 0;
    *(float*)(r1 + 0x2A8) = 0.03333333507180214f;
    char* r2 = *(char**)((char*)self + 0x14);
    *(float*)(r2 + 0x2B4) = 0.03333333507180214f;
    *(int*)(r2 + 0x2B8) = 0;
}
#endif

INCLUDE_ASM("ai/ai", func_0012F730);

//100%
INCLUDE_ASM("ai/ai", func_0012FB68);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);
extern "C" void func_00311E88(void*, int, float);
extern "C" char* func_00311B20(void*, int);

extern "C" void func_0012FB68(void* self, int a1)
{
    if (a1)
    {
        if (func_00311AE8(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 1) == 3 ||
            func_00311AE8(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 1) == 0xD)
        {
            func_00311E88(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 0, 0.10000000149011612f);
            *(float*)(func_00311B20(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 1) + 0x90) = 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012FBF0);
#ifdef SKIP_ASM
struct sVEntry0012FBF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012FBF0(void* self, void* obj)
{
    sVEntry0012FBF0* vt = *(sVEntry0012FBF0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012FC28);
#ifdef SKIP_ASM
struct sVEntry0012FC28 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0012FC28(void* self, void* obj)
{
    sVEntry0012FC28* vt = *(sVEntry0012FC28**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012FC60__FPv);
#ifdef SKIP_ASM
void func_0012FC60(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)*(void**)((char*)self + 0x14) + 0x360) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

INCLUDE_ASM("ai/ai", func_0012FC80);

//100%
INCLUDE_ASM("ai/ai", func_0012FE98);
#ifdef SKIP_ASM
extern "C" void func_0012FE98(void* self)
{
    char* a = *(char**)((char*)self + 0x14);
    *(int*)(a + 0x330) = 0;
    char* b = *(char**)((char*)self + 0x14);
    *(float*)(b + 0x278) = 0.01666666753590107f;
    *(int*)(b + 0x27C) = 0;
    char* c = *(char**)((char*)self + 0x14);
    *(float*)(c + 0x26C) = 0.03333333507180214f;
    *(int*)(c + 0x270) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_0012FEC8);
#ifdef SKIP_ASM
extern "C" void func_0031BE50(float* s, float* c, float angle);
extern "C" void func_00116930(void* rider);

struct sQuat_12FEC8
{
    float x, y, z, w;
    sQuat_12FEC8() {}
    sQuat_12FEC8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sQuat_12FEC8 D_004FF130;
extern sQuat_12FEC8 D_004FF160;

static inline sQuat_12FEC8 AxisAngle_12FEC8(const sQuat_12FEC8& axis, float angle)
{
    float s, c;
    func_0031BE50(&s, &c, angle * 0.5f);
    return sQuat_12FEC8(s * axis.x, s * axis.y, s * axis.z, c);
}

extern "C" void func_0012FEC8(void* self)
{
    *(int*)(*(char**)((char*)self + 0x14) + 0x320) ^= 1;
    char* r = *(char**)((char*)self + 0x14);
    float a = 0.0f;
    if (*(int*)(r + 0x320) != 0) a = 3.1415927410125732f;
    char* anim = *(char**)(r + 0x784);
    *(sQuat_12FEC8*)(anim + 0x30) = D_004FF130;
    *(sQuat_12FEC8*)(anim + 0x40) = AxisAngle_12FEC8(D_004FF160, -a);
    char* r2 = *(char**)((char*)self + 0x14);
    *(int*)(*(char**)(r2 + 0x784) + 0x18) = *(int*)(r2 + 0x320);
    char* r3 = *(char**)((char*)self + 0x14);
    if (*(int*)(r3 + 0x328) == 1)
    {
        *(int*)(r3 + 0x328) = 2;
    }
    else if (*(int*)(r3 + 0x328) == 2)
    {
        *(int*)(r3 + 0x328) = 1;
    }
    *(float*)(*(char**)((char*)self + 0x14) + 0x280) = -*(float*)(*(char**)((char*)self + 0x14) + 0x280);
    *(float*)(*(char**)((char*)self + 0x14) + 0x288) = -*(float*)(*(char**)((char*)self + 0x14) + 0x288);
    func_00116930(*(void**)((char*)self + 0x14));
}
#endif

INCLUDE_ASM("ai/ai", func_0012FFF8);

INCLUDE_ASM("ai/ai", func_00130228);

//100%
INCLUDE_ASM("ai/ai", func_001303E0);
#ifdef SKIP_ASM
extern "C" int func_001306B0(void* self);
extern "C" int func_001307B8(void* self, void* a1);
extern "C" int func_001308D8(void* self, void* a1);
extern "C" int func_00130DD0(void* self, void* a1);
extern "C" int func_00131200(void* self, void* a1);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

extern "C" void func_001303E0(void* self, void* a1)
{
    if (func_001306B0(self) != 0) return;
    if (func_001307B8(self, a1) != 0) return;
    if (func_001308D8(self, a1) != 0) return;
    if (func_00130DD0(self, a1) != 0) return;
    if (func_00131200(self, a1) != 0)
    {
        char* r = *(char**)((char*)self + 0x14);
        *(int*)(r + 0x270) = 0;
        *(float*)(r + 0x26C) = *(float*)(r + 0x300) * 2.0f * 0.01666666753590107f;
        char* r2 = *(char**)((char*)self + 0x14);
        if (*(int*)(r2 + 0x330) == 2)
        {
            cRiderAnimBase_play(*(void**)(r2 + 0x784), 0x21, 0, -1.0f);
        }
        else
        {
            cRiderAnimBase_play(*(void**)(r2 + 0x784), 0x19, 0, -1.0f);
        }
        *(int*)self = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001304D0__FPv);
#ifdef SKIP_ASM
void func_001304D0(void* self)
{
    *(int*)self = 1;
}
#endif

INCLUDE_ASM("ai/ai", func_001304E0);

//100%
INCLUDE_ASM("ai/ai", func_001306B0);
#ifdef SKIP_ASM
int func_00312AA0(void* self, int i);
int func_0011FE98(void* self);
extern "C" int func_00114CC0(void* rider);
// PORT: cRiderAnimBase_play returns a value (the unit declares it void); int-returning view bound by asm label.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");

extern "C" int func_001306B0(void* self)
{
    int s = func_00312AA0(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 2);
    if (s == 0x17 || s == 0x1F) return 1;
    if (s == 0x25 || s == 0x26 || s == 0x1D || s == 0x1E) return 0;
    if (func_0011FE98(*(void**)((char*)self + 0x14)) != 0) return 0;
    char* r = *(char**)((char*)self + 0x14);
    if (*(float*)(r + 0x2DC) != 0.0f) return 0;
    if (func_00114CC0(r) != 0)
    {
        char* r2 = *(char**)((char*)self + 0x14);
        if (*(int*)(r2 + 0x330) == 1)
        {
            cRiderAnimBase_play_i(*(void**)(r2 + 0x784), 0x17, 0, -1.0f);
            *(int*)(*(char**)((char*)self + 0x14) + 0x330) = 2;
        }
        else
        {
            cRiderAnimBase_play_i(*(void**)(r2 + 0x784), 0x1F, 0, -1.0f);
            *(int*)(*(char**)((char*)self + 0x14) + 0x330) = 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001307B8);
#ifdef SKIP_ASM
extern "C" void func_00131428(void*);
extern "C" void func_00116930(void* rider);
extern "C" void func_00114298(void* rider, float v);
extern "C" void func_00294170(void* self, void* rider);
// PORT: prototype mismatch. func_0011FEC8/func_0011FE78 are defined with one param, but callers pass a mode in $a1.
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
void func_0011FE78_impl(void* self, int v) __asm__("func_0011FE78__FPv");
extern void* D_004A3500;

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_1307B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" int func_001307B8(void* self, void* a1)
{
    int mask = 0x20000;
    if (*(int*)a1 & mask) *(int*)((char*)self + 0x10) = 1;
    int ok = *(int*)((char*)self + 0x10) != 0 && (*(int*)a1 & mask) == 0;
    float speed = aiAbs_1307B8(*(float*)(*(char**)((char*)self + 0x14) + 0x280));
    if (!ok) return 0;
    func_00131428(self);
    if (speed > 0.5f)
    {
        cRiderAnimBase_play(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 0x23, 0, -1.0f);
    }
    else
    {
        cRiderAnimBase_play(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 0x1B, 0, -1.0f);
    }
    char* r = *(char**)((char*)self + 0x14);
    if (*(int*)(r + 0x328) != 0)
    {
        *(int*)(r + 0x328) = 0;
        func_00116930(*(void**)((char*)self + 0x14));
    }
    func_00114298(*(void**)((char*)self + 0x14), 1.0f);
    func_0011FE78_impl(*(void**)((char*)self + 0x14), 1);
    func_0011FEC8_impl(*(void**)((char*)self + 0x14), 5);
    func_00294170(D_004A3500, *(void**)((char*)self + 0x14));
    return 1;
}
#endif

INCLUDE_ASM("ai/ai", func_001308D8);

INCLUDE_ASM("ai/ai", func_00130DD0);

//100%
INCLUDE_ASM("ai/ai", func_00131200);
#ifdef SKIP_ASM
struct sAiStick_131200
{
    int pad0;
    int x : 6;
    int y : 6;
};

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_131200(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" int func_00131200(void* self, void* a1)
{
    float x = ((sAiStick_131200*)a1)->x * 0.032258063554763794f;
    float y = ((sAiStick_131200*)a1)->y * 0.032258063554763794f;
    if (x == 0.0f && aiAbs_131200(y) < 0.5f)
    {
        *(float*)((char*)self + 0xC) += *(float*)(*(char**)((char*)self + 0x14) + 0x300) * 0.01666666753590107f;
    }
    else
    {
        *(float*)((char*)self + 0xC) = 0.0f;
    }
    char* r = *(char**)((char*)self + 0x14);
    if (*(float*)((char*)self + 0xC) > 0.5f ||
        (*(int*)(r + 0x330) == 2 && x > 0.8f) ||
        (*(int*)(r + 0x330) == 1 && x < -0.8f))
    {
        *(int*)(r + 0x27C) = 0;
        *(float*)(r + 0x278) = *(float*)(r + 0x300) * 2.0f * 0.01666666753590107f;
        if (*(float*)(*(char**)((char*)self + 0x14) + 0x274) < 0.5f) return 1;
    }
    if (*(float*)(*(char**)((char*)self + 0x14) + 0x274) == 0.0f) return 1;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00131348);
#ifdef SKIP_ASM
extern "C" void func_001313A8(void*);
extern "C" void func_00131428(void*);
extern "C" float func_00119A38(void*);
extern "C" void func_0010E098(void*, int, float);

extern "C" void func_00131348(void* self)
{
    func_001313A8(self);
    func_00131428(self);
    void* r = *(void**)((char*)self + 0x14);
    int s = *(int*)((char*)r + 0x330);
    if (s >= 1 && s <= 2)
    {
        func_0010E098(*(void**)((char*)self + 0x14), 1, func_00119A38(*(void**)((char*)r + 0x790)));
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001313A8);
#ifdef SKIP_ASM
extern "C" void func_001313A8(void* self)
{
    char* r = *(char**)((char*)self + 0x14);
    if (*(float*)(r + 0x280) < -0.5f)
    {
        *(float*)(r + 0x288) = -1.0f;
        *(float*)(r + 0x284) = *(float*)(r + 0x300) * 0.02500000223517418f;
    }
    else if (*(float*)(r + 0x280) > 0.5f)
    {
        *(float*)(r + 0x288) = 1.0f;
        *(float*)(r + 0x284) = *(float*)(r + 0x300) * 0.02500000223517418f;
    }
    else
    {
        *(float*)(r + 0x288) = 0.0f;
        *(float*)(r + 0x284) = *(float*)(r + 0x300) * 0.02500000223517418f;
    }
}
#endif

INCLUDE_ASM("ai/ai", func_00131428);

//100%
INCLUDE_ASM("ai/ai", func_00131598);
#ifdef SKIP_ASM
struct sVEntry00131598 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00131598(void* self, void* obj)
{
    sVEntry00131598* vt = *(sVEntry00131598**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001315D0);
#ifdef SKIP_ASM
struct sVEntry001315D0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001315D0(void* self, void* obj)
{
    sVEntry001315D0* vt = *(sVEntry001315D0**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00131608__FPv);
#ifdef SKIP_ASM
void func_00131608(void* self)
{
    *(int*)((char*)*(void**)self + 0x35c) = 0;
    *(int*)((char*)*(void**)self + 0x360) = 0;
}
#endif

INCLUDE_ASM("ai/ai", func_00131620);

//100%
INCLUDE_ASM("ai/ai", func_00131C30);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);
extern "C" void func_00311E88(void*, int, float);
extern "C" char* func_00311B20(void*, int);

extern "C" void func_00131C30(void* self, int a1)
{
    if (a1 != 4)
    {
        if (func_00311AE8(*(void**)(*(char**)self + 0x784), 1) == 3 ||
            func_00311AE8(*(void**)(*(char**)self + 0x784), 1) == 0xD)
        {
            func_00311E88(*(void**)(*(char**)self + 0x784), 0, 0.10000000149011612f);
            *(float*)(func_00311B20(*(void**)(*(char**)self + 0x784), 1) + 0x90) = 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00131CC0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int func_0011FE98(void* self);
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");

extern "C" int func_00131CC0(void* self)
{
    if (func_0011FE98(*(void**)self) == 1)
    {
        func_0011FEC8_impl(*(void**)self, 4);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00131D08);
#ifdef SKIP_ASM
extern "C" void func_00131D08(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)(*(char**)((char*)self + 0x8) + 0x360) = 0;
    *(int*)((char*)self + 0x4) = -1;
    char* r = *(char**)((char*)self + 0x8);
    *(float*)(r + 0x200) = 0.06666667014360428f;
    *(int*)(r + 0x204) = 0;
}
#endif

INCLUDE_ASM("ai/ai", func_00131D30);

//100%
INCLUDE_ASM("ai/ai", func_00132048);
#ifdef SKIP_ASM
extern "C" void func_00132048(void* self)
{
    char* r = *(char**)((char*)self + 0x8);
    *(float*)(r + 0x23C) = 0.06666667014360428f;
    *(int*)(r + 0x240) = 0;
}
#endif

INCLUDE_ASM("ai/ai", func_00132060);

//100%
INCLUDE_ASM("ai/ai", func_00132620);
#ifdef SKIP_ASM
// PORT: prototype mismatch. func_0011FEC8 is defined with one param, but its body
// forwards $5 to func_00111538.
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
extern "C" void* func_0028B180();
extern "C" void func_00299B70(void* snd, void* rider);

extern "C" int func_00132620(void* self, int id)
{
    if (id != -1)
    {
        char* r = *(char**)((char*)self + 0x8);
        if (*(float*)(r + 0x2F0) > 0.0f && ((*(int*)(r + 0xB2C) >> 1) & 1))
        {
            *(int*)(*(char**)(r + 0x77C) + 0x394) = id;
            func_0011FEC8_impl(*(void**)((char*)self + 0x8), 0xC);
            return 1;
        }
        if (id != *(int*)((char*)self + 0x4))
        {
            func_00299B70(func_0028B180(), *(void**)((char*)self + 0x8));
        }
    }
    *(int*)((char*)self + 0x4) = id;
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001326C8);
#ifdef SKIP_ASM
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

extern "C" void func_001326C8(void* self, int alt)
{
    char* r = *(char**)((char*)self + 0x8);
    int s = *(int*)(r + 0x328);
    if (s == 4)
    {
        if (alt)
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x45, 0, -1.0f);
        else
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x14, 0, -1.0f);
    }
    else if (s == 3)
    {
        if (alt)
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x46, 0, -1.0f);
        else
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x13, 0, -1.0f);
    }
    else
    {
        if (alt)
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x44, 0, -1.0f);
        else
            cRiderAnimBase_play(*(void**)(r + 0x784), 0x12, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00132770);
#ifdef SKIP_ASM
int func_0011FE98(void* self);
// PORT: prototype mismatch. func_0011FEC8 is defined with one param, but its body
// forwards $5 to func_00111538.
void func_0011FEC8_impl(void* self, int v) __asm__("func_0011FEC8__FPv");
extern "C" int func_00311AE8(void*, int);
extern "C" char* func_00311B20(void*, int);
extern "C" bool func_001446A0(void* self, int bit);
extern "C" void func_00115640(void* rider);

struct sAiBits00132770
{
    int v : 6;
};

extern "C" int func_00132770(void* self, void* a1)
{
    if (func_0011FE98(*(void**)((char*)self + 0x8)) != 1)
    {
        return 0;
    }
    if (func_00311AE8(*(void**)(*(char**)((char*)self + 0x8) + 0x784), 2) == 0xE)
    {
        if (!func_001446A0(func_00311B20(*(void**)(*(char**)((char*)self + 0x8) + 0x784), 2) + 0xB0, 0))
        {
            return 0;
        }
    }
    func_00115640(*(void**)((char*)self + 0x8));
    if (((sAiBits00132770*)((char*)a1 + 4))->v * 0.032258063554763794f != 0.0f)
    {
        func_0011FEC8_impl(*(void**)((char*)self + 0x8), 5);
    }
    else
    {
        func_0011FEC8_impl(*(void**)((char*)self + 0x8), 4);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00132840);
#ifdef SKIP_ASM
struct sVEntry00132840 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00132840(void* self, void* obj)
{
    sVEntry00132840* vt = *(sVEntry00132840**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00132878);
#ifdef SKIP_ASM
struct sVEntry00132878 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00132878(void* self, void* obj)
{
    sVEntry00132878* vt = *(sVEntry00132878**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001328B0);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029DC48(void* self, void* rider);
extern "C" float func_00312790(char* self, int a1);
extern "C" void func_00132FB8(void* self, int a1, int a2);
extern "C" void func_00311E88(void*, int, float);
extern char D_00459FC8[];

extern "C" void func_001328B0(void* self)
{
    float k = 0.05000000447034836f;
    float blend = 0.33000001311302185f;
    char* r = *(char**)((char*)self + 0x14);
    *(float*)(r + 0x200) = k;
    *(int*)(r + 0x204) = 0;
    char* r2 = *(char**)((char*)self + 0x14);
    *(float*)(r2 + 0x1F4) = k;
    *(int*)(r2 + 0x1F8) = 0;
    *(int*)(*(char**)((char*)self + 0x14) + 0x32C) = 1;
    func_0029DC48(func_0028B180(), *(void**)((char*)self + 0x14));
    char* r3 = *(char**)((char*)self + 0x14);
    float d = func_00312790(*(char**)(r3 + 0x784), *(int*)(D_00459FC8 + *(int*)((char*)self + 0x10) * 8 + *(int*)(r3 + 0x32C) * 16));
    *(float*)(*(char**)(*(char**)((char*)self + 0x14) + 0x784) + 0x1C) = d / *(float*)self;
    func_00132FB8(self, *(int*)(*(char**)((char*)self + 0x14) + 0x32C), *(int*)((char*)self + 0x10));
    *(float*)(*(char**)(*(char**)((char*)self + 0x14) + 0x784) + 0x1C) = 1.0f;
    *(int*)((char*)self + 0x4) = 0;
    func_00311E88(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 1, blend);
    func_00311E88(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 0, blend);
}
#endif

INCLUDE_ASM("ai/ai", func_001329B0);

INCLUDE_ASM("ai/ai", func_00132A30);

//100%
INCLUDE_ASM("ai/ai", func_00132F98);
#ifdef SKIP_ASM
extern "C" void func_00132F98(void* self)
{
    char* a = *(char**)((char*)self + 0x14);
    *(int*)(a + 0x32C) = 0;
    char* b = *(char**)((char*)self + 0x14);
    *(float*)(b + 0x248) = 0.05000000447034836f;
    *(int*)(b + 0x24C) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00132FB8);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* self, float angle);

struct sAnimEntry_132FB8
{
    int anim;
    int flip;
};
// Typed view of D_00459FC8 (func_001328B0 reads it as raw bytes); bound by asm label.
extern sAnimEntry_132FB8 D_00459FC8_tbl[][2] __asm__("D_00459FC8");

struct sVec4_132FB8
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVec3_132FB8
{
    float x, y, z;
    sVec3_132FB8(sVec4_132FB8 v) { x = v.x; y = v.y; z = v.z; }
};

struct sQuat_132FB8
{
    float x, y, z, w;
    sQuat_132FB8() {}
    sQuat_132FB8(const sVec3_132FB8& v, float aw) { x = v.x; y = v.y; z = v.z; w = aw; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQuat_132FB8 QuatMul_132FB8(const sQuat_132FB8& a, const sQuat_132FB8& b)
{
    sQuat_132FB8 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" void func_00132FB8(void* self, int a1, int a2)
{
    if (D_00459FC8_tbl[a1][a2].flip != 0)
    {
        char* r = *(char**)((char*)self + 0x14);
        sQuat_132FB8 t = QuatMul_132FB8(sQuat_132FB8(sVec3_132FB8(*(sVec4_132FB8*)(r + 0x1C0)), 0.0f), *(sQuat_132FB8*)(r + 0x120));
        *(sQuat_132FB8*)(*(char**)((char*)self + 0x14) + 0x120) = t;
        cRider_updateOrientationImplicit(*(void**)((char*)self + 0x14));
        cRiderAnimBase_changeHeadingOffset(*(void**)(*(char**)((char*)self + 0x14) + 0x784), 3.1415927410125732f);
    }
    cRiderAnimBase_play(*(void**)(*(char**)((char*)self + 0x14) + 0x784), D_00459FC8_tbl[a1][a2].anim, 0, -1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001330B8);
#ifdef SKIP_ASM
struct sVEntry001330B8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001330B8(void* self, void* obj)
{
    sVEntry001330B8* vt = *(sVEntry001330B8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x14);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_001330F0);
#ifdef SKIP_ASM
struct sVEntry001330F0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001330F0(void* self, void* obj)
{
    sVEntry001330F0* vt = *(sVEntry001330F0**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x14);
}
#endif

INCLUDE_ASM("ai/ai", func_00133128);

INCLUDE_ASM("ai/ai", func_00133308);

//100%
INCLUDE_ASM("ai/ai", func_00134CB0);
#ifdef SKIP_ASM
extern "C" void func_00311E88(void*, int, float);
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void func_00134DD0(void* self, void* pos);

extern "C" void func_00134CB0(void* self)
{
    func_00311E88(*(void**)(*(char**)((char*)self + 0x58) + 0x784), 1, 0.33000001311302185f);
    char* r = *(char**)((char*)self + 0x58);
    *(int*)(r + 0x2AC) = 0;
    *(int*)(r + 0x2A4) = 0;
    *(int*)(r + 0x2A8) = 0;
    char* r2 = *(char**)((char*)self + 0x58);
    *(int*)(r2 + 0x2B8) = 0;
    *(int*)(r2 + 0x2B0) = 0;
    *(int*)(r2 + 0x2B4) = 0;
    char* r3 = *(char**)((char*)self + 0x58);
    *(float*)(r3 + 0x290) = 0.01666666753590107f;
    *(int*)(r3 + 0x294) = 0;
    char* r4 = *(char**)((char*)self + 0x58);
    *(float*)(r4 + 0x29C) = 0.01666666753590107f;
    *(int*)(r4 + 0x2A0) = 0;
    func_00134DD0(self, *(char**)((char*)self + 0x58) + 0x110);
    cRider_updateOrientationImplicit(*(void**)((char*)self + 0x58));
    *(float*)(*(char**)((char*)self + 0x58) + 0x2DC) *= 0.7500380277633667f;
    char* r5 = *(char**)((char*)self + 0x58);
    float a = *(float*)(r5 + 0x2DC);
    if (a > 4.7123894691467285f)
        *(float*)(r5 + 0x2DC) = 7.853982448577881f;
    else if (a > 3.1415929794311523f)
        *(float*)(r5 + 0x2DC) = 4.7123894691467285f;
    else if (a > 0.0f)
        *(float*)(r5 + 0x2DC) = 1.5707964897155762f;
    else
        *(float*)(r5 + 0x2DC) = 0.0f;
    *(int*)(*(char**)((char*)self + 0x58) + 0x2E0) = 0;
    if (*(float*)((char*)self + 0x1C) > *(float*)((char*)self + 0x14))
    {
        *(float*)(*(char**)((char*)self + 0x58) + 0x2DC) = -*(float*)(*(char**)((char*)self + 0x58) + 0x2DC);
    }
}
#endif

INCLUDE_ASM("ai/ai", func_00134DD0);

//100%
INCLUDE_ASM("ai/ai", func_00135180);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);

static inline float aiAtan2_135180(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f) return y;
        if (y >= 0.0f) return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f) r += 3.1415927410125732f;
        else r -= 3.1415927410125732f;
    }
    return r;
}

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135180(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float aiSqrt_135180(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float aiClamp_135180(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

extern "C" float func_00135180(void* self)
{
    char* r = *(char**)((char*)self + 0x58);
    float x = *(float*)(r + 0x2DC);
    float y = *(float*)(r + 0x2E0);
    float ax = aiAbs_135180(x);
    float ay = aiAbs_135180(y);
    float mag = aiSqrt_135180(x * x + y * y) * 0.14284929633140564f;
    float ang = aiAtan2_135180(ay, ax);
    float c = aiClamp_135180(mag - 0.30000001192092896f, 0.0f, 0.699999988079071f);
    return c * 1.4285714626312256f * (1.0f - aiAbs_135180(ang - 0.7853982448577881f) / 0.7853982448577881f);
}
#endif

INCLUDE_ASM("ai/ai", func_001352A8);

//100%
INCLUDE_ASM("ai/ai", func_00135B30);
#ifdef SKIP_ASM
extern "C" void func_00135B30(void* self, float a, float b)
{
    char* s = (char*)self;
    if (b > 0.0f)
        *(float*)(s + 0x14) = 3.1415929794311523f;
    else if (b < 0.0f)
        *(float*)(s + 0x14) = -3.1415929794311523f;
    else
        *(float*)(s + 0x14) = 0.0f;
    if (a > 0.0f)
        *(float*)(s + 0x10) = 6.283185958862305f;
    else if (a < 0.0f)
        *(float*)(s + 0x10) = -6.283185958862305f;
    else
        *(float*)(s + 0x10) = 0.0f;
    *(float*)(s + 0x1C) += *(float*)(s + 0x28);
    *(float*)(s + 0x18) += *(float*)(s + 0x2C);
    *(float*)(s + 0x20) += *(float*)(s + 0x28);
    *(float*)(s + 0x24) += *(float*)(s + 0x2C);
    *(float*)(s + 0x28) = 0.0f;
    *(float*)(s + 0x2C) = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135BE0);
#ifdef SKIP_ASM
extern "C" int func_00114DB8(void* rider);
extern "C" void cRiderAnimBase_play(void* self, int anim, int flags, float blend);

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float aiFloor_135BE0(float x)
{
    float t;
    __asm__("cvt.w.s %0, %1\n\tcvt.s.w %0, %0" : "=f"(t) : "f"(x));
    if (x < t)
    {
        t -= 1.0f;
    }
    return t;
}

static inline float aiWrap_135BE0(float x)
{
    return x - aiFloor_135BE0(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

extern "C" int func_00135BE0(void* self)
{
    char* s = (char*)self;
    if (func_00114DB8(*(void**)(s + 0x58)) != 0)
    {
    *(float*)(s + 0x20) = aiWrap_135BE0(*(float*)(s + 0x20) + 3.1415927410125732f);
    *(float*)(s + 0x24) = -*(float*)(s + 0x24);
    *(float*)(s + 0x2C) = -*(float*)(s + 0x2C);
    *(float*)(s + 0x18) = -*(float*)(s + 0x18);
    *(float*)(s + 0x10) = -*(float*)(s + 0x10);
    cRiderAnimBase_play(*(void**)(*(char**)(s + 0x58) + 0x784), 0x120, 0, -1.0f);
    return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135CB0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float func_001495A8(void* self, int rider, int value);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135CB0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void func_00135CB0(void* self, float* a, float* b)
{
    float pi = 3.1415929794311523f;
    float d = aiAbs_135CB0(*(float*)((char*)self + 0x1C) - *(float*)((char*)self + 0x14));
    if (d > pi) d -= pi;
    *a = d / *(float*)((char*)self + 0x38);
    *b = pi / *(float*)((char*)self + 0x38);
    if (*(float*)(*(char**)((char*)self + 0x58) + 0x2EC) > 0.0f)
    {
        *b *= 0.5000624656677246f;
        *a *= 0.5000624656677246f;
    }
    float s = func_001495A8(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(*(char**)((char*)self + 0x58) + 0x86C), *(int*)(*(char**)((char*)self + 0x58) + 0xB34));
    float m = 1.0f / (s * 0.5002591609954834f + 1.0f);
    *b *= m;
    *a *= m;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135DB0);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" float func_001495A8(void* self, int rider, int value);

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float aiAbs_135DB0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

extern "C" void func_00135DB0(void* self, float* a, float* b)
{
    float twopi = 6.283185958862305f;
    float d = aiAbs_135DB0(*(float*)((char*)self + 0x18) - *(float*)((char*)self + 0x10));
    if (d > twopi) d -= twopi;
    *a = d / *(float*)((char*)self + 0x3C);
    *b = twopi / *(float*)((char*)self + 0x3C);
    if (*(float*)(*(char**)((char*)self + 0x58) + 0x2EC) > 0.0f)
    {
        *b *= 0.5000624656677246f;
        *a *= 0.5000624656677246f;
    }
    float s = func_001495A8(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(*(char**)((char*)self + 0x58) + 0x86C), *(int*)(*(char**)((char*)self + 0x58) + 0xB34));
    float m = 1.0f / (s * 0.5002591609954834f + 1.0f);
    *b *= m;
    *a *= m;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135EB0);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);

extern "C" int func_00135EB0(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x4) == 2)
    {
        r = func_00311AE8(*(void**)(*(char**)((char*)self + 0x58) + 0x784), 2) == 0x12;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135EF0);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);

extern "C" int func_00135EF0(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x4) == 4)
    {
        r = func_00311AE8(*(void**)(*(char**)((char*)self + 0x58) + 0x784), 2) == 0x13;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00135F30);
#ifdef SKIP_ASM
extern "C" int func_00311AE8(void*, int);

extern "C" int func_00135F30(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x4) == 4)
    {
        r = func_00311AE8(*(void**)(*(char**)((char*)self + 0x58) + 0x784), 2) == 0x14;
    }
    return r;
}
#endif

INCLUDE_ASM("ai/ai", func_00135F70);

//100%
INCLUDE_ASM("ai/ai", func_001360C8);
#ifdef SKIP_ASM
extern "C" float func_001360C8(void* self, int* out)
{
    int b = *(float*)((char*)self + 0x50) == 1.0000000150474662e+30f;
    *out = b;
    if (b)
        return 0.0f;
    return *(float*)((char*)self + 0x50);
}
#endif

INCLUDE_ASM("ai/ai", func_00136100);

INCLUDE_ASM("ai/ai", func_00136168);

//100%
INCLUDE_ASM("ai/ai", func_001361F8);
#ifdef SKIP_ASM
struct sVEntry001361F8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001361F8(void* self, void* obj)
{
    sVEntry001361F8* vt = *(sVEntry001361F8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x58);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00136230);
#ifdef SKIP_ASM
struct sVEntry00136230 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00136230(void* self, void* obj)
{
    sVEntry00136230* vt = *(sVEntry00136230**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x58);
}
#endif

INCLUDE_ASM("ai/ai", func_00136268);

INCLUDE_ASM("ai/ai", func_00136508);

//100%
INCLUDE_ASM("ai/ai", func_001368E8);
#ifdef SKIP_ASM
struct sVEntry001368E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_001368E8(void* self, void* obj)
{
    sVEntry001368E8* vt = *(sVEntry001368E8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x8);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00136920);
#ifdef SKIP_ASM
struct sVEntry00136920 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00136920(void* self, void* obj)
{
    sVEntry00136920* vt = *(sVEntry00136920**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x8);
}
#endif

extern "C" void cRider_updateOrientationImplicit(void*);

//100%
INCLUDE_ASM("ai/ai", func_00136958);
#ifdef SKIP_ASM
extern "C" void func_00136958(void* self)
{
    cRider_updateOrientationImplicit(*(void**)self);
}
#endif

//100%
INCLUDE_ASM("ai/ai", func_00136978__FPv);
#ifdef SKIP_ASM
void func_00136978(void* self)
{
}
#endif

INCLUDE_ASM("ai/ai", func_00136980);

INCLUDE_ASM("ai/ai", func_00136AD0);

