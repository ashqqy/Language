#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

//--------------------------------------------------------------------------

keyword_t StringToKeyword (const char* string_begin, size_t string_len)
{
    assert (string_begin != NULL);

    keyword_t keyword = UNKNOWN_KEYWORD;

    for (size_t i = 0; i < sizeof (keyword_mappings) / sizeof (keyword_mappings[0]); ++i)
    {
        if (strncmp (string_begin, keyword_mappings[i].string, string_len) == 0)
        {
            keyword = keyword_mappings[i].keyword;
            return keyword;
        }
    }

    return keyword;
}

//--------------------------------------------------------------------------

const char* KeywordToString (keyword_t keyword)
{
    for (size_t i = 0; i < sizeof (keyword_mappings) / sizeof (keyword_mappings[0]); ++i)
    {
        if (keyword == keyword_mappings[i].keyword)
        {
            return keyword_mappings[i].string;
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------

const char* TypeToString (token_type_t type)
{
    for (size_t i = 0; i < sizeof (token_type_mappings) / sizeof (token_type_mappings[0]); ++i)
    {
        if (type == token_type_mappings[i].type)
        {
            return token_type_mappings[i].string;
        }
    }

    return NULL;
}

//--------------------------------------------------------------------------

const char* KeywordEnumToString (keyword_t keyword)
{
    #define ENUM_TO_STRING(enum) case enum: return #enum

    switch (keyword)
    {
        ENUM_TO_STRING (UNKNOWN_KEYWORD);
        ENUM_TO_STRING (ASSIGNMENT);
        ENUM_TO_STRING (IF);
        ENUM_TO_STRING (ELSE);
        ENUM_TO_STRING (WHILE);
        ENUM_TO_STRING (RETURN);
        ENUM_TO_STRING (VOID);
        ENUM_TO_STRING (INT);
        ENUM_TO_STRING (DOUBLE);
        ENUM_TO_STRING (INPUT);
        ENUM_TO_STRING (PRINT);
        ENUM_TO_STRING (TERMINATOR);
        ENUM_TO_STRING (ARGUMENTS_SEPARATOR);
        ENUM_TO_STRING (LEFT_BRACKET);
        ENUM_TO_STRING (RIGHT_BRACKET);
        ENUM_TO_STRING (BLOCK_OPEN);
        ENUM_TO_STRING (BLOCK_CLOSE);
        ENUM_TO_STRING (ADD);
        ENUM_TO_STRING (SUB);
        ENUM_TO_STRING (MUL);
        ENUM_TO_STRING (DIV);
        ENUM_TO_STRING (EQUAL);
        ENUM_TO_STRING (LESS);
        ENUM_TO_STRING (GREATER);
        ENUM_TO_STRING (LESS_OR_EQUAL);
        ENUM_TO_STRING (GREATER_OR_EQUAL);
        ENUM_TO_STRING (NOT_EQUAL);

        default:
        {
            fprintf (stderr, "Unknown keyword enum");
            exit (EXIT_FAILURE);
        }
    }

    #undef ENUM_TO_STRING
}
