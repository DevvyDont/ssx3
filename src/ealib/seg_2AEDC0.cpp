#include "common.h"

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADDC0);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADDC0(void* self, int videoSource, int videoBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, 0, videoBufferSize, 0, 0, 0, sourceType, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE08);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADE08(void* self, int videoSource, int videoBufferSize, int audioSource, int audioBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, 0, videoBufferSize, audioSource, 0, audioBufferSize, sourceType, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE58);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADE58(void* self, int videoSource, int videoDataSize, int videoBufferSize, int audioSource, int audioDataSize, int audioBufferSize, int sourceType, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, videoDataSize, videoBufferSize, audioSource, audioDataSize, audioBufferSize, sourceType, noAudio);
    return self;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADE90);
#ifdef SKIP_ASM
extern "C" void func_003ADEC8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

extern "C" void* func_003ADE90(void* self, int videoSource, int videoDataSize, int videoBufferSize, int audioSource, int audioDataSize, int audioBufferSize, int noAudio)
{
    *(int*)((char*)self + 0x60) = 0;
    func_003ADEC8(self, videoSource, videoDataSize, videoBufferSize, audioSource, audioDataSize, audioBufferSize, 2, noAudio);
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003ADEC8);
#ifdef SKIP_ASM
struct sAlloc_ADEC8 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_ADEC8 D_00509430_ADEC8 __asm__("D_00509430");
extern char str_AV_VideoStreamBuffer[];
extern char str_AV_AudioStreamBuffer[];
extern "C" signed char func_003B5440();
extern "C" void func_003E62B0();
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" void func_003E0AD8(int h, int a, int b);
extern "C" int func_003E0D80(int h, int a, int b, int c);
extern "C" int ASYNCFILE_load(int a, int flags);
extern "C" int STREAM_create(int a, int b, int c, void* buf, int size);
extern "C" void func_003E0948(int h, int a, int b, int c, int d);
extern "C" int func_003E0BF8(int h, int a);
extern "C" int STREAM_queuefile(int h, int a, int b, int c);

static inline bool wsCanAlloc_ADEC8(sAlloc_ADEC8* a)
{
    return a->alloc != 0 && a->free != 0;
}

