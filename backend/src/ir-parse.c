#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "ir-parse.h"
#include "ir.h"

void IRParse (ir_context_t* ir_context)
{
    assert (ir_context != NULL);

    char* ir_buffer = ir_context->ir_buffer;

    size_t ir_buffer_shift = 0;

    while (ir_buffer_shift < ir_context->ir_buffer_size)
    {
        while (isspace (ir_buffer[ir_buffer_shift]))
        {
            ir_buffer_shift++;
        }
        
        size_t ir_keyword_size = 0;
        sscanf (ir_buffer + ir_buffer_shift, "%*[a-zA-Z_]%n", (int*) &ir_keyword_size);

        ir_keyword_t ir_keyword = IRStringToKeyword (ir_buffer + ir_buffer_shift, ir_keyword_size);
        ir_node_t ir_node = {.ir_keyword = ir_keyword};

        ir_buffer_shift += ir_keyword_size + 1;

        size_t ir_keyword_arg_size = 0;

        switch (ir_keyword)
        {
            case IR_FUNCTION_DEFINITION:
            {
                sscanf (ir_buffer + ir_buffer_shift, "%*[a-zA-Z0-9]%n", (int*) &ir_keyword_arg_size);
                ir_node.first_arg.label.begin  = ir_buffer + ir_buffer_shift;
                ir_node.first_arg.label.length = ir_keyword_arg_size;
                
                ir_buffer_shift += ir_keyword_arg_size + 2;

                sscanf (ir_buffer + ir_buffer_shift, "%d%n", &ir_node.second_arg.identifier, (int*) &ir_keyword_arg_size);
                ir_buffer_shift += ir_keyword_arg_size + 1;

                break;
            }

            case IR_CALL:
            {
                sscanf (ir_buffer + ir_buffer_shift, "%d%n", &ir_node.second_arg.identifier, (int*) &ir_keyword_arg_size);
                ir_buffer_shift += ir_keyword_arg_size;

                break;
            }

            case IR_RET:
            case IR_CMP:
            case IR_CALL_ARG_DTR:
            case IR_CONDITION_ARG_DTR:
            {
                break;
            }

            case IR_MOV:
            {
                ir_node.first_arg  = ParseMovArg (ir_buffer, &ir_buffer_shift);
                ir_node.second_arg = ParseMovArg (ir_buffer, &ir_buffer_shift);

                break;
            }

            case IR_JUMP:
            {
                sscanf (ir_buffer + ir_buffer_shift, "%*[A-Z]%n", (int*) &ir_keyword_arg_size);
                ir_node.first_arg.jump_type = IRStringToJmpType (ir_buffer + ir_buffer_shift, ir_keyword_arg_size);
                ir_buffer_shift += ir_keyword_arg_size + 1;

                sscanf (ir_buffer + ir_buffer_shift, "%*[a-zA-Z0-9]%n", (int*) &ir_keyword_arg_size);
                ir_node.second_arg.label.begin  = ir_buffer + ir_buffer_shift;
                ir_node.second_arg.label.length = ir_keyword_arg_size;

                ir_buffer_shift += ir_keyword_arg_size;   

                break;
            }

            case IR_LABEL:
            {
                sscanf (ir_buffer + ir_buffer_shift, "%*[a-zA-Z0-9]%n", (int*) &ir_keyword_arg_size);
                ir_node.second_arg.label.begin  = ir_buffer + ir_buffer_shift;
                ir_node.second_arg.label.length = ir_keyword_arg_size;
                ir_buffer_shift += ir_keyword_arg_size;   
                
                break;
            }

            case IR_MATH:
            {
                sscanf (ir_buffer + ir_buffer_shift, "%*[A-Z]%n", (int*) &ir_keyword_arg_size);

                ir_node.first_arg.math_type = IRStringToMathOp (ir_buffer + ir_buffer_shift, ir_keyword_arg_size);

                ir_buffer_shift += ir_keyword_arg_size;

                break;
            }

            default:
            {
                fprintf (stderr, "IR parsing error: unknown ir keyword %.15s\n", ir_buffer + ir_buffer_shift - (ir_keyword_size + 1));
                exit (EXIT_FAILURE);
            }
        }

        ListInsertEnd (&ir_context->ir_list, &ir_node);
    }
}               

ir_node_arg_t ParseMovArg (char* ir_buffer, size_t* ir_buffer_shift)
{
    ir_node_arg_t mov_arg = {};

    if (ir_buffer[*ir_buffer_shift] == 's')
    {
        mov_arg.stack = true;
        *ir_buffer_shift += 6;

        return mov_arg;
    }
    
    if (ir_buffer[*ir_buffer_shift] == 'A')
    {
        mov_arg.reg = true;
        *ir_buffer_shift += 3;
    }

    else if (ir_buffer[*ir_buffer_shift] == '[')
    {
        size_t mem_identifier_size = 0;
        sscanf (ir_buffer + *ir_buffer_shift, "[%d]%n", &mov_arg.identifier, (int*) &mem_identifier_size);
        *ir_buffer_shift += mem_identifier_size + 1;
        mov_arg.memory = true;
    }

    else
    {
        size_t constant_size = 0;
        sscanf (ir_buffer + *ir_buffer_shift, "%d%n", &mov_arg.constant, (int*) &constant_size);
        *ir_buffer_shift += constant_size + 1;
    }

    return mov_arg;
}
