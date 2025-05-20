#ifndef AST_H
#define AST_H

#include <stdio.h>

#include "token.h"
#include "common.h"
    
//--------------------------------------------------------------------------

typedef struct ast_node    ast_node_t;
typedef struct token_array token_array_t;

struct ast_node
{
    token_t     token;
    ast_node_t* left;
    ast_node_t* right;
};

struct token_array
{
    ast_node_t** token_array;
    size_t size;
    size_t capacity;
};

//--------------------------------------------------------------------------

ast_node_t* NodeCreate  (token_t token, ast_node_t* left_node, ast_node_t* right_node);
void        NodeLink    (ast_node_t* node, ast_node_t** node_to_link_to);
void        TreeDestroy (ast_node_t* node);

void        AstSerialize   (FILE* output_file, ast_node_t* node);
ast_node_t* AstDeserialize (FILE* input_file);

void AstGraphvizDump        (ast_node_t* root_node);
void TokenArrayGraphvizDump (token_array_t tokens);

//--------------------------------------------------------------------------

#endif //AST_H
