#include "Tokenization.h"
#include "Parse.h"
#include "Tree.h"
#include "Common.h"
#define TKN_DATA_(section) token_array[*shift]->data.section

#define GET_RESERVED_(reserved_token, node_name, return_null, expected)                 \
    tree_node_t* node_name = NULL;                                                      \
    if (TKN_DATA_ (type) == RESERVED && TKN_DATA_ (content.reserved) == reserved_token) \
    {                                                                                   \
        node_name = token_array[*shift];                                                \
        *shift += 1;                                                                    \
    }                                                                                   \
    else if (return_null)                                                               \
    {                                                                                   \
        return node_name;                                                               \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        SyntaxError ("Expected " #expected);                                            \
    }

tree_node_t* GetProgram (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* root_node = GetStatements (token_array, shift, END);

    return root_node;
}

tree_node_t* GetStatement (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* if_node = GetIf (token_array, shift);
    if (if_node != NULL)
        return if_node;

    tree_node_t* while_node = GetWhile (token_array, shift);
    if (while_node != NULL)
        return while_node;

    tree_node_t* print_node = GetPrint (token_array, shift);
    if (print_node != NULL)
        return print_node;

    tree_node_t* var_node = GetVarInit (token_array, shift);
    return var_node;
}

tree_node_t* GetStatements (tree_node_t** token_array, size_t* shift, reserved_t end_token)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* root_node = GetStatement (token_array, shift);
    tree_node_t* join_node = root_node;
    while (TKN_DATA_ (type) != RESERVED || TKN_DATA_ (content.reserved) != end_token)
    {
        tree_node_t* next_node = GetStatement (token_array, shift);
        NodeLink (next_node, &join_node->right);
        join_node = next_node;    
    }

    return root_node;
}

tree_node_t* GetPrint (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    GET_RESERVED_ (PRINT, print_node, 1, 'print');
    GET_RESERVED_ (LBRACK, lbrack_node, 0, '(');
    tree_node_t* name_node = GetName (token_array, shift);
    GET_RESERVED_ (RBRACK, rbrack_node, 0, ')');
    GET_RESERVED_ (SEMI, semi_node, 0, ';');

    lbrack_node = NULL; rbrack_node = NULL;

    NodeLink (name_node, &print_node->left);
    NodeLink (print_node, &semi_node->left);

    return semi_node;
}

tree_node_t* GetIf (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    GET_RESERVED_ (IF, if_node, 1, 'if');
    GET_RESERVED_ (LBRACK, lbrack_node, 0, '(');
    tree_node_t* bool_node = GetBool (token_array, shift);
    GET_RESERVED_ (RBRACK, rbrack_node, 0, ')');
    GET_RESERVED_ (LCURBR, lcurbr_node, 0, '{');

    tree_node_t* if_statement_node = GetStatements (token_array, shift, RCURBR);

    GET_RESERVED_ (RCURBR, rcurbr_node, 0, '}');

    if (TKN_DATA_ (type) == RESERVED && TKN_DATA_ (content.reserved) == ELSE)
    {
        GET_RESERVED_ (ELSE, else_node, 1, 'else');
        GET_RESERVED_ (LCURBR, else_lcurbr_node, 0, '{');
        tree_node_t* else_statement_node = GetStatements (token_array, shift, RCURBR);
        GET_RESERVED_ (RCURBR, else_rcurbr_node, 0, '}');

        NodeLink (bool_node, &if_node->left);
        NodeLink (else_node, &if_node->right);
        NodeLink (if_statement_node, &else_node->left);        
        NodeLink (else_statement_node, &else_node->right);
        NodeLink (if_node, &lbrack_node->left);
        lbrack_node->data.content.reserved = SEMI;        
        
        // не используются
        else_lcurbr_node = NULL; else_rcurbr_node = NULL;
    }
    else 
    {
        NodeLink (bool_node, &if_node->left);
        NodeLink (if_statement_node, &if_node->right);
        NodeLink (if_node, &lbrack_node->left);
        lbrack_node->data.content.reserved = SEMI;
    }

    // не используются
    rbrack_node = NULL; lcurbr_node = NULL; rcurbr_node = NULL;

    return lbrack_node;
}

