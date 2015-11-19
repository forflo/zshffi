#include "ffi_parser.h"
#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_generate_dot.h"
#include "ffi_offset_table.h"
#include "ffi_scanner.h"
#include "ffi_generate_ops.h"
#include "ffi_storage.h"
#include "ffi_util.h"
#include "ffi_read_write.h"

#include <stdlib.h>
#include <stdio.h>

#define DEBUG

extern int ffidebug;

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

struct test_real_ptr {
    char *a;
    double b;
    struct t3baz *c;
};

struct t3baz {
    struct t3foo *a;
    struct t3foo *b;
};

struct t3foo {
    double *a;
    char b;
    int *c; 
};

struct test_read_4 {
    double a;
    int b;
    struct anon_foo {
        double c;
    } *p;
};

struct test_real_2 {
    char a;
    double b;
    struct {
        double a;
        int b;
    } c;
};

void modify_t3(struct test_real_ptr *stru){
    *stru->a = '6';
    stru->b = 123.123123123;
    stru->c->b->b = '{';
}

int main(int argc, char **argv){
    struct nary_node *root;
    struct ffi_instruction_obj *instructions;
    struct offset_table *tbl;
    void *ffi_scan;
    char function[200], stru[200], argbuf[20], retbuf[20];

    struct ffi_instruction_obj *objarr[20];
    struct nary_node *treearr[20];
    struct offset_table *tblarr[20];
    void *ptrarr[20];

    int args, i;

    ffidebug = 0;


    printf("Geben Sie den Funktionspfad an!\n");
    fgets(function, 199, stdin);

    printf("Wie viele Argumente?\n");
    fgets(argbuf, 19, stdin);
    args = atoi(argbuf);

    for (i=0; i< args; i++){
        printf("Geben Sie das %d-te Argument and!\n", i);
    
        fgets(stru, 199, stdin);
        ffilex_init(&ffi_scan);
        ffi_scan_string(function, ffi_scan);
        ffiparse(&treearr[i], ffi_scan);

        genops(&objarr[i], treearr[i]->nodes[0]);
        printf("\n");
        emit_human(objarr[i]);

        gentbl(objarr[i], &tblarr[i]);
        printf("\n");
        emit_human_otbl(tblarr[i], 0);
    
        get_storage(&ptrarr[i], instructions);

    }

//    struct t1 t = *((struct t1 *) res);
//    printf("foo: %hhc | bar: %hhu | baz: %hi | moo: %hu\n", t.a, t.b, t.c, t.d);
//    printf("foo: %i | bar: %u | baz: %li | moo: %lu\n", t.e, t.f, t.g, t.h);
//    printf("foo: %lli | bar: %llu | baz: %f | moo: %lf\n", t.i, t.j, t.k, t.l);
//
//    struct t2 t = *((struct t2 *) res);
//    printf("a: %c b: %lf c: %lf d: %i\n", t.a, t.b, t.anon2.c, t.anon2.d);
//    printf("Storage ok!");
//
//    struct test_real_ptr *t = ((struct test_real_ptr *) res);
//    printf("\nVorher t: %p\n", t);
//    printf("a: %c b: %lf c: %lf d: %c\n", *(t->a), t->b, *t->c->a->a, t->c->a->b);
//    printf("e: %d f: %lf g: %c h: %i\n", *t->c->a->c, *t->c->b->a, t->c->b->b, *t->c->b->c);


    //modify_t3(t);

//    printf("\nNachher t: %p\n", t);
//    printf("a: %c b: %lf c: %lf d: %c\n", t->a, t->b, t->c->a->a, t->c->a->b);
//    printf("e: %d f: %lf g: %c h: %i\n", t->c->a->c, t->c->b->a, t->c->b->b, t->c->b->c);
//
//    ffilex_destroy(ffi_scan);

    return EXIT_SUCCESS;
}
