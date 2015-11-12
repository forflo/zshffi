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
<<EOF>>          { return(ENDOFFILE); }

%%

/* for debugging purposes */
#include "ffi_scanner_location.h"

#define GENERATE_STRING(X) #X,
static const char *table[] = { TOKEN_LIST(GENERATE_STRING) };
#undef GENERATE_STRING
    

int main(void){
    void *ffiloc_scan;
    ffiloclex_init(&ffiloc_scan);
//    ffilocset_in(stdin, ffiloc_scan);

    ffiloc_scan_string("@->[123]->[32] -> [4711123] =[0xCAFFEEBABE]", ffiloc_scan);
    

    int i;
    char *dup;

    int a, b, c, e;
    int level;
    if(ffiloclex(ffiloc_scan) != START){
        printf("No start symbold found!\n");
        return EXIT_FAILURE;
    }

    for (;;){
        a = ffiloclex(ffiloc_scan);
        b = ffiloclex(ffiloc_scan);
        c = ffiloclex(ffiloc_scan);
        if (c == UNUMBER)
            level = atoi(ffilocget_text(ffiloc_scan));
        e = ffiloclex(ffiloc_scan);
        
        if(a == ARROW && b == BOXOPEN && e == BOXCLOSE){
            switch(c){
                case UNUMBER:
                    printf("Level: %d\n", level);
                    break;
                default :
                    printf("Not a number!\n");
                    return EXIT_FAILURE;
                    break;
            }
        } else {
            printf("Syntax error!\n");
            break;
        }
    }

    return 0;
}
