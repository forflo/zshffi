#ifndef OFFSET_TABLE
#define OFFSET_TABLE

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

#endif /* OFFSET_TABLE */
