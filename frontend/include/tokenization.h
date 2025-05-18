#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "tree.h"

static const int TOKEN_ARRAY_SIZE = 100;
static const int NAMES_TABLE_SIZE = 10;

typedef struct
{
    tree_node_t** token_array;
    name_t* names_table;
    size_t names_table_size;
    int n_names;
} frontend_t;

void FrontendInit (frontend_t* frontend);
tree_node_t** Tokenization (char* buf, size_t buf_size, frontend_t* frontend);
tree_node_t** TokenArrayResize (tree_node_t** token_array, size_t* tkn_arr_size);
int FindNameIndex (frontend_t* frontend, name_t* name);
name_t* NamesArrayResize (frontend_t* frontend, size_t* arr_size);
void TokenArrayDestroy (tree_node_t** token_array);
void SyntaxError (const char* message);

#endif // TOKENIZATION_H
