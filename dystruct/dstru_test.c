/*
Copyright (c) 2014 Florian Mayer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

----------------------------------------------------------------------------
This license slightly differs from the original MIT license.

* Sublicensing is not allowed.
* Alteration of the packages license is not allowed.
*/

#include <stdlib.h>
#include <stdint.h>

#include "config.h"


#ifdef CUNIT_K1
#	include <CUnit/CUnit.h>
#endif

#ifdef CUNIT_K2
#	include <CUnit/Cunit.h>
#endif

#ifdef CUNIT_BASIC
#	include <CUnit/Basic.h>
#endif

#include "dstru_test_types.h"
#include "dstru_test_funcs.h"

#include "dstru_funcs.h"
#include "dstru_defines.h"
#include "dstru_types.h"


/* unused but required by CUnit */
int init_test_dyn_str(void){ return 0; }
int clean_test_dyn_str(void){ return 0; }

/* creation and destruction test */
void test_init(void){
	struct dstru_struct *ds;
	CU_ASSERT(dstru_init(0, &ds) == 0);
	CU_ASSERT(ds != NULL);
	CU_ASSERT(ds->buffer == NULL && ds->align == 0);
	CU_ASSERT(ds->size == 0 && ds->elem_num == 0);

	dstru_free(ds);
}

/* Test the special case of adding a void pointer
	to a dynamic strucutre */
void test_voidp(void){
	struct dstru_struct *ds;
	struct flat_4 *v;
	int *i = (int *) malloc(sizeof(int));

	CU_ASSERT(dstru_init(0, &ds) == 0);
	CU_ASSERT(ds != NULL);
	CU_ASSERT(dstru_add_member(DYN_S_VOIDP, (void *) i, ds) == 0);

	v = (struct flat_4 *) ds->buffer;
	CU_ASSERT(v->a == i);

	dstru_free(ds);
	free(i);
}

void test_creation_normal1(void){
	uint32_t i1 = 700;
	uint32_t i2 = -700;

	struct dstru_struct *ds1;
	struct flat_1 *t;

   	CU_ASSERT(dstru_init(0, &ds1) == 0);
	CU_ASSERT(ds1 != NULL);

	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i1, ds1) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i2, ds1) == 0);

	t = (struct flat_1 *) ds1->buffer;

	CU_ASSERT(t->a == i1);
	CU_ASSERT(t->b == i2);
}

/* Main test. Creates different dynamic structures 
	on the heap using the dstru_add_member function . The byte-Buffer
	contained by the dystructs is then casted to a pointer
	with type of one of above specified abstract data types */
void test_creation_normal2(void){
	uint32_t i1 = 100123, i2 = -100123;
	double d = 100000.123123;
	uint8_t c1 = 'F', c2 = '\n';
	uint16_t s1 = 0x12FF;

	struct dstru_struct *ds1; 
	struct dstru_struct *ds2;
	struct flat_3 *ts1;
	struct flat_2 *ts2;

	CU_ASSERT(dstru_init(0, &ds1) == 0);
	CU_ASSERT(dstru_init(0, &ds2) == 0);

	CU_ASSERT(ds1 != NULL);
	CU_ASSERT(ds2 != NULL);
	
	/* ds2 => instance of the test2 structure */
	CU_ASSERT(dstru_add_member(DYN_S_UINT8, (void *) &c1, ds1) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i1, ds1) == 0);	
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s1, ds1) == 0);
	CU_ASSERT(dstru_finalize(ds1) == 0);
	
	/* ds1 => instance of the test2 structure*/	
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i2, ds2) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d, ds2) == 0);	
	CU_ASSERT(dstru_add_member(DYN_S_UINT8, (void *) &c2, ds2) == 0);
	CU_ASSERT(dstru_finalize(ds2) == 0);

	ts1 = (struct flat_3 *) ds1->buffer;
	ts2 = (struct flat_2 *) ds2->buffer;

	/*printf("\n");*/
	/*printf("Final Size ds1: %d ds2: %d\n", ds1->size, ds2->size);*/
	/*printf("Size of original test1: %ld test2: %ld\n", */
	/*sizeof(struct test1), sizeof(struct test2));*/

	CU_ASSERT(ts1->a == c1);
	CU_ASSERT(ts1->b == i1);
	CU_ASSERT(ts1->c == s1);

	CU_ASSERT(ts2->a == i2);
	CU_ASSERT(ts2->b == d);
	CU_ASSERT(ts2->c == c2);
}

