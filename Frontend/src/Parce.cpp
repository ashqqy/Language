#include "Tokenization.h"
#include "Parce.h"
#include "Tree.h"
#include "CustomAssert.h"

#define TKN_DATA_(section) token_array[*shift]->data.section

tree_node_t* GetProgram (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* root_node = GetVarInit (token_array, shift);

    if (TKN_DATA_ (content.reserved) != END)
        SyntaxError ("EOF not reached");

    return root_node;
}

tree_node_t* GetVarInit (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* type_node = GetType (token_array, shift);

    if (type_node != NULL)
    {
        tree_node_t* semi_node = GetAsg (token_array, shift);
        NodeLink (semi_node->left, &type_node->left);
        NodeLink (type_node, &semi_node->left);
        return semi_node;
    }

    else
    {
        tree_node_t* semi_node = GetAsg (token_array, shift);
        return semi_node;
    }
}

tree_node_t* GetType (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (TKN_DATA_ (type) == RESERVED && (TKN_DATA_ (content.reserved) == INT || TKN_DATA_ (content.reserved) == DBL))
    {
        tree_node_t* type_node = token_array[*shift];
        *shift += 1;
        return type_node;
    }

    else 
        return NULL;
}

tree_node_t* GetAsg (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* name_node = GetName (token_array, shift);
    tree_node_t* asg_node  = GetEql (token_array, shift);
    tree_node_t* num_node  = GetNum (token_array, shift);
    tree_node_t* semi_node = GetSemi (token_array, shift);

    NodeLink (asg_node, &semi_node->left);
    NodeLink (name_node, &asg_node->left);
    NodeLink (num_node, &asg_node->right);

    return semi_node;
}

tree_node_t* GetName (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (TKN_DATA_ (type) == NAME)
    {
        tree_node_t* name_node = token_array[*shift];
        *shift += 1;
        return name_node;
    }
    else
        SyntaxError ("Name Error");
}

tree_node_t* GetEql (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (TKN_DATA_ (type) == RESERVED && TKN_DATA_ (content.reserved) == ASG)
    {
        tree_node_t* eql_node = token_array[*shift];
        *shift += 1;
        return eql_node;
    }
    else
        SyntaxError ("Expected '='");
}

tree_node_t* GetNum (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (TKN_DATA_ (type) == RESERVED && TKN_DATA_ (content.reserved) == SUB)
    {
        tree_node_t* sub_node = token_array[*shift];
        *shift += 1;
        if (TKN_DATA_ (type) == NUMBER)
        {
            tree_node_t* num_node = token_array[*shift];
            *shift += 1;
            NodeLink (num_node, &sub_node->left);
            return sub_node;
        }
        
        else
            SyntaxError ("Number error");
    }

    else if (TKN_DATA_ (type) == NUMBER)
    {
        tree_node_t* num_node = token_array[*shift];
        *shift += 1;
        return num_node;
    }

    else
        SyntaxError ("Number Error");
}

tree_node_t* GetSemi (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if (TKN_DATA_ (type) == RESERVED && TKN_DATA_ (content.reserved) == SEMI)
    {
        tree_node_t* semi_node = token_array[*shift];
        *shift += 1;
        return semi_node;
    }
    else
        SyntaxError ("Expected ';'");
}