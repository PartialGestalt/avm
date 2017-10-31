/**************************************************************************//**
 * @file avmlib_data.h
 *
 * @brief Data entity utilities.
 *
 * @details Internal objects and entities for general operations.  Most entities
 * require an object.
 *
 * <em>Copyright (C) 2017 Andrew Kephart, all rights reserved</em>
 * */
#ifndef _AVMLIB_DATA_H_
#define _AVMLIB_DATA_H_

#include "avmm_data.h"

/* Debug assist */
void avmlib_dump_seg(avm_t *avm, class_segment_t *seg);

/* Entity operations */
int avmlib_entity_assert_class(entity_t e, int num_checks, ...);
entity_t avmlib_instruction_new(avm_opcode_t op, uint8_t flags, uint8_t argc);
entity_t avmlib_entity_new(avm_class_e class, int table_index);
entity_t avmlib_immediate_new(int64_t val);

/* Object operations */
class_register_t *avmlib_register_new(char *name, 
                                      register_mode_t mode,
                                      intptr_t priv,
                                      void (*reset)(class_register_t *),
                                      uint32_t (*get)(class_register_t *),
                                      uint32_t (*set)(class_register_t *,uint32_t value));

class_number_t *avmlib_number_new(char *name, int width, int64_t value);
class_unresolved_t *avmlib_unresolved_new(char *name);

/**
 * @brief Decode the class of a given entity.
 */
#define avmlib_entity_class(__e) ((((uint32_t)(__e)) >> 24) & 0xFF)

#endif /* _AVMLIB_DATA_H_ */
