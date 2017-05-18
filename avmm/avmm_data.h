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
 * Core entity type
 */
typedef uint32_t entity_t;

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
    AVM_CLASS_SEGMENT = 0x0B, /* A program segment */

    AVM_CLASS_RESERVED = 0xFF /* System-reserved values */
} avm_class_e; 

/**
 * From the AVM_CLASS_RESERVED, some entities have 
 * predefined meanings.
 */
#define ENTITY_INVALID ((uint32_t)(0xFFFFFFFF))



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

/**
 * Common struct for all entity stores
 */
typedef struct {
    char symname[64]; /* Name of this entity */
} class_header_t;

    
/**
 * Storage for error entity
 */
typedef struct {
    class_header_t header; /* Generic common header */
    entity_t segment; /* Entity of segment containing handler */
    entity_t label;   /* Label to jump to */
} class_error_t;

/**
 * Storage for a group entity
 */
typedef struct {
    class_header_t header; /* Generic common header */
    table_t members; /* Table of member entities */
} class_group_t;

/**
 * Storage for a register entity
 *
 * Registers represent u32 values, but handle them 
 * differently from number entities.
 */
typedef struct _class_register_s {
    class_header_t header; /* Generic common header */
    /* If a register can be read, assign a getter */
    uint32_t (*get)(struct _class_register_s reg);
    /* If a register can be written, assign a setter */
    uint32_t (*set)(struct _class_register_s reg, uint32_t value);
} class_register_t;

/**
 * Storage for a buffer entity
 *
 */
typedef struct {
    class_header_t header; /* Generic common header */
    void *buf; /* The actual buffer */
    uint32_t capacity; /* Bytes available in buf */
    uint32_t size; /* How many bytes actually stored */
    uint32_t cursor; /* Position within buffer of cursor */
} class_buffer_t;

/**
 * Storage for a port entity 
 *
 * CLEAN: TODO: Flesh this out more.
 */
typedef struct {
    class_header_t header; /* Generic common header */
    int file; /* File descriptor */
} class_port_t;

/**
 * Storage for a string entity
 */
typedef struct {
    class_header_t header; /* Generic common header */
    char *text;
    uint32_t capacity;
} class_string_t;

/**
 * Storage for a label entity
 */
typedef struct {
    class_header_t header; /* Generic common header */
    uint32_t offset; /* Word offset into this segment's code */
} class_label_t;

/**
 * Storage for a process/thread/core entity
 */
typedef struct {
    class_header_t header; /* Generic common header */
    table_t registers; /* Table of per-core registers */
    table_t stack;     /* Entity stack */
} class_process_t;

/**
 * Storage for a numeric entity 
 */
typdef struct {
    class_header_t header; /* Generic common header */
    uint32_t bitwidth;
    uint32_t lo_bits;
    uint32_t hi_bits;
} class_number_t;

/**
 * Storage for a program segment
 */
typdef struct {
    class_header_t header; /* Generic common header */
    table_t code; /* The program entity stream */
    table_t groups; /* Groups defined in this segment */
    table_t buffers; /* Buffers defined in this segment */
    table_t ports; /* Ports defined in this segment */
    table_t strings; /* Text strings defined in this segment */
    table_t labels; /* Labels defined in this segment */
    table_t numbers; /* Numeric variables */
} class_segment_t;

/**
 * Storage for a virtual machine
 */
typedef struct {
    class_header_t header; /* Generic common header */
    entity_t entrypoint; /* Segment entrypoint */
    table_t interrupts; /* Exception table */
    table_t registers; /* Process-independent registers */
    table_t processes; /* PID/Thread table */
    table_t segments; /* Segments (Seg 0 is bootstrap) */
} avm_t;


#define avmm_entity_name(__entity) \
    ((class_header_t *)__entity)->symname


#endif /* _AVMM_DATA_H_ */
