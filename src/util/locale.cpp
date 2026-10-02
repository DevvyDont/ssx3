#include "common.h"

//100%
INCLUDE_ASM("util/locale", cFELocale_addFile);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is probably the game's overloaded operator new(size, tag, flags, align). Calling it as
// operator new gives gcc's malloc-like (REG_NOALIAS) aliasing that the target shows. On PC use a real operator new.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void func_002C6E98(void* file, const char* name, signed char lang);
extern char D_004603F0[];
extern const char* D_00441138[];
extern signed char D_00441188[];

struct cFELocale_sFile {
    signed char lang;
    char pad1[3];
    int f4;
    int f8;
    int fC;
    int f10;
};

struct cFELocale_sFiles {
    cFELocale_sFile* items[4];
};

extern "C" void cFELocale_addFile(void* self, int lang)
{
    signed char i = lang;
    if (i >= 0) {
        cFELocale_sFiles* arr = (cFELocale_sFiles*)((char*)self + 8);
        if (arr->items[i] == 0) {
            cFELocale_sFile* f = (cFELocale_sFile*)operator new(0x14, D_004603F0, 0, 0);
            arr->items[i] = f;
            f->lang = -1;
            f->f4 = 0;
            f->fC = 0;
            func_002C6E98(f, D_00441138[i], D_00441188[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195A50);
#ifdef SKIP_ASM
extern "C" void func_002C6D70(void*, int);

struct s195A50Arr {
    void* items[4];
};

extern "C" void func_00195A50(void* self, signed char i)
{
    s195A50Arr* arr = (s195A50Arr*)((char*)self + 8);
    if (arr->items[i] != 0) {
        func_002C6D70(arr->items[i], 3);
        arr->items[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195AA0);
#ifdef SKIP_ASM
extern "C" void func_00195948(void* self);
extern "C" void func_00195BE0(void* self);
extern "C" void func_00195DE0(void* self);
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195AA0(void* self, signed char lang)
{
    if (*(signed char*)((char*)self + 0x1) != lang) {
        *(signed char*)((char*)self + 0x1) = lang;
        int flags = *(signed char*)((char*)self + 0x3);
        if ((flags & 1) == 0) {
            func_00195948(self);
            int l = -1;
            switch (lang) {
            case 0:
                l = 0;
                break;
            case 1:
                l = 1;
                break;
            case 2:
                l = 2;
                break;
            case 3:
                l = 3;
                break;
            }
            cFELocale_addFile(self, l);
            if (*(signed char*)((char*)self + 0x58) == 1) {
                func_00195BE0(self);
            } else {
                func_00195DE0(self);
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195B70);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195B70(void* self)
{
    int lang = -1;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        lang = 0;
        break;
    case 1:
        lang = 1;
        break;
    case 2:
        lang = 2;
        break;
    case 3:
        lang = 3;
        break;
    }
    cFELocale_addFile(self, lang);
}
#endif

INCLUDE_ASM("util/locale", func_00195BE0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195D50);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, signed char i);

extern "C" void func_00195D50(void* self)
{
    int flags = *(signed char*)((char*)self + 0x3);
    if ((flags & 1) == 0) {
        func_00195A50(self, 4);
        func_00195A50(self, 5);
        func_00195A50(self, 6);
        func_00195A50(self, 7);
        func_00195A50(self, 0x10);
        func_00195A50(self, 0x11);
        func_00195A50(self, 0x12);
        func_00195A50(self, 0x13);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195DE0);
#ifdef SKIP_ASM
extern "C" void cFELocale_addFile(void* self, int lang);

extern "C" void func_00195DE0(void* self)
{
    int file = -1;
    *(signed char*)((char*)self + 0x58) = 2;
    switch (*(signed char*)((char*)self + 0x1)) {
    case 0:
        file = 0xC;
        break;
    case 1:
        file = 0xD;
        break;
    case 2:
        file = 0xE;
        break;
    case 3:
        file = 0xF;
        break;
    }
    cFELocale_addFile(self, file);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("util/locale", func_00195E58);
#ifdef SKIP_ASM
extern "C" void func_00195A50(void* self, signed char i);

extern "C" void func_00195E58(void* self)
{
    int flags = *(signed char*)((char*)self + 3);
    if ((flags & 1) == 0) {
        func_00195A50(self, 0xC);
        func_00195A50(self, 0xD);
        func_00195A50(self, 0xE);
        func_00195A50(self, 0xF);
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195EB8);
#ifdef SKIP_ASM
extern "C" const char* func_002C6DE0(void* file, int key);
extern "C" void func_002C27C0(char* dst, const char* src);
extern char D_004A1390[];

struct func_00195EB8_sLocale {
    signed char useDefault;
    signed char lang;
    char pad2[6];
    signed char* files[20];
};

extern "C" const char* func_00195EB8(func_00195EB8_sLocale* self, int key)
{
    if (self->useDefault == 0) {
        for (signed char i = 0; i < 20; i++) {
            signed char* f = self->files[i];
            if (f != 0 && *f == self->lang) {
                const char* s = func_002C6DE0(f, key);
                if (s != 0) {
                    return s;
                }
            }
        }
    }
    char* buf = (char*)self + 0x5A;
    func_002C27C0(buf, D_004A1390);
    return buf;
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00195F70);
#ifdef SKIP_ASM
// PORT: func_002C6DC0__FPv really takes a second argument (it passes $5 through to
// func_003DBB68); bind the 2-arg view to the 1-arg mangled symbol.
void* func_002C6DC0_2(void* self, void* arg) __asm__("func_002C6DC0__FPv");

struct func_00195F70_sArr {
    void* items[20];
};

extern "C" void* func_00195F70(void* self, void* arg)
{
    func_00195F70_sArr* arr = (func_00195F70_sArr*)((char*)self + 8);
    signed char i;
    for (i = 0; i < 20; i++) {
        if (arr->items[i] != 0) {
            void* r = func_002C6DC0_2(arr->items[i], arg);
            if (r != 0) {
                return r;
            }
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("util/locale", func_00195FF0);

//100%
INCLUDE_ASM("util/locale", func_00196148);
#ifdef SKIP_ASM
int GetHashValue32(char*);
extern char D_004A18A8[];
extern char* D_004A28A8;
extern char D_00460400[];
extern char D_00460410[];
class func_00196228_cObj;
extern "C" func_00196228_cObj* func_0039F9D8(void* list, int hash);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void* func_0028B180();
extern "C" void func_0028F140(void* self, int a1);

class func_00196148_cObj {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
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
    virtual int v24(int a, int b);
};

extern "C" void func_00196148(void* self)
{
    char* name;
    if (*(void**)(D_004A28A8 + 0x84) != 0) {
        name = D_00460400;
        func_00196148_cObj* obj = (func_00196148_cObj*)func_0039F9D8(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(D_004A18A8));
        if (obj != 0) {
            obj->v24(3, 0);
        }
    } else {
        name = D_00460410;
        func_0028F140(func_0028B180(), 4);
    }
    void* engine = *(void**)((char*)self + 0x10);
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(name), 0);
    *(void**)((char*)self + 0x40) = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("util/locale", func_00196228);
#ifdef SKIP_ASM
int GetHashValue32(char*);
void* func_0039E4A0(void* self);
extern char D_004A18A8[];
extern char* D_004A28A8;

class func_00196228_cObj {
public:
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
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
    virtual int v24(int a, int b);
};

extern "C" func_00196228_cObj* func_0039F9D8(void* list, int hash);

extern "C" void func_00196228(void* self)
{
    if (*(void**)(D_004A28A8 + 0x84) != 0) {
        func_00196228_cObj* obj = func_0039F9D8(*(char**)((char*)self + 0x10) + 0x18, GetHashValue32(D_004A18A8));
        if (obj != 0) {
            obj->v24(4, 0);
        }
    }
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("util/locale", func_001962A8);
#ifdef SKIP_ASM
struct func_001962A8_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void func_001962A8_show(char* o, int on)
{
    func_001962A8_sVEntry* vt = *(func_001962A8_sVEntry**)(o + 8);
    vt[9].fn(o + vt[9].delta, on);
}

extern "C" int func_001962A8(void* self, int a1)
{
    if (a1 != 0 && *(int*)((char*)self + 0x7C) == 0) {
        func_001962A8_show(*(char**)((char*)self + 0x68), 0);
        func_001962A8_show(*(char**)((char*)self + 0x6C), 0);
        func_001962A8_show(*(char**)((char*)self + 0x50), 0);
        func_001962A8_show(*(char**)((char*)self + 0x58), 0);
        if (*(int*)((char*)self + 0x74) == 0) {
            func_001962A8_show(*(char**)((char*)self + 0x64), 0);
        }
    }
    return 0;
}
#endif

INCLUDE_ASM("util/locale", func_00196378);

