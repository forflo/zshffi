%top{

#include "ffi_node_defines.h"
#include "ffi_parser.h"
#include "ffi_scanner_util.h"

}

%option reentrant
%option noyywrap
%option outfile="ffi_scanner.c"
%option prefix="ffi"
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
array "@"
equals "="
paropen "("
parclose ")"
curlopen "{"
curlclose "}"
arrow "->"
comma ","
pipe_del "|"

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
cintmax_t    "cintmax_t"
cuintmax_t   "cuintmax_t"
csize_t      "csize_t"
cssize_t     "cssize_t"
cptrdiff_t   "cptrdiff_t"
coff_t       "coff_t"
cwchar_t     "cwchar_t"
cfloat       "cfloat"
cdouble      "cdouble"

union        "union"
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

{cchar}         { yylval_param->v = (void *) CCHAR      ; return(s_type); }
{cuchar}        { yylval_param->v = (void *) CUCHAR     ; return(s_type); }
{cshort}        { yylval_param->v = (void *) CSHORT     ; return(s_type); }
{cushort}       { yylval_param->v = (void *) CUSHORT    ; return(s_type); }
{cint}          { yylval_param->v = (void *) CINT       ; return(s_type); }
{cuint}         { yylval_param->v = (void *) CUINT      ; return(s_type); }
{clong}         { yylval_param->v = (void *) CLONG      ; return(s_type); }
{culong}        { yylval_param->v = (void *) CULONG     ; return(s_type); }
{clonglong}     { yylval_param->v = (void *) CLONGLONG  ; return(s_type); }
{culonglong}    { yylval_param->v = (void *) CULONGLONG ; return(s_type); }
{cintmax_t}     { yylval_param->v = (void *) CINTMAX_T  ; return(s_type); }
{cuintmax_t}    { yylval_param->v = (void *) CUINTMAX_T ; return(s_type); }
{csize_t}       { yylval_param->v = (void *) CSIZE_T    ; return(s_type); }
{cssize_t}      { yylval_param->v = (void *) CSSIZE_T   ; return(s_type); }
{cptrdiff_t}    { yylval_param->v = (void *) CPTRDIFF_T ; return(s_type); }
{coff_t}        { yylval_param->v = (void *) COFF_T     ; return(s_type); }
{cwchar_t}      { yylval_param->v = (void *) CWCHAR_T   ; return(s_type); }
{cfloat}        { yylval_param->v = (void *) CFLOAT     ; return(s_type); }
{cdouble}       { yylval_param->v = (void *) CDOUBLE    ; return(s_type); }

{pointer}       { return(pointer);   }
{equals}        { return(equals);    }
{paropen}       { return(paropen);   }
{parclose}      { return(parclose);  }
{curlopen}      { return(curlopen);  }
{curlclose}     { return(curlclose); }
{arrow}         { return(arrow);     }
{comma}         { return(comma);     }
{pipe_del}      { return(pipe_del);  }
{array}         { return(array);     }

{union}         { yylval_param->v = (void *) UNION      ; return(c_type); }
{struct}        { yylval_param->v = (void *) STRUCT     ; return(c_type); }

{string}        { yylval_param->v = make_valueStr(yytext); return(string); }

%%
