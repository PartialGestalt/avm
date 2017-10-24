/**************************************************************************//**
 * @file avmm_regs.h
 *
 * @brief Define the different registers that the machine can deal with.
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMM_REGS_H_
#define _AVMM_REGS_H_

#include "avmm_data.h"

/**
 * Predefined prototypes
 */
void avmm_regs_init(avm_t *avm);
void avmm_regs_default_reset(class_register_t *reg);
uint32_t avmm_regs_default_get(class_register_t *reg);
uint32_t avmm_regs_default_set(class_register_t *reg, uint32_t value);


/**
 * Definitions tables
 *
 * NULL handlers will resolve to default handlers.
 */
#ifdef _AVMLIB_REGS_C_ 
class_register_t avm_global_regs[] = {
    { {"CLK"},REGMODE_READ,0,NULL,NULL,NULL},
    { {"VER"},REGMODE_READ,0,NULL,NULL,NULL},
    { {"GR0"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR1"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR2"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR3"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR4"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR5"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR6"},REGMODE_RW,0,NULL,NULL,NULL},
    { {"GR7"},REGMODE_RW,0,NULL,NULL,NULL},
    {{"\0"},REGMODE_INVALID}
};
#else
extern class_register_t avm_global_regs[];
#endif

/**
 * Test for validity
 *
 * A register definition is valid if it has:
 * 1. a name, and
 * 2. At least one setter or getter
 */
#define AVM_REG_VALID(__reg) \
    ((((class_register_t *)__reg)->header.symname[0]) && \
          (((class_register_t *)__reg)->mode != REGMODE_INVALID))


#endif /* _AVMM_REGS_H_ */
