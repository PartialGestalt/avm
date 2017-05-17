/**************************************************************************//**
 * @file avmlib_table.h
 *
 * @brief Header definitions and declarations for the AVM table API.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 *
 * An avmlib table is a dynamic array of entries of a user-defined
 * type. 
 *    + Table data may be directly indexed.
 *    + Tables may be serialized to, or deserialized from, memory.
 *    + Table entries may be added, but not removed (the data an entry
 *      refers to may be cleared, of course).  
 * */
#ifndef _AVMLIB_TABLE_H_
#define _AVMLIB_TABLE_H_

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * Table entry
 *
 * Each table entry is either an integer, or a pointer to
 * some entry structure. Define a simple cast target.
 */
typedef intptr_t entry_t;

/**
 * Overall table type
 */
typedef struct _table_s {
    /* Properties */ 
    uint32_t size; /* How many entries in table */ 
    uint32_t capacity; /* How many available slots in table */ 
    uint32_t alloc_count; /* How much to expand by on each expansion. */
    const char *type_name; /* Name of this table type */ 
    /* Entry array */ 
    entry_t *entries; 
    /* Methods */ 
        /* Add an entry to the table */ 
    int (*add)(struct _table_s *this, entry_t entry); 
        /* Compare an entry to a given value */ 
    int (*compare)(struct _table_s *this, entry_t left, intptr_t test);
        /* Find an entry (index) from a given value */ 
    int (*find)(struct _table_s *this, intptr_t test); 
        /* Destroy an entry on table destruction */
    void (*destroy)(struct _table_s *this, entry_t entry);
} table_t;

#define NULL_TABLE (table_t *)(NULL)

/* Prototypes */
table_t *avmlib_table_init(table_t *this, int initial_capacity);
table_t *avmlib_table_new(int initial_capacity);
void     avmlib_table_clear(table_t *this);
void     avmlib_table_destroy(table_t *this);

/**
 * Wrapper for adding an entry to the table.
 * Mostly just for the cast.
 */
#define avmlib_table_add(__tbl,__entry) \
    (__tbl)->add((__tbl),((entry_t)(__entry)))

#define avmlib_table_find(__tbl,__test) \
    (__tbl)->find((__tbl),((entry_t)(__test)))

#endif /* _AVMLIB_TABLE_H_ */
