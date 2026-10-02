#include "common.h"

INCLUDE_ASM("replay/playbackman", cPlaybackMan_cPlaybackMan);

//100%
INCLUDE_ASM("replay/playbackman", func_0026C588);
#ifdef SKIP_ASM
struct cBigFile;
void cBigFile__cBigFile(cBigFile* self, int flags);
void operator_delete(int* ptr);
extern "C" void func_0026C898(void* self);
extern void* D_00481898[];

extern "C" void func_0026C588(void* self, int flags)
{
    cBigFile* bf = *(cBigFile**)self;
    if (bf != 0) {
        cBigFile__cBigFile(bf, 3);
    }
    operator_delete(*(int**)((char*)self + 0x6C));
    operator_delete(*(int**)((char*)self + 0x68));
    func_0026C898(self);
    *(void***)((char*)self + 0x14) = D_00481898;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", cPlaybackMan_update);
#ifdef SKIP_ASM
extern "C" int func_003DF980(int handle);
extern "C" int ASYNCFILE_release(int handle, void** data, int* size);
extern "C" void func_002523A8(void* p);
extern "C" void func_003E6574(void* dst, void* src, int size);
extern const char D_00481560[];
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

struct sPlaybackEntry_C5F8 {
    int field_0x0;
    void* buf;      // 0x4
    int size;       // 0x8
};

struct cPlaybackMan_C5F8 {
    char pad_0x0[0x58];
    sPlaybackEntry_C5F8* entries;   // 0x58
    char pad_0x5c[0x18];
    int cur;                        // 0x74
    int handle;                     // 0x78
};

extern "C" void cPlaybackMan_update(cPlaybackMan_C5F8* self)
{
    if (self->handle >= 0 && func_003DF980(self->handle) == 1) {
        void* data = 0;
        int size = 0;
        ASYNCFILE_release(self->handle, &data, &size);
        if (data != 0 && size > 0) {
            // PORT: pointer held in int for the entry address (index-first addu); not 64-bit safe.
            sPlaybackEntry_C5F8* e = (sPlaybackEntry_C5F8*)(self->cur * 12 + (int)self->entries);
            e->buf = operator_new_tag(size, D_00481560, 0x20000000, 0);
            self->entries[self->cur].size = size;
            func_003E6574(self->entries[self->cur].buf, data, size);
            func_002523A8(data);
        }
        self->handle = -1;
    }
}
#endif

INCLUDE_ASM("replay/playbackman", cPlaybackMan_initLocation);

INCLUDE_ASM("replay/playbackman", func_0026C898);

//100%
INCLUDE_ASM("replay/playbackman", func_0026CA90);
#ifdef SKIP_ASM
extern "C" void func_0026CDF8(void* self);
extern "C" void func_003E6448(void* dst, int c, int n);

extern "C" void func_0026CA90(void* self)
{
    func_0026CDF8(self);
    unsigned int* buf = *(unsigned int**)((char*)self + 0x4);
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x70) = 0;
    if (buf != 0) {
        func_003E6448(buf, 0, 0x1C22C);
        buf[0] = 0xDBAC0101;
        buf[1] = 0x1C200;
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", func_0026CC48);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void*);
extern "C" void func_00128AC0(void* self);
extern "C" void func_0015EDC8(void* self, int value);
void func_0026F4A0(void* self, int val);
extern "C" void func_0026CBB8(void* self);
void func_00317908(void* self);

extern "C" void func_0026CC48(void* self)
{
    if (*(int*)((char*)self + 0x70) == 0 && *(int*)(*(char**)((char*)self + 0x4) + 0xC) > 0) {
        cAI_purgeMissionRiders(*(void**)((char*)self + 0x60));
        func_00128AC0(*(void**)((char*)self + 0x60));
        void* fp = *(void**)(*(char**)((char*)self + 0x5C) + 0x28);
        *(int*)((char*)fp + 0x4) = *(int*)fp;
        func_0026F4A0(fp, 14);
        func_0026CBB8(self);
        *(int*)(*(char**)((char*)self + 0x4) + 0x8) = 0;
        func_00317908(*(char**)((char*)self + 0x4) + 0x1C214);
        *(int*)(*(char**)((char*)self + 0x60) + 0x8) = *(int*)(*(char**)((char*)self + 0x4) + 0x1C210);
        *(int*)((char*)self + 0x70) = 1;
        char* x = *(char**)(*(char**)((char*)self + 0x60) + 0x70);
        int v = x ? *(int*)(x + 0x18) : 0;
        func_0015EDC8(*(void**)(*(char**)(*(char**)((char*)self + 0x5C) + 0x84) + 0x4), v);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", func_0026CD20);
#ifdef SKIP_ASM
extern "C" void cAI_purgeMissionRiders(void*);
extern "C" void func_00128AC0(void* self);
extern "C" void func_0015EDC8(void* self, int value);
void func_0026F4A0(void* self, int val);

extern "C" void func_0026CD20(void* self)
{
    if (*(int*)((char*)self + 0x70) == 1) {
        cAI_purgeMissionRiders(*(void**)((char*)self + 0x60));
        func_00128AC0(*(void**)((char*)self + 0x60));
        void* fp = *(void**)(*(char**)((char*)self + 0x5C) + 0x28);
        func_0026F4A0(fp, *(int*)((char*)fp + 0x4));
        func_0026CC18(self);
        *(int*)((char*)self + 0x70) = 0;
        int v = *(int*)(*(char**)((char*)self + 0x60) + 0x28);
        func_0015EDC8(*(void**)(*(char**)(*(char**)((char*)self + 0x5C) + 0x84) + 0x4), v);
    }
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("replay/playbackman", func_0026D2B0);
#ifdef SKIP_ASM
struct sPbChunk_D2B0 {
    unsigned int n : 12;
    unsigned int key : 20;
    int data;
};

struct sPbStream_D2B0 {
    int count;              // 0x0
    int index;              // 0x4
    int offset;             // 0x8
    sPbChunk_D2B0* chunks;  // 0xC
};

extern "C" int func_00415FC8(const void* a, const void* b, int n);
extern "C" void func_0026D228(sPbStream_D2B0* self, sPbChunk_D2B0* chunk);

extern "C" int func_0026D2B0(sPbStream_D2B0* self, sPbChunk_D2B0* chunk)
{
    if (self->count > 0) {
        int i = self->count - 1;
        sPbChunk_D2B0 last = self->chunks[i];
        last.n = 0;
        if (func_00415FC8(&last, chunk, 8) == 0 && (int)self->chunks[i].n < 0xFFF) {
            self->chunks[i].n++;
        } else {
            func_0026D228(self, chunk);
        }
    } else {
        func_0026D228(self, chunk);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("replay/playbackman", func_0026D420);
#ifdef SKIP_ASM
struct sPbChunk_D420 {
    unsigned int n : 12;
    unsigned int key : 20;
    int data;
};

struct sPbStream_D420 {
    int count;              // 0x0
    int index;              // 0x4
    int offset;             // 0x8
    sPbChunk_D420* chunks;  // 0xC
};

extern "C" void func_003E6448(void* dst, int c, int n);

extern "C" int func_0026D420(sPbStream_D420* self, sPbChunk_D420* out)
{
    func_003E6448(out, 0, 8);
    if (self->index >= self->count) {
        return 0;
    }
    if (self->offset <= 0) {
        self->offset = self->chunks[self->index].n;
    }
    *out = self->chunks[self->index];
    if (--self->offset <= 0) {
        self->index++;
    }
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("replay/playbackman", func_0026D558);
#ifdef SKIP_ASM
extern "C" void func_003E6574(void* dst, void* src, int size);
extern "C" int func_002C85A0(void* src, int a, int dst);

extern "C" int func_0026D558(void* self, char** pp)
{
    struct {
        int tag;
        int value;
    } hdr;
    int size;
    func_003E6574(&hdr, *pp, 8);
    *pp += 8;
    *(int*)self = hdr.value;
    func_003E6574(&size, *pp, 4);
    *pp += 4;
    func_002C85A0(*pp, 0, *(int*)((char*)self + 0xC));
    *pp += size;
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("replay/playbackman", func_0026D678);
#ifdef SKIP_ASM
struct sPbRgba_D678 {
    unsigned char c[4];
};

struct sPbHeader_D678 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    sPbRgba_D678 colour;    // 0x10
    int field_0x14;
    int field_0x18;
    int field_0x1c;
    int field_0x20;
    int field_0x24;
};

struct sPbFrame_D678 {
    char pad_0x0[0x10];
    int field_0x10;         // 0x10
    char pad_0x14[0x8];
    sPbRgba_D678 colour;    // 0x1C
    int field_0x20;         // 0x20
    int field_0x24;         // 0x24
    int field_0x28;         // 0x28
    int field_0x2c;         // 0x2C
    int field_0x30;         // 0x30
    int field_0x34;         // 0x34
};

extern "C" void func_003E6574(void* dst, void* src, int size);
extern "C" int func_0026EAB8(void* self, char** pp, void* src);
// PORT: func_0026ED88 is an empty (void*) stub, but this caller passes a second
// argument; bind the 2-arg form to the symbol.
void func_0026ED88_2(void* self, int a) __asm__("func_0026ED88__FPv");

extern "C" int func_0026D678(sPbFrame_D678* self, char** pp)
{
    sPbHeader_D678 h;
    char* p = *pp;
    *pp = p + 0x28;
    func_003E6574(&h, p, 0x28);
    self->field_0x10 = h.field_0x4;
    self->field_0x30 = h.field_0x8;
    self->field_0x34 = h.field_0xc;
    self->colour = h.colour;
    self->field_0x20 = h.field_0x14;
    self->field_0x24 = h.field_0x18;
    self->field_0x28 = h.field_0x1c;
    self->field_0x2c = h.field_0x20;
    func_0026EAB8(self, pp, p + 0x24);
    func_0026ED88_2(self, 1);
    return 1;
}
#endif

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

//100%
INCLUDE_ASM("replay/playbackman", func_0026D760);
#ifdef SKIP_ASM
struct sPbBytes4D760 {
    char b[4];
};

extern "C" void* func_00270130(void* self);
extern "C" void func_0026D818(void* self, void* out);
// PORT: func_0026ED88 is an empty (void*) stub, but this caller passes a second
// argument; bind the 2-arg form to the symbol.
void func_0026ED88_2(void* self, int a) __asm__("func_0026ED88__FPv");

extern "C" void* func_0026D760(void* self, sPbBytes4D760* src)
{
    void* out = func_00270130(self);
    *(sPbBytes4D760*)((char*)out + 0x1C) = *src;
    func_0026D818(self, out);
    *(int*)((char*)out + 0x30) = *(int*)((char*)self + 0x3C8);
    func_0026ED88_2(out, 0);
    return out;
}
#endif

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

