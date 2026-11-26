#ifndef INPUT_INPUTPARSE_H
#define INPUT_INPUTPARSE_H

// cInputMapParser: compiles the controller input-map script (cInputMap_loadMapFile hands it the file text) into
// cInputMap's op-code stream. One instance per loaded map: `cMemMan_alloc(0x109EC, "InputParser", 0x100, 0)` in
// cInputMap_loadMapFile, freed by cInputMap_purgeMapFile (plain operator delete: no destructor body). EA's header was
// ../source/input/inputparse.h (the July 2003 preview's assert path; its inline-function asserts name CurrentToken,
// FreeRegisters, NumFreeRegisters, kTokenIdent, kTokenConstant, kButtonModifier_Default, kNumConfigRegisters).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance (the class has no bases and
// no vtable). Member names are EA's, from SSX On Tour's PSP DWARF (class cInputMapParser, size 0x109E8), placed at
// retail offsets by:
//   [R] retail's own initialisation: cInputMapParser_cInputMapParser (stores, the free-list and register-stack loops)
//       and the allocation size 0x109EC (the July 2003 preview allocates 0x109EC too, with the same constructor
//       stores);
//   [U] how matched retail code uses the field;
//   [O] On Tour's member order/name, where the fields on both sides are placed by [R]/[U].
// Retail differs from On Tour only in IsLineStart/ErrorFlag: On Tour packs them as two 1-byte bools at 0x90/0x91,
// retail stores each as a word (sw at 0x90 and 0x94), so every later member sits 4 bytes higher than in On Tour.
// Every byte of the class is accounted for by On Tour's members; there are no unk_ regions.
//
// Enum-typed members (eToken, eNodeType) are kept as int: their values match On Tour's enums exactly (readToken's token
// numbers, freeParseNode/compileParseNode's node numbers), but no shared enum header exists yet.

#include "layout.h"

// A button's modifier: what of the button a Reference / Button node reads (cInputExec::getButton). [O] On Tour's
// enum eButtonModifiers, names and values; [P] preview asserts "DefaultButtonModifier != kButtonModifier_Default";
// [U] buttonToRegister packs 6 (kButtonModifier_Num) per button, compileParseNode replaces kButtonModifier_Default by
// the statement's default, and getButton reads held, pressed, released or repeat for 1 to 4.
enum eButtonModifiers {
    kButtonModifier_Value = 0,
    kButtonModifier_Held = 1,
    kButtonModifier_Pressed = 2,
    kButtonModifier_Released = 3,
    kButtonModifier_Repeat = 4,
    kButtonModifier_Default = 5,
    kButtonModifier_Num = 6
};

// The op codes of a compiled input-map program (tInputOpCode's opcode field). [O] On Tour's enum eInputOpCodes, names
// and values; [U] compileParseNode emits kInputOpCodeOr .. kInputOpCodeNot for the operator nodes in On Tour's order,
// and cInputExec_execute switches on them.
enum eInputOpCodes {
    kInputOpCodeLoad = 0, kInputOpCodeMove = 1, kInputOpCodeBranchFalse = 2, kInputOpCodeBranch = 3,
    kInputOpCodeOr = 4, kInputOpCodeAnd = 5, kInputOpCodeGR = 6, kInputOpCodeGE = 7, kInputOpCodeLS = 8,
    kInputOpCodeLE = 9, kInputOpCodeEQ = 10, kInputOpCodeNE = 11, kInputOpCodeAdd = 12, kInputOpCodeSub = 13,
    kInputOpCodeMul = 14, kInputOpCodeDiv = 15, kInputOpCodeMax = 16, kInputOpCodeMin = 17, kInputOpCodePow = 18,
    kInputOpCodeAtan2 = 19, kInputOpCodeAtan = 20, kInputOpCodeSin = 21, kInputOpCodeCos = 22, kInputOpCodeExp = 23,
    kInputOpCodeLog = 24, kInputOpCodeFloor = 25, kInputOpCodeCeil = 26, kInputOpCodeAbs = 27, kInputOpCodeSqrt = 28,
    kInputOpCodeMinRadian = 29, kInputOpCodeNeg = 30, kInputOpCodeNot = 31, kInputOpCodeEnd = 32
};

