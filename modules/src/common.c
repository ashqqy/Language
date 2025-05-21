#include <assert.h>
#include <string.h>

#include "common.h"

//--------------------------------------------------------------------------

char* ReadFile (const char* file_name, size_t* buffer_size)
{
    assert (file_name != NULL);
    assert (buffer_size != NULL);

    FILE* file = fopen (file_name, "r");
    if (file == NULL)
        return NULL;

    char* buffer = ReadOpenedFile (file, buffer_size);

    fclose (file);

    return buffer;
}

char* ReadOpenedFile (FILE* file, size_t* buffer_size)
{
    assert (file != NULL);
    assert (buffer_size != NULL);

    size_t file_size = FindFileSize (file);
    if (file_size == 0)
        return NULL;

    char* buffer = (char*) calloc (file_size + 1, sizeof (char));
    if (buffer == NULL)
        return NULL;

    *buffer_size = fread (buffer, sizeof (char), file_size, file);
    if (*buffer_size != file_size)
        return NULL;


    return buffer;
}

size_t FindFileSize (FILE* file)
{
    assert (file != NULL);

    fseek (file, 0L, SEEK_END);
    size_t size_file = (size_t) ftell (file);
    fseek (file, 0L, SEEK_SET);

    return size_file;
}

//--------------------------------------------------------------------------

void* MyRecalloc (void* memory, size_t new_capacity, size_t prev_capacity, size_t elem_size)
{
    if (memory == NULL)
    {
        return calloc (new_capacity, elem_size);
    }

    if (prev_capacity > new_capacity)
    {
        memset ((char*) memory + new_capacity * elem_size, 0, elem_size * (prev_capacity - new_capacity));
    }

    void* save_memory = memory;
    memory = realloc (memory, new_capacity * elem_size);
    if (memory == NULL)
    {
        FREE (save_memory);
        return NULL;
    }

    if (prev_capacity < new_capacity)
    {
        memset((char*) memory + prev_capacity * elem_size, 0, elem_size * (new_capacity - prev_capacity));
    }

    return memory;
}

//--------------------------------------------------------------------------
