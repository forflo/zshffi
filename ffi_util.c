#include "ffi_util.h"
#include "ffi_generate_ops.h"
#include "ffi_node_defines.h"

char *get_stype_string(int val){
    static char *strings[] = {
        "cchar",     "cuchar",    "cshort",    "cushort",   
        "cint",      "cuint",     "clong",     "culong",    
        "clonglong", "culonglong", "cintmax_t", "cuintmax_t",
        "csize_t",   "cssize_t",  "cptrdiff_t", "coff_t",    
        "cwchar_t",  "cfloat",    "cdouble", "union", "struct"
    };

    switch(val){
        case CCHAR      : return(strings[0 ]); break;
        case CUCHAR     : return(strings[1 ]); break;
        case CSHORT     : return(strings[2 ]); break;
        case CUSHORT    : return(strings[3 ]); break;
        case CINT       : return(strings[4 ]); break;
        case CUINT      : return(strings[5 ]); break;
        case CLONG      : return(strings[6 ]); break;
        case CULONG     : return(strings[7 ]); break;
        case CLONGLONG  : return(strings[8 ]); break;
        case CULONGLONG : return(strings[9 ]); break;
        case CINTMAX_T  : return(strings[10]); break;
        case CUINTMAX_T : return(strings[11]); break;
        case CSIZE_T    : return(strings[12]); break;
        case CSSIZE_T   : return(strings[13]); break;
        case CPTRDIFF_T : return(strings[14]); break;
        case COFF_T     : return(strings[15]); break;    
        case CWCHAR_T   : return(strings[16]); break;
        case CFLOAT     : return(strings[17]); break;
        case CDOUBLE    : return(strings[18]); break;
        case UNION      : return(strings[19]); break;
        case STRUCT     : return(strings[20]); break;
    }                                   

    return "(NOT-FOUND)";             
}

char *get_operation_string(int val){
    static char *strings[] = {
        "arr_to", "ptr_to",
        "start_struct", "start_union",
        "member", "end_struct",
        "end_union", "pop",
        "arr_end"
    };

    switch(val){
        case arr_to			: return(strings[0]); break;
        case ptr_to			: return(strings[1]); break;
        case start_struct	: return(strings[2]); break;
        case start_union	: return(strings[3]); break;
        case member			: return(strings[4]); break;
        case end_struct		: return(strings[5]); break;
        case end_union		: return(strings[6]); break;
        case pop			: return(strings[7]); break;
        case arr_end		: return(strings[8]); break;
    }

    return "(NOT_FOUND)";
}

char *get_non_terminal(int val){
    static char *strings[] = {
       "SIG", "TVAL", "SARR", "SPTR", "S",
       "CARR", "CPTR", "C", "VAL", "TVAL_LST"
    };

    switch(val){
        case NT_SIGNATURE   : return strings[0]; break;
        case NT_TYPEANDVAL  : return strings[1]; break;
        case NT_SCALAR_ARR  : return strings[2]; break;
        case NT_SCALAR      : return strings[4]; break;
        case NT_SCALAR_PTR  : return strings[3]; break; 
        case NT_COMPOUND    : return strings[7]; break;   
        case NT_COMPOUND_ARR: return strings[5]; break;
        case NT_COMPOUND_PTR: return strings[6]; break;
        case NT_TVAL_LIST   : return strings[9]; break;   
        case NT_VALUE       : return strings[8]; break;   
    } 
    
    return "(NIL)";
}
