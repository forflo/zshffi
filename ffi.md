# FFI module spec
## Usage
    usage: ffi_call <store_var> <funcname> <signature>
           ffi_show <store_var>
           => Format: 
           $ ffi_show foo
             (type: uint64*) 5000323
           ffi_read <store_var> 

ffi_read var_1 "@->[0]->uint16"
ffi_read var_1 "@->[1].[0]->uint64"

## Grammatik
    <signature> ::= <arguments> "->" <return>
    <arguments> ::= <tval_list> |
                    <arguments> "|" <tval_list>
    <return> ::= <type_and_val>

    <tval_list> ::= <type_and_val> |
                    <tval_list> , <type_and_val>

    <type_and_val> ::= <scalar> | <compound>

    <scalar> ::= <s_type> = <value> |
                 <s_type> * = <value> |
                 <s_type> @ = { <tval_list> }

    <compound> ::= <c_type> = { <tval_list> }
                   <c_type> * = { <tval_list> }
                   <c_type> @ = { <tval_list> }

    <value> ::= ( <string> )
    <c_type> ::= union | struct
    <s_type> ::= cchar       cuchar      cshort      cushort     cint        
                 cuint       clong       culong      clonglong   culonglong  
                 cfloat      cdouble     

## Example Code
ffi_call printf_ret printf "
    struct * = {
        cushort * = (42),
        culong @ = {
            culong * = (32), 
            culong = (64),
        },
        cdouble * = (3.141592), 
        union = {
            cfloat = (0.32),
            cdouble = (0.342),
        },
        struct * = {
            cchar = (a),
            cchar @ = {
                cchar = (a), 
                cchar = (b)
            },
            char * = (foobar),
            callback* = (zsh_functionname),
        },
    }, float = (0.53), int = (32) -> cint = (?)"


## Specification of the type names
Cchar

     Equivalent to the native char c-type

Cuchar

     Equivalent to the native unsigned char c-type (UInt8)

Cshort

    Equivalent to the native signed short c-type (Int16)

Cushort

    Equivalent to the native unsigned short c-type (UInt16)

Cint

    Equivalent to the native signed int c-type (Int32)

Cuint

    Equivalent to the native unsigned int c-type (UInt32)

Clong

    Equivalent to the native signed long c-type

Culong

        Equivalent to the native unsigned long c-type

Clonglong

            Equivalent to the native signed long long c-type (Int64)

Culonglong

    Equivalent to the native unsigned long long c-type (UInt64)

Cfloat

    Equivalent to the native float c-type (Float32)

Cdouble

    Equivalent to the native double c-type (Float64)