void test_creation_aligned(void){
	uint32_t i1 = 100123, i2 = -100123;
	double d = 100000.123123;
	uint8_t c1 = 'F', c2 = '\n';
	uint16_t s1 = 0x12FF;

	struct dstru_struct *ds1; 
	struct dstru_struct *ds2;

	/* aligns all members with 2 bytes alignment */
	CU_ASSERT(dstru_init(2, &ds1) == 0);
	/* aligns all members with 4 bytes alignment */
	CU_ASSERT(dstru_init(4, &ds2) == 0);

	CU_ASSERT(ds1 != NULL);
	CU_ASSERT(ds2 != NULL);
	
	/* ds2 => instance of the test2 structure */
	CU_ASSERT(dstru_add_member(DYN_S_UINT8, (void *) &c1, ds1) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i1, ds1) == 0);	
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s1, ds1) == 0);
	CU_ASSERT(dstru_finalize(ds1) == 0);
	
	/* ds1 => instance of the test2 structure*/	
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i2, ds2) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d, ds2) == 0);	
	CU_ASSERT(dstru_add_member(DYN_S_UINT8, (void *) &c2, ds2) == 0);
	CU_ASSERT(dstru_finalize(ds2) == 0);


	CU_ASSERT(*((uint8_t *) ds1->buffer) == c1);
	CU_ASSERT(*((uint32_t *) ((uint8_t*) ds1->buffer + 2)) == i1);
	CU_ASSERT(*((uint16_t *) ((uint8_t*) ds1->buffer + 6)) == s1);

	CU_ASSERT(*((uint32_t *) ds2->buffer) == i2);
	CU_ASSERT(*((double *) ((uint8_t*) ds2->buffer + 4)) == d);
	CU_ASSERT(*((uint8_t *) ((uint8_t*) ds2->buffer + 12)) == c2);
}

void test_creation_nested(void){
	uint32_t i1 = 42, i2 = 43;
	uint16_t s1 = 500, s2 = -500;	
	double d1 = 10000.123332, d2 = -299234.33433;

	struct dstru_struct *dest;
	struct nested_1 *ts;

	CU_ASSERT(dstru_init(0, &dest) == 0);

	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i1, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i2, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s1, dest) == 0);

	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d1, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s2, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d2, dest) == 0);
	CU_ASSERT(dstru_finalize(dest) == 0);

	ts = (struct nested_1 *) dest->buffer;

	CU_ASSERT(ts->a == i1);
	CU_ASSERT(ts->b == i2);
	CU_ASSERT(ts->c == s1);
	CU_ASSERT(ts->anon.d1 == d1);
	CU_ASSERT(ts->anon.s == (uint16_t) s2);
	CU_ASSERT(ts->anon.d2 == d2);
}

void test_creation_nested_2(void){
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

	CU_ASSERT(dstru_init(0, &dest) == 0);

	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i1, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT32, (void *) &i2, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s1, dest) == 0);

	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d1, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_UINT16, (void *) &s2, dest) == 0);
	CU_ASSERT(dstru_add_member(DYN_S_DOUBLE, (void *) &d1, dest) == 0);

	CU_ASSERT(dstru_add_bytefield(32, (void *) foo1, dest) == 0);
	CU_ASSERT(dstru_add_array(4, DYN_S_UINT32, (void *) foo2, dest) == 0);

	CU_ASSERT(dstru_finalize(dest) == 0);

	result = (struct nested_2 *) dest->buffer;

	CU_ASSERT(result->anon1.a == i1);
	CU_ASSERT(result->anon1.b == i2);
	CU_ASSERT(result->anon1.c == s1);

	CU_ASSERT(result->anon2.d1 == d1);
	CU_ASSERT(result->anon2.s == s2);
	CU_ASSERT(result->anon2.d2 == d1);

	CU_ASSERT(memcmp(result->anon3.string, foo1, 32) == 0);
	for (cnt = 0; cnt < 4; cnt++)
		CU_ASSERT(result->anon3.arr[cnt] == cnt + 42);

	CU_ASSERT(dest->size == sizeof(struct nested_2));
}

void test_sizeof(void){
	CU_ASSERT(dstru_sizeof(DYN_S_UINT64, NULL) == 8); 
	CU_ASSERT(dstru_sizeof(DYN_S_UINT8, NULL) == 1); 
	CU_ASSERT(dstru_sizeof(DYN_S_UINT16, NULL) == 2); 
	CU_ASSERT(dstru_sizeof(DYN_S_UINT32, NULL) == sizeof(uint32_t)); 
	CU_ASSERT(dstru_sizeof(DYN_S_DOUBLE, NULL) == sizeof(double)); 
}

