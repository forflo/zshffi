#include "ffi_storage.h"
#include "ffi_dstru_types.h"
#include "ffi_dstru_funcs.h"
#include "ffi_dstru_defines.h"

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

char conv_to_cchar();
uint8_t conv_to_cuchar();
short conv_to_cshort();
uint16_t conv_to_cushort();
int conv_to_cint();
uint32_t conv_to_cuint();
clong conv_to_clong();
int conv_to_culong();
long long conv_to_clonglong();
int conv_to_culonglong();
intmax_t conv_to_cintmax_t();
uintmax_t conv_to_cuintmax_t();
size_t conv_to_csize_t();
int conv_to_cssize_t();
int conv_to_cptrdiff_t();
int conv_to_coff_t();
wchar_t conv_to_cwchar_t();
float conv_to_cfloat();
double conv_to_cdouble();
void *conv_to_voidp();

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
