%{
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void yyerror(struct nary_node **root, void *scanner, const char *str){
	fprintf(stderr, "[Interactive] %s\n", str);
}

%}

%union {
	struct value *v;
	struct nary_node *k;
}

/* Make the generated parser reentrant */
%define api.prefix "iwet"
%define api.pure full

%output "iwetparse.c"

/* Adjust the calling conventions for the lexter. See
	http://www.phpcompiler.org/articles/reentrantparser.html for
	more details */
%parse-param {struct nary_node **root}
%parse-param {void *scan}
%lex-param {void *scan}

%code requires {
#define YYSTYPE IWETSTYPE
}

/* Operators, their precedence and associativity */
%left OR
%left AND
%left BINOR
%left BINXOR
%left BINAND
%left EQUAL NOTEQUAL
%left GREATERTHAN LESSTHAN GREATER LESS
%left LEFTSHIFT RIGHTSHIFT
%left PLUS MINUS
%left MUL DIV MOD
%left POW
%left COMPL NOT UMINUS
%left CAST

/* Assignment operators */
%token ASSOP ASPLUS ASMINUS ASMUL ASDIV ASMOD ASPOW ASBINOR ASBINAND 
%token ASBINXOR ASRIGHTSHIFT ASLEFTSHIFT
/* Hot things */
%token KEYSTROKE HOTSTRING ON
/* Statements*/
%token IF GLOBAL ELSE ELIF CONTINUE BREAK UTIL FOR 
%token IN WHILE DO SWITCH PRINT INC DEC DEFAULT
%token RET CASE FUNCTION EXTERNAL TRUEV FALSEV
/* Literals */
%token STRING NUMBER ID FCELANG FCEB_CODE INT
/* Token for FFI */
%token FFI_CHAR FFI_SHORT FFI_INT FFI_LONG FFI_LONG_LONG FFI_DOUBLE 
%token FFI_FLOAT FFI_LONG_DOUBLE FFI_VOIDPTR
/* Symbols*/
%token CURLOPEN CURLCLOSE PAROPEN PARCLOSE BOXOPEN BOXCLOSE SEMI DP POINT 
%token COMMA SHARP DOLLAR QMARK
%token NL

%type <v> FFI_CHAR FFI_SHORT FFI_INT FFI_LONG FFI_LONG_LONG FFI_DOUBLE 
%type <v> FFI_FLOAT FFI_LONG_DOUBLE FFI_VOIDPTR
%type <v> NUMBER TRUEV FALSEV INT
%type <v> STRING ID FCELANG FCEB_CODE
%type <k> functioncall expression evalexpression fceexp fceexp_rc 
%type <k> listconstructor ffi_struct_def var_exp block stmtlist statement 
%type <k> fceblock assignment functiondef explist idlist 
%type <k> switchblock arglist parlist hotstringdef varlist
%type <k> hotkeydef castlist program ffi_cast complex_cast 
%type <k> elif_block_list elif_block
%type <k> case_stmtlist case_statement

%start program

%%

program 			: statement 
						{ *root = make_node(P_OP_STMT, NULL, 1, $1); YYACCEPT; }
					;

block 				: CURLOPEN stmtlist CURLCLOSE 
						{$$ = make_node(P_OP_STMTLST, NULL, 1, $2);}
					;

elif_block			: ELIF PAROPEN expression PARCLOSE block
						{$$ = make_node(P_OP_ELIFBLK, NULL, 2, $3, $5);}
					;

elif_block_list		: elif_block_list elif_block
						{$$ = make_node(P_OP_ELIFLST, NULL, 2, $1, $2); }
					| elif_block
						{$$ = make_node(P_OP_ELIFBLK, NULL, 1, $1);}
					;

switchblock			: CURLOPEN NL case_stmtlist CURLCLOSE 
						{$$ = make_node(P_OP_SWBLOCK, NULL, 1, $3);}
					| CURLOPEN case_stmtlist CURLCLOSE 
						{$$ = make_node(P_OP_SWBLOCK, NULL, 1, $2);}
					| CURLOPEN NL case_stmtlist DEFAULT stmtlist CURLCLOSE
						{$$ = make_node(P_OP_SWBLOCK, NULL, 2, $3, $5);}
					| CURLOPEN case_stmtlist DEFAULT stmtlist CURLCLOSE
						{$$ = make_node(P_OP_SWBLOCK, NULL, 2, $2, $4);}
					;

case_stmtlist		: case_statement
						{$$ = make_node(P_OP_CASESTMT, NULL, 1, $1);}
					| case_stmtlist case_statement 
						{$$ = make_node(P_OP_CASELST, NULL, 2, $1, $2);}
					;

