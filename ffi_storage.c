#include "ffi_storage.h"
#include "ffi_dstru.h"

#define STACK_INITIAL 100

static enum vm_flags {
    flg_ptr_to = 1
};

static int vm_flag
static struct dstru_struct *vm_stack[STACK_INITIAL];
static int stack_size = STACK_INITIAL;
static int stack_elem = 0;

static int reset(){
    stack_size = STACK_INITIAL;
    stack_elem = 0;
    vm_stack = realloc(vm_stack, stack_size);
    if(vm_stack == NULL)
       return 1;
    return 0; 
}

static int top(struct dstru_struct **res){
    vm_stack[stack_elem - 1]; 
    return 0;
}

static int push(struct dstru_struct *e){
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
    if (stack_elem == 0)
        return 1;
    *res = vm_stack[--stack_elem]; 
    return 0; 
}

int get_storage(void **res, struct ffi_instruction **s_ops){
    if (s_ops == NULL || res == NULL || *res == NULL)
        return 1;

    int position = 0;
    struct ffi_instruction cur = s_ops[position++];

    struct dstru_struct *s;
    dstru_init(0, &s);
    push(s);

    while (cur != NULL){
        switch (cur->operation){
            case arr_to: break;

            case arr_end: break; 
            case ptr_to:        
                vm_flags |= flg_ptr_to;
                break;
            case start_struct: 
                break;
            case end_struct:
                break;
            case member:
                if(vm_flag = flg_ptr_to){
                    struct dstru_struct *s;
                    dstru_init(0, &s);
                    push(s);                    

                    gs_add(                    
                } else {
                    dstru_add 
                }

                vm_flag = 0;

                break;
            case start_union:
            case end_union:
        }

    }

    return 0;
}

int add_to_dystru(struct ffi_instruction *s, struct dstru_struct *s){
    long type = s->type;

    switch(type){
        case STYPE_CCHAR:
            uint8_t res = 
            break;
        case STYPE_CINT; 
            uint32_t res = atoi((char *) s->value);
            printf("gs_add res: %i\n", res);
            dstru_add_uint32(res, s);
            break;
    }

    return 0;
}

int conv_to_cchar(int8_t *res, struct token_value *val);
int conv_to_cuchar(uint8_t *res, struct token_value *val);
int conv_to_cshort(int16_t *res, struct token_value *val);
int conv_to_cushort(uint16_t *res, struct token_value *val);
int conv_to_cint(int32_t *res, struct token_value *val);
int conv_to_cuint(uint32_t *res, struct token_value *val);

int conv_to_clong(*res, struct token_value *val);
int conv_to_culong(*res, struct token_value *val);
int conv_to_clonglong(*res, struct token_value *val);
int conv_to_culonglong(*res, struct token_value *val);
int conv_to_cfloat(*res, struct token_value *val);
int conv_to_cdouble(*res, struct token_value *val);
int *conv_to_voidp(*res, struct token_value *val);

int conv_to_cchar(int8_t *res, struct token_value *val){
    if (value->length > 0)
        *res = (int8_t) val->value[0];
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
    *res = atof(res);

    return 0;
}

int conv_to_cdouble(double *res, struct token_value *val){
    *res = atod(res);

    return 0;
}

int conv_to_voidp(void **res, struct token_value *val){
    *res = (void *) strtoul(val->value, NULL, 16); 

    return 0;
}
