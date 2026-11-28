#include "vtable.h"
#include "common.h"
#include "input/inputpad.h"
#include "input/ps2pad.h"
#include "input/padcache.h"
#include "intersect/aifwddiff.h"
#include "input/inputparse.h"

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_lookupModifierName);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern char str_value[];
extern char str_held[];
extern char str_pressed[];
extern char str_released[];
extern char str_repeat[];

/**
 * Maps a button modifier name to its modifier code.
 *
 * Compared case-insensitively (strcasecmp): "value" 0, "held" 1, "pressed" 2, "released" 3, "repeat" 4. Any
 * other name gives 6 rather than an error value.
 *
 * @param name  modifier name
 * @return the modifier code, or 6 for an unknown name
 */
int cInputMapParser::lookupModifierName(const char* name)
{
    if (strcasecmp(name, str_value) == 0) {
        return 0;
    }
    if (strcasecmp(name, str_held) == 0) {
        return 1;
    }
    if (strcasecmp(name, str_pressed) == 0) {
        return 2;
    }
    if (strcasecmp(name, str_released) == 0) {
        return 3;
    }
    int cmpResult = strcasecmp(name, str_repeat);
    int modifier = 4;
    if (cmpResult != 0) {
        modifier = 6;
    }
    return modifier;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_lookupConfigName);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int strncasecmp(const char* a, const char* b, int n);
extern char str_padtype[];
extern char str_arg[];
extern char str_config[];

/**
 * Maps a config variable name ("padtype", "argN" or "configN") to its config index.
 *
 * "padtype" (case-insensitive) is 0; "arg" followed by a decimal N below 8 is N + 1; "config" followed by a
 * decimal N below 23 is N + 9. The number must not be followed by a letter or '_'. The prefixes are compared
 * with strncasecmp over 3 and 6 characters.
 *
 * @param name  identifier text to look up
 * @return the config index (0 to 31), or -1 if @p name is not a config name
 */
int cInputMapParser::lookupConfigName(char* name)
{
    if (strcasecmp(name, str_padtype) == 0)
        return 0;
    if (strncasecmp(name, str_arg, 3) == 0) {
        int digit = name[3] - '0';
        if ((unsigned char)digit < 10) {
            int n = digit;
            char* cursor = name + 4;
            while ((unsigned)(*cursor - '0') < 10) {
                n = n * 10 + (*cursor++ - '0');
            }
            int alpha = 0;
            if ((unsigned)(*cursor - 'a') < 26 || (unsigned)(*cursor - 'A') < 26) {
                alpha = 1;
            }
            if (!alpha) {
                if (*cursor != '_' && n < 8) {
                    return n + 1;
                }
            }
        }
    }
    if (strncasecmp(name, str_config, 6) != 0) {
        return -1;
    }
    int digit = name[6] - '0';
    if ((unsigned char)digit >= 10) {
        return -1;
    }
    int n = digit;
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
    if (*cursor != '_' && n < 23) {
        return n + 9;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_lookupIdentifierName);
#ifdef SKIP_ASM
extern "C" int strcasecmp(const char* a, const char* b);

/**
 * Finds an identifier already defined by the input-map script.
 *
 * Scans the first NumIdentifiers entries of Identifiers and compares names case-insensitively (strcasecmp
 * lowercases both characters before comparing).
 *
 * @param name  identifier name to look for
 * @return the matching identifier, or 0 if none has that name
 */
tIdentifier* cInputMapParser::lookupIdentifierName(const char* name)
{
    int i;
    for (i = 0; i < NumIdentifiers; i++) {
        if (strcasecmp(name, Identifiers[i].Name) == 0) {
            return &Identifiers[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_configToRegister);
#ifdef SKIP_ASM
/**
 * Converts a config index into an input register number: index + 0x40.
 *
 * Registers 0x40..0x5F are the config values that cInputExec_getRegister reads through cInputExec_getConfig. The index is declared as a
 * pointer but is a plain number.
 *
 * @param varIndex  the config index (On Tour config)
 * @return the register number
 */
int cInputMapParser::configToRegister(int varIndex)
{
    return varIndex + 0x40;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_buttonToRegister);
#ifdef SKIP_ASM
/**
 * Converts a button and button modifier into an input register number: 0x60 + button x 6 + modifier.
 *
 * cInputExec_getRegister decodes these registers back into (button, modifier) for cInputExec_getButton; the 6 is On Tour's
 * kButtonModifier_Num.
 *
 * @param inputIndex  the button index (On Tour button)
 * @param modifier  the button modifier, 0..5 (On Tour eButtonModifiers)
 * @return the register number
 */
int cInputMapParser::buttonToRegister(int inputIndex, int modifier)
{
    return inputIndex * 6 + modifier + 0x60;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_readToken);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// cInputMapParser::readToken: skips whitespace, '#' line directives and '//' comments, then reads one token into
// s->text / s->tok (/ s->value for numbers). The input is a byte stream (base, len, pos); peek/advance are inline
// helpers that each re-test the end of the buffer. In the number branch the lookahead character is kept in `c`:
// every advance is followed by a peek (also after the optional 'f' float suffix, whose re-read value is unused).
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int strcasecmp(const char* a, const char* b);
extern "C" int sscanf(const char* a, const char* b, ...);
extern char str_malformed_pre_processor_line[];
extern char str_Operator_plus_plus_not_defined[];
extern char str_Identifer_exceeds_64_characters[];
extern char str_minradian[];
extern char str_Constant_exceeds_64_characters[];
extern char str_Malformed_floating_point_constant[];
extern char str_Unexpected_character_in_input_pct_c[];
extern char str_max[];
extern char str_min[];
extern char str_pow[];
extern char str_atan2[];
extern char str_atan[];
extern char str_sin[];
extern char str_cos[];
extern char str_exp[];
extern char str_log_b[];
extern char str_floor[];
extern char str_ceil[];
extern char str_abs[];
extern char str_sqrt[];
extern char str_pct_f_b[];

struct sTok_321AF0 {
    unsigned char* base;    // 0x0
    int len;                // 0x4
    unsigned char* pos;     // 0x8
    char fname[0x80];       // 0xC
    int line;               // 0x8C
    int lineStart;          // 0x90
    int pad94;              // 0x94
    int tok;                // 0x98
    char text[0x40];        // 0x9C
    float value;            // 0xDC
};

static inline bool more_321AF0(sTok_321AF0* s)
{
    return s->pos < s->base + s->len;
}
// What the readers below return past the end of the file text (name ours).
enum { kEndOfText = -1 };
static inline int pk_321AF0(sTok_321AF0* s)
{
    return more_321AF0(s) ? *s->pos : kEndOfText;
}
// peek into a variable (statement form: the token loops keep the lookahead in `c`)
#define PKC_321AF0(v) { v = kEndOfText; if (more_321AF0(s)) v = *s->pos; }
static inline int pk1_321AF0(sTok_321AF0* s)
{
    return s->pos + 1 < s->base + s->len ? s->pos[1] : kEndOfText;
}
static inline void adv_321AF0(sTok_321AF0* s)
{
    if (s->pos < s->base + s->len) {
        s->pos++;
    }
}
static inline int isWh_321AF0(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
static inline int isDig_321AF0(char c)
{
    return (unsigned)(c - '0') < 10;
}
static inline int isAlp_321AF0(char c)
{
    return (unsigned)(c - 'a') < 26 || (unsigned)(c - 'A') < 26;
}
static inline void blanks_321AF0(sTok_321AF0* s)
{
    while (isWh_321AF0(pk_321AF0(s)) && pk_321AF0(s) != kEndOfText && pk_321AF0(s) != '\n') adv_321AF0(s);
}

// append the current character to the token text (63 chars max) and advance
#define PUSHC_321AF0(c, msg)                \
    if (p - s->text >= 0x3F) {              \
        *p = 0;                             \
        cInputMapParser_error(s, msg);              \
        s->tok = 1;                         \
    }                                       \
    *p++ = c;                               \
    adv_321AF0(s);

/**
 * Reads the next token of the input-map script into CurrentToken, TokenText and TokenValue.
 *
 * Skips blanks, "//" comments and "#<line> "file"" tags (which set the line and file for errors). Recognises
 * the
 * operators, the function names max to sqrt and minradian, identifiers, and numbers (scanned with "%f"). The
 * "-"
 * case tests the next character for '+', copied from the "++" check: "-+" is rejected as "Operator '++' not
 * defined." while "--" passes.
 */
void cInputMapParser::readToken()
{
    sTok_321AF0* s = (sTok_321AF0*)this;
    int c;
    char* text;
    char* p;
    do {
        PKC_321AF0(c);
        if (c == '\n') {
            adv_321AF0(s);
            s->lineStart = 1;
            s->line++;
        } else if (c == '#' && s->lineStart) {
            adv_321AF0(s);
            blanks_321AF0(s);
            if (!isDig_321AF0(pk_321AF0(s))) {
                cInputMapParser_error(s, str_malformed_pre_processor_line);
                while (pk_321AF0(s) != '\n' && pk_321AF0(s) != kEndOfText) adv_321AF0(s);
            } else {
                s->line = 0;
                while (isDig_321AF0(pk_321AF0(s))) {
                    s->line = s->line * 10 + (pk_321AF0(s) - '0');
                    adv_321AF0(s);
                }
                blanks_321AF0(s);
                if (pk_321AF0(s) != '"') {
                    cInputMapParser_error(s, str_malformed_pre_processor_line);
                    while (pk_321AF0(s) != '\n' && pk_321AF0(s) != kEndOfText) adv_321AF0(s);
                    c = ' ';
                } else {
                    adv_321AF0(s);
                    char* fnameCursor = s->fname;
                    int n = 0;
                    for (;;) {
                        if (pk_321AF0(s) == '\n' || pk_321AF0(s) == kEndOfText) {
                            cInputMapParser_error(s, str_malformed_pre_processor_line);
                            adv_321AF0(s);
                            goto strdone_321AF0;
                        }
                        if (pk_321AF0(s) == '"') {
                            adv_321AF0(s);
                            goto strdone_321AF0;
                        }
                        if (n < 0x7F) {
                            *fnameCursor++ = pk_321AF0(s);
                            n++;
                        }
                        adv_321AF0(s);
                    }
                strdone_321AF0:
                    *fnameCursor = 0;
                    blanks_321AF0(s);
                    if (pk_321AF0(s) != '\n') {
                        cInputMapParser_error(s, str_malformed_pre_processor_line);
                        blanks_321AF0(s);
                    } else {
                        adv_321AF0(s);
                    }
                    PKC_321AF0(c);
                }
            }
        } else if (c == '/' && pk1_321AF0(s) == c) {
            adv_321AF0(s);
            adv_321AF0(s);
            while (pk_321AF0(s) != '\n' && pk_321AF0(s) != kEndOfText) adv_321AF0(s);
            s->lineStart = 0;
            c = ' ';
        } else {
            if (isWh_321AF0(c)) {
                adv_321AF0(s);
            }
        }
        text = s->text;
    } while (isWh_321AF0(c));

    p = text;
    s->lineStart = 0;
    PKC_321AF0(c);
    *p++ = c;
    if (c == kEndOfText) {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenEOF;
        return;
    }
    if (c == '!') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = cInputMapParser::kTokenNE;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenNot;
        }
    } else if (c == '+') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '+') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            cInputMapParser_error(s, str_Operator_plus_plus_not_defined);
            s->tok = cInputMapParser::kTokenError;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenPlus;
        }
    } else if (c == '-') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '+') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            cInputMapParser_error(s, str_Operator_plus_plus_not_defined);
            s->tok = cInputMapParser::kTokenError;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenMinus;
        }
    } else if (c == '.') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenPeriod;
    } else if (c == '*') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenMul;
    } else if (c == '/') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenDiv;
    } else if (c == '=') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = cInputMapParser::kTokenEQ;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenAssign;
        }
    } else if (c == '<') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = cInputMapParser::kTokenLE;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenLS;
        }
    } else if (c == '>') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = cInputMapParser::kTokenGE;
        } else {
            *p = 0;
            s->tok = cInputMapParser::kTokenGR;
        }
    } else if (c == '&' && pk1_321AF0(s) == c) {
        adv_321AF0(s);
        *p++ = pk_321AF0(s);
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenAnd;
    } else if (c == '|' && pk1_321AF0(s) == c) {
        adv_321AF0(s);
        *p++ = pk_321AF0(s);
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenOr;
    } else if (c == '(') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenOpenBracket;
    } else if (c == ')') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenCloseBracket;
    } else if (c == ',') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenComma;
    } else if (c == '?') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenQuestion;
    } else if (c == ';') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenSemicolon;
    } else if (c == ':') {
        adv_321AF0(s);
        *p = 0;
        s->tok = cInputMapParser::kTokenColon;
    } else if (isAlp_321AF0(c)) {
        // identifier or keyword
        adv_321AF0(s);
        for (;;) {
            PKC_321AF0(c);
            if (isAlp_321AF0(c) || isDig_321AF0(c) || c == '_') {
                PUSHC_321AF0(c, str_Identifer_exceeds_64_characters)
            } else {
                break;
            }
        }
        *p = 0;
        if (strcasecmp(text, str_max) == 0) {
            s->tok = cInputMapParser::kTokenMax;
        } else if (strcasecmp(text, str_min) == 0) {
            s->tok = cInputMapParser::kTokenMin;
        } else if (strcasecmp(text, str_pow) == 0) {
            s->tok = cInputMapParser::kTokenPow;
        } else if (strcasecmp(text, str_atan2) == 0) {
            s->tok = cInputMapParser::kTokenAtan2;
        } else if (strcasecmp(text, str_atan) == 0) {
            s->tok = cInputMapParser::kTokenAtan;
        } else if (strcasecmp(text, str_sin) == 0) {
            s->tok = cInputMapParser::kTokenSin;
        } else if (strcasecmp(text, str_cos) == 0) {
            s->tok = cInputMapParser::kTokenCos;
        } else if (strcasecmp(text, str_exp) == 0) {
            s->tok = cInputMapParser::kTokenExp;
        } else if (strcasecmp(text, str_log_b) == 0) {
            s->tok = cInputMapParser::kTokenLog;
        } else if (strcasecmp(text, str_floor) == 0) {
            s->tok = cInputMapParser::kTokenFloor;
        } else if (strcasecmp(text, str_ceil) == 0) {
            s->tok = cInputMapParser::kTokenCeil;
        } else if (strcasecmp(text, str_abs) == 0) {
            s->tok = cInputMapParser::kTokenAbs;
        } else if (strcasecmp(text, str_sqrt) == 0) {
            s->tok = cInputMapParser::kTokenSqrt;
        } else if (strcasecmp(text, str_minradian) == 0) {
            s->tok = cInputMapParser::kTokenMinRadian;
        } else {
            s->tok = cInputMapParser::kTokenIdent;
        }
    } else if (c == '+' || c == '-' || isDig_321AF0(c)) {
        // number: [sign] digits [. digits] [(E|e) [sign] digits] [f]
        adv_321AF0(s);
        for (;;) {
            PKC_321AF0(c);
            if (isDig_321AF0(c)) {
                PUSHC_321AF0(c, str_Constant_exceeds_64_characters)
            } else {
                break;
            }
        }
        if (c == '.' || c == 'E' || c == 'e') {
            if (c == '.') {
                do {
                    PUSHC_321AF0(c, str_Constant_exceeds_64_characters)
                    PKC_321AF0(c);
                } while (isDig_321AF0(c));
            }
            if (c == 'E' || c == 'e') {
                PUSHC_321AF0(c, str_Constant_exceeds_64_characters)
                PKC_321AF0(c);
                if (c == '-' || c == '+') {
                    PUSHC_321AF0(c, str_Constant_exceeds_64_characters)
                    PKC_321AF0(c);
                }
                if (!isDig_321AF0(c)) {
                    *p = 0;
                    cInputMapParser_error(s, str_Malformed_floating_point_constant);
                    s->tok = cInputMapParser::kTokenError;
                }
                do {
                    PUSHC_321AF0(c, str_Constant_exceeds_64_characters)
                    PKC_321AF0(c);
                } while (isDig_321AF0(c));
            }
        }
        if (c == 'f') {
            adv_321AF0(s);
            PKC_321AF0(c);
        }
        *p = 0;
        sscanf(text, str_pct_f_b, &s->value);
        s->tok = cInputMapParser::kTokenConstant;
        return;
    } else {
        cInputMapParser_error(s, str_Unexpected_character_in_input_pct_c, c);
        *p = 0;
        adv_321AF0(s);
        s->tok = cInputMapParser::kTokenError;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseFile);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_parseStatement(cInputMapParser* self);

