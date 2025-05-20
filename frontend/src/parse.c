#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

#include "ast.h"
#include "token.h"
#include "common.h"

//----------------------------------------------------------------------------

#define CURRENT_NODE  tokens->token_array[*shift]
#define CURRENT_TOKEN tokens->token_array[*shift]->token

#define HANDLE_COMPARSION_KEYWORDS(HANDLER_MACRO)   \
    HANDLER_MACRO (EQUAL)                           \
    HANDLER_MACRO (NOT_EQUAL)                       \
    HANDLER_MACRO (LESS_OR_EQUAL)                   \
    HANDLER_MACRO (GREATER_OR_EQUAL)                \
    HANDLER_MACRO (LESS)                            \
    HANDLER_MACRO (GREATER)                         \

//----------------------------------------------------------------------------

// extern void TokenAddExternal (token_array_t* tokens, token_t token);

//----------------------------------------------------------------------------

static void SyntaxError (const char* format, ...)
{
    va_list args;
    
    fprintf (stderr, "Syntax error: ");
    
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    
    fprintf (stderr, "\n");

    exit (EXIT_FAILURE);
}

//----------------------------------------------------------------------------

static ast_node_t* ParseOptionalKeyword (token_array_t* tokens, size_t* shift, keyword_t keyword)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* keyword_node = NULL;

    if ((CURRENT_TOKEN.type == KEYWORD) && (CURRENT_TOKEN.content.keyword == keyword))
    {
        keyword_node = CURRENT_NODE;
        *shift += 1;
    }

    return keyword_node;
}

static ast_node_t* ParseNecessaryKeyword (token_array_t* tokens, size_t* shift, keyword_t keyword)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* keyword_node = ParseOptionalKeyword (tokens, shift, keyword);

    if (keyword_node == NULL)
    {
        const char* keyword_string = KeywordToString (keyword);
        SyntaxError ("%s \"%s\"", "expected", keyword_string);
    }

    return keyword_node;
}
//----------------------------------------------------------------------------
//                               MAIN PARSERS                               //
//----------------------------------------------------------------------------

