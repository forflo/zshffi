#include "ffi_offset_table.h"
#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"
#include "ffi_util.h"
#include "ffi_dstru.h"

#include <stdlib.h>
#include <stdio.h>

#define DEBUG
#define INIT_SIZE 10
#define STACK_INITIAL 100

struct offset_stack {
    struct offset_table **stack;
    int stack_size;
    int stack_elem;
};

static int stack_init(struct offset_stack **stack){
    *stack = malloc(sizeof(struct offset_stack));
    if(stack == NULL)
        return 1;
    
    (*stack)->stack = malloc(sizeof(struct offset_table *) * STACK_INITIAL);
    (*stack)->stack_size = STACK_INITIAL;
    (*stack)->stack_elem = 0;

    if((*stack)->stack == NULL)
        return 1;

    return 0;
}

static int top(struct offset_table **res, struct offset_stack *stack){
#ifdef DEBUG
    printf("top(): %p\n", res);
#endif
    if (stack->stack_elem == 0)
        return 1;
    *res = stack->stack[stack->stack_elem - 1]; 
    return 0;
}

static int push(struct offset_table *e, struct offset_stack *stack){
#ifdef DEBUG
    printf("push(): %p\n", e);
#endif
    if (stack->stack_elem < stack->stack_size){
        stack->stack[stack->stack_elem] = e;
    } else {
        stack->stack_size *= 2;
        stack->stack = realloc(stack->stack, stack->stack_size); 
        if(stack->stack == NULL)
            return 1;
        stack->stack[stack->stack_elem] = e;
    }

    stack->stack_elem++;

    return 0;
}

/* The stack does not shrink! */
static int pop(struct offset_table **res, struct offset_stack *stack){
#ifdef DEBUG
    printf("pop(): %p\n", res);
#endif
    if (stack->stack_elem == 0)
        return 1;
    *res = stack->stack[--stack->stack_elem]; 
    return 0; 
}


static int table_init(struct offset_table **tbl){
    (*tbl) = malloc(sizeof(struct offset_table));
    if (*tbl == NULL)
        return 1;
    (*tbl)->member_count = 0;
    (*tbl)->table_size = INIT_SIZE;
    (*tbl)->members = malloc(sizeof(struct offset_member) * INIT_SIZE);
    if ((*tbl)->members == NULL)
        return 1;

    return 0;
}

static int table_add_entry(struct offset_table *tbl, struct offset_member mem){
    if(tbl->member_count == tbl->table_size){
        tbl->table_size *= 2;
        tbl->members = realloc(tbl->members, 
                sizeof(struct offset_member) * tbl->table_size);
        if (tbl->members == NULL)
            return 1;
    }

    tbl->members[tbl->member_count++] = mem;

    return 0;
}

int gentbl(struct ffi_instruction_obj *ops, struct offset_table **table){
    struct offset_table *result;
    struct offset_table *temp;
    struct ffi_instruction cur;
    int i;

    if (table_init(&result) != 0)
        return 1;

    struct offset_stack *ostack;
    if(stack_init(&ostack) != 0)
        return 1;
    
    for (i=0; i<ops->instruction_count; cur = ops->instructions[i++]){
        switch (cur.operation){
            /* Will be flattened */
            case START_STRUCT: break;
            case END_STRUCT: break;
            case START_STRUCT_PTR:
#ifdef DEBUG
                printf("gentab(): PTR\n");
#endif
                break;
            case END_STRUCT_PTR:
#ifdef DEBUG
                printf("gentab(): END-PTR\n");
#endif
                break;
            case MEMBER:
#ifdef DEBUG
                printf("gentab(): MMEMBER\n");
#endif
                break;
            case START_UNION:
                break;
            case END_UNION:
                break;
            default:
#ifdef DEBUG
                fprintf(stderr, "Currently not implemented!\n");
#endif
                break;
        }
    }

    
    *table = result;

    return 0;
}

int add_to_table(struct ffi_instruction *ins, struct offset_table *tbl){
    long type = ins->type;
    int pad_size, new_size;
    enum dstru_types t = ffi_dstru_bridge(type);
    struct dstru_struct dstru_temp = { NULL, 0, 0, 0, 0, NULL, 0, 0  };
    struct offset_member temp = { 0, 0, STYPE_NONE, NULL };

    new_size = tbl->structure_size +
        dstru_padding(t, &dstru_temp) +
        dstru_sizeof(t, NULL);

    pad_size = new_size - dstru_sizeof(t, NULL);

    temp.scalar_type = type;
    temp.size = dstru_sizeof(t, NULL);
    temp.subtable = NULL;
    temp.offset = pad_size;

    table_add_entry(tbl, temp);

    return 0;
}