extern "C" void func_003ADEC8(void* selfp, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    char* self = (char*)selfp;
    if (func_003B5440() == 0) {
        a8 = 1;
    }
    MEM_fill(self, 0, 0x78);
    *(int*)(self + 0x18) = a7;
    *(int*)(self + 0x1C) = a8;
    *(int*)(self + 0x20) = 0;
    *(int*)(self + 0x28) = 0;
    *(int*)(self + 0x24) = 0;
    *(int*)(self + 0x2C) = 0;
    *(int*)(self + 0x44) = 0;
    *(int*)(self + 0x5C) = 0;
    *(int*)(self + 0x64) = 0;
    *(int*)(self + 0x0) = 0;
    *(int*)(self + 0x70) = 0;
    *(int*)(self + 0x74) = 0;
    *(int*)(self + 0xC) = a1;
    *(int*)(self + 0x10) = a4;
    if (!wsCanAlloc_ADEC8(&D_00509430_ADEC8)) {
        func_003E62B0();
        return;
    }
    if (a4 != 0) {
        *(int*)(self + 0x14) = 1;
        *(void**)(self + 0x4) = D_00509430_ADEC8.alloc(str_AV_VideoStreamBuffer, a3, 0, 0, D_00509430_ADEC8.flags);
        *(void**)(self + 0x8) = D_00509430_ADEC8.alloc(str_AV_AudioStreamBuffer, a6, 0, 0, D_00509430_ADEC8.flags);
        *(int*)(self + 0x30) = STREAM_create(2, 2, 1, *(void**)(self + 0x4), a3);
        *(int*)(self + 0x34) = STREAM_create(2, 2, 1, *(void**)(self + 0x8), a6);
        func_003E0AD8(*(int*)(self + 0x34), 0x98, 0x34);
        func_003E0AD8(*(int*)(self + 0x30), 0x98, 0x34);
        func_003E0948(*(int*)(self + 0x30), 1, 0xFF, 0x4D, 1);
        func_003E0948(*(int*)(self + 0x30), 2, 0, 0, -2);
        if (*(int*)(self + 0x1C) == 0) {
            func_003E0948(*(int*)(self + 0x34), 1, 0xFFFF, 0x4353, 1);
        } else {
            func_003E0948(*(int*)(self + 0x34), 1, 0xFFFF, 0x4353, -1);
        }
        func_003E0948(*(int*)(self + 0x34), 2, 0, 0, -2);
        int type = *(int*)(self + 0x18);
        if (type == 1) {
            *(int*)(self + 0x2C) = ASYNCFILE_load(a4, D_00509430_ADEC8.flags);
            *(int*)(self + 0x24) = ASYNCFILE_load(a1, D_00509430_ADEC8.flags);
        } else if (type == 0) {
            *(int*)(self + 0x3C) = STREAM_queuefile(*(int*)(self + 0x34), a4, 0, 0);
            *(int*)(self + 0x38) = STREAM_queuefile(*(int*)(self + 0x30), a1, 0, 0);
        } else if (type == 2) {
            *(int*)(self + 0x38) = func_003E0D80(*(int*)(self + 0x30), *(int*)(self + 0xC), a2, 0);
            *(int*)(self + 0x3C) = func_003E0D80(*(int*)(self + 0x34), *(int*)(self + 0x10), a5, 0);
        }
    } else {
        *(int*)(self + 0x14) = 0;
        *(void**)(self + 0x4) = D_00509430_ADEC8.alloc(str_AV_VideoStreamBuffer, a3, 0, 0, D_00509430_ADEC8.flags);
        *(void**)(self + 0x8) = 0;
        *(int*)(self + 0x30) = STREAM_create(2, 3, 2, *(void**)(self + 0x4), a3);
        *(int*)(self + 0x34) = func_003E0BF8(*(int*)(self + 0x30), 2);
        func_003E0AD8(*(int*)(self + 0x34), 0x98, 0x34);
        func_003E0AD8(*(int*)(self + 0x30), 0x98, 0x34);
        func_003E0948(*(int*)(self + 0x30), 1, 0xFF, 0x4D, 1);
        func_003E0948(*(int*)(self + 0x30), 3, 0, 0, -2);
        if (*(int*)(self + 0x1C) == 0) {
            func_003E0948(*(int*)(self + 0x30), 2, 0xFFFF, 0x4353, 2);
        } else {
            func_003E0948(*(int*)(self + 0x30), 2, 0xFFFF, 0x4353, -1);
        }
        int type = *(int*)(self + 0x18);
        if (type == 1) {
            *(int*)(self + 0x24) = ASYNCFILE_load(a1, D_00509430_ADEC8.flags);
        } else if (type == 0) {
            *(int*)(self + 0x38) = STREAM_queuefile(*(int*)(self + 0x30), a1, 0, 0);
        } else if (type == 2) {
            *(int*)(self + 0x38) = func_003E0D80(*(int*)(self + 0x30), *(int*)(self + 0xC), a2, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE2F8);
#ifdef SKIP_ASM
extern "C" void func_003AE2F8(void *player, int table) {
    (*(int *)((char*)(player) + (0x70))) = table;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE300);
#ifdef SKIP_ASM
struct sWSVtE300 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// Memory interface record filled by func_003DE928 (size field first).
struct sWSMemIfE300 {
    int size;                   // 0x0
    int f4;                     // 0x4
    void* (*release)(void*);    // 0x8
    int pad[5];
};

extern void* (*D_00509434[])(void*);
extern "C" void func_003AEE30(void* self, int flags);
extern "C" int func_003DE928(sWSMemIfE300* mi);
extern "C" void STREAM_destroy(void* p);

extern "C" void func_003AE300(char* self, int flags)
{
    char* o = *(char**)(self + 0x64);
    if (o != 0) {
        sWSVtE300* vt = *(sWSVtE300**)(o + 0x18);
        vt[1].fn(o + vt[1].delta, 3);
        *(void**)(self + 0x64) = 0;
    }
    if (*(void**)(self + 0x0) != 0) {
        func_003AEE30(*(void**)(self + 0x0), 3);
        *(void**)(self + 0x0) = 0;
    }
    if (*(void**)(self + 0x5C) != 0) {
        D_00509434[0](*(void**)(self + 0x5C));
        *(void**)(self + 0x5C) = 0;
    }
    STREAM_destroy(*(void**)(self + 0x30));
    *(void**)(self + 0x30) = 0;
    if (*(int*)(self + 0x14) != 0) {
        STREAM_destroy(*(void**)(self + 0x34));
        *(void**)(self + 0x34) = 0;
    }
    {
        void* p = *(void**)(self + 0x20);
        if (p != 0) {
            sWSMemIfE300 mi;
            mi.size = 0x20;
            func_003DE928(&mi);
            mi.release(p);
            *(void**)(self + 0x20) = 0;
        }
    }
    {
        void* p = *(void**)(self + 0x28);
        if (p != 0) {
            sWSMemIfE300 mi;
            mi.size = 0x20;
            func_003DE928(&mi);
            mi.release(p);
            *(void**)(self + 0x28) = 0;
        }
    }
    if (*(void**)(self + 0x4) != 0) {
        D_00509434[0](*(void**)(self + 0x4));
        *(void**)(self + 0x4) = 0;
    }
    if (*(void**)(self + 0x8) != 0) {
        D_00509434[0](*(void**)(self + 0x8));
        *(void**)(self + 0x8) = 0;
    }
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE450);
#ifdef SKIP_ASM
struct sAlloc_AE450 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_AE450 D_00509430_AE450 __asm__("D_00509430");
extern char D_00495210[];

struct sTimer_AE450 {
    unsigned int start;     // 0x0
    int pad4;
    long ticks;             // 0x8
    int period;             // 0x10
    int pad14;
};

struct func_003B0430_sObj;
struct func_003B0430_sKey;
extern "C" func_003B0430_sObj* func_003B0430(func_003B0430_sObj* self, func_003B0430_sKey* key);
// PORT: the unit declares func_003B0758 with int params; it stores pointers.
extern "C" void* func_003B0758_AE450(void* self, void* a1, void* a2, void* a3, int a4) __asm__("func_003B0758");
void* func_003AECB8(void* self, void* a1);
void* func_003AED20(void* self, void* a1, int a2);
extern "C" void* func_003AED40(void* self, int a1, int a2);
extern "C" int func_003AEEF8(void* self);
extern "C" void func_003E0AD8(int h, int a, int b);
extern "C" int func_003E0D80(int h, int a, int b, int c);
extern "C" void ASYNCFILE_release(int h, void* data, int* size);
extern "C" int func_003B0600(void* p, int a1);
extern "C" unsigned int func_003E4E98();
extern "C" unsigned int func_003E5008();
extern "C" void func_0042E508(void* timer);
extern "C" void* func_003AE888(void*, int);

static inline void* wsAlloc_AE450(sAlloc_AE450* a, int size)
{
    return a->alloc(D_00495210, size, 0, 0, a->flags);
}

static inline bool wsCanAlloc_AE450(sAlloc_AE450* a)
{
    return a->alloc != 0 && a->free != 0;
}

// PORT: 64-bit long tick counter
extern "C" int func_003AE450(char* self, int a1, int a2)
{
    func_003E0AD8(*(int*)(self + 0x34), 0x97, 0x33);
    func_003E0AD8(*(int*)(self + 0x30), 0x97, 0x33);
    if (*(int*)(self + 0x24) != 0) {
        int size;
        ASYNCFILE_release(*(int*)(self + 0x24), self + 0x20, &size);
        *(int*)(self + 0x38) = func_003E0D80(*(int*)(self + 0x30), *(int*)(self + 0x20), size, 0);
    }
    if (*(int*)(self + 0x2C) != 0) {
        int size;
        ASYNCFILE_release(*(int*)(self + 0x2C), self + 0x28, &size);
        *(int*)(self + 0x3C) = func_003E0D80(*(int*)(self + 0x34), *(int*)(self + 0x28), size, 0);
    }
    if (*(int*)(self + 0x14) == 0) {
        *(int*)(self + 0x3C) = *(int*)(self + 0x38);
    }
    if (*(int*)(self + 0x1C) == 0) {
        char* s = (char*)func_003AED40(wsAlloc_AE450(&D_00509430_AE450, 0x14), *(int*)(self + 0x34), *(int*)(self + 0x3C));
        *(char**)(self + 0x0) = s;
        *(unsigned int*)(self + 0x54) = *(int*)(s + 0xC) >= 0;
    } else {
        *(unsigned int*)(self + 0x54) = 0;
    }
    *(void**)(self + 0x64) = 0;
    if (wsCanAlloc_AE450(&D_00509430_AE450)) {
        char key[0x10];
        *(char**)(self + 0x60) = self;
        func_003B0758_AE450(key, self + 0x60, (void*)func_003AECB8, (void*)func_003AED20, a1);
        *(func_003B0430_sObj**)(self + 0x64) = func_003B0430((func_003B0430_sObj*)wsAlloc_AE450(&D_00509430_AE450, 0x1C), (func_003B0430_sKey*)key);
    }
    *(int*)(self + 0x74) = func_003B0600(*(void**)(self + 0x64), 0);
    sTimer_AE450* t = (sTimer_AE450*)wsAlloc_AE450(&D_00509430_AE450, 0x18);
    t->ticks = 0;
    t->start = func_003E4E98();
    t->period = 0x1000;
    *(sTimer_AE450**)(self + 0x5C) = t;
    func_0042E508(t);
    unsigned int ms = (unsigned int)(t->ticks * 1000) / func_003E5008();
    *(int*)(self + 0x40) = a2;
    *(int*)(self + 0x50) = 0;
    *(int*)(self + 0x58) = 0;
    *(unsigned int*)(self + 0x4C) = ms;
    if (*(void**)(self + 0x0) != 0) {
        func_003AEEF8(*(void**)(self + 0x0));
    }
    func_003AE888(self, 0x1000);
    return *(int*)(self + 0x74);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE6C8);
#ifdef SKIP_ASM
extern "C" int func_004139F8(float f);
extern "C" int func_003B0600(void* p, int a1);
extern "C" int func_003B0580(void* self);

extern "C" int func_003AE6C8(void* self, float position)
{
    *(float*)((char*)self + 0x48) = position;
    *(int*)((char*)self + 0x74) = func_003B0600(*(void**)((char*)self + 0x64), func_004139F8(position));
    *(int*)((char*)self + 0x44) = func_003B0580(*(void**)((char*)self + 0x64));
    return *(int*)((char*)self + 0x74);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE718);
#ifdef SKIP_ASM
extern "C" int func_003AF258(int, int);

extern "C" int func_003AE718(void *self) {
    if (*(int *)((char *)self + 0x70) != 0) {
        if (*(int *)((char *)self + 0x74) != 0) {
            return func_003AF258(*(int *)((char *)self + 0x70), *(int *)((char *)self + 0x44));
        }
        return func_003AF258(*(int *)((char *)self + 0x70), func_004139F8(*(float *)((char *)self + 0x48)));
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE780);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned int func_003AE9A0_E780(void* self) __asm__("func_003AE9A0");
extern "C" float func_003B05C0(void*);

extern "C" int func_003AE780(char* self)
{
    unsigned int elapsed = func_003AE9A0_E780(self);
    float rate = func_003B05C0(*(void**)(self + 0x64));
    *(float*)(self + 0x48) = (float)(elapsed + *(unsigned int*)(self + 0x40)) * 0.001f * rate;
    if (*(float*)(self + 0x48) < (float)*(unsigned int*)(self + 0x44) + 1.0f)
        return 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE860);
#ifdef SKIP_ASM
extern "C" int func_003AEFA8(void* p);

extern "C" int func_003AE860(void** self)
{
    void* audio = *self;
    int result = 1;
    if (audio != 0) {
        result = func_003AEFA8(audio);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE888);
#ifdef SKIP_ASM
struct func_003AE888_sTimer {
    unsigned int last;
    int pad4;
    ulong accum;
    unsigned int rate;
};

extern "C" int func_003AEE98(void* self, int v);
extern "C" unsigned int func_003E4E98();

// PORT: accum is a 64-bit ulong
extern "C" void* func_003AE888(void* self, int speed)
{
    if ((unsigned int)speed > 0x4000) {
        speed = 0x4000;
    }
    int newRate = speed;
    if (*(void**)self != 0) {
        func_003AEE98(*(void**)self, newRate);
    }
    func_003AE888_sTimer* timer = *(func_003AE888_sTimer**)((char*)self + 0x5C);
    unsigned int now = func_003E4E98();
    if (timer->rate == 0x1000) {
        timer->accum += (ulong)(now - timer->last);
    } else {
        timer->accum += ((ulong)timer->rate * (ulong)(now - timer->last)) >> 12;
    }
    timer->rate = newRate;
    timer->last = now;
    return 0;
}
#endif

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE938__FPv);
#ifdef SKIP_ASM
void* func_003AE938(void* self)
{
    return func_003AE888(self, 0);
}
#endif

extern "C" void* func_003AE888(void*, int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE958__FPv);
#ifdef SKIP_ASM
void* func_003AE958(void* self)
{
    return func_003AE888(self, 0x1000);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE978);
#ifdef SKIP_ASM
extern "C" int func_003AEF48(int);

extern "C" int func_003AE978(int *player) {
    int temp_4;
    int var_2;

    temp_4 = *player;
    var_2 = 0;
    if (temp_4 != 0) {
        var_2 = func_003AEF48(temp_4);
    }
    return var_2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AE9A0);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B5F60();
extern "C" int func_003B7B48(int id, void* out);
extern "C" int func_003B7C40(int id, void* out);
extern "C" unsigned int func_003E5008();
extern "C" void func_0042E508(void* timer);

// PORT: accum is a 64-bit ulong
struct func_003AE9A0_sTimer {
    unsigned int last;
    int pad4;
    ulong accum;
    unsigned int rate;
};

struct func_003AE9A0_sInfo {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
};

struct func_003AE9A0_sOwner {
    char pad[0xC];
    int id;
};

struct func_003AE9A0_sSync {
    func_003AE9A0_sOwner* owner;
    char pad04[0x48];
    int offset;
    int last;
    int enabled;
    int drift;
    func_003AE9A0_sTimer* timer;
};

extern "C" int func_003AE9A0(func_003AE9A0_sSync* self)
{
    func_003AE9A0_sInfo b;
    func_003AE9A0_sInfo a;
    func_003AE9A0_sTimer* t = self->timer;
    func_0042E508(t);
    unsigned int ms = (unsigned int)(t->accum * 1000) / func_003E5008();
    int res = ms - self->offset;
    if (self->enabled != 0) {
        func_003B58A0();
        func_003B7B48(self->owner->id, &a);
        func_003B7C40(a.field_0x4, &b);
        func_003B58D8();
        int d = b.field_0x4 - func_003B5F60();
        if (self->last < d) {
            self->last = d;
            self->drift = self->drift - self->drift / 8 + (d - res);
            int av = -self->drift;
            if (av < self->drift) av = self->drift;
            if (av > 0x108) {
                self->offset -= self->drift / 8;
                self->drift = 0;
                res = ms - self->offset;
            }
        }
    }
    return res;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEAD0);
#ifdef SKIP_ASM
struct sAlloc_AEAD0 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_AEAD0 D_00509430_AEAD0 __asm__("D_00509430");
extern char D_00495210[];

extern "C" unsigned char* func_003E12E0(void* q);
extern "C" void SYNCTASK_run(int a);
extern "C" int func_003B11F0(int v);
void* func_003B0720(void* self);
extern "C" void* func_003B11A0();
// PORT: func_003B04F8 returns its vcall's int result; the unit defines it void.
extern "C" int func_003B04F8_i(void* self, void* obj, void* item) __asm__("func_003B04F8");
extern "C" void func_003B0538(void* self);
extern "C" void* func_003E13E8(void* p, int a1);
extern "C" int func_003E1530(void* q);

struct sChunk_AEAD0 {
    unsigned char* data;    // 0x0
    unsigned char* body;    // 0x4
    unsigned int size;      // 0x8
};

static inline unsigned int le32_AEAD0(unsigned char* p)
{
    return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
}

// PORT: chunk pointers passed through int parameters of some callees. The unit declares
// func_003AEAD0(int, void*) for its caller; the real body takes (self, ctx, out), bound by asm label.
extern "C" void func_003AEAD0_impl(char* self, char* ctx, sChunk_AEAD0** out) __asm__("func_003AEAD0");
extern "C" void func_003AEAD0_impl(char* self, char* ctx, sChunk_AEAD0** out)
{
    int kind = 0;
    void* q = *(void**)(self + 0x30);
    *out = 0;
    for (;;) {
        unsigned char* p = func_003E12E0(q);
        SYNCTASK_run(0);
        if (p != 0) {
            unsigned int tag = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
            if (func_003B11F0(tag))
                kind = 3;
            if (kind != 0) {
                sChunk_AEAD0* c = (sChunk_AEAD0*)func_003B0720(D_00509430_AEAD0.alloc(D_00495210, 0xC, 0, 0, D_00509430_AEAD0.flags));
                *out = c;
                c->data = p;
                if (kind == 2) {
                    unsigned int size = le32_AEAD0(p + 4);
                    c->body = p;
                    c->size = size;
                } else {
                    unsigned int size = le32_AEAD0(p + 4);
                    c->body = p + 8;
                    c->size = size;
                }
                if (*(int*)(ctx + 0x14) != 0)
                    return;
                if (kind != 3)
                    return;
                if (func_003B04F8_i(ctx, func_003B11A0(), c) != 2)
                    func_003B0538(ctx);
                return;
            }
            char name[8];
            *(unsigned int*)name = tag;
            name[4] = 0;
            func_003E13E8(q, (int)p);
        }
        if (func_003E1530(q))
            return;
    }
}
#endif

extern "C" void* func_003AEAD0(int, void*);

//99.44%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AECB8__FPvT0);
#ifdef SKIP_ASM
void* func_003AECB8(void* self, void* context)
{
    return func_003AEAD0(*(int*)context, self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AECE0);
#ifdef SKIP_ASM
extern "C" void* func_003E13E8(void* p, int a1);
extern void* (*D_00509434[])(void*);

extern "C" void* func_003AECE0(int self, int a1)
{
    int* obj = (int*)a1;
    void* r = func_003E13E8(*(void**)(self + 0x30), *obj);
    if (obj != 0) {
        r = D_00509434[0](obj);
    }
    return r;
}
#endif

extern "C" void* func_003AECE0(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AED20__FPvT0i);
#ifdef SKIP_ASM
void* func_003AED20(void* self, void* context, int chunk)
{
    return func_003AECE0(*(int*)context, chunk);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AED40);
#ifdef SKIP_ASM
struct func_003AED40_sAlloc {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* free;
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern func_003AED40_sAlloc D_00509430_v __asm__("D_00509430");
extern char str_AV_audiobuff[];

extern "C" signed char func_003B5440();
extern "C" void func_003E62B0();
extern "C" int func_003BA020(void* desc);
extern "C" int func_003B7AC8(int a, int b);
extern "C" int func_003B7E38(int a, void* desc, int b, int c, void* mem, int size);
extern "C" int func_003B7DE8(int h, int a, int b);

extern "C" void* func_003AED40(void* self, int a1, int a2)
{
    char desc[0x18];
    if (func_003B5440() == 0) {
        func_003E62B0();
    } else {
        *(void**)((char*)self + 0x4) = 0;
        *(int*)((char*)self + 0x10) = -1;
        *(int*)((char*)self + 0xC) = -1;
        func_003BA020(desc);
        int size = func_003B7AC8(1, 0x1E);
        void* mem = D_00509430_v.alloc(str_AV_audiobuff, size, 0, 0, D_00509430_v.flags);
        *(void**)((char*)self + 0x4) = mem;
        int h = func_003B7E38(a1, desc, 1, 0x1E, mem, size);
        *(int*)((char*)self + 0xC) = h;
        *(int*)((char*)self + 0x10) = func_003B7DE8(h, -1, a2);
        *(float*)self = 1.0f;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEE30);
#ifdef SKIP_ASM
extern "C" void func_003B7838(void* a);
extern void* (*D_00509434[])(void*);

extern "C" void func_003AEE30(void* self, int flags)
{
    if (*(void**)((char*)self + 0x4) != 0) {
        func_003B7838(*(void**)((char*)self + 0xC));
        D_00509434[0](*(void**)((char*)self + 0x4));
        *(void**)((char*)self + 0x4) = 0;
    }
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEE98);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7FB8(int h, int v);

extern "C" int func_003AEE98(void* self, int speed)
{
    int result;
    if (*(int*)((char*)self + 0xC) != -1) {
        func_003B58A0();
        result = func_003B7FB8(*(int*)((char*)self + 0xC), speed);
        func_003B58D8();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEEF8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7D80(int h, int a1);

extern "C" int func_003AEEF8(void* self)
{
    int result;
    if (*(int*)((char*)self + 0x10) != -1) {
        func_003B58A0();
        result = func_003B7D80(*(int*)((char*)self + 0x10), 0);
        func_003B58D8();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEF48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_003B7E70(int, int);
int func_003AEF48_impl(void *self, int arg) __asm__("func_003AEF48");

int func_003AEF48_impl(void *self, int volume) {
    int result;

    if (*(int *)((char *)self + 0xC) != -1) {
        func_003B58A0();
        result = func_003B7E70(*(int *)((char *)self + 0xC), volume);
        func_003B58D8();
        return result;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AEFA8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7B48(int id, void* out);
extern "C" int func_003B7C40(int id, void* out);

struct func_003AEFA8_sInfo {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xC;
};

extern "C" int func_003AEFA8(void* self)
{
    func_003AEFA8_sInfo b;
    func_003AEFA8_sInfo a;
    func_003B58A0();
    if (func_003B7B48(*(int*)((char*)self + 0xC), &a) >= 0 && func_003B7C40(a.field_0x4, &b) >= 0) {
        func_003B58D8();
        return b.field_0x8 == 0;
    }
    func_003B58D8();
    return 0;
}
#endif

extern void* D_00456890[];

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF010);
#ifdef SKIP_ASM
extern "C" void *func_003AF010(void *table) {
    (*(int *)((char*)(table) + (0))) = 0;
    (*(int *)((char*)(table) + (4))) = 0;
    return table;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF020);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern char D_00495260[];
extern void (*D_00509434_AF020[])(void*) __asm__("D_00509434");
struct sElem_AF020 {
    char pad[0x34];
    // PORT: the constructor is func_003B0340, defined later in this unit as a C function.
    sElem_AF020() __asm__("func_003B0340");
    ~sElem_AF020() {}
    void* operator new[](unsigned int size) { return D_00509430_ADEC8.alloc(D_00495260, size, 0, 0, D_00509430_ADEC8.flags); }
    void operator delete[](void* p) { D_00509430_ADEC8.free(p); }
};
struct sArr_AF020 {
    sElem_AF020* arr;
    int n;
};

extern "C" void func_003AF020(sArr_AF020* self, int flags)
{
    if (self->arr) {
        delete[] self->arr;
        self->arr = 0;
    }
    self->n = 0;
    if (flags & 1) D_00509434_AF020[0](self);
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF0C8);
#ifdef SKIP_ASM
extern "C" void* func_003B0340(void*);
extern "C" void f03A8_F0C8(void*, int) __asm__("func_003B03A8");
extern "C" void f03B0_F0C8(void*, int) __asm__("func_003B03B0");
extern "C" void f03B8_F0C8(void*, int) __asm__("func_003B03B8");
extern "C" void f03C0_F0C8(void*, int) __asm__("func_003B03C0");
extern "C" void f03C8_F0C8(void*, unsigned int) __asm__("func_003B03C8");
extern "C" void f03D0_F0C8(void*, int) __asm__("func_003B03D0");
extern char D_00495260[];
struct sAlloc_F0C8 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void (*free)(void* p);
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_F0C8 D_00509430_F0C8 __asm__("D_00509430");

struct sElem_F0C8 { char pad[0x34]; };
struct sSelf_F0C8 { sElem_F0C8* arr; unsigned int count; };

extern "C" void func_003AF0C8(void* vself, unsigned int count, int a2, int a3, int a4, int a5)
{
    sSelf_F0C8* self = (sSelf_F0C8*)vself;
    unsigned int* raw;
    char* arr;
    char* p;
    int i;
    unsigned int k;

    i = count - 1;
    raw = (unsigned int*)D_00509430_F0C8.alloc(D_00495260, count * 0x34 + 0x10, 0, 0, D_00509430_F0C8.flags);
    arr = (char*)raw + 0x10;
    *raw = count;
    p = arr;
    if (count != 0) {
        do {
            i--;
            func_003B0340(p);
            p += 0x34;
        } while (i != -1);
    }
    self->arr = (sElem_F0C8*)arr;
    self->count = count;
    if (count != 0) {
        f03D0_F0C8(arr, 0);
    }
    for (k = 0; k < self->count; k++) {
        f03A8_F0C8(&self->arr[k], a2);
        f03B0_F0C8(&self->arr[k], a3);
        f03B8_F0C8(&self->arr[k], a4);
        f03C0_F0C8(&self->arr[k], a5);
        f03C8_F0C8(&self->arr[k], k);
    }
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF230);
#ifdef SKIP_ASM
struct sWScriptEntry_3AF230 {
    char pad[0x34];
};
struct sWScriptArray_3AF230 {
    sWScriptEntry_3AF230* items;
    unsigned int count;
};

extern "C" sWScriptEntry_3AF230* func_003AF230(sWScriptArray_3AF230* self, unsigned int i)
{
    if (self->count < i) {
        return 0;
    }
    return &self->items[i];
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF258);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void* func_003AF258_impl(void* self, unsigned int key) __asm__("func_003AF258");
extern "C" void* func_003AF258_impl(void* self, unsigned int key)
{
    struct S { char* arr; int n; };
    S* s = (S*)self;
    if (s->n == 0) {
        return 0;
    }
    int hi = s->n;
    char* arr = s->arr;
    int lo = -1;
    while (hi - lo >= 2) {
        int mid = (hi + lo) / 2;
        if (key <= *(unsigned int*)(arr + mid * 0x34 + 0x14)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    int i = hi - 1;
    if (i < 0) i = 0;
    i = (i < s->n - 1) ? i : s->n - 1;
    return arr + i * 0x34;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF2F8);
#ifdef SKIP_ASM
extern "C" int func_003AF2F8(void *table) {
    return (*(int *)((char*)(table) + (4)));
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF300);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern unsigned char D_00499C81[];
extern "C" void func_003AF300(char** pp, unsigned int* plen)
{
    unsigned int i;
    char* p = *pp;
    for (i = 0; i < *plen && ((D_00499C81 + p[i])[0] & 8) != 0; i++) {
    }
    *pp = p + i;
    *plen -= i;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF378);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern unsigned char D_00499C81[];

static inline int to_lower_3AF378(int c) { if (D_00499C81[c] & 1) return c + 0x20; return c; }
extern "C" int func_003AF378(const char* lhs, const char* rhs)
{
    int i;
    int diff;
    diff = 0;
    i = 0;
    while (lhs[i] != 0 && rhs[i] != 0) {
        diff += to_lower_3AF378(lhs[i]) - to_lower_3AF378(rhs[i]);
        if (diff != 0) {
            return diff;
        }
        i++;
    }
    return lhs[i] - rhs[i];
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF428);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sNames_F428 { const char* e[5]; };
struct sVals_F428 { unsigned int e[5]; };
extern const sNames_F428 D_00495290;
extern const sVals_F428 D_004952A8;
extern "C" int func_003AF378(const char*, const char*) __asm__("func_003AF378");

extern "C" unsigned int func_003AF428(char* s, unsigned int len)
{
    unsigned int i;
    char ch = 9;
    unsigned int r;
    unsigned int j;

    for (i = 0; i < len; i++) {
        char c = s[i];
        if (c == 9 || c == 13) {
            s[i] = 0;
            ch = c;
            break;
        }
    }
    sNames_F428 names = D_00495290;
    sVals_F428 vals = D_004952A8;
    r = 0xFFFFFFFF;
    for (j = 0; j < 5; j++) {
        if (func_003AF378(names.e[j], s) == 0) {
            r = vals.e[j];
            break;
        }
    }
    if (i != len) {
        s[i] = ch;
    }
    return r;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF560);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long func_0041E9E0(unsigned char *s, unsigned char **end, int base);

extern "C" int func_003AF560(unsigned char **pp, unsigned int *plen, int *out) {
    unsigned int i;
    int ch = 9;
    unsigned char *end;
    long value;

    for (i = 0; i < *plen; i++) {
        unsigned char curChar = (*pp)[i];
        if ((unsigned int)(curChar - 9) < 2U) {
            ch = (signed char)curChar;
            (*pp)[i] = 0;
            break;
        }
    }
    value = func_0041E9E0(*pp, &end, 10);
    *plen = *plen - (end - *pp);
    if (i != *plen) {
        (*pp)[i] = (unsigned char)ch;
    }
    if (end == *pp) {
        return 0;
    }
    *out = (int)value;
    return 1;
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF670);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void func_003AF300(char** pp, unsigned int* plen);
extern "C" void func_003AF670(char** ps, unsigned int* plen, char** words, unsigned int* lens, unsigned int nwords, unsigned int* outIdx)
{
    char* text = *ps;
    if (text == 0) {
        *ps = 0;
        *plen = 0;
        *outIdx = 0xFFFFFFFF;
        return;
    }
    func_003AF300(&text, plen);
    for (unsigned int i = 0; i < *plen; i++) {
        for (unsigned int k = 0; k < nwords; k++) {
            unsigned int j;
            for (j = 0; j < lens[k] && i + j < *plen && words[k][j] == text[i + j]; j++) {
            }
            if (j == lens[k]) {
                unsigned int consumed = i + j;
                *ps = text + consumed;
                *plen -= consumed;
                *outIdx = k;
                return;
            }
        }
    }
    *ps = 0;
    *plen = 0;
    *outIdx = 0xFFFFFFFF;
}
#endif
INCLUDE_ASM("ealib/seg_2AEDC0", func_003AF7F0);
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0340);
#ifdef SKIP_ASM
extern "C" void *func_003B0340(void *self) {
    *(int *)((char *)self + 0x00) = 0x20;
    *(int *)((char *)self + 0x04) = 0x15E;
    *(int *)((char *)self + 0x08) = 0x258;
    *(int *)((char *)self + 0x0C) = 0x32;
    *(unsigned *)((char *)self + 0x10) = 0xFFFFFFFFu;
    *(unsigned char *)((char *)self + 0x24) = 0xFF;
    *(unsigned char *)((char *)self + 0x25) = 0xFF;
    *(unsigned char *)((char *)self + 0x26) = 0xFF;
    *(int *)((char *)self + 0x18) = 2;
    *(unsigned *)((char *)self + 0x14) = 0xFFFFFFFFu;
    *(unsigned char *)((char *)self + 0x27) = 0xFF;
    *(int *)((char *)self + 0x1C) = 2;
    *(int *)((char *)self + 0x20) = 0;
    *(int *)((char *)self + 0x28) = 0;
    *(int *)((char *)self + 0x2C) = 0;
    *(int *)((char *)self + 0x30) = 0;
    return self;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03A8);
#ifdef SKIP_ASM
extern "C" void func_003B03A8(int *entry, int value) {
    *entry = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03B0);
#ifdef SKIP_ASM
extern "C" void func_003B03B0(void *entry, int value) {
    (*(int *)((char*)(entry) + (4))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03B8);
#ifdef SKIP_ASM
extern "C" void func_003B03B8(void *entry, int value) {
    (*(int *)((char*)(entry) + (8))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03C0);
#ifdef SKIP_ASM
extern "C" void func_003B03C0(void *entry, int value) {
    (*(int *)((char*)(entry) + (0xC))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03C8);
#ifdef SKIP_ASM
extern "C" void func_003B03C8(void *entry, int index) {
    (*(int *)((char*)(entry) + (0x10))) = index;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03D0);
#ifdef SKIP_ASM
extern "C" void func_003B03D0(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x14))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03D8);
#ifdef SKIP_ASM
extern "C" void func_003B03D8(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x18))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03E0);
#ifdef SKIP_ASM
extern "C" void func_003B03E0(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x1C))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03E8);
#ifdef SKIP_ASM
extern "C" void func_003B03E8(void *entry, int value) {
    (*(int *)((char*)(entry) + (0x20))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03F0);
#ifdef SKIP_ASM
extern "C" void func_003B03F0(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x24))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B03F8);
#ifdef SKIP_ASM
extern "C" void func_003B03F8(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x25))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0400);
#ifdef SKIP_ASM
extern "C" void func_003B0400(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x26))) = value;
}
#endif
//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0408);
#ifdef SKIP_ASM
extern "C" void func_003B0408(void *entry, signed char value) {
    (*(signed char *)((char*)(entry) + (0x27))) = value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0410__FPv);
#ifdef SKIP_ASM
void* func_003B0410(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0xc) = (int)(void*)D_00456890;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0430);
#ifdef SKIP_ASM
void* func_003B0738(void* self);
extern void* D_004568A8[];

struct func_003B0430_sKey {
    int v[4];
};

struct func_003B0430_sObj {
    int field_0x0;
    func_003B0430_sKey key;   // 0x4
    int field_0x14;
    void** vtable;            // 0x18
};

extern "C" func_003B0430_sObj* func_003B0430(func_003B0430_sObj* self, func_003B0430_sKey* key)
{
    self->vtable = D_004568A8;
    func_003B0738(&self->key);
    self->key = *key;
    self->field_0x14 = 0;
    self->field_0x0 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B04A0);
#ifdef SKIP_ASM
extern "C" void func_003B0538(void* self);
extern void* (*D_00509434[])(void*);
extern void* D_004568A8[];

extern "C" void func_003B04A0(void* self, int flags)
{
    *(void***)((char*)self + 0x18) = D_004568A8;
    func_003B0538(self);
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B04F8);
#ifdef SKIP_ASM
class func_003B04F8_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual int v02(void* a, int b);
    virtual void v03();
    virtual int v04();
    virtual float v05();
};

extern "C" void func_003B04F8(void* self, func_003B04F8_cObj* obj, int a2)
{
    *(func_003B04F8_cObj**)((char*)self + 0x14) = obj;
    *(int*)self = a2;
    obj->v02(self, a2);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0538);
#ifdef SKIP_ASM
struct sVEntry003B0538 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_003B0538(void* self)
{
    void* obj = *(void**)((char*)self + 0x14);
    if (obj != 0) {
        sVEntry003B0538* vt = *(sVEntry003B0538**)obj;
        vt[1].fn((char*)obj + vt[1].delta, 3);
        *(void**)((char*)self + 0x14) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0580);
#ifdef SKIP_ASM
extern "C" int func_003B0580(void* self)
{
    func_003B04F8_cObj* obj = *(func_003B04F8_cObj**)((char*)self + 0x14);
    if (obj == 0) {
        return 0;
    }
    return obj->v04();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B05C0);
#ifdef SKIP_ASM
extern "C" float func_003B05C0(void* self)
{
    func_003B04F8_cObj* obj = *(func_003B04F8_cObj**)((char*)self + 0x14);
    if (obj == 0) {
        return 0.0f;
    }
    return obj->v05();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0600);
#ifdef SKIP_ASM
class func_003B0600_cVirt {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(int);
};

struct func_003B0600_sObj {
    int loaded;                                    // 0x0
    void* arg;                                     // 0x4
    void (*load)(func_003B0600_sObj*, void*, func_003B0600_sObj*); // 0x8
    char pad_0xC[0x8];
    func_003B0600_cVirt* obj;                      // 0x14
};

extern "C" int func_003B0600(void* p, int a1)
{
    func_003B0600_sObj* self = (func_003B0600_sObj*)p;
    if (self->obj == 0) {
        if (self->loaded == 0) {
            self->load(self, self->arg, self);
        }
    }
    int r;
    if (self->obj != 0) {
        r = self->obj->v03(a1);
    } else {
        r = 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0680);
#ifdef SKIP_ASM
class func_003B0680_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(int a);
};

extern "C" void func_003B0680(void* self, int a)
{
    (*(func_003B0680_cObj**)((char*)self + 0x14))->v06(a);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B06B0);
#ifdef SKIP_ASM
struct func_003B06B0_sReq {
    int pending;
    int arg;
    void (*fn)(func_003B06B0_sReq* self, int arg, int* out);
};

extern "C" int func_003B06B0(func_003B06B0_sReq* self)
{
    int chunk;
    int pendingChunk = self->pending;
    if (pendingChunk != 0) {
        self->pending = 0;
        chunk = pendingChunk;
        return pendingChunk;
    }
    self->fn(self, self->arg, &chunk);
    return chunk;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B06F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sWSCb06F8 {
    int pad0;
    void* ctx;
    int pad8;
    void (*fn)(sWSCb06F8* self, void* ctx, int arg);
};

// The unit later declares func_003B06F8(void*, void*) for its caller; bind this definition by asm label.
void func_003B06F8_impl(sWSCb06F8* self, int arg) __asm__("func_003B06F8");
void func_003B06F8_impl(sWSCb06F8* self, int chunk)
{
    self->fn(self, self->ctx, chunk);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0720__FPv);
#ifdef SKIP_ASM
void* func_003B0720(void* self)
{
    int zero = 0;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)((char*)self + 0x8) = zero;
    *(int*)self = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0738__FPv);
#ifdef SKIP_ASM
void* func_003B0738(void* self)
{
    int zero = 0;
    *(int*)self = zero;
    *(int*)((char*)self + 0xc) = 2;
    *(int*)((char*)self + 0x4) = zero;
    *(int*)((char*)self + 0x8) = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0758);
#ifdef SKIP_ASM
extern "C" void* func_003B0758(void* self, int context, int fetchFn, int releaseFn, int a4)
{
    *(int*)((char*)self + 0xc) = a4;
    *(int*)self = context;
    *(int*)((char*)self + 0x4) = fetchFn;
    *(int*)((char*)self + 0x8) = releaseFn;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0770);
#ifdef SKIP_ASM
extern char D_00509430[];
extern void* D_0050943C[];

extern "C" void* func_003B0770(int init, int prio)
{
    if (prio == 0xFFFF) {
        if (init != 0) {
            func_003B0410(D_00509430);
        } else {
            D_0050943C[0] = (void*)D_00456890;
        }
    }
}
#endif

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07B8__FPv);
#ifdef SKIP_ASM
void* func_003B07B8(void* self)
{
    return func_003B0770(1, 0xffff);
}
#endif

extern "C" void* func_003B0770(int, int);

//99.38%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07D8__FPv);
#ifdef SKIP_ASM
void* func_003B07D8(void* self)
{
    return func_003B0770(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B07F8);
#ifdef SKIP_ASM
extern "C" void* SHAPE_create(unsigned int w, int h, int bpp, int a3, int a4, int flags, int a6, int a7);
extern "C" void* func_003B3D40(void* img);
extern "C" void MEM_fill(void* dst, int value, int size);
extern int D_00509438[];

static inline char* func_003B07F8_data(char* p)
{
    if (*(int*)(p + 0xC) & 0x1000) {
        return p + *(int*)(p + 0x10);
    }
    return p + 0x10;
}

extern "C" void* func_003B07F8(void* self, unsigned int w, int h)
{
    *(int*)((char*)self + 0x0) = 2;
    *(int*)((char*)self + 0x8) = 3;
    *(int*)((char*)self + 0xC) = 3;
    *(int*)((char*)self + 0x10) = 0;
    char* img = (char*)SHAPE_create(w, h, 0x20, 0, 0, D_00509438[0], 0, 0xF0);
    *(char**)((char*)self + 0x4) = img;
    char* hdr = (char*)func_003B3D40(img);
    hdr[4] = 0x32;
    hdr[5] = 0x34;
    hdr[6] = 0x30;
    unsigned int size = ((w >> 4) * h >> 4) << 10;
    MEM_fill(func_003B07F8_data(*(char**)((char*)self + 0x4)), 0, size);
    **(char**)((char*)self + 0x4) = 5;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B08E0);
#ifdef SKIP_ASM
extern "C" void func_003B2B68(char* buf);
extern void* (*D_00509434[])(void*);

extern "C" void func_003B08E0(void* self, int flags)
{
    if (*(void**)((char*)self + 0x4) != 0) {
        func_003B2B68(*(char**)((char*)self + 0x4));
        *(void**)((char*)self + 0x4) = 0;
    }
    *(int*)((char*)self + 0x8) = 7;
    *(int*)((char*)self + 0xC) = 7;
    if (flags & 1) {
        D_00509434[0](self);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0948);
#ifdef SKIP_ASM
extern void* D_00456850[];

struct sWSNode2 {
    sWSNode2* next;
    sWSNode2* prev;
};

static inline void wsListInit(sWSNode2* h)
{
    h->next = (sWSNode2*)3;
    h->prev = (sWSNode2*)3;
    h->next = h;
    h->prev = h;
    h->next = h;
    h->prev = h;
}

extern "C" void* func_003B0948(void* self)
{
    *(void***)self = D_00456850;
    wsListInit((sWSNode2*)((char*)self + 0x18));
    wsListInit((sWSNode2*)((char*)self + 0x20));
    *(int*)((char*)self + 0x2C) = 0;
    *(int*)((char*)self + 0x78) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B09A0);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* D_00456810[];

struct sWSList_09A0 {
    sWSNode2* next;
    sWSNode2* prev;
    int empty() { return next == (sWSNode2*)this; }
    sWSNode2* front() { return next; }
    void kill() { next = (sWSNode2*)7; prev = (sWSNode2*)7; }
};

struct sPl_09A0 {
    void** vt;
    char pad4[0x14];
    sWSList_09A0 list18;
    sWSList_09A0 list20;
    char pad28[4];
    void* work;
    char pad30[0x48];
    void* ipuBuf;
};

static inline void wsUnlink_09A0(sWSNode2* n)
{
    void* obj = n ? (char*)n - 8 : 0;
    sWSNode2* next = n->next;
    sWSNode2* prev = n->prev;
    prev->next = next;
    next->prev = prev;
    n->prev = (sWSNode2*)0xB;
    n->next = (sWSNode2*)0xB;
    if (obj) func_003B08E0(obj, 3);
}

extern "C" void func_003B09A0(sPl_09A0* self, int flags)
{
    self->vt = D_00456850;
    if (self->work != 0) {
        D_00509434[0](self->work);
        self->work = 0;
    }
    if (self->ipuBuf != 0) {
        D_00509434[0](self->ipuBuf);
        self->ipuBuf = 0;
    }
    while (!self->list18.empty()) wsUnlink_09A0(self->list18.front());
    while (!self->list20.empty()) wsUnlink_09A0(self->list20.front());
    self->list20.kill();
    self->list18.kill();
    self->vt = D_00456810;
    if (flags & 1) D_00509434[0](self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0B10);
#ifdef SKIP_ASM
extern "C" void func_003B0B40(void* a, void* b, void* c);

extern "C" int func_003B0B10(void* self, void* a1, void* source)
{
    func_003B0B40(source, self, a1);
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0B40);
#ifdef SKIP_ASM
extern "C" void func_003B06F8(void* req, void* blk);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void func_004029D0(void* dst, void* src, int size);

struct func_003B0B40_sBlock {
    int f0;
    void* data;
    int size;
};

struct func_003B0B40_sScript {
    char pad00[0x14];
    int count;
    char pad18[0x10];
    func_003B06B0_sReq* req;
    char pad2C[0x4C];
    void* hdr;
    char* buf;
};

// PORT: func_003B06B0 returns the block pointer as int.
extern "C" void func_003B0B40(void* source, void* consumer, void* c)
{
    func_003B0B40_sScript* self = (func_003B0B40_sScript*)source;
    void* data = 0;
    func_003B0B40_sBlock* blk = (func_003B0B40_sBlock*)func_003B06B0(self->req);
    if (blk != 0) {
        data = blk->data;
    }
    int total;
    int i;
    if (data != 0) {
        i = blk->size;
        total = (i + 7) & ~0xF;
        i -= 8;
        MEM_copy(self->buf, data, i);
        for (; i < total + 0x10; i += 4) {
            *(int*)(self->buf + i) = 0;
        }
    } else {
        total = 0x10;
        for (i = 0; i < 0x10; i += 4) {
            *(int*)(self->buf + i) = 0xB7010000;
        }
    }
    if (blk != 0) {
        func_003B06F8(self->req, blk);
    }
    self->count++;
    func_004029D0(consumer, self->hdr, total);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0C58);
#ifdef SKIP_ASM
struct sAlloc_B0C58 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_B0C58 D_00509430_B0C58 __asm__("D_00509430");
// mpeg2decode: struct layer_data *ld (current layer), outside small data like the other MPEG globals
struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));
extern int D_00509648[];
// MPEG decoder globals: plain scalars outside small data (the original TU kept them out of $gp)
#define BSS_B0C58 __attribute__((section(".bss")))
extern int D_005094F4_s __asm__("D_005094F4") BSS_B0C58;
extern int D_005094F8_s __asm__("D_005094F8") BSS_B0C58;
extern int D_005094FC_s __asm__("D_005094FC") BSS_B0C58;
extern int D_00509500_s __asm__("D_00509500") BSS_B0C58;
extern int D_00509524_s __asm__("D_00509524") BSS_B0C58;
extern int D_00509548_s __asm__("D_00509548") BSS_B0C58;
extern int D_00509584_s __asm__("D_00509584") BSS_B0C58;
extern int D_0050958C_s __asm__("D_0050958C") BSS_B0C58;
extern int D_005095A0_s __asm__("D_005095A0") BSS_B0C58;
extern int D_005094D4_s __asm__("D_005094D4") BSS_B0C58;
extern int D_005094D8_s __asm__("D_005094D8") BSS_B0C58;
extern int D_005094DC_s __asm__("D_005094DC") BSS_B0C58;
extern int D_005094E0_s __asm__("D_005094E0") BSS_B0C58;
extern char D_004953E8[];
extern char str_RCMP_mpegbuff[];
extern char str_RCMP_chunkbuff[];
extern "C" void func_003B1228(void* data);
extern "C" int func_003B13D8(void);
extern "C" void* func_003B07F8(void* self, unsigned int w, int h);
extern "C" void func_00402708(void);
extern "C" void func_004027B8(void* a, void* b, int size);
extern "C" void* func_00402C08(void* a, int n, void* fn, void* arg);
extern "C" int func_003B0B10(void* self, void* a1, void* a2);

struct sWSNode_B0C58 {
    sWSNode_B0C58* next;
    sWSNode_B0C58* prev;
};

struct sScript_B0C58 {
    char pad00[0x10];
    int count;
};

struct sData_B0C58 {
    int f0;
    void* data;
};

struct sPlayer_B0C58 {
    int f0;
    int state;              // 0x4
    int w;                  // 0x8
    int h;                  // 0xC
    int f10;                // 0x10
    int f14;                // 0x14
    sWSNode_B0C58 list18;   // 0x18
    sWSNode_B0C58 frames;   // 0x20
    sScript_B0C58* script;  // 0x28
    void* work;             // 0x2C
    char mpeg[0x48];        // 0x30
    void* ipuBuf;           // 0x78
    void* ipuBufUc;         // 0x7C
};

extern "C" int func_003B0C58(sPlayer_B0C58* self, sScript_B0C58* script, sData_B0C58* d)
{
    self->script = script;
    self->f10 = 0;
    self->f14 = 0;
    if (self->work != 0) {
        D_00509430_B0C58.free(self->work);
        self->work = 0;
    }
    if (self->ipuBuf != 0) {
        D_00509430_B0C58.free(self->ipuBuf);
        self->ipuBuf = 0;
    }
    self->state = 1;
    void* data = d->data;
    ld = (layer_data*)D_00509648;
    func_003B1228(data);
    func_003B13D8();
    int mode = D_00509648[0x420 / 4];
    if (mode == 0) {
        D_00509524_s = 1;
        D_005095A0_s = 1;
        D_00509584_s = 3;
        D_0050958C_s = 1;
        D_00509548_s = 5;
    }
    D_005094FC_s = (D_005094F4_s + 15) / 16;
    D_00509500_s = (mode != 0 && D_00509524_s == 0) ? (D_005094F8_s + 31) / 32 * 2 : (D_005094F8_s + 15) / 16;
    int w = D_005094FC_s * 16;
    D_005094D4_s = w;
    self->w = w;
    int h = D_00509500_s * 16;
    D_005094D8_s = h;
    self->h = h;
    D_005094DC_s = w >> 1;
    D_005094E0_s = h >> 1;
    int n = self->script->count;
    for (int i = 0; i < n; i++) {
        int cw = self->w;
        int ch = self->h;
        char* r = (char*)func_003B07F8(D_00509430_B0C58.alloc(D_004953E8, 0x14, 0, 0, D_00509430_B0C58.flags), cw, ch);
        sWSNode_B0C58* node = r ? (sWSNode_B0C58*)(r + 8) : 0;
        sWSNode_B0C58* head = &self->frames;
        sWSNode_B0C58* old = head->next;
        head->next = node;
        old->prev = node;
        node->next = old;
        node->prev = head;
    }
    self->work = D_00509430_B0C58.alloc(str_RCMP_mpegbuff, self->w * self->h * 9 / 2 + 0x176C, 0, 0, D_00509430_B0C58.flags | 0x400);
    self->ipuBuf = D_00509430_B0C58.alloc(str_RCMP_chunkbuff, 0x32000, 0, 0, D_00509430_B0C58.flags | 0x400);
    self->ipuBufUc = (void*)(((unsigned int)self->ipuBuf & 0x0FFFFFFF) | 0x30000000);
    func_00402708();
    func_004027B8(self->mpeg, self->work, self->w * self->h * 9 / 2 + 0x176C);
    func_00402C08(self->mpeg, 1, (void*)func_003B0B10, self);
    return 2;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0FA0__FPv);
#ifdef SKIP_ASM
int func_003B0FA0(void* self)
{
    return *(int*)((char*)self + 0x10);
}
#endif

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sD_00509508 { float value; int pad[2]; };
extern sD_00509508 D_00509508;

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0FA8);
#ifdef SKIP_ASM
extern "C" float func_003B0FA8()
{
    return D_00509508.value;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B0FB8);
#ifdef SKIP_ASM
struct sWSPool;
struct sWSItem;
extern "C" sWSItem* func_003B10D0(sWSPool* self);
extern "C" void func_00402A10(void* dma, void* data, int qwc);

extern "C" void* func_003B0FB8(void* self)
{
    sWSItem* item = func_003B10D0((sWSPool*)self);
    char* hdr = *(char**)((char*)item + 0x4);
    char* data;
    if (*(int*)(hdr + 0xC) & 0x1000) {
        data = hdr + *(int*)(hdr + 0x10);
    } else {
        data = hdr + 0x10;
    }
    func_00402A10((char*)self + 0x30, data, *(int*)((char*)self + 0x8) / 16 * *(int*)((char*)self + 0xC) / 16);
    *(int*)((char*)self + 0x10) += 1;
    return item;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1050);
#ifdef SKIP_ASM
extern "C" void* func_003B0FB8(void* self);
extern "C" int func_00402B38(void* sema);

class func_003B1050_cCache {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06(void*);
};

extern "C" void* func_003B1050(func_003B1050_cCache* self, unsigned int limit)
{
    void* item = 0;
    do {
        if (item != 0) {
            self->v06(item);
        }
        if (func_00402B38((char*)self + 0x30) != 0) {
            return 0;
        }
        item = func_003B0FB8(self);
    } while (*(unsigned int*)((char*)self + 0x10) < limit);
    return item;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B10D0);
#ifdef SKIP_ASM
struct sWSNode {
    sWSNode* next;
    sWSNode* prev;

    void unlink()
    {
        sWSNode* n = next;
        sWSNode* p = prev;
        p->next = n;
        n->prev = p;
        prev = next = (sWSNode*)0xB;
    }
};

struct sWSList {
    sWSNode head;

    void addTail(sWSNode* n)
    {
        sWSNode* tail = head.prev;
        tail->next = n;
        head.prev = n;
        n->prev = tail;
        n->next = &head;
    }
};

struct sWSItemBase {
    int a;
    int b;
};

struct sWSItem : sWSItemBase, sWSNode {
    int refs; // 0x10
};

struct sWSPool {
    char pad00[0x18];
    sWSList used; // 0x18
    sWSList free; // 0x20
};

extern "C" sWSItem* func_003B10D0(sWSPool* self)
{
    sWSNode* n = self->free.head.next;
    if (n != &self->free.head) {
        sWSItem* item;
        n->unlink();
        item = static_cast<sWSItem*>(n);
        self->used.addTail(item);
        item->refs = 1;
        return item;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1140);
#ifdef SKIP_ASM
extern "C" void func_003B1140(sWSPool* self, sWSItem* item)
{
    if (--item->refs > 0) {
        return;
    }
    item->unlink();
    self->free.addTail(item);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B11A0);
#ifdef SKIP_ASM
struct sAlloc_11A0 {
    void* (*alloc)(const char* tag, int size, int align, int d, int flags);
    void* (*free)(void* p);
    int flags;
};
// PORT: the unit declares D_00509430 as char[] later; view bound by asm label
extern sAlloc_11A0 D_00509430_11A0 __asm__("D_00509430");
extern "C" void* func_003B11A0(void)
{
    return func_003B0948(D_00509430_11A0.alloc(D_004953E8, 0x80, 0, 0, D_00509430_11A0.flags));
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B11E8__FPv);
#ifdef SKIP_ASM
void func_003B11E8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B11F0);
#ifdef SKIP_ASM
extern int D_00495448[];

extern "C" int func_003B11F0(int v)
{
    unsigned int i;
    for (i = 0; i < 2; i++) {
        if (D_00495448[i] == v) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1228);
#ifdef SKIP_ASM
struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));

extern "C" void func_003B1290(int n);

extern "C" void func_003B1228(void* data)
{
    char* reader = (char*)ld;
    *(void**)(reader + 0x0) = data;
    *(int*)(reader + 0x8) = 0;
    *(int*)(reader + 0x4) = 0;
    func_003B1290(0);
}
#endif

struct layer_data;
extern layer_data* ld __asm__("D_0050A088") __attribute__((section(".bss")));

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1258);
#ifdef SKIP_ASM
extern "C" unsigned int func_003B1258(int numBits)
{
    return *(unsigned int*)((char*)ld + 0x4) >> (unsigned int)(-numBits);
}
#endif

