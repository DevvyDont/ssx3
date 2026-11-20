#include "common.h"
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
extern "C" int cInputMapParser_lookupModifierName(cInputMapParser* self, const char* name)
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
extern "C" int cInputMapParser_lookupConfigName(cInputMapParser* self, char* name)
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
extern "C" tIdentifier* cInputMapParser_lookupIdentifierName(cInputMapParser* self, const char* name)
{
    int i;
    for (i = 0; i < self->NumIdentifiers; i++) {
        if (strcasecmp(name, self->Identifiers[i].Name) == 0) {
            return &self->Identifiers[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00321AD0);
#ifdef SKIP_ASM
/**
 * Converts a config index into an input register number: index + 0x40.
 *
 * Registers 0x40..0x5F are the config values that func_00325260 reads through func_00325430. The index is declared as a
 * pointer but is a plain number.
 *
 * @param varIndex  the config index (On Tour config)
 * @return the register number
 */
extern "C" void* func_00321AD0(void* self, void* varIndex)
{
    return (char*)varIndex + 0x40;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00321AD8);
#ifdef SKIP_ASM
/**
 * Converts a button and button modifier into an input register number: 0x60 + button x 6 + modifier.
 *
 * func_00325260 decodes these registers back into (button, modifier) for func_003252F8; the 6 is On Tour's
 * kButtonModifier_Num.
 *
 * @param inputIndex  the button index (On Tour button)
 * @param modifier  the button modifier, 0..5 (On Tour eButtonModifiers)
 * @return the register number
 */
extern "C" int func_00321AD8(void* self, int inputIndex, int modifier)
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
extern "C" int func_004178B0(const char* a, const char* b, ...);
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
static inline int pk_321AF0(sTok_321AF0* s)
{
    return more_321AF0(s) ? *s->pos : -1;
}
// peek into a variable (statement form: the token loops keep the lookahead in `c`)
#define PKC_321AF0(v) { v = -1; if (more_321AF0(s)) v = *s->pos; }
static inline int pk1_321AF0(sTok_321AF0* s)
{
    return s->pos + 1 < s->base + s->len ? s->pos[1] : -1;
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
    while (isWh_321AF0(pk_321AF0(s)) && pk_321AF0(s) != -1 && pk_321AF0(s) != '\n') adv_321AF0(s);
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
extern "C" void cInputMapParser_readToken(cInputMapParser* self)
{
    sTok_321AF0* s = (sTok_321AF0*)self;
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
                while (pk_321AF0(s) != '\n' && pk_321AF0(s) != -1) adv_321AF0(s);
            } else {
                s->line = 0;
                while (isDig_321AF0(pk_321AF0(s))) {
                    s->line = s->line * 10 + (pk_321AF0(s) - '0');
                    adv_321AF0(s);
                }
                blanks_321AF0(s);
                if (pk_321AF0(s) != '"') {
                    cInputMapParser_error(s, str_malformed_pre_processor_line);
                    while (pk_321AF0(s) != '\n' && pk_321AF0(s) != -1) adv_321AF0(s);
                    c = ' ';
                } else {
                    adv_321AF0(s);
                    char* fnameCursor = s->fname;
                    int n = 0;
                    for (;;) {
                        if (pk_321AF0(s) == '\n' || pk_321AF0(s) == -1) {
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
            while (pk_321AF0(s) != '\n' && pk_321AF0(s) != -1) adv_321AF0(s);
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
    if (c == -1) {
        adv_321AF0(s);
        *p = 0;
        s->tok = 0;
        return;
    }
    if (c == '!') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = 19;
        } else {
            *p = 0;
            s->tok = 0x18;
        }
    } else if (c == '+') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '+') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            cInputMapParser_error(s, str_Operator_plus_plus_not_defined);
            s->tok = 1;
        } else {
            *p = 0;
            s->tok = 20;
        }
    } else if (c == '-') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '+') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            cInputMapParser_error(s, str_Operator_plus_plus_not_defined);
            s->tok = 1;
        } else {
            *p = 0;
            s->tok = 21;
        }
    } else if (c == '.') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 4;
    } else if (c == '*') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 22;
    } else if (c == '/') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 23;
    } else if (c == '=') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = 18;
        } else {
            *p = 0;
            s->tok = 3;
        }
    } else if (c == '<') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = 17;
        } else {
            *p = 0;
            s->tok = 0x10;
        }
    } else if (c == '>') {
        adv_321AF0(s);
        PKC_321AF0(c);
        if (c == '=') {
            *p++ = c;
            adv_321AF0(s);
            *p = 0;
            s->tok = 0xF;
        } else {
            *p = 0;
            s->tok = 0xE;
        }
    } else if (c == '&' && pk1_321AF0(s) == c) {
        adv_321AF0(s);
        *p++ = pk_321AF0(s);
        adv_321AF0(s);
        *p = 0;
        s->tok = 13;
    } else if (c == '|' && pk1_321AF0(s) == c) {
        adv_321AF0(s);
        *p++ = pk_321AF0(s);
        adv_321AF0(s);
        *p = 0;
        s->tok = 0xC;
    } else if (c == '(') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 9;
    } else if (c == ')') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 10;
    } else if (c == ',') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 8;
    } else if (c == '?') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 6;
    } else if (c == ';') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 5;
    } else if (c == ':') {
        adv_321AF0(s);
        *p = 0;
        s->tok = 7;
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
            s->tok = 25;
        } else if (strcasecmp(text, str_min) == 0) {
            s->tok = 26;
        } else if (strcasecmp(text, str_pow) == 0) {
            s->tok = 27;
        } else if (strcasecmp(text, str_atan2) == 0) {
            s->tok = 0x1C;
        } else if (strcasecmp(text, str_atan) == 0) {
            s->tok = 29;
        } else if (strcasecmp(text, str_sin) == 0) {
            s->tok = 0x1E;
        } else if (strcasecmp(text, str_cos) == 0) {
            s->tok = 0x1F;
        } else if (strcasecmp(text, str_exp) == 0) {
            s->tok = 0x20;
        } else if (strcasecmp(text, str_log_b) == 0) {
            s->tok = 33;
        } else if (strcasecmp(text, str_floor) == 0) {
            s->tok = 34;
        } else if (strcasecmp(text, str_ceil) == 0) {
            s->tok = 35;
        } else if (strcasecmp(text, str_abs) == 0) {
            s->tok = 36;
        } else if (strcasecmp(text, str_sqrt) == 0) {
            s->tok = 37;
        } else if (strcasecmp(text, str_minradian) == 0) {
            s->tok = 38;
        } else {
            s->tok = 2;
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
                    s->tok = 1;
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
        func_004178B0(text, str_pct_f_b, &s->value);
        s->tok = 11;
        return;
    } else {
        cInputMapParser_error(s, str_Unexpected_character_in_input_pct_c, c);
        *p = 0;
        adv_321AF0(s);
        s->tok = 1;
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
extern "C" void cInputMapParser_parseFile(cInputMapParser* self)
{
    cInputMapParser_readToken(self);
    while (self->CurrentToken != 0) {
        cInputMapParser_parseStatement(self);
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
extern "C" void cInputMapParser_parseStatement(cInputMapParser* self)
{
    if (self->CurrentToken != 2) {
        cInputMapParser_error(self, str_Expected_identifier_in_statement);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    char* name = self->TokenText;
    if (cInputMapParser_lookupButtonName(self, name) >= 0) {
        cInputMapParser_error(self, str_Button_name_used_as_identifier_pct_s, name);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    if (cInputMapParser_lookupConfigName(self, name) >= 0) {
        cInputMapParser_error(self, str_Config_name_used_as_identifier_pct_s, name);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    if (cInputMapParser_lookupIdentifierName(self, name) != 0) {
        cInputMapParser_error(self, str_Duplicate_identifier_name_pct_s, name);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    tIdentifier* identifier = &self->Identifiers[self->NumIdentifiers];
    self->CurrentIdentifier = identifier;
    strcpy(identifier->Name, name);
    cInputMapParser_readToken(self);
    if (self->CurrentToken != 3) {
        cInputMapParser_error(self, str_Expected_in_statement);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    cInputMapParser_readToken(self);
    if ((identifier->Expression = (tParseNode*)cInputMapParser_parseExpression(self)) == 0) {
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    if (self->CurrentToken != 5) {
        cInputMapParser_error(self, str_Expected_to_end_statement);
        cInputMapParser_skipPastSemicolon(self);
        return;
    }
    cInputMapParser_readToken(self);
    self->NumIdentifiers++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseExpression);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_readToken(cInputMapParser* self);
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" void* func_003215F8(void* self);
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
extern "C" void* cInputMapParser_parseExpression(cInputMapParser* self)
{
    sExprNode_00323268* cond = (sExprNode_00323268*)cInputMapParser_parseOr(self);
    if (cond == 0) {
        return 0;
    }
    if (self->CurrentToken != 6) {
        return cond;
    }
    cInputMapParser_readToken(self);
    sExprNode_00323268* thenExpr = (sExprNode_00323268*)cInputMapParser_parseExpression(self);
    if (thenExpr == 0) {
        cInputMapParser_freeParseNode(self, cond);
        return 0;
    }
    if (self->CurrentToken != 7) {
        cInputMapParser_error(self, D_0048DFA8);
        cInputMapParser_freeParseNode(self, cond);
        cInputMapParser_freeParseNode(self, thenExpr);
        return 0;
    }
    cInputMapParser_readToken(self);
    sExprNode_00323268* elseExpr = (sExprNode_00323268*)cInputMapParser_parseExpression(self);
    if (elseExpr == 0) {
        cInputMapParser_freeParseNode(self, cond);
        cInputMapParser_freeParseNode(self, thenExpr);
        return 0;
    }
    if (cond->type == 4) {
        if (cond->value != 0.0f) {
            cInputMapParser_freeParseNode(self, elseExpr);
            return thenExpr;
        }
        cInputMapParser_freeParseNode(self, thenExpr);
        return elseExpr;
    }
    sExprNode_00323268* n = (sExprNode_00323268*)func_003215F8(self);
    n->type = 5;
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
extern "C" void* func_003215F8(void* self);
extern "C" void* cInputMapParser_parseAnd(cInputMapParser* self);

struct sOrNode_003233B8 {
    int type;
    union {
        float value;
        sOrNode_003233B8* a;
    };
    sOrNode_003233B8* b;
};

/**
 * Parses an OR expression: one or more AND expressions joined by the `||` token (token 12).
 *
 * Folds constants (node type 4): a true constant operand turns the result into the constant 1.0 and a false one
 * is dropped. Other operands are joined into type-6 OR nodes taken from the node pool (func_003215F8). If an
 * operand fails to parse, the nodes built so far are freed.
 *
 * @return the expression's parse node, or 0 on a parse error
 */
extern "C" void* cInputMapParser_parseOr(cInputMapParser* self)
{
    sOrNode_003233B8* left = (sOrNode_003233B8*)cInputMapParser_parseAnd(self);
    if (left == 0) {
        return 0;
    }
    int isTrue = 0;
    if (left->type == 4 && left->value != 0.0f) {
        isTrue = 1;
    }
    while (self->CurrentToken == 0xC) {
        cInputMapParser_readToken(self);
        sOrNode_003233B8* right = (sOrNode_003233B8*)cInputMapParser_parseAnd(self);
        if (right == 0) {
            cInputMapParser_freeParseNode(self, left);
            return 0;
        }
        if (isTrue) {
            left->value = 1.0f;
            cInputMapParser_freeParseNode(self, right);
        } else if (right->type == 4) {
            if (right->value != 0.0f) {
                right->value = 1.0f;
                cInputMapParser_freeParseNode(self, left);
                left = right;
                isTrue = 1;
            } else {
                cInputMapParser_freeParseNode(self, right);
            }
        } else if (left->type == 4) {
            cInputMapParser_freeParseNode(self, left);
            left = right;
        } else {
            sOrNode_003233B8* n = (sOrNode_003233B8*)func_003215F8(self);
            n->a = left;
            n->b = right;
            n->type = 6;
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
extern "C" void* func_003215F8(void* self);
extern "C" void* cInputMapParser_parseCompare(cInputMapParser* self);

struct sAndNode_00323540 {
    int type;
    union {
        float value;
        sAndNode_00323540* a;
    };
    sAndNode_00323540* b;
};

/**
 * Parses a chain of "&&" operands, folding constants: a constant 0 makes the whole chain 0.
 *
 * @return the expression tree, or 0 after an error
 */
extern "C" void* cInputMapParser_parseAnd(cInputMapParser* self)
{
    sAndNode_00323540* left = (sAndNode_00323540*)cInputMapParser_parseCompare(self);
    if (left == 0) {
        return 0;
    }
    int isFalse = 0;
    if (left->type == 4 && left->value == 0.0f) {
        isFalse = 1;
    }
    while (self->CurrentToken == 13) {
        cInputMapParser_readToken(self);
        sAndNode_00323540* right = (sAndNode_00323540*)cInputMapParser_parseCompare(self);
        if (right == 0) {
            cInputMapParser_freeParseNode(self, left);
            return 0;
        }
        if (isFalse) {
            cInputMapParser_freeParseNode(self, right);
        } else if (right->type == 4) {
            if (right->value == 0.0f) {
                cInputMapParser_freeParseNode(self, left);
                left = right;
                isFalse = 1;
            } else {
                cInputMapParser_freeParseNode(self, right);
                if (left->type == 4)
                    left->value = 1.0f;
            }
        } else if (left->type == 4) {
            cInputMapParser_freeParseNode(self, left);
            left = right;
        } else {
            sAndNode_00323540* n = (sAndNode_00323540*)func_003215F8(self);
            n->a = left;
            n->b = right;
            n->type = 7;
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

struct sCmpNode_003236D8 {
    int type;
    union {
        float value;
        sCmpNode_003236D8* a;
    };
    sCmpNode_003236D8* b;
};

/**
 * Parses a chain of comparison operators; comparisons of two constants fold to 1.0 or 0.0.
 *
 * @return the expression tree, or 0 after an error
 */
// Parses a chain of comparison operators; comparisons of two constants are folded to 1.0/0.0.
extern "C" void* cInputMapParser_parseCompare(cInputMapParser* self)
{
    sCmpNode_003236D8* left = (sCmpNode_003236D8*)cInputMapParser_parseSum(self);
    if (left == 0) {
        return 0;
    }
    for (;;) {
        int op;
        switch (self->CurrentToken) {
        case 0x10:
            op = 10;
            break;
        case 17:
            op = 11;
            break;
        case 0xF:
            op = 9;
            break;
        case 0xE:
            op = 8;
            break;
        case 18:
            op = 0xC;
            break;
        case 19:
            op = 13;
            break;
        default:
            return left;
        }
        cInputMapParser_readToken(self);
        sCmpNode_003236D8* right = (sCmpNode_003236D8*)cInputMapParser_parseSum(self);
        if (right == 0) {
            cInputMapParser_freeParseNode(self, left);
            return 0;
        }
        if (left->type == 4 && right->type == 4) {
            switch (op) {
            case 10:
                left->value = left->value < right->value ? 1.0f : 0.0f;
                break;
            case 11:
                left->value = left->value <= right->value ? 1.0f : 0.0f;
                break;
            case 9:
                left->value = left->value >= right->value ? 1.0f : 0.0f;
                break;
            case 8:
                left->value = left->value > right->value ? 1.0f : 0.0f;
                break;
            case 0xC:
                left->value = left->value == right->value ? 1.0f : 0.0f;
                break;
            case 13:
                left->value = left->value != right->value ? 1.0f : 0.0f;
                break;
            }
            cInputMapParser_freeParseNode(self, right);
        } else {
            sCmpNode_003236D8* n = (sCmpNode_003236D8*)func_003215F8(self);
            n->a = left;
            n->type = op;
            n->b = right;
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
extern "C" void* func_003215F8(void* self);
extern "C" void* cInputMapParser_parseProduct(cInputMapParser* self);

struct sAddNode_00323900 {
    int type;
    union {
        float value;
        sAddNode_00323900* a;
    };
    sAddNode_00323900* b;
};

/**
 * Parses a chain of additions and subtractions, folding all constant operands into one constant.
 *
 * A subtracted constant is negated and added, so the chain keeps a single constant node.
 *
 * @return the expression tree, or 0 after an error
 */
extern "C" void* cInputMapParser_parseSum(cInputMapParser* self)
{
    sAddNode_00323900* left = (sAddNode_00323900*)cInputMapParser_parseProduct(self);
    if (left == 0) {
        return 0;
    }
    sAddNode_00323900* konst = 0;
    if (left->type == 4) {
        konst = left;
    }
    for (;;) {
        int op;
        switch (self->CurrentToken) {
        case 20:
            op = 0xE;
            break;
        case 21:
            op = 0xF;
            break;
        default:
            return left;
        }
        cInputMapParser_readToken(self);
        sAddNode_00323900* right = (sAddNode_00323900*)cInputMapParser_parseProduct(self);
        if (right == 0) {
            cInputMapParser_freeParseNode(self, left);
            return 0;
        }
        if (right->type == 4) {
            if (konst != 0) {
                switch (op) {
                case 0xE:
                    konst->value = konst->value + right->value;
                    break;
                case 0xF:
                    konst->value = konst->value - right->value;
                    break;
                }
                cInputMapParser_freeParseNode(self, right);
            } else {
                konst = right;
                if (op == 0xF) {
                    right->value = -right->value;
                }
                sAddNode_00323900* n = (sAddNode_00323900*)func_003215F8(self);
                n->a = left;
                n->type = 0xE;
                n->b = right;
                left = n;
            }
        } else {
            sAddNode_00323900* n = (sAddNode_00323900*)func_003215F8(self);
            n->a = left;
            n->type = op;
            n->b = right;
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
extern "C" void* func_003215F8(void* self);
extern "C" void* cInputMapParser_parseTerm(cInputMapParser* self);

struct sMulNode_00323A68 {
    int type;
    union {
        float value;
        sMulNode_00323A68* a;
    };
    sMulNode_00323A68* b;
};

/**
 * Parses a chain of multiplications and divisions, folding all constant operands into one constant.
 *
 * A divisor constant is inverted and multiplied, so the chain keeps a single constant node.
 *
 * @return the expression tree, or 0 after an error
 */
extern "C" void* cInputMapParser_parseProduct(cInputMapParser* self)
{
    sMulNode_00323A68* left = (sMulNode_00323A68*)cInputMapParser_parseTerm(self);
    if (left == 0) {
        return 0;
    }
    sMulNode_00323A68* konst = 0;
    if (left->type == 4) {
        konst = left;
    }
    for (;;) {
        int op;
        switch (self->CurrentToken) {
        case 22:
            op = 0x10;
            break;
        case 23:
            op = 17;
            break;
        default:
            return left;
        }
        cInputMapParser_readToken(self);
        sMulNode_00323A68* right = (sMulNode_00323A68*)cInputMapParser_parseTerm(self);
        if (right == 0) {
            cInputMapParser_freeParseNode(self, left);
            return 0;
        }
        if (right->type == 4) {
            if (konst != 0) {
                switch (op) {
                case 0x10:
                    konst->value = konst->value * right->value;
                    break;
                case 17:
                    konst->value = konst->value / right->value;
                    break;
                }
                cInputMapParser_freeParseNode(self, right);
            } else {
                konst = right;
                if (op == 17) {
                    right->value = 1.0f / right->value;
                }
                sMulNode_00323A68* n = (sMulNode_00323A68*)func_003215F8(self);
                n->a = left;
                n->type = 0x10;
                n->b = right;
                left = n;
            }
        } else {
            sMulNode_00323A68* n = (sMulNode_00323A68*)func_003215F8(self);
            n->a = left;
            n->type = op;
            n->b = right;
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
extern "C" void* cInputMapParser_parseTerm(cInputMapParser* self)
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
    switch (self->CurrentToken) {
    case 2:
        name = self->TokenText;
        input = cInputMapParser_lookupButtonName(self, name);
        if (input >= 0) {
            mod = 5;
            cInputMapParser_readToken(self);
            if (self->CurrentToken == 4) {
                cInputMapParser_readToken(self);
                if (self->CurrentToken != 2) {
                    cInputMapParser_error(self, str_Expected_modifier_after_button);
                    return 0;
                }
                mod = cInputMapParser_lookupModifierName(self, name);
                if (mod == 6) {
                    cInputMapParser_error(self, str_Illegal_modifier_on_button_pct_s, name);
                    return 0;
                }
                cInputMapParser_readToken(self);
            }
            node = (sTermNode_3BD8*)func_003215F8(self);
            node->type = 2;
            node->index = input;
            node->b = mod;
            return node;
        }
        cfg = cInputMapParser_lookupConfigName(self, name);
        if (cfg >= 0) {
            cInputMapParser_readToken(self);
            node = (sTermNode_3BD8*)func_003215F8(self);
            node->type = 3;
            node->index = cfg;
            return node;
        }
        identifier = cInputMapParser_lookupIdentifierName(self, name);
        if (identifier != 0) {
            int mod2;
            cInputMapParser_readToken(self);
            if ((tIdentifier*)identifier == self->CurrentIdentifier) {
                cInputMapParser_error(self, str_Recursive_identifier_definition);
                return 0;
            }
            mod2 = 5;
            if (self->CurrentToken == 4) {
                cInputMapParser_readToken(self);
                if (self->CurrentToken != 2) {
                    cInputMapParser_error(self, str_Expected_modifier_after_button);
                    return 0;
                }
                mod2 = cInputMapParser_lookupModifierName(self, name);
                if (mod2 == 6) {
                    cInputMapParser_error(self, str_Illegal_modifier_on_reference_pct_s, name);
                    return 0;
                }
                cInputMapParser_readToken(self);
            }
            type = ((sTermNode_3BD8*)identifier->Expression)->type;
            if (type == 4) {
                node = (sTermNode_3BD8*)func_003215F8(self);
                node->type = type;
                node->value = ((sTermNode_3BD8*)identifier->Expression)->value;
                return node;
            }
            node = (sTermNode_3BD8*)func_003215F8(self);
            node->type = 1;
            node->a = identifier;
            node->b = mod2;
            return node;
        }
        cInputMapParser_error(self, str_Unexpected_identifier_pct_s, name);
        return 0;
    case 11:
        node = (sTermNode_3BD8*)func_003215F8(self);
        node->type = 4;
        node->value = self->TokenValue;
        cInputMapParser_readToken(self);
        return node;
    case 20:
        cInputMapParser_readToken(self);
        goto again;
    case 21:
        cInputMapParser_readToken(self);
        operand = (sTermNode_3BD8*)cInputMapParser_parseTerm(self);
        if (operand == 0)
            return 0;
        if (operand->type == 4) {
            operand->value = -operand->value;
            return operand;
        }
        node = (sTermNode_3BD8*)func_003215F8(self);
        node->type = 18;
        node->a = operand;
        return node;
    case 24:
        cInputMapParser_readToken(self);
        operand = (sTermNode_3BD8*)cInputMapParser_parseTerm(self);
        if (operand == 0)
            return 0;
        if (operand->type == 4) {
            operand->value = operand->value == 0.0f ? 1.0f : 0.0f;
            return operand;
        }
        node = (sTermNode_3BD8*)func_003215F8(self);
        node->type = 19;
        node->a = operand;
        return node;
    case 9:
        cInputMapParser_readToken(self);
        node = (sTermNode_3BD8*)cInputMapParser_parseExpression(self);
        if (self->CurrentToken != 10) {
            cInputMapParser_error(self, str_Expected_in_expression);
            cInputMapParser_freeParseNode(self, node);
            return 0;
        }
        cInputMapParser_readToken(self);
        return node;
    case 25:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseBinaryFunc(self, 20);
    case 26:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseBinaryFunc(self, 21);
    case 27:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseBinaryFunc(self, 22);
    case 28:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseBinaryFunc(self, 23);
    case 29:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 0x18);
    case 30:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 25);
    case 31:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 26);
    case 32:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 27);
    case 33:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 0x1C);
    case 34:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 29);
    case 35:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 0x1E);
    case 36:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 0x1F);
    case 37:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 0x20);
    case 38:
        cInputMapParser_readToken(self);
        return cInputMapParser_parseUnaryFunc(self, 33);
    default:
        cInputMapParser_error(self, str_Expected_term_in_expression);
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
extern "C" void* func_003215F8(void* self);
extern "C" void cInputMapParser_freeParseNode(void* self, void* node);
extern "C" float func_0031C228(float x);
extern "C" float func_0040DA10(float x);
extern "C" float func_0040D758(float x);
extern char str_Expected_in_expression_b[];
extern char str_Expected_in_expression_c[];
extern char str_Expected_in_expression[];

struct sBinNode_4138 {
    int type;
    union {
        float value;
        sBinNode_4138* a;
    };
    sBinNode_4138* b;
};

static inline float pow_4138(float x, float y)
{
    return func_0040D758(func_0040DA10(x) * y);
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
extern "C" void* cInputMapParser_parseBinaryFunc(cInputMapParser* self, int op)
{
    if (self->CurrentToken != 9) {
        cInputMapParser_error(self, str_Expected_in_expression_b);
        return 0;
    }
    cInputMapParser_readToken(self);
    sBinNode_4138* firstArg = (sBinNode_4138*)cInputMapParser_parseExpression(self);
    if (firstArg == 0) {
        return 0;
    }
    if (self->CurrentToken != 8) {
        cInputMapParser_error(self, str_Expected_in_expression_c);
        cInputMapParser_freeParseNode(self, firstArg);
        return 0;
    }
    cInputMapParser_readToken(self);
    sBinNode_4138* secondArg = (sBinNode_4138*)cInputMapParser_parseExpression(self);
    if (secondArg == 0) {
        return 0;
    }
    if (self->CurrentToken != 10) {
        cInputMapParser_error(self, str_Expected_in_expression);
        cInputMapParser_freeParseNode(self, firstArg);
        cInputMapParser_freeParseNode(self, secondArg);
        return 0;
    }
    cInputMapParser_readToken(self);
    if (firstArg->type == 4 && secondArg->type == 4) {
        switch (op) {
        case 21:
            firstArg->value = secondArg->value <? firstArg->value;
            break;
        case 20:
            firstArg->value = secondArg->value >? firstArg->value;
            break;
        case 22:
            firstArg->value = pow_4138(firstArg->value, secondArg->value);
            break;
        case 23:
            firstArg->value = atan2_4138(firstArg->value, secondArg->value);
            break;
        }
        cInputMapParser_freeParseNode(self, secondArg);
        return firstArg;
    }
    sBinNode_4138* n = (sBinNode_4138*)func_003215F8(self);
    n->type = op;
    n->a = firstArg;
    n->b = secondArg;
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputMapParser_parseUnaryFunc);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" float func_0031BF60(float x);
extern "C" float func_0031C040(float x);

struct sUnNode_43A0 {
    int type;
    union {
        float value;
        sUnNode_43A0* a;
    };
};

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
extern "C" void* cInputMapParser_parseUnaryFunc(cInputMapParser* self, int op)
{
    if (self->CurrentToken != 9) {
        cInputMapParser_error(self, str_Expected_in_expression_b);
        return 0;
    }
    cInputMapParser_readToken(self);
    sUnNode_43A0* operand = (sUnNode_43A0*)cInputMapParser_parseExpression(self);
    if (operand == 0) {
        return 0;
    }
    if (self->CurrentToken != 10) {
        cInputMapParser_error(self, str_Expected_in_expression);
        cInputMapParser_freeParseNode(self, operand);
        return 0;
    }
    cInputMapParser_readToken(self);
    if (operand->type == 4) {
        switch (op) {
        case 0x18:
            operand->value = func_0031C228(operand->value);
            break;
        case 25:
            operand->value = func_0031BF60(operand->value);
            break;
        case 26:
            operand->value = func_0031C040(operand->value);
            break;
        case 27:
            operand->value = func_0040D758(operand->value);
            break;
        case 0x1C:
            operand->value = func_0040DA10(operand->value);
            break;
        case 29:
            operand->value = floor_43A0(operand->value);
            break;
        case 0x1E:
            operand->value = ceil_43A0(operand->value);
            break;
        case 0x1F:
            operand->value = abs_43A0(operand->value);
            break;
        case 0x20:
            operand->value = sqrt_43A0(operand->value);
            break;
        case 33: {
            float x = operand->value;
            operand->value = x - floor_43A0(x * 0.159154937f + 0.5f) * 6.28318548f;
            break;
        }
        }
        return operand;
    }
    sUnNode_43A0* n = (sUnNode_43A0*)func_003215F8(self);
    n->type = op;
    n->a = operand;
    return n;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileStatement);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sEntry_324590 {
    char pad[0x40];
    void* code;     // 0x40
};
extern "C" sEntry_324590* cInputMapParser_lookupIdentifierName_K(void* self, const char* name) __asm__("cInputMapParser_lookupIdentifierName");
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, void* node, int* src, void* code, int cap, int a5);
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
extern "C" int cInputMapParser_compileStatement(cInputMapParser* self, const char* name, int* out, int max, int defaultModifier)
{
    sEntry_324590* ident = cInputMapParser_lookupIdentifierName_K(self, name);
    if (ident == 0) {
        cInputMapParser_error(self, D_0048E208, name);
        return -1;
    }
    int sep = ' ';
    int state = 0;
    int n = cInputMapParser_compileParseNode(self, ident->code, &state, out, max, defaultModifier);
    max -= n;
    out += n;
    if (max <= 0) {
        cInputMapParser_error(self, str_opcode_buffer_overflow);
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
extern "C" int func_00321AD0_i(void* self, void* var) __asm__("func_00321AD0");
extern "C" int func_00321AD8(void* self, int a1, int a2);
extern "C" int cInputMapParser_compileBinaryNode_K(cInputMapParser* self, void* node, int* dst, void* code, int cap, int op, int a6) __asm__("cInputMapParser_compileBinaryNode");
extern "C" int cInputMapParser_compileUnaryNode_K(cInputMapParser* self, void* node, int* dst, void* code, int cap, int op, int a6) __asm__("cInputMapParser_compileUnaryNode");
extern char str_opcode_buffer_overflow[];
extern char str_failed_to_allocate_temporary_register[];
extern char str_branch_offset_overflow[];

struct sNode_324678 {
    int type;
    union {
        float value;
        int index;
        void* a;
    };
    union {
        int b;
        sNode_324678* then_;
    };
    sNode_324678* else_;
};
struct sVar_324678 {
    char name[0x40];
    sNode_324678* expr;         // 0x40
};
union sInsn_324678 {
    struct {
        unsigned int op : 6;
        unsigned int dst : 6;
        unsigned int src : 10;
        unsigned int arg : 10;
    } r;
    struct {
        unsigned int op : 6;
        unsigned int reg : 10;
        unsigned int offset : 16;
    } j;
    float f;
};
/**
 * Compiles an expression tree into op-codes whose result lands in a register, allocating one when needed.
 *
 * Constants load a value (two words); buttons and configs are read straight from their register when no
 * target
 * is given; references compile their identifier's expression; selections compile to a conditional jump, the
 * then
 * branch, a jump and the else branch; operators go to compileBinaryNode() or compileUnaryNode().
 *
 * @param vnode  the tree (On Tour tParseNode* node)
 * @param dst  the result register in and out; -1 lets the node pick one (On Tour int& reg)
 * @param vcode  where to write the op-codes (On Tour codes)
 * @param cap  room left in op-codes (On Tour clen)
 * @param defaultModifier  button modifier for buttons and references without one
 * @return the number of op-codes written, or -1 on error
 */
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, void* vnode, int* dst, void* vcode, int cap, int defaultModifier)
{
    sNode_324678* node = (sNode_324678*)vnode;
    sInsn_324678* code = (sInsn_324678*)vcode;
    switch (node->type) {
    case 4:
        if (*dst < 0) {
            if (self->NumFreeRegisters > 0) {
                *dst = self->FreeRegisters[--self->NumFreeRegisters];
            }
            if (*dst < 0) {
                cInputMapParser_error(self, str_failed_to_allocate_temporary_register);
                return -1;
            }
        }
        if (cap < 2) {
            cInputMapParser_error(self, str_opcode_buffer_overflow);
            return -1;
        }
        code->r.op = 0;
        code->r.dst = *dst;
        code->r.src = 0;
        code->r.arg = 0;
        code[1].f = node->value;
        return 2;
    case 1:
        return cInputMapParser_compileParseNode(self, ((sVar_324678*)node->a)->expr, dst, code, cap, node->b != 5 ? node->b : defaultModifier);
    case 2: {
        int idx = func_00321AD8(self, node->index, node->b == 5 ? defaultModifier : node->b);
        if (*dst == -1) {
            *dst = idx;
            return 0;
        }
        if (cap <= 0) {
            cInputMapParser_error(self, str_opcode_buffer_overflow);
            return -1;
        }
        code->r.op = 1;
        code->r.dst = *dst;
        code->r.src = idx;
        code->r.arg = 0;
        return 1;
    }
    case 3:
        if (*dst == -1) {
            *dst = func_00321AD0_i(self, node->a);
            return 0;
        }
        if (cap <= 0) {
            cInputMapParser_error(self, str_opcode_buffer_overflow);
            return -1;
        }
        code->r.op = 1;
        code->r.dst = *dst;
        code->r.src = func_00321AD0_i(self, node->a);
        code->r.arg = 0;
        return 1;
    case 5: {
        int cond = -1;
        int condLen = cInputMapParser_compileParseNode(self, node->a, &cond, code, cap, defaultModifier);
        if (condLen < 0) {
            return -1;
        }
        code += condLen;
        cap -= condLen;
        if (cap <= 0) {
            cInputMapParser_error(self, str_opcode_buffer_overflow);
            return -1;
        }
        unsigned int reg = cond;
        code->j.op = 2;
        code->j.reg = reg;
        if (reg < 0x40) {
            self->FreeRegisters[self->NumFreeRegisters++] = reg;
        }
        cap--;
        int thenLen = cInputMapParser_compileParseNode(self, node->then_, dst, code + 1, cap, defaultModifier);
        if (thenLen < 0) {
            return -1;
        }
        if (thenLen + 1 > 0xFFFF) {
            cInputMapParser_error(self, str_branch_offset_overflow);
            return -1;
        }
        code->j.offset = thenLen + 1;
        code += thenLen + 1;
        cap -= thenLen;
        if (cap <= 0) {
            cInputMapParser_error(self, str_opcode_buffer_overflow);
            return -1;
        }
        code->j.op = 3;
        code->j.reg = 0;
        int elseLen = cInputMapParser_compileParseNode(self, node->else_, dst, code + 1, cap - 1, defaultModifier);
        if (elseLen < 0) {
            return -1;
        }
        if (elseLen > 0xFFFF) {
            cInputMapParser_error(self, str_branch_offset_overflow);
            return -1;
        }
        code->j.offset = elseLen;
        return condLen + thenLen + elseLen + 2;
    }
    case 6: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 4, defaultModifier);
    case 7: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 5, defaultModifier);
    case 8: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 6, defaultModifier);
    case 9: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 7, defaultModifier);
    case 10: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 8, defaultModifier);
    case 11: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 9, defaultModifier);
    case 12: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 10, defaultModifier);
    case 13: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 11, defaultModifier);
    case 14: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 12, defaultModifier);
    case 15: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 13, defaultModifier);
    case 16: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 14, defaultModifier);
    case 17: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 15, defaultModifier);
    case 20: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 16, defaultModifier);
    case 21: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 17, defaultModifier);
    case 22: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 18, defaultModifier);
    case 23: return cInputMapParser_compileBinaryNode_K(self, node, dst, code, cap, 19, defaultModifier);
    case 24: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 20, defaultModifier);
    case 25: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 21, defaultModifier);
    case 26: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 22, defaultModifier);
    case 27: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 23, defaultModifier);
    case 28: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 24, defaultModifier);
    case 29: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 25, defaultModifier);
    case 30: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 26, defaultModifier);
    case 31: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 27, defaultModifier);
    case 32: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 28, defaultModifier);
    case 33: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 29, defaultModifier);
    case 18: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 30, defaultModifier);
    case 19: return cInputMapParser_compileUnaryNode_K(self, node, dst, code, cap, 31, defaultModifier);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileBinaryNode);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, void* node, int* src, void* code, int cap, int a5);
