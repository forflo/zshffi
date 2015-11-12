#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ffi_nary_tree.h"
#include "ffi_parser_util.h"
#include "ffi_node_defines.h"

static const char *NODETYPE_STRING_TAB[] = { LIST_NTYPE(GENERATE_STRING) };

struct nary_node *make_node(int cnt, int node_type, void *v, int childs, ...){
	va_list args;
	int i;
	va_start (args, childs);
    char buf[10];

	struct nary_node *result = new_node(v);
    result->node_type = node_type;
    result->name[0] = '\0';
    strcat(result->name, NODETYPE_STRING_TAB[node_type]);
    sprintf(buf, "%d", cnt);
    strcat(result->name, buf);

	for(i = 0; i < childs; i++)
		add_child(result, va_arg(args, struct nary_node *));

	va_end(args);
	return result;
}
