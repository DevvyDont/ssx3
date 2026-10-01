#include "common.h"

INCLUDE_ASM("replay/playbackman", cPlaybackMan_cPlaybackMan);

INCLUDE_ASM("replay/playbackman", func_0026C588);

INCLUDE_ASM("replay/playbackman", cPlaybackMan_update);

INCLUDE_ASM("replay/playbackman", cPlaybackMan_initLocation);

INCLUDE_ASM("replay/playbackman", func_0026C898);

INCLUDE_ASM("replay/playbackman", func_0026CA90);

//100%
INCLUDE_ASM("replay/playbackman", func_0026CBB0__FPv);
#ifdef SKIP_ASM
void func_0026CBB0(void* self)
{
    *(int*)((char*)self + 0x10) = 0;
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026CBB8);

INCLUDE_ASM("replay/playbackman", func_0026CC18);

INCLUDE_ASM("replay/playbackman", func_0026CC48);

INCLUDE_ASM("replay/playbackman", func_0026CD20);

extern "C" void* func_0026CC18(void*);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", func_0026CDD0__FPv);
#ifdef SKIP_ASM
void func_0026CDD0(void* self)
{
    func_0026CC18(self);
    *(int*)((char*)self + 0x70) = 0;
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026CDF8);

INCLUDE_ASM("replay/playbackman", func_0026CE50);

INCLUDE_ASM("replay/playbackman", func_0026D0A0);

INCLUDE_ASM("replay/playbackman", func_0026D0E8);

INCLUDE_ASM("replay/playbackman", func_0026D130);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D168__FPv);
#ifdef SKIP_ASM
void func_0026D168(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026D178);

INCLUDE_ASM("replay/playbackman", func_0026D228);

INCLUDE_ASM("replay/playbackman", func_0026D2B0);

INCLUDE_ASM("replay/playbackman", func_0026D420);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D4D8);
#ifdef SKIP_ASM
struct sPlaybackChunk {
    unsigned int info;
    int data;
};

struct sPlaybackStream {
    int count;
    int index;
    int offset;
    sPlaybackChunk* chunks;
};

extern "C" int func_0026D4D8(sPlaybackStream* self, unsigned int pos)
{
    int i = 0;
    sPlaybackChunk* c = self->chunks;
    unsigned int total = 0;
    for (; i < self->count; i++) {
        total += (c++)->info & 0xFFF;
        if (pos < total) {
            break;
        }
    }
    if (i >= self->count) {
        return 0;
    }
    self->index = i;
    self->offset = total - pos;
    return 1;
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026D558);

INCLUDE_ASM("replay/playbackman", func_0026D5E8);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D628);
#ifdef SKIP_ASM
struct sPlaybackKey {
    short a;
    char b;
    char c;
};

extern "C" void func_0026D628(void* self)
{
    sPlaybackKey k;
    k.a = 0;
    k.b = 0;
    k.c = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(int*)((char*)self + 0x34) = -1;
    *(sPlaybackKey*)((char*)self + 0x1c) = k;
    *(int*)((char*)self + 0x20) = -1;
    *(int*)((char*)self + 0x24) = -1;
    *(int*)((char*)self + 0x28) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x2c) = -1;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026D678);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D730__FPvii);
#ifdef SKIP_ASM
void func_0026D730(void* self, int i, int value)
{
    *(int*)((char*)self + (i << 2) + 0x20) = value;
}
#endif

extern "C" void* func_0026E5D0(void*);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D740__FPv);
#ifdef SKIP_ASM
void* func_0026D740(void* self)
{
    return func_0026E5D0((char*)self + 0x3b0);
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026D760);

INCLUDE_ASM("replay/playbackman", func_0026D7D0);

INCLUDE_ASM("replay/playbackman", func_0026D818);

INCLUDE_ASM("replay/playbackman", func_0026D988);

INCLUDE_ASM("replay/playbackman", func_0026DA88);

INCLUDE_ASM("replay/playbackman", func_0026DB88);

extern "C" void cReplay_restoreFrame(void*);

//100%
INCLUDE_ASM("replay/playbackman", func_0026DBD0);
#ifdef SKIP_ASM
extern "C" void func_0026DBD0(void* self)
{
    cReplay_restoreFrame(self);
}
#endif

