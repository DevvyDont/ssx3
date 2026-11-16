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
//       and the allocation size 0x109EC (the July 2003 preview allocates 0x109EC too, with the same constructor stores);
//   [U] how matched retail code uses the field;
//   [O] On Tour's member order/name, where the fields on both sides are placed by [R]/[U].
// Retail differs from On Tour only in IsLineStart/ErrorFlag: On Tour packs them as two 1-byte bools at 0x90/0x91,
// retail stores each as a word (sw at 0x90 and 0x94), so every later member sits 4 bytes higher than in On Tour.
// Every byte of the class is accounted for by On Tour's members; there are no unk_ regions.
//
// Enum-typed members (eToken, eNodeType) are kept as int: their values match On Tour's enums exactly (readToken's token
// numbers, freeParseNode/compileParseNode's node numbers), but no shared enum header exists yet.

#include "layout.h"

// One expression-tree node; 0x800 of them are embedded in the parser as a pool (cInputMapParser::ParseNodes).
// On Tour: struct tParseNode { eNodeType Type; <anonymous union, members not in the dump>; } (size 0x10).
// The union's member names are unknown; the members below are typed by retail use and named by offset.
struct tIdentifier;
struct tParseNode {
    int Type;                       // 0x00 [O] eNodeType; [U] the switch in freeParseNode / compileParseNode
                                    //      (1 Reference, 2 Button, 3 Config, 4 Constant, 5 Selection, 6.. operators)
    union {
        tParseNode* unk_04;         // 0x04 [U] first operand (operators, Selection's condition); the free-list link
                                    //      (cInputMapParser_cInputMapParser, func_003215F8, freeParseNode)
        tIdentifier* unk_04_id;     // 0x04 [U] Reference node (Type 1): the identifier (compileParseNode reads ->Expression)
        int unk_04_i;               // 0x04 [U] Button/Config node (Type 2/3): the button or config index (compileParseNode)
        float unk_04_f;             // 0x04 [U] Constant node (Type 4): the value (parseOr/parseAnd/parseExpression fold it)
    };
    union {
        tParseNode* unk_08;         // 0x08 [U] second operand (binary operators), Selection's then-branch
        int unk_08_i;               // 0x08 [U] Reference/Button node: the button modifier (compileParseNode compares
                                    //      it with 5 = On Tour kButtonModifier_Default)
    };
    tParseNode* unk_0C;             // 0x0C [U] Selection node (Type 5): the else-branch (freeParseNode, compileParseNode)
};

CHECK_OFFSET(tParseNode, Type, 0x00);
CHECK_OFFSET(tParseNode, unk_04, 0x04);
CHECK_OFFSET(tParseNode, unk_04_f, 0x04);
CHECK_OFFSET(tParseNode, unk_08, 0x08);
CHECK_OFFSET(tParseNode, unk_0C, 0x0C);
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
    char TokenText[64];             // 0x0009C [O]; [U] readToken's token text (63 chars max); parseStatement / parseTerm
                                    //         use it as the identifier name
    float TokenValue;               // 0x000DC [O]; [U] readToken scans a constant into it; parseTerm copies it into a
                                    //         Constant node
    tParseNode* FreeParseNodes;     // 0x000E0 [O]; [R] = ParseNodes; [U] func_003215F8 (newParseNode) pops, freeParseNode
                                    //         pushes
    tParseNode ParseNodes[2048];    // 0x000E4 [O]; [R] the constructor links all 0x800 into the free list
    int NumIdentifiers;             // 0x080E4 [O]; [R] = 0; [U] lookupIdentifierName's loop bound, parseStatement's ++
    tIdentifier Identifiers[512];   // 0x080E8 [O]; [U] lookupIdentifierName scans them (0x44 stride), parseStatement
                                    //         fills Identifiers[NumIdentifiers]
    tIdentifier* CurrentIdentifier; // 0x108E8 [O]; [U] parseStatement points it at the statement being parsed; parseTerm
                                    //         compares a referenced identifier with it (self-reference error)
    int FreeRegisters[63];          // 0x108EC [O]; [P] preview assert "NumFreeRegisters < arraysize(FreeRegisters)";
                                    //         [R] filled with 63..1; [U] compileParseNode / func_00324EC8 / func_003250D8
                                    //         pop and push output registers
    int NumFreeRegisters;           // 0x109E8 [O]; [P] same assert; [R] = 0, then counts the 63 pushes; [U] as above
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

#endif /* INPUT_INPUTPARSE_H */
