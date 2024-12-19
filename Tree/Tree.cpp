#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Common.h"
#include "CustomAssert.h"
#include "Tree.h"

//------------------------------------------------------

tree_node_t* NodeCreate (tree_data_t data, tree_node_t* left_node, tree_node_t* right_node)
{
    tree_node_t* node = (tree_node_t*) calloc (1, sizeof (tree_node_t));
    if (node == NULL)
        return NULL;

    node->data   = data;
    node->left   = left_node;
    node->right  = right_node;

    return node;
}
//------------------------------------------------------

tree_node_t* NodeLink (tree_node_t* node, tree_node_t** node_to_link_to)
{
    CustomAssert (node            != NULL);
    CustomAssert (node_to_link_to != NULL);

    *node_to_link_to = node;

    return node;
}

//------------------------------------------------------

tree_node_t* NodeEditData (tree_node_t* node, tree_data_t new_data)
{
    CustomAssert (node != NULL);

    node->data = new_data;

    return node;
}

//------------------------------------------------------

tree_node_t* SubtreeCopy (tree_node_t* node)
{
    CustomAssert (node != NULL);

    tree_node_t* node_copy = NodeCreate (node->data);

    if (node->left != NULL)
    {
        tree_node_t* left_copy = SubtreeCopy (node->left);
        NodeLink (left_copy, &node_copy->left);
    }

    if (node->right != NULL)
    {
        tree_node_t* right_copy = SubtreeCopy (node->right);
        NodeLink (right_copy, &node_copy->right);
    }

    return node_copy;
}

//------------------------------------------------------

void TreeDestroy (tree_node_t* node)
{
    if (node == NULL)
        return;

    TreeDestroy (node->left);
    TreeDestroy (node->right);

    free (node); node = NULL;
}

//------------------------------------------------------

void TreeDump (tree_node_t* root_node)
{
    CustomAssert (root_node != NULL);

    FILE* dump_file = fopen ("./Dump/dump.dot", "w");

    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    // определяем узлы 
    if (root_node != NULL)
        TreeNodeDescrDump (dump_file, root_node);
    fprintf (dump_file, "\n");

    // соединяем узлы
    if (root_node != NULL)
        TreeNodeLinkDump (dump_file, root_node);

    fprintf (dump_file, "}\n");

    fclose (dump_file);

    const char command[81] = "dot ./dump/dump.dot -Tpng -o ./dump/dump.png"; // linux
    // const char command[81] = "\"C:/Program Files/Graphviz/bin/dot.exe\" ./dump/dump.dot -Tpng -o ./dump/dump.png"; // windows
    system(command);
}

void TreeNodeDescrDump (FILE* dump_file, tree_node_t* node)
{
    CustomAssert (dump_file != NULL);

    if (node == NULL)
        return;

    const char* node_data = FindReservedNameByData (node->data);

    switch (node->data.type)
    {
        case NUMBER: 
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %lg | { <l>left|<r>right } }\"];\n", 
                    node, node, "NUMBER", node->data.content.number);
            break;
        }

        case NAME:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> name_len = %lu | { <l>left|<r>right } }\"];\n", 
                    node, node, "NAME", node->data.content.name.len);
            break;
        }

        case RESERVED:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %s | { <l>left|<r>right } }\"];\n", 
                    node, node, "RESERVED", node_data);
            break;
        }

        default: 
            break;
    }
    
    if (node->left  != NULL) TreeNodeDescrDump (dump_file, node->left);
    if (node->right != NULL) TreeNodeDescrDump (dump_file, node->right);
}

void TreeNodeLinkDump (FILE* dump_file, tree_node_t* node)
{   
    CustomAssert (dump_file != NULL);

    if (node->left  != NULL) 
    {
        fprintf (dump_file, "p%p:<l> -> p%p\n", node, node->left);
        TreeNodeLinkDump (dump_file, node->left);
    }
    if (node->right != NULL) 
    {
        fprintf (dump_file, "p%p:<r> -> p%p\n", node, node->right);
        TreeNodeLinkDump (dump_file, node->right);
    }
}

//------------------------------------------------------
