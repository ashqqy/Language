#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef struct token           token_t;
typedef enum   token_type      token_type_t;
typedef union  token_content   token_content_t;

typedef struct identifier      identifier_t;
typedef struct name_table      name_table_t;

typedef enum   keyword         keyword_t;
typedef struct keyword_mapping keyword_mapping_t;

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
    char*  begin;
    size_t length;
    int    index;
};

struct name_table
{
    identifier_t* name_table;
    size_t size;
    size_t capacity;
};

//--------------------------------------------------------------------------

enum keyword
{
    UNKNOWN_KEYWORD     = 0,

    ASSIGNMENT          = 11,
    IF                  = 12,
    ELSE                = 13,
    WHILE               = 14,
    RETURN              = 15,

    VOID                = 21,
    DOUBLE              = 22,

    INPUT               = 31,
    PRINT               = 32,

    TERMINATOR          = 41,
    SEPARATOR           = 42,

    LEFT_BRACKET        = 51,
    RIGHT_BRACKET       = 52,
    BLOCK_OPEN          = 53,
    BLOCK_CLOSE         = 54,

    ADD                 = 61,
    SUB                 = 62,
    MUL                 = 63,
    DIV                 = 64,

    EQUAL               = 71,
    LESS                = 72,
    GREATER             = 73,
    LESS_OR_EQUAL       = 74,
    GREATER_OR_EQUAL    = 75,
    NOT_EQUAL           = 76
};

//--------------------------------------------------------------------------

union token_content
{
    double       constant;
    identifier_t identifier; 
    keyword_t    keyword;
};

struct token
{
    token_type_t    type;
    token_content_t content;
};

//--------------------------------------------------------------------------

struct keyword_mapping
{
    const char* string;
    keyword_t   keyword;
};

static const keyword_mapping_t keyword_mappings[] = 
{
    {.string = "=",           .keyword = ASSIGNMENT},
    {.string = "if",          .keyword = IF},
    {.string = "else",        .keyword = ELSE},
    {.string = "while",       .keyword = WHILE},
    {.string = "return",      .keyword = RETURN},

    {.string = "void",        .keyword = VOID},
    {.string = "double",      .keyword = DOUBLE},

    {.string = "input",       .keyword = INPUT},
    {.string = "print",       .keyword = PRINT},

    {.string = ";",           .keyword = TERMINATOR},
    {.string = ",",           .keyword = SEPARATOR},

    {.string = "(",           .keyword = LEFT_BRACKET},
    {.string = ")",           .keyword = RIGHT_BRACKET},
    {.string = "{",           .keyword = BLOCK_OPEN},
    {.string = "}",           .keyword = BLOCK_CLOSE},

    {.string = "+",           .keyword = ADD},
    {.string = "-",           .keyword = SUB},
    {.string = "*",           .keyword = MUL},
    {.string = "/",           .keyword = DIV},

    {.string = "==",          .keyword = EQUAL},
    {.string = "<",           .keyword = LESS},
    {.string = ">",           .keyword = GREATER},
    {.string = "<=",          .keyword = LESS_OR_EQUAL},
    {.string = ">=",          .keyword = GREATER_OR_EQUAL},
    {.string = "!=",          .keyword = NOT_EQUAL},
};

//--------------------------------------------------------------------------

keyword_t   StringToKeyword (const char* string_begin, size_t string_len);
const char* KeywordToString (keyword_t keyword);

#endif // TOKEN_H