// One expression-tree node; 0x800 of them are embedded in the parser as a pool (cInputMapParser::ParseNodes).
// On Tour: struct tParseNode { eNodeType Type; <anonymous union, members not in the dump>; } (size 0x10).
// The union's member names are unknown; the members below are typed and named by retail use (Operand1..3 are the
// child nodes in the order freeParseNode frees them and compileParseNode compiles them).
struct tIdentifier;
struct tParseNode {
    int Type;                       // 0x00 [O] eNodeType; [U] the switch in freeParseNode / compileParseNode
                                    //      (1 Reference, 2 Button, 3 Config, 4 Constant, 5 Selection, 6.. operators)
    union {
        tParseNode* Operand1;       // 0x04 [U] first operand (operators, Selection's condition); the free-list link
                                    //       (cInputMapParser_cInputMapParser, cInputMapParser_newParseNode,
                                    //       freeParseNode)
        tIdentifier* Identifier;    // 0x04 [U] Reference node (Type 1): the identifier (compileParseNode reads
                                    //       ->Expression)
        int Index;                  // 0x04 [U] Button/Config node (Type 2/3): the button or config index
                                    //       (compileParseNode)
        float Value;                // 0x04 [U] Constant node (Type 4): the value (parseOr/parseAnd/parseExpression fold
                                    //       it)
    };
    union {
        tParseNode* Operand2;       // 0x08 [U] second operand (binary operators), Selection's then-branch
        int Modifier;               // 0x08 [U] Reference/Button node: the button modifier (compileParseNode compares
                                    //      it with 5 = On Tour kButtonModifier_Default)
    };
    tParseNode* Operand3;           // 0x0C [U] Selection node (Type 5): the else-branch (freeParseNode,
                                    //       compileParseNode)
};

CHECK_OFFSET(tParseNode, Type, 0x00);
CHECK_OFFSET(tParseNode, Operand1, 0x04);
CHECK_OFFSET(tParseNode, Identifier, 0x04);
CHECK_OFFSET(tParseNode, Index, 0x04);
CHECK_OFFSET(tParseNode, Value, 0x04);
CHECK_OFFSET(tParseNode, Operand2, 0x08);
CHECK_OFFSET(tParseNode, Modifier, 0x08);
CHECK_OFFSET(tParseNode, Operand3, 0x0C);
CHECK_SIZE(tParseNode, 0x10);

// One named statement of the map file ("name = expression;"); 512 are embedded in the parser.
struct tIdentifier {
    char Name[64];                  // 0x00 [O]; [U] lookupIdentifierName compares it, parseStatement strcpy's the token
    tParseNode* Expression;         // 0x40 [O]; [U] parseStatement stores parseExpression's tree; compileStatement and
                                    //      compileParseNode (Reference nodes) compile it
};

CHECK_OFFSET(tIdentifier, Name, 0x00);
CHECK_OFFSET(tIdentifier, Expression, 0x40);
CHECK_SIZE(tIdentifier, 0x44);

