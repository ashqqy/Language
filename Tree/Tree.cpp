#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Common.h"
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

    FILE* dump_file = fopen ("./dump/dump.dot", "w");
    CustomWarning (dump_file != NULL, ;);

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
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %d | { <l>left|<r>right } }\"];\n", 
                    node, node, "NAME", node->data.content.name.index);
            break;
        }

        case RESERVED:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> \\%s | { <l>left|<r>right } }\"];\n", 
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

void TreeArrayDump (tree_node_t** array)
{
    CustomAssert (array != NULL);

    FILE* dump_file = fopen ("./Dump/dump.dot", "w");
    CustomWarning (dump_file != NULL, ;);

    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    // определяем узлы 
    int i = 0;
    while (!(array[i]->data.type == RESERVED && array[i]->data.content.reserved == END))
    {
        TreeNodeDescrArrayDump (dump_file, array[i]);
        i++;
    }

    fprintf (dump_file, "\n");

    fprintf (dump_file, "}\n");

    fclose (dump_file);

    const char command[81] = "dot ./dump/dump.dot -Tpng -o ./dump/dump.png"; // linux
    // const char command[81] = "\"C:/Program Files/Graphviz/bin/dot.exe\" ./dump/dump.dot -Tpng -o ./dump/dump.png"; // windows
    system(command);
}

void TreeNodeDescrArrayDump (FILE* dump_file, tree_node_t* node)
{
    CustomAssert (dump_file != NULL);

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
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> %d | { <l>left|<r>right } }\"];\n", 
                    node, node, "NAME", node->data.content.name.index);
            break;
        }

        case RESERVED:
        {
            fprintf (dump_file, "p%p[label = \"{ <ptr> %p | <type> %s | <data> \\%s | { <l>left|<r>right } }\"];\n", 
                    node, node, "RESERVED", node_data);
            break;
        }

        default: 
            break;
    }
}

//------------------------------------------------------

void TreeOutput (FILE* output_file, tree_node_t* node)
{
    CustomAssert (output_file != NULL);
    CustomAssert (node        != NULL);

    fprintf (output_file, "( ");
    fprintf (output_file, "%d ", node->data.type);
    if (node->data.type == RESERVED)
        fprintf (output_file, "%d ", node->data.content.reserved);
    if (node->data.type == NAME)
        fprintf (output_file, "%d ", node->data.content.name.index);

    if (node->data.type == NUMBER)
        fprintf (output_file, "%g ", node->data.content.number);

    if (node->left != NULL)
        TreeOutput (output_file, node->left);
    else
        fprintf (output_file, "_ ");

    if (node->right != NULL)
        TreeOutput (output_file, node->right);
    else
        fprintf (output_file, "_ ");

    fprintf (output_file, ") ");
}

tree_node_t* TreeInput (FILE* database)
{
    CustomAssert (database != NULL);

    size_t buf_size = 0;
    char* buf = ReadFile (database, &buf_size);
    CustomWarning (buf != NULL, NULL);
    fclose (database);

    size_t shift = 0;

    tree_node_t* root_node = ReadNode (buf, &shift);

    free(buf); buf = NULL;

    return root_node;
}

tree_node_t* ReadNode (char* buf, size_t* shift)
{
    CustomAssert (buf != NULL);
    CustomAssert (shift != NULL);

    tree_data_t node_data = {};

    if (buf[*shift] == '(')
    {
        *shift += 2;
        int type = 0;
        int len_type = 0;
        sscanf (buf + *shift, "%d%n", &type, &len_type);
        node_data.type = (tree_data_type_t) type;
        *shift += (size_t) len_type + 1;
        if (node_data.type == RESERVED)
        {
            int reserved = 0;
            int len_reserved = 0;
            sscanf (buf + *shift, "%d%n", &reserved, &len_reserved);
            node_data.content.reserved = (reserved_t) reserved;
            *shift += (size_t) len_reserved + 1;
        }
        else if (node_data.type == NAME)
        {
            int index = -1;
            int name_len = 0;
            sscanf (buf + *shift, "%d%n", &index, &name_len);
            node_data.content.name.index = index;
            *shift += (size_t) name_len + 1;
        }
        else if (node_data.type == NUMBER)
        {
            double number = 0;
            int num_len = 0;
            sscanf (buf + *shift, "%lf%n", &number, &num_len);
            node_data.content.number = number;
            *shift += (size_t) num_len + 1;
        }

        tree_node_t* left_node  = ReadNode (buf, shift);
        tree_node_t* right_node = ReadNode (buf, shift);

        if (buf[*shift] == ')')
        {
            *shift += 2;
            return NodeCreate (node_data, left_node, right_node);
        }
        else
        {
            CustomAssert ("expected close bracket" && 0);
        }
    }

    else
    {
        *shift += 2;
        return NULL;
    }
}
