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

#ifndef DSTRU_TEST_TYPES
#define DSTRU_TEST_TYPES

/* Flat structures */
struct flat_3 { 
	char a; 
	int b; 
	short c; 
};

struct flat_2 { 
	int a; 
	double b; 
	char c; 
};

struct flat_1 { 
	int a; 
	int b; 
};

struct flat_4 {
	void *a;
};

/* Nested structures and their components */
struct nested_1 { 
	int a; 
	int b; 
	short c;
	struct { 
		double d1; 
		uint16_t s; 
		double d2;
	} anon; 
};

struct nested_2 {
	struct { 
		uint32_t a; 
		uint32_t b; 
		uint16_t c; 
	} anon1; 

	struct { 
		double d1; 
		uint16_t s; 
		double d2; 
	} anon2; 

	struct { 
		uint8_t string[32]; 
		uint32_t arr[4]; 
	} anon3;
};

struct nested_4 {
	struct { 
		int a; 
		int b; 
		short c; 
	} anon1; 

	struct { 
		double d1; 
		uint16_t s; 
		double d2; 
	} anon2; 

	struct { 
		uint8_t string[7]; 
		int arr[21]; 
	} anon3;
};

struct nested_3_part_1 {
	uint32_t a;
	double b;
};

struct nested_3_part_2 {
	uint8_t name[8];
	uint8_t addr[8];	
};

struct nested_3 {
	struct nested_3_part_1 a;
	struct nested_3_part_2 b;
};

#endif
