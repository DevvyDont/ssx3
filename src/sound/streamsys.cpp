#include "common.h"

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_cStreamInstance);
#ifdef SKIP_ASM
struct sSsEnt_99D8 { char vol; char pad[0x17]; };
struct sSsPlayer_99D8 {
    char pad0[0x1C];
    volatile int cur;           // 0x1C
    sSsEnt_99D8* ents;          // 0x20
    char pad24[0x38 - 0x24];
    int* vols;                  // 0x38
    float** scales;             // 0x3C
};
struct sStreamInst_99D8 {
    char* mem;          // 0x0
    int f4;             // 0x4
    int handle;         // 0x8
    char vol;           // 0xC
    float* scale;       // 0x10
    char pad14[0x74 - 0x14];
    int f74;            // 0x74
    int f78;            // 0x78
    int heap;           // 0x7C
    int f80;            // 0x80
};
extern "C" int func_003B7818(void* desc, int n, int m, void* mem, int size);
extern "C" int func_003B7B00(int n, int m);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_Stream[];

static inline int curVol_99D8(sSsPlayer_99D8* p) { return p->vols[p->cur]; }

#define MIN_99D8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_99D8(a, b) ((a) >= (b) ? (a) : (b))

/**
 * Constructs a sound stream instance from play options: applies the volume scale and opens the stream.
 *
 * Scales the options' current volume by their volume-scale pointer (clamped to 0-127), allocates the stream
 * buffer ("Stream": the library's size for 8 requests of 300 plus @p extra) and opens the stream into it.
 *
 * @param p  play options (On Tour cSndPlayOpts* PlayOpts)
 * @param heap  memory type for the buffer (On Tour MemType)
 * @param extra  extra buffer space in bytes (On Tour BuffSpace)
 * @return @p self
 */
extern "C" sStreamInst_99D8* cStreamInstance_cStreamInstance(sStreamInst_99D8* self, sSsPlayer_99D8* p, int heap, int extra)
{
    self->f74 = 0;
    self->handle = -1;
    self->f78 = 0;
    self->heap = heap;
    self->f80 = 0;
    self->scale = p->scales[p->cur];
    self->vol = curVol_99D8(p);
    if (self->scale) {
        p->vols[p->cur] = (int)MAX_99D8(MIN_99D8((float)p->vols[p->cur] * *self->scale, 127.0f), 0.0f);
        p->ents[p->cur].vol = MAX_99D8(MIN_99D8(p->vols[p->cur], 127), 0);
    }
    int n = func_003B7B00(8, 300) + extra;
    self->mem = new (str_Stream, heap, 0) char[n];
    self->f4 = 0;
    self->handle = func_003B7818(&p->ents[p->cur], 8, 300, self->mem, n);
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9C00);
#ifdef SKIP_ASM
extern "C" sStreamInst_99D8* func_002A9C00(sStreamInst_99D8* self, sSsPlayer_99D8* p, char* mem, int size, int n, int m)
{
    self->scale = p->scales[p->cur];
    self->vol = curVol_99D8(p);
    if (self->scale) {
        p->vols[p->cur] = (int)MAX_99D8(MIN_99D8((float)p->vols[p->cur] * *self->scale, 127.0f), 0.0f);
        p->ents[p->cur].vol = MAX_99D8(MIN_99D8(p->vols[p->cur], 127), 0);
    }
    self->mem = mem;
    self->f4 = 1;
    self->handle = func_003B7818(&p->ents[p->cur], n, m, mem, size);
    return self;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002A9DD8);
#ifdef SKIP_ASM
extern "C" void *func_002A9DD8(void *stream, int handle) {
    (*(int *)((char*)(stream) + (8))) = handle;
    (*(int *)((char*)(stream) + (0))) = 0;
    (*(int *)((char*)(stream) + (0x74))) = 0;
    (*(int *)((char*)(stream) + (0x78))) = 0;
    return stream;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9DF0);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" int func_003B7838(int h);
extern "C" void* MEM_free(void* self);

