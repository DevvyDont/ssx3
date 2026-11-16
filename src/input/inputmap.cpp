#include "common.h"
#include "input/inputparse.h"

//100%
INCLUDE_ASM("input/inputmap", cInputMap_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void MEM_fill(void* dst, unsigned int value, int size);
extern char str_InputMapTable[];

/**
 * Initialises an input map with room for a number of compiled statements.
 *
 * Allocates the statement offset table ("InputMapTable", count words) and fills it with -1 (no code yet), and
 * clears the op-code buffer pointer (+0x14), its capacity (+0xC) and its used size (+0x10).
 *
 * @param count  number of statement slots (input state ids)
 */
extern "C" void cInputMap_init(void* self, int count)
{
    *(int*)((char*)self + 0x0) = count;
    *(void**)((char*)self + 0x4) = operator_new_tag(count * 4, str_InputMapTable, 0, 0);
    MEM_fill(*(void**)((char*)self + 0x4), 0xFFFFFFFF, *(int*)((char*)self + 0x0) * 4);
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_00320D08);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_00320D08(void *self)
{
    void *p = *(void **)((char *)self + 4);
    if (p != 0) {
        cMemMan_free(p);
    }
    p = *(void **)((char *)self + 0x14);
    if (p != 0) {
        cMemMan_free(p);
    }
}
#endif
//100%
INCLUDE_ASM("input/inputmap", func_00320D50);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cInputMap_loadMapFile_vvi(void* self, void* data, int size) __asm__("cInputMap_loadMapFile");
void cMemMan_free(void* p);
extern "C" void* func_003E1A10(const char* path, int* size, int a2);

extern "C" void func_00320D50(void* self, const char* path)
{
    int size;
    void* data = func_003E1A10(path, &size, 0x100);
    cInputMap_loadMapFile_vvi(self, data, size);
    if (data != 0) {
        cMemMan_free(data);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_loadMapFile);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cInputMapParser_cInputMapParser(cInputMapParser* self, void* a1, void* a2);
extern char str_InputParser[];

/**
 * Creates the parser for an input-map script that is already in memory.
 *
 * Allocates a cInputMapParser ("InputParser", 0x109EC bytes) over the text and keeps it at +0x8; the parser's
 * constructor parses the whole file. Statements are compiled later with cInputMap_compileMap().
 *
 * @param mem  the map file's text
 * @param len  the text's length in bytes
 */
extern "C" void cInputMap_loadMapFile(void* self, void* mem, void* len)
{
    *(void**)((char*)self + 0x8) = cInputMapParser_cInputMapParser((cInputMapParser*)cMemMan_alloc(0x109EC, str_InputParser, 0x100, 0), mem, len);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_compileMap);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int cInputMapParser_compileStatement(void* parser, char* src, int* dst, int room, int a4);
extern char str_TempOpCodes[];

struct sInputMap_0E18 {
    int f0;
    int* offsets;   // 0x4
    void* parser;   // 0x8
    int cap;        // 0xC
    int used;       // 0x10
    int* code;      // 0x14
};

/**
 * Compiles one named statement of the loaded map file into the map's op-code buffer.
 *
 * First makes sure at least 256 words are free, growing the buffer by 1024 words ("TempOpCodes") and copying
 * the old code across. Then records where the statement's code starts in the offset table and appends what
 * cInputMapParser_compileStatement() emits.
 *
 * @param idx  statement slot (input state id) that receives the code
 * @param src  name of the statement in the map file
 */
extern "C" void cInputMap_compileMap(sInputMap_0E18* self, int idx, char* src)
{
    if (self->cap - self->used < 0x100) {
        int newCap = self->cap + 0x400;
        int* buf = (int*)operator_new_tag(newCap * 4, str_TempOpCodes, 0x100, 0);
        if (self->code != 0) {
            MEM_copy(buf, self->code, self->cap * 4);
            if (self->code != 0) {
                cMemMan_free(self->code);
            }
        }
        self->code = buf;
        self->cap = newCap;
    }
    self->offsets[idx] = self->used;
    self->used += cInputMapParser_compileStatement(self->parser, src, self->code + self->used, self->cap - self->used, 5);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_purgeMapFile);
#ifdef SKIP_ASM
// PORT: operator new with tag args bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void operator_delete(int* p);
void cMemMan_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
extern char str_InputOpCodes[];

/**
 * Frees the map file's parser and trims the op-code buffer to the code actually compiled.
 *
 * Deletes the parser (+0x8), copies the used words into a new buffer of exactly that size ("InputOpCodes"),
 * frees the old one and sets the capacity to the used size.
 */
extern "C" void cInputMap_purgeMapFile(void* self)
{
    if (*(int**)((char*)self + 0x8) != 0) {
        operator_delete(*(int**)((char*)self + 0x8));
    }
    *(int**)((char*)self + 0x8) = 0;
    void* copy = operator_new_tag(*(int*)((char*)self + 0x10) << 2, str_InputOpCodes, 0, 0);
    MEM_copy(copy, *(void**)((char*)self + 0x14), *(int*)((char*)self + 0x10) << 2);
    if (*(void**)((char*)self + 0x14) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x14));
    }
    *(void**)((char*)self + 0x14) = copy;
    *(int*)((char*)self + 0xC) = *(int*)((char*)self + 0x10);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getValue);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

