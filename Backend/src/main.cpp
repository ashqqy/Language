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


tree_node_t* ReadDataBase (FILE* database)
{
    CustomAssert (database != NULL);

    size_t buf_size = 0;
    char* buf = ReadFile (database, &buf_size);
    CustomWarning (buf != NULL, NULL);
    fclose (database);

    size_t shift = 0;
    
    while (shift < buf_size)
        ReadNode (buf, &shift);

    // return root_node;
}

tree_node_t* ReadNode (char* buf, size_t* shift)
{

}