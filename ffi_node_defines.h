#ifndef NODE_DEFINES
#define NODE_DEFINES

/* Generator Makro for automatic
   enum and string-table generation.
   This is also commonly known as X-Macro */
#define LIST_TYPE(F) \
    F(STYPE_NONE)       \
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
    F(STYPE_CFLOAT)     \
    F(STYPE_CDOUBLE)    \
    F(STYPE_SCALARPTR)  \
    F(CTYPE_STRUCTPTR)  \
    F(CTYPE_UNION)      \
    F(CTYPE_STRUCT)

#define LIST_NTYPE(F)       \
    F(NT_START)         \
    F(NT_TYPEANDVAL)    \
    F(NT_SCALAR)        \
    F(NT_SCALAR_PTR)    \
    F(NT_COMPOUND)      \
    F(NT_COMPOUND_PTR)  \
    F(NT_TVAL_LIST)     \
    F(NT_VALUE)

#define GENERATE_STRING(X) #X,
#define GENERATE_ENUM(X) X,

/* Codes for s_types and c_types */
enum type { LIST_TYPE(GENERATE_ENUM) };

/* Code for non Terminals */
enum node_types { LIST_NTYPE(GENERATE_ENUM) };

#endif /* NODE_DEFINES */
