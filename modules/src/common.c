#include <assert.h>
#include <string.h>

#include "common.h"

//--------------------------------------------------------------------------

char* ReadFile (FILE* file, size_t* n_readen)
{
    assert (file != NULL);
    assert (n_readen != NULL);

    size_t file_size = FindFileSize (file);
    if (file_size == 0)
        return NULL;

    char* buffer = (char*) calloc (file_size + 1, sizeof (char));
    if (buffer == NULL)
        return NULL;

    *n_readen = fread (buffer, sizeof (char), file_size, file);
    if (*n_readen != file_size)
        return NULL;

    return buffer;
}

size_t FindFileSize (FILE* file)
{
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
