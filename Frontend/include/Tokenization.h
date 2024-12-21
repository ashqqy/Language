#define TOKENIZATION_H

#include "Tree.h"

const int TOKEN_ARRAY_SIZE = 10;

struct frontend_t
{
    tree_node_t** token_array;
    const char* name_table;
};

tree_node_t** Tokenization (char* buf, size_t buf_size, frontend_t* frontend);
tree_node_t** TokenArrayResize (tree_node_t** token_array, size_t* tkn_arr_size);
void TokenArrayDestroy (tree_node_t** token_array);
[[noreturn]] void SyntaxError (const char* message);

#endif // TOKENIZATION_H