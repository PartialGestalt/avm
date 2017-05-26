/**************************************************************************//**
 * @file avmlib_utils.c 
 *
 * @brief Support and utility functions for AVM
 *
 * @details
 * <em>Copyright (C) 2017, Andrew kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_UTILS_C_
#define _AVMLIB_UTILS_C_

#include "avmlib.h"
#include <inttypes.h>


/**************************************************************************//**
 * @brief Convert a string to a 32-bit number
 *
 * @details This method supports numeric conversion in decimal (no prefix),
 * binary ("0b" prefix), hexadecimal ("0x" prefix), or octal ("0" prefix)
 *
 * @param str The string token containing the number
 *
 * @returns Number of successfully converted characters. 
 *
 * @remarksa
 * */
int
avmlib_getnum(
    char *str,
    uint32_t *num
)
{
    int len, digit;
    int radix = 10;
    char *p = str;
    uint32_t accum = 0;
    int retval = 0;

    /* Sanity check */
    if (!str || !num) return -1;

    /* Get length - need at least 1 char */
    if (0 >= (len = strnlen(str,64))) return -1;

    /* Determine radix */
    if ((str[0] == '0') && (len >= 2)) {
        switch (str[1]) {
            /* Check for hexadecimal prefix */
            case 'x':
            case 'X':
                radix = 16;
                p+=2;
                break;
            /* Check for binary prefix */
            case 'b':
            case 'B':
                radix = 2;
                p+=2;
                break;
            /* Valid octal? */
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
                radix = 8;
                p+=1;
                break;
            default:
                /* Invalid prefix */
                return -1;
        }
    }

    /* Walk it */
    while (*p) {
        digit = avmlib_digitval(*p);
        if ((0 > digit) || (digit >= radix)) {
            /* Hit an unsupported digit. Break out */
            goto _avmlib_getnum_out;
        } 
        retval++; /* Got an(other) good one */
        accum = (accum * radix) + digit;
    }

_avmlib_getnum_out:
    *num = accum;
    return retval;

}

#endif /* _AVMLIB_UTILS_C_ */
