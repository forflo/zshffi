#ifndef UTIL
#define UTIL

void emit_human(struct ffi_instruction ins[], int cnt);
static char *get_non_terminal(int val);
char *get_stype_string(int val);
char *get_operation_string(int val);

#endif /* UTIL */
