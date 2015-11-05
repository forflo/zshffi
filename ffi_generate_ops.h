#ifndef INTERPRETER
#define INTERPRETER

#include "ffi_nary_tree.h"
#define INS_MAX 1000

enum ffi_bytecode_op {
    arr_to,
    arr_end,
    ptr_to,
    start_struct,
    start_union,
    member,
    end_struct,
    end_union,
    pop
};

struct ffi_instruction {
    enum ffi_bytecode_op operation;
    long type;
    char *value; 
};

static void emit_human(struct ffi_instruction ins[], int cnt);

struct ffi_instruction **genops(struct nary_node *tval_list);
int genops_scalar(struct ffi_instruction ins[], int *position, struct nary_node *scalar);
int genops_tvallist(struct ffi_instruction ins[], int *position, struct nary_node *tvallist);
int genops_tval(struct ffi_instruction ins[], int *position, struct nary_node *tval);
int genops_compound(struct ffi_instruction ins[], int *position, struct nary_node *compound);


#endif /* INTERPRETER */
