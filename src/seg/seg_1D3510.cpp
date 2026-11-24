#include "common.h"
#include "vtable.h"
#include "util/objectinterface.h"
#include "bx/math3d.h"
#include "ui/uithing.h"
#include "util/consoleconfig.h"
#include "util/menu.h"
#include "util/bxmemcard.h"
#include "util/bxps2memcard.h"
#include "util/menusys.h"
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
extern "C" int get_float_25A0(void*, void*) __asm__("get_float");
/**
 * Reads a three-float vector from a comms data stream.
 *
 * @param src  the stream position
 * @param out  receives the vector
 * @return number of bytes read
 */
extern "C" int get_t3Vector(void *src, t3Vector *out) {
    t3Vector vec;
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
INCLUDE_ASM("seg/seg_1D3510", get_t4Vector);
#ifdef SKIP_ASM
extern "C" int get_float(int, int);

/**
 * Reads four floats from a byte stream into four consecutive floats (four get_float calls).
 *
 * @param src  the read cursor, advanced 16 bytes
 * @param out  receives the four floats
 * @return the bytes read (16)
 */
extern "C" int get_t4Vector(int src, int out) {
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
INCLUDE_ASM("seg/seg_1D3510", get_t4Matrix);
#ifdef SKIP_ASM
extern "C" int get_float(int, int);

/**
 * Reads sixteen floats from a byte stream into sixteen consecutive floats (16 get_float calls).
 *
 * @param src  the read cursor, advanced 64 bytes
 * @param out  receives the floats
 * @return the bytes read (64)
 */
extern "C" int get_t4Matrix(int src, int out) {
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
INCLUDE_ASM("seg/seg_1D3510", get_tQuaternion);
#ifdef SKIP_ASM
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
extern "C" int get_tQuaternion(void *src, tQuaternion *out) {
    float readW, readX, readY, readZ;
    tQuaternion result;
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
INCLUDE_ASM("seg/seg_1D3510", get_char);
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
extern "C" int get_char(unsigned char **cursor, unsigned char *out) {
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
extern "C" int get_char_27B0(unsigned char **, unsigned char *) __asm__("get_char");
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
        n += get_char_27B0(src, (unsigned char*)&curChar);
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
extern "C" int get_char_2868(unsigned char **, unsigned char *) __asm__("get_char");
extern "C" int get_uint_2868(unsigned char **, void *) __asm__("get_uint");
extern char str_Comm_Data_Block[];
extern char str_Comm_Data[];
/**
 * Reads a length-prefixed byte block from a comms data stream into a new "Comm Data Block".
 *
 * @param src  the stream position, advanced past the block
 * @param out  receives the new block (count and "Comm Data" bytes)
 * @return number of bytes read
 */
extern "C" int get_data(unsigned char **src, tCommDataBlock **out) {
    unsigned int i = 0;
    tCommDataBlock *newData = (tCommDataBlock*)cMemMan_alloc_2868(8, str_Comm_Data_Block, 0x100, 0);
    newData->Size = 0;
    (*(unsigned char**)&newData->Data) = 0;
    *out = newData;
    int total = get_uint_2868(src, newData);
    tCommDataBlock *reloaded = *out;
    (*(unsigned char**)&reloaded->Data) = (unsigned char*)operator_new_tag_2868(reloaded->Size, str_Comm_Data, 0x100, 0);
    tCommDataBlock *current = *out;
    for (i = 0; i < current->Size; i++) {
        total += get_char_2868(src, (unsigned char*)&current->Data[i]);
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
INCLUDE_ASM("seg/seg_1D3510", cMemCard__cMemCard);
#ifdef SKIP_ASM
void operator_delete(int *);
extern void* cMemCard_vtbl[];
/**
 * Destroys a memory-card object of the base class: stores the base vtable cMemCard_vtbl back at +0 and frees the object
 * when bit 0 of @p flags is set.
 *
 * @param flags  bit 0: free the memory
 */
extern "C" void cMemCard__cMemCard(void ***self, int flags) {
    *self = cMemCard_vtbl;
    if (flags & 1) operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMemCard_IsDeviceChanged);
#ifdef SKIP_ASM
/**
 * Returns 1.
 *
 * It is word 53 (slot 26) of both cMemCard_vtbl (the base memory-card vtable) and cPS2MemCard_vtbl, which does not
 * override it.
 *
 * @return 1
 */
extern "C" int cMemCard_IsDeviceChanged(void) {
    return 1;
}
#endif

/**
 * Does nothing.
 *
 * It is word 55 (slot 27) of both cMemCard_vtbl (the base memory-card vtable) and cPS2MemCard_vtbl.
 */
extern "C" void cMemCard_ClearDeviceChanges(void) {
}

/**
 * Does nothing.
 *
 * It is word 57 (slot 28) of both cMemCard_vtbl (the base memory-card vtable) and cPS2MemCard_vtbl.
 */
extern "C" void cMemCard_SetAllDevicesChanged(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", cMemCard_FileExistsError);
#ifdef SKIP_ASM
/**
 * Returns 0.
 *
 * It is word 149 (slot 74) of cMemCard_vtbl, the base memory-card vtable; cPS2MemCard_vtbl overrides that slot with
 * cPS2MemCard_FileExistsError.
 *
 * @return 0
 */
extern "C" int cMemCard_FileExistsError(void) {
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
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_IsInvalid1);
#ifdef SKIP_ASM
/**
 * Tells whether the card in the current port (mDevice) is not a PS2 memory card: calls the card's virtual slot 48 (word
 * 97, cPS2MemCard_IsInvalid) with mDevice.
 *
 * It is word 99 (slot 49) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void cPS2MemCard_IsInvalid1(cPS2MemCard* pS2MemCard) {
    ((cMemCard*)pS2MemCard)->IsInvalid(pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_IsAvailable1);
#ifdef SKIP_ASM
/**
 * Tells whether the card in the current port (mDevice) is usable: calls the card's virtual slot 52 (word 105,
 * cPS2MemCard_IsAvailable) with mDevice.
 *
 * It is word 107 (slot 53) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void cPS2MemCard_IsAvailable1(cPS2MemCard* pS2MemCard) {
    ((cMemCard*)pS2MemCard)->IsAvailable(pS2MemCard->mDevice);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_IsActionable);
#ifdef SKIP_ASM
/**
 * Tells whether the card in a port is ready for use.
 *
 * Through the card's own cMemCard virtuals, with port @p x: not IsInvalid (slot 48, word 97, cPS2MemCard_IsInvalid), not
 * IsDamaged (slot 50, word 101, func_002C67A8__FPv, which always returns 0), IsFormatted1 (slot 56, word 113,
 * cPS2MemCard_IsFormatted) and IsAvailable (slot 52, word 105, cPS2MemCard_IsAvailable). It is word 109 (slot 54) of
 * cPS2MemCard_vtbl, cMemCard::IsActionable; the profile and options screens call it before using a card.
 *
 * @param pS2MemCard  the memory card
 * @param x  the port (0 or 1)
 * @return 1 when every test passes, else 0
 */
extern "C" int cPS2MemCard_IsActionable(cPS2MemCard* pS2MemCard, int x) {
    cMemCard* card = (cMemCard*)pS2MemCard;
    int ok = 0;
    if (!card->IsInvalid(x) && !card->IsDamaged(x) && card->IsFormatted1(x)) {
        ok = card->IsAvailable(x) != 0;
    }
    return ok;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_IsFormatted1);
#ifdef SKIP_ASM
/**
 * Tells whether the card in the current port (mDevice) is formatted: calls the card's virtual slot 56 (word 113,
 * cPS2MemCard_IsFormatted) with mDevice.
 *
 * It is word 111 (slot 55) of cPS2MemCard_vtbl. The C declares no return value; the call's result is left in the return
 * register.
 *
 * @param pS2MemCard  the memory card
 */
extern "C" void cPS2MemCard_IsFormatted1(cPS2MemCard* pS2MemCard) {
    ((cMemCard*)pS2MemCard)->IsFormatted1(pS2MemCard->mDevice);
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
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_CardChanged1);
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
extern "C" void cPS2MemCard_CardChanged1(cPS2MemCard* pS2MemCard) {
    cPS2MemCard_CardChanged_3930(pS2MemCard, (pS2MemCard->mDevice));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetStatus1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cPS2MemCard_GetStatus_3950(void*, int, unsigned short*) __asm__("cPS2MemCard_GetStatus");
/**
 * Writes the status line of the card in the current port: calls cPS2MemCard_GetStatus with mDevice and @p outText.
 *
 * It is word 129 (slot 64) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @param outText  the 16-bit text buffer
 */
extern "C" void cPS2MemCard_GetStatus1(cPS2MemCard* pS2MemCard, unsigned short* outText) {
    cPS2MemCard_GetStatus_3950(pS2MemCard, (pS2MemCard->mDevice), outText);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetMaxFiles);
#ifdef SKIP_ASM
/**
 * Returns the number of save-file slots, mMaxFiles (6 after a reset).
 *
 * It is word 137 (slot 68) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @return mMaxFiles
 */
extern "C" int cPS2MemCard_GetMaxFiles(cPS2MemCard* pS2MemCard) {
    return (pS2MemCard->mMaxFiles);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetMaxDevices);
#ifdef SKIP_ASM
/**
 * Returns 2, the number of memory-card ports.
 *
 * It is word 139 (slot 69) of cPS2MemCard_vtbl.
 *
 * @return 2
 */
extern "C" int cPS2MemCard_GetMaxDevices(void) {
    return 2;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetDeviceNameRaw);
#ifdef SKIP_ASM
/**
 * Passes its three arguments on to the card's GetDeviceName (slot 70, word 141, cPS2MemCard_GetDeviceName).
 *
 * The PS2 card has no custom device names (slot 72 returns 0), so its raw name is its display name;
 * cPS2MemCard_GetDeviceName reads only the port and the buffer. It is word 143 (slot 71) of cPS2MemCard_vtbl,
 * cMemCard::GetDeviceNameRaw.
 *
 * @param pS2MemCard  the memory card
 * @param device  the port
 * @param dst  the 16-bit buffer for the name
 * @param dstLen  the buffer's length, passed on
 */
extern "C" void cPS2MemCard_GetDeviceNameRaw(cPS2MemCard* pS2MemCard, int device, unsigned short* dst, int dstLen) {
    ((cMemCard*)pS2MemCard)->GetDeviceName(device, dst, dstLen);
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
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_IsFileInfoValid);
#ifdef SKIP_ASM
/**
 * Returns mFileInfoValid, the outcome flag of the last card operation (cPS2MemCard_Update sets it to 0 or 1).
 *
 * It is word 151 (slot 75) of cPS2MemCard_vtbl.
 *
 * @param pS2MemCard  the memory card
 * @return mFileInfoValid
 */
extern "C" int cPS2MemCard_IsFileInfoValid(cPS2MemCard* pS2MemCard) {
    return (pS2MemCard->mFileInfoValid);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetLastError1);
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
extern "C" void cPS2MemCard_GetLastError1(cPS2MemCard* memCard) {
    cPS2MemCard_GetLastError_39B8(memCard, (memCard->mDevice));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetState);
#ifdef SKIP_ASM
/**
 * Returns the memory card's state word mState (+4).
 *
 * Nothing in the build calls it.
 *
 * @param memCard  the memory card
 * @return mState
 */
extern "C" int cPS2MemCard_GetState(cPS2MemCard* memCard) {
    return (memCard->mState);
}
#endif

/**
 * Does nothing. Nothing in the build references it.
 */
extern "C" void cPS2MemCard_UnMount(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetNumDirTable);
#ifdef SKIP_ASM
/**
 * Returns the number of directory entries read from the card, mNumDirTable (+0x54).
 *
 * Nothing in the build calls it.
 *
 * @param memCard  the memory card
 * @return mNumDirTable
 */
extern "C" int cPS2MemCard_GetNumDirTable(cPS2MemCard* memCard) {
    return (memCard->mNumDirTable);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cPS2MemCard_GetResult);
#ifdef SKIP_ASM
extern int D_004A3940;

/**
 * Returns D_004A3940, the static word where the memory-card code keeps the last libmc result.
 *
 * Nothing in the build calls it.
 *
 * @return D_004A3940
 */
extern "C" int cPS2MemCard_GetResult(void) {
    return D_004A3940;
}
#endif

/**
 * Does nothing: the base menu item's word 17 (slot 8), which the menu-item vtables without an override share.
 *
 * cIntMenuItem_vtbl, cFloatMenuItem_vtbl and cCharMenuItem_vtbl put their own functions in that slot.
 */
extern "C" void cMenuItem_zeroValue(void) {
}

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_setID);
#ifdef SKIP_ASM
/**
 * Sets a menu item's ID (+8).
 *
 * @param menuItem  the menu item
 * @param itemId  the new ID
 */
extern "C" void cMenuItem_setID(cMenuItem* menuItem, int itemId) {
    menuItem->ID = itemId;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_getID);
#ifdef SKIP_ASM
/**
 * Returns a menu item's ID (+8).
 *
 * @param menuItem  the menu item
 * @return the ID
 */
extern "C" int cMenuItem_getID(cMenuItem* menuItem) {
    return (menuItem->ID);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_setArg);
#ifdef SKIP_ASM
/**
 * Sets a menu item's Arg word (+0xC).
 *
 * @param menuItem  the menu item
 * @param itemArg  the new value
 */
extern "C" void cMenuItem_setArg(cMenuItem* menuItem, int itemArg) {
    menuItem->Arg = (void*)itemArg;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_getArg);
#ifdef SKIP_ASM
/**
 * Returns a menu item's Arg word (+0xC).
 *
 * @param menuItem  the menu item
 * @return Arg
 */
extern "C" int cMenuItem_getArg(cMenuItem* menuItem) {
    return ((int)menuItem->Arg);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_getMenu);
#ifdef SKIP_ASM
/**
 * Returns the menu a menu item belongs to (Menu, +0; cMenu_addItem fills it).
 *
 * @param menuItem  the menu item
 * @return the menu
 */
extern "C" int cMenuItem_getMenu(int *menuItem) {
    return *menuItem;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenu_getItem);
#ifdef SKIP_ASM
/**
 * Returns one of a menu's items: Items[@p itemIndex] (+0xC + 4 * index). The index is not checked.
 *
 * @param menu  the menu
 * @param itemIndex  the item index
 * @return the item
 */
extern "C" int cMenu_getItem(cMenu* menu, int itemIndex) {
    return (int)menu->Items[itemIndex];
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenu_getNumItems);
#ifdef SKIP_ASM
/**
 * Returns the number of items in a menu (NumItems, +0).
 *
 * @param menu  the menu
 * @return NumItems
 */
extern "C" int cMenu_getNumItems(int *menu) {
    return *menu;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenu_getMenuSys);
#ifdef SKIP_ASM
/**
 * Returns the menu system that shows a menu (MenuSys, +0x124; cMenu_onGainFocus sets it).
 *
 * @param menu  the menu
 * @return MenuSys
 */
extern "C" int cMenu_getMenuSys(cMenu* menu) {
    return ((int)menu->MenuSys);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenu_getFont);
#ifdef SKIP_ASM
/**
 * Returns the font of a menu's menu system: the word at +0x58 of MenuSys (+0x124).
 *
 * @param menu  the menu
 * @return the font
 */
extern "C" int cMenu_getFont(cMenu* menu) {
    return (int)menu->MenuSys->Font;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cMenuItem_getFont);
#ifdef SKIP_ASM
/**
 * Returns the font of the menu system that shows a menu item: the word at +0x58 of the MenuSys (+0x124) of the item's
 * Menu (+0).
 *
 * @param menuItem  the menu item
 * @return the font
 */
extern "C" int cMenuItem_getFont(cMenuItem* menuItem) {
    return (int)menuItem->Menu->MenuSys->Font;
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
INCLUDE_ASM("seg/seg_1D3510", cBoolMenuItem_setValue);
#ifdef SKIP_ASM
/**
 * Sets the flag an on/off menu item toggles (the flag pointer, +0x20).
 *
 * @param menuItem  the on/off item
 * @param value  the flag's address
 */
extern "C" void cBoolMenuItem_setValue(cBoolMenuItem* menuItem, int value) {
    menuItem->Flag = (int*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cBoolMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets an on/off menu item's caption (+0x14).
 *
 * @param menuItem  the on/off item
 * @param value  the caption text
 */
extern "C" void cBoolMenuItem_setCaption(cBoolMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cEnumMenuItem_setEnumTags);
#ifdef SKIP_ASM
/**
 * Sets the entries a list-choice menu item offers: the entry count (+0x18) and the table of {name, value} entries
 * (+0x1C).
 *
 * @param menuItem  the list-choice item
 * @param numTags  the number of entries
 * @param tagTable  the entries, 8 bytes each
 */
extern "C" void cEnumMenuItem_setEnumTags(cEnumMenuItem* menuItem, int numTags, int tagTable) {
    menuItem->NumTags = numTags;
    menuItem->Tags = (const tCFMEnumTag*)tagTable;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cEnumMenuItem_setEnum);
#ifdef SKIP_ASM
/**
 * Sets the int a list-choice menu item sets (the value pointer, +0x20).
 *
 * @param menuItem  the list-choice item
 * @param value  the int's address
 */
extern "C" void cEnumMenuItem_setEnum(cEnumMenuItem* menuItem, int value) {
    menuItem->Enum = (int*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cEnumMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a list-choice menu item's caption (+0x14).
 *
 * @param menuItem  the list-choice item
 * @param value  the caption text
 */
extern "C" void cEnumMenuItem_setCaption(cEnumMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cEnumMenuItem_getCaption);
#ifdef SKIP_ASM
/**
 * Returns a list-choice menu item's caption (+0x14).
 *
 * @param menuItem  the list-choice item
 * @return the caption
 */
extern "C" int cEnumMenuItem_getCaption(cEnumMenuItem* menuItem) {
    return ((int)menuItem->Caption);
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
INCLUDE_ASM("seg/seg_1D3510", cSubMenuItem_setMenu);
#ifdef SKIP_ASM
/**
 * Sets the sub-menu a sub-menu item opens (+0x18).
 *
 * @param menuItem  the sub-menu item
 * @param value  the sub-menu
 */
extern "C" void cSubMenuItem_setMenu(cSubMenuItem* menuItem, int value) {
    menuItem->Sub = (cMenu*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cSubMenuItem_getSubMenu);
#ifdef SKIP_ASM
/**
 * Returns the sub-menu a sub-menu item opens (+0x18).
 *
 * @param menuItem  the sub-menu item
 * @return the sub-menu
 */
extern "C" int cSubMenuItem_getSubMenu(cSubMenuItem* menuItem) {
    return ((int)menuItem->Sub);
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
INCLUDE_ASM("seg/seg_1D3510", cPopMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a "back" menu item's caption (+0x14).
 *
 * @param menuItem  the "back" item
 * @param value  the caption text
 */
extern "C" void cPopMenuItem_setCaption(cPopMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem_setStateText);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's state text (+0x18), shown next to its caption.
 *
 * @param menuItem  the command item
 * @param value  the text
 */
extern "C" void cCommandMenuItem_setStateText(cCommandMenuItem* menuItem, int value) {
    menuItem->State = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's caption (+0x14).
 *
 * @param menuItem  the command item
 * @param value  the caption text
 */
extern "C" void cCommandMenuItem_setCaption(cCommandMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem_getCaption);
#ifdef SKIP_ASM
/**
 * Returns a command menu item's caption (+0x14).
 *
 * @param menuItem  the command item
 * @return the caption
 */
extern "C" int cCommandMenuItem_getCaption(cCommandMenuItem* menuItem) {
    return ((int)menuItem->Caption);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem_setData);
#ifdef SKIP_ASM
/**
 * Sets a command menu item's data word (+0x20).
 *
 * @param menuItem  the command item
 * @param value  the data
 */
extern "C" void cCommandMenuItem_setData(cCommandMenuItem* menuItem, int value) {
    menuItem->Data = (void*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCommandMenuItem_getData);
#ifdef SKIP_ASM
/**
 * Returns a command menu item's data word (+0x20).
 *
 * @param menuItem  the command item
 * @return the data
 */
extern "C" int cCommandMenuItem_getData(cCommandMenuItem* menuItem) {
    return ((int)menuItem->Data);
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
INCLUDE_ASM("seg/seg_1D3510", cCallbackMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a callback menu item's caption (+0x14).
 *
 * @param menuItem  the callback item
 * @param value  the caption text
 */
extern "C" void cCallbackMenuItem_setCaption(cCallbackMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCallbackMenuItem_getCaption);
#ifdef SKIP_ASM
/**
 * Returns a callback menu item's caption (+0x14).
 *
 * @param menuItem  the callback item
 * @return the caption
 */
extern "C" int cCallbackMenuItem_getCaption(cCallbackMenuItem* menuItem) {
    return ((int)menuItem->Caption);
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
INCLUDE_ASM("seg/seg_1D3510", cNullMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a text-only menu item's text (+0x14).
 *
 * @param menuItem  the text-only item
 * @param value  the text
 */
extern "C" void cNullMenuItem_setCaption(cNullMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cNullMenuItem_setStateText);
#ifdef SKIP_ASM
/**
 * Stores a text pointer at +0x18 of a text-only menu item (cNullMenuItem_cNullMenuItem does not set this word).
 *
 * @param menuItem  the text-only item
 * @param value  the text
 */
extern "C" void cNullMenuItem_setStateText(cNullMenuItem* menuItem, int value) {
    menuItem->State = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cCharMenuItem_setValue);
#ifdef SKIP_ASM
/**
 * Sets the signed byte a signed-byte menu item edits (the value pointer, +0x18).
 *
 * @param menuItem  the signed-byte item
 * @param value  the byte's address
 */
extern "C" void cCharMenuItem_setValue(cCharMenuItem* menuItem, int value) {
    menuItem->Value = (signed char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cCharMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a signed-byte menu item's caption (+0x14).
 *
 * @param menuItem  the signed-byte item
 * @param value  the caption text
 */
extern "C" void cCharMenuItem_setCaption(cCharMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cIntMenuItem_setValue);
#ifdef SKIP_ASM
/**
 * Sets the int an int menu item edits (the variable's address, +0x18).
 *
 * @param menuItem  the int item
 * @param value  the int's address
 */
extern "C" void cIntMenuItem_setValue(cIntMenuItem* menuItem, int value) {
    menuItem->Value = (int*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cIntMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets an int menu item's caption (+0x14).
 *
 * @param menuItem  the int item
 * @param value  the caption text
 */
extern "C" void cIntMenuItem_setCaption(cIntMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_setValue);
#ifdef SKIP_ASM
/**
 * Sets the float a float menu item edits (the variable's address, +0x18).
 *
 * @param menuItem  the float item
 * @param value  the float's address
 */
extern "C" void cFloatMenuItem_setValue(cFloatMenuItem* menuItem, int value) {
    menuItem->Value = (float*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_setMinMax);
#ifdef SKIP_ASM
/**
 * Sets a float menu item's range: minimum (+0x1C) and maximum (+0x20).
 *
 * @param menuItem  the float item
 * @param fparg0  the minimum
 * @param fparg1  the maximum
 */
extern "C" void cFloatMenuItem_setMinMax(cFloatMenuItem* menuItem, float fparg0, float fparg1) {
    menuItem->Min = fparg0;
    menuItem->Max = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a float menu item's caption (+0x14).
 *
 * @param menuItem  the float item
 * @param value  the caption text
 */
extern "C" void cFloatMenuItem_setCaption(cFloatMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_setValueScale);
#ifdef SKIP_ASM
/**
 * Sets how a float menu item shows its value: the display scale (+0x24) and offset (+0x28); the item shows scale *
 * value + offset.
 *
 * @param menuItem  the float item
 * @param fparg0  the scale
 * @param fparg1  the offset
 */
extern "C" void cFloatMenuItem_setValueScale(cFloatMenuItem* menuItem, float fparg0, float fparg1) {
    menuItem->ValueScale = fparg0;
    menuItem->ValueOffset = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_setValue1);
#ifdef SKIP_ASM
/**
 * Writes a new value into the float a float menu item edits (through the value pointer, +0x18).
 *
 * The value is stored as given, without the display scale and offset.
 *
 * @param menuItem  the float item
 * @param fparg0  the new value
 */
extern "C" void cFloatMenuItem_setValue1(cFloatMenuItem* menuItem, float fparg0) {
    *(menuItem->Value) = fparg0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cFloatMenuItem_getValue);
#ifdef SKIP_ASM
/**
 * Returns a float menu item's value as displayed: scale (+0x24) * the edited float (through +0x18) + offset (+0x28).
 *
 * @param menuItem  the float item
 * @return the displayed value
 */
extern "C" float cFloatMenuItem_getValue(cFloatMenuItem* menuItem) {
    return ((menuItem->ValueScale) * *(menuItem->Value)) + (menuItem->ValueOffset);
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
INCLUDE_ASM("seg/seg_1D3510", cAngleMenuItem_setValue);
#ifdef SKIP_ASM
/**
 * Sets the float an angle menu item edits (the value pointer, +0x18).
 *
 * @param menuItem  the angle item
 * @param value  the float's address
 */
extern "C" void cAngleMenuItem_setValue(cAngleMenuItem* menuItem, int value) {
    menuItem->Value = (float*)value;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cAngleMenuItem_setMinMax);
#ifdef SKIP_ASM
/**
 * Sets an angle menu item's range: minimum (+0x1C) and maximum (+0x20).
 *
 * @param menuItem  the angle item
 * @param fparg0  the minimum
 * @param fparg1  the maximum
 */
extern "C" void cAngleMenuItem_setMinMax(cAngleMenuItem* menuItem, float fparg0, float fparg1) {
    menuItem->Min = fparg0;
    menuItem->Max = fparg1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cAngleMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets an angle menu item's caption (+0x14).
 *
 * @param menuItem  the angle item
 * @param value  the caption text
 */
extern "C" void cAngleMenuItem_setCaption(cAngleMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cTextMenuItem_gettext);
#ifdef SKIP_ASM
/**
 * Returns a text-entry menu item's text: the address of its 16-byte buffer (+0x18).
 *
 * @param menuItem  the text-entry item
 * @return the buffer address
 */
extern "C" int cTextMenuItem_gettext(int menuItem) {
    return menuItem + 0x18;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cTextMenuItem_settext);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int strncat_3D48(char*, int, int) __asm__("strncat");
/**
 * Sets a text-entry menu item's text: clears the 16-byte buffer at +0x18 and copies @p text into it with strncat
 * (at most 15 characters).
 *
 * @param menuItem  the text-entry item
 * @param text  the new text
 * @return what strncat returns
 */
extern "C" int cTextMenuItem_settext(cTextMenuItem* menuItem, int text) {
    char *textBuf = menuItem->Text;
    *textBuf = 0;
    return strncat_3D48(textBuf, text, 0xF);
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
INCLUDE_ASM("seg/seg_1D3510", cRGBTitleMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets the title text of a colour editor's title row (+0x18).
 *
 * @param menuItem  the title row
 * @param value  the title text
 */
extern "C" void cRGBTitleMenuItem_setCaption(cRGBTitleMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cARGBMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a colour editor menu item's caption: the title text (+0x18) of its embedded title row (+0x640), i.e. +0x658.
 *
 * @param menuItem  the colour editor item
 * @param value  the caption text
 */
extern "C" void cARGBMenuItem_setCaption(cARGBMenuItem* menuItem, int value) {
    menuItem->TitleItem.Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cVectorTitleMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a vector title menu item's caption (+0x18).
 *
 * @param menuItem  the vector title item
 * @param value  the caption text
 */
extern "C" void cVectorTitleMenuItem_setCaption(cVectorTitleMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cVectorMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a vector editor menu item's caption: the caption (+0x18) of its embedded title row (+0x654), i.e. +0x66C.
 *
 * @param menuItem  the vector editor item
 * @param value  the caption text
 */
extern "C" void cVectorMenuItem_setCaption(cVectorMenuItem* menuItem, int value) {
    menuItem->TitleItem.Caption = (const char*)value;
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
INCLUDE_ASM("seg/seg_1D3510", cGraphMenuItem_setCaption);
#ifdef SKIP_ASM
/**
 * Sets a graph menu item's caption (+0x14).
 *
 * @param menuItem  the graph item
 * @param value  the caption text
 */
extern "C" void cGraphMenuItem_setCaption(cGraphMenuItem* menuItem, int value) {
    menuItem->Caption = (const char*)value;
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
 * 3 (slot 1) of cErrorMenu_vtbl, the vtable cErrorMenu_cErrorMenu installs.
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
extern void* cConsoleConfig_vtbl[];
/**
 * Destroys the PS2 console configuration object.
 *
 * Nothing of its own to release: stores the cConsoleConfig vtable cConsoleConfig_vtbl back at +0x34 (where the vptr sits) and
 * frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPS2ConsoleConfig__cPS2ConsoleConfig(cPS2ConsoleConfig* self, int flags) {
    self->mVtbl = cConsoleConfig_vtbl;
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
 * cMenu_sortItems stores its comparison function in D_004A54BC and passes this function to the qsort qsort. The
 * C declares no return value; the comparison's result is left in the return register.
 *
 * @param lhsItem  points to the first item pointer
 * @param rhsItem  points to the second item pointer
 */
extern "C" void func_002D4008(int *lhsItem, int *rhsItem) {
    D_004A54BC(*lhsItem, *rhsItem);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1D3510", cConsoleConfig__cConsoleConfig);
#ifdef SKIP_ASM
void operator_delete(int *);
extern void* cConsoleConfig_vtbl[];
/**
 * Destroys a console configuration object: stores the vtable cConsoleConfig_vtbl at +0x34 and frees the object when bit 0 of
 * @p flags is set.
 *
 * It is word 3 (slot 1) of cConsoleConfig_vtbl. Its code is the same as cPS2ConsoleConfig__cPS2ConsoleConfig.
 *
 * @param flags  bit 0: free the memory
 */
extern "C" void cConsoleConfig__cConsoleConfig(cConsoleConfig* self, int flags) {
    self->mVtbl = cConsoleConfig_vtbl;
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
 * It is word 24 of the global constructor table __CTOR_LIST__ (walked by __do_global_ctors).
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
 * It is word 25 of the global constructor table __CTOR_LIST__ (walked by __do_global_ctors).
 */
extern "C" void func_002D48D0(void) {
    func_002D40C0(1, 0xFFFF);
}
#endif
