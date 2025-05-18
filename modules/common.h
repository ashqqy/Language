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

char*  ReadFile     (FILE* file, size_t* n_readen);
size_t FindFileSize (FILE* file);

void* MyCalloc   (size_t n_elems, size_t size_elems, const void* poison);
void* MyRecalloc (void* memory, size_t n_elements, size_t size_elements, size_t previous_n_elements, const void* poison);

//--------------------------------------------------------------------------

#endif // COMMON_H
