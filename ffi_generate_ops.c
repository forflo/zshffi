#include "ffi_nary_tree.h"
#include "ffi_util.h"
#include "ffi_node_defines.h"
#include "ffi_generate_ops.h"
#include "ffi_parser_util.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG

static const struct ffi_instruction struct_temp_start = {START_STRUCT, STYPE_NONE, NULL};
static const struct ffi_instruction union_temp_start = {START_UNION, STYPE_NONE, NULL};
static const struct ffi_instruction struct_temp_end = {END_STRUCT, STYPE_NONE, NULL};
static const struct ffi_instruction union_temp_end = {END_UNION, STYPE_NONE, NULL};
static const struct ffi_instruction arr_end_temp = {ARR_END, STYPE_NONE, NULL};
static const struct ffi_instruction arr_temp = {ARR_TO, STYPE_NONE, NULL};
static const struct ffi_instruction ptr_temp = {PTR_TO, STYPE_NONE, NULL};

int genops(struct ffi_instruction ***genops, struct nary_node *tval){
    printf("genops(): genops = %p, tval_list = %p\n", genops, tval);
    struct ffi_instruction instructions[1000] = { 0 };
    struct ffi_instruction **result = 
        malloc(sizeof(struct ffi_instruction *) * INS_MAX);
    int instruction_cnt = 0, i;

    if (result == NULL)
        return NULL;

    for (i=0; i<INS_MAX; i++){
        result[i] = malloc(sizeof(struct ffi_instruction));
        if (result[i] == NULL)
            return NULL;
    }

    genops_tval(instructions, &instruction_cnt, tval);

    for (i=0; i<instruction_cnt; i++){
        result[i]->operation = instructions[i].operation;
        result[i]->type = instructions[i].type;
        result[i]->value = instructions[i].value;
    }

    result[instruction_cnt] = NULL;
    *genops = result;
#ifdef DEBUG
    printf("Fertig. Cnt: %d\n", instruction_cnt);
#endif

    return 0;
}

int genops_scalar(struct ffi_instruction ins[], 
        int *position, 
        struct nary_node *scalar){

    int ty = scalar->node_type;
#ifdef DEBUG
    printf("genops_scalar() Node_type: [%s]\n", NONTERMINAL_STRING_TAB[ty]);
#endif

    struct ffi_instruction scalar_temp = { 
        MEMBER, 
        (long) scalar->content, 
        NULL
    }; 

    switch (ty){
        case NT_SCALAR: case NT_SCALAR_PTR:
            scalar_temp.value = 
                (char *) ((struct token_value *) 
                scalar->nodes[0]->content)->value;
            break;
        case NT_SCALAR_ARR:
            scalar_temp.value = NULL;
            break;
    }

    switch (ty){
        case NT_SCALAR    : 
#ifdef DEBUG
            printf("genops_scalar().plain\n");
#endif
            ins[*position] = scalar_temp;
            (*position)++;
            break;
        case NT_SCALAR_PTR: 
#ifdef DEBUG
            printf("genops_scalar().ptr\n");
#endif

            ins[*position] = ptr_temp;
            (*position)++;
            ins[*position] = scalar_temp;
            (*position)++;
            break;
        case NT_SCALAR_ARR: 
#ifdef DEBUG
            printf("genops_scalar().arr\n");
#endif

            ins[*position] = arr_temp;
            (*position)++;
            genops_tvallist(ins, position, scalar->nodes[0]);
            ins[*position] = arr_end_temp;
            (*position)++;
            break;
    }

    return 0;
}

int genops_compound(struct ffi_instruction ins[], 
        int *position, 
        struct nary_node *compound){
    int ty = compound->node_type;
#ifdef DEBUG
    printf("genops_compound() Node_type: [%s]\n", NONTERMINAL_STRING_TAB[ty]);
#endif
    long c_type = (long) compound->content;

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
        case CTYPE_STRUCT: 
            ins[*position] = struct_temp_start;
            (*position)++; 
            genops_tvallist(ins, position, compound->nodes[0]);

            ins[*position] = struct_temp_end;
            (*position)++; 
            break;
        case CTYPE_UNION: 
            ins[*position] = union_temp_start; 
            (*position)++; 
            genops_tvallist(ins, position, compound->nodes[0]);

            ins[*position] = union_temp_end;
            (*position)++; 
            break;
    } 

    return 0;
}

int genops_tvallist(struct ffi_instruction ins[], 
        int *position, 
        struct nary_node *tvallist){
#ifdef DEBUG
    printf("genops_tvallist() Node_type: [%s]\n", NONTERMINAL_STRING_TAB[tvallist->node_type]);
#endif
    if (tvallist->nnode == 2){
        genops_tvallist(ins, position, tvallist->nodes[0]);
        genops_tval(ins, position, tvallist->nodes[1]);
    } else {
        genops_tval(ins, position, tvallist->nodes[0]);
    }
}

int genops_tval(struct ffi_instruction ins[], 
        int *position, 
        struct nary_node *tval){
#ifdef DEBUG
    printf("genops_tval() Node_type: [%s]\n", NONTERMINAL_STRING_TAB[tval->nodes[0]->node_type]);
#endif
    int ty = tval->nodes[0]->node_type;


    if (ty == NT_SCALAR || ty == NT_SCALAR_PTR || ty == NT_SCALAR_ARR){
        genops_scalar(ins, position, tval->nodes[0]);
    } else if (ty == NT_COMPOUND || ty == NT_COMPOUND_PTR || ty == NT_COMPOUND_ARR) {
        genops_compound(ins, position, tval->nodes[0]);
    } else {
        /* should really not happen!!! */
        fprintf(stderr, "Panic! Node [%s] is neither" 
                        " a scalar nor a compound!\n", NONTERMINAL_STRING_TAB[ty]);
    }

    return 0;
}
