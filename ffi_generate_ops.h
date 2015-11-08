#ifndef INTERPRETER
#define INTERPRETER

#include "ffi_nary_tree.h"
#define INS_MAX 1000

#define FFI_BYTECODE(F)     \
        F(ARR_TO)           \
        F(ARR_END)          \
        F(PTR_TO)           \
        F(START_STRUCT)     \
        F(START_UNION)      \
        F(MEMBER     )      \
        F(END_STRUCT )      \
        F(END_UNION  )      \
        F(POP         )     \

#define GENERATE_ENUM(X) X,
#define GENERATE_STRING(X) #X,

enum ffi_bytecode_op { FFI_BYTECODE(GENERATE_ENUM) };

struct ffi_instruction {
    enum ffi_bytecode_op operation;
    long type;
    char *value; 
};

int genops(struct ffi_instruction ***genops, struct nary_node *tval_list);

int genops_scalar(struct ffi_instruction ins[], 
    int *position, 
    struct nary_node *scalar);

int genops_tvallist(struct ffi_instruction ins[], 
    int *position, 
    struct nary_node *tvallist);

int genops_tval(struct ffi_instruction ins[], 
    int *position, 
    struct nary_node *tval);

int genops_compound(struct ffi_instruction ins[], 
    int *position, 
    struct nary_node *compound);


#endif /* INTERPRETER */
