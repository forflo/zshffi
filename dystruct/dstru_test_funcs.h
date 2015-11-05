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

#ifndef DSTRU_TEST_FUNCS
#define DSTRU_TEST_FUNCS

/* Required by CUnit. They do nothing */
int init_test_dyn_str(void);
int clean_test_dyn_str(void);

/* Creation and destruction test */
void test_init(void);

/* Tests whether the padding is calculated correctly */
void test_padding_normal(void);
void test_padding_aligned(void);

/* Tests wheter the function dstru_sizeof() works correctly */
void test_sizeof(void);

/* Test for adding voidpointer */
void test_voidp(void);

/* Tests the creation of structures under several circumstances*/
void test_creation_normal1(void);
void test_creation_normal2(void);
void test_creation_aligned(void);
void test_creation_nested(void);
void test_creation_field(void);

#endif
