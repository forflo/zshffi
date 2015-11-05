#include "ffi_generate_dot.h"
#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_util.h"
#include <stdio.h>
#include <string.h>

static int gen_sig_cnt = 0;
static int gen_tval_cnt = 0;
static int gen_tval_lst_cnt = 0;
static int gen_s_cnt = 0;
static int gen_sarr_cnt = 0;
static int gen_sptr_cnt = 0;
static int gen_c_cnt = 0;
static int gen_cptr_cnt = 0;
static int gen_carr_cnt = 0;
static int gen_val_cnt = 0;

static int emit_childs(struct nary_node *node);
static int output(struct nary_node *node);
static void gen_count(int val);
static char *gen_get_typestring(int val);
static int gen_get(int val);

static void gen_count(int val){
    switch(val){
        case NT_SIGNATURE   : gen_sig_cnt++     ; break;
        case NT_TYPEANDVAL  : gen_tval_cnt++    ; break;
        case NT_SCALAR_ARR  : gen_sarr_cnt++    ; break;
        case NT_SCALAR      : gen_s_cnt++       ; break;
        case NT_SCALAR_PTR  : gen_sptr_cnt++    ; break; 
        case NT_COMPOUND    : gen_c_cnt++       ; break;   
        case NT_COMPOUND_ARR: gen_carr_cnt++    ; break;
        case NT_COMPOUND_PTR: gen_cptr_cnt++    ; break;
        case NT_TVAL_LIST   : gen_tval_lst_cnt++; break;   
        case NT_VALUE       : gen_val_cnt++     ; break;   
    } 
}

static int gen_get(int val){
    switch(val){
        case NT_SIGNATURE   : return gen_sig_cnt     ; break;
        case NT_TYPEANDVAL  : return gen_tval_cnt    ; break;
        case NT_SCALAR_ARR  : return gen_sarr_cnt    ; break;
        case NT_SCALAR      : return gen_s_cnt       ; break;
        case NT_SCALAR_PTR  : return gen_sptr_cnt    ; break; 
        case NT_COMPOUND    : return gen_c_cnt       ; break;   
        case NT_COMPOUND_ARR: return gen_carr_cnt    ; break;
        case NT_COMPOUND_PTR: return gen_cptr_cnt    ; break;
        case NT_TVAL_LIST   : return gen_tval_lst_cnt; break;   
        case NT_VALUE       : return gen_val_cnt     ; break;   
    } 
}

static int emit_childs(struct nary_node *node){
    int i;
    char parent_name[20] = "";
    char child_name[20] = "";

    char *ts_parent = gen_get_typestring(node->node_type);
    char *ts_child = NULL;

    sprintf(parent_name, "%s_%d", ts_parent, gen_get(node->node_type));

    for (i=0; i<node->nnode; i++){
        ts_child = gen_get_typestring(node->nodes[i]->node_type);
        sprintf(child_name, "%s_%d", ts_child, gen_get(node->nodes[i]->node_type) - 1 );
        printf("%s -> %s\n", parent_name, child_name);
        child_name[0] = '\0';
    }

    return 0;
}

static int output(struct nary_node *node){
    int i;

    emit_childs(node); 
    for (i=0; i<node->nnode; i++){
        output(node->nodes[i]);
    gen_count(node->node_type);
    }

    return 0;
}

int generate_dot(struct nary_node *node){
    output(node);
    return 0;
}
