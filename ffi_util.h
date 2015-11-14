#ifndef UTIL
#define UTIL

#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"
#include "ffi_offset_table.h"

const char *TYPE_STRING_TAB[];
const char *OPERATION_STRING_TAB[];
const char *NONTERMINAL_STRING_TAB[];

void emit_human(struct ffi_instruction_obj *ins);
enum dstru_types ffi_dstru_bridge(enum type stype);
int emit_human_otbl(struct offset_table *tbl, int tabs);

#endif /* UTIL */
