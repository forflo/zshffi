%{

#include "ffi_node_defines.h"
#include "ffi_nary_tree.h"
#include "ffi_parser_util.h"
#include <stdio.h>
#include <stdlib.h>

//extern int ffilex();
void yyerror(struct nary_node **root, void *scanner, const char *str){
	fprintf(stderr, "[Non-Interactive] %s\n", str);
}

extern int ffilex();

int start_cnt = 0;
int tval_cnt = 0;
int tval_lst_cnt = 0;
int s_cnt = 0;
int sarr_cnt = 0;
int sptr_cnt = 0;
int c_cnt = 0;
int cptr_cnt = 0;
int carr_cnt = 0;
int val_cnt = 0;

%}

%union {
	struct token_value *v;
	struct nary_node *k;
}

/* Make the generated parser reentrant */
%define api.prefix "ffi"
%define api.pure full
%output "ffi_parser.c"

/* Adjust the calling conventions for the lexter. See
	http://www.phpcompiler.org/articles/reentrantparser.html for
	more details */
%parse-param {struct nary_node **root}
%parse-param {void *scan}
%lex-param {void *scan}

%code requires {
#define YYSTYPE FFISTYPE
}

%token arrow comma equals pointer
%token curlopen curlclose 
%token string c_type s_type

%type <k> type_and_val start
%type <k> tval_list scalar value compound
%type <v> s_type c_type string

%start start 

%%

start               : type_and_val
                        { *root = make_node(start_cnt++, NT_START, NULL, 1, $1); }
                    ;

type_and_val        : scalar
                        {$$ = make_node(tval_cnt++, NT_TYPEANDVAL, NULL, 1, $1);  }
                    | compound
                        {$$ = make_node(tval_cnt++, NT_TYPEANDVAL, NULL, 1, $1);  }
                    ;

tval_list           : type_and_val
                        {$$ = make_node(tval_lst_cnt++, NT_TVAL_LIST, NULL, 1, $1);  }
                    | tval_list comma type_and_val
                        {$$ = make_node(tval_lst_cnt++, NT_TVAL_LIST, NULL, 2, $1, $3);  }
                    ;

scalar              : s_type equals value
                        {$$ = make_node(s_cnt++, NT_SCALAR    , $1, 1, $3); }
                    | s_type pointer equals value
                        {$$ = make_node(sptr_cnt++, NT_SCALAR_PTR, $1, 1, $4); }
                    ;

compound            : c_type equals curlopen tval_list curlclose
                        {$$ = make_node(c_cnt++, NT_COMPOUND    , $1, 1, $4); }
                    | c_type pointer equals curlopen tval_list curlclose
                        {$$ = make_node(cptr_cnt++, NT_COMPOUND_PTR, $1, 1, $5); }
                    ;

value               : string
                        {$$ = make_node(val_cnt++, NT_VALUE, $1, 0); }
                    ;

%%
