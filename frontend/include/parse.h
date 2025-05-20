#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>

#include "ast.h"

// Main parsers

ast_node_t* ParseProgram             (token_array_t* tokens, size_t* shift);
ast_node_t* ParseTopLevel            (token_array_t* tokens, size_t* shift);
ast_node_t* ParseFunctionDefinition  (token_array_t* tokens, size_t* shift);
ast_node_t* ParseVariableAssignment  (token_array_t* tokens, size_t* shift);
ast_node_t* ParseStatements          (token_array_t* tokens, size_t* shift);
ast_node_t* ParseStatement           (token_array_t* tokens, size_t* shift);

// Statement parsers

ast_node_t* ParseIf                  (token_array_t* tokens, size_t* shift);
ast_node_t* ParseWhile               (token_array_t* tokens, size_t* shift);
ast_node_t* ParseFunctionCall        (token_array_t* tokens, size_t* shift);
ast_node_t* ParseReturn              (token_array_t* tokens, size_t* shift);
ast_node_t* ParsePrint               (token_array_t* tokens, size_t* shift);
ast_node_t* ParseInput               (token_array_t* tokens, size_t* shift);

// Expression parsers

ast_node_t* ParseBoolExpression      (token_array_t* tokens, size_t* shift);
ast_node_t* ParseExpression          (token_array_t* tokens, size_t* shift);
ast_node_t* ParseTerm                (token_array_t* tokens, size_t* shift);
ast_node_t* ParsePrimary             (token_array_t* tokens, size_t* shift);
ast_node_t* ParseConstant            (token_array_t* tokens, size_t* shift);

// Support parsers

ast_node_t* ParseAssignment          (token_array_t* tokens, size_t* shift);
ast_node_t* ParseCallArguments       (token_array_t* tokens, size_t* shift);
ast_node_t* ParseDefinitionArguments (token_array_t* tokens, size_t* shift);
ast_node_t* ParseType                (token_array_t* tokens, size_t* shift);
ast_node_t* ParseCompareOperator     (token_array_t* tokens, size_t* shift);
ast_node_t* ParseIdentifier          (token_array_t* tokens, size_t* shift);

#endif // PARSE_H