tree_node_t* GetWhile (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    GET_RESERVED_ (WHILE, while_node, 1, 'while');
    GET_RESERVED_ (LBRACK, lbrack_node, 0, '(');
    tree_node_t* bool_node = GetBool (token_array, shift);
    GET_RESERVED_ (RBRACK, rbrack_node, 0, ')');
    GET_RESERVED_ (LCURBR, lcurbr_node, 0, '{');

    tree_node_t* statement_node = GetStatements (token_array, shift, RCURBR);

    GET_RESERVED_ (RCURBR, rcurbr_node, 0, '}');

    // не используются
    rbrack_node = NULL; lcurbr_node = NULL; rcurbr_node = NULL;

    NodeLink (bool_node, &while_node->left);
    NodeLink (statement_node, &while_node->right);
    NodeLink (while_node, &lbrack_node->left);
    lbrack_node->data.content.reserved = SEMI;

    return lbrack_node;
}

tree_node_t* GetBool (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* left_node  = GetBoolP1 (token_array, shift);
    tree_node_t* comp_node  = GetCompare (token_array, shift);
    tree_node_t* right_node = GetBoolP1 (token_array, shift);
    
    NodeLink (left_node, &comp_node->left);
    NodeLink (right_node, &comp_node->right);

    return comp_node;
}

tree_node_t* GetBoolP1 (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* num_node = GetMath (token_array, shift);
    if (num_node != NULL)
        return num_node;     

    tree_node_t* var_node = GetName (token_array, shift);
    return var_node;
}

tree_node_t* GetCompare (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if ((TKN_DATA_ (type) == RESERVED) &&      
        (TKN_DATA_ (content.reserved) == EQ  || TKN_DATA_ (content.reserved) == NEQ ||
         TKN_DATA_ (content.reserved) == LTE || TKN_DATA_ (content.reserved) == GTE || 
         TKN_DATA_ (content.reserved) == LT  || TKN_DATA_ (content.reserved) == GT))
    {
        tree_node_t* comp_node = token_array[*shift];
        *shift += 1;
        return comp_node;
    }

    else
        SyntaxError ("Expected compare operator");
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

    GET_RESERVED_ (ASG, asg_node, 0, '=');

    tree_node_t* math_node  = GetMath (token_array, shift);

    if (math_node == NULL)
        SyntaxError ("Math assignment error");
    GET_RESERVED_ (SEMI, semi_node, 0, ';');


    NodeLink (asg_node, &semi_node->left);
    NodeLink (name_node, &asg_node->left);
    NodeLink (math_node, &asg_node->right);

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
        return NULL;
}

//--------------------------------------------------------------------------

tree_node_t* GetMath (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* math_node = GetPlus (token_array, shift);

    return math_node;
}

tree_node_t* GetPlus (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetMult (token_array, shift);
    if ((TKN_DATA_ (type) == RESERVED) && (TKN_DATA_ (content.reserved) == ADD || TKN_DATA_ (content.reserved) == SUB))
    {   
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetPlus (token_array, shift);
        
        NodeLink (first_node, &parent_node->left);
        NodeLink (second_node, &parent_node->right);

        first_node = parent_node;
    }
    return first_node;
}

tree_node_t* GetMult (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetDeg (token_array, shift);

    if ((TKN_DATA_ (type) == RESERVED) && 
        (TKN_DATA_ (content.reserved) == MUL || TKN_DATA_ (content.reserved) == DIV))
    {
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetMult (token_array, shift);

        NodeLink (first_node,  &parent_node->left);
        NodeLink (second_node, &parent_node->right);
        
        first_node = parent_node;
    }
    return first_node;
}

tree_node_t* GetDeg (tree_node_t** token_array, size_t* shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    tree_node_t* first_node = GetBracket (token_array, shift);

    if ((TKN_DATA_ (type) == RESERVED) && (TKN_DATA_ (content.reserved) == DEG))
    {
        tree_node_t* parent_node = token_array[*shift];

        *shift += 1;
        tree_node_t* second_node = GetDeg (token_array, shift);

        NodeLink (first_node,  &parent_node->left);
        NodeLink (second_node, &parent_node->right);

        first_node = parent_node;
    }

    return first_node;
}

tree_node_t* GetBracket (tree_node_t** token_array, size_t* const shift)
{
    CustomAssert (token_array != NULL);
    CustomAssert (shift       != NULL);

    if ((TKN_DATA_ (type) == RESERVED) && 
       (TKN_DATA_ (content.reserved) == LBRACK))
    {
        *shift += 1;
        tree_node_t* node = GetPlus (token_array, shift);
        if ((TKN_DATA_ (type) != RESERVED) || 
            (TKN_DATA_ (content.reserved) != RBRACK))
            SyntaxError ("Expected close bracket");

        *shift += 1;
        return node;
    }

    else if (TKN_DATA_ (type) == NUMBER)
        return GetNum (token_array, shift);
    else
        return GetName (token_array, shift);
}

//--------------------------------------------------------------------------

#undef TKN_DATA_
#undef GET_RESERVED_

//--------------------------------------------------------------------------