case_statement		: CASE PAROPEN expression PARCLOSE stmtlist 
						{$$ = make_node(P_OP_CASESTMT, NULL, 2, $3, $5);}
					;

explist				: expression 
						{$$ = make_node(P_OP_EXP, NULL, 1, $1);}
					| explist COMMA expression 
						{$$ = make_node(P_OP_EXPLST, NULL, 2, $1, $3);}
					;

varlist 			: var_exp
						{$$ = make_node(P_OP_VAREXP, NULL, 1, $1);}
					| varlist COMMA var_exp
						{$$ = make_node(P_OP_VARLST, NULL, 2, $1, $3);}	
					;

castlist			: castlist COMMA ffi_cast
					| castlist COMMA complex_cast
					| ffi_cast
					| complex_cast
					;

parlist 			: PAROPEN idlist PARCLOSE 
						{ $$ = make_node(P_OP_PARLST, NULL, 1, $2); }
					| PAROPEN PARCLOSE 
						{ $$ = make_node(P_OP_PARLST, NULL, 0); }
					;

arglist				: PAROPEN explist PARCLOSE 
						{ $$ = make_node(P_OP_ARGLST, NULL, 1, $2); }
					| PAROPEN PARCLOSE 
						{ $$ = make_node(P_OP_ARGLST, NULL, 0); }
					;

idlist				: ID 
						{ printf("debug: %s\n", 
							(char *)((struct value*)$1)->c);$$ = 
								make_node(ID, $1, 0); }
					| idlist COMMA ID 
						{ $$ = make_node(P_OP_IDLST, NULL, 2, 
							$1, make_node(ID, $3, 0)); }
					;

stmtlist 			: statement 
						{ $$ = make_node(P_OP_STMT, NULL, 1, $1); }
					| stmtlist statement 
						{$$ = make_node(P_OP_STMTLST, NULL, 2, $1, $2);}
					;

assignment 			: varlist ASSOP explist 
						{ $$ = make_node(P_OP_ASSOP, NULL, 2, $1, $3);}
					| varlist ASPLUS explist 
						{$$ = make_node(P_OP_ASPLUS, NULL, 2, $1, $3); }
					| varlist ASMINUS explist 
						{$$ = make_node(P_OP_ASMINUS, NULL, 2, $1, $3);}
					| varlist ASMUL explist 
						{$$ = make_node(P_OP_ASMUL, NULL, 2, $1, $3);}
					| varlist ASDIV explist 
						{$$ = make_node(P_OP_ASDIV, NULL, 2, $1, $3);}
					| varlist ASMOD explist 
						{$$ = make_node(P_OP_ASMOD, NULL, 2, $1, $3);}
					| varlist ASPOW explist 
						{$$ = make_node(P_OP_ASPOW, NULL, 2, $1, $3);}
					| varlist ASBINOR explist 
						{$$ = make_node(P_OP_ASBINOR, NULL, 2, $1, $3);}
					| varlist ASBINAND explist 
						{$$ = make_node(P_OP_ASBINAND, NULL, 2, $1, $3);}
					| varlist ASBINXOR explist 
						{$$ = make_node(P_OP_ASBINXOR, NULL, 2, $1, $3);}
					| varlist ASRIGHTSHIFT explist 
						{$$ = make_node(P_OP_ASRIGHTSHIFT, NULL, 2, $1, $3);}
					| varlist ASLEFTSHIFT explist 
						{$$ = make_node(P_OP_ASLEFTSHIFT, NULL, 2, $1, $3);}
					;


