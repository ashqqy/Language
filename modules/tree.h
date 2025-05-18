#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#include "common.h"
    
//--------------------------------------------------------------------------

typedef struct tree_node tree_node_t;

struct tree_node
{
    tree_data_t data;
    tree_node_t* left;
    tree_node_t* right;
};

//--------------------------------------------------------------------------

tree_node_t* NodeCreate (tree_data_t data, tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to);
tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data);
tree_node_t* SubtreeCopy (tree_node_t* node);
void TreeDestroy (tree_node_t* node);

void TreeDump          (FILE* dump_file, tree_node_t* root_node);
void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node);
void TreeNodeLinkDump  (FILE* dump_file, tree_node_t* node);

void TreeArrayDump          (tree_node_t** array);
void TreeNodeDescrArrayDump (FILE* dump_file, tree_node_t* node);

void TreeOutput (FILE* output_file, tree_node_t* node);
tree_node_t* TreeInput (FILE* database);
tree_node_t* ReadNode (char* buf, size_t* shift);

//--------------------------------------------------------------------------

#endif //TREE_H
