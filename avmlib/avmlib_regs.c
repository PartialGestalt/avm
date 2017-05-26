/**************************************************************************//**
 * @file avmlib_regs.c
 *
 * @brief Register handling implementation
 *
 * @details
 * <em>Copyright (C) 2017, Andrew Kephart.  All rights reserved.</em>
 * */

#ifndef _AVMLIB_REGS_C_
#define _AVMLIB_REGS_C_

#include "avmlib.h"

/**************************************************************************//**
 * @brief Register comparison function
 *
 * @details This method compares a test value to an entry in the table.
 *
 * @param this The table containing the entry for comparison
 * @param left The entry to check
 * @param test The test value.
 *
 * @returns Zero if a match is found, nonzero otherwise.
 *
 * @remarks Register comparison goes by entity name.  Test should be a string.
 * */
int
avmlib_reg_compare(
    table_t *this,
    entry_t left,
    intptr_t test
) 
{
    char *testname = (char *)test;

    if (!strcmp(avmm_entity_name(left),testname)) return 0;
    return -1;
}

/**************************************************************************//**
 * @brief Table handler for destroying a register.
 *
 * @details This method handles the specifics of releasing resources
 * associated with a given register.
 *
 * @param this Table containing registers
 * @param entry The register to release.
 *
 * @remarks CLEAN: TODO: This may be difficult. Implement me.
 * */
void
avmlib_reg_destroy(
    table_t *this,
    entry_t entry
)
{
    free((void *)entry);
    return;
}

/**************************************************************************//**
 * @brief
 *
 * @details Base register initializations.
 *
 * @param avm The machine whose regs we should init.
 *
 * @returns Result code indicating success or failure mode
 *
 * @remarks This only initializes the global registers, since the
 * per-core (process-local) registers are initialized with the
 * process.
 * */
void
avmlib_regs_init(
    avm_t *avm
)
{
    int i;
    table_t *regs = AVM_CLASS_TABLE(avm,AVM_CLASS_REGISTER);

    /* Step 1: Clear table */
    avmlib_table_clear(regs);

    /* Step 2: Set custom table handlers */
    regs->compare = avmlib_reg_compare;
    regs->destroy = avmlib_reg_destroy;

    /* Step 3: Load from defs */
    for (i=0;AVM_REG_VALID(&avm_global_regs[i]);i++) {
        avmlib_table_add(regs,&avm_global_regs[i]);
    }
}

#endif /* _AVMLIB_REGS_C_ */
