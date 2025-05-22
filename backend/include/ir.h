#ifndef IR_H
#define IR_H

#include <stdbool.h>
#include <stdio.h>

#include "linked-list.h"

//----------------------------------------------------------------------------

static const char* IR_FILE_NAME = "./tmp/intermediate.ir";

//----------------------------------------------------------------------------

typedef enum ir_keyword
{
    IR_UNKNOWN_KEYWORD     = 0,

    IR_FUNCTION_DEFINITION = 1,
    IR_CALL                = 2,
    IR_RET                 = 3,

    IR_MOV                 = 4,

    IR_JUMP                = 5,
    IR_LABEL               = 6,
    IR_CMP                 = 7,

    IR_MATH                = 8,

    IR_CALL_ARG_DTR        = 9,
    IR_CONDITION_ARG_DTR   = 10
} ir_keyword_t;

typedef struct ir_keyword_mapping
{
    const char*  string;
    ir_keyword_t keyword;
} ir_keyword_mapping_t;

static const ir_keyword_mapping_t ir_keyword_mappings[] = 
{
    #define MATCH_KEYWORD(ir_keyword) {.string = #ir_keyword + 3, .keyword = ir_keyword}

    MATCH_KEYWORD (IR_UNKNOWN_KEYWORD),
    MATCH_KEYWORD (IR_FUNCTION_DEFINITION),
    MATCH_KEYWORD (IR_CALL),
    MATCH_KEYWORD (IR_RET),
    MATCH_KEYWORD (IR_MOV),
    MATCH_KEYWORD (IR_JUMP),
    MATCH_KEYWORD (IR_LABEL),
    MATCH_KEYWORD (IR_CMP),
    MATCH_KEYWORD (IR_MATH),
    MATCH_KEYWORD (IR_CALL_ARG_DTR),
    MATCH_KEYWORD (IR_CONDITION_ARG_DTR),

    #undef MATCH_KEYWORD
};

//----------------------------------------------------------------------------

typedef enum ir_jump_type
{
    IR_UNKNOWN_JMP = 0,

    IR_JMP = 1,
    IR_JE  = 2,
    IR_JL  = 3,
    IR_JG  = 4,
    IR_JLE = 5,
    IR_JGE = 6,
    IR_JNE = 7,
} ir_jump_type_t;

typedef struct ir_jump_type_mapping
{
    const char*    string;
    ir_jump_type_t jump_type;
} ir_jump_type_mapping_t;

static const ir_jump_type_mapping_t ir_jump_type_mappings[] = 
{
    {.string = "JMP",              .jump_type = IR_JMP},
    {.string = "EQUAL",            .jump_type = IR_JE},
    {.string = "LESS",             .jump_type = IR_JL},
    {.string = "GREATER",          .jump_type = IR_JG},
    {.string = "LESS_OR_EQUAL",    .jump_type = IR_JLE},
    {.string = "GREATER_OR_EQUAL", .jump_type = IR_JGE},
    {.string = "NOT_EQUAL",        .jump_type = IR_JNE},
};

//----------------------------------------------------------------------------

typedef enum ir_math
{
    IR_UNKNOWN_MATH_OPERATION = 0,

    IR_ADD = 1,
    IR_SUB = 2,
    IR_MUL = 3,
    IR_DIV = 4,
} ir_math_type_t;

typedef struct ir_math_operation_mapping
{
    const char*    string;
    ir_math_type_t math_operation;
} ir_math_operation_mapping_t;

static const ir_math_operation_mapping_t ir_math_operation_mappings[] = 
{
    #define MATCH_MATH(keyword) {.string = #keyword + 3, .math_operation = keyword}

    MATCH_MATH (IR_UNKNOWN_MATH_OPERATION),
    MATCH_MATH (IR_ADD),
    MATCH_MATH (IR_SUB),
    MATCH_MATH (IR_MUL),
    MATCH_MATH (IR_DIV),

    #undef MATCH_MATH
};

//----------------------------------------------------------------------------

typedef struct ir_context
{
    list_t ir_list;

    FILE* ir_file;

    char* ir_buffer;
    size_t ir_buffer_size;

    FILE* output_file;
} ir_context_t;

//----------------------------------------------------------------------------

typedef struct ir_label
{
    char*  begin;
    size_t length;
} ir_label_t;

typedef struct ir_node_arg
{
    // ListCommand_t com;
    int constant;
    int identifier;
    ir_label_t label;
    ir_jump_type_t jump_type;
    ir_math_type_t math_type;
    bool stack;
    bool memory;
    bool reg;
} ir_node_arg_t;

typedef struct ir_node 
{
    ir_keyword_t ir_keyword;
    ir_node_arg_t first_arg;
    ir_node_arg_t second_arg;
} ir_node_t;

//----------------------------------------------------------------------------

void IRContextInit    (ir_context_t* ir_context, const char* output_file);
void IRContextDestroy (ir_context_t* ir_context);

ir_keyword_t   IRStringToKeyword (const char* string_begin, size_t string_len);
ir_jump_type_t IRStringToJmpType (const char* string_begin, size_t string_len);
ir_math_type_t IRStringToMathOp  (const char* string_begin, size_t string_len);

#endif // IR_H
