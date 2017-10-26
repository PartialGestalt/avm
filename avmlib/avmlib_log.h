/**************************************************************************//**
 * @file avmlib_log.h
 *
 * @brief Header definitions and declarations for the AVM log API.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 *
 * Logs are just prints to FILE * outputs.  By default, these are
 * stdout (for log messages), and stderr (for error messages).  Either
 * may be updated, however.
 *
 * ERRORS are always sent to the configured error target
 * LOGS are always sent to the configured log target
 * DEBUG messages are sent to the configured log target iff debugging is
 * enabled, and the debug level is at least as high as the message's
 * debug level.
 * */
#ifndef _AVMLIB_LOG_H_
#define _AVMLIB_LOG_H_

#include <stdio.h>

/*
 * DEBUG
 *
 * To turn on debugging, define AVM_DEBUG in your build.
 * To modify the debug level, define AVM_DEBUG_LEVEL in your build.
 */
#ifndef AVM_DEBUG_LEVEL 
#define AVM_DEBUG_LEVEL 3
#endif /* AVM_DEBUG_LEVEL */

#ifdef _AVMLIB_LOG_C_
int avmlib_debug_level = AVM_DEBUG_LEVEL;
#else /* _AVMLIB_LOG_C_ */
extern int avmlib_debug_level;
#endif /* _AVMLIB_LOG_C_ */

/*
 * Output targets
 */
extern FILE *avmlib_logfile;
extern FILE *avmlib_errfile;

/*
 * Prototypes
 */
FILE *avmlib_log_set_log(FILE *newtarget);
FILE *avmlib_log_set_err(FILE *newtarget);


#ifdef AVM_DEBUG
#define avm_set_debuglevel(__lvl) \
    do { \
        avmlib_debug_level = (__lvl); \
    } while (0)

#define avm_dbg(__lvl, __token, __format_and_args...) \
    do { \
        if (avmlib_debug_level >= (__lvl)) { \
            fprintf(avmlib_logfile?avmlib_logfile:stdout, __token ": DEBUG: " __format_and_args); \
        } \
    } while (0)
#else
#define avm_set_debuglevel(__lvl)
#define avm_dbg(__lvl, __token, __format_and_args...)
#endif /* AVM_DEBUG */

#define avm_log(__token, __format_and_args...) \
    fprintf(avmlib_logfile?avmlib_logfile:stdout, __token ": " __format_and_args)

#define avm_err(__token, __format_and_args...) \
    fprintf(avmlib_errfile?avmlib_errfile:stderr, __token ": ERROR: " __format_and_args)

#endif /* _AVMLIB_LOG_H_ */
