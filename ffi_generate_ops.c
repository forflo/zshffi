#include "ffi_nary_tree.h"
#include "ffi_util.h"
#include "ffi_node_defines.h"
#include "ffi_generate_ops.h"
#include <stdio.h>
#include <stdlib.h>

struct ffi_instruction **genops(struct nary_node *tval_list){
    struct ffi_instruction **result = malloc(sizeof(struct ffi_instruction *) * INS_MAX);
    struct ffi_instruction instructions[1000] = { 0 };
    int instruction_cnt = 0, i;

    if (result == NULL){
        return NULL;
    }

    for (i=0; i<INS_MAX; i++){
        result[i] = malloc(sizeof(struct ffi_instruction));
        if (result[i] == NULL){
            return NULL;
        }
    }

    genops_tvallist(instructions, &instruction_cnt, tval_list);
    //emit_human(instructions, instruction_cnt);

    for (i=0; i<instruction_cnt; i++){
        result[i]->operation = instructions[i].operation;
        result[i]->type = instructions[i].type;
        result[i]->value = instructions[i].value;
    }

    result[instruction_cnt] = NULL;

    return result;
}


int genops_scalar(struct ffi_instruction ins[], int *position, struct nary_node *scalar){
    int ty = scalar->node_type;
    struct ffi_instruction scalar_temp = { 
        member, (long) scalar->content, (char *) scalar->nodes[0]->content
    }; 
    struct ffi_instruction arr_temp = {arr_to, 0, NULL};
    struct ffi_instruction ptr_temp = {ptr_to, 0, NULL};
    struct ffi_instruction arr_end_temp = {arr_end, 0, NULL};

    switch (ty){
        case NT_SCALAR    : 
            ins[*position] = scalar_temp;
            (*position)++;
            break;
        case NT_SCALAR_PTR: 
            ins[*position] = ptr_temp;
            (*position)++;
            ins[*position] = scalar_temp;
            (*position)++;
            break;
        case NT_SCALAR_ARR: 
            ins[*position] = arr_temp;
            (*position)++;
            genops_tvallist(ins, position, scalar->nodes[0]);
            ins[*position] = arr_end_temp;
            (*position)++;
            break;
    }

    return 0;
}

int genops_compound(struct ffi_instruction ins[], int *position, struct nary_node *compound){
    int ty = compound->node_type;
    long c_type = (long) compound->content;
    struct ffi_instruction arr_temp = {arr_to, 0, NULL};
    struct ffi_instruction ptr_temp = {ptr_to, 0, NULL};
    struct ffi_instruction struct_temp_start = {start_struct, 0, NULL};
    struct ffi_instruction union_temp_start = {start_union, 0, NULL};
    struct ffi_instruction struct_temp_end = {end_struct, 0, NULL};
    struct ffi_instruction union_temp_end = {end_union, 0, NULL};

    switch (ty){
        case NT_COMPOUND: break;
        case NT_COMPOUND_ARR:
            ins[*position] = arr_temp;
            (*position)++;
            break;
        case NT_COMPOUND_PTR:
            ins[*position] = ptr_temp;
            (*position)++;
            break;
    }

    switch(c_type){
        case STRUCT: 
            ins[*position] = struct_temp_start;
            (*position)++; 
            genops_tvallist(ins, position, compound->nodes[0]);

            ins[*position] = struct_temp_end;
            (*position)++; 
            break;
        case UNION: 
            ins[*position] = union_temp_start; 
            (*position)++; 
            genops_tvallist(ins, position, compound->nodes[0]);

            ins[*position] = union_temp_end;
            (*position)++; 
            break;
    } 

    return 0;
}

int genops_tvallist(struct ffi_instruction ins[], int *position, struct nary_node *tvallist){
    if (tvallist->nnode == 2){
        genops_tvallist(ins, position, tvallist->nodes[0]);
        genops_tval(ins, position, tvallist->nodes[1]);
    } else {
        genops_tval(ins, position, tvallist->nodes[0]);
    }
}

int genops_tval(struct ffi_instruction ins[], int *position, struct nary_node *tval){
    int ty = tval->nodes[0]->node_type;

    if (ty == NT_SCALAR || ty == NT_SCALAR_PTR || ty == NT_SCALAR_ARR){
        genops_scalar(ins, position, tval->nodes[0]);
    } else if (ty == NT_COMPOUND || ty == NT_COMPOUND_PTR || ty == NT_COMPOUND_ARR) {
        genops_compound(ins, position, tval->nodes[0]);
    } else {
        /* should really not happen!!! */
        fprintf(stderr, "Panic! Node is neither a scalar nor a compound!\n");
    }

    return 0;
}
