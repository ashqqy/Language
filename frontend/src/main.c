#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "tokenization.h"

#include "common.h"
#include "ast.h"
#include "token.h"

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

    Tokenization (&frontend, source_code_buffer, source_code_buffer_size);

    // TokenArrayGraphvizDump (frontend.tokens);

    FREE (source_code_buffer);

    size_t shift = 0;
    ast_node_t* ast_root_node = ParseProgram (&frontend.tokens, &shift);

    // AstGraphvizDump (ast_root_node);

    AstSerialize (stdout, ast_root_node);

    FrontendDestroy (&frontend);
}

//----------------------------------------------------------------------------
