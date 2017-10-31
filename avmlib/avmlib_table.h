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
 *    + Table entries may be primitive values, or pointers to other 
 *      memory.
 * */
#ifndef _AVMLIB_TABLE_H_
#define _AVMLIB_TABLE_H_

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * Define the default table increment size.
 */
#define AVMLIB_DEFAULT_TABLE_SIZE 16

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
    int (*add)(struct _table_s *tbl, entry_t entry); 
        /* Compare an entry to a given value */ 
    int (*compare)(struct _table_s *tbl, entry_t left, intptr_t test);
        /* Find an entry (index) from a given value */ 
    int (*find)(struct _table_s *tbl, intptr_t test); 
        /* Destroy an entry on table destruction */
    void (*destroy)(struct _table_s *tbl, entry_t entry);
        /* Serialize an entry to a buffer */
    int (*serialize)(struct _table_s *tbl, entry_t entry, void *binbuf, int binsize);
        /* Deserialize an entry from a buffer */
    int (*deserialize)(struct _table_s *tbl, entry_t *entry, void *binbuf, int binsize);
} table_t;

#define NULL_TABLE (table_t *)(NULL)

/* Prototypes */
    /* default handlers */
int avmlib_table_default_add(table_t *tbl, entry_t entry);
int avmlib_table_default_compare(table_t *tbl, entry_t entry, intptr_t test);
int avmlib_table_default_find(table_t *tbl, intptr_t test);
int avmlib_table_default_serialize(table_t *tbl, entry_t entry, void *binbuf,int binsize);
int avmlib_table_default_deserialize(table_t *tbl, entry_t *entry, void *binbuf,int binsize);
    /* Core API */
table_t *avmlib_table_init(table_t *tbl, int initial_capacity);
table_t *avmlib_table_new(int initial_capacity);
void     avmlib_table_clear(table_t *tbl);
void     avmlib_table_destroy(table_t *tbl);
    /* Wrappers */
int avmlib_table_add_wrapper(table_t *tbl, entry_t entry);
int avmlib_table_find_wrapper(table_t *tbl, intptr_t test);
int avmlib_table_serialize_wrapper(table_t *tbl, entry_t entry, void *binbuf, int binsize);
int avmlib_table_deserialize_wrapper(table_t *tbl, entry_t *entry, void *binbuf, int binsize);

#define avmlib_table_add(__tbl,__entry) \
    avmlib_table_add_wrapper((__tbl),((entry_t)__entry)) 

#define avmlib_table_find(__tbl,__test) \
    avmlib_table_find_wrapper(__tbl,(intptr_t)__test)

#define avmlib_table_size(__tbl) \
    (((table_t *)(__tbl))->size)

#define avmlib_table_serialize(__tbl, __entry, __buf, __bufsz) \
    avmlib_table_serialize_wrapper((__tbl), ((entry_t)__entry), ((void *)__buf, (int)__bufsz))

#define avmlib_table_deserialize(__tbl, __entryp, __buf, __bufsz) \
    avmlib_table_serialize_wrapper((__tbl), ((entry_t *)__entryp), ((void *)__buf, (int)__bufsz))

/**************************************************************************//**
 * @brief Determine if a key is in a table already
 *
 * @details
 *
 * @param
 *
 * @returns true if the table contains the key, false otherwise
 *
 * @remarks
 * */
#define avmlib_table_contains(__tbl,__test) \
    (0 <= avmlib_table_find((__tbl),(__test)))

#endif /* _AVMLIB_TABLE_H_ */
