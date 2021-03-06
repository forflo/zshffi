#include "ffi_storage.h"
#include "ffi_dstru.h"
#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"
#include "ffi_parser_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define STACK_INITIAL 100
#define DEBUG

struct ffi_stack {
    struct dstru_struct **dstru_stack;
    int stack_size;
    int stack_elem;
};

static int stack_init(struct ffi_stack **stack){
    *stack = malloc(sizeof(struct ffi_stack));
    if(stack == NULL)
        return 1;
    
    (*stack)->dstru_stack = malloc(sizeof(struct dstru_struct *) * STACK_INITIAL);
    (*stack)->stack_size = STACK_INITIAL;
    (*stack)->stack_elem = 0;

    if((*stack)->dstru_stack == NULL)
        return 1;

    return 0;
}

static int top(struct dstru_struct **res, struct ffi_stack *stack){
#ifdef DEBUG
    printf("top(): %p\n", res);
#endif
    if (stack->stack_elem == 0)
        return 1;
    *res = stack->dstru_stack[stack->stack_elem - 1]; 
    return 0;
}

static int push(struct dstru_struct *e, struct ffi_stack *stack){
#ifdef DEBUG
    printf("push(): %p\n", e);
#endif
    if (stack->stack_elem < stack->stack_size){
        stack->dstru_stack[stack->stack_elem] = e;
    } else {
        stack->stack_size *= 2;
        stack->dstru_stack = realloc(stack->dstru_stack, stack->stack_size); 
        if(stack->dstru_stack == NULL)
            return 1;
        stack->dstru_stack[stack->stack_elem] = e;
    }

    stack->stack_elem++;

    return 0;
}

/* The stack does not shrink! */
static int pop(struct dstru_struct **res, struct ffi_stack *stack){
#ifdef DEBUG
    printf("pop(): %p\n", res);
#endif
    if (stack->stack_elem == 0)
        return 1;
    *res = stack->dstru_stack[--stack->stack_elem]; 
    return 0; 
}

int get_storage(void **res, struct ffi_instruction_obj *s_ops){
#ifdef DEBUG
    printf("get_storage(): %p\n", s_ops);
#endif
    if (s_ops == NULL || res == NULL)
        return 1;

    struct ffi_instruction cur;
    struct dstru_struct *temp1;
    struct dstru_struct *temp2;
    struct ffi_stack *stack;
    bool is_first = true;
    int i;


    if(stack_init(&stack) != 0)
        return 1;

    for (i=0; i<s_ops->instruction_count; cur = s_ops->instructions[i++]){
        switch (cur.operation){
            case START_STRUCT_PTR:
#ifdef DEBUG
                printf("get_storage(): PTR\n");
#endif
                dstru_init(0, &temp1);
                push(temp1, stack);
                
                break;
            case END_STRUCT_PTR:
#ifdef DEBUG
                printf("get_storage(): END-PTR\n");
#endif
                if (stack->stack_elem > 1){
                    pop(&temp1, stack);
                    dstru_finalize(temp1);
                    top(&temp2, stack);
#ifdef DEBUG
                    printf("temp1: %p temp2: %p\n", temp1, temp2);
                    printf("temp1->buffer: %p\n", temp1->buffer);
#endif
                    dstru_add_voidp(temp1->buffer, temp2);
                    free(temp1); /* Only free dstru struct not the buffer! */
                }
                break;
            case MEMBER:
#ifdef DEBUG
                printf("get_storage(): MMEMBER\n");
#endif
                top(&temp1, stack);
                add_to_top(&cur, temp1);
                break;
            case MEMBER_PTR:
#ifdef DEBUG
                printf("get_storage(): MEMBER-PTR\n");
#endif
                dstru_init(0, &temp1);
                add_to_top(&cur, temp1);
                dstru_finalize(temp1);

                top(&temp2, stack);
                dstru_add_voidp(temp1->buffer, temp2);
                free(temp1);
                break;
        }
    }

    top(&temp2, stack);
    dstru_finalize(temp2);
    *res = temp2->buffer;

    return 0;
}

