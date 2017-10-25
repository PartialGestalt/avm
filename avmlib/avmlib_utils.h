/**************************************************************************//**
 * @file avmlib_utils.h
 *
 * @brief Provide some utility functions to AVM
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMLIB_UTILS_H_
#define _AVMLIB_UTILS_H_

/**************************************************************************//**
 * @brief Return an integer value of a common digit character
 * */
static inline int avmlib_digitval(char c) {
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'A') && (c <= 'F')) return c - 'A';
    if ((c >= 'a') && (c <= 'f')) return c - 'a';
    return -1; /* Invalid value */
}

/* Prototypes */
int avmlib_getnum(char *str, int64_t *num);

#endif /* _AVMLIB_UTILS_H_ */