struct cInputMapParser {
    // A token (CurrentToken). [O] On Tour's cInputMapParser::eToken, names and values; [P] preview assert
    // "CurrentToken == kTokenIdent"; [U] readToken stores these numbers and the parse* functions compare them.
    enum eToken {
        kTokenEOF = 0, kTokenError = 1, kTokenIdent = 2, kTokenAssign = 3, kTokenPeriod = 4, kTokenSemicolon = 5,
        kTokenQuestion = 6, kTokenColon = 7, kTokenComma = 8, kTokenOpenBracket = 9, kTokenCloseBracket = 10,
        kTokenConstant = 11, kTokenOr = 12, kTokenAnd = 13, kTokenGR = 14, kTokenGE = 15, kTokenLS = 16,
        kTokenLE = 17, kTokenEQ = 18, kTokenNE = 19, kTokenPlus = 20, kTokenMinus = 21, kTokenMul = 22,
        kTokenDiv = 23, kTokenNot = 24, kTokenMax = 25, kTokenMin = 26, kTokenPow = 27, kTokenAtan2 = 28,
        kTokenAtan = 29, kTokenSin = 30, kTokenCos = 31, kTokenExp = 32, kTokenLog = 33, kTokenFloor = 34,
        kTokenCeil = 35, kTokenAbs = 36, kTokenSqrt = 37, kTokenMinRadian = 38
    };
    // The register number for "none" (name ours): registers are 0..63 (temporaries), 0x40.. (configs) and 0x60..
    // (buttons). [U] compileParseNode allocates a temporary for a negative destination, emits nothing for a button or
    // config read with this one, and starts a selection's condition register at it.
    enum { kNoRegister = -1 };
    // A parse node's type (tParseNode::Type). [O] On Tour's cInputMapParser::eNodeType, names and values; [U]
    // freeParseNode and compileParseNode switch on them, the parse* functions build them.
    enum eNodeType {
        kNodeNull = 0, kNodeReference = 1, kNodeButton = 2, kNodeConfig = 3, kNodeConstant = 4, kNodeSelection = 5,
        kNodeOr = 6, kNodeAnd = 7, kNodeGR = 8, kNodeGE = 9, kNodeLS = 10, kNodeLE = 11, kNodeEQ = 12, kNodeNE = 13,
        kNodeAdd = 14, kNodeSub = 15, kNodeMul = 16, kNodeDiv = 17, kNodeNeg = 18, kNodeNot = 19, kNodeMax = 20,
        kNodeMin = 21, kNodePow = 22, kNodeAtan2 = 23, kNodeAtan = 24, kNodeSin = 25, kNodeCos = 26, kNodeExp = 27,
        kNodeLog = 28, kNodeFloor = 29, kNodeCeil = 30, kNodeAbs = 31, kNodeSqrt = 32, kNodeMinRadian = 33
    };

    unsigned char* File;            // 0x00000 [O] (On Tour: const char*); [R] = file argument; [U] readToken's buffer
                                    //         start (end test File + FileLength)
    int FileLength;                 // 0x00004 [O]; [R] = length argument; [U] readToken's end test
    unsigned char* FilePointer;     // 0x00008 [O] (On Tour: const char*); [R] = file argument; [U] readToken's read
                                    //         cursor, read with lbu (unsigned bytes)
    char FileName[128];             // 0x0000C [O]; [R] FileName[0] = 0 (sb); [U] cInputMapParser_error prints it with
                                    //         "%s(%d): "; readToken copies a '#' line directive's file name into it
    int LineNumber;                 // 0x0008C [O]; [R] = 1; [U] the %d of "%s(%d): "; readToken counts '\n'
    int IsLineStart;                // 0x00090 [O] (On Tour: a 1-byte bool at 0x90; retail: sw); [R] = 1; [U] readToken
                                    //         sets it on '\n' and accepts '#' directives only while it is set
    int ErrorFlag;                  // 0x00094 [O] (On Tour: a 1-byte bool at 0x91; retail: sw); [R] = 0;
                                    //         [U] cInputMapParser_error sets it to 1
    int CurrentToken;               // 0x00098 [O] eToken; [P] preview assert "CurrentToken == kTokenIdent";
                                    //         [U] readToken stores On Tour's eToken numbers; parse* compare (0 EOF,
                                    //         2 Ident, 3 Assign, 5 Semicolon, 6 Question, 7 Colon, 9/10 brackets, ...)
    char TokenText[64];             // 0x0009C [O]; [U] readToken's token text (63 chars max); parseStatement /
                                    //       parseTerm use it as the identifier name
    float TokenValue;               // 0x000DC [O]; [U] readToken scans a constant into it; parseTerm copies it into a
                                    //         Constant node
    tParseNode* FreeParseNodes;     // 0x000E0 [O]; [R] = ParseNodes; [U] cInputMapParser_newParseNode pops,
                                    //       freeParseNode pushes
    tParseNode ParseNodes[2048];    // 0x000E4 [O]; [R] the constructor links all 0x800 into the free list
    int NumIdentifiers;             // 0x080E4 [O]; [R] = 0; [U] lookupIdentifierName's loop bound, parseStatement's ++
    tIdentifier Identifiers[512];   // 0x080E8 [O]; [U] lookupIdentifierName scans them (0x44 stride), parseStatement
                                    //         fills Identifiers[NumIdentifiers]
    tIdentifier* CurrentIdentifier; // 0x108E8 [O]; [U] parseStatement points it at the statement being parsed;
                                    //       parseTerm compares a referenced identifier with it (self-reference error)
    int FreeRegisters[63];          // 0x108EC [O]; [P] preview assert "NumFreeRegisters < arraysize(FreeRegisters)";
                                    //       [R] filled with 63..1; [U] compileParseNode /
                                    //       cInputMapParser_compileBinaryNode / cInputMapParser_compileUnaryNode pop
                                    //       and push output registers
    int NumFreeRegisters;           // 0x109E8 [O]; [P] same assert; [R] = 0, then counts the 63 pushes; [U] as above

