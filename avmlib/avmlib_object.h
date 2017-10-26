/**************************************************************************//**
 * @file avmlib_object.h
 *
 * @brief Header file for various object support.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */
#ifndef _AVMLIB_OBJECT_H_
#define _AVMLIB_OBJECT_H_

#include "avmm_data.h"
#include "avmc_ops.h"

/* Label */
char *avmlib_compile_label(class_segment_t *seg, op_t *op);
class_label_t *avmlib_new_label(char *name, uint8_t target_segment, uint32_t location);

#endif /* _AVMLIB_OBJECT_H_ */
