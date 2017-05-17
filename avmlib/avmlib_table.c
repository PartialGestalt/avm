/**************************************************************************//**
 * @file avmlib_table.c
 *
 * @brief Implementation bits for "simple" table API
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_TABLE_C_ 
#define _AVMLIB_TABLE_C_

#include "avmlib_table.h"

/**************************************************************************//**
 * @brief Provide a default 'add' method.
 *
 * @details This method adds a new entry to the table, expanding the table
 * entry array if necessary.
 *
 * @param this The table which will receive the new entry
 * @param entry The entry to add
 *
 * @returns On success, returns the index of the new entry; -1 on failure.
 *
 * @remarks
 * */
static int 
avmlib_table_default_add(
    table_t *this,
    entry_t entry
)
{
    /* Realloc if needed */ 
    if (this->size >= this->capacity) { 
        entry_t *newdata = realloc(this->entries,sizeof(entry_t) * (this->size + this->alloc_count)); 
        if (!newdata) return -1; 
        this->entries = newdata; 
    } 
    /* There's enough space for this one. */ 
    this->entries[this->size] = entry; 
    this->size++; 
    return (this->size - 1); 
}


/**************************************************************************//**
 * @brief Provide a default 'compare' method.
 *
 * @details This method compares the values as integers, which is
 * pretty much useless.  API consumers should provide a table-specific
 * comparison function.
 *
 * @param this The table in which the comparison occurs
 * @param entry An entry from the table
 * @param test A value to use for comparison.
 *
 * @returns 0 if the given entry is the same as the reference, nonzero
 * if not.
 *
 * @remarks Note that user functions will be less direct, typically 
 * comparing a key to elements within an entry.
 * */
static int
avmlib_table_default_compare(
    table_t *this,
    entry_t entry,
    intptr_t test
) 
{
    if (entry == test) return 0;
    return -1;
}

/**************************************************************************//**
 * @brief Provide a default 'find' method.
 *
 * @details This method naively walks the array front to back, calling
 * the comparison routine each time.
 *
 * @param this The table in which the comparison occurs
 * @param test A value to use for comparison.
 *
 * @returns index into the table of the matching entry, or -1 if not found.
 * if not.
 *
 * @remarks Note that user functions should be a lot smarter about this.
 * */
static int
avmlib_table_default_find(
    table_t *this,
    intptr_t test
)
{
    int i;
    for (i=0;i<this->size;i++) {
        if (0 == this->compare(this,this->entries[i],test)) {
            return i;
        }
    }
    /* Not found. */
    return -1;
}

/**************************************************************************//**
 * @brief Initialize a table for use.
 *
 * @details This method clears and prepares internal fields as 
 * necessary to prepare a table for use.
 *
 * @param this The table to prepare
 * @param initial_capacity How many entries to initially allocate for.
 *
 * @returns Pointer to the table on succes, NULL on failure.
 * */
table_t *
avmlib_table_init(
    table_t *this,
    int initial_capacity
)
{
    entry_t *entries;

    /* Step 1: sanity check */
    if (NULL_TABLE == this) return NULL;

    /* Step 2: Attempt initial allocation */
    if (NULL == (entries = calloc(initial_capacity,sizeof(entry_t)))) {
        return NULL;
    }

    /* Step 3: basic field assignments */
    this->size = 0;
    this->capacity = initial_capacity;
    this->alloc_count = 1;  /* May be overridden by users */
    this->type_name = "AnonymousTable"; /* May be overridden by users */
    this->entries = entries;
    this->add = avmlib_table_default_add;
    this->compare = avmlib_table_default_compare;
    this->find = avmlib_table_default_find;
    this->destroy = NULL;

    return this;
}


/**************************************************************************//**
 * @brief Alloc and prepare a new table struct.
 *
 * @details
 *
 * @param initial_capacity Initial table capacity
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
table_t *
avmlib_table_new(
    int initial_capacity
)
{
    table_t *newtable;
    /* Step 1: Sanity check */
    if (initial_capacity <= 0) return NULL;

    /* Step 2: Struct alloc */
    if (NULL == (newtable = malloc(sizeof(table_t)))) {
        return NULL;
    }

    /* Step 3: Init */
    if (NULL == avmlib_table_init(newtable,initial_capacity)) {
        /* Something failed. */
        free(newtable);
        return NULL;
    }

    return newtable;

}

/**************************************************************************//**
 * @brief Remove all entries from a table and reset.
 *
 * @details This function clears all entries from a table, but doesn't
 * free/realloc the entry area itself.
 *
 * @param this The table to clear
 *
 * */
void
avmlib_table_clear(
    table_t *this
)
{
    int i;
    /* Step 1: Individual entries */
    if (this->destroy) {
        for (i=0;i<this->size;i++) {
            this->destroy(this,this->entries[i]);
        }
    }

    /* Step 2: Reset some parameters */
    this->size = 0;
}

/**************************************************************************//**
 * @brief Destroy a table and release resources.
 *
 * @details This method attempts to release any resources associated with
 * a table, first by releasing each entry with the provided destructor
 * (if set), then by releasing the table memory itself.
 *
 * @param this The table to destroy
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks
 * */
void
avmlib_table_destroy(
    table_t *this
)
{
    int i;
    if (!this) return;

    /* Step 1: Individual entries */
    if (this->destroy) {
        for (i=0;i<this->size;i++) {
            this->destroy(this,this->entries[i]);
        }
    }

    /* Step 2: Entries array and table itself */
    this->capacity = this->size = 0;
    free(this->entries);
    free(this);
}

#endif /* _AVMLIB_TABLE_C_ */