    // Methods bound to their retail symbols by asm labels.
    void skipPastSemicolon() __asm__("cInputMapParser_skipPastSemicolon");
    void* newParseNode() __asm__("cInputMapParser_newParseNode");
    void freeParseNode(tParseNode* node) __asm__("cInputMapParser_freeParseNode");
    int lookupButtonName(char* name) __asm__("cInputMapParser_lookupButtonName");
    int lookupModifierName(const char* name) __asm__("cInputMapParser_lookupModifierName");
    int lookupConfigName(char* name) __asm__("cInputMapParser_lookupConfigName");
    tIdentifier* lookupIdentifierName(const char* name) __asm__("cInputMapParser_lookupIdentifierName");
    int configToRegister(int varIndex) __asm__("cInputMapParser_configToRegister");
    int buttonToRegister(int inputIndex, int modifier) __asm__("cInputMapParser_buttonToRegister");
    void readToken() __asm__("cInputMapParser_readToken");
    void parseFile() __asm__("cInputMapParser_parseFile");
    void parseStatement() __asm__("cInputMapParser_parseStatement");
    void* parseExpression() __asm__("cInputMapParser_parseExpression");
    void* parseOr() __asm__("cInputMapParser_parseOr");
    void* parseAnd() __asm__("cInputMapParser_parseAnd");
    void* parseCompare() __asm__("cInputMapParser_parseCompare");
    void* parseSum() __asm__("cInputMapParser_parseSum");
    void* parseProduct() __asm__("cInputMapParser_parseProduct");
    void* parseTerm() __asm__("cInputMapParser_parseTerm");
    void* parseBinaryFunc(int op) __asm__("cInputMapParser_parseBinaryFunc");
    void* parseUnaryFunc(int op) __asm__("cInputMapParser_parseUnaryFunc");
    int compileStatement(const char* name, int* out, int max, int defaultModifier)
        __asm__("cInputMapParser_compileStatement");
};

CHECK_OFFSET(cInputMapParser, File, 0x00000);
CHECK_OFFSET(cInputMapParser, FileLength, 0x00004);
CHECK_OFFSET(cInputMapParser, FilePointer, 0x00008);
CHECK_OFFSET(cInputMapParser, FileName, 0x0000C);
CHECK_OFFSET(cInputMapParser, LineNumber, 0x0008C);
CHECK_OFFSET(cInputMapParser, IsLineStart, 0x00090);
CHECK_OFFSET(cInputMapParser, ErrorFlag, 0x00094);
CHECK_OFFSET(cInputMapParser, CurrentToken, 0x00098);
CHECK_OFFSET(cInputMapParser, TokenText, 0x0009C);
CHECK_OFFSET(cInputMapParser, TokenValue, 0x000DC);
CHECK_OFFSET(cInputMapParser, FreeParseNodes, 0x000E0);
CHECK_OFFSET(cInputMapParser, ParseNodes, 0x000E4);
CHECK_OFFSET(cInputMapParser, NumIdentifiers, 0x080E4);
CHECK_OFFSET(cInputMapParser, Identifiers, 0x080E8);
CHECK_OFFSET(cInputMapParser, CurrentIdentifier, 0x108E8);
CHECK_OFFSET(cInputMapParser, FreeRegisters, 0x108EC);
CHECK_OFFSET(cInputMapParser, NumFreeRegisters, 0x109E8);
CHECK_SIZE(cInputMapParser, 0x109EC);

