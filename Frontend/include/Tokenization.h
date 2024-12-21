#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "Tree.h"

const int TOKEN_ARRAY_SIZE = 100;
const int NAMES_TABLE_SIZE = 10;

struct frontend_t
{
    tree_node_t** token_array;
    name_t* names_table;
    size_t names_table_size;
    int n_names;
};

common_errors FrontendInit (frontend_t* frontend);
tree_node_t** Tokenization (char* buf, size_t buf_size, frontend_t* frontend);
tree_node_t** TokenArrayResize (tree_node_t** token_array, size_t* tkn_arr_size);
int FindNameIndex (frontend_t* frontend, name_t* name);
name_t* NamesArrayResize (frontend_t* frontend, size_t* arr_size);
void TokenArrayDestroy (tree_node_t** token_array);
[[noreturn]] void SyntaxError (const char* message);

#endif // TOKENIZATION_H