extern "C" void* func_003B1300(int);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1270__FPv);
#ifdef SKIP_ASM
void* func_003B1270(void* self)
{
    return func_003B1300(1);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1290);
#ifdef SKIP_ASM
struct sBitReader_1290 {
    unsigned char* p;   // 0x0
    unsigned int bits;  // 0x4
    int count;          // 0x8
};

// Reads the next byte into the bit buffer; taking the reader by reference matches the target's register copy.
static inline void FillByte_1290(sBitReader_1290*& r, int c)
{
    r->bits |= *r->p++ << (24 - c);
}

// Drops n bits from the bit buffer and refills it a byte at a time.
extern "C" void func_003B1290(int n)
{
    sBitReader_1290* reader = (sBitReader_1290*)ld;
    reader->bits <<= n;
    int bitCount = reader->count - n;
    while (bitCount < 25) {
        FillByte_1290(reader, bitCount);
        bitCount += 8;
    }
    ((sBitReader_1290*)ld)->count = bitCount;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1300);
#ifdef SKIP_ASM
extern "C" void func_003B1290(int n);

// PORT: returns a bit-field value as void* to fit the unit's existing
// `extern "C" void* func_003B1300(int)` declaration (really unsigned int).
extern "C" void* func_003B1300(int n)
{
    unsigned int bits = func_003B1258(n);
    func_003B1290(n);
    return (void*)bits;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1340);