/**
 * Evaluates one compiled map statement against a controller pad and returns its value.
 *
 * Builds a 0x190-byte evaluator on the stack with func_00325250() and runs func_00325450() on the op-code
 * stream at the statement's offset.
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param id  statement slot (input state id) to evaluate
 * @return the statement's value
 */
extern "C" float cInputMap_getValue(void* self, void* inputPad, int id)
{
    char parser[0x190];
    func_00325250(parser);
    return func_00325450(parser, inputPad, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[id] << 2));
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_00321010);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

struct sParser_1010 {
    int f0;                 // 0x0
    char pad4[0x108];
    float v[8];             // 0x10C
    char pad12C[0x190 - 0x12C];
};

extern "C" float func_00321010(void* self, void* a1, int a2, int a3, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7)
{
    sParser_1010 parser;
    func_00325250(&parser);
    parser.f0 = a3;
    parser.v[0] = f0;
    parser.v[1] = f1;
    parser.v[2] = f2;
    parser.v[3] = f3;
    parser.v[4] = f4;
    parser.v[5] = f5;
    parser.v[6] = f6;
    parser.v[7] = f7;
    return func_00325450(&parser, a1, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[a2] << 2));
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getState);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

/**
 * Evaluates one compiled map statement against a controller pad and tells whether it is nonzero.
 *
 * Same evaluation as cInputMap_getValue().
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param id  statement slot (input state id) to evaluate
 * @return 1 when the statement's value is not 0.0, otherwise 0
 */
extern "C" int cInputMap_getState(void* self, void* inputPad, int id)
{
    char parser[0x190];
    func_00325250(parser);
    return func_00325450(parser, inputPad, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[id] << 2)) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_00321188);
#ifdef SKIP_ASM
void* func_00325250(void* self);
extern "C" float func_00325450(void* parser, void* a1, void* a2);

struct sParser_1188 {
    int f0;                 // 0x0
    char pad4[0x108];
    float v[8];             // 0x10C
    char pad12C[0x190 - 0x12C];
};

extern "C" int func_00321188(void* self, void* a1, int a2, int a3, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7)
{
    sParser_1188 parser;
    func_00325250(&parser);
    parser.f0 = a3;
    parser.v[0] = f0;
    parser.v[1] = f1;
    parser.v[2] = f2;
    parser.v[3] = f3;
    parser.v[4] = f4;
    parser.v[5] = f5;
    parser.v[6] = f6;
    parser.v[7] = f7;
    return func_00325450(&parser, a1, (char*)*(void**)((char*)self + 0x14) + ((*(int**)((char*)self + 0x4))[a2] << 2)) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_00321298);
#ifdef SKIP_ASM
struct sBtn_00321298 {
    float value;    // 0x00
    int pressed;    // 0x04
    int released;   // 0x08
    int down;       // 0x0C
    int repeat;     // 0x10
    int timer;      // 0x14
    int settle;     // 0x18
};
struct sBtnSet_00321298 {
    int count;
    sBtn_00321298 b[1];
};

