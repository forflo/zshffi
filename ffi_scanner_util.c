/*
 * Author: Florian Mayer
 */
#include "ffi_scanner_util.h"
#include <stdlib.h>
#include <string.h>

void *make_valueStr(const char *str){
	char *res; 
    res = malloc(sizeof(char) * (strlen(str) + 1));
	strcpy(res, str);
	return res;
}

