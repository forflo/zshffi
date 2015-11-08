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
