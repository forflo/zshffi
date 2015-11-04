#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ffi_nary_tree.h"
#include "ffi_parser_util.h"
#include "ffi_node_defines.h"

char *get_typestring(int val){
    static char *strings[] = {
       "SIG", "TVAL", "SARR", "SPTR", "S",
       "CARR", "CPTR", "C", "VAL", "TVAL_LST"
    };

    switch(val){
        case NT_SIGNATURE   : return strings[0]; break;
        case NT_TYPEANDVAL  : return strings[1]; break;
        case NT_SCALAR_ARR  : return strings[2]; break;
        case NT_SCALAR      : return strings[4]; break;
        case NT_SCALAR_PTR  : return strings[3]; break; 
        case NT_COMPOUND    : return strings[7]; break;   
        case NT_COMPOUND_ARR: return strings[5]; break;
        case NT_COMPOUND_PTR: return strings[6]; break;
        case NT_TVAL_LIST   : return strings[9]; break;   
        case NT_VALUE       : return strings[8]; break;   
    } 
    
    return "(NIL)";
}

struct nary_node *make_node(int cnt, int node_type, void *v, int childs, ...){
	va_list args;
	int i;
	va_start (args, childs);
    char buf[10];

	struct nary_node *result = new_node(v);
    result->node_type = node_type;
    result->name[0] = '\0';
    strcat(result->name, get_typestring(node_type));
    sprintf(buf, "%d", cnt);
    strcat(result->name, buf);

	for(i = 0; i < childs; i++){
		add_child(result, va_arg(args, struct nary_node *));
    }

	va_end(args);
	return result;
}