extern "C" void func_00321298(sBtnSet_00321298* self, int n, float* vals)
{
    if (self->count < n) {
        MEM_fill(&self->b[self->count], 0, (n - self->count) * sizeof(sBtn_00321298));
    }
    self->count = n;
    for (int i = 0; i < n; i++) {
        self->b[i].value = vals[i];
        if (self->b[i].settle < 3) {
            self->b[i].pressed = 0;
            self->b[i].settle++;
            self->b[i].released = 0;
        } else {
            int on = self->b[i].value > 0.0f;
            if (on == self->b[i].down) {
                self->b[i].pressed = 0;
                self->b[i].released = 0;
            } else {
                self->b[i].down = on;
                if (on) {
                    self->b[i].pressed = 1;
                    self->b[i].released = 0;
                } else {
                    self->b[i].pressed = 0;
                    self->b[i].released = 1;
                }
                self->b[i].settle = 0;
            }
        }
        if (self->b[i].down) {
            if (self->b[i].timer == 0) {
                self->b[i].repeat = 1;
                self->b[i].timer = 0x18;
            } else {
                self->b[i].timer--;
                if (self->b[i].timer <= 0) {
                    self->b[i].repeat = 1;
                    self->b[i].timer = 0xC;
                } else {
                    self->b[i].repeat = 0;
                }
            }
        } else {
            self->b[i].repeat = 0;
            self->b[i].timer = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_cInputMapParser);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInputMapParser_parseFile(void* self);

struct sMapNode_1428 {
    int f0;
    sMapNode_1428* next;    // 0x4
    int f8;
    int fC;
};

struct sMapData_1428 {
    void* f0;
    void* f4;
    void* f8;
    unsigned char fC;
    char pad_0xD[0x8C - 0xD];
    int f8C;
    int f90;
    int f94;
    char pad_0x98[0xE0 - 0x98];
    sMapNode_1428* freeList;        // 0xE0
    sMapNode_1428 nodes[0x800];     // 0xE4
    int f80E4;
    char pad_0x80E8[0x108EC - 0x80E8];
    int ids[63];                    // 0x108EC
    int numIds;                     // 0x109E8
};

/**
 * Constructs an input-map parser over a script in memory and parses the whole file.
 *
 * Sets the read cursor to the start, LineNumber and IsLineStart to 1, clears the file name, the error flag
 * and the identifier count, pushes registers 63..1 onto FreeRegisters, links all 2048 parse nodes into the
 * free list and then calls cInputMapParser_parseFile().
 *
 * @param vself   the parser to construct
 * @param file    the script text (On Tour const char*)
 * @param length  the text's length in bytes
 * @return the parser
 */
// Initialises a compiled input map: id stack 63..1 and the node free list.
extern "C" void* cInputMapParser_cInputMapParser(cInputMapParser* vself, void* file, void* length)
{
    sMapData_1428* self = (sMapData_1428*)vself;
    self->f4 = length;
    self->f0 = file;
    self->f8 = file;
    self->f80E4 = 0;
    self->f8C = 1;
    self->f90 = 1;
    self->fC = 0;
    self->f94 = 0;
    self->numIds = 0;
    for (int i = 63; i > 0; i--) {
        self->ids[self->numIds++] = i;
    }
    self->freeList = self->nodes;
    for (int j = 1; j < 0x800; j++) {
        self->nodes[j - 1].next = &self->nodes[j];
    }
    self->nodes[0x7FF].next = 0;
    cInputMapParser_parseFile(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_error);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* cInputMapParser_error_va_list;
#define cInputMapParser_error_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" int func_004186C8(char* dst, const char* fmt, char* ap);
extern char str_pct_s_pct_d[];

/**
 * Reports a syntax error in the input-map script and sets the parser's error flag.
 *
 * Formats "FileName(LineNumber): " followed by the printf-style message into a 1 KB local buffer. Nothing
 * prints the buffer in this build, so only ErrorFlag = 1 has an effect.
 *
 * @param fmt  printf-style message, followed by its arguments
 */
extern "C" void cInputMapParser_error(cInputMapParser* self, const char* fmt, ...)
{
    char buf[0x400];
    cInputMapParser_error_va_list ap;
    cInputMapParser_error_va_start(ap);
    func_004186C8(buf + sprintf(buf, str_pct_s_pct_d, (char*)self + 0xC, self->LineNumber), fmt, ap);
    self->ErrorFlag = 1;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_skipPastSemicolon);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(void* self);

/**
 * Skips the rest of a malformed input-map statement, up to and including its semicolon.
 *
 * Reads tokens until the current token is a semicolon (5) or end of file (0), then reads one more.
 * cInputMapParser_parseStatement() calls it after each error it reports.
 */
extern "C" void cInputMapParser_skipPastSemicolon(cInputMapParser* self)
{
    while (self->CurrentToken != 5 && self->CurrentToken != 0) {
        cInputMapParser_readToken(self);
    }
    cInputMapParser_readToken(self);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", func_003215F8);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);

extern "C" void* func_003215F8(void* self)
{
    void* p = *(void**)((char*)self + 0xE0);
    *(void**)((char*)self + 0xE0) = *(void**)((char*)p + 4);
    func_00416210(p, 0, 0x10);
    return p;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_freeParseNode);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sMapNode_1638 {
    int type;                   // 0x0
    sMapNode_1638* a;           // 0x4, also the free-list link
    sMapNode_1638* b;           // 0x8
    sMapNode_1638* c;           // 0xC
};

/**
 * Returns a parse node and all of its child nodes to the parser's free node list.
 *
 * The node type decides how many children are freed first: one for types 18-19 and 24-33, two for 6-17 and
 * 20-21, three for type 5, none for 0-4 and 22-23. The node is then pushed onto FreeParseNodes, linked through
 * its first child field. A null node is ignored.
 *
 * @param node  the node to free, or 0
 */
// Frees an expression node and its children back onto the map's node free list.
extern "C" void cInputMapParser_freeParseNode(cInputMapParser* self, sMapNode_1638* node)
{
    if (node == 0) {
        return;
    }
    switch (node->type) {
    case 18: case 19:
    case 24: case 25: case 26: case 27: case 28:
    case 29: case 30: case 31: case 32: case 33:
        cInputMapParser_freeParseNode(self, node->a);
        break;
    case 6: case 7: case 8: case 9: case 10: case 11:
    case 12: case 13: case 14: case 15: case 16: case 17:
    case 20: case 21:
        cInputMapParser_freeParseNode(self, node->a);
        cInputMapParser_freeParseNode(self, node->b);
        break;
    case 5:
        cInputMapParser_freeParseNode(self, node->a);
        cInputMapParser_freeParseNode(self, node->b);
        cInputMapParser_freeParseNode(self, node->c);
        break;
    case 0: case 1: case 2: case 3: case 4:
    case 22: case 23:
        break;
    }
    node->a = (sMapNode_1638*)self->FreeParseNodes;
    self->FreeParseNodes = (tParseNode*)node;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_lookupButtonName);
#ifdef SKIP_ASM
extern "C" int func_0041AB78(const char* a, const char* b, int n);
extern char str_button[];

/**
 * Parses a button name of the form "buttonN" and returns its number.
 *
 * "button" must be followed by at least one decimal digit; the name is rejected when the digits are followed
 * by a letter or '_', or when the number is 924 or more.
 *
 * @param name  identifier text from the map script
 * @return the button number, or -1 if the name is not a button name
 */
extern "C" int cInputMapParser_lookupButtonName(cInputMapParser* self, char* name)
{
    if (func_0041AB78(name, str_button, 6) != 0) {
        return -1;
    }
    int firstDigit = name[6] - '0';
    if ((unsigned char)firstDigit >= 10) {
        return -1;
    }
    int n = firstDigit;
    char* cursor = name + 7;
    while ((unsigned)(*cursor - '0') < 10) {
        n = n * 10 + (*cursor++ - '0');
    }
    int alpha = 0;
    if ((unsigned)(*cursor - 'a') < 26 || (unsigned)(*cursor - 'A') < 26) {
        alpha = 1;
    }
    if (alpha) {
        return -1;
    }
    if (*cursor != '_' && n < 0x39C) {
        return n;
    }
    return -1;
}
#endif

