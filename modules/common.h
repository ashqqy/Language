#ifndef COMMON_ERRORS_H
#define COMMON_ERRORS_H

#include <stdlib.h>
#include <stdio.h>

typedef enum
{
    NO_ERRORS           = 1,
    EXPECTED_INPUT_FILE = 2,
    OPENING_FILE_ERROR  = 3,
    READING_FILE_ERROR  = 4,
    TOKENIZATION_ERROR  = 5,
    ALLOCATION_ERROR    = 6
} common_errors;

typedef enum
{
    NUMBER   = 1,
    NAME     = 2,
    RESERVED = 3
} tree_data_type_t;

typedef struct
{
    char* begin;
    size_t len;
    int index;
} name_t;

typedef enum
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
    LCURBR  = 10,
    RCURBR  = 11,
    SEMI    = 12,

    INT     = 13,
    DBL     = 14,

    OR      = 15,
    AND     = 16,
    NOT     = 17,

    EQ      = 18,
    NEQ     = 19,
    GT      = 20,
    LT      = 21,
    GTE     = 22,
    LTE     = 23,

    IF      = 24,
    ELSE    = 25,
    WHILE   = 26,
    PRINT   = 27
} reserved_t;

typedef struct 
{
    tree_data_type_t type;

    union
    {
        double number;
        name_t name; 
        reserved_t reserved;
    } content;
} tree_data_t;

typedef struct
{
    const char* name;
    tree_data_t data;
} reserved_names_t;

static const reserved_names_t reserved_names[] = 
{
    {.name = "(",       .data = {.type = RESERVED, .content = {.reserved = LBRACK}}},
    {.name = ")",       .data = {.type = RESERVED, .content = {.reserved = RBRACK}}},
    {.name = "{",       .data = {.type = RESERVED, .content = {.reserved = LCURBR}}},
    {.name = "}",       .data = {.type = RESERVED, .content = {.reserved = RCURBR}}},
    {.name = ";",       .data = {.type = RESERVED, .content = {.reserved = SEMI}}},

    {.name = "=",       .data = {.type = RESERVED, .content = {.reserved = ASG}}},

    {.name = "+",       .data = {.type = RESERVED, .content = {.reserved = ADD}}},
    {.name = "-",       .data = {.type = RESERVED, .content = {.reserved = SUB}}},
    {.name = "*",       .data = {.type = RESERVED, .content = {.reserved = MUL}}},
    {.name = "/",       .data = {.type = RESERVED, .content = {.reserved = DIV}}},
    {.name = "^",       .data = {.type = RESERVED, .content = {.reserved = DEG}}},

    {.name = "<",       .data = {.type = RESERVED, .content = {.reserved = LT}}},
    {.name = ">",       .data = {.type = RESERVED, .content = {.reserved = GT}}},
    {.name = "==",      .data = {.type = RESERVED, .content = {.reserved = EQ}}},
    {.name = "!=",      .data = {.type = RESERVED, .content = {.reserved = NEQ}}},
    {.name = "<=",      .data = {.type = RESERVED, .content = {.reserved = LTE}}},
    {.name = ">=",      .data = {.type = RESERVED, .content = {.reserved = GTE}}},

    {.name = "||",      .data = {.type = RESERVED, .content = {.reserved = OR}}},
    {.name = "&&",      .data = {.type = RESERVED, .content = {.reserved = AND}}},
    {.name = "!",       .data = {.type = RESERVED, .content = {.reserved = NOT}}},

    {.name = "int",     .data = {.type = RESERVED, .content = {.reserved = INT}}},
    {.name = "double",  .data = {.type = RESERVED, .content = {.reserved = DBL}}},
    {.name = "if",      .data = {.type = RESERVED, .content = {.reserved = IF}}},
    {.name = "else",    .data = {.type = RESERVED, .content = {.reserved = ELSE}}},
    {.name = "while",   .data = {.type = RESERVED, .content = {.reserved = WHILE}}},
    {.name = "print",   .data = {.type = RESERVED, .content = {.reserved = PRINT}}}
};

int FindReservedDataByName (const char* name_begin, size_t name_len, tree_data_t* data);
const char* FindReservedNameByData (tree_data_t data);

char* ReadFile (FILE* file_input, size_t* n_readen);
size_t FileSizeFinder (FILE* file);

void* MyCalloc (size_t n_elems, size_t size_elems, const void* poison);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, const void* poison);

// soft assert
#define CUSTOM_WARNING(expression, return_message) do                                                              \
{                                                                                                                  \
    if (!(expression))                                                                                             \
    {                                                                                                              \
        fprintf (stderr, "%s: %s:%d: Expression `%s' warning.\n", __FUNCTION__, __FILE__, __LINE__, #expression);  \
        return return_message;                                                                                     \
    }                                                                                                              \
}                                                                                                                  \
while (0)                                                                                                          \

// hard assert
#define CUSTOM_ASSERT(expression) do                                                                             \
{                                                                                                                \
    if (!(expression))                                                                                           \
    {                                                                                                            \
        fprintf (stderr, "%s: %s:%d: Assertion `%s' failed.\n", __FUNCTION__, __FILE__, __LINE__, #expression);  \
        fprintf (stderr, "Program aborting\n");                                                                  \
        exit(EXIT_FAILURE);                                                                                      \
    }                                                                                                            \
}                                                                                                                \
while (0)  

#define FREE(ptr) free (ptr); ptr = NULL;

#endif // COMMON_ERRORS_H
