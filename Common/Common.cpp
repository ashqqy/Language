#include <string.h>

#include <CustomAssert.h>
#include "Common.h"

int FindReservedDataByName (const char* name_begin, size_t name_len, tree_data_t* data)
{
    CustomAssert (name_begin != NULL);
    CustomAssert (data != NULL);

    for (size_t i = 0; i < sizeof (reserved_names) / sizeof (reserved_names[0]); ++i)
    {
        if (strncmp (name_begin, reserved_names[i].name, name_len) == 0)
        {
            *data = reserved_names[i].data;
            return 0;
        }
    }

    return -1;
}

const char* FindReservedNameByData (tree_data_t data)
{
    #define IFCASE_(data_type, content_type)                                    \
    if ((data.type == reserved_names[i].data.type && data.type == data_type &&  \
    data.content.content_type == reserved_names[i].data.content.content_type))  \
        return reserved_names[i].name

    for (size_t i = 0; i < sizeof (reserved_names) / sizeof (reserved_names[0]); ++i)
    {
        IFCASE_ (RESERVED, reserved);
    }

    return "Not found";

    #undef IFCASE_
}