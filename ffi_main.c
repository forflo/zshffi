#include "ffi_parser.h"
#include "ffi_nary_tree.h"
#include "ffi_node_defines.h"
#include "ffi_generate_dot.h"
#include <stdlib.h>
#include <stdio.h>

extern int ffidebug;

extern void ffiset_in(FILE *f, void *scan);
extern int ffilex_init(void *ffi);
extern int ffilex_destroy(void *ffi);
extern int ffiparse(struct nary_node **r, void *scan);

static int callback(void *node, void *u){
    if (node != NULL){
        printf("Knotentyp: [%d]\n", ((struct nary_node *) node)->node_type);
    } else {
        return -1;
    }

    return 0;
}

int main(){
    struct nary_node *root;

    ffidebug = 0;

    void *ffi_scan;
    ffilex_init(&ffi_scan);
    ffiset_in(stdin, ffi_scan);
    ffiparse(&root, ffi_scan);

//    traverse_preorder(root, callback, NULL);
    output(root);

    ffilex_destroy(ffi_scan);

    return EXIT_SUCCESS;
}
