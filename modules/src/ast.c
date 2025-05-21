#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

#include "token.h"
#include "common.h"

//------------------------------------------------------

#define ENUM_TO_STRING(enum) #enum

//------------------------------------------------------

static void AstNodesGraphvizDump (FILE* dump_file, ast_node_t* node);
static void AstNodesGraphvizLink (FILE* dump_file, ast_node_t* node);

static ast_node_t* DeserializeNodes (char* buffer, int* shift);

//------------------------------------------------------

ast_node_t* AstNodeCreate (token_t data, ast_node_t* left_node, ast_node_t* right_node)
{
    ast_node_t* node = (ast_node_t*) calloc (1, sizeof (ast_node_t));
    CUSTOM_ASSERT (node != NULL);

    node->token  = data;
    node->left  = left_node;
    node->right = right_node;

    return node;
}
//------------------------------------------------------

void AstNodeLink (ast_node_t* node, ast_node_t** node_to_link_to)
{
    assert (node_to_link_to != NULL);

    *node_to_link_to = node;
}

//------------------------------------------------------

void AstDestroy (ast_node_t* node)
{
    if (node == NULL)
        return;

    AstDestroy (node->left);
    AstDestroy (node->right);

    FREE (node);
}

//------------------------------------------------------

void AstSerialize (FILE* output_file, ast_node_t* node)
{
    assert (output_file != NULL);

    if (node == NULL)
    {
        fprintf (output_file, "_ ");
        return;
    }

    fprintf (output_file, "( ");
    fprintf (output_file, "%d ", node->token.type);

    switch (node->token.type)
    {
        case CONSTANT:
        {
            fprintf (output_file, "%lg ", node->token.content.constant);
            break;
        }

        case IDENTIFIER:
        {
            fprintf (output_file, "%d ", node->token.content.identifier.index);
            break;
        }

        case KEYWORD:
        {
            fprintf (output_file, "%d ", node->token.content.keyword);
            break;
        }

        case FUNCTION_DEFINITION:
        case VARIABLE_DECLARATION:
        {
            fprintf (output_file, "%d ", node->token.content.identifier.index);
            break;
        }

        case PARAMETERS:
        case CALL:
        {
            break;
        }

        default:
        {
            fprintf (stderr, "Switch case expected\n");
            exit (EXIT_FAILURE);
        }
    }

    AstSerialize (output_file, node->left);
    AstSerialize (output_file, node->right);

    fprintf (output_file, ") ");
}

ast_node_t* AstDeserialize (const char* file_name)
{
    assert (file_name != NULL);

    size_t buffer_size = 0;
    char* buffer = ReadFile (file_name, &buffer_size);
    CUSTOM_ASSERT (buffer != NULL && "AST reading error");

    int shift = 0;
    ast_node_t* root_node = DeserializeNodes (buffer, &shift);
    CUSTOM_ASSERT (root_node != NULL && "AST deserializing error");

    FREE (buffer);

    return root_node;
}

static ast_node_t* DeserializeNodes (char* buffer, int* shift)
{
    assert (buffer != NULL);
    assert (shift != NULL);

    token_t node_data = {};

    if (buffer[*shift] == '(')
    {
        *shift += 2;

        int token_type = 0;
        int token_type_len = 0;
        sscanf (buffer + *shift, "%d%n", &token_type, &token_type_len);

        node_data.type = (token_type_t) token_type;
        *shift += token_type_len + 1;

        switch (node_data.type)
        {
            case KEYWORD:
            {
                int keyword = 0;
                int keyword_len = 0;
                sscanf (buffer + *shift, "%d%n", &keyword, &keyword_len);

                node_data.content.keyword = (keyword_t) keyword;
                *shift += keyword_len + 1;

                break;
            }

            case IDENTIFIER:
            case FUNCTION_DEFINITION:
            case VARIABLE_DECLARATION:
            {
                int identifier_index = -1;
                int identifier_len = 0;
                sscanf (buffer + *shift, "%d%n", &identifier_index, &identifier_len);

                node_data.content.identifier.index = identifier_index;
                *shift += identifier_len + 1;

                break;
            }

            case CONSTANT:
            {
                double constant = 0;
                int constant_len = 0;
                sscanf (buffer + *shift, "%lf%n", &constant, &constant_len);

                node_data.content.constant = constant;
                *shift += constant_len + 1;

                break;
            }

            case PARAMETERS:
            case CALL:
            {
                break;
            }

            default:
            {
                assert ("Switch case expected\n" && 0);
            }
        }

        ast_node_t* left_node  = DeserializeNodes (buffer, shift);
        ast_node_t* right_node = DeserializeNodes (buffer, shift);

        if (buffer[*shift] == ')')
        {
            *shift += 2;
            return AstNodeCreate (node_data, left_node, right_node);
        }

        else
        {
            CUSTOM_WARNING ("AST deserializing error: Expected close bracket" && 0, NULL);
        }
    }

    else if (buffer[*shift] == '_')
    {
        *shift += 2;
        return NULL;
    }

    else
    {
        CUSTOM_WARNING ("AST deserializing error: Expected node" && 0, NULL);
    }
}

