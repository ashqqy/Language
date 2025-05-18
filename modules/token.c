#include <stdlib.h>

#include "token.h"

//--------------------------------------------------------------------------

int FindKeywordByName (const char* name_begin, size_t name_len, token_t* data)
{
    assert (name_begin != NULL);
    assert (data != NULL);

    for (size_t i = 0; i < sizeof (keyword_mappings) / sizeof (keyword_mappings[0]); ++i)
    {
        if (strncmp (name_begin, keyword_mappings[i].string, name_len) == 0)
        {
            data->token_type = KEYWORD;
            data->content.keyword = keyword_mappings[i].keyword;
            return 0;
        }
    }

    return -1;
}

//--------------------------------------------------------------------------

const char* FindNameByKeyword (token_t data)
{
    if (data.token_type != KEYWORD)
    {
        return NULL;
    }

    for (size_t i = 0; i < sizeof (keyword_mappings) / sizeof (keyword_mappings[0]); ++i)
    {
        if (data.content.keyword == keyword_mappings[i].keyword)
        {
            return keyword_mappings[i].string;
        }
    }

    return NULL;
}
