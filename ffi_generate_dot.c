#include "ffi_generate_dot.h"
#include "ffi_nary_tree.h"
#include <stdio.h>

int emit_childs(struct nary_node *node){
    int i;
    for (i=0; i<node->nnode; i++){
        printf("%s -> %s\n", node->name, node->nodes[i]->name);
    }

    return 0;
}

int output(struct nary_node *node){
    int i;

    emit_childs(node); 
    for (i=0; i<node->nnode; i++){
        output(node->nodes[i]);
    }

    return 0;
}

