#include "common.h"

INCLUDE_ASM("camera/cameracontroller", cCameraController_cCameraController);

INCLUDE_ASM("camera/cameracontroller", func_0015CC10);

INCLUDE_ASM("camera/cameracontroller", func_0015CC70);

INCLUDE_ASM("camera/cameracontroller", func_0015CCF0);

extern "C" void* func_0015CB08(int);

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015CD60__FPv);
#ifdef SKIP_ASM
void* func_0015CD60(void* self)
{
    return func_0015CB08(*(int*)((char*)self + 0x8));
}
#endif

INCLUDE_ASM("camera/cameracontroller", cManualCameraController_cManualCameraController);

INCLUDE_ASM("camera/cameracontroller", func_0015CE10);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", func_0015CE98);
#ifdef SKIP_ASM
class cCamCtrlVObj_0015CE98 {
public:
    char pad[0x10];
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03(int a);
};

extern "C" void func_0015CC70(void* self);

extern "C" void func_0015CE98(void* self, int a)
{
    func_0015CC70(self);
    (*(cCamCtrlVObj_0015CE98**)((char*)self + 0x18))->v03(a);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/cameracontroller", func_0015CEE8);
#ifdef SKIP_ASM
class cCamCtrlVObj {
public:
    char pad[0x10];
    // vptr at 0x10 (g++ 2.95 places it after the class's own data)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
};

extern "C" void func_0015CCF0(void* self);

extern "C" void func_0015CEE8(void* self)
{
    func_0015CCF0(self);
    (*(cCamCtrlVObj**)((char*)self + 0x18))->v04();
}
#endif

INCLUDE_ASM("camera/cameracontroller", func_0015CF28);

INCLUDE_ASM("camera/cameracontroller", func_0015CF80);

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015CFE8);
#ifdef SKIP_ASM
extern "C" void func_0015CFE8(void* self)
{
    void** p = *(void***)((char*)self + 0x8);
    cCamCtrlVObj* obj = *(cCamCtrlVObj**)*p;
    obj->v05();
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015D020);
#ifdef SKIP_ASM
extern "C" int func_0015D020(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x30) & 2) {
        int s = *(int*)((char*)self + 0x2C);
        if (s != 0x5D) {
            r = s != 0x3C;
        }
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("camera/cameracontroller", func_0015D050);
#ifdef SKIP_ASM
extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

extern "C" void* func_0015D050(void* self, int a1, int a2)
{
    return cChaseCameraController_createChaseAlgorithmBlend(self, a1, a2, 1.0f);
}
#endif

