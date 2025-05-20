#include <assert.h>
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
