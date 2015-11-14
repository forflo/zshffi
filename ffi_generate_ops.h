#ifndef INTERPRETER
#define INTERPRETER

#include "ffi_nary_tree.h"
#define INS_MAX 1000

#define FFI_BYTECODE(F)         \
        F(START_STRUCT_PTR)     \
        F(START_STRUCT)         \
        F(MEMBER_PTR)           \
        F(MEMBER)               \
        F(END_STRUCT_PTR)       \
        F(END_STRUCT)           \
        F(END_UNION_PTR)        \
        F(END_UNION)            \

#define GENERATE_ENUM(X) X,
#define GENERATE_STRING(X) #X,

enum ffi_bytecode_op { FFI_BYTECODE(GENERATE_ENUM) };

struct ffi_instruction {
    enum ffi_bytecode_op operation;
    long type;
    struct token_value *value; 
};

struct ffi_instruction_obj {
    struct ffi_instruction *instructions;
    int instruction_count;
    int size;
};

static int genops_insert(struct ffi_instruction_obj *ins, struct ffi_instruction stru);

int genops(struct ffi_instruction_obj **genops, struct nary_node *tval_list);

int genops_scalar(struct ffi_instruction_obj *ins, 
    struct nary_node *scalar);

int genops_tvallist(struct ffi_instruction_obj *ins, 
    struct nary_node *tvallist);

int genops_tval(struct ffi_instruction_obj *ins, 
    struct nary_node *tval);

int genops_compound(struct ffi_instruction_obj *ins, 
    struct nary_node *compound);


#endif /* INTERPRETER */
