#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Common.h"
#include "Tree.h"
#include "Tokenization.h"
#include "CustomAssert.h"
#include "MyAllocation.h"

//--------------------------------------------------------------------------

#define TOKEN_INIT_(token_data)                                                     \
    token_array[tkn_arr_shift] = (tree_node_t*) calloc (1, sizeof (tree_node_t));   \
    token_array[tkn_arr_shift]->data = token_data;                                  \
    tkn_arr_shift += 1;                                                             \
    if (tkn_arr_shift >= tkn_arr_size)                                              \
    {                                                                               \
        token_array = TokenArrayResize (token_array, &tkn_arr_size);                \
        CustomWarning (token_array != NULL, NULL);                                  \
    }

//--------------------------------------------------------------------------

common_errors FrontendInit (frontend_t* frontend)
{
    CustomAssert (frontend != NULL);

    frontend->names_table = (name_t*) calloc (NAMES_TABLE_SIZE, sizeof (name_t));
    CustomWarning (frontend->names_table != NULL, ALLOCATION_ERROR);
    frontend->n_names = 0;
    frontend->token_array = NULL;
    frontend->names_table_size = NAMES_TABLE_SIZE;

    return NO_ERRORS;
}

//--------------------------------------------------------------------------

tree_node_t** Tokenization (char* buf, size_t buf_size, frontend_t* frontend)
{
    CustomAssert (buf != NULL);

    size_t buf_shift = 0;

    tree_node_t** token_array = (tree_node_t**) calloc (TOKEN_ARRAY_SIZE, sizeof (tree_node_t*));
    CustomWarning (token_array != NULL, NULL);
    size_t tkn_arr_size = TOKEN_ARRAY_SIZE;
    size_t tkn_arr_shift = 0;

    while (buf_shift < buf_size)
    {
        // считываем число
        if (isdigit(buf[buf_shift]))
        {
            tree_data_t token_data = {.type = NUMBER};
            size_t num_len = 0;
            double readen_number = 0;

            sscanf (buf + buf_shift, "%lf%n", &readen_number, (int*) &num_len);
            token_data.content.number = readen_number;

            TOKEN_INIT_ (token_data);

            buf_shift += num_len;
        }

        // считываем слово
        else if (isalpha(buf[buf_shift]))
        {
            tree_data_t token_data = {};
            size_t name_len = 0;
            sscanf (buf + buf_shift, "%*[a-zA-Z]%n", (int*) &name_len);

            // если не нашлось такое зарезервированное имя, то заполняем токен именем
            if (FindReservedDataByName (buf + buf_shift, name_len, &token_data) != 0)
            {
                name_t name_struct = {.begin = buf + buf_shift, .len = name_len};
                name_struct.index = FindNameIndex (frontend, &name_struct);
                CustomWarning (name_struct.index != -1, NULL);
                token_data.type = NAME;
                token_data.content.name = name_struct;
            }

            TOKEN_INIT_ (token_data);
            buf_shift += name_len;
        }

        // пропускаем все после # (комментарий)
        else if (buf[buf_shift] == '#')
        {
            char* end_of_line = strchr (buf + buf_shift, '\n');
            if (end_of_line == NULL)
                *(buf + buf_shift) = '\n';
            else
                buf_shift = (size_t) (end_of_line - buf);
        }

        // игнорируем мусорные символы
        else if (isspace(buf[buf_shift]))
        {
            buf_shift += 1;
        }

        else
        {
            tree_data_t token_data = {};
            // если нашлась комбинация из 2 байт
            if (FindReservedDataByName (buf + buf_shift, 2, &token_data) == 0)
            {
                TOKEN_INIT_ (token_data);
                buf_shift += 2;
            }
            // иначе считываем как 1 байт
            else if (FindReservedDataByName (buf + buf_shift, 1, &token_data) == 0)
            {
                TOKEN_INIT_ (token_data);
                buf_shift += 1;
            }

            else 
            {
                SyntaxError ("Unexpected character");
            }
        }
    }

    tree_data_t token_data = {.type = RESERVED, .content = {.reserved = END}};
    TOKEN_INIT_ (token_data);

    frontend->token_array = token_array;

    return token_array;
}

//--------------------------------------------------------------------------

int FindNameIndex (frontend_t* frontend, name_t* name)
{
    CustomAssert (name != NULL);

    for (int i = 0; i < frontend->n_names; ++i)
    {
        if (strncmp (frontend->names_table[i].begin, name->begin, frontend->names_table[i].len) == 0)
        {
            name->index = i;
            return i;
        }
    }

    if ((size_t) frontend->n_names >= frontend->names_table_size)
    {
        frontend->names_table = NamesArrayResize (frontend, &frontend->names_table_size);
        CustomWarning (frontend->names_table != NULL, -1);
    }

    name->index = frontend->n_names;
    frontend->names_table[frontend->n_names] = *name;
    frontend->n_names += 1;

    return name->index;
}

//--------------------------------------------------------------------------

tree_node_t** TokenArrayResize (tree_node_t** token_array, size_t* token_arr_size)
{
    CustomAssert (token_array  != NULL);
    CustomAssert (token_arr_size != NULL);

    const tree_node_t* POISON = NULL;
    token_array = (tree_node_t**) MyRecalloc (token_array, *token_arr_size * 2, sizeof (tree_node_t*), *token_arr_size, &POISON);
    CustomWarning (token_array != NULL, NULL);
    *token_arr_size *= 2;

    return token_array;
}

//--------------------------------------------------------------------------

name_t* NamesArrayResize (frontend_t* frontend, size_t* arr_size)
{
    CustomAssert (frontend != NULL);
    CustomAssert (arr_size    != NULL);

    const name_t* POISON = NULL;
    frontend->names_table = (name_t*) MyRecalloc (frontend->names_table, *arr_size * 2, sizeof (tree_node_t*), *arr_size, &POISON);
    CustomWarning (frontend->names_table != NULL, NULL);
    *arr_size *= 2;

    return frontend->names_table;
}

//--------------------------------------------------------------------------

void TokenArrayDestroy (tree_node_t** token_array)
{   
    int i = 0;
    while ((token_array[i]->data.type != RESERVED) || (token_array[i]->data.content.reserved != END))
    {
        free (token_array[i]); token_array[i] = NULL;
        i++;
    }
    free (token_array[i]); token_array[i] = NULL;
    free (token_array); token_array = NULL;
}

//--------------------------------------------------------------------------

void SyntaxError (const char* message)
{
    printf ("Syntax error: %s\n", message);
    CustomAssert(0);
}

//--------------------------------------------------------------------------