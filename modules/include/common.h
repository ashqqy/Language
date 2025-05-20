#ifndef COMMON_H
#define COMMON_H

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

//--------------------------------------------------------------------------

#define CUSTOM_WARNING(expression, return_message) do                                                             \
{                                                                                                                 \
    if (!(expression))                                                                                            \
    {                                                                                                             \
        fprintf (stderr, "%s: %s:%d: Expression `%s' warning.\n", __FUNCTION__, __FILE__, __LINE__, #expression); \
        return return_message;                                                                                    \
    }                                                                                                             \
}                                                                                                                 \
while (0)                                                                                                         \

#define CUSTOM_ASSERT(expression) do                                                                              \
{                                                                                                                 \
    if (!(expression))                                                                                            \
    {                                                                                                             \
        fprintf (stderr, "%s: %s:%d: Assertion `%s' failed.\n", __FUNCTION__, __FILE__, __LINE__, #expression);   \
        fprintf (stderr, "Program aborting\n");                                                                   \
        exit (EXIT_FAILURE);                                                                                      \
    }                                                                                                             \
}                                                                                                                 \
while (0)  

#define FREE(ptr) free (ptr); ptr = NULL;

//--------------------------------------------------------------------------

char* ReadFile (const char* file_name, size_t* buffer_size);
char* ReadOpenedFile (FILE* file, size_t* buffer_size);
size_t FindFileSize (FILE* file);

void* MyRecalloc (void* memory, size_t new_capacity, size_t prev_capacity, size_t elem_size);

//--------------------------------------------------------------------------

#endif // COMMON_H
