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

//100%
INCLUDE_ASM("replay/playbackman", func_0026CBB8);
#ifdef SKIP_ASM
struct sPbBytes4;
extern "C" void func_0026D7D0(void* self, void* out, sPbBytes4* src);

struct sFrameDesc26CBB8 {
    short type;
    char a;
    char b;
};

extern "C" void func_0026CBB8(void* self)
{
    if (*(int*)((char*)self + 0x10) == 0) {
        int one = 1;
        sFrameDesc26CBB8 desc;
        desc.type = one;
        desc.a = 0;
        desc.b = 0;
        func_0026D7D0(*(void**)(*(char**)((char*)self + 0x5C) + 0x28), (char*)self + 0x14,
                      (sPbBytes4*)&desc);
        *(int*)((char*)self + 0x10) = one;
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", func_0026CC18);
#ifdef SKIP_ASM
// PORT: func_0026DBD0 is declared in this unit with one parameter, but it passes a
// second one ($5) through to cReplay_restoreFrame; bind the 2-arg form to the symbol.
extern "C" void func_0026DBD0_2(void* replay, void* frame) __asm__("func_0026DBD0");

extern "C" void* func_0026CC18(void* self)
{
    if (*(int*)((char*)self + 0x10)) {
        func_0026DBD0_2(*(void**)(*(char**)((char*)self + 0x5C) + 0x28), (char*)self + 0x14);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", func_0026CDF8);
#ifdef SKIP_ASM
extern "C" void func_0026CD20(void* self);
void func_0026CDD0(void* self);

extern "C" void func_0026CDF8(void* self)
{
    switch (*(int*)((char*)self + 0x70)) {
    case 0:
    default:
        break;
    case 1:
        func_0026CD20(self);
        break;
    case 2:
        func_0026CDD0(self);
        break;
    }
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026CE50);

//100%
INCLUDE_ASM("replay/playbackman", func_0026D0A0);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char D_004815A0[];
void func_0026D168(void* self);

extern "C" void* func_0026D0A0(void* self)
{
    *(void**)((char*)self + 0xC) = operator_new_tag(0x8000, D_004815A0, 0, 0);
    func_0026D168(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", func_0026D0E8);
#ifdef SKIP_ASM
extern "C" void func_0026D130(void* self);
void operator_delete(int* ptr);

extern "C" void func_0026D0E8(int* self, int flags)
{
    func_0026D130(self);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", func_0026D130);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_0026D130(void* self)
{
    void* p = *(void**)((char*)self + 0xC);
    if (p != 0) {
        cMemMan_free(p);
        *(void**)((char*)self + 0xC) = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("replay/playbackman", func_0026D5E8);
#ifdef SKIP_ASM
void* func_0026E8E0(void* self);
extern "C" void func_0026D628(void* self);
extern void* D_00481850[];

extern "C" void* func_0026D5E8(void* self)
{
    func_0026E8E0(self);
    *(void***)self = D_00481850;
    func_0026D628(self);
    return self;
}
#endif

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

//100%
INCLUDE_ASM("replay/playbackman", func_0026D7D0);
#ifdef SKIP_ASM
struct sPbBytes4 {
    char b[4];
};

extern "C" void func_0026D818(void* self, void* out);

extern "C" void func_0026D7D0(void* self, void* out, sPbBytes4* src)
{
    *(sPbBytes4*)((char*)out + 0x1C) = *src;
    func_0026D818(self, out);
    *(int*)((char*)out + 0x30) = *(int*)((char*)self + 0x3C8);
}
#endif

INCLUDE_ASM("replay/playbackman", func_0026D818);

INCLUDE_ASM("replay/playbackman", func_0026D988);

INCLUDE_ASM("replay/playbackman", func_0026DA88);

//100%
INCLUDE_ASM("replay/playbackman", func_0026DB88);
#ifdef SKIP_ASM
// PORT: cReplayFramePtr_getFrameBlock is mangled with no parameters (__Fv) but is
// called here with (frame, 1); bind the 2-arg form to the symbol.
void cReplayFramePtr_getFrameBlock_2(void* frame, int n) __asm__("cReplayFramePtr_getFrameBlock__Fv");
extern "C" void func_0026DBD0_2(void* replay, void* frame) __asm__("func_0026DBD0");

extern "C" void func_0026DB88(void* self, void* frame)
{
    cReplayFramePtr_getFrameBlock_2(frame, 1);
    func_0026DBD0_2(self, frame);
}
#endif

extern "C" void cReplay_restoreFrame(void*);

//100%
INCLUDE_ASM("replay/playbackman", func_0026DBD0);
#ifdef SKIP_ASM
extern "C" void func_0026DBD0(void* self)
{
    cReplay_restoreFrame(self);
}
#endif

