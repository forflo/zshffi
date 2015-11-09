#ifndef STORAGE 
#define STORAGE

#include "ffi_generate_ops.h"
#include "ffi_dstru.h"
#include "ffi_parser_util.h"

int get_storage(void **res, struct ffi_instruction **s_ops);
int add_to_top(struct ffi_instruction *ins, struct dstru_struct *top);

int conv_to_cchar(int8_t *res, struct token_value *val);
int conv_to_cuchar(uint8_t *res, struct token_value *val);
int conv_to_cshort(int16_t *res, struct token_value *val);
int conv_to_cushort(uint16_t *res, struct token_value *val);
int conv_to_cint(int32_t *res, struct token_value *val);
int conv_to_cuint(uint32_t *res, struct token_value *val);

int conv_to_clong(int64_t *res, struct token_value *val);
int conv_to_culong(uint64_t *res, struct token_value *val);
int conv_to_clonglong(long long *res, struct token_value *val);
int conv_to_culonglong(unsigned long long *res, struct token_value *val);
int conv_to_cfloat(float *res, struct token_value *val);
int conv_to_cdouble(double *res, struct token_value *val);
int conv_to_voidp(void **res, struct token_value *val);


#endif /* STORAGE */
