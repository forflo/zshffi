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

static int stack_init(){
    vm_stack = malloc(sizeof(struct dstru_struct *) * STACK_INITIAL);
    if(vm_stack == NULL)
        return 1;
    return 0;
}

int get_storage(void **res, struct ffi_instruction_obj *s_ops){
#ifdef DEBUG
    printf("get_storage(): %p\n", s_ops);
#endif
    if (s_ops == NULL || res == NULL || *res == NULL)
        return 1;

    struct ffi_instruction cur;
    struct dstru_struct *first;
    struct dstru_struct *temp;
    struct dstru_struct *temp2;
    int position = 0;
    int first_flag = 0;
    int i;

    if(stack_init() != 0)
        return 1;

    dstru_init(0, &first);
    if (first == NULL)
        return 1;
    push(first);

    for (i=0; i<s_ops->instruction_count; cur = s_ops->instructions[i++]){
        switch (cur.operation){
            /* Will be flattened */
            case START_STRUCT: break;
            case END_STRUCT: break;
            case START_STRUCT_PTR:
#ifdef DEBUG
                printf("get_storage(): PTR\n");
#endif
                if (first_flag){
                    first_flag = 0;
                    continue;
                }

                dstru_init(0, &temp);
                push(temp);
                printf("New top: %p\n", temp);
                
                break;
            case END_STRUCT_PTR:
#ifdef DEBUG
                printf("get_storage(): END-PTR\n");
#endif
                if (stack_elem != 2){
                    pop(&temp);
                    dstru_finalize(temp);
                    top(&temp2);
                    printf("???%p %p\n", temp, temp2);
                    dstru_add_voidp(temp->buffer, temp2);
                }
                break;
            case MEMBER:
#ifdef DEBUG
                printf("get_storage(): MMEMBER\n");
#endif
                top(&temp);
                add_to_top(&cur, temp);
                break;
            case START_UNION:
                break;
            case END_UNION:
                break;
        }
    }

    top(&temp2);
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
