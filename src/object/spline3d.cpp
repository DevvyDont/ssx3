#include "common.h"

struct sSplinePoint {
    char pad_0x00[0xC];
    float field_0xC;
    char pad_0x10[0x64 - 0x10];
    sSplinePoint* next; // 0x64
    char pad_0x68[0x84 - 0x68];
    float field_0x84;
};

struct sSplineHeader {
    char pad_0x00[0x20];
    int numPoints; // 0x20
    sSplinePoint* first; // 0x24
};

struct sSplineNode {
    char pad_0x00[0x68];
    sSplineHeader* header; // 0x68
};

struct cSpline {
    char pad_0x00[0x8];
    sSplineNode* node; // 0x8
};

//100%
INCLUDE_ASM("object/spline3d", cSpline_calcLength__FP7cSpline);
#ifdef SKIP_ASM
float cSpline_calcLength(cSpline* self)
{
    sSplineHeader* header = self->node->header;
    sSplinePoint* cur = header->first;
    for (int i = 1; i < header->numPoints; i++) {
        cur = cur->next;
    }
    return cur->field_0x84 + cur->field_0xC;
}
#endif

INCLUDE_ASM("object/spline3d", func_00345538);

INCLUDE_ASM("object/spline3d", cSpline_readFromReplayFrame);

INCLUDE_ASM("object/spline3d", func_00345638);

