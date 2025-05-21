#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "tokenization.h"

#include "ast.h"
#include "token.h"
#include "common.h"

//--------------------------------------------------------------------------

static const size_t TOKEN_ARRAY_INITIAL_CAPACITY = 100;
static const size_t NAME_TABLE_INITIAL_CAPACITY  = 10;

static const size_t INCREASE_FACTOR = 2;

//--------------------------------------------------------------------------

extern void SyntaxError (const char* format, ...);

static void TokenAdd     (token_array_t* tokens, token_t token);
static void NameTableAdd (name_table_t* identifiers, identifier_t* identifier);

static void TokenArrayResize (token_array_t* tokens);
static void NameTableResize  (name_table_t* identifiers);

static void TokenArrayDestroy (token_array_t* tokens);
static void NameTableDestroy  (name_table_t* identifiers);

//--------------------------------------------------------------------------

void FrontendInit (frontend_t* frontend)
{
    assert (frontend != NULL);

    frontend->tokens.token_array = (ast_node_t**) calloc (TOKEN_ARRAY_INITIAL_CAPACITY, sizeof (ast_node_t*));
    CUSTOM_ASSERT (frontend->tokens.token_array != NULL);

    frontend->tokens.size = 0;
    frontend->tokens.capacity = TOKEN_ARRAY_INITIAL_CAPACITY;    

    frontend->identifiers.name_table = (identifier_t*) calloc (NAME_TABLE_INITIAL_CAPACITY, sizeof (identifier_t));
    CUSTOM_ASSERT (frontend->identifiers.name_table != NULL);

    frontend->identifiers.size = 0;
    frontend->identifiers.capacity = NAME_TABLE_INITIAL_CAPACITY;
}

void FrontendDestroy (frontend_t* frontend)
{
    assert (frontend != NULL);

    TokenArrayDestroy (&frontend->tokens);
    NameTableDestroy  (&frontend->identifiers);
}

//--------------------------------------------------------------------------

void Tokenization (frontend_t* frontend, char* buffer, size_t buffer_size)
{
    assert (buffer != NULL);

    size_t buf_shift = 0;

    while (buf_shift < buffer_size)
    {
        // Read constant
        if (isdigit (buffer[buf_shift]))
        {
            token_t token = {.type = CONSTANT};
            double constant = 0;
            size_t constant_length = 0;

            sscanf (buffer + buf_shift, "%lf%n", &constant, (int*) &constant_length);
            token.content.constant = constant;

            TokenAdd (&frontend->tokens, token);

            buf_shift += constant_length;
        }

        // Read keyword (made up of letters) or identifier
        else if (isalpha (buffer[buf_shift]))
        {
            token_t token = {};
            size_t name_len = 0;

            sscanf (buffer + buf_shift, "%*[a-zA-Z]%n", (int*) &name_len);

            keyword_t keyword = StringToKeyword (buffer + buf_shift, name_len);

            if (keyword != UNKNOWN_KEYWORD)
            {
                token.type = KEYWORD;
                token.content.keyword = keyword;
            }

            else 
            {
                identifier_t identifier = {.begin = buffer + buf_shift, .length = name_len};
                NameTableAdd (&frontend->identifiers, &identifier);

                token.type = IDENTIFIER;
                token.content.identifier = identifier;
            }

            TokenAdd (&frontend->tokens, token);
            buf_shift += name_len;
        }

        // Skip comments
        else if (buffer[buf_shift] == '#')
        {
            char* end_of_line = strchr (buffer + buf_shift, '\n');
            if (end_of_line == NULL)
            {
                *(buffer + buf_shift) = '\n';
            }

            else
            {
                buf_shift = (size_t) (end_of_line - buffer);
            }
        }

        // Skip spaces
        else if (isspace (buffer[buf_shift]))
        {
            buf_shift += 1;
        }

        // Read keyword (made up of special characters)
        else
        {
            token_t token = {.type = KEYWORD};

            // First try to match a 2-byte keyword combination
            token.content.keyword = StringToKeyword (buffer + buf_shift, 2);

            if (token.content.keyword != UNKNOWN_KEYWORD)
            {
                TokenAdd (&frontend->tokens, token);
                buf_shift += 2;
                continue;
            }

            // If 2-byte match fails, try 1-byte keyword
            token.content.keyword = StringToKeyword (buffer + buf_shift, 1);

            if (token.content.keyword != UNKNOWN_KEYWORD)
            {
                TokenAdd (&frontend->tokens, token);
                buf_shift += 1;
                continue;
            }

            // If no valid keyword is found, this is a syntax error
            SyntaxError ("%s \"%c\"", "unexpected character", buffer[buf_shift]);
        }
    }
}

//--------------------------------------------------------------------------

static void TokenAdd (token_array_t* tokens, token_t token)
{
    assert (tokens != NULL);

    ast_node_t** token_array = tokens->token_array;

    if (tokens->size >= tokens->capacity)
    {
        TokenArrayResize (tokens);
    }
    
    token_array[tokens->size++] = AstNodeCreate (token, NULL, NULL);
}

//--------------------------------------------------------------------------

static void NameTableAdd (name_table_t* identifiers, identifier_t* identifier)
{
    assert (identifiers != NULL);
    assert (identifier != NULL);

    identifier_t* name_table = identifiers->name_table;

    if (identifiers->size >= identifiers->capacity)
    {
        NameTableResize (identifiers);
    }

    for (size_t i = 0; i < identifiers->size; ++i)
    {
        if (!strncmp (name_table[i].begin, identifier->begin, name_table[i].length))
        {
            identifier->index = (int) i;
            return;
        }
    }

    identifier->index = (int) identifiers->size;
    name_table[identifiers->size++] = *identifier;
}

//--------------------------------------------------------------------------

static void TokenArrayResize (token_array_t* tokens)
{
    assert (tokens != NULL);

    tokens->token_array = (ast_node_t**) 
        MyRecalloc (tokens->token_array, tokens->capacity * INCREASE_FACTOR, tokens->capacity, sizeof (ast_node_t*));
        
    CUSTOM_ASSERT (tokens->token_array != NULL);

    tokens->capacity *= INCREASE_FACTOR;
}

//--------------------------------------------------------------------------

static void NameTableResize (name_table_t* identifiers)
{
    assert (identifiers != NULL);

    identifiers->name_table = (identifier_t*) 
        MyRecalloc (identifiers->name_table, identifiers->capacity * INCREASE_FACTOR, identifiers->capacity, sizeof (ast_node_t*));

    CUSTOM_ASSERT (identifiers->name_table != NULL);

    identifiers->capacity *= INCREASE_FACTOR;
}

//--------------------------------------------------------------------------

static void TokenArrayDestroy (token_array_t* tokens)
{   
    assert (tokens != NULL);

    for (size_t i = 0; i < tokens->size; ++i)
    {
        FREE (tokens->token_array[i]);
    }

    tokens->size = 0;
    tokens->capacity = 0;

    FREE (tokens->token_array);
}

static void NameTableDestroy (name_table_t* identifiers)
{
    assert (identifiers != NULL);

    FREE (identifiers->name_table);

    identifiers->size = 0;
    identifiers->capacity = 0;
}

//--------------------------------------------------------------------------
