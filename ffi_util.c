#include "ffi_generate_ops.h"
#include "ffi_util.h"
#include "ffi_node_defines.h"
#include "ffi_generate_ops.h"
#include "ffi_parser_util.h"
#include "ffi_dstru.h"

#include <stdio.h>

#define DEBUG

const char *TYPE_STRING_TAB[] = { LIST_TYPE(GENERATE_STRING) };
const char *OPERATION_STRING_TAB[] = { FFI_BYTECODE(GENERATE_STRING) };
const char *NONTERMINAL_STRING_TAB[] = { LIST_NTYPE(GENERATE_STRING) };

void emit_human(struct ffi_instruction_obj *ins){
    int i;

#ifdef DEBUG
    printf("Insptr: %p, second check: %p\n", ins, ins->instructions[0].value);
#endif


    for (i=0; i<ins->instruction_count; i++)
        if(ins->instructions[i].operation)
            printf("[op: %16s | type: %13s | value: %8s]\n", 
                OPERATION_STRING_TAB[ins->instructions[i].operation],
                TYPE_STRING_TAB[ins->instructions[i].type],
                (char *) 
                    (ins->instructions[i].value != NULL ? 
                        ins->instructions[i].value->value : 
                        "null"));
        else
            printf("[op: %16s | type: %13s | value: %8s]\n", 
                OPERATION_STRING_TAB[ins->instructions[i].operation], 
                "none", 
                "none");
}

enum dstru_types ffi_dstru_bridge(enum type stype){
#ifdef DEBUG
    printf("ffi_dstru_bridge()");
#endif
    switch(stype){
        case STYPE_CCHAR: return DYN_S_UINT8; break;
        case STYPE_CUCHAR: return DYN_S_UINT8; break;
        case STYPE_CSHORT: return DYN_S_UINT16; break;
        case STYPE_CUSHORT: return DYN_S_UINT16; break;
        case STYPE_CINT: return DYN_S_UINT32; break;
        case STYPE_CUINT: return DYN_S_UINT32; break;
        case STYPE_CLONG: return DYN_S_UINT64; break;
        case STYPE_CULONG: return DYN_S_UINT64; break;
        case STYPE_CLONGLONG: return DYN_S_UINT64; break;
        case STYPE_CULONGLONG: return DYN_S_UINT64; break;
        case STYPE_CFLOAT: return DYN_S_FLOAT; break;
        case STYPE_CDOUBLE: return DYN_S_DOUBLE; break;
    }
    return -1;
}

/* replaces each escaped character with the real character */
int unescape(char **dest_string, const char *src){
    //TODO

    return 0;
}
