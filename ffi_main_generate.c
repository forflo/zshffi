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

int main(void){
    struct nary_node *root;
    struct ffi_instruction_obj *instructions;
    void *res;

    ffidebug = 1;

    void *ffi_scan;
    ffilex_init(&ffi_scan);
    ffiset_in(stdin, ffi_scan);
    ffiparse(&root, ffi_scan);

#ifdef DEBUG
    printf("root: %p, root->nodes[0]: %p\n", root, root->nodes[0]);
#endif


    struct nary_node *copy;
    copy = clone_node(root);


    genops(&instructions, copy->nodes[0]);
    emit_human(instructions);
    



    ffilex_destroy(ffi_scan);

    return EXIT_SUCCESS;
}
