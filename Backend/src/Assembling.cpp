#include <stdio.h>

#include "Tree.h"
#include "Assembling.h"

#define NODE_RESERVED_(token_name) (node->data.type == RESERVED && node->data.content.reserved == token_name)

void TreeToAsm (FILE* asm_file, tree_node_t* node, int* label_num)
{
    CustomAssert (asm_file  != NULL);
    CustomAssert (label_num != NULL);

    if (node == NULL)
        return;

    if (NODE_RESERVED_ (SEMI))
    {
        TreeToAsm (asm_file, node->left, label_num);
        TreeToAsm (asm_file, node->right, label_num);
    }

    else if (node->data.type == NAME)
    {
        fprintf (asm_file, "push [%d]\n", node->data.content.name.index);
    }

    else if (node->data.type == NUMBER)
    {
        fprintf (asm_file, "push %lg\n", node->data.content.number);
    }

    else if (NODE_RESERVED_ (ADD) || NODE_RESERVED_ (SUB) || NODE_RESERVED_ (MUL) || NODE_RESERVED_ (DIV))
    {
        TreeToAsm (asm_file, node->left, label_num);
        TreeToAsm (asm_file, node->right, label_num);
        
        if (NODE_RESERVED_ (ADD))
            fprintf (asm_file, "add\n");

        else if (NODE_RESERVED_ (SUB))
            fprintf (asm_file, "sub\n");

        else if (NODE_RESERVED_ (MUL))
            fprintf (asm_file, "mul\n");

        else if (NODE_RESERVED_ (DIV))
            fprintf (asm_file, "div\n");
    }

    else if (NODE_RESERVED_ (LT)  || NODE_RESERVED_ (GT)  || 
             NODE_RESERVED_ (LTE) || NODE_RESERVED_ (GTE) ||
             NODE_RESERVED_ (EQ)  || NODE_RESERVED_ (NEQ))
    {
        TreeToAsm (asm_file, node->left, label_num);
        TreeToAsm (asm_file, node->right, label_num);
        
        if (NODE_RESERVED_ (LT))
            fprintf (asm_file, "jb %d:\n", (*label_num)++);

        else if (NODE_RESERVED_ (GT))
            fprintf (asm_file, "ja %d:\n", (*label_num)++);

        else if (NODE_RESERVED_ (LTE))
            fprintf (asm_file, "jbe %d:\n", (*label_num)++);

        else if (NODE_RESERVED_ (GTE))
            fprintf (asm_file, "jae %d:\n", (*label_num)++);

        else if (NODE_RESERVED_ (EQ))
            fprintf (asm_file, "je %d:\n", (*label_num)++);

        else if (NODE_RESERVED_ (NEQ))
            fprintf (asm_file, "jne %d:\n", (*label_num)++);
    }

    else if (NODE_RESERVED_ (ASG))
    {
        TreeToAsm (asm_file, node->right, label_num);
        fprintf (asm_file, "pop [%d]\n", node->left->data.content.name.index);
    }

    else if (NODE_RESERVED_ (PRINT))
    {
        fprintf (asm_file, "push [%d]\n", node->left->data.content.name.index);
        fprintf (asm_file, "out");
    }

    else if (NODE_RESERVED_ (IF))
    {
        int if_body_label = *label_num;

        TreeToAsm (asm_file, node->left, label_num);

        int if_end_label = (*label_num)++;
        fprintf (asm_file, "jmp %d:\n", if_end_label);
        fprintf (asm_file, "%d:\n", if_body_label);

        TreeToAsm (asm_file, node->right, label_num);

        fprintf (asm_file, "%d:\n", if_end_label);
    }

    else if (NODE_RESERVED_ (WHILE))
    {
        int while_label = (*label_num)++;
        int while_body_label = *label_num;

        fprintf (asm_file, "%d:\n", while_label);
        TreeToAsm (asm_file, node->left, label_num);
        int while_end_label = (*label_num)++;

        fprintf (asm_file, "jmp %d:\n", while_end_label);
        fprintf (asm_file, "%d:\n", while_body_label);

        TreeToAsm (asm_file, node->right, label_num);

        fprintf (asm_file, "jmp %d:\n", while_label);
        fprintf (asm_file, "%d:\n", while_end_label);
    }
}

#undef NODE_RESERVED_

/*
while:
jne while_body
jmp while_end
while_body:
...
jmp while:
while_end:
*/

/*
jb if_body
jmp if_end
if_body:
...
if_end:
*/
