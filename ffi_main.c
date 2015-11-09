#include "ffi_parser.h"
#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_generate_dot.h"
#include "ffi_generate_ops.h"
#include "ffi_storage.h"
#include "ffi_util.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG

extern int ffidebug;

extern void ffiset_in(FILE *f, void *scan);
extern int ffilex_init(void *ffi);
extern int ffilex_destroy(void *ffi);
extern int ffiparse(struct nary_node **r, void *scan);

struct t1 {
    char a;
    unsigned char b;
    short c;
    unsigned short d;
    int e;
    unsigned int f;
    long g;
    unsigned long h;
    long long i;
    unsigned long long j;
    float k;
    double l;
};

struct t2 {
    char a;
    double b;
    struct {
        double c;
        int d; 
    } anon2;
};

struct t3 {
    char a;
    double b;
    struct t3baz *c;
};

struct t3baz {
    struct t3foo *a;
    struct t3foo *b;
};

struct t3foo {
    double a;
    char b;
    int c; 
};

int main(void){
    struct nary_node *root;
    struct ffi_instruction **instructions;
    void *res;

    ffidebug = 1;

    void *ffi_scan;
    ffilex_init(&ffi_scan);
    ffiset_in(stdin, ffi_scan);
    ffiparse(&root, ffi_scan);

#ifdef DEBUG
    printf("root: %p, root->nodes[0]: %p\n", root, root->nodes[0]);
#endif
    genops(&instructions, root->nodes[0]);
    emit_human(instructions);
    
    get_storage(&res, instructions);

//    struct t1 t = *((struct t1 *) res);
//    printf("foo: %hhc | bar: %hhu | baz: %hi | moo: %hu\n", t.a, t.b, t.c, t.d);
//    printf("foo: %i | bar: %u | baz: %li | moo: %lu\n", t.e, t.f, t.g, t.h);
//    printf("foo: %lli | bar: %llu | baz: %f | moo: %lf\n", t.i, t.j, t.k, t.l);

//    struct t2 t = *((struct t2 *) res);
//    printf("a: %c b: %lf c: %lf d: %i\n", t.a, t.b, t.anon2.c, t.anon2.d);

    struct t3 *t = ((struct t3 *) res);
    printf("t: %p\n", t);
    printf("a: %c b: %lf c: %lf d: %c\n", t->a, t->b, t->c->a->a, t->c->a->b);
    printf("e: %d f: %lf g: %c h: %i\n", t->c->a->c, t->c->b->a, t->c->b->b, t->c->b->c);

    ffilex_destroy(ffi_scan);

    return EXIT_SUCCESS;
}
