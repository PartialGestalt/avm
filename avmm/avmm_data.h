/**************************************************************************//**
 * @file avmm_data.h
 *
 * @brief Define the different entities that the machine can deal with.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMM_DATA_H_
#define _AVMM_DATA_H_

/**
 * Enumerate the supported entity types.  Each type's value defined
 * here is an index into the table of tables.  All opcodes (entities) of
 * each type will have the type as the high 8-bits of the
 * 32-bit opcode value.
 */
typedef enum {
    AVM_CLASS_INSTRUCTION = 0x00, /* Basic OP */
    AVM_CLASS_ERROR = 0x01, /* Errors/exceptions */
    AVM_CLASS_GROUP = 0x02, /* Grouping of other elements */
    AVM_CLASS_REGISTER = 0x03, /* A register of (nearly) arbitrary width */
    AVM_CLASS_BUFFER = 0x04, /* A memory buffer. Seekable. */
    AVM_CLASS_PORT = 0x05, /* An I/O port. (File, socket, etc.) */
    AVM_CLASS_STRING = 0x06, /* A character string reference */
    AVM_CLASS_LABEL = 0x07, /* A named code location (for jumps, gotos, etc.) */
    AVM_CLASS_PROCESS = 0x08, /* A thread ID */
    AVM_CLASS_NUMBER = 0x09, /* A numeric reference (basically an 'int' variable) */
    AVM_CLASS_IMMEDIATE = 0x0A, /* Lower 16 bits are an immediate value. */
} avm_class_e; 


/**
 * Enumerate the supported opcodes
 */
typedef enum {
    AVM_OP_NOP = 0x00000000, 
    AVM_OP_STOR = 0x00010000, 
    AVM_OP_INS = 0x00020000, 
    AVM_OP_GOTO = 0x00030000, 
    AVM_OP_JZ = 0x00040000, 
    AVM_OP_JNZ = 0x00050000, 
    AVM_OP_FORK = 0x00060000, 
    AVM_OP_KILL = 0x00070000, 
    AVM_OP_PUSH = 0x00080000, 
    AVM_OP_POP = 0x00090000, 
    AVM_OP_WIDTH = 0x000A0000, 

    AVM_OP_ADD = 0x00100000, 
    AVM_OP_SUB = 0x00110000, 
    AVM_OP_MUL = 0x00120000, 
    AVM_OP_DIV = 0x00130000, 
    AVM_OP_POW = 0x00140000, 
    AVM_OP_OR = 0x00150000, 
    AVM_OP_AND = 0x00160000, 
    AVM_OP_CMP = 0x00170000, 
    AVM_OP_INC = 0x00180000,
    AVM_OP_DEC = 0x00190000,

    AVM_OP_FILE = 0x00200000, 
    AVM_OP_IN = 0x00210000, 
    AVM_OP_OUT = 0x00220000, 
    AVM_OP_SIZE = 0x00230000,

    AVM_OP_LABEL = 0x00FF0000,
    AVM_OP_INVALID = 0x00FFFFFF
} avm_opcode_t;

#endif /* _AVMM_DATA_H_ */
