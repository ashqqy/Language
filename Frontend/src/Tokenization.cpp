#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Common.h"
#include "Tree.h"
#include "Tokenization.h"
#include "CustomAssert.h"

//--------------------------------------------------------------------------

#define TOKEN_INIT_(token_data)                                                   \
    token_array[tkn_arr_shift] = (tree_node_t*) calloc (1, sizeof (tree_node_t)); \
    token_array[tkn_arr_shift]->data = token_data;                                \
    tkn_arr_shift += 1

//--------------------------------------------------------------------------

tree_node_t** Tokenization (char* buf, size_t buf_size)
{
    CustomAssert (buf != NULL);

    size_t buf_shift = 0;

    tree_node_t** token_array = (tree_node_t**) calloc (TOKEN_ARRAY_SIZE, sizeof (tree_node_t*));
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
                token_data.type = NAME;
                token_data.content.name = name_struct;
            }

            TOKEN_INIT_ (token_data);
            buf_shift += name_len;
        }

        // считываем двухбайтовые символы
        else if (buf[buf_shift] == '|' || buf[buf_shift] == '&' || buf[buf_shift] == '=' ||
                 buf[buf_shift] == '!' || buf[buf_shift] == '<' || buf[buf_shift] == '>')
        {
            tree_data_t token_data = {};
            // если нашлось такое зарезервированное имя
            if (FindReservedDataByName (buf + buf_shift, 2, &token_data) == 0)
            {
                TOKEN_INIT_ (token_data);
                buf_shift += 1;
            }
        }

        // считываем однобайтовые символы
        else if (buf[buf_shift] == '(' || buf[buf_shift] == ')' || buf[buf_shift] == ';' ||
                 buf[buf_shift] == '{' || buf[buf_shift] == '}' || buf[buf_shift] == '+' || 
                 buf[buf_shift] == '-' || buf[buf_shift] == '/' || buf[buf_shift] == '*' || 
                 buf[buf_shift] == '^' || buf[buf_shift] == '=' || buf[buf_shift] == '!' || 
                 buf[buf_shift] == '<' || buf[buf_shift] == '>')
        {
            tree_data_t token_data = {};
            FindReservedDataByName (buf + buf_shift, 1, &token_data);
            TOKEN_INIT_ (token_data);
            buf_shift += 1;
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
        else if (buf[buf_shift] == ' ' || buf[buf_shift] == '\r' || buf[buf_shift] == '\n')
            buf_shift += 1;

        else 
            SyntaxError ("Unexpected character");
    }

    tree_data_t token_data = {.type = RESERVED, .content = {.reserved = END}};
    TOKEN_INIT_ (token_data);

    return token_array;
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