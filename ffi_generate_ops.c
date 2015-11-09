#include "ffi_nary_tree.h"
#include "ffi_util.h"
#include "ffi_node_defines.h"
#include "ffi_generate_ops.h"
#include "ffi_parser_util.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG
static int genops_add(struct ffi_instruction ins[], int *position, struct ffi_instruction i);

static const struct ffi_instruction start_struct = {START_STRUCT, STYPE_NONE, NULL};
static const struct ffi_instruction start_struct_ptr = {START_STRUCT_PTR, STYPE_NONE, NULL};
static const struct ffi_instruction start_struct_arr = {START_STRUCT_ARR, STYPE_NONE, NULL};
static const struct ffi_instruction start_union = {START_UNION, STYPE_NONE, NULL};
static const struct ffi_instruction start_union_ptr = {START_UNION_PTR, STYPE_NONE, NULL};
static const struct ffi_instruction start_union_arr = {START_UNION_ARR, STYPE_NONE, NULL};
static const struct ffi_instruction end_struct = {END_STRUCT, STYPE_NONE, NULL};
static const struct ffi_instruction end_struct_arr = {END_STRUCT_ARR, STYPE_NONE, NULL};
static const struct ffi_instruction end_struct_ptr = {END_STRUCT_PTR, STYPE_NONE, NULL};
static const struct ffi_instruction end_union = {END_UNION, STYPE_NONE, NULL};
static const struct ffi_instruction end_union_arr= {END_UNION_ARR, STYPE_NONE, NULL};
static const struct ffi_instruction end_union_ptr = {END_UNION_PTR, STYPE_NONE, NULL};
static const struct ffi_instruction arr_end_temp = {ARR_END, STYPE_NONE, NULL};

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
                scalar->nodes[0]->content;
            break;
        case NT_SCALAR_ARR:
            scalar_temp.type = (long) scalar->node_type;
            scalar_temp.value = NULL;
            break;
    }

    switch (ty){
        case NT_SCALAR: 
#ifdef DEBUG
            printf("genops_scalar().plain\n");
#endif
            scalar_temp.operation = MEMBER;
            genops_add(ins, position, scalar_temp);
            break;
        case NT_SCALAR_PTR: 
#ifdef DEBUG
            printf("genops_scalar().ptr\n");
#endif

            scalar_temp.operation = MEMBER_PTR;
            genops_add(ins, position, scalar_temp);
            break;
        case NT_SCALAR_ARR: 
#ifdef DEBUG
            printf("genops_scalar().arr\n");
#endif

            scalar_temp.operation = MEMBER_ARR;
            scalar_temp.type = (long) scalar->content;
            genops_add(ins, position, scalar_temp);
            genops_tvallist(ins, position, scalar->nodes[0]);
            genops_add(ins, position, arr_end_temp);
            break;
    }

    return 0;
}

/* TODO: Find better solution for this */
int genops_compound(struct ffi_instruction ins[], 
        int *position, 
        struct nary_node *compound){
    int ty = compound->node_type;
#ifdef DEBUG
    printf("genops_compound() Node_type: [%s]\n", NONTERMINAL_STRING_TAB[ty]);
#endif
    long c_type = (long) compound->content;

    switch(c_type){
        case CTYPE_STRUCT: 
            switch (ty){
                case NT_COMPOUND: 
                    genops_add(ins, position, start_struct);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_struct);
                    break;
                case NT_COMPOUND_ARR: 
                    genops_add(ins, position, start_struct_arr);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_struct_arr);
                    break;
                case NT_COMPOUND_PTR: 
                    genops_add(ins, position, start_struct_ptr);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_struct_ptr);
                    break;
            }
            break;
        case CTYPE_UNION: 
            switch (ty){
                case NT_COMPOUND: 
                    genops_add(ins, position, start_union);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_union);
                    break;
                case NT_COMPOUND_ARR: 
                    genops_add(ins, position, start_union_arr);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_union_arr);
                    break;
                case NT_COMPOUND_PTR: 
                    genops_add(ins, position, start_union_ptr);
                    genops_tvallist(ins, position, compound->nodes[0]);
                    genops_add(ins, position, end_union_ptr);
                    break;
            }
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

static int genops_add(struct ffi_instruction ins[], 
        int *position, 
        struct ffi_instruction i){
    ins[*position] = i;
    (*position)++;
    return 0;
}