/* Tests whether the padding is calculated correctly */
void test_padding_normal(void){
	struct dstru_struct *foo;
	CU_ASSERT(dstru_init(0, &foo) == 0);

	/* uint32 */
	foo->size = 0;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo) == 0);
	foo->size = 1;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo) == 3);
	foo->size = 2;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo) == 2);
	foo->size = 3;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo) == 1);

	/* double */
	foo->size = 4;
	CU_ASSERT(dstru_padding(DYN_S_DOUBLE, foo) == 4);

	/* uint16 */
	foo->size = 1;
	CU_ASSERT(dstru_padding(DYN_S_UINT16, foo) == 1);
	foo->size = 2;
	CU_ASSERT(dstru_padding(DYN_S_UINT16, foo) == 0);

	/* uint8 */	
	CU_ASSERT(dstru_padding(DYN_S_UINT8, foo) == 0);
	CU_ASSERT(dstru_padding(DYN_S_UINT8, foo) == 0);

	/* voidpointer */
	foo->size = 4;
	CU_ASSERT(dstru_padding(DYN_S_VOIDP, foo) == 4);

	dstru_free(foo);
}

void test_padding_aligned(void){
	struct dstru_struct *foo1, *foo2, *foo3;
	CU_ASSERT(dstru_init(1, &foo1) == 0);
	CU_ASSERT(dstru_init(2, &foo2) == 0);
	CU_ASSERT(dstru_init(4, &foo3) == 0);

	/* uint32 */
	foo1->size = 0;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo1) == 0);
	foo1->size = 1;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo1) == 0);
	foo1->size = 2;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo1) == 0);
	foo1->size = 3;
	CU_ASSERT(dstru_padding(DYN_S_UINT32, foo1) == 0);

	foo2->size = 0;
	CU_ASSERT(dstru_padding(DYN_S_UINT16, foo2) == 0);
	foo2->size = 5;
	CU_ASSERT(dstru_padding(DYN_S_UINT8, foo2) == 1);
	foo2->size = 5;
	CU_ASSERT(dstru_padding(DYN_S_DOUBLE, foo2) == 1);

	foo3->size = 1;
	CU_ASSERT(dstru_padding(DYN_S_DOUBLE, foo3) == 3);
	foo3->size = 2;
	CU_ASSERT(dstru_padding(DYN_S_DOUBLE, foo3) == 2);
	foo3->size = 5;
	CU_ASSERT(dstru_padding(DYN_S_FLOAT, foo3) == 3);

	dstru_free(foo1);
	dstru_free(foo2);
	dstru_free(foo3);
}

/* Required calls to CUnit. Test will be registered  */
int main(int argc, char **argv){
	/* Initialize and build a Testsuite */
	CU_pSuite pSuite = NULL;


	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* Creation of the main test suite */
	pSuite = CU_add_suite("Dynamic C-Heap Structures", init_test_dyn_str, 
							clean_test_dyn_str); 
	if (NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Add tests */
	if((NULL == CU_add_test(pSuite, "Creation of structs", test_init)) || 
		(NULL == CU_add_test(pSuite, "Voidpointer test", test_voidp)) || 
		(NULL == CU_add_test(pSuite, "Sizeof test", test_sizeof)) ||
		(NULL == CU_add_test(pSuite, "Member adding test 1", test_creation_normal1)) ||	
		(NULL == CU_add_test(pSuite, "Member adding test 2", test_creation_normal2)) || 
		(NULL == CU_add_test(pSuite, 
				"Member adding test with alignment", 
				test_creation_aligned)) ||
		(NULL == CU_add_test(pSuite,
				"Member adding test with nested structs",
				test_creation_nested)) ||
		(NULL == CU_add_test(pSuite, 
				"Second member adding test with nested structs", 
				test_creation_nested_2)) ||
		(NULL == CU_add_test(pSuite, 
				"Padding test with alignment", 
				test_padding_aligned)) ||
		(NULL == CU_add_test(pSuite, 
				"Padding test with platform alignment", 
				test_padding_normal)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	printf("\nSome sizeof considerations...\n");
	printf("sizeof(nested_1) :%ld\n", sizeof(struct nested_1));
	printf("sizeof(nested_2) :%ld\n", sizeof(struct nested_2));
	printf("sizeof(nested_3) :%ld\n", sizeof(struct nested_3));
	printf("sizeof(nested_3_part_1) :%ld\n", sizeof(struct nested_3_part_1));
	printf("sizeof(nested_3_part_2) :%ld\n", sizeof(struct nested_3_part_2));
	printf("sizeof(nested_4) :%ld\n", sizeof(struct nested_4));

	/* misc configuration */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_suite(pSuite);

	if (CU_get_number_of_tests_failed())
		return EXIT_FAILURE;

	CU_cleanup_registry();

	return CU_get_error();
}
