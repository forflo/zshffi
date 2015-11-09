#include "ffi_util.h"
#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"
#include "ffi_generate_ops.h"
#include <stdio.h>

#define DEBUG

const char *TYPE_STRING_TAB[] = { LIST_TYPE(GENERATE_STRING) };
const char *OPERATION_STRING_TAB[] = { FFI_BYTECODE(GENERATE_STRING) };
const char *NONTERMINAL_STRING_TAB[] = { LIST_NTYPE(GENERATE_STRING) };

void emit_human(struct ffi_instruction **ins){
    int i;

#ifdef DEBUG
    printf("Insptr: %p\n", ins);
#endif

    for (i=0; ins[i] != NULL; i++)
        if(ins[i]->operation)
            printf("[op: %16s | type: %13s | value: %8s]\n", 
                OPERATION_STRING_TAB[ins[i]->operation],
                TYPE_STRING_TAB[ins[i]->type],
                ins[i]->value);
        else
            printf("[op: %16s | type: %13s | value: %8s]\n", 
                OPERATION_STRING_TAB[ins[i]->operation], "none", "none");
}

/* replaces each escaped character with the real character */
int unescape(char **dest_string, const char *src){
    //TODO

    return 0;
}

