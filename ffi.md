# FFI module spec
## Usage
    usage: ffi_call <store_var> <funcname> <signature>
           ffi_show <store_var>
           => Format: 
           $ ffi_show foo
             (type: uint64*) 5000323
           ffi_read <store_var> 

    
    consider the following ffi code:
    
    struct * = {
        cchar = [5],
        cdouble = [255.255],
        struct * = {
            struct * = {
                cdouble = [7431.123],
                cchar = [?],
                cint = [-123]
            },
            struct * = {
                cdouble = [431.123],
                cchar = [/],
                cint = [-999]
            }
        }
    }

    ffi_read var_1 @->[2]->[0]->[1] 
    => would return the string "?"

    ffi_read var_1 @->[1]
    => would return the string "255.255"

    ffi_read var_1 @->[2]->[1]->[2]
    => would return the string "-999"

## Grammatik location specifier

    <locspec> ::= @ <descent_list>
    <descent_list> ::= <descent> |
                       <descent_list> <descent>
    <descent> ::= -> [ <number> ]
    <number> ::= <digit> |
                 <number> <digit>
    <digit> ::= 0 | 1 | ... | 9

## Grammatik struktur specifier
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
                 cfloat      cdouble     voidp

## Konversoin semantics
Lets consider the following sample code:

    1    cchar = [ ], 
    2    cuchar = [255],
    3    cuchar = [256],
    4    
    5    cshort = [-3421], 
    6    cushort = [32443], 
    7    cushort = [1232443], 
    8    
    9    cint = [-1232323],
    10   cuint = [1232123],
    11   
    12   clong = [-123], 
    13   culong = [23],
    14   
    15   clonglong = [123123],
    16   culonglong = [1123123],
    17   culonglong = [1123123],
    18   
    19   cfloat = [0.23],
    20   cdouble = [0.000001]
    21   voidp = [0x00123012300]  -> cchar = [?]

The semantics of "from-string" conversion are as follows:
(1) First character in string gets used as number.
(2,3) String gets converted to unsigned long. Then to unsigned char.

(5) String gets converted to signed integer using atoi, 
    then to signed short using a explicit type cast.
(6,7) String get converted to unsigned long, then to unsigned short.

(9) String gets converted to signed integer.
(10) String gets converted to unsigned long, then to unsigned int.

(12) String gets converted to signed long.
(13) String gets converted to unsinged long.

(15) String gets converted to signed long long
(16) String gets converted to unsigned long long
(17) String gets converted to unsigned long long

(19) String gets converted to float
(20) String gets converted to double

(21) String gets converted to voidpointer. Has to be in hex.

Conversion functions used:
String -> double: atod
String -> float: atof
String -> signed long: strtol
String -> unsigned long: strtoul
String -> signed char: none
String -> signed int: atoi
String -> signed long long: strtoll
String -> unsigned long long: strtoull

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
