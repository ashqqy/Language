#ifndef IR_GENERATE_H
#define IR_GENERATE_H

#include "ast.h"

const int INPUT_FUNCTION_IDENTIFIER = -1;
const int PRINT_FUNCTION_IDENTIFIER = -2;

void IRGenerate (const char* ir_file_name, ast_node_t* ast_root);
void ParseAstNode (FILE* ir_file, ast_node_t* current_node);

void GenerateConditionalJump (FILE* ir_file, ast_node_t* node);

#endif // IR_GENERATE_H



// #include "linked-list.h"

// typedef enum ir_operator
// {
//     IR_NOP,

//     IR_MOV,
//     IR_PUSH,
//     IR_POP,

//     IR_ADD,
//     IR_SUB,
//     IR_IMUL,

//     IR_CMP,
//     IR_JMP,
//     IR_JE,

//     IR_CALL,
//     IR_RET,

//     IR_IN,
//     IR_OUT,   
//     IR_HLT,  
//     IR_SYSCALL
// } ir_operator_t;

// typedef enum ir_register
// {
//     RAX = 0,
//     RBX = 1,
//     RCX = 2,
//     RDX = 3,
//     RSI = 4,
//     RDI = 5,
//     RBP = 6,
//     RSP = 7,
//     R8  = 8,
//     R9  = 9,
//     R10 = 10,
//     R11 = 11,
//     R12 = 12,
//     R13 = 13,
//     R14 = 14,
//     R15 = 15,
// } ir_register_t;

// typedef struct ir_operand_type
// {
//     REG,
//     REG_MEM,
//     IMM,
//     IMM_MEM,
//     REG_ADD_IMM_MEM,
//     REG_SUB_IMM_MEM,
//     LABEL,
//     FUNCTION_INDEX
// } ir_operand_type_t;

// typedef struct ir_operand
// {
//     ir_operand_type_t type;
    
//     ir_register_t reg;            // type = ( REG | REG_MEM  | REG_ADD_IMM_MEM | REG_SUB_IMM_MEM )
//     int           immediate;      // type = ( IMM | IMM_MEM, | REG_ADD_IMM_MEM | REG_SUB_IMM_MEM )
//     char*         label;          // type = LABEL
//     int           function_index; // type = FUNC_INDEX
// } ir_operand_t;

// typedef struct ir_basic_block
// {
//     char* label;
//     int current_function_index;

//     list_t instructions;
// } ir_basic_block_t;

// typedef struct ir_instruction
// {
//     ir_operator_t operator;

//     int operand_count;
//     ir_operand_t first_operand;
//     ir_operand_t second_operand;
// } if_instruction_t;

// typedef struct ir
// {
    
// } ir_t;
