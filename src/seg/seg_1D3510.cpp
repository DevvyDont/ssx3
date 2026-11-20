#include "common.h"
#include "util/bxps2memcard.h"

//100%
INCLUDE_ASM("seg/seg_1D3510", get_float);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int get_float_2510(unsigned char **pp, float *out) __asm__("get_float");
/**
 * Reads a little-endian 32-bit float from a byte stream and advances the cursor.
 *
 * @param pp   pointer to the read cursor, moved forward 4 bytes
 * @param out  receives the float
 * @return 4, the number of bytes read
 */
extern "C" int get_float_2510(unsigned char **pp, float *out) {
    unsigned char *bytes = *pp;
    union { unsigned int u; float f; } x;
    x.u = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
    *out = x.f;
    *pp += 4;
    return 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", get_uint);
#ifdef SKIP_ASM
/**
 * Reads a little-endian 32-bit unsigned integer from a byte stream and advances the cursor.
 *
 * @param pp   pointer to the read cursor, moved forward 4 bytes
 * @param out  receives the value
 * @return 4, the number of bytes read
 */
extern "C" int get_uint(unsigned char **pp, unsigned int *out) {
    unsigned char *bytes = *pp;
    *out = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
    *pp += 4;
    return 4;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", get_t3Vector);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct t3Vector_25A0 { float x, y, z; };
extern "C" int get_float_25A0(void*, void*) __asm__("get_float");
/**
 * Reads a three-float vector from a comms data stream.
 *
 * @param src  the stream position
 * @param out  receives the vector
 * @return number of bytes read
 */
extern "C" int get_t3Vector(void *src, t3Vector_25A0 *out) {
    t3Vector_25A0 vec;
    int total = 0;
    int i;
    float *component = &vec.x;
    for (i = 0; i < 3; i++) {
        total += get_float_25A0(src, component);
        component++;
    }
    *out = vec;
    return total;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1D3510", func_002D2628);
#ifdef SKIP_ASM
extern "C" int get_float(int, int);

/**
 * Reads four floats from a byte stream into four consecutive floats (four get_float calls).
 *
 * @param src  the read cursor, advanced 16 bytes
 * @param out  receives the four floats
 * @return the bytes read (16)
 */
extern "C" int func_002D2628(int src, int out) {
    int temp_2;
    int outCursor;
    int remaining;
    int total;

    outCursor = out;
    remaining = 3;
    total = 0;
    do {
        temp_2 = get_float(src, outCursor);
        outCursor += 4;
        remaining -= 1;
        total += temp_2;
    } while (remaining >= 0);
    return total;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1D3510", func_002D2690);
#ifdef SKIP_ASM
extern "C" int get_float(int, int);

/**
 * Reads sixteen floats from a byte stream into sixteen consecutive floats (16 get_float calls).
 *
 * @param src  the read cursor, advanced 64 bytes
 * @param out  receives the floats
 * @return the bytes read (64)
 */
extern "C" int func_002D2690(int src, int out) {
    int temp_2;
    int outCursor;
    int remaining;
    int total;

    outCursor = out;
    remaining = 0xF;
    total = 0;
    do {
        temp_2 = get_float(src, outCursor);
        outCursor += 4;
        remaining -= 1;
        total += temp_2;
    } while (remaining >= 0);
    return total;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D26F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct V4_26F8 { float x, y, z, w; } __attribute__((aligned(16)));
extern "C" int get_float_26F8(void*, void*) __asm__("get_float");
/**
 * Reads four floats from a byte stream into a 16-byte aligned vector, the first one read going to the last component.
 *
 * The floats are read in the order w, x, y, z (four get_float calls) and stored as x, y, z, w.
 *
 * @param src  the read cursor, advanced 16 bytes
 * @param out  receives the vector
 * @return the bytes read (16)
 */
extern "C" int func_002D26F8(void *src, V4_26F8 *out) {
    float readW, readX, readY, readZ;
    V4_26F8 result;
    int total;
    total = get_float_26F8(src, &readW);
    total += get_float_26F8(src, &readX);
    total += get_float_26F8(src, &readY);
    total += get_float_26F8(src, &readZ);
    result.w = readW;
    result.x = readX;
    result.y = readY;
    result.z = readZ;
    *out = result;
    return total;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D2790);
#ifdef SKIP_ASM
/**
 * Reads one byte from a byte stream and advances the cursor.
 *
 * get_string and get_data read with it.
 *
 * @param cursor  the read cursor, advanced 1 byte
 * @param out  receives the byte
 * @return 1, the bytes read
 */
extern "C" int func_002D2790(unsigned char **cursor, unsigned char *out) {
    *out = **cursor;
    *cursor += 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", get_string);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag_27B0(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int func_002D2790_27B0(unsigned char **, unsigned char *) __asm__("func_002D2790");
extern "C" char *strcpy(char *, const char *);
extern char D_004D49B8[];
extern char str_Comm_String[];
/**
 * Reads a null-terminated string from a byte stream into a newly allocated copy.
 *
 * Copies bytes into the static buffer D_004D49B8 until a 0 or 1000 bytes (a string cut at 1000 is terminated at
 * byte 999), allocates that many bytes named "Comm String" (type 0x100) and copies the text there.
 *
 * @param src  pointer to the read cursor, advanced past the bytes read
 * @param out  receives the new string
 * @return the number of bytes read, including the terminator
 */
extern "C" int get_string(unsigned char **src, char **out) {
    int n = 0;
    char curChar = 1;
    do {
        n += func_002D2790_27B0(src, (unsigned char*)&curChar);
        D_004D49B8[n - 1] = curChar;
    } while (curChar != 0 && n < 1000);
    if (n == 1000) D_004D49B8[999] = 0;
    char *copy = (char*)operator_new_tag_27B0(n, str_Comm_String, 0x100, 0);
    *out = copy;
    strcpy(copy, D_004D49B8);
    return n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", get_data);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag_2868(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc_2868(int, const char*, int, int) __asm__("cMemMan_alloc");
extern "C" int func_002D2790_2868(unsigned char **, unsigned char *) __asm__("func_002D2790");
extern "C" int get_uint_2868(unsigned char **, void *) __asm__("get_uint");
extern char str_Comm_Data_Block[];
extern char str_Comm_Data[];
struct Data_2868 { unsigned int n; unsigned char *data; };
/**
 * Reads a length-prefixed byte block from a comms data stream into a new "Comm Data Block".
 *
 * @param src  the stream position, advanced past the block
 * @param out  receives the new block (count and "Comm Data" bytes)
 * @return number of bytes read
 */
extern "C" int get_data(unsigned char **src, Data_2868 **out) {
    unsigned int i = 0;
    Data_2868 *newData = (Data_2868*)cMemMan_alloc_2868(8, str_Comm_Data_Block, 0x100, 0);
    newData->n = 0;
    newData->data = 0;
    *out = newData;
    int total = get_uint_2868(src, newData);
    Data_2868 *reloaded = *out;
    reloaded->data = (unsigned char*)operator_new_tag_2868(reloaded->n, str_Comm_Data, 0x100, 0);
    Data_2868 *current = *out;
    for (i = 0; i < current->n; i++) {
        total += func_002D2790_2868(src, current->data + i);
        current = *out;
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D2948);
#ifdef SKIP_ASM
/**
 * Returns its first argument.
 *
 * It is variadic: the prologue stores the other argument registers (integer and float) to the stack and nothing reads
 * them. Nothing in the build references it; func_002D4080 is the same code.
 *
 * @return @p self
 */
extern "C" int func_002D2948(int self, ...) {
    return self;
}
#endif

INCLUDE_ASM("seg/seg_1D3510", func_002D2988);

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3788);
#ifdef SKIP_ASM
void operator_delete(int *);
extern void* D_004871E8[];
/**
 * Destroys a memory-card object of the base class: stores the base vtable D_004871E8 back at +0 and frees the object
 * when bit 0 of @p flags is set.
 *
 * @param flags  bit 0: free the memory
 */
extern "C" void func_002D3788(void ***self, int flags) {
    *self = D_004871E8;
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D37B8);
#ifdef SKIP_ASM
/**
 * Returns 1.
 *
 * It is word 53 (slot 26) of both D_004871E8 (the base memory-card vtable) and cPS2MemCard_vtbl, which does not
 * override it.
 *
 * @return 1
 */
extern "C" int func_002D37B8(void) {
    return 1;
}
#endif

/**
 * Does nothing.
 *
 * It is word 55 (slot 27) of both D_004871E8 (the base memory-card vtable) and cPS2MemCard_vtbl.
 */
extern "C" void func_002D37C0(void) {
}

/**
 * Does nothing.
 *
 * It is word 57 (slot 28) of both D_004871E8 (the base memory-card vtable) and cPS2MemCard_vtbl.
 */
extern "C" void func_002D37C8(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D37D0);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * It is word 149 (slot 74) of D_004871E8, the base memory-card vtable; cPS2MemCard_vtbl overrides that slot with
 * cPS2MemCard_FileExistsError.
 *
 * @return 0
 */
extern "C" int func_002D37D0(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetDevice);
#ifdef SKIP_ASM
/**
 * Returns the memory-card port the card object works on.
 *
 * An out-of-line inline (virtual slot 4): the whole body is `return mDevice;`.
 *
 * @param pS2MemCard  the memory card object (this)
 * @return mDevice (0 or 1)
 */
extern "C" int cPS2MemCard_GetDevice(cPS2MemCard* pS2MemCard) {
    return (pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D37E0);
#ifdef SKIP_ASM
struct VEnt_37E0 { short delta; short idx; int (*fn)(void*, int); };
/**
 * Tells whether the card in the current port (mDevice) is not a PS2 memory card: calls the card's virtual slot 48 (word
 * 97, cPS2MemCard_IsInvalid) with mDevice.
 *
 * It is word 99 (slot 49) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void func_002D37E0(cPS2MemCard* pS2MemCard) {
    VEnt_37E0 *vtEntry = (VEnt_37E0*)(*(char **)pS2MemCard + 0x180);
    vtEntry->fn((char*)pS2MemCard + vtEntry->delta, pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3810);
#ifdef SKIP_ASM
struct VEnt_3810 { short delta; short idx; int (*fn)(void*, int); };
/**
 * Tells whether the card in the current port (mDevice) is usable: calls the card's virtual slot 52 (word 105,
 * cPS2MemCard_IsAvailable) with mDevice.
 *
 * It is word 107 (slot 53) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void func_002D3810(cPS2MemCard* pS2MemCard) {
    VEnt_3810 *vtEntry = (VEnt_3810*)(*(char **)pS2MemCard + 0x1A0);
    vtEntry->fn((char*)pS2MemCard + vtEntry->delta, pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3840);
#ifdef SKIP_ASM
struct Obj_3840 {
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual void v8();
  virtual void v9();
  virtual void v10();
  virtual void v11();
  virtual void v12();
  virtual void v13();
  virtual void v14();
  virtual void v15();
  virtual void v16();
  virtual void v17();
  virtual void v18();
  virtual void v19();
  virtual void v20();
  virtual void v21();
  virtual void v22();
  virtual void v23();
  virtual void v24();
  virtual void v25();
  virtual void v26();
  virtual void v27();
  virtual void v28();
  virtual void v29();
  virtual void v30();
  virtual void v31();
  virtual void v32();
  virtual void v33();
  virtual void v34();
  virtual void v35();
  virtual void v36();
  virtual void v37();
  virtual void v38();
  virtual void v39();
  virtual void v40();
  virtual void v41();
  virtual void v42();
  virtual void v43();
  virtual void v44();
  virtual void v45();
  virtual void v46();
  virtual void v47();
  virtual int v48();
  virtual void v49();
  virtual int v50(int);
  virtual void v51();
  virtual int v52(int);
  virtual void v53();
  virtual void v54();
  virtual void v55();
  virtual int v56(int);
};
/**
 * Tells whether the card in a port is ready for use.
 *
 * Through the card's own virtual slots, with port @p x: not slot 48 (word 97, cPS2MemCard_IsInvalid), not slot 50 (word
 * 101, func_002C67A8__FPv, which always returns 0), slot 56 (word 113, cPS2MemCard_IsFormatted) and slot 52 (word 105,
 * cPS2MemCard_IsAvailable). The first call is declared without the port; it is still in its register. It is word 109
 * (slot 54) of cPS2MemCard_vtbl; the profile and options screens call it through the vtable before using a card.
 *
 * @param pS2MemCard  the memory card
 * @param x  the port (0 or 1)
 * @return 1 when every test passes, else 0
 */
extern "C" int func_002D3840(cPS2MemCard* pS2MemCard, int x) {
    int ok = 0;
    if (!((Obj_3840*)pS2MemCard)->v48() && !((Obj_3840*)pS2MemCard)->v50(x) && ((Obj_3840*)pS2MemCard)->v56(x)) {
        ok = ((Obj_3840*)pS2MemCard)->v52(x) != 0;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D38F0);
#ifdef SKIP_ASM
struct VEnt_38F0 { short delta; short idx; int (*fn)(void*, int); };
/**
 * Tells whether the card in the current port (mDevice) is formatted: calls the card's virtual slot 56 (word 113,
 * cPS2MemCard_IsFormatted) with mDevice.
 *
 * It is word 111 (slot 55) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void func_002D38F0(cPS2MemCard* pS2MemCard) {
    VEnt_38F0 *vtEntry = (VEnt_38F0*)(*(char **)pS2MemCard + 0x1C0);
    vtEntry->fn((char*)pS2MemCard + vtEntry->delta, pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3920);
#ifdef SKIP_ASM
/**
 * Returns 0: word 119 (slot 59) of cPS2MemCard_vtbl.
 *
 * @return 0
 */
extern "C" int func_002D3920(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3928);
#ifdef SKIP_ASM
/**
 * Returns 0: word 121 (slot 60) of cPS2MemCard_vtbl.
 *
 * @return 0
 */
extern "C" int func_002D3928(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3930);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPS2MemCard_CardChanged_3930(void*, int) __asm__("cPS2MemCard_CardChanged");
/**
 * Tells whether the card in the current port has changed: calls cPS2MemCard_CardChanged with mDevice (a direct call).
 *
 * It is word 127 (slot 63) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void func_002D3930(cPS2MemCard* pS2MemCard) {
    cPS2MemCard_CardChanged_3930(pS2MemCard, (pS2MemCard->mDevice));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3950);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPS2MemCard_GetStatus_3950(void*, int, int) __asm__("cPS2MemCard_GetStatus");
/**
 * Writes the status line of the card in the current port: calls cPS2MemCard_GetStatus with mDevice and @p outText.
 *
 * It is word 129 (slot 64) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @param outText  the 16-bit text buffer
 */
extern "C" void func_002D3950(cPS2MemCard* pS2MemCard, int outText) {
    cPS2MemCard_GetStatus_3950(pS2MemCard, (pS2MemCard->mDevice), outText);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3970);
#ifdef SKIP_ASM
/**
 * Returns the number of save-file slots, mMaxFiles (6 after a reset).
 *
 * It is word 137 (slot 68) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @return mMaxFiles
 */
extern "C" int func_002D3970(cPS2MemCard* pS2MemCard) {
    return (pS2MemCard->mMaxFiles);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3978);
#ifdef SKIP_ASM
/**
 * Returns 2, the number of memory-card ports.
 *
 * It is word 139 (slot 69) of cPS2MemCard_vtbl.
 *
 * @return 2
 */
extern "C" int func_002D3978(void) {
    return 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3980);
#ifdef SKIP_ASM
struct VEnt_3980 { short delta; short idx; void (*fn)(void*, int, int, int); };
/**
 * Passes its three arguments on to the card's virtual slot 70 (word 141, cPS2MemCard_GetDeviceName).
 *
 * cPS2MemCard_GetDeviceName reads only the port and the buffer. It is word 143 (slot 71) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @param a1  the port
 * @param a2  the 16-bit buffer for the name
 * @param a3  passed on as the third argument
 */
extern "C" void func_002D3980(cPS2MemCard* pS2MemCard, int a1, int a2, int a3) {
    VEnt_3980 *vtEntry = (VEnt_3980*)(*(char **)pS2MemCard + 0x230);
    vtEntry->fn((char*)pS2MemCard + vtEntry->delta, a1, a2, a3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39A8);
#ifdef SKIP_ASM
/**
 * Returns 0: word 145 (slot 72) of cPS2MemCard_vtbl.
 *
 * @return 0
 */
extern "C" int func_002D39A8(void) {
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39B0);
#ifdef SKIP_ASM
/**
 * Returns unk_03C, the outcome flag of the last card operation (cPS2MemCard_Update sets it to 0 or 1).
 *
 * It is word 151 (slot 75) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @return unk_03C
 */
extern "C" int func_002D39B0(cPS2MemCard* pS2MemCard) {
    return (pS2MemCard->unk_03C);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39B8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPS2MemCard_GetLastError_39B8(void*, int) __asm__("cPS2MemCard_GetLastError");
/**
 * Returns the last libmc result of the current port: calls cPS2MemCard_GetLastError with mDevice (+0xC).
 *
 * The C declares no return value; the call's result is left in the return register. Nothing in the build calls it.
 *
 * @param memCard  the memory card
 */
extern "C" void func_002D39B8(void *memCard) {
    cPS2MemCard_GetLastError_39B8(memCard, (*(int *)((char*)(memCard) + (0xC))));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39D8);
#ifdef SKIP_ASM
/**
 * Returns the memory card's state word mState (+4).
 *
 * Nothing in the build calls it.
 *
 * @param memCard  the memory card
 * @return mState
 */
extern "C" int func_002D39D8(void *memCard) {
    return (*(int *)((char*)(memCard) + (4)));
}
#endif

/**
 * Does nothing. Nothing in the build references it.
 */
extern "C" void func_002D39E0(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39E8);
#ifdef SKIP_ASM
/**
 * Returns the number of directory entries read from the card, mNumDirTable (+0x54).
 *
 * Nothing in the build calls it.
 *
 * @param memCard  the memory card
 * @return mNumDirTable
 */
extern "C" int func_002D39E8(void *memCard) {
    return (*(int *)((char*)(memCard) + (0x54)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D39F0);
#ifdef SKIP_ASM
extern int D_004A3940;

/**
 * Returns D_004A3940, the static word where the memory-card code keeps the last libmc result.
 *
 * Nothing in the build calls it.
 *
 * @return D_004A3940
 */
extern "C" int func_002D39F0(void) {
    return D_004A3940;
}
#endif

/**
 * Does nothing: the base menu item's word 17 (slot 8), which the menu-item vtables without an override share.
 *
 * cIntMenuItem_vtbl, cFloatMenuItem_vtbl and cCharMenuItem_vtbl put their own functions in that slot.
 */
extern "C" void func_002D39F8(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A00);
#ifdef SKIP_ASM
/**
 * Sets a menu item's ID (+8).
 *
 * @param arg0  the menu item
 * @param arg1  the new ID
 */
extern "C" void func_002D3A00(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (8))) = arg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A08);
#ifdef SKIP_ASM
/**
 * Returns a menu item's ID (+8).
 *
 * @param arg0  the menu item
 * @return the ID
 */
extern "C" int func_002D3A08(void *arg0) {
    return (*(int *)((char*)(arg0) + (8)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A10);
#ifdef SKIP_ASM
/**
 * Sets a menu item's Arg word (+0xC).
 *
 * @param arg0  the menu item
 * @param arg1  the new value
 */
extern "C" void func_002D3A10(void *arg0, int arg1) {
    (*(int *)((char*)(arg0) + (0xC))) = arg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A18);
#ifdef SKIP_ASM
/**
 * Returns a menu item's Arg word (+0xC).
 *
 * @param arg0  the menu item
 * @return Arg
 */
extern "C" int func_002D3A18(void *arg0) {
    return (*(int *)((char*)(arg0) + (0xC)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A20);
#ifdef SKIP_ASM
/**
 * Returns the menu a menu item belongs to (Menu, +0; cMenu_addItem fills it).
 *
 * @param arg0  the menu item
 * @return the menu
 */
extern "C" int func_002D3A20(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A28);
#ifdef SKIP_ASM
/**
 * Returns one of a menu's items: Items[@p arg1] (+0xC + 4 * index). The index is not checked.
 *
 * @param arg0  the menu
 * @param arg1  the item index
 * @return the item
 */
extern "C" int func_002D3A28(char *arg0, int arg1) {
    return *(int *)(arg0 + (arg1 << 2) + 0xC);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A38);
#ifdef SKIP_ASM
/**
 * Returns the number of items in a menu (NumItems, +0).
 *
 * @param arg0  the menu
 * @return NumItems
 */
extern "C" int func_002D3A38(int *arg0) {
    return *arg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A40);
#ifdef SKIP_ASM
/**
 * Returns the menu system that shows a menu (MenuSys, +0x124; cMenu_onGainFocus sets it).
 *
 * @param arg0  the menu
 * @return MenuSys
 */
extern "C" int func_002D3A40(void *arg0) {
    return (*(int *)((char*)(arg0) + (0x124)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A48);
#ifdef SKIP_ASM
/**
 * Returns the font of a menu's menu system: the word at +0x58 of MenuSys (+0x124).
 *
 * @param arg0  the menu
 * @return the font
 */
extern "C" int func_002D3A48(void *arg0) {
    return (*(int *)((char*)((*(void **)((char*)(arg0) + (0x124)))) + (0x58)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A58);
#ifdef SKIP_ASM
/**
 * Returns the font of the menu system that shows a menu item: the word at +0x58 of the MenuSys (+0x124) of the item's
 * Menu (+0).
 *
 * @param arg0  the menu item
 * @return the font
 */
extern "C" int func_002D3A58(void **arg0) {
    return (*(int *)((char*)((*(void **)((char*)(*arg0) + (0x124)))) + (0x58)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cBoolMenuItem__cBoolMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys an on/off menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cBoolMenuItem_vtbl.
 */
extern "C" void cBoolMenuItem__cBoolMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A88);
#ifdef SKIP_ASM
/**
 * Sets the flag an on/off menu item toggles (the flag pointer, +0x20).
 *
 * @param menuItem  the on/off item
 * @param value  the flag's address
 */
extern "C" void func_002D3A88(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x20))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3A90);
#ifdef SKIP_ASM
/**
 * Sets an on/off menu item's caption (+0x14).
 *
 * @param menuItem  the on/off item
 * @param value  the caption text
 */
extern "C" void func_002D3A90(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cEnumMenuItem__cEnumMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a list-choice menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cEnumMenuItem_vtbl.
 */
extern "C" void cEnumMenuItem__cEnumMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3AB8);
#ifdef SKIP_ASM
/**
 * Sets the entries a list-choice menu item offers: the entry count (+0x18) and the table of {name, value} entries
 * (+0x1C).
 *
 * @param menuItem  the list-choice item
 * @param arg1  the number of entries
 * @param arg2  the entries, 8 bytes each
 */
extern "C" void func_002D3AB8(void *menuItem, int arg1, int arg2) {
    (*(int *)((char*)(menuItem) + (0x18))) = arg1;
    (*(int *)((char*)(menuItem) + (0x1C))) = arg2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3AC8);
#ifdef SKIP_ASM
/**
 * Sets the int a list-choice menu item sets (the value pointer, +0x20).
 *
 * @param menuItem  the list-choice item
 * @param value  the int's address
 */
extern "C" void func_002D3AC8(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x20))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3AD0);
#ifdef SKIP_ASM
/**
 * Sets a list-choice menu item's caption (+0x14).
 *
 * @param menuItem  the list-choice item
 * @param value  the caption text
 */
extern "C" void func_002D3AD0(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3AD8);
#ifdef SKIP_ASM
/**
 * Returns a list-choice menu item's caption (+0x14).
 *
 * @param menuItem  the list-choice item
 * @return the caption
 */
extern "C" int func_002D3AD8(void *menuItem) {
    return (*(int *)((char*)(menuItem) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cSubMenuItem__cSubMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a sub-menu item (a menu entry that opens another menu) through the base cMenuItem destructor.
 *
 * Declared without parameters: the object and the destructor flags reach cMenuItem__cMenuItem() unchanged in
 * their registers. It restores cMenuItem's vtable and frees the object when bit 0 of the flags is set.
 */
extern "C" void cSubMenuItem__cSubMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B00);
#ifdef SKIP_ASM
/**
 * Sets the sub-menu a sub-menu item opens (+0x18).
 *
 * @param menuItem  the sub-menu item
 * @param value  the sub-menu
 */
extern "C" void func_002D3B00(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B08);
#ifdef SKIP_ASM
/**
 * Returns the sub-menu a sub-menu item opens (+0x18).
 *
 * @param menuItem  the sub-menu item
 * @return the sub-menu
 */
extern "C" int func_002D3B08(void *menuItem) {
    return (*(int *)((char*)(menuItem) + (0x18)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPopMenuItem__cPopMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a "back" menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cPopMenuItem_vtbl.
 */
extern "C" void cPopMenuItem__cPopMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B30);
#ifdef SKIP_ASM
/**
 * Sets a "back" menu item's caption (+0x14).
 *
 * @param menuItem  the "back" item
 * @param value  the caption text
 */
extern "C" void func_002D3B30(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem__cCommandMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a command menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cCommandMenuItem_vtbl.
 */
extern "C" void cCommandMenuItem__cCommandMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B58);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's state text (+0x18), shown next to its caption.
 *
 * @param menuItem  the command item
 * @param value  the text
 */
extern "C" void func_002D3B58(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B60);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's caption (+0x14).
 *
 * @param menuItem  the command item
 * @param value  the caption text
 */
extern "C" void func_002D3B60(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B68);
#ifdef SKIP_ASM
/**
 * Returns a command menu item's caption (+0x14).
 *
 * @param menuItem  the command item
 * @return the caption
 */
extern "C" int func_002D3B68(void *menuItem) {
    return (*(int *)((char*)(menuItem) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B70);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's data word (+0x20).
 *
 * @param menuItem  the command item
 * @param value  the data
 */
extern "C" void func_002D3B70(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x20))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3B78);
#ifdef SKIP_ASM
/**
 * Returns a command menu item's data word (+0x20).
 *
 * @param menuItem  the command item
 * @return the data
 */
extern "C" int func_002D3B78(void *menuItem) {
    return (*(int *)((char*)(menuItem) + (0x20)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCallbackMenuItem__cCallbackMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a callback menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cCallbackMenuItem_vtbl.
 */
extern "C" void cCallbackMenuItem__cCallbackMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3BA0);
#ifdef SKIP_ASM
/**
 * Sets a callback menu item's caption (+0x14).
 *
 * @param menuItem  the callback item
 * @param value  the caption text
 */
extern "C" void func_002D3BA0(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3BA8);
#ifdef SKIP_ASM
/**
 * Returns a callback menu item's caption (+0x14).
 *
 * @param menuItem  the callback item
 * @return the caption
 */
extern "C" int func_002D3BA8(void *menuItem) {
    return (*(int *)((char*)(menuItem) + (0x14)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cNullMenuItem__cNullMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a null (empty) debug menu item: only the cMenuItem destructor, which receives the object and flags in
 * the untouched argument registers.
 */
extern "C" void cNullMenuItem__cNullMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3BD0);
#ifdef SKIP_ASM
/**
 * Sets a text-only menu item's text (+0x14).
 *
 * @param menuItem  the text-only item
 * @param value  the text
 */
extern "C" void func_002D3BD0(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3BD8);
#ifdef SKIP_ASM
/**
 * Stores a text pointer at +0x18 of a text-only menu item (cNullMenuItem_cNullMenuItem does not set this word).
 *
 * @param menuItem  the text-only item
 * @param value  the text
 */
extern "C" void func_002D3BD8(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cSpaceMenuItem__cSpaceMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a spacer debug menu item: only the cMenuItem destructor, which receives the object and flags in the
 * untouched argument registers.
 */
extern "C" void cSpaceMenuItem__cSpaceMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCharMenuItem__cCharMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a signed-byte menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cCharMenuItem_vtbl.
 */
extern "C" void cCharMenuItem__cCharMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C20);
#ifdef SKIP_ASM
/**
 * Sets the signed byte a signed-byte menu item edits (the value pointer, +0x18).
 *
 * @param menuItem  the signed-byte item
 * @param value  the byte's address
 */
extern "C" void func_002D3C20(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C28);
#ifdef SKIP_ASM
/**
 * Sets a signed-byte menu item's caption (+0x14).
 *
 * @param menuItem  the signed-byte item
 * @param value  the caption text
 */
extern "C" void func_002D3C28(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cIntMenuItem__cIntMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys an integer menu item through the base cMenuItem destructor.
 *
 * Declared without parameters: the object and the destructor flags reach cMenuItem__cMenuItem() unchanged in
 * their registers. It restores cMenuItem's vtable and frees the object when bit 0 of the flags is set.
 */
extern "C" void cIntMenuItem__cIntMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C50);
#ifdef SKIP_ASM
/**
 * Sets the int an int menu item edits (the variable's address, +0x18).
 *
 * @param menuItem  the int item
 * @param value  the int's address
 */
extern "C" void func_002D3C50(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C58);
#ifdef SKIP_ASM
/**
 * Sets an int menu item's caption (+0x14).
 *
 * @param menuItem  the int item
 * @param value  the caption text
 */
extern "C" void func_002D3C58(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem__cFloatMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a float menu item through the base cMenuItem destructor.
 *
 * Declared without parameters: the object and the destructor flags reach cMenuItem__cMenuItem() unchanged in
 * their registers. It restores cMenuItem's vtable and frees the object when bit 0 of the flags is set.
 */
extern "C" void cFloatMenuItem__cFloatMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C80);
#ifdef SKIP_ASM
/**
 * Sets the float a float menu item edits (the variable's address, +0x18).
 *
 * @param menuItem  the float item
 * @param value  the float's address
 */
extern "C" void func_002D3C80(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C88);
#ifdef SKIP_ASM
/**
 * Sets a float menu item's range: minimum (+0x1C) and maximum (+0x20).
 *
 * @param menuItem  the float item
 * @param fparg0  the minimum
 * @param fparg1  the maximum
 */
extern "C" void func_002D3C88(void *menuItem, float fparg0, float fparg1) {
    (*(float *)((char*)(menuItem) + (0x1C))) = fparg0;
    (*(float *)((char*)(menuItem) + (0x20))) = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3C98);
#ifdef SKIP_ASM
/**
 * Sets a float menu item's caption (+0x14).
 *
 * @param menuItem  the float item
 * @param value  the caption text
 */
extern "C" void func_002D3C98(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3CA0);
#ifdef SKIP_ASM
/**
 * Sets how a float menu item shows its value: the display scale (+0x24) and offset (+0x28); the item shows scale *
 * value + offset.
 *
 * @param menuItem  the float item
 * @param fparg0  the scale
 * @param fparg1  the offset
 */
extern "C" void func_002D3CA0(void *menuItem, float fparg0, float fparg1) {
    (*(float *)((char*)(menuItem) + (0x24))) = fparg0;
    (*(float *)((char*)(menuItem) + (0x28))) = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3CB0);
#ifdef SKIP_ASM
/**
 * Writes a new value into the float a float menu item edits (through the value pointer, +0x18).
 *
 * The value is stored as given, without the display scale and offset.
 *
 * @param menuItem  the float item
 * @param fparg0  the new value
 */
extern "C" void func_002D3CB0(void *menuItem, float fparg0) {
    *(*(float **)((char*)(menuItem) + (0x18))) = fparg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3CC0);
#ifdef SKIP_ASM
/**
 * Returns a float menu item's value as displayed: scale (+0x24) * the edited float (through +0x18) + offset (+0x28).
 *
 * @param menuItem  the float item
 * @return the displayed value
 */
extern "C" float func_002D3CC0(void *menuItem) {
    return ((*(float *)((char*)(menuItem) + (0x24))) * *(*(float **)((char*)(menuItem) + (0x18)))) + (*(float *)((char*)(menuItem) + (0x28)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cAngleMenuItem__cAngleMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys an angle menu item through the base cMenuItem destructor.
 *
 * Declared without parameters: the object and the destructor flags reach cMenuItem__cMenuItem() unchanged in
 * their registers. It restores cMenuItem's vtable and frees the object when bit 0 of the flags is set.
 */
extern "C" void cAngleMenuItem__cAngleMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3D00);
#ifdef SKIP_ASM
/**
 * Sets the float an angle menu item edits (the value pointer, +0x18).
 *
 * @param menuItem  the angle item
 * @param value  the float's address
 */
extern "C" void func_002D3D00(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3D08);
#ifdef SKIP_ASM
/**
 * Sets an angle menu item's range: minimum (+0x1C) and maximum (+0x20).
 *
 * @param menuItem  the angle item
 * @param fparg0  the minimum
 * @param fparg1  the maximum
 */
extern "C" void func_002D3D08(void *menuItem, float fparg0, float fparg1) {
    (*(float *)((char*)(menuItem) + (0x1C))) = fparg0;
    (*(float *)((char*)(menuItem) + (0x20))) = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3D18);
#ifdef SKIP_ASM
/**
 * Sets an angle menu item's caption (+0x14).
 *
 * @param menuItem  the angle item
 * @param value  the caption text
 */
extern "C" void func_002D3D18(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cTextMenuItem__cTextMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a text-entry menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in
 * their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cTextMenuItem_vtbl.
 */
extern "C" void cTextMenuItem__cTextMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3D40);
#ifdef SKIP_ASM
/**
 * Returns a text-entry menu item's text: the address of its 16-byte buffer (+0x18).
 *
 * @param menuItem  the text-entry item
 * @return the buffer address
 */
extern "C" int func_002D3D40(int menuItem) {
    return menuItem + 0x18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3D48);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int func_00416958_3D48(char*, int, int) __asm__("func_00416958");
/**
 * Sets a text-entry menu item's text: clears the 16-byte buffer at +0x18 and copies @p text into it with func_00416958
 * (at most 15 characters).
 *
 * @param menuItem  the text-entry item
 * @param text  the new text
 * @return what func_00416958 returns
 */
extern "C" int func_002D3D48(char *menuItem, int text) {
    char *textBuf = menuItem + 0x18;
    *textBuf = 0;
    return func_00416958_3D48(textBuf, text, 0xF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cColorMenuItem__cColorMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a colour-channel menu item through the base cMenuItem destructor (a tail call that passes its arguments
 * through).
 */
extern "C" void cColorMenuItem__cColorMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cExpandMenuItemMenu__cExpandMenuItemMenu);
#ifdef SKIP_ASM
extern "C" void cMenu__cMenu();

/**
 * Destroys an expand-menu-item menu through the base cMenu destructor.
 *
 * Declared without parameters: the object and the destructor flags reach cMenu__cMenu() unchanged in their
 * registers.
 */
extern "C" void cExpandMenuItemMenu__cExpandMenuItemMenu(void) {
    cMenu__cMenu();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cExpandMenuItem__cExpandMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMenuItem__cMenuItem_3DB0(int, int) __asm__("cMenuItem__cMenuItem");
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys an expand menu item: its embedded sub-menu, then the cMenuItem base.
 *
 * Runs cMenu__cMenu() on the menu at offset 0x18 with flags 2 (no free), then cMenuItem__cMenuItem().
 *
 * @param menuItem  the item
 * @param flags  g++ destructor flags, passed to cMenuItem__cMenuItem(); bit 0 set frees the item's memory
 */
extern "C" void cExpandMenuItem__cExpandMenuItem(int menuItem, int flags) {
    cMenu__cMenu(menuItem + 0x18, 2);
    cMenuItem__cMenuItem_3DB0(menuItem, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cRGBTitleMenuItem__cRGBTitleMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a coloured-title debug menu item: only the cMenuItem destructor, which receives the object and flags in
 * the untouched argument registers.
 */
extern "C" void cRGBTitleMenuItem__cRGBTitleMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3E18);
#ifdef SKIP_ASM
/**
 * Sets the title text of a colour editor's title row (+0x18).
 *
 * @param menuItem  the title row
 * @param value  the title text
 */
extern "C" void func_002D3E18(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cARGBMenuItem__cARGBMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMenuItem__cMenuItem_3E20(int, int) __asm__("cMenuItem__cMenuItem");
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys a colour (ARGB) debug-menu item: its five channel items and embedded menu, then the cMenuItem
 * base.
 *
 * @param menuItem  the menu item
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cARGBMenuItem__cARGBMenuItem(int menuItem, int flags) {
    cMenuItem__cMenuItem_3E20(menuItem + 1600, 2);
    cMenuItem__cMenuItem_3E20(menuItem + 1564, 2);
    cMenuItem__cMenuItem_3E20(menuItem + 1528, 2);
    cMenuItem__cMenuItem_3E20(menuItem + 1492, 2);
    cMenuItem__cMenuItem_3E20(menuItem + 1456, 2);
    cMenu__cMenu(menuItem + 0x18, 2);
    cMenuItem__cMenuItem_3E20(menuItem, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3EA0);
#ifdef SKIP_ASM
/**
 * Sets a colour editor menu item's caption: the title text (+0x18) of its embedded title row (+0x640), i.e. +0x658.
 *
 * @param menuItem  the colour editor item
 * @param value  the caption text
 */
extern "C" void func_002D3EA0(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x658))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cVectorTitleMenuItem__cVectorTitleMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a vector title menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay
 * in their registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cVectorTitleMenuItem_vtbl.
 */
extern "C" void cVectorTitleMenuItem__cVectorTitleMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3EC8);
#ifdef SKIP_ASM
/**
 * Sets a vector title menu item's caption (+0x18).
 *
 * @param menuItem  the vector title item
 * @param value  the caption text
 */
extern "C" void func_002D3EC8(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x18))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cVectorMenuItem__cVectorMenuItem);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern void cMenuItem__cMenuItem_3ED0(int, int) __asm__("cMenuItem__cMenuItem");
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys a vector debug-menu item: its four component items and embedded menu, then the cMenuItem base.
 *
 * @param menuItem  the menu item
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVectorMenuItem__cVectorMenuItem(int menuItem, int flags) {
    cMenuItem__cMenuItem_3ED0(menuItem + 1620, 2);
    cMenuItem__cMenuItem_3ED0(menuItem + 1568, 2);
    cMenuItem__cMenuItem_3ED0(menuItem + 1516, 2);
    cMenuItem__cMenuItem_3ED0(menuItem + 1464, 2);
    cMenu__cMenu(menuItem + 0x18, 2);
    cMenuItem__cMenuItem_3ED0(menuItem, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3F48);
#ifdef SKIP_ASM
/**
 * Sets a vector editor menu item's caption: the caption (+0x18) of its embedded title row (+0x654), i.e. +0x66C.
 *
 * @param menuItem  the vector editor item
 * @param value  the caption text
 */
extern "C" void func_002D3F48(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x66C))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cGraphMenuItem__cGraphMenuItem);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem();

/**
 * Destroys a graph menu item: passes the object and the destructor flags on to cMenuItem__cMenuItem (they stay in their
 * registers; the C declares no parameters).
 *
 * It is word 3 (slot 1) of cGraphMenuItem_vtbl.
 */
extern "C" void cGraphMenuItem__cGraphMenuItem(void) {
    cMenuItem__cMenuItem();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D3F70);
#ifdef SKIP_ASM
/**
 * Sets a graph menu item's caption (+0x14).
 *
 * @param menuItem  the graph item
 * @param value  the caption text
 */
extern "C" void func_002D3F70(void *menuItem, int value) {
    (*(int *)((char*)(menuItem) + (0x14))) = value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cErrorMenu__cErrorMenu);
#ifdef SKIP_ASM
extern "C" void cMenuItem__cMenuItem(int, int);
extern "C" void cMenu__cMenu(int, int);

/**
 * Destroys an error menu.
 *
 * Destroys its three embedded items in reverse order of construction, each with cMenuItem__cMenuItem and flags 2: the
 * "back" item (+0x164), the space (+0x14C) and the text item (+0x130). Then runs cMenu__cMenu with @p flags. It is word
 * 3 (slot 1) of D_004867B0, the vtable cErrorMenu_cErrorMenu installs.
 *
 * @param menu  the error menu
 * @param flags  passed to cMenu__cMenu (bit 0: free the memory)
 */
extern "C" void cErrorMenu__cErrorMenu(int menu, int flags) {
    cMenuItem__cMenuItem(menu + 356, 2);
    cMenuItem__cMenuItem(menu + 332, 2);
    cMenuItem__cMenuItem(menu + 304, 2);
    cMenu__cMenu(menu, flags);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2ConsoleConfig__cPS2ConsoleConfig);
#ifdef SKIP_ASM
void operator_delete(int *);
extern void* D_00487480[];
/**
 * Destroys the PS2 console configuration object.
 *
 * Nothing of its own to release: stores the cConsoleConfig vtable D_00487480 back at +0x34 (where the vptr sits) and
 * frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2ConsoleConfig__cPS2ConsoleConfig(char *self, int flags) {
    *(void***)(self + 0x34) = D_00487480;
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D4008);
#ifdef SKIP_ASM
extern int (*D_004A54BC)(int, int);

/**
 * Compares two menu-item pointers for cMenu_sortItems: calls the comparison function in D_004A54BC with the two items.
 *
 * cMenu_sortItems stores its comparison function in D_004A54BC and passes this function to the qsort func_00418EF8. The
 * C declares no return value; the comparison's result is left in the return register.
 *
 * @param arg0  points to the first item pointer
 * @param arg1  points to the second item pointer
 */
extern "C" void func_002D4008(int *arg0, int *arg1) {
    D_004A54BC(*arg0, *arg1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D4030);
#ifdef SKIP_ASM
void operator_delete(int *);
extern void* D_00487480[];
/**
 * Destroys a console configuration object: stores the vtable D_00487480 at +0x34 and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is word 3 (slot 1) of D_00487480. Its code is the same as cPS2ConsoleConfig__cPS2ConsoleConfig.
 *
 * @param flags  bit 0: free the memory
 */
extern "C" void func_002D4030(char *self, int flags) {
    *(void***)(self + 0x34) = D_00487480;
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1D3510", func_002D4060);
#ifdef SKIP_ASM
extern "C" void func_002D2988(int, int);

/**
 * Runs a static initializer of this unit: func_002D2988 with initialize 1 and priority 0xFFFF.
 *
 * It is word 24 of the global constructor table D_0043CE38 (walked by func_0040FBE0).
 */
extern "C" void func_002D4060(void) {
    func_002D2988(1, 0xFFFF);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D4080);
#ifdef SKIP_ASM
/**
 * Returns its first argument.
 *
 * It is variadic: the prologue stores the other argument registers (integer and float) to the stack and nothing reads
 * them. Nothing in the build references it; func_002D2948 is the same code.
 *
 * @return @p self
 */
extern "C" int func_002D4080(int self, ...) {
    return self;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", func_002D40C0);
#ifdef SKIP_ASM
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_40C0 {
    float r, g, b, a;
    sColor_40C0(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sSlot_40C0 {
    unsigned a : 8;
    unsigned b : 24;
    int c;
    sSlot_40C0(int n) { a = n; b = n; }
};

extern sSlot_40C0 D_004A54C0;
extern sColor_40C0 D_004D53F0;
extern sColor_40C0 D_004D5400;
extern sColor_40C0 D_004D5410;
extern sColor_40C0 D_004D5420;
extern sColor_40C0 D_004D5430;
extern sColor_40C0 D_004D5440;
extern sColor_40C0 D_004D5450;
extern sColor_40C0 D_004D5460;
extern sColor_40C0 D_004D5470;
extern sColor_40C0 D_004D5480;
extern sColor_40C0 D_004D5490;
extern sColor_40C0 D_004D54A0;
extern sColor_40C0 D_004D54B0;
extern sColor_40C0 D_004D54C0;
extern sColor_40C0 D_004D54D0;
extern sColor_40C0 D_004D54E0;
extern sColor_40C0 D_004D54F0;
extern sColor_40C0 D_004D5500;
extern sColor_40C0 D_004D5510;
extern sColor_40C0 D_004D5520;
extern sColor_40C0 D_004D5530;
extern sColor_40C0 D_004D5540;
extern sColor_40C0 D_004D5550;
extern sColor_40C0 D_004D5560;
extern sColor_40C0 D_004D5570;
extern sColor_40C0 D_004D5580;
extern sColor_40C0 D_004D5590;
extern sColor_40C0 D_004D55A0;
extern sColor_40C0 D_004D55B0;
extern sColor_40C0 D_004D55C0;
extern sColor_40C0 D_004D55D0;
extern sColor_40C0 D_004D55E0;
extern sColor_40C0 D_004D55F0;
extern sColor_40C0 D_004D5600;
extern sColor_40C0 D_004D5610;
extern sColor_40C0 D_004D5620;
extern sColor_40C0 D_004D5630;
extern sColor_40C0 D_004D5640;
extern sColor_40C0 D_004D5650;
extern sColor_40C0 D_004D5660;
extern sColor_40C0 D_004D5670;
extern sColor_40C0 D_004D5680;
extern sColor_40C0 D_004D5690;
extern sColor_40C0 D_004D56A0;
extern sColor_40C0 D_004D56B0;
extern sColor_40C0 D_004D56C0;
extern sColor_40C0 D_004D56D0;
extern sColor_40C0 D_004D56E0;
extern sColor_40C0 D_004D56F0;
extern sColor_40C0 D_004D5700;
extern sColor_40C0 D_004D5710;
extern sColor_40C0 D_004D5720;
extern sColor_40C0 D_004D5730;
extern sColor_40C0 D_004D5740;
extern sColor_40C0 D_004D5750;
extern sColor_40C0 D_004D5760;

/**
 * Constructs this unit's static four-float values (a g++ static-initialization function).
 *
 * Only for @p initialize nonzero and @p priority 0xFFFF: fills the 56 16-byte values D_004D53F0 to D_004D5760 with
 * their constant floats, and sets both bit-fields of D_004A54C0 (8 and 24 bits) to -1. func_002D48D0 calls it.
 *
 * @param initialize  nonzero to construct
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_002D40C0(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004D53F0) sColor_40C0(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004D5400) sColor_40C0(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004D5410) sColor_40C0(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004D5420) sColor_40C0(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004D5430) sColor_40C0(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004D5440) sColor_40C0(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004D5450) sColor_40C0(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004D5460) sColor_40C0(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004D5470) sColor_40C0(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004D5480) sColor_40C0(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004D5490) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D54A0) sColor_40C0(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D54B0) sColor_40C0(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004D54C0) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D54D0) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D54E0) sColor_40C0(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D54F0) sColor_40C0(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004D5500) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D5510) sColor_40C0(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D5520) sColor_40C0(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D5530) sColor_40C0(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004D5540) sColor_40C0(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004D5550) sColor_40C0(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004D5560) sColor_40C0(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004D5570) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D5580) sColor_40C0(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004D5590) sColor_40C0(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004D55A0) sColor_40C0(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004D55B0) sColor_40C0(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004D55C0) sColor_40C0(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004D55D0) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D55E0) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D55F0) sColor_40C0(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004D5600) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D5610) sColor_40C0(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004D5620) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D5630) sColor_40C0(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D5640) sColor_40C0(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004D5650) sColor_40C0(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004D5660) sColor_40C0(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004D5670) sColor_40C0(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004D5680) sColor_40C0(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004D5690) sColor_40C0(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004D56A0) sColor_40C0(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004D56B0) sColor_40C0(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D56C0) sColor_40C0(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D56D0) sColor_40C0(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004D56E0) sColor_40C0(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004D56F0) sColor_40C0(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004D5700) sColor_40C0(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004D5710) sColor_40C0(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004D5720) sColor_40C0(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004D5730) sColor_40C0(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004D5740) sColor_40C0(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004D5750) sColor_40C0(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004D5760) sColor_40C0(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004A54C0) sSlot_40C0(-1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1D3510", func_002D48D0);
#ifdef SKIP_ASM
extern "C" void func_002D40C0(int, int);

/**
 * Runs a static initializer of this unit: func_002D40C0 with initialize 1 and priority 0xFFFF.
 *
 * It is word 25 of the global constructor table D_0043CE38 (walked by func_0040FBE0).
 */
extern "C" void func_002D48D0(void) {
    func_002D40C0(1, 0xFFFF);
}
#endif
