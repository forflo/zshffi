#include "ffi_nary_tree.h"
#include <stdlib.h>

/***************************************************************/
/* takes up 24Byte on a amd64 System */
/* Build with $ gcc -g -o test nary_tree.c -lcunit -DTEST */
int destroy_tree(struct nary_node *tree){
	int i;

	for(i=0; i<tree->nnode; i++){
		destroy_tree(tree->nodes[i]);
	}

	/* aktuellen Knoten zerstören */
	if(tree->content != NULL)
		free(tree->content);
	free(tree);
	return 0;
}

/* traverses a given nary_tree preorder
 	Param: node = The tree to traverse
 	Return 0 on success, -1 on failure */
int traverse_preorder(struct nary_node *node, 
						int (*callback)(void *node_ptr, void *userparam), 
						void *userparam){
	int i;

	if(-1 == callback(node, userparam)){
		return -1;
    }

	for(i=0; i<node->nnode; i++){
		traverse_preorder(node->nodes[i], callback, userparam);
	}

	return 0;
}

/* traverses a given nary_tree postorder
 	Param: node = The tree to traverse
 	Return 0 on success, -1 on failure */
int traverse_postorder(struct nary_node *node, 
						int (*callback)(void *node_ptr, void *userparam), 
						void *userparam){
	int i;
	for(i=0; i<node->nnode; i++){
		traverse_postorder(node->nodes[i], callback, userparam);
	}

	if(-1 == callback(node, userparam)){
		return -1;
    }

	return 0;
}

/* Initializes a node of an nary tree 
	Param: content = Voidpointer to arbitrary data used as content
	Return: Pointer to a valid nary_node structure or NULL if a failure
		occured */
struct nary_node *new_node(void *content){
	struct nary_node *r = malloc(sizeof(struct nary_node));
	if (r == NULL)
		return NULL;
	r->content = content;
	r->leaf = 1;
	r->nnode = 0;
	r->nodes = NULL;
	return r;
}

/* Deletes a node childs remain intact 
 	Param: node = The node to delete 
 	Return 0 on success */
int free_node(struct nary_node *node){
	int i;
	free(node->content);
	free(node->nodes);
	free(node);
	
	return 0;
}

/* Adds a new node to a given one. The new node will contain
 	the content content 
 	Param: parent = The node to add another to 
 	Return: 0 on success, -1 on failure */
int add_node(struct nary_node *parent, void *content){
	/* allocate memory for new node in the node vector */
	if(parent->nodes == NULL){
		parent->nodes = malloc(sizeof(struct nary_node *));
	} else {
		parent->nodes = realloc(parent->nodes, sizeof(struct nary_node *) * 
											(parent->nnode + 1));
	}

	if(parent->nodes == NULL){
		return -1;
    }

	/* increments number of nodes in parent. and
		appends the newly generated node */
	parent->nodes[parent->nnode++] = new_node(content);

	/* if parent was empty */
	if(parent->leaf)
		parent->leaf = 0;	

	return 0;	
}

/* Adds a new node to parent.
 	Param: parent = The parent node
 		child = The node to add to parent
 	Return -1 on failure, 0 on success */
int add_child(struct nary_node *parent, struct nary_node *child){
	/* allocate memory for new node in the node vector */
	if(parent->nodes == NULL){
		parent->nodes = (struct nary_node **) malloc(sizeof(struct 
													nary_node *));
	} else {
		parent->nodes = (struct nary_node **) realloc(parent->nodes, 
											sizeof(struct nary_node *) * 
											(parent->nnode + 1));
	}

	if(parent->nodes == NULL){
		return -1;
    }

	parent->nodes[parent->nnode++] = child;

	if(parent->leaf){
		parent->leaf = 0;
    }

	return 0;
}

