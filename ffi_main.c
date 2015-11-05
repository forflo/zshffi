#include "ffi_parser.h"
#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_generate_dot.h"
#include "ffi_generate_ops.h"
#include <stdlib.h>
#include <stdio.h>

extern int ffidebug;

extern void ffiset_in(FILE *f, void *scan);
extern int ffilex_init(void *ffi);
extern int ffilex_destroy(void *ffi);
extern int ffiparse(struct nary_node **r, void *scan);

int main(void){
    struct nary_node *root;

    //ffidebug = 1;

    void *ffi_scan;
    ffilex_init(&ffi_scan);
    ffiset_in(stdin, ffi_scan);
    ffiparse(&root, ffi_scan);

    interpret(root->nodes[0]->nodes[0]->nodes[0]);

    ffilex_destroy(ffi_scan);

    return EXIT_SUCCESS;
}