/**
 * Parses the whole input-map script: reads the first token, then parses statements until end of file.
 */
void cInputMapParser::parseFile()
{
    readToken();
    while (CurrentToken != 0) {
        parseStatement();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseStatement);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" void cInputMapParser_skipPastSemicolon(void* self);
extern "C" int cInputMapParser_lookupButtonName(void* self, char* name);
extern "C" int cInputMapParser_lookupConfigName(cInputMapParser* self, char* name);
extern "C" void* cInputMapParser_parseExpression(cInputMapParser* self);
extern char str_Expected_identifier_in_statement[];
extern char str_Button_name_used_as_identifier_pct_s[];
extern char str_Config_name_used_as_identifier_pct_s[];
extern char str_Duplicate_identifier_name_pct_s[];
extern char str_Expected_in_statement[];
extern char str_Expected_to_end_statement[];

/**
 * Parses one `name = expression;` statement of an input map and records it as a new identifier.
 *
 * The name must be an identifier token that is not a button name, a config name or an existing identifier. On
 * any error it reports through cInputMapParser_error() (for example "Expected '=' in statement.") and skips past
 * the next semicolon; NumIdentifiers only grows when the whole statement parsed.
 */
void cInputMapParser::parseStatement()
{
    if (CurrentToken != kTokenIdent) {
        cInputMapParser_error(this, str_Expected_identifier_in_statement);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    char* name = TokenText;
    if (cInputMapParser_lookupButtonName(this, name) >= 0) {
        cInputMapParser_error(this, str_Button_name_used_as_identifier_pct_s, name);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    if (lookupConfigName(name) >= 0) {
        cInputMapParser_error(this, str_Config_name_used_as_identifier_pct_s, name);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    if (lookupIdentifierName(name) != 0) {
        cInputMapParser_error(this, str_Duplicate_identifier_name_pct_s, name);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    tIdentifier* identifier = &Identifiers[NumIdentifiers];
    CurrentIdentifier = identifier;
    strcpy(identifier->Name, name);
    readToken();
    if (CurrentToken != kTokenAssign) {
        cInputMapParser_error(this, str_Expected_in_statement);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    readToken();
    if ((identifier->Expression = (tParseNode*)parseExpression()) == 0) {
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    if (CurrentToken != kTokenSemicolon) {
        cInputMapParser_error(this, str_Expected_to_end_statement);
        cInputMapParser_skipPastSemicolon(this);
        return;
    }
    readToken();
    NumIdentifiers++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseExpression);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void* cInputMapParser_parseOr(cInputMapParser* self);
extern char D_0048DFA8[];

struct sExprNode_00323268 {
    int type;
    union {
        float value;
        sExprNode_00323268* cond;
    };
    sExprNode_00323268* a;
    sExprNode_00323268* b;
};

/**
 * Parses a conditional expression (cond ? a : b), folding it when the condition is a constant.
 *
 * @return the expression tree, or 0 after an error
 */
void* cInputMapParser::parseExpression()
{
    sExprNode_00323268* cond = (sExprNode_00323268*)parseOr();
    if (cond == 0) {
        return 0;
    }
    if (CurrentToken != kTokenQuestion) {
        return cond;
    }
    readToken();
    sExprNode_00323268* thenExpr = (sExprNode_00323268*)parseExpression();
    if (thenExpr == 0) {
        cInputMapParser_freeParseNode(this, cond);
        return 0;
    }
    if (CurrentToken != kTokenColon) {
        cInputMapParser_error(this, D_0048DFA8);
        cInputMapParser_freeParseNode(this, cond);
        cInputMapParser_freeParseNode(this, thenExpr);
        return 0;
    }
    readToken();
    sExprNode_00323268* elseExpr = (sExprNode_00323268*)parseExpression();
    if (elseExpr == 0) {
        cInputMapParser_freeParseNode(this, cond);
        cInputMapParser_freeParseNode(this, thenExpr);
        return 0;
    }
    if (cond->type == kNodeConstant) {
        if (cond->value != 0.0f) {
            cInputMapParser_freeParseNode(this, elseExpr);
            return thenExpr;
        }
        cInputMapParser_freeParseNode(this, thenExpr);
        return elseExpr;
    }
    sExprNode_00323268* n = (sExprNode_00323268*)cInputMapParser_newParseNode(this);
    n->type = kNodeSelection;
    n->cond = cond;
    n->a = thenExpr;
    n->b = elseExpr;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseOr);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void* cInputMapParser_parseAnd(cInputMapParser* self);

/**
 * Parses an OR expression: one or more AND expressions joined by the `||` token (token 12).
 *
 * Folds constants (node type 4): a true constant operand turns the result into the constant 1.0 and a false one
 * is dropped. Other operands are joined into type-6 OR nodes taken from the node pool (cInputMapParser_newParseNode). If an
 * operand fails to parse, the nodes built so far are freed.
 *
 * @return the expression's parse node, or 0 on a parse error
 */
void* cInputMapParser::parseOr()
{
    tParseNode* left = (tParseNode*)parseAnd();
    if (left == 0) {
        return 0;
    }
    int isTrue = 0;
    if (left->Type == kNodeConstant && left->Value != 0.0f) {
        isTrue = 1;
    }
    while (CurrentToken == kTokenOr) {
        readToken();
        tParseNode* right = (tParseNode*)parseAnd();
        if (right == 0) {
            cInputMapParser_freeParseNode(this, left);
            return 0;
        }
        if (isTrue) {
            left->Value = 1.0f;
            cInputMapParser_freeParseNode(this, right);
        } else if (right->Type == kNodeConstant) {
            if (right->Value != 0.0f) {
                right->Value = 1.0f;
                cInputMapParser_freeParseNode(this, left);
                left = right;
                isTrue = 1;
            } else {
                cInputMapParser_freeParseNode(this, right);
            }
        } else if (left->Type == kNodeConstant) {
            cInputMapParser_freeParseNode(this, left);
            left = right;
        } else {
            tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
            n->Operand1 = left;
            n->Operand2 = right;
            n->Type = kNodeOr;
            left = n;
        }
    }
    return left;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseAnd);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void* cInputMapParser_parseCompare(cInputMapParser* self);

/**
 * Parses a chain of "&&" operands, folding constants: a constant 0 makes the whole chain 0.
 *
 * @return the expression tree, or 0 after an error
 */
void* cInputMapParser::parseAnd()
{
    tParseNode* left = (tParseNode*)parseCompare();
    if (left == 0) {
        return 0;
    }
    int isFalse = 0;
    if (left->Type == kNodeConstant && left->Value == 0.0f) {
        isFalse = 1;
    }
    while (CurrentToken == kTokenAnd) {
        readToken();
        tParseNode* right = (tParseNode*)parseCompare();
        if (right == 0) {
            cInputMapParser_freeParseNode(this, left);
            return 0;
        }
        if (isFalse) {
            cInputMapParser_freeParseNode(this, right);
        } else if (right->Type == kNodeConstant) {
            if (right->Value == 0.0f) {
                cInputMapParser_freeParseNode(this, left);
                left = right;
                isFalse = 1;
            } else {
                cInputMapParser_freeParseNode(this, right);
                if (left->Type == kNodeConstant)
                    left->Value = 1.0f;
            }
        } else if (left->Type == kNodeConstant) {
            cInputMapParser_freeParseNode(this, left);
            left = right;
        } else {
            tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
            n->Operand1 = left;
            n->Operand2 = right;
            n->Type = kNodeAnd;
            left = n;
        }
    }
    return left;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseCompare);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cInputMapParser_parseSum(cInputMapParser* self);

/**
 * Parses a chain of comparison operators; comparisons of two constants fold to 1.0 or 0.0.
 *
 * @return the expression tree, or 0 after an error
 */
// Parses a chain of comparison operators; comparisons of two constants are folded to 1.0/0.0.
void* cInputMapParser::parseCompare()
{
    tParseNode* left = (tParseNode*)parseSum();
    if (left == 0) {
        return 0;
    }
    for (;;) {
        int op;
        switch (CurrentToken) {
        case kTokenLS:
            op = kNodeLS;
            break;
        case kTokenLE:
            op = kNodeLE;
            break;
        case kTokenGE:
            op = kNodeGE;
            break;
        case kTokenGR:
            op = kNodeGR;
            break;
        case kTokenEQ:
            op = kNodeEQ;
            break;
        case kTokenNE:
            op = kNodeNE;
            break;
        default:
            return left;
        }
        readToken();
        tParseNode* right = (tParseNode*)parseSum();
        if (right == 0) {
            cInputMapParser_freeParseNode(this, left);
            return 0;
        }
        if (left->Type == kNodeConstant && right->Type == kNodeConstant) {
            switch (op) {
            case kNodeLS:
                left->Value = left->Value < right->Value ? 1.0f : 0.0f;
                break;
            case kNodeLE:
                left->Value = left->Value <= right->Value ? 1.0f : 0.0f;
                break;
            case kNodeGE:
                left->Value = left->Value >= right->Value ? 1.0f : 0.0f;
                break;
            case kNodeGR:
                left->Value = left->Value > right->Value ? 1.0f : 0.0f;
                break;
            case kNodeEQ:
                left->Value = left->Value == right->Value ? 1.0f : 0.0f;
                break;
            case kNodeNE:
                left->Value = left->Value != right->Value ? 1.0f : 0.0f;
                break;
            }
            cInputMapParser_freeParseNode(this, right);
        } else {
            tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
            n->Operand1 = left;
            n->Type = op;
            n->Operand2 = right;
            left = n;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseSum);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void* cInputMapParser_parseProduct(cInputMapParser* self);

/**
 * Parses a chain of additions and subtractions, folding all constant operands into one constant.
 *
 * A subtracted constant is negated and added, so the chain keeps a single constant node.
 *
 * @return the expression tree, or 0 after an error
 */
void* cInputMapParser::parseSum()
{
    tParseNode* left = (tParseNode*)parseProduct();
    if (left == 0) {
        return 0;
    }
    tParseNode* konst = 0;
    if (left->Type == kNodeConstant) {
        konst = left;
    }
    for (;;) {
        int op;
        switch (CurrentToken) {
        case kTokenPlus:
            op = kNodeAdd;
            break;
        case kTokenMinus:
            op = kNodeSub;
            break;
        default:
            return left;
        }
        readToken();
        tParseNode* right = (tParseNode*)parseProduct();
        if (right == 0) {
            cInputMapParser_freeParseNode(this, left);
            return 0;
        }
        if (right->Type == kNodeConstant) {
            if (konst != 0) {
                switch (op) {
                case kNodeAdd:
                    konst->Value = konst->Value + right->Value;
                    break;
                case kNodeSub:
                    konst->Value = konst->Value - right->Value;
                    break;
                }
                cInputMapParser_freeParseNode(this, right);
            } else {
                konst = right;
                if (op == kNodeSub) {
                    right->Value = -right->Value;
                }
                tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
                n->Operand1 = left;
                n->Type = kNodeAdd;
                n->Operand2 = right;
                left = n;
            }
        } else {
            tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
            n->Operand1 = left;
            n->Type = op;
            n->Operand2 = right;
            left = n;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseProduct);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void* cInputMapParser_parseTerm(cInputMapParser* self);

/**
 * Parses a chain of multiplications and divisions, folding all constant operands into one constant.
 *
 * A divisor constant is inverted and multiplied, so the chain keeps a single constant node.
 *
 * @return the expression tree, or 0 after an error
 */
void* cInputMapParser::parseProduct()
{
    tParseNode* left = (tParseNode*)parseTerm();
    if (left == 0) {
        return 0;
    }
    tParseNode* konst = 0;
    if (left->Type == kNodeConstant) {
        konst = left;
    }
    for (;;) {
        int op;
        switch (CurrentToken) {
        case kTokenMul:
            op = kNodeMul;
            break;
        case kTokenDiv:
            op = kNodeDiv;
            break;
        default:
            return left;
        }
        readToken();
        tParseNode* right = (tParseNode*)parseTerm();
        if (right == 0) {
            cInputMapParser_freeParseNode(this, left);
            return 0;
        }
        if (right->Type == kNodeConstant) {
            if (konst != 0) {
                switch (op) {
                case kNodeMul:
                    konst->Value = konst->Value * right->Value;
                    break;
                case kNodeDiv:
                    konst->Value = konst->Value / right->Value;
                    break;
                }
                cInputMapParser_freeParseNode(this, right);
            } else {
                konst = right;
                if (op == kNodeDiv) {
                    right->Value = 1.0f / right->Value;
                }
                tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
                n->Operand1 = left;
                n->Type = kNodeMul;
                n->Operand2 = right;
                left = n;
            }
        } else {
            tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
            n->Operand1 = left;
            n->Type = op;
            n->Operand2 = right;
            left = n;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseTerm);
#ifdef SKIP_ASM
extern "C" void* cInputMapParser_parseBinaryFunc(cInputMapParser* self, int op);
extern "C" void* cInputMapParser_parseUnaryFunc(cInputMapParser* self, int op);
extern char str_Expected_modifier_after_button[];
extern char str_Illegal_modifier_on_button_pct_s[];
extern char str_Recursive_identifier_definition[];
extern char str_Illegal_modifier_on_reference_pct_s[];
extern char str_Unexpected_identifier_pct_s[];
extern char str_Expected_in_expression[];
extern char str_Expected_term_in_expression[];

struct sTermNode_3BD8 {
    int type;
    union {
        float value;
        int index;
        void* a;
    };
    int b;
};


/**
 * Parses a term: a button (with an optional ".modifier"), a config value, an identifier, a number, a unary
 * operator, a parenthesised expression or a function call.
 *
 * References to constant identifiers are inlined as constants; a reference to the identifier being defined is
 * an
 * error. Unary minus and "!" on constants are folded.
 *
 * @return the term's tree, or 0 after an error
 */
void* cInputMapParser::parseTerm()
{
    sTermNode_3BD8* node;
    sTermNode_3BD8* operand;
    char* name;
    int input;
    int mod;
    int cfg;
    tIdentifier* identifier;
    int type;

again:
    switch (CurrentToken) {
    case kTokenIdent:
        name = TokenText;
        input = cInputMapParser_lookupButtonName(this, name);
        if (input >= 0) {
            mod = kButtonModifier_Default;
            readToken();
            if (CurrentToken == kTokenPeriod) {
                readToken();
                if (CurrentToken != kTokenIdent) {
                    cInputMapParser_error(this, str_Expected_modifier_after_button);
                    return 0;
                }
                mod = lookupModifierName(name);
                if (mod == kButtonModifier_Num) {
                    cInputMapParser_error(this, str_Illegal_modifier_on_button_pct_s, name);
                    return 0;
                }
                readToken();
            }
            node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
            node->type = kNodeButton;
            node->index = input;
            node->b = mod;
            return node;
        }
        cfg = lookupConfigName(name);
        if (cfg >= 0) {
            readToken();
            node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
            node->type = kNodeConfig;
            node->index = cfg;
            return node;
        }
        identifier = lookupIdentifierName(name);
        if (identifier != 0) {
            int mod2;
            readToken();
            if ((tIdentifier*)identifier == CurrentIdentifier) {
                cInputMapParser_error(this, str_Recursive_identifier_definition);
                return 0;
            }
            mod2 = kButtonModifier_Default;
            if (CurrentToken == kTokenPeriod) {
                readToken();
                if (CurrentToken != kTokenIdent) {
                    cInputMapParser_error(this, str_Expected_modifier_after_button);
                    return 0;
                }
                mod2 = lookupModifierName(name);
                if (mod2 == kButtonModifier_Num) {
                    cInputMapParser_error(this, str_Illegal_modifier_on_reference_pct_s, name);
                    return 0;
                }
                readToken();
            }
            type = ((sTermNode_3BD8*)identifier->Expression)->type;
            if (type == kNodeConstant) {
                node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
                node->type = type;
                node->value = ((sTermNode_3BD8*)identifier->Expression)->value;
                return node;
            }
            node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
            node->type = kNodeReference;
            node->a = identifier;
            node->b = mod2;
            return node;
        }
        cInputMapParser_error(this, str_Unexpected_identifier_pct_s, name);
        return 0;
    case kTokenConstant:
        node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
        node->type = kNodeConstant;
        node->value = TokenValue;
        readToken();
        return node;
    case kTokenPlus:
        readToken();
        goto again;
    case kTokenMinus:
        readToken();
        operand = (sTermNode_3BD8*)parseTerm();
        if (operand == 0)
            return 0;
        if (operand->type == kNodeConstant) {
            operand->value = -operand->value;
            return operand;
        }
        node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
        node->type = kNodeNeg;
        node->a = operand;
        return node;
    case kTokenNot:
        readToken();
        operand = (sTermNode_3BD8*)parseTerm();
        if (operand == 0)
            return 0;
        if (operand->type == kNodeConstant) {
            operand->value = operand->value == 0.0f ? 1.0f : 0.0f;
            return operand;
        }
        node = (sTermNode_3BD8*)cInputMapParser_newParseNode(this);
        node->type = kNodeNot;
        node->a = operand;
        return node;
    case kTokenOpenBracket:
        readToken();
        node = (sTermNode_3BD8*)parseExpression();
        if (CurrentToken != kTokenCloseBracket) {
            cInputMapParser_error(this, str_Expected_in_expression);
            cInputMapParser_freeParseNode(this, node);
            return 0;
        }
        readToken();
        return node;
    case kTokenMax:
        readToken();
        return parseBinaryFunc(kNodeMax);
    case kTokenMin:
        readToken();
        return parseBinaryFunc(kNodeMin);
    case kTokenPow:
        readToken();
        return parseBinaryFunc(kNodePow);
    case kTokenAtan2:
        readToken();
        return parseBinaryFunc(kNodeAtan2);
    case kTokenAtan:
        readToken();
        return parseUnaryFunc(kNodeAtan);
    case kTokenSin:
        readToken();
        return parseUnaryFunc(kNodeSin);
    case kTokenCos:
        readToken();
        return parseUnaryFunc(kNodeCos);
    case kTokenExp:
        readToken();
        return parseUnaryFunc(kNodeExp);
    case kTokenLog:
        readToken();
        return parseUnaryFunc(kNodeLog);
    case kTokenFloor:
        readToken();
        return parseUnaryFunc(kNodeFloor);
    case kTokenCeil:
        readToken();
        return parseUnaryFunc(kNodeCeil);
    case kTokenAbs:
        readToken();
        return parseUnaryFunc(kNodeAbs);
    case kTokenSqrt:
        readToken();
        return parseUnaryFunc(kNodeSqrt);
    case kTokenMinRadian:
        readToken();
        return parseUnaryFunc(kNodeMinRadian);
    default:
        cInputMapParser_error(this, str_Expected_term_in_expression);
        return 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseBinaryFunc);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void* cInputMapParser_parseExpression(cInputMapParser* self);
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" void* cInputMapParser_newParseNode(void* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" float func_0031C228(float x);
extern "C" float logf(float x);
extern "C" float expf(float x);
extern char str_Expected_in_expression_b[];
extern char str_Expected_in_expression_c[];
extern char str_Expected_in_expression[];

static inline float pow_4138(float x, float y)
{
    return expf(logf(x) * y);
}

static inline float atan2_4138(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += 3.1415927410125732f;
        } else {
            a -= 3.1415927410125732f;
        }
    }
    return a;
}

/**
 * Parses the "(a, b)" arguments of max, min, pow or atan2, folding the call when both are constants.
 *
 * When the second argument fails to parse, the first argument's tree is not freed.
 *
 * @param op  the node type to build (On Tour eNodeType type)
 * @return the call's tree, or 0 after an error
 */
// PORT: uses g++ min/max operators (<? >?).
void* cInputMapParser::parseBinaryFunc(int op)
{
    if (CurrentToken != kTokenOpenBracket) {
        cInputMapParser_error(this, str_Expected_in_expression_b);
        return 0;
    }
    readToken();
    tParseNode* firstArg = (tParseNode*)parseExpression();
    if (firstArg == 0) {
        return 0;
    }
    if (CurrentToken != kTokenComma) {
        cInputMapParser_error(this, str_Expected_in_expression_c);
        cInputMapParser_freeParseNode(this, firstArg);
        return 0;
    }
    readToken();
    tParseNode* secondArg = (tParseNode*)parseExpression();
    if (secondArg == 0) {
        return 0;
    }
    if (CurrentToken != kTokenCloseBracket) {
        cInputMapParser_error(this, str_Expected_in_expression);
        cInputMapParser_freeParseNode(this, firstArg);
        cInputMapParser_freeParseNode(this, secondArg);
        return 0;
    }
    readToken();
    if (firstArg->Type == kNodeConstant && secondArg->Type == kNodeConstant) {
        switch (op) {
        case kNodeMin:
            firstArg->Value = secondArg->Value <? firstArg->Value;
            break;
        case kNodeMax:
            firstArg->Value = secondArg->Value >? firstArg->Value;
            break;
        case kNodePow:
            firstArg->Value = pow_4138(firstArg->Value, secondArg->Value);
            break;
        case kNodeAtan2:
            firstArg->Value = atan2_4138(firstArg->Value, secondArg->Value);
            break;
        }
        cInputMapParser_freeParseNode(this, secondArg);
        return firstArg;
    }
    tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
    n->Type = op;
    n->Operand1 = firstArg;
    n->Operand2 = secondArg;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseUnaryFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float fSin(float x);
extern "C" float fCos(float x);

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float trunc_43A0(float x)
{
    float t;
    __asm__("cvt.w.s %0, %1\n\tcvt.s.w %0, %0" : "=f"(t) : "f"(x));
    return t;
}

static inline float floor_43A0(float x)
{
    float t = trunc_43A0(x);
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline float ceil_43A0(float x)
{
    float t = trunc_43A0(x);
    if (t < x) {
        t += 1.0f;
    }
    return t;
}

// PORT: PS2-only inline asm (EE sqrt.s).
static inline float sqrt_43A0(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only inline asm (abs.s).
static inline float abs_43A0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Parses the "(x)" argument of a one-argument function (atan to sqrt, minradian), folding a constant
 * argument.
 *
 * "minradian" wraps an angle into [-pi, pi).
 *
 * @param op  the node type to build (On Tour eNodeType type)
 * @return the call's tree, or 0 after an error
 */
// Parses "(expr)" after a unary function name; constant arguments are folded immediately.
void* cInputMapParser::parseUnaryFunc(int op)
{
    if (CurrentToken != kTokenOpenBracket) {
        cInputMapParser_error(this, str_Expected_in_expression_b);
        return 0;
    }
    readToken();
    tParseNode* operand = (tParseNode*)parseExpression();
    if (operand == 0) {
        return 0;
    }
    if (CurrentToken != kTokenCloseBracket) {
        cInputMapParser_error(this, str_Expected_in_expression);
        cInputMapParser_freeParseNode(this, operand);
        return 0;
    }
    readToken();
    if (operand->Type == kNodeConstant) {
        switch (op) {
        case kNodeAtan:
            operand->Value = func_0031C228(operand->Value);
            break;
        case kNodeSin:
            operand->Value = fSin(operand->Value);
            break;
        case kNodeCos:
            operand->Value = fCos(operand->Value);
            break;
        case kNodeExp:
            operand->Value = expf(operand->Value);
            break;
        case kNodeLog:
            operand->Value = logf(operand->Value);
            break;
        case kNodeFloor:
            operand->Value = floor_43A0(operand->Value);
            break;
        case kNodeCeil:
            operand->Value = ceil_43A0(operand->Value);
            break;
        case kNodeAbs:
            operand->Value = abs_43A0(operand->Value);
            break;
        case kNodeSqrt:
            operand->Value = sqrt_43A0(operand->Value);
            break;
        case kNodeMinRadian: {
            float x = operand->Value;
            operand->Value = x - floor_43A0(x * 0.159154937f + 0.5f) * 6.28318548f;
            break;
        }
        }
        return operand;
    }
    tParseNode* n = (tParseNode*)cInputMapParser_newParseNode(this);
    n->Type = op;
    n->Operand1 = operand;
    return n;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileStatement);
#ifdef SKIP_ASM
extern "C" tIdentifier* cInputMapParser_lookupIdentifierName_K(void* self, const char* name) __asm__("cInputMapParser_lookupIdentifierName");
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, tParseNode* node, int* src, void* code, int cap, int a5);
extern char D_0048E208[];
extern char str_opcode_buffer_overflow[];

/**
 * Compiles a named statement of the map into op-codes, ending them with a space (0x20) terminator.
 *
 * @param name  the statement's identifier (On Tour id)
 * @param out  the op-code buffer (On Tour codes)
 * @param max  room left in the buffer, in op-codes (On Tour clen)
 * @param defaultModifier  button modifier for buttons and references without one
 * @return the number of op-codes written including the terminator, or -1 on an unknown name or a full buffer
 */
// Compiles one named statement into out (space-terminated); returns the new length or -1.
int cInputMapParser::compileStatement(const char* name, int* out, int max, int defaultModifier)
{
    tIdentifier* ident = cInputMapParser_lookupIdentifierName_K(this, name);
    if (ident == 0) {
        cInputMapParser_error(this, D_0048E208, name);
        return -1;
    }
    int sep = ' ';
    int state = 0;
    int n = this->compileParseNode((tParseNode*)((void*)ident->Expression), &state, out, max,
            defaultModifier);
    max -= n;
    out += n;
    if (max <= 0) {
        cInputMapParser_error(this, str_opcode_buffer_overflow);
        return -1;
    }
    *out = sep;
    return n + 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileParseNode);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int cInputMapParser_configToRegister_i(void* self, int varIndex) __asm__("cInputMapParser_configToRegister");
extern "C" int cInputMapParser_buttonToRegister(cInputMapParser* self, int a1, int a2);
extern "C" int cInputMapParser_compileBinaryNode_K(cInputMapParser* self, void* node, int* dst, void* code, int cap, int op, int a6) __asm__("cInputMapParser_compileBinaryNode");
extern "C" int cInputMapParser_compileUnaryNode_K(cInputMapParser* self, void* node, int* dst, void* code, int cap, int op, int a6) __asm__("cInputMapParser_compileUnaryNode");
extern char str_opcode_buffer_overflow[];
extern char str_failed_to_allocate_temporary_register[];
extern char str_branch_offset_overflow[];

/**
 * Compiles an expression tree into op-codes whose result lands in a register, allocating one when needed.
 *
 * Constants load a value (two words); buttons and configs are read straight from their register when no
 * target
 * is given; references compile their identifier's expression; selections compile to a conditional jump, the
 * then
 * branch, a jump and the else branch; operators go to compileBinaryNode() or compileUnaryNode().
 *
 * @param node  the tree
 * @param dst  the result register in and out; -1 lets the node pick one (On Tour int& reg)
 * @param vcode  where to write the op-codes (On Tour const tInputOpCode* codes)
 * @param cap  room left in op-codes (On Tour clen)
 * @param defaultModifier  button modifier for buttons and references without one
 * @return the number of op-codes written, or -1 on error
 */
int cInputMapParser::compileParseNode(tParseNode* node, int* dst, void* vcode, int cap, int defaultModifier)
{
    tInputOpCode* code = (tInputOpCode*)vcode;
    switch (node->Type) {
    case cInputMapParser::kNodeConstant:
        if (*dst < 0) {
            if (NumFreeRegisters > 0) {
                *dst = FreeRegisters[--NumFreeRegisters];
            }
            if (*dst < 0) {
                cInputMapParser_error(this, str_failed_to_allocate_temporary_register);
                return -1;
            }
        }
        if (cap < 2) {
            cInputMapParser_error(this, str_opcode_buffer_overflow);
            return -1;
        }
        code->Binary.opcode = kInputOpCodeLoad;
        code->Binary.dest = *dst;
        code->Binary.operand1 = 0;
        code->Binary.operand2 = 0;
        code[1].Float.value = node->Value;
        return 2;
    case cInputMapParser::kNodeReference:
        return compileParseNode(node->Identifier->Expression, dst, code, cap,
                                                node->Modifier != kButtonModifier_Default ? node->Modifier
                                                                                          : defaultModifier);
    case cInputMapParser::kNodeButton: {
        int idx = buttonToRegister(node->Index,
                                         node->Modifier == kButtonModifier_Default ? defaultModifier : node->Modifier);
        if (*dst == cInputMapParser::kNoRegister) {
            *dst = idx;
            return 0;
        }
        if (cap <= 0) {
            cInputMapParser_error(this, str_opcode_buffer_overflow);
            return -1;
        }
        code->Binary.opcode = kInputOpCodeMove;
        code->Binary.dest = *dst;
        code->Binary.operand1 = idx;
        code->Binary.operand2 = 0;
        return 1;
    }
    case cInputMapParser::kNodeConfig:
        if (*dst == cInputMapParser::kNoRegister) {
            *dst = cInputMapParser_configToRegister_i(this, node->Index);
            return 0;
        }
        if (cap <= 0) {
            cInputMapParser_error(this, str_opcode_buffer_overflow);
            return -1;
        }
        code->Binary.opcode = kInputOpCodeMove;
        code->Binary.dest = *dst;
        code->Binary.operand1 = cInputMapParser_configToRegister_i(this, node->Index);
        code->Binary.operand2 = 0;
        return 1;
    case cInputMapParser::kNodeSelection: {
        int cond = cInputMapParser::kNoRegister;
        int condLen = compileParseNode(node->Operand1, &cond, code, cap, defaultModifier);
        if (condLen < 0) {
            return -1;
        }
        code += condLen;
        cap -= condLen;
        if (cap <= 0) {
            cInputMapParser_error(this, str_opcode_buffer_overflow);
            return -1;
        }
        unsigned int reg = cond;
        code->Branch.opcode = kInputOpCodeBranchFalse;
        code->Branch.operand = reg;
        if (reg < 0x40) {
            FreeRegisters[NumFreeRegisters++] = reg;
        }
        cap--;
        int thenLen = compileParseNode(node->Operand2, dst, code + 1, cap, defaultModifier);
        if (thenLen < 0) {
            return -1;
        }
        if (thenLen + 1 > 0xFFFF) {
            cInputMapParser_error(this, str_branch_offset_overflow);
            return -1;
        }
        code->Branch.offset = thenLen + 1;
        code += thenLen + 1;
        cap -= thenLen;
        if (cap <= 0) {
            cInputMapParser_error(this, str_opcode_buffer_overflow);
            return -1;
        }
        code->Branch.opcode = kInputOpCodeBranch;
        code->Branch.operand = 0;
        int elseLen = compileParseNode(node->Operand3, dst, code + 1, cap - 1, defaultModifier);
        if (elseLen < 0) {
            return -1;
        }
        if (elseLen > 0xFFFF) {
            cInputMapParser_error(this, str_branch_offset_overflow);
            return -1;
        }
        code->Branch.offset = elseLen;
        return condLen + thenLen + elseLen + 2;
    }
    case cInputMapParser::kNodeOr:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeOr, defaultModifier);
    case cInputMapParser::kNodeAnd:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeAnd, defaultModifier);
    case cInputMapParser::kNodeGR:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeGR, defaultModifier);
    case cInputMapParser::kNodeGE:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeGE, defaultModifier);
    case cInputMapParser::kNodeLS:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeLS, defaultModifier);
    case cInputMapParser::kNodeLE:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeLE, defaultModifier);
    case cInputMapParser::kNodeEQ:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeEQ, defaultModifier);
    case cInputMapParser::kNodeNE:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeNE, defaultModifier);
    case cInputMapParser::kNodeAdd:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeAdd, defaultModifier);
    case cInputMapParser::kNodeSub:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeSub, defaultModifier);
    case cInputMapParser::kNodeMul:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeMul, defaultModifier);
    case cInputMapParser::kNodeDiv:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeDiv, defaultModifier);
    case cInputMapParser::kNodeMax:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeMax, defaultModifier);
    case cInputMapParser::kNodeMin:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeMin, defaultModifier);
    case cInputMapParser::kNodePow:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodePow, defaultModifier);
    case cInputMapParser::kNodeAtan2:
        return cInputMapParser_compileBinaryNode_K(this, node, dst, code, cap, kInputOpCodeAtan2, defaultModifier);
    case cInputMapParser::kNodeAtan:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeAtan, defaultModifier);
    case cInputMapParser::kNodeSin:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeSin, defaultModifier);
    case cInputMapParser::kNodeCos:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeCos, defaultModifier);
    case cInputMapParser::kNodeExp:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeExp, defaultModifier);
    case cInputMapParser::kNodeLog:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeLog, defaultModifier);
    case cInputMapParser::kNodeFloor:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeFloor, defaultModifier);
    case cInputMapParser::kNodeCeil:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeCeil, defaultModifier);
    case cInputMapParser::kNodeAbs:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeAbs, defaultModifier);
    case cInputMapParser::kNodeSqrt:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeSqrt, defaultModifier);
    case cInputMapParser::kNodeMinRadian:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeMinRadian, defaultModifier);
    case cInputMapParser::kNodeNeg:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeNeg, defaultModifier);
    case cInputMapParser::kNodeNot:
        return cInputMapParser_compileUnaryNode_K(this, node, dst, code, cap, kInputOpCodeNot, defaultModifier);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileBinaryNode);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, tParseNode* node, int* src, void* code, int cap, int a5);