expression			: expression OR expression 	
						{ $$ = make_node(P_OP_OR, NULL, 2, $1, $3);}
					| expression AND expression 
						{ $$ = make_node(P_OP_AND, NULL, 2, $1, $3);}
					| expression BINOR expression	
						{ $$ = make_node(P_OP_BINOR, NULL, 2, $1, $3);}
					| expression BINXOR expression	
						{ $$ = make_node(P_OP_BINXOR, NULL, 2, $1, $3); }
					| expression BINAND expression	
						{ $$ = make_node(P_OP_BINAND, NULL, 2, $1, $3);}
					| expression EQUAL expression 	
						{ $$ = make_node(P_OP_EQUAL, NULL, 2, $1, $3); }
					| expression NOTEQUAL expression	
						{ $$ = make_node(P_OP_NOTEQUAL, NULL, 2, $1, $3); }
					| expression GREATERTHAN expression 
						{ $$ = make_node(P_OP_GREATERTHAN, NULL, 2, $1, $3);}
					| expression LESSTHAN expression 	
						{ $$ = make_node(P_OP_LESSTHAN, NULL, 2, $1, $3);}
					| expression GREATER expression 	
						{ $$ = make_node(P_OP_GREATER, NULL, 2, $1, $3);}
					| expression LESS expression	
						{ $$ = make_node(P_OP_LESS, NULL, 2, $1, $3);}
					| expression LEFTSHIFT expression 	
						{ $$ = make_node(P_OP_LEFTSHIFT, NULL, 2, $1, $3);}
					| expression RIGHTSHIFT expression	
						{ $$ = make_node(P_OP_RIGHTSHIFT, NULL, 2, $1, $3);}
					| expression PLUS expression 	
						{ $$ = make_node(P_OP_PLUS, NULL, 2, $1, $3);}
					| expression MINUS expression	
						{ $$ = make_node(P_OP_MINUS, NULL, 2, $1, $3);}
					| expression MUL expression 	
						{ $$ = make_node(P_OP_MUL, NULL, 2, $1, $3);}
					| expression DIV expression 	
						{ $$ = make_node(P_OP_DIV, NULL, 2, $1, $3);}
					| expression MOD expression	
						{ $$ = make_node(P_OP_MOD, NULL, 2, $1, $3);}
					| expression POW expression	
						{ $$ = make_node(P_OP_POW, NULL, 2, $1, $3);}
					| MINUS expression %prec UMINUS 
						{ $$ = make_node(P_OP_UMINUS, NULL, 1, $2);}
					| COMPL expression	
						{ $$ = make_node(P_OP_COMPL, NULL, 1, $2);}
					| NOT expression	
						{ $$ = make_node(P_OP_NOT, NULL,1,  $2);}
					| listconstructor 
						{ $$ = make_node(P_OP_LSTCONST, NULL, 1, $1);}
					| ffi_struct_def 
						{ $$ = make_node(P_OP_STRDEF, NULL, 1, $1);}
					| NUMBER 
						{ $$ = make_node(NUMBER, $1, 0); }
					| STRING 
						{ $$ = make_node(STRING, $1, 0); }
					| TRUEV
						{ $$ = make_node(TRUEV, $1, 0); }
					| INT
						{ $$ = make_node(INT, $1, 0); }
					| FALSEV 
						{ $$ = make_node(FALSEV, $1, 0); }
					| evalexpression	
						{ $$ = make_node(P_OP_EVEXP, NULL, 1, $1); }
					| complex_cast %prec OR expression
					;

evalexpression		: fceexp 
						{ $$ = make_node(P_OP_FCEXP, NULL, 1, $1);}	
					| fceexp_rc 
						{ $$ = make_node(P_OP_FCRC, NULL, 1, $1);}
					| var_exp 
						{ $$ = make_node(P_OP_VAREXP, NULL, 1, $1);}
					| functioncall 
						{ $$ = make_node(P_OP_CALL, NULL, 1, $1);}
					| PAROPEN expression PARCLOSE
						{ $$ = make_node(P_OP_EXP, NULL, 1, $2); }
					;

functioncall		: evalexpression arglist 
						{ $$ = make_node(P_OP_EVEXP, NULL, 2, $1, $2); }
					;

var_exp				: ID 
						{ $$ = make_node(ID, $1, 0); }
					| evalexpression BOXOPEN expression BOXCLOSE 
						{ $$ = make_node(P_OP_SEL, NULL, 2, $1, $3);}
					;

functiondef			: FUNCTION ID parlist block 
						{$$ = make_node(P_OP_PARLST, NULL, 2, $3, $4);}
					| FUNCTION ID parlist hotkeydef block 
						{$$ = make_node(P_OP_PARLSTHK, NULL, 3, $3, $4, $5);}
					| FUNCTION ID parlist hotstringdef block 
						{$$ = make_node(P_OP_PARLSTHK, NULL, 3, $3, $4, $5);}
					;

hotkeydef			: ON KEYSTROKE expression 
						{ $$ = make_node(P_OP_HKDEF, NULL, 1, $3); }
					;

hotstringdef		: ON HOTSTRING expression 
						{ $$ = make_node(P_OP_HSDEF, NULL, 1, $3); } 
					;

ffi_struct_def		: CURLOPEN explist CURLCLOSE 
						{ $$ = make_node(0, NULL, 0); }
					| MUL CURLOPEN explist CURLCLOSE 
						{ $$ = make_node(0, NULL, 0); }
					;

