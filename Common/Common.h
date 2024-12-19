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
    END = 1,

    ASG = 2,

    ADD = 3,
    SUB = 4,
    MUL = 5,
    DIV = 6,
    DEG = 7,

    LBRACK = 8,
    RBRACK = 9,
    SEMI   = 10,

    INT = 11,
    DBL = 12
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

    {.name = "=",  .data = {.type = RESERVED, .content = {.reserved = ASG}}},

    {.name = "+",  .data = {.type = RESERVED, .content = {.reserved = ADD}}},
    {.name = "-",  .data = {.type = RESERVED, .content = {.reserved = SUB}}},
    {.name = "*",  .data = {.type = RESERVED, .content = {.reserved = MUL}}},
    {.name = "/",  .data = {.type = RESERVED, .content = {.reserved = DIV}}},
    {.name = "^",  .data = {.type = RESERVED, .content = {.reserved = DEG}}},

    {.name = "int",  .data = {.type = RESERVED, .content = {.reserved = INT}}},
    {.name = "double",  .data = {.type = RESERVED, .content = {.reserved = DBL}}}
};

int FindReservedDataByName (const char* name_begin, size_t name_len, tree_data_t* data);
const char* FindReservedNameByData (tree_data_t data);

#endif // COMMON_ERRORS_H