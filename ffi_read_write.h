#ifndef READ_WRITE
#define READ_WRITE

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
        X(START)


#define GENERATE_ENUM(X) X,
enum token { TOKEN_LIST(GENERATE_ENUM) };
#undef GENERATE_ENUM

struct query_op {
    enum token operation;
    int level; 
};

static int next_level(void *ffiloc_scan, int *lvl);
int ffi_read(struct dstru_struct *storage, const char *query, char **result);

#endif
