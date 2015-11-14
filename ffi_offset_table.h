#ifndef OFFSET_TABLE
#define OFFSET_TABLE

#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"

struct offset_member {
    int offset;
    int size;
    enum type scalar_type;
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

#endif /* OFFSET_TABLE */
