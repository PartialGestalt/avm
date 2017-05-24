/**************************************************************************//**
 * @file avmlib.h
 *
 * @brief Common header for avmlib files
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_H_
#define _AVMLIB_H_

#include "avmm_data.h"
#include "avmlib_regs.h"
#include "avmlib_ports.h"
#include "avmlib_table.h"
#include "avmlib_machine.h"
#include "avmlib_log.h"

#define avmlib_log(__format_and_args...) \
    avm_log("AVMLIB", __format_and_args)

#define avmlib_err(__format_and_args...) \
    avm_err("AVMLIB", __format_and_args)


#endif /* _AVMLIB_H_ */

