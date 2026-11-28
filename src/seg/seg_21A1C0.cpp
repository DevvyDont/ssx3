#include "common.h"
#include "bx/bxstring.h"
#include "bx/dynamicmemman.h"

//100%
INCLUDE_ASM("seg/seg_21A1C0", cBXString_vprintf);
#ifdef SKIP_ASM
extern "C" long strtol(const char* s, char** end, int base);
extern "C" int strlen(const char* s);
extern "C" void cBXString_getBuffer(cBXString* self, int len);
extern "C" int vsprintf(char* buf, const char* fmt, char* ap);
extern "C" void cBXString_releaseBuffer(cBXString* self, int len);

#define MAX_91C0(a, b) (((a) > (b)) ? (a) : (b))
#define MIN_91C0(a, b) (((a) < (b)) ? (a) : (b))

/**
 * Formats text into a string, printf style, from an argument list (vsprintf into a cBXString).
 *
 * It first estimates the length: one per plain character, and per conversion its flags ('#' adds 2), width and
 * precision (also from '*' arguments), then 2 for %c, the string length (at least 1) for %s, 128 for e/f/g/G and 32 for
 * integer and pointer conversions, limited by width and precision. It reserves that much (cBXString_getBuffer), formats
 * with vsprintf (vsprintf) and lets cBXString_releaseBuffer(-1) take the length from the text.
 *
 * Each argument is taken as 8 bytes (the EE's va_list slots).
 *
 * @param self_  the string (cBXString)
 * @param fmt  the printf format
 * @param args  the argument list
 */
