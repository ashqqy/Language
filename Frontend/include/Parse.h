#ifndef PARSE_H
#define PARSE_H

tree_node_t* GetProgram (tree_node_t** token_array, size_t* shift);
tree_node_t* GetStatement (tree_node_t** token_array, size_t* shift);
tree_node_t* GetStatements (tree_node_t** token_array, size_t* shift, reserved_t end_token);
tree_node_t* GetIf (tree_node_t** token_array, size_t* shift);
tree_node_t* GetWhile (tree_node_t** token_array, size_t* shift);
tree_node_t* GetBool (tree_node_t** token_array, size_t* shift);
tree_node_t* GetBoolP1 (tree_node_t** token_array, size_t* shift);
tree_node_t* GetCompare (tree_node_t** token_array, size_t* shift);
tree_node_t* GetVarInit (tree_node_t** token_array, size_t* shift);
tree_node_t* GetType (tree_node_t** token_array, size_t* shift);
tree_node_t* GetAsg (tree_node_t** token_array, size_t* shift);
tree_node_t* GetName (tree_node_t** token_array, size_t* shift);
tree_node_t* GetNum (tree_node_t** token_array, size_t* shift);

tree_node_t* GetMath (tree_node_t** token_array, size_t* shift);
tree_node_t* GetPlus (tree_node_t** token_array, size_t* shift);
tree_node_t* GetMult (tree_node_t** token_array, size_t* shift);
tree_node_t* GetDeg (tree_node_t** token_array, size_t* shift);
tree_node_t* GetBracket (tree_node_t** token_array, size_t* const shift);

#endif // PARSE_H