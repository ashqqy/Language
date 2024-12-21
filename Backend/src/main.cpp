#include <stdio.h>
#include <string.h>

#include "FilesReading.h"
#include "Common.h"
#include "Tree.h"
#include "CustomAssert.h"
#include "Assembling.h"

//----------------------------------------------------------------------------

int main ()
{
    FILE* file_with_tree = fopen ("./Dump/OutputFile.txt", "r");
    CustomWarning (file_with_tree != NULL, OPENING_FILE_ERROR);

    tree_node_t* root_node = TreeInput (file_with_tree);
    CustomWarning (root_node != NULL, READING_FILE_ERROR);

    TreeDump (root_node);

    FILE* asm_file = fopen ("./asm/program.asm", "w");
    CustomWarning (asm_file != NULL, OPENING_FILE_ERROR);
    int label_num = 0;
    TreeToAsm (asm_file, root_node, &label_num);

    TreeDestroy (root_node); root_node = NULL;
}

//----------------------------------------------------------------------------
