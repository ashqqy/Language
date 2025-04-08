#include <stdio.h>
#include <string.h>

#include "Common.h"

#include "Tokenization.h"
#include "Parse.h"

//----------------------------------------------------------------------------

int main ()
{
    FILE* file_with_code = fopen ("./Tests/Factorial.chinazes", "r");
    CustomWarning (file_with_code != NULL, OPENING_FILE_ERROR);

    size_t buf_size = 0;
    char* buf = ReadFile (file_with_code, &buf_size);
    CustomWarning (buf != NULL, READING_FILE_ERROR);
    fclose (file_with_code);

    frontend_t frontend = {};
    CustomWarning (FrontendInit (&frontend) == NO_ERRORS, ALLOCATION_ERROR);

    tree_node_t** token_array = Tokenization (buf, buf_size, &frontend);
    CustomWarning (token_array != NULL, TOKENIZATION_ERROR);

    size_t shift = 0;
    tree_node_t* root_node = GetProgram (token_array, &shift);

    // TreeDump (root_node);

    FILE* output_file = fopen ("./dump/OutputFile.txt", "w");
    TreeOutput (output_file, root_node);
    fclose (output_file);

    free(frontend.names_table); frontend.names_table = NULL;
    TokenArrayDestroy (frontend.token_array);
    free (buf); buf = NULL;
}

//----------------------------------------------------------------------------
