#ifndef COMMON_ERRORS_H
#define COMMON_ERRORS_H

enum common_errors
{
    OPENING_FILE_ERROR = 1,
    READING_FILE_ERROR = 2
};

enum tree_data_type_t
{
    NUMBER   = 1,
    NAME     = 2,
    RESERVED = 3
};

struct name_t
{
    char* begin;
    size_t len;
};

enum reserved_t
{
    END     = 1,

    ASG     = 2,

    ADD     = 3,
    SUB     = 4,
    MUL     = 5,
    DIV     = 6,
    DEG     = 7,

    LBRACK  = 8,
    RBRACK  = 9,
    SEMI    = 10,

    INT     = 11,
    DBL     = 12,

    OR      = 13,
    AND     = 14,
    NOT     = 15,

    EQ      = 16,
    NEQ     = 17,
    GT      = 18,
    LT      = 19,
    GTE     = 20,
    LTE     = 21,

    IF      = 22,
    ELSE    = 23
};

struct tree_data_t
{
    tree_data_type_t type;

    union
    {
        double number;
        name_t name; 
        reserved_t reserved;
    } content;
};

struct reserved_names_t
{
    const char* name;
    tree_data_t data;
};

const reserved_names_t reserved_names[] = 
{
    {.name = "(",  .data = {.type = RESERVED, .content = {.reserved = LBRACK}}},
    {.name = ")",  .data = {.type = RESERVED, .content = {.reserved = RBRACK}}},
    {.name = ";",  .data = {.type = RESERVED, .content = {.reserved = SEMI}}},

    {.name = "||",  .data = {.type = RESERVED, .content = {.reserved = OR}}},
    {.name = "&&",  .data = {.type = RESERVED, .content = {.reserved = AND}}},
    {.name = "!",  .data = {.type = RESERVED, .content = {.reserved = NOT}}},

    {.name = "==",  .data = {.type = RESERVED, .content = {.reserved = EQ}}},
    {.name = "!=",  .data = {.type = RESERVED, .content = {.reserved = NEQ}}},
    {.name = "<=",  .data = {.type = RESERVED, .content = {.reserved = LTE}}},
    {.name = ">=",  .data = {.type = RESERVED, .content = {.reserved = GTE}}},
    {.name = "<",  .data = {.type = RESERVED, .content = {.reserved = LT}}},
    {.name = ">",  .data = {.type = RESERVED, .content = {.reserved = GT}}},

    {.name = "=",  .data = {.type = RESERVED, .content = {.reserved = ASG}}},

    {.name = "+",  .data = {.type = RESERVED, .content = {.reserved = ADD}}},
    {.name = "-",  .data = {.type = RESERVED, .content = {.reserved = SUB}}},
    {.name = "*",  .data = {.type = RESERVED, .content = {.reserved = MUL}}},
    {.name = "/",  .data = {.type = RESERVED, .content = {.reserved = DIV}}},
    {.name = "^",  .data = {.type = RESERVED, .content = {.reserved = DEG}}},

    {.name = "int",  .data = {.type = RESERVED, .content = {.reserved = INT}}},
    {.name = "double",  .data = {.type = RESERVED, .content = {.reserved = DBL}}},
    {.name = "if",  .data = {.type = RESERVED, .content = {.reserved = IF}}},
    {.name = "else",  .data = {.type = RESERVED, .content = {.reserved = ELSE}}}
};

int FindReservedDataByName (const char* name_begin, size_t name_len, tree_data_t* data);
const char* FindReservedNameByData (tree_data_t data);

#endif // COMMON_ERRORS_H