extern char str_failed_to_allocate_temporary_register[];
extern char str_opcode_buffer_overflow[];

/**
 * Compiles both operands of a binary operator, frees their temporary registers and emits the operation.
 *
 * @param node  the operator's tree node
 * @param dst  the result register in and out; a negative value allocates one (On Tour int& reg)
 * @param code  where to write the op-codes
 * @param cap  room left in op-codes (On Tour clen)
 * @param op  the op-code to emit (On Tour eInputOpCodes opcode)
 * @param defaultModifier  button modifier passed down to the operands
 * @return the number of op-codes written, or -1 on error
 */
int cInputMapParser::compileBinaryNode(tParseNode* node, int* dst, tInputOpCode* code, int cap, int op,
                                       int defaultModifier)
{
    int src = -1;
    int srcLen = compileParseNode(node->Operand1, &src, code, cap, defaultModifier);
    if (srcLen < 0) {
        return -1;
    }
    code += srcLen;
    cap -= srcLen;
    int argReg = -1;
    int argLen = compileParseNode(node->Operand2, &argReg, code, cap, defaultModifier);
    if (argLen < 0) {
        return -1;
    }
    code += argLen;
    cap -= argLen;
    if ((unsigned int)argReg < 0x40) {
        FreeRegisters[NumFreeRegisters++] = argReg;
    }
    if ((unsigned int)src < 0x40) {
        FreeRegisters[NumFreeRegisters++] = src;
    }
    if (*dst < 0) {
        if (NumFreeRegisters > 0) {
            *dst = FreeRegisters[--NumFreeRegisters];
        }
        if (*dst < 0) {
            cInputMapParser_error(this, str_failed_to_allocate_temporary_register);
            return -1;
        }
    }
    if (cap <= 0) {
        cInputMapParser_error(this, str_opcode_buffer_overflow);
        return -1;
    }
    code->Binary.opcode = op;
    code->Binary.dest = *dst;
    code->Binary.operand1 = src;
    code->Binary.operand2 = argReg;
    return srcLen + argLen + 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileUnaryNode);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, tParseNode* node, int* src, void* code, int cap, int a5);