// cInputPreProcessor: expands an input-map file's #include lines into one text buffer before cInputMapParser sees it
// (cSSXApp_loadInputMap: `cInputPreProcessor_loadFileSize(&pp, "data/config/input.map", &size)`). It writes
// "# <line> "<file>"" tag lines (writeTagLine) that cInputMapParser's readToken turns into FileName / LineNumber, and
// strips C and C++ comments. Its methods live in input/inputparse.cpp (EA's d:/ssx/code/source/input/inputparse.cpp,
// 0x00325F48 loadFileSize ... 0x00326A20 error).
//
// Size 0x10: [R] cSSXApp_loadInputMap builds the only instance on its stack at sp+0x00 and the next local (the size
// out-parameter) at sp+0x10; [O] On Tour's cInputPreProcessor is 0x10 with the same four members. No constructor,
// destructor, bases or vtable (loadFileSize initialises the buffer words itself).
// Tags: [R] cInputPreProcessor_loadFileSize's initialisation; [U] how retail code uses the field; [O] On Tour's DWARF.

// The file being read (On Tour struct tInputSource, 0x10): readFile builds one on its stack per (included) file and
// passes it by reference to every reader method.
struct tInputSource {
    const unsigned char* Data;      // 0x0 [O] (const char* in On Tour; retail reads it with lbu); [U] readFile sets it
                                    //       to FILE_loadsize's buffer and frees it at the end; readChar's end test Data
                                    //       + DataLen
    int DataLen;                    // 0x4 [O]; [U] FILE_loadsize's size out-parameter (readFile); readChar's end test
    const unsigned char* ReadPtr;   // 0x8 [O]; [U] readChar: CurrentChar = *ReadPtr++ (lbu)
    int LineNumber;                 // 0xC [O]; [U] readFile sets 1 and passes it to writeTagLine; skipCComment /
                                    //       copyLine count '\n'
};

CHECK_OFFSET(tInputSource, Data, 0x0);
CHECK_OFFSET(tInputSource, DataLen, 0x4);
CHECK_OFFSET(tInputSource, ReadPtr, 0x8);
CHECK_OFFSET(tInputSource, LineNumber, 0xC);
CHECK_SIZE(tInputSource, 0x10);

struct cInputPreProcessor {
    int OutputTop;                  // 0x0 [O]; [R] loadFileSize sets 0; [U] writeChar appends at
                                    //       OutputBuf[OutputTop++]; loadFileSize copies OutputTop bytes out and returns
                                    //       it as the size
    int OutputLen;                  // 0x4 [O]; [R] loadFileSize sets 0x800; [U] writeChar grows the buffer when
                                    //      OutputTop reaches it (max(OutputTop + 0x400, OutputTop * 3 / 2))
    char* OutputBuf;                // 0x8 [O]; [R] loadFileSize: new (0x800, "PreProcBuf"); [U] writeChar reallocates
                                    //      it; loadFileSize frees it after copying the text out
    int CurrentChar;                // 0xC [O]; [U] readChar stores the next byte or -1 at the end of the source; every
                                    //      reader compares it ('\n', '/', '*', '#'); copyLine / skipCComment /
                                    //      skipCppComment / readCmdLine pass its low byte to writeChar (lb 0xC)