// vsprintf into a cBXString: estimates the formatted length first (the MFC CString::FormatV
// scheme), reserves it, then formats in place.
void cBXString::vprintf(const char* fmt, char* args)
{
    cBXString* self = (cBXString*)this;
    char* ap = args;
    int maxLen = 1;
    const char* fmtPos;
    for (fmtPos = fmt; *fmtPos != 0; fmtPos++) {
        if (*fmtPos != '%' || *++fmtPos == '%') {
            maxLen++;
            continue;
        }
        int width = 0;
        int widthArg = 0;
        for (; *fmtPos != 0; fmtPos++) {
            if (*fmtPos == '#')
                maxLen += 2;
            else if (*fmtPos == '*') {
                ap += 8;
                widthArg = 1;
                width = *(int*)(ap - 8);
            } else if (*fmtPos == '-' || *fmtPos == '+' || *fmtPos == '0' || *fmtPos == ' ')
                ;
            else
                break;
        }
        if (!widthArg) {
            char* end;
            width = strtol(fmtPos, &end, 10);
            fmtPos = end;
        }
        int precision = 0;
        if (*fmtPos == '.') {
            fmtPos++;
            if (*fmtPos == '*') {
                ap += 8;
                precision = *(int*)(ap - 8);
                fmtPos++;
            } else {
                char* end;
                precision = strtol(fmtPos, &end, 10);
                fmtPos = end;
            }
        }
        switch (*fmtPos) {
        case 'h':
        case 'l':
        case 'F':
        case 'N':
        case 'L':
            fmtPos++;
            break;
        }
        int itemLen = 0;
        switch (*fmtPos) {
        case 'c':
        case 'C':
            itemLen = 2;
            ap += 8;
            break;
        case 's':
        case 'S':
            ap += 8;
            itemLen = strlen(*(char**)(ap - 8));
            itemLen = MAX_91C0(1, itemLen);
            break;
        }
        if (itemLen != 0) {
            itemLen = MAX_91C0(itemLen, width);
            if (precision != 0)
                itemLen = MIN_91C0(itemLen, precision);
        } else {
            switch (*fmtPos) {
            case 'e':
            case 'f':
            case 'g':
            case 'G':
                ap += 8;
                itemLen = 128;
                itemLen = MAX_91C0(itemLen, width + precision);
                break;
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            case 'o':
                ap += 8;
                itemLen = 32;
                itemLen = MAX_91C0(itemLen, width + precision);
                break;
            case 'p':
                ap += 8;
                itemLen = 32;
                itemLen = MAX_91C0(itemLen, width + precision);
                break;
            case 'n':
                ap += 8;
                break;
            }
        }
        maxLen += itemLen;
    }
    cBXString_getBuffer(self, maxLen);
    vsprintf(self->Str, fmt, args);
    cBXString_releaseBuffer(self, -1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", cBXString_printf);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cBXString_vprintf(cBXString* str, const char* fmt, char* ap);

/**
 * Formats text into a string, printf style (sprintf into a cBXString).
 *
 * Builds the argument list from its variadic arguments and calls cBXString_vprintf.
 *
 * @param str  the string (cBXString)
 * @param fmt  the printf format, followed by its arguments
 */
// sprintf into a cBXString.
// PORT: SN-specific va_start (gcc 2.95 EE EABI form); use <stdarg.h> off-PS2.
extern "C" void cBXString_printf(void* str, const char* fmt, ...)
{
    char* ap;
    ap = (char*)__builtin_next_arg() - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0);
    ((cBXString*)str)->vprintf(fmt, ap);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", cBXString_trimRight);
#ifdef SKIP_ASM
extern "C" void cBXString_copyBeforeWrite(void* self);
extern unsigned char D_00499C81[];

/**
 * Removes trailing white space from a string.
 *
 * Makes the text unshared (cBXString_copyBeforeWrite), finds where the last run of white space (bit 3 of the ctype
 * table D_00499C81) starts, cuts the text there and sets the header's DataLength (text - 8). No retail code calls it.
 */
// Strips trailing white space (D_00499C81 is the ctype table + 1, bit 8 = space).
void cBXString::trimRight()
{
    cBXString_copyBeforeWrite(this);
    char* end = 0;
    char* scanPos;
    for (scanPos = Str; *scanPos != 0; scanPos++) {
        if (end == 0)
            end = scanPos;
        if (!(D_00499C81[(int)*scanPos] & 8))
            end = 0;
    }
    if (end != 0) {
        *end = 0;
        getData()->DataLength = end - Str;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", cBXString_trimLeft);
#ifdef SKIP_ASM
extern "C" void cBXString_copyBeforeWrite(void* self);
extern "C" void* func_0041610C(void* dst, const void* src, int n);
extern unsigned char D_00499C81[];

/**
 * Removes leading white space from a string.
 *
 * Makes the text unshared (cBXString_copyBeforeWrite), skips the leading white space (bit 3 of the ctype table
 * D_00499C81), moves the rest (with its terminator) to the front (func_0041610C) and sets the header's DataLength (text
 * - 8). No retail code calls it.
 */
// Strips leading white space.
void cBXString::trimLeft()
{
    cBXString_copyBeforeWrite(this);
    char* textStart = Str;
    while (D_00499C81[(int)*textStart] & 8)
        textStart++;
    int len = getData()->DataLength - (textStart - Str);
    func_0041610C(Str, textStart, len + 1);
    getData()->DataLength = len;
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_initIop);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00426408(int a);
extern "C" void sceSifInitIopHeap(void);
extern "C" void bxIop_rebootWithDnasImage(void* self);
extern "C" void sceCdInit(int a);
extern "C" void sceCdMmode(int a);
extern "C" void sceFsReset(void);

/**
 * Prepares the IOP at start-up, rebooting it with the DNAS image.
 *
 * Calls func_00426408(0) and sceSifInitIopHeap, reboots the IOP with DNAS271.IMG (bxIop_rebootWithDnasImage), then calls
 * sceCdInit(0), sceCdMmode(2) and sceFsReset. systemInit calls it; @p self is not used.
 */
extern "C" void bxIop_initIop(void* self)
{
    func_00426408(0);
    sceSifInitIopHeap();
    bxIop_rebootWithDnasImage(self);
    sceCdInit(0);
    sceCdMmode(2);
    sceFsReset();
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_getModuleDir);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_pct_sDATA_MODULES[];
extern char str_cdrom0_b[];

/**
 * Writes the IOP module directory, "cdrom0:\DATA\MODULES\", into a buffer.
 *
 * sprintf of str_pct_sDATA_MODULES with str_cdrom0_b. @p self is not used.
 *
 * @param out  the buffer
 */
// Builds the IOP module directory ("cdrom0:\DATA\MODULES\").
extern "C" void bxIop_getModuleDir(void* self, char* out)
{
    sprintf(out, str_pct_sDATA_MODULES, str_cdrom0_b);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_buildModulePath);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strchr(const char* s, int c);
extern "C" char* strcpy(char* d, const char* s);
extern "C" void bxIop_getModuleDir(void* self, char* out);
extern char str_pct_spct_spct_s_c[];
extern char str_1_b[];

/**
 * Builds the full path of an IOP module.
 *
 * A @p name that contains ':' (it names a device) is copied as is; otherwise the path is the module directory
 * (bxIop_getModuleDir), @p name and the ISO version suffix ";1". @p self is not used.
 *
 * @param out  the buffer for the path
 * @param name  the module file name
 */
// Full path of an IOP module: names with a device prefix are used as is, others get
// the module directory and the ";1" ISO version suffix.
extern "C" void bxIop_buildModulePath(void* self, char* out, const char* name)
{
    if (strchr(name, ':') != 0) {
        strcpy(out, name);
    } else {
        char dir[256];
        bxIop_getModuleDir(self, dir);
        sprintf(out, str_pct_spct_spct_s_c, dir, name, str_1_b);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_loadModule);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void bxIop_buildModulePath(void* self, char* out, const char* name);
extern "C" int sceSifLoadModule(const char* path, int argLen, const char* args);

/**
 * Loads an IOP module, retrying while the load fails.
 *
 * Builds the path (bxIop_buildModulePath) and calls sceSifLoadModule(path, @p argLen, @p args) until it returns a non-negative
 * result; a failure is retried when it is -204 (IOP busy) or when @p retry is set, otherwise returned.
 *
 * @param name  the module file name
 * @param retry  non-zero to retry every failure
 * @param argLen  the length of the module arguments
 * @param args  the module arguments
 * @return the module id, or the error code
 */
// Loads an IOP module, retrying while the IOP is busy (-204) or always when 'retry' is set.
extern "C" int bxIop_loadModule(void* self, const char* name, int retry, int argLen, const char* args)
{
    char path[256];
    int result;
    bxIop_buildModulePath(self, path, name);
    do {
        result = sceSifLoadModule(path, argLen, args);
    } while (result < 0 && (retry != 0 || result == -204));
    return result;
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_unloadModule);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void sceSifStopModule(int fd, int a, int b, void* out);
extern "C" void sceSifUnloadModule(int fd);

/**
 * Calls sceSifStopModule(@p fd, 0, 0, a 16-byte scratch buffer) and then sceSifUnloadModule(@p fd).
 *
 * No retail code calls it; @p self is not used.
 *
 * @param fd  the id passed to both calls
 */
extern "C" void bxIop_unloadModule(void* self, int fd)
{
    char scratch[16];
    sceSifStopModule(fd, 0, 0, scratch);
    sceSifUnloadModule(fd);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_loadMsifrpc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int bxIop_loadModule(void* self, const char* name, int retry, int argLen, const char* args);
extern void* D_004A3E94;
extern char str_MSIFRPC_IRX[];

/**
 * Loads the IOP module MSIFRPC.IRX.
 *
 * Calls bxIop_loadModule(D_004A3E94, "MSIFRPC.IRX", retry 1, no arguments). initOnline calls it; @p self is not used.
 */
// Loads MSIFRPC.IRX.
extern "C" void bxIop_loadMsifrpc(void* self)
{
    bxIop_loadModule(D_004A3E94, str_MSIFRPC_IRX, 1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", func_00319800);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0040B1F8(void);

/**
 * Calls func_0040B1F8.
 *
 * No retail code calls it; @p self is not used.
 */
extern "C" void func_00319800(void* self)
{
    func_0040B1F8();
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_rebootWithDnasImage);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00426408(int a);
extern "C" void sceCdInit(int a);
extern "C" void sceCdMmode(int a);
extern "C" void bxIop_buildModulePath(void* self, char* out, const char* name);
extern "C" int sceSifRebootIop(const char* path);
extern "C" int sceSifSyncIop(void);
extern "C" void sceSifInitIopHeap(void);
extern char str_DNAS271_IMG[];

/**
 * Reboots the IOP with the DNAS image DNAS271.IMG and waits until it is done.
 *
 * Calls func_00426408(0), sceCdInit(0) and sceCdMmode(2), builds the image path (bxIop_buildModulePath), retries
 * sceSifRebootIop (the IOP reboot through rom0:UDNL) until it succeeds, polls sceSifSyncIop until it reports done, then
 * calls func_00426408(0) and sceSifInitIopHeap again. bxIop_initIop calls it; @p self is not used.
 */
// Loads the DNAS image (DNAS271.IMG) and waits for it to finish.
extern "C" void bxIop_rebootWithDnasImage(void* self)
{
    char path[256];
    func_00426408(0);
    sceCdInit(0);
    sceCdMmode(2);
    bxIop_buildModulePath(self, path, str_DNAS271_IMG);
    while (sceSifRebootIop(path) == 0)
        ;
    while (sceSifSyncIop() == 0)
        ;
    func_00426408(0);
    sceSifInitIopHeap();
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_powerOffThread);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void WaitSema(int sema);
extern "C" int sceDevctl(void* client, int cmd, int a, int b, int c, int d);
extern "C" int sceCdPowerOff(int* status);
extern char str_dev9x[];

/**
 * Runs a worker thread that, on each semaphore signal, sends the "dev9x" request 0x4402 and then calls sceCdPowerOff
 * until both succeed.
 *
 * Loops forever: waits on @p sema (WaitSema), retries sceDevctl("dev9x", 0x4402, ...) until it succeeds, then
 * retries sceCdPowerOff(&status) until it returns non-zero with status 0. bxIop_installPowerOffHandler starts it as a thread;
 * bxIop_onPowerOff signals the semaphore. The shape is that of the usual PS2 power-off handler (dev9 shutdown, then
 * console power-off), but the callees are not named in this build.
 *
 * @param sema  the semaphore id to wait on
 */
// IOP poll thread: waits for its semaphore, then issues RPC 0x4402 and waits for completion.
extern "C" void bxIop_powerOffThread(int sema)
{
    for (;;) {
        WaitSema(sema);
        while (sceDevctl(str_dev9x, 17410, 0, 0, 0, 0) < 0)
            ;
        int status = 0;
        while (sceCdPowerOff(&status) == 0 || status != 0)
            ;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_onPowerOff);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void iSignalSema(void);

/**
 * Signals the power-off thread's semaphore (iSignalSema).
 *
 * bxIop_installPowerOffHandler installs it as the power-off callback (sceCdPOffCallback); the semaphore id arrives as the callback
 * argument.
 */
extern "C" void bxIop_onPowerOff(void)
{
    iSignalSema();
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", bxIop_installPowerOffHandler);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Sony EE kernel ThreadParam / SemaParam.
struct sThreadParam_9930 {
    int status;
    void (*entry)(int);
    void* stack;
    int stackSize;
    void* gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
    int waitType;
    int waitId;
    int wakeupCount;
};
struct sSemaParam_9930 {
    int count;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

extern "C" int sceSifSearchModuleByName(void* client);
extern "C" int CreateSema(sSemaParam_9930* p);
extern "C" int CreateThread(sThreadParam_9930* p);
extern "C" int StartThread(int thread, int arg);
extern "C" void sceCdPOffCallback(void (*fn)(void), int arg);
extern "C" void bxIop_powerOffThread(int sema);
extern "C" void bxIop_onPowerOff(void);
extern int _gp;
extern char str_dev9[];
extern char D_00538B10[];
extern int D_004A5B54;
extern int D_004A5B70;

/**
 * Sets up a semaphore, a worker thread running bxIop_powerOffThread and a callback (bxIop_onPowerOff) that wakes it.
 *
 * Only when sceSifSearchModuleByName("dev9") succeeds: creates a semaphore (initial count 0, maximum 1; D_004A5B54), creates and
 * starts a thread (priority 1, 0x2000-byte stack D_00538B10, _gp; id in D_004A5B70) on bxIop_powerOffThread with the semaphore
 * as its argument, and registers bxIop_onPowerOff with sceCdPOffCallback as the callback, with the same argument. initOnline
 * calls it.
 */
extern "C" void bxIop_installPowerOffHandler(void)
{
    sThreadParam_9930 tp;
    sSemaParam_9930 sema;
    if (sceSifSearchModuleByName(str_dev9) >= 0) {
        sema.initCount = 0;
        sema.maxCount = 1;
        sema.option = 0;
        D_004A5B54 = CreateSema(&sema);
        tp.stackSize = 0x2000;
        tp.gpReg = &_gp;
        tp.entry = bxIop_powerOffThread;
        tp.stack = D_00538B10;
        tp.initPriority = 1;
        D_004A5B70 = CreateThread(&tp);
        StartThread(D_004A5B70, D_004A5B54);
        sceCdPOffCallback(bxIop_onPowerOff, D_004A5B54);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_21A1C0", cMemMan_totalUnused);
#ifdef SKIP_ASM
extern "C" mallinfo cDynamicMemMan_dlmallinfo(void* ms);
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern void* cMemMan_m_mutex[16];
extern char TheDynamicMemMan[];
extern unsigned int cMemMan_m_heapSize;

/**
 * Returns the free bytes in the dynamic heap.
 *
 * Locks cMemMan_m_mutex, takes cDynamicMemMan_dlmallinfo's mallinfo of TheDynamicMemMan and returns cMemMan_m_heapSize minus its
 * uordblks (the bytes handed out), then unlocks. On Tour calls cDynamicMemMan::totalUsed, which retail inlines.
 *
 * @return the unused heap bytes
 */
// Free heap bytes: heap size minus the bytes dlmalloc has handed out.
extern "C" int cMemMan_totalUnused(void)
{
    MUTEX_lock(cMemMan_m_mutex);
    mallinfo mi = cDynamicMemMan_dlmallinfo(TheDynamicMemMan);
    int freeBytes = cMemMan_m_heapSize - mi.uordblks;
    MUTEX_unlock(cMemMan_m_mutex);
    return freeBytes;
}
#endif
