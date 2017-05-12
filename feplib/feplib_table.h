/**************************************************************************//**
 * @file feplib_table.h
 *
 * @brief Header definitions and declarations for the FEP table API.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 *
 * A feplib table is a dynamic array of entries of a user-defined
 * type. 
 *    + Table data may be directly indexed.
 *    + Tables may be serialized to, or deserialized from, memory.
 *    + Table entries may be added, but not removed (the data an entry
 *      refers to may be cleared, of course).  
 * */
#ifndef _FEPLIB_TABLE_H_
#define _FEPLIB_TABLE_H_

#include <string.h>
#include <stdlib.h>

/**
 * Generic reference type 
 */
typedef void * table_t;

#define NULL_TABLE (table_t)(NULL)

/**
 * Declare a new table type.
 *
 * @param __name Name token for this table type.  Short, no spaces.
 * @param __entry_type Type of each item in the entry array.
 *
 * E.g. NEW_TABLE_TYPE(address,uint16_t);  
 * defines _table_address_t type, * where the entry array 
 * is uint16_t entries[];
 */
#define TABLE_TYPE_DECLARE(__name,__entry_type) \
typedef struct _table_##__name##_s { \
    /* Properties */ \
    uint32_t size; /* How many entries in table */ \
    uint32_t capacity; /* How many available slots in table */ \
    uint32_t alloc_count; /* How much to expand by on each expansion. */\
    const char *type_name; /* Name of this table type */ \
    uint32_t entry_size; /* Bytes per entry */ \
    /* Entry array */ \
    __entry_type *entries; \
    /* Methods */ \
        /* Add an entry to the table */ \
    int (*add)(table_t self, __entry_type entry); \
        /* Compare an entry to a given value */ \
    int (*compare)(table_t self, __entry_type left, intptr_t test);\
        /* Find an entry (index) from a given value */ \
    int (*find)(table_t self, intptr_t test); \
} _table_##__name##_t; \
\
/* Default function for adding a new entry.  */ \
/* On success, returns the entry's new index. */ \
/* On failure, returns -1. */ \
static int \
_table_##__name##_default_add( \
    table_t self, \
    __entry_type entry \
) \
{ \
    _table_##__name##_t *tbl = (_table_##__name##_t *)self; \
    /* Realloc if needed */ \
    if (tbl->size >= tbl->capacity) { \
        __entry_type *newdata = realloc(tbl->entries,sizeof(__entry_type) * (tbl->size + tbl->alloc_count)); \
        if (!newdata) return -1; \
        tbl->entries = newdata; \
    } \
    /* There's enough space for this one. */ \
    tbl->entries[tbl->size] = entry; \
    tbl->size++; \
    return (tbl->size - 1); \
} \
\
/* Default function for comparing. */ \
/* Returns 0 if a match, nonzero otherwise. */\
static int \
_table_##__name##_default_compare( \
    table_t self, \
    __entry_type entry, \
    intptr_t test \
) \
{ \
    /* This default is useless, and only exists to */\
    /* prevent NULL function pointers.  For finding */\
    /* or comparison, the caller MUST update this. */\
    if ((void *)&(entry) == (void *)test) return 0; \
    return 1; \
} \
\
/* Default function for finding an entry. */ \
/* Returns index of match on success, -1 on failure. */\
static int \
_table_##__name##_default_find( \
    table_t self, \
    intptr_t test \
) \
{ \
    _table_##__name##_t *tbl = (_table_##__name##_t *)self; \
    /* Naively walk. */ \
    int i; \
    for (i=0;i<(tbl->size);i++) { \
        if (0 == tbl->compare(tbl,tbl->entries[i],test)) { \
            return i; \
        } \
    } \
    /* Not found. */ \
    return -1; \
} \
\
    /* Initialize a table struct.  Returns the initialized table */ \
    /* on success, or NULL on some kind of failure.              */ \
static _table_##__name##_t * \
_table_##__name##_init( \
    _table_##__name##_t *tbl, \
    int capacity \
) \
{ \
    if (!(tbl) || (capacity <= 0))  { \
        return NULL; \
    } \
    (tbl)->size = 0; \
    (tbl)->capacity = (capacity); \
    (tbl)->alloc_count = 1; \
    (tbl)->type_name = strdup((#__name)); \
    (tbl)->entry_size = sizeof(__entry_type); \
    (tbl)->entries = calloc((capacity),sizeof(__entry_type)); \
    (tbl)->add = _table_##__name##_default_add; \
    (tbl)->compare = _table_##__name##_default_compare; \
    (tbl)->find = _table_##__name##_default_find; \
         /* All setup.  Return success. */ \
    return (tbl); \
}  \
    /* Allocator for tables of this type. */ \
static _table_##__name##_t * \
_table_##__name##_create( \
    int capacity \
) \
{ \
    /* Simple alloc */ \
    _table_##__name##_t *tbl = calloc(1,sizeof(_table_##__name##_t)); \
    if (!tbl) return NULL; \
    if (NULL == _table_##__name##_init(tbl,capacity)) { \
        /* Failed? */ \
        free(tbl);  \
        return NULL; \
    } \
    return tbl; \
} 


/**
 * Shorthand for core table struct.
 */
#define TABLE_TYPE(__typename) _table_##__typename##_t

/**
 * Simple declaration of a table (must be initialized before use)
 */
#define TABLE_DECLARE(__typename,__varname)  \
    TABLE_TYPE(__typename) __varname 

/**
 * Initialize a previously-declared table
 */
#define TABLE_INIT(__typename, __varname, __capacity)  \
    _table_##__typename##_init((__varname),(__capacity))

/**
 * Allocate and prepare a new table struct.  Must be in code, not
 * declarations.
 */
#define TABLE_NEW(__typename, __capacity) \
    _table_##__typename##_create(__capacity)


/**
 * Wrapper for table search.
 */
#define TABLE_FIND(__tbl,__test) \
    (__tbl)->find((__tbl),(intptr_t)(__test))

/**
 * Wrapper for table add
 */
#define TABLE_ADD(__tbl,__entry) \
    (__tbl)->add((__tbl),__entry)


#endif /* _FEPLIB_TABLE_H_ */
