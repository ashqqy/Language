#include <assert.h>
#include <stdio.h>

#include "ir-generate.h"
#include "ir.h"

#include "ast.h"
#include "common.h"

#define ENUM_TO_STRING(enum) #enum

void IRGenerate (const char* ir_file_name, ast_node_t* ast_root)
{
    assert (ast_root != NULL);

    system("mkdir -p ./tmp");

    FILE* ir_file = fopen (IR_FILE_NAME, "r");
    CUSTOM_ASSERT (ir_file != NULL);

    fprintf (ir_file, "CALL 0\n");
    fprintf (ir_file, "CALL -3\n");

    ParseAstNode (ir_file, ast_root);

    fclose (ir_file);
}

void ParseAstNode (FILE* ir_file, ast_node_t* current_node)
{
    if (current_node == NULL)
        return;

    switch (current_node->token.type)
    {
        case CONSTANT:
        {
            int constant = current_node->token.content.constant;
            fprintf (ir_file, "MOV stack %d\n", constant);

            break;
        }

        case IDENTIFIER:
        {
            int identifier_index = current_node->token.content.identifier.index;
            fprintf (ir_file, "MOV stack [%d]\n", identifier_index);

            break;
        }

        case KEYWORD:
        {
            switch (current_node->token.content.keyword)
            {
                case TERMINATOR:
                {
                    ParseAstNode (ir_file, current_node->left);
                    ParseAstNode (ir_file, current_node->right);
                    
                    break;
                }

                case INT:
                case DOUBLE:
                {
                    ParseAstNode (ir_file, current_node->left);

                    break;
                }

                case ASSIGNMENT:
                {
                    int variable_identifier = current_node->left->token.content.identifier.index;
                    ParseAstNode (ir_file, current_node->right);
                    if (current_node->right->token.type == CALL)
                        fprintf (ir_file, "MOV stack AX\n");

                    fprintf (ir_file, "MOV [%d] stack\n", variable_identifier);

                    break;
                }

                case ADD:
                case SUB:
                case MUL:
                case DIV:
                {
                    ParseAstNode (ir_file, current_node->left);
                    if (current_node->left->token.type == CALL)
                        fprintf (ir_file, "MOV stack AX\n");

                    ParseAstNode (ir_file, current_node->right);
                    if (current_node->right->token.type == CALL)
                        fprintf (ir_file, "MOV stack AX\n");
                    
                    const char* operation_name = KeywordEnumToString (current_node->token.content.keyword);
                    fprintf (ir_file, "MATH %s\n", operation_name);
                    break;
                }

                case RETURN:
                {
                    ast_node_t* return_value_node = current_node->left;
                    if (return_value_node != NULL)
                    {
                        ParseAstNode (ir_file, return_value_node);
                        fprintf (ir_file, "MOV AX stack\n");
                    }

                    fprintf (ir_file, "RET\n");

                    break;
                }

                case INPUT:
                {
                    int function_index = INPUT_FUNCTION_IDENTIFIER;
                    ast_node_t* function_arguments_node = current_node->left;
                    
                    // push arg on stack
                    if (function_arguments_node != NULL)
                        ParseAstNode (ir_file, function_arguments_node);

                    fprintf (ir_file, "CALL %d\n", function_index);

                    // delete arg from stack
                    fprintf (ir_file, "CALL_ARG_DTR\n");

                    break;
                }

                case PRINT:
                {
                    int function_index = PRINT_FUNCTION_IDENTIFIER;
                    ast_node_t* function_arguments_node = current_node->left;
                    
                    // push arg on stack
                    if (function_arguments_node != NULL)
                        ParseAstNode (ir_file, function_arguments_node);

                    fprintf (ir_file, "CALL %d\n", function_index);

                    // delete arg from stack
                    fprintf (ir_file, "CALL_ARG_DTR\n");

                    break;
                }

                /*
                jmp_cond if_body
                jmp      if_end
                if_body:
                ...
                if_end:
                */
                case IF:
                {
                    void* if_body_label = (void*) current_node->left;
                    void* if_end_label  = (void*) current_node;

                    GenerateConditionalJump (ir_file, current_node->left);

                    fprintf (ir_file, "JUMP JMP %p\n", if_end_label);
                    fprintf (ir_file, "LABEL %p\n", if_body_label);

                    ParseAstNode (ir_file, current_node->right);

                    fprintf (ir_file, "LABEL %p\n", if_end_label);
                    
                    break;
                }
            }

            break;
        }

        case FUNCTION_DEFINITION:
        {
            int function_identifier = current_node->token.content.identifier.index;
            keyword_t function_return_type_keyword = current_node->left->token.content.keyword;
            // const char* function_return_type_string = KeywordToString (function_return_type_keyword);

            fprintf (ir_file, "FUNCTION_DEFINITION %d, 0\n", function_identifier);
            
            ParseAstNode (ir_file, current_node->right);

            if (function_return_type_keyword == VOID)
                fprintf (ir_file, "RET\n");
            
            break;
        }

        case PARAMETERS:
        {
            if (current_node->left != NULL)
            {
                int function_parameter_index = current_node->left->left->token.content.identifier.index;
                fprintf (ir_file, "MOV stack [%d]\n", function_parameter_index);
            }

            ParseAstNode (ir_file, current_node->right);

            break;
        }

        case VARIABLE_DECLARATION:
        {
            fprintf (stderr, "Error: VARIABLE DECLARATION found\n");
            exit (EXIT_FAILURE);
        }

        case CALL:
        {
            int function_index = current_node->token.content.identifier.index;
            ast_node_t* function_arguments_node = current_node->left;
            
            // push arg on stack
            if (function_arguments_node != NULL)
                ParseAstNode (ir_file, function_arguments_node);

            fprintf (ir_file, "CALL %d\n", function_index);

            // delete arg from stack
            fprintf (ir_file, "CALL_ARG_DTR\n");

            break;
        }
    }
}

void GenerateConditionalJump (FILE* ir_file, ast_node_t* node)
{
    assert (ir_file != NULL);
    assert (node != NULL);
    assert (node->token.type == KEYWORD);

    ParseAstNode (ir_file, node->left);
    ParseAstNode (ir_file, node->right);
    fprintf (ir_file, "CMP\n");
    fprintf (ir_file, "CONDITION_ARG_DTR\n");

    void* body_label = (void*) node;
    const char* jump_type = KeywordEnumToString (node->token.content.keyword);

    fprintf (ir_file, "JUMP %s %p\n", jump_type, body_label);
}
