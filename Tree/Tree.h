#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#include "Common.h"
    
//--------------------------------------------------------------------------

struct tree_node_t
{
    tree_data_t data;
    tree_node_t* left;
    tree_node_t* right;
};

//--------------------------------------------------------------------------

tree_node_t* NodeCreate (tree_data_t data, tree_node_t* left_node = NULL, tree_node_t* right_node = NULL);
tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to);
tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data);
tree_node_t* SubtreeCopy (tree_node_t* node);
void TreeDestroy (tree_node_t* node);

void TreeDump (tree_node_t* root_node);
void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node);
void TreeNodeLinkDump (FILE* dump_file, tree_node_t* node);

//--------------------------------------------------------------------------

#endif //TREE_H