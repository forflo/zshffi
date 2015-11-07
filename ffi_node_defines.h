#ifndef NODE_DEFINES
#define NODE_DEFINES

/* Generator Makro for automatic
   enum and string-table generation */
#define LIST_TYPE(F) \
        F(STYPE_CCHAR)      \
        F(STYPE_CUCHAR)     \
        F(STYPE_CSHORT)     \
        F(STYPE_CUSHORT)    \
        F(STYPE_CINT)       \
        F(STYPE_CUINT)      \
        F(STYPE_CLONG)      \
        F(STYPE_CULONG)     \
        F(STYPE_CLONGLONG)  \
        F(STYPE_CULONGLONG) \
        F(STYPE_CINTMAX_T)  \
        F(STYPE_CUINTMAX_T) \
        F(STYPE_CSIZE_T)    \
        F(STYPE_CSSIZE_T)   \
        F(STYPE_CPTRDIFF_T) \
        F(STYPE_COFF_T)     \
        F(STYPE_CWCHAR_T)   \
        F(STYPE_CFLOAT)     \
        F(STYPE_CDOUBLE)    \
        F(CTYPE_UNION)      \
        F(CTYPE_STRUCT)

#define LIST_NTYPE(F)       \
        F(NT_SIGNATURE)     \
        F(NT_TYPEANDVAL)    \
        F(NT_SCALAR_ARR)    \
        F(NT_SCALAR)        \
        F(NT_SCALAR_PTR)    \
        F(NT_COMPOUND)      \
        F(NT_COMPOUND_ARR)  \
        F(NT_COMPOUND_PTR)  \
        F(NT_TVAL_LIST)     \
        F(NT_VALUE)

#define GENERATE_STRING(X) #X,
#define GENERATE_ENUM(X) X,

/* Codes for s_types and c_types */
enum type {
    LIST_TYPE(GENERATE_ENUM)
};

/* Code for non Terminals */
enum node_types {
    LIST_NTYPE(GENERATE_ENUM)
};

#endif /* NODE_DEFINES */