extern char str_failed_to_allocate_temporary_register[];
extern char str_opcode_buffer_overflow[];

struct sInsn_00324EC8 {
    unsigned int op : 6;
    unsigned int dst : 6;
    unsigned int src : 10;
    unsigned int arg : 10;
};
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
extern "C" int cInputMapParser_compileBinaryNode(cInputMapParser* self, void* node, int* dst, sInsn_00324EC8* code, int cap, int op, int defaultModifier)
{
    int src = -1;
    int srcLen = cInputMapParser_compileParseNode(self, *(void**)((char*)node + 4), &src, code, cap, defaultModifier);
    if (srcLen < 0) {
        return -1;
    }
    code += srcLen;
    cap -= srcLen;
    int argReg = -1;
    int argLen = cInputMapParser_compileParseNode(self, *(void**)((char*)node + 8), &argReg, code, cap, defaultModifier);
    if (argLen < 0) {
        return -1;
    }
    code += argLen;
    cap -= argLen;
    if ((unsigned int)argReg < 0x40) {
        self->FreeRegisters[self->NumFreeRegisters++] = argReg;
    }
    if ((unsigned int)src < 0x40) {
        self->FreeRegisters[self->NumFreeRegisters++] = src;
    }
    if (*dst < 0) {
        if (self->NumFreeRegisters > 0) {
            *dst = self->FreeRegisters[--self->NumFreeRegisters];
        }
        if (*dst < 0) {
            cInputMapParser_error(self, str_failed_to_allocate_temporary_register);
            return -1;
        }
    }
    if (cap <= 0) {
        cInputMapParser_error(self, str_opcode_buffer_overflow);
        return -1;
    }
    code->op = op;
    code->dst = *dst;
    code->src = src;
    code->arg = argReg;
    return srcLen + argLen + 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputMapParser_compileUnaryNode);
