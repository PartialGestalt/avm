/**************************************************************************//**
 * @file avmlib_log.c 
 *
 * @brief Log implementation
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_LOG_C_
#define _AVMLIB_LOG_C_ 

#include "avmlib_log.h"

/*
 * Configured targets
 */
FILE *avmlib_logfile = NULL;
FILE *avmlib_errfile = NULL;

/**************************************************************************//**
 * @brief Set the current error target
 *
 * @details This method simply assigns the defined output to the
 * given file, unless an error occurs
 *
 * @param newtarget The FILE that error messages should now go to.
 *
 * @returns Configured output target, which will be newtarget on success,
 * or the unchanged previous target.
 *
 * @remarks Callers can check success/failure by comparing the return value
 * to the provided argument.
 * */
FILE *
avmlib_log_set_err(
    FILE *newtarget
)
{
    if (!newtarget) {
        avm_err("INTERNAL", "Can't set log target to NULL.\n");
    } else {
        avmlib_errfile = newtarget;
    }
    return avmlib_errfile;
}

/**************************************************************************//**
 * @brief Set the current log target
 *
 * @details This method simply assigns the defined output to the
 * given file, unless an error occurs
 *
 * @param newtarget The FILE that log messages should now go to.
 *
 * @returns Configured output target, which will be newtarget on success,
 * or the unchanged previous target.
 *
 * @remarks Callers can check success/failure by comparing the return value
 * to the provided argument.
 * */
FILE *
avmlib_log_set_log(
    FILE *newtarget
)
{
    if (!newtarget) {
        avm_err("INTERNAL", "Can't set log target to NULL.\n");
    } else {
        avmlib_logfile = newtarget;
    }
    return avmlib_logfile;
}


#endif /* _AVMLIB_LOG_C_ */
