/**************************************************************************//**
 * @file fepvm_data.h
 *
 * @brief Define the different entities that the machine can deal with.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _FEPVM_DATA_H_
#define _FEPVM_DATA_H_

/**
 * Enumerate the supported types.  Each type's value defined
 * here is an index into the table of tables.  All opcodes of
 * each type will have the type as the high 8-bits of the
 * 32-bit opcode value.
 */
typedef enum {
    FEP_INSTRUCTION = 0x00, /* Basic OP */
    FEP_ERROR = 0x01, /* Errors/exceptions */
    FEP_GROUP = 0x02, /* Grouping of other elements */
    FEP_REGISTER = 0x03, /* A register of (nearly) arbitrary width */
    FEP_BUFFER = 0x04, /* A memory buffer. Seekable. */
    FEP_PORT = 0x05, /* An I/O port. (File, socket, etc.) */
    FEP_STRING = 0x06, /* A character string */
    FEP_LABEL = 0x07, /* A named code location (for jumps, gotos, etc.) */
    FEP_PROCESS = 0x08, /* A thread ID */
    FEP_NUMBER = 0x09, /* A numeric value (basically an 'int' variable) */
    FEP_IMMEDIATE = 0x0A, /* Lower 16 bits are an immediate value. */
} fep_types_e;


/**
 * Enumerate the supported opcodes
 */
typedef enum {
    FEP_OP_NOP = 0x00000000, 
    FEP_OP_DEF = 0x00000001, 
    FEP_OP_STOR = 0x00000002, 
    FEP_OP_INS = 0x00000003, 
    FEP_OP_GOTO = 0x00000004, 
    FEP_OP_JZ = 0x00000005, 
    FEP_OP_JNZ = 0x00000006, 
    FEP_OP_FORK = 0x00000007, 
    FEP_OP_KILL = 0x00000008, 
    FEP_OP_PUSH = 0x00000009, 
    FEP_OP_POP = 0x0000000A, 
    FEP_OP_WIDTH = 0x0000000B, 

    FEP_OP_ADD = 0x00000010, 
    FEP_OP_SUB = 0x00000011, 
    FEP_OP_MUL = 0x00000012, 
    FEP_OP_DIV = 0x00000013, 
    FEP_OP_POW = 0x00000014, 
    FEP_OP_OR = 0x00000015, 
    FEP_OP_AND = 0x00000016, 
    FEP_OP_CMP = 0x00000017, 
    FEP_OP_FILE = 0x00000020, 
    FEP_OP_IN = 0x00000021, 
    FEP_OP_OUT = 0x00000022, 
    FEP_OP_SIZE = 0x00000023 
} fep_opcode_t;

#endif /* _FEPVM_DATA_H_ */
