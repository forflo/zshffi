#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_interpreter.h"
#include <stdio.h>

int interpret_scalar(struct ffi_instruction ins[], int *position, struct nary_node *scalar);
int interpret_tvallist(struct ffi_instruction ins[], int *position, struct nary_node *tvallist);
int interpret_tval(struct ffi_instruction ins[], int *position, struct nary_node *tval);
int interpret_compound(struct ffi_instruction ins[], int *position, struct nary_node *compound);

int interpret(void){
    struct ffi_instruction instructions[1000] = { 0 };
    int instruction_cnt = 0;

    struct ffi_instruction foo = {start_struct, 1, "foobar"};

    instructions[0] = foo;
}

int interpret_scalar(struct ffi_instruction ins[], int *position, struct nary_node *scalar){
    struct ffi_instruction tmp = 
        { member, (long) scalar->content, (char *) scalar->nodes[0]->content}; 

    ins[*position] = tmp;
    (*position)++;
    return 0;
}

int interpret_compound(struct ffi_instruction ins[], int *position, struct nary_node *compound){
    int ty = compound->node_type;

    switch (ty){
        case NT_COMPOUND:
            break;
        case NT_COMPOUND_ARR:
            break;
        case NT_COMPOUND_PTR:
            break;
    }
    
    return 0;
}

int interpret_tvallist(struct ffi_instruction ins[], int *position, struct nary_node *tvallist){
    if (tvallist->nnode == 2){
        interpret_tval(ins, position, tvallist->nodes[1]);
        interpret_tvallist(ins, position, tvallist->nodes[0]);
    } else {
        interpret_tval(ins, position, tvallist->nodes[0]);
    }
}

int interpret_tval(struct ffi_instruction ins[], int *position, struct nary_node *tval){
    int ty = tval->nodes[0]->node_type;

    if (ty == NT_SCALAR || ty == NT_SCALAR_PTR || ty == NT_SCALAR_ARR){
        interpret_scalar(ins, position, tval->nodes[0]);
    } else if (ty == NT_COMPOUND || ty == NT_COMPOUND_PTR || ty == NT_COMPOUND_ARR) {
        interpret_compound(ins, position, tval->nodes[0]);
    } else {
        /* should really nod happen!!! */
        fprintf(stderr, "Panic! Node is neither a scalar nor a compound!\n");
    }

    return 0;
}
