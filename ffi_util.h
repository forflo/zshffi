#ifndef UTIL
#define UTIL

#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"

const char *TYPE_STRING_TAB[];
const char *OPERATION_STRING_TAB[];
const char *NONTERMINAL_STRING_TAB[];

void emit_human(struct ffi_instruction_obj *ins);
enum dstru_types ffi_dstru_bridge(enum type stype);

#endif /* UTIL */
