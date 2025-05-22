#include <stdio.h>
#include <string.h>

#include "ir.h"
#include "ir-generate.h"
#include "ir-parse.h"

#include "common.h"
#include "ast.h"

//----------------------------------------------------------------------------

int main (const int argc, const char* argv[])
{
    if (argc < 3)
    {
        fprintf (stderr, "Expected output file\n");

        if (argc < 2)
            fprintf (stderr, "Expected AST file\n");

        return EXPECTED_INPUT_FILE;
    }

    ast_node_t* ast_root_node = AstDeserialize (argv[1]);
    // AstGraphvizDump (ast_root_node);

    IRGenerate (IR_FILE_NAME, ast_root_node);
    AstDestroy (ast_root_node);

    ir_context_t ir_context = {};
    IRContextInit (&ir_context, argv[2]);

    IRParse (&ir_context);

    // FILE* dump_file = fopen ("./tmp/ir.dump", "w");
    // ListDump (dump_file, &ir_context.ir_list);

    IRContextDestroy (&ir_context);
}

//----------------------------------------------------------------------------