//------------------------------------------------------

void AstGraphvizDump (ast_node_t* root_node)
{
    assert (root_node != NULL);

    const char* dot_file_name = "./dump/ast.dot";
    const char* png_file_name = "./dump/ast.png";

    system("mkdir -p ./dump");

    FILE* dot_file = fopen (dot_file_name, "w");
    CUSTOM_ASSERT (dot_file != NULL);

    fprintf (dot_file, "digraph G\n");
    fprintf (dot_file, "{\n");
    fprintf (dot_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    AstNodesGraphvizDump (dot_file, root_node);
    fprintf (dot_file, "\n");

    AstNodesGraphvizLink (dot_file, root_node);
    fprintf (dot_file, "}\n");

    fclose (dot_file);

    char command[256] = {};
    snprintf (command, sizeof (command), "dot %s -Tpng -o %s", dot_file_name, png_file_name);
    
    if (system(command) != 0)
    {
        fprintf(stderr, "Failed to generate PNG from DOT file\n");
    }
}

void TokenArrayGraphvizDump (token_array_t tokens)
{
    const char* dot_file_name = "./dump/token_array.dot";
    const char* png_file_name = "./dump/token_array.png";
    
    system("mkdir -p ./dump");

    FILE* dot_file = fopen (dot_file_name, "w");
    CUSTOM_ASSERT (dot_file != NULL);

    fprintf (dot_file, "digraph G\n");
    fprintf (dot_file, "{\n");
    fprintf (dot_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    for (size_t i = 0; i < tokens.size; ++i)
    {
        AstNodesGraphvizDump (dot_file, tokens.token_array[i]);
    }

    fprintf (dot_file, "}\n");

    fclose (dot_file);

    char command[256] = {};
    snprintf (command, sizeof (command), "dot %s -Tpng -o %s", dot_file_name, png_file_name);
    
    if (system(command) != 0)
    {
        fprintf(stderr, "Failed to generate PNG from DOT file\n");
    }
}

//------------------------------------------------------

static void AstNodesGraphvizDump (FILE* dump_file, ast_node_t* node)
{
    assert (dump_file != NULL);

    if (node == NULL)
        return;

    switch (node->token.type)
    {
        case CONSTANT: 
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %lg | { <l>left|<r>right } }\"];\n", 
                    node, node, TypeToString (node->token.type), node->token.content.constant);
            break;
        }

        case IDENTIFIER:
        case FUNCTION_DEFINITION:
        case VARIABLE_DECLARATION:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %d | { <l>left|<r>right } }\"];\n", 
                    node, node, TypeToString (node->token.type), node->token.content.identifier.index);
            break;
        }

        case KEYWORD:
        {
            const char* keyword_string = KeywordToString (node->token.content.keyword);

            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> \\%s | { <l>left|<r>right } }\"];\n", 
                    node, node, TypeToString (node->token.type), keyword_string);
            break;
        }

        case PARAMETERS:
        case CALL:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | { <l>left|<r>right } }\"];\n", 
                    node, node, TypeToString (node->token.type));
            break;
        }

        default:
        {
            fprintf (stderr, "Switch case expected\n");
            exit (EXIT_FAILURE);
        }
    }
    
    AstNodesGraphvizDump (dump_file, node->left);
    AstNodesGraphvizDump (dump_file, node->right);
}

static void AstNodesGraphvizLink (FILE* dump_file, ast_node_t* node)
{   
    assert (dump_file != NULL);
    assert (node != NULL);

    if (node->left  != NULL) 
    {
        fprintf (dump_file, "p%p:<l> -> p%p\n", node, node->left);
        AstNodesGraphvizLink (dump_file, node->left);
    }
    if (node->right != NULL) 
    {
        fprintf (dump_file, "p%p:<r> -> p%p\n", node, node->right);
        AstNodesGraphvizLink (dump_file, node->right);
    }
}

//------------------------------------------------------
