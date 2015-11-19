%top{

#include "ffi_node_defines.h"
#include "ffi_parser.h"
#include "ffi_scanner_util.h"

}

%option reentrant
%option noyywrap
%option outfile="ffi_scanner.c"
%option prefix="ffi"
%option header-file="ffi_scanner.h"
/* 
 * The following ensures, that we generate 
 * the function ffilex(YYSTYPE *lvalp, ffiscan_t scanner); 
 * which is needet for proper bison interaction. See
 * http://flex.sourceforge.net/manual/Bison-Bridge.html,
 * http://www.lemoda.net/c/reentrant-parser/index.html and
 * http://www.phpcompiler.org/articles/reentrantparser.html
 * for the gory details.
 */
%option bison-bridge

/* Symbols */

pointer "*"
equals "="
curlopen "{"
curlclose "}"
comma ","

/* FFI Keywords */
cchar        "cchar"
cuchar       "cuchar"
cshort       "cshort"
cushort      "cushort"
cint         "cint"
cuint        "cuint"
clong        "clong"
culong       "culong"
clonglong    "clonglong"
culonglong   "culonglong"
cfloat       "cfloat"
cdouble      "cdouble"

struct       "struct"

/* helpers */
letter  [A-Za-z]
digit [0-9]
allowed_symbols   "!"|"#"|"$"|"%"|"&"|"'"|"*"|"+"|"`"|"´"|"-"|"."|"/"|":"|";"|"<"|">"|"="|"?"|"@"|"["|"]"|"^"|"_"|"{"|"}"|"~"|"|"|" "
escapes  \\n|\\t|\\a|\\b|\\r|\\v|\\\\|\\\"|\\\'|\\f|\\0|\\\(|\\\)
delim [\t\v\b\ \n]+

/* literals */
string  \[(({letter}|{escapes}|{allowed_symbols}|{digit})+)\]

%%

{delim} { }

{cchar}         { yylval_param->v = (void *) STYPE_CCHAR      ; return(s_type); }
{cuchar}        { yylval_param->v = (void *) STYPE_CUCHAR     ; return(s_type); }
{cshort}        { yylval_param->v = (void *) STYPE_CSHORT     ; return(s_type); }
{cushort}       { yylval_param->v = (void *) STYPE_CUSHORT    ; return(s_type); }
{cint}          { yylval_param->v = (void *) STYPE_CINT       ; return(s_type); }
{cuint}         { yylval_param->v = (void *) STYPE_CUINT      ; return(s_type); }
{clong}         { yylval_param->v = (void *) STYPE_CLONG      ; return(s_type); }
{culong}        { yylval_param->v = (void *) STYPE_CULONG     ; return(s_type); }
{clonglong}     { yylval_param->v = (void *) STYPE_CLONGLONG  ; return(s_type); }
{culonglong}    { yylval_param->v = (void *) STYPE_CULONGLONG ; return(s_type); }
{cfloat}        { yylval_param->v = (void *) STYPE_CFLOAT     ; return(s_type); }
{cdouble}       { yylval_param->v = (void *) STYPE_CDOUBLE    ; return(s_type); }
{struct}        { yylval_param->v = (void *) CTYPE_STRUCT     ; return(c_type); }

{pointer}       { return(pointer);   }
{equals}        { return(equals);    }
{curlopen}      { return(curlopen);  }
{curlclose}     { return(curlclose); }
{comma}         { return(comma);     }

{string}        { yylval_param->v = make_valueStr(yytext); return(string); }

%%
