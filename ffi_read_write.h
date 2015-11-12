#ifndef READ_WRITE
#define READ_WRITE

#define TOKEN_LIST(X) \
        X(NUMBER)   \
        X(ARROW)     \
        X(BOXOPEN)   \
        X(BOXCLOSE)   \
        X(UNUMBER)   \
        X(HEY)   \
        X(FLOAT)   \
        X(EQUALS)   \
        X(START)
    

#define GENERATE_ENUM(X) X,
enum token { TOKEN_LIST(GENERATE_ENUM) };
#undef GENERATE_ENUM

struct query_op {
    enum level_op operation;
    int level; 
};

extern int ffilocdebug;

extern void ffilocset_in(FILE *f, void *scan);
extern int ffiloclex_init(void *ffiloc);
extern int ffiloclex_destroy(void *ffiloc);

#endif