    // Methods bound to their retail symbols by asm labels.
    void* loadFileSize(char* path, int* outSize) __asm__("cInputPreProcessor_loadFileSize");
    void readFile(int path, int fn) __asm__("cInputPreProcessor_readFile");
    bool copyLine(tInputSource* isrc, int path, int fn) __asm__("cInputPreProcessor_copyLine");
    void writeString(char* str) __asm__("cInputPreProcessor_writeString");
    void writeChar(char ch) __asm__("cInputPreProcessor_writeChar");
    void writeTagLine(int line, int name) __asm__("cInputPreProcessor_writeTagLine");
    void readChar(tInputSource* isrc) __asm__("cInputPreProcessor_readChar");
    void skipCComment(tInputSource* isrc, int path, int fileName) __asm__("cInputPreProcessor_skipCComment");
    void skipCppComment(tInputSource* isrc, int path, int fileName) __asm__("cInputPreProcessor_skipCppComment");
    void skipToEOL(tInputSource* isrc) __asm__("cInputPreProcessor_skipToEOL");
    void readCmdLine(tInputSource* isrc, int path, int fn) __asm__("cInputPreProcessor_readCmdLine");
    void error() __asm__("cInputPreProcessor_error__FPv");
};

CHECK_OFFSET(cInputPreProcessor, OutputTop, 0x0);
CHECK_OFFSET(cInputPreProcessor, OutputLen, 0x4);
CHECK_OFFSET(cInputPreProcessor, OutputBuf, 0x8);
CHECK_OFFSET(cInputPreProcessor, CurrentChar, 0xC);
CHECK_SIZE(cInputPreProcessor, 0x10);

// cInputExec: the input-map evaluator - runs one compiled input-map statement (cInputMapParser's op-codes) against a
// controller pad and returns register 0 (execute). Never allocated: cInputMap_getValue / getValue1 / getState /
// getState1 (input/inputmap.cpp) build one at sp + 0 (declared there as char parser[400] or 0x190-byte views
// sParser_1010 / sParser_1188; the saved registers start at sp + 0x190), construct it with cInputExec_cInputExec
// (DefaultButtonModifier = 0) and, in the *1 overloads, store the default modifier and configs 1..8 before
// cInputExec_execute. Methods in input/inputparse.cpp (0x00325250..0x00325450); EA's inline asserts
// for it are filed under d:/ssx/code/source/input/inputparse.cpp ("DefaultButtonModifier != kButtonModifier_Default",
// "reg >= kFirstTempRegister && reg < kFirstTempRegister + kNumTempRegisters", "reg >= 0 && reg < kNumConfigRegisters").
//
// Size 0x188: [O] On Tour's cInputExec is 0x188 with the same four members; [U] getRegister reads configs 0..0x1F
// (registers 0x40..0x5F) through getConfig at 0x108 + 4 * i, so Config ends at 0x188. No bases, no vtable.
// Registers: 0..0x3F = Temp (getTempReg / getRegister), 0x40..0x5F = Config, 0x60 + 6 * button + modifier = a button of
// Pad (getButton).
// Tags: [R] cInputExec_cInputExec; [P] the preview's assert strings above; [U] how retail code uses the field;
// [O] On Tour's DWARF (class cInputExec: same names, types, order and offsets). Enums are ints.

struct cInputPad;   // On Tour: int NumButtons + 24 button records of 0x1C (cPlaybackMan allocates 676 "cInputPad")

struct cInputExec {
    int DefaultButtonModifier;      // 0x000 [O] eButtonModifiers; [P] assert "DefaultButtonModifier !=
                                    //       kButtonModifier_Default"; [R] ctor 0; [U] getButton(kind 5) recurses with it;
                                    //       cInputMap_getValue1 / getState1 store their modifier argument
    cInputPad* Pad;                 // 0x004 [O]; [U] execute stores its pad argument; getButton reads Pad->NumButtons
                                    //       and the button records (0x1C stride)
    float Temp[64];                 // 0x008 [O]; [P] kFirstTempRegister / kNumTempRegisters assert; [U] getTempReg
                                    //       returns &Temp[i], getRegister reads Temp[id] for id < 0x40
    float Config[32];               // 0x108 [O]; [P] kNumConfigRegisters asserts; [U] getConfig reads Config[i] (i != 0);
                                    //       cInputMap_getValue1 / getState1 store configs 1..8 (0x10C..0x128)

