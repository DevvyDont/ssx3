#include "common.h"

INCLUDE_ASM("animation/mdfarchive", cMdfArchive_getModelPartByIndex);

INCLUDE_ASM("animation/mdfarchive", func_00314D60);

INCLUDE_ASM("animation/mdfarchive", func_00314D98);

INCLUDE_ASM("animation/mdfarchive", func_00314DD8);

//100%
INCLUDE_ASM("animation/mdfarchive", func_00314E88);
#ifdef SKIP_ASM
struct sMdfIndex_00314E88
{
    unsigned int hashes[0x206]; // 0x000: sorted by order[]
    int order[0x206];           // 0x818
};

// Binary search for `hash`; returns its entry index, or -1.
extern "C" int func_00314E88(sMdfIndex_00314E88* self, unsigned int hash)
{
    int lo = 0;
    int hi = 0x206;
    while (lo < hi)
    {
        int mid = (lo + hi) / 2;
        int idx = self->order[mid];
        unsigned int h = self->hashes[idx];
        if (hash == h)
        {
            return idx;
        }
        if (hash < h)
        {
            hi = mid;
        }
        else
        {
            lo = mid + 1;
        }
    }
    return -1;
}
#endif

INCLUDE_ASM("animation/mdfarchive", func_00314EF0);

INCLUDE_ASM("animation/mdfarchive", func_00314F30);

INCLUDE_ASM("animation/mdfarchive", func_00314FE8);

