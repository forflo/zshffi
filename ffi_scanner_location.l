%top{

#include "ffi_read_write.h"
#include <stdlib.h>

}

%option reentrant
%option noyywrap
%option header-file="ffi_scanner_location.h"
%option outfile="ffi_scanner_location.c"
%option prefix="ffiloc"

/* Symbols */

start "@"
boxopen "["
boxclose "]"
arrow "->"
equals "="
dollar "$"

/* helpers */
digit [0-9]
unum ({digit})+
num  (-)?({digit})+
delim [ \n\t\b\a\r]+
hex 0[xX]([[:digit:]]|[a-fA-F])+
float {num}"."{digit}+

%%

{delim} { }

{unum}           { return(UNUMBER); }
{num}            { return(NUMBER); }
{boxopen}        { return(BOXOPEN); }
{boxclose}       { return(BOXCLOSE); }
{arrow}          { return(ARROW); }
{start}          { return(START); }
{hex}            { return(HEX); }
{float}          { return(FLOAT); }
{equals}         { return(EQUALS); }
{dollar}         { return(DOLLAR); }
<<EOF>>          { return(ENDOFFILE); }

%%
