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
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "dstru_funcs.h"
#include "dstru_defines.h"
#include "dstru_types.h"

#include "dstru_util_funcs.h"

int dstru_sizeof(int type, struct dstru_struct *content){
	switch(type){
		case DYN_S_UINT8 : return sizeof(uint8_t);
		case DYN_S_UINT16: return sizeof(uint16_t);
		case DYN_S_UINT32: return sizeof(uint32_t);
		case DYN_S_UINT64: return sizeof(uint64_t);
		case DYN_S_FLOAT : return sizeof(float);
		case DYN_S_DOUBLE: return sizeof(double);
		case DYN_S_VOIDP : return sizeof(void*);
		case DYN_S_STRUCT: return content->size;
		default: return 1;
	}
}

int dstru_padding(int type, struct dstru_struct *s){
	if (s == NULL || type < 0)
		return 1;

	/* Full structure packing enabled. No alignment padding! */
	if(s->align == 1){
		return 0; 
	} else if (s->align >= 1){
		return (s->align - (s->size % s->align)) % s->align;
	/* No packing. Alignment follows the DYN_S_AL_* defines*/
	} else {
		switch(type){
			case DYN_S_UINT8: 
				return 0;
			case DYN_S_STRUCT:
			case DYN_S_UINT16:
				return   (DYN_S_AL_UINT16 - (s->size % DYN_S_AL_UINT16)) %
				DYN_S_AL_UINT16;
			case DYN_S_UINT32:
				return   (DYN_S_AL_UINT32 - (s->size % DYN_S_AL_UINT32)) %
				DYN_S_AL_UINT32;
			case DYN_S_UINT64:
				return   (DYN_S_AL_UINT64 - (s->size % DYN_S_AL_UINT64)) %
				DYN_S_AL_UINT64;
			case DYN_S_FLOAT:
				return   (DYN_S_AL_FLOAT - (s->size % DYN_S_AL_FLOAT)) %
				DYN_S_AL_FLOAT;
			case DYN_S_DOUBLE:
				return   (DYN_S_AL_DOUBLE - (s->size % DYN_S_AL_DOUBLE)) %
				DYN_S_AL_DOUBLE;
			case DYN_S_VOIDP:
				return   (DYN_S_AL_VOIDP - (s->size % DYN_S_AL_VOIDP)) %
				DYN_S_AL_VOIDP;
			default: 
				return 0;
		}	
	}
}

int dstru_finalize(struct dstru_struct *dest){
	int new_size;
	if (!dstru_is_power_of_two(dest->align) 
		&& (dest->align != 0)
		|| dest == NULL)
		return 1;

	/* biggest_member alignment cannot be bigger than 8 */
	if (dest->biggest_member > 8){
		return 1;
	} else {
		new_size = dest->size + 
			dstru_padding(dstru_lookup_type(dest->biggest_member), dest);
	}

	dest->size = new_size;
	dest->buffer = realloc(dest->buffer, new_size);

	if(dest->buffer == NULL)
		return 1;
	
	return 0;
}

int dstru_add_member(int type, void *content, struct dstru_struct *dest){
	int pad_size, new_size;
	uint8_t *tempv, *tempp;
	int vi;

	if (!dstru_is_power_of_two(dest->align) && (dest->align != 0))
		return 1;

	if(dest == NULL || content == NULL)
		return 1;	

	/* element adding logic begins here */
	new_size = dest->size + 
				dstru_padding(type, dest) + 
				dstru_sizeof(type, content);

	pad_size = new_size - dstru_sizeof(type, content);

	dest->buffer = realloc(dest->buffer, new_size);
	if(dest->buffer == NULL)
		return 1;

	dest->size = new_size;
	dest->elem_num++;

	/* update the biggest_member field if the new field 
	 	will be bigger than every other field before (dest->buffer) */
	if (dest->biggest_member < dstru_sizeof(type,content)){
		dest->biggest_member = dstru_sizeof(type,content);
	}

	/* type dependent adding logic */
	switch(type){
		case DYN_S_STRUCT:
			/* copies the field into the own buffer */
			if(!memcpy(((uint8_t *) dest->buffer) + pad_size, 
						((struct dstru_struct *)content)->buffer, 
						((struct dstru_struct *)content)->size))
				return 1;
			break;
		case DYN_S_UINT32:
			/* Memory has been allocated at this point 
				Copy data from c to the buffer 
			 	Can be understood as.
				1) Get startaddress (new_size - sizeof(int) = number of padding
				2) Cast the address appropriate
				3) Copy the castet content of c into the memory */
			*((uint32_t *) (((uint8_t *) dest->buffer) + pad_size)) = 
				*((uint32_t *) content);
			break;
		case DYN_S_UINT8:
			/* Memory has been allocated at this point */
			*((uint8_t *) (((uint8_t *) dest->buffer) + pad_size)) = 
				*((uint8_t *) content);
			break;
		case DYN_S_UINT16:
			/* Memory has been allocated at this point */
			*((uint16_t *) (((uint8_t *) dest->buffer) + pad_size )) = 
				*((uint16_t *) content);
			break;
		case DYN_S_UINT64:
			/* Memory has been allocated at this point */
			*((uint64_t *) (((uint8_t *) dest->buffer) + pad_size)) = 
				*((uint64_t *) content);
			break;
		case DYN_S_FLOAT:
			/* Memory has been allocated at this point */
			*((float *) (((uint8_t *) dest->buffer) + pad_size )) = 
				*((float *) content);
			break;
		case DYN_S_DOUBLE:
			/* Memory has been allocated at this point */
			*((double *) (((uint8_t *) dest->buffer) + pad_size )) = 
				*((double *) content);
			break;
		case DYN_S_VOIDP:
			/* The tricky bit */
			tempv = ((uint8_t *) dest->buffer) + 
					(new_size - dstru_sizeof(type, content));
			tempp = (uint8_t *) &content;

			for (vi=0; vi< dstru_sizeof(type, content); vi++)
				tempv[vi] = tempp[vi];

			break;
		default:
			return 1;
	}

	return 0;
} 

