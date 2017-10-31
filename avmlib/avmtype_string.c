/**************************************************************************//**
 * @file avmtype_string.c
 *
 * @brief Handle string-related operations
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMTYPE_STRING_C_
#define _AVMTYPE_STRING_C_

#include "avmlib.h"
#include <errno.h>


/**************************************************************************//**
 * @brief Create a string object from parameters
 *
 * @details
 *
 * @param
 *
 * @returns New string object on success, NULL on failure.
 *
 * @remarks
 * */
class_string_t *
avmtype_string_new(
    char *name,
    char *value
)
{
    /* Base declaration/alloc */
    class_string_t *obj = calloc(1,sizeof(*obj));
    char *val = strdup(value!=NULL?value:"");
    if ((NULL == obj) || (NULL == val)) {
        if (NULL != obj) free(obj);
        if (NULL != val) free(val);
        return NULL;
    }

    /* Save name */
    if (name) strncpy(obj->header.symname, name, sizeof(obj->header.symname));
    obj->text = val;
    obj->capacity = strlen(val);
    return obj;
}

/**************************************************************************//**
 * @brief serialize a string object into a buffer
 *
 * @details  This method copies a string table entry into a binary buffer
 *
 * @param tbl The table this entry is serialized from
 * @param entry The entry to serialize
 * @param binbuf Start of area into which we'll serialize
 * @param binsize Number of bytes available in binbuf
 *
 * @returns Number of bytes serialized into binbuf, or -1 on error.
 *
 * @remarks This assumes NULL-terminated strings.
 * */
int
avmtype_string_serialize(
    table_t *tbl,
    entry_t entry,
    void *binbuf,
    int binsize
)
{
    char *p = binbuf;
    class_string_t *s = (class_string_t *)entry;
    int need = 2; 
    char *n = avmm_entity_name(s);

    /* Step 1: Size needed? */
    if (n && *n) {
        need += strlen(n);
    }
    if (s->text && *(s->text)) {
        need += strlen(s->text);
    }
    if (need > binsize) {
        errno = ENOBUFS;
        return -1;
    }

    /* Step 2: Copyin */
    memset(binbuf,0,need);
    if (n && *n) {
        strcpy(p,n);
        p+=strlen(n);
    }
    if (s->text && *(s->text)) {
        strcpy(p,s->text);
    }
    return need;
}

#endif /* _AVMTYPE_STRING_C_ */
