#include "visualfx/crowdrender2d.h"
#include <stdint.h>

//100%
//https://decomp.me/scratch/pOTwa

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_cCrowdRender2D__Fi);
#ifdef SKIP_ASM
int cCrowdRender2D_cCrowdRender2D(int param_1) {
	cCrowdRender2D_init();
	return param_1;
}
#endif

//100%
//https://decomp.me/scratch/zsTkS
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D__cCrowdRender2D__FPii);
#ifdef SKIP_ASM
void cCrowdRender2D__cCrowdRender2D(int* param_1, int param_2)
{
	cCrowdRender2D_purge(param_1);

	if ((param_2 & 1) != 0) {
		operator_delete(param_1);
	}

	return;
}
#endif

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_init__Fv);
#ifdef SKIP_ASM
void cCrowdRender2D_init()
{

}
#endif

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_purge__FPi);
#ifdef SKIP_ASM
int cCrowdRender2D_purge(int *param_1)
{
	return *param_1;
}
#endif

//100%
//https://decomp.me/scratch/Rbiv4
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdRender2D_constructCrowdAnim2D__FPv);
#ifdef SKIP_ASM

void* cMemMan_alloc(int a, const char* b, uint32_t c, int d);

void* cCrowdRender2D_constructCrowdAnim2D(void* param) {
	void* memory = cMemMan_alloc(0x20, D_004875D8, 0x20000000, 0);
	return cCrowdAnim2D_cCrowdAnim2D(memory, param);
}
#endif

//100%
INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_cCrowdAnim2D__FPvT0);
#ifdef SKIP_ASM
struct sCrowdAnim2D {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    void* f1C;
};

extern int D_00445DD0[];
extern char* D_004A5B64;

void* cCrowdAnim2D_cCrowdAnim2D(void* memory, void* param_1)
{
    sCrowdAnim2D* a = (sCrowdAnim2D*)memory;
    a->f1C = param_1;
    a->f8 = 4;
    a->f4 = 0;
    a->fC = 0;
    a->f10 = D_00445DD0[0];
    a->f0 = 0;
    a->f18 = *(int*)(D_004A5B64 + 0x1C);
    a->f14 = 3;
    return a;
}
#endif

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_advanceFrame);

INCLUDE_ASM("visualfx/crowdrender2d", cCrowdAnim2D_update);

