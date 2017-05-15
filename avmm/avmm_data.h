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
    AVM_OP_NOP = 0x00,
    AVM_OP_STOR = 0x01,
    AVM_OP_INS = 0x02,
    AVM_OP_GOTO = 0x03,
    AVM_OP_JZ = 0x04,
    AVM_OP_JE = 0x05,
    AVM_OP_JNZ = 0x06,
    AVM_OP_FORK = 0x07,
    AVM_OP_KILL = 0x08,
    AVM_OP_PUSH = 0x09,
    AVM_OP_POP = 0x0A,
    AVM_OP_LABEL = 0x0B,

    AVM_OP_ADD = 0x0C,
    AVM_OP_SUB = 0x0D,
    AVM_OP_MUL = 0x0E,
    AVM_OP_DIV = 0x0F,
    AVM_OP_POW = 0x10,
    AVM_OP_OR = 0x11,
    AVM_OP_AND = 0x12,
    AVM_OP_CMP = 0x13,
    AVM_OP_INC = 0x14,
    AVM_OP_DEC = 0x15,

    AVM_OP_FILE = 0x16,
    AVM_OP_IN = 0x17,
    AVM_OP_OUT = 0x18,
    AVM_OP_SIZE = 0x19,

    AVM_OP_INVALID = 0xFF

} avm_opcode_t;

    


#endif /* _AVMM_DATA_H_ */
