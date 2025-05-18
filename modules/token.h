#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef struct token           token_t;
typedef enum   token_type      token_type_t;
typedef struct identifier      identifier_t;
typedef enum   keyword         keyword_t;
typedef struct keyword_mapping keyword_mapping_t;

//--------------------------------------------------------------------------

struct token
{
    token_type_t token_type;

    union
    {
        double       constant;
        identifier_t identifier; 
        keyword_t    keyword;
    } content;
};

//--------------------------------------------------------------------------

enum token_type
{
    CONSTANT   = 1,
    IDENTIFIER = 2,
    KEYWORD    = 3
};

//--------------------------------------------------------------------------

struct identifier
{
    char*  identifier_begin;
    size_t identifier_lenght;
    int    identifier_index;
};

//--------------------------------------------------------------------------

enum keyword
{
    ENTRY_POINT         = 1,
    END                 = 2,

    IF                  = 11,
    ELSE                = 12,
    WHILE               = 13,
    ASSIGNMENT          = 14,

    ADD                 = 21,
    SUB                 = 22,
    MUL                 = 23,
    DIV                 = 24,
    DEG                 = 25,
    SQRT                = 26,

    EQUAL               = 31,
    LESS                = 32,
    GREATER             = 33,
    LESS_OR_EQUAL       = 34,
    GREATER_OR_EQUAL    = 35,
    NOT_EQUAL           = 36,
    AND                 = 37,
    OR                  = 38,
    NOT                 = 39,

    OPERATOR_SEPARATOR  = 41,
    ARGUMENT_SEPARATOR  = 42,

    NUMBER              = 51,

    INPUT               = 61,
    PRINT               = 62,

    RETURN              = 71,
    BREAK               = 72,
    CONTINUE            = 73,
    ABORT               = 74,

    FUNCTION_DEFINITION = 81,
    FUNCTION_CALL       = 82,

    LEFT_BRACKET        = 91,
    RIGHT_BRACKET       = 92,
    BLOCK_OPEN          = 93,
    BLOCK_CLOSE         = 94,
};

struct keyword_mapping
{
    const char* string;
    keyword_t   keyword;
};

static const keyword_mapping_t keyword_mappings[] = 
{
    {.string = "entry",       .keyword = ENTRY_POINT},

    {.string = "if",          .keyword = IF},
    {.string = "else",        .keyword = ELSE},
    {.string = "while",       .keyword = WHILE},
    {.string = "=",           .keyword = ASSIGNMENT},

    {.string = "+",           .keyword = ADD},
    {.string = "-",           .keyword = SUB},
    {.string = "*",           .keyword = MUL},
    {.string = "/",           .keyword = DIV},
    {.string = "^",           .keyword = DEG},
    {.string = "sqrt",        .keyword = SQRT},

    {.string = "==",          .keyword = EQUAL},
    {.string = "<",           .keyword = LESS},
    {.string = ">",           .keyword = GREATER},
    {.string = "<=",          .keyword = LESS_OR_EQUAL},
    {.string = ">=",          .keyword = GREATER_OR_EQUAL},
    {.string = "!=",          .keyword = NOT_EQUAL},
    {.string = "&&",          .keyword = AND},
    {.string = "||",          .keyword = OR},
    {.string = "!",           .keyword = NOT},

    {.string = ";",           .keyword = OPERATOR_SEPARATOR},
    {.string = ",",           .keyword = ARGUMENT_SEPARATOR},

    {.string = "number",      .keyword = NUMBER},

    {.string = "input",       .keyword = INPUT},
    {.string = "print",       .keyword = PRINT},

    {.string = "return",      .keyword = RETURN},
    {.string = "break",       .keyword = BREAK},
    {.string = "continue",    .keyword = CONTINUE},
    {.string = "abort",       .keyword = ABORT},

    {.string = "function",    .keyword = FUNCTION_DEFINITION},
    {.string = "call",        .keyword = FUNCTION_CALL},

    {.string = "(",           .keyword = LEFT_BRACKET},
    {.string = ")",           .keyword = RIGHT_BRACKET},
    {.string = "{",           .keyword = BLOCK_OPEN},
    {.string = "}",           .keyword = BLOCK_CLOSE},
};

//--------------------------------------------------------------------------

int         FindKeywordByName (const char* name_begin, size_t name_len, token_t* keyword);
const char* FindNameByKeyword (token_t keyword);

#endif // TOKEN_H
