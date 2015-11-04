#ifndef INTERPRETER
#define INTERPRETER

enum ffi_bytecode_op {
    start_struct,
    member,
    end_struct,
    pop
};

struct ffi_instruction {
    enum ffi_bytecode_op operation;
    long type;
    char *value; 
};

#endif /* INTERPRETER */
