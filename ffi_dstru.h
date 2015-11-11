/*
Copyright (c) 2014 Florian Mayer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, and/or sell
copies of the Software, and o permit persons to whom the Software is
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

#ifndef DSTRU
#define DSTRU

#include <stdint.h>
#include <stdbool.h>


/* Enum to string mapping generator. */
#define DYN_S_TYPE(F)       \
    F(DYN_S_UINT8)          \
    F(DYN_S_UINT16)         \
    F(DYN_S_UINT32)         \
    F(DYN_S_UINT64)         \
    F(DYN_S_FLOAT)          \
    F(DYN_S_DOUBLE)         \
    F(DYN_S_VOIDP)          \
    F(DYN_S_STRUCT)         \
    F(DYN_S_FIELD)          \

/* Produces quoted, komma separated strings from DYN_S_TYPE*/
#define GENERATE_STRING(X) #X,
/* Produces unquoted, komma separated enum names from DYN_S_TYPE*/
#define GENERATE_ENUM(X) X,

/* Wrapper types */
enum dstru_types {
    DYN_S_TYPE(GENERATE_ENUM)
};

/* Alignment configuration */
#ifndef DSTRU_UINT_8_AL
#	define DYN_S_AL_UINT8 1
/* todo add more platforms */
#else
#	define DYN_S_AL_UINT8 DSTRU_UINT_8_AL
#endif

#ifndef DSTRU_UINT_16_AL
#	define DYN_S_AL_UINT16 2
/* todo add more platforms */
#else
#	define DYN_S_AL_UINT16 DSTRU_UINT_16_AL
#endif

#ifndef DSTRU_UINT_32_AL
#	define DYN_S_AL_UINT32 4
#else
#	define DYN_S_AL_UINT32 DSTRU_UINT_32_AL
#endif

#ifndef DSTRU_UINT_64_AL
#	define DYN_S_AL_UINT64 8
#else
#	define DYN_S_AL_UINT64 DSTRU_UINT_64_AL
#endif

#ifndef DSTRU_FLOAT_AL
#	define DYN_S_AL_FLOAT 4
#else
#	define DYN_S_AL_FLOAT DSTRU_FLOAT_AL
#endif

#ifndef DSTRU_DOUBLE_AL
#	define DYN_S_AL_DOUBLE 8
#else
#	define DYN_S_AL_DOUBLE DSTRU_DOUBLE_AL
#endif

#ifndef DSTRU_PTR_AL
#	define DYN_S_AL_VOIDP 8
#else
#	define DYN_S_AL_VOIDP DSTRU_PTR_AL
#endif

#ifndef DSTRU_WORDSIZE
#	define DYN_S_AL_WS 8
#else
#	define DYN_S_AL_WS 8
#endif

#define DSTRU_MEMBER_INIT 10

struct dstru_member {
    int offset;
    int size;
    bool is_dstru;
};

/* align = 0 => The Library takes care of member Alignment
 	align in f(x)=2^x => Alignment will be f(x) 
 	elem_num = Number of first level members
 	    embeddet Bitfields (e.g. of structs) count as one.
 	size = Number of bytes occupied by buffer. 
 	biggest_member = Number of bytes of the biggest member */
struct dstru_struct {
	void *buffer;
	int size;
	int elem_num;
	int align;
	int biggest_member;
    struct {
        struct dstru_member *members; 
        int member_count;
        int members_size;
    } member_table;
};

/* Adds a member to a dynamic structure without respection correct alignment
	Param: 
		type: The members type
		content: The members data
		ds: A Pointer to an initialized Struct	
	Return: 0 on success
		-1 on failure */
int dstru_add_member(enum dstru_types type, void *content, struct dstru_struct *ds);

int dstru_add_entry(struct dstru_struct *s, int offset, int size, bool is_dstru);

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
int dstru_padding(enum dstru_types type, struct dstru_struct *s);
/* Helper function for dstru_add_member.
 	returns the size of a #defined type.
	-----------------------------------
 	Param: type = Type as specified in dstru_defines.h
 		s = instance of an dstru_struct structrue 
 	Return: Number of bytes occupied by the type */
int dstru_sizeof(enum dstru_types type, struct dstru_struct *s);

#endif /* DSTRU */