int add_to_top(struct ffi_instruction *ins, struct dstru_struct *top){
    long type = ins->type;

    switch(type){
        case STYPE_CCHAR:
            {
            int8_t tmp;
            conv_to_cchar(&tmp, ins->value);
#ifdef DEBUG
            printf("add_to_top() cchar: %hhc\n", tmp);
#endif
            dstru_add_uint8(tmp, top);
            printf("AFTER!\n");
            }
            break;
        case STYPE_CUCHAR:
            {
            uint8_t tmp;
            conv_to_cuchar(&tmp, ins->value);
#ifdef DEBUG
            printf("add_to_top() cuchar: %hhu\n", tmp);
#endif
            dstru_add_uint8(tmp, top);
            }
            break;
        case STYPE_CSHORT:
            {
            int16_t tmp;
            conv_to_cshort(&tmp, ins->value);
#ifdef DEBUG
            printf("add_to_top() cshort: %hi\n", tmp);
#endif
            dstru_add_uint16(tmp, top);
            }
            break;
        case STYPE_CUSHORT:
            {
            uint16_t tmp;
            conv_to_cushort(&tmp, ins->value);
#ifdef DEBUG
            printf("add_to_top() cushort: %hu\n", tmp);
#endif
            dstru_add_uint16(tmp, top);
            }
            break;
        case STYPE_CINT:
            {
            int32_t tmp;
            conv_to_cint(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() cint: %d\n", tmp);
#endif
            dstru_add_uint32(tmp, top);
            }
            break;
        case STYPE_CUINT:
            {
            uint32_t tmp;
            conv_to_cuint(&tmp, ins->value);
#ifdef DEBUG
            printf("add_to_top() cuint: %u\n", tmp);
#endif
            dstru_add_uint32(tmp, top);
            }
            break;
        case STYPE_CLONG:
            {
            int64_t tmp;
            conv_to_clong(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %li\n", tmp);
#endif
            dstru_add_uint64(tmp, top);
            }
            break;
        case STYPE_CULONG:
            {
            uint64_t tmp;
            conv_to_culong(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %lu\n", tmp);
#endif
            dstru_add_uint64(tmp, top);
            }
            break;
        case STYPE_CLONGLONG:
            {
            long long tmp;
            conv_to_clonglong(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %llu\n", tmp);
#endif
            dstru_add_uint64(tmp, top);
            }
            break;
        case STYPE_CULONGLONG:
            {
            unsigned long long tmp;
            conv_to_culonglong(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %llu\n", tmp);
#endif
            dstru_add_uint64(tmp, top);
            }
            break;
        case STYPE_CFLOAT:
            {
            float tmp;
            conv_to_cfloat(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %f\n", tmp);
#endif
            dstru_add_float(tmp, top);
            }
            break;
        case STYPE_CDOUBLE:
            {
            double tmp;
            conv_to_cdouble(&tmp, ins->value); 
#ifdef DEBUG
            printf("add_to_top() %lf\n", tmp);
#endif
            dstru_add_double(tmp, top);
            }
            break;
    }

    return 0;
}


int conv_to_cchar(int8_t *res, struct token_value *val){
    if (val->length > 0)
        *res = (int8_t) ((char *) val->value)[0];
    else 
        return 1;

    return 0;
}

int conv_to_cuchar(uint8_t *res, struct token_value *val){
    *res = (uint8_t) strtol(val->value, NULL, 10);

    return 0;
}

int conv_to_cshort(int16_t *res, struct token_value *val){
    *res = (int16_t) atoi(val->value);

    return 0;
}

int conv_to_cushort(uint16_t *res, struct token_value *val){
    *res = (uint16_t) strtol(val->value, NULL, 10);

    return 0;
}

int conv_to_cint(int32_t *res, struct token_value *val){
    *res = atoi(val->value);

    return 0;
}

int conv_to_cuint(uint32_t *res, struct token_value *val){
    *res = (uint32_t) strtol(val->value, NULL, 10);

    return 0;
}

int conv_to_clong(int64_t *res, struct token_value *val){
    *res = strtol(val->value, NULL, 10);

    return 0;
}

int conv_to_culong(uint64_t*res, struct token_value *val){
    *res = strtoul(val->value, NULL, 10);

    return 0;
}

int conv_to_clonglong(long long *res, struct token_value *val){
    *res = strtoll(val->value, NULL, 10);

    return 0;
}

int conv_to_culonglong(unsigned long long *res, struct token_value *val){
    *res = strtoull(val->value, NULL, 10);

    return 0;
}

int conv_to_cfloat(float *res, struct token_value *val){
    *res = atof(val->value);

    return 0;
}

int conv_to_cdouble(double *res, struct token_value *val){
    *res = strtod(val->value, NULL);

    return 0;
}

int conv_to_voidp(void **res, struct token_value *val){
    *res = (void *) strtoul(val->value, NULL, 16); 

    return 0;
}