extern "C" void func_002A9DF0(void* self, int flags)
{
    if (*(void**)self != 0) {
        func_003B7838(*(int*)((char*)self + 0x8));
        if (*(int*)((char*)self + 0x4) == 0 && *(void**)self != 0) {
            cMemMan_free(*(void**)self);
        }
    }
    if (*(void**)((char*)self + 0x74) != 0) {
        MEM_free(*(void**)((char*)self + 0x74));
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9E78);
#ifdef SKIP_ASM
extern "C" int BXFILE_exists(const char* name);
extern "C" int func_003E1EC8(const char* name, int a1);

// PORT: second parameter is a const char* passed as int (unit declares func_002A9E78(void*, int)).
extern "C" void func_002A9E78(void* self, int name)
{
    if (BXFILE_exists((const char*)name) != 0) {
        strcpy((char*)self + 0x14, (const char*)name);
        *(int*)((char*)self + 0x74) = func_003E1EC8((char*)self + 0x14, *(int*)((char*)self + 0x7C));
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9ED8);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7F60(int h, int a, int b);

extern "C" void func_002A9ED8(void* self, int a, int b)
{
    func_003B58A0();
    func_003B7F60(*(int*)((char*)self + 0x8), a, b);
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_SetBaseVolume__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a stream instance's base volume.
 *
 * Stores the low byte of @p val at +0xC.
 *
 * @param val  the volume, 0..127 (On Tour Vol)
 */
void cStreamInstance_SetBaseVolume(void* self, int val)
{
    *(char*)((char*)self + 0xC) = (char)val;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9F38);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7FB8(int h, int v);

extern "C" void func_002A9F38(void* self, int pitch)
{
    func_003B58A0();
    func_003B7FB8(*(int*)((char*)self + 0x8), pitch);
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002A9F80);
#ifdef SKIP_ASM
int func_0028B240(void* self);
void* cStreamInstance_Purge(void* self);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B7E70(int h, int vol);

extern "C" void func_002A9F80(void* self)
{
    if (func_0028B240(self) * 2 < *(int*)((char*)self + 0x80)) {
        cStreamInstance_Purge(self);
        *(int*)((char*)self + 0x80) = 0;
        return;
    }
    float* volumeScale = *(float**)((char*)self + 0x10);
    if (volumeScale != 0) {
        int volume = (int)((float)*(signed char*)((char*)self + 0xC) * *volumeScale);
        if (volume > 0x7F) {
            volume = 0x7F;
        }
        if (volume < 0) {
            volume = 0;
        }
        func_003B58A0();
        func_003B7E70(*(int*)((char*)self + 0x8), volume);
        func_003B58D8();
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA020);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7B48(int h, void* out);

// PORT: returns int; the unit declares it void(void*, void*) further down
int func_002AA020_impl(void* self, void* out) __asm__("func_002AA020");

int func_002AA020_impl(void* self, void* out)
{
    int result;
    func_003B58A0();
    result = func_003B7B48(*(int*)((char*)self + 0x8), out);
    func_003B58D8();
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA068);
#ifdef SKIP_ASM
extern "C" void func_002AA020(void*, void*);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B7C40(int h, int a1);

// PORT: returns int and takes (self, int, out); later callers in this unit declare it void(void*, sStatus*, int)
int func_002AA068_impl(void* self, int a1, int* out) __asm__("func_002AA068");

int func_002AA068_impl(void* self, int a1, int* out)
{
    int buf[4];
    int* status = out ? out : buf;
    int result;
    func_002AA020(self, status);
    func_003B58A0();
    result = func_003B7C40(status[1], a1);
    func_003B58D8();
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA0C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002AA0C0_sStatus {
    int state;
    int a;
    int b;
    int c;
};

// the unit declares func_002AA068 later with a different pointer type; alias it
void func_002AA068_status(void* self, func_002AA0C0_sStatus* out, int flags) __asm__("func_002AA068");

extern "C" float func_002AA0C0(void* self)
{
    func_002AA0C0_sStatus st;
    func_002AA068_status(self, &st, 0);
    if (st.state == 2) {
        return (float)st.a;
    }
    return 0.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA108);
#ifdef SKIP_ASM
struct func_002AA1B8_sStatus;
extern "C" void func_002AA068(void* self, func_002AA1B8_sStatus* out, int flags);

struct func_002AA108_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" float func_002AA108(void* self)
{
    func_002AA108_sStatus st;
    func_002AA068(self, (func_002AA1B8_sStatus*)&st, 0);
    if (st.state == 2) {
        return (float)st.b;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA150);
#ifdef SKIP_ASM
struct func_002AA150_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" int func_002AA150(void* self, int minVal)
{
    func_002AA150_sStatus st;
    func_002AA020(self, &st);
    return st.b >= minVal;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AA188);
#ifdef SKIP_ASM
struct func_002AA188_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" int func_002AA188(void* self)
{
    func_002AA188_sStatus st;
    func_002AA068(self, (func_002AA1B8_sStatus*)&st, 0);
    return st.state == 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA1B8);
#ifdef SKIP_ASM
struct func_002AA1B8_sStatus {
    int state;
    int a;
    int b;
    int c;
};

extern "C" void func_002AA068(void* self, func_002AA1B8_sStatus* out, int flags);

extern "C" int func_002AA1B8(void* self)
{
    func_002AA1B8_sStatus st;
    func_002AA068(self, &st, 0);
    return st.state == 2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA1E8);
#ifdef SKIP_ASM
extern "C" void func_003B7D80(int, int);

extern "C" void func_002AA1E8(void* self)
{
    int buf[2];
    func_002AA020(self, buf);
    func_003B7D80(buf[1], 0);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA210);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// func_002AA020 returns the status (the unit declares it void); the int return keeps $2 free as the target has it.
int func_002AA020_st(void* self, int* out) __asm__("func_002AA020");

// Returns the stream's second status word, or -1 when the stream is not active.
extern "C" int func_002AA210(void* self)
{
    int st[2];
    func_002AA020_st(self, st);
    if (st[0] == 0) {
        return -1;
    }
    return st[1];
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA240);
#ifdef SKIP_ASM
extern "C" void func_003B7D80(int, int);

extern "C" void func_002AA240(void* self, int requestHandle)
{
    func_003B7D80(requestHandle, 0);
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AA260);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct func_002AA260_sStatus {
    int state;
    int a;
    int b;
    int c;
};

void func_002AA068_status(void* self, func_002AA260_sStatus* out, int flags) __asm__("func_002AA068");

extern "C" float func_002AA260(void* self)
{
    func_002AA260_sStatus st;
    func_002AA068_status(self, &st, 0);
    if (st.state == 2) {
        return (float)(st.a + st.b);
    }
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AA2B0);
#ifdef SKIP_ASM
extern "C" void func_002AA2B0(void *stream) {
    func_003B58A0();
    func_003B7FB8((*(int *)((char*)(stream) + (8))), 0);
    func_003B58D8();
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AA2E8);
#ifdef SKIP_ASM
extern "C" void func_002AA2E8(void *stream) {
    func_003B58A0();
    func_003B7FB8((*(int *)((char*)(stream) + (8))), 0x1000);
    func_003B58D8();
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA320);
#ifdef SKIP_ASM
extern "C" int func_003B78D8(int h, int a, int buf, int off);
extern "C" void func_003E2CE0(void* dec, int src, int a, int* out, int b);

// PORT: buf is a pointer carried in an int (the unit's caller declares this function with int params).
extern "C" int func_002AA320(void* self, int holdTime, int buf, int off)
{
    if (*(void**)((char*)self + 0x74) != 0) {
        int n;
        func_003E2CE0(*(void**)((char*)self + 0x74), buf, 0, &n, 0);
        return func_003B78D8(*(int*)((char*)self + 0x8), holdTime, (int)((char*)self + 0x14), off + n);
    }
    return func_003B78D8(*(int*)((char*)self + 0x8), holdTime, buf, off);
}
#endif

extern "C" void* func_003B78F8(int);

//100%
INCLUDE_ASM("sound/streamsys", func_002AA398);
#ifdef SKIP_ASM
extern "C" int func_002AA398(void* self, int holdTime, int b, int off)
{
    void* dec = *(void**)((char*)self + 0x74);
    if (dec != 0) {
        int n;
        func_003E2CE0(dec, 0, b, &n, 0);
        return func_003B78D8(*(int*)((char*)self + 0x8), holdTime, (int)((char*)self + 0x14), off + n);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamInstance_Purge__FPv);
#ifdef SKIP_ASM
/**
 * Purges (stops and releases) a stream instance's stream.
 *
 * Calls func_003B78F8 (On Tour SNDSTRM_purge) on the stream handle at +0x8 and returns its result.
 */
void* cStreamInstance_Purge(void* self)
{
    return func_003B78F8(*(int*)((char*)self + 0x8));
}
#endif

extern "C" void func_002AA020(void*, void*);

//99.38% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("sound/streamsys", func_002AA428);
#ifdef SKIP_ASM
extern "C" int func_002AA428(void* self)
{
    int buf[4];
    func_002AA020(self, buf);
    return buf[0];
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA448);
#ifdef SKIP_ASM
extern "C" int func_003B80E0(int, int);

extern "C" int func_002AA448(void *stream, int arg1) {
    int result;

    func_003B58A0();
    result = func_003B80E0((*(int *)((char*)(stream) + (8))), arg1);
    func_003B58D8();
    return result;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cStreamSys_cStreamSys);
#ifdef SKIP_ASM
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_StreamArray[];
extern void* cBankMonitor_vtbl[];

struct sSsVEntryA490 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAA490 {
    sSsVEntryA490 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBA490 {
    sSsVEntryA490 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAA490 D_00483998;
extern const sSsVtblBA490 D_004839B8;

struct sStreamSysA490 {
    char* vbase;        // 0x0
    int count;          // 0x4
    void** items;       // 0x8
    char vb2[0x8];      // 0xC
    char vb[0x4];       // 0x14
};

/**
 * Constructs the stream system part of the audio system and allocates its stream table.
 *
 * With @p inchrg set it first builds the shared virtual bases (cSoundSys, cBankManager); otherwise it patches
 * stack copies of the base vtables with this object's offset. The table ("StreamArray") starts empty.
 *
 * @param inchrg  g++ in-charge flag: nonzero when the virtual bases are built here
 * @param banks  maximum number of banks (On Tour MaxBanks)
 * @param count  maximum number of streams (On Tour MaxStreams)
 * @param heap  audio mode (eSOUNDDEF_MODE), passed to the base constructors
 * @return @p self
 */
extern "C" sStreamSysA490* cStreamSys_cStreamSys(sStreamSysA490* self, int inchrg, int banks, int count, int heap)
{
    if (inchrg) {
        self->vbase = self->vb;
        *(char**)(self->vb + 0x1D8) = self->vb2;
        cSoundSys_cSoundSys(self->vb2, heap);
        cBankManager_cBankManager(self->vbase, 0, banks, heap);
    }
    *(const sSsVtblAA490**)(*(char**)(self->vbase + 0x1D8) + 4) = &D_00483998;
    *(void***)(self->vbase + 0xAB0) = cBankMonitor_vtbl;
    *(const sSsVtblBA490**)(self->vbase + 0x1D4) = &D_004839B8;
    if (!inchrg) {
        // PORT: g++ 2.95 virtual-base this-adjust fix-ups (copied vtables on the stack), written out by hand.
        sSsVtblAA490 vtCopyA = D_00483998;
        *(sSsVtblAA490**)(*(char**)(self->vbase + 0x1D8) + 4) = &vtCopyA;
        char* base1 = *(char**)(self->vbase + 0x1D8) - 0xC;
        int adjustA = (char*)self - base1;
        vtCopyA.e[1].delta = D_00483998.e[1].delta + adjustA;
        sSsVtblBA490 vtCopyB = D_004839B8;
        *(sSsVtblBA490**)(self->vbase + 0x1D4) = &vtCopyB;
        char* base2 = self->vbase - 0x14;
        int adjustB = (char*)self - base2;
        vtCopyB.e[1].delta = D_004839B8.e[1].delta + adjustB;
    }
    self->count = count;
    self->items = new (str_StreamArray, 0, 0) void*[count];
    for (int i = 0; i < self->count; i++)
        self->items[i] = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA648);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void func_002A9DF0(void* e, int a1);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern void* cBankMonitor_vtbl[];

struct sSsVEntryA648 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAA648 {
    sSsVEntryA648 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBA648 {
    sSsVEntryA648 e[7];
} __attribute__((aligned(8)));
// Views of the vtables cStreamSys_cStreamSys (earlier in this unit) declares with its own types.
extern const sSsVtblAA648 D_00483998_A648 __asm__("D_00483998");
extern const sSsVtblBA648 D_004839B8_A648 __asm__("D_004839B8");

// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void func_002AA648(void* p, int flags)
{
    char* self = (char*)p;
    *(const sSsVtblAA648**)(*(char**)(*(char**)self + 0x1D8) + 4) = &D_00483998_A648;
    *(void***)(*(char**)self + 0xAB0) = cBankMonitor_vtbl;
    *(const sSsVtblBA648**)(*(char**)self + 0x1D4) = &D_004839B8_A648;
    if (flags == 0) {
        sSsVtblAA648 t1 = D_00483998_A648;
        *(sSsVtblAA648**)(*(char**)(*(char**)self + 0x1D8) + 4) = &t1;
        char* base1 = *(char**)(*(char**)self + 0x1D8) - 0xC;
        int d1 = self - base1;
        t1.e[1].delta = D_00483998_A648.e[1].delta + d1;
        sSsVtblBA648 t2 = D_004839B8_A648;
        *(sSsVtblBA648**)(*(char**)self + 0x1D4) = &t2;
        char* base2 = *(char**)self - 0x14;
        int d2 = self - base2;
        t2.e[1].delta = D_004839B8_A648.e[1].delta + d2;
    }
    for (int i = 0; i < *(int*)(self + 4); i++) {
        void* e = (*(void***)(self + 8))[i];
        if (e != 0) {
            func_002A9DF0(e, 3);
        }
    }
    if (*(void**)(self + 8) != 0) {
        cMemMan_free(*(void**)(self + 8));
    }
    if (flags & 2) {
        cBankManager__cBankManager(*(void**)self, 0);
        cSoundSys__cSoundSys(*(void**)(*(char**)self + 0x1D8), 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA7F0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_StreamBuffer[];

extern "C" void func_002AA7F0(void* self, int idx, int heap, int extra)
{
    sStreamInst_99D8** slot = (sStreamInst_99D8**)((idx << 2) + *(int*)((char*)self + 0x8));
    void* mem = cMemMan_alloc(132, str_StreamBuffer, heap, 0);
    *slot = cStreamInstance_cStreamInstance((sStreamInst_99D8*)mem, (sSsPlayer_99D8*)(*(char**)self + 0x1D8), heap, extra);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AA870);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern char str_StreamBuffer[];
extern "C" sStreamInst_99D8* func_002A9C00(sStreamInst_99D8* self, sSsPlayer_99D8* p, char* mem, int size, int n, int m);

extern "C" void func_002AA870(void* self, int idx, int heap, char* mem, int size, int n, int m)
{
    sStreamInst_99D8** slot = (sStreamInst_99D8**)((idx << 2) + *(int*)((char*)self + 0x8));
    void* obj = cMemMan_alloc(132, str_StreamBuffer, heap, 0);
    *slot = func_002A9C00((sStreamInst_99D8*)obj, (sSsPlayer_99D8*)(*(char**)self + 0x1D8), mem, size, n, m);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AA910);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void func_002AA7F0(void* self, int a1, int a2, int a3);

struct sEntry_A910 {
    signed char kind;
    char pad[0x17];
};
struct sQuad_A910 {
    int a;
    int b;
    int c;
    int d;
};
struct sStack_A910 {
    int field_0x0;                  // 0x1D8
    sEntry_A910 cur;                // 0x1DC
    volatile int depth;             // 0x1F4
    sEntry_A910* entries;           // 0x1F8
    int flags[4];                   // 0x1FC
    int* a34;
    int* a38;
    int* a3C;
    char a40[4][8];
    int* a60;
    int* a64;
    int* a68;
    int* a6C;
    sQuad_A910* a70;
    int* a74;
    int* a78;
    int* a7C;
    int* a80;
    int* a84;
};
struct sMgr_A910 {
    char pad_0x0[0x1D8];
    sStack_A910 st;                 // 0x1D8
};

extern "C" void func_002AA910(sMgr_A910** self, int a1, int a2, int a3, int a4)
{
    sMgr_A910* m = *self;
    sStack_A910* st = &m->st;
    m->st.depth++;
    st->entries[m->st.depth] = m->st.cur;
    m->st.flags[m->st.depth] = 0;
    st->a34[m->st.depth] = 0;
    st->a38[m->st.depth] = st->entries[m->st.depth].kind;
    st->a3C[m->st.depth] = 0;
    func_00416210((char*)st + m->st.depth * 8 + 0x40, 0, 8);
    st->a60[m->st.depth] = 0;
    st->a64[m->st.depth] = 0;
    st->a68[m->st.depth] = 0;
    st->a6C[m->st.depth] = 0;
    st->a70[m->st.depth].b = 0;
    st->a70[m->st.depth].a = 100;
    st->a70[m->st.depth].c = 90;
    st->a70[m->st.depth].d = 50;
    st->a74[m->st.depth] = 0;
    st->a80[m->st.depth] = 0;
    st->a84[m->st.depth] = 0;
    st->a78[m->st.depth] = 127;
    st->a7C[m->st.depth] = 1;
    (*self)->st.a3C[(*self)->st.depth] = a2;
    func_002AA7F0(self, a1, a3, a4);
    (*self)->st.depth--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAB98);
#ifdef SKIP_ASM
extern "C" void func_002A9E78(void*, int);

extern "C" void func_002AAB98(void* self, int i, int val)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        func_002A9E78(stream, val);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AABD0);
#ifdef SKIP_ASM
extern "C" void func_002A9DF0(void* e, int a1);

extern "C" void func_002AABD0(void* self, int i)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        func_002A9DF0(stream, 3);
    }
    (*(void***)((char*)self + 0x8))[i] = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAC28);
#ifdef SKIP_ASM
extern "C" void func_002A9F80(void* e);

extern "C" void func_002AAC28(void* self)
{
    for (int i = 0; i < *(int*)((char*)self + 0x4); i++) {
        void* stream = (*(void***)((char*)self + 0x8))[i];
        if (stream != 0) {
            func_002A9F80(stream);
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AAC98);
#ifdef SKIP_ASM
extern "C" void func_002AA2B0(void* p);

extern "C" void func_002AAC98(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        void* stream = *(void**)((char*)*(void**)((char*)self + 0x8) + i * 4);
        if (stream != 0) {
            func_002AA2B0(stream);
        }
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AAD08);
#ifdef SKIP_ASM
extern "C" void func_002AA2E8(void* p);

extern "C" void func_002AAD08(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        void* stream = *(void**)((char*)*(void**)((char*)self + 0x8) + i * 4);
        if (stream != 0) {
            func_002AA2E8(stream);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAD78);
#ifdef SKIP_ASM
extern "C" int func_002AA320(void* e, int a, int b, int c);

extern "C" int func_002AAD78(void* self, int i, int holdTime, int buf, int offset)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        return func_002AA320(stream, holdTime, buf, offset);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AADC0);
#ifdef SKIP_ASM
extern "C" int func_002AA398(void*, int, int, int);

extern "C" int func_002AADC0(void* self, int idx, int holdTime, int arg3, int offset)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    if (stream != 0) {
        return func_002AA398(stream, holdTime, arg3, offset);
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAE08);
#ifdef SKIP_ASM
extern "C" int func_002AA428(void*);

extern "C" int func_002AAE08(void* self, int i)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        return func_002AA428(stream);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AAE40);
#ifdef SKIP_ASM
extern "C" void func_002AAE40(void* self, int i)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        cStreamInstance_Purge(stream);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAE78);
#ifdef SKIP_ASM
extern "C" float func_002AA108(void* self);

extern "C" float func_002AAE78(void* self, int i)
{
    void* stream;
    if (i < 0 || (stream = (*(void***)((char*)self + 0x8))[i]) == 0) {
        return 0.0f;
    }
    return func_002AA108(stream);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AAEC0);
#ifdef SKIP_ASM
extern "C" float func_002AA260(void*);

extern "C" float func_002AAEC0(void* self, int idx)
{
    void* stream;
    if (idx < 0 || (stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4)) == 0) {
        return 0.0f;
    }
    return func_002AA260(stream);
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AAF08);
#ifdef SKIP_ASM
extern "C" int func_002AA150(void*, int);

extern "C" int func_002AAF08(void* self, int idx, int minVal)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    return stream != 0 ? func_002AA150(stream, minVal) : 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AAF40);
#ifdef SKIP_ASM
extern "C" int func_002AA188(void*);

extern "C" int func_002AAF40(void* self, int idx)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    return stream != 0 ? func_002AA188(stream) : 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AAF78);
#ifdef SKIP_ASM
// PORT: the unit declares func_003B7C40(int, int); this call passes an out buffer
int func_003B7C40_pbuf(int h, int* out) __asm__("func_003B7C40");

extern "C" int func_002AAF78(void* self, int handle)
{
    int st[4];
    int result;
    func_003B58A0();
    result = func_003B7C40_pbuf(handle, st);
    func_003B58D8();
    return st[0] == 3 || result < 0;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AAFD0);
#ifdef SKIP_ASM
// PORT: the unit declares func_003B7C40(int, int); this call passes an out buffer
int func_003B7C40_pbuf(int h, int* out) __asm__("func_003B7C40");

extern "C" int func_002AAFD0(void* self, int handle)
{
    int st[4];
    int result;
    func_003B58A0();
    result = func_003B7C40_pbuf(handle, st);
    func_003B58D8();
    return st[0] == 2 && !(result < 0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB028);
#ifdef SKIP_ASM
extern "C" int func_002AA1B8(void*);

extern "C" int func_002AB028(void* self, int i)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        return func_002AA1B8(stream);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB060);
#ifdef SKIP_ASM
extern "C" void func_002AA1E8(void*);

extern "C" void func_002AB060(void* self, int idx)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    if (stream != 0) {
        func_002AA1E8(stream);
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AB098);
#ifdef SKIP_ASM
extern "C" void func_002AA240(void*, int);

extern "C" void func_002AB098(void* self, int idx, int requestHandle)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    if (stream != 0) {
        func_002AA240(stream, requestHandle);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB0D0);
#ifdef SKIP_ASM
extern "C" void func_002AB0D0(void* self, int i, int val)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        *(int*)((char*)stream + 0x10) = val;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB0F0);
#ifdef SKIP_ASM
extern "C" void func_002AB0F0(void* self, int idx, int val)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    if (stream != 0) {
        cStreamInstance_SetBaseVolume(stream, val);
    }
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AB128);
#ifdef SKIP_ASM
extern "C" int func_002AB128(void* self, int idx)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    if (stream != 0) {
        return *(int*)((char*)stream + 0x8);
    }
    return -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB150);
#ifdef SKIP_ASM
extern "C" int func_002AA210(void*);

extern "C" int func_002AB150(void* self, int i)
{
    void* stream = (*(void***)((char*)self + 0x8))[i];
    if (stream != 0) {
        return func_002AA210(stream);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB188__FPvi);
#ifdef SKIP_ASM
int func_002AB188(void* self, int index)
{
    return *(int*)((char*)*(void**)((char*)self + 0x8) + index * 4);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB1A0);
#ifdef SKIP_ASM
extern "C" void func_003B7D80(int, int);

extern "C" int func_002AB1A0(void* self, void* arg1)
{
    func_003B7D80(*(int*)((char*)arg1 + 0x1C), 0);
    return 1;
}
#endif
//100%
INCLUDE_ASM("sound/streamsys", func_002AB1C8);
#ifdef SKIP_ASM
extern "C" int func_002AA448(void*, int);

extern "C" int func_002AB1C8(void* self, int idx, int arg2)
{
    void* stream = *(void**)(*(char**)((char*)self + 0x8) + idx * 4);
    return stream != 0 ? func_002AA448(stream, arg2) : -1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB200);
#ifdef SKIP_ASM
extern "C" void* cSoundSys_cSoundSys(void* self, int heap);
extern "C" void* cBankManager_cBankManager(void* self, int embedded, int count, int heap);
extern "C" void* cCrowdSys_cCrowdSys(void* self, int embedded, int a2, int a3, int heap);
extern "C" void* func_002B28C0(void* self, int embedded, int a2, int a3, int heap);
extern "C" void* cSpeechSys_cSpeechSys(void* self, int embedded, int a2, int a3, int heap);
extern void* D_004838F8[];
extern void* D_004837F8[];
extern void* D_00483870[];

struct sSsVEntryB200 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAB200 {
    sSsVEntryB200 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBB200 {
    sSsVEntryB200 e[8];
} __attribute__((aligned(8)));
struct sSsVtblCB200 {
    sSsVEntryB200 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAB200 D_004837D8_B200 __asm__("D_004837D8");
extern const sSsVtblBB200 D_00483760_B200 __asm__("D_00483760");
extern const sSsVtblCB200 D_004837A0_B200 __asm__("D_004837A0");

// PORT: g++ 2.95 virtual-base construction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" char* func_002AB200(char* self, int embedded, int a2, int a3, int heap)
{
    if (embedded != 0) {
        *(char**)(self + 0x5560) = self + 0x6200;
        *(char**)(self + 0x6200) = self + 0x5730;
        *(char**)(self + 0x0) = self + 0x5730;
        *(char**)(self + 0x5908) = self + 0x5728;
        *(char**)(self + 0x118) = self + 0x6200;
        cSoundSys_cSoundSys(self + 0x5728, heap);
        cBankManager_cBankManager(*(void**)(self + 0x6200), 0, a2, heap);
        cStreamSys_cStreamSys(*(sStreamSysA490**)(self + 0x5560), 0, a2, a3, heap);
    }
    cCrowdSys_cCrowdSys(self, 0, a2, a3, heap);
    func_002B28C0(self + 0x118, 0, a2, a3, heap);
    cSpeechSys_cSpeechSys(self + 0x5560, 0, a2, a3, heap);
    *(const sSsVtblAB200**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &D_004837D8_B200;
    *(const sSsVtblBB200**)(**(char***)(self + 0x118) + 0xAB0) = &D_00483760_B200;
    *(const sSsVtblCB200**)(**(char***)(self + 0x118) + 0x1D4) = &D_004837A0_B200;
    if (embedded == 0) {
        sSsVtblAB200 t1 = D_004837D8_B200;
        *(sSsVtblAB200**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &t1;
        char* base1 = *(char**)(**(char***)(self + 0x118) + 0x1D8) - 0x5728;
        int d1 = self - base1;
        t1.e[1].delta = D_004837D8_B200.e[1].delta + d1;
        sSsVtblBB200 t2 = D_00483760_B200;
        *(sSsVtblBB200**)(**(char***)(self + 0x118) + 0xAB0) = &t2;
        char* base2 = **(char***)(self + 0x118) - 0x5730;
        int d2 = self - base2;
        t2.e[1].delta = D_00483760_B200.e[1].delta + d2;
        t2.e[5].delta = D_00483760_B200.e[5].delta + d2;
        sSsVtblCB200 t3 = D_004837A0_B200;
        *(sSsVtblCB200**)(**(char***)(self + 0x118) + 0x1D4) = &t3;
        t3.e[1].delta = D_004837A0_B200.e[1].delta + d2;
    }
    *(void***)(self + 0x5558) = D_004838F8;
    *(void***)(self + 0x571C) = D_00483870;
    *(void***)(self + 0xC) = D_004837F8;
    *(int*)(self + 0x5720) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cController__cController);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBankManager__cBankManager(void* self, int flags);
extern "C" void cCrowdSys__cCrowdSys(void* self, int flags);
extern "C" void func_002AA648(void* self, int flags);
extern "C" void cSoundSys__cSoundSys(void* self, int flags);
extern "C" void cSpeechSys__cSpeechSys(char* self, int flags);
extern "C" void func_002B3398(char* self, int flags);
extern void* D_004838F8[];
extern void* D_004837F8[];
extern void* D_00483870[];

struct sSsVEntryB478 {
    short delta;
    short index;
    void* fn;
};
struct sSsVtblAB478 {
    sSsVEntryB478 e[4];
} __attribute__((aligned(8)));
struct sSsVtblBB478 {
    sSsVEntryB478 e[8];
} __attribute__((aligned(8)));
struct sSsVtblCB478 {
    sSsVEntryB478 e[7];
} __attribute__((aligned(8)));
extern const sSsVtblAB478 D_004837D8;
extern const sSsVtblBB478 D_00483760;
extern const sSsVtblCB478 D_004837A0;

/**
 * Destroys the sound controller and its bank, stream and monitor sub-systems.
 *
 * Hand-written g++ 2.95 virtual-base destruction: installs the cController vtables at +0x5558, +0x571C and +0xC and in
 * the shared sound object (+0x118), then runs the sub-object destructors (cSpeechSys__cSpeechSys, func_002B3398, cCrowdSys__cCrowdSys,
 * func_002AA648, cBankManager__cBankManager, cSoundSys__cSoundSys) with this-adjusted temporary vtables, and deletes the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ 2.95 in-charge flags; bit 0 deletes the object
 */
// PORT: g++ 2.95 virtual-base destruction with stack vtable this-adjust fix-ups, written out by hand.
extern "C" void cController__cController(char* self, int flags)
{
    char* mon = self + 0x118;
    char* bank = self + 0x5560;
    *(void***)(self + 0x5558) = D_004838F8;
    *(void***)(self + 0x571C) = D_00483870;
    *(void***)(self + 0xC) = D_004837F8;
    *(const sSsVtblAB478**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &D_004837D8;
    *(const sSsVtblBB478**)(**(char***)(self + 0x118) + 0xAB0) = &D_00483760;
    *(const sSsVtblCB478**)(**(char***)(self + 0x118) + 0x1D4) = &D_004837A0;
    if (flags == 0) {
        sSsVtblAB478 t1 = D_004837D8;
        *(sSsVtblAB478**)(*(char**)(**(char***)(self + 0x118) + 0x1D8) + 4) = &t1;
        char* base1 = *(char**)(**(char***)(self + 0x118) + 0x1D8) - 0x5728;
        int d1 = self - base1;
        t1.e[1].delta = D_004837D8.e[1].delta + d1;
        sSsVtblBB478 t2 = D_00483760;
        *(sSsVtblBB478**)(**(char***)(self + 0x118) + 0xAB0) = &t2;
        char* base2 = **(char***)(self + 0x118) - 0x5730;
        int d2 = self - base2;
        t2.e[1].delta = D_00483760.e[1].delta + d2;
        t2.e[5].delta = D_00483760.e[5].delta + d2;
        sSsVtblCB478 t3 = D_004837A0;
        *(sSsVtblCB478**)(**(char***)(self + 0x118) + 0x1D4) = &t3;
        t3.e[1].delta = D_004837A0.e[1].delta + d2;
    }
    cSpeechSys__cSpeechSys(bank, 0);
    func_002B3398(mon, 0);
    cCrowdSys__cCrowdSys(self, 0);
    if (flags & 2) {
        func_002AA648(*(void**)(self + 0x118), 0);
        cBankManager__cBankManager(**(void***)(self + 0x118), 0);
        cSoundSys__cSoundSys(*(void**)(**(char***)(self + 0x118) + 0x1D8), 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Update);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_Update3D(void* voice, void* bm, float v);

class cStrmCbOwner {
public:
    virtual void dummy();
};

struct sStrmVoiceCb {
    char pad[0xA8];
    void (cStrmCbOwner::*cb)(void* voice, float v);
};

/**
 * Updates one bank-monitor instance for a frame.
 *
 * Only when the instance is not free (cBankMonitorInstance_IsFree) and @p time is positive: calls the member-function callback at +0xA8
 * on the controller (if set), then cBankMonitorInstance_Update3D with the controller's bank monitor (its sound object +0x1D8). Note the
 * parameter named self is the controller, not this.
 *
 * @param self  the controller (On Tour cController* Monitor)
 * @param time  elapsed time (On Tour Time)
 */
extern "C" void cBankMonitorInstance_Update(void* voice, void* self, float time)
{
    if (cBankMonitorInstance_IsFree(voice) == 0 && time > 0.0f) {
        sStrmVoiceCb* sv = (sStrmVoiceCb*)voice;
        if (sv->cb) {
            (((cStrmCbOwner*)self)->*(sv->cb))(voice, time);
        }
        char* bm = 0;
        if (self) {
            char* snd = **(char***)((char*)self + 0x118);
            if (snd) {
                bm = snd;
                bm += 0x1D8;
            }
        }
        cBankMonitorInstance_Update3D(voice, bm, time);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cController_BANKMONITOR_Update);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_Update(void* voice, void* self, float v);

/**
 * Updates all 64 bank-monitor instances.
 *
 * Uses a time of 0 when the bank monitor's pause flag (+0xAC0 of the sound object) is set, then calls cBankMonitorInstance_Update for
 * each instance in the array at +0xAC4.
 *
 * @param time  elapsed time (On Tour Time)
 */
extern "C" void cController_BANKMONITOR_Update(void* self, float time)
{
    if (*(int*)(**(char***)((char*)self + 0x118) + 0xAC0) != 0) {
        time = 0.0f;
    }
    for (int i = 0; i < 64; i++) {
        cBankMonitorInstance_Update(*(char**)(**(char***)((char*)self + 0x118) + 0xAC4) + i * 0xC0, self, time);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB828);
#ifdef SKIP_ASM
extern "C" void cMonitorInstance_Reset(void* e);

class cStrmTickOwner {
public:
    virtual void dummy();
};

struct sStrmTick {
    int active;
    int period;
    int count;
    int (cStrmTickOwner::*cb)(sStrmTick* e);
};

extern "C" void func_002AB828(void* entry, void* self)
{
    sStrmTick* tick = (sStrmTick*)entry;
    if (tick->active) {
        if ((tick->count -= 10) < 5) {
            if ((((cStrmTickOwner*)self)->*(tick->cb))(tick)) {
                cMonitorInstance_Reset(tick);
            } else {
                tick->count += tick->period;
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB8E8);
#ifdef SKIP_ASM
extern "C" void cMonitor_MONITOR_Update(void* bm);
extern "C" void func_002AB828(void* e, void* self);

extern "C" void func_002AB8E8(void* self)
{
    char* snd = **(char***)((char*)self + 0x118);
    char* bm = snd + 0x1D8;
    if (*(int*)(snd + 0x26C) == 0) {
        cMonitor_MONITOR_Update(bm);
        for (int i = 0; i < 48; i++) {
            func_002AB828(**(char***)((char*)self + 0x118) + 0x270 + i * 44, self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002AB958);
#ifdef SKIP_ASM
extern "C" void func_0028AAF8(void* snd);
extern "C" void func_002A6D18(void* self);
extern "C" void func_002AAC28(void* self);
extern "C" void func_002B0C78(void* self);
extern "C" void func_002B4388(void* self);

struct sStrmVtEntF { short delta; short index; void (*fn)(void*, float); };

extern "C" void func_002AB958(void* self, float v)
{
    char** sys = (char**)((char*)self + 0x118);
    func_0028AAF8(*(void**)*sys);
    func_002A6D18(self);
    char* bankMan = *(char**)*sys;
    char* bankMon = bankMan + 0x1D8;
    sStrmVtEntF* entry = &(*(sStrmVtEntF**)(bankMan + 0xAB0))[5];
    entry->fn(bankMon + entry->delta, v);
    func_002B4388(sys);
    func_002B0C78((char*)self + 0x5560);
    func_002AAC28(*sys);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AB9E0);
#ifdef SKIP_ASM
extern "C" void* func_002AB9E0(void* self)
{
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(float*)((char*)self + 0x90) = -1.0f;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0xB8) = -30000;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABA18);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_002ABA18(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_DonePlaying);
#ifdef SKIP_ASM
extern "C" int func_003B8AB8(int h);
extern "C" int func_003B9FC8(int h);
// PORT: the unit declares func_003B7C40(int, int); its second argument is a status buffer here.
int func_003B7C40_st(int h, int* st) __asm__("func_003B7C40");

/**
 * Returns whether the instance's sound has finished playing.
 *
 * By the type at +0: 1 (sample) asks func_003B8AB8 under the sound critical section; 2 (sequence) checks func_003B9FC8
 * for status 2 or an error; 3 (stream) reads the request status (func_003B7C40) and reports done when it is 3 or cannot
 * be read. Any other type counts as done.
 *
 * @return nonzero when done
 */
extern "C" int cBankMonitorInstance_DonePlaying(void* self)
{
    int type = *(int*)self;
    switch (type) {
    case 1: {
        func_003B58A0();
        int r = func_003B8AB8(*(int*)((char*)self + 4)) != 0;
        func_003B58D8();
        return r;
    }
    case 2: {
        func_003B58A0();
        int r = func_003B9FC8(*(int*)((char*)self + 4));
        func_003B58D8();
        return r == 2 || r < 0;
    }
    case 3: {
        int st[4];
        func_003B58A0();
        if (func_003B7C40_st(*(int*)((char*)self + 4), st) >= 0) {
            func_003B58D8();
            return st[0] == 3;
        }
        func_003B58D8();
        return 1;
    }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Reset);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_StopSound(void*);

/**
 * Stops an instance's sound and returns it to the free state.
 *
 * Calls cBankMonitorInstance_StopSound, then sets the cached pitch (+0xB0) to 0x8000, the cached volume (+0xB4) to -1, the cached
 * position (+0xB8) to 0xFFFF and clears the type (+0), the stopping flag (+0x54) and +0x58.
 */
extern "C" void cBankMonitorInstance_Reset(void* self)
{
    cBankMonitorInstance_StopSound(self);
    *(int*)((char*)self + 0xB0) = 0x8000;
    *(int*)((char*)self + 0xB4) = -1;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0xB8) = 0xFFFF;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_IsFree);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_DonePlaying(void* self);
extern "C" void cBankMonitorInstance_Reset(void* self);

/**
 * Returns whether an instance is free, releasing it if its sound has finished.
 *
 * An instance with no type (+0) is free. One without the auto-release flag (+0x9C) is never free. Otherwise it is free
 * once cBankMonitorInstance_DonePlaying says the sound is done, after cBankMonitorInstance_Reset resets it.
 *
 * @return nonzero when free
 */
extern "C" int cBankMonitorInstance_IsFree(void* self)
{
    if (*(int*)self == 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x9C) == 0) {
        return 0;
    }
    if (cBankMonitorInstance_DonePlaying(self) == 0) {
        return 0;
    }
    cBankMonitorInstance_Reset(self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABBD8);
#ifdef SKIP_ASM
extern "C" float func_002ABBD8(void* self, float* posA, float* posB)
{
    float dx = posA[0] - posB[0];
    float dy = posA[1] - posB[1];
    float dz = posA[2] - posB[2];
    return dx * dx + dy * dy + dz * dz;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", ExpScale);
#ifdef SKIP_ASM
// PORT: the unit declares ExpScale as void*(int); the body takes (int, float, float).
int ExpScale_impl(int vol, float dist, float range) __asm__("ExpScale");

/**
 * Attenuates a volume by distance with a squared fall-off.
 *
 * The distance is reduced by 0.5 (not below 0); within @p range the result is vol * ((range - dist) / range)^2, beyond
 * it (or for a non-positive range) 0.
 *
 * @param vol  the full volume (On Tour Vol)
 * @param dist  the distance (On Tour Dist)
 * @param range  the distance at which the sound vanishes (On Tour Vanish)
 * @return the attenuated volume
 */
int ExpScale_impl(int vol, float dist, float range)
{
    dist = dist - 0.5f;
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    int result = 0;
    if (dist < range && 0.0f < range) {
        float t = (range - dist) / range;
        result = (int)(t * t * (float)vol);
    }
    return result;
}
#endif

extern "C" void* ExpScale(int);

//99.29%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Dist2Vol__FPvi);
#ifdef SKIP_ASM
/**
 * Converts a distance to a volume for this instance (forwards to ExpScale).
 *
 * The C declaration shows only the volume; the distance and vanish range pass through in $f12/$f13.
 *
 * @param vol  the full volume (On Tour Vol)
 * @return the attenuated volume
 */
void* cBankMonitorInstance_Dist2Vol(void* self, int vol)
{
    return ExpScale(vol);
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Doppler);
#ifdef SKIP_ASM
/**
 * Returns the Doppler pitch factor for a change in distance over a time step.
 *
 * The speed @p delta / @p time is clamped to +-894.08 and mapped to (speed + 8000) / 8000. The object pointer is not used, so
 * the C definition omits it.
 *
 * @param delta  change in distance (On Tour Delta)
 * @param time  time step (On Tour Time)
 * @return the pitch factor (1.0 for no motion)
 */
extern "C" float cBankMonitorInstance_Doppler(float delta, float time)
{
    float f = delta / time;
    if (f < -894.0799560546875f) {
        f = -894.0799560546875f;
    } else if (894.0799560546875f < f) {
        f = 894.0799560546875f;
    }
    return (f + 8000.0f) * 0.0001250000059371814f;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABCE8__FPvii);
#ifdef SKIP_ASM
void func_002ABCE8(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABCF8);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);

extern "C" void func_002ABCF8(void* self, int a, const char* name, int b)
{
    *(int*)((char*)self + 0xC) = a;
    strcpy((char*)self + 0x10, name);
    *(int*)((char*)self + 0x50) = b;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABD38__FPvii);
#ifdef SKIP_ASM
void func_002ABD38(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0xc) = a1;
    *(int*)((char*)self + 0x10) = a2;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002ABD48);
#ifdef SKIP_ASM
class func_002ABD48_cObj {
public:
    char pad[0x8D8];
    // vptr at 0x8D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float v06(void* a, void* out);
};

extern "C" void func_002ABD48(void* self, func_002ABD48_cObj* obj)
{
    char buf[0x20];
    float d;
    float d2 = obj->v06(self, buf);
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    *(float*)((char*)self + 0x5C) = d;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_PosUpdate);
#ifdef SKIP_ASM
extern "C" void func_002A9ED8(void* s, int pos, int a2);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B8C00(int h, int pos, int a2);
extern "C" void func_003B9C20(int h, int pos);

/**
 * Updates the 3D position (azimuth, elevation) of an instance's sound.
 *
 * Does nothing when the azimuth differs from the cached one (+0xB8) by less than 0x200; otherwise caches it and, by
 * type, calls func_003B8C00 (sample), func_003B9C20 (sequence) or func_002A9ED8 (stream at +0xC).
 *
 * @param pos  the azimuth (On Tour Azimuth)
 * @param elevation  the elevation (On Tour Elevation)
 */
extern "C" void cBankMonitorInstance_PosUpdate(void* self, int pos, int elevation)
{
    int d = pos - *(int*)((char*)self + 0xB8);
    if (d >= -0x1FF && d <= 0x1FF) {
        return;
    }
    *(int*)((char*)self + 0xB8) = pos;
    switch (*(int*)self) {
    case 1:
        func_003B58A0();
        func_003B8C00(*(int*)((char*)self + 0x4), pos, elevation);
        func_003B58D8();
        break;
    case 2:
        func_003B58A0();
        func_003B9C20(*(int*)((char*)self + 0x4), pos);
        func_003B58D8();
        break;
    case 3:
        func_002A9ED8(*(void**)((char*)self + 0xC), pos, elevation);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_PitchUpdate);
#ifdef SKIP_ASM
extern "C" void func_002A9F38(void* s, int vol);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B8790(int h, int vol);
extern "C" void func_003B9B68(int h, int vol);

/**
 * Sets the pitch multiplier of an instance's sound.
 *
 * The value is clamped to 0..0x3FFF (0x1000 = normal pitch) and ignored when within 0xFF of the cached pitch at +0xB0;
 * otherwise it is cached and applied by type: func_003B8790 (sample), func_003B9B68 (sequence) or func_002A9F38 (stream
 * at +0xC). Note the parameter is named vol but is a pitch.
 *
 * @param vol  the pitch multiplier, 4.12 fixed point (On Tour Pitch)
 */
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int vol)
{
    if (vol > 0x3FFF) {
        vol = 0x3FFF;
    }
    if (vol < 0) {
        vol = 0;
    }
    int d = vol - *(int*)((char*)self + 0xB0);
    if (d >= -0xFF && d <= 0xFF) {
        return;
    }
    *(int*)((char*)self + 0xB0) = vol;
    switch (*(int*)self) {
    case 1:
        func_003B58A0();
        func_003B8790(*(int*)((char*)self + 0x4), vol);
        func_003B58D8();
        break;
    case 2:
        func_003B58A0();
        func_003B9B68(*(int*)((char*)self + 0x4), vol);
        func_003B58D8();
        break;
    case 3:
        func_002A9F38(*(void**)((char*)self + 0xC), vol);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_VolUpdate);
#ifdef SKIP_ASM
void cStreamInstance_SetBaseVolume(void* self, int val);
void* cStreamInstance_Purge(void* self);
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B85F0(int h, int a);
extern "C" void func_003B89E0(int h);
extern "C" void func_003B9920(int h);
extern "C" void func_003B9E48(int h, int a);

/**
 * Sets the volume of an instance's sound, or stops it for -1.
 *
 * The volume is scaled by the factor at +0x90 when that is non-negative, clamped to 0..127 and ignored when within 4 of
 * the cached volume at +0xB4. A requested -1 stops the sound instead: func_003B89E0 (sample), func_003B9920 (sequence)
 * or cStreamInstance_Purge__FPv (stream); other values go to func_003B85F0, func_003B9E48 or cStreamInstance_SetBaseVolume__FPvi.
 *
 * @param vol  the volume 0..127, or -1 to stop (On Tour Vol)
 */
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int vol)
{
    int orig = vol;
    switch (*(int*)self) {
    case 1: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int d = vol - *(int*)((char*)self + 0xB4);
            if (d >= -4 && d <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        func_003B58A0();
        if (orig == none)
            func_003B89E0(*(int*)((char*)self + 0x4));
        else
            func_003B85F0(*(int*)((char*)self + 0x4), vol);
        func_003B58D8();
        break;
    }
    case 2: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int d = vol - *(int*)((char*)self + 0xB4);
            if (d >= -4 && d <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        func_003B58A0();
        if (orig == none)
            func_003B9920(*(int*)((char*)self + 0x4));
        else
            func_003B9E48(*(int*)((char*)self + 0x4), vol);
        func_003B58D8();
        break;
    }
    case 3: {
        if (*(float*)((char*)self + 0x90) >= 0.0f)
            vol = (int)(*(float*)((char*)self + 0x90) * (float)vol);
        if (vol > 127)
            vol = 127;
        int none = -1;
        if (vol <= none)
            vol = 0;
        if (*(int*)((char*)self + 0xB4) >= 0) {
            int d = vol - *(int*)((char*)self + 0xB4);
            if (d >= -4 && d <= 4)
                return;
        }
        *(int*)((char*)self + 0xB4) = vol;
        if (orig == none)
            cStreamInstance_Purge(*(void**)((char*)self + 0xC));
        else
            cStreamInstance_SetBaseVolume(*(void**)((char*)self + 0xC), vol);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_StopSound);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B8530(int h, int a, int b);
extern "C" void func_003B9F38(int h, int a, int b);

/**
 * Starts fading an instance's sound out.
 *
 * Does nothing when already stopping (+0x54). Otherwise clears +0x8, sets +0x54 and starts an automatic volume ramp
 * (0x19, -1) on the sample (func_003B8530) or sequence (func_003B9F38).
 */
extern "C" void cBankMonitorInstance_StopSound(void* self)
{
    if (*(int*)((char*)self + 0x54) != 0) {
        return;
    }
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x54) = 1;
    switch (*(int*)self) {
    case 1:
        func_003B58A0();
        func_003B8530(*(int*)((char*)self + 0x4), 25, -1);
        func_003B58D8();
        break;
    case 2:
        func_003B58A0();
        func_003B9F38(*(int*)((char*)self + 0x4), 25, -1);
        func_003B58D8();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", func_002AC220);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" int func_003B6820(int a, int b, void* desc);
extern "C" int func_003B9A00(void* desc, int a, int b, int c);

struct sSndDesc_C220 {
    char c[0x18];
};

struct sSndEnv_C220 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sSndPlayOpts_C220 {
    char* vbase;                // 0x00
    sSndDesc_C220 cur;          // 0x04
    volatile int depth;         // 0x1C
    sSndDesc_C220* descs;       // 0x20
    int f24[4];                 // 0x24
    int* f34;                   // 0x34
    int* f38;                   // 0x38
    int* f3C;                   // 0x3C
    char f40[4][8];             // 0x40
    int* f60;                   // 0x60
    int* f64;                   // 0x64
    int* f68;                   // 0x68
    int* f6C;                   // 0x6C
    sSndEnv_C220* env;          // 0x70
    int* f74;                   // 0x74
    int* f78;                   // 0x78
    int* f7C;                   // 0x7C
    int* f80;                   // 0x80
    int* f84;                   // 0x84
};

#define C220_MIN(a, b) ((a) < (b) ? (a) : (b))
#define C220_MAX(a, b) ((a) < (b) ? (b) : (a))

// PORT: the unit declares func_002AC220(void*) later; the real function also takes the play options
extern "C" void func_002AC220_impl(void* self, sSndPlayOpts_C220* opts) __asm__("func_002AC220");

extern "C" void func_002AC220_impl(void* self, sSndPlayOpts_C220* opts)
{
    *(int*)((char*)self + 0x8) = 1;
    *(int*)((char*)self + 0x54) = 0;
    switch (*(int*)self) {
    case 1:
        opts->depth++;
        opts->descs[opts->depth] = opts->cur;
        opts->f24[opts->depth] = 0;
        opts->f34[opts->depth] = 0;
        opts->f38[opts->depth] = opts->descs[opts->depth].c[0];
        opts->f3C[opts->depth] = 0;
        func_00416210((char*)opts + opts->depth * 8 + 0x40, 0, 8);
        opts->f60[opts->depth] = 0;
        opts->f64[opts->depth] = 0;
        opts->f68[opts->depth] = 0;
        opts->f6C[opts->depth] = 0;
        opts->env[opts->depth].f4 = 0;
        opts->env[opts->depth].f0 = 100;
        opts->env[opts->depth].f8 = 90;
        opts->env[opts->depth].fC = 50;
        opts->f74[opts->depth] = 0;
        opts->f80[opts->depth] = 0;
        opts->f84[opts->depth] = 0;
        opts->f78[opts->depth] = 127;
        opts->f7C[opts->depth] = 1;
        opts->f3C[opts->depth] = *(int*)((char*)self + 0x60);
        opts->f38[opts->depth] = 0;
        opts->descs[opts->depth].c[0] = C220_MAX(C220_MIN(opts->f38[opts->depth], 127), 0);
        func_003B58A0();
        *(int*)((char*)self + 0x4) = func_003B6820(*(int*)((char*)self + 0xC), *(int*)((char*)self + 0x10), &opts->descs[opts->depth]);
        func_003B58D8();
        opts->depth--;
        break;
    case 2:
        opts->depth++;
        opts->descs[opts->depth] = opts->cur;
        opts->f24[opts->depth] = 0;
        opts->f34[opts->depth] = 0;
        opts->f38[opts->depth] = opts->descs[opts->depth].c[0];
        opts->f3C[opts->depth] = 0;
        func_00416210((char*)opts + opts->depth * 8 + 0x40, 0, 8);
        opts->f60[opts->depth] = 0;
        opts->f64[opts->depth] = 0;
        opts->f68[opts->depth] = 0;
        opts->f6C[opts->depth] = 0;
        opts->env[opts->depth].f4 = 0;
        opts->env[opts->depth].f0 = 100;
        opts->env[opts->depth].f8 = 90;
        opts->env[opts->depth].fC = 50;
        opts->f74[opts->depth] = 0;
        opts->f80[opts->depth] = 0;
        opts->f84[opts->depth] = 0;
        opts->f78[opts->depth] = 127;
        opts->f7C[opts->depth] = 1;
        opts->f3C[opts->depth] = *(int*)((char*)self + 0x60);
        opts->f38[opts->depth] = 0;
        opts->descs[opts->depth].c[0] = C220_MAX(C220_MIN(opts->f38[opts->depth], 127), 0);
        func_003B58A0();
        *(int*)((char*)self + 0x4) = func_003B9A00(&opts->descs[opts->depth], *(int*)((char*)self + 0xC), *(int*)((char*)self + 0x10), -1);
        func_003B58D8();
        opts->depth--;
        break;
    case 3:
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_CheckActive);
#ifdef SKIP_ASM
extern "C" void cBankMonitorInstance_StopSound(void*);
extern "C" void func_002AC220(void*);

/**
 * Starts or stops an instance's sound as it crosses its audible distance.
 *
 * With no limit (+0x8C == 0) the sound is always active. A playing sound (+0x8 set) farther than the limit is stopped
 * (cBankMonitorInstance_StopSound, returns 0); a silent one within it is restarted (func_002AC220, returns 1).
 *
 * @param radius  the current distance (On Tour Radius)
 * @return nonzero while the sound should play
 */
extern "C" int cBankMonitorInstance_CheckActive(void* self, float radius)
{
    float th = *(float*)((char*)self + 0x8C);
    if (th == 0.0f) {
        return 1;
    }
    if (*(int*)((char*)self + 0x8) != 0) {
        if (th < radius) {
            cBankMonitorInstance_StopSound(self);
            return 0;
        }
        return 1;
    }
    if (radius < th) {
        func_002AC220(self);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Update3D);
#ifdef SKIP_ASM
class cStreamBM_C868 {
public:
    char pad[0x8D8];
    // vptr at 0x8D8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float v06(void* a, void* out);
};
struct sPos_C868 {
    int a;
    int b;
    unsigned short pos;     // 0x8
    short pos2;             // 0xA
    int c[5];
};
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
// PORT: the unit declares func_003B7C40(int, int); its second argument is a status buffer here.
int func_003B7C40_st(int h, int* st) __asm__("func_003B7C40");
extern "C" int cBankMonitorInstance_DonePlaying(void* self);
extern "C" void cBankMonitorInstance_PosUpdate(void* self, int pos, int a2);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int a1);
// PORT: these callees are declared with fewer parameters elsewhere in the unit; the caller passes the extra registers.
int cBankMonitorInstance_Dist2Vol_4(void* self, int vol, float dist, float range) __asm__("cBankMonitorInstance_Dist2Vol__FPvi");
float cBankMonitorInstance_Doppler_3(void* self, float a, float b) __asm__("cBankMonitorInstance_Doppler");
int cBankMonitorInstance_CheckActive_3(void* self, void* bm, float t) __asm__("cBankMonitorInstance_CheckActive");

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_C868(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Updates the 3D audio of an instance: Doppler pitch, position and distance volume.
 *
 * Streams whose request has not started are skipped. For a positioned instance (+0x68) when the monitor has 3D enabled
 * (+0x8E4), the distance is the square root of the monitor's virtual slot 6; the sound is started/stopped by
 * cBankMonitorInstance_CheckActive, and while active the Doppler factor (cBankMonitorInstance_Doppler, from the previous distance at +0x5C, when +0x8E0
 * enables it), azimuth/elevation (cBankMonitorInstance_PosUpdate), pitch (cBankMonitorInstance_PitchUpdate, factor * 4096) and volume (cBankMonitorInstance_Dist2Vol__FPvi over
 * the range at +0x88, default 30, rescaled by 127 / the maximum at +0x98) are applied. Otherwise the fixed volume at
 * +0x64 is used. The volume is multiplied by the optional factor at +0x60 and set with cBankMonitorInstance_VolUpdate; a stopping
 * instance whose sound is done (cBankMonitorInstance_DonePlaying) is cleared instead.
 *
 * @param bm  the bank monitor (On Tour Monitor)
 * @param time  elapsed time (On Tour Time)
 */
extern "C" void cBankMonitorInstance_Update3D(void* voice, void* bm, float time)
{
    char* s = (char*)voice;
    char* b = (char*)bm;
    if (*(int*)s == 0) {
        return;
    }
    int st[4];
    sPos_C868 pos;
    int vol = 0;
    if (*(int*)s == 3) {
        func_003B58A0();
        int r = func_003B7C40_st(*(int*)(s + 0x4), st);
        func_003B58D8();
        if (r <= 0 && st[0] != 2) {
            return;
        }
    }
    if (*(int*)(b + 0x8E4) != 0 && *(int*)(s + 0x68) != 0) {
        float d = Sqrt_C868(((cStreamBM_C868*)bm)->v06(voice, &pos));
        if (*(int*)(s + 0x94) != 0 || cBankMonitorInstance_CheckActive_3(voice, bm, d) != 0) {
            float f = *(float*)(s + 0x5C);
            if (f != -1.0f && *(int*)(b + 0x8E0) != 0) {
                time = cBankMonitorInstance_Doppler_3(voice, f - d, time);
            } else {
                time = 1.0f;
            }
            cBankMonitorInstance_PosUpdate(voice, pos.pos, pos.pos2);
            cBankMonitorInstance_PitchUpdate(voice, (int)(time * 4096.0f));
            if (*(int*)(s + 0x94) != 0) {
                vol = *(int*)(s + 0x64);
            } else {
                float range = *(float*)(s + 0x88);
                if (range == 0.0f) {
                    vol = cBankMonitorInstance_Dist2Vol_4(voice, *(int*)(s + 0x64), d, 30.0f);
                } else {
                    vol = cBankMonitorInstance_Dist2Vol_4(voice, *(int*)(s + 0x64), d, range);
                }
                int m = *(int*)(s + 0x98);
                if (m != 0x7F) {
                    vol = vol * 0x7F / m;
                    if (vol > 0x7F) {
                        vol = 0x7F;
                    }
                    if (vol < 0) {
                        vol = 0;
                    }
                }
            }
        }
        if (vol > 0x7F) {
            vol = 0x7F;
        }
        if (vol < 0) {
            vol = 0;
        }
        *(float*)(s + 0x5C) = d;
    } else {
        vol = *(int*)(s + 0x64);
    }
    if (*(int*)(s + 0x54) == 0) {
        float* p = *(float**)(s + 0x60);
        if (p != 0) {
            vol = (int)((float)vol * *p);
        }
        cBankMonitorInstance_VolUpdate(voice, vol);
        return;
    }
    if (cBankMonitorInstance_DonePlaying(voice) != 0) {
        *(int*)(s + 0x8) = 0;
        *(int*)(s + 0x54) = 0;
        *(int*)(s + 0x4) = -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Pause);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void cBankMonitorInstance_VolUpdate(void* self, int a1);
extern "C" void func_003B9D00(int h);

/**
 * Pauses an instance's sound.
 *
 * For a non-free instance that is not stopping: sets pitch and volume to 0 and, for a sequence (type 2), pauses it with
 * func_003B9D00.
 */
extern "C" void cBankMonitorInstance_Pause(void* self)
{
    if (cBankMonitorInstance_IsFree(self) == 0 && *(int*)((char*)self + 0x54) == 0) {
        cBankMonitorInstance_PitchUpdate(self, 0);
        cBankMonitorInstance_VolUpdate(self, 0);
        if (*(int*)((char*)self + 0x0) == 2) {
            func_003B9D00(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/streamsys", func_002ACB30);
#ifdef SKIP_ASM
extern "C" int cBankMonitorInstance_IsFree(void* self);
extern "C" void cBankMonitorInstance_PitchUpdate(void* self, int a1);
extern "C" void func_003B9DC8(int h);

extern "C" void func_002ACB30(void* self)
{
    if (cBankMonitorInstance_IsFree(self) == 0) {
        cBankMonitorInstance_PitchUpdate(self, 0x1000);
        if (*(int*)self == 2) {
            func_003B9DC8(*(int*)((char*)self + 0x4));
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/streamsys", cBankMonitorInstance_Fade);
#ifdef SKIP_ASM
extern "C" void func_003B58A0();
extern "C" void func_003B58D8();
extern "C" void func_003B8530(int h, int a, int b);
extern "C" void func_003B85F0(int h, int a);
extern "C" void func_003B9E48(int h, int a);
extern "C" void func_003B9F38(int h, int a, int b);
void* cStreamInstance_Purge(void* self);
void cStreamInstance_SetBaseVolume(void* self, int val);

/**
 * Fades an instance's sound to a volume level over a duration, or out and stopped for level -1.
 *
 * When @p resetMonitor is set the instance is flagged for release (+0x58, +0x9C). A stopping instance is left alone. Level -1
 * marks it stopping; other levels are clamped to 0..127. Samples and sequences ramp over duration * 100 ticks
 * (func_003B8530, func_003B9F38) or jump at once when the duration is 0 (func_003B85F0, func_003B9E48); streams are
 * purged when stopping or get the level as base volume.
 *
 * @param n  the target volume 0..127, or -1 to stop (On Tour Level)
 * @param resetMonitor  nonzero to release the instance afterwards (On Tour ResetMonitor)
 * @param duration  the fade time in seconds (On Tour Duration)
 */
extern "C" void cBankMonitorInstance_Fade(void* self, int n, int resetMonitor, float duration)
{
    if (resetMonitor) {
        *(int*)((char*)self + 0x58) = 1;
        *(int*)((char*)self + 0x9C) = 1;
    }
    if (*(int*)((char*)self + 0x54) != 0) {
        return;
    }
    if (n == -1) {
        *(int*)((char*)self + 0x54) = 1;
    } else {
        if (n > 127) {
            n = 127;
        }
        if (n < 0) {
            n = 0;
        }
    }
    switch (*(int*)self) {
    case 1:
        func_003B58A0();
        if (duration != 0.0f) {
            func_003B8530(*(int*)((char*)self + 0x4), (int)(duration * 100.0f), n);
        } else {
            func_003B85F0(*(int*)((char*)self + 0x4), n);
        }
        func_003B58D8();
        break;
    case 2:
        func_003B58A0();
        if (duration != 0.0f) {
            func_003B9F38(*(int*)((char*)self + 0x4), (int)(duration * 100.0f), n);
        } else {
            func_003B9E48(*(int*)((char*)self + 0x4), n);
        }
        func_003B58D8();
        break;
    case 3:
        if (*(int*)((char*)self + 0x54) != 0) {
            cStreamInstance_Purge(*(void**)((char*)self + 0xC));
        } else {
            cStreamInstance_SetBaseVolume(*(void**)((char*)self + 0xC), n);
        }
        break;
    }
}
#endif

