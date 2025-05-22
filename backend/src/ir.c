#include "string.h"

#include <assert.h>

#include "ir.h"

#include "linked-list.h"
#include "common.h"

void IRContextInit (ir_context_t* ir_context, const char* output_file_name)
{
    assert (ir_context != NULL);
    assert (output_file_name != NULL);

    ListInit (&ir_context->ir_list, 10, sizeof (ir_node_t));

    system("mkdir -p ./tmp");

    FILE* ir_file = fopen (IR_FILE_NAME, "r");
    CUSTOM_ASSERT (ir_file != NULL);

    ir_context->ir_file = ir_file;

    ir_context->ir_buffer = ReadOpenedFile (ir_file, &ir_context->ir_buffer_size);

    FILE* output_file = fopen (output_file_name, "w");
    CUSTOM_ASSERT (output_file != NULL);

    ir_context->output_file = output_file;
}

void IRContextDestroy (ir_context_t* ir_context)
{
    assert (ir_context != NULL);

    ListDestroy (&ir_context->ir_list);

    fclose (ir_context->ir_file);

    FREE (ir_context->ir_buffer);
    ir_context->ir_buffer_size = 0;

    fclose (ir_context->output_file);
}

const char* IRKeywordToString (ir_keyword_t ir_keyword)
{
    for (size_t i = 0; i < sizeof (ir_keyword_mappings) / sizeof (ir_keyword_mappings[0]); ++i)
    {
        if (ir_keyword == ir_keyword_mappings[i].keyword)
        {
            return ir_keyword_mappings[i].string;
        }
    }

    return NULL;
}

ir_keyword_t IRStringToKeyword (const char* string_begin, size_t string_len)
{
    assert (string_begin != NULL);

    ir_keyword_t ir_keyword = IR_UNKNOWN_KEYWORD;

    for (size_t i = 0; i < sizeof (ir_keyword_mappings) / sizeof (ir_keyword_mappings[0]); ++i)
    {
        if (strncmp (string_begin, ir_keyword_mappings[i].string, string_len) == 0)
        {
            ir_keyword = ir_keyword_mappings[i].keyword;
            return ir_keyword;
        }
    }

    return ir_keyword;
}

ir_jump_type_t IRStringToJmpType (const char* string_begin, size_t string_len)
{
    assert (string_begin != NULL);

    ir_jump_type_t ir_jump_type = IR_UNKNOWN_JMP;

    for (size_t i = 0; i < sizeof (ir_jump_type_mappings) / sizeof (ir_jump_type_mappings[0]); ++i)
    {
        if (strncmp (string_begin, ir_jump_type_mappings[i].string, string_len) == 0)
        {
            ir_jump_type = ir_jump_type_mappings[i].jump_type;
            return ir_jump_type;
        }
    }

    return ir_jump_type;
}

ir_math_type_t IRStringToMathOp  (const char* string_begin, size_t string_len)
{
    assert (string_begin != NULL);

    ir_math_type_t ir_math_op = IR_UNKNOWN_MATH_OPERATION;

    for (size_t i = 0; i < sizeof (ir_math_operation_mappings) / sizeof (ir_math_operation_mappings[0]); ++i)
    {
        if (strncmp (string_begin, ir_math_operation_mappings[i].string, string_len) == 0)
        {
            ir_math_op = ir_math_operation_mappings[i].math_operation;
            return ir_math_op;
        }
    }

    return ir_math_op;
}
