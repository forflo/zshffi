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

#ifndef DSTRU_DEFINES
#define DSTRU_DEFINES

#include "config.h"

/* Wrapper types */
#define DYN_S_UINT8 1001
#define DYN_S_UINT16 1002
#define DYN_S_UINT32 1000
#define DYN_S_UINT64 1003
#define DYN_S_FLOAT 1004
#define DYN_S_DOUBLE 1005
#define DYN_S_VOIDP 1006
#define DYN_S_STRUCT 1007
#define DYN_S_FIELD 1008

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

#endif /* DSTRU_DEFINES */