int dstru_add_bytefield(int size, void *content, struct dstru_struct *dest){
	int i;

	if (!dstru_is_power_of_two(dest->align) 
		&& (dest->align != 0) 
		|| dest == NULL 
		|| content == NULL)
		return 1;	

	for (i = 0; i < size; i++)
		dstru_add_member(DYN_S_UINT8, (uint8_t *) content + i, dest);

	return 0;
}

int dstru_add_array(int num, int arr_member_type, 
		void *content, struct dstru_struct *dest){
	int i;

	if (!dstru_is_power_of_two(dest->align) 
		&& (dest->align != 0) 
		|| dest == NULL 
		|| content == NULL)
		return 1;	

	for (i = 0; i < num; i++){
		switch(arr_member_type){
			case DYN_S_UINT8:
				if (dstru_add_member(arr_member_type, 
							(uint8_t *) content + i, dest))
					return 1;

				break;
			case DYN_S_UINT16:
				if (dstru_add_member(arr_member_type, 
							(uint16_t *) content + i, dest))
					return 1;

				break;
			case DYN_S_UINT32:
				if (dstru_add_member(arr_member_type, 
							(uint32_t *) content + i, dest))
					return 1;

				break;
			case DYN_S_UINT64:
				if (dstru_add_member(arr_member_type, 
							(uint64_t *) content + i, dest))
					return 1;

				break;
			case DYN_S_FLOAT:
				if (dstru_add_member(arr_member_type, 
							(float *) content + i, dest))
					return 1;

				break;
			case DYN_S_DOUBLE:
				if (dstru_add_member(arr_member_type, 
							(double *) content + i, dest))
					return 1;

				break;
			case DYN_S_VOIDP:
				return 1;
			case DYN_S_STRUCT:
				return 1;
			default : 
				return 1;
		}
	}

	return 0;
}

int dstru_add_uint16(uint16_t i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_UINT16, (void *) &i, ds);
}

int dstru_add_uint32(uint32_t i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_UINT32, (void *) &i, ds);
}

int dstru_add_double(double i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_DOUBLE, (void *) &i, ds);
}

int dstru_add_float(float i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_FLOAT, (void *) &i, ds);	
}

int dstru_add_char(uint8_t i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_UINT8, (void *) &i, ds);	
}

int dstru_add_voidp(void *i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_VOIDP, i, ds);	
}

int dstru_add_long(uint64_t i, struct dstru_struct *ds){
	return dstru_add_member(DYN_S_UINT64, (long *) &i, ds);	
}

int dstru_init(int align, struct dstru_struct **s){
	struct dstru_struct *ret;
	ret = malloc(sizeof(struct dstru_struct));

	if (ret == NULL || align < 0 || !dstru_is_power_of_two(align) && align != 0)
		return 1;

	ret->buffer = NULL;
	ret->size = 0;
	ret->elem_num = 0;
	ret->align = align;
	ret->biggest_member = 0;

	*s = ret;

	return 0;
}

int dstru_free(struct dstru_struct *s){
	if (s == NULL)
		return 1;

	free(s->buffer);
	free(s);
	return 0;
}
