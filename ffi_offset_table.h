#ifndef OFFSET_TABLE
#define OFFSET_TABLE

#include "ffi_generate_ops.h"

struct offset_member {
    int offset;
    int size;
    enum type scalar_type;
    struct offset_table *subtable;
};

struct offset_table {
    int structure_size; //used to calculate the new offsets see dystruct.c (126)
    struct offset_member *members;
    int member_count;
    int table_size;
};

int add_to_table_otable(struct offset_table *src, struct offset_table *dest);
int add_to_table(struct ffi_instruction *ins, struct offset_table *tbl);

#endif /* OFFSET_TABLE */
