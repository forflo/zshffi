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


#ifndef DSTRU_FUNCS
#define DSTRU_FUNCS

#include "dstru_types.h"

/* Adds a member to a dynamic structure without respection correct alignment
	Param: *rc: Returncodes from this function
		t: The members type
		c: The members data
		ds: A Pointer to an initialized Struct	
	Return: 0 on success
		-1 on failure*/
int dstru_add_member(int type, void *content, struct dstru_struct *ds);

/* Fills the buffer with the required number of padding bytes 
 	Param: ds = instance of an dstru_struct structure
 	Return: 0 if successfull, 1 if not */
int dstru_finalize(struct dstru_struct *ds);

/* For convenience. All these function use dstru_add_member internally */ 
int dstru_add_uint8(uint8_t i, struct dstru_struct *ds);
int dstru_add_uint16(uint16_t i, struct dstru_struct *ds);
int dstru_add_uint32(uint32_t i, struct dstru_struct *ds);
int dstru_add_uint64(uint64_t i, struct dstru_struct *ds);
int dstru_add_float(float i, struct dstru_struct *ds);
int dstru_add_double(double i, struct dstru_struct *ds);
int dstru_add_voidp(void *i, struct dstru_struct *ds);
/* Will add size-bytes to dest, which are read from *content 
 	Notice, that the added bytes will be aligned to 0 bytes constraint */
int dstru_add_bytefield(int size, void *content, struct dstru_struct *dest);
/* Will add elem_num times sizeof(arr_elem_type) bytes big chunks of data
 	read from *content to the buffer of *dest */
int dstru_add_array(int elem_num, int arr_elem_type, void *content, struct dstru_struct *dest);

/* Returns a newly initialized dstru_struct object 
	Param: aling = Alignment specs (see also #pragma pack(x))
		s = The pointer address to use
	Return: 0 on success, 1 on error */
int dstru_init(int align, struct dstru_struct **s);
/* Frees a dynamic structure 
 	Param: s = Pointer to the structure to free
	Return: 0 on success, 1 on error */
int dstru_free(struct dstru_struct *s);

/* Caluclates the number of required padding bytes
	between the structures offset (s->size) and the start address
	using the following formula: p = a - (o % a) % a, 
	where p is the number of paddingbytes,
	a is the alignment and o ist the initial address offset 
	-------------------
	Param: t = Datatype
		s = instance of the structure
	Return: Number of padding bytes 
 	Semantics: 
		1) s->align can only be of the values {0, 1, 2, 4, 8, 16 ...}
			no checks necessary since dstru_init already verfied, that 
			s->align is a power of two.
		2) type can only be the values DYN_S_* defined in dstru_defines.h */
int dstru_padding(int type, struct dstru_struct *s);
/* Helper function for dstru_add_member.
 	returns the size of a #defined type.
	-----------------------------------
 	Param: type = Type as specified in dstru_defines.h
 		s = instance of an dstru_struct structrue 
 	Return: Number of bytes occupied by the type */
int dstru_sizeof(int type, struct dstru_struct *s);

#endif
