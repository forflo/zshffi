#ifndef SCANNER_UTIL
#define SCANNER_UTIL

#include "ffi_parser_util.h"

/* Creates a new value object with a string as content
 	it manually allocates memory for the contained string
 	Param: A valid pointer
 	Return: A valid pointer or NULL on failure */
void *make_valueStr(const char *str);

#endif /* SCANNER_UTIL */