#ifdef SKIP_ASM
extern "C" void cInputMapParser_error(void* self, const char* fmt, ...);
extern "C" int cInputMapParser_compileParseNode(cInputMapParser* self, void* node, int* src, void* code, int cap, int a5);
extern char str_failed_to_allocate_temporary_register[];
extern char str_opcode_buffer_overflow[];

struct sInsn_003250D8 {
    unsigned int op : 6;
    unsigned int dst : 6;
    unsigned int src : 10;
    unsigned int arg : 10;
};
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
extern "C" int cInputMapParser_compileUnaryNode(cInputMapParser* self, void* node, int* dst, sInsn_003250D8* code, int cap, int op, int defaultModifier)
{
    void* operand = *(void**)((char*)node + 4);
    int src = -1;
    int n = cInputMapParser_compileParseNode(self, operand, &src, code, cap, defaultModifier);
    if (n < 0) {
        return -1;
    }
    code += n;
    cap -= n;
    if ((unsigned int)src < 0x40) {
        self->FreeRegisters[self->NumFreeRegisters++] = src;
    }
    if (*dst < 0) {
        if (self->NumFreeRegisters > 0) {
            *dst = self->FreeRegisters[--self->NumFreeRegisters];
        }
        if (*dst < 0) {
            cInputMapParser_error(self, str_failed_to_allocate_temporary_register);
            return -1;
        }
    }
    if (cap <= 0) {
        cInputMapParser_error(self, str_opcode_buffer_overflow);
        return -1;
    }
    code->op = op;
    code->dst = *dst;
    code->src = src;
    code->arg = 0;
    return n + 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00325250__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an input map evaluator: sets its default button modifier (+0x0) to 0.
 *
 * cInputMap_getValue, cInputMap_getState and their modifier overloads build a 0x190-byte evaluator on the stack with it
 * before running func_00325450.
 *
 * @return @p self
 */
void* func_00325250(void* self)
{
    *(int*)self = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00325260);
#ifdef SKIP_ASM
struct sInputParse_325260 {
    int pad[2];
    float v[0x40];
};
extern "C" float func_003252F8(void* self, int idx, int sel);
extern "C" float func_00325430(void* self, int a1);

/**
 * Reads an input register: a temporary, a config value or a button.
 *
 * Registers 0..0x3F are the temporaries (+0x8), 0x40..0x5F the config values (func_00325430) and 0x60.. buttons: id -
 * 0x60 is split into button = n / 6 and modifier = n % 6 for func_003252F8. Numbers from 0x60 + 924 up read as 0.
 *
 * @param id  the register number
 * @return the register's value
 */
extern "C" float func_00325260(sInputParse_325260* self, unsigned int id)
{
    if (id < 0x40) {
        return self->v[id];
    }
    if (id - 0x40 < 0x20) {
        return func_00325430(self, id - 0x40);
    }
    id -= 0x60;
    if (id < 924) {
        return func_003252F8(self, (int)id / 6, (int)id % 6);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_003252E8);
#ifdef SKIP_ASM
struct sInputParse_003252E8 {
    int pad[2];
    int a[1];
};

/**
 * Returns the address of one of an input map evaluator's temporary registers.
 *
 * Temporaries are 64 floats from +0x8; no range check.
 *
 * @param i  the temporary register index
 * @return the register's address
 */
extern "C" void* func_003252E8(void* self, int i)
{
    return &((sInputParse_003252E8*)self)->a[i];
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_003252F8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sInputEntry3252F8 {
    float value;        // 0x0
    unsigned int a;     // 0x4
    unsigned int b;     // 0x8
    unsigned int c;     // 0xC
    unsigned int d;     // 0x10
    int pad[2];
};

struct sInputList3252F8 {
    int count;
    sInputEntry3252F8 entries[1];
};

struct sInputParse3252F8 {
    int defKind;
    sInputList3252F8* list;
};

static inline unsigned int GetA3252F8(sInputList3252F8* l, int idx)
{
    if (idx < l->count) {
        return l->entries[idx].a;
    }
    return 0;
}

static inline unsigned int GetB3252F8(sInputList3252F8* l, int idx)
{
    if (idx < l->count) {
        return l->entries[idx].b;
    }
    return 0;
}

static inline unsigned int GetC3252F8(sInputList3252F8* l, int idx)
{
    if (idx < l->count) {
        return l->entries[idx].c;
    }
    return 0;
}

static inline unsigned int GetD3252F8(sInputList3252F8* l, int idx)
{
    if (idx < l->count) {
        return l->entries[idx].d;
    }
    return 0;
}

// PORT: the unit declares func_003252F8 as (void*, int, int); the definition takes an unsigned kind.
float func_003252F8_impl(sInputParse3252F8* self, int idx, unsigned int kind) __asm__("func_003252F8");

/**
 * Reads one button of the pad being evaluated, as its analogue value or as one of its state flags.
 *
 * @p kind (On Tour eButtonModifiers) picks the field of button @p idx of the pad (+0x4): 0 the value, 1 held (+0xC), 2
 * pressed (+0x4), 3 released (+0x8), 4 repeated (+0x10), the flags converted to 0.0 / 1.0. 5 (default) uses the
 * evaluator's default modifier (+0x0) instead; if that is itself 5 it recurses forever. A button past the pad's count,
 * or an unknown kind, reads 0.
 *
 * @param idx  the button index
 * @param kind  the button modifier, 0..5
 * @return the value
 */
float func_003252F8_impl(sInputParse3252F8* self, int idx, unsigned int kind)
{
    switch (kind) {
    case 0:
        if (idx < self->list->count) {
            return self->list->entries[idx].value;
        }
        break;
    case 1:
        return (float)GetC3252F8(self->list, idx);
    case 2:
        return (float)GetA3252F8(self->list, idx);
    case 3:
        return (float)GetB3252F8(self->list, idx);
    case 4:
        return (float)GetD3252F8(self->list, idx);
    case 5:
        return func_003252F8_impl(self, idx, self->defKind);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00325430);
#ifdef SKIP_ASM
/**
 * Returns one of an input map evaluator's config values; config 0 always reads 0.
 *
 * The values are floats from +0x108. func_00321010 and func_00321188 fill configs 1..8.
 *
 * @param varIndex  the config index
 * @return the value, or 0.0 for config 0
 */
extern "C" float func_00325430(void* self, int varIndex)
{
    if (varIndex == 0) {
        return 0.0f;
    }
    char* varBase = (char*)self + varIndex * 4;
    return *(float*)(varBase + 0x108);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00325450);
#ifdef SKIP_ASM
struct sInputVM_325450 {
    int defKind;        // 0x0
    void* list;         // 0x4
    float regs[0x40];   // 0x8
};

// PORT: func_00325260 is declared with its own self struct earlier in the unit.
extern "C" float func_00325260_vm(sInputVM_325450* self, unsigned int id) __asm__("func_00325260");

/**
 * Runs a compiled input map statement against a controller pad and returns its result.
 *
 * Stores @p list as the evaluator's pad (+0x4) and steps through the op-codes from @p code until op 32 (End). The op
 * numbers are On Tour's eInputOpCodes: 0 Load (the next word is a float constant), 1 Move, 2 BranchFalse and 3 Branch
 * (relative jumps), 4 Or, 5 And, 6..11 the comparisons (>, >=, <, <=, ==, !=) giving 1.0 or 0.0, 12..15 + - x /, 16
 * Max, 17 Min, 18 Pow (computed as exp(log(x) x y), so a negative base gives NaN), 19 Atan2, 20 Atan, 21 Sin, 22 Cos,
 * 23 Exp, 24 Log, 25 Floor, 26 Ceil, 27 Abs, 28 Sqrt, 29 MinRadian (wraps an angle into [-pi, pi)), 30 Neg and 31 Not.
 * Operands are read with func_00325260 and results written through func_003252E8. Unknown op-codes are skipped. The
 * encoder is cInputMapParser_compileParseNode.
 *
 * @param list  the controller pad to read (On Tour cInputPad*)
 * @param code  the statement's op-codes (On Tour program)
 * @return the value of register 0 when End is reached
 */
// Runs one compiled input expression (see cInputMapParser_compileParseNode for the encoder); returns register 0.
extern "C" float func_00325450(sInputVM_325450* self, void* list, sInsn_324678* code)
{
    int pc = 0;
    int done = 0;
    self->list = list;
    do {
        sInsn_324678* ins = &code[pc];
        pc++;
        switch (ins->r.op) {
        case 0: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = code[pc++].f;
            break;
        }
        case 1: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_00325260_vm(self, ins->r.src);
            break;
        }
        case 2:
            if (func_00325260_vm(self, ins->j.reg) != 0.0f) {
                break;
            }
        case 3:
            pc += ins->j.offset;
            break;
        case 4: {
            unsigned int boolResult = 0;
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            if (func_00325260_vm(self, ins->r.src) != 0.0f || func_00325260_vm(self, ins->r.arg) != 0.0f) {
                boolResult = 1;
            }
            *destSlot = boolResult;
            break;
        }
        case 5: {
            unsigned int boolResult = 0;
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            if (func_00325260_vm(self, ins->r.src) != 0.0f && func_00325260_vm(self, ins->r.arg) != 0.0f) {
                boolResult = 1;
            }
            *destSlot = boolResult;
            break;
        }
        case 6: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs > func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 7: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs >= func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 8: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs < func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 9: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs <= func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 10: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs == func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 11: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = (lhs != func_00325260_vm(self, ins->r.arg)) ? 1.0f : 0.0f;
            break;
        }
        case 12: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = lhs + func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 13: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = lhs - func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 14: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = lhs * func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 15: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = lhs / func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 16: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = (func_00325260_vm(self, ins->r.src) > func_00325260_vm(self, ins->r.arg))
                ? func_00325260_vm(self, ins->r.src) : func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 17: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = (func_00325260_vm(self, ins->r.src) < func_00325260_vm(self, ins->r.arg))
                ? func_00325260_vm(self, ins->r.src) : func_00325260_vm(self, ins->r.arg);
            break;
        }
        case 18: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = pow_4138(lhs, func_00325260_vm(self, ins->r.arg));
            break;
        }
        case 19: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float lhs = func_00325260_vm(self, ins->r.src);
            *destSlot = atan2_4138(lhs, func_00325260_vm(self, ins->r.arg));
            break;
        }
        case 20: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_0031C228(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 21: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_0031BF60(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 22: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_0031C040(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 23: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_0040D758(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 24: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = func_0040DA10(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 25: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = floor_43A0(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 26: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = ceil_43A0(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 27: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = abs_43A0(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 28: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = sqrt_43A0(func_00325260_vm(self, ins->r.src));
            break;
        }
        case 29: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            float x = func_00325260_vm(self, ins->r.src);
            *destSlot = x - floor_43A0(x * 0.159154937f + 0.5f) * 6.28318548f;
            break;
        }
        case 30: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = -func_00325260_vm(self, ins->r.src);
            break;
        }
        case 31: {
            float* destSlot = (float*)func_003252E8(self, ins->r.dst);
            *destSlot = (func_00325260_vm(self, ins->r.src) == 0.0f) ? 1.0f : 0.0f;
            break;
        }
        case 32:
            done = 1;
            break;
        }
    } while (!done);
    return func_00325260_vm(self, 0);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_loadFileSize);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern "C" char* func_0041ACC0(const char* s, int c);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
// PORT: cInputPreProcessor_readFile is declared (void*, int, int) later in the unit; this caller passes two strings.
void cInputPreProcessor_readFile_s(void* self, char* dir, char* file) __asm__("cInputPreProcessor_readFile");
extern char str_PreProcBuf[];

struct sParseBuf_5F48 {
    int size;
    int cap;
    char* data;
};

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
extern "C" void* cInputPreProcessor_loadFileSize(sParseBuf_5F48* self, char* path, int* outSize)
{
    char dir[128];
    char file[32];
    self->size = 0;
    self->cap = 0x800;
    self->data = (char*)operator_new_tag(0x800, str_PreProcBuf, 0, 0);
    char* lastSlash = func_0041ACC0(path, '/');
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
    cInputPreProcessor_readFile_s(self, dir, file);
    void* out = operator_new_tag(self->size, path, 0x100, 0);
    func_0041605C(out, self->data, self->size);
    if (self->data != 0) {
        cMemMan_free(self->data);
    }
    *outSize = self->size;
    return out;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readFile);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
void cMemMan_free(void* ptr);
struct sParseStream;
extern "C" void cInputPreProcessor_readChar(void* self, sParseStream* s);
extern "C" void cInputPreProcessor_writeTagLine(void* self, int a1, int a2);
extern "C" int cInputPreProcessor_copyLine(void* self, sParseStream* s, int a2, int a3);
extern "C" unsigned char* func_003E1A10(const char* name, int* size, int flags);
extern char D_004A3FA0[];

struct sParseStream_326078 {
    unsigned char* base;
    int size;
    unsigned char* cur;
    int line;
};

/**
 * Reads one file into the output, line by line, starting with a "# 1 "file"" tag line.
 *
 * @param path  directory of the file, also used for its includes
 * @param fn  file name within @p path
 */
extern "C" void cInputPreProcessor_readFile(void* self, int path, int fn)
{
    char name[160];
    sParseStream_326078 st;
    sprintf(name, D_004A3FA0, path, fn);
    unsigned char* fileData = func_003E1A10(name, &st.size, 0);
    st.cur = st.base = fileData;
    st.line = 1;
    cInputPreProcessor_readChar(self, (sParseStream*)&st);
    // PORT: the unit declares cInputPreProcessor_writeTagLine's third parameter as int; it receives a char*.
    cInputPreProcessor_writeTagLine(self, st.line, (int)name);
    while (cInputPreProcessor_copyLine(self, (sParseStream*)&st, path, fn)) {
    }
    if (st.base) {
        cMemMan_free(st.base);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_copyLine);
#ifdef SKIP_ASM
struct sParseStream;
extern "C" void cInputPreProcessor_writeChar(void* self, char c);
extern "C" void cInputPreProcessor_readChar(void* self, sParseStream* s);
extern "C" void cInputPreProcessor_readCmdLine(void* self, sParseStream* s, int a2, int a3);
extern "C" void cInputPreProcessor_skipCComment(void* self, sParseStream* s, int a2, int a3);
extern "C" void cInputPreProcessor_skipCppComment(void* self, sParseStream* s, int a2, int a3);

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
 * @return nonzero while the source has more characters
 */
extern "C" int cInputPreProcessor_copyLine(void* self, sParseStream* isrc, int path, int fn)
{
    int ch;
    for (;;) {
        ch = *(int*)((char*)self + 0xC);
        if (!isWS_326150(ch))
            break;
        cInputPreProcessor_writeChar(self, (char)*(int*)((char*)self + 0xC));
        cInputPreProcessor_readChar(self, isrc);
        if (*(int*)((char*)self + 0xC) < 0)
            return 0;
    }
    if (ch == '#') {
        cInputPreProcessor_readChar(self, isrc);
        cInputPreProcessor_readCmdLine(self, isrc, path, fn);
        return *(int*)((char*)self + 0xC) >= 0;
    }
    if (ch == '/') {
        cInputPreProcessor_readChar(self, isrc);
        int nextChar = *(int*)((char*)self + 0xC);
        if (nextChar == '/') {
            cInputPreProcessor_skipCppComment(self, isrc, path, fn);
            return *(int*)((char*)self + 0xC) >= 0;
        }
        if (nextChar == '*') {
            cInputPreProcessor_readChar(self, isrc);
            cInputPreProcessor_skipCComment(self, isrc, path, fn);
            return *(int*)((char*)self + 0xC) >= 0;
        }
        cInputPreProcessor_writeChar(self, '/');
    }
    while (*(int*)((char*)self + 0xC) >= 0) {
        cInputPreProcessor_writeChar(self, (char)*(int*)((char*)self + 0xC));
        if (*(int*)((char*)self + 0xC) == '\n') {
            *(int*)((char*)isrc + 0xC) += 1;
            cInputPreProcessor_readChar(self, isrc);
            break;
        }
        cInputPreProcessor_readChar(self, isrc);
    }
    return *(int*)((char*)self + 0xC) >= 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_writeString);
#ifdef SKIP_ASM
extern "C" void cInputPreProcessor_writeChar(void* self, char c);

/**
 * Writes a null-terminated string to the preprocessor output, one cInputPreProcessor_writeChar() per character.
 *
 * @param str  the string to write
 */
extern "C" void cInputPreProcessor_writeString(void* self, char* str)
{
    while (*str != 0) {
        cInputPreProcessor_writeChar(self, *str++);
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

struct sCharBuf_326360 {
    int count;
    int cap;
    char* data;
};

/**
 * Appends a character to the output buffer, growing it by 1 KB or half its size, whichever is larger.
 *
 * @param ch  the character
 */
extern "C" void cInputPreProcessor_writeChar(void* self, char ch)
{
    sCharBuf_326360* outBuf = (sCharBuf_326360*)self;
    if (outBuf->count == outBuf->cap) {
        int newCap = outBuf->count + 0x400;
        int grow = outBuf->count * 3 / 2;
        if (newCap < grow) {
            newCap = grow;
        }
        char* newData = (char*)operator_new_tag(newCap, str_PreProcBuf, 0, 0);
        func_0041605C(newData, outBuf->data, outBuf->cap);
        if (outBuf->data) {
            cMemMan_free(outBuf->data);
        }
        outBuf->data = newData;
        outBuf->cap = newCap;
    }
    outBuf->data[outBuf->count++] = ch;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("input/inputparse", cInputPreProcessor_writeTagLine);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cInputPreProcessor_writeString(void* self, char* msg);
extern char D_0048E3C8[];

/**
 * Writes a "# <line> "<name>"" tag line so the parser can report the right file and line.
 *
 * @param line  line number in the source
 * @param name  file name (passed as an int in this unit)
 */
extern "C" void cInputPreProcessor_writeTagLine(void* self, int line, int name)
{
    char buf[176];
    sprintf(buf, D_0048E3C8, line, name);
    cInputPreProcessor_writeString(self, buf);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readChar);
#ifdef SKIP_ASM
struct sParseStream {
    unsigned char* base;
    int size;
    unsigned char* cur;
};

/**
 * Reads the source's next byte into the preprocessor's current character, or -1 at the end.
 *
 * @param isrc  the input source (On Tour tInputSource&)
 */
extern "C" void cInputPreProcessor_readChar(void* self, sParseStream* isrc)
{
    if (isrc->cur == isrc->base + isrc->size) {
        *(int*)((char*)self + 0xc) = -1;
    } else {
        *(int*)((char*)self + 0xc) = *isrc->cur++;
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_skipCComment);
#ifdef SKIP_ASM
// PORT: func_00326A20__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void func_00326A20_5(void* self, sParseStream* s, int a2, int a3, const char* msg) __asm__("func_00326A20__FPv");
extern char str_unexpected_EOF_in_comment[];

/**
 * Skips a block comment, keeping its line breaks so line numbers stay right.
 *
 * Reads characters (the current one is at +0xC) until the closing star-slash: each newline inside counts a line on
 * @p isrc (+0xC) and is written out, and a comment without newlines is replaced by one space. When the input ends first
 * it calls the error reporter func_00326A20 with "unexpected EOF in comment"; that function is an empty stub in retail,
 * so nothing is reported.
 *
 * @param isrc      the input source (On Tour tInputSource); its line counter (+0xC) is advanced
 * @param path      directory of the file, for the error message
 * @param fileName  file name, for the error message
 */
// Skips a /* */ comment; newlines inside it are kept, otherwise it becomes one space.
extern "C" void cInputPreProcessor_skipCComment(void* self, sParseStream* isrc, int path, int fileName)
{
    int single = 1;
    int run = 1;
    while (run) {
        int ch = *(int*)((char*)self + 0xC);
        if (ch == '\n') {
            single = 0;
            *(int*)((char*)isrc + 0xC) += 1;
            cInputPreProcessor_writeChar(self, (char)*(int*)((char*)self + 0xC));
        } else if (ch == '*') {
            cInputPreProcessor_readChar(self, isrc);
            if (*(int*)((char*)self + 0xC) != '/')
                continue;
            cInputPreProcessor_readChar(self, isrc);
            if (single) {
                cInputPreProcessor_writeChar(self, ' ');
            }
            return;
        } else if (ch < 0) {
            goto error;
        }
        cInputPreProcessor_readChar(self, isrc);
    }
error:
    func_00326A20_5(self, isrc, path, fileName, str_unexpected_EOF_in_comment);
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_skipCppComment);
#ifdef SKIP_ASM
struct sParseStream;
extern "C" void cInputPreProcessor_readChar(void* self, sParseStream* s);
// PORT: func_00326A20__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void func_00326A20_5(void* self, sParseStream* s, int a2, int a3, const char* msg) __asm__("func_00326A20__FPv");
extern char str_unexpected_EOF_in_comment[];

/**
 * Skips a line comment up to the end of the line.
 *
 * Reads characters until a newline, which is counted on @p isrc, written out and consumed; reports "unexpected EOF in
 * comment" through func_00326A20 when the input ends first.
 *
 * @param isrc   the input source (On Tour tInputSource)
 * @param path  the file path, for the error message
 * @param fileName  the file name, for the error message
 */
extern "C" void cInputPreProcessor_skipCppComment(void* self, sParseStream* isrc, int path, int fileName)
{
    for (;;) {
        int ch = *(int*)((char*)self + 0xC);
        if (ch == '\n') {
            *(int*)((char*)isrc + 0xC) += 1;
            cInputPreProcessor_writeChar(self, (char)*(int*)((char*)self + 0xC));
            cInputPreProcessor_readChar(self, isrc);
            return;
        }
        if (ch < 0) {
            goto error;
        }
        cInputPreProcessor_readChar(self, isrc);
    }
error:
    func_00326A20_5(self, isrc, path, fileName, str_unexpected_EOF_in_comment);
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
extern "C" void cInputPreProcessor_skipToEOL(void* self, sParseStream* isrc)
{
    while (*(int*)((char*)self + 0xC) >= 0) {
        cInputPreProcessor_readChar(self, isrc);
        if (*(int*)((char*)self + 0xC) == 10) {
            cInputPreProcessor_readChar(self, isrc);
            *(int*)((char*)isrc + 0xC) += 1;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cInputPreProcessor_readCmdLine);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct sParseStream;
extern "C" void cInputPreProcessor_readChar(void* self, sParseStream* s);
extern "C" void cInputPreProcessor_writeTagLine(void* self, int a1, int a2);
extern "C" void cInputPreProcessor_readFile(void* self, int a1, int a2);
extern "C" void cInputPreProcessor_skipToEOL(void* self, sParseStream* s);
extern "C" int func_004165A8(const char* a, const char* b);
// PORT: func_00326A20__FPv is called with (self, s, a2, a3, msg) here; bind the 5-arg form to that symbol.
void func_00326A20_5(void* self, sParseStream* s, int a2, int a3, const char* msg) __asm__("func_00326A20__FPv");

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
extern "C" void cInputPreProcessor_readCmdLine(void* self, sParseStream* isrc, int path, int fn)
{
    char word[16];
    char name[160];
    char buf[160];
    int ch;
    while (isWS_3266E8(ch = *(int*)((char*)self + 0xC)))
        cInputPreProcessor_readChar(self, isrc);
    if (ch <= 0) {
        func_00326A20_5(self, isrc, path, fn, "unexpected EOF in pre-processor command");
        return;
    }
    char* wordPtr = word;
    do {
        if (!isAlpha_3266E8(*(int*)((char*)self + 0xC)))
            break;
        *wordPtr = *(int*)((char*)self + 0xC);
        cInputPreProcessor_readChar(self, isrc);
        wordPtr++;
    } while (wordPtr - word < 15);
    *wordPtr = 0;
    if (func_004165A8(word, "include") == 0) {
        while (isWS_3266E8(ch = *(int*)((char*)self + 0xC)))
            cInputPreProcessor_readChar(self, isrc);
        if (ch != '"') {
            func_00326A20_5(self, isrc, path, fn, "illegal 'include' directive");
            cInputPreProcessor_skipToEOL(self, isrc);
            return;
        }
        cInputPreProcessor_readChar(self, isrc);
        int n = 0;
        while (*(int*)((char*)self + 0xC) != '"') {
            int nameChar = *(int*)((char*)self + 0xC);
            if (nameChar < 0 || nameChar == '\n') {
                func_00326A20_5(self, isrc, path, fn, "illegal 'include' directive");
                cInputPreProcessor_skipToEOL(self, isrc);
                return;
            }
            name[n++] = *(int*)((char*)self + 0xC);
            cInputPreProcessor_readChar(self, isrc);
        }
        name[n] = 0;
        cInputPreProcessor_readChar(self, isrc);
        while (isWS_3266E8(ch = *(int*)((char*)self + 0xC)))
            cInputPreProcessor_readChar(self, isrc);
        if (ch >= 0 && ch != '\n') {
            func_00326A20_5(self, isrc, path, fn, "illegal 'include' directive");
            cInputPreProcessor_skipToEOL(self, isrc);
            return;
        }
        if (ch == '\n')
            *(int*)((char*)isrc + 0xC) += 1;
        // PORT: the unit declares cInputPreProcessor_readFile's third parameter as int; it receives a char*.
        cInputPreProcessor_readFile(self, path, (int)name);
        sprintf(buf, "%s%s", path, fn);
        cInputPreProcessor_writeTagLine(self, *(int*)((char*)isrc + 0xC), (int)buf);
        cInputPreProcessor_readChar(self, isrc);
    } else {
        func_00326A20_5(self, isrc, path, fn, "illegal pre-processor directive");
        cInputPreProcessor_skipToEOL(self, isrc);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326A20__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the input preprocessor's error report, compiled out of this build.
 *
 * cInputPreProcessor_skipCComment, cInputPreProcessor_skipCppComment and cInputPreProcessor_readCmdLine call it with
 * the input source, path, file name and a message such as "unexpected EOF in comment"; the arguments are ignored.
 */
void func_00326A20(void* self)
{
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326A28);
#ifdef SKIP_ASM
extern "C" int func_00326D60(void* p, int n);

/**
 * Initialises the pad cache: creates the pad devices and resets the ring buffer positions.
 *
 * Stores the count func_00326D60 returns for the device array (+0x2EEC, up to 4) in +0x2EE8, sets the read position
 * (+0x2EE0) to 29 and the write position (+0x2EE4) to 0. Called by cSSXApp_init.
 */
extern "C" void func_00326A28(void* self)
{
    *(int*)((char*)self + 0x2EE8) = func_00326D60((char*)self + 0x2EEC, 4);
    *(int*)((char*)self + 0x2EE0) = 29;
    *(int*)((char*)self + 0x2EE4) = 0;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326A68);
#ifdef SKIP_ASM
struct sVEntry00326A68 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sObj00326A68 {
    sVEntry00326A68* vt;
};

struct sOwner00326A68 {
    char pad[0x2EE8];
    int count;
    sObj00326A68* objs[1];
};

/**
 * Destroys every pad device of the pad cache.
 *
 * Calls each non-null device's virtual destructor (slot 1) with flags 3, which frees it. The array entries and the
 * count (+0x2EE8) are left as they were. Called by cSSXApp_purge.
 */
extern "C" void func_00326A68(sOwner00326A68* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        sObj00326A68* inputPad = self->objs[i];
        if (inputPad != 0) {
            inputPad->vt[1].fn((char*)inputPad + inputPad->vt[1].delta, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326AE0);
#ifdef SKIP_ASM
extern "C" void func_00326CF0(void* p);

/**
 * Stops the vibration of every pad device of the pad cache.
 *
 * Calls func_00326CF0 on each device. Called by cGame_update and cGame_updateUI.
 */
extern "C" void func_00326AE0(void* self)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x2EE8); i++) {
        func_00326CF0(((void**)((char*)self + 0x2EEC))[i]);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326B48);
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
 * @param controllerCache  the pad cache (this)
 * @return 1 when it advanced, 0 when no newer frame is cached
 */
extern "C" int func_00326B48(sInputParse_00326B48* controllerCache)
{
    unsigned int next = (controllerCache->head + 1) % 30;
    if (next == controllerCache->tail) {
        return 0;
    }
    controllerCache->head = next;
    return 1;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", cPadCache_cacheInput);
#ifdef SKIP_ASM
class cInputDevice_326B88 {
public:
    virtual void v01();
    virtual int read(char* buf, int size);
};

struct sInputSlot_00326B88 {
    int value;
    char data[0x60];
};

struct sInputParse_00326B88 {
    sInputSlot_00326B88 slots[30][4];
    unsigned int head;
    unsigned int tail;
    int count;
    cInputDevice_326B88* devices[4];
};

/**
 * Reads every pad device into the next slot of the 30-entry input ring buffer.
 */
extern "C" void cPadCache_cacheInput(sInputParse_00326B88* self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        self->slots[self->tail][i].value = self->devices[i]->read(self->slots[self->tail][i].data, 0x18);
    }
    self->tail = (self->tail + 1) % 30;
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
extern "C" int cPadCache_flush(sInputParse_00326C60* self)
{
    int tail = self->tail;
    int head = self->head;
    self->head = (tail + 29) % 30;
    return (tail - head + 30) % 30;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326CA0);
#ifdef SKIP_ASM
struct sInputSlot_00326CA0 {
    int value;
    char pad[0x60];
};

struct sInputParse_00326CA0 {
    sInputSlot_00326CA0 slots[30][4];
    int bank;
};

/**
 * Returns how many button values a pad delivered in the pad cache's current input frame.
 *
 * Reads the count word of line (read position +0x2EE0), pad @p i. Called by cSSXApp_preUpdate and
 * cPreStartScreen_update together with func_00326CC8.
 *
 * @param i  the pad index (On Tour pad)
 * @return the number of button values
 */
extern "C" int func_00326CA0(void* self, int i)
{
    sInputParse_00326CA0* controllerCache = (sInputParse_00326CA0*)self;
    return controllerCache->slots[controllerCache->bank][i].value;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326CC8);
#ifdef SKIP_ASM
struct sInputSlot_00326CC8 {
    int value;
    char data[0x60];
};

struct sInputParse_00326CC8 {
    sInputSlot_00326CC8 slots[30][4];
    int bank;
};

/**
 * Returns the button values a pad delivered in the pad cache's current input frame.
 *
 * The values (up to 24 floats) of line (read position +0x2EE0), pad @p i.
 *
 * @param i  the pad index (On Tour pad)
 * @return the address of the values
 */
extern "C" void* func_00326CC8(void* self, int i)
{
    sInputParse_00326CC8* controllerCache = (sInputParse_00326CC8*)self;
    return controllerCache->slots[controllerCache->bank][i].data;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326CF0);
#ifdef SKIP_ASM
struct sVEntry00326CF0 {
    short delta;
    short index;
    void* fn;
};

typedef int (*CountFn00326CF0)(void*);
typedef void (*SetFn00326CF0)(void*, int, float);

/**
 * Stops a pad device's vibration: sets every actuator to 0.
 *
 * Calls the device's virtual setActuator (slot 4) with level 0.0 for each actuator it reports through slot 3. Called by
 * func_00326AE0 and func_00125B18.
 *
 * @param inputPad  the pad device (this)
 */
extern "C" void func_00326CF0(void* inputPad)
{
    sVEntry00326CF0** self = (sVEntry00326CF0**)inputPad;
    int i;
    for (i = 0; i < ((CountFn00326CF0)(*self)[3].fn)((char*)self + (*self)[3].delta); i++) {
        ((SetFn00326CF0)(*self)[4].fn)((char*)self + (*self)[4].delta, i, 0.0f);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326D60);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_00326DF0(void* self, int a1, int a2);
extern char str_PS2Pad[];

/**
 * Creates the PS2 pad devices: one per controller port, at most two.
 *
 * Clamps @p n to 2 and fills @p inputPads[i] with a 0x180-byte "PS2Pad" allocation constructed by func_00326DF0 for
 * port i, slot 0.
 *
 * @param inputPads  the array to fill (On Tour pads)
 * @param n  its capacity (On Tour maxpads)
 * @return the number of devices created
 */
extern "C" int func_00326D60(void* inputPads, int n)
{
    void** arr = (void**)inputPads;
    int i;
    if (n > 2) {
        n = 2;
    }
    for (i = 0; i < n; i++) {
        arr[i] = func_00326DF0(cMemMan_alloc(0x180, str_PS2Pad, 0x4000000, 0), i, 0);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326DF0);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern void* D_0048E488[];

/**
 * Constructs a PS2 pad device for a controller port and slot.
 *
 * Installs the vtable D_0048E488, stores @p port (+0x4) and @p slot (+0x8), zeroes 36 bytes at +0xC and the sampling state
 * words +0x48..+0x5C (state 0: the port is opened on the next sample). Called by func_00326D60.
 *
 * @param port  the controller port
 * @param slot  the multitap slot
 * @return @p self
 */
extern "C" void* func_00326DF0(void* self, int port, int slot)
{
    *(void***)self = D_0048E488;
    *(int*)((char*)self + 0x4) = port;
    *(int*)((char*)self + 0x8) = slot;
    func_00416210((char*)self + 0xC, 0, 36);
    *(int*)((char*)self + 0x54) = 0;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x50) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326E50);
#ifdef SKIP_ASM
extern "C" int func_003FF8F0(int a, int b);
void operator_delete(int* ptr);
extern void* D_0048E488[];
extern char cPadDevice_vtbl[];

/**
 * Destroys a PS2 pad device: closes its controller port and frees the object when asked to.
 *
 * Reinstalls D_0048E488, closes the port (func_003FF8F0 with port +0x4 and slot +0x8), installs cPadDevice_vtbl and
 * calls operator delete when bit 0 of @p flags is set. Slot 1 of D_0048E488.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void func_00326E50(void* self, int flags)
{
    *(void***)self = D_0048E488;
    func_003FF8F0(*(int*)((char*)self + 0x4), *(int*)((char*)self + 0x8));
    *(void**)self = cPadDevice_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00326EB0);
#ifdef SKIP_ASM
extern "C" int func_003FF708(int port, int slot, void* buf);
extern "C" int func_003FFA58(int port, int slot, unsigned char* data);
extern "C" int func_003FFBC0(int port, int slot);
extern "C" int func_003FFCD8(int port, int slot);
extern "C" int func_003FFD60(int port, int slot, int id, int idx);
extern "C" int func_003FFFA0(int port, int slot, int term, int offs);
extern "C" int func_004000D8(int port, int slot, int mode, int lock);
extern "C" int func_00400190(int port, int slot, unsigned char* data);
extern "C" int func_00400250(int port, int slot, unsigned char* data);
extern "C" int func_00400490(int port, int slot);
extern "C" int func_004004F0(int port, int slot);
extern unsigned char D_004A3FB8;

struct sPad_326EB0 {
    char pad0[0x4];
    int port;                   // 0x4
    int slot;                   // 0x8
    char padC[0x30 - 0xC];
    int actSmall;               // 0x30
    int lastSmall;              // 0x34
    int actBig;                 // 0x38
    int lastBig;                // 0x3C
    unsigned char actDirect[8]; // 0x40
    int phase;                  // 0x48
    int retry;                  // 0x4C
    int modeSet;                // 0x50
    int pressure;               // 0x54
    int analog;                 // 0x58
    int actuator;               // 0x5C
    char pad60[0x80 - 0x60];
    unsigned char buf[0x100];   // 0x80
};

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
 * mode (+0x54 set when that works), align the actuators (+0x5C) and finally read (state 10). A lost or reset pad goes
 * back to state 1.
 *
 * Once a read succeeds it fills 24 values in @p out: Select, Start, L3 and R3 as 0 or 1; then the d-pad (right, left,
 * up, down), triangle, circle, cross, square, L1, R1, L2 and R2, as pressure / 255 in pressure mode, else 0 or 1; then
 * the four analogue stick axes (+0x58 set; else zeros) split into eight half-axis values 0..1, with a dead zone of
 * about 48 steps either side of the centre. In state 10 it also sends changed motor levels (+0x30, +0x38) with
 * func_00400190. Slot 2 of D_0048E488; cPadCache_cacheInput calls it for every pad.
 *
 * @param out  receives up to 24 button values
 * @return 24 when the pad was read, otherwise 0
 */
extern "C" int func_00326EB0(sPad_326EB0* self, float* out)
{
    unsigned char data[32];
    int ok = 0;
    int ret;
    if (self->phase == 0) {
        int r = func_003FF708(self->port, self->slot, self->buf);
        if (r == 1) {
            self->phase = r;
            self->modeSet = 0;
            self->actSmall = 0;
            self->lastSmall = -1;
            self->actBig = 0;
            self->lastBig = -1;
        }
    }
    int state = func_003FFBC0(self->port, self->slot);
    if (state == 0 || state == 7) {
        self->phase = 1;
        self->modeSet = 0;
    }
    switch (self->phase) {
    case 1:
        if (state == 6 || state == 2) {
            self->pressure = 0;
            self->analog = 0;
            self->actuator = 0;
            int id = func_003FFFA0(self->port, self->slot, 1, 0);
            if (id != 0) {
                int ext = func_003FFFA0(self->port, self->slot, 2, 0);
                switch (ext) {
                case 0:
                    if (id == 4)
                        self->phase = 10;
                    break;
                case 4:
                    self->phase = 2;
                    break;
                case 7:
                    self->analog = 1;
                    self->phase = 2;
                    break;
                }
            }
        }
        break;
    case 2:
        if (self->analog == 0 || self->modeSet == 0) {
            int r = func_004000D8(self->port, self->slot, 1, 3);
            if (r == 1) {
                self->modeSet = r;
                self->phase = 3;
            }
        } else {
            self->phase = 4;
        }
        break;
    case 3:
        if (func_003FFBC0(self->port, self->slot) != 5)
            self->phase = 1;
        break;
    case 4:
        if (func_00400490(self->port, self->slot) == 1)
            self->phase = 5;
        else
            self->phase = 7;
        break;
    case 5:
        if (func_004004F0(self->port, self->slot) == 1) {
            self->retry = 0;
            self->phase = 6;
        }
        break;
    case 6: {
        int r = func_003FFCD8(self->port, self->slot);
        if (r == 1) {
            self->phase = 1;
            self->modeSet = 0;
        } else if (r == 0) {
            if (++self->retry >= 5) {
                self->pressure = 1;
                self->phase = 7;
            }
        } else {
            self->retry = 0;
        }
        break;
    }
    case 7:
        if (func_003FFD60(self->port, self->slot, -1, 0) != 0)
            self->phase = 8;
        else
            self->phase = 10;
        break;
    case 8:
        if (func_00400250(self->port, self->slot, &D_004A3FB8) == 1) {
            self->retry = 0;
            self->phase = 9;
        }
        break;
    case 9: {
        int r = func_003FFCD8(self->port, self->slot);
        if (r == 1) {
            self->phase = 1;
            self->modeSet = 0;
        } else if (r == 0) {
            if (++self->retry >= 5) {
                self->actuator = 1;
                self->phase = 10;
            }
        } else {
            self->retry = 0;
        }
        break;
    }
    case 10:
        if (func_003FFA58(self->port, self->slot, data) == 0 || data[0] != 0) {
            self->phase = 1;
            self->modeSet = 0;
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
        if (self->pressure != 0) {
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
        if (self->analog != 0) {
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
    if (self->phase == 10 && (self->actSmall != self->lastSmall || self->actBig != self->lastBig)) {
        self->actDirect[0] = self->actSmall;
        self->actDirect[1] = self->actBig;
        self->lastSmall = self->actSmall;
        self->lastBig = self->actBig;
        func_00400190(self->port, self->slot, self->actDirect);
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327738__FPv);
#ifdef SKIP_ASM
/**
 * Returns 2: a PS2 pad has two vibration actuators.
 *
 * Slot 3 of D_0048E488, overriding cPadDevice_numActuators (0).
 *
 * @return 2
 */
int func_00327738(void* self)
{
    return 0x2;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327740);
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
 * Actuator 1 is the on/off motor: +0x30 = (@p level != 0). Any other actuator sets the variable motor: +0x38 = level x
 * 205 + 50 clamped to 50..255, or 0 when @p level is 0. func_00326EB0 sends changed values to the pad. Slot 4 of
 * D_0048E488.
 *
 * @param mode  the actuator index (On Tour act)
 * @param level  the strength, 0..1 (On Tour frequency)
 */
extern "C" void func_00327740(void* self, int mode, float level)
{
    if (mode == 1) {
        *(int*)((char*)self + 0x30) = level != 0.0f;
        return;
    }
    int* motorOut = (int*)((char*)self + 0x38);
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
extern "C" void cPadDevice__cPadDevice(void* self, int flags)
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
int cPadDevice_numActuators(void* self)
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
void cPadDevice_setActuator(void* self)
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
void* cAIFwdDiffCache_cAIFwdDiffCache(void* self)
{
    int initValue = 0;
    *(int*)self = initValue;
    *(int*)((char*)self + 0x4) = initValue;
    *(int*)((char*)self + 0x8) = initValue;
    *(int*)((char*)self + 0xc) = initValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("input/inputparse", func_00327828);
#ifdef SKIP_ASM
void cMemMan_free(void* ptr);
void operator_delete(int* ptr);

/**
 * Destroys an AI forward-difference cache: frees its node array and hash table and, when asked, the cache itself.
 *
 * Frees the words at +0x0 and +0x8 with cMemMan_free when set, then calls operator delete when bit 0 of @p flags is
 * set. Called by cBXNash_purgeTheAIFwdDiffCache.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
extern "C" void func_00327828(void* self, int flags)
{
    if (*(void**)self != 0) {
        cMemMan_free(*(void**)self);
    }
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

