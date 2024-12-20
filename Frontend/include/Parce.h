#ifndef PARCE_H
#define PARCE_H

tree_node_t* GetProgram (tree_node_t** token_array, size_t* shift);
tree_node_t* GetStatement (tree_node_t** token_array, size_t* shift);
tree_node_t* GetIf (tree_node_t** token_array, size_t* shift);
tree_node_t* GetBool (tree_node_t** token_array, size_t* shift);
tree_node_t* GetBoolP1 (tree_node_t** token_array, size_t* shift);
tree_node_t* GetCompare (tree_node_t** token_array, size_t* shift);
tree_node_t* GetVarInit (tree_node_t** token_array, size_t* shift);
tree_node_t* GetType (tree_node_t** token_array, size_t* shift);
tree_node_t* GetAsg (tree_node_t** token_array, size_t* shift);
tree_node_t* GetName (tree_node_t** token_array, size_t* shift);
tree_node_t* GetNum (tree_node_t** token_array, size_t* shift);

#endif // PARCE_H