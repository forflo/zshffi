#ifndef OFFSET_TABLE
#define OFFSET_TABLE

#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"

#define OFFSET_TABLE_FLAG(X) \
        X(NORMAL) \
        X(PTR_MEMBER) \
        X(SPTR_OFFSET_TBL)

#define GENERATE_ENUM(X) X, 
#define GENERATE_STRING(X) #X, 

enum offset_table_flag { OFFSET_TABLE_FLAG(GENERATE_ENUM) };

struct offset_member {
    int offset;
    int size;
    enum type scalar_type;
    enum offset_table_flag flags;
    struct offset_table *subtable;
};

struct offset_table {
    /* bytes all members would need if they'd lie in a struct */
    int structure_size; 
    struct offset_member *members;
    int member_count;
    int table_size;
};

int add_to_table_otable(struct offset_table *src, struct offset_table *dest);
int add_to_table(struct ffi_instruction *ins, struct offset_table *tbl);
int gentbl(struct ffi_instruction_obj *ops, struct offset_table **table);
int add_to_table_sptr(struct offset_table *dest, struct ffi_instruction *ins);

#endif /* OFFSET_TABLE */
