/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_FFI_FFI_PARSER_H_INCLUDED
# define YY_FFI_FFI_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef FFIDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define FFIDEBUG 1
#  else
#   define FFIDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define FFIDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined FFIDEBUG */
#if FFIDEBUG
extern int ffidebug;
#endif
/* "%code requires" blocks.  */
#line 44 "ffi_parser.y" /* yacc.c:1909  */

#define YYSTYPE FFISTYPE

#line 56 "ffi_parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef FFITOKENTYPE
# define FFITOKENTYPE
  enum ffitokentype
  {
    arrow = 258,
    comma = 259,
    equals = 260,
    pointer = 261,
    array = 262,
    curlopen = 263,
    curlclose = 264,
    paropen = 265,
    parclose = 266,
    string = 267,
    c_type = 268,
    s_type = 269,
    pipe_del = 270
  };
#endif

/* Value type.  */
#if ! defined FFISTYPE && ! defined FFISTYPE_IS_DECLARED

union FFISTYPE
{
#line 27 "ffi_parser.y" /* yacc.c:1909  */

	void *v;
	struct nary_node *k;

#line 89 "ffi_parser.h" /* yacc.c:1909  */
};

typedef union FFISTYPE FFISTYPE;
# define FFISTYPE_IS_TRIVIAL 1
# define FFISTYPE_IS_DECLARED 1
#endif



int ffiparse (struct nary_node **root, void *scan);

#endif /* !YY_FFI_FFI_PARSER_H_INCLUDED  */
