#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include "ast.h"
#include "token.h"

static const int TOKEN_ARRAY_SIZE = 100;
static const int NAMES_TABLE_SIZE = 10;

typedef struct
{
    ast_node_t** token_array;
    identifier_t* names_table;
    size_t names_table_size;
    int n_names;
} frontend_t;

void FrontendInit (frontend_t* frontend);
ast_node_t** Tokenization (char* buf, size_t buf_size, frontend_t* frontend);
ast_node_t** TokenArrayResize (ast_node_t** token_array, size_t* tkn_arr_size);
int FindNameIndex (frontend_t* frontend, identifier_t* name);
identifier_t* NamesArrayResize (frontend_t* frontend, size_t* arr_size);
void TokenArrayDestroy (ast_node_t** token_array);
void SyntaxError (const char* message);

#endif // TOKENIZATION_H
