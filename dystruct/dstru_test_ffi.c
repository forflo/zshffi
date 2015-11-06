#include "dstru_test_types.h"
#include "dstru_types.h"
#include "dstru_defines.h"
#include "dstru_funcs.h"

#include <ffi.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>

struct dstru_struct *get_nested(){
	int cnt;
	struct dstru_struct *dest;
	struct nested_2 *result;

	/* Future member values */
	uint32_t i1 = 42, i2 = 100;
	uint16_t s1 = 3220, s2 = 123;	
	double d1 = 424242.121212, d2 = 4711.332273;
	uint8_t foo1[32];
	uint32_t foo2[4];

	for (cnt = 0; cnt < 32; cnt++)
		foo1[cnt] = cnt + '0';

	for (cnt = 0; cnt < 4; cnt++)
		foo2[cnt] = cnt + 42;	

	dstru_init(0, &dest);

	dstru_add_member(DYN_S_UINT32, &i1, dest);
	dstru_add_member(DYN_S_UINT32, &i2, dest);
	dstru_add_member(DYN_S_UINT16, &s1, dest);

	dstru_add_member(DYN_S_DOUBLE, &d1, dest);
	dstru_add_member(DYN_S_UINT16, &s2, dest);
	dstru_add_member(DYN_S_DOUBLE, &d2, dest);

	dstru_add_bytefield(32, (void *) foo1, dest);
	dstru_add_array(4, DYN_S_UINT32, (void *) foo2, dest);

	dstru_finalize(dest);

    return dest;
}

void test_func_ffi(struct nested_2 value){
    int i;
    printf("Sizeof nested_2: %i\n", sizeof(struct nested_2));
    printf("Member anon1.a %i\n", value.anon1.a);
    printf("Member anon1.b %i\n", value.anon1.b);
    printf("Member anon1.c %hi\n", value.anon1.c);
    printf("Member anon2.d1 %F\n", value.anon2.d1);
    printf("Member anon2.s %hu\n", value.anon2.s);
    printf("Member anon2.d2 %F\n", value.anon2.d2);
    for (i=0; i<32; i++){
        printf("Member anon3.byte[%i]: %hhu\n", i, value.anon3.string[i]);
    }
    for (i=0; i<4; i++){
        printf("Member anon3.arr[%i]: %i\n", i, value.anon3.arr[i]);
    }
}

void test_func_ffi_ptr(struct nested_2 *value){
    int i;
    printf("Sizeof nested_2: %i\n", sizeof(struct nested_2));
    printf("Member anon1.a %i\n", value->anon1.a);
    printf("Member anon1.b %i\n", value->anon1.b);
    printf("Member anon1.c %hi\n", value->anon1.c);
    printf("Member anon2.d1 %F\n", value->anon2.d1);
    printf("Member anon2.s %hu\n", value->anon2.s);
    printf("Member anon2.d2 %F\n", value->anon2.d2);
    for (i=0; i<32; i++){
        printf("Member anon3.byte[%i]: %hhu\n", i, value->anon3.string[i]);
    }
    for (i=0; i<4; i++){
        printf("Member anon3.arr[%i]: %i\n", i, value->anon3.arr[i]);
    }
}

int main(int argch, char **argv){
    struct dstru_struct *nested = get_nested();

    ffi_cif cif;
    ffi_type struct_type;

    /* We can omit the elements since we know
       which amount of bytes libffi should write
       to the called functions stack */
    struct_type.size = sizeof(struct nested_2);
    struct_type.elements = NULL;
    struct_type.alignment = 0;
    struct_type.type = FFI_TYPE_STRUCT;

    ffi_type *args[1];
    void *values[1];
    int rc;

    struct nested_2 foo = {
        123, 321,12, 0.00232F, 32, 0.1002F, 
        '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
         '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
         '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' , 
        43, 43, 43, 43
    };

    test_func_ffi(foo);
    test_func_ffi_ptr(&foo);

    test_func_ffi_ptr(nested->buffer);
   
    args[0] = &struct_type;
    values[0] = nested->buffer; 
    if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &struct_type, args) == FFI_OK){
        printf("Doing call!\n");
        ffi_call(&cif, (void *) test_func_ffi, &rc, values);
    } else {
        fprintf(stderr, "FFI-nok!\n");
    }

    return EXIT_SUCCESS;
}
