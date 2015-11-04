#include <stdio.h>
#include "ffi_interpreter.h"
#include "ffi_nary_tree.h"

int interpret_foo(struct ffi_instruction ins[], int *position, struct nary_node *foo);

void interpret(){
    struct nary_node *par = new_node("foobar");
    struct nary_node *child_par = new_node("mooo");
    struct nary_node *child = new_node("muusch");
    par->node_type = 1;
    child_par->node_type = 2;
    child->node_type = 3;
    add_child(child_par, child);
    add_child(par, child_par);

    struct ffi_instruction ins[1000] = { 0 };
    int position = 0;
    int i;

    interpret_foo(ins, &position, par);

    printf("New position: %d\n", position);
    for (i=0; i<position; i++){
        printf("Instruction: %d, Type: %d, Value: %s\n", ins[i].operation, ins[i].type, ins[i].value);
    }
}

int interpret_foo(struct ffi_instruction ins[], int *position, struct nary_node *foo){
    struct ffi_instruction tmp = { foo->node_type, *position, (char *) foo->content };

    if(foo->nnode != 0){
        ins[*position] = tmp;
        (*position)++;
        interpret_foo(ins, position, foo->nodes[0]);
    } else {
        ins[*position] = tmp;
        (*position)++;
    }

    return 0;
}

int main(void){
    interpret();
}
