#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "parse.h"
#include "tokenization.h"
#include "tree.h"

//----------------------------------------------------------------------------

int main (const int argc, const char* argv[])
{
    if (argc != 2)
    {
        return EXPECTED_INPUT_FILE;
    }

    FILE* source_code_file = fopen (argv[1], "r");
    CUSTOM_ASSERT (source_code_file != NULL);

    size_t source_code_buffer_size = 0;
    char* source_code_buffer = ReadFile (source_code_file, &source_code_buffer_size);
    fclose (source_code_file);

    frontend_t frontend = {};
    FrontendInit (&frontend);

    tree_node_t** token_array = Tokenization (source_code_buffer, source_code_buffer_size, &frontend);
    CUSTOM_WARNING (token_array != NULL, TOKENIZATION_ERROR);

    size_t shift = 0;
    tree_node_t* ast_root_node = GetProgram (token_array, &shift);

    FILE* dump_file = fopen ("./dump/dump.dot", "w");
    CUSTOM_ASSERT (source_code_file != NULL);
    TreeDump (dump_file, ast_root_node);
    fclose (dump_file);

    FILE* ast_file = fopen ("./dump/AST.txt", "w");
    CUSTOM_ASSERT (ast_file != NULL);
    TreeOutput (ast_file, ast_root_node);
    fclose (ast_file);

    FREE (frontend.names_table);
    TokenArrayDestroy (frontend.token_array);

    FREE (source_code_buffer);
}

//----------------------------------------------------------------------------
