#include <stdio.h>
#include <string.h>

#include "Common.h"
#include "Tree.h"
#include "Assembling.h"

//----------------------------------------------------------------------------

int main ()
{
    FILE* ast_file = fopen ("./tmp/serialized.ast", "r");
    CustomWarning (ast_file != NULL, OPENING_FILE_ERROR);

    tree_node_t* root_node = AstDeseialize (ast_file);
    CustomWarning (root_node != NULL, READING_FILE_ERROR);

    TreeDump (root_node);

    FILE* asm_file = fopen ("./tmp/program.asm", "w");
    CustomWarning (asm_file != NULL, OPENING_FILE_ERROR);
    int label_num = 0;
    TreeToAsm (asm_file, root_node, &label_num);

    TreeDestroy (root_node); root_node = NULL;
}

//----------------------------------------------------------------------------
