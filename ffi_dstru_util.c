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

#include "ffi_dstru_util.h"
#include "ffi_dstru.h"

static const int TYPE_LOOKUP[] = {
	0, DYN_S_UINT8, DYN_S_UINT16, 
	0, DYN_S_UINT32, 0, 0, 0, DYN_S_UINT64
}; 

int dstru_lookup_type(int t){
	return TYPE_LOOKUP[t];
}

int dstru_is_power_of_two(int t){
	return ((t != 0) && !(t & (t - 1)));
}