    // Methods bound to their retail symbols by asm labels.
    float getRegister(unsigned int id) __asm__("cInputExec_getRegister");
    void* getTempReg(int i) __asm__("cInputExec_getTempReg");
    float getConfig(int varIndex) __asm__("cInputExec_getConfig");
};

CHECK_OFFSET(cInputExec, DefaultButtonModifier, 0x000);
CHECK_OFFSET(cInputExec, Pad, 0x004);
CHECK_OFFSET(cInputExec, Temp, 0x008);
CHECK_OFFSET(cInputExec, Config, 0x108);
CHECK_SIZE(cInputExec, 0x188);

// ---- tInputOpCode: one word of a compiled input-map program (views group D) ----
//
// [O] On Tour union tInputOpCode (0x4) of six views of the word: Base {opcode:6}, Unary {opcode, dest:6, operand:10},
// Binary {opcode, dest, operand1:10, operand2:10}, Branch {opcode, operand:10, offset:16}, Load {opcode, dest} and Float
// {float value} (a constant that follows a load); [O] cInputMapParser::compileParseNode writes them into
// cInputMap::OpCodeBuf (input/inputmap.h, a tInputOpCode*) and cInputExec::execute(.., const tInputOpCode*) runs them;
// [U] the retail executor (view sInsn_324678) extracts the same bit fields (r = Binary, j = Branch, f = Float.value).
// The six struct names are On Tour's.

struct tInputOpCodeBase {
    unsigned int opcode : 6;        // bits 0..5 [O]
    unsigned int pad : 26;          // bits 6..31 [O]
};

CHECK_SIZE(tInputOpCodeBase, 0x4);

struct tInputOpCodeUnary {
    unsigned int opcode : 6;        // bits 0..5 [O]
    unsigned int dest : 6;          // bits 6..11 [O]
    unsigned int operand : 10;      // bits 12..21 [O]
    unsigned int pad : 10;          // bits 22..31 [O]
};

CHECK_SIZE(tInputOpCodeUnary, 0x4);

struct tInputOpCodeBinary {
    unsigned int opcode : 6;        // bits 0..5 [O]
    unsigned int dest : 6;          // bits 6..11 [O]
    unsigned int operand1 : 10;     // bits 12..21 [O]
    unsigned int operand2 : 10;     // bits 22..31 [O]
};

CHECK_SIZE(tInputOpCodeBinary, 0x4);

struct tInputOpCodeBranch {
    unsigned int opcode : 6;        // bits 0..5 [O]
    unsigned int operand : 10;      // bits 6..15 [O]
    unsigned int offset : 16;       // bits 16..31 [O]
};

CHECK_SIZE(tInputOpCodeBranch, 0x4);

struct tInputOpCodeLoad {
    unsigned int opcode : 6;        // bits 0..5 [O]
    unsigned int dest : 6;          // bits 6..11 [O]
    unsigned int pad : 20;          // bits 12..31 [O]
};

CHECK_SIZE(tInputOpCodeLoad, 0x4);

struct tInputOpCodeFloat {
    float value;                    // 0x0 [O]
};

CHECK_SIZE(tInputOpCodeFloat, 0x4);

union tInputOpCode {
    tInputOpCodeBase Base;          // 0x0 [O]
    tInputOpCodeUnary Unary;        // 0x0 [O]
    tInputOpCodeBinary Binary;      // 0x0 [O]
    tInputOpCodeBranch Branch;      // 0x0 [O]
    tInputOpCodeLoad Load;          // 0x0 [O]
    tInputOpCodeFloat Float;        // 0x0 [O]
};

CHECK_SIZE(tInputOpCode, 0x4);

#endif /* INPUT_INPUTPARSE_H */