ast_node_t* ParseProgram (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* root_node = NULL;
    ast_node_t* last_node = NULL;

    while (*shift < tokens->size)
    {
        ast_node_t* next_node = ParseTopLevel (tokens, shift);
        if (next_node == NULL)
        {
            SyntaxError ("%s", "top level definition error");
        }

        if (root_node == NULL)
        {
            root_node = next_node;
        }

        else
        {
            NodeLink (next_node, &last_node->right);
        }

        last_node = next_node;
    }

    return root_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseTopLevel (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* top_level_definition_node = ParseFunctionDefinition (tokens, shift);

    if (top_level_definition_node == NULL)
    {
        top_level_definition_node = ParseVariableAssignment (tokens, shift);
    }

    return top_level_definition_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseFunctionDefinition (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* type_node = ParseType (tokens, shift);
    if (type_node == NULL)
    {
        return NULL;
    }

    ast_node_t* identifier_node = ParseIdentifier (tokens, shift);
    if (identifier_node == NULL)
    {
        SyntaxError ("%s", "expected identifier");
        return NULL;
    }

    ast_node_t* left_bracket_node  = ParseOptionalKeyword  (tokens, shift, LEFT_BRACKET);
    if (left_bracket_node == NULL)
    {
        *shift -= 2;
        return NULL;
    }

    ast_node_t* arguments_node     = ParseDefinitionArguments (tokens, shift);
    ast_node_t* right_bracket_node = ParseNecessaryKeyword    (tokens, shift, RIGHT_BRACKET);

    ast_node_t* block_open_node    = ParseNecessaryKeyword    (tokens, shift, BLOCK_OPEN);
    ast_node_t* statement_node     = ParseStatements          (tokens, shift);
    ast_node_t* block_close_node   = ParseNecessaryKeyword    (tokens, shift, BLOCK_CLOSE);

    ast_node_t* terminator_node            = left_bracket_node;
    terminator_node->token.content.keyword = TERMINATOR;

    ast_node_t* function_definition_node               = right_bracket_node;
    function_definition_node->token.type               = FUNCTION_DEFINITION;
    function_definition_node->token.content.identifier = identifier_node->token.content.identifier;

    ast_node_t* parameters_node          = block_open_node;
    parameters_node->token.type          = PARAMETERS;

    NodeLink (function_definition_node, &terminator_node->left);
    NodeLink (type_node,                &function_definition_node->left);
    NodeLink (parameters_node,          &function_definition_node->right);
    NodeLink (arguments_node,           &parameters_node->left);
    NodeLink (statement_node,           &parameters_node->right);

    return terminator_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseVariableAssignment (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* type_node       = ParseType       (tokens, shift);
    ast_node_t* assignment_node = ParseAssignment (tokens, shift);
    if (assignment_node == NULL)
    {
        if (type_node != NULL)
            *shift -= 1;

        return NULL;
    }

    ast_node_t* terminator_node = ParseNecessaryKeyword  (tokens, shift, TERMINATOR);

    if (type_node != NULL)
    {
        NodeLink (type_node, &terminator_node->left);
        NodeLink (assignment_node, &type_node->left);
    }

    else
    {
        NodeLink (assignment_node, &terminator_node->left);
    }

    return terminator_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseStatements (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* root_node = ParseStatement (tokens, shift);
    ast_node_t* last_node = root_node;

    while (last_node != NULL)
    {
        ast_node_t* next_node = ParseStatement (tokens, shift);
        NodeLink (next_node, &last_node->right);
        last_node = next_node;
    }

    return root_node;
}

ast_node_t* ParseStatement (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* if_node = ParseIf (tokens, shift);
    if (if_node != NULL)
        return if_node;

    ast_node_t* while_node = ParseWhile (tokens, shift);
    if (while_node != NULL)
        return while_node;

    ast_node_t* print_node = ParsePrint (tokens, shift);
    if (print_node != NULL)
        return print_node;

    ast_node_t* var_node = ParseVariableAssignment (tokens, shift);
    return var_node;
}

//----------------------------------------------------------------------------
//                            STATEMENT PARSERS                             //
//----------------------------------------------------------------------------

ast_node_t* ParseIf (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* if_node = ParseOptionalKeyword (tokens, shift, IF);
    if (if_node == NULL)
        return NULL;

    ast_node_t* if_left_bracket_node  = ParseNecessaryKeyword (tokens, shift, LEFT_BRACKET);
    ast_node_t* if_bool_node          = ParseBoolExpression   (tokens, shift);
    ast_node_t* if_right_bracket_node = ParseNecessaryKeyword (tokens, shift, RIGHT_BRACKET);

    ast_node_t* if_block_open_node    = ParseNecessaryKeyword (tokens, shift, BLOCK_OPEN);
    ast_node_t* if_statement_node     = ParseStatements       (tokens, shift);
    ast_node_t* if_block_clode_node   = ParseNecessaryKeyword (tokens, shift, BLOCK_CLOSE);

    ast_node_t* else_node             = ParseOptionalKeyword  (tokens, shift, ELSE);

    if (else_node != NULL)
    {
        ast_node_t* else_block_open_node    = ParseNecessaryKeyword (tokens, shift, BLOCK_OPEN);
        ast_node_t* else_statement_node     = ParseStatements       (tokens, shift);
        ast_node_t* else_block_clode_node   = ParseNecessaryKeyword (tokens, shift, BLOCK_CLOSE);

        NodeLink (if_bool_node, &if_node->left);
        NodeLink (else_node, &if_node->right);
        NodeLink (if_statement_node, &else_node->left);        
        NodeLink (else_statement_node, &else_node->right);        
    }

    else 
    {
        NodeLink (if_bool_node, &if_node->left);
        NodeLink (if_statement_node, &if_node->right);
    }

    // Workaround
    ast_node_t* terminator_node = if_left_bracket_node;
    terminator_node->token.content.keyword = TERMINATOR;
    NodeLink (if_node, &terminator_node->left);

    return terminator_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseWhile (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* while_node = ParseOptionalKeyword (tokens, shift, WHILE);
    if (while_node == NULL)
        return NULL;

    ast_node_t* left_bracket_node  = ParseNecessaryKeyword (tokens, shift, LEFT_BRACKET);
    ast_node_t* bool_node          = ParseBoolExpression   (tokens, shift);
    ast_node_t* right_bracket_node = ParseNecessaryKeyword (tokens, shift, RIGHT_BRACKET);

    ast_node_t* block_open_node    = ParseNecessaryKeyword (tokens, shift, BLOCK_OPEN);
    ast_node_t* statement_node     = ParseStatements       (tokens, shift);
    ast_node_t* block_close_node   = ParseNecessaryKeyword (tokens, shift, BLOCK_CLOSE);

    NodeLink (bool_node, &while_node->left);
    NodeLink (statement_node, &while_node->right);

    // Workaround
    ast_node_t* terminator_node = left_bracket_node;
    terminator_node->token.content.keyword = TERMINATOR;
    NodeLink (while_node, &left_bracket_node->left);

    return terminator_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseFunctionCall (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    return NULL;
    // TODO
}

//----------------------------------------------------------------------------

ast_node_t* ParseReturn (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    return NULL;
    // TODO
}

//----------------------------------------------------------------------------

ast_node_t* ParsePrint (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* print_node = ParseOptionalKeyword (tokens, shift, PRINT);
    if (print_node == NULL)
        return NULL;

    ast_node_t* left_bracket_node   = ParseNecessaryKeyword (tokens, shift, LEFT_BRACKET);
    ast_node_t* expression_node     = ParseExpression       (tokens, shift);
    ast_node_t* right_bracket_node  = ParseNecessaryKeyword (tokens, shift, RIGHT_BRACKET);
    ast_node_t* terminator_node     = ParseNecessaryKeyword (tokens, shift, TERMINATOR);

    NodeLink (expression_node, &print_node->left);
    NodeLink (print_node, &terminator_node->left);

    return terminator_node;
}

ast_node_t* ParseInput (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* input_node = ParseOptionalKeyword (tokens, shift, INPUT);
    if (input_node == NULL)
        return NULL;

    ast_node_t* left_bracket_node   = ParseNecessaryKeyword (tokens, shift, LEFT_BRACKET);
    ast_node_t* identifier_node     = ParseIdentifier       (tokens, shift);
    ast_node_t* right_bracket_node  = ParseNecessaryKeyword (tokens, shift, RIGHT_BRACKET);
    ast_node_t* terminator_node     = ParseNecessaryKeyword (tokens, shift, TERMINATOR);

    NodeLink (identifier_node, &input_node->left);
    NodeLink (input_node, &terminator_node->left);

    return terminator_node;
}

//----------------------------------------------------------------------------
//                            EXPRESSION PARSERS                            //
//----------------------------------------------------------------------------

ast_node_t* ParseBoolExpression (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* left_node    = ParseExpression      (tokens, shift);
    ast_node_t* compare_node = ParseCompareOperator (tokens, shift);

    if (compare_node != NULL)
    {
        ast_node_t* right_node = ParseExpression (tokens, shift);
        
        NodeLink (left_node,  &compare_node->left);
        NodeLink (right_node, &compare_node->right);

        return compare_node;
    }

    return left_node;
}   

//--------------------------------------------------------------------------

ast_node_t* ParseExpression (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* left_node      = ParseTerm (tokens, shift);
    ast_node_t* operation_node = ParseOptionalKeyword (tokens, shift, ADD) ?: 
                                 ParseOptionalKeyword (tokens, shift, SUB);

    if (operation_node != NULL)
    {
        ast_node_t* right_node = ParseExpression (tokens, shift);
        
        NodeLink (left_node,  &operation_node->left);
        NodeLink (right_node, &operation_node->right);

        return operation_node;
    }

    return left_node;
}

ast_node_t* ParseTerm (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* left_node      = ParsePrimary         (tokens, shift);
    ast_node_t* operation_node = ParseOptionalKeyword (tokens, shift, MUL) ?: 
                                 ParseOptionalKeyword (tokens, shift, DIV);

    if (operation_node != NULL)
    {
        ast_node_t* right_node = ParseTerm (tokens, shift);

        NodeLink (left_node,  &operation_node->left);
        NodeLink (right_node, &operation_node->right);
        
        return operation_node;
    }

    return left_node;
}

ast_node_t* ParsePrimary (token_array_t* tokens, size_t* const shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* left_bracket_node = ParseOptionalKeyword (tokens, shift, LEFT_BRACKET);

    if (left_bracket_node != NULL)
    {
        ast_node_t* node = ParseExpression (tokens, shift);
        ast_node_t* right_bracket_node = ParseNecessaryKeyword (tokens, shift, RIGHT_BRACKET);

        return node;
    }

    else if (CURRENT_TOKEN.type == IDENTIFIER)
    {
        return ParseIdentifier (tokens, shift);
    }

    else
    {
        return ParseConstant (tokens, shift);
    }
}

ast_node_t* ParseConstant (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* minus_node = ParseOptionalKeyword (tokens, shift, SUB);
    ast_node_t* constant_node = NULL;

    if (CURRENT_TOKEN.type == CONSTANT)
    {
        constant_node = CURRENT_NODE;
        *shift += 1;
    }
    else
    {
        SyntaxError ("%s", "expected constant");
    }

    if (minus_node != NULL)
    {
        NodeLink (constant_node, &minus_node->left);
        return minus_node;
    }

    return constant_node;
}

//--------------------------------------------------------------------------

ast_node_t* ParseAssignment (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* variable_node   = ParseIdentifier (tokens, shift);
    if (variable_node == NULL)
    {
        return NULL;
    }

    ast_node_t* assignment_node = ParseOptionalKeyword (tokens, shift, ASSIGNMENT);
    if (assignment_node == NULL)
    {
        *shift -= 1;
        return NULL;
    }

    ast_node_t* expression_node = ParseExpression (tokens, shift);

    NodeLink (variable_node, &assignment_node->left);
    NodeLink (expression_node, &assignment_node->right);

    return assignment_node;
}

ast_node_t* ParseCallArguments (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    return NULL;
    // TODO
}

ast_node_t* ParseDefinitionArguments (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    return NULL;
    // TODO
}

ast_node_t* ParseType (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);

    ast_node_t* void_node   = ParseOptionalKeyword (tokens, shift, VOID);
    if (void_node != NULL)
        return void_node;

    ast_node_t* int_node = ParseOptionalKeyword (tokens, shift, INT);
    if (int_node != NULL)
        return int_node;

    ast_node_t* double_node = ParseOptionalKeyword (tokens, shift, DOUBLE);
        return double_node;
}

//----------------------------------------------------------------------------

ast_node_t* ParseCompareOperator (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* comparsion_node = NULL;

    #define COMPARSION_HANDLER(comparsion_keyword)                                   \
        comparsion_node = ParseOptionalKeyword (tokens, shift, comparsion_keyword);  \
        if (comparsion_node != NULL)                                                 \
        {                                                                            \
            return comparsion_node;                                                  \
        }
    
    HANDLE_COMPARSION_KEYWORDS (COMPARSION_HANDLER)

    #undef COMPARSION_HANDLE

    return NULL;
}

//----------------------------------------------------------------------------

ast_node_t* ParseIdentifier (token_array_t* tokens, size_t* shift)
{
    assert (tokens != NULL);
    assert (shift  != NULL);
    assert (*shift < tokens->size);

    ast_node_t* identifier_node = NULL;

    if (CURRENT_TOKEN.type == IDENTIFIER)
    {
        identifier_node = CURRENT_NODE;
        *shift += 1;
    }

    return identifier_node;
}

//--------------------------------------------------------------------------
