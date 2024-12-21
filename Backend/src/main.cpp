#include <stdio.h>
#include <string.h>

#include "FilesReading.h"
#include "Common.h"
#include "Tree.h"
#include "CustomAssert.h"

//----------------------------------------------------------------------------

int main ()
{
    FILE* file_with_tree = fopen ("./Dump/OutputFile.txt", "r");
    CustomWarning (file_with_tree != NULL, OPENING_FILE_ERROR);

    tree_node_t* root_node = ReadDataBase (file_with_tree);
    CustomWarning (root_node != NULL, READING_FILE_ERROR);


}

//----------------------------------------------------------------------------