#ifndef READ_WRITE
#define READ_WRITE

#include "ffi_dstru.h"
#include "ffi_offset_table.h"

#define TOKEN_LIST(X) \
        X(ENDOFFILE) \
        X(NUMBER)   \
        X(ARROW)     \
        X(BOXOPEN)   \
        X(BOXCLOSE)   \
        X(UNUMBER)   \
        X(HEX)   \
        X(FLOAT)   \
        X(EQUALS)   \
        X(DOLLAR) \
        X(START)


#define GENERATE_ENUM(X) X,
enum token { TOKEN_LIST(GENERATE_ENUM) };
#undef GENERATE_ENUM

struct query_op {
    enum token operation;
    int level; 
};

static int next_level(void *ffiloc_scan, int *lvl);
int ffi_read(struct offset_table *tbl, 
        void *dystru_buffer, const char *query, char **result);

#endif
