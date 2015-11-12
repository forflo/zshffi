%top{
#include "ffi_read_write.h"
}

%option reentrant
%option noyywrap
%option header-file="ffi_scanner_location.h"
%option outfile="ffi_scanner_location.c"
%option prefix="ffiloc"
%option bison-bridge

/* Symbols */

start "@"
boxopen "["
boxclose "]"
arrow "->"
equals "="

/* helpers */
digit [0-9]
unum ({digit})+
num  (-)?({digit})+
delim " \n\t\b\a\r"
hex 0[xX][[:digit:]]+
float {num}"."{digit}+

%%

{delim} { }

{num}            { return(NUMBER); }
{boxopen}        { return(BOXOPEN); }
{boxclose}       { return(BOXCLOSE); }
{arrow}          { return(ARROW); }
{start}          { return(START); }
{hex}            { return(HEX); }
{float}          { return(FLOAT); }
{unum}           { return(UNUMBER); }
{equals}         { return(EQUALS); }

%%
