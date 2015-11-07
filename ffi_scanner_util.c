#include "ffi_scanner_util.h"
#include <stdlib.h>
#include <string.h>

/* snips off first and last char before duplication */
struct token_value *make_valueStr(const char *str){
    struct token_value *v = malloc(sizeof(struct token_value));
	char *res = malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(res, ++str);
    res[strlen(res) - 1] = '\0';
    v->value = res;
    v->length = strlen(res);
	return v;
}
