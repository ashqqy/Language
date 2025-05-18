#ifndef PARSE_H
#define PARSE_H

#include <ast.h>

ast_node_t* GetProgram    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetStatement  (ast_node_t** token_array, size_t* shift);
ast_node_t* GetStatements (ast_node_t** token_array, size_t* shift, keyword_t end_token);
ast_node_t* GetPrint      (ast_node_t** token_array, size_t* shift);
ast_node_t* GetIf         (ast_node_t** token_array, size_t* shift);
ast_node_t* GetWhile      (ast_node_t** token_array, size_t* shift);
ast_node_t* GetBool       (ast_node_t** token_array, size_t* shift);
ast_node_t* GetBoolP1     (ast_node_t** token_array, size_t* shift);
ast_node_t* GetCompare    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetVarInit    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetType       (ast_node_t** token_array, size_t* shift);
ast_node_t* GetAsg        (ast_node_t** token_array, size_t* shift);
ast_node_t* GetName       (ast_node_t** token_array, size_t* shift);
ast_node_t* GetNum        (ast_node_t** token_array, size_t* shift);

ast_node_t* GetMath    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetPlus    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetMult    (ast_node_t** token_array, size_t* shift);
ast_node_t* GetDeg     (ast_node_t** token_array, size_t* shift);
ast_node_t* GetBracket (ast_node_t** token_array, size_t* const shift);

#endif // PARSE_H
