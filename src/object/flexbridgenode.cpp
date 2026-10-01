#include "common.h"

INCLUDE_ASM("object/flexbridgenode", cFlexBridgeNode_setupGrid);

INCLUDE_ASM("object/flexbridgenode", func_00346D38);

INCLUDE_ASM("object/flexbridgenode", func_00346E38);

INCLUDE_ASM("object/flexbridgenode", func_003470D0);

INCLUDE_ASM("object/flexbridgenode", func_00347268);

//100%
INCLUDE_ASM("object/flexbridgenode", func_003475A8);
#ifdef SKIP_ASM
extern "C" void func_00353FC0(void*);
extern "C" void func_003475D8(void*);

extern "C" void func_003475A8(void* self)
{
    func_00353FC0((char*)self + 0x50);
    func_003475D8(self);
}
#endif

INCLUDE_ASM("object/flexbridgenode", func_003475D8);

INCLUDE_ASM("object/flexbridgenode", func_00347B80);

INCLUDE_ASM("object/flexbridgenode", func_00347D38);

INCLUDE_ASM("object/flexbridgenode", func_00347D90);

INCLUDE_ASM("object/flexbridgenode", func_00347EA8);

INCLUDE_ASM("object/flexbridgenode", func_00347F90);

INCLUDE_ASM("object/flexbridgenode", func_00348008);

INCLUDE_ASM("object/flexbridgenode", func_00348058);

INCLUDE_ASM("object/flexbridgenode", func_003480C8);

INCLUDE_ASM("object/flexbridgenode", func_00348290);

INCLUDE_ASM("object/flexbridgenode", func_00348B40);

