#ifndef UTIL
#define UTIL

#include "ffi_generate_ops.h"

const char *TYPE_STRING_TAB[];
const char *OPERATION_STRING_TAB[];
const char *NONTERMINAL_STRING_TAB[];

void emit_human(struct ffi_instruction **ins);

#endif /* UTIL */
