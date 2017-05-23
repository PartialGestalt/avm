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


/**
 * Definitions tables
 */
#ifdef _AVMLIB_REGS_C_ 
class_register_t avm_global_regs[] = {
    { {"CS"},NULL,NULL,NULL,NULL},
    {"\0",NULL,NULL,NULL,NULL}
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
          (((class_register_t *)__reg)->get || \
           ((class_register_t *)__reg)->set))


#endif /* _AVMM_REGS_H_ */