ffi_cast 			: FFI_CHAR  
						{ $$ = make_node(P_OP_FFICHAR, $1, 0);}
					| FFI_SHORT 
						{ $$ = make_node(P_OP_FFISHORT, $1, 0);} 
					| FFI_INT   
						{ $$ = make_node(P_OP_FFIINT, $1, 0);}
					| FFI_LONG  
						{ $$ = make_node(P_OP_FFILONG, $1, 0);}
					| FFI_LONG_LONG 
						{ $$ = make_node(P_OP_FFILONGLONG, $1, 0);}
					| FFI_DOUBLE 	
						{ $$ = make_node(P_OP_FFIDOUBLE, $1, 0);}
					| FFI_FLOAT 	
						{ $$ = make_node(P_OP_FFIFLOAT, $1, 0);}
					| FFI_LONG_DOUBLE 
						{ $$ = make_node(P_OP_FFILONGDOUBLE, $1, 0);}
					| FFI_VOIDPTR 	  
						{ $$ = make_node(P_OP_FFIVOIDPTR, $1, 0);}
					;

complex_cast		: PAROPEN castlist PARCLOSE 
						{ $$ = $2; }
					| MUL PARCLOSE castlist PAROPEN 
						{ $$ = $3; }
					;

listconstructor 	: BOXOPEN BOXCLOSE 
						{ $$ = make_node(P_OP_LSTCONST, NULL, 0); }
					| BOXOPEN explist BOXCLOSE 
						{ $$ = make_node(P_OP_LSTCONST, NULL, 1, $2); }
					;

statement			: assignment SEMI NL
						{ $$ = make_node(P_OP_ASSIGN, NULL, 1, $1); }
					| BREAK SEMI NL
						{ $$ = make_node(P_OP_BREAK, NULL, 0); }
					| CONTINUE SEMI NL
						{ $$ = make_node(P_OP_CONTINUE, NULL, 0); }
					| INC var_exp SEMI NL
						{ $$ = make_node(P_OP_INC, NULL, 1, $2); }
					| DEC var_exp SEMI NL
						{ $$ = make_node(P_OP_DEC, NULL, 1, $2); }
					| PRINT expression SEMI NL
						{ $$ = make_node(P_OP_PRINT, NULL, 1, $2); }
					| block NL
						{ $$ = make_node(P_OP_BLOCK, NULL, 1, $1); }
					| functioncall SEMI NL
						{ $$ = make_node(P_OP_CALL, NULL, 1, $1); }
					| functiondef NL
						{ $$ = make_node(P_OP_FDEF, NULL, 1, $1); }
					| GLOBAL assignment SEMI NL
						{ }
					| GLOBAL functiondef NL
						{ }
					| IF PAROPEN expression PARCLOSE block NL
						{ $$ = make_node(P_OP_IF, NULL, 2, $3, $5); }
					| IF PAROPEN expression PARCLOSE block ELSE block NL
						{ $$ = make_node(P_OP_ELSE, NULL, 3, $3, $5, $7); }
					| IF PAROPEN expression PARCLOSE block elif_block_list ELSE block NL
						{ $$ = make_node(P_OP_ELIF, NULL, 4, $3,$5,$6,$8); }
					| IF PAROPEN expression PARCLOSE block elif_block_list NL
						{ $$ = make_node(P_OP_ELIF, NULL, 3, $3,$5,$6); }
					| FOR PAROPEN assignment SEMI expression SEMI 
						assignment PARCLOSE block NL
						{ $$ = make_node(P_OP_FOR, NULL, 4, $3, $5, $7, $9); }
					| FOR idlist IN explist block NL
						{ $$ = make_node(P_OP_FORIN, NULL, 3, $2, $4, $5);} 
					| WHILE PAROPEN expression PARCLOSE block NL
						{ $$ = make_node(P_OP_WHILE, NULL, 2, $3, $5); }
					| DO block WHILE PAROPEN expression PARCLOSE SEMI NL
						{ $$ = make_node(P_OP_DOWH, NULL, 2, $2, $5);}
					| SWITCH PAROPEN expression PARCLOSE switchblock NL
						{ $$ = make_node(P_OP_SWITCH, NULL, 2, $3, $5); }
					| fceblock NL
						{ $$ = make_node(P_OP_FCEB, NULL, 1, $1); }
					| NL 
						{ $$ = make_node(P_OP_NOOP, NULL, 0); }
					| error SEMI
						{ printf("Invalid Statement. Skipping until semicolon\n"); 
							yyerrok; }
					| error NL
						{ printf("Invalid Statement. Skipping until newline\n");
							yyerrok; }
					;

fceblock 			: EXTERNAL FCELANG FCEB_CODE 
						{ $$ = make_node(P_OP_FCEB, $2, 0); }
					;

fceexp				: PAROPEN NOT FCELANG PARCLOSE FCEB_CODE 
						{ $$ = make_node(P_OP_FCEXP, $5, 0); }
					;

fceexp_rc			: PAROPEN QMARK FCELANG PARCLOSE FCEB_CODE 
						{ $$ = make_node(P_OP_FCRC, $5, 0); }
					;

%%

