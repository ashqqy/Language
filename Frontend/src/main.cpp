#include <stdio.h>
#include <string.h>

#include "FilesReading.h"
#include "Common.h"
#include "CustomAssert.h"

#include "Tokenization.h"
#include "Parce.h"

//----------------------------------------------------------------------------

int main ()
{
    FILE* file_with_code = fopen ("./Tests/Assignment.sex", "r");
    CustomWarning (file_with_code != NULL, OPENING_FILE_ERROR);

    size_t buf_size = 0;
    char* buf = ReadFile (file_with_code, &buf_size);
    CustomWarning (buf != NULL, READING_FILE_ERROR);
    fclose (file_with_code);

    tree_node_t** token_array = Tokenization (buf, buf_size);

    size_t shift = 0;
    tree_node_t* root_node = GetProgram (token_array, &shift);

    TreeDump (root_node);

    TokenArrayDestroy (token_array);
    free (buf); buf = NULL;
}

//----------------------------------------------------------------------------
