#include "common.h"

INCLUDE_ASM("ai/ai", cAI_cAI);

INCLUDE_ASM("ai/ai", func_001287B8);

INCLUDE_ASM("ai/ai", func_00128818);

INCLUDE_ASM("ai/ai", func_00128958);

INCLUDE_ASM("ai/ai", func_00128998);

extern "C" void* func_001297C8(void*, int);

//100%
INCLUDE_ASM("ai/ai", func_001289F0__FPv);
#ifdef SKIP_ASM
void* func_001289F0(void* self)
{
    return func_001297C8(self, 0);
}
#endif

INCLUDE_ASM("ai/ai", func_00128A10);

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

INCLUDE_ASM("ai/ai", func_00129160);

INCLUDE_ASM("ai/ai", func_001291E0);

INCLUDE_ASM("ai/ai", func_001296F8);

INCLUDE_ASM("ai/ai", func_00129768);

INCLUDE_ASM("ai/ai", func_001297C8);

INCLUDE_ASM("ai/ai", func_001298C8);

INCLUDE_ASM("ai/ai", cAI_InitPlayers);

INCLUDE_ASM("ai/ai", cAI_initMissionRiders);

INCLUDE_ASM("ai/ai", cAI_initComputerRiders);

INCLUDE_ASM("ai/ai", cAI_initComputerActors);

INCLUDE_ASM("ai/ai", func_0012A180);

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

INCLUDE_ASM("ai/ai", func_0012A490);

INCLUDE_ASM("ai/ai", func_0012AB20);

INCLUDE_ASM("ai/ai", func_0012ABD0);

INCLUDE_ASM("ai/ai", func_0012AC48);

INCLUDE_ASM("ai/ai", func_0012ACF8);

INCLUDE_ASM("ai/ai", func_0012AE38);

INCLUDE_ASM("ai/ai", func_0012AED0);

INCLUDE_ASM("ai/ai", cAI_purgeMissionRiders);

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

INCLUDE_ASM("ai/ai", func_0012B090);

INCLUDE_ASM("ai/ai", func_0012B180);

INCLUDE_ASM("ai/ai", func_0012B200);

INCLUDE_ASM("ai/ai", cAI_loadAnims);

INCLUDE_ASM("ai/ai", func_0012B340);

INCLUDE_ASM("ai/ai", func_0012B420);

INCLUDE_ASM("ai/ai", func_0012B498);

INCLUDE_ASM("ai/ai", func_0012B698);

INCLUDE_ASM("ai/ai", func_0012B788);

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

INCLUDE_ASM("ai/ai", func_0012BF68);

INCLUDE_ASM("ai/ai", func_0012C028);

INCLUDE_ASM("ai/ai", func_0012C078);

INCLUDE_ASM("ai/ai", func_0012C0C0);

INCLUDE_ASM("ai/ai", func_0012C130);

INCLUDE_ASM("ai/ai", func_0012C230);

INCLUDE_ASM("ai/ai", func_0012C408);

INCLUDE_ASM("ai/ai", func_0012C558);

INCLUDE_ASM("ai/ai", func_0012C590);

INCLUDE_ASM("ai/ai", func_0012C5C8);

INCLUDE_ASM("ai/ai", func_0012C678);

INCLUDE_ASM("ai/ai", func_0012C9C0);

INCLUDE_ASM("ai/ai", func_0012C9F8);

INCLUDE_ASM("ai/ai", func_0012CA30);

INCLUDE_ASM("ai/ai", func_0012CB68);

INCLUDE_ASM("ai/ai", func_0012CD20);

INCLUDE_ASM("ai/ai", func_0012D160);

INCLUDE_ASM("ai/ai", func_0012D4E8);

INCLUDE_ASM("ai/ai", func_0012D848);

INCLUDE_ASM("ai/ai", func_0012D9D8);

INCLUDE_ASM("ai/ai", func_0012DA88);

INCLUDE_ASM("ai/ai", func_0012DCB0);

INCLUDE_ASM("ai/ai", func_0012DD98);

INCLUDE_ASM("ai/ai", func_0012DE80);

INCLUDE_ASM("ai/ai", func_0012DF48);

INCLUDE_ASM("ai/ai", func_0012E010);

INCLUDE_ASM("ai/ai", func_0012E468);

INCLUDE_ASM("ai/ai", func_0012E528);

INCLUDE_ASM("ai/ai", func_0012E690);

INCLUDE_ASM("ai/ai", func_0012E708);

INCLUDE_ASM("ai/ai", func_0012E740);

INCLUDE_ASM("ai/ai", func_0012E778);

INCLUDE_ASM("ai/ai", func_0012E980);

