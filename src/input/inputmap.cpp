#include "common.h"
#include "input/inputpad.h"
#include "input/inputmap.h"
#include "input/inputparse.h"

// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

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
void cInputMap::init(int count)
{
    MaxMaps = count;
    MapOffset = (int*)operator_new_tag(count * 4, str_InputMapTable, 0, 0);
    MEM_fill(MapOffset, 0xFFFFFFFF, MaxMaps * 4);
    OpCodeBuf = 0;
    OpCodeLen = 0;
    OpCodeTop = 0;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_purge);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Frees an input map's statement offset table and op-code buffer.
 *
 * Frees MapOffset (+0x4) and the op-code buffer (+0x14) with cMemMan_free when they are set. The pointers are not
 * cleared afterwards. Nothing calls it.
 */
void cInputMap::purge()
{
    void *buffer = MapOffset;
    if (buffer != 0) {
        cMemMan_free(buffer);
    }
    buffer = (void*)OpCodeBuf;
    if (buffer != 0) {
        cMemMan_free(buffer);
    }
}
#endif
//100%
INCLUDE_ASM("input/inputmap", cInputMap_loadMapFile1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void cInputMap_loadMapFile_vvi(void* self, void* data, int size) __asm__("cInputMap_loadMapFile");
void cMemMan_free(void* p);
extern "C" void* FILE_loadsize(const char* path, int* size, int a2);

/**
 * Loads an input map script from a file and compiles it into the map.
 *
 * Reads the whole file with FILE_loadsize (flag 0x100), passes the text and its size to cInputMap_loadMapFile and then
 * frees the buffer. Nothing calls it.
 *
 * @param path  the map file's path (On Tour fn)
 */
void cInputMap::loadMapFile1(const char* path)
{
    int size;
    void* data = FILE_loadsize(path, &size, 0x100);
    cInputMap_loadMapFile_vvi(this, data, size);
    if (data != 0) {
        cMemMan_free(data);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_loadMapFile);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* __15cInputMapParserPvT1(cInputMapParser* self, void* a1, void* a2);
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
void cInputMap::loadMapFile(void* mem, void* len)
{
    Parser = new (str_InputParser, 0x100, 0) cInputMapParser(mem, len);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_compileMap);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void MEM_copy(void* dst, void* src, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int cInputMapParser_compileStatement(void* parser, char* src, tInputOpCode* dst, int room, int a4);
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
void cInputMap::compileMap(int idx, char* src)
{
    if (OpCodeLen - OpCodeTop < 0x100) {
        int newCap = OpCodeLen + 0x400;
        int* buf = (int*)operator_new_tag(newCap * 4, str_TempOpCodes, 0x100, 0);
        if ((int*)OpCodeBuf != 0) {
            MEM_copy(buf, (int*)OpCodeBuf, OpCodeLen * 4);
            if ((int*)OpCodeBuf != 0) {
                cMemMan_free((int*)OpCodeBuf);
            }
        }
        OpCodeBuf = (tInputOpCode*)buf;
        OpCodeLen = newCap;
    }
    MapOffset[idx] = OpCodeTop;
    OpCodeTop += cInputMapParser_compileStatement(Parser, src, &OpCodeBuf[OpCodeTop],
                                                       OpCodeLen - OpCodeTop, 5);
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
void cInputMap::purgeMapFile()
{
    if ((int*)Parser != 0) {
        operator_delete((int*)Parser);
    }
    Parser = 0;
    void* copy = operator_new_tag(OpCodeTop << 2, str_InputOpCodes, 0, 0);
    MEM_copy(copy, OpCodeBuf, OpCodeTop << 2);
    if (OpCodeBuf != 0) {
        cMemMan_free(OpCodeBuf);
    }
    OpCodeBuf = (tInputOpCode*)copy;
    OpCodeLen = OpCodeTop;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getValue);
#ifdef SKIP_ASM
void* cInputExec_cInputExec(void* self);
extern "C" float cInputExec_execute(void* parser, void* a1, void* a2);

/**
 * Evaluates one compiled map statement against a controller pad and returns its value.
 *
 * Builds a 0x190-byte evaluator on the stack with cInputExec_cInputExec() and runs cInputExec_execute() on the op-code
 * stream at the statement's offset.
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param id  statement slot (input state id) to evaluate
 * @return the statement's value
 */
float cInputMap::getValue(cInputPad* inputPad, int id)
{
    char parser[400];
    cInputExec_cInputExec(parser);
    return cInputExec_execute(parser, inputPad, &OpCodeBuf[MapOffset[id]]);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getValue1);
#ifdef SKIP_ASM
void* cInputExec_cInputExec(void* self);
extern "C" float cInputExec_execute(void* parser, void* a1, void* a2);


/**
 * Evaluates one compiled map statement against a controller pad with a default button modifier and eight config values.
 *
 * Like cInputMap_getValue, but before running the statement through cInputExec_execute it stores @p modifier as the evaluator's
 * default button modifier (DefaultButtonModifier) and @p config0..@p config7 as its config values 1..8 (Config[1..8];
 * config 0 always reads 0).
 * Called by cInput_getValue1, the cInput wrapper beside cInput_getValue.
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param stateId  statement slot (input state id) to evaluate
 * @param modifier  the button modifier used for buttons the script leaves unqualified (On Tour eButtonModifiers)
 * @param config0  config value 1 (On Tour c0)
 * @param config1  config value 2
 * @param config2  config value 3
 * @param config3  config value 4
 * @param config4  config value 5
 * @param config5  config value 6
 * @param config6  config value 7
 * @param config7  config value 8 (On Tour c7)
 * @return the statement's value
 */
float cInputMap::getValue1(cInputPad* inputPad, int stateId, int modifier, float config0, float config1, float config2,
                           float config3, float config4, float config5, float config6, float config7)
{
    cInputExec parser;
    cInputExec_cInputExec(&parser);
    parser.DefaultButtonModifier = modifier;
    parser.Config[1] = config0;
    parser.Config[2] = config1;
    parser.Config[3] = config2;
    parser.Config[4] = config3;
    parser.Config[5] = config4;
    parser.Config[6] = config5;
    parser.Config[7] = config6;
    parser.Config[8] = config7;
    return cInputExec_execute(&parser, inputPad, &OpCodeBuf[MapOffset[stateId]]);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getState);
#ifdef SKIP_ASM
void* cInputExec_cInputExec(void* self);
extern "C" float cInputExec_execute(void* parser, void* a1, void* a2);

/**
 * Evaluates one compiled map statement against a controller pad and tells whether it is nonzero.
 *
 * Same evaluation as cInputMap_getValue().
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param id  statement slot (input state id) to evaluate
 * @return true when the statement's value is not 0.0
 */
bool cInputMap::getState(cInputPad* inputPad, int id)
{
    char parser[400];
    cInputExec_cInputExec(parser);
    return cInputExec_execute(parser, inputPad, &OpCodeBuf[MapOffset[id]]) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMap_getState1);
#ifdef SKIP_ASM
void* cInputExec_cInputExec(void* self);
extern "C" float cInputExec_execute(void* parser, void* a1, void* a2);


/**
 * Tells whether one compiled map statement is nonzero for a controller pad, with a default button modifier and eight
 * config values.
 *
 * Same evaluation as cInputMap_getValue1 (modifier into DefaultButtonModifier, config values into Config[1..8]),
 * compared with 0.0 like
 * cInputMap_getState. Called by cInput_getState1, the cInput wrapper beside cInput_getState.
 *
 * @param inputPad  the controller pad to read (On Tour cInputPad*)
 * @param stateId  statement slot (input state id) to evaluate
 * @param modifier  the button modifier used for buttons the script leaves unqualified (On Tour eButtonModifiers)
 * @param config0  config value 1 (On Tour c0)
 * @param config1  config value 2
 * @param config2  config value 3
 * @param config3  config value 4
 * @param config4  config value 5
 * @param config5  config value 6
 * @param config6  config value 7
 * @param config7  config value 8 (On Tour c7)
 * @return true when the statement's value is not 0.0
 */
bool cInputMap::getState1(cInputPad* inputPad, int stateId, int modifier, float config0, float config1, float config2,
                          float config3, float config4, float config5, float config6, float config7)
{
    cInputExec parser;
    cInputExec_cInputExec(&parser);
    parser.DefaultButtonModifier = modifier;
    parser.Config[1] = config0;
    parser.Config[2] = config1;
    parser.Config[3] = config2;
    parser.Config[4] = config3;
    parser.Config[5] = config4;
    parser.Config[6] = config5;
    parser.Config[7] = config6;
    parser.Config[8] = config7;
    return cInputExec_execute(&parser, inputPad, &OpCodeBuf[MapOffset[stateId]]) != 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputPad_update);
#ifdef SKIP_ASM

/**
 * Updates one controller's button states from this frame's analogue values.
 *
 * Grows the button count to @p n (new entries cleared with MEM_fill), stores each value and, once a button has
 * settled for 3 updates, sets pressed/released on a change of the held state (value > 0). Held buttons repeat
 * after 0x18 updates and then every 0xC.
 *
 * @param n     the number of buttons (On Tour numbuttons)
 * @param vals  one value per button
 */
void cInputPad::update(int n, float* vals)
{
    if (NumButtons < n) {
        MEM_fill(&Buttons[NumButtons], 0, (n - NumButtons) * sizeof(tButton));
    }
    NumButtons = n;
    for (int i = 0; i < n; i++) {
        Buttons[i].Value = vals[i];
        if (Buttons[i].DebounceCount < 3) {
            Buttons[i].IsPressed = 0;
            Buttons[i].DebounceCount++;
            Buttons[i].IsReleased = 0;
        } else {
            int on = Buttons[i].Value > 0.0f;
            if (on == Buttons[i].IsHeld) {
                Buttons[i].IsPressed = 0;
                Buttons[i].IsReleased = 0;
            } else {
                Buttons[i].IsHeld = on;
                if (on) {
                    Buttons[i].IsPressed = 1;
                    Buttons[i].IsReleased = 0;
                } else {
                    Buttons[i].IsPressed = 0;
                    Buttons[i].IsReleased = 1;
                }
                Buttons[i].DebounceCount = 0;
            }
        }
        if (Buttons[i].IsHeld) {
            if (Buttons[i].RepeatCount == 0) {
                Buttons[i].IsRepeated = 1;
                Buttons[i].RepeatCount = 0x18;
            } else {
                Buttons[i].RepeatCount--;
                if (Buttons[i].RepeatCount <= 0) {
                    Buttons[i].IsRepeated = 1;
                    Buttons[i].RepeatCount = 0xC;
                } else {
                    Buttons[i].IsRepeated = 0;
                }
            }
        } else {
            Buttons[i].IsRepeated = 0;
            Buttons[i].RepeatCount = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputmap", __15cInputMapParserPvT1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInputMapParser_parseFile(void* self);


/**
 * Constructs an input-map parser over a script in memory and parses the whole file.
 *
 * Sets the read cursor to the start, LineNumber and IsLineStart to 1, clears the file name, the error flag
 * and the identifier count, pushes registers 63..1 onto FreeRegisters, links all 2048 parse nodes into the
 * free list and then calls cInputMapParser_parseFile().
 *
 * @param file    the script text (On Tour const char*)
 * @param length  the text's length in bytes
 */
// Initialises a compiled input map: id stack 63..1 and the node free list.
cInputMapParser::cInputMapParser(void* file, void* length)
{
    cInputMapParser* self = this;
    (*(void**)&self->FileLength) = length;
    (*(void**)&self->File) = file;
    (*(void**)&self->FilePointer) = file;
    self->NumIdentifiers = 0;
    self->LineNumber = 1;
    self->IsLineStart = 1;
    self->FileName[0] = 0;
    self->ErrorFlag = 0;
    self->NumFreeRegisters = 0;
    for (int i = 63; i > 0; i--) {
        self->FreeRegisters[self->NumFreeRegisters++] = i;
    }
    self->FreeParseNodes = &self->ParseNodes[0];
    for (int j = 1; j < 0x800; j++) {
        self->ParseNodes[j - 1].Operand1 = &self->ParseNodes[j]; // the free-list link
    }
    self->ParseNodes[0x7FF].Operand1 = 0;
    cInputMapParser_parseFile(self);
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
extern "C" int vsprintf(char* dst, const char* fmt, char* ap);
extern char str_pct_s_pct_d[];

/**
 * Reports a syntax error in the input-map script and sets the parser's error flag.
 *
 * Formats "FileName(LineNumber): " followed by the printf-style message into a 1 KB local buffer. Nothing
 * prints the buffer in this build, so only ErrorFlag = 1 has an effect.
 *
 * @param fmt  printf-style message, followed by its arguments
 */
void cInputMapParser::error(const char* fmt, ...)
{
    char buf[0x400];
    cInputMapParser_error_va_list ap;
    cInputMapParser_error_va_start(ap);
    vsprintf(buf + sprintf(buf, str_pct_s_pct_d, (char*)&FileName, LineNumber), fmt, ap);
    ErrorFlag = 1;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_skipPastSemicolon);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(void* self);

/**
 * Skips the rest of a malformed input-map statement, up to and including its semicolon.
 *
 * Reads tokens until the current token is a semicolon (kTokenSemicolon) or end of file (kTokenEOF), then reads one
 * more. cInputMapParser_parseStatement() calls it after each error it reports.
 */
void cInputMapParser::skipPastSemicolon()
{
    while (CurrentToken != kTokenSemicolon && CurrentToken != kTokenEOF) {
        cInputMapParser_readToken(this);
    }
    cInputMapParser_readToken(this);
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_newParseNode);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);

/**
 * Takes a parse node from the input map parser's free node list and returns it cleared.
 *
 * Pops the head of FreeParseNodes (+0xE0, linked through each node's Operand1) and zeroes the node's 16 bytes
 * (memset). There is no check for an empty list: once the 0x800-node pool is used up it reads through the list's
 * null end. The parse functions (parseOr, parseAnd, parseTerm and the others) call it for every node they build.
 *
 * @return the new node
 */
void* cInputMapParser::newParseNode()
{
    tParseNode* freeNode = FreeParseNodes;
    FreeParseNodes = (tParseNode*)(freeNode->Operand1);
    memset(freeNode, 0, 0x10);
    return freeNode;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_freeParseNode);
#ifdef SKIP_ASM

/**
 * Returns a parse node and all of its child nodes to the parser's free node list.
 *
 * The node type decides how many children are freed first: one for the unary nodes (kNodeNeg, kNodeNot, kNodeAtan
 * to kNodeMinRadian), two for the binary operators kNodeOr to kNodeDiv and kNodeMax / kNodeMin, three for
 * kNodeSelection, none for the leaves (kNodeNull to kNodeConstant) and, a leak, for kNodePow and kNodeAtan2. The
 * node is then pushed onto FreeParseNodes, linked through its first child field. A null node is ignored.
 *
 * @param node  the node to free, or 0
 */
// Frees an expression node and its children back onto the map's node free list.
void cInputMapParser::freeParseNode(tParseNode* node)
{
    if (node == 0) {
        return;
    }
    switch (node->Type) {
    case kNodeNeg: case kNodeNot:
    case kNodeAtan: case kNodeSin: case kNodeCos: case kNodeExp: case kNodeLog:
    case kNodeFloor: case kNodeCeil: case kNodeAbs: case kNodeSqrt: case kNodeMinRadian:
        freeParseNode(node->Operand1);
        break;
    case kNodeOr: case kNodeAnd: case kNodeGR: case kNodeGE: case kNodeLS: case kNodeLE:
    case kNodeEQ: case kNodeNE: case kNodeAdd: case kNodeSub: case kNodeMul: case kNodeDiv:
    case kNodeMax: case kNodeMin:
        freeParseNode(node->Operand1);
        freeParseNode(node->Operand2);
        break;
    case kNodeSelection:
        freeParseNode(node->Operand1);
        freeParseNode(node->Operand2);
        freeParseNode(node->Operand3);
        break;
    case kNodeNull: case kNodeReference: case kNodeButton: case kNodeConfig: case kNodeConstant:
    case kNodePow: case kNodeAtan2:
        break;
    }
    node->Operand1 = FreeParseNodes;
    FreeParseNodes = (tParseNode*)node;
}
#endif

//100%
INCLUDE_ASM("input/inputmap", cInputMapParser_lookupButtonName);
#ifdef SKIP_ASM
extern "C" int strncasecmp(const char* a, const char* b, int n);
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
int cInputMapParser::lookupButtonName(char* name)
{
    if (strncasecmp(name, str_button, 6) != 0) {
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
    if (*cursor != '_' && n < 924) {
        return n;
    }
    return -1;
}
#endif

