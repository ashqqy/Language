#ifndef IR_PARSE_H
#define IR_PARSE_H

#include "ir.h"

void IRParse (ir_context_t* ir_context);

ir_node_arg_t ParseMovArg (char* ir_buffer, size_t* ir_buffer_shift);

#endif // IR_PARSE_H