//100%
INCLUDE_ASM("ai/ai", func_0012E9B0__FPv);
#ifdef SKIP_ASM
void func_0012E9B0(void* self)
{
}
#endif

INCLUDE_ASM("ai/ai", func_0012E9B8);

INCLUDE_ASM("ai/ai", func_0012EE30);

INCLUDE_ASM("ai/ai", func_0012F118);

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

INCLUDE_ASM("ai/ai", func_0012F5B0);

INCLUDE_ASM("ai/ai", func_0012F5E8);

INCLUDE_ASM("ai/ai", func_0012F620);

INCLUDE_ASM("ai/ai", func_0012F730);

INCLUDE_ASM("ai/ai", func_0012FB68);

INCLUDE_ASM("ai/ai", func_0012FBF0);

INCLUDE_ASM("ai/ai", func_0012FC28);

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

INCLUDE_ASM("ai/ai", func_0012FE98);

INCLUDE_ASM("ai/ai", func_0012FEC8);

INCLUDE_ASM("ai/ai", func_0012FFF8);

INCLUDE_ASM("ai/ai", func_00130228);

INCLUDE_ASM("ai/ai", func_001303E0);

//100%
INCLUDE_ASM("ai/ai", func_001304D0__FPv);
#ifdef SKIP_ASM
void func_001304D0(void* self)
{
    *(int*)self = 1;
}
#endif

INCLUDE_ASM("ai/ai", func_001304E0);

INCLUDE_ASM("ai/ai", func_001306B0);

INCLUDE_ASM("ai/ai", func_001307B8);

INCLUDE_ASM("ai/ai", func_001308D8);

INCLUDE_ASM("ai/ai", func_00130DD0);

INCLUDE_ASM("ai/ai", func_00131200);

INCLUDE_ASM("ai/ai", func_00131348);

INCLUDE_ASM("ai/ai", func_001313A8);

INCLUDE_ASM("ai/ai", func_00131428);

INCLUDE_ASM("ai/ai", func_00131598);

INCLUDE_ASM("ai/ai", func_001315D0);

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

INCLUDE_ASM("ai/ai", func_00131C30);

INCLUDE_ASM("ai/ai", func_00131CC0);

INCLUDE_ASM("ai/ai", func_00131D08);

INCLUDE_ASM("ai/ai", func_00131D30);

INCLUDE_ASM("ai/ai", func_00132048);

INCLUDE_ASM("ai/ai", func_00132060);

INCLUDE_ASM("ai/ai", func_00132620);

INCLUDE_ASM("ai/ai", func_001326C8);

INCLUDE_ASM("ai/ai", func_00132770);

INCLUDE_ASM("ai/ai", func_00132840);

INCLUDE_ASM("ai/ai", func_00132878);

INCLUDE_ASM("ai/ai", func_001328B0);

INCLUDE_ASM("ai/ai", func_001329B0);

INCLUDE_ASM("ai/ai", func_00132A30);

INCLUDE_ASM("ai/ai", func_00132F98);

INCLUDE_ASM("ai/ai", func_00132FB8);

INCLUDE_ASM("ai/ai", func_001330B8);

INCLUDE_ASM("ai/ai", func_001330F0);

INCLUDE_ASM("ai/ai", func_00133128);

INCLUDE_ASM("ai/ai", func_00133308);

INCLUDE_ASM("ai/ai", func_00134CB0);

INCLUDE_ASM("ai/ai", func_00134DD0);

INCLUDE_ASM("ai/ai", func_00135180);

INCLUDE_ASM("ai/ai", func_001352A8);

INCLUDE_ASM("ai/ai", func_00135B30);

INCLUDE_ASM("ai/ai", func_00135BE0);

INCLUDE_ASM("ai/ai", func_00135CB0);

INCLUDE_ASM("ai/ai", func_00135DB0);

INCLUDE_ASM("ai/ai", func_00135EB0);

INCLUDE_ASM("ai/ai", func_00135EF0);

INCLUDE_ASM("ai/ai", func_00135F30);

INCLUDE_ASM("ai/ai", func_00135F70);

INCLUDE_ASM("ai/ai", func_001360C8);

INCLUDE_ASM("ai/ai", func_00136100);

INCLUDE_ASM("ai/ai", func_00136168);

INCLUDE_ASM("ai/ai", func_001361F8);

INCLUDE_ASM("ai/ai", func_00136230);

INCLUDE_ASM("ai/ai", func_00136268);

INCLUDE_ASM("ai/ai", func_00136508);

INCLUDE_ASM("ai/ai", func_001368E8);

INCLUDE_ASM("ai/ai", func_00136920);

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