#ifdef SKIP_ASM
struct sBitReader_1340 {
    unsigned char* p;   // 0x0
    unsigned int bits;  // 0x4
    int count;          // 0x8
};

// Reads the next byte into the bit buffer; taking the reader by reference matches the target's register copy.
static inline void FillByte_1340(sBitReader_1340*& r, int c)
{
    r->bits |= *r->p++ << (24 - c);
}

// Drops the whole 32-bit buffer and refills it a byte at a time.
extern "C" void func_003B1340(void)
{
    sBitReader_1340* reader = (sBitReader_1340*)ld;
    reader->bits = 0;
    int bitCount = reader->count - 32;
    while (bitCount < 25) {
        FillByte_1340(reader, bitCount);
        bitCount += 8;
    }
    ((sBitReader_1340*)ld)->count = bitCount;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B13A8);
#ifdef SKIP_ASM
extern "C" void func_003B1340(void);

extern "C" unsigned int func_003B13A8(void)
{
    unsigned int bits = func_003B1258(0x20);
    func_003B1340();
    return bits;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B13D8);
#ifdef SKIP_ASM
// PORT: func_003B1498 takes no arguments (the unit declares it void*(void*)); bound by asm label
extern "C" void func_003B1498_noarg(void) __asm__("func_003B1498");
extern "C" void func_003B14F0(void);
extern "C" void group_of_pictures_header(void);
extern "C" void func_003B17A8(void);

