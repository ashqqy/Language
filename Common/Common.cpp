#include <string.h>

#include "Common.h"

//--------------------------------------------------------------------------

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

//--------------------------------------------------------------------------

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

//--------------------------------------------------------------------------

char* ReadFile (FILE* file_input, size_t* n_readen)
{
    if (file_input == NULL || n_readen == NULL)
        return NULL;

    size_t file_size = FileSizeFinder (file_input);
    if (file_size == 0)
        return NULL;

    char* buffer = (char*) calloc (file_size + 1, sizeof (char));
    if (buffer == NULL)
        return NULL;

    *n_readen = fread (buffer, sizeof (char), file_size, file_input);
    if (*n_readen != file_size)
        return NULL;

    return buffer;
}

//--------------------------------------------------------------------------

size_t FileSizeFinder (FILE* file)
{
    fseek (file, 0L, SEEK_END); //лучше fstat
    size_t size_file = (size_t) ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size_file;
}

//--------------------------------------------------------------------------

void* MyCalloc (size_t n_elems, size_t size_elems, const void* poison)
    {
    void* mem_ptr = calloc (n_elems, size_elems);
    for (size_t i = 0; i < n_elems; i++)
        memcpy ((char*) mem_ptr + i * size_elems, poison, size_elems);
    return mem_ptr;
    }

//--------------------------------------------------------------------------

void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, const void* poison)
    {
    /// Если уменьшаем блок памяти, то удаленные ячейки обнуляем
    if (previous_n_elements > n_elements)
        memset ((char*) memory + n_elements * size_elements, 0, size_elements * (previous_n_elements - n_elements));

    void* save_memory = memory;
    memory = realloc (memory, n_elements * size_elements);
    /// Если память не удалось перевыделить, то освобождаем старый указатель и возвращаем 0
    if (memory == NULL)
        {
        free (save_memory); save_memory = NULL;
        return NULL;
        }

    /// Если увеличиваем блок памяти, то новые ячейки заполняем ядовитым значением
    if (previous_n_elements < n_elements)
        for (size_t i = 0; i < n_elements - previous_n_elements; i++)
            memcpy(((char*) memory + (previous_n_elements + i) * size_elements), poison, size_elements);

    return memory;
    }

//--------------------------------------------------------------------------
