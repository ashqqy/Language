#ifndef PARCE_H
#define PARCE_H

tree_node_t* GetProgram (tree_node_t** token_array, size_t* shift);
tree_node_t* GetVarInit (tree_node_t** token_array, size_t* shift);
tree_node_t* GetType (tree_node_t** token_array, size_t* shift);
tree_node_t* GetAsg (tree_node_t** token_array, size_t* shift);
tree_node_t* GetName (tree_node_t** token_array, size_t* shift);
tree_node_t* GetEql (tree_node_t** token_array, size_t* shift);
tree_node_t* GetNum (tree_node_t** token_array, size_t* shift);
tree_node_t* GetSemi (tree_node_t** token_array, size_t* shift);

#endif // PARCE_H