extern "C" int func_003B13D8(void)
{
    for (;;) {
        func_003B1498_noarg();
        switch ((int)func_003B13A8()) {
        case 0x1B3:
            func_003B14F0();
            break;
        case 0x1B8:
            group_of_pictures_header();
            break;
        case 0x1B7:
            return 0;
        case 0x100:
            func_003B17A8();
            return 1;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1498);
#ifdef SKIP_ASM
extern "C" void func_003B1290(int n);

// PORT: no return value here; the unit declares it void* and callers forward $v0.
void func_003B1498_impl(void* self) __asm__("func_003B1498");

void func_003B1498_impl(void* self)
{
    func_003B1290(*(int*)((char*)ld + 0x8) & 7);
    while (func_003B1258(0x18) != 1) {
        func_003B1290(8);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B14F0);
#ifdef SKIP_ASM
extern int D_005094F4[];
extern int D_005094F8[];
extern void* D_0050950C[];
extern int D_00509510[];
extern int D_00509514[];
extern int D_00509518[];
extern void* D_0050951C[];
extern unsigned char D_0044C218[];
extern unsigned char D_0044C298[];
extern char str_sequence_header[];
void* func_003B2360(void* self);
extern "C" void func_003B1988(void);

// mpeg2decode's struct layer_data (EA's memory-buffer variant); the unit declares `ld` (D_0050A088).
struct layer_data {
    char pad0[0xC];
    int Bitcnt;                                 // 0x00C
    int intra_quantizer_matrix[64];             // 0x010
    int non_intra_quantizer_matrix[64];         // 0x110
    int chroma_intra_quantizer_matrix[64];      // 0x210
    int chroma_non_intra_quantizer_matrix[64];  // 0x310
    void* load_intra_quantizer_matrix;          // 0x410 (the unit's Get_Bits returns void*)
    void* load_non_intra_quantizer_matrix;      // 0x414
    int load_chroma_intra_quantizer_matrix;     // 0x418
    int load_chroma_non_intra_quantizer_matrix; // 0x41C
    int MPEG2_Flag;                             // 0x420
    int scalable_mode;                          // 0x424 (sequence scalable extension)
};

// MPEG2_sequence_header (mpeg2decode getheader.c sequence_header()).
extern "C" void func_003B14F0(void)
{
    int i;
    int pos;

    pos = ld->Bitcnt;  // mpeg2decode reads it unconditionally; only its VERBOSE trace uses pos
    D_005094F4[0] = (int)func_003B1300(0xC);
    D_005094F8[0] = (int)func_003B1300(0xC);
    D_0050950C[0] = func_003B1300(4);
    D_00509510[0] = (int)func_003B1300(4);
    D_00509514[0] = (int)func_003B1300(0x12);
    func_003B2360(str_sequence_header);
    D_00509518[0] = (int)func_003B1300(0xA);
    D_0050951C[0] = func_003B1300(1);
    if ((ld->load_intra_quantizer_matrix = func_003B1300(1)) != 0) {
        for (i = 0; i < 64; i++)
            ld->intra_quantizer_matrix[D_0044C218[i]] = (int)func_003B1300(8);
    } else {
        for (i = 0; i < 64; i++)
            ld->intra_quantizer_matrix[i] = D_0044C298[i];
    }
    if ((ld->load_non_intra_quantizer_matrix = func_003B1300(1)) != 0) {
        for (i = 0; i < 64; i++)
            ld->non_intra_quantizer_matrix[D_0044C218[i]] = (int)func_003B1300(8);
    } else {
        for (i = 0; i < 64; i++)
            ld->non_intra_quantizer_matrix[i] = 16;
    }
    for (i = 0; i < 64; i++) {
        ld->chroma_intra_quantizer_matrix[i] = ld->intra_quantizer_matrix[i];
        ld->chroma_non_intra_quantizer_matrix[i] = ld->non_intra_quantizer_matrix[i];
    }
    func_003B1988();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", group_of_pictures_header);
#ifdef SKIP_ASM
extern int D_00509648[];
extern int D_0044C378[];
extern int D_0044C37C[];
extern int D_0044C380[];
extern void* D_00509628[];
extern void* D_0050962C[];
extern void* D_00509630[];
extern void* D_00509634[];
extern void* D_00509638[];
extern void* D_0050963C[];
extern void* D_00509640[];
extern char str_group_of_pictures_header[];
void* func_003B2360(void* self);
extern "C" void func_003B1988(void);

/**
 * Parses an MPEG-2 group-of-pictures header from the bitstream (mpeg2decode's getheader.c).
 *
 * On the base layer it first resets the temporal reference base to the last frame number + 1 and flags a GOP
 * reset. Then reads drop_flag, hour, minute, a marker bit, second, frame, closed_gop and broken_link into the
 * decoder's globals (D_00509628-D_00509640), and finishes with the extension and user data (func_003B1988).
 */
extern "C" void group_of_pictures_header(void)
{
    if (ld == (layer_data*)D_00509648) {
        D_0044C378[0] = D_0044C37C[0] + 1;
        D_0044C380[0] = 1;
    }
    D_00509628[0] = func_003B1300(1);
    D_0050962C[0] = func_003B1300(5);
    D_00509630[0] = func_003B1300(6);
    func_003B2360(str_group_of_pictures_header);
    D_00509634[0] = func_003B1300(6);
    D_00509638[0] = func_003B1300(6);
    D_0050963C[0] = func_003B1300(1);
    D_00509640[0] = func_003B1300(1);
    func_003B1988();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B17A8);
#ifdef SKIP_ASM
extern int D_00509554[];
extern int D_00509558[];
extern void* D_0050955C[];
extern void* D_00509560[];
extern void* D_00509564[];
extern void* D_00509568[];
extern void* D_0050956C[];
extern "C" void func_003B1988(void);
extern "C" int func_003B2318(void);
extern "C" void func_003B2440(void);

// PORT: func_003B1300 really returns unsigned int (unit declares void*)
extern "C" void func_003B17A8(void)
{
    *(int*)((char*)ld + 0x430) = 0;
    D_00509554[0] = (int)func_003B1300(0xA);
    D_00509558[0] = (int)func_003B1300(3);
    D_0050955C[0] = func_003B1300(0x10);
    if (D_00509558[0] == 2 || D_00509558[0] == 3) {
        D_00509560[0] = func_003B1300(1);
        D_00509564[0] = func_003B1300(3);
    }
    if (D_00509558[0] == 3) {
        D_00509568[0] = func_003B1300(1);
        D_0050956C[0] = func_003B1300(3);
    }
    func_003B2318();
    func_003B1988();
    func_003B2440();
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1878);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern unsigned char D_0044C2D8[];
extern int D_005094F8_B1878[] __asm__("D_005094F8");

extern "C" int func_003B1878(void)
{
    int r;
    int v;
    int t;

    if (*(int*)((char*)ld + 0x420) != 0) {
        r = 0;
        if (D_005094F8_B1878[0] >= 0xAF1) {
            r = (int)func_003B1300(3);
        }
    } else {
        r = 0;
    }
    if (*(int*)((char*)ld + 0x424) == 1) {
        *(int*)((char*)ld + 0x434) = (int)func_003B1300(7);
    }
    t = (int)func_003B1300(5);
    {
        char *p = (char*)ld;
        *(int*)(p + 0x438) = *(int*)(p + 0x420) != 0
            ? (*(int*)(p + 0x428) != 0 ? (int)D_0044C2D8[t] : t * 2)
            : t;
    }
    if (func_003B1300(1) != 0) {
        *(int*)((char*)ld + 0x43C) = (int)func_003B1300(1);
        func_003B1300(1);
        func_003B1300(6);
        func_003B2318();
    } else {
        *(int*)((char*)ld + 0x43C) = 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1988);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" unsigned int func_003B1258(int a0);
extern "C" unsigned int func_003B1300_u(int n) __asm__("func_003B1300");
extern "C" void func_003B1340(void);
extern "C" void func_003B1498_v(void) __asm__("func_003B1498");
extern "C" void func_003B2380_v(void) __asm__("func_003B2380__FPv");
extern "C" void func_003B1AB0(void);
extern "C" void func_003B1CC0(void);
extern "C" void func_003B1D58(void);
extern "C" void sequence_scalable_extension(void);
extern "C" void func_003B1FC8(void);
extern "C" void func_003B20B8(void);
extern "C" void picture_spatial_scalable_extension(void);
extern "C" void func_003B22F8(void);
extern "C" void copyright_extension(void);

// Parses a script block: runs one command per 0x1B5 token, skips 0x1B2 sections.
extern "C" void func_003B1988(void)
{
    unsigned int tok;
    func_003B1498_v();
    while ((tok = func_003B1258(0x20)) == 0x1B5 || tok == 0x1B2) {
        if (tok == 0x1B5) {
            func_003B1340();
            switch (func_003B1300_u(4)) {
            case 1: func_003B1AB0(); break;
            case 2: func_003B1CC0(); break;
            case 3: func_003B1D58(); break;
            case 5: sequence_scalable_extension(); break;
            case 7: func_003B1FC8(); break;
            case 8: func_003B20B8(); break;
            case 9: picture_spatial_scalable_extension(); break;
            case 10: func_003B22F8(); break;
            case 4: copyright_extension(); break;
            case 6: break;
            }
            func_003B1498_v();
        } else {
            func_003B1340();
            func_003B2380_v();
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1AB0);
#ifdef SKIP_ASM
extern int D_005095DC[];
extern int D_00509520[];
extern int D_00509524[];
extern int D_00509528[];
extern int D_0050952C[];
extern int D_00509530[];
extern int D_00509534[];
extern int D_00509510[];
extern int D_00509514[];
extern int D_00509518[];
extern int D_005094EC[];
extern int D_005094F0[];
extern int D_005094F4[];
extern int D_005094F8[];
extern float D_00509504[];
struct sD_00509508;
extern float D_0044C388[];
extern char str_sequence_extension[];
void* func_003B2360(void* self);

struct sMpegCtx1AB0 {
    char pad[0x420];
    int f420;
    int f424;
};

// Parse an MPEG-2 sequence extension.
extern "C" void func_003B1AB0(void)
{
    sMpegCtx1AB0* g = (sMpegCtx1AB0*)ld;
    g->f420 = 1;
    g->f424 = 0;
    D_005095DC[0] = 0;
    D_00509520[0] = (int)func_003B1300(8);
    D_00509524[0] = (int)func_003B1300(1);
    D_00509528[0] = (int)func_003B1300(2);
    int hext = (int)func_003B1300(2);
    int vext = (int)func_003B1300(2);
    int brext = (int)func_003B1300(0xC);
    func_003B2360(str_sequence_extension);
    int vbvext = (int)func_003B1300(8);
    D_0050952C[0] = (int)func_003B1300(1);
    D_00509530[0] = (int)func_003B1300(2);
    D_00509534[0] = (int)func_003B1300(5);
    D_00509508.value = D_0044C388[D_00509510[0]] * ((float)(D_00509530[0] + 1) / (float)(D_00509534[0] + 1));
    if ((D_00509520[0] >> 7) & 1) {
        if ((D_00509520[0] & 0xF) == 5) {
            D_005094EC[0] = 0x85;
            D_005094F0[0] = 8;
        }
    } else {
        D_005094EC[0] = D_00509520[0] >> 4;
        D_005094F0[0] = D_00509520[0] & 0xF;
    }
    D_005094F4[0] = (hext << 12) | (D_005094F4[0] & 0xFFF);
    D_005094F8[0] = (vext << 12) | (D_005094F8[0] & 0xFFF);
    D_00509514[0] = D_00509514[0] + (brext << 18);
    D_00509504[0] = D_00509514[0] * 400.0f;
    D_00509518[0] = D_00509518[0] + (vbvext << 10);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1CC0);
#ifdef SKIP_ASM
extern void* D_00509538[];
extern void* D_0050953C[];
extern void* D_00509540[];
extern void* D_00509544[];
extern void* D_00509548[];
extern void* D_0050954C[];
extern void* D_00509550[];
extern char str_sequence_display_extension[];
void* func_003B2360(void* self);

extern "C" void func_003B1CC0(void)
{
    D_00509538[0] = func_003B1300(3);
    D_0050953C[0] = func_003B1300(1);
    if (D_0050953C[0] != 0) {
        D_00509540[0] = func_003B1300(8);
        D_00509544[0] = func_003B1300(8);
        D_00509548[0] = func_003B1300(8);
    }
    D_0050954C[0] = func_003B1300(0xE);
    func_003B2360(str_sequence_display_extension);
    D_00509550[0] = func_003B1300(0xE);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1D58);
#ifdef SKIP_ASM
struct sWSTables1D58 {
    char pad0[0x10];
    void* a10[64];              // 0x010
    void* a110[64];             // 0x110
    void* a210[64];             // 0x210
    void* a310[64];             // 0x310
    void* p410;                 // 0x410
    void* p414;                 // 0x414
    void* p418;                 // 0x418
    void* p41C;                 // 0x41C
};

extern unsigned char D_0044C218[];


extern "C" void func_003B1D58(void)
{
    layer_data** gd = &ld;
    {
        void* p = func_003B1300(1);
        ((sWSTables1D58*)*gd)->p410 = p;
        if (p != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* q = func_003B1300(8);
                int k = D_0044C218[i];
                sWSTables1D58* g = ((sWSTables1D58*)ld);
                g->a10[k] = q;
                g->a210[k] = q;
            }
        }
    }
    {
        void* p = func_003B1300(1);
        ((sWSTables1D58*)ld)->p414 = p;
        if (p != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* q = func_003B1300(8);
                int k = D_0044C218[i];
                sWSTables1D58* g = ((sWSTables1D58*)ld);
                g->a110[k] = q;
                g->a310[k] = q;
            }
        }
    }
    {
        void* p = func_003B1300(1);
        ((sWSTables1D58*)ld)->p418 = p;
        if (p != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* q = func_003B1300(8);
                int k = D_0044C218[i];
                sWSTables1D58* g = ((sWSTables1D58*)ld);
                g->a210[k] = q;
            }
        }
    }
    {
        void* p = func_003B1300(1);
        ((sWSTables1D58*)ld)->p41C = p;
        if (p != 0) {
            int i;
            for (i = 0; i < 64; i++) {
                void* q = func_003B1300(8);
                int k = D_0044C218[i];
                sWSTables1D58* g = ((sWSTables1D58*)ld);
                g->a310[k] = q;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", sequence_scalable_extension);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// mpeg2decode global.h: the sequence scalable extension's plain int globals
extern int layer_id __asm__("D_005095DC");
extern int lower_layer_prediction_horizontal_size __asm__("D_005095E0");
extern int lower_layer_prediction_vertical_size __asm__("D_005095E4");
extern int horizontal_subsampling_factor_m __asm__("D_005095E8");
extern int horizontal_subsampling_factor_n __asm__("D_005095EC");
extern int vertical_subsampling_factor_m __asm__("D_005095F0");
extern int vertical_subsampling_factor_n __asm__("D_005095F4");
extern char str_sequence_scalable_extension[];
extern char D_00495510[];
void* func_003B2360(void* self);
// Get_Bits returning int: with the unit's void* (or mpeg2decode's unsigned int) declaration GCC keeps the
// globals' %hi parts in s-registers across the calls.
extern "C" int Get_Bits(int n) __asm__("func_003B1300");
void func_003B11E8(void* self);

/**
 * Parses an MPEG-2 sequence scalable extension from the bitstream (mpeg2decode's getheader.c).
 *
 * Reads the scalable mode (stored + 1) and layer ID; spatial scalability also reads the lower layer's
 * prediction size and the subsampling factors. Temporal scalability is reported as unsupported through the
 * error routine (func_003B11E8).
 */
// mpeg2decode getheader.c sequence_scalable_extension(); func_003B1300 = Get_Bits, func_003B2360 = marker_bit,
// func_003B11E8 = Error.
extern "C" void sequence_scalable_extension(void)
{
    int pos;

    pos = ld->Bitcnt;  // mpeg2decode reads it unconditionally; only its VERBOSE trace uses pos

    /* values (without the +1 offset) of scalable_mode are defined in Table 6-10 of ISO/IEC 13818-2 */
    ld->scalable_mode = Get_Bits(2) + 1; /* add 1 to make SC_DP != SC_NONE */

    layer_id = Get_Bits(4);

    if (ld->scalable_mode == 2) { /* SC_SPAT */
        lower_layer_prediction_horizontal_size = Get_Bits(14);
        func_003B2360(str_sequence_scalable_extension);
        lower_layer_prediction_vertical_size = Get_Bits(14);
        horizontal_subsampling_factor_m = Get_Bits(5);
        horizontal_subsampling_factor_n = Get_Bits(5);
        vertical_subsampling_factor_m = Get_Bits(5);
        vertical_subsampling_factor_n = Get_Bits(5);
    }

    if (ld->scalable_mode == 4) /* SC_TEMP */
        func_003B11E8(D_00495510);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B1FC8);
#ifdef SKIP_ASM
extern int D_00509524[];
extern int D_00509584[];
extern int D_00509588[];
extern int D_00509598[];
extern void* D_005095C0[];
extern void* D_005095D0[];
extern char str_picture_display_extension_first_marker_b[];
extern char str_picture_display_extension_second_marker[];
void* func_003B2360(void* self);

extern "C" void func_003B1FC8(void)
{
    int n;
    int i;
    if (D_00509524[0] != 0) {
        if (D_00509598[0] == 0) {
            n = 1;
        } else {
            n = D_00509588[0] != 0 ? 3 : 2;
        }
    } else if (D_00509584[0] != 3) {
        n = 1;
    } else {
        n = D_00509598[0] != 0 ? 3 : 2;
    }
    for (i = 0; i < n; i++) {
        D_005095C0[i] = func_003B1300(0x10);
        func_003B2360(str_picture_display_extension_first_marker_b);
        D_005095D0[i] = func_003B1300(0x10);
        func_003B2360(str_picture_display_extension_second_marker);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B20B8);
#ifdef SKIP_ASM
extern void* D_00509570[];
extern void* D_00509580[];
extern int D_00509584[];
extern int D_00509588[];
extern void* D_0050958C[];
extern void* D_00509590[];
extern void* D_00509594[];
extern int D_00509598[];
extern void* D_0050959C[];
extern void* D_005095A0[];
extern void* D_005095A4[];
extern void* D_005095A8[];
extern void* D_005095AC[];
extern void* D_005095B0[];
extern void* D_005095B4[];
extern void* D_005095B8[];

extern "C" void func_003B20B8(void)
{
    D_00509570[0] = func_003B1300(4);
    D_00509570[1] = func_003B1300(4);
    D_00509570[2] = func_003B1300(4);
    D_00509570[3] = func_003B1300(4);
    D_00509580[0] = func_003B1300(2);
    D_00509584[0] = (int)func_003B1300(2);
    D_00509588[0] = (int)func_003B1300(1);
    D_0050958C[0] = func_003B1300(1);
    D_00509590[0] = func_003B1300(1);
    *(void**)((char*)ld + 0x428) = func_003B1300(1);
    D_00509594[0] = func_003B1300(1);
    *(void**)((char*)ld + 0x42C) = func_003B1300(1);
    D_00509598[0] = (int)func_003B1300(1);
    D_0050959C[0] = func_003B1300(1);
    D_005095A0[0] = func_003B1300(1);
    D_005095A4[0] = func_003B1300(1);
    if (D_005095A4[0] != 0) {
        D_005095A8[0] = func_003B1300(1);
        D_005095AC[0] = func_003B1300(3);
        D_005095B0[0] = func_003B1300(1);
        D_005095B4[0] = func_003B1300(7);
        D_005095B8[0] = func_003B1300(8);
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", picture_spatial_scalable_extension);
#ifdef SKIP_ASM
extern void* D_005095F8[];
extern int D_005095FC[];
extern int D_00509600[];
extern void* D_00509604[];
extern void* D_00509608[];
extern void* D_0050960C[];
extern char str_picture_spatial_scalable_extension_first[];
extern char str_picture_spatial_scalable_extension_secon[];
void* func_003B2360(void* self);

/**
 * Reads an MPEG-2 picture spatial scalable extension header (as in the reference mpeg2decode's gethdr.c).
 *
 * Reads the lower layer's temporal reference, its signed 15-bit horizontal and vertical offsets (each after a
 * checked marker bit), the spatial-temporal weight table index and two lower-layer field flags, and marks the
 * stream as spatially scalable (ld+0x430).
 */
// PORT: func_003B1300 really returns unsigned int (unit declares void*)
extern "C" void picture_spatial_scalable_extension(void)
{
    *(int*)((char*)ld + 0x430) = 1;
    D_005095F8[0] = func_003B1300(0xA);
    func_003B2360(str_picture_spatial_scalable_extension_first);
    D_005095FC[0] = (int)func_003B1300(0xF);
    if (D_005095FC[0] >= 0x4000) {
        D_005095FC[0] -= 0x8000;
    }
    func_003B2360(str_picture_spatial_scalable_extension_secon);
    D_00509600[0] = (int)func_003B1300(0xF);
    if (D_00509600[0] >= 0x4000) {
        D_00509600[0] -= 0x8000;
    }
    D_00509604[0] = func_003B1300(2);
    D_00509608[0] = func_003B1300(1);
    D_0050960C[0] = func_003B1300(1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B22F8);
#ifdef SKIP_ASM
extern int D_00495608[];

extern "C" void func_003B22F8(void)
{
    func_003B11E8(D_00495608);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B2318);
#ifdef SKIP_ASM
// PORT: func_003B1270 takes a void* it never reads; this caller passes nothing, so it binds
// a no-argument declaration to the mangled symbol.
void* func_003B1270_noarg() __asm__("func_003B1270__FPv");

extern "C" int func_003B2318(void)
{
    int n = 0;
    while (func_003B1270_noarg() != 0) {
        func_003B1290(8);
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B2360__FPv);
#ifdef SKIP_ASM
void* func_003B2360(void* self)
{
    return func_003B1300(1);
}
#endif

extern "C" void* func_003B1498(void* self);

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B2380__FPv);
#ifdef SKIP_ASM
void* func_003B2380(void* self)
{
    return func_003B1498(self);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", copyright_extension);
#ifdef SKIP_ASM
extern void* D_00509610[];
extern void* D_00509614[];
extern void* D_00509618[];
extern void* D_0050961C[];
extern void* D_00509620[];
extern void* D_00509624[];
extern char str_copyright_extension_first_marker_bit[];
extern char str_copyright_extension_second_marker_bit[];
extern char str_copyright_extension_third_marker_bit[];
void* func_003B2360(void* self);

/**
 * Reads an MPEG-2 copyright extension header into globals (as in the reference mpeg2decode's gethdr.c).
 *
 * Copyright flag, identifier, original-or-copy flag, 7 reserved bits, then the 20-, 22- and 22-bit copyright
 * number parts, each after a checked marker bit.
 */
extern "C" void copyright_extension(void)
{
    D_00509610[0] = func_003B1300(1);
    D_00509614[0] = func_003B1300(8);
    D_00509618[0] = func_003B1300(1);
    func_003B1300(7);
    func_003B2360(str_copyright_extension_first_marker_bit);
    D_0050961C[0] = func_003B1300(0x14);
    func_003B2360(str_copyright_extension_second_marker_bit);
    D_00509620[0] = func_003B1300(0x16);
    func_003B2360(str_copyright_extension_third_marker_bit);
    D_00509624[0] = func_003B1300(0x16);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B2440);
#ifdef SKIP_ASM
extern int D_00509648[];
extern int D_00509554[];
extern int D_00509558[];
extern int D_0044C378[];
extern int D_0044C37C[];
extern int D_0044C380[];
extern int D_0044C3C8[];
extern int D_0044C3CC[];
extern int D_0050A098[];

extern "C" void func_003B2440(void)
{
    if (ld != (layer_data*)D_00509648) {
        return;
    }
    int last;
    if (D_00509558[0] != 3 && D_00509554[0] != (last = D_0044C3CC[0])) {
        if (D_0044C3C8[0] != 0) {
            D_0044C3C8[0] = 0;
            D_0044C378[0] += 0x400;
        }
        if (D_00509554[0] < last && D_0044C380[0] == 0) {
            D_0044C3C8[0] = 1;
        }
        D_0044C380[0] = 0;
        D_0044C3CC[0] = D_00509554[0];
    }
    D_0050A098[0] = D_0044C378[0] + D_00509554[0];
    if (D_0044C3C8[0] != 0 && D_0044C3CC[0] >= D_00509554[0]) {
        D_0050A098[0] += 0x400;
    }
    D_0044C37C[0] = (D_0044C37C[0] < D_0050A098[0]) ? D_0050A098[0] : D_0044C37C[0];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_bpctobpp);
#ifdef SKIP_ASM
struct SHAPE_bpctobpp_sKey {
    char c;
};

extern "C" char SHAPE_type(int key);
extern "C" void SHAPE_depth(SHAPE_bpctobpp_sKey* k);

/**
 * Converts a bits-per-component format code to its bits per pixel.
 *
 * Maps @p key to a shape type with SHAPE_type() (codes such as 4, 8, 15, 16, 555, 1555, 32 and 8888, decimal)
 * and looks the type up with SHAPE_depth(). A @p key of 0 is treated as 32. Declared void here, but
 * SHAPE_depth()'s result is still in the return register when it returns.
 *
 * @param key  format code, or 0 for 32
 */
extern "C" void SHAPE_bpctobpp(int key)
{
    SHAPE_bpctobpp_sKey k;
    if (key == 0) {
        key = 0x20;
    }
    k.c = SHAPE_type(key);
    SHAPE_depth(&k);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_imagesize);
#ifdef SKIP_ASM
// PORT: the unit declares SHAPE_bpctobpp `void`, but it returns SHAPE_depth's result
// (the format's bits per pixel); bound via asm label.
extern "C" int SHAPE_bpctobpp_bits(int fmt) __asm__("SHAPE_bpctobpp");

/**
 * Computes the bytes needed for a texture's pixels, including its mipmap levels.
 *
 * The bits per pixel come from SHAPE_bpctobpp_bits(fmt) (16 when it returns 15). Each level halves the size
 * (down to 1x1, where it stops), rows round up to whole bytes, and each level after the first starts 16-byte
 * aligned.
 *
 * @param w         width in pixels, 1 to 65536
 * @param height         height in pixels, 1 to 65536
 * @param fmt       pixel format code (On Tour: bpc)
 * @param maxLevel  last mipmap level to include (0 for none)
 * @return the size in bytes, or 0 for an invalid size or a format SHAPE_type() rejects
 */
extern "C" int SHAPE_imagesize(int w, int height, int fmt, int maxLevel)
{
    int total = 0;
    int bppBits = SHAPE_bpctobpp_bits(fmt);
    int bpp = 0x10;
    if (bppBits != 0xF) bpp = bppBits;
    if (w < 1 || w > 0x10000 || height < 1) return 0;
    if (height > 0x10000) return 0;
    if (w * height > 1000000000) return 0;
    if (SHAPE_type(bppBits) == 0) return 0;
    int i;
    for (i = 0; i <= maxLevel; i++) {
        int lw = w >> i;
        int lh = height >> i;
        if (lw <= 0) lw = 1;
        if (lh <= 0) lh = 1;
        if (total != 0) total = (total + 0xF) & ~0xF;
        total += ((lw * bpp + 7) / 8) * lh;
        if (lw == 1 && lh == 1) break;
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", GetNumberOfColours);
#ifdef SKIP_ASM
/**
 * Returns the palette size for a palettised bit depth.
 *
 * @param bpp  bits per pixel
 * @return 256 for 8, 16 for 4, otherwise 0
 */
extern "C" int GetNumberOfColours(int bpp)
{
    switch (bpp) {
    case 8:
        return 0x100;
    case 4:
        return 0x10;
    default:
        return 0;
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_createsize);
#ifdef SKIP_ASM
// PORT: the unit declares SHAPE_bpctobpp `void`, but it returns the format's bits per pixel.
extern "C" int SHAPE_bpctobpp_bits(int fmt) __asm__("SHAPE_bpctobpp");
extern "C" int SHAPE_imagesize(int w, int h, int fmt, int maxLevel);

/**
 * Computes the full size of a SHAPE texture: header, pixels, palette and optional comment and metal-bin blocks.
 *
 * Adds a 16-byte header to SHAPE_imagesize(); a palettised format (8 bits or fewer) with a palette depth gets a
 * 16-byte aligned palette with its own 16-byte header (depth 15 counts as 16). A comment adds 8 bytes plus its
 * size, a metal bin 16 bytes plus its size.
 *
 * @param w         width in pixels
 * @param height         height in pixels
 * @param fmt       pixel format code (On Tour: bpc)
 * @param clut      palette entry depth in bits (On Tour: clutbpp), 0 for none
 * @param maxLevel  last mipmap level to include
 * @param extra1    comment size in bytes (On Tour: commentsize), 0 for none
 * @param extra2    metal bin size in bytes (On Tour: metalbinsize), 0 for none
 * @return the total size in bytes
 */
extern "C" int SHAPE_createsize(int w, int height, int fmt, int clut, int maxLevel, int extra1, int extra2)
{
    int bppBits = SHAPE_bpctobpp_bits(fmt);
    int size = SHAPE_imagesize(w, height, fmt, maxLevel);
    size += 0x10;
    if (bppBits < 9 && clut != 0) {
        if (clut == 0xF) clut = 0x10;
        if (size != 0) size = (size + 0xF) & ~0xF;
        int n = GetNumberOfColours(bppBits) * clut / 8 + 0x10;
        size = ((size + 0xF) & ~0xF) + n;
    }
    if (extra1 != 0) {
        int withHeader = size + 8;
        size = withHeader + extra1;
    }
    if (extra2 != 0) {
        int withHeader = size + 0x10;
        size = withHeader + extra2;
    }
    return size;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_createat);
#ifdef SKIP_ASM
extern "C" char SHAPE_type(int key);
extern "C" void MEM_fill(void* dst, int value, int size);
// PORT: memset-like fill called with an unsigned all-ones value here
extern "C" void MEM_fill_u(void* dst, unsigned int value, int size) __asm__("MEM_fill");
extern "C" int SHAPE_imagesize(int w, int h, int fmt, int maxLevel);
extern "C" int GetNumberOfColours(int v);
extern "C" unsigned char SHAPE_cluttype(int clut);

struct sTexHdr_27C8 {
    unsigned type : 8;          // 0x0
    unsigned next : 24;
    short w;                    // 0x4
    short h;                    // 0x6
    int f8;                     // 0x8
    unsigned flags : 28;        // 0xC
    unsigned levels : 4;
    int dataOff;                // 0x10
};

struct sRGBA_27C8 {
    unsigned char r, g, b, a;
};

static inline unsigned char* texData_27C8(sTexHdr_27C8* t)
{
    if (*(int*)((char*)t + 0xC) & 0x1000) {
        return (unsigned char*)t + t->dataOff;
    }
    return (unsigned char*)t + 0x10;
}

/**
 * Lays out a new SHAPE in caller-provided memory: header, then the palette, comment and metal-bin blocks it
 * asks for, linked in a chain.
 *
 * A 32-bit or wider palette starts as a grey ramp; narrower ones are filled with 0xFF.
 *
 * @param tex  memory for the shape, sized by SHAPE_createsize (On Tour SHAPE* shp)
 * @param w  width in pixels
 * @param height  height in pixels
 * @param fmt  pixel format; 0 means 32 bpp (On Tour bpp)
 * @param clut  palette colour depth, 0 for none (On Tour clutbpp)
 * @param levels  number of mipmap levels (On Tour mipmaplevels)
 * @param extra1  comment size, 0 for none (On Tour commentsize)
 * @param extra2  metal-bin size, 0 for none (On Tour metalbinsize)
 */
extern "C" void SHAPE_createat(void* tex, int w, int height, int fmt, int clut, int levels, int extra1, int extra2)
{
    sTexHdr_27C8* texHeader = (sTexHdr_27C8*)tex;
    int bpp = 0x20;
    if (fmt != 0) bpp = fmt;
    char shapeType = SHAPE_type(bpp);
    MEM_fill(texHeader, 0, 0x10);
    texHeader->type = shapeType;
    texHeader->w = w;
    texHeader->h = height;
    texHeader->levels = levels;
    sTexHdr_27C8* hdr = texHeader;
    unsigned char* cursor = (unsigned char*)hdr + (SHAPE_imagesize(w, height, bpp, levels) + 0x10);
    if (bpp < 9 && clut != 0) {
        int n = GetNumberOfColours(bpp);
        unsigned char cfmt = SHAPE_cluttype(clut);
        int off = ((cursor - (unsigned char*)hdr) + 0xF) & ~0xF;
        cursor = (unsigned char*)hdr + off;
        int cbits = 0x10;
        if (clut != 0xF) cbits = clut;
        hdr->next = off;
        MEM_fill(cursor, 0, 0x10);
        ((sTexHdr_27C8*)cursor)->type = cfmt;
        ((sTexHdr_27C8*)cursor)->h = 1;
        ((sTexHdr_27C8*)cursor)->w = n;
        if (clut >= 0x20) {
            sRGBA_27C8* pal = (sRGBA_27C8*)texData_27C8((sTexHdr_27C8*)cursor);
            for (int i = 0; i < n; i++) {
                pal[i].b = i;
                pal[i].g = i;
                pal[i].r = i;
                pal[i].a = 0xFF;
            }
        } else {
            MEM_fill_u(texData_27C8((sTexHdr_27C8*)cursor), 0xFFFFFFFF, n * cbits / 8);
        }
        hdr = (sTexHdr_27C8*)cursor;
        cursor += n * cbits / 8 + 0x10;
    }
    if (extra1 != 0) {
        hdr->next = cursor - (unsigned char*)hdr;
        hdr = (sTexHdr_27C8*)cursor;
        MEM_fill(cursor, 0, extra1 + 8);
        *(int*)(cursor + 4) = extra1;
        cursor[0] = 0x6F;
        cursor += extra1 + 8;
    }
    if (extra2 != 0) {
        hdr->next = cursor - (unsigned char*)hdr;
        MEM_fill(cursor, 0, extra2 + 0x10);
        cursor[0] = 0x69;
        *(short*)(cursor + 6) = 0x10;
        *(int*)(cursor + 0x10) = 0x455355;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", SHAPE_create);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int SHAPE_createsize(int w, int h, int fmt, int clut, int maxLevel, int extra1, int extra2);
extern "C" void SHAPE_createat(void* tex, int w, int h, int fmt, int clut, int maxLevel, int extra1, int extra2);
extern void* (*SHAPE_memallocalign[])(char* name, int size, int a2, int align, int heap);
extern char str_SHPpct_dxpct_dxpct_d[];

/**
 * Allocates and initialises a SHAPE texture.
 *
 * Sizes it with SHAPE_createsize(), allocates it 16-byte aligned through the SHAPE_memallocalign hook under the
 * name "SHP<w>x<h>x<fmt>", then lays it out with SHAPE_createat().
 *
 * @param w         width in pixels
 * @param height         height in pixels
 * @param fmt       pixel format code (On Tour: bpp)
 * @param clut      palette entry depth in bits, 0 for none
 * @param maxLevel  last mipmap level to include
 * @param heap      memory type passed to the allocator (On Tour: memtype)
 * @param extra1    comment size in bytes, 0 for none
 * @param extra2    metal bin size in bytes, 0 for none
 * @return the new texture, or 0 when the size is invalid or the allocation fails
 */
extern "C" void* SHAPE_create(unsigned int w, int height, int fmt, int clut, int maxLevel, int heap, int extra1, int extra2)
{
    char name[16];
    void* tex = 0;
    int size = SHAPE_createsize(w, height, fmt, clut, maxLevel, extra1, extra2);
    if (size != 0) {
        sprintf(name, str_SHPpct_dxpct_dxpct_d, w, height, fmt);
        tex = SHAPE_memallocalign[0](name, size, 0, 0x10, heap);
        if (tex != 0) {
            SHAPE_createat(tex, w, height, fmt, clut, maxLevel, extra1, extra2);
        }
    }
    return tex;
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", func_003B2B68);
#ifdef SKIP_ASM
extern void (*D_0044C458[])(char* buf);

extern "C" void func_003B2B68(char* buf)
{
    buf[0] = 0;
    D_0044C458[0](buf);
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", decodeAlpha);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Decodes one 4x4 alpha block into the alpha bytes of dst: mode 3 is DXT3 (explicit 4-bit alpha),
// anything else is DXT5 (two endpoints + 3-bit indices).
// NOTE: the DXT3 path steps through dst as `unsigned long` (64-bit on the EE), not as 32-bit pixels.
// PORT: callers declare decodeAlpha with char* parameters; defined under an asm label.
extern "C" void decodeAlpha_impl(unsigned long* dst, int stride, unsigned char* src, int mode) __asm__("decodeAlpha");
/**
 * Decodes the alpha of one DXT block into the alpha bytes of its 4x4 texels.
 *
 * Mode 3 (DXT3) has explicit 4-bit alpha; otherwise (DXT5) two endpoints with 6 interpolated levels, or 4
 * plus 0
 * and 255.
 *
 * @param dst  the block's first output texel (On Tour d)
 * @param stride  bytes per output row (On Tour rowbytes)
 * @param src  the compressed alpha block (On Tour blockbuf)
 * @param mode  the DXT type (On Tour dxttype)
 */
extern "C" void decodeAlpha_impl(unsigned long* dst, int stride, unsigned char* src, int mode)
{
    if (mode == 3) {
        for (unsigned int y = 0; y < 4; y++) {
            for (unsigned int x = 0; x < 2; x++) {
                unsigned char alphas = *src++;
                unsigned int lo = alphas & 0xF;
                *dst = (*dst & 0xFFFFFF) | (int)((lo | (lo << 4)) << 24);
                dst++;
                unsigned int hi = alphas & 0xF0;
                *dst = (*dst & 0xFFFFFF) | (int)((hi | (hi >> 4)) << 24);
                dst++;
            }
            dst = (unsigned long*)((char*)dst + stride - 0x20);
        }
        return;
    }
    unsigned char* curPtr = src;
    unsigned char* dstAlpha = (unsigned char*)dst + 3;
    unsigned char alpha0 = *curPtr++;
    unsigned char alpha1 = *curPtr++;
    unsigned char alpha2, alpha3, alpha4, alpha5, alpha6, alpha7;
    if (alpha0 > alpha1) {
        alpha2 = (6 * alpha0 + alpha1 + 3) / 7;
        alpha3 = (5 * alpha0 + 2 * alpha1 + 3) / 7;
        alpha4 = (4 * alpha0 + 3 * alpha1 + 3) / 7;
        alpha5 = (3 * alpha0 + 4 * alpha1 + 3) / 7;
        alpha6 = (2 * alpha0 + 5 * alpha1 + 3) / 7;
        alpha7 = (alpha0 + 6 * alpha1 + 3) / 7;
    } else {
        alpha2 = (4 * alpha0 + alpha1 + 2) / 5;
        alpha3 = (3 * alpha0 + 2 * alpha1 + 2) / 5;
        alpha4 = (2 * alpha0 + 3 * alpha1 + 2) / 5;
        alpha5 = (alpha0 + 4 * alpha1 + 2) / 5;
        alpha6 = 0;
        alpha7 = 0xFF;
    }
    for (int i = 0; i < 2; i++) {
        unsigned long bits = *(unsigned long*)curPtr;
        curPtr += 3;
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 4; x++) {
                unsigned char idx = bits & 7;
                bits >>= 3;
                switch (idx) {
                case 0: *dstAlpha = alpha0; break;
                case 1: *dstAlpha = alpha1; break;
                case 2: *dstAlpha = alpha2; break;
                case 3: *dstAlpha = alpha3; break;
                case 4: *dstAlpha = alpha4; break;
                case 5: *dstAlpha = alpha5; break;
                case 6: *dstAlpha = alpha6; break;
                case 7: *dstAlpha = alpha7; break;
                }
                dstAlpha += 4;
            }
            dstAlpha += stride - 0x10;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", decodeRGB);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` palette entries (`long` is 8 bytes on this compiler).
static inline unsigned short dxtTo1555_2E78(unsigned short c)
{
    if (c != 0) {
        // the alpha bit is a sign-extended short constant (forces the final 16-bit mask)
        int g = ((c >> 1) & 0x3E0) | (short)0x8000;
        c = ((c & 0x1F) << 10) | g | (c >> 11);
    }
    return c;
}

/**
 * Decodes the colour part of one DXT block into 4 rows of 4 texels.
 *
 * Builds the four-colour palette from the two 565 endpoints (5:3 and 3:5 blends, or their average and
 * transparent
 * black when the first is not larger) and converts it with dxtTo1555. Despite the names, @p src is the output
 * and
 * @p dst the compressed block.
 *
 * @param src  the block's first output texel (On Tour d)
 * @param pitch  bytes per output row (On Tour rowbytes)
 * @param dst  the compressed colour block (On Tour blockbuf)
 */
// Decode one DXT1 block into 4 rows of 4 64-bit texels.
extern "C" void decodeRGB(char* src, int pitch, char* dst)
{
    ulong pal[4];
    ulong endpoints = *(ulong*)dst;
    ulong color0 = ((endpoints & 0x1F) << 3) | ((endpoints & 0x7E0) << 5) | ((endpoints & 0xF800) << 8);
    ulong color1 = ((endpoints >> 13) & 0xF8) | ((endpoints >> 11) & 0xFC00) | ((endpoints & 0xF8000000) >> 8);
    pal[0] = color0 | 0x40204;
    pal[1] = color1 | 0x40204;
    if (color0 >= color1) {
        int blue0 = color0 & 0xFF0000;
        int blue1 = color1 & 0xFF0000;
        int green0 = color0 & 0xFF00;
        int green1 = color1 & 0xFF00;
        int red0 = color0 & 0xFF;
        int red1 = color1 & 0xFF;
        pal[2] = ((((blue0 * 5 + blue1 * 3) >> 3) & 0xFF0000) | (((green0 * 5 + green1 * 3) >> 3) & 0xFF00)) | (((red0 * 5 + red1 * 3) >> 3) & 0xFF);
        pal[3] = (((((blue0 * 3 + blue1 * 5) >> 3) & 0xFF0000) | (((green0 * 3 + green1 * 5) >> 3) & 0xFF00)) | (((red0 * 3 + red1 * 5) >> 3) & 0xFF)) | 0xFF000000UL;
    } else {
        int blue0 = color0 & 0xFF0000;
        int blue1 = color1 & 0xFF0000;
        int green0 = color0 & 0xFF00;
        int green1 = color1 & 0xFF00;
        int red0 = color0 & 0xFF;
        int red1 = color1 & 0xFF;
        pal[2] = ((ulong)((red0 + red1) | ((green0 + green1) | (blue0 + blue1))) >> 1) | 0xFF000000UL;
        pal[3] = 0;
    }
    pal[0] |= 0xFF000000;
    pal[1] |= 0xFF000000;
    pal[2] |= 0xFF000000;
    pal[0] = dxtTo1555_2E78(pal[0]);
    pal[1] = dxtTo1555_2E78(pal[1]);
    pal[2] = dxtTo1555_2E78(pal[2]);
    pal[3] = dxtTo1555_2E78(pal[3]);
    unsigned int rowIndices;
    rowIndices = ((unsigned char*)dst)[4];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[5];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[6];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
    src += pitch;
    rowIndices = ((unsigned char*)dst)[7];
    ((ulong*)src)[0] = pal[rowIndices & 3];
    ((ulong*)src)[1] = pal[(rowIndices >> 2) & 3];
    ((ulong*)src)[2] = pal[(rowIndices >> 4) & 3];
    ((ulong*)src)[3] = pal[rowIndices >> 6];
}
#endif

//100%
INCLUDE_ASM("ealib/seg_2AEDC0", hasAlpha);
#ifdef SKIP_ASM
/**
 * Tells whether DXT-compressed data has any alpha that is not fully opaque.
 *
 * Checks @p size / 16 blocks. Type 3 steps 16 bytes and needs both 64-bit words of each block to equal
 * 0xFFFFFFFF; any other type steps 32 bytes and needs the first 16-bit word to be 0xFFFF.
 *
 * @param buf   the compressed data
 * @param size  number of pixels (blocks are counted as size / 16)
 * @param type  DXT type (On Tour: dxttype)
 * @return 1 at the first block that fails the test, 0 when all pass
 */
// PORT: 64-bit `long` compares (ld); `long` is 8 bytes on this compiler.
extern "C" int hasAlpha(void* buf, int size, int type)
{
    if (type == 3) {
        unsigned long* pixel32 = (unsigned long*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            unsigned long first = *pixel32++;
            if (first != 0xFFFFFFFF) {
                return 1;
            }
            if (*pixel32 != first) {
                return 1;
            }
            pixel32 += 3;
        }
    } else {
        unsigned short* pixel16 = (unsigned short*)buf;
        int n = size / 16;
        int i;
        for (i = 0; i < n; i++) {
            if (*pixel16 != 0xFFFF) {
                return 1;
            }
            pixel16 += 16;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ealib/seg_2AEDC0", undxtpack);
#ifdef SKIP_ASM
extern "C" int hasAlpha(void* buf, int size, int type);
extern "C" void decodeRGB(char* src, int pitch, char* dst);
extern "C" void decodeAlpha(char* src, int pitch, char* dst, int fmt);

/**
 * Decompresses a DXT-compressed image into pixels, one 4x4 block at a time.
 *
 * Types 3 and 5 use 16-byte blocks: decodeRGB() decodes the colour half (+8), and decodeAlpha() the alpha half
 * only when hasAlpha() finds non-opaque alpha. Other types use 8-byte colour-only blocks.
 *
 * @param out  the compressed data (On Tour: compressed); despite the name it is the input
 * @param src  the destination pixels (On Tour: uncompressed); despite the name it is the output
 * @param w    width in pixels
 * @param height    height in pixels
 * @param fmt  DXT type (On Tour: dxttype)
 */
// Compress a w x h image into 4x4 blocks.
extern "C" void undxtpack(void* out, char* src, int w, int height, int fmt)
{
    char* dst = (char*)out;
    int x, y;
    int n = w * height;
    if (fmt == 3 || fmt == 5) {
        if (hasAlpha(dst, n, fmt)) {
            for (y = 0; y < height; y += 4) {
                for (x = 0; x < w; x += 4) {
                    decodeRGB(src, w * 8, dst + 8);
                    decodeAlpha(src, w * 8, dst, fmt);
                    src += 0x20;
                    dst += 0x10;
                }
                src += w * 0x18;
            }
        } else {
            for (y = 0; y < height; y += 4) {
                for (x = 0; x < w; x += 4) {
                    decodeRGB(src, w * 8, dst + 8);
                    src += 0x20;
                    dst += 0x10;
                }
                src += w * 0x18;
            }
        }
    } else {
        for (y = 0; y < height; y += 4) {
            for (x = 0; x < w; x += 4) {
                decodeRGB(src, w * 4, dst);
                src += 0x20;
                dst += 8;
            }
            src += w * 0x18;
        }
    }
}
#endif