extern char str_failed_to_allocate_temporary_register[];
extern char str_opcode_buffer_overflow[];

/**
 * Compiles the operand of a unary operator, frees its temporary register and emits the operation.
 *
 * @param node  the operator's tree node
 * @param dst  the result register in and out; a negative value allocates one (On Tour int& reg)
 * @param code  where to write the op-codes
 * @param cap  room left in op-codes (On Tour clen)
 * @param op  the op-code to emit (On Tour eInputOpCodes opcode)
 * @param defaultModifier  button modifier passed down to the operand
 * @return the number of op-codes written, or -1 on error
 */
int cInputMapParser::compileUnaryNode(tParseNode* node, int* dst, tInputOpCode* code, int cap, int op,
                                      int defaultModifier)
{
    tParseNode* operand = node->Operand1;
    int src = -1;
    int n = compileParseNode(operand, &src, code, cap, defaultModifier);
    if (n < 0) {
        return -1;
    }
    code += n;
    cap -= n;
    if ((unsigned int)src < 0x40) {
        FreeRegisters[NumFreeRegisters++] = src;
    }
    if (*dst < 0) {
        if (NumFreeRegisters > 0) {
            *dst = FreeRegisters[--NumFreeRegisters];
        }
        if (*dst < 0) {
            cInputMapParser_error(this, str_failed_to_allocate_temporary_register);
            return -1;
        }
    }
    if (cap <= 0) {
        cInputMapParser_error(this, str_opcode_buffer_overflow);
        return -1;
    }
    code->Binary.opcode = op;
    code->Binary.dest = *dst;
    code->Binary.operand1 = src;
    code->Binary.operand2 = 0;
    return n + 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_cInputExec__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an input map evaluator: sets its default button modifier (+0x0) to 0.
 *
 * cInputMap_getValue, cInputMap_getState and their modifier overloads build a 0x190-byte evaluator on the stack with it
 * before running cInputExec_execute.
 *
 * @return @p self
 */
void* cInputExec_cInputExec(void* vself)
{
    cInputExec* self = (cInputExec*)vself;
    *(int*)self = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_getRegister);
#ifdef SKIP_ASM
struct sInputParse_325260 {
    int pad[2];
    float v[0x40];
};
extern "C" float cInputExec_getButton(void* self, int idx, int sel);
extern "C" float cInputExec_getConfig(cInputExec* self, int a1);

/**
 * Reads an input register: a temporary, a config value or a button.
 *
 * Registers 0..0x3F are the temporaries (+0x8), 0x40..0x5F the config values (cInputExec_getConfig) and 0x60.. buttons: id -
 * 0x60 is split into button = n / 6 and modifier = n % 6 for cInputExec_getButton. Numbers from 0x60 + 924 up read as 0.
 *
 * @param id  the register number
 * @return the register's value
 */
float cInputExec::getRegister(unsigned int id)
{
    if (id < 0x40) {
        return Temp[id];
    }
    if (id - 0x40 < 0x20) {
        return getConfig(id - 0x40);
    }
    id -= 0x60;
    if (id < 924) {
        return cInputExec_getButton(this, (int)id / 6, (int)id % 6);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_getTempReg);
#ifdef SKIP_ASM

/**
 * Returns the address of one of an input map evaluator's temporary registers.
 *
 * Temporaries are the 64 floats of Temp; no range check.
 *
 * @param i  the temporary register index
 * @return the register's address
 */
void* cInputExec::getTempReg(int i)
{
    return &Temp[i];
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_getButton);
#ifdef SKIP_ASM



static inline unsigned int GetA3252F8(cInputPad* l, int idx)
{
    if (idx < l->NumButtons) {
        return ((unsigned int)((&l->Buttons[0]))[idx].IsPressed);
    }
    return 0;
}

static inline unsigned int GetB3252F8(cInputPad* l, int idx)
{
    if (idx < l->NumButtons) {
        return ((unsigned int)((&l->Buttons[0]))[idx].IsReleased);
    }
    return 0;
}

static inline unsigned int GetC3252F8(cInputPad* l, int idx)
{
    if (idx < l->NumButtons) {
        return ((unsigned int)((&l->Buttons[0]))[idx].IsHeld);
    }
    return 0;
}

static inline unsigned int GetD3252F8(cInputPad* l, int idx)
{
    if (idx < l->NumButtons) {
        return ((unsigned int)((&l->Buttons[0]))[idx].IsRepeated);
    }
    return 0;
}

// PORT: the unit declares cInputExec_getButton as (void*, int, int); the definition takes an unsigned kind.

/**
 * Reads one button of the pad being evaluated, as its analogue value or as one of its state flags.
 *
 * @p kind (eButtonModifiers) picks the field of button @p idx of the pad (+0x4): kButtonModifier_Value the value, Held
 * (+0xC), Pressed (+0x4), Released (+0x8) or Repeat (+0x10) the flag converted to 0.0 / 1.0. kButtonModifier_Default
 * uses the evaluator's default modifier (+0x0) instead; if that is itself the default it recurses forever. A button
 * past the pad's count, or an unknown kind, reads 0.
 *
 * @param idx  the button index
 * @param kind  the button modifier (eButtonModifiers)
 * @return the value
 */
float cInputExec::getButton(int idx, unsigned int kind)
{
    switch (kind) {
    case kButtonModifier_Value:
        if (idx < Pad->NumButtons) {
            return ((&Pad->Buttons[0]))[idx].Value;
        }
        break;
    case kButtonModifier_Held:
        return (float)GetC3252F8((Pad), idx);
    case kButtonModifier_Pressed:
        return (float)GetA3252F8((Pad), idx);
    case kButtonModifier_Released:
        return (float)GetB3252F8((Pad), idx);
    case kButtonModifier_Repeat:
        return (float)GetD3252F8((Pad), idx);
    case kButtonModifier_Default:
        return getButton(idx, DefaultButtonModifier);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_getConfig);
#ifdef SKIP_ASM
/**
 * Returns one of an input map evaluator's config values; config 0 always reads 0.
 *
 * The values are the floats of Config. cInputMap_getValue1 and cInputMap_getState1 fill configs 1..8.
 *
 * @param varIndex  the config index
 * @return the value, or 0.0 for config 0
 */
float cInputExec::getConfig(int varIndex)
{
    if (varIndex == 0) {
        return 0.0f;
    }
    return Config[varIndex];
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputExec_execute);
#ifdef SKIP_ASM

// PORT: cInputExec_getRegister is declared with its own self struct earlier in the unit.
extern "C" float cInputExec_getRegister_vm(cInputExec* self, unsigned int id) __asm__("cInputExec_getRegister");

/**
 * Runs a compiled input map statement against a controller pad and returns its result.
 *
 * Stores @p list as the evaluator's Pad and steps through the op-codes from @p code until op 32 (End). The op
 * numbers are On Tour's eInputOpCodes: 0 Load (the next word is a float constant), 1 Move, 2 BranchFalse and 3 Branch
 * (relative jumps), 4 Or, 5 And, 6..11 the comparisons (>, >=, <, <=, ==, !=) giving 1.0 or 0.0, 12..15 + - x /, 16
 * Max, 17 Min, 18 Pow (computed as exp(log(x) x y), so a negative base gives NaN), 19 Atan2, 20 Atan, 21 Sin, 22 Cos,
 * 23 Exp, 24 Log, 25 Floor, 26 Ceil, 27 Abs, 28 Sqrt, 29 MinRadian (wraps an angle into [-pi, pi)), 30 Neg and 31 Not.
 * Operands are read with cInputExec_getRegister and results written through cInputExec_getTempReg. Unknown op-codes are skipped. The
 * encoder is cInputMapParser_compileParseNode.
 *
 * @param list  the controller pad to read (On Tour cInputPad*)
 * @param code  the statement's op-codes (On Tour program)
 * @return the value of register 0 when End is reached
 */
// Runs one compiled input expression (see cInputMapParser_compileParseNode for the encoder); returns register 0.
float cInputExec::execute(cInputPad* list, const tInputOpCode* code)
{
    int pc = 0;
    int done = 0;
    Pad = (cInputPad*)list;
    do {
        const tInputOpCode* ins = &code[pc];
        pc++;
        switch (ins->Base.opcode) {
        case kInputOpCodeLoad: {
            float* destSlot = (float*)getTempReg(ins->Load.dest);
            *destSlot = code[pc++].Float.value;
            break;
        }
        case kInputOpCodeMove: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = cInputExec_getRegister_vm((this), ins->Unary.operand);
            break;
        }
        case kInputOpCodeBranchFalse:
            if (cInputExec_getRegister_vm((this), ins->Branch.operand) != 0.0f) {
                break;
            }
        case kInputOpCodeBranch:
            pc += ins->Branch.offset;
            break;
        case kInputOpCodeOr: {
            unsigned int boolResult = 0;
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            if (cInputExec_getRegister_vm((this), ins->Binary.operand1) != 0.0f
                || cInputExec_getRegister_vm((this), ins->Binary.operand2) != 0.0f) {
                boolResult = 1;
            }
            *destSlot = boolResult;
            break;
        }
        case kInputOpCodeAnd: {
            unsigned int boolResult = 0;
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            if (cInputExec_getRegister_vm((this), ins->Binary.operand1) != 0.0f
                && cInputExec_getRegister_vm((this), ins->Binary.operand2) != 0.0f) {
                boolResult = 1;
            }
            *destSlot = boolResult;
            break;
        }
        case kInputOpCodeGR: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs > cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeGE: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs >= cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeLS: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs < cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeLE: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs <= cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeEQ: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs == cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeNE: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = (lhs != cInputExec_getRegister_vm((this), ins->Binary.operand2)) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeAdd: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = lhs + cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodeSub: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = lhs - cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodeMul: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = lhs * cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodeDiv: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = lhs / cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodeMax: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            *destSlot = (cInputExec_getRegister_vm((this), ins->Binary.operand1)
                         > cInputExec_getRegister_vm((this), ins->Binary.operand2))
                ? cInputExec_getRegister_vm((this), ins->Binary.operand1)
                : cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodeMin: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            *destSlot = (cInputExec_getRegister_vm((this), ins->Binary.operand1)
                         < cInputExec_getRegister_vm((this), ins->Binary.operand2))
                ? cInputExec_getRegister_vm((this), ins->Binary.operand1)
                : cInputExec_getRegister_vm((this), ins->Binary.operand2);
            break;
        }
        case kInputOpCodePow: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = pow_4138(lhs, cInputExec_getRegister_vm((this), ins->Binary.operand2));
            break;
        }
        case kInputOpCodeAtan2: {
            float* destSlot = (float*)getTempReg(ins->Binary.dest);
            float lhs = cInputExec_getRegister_vm((this), ins->Binary.operand1);
            *destSlot = atan2_4138(lhs, cInputExec_getRegister_vm((this), ins->Binary.operand2));
            break;
        }
        case kInputOpCodeAtan: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = func_0031C228(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeSin: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = fSin(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeCos: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = fCos(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeExp: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = expf(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeLog: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = logf(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeFloor: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = floor_43A0(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeCeil: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = ceil_43A0(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeAbs: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = abs_43A0(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeSqrt: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = sqrt_43A0(cInputExec_getRegister_vm((this), ins->Unary.operand));
            break;
        }
        case kInputOpCodeMinRadian: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            float x = cInputExec_getRegister_vm((this), ins->Unary.operand);
            *destSlot = x - floor_43A0(x * 0.159154937f + 0.5f) * 6.28318548f;
            break;
        }
        case kInputOpCodeNeg: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = -cInputExec_getRegister_vm((this), ins->Unary.operand);
            break;
        }
        case kInputOpCodeNot: {
            float* destSlot = (float*)getTempReg(ins->Unary.dest);
            *destSlot = (cInputExec_getRegister_vm((this), ins->Unary.operand) == 0.0f) ? 1.0f : 0.0f;
            break;
        }
        case kInputOpCodeEnd:
            done = 1;
            break;
        }
    } while (!done);
    return cInputExec_getRegister_vm((this), 0);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_loadFileSize);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern "C" char* strrchr(const char* s, int c);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
// PORT: cInputPreProcessor_readFile is declared (void*, int, int) later in the unit; this caller passes two strings.
void cInputPreProcessor_readFile_s(void* self, char* dir, char* file) __asm__("cInputPreProcessor_readFile");
extern char str_PreProcBuf[];


/**
 * Loads an input-map file with its "#include"s expanded and returns the result in a new buffer.
 *
 * Splits @p path into directory and file name, runs readFile() into a growing "PreProcBuf", then copies the
 * text
 * into a block tagged with @p path.
 *
 * @param path  the file to load (On Tour fn)
 * @param outSize  receives the text's size in bytes
 * @return the preprocessed text (not null-terminated)
 */
void* cInputPreProcessor::loadFileSize(char* path, int* outSize)
{
    char dir[128];
    char file[32];
    OutputTop = 0;
    OutputLen = 0x800;
    OutputBuf = (char*)operator_new_tag(0x800, str_PreProcBuf, 0, 0);
    char* lastSlash = strrchr(path, '/');
    if (lastSlash != 0) {
        lastSlash++;
        int n = lastSlash - path;
        strncpy(dir, path, n);
        dir[n] = 0;
        strcpy(file, lastSlash);
    } else {
        dir[0] = 0;
        strcpy(file, path);
    }
    cInputPreProcessor_readFile_s((this), dir, file);
    void* out = operator_new_tag(OutputTop, path, 0x100, 0);
    func_0041605C(out, OutputBuf, OutputTop);
    if (OutputBuf != 0) {
        cMemMan_free(OutputBuf);
    }
    *outSize = OutputTop;
    return out;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readFile);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cMemMan_free(void* ptr);
extern "C" void cInputPreProcessor_readChar(cInputPreProcessor* self, tInputSource* s);
extern "C" void cInputPreProcessor_writeTagLine(cInputPreProcessor* self, int a1, int a2);
extern "C" bool cInputPreProcessor_copyLine(cInputPreProcessor* self, tInputSource* s, int a2, int a3);
extern "C" unsigned char* FILE_loadsize(const char* name, int* size, int flags);
extern char D_004A3FA0[];


/**
 * Reads one file into the output, line by line, starting with a "# 1 "file"" tag line.
 *
 * @param path  directory of the file, also used for its includes
 * @param fn  file name within @p path
 */
void cInputPreProcessor::readFile(int path, int fn)
{
    char name[160];
    tInputSource st;
    sprintf(name, D_004A3FA0, path, fn);
    unsigned char* fileData = FILE_loadsize(name, &st.DataLen, 0);
    (*(unsigned char**)&st.ReadPtr) = (*(unsigned char**)&st.Data) = fileData;
    st.LineNumber = 1;
    readChar(&st);
    // PORT: the unit declares cInputPreProcessor_writeTagLine's third parameter as int; it receives a char*.
    writeTagLine(st.LineNumber, (int)name);
    while (copyLine(&st, path, fn)) {
    }
    if (st.Data) {
        cMemMan_free(((unsigned char*)st.Data));
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_copyLine);
#ifdef SKIP_ASM
extern "C" void cInputPreProcessor_writeChar(cInputPreProcessor* self, char c);
extern "C" void cInputPreProcessor_readChar(cInputPreProcessor* self, tInputSource* s);
extern "C" void cInputPreProcessor_readCmdLine(cInputPreProcessor* self, tInputSource* s, int a2, int a3);
extern "C" void cInputPreProcessor_skipCComment(cInputPreProcessor* self, tInputSource* s, int a2, int a3);
extern "C" void cInputPreProcessor_skipCppComment(cInputPreProcessor* self, tInputSource* s, int a2, int a3);

static inline int isWS_326150(int c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

/**
 * Copies one line of the source to the output, handling a "#" directive or a comment at the line start.
 *
 * Leading blanks are copied; line and block comments are passed to cInputPreProcessor_skipCppComment() and cInputPreProcessor_skipCComment().
 *
 * @param isrc  the input source (On Tour tInputSource&)
 * @param path  directory of the file, for includes and messages
 * @param fn  file name, for messages
 * @return true while the source has more characters
 */
bool cInputPreProcessor::copyLine(tInputSource* isrc, int path, int fn)
{
    int ch;
    for (;;) {
        ch = CurrentChar;
        if (!isWS_326150(ch))
            break;
        writeChar((char)CurrentChar);
        readChar(isrc);
        if (CurrentChar < 0)
            return false;
    }
    if (ch == '#') {
        readChar(isrc);
        readCmdLine(isrc, path, fn);
        return CurrentChar >= 0;
    }
    if (ch == '/') {
        readChar(isrc);
        int nextChar = CurrentChar;
        if (nextChar == '/') {
            skipCppComment(isrc, path, fn);
            return CurrentChar >= 0;
        }
        if (nextChar == '*') {
            readChar(isrc);
            skipCComment(isrc, path, fn);
            return CurrentChar >= 0;
        }
        writeChar('/');
    }
    while (CurrentChar >= 0) {
        writeChar((char)CurrentChar);
        if (CurrentChar == '\n') {
            isrc->LineNumber += 1;
            readChar(isrc);
            break;
        }
        readChar(isrc);
    }
    return CurrentChar >= 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_writeString);
#ifdef SKIP_ASM
extern "C" void cInputPreProcessor_writeChar(cInputPreProcessor* self, char c);

/**
 * Writes a null-terminated string to the preprocessor output, one cInputPreProcessor_writeChar() per character.
 *
 * @param str  the string to write
 */
void cInputPreProcessor::writeString(char* str)
{
    while (*str != 0) {
        writeChar(*str++);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_writeChar);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern char str_PreProcBuf[];


/**
 * Appends a character to the output buffer, growing it by 1 KB or half its size, whichever is larger.
 *
 * @param ch  the character
 */
void cInputPreProcessor::writeChar(char ch)
{
    cInputPreProcessor* outBuf = this;
    if (outBuf->OutputTop == outBuf->OutputLen) {
        int newCap = outBuf->OutputTop + 0x400;
        int grow = outBuf->OutputTop * 3 / 2;
        if (newCap < grow) {
            newCap = grow;
        }
        char* newData = (char*)operator_new_tag(newCap, str_PreProcBuf, 0, 0);
        func_0041605C(newData, outBuf->OutputBuf, outBuf->OutputLen);
        if (outBuf->OutputBuf) {
            cMemMan_free(outBuf->OutputBuf);
        }
        outBuf->OutputBuf = newData;
        outBuf->OutputLen = newCap;
    }
    outBuf->OutputBuf[outBuf->OutputTop++] = ch;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputPreProcessor_writeTagLine);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cInputPreProcessor_writeString(cInputPreProcessor* self, char* msg);
extern char D_0048E3C8[];

/**
 * Writes a "# <line> "<name>"" tag line so the parser can report the right file and line.
 *
 * @param line  line number in the source
 * @param name  file name (passed as an int in this unit)
 */
void cInputPreProcessor::writeTagLine(int line, int name)
{
    char buf[176];
    sprintf(buf, D_0048E3C8, line, name);
    writeString(buf);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readChar);
#ifdef SKIP_ASM
/**
 * Reads the source's next byte into the preprocessor's current character, or -1 at the end.
 *
 * @param isrc  the input source (On Tour tInputSource&)
 */
void cInputPreProcessor::readChar(tInputSource* isrc)
{
    if (isrc->ReadPtr == isrc->Data + isrc->DataLen) {
        CurrentChar = -1;
    } else {
        CurrentChar = *isrc->ReadPtr++;
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_skipCComment);
#ifdef SKIP_ASM
// PORT: cInputPreProcessor_error__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void cInputPreProcessor_error_5(void* self, tInputSource* s, int a2, int a3, const char* msg) __asm__("cInputPreProcessor_error__FPv");
extern char str_unexpected_EOF_in_comment[];

/**
 * Skips a block comment, keeping its line breaks so line numbers stay right.
 *
 * Reads characters (the current one is at +0xC) until the closing star-slash: each newline inside counts a line on
 * @p isrc (+0xC) and is written out, and a comment without newlines is replaced by one space. When the input ends first
 * it calls the error reporter cInputPreProcessor_error with "unexpected EOF in comment"; that function is an empty stub in retail,
 * so nothing is reported.
 *
 * @param isrc      the input source (On Tour tInputSource); its line counter (+0xC) is advanced
 * @param path      directory of the file, for the error message
 * @param fileName  file name, for the error message
 */
// Skips a /* */ comment; newlines inside it are kept, otherwise it becomes one space.
void cInputPreProcessor::skipCComment(tInputSource* isrc, int path, int fileName)
{
    int single = 1;
    int run = 1;
    while (run) {
        int ch = CurrentChar;
        if (ch == '\n') {
            single = 0;
            isrc->LineNumber += 1;
            writeChar((char)CurrentChar);
        } else if (ch == '*') {
            readChar(isrc);
            if (CurrentChar != '/')
                continue;
            readChar(isrc);
            if (single) {
                writeChar(' ');
            }
            return;
        } else if (ch < 0) {
            goto error;
        }
        readChar(isrc);
    }
error:
    cInputPreProcessor_error_5(this, isrc, path, fileName, str_unexpected_EOF_in_comment);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_skipCppComment);
#ifdef SKIP_ASM
extern "C" void cInputPreProcessor_readChar(cInputPreProcessor* self, tInputSource* s);
// PORT: cInputPreProcessor_error__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void cInputPreProcessor_error_5(void* self, tInputSource* s, int a2, int a3, const char* msg) __asm__("cInputPreProcessor_error__FPv");
extern char str_unexpected_EOF_in_comment[];

/**
 * Skips a line comment up to the end of the line.
 *
 * Reads characters until a newline, which is counted on @p isrc, written out and consumed; reports "unexpected EOF in
 * comment" through cInputPreProcessor_error when the input ends first.
 *
 * @param isrc   the input source (On Tour tInputSource)
 * @param path  the file path, for the error message
 * @param fileName  the file name, for the error message
 */
void cInputPreProcessor::skipCppComment(tInputSource* isrc, int path, int fileName)
{
    for (;;) {
        int ch = CurrentChar;
        if (ch == '\n') {
            isrc->LineNumber += 1;
            writeChar((char)CurrentChar);
            readChar(isrc);
            return;
        }
        if (ch < 0) {
            goto error;
        }
        readChar(isrc);
    }
error:
    cInputPreProcessor_error_5(this, isrc, path, fileName, str_unexpected_EOF_in_comment);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_skipToEOL);
#ifdef SKIP_ASM
/**
 * Skips the rest of the current line, including its newline, and counts the line.
 *
 * @param isrc  the input source (On Tour tInputSource&)
 */
void cInputPreProcessor::skipToEOL(tInputSource* isrc)
{
    while (CurrentChar >= 0) {
        readChar(isrc);
        if (CurrentChar == '\n') {
            readChar(isrc);
            isrc->LineNumber += 1;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readCmdLine);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cInputPreProcessor_readChar(cInputPreProcessor* self, tInputSource* s);
extern "C" void cInputPreProcessor_writeTagLine(cInputPreProcessor* self, int a1, int a2);
extern "C" void cInputPreProcessor_readFile(cInputPreProcessor* self, int a1, int a2);
extern "C" void cInputPreProcessor_skipToEOL(cInputPreProcessor* self, tInputSource* s);
extern "C" int strcmp(const char* a, const char* b);
// PORT: cInputPreProcessor_error__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void cInputPreProcessor_error_5(void* self, tInputSource* s, int a2, int a3, const char* msg) __asm__("cInputPreProcessor_error__FPv");

static inline int isWS_3266E8(int c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

static inline int isAlpha_3266E8(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/**
 * Handles a "#" directive: only #include "file", which reads the file (relative to @p path) and writes a tag
 * line back.
 *
 * Any other directive or a malformed include is reported and the line skipped. The included name is copied
 * into
 * a 160-byte buffer without a length check.
 *
 * @param isrc  the input source (On Tour tInputSource&)
 * @param path  directory of the current file
 * @param fn  name of the current file
 */
void cInputPreProcessor::readCmdLine(tInputSource* isrc, int path, int fn)
{
    char word[16];
    char name[160];
    char buf[160];
    int ch;
    while (isWS_3266E8(ch = CurrentChar))
        readChar(isrc);
    if (ch <= 0) {
        cInputPreProcessor_error_5(this, isrc, path, fn, "unexpected EOF in pre-processor command");
        return;
    }
    char* wordPtr = word;
    do {
        if (!isAlpha_3266E8(CurrentChar))
            break;
        *wordPtr = CurrentChar;
        readChar(isrc);
        wordPtr++;
    } while (wordPtr - word < 15);
    *wordPtr = 0;
    if (strcmp(word, "include") == 0) {
        while (isWS_3266E8(ch = CurrentChar))
            readChar(isrc);
        if (ch != '"') {
            cInputPreProcessor_error_5(this, isrc, path, fn, "illegal 'include' directive");
            skipToEOL(isrc);
            return;
        }
        readChar(isrc);
        int n = 0;
        while (CurrentChar != '"') {
            int nameChar = CurrentChar;
            if (nameChar < 0 || nameChar == '\n') {
                cInputPreProcessor_error_5(this, isrc, path, fn, "illegal 'include' directive");
                skipToEOL(isrc);
                return;
            }
            name[n++] = CurrentChar;
            readChar(isrc);
        }
        name[n] = 0;
        readChar(isrc);
        while (isWS_3266E8(ch = CurrentChar))
            readChar(isrc);
        if (ch >= 0 && ch != '\n') {
            cInputPreProcessor_error_5(this, isrc, path, fn, "illegal 'include' directive");
            skipToEOL(isrc);
            return;
        }
        if (ch == '\n')
            isrc->LineNumber += 1;
        // PORT: the unit declares cInputPreProcessor_readFile's third parameter as int; it receives a char*.
        readFile(path, (int)name);
        sprintf(buf, "%s%s", path, fn);
        writeTagLine(isrc->LineNumber, (int)buf);
        readChar(isrc);
    } else {
        cInputPreProcessor_error_5(this, isrc, path, fn, "illegal pre-processor directive");
        skipToEOL(isrc);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_error__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the input preprocessor's error report, compiled out of this build.
 *
 * cInputPreProcessor_skipCComment, cInputPreProcessor_skipCppComment and cInputPreProcessor_readCmdLine call it with
 * the input source, path, file name and a message such as "unexpected EOF in comment"; the arguments are ignored.
 */
void cInputPreProcessor::error()
{
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_init);
#ifdef SKIP_ASM
extern "C" int cPadDevice_construct(void* p, int n);

/**
 * Initialises the pad cache: creates the pad devices and resets the ring buffer positions.
 *
 * Stores the count cPadDevice_construct returns for the device array (+0x2EEC, up to 4) in +0x2EE8, sets the read position
 * (+0x2EE0) to 29 and the write position (+0x2EE4) to 0. Called by cSSXApp_init.
 */
void cPadCache::init()
{
    NumPadDevices = cPadDevice_construct(&PadDevices[0], 4);
    CacheReadPos = (unsigned int)29;
    CacheWritePos = (unsigned int)0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_purge);
#ifdef SKIP_ASM
/**
 * Destroys every pad device of the pad cache.
 *
 * Calls each non-null device's virtual destructor (slot 1) with flags 3, which frees it. The array entries and the
 * count (NumPadDevices) are left as they were. Called by cSSXApp_purge.
 */
void cPadCache::purge()
{
    int i;
    for (i = 0; i < NumPadDevices; i++) {
        cPadDevice* inputPad = PadDevices[i];
        if (inputPad != 0) {
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)inputPad->mVtbl;
            vt[1].fn(vthis(inputPad, vt[1]), 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_clearAllVibrations);
#ifdef SKIP_ASM
extern "C" void cPadDevice_clearAllVibrations(cPadDevice* inputPad);

/**
 * Stops the vibration of every pad device of the pad cache.
 *
 * Calls cPadDevice_clearAllVibrations on each device. Called by cGame_update and cGame_updateUI.
 */
void cPadCache::clearAllVibrations()
{
    int i;
    for (i = 0; i < NumPadDevices; i++) {
        PadDevices[i]->clearAllVibrations();
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_nextInput);
#ifdef SKIP_ASM
struct sInputSlot_00326B48 {
    int value;
    char data[0x60];
};

struct sInputParse_00326B48 {
    sInputSlot_00326B48 slots[30][4];
    unsigned int head;
    unsigned int tail;
};

/**
 * Advances the pad cache's read position to the next cached input frame, if there is one.
 *
 * Moves the read position (+0x2EE0) one step round the 30-line ring unless that would reach the write position
 * (+0x2EE4). Called once per game tick by cSSXApp_preUpdate.
 *
 * @return true when it advanced, false when no newer frame is cached
 */
bool cPadCache::nextInput()
{
    unsigned int next = (CacheReadPos + 1) % 30;
    if (next == CacheWritePos) {
        return false;
    }
    CacheReadPos = next;
    return true;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_cacheInput);
#ifdef SKIP_ASM
/**
 * Reads every pad device into the next slot of the 30-entry input ring buffer.
 */
void cPadCache::cacheInput()
{
    int i;
    for (i = 0; i < NumPadDevices; i++) {
        cPadDevice* device = PadDevices[i];
        sVEntry<int(void*, float*, int)>* vt = (sVEntry<int(void*, float*, int)>*)device->mVtbl;
        InputCache[CacheWritePos][i].NumButtons =
            vt[2].fn(vthis(device, vt[2]), InputCache[CacheWritePos][i].Buttons, 0x18);
    }
    CacheWritePos = (CacheWritePos + 1) % 30;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_flush);
#ifdef SKIP_ASM
struct sInputSlot_00326C60 {
    int value;
    char data[0x60];
};

struct sInputParse_00326C60 {
    sInputSlot_00326C60 slots[30][4];
    int head;
    int tail;
};

/**
 * Counts the input frames cached since the last flush and moves the read position to the newest one.
 *
 * @return the number of cached frames
 */
int cPadCache::flush()
{
    int tail = (int)CacheWritePos;
    int head = (int)CacheReadPos;
    CacheReadPos = (unsigned int)((tail + 29) % 30);
    return (tail - head + 30) % 30;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_numButtons);
#ifdef SKIP_ASM
/**
 * Returns how many button values a pad delivered in the pad cache's current input frame.
 *
 * Reads NumButtons of line CacheReadPos, pad @p i. Called by cSSXApp_preUpdate and
 * cPreStartScreen_update together with cPadCache_buttonValues.
 *
 * @param i  the pad index (On Tour pad)
 * @return the number of button values
 */
int cPadCache::numButtons(int i)
{
    return InputCache[(int)CacheReadPos][i].NumButtons;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_buttonValues);
#ifdef SKIP_ASM
/**
 * Returns the button values a pad delivered in the pad cache's current input frame.
 *
 * The values (Buttons, up to 24 floats) of line CacheReadPos, pad @p i.
 *
 * @param i  the pad index (On Tour pad)
 * @return the address of the values
 */
void* cPadCache::buttonValues(int i)
{
    return InputCache[(int)CacheReadPos][i].Buttons;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadDevice_clearAllVibrations);
#ifdef SKIP_ASM

// cPadDevice's virtual numActuators (slot 3) and setActuator (slot 4), called through the vtable.
static inline int padNumActuators(cPadDevice* pad)
{
    sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)pad->mVtbl;
    return vt[3].fn(vthis(pad, vt[3]));
}

static inline void padSetActuator(cPadDevice* pad, int act, float level)
{
    sVEntry<void(void*, int, float)>* vt = (sVEntry<void(void*, int, float)>*)pad->mVtbl;
    vt[4].fn(vthis(pad, vt[4]), act, level);
}

/**
 * Stops a pad device's vibration: sets every actuator to 0.
 *
 * Calls the device's virtual setActuator (slot 4) with level 0.0 for each actuator it reports through slot 3. Called by
 * cPadCache_clearAllVibrations and updatePadVibration.
 *
 */
void cPadDevice::clearAllVibrations()
{
    int i;
    for (i = 0; i < padNumActuators(this); i++) {
        padSetActuator(this, i, 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadDevice_construct);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cPS2PadDevice_cPS2PadDevice(cPS2PadDevice* self, int a1, int a2);
extern char str_PS2Pad[];

/**
 * Creates the PS2 pad devices: one per controller port, at most two.
 *
 * Clamps @p n to 2 and fills @p inputPads[i] with a 0x180-byte "PS2Pad" allocation constructed by cPS2PadDevice_cPS2PadDevice for
 * port i, slot 0.
 *
 * @param inputPads  the array to fill (On Tour pads)
 * @param n  its capacity (On Tour maxpads)
 * @return the number of devices created
 */
extern "C" int cPadDevice_construct(void* inputPads, int n)
{
    void** arr = (void**)inputPads;
    int i;
    if (n > 2) {
        n = 2;
    }
    for (i = 0; i < n; i++) {
        arr[i] = cPS2PadDevice_cPS2PadDevice((cPS2PadDevice*)cMemMan_alloc(0x180, str_PS2Pad, 0x4000000, 0), i, 0);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPS2PadDevice_cPS2PadDevice);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern void* cPS2PadDevice_vtbl[];

/**
 * Constructs a PS2 pad device for a controller port and slot.
 *
 * Installs the vtable cPS2PadDevice_vtbl, stores @p port (mPort) and @p slot (mSlot), zeroes the 36 bytes of mReserved
 * and the sampling state words mPhase..mActuator (+0x48..+0x5C; state 0: the port is opened on the next sample).
 * Called by cPadDevice_construct.
 *
 * @param port  the controller port
 * @param slot  the multitap slot
 * @return @p self
 */
extern "C" void* cPS2PadDevice_cPS2PadDevice(cPS2PadDevice* self, int port, int slot)
{
    self->mVtbl = cPS2PadDevice_vtbl;
    self->mPort = port;
    self->mSlot = slot;
    memset(self->mReserved, 0, 36);
    self->mPressure = 0;
    self->mAnalog = 0;
    self->mModeSet = 0;
    self->mActuator = 0;
    self->mPhase = 0;
    self->mRetry = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPS2PadDevice__cPS2PadDevice);
#ifdef SKIP_ASM
extern "C" int scePadPortClose(int a, int b);
void operator_delete(int* ptr);
extern void* cPS2PadDevice_vtbl[];
extern char cPadDevice_vtbl[];

/**
 * Destroys a PS2 pad device: closes its controller port and frees the object when asked to.
 *
 * Reinstalls cPS2PadDevice_vtbl, closes the port (scePadPortClose with port +0x4 and slot +0x8), installs cPadDevice_vtbl and
 * calls operator delete when bit 0 of @p flags is set. Slot 1 of cPS2PadDevice_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void cPS2PadDevice__cPS2PadDevice(cPS2PadDevice* self, int flags)
{
    *(void***)self = cPS2PadDevice_vtbl;
    scePadPortClose(self->mPort, self->mSlot);
    *(void**)self = cPadDevice_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPS2PadDevice_sampleButtons);
#ifdef SKIP_ASM
extern "C" int func_003FF708(int port, int slot, void* buf);
extern "C" int scePadRead(int port, int slot, unsigned char* data);
extern "C" int scePadGetState(int port, int slot);
extern "C" int scePadGetReqState(int port, int slot);
extern "C" int scePadInfoAct(int port, int slot, int id, int idx);
extern "C" int scePadInfoMode(int port, int slot, int term, int offs);
extern "C" int scePadSetMainMode(int port, int slot, int mode, int lock);
extern "C" int scePadSetActDirect(int port, int slot, unsigned char* data);
extern "C" int scePadSetActAlign(int port, int slot, unsigned char* data);
extern "C" int scePadInfoPressMode(int port, int slot);
extern "C" int scePadEnterPressMode(int port, int slot);
extern unsigned char D_004A3FB8;


static inline float Analog_326EB0(int v)
{
    if (v < 0)
        v = 0;
    return v * 0.003921568859368563f;
}

/**
 * Samples a PS2 controller into button values, stepping the pad's connection state machine once per call.
 *
 * Opens the port when needed and follows the usual libpad setup sequence over the state word +0x48: wait for a stable
 * pad, read its type (digital or analogue), switch an analogue pad to locked analogue mode, enter pressure-sensitive
 * mode (+0x54 set when that works), align the actuators (+0x5C) and finally read (kPhaseRead). A lost or reset pad
 * goes back to kPhaseGetType.
 *
 * Once a read succeeds it fills 24 values in @p out: Select, Start, L3 and R3 as 0 or 1; then the d-pad (right, left,
 * up, down), triangle, circle, cross, square, L1, R1, L2 and R2, as pressure / 255 in pressure mode, else 0 or 1; then
 * the four analogue stick axes (+0x58 set; else zeros) split into eight half-axis values 0..1, with a dead zone of
 * about 48 steps either side of the centre. In kPhaseRead it also sends changed motor levels (+0x30, +0x38) with
 * scePadSetActDirect. Slot 2 of cPS2PadDevice_vtbl; cPadCache_cacheInput calls it for every pad.
 *
 * @param out  receives up to 24 button values
 * @return 24 when the pad was read, otherwise 0
 */
int cPS2PadDevice::sampleButtons(float* out)
{
    unsigned char data[32];
    int ok = 0;
    int ret;
    if (mPhase == cPS2PadDevice::kPhaseOpen) {
        int result = func_003FF708(mPort, mSlot, mPadBuf);
        if (result == kPadCallOk) {
            mPhase = result;
            mModeSet = 0;
            mActSmall = 0;
            mLastSmall = -1;
            mActBig = 0;
            mLastBig = -1;
        }
    }
    int state = scePadGetState(mPort, mSlot);
    if (state == kPadStateDisconnect || state == kPadStateError) {
        mPhase = cPS2PadDevice::kPhaseGetType;
        mModeSet = 0;
    }
    switch (mPhase) {
    case cPS2PadDevice::kPhaseGetType:
        if (state == kPadStateStable || state == kPadStateFindCTP1) {
            mPressure = 0;
            mAnalog = 0;
            mActuator = 0;
            int id = scePadInfoMode(mPort, mSlot, 1, 0);
            if (id != 0) {
                int ext = scePadInfoMode(mPort, mSlot, 2, 0);
                switch (ext) {
                case 0:
                    if (id == kPadIdDigital)
                        mPhase = cPS2PadDevice::kPhaseRead;
                    break;
                case kPadIdDigital:
                    mPhase = cPS2PadDevice::kPhaseSetMode;
                    break;
                case kPadIdAnalog:
                    mAnalog = 1;
                    mPhase = cPS2PadDevice::kPhaseSetMode;
                    break;
                }
            }
        }
        break;
    case cPS2PadDevice::kPhaseSetMode:
        if (mAnalog == 0 || mModeSet == 0) {
            int result = scePadSetMainMode(mPort, mSlot, 1, 3);
            if (result == kPadCallOk) {
                mModeSet = result;
                mPhase = cPS2PadDevice::kPhaseWaitMode;
            }
        } else {
            mPhase = cPS2PadDevice::kPhaseCheckPressure;
        }
        break;
    case cPS2PadDevice::kPhaseWaitMode:
        if (scePadGetState(mPort, mSlot) != kPadStateExecCmd)
            mPhase = cPS2PadDevice::kPhaseGetType;
        break;
    case cPS2PadDevice::kPhaseCheckPressure:
        if (scePadInfoPressMode(mPort, mSlot) == kPadCallOk)
            mPhase = cPS2PadDevice::kPhaseEnterPressure;
        else
            mPhase = cPS2PadDevice::kPhaseCheckActuators;
        break;
    case cPS2PadDevice::kPhaseEnterPressure:
        if (scePadEnterPressMode(mPort, mSlot) == kPadCallOk) {
            mRetry = 0;
            mPhase = cPS2PadDevice::kPhaseWaitPressure;
        }
        break;
    case cPS2PadDevice::kPhaseWaitPressure: {
        int result = scePadGetReqState(mPort, mSlot);
        if (result == kPadReqFailed) {
            mPhase = cPS2PadDevice::kPhaseGetType;
            mModeSet = 0;
        } else if (result == kPadReqComplete) {
            if (++mRetry >= 5) {
                mPressure = 1;
                mPhase = cPS2PadDevice::kPhaseCheckActuators;
            }
        } else {
            mRetry = 0;
        }
        break;
    }
    case cPS2PadDevice::kPhaseCheckActuators:
        if (scePadInfoAct(mPort, mSlot, -1, 0) != 0)
            mPhase = cPS2PadDevice::kPhaseAlignActuators;
        else
            mPhase = cPS2PadDevice::kPhaseRead;
        break;
    case cPS2PadDevice::kPhaseAlignActuators:
        if (scePadSetActAlign(mPort, mSlot, &D_004A3FB8) == kPadCallOk) {
            mRetry = 0;
            mPhase = cPS2PadDevice::kPhaseWaitActuators;
        }
        break;
    case cPS2PadDevice::kPhaseWaitActuators: {
        int result = scePadGetReqState(mPort, mSlot);
        if (result == kPadReqFailed) {
            mPhase = cPS2PadDevice::kPhaseGetType;
            mModeSet = 0;
        } else if (result == kPadReqComplete) {
            if (++mRetry >= 5) {
                mActuator = 1;
                mPhase = cPS2PadDevice::kPhaseRead;
            }
        } else {
            mRetry = 0;
        }
        break;
    }
    case cPS2PadDevice::kPhaseRead:
        if (scePadRead(mPort, mSlot, data) == 0 || data[0] != 0) {
            mPhase = cPS2PadDevice::kPhaseGetType;
            mModeSet = 0;
        } else {
            ok = 1;
        }
        break;
    }
    ret = 0;
    if (ok) {
        ret = 0x18;
        out[0] = !(data[2] & 0x01) ? 1.0f : 0.0f;
        out[1] = !(data[2] & 0x08) ? 1.0f : 0.0f;
        out[2] = !(data[2] & 0x02) ? 1.0f : 0.0f;
        out[3] = !(data[2] & 0x04) ? 1.0f : 0.0f;
        if (mPressure != 0) {
            out[4] = data[8] * 0.003921568859368563f;
            out[5] = data[9] * 0.003921568859368563f;
            out[6] = data[10] * 0.003921568859368563f;
            out[7] = data[11] * 0.003921568859368563f;
            out[8] = data[12] * 0.003921568859368563f;
            out[9] = data[13] * 0.003921568859368563f;
            out[10] = data[14] * 0.003921568859368563f;
            out[11] = data[15] * 0.003921568859368563f;
            out[12] = data[16] * 0.003921568859368563f;
            out[13] = data[17] * 0.003921568859368563f;
            out[14] = data[18] * 0.003921568859368563f;
            out[15] = data[19] * 0.003921568859368563f;
        } else {
            out[4] = !(data[2] & 0x20) ? 1.0f : 0.0f;
            out[5] = !(data[2] & 0x80) ? 1.0f : 0.0f;
            out[6] = !(data[2] & 0x10) ? 1.0f : 0.0f;
            out[7] = !(data[2] & 0x40) ? 1.0f : 0.0f;
            out[8] = !(data[3] & 0x10) ? 1.0f : 0.0f;
            out[9] = !(data[3] & 0x20) ? 1.0f : 0.0f;
            out[10] = !(data[3] & 0x40) ? 1.0f : 0.0f;
            out[11] = !(data[3] & 0x80) ? 1.0f : 0.0f;
            out[12] = !(data[3] & 0x04) ? 1.0f : 0.0f;
            out[13] = !(data[3] & 0x08) ? 1.0f : 0.0f;
            out[14] = !(data[3] & 0x01) ? 1.0f : 0.0f;
            out[15] = !(data[3] & 0x02) ? 1.0f : 0.0f;
        }
        if (mAnalog != 0) {
            out[16] = Analog_326EB0(((79 - data[4]) * 0xFF) / 79);
            out[17] = Analog_326EB0(((data[4] - 176) * 0xFF) / 79);
            out[18] = Analog_326EB0(((79 - data[5]) * 0xFF) / 79);
            out[19] = Analog_326EB0(((data[5] - 176) * 0xFF) / 79);
            out[20] = Analog_326EB0(((79 - data[6]) * 0xFF) / 79);
            out[21] = Analog_326EB0(((data[6] - 176) * 0xFF) / 79);
            out[22] = Analog_326EB0(((79 - data[7]) * 0xFF) / 79);
            out[23] = Analog_326EB0(((data[7] - 176) * 0xFF) / 79);
        } else {
            out[16] = 0.0f;
            out[17] = 0.0f;
            out[18] = 0.0f;
            out[19] = 0.0f;
            out[20] = 0.0f;
            out[21] = 0.0f;
            out[22] = 0.0f;
            out[23] = 0.0f;
        }
    }
    if (mPhase == cPS2PadDevice::kPhaseRead
        && (mActSmall != mLastSmall || mActBig != mLastBig)) {
        mActDirect[0] = mActSmall;
        mActDirect[1] = mActBig;
        mLastSmall = mActSmall;
        mLastBig = mActBig;
        scePadSetActDirect(mPort, mSlot, mActDirect);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPS2PadDevice_numActuators__FPv);
#ifdef SKIP_ASM
/**
 * Returns 2: a PS2 pad has two vibration actuators.
 *
 * Slot 3 of cPS2PadDevice_vtbl, overriding cPadDevice_numActuators (0).
 *
 * @return 2
 */
int cPS2PadDevice::numActuators()
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPS2PadDevice_setActuator);
#ifdef SKIP_ASM
static inline int clamp_327740(int v, int lo, int hi)
{
    if (v >= lo) {
        if (v > hi) {
            v = hi;
        }
    } else {
        v = lo;
    }
    return v;
}

/**
 * Sets one of a PS2 pad's vibration motors.
 *
 * kActuatorSmall (1) is the on/off motor: +0x30 = (@p level != 0). Any other actuator sets the variable motor:
 * +0x38 = level x
 * 205 + 50 clamped to 50..255, or 0 when @p level is 0. cPS2PadDevice_sampleButtons sends changed values to the pad. Slot 4 of
 * cPS2PadDevice_vtbl.
 *
 * @param mode  the actuator index (On Tour act)
 * @param level  the strength, 0..1 (On Tour frequency)
 */
void cPS2PadDevice::setActuator(int mode, float level)
{
    if (mode == kActuatorSmall) {
        mActSmall = level != 0.0f;
        return;
    }
    int* motorOut = (int*)(&mActBig);
    int speed;
    if (level != 0.0f) {
        int rawSpeed = (int)(level * 205.0f + 50.0f);
        speed = clamp_327740(rawSpeed, 50, 255);
    } else {
        speed = 0;
    }
    *motorOut = speed;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadDevice__cPadDevice);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern char cPadDevice_vtbl[];

/**
 * Destroys a pad device: restores cPadDevice's vtable pointer, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cPadDevice__cPadDevice(cPadDevice* self, int flags)
{
    *(void**)self = cPadDevice_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadDevice_numActuators__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default: a pad device has no vibration actuators.
 *
 * @return 0
 */
int cPadDevice::numActuators()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadDevice_setActuator__FPv);
#ifdef SKIP_ASM
/**
 * Base-class default for driving a vibration actuator: does nothing.
 *
 * On Tour's version only fails an assert, which retail compiles out.
 */
void cPadDevice::setActuator()
{
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cAIFwdDiffCache_cAIFwdDiffCache__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an AI forward-difference cache by zeroing its four words.
 *
 * @return @p self
 */
void* cAIFwdDiffCache_cAIFwdDiffCache(void* vself)
{
    cAIFwdDiffCache* self = (cAIFwdDiffCache*)vself;
    int initValue = 0;
    *(int*)self = initValue;
    self->mAgeQueue = (tAIFwdDiff*)initValue;
    self->mPatchHash = (tAIFwdDiff**)initValue;
    self->mHashSize = initValue;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", _$_15cAIFwdDiffCache);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
void operator_delete(int* ptr);

/**
 * Destroys an AI forward-difference cache: frees its node array and hash table and, when asked, the cache itself.
 *
 * Frees the words at +0x0 and +0x8 with cMemMan_free when set, then calls operator delete when bit 0 of g++'s delete
 * flag is set. Called by cBXNash_purgeTheAIFwdDiffCache.
 */
cAIFwdDiffCache::~cAIFwdDiffCache()
{
    if (*(void**)this != 0) {
        cMemMan_free(*(void**)this);
    }
    if (mPatchHash != 0) {
        cMemMan_free(mPatchHash);
    }
}
#endif

