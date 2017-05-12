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
 * Enumerate the supported entity types.  Each type's value defined
 * here is an index into the table of tables.  All opcodes (entities) of
 * each type will have the type as the high 8-bits of the
 * 32-bit opcode value.
 */
typedef enum {
    FEP_CLASS_INSTRUCTION = 0x00, /* Basic OP */
    FEP_CLASS_ERROR = 0x01, /* Errors/exceptions */
    FEP_CLASS_GROUP = 0x02, /* Grouping of other elements */
    FEP_CLASS_REGISTER = 0x03, /* A register of (nearly) arbitrary width */
    FEP_CLASS_BUFFER = 0x04, /* A memory buffer. Seekable. */
    FEP_CLASS_PORT = 0x05, /* An I/O port. (File, socket, etc.) */
    FEP_CLASS_STRING = 0x06, /* A character string reference */
    FEP_CLASS_LABEL = 0x07, /* A named code location (for jumps, gotos, etc.) */
    FEP_CLASS_PROCESS = 0x08, /* A thread ID */
    FEP_CLASS_NUMBER = 0x09, /* A numeric reference (basically an 'int' variable) */
    FEP_CLASS_IMMEDIATE = 0x0A, /* Lower 16 bits are an immediate value. */
} fep_class_e; 


/**
 * Enumerate the supported opcodes
 */
typedef enum {
    FEP_OP_NOP = 0x00000000, 
    FEP_OP_STOR = 0x00010000, 
    FEP_OP_INS = 0x00020000, 
    FEP_OP_GOTO = 0x00030000, 
    FEP_OP_JZ = 0x00040000, 
    FEP_OP_JNZ = 0x00050000, 
    FEP_OP_FORK = 0x00060000, 
    FEP_OP_KILL = 0x00070000, 
    FEP_OP_PUSH = 0x00080000, 
    FEP_OP_POP = 0x00090000, 
    FEP_OP_WIDTH = 0x000A0000, 

    FEP_OP_ADD = 0x00100000, 
    FEP_OP_SUB = 0x00110000, 
    FEP_OP_MUL = 0x00120000, 
    FEP_OP_DIV = 0x00130000, 
    FEP_OP_POW = 0x00140000, 
    FEP_OP_OR = 0x00150000, 
    FEP_OP_AND = 0x00160000, 
    FEP_OP_CMP = 0x00170000, 
    FEP_OP_INC = 0x00180000,
    FEP_OP_DEC = 0x00190000,

    FEP_OP_FILE = 0x00200000, 
    FEP_OP_IN = 0x00210000, 
    FEP_OP_OUT = 0x00220000, 
    FEP_OP_SIZE = 0x00230000,

    FEP_OP_LABEL = 0x00FF0000,
    FEP_OP_INVALID = 0x00FFFFFF
} fep_opcode_t;

#endif /* _FEPVM_DATA_H_ */
