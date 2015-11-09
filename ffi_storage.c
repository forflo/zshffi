#include "ffi_storage.h"
#include "ffi_dstru.h"
#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"
#include "ffi_parser_util.h"

#include <stdlib.h>
#include <stdio.h>

#define STACK_INITIAL 100
#define DEBUG

static struct dstru_struct **vm_stack;
static int stack_size = STACK_INITIAL;
static int stack_elem = 0;

static int reset(){
    int i;
    stack_size = STACK_INITIAL;
    stack_elem = 0;
    vm_stack = realloc(vm_stack, stack_size);
    if(vm_stack == NULL)
       return 1;
    return 0; 
}

static int top(struct dstru_struct **res){
#ifdef DEBUG
    printf("top(): %p\n", res);
#endif
    if (stack_elem == 0)
        return 1;
    *res = vm_stack[stack_elem - 1]; 
    return 0;
}

static int push(struct dstru_struct *e){
#ifdef DEBUG
    printf("push(): %p\n", e);
#endif
    if (stack_elem < stack_size){
        vm_stack[stack_elem] = e;
    } else {
        stack_size *= 2;
        vm_stack = realloc(vm_stack, stack_size); 
        if(vm_stack == NULL)
            return 1;
        vm_stack[stack_elem] = e;
    }

    stack_elem++;

    return 0;
}

/* The stack does not shrink! */
static int pop(struct dstru_struct **res){
#ifdef DEBUG
    printf("pop(): %p\n", res);
#endif
    if (stack_elem == 0)
        return 1;
    *res = vm_stack[--stack_elem]; 
    return 0; 
}

int get_storage(void **res, struct ffi_instruction **s_ops){
    if (s_ops == NULL || res == NULL || *res == NULL)
        return 1;

    int position = 0;
    struct ffi_instruction *cur = s_ops[position++];

    enum type arr_members;
    struct dstru_struct *first;
    struct dstru_struct *temp;
    dstru_init(0, &first);
    push(first);

    while (cur != NULL){
        switch (cur->operation){
            case START_STRUCT: 
                break;
            case END_STRUCT:
                break;
            case MEMBER:
                top(&temp);
                add_to_top(cur, temp);
                break;
            case START_UNION:
                break;
            case END_UNION:
                break;
        }
    }

    return 0;
}

int add_to_top(struct ffi_instruction *ins, struct dstru_struct *top){
    long type = ins->type;

    switch(type){
        case STYPE_CCHAR:
        case STYPE_CINT:
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
