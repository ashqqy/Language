#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "tokenization.h"

#include "common.h"
#include "ast.h"

//----------------------------------------------------------------------------

int main (const int argc, const char* argv[])
{
    if (argc != 2)
    {
        fprintf (stderr, "Expected source code file\n");
        return EXPECTED_INPUT_FILE;
    }

    size_t source_code_buffer_size = 0;
    char* source_code_buffer = ReadFile (argv[1], &source_code_buffer_size);

    frontend_t frontend = {};
    FrontendInit (&frontend);

    Tokenization (&frontend, source_code_buffer, source_code_buffer_size);
    FREE (source_code_buffer);

    TokenArrayGraphvizDump (frontend.tokens);

    size_t shift = 0;
    ast_node_t* ast_root_node = ParseProgram (&frontend.tokens, &shift);

    AstGraphvizDump (ast_root_node);

    AstSerialize (stdout, ast_root_node);

    FrontendDestroy (&frontend);
}

//----------------------------------------------------------------